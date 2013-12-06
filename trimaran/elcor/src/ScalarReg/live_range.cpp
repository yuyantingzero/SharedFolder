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
//      File:           live_range.cpp
//      Authors:        Nate Clark
//      Created:        December 2004
//      Description:    a (hopefully) more intuitive replacement for the 
//                      usLiveRange class.
//
/////////////////////////////////////////////////////////////////////////////


#include "live_range.h"
#include "hash_functions.h"
#include "region.h"
#include "dbg.h"
#include "opcode.h"
#include "register_file.h"
#include "opcode_load_store.h"
#include "intf.h"
#include "scalar_reg_alloc.h"
#include "spill_code_gen.h"
#include "el_io.h"
#include "opcode_properties.h"
#include "region_utilities.h"
#include "iterators.h"
#include "op_attributes.h"
#include "el_sreg_init.h"
#include "el_lsched_init.h"


#define PRIORITY_NOT_SET (-ELCOR_MAX_DOUBLE)


unsigned hash_live_range_ptr(Live_range*& i)
{
   return(ELCOR_INT_HASH(i)) ;
}


Live_range::Live_range(Operand& v) : 
  oper(v), state(NOT_BOUND), ops_set(hash_op_ptr, 64), pred_map(hash_op_ptr, 64),
  the_priority(PRIORITY_NOT_SET), caller_benefit(PRIORITY_NOT_SET),
  callee_benefit(PRIORITY_NOT_SET), unspillable(false),
  inf_lrs(hash_live_range_ptr, 64)
{
  // FIX: figure out why an operand could NOT be bound to a physical file.
  if(oper.physical_file_type() == "") {
    if(oper.is_macro_reg()) {
      Hash_set<eString>& prfs = current_MDES->get_MDES_pf_for_macro(oper.name());
      if (prfs.size() != 1) {
	calculate_priority();
	cdbg << "HUH!?\n" << *this << endl;
      } else
	oper.bind_physical_file(prfs.head());
    } else {
      cdbg << "SCREWED!\n" << *this << endl;
      assert(0);
    }
  }

  cant_use.resize(get_reg_file().size());
}


void Live_range::combine_with(Live_range* lr)
{
  for(Hash_set_iterator<Op*> li(lr->ops_set); li != 0; li++) {
    Op* the_op = *li;
    if( ! ops_set.is_member(the_op)) {
      ops_set += the_op;
      if( ! pred_map.is_bound(the_op)) {
	pred_map.bind(the_op, lr->pred_map[the_op]);
      }
    }
  }

  for(List_iterator<El_ref> ri(lr->refs); ri != 0; ri++) {
    refs.add_tail(*ri);
  }

  the_priority = PRIORITY_NOT_SET;
  caller_benefit = PRIORITY_NOT_SET;
  callee_benefit = PRIORITY_NOT_SET;
  calculate_priority();
}


void Live_range::add_op(Op* op, Pred_cookie pc)
{
  ops_set += op;
  if( ! pred_map.is_bound(op))
    pred_map.bind(op, pc);
}


void Live_range::add_ref(const El_ref& ref)
{
  refs.add_tail(ref);
}


// This function determines the order which live ranges are allocated in.
double Live_range::get_priority() const
{
  if(the_priority == PRIORITY_NOT_SET)
    cdbg << "WARNING: it looks like you're trying to use "
	 << "Live_range::get_priority\nbefore calling "
	 << "Live_range::calculate_priority()!" << endl;
  return the_priority;
}


void Live_range::calculate_priority()
{
  if(oper.is_macro_reg()) {
    the_priority = 1;
    caller_benefit = 1;
    callee_benefit = 1;
    return;
  }

  /*

   This is a really huge comment, that I cut-and-pasted from the old
   code. I'm not copying all of the code from the old version of the
   function, however, so be warned that parts of this comment may be
   irrelevant. -ntclark 12/2004

   **********************************************************************

    callee saved register needs to spilling that register at the function
    boundaries (in prologue and epilogue).
    This extra cost can be spread out to all live ranges in this function
    which is using that callee saved register.
    To estimate this cost, I use 1/2 in the following code
    All cost are mutiplied with weight information, but added 1 for not 
    profiled Elcor
    
    added 9/9/98
    But the above algorothm has following problem
    Consider a usLiveRange lr1 where it has n definition of variable vr1
    and m number of uses, but no function calles. In this case, caller
    benefit is sligtly bigger that callee benefit (bt the factor of
    callee saved register spill in prologue and epilogue). If there is
    no caller saved register, then callee register may be used. In the 
    case of m and n are 0 (this is pass-through) live range, 
    callee_benefit is negative and caller_benefit is 0. This leads
    propagate_in() for lr1 to reject all callee_saved register.
    
    So another algorithm is
    A. Caller/Callee Benefit
    Do not consider callee-register spill cost for benefit
    
    B. Propagate_in 
    (used for new register selection in non pass-through
    and  reconcile for pass-through)
    For a given adjacent LR adj_lr
    if (adj_lr is SPILLED) {
    if (lr1->caller_benefit() - patch_cost(adj_lr,lr1) > 0)
    try other to bound;
    else if (lr1->callee_benefit() - patch_cost(adj_lr,lr1) > 0)
    try other to bound;
    else 
    SPILL lr1;
    }
    else if (adj_lr is CALLER_BOUND with reg1) {
    if (reg1 is available &&
    lr1->caller_benefit() > 0)
    BOUND lr1 with reg1;
    }
    else if (adj_lr is CALLEE_BOUND with reg1) {
    int extra_cost = (
    LOAD_COST+STORE_COST  if reg1 is first use
    0, other wise);
			
    if (reg1 is available &&
    lr1->caller_benefit() - extra_cost)
    BOUND lr1 with reg1;
    }
			  
    C. Register selection (coloring, new register without neighbor))
    if (caller_benefit == callee_benefit) {
    // this is when there is no function call
    assert(_caller_benefit >= 0); // _caller_benefit is 0, it lr1 is pass-through
    use caller_first; 
    int extra_cost = (
    LOAD_COST+STORE_COST  if reg1 is first use
    0, other wise);
    if (_callee_benefit - extra cost > 0) {
    then try callee;
    }
    }
    else if (caller_benefit() > callee_benefit) {
    use caller first;
    try callee, if possible
    } 
    else {
    ....
    }
  */

  unsigned store_latency = MDES_flow_time(el_opcode_to_string_map.value((Opcode)S_W_C1),
					  DATA_OUT, DEST1);
  unsigned load_latency = MDES_flow_time(el_opcode_to_string_map.value((Opcode)L_W_C1_C1),
                                         DATA_OUT, DEST1);

  double use_spill_cost = 0;
  double brl_spill_cost = 0;
  if(ops_set.size()) {
    for(Hash_set_iterator<Op*> li(ops_set); li != 0; li++) {
      Op* op = *li;
      if(is_brl(op)) {
	Pred_jar pj(op->parent());
	El_ref brl_pred(op, &(op->src(PRED1)), EXP_SRC);
	Pred_cookie brl_guard = pj.get_lub_guard(brl_pred);
	bool need_caller_save_code = false;
	for (Op* next_op = op->next(); next_op != NULL; next_op = next_op->next()) {
	  if(is_pseudo(next_op) || next_op->flag(EL_OPER_SPILL_CODE))
	    continue;
	  El_ref cur_pred(next_op, &(next_op->src(PRED1)), EXP_SRC);
	  Pred_cookie cur_guard = pj.get_lub_guard(cur_pred);
	  if( ! brl_guard.is_disjoint(cur_guard)) {
	    if(ops_set.is_member(next_op))
	      need_caller_save_code = true;
	    break;
	  }
	}

	if(need_caller_save_code)
	  brl_spill_cost += ((store_latency + load_latency) *
			     (op->parent()->weight + 1));
      }
    }

  }

  caller_benefit = 0;
  callee_benefit = 0;
  bool found_modulo_scheduled_loop = false;
  for(List_iterator<El_ref> ri(refs); ri != 0; ri++) {
    El_ref& the_ref = *ri;

    //cheap add-on, but seems to help cycle-count somewhat
    Op *curr_op = the_ref.get_op();
    Compound_region *curr_cr = curr_op->parent();
    double weight = (curr_cr->is_bb()) ? (curr_cr->weight + 1) : (El_calc_op_weight(curr_op) + 1); 


    if(((the_ref.get_op())->parent())->flag(EL_REGION_SOFTPIPE))
      found_modulo_scheduled_loop = true;

    if((the_ref.get_ref_type() == EXP_SRC) || (the_ref.get_ref_type() == PRED_SRC)) {
      use_spill_cost += (load_latency * weight);
    } else if(the_ref.get_ref_type() == EXP_DEST) {
      use_spill_cost += (store_latency * weight);
    }
  }

  caller_benefit = use_spill_cost - brl_spill_cost;
  //reduce the benefit to the caller b/c you have to spill the reg a lot anyway

  //callee_benefit = use_spill_cost + brl_spill_cost;
  callee_benefit = use_spill_cost;
    


  the_priority = use_spill_cost;

  // modulo scheduling has already been performed by the time we get
  // to register allocation, so if this variable is referenced in a
  // modulo scheduled loop, we can't spill it. give it a really big
  // priority.
  if (found_modulo_scheduled_loop) {
    the_priority = ELCOR_MAX_DOUBLE;
    caller_benefit = ELCOR_MAX(1, caller_benefit);
    callee_benefit = ELCOR_MAX(1, callee_benefit);
  }

  if (dbg(sa,7)) {
    cdbg << "Live_range::priority\tvar: " << oper << " caller_benefit: "
	 << caller_benefit << " callee_benefit: " << callee_benefit 
	 << " spill cost:" << use_spill_cost << " priority: " << the_priority
	 << endl;
  }
}


// FIX: this should be made predicate cognizant.
void Live_range::set_interference(Live_range* lr)
{
  // If they aren't in the same register file, then they can't interfere.
  if (oper.physical_file_type() != lr->oper.physical_file_type())
    return;
 
  // Take the intersection of the two opsets. if they are disjoint,
  // then they don't interfere. Note that this is
  // conservative because it is predicate ignorant.

  if(ops_set.intersects(lr->ops_set)) {
    if(dbg(sa, 8))
      cdbg << "These operands interfere: " << oper << "\t" << lr->get_oper() << endl;
    inf_lrs += lr;
    lr->inf_lrs += this;
  }
}


void Live_range::update_interference_graph()
{
  if(state != SPILLED)
    return;

  // Remove myself from the interfering live ranges.
  for(Hash_set_iterator<Live_range*> hsi(inf_lrs); hsi != 0; hsi++) {
    Live_range* some_lr = *hsi;
    some_lr->inf_lrs -= this;
  }

  inf_lrs.clear();
}


void Live_range::spill()
{
  state = SPILLED;
  oper.bind_reg(-1);

  if(unspillable || (the_priority == ELCOR_MAX_DOUBLE)) {
    if((the_priority == ELCOR_MAX_DOUBLE) &&
       (El_msched_allocator == MODULO_VARIABLE_EXPANSION)) {
      // Legally we can spill ops in MVE code, but it completely
      // screws up the modulo schedule. This is a performance, not a
      // correctness problem.
      El_warn("Spilling in a modulo scheduled loop!\n");
    }

    // If you get here, you're kind of screwed. There weren't enough
    // registers to color this interference graph even after
    // spilling. Five potential solutions: 1. This is a bug in the
    // register allocator. You'll get this assert if spilled live
    // ranges aren't actually taken out of the interference graph, for
    // example. If this is the case, then you need to fix the
    // allocator. Otherwise you have the following
    // options. 2. Implement Live_range splitting. This will make the
    // live ranges shorter and hopefully provide you with enough free
    // registers to allocate. 3. Turn off prepass
    // scheduling. Scheduling tends to increase the size of live
    // ranges. If you turn it off, your schedule will suck (assuming
    // you have a multiple issue machine), but it gives you a better
    // chance to get through the compiler. 4. Turn off loop unrolling
    // (in impact). This creates a ton of register pressure. 5. Add
    // more registers to the architecture.
    cdbg << "ERROR: Trying to spill an unspillable Live_range! "
	 << "There's not enough registers\n"
	 << "to complete allocation of this procedure.\n\n" << *this << endl;
    cdbg << "INTERFERENCES: " << flush;
    bool first_trip = true;
    for(Hash_set_iterator<Live_range*> hsi(inf_lrs); hsi != 0; hsi++) {
      if( ! first_trip)
	cdbg << ", ";

      if((*hsi)->get_oper().is_macro_reg())
	cdbg << el_macro_to_string_map[(*hsi)->get_oper().name()];
      else
	cdbg << (*hsi)->get_oper().vr_num();
      first_trip = false;
    }
    cdbg << endl;
    assert(0);
  }
 
  for (List_iterator<El_ref> iter(refs); iter != 0; ++iter) { 
    El_ref& ref = *iter; 
    Op* op_ptr = ref.get_op(); 

    Operand spill_oper;
    if(ref.get_operand().file_type() == PR) {
      // If we're spilling predicates, we don't want to create a new
      // register. PQS uses the virtual number to do relational
      // lookups, and creating the new register would make this
      // harder.
      spill_oper = ref.get_operand();
    } else {
      // Create a new register for the spill code. 
      Operand new_reg(new Reg(ref.get_operand().dtype())); 
      new_reg.bind_physical_file(ref.get_operand().physical_file_type()); 
      Scalar_reg_alloc::spill_stack.alias_operand(new_reg, ref.get_operand());
      spill_oper = new_reg;
    }
 
    El_ref_type ref_type= ref.get_ref_type(); 
    if (ref_type == EXP_DEST) {
      if(ref.get_operand().dtype() != EL_STANDARD_PREDICATE) {
	op_ptr->set_dest(ref.get_port_num(), spill_oper);
      }
 
      Op* the_op = insert_spill_op(spill_oper, op_ptr->src(PRED1),
				   op_ptr->parent(), op_ptr, AFTER_OP,
				   SPILL_STORE);

      // For predicate spill stores, guarding on true is bad because then the
      // spill dominates the define, and the predicate's live range contains
      // many more ops than it needs to, leading to graphs that should be
      // colorable being uncolorable.  Therefore guard predicate spill stores by
      // the same guard as the define op, except in the case of U_preds which
      // always write their dests regardless of guard.  -KF 1/2006
      if(ref.get_operand().file_type() == PR) {
        if(op_ptr->predicated() && !is_U_pred(op_ptr, ref.get_port_num())) {
          the_op->set_src(PRED1, op_ptr->src(PRED1));
        }
      }
      
      if(dbg(sa, 7))
	cdbg << "   INSERTING STORE OP: " << the_op->id() << " FOR "
	     << oper.vr_num() << endl;

    } else if (ref_type == EXP_SRC || ref_type == PRED_SRC) { 
      if(ref.get_operand().dtype() != EL_STANDARD_PREDICATE) {
	op_ptr->set_src(ref.get_port_num(), spill_oper);
      }

      Op* the_op = insert_spill_op(spill_oper, op_ptr->src(PRED1),
				   op_ptr->parent(), op_ptr, BEFORE_OP,
				   SPILL_LOAD);
      if(dbg(sa, 7))
	cdbg << "   INSERTING LOAD OP: " << the_op->id() << " FOR "
	     << oper.vr_num() << endl;

    } else if (ref_type == IMP_SRC) {
      List<Operand>* imp_srcs = get_implicit_use_list (op_ptr);
      Node<Operand>* old_imp_src = imp_srcs -> find (oper);
      imp_srcs -> insert_after (old_imp_src, spill_oper);
      imp_srcs -> remove (oper);
      Op* the_op = insert_spill_op(spill_oper, op_ptr->src(PRED1),
				   op_ptr->parent(), op_ptr, BEFORE_OP,
				   SPILL_LOAD);
      if(dbg(sa, 7))
	cdbg << "   INSERTING LOAD OP: " << the_op->id() << " FOR "
	     << oper.vr_num() << endl;

    } else if (ref_type == IMP_DEST) {
      List<Operand>* imp_dsts = get_implicit_def_list (op_ptr);
      Node<Operand>* old_imp_dst = imp_dsts -> find (oper);
      imp_dsts -> insert_after (old_imp_dst, spill_oper);
      imp_dsts -> remove (oper);
      Op* the_op = insert_spill_op(spill_oper, op_ptr->src(PRED1),
				   op_ptr->parent(), op_ptr, AFTER_OP,
				   SPILL_STORE);
      if(dbg(sa, 7))
	cdbg << "   INSERTING STORE OP: " << the_op->id() << " FOR "
	     << oper.vr_num() << endl;
      
    }
  }
}


bool Live_range::is_constrained()
{
  if (oper.assigned_to_physical_file()) { 
    unsigned int static_reg_size =
      (get_reg_file().caller_size() + get_reg_file().callee_size());
 
    if (inf_lrs.size() < static_reg_size)
      return false;
    else
      return true;
  } else {
    El_warn("Something's wrong with the Live_range you're trying to allocate...\n");
  }
  return false;
}


reg_state Live_range::color()
{
  if (oper.is_macro_reg()) {
    // Macros must always be allocated to their assigned physical register(s)
    Hash_set<int> phys_regs(hash_int, 8);
    MDES_get_phys_regs(oper, phys_regs);
    bind_register(phys_regs.head());
    return state;
  }

  Register_file& rb = get_reg_file();
  if ( ! rb.has_free_regs(cant_use)) {
    // If you want to add any optimizations like live range splitting,
    // this is a good place to do it.

    spill();
    return state;
  }

  if (caller_benefit < 0 && callee_benefit < 0) {
    spill();
    return state;
  }

  int reg = select_register();

  if(reg == -1) {
    spill();
  } else {
    bind_register(reg);
  }

  return state;
}


int Live_range::select_register()
{
  Register_file& rb = get_reg_file();

  Bitvector reg_pool;
  if (caller_benefit > callee_benefit) {
    reg_pool = rb.get_caller_bv();
    reg_pool -= cant_use;
    if (reg_pool.ones_count() == 0) {
      reg_pool = rb.get_callee_bv();
      reg_pool -= cant_use;
      if(reg_pool.ones_count() <= 0) {
	cdbg << "WARNING: this should never happen!" << endl
	     << "Caller: " << rb.get_caller_bv() << endl
	     << "Callee: " << rb.get_callee_bv() << endl;	  
	return -1;
      }
    }
  } else if (caller_benefit < callee_benefit) {
    reg_pool = rb.get_callee_bv();
    reg_pool -= cant_use;
    if (reg_pool.ones_count() == 0) {
      reg_pool = rb.get_caller_bv();
      reg_pool -= cant_use;
      if(reg_pool.ones_count() <= 0) {
	cdbg << "WARNING: this should never happen!" << endl
	     << "Caller: " << rb.get_caller_bv() << endl
	     << "Callee: " << rb.get_callee_bv() << endl;	  
	return -1;
      }
    }
  } else {
    reg_pool = rb.get_callee_bv();
    reg_pool += rb.get_caller_bv();
    reg_pool -= cant_use;
    if(reg_pool.ones_count() <= 0) {
      cdbg << "WARNING: this should never happen!" << endl
	   << "Caller: " << rb.get_caller_bv() << endl
	   << "Callee: " << rb.get_callee_bv() << endl;	  
      return -1;
    }
  }

  return reg_pool.first_one();
}


void Live_range::bind_register(int reg_num)
{
  if(reg_num < 0)
    return;

  if(get_reg_file().is_caller_save(reg_num))
    state = CALLER_BOUND;
  else
    state = CALLEE_BOUND;

  oper.bind_reg(reg_num);

  for(List_iterator<El_ref> ri(refs); ri != 0; ri++) {
    (*ri).get_operand().bind_reg(reg_num);
  }

  for (Hash_set_iterator<Live_range*> iter(inf_lrs); iter != 0; iter++) {
    (*iter)->cant_use.set_bit(reg_num);
  }

  // Check if this is a return macro. If so, caller saving it is a bad idea.
  bool is_return_macro = false;
  if(oper.is_macro_reg()) {
    if(oper.name() == INT_RETURN || oper.name() == FLT_RETURN ||
       oper.name() == DBL_RETURN) {
      is_return_macro = true;
    } else {
      eString& the_name = el_macro_to_string_map[oper.name()];
      Hash_set<eString>& aliases = current_MDES->get_MDES_alias_macros();
      if (aliases.is_member(the_name)) {
	if(current_MDES->get_macro_aliased(the_name) == "INT_RETURN" ||
	   current_MDES->get_macro_aliased(the_name) == "FLT_RETURN" ||
	   current_MDES->get_macro_aliased(the_name) == "DBL_RETURN") {
	  is_return_macro = true;
	}
      } else {
	eString& int_ret = el_macro_to_string_map[INT_RETURN];
	eString& flt_ret = el_macro_to_string_map[FLT_RETURN];
	eString& dbl_ret = el_macro_to_string_map[DBL_RETURN];
	if(aliases.is_member(int_ret) && 
	   current_MDES->get_macro_aliased(int_ret) == the_name) {
	  is_return_macro = true;
	} else if(aliases.is_member(flt_ret) && 
		  current_MDES->get_macro_aliased(flt_ret) == the_name) {
	  is_return_macro = true;
	} else if(aliases.is_member(dbl_ret) && 
		  current_MDES->get_macro_aliased(dbl_ret) == the_name) {
	  is_return_macro = true;
	}
      }
    }
  }


  // Insert spill code for caller save regs.
  if((state == CALLER_BOUND) && ( ! is_return_macro)) {

    for (Hash_set_iterator<Op*> iter(ops_set); iter != 0; ++iter) {
      Op* live_op = *iter;
      if(is_brl(live_op)) {

	// Find the next non-spill op after the BRL. If that is in the
	// live range, then we need some caller save code.

	Pred_jar pj(live_op->parent());
	El_ref brl_pred(live_op, &(live_op->src(PRED1)), EXP_SRC);
	Pred_cookie brl_guard = pj.get_lub_guard(brl_pred);
	bool need_caller_save_code = false;
	Op* next_op = NULL, *exit_op = NULL;
	for (next_op = live_op->next(); next_op != NULL; next_op = next_op->next()) {
	  if(live_op->parent()->is_exit_op(next_op))
	    exit_op = next_op;
	  if(is_pseudo(next_op) || next_op->flag(EL_OPER_SPILL_CODE))
	    continue;
	  El_ref cur_pred(next_op, &(next_op->src(PRED1)), EXP_SRC);
	  Pred_cookie cur_guard = pj.get_lub_guard(cur_pred);
	  if( ! brl_guard.is_disjoint(cur_guard)) {
	    if(ops_set.is_member(next_op))
	      need_caller_save_code = true;
	    break;
	  }
	}

	// If the BRL is at the end of the block, then we need to
	// check if the operand is live out of the block.
	if(exit_op && ! need_caller_save_code) {
	  for(Op_pseudo_sources lives(exit_op); lives != 0; ++lives) {
	    El_ref cur_ref = lives.get_ref();
	    if(lives.get_ref().get_operand() == oper) {
	      need_caller_save_code = true;
	      break;
	    }
	  }
	}

	// This is an ugly hack...
	if(oper.is_macro_reg()) {
	  if(oper.name() == INT_RETURN || oper.name() == FLT_RETURN ||
	     oper.name() == DBL_RETURN)
	    need_caller_save_code = false;
	}

	if (need_caller_save_code) {
	  Op* the_op = insert_spill_op(oper, live_op->src(PRED1), live_op->parent(),
				       live_op, BEFORE_OP, SPILL_STORE);
	  the_op->set_flag(EL_OPER_CALLER_SAVE);
	  if(dbg(sa, 7)) {
	    if(oper.is_macro_reg())
	      cdbg << "   INSERTING CALLER SAVE STORE: " << the_op->id() << " FOR "
		   << oper.name() << endl;
	    else
	      cdbg << "   INSERTING CALLER SAVE STORE: " << the_op->id() << " FOR "
		   << oper.vr_num() << endl;
	  }

	  the_op = insert_spill_op(oper, live_op->src(PRED1), live_op->parent(),
				   live_op, AFTER_OP, SPILL_LOAD);
	  the_op->set_flag(EL_OPER_CALLER_SAVE);
             
	  if(dbg(sa, 7)) {
	    if(oper.is_macro_reg())
	      cdbg << "   INSERTING CALLER SAVE LOAD:  " << the_op->id() << " FOR "
		   << oper.name() << endl;
	    else
	      cdbg << "   INSERTING CALLER SAVE LOAD:  " << the_op->id() << " FOR "
		   << oper.vr_num() << endl;
	  }
	} // end need_caller_save_code

      } // end if is_brl
    } // end for all live ops
  } // end if caller save

  get_reg_file().add_use(reg_num);
}


void Live_range::unbind()
{
  state = NOT_BOUND;
  oper.unbind_reg();
  for(List_iterator<El_ref> ri(refs); ri != 0; ri++) {
    (*ri).get_operand().unbind_reg();
  }
  cant_use.clear();
}


Register_file& Live_range::get_reg_file() const
{
  return reg_file_pool.get_reg_file((char*)oper.physical_file_type());
}


ostream& operator<<(ostream& os, const Live_range& lr)
{
  os << "Live_range:\nOperand: " << lr.oper << endl;
  os << " Refs:";
  for(List_iterator<El_ref> li(lr.refs); li != 0; li++) {
    os << "\t" << *li << endl;
  }
  os << "\n\tpriority: " << lr.get_priority() << endl << "\tcaller_benefit: "
     << lr.caller_benefit << "\n\tcallee_benefit: "
     << lr.callee_benefit << endl;
  os << " Ops: ";
  if(lr.ops_set.size()) {
    Vector<Op*> sorted_ops(lr.ops_set.size());
    unsigned i = 0;
    for(Hash_set_iterator<Op*> opi(lr.ops_set); opi != 0; opi++, i++) {
      sorted_ops[i] = *opi;
    }
    for(unsigned i = 0; i < (sorted_ops.size() - 1); i++) {
      for(unsigned j = i + 1; j < sorted_ops.size(); j++) {
	if(sorted_ops[i]->id() > sorted_ops[j]->id()) {
	  Op* tmp = sorted_ops[i];
	  sorted_ops[i] = sorted_ops[j];
	  sorted_ops[j] = tmp;
	}
      }
    }

    for(unsigned i = 0; i < sorted_ops.size(); i++) {
      os << sorted_ops[i]->id();
      if(i != (sorted_ops.size() - 1))
	os << ", ";
    }
  }
  os << endl;
  return os;
}


#undef PRIORITY_NOT_SET
