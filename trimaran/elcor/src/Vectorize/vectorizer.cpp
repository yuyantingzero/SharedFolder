/***********************************************************************

Copyright (c) 2007 Massachusetts Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

***********************************************************************/



//===========================================================================
//
//   FILE: vectorizer.cpp:
//   
//   Author: Sam Larsen
//   Date: Fri Mar 18 11:11:57 2005
//
//   Function:  Selective vectorizer
//
//===========================================================================

#include "vectorizer.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "opcode_load_store.h"
#include "matrix.h"
#include "el_normalize_branches.h"
#include "ir_writer.h"
#include "pred_analysis.h"
#include "el_dot_tools.h"

const int VEC_SCRATCH_PAD_SIZE = 1024;

// Data structures to hold mdes info for operations that won't appear
// in the instruction stream (e.g., vsmoves & svmoves)
Hash_map<Opcode,char*>* Vectorizer::misc_op_string = NULL;
Hash_map<Opcode,char*>* Vectorizer::misc_io_pat = NULL;

Vectorizer::Vectorizer(El_Loop* loop_in)
  : loop(loop_in),
    body(loop->loop_blocks.head()),
    num_ops(num_real_ops()),
    id_to_op(hash_int, 1021),
    op_to_id(hash_op_ptr, 1021),
    privatizable(hash_operand, 1021),
    invariant(hash_operand, 1021),
    vect_vr_num(hash_operand, 1021),
    scal_vr_num(hash_operand, 1021),
    adjMat(num_ops, num_ops, false),
    rmsl(current_MDES)
{
  //unl_fact = MDES_reg_vector_length("VIR");
  //assert(unl_fact == MDES_reg_vector_length("VFR"));
  // Replace the above for multi-cluster.  As in simu, assume all vector
  // register files have the same vector lengths.  -KF 1/2007
  unl_fact = 0;
  for (int i = 0; i < MDES_num_reg_types(); i++) {
    Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
    if (strstr(rd->get_vname(), vregfile_to_mdesname(VIR)) ||
        strstr(rd->get_vname(), vregfile_to_mdesname(VFR))) {
      int cur_vec_length = MDES_reg_vector_length(rd->get_aname());
      if (unl_fact == 0)
        unl_fact = cur_vec_length;
      else if (cur_vec_length != unl_fact)
        El_punt("Vector length mismatch!  Currently, vectorization assumes that all\n"
                "vector register files have the same vector length.");
    }
  }
  if (unl_fact == 0)
    El_punt("Vector length zero!  Perhaps you are running vectorization with an Mdes\n"
            "that does not contain vector register files?");

  num_vectorizable = 0;
  scratch_offs = 0;

  init_op_info();
  find_sig_operands();
  draw_dep_edges();
  find_vectorizable();

  if (num_vectorizable > 0)
  {
    switch (El_vectorize_model)
    {
      case 0: break; // unroll
      case 1: mostly_vectorize(); break;
      case 2: partition_ops(); break;
      case 3: opt_partition(); break;
      default: assert(0);
    }
    if (El_vectorize_print_ddg)
      print_ddg();

    transform_loop();
    print_vect_info();
  }
}


// Count the number of non-pseudo ops in the loop body
int Vectorizer::num_real_ops()
{
  // There should be a better way to get an op count
  int count = 0;
  for (Region_ops_C0_order ops1(body); ops1 != 0; ops1++)
    if (!is_pseudo(*ops1)) count++;

  return count;
}


// Find the operations that might be vectorized and initialize the
// op_info structure.  Also locate induction variables, the loop 
// condition and the loop branch.
void Vectorizer::init_op_info()
{
  assert(loop->primary_ind_var_ops.size() == 1);
  incr = loop->primary_ind_var_ops.head();
  init_stride = incr->src(SRC2).int_value();
  branch = El_get_branch_op(body);
  cond = El_get_cond_for_branch(branch);

  // Give each operation a unique identifier
  int count = 0;
  for (Region_ops_C0_order ops1(body); ops1 != 0; ops1++)
  {
    Op* op = *ops1;
    if (is_pseudo(op)) continue;

    id_to_op.bind(count, op);
    op_to_id.bind(op, count);
    count++;
  }

  // Initialize op_info
  op_info = new Op_info[num_ops];
  for (Region_ops_C0_order ops2(body); ops2 != 0; ops2++)
  {
    Op* op = *ops2;
    if (is_pseudo(op)) continue;

    // See if this is an induction variable for a memory address
    Op* mem_op = NULL;
    if (is_addr_ind_var(op, mem_op))
    {
      Op_info& mem_op_info = op_to_info(mem_op);

      // is the memop before or after its address increment?
      mem_op_info.before = op_to_id.value(mem_op) < op_to_id.value(op);
      mem_op_info.stride = op->src(SRC2).int_value();
      mem_op_info.aligned = !El_require_aligned_ops || is_aligned(mem_op);

      addr_ops.push_tail(op);
    }

    // Don't add branch-related ops to sig_ops
    else if (op != branch && op != cond)
      sig_ops.push_tail(op);
  }
}


Vectorizer::~Vectorizer()
{
  Hash_map_iterator<Operand,Vector<int>*> it1;
  for (it1(scal_vr_num); it1 != 0; it1++)
    delete (*it1).second;

  for (List_iterator<List<Op*>*> it2(sccs); it2 != 0; it2++)
    delete *it2;

  delete[] op_info;
}


// See if this op is an induction variable for a memop
bool Vectorizer::is_addr_ind_var(Op* ind_var_op, Op*& mem_op)
{
  mem_op = NULL;

  if (!loop->basic_ind_var_ops.is_member(ind_var_op))
    return false;

  if (!ind_var_op->src(SRC2).is_int())
    return false;

  Operand& dst = ind_var_op->dest(DEST1);
  for (Region_all_ops ops(body); ops != 0; ops++)
  {
    Op* op = *ops;
    if (op != ind_var_op)
    {
      for (Op_all_inputs srcs(op); srcs != 0; srcs++)
      {
	if (*srcs == dst)
	{
	  if (!is_memory(op)) return false;
	  if (mem_op != NULL) return false;
	  mem_op = op;
	}
      }
    }
  }

  return mem_op != NULL;
}


// Find privatizable and loop invariant operands.  Privatizable virtual
// regs are safely transformed into vector registers.  Loop invariants
// must be initialized before they can be used in a vector loop.
void Vectorizer::find_sig_operands()
{
  Hash_set<Operand> read(hash_operand, 1021);
  Hash_set<Operand> written(hash_operand, 1021);

  // privatizable
  for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)
  {
    Op* curr = *ops;

    for (Op_all_inputs srcs(curr); srcs != 0; srcs++)
      if ((*srcs).is_reg())
	read += *srcs;
    
    for (Op_all_dests dsts(curr); dsts != 0; dsts++)
    {
      Operand& dst = *dsts;
      written += dst;

      if (dst.is_reg() && !read.is_member(dst)) {
        // privatize only if op is guarded under true : parkhc 02/2007
        if (!curr->predicated() || curr->src(PRED1).is_predicate_true())
          privatizable += dst;
      }
    }
  }

  // invariant
  for (Hash_set_iterator<Operand> it(read); it != 0; it++)
    if (!written.is_member(*it))
      invariant += *it;
}


// Populate an adjacency matrix with dependences.  I can't figure out
// how to get proper dependence info from the existing infrastructure -- 
// I'll just do it myself.  Identifying dependences is simple here since 
// the loop body has no control flow.
void Vectorizer::draw_dep_edges()
{
  // Keep track of which regs have been written so far
  Hash_map<Operand,Op*> written(hash_operand, 1021);

  // Scalar flow dependences
  int i;
  for (i=0; i<2; i++) {
    for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)  {
      Op* curr = *ops;
      int curr_id = op_to_id.value(curr);
      for (Op_all_inputs srcs(curr); srcs != 0; srcs++) {
	Operand& src = *srcs;
	if (written.is_bound(src)) {
	  int id = op_to_id.value(written.value(src));
	  adjMat[id][curr_id] = true;
	  record_flow_dep(id, curr_id);
	}
      }
      for (Op_all_dests dsts(curr); dsts != 0; dsts++) {
	if (i==0) written.bind(*dsts, curr);
	else written.unbind(*dsts);
      }
    }
  }
  assert(written.size() == 0);

  // Scalar output dependences
  for (i=0; i<2; i++) {
    for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)  {
      Op* curr = *ops;
      int curr_id = op_to_id.value(curr);
      for (Op_all_dests dsts(curr); dsts != 0; dsts++) {
	Operand& dst = *dsts;
	// No loop-carried dependences for privatizable vars
	bool do_check = i==0 || !privatizable.is_member(dst);
	if (written.is_bound(dst) && do_check) {
	  int id = op_to_id.value(written.value(dst));
	  adjMat[id][curr_id] = true;
	}
	if (i==0) written.bind(dst, curr);
	else written.unbind(dst);
      }
    }
  }
  assert(written.size() == 0);

  // Anti dependences
  for (i=0; i<2; i++) {
    for (Dlist_iterator<Op*> ops(sig_ops, true); ops != 0; ops--) {
      Op* curr = *ops;
      int curr_id = op_to_id.value(curr);
      for (Op_all_inputs srcs(curr); srcs != 0; srcs++) {
	Operand& src = *srcs;
	// No loop-carried dependences for privatizable vars
	bool do_check = i==0 || !privatizable.is_member(src);
	if (written.is_bound(src) && do_check) {
	  int id = op_to_id.value(written.value(src));
	  adjMat[curr_id][id] = true;
	}
      }
      for (Op_all_dests dsts(curr); dsts != 0; dsts++)
      {
	if (i==0) written.bind(*dsts, curr);
	else written.unbind(*dsts);
      }
    }
  }
  assert(written.size() == 0);

  // Memory dependences
  for (i=0; i<2; i++) {
    for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)  {
      Op* curr = *ops;
      int curr_id = op_to_id.value(curr);
      for (Op_mem_sources srcs(curr); srcs != 0; srcs++) {
	Operand& src = *srcs;
	if (written.is_bound(src)) {
	  int id = op_to_id.value(written.value(src));
	  adjMat[id][curr_id] = true;
	}
      }
      for (Op_mem_dests dsts(curr); dsts != 0; dsts++) {
	if (i==0) written.bind(*dsts, curr);
	else written.unbind(*dsts);
      }
    }
  }
  assert(written.size() == 0);
}


// Record predecessor and successor info for scalar flow dependences.
// This is used for determining when explicit communication is required
// between scalar and vector ops.
void Vectorizer::record_flow_dep(int src, int sink)
{
  // Could use sets instead of lists, but...
  // first check to see if there's already an edge
  for (List_iterator<int> it(op_info[sink].pred_ids); it != 0; it++) {
    if (*it == src) return;
  }

  op_info[sink].pred_ids.add_tail(src);
  op_info[src].succ_ids.add_tail(sink);
}


// Identify vectorizable operations using the technique described by
// Allen & Kennedy.  The difference here is that we're operating on a 
// low-level IR.  The only real difficulty, however, is handling address
// calculations correctly.  Addresses for vectorizable memory operations 
// should have been strength reduced and replaced by an induction 
// variable.  These are ignored in the analysis and updated appropriately
// whenever the associated memop is vectorized.
void Vectorizer::find_vectorizable()
{
  int ID = 0;
  Vector<int> value(num_ops, 0);
  List<int> stack;

  // Find strongly connected components
  for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)
  {
    int k = op_to_id.value(*ops);
    if (!value[k])
      visit(k, ID, value, stack);
  }
}

int Vectorizer::visit(int k, int& ID, Vector<int>& value, List<int>& stack)
{
  value[k] = ++ID;
  int min = ID;

  stack.push(k);

  for (int i=0; i<num_ops; i++)
  {
    if (adjMat[k][i])
    {
      int m = value[i] ? value[i] : visit(i, ID, value, stack);
      if (m < min) min = m;
    }
  }

  if (min == value[k])
  {
    int index;
    List<Op*>* scc = new List<Op*>;
    do
    {
      index = stack.pop();
      value[index] = num_ops + 1;
      scc->push(id_to_op.value(index));
    }
    while (index != k);

    sccs.push(scc);
    if (is_vectorizable(scc))
    {
      op_to_info(scc->head()).can_vectorize = true;
      num_vectorizable++;
    }
  }

  return min;
}

// Is this strongly-connected component vectorizable?
bool Vectorizer::is_vectorizable(List<Op*>* scc)
{
  if (scc->size() != 1)
    return false;

  Op* op = scc->head();
  int id = op_to_id.value(op);

  // Are we only vectorizing floating-point ops?
  if (El_vectorize_fp_only && !is_float(op))
    return false;

  // Self dependent ops aren't vectorizable
  if (adjMat[id][id] == true)
    return false;

  // See if there is an appropriate vector opcode
  if (!el_opcode_to_vector_opcode_map.is_bound(op->opcode()))
  {
    //cerr << "\n*-*-* No vector opcode: " << op->opcode() << " *-*-*\n\n";
    return false;
  }

  // Memops must have unit stride
  if (is_memory(op) && op_to_info(op).stride != data_type_width(op))
    return false;

  // Don't try to vectorize ops with sources that aren't regs or constants
  Operand_2_iter<Op_all_inputs,Op_all_dests> opers;
  for (opers(op); opers != 0; opers++)
  {
    Operand& oper = *opers;
    if (oper.is_macro_reg() || oper.is_string() || oper.is_cb())
      return false;

    // don't vectorize if operand is not either privatizable or invariant
    // parkhc 02/2007
    if (oper.is_reg() && !privatizable.is_member(oper) && !invariant.is_member(oper))
      return false;

 }

  // Don't vectorize ops not guarded under true.  It doesn't seem to work
  // properly -- it requires the use of mask registers, which seem to be
  // unimplemented.  -KF 2/2007
  if (op->predicated() && !op->src(PRED1).is_predicate())
    return false;

  
  // Phew.  Must be vectorizable.
  return true;
}


bool Vectorizer::is_float(Op* op)
{
  if (is_falu(op)) return true;

  IR_ROOT_OPCODE t = get_root(op->opcode());
  return (t == ROOT_FLD ||
	  t == ROOT_FST ||
	  t == ROOT_FLG ||
	  t == ROOT_FSG);
}


int Vectorizer::data_type_width(Op* op)
{
  // use opcode's own data type width instead of operand's width
  // parkhc 02/2007
  return opcode_data_width(op) / 8;
}

DType Vectorizer::dtype(Op* op)
{
  if (op->num_dests() == 1)
    return op->dest(DEST1).dtype();

  if (is_store(op))
  {
    assert(op->num_srcs() >= 2);
    return op->src(SRC2).dtype();
  }

  return EL_STANDARD_INT;
}


// Transform the loop for unrolling or selective vectorization.  This
// consists of generating the new loop body, a cleanup loop, and the
// various tests to determine when to execute each loop.
void Vectorizer::transform_loop()
{
  assert(unl_fact > 1);

  Edge* pred_edge = get_nonloop_inedge(body);
  Edge* succ_edge = get_nonloop_outedge(body);

  Op* pred = pred_edge->src();
  Op* succ = succ_edge->dest();
  pred->remove_outedge_recursively(pred_edge);
  succ->remove_inedge_recursively(succ_edge);

  Basicblock* loop_head = build_loop_head();
  Basicblock* main_loop = build_main_loop();
  Basicblock* post_loop = build_post_loop();
  Basicblock* main_test = build_main_test(post_loop);
  Basicblock* post_test = build_post_test(succ->parent());

  printf("BB %4d -> %4d   ", body->id(), main_loop->id());

  Compound_region* orig = body;
  Compound_region* parent = body->parent();
  if (parent->is_hb())
  {
    orig = parent;
    parent = parent->parent();
  }

  loop_head->set_parent(parent);
  main_loop->set_parent(parent);
  post_loop->set_parent(parent);
  main_test->set_parent(parent);
  post_test->set_parent(parent);

  // Create the control-flow edges between blocks
  Op* head_entry = (Op*)loop_head->first_subregion();
  Op* head_exit = (Op*)loop_head->last_subregion();

  Op* main_entry = (Op*)main_loop->first_subregion();
  Op* main_exit = (Op*)main_loop->last_subregion();

  Op* post_entry = (Op*)post_loop->first_subregion();
  Op* post_exit = (Op*)post_loop->last_subregion();

  Op* tst1_entry = (Op*)main_test->first_subregion();
  Op* tst1_exit = (Op*)main_test->last_subregion();

  Op* tst2_entry = (Op*)post_test->first_subregion();
  Op* tst2_exit = (Op*)post_test->last_subregion();

  connect(pred, tst1_entry, true);
  connect(tst1_exit, post_entry);
  connect(tst1_exit, head_entry, true);
  connect(head_exit, main_entry, true);
  connect(main_exit, tst2_entry, true);
  connect(main_exit, main_entry);
  connect(tst2_exit, succ);
  connect(tst2_exit, post_entry, true);
  connect(post_exit, succ, true);
  connect(post_exit, post_entry);

  // Replace the old loop body with the new loops
  parent->insert_before_region(main_test, orig);
  parent->insert_before_region(loop_head, orig);
  parent->insert_before_region(main_loop, orig);
  parent->insert_before_region(post_test, orig);
  parent->insert_before_region(post_loop, orig);

  parent->remove_region(orig);
}


// Function to construct the main vectorized loop
Basicblock* Vectorizer::build_main_loop()
{
  // Create a new basic block
  Basicblock* nbody = new Basicblock;
  nbody->weight = body->weight;

  int curr_prim_vr = incr->dest(DEST1).vr_num();
  bool primary_used = primary_used_in_loop();

  // Insert a c_merge as the first instruction
  Op* merge = new Op(C_MERGE);
  insert_op(merge, nbody);
  nbody->add_entry(merge);

  // Main loop to build all new ops
  for (List_iterator<List<Op*>*> it(sccs); it != 0; it++)
  {
    List<Op*>* scc = *it;

    bool vectorize = op_to_info(scc->head()).do_vectorize;
    int f = vectorize ? 1 : unl_fact;

    // Unroll factor
    for (int i=0; i<f; i++)
    {
      // Add the ops in this scc in original C0 order. (This list is
      // created in C0 order)
      for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)
      {
	Op* op = *ops;
	if (scc->is_member(op))
	{
	  // no need to insert multiple increments to the primary ind_var
	  // if it's only used in the branch condition.
	  if (op == incr && !primary_used && i < unl_fact-1)
	    continue;

	  Op* new_op;
	  if (vectorize)
	    new_op = insert_new_vector_op(op, nbody);
	  else
	    new_op = insert_new_scalar_op(op, i, nbody);

	  if (is_memory(new_op))
	    add_offset_to_memop(new_op, op_to_info(op), i);

	  // Adjust the loop so the primary induction vr is incremented
	  // once.  Also, fix the stride to reflect the unroll factor.
	  adjust_for_primary(new_op, curr_prim_vr, i);
	}
      }
    }
  }

  // Insert address increments with updated strides
  for (Dlist_iterator<Op*> addrs(addr_ops); addrs != 0; addrs++)
  {
    Op* op = copy_op(*addrs);
    Operand& stride = op->src(SRC2);
    stride.set_int_value(stride.int_value() * unl_fact);
    insert_op(op, nbody);
  }

  // Update scratch pad pointer
  if (scratch_offs > 0)
  {
    Op* incr = make_new_op(ADDL_W, scratch_inc->clone(),
			   scratch_inc->clone(),
			   new Int_lit(scratch_offs));
    Op* mask = make_new_op(AND_W, scratch_inc->clone(),
			   scratch_inc->clone(),
			   new Int_lit(VEC_SCRATCH_PAD_SIZE-1));
    Op* init = make_new_op(ADDL_W, scratch_ptr->clone(),
			   new Macro_reg(VEC_SCRATCH_PAD),
			   scratch_inc->clone());
    insert_op(incr, nbody);
    insert_op(mask, nbody);
    insert_op(init, nbody);
  }

  // Insert the cond op and update the upper bound
  Op* new_cond = copy_op(cond);
  insert_op(new_cond, nbody);
  Operand new_ub(ub_reg);
  new_cond->set_src(SRC2, new_ub);

  // Insert the branch and update the target
  Op* new_branch = copy_op(branch);
  insert_op(new_branch, nbody);
//   assert(new_branch->src(SRC1).is_cb());
//   assert(new_branch->src(SRC1).id() == body->id());
//   new_branch->src(SRC1).set_id(nbody->id());
  // Changed to insert PBR.  -KF 1/2007
  Op* pbr_op = el_insert_pbr(new_branch, nbody, true);
  new_branch->set_src(SRC1, pbr_op->dest(DEST1));

  nbody->add_exit(new_branch);

  // Rename mem_vrs and update memory dependences
  update_mem_vrs(nbody);

  return nbody;
}


// Create and insert a vector version of op
Op* Vectorizer::insert_new_vector_op(Op* op, Basicblock* nbody)
{
  Op* new_op;

  if (is_store(op))
    new_op = insert_vector_store(op, nbody);

  else if (is_load(op))
    new_op = insert_vector_load(op, nbody);

  else
  {
    new_op = new_vector_op(op);
    insert_op(new_op, nbody);
  }

  // Communicate operands to scalar consumers if necessary
  if (has_scalar_flow_deps(op))
    insert_v2s_xfr(op, nbody);

  return new_op;
}

bool Vectorizer::has_scalar_flow_deps(Op* op)
{
  for (List_iterator<int> it(op_to_info(op).succ_ids); it != 0; it++)
    if (!op_info[*it].do_vectorize)
      return true;

  return false;
}

Op* Vectorizer::insert_vector_store(Op* op, Basicblock* nbody)
{
  Op* new_op = new_vector_op(op);

  // Insert a permute before misaligned stores
  if (!op_to_info(op).aligned)
  {
    assert(!op->src(SRC2).is_lit());
    Operand src = new_op->src(SRC2);
    new_op->src(SRC2).rename();
    Operand dst = new_op->src(SRC2);
    Op* perm = new_vec_perm(op, dst, src);
    insert_op(perm, nbody);
  }

  insert_op(new_op, nbody);
  return new_op;
}

Op* Vectorizer::insert_vector_load(Op* op, Basicblock* nbody)
{
  Op* new_op = new_vector_op(op);
  insert_op(new_op, nbody);

  // Insert a permute after misaligned loads
  if (!op_to_info(op).aligned)
  {
    Operand dst = new_op->dest(DEST1);
    new_op->dest(DEST1).rename();
    Operand src = new_op->dest(DEST1);
    Op* perm = new_vec_perm(op, dst, src);
    insert_op(perm, nbody);
  }

  return new_op;
}


// Create and insert a new scalar version of op, corresponding to 
// unroll iteration iter.  iter determines how scalar virtual regs
// are renamed
Op* Vectorizer::insert_new_scalar_op(Op* op, int iter, Basicblock* nbody)
{
  Op* new_op = new_scalar_op(op, iter);
  insert_op(new_op, nbody);

  // Communicate operands to vector consumers if necessary
  if (iter == unl_fact-1 && has_vector_flow_deps(op))
    insert_s2v_xfr(op, nbody);

  return new_op;
}

bool Vectorizer::has_vector_flow_deps(Op* op)
{
  for (List_iterator<int> it(op_to_info(op).succ_ids); it != 0; it++)
    if (op_info[*it].do_vectorize)
      return true;

  return false;
}


// Memops with known stride will have their address incremented
// once in the unrolled loop.  We need to update the opcode to
// use base + offset addressing and attach the offset.
//
// Modifying the vector opcodes is only necessary because of an 
// implementation artifact.  It's easiest to put all address
// increment ops at the end of the loop.  For memops that should
// have their address incremented before referencing, we can
// use an offset instead.
void Vectorizer::add_offset_to_memop(Op* new_op, Op_info& info, int iter)
{
  if (info.stride > 0)
  {
    assert(is_memory(new_op));

    int offset = (iter + (info.before ? 0 : 1)) * info.stride;
    if (offset > 0)
    {
      if (is_mem_base_plus_offset(new_op)) {
        // Have to handle case where the op is already base+offset.  -KF 1/2007

        Operand& src2 = new_op->src(SRC2);

        if (src2.is_lit()) {

          // If the operand is a literal, calculate the new offset here and change
          // the operand accordingly.

          if (!src2.is_int())
            El_punt("Vectorizer::add_offset_to_memop(): cannot handle non-int literal in op %d.",
                    new_op->id());
          int old_offset = src2.int_value();
          Operand off_opnd(new Int_lit(old_offset + offset));
          new_op->set_src(SRC2, off_opnd);

        } else {

          // If the operand is a register, have to insert an ADD op to calculate
          // the new offset at runtime.

          if (!src2.is_reg() && !src2.is_macro_reg())
            El_punt("Vectorizer::add_offset_to_memop(): cannot handle SRC2 of op %d",
                    new_op->id());
          if (!src2.dtype().is_int())
            El_punt("Vectorizer::add_offset_to_memop(): offset of op %d is not an int.",
                    new_op->id());

          Op* add_op = new Op(ADD_W);
          Operand tmp_opnd(new Reg(src2.dtype()));
          Operand off_opnd(new Int_lit(offset));
          add_op->set_src(PRED1, new_op->src(PRED1));
          add_op->set_src(SRC1, src2);
          add_op->set_src(SRC2, off_opnd);
          add_op->set_dest(DEST1, tmp_opnd);
          El_insert_op_before(add_op, new_op);
          new_op->set_src(SRC2, tmp_opnd);
        }
      } else {
        Opcode_load_store opc = FLG_D_C1_C1;
        switch (new_op->opcode()) {
          case FL_D_C1_C1:  opc = FLG_D_C1_C1; break;
          case FL_S_C1_C1:  opc = FLG_S_C1_C1; break;
          case L_W_C1_C1:   opc = LG_W_C1_C1;  break;
          case L_H_C1_C1:   opc = LG_H_C1_C1;  break;
          case L_B_C1_C1:   opc = LG_B_C1_C1;  break;
          case LX_H_C1_C1:  opc = LGX_H_C1_C1; break;
          case LX_B_C1_C1:  opc = LGX_B_C1_C1; break;
          case FS_D_C1:     opc = FSG_D_C1;    break;
          case FS_S_C1:     opc = FSG_S_C1;    break;
          case S_W_C1:      opc = SG_W_C1;     break;
          case S_H_C1:      opc = SG_H_C1;     break;
          case S_B_C1:      opc = SG_B_C1;     break;

          case VFL_D_C1_C1: opc = VFLG_D_C1_C1; break;
          case VFL_S_C1_C1: opc = VFLG_S_C1_C1; break;
          case VL_W_C1_C1:  opc = VLG_W_C1_C1;  break;
          case VL_H_C1_C1:  opc = VLG_H_C1_C1;  break;
          case VL_B_C1_C1:  opc = VLG_B_C1_C1;  break;
          case VFS_D_C1:    opc = VFSG_D_C1;    break;
          case VFS_S_C1:    opc = VFSG_S_C1;    break;
          case VS_W_C1:     opc = VSG_W_C1;     break;
          case VS_H_C1:     opc = VSG_H_C1;     break;
          case VS_B_C1:     opc = VSG_B_C1;     break;	
	  
          default:
            El_punt("Vectorizer does not support memory opcode %s.",
                    (char*)el_opcode_to_string_map.value(new_op->opcode()));
        }

        new_op->set_opcode((Opcode)opc);
        new_op->update_num_operands();

        if (new_op->num_srcs() > 3)
          new_op->set_src(SRC4, new_op->src(SRC3));

        if (new_op->num_srcs() > 2)
          new_op->set_src(SRC3, new_op->src(SRC2));

        Operand off(new Int_lit(offset));
        new_op->set_src(SRC2, off);
      }
    }
  }
}


// Returns true if the primary induction variable is referenced by ops
// other the the increment and the branch condition.
bool Vectorizer::primary_used_in_loop()
{
  Operand primary = incr->dest(DEST1);

  for (Region_all_ops ops(body); ops != 0; ops++)
  {
    Op* op = *ops;
    if (op == incr || op == cond) continue;

    for (Op_all_inputs ins(op); ins != 0; ins++)
      if (*ins == primary)
	return true;
  }

  return false;
}


// In order for the loop to be modulo scheduled, the primary ind_var
// must be incremented exactly once.  The last increment in the loop
// is updated to reflect the unroll factor.  The other increments
// are changed to increment a temp vr instead
void Vectorizer::adjust_for_primary(Op* new_op, int& curr_vr, int iter)
{
  // See if this is the primary ind var increment
  Operand& prim = incr->dest(DEST1);
  if (new_op->num_dests() == 1 && new_op->dest(DEST1) == prim)
  {
    // use the original vr number on the last iteration
    if (iter == unl_fact-1)
    {
      curr_vr = prim.vr_num();
    }
    else
    {
      Operand& dst = new_op->dest(DEST1);
      dst.rename();
      curr_vr = dst.vr_num();
    }

    // update the stride
    Operand& stride = new_op->src(SRC2);
    assert(stride.is_int());
    stride.set_int_value(stride.int_value() * (iter+1));
  }

  // If not, replace references to the primary
  else for (Op_all_inputs srcs(new_op); srcs != 0; srcs++)
  {
    Operand& src = *srcs;
    if (src == prim)
      src.set_vr_num(curr_vr);
  }
}


// Construct the cleanup loop
Basicblock* Vectorizer::build_post_loop()
{
  Basicblock* post = new Basicblock;
  post->weight = body->weight;

  for (Region_ops_C0_order it(body); it != 0; it++)
    insert_op(copy_op(*it), post);

  Op* merge = (Op*)post->first_subregion();
  Op* branch = (Op*)post->last_subregion();

  post->add_entry(merge);
  post->add_exit(branch);

  // fix the branch target
//   assert(branch->src(SRC1).is_cb());
//   branch->src(SRC1).set_id(post->id());
  // Changed to support PBR.  -KF 1/2007
  Op* pbr_op = El_find_pbr_for_branch(branch);
  assert(pbr_op->src(SRC1).is_cb());
  pbr_op->src(SRC1).set_id(post->id());

  // Rename mem_vrs and update memory dependences
  update_mem_vrs(post);

  // Don't modulo schedule the post loop
  post->set_flag(EL_DONT_SWP);

  return post;
}


// If pre is true, create a test to see if we should skip the main loop 
// and jump directly to the post-loop.  If pre is false, create a test
// to see if we should skip the post loop.
Basicblock* Vectorizer::build_loop_test(Compound_region* br_targ, bool pre)
{
  Basicblock* test = new Basicblock;
  test->weight = body->weight;

  Op* entry = new Op(C_MERGE);
  insert_op(entry, test);
  test->add_entry(entry);

  Op* cmp = copy_op(cond);
  cmp->dest(DEST1).rename();

  if (pre)
  {
    Op* add = copy_op(incr);
    add->dest(DEST1).rename();
    // get a proper increment for test: parkhc 02/2007
    int increment = (unl_fact - 1) * init_stride;
    Operand lit(new Int_lit(increment));
    add->set_src(SRC2, lit);
    insert_op(add, test);
    cmp->set_src(SRC1, add->dest(DEST1));
  }

  cmp->set_opcode(get_opposite_cond(cmp->opcode()));
  insert_op(cmp, test);

  Op* jmp = copy_op(branch);
//   jmp->src(SRC1).set_id(br_targ);
  // Changed to insert PBR.  -KF 1/2007
  jmp->set_src(SRC2, cmp->dest(DEST1));
  insert_op(jmp, test);
  Op* pbr_op = el_insert_pbr(jmp, br_targ, true);
  jmp->set_src(SRC1, pbr_op->dest(DEST1));

  test->add_exit(jmp);

  return test;
}


// This block initializes the new upper bound of the vector loop
// and initializes vectors for loop invariants
Basicblock* Vectorizer::build_loop_head()
{
  Basicblock* head = new Basicblock;
  head->weight = body->weight;

  Op* entry = new Op(C_MERGE);
  insert_op(entry, head);
  head->add_entry(entry);

  // Build vectors for loop invariants
  Hash_set<Operand> bcast_opers(hash_operand, 9);
  for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)
  {
    Op* op = *ops;
    if (op_to_info(op).do_vectorize)
    {
      for (Port_num j=op->first_src(); j <= op->last_src(); j++)
      {
	// Leave addresses alone
	if (j == SRC1 && is_memory(op)) continue;

	Operand& src = op->src(j);
	if (invariant.is_member(src))
	  bcast_opers += src;
      }
    }
  }

  // Initialize vectors with the splat instruction
  for (Hash_set_iterator<Operand> it(bcast_opers); it != 0; it++)
  {
    Opcode opc;
    DType dtype = (*it).dtype();

    if (dtype.is_int())
      opc = VSPLAT;
    else if (dtype.is_single())
      opc = VFSPLAT_S;
    else if (dtype.is_double())
      opc = VFSPLAT_D;
    else
      assert(0);

    Op* splat = make_new_op(opc, new_vector_reg(*it), 
			    new_scalar_reg(*it,0),
			    new Macro_reg(VEC_MASK_TRUE));
    insert_op(splat, head);
  }

  // Make new upper bound
  Operand& upper = cond->src(SRC2);
  ub_reg = new Reg(upper.dtype());
  // take the original increment into account when
  // calculating new upper bound
  int increment = (unl_fact - 1) * init_stride;
  Op* sub = make_new_op(SUB_W, ub_reg->clone(),
			upper.get_ptr()->clone(),
			new Int_lit(increment));
  insert_op(sub, head);

  // Initialize pointer for accessing vector scratch pad
  if (need_scratch_pad())
  {
    scratch_ptr = new Reg(EL_STANDARD_INT);
    scratch_inc = new Reg(EL_STANDARD_INT);
    Op* ptr = make_new_op(MOVE, scratch_ptr, new Macro_reg(VEC_SCRATCH_PAD));
    Op* inc = make_new_op(MOVE, scratch_inc, new Int_lit(0));
    insert_op(ptr, head);
    insert_op(inc, head);
  }

  // dummy exit op
  Op* exit = new Op(DUMMY_BR);
  insert_op(exit, head);
  head->add_exit(exit);

  return head;
}


// The scratch pad pointer is needed if there is communication
// between scalar and vector operations
bool Vectorizer::need_scratch_pad()
{
  if (El_comm_through_mem)
  {
    for (int i=0; i<num_ops; i++)
    {
      Op* op = id_to_op.value(i);
      if (op_info[i].do_vectorize) {
	if (has_scalar_flow_deps(op)) return true;
      }
      else {
	if (has_vector_flow_deps(op)) return true;
      }
    }
  }

  return false;
}


// Insert an operation into a basic block
void Vectorizer::insert_op(Op* new_op, Basicblock* block)
{
  new_op->set_parent(block);

  // empty block
  if (((Region*)block)->subregions().size() == 0)
    block->add_region(new_op);

  // insert and connect to previous op
  else
  {
    Op* prev_op = (Op*)block->last_subregion();
    block->insert_after_region(new_op, prev_op);
    C0_connect_fallthrough(prev_op, new_op);
  }
}


// Control-flow from op1 to op2
void Vectorizer::connect(Op* op1, Op* op2, bool fallthrough)
{
  assert(op1 != NULL && op2 != NULL);

  Edge* edge;
  if (fallthrough)
    edge = C0_connect_fallthrough(op1, op2);
  else
    edge = C0_connect(op1, op2);

  Control_flow_freq* cf_freq = new Control_flow_freq;
  cf_freq->freq = 0.0;
  cf_freq->cc = 0;
  set_control_flow_freq(edge, cf_freq);

  op1->parent()->add_exit(edge);
  op2->parent()->add_entry(edge);

  // if there's a second level hb, add the edge to entry/exit list
  // parkhc 01/2007
  Compound_region* second_parent1 = op1->parent()->parent();
  Compound_region* second_parent2 = op2->parent()->parent();

  if (second_parent1 != second_parent2) {
    if (second_parent1->is_hb())
      second_parent1->add_exit(edge);
    if (second_parent2->is_hb())
      second_parent2->add_entry(edge);
  }
}


// Get the incoming edge to this block that's not the loop backedge
Edge* Vectorizer::get_nonloop_inedge(Basicblock* block)
{
  Op* merge = (Op*)block->first_subregion(); 
  Op* branch = (Op*)block->last_subregion();

  assert(is_control_merge(merge));
  assert(is_branch(branch));

  Edge* edge = NULL;
  for (Op_inedges ins(merge, CONTROL0_INEDGES); ins != 0; ins++)
  {
    Edge* tmp = *ins;
    if (tmp->src() != branch)
    {
      if (edge != NULL)
	El_punt("block %d has too many inedges\n", block->id());
      edge = tmp;
    }
  }

  return edge;
}


// Get the outgoing edge from the block that's not the loop backedge
Edge* Vectorizer::get_nonloop_outedge(Basicblock* block)
{
  Op* merge = (Op*)block->first_subregion(); 
  Op* branch = (Op*)block->last_subregion();

  assert(is_control_merge(merge));
  assert(is_branch(branch));

  Edge* edge = NULL;
  for (Op_outedges outs(branch, CONTROL0_OUTEDGES); outs != 0; outs++)
  {
    Edge* tmp = *outs;
    if (tmp->dest() != merge)
    {
      assert(edge == NULL);
      edge = tmp;
    }
  }

  return edge;
}


// Duplicate an operation
Op* Vectorizer::copy_op(Op* old_op)
{
  Op* new_op = new Op;
  int id = new_op->id();
  *new_op = *old_op;
  new_op->set_id(id);
  return new_op;
}


// Create a new scalar operation corresponding to unroll iteration 'iter'.
// Iter is needed because scalar instructions are unrolled in the vector
// loop.  Scalar operands are renamed in each unrolled version.
Op* Vectorizer::new_scalar_op(Op* old_op, int iter)
{
  Op* new_op = copy_op(old_op);

  Operand_2_iter<Op_all_inputs,Op_all_dests> opers;
  for (opers(new_op); opers != 0; opers++)
  {
    Operand& oper = *opers;
    if (oper.is_reg())
      oper = new_scalar_reg(oper, iter);
  }

  return new_op;
}


// Create a new vector operation from old_op
Op* Vectorizer::new_vector_op(Op* old_op)
{
  Op* new_op = copy_op(old_op);

  assert(el_opcode_to_vector_opcode_map.is_bound(old_op->opcode()));
  int vec_opc = el_opcode_to_vector_opcode_map.value(old_op->opcode());
  new_op->set_opcode((Opcode)vec_opc);
  new_op->update_num_operands();

  // Change dest to use vector reg file
  for (Port_num i=new_op->first_dest(); i <= new_op->last_dest(); i++)
  {
    Operand& dst = new_op->dest(i);
    if (dst.is_reg())
    {
      Operand vect_dst(new_vector_reg(dst));
      new_op->set_dest(i, vect_dst);
    }
  }

  // Change sources to use vector reg file
  for (Port_num j=new_op->first_src(); j <= new_op->last_src(); j++)
  {
    // Leave address operand scalar
    if (j == SRC1 && is_memory(new_op)) continue;

    // Leave disp operand scalar : parkhc 01/2007
    if (j == SRC2 && is_mem_base_plus_offset(old_op)) continue;

    Operand& src = new_op->src(j);
    if (src.is_reg())
    {
      Operand vect_src(new_vector_reg(src));
      new_op->set_src(j, vect_src);
    }
  }

  // Predicates get turned into vector masks
  assert(new_op->predicated());
  Operand& pred = new_op->src(PRED1);
  if (pred.is_predicate_true())
  {
    Operand mask(new Macro_reg(VEC_MASK_TRUE));
    new_op->set_src(new_op->last_src(), mask);
  }
  else
  {
    Operand mask(new_vector_reg(pred));
    new_op->set_src(new_op->last_src(), mask);
    Operand ptrue(new Pred_lit(true));
    new_op->set_src(PRED1, ptrue);
  }

  return new_op;
}


// Create a new scalar register from oper (which is a register).  
// Create a mapping from the original vr_num to the new identifier
// based on unroll iteration
Reg* Vectorizer::new_scalar_reg(Operand& oper, int iter)
{
  // there should be no rotating registers at this point
  assert(oper.is_reg() && oper.is_static());

  int vr_num;

  // Communication between vector and scalar ops happens through privatizable
  // virtual registers.  Since each of the scalar regs involved must have a
  // unique name, privatizable vrs must be renamed in each unrolled section.
  if (privatizable.is_member(oper))
  {
    if (!scal_vr_num.is_bound(oper))
    {
      int vr_num = virtual_register_number;

      Vector<int>* tmp = new Vector<int>(unl_fact);
      scal_vr_num.bind(oper, tmp);
      int i;
      for (i=0; i<unl_fact-1; i++)
	(*tmp)[i] = vr_num++;

      // use original vr_num on last iteration
      (*tmp)[i] = oper.vr_num();

      adjust_vr_num(vr_num);
    }

    vr_num = (*scal_vr_num.value(oper))[iter];
  }
  else vr_num = oper.vr_num();

  return new Reg(oper.dtype(), vr_num);
}


// Create a new vector register from oper
Reg* Vectorizer::new_vector_reg(Operand& oper)
{
  // there should be no rotating registers at this point
  assert(oper.is_reg());
  assert(oper.is_static());

  // new register should have a vector dtype.  -KF 1/2007
  DType_info dti;
  if (oper.dtype().is_int())
    dti = EL_DT_INFO_VEC_INT;
  else if (oper.dtype().is_float())
    dti = EL_DT_INFO_VEC_FLOAT;
  else if (oper.dtype().is_predicate())
    dti = EL_DT_INFO_VEC_MASK;
  else
    El_punt("Vectorizer::new_vector_reg(): Unknown dtype.");

  DType new_dtype(oper.dtype().width(), oper.dtype().is_signed(), dti);
  Reg* new_reg;
  if (!vect_vr_num.is_bound(oper))
  {
    new_reg = new Reg(new_dtype);
    vect_vr_num.bind(oper, new_reg->vr_num());
  }
  else
  {
    int vr_num = vect_vr_num.value(oper);
    new_reg = new Reg(new_dtype, vr_num);
  }

  // change to use a vector register file
  switch (oper.file_type())
  {
    case GPR: new_reg->bind_file(VIR, STATIC_REG); break;
    case FPR: new_reg->bind_file(VFR, STATIC_REG); break;
    case PR:  new_reg->bind_file(VMR, STATIC_REG); break;
    default: assert(0);
  }

  return new_reg;
}


bool Vectorizer::is_aligned(Op* op)
{
  assert(is_memory(op));

  // Meh, this is ugly.  I'm not enforcing alignment for stores of
  // literals.  Why?  Mostly because it's easier to insert permute ops
  // this way, but one could also argue that they're not needed in
  // the first place.
  if (is_store(op) && op->src(SRC2).is_lit())
    return true;

  // Check for an attribute originally attached in suif.
  Lcode_attribute_map* map = get_lcode_attributes(op);
  assert(map);

  for (Map_iterator<eString, List<Operand> > it(*map); it != 0; it++)
  {
    Pair<eString, List<Operand> > attr = *it;
    if (attr.first == "aligned")
      return true;
  }

  return false;
}


Op* Vectorizer::new_vec_perm(Op* op, Operand& dst, Operand& src)
{
  assert(is_memory(op));

  Opcode opc = get_perm_opcode(dst.dtype());
  Op* new_op = new Op(opc);

  Operand pred(new Pred_lit(true));
  new_op->set_src(PRED1, pred);

  Operand mask(new Macro_reg(VEC_MASK_TRUE));
  new_op->set_src(SRC2, mask);

  new_op->set_src(SRC1, src);
  new_op->set_dest(DEST1, dst);

  return new_op;
}


Opcode Vectorizer::get_perm_opcode(DType dtype)
{
  if (dtype.is_int())
    return VPERM;
  else if (dtype.is_single())
    return VFPERM_S;
  else if (dtype.is_double())
    return VFPERM_D;
  else
    El_punt("Vectorizer::get_perm_opcode(): unknown type: %d\n", dtype.info());

  return (Opcode)0;
}


// Insert operations to transfer op's result from a vector register
// to a group of scalar registers.  I'm currently supporting two
// communication models: a series of vsmove operations, or a store
// followed by scalar loads.  I can't find a clean way to support both
//  models -- specialize based on the El_comm_through_mem flag
void Vectorizer::insert_v2s_xfr(Op* op, Basicblock* body)
{
  Operand reg = op->dest(DEST1);

  // If requested, transfer operands through memory
  if (El_comm_through_mem)
  {
    // If this op is a load, there's no point in performing the 
    // communication.  Just load directly to the both sides.
    if (is_load(op))
    {
      for (int i=0; i<unl_fact; i++)
      {
	Op* load = new_scalar_op(op, i);
	add_offset_to_memop(load, op_to_info(op), i);
	insert_op(load, body);
      }
    }
    // Otherwise write to scratch pad space and then load it 
    // on the scalar side
    else
    {
      Opcode load_opc, store_opc;

      if (reg.dtype().is_int()) {
        load_opc  = (Opcode)LG_W_C1_C1;
        store_opc = (Opcode)VSG_W_C1;
      } else if (reg.dtype().is_single()) {
        load_opc  = (Opcode)FLG_S_C1_C1;
        store_opc = (Opcode)VFSG_S_C1;
      } else if (reg.dtype().is_double()) {
        load_opc  = (Opcode)FLG_D_C1_C1;
        store_opc = (Opcode)VFSG_D_C1;
      } else {
        El_punt("Vectorizer::new_v2s_xfr(): unimplemented type");
      }

      // write vector register to memory
      Op* store = make_new_op(store_opc, NULL, 
			      scratch_ptr->clone(),
			      new Int_lit(scratch_offs),
			      new_vector_reg(reg),
			      new Macro_reg(VEC_MASK_TRUE));
      insert_op(store, body);

      // take care of memory dependences
      Operand mem_vr(new Mem_vr);
      store->add_mem_dest(mem_vr);
    
      // read scalar values from memory
      int size = reg.data_type_width() / 8;
      for (int i=0; i<unl_fact; i++)
      {
	Op* load = make_new_op(load_opc, new_scalar_reg(reg, i),
			       scratch_ptr->clone(),
			       new Int_lit(i*size + scratch_offs));
	insert_op(load, body);
	load->add_mem_src(mem_vr);
      }

      // update scratch space pointer
      scratch_offs += size * unl_fact;
    }
  }

  // Otherwise use vsmove opcodes
  else
  {
    Opcode opc;
    if (reg.dtype().is_int())
      opc = VSMOVE;    
    else if (reg.dtype().is_single())
      opc = VSFMOVE_S;
    else if (reg.dtype().is_double())
      opc = VSFMOVE_D;
    else
      El_punt("Vectorizer::new_v2s_xfr(): unimplemented type");

    for (int i=0; i<unl_fact; i++)
    {
      Op* new_op = make_new_op(opc, new_scalar_reg(reg, i), 
			       new_vector_reg(reg), 
			       new Int_lit(i));
      insert_op(new_op, body);
    }
  }
}


// Insert operations to transfer the results of each unrolled version
// of op to a vector register
void Vectorizer::insert_s2v_xfr(Op* op, Basicblock* body)
{
  Operand reg = op->dest(DEST1);

  // If requested, transfer operands through memory
  if (El_comm_through_mem)
  {
    // If this op is a vectorizable load, there's no point in performing 
    // the communication.  Just load directly to both sides.
    if (is_load(op) && op_to_info(op).can_vectorize)
    {
      Op* load = insert_vector_load(op, body);
      add_offset_to_memop(load, op_to_info(op), 0);
    }
    // otherwise write to scratch pad space and load it on the
    // vector side
    else
    {
      Opcode load_opc, store_opc;
      if (reg.dtype().is_int()) {
        load_opc  = (Opcode)VLG_W_C1_C1;
        store_opc = (Opcode)SG_W_C1;
      } else if (reg.dtype().is_single()) {
        load_opc  = (Opcode)VFLG_S_C1_C1;
        store_opc = (Opcode)FSG_S_C1;
      } else if (reg.dtype().is_double()) {
        load_opc  = (Opcode)VFLG_D_C1_C1;
        store_opc = (Opcode)FSG_D_C1;
      } else {
        cerr << *op << endl;
        El_punt("Vectorizer::insert_s2v_xfr(): unimplemented type");
      }

      // memory dependences
      Operand mem_vr(new Mem_vr);
    
      // write scalar values to memory
      int size = reg.data_type_width() / 8;
      for (int i=0; i<unl_fact; i++)
      {
	Op* store = make_new_op(store_opc, NULL,
				scratch_ptr->clone(),
				new Int_lit(i*size + scratch_offs),
				new_scalar_reg(reg, i));
	insert_op(store, body);
	store->add_mem_dest(mem_vr);
      }

      // load vector from memory
      Op* load = make_new_op(load_opc, new_vector_reg(reg),
			     scratch_ptr->clone(),
			     new Int_lit(scratch_offs),
			     new Macro_reg(VEC_MASK_TRUE));
      insert_op(load, body);
      load->add_mem_src(mem_vr);

      // update scratch space pointer
      scratch_offs += size * unl_fact;
    }
  }

  // Otherwise use svmove opcodes
  else
  {
    Opcode opc;
    if (reg.dtype().is_int())
      opc = SVMOVE; 
    else if (reg.dtype().is_single())
      opc = SVFMOVE_S;
    else if (reg.dtype().is_double())
      opc = SVFMOVE_D;
    else {
      cerr << *op << endl;
      El_punt("Vectorizer::insert_s2v_xfr(): unimplemented type");
    }

    for (int i=0; i<unl_fact; i++)
    {
      Op* new_op = make_new_op(opc, new_vector_reg(reg), 
			       new_scalar_reg(reg, i),
			       new Int_lit(i));
      insert_op(new_op, body);
    }
  }
}


// Create a new operation with specified opcode and operands
Op* Vectorizer::make_new_op(Opcode opc, Base_operand* dst,
			 Base_operand* src1, Base_operand* src2,
			 Base_operand* src3, Base_operand* src4)
{
  Op* new_op = new Op(opc);
  Operand pred(new Pred_lit(true));
  new_op->set_src(PRED1, pred);

  if (src1 != NULL)
  {
    Operand opnd(src1);
    new_op->set_src(SRC1, opnd);
  }
  if (src2 != NULL)
  {
    Operand opnd(src2);
    new_op->set_src(SRC2, opnd);
  }
  if (src3 != NULL)
  {
    Operand opnd(src3);
    new_op->set_src(SRC3, opnd);
  }
  if (src4 != NULL)
  {
    Operand opnd(src4);
    new_op->set_src(SRC4, opnd);
  }
  if (dst != NULL)
  {
    Operand opnd(dst);
    new_op->set_dest(DEST1, opnd);
  }

  return new_op;
}


// Update memory dependence information after vectorizing
void Vectorizer::update_mem_vrs(Basicblock* block)
{
  Hash_map<int,List<int>*> mem_vr_map(hash_int, num_ops);

  // Build a map from old vr_nums to new vr_nums.  There may be multiple
  // entries for a given vr due to unrolling.
  for (Region_all_ops ops1(block); ops1 != 0; ops1++)
  {
    for (Op_mem_dests dsts(*ops1); dsts != 0; dsts++)
    {
      Operand& dst = *dsts;
      int old_num = dst.vr_num();

      dst.rename();
      int new_num = dst.vr_num();

      List<int>* list;
      if (mem_vr_map.is_bound(old_num))
	list = mem_vr_map.value(old_num);
      else {
	list = new List<int>;
	mem_vr_map.bind(old_num, list);
      }

      list->add_tail(new_num);
    }
  }

  // Reassign mem_vr sources
  for (Region_all_ops ops2(block); ops2 != 0; ops2++)
  {
    Op* op = *ops2;
    List<Operand> to_delete;

    for (Op_mem_sources srcs(op); srcs != 0; srcs++) {
      Operand& src = *srcs;
      int num = src.vr_num();
      if (mem_vr_map.is_bound(num))
        to_delete.add_tail(src);
    }
    for (List_iterator<Operand> li(to_delete); li != 0; li++) {
      Operand& src = *li;
      int num = src.vr_num();
      op->remove_mem_src(src);
      for (List_iterator<int> it(*(mem_vr_map.value(num))); it != 0; it++)
      {
        Operand new_src(new Mem_vr(*it));
        op->add_mem_src(new_src);
      }
    }
  }

  // de-allocate mem_vr_map
  for (Hash_map_iterator<int,List<int>*> it(mem_vr_map); it != 0; it++)
    delete (*it).second;
}


// Vectorize ops only if they have at least one pred or succ that can
// also be vectorized.  (or they have no preds or succs)
void Vectorizer::mostly_vectorize()
{
  for (int i=0; i<num_ops; i++)
  {
    bool vec = false;

    Op_info& info = op_info[i];
    if (info.can_vectorize)
    {
      // Vectorize if there are no preds or succs
      if (info.succ_ids.size() == 0 && info.pred_ids.size() == 0)
	vec = true;

      if (!vec)
      {
	for (List_iterator<int> it1(info.succ_ids); it1 != 0; it1++)
	{
	  int k = *it1;
	  if (op_info[k].can_vectorize)
	  {
	    assert(i != k);
	    vec = true;
	    break;
	  }
	}
      }

      if (!vec)
      {
	for (List_iterator<int> it2(info.pred_ids); it2 != 0; it2++)
	{
	  int k = *it2;
	  if (op_info[k].can_vectorize)
	  {
	    assert(i != k);
	    vec = true;
	    break;
	  }
	}
      }
    }

    info.do_vectorize = vec;
  }
}


// Partition operations using the 2-cluster partitioning heuristic described
// in the Micro paper.  The heuristic is based on the papers by
// Fidducia & Matheyses and Kernighan & Lin.
void Vectorizer::partition_ops()
{
  assert(unl_fact > 1);
  init_mdes_info();

  // start with a completely vector partitioning
  Vector<bool> curr_partition(num_ops, false);
  place_all_ops(curr_partition);

  double best_fval;
  int best_ival = rmsl.value(best_fval);

  // Switch every vectorizable op one by one, at each step choosing
  // the op that minimizes cost.  Record the best configuration seen,
  // starting with that on the next iteration.  Iterate until no
  // improvement is made.
  int last_ival = best_ival;
  double last_fval = best_fval;

  Vector<bool> best_partition = curr_partition;
  for (passes=1; true; passes++)
  {
    Vector<bool> locked(num_ops, false);
    for (int i=0; i<num_vectorizable; i++)
    {
      double fval;
      int ival = switch_one_op(curr_partition, locked, fval);

      if (ival < best_ival || (ival == best_ival && fval < best_fval))
      {
	best_ival = ival;
	best_fval = fval;
	best_partition = curr_partition;
      }
    }

    if (best_ival == last_ival && best_fval == last_fval) break;

    last_ival = best_ival;
    last_fval = best_fval;
    curr_partition = best_partition;
  }

  for (int i=0; i<num_ops; i++)
    op_info[i].do_vectorize = best_partition[i];
}


// Choose one operation to move to the other partition
int Vectorizer::switch_one_op(Vector<bool>& curr_partition,
			   Vector<bool>& locked, double& fval_out)
{
  place_all_ops(curr_partition);
  rmsl.checkpoint();

  int best_id = -1;
  int best_ival = infinity;
  double best_fval = ELCOR_MAX_DOUBLE;

  for (int i=0; i<num_ops; i++)
  {
    Op_info& info = op_info[i];
    if (info.vect_op_string && !locked[i])
    {
      if (curr_partition[i])
	try_scalar_alloc(i, curr_partition);
      else try_vector_alloc(i, curr_partition);

      double fval;
      int ival = rmsl.value(fval);
      rmsl.restore();

      if (ival < best_ival || (ival == best_ival && fval < best_fval))
      {
	best_ival = ival;
	best_fval = fval;
	best_id = i;
      }
    }
  }

  assert(best_id >= 0);
  curr_partition[best_id] = !curr_partition[best_id];
  locked[best_id] = true;

  fval_out = best_fval;
  return best_ival;
}


// Release this operation's vector resources and reserve scalar resources
void Vectorizer::try_scalar_alloc(int id, Vector<bool>& curr_partition)
{
  assert(curr_partition[id] == true);
  Op_info& info = op_info[id];

  // Remove the vector op
  rmsl.remove_io(info.vect_op_string, info.vect_io_pat);

  // Remove vector perm for unaligned memops
  if (!info.aligned) adjust_perm_usage(info.dtype, -1);

  // Add the scalar ops
  for (int i=0; i<unl_fact; i++)
    rmsl.nextop_io(info.scal_op_string, info.scal_io_pat);

  // Account for transfer ops
  for (List_iterator<int> it(info.pred_ids); it != 0; it++)
  {
    int pred_id = *it;
    if (curr_partition[pred_id])
    {
      if (num_scal_consumers(pred_id, curr_partition) == 0)
	adjust_xfr_usage(true, pred_id, false);
    }
    else
    {
      if (num_vect_consumers(pred_id, curr_partition) == 1)
	adjust_xfr_usage(false, pred_id, true);
    }
  }

  if (num_vect_consumers(id, curr_partition) > 0)
    adjust_xfr_usage(false, id, false);

  if (num_scal_consumers(id, curr_partition) > 0)
    adjust_xfr_usage(true, id, true);
}


// Release this operation's scalar resources and reserve vector resources
void Vectorizer::try_vector_alloc(int id, Vector<bool>& curr_partition)
{
  assert(curr_partition[id] == false);
  Op_info& info = op_info[id];

  // Remove the scalar ops
  for (int i=0; i<unl_fact; i++)
    rmsl.remove_io(info.scal_op_string, info.scal_io_pat);

  // Add the vector op
  rmsl.nextop_io(info.vect_op_string, info.vect_io_pat);

  // Add vector perm for unaligned memops
  if (!info.aligned) adjust_perm_usage(info.dtype, 1);

  // Account for transfer ops
  for (List_iterator<int> it(info.pred_ids); it != 0; it++)
  {
    int pred_id = *it;
    if (curr_partition[pred_id])
    {
      if (num_scal_consumers(pred_id, curr_partition) == 1)
	adjust_xfr_usage(true, pred_id, true);
    }
    else
    {
      if (num_vect_consumers(pred_id, curr_partition) == 0)
	adjust_xfr_usage(false, pred_id, false);
    }
  }

  if (num_vect_consumers(id, curr_partition) > 0)
    adjust_xfr_usage(false, id, true);

  if (num_scal_consumers(id, curr_partition) > 0)
    adjust_xfr_usage(true, id, false);
}


// Bin-pack all operations in the loop based on the current partitioning
// so we can calculate the implied ResMII
void Vectorizer::place_all_ops(Vector<bool>& curr_partition)
{
  rmsl.reset();

  for (int i=0; i<num_ops; i++)
  {
    // cond is eliminated in swplined loops
    if (id_to_op.value(i) == cond) continue;

    Op_info& info = op_info[i];
    if (curr_partition[i])
    {
      rmsl.nextop_io(info.vect_op_string, info.vect_io_pat);

      if (!info.aligned)
	adjust_perm_usage(info.dtype, 1);

      if (num_scal_consumers(i, curr_partition) > 0)
	adjust_xfr_usage(true, i, false);
    }
    else
    {
      int num = info.do_not_unroll ? 1 : unl_fact;
      for (int j=0; j<num; j++)
	rmsl.nextop_io(info.scal_op_string, info.scal_io_pat);

      if (num_vect_consumers(i, curr_partition) > 0)
	adjust_xfr_usage(false, i, false);
    }
  }

  // account for scratch pad ops
  if (El_comm_through_mem)
    for (int i=0; i<3; i++)
      rmsl.nextop_io(op_to_info(incr).scal_op_string,
		     op_to_info(incr).scal_io_pat);
}


void Vectorizer::adjust_perm_usage(DType dtype, int num)
{
  Opcode opc = get_perm_opcode(dtype);
  char* op_string = misc_op_string->value(opc);
  char* io_pat = misc_io_pat->value(opc);

  assert(op_string && io_pat);

  if (num == 1)
    rmsl.nextop_io(op_string, io_pat);

  else if (num == -1)
    rmsl.remove_io(op_string, io_pat);

  else assert(0);
}


// Sigh - this is way too complicated, but I can't think of a good
// way to account for all the different sets of opcodes needed
// when transferring data.
void Vectorizer::adjust_xfr_usage(bool v2s, int src_id, bool remove)
{
  // This is only for measuring the degradation when
  // communication overhead is ignored by the partitioner
  if (El_ignore_comm) return;

  // If requested, transfer data through memory
  if (El_comm_through_mem)
  {
    // vector -> scalar
    if (v2s)
    {
      // If the src is a vectorizable load, can just load to both sides
      if (is_load(id_to_op.value(src_id)) && op_info[src_id].can_vectorize)
      {
        DType dtype = op_info[src_id].dtype;
        if (dtype.is_int())
          adjust_xfr_usage((Opcode)LG_W_C1_C1, unl_fact, remove);
        else if (dtype.is_single())
          adjust_xfr_usage((Opcode)FLG_S_C1_C1, unl_fact, remove);
        else if (dtype.is_double())
          adjust_xfr_usage((Opcode)FLG_D_C1_C1, unl_fact, remove);
        else
          assert(0);
      }
      else 
      {
        DType dtype = op_info[src_id].dtype;
        if (dtype.is_int()) {
          adjust_xfr_usage((Opcode)VSG_W_C1, 1, remove);
          adjust_xfr_usage((Opcode)LG_W_C1_C1, unl_fact, remove);
        } else if (dtype.is_single()) {
          adjust_xfr_usage((Opcode)VFSG_S_C1, 1, remove);
          adjust_xfr_usage((Opcode)FLG_S_C1_C1, unl_fact, remove);
        } else if (dtype.is_double()) {
          adjust_xfr_usage((Opcode)VFSG_D_C1, 1, remove);
          adjust_xfr_usage((Opcode)FLG_D_C1_C1, unl_fact, remove);
        } else {
          assert(0);
        }
      }
    }
    // scalar -> vector
    else
    {
     // If the op is a vectorizable load, can xfr to both sides
      if (is_load(id_to_op.value(src_id)) && op_info[src_id].can_vectorize)
      {
        DType dtype = op_info[src_id].dtype;
        if (dtype.is_int())
          adjust_xfr_usage((Opcode)VLG_W_C1_C1, 1, remove);
        else if (dtype.is_single())
          adjust_xfr_usage((Opcode)VFLG_S_C1_C1, 1, remove);
        else if (dtype.is_double())
          adjust_xfr_usage((Opcode)VFLG_D_C1_C1, 1, remove);
        else
          assert(0);

	// Permutes for misaligned vector loads
	if (!op_info[src_id].aligned)
	{
	  Opcode perm = get_perm_opcode(op_info[src_id].dtype);
	  adjust_xfr_usage(perm, 1, remove);
	}
      }
      else
      {
        DType dtype = op_info[src_id].dtype;
        if (dtype.is_int()) {
          adjust_xfr_usage((Opcode)SG_W_C1, unl_fact, remove);
          adjust_xfr_usage((Opcode)VLG_W_C1_C1, 1, remove);
        } else if (dtype.is_single()) {
          adjust_xfr_usage((Opcode)FSG_S_C1, unl_fact, remove);
          adjust_xfr_usage((Opcode)VFLG_S_C1_C1, 1, remove);
        } else if (dtype.is_double()) {
          adjust_xfr_usage((Opcode)FSG_D_C1, unl_fact, remove);
          adjust_xfr_usage((Opcode)VFLG_D_C1_C1, 1, remove);
        } else {
          assert(0);
        }
      }
    }
  }

  // Otherwise, use svmove / vsmove ops
  else
  {
    // vector -> scalar
    if (v2s)
    {
      DType dtype = op_info[src_id].dtype;
      if (dtype.is_int())
        adjust_xfr_usage(VSMOVE, unl_fact, remove);
      else if (dtype.is_single())
        adjust_xfr_usage(VSFMOVE_S, unl_fact, remove);
      else if (dtype.is_double())
        adjust_xfr_usage(VSFMOVE_D, unl_fact, remove);
      else
        assert(0);
    }
    // scalar -> vector
    else
    {
      DType dtype = op_info[src_id].dtype;
      if (dtype.is_int())
        adjust_xfr_usage(SVMOVE, unl_fact, remove);
      else if (dtype.is_single())
        adjust_xfr_usage(SVFMOVE_S, unl_fact, remove);
      else if (dtype.is_double())
        adjust_xfr_usage(SVFMOVE_D, unl_fact, remove);
      else
        assert(0);
    }
  }

}


void Vectorizer::adjust_xfr_usage(Opcode opcode, int num, bool remove)
{
  char* op_string = misc_op_string->value(opcode);
  char* io_pat = misc_io_pat->value(opcode);

  if (remove)
  {
    for (int i=0; i<num; i++)
      rmsl.remove_io(op_string, io_pat);
  }
  else
  {
    for (int i=0; i<num; i++)
      rmsl.nextop_io(op_string, io_pat);
  }
}


// How many consumers of op# id are scalar (b=false) or vector (b=true)
int Vectorizer::num_consumers(int id, Vector<bool>& curr_partition, bool b)
{
  int num = 0;
  Op_info& info = op_info[id];
  for (List_iterator<int> it(info.succ_ids); it != 0; it++)
    if (curr_partition[*it] == b)
      num++;

  return num;
}


void Vectorizer::opt_partition()
{
  init_mdes_info();

  Vector<bool> partition(num_ops, false);
  Vector<bool> best = partition;

  int best_ival = infinity;
  double best_fval = ELCOR_MAX_DOUBLE;

  long long max = 1 << num_vectorizable;
  for (long long i=0; i<max; i++)
  {
    long long k = 0;
    for (long long j=0; j<num_ops; j++)
      if (op_info[j].can_vectorize)
	partition[j] = (((long long)1 << k++) & i) != 0;

    place_all_ops(partition);

    double fval;
    int ival = rmsl.value(fval);

    if (ival < best_ival || (ival == best_ival && fval < best_fval))
    {
      best_ival = ival;
      best_fval = fval;
      best = partition;
    }
  }

  for (int j=0; j<num_ops; j++)
    op_info[j].do_vectorize = best[j];
}


// Store away each operation's mdes info so we can look it up quickly
// during partitioning.  Vectorizable operations will have info for
// scalar and vector allocations.
void Vectorizer::init_mdes_info()
{
  init_op_mdes_info();
  init_misc_mdes_info();
}


void Vectorizer::init_op_mdes_info()
{
  for (Dlist_iterator<Op*> it1(sig_ops); it1 != 0; it1++)
    init_op_mdes_info(*it1, *it1 == incr);

  for (Dlist_iterator<Op*> it2(addr_ops); it2 != 0; it2++)
    init_op_mdes_info(*it2, true);

  init_op_mdes_info(branch, true);
  init_op_mdes_info(cond, true);
}


void Vectorizer::init_op_mdes_info(Op* op, bool do_not_unroll)
{
  Op_info& info = op_to_info(op);
  info.do_not_unroll = do_not_unroll;
  info.dtype = dtype(op);

  assert(el_opcode_to_string_map.is_bound(op->opcode()));
  info.scal_op_string = el_opcode_to_string_map.value(op->opcode());
  op->build_io_pat(info.scal_io_pat);

  if (!do_not_unroll && op_to_info(op).can_vectorize)
  {
    Op* vec_op = new_vector_op(op);
    assert(el_opcode_to_string_map.is_bound(vec_op->opcode()));
    info.vect_op_string = el_opcode_to_string_map.value(vec_op->opcode());
    vec_op->build_io_pat(info.vect_io_pat);
    delete vec_op;
  }
}


// During partitioning, we will need to account for various ops that 
// aren't in the original instruction stream: scalar <-> vector transfer
// ops and permutes for misaligned memory ops.  Compute their IO patterns
// and store them in a data structure that can be accessed during
// bin packing.
void Vectorizer::init_misc_mdes_info()
{
  if (misc_op_string == NULL)
  {
    misc_op_string = new Hash_map<Opcode,char*>(hash_opcode, 9);
    misc_io_pat = new Hash_map<Opcode,char*>(hash_opcode, 9);

    // vector permutes
    init_misc_mdes_info(VPERM,
			new Reg(EL_STANDARD_VEC_INT, 0),
			new Reg(EL_STANDARD_VEC_INT, 0),
			new Macro_reg(VEC_MASK_TRUE));

    init_misc_mdes_info(VFPERM_S,
			new Reg(EL_STANDARD_VEC_FLOAT, 0),
			new Reg(EL_STANDARD_VEC_FLOAT, 0),
			new Macro_reg(VEC_MASK_TRUE));

    init_misc_mdes_info(VFPERM_D,
			new Reg(EL_STANDARD_VEC_DOUBLE, 0),
			new Reg(EL_STANDARD_VEC_DOUBLE, 0),
			new Macro_reg(VEC_MASK_TRUE));

    // scalar <-> vector transfer ops
    if (El_comm_through_mem)
    {
      // loads + stores for communicating through memory
      init_misc_mdes_info((Opcode)LG_W_C1_C1,
			  new Reg(EL_STANDARD_INT, 0),
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0));

      init_misc_mdes_info((Opcode)FLG_S_C1_C1,
			  new Reg(EL_STANDARD_FLOAT, 0),
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0));

      init_misc_mdes_info((Opcode)FLG_D_C1_C1,
			  new Reg(EL_STANDARD_DOUBLE, 0),
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0));

      init_misc_mdes_info((Opcode)VLG_W_C1_C1,
			  new Reg(EL_STANDARD_VEC_INT, 0),
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Macro_reg(VEC_MASK_TRUE));

      init_misc_mdes_info((Opcode)VFLG_S_C1_C1,
			  new Reg(EL_STANDARD_VEC_FLOAT, 0),
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Macro_reg(VEC_MASK_TRUE));

      init_misc_mdes_info((Opcode)VFLG_D_C1_C1,
			  new Reg(EL_STANDARD_VEC_DOUBLE, 0),
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Macro_reg(VEC_MASK_TRUE));

      init_misc_mdes_info((Opcode)SG_W_C1,
			  NULL,
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Reg(EL_STANDARD_INT, 0));

      init_misc_mdes_info((Opcode)FSG_S_C1,
			  NULL,
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Reg(EL_STANDARD_FLOAT, 0));

      init_misc_mdes_info((Opcode)FSG_D_C1,
			  NULL,
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Reg(EL_STANDARD_DOUBLE, 0));

      init_misc_mdes_info((Opcode)VSG_W_C1,
			  NULL,
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Reg(EL_STANDARD_VEC_INT, 0),
			  new Macro_reg(VEC_MASK_TRUE));

      init_misc_mdes_info((Opcode)VFSG_S_C1,
			  NULL,
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Reg(EL_STANDARD_VEC_FLOAT, 0),
			  new Macro_reg(VEC_MASK_TRUE));

      init_misc_mdes_info((Opcode)VFSG_D_C1,
			  NULL,
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0),
			  new Reg(EL_STANDARD_VEC_DOUBLE, 0),
			  new Macro_reg(VEC_MASK_TRUE));
    }
    else
    {
      // specialized svmove and vsmove ops
      init_misc_mdes_info(SVMOVE,
			  new Reg(EL_STANDARD_VEC_INT, 0),
			  new Reg(EL_STANDARD_INT, 0),
			  new Int_lit(0));

      init_misc_mdes_info(VSMOVE,
			  new Reg(EL_STANDARD_INT, 0),
			  new Reg(EL_STANDARD_VEC_INT, 0),
			  new Int_lit(0));

      init_misc_mdes_info(SVFMOVE_S,
			  new Reg(EL_STANDARD_VEC_FLOAT, 0),
			  new Reg(EL_STANDARD_FLOAT, 0),
			  new Int_lit(0));

      init_misc_mdes_info(VSFMOVE_S,
			  new Reg(EL_STANDARD_FLOAT, 0),
			  new Reg(EL_STANDARD_VEC_FLOAT, 0),
			  new Int_lit(0));

      init_misc_mdes_info(SVFMOVE_D,
			  new Reg(EL_STANDARD_VEC_DOUBLE, 0),
			  new Reg(EL_STANDARD_DOUBLE, 0),
			  new Int_lit(0));

      init_misc_mdes_info(VSFMOVE_D,
			  new Reg(EL_STANDARD_DOUBLE, 0),
			  new Reg(EL_STANDARD_VEC_DOUBLE, 0),
			  new Int_lit(0));
    }
  }
}


void Vectorizer::init_misc_mdes_info(Opcode opc, Base_operand* dest,
				  Base_operand* src1, Base_operand* src2,
				  Base_operand* src3, Base_operand* src4)
{
  Op* op = make_new_op(opc, dest, src1, src2, src3, src4);
  misc_op_string->bind(opc, el_opcode_to_string_map.value(opc));
  char* io_pat = new char[MAXIOPATLEN];
  op->build_io_pat(io_pat);
  misc_io_pat->bind(opc, io_pat);
}


void Vectorizer::print_ddg()
{
  char file[32];
  sprintf(file, "%d.dot", body->id());

  ofstream fout(file);
  fout << "digraph ddg {\n";

  for (Dlist_iterator<Op*> ops(sig_ops); ops != 0; ops++)
  {
    Op* op = *ops;
    int id = op_to_id.value(op);
    Opcode opc = op->opcode();
    fout << id << " [label=\"" << op->id() << ": " << opc << "\"";
    if (op_to_info(op).do_vectorize)
      fout << ",style=filled,color=\"#89C4FF\"";
    fout << "]\n";
  }

  for (int i=0; i<num_ops; i++)
  {
    for (int j=0; j<num_ops; j++)
    {
      if (adjMat[i][j])
      {
	fout << i << " -> " << j;
	if (op_info[i].do_vectorize != op_info[j].do_vectorize)
	  fout << " [style=bold,color=\"red\"]";
	fout << endl;
      }
    }
  }

  fout << "}\n";
  fout.close();
}


void Vectorizer::print_vect_info()
{
  int num = 0;

  Vector<bool> curr_partition(num_ops, false);
  for (int i=0; i<num_ops; i++) {
    if (op_info[i].do_vectorize)
    {
      curr_partition[i] = true;
      num++;
    }
  }

  init_mdes_info();
  place_all_ops(curr_partition);

  double best_fval;
  int best_ival = rmsl.value(best_fval);

  printf("( %3d of %3d )     ", num, num_vectorizable);
  printf("II = %3d     ", best_ival);
  printf("Most used: %s     ", rmsl.most_used());
  if (El_vectorize_model == 2) printf("Passes = %d", passes);
  printf("\n");
}
