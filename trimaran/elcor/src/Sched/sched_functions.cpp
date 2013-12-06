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
//      File:           sched_functions.cpp
//      Authors:        Santosh G. Abraham, Vinod Kathail,
//			Srini Mantripragada
//      Created:        April 1994
//      Description:    Scheduler functions such as etime/ltime calculation
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "defs.h"
#include "attributes.h"
#include "connect.h"
#include "intf.h"
#include "iterators.h"
#include "op_info.h"
#include "opcode.h"
#include "sched_functions.h"
#include "schedule.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "meld_constraints.h"
#include "el_ssched_init.h"
#include "hash_functions.h"
#include "slist.h"
#include "el_main.h"
#include "dbg.h"

void display_sched (Compound_region *r);

//MP: Remove all C0 edges not to/from start/stop node of hb
void el_remove_C1_edges (Compound_region *region)
{
  Op* start_op=get_first_region_op_from_subregions(region);
  Op* stop_op=get_last_region_op_from_subregions(region);
  
  Hash_set<Edge*> remove_edges(hash_edge_ptr, 32);

  for (Region_all_ops oiter(region); oiter != 0; oiter++) 
    {
      Op* op=*oiter;

      remove_edges.clear();

      for (Op_outedges int_edge(op, CONTROL1_OUTEDGES);
	   int_edge != 0; int_edge++) 
	{
	  Edge* edge = (*int_edge);
	  Op* src_op=edge->src();
	  Op* dest_op=edge->dest();

	  if((src_op!=start_op)&&
	     (dest_op!=stop_op))
	    {
	      remove_edges+=edge;
	    }
	}
      
      for(Hash_set_iterator<Edge*> eiter(remove_edges);eiter!=0;eiter++)
	{
	  Edge* edge=*eiter;
	  Op* src_op=edge->src();
	  Op* dest_op=edge->dest();
	  src_op->remove_outedge_recursively(edge);
	  dest_op->remove_inedge_recursively(edge);	  
	}
    }
  
  return;
}

//MP: Add a C1 edge (if it doesn't already exist) from
//every op in the region to the last op in the region
void el_add_C1_edges_to_stop_op(Compound_region *region)
{
  Op* stop_op=get_last_region_op_from_subregions(region);

  //process every op in the region...
  for (Region_ops_C0_order oiter(region); oiter != 0; oiter++) 
    {
      Op* op=*oiter;
      bool edge_exists=false;

      if(op==stop_op)
	continue;

      //check if the op already has an c1 edge to the stop_op
      for (Op_outedges int_edge(op, CONTROL1_OUTEDGES);
	   int_edge != 0; int_edge++) 
	{
	  Edge* edge = (*int_edge);
	  Op* dest_op=edge->dest();

	  if(dest_op==stop_op)
	    {
	      edge_exists=true;
	    }
	}
      
      //if the C1 edge is not already there, put it in
      if(!edge_exists)
	C1_connect(op,stop_op);
    }
  
  return;
}

static int maxtime = -1;  /* used to calculate the latest times */

/* calculate the earliest times of all ops from one entry op. */

void calculate_earliest_times_from_entry_op(Compound_region* region, 
                Op* entry_op, Hash_map<Op*, int>& early_map, 
                const Edge_filter* efilter)
// function only works correctly if entry_op is first op in region.
{
 Op *current_op, *src_op;
 int new_early_time, max_early_time;

 // Set entry op early time to 0.
 if (dbg(ss, 6)) {
   if (!is_control_merge(entry_op))
     El_punt("Found a non-control merge entry op");
   if (entry_op != get_first_region_op_from_inops (region))
     El_punt("entry op is not the first operation in the region.");
 }
 for (Region_ops_C0_order iter(region); iter != 0; ++iter)
 {
   current_op = *iter;
   max_early_time = 0;

   for (Op_inedges_rel ei(current_op, efilter); ei != 0; ei++)
   {
     src_op = real_op((*ei)->src());
     // source op is earlier in C0 order and must be bound by now
     if (dbg(ss, 6)) assert(early_map.is_bound(src_op)); 
     new_early_time = early_map.value(src_op) + (*ei)->latency();
     if (new_early_time > max_early_time)
       max_early_time = new_early_time;
   }
   
   if (dbg(ss, 6)) assert(!(early_map.is_bound(current_op)));
   early_map.bind(current_op, max_early_time);
   if (dbg(ss, 6)) 
     cout << "op = " << current_op->id() << " e = " << max_early_time << endl;
 }
}

    
void check_input (Compound_region* region) {

/* Some problems with using the Region_all_internal_edges iterator
 	Uncomment and fix problem later -- SGA 6/95
// Ensure that all intraregion edges have non-negative latencies
 for (Region_all_internal_edges eiter(region); eiter != 0; eiter++) {
 if (!(is_CONTROL0(*eiter)))
   assert ((*eiter)->latency() >= 0);
   }
*/

// Check that Region_ops_C0_order gets all the ops in the region
// except the switch and merge ops
 List<Op*> olist, clist;
 for (Region_all_ops oiter(region);      oiter != 0; ++oiter) {
    olist.add_head(*oiter); }
 for (Region_ops_C0_order citer(region); citer != 0; ++citer) {
    clist.add_head(*citer); }
 for (List_iterator<Op*> liter(olist);   liter != 0; ++liter) {
   if ((!(is_switch(*liter))) && (!(is_merge(*liter)))) {
     if (!clist.is_member(*liter)) {
	cerr << *(*liter) << endl;
     }
     assert(clist.is_member(*liter));
   }
 }
 for (List_iterator<Op*> miter(clist);   miter != 0; ++miter) {
    assert(olist.is_member(*miter)); }

// Make sets of all control-merge, switch, merge, branch, and brl ops
// Op* cm_op;
 Hash_set<Op*> cmset(hash_op_ptr, 211);
 Hash_set<Op*> swset(hash_op_ptr, 211);
 Hash_set<Op*> mrset(hash_op_ptr, 211);
 Hash_set<Op*> brset(hash_op_ptr, 211);
 Hash_set<Op*> brlset(hash_op_ptr, 211);
 for (Region_all_ops piter(region); piter != 0; ++piter) {
    if (is_control_merge (*piter)) cmset += (*piter);
    if (is_branch(*piter)) brset += (*piter);
    if (is_call(*piter))   brlset += (*piter);
    if (is_switch(*piter)) swset += (*piter);
    if (is_merge (*piter)) mrset += (*piter);
 }


// All related ops of control-merge op are merge ops
// All merge ops in block are related ops of control-merge op orBRLs
// All merge ops are not bound
// Sole related op of each merge op is control-merge op
 for (Hash_set_iterator<Op*> cmiter (cmset); cmiter !=0; ++cmiter) {
 Op* cm_op = (*cmiter);
 assert(opInfoMap->is_bound(cm_op));
 for (List_iterator<Op*> relcmop (cm_op->get_relops());
		 relcmop !=0; ++relcmop) {
   assert (is_merge(*relcmop));
   List_iterator<Op*> relmrop ((*relcmop)->get_relops());
   assert ((*relmrop)->opcode() == C_MERGE);
   ++relmrop;
   assert(relmrop == 0);
   assert (mrset.is_member(*relcmop));
   mrset -= (*relcmop);
   assert (!(opInfoMap->is_bound(*relcmop)));
 }
 }

// All related ops of calls are either switches or merges
// A merge or switch has call as sole related op
// and is member of mrset/swset constructed earlier (i.e. part of region)
 for (Hash_set_iterator<Op*> pbrl (brlset); pbrl != 0; ++pbrl) {
   for (List_iterator<Op*> relbrl ((*pbrl)->get_relops());
   		relbrl !=0; ++relbrl) {
      assert (is_merge(*relbrl) || is_switch(*relbrl));
      if (is_merge(*relbrl)) {
         assert (mrset.is_member(*relbrl));
         mrset -= (*relbrl);
	 assert (!(opInfoMap->is_bound(*relbrl)));
         List_iterator<Op*> relmrop ((*relbrl)->get_relops());
         assert ((*relmrop)->opcode() == BRL);
         ++relmrop;
	 assert(relmrop == 0);
      }
      if (is_switch(*relbrl)) {
         assert (swset.is_member(*relbrl));
         swset -= (*relbrl);
	 assert (!(opInfoMap->is_bound(*relbrl)));
         List_iterator<Op*> relswop ((*relbrl)->get_relops());
         assert ((*relswop)->opcode() == BRL);
         ++relswop;
	 assert(relswop == 0);	 
      }
   }
 }
 
// All related ops of branches are switches
// Switch has branch as its sole related op and i not bound to opInfoMap

 for (Hash_set_iterator<Op*> ptbr (brset); ptbr != 0; ++ptbr) {
   assert(opInfoMap->is_bound(*ptbr));
   for (List_iterator<Op*> relbr ((*ptbr)->get_relops());
   		relbr !=0; ++relbr) {
      assert (is_switch(*relbr) || (is_call(*ptbr) && is_merge(*relbr)) ||
                                   (is_return(*ptbr) && is_merge(*relbr)));
      List_iterator<Op*> relswop ((*relbr)->get_relops());
      assert (is_branch(*relswop));
      ++relswop;
      assert(relswop == 0);
//      assert (swset.is_member(*relbr));
      swset -= (*relbr);
      assert (!(opInfoMap->is_bound(*relbr)));
   }
 }
// assert (mrset.is_empty());
// assert (swset.is_empty());
}

// priority calculation and setting of priority in opInfoMap
void set_priority (Compound_region* region) {

 Op* exit_op;
 OpInfo* info;
 double total_flow_freq = 0.0;

 Region_exit_ops ei_op(region);
 int num_of_exits = opInfoMap->value(*ei_op)->get_num_ltimes();
 Vector<double> exit_freqs(num_of_exits, 0);

 for (Region_exit_edges ei(region); ei!=0; ei++) 
 {
   exit_op = (*ei)->src();
   info = opInfoMap->value(exit_op);
   int exit_home_block = info->get_home_block();
   Control_flow_freq *cfreq = get_control_flow_freq(*ei);
   if(cfreq) {
     total_flow_freq += cfreq->freq;
     exit_freqs[exit_home_block] += cfreq->freq;
   } else {
     cdbg << "\nWARNING: there's no control flow frequency on this edge, meaning\n"
	  << "I have no profile data, and the output statistics will be wrong!\n\n"
	  << **ei << endl;
   }
 }

 // account for an RTS (which doesn't have an exit edge!)
 for (Region_exit_ops exit_ops(region); exit_ops != 0; exit_ops++) {
   if (is_return(*exit_ops))
   {
     Op* last_exit_op = *exit_ops;
     info = opInfoMap->value(last_exit_op);
     int exit_home_block = info->get_home_block();
     exit_freqs[exit_home_block] = region->weight - total_flow_freq;
     total_flow_freq = region->weight; // total_flow_freq does not account for
                                       // an RTS exit.
   }
 }

 // put actual freq percentages in exit_freqs Vector.
 // if particular exit has zero freq, assign it ELCOR_MIN_DOUBLE freq (not 0),
 // so that ops higher up in dep chain to this exit have higher priority
 if (total_flow_freq != 0.0) // profile information is present.
 {
   for (int k = 0; k < num_of_exits; k++)
     if (exit_freqs[k] == 0.0) {
       exit_freqs[k] = ELCOR_MIN_DOUBLE;
     } else {
       exit_freqs[k] = exit_freqs[k] / total_flow_freq;
     }
 }
 else // no profile info available, so assume the last branch (dummy or
      // unconditional) has a probability of 1.
 {
   for (int k = 0; k < (num_of_exits - 1); k++)
     exit_freqs[k] = ELCOR_MIN_DOUBLE;
   exit_freqs[num_of_exits - 1] = 1;
 }

 // Priority calculation using Chandra's heuristic
 // Calculates for each exit i, prob i/ weight i where weight i is number of
 // ops reachable from exit i backwards through dependence edges
 // Starting from exit with largest prob i / weight i,
 // put all reachable ops from that exit in height order into priority list
 // Repeat for other exits
 if (El_ssched_op_priority_model == CHANDRAS) {
  Vector<int> op_cnt_of_br (num_of_exits, 0); // weight of each exit
  // iterate over ops in block
  for (Region_ops_C0_order oiter (region); oiter != 0; oiter++) {
     Op* op = (*oiter);
     info = opInfoMap->value(op);
     info->set_visited(false); 		// initialize visited to false
     // iterate over all possible late times of op
     for (int i = 0; i < num_of_exits; i++) {
       if (info->get_ltime(i) != -1)	// if ltime is -1,
       					// then op is not reachable from exit i
       	op_cnt_of_br[i] = op_cnt_of_br[i] + 1;	// increment number of ops
					 	// reachable from exit i
     }
  }
  
  Slist <Stuple < double, Op*> > pr_br_list; // sorted list of branches
  // iterate over exits of region
  for (Region_exit_ops exit(region); exit != 0; exit++) {
    Op* br = (*exit);
    info = opInfoMap->value(br);
    int home = info->get_home_block(); // home block of op
    	// starting from 0 for entry op through num_of_exits -1 for fall thru
    double pr = - (exit_freqs[home]/op_cnt_of_br[home]);
    	// priority is branch exit probability over weight
	// (number of reachable ops)
    Stuple <double, Op* > pr_br (pr, br);
    pr_br_list.add(pr_br);	// add to increasing order sorted list
    assert (info->get_etime() < MAX_SCHED_LEN);
  }
  int br_num = 0;
  // iterate over branches starting with highest priority
  for (Slist_iterator<Stuple < double, Op* > > siter(pr_br_list);
  		siter != 0; siter++) {
    Op* br = (*siter).second;
    info = opInfoMap->value(br);
    int home = info->get_home_block();
    // iterate over ops in region/block
    for (Region_ops_C0_order oiter(region); oiter != 0; oiter++) {
      info = opInfoMap->value(*oiter);
      // if op isn't reachable from any of the previous exits
      // and is reachable from this exit
      if (!info->is_visited() && info->get_ltime(home) != -1) {
        // priority is some large number (to give previous exits' reachable ops
	// higher priority - height of this op wrt this exit
        double pr = (MAX_SCHED_LEN*br_num) -
			((maxtime + 1) - info->get_ltime(home));
        info->set_priority(pr);		// set the priority
	info->set_visited(true);	// set visited so that we know that
					// this op is reachable from some exit
      }
    }
    br_num++;	// increment br_num so that next exit's ops have lower priority
  }
  // iterate over ops in block once more
  for (Region_ops_C0_order piter(region); piter != 0; piter++) {
    info = opInfoMap->value(*piter);
    if  (!info->is_visited()) { // op is not reachable from any exit
      // make sure that it is unreachable by ensuring that all ltimes are -1
      for (int i=0; i < num_of_exits; i++)
        assert(info->get_ltime(i) == -1);
      double pr = (MAX_SCHED_LEN*br_num); // set priority to a very large value
      info->set_priority(pr);
    }
    else {
    info->set_visited(false);	// set visited to false just in case it is used
    				// later without initialization
    }
  }
 }
 else if (El_ssched_op_priority_model == REGULAR) 
 { // El_ssched_op_priority_model == REGULAR
 // calculate priorities for each oper in region.
   for (Region_ops_C0_order iter3(region); iter3 != 0; ++iter3) {
     info = opInfoMap->value(*iter3);
     double priority = 0;
     for (int m = 0; m < num_of_exits; m++) {
       if (info->get_ltime(m) != -1)
	 {
	   // maxtime corresponds to the max etime (and max ltime) present
	   //   in the region.  the more negative the priority, the higher the
	   //   priority.  maxtime + 1 is used to allow the frequency function
	   //   to be taken into account.  if maxtime was used, the final
	   //   operation in a region along a 100% taken path might get
	   //   scheduled after ops in a 0% taken path.
	   priority += exit_freqs[m] * (info->get_ltime(m) - (maxtime + 1));
	 }
     }
     info->set_priority(priority);
   }
 } else {
   assert(El_ssched_op_priority_model == CRITICAL_PATH);
   for (Region_ops_C0_order oiter(region); oiter != 0; oiter++) {
     info = opInfoMap->value(*oiter);
     info->set_priority(-(info->get_ltime(num_of_exits-1)));
   }
 }
 
// commented out since PRED_CLEAR and PRED_SET are now real ops. KF 8-2001

//  // Set PRED_CLEAR ops to highest priority so that
//  // they are scheduled before any other operation
//  double max_priority = - (ELCOR_MAX_DOUBLE);
//  for (Region_ops_C0_order iter4(region); iter4 != 0; ++iter4)
//  {
//    info = opInfoMap->value(*iter4);
//    if ((*iter4)->opcode() == PRED_CLEAR) info->set_priority(max_priority);
//    else if ((*iter4)->opcode() == PRED_SET) info->set_priority(max_priority);
//  }
}




//  Bind opInfoMap entries and set in opInfo
//	 home_blocks, etimes, set_succ, set_visited
void calculate_earliest_times(Compound_region* region, const Relax_dep_constr* relax_dep,
		const Op_filter* ofilter, const Edge_filter* einfilter)
{
 List<Op*> opers;
 OpInfo* info;
 Hash_map<Op*, int> early_map (hash_op_ptr, 211);

 /* Initialize the search process and determine home blocks of ops in region */
 int num_exits = 0;
 for(Region_ops_C0_order iter(region); iter != 0; ++iter) 
 {
   if (dbg(ss, 6)) assert (!opInfoMap->is_bound(*iter));
   opInfoMap->bind(*iter, new OpInfo(*iter));
   if (is_branch(*iter)) num_exits++;
 }
 
 int exits_above = 0;
 for(Region_ops_C0_order biter(region); biter != 0; ++biter)
 {
    info = opInfoMap->value(*biter);
    info->init_ltimes(num_exits);
    info->set_home_block(exits_above);
    if (is_branch(*biter)) exits_above++;
  }

  if (dbg(ss, 6)) check_input (region);

 // determine early times.
 Region_entry_ops entry_op(region);

 calculate_earliest_times_from_entry_op(region, *entry_op, early_map, 
                                        einfilter);
 entry_op++;
 if (entry_op != 0)
   El_punt("More than one entry op found in region.");

 // set calculated early times into opInfo data structure.
 maxtime = -1;
 for(Region_ops_C0_order diter(region); diter != 0; ++diter)
 { // ********* make sure set_succ and set_visited are set here when
   // implementing this section of the code. **************
   assert((!(is_merge(*diter))) && (!(is_switch(*diter))));
   assert (opInfoMap->is_bound(*diter));
   info = opInfoMap->value(*diter);
   info->set_visited (true);
   info->set_succ(0);
   if (early_map.is_bound(*diter)) // set early time to one calculated.
   {
     info->set_etime(early_map.value(*diter));
     if (maxtime < early_map.value(*diter))
       maxtime = early_map.value(*diter);
   }
   else // no early time calculated, so set early time to -1.
     info->set_etime(-1);
 }
}

/* calculate the latest times of ops from one exit op */

void calculate_latest_times_from_exit_op(Compound_region* region, Op *exit_op, 
                Hash_map<Op*, int>& late_map,
                const Edge_filter *efilter)
{
 Op *current_op, *dest_op;
 int new_late_time, min_late_time;
 
 if (dbg(ss, 6)) {
   if (!is_control_switch(exit_op))
     El_punt("Found a non-control switch exit op");
   if (opInfoMap->value(exit_op)->get_etime() == -1)
     El_punt("exit op's early time was not defined");
 }

 // Set exit op late time to its early time.
 late_map.bind(exit_op, opInfoMap->value(exit_op)->get_etime());

 for (Region_ops_reverse_C0_order iter(region, exit_op); iter != 0; ++iter)
 {
   current_op = *iter;
   if (current_op == exit_op) continue;
   min_late_time = ELCOR_MAX_INT;
   for (Op_outedges_rel ei(current_op, efilter); ei != 0; ei++)
   {
     dest_op = real_op((*ei)->dest());
     if (late_map.is_bound(dest_op)) // only continue if ltime present for dst
     {
       new_late_time = late_map.value(dest_op) - (*ei)->latency();
       if (new_late_time < min_late_time) 
	 min_late_time = new_late_time;
     }
   }
   
   if (min_late_time < ELCOR_MAX_INT)
       late_map.bind(current_op, min_late_time);
 }
}


/* calculate the earliest and latest times of all ops in "region" */

void calculate_earliest_and_latest_times(Compound_region* region, 
                const Relax_dep_constr* relax_dep, const Op_filter* ofilter, 
                const Edge_filter* einfilter, const Edge_filter* eoutfilter)
{
 int num_of_exits;
 List<Op*> opers;
 OpInfo* info;
 Hash_map<Op*, int> late_map (hash_op_ptr, 211);

 calculate_earliest_times(region, relax_dep, ofilter, einfilter);

 // determine late times for all the exit ops and put it into ltimes Vector.
 for (Region_exit_ops exit_ops1(region); exit_ops1 != 0; exit_ops1++)
 {
   info = opInfoMap->value(*exit_ops1);
   late_map.clear();
   calculate_latest_times_from_exit_op(region, *exit_ops1, late_map,
                                       einfilter);

   for(Region_ops_C0_order iter1(region); iter1 != 0; ++iter1)
   {
      info = opInfoMap->value(*iter1);
      if (late_map.is_bound(*iter1)) // set early time to one calculated.
        info->set_ltime(opInfoMap->value(*exit_ops1)->get_home_block(), 
                        late_map.value(*iter1));
   }
 }

#if 0
   for(Region_ops_C0_order iter2(region); iter2 != 0; ++iter2)
   {
      info = opInfoMap->value(*iter2);
      if (late_map.is_bound(*iter2)) {
	cout << "Op = " << (*iter2)->id() 
	     << "S = " << info->get_etime() << "  L = ";
	int numl = info->get_num_ltimes();
	for(int k = 0; k<numl; k++){
	  cout << info->get_ltime(k) << " " ;
	}
	cout << endl << flush;
      }
   }
#endif 


 Region_exit_ops exit_ops3(region);
 num_of_exits = opInfoMap->value(*exit_ops3)->get_num_ltimes();

 for (Region_ops_C0_order iter1(region); iter1 != 0; ++iter1)
 {
   info = opInfoMap->value(*iter1);
   info->set_ltime(ELCOR_MAX_INT);
   for (int iiter = 0; iiter < num_of_exits; iiter++)
     if ((info->get_ltime(iiter) != -1)
     		&& (info->get_ltime(iiter) < info->get_ltime()))
       info->set_ltime(info->get_ltime(iiter));
 }

 // Check that the ltime is at least the etime for all operations
 if (dbg(ss, 6)) {
   for(List_iterator<Op*> citer(opers); citer != 0; ++citer) {
     assert (opInfoMap->is_bound(*citer));
     info = opInfoMap->value(*citer);
     if (info->get_etime() != -1)
       assert(info->get_ltime() >= info->get_etime());
   }
 }

 set_priority (region);
}

/* calculate the slack for all ops in "region" */

void calculate_slack(Compound_region* region,
	const Relax_dep_constr* relax_dep, const Op_filter* ofilter,
	const Edge_filter* einfilter, const Edge_filter* eoutfilter)
{
#if 0 // ** can this be deleted?
  calculate_earliest_times(region, relax_dep, ofilter, einfilter);
#endif
  calculate_earliest_and_latest_times(region, relax_dep, ofilter, einfilter, 
                                      eoutfilter);

  for(Region_ops_C0_order iter(region); iter != 0; ++iter)
   opInfoMap->value(*iter)->set_slack(opInfoMap->value(*iter)->get_ltime() -
			    opInfoMap->value(*iter)->get_etime()) ;
}

/*  Find the control merge op for the concerned region */

Op* find_merge_op(Compound_region* region)
{
  for(Region_ops_C0_order iter(region); iter != 0; ++iter)
      if (is_control_merge(*iter)) return(*iter);
  return(NULL);
}

/* Find the control switch op for the concerned region */

Op* find_switch_op(Compound_region* region)
{
  for(Region_ops_C0_order iter(region); iter != 0; ++iter)
      if (is_control_switch(*iter)) return(*iter);
  return(NULL);
}

/* Gets the next op to be scheduled among a list of ready op's depending on 
   the Motion_type. 
*/
   
Op* get_next_ready_node(List<Op*>& op_list, const Op_filter* filter,
							Motion_type type)
{
  int curr_max = -1;
  int curr_min = ELCOR_MAX_INT;
  Op* temp_op = NULL;

  switch (type) 
  {
   case MT_LIST:
   { /* get the next node with the maximum breadth (successors count) */
     for(List_filterator<Op*> iter1(op_list, filter); iter1 != 0; ++iter1)
     {
       if(opInfoMap->is_bound(*iter1))
         if((opInfoMap->value(*iter1)->get_succ()) > curr_max) 
         {
           temp_op = *iter1;
	   curr_max = opInfoMap->value(*iter1)->get_succ();
         }
     }
     break;
   }
   case MT_SLACK:
   { /* get the next node with the lowest slack value */
     for(List_filterator<Op*> iter2(op_list, filter); iter2 != 0; ++iter2)
     {
       if(opInfoMap->is_bound(*iter2))
         if((opInfoMap->value(*iter2)->get_slack()) < curr_min) {
           temp_op = *iter2;
	   curr_min = opInfoMap->value(*iter2)->get_slack();
         }
     }
     break;
   }
   default:
   {
    /* a naive walk thru the list of ready op's, no prioritization employed */
     for(List_filterator<Op*> iter3(op_list, filter); iter3 != 0; ++iter3)
      {  temp_op = *iter3;  break;  }
   }
  }
 return temp_op;
}

/* Adds a list of nodes which have become ready after the "nextop" has been
   scheduled. The list of ready ops is maintained as a continous window, with
   ops being ready, added to the window and the ops considered for scheduling,
   removed from the window.
*/

/*
	add_ready_nodes not required in List scheduler
	needs to be upgraded to handle switch and merge nodes
	when it is needed for operation scheduling - SGA 6/95
*/
List<Op*>& add_ready_nodes(Op* nextop, List<Op*>& oplist, 
                           Compound_region* parent)
{
  Op* dest;
  Op* src;
  bool cond;

  for(Op_outedges_rel iter(nextop); iter != 0; ++iter)
  {
    dest = real_op((*iter)->dest());
    cond = true;

    assert(opInfoMap->is_bound(dest));
    OpInfo* info = opInfoMap->value(dest);
    /* if "dest" has not been prior scheduled and is in the same region as
       "nextop", consider the possibility of it being ready. The matching_
       parents function is used to limit the scope of ready_ops to within
       the home region. In a true sense, it need to be extended to accomodate 
       ready ops across regions as well.
    */
    if((!info->is_scheduled()) && matching_parents(dest, parent))
    {
        for(Op_inedges_rel in(dest); in != 0; ++in)
        {
          src = real_op((*in)->src());
          if(oplist.is_member(src)) { cond = false; break; }
        }
    }
    else cond = false;
    if(cond) 
       oplist.add_head( dest);
  }
 return(oplist);
}

/* returns true/false depending on if "dest" is in the same home region
   as "parent" or not.
*/

bool matching_parents(Op* dest, Compound_region* parent)
{
  Region* curr = dest->parent();
  while(curr != 0 && curr != parent)
    curr = curr->parent();
  if(curr == parent) return(true); 
 return(false);
}
