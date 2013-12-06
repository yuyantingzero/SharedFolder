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

Copyright 1994 Hewlett-Packard Company

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




///////////////////////////////////////////////////////////////////////////
//									 //
//      File:           el_opti_loopinvariant.cpp                        //
//      Authors:        Sadun Anik, Matt Jennings        		 //
//      Created:        June 1997        				 //
//      Description:    Loop Invariant Code Removal interface		 //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "dbg.h"
#include "iterators.h"
#include "el_opti_loopinvariant.h" 
#include "el_loop.h"
#include "reaching_defs_solver.h"
#include "el_loop_iterators.h"
#include "el_bb_tools.h"
#include "pred_analysis.h"
#include "opcode.h"
#include "opcode_properties.h"
#include "intf.h"
#include "el_control.h"
#include "control_analysis_solver.h"
#include "el_main.h"

using namespace std;

static bool is_movable_opcode(Op* op_ptr) 
{
  // Opcode is movable if it is not a branch (control_switch) op 
  // or a load/store (mem_ref) op or an op without operands.

  // Rajiv: New flag to include pbr during loop invariant code Motion
  if (is_pbr(op_ptr) && !El_include_pbr_during_LICR)
      return false;
  if ( !(is_control_switch(op_ptr) ||
	 is_control_merge(op_ptr) ||
	 is_cmpp(op_ptr) ||
	 is_remap(op_ptr) ||
	 is_predicate_set(op_ptr) ||
	 is_predicate_clear(op_ptr) ||
	 is_clear_all_rotating(op_ptr)) ) 
    return true ;
  else return false ;
}

static bool is_op_loop_invariant(Op* op_ptr, Compound_region* loop, Reaching_defs_info* rdinfo) 
{
  List_iterator<El_ref> def_iter ;
  List_iterator<El_ref> depend_iter ;
  // Op is loop invariant if source operands are defined outside loop
  // region. This function does not detect ops that are loop invariant 
  // because source operands are defined by other loop invariant ops.
  // Repeated use of this function after loop invariant ops are moved
  // to the pre-header finds these cases.
  
  // Iterate for each source operand on input op_ptr, excluding predicate inputs.
  for (Op_complete_sources src_operand_iter(op_ptr); src_operand_iter!=0; src_operand_iter++) {
    Operand src_operand = *src_operand_iter;
    El_ref src = src_operand_iter.get_ref() ;
    if (rdinfo->is_interesting(src_operand)) {
      List<El_ref>* defs = rdinfo->get_ud_chain(src) ;
      // Iterate over each def to determine if source operand is defined
      // at a region entry (outside of loop) or in loop region.
      for (def_iter(*defs); def_iter != 0; def_iter++) {
	El_ref& cur_def = *def_iter ;
	Op* cur_op = cur_def.get_op();
	if ( !(loop->is_entry_op(cur_op)) ) return false ;
      }
    }
  }
  return true;
}

static bool is_op_dest_movable(El_ref def1, Compound_region* loop, Reaching_defs_info* rdinfo) 
{
  List_iterator<El_ref> use_iter, def_iter ;

  Operand def1_var = def1.get_operand();
  if (!rdinfo->is_interesting(def1_var)) return true ;
  // 1st destination constraint: if dest_var(s) is live at a loop exit, def1 is
  // the only definition of dest_var reaching that exit.
  
  // 2nd destination constraint: if there is a use of dest_var(s) in the loop
  // def1 is the only reaching definition for that use.
  
  // Iterate over each use of def1 in loop region to determine  
  // if def1 is the only def for this use.
  // Because du chains include live out variables as pseudo-operands, 
  // this analysis does check defs for exit variables, satisfying
  // 1st and 2nd destination constraints.
  List<El_ref>* uses = rdinfo->get_du_chain(def1);
  for (use_iter(*uses) ; use_iter != 0 ; use_iter++) {
    El_ref& use = *use_iter;
    // There is a use of def1.
    // Check if def1 is the only reaching definition for this use.
    List<El_ref>* defs = rdinfo->get_ud_chain(use);
    if (defs->size() == 1) {
      assert(def1 == defs->head()) ;
    }
    else {
      // Def1 is not the only reaching definition because there are
      // more than one definitions that reach this use.
      return false ;
    }
  }
  
  // 3rd destination constraint: def1 is the only definition of dest_var(s) in 
  // the loop.
  
  // Iterate over each Op in loop region to determine if def1
  // operand(s) is the only definition(s) of dest_var(s).
  for (Region_all_ops op_iter(loop); op_iter != 0; op_iter++) {
    for (Op_all_dests dest_iter(*op_iter); dest_iter!=0; dest_iter++) {
      Operand def_var = *dest_iter;
      if ((def_var == def1_var) && (*op_iter != def1.get_op())) {
	// Def1 is not the only definition of dest_var(s) in the loop.
	return false;
      }
    }
  } 
  return true;
}

static bool is_op_conditionally_executed(Op* op_ptr, El_Loop* loop, Compound_region* loop_cr, 
					 Reaching_defs_info* rdinfo, Dominator_CFA_solver* dominfo)
{
  Hash_set_iterator<Basicblock*> post_BB_iter ;
  List_iterator<El_ref> def_iter ;
  List_iterator<El_ref> pred_operand_iter ;
  Basicblock* test_BB;

  // This function tests for whether or not an operation is conditionally 
  // executed in a loop. If it is conditionally executed, it can only
  // be moved to the pre-header if a silent version exists. It is conditionally
  // executed if it is predicated or if it does not dominate all loop exits
  Compound_region* test_block = op_ptr->parent();
  if (test_block->is_bb()) {
    test_BB = (Basicblock*) test_block;
    Hash_set<Basicblock*> postloops = loop->postloop_blocks;
    for (post_BB_iter(postloops); post_BB_iter != 0; post_BB_iter++) {
      Basicblock* exit_block = (*post_BB_iter);
      if (!dominfo->is_dom(test_BB, exit_block)) return true;
    }
  }
  
  if (is_conditionally_executed(op_ptr)) {
    for (Op_predicate_sources pred_operand_iter(op_ptr); pred_operand_iter!=0; pred_operand_iter++) {
      Operand pred_operand = *pred_operand_iter;
      El_ref pred = pred_operand_iter.get_ref() ;
      if (rdinfo->is_interesting(pred_operand)) {
	List<El_ref>* defs = rdinfo->get_ud_chain(pred) ;
	for (def_iter(*defs); def_iter != 0; def_iter++) {
	  // if a def exists and it is in the loop compound region (not an entry op), 
	  // then the op is conditionally executed. 
	  El_ref& cur_def = *def_iter ;
	  Op* cur_op = cur_def.get_op();
	  if ( !(loop_cr->is_entry_op(cur_op)) ) return true;
	}
      }
    }
  }
  return false;
}
static bool is_op_speculatively_movable(Op* op) 
{
  return (MDES_has_speculative_version(get_mdes_opcode_string(op))) ;
}


/* SAM, stuck something in that seems reasonable for systolic synthesis */
/* maybe more sophistication should be added */
static bool is_profitable_to_move_op(Op* op_ptr) 
{
  if (El_only_remove_expensive_invariants) {
    /* Move expensive ops out always */
    if (is_mul(op_ptr) || is_div(op_ptr) || is_int_rem(op_ptr) ||
        is_int_lshift(op_ptr) || is_int_rshift(op_ptr) ||
        is_int_satadd(op_ptr) || is_int_satsub(op_ptr) ||
        is_muladd(op_ptr) || is_mulsub(op_ptr))
      return (true);
    else
      return (false);
  }
  return (true);
}

static bool is_profitable_to_move_expr(Op* op_ptr) 
{
  // May want to include a better heuristic here in the future 
  if (El_only_remove_expensive_invariants) {
    /* Move expensive ops out always */
    if (is_mul(op_ptr) || is_div(op_ptr) || is_int_rem(op_ptr) ||
        is_int_lshift(op_ptr) || is_int_rshift(op_ptr) ||
        is_int_satadd(op_ptr) || is_int_satsub(op_ptr) ||
        is_muladd(op_ptr) || is_mulsub(op_ptr))
      return (true);
    else
      return (false);
  }
  return true ;
}

Op* create_copy_for_dest(Op* op_ptr, El_ref def, Reaching_defs_info* rdinfo) 
{
  List_iterator<El_ref> use_iter ;
  List_iterator<El_ref> def_iter ;
  // For this un-movable dest, change cur_op dest to a unique temp and add 
  // copy op from temp to original dest so cur_op can be moved to pre-header.
  // For new copy op, replicate du chains from prev. dest.
  Operand dest_operand = def.get_operand();
  Port_num dest_port = def.get_port_num();
  El_ref_type dest_ref_type = def.get_ref_type();

  Opcode move_opcode = get_move_opcode_for_operand(dest_operand);
  Op* move_op = new Op(move_opcode);

  move_op->set_dest(DEST1, dest_operand);
  Operand temp = new Reg(dest_operand.dtype());
  move_op->set_src(SRC1, temp);
  move_op->set_src(PRED1, op_ptr->src(PRED1)) ;

  El_ref new_move_dest(move_op, &(move_op->dest(DEST1)), EXP_DEST) ;
  El_ref new_move_src(move_op, &(move_op->src(SRC1)), EXP_SRC) ;
  El_ref new_move_pred(move_op, &(move_op->src(PRED1)), PRED_SRC) ;
  
  El_ref old_pred(op_ptr, &(op_ptr->src(PRED1)), PRED_SRC) ;

  rdinfo->clone_ud_for_new_use(new_move_pred, old_pred) ;
  rdinfo->clone_du_for_new_def(new_move_dest, def) ;

  rdinfo->remove_def(def) ;
  rdinfo->remove_use(old_pred) ;

  // rebuld the old operation
  
  op_ptr->set_dest(dest_port, temp);
  Operand new_pred_oper = new Pred_lit(true) ;
  op_ptr->src(PRED1) = new_pred_oper ;

  El_ref old_def2(op_ptr,&(op_ptr->dest(dest_port)), dest_ref_type) ;
  rdinfo->add_du_ud_links(old_def2, new_move_src) ;

  if (dbg (opti, 3)) {
     cdbg<<"\tmove op:\n"<<(*move_op)<<"added and op:\n"<<(*op_ptr);
     cdbg<<"\tmodified to support moving invariant expression\n";
  }
  return(move_op);

}

void move_op_to_pre_header(Op* op_ptr, El_Loop* loop, Compound_region* loop_cr, 
			   Reaching_defs_info* rdinfo, Dominator_CFA_solver* dominfo) 
{
  // Move op to pre-header.
  // du/ud chains for operation are dropped. 
  // Remove Op from loop kernel.
  bool cond1 = is_op_conditionally_executed(op_ptr, loop, loop_cr, rdinfo,
      dominfo) && is_op_speculatively_movable(op_ptr);
  bool cond2 = is_conditionally_executed(op_ptr);
  
  // parkhc 10/13/2005
  // remove du/ud chains before its pred source operand is changed.
  for (Op_complete_dests dest_iter(op_ptr); dest_iter!=0; dest_iter++) {
    El_ref dest = dest_iter.get_ref() ;
    rdinfo->remove_def(dest) ;
  }
  for (Op_complete_inputs use_iter(op_ptr); use_iter!=0; use_iter++) {
    El_ref use = use_iter.get_ref() ;
    rdinfo->remove_use(use) ;
  }

  // set speculative flag if op is conditionally executed and has a 
  // speculative version
  if (cond1) {
    if (cond2) {
      Operand old_pred = op_ptr->src(PRED1);
      op_ptr->src(PRED1) = *(new Operand(new Pred_lit(true)));
    }
    op_ptr->set_flag(EL_OPER_SPECULATIVE);
  }

  Basicblock* preloop = loop->preloop_block;
  El_insert_op_before_switch(preloop, El_remove_op(op_ptr));
  return;
}

//
// This optimization requires valid global liveness information
//   ** including memory virtual registers **
// It should not change liveness information outside loops
// that are optimized.
//
void el_opti_remove_loop_invariants(Compound_region* R, Reaching_defs_info* rdinfo) 
{
  if (dbg(opti, 1) || dbg(status, 3)) 
    cdbg << "-- Loop invariant code removal " << endl;

  // Detect loops and insert loop preheaders.
  El_form_basic_blocks(R);
  El_Loop_Graph all_loops(R) ;
  all_loops.create_preloop_blocks(ALL_LOOPS) ;

  Dominator_CFA_solver dominfo(R); 
  dominfo.solve();

  // Analyze and optimize each loop in all_loops.
  for (El_Loop_Graph_Innermost li(all_loops) ; li != 0 ; li++) {
    El_Loop* current_loop = *li ; 
    Compound_region* loop_cr = current_loop->insert_compound_region() ;
    if (loop_cr == NULL) continue;
    
    // Look for loop invariant ops which can be moved to pre-header until 
    // one is not found during an interation over all ops in a region. This 
    // allows for catching ops with source operands defined by loop 
    // invariant op already moved to the pre-header. These ops may also be 
    // loop invariant.
    bool loop_invariant_found = true;
    while (loop_invariant_found) {
      loop_invariant_found = false;
      
      // Iterate over all Ops in loop region.
      Region_all_ops op_iter(loop_cr); 
      while (op_iter != 0) {
        bool ok_to_move = true;
        bool profitability_checked = false;
	Op* cur_op = *op_iter; 
	op_iter++;
	if (dbg (opti, 10)) {
	   cdbg <<"\tcurrent op: "<<(*cur_op)<<"\n";
	}
	// Determine if op is loop invariant and therefore op or expression can 
	// be moved to the loop pre-header basic block if opcode is also movable.
	if (is_movable_opcode(cur_op) && is_op_loop_invariant(cur_op, loop_cr, rdinfo)) {
	  // Check if dest is movable for each possible dest_var of Op.
	  // Op or Expression can be moved to pre-header unless it is both conditionally 
	  // executed and a speculatively movable version does not exist.
	  if ( !(is_op_conditionally_executed(cur_op, current_loop, loop_cr, rdinfo, &dominfo) && 
		 (!is_op_speculatively_movable(cur_op))) ) {
	    // Check if dest is movable for each possible dest_var of Op.
	    for (Op_all_dests dest_var_iter(cur_op); dest_var_iter!=0; dest_var_iter++) { 
	      El_ref def = dest_var_iter.get_ref();
	      // check if destination requirements restrict movement of entire op
	      // to the pre-header, thereby only allowing the expression to be moved
	      if (!is_op_dest_movable(def, loop_cr, rdinfo)) {
		// check global compiler flag to determine if loop invariant expressions should 
		// be moved
		if (!El_include_loop_invariant_expressions) {
		  ok_to_move = false;
		  break;
		}
		// Do not move expressions of move operations (it would recursively replace 
		// them with more move operations)
		// Also, do not move expressions of compare-to-predicate operations as 
		// no predicate move operations exist to support this
		if (is_move(cur_op) || is_cmpp(cur_op)) {
		  ok_to_move = false;
		  break; 
		}
		// A heuristic can be added to determine profitability.
		// Do not move expressions unless profitable.
		// Need to only check heuristic once, may be too expensive to check more often
		if (!profitability_checked) {
		  if (!is_profitable_to_move_expr(cur_op)) {
		    ok_to_move = false;
		    break;
		  }
		  profitability_checked = true;
		}
		// For this un-movable dest, change cur_op dest to a unique temp and add 
		// copy op from temp to original dest so cur_op can be moved to pre-header.
		// For new copy op, replicate du chains from prev. dest.
		Op* new_copy = create_copy_for_dest(cur_op, def, rdinfo);
		El_insert_op_after(new_copy, cur_op);
	      }
	    }
	    // ok_to_move if no dest constraints exist or if there are dest constraint
	    // but it is profitable to move the expression and op is not a move op
	    if (ok_to_move) {
	      if (profitability_checked) {
		// Move modified op that includes loop invariant expression to pre-header.
		// du/ud chains for operation are dropped. 
		// Op is also removed from loop kernel.
		move_op_to_pre_header(cur_op, current_loop, loop_cr, rdinfo, &dominfo);
		if (dbg (opti,3)) {
		   cdbg << "\tinvariant expression moved to pre-header\n";
		   cdbg <<"\tinvariant expression:\n"<<(*cur_op)<<"\tmoved to pre-header\n\n";
		}
	      }
	      // else there are no dest constraints and just need to determine if it
	      // is profitable to move op to the loop pre-header
	      else if (is_profitable_to_move_op(cur_op)) {
		// Move op to pre-header.
		// du/ud chains for operation are dropped. 
		// Op is also removed from loop kernel.
		move_op_to_pre_header(cur_op, current_loop, loop_cr, rdinfo, &dominfo);
    		loop_invariant_found = true;
		if (dbg (opti,3)) {
		   cdbg <<"\tinvariant operation moved to pre-header\n";
		   cdbg <<"\tinvariant operation:\n"<<(*cur_op)<<"\tmoved to pre-header\n\n";
		}
	      }
	    }
	  }
	}
      }
    }
    El_remove_region(loop_cr, true);
  }
  El_remove_basic_blocks(R) ;
  // loop invariant code removal must be followed by region_sort_subregions_wrt_layout()
  // to match flow thru paths that get confused after compound region insertions and deletions 
  region_sort_subregions_wrt_layout(R) ;
  // loop invariant code removal must be followed by El_fix_all_pbr_info() to 
  // repair branch targets (via PBR instrs) which do not match exit ops for 
  // the control blocks as a result of pre-header block insertions
  El_fix_all_pbr_info(R, false);
}


