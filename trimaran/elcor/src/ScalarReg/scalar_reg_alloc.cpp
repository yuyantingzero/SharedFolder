/******************************************************************************

                    SOFTWARE LICENSE AGREEMENT NOTICE
                   -----------------------------------

IT IS A BREACH OF THIS LICENSE AGREEMENT TO REMOVE THIS NOTICE FROM THE FILE
OR SOFTWARE, OR ANY MODIFIED VERSIONS OF THIS FILE OR SOFTWARE OR DERIVATIVE
WORKS.
___________________________________________________

Copyright Notices/Identification of Licensor(s) of Original Software
in the File


All rights reserved by the foregoing, respectively.
___________________________________________________

Copyright Notices/Identification of Subsequent Licensor(s)/Contributors of
Derivative Works

Copyright 2007 University of Michigan

All rights reserved by the foregoing, respectively.
___________________________________________________

The code contained in this file, including both binary and source [if released
by the owner(s)] (hereafter, Software) is subject to copyright by the
respective Licensor(s) and ownership remains with such Licensor(s).  The
Licensor(s) of the original Software remain free to license their respective
proprietary Software for other purposes that are independent and separate from
this file, without obligation to any party.

Licensor(s) grant(s) you (hereafter, Licensee) a license to use the Software
for academic, research and internal business purposes only, without a fee.
"Internal business purposes" means that Licensee may install, use and execute
the Software for the purpose of designing and evaluating products.  Licensee
may submit proposals for research support, and receive funding from private
and Government sponsors for continued development, support and maintenance of
the Software for the purposes permitted herein.

Licensee may also disclose results obtained by executing the Software, as well
as algorithms embodied therein.  Licensee may redistribute the Software to
third parties provided that the copyright notices and this License Agreement
Notice statement are reproduced on all copies and that no charge is associated
with such copies. No patent or other intellectual property license is granted
or implied by this Agreement, and this Agreement does not license any acts
except those expressly recited.

Licensee may modify the Software to make derivative works (as defined in
Section 101 of Title 17, U.S. Code) (hereafter, Derivative Works), as
necessary for its own academic, research and internal business purposes.
Title to copyrights and other proprietary rights in Derivative Works created
by Licensee shall be owned by Licensee subject, however, to the underlying
ownership interest(s) of the Licensor(s) in the copyrights and other
proprietary rights in the original Software.  All the same rights and licenses
granted herein and all other terms and conditions contained in this Agreement
pertaining to the Software shall continue to apply to any parts of the
Software included in Derivative Works.  Licensee's Derivative Work should
clearly notify users that it is a modified version and not the original
Software distributed by the Licensor(s).

If Licensee wants to make its Derivative Works available to other parties,
such distribution will be governed by the terms and conditions of this License
Agreement.  Licensee shall not modify this License Agreement, except that
Licensee shall clearly identify the contribution of its Derivative Work to
this file by adding an additional copyright notice to the other copyright
notices listed above, to be added below the line "Copyright
Notices/Identification of Subsequent Licensor(s)/Contributors of Derivative
Works."  A party who is not an owner of such Derivative Work within the
meaning of U.S. Copyright Law (i.e., the original author, or the employer of
the author if "work of hire") shall not modify this License Agreement or add
such party's name to the copyright notices above.

Each party who contributes Software or makes a Derivative Work to this file
(hereafter, Contributed Code) represents to each Licensor and to other
Licensees for its own Contributed Code that:

(a)  Such Contributed Code does not violate (or cause the Software to
violate) the laws of the United States, including the export control laws of
the United States, or the laws of any other jurisdiction.

(b)  The contributing party has all legal right and authority to make such
Contributed Code available and to grant the rights and licenses contained in
this License Agreement without violation or conflict with any law.

(c)  To the best of the contributing party's knowledge and belief, the
Contributed Code does not infringe upon any proprietary rights or intellectual
property rights of any third party.

LICENSOR(S) MAKE(S) NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE SOFTWARE
OR DERIVATIVE WORKS FOR ANY PURPOSE.  IT IS PROVIDED "AS IS"    WITHOUT
EXPRESS OR IMPLIED WARRANTY, INCLUDING BUT NOT LIMITED TO THE MERCHANTABILITY,
USE OR FITNESS FOR ANY PARTICULAR PURPOSE AND ANY WARRANTY AGAINST
INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHTS.  LICENSOR(S) SHALL NOT BE
LIABLE FOR ANY DAMAGES SUFFERED BY THE USERS OF THE SOFTWARE OR DERIVATIVE
WORKS.

Any Licensee wishing to make commercial use of the Software or Derivative
Works should contact each and every Licensor to negotiate an appropriate
license for such commercial use, and written permission of all Licensors will
be required for such a commercial license.  Commercial use includes (1)
integration of all or part of the source code into a product for sale by or on
behalf of Licensee to third parties, or (2) distribution of the Software or
Derivative Works to third parties that need it to utilize a commercial product
sold or licensed by or on behalf of Licensee.

By using or copying this Contributed Code, Licensee agrees to abide by the
copyright law and all other applicable laws of the U.S., and the terms of this
License Agreement.  Any individual Licensor shall have the right to terminate
this license immediately by written notice upon Licensee's breach of, or
non-compliance with, any of its terms.  Licensee may be held legally
responsible for any copyright infringement that is caused or encouraged by
Licensee's failure to abide by the terms of this License Agreement.

******************************************************************************/



/////////////////////////////////////////////////////////////////////////////
//
//      File:           scalar_reg_alloc.cpp
//      Authors:        Nate Clark (based on code written by Hansoo Kim
//                        and Aaron Erlandson)
//      Created:        December 2004
//      Description:    
//
/////////////////////////////////////////////////////////////////////////////


#include "scalar_reg_alloc.h"
#include "pred_analysis.h"
#include "spill_code_gen.h"
#include "el_control.h"
#include "el_bb_tools.h"
#include "ir_writer.h"
#include "el_opti_utilities.h"
#include "el_opti_dead_code.h"


Spill_stack Scalar_reg_alloc::spill_stack;


static void update_operand_list(List<Operand>& opnd_list, Hash_set<Operand>& vr_to_pr_set)
{
  for (List_iterator<Operand> opndi(opnd_list); opndi != 0; opndi++) {
    Operand& opnd = *opndi;
    if (vr_to_pr_set.is_member(opnd)) {
      Operand* bound_opnd = vr_to_pr_set.find(opnd);
      if (bound_opnd->is_reg() && bound_opnd->allocated()) {
        opnd.bind_physical_file(bound_opnd->physical_file_type());
        opnd.bind_reg(bound_opnd->mc_num());
      }
    }
  }
}


static void update_msched_attributes(Compound_region* r, Hash_set<Operand>& vr_to_pr_set)
{
  for (Region_subregions subi(r, LOOPBODY_FILTER); subi != 0; subi++) {
    LoopBody* loop = (LoopBody*)*subi;
    Msched_attr* ms_attr = get_msched_attr(loop);
    if (ms_attr) {
      update_operand_list(ms_attr->liveness_gen, vr_to_pr_set);
      update_operand_list(ms_attr->liveness_kill, vr_to_pr_set);
      update_operand_list(ms_attr->down_exposed_use_gen, vr_to_pr_set);
      update_operand_list(ms_attr->down_exposed_use_kill, vr_to_pr_set);
      update_operand_list(ms_attr->up_exposed_def_gen, vr_to_pr_set);
      update_operand_list(ms_attr->up_exposed_def_kill, vr_to_pr_set);
      update_operand_list(ms_attr->down_exposed_def_gen, vr_to_pr_set);
      update_operand_list(ms_attr->down_exposed_def_kill, vr_to_pr_set);
    }
  }
}


Scalar_reg_alloc::Scalar_reg_alloc(Procedure* proc)
{
  initialize();

  el_opti_eliminate_dead_code(proc, ANALYZE_ALLREG, true, true);

  el_flow_delete_liveness(proc);
  create_local_analysis_info_for_all_hbs_bbs(proc);
  el_flow_compute_liveness(proc, ANALYZE_ALLREG);
  El_do_reaching_defs(proc, ANALYZE_ALLREG);

  if(dbg(sa, 6)) {
    char buff[256];
    sprintf(buff, "%s_before_regalloc.reb", (char*)(proc->get_name()));
    IR_outstream dbg_out(buff);
    ir_write(dbg_out, proc);
  }

  reg_alloc(proc);
  spill_code_opti(proc);
  caller_save_opti(proc);

  generate_callee_save_code(proc);

  remove_reaching_defs_info(proc);
  delete_local_analysis_info_for_all_hbs_bbs(proc);
}


void Scalar_reg_alloc::initialize()
{
  spill_stack.clear();

  // FIX: reg_pool initialization should go in Main/el_init.cpp near
  // the opcode map initialization, but because of some dumb library
  // interdependences, doing that would make simu not compile (i think
  // el_init is in libgraph; ugh).
  reg_file_pool.initialize();
  vr_to_memvr.clear();
}


void Scalar_reg_alloc::reg_alloc(Compound_region* r)
{
  if (dbg(status, 1))
    cdbg << "(Reg Alloc) proc: " << r->id() << endl;

  List<Live_range*> lrs;
  Hash_set<Live_range*> to_delete(hash_live_range_ptr, 128);
  Hash_set<Operand> vr_to_pr_set(hash_operand_2, 10007);  // need to hash by VR

  // Predicate allocation creates new GPR vrs, so if predicate
  // allocation is done pefore GPR, then there will be unallocated
  // registers. This loop sorts the files to make sure that GPRs are
  // done last. Update: predicate allocation has to come before
  // control allocation, too, so the PV macros are bound.
  Vector<int> reg_files(MDES_num_reg_types());
  int head = 0, tail = reg_files.size() - 1;
  for(int i = 0; i < MDES_num_reg_types(); i++) {
    Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
    if(strstr(rd->get_vname(), vregfile_to_mdesname(PR)))
      reg_files[head++] = i;
    else
      reg_files[tail--] = i;
  }

  for(unsigned i = 0; i < reg_files.size(); i++) {

    Reg_descr* rd = current_MDES->get_MDES_reg_descr(reg_files[i]);
    if(rd->get_regkind() != REG_PHYSICAL)
      continue;

    if (rd->donot_allocate()) {
      cdbg << "  Skipping allocation for file: " << rd->get_aname() << endl;
      continue;
    }

    char* virt_file_name = rd->get_vname();
    eString phys_file_name = rd->get_aname();

    Reg_alloc_solver reg_solver(phys_file_name);

    // Add the live ranges for this phys file to the reg alloc solver
    lr_constr.clear();
    lrs = lr_constr.construct_new_live_ranges(r, phys_file_name);
    for(List_iterator<Live_range*> lri(lrs); lri != 0; ++lri) {
      Live_range* a_lr = *lri;
      reg_solver.add_live_range(a_lr);
      to_delete += a_lr;
      if(dbg(sa, 4)) {
	cdbg << "ADDING: " << *a_lr << endl;
      }
    }

    reg_solver.init_interference_graph();

    bool done = false;
    unsigned pass = 0;

    // The main iterative coloring loop. Keep coloring until no spill
    // code is generated.
    while( ! done) {
      done = true;
      pass++;
      if(dbg(status, 1))
	cdbg << "  Starting pass " << pass << " for file " << virt_file_name
	     << ":" << phys_file_name << endl;

      if(pass >= 20) {
	cdbg << "ERROR: Starting pass " << pass << " of register allocation.\n"
	     << "This looks like an infinite loop...\n" << endl;
	exit(1);
      }

      bool anything_spilled = reg_solver.process_coloring();

      if(anything_spilled) {
	if(dbg(sa, 6)) {
	  char buff[256];
	  sprintf(buff, "%s_after_ra_pass_%s_%d.reb",
		  (char*)(el_get_enclosing_procedure(r)->get_name()),
		  (char*)phys_file_name, pass);
	  IR_outstream dbg_out(buff);
	  ir_write(dbg_out, el_get_enclosing_procedure(r));
	}

	done = false;
	remove_caller_save_code(r, phys_file_name);
	update_live_ranges(r, reg_solver, to_delete);
	// Reset the register usage for this file. This prevents
	// unnecessary callee save code.
	reg_file_pool.get_reg_file(phys_file_name).clear_reg_use_count();
      } else {

        // This function is supposed to replace the predicate registers in 
        // a predicate spill op with its corresponding PV save op.
        // However, this is broken because the restore ops restore the
        // entire PV.

        //	if (strstr(virt_file_name, vregfile_to_mdesname(PR)))
        //	  bind_pr_spill(r);

	if(dbg(sa, 6)) {
	  char buff[256];
	  sprintf(buff, "%s_after_ra_pass_%s.reb",
		  (char*)(el_get_enclosing_procedure(r)->get_name()),
		  (char*)phys_file_name);
	  IR_outstream dbg_out(buff);
	  ir_write(dbg_out, el_get_enclosing_procedure(r));
	}
	if(dbg(status, 2))
	  cdbg << "    Done with " << virt_file_name << ":" << phys_file_name
	       << endl;
      }
    } // End while( ! done)

    for (List_iterator<Live_range*> lri(reg_solver.live_ranges()); lri != 0; lri++) {
      Live_range* lr = *lri;
      Operand& opnd = lr->get_oper();
      vr_to_pr_set += opnd;
    }

    // Free memory.
    for(Hash_set_iterator<Live_range*> hsi(to_delete); hsi != 0; hsi++) {
      delete *hsi;
    }
    to_delete.clear();

    reverse_macros(r, rd);

  } // End for each register file

  // This is sort of a hackish thing.  The problem is, msched attributes are
  // lists of unbound registers.  These virtual registers need to be bound to
  // the appropriate physical files and physical registers in order for dataflow
  // analysis to work after register allocation.  -KF 9/2006
  update_msched_attributes(r, vr_to_pr_set);
}


void Scalar_reg_alloc::update_live_ranges(Compound_region* r,
					  Reg_alloc_solver& ras,
					  Hash_set<Live_range*>& to_delete)
{
  List<Live_range*> remove_from_ras;

  // Mark which vrs were spilled for the live range constructor.
  for(List_iterator<Live_range*> li(ras.live_ranges()); li != 0; li++) {
    Live_range* lr = *li;
    if(lr->get_state() == SPILLED) {
      lr_constr.spilled(lr->get_oper().vr_num());
    }
  }

  // Add spill code to existing live ranges.
  put_spill_in_live_ranges(r, ras);

  // Construct live ranges for spill code.
  List<Live_range*> new_lrs =
    lr_constr.construct_new_live_ranges(r, ras.get_file(), false);

  for(List_iterator<Live_range*> li(ras.live_ranges()); li != 0; li++) {
    Live_range* lr = *li;

    // Remove spilled live ranges from the interference graph
    if(lr->get_state() == SPILLED) {
      lr->update_interference_graph();
      remove_from_ras.add_tail(lr);
    } else {
      // Remove the register bindings from the previous coloring.
      if(lr->get_oper().allocated() && ! lr->get_oper().is_rotating())
	lr->unbind();
    }
  }

  for(List_iterator<Live_range*> li(remove_from_ras); li != 0; li++) {
    Live_range* lr = *li;
    ras.remove_live_range(lr);
    // I could delete lr at this point in the code, but I would have
    // to remove it from the 'lrs' list in the function
    // ::reg_alloc. I can't think of a real clean way to do that, so
    // we'll live with the bigger memory footprint for the time being.
  }

  // Sometimes, new live ranges are made for the same operands as existing live
  // ranges (i.e. if some new spill code uses existing operands).  Combine the
  // corresponding live ranges together, otherwise code will be wrong.  -KF 1/2006
  // Don't mark these combined live ranges as unspillable -- they should be
  // spillable.  -KF 1/2006
  Hash_set<Live_range*> dont_mark_unspillable(hash_live_range_ptr, 32);
  for (List_iterator<Live_range*> new_li(new_lrs); new_li != 0; new_li++) {
    Live_range* new_lr = *new_li;
    Operand new_oper = new_lr->get_oper();
    for (List_iterator<Live_range*> old_li(ras.live_ranges());
         old_li != 0; old_li++) {
      Live_range* old_lr = *old_li;
      Operand old_oper = old_lr->get_oper();
      if (new_oper == old_oper) {
        new_lr->combine_with(old_lr);
        ras.remove_live_range(old_lr);
        dont_mark_unspillable += new_lr;
      }
    }
  }

  // Figure out which new live ranges interfere with each other.
  List<Live_range*> tmp_list;
  for (List_iterator<Live_range*> iter1(new_lrs); iter1 != 0; ++iter1) {
    Live_range* lr_ptr1 = *iter1;
    for (List_iterator<Live_range*> iter2(tmp_list); iter2 != 0; ++iter2) {
      Live_range* lr_ptr2 = *iter2;
      lr_ptr1->set_interference(lr_ptr2);
    }
    tmp_list.add_tail(lr_ptr1);
  }

  // Figure out which new live ranges interfere with old ones.
  for(List_iterator<Live_range*> li(ras.live_ranges()); li != 0; li++) {
    Live_range* lr = *li;

    for(List_iterator<Live_range*> li2(new_lrs); li2 != 0; li2++) {
      Live_range* new_lr = *li2;
      new_lr->set_interference(lr);
    }
  }
  
  for(List_iterator<Live_range*> li(new_lrs); li != 0; li++) {
    Live_range* new_lr = *li;
    if (dbg(sa, 4)) {
      cdbg << "NEWLY ADDED: " << *new_lr << endl;
    }
    if (!dont_mark_unspillable.is_member(new_lr))
      new_lr->set_unspillable();
    ras.add_live_range(*li);
    to_delete += new_lr;
  }
}


void Scalar_reg_alloc::put_spill_in_live_ranges(Compound_region* r,
						Reg_alloc_solver& ras)
{
  if(r->is_bb() || r->is_hb()) {
    Pred_jar pj(r);
    List<Op*> spill_ops;
    for(Region_ops_C0_order rops(r); rops != 0; rops++) {
      Op* op = *rops;
      if(op->flag(EL_OPER_SPILL_CODE)) {
	spill_ops.add_tail(op);
      } else {
	if( ! is_pseudo(op)) {

	  for(List_iterator<Live_range*> li(ras.live_ranges()); li != 0; li++) {
	    Live_range* lr = *li;
	    if(lr->in_range(op)) {
	      for(List_iterator<Op*> si(spill_ops); si != 0; si++) {
		Op* spill_op = *si;
		// FIX: Get the real predicate, instead of true. This
		// is irrelevant until we add predicate cognizant
		// interference calculation. Is this next line of code
		// correct? 
		// lr->add_op(spill_op, pj.get_lub_src_expression_guard(spill_op));
		lr->add_op(spill_op, Pred_jar::get_true());
	      }
	    }
	  }
	  spill_ops.clear();

	} // if ! pseudo
      } // else
    } // for all ops
    if ( ! spill_ops.is_empty()) {
      bool saw_a_real_op = false;
      Compound_region* reg = r;
      while( ! saw_a_real_op) {
	Op *last_op = get_last_region_op_from_subregions(reg);
	assert(is_dummy_branch(last_op));
	Edge *ft = get_outgoing_CONTROL0_fall_through_edge(last_op);
	Compound_region *ft_cr = ft->dest()->parent();
	// Get the op following c_merge
	for(Region_ops_C0_order op_iter(ft_cr, ft->dest()); op_iter != 0 && ! saw_a_real_op; op_iter++) {
	  Op* real_op = *op_iter;
	  if( ! is_pseudo(real_op) && ! real_op->flag(EL_OPER_SPILL_CODE)) {
	    saw_a_real_op = true;
	    for (List_iterator<Live_range*> li(ras.live_ranges()); li != 0; li++) {
	      Live_range* lr = *li;
	      if (lr->in_range(real_op)) {
		for (List_iterator<Op*> si(spill_ops); si != 0; si++) {
		  Op *spill_op = *si;
		  lr->add_op(spill_op, Pred_jar::get_true());
		}
	      }
	    }
	  }
        }
	reg = ft_cr;
      }
    }
  } else {
    for(Region_subregions subr(r); subr != 0; subr++) {
      put_spill_in_live_ranges((Compound_region*)*subr, ras);
    }
  }
}


void Scalar_reg_alloc::remove_caller_save_code(Compound_region* r,
					       const eString& phys_file)
{
  List<Op*> to_remove;
  List<Region*> regions;
  regions.add_tail(r);
  while( ! regions.is_empty()) {
    Compound_region* rg = (Compound_region*)regions.pop();
    if(rg->is_bb() || rg->is_hb()) {
      for(Region_ops_C0_order rops(rg); rops != 0; rops++) {
	Op* the_op = *rops;
	if(the_op->flag(EL_OPER_CALLER_SAVE)) {
	  Operand spilled;
	  if(is_load(the_op))
	    spilled = the_op->dest(DEST1);
	  else if(is_base_plus_offset_store(the_op))
	    spilled = the_op->src(SRC3);
	  else if(is_store(the_op))
	    spilled = the_op->src(SRC2);
	  else if(is_move(the_op))
	    spilled = the_op->dest(DEST1);
	  else 
	    continue;

	  // We only want to remove spill code for the file currently
	  // being allocated.
	  if(phys_file == spilled.physical_file_type())
	    to_remove.add_tail(the_op);
	}
      }
    } else {
      for(Region_subregions subr(rg); subr != 0; subr++) {
	regions.add_tail(*subr);
      }
    }
  }

  for(List_iterator<Op*> li(to_remove); li != 0; li++) {
    El_remove_op(*li);
    delete *li;
  }

  to_remove.clear();

  // There's no real good way to tell when stack adjustment (i.e. ADD)
  // operations can be removed, so after removing the loads and
  // stores, we do another pass over the code to look for dead code
  // adds marked caller save.
  regions.add_tail(r);
  List<Operand> defs;
  // It's ok to hash operands here, because all the ones I want to get
  // rid of won't be bound to physical registers.
  Hash_map<Operand, Op*> def_to_op(hash_operand, 128);
  Hash_set<Operand> uses(hash_operand, 128);
  while( ! regions.is_empty()) {
    Compound_region* rg = (Compound_region*)regions.pop();
    if(rg->is_bb() || rg->is_hb()) {
      defs.clear();
      def_to_op.clear();
      uses.clear();
      for(Region_ops_C0_order rops(rg); rops != 0; rops++) {
	Op* the_op = *rops;
	if(the_op->flag(EL_OPER_CALLER_SAVE)) {
	  Operand spilled;
	  if(is_add(the_op)) {
	    defs.add_tail(the_op->dest(DEST1));
	    def_to_op.bind(the_op->dest(DEST1), the_op);
	  } else if(is_memory(the_op))
	    uses += the_op->src(SRC1);
	}
      }

      for(List_iterator<Operand> di(defs); di != 0; di++) {
	if( ! uses.is_member(*di))
	  to_remove.add_tail(def_to_op[*di]);
      }
    } else {
      for(Region_subregions subr(rg); subr != 0; subr++) {
	regions.add_tail(*subr);
      }
    }
  }

  for(List_iterator<Op*> li(to_remove); li != 0; li++) {
    El_remove_op(*li);
    delete *li;
  }
}


///////////////////////////////////////////////////////////////
// Caller save code is generated for each reg live over a
// BRL. However, if a reg is live over 2 BRLs and isn't referenced
// between them, then the restore and store between those BRLs is
// unnecessary. This function removes those. E.g. if the code was
//
// op1  r1 = ...
// op2  ST r1, [stack]
// op3  BRL function_1
// op4  r1 = LD [stack]
// op5  ST r1, [stack]
// op6  BRL function_2
// op7  r1 = LD [stack]
// op8  ... = r1
//
// This function would get rid of ops 4 and 5. Note that this function
// only does peephole optimization; a better job could be done with
// global analysis. FIX: this doesn't work for predicates and it
// doesn't get rid of the address computation used in
// non-base+displacement architectures.

void Scalar_reg_alloc::caller_save_opti(Compound_region* r)
{
  if(r->is_bb() || r->is_hb()) {

    List<Op*> to_remove;
    Hash_map<Operand, Op*> oper_to_save(hash_operand, 64),
      oper_to_restore(hash_operand, 64);
    Hash_set<Operand> referenced(hash_operand, 128);
    Op* last_brl = NULL;
    for(Region_ops_C0_order rops(r); rops != 0; rops++) {
      Op* op = *rops;

      if(op->flag(EL_OPER_CALLER_SAVE)) {
	if(is_base_plus_offset_load(op))
	  oper_to_save.bind(op->dest(DEST1), op);
	else if(is_base_plus_offset_store(op))
	  oper_to_restore.bind(op->src(SRC3), op);
	else if(is_store(op))
	  oper_to_restore.bind(op->src(SRC2), op);
      } else {
	// Add references for this op.
	for(Op_all_dests dests(op); dests != 0; ++dests) {
	  El_ref cur_ref = dests.get_ref();
	  referenced += cur_ref.get_operand();
	}
	for(Op_all_inputs ins(op); ins != 0; ++ins) {
	  El_ref cur_ref = ins.get_ref();
	  referenced += cur_ref.get_operand();
	}
	if(r->is_exit_op(op)) {
	  oper_to_save.clear();
	  oper_to_restore.clear();
	}

      }

      if(is_brl(op)) {
	if(last_brl != NULL) {
	  for(Hash_map_iterator<Operand, Op*> hmi(oper_to_save); hmi != 0; hmi++) {
	    Operand& oper = (*hmi).first;
	    if(oper.file_type() != PR) {
	      if( ! referenced.is_member(oper)) {
		// Only remove them if both the save and restore were
		// found. I can't think of any reason why both wouldn't
		// be found, but just to protect against segfaults.
		if(oper_to_restore.is_bound(oper)) {
                  // FIX: this is overly conservative

                  // Do not use Operand== here for allocated operands.  If two
                  // predicates happen to be allocated to the same register,
                  // they will be == but this opti should not occur.  -KF 1/2006
                  Operand& pred_s = oper_to_save[oper]->src(PRED1);
                  Operand& pred_r = oper_to_restore[oper]->src(PRED1);
                  if (pred_s.is_reg() && pred_r.is_reg()) {
                    if (pred_s.equal_vr(pred_r)) {
                      to_remove.add_tail(oper_to_save[oper]);
                      to_remove.add_tail(oper_to_restore[oper]);
                    }
		  } else if (pred_s == pred_r) {
                    to_remove.add_tail(oper_to_save[oper]);
		    to_remove.add_tail(oper_to_restore[oper]);
                  }                    
		}
	      }
	    }
	  }
	}
	oper_to_save.clear();
	oper_to_restore.clear();
	referenced.clear();
	last_brl = op;
      } // is_brl

    }
   
    for(List_iterator<Op*> li(to_remove); li != 0; li++) {
      El_remove_op(*li);
      delete *li;
    }
  } else {
    for(Region_subregions subr(r); subr != 0; subr++) {
      caller_save_opti((Compound_region*)*subr);
    }
  }
}


///////////////////////////////////////////////////////////////
// When a variable is spilled, a SAVE is put right after each def, and
// a RESTORE is put right before each use. Sometimes the SAVE and
// RESTORE are right next to each other, which is stupid. This gets
// rid of restores of variables that are not killed between the SAVE
// and the use. FIX: make this predicate cognizant! Additionally, this
// is just a peephole opti; more opportunities may exist if we did the
// analysis on a more global scale.

void Scalar_reg_alloc::spill_code_opti(Compound_region* r)
{
  if(r->is_bb() || r->is_hb()) {

    List<Op*> to_remove;
    Hash_map<int, Op*> most_recent_save(hash_int, 71);

    for(Region_ops_C0_order rops(r); rops != 0; rops++) {
      Op* op = *rops;

      if(op->flag(EL_OPER_SPILL_CODE) && ! op->flag(EL_OPER_CALLER_SAVE)
	 && ! op->flag(EL_OPER_CALLEE_SAVE)) {
	if(is_base_plus_offset_load(op)) {
	  if(most_recent_save.is_bound(op->src(SRC2).int_value())) {
	    Op* the_save = most_recent_save[op->src(SRC2).int_value()];
	    
	    // This predicate compare is overly conservative and thus
	    // produces innefficient code, but I'm lazy.
            if(the_save->src(SRC3) == op->dest(DEST1)) {
              if(El_no_defs_between(r, the_save, op, op->dest(DEST1))) {
                // Use equal_vr instead of Operand==.  See comment in
                // caller_save_opti.  -KF 1/2006
                Operand& pred_s = the_save->src(PRED1);
                Operand& pred_r = op->src(PRED1);
                if(pred_s.is_reg() && pred_r.is_reg()) {
                  if(pred_s.equal_vr(pred_r)) {
                    to_remove.add_tail(op);
                  }
                } else if(pred_s == pred_r) {
                  to_remove.add_tail(op);
                }
              }
            }
          }
	} else if(is_base_plus_offset_store(op)) {
	  most_recent_save.bind(op->src(SRC2).int_value(), op);
	}
      } else if(is_brl(op)) {
	most_recent_save.clear();
      }
    } // for all ops
   
    for(List_iterator<Op*> li(to_remove); li != 0; li++) {
      El_remove_op(*li);
      delete *li;
    }
  } else {
    for(Region_subregions subr(r); subr != 0; subr++) {
      spill_code_opti((Compound_region*)*subr);
    }
  }
}


////////////////////////////////////////////////////////////////////////
// Spill code for predicates relies on the physical register number
// being known in order to determine the appropriate PV macro and mask
// for spill restores. The physical number isn't known until after
// allocation, though, so we fill in that information here.

void Scalar_reg_alloc::bind_pr_spill(Compound_region* r)
{
  List<Region*> regions;
  regions.add_tail(r);
  while( ! regions.is_empty()) {
    Compound_region* rg = (Compound_region*)regions.pop();
    if(rg->is_bb() || rg->is_hb()) {
      for(Region_ops_C0_order rops(rg); rops != 0; rops++) {
	Op* the_op = *rops;
	if(the_op->flag(EL_OPER_SPILL_CODE)) {
	  if(is_base_plus_offset_store(the_op)) {
	    if(the_op->src(SRC3).file_type() == PR) {
	      int pv_num = get_PV_num_for_reg(the_op->src(SRC3).mc_num());
	      Operand pv_macro(new Macro_reg(get_PV_macro(pv_num)));
	      pv_macro.bind_physical_file(the_op->src(SRC3).physical_file_type());
	      the_op->set_src(SRC3, pv_macro);
	    }
	  } else if(is_store(the_op)) {
	    if(the_op->src(SRC2).file_type() == PR) {
	      int pv_num = get_PV_num_for_reg(the_op->src(SRC2).mc_num());
	      Operand pv_macro(new Macro_reg(get_PV_macro(pv_num)));
	      pv_macro.bind_physical_file(the_op->src(SRC2).physical_file_type());
	      the_op->set_src(SRC2, pv_macro);
	    }
	  } else if(is_move(the_op)) {
	    if(the_op->src(SRC2).is_macro_reg() &&
	       (the_op->src(SRC2).name() == UNDEFINED)) {
	      // FIX: This assumes that the size of the PV macro is 32
	      // bits. Should query the MDES.
	      int mask_num = the_op->dest(DEST1).mc_num() & 0x1F;
	      Operand mask_oper(new Int_lit(mask_num));
	      the_op->set_src(SRC2, mask_oper);
	    }
	  }

	} // end if(it's spill code)
      }
    } else {
      for(Region_subregions subr(rg); subr != 0; subr++) {
	regions.add_tail(*subr);
      }
    }
  }
}


///////////////////////////////////////////////////////////////
// Find all used callee saved register and add spill code 
// at the entry and exit points.

void Scalar_reg_alloc::generate_callee_save_code(Procedure* r)
{
  int base = spill_stack.var_spill_size;
  int alignment = current_MDES->getAlignment();
  int i;
  Operand pred_operand(new Pred_lit(true));

  // Find first and last blocks in the procedure
  Compound_region* prologue_region = NULL, *epilogue_region = NULL;
  for (Region_subregions iter(r); iter != 0; iter++) {
    Region* sub_region = (*iter);
    if (sub_region->flag(EL_REGION_HAS_PROLOGUE_OP))
      prologue_region = (Compound_region*)sub_region;
    else if (sub_region->flag(EL_REGION_HAS_EPILOGUE_OP))
      epilogue_region = (Compound_region*)sub_region;
  }
  Op* entry_op = get_first_region_op_from_subregions(prologue_region);
  Op* exit_op = get_last_region_op_from_subregions(epilogue_region);

  int num_phys_files = MDES_num_reg_types();
  for (i = 0; i < num_phys_files; ++i) {
    Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
    if (rd->get_regkind() != REG_PHYSICAL)
      continue;
    char* phys_file_name = rd->get_aname();
    char* virt_file_name = rd->get_vname();
    // Only take the first vrf. see comment below.
    Reg_file vrf = mdesname_to_vregfile(virt_file_name[0]);

    Register_file& a_file = reg_file_pool.get_reg_file(phys_file_name);

    int width = MDES_reg_width(phys_file_name);

    // This code assumes that all virtual files that are colocated
    // within one physical file have the same DType. E.g. GPR and FPR
    // can't be in the same phys file.
    DType d_type = EL_STANDARD_INT;
    if (vrf == GPR) {
      d_type.set_width(width);
    } else if (vrf == FPR) {
      d_type = EL_STANDARD_DOUBLE;
    } else if (vrf == PR) {
      d_type = EL_STANDARD_PREDICATE;
      // Fix for WIMS and ARM architectures, which do not 
      // want PR callee save code and do not have PV macros
      if(current_MDES->is_macro_present(PV_0)) {
        generate_PR_callee_save_code(r, a_file, base);
        continue;
      }
    } else if (vrf == BTR) {
      d_type = EL_STANDARD_BRANCH;
    } else if (vrf == CR) {
      d_type.set_width(width);
    } else if (vrf == VIR) {
      d_type = EL_STANDARD_VEC_INT;
    } else if (vrf == VFR) {
      d_type = EL_STANDARD_VEC_DOUBLE;
    } else if (vrf == VMR) {
      d_type = EL_STANDARD_VEC_MASK;
    } else {
      El_punt("Scalar_reg_alloc::generate_callee_save_code: unknown reg file");
    }

    for(int reg_i = 0; reg_i < a_file.size(); reg_i++) {
      if(a_file.is_callee_save(reg_i)) {
	if(a_file.use_count(reg_i) != 0) {
	  //int reg_size = a_file.get_bit_width() / 8;
          // This should be ceilinged.  -KF 2/2006
          int reg_size = (a_file.get_bit_width() + 7) / 8;

	  base = ((base + (alignment - 1)) / alignment) * alignment;

	  int stack_loc = base;
	  if (current_MDES->getStackDir() == LOW_TO_HIGH)
	    stack_loc = -stack_loc;

	  Operand reg_operand( new Reg(d_type) );
	  reg_operand.bind_reg(reg_i);
	  reg_operand.bind_physical_file(eString(phys_file_name));

	  base += reg_size;
	  Op* the_op = insert_spill_op(reg_operand, pred_operand, prologue_region,
				       entry_op, AFTER_OP, SPILL_STORE);
	  the_op->set_flag(EL_OPER_CALLEE_SAVE);

	  the_op = insert_spill_op(reg_operand, pred_operand, epilogue_region,
				   exit_op, BEFORE_OP, SPILL_LOAD);
	  the_op->set_flag(EL_OPER_CALLEE_SAVE);
	}
      }
    }

  } // end for all physical files

  // define SWAP with register spill area size
  Op* define_op = new Op((Opcode) DEFINE);
  assert(define_op && "you're out of memory");
  Operand sp_operand(new Macro_reg(SWAP));

  // align the spill stack before assigning to swap macro
  base = ((base + (alignment - 1)) / alignment) * alignment;

  define_op->set_dest(DEST1, sp_operand);
  Operand loc_operand(new Int_lit(base));
  define_op->set_src(SRC1, loc_operand);
  Region_entry_ops op_iter(r);
  Op* first_op = *op_iter;
  El_insert_op_after(define_op, first_op);
}


void Scalar_reg_alloc::generate_PR_callee_save_code(Procedure* r,
						    Register_file& PR_reg_file,
						    int& base)
{
  int i;
  int reg_size = PR_reg_file.size();
  int CR_width = 32; // hard coded, fix it later
  int alignment = current_MDES->getAlignment();

  // Find first and last blocks in the procedure
  Compound_region* prologue_region = NULL, *epilogue_region = NULL;
  for(Region_subregions iter(r); iter != 0; iter++) {
    Region* sub_region = (*iter);
    if (sub_region->flag(EL_REGION_HAS_PROLOGUE_OP))
      prologue_region = (Compound_region*)sub_region;
    else if (sub_region->flag(EL_REGION_HAS_EPILOGUE_OP))
      epilogue_region = (Compound_region*)sub_region;
  }
  Op* entry_op = get_first_region_op_from_subregions(prologue_region);
  Op* exit_op = get_last_region_op_from_subregions(epilogue_region);

  // This code is kind of funny, because you can store up to 32
  // predicate registers at a time using special PV macros.
  Bitvector callee_vector(reg_size);
  for(i = 0; i < PR_reg_file.size(); ++i) {
    if(PR_reg_file.use_count(i) > 0) {
      callee_vector.set_bit(i);
    }
  }

  for(int index = 0; index < ((reg_size+CR_width-1) / CR_width); ++index) {
    Macro_name PV_i = get_PV_macro(index);
    Operand pv_operand(new Macro_reg(PV_i));
    pv_operand.bind_physical_file(PR_reg_file.get_name());
    Operand pred_operand(new Pred_lit(true));

    Op* the_op = insert_spill_op(pv_operand, pred_operand, prologue_region,
				 entry_op, AFTER_OP, SPILL_STORE);
    the_op->set_flag(EL_OPER_CALLEE_SAVE);

    the_op = insert_spill_op(pv_operand, pred_operand, epilogue_region,
			     exit_op, BEFORE_OP, SPILL_LOAD);
    the_op->set_flag(EL_OPER_CALLEE_SAVE);

    base = ((base + (alignment - 1)) / alignment) * alignment;

    base += (CR_width / 8);

    if(dbg(sa, 1)) 
      cdbg << "adding PR callee save op" << *the_op << endl;
  }
}


void Scalar_reg_alloc::reverse_macros(Compound_region* r, Reg_descr* rd)
{
  Hash_set<eString> macs(hash_estring, 8);
  rd->get_macros(macs);
  Hash_map<int, Macro_reg> reg_to_mac(hash_int);
  Map<Macro_name, int> mac_to_reg;

  for (Hash_set_iterator<eString> hsi_mac(macs); hsi_mac != 0; ++hsi_mac) {
    Macro_name name = el_string_to_macro_map.value(*hsi_mac);
    if(current_MDES->is_multialias_macro(name)) continue;
    Hash_set<eString>& regs = rd->get_physical_regs(*hsi_mac); 
    mac_to_reg.bind(name, current_MDES->get_MDES_reg_number_for_pf(regs.head()));
  }

  Hash_set<eString> bad_macs = current_MDES->get_MDES_alias_macros();
  Hash_set<eString>& multi_macs = current_MDES->get_MDES_multialias_macros();
  Hash_set<eString>& pseudo_macros = current_MDES->get_MDES_pseudo_macros();
  bad_macs += pseudo_macros;
  bad_macs += multi_macs;
  macs -= bad_macs; 

  for (Hash_set_iterator<eString> hsi_mac(macs); hsi_mac != 0; ++hsi_mac) {
    Macro_name name = el_string_to_macro_map.value(*hsi_mac);
    Macro_reg new_mac(name);
    Hash_set<eString>& regs = rd->get_physical_regs(*hsi_mac);
    for (Hash_set_iterator<eString> hsi_phys(regs); hsi_phys!=0; ++hsi_phys)
      reg_to_mac.bind(current_MDES->get_MDES_reg_number_for_pf(*hsi_phys), new_mac);
  }

  for (Region_all_ops opi(r); opi != 0; ++opi) {
    Op* op = *opi;
    for (Op_all_inputs ins(op); ins != 0; ++ins) {
      Operand opr = *ins;
      if (opr.is_reg() && !opr.is_rotating() && 
	  !strcmp(opr.physical_file_type(), rd->get_aname()) && opr.allocated()
	  && reg_to_mac.is_bound(opr.mc_num())) {
	Operand new_opr(new Macro_reg(reg_to_mac.value(opr.mc_num())));
	new_opr.bind_physical_file(opr.physical_file_type());
	new_opr.bind_reg(opr.mc_num());
	if (ins.ref_type() != IMP_SRC)
	  op->set_src(ins.get_port_num(), new_opr);
	else {
	  List<Operand>* imp_srcs = get_implicit_use_list (op);
	  Node<Operand>* old_imp_src = imp_srcs -> find (opr);
	  imp_srcs -> insert_after (old_imp_src, new_opr);
	  imp_srcs -> remove (opr);
	}
      } else if(opr.is_macro_reg()) {
	if(( ! opr.allocated()) &&
	   (opr.physical_file_type() == (const char*) rd->get_aname())) {
	  opr.bind_reg(mac_to_reg[opr.name()]);
	}
      }
    }
    for (Op_all_dests dests(op); dests != 0; ++dests) {
      Operand opr = *dests;
      if (opr.is_reg() && !opr.is_rotating() &&
	  !strcmp(opr.physical_file_type(), rd->get_aname()) && opr.allocated()
	  && reg_to_mac.is_bound(opr.mc_num())) {
	Operand new_opr(new Macro_reg(reg_to_mac.value(opr.mc_num())));
	new_opr.bind_physical_file(opr.physical_file_type());
	new_opr.bind_reg(opr.mc_num());
	if (dests.ref_type() != IMP_DEST)
	  op->set_dest(dests.get_port_num(), new_opr);
	else {
	  List<Operand>* imp_dests = get_implicit_def_list (op);
	  Node<Operand>* old_imp_dest = imp_dests -> find (opr);
	  imp_dests -> insert_after (old_imp_dest, new_opr);
	  imp_dests -> remove (opr);
	}
      } else if(opr.is_macro_reg()) {
	if(( ! opr.allocated()) &&
	   (opr.physical_file_type() == (const char*)rd->get_aname())) {
	  opr.bind_reg(mac_to_reg[opr.name()]);
	}
      }
    } // end for all dests
  } // end for all ops
}
