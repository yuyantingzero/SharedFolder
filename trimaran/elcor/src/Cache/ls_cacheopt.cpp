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
//      File:           ls_cacheopt.cpp
//      Authors:        Teresa L. Johnson
//      Created:        May 1996
//      Description:    Measure parallelism/slack associated with missing loads
//
/////////////////////////////////////////////////////////////////////////////

#include <math.h>

#include "ls_cacheopt.h"
#include "ls_adv_load.h"
#include "ls_prefetch.h"
#include "load_info.h"
#include "load_par.h"
#include "ls_get_profile.h"
#include "ls_efilt.h"
#include "ls_prof_mem_edge.h"
#include "el_cache_init.h"

#include "iterators.h"
#include "op_info.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "sched_functions.h"
#include "dbg.h"
#include "intf.h"
#include "slist.h"
#include "list_set.h"
#include "opcode.h"
#include "attributes.h"
#include "el_io.h"

class Hash_map <Op*, Hash_set<Op*>* > *force_above;
class Hash_map <Op*, Hash_set<Op*>* > *force_below;

int calc_res_length(Compound_region* region) {
  char iopat[MAXIOPATLEN]; // used to hold I/O pattern for operation
  RMSL_alloc();

  for (Region_ops_C0_order oiter (region); oiter != 0; oiter++) {
    Op* op = *oiter;
    if (el_opcode_to_string_map.is_bound((op)->opcode())) {
      (op)->build_io_pat(iopat);
      //RMSL_nextop(el_opcode_to_string_map.value((op)->opcode()), iopat);
      //tangw (06-03-02)
      RMSL_nextop(get_mdes_opcode_string(op), iopat);


    }
  }
  int result=RMSL_value();
  RMSL_dealloc();
  return result;
}

LSBlockInfo* create_cacheopt_loadinfo_map (Compound_region* region) {

 loadInfoMap = new Hash_map<Op*, LoadInfo*> (hash_op_ptr, 211);
 lsBlockInfo = new LSBlockInfo;
 int init_val = 0;
 lsBlockInfo->num_loads_mr.resize(NUM_MR_BUCKETS, init_val);
 lsBlockInfo->region = region;
 lsBlockInfo->cb_exec_freq = region->weight;
 int crit_path_len = 1;
 for (Region_exit_ops xiter(region); xiter !=0; xiter++) {
   Op* op = (*xiter);
   assert (opInfoMap->is_bound(op));
   //if (!opInfoMap->is_bound(op)) continue;
   OpInfo* info = opInfoMap->value(op);
   // TLJ - initial etime filled in via 'new Listt'

   // SGA - dummy branch exits always have branch latency of 1
   // replaced MDES_branch_latency() with br_lat
   int br_lat = MDES_branch_latency();
   if (is_dummy_branch(op)) br_lat = 1;
   crit_path_len = ELCOR_MAX (crit_path_len,
		 (info->get_etime() + br_lat));
 }
 lsBlockInfo->crit_path_len = crit_path_len;
 // change comp_cycles to be resource usage based
 lsBlockInfo->comp_cycles = crit_path_len - MDES_branch_latency() + 1;
 if (El_cache_use_res_dist)
   lsBlockInfo->res_length = calc_res_length(region);
 float stalls = 0.0f; 

 // Calculate the early times with no mem deps
 Hash_map<Op*, int> early_map (hash_op_ptr, 211);
 Region_entry_ops entry_op(region);
 calculate_earliest_times_from_entry_op(region, *entry_op, early_map,
	new No_mem_dep_filter());

 Op *fallthrough_exit = NULL;
 int min_ltime = 9999999,new_ltime;
 for (Region_exit_ops exit_ops1(region); exit_ops1 != 0; exit_ops1++) {
   OpInfo *info = opInfoMap->value(*exit_ops1);
   if (!fallthrough_exit) {
     fallthrough_exit = *exit_ops1;
     min_ltime = 
	info->get_ltime(opInfoMap->value(*exit_ops1)->get_home_block());
   }
   else if ((new_ltime=
	info->get_ltime(opInfoMap->value(*exit_ops1)->get_home_block())) <
	min_ltime) {
     min_ltime = new_ltime;
     fallthrough_exit = *exit_ops1;
   }
 }

 // For each operation init its ltime, and for loads fill in all
 // LoadInfo fields. Also fill in miss ratio histogram and count
 // stall cycles.
 Op *last_jsr = NULL;
 int num_ops = 0;
 for (Region_ops_C0_order oiter(region); oiter != 0; oiter++) {
   Op* op = (*oiter);
   if (is_call(op) && !op->flag(EL_OPER_SIDE_EFFECT_FREE)) last_jsr = op;
   if (!is_pseudo(op)) num_ops++;
   assert (opInfoMap->is_bound(op));
   OpInfo* info = opInfoMap->value(op);
   if (!is_load(op)) continue;
     double mr = get_load_miss_ratio(op);
     int exec_freq = get_load_exec_freq(op);
     stalls += (float)mr*exec_freq;
     bool incflag = false;
     for (int i=0; i<NUM_MR_BUCKETS; i++) {
       if (mr <= bkts_lim[i]) {
         lsBlockInfo->num_loads_mr[i] += 1;
         incflag = true;
         break;
       }
     }
     assert (incflag);
     // TLJ - want to fill in etime and ltime for all loads
     if (mr <= MR_GATHER_STATS) continue;
       assert (!loadInfoMap->is_bound(op));
       LoadInfo* ldinfo = new LoadInfo(op);
       loadInfoMap->bind(op, ldinfo);
       ldinfo->set_load_op(op);
       ldinfo->set_miss_ratio((float)mr);
       ldinfo->set_exec_freq(exec_freq);
       int data_miss_count = get_load_miss_count(op);
       ldinfo->set_data_miss_count(data_miss_count);
       ldinfo->set_pos_in_block(num_ops);
     
       ldinfo->set_etime(info->get_etime());
       ldinfo->set_ltime(info->get_ltime());
       ldinfo->set_slack(info->get_ltime() - info->get_etime());

       ldinfo->set_last_jsr(last_jsr);

       if (early_map.is_bound(op)) // set early time to one calculated.
         ldinfo->set_nomem_etime(early_map.value(op));
       else // no early time calculated, so set early time to -1.
         ldinfo->set_nomem_etime(-1);
 }

 int num_exits = 0;
 int issue_count = 0;
 int branch_count = 0;
 int load_count = 0;
 int store_count = 0;
 int pbr_count = 0;
 int cmpp_count = 0;
 int ialu_count = 0;
 int falu_count = 0;

 for (Region_ops_C0_order oi (region); oi != 0; oi++) {
   Op* op = *oi;
   if (is_branch(op) || is_rts(op)) num_exits++;

   // Increment op counters
   if (is_pseudo(op))          continue;
   issue_count++;

   if (is_control_switch(op)) {
     branch_count ++;
   }
   else if (is_load(op)) {
     load_count ++;
   }
   else if (is_store(op)) {
     store_count ++;
   }
   else if (is_pbr(op)) {
     pbr_count ++;
   }
   else if (is_cmpp(op) || is_predicate_clear(op) || is_predicate_set(op) ||
	    is_clear_all_rotating(op) ) {
     cmpp_count ++;
   }
   else if (is_ialu(op)) {
     ialu_count ++;
   }
   else if (is_falu(op)) {
     falu_count ++;
   }
   else {
     El_punt("ls_cacheopt: unknown op type for op %d (%s)", op->id(),
	     get_mdes_opcode_string(op));
   }
 }

 lsBlockInfo->res_use.resize(MAX_RES);
 lsBlockInfo->res_use[ISSUE_RES] = issue_count;
 lsBlockInfo->res_use[INT_RES] = ialu_count;
 lsBlockInfo->res_use[FLOAT_RES] = falu_count;
 lsBlockInfo->res_use[LOAD_RES] = load_count;
 lsBlockInfo->res_use[STORE_RES] = store_count;
 lsBlockInfo->res_use[PBR_RES] = pbr_count;
 lsBlockInfo->res_use[CMPP_RES] = cmpp_count;
 lsBlockInfo->res_use[BRANCH_RES] = branch_count;

 lsBlockInfo->num_ops = num_ops;
 lsBlockInfo->num_exits = num_exits;
 lsBlockInfo->norm_stall_cycles = stalls;

 lsBlockInfo->exitInfoVec.resize(num_exits);
 int xnum = 0;
 double cb_residual_freq = region->weight;
 for (Region_ops_C0_order xi (region); xi != 0; xi++) {
   Op* exit_op = (*xi);
   if (!is_branch(exit_op) && !is_rts(exit_op)) continue;
   assert (xnum < num_exits);
   LSExitInfo* lsExitInfo = new LSExitInfo;
   lsBlockInfo->exitInfoVec[xnum] = lsExitInfo;
   OpInfo* info = opInfoMap->value(exit_op);
   lsExitInfo->exit_op = exit_op;
   lsExitInfo->parent_region = region;
   int br_lat = MDES_branch_latency();
   if (is_dummy_branch(exit_op)) br_lat = 1;
   lsExitInfo->crit_path_len = info->get_etime() + br_lat;
   lsExitInfo->target_region = NULL;

   if (is_branch(exit_op)) {
     double exit_freq = 0.0;
     for (Op_outedges ei (exit_op); ei != 0; ei++) {
       Edge *edge = *ei;
       if (! region->is_exit_edge(edge))
	 continue;
       Control_flow_freq *cfreq = get_control_flow_freq (edge);
       cb_residual_freq -= cfreq->freq;
       exit_freq += cfreq->freq;
       Compound_region* target_region = edge->dest()->parent();
       while (target_region->parent()->is_hb() ||
	      target_region->parent()->is_bb()) {
	 target_region = target_region->parent();
       }
       if(lsExitInfo->target_region == NULL)
	 lsExitInfo->target_region = target_region;
     }
     lsExitInfo->exit_freq = exit_freq;
   }
       
   /* RTS's have no edges; but treat as branch */
   if (is_rts(exit_op)) {
     lsExitInfo->exit_freq = cb_residual_freq;
   }

   int issue_count = 0;
   int branch_count = 0;
   int load_count = 0;
   int store_count = 0;
   int pbr_count = 0;
   int cmpp_count = 0;
   int ialu_count = 0;
   int falu_count = 0;
   if (!is_pseudo(exit_op)) branch_count++;

   for (Op_ancestors ai (exit_op, region, NON_CONTROL0_INEDGES); ai !=0; ai++) {

     Op* op = *ai;
     if (is_pseudo(op))          continue;
     issue_count++;

     if (is_control_switch(op)) {
       branch_count ++;
     }
     else if (is_load(op)) {
       load_count ++;
     }
     else if (is_store(op)) {
       store_count ++;
     }
     else if (is_pbr(op)) {
       pbr_count ++;
     }
     else if (is_cmpp(op) || is_predicate_clear(op) || is_predicate_set(op) ||
	      is_clear_all_rotating(op) ) {
       cmpp_count ++;
     }
     else if (is_ialu(op)) {
       ialu_count ++;
     }
     else if (is_falu(op)) {
       falu_count ++;
     }
     else {
       El_punt("ls_cacheopt: unknown op type for op %d (%s)", op->id(),
	       get_mdes_opcode_string(op));
     }
   }

   lsExitInfo->res_use.resize(MAX_RES);
   lsExitInfo->res_use[ISSUE_RES] = issue_count;
   lsExitInfo->res_use[INT_RES] = ialu_count;
   lsExitInfo->res_use[FLOAT_RES] = falu_count;
   lsExitInfo->res_use[LOAD_RES] = load_count;
   lsExitInfo->res_use[STORE_RES] = store_count;
   lsExitInfo->res_use[PBR_RES] = pbr_count;
   lsExitInfo->res_use[CMPP_RES] = cmpp_count;
   lsExitInfo->res_use[BRANCH_RES] = branch_count;
   xnum++;
 }
     
  // Set final LoadInfo fields for all loads.
 for (Hash_map_iterator<Op*, LoadInfo*> miter(*loadInfoMap);
      miter != 0; miter++) {
	 (*miter).second->set_num_ops_in_block(num_ops);
	 (*miter).second->set_block_path_len(crit_path_len);
 }

 return lsBlockInfo;
}

// Recompute the slack, etime for all uses of load op, and
// all outgoing edges of use. Optimized 7/29/96 so that each
// op visited at most once, by using hash set and traversing
// in C0 order, rather than recomputing recursively.
void recompute_etime(Compound_region *region, Op *op) {
 int new_early_time, etime;
 Op *curr_op;
 Hash_set<Op*> process_set(hash_op_ptr, 211);

 process_set += op;

 Region_ops_C0_order oiter (region, op);
 for (; oiter != 0; oiter++) {
   curr_op = *oiter;
   if (process_set.is_member(curr_op)) {
     // Remove current op from hash set
     process_set -= curr_op;

     assert (opInfoMap->is_bound(curr_op));
     OpInfo* info = opInfoMap->value(curr_op);
     etime = info->get_etime();
     // for all ougoing edges to uses update etime and slack
     for (Op_outedges_rel iter(curr_op,NON_CONTROL0_OUTEDGES);iter!=0;iter++) {
       new_early_time = etime + (*iter)->latency();
       Op *dest = real_op((*iter)->dest());
       if (!opInfoMap->is_bound(dest)) continue;
       OpInfo* info2 = opInfoMap->value(dest);
       // check if early time based on new early time of src is greater
       // than early time of dest.
       if (new_early_time > info2->get_etime()) {
     	//cout << "e\t" << *(*iter) << endl;
     	info2->set_etime(new_early_time);
     	// recompute slack
     	if (new_early_time > info2->get_ltime())
	  info2->set_ltime(new_early_time);
     	info2->set_slack(info2->get_ltime() - new_early_time);
     	if (loadInfoMap->is_bound(dest)) {
          LoadInfo* ldinfo = loadInfoMap->value(dest);
          ldinfo->set_etime(new_early_time);
          if (new_early_time > ldinfo->get_ltime())
	    ldinfo->set_ltime(new_early_time);
          ldinfo->set_slack(ldinfo->get_ltime() - new_early_time);
     	}
     	// Since there was a change, place dest into hash set for later processing
     	process_set += dest;
       }
     }
   }
 }
}

// Recompute the slack, ltime for all incoming edges of load op, and
// all outgoing edges of use. Optimized 7/29/96 so that each
// op visited at most once, by using hash set and traversing
// in C0 order, rather than recomputing recursively.
void recompute_ltime(Compound_region *region, Op *op) {
 int new_late_time, ltime;
 Op *curr_op;
 Hash_set<Op*> process_set(hash_op_ptr, 211);

 Region_ops_reverse_C0_order oiter (region, op);
 process_set += *oiter;
 for (; oiter != 0; oiter++) {
   curr_op = *oiter;
   if (process_set.is_member(curr_op)) {
     // Remove current op from hash set
     process_set -= curr_op;

     assert (opInfoMap->is_bound(curr_op));
     OpInfo* info = opInfoMap->value(curr_op);
     ltime = info->get_ltime();
     // for all incoming edges update ltime and slack
     for (Op_inedges_rel iter(curr_op,NON_CONTROL0_OUTEDGES);iter!=0;iter++) {
       new_late_time = ltime - (*iter)->latency();
       Op *src = real_op((*iter)->src());
       assert (opInfoMap->is_bound(src));
       OpInfo* info2 = opInfoMap->value(src);
       // check if late time based on new late time of dest is less
       // than late time of src.
       if (new_late_time < info2->get_ltime()) {
     	//cout << "l\t" << *(*iter) << endl;
     	info2->set_ltime(new_late_time);
     	// recompute slack
     	info2->set_slack(new_late_time - info2->get_etime());
     	if (loadInfoMap->is_bound(src)) {
          LoadInfo* ldinfo = loadInfoMap->value(src);
          ldinfo->set_ltime(new_late_time);
          ldinfo->set_slack(new_late_time - ldinfo->get_etime());
     	}
     	// Since there was a change, place src into hash set for later processing
     	process_set += src;
       }
     }
   }
 }
}

int calc_res_dist_slack(Compound_region* region, Op* ld, 
				LSBlockInfo* lsBlockInfo) {
  // must filter out control zero edges
  Filter<Edge*>* efilter  = new Filter_not <Edge*> (CONTROL0_INEDGES,  false);
  char iopat[MAXIOPATLEN]; // used to hold I/O pattern for operation
  RMSL_alloc();
  Hash_set<Op*> universe;
  Hash_set<Op*> trapped_above_first;
  Hash_set<Op*> trapped_below_first;
  Hash_set<Op*> trapped_above_second;
  Hash_set<Op*> trapped_below_second;
  Hash_set<Op*> can_schedule_along;

  for (Region_ops_C0_order oiter (region); oiter != 0; oiter++) {
    Op* op = *oiter;
    universe += op;
  }
  for (Op_ancestors op2(ld, region, efilter);  op2!=0 ; op2++)
         {trapped_above_first += *op2;}
  for (Op_outedges_rel iter(ld,new Outedge_port_filter(DEST));iter!=0;iter++) {
    for (Op_descendants op3((*iter)->dest(), region, efilter);  op3!=0 ; op3++)
      {trapped_below_second += *op3;}
  }
  can_schedule_along = universe; 
  can_schedule_along-=trapped_above_first;
  can_schedule_along-=trapped_below_second;
  // take out the ones that were forced out by previous calls to this routine
  if (force_above->is_bound(ld))
    can_schedule_along-=*(force_above->value(ld));
  if (force_below->is_bound(ld))
    can_schedule_along-=*(force_below->value(ld));
  can_schedule_along += ld; // between includes first

  int num_ops=0; // used to count number of ops
  for (Hash_set_iterator<Op*> op4(can_schedule_along); op4!=0; op4++) {
  // present  ops to RMSL
    if (el_opcode_to_string_map.is_bound((*op4)->opcode())) {
      (*op4)->build_io_pat(iopat);
      //cerr << (*op4)->opcode() << endl;
      //cerr << iopat << endl;
      //tangw (06-03-02)
      //RMSL_nextop(el_opcode_to_string_map.value((*op4)->opcode()), iopat);
      RMSL_nextop(get_mdes_opcode_string(*op4), iopat);


      num_ops++;
    }
  }
  int result=RMSL_value();

  result = ELCOR_MIN(result,
	    ELCOR_MAX(lsBlockInfo->res_length-lsBlockInfo->comp_cycles,0));

  // find the ops which we can schedule along, but that are not
  // trapped between.
  for (Op_outedges_rel itr(ld,new Outedge_port_filter(DEST));itr!=0;itr++) {
    for (Op_ancestors op1((*itr)->dest(), region, efilter);  op1!=0 ; op1++)
      {trapped_above_second += *op1;}
  }
  for (Op_descendants op5(ld, region, efilter);  op5!=0 ; op5++)
      {trapped_below_first += *op5;}
  can_schedule_along -= ld; 
  can_schedule_along-=trapped_above_second;
  can_schedule_along-=trapped_below_first;

  // make a sorted list of ops in can_schedule_along 
  // in increasing order of the delta between the op's etime
  // and the load's etime
  Slist< Stuple <int, Op* > > slist;
  assert (loadInfoMap->is_bound(ld));
  LoadInfo* ldinfo = loadInfoMap->value(ld);
  for (Hash_set_iterator<Op*> liter(can_schedule_along);liter!=0;liter++) {
   Op* op = (*liter);
   assert (opInfoMap->is_bound(op));
   OpInfo* opinfo = opInfoMap->value(op);
   if (opinfo->get_etime() < ldinfo->get_etime()) {
     Stuple<int,Op*> stuple(ldinfo->get_etime()-opinfo->get_etime(),op);
     slist.add(stuple);
   }
   else {
     Stuple<int,Op*> stuple(opinfo->get_etime()-ldinfo->get_etime(),op);
     slist.add(stuple);
   }
  }

  // place ld and uses in force_above and force_below sets for these ops
  // only those needed to fill result number of cycles
  RMSL_init();
  for (Slist_iterator< Stuple <int, Op* > > siter(slist); siter!=0; siter++) {
    Op* op = (*siter).second;
    // check if this op can fit in result cycles
    if (el_opcode_to_string_map.is_bound(op->opcode())) {
      op->build_io_pat(iopat);
      //RMSL_nextop(el_opcode_to_string_map.value(op->opcode()), iopat);
      //tangw (06-03-02)
      RMSL_nextop(get_mdes_opcode_string(op), iopat);

 }

    if (RMSL_value() > result) break;

    if (force_above->is_bound(op))
      *(force_above->value(op)) += ld;
    else
    {
      force_above->bind(op,new Hash_set<Op*> (hash_op_ptr, 211));
      *(force_above->value(op)) += ld;
    }

    if (!force_below->is_bound(op))
      force_below->bind(op,new Hash_set<Op*> (hash_op_ptr, 211));
    for (Op_outedges_rel iter(ld,new Outedge_port_filter(DEST));iter!=0;iter++)
      *(force_below->value(op)) += (*iter)->dest();
  }

  RMSL_dealloc();
  return result;
}

void presched_cacheopt (Compound_region* region) {
 //cout << "start presched_cacheopt\n";
 // Call init_load_par to get Vector of edge filters
 Vector<Edge_filter* >* efilt_arr = init_load_par();

 // Fill in LoadInfo structs and LSBlockInfo struct
 LSBlockInfo* lsBlockInfo =  create_cacheopt_loadinfo_map (region);

 // determine entry_load, load_exit and dep_ops_above/below under
 // various dep models and put in loadInfo structure
 determine_loadpar (region, efilt_arr);

 if (El_cache_use_res_dist) {
   // Create the hash maps used when calc'ing res dists
   force_above = new Hash_map<Op*, Hash_set<Op*>* > (hash_op_ptr, 211);
   force_below = new Hash_map<Op*, Hash_set<Op*>* > (hash_op_ptr, 211);
 }

 // Put all loads into a sorted list based on their miss ratio,
 //	-> now by miss count
 // want this to be in decreasing order so we make miss ratio negative. 
 //cout << "sort ops by miss ratio\n";
 Slist< Stuple <double, Op* > > slist;
 for (Region_ops_C0_order oiter(region); oiter != 0; oiter++) {
   Op* op = (*oiter);
   if (is_load(op) && loadInfoMap->is_bound(op)) {
     LoadInfo* ldinfo = loadInfoMap->value(op);
     Stuple<double,Op*> stuple(-(ldinfo->get_data_miss_count()),op);
     slist.add(stuple);
   }
 }

 // Iterate over the list of ops based on decreasing value of miss ratio (count)
 for (Slist_iterator< Stuple <double, Op* > > siter(slist); siter!=0; siter++) {
   Op* op = (*siter).second;
   assert(is_load(op) && loadInfoMap->is_bound(op));
   LoadInfo* ldinfo = loadInfoMap->value(op);
   assert(opInfoMap->is_bound(op));
   OpInfo* info = opInfoMap->value(op);
 
   // If miss ratio less than a threshold then ignore it. Could also
   // do this by using a filterator?
   if (ldinfo->get_miss_ratio() <= El_cache_load_thresh) {
     Op_outedges_rel iter(op,new Outedge_port_filter(DEST));
     // fill in ldinfo presched_lat field
     //if (iter != 0)
       ldinfo->set_presched_lat((*iter)->latency());
     continue;
   }

   if (El_cache_load_sched_type == ADVANCED_LOADS) {
     // *** Change into an advanced load ***
     if (ldinfo->get_nomem_etime() < ldinfo->get_etime())
       convert_load_to_adv_load(region,op);
   }
   else if (El_cache_load_sched_type == PREFETCHES) {
     // *** Change into a prefetch ***
     if (ldinfo->get_nomem_etime() < ldinfo->get_etime())
       convert_load_to_prefetch(region,op);
   }
   else {
     assert(El_cache_load_sched_type==LONG_LATENCY);
   }

   // Make into a long latency load if there is no associated prefetch 
   Op *pre;
   int lat;
   if (!(pre = ldinfo->get_prefetch())) {
    //cout << "assign latency\n";
    if (El_cache_use_res_dist) {
      // get res dist slack
      ldinfo->set_res_dist_slack(calc_res_dist_slack(region, op, lsBlockInfo));
    }
    // Assign min of slack and miss latency to this load
    Op_outedges_rel iter(op,new Outedge_port_filter(DEST));
    //if (iter == 0) continue;
    lat = ELCOR_MAX(ldinfo->get_slack(),ldinfo->get_res_dist_slack());
    lat = ELCOR_MIN(lat,load_miss_lat(op)-(*iter)->latency());
    // fill in ldinfo presched_lat field
    ldinfo->set_presched_lat((*iter)->latency() + lat);
    for (;iter!=0;iter++) {
     //if (lat) cout << *(*iter) << " {+" << lat << "}" << endl;
     (*iter)->set_latency((*iter)->latency() + lat);
    }
   }
   else {
    // first get C1_latency from associated load
    Op_outedges_rel iter(op,new Outedge_port_filter(DEST));
    int C1_latency = (*iter)->latency();

    // must create ldinfo for prefetch
    assert (!loadInfoMap->is_bound(pre));
    ldinfo = new LoadInfo(pre);
    loadInfoMap->bind(pre, ldinfo);
    ldinfo->set_load_op(pre);
    ldinfo->set_etime(info->get_etime());
    ldinfo->set_ltime(info->get_ltime());
    ldinfo->set_slack(info->get_ltime() - info->get_etime());

    if (El_cache_use_res_dist) {
      // get res dist slack
      ldinfo->set_res_dist_slack(calc_res_dist_slack(region, pre, lsBlockInfo));
    }

    // Assign min of slack and miss latency to prefetch
    Op_outedges_rel itr(pre,new Outedge_port_filter(MEM));
    for (;itr!=0;itr++) {
      if ((*itr)->dest() == op) break;
    }
    assert (itr!=0 && (*itr)->dest() == op);
    lat = ELCOR_MAX(ldinfo->get_slack(),ldinfo->get_res_dist_slack());
    lat = ELCOR_MIN(lat,load_miss_lat(pre)-C1_latency);
    (*itr)->set_latency(lat);
    // fill in ldinfo presched_lat field (make it prefetch to use latency)
    ldinfo->set_presched_lat(lat+C1_latency);

    // make op the prefetch now
    op = pre;
    info = opInfoMap->value(op);
   }

   // This will affect the op's ltime. Since all of its outgoing edges
   // have increased in latency by lat, ltime will decrease by lat.
   // Slack will therefore need to be recomputed (can just subtract lat).
   ldinfo->set_ltime(ELCOR_MAX(ldinfo->get_ltime() - lat,ldinfo->get_etime()));
   info->set_ltime(ELCOR_MAX(info->get_ltime() - lat,info->get_etime()));
   ldinfo->set_slack(ELCOR_MAX(ldinfo->get_slack() - lat,0));
   info->set_slack(ELCOR_MAX(info->get_slack() - lat,0));

   // Recursively recompute the slack, etime for all uses of load, and
   // all outgoing edges of use.
   recompute_etime(region,op);

   // Recursively recompute the slack, ltime for all 
   // incoming edges of op.
   recompute_ltime(region,op);
 }

 //cout << "end presched_cacheopt\n";
}

static Hash_map<Region*, int>* RegStallMap = 
  	new Hash_map<Region*, int> (hash_region_ptr, 211);

void postsched_cacheopt (Compound_region* region) {
  int miss_cycles = 0;

  // TLJ - this will bomb if we are doing modulo scheduling (which may
  // pass the following a scheduled LoopBody). Needs fix
  for (Region_ops_C0_order oiter (region); oiter != 0; oiter++) {
    Op* op = *oiter;

    if (is_load_from_C1(op) && !is_prefetch(op)) {
      int miss_count = get_load_miss_count(op);
      int stalls = 0;
      //if (miss_ratio > 0) {
	Op *prefetch = NULL;
        LoadInfo* ldinfo = NULL;
        LoadInfo* ldinfo2 = NULL;
	if (loadInfoMap && loadInfoMap->is_bound(op)) {
          ldinfo = loadInfoMap->value(op);
	  prefetch = ldinfo->get_prefetch();
	  if (prefetch) {
	    assert(loadInfoMap->is_bound(prefetch));
	    ldinfo2 = ldinfo;
            ldinfo = loadInfoMap->value(prefetch);
	  }
	}
        int miss_lat = load_miss_lat(op);
	int lat = miss_lat;
	int additional = 0;
        for (Op_outedges_rel iter(op,new Reg_flow_filter);
							iter!=0;iter++) {
	  // If dest is a branch, need to add one to the difference
	  // between dest and src sched times.
	  //
	  if (is_branch(real_op((*iter)->dest())))
		additional = 1;
	  if (prefetch) {
	    int lat2 = ELCOR_MIN(lat,
		(*iter)->dest()->sched_time()-(*iter)->src()->sched_time()+
								additional);
	    // fill in the sched_lat field of ldinfo for load op
	    if (ldinfo2)
	      ldinfo2->set_sched_lat(lat2);
	    lat = ELCOR_MIN(lat,
		(*iter)->dest()->sched_time()-prefetch->sched_time()+
								additional);
	  }
	  else {
	    lat = ELCOR_MIN(lat,
		(*iter)->dest()->sched_time()-(*iter)->src()->sched_time()+
								additional);
	  }
	  //lat = ELCOR_MIN(lat,(*iter)->latency());
        }
	// fill in the sched_lat field of ldinfo
	if (ldinfo)
	  ldinfo->set_sched_lat(lat);
	// calc stalls from the sched_lat
	if (lat < miss_lat) {
          stalls = (miss_lat - lat) * miss_count;
	}
	if (is_adv_load(op)) {
	  double p = calculate_alias_probability(region,op);
	  miss_cycles += 
		(int)ceil((double)stalls*(1-p) + 
			p*(double)El_cache_alias_penalty*
			(double)get_load_exec_freq(op));
	}
	else {
          miss_cycles += stalls;
	}
      //}
    }
  }
  //cout << "Region " << region->id() << "  Dcache stall cycles " << miss_cycles
  //	<< endl;
  RegStallMap->bind(region,miss_cycles);

 //cout << "cleanup\n";
// write out the entries in loadInfo and delete them
 if (loadInfoMap) {
  for (Hash_map_iterator<Op*, LoadInfo*> miter(*loadInfoMap);
	miter != 0; miter++) {
   LoadInfo* ldi = (*miter).second;
   (*EL_STAT_STREAM) << (*ldi);
   delete ldi;
  }
  (*EL_STAT_STREAM) << (*lsBlockInfo);
  
  for (int i=0; i < lsBlockInfo->num_exits; i++) {
    delete lsBlockInfo->exitInfoVec[i];
  }
  delete lsBlockInfo;

// delete loadInfoMap
  delete  loadInfoMap;
  
   if (El_cache_use_res_dist) {
     // delete other hash maps
     delete force_above;
     delete force_below;
   }
 }
}

int get_region_stall_cycles(Compound_region* region) {
  int stalls = 0;

  if (region->flag(EL_REGION_SCHEDULED)) {
    assert (RegStallMap->is_bound(region));
    stalls += RegStallMap->value(region);
    RegStallMap->unbind(region);
  }
  else {
    for (Region_subregions sub (region); sub != 0; sub++) {
      Compound_region* subreg = (Compound_region* ) (*sub);
      stalls += get_region_stall_cycles (subreg);
    }
  }

  return stalls;
}

