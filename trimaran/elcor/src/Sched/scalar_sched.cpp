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
//      File:           scalar_sched.cpp
//      Authors:        Santosh G. Abraham, Vinod Kathail
//      Created:        November 1995
//      Description:    Scalar scheduler driver
//
/////////////////////////////////////////////////////////////////////////////

#include "relax_dep_constr.h"
#include "graph_filters.h"
#include "sched_filters.h"
#include "sched_functions.h"
#include "schedule.h"
#include "priority.h"
#include "cycle.h"
#include "unsched.h"
#include "meld.h"
#include "meld_propagate.h"
#include "meld_constraints.h"
#include "meld_check.h"
#include "meld_pad.h"
#include "sched_pad.h"
#include "el_ssched_init.h"
#include "print_sched.h"
#include "scalar_sched.h"
#include "edge_drawing.h"
#include "el_bb_tools.h"
#include "el_dot_tools.h"
#include "ir_reader.h"
#include "ir_writer.h"
#include "hash_functions.h"
#include "op_info.h"
#include "dbg.h"
#include "attributes.h"
#include "intf.h"	// defines MDES_max_op_lat
#include "opcode_properties.h"
#include "el_control_init.h"
#include "el_control.h"
#include "el_stats_init.h"
#include "el_clock.h"
#include "el_clock_init.h"
#include "edge_utilities.h"
#include "el_codegen_init.h"
#include "ls_get_profile.h"
#include "load_par.h"
#include "ls_cacheopt.h"
#include "el_cache_init.h"

void display_sched (Compound_region *r);
Vector<Edge_filter* > * efilt_arr;

void meld_entry(Compound_region* region_sched, Hash_map<Op*, int>& early_map)
{
  Map<Pair<Compound_region*, Op*>, Pair<int, bool> > dist_map;
  Latency_map *import_def_map, *import_use_map;
  int cutoff_dist;

  cutoff_dist = MDES_max_op_lat;
  Region_entry_ops ei (region_sched);
  Op* entry_op = (*ei);
  MS_constraints* msc = get_ms_constraints(region_sched);
  import_def_map = msc->import_def_at_entry(entry_op);
  import_use_map = msc->import_use_at_entry(entry_op);

  min_dist_to_entry (region_sched, cutoff_dist, dist_map);
  propagate_to_entry_lat_maps (region_sched, dist_map,
			import_def_map, import_use_map);
  set_merge_list (region_sched, import_def_map, import_use_map);
  insert_region_meld_edges (region_sched);
  set_entry_edge_latencies(region_sched, entry_op,
		 import_def_map, import_use_map);
  if (dbg(ss, 6)) {
    cdbg << "Meld_entry: prelim version for region " << region_sched->id() << ":  " << *msc << endl;
  }
  if (dbg (ss, 5)) {
    check_entry_edge_lats(region_sched, entry_op,
			  import_def_map, import_use_map);
  }
  if (dbg(ss, 6)) {
    IR_outstream schedbug ("cout");
    ir_write(schedbug, region_sched);
  }
  calculate_earliest_times_from_entry_op(region_sched, entry_op,
			 early_map, NON_CONTROL0_INEDGES);
  set_prelim_entry_export_latency_maps(region_sched, &early_map);
}

void meld_exits(Compound_region* region_sched, Hash_map<Op*, int>& early_map)
{
  Map<Pair<Compound_region*, Op*>, int > dist_map;
  Latency_map* import_def_map;
  Latency_map* import_use_map;
  List<Operand> swdeflist;
  int in_dangle, cutoff_dist;

  MS_constraints* msc = get_ms_constraints (region_sched);

  // cutoff distance =
  // max (incoming dangles - etime_of_exit , edge latencies)
  // calculate max of incoming dangles here
  Region_entry_ops ei (region_sched);
  Op* entry = (*ei);
  Latency_map* entry_import_def_map = msc->import_def_at_entry(entry);
  Latency_map* entry_import_use_map = msc->import_use_at_entry(entry);
  in_dangle = 0;
  for (Latency_map_iterator di (*entry_import_def_map); di != 0; di++) {
    in_dangle = ELCOR_MAX (in_dangle, (*di).second);
  }
  for (Latency_map_iterator ui (*entry_import_use_map); ui != 0; ui++) {
    in_dangle = ELCOR_MAX (in_dangle, (*ui).second);
  }
  
  bool redraw_edges = false;
  for (Region_exit_ops ptexop(region_sched); ptexop !=0; ptexop++) {
    Op* exit_op_sched = (*ptexop);
    cutoff_dist = ELCOR_MAX (0, in_dangle - early_map.value(exit_op_sched));
    import_def_map = msc->import_def_at_exit(exit_op_sched);
    import_use_map = msc->import_use_at_exit(exit_op_sched);
    
    for (List_iterator<Op*> ptsmop (exit_op_sched->get_relops());
   		ptsmop !=0; ++ptsmop) {
      for (Op_inedges pted (*ptsmop); pted != 0; pted++)
	cutoff_dist = ELCOR_MAX(cutoff_dist, (*pted)->latency());
    }
    min_dist_from_exit (region_sched, exit_op_sched, cutoff_dist, dist_map);
    propagate_to_exit_lat_maps(region_sched, dist_map, 
  		import_def_map, import_use_map);
    if (reorder_ops_around_branches(region_sched, exit_op_sched,
			import_def_map, &early_map, swdeflist)) {
      set_switch_list(region_sched, exit_op_sched, swdeflist);
      swdeflist.clear();
      redraw_edges = true;
    }
    dist_map.clear();
  }
  if (redraw_edges) {
    delete_region_edges(region_sched);
    insert_region_meld_edges(region_sched);
    Region_entry_ops ei (region_sched);
    Op* entry_op = (*ei);
    Latency_map* entry_import_def_map = msc->import_def_at_entry(entry_op);
    Latency_map* entry_import_use_map = msc->import_use_at_entry(entry_op);
    set_entry_edge_latencies(region_sched, entry_op,
		 entry_import_def_map, entry_import_use_map);
    entry_import_def_map->clear();
    entry_import_use_map->clear();
  }
//  insert_pass_through_edges(region_sched);
  for (Region_exit_ops exi(region_sched); exi !=0; exi++) {
    Op* exit_op_sched = (*exi);
    import_def_map = msc->import_def_at_exit(exit_op_sched);
    import_use_map = msc->import_use_at_exit(exit_op_sched);
    set_exit_edge_latencies (region_sched, exit_op_sched,
			import_def_map, import_use_map);
    if (dbg (ss, 5)) {
      check_exit_edge_lats(region_sched, exit_op_sched,
			   import_def_map,  import_use_map);
    }
    //    import_def_map->clear();
    //    import_use_map->clear();
  }
}

void meld_set(Compound_region* blk) {

  if (region_has_rts (blk)) {
    meld_pad_rts (blk);
  }
  set_export_latency_maps (blk);
  if (dbg(ss, 4)) {
    MS_constraints *msc = get_ms_constraints(blk);
    cdbg << "Meld set: MS_constraints for region " << blk->id() << ":  " << *msc << endl;
  }
  if (dbg (ss, 5)) {
    assert(check_set_lat_maps(blk));
  }
  delete_region_edges(blk);
}

// Added by SAM, 5-97 
// After scheduling, the internal control_merges may not be in the
// correct position, ie directly after exit branches.  Also the freq
// attributes on entering the control_merges may not be correct.  This
// routine fixes both problems.
void fix_internal_control_merges(Compound_region *blk)
{
    Dlist<Op*> merge_list;
    Dlist<Op*> branch_list;
    Dlist<double> weight_list;
    Op *op, *merge_op, *branch_op, *next_op;
    int i;
    double w;
    Control_flow_freq *cfreq;
    Edge *edge;

    if (! blk->is_hb())
        return;

    i = -1;
    for (Region_ops_C0_order op_i(blk); op_i!=0; op_i++) {
      op = *op_i;
      i++;
      if (i == 0)	// Ignore the first control_merge, it is ok
	continue;
      if (is_branch(op)) {
	branch_list.push_tail(op);
	w = 0.0;
	for (Op_outedges edge_i(op, CONTROL0_OUTEDGES); edge_i!=0; edge_i++) {
	    edge = *edge_i;
	    if (is_fall_through(edge))
		continue;
	    cfreq = get_control_flow_freq(edge);
	    if (cfreq == NULL)
		El_punt("fix_internal_control_merges: missing taken edge freq: %d",
				op->id());
	    w += cfreq->freq;
        }
	weight_list.push_tail(w);
      }
      if (is_control_merge(op))
	merge_list.push_tail(op);
    }

    (void) branch_list.pop_tail();
    (void) weight_list.pop_tail();
    assert(branch_list.size() == merge_list.size());
    assert(branch_list.size() == weight_list.size());

    for (Dlist_iterator<Op*> ml_i(merge_list); ml_i!=0; ml_i++) {
	merge_op = *ml_i;
	El_remove_op(merge_op);
    }

    w = blk->weight;
    for (Dlist_iterator<Op*> bl_i(branch_list); bl_i!=0; bl_i++) {
	branch_op = *bl_i;
	next_op = branch_op->next();
        merge_op = merge_list.pop();
	w -= weight_list.pop();
	if (w < 0.0)
	    w = 0.0;
	El_insert_op_after(merge_op, branch_op);
	merge_op->set_sched_time(branch_op->sched_time());
	edge = get_incoming_CONTROL0_edge(merge_op);
	cfreq = get_control_flow_freq(edge);
	if (cfreq == NULL)
	    El_punt("fix_internal_control_merges: missing incoming edge freq: %d",
			merge_op->id());
	cfreq->freq = w;
    }
}


//-----------------------------------------------------------------------------
// The main driver for hyperblock scheduling
//----------------------------------------------------------------------------

void scalar_sched_prolog(Procedure* f) {
  if (El_do_run_time)
    El_clock1.start();

  opInfoMap = new Hash_map<Op*, OpInfo*> (hash_op_ptr, 211);
  if (SUPERSCALAR_MELD || MELD_COMPARE) {
    cfEdgeIsPredicted = new Hash_map <Edge*, bool> (hash_edge_ptr, 211);
    init_cfEdgeIsPredicted ((Compound_region*) f);
  }
  if (El_cache_load_sched == ANALYZE) {
      efilt_arr = init_load_par();
  }

  if (El_do_run_time) {
    El_clock1.stop();
    El_stopwatch_print_time("scalar_sched_prolog", El_clock1.last_elapsed());
  }
}


void scalar_scheduler_driver(Compound_region* region, El_Scalar_Sched_Phase phase)
{
  if (dbg(ss, 10)) el_dot_display_dfg(region);

  scalar_sched(region, phase);
  fix_internal_control_merges(region);
}

void scalar_sched(Compound_region* blk, El_Scalar_Sched_Phase phase) {

  Schedule* schedule;
  Priority* priority;

    if (dbg(status, 1)) {
      int id = blk->id();
      if (phase == EL_SCALAR_SCHED_PREPASS)
        cdbg << "(Pre_ss-" << El_ssched_meld_model << ")";
      else
        cdbg << "(Post_ss-" << El_ssched_meld_model << ")";
      cdbg << (blk->is_bb() ? "bb: " : "hb: ") << id << endl;
    }

    if (El_cache_load_sched == ANALYZE) {
      insert_region_scalar_edges (blk);
      calculate_earliest_and_latest_times (blk, CONTROL_SPEC_ALWAYS,
	NON_PSEUDO_OPS, NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES);
      determine_loadpar (blk, efilt_arr);

      for (Hash_map_iterator<Op*, OpInfo*> iter(*opInfoMap);
  		iter != 0; iter++) {
	OpInfo* info = (*iter).second;
	delete info;
      }
      opInfoMap->clear();
      delete_region_edges (blk);
      return;
    }

    if (El_ssched_meld_model != NO_MELD) { // MELD
      MS_constraints* msc = new MS_constraints(blk);
      set_ms_constraints(blk, msc);
      Hash_map<Op*, int> early_map (hash_op_ptr, 211);
      meld_entry(blk, early_map);
      meld_exits (blk, early_map);

      if (dbg(ss, 5)) {
	IR_outstream foo("cout");
	ir_write(foo, blk);
      }

      if (El_ssched_display_before_sched)
	        display_sched(blk);
      
      if (El_ssched_sched_model == CYCLE_SCHED){
	priority = new Listt(blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_list_sched(blk), 
	  new UnSched_heur_cycle(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      } else if (El_ssched_sched_model == LIST_SCHED) {
	priority = new Dep_pri(blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_list_sched(blk), 
	  new UnSched_heur_cycle(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      } else if (El_ssched_sched_model == LIST_BT_SCHED) {
	priority = new Dep_pri_bt (blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_seq(blk), 
	  new UnSched_heur_list_bt(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      } else {
	assert (El_ssched_sched_model == OPER_SCHED);
	priority = new Pri_bt(blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_seq(blk), 
	  new UnSched_heur_oper(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      }
	
      calculate_earliest_and_latest_times(blk, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES);
      priority->initialize();

      if (El_cache_load_sched == USE_STALL)
        presched_cacheopt(blk);

      schedule->derive_schedules(blk);

      if (El_cache_load_sched == USE_STALL)
        postsched_cacheopt(blk);

      blk->set_flag(EL_REGION_SCHEDULED);
      delete schedule;
	
      if (dbg(ss, 10)) {
	cdbg << "Superblock after scheduling:" << endl;
	print_sched(blk);
      }
      meld_set(blk);
    }
    else { 	// NO_MELD

      if (El_do_run_time) {
          El_clock2.start();
      }
      insert_region_scalar_edges (blk, phase);

      //MP: remove C1 edges that don't begin/end at the hb start/stop nodes
      if(El_enable_branch_reorder)
	{
	  el_add_C1_edges_to_stop_op(blk);
	  el_remove_C1_edges(blk);
	}

      if (El_do_run_time) {
          El_clock2.stop();
	  if (El_do_block_run_time)
              El_stopwatch_print_region_time(blk, "edge_drawing",
					El_clock2.last_elapsed());
          El_clock3.start();
      }

      if (dbg(ss, 7)) {
	IR_outstream schedbug ("cout");
	ir_write(schedbug, blk);
      }
      if (El_ssched_display_before_sched)	display_sched(blk);
      
      if (El_ssched_sched_model == CYCLE_SCHED){
	priority = new Listt(blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_list_sched(blk), 
	  new UnSched_heur_cycle(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      } else if (El_ssched_sched_model == LIST_SCHED) {
	priority = new Dep_pri(blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_list_sched(blk), 
	  new UnSched_heur_cycle(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      } else if (El_ssched_sched_model == LIST_BT_SCHED) {
	priority = new Dep_pri_bt (blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_seq(blk), 
	  new UnSched_heur_list_bt(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      } else {
	assert (El_ssched_sched_model == OPER_SCHED);
	priority = new Pri_bt(blk, CONTROL_SPEC_ALWAYS, NON_PSEUDO_OPS,
	  	NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES, false);
	schedule = new Schedule(priority, new Cycle_seq(blk), 
	  new UnSched_heur_oper(blk), phase, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES,
	  NON_CONTROL0_OUTEDGES, false);
      }
	
      calculate_earliest_and_latest_times(blk, CONTROL_SPEC_ALWAYS,
	  NON_PSEUDO_OPS, NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES);
      priority->initialize();

      if (El_cache_load_sched == USE_STALL)
        presched_cacheopt(blk);

      schedule->derive_schedules(blk);

      if (El_cache_load_sched == USE_STALL)
        postsched_cacheopt(blk);

      blk->set_flag(EL_REGION_SCHEDULED);
      if (dbg(ss, 7)) {
	IR_outstream schedbug ("cout");
        ir_write(schedbug, blk);
      }

      if (MELD_COMPARE) {
	// for comparison with meld
	MS_constraints* msc = new MS_constraints(blk);
	set_ms_constraints(blk, msc);
	set_export_latency_maps (blk);
      }
      if (dbg(ss, 7)) {
	print_sched(blk);
      }
      delete schedule;

      if (El_do_run_time) {
          El_clock3.stop();
	  if (El_do_block_run_time)
              El_stopwatch_print_region_time(blk, "scalar_scheduling",
                                        El_clock3.last_elapsed());
          El_clock4.start();
      }
      
      delete_region_edges(blk);

      if (El_do_run_time) {
          El_clock4.stop();
	  if (El_do_block_run_time)
              El_stopwatch_print_region_time(blk, "delete_edges",
                                        El_clock4.last_elapsed());
      }
    }
}

void scalar_sched_epilog(Procedure* f) {

  if (El_do_run_time)
    El_clock1.start();

  // for comparing NO_MELD with MELD
  // if (El_ssched_meld_model == NO_MELD) pad_meld_dangle (f);

  if (El_ssched_noop_pads_for_calls) {
    pad_calls_and_rts (f);
  }
  if ((El_ssched_meld_model != NO_MELD) && (dbg(ss, 5))) {
    assert(check_lat_map_propagation(f));
  }

  delete opInfoMap;
  if (SUPERSCALAR_MELD || MELD_COMPARE)	delete cfEdgeIsPredicted;

  // may also need to delete the individual filters in the Vector efilt_arr
  if (El_cache_load_sched == ANALYZE)	delete efilt_arr;

  f->set_flag (EL_PROC_SCHEDULED);
  if (mask_pe_set)
    f->set_flag (EL_PROC_MASK_PE);

  if (El_do_run_time) {
    El_clock1.stop();
    El_stopwatch_print_time("scalar_sched_epilog", El_clock1.last_elapsed());
  }
}
