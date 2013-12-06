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



/////////////////////////////////////////////////////////////////////////////
//
//      File:           modulo_variable_expansion.cpp
//      Authors:        Nate Clark
//      Created:        November 2006
//      Description:    Class to handle modulo variable expansion for 
//                      architectures without rotating registers.
//
/////////////////////////////////////////////////////////////////////////////


#include "modulo_variable_expansion.h"
#include "rr_allocation.h"
#include "el_control.h"
#include "hash_functions.h"

#include <iostream>

using namespace std;


Mod_var_expansion::Mod_var_expansion(LoopBody* lb) : loop(lb) { }


void Mod_var_expansion::expand(ModuloScheduler& msched, Cluster_mgr& cmgr,
			       Hash_map<Op*, int>& pre_clst_op_map)
{
  int old_ii = msched.II();
  unsigned times_to_unroll = compute_unroll_times((unsigned)old_ii,
						  (unsigned)msched.ESC());
  if(times_to_unroll > 1) {
    unroll(times_to_unroll, cmgr, pre_clst_op_map);

    cdbg << "Done unrolling" << endl;
    // ir_write(loop, "%d_unrolled", loop->id());

    // Reschedule the loop with the higher II. Note that 'old_ii *
    // times_to_unroll' is pessimistic, since fractional ResMIIs can
    // potentially be combined resulting in lower II. E.g., if ResMII
    // is 2.5 we'd originally have to schedule at II = 3. Once we
    // unroll twice, you could potentially schedule at II 5, though,
    // but here we would just try original II * 2 = 6.

    msched.reinit(old_ii * times_to_unroll);
    if( ! msched.schedule(old_ii * times_to_unroll, El_budget_ratio,
			  (El_ssched_meld_model == NO_MELD ? false : true))) {

      // This should almost never happen. If this error fires it means
      // the modulo scheduler found a schedule for the loop at II, but
      // couldn't find a schedule for N copies of the loop at
      // N*II. Maybe we could increase the budget and try again? Maybe
      // there's a bug in the scheduler? The scheduler is a heuristic,
      // so I won't say it could _never_ happen, even in the absence
      // of bugs.
      El_punt("Modulo Variable Expansion couldn't find a schedule after unrolling!\n");
    } else {
      if(1 || dbg(ms,2))
	cdbg << "Scheduled unrolled loop successfully at II " << old_ii * times_to_unroll
	     << endl;
      
      // Need to adjust the loop counter in the preloop and the
      // preremainder loop based on how many times this was unrolled.
      if(loop->preremainder() == NULL || loop->remainder() == NULL) {
	cdbg << "Trying to do modulo variable expansion on a loop that is not\n"
	     << "structured the way MVE expects it. Something happened between\n"
	     << "the loop setup (El_setup_swp_loop_regions) and this point.\n" << endl;
	assert(0);
      }

      for(Region_all_ops rops(loop->preremainder()); rops != 0; rops++) {
	Op* op = *rops;
	if(is_int_rem(op)) {
	  Operand& step = op->src(SRC2);
	  if(step.is_int()) {
	    step.set_int_value(step.int_value() * times_to_unroll);
	  } else {
	    Op* mpy = new Op((Opcode) MPY_W);
	    assert(mpy && "out of memory");

	    Operand new_oper(new Reg(EL_STANDARD_INT));
	    mpy->set_src(PRED1, op->src(PRED1));
	    mpy->set_dest(DEST1, new_oper);
	    mpy->set_src(SRC1, step);
	    mpy->set_src(SRC2, Operand(new Int_lit(times_to_unroll)));
	    El_insert_op_before(mpy, op);
	    op->set_src(SRC2, new_oper);
	  }
	}
      }

      for(Region_all_ops rops(loop->remainder()); rops != 0; rops++) {
	Op* op = *rops;
	if(is_int_div(op)) {
	  Operand& step = op->src(SRC2);
	  if(step.is_int()) {
	    step.set_int_value(step.int_value() * times_to_unroll);
	  } else {
	    Op* mpy = new Op((Opcode) MPY_W);
	    assert(mpy && "out of memory");

	    Operand new_oper(new Reg(EL_STANDARD_INT));
	    mpy->set_src(PRED1, op->src(PRED1));
	    mpy->set_dest(DEST1, new_oper);
	    mpy->set_src(SRC1, step);
	    mpy->set_src(SRC2, Operand(new Int_lit(times_to_unroll)));
	    El_insert_op_before(mpy, op);
	    op->set_src(SRC2, new_oper);
	  }
	}
      }

    } // else
  } // if(times_to_unroll > 1)

  // Remove the virtualized rotating registers.
  unrotate_registers();
}


unsigned Mod_var_expansion::compute_unroll_times(unsigned II, unsigned ESC)
{
  // The loop needs to be unrolled enough times so that no lifetimes
  // of the same register overlap. Use the the rotating register
  // allocator class just to compute lifetimes.
  RR_allocation rr_alloc(loop, El_lreg_rr_open_loop);
  int longest_lifetime = rr_alloc.get_longest_lifetime(II, ESC);

  // Need to unroll ceil(longest_life / II) times. Exploit the fact
  // that integer division returns the floor.
  unsigned unroll_times = (unsigned) ((longest_lifetime + II - 1) / II);

  if(1 || dbg(ms,2)) {
    cdbg << "MVE: Longest life: " << longest_lifetime << "\tII: " << II
	 << "\tUnroll: " << unroll_times << endl;
  }

  return unroll_times;
}


// This is similar to C0_remove_op(), but it doesn't adjust the
// incoming edge to point to the next op. It just deletes both edges.
static void remove_C0_edges(Op* op)
{
   Edge* src_edge = get_incoming_CONTROL0_edge(op) ;
   Edge* dest_edge = get_outgoing_CONTROL0_edge(op) ;
   op->remove_outedge(dest_edge, CONTROL, CONTROL0, 0);
   op->remove_inedge(src_edge, CONTROL, CONTROL0, 0);

   src_edge->src()->remove_outedge(src_edge, CONTROL, CONTROL0, 0);
   dest_edge->dest()->remove_inedge(dest_edge, CONTROL, CONTROL0, 0);

   delete src_edge;
   delete dest_edge;
}


// Note: This function was only intended to unroll modulo scheduled
// loops. It is not generic enough to unroll loops with arbitrary
// control flow or multiple blocks, and does very few optimizations
// typically associated with loop unrolling (e.g., collapsing multiple
// induction variables).
void Mod_var_expansion::unroll(unsigned times, Cluster_mgr& cmgr,
			       Hash_map<Op*, int>& pre_clst_op_map)
{
  if(1 || dbg(ms,1) || dbg(status,2)) {
    cdbg << "Unrolling loop " << loop->id() << " " << times << " times." << endl;
  }


  ////////////////////////////////////////////////////////////////////////////
  // Remove all the information for the previous schedule. Undraw
  // edges, get rid of remaps, unschedule, etc.

  delete_region_edges(loop);
  el_swp_strip_remaps(loop);
  unrotate_registers();
  cmgr.undo_block_clustering(loop);
  unschedule();

  // Reset the cluster manager's state
  cmgr.op_map = pre_clst_op_map;

  // and Codegen's
  Codegen::prf_bind_map.clear_prf_map();

  delete_liveness();
  redo_liveness_sets();


  ////////////////////////////////////////////////////////////////////////////
  // Find all the ops that need to be cloned.

  List<Op*> ops_to_clone;
  Op* first_branch_op = NULL;
  for(Region_all_ops rops(loop); rops != 0; rops++) {
    Op* op = *rops;
    if(is_pseudo(op) || op->flag(EL_OPER_ICMOVE) || op->flag(EL_OPER_SWP_BRANCH)) {
      if(op->flag(EL_OPER_SWP_BRANCH) && (first_branch_op == NULL)) {
	first_branch_op = op;
      }
      if(op->flag(EL_OPER_SWP_BRANCH) && is_sub(op)) {
	// update the esc and lc subtracts to reflect the unrolling
	op->set_src(SRC2, Operand(new Int_lit(op->src(SRC2).int_value() * times)));
      }
      continue;
    }
    ops_to_clone.add_tail(op);
  }


  ////////////////////////////////////////////////////////////////////////////
  // Copy all non-branch ops

  for(unsigned i = 1; i < times; i++) {
    for(List_iterator<Op*> li(ops_to_clone); li != 0; li++) {
      Op* original = *li;
      Op* copy = original->clone();
      copy->renumber();

      // Need to remove the op, because the copy constructor will copy
      // edges, making the copy think it's already in the code.
      remove_C0_edges(copy);
      El_insert_op_before(copy, first_branch_op);
    }
  }


  ////////////////////////////////////////////////////////////////////////////
  // And rename the registers, to prevent separate instances from
  // artificially generating live ranges too long to allocate.

  rename_registers();

  
  ////////////////////////////////////////////////////////////////////////////
  // Redo all the analysis and setup that's needed for rescheduling

  delete_liveness();
  redo_liveness_sets();
  
  if (El_do_width_analysis) {
    do_width_analysis(loop->kernel(), true);
  }

  cmgr.do_block_clustering(loop->kernel(), El_cluster_method);

  // Assign physical files and bind them
  Codegen::do_prepass_post_cluster_codegen(loop->kernel(), cmgr);

  // Convert to DSA form (needs an updated PQS)
  if (dbg(status,2))
    cdbg << "DSA conversion, Flow Analysis, Edge Drawing for the loop " 
	 << loop->kernel()->id() << endl;

  // FIX: are you sure this must be done here...
  delete_liveness();
  redo_liveness_sets();

  el_swp_dsa_convert_loop(loop);
  // ntclark 1/18/07
  el_swp_strip_remaps(loop);
  unrotate_registers();

  delete_liveness();
  redo_liveness_sets();

  // ntclark 11/3/06: Don't know if this needs to be done... It seems
  // unnecessary. If it does need done, then I believe the ppl region
  // created in modulo_scheduler_driver needs to be passed down to
  // this function.

  // Reanalyze the loop, preloop, and postloop
//   create_local_analysis_info_for_all_hbs_bbs(ppl);
//   el_flow_compute_four_dataflow_sets(ppl);

  if (dbg(ms,1)) {
    Edge* cf_edge = (loop->controlFlowBackEdges())->head();
    cdbg << "Liveness on backedge: \n" << *get_liveness_info(cf_edge) << endl;
    cdbg << "Down exposed uses : \n" << *get_down_exposed_uses_info(cf_edge) << endl;
    cdbg << "Up exposed defs : \n" << *get_up_exposed_defs_info(cf_edge) << endl;
    cdbg << "Down exposed defs : \n" << *get_down_exposed_defs_info(cf_edge) << endl;
  }

  // Construct dependence graph
  insert_region_modulo_edges(loop);
}


static void unrotate_dataflow_set(Liveness_info* dataflow_set)
{
  if (dataflow_set == NULL)
    return;

  for (Liveness_info_iterator liter(*dataflow_set) ; liter != 0 ; liter++) {
    Operand& liveop = *liter;
    if(liveop.is_rotating()) {
      liveop.bind_file(liveop.file_type(), STATIC_REG);
    }
    if(liveop.is_reg() && (liveop.omega() != 0)) {
      liveop.set_omega(0);
    }
  }
}


void Mod_var_expansion::unschedule()
{
  for(Region_all_ops rops(loop); rops != 0; rops++) {
    Op* op = *rops;
    op->set_sched_time(-1);
    op->set_sched_opcode(NULL);
  }  
}


void Mod_var_expansion::unrotate_registers()
{
  for(Region_all_ops rops(loop); rops != 0; rops++) {
    Op* op = *rops;

    // Not 100% sure why we only look at explicit inputs/dests instead
    // of all inputs/dests, but that's the iterator used in
    // el_swp_dsa_convert.cpp that converts the static to rotating, so
    // we'll follow suit for rotating to static conversion.
    for(Op_explicit_inputs src_i(op) ; src_i != 0 ; src_i++) {
      Operand& tmp_src = *src_i ;
      if(tmp_src.is_reg()) {
	if(tmp_src.is_rotating())
	  tmp_src.bind_file(tmp_src.file_type(), STATIC_REG);
	tmp_src.set_omega(0);
      }
    }
    for(Op_explicit_dests dest_i(op) ; dest_i != 0 ; dest_i++) {
      Operand& tmp_dest = *dest_i ;
      if(tmp_dest.is_reg()) {
	if(tmp_dest.is_rotating())
	  tmp_dest.bind_file(tmp_dest.file_type(), STATIC_REG);
	tmp_dest.set_omega(0);
      }
    }
  }

  // Need to update the liveness info as well.
  for(Region_entry_edges eni(loop); eni != 0; eni++) {
    Edge* inedge = *eni;
    unrotate_dataflow_set(get_liveness_info(inedge));
    unrotate_dataflow_set(get_down_exposed_uses_info(inedge));
    unrotate_dataflow_set(get_up_exposed_defs_info(inedge));
    unrotate_dataflow_set(get_down_exposed_defs_info(inedge));
  }

  for(Region_exit_edges exi(loop); exi != 0; exi++) {
    Edge* outedge = *exi;
    unrotate_dataflow_set(get_liveness_info(outedge));
    unrotate_dataflow_set(get_down_exposed_uses_info(outedge));
    unrotate_dataflow_set(get_up_exposed_defs_info(outedge));
    unrotate_dataflow_set(get_down_exposed_defs_info(outedge));
  }

  // It appears the backedge isn't considered an entry or exit edge.
  Edge* edge = get_outgoing_CONTROL0_non_fall_through_edge(get_last_region_op_from_outops(loop));
  unrotate_dataflow_set(get_liveness_info(edge));
  unrotate_dataflow_set(get_down_exposed_uses_info(edge));
  unrotate_dataflow_set(get_up_exposed_defs_info(edge));
  unrotate_dataflow_set(get_down_exposed_defs_info(edge));
}


void Mod_var_expansion::rename_registers()
{
  Procedure* proc = el_get_enclosing_procedure(loop);
  create_local_analysis_info_for_all_hbs_bbs(proc);
  el_flow_compute_liveness(proc, ANALYZE_ALLREG);
  El_do_reaching_defs(proc, ANALYZE_ALLREG);

  Reaching_defs_info* rdef = get_reaching_defs_info(proc);

  Hash_map<El_ref, int> ref_to_web(hash_El_ref, 137);
  int current_web_number = 0;

  ////////////////////////////////////////////////////////////////////////////
  // First construct webs for all virtual registers that could
  // potentially communicate with one another. All references within a
  // web must have the same virtual register number. References in
  // separate webs can (and should) have different register numbers.

  for(Region_all_ops rops(proc); rops != 0; rops++) {
    Op* op = *rops;

    for(Op_explicit_inputs ins(op); ins != 0; ins++) {
      Operand& oper = *ins;
      if(oper.is_reg()) {
	El_ref use(op, &oper, EXP_SRC);
	List<El_ref>* defs = rdef->get_ud_chain(use);

	for(List_iterator<El_ref> li(*defs); li != 0; li++) {
	  El_ref& def = *li;

	  if( ! ref_to_web.is_bound(def) && ! ref_to_web.is_bound(use)) {
	    // Neither ref is bound, so create a new web number for
	    // this def-use pair.

	    ref_to_web.bind(def, current_web_number);
	    ref_to_web.bind(use, current_web_number);
	    current_web_number++;
	  } else if( ref_to_web.is_bound(def) && ! ref_to_web.is_bound(use)) {
	    ref_to_web.bind(use, ref_to_web[def]);
	  } else if( ref_to_web.is_bound(use) && ! ref_to_web.is_bound(def)) {
	    ref_to_web.bind(def, ref_to_web[use]);
	  } else if(ref_to_web[def] != ref_to_web[use]) {
	    // If they're both assigned to webs, then we need to merge
	    // the two webs. Change the use web to match the def web.

	    List<El_ref> to_change;
	    for(Hash_map_iterator<El_ref, int> hmi(ref_to_web); hmi != 0; hmi++) {
	      Pair<El_ref, int>& pair = *hmi;
	      if(pair.second == ref_to_web[use]) {
		to_change.add_tail(pair.first);
	      }
	    }

	    for(List_iterator<El_ref> li2(to_change); li2 != 0; li2++) {
	      El_ref& ref = *li2;
	      ref_to_web.unbind(ref);
	      ref_to_web.bind(ref, ref_to_web[def]);
	    }
	  }
	} // for each def
      } // if oper.is_reg()
    } // for each explicit input

    for(Op_explicit_dests outs(op); outs != 0; outs++) {
      Operand& oper = *outs;
      if(oper.is_reg()) {
	El_ref def(op, &oper, EXP_DEST);
	List<El_ref>* uses = rdef->get_du_chain(def);

	for(List_iterator<El_ref> li(*uses); li != 0; li++) {
	  El_ref& use = *li;
	  
	  if( ! ref_to_web.is_bound(def) && ! ref_to_web.is_bound(use)) {
	    // Neither ref is bound, so create a new web number for
	    // this def-use pair.

	    ref_to_web.bind(def, current_web_number);
	    ref_to_web.bind(use, current_web_number);
	    current_web_number++;
	  } else if( ref_to_web.is_bound(def) && ! ref_to_web.is_bound(use)) {
	    ref_to_web.bind(use, ref_to_web[def]);
	  } else if( ref_to_web.is_bound(use) && ! ref_to_web.is_bound(def)) {
	    ref_to_web.bind(def, ref_to_web[use]);
	  } else if(ref_to_web[def] != ref_to_web[use]) {
	    // If they're both assigned to webs, then we need to merge
	    // the two webs. Change the use web to match the def web.
	    
	    List<El_ref> to_change;
	    for(Hash_map_iterator<El_ref, int> hmi(ref_to_web); hmi != 0; hmi++) {
	      Pair<El_ref, int>& pair = *hmi;
	      if(pair.second == ref_to_web[use]) {
		to_change.add_tail(pair.first);
	      }
	    }

	    for(List_iterator<El_ref> li2(to_change); li2 != 0; li2++) {
	      El_ref& ref = *li2;
	      ref_to_web.unbind(ref);
	      ref_to_web.bind(ref, ref_to_web[def]);
	    }
	  }
	} // for each use
      } // if oper.is_reg()
    } // for each explicit dests

  } // for all ops

  ////////////////////////////////////////////////////////////////////////////
  // Now that webs have been constructed, make sure separate webs have
  // separate register numbers.

  // First, collect each web
  Vector<List<El_ref> > webs(current_web_number);
  for(Hash_map_iterator<El_ref, int> hmi(ref_to_web); hmi != 0; hmi++) {
    Pair<El_ref, int>& pair = *hmi;
    webs[pair.second].add_tail(pair.first);
  }

  // As a sanity check, make sure all the webs only have one VR number
  for(unsigned i = 0; i < webs.size(); i++) {
    if(webs[i].size()) {
      List_iterator<El_ref> li(webs[i]);
      int first_vr = (*li).get_operand().vr_num();
      for(li++; li != 0; li++) {
	if((*li).get_operand().vr_num() != first_vr) {
	  cdbg << "VRs differ within a web!?: " << first_vr << "\t"
	       << (*li).get_operand().vr_num() << endl;
	  assert(0);
	}
      }
    }
  }

  // Now rename webs that have duplicate vr numbers
  Hash_set<int> vrs_seen(hash_int, 37);
  for(unsigned i = 0; i < webs.size(); i++) {
    if(webs[i].size()) {
      List_iterator<El_ref> li(webs[i]);
      int vr = (*li).get_operand().vr_num();

      if(vrs_seen.is_member(vr)) {
	// Need to rename this web.

	(*li).get_operand().rename();
	int new_vr = (*li).get_operand().vr_num();

	// cdbg << "Renaming VR " << vr << " to " << new_vr << endl;
	for(li++; li != 0; li++) {
	  (*li).get_operand().rename(new_vr);
	}
      } else {
	vrs_seen += vr;
      }
    }
  }

  remove_reaching_defs_info(proc);
  delete_local_analysis_info_for_all_hbs_bbs(proc);
}


void Mod_var_expansion::redo_liveness_sets()
{
  create_local_analysis_info_for_all_hbs_bbs(loop);
  el_flow_compute_liveness(loop, ANALYZE_ALLREG);
  Msched_attr* ms_attr = get_msched_attr(loop);
  if(NULL == ms_attr) {
    ms_attr = new Msched_attr;
    assert(ms_attr && "no more memory");
    set_msched_attr(loop, ms_attr);
  }
  el_flow_compute_four_dataflow_sets(loop->kernel(), ANALYZE_ALL,
				     ms_attr->liveness_gen,
				     ms_attr->liveness_kill,
				     ms_attr->down_exposed_use_gen,
				     ms_attr->down_exposed_use_kill,
				     ms_attr->up_exposed_def_gen,
				     ms_attr->up_exposed_def_kill,
				     ms_attr->down_exposed_def_gen,
				     ms_attr->down_exposed_def_kill);
}


void Mod_var_expansion::delete_liveness()
{
  // Delete information on the internal edges.
  for (Region_all_internal_edges ei(loop) ; ei != 0 ; ei++) {
    Edge* e_ptr = *ei ;
    if (is_CONTROL0(e_ptr)) {
      remove_liveness_info(e_ptr);
      remove_down_exposed_uses_info(e_ptr);
      remove_up_exposed_defs_info(e_ptr);
      remove_down_exposed_defs_info(e_ptr);
    }
  }

  // Now the incoming and outgoing edges.
  for(Region_entry_edges eni(loop); eni != 0; eni++) {
    Edge* e_ptr = *eni;
    remove_liveness_info(e_ptr);
    remove_down_exposed_uses_info(e_ptr);
    remove_up_exposed_defs_info(e_ptr);
    remove_down_exposed_defs_info(e_ptr);
  }

  for(Region_exit_edges exi(loop); exi != 0; exi++) {
    Edge* e_ptr = *exi;
    remove_liveness_info(e_ptr);
    remove_down_exposed_uses_info(e_ptr);
    remove_up_exposed_defs_info(e_ptr);
    remove_down_exposed_defs_info(e_ptr);
  }
  delete_local_analysis_info_for_all_hbs_bbs(loop);
}
