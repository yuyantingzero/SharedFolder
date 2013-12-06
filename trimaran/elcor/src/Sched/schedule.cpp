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
//      File:           schedule.cpp
//      Authors:        Santosh G. Abraham, Vinod Kathail,
//			Brian Dietrich, Srini Mantripragada
//      Created:        July 1997
//      Description:    Main scalar scheduler class routines
//
/////////////////////////////////////////////////////////////////////////////

#include "el_error.h"
#include "schedule.h"
#include "intf.h"
#include "op.h"
#include "iterators.h"
#include "connect.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "attributes.h"
#include "hash_functions.h"
#include "bit_vector.h"
#include "el_ssched_init.h"
#include "dbg.h"
#include "ir_writer.h"
#include "mdes.h"
#include "el_driver_init.h"
#include "el_cache_init.h"
#include "load_slack.h"
#include "load_info.h"

extern "C" {
#include <limits.h>
};

bool mask_pe_set; // used to know to set function's flag, EL_PROC_MASK_PE.

void display_sched (Compound_region *r);

Hash_map<Op*,int > *save_etimes() {
  Hash_map<Op*,int > *etime_map;

  etime_map = new Hash_map<Op*,int > (hash_op_ptr, 211);
  for (Hash_map_iterator<Op*, OpInfo*> miter(*opInfoMap); miter != 0; miter++) {
    Op* op = (*miter).first;
    etime_map->bind(op,(*miter).second->get_etime());
  }
  return etime_map;
}

Schedule::Schedule(Priority* prio, Cycle* cycl, UnSched_heur* unsch,
		const El_Scalar_Sched_Phase phase,
		const Relax_dep_constr* relax_dep,
		const Op_filter* ofilter,
		const Edge_filter* efilter, const Edge_filter* eoutfilter,
		bool delete_filter_flag)
   :	priority_rep(prio), cycl_rep(cycl), unsch_rep(unsch),
	phase_rep(phase),
	relax_rep(relax_dep),
	ofilter_rep(ofilter), efilter_rep(efilter), eoutfilter_rep(eoutfilter),
	delete_filter (delete_filter_flag), sched_list_vec(0)
{
 br_lat = MDES_branch_latency();

 move_branches = (br_lat > 1) 
    && ( ( phase_rep == EL_SCALAR_SCHED_PREPASS
	   && El_ssched_prepass_move_branches_to_branch_point )
	 || ( phase_rep == EL_SCALAR_SCHED_POSTPASS
	      && El_ssched_postpass_move_branches_to_branch_point ) );

}

Schedule::~Schedule() { 
  delete priority_rep;
  delete cycl_rep;
  delete unsch_rep; 
  if(delete_filter)
  {
     delete (Relax_dep_constr*) relax_rep;
     delete (Op_filter*) ofilter_rep;
     delete (Edge_filter*) efilter_rep;
     delete (Edge_filter*) eoutfilter_rep;
  }
  // delete etime_map if USE_STALL in load scheduler
}

static bool ok_to_schedule(Op *op, char *sched_op, int stime, List<Op *> d_merge_list)
{
  return true;
}

// moved this function to Mdes/intf.cpp -KF 12/2004
#if 0
void get_sched_opcode_from_res_idx(Op *op, char *iopat, int res_idx, char **outchar)
{
  Io_list* iol_elem;
  Io_descr* iod_elem;
  Alt_descr* altd_elem;
  Io_descr request_iod(current_MDES);

  char *opcode = get_mdes_opcode_string(op);
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);

  operation->io_list_init();
  iol_elem = operation->get_next_io();
  iod_elem = iol_elem->get_iod();
  
  request_iod.parse_iospec(iopat);

  while(iol_elem) {
    if(iod_elem->test_io(&request_iod)) {
      while((altd_elem = iol_elem->get_next_alt()) != NULL){
        Op_descr* alt_op = altd_elem->get_op();
        char* aname = alt_op->get_aname_ptr();
        *outchar = aname;
        Res_use_descr* res_use = altd_elem->get_res_use();
        if(res_use->get_res() == res_idx)
          return;
      }
    }
    iol_elem = operation->get_next_io();
    if (iol_elem) iod_elem = iol_elem->get_iod();
  }
  cdbg << "get_sched_opcode : " << get_mdes_opcode_string(op) << " : " << res_idx << endl;
  assert(0);
}
#endif

/* Derives schedules for all ops present in the regions.
   The priority function is used to walk through the list of ops
   based on the static priority of the op and dynamic information,
   such as whether the op's predecessors have been scheduled.
   Depending on the setting of El_ssched_sched_model, one of
   four different scheduling strategies (models) is used:
   0. Cycle scheduling (CYCLE_SCHED): 
      operations are scheduled cycle by cycle
      with operations in a particular cycle being scheduled
      before operations in the next cycle
      (except when there are negative incoming edge lats to an op,
      when the op may be scheduled at an earlier cycle than its scheduled
      predecessors) 
      (Simplest scheduler and only scheduler tested with meld
      and load scheduler)
   1. List scheduling (LIST_SCHED):
      operations become ready when their
      predecessors are scheduled and ready operations are scheduled in
      static priority order. An operation may be scheduled in a cycle
      before/after the previous operation's cycle. No backtracking;
      once an operation is scheduled, it is never rescheduled.
      (Least compile time)
   2. List scheduling with backtracking (LIST_BT_SCHED):
      Similar to LIST_SCHED but ops with negative incoming edge latencies
      (typically branches) are initially marked as forcibly schedulable
      and can forcibly unschedule other already-scheduled operations.
      Forcibly unscheduled operations are also marked forcibly schedulable.
      Operations marked forcibly schedulable are scheduled as below in
      OPER_SCHED; other operations are scheduled as in LIST_SCHED
   3. Operation scheduling (OPER_SCHED)  
      Operations are scheduled strictly ordered by static priority.
      If an operation cannot be placed in the range [etime, ltime],
      other operations are unscheduled to place it at an
      attempted time which is one more than its previous attempted time
      or the etime. Unscheduled operations are put back to be scheduled
      in priority order. (Handles compound branch ops with negative
      latencies).
*/

void Schedule::derive_schedules(Compound_region* region) 
{
 int dag=0;  /* scalar scheduling */
 int* time;
 int etime, ltime, force_time;
 int priority;
 char iopat[MAXIOPATLEN];
 char *outchar;
 Hash_set <void*> unsched_set(hash_void_ptr, 16);
 char* name;
 OpInfo* info;
 bool success;

 Region_entry_ops entry(region);
 Op* entry_op = (*entry);
 info = opInfoMap->value(entry_op);
 num_exits = info->get_num_ltimes();

 seq_length = get_seq_exec_time(region); 
 RU_alloc_map(seq_length);
 RU_init_map(dag, seq_length);

 int stime = 0;
 max_sched_cycle = 0;
 sched_list_vec.resize(seq_length);


 // Use CYCLE scheduler with load scheduling
 if (El_cache_load_sched_type || El_cache_load_sched) {
     assert (El_ssched_sched_model == CYCLE_SCHED);
 }

 // TLJ - save the etimes for all load opers (they will change
 //	during scheduling)
 if (El_cache_load_sched == USE_STALL) {
   // recompute priorities (new ops/latencies)
   delete priority_rep;
   priority_rep = new Listt(region,relax_rep,ofilter_rep,efilter_rep,
			eoutfilter_rep,delete_filter);
   etime_map = save_etimes();
 }

 if (El_ssched_sched_model == LIST_SCHED) {
   // optimized and customized scheduling loop for LIST_SCHED
   // assuming  Priority is List_opt (Listt may also work)
   //		Unsched  is set to Unsched_heur_cycle but not used, 
   //		Cycle    is set to Cycle_list_sched but not used
   // iterates over ops in order presented by Priority
   //  iterates over cycles starting from etime
   //    schedule op in this cycle and break out of inner loop

   // KVM : Live-ins from the scheduled predecessors of this region

   List<Op *> d_merge_list;
   d_merge_list.clear();
   
   for(Region_entry_edges in(region, CONTROL0_INEDGES); in != 0; in++) {
     Edge* edge = *in;
     Op* in_op = edge->dest();

     List<Op*> relops = in_op->get_relops();

     for(List_iterator<Op*> li(relops); li!=0; li++) {
       d_merge_list.add_tail(*li);
     } 
       
   }

   for(Op* curr_op = *(*priority_rep); curr_op != 0; ++(*priority_rep),
	 curr_op = *(*priority_rep)) {
      //cerr << "Scheduling Op: " << curr_op->id() << "\t"
      // << curr_op->opcode() << endl;

     curr_op->build_io_pat(iopat);
     // name = el_opcode_to_string_map.value(curr_op->opcode());
     name = get_mdes_opcode_string(curr_op);
     info = opInfoMap->value(curr_op);

     stime = info->get_etime();

     // scheduling at etime must check for any exec_priority constraints
     int exec_priority = info->get_exec_priority();
     
     if (exec_priority == -1) {
       bool sched_success = false;
       while(!sched_success) {
         // simpler RU iteration when no exec_priority constraints
         RU_init_iterator(name, curr_op, iopat, stime);
         // KVM : check all the free resources for the bypass
         //       criterion (ok_to_schedule)
         while (RU_get_next_nonconfl_alt(&outchar, &priority)) {
           if(ok_to_schedule(curr_op, outchar, stime, d_merge_list)) {
             sched_success = true;
             break;
           }
         }
         if(!sched_success) {
           stime++;
         }
         assert (stime < MAX_SCHED_LEN);
       }
     } else {
    		
       bool success = false;

       RU_init_iterator(name, curr_op, iopat, stime);
       // schedule at etime honoring exec_priority constraints ...
       while (RU_get_next_nonconfl_alt(&outchar, &priority)) {
         if ((priority >= exec_priority) && (ok_to_schedule(curr_op, outchar, stime, d_merge_list))) {
           success = true;
           break;
         }
       }

       // or after etime without any exec_priority constraints
       if (!success) {
   stime++;

         bool sched_success = false;
         while(!sched_success) {
           // simpler RU iteration when no exec_priority constraints
           RU_init_iterator(name, curr_op, iopat, stime);
           // KVM : check all the free resources for the bypass
           //       criterion (ok_to_schedule)
           while (RU_get_next_nonconfl_alt(&outchar, &priority)) {
             if(ok_to_schedule(curr_op, outchar, stime, d_merge_list)) {
               sched_success = true;
               break;
             }
           }
         if(!sched_success)
         stime++;
         assert (stime < MAX_SCHED_LEN);
       }
#if 0
   RU_init_iterator(name, curr_op, iopat, stime);
   while (!RU_get_next_nonconfl_alt(&outchar, &priority)) {
     stime++ ;
     assert (stime < MAX_SCHED_LEN);
     RU_init_iterator(name, curr_op, iopat, stime);
   }
#endif
       }
     }
     if (dbg(ss, 6)) {
       cerr << "Scheduling Op: " << curr_op->id() << " "
      << curr_op->opcode() << " at " << stime << endl;
     }

     curr_op->set_sched_opcode(outchar);
     if (El_ssched_priority_model != FREEZE_PRIORITIZED) 
       info->set_exec_priority(priority);
     schedule_op (region, curr_op, info, stime);
     sched_list_vec[stime].add_tail(curr_op);

     if (stime > max_sched_cycle) {
       max_sched_cycle = stime;
       if (El_ssched_display_every_cycle) display_sched(region);
     }
   }

   //cdbg << "List scheduling ends" << endl;

 } else {

   // GENERAL SCHEDULING LOOP
   // currently used by CYCLE_SCHED, LIST_BT_SCHED and OPER_SCHED
   // max exec_priority op can be scheduled at ltime
   int ltime_exec_priority;
   for(Op* curr_op = *(*priority_rep); curr_op != 0; ++(*priority_rep),
       curr_op = *(*priority_rep)) {
     curr_op->build_io_pat(iopat);
     // name = el_opcode_to_string_map.value(curr_op->opcode());
     name = get_mdes_opcode_string(curr_op);
     info = opInfoMap->value(curr_op);
     etime = ELCOR_MAX (find_earliest_sched_pos(curr_op, region), info->get_etime());
     ltime = find_latest_sched_pos(curr_op, region, ltime_exec_priority);

     // To handle operation prioritization and 0-cycle edges more efficiently,
     // when using a scheduler that does not schedule in dependence order,
     // we need following enhancements not yet implemented,
     // because Scheduler-Mdes interface needs to be first exapnded.
     // Determine set of ops scheduled at etime that curr_op is dependent upon
     // and set of ops scheduled at ltime that depend upon curr_op
     // Set up the RU_init_iterator with these two sets
     // Also, unschedule must unschedule (some of the) ops
     // that are connected by zero-cycle edges
     // and forcibly scheduling in unscheduled cycle must permit
     // specification of scheduled ops of higher/lower priority

     success = false;
     (*cycl_rep).initialize(etime, ltime, curr_op, 
			   (*priority_rep).get_relax_constr_function());
     (*unsch_rep).initialize(etime, ltime, curr_op);

     time = *(*cycl_rep);
     int etime_exec_priority = info->get_exec_priority();
       
     while (success == false && time != NULL)
     {
       stime = *time;
       RU_init_iterator(name, curr_op, iopat, stime);
       while (RU_get_next_nonconfl_alt(&outchar, &priority)) {
	 if ( (stime > etime) && (stime < ltime) ) {
	   success = true;
	   break;
	 } else if (((stime == etime) && (priority >= etime_exec_priority)) ||
		    ((stime == ltime) && (priority <= ltime_exec_priority)) ) {
	     success = true;
	     break;
	 }
       }
       if ((success == false) &&
	   ((*unsch_rep).unschedule(force_time)) &&
	   (force_time <= stime ) ) {
	   unsched_set.clear();
	   get_all_conflicting_ops(name, curr_op, stime, unsched_set);
	   if (higher_priority_curr_op (curr_op, unsched_set)) {
	     force_schedule (curr_op, region, stime, unsched_set, outchar);
	     success = true;
	     break;
	   }
       }
       ++(*cycl_rep);
       time = *(*cycl_rep);
     }
     if(!success) /* coudn't find a slot between "etime" and "ltime" */
     {   
       if ((*unsch_rep).unschedule(force_time))
       {
	 unsched_set.clear();
	 get_all_conflicting_ops(name, curr_op, force_time, unsched_set);
	 force_schedule (curr_op, region, force_time, unsched_set, outchar);
	 success = true;
	 stime = force_time;
       }
       else /* reschedule the op */
       {
	   (*priority_rep).reschedule(curr_op, region);
	   info->incr_unsch_time();
	   continue;
       } 
     }
     if(success)  /* scheduled */ {
       if (dbg(ss, 6)) {
         cerr << "Scheduling Op: " << curr_op->id() << " " << curr_op->opcode() << " at " << stime << endl;
       }
       curr_op->set_sched_opcode(outchar);
       schedule_op (region, curr_op, info, stime);
       sched_list_vec[stime].add_tail(curr_op);
     }
     if (stime > max_sched_cycle) {
       max_sched_cycle = stime;
       if (El_ssched_display_every_cycle) display_sched(region);
     }
   } /* for */
 }

 // Reorders ops in a cycle so that regular ops are followed by branches, jsrs
 // and dummy branches in that order
 if(El_ssched_reorder_scheduled_ops)
   reorder_scheduled_ops (region);

// Insert a dummy branch, if schedule does not end with a dummy branch
 if (br_lat > 1)
   ensure_last_op_is_dummy (region);

 // Determine ops that have moved above branches and mark them speculative
 mark_speculative_ops (region);

 // Move branches down by br_lat-1 to branch point
 if (move_branches)
   move_branches_to_branch_point (region);

 // Sequential Control-0 threading of ops by schedule cycle
 // and intra-instruction prioriy
 //fout << "before thread_scheduled_region" << endl;
 //fout << *region << endl;
 thread_scheduled_region (region);

 // Ensure that schedule satisfies dependence constraints
 if (dbg(ss, 6)) check_schedule (region);

 if (El_ssched_display_after_sched)
    display_sched(region);

 RU_delete_map();
}

bool Schedule::higher_priority_curr_op(Op* curr_op,
				       Hash_set<void*>& sched_set) {
  OpInfo* curr_info = opInfoMap->value(curr_op);
  double currop_pr =  curr_info->get_priority();
  for(Hash_set_iterator<void*> schedi(sched_set); schedi != 0; schedi++) {
    Op* sched_op = (Op*) (*schedi);
    OpInfo* sched_info = opInfoMap->value(sched_op);
    double sched_pr = sched_info->get_priority();
    if (currop_pr < sched_pr)      return true;
  }
  return false;
}

void Schedule::force_schedule (Op* curr_op, Compound_region* region,
			       int force_time,
			       Hash_set<void*>& unsched_set, char* outchar)
{
  int priority;
  char iopat[MAXIOPATLEN];

  if (dbg(ss, 6)) {
    cerr << "UnScheduling at time: " << force_time << endl;
    cerr << '\t';
    if (unsched_set.is_empty() ) cerr << "Unsched_set is empty";
  }

  // First, make sure unsched_set ops have their scheduled flag set to false
  for(Hash_set_iterator<void*> unsch(unsched_set);
      unsch != 0; ++unsch) {
    Op* unsch_op = (Op*) (*unsch);
    OpInfo* unsch_info = opInfoMap->value(unsch_op);
    int unsch_op_sched_time = unsch_op->sched_time();
    sched_list_vec[unsch_op_sched_time].remove(unsch_op);
    unsch_info->set_scheduled(false);
    unschedule_op (unsch_op, unsch_info);
    if (dbg(ss, 6)) {
      cerr << " [" << unsch_op->id() << " " << unsch_op->opcode() << "]";
    }
    unsch_info->incr_unsch_time();
  }
  if (dbg(ss, 6)) {
    cerr << endl;
  }

  // Then, reschedule which checks for scheduled flag of predecessors
  // Split these loops to handle zero-cycle edges and dependent ops scheduled
  // in same cycle and unscheduled together ??
  for(Hash_set_iterator<void*> unsched(unsched_set); unsched != 0; ++unsched) {
    Op* unsch_op = (Op*) (*unsched);
    (*priority_rep).reschedule(unsch_op, region);
  }

  OpInfo* info = opInfoMap->value(curr_op);
  int prev_attempted_time = -1;
  if (info->is_attempted()) prev_attempted_time = info->get_attempted_time();
  assert (prev_attempted_time < force_time);
  info->set_attempted_time (force_time);
    
  curr_op->build_io_pat(iopat);
  // char* name =  el_opcode_to_string_map.value(curr_op->opcode());
  char* name =  get_mdes_opcode_string(curr_op);
  RU_init_iterator(name, curr_op, iopat, force_time);
  if (dbg(ss, 6)) {
    cerr << "Forcibly " ;
  }  
  assert (RU_get_next_nonconfl_alt(&outchar, &priority));
}

/* Find the earliest cycle at  which "op" can be scheduled taking into
   account dependence constraints from already scheduled predecessors.
*/

int Schedule::find_earliest_sched_pos(Op* op, Compound_region* region)
{
  int displ = 0, pos;
  OpInfo* dest_info = opInfoMap->value(op);
  int dest_exec_priority = dest_info->get_exec_priority();
  
  // earliest time based on data dependent predecessors
  // op's exec_priority set to be one more than max of 
  // exec_priorities of constraining scheduled src ops of latency 0
  for(Op_inedges_rel ei(op, efilter_rep); ei != 0; ++ei)
  {
    Op* source = real_op((*ei)->src());
    OpInfo* src_info = opInfoMap->value(source);
    int lat = (*ei)->latency();
    if (src_info->is_scheduled()) {
      if ((pos = lat + source->sched_time()) > displ) {
	displ = pos;
	dest_exec_priority = -1;
      }
      // if scheduled op has latency 0 and constrains op
      // then set dest_exec_priority to be max (current, src_exec_priority)
      if ((lat == 0) && (pos == displ)) {
	int src_exec_priority = src_info->get_exec_priority();
	if (src_exec_priority > dest_exec_priority)
	  dest_exec_priority = src_exec_priority;
      }
    } else if (src_info->is_attempted()) {
      if ((pos = lat + src_info->get_attempted_time() + 1) > displ)
	displ = pos;
	dest_exec_priority = -1;
    }
  }
  if (El_ssched_priority_model != FREEZE_PRIORITIZED)
    dest_info->set_exec_priority (dest_exec_priority);
  return(displ);
}

/* Find the latest cycle time that this op can be scheduled 
   under constraints placed by currently scheduled ops
*/

int Schedule::find_latest_sched_pos(Op* op, Compound_region* region,
				    int& exec_priority)
{
  int lat, pos, displ = seq_length;
  exec_priority =  MAX_ISSUE_WIDTH;
  for(Op_outedges_rel eo(op, eoutfilter_rep); eo != 0; ++eo)
  {
    Op* dest = real_op((*eo)->dest());
    OpInfo* dest_info = opInfoMap->value(dest);
    if (dest_info->is_scheduled()) {
      lat = (*eo)->latency();
      pos = dest->sched_time() - lat;
      if (pos < displ) {
	displ = pos;
	exec_priority = MAX_ISSUE_WIDTH;
      }
      if ((lat == 0) && (pos == displ)) {
	int dest_exec_priority = dest_info->get_exec_priority();
	if (exec_priority > dest_exec_priority)
	  exec_priority = dest_exec_priority;
      }
    }
  }
  return(displ);
}

int Schedule::get_seq_exec_time(Compound_region* region)
{
  int no_ops = 0, time = 0;

  for(Region_ops_C0_order iter(region); iter != 0; ++iter)
     no_ops++;
  time = no_ops * MAX_CYCLES;
 return time;
}

void Schedule::get_all_conflicting_ops(char* opcode, Op* op, 
				  int time, Hash_set<void*>& confl_ops)
{
  char iopat[MAXIOPATLEN];

  op->build_io_pat(iopat);
  RU_init_iterator(opcode, op, iopat, time);
  RU_get_conflicting_ops(confl_ops);
  for(Op_inedges_rel in_iter(op, efilter_rep); in_iter != 0; ++in_iter)
  {
    Op* src = real_op((*in_iter)->src());
    OpInfo* in_info = opInfoMap->value(src);
    if(in_info->is_scheduled()) {
      if(src->sched_time() + (*in_iter)->latency()  > time)
	confl_ops += (void*) src;
    }
  }
  for(Op_outedges_rel out_iter(op, eoutfilter_rep); out_iter != 0; ++out_iter)
  {
    Op* dest = real_op((*out_iter)->dest());
    OpInfo* out_info = opInfoMap->value(dest);
    if(out_info->is_scheduled()) {
      if(dest->sched_time() - (*out_iter)->latency() < time)
	confl_ops += (void*) dest;
    }
  }
}

void Schedule::schedule_op (Compound_region* region, Op* curr_op,
			    OpInfo* info, int stime)
{
  int diff = -1;
  char* name;
  char iopat[MAXIOPATLEN];
  char* outchar ;
  int priority;

  curr_op->set_sched_time(stime);
  curr_op->set_flag(EL_REGION_SCHEDULED);
  if (ofilter_rep->filter(curr_op)) RU_place();
  info->set_scheduled(true); /* scheduled */

 // TLJ
  if (El_cache_load_sched == USE_STALL && is_load(curr_op)) {
    if (loadInfoMap->is_bound(curr_op)) {
      diff = recompute_earliest_times(region,etime_map);
      recompute_latest_times(region);
      LoadInfo *ldinfo = loadInfoMap->value(curr_op);
      diff = info->get_ltime() - ldinfo->get_ltime();
      // fill in updated_ltime field of ldinfo
      ldinfo->set_updated_ltime(info->get_ltime());
    }
    else
      diff = 0;
  }

  if (is_control_switch(curr_op) || is_control_merge(curr_op)) {
    for (List_iterator<Op*> op_iter(curr_op->get_relops());
	 op_iter != 0 ; op_iter++) {
      Op* sm_op = (*op_iter);
      if (dbg (ss, 6)) assert (is_switch(sm_op) || is_merge(sm_op));
      sm_op->build_io_pat(iopat);
      // name = el_opcode_to_string_map.value(sm_op->opcode());
      name = get_mdes_opcode_string(sm_op);
      RU_init_iterator(name, sm_op, iopat, stime);
      assert(RU_get_next_nonconfl_alt(&outchar, &priority));
      RU_place();
      
      sm_op->set_sched_time(stime);
      // Changed sched time of switches to be that of branch - 3/96
      /*
	if (is_branch(curr_op))
	sm_op->set_sched_time(stime + br_lat); 
	if (is_dummy_branch(curr_op)) sm_op->set_sched_time(stime + 1);
	*/
      sm_op->set_sched_opcode(outchar);
      sm_op->set_flag(EL_REGION_SCHEDULED);
    }
  }
  
  // TLJ - 5/29/96
  if (El_cache_load_sched == USE_STALL && is_load(curr_op)) {
    adjust_load_use_latency(region,etime_map,curr_op,diff,stime);
  }
}

// Unschedule op and all its related ops
// (undo what schedule_op does)
void Schedule::unschedule_op (Op* op, OpInfo* info) {
  char iopat[MAXIOPATLEN];
  op->build_io_pat(iopat); /* unschedule */
  if (ofilter_rep->filter(op))	// is this guard necessary ??
    RU_remove((void*) op, iopat, op->sched_time());
  op->set_sched_opcode (NULL);
  op->reset_flag(EL_REGION_SCHEDULED);
  // info->reset_etime Keep etime as it is
  info->set_scheduled(false);
  info->set_exec_priority(-1);

  if (is_control_switch(op) || is_control_merge(op)) {
    for (List_iterator<Op*> op_iter(op->get_relops());
	 op_iter != 0 ; op_iter++) {
      Op* sm_op = (*op_iter);
      sm_op->build_io_pat(iopat);
      RU_remove((void*) sm_op, iopat, sm_op->sched_time());
      sm_op->set_sched_opcode (NULL);
      sm_op->set_sched_time(-1);
      sm_op->reset_flag(EL_REGION_SCHEDULED);
    }
  }
}

void Schedule::change_sched_time (Op* op, int new_stime) {

  op->set_sched_time(new_stime);
  if (is_control_switch(op) || is_control_merge(op)) {
    for (List_iterator<Op*> op_iter(op->get_relops());
	 op_iter != 0 ; op_iter++) {
      Op* sm_op = (*op_iter);
      sm_op->set_sched_time(new_stime);
    }
  }
}

// Reorder operations in each scheduled cycle so that
// any zero-cycle dependencies flow left to right
// This reordering required when operations are not scheduled
//  in dependence order, e.g. oper_bt
// Additionally, sort operations so that dummy branches are rightmost,
// jsr's are next rightmost and then other operations.
// Retain order within each group, e.g. within jsrs in same cycle

void Schedule::reorder_scheduled_ops (Compound_region* region) {

  // char iopat[MAXIOPATLEN];  

  // iterate over cycles of schedule
  for (int cycle = 0; cycle <= max_sched_cycle; cycle++) {
    bool reordered = false;	// has there been any reordering in this cycle
    // Only LIST_BT_SCHED and OPER_SCHED do not always schedule ops
    // in dependence order
    if (El_ssched_sched_model == LIST_BT_SCHED ||
	El_ssched_sched_model == OPER_SCHED) {
      bool zero_edges = false;
      int ops_in_cycle = 0;
      // Check for zero cycle edges between ops in same cycle
      for (List_iterator <Op*> li (sched_list_vec[cycle]); li != 0; li++) {
	Op* op = (*li);
	ops_in_cycle++;
	if (zero_edges) continue;
	for (Op_inedges_rel ei (op, efilter_rep); ei != 0; ei++) {
	  if ((*ei)->latency() == 0 && (*ei)->src()->sched_time() == cycle) {
	    zero_edges = true;
	    break;
	  }
	}
      }
      // More work may be required if zero-cycle edges
      if (zero_edges) {
	// Map scheduled ops in cycle to its position (priority)
	Hash_map <Op*, int> schedop2pos (hash_op_ptr, 7);
	Vector <Op*> pos2schedop (ops_in_cycle, (Op*)NULL);	// map from pos to op*
	int pos = 0;
	int iterations = 0;
	// repeatedly scan ops in original order
	while (pos < ops_in_cycle) {
	  iterations++;
	  // quit if too many scans needed; something wrong
	  if (iterations > 100) {
	    cerr << "TOO MANY ITERATIONS IN CB: " << region->id()
	      << " IN CYCLE: " << cycle << " WITH: " << ops_in_cycle << " OPS"
	      << endl;
	    cerr << "SCHED_LIST_VEC: " ;
	    for (List_iterator <Op*> li (sched_list_vec[cycle]);
		 li != 0; li++) { 
	      cerr << (*li)->id() << " " ;
	    }
	    cerr << endl;
	    cerr << "POS2SCHEDOP: " ;
	    for (int i = 0; i<pos; i++) {
	      cerr << pos2schedop[i]->id() << " " ;
	    }
	    cerr << endl;
	    assert(0);
	  }
	  // one scan of ops scheduled in cycle to find ops
	  // that don't have predecessors or
	  // whose predecessors have already been placed
	  for (List_iterator <Op*> li (sched_list_vec[cycle]); li != 0; li++) {
	    Op* op = (*li);
	    // if op has not yet been placed
	    if (!schedop2pos.is_bound(op)) {
	      bool place_op = true;
	      // check if all its predecessors along incoming zero-cycle
	      // edges are placed
	      for (Op_inedges_rel ei (op, efilter_rep); ei != 0; ei++) {
		Op* src = (*ei)->src();
		if ((*ei)->latency() == 0 && src->sched_time() == cycle
		    && (!schedop2pos.is_bound(src))) {
		  place_op = false;
		  break;
		}
	      }
	      // place and bind ops
	      if (place_op) {
		schedop2pos.bind(op, pos);
		pos2schedop[pos] = op;
		pos++;
	      }
	    }
	  }
	}
	// if all operations were placed in one cycle then the new ordering
	// is same as original ordering and reordering is not necessary
	if (iterations > 1) {
	  reordered = true;
	  sched_list_vec[cycle].clear();
	  for (int slot = 0; slot < ops_in_cycle; slot++) {
	    sched_list_vec[cycle].add_tail(pos2schedop[slot]);
	  }
	}
      }
    }
    // Required for all scheduling models
    // Move dummy branches and jsrs to the right
    List<Op*> jsr_ops, dbr_ops, br_ops;
    for (List_iterator <Op*> li (sched_list_vec[cycle]); li != 0; li++) {
      Op* sched_op = (*li);
      if (is_call	  (sched_op)) 	jsr_ops.add_tail(sched_op);
      if (is_dummy_branch (sched_op))	dbr_ops.add_tail(sched_op);
      if (is_proper_branch(sched_op))	 br_ops.add_tail(sched_op);
    }
    // Checking to make sure of assumption that no zero-cycle edges
    // that will be broken
    if (!br_ops.is_empty() || !jsr_ops.is_empty() || !dbr_ops.is_empty()) {
      for (List_iterator <Op*> lj (sched_list_vec[cycle]); lj != 0; lj++) {
	if (jsr_ops.is_member(*lj) || dbr_ops.is_member(*lj) || 
	    br_ops.is_member(*lj) )
	  for (Op_outedges_rel eo ((*lj), efilter_rep); eo != 0; eo++) {
	    if ((*eo)->latency() == 0 ) {
	      Op* src_op = (*lj);
	      Op* dest_op = (*eo)->dest();
	      if (dbr_ops.is_member (src_op)) {
		assert (0);
	      } else if (jsr_ops.is_member (src_op)) {
		assert (dbr_ops.is_member (dest_op));
	      } else {
		assert (br_ops.is_member (src_op)
			&& (jsr_ops.is_member (dest_op)
			    || dbr_ops.is_member(dest_op)) );
	      }
	    }
	  }
      }
    }

    // Remove any proper branches and place them at the right end
    if (!br_ops.is_empty()) {
      reordered = true;
      for (List_iterator <Op*> li (br_ops); li != 0; li++) {
	sched_list_vec[cycle].remove(*li);
	sched_list_vec[cycle].add_tail(*li);
      }
    }
    // Remove any jsrs and place them at the right end
    if (!jsr_ops.is_empty()) {
      reordered = true;
      for (List_iterator <Op*> li (jsr_ops); li != 0; li++) {
	sched_list_vec[cycle].remove(*li);
	sched_list_vec[cycle].add_tail(*li);
      }
    }
    // Remove any dummy branches and place them at the right end
    int dbr_ops_size = dbr_ops.size();
    if ( (dbr_ops_size >  1 ) ||
	 // Check for case of one dbr_op already at tail of sched_list
	 // (no reordering required for this common case)
	((dbr_ops_size == 1 ) &&
	 (sched_list_vec[cycle].tail() != dbr_ops.head()))) {
      reordered = true;
      for (List_iterator <Op*> li (dbr_ops); li != 0; li++) {
	sched_list_vec[cycle].remove(*li);
	sched_list_vec[cycle].add_tail(*li);
      }
    }
#if 0
    if (reordered) {
      // unplace all operations in cycle
      for (List_iterator <Op*> li (sched_list_vec[cycle]); li != 0; li++) {
	Op* op = (*li);
	OpInfo* info = opInfoMap->value(op);
	unschedule_op (op, info);
      }
      // place them back in again
      for (List_iterator <Op*> lj (sched_list_vec[cycle]); lj != 0; lj++) {
	char* outchar;
	int priority;
	Op* op = (*lj);
	op->build_io_pat (iopat);
	// char* name = el_opcode_to_string_map.value(op->opcode());
	char* name = get_mdes_opcode_string(op);
	OpInfo* info = opInfoMap->value(op);
	RU_init_iterator(name, op, iopat, cycle);
        if (!RU_get_next_nonconfl_alt(&outchar, &priority)) {
          cdbg << "Fatal : " << name << " did not have an alt" << endl;
        }
	RU_init_iterator(name, op, iopat, cycle);
        assert (RU_get_next_nonconfl_alt(&outchar, &priority));
	op->set_sched_opcode(outchar);
	schedule_op (region, op, info, cycle);
      }
    }
#endif
  }
}


// If branch latency > 1 and dummy branch is not the last scheduled op,
// schedule a dummy branch at last_br_sched_time + br_lat - 1
// (but an rts is allowed to be the last op in rts block,
// inserting dummy in an rts block is a problem,
// because dummy needs a target and rts block does not have fall through)

void Schedule::ensure_last_op_is_dummy (Compound_region* region) {

 Op* last_op = sched_list_vec[max_sched_cycle].tail();
 if (is_dummy_branch(last_op) || is_rts(last_op)) return;

 bool seen_br = false;
 Op* last_br = NULL;
 int last_br_sched_cycle = -1;
 // Deal with two cases separately
 if (El_ssched_reorder_scheduled_ops) {
   // Branches/dummies guaranteed to be at the right end
   for (int sched_cycle = max_sched_cycle;
	sched_cycle > max_sched_cycle - br_lat; sched_cycle-- ) {
     Op* tail_op = sched_list_vec[sched_cycle].tail();
     if (is_control_switch(tail_op) ) {
       assert (!is_dummy_branch(tail_op));
       seen_br = true;
       last_br = tail_op;
       last_br_sched_cycle = sched_cycle;
       break;
     }
   }
 } else {
   // Control switches may be anywhere in the cycle
   for (int sched_cycle = max_sched_cycle;
	sched_cycle > max_sched_cycle - br_lat; sched_cycle-- ) {
     for (List_iterator<Op*> li (sched_list_vec[sched_cycle]);
	    li != 0; li++ ) {
       if(is_control_switch (*li) ) {
	 assert (!is_dummy_branch(*li));
	 seen_br = true;
	 last_br = (*li);
	 last_br_sched_cycle = sched_cycle;
	 break;
       }
     }
     if (seen_br) break;
   }
 }
 assert (seen_br);
 max_sched_cycle = last_br_sched_cycle + br_lat - 1;

 // Determine original main exit in C0 order and fall_thru_edge
 Region_ops_reverse_C0_order roi (region);
 Op* orig_main_exit = (*roi);
 assert (is_control_switch(orig_main_exit)
 	&& (! is_dummy_branch(orig_main_exit) ) && (! is_rts(orig_main_exit)));
 Edge* fall_thru_edge
   		= get_outgoing_CONTROL0_fall_through_edge (orig_main_exit);
 bool uncond_br = ( is_bru(orig_main_exit) || is_brind(orig_main_exit) ||
		    is_rts(orig_main_exit) );
 assert  (uncond_br  || (fall_thru_edge != NULL) );

 // Create a control-merge and put it in the scheduling region
 // downstream modules require control merge with each branch
 // Draw Control-0 from orig_main_exit and Control-1 from last_branch
 Op* dbr_cm_op = new Op(C_MERGE);
 region->add_region(dbr_cm_op);
 dbr_cm_op->set_parent(region);
 Edge* origbr2cmop_edge = C0_connect_fallthrough (orig_main_exit, dbr_cm_op);
 Control* ctl = new Control();
 connect_ops(ctl, last_br, CONTROL1, dbr_cm_op, CONTROL1, 0, 0);
 ((Edge *)ctl)->set_latency(br_lat - 1);

 // Create dummy branch and put in the scheduling region
 // Draw C0 edge from dbr_cm_op and C1 from last_bra
 Op* dum_br = new Op(DUMMY_BR);
 region->add_region(dum_br);
 dum_br->set_parent(region);
 region->add_exit(dum_br);
 C0_connect_fallthrough (dbr_cm_op, dum_br);
 ctl = new Control();
 connect_ops(ctl, last_br, CONTROL1, dum_br, CONTROL1, 0, 0);
 ((Edge *)ctl)->set_latency(br_lat - 1);
   
 // Schedule dbr_cm_op and dum_br at new max_sched_cycle
 OpInfo* info;
 char iopat[MAXIOPATLEN];
 char *outchar;
 char *name;
 int priority;

 dbr_cm_op->build_io_pat(iopat);
 // name = el_opcode_to_string_map.value(dbr_cm_op->opcode());
 name = get_mdes_opcode_string(dbr_cm_op);
 info = new OpInfo(dbr_cm_op);
 opInfoMap->bind(dbr_cm_op, info);
 RU_init_iterator(name, dbr_cm_op, iopat, max_sched_cycle);
 assert (RU_get_next_nonconfl_alt(&outchar, &priority));
 dbr_cm_op->set_sched_opcode(outchar);
 schedule_op (region, dbr_cm_op, info, max_sched_cycle);
 sched_list_vec[max_sched_cycle].add_tail(dbr_cm_op);

 dum_br->build_io_pat(iopat);
 // name = el_opcode_to_string_map.value(dum_br->opcode());
 name = get_mdes_opcode_string(dum_br);
 info = new OpInfo(dum_br);
 opInfoMap->bind(dum_br, info);
 RU_init_iterator(name, dum_br, iopat, max_sched_cycle);
 assert (RU_get_next_nonconfl_alt(&outchar, &priority));
 dum_br->set_sched_opcode(outchar);
 schedule_op (region, dum_br, info, max_sched_cycle);
 sched_list_vec[max_sched_cycle].add_tail(dum_br);

 if (fall_thru_edge != NULL) {
   // If orig_main_exit was conditional with a fall_thru_edge
   // connect fall through to dummy branch
   assert (region->is_exit_edge (fall_thru_edge));
   orig_main_exit->remove_outedge(fall_thru_edge, CONTROL, CONTROL0, 0); 
   dum_br->add_outedge(fall_thru_edge, CONTROL, CONTROL0, 0);
   fall_thru_edge->set_src(dum_br, CONTROL0);

   // Copy over control flow frequency info to edge from main exit op
   // to dbr_cm_op
   Control_flow_freq *cf1 = get_control_flow_freq(fall_thru_edge);
   Control_flow_freq *cf2 = new Control_flow_freq();
   cf2->freq = cf1->freq;
   cf2->cc   = cf1->cc;
   set_control_flow_freq (origbr2cmop_edge, cf2); 
 } else {
   // Otherwise, set dummy branches target to one of the targets of the
   // last_br
   Control_flow_freq *cfreq = new Control_flow_freq();
   cfreq->freq = 0.0;
   cfreq->cc   = 0;
   set_control_flow_freq(origbr2cmop_edge, cfreq);

   Edge* last_taken_edge =
     get_outgoing_CONTROL0_non_fall_through_edge (last_br);
   Op* fake_fall_thru_target = last_taken_edge->dest();

   // Draw a C0 edge from dum_br to fall_thru_target
   C0_connect_fallthrough (dum_br, fake_fall_thru_target);
   Edge* fake_fall_thru_edge
   		= get_outgoing_CONTROL0_fall_through_edge (dum_br);
   assert (fake_fall_thru_edge != NULL);

   // Find target hb to attach C0 to
   Compound_region* target_hb = fake_fall_thru_target->parent();
   assert (target_hb->is_bb() || target_hb->is_hb());
   Compound_region* next_ancestor = target_hb->parent();
   while (next_ancestor->is_bb() || next_ancestor->is_hb()) {
     target_hb = next_ancestor;
     next_ancestor = target_hb->parent();
   }
   assert (next_ancestor != NULL);
   target_hb->add_entry(fake_fall_thru_edge);
   region->add_exit(fake_fall_thru_edge);
 
   // Make sure control flow frequencies are sufficiently in sync
   // These will be fixed up and made consistent by downstream modules
   // Set up a Control_flow_freq attribute of zero for new fake fall_thru_edge
   cfreq = new Control_flow_freq();
   cfreq->freq = 0.0;
   cfreq->cc   = 0;
   set_control_flow_freq(fake_fall_thru_edge, cfreq);
 }
}

void Schedule::mark_speculative_ops (Compound_region* region) {

  int sched_cycle;
  OpInfo* info;
  
  // Derive map from home block index to branch
  // Required for determining whether each op is speculative
  // May want to move this to sched initialization step to save walk over ops
  Map<int, Op*> idxBrMap;
  for (sched_cycle = 0; sched_cycle <= max_sched_cycle; sched_cycle++) {
    for (List_iterator<Op*> piter(sched_list_vec[sched_cycle]); piter !=0; piter++) {
      if (is_branch(*piter)) {
       info = opInfoMap->value(*piter);
       int orig_block = info->get_home_block();
       idxBrMap.bind(orig_block, *piter);
      }
    }
  }
 
 // Keep a running record of branches encountered in bit Vector brs_seen 
 // Bit set if associated branch has taken effect
 Bitvector brs_seen(num_exits, false);
 // List of branch indices over which op has speculated past
 List<int> spec_brs_index; 
 // Branches that have issued but not yet taken effect
 Slist<Stuple<int, Op*> > stime_branch_list;

 // Iterate over scheduled cycles
 for (sched_cycle = 0; sched_cycle <= max_sched_cycle; sched_cycle++) {

   // Mark bit-Vector for already issued branches that have now taken effect
   while ((!stime_branch_list.is_empty()) &&
	  ((stime_branch_list.head()).first + br_lat
	   <= sched_cycle))  {
     Stuple<int, Op*> st_br = stime_branch_list.pop();
     Op* br = st_br.second;
     brs_seen.set_bit((opInfoMap->value(br))->get_home_block());
   }

   // Iterate over ops in this cycle  
   List_iterator<Op*> oiter(sched_list_vec[sched_cycle]) ;
   for (; oiter !=0; oiter++) {

     // Make list of branch indices beyond which this op has speculated past
     info = opInfoMap->value(*oiter);
     int orig_block = info->get_home_block();
     bool spec_flag = false;
     spec_brs_index.clear();
     int pos;
     for (pos = 0; pos < orig_block; pos++) {
       if (brs_seen.bit(pos) == false) {
	 spec_flag = true;
	 spec_brs_index.add_tail(pos);
       }
     }

     // Following was added to allow emulation to work.
     if (spec_flag && is_load (*oiter) && !(*oiter)->flag (EL_OPER_SAFE_PEI)) {
       (*oiter)->set_flag (EL_OPER_MASK_PE);
       mask_pe_set = true;
     }

     // If op has speculated past any branch, mark it speculative
     if (spec_flag) {
       (*oiter)->set_speculative();     
       /* For the time being, mark all ops that move above a branch as speculative.
	  Don't also check if a C1 edge has been broken, as below.
	  Make changes when FRP code starts coming in. -- SGA 7/96
	  bool speculative = false;
   
	  for (List_iterator<int> briter (spec_brs_index); briter !=0; briter++) {
          Op* br = idxBrMap.value(*briter);
	  for (Op_outedges eiter(br); eiter !=0; eiter++) {
	  if ((*eiter)->dest() != (*oiter)) continue;
	  if (((*eiter)->is_control()) && ((*eiter)->src_port() == CONTROL1))
	  { 
	  speculative = true;
	  (*oiter)->set_speculative();
	  break; }
	  }
	  if (speculative) break;
	  }
	  */
     }   

     // If branch op, add to the issued but not completed branch list
     if (is_branch(*oiter)) {
       Stuple<int, Op*> st_br (sched_cycle, (*oiter));
       stime_branch_list.add(st_br);
     }
   }
 }

 // Output delay slot filling stats
#ifdef DELAY_SLOT_STATS
 Vector<double> exit_freqs(num_exits, 0);
 double total_flow_freq = 0.0;

 for (Region_exit_edges ei(region); ei!=0; ei++) 
 {
   Op* exit_op = (*ei)->src();
   info = opInfoMap->value(exit_op);
   int exit_home_block = info->get_home_block();
   Control_flow_freq *cfreq = get_control_flow_freq(*ei);
   total_flow_freq += cfreq->freq;
   exit_freqs[exit_home_block] += cfreq->freq;
 }

 // account for an RTS (which doesn't have an exit edge!)
 for (Region_exit_ops exit_ops(region); exit_ops != 0; exit_ops++) {
   if (is_return(*exit_ops)) {
     Op* last_exit_op = *exit_ops;
     info = opInfoMap->value(last_exit_op);
     int exit_home_block = info->get_home_block();
     exit_freqs[exit_home_block] = region->weight - total_flow_freq;
   }
 }

 for (Region_exit_ops xi (region); xi != 0; xi++) {
   Op* exit_op = *xi;
   bool is_main_exit = (exit_op->id() == main_exit->id());
   int exit_op_block = opInfoMap->value(exit_op)->get_home_block();
   int ops_from_above = 0;
   int ops_from_below = 0;
   for (sched_cycle = exit_op->sched_time() + 1;
	sched_cycle < exit_op->sched_time() + br_lat; sched_cycle++) {
     for (List_iterator<Op*> piter(sched_list_vec[sched_cycle]);
	  piter !=0; piter++) {
       Op* ds_op = (*piter);
       // skip if pseudo-op
       if (!ofilter_rep->filter(ds_op)) continue;
       info = opInfoMap->value(ds_op);
       if (info->get_home_block() <= exit_op_block) {
	 ops_from_above++;
       } else {
	 ops_from_below++;
       }
     }
   }
   (*EL_STAT_STREAM) << "          Delay_slot_stats: " <<  region->id() << '\t'
		  << exit_op->id() << '\t' 
     		  << exit_freqs[exit_op_block] << '\t'
		  << is_main_exit << '\t'
		  << ops_from_above << '\t' << ops_from_below << endl;
 }
#endif

}

//  Moves branches in sched_list_vec down by delay_slots
void Schedule::move_branches_to_branch_point (Compound_region* region) {
  int sched_cycle;
  int delay_slots = br_lat - 1;
  assert (delay_slots > 0);

  // Handle special case of rts block
  // Assume no other control-switches (including branches) in rts block
  Op* last_op = sched_list_vec[max_sched_cycle].tail();
  if (is_rts(last_op)) {
    sched_list_vec[max_sched_cycle].remove(last_op);
    max_sched_cycle += delay_slots;
    sched_list_vec[max_sched_cycle].add_tail(last_op);
    change_sched_time (last_op, max_sched_cycle);
    return;
  }

  // First move branches from sched_list_vec to branches_to_move    
  Vector <List<Op*> > branches_to_move(max_sched_cycle); 
  for (sched_cycle = 0; sched_cycle <= max_sched_cycle - delay_slots;
       sched_cycle++) {
    bool brs_removed = false;
    for (List_iterator<Op*> oi(sched_list_vec[sched_cycle]); oi != 0; oi++) {
      Op* op = (*oi);
      if (is_control_switch(op)) {
	brs_removed = true;
	assert (!is_dummy_branch(op));
	branches_to_move[sched_cycle].add_tail(op);
      }
    }
    if (brs_removed) {
      for (List_iterator<Op*> bi(branches_to_move[sched_cycle]);
	   bi != 0; bi++) {
	sched_list_vec[sched_cycle].remove (*bi);
      }
    }
  }

  // Last but one (delay_slots - 1) cycles should not contain any branches
  for (sched_cycle = ELCOR_MAX(0, max_sched_cycle - delay_slots + 1);
       sched_cycle < max_sched_cycle; sched_cycle++) {
    for (List_iterator<Op*> oi(sched_list_vec[sched_cycle]); oi != 0; oi++) {
      Op* op = (*oi);
      assert (!is_control_switch(op));
    }
  }

  // Last cycle must have exactly one dummy branch and no other branches
  Op* dummy = NULL;
  for (List_iterator<Op*> oi(sched_list_vec[max_sched_cycle]); oi != 0; oi++) {
    Op* op = (*oi);
    assert (is_dummy_branch(op) || (!is_control_switch(op)));
    if (is_dummy_branch(op) ) {
      assert (dummy == NULL);
      dummy = op;
      sched_list_vec[max_sched_cycle].remove(op);
    } else {
      assert (!is_control_switch(op));
    }
  }

  // Put back removed operations into sched list displaced downwards
  // by delay_slots cycles
  for (sched_cycle = delay_slots; sched_cycle <= max_sched_cycle;
       sched_cycle++) {
    if (! branches_to_move[sched_cycle - delay_slots].is_empty()) {
      for (List_iterator<Op*> ci(branches_to_move[sched_cycle - delay_slots]);
	   ci != 0; ci++) {
	Op* op = (*ci);
	sched_list_vec[sched_cycle].add_tail (op);
	change_sched_time (op, sched_cycle);
      }
    }
  }
  // Finally put back the dummy branch into sched_list_vec
  assert (dummy != NULL);
  sched_list_vec[max_sched_cycle].add_tail(dummy);
}

// rethread C0 edges according to schedule and cleanup
void Schedule::thread_scheduled_region (Compound_region* region) {
 
 // This section redraws C0 edges using schedule order in sched_list
 // Control merge must be first op in C0 order
 int cycle;

 Region_entry_ops entry(region);
 Op* cm_op = (*entry);
 assert (is_control_merge(cm_op));
 sched_list_vec[0].remove(cm_op);
 Op* prev = cm_op;
 
 // Clear list of all Ops in region and put  CM-op and its related ops
 // back on the list, followed later by other ops and their related ops
 region->clear_region();
 region->add_region(cm_op);
 for (List_iterator<Op*> relcmop (cm_op->get_relops()); relcmop !=0;
  						++relcmop) {
   region->add_region(*relcmop); }
 
 // Save main_exit_op for correctness check at end
 Region_ops_reverse_C0_order roi (region);
 Op* main_exit_op = (*roi);

 // Find fall thru CONTROL0 edge for each op in sched_list_vec
 // and connect that op to next op in sched_list_vec
 // Iterate over scheduled ops in order
 for (cycle = 0; cycle <= max_sched_cycle; cycle++) {
   List<Op*> sched_list = sched_list_vec[cycle];

   // Append current op and its related ops to region dlist
   for (List_iterator<Op*> oiter (sched_list); oiter !=0; oiter++) {
     Op* curr = (*oiter);
     if (is_control_switch(curr)) {
       for (List_iterator<Op*> relswop (curr->get_relops()); relswop !=0;
	    relswop++) {
	 Op* relop = *relswop;
	 region->add_region(relop);
       }
     }
     region->add_region(curr);

     Edge* seq_edge = get_outgoing_CONTROL0_fall_through_edge (prev);

     // ensure_last_op_is_dummy ensures existing C0 edge from prev op
     assert (seq_edge != NULL);
     //       Alt_num dest_alt = get_new_in_alt_num (curr);
     (seq_edge->dest())->remove_inedge(seq_edge, CONTROL, CONTROL0);
     seq_edge->set_dest(curr, CONTROL0);
     curr->add_inedge(seq_edge, CONTROL, CONTROL0);
     prev = curr;
   }
 }

 if(prev->id() != main_exit_op->id())
   cdbg << "prev : " <<  get_mdes_opcode_string(prev) << " main_exit_op :" <<   get_mdes_opcode_string(main_exit_op) << endl;
 assert (prev->id() == main_exit_op->id());
}

void Schedule::check_schedule (Compound_region* region) {
 
  char* name;
  char iopat[MAXIOPATLEN];
  char *outchar;
  int priority, cycle;
#ifdef NO_FILL_DELAY_SLOTS
  int br_min_time = 0;
#endif

  // Check that C0 order is same as in sched_list_vec
  Region_ops_C0_order coi (region);
  coi++; // skip past entry Cm which is not in sched_list_vec
  for (cycle = 0; cycle <= max_sched_cycle; cycle++) {
    for (List_iterator<Op*> oiter(sched_list_vec[cycle]); oiter !=0; oiter++) {
      //     cout << "CO: " << (*coi)->id() <<
      //		"\tSlist: " <<  (*oiter)->id() << endl;
     assert ((*coi)->id() == (*oiter)->id());
     assert ((*coi)->sched_time() == cycle);
     coi++;
   }
 }
 assert (coi == 0);

 // Check the schedule produced for violation of dependency constraints
 for (Region_ops_C0_order opiter(region); opiter != 0; opiter++) {
    Op* op = (*opiter);
    assert (opInfoMap->is_bound(op));
    OpInfo* info = opInfoMap->value(op);
    assert (info->is_scheduled());
    int op_sched_time = op->sched_time();   
    sched_list_vec[op_sched_time].remove(op);
    if (move_branches && is_control_switch(op) && (!is_dummy_branch(op)))
      op_sched_time = op_sched_time - br_lat + 1;
    if ((op_sched_time == 0) && (is_control_merge (op))) continue;

    int min_time = 0;
#ifdef NO_FILL_DELAY_SLOTS
    if (El_ssched_sched_model == 0) min_time = br_min_time;
#endif

    for (Op_inedges_rel in(op, efilter_rep); in != 0; ++in) {
      Op* source = real_op((*in)->src());
      assert (opInfoMap->is_bound(source));
      OpInfo* src_info = opInfoMap->value(source);
      assert (src_info->is_scheduled());
      int src_sched_time = source->sched_time();
      if (move_branches && is_control_switch(source)
      		&& !is_dummy_branch(source))
	src_sched_time = src_sched_time - br_lat + 1;
      min_time = ELCOR_MAX (src_sched_time + (*in)->latency(), min_time);
    }
    assert (min_time <= op_sched_time);
    
    if (!move_branches && (min_time < op_sched_time)) {
        bool op_placed = false;
        op->build_io_pat(iopat);
        // name = el_opcode_to_string_map.value(op->opcode());
        name = get_mdes_opcode_string(op);
        RU_remove(op, iopat, op_sched_time);
        while (min_time <= op_sched_time) {
          RU_init_iterator(name, op, iopat, min_time);
          if(RU_get_next_nonconfl_alt(&outchar, &priority)) {
	     op_placed = true;
	     RU_place();
	     if (min_time <op_sched_time)
                cerr << "Warning: Schedule checker says Op can be placed earlier:\n"
	        << "Op_name: " << name << "\t can be placed at time: " << min_time
	        << "\nOp description:\n" << (*op) << endl;
	   }
           min_time++;
         }
	 assert(op_placed);
     }
#ifdef NO_FILL_DELAY_SLOTS
    if ((El_ssched_sched_model == 0) && (is_control_switch(op))) {
      br_min_time = op->sched_time() + NUM_DELAY_SLOTS + 1;
    }
#endif

 }
 for (cycle = 0; cycle <= max_sched_cycle; cycle++) {
   assert (sched_list_vec[cycle].is_empty());
 }
}
