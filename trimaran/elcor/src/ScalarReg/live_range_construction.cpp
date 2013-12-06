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
//      File:           live_range_construction.cpp
//      Authors:        Nate Clark (heavily copied from code by Hansoo Kim)
//      Created:        December 2004
//      Description:    
//
/////////////////////////////////////////////////////////////////////////////

#include "adjlist_graph.h"
#include "attributes.h"
#include "edge_utilities.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "operand.h"
#include "region.h"
#include "live_range.h"
#include "live_range_construction.h"
#include "pred_analysis.h"
#include "el_init.h"



////////////////////////////////////////////////////////////////////////////////
//
// This functions creates live ranges for each operand in the compound
// region. This is accomplished by recursing down to the hb/bb level
// and doing a backwards walk over each operation. Once the analysis
// is performed on each block, live ranges on identical variables are
// combined. It's assumed that all live ranges in a function are
// eventually combined.
//
////////////////////////////////////////////////////////////////////////////////

List<Live_range*>
Live_range_constructor::construct_new_live_ranges(Compound_region* r,
						  const eString& phys_file,
						  bool first_pass)
{
  List<Live_range*> lr_list;

  if (r->is_bb() || r->is_hb()) {
    // do the walk over operations
    lr_list = construct_live_ranges_for_block(r, phys_file, first_pass);
  } else { 
    // recurse until we get to the hb/bb level

    // This should be a hash map, but you can't hash operands during
    // allocation.
    Map<Operand, Live_range*> lr_map;
    List<Live_range*> lrs;

    // Traverse all sub-regions in topologically sorted order
    Alist_region_tiling_graph sub_regs(r, GRAPH_OPS_PLUS);
    for (Alist_graph_post_dfs r_iter(sub_regs, UP); r_iter != 0; r_iter++) {
      int index = *r_iter;
      List<Live_range*> sub_lrs = 
	construct_new_live_ranges((Compound_region*) sub_regs.b_map[index],
				  phys_file, first_pass);

      // Combine all the Live_ranges for operands that already have
      // Live_ranges defined. i.e., make sure there's only one
      // Live_range for each variable.
      for (List_iterator<Live_range*> m_iter(sub_lrs); m_iter != 0; ++m_iter) {
	Live_range* sub_lr = *m_iter;
	Operand var = sub_lr->get_oper();
	if (lr_map.is_bound(var)) {
	  lr_map[var]->combine_with(sub_lr);
	  delete sub_lr;
	} else {
	  lr_map.bind(var, sub_lr);
	}
      } // for each sub_lr
    } // for each subregion

    for(Map_iterator<Operand, Live_range*> mi(lr_map); mi != 0; mi++) {
      lr_list.add_tail((*mi).second);
    }
  }
  return lr_list;
}


////////////////////////////////////////////////////////////////////////////////
//
// This is the code for initializing pseudo sources before you call the
// Op_pseudo_sources iterator. Call this function before any call to
// Op_pseudo_sources, preferably call it right in the beginning of the
// intra-region allocation.
//
// This is a cleaned-up version of some of the code in reaching
// definitions. See the function Reaching_defs_maps::Reaching_defs_maps
// in file reaching_defs_solver.cpp.
//
////////////////////////////////////////////////////////////////////////////////

void Live_range_constructor::initialize_pseudo_sources(Compound_region* r)
{
  //
  // Use liveness information for entry/exit processing
  //
  // Compute pseudo defs for entry operations
  //
  for (Region_entry_ops en_op_iter(r); en_op_iter != 0; ++en_op_iter) {
    Op* cur_op = *en_op_iter;
    List<Operand>* entry_pseudo_defs = 
      (List<Operand>*)get_generic_attribute(cur_op, "entry_pseudo_defs");
    if (entry_pseudo_defs == NULL) {
      entry_pseudo_defs = new List<Operand>();
    } else {
      entry_pseudo_defs->clear();
    }
    for(Region_entry_edges en_edge_iter(r); en_edge_iter != 0; ++en_edge_iter) {
      Edge* cur_edge = *en_edge_iter;
      if (cur_edge->dest() != cur_op) continue;
	    
      Liveness_info* linfo = get_liveness_info(cur_edge);
      if (linfo) {
	for (Liveness_info_iterator li(*linfo); li != 0; ++li) {
	  Operand& tmpoper = *li;
	  if (!entry_pseudo_defs->is_member(tmpoper)) {
	    entry_pseudo_defs->add_tail(tmpoper);
	  }
	}
      }
    }

    if (entry_pseudo_defs->is_empty()) {
      delete entry_pseudo_defs;
      remove_generic_attribute(cur_op, "entry_pseudo_defs");
    } else {
      set_generic_attribute(cur_op, "entry_pseudo_defs", entry_pseudo_defs);
    }
  }
   
  //
  // Now compute pseudo uses for exit ops
  //

  for(Region_exit_ops ex_op_iter(r); ex_op_iter != 0; ++ex_op_iter) {
    Op* cur_op = *ex_op_iter;
    List<Operand>* taken_exit_pseudo_uses = 
      (List<Operand>*)get_generic_attribute(cur_op, "taken_exit_pseudo_uses");
    List<Operand>* fallthrough_exit_pseudo_uses = 
      (List<Operand>*)get_generic_attribute(cur_op, "fallthrough_exit_pseudo_uses");
    if (taken_exit_pseudo_uses == NULL) {
      taken_exit_pseudo_uses = new List<Operand>();
    } else {
      taken_exit_pseudo_uses->clear();
    }
    if (fallthrough_exit_pseudo_uses == NULL) {
      fallthrough_exit_pseudo_uses = new List<Operand>();
    } else {
      fallthrough_exit_pseudo_uses->clear();
    }
    for(Region_exit_edges ex_edge_iter(r); ex_edge_iter != 0; 
	ex_edge_iter++) {
      Edge* cur_edge = *ex_edge_iter;
      if (cur_edge->src() != cur_op)
	continue;
	    
      List<Operand>* tmp_exit_pseudo_uses;
      if (is_fall_through(cur_edge)) {
	tmp_exit_pseudo_uses = fallthrough_exit_pseudo_uses;
      } else {
	tmp_exit_pseudo_uses = taken_exit_pseudo_uses;
      }

      Liveness_info* linfo = get_liveness_info(cur_edge);
      if (linfo) {
	for (Liveness_info_iterator li(*linfo); li != 0; ++li) {
	  Operand& tmpoper = *li;
	  if (!tmp_exit_pseudo_uses->is_member(tmpoper)){
	    tmp_exit_pseudo_uses->add_tail(tmpoper);
	  }
	}
      }
    }

    if (taken_exit_pseudo_uses->is_empty()) {
      delete taken_exit_pseudo_uses;
      remove_generic_attribute(cur_op, "taken_exit_pseudo_uses");
    } else {
      set_generic_attribute (cur_op, "taken_exit_pseudo_uses", taken_exit_pseudo_uses);
    }

    if (fallthrough_exit_pseudo_uses->is_empty()) {
      delete fallthrough_exit_pseudo_uses;
      remove_generic_attribute(cur_op, "fallthrough_exit_pseudo_uses");
    } else {
      set_generic_attribute (cur_op, "fallthrough_exit_pseudo_uses",
			     fallthrough_exit_pseudo_uses);
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
//
// Returns true if Live_ranges don't need to be constructed for the
// Macro. I.e., you can't allocate to the register.
// 
////////////////////////////////////////////////////////////////////////////////

bool Live_range_constructor::no_lr_needed(Macro_name name)
{
  // We don't need a live range if it's a pseudo macro, alias for the
  // stack pointer (RGS), RGS itself, or a read only macro since they
  // won't interfere with other live ranges. FYI, RGS is short for
  // ReGister Swap, which is the stack frame defined for this region
  // of code (I didn't come up with that abbreviation).
  if (name == RGS_REG)
    return true;
  if (name == SP_REG)
    return true;
  if (name == ALL_ROT_PRED)
    return true;
  if (name == ALL_PRED)
    return true;
  if (name == ALL_STATIC_PRED)
    return true;
  if (name == PV_0)
    return true;
  if (name == PV_1)
    return true;
  if (name == PV_2)
    return true;
  if (name == PV_3)
    return true;
  if (name == PV_4)
    return true;
  if (name == PV_5)
    return true;
  if (name == PV_6)
    return true;
  if (name == PV_7)
    return true;

  eString the_name = el_macro_to_string_map[name];

  Hash_set<eString>& pseudos = current_MDES->get_MDES_pseudo_macros();
  if (pseudos.is_member(the_name))
    return true;
  Hash_set<eString>& ro = current_MDES->get_MDES_read_only_macros();
  if (ro.is_member(the_name))
    return true;
  Hash_set<eString>& aliases = current_MDES->get_MDES_alias_macros();
  if (aliases.is_member(the_name) &&
      current_MDES->get_macro_aliased(the_name) == "RGS_REG")
    return true;
  if (aliases.is_member(the_name) &&
      current_MDES->get_macro_aliased(the_name) == "SP_REG")
    return true;

  return false;
}


////////////////////////////////////////////////////////////////////////////////
// 
// Walks through a Hyperblock or Basicblock in backwards order, and
// computes the liveness of each variable (operand) encountered. This
// information is stored in a Live_range. All of the Live_ranges
// constructed are returned.
//
////////////////////////////////////////////////////////////////////////////////

List<Live_range*> 
Live_range_constructor::construct_live_ranges_for_block(Compound_region* r,
							const eString& phys_file,
							bool first_pass)
{
  assert(r->is_bb() || r->is_hb());
  
  List<Live_range*> live_range_list;

  // These should be Hash_maps, and making the hash_maps yields a
  // noticeable speed improvement, however, there are problems hashing
  // operands. Specifically, because some operands may be bound to
  // physical registers and others not bound, they may hash to
  // different places, even though they would still be ==.
  Map<Operand, Live_range*> live_range_map;
  Map<Operand, Pred_cookie> liveness_map;
  Map<Operand, int>         liveness_time;

  // The pseudo sources tell the compiler when operands are live-out
  // of branches to other parts of the code. The pseudo sources don't
  // change from inserting spill code, so there's no need to
  // re-initialize them after the first pass through iterative
  // coloring.
  if(first_pass) {
    initialize_pseudo_sources(r);
  } else {
    // FIX: We need to have the predicates for the inserted spill code
    // show up in the Pred_jar, so I recompute analysis here. I should
    // make the spill code generator iteratively update the
    // PredicateMap in the pqs data structure instead of
    // recomputing. This is a performance, not correctness, issue.
    create_local_analysis_info(r);
  }

  Pred_jar pj(r);

  // This procedure for constructing live ranges originally assumed
  // that the code was unscheduled, or that it would be rescheduled
  // later in compilation. However, modulo variable expansion requires
  // that the code not be rescheduled, so we need to do a few more
  // checks in this procedure.
  bool softpipe_region = (El_msched_allocator == MODULO_VARIABLE_EXPANSION) &&
    r->flag(EL_REGION_SOFTPIPE) && r->flag(EL_REGION_SCHEDULED);

  for (Region_ops_reverse_C0_order ops(r); ops != 0; ++ops) {
    Op* op = *ops;
    
    if (is_branch(op)) {

      // SAM 9-2002
      for(Op_pseudo_sources lives(op); lives != 0; ++lives) {
	El_ref cur_ref = lives.get_ref();
	Operand& var = cur_ref.get_operand();
	if ( ! (var.is_reg() || var.is_macro_reg()) )
	  continue;

	if(phys_file != var.physical_file_type())
	  continue;

	// rotating allocation is done elsewhere
	if (var.is_rotating())
	  continue;

	if (var.is_macro_reg() && no_lr_needed(var.name()))
	  continue;

	if (cur_ref.get_ref_type() != LIVEOUT_FALLTHROUGH_SRC)
	  continue;

	// Spilled vrs don't need to include pseudo sources in their
	// live ranges, because spill code will be generated before
	// each use anyway.
  	if(var.is_reg() && spilled_vrs.is_member(var.vr_num()))
  	  continue;

	// There's no reason to check if var is allocated, because
	// pseudo sources are never bound with a register number.

	Pred_cookie guard = pj.get_lub_guard(cur_ref);
	if (liveness_map.is_bound(cur_ref.get_operand()))
	  El_punt("local_liveness_walk: liveness map should be empty!");
	liveness_map.bind(cur_ref.get_operand(), guard);
	if(softpipe_region)
	  liveness_time.bind(cur_ref.get_operand(), op->sched_time());
      }
      // End SAM 9-2002
			
      for(Op_pseudo_sources lives(op); lives != 0; ++lives) {
	El_ref cur_ref = lives.get_ref();
	Operand& var = cur_ref.get_operand();
	if ( ! (var.is_reg() || var.is_macro_reg()))
	  continue;

	if(phys_file != var.physical_file_type())
	  continue;

	// rotating allocation is done elsewhere
	if (var.is_reg() && var.is_rotating())
	  continue;

	if (var.is_macro_reg() && no_lr_needed(var.name()))
	  continue;

	if (cur_ref.get_ref_type() == LIVEOUT_FALLTHROUGH_SRC)
	  continue;

	// Spilled vrs don't need to include pseudo sources in their
	// live ranges, because spill code will be generated before
	// each use anyway.
  	if(var.is_reg() && spilled_vrs.is_member(var.vr_num()))
  	  continue;

	// There's no reason to check if var is allocated, because
	// pseudo sources are never bound with a register number.

	Pred_cookie guard = pj.get_lub_guard(cur_ref);
	if (liveness_map.is_bound(cur_ref.get_operand())) {
	  Pred_cookie& p = liveness_map.value(cur_ref.get_operand());
	  p.lub_sum(guard);
	  if (p == Pred_jar::get_false()) {
	    liveness_map.unbind(cur_ref.get_operand());
	    if(softpipe_region) {
	      liveness_time.unbind(cur_ref.get_operand());
	    }
	  }
	} else {
	  if (guard != Pred_jar::get_false()) {
	    liveness_map.bind(cur_ref.get_operand(), guard);
	    if(softpipe_region) {
	      liveness_time.bind(cur_ref.get_operand(), op->sched_time());
	    }
	  }
	}
      }	// end for op_pseudo_sources
    }	// end if is a branch op

    // Removed 8-21-2006 mchu
    // Unallocated registers in pseudo ops can cause problems in edge drawing,
    // which creates a Hash_map of operands.  Mixing allocated/unallocated in 
    // a map is bad.    
    //    if (is_pseudo(op)) continue;

    // update Live_range
    for (Map_iterator<Operand, Pred_cookie> iter(liveness_map); iter != 0; ++iter) {
      Pair<Operand, Pred_cookie>& ele = *iter;
      Operand& var = ele.first;

      if(phys_file != var.physical_file_type())
	continue;

      if(var.is_rotating())
	continue;

      if(var.allocated()) {
	if(live_range_map.is_bound(var)) {
	  live_range_map.unbind(var);
	}
	continue;
      }

      Live_range* live_range = NULL;
      if (live_range_map.is_bound(var)) {
	live_range = live_range_map.value(var);
      } else {
 	live_range = new Live_range(var);
	live_range_list.add_tail(live_range);
	live_range_map.bind(var, live_range);
      }

      live_range->add_op(op, ele.second);
    }	// end for(Map_iterator)

    for(Op_all_dests dests(op); dests != 0; ++dests) {
      El_ref cur_ref = dests.get_ref();
      Operand& var = cur_ref.get_operand();

      if( ! (var.is_reg() || var.is_macro_reg()))
	continue;

      if(phys_file != var.physical_file_type())
	continue;

      if(var.is_reg() && var.is_rotating())
	continue;

      if(var.is_macro_reg() && (no_lr_needed(var.name())))
	continue;

      if(var.allocated()) {
	if(live_range_map.is_bound(var)) {
	  live_range_map.unbind(var);
	}
	continue;
      }

      Pred_cookie guard = pj.get_glb_guard(cur_ref);

      Live_range* live_range = NULL;
      if (live_range_map.is_bound(var)) {
	live_range = live_range_map.value(var);
      } else {
	live_range = new Live_range(var);
	live_range_list.add_tail(live_range);
	live_range_map.bind(var, live_range);
      }

      if (liveness_map.is_bound(cur_ref.get_operand())) {
	Pred_cookie& p = liveness_map.value(cur_ref.get_operand());
	p.lub_diff(guard);
	if (p == Pred_jar::get_false()) {
	  if(softpipe_region) {
	    // In software pipelined code, scheduling has already been
	    // done, meaning we have to check whether or not dests are
	    // written before the src that is potentially unbound.

	    if(op->sched_time() + op->flow_time(DEST, cur_ref.get_port_num()) <=
	       liveness_time[cur_ref.get_operand()]) {
	      liveness_map.unbind(cur_ref.get_operand());
	    }
	  } else {
            // HACK: don't let SVMOVE (scalar-to-vec move) kill, as we cannot
            // analyze them right now.  -KF 1/2007
            if (is_svmove(op))
              live_range->add_op(op, guard);
            else
              liveness_map.unbind(cur_ref.get_operand());
	  }
	}
      } else {
 	live_range->add_op(op, guard);
      }

      live_range->add_ref(cur_ref);
    } // end for all dests

    for(Op_all_inputs ins(op); ins != 0; ++ins) {
      El_ref cur_ref = ins.get_ref();
      Operand& var = cur_ref.get_operand();

      if( ! (var.is_reg() || var.is_macro_reg()))
	continue;

      if(phys_file != var.physical_file_type())
	continue;

      if(var.is_reg() && var.is_rotating())
	continue;

      if(var.is_macro_reg() && (no_lr_needed(var.name())))
	continue;

      if(var.allocated()) {
	if(live_range_map.is_bound(var)) {
	  live_range_map.unbind(var);
	}
	continue;
      }

      Pred_cookie guard = pj.get_lub_guard(cur_ref);
      if (liveness_map.is_bound(var)) {
	Pred_cookie& p = liveness_map.value(cur_ref.get_operand());
	p.lub_sum(guard);
      } else {
	liveness_map.bind(var, guard);
      }
      liveness_time.bind(var, op->sched_time());

      Live_range* live_range = NULL;
      if (live_range_map.is_bound(var)) {
	live_range = live_range_map.value(var);
      } else {
	live_range = new Live_range(var);
	live_range_list.add_tail(live_range);
	live_range_map.bind(var, live_range);
      }

      live_range->add_ref(cur_ref);
      live_range->add_op(op, liveness_map.value(var));
    }	// end for all inputs
    
  } // end for reverse order ops

  // Live ranges for operands that have already been allocated still
  // get created because the pseudo sources on the branches, but they
  // don't have any references. Those live ranges already exist, and
  // are in the interference graph, so we don't want them in the list
  // that's returned.
  if( ! first_pass) {
    List<Live_range*> to_remove;
    for(List_iterator<Live_range*> li(live_range_list); li != 0; li++) {
      Live_range* lr = *li;
      if(lr->num_refs() == 0) {
	to_remove.add_tail(lr);
      }
    }

    for(List_iterator<Live_range*> li(to_remove); li != 0; li++) {
      live_range_list.remove(*li);
      delete *li;
    }
  }

  // Remove ops that only define the operand from the live range. This
  // is done so that vr1 and vr2 can use the same register in this example.
  // 
  // op1  vr1 = ...
  // op2  vr2 = vr1
  // op3  ... = vr2
  // 
  // Before this transformation, vr1's live range would have op1 &
  // op2; vr2 would have op2 & op3. Afterward vr1 will have op2, and
  // vr2 will have op3.

  Hash_set<Op*> src_ops(hash_op_ptr, 64);
  List<Op*> dest_ops;
  for(List_iterator<Live_range*> li(live_range_list); li != 0; li++) {
    Live_range* lr = *li;

    // Don't remove define ops for PR live ranges.  Otherwise
    // put_spill_in_live_ranges might not add spill code to this live range.  Example:
    //
    // p1 = pred_clear
    // ...
    // (some spill code for a register other than p1)
    // p1 = CMPP_ON
    // ...
    // use of p1 
    //
    // The spill code should be added to p1's live range, but since the CMPP op
    // is a define, it would not be in p1's live range, and the spill code won't
    // get added.  (put_spill_in_live_ranges looks at the op following the spill
    // and adds the spill ops to the live ranges containing that op.)
    // -KF, mchu 1/2006

    if(lr->oper.file_type() == PR)
      continue;

    src_ops.clear();
    dest_ops.clear();

    for(List_iterator<El_ref> ri(lr->refs); ri != 0; ri++) {
      El_ref& ref = *ri;
      if(ref.get_ref_type() == EXP_SRC || ref.get_ref_type() == IMP_SRC ||
	 ref.get_ref_type() == PRED_SRC) {
	src_ops += ref.get_op();
      } else if(ref.get_ref_type() == EXP_DEST) {
	if( ! is_brl(ref.get_op()))
	  dest_ops.add_tail(ref.get_op());
      } else if(ref.get_ref_type() == IMP_DEST) {
	// just ignore these.
      } else {
	cdbg << "WARNING: I don't know what kind of ref this is:\n"
	     << ref << endl;
      }
    }

    for(List_iterator<Op*> dests(dest_ops); dests != 0; dests++) {
      Op* dst = *dests;
      // ARM has a quirk, where the first source of a multiply can't
      // be the same as the dest. Make sure that sources and dests of
      // multiplies interfere.
      if( ! strcasecmp(El_arch, "arm")) {
        extern Hash_set<Op*> ugly_hack_ops;
       
	if(is_mul(dst) || is_abs(dst) || ugly_hack_ops.is_member(dst))
	  continue;
      }

      if( ! src_ops.is_member(dst)) {
	// FIX: This too conservative. Think about a better way.
	if(dst->src(PRED1).is_predicate_true()) {
	  lr->ops_set -= dst;
	}
      }
    }
  }

  // do some initialization on the live ranges
  for(List_iterator<Live_range*> li(live_range_list); li != 0; li++) {
    (*li)->calculate_priority();
  }

  return live_range_list;
}



