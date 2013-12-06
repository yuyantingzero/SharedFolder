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
//      File:           meld_check.cpp
//      Authors:        Santosh G. Abraham
//      Created:        July 1995
//      Description:    Checks whether meld scheduling violated constraints
//
/////////////////////////////////////////////////////////////////////////////

#include "meld_check.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "region_utilities.h"
#include "attributes.h"
#include "edge_utilities.h"
#include "mdes.h"
#include "meld_constraints.h"
#include "el_ssched_init.h"
#include "edge.h"
#include "dbg.h"
#include "sched_functions.h"
#include "dfa.h"
#include "intf.h"
#include "core_edge_drawing.h"

// check that the transfer of import_def_map and import_use_map
// to edge latencies at the entry are done consistently
// Assumes that the only merges created are for melding entry constraints,
//   so, for instance, assumes that only liveness analysis is done.
void check_entry_edge_lats(Compound_region* region_sched, Op* entry_sched,
	Latency_map* msc_entry_def_map, Latency_map* msc_entry_use_map) {
	
 // Copy the main MS_constraints import maps to a temporary map
 // because the check destroys the maps and the original maps are needed
 // if we redraw edges
 Latency_map entry_def_map(*msc_entry_def_map);
 Latency_map entry_use_map(*msc_entry_use_map);

// iterate over the merge ops at entry of region
 for (List_iterator<Op*> op_iter(entry_sched->get_relops());
 	op_iter != 0; op_iter++) {
   Port_type type;
   int map_lat = 0, edge_lat = 0;
   assert (is_merge(*op_iter));
   if (!((*op_iter)->src(SRC1).is_undefined())) {
     assert ((*op_iter)->dest(DEST1).is_undefined());
     type = SRC;
   } else {
     assert (!((*op_iter)->dest(DEST1).is_undefined()));
     type = DEST;
   }

   // latency > 0 <=> appear in the use map
   // Assumes that early sample/write times are 0.
   // remove from use/def map, to eventually make sure that all entries
   // in use map have corresponding merge nodes and associated edges
   if (type == SRC) {
     Operand opnd = (*op_iter)->src(SRC1);
     assert(opnd.is_reg() || opnd.is_macro_reg());
		// Don't want to meld mem_vr's anymore  || opnd.is_mem_vr());
     assert(entry_use_map.is_bound(opnd));
     map_lat = entry_use_map.value(opnd);
     assert(map_lat > 0); // woudn't have inserted merge if no incoming meld
     entry_use_map.unbind(opnd);
   }
   if (type == DEST) {
     Operand opnd = (*op_iter)->dest(DEST1);
     assert(opnd.is_reg() || opnd.is_macro_reg());
    						//  || opnd.is_mem_vr());
     assert(entry_def_map.is_bound(opnd));
     map_lat = entry_def_map.value(opnd);
     assert (map_lat > 0);
     entry_def_map.unbind(opnd);
   }
   if (!((type == SRC) || (type == DEST))) {
     cdbg << "Punt in check_entry_edge_lats\n" << *region_sched;
     cdbg << " Entry constraints for region being melded are: " << endl;
     cdbg << "\nEntry def meld constraints:\n" << *msc_entry_def_map << endl;
     cdbg << "\nEntry use meld constraints:\n" <<* msc_entry_use_map << endl;
     assert(0);
   }

   Op_outedges ped (*op_iter);
   assert (ped != 0);
   // All outgoing edges from merge must have same latencies, port_type, etc
   // assuming early sample times are all zero, except for switches
   // for which they are either 1 or BRANCH_LATENCY
   while (ped !=0) {
     assert(region_contains_op(region_sched, (*ped)->dest()));
     edge_lat = (*ped)->latency();
     if (is_switch((*ped)->dest())) {
       Op* real = real_op ((*ped)->dest());
       if (is_dummy_branch(real))
	 assert (edge_lat == ELCOR_MAX (0, map_lat - 1));
       else
	 if (edge_lat != ELCOR_MAX (0, map_lat - MDES_branch_latency())) {
	   cerr << "Merge op is:\n" << (*(*op_iter)) << endl;
	   cerr << "Map lat: " << map_lat <<
	     "\tEdge lat: " << edge_lat << endl;
	   cerr << "Br lat: " << MDES_branch_latency() << endl;
	   cerr << "Region is:\n" << (*(real->parent()));
	   assert (edge_lat == ELCOR_MAX (0, map_lat - MDES_branch_latency()));
	 }
     } else {
       assert (edge_lat == map_lat);
     }
     assert (type == (*ped)->src_port_type());
     if (type == SRC) assert ((*ped)->src_port() == SRC1);
     else assert ((*ped)->src_port() == DEST1);
     ped++;
   }
 }
 // Since there must be a one-to-one constraint between a merge node
 // and an incoming meld constraint, and since for each merge node
 // a constraint is removed, constraint maps must be empty after all
 // merge nodes are traversed, except for non-local operands
 //  which don't have any pass-through constraints
 VR_map* vr_map = get_local_vr_map (region_sched);
 Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;
 MS_constraints* msc = get_ms_constraints(region_sched);
 for (Latency_map_iterator di (entry_def_map); di != 0; di++) {
   Operand oper = (*di).first;
   int dangle = (*di).second;
   assert (!vr_to_index.is_bound(oper));
   bool pass_thru_constraint = false;
   Latency_map* import_exit_def_map = NULL;
   Latency_map* import_exit_use_map = NULL;
   for (Region_exit_ops xi (region_sched); xi != 0; xi++) {
     Op* exit = (*xi);
     import_exit_def_map = msc->import_def_at_exit(exit);
     if ( import_exit_def_map->is_bound(oper) &&
	 (dangle > import_exit_def_map->value(oper) + MDES_branch_latency())) {
	pass_thru_constraint = true;
        break;
     }
     import_exit_use_map = msc->import_use_at_exit(exit);
     if ( import_exit_use_map->is_bound(oper) &&
	 (dangle > import_exit_def_map->value(oper) + MDES_branch_latency())) {
	pass_thru_constraint = true;
        break;
     }
   }
   if (pass_thru_constraint) {
     cerr << "	In check_entry_edge_lats:\n";
     cerr << "	Need merge and merge-switch edge\n";
     cerr << "		 for following oper from import_entry_def_map:\n";
     cerr << "Operand:\n" << oper;
     cerr << "Entry_def_map:\n" << entry_def_map;
     cerr << "Exit_maps:\n" << (*import_exit_def_map) <<
		 (*import_exit_use_map);
     assert (!pass_thru_constraint);
   }
 }
 for (Latency_map_iterator ui (entry_use_map); ui != 0; ui++) {
   Operand oper = (*ui).first;
   int dangle = (*ui).second;
   assert (!vr_to_index.is_bound(oper));
   bool pass_thru_constraint = false;
   Latency_map* import_exit_def_map = NULL;
   for (Region_exit_ops xi (region_sched); xi != 0; xi++) {
     Op* exit = (*xi);
     import_exit_def_map = msc->import_def_at_exit(exit);
     if ( import_exit_def_map->is_bound(oper) &&
	 (dangle > import_exit_def_map->value(oper) + MDES_branch_latency())) {
	pass_thru_constraint = true;
        break;
     }
   }
   if (pass_thru_constraint) {
     cerr << "	In check_entry_edge_lats:\n";
     cerr << "	Need merge and merge-switch edge\n";
     cerr << "		 for following oper from import_entry_use_map:\n";
     cerr << "Operand:\n" << oper;
     cerr << "Entry_use_map:\n" << entry_use_map;
     cerr << "Import exit def map:\n" << (*import_exit_def_map);
     assert (!pass_thru_constraint);
   }
 }
}

// Checks that the incoming edge latencies into switches associated with
// the exit op are consistent with the import_*_map's at the exit
// This routine destroys the import_*_map's but that should be fine
// because these maps are not required any more.
void check_exit_edge_lats(Compound_region* region_sched, Op* exit_sched,
	Latency_map* exit_def_map, Latency_map* exit_use_map) {

 Hash_set<Operand> reg_use_opers(hash_operand, 128),
    reg_def_opers(hash_operand, 128);
 Operand oper;

// gather operands defined or used prior to exit in reg_*_opers,
 for (Region_ops_reverse_C0_order oiter(region_sched, exit_sched);
 		oiter !=0; oiter++) {
   Op* op = (*oiter);
   for (Op_all_dests oi(op); oi != 0; oi++) {
     oper = (*oi);
     if (!oper.is_undefined()) reg_def_opers += oper;
   }      
   for (Op_all_inputs ii(op); ii != 0; ii++) {
     oper = (*ii);
     if (!oper.is_undefined()) reg_use_opers += oper;
   } 
 }

 int map_lat;
// iterate over related switch ops of exit
 for (List_iterator<Op*> op_iter(exit_sched->get_relops());
 	op_iter != 0; op_iter++) {
   Op* op = (*op_iter);
   assert (is_switch(op));
   // no merges on returns any more
	// || (is_return(exit_sched) && (is_merge(op))));
	//   if (is_merge(op)) continue;
   // handle switches with real operands in SRC1 field
   oper = (*op_iter)->src(SRC1);
   if (!oper.is_undefined()) {
     map_lat = exit_use_map->value(oper);
   
     // iterate over incoming edges of switch  
     Edge_style es = ES_MELD;
     for (Op_inedges ped (*op_iter, SRC, SRC1); ped != 0; ped++) {
       int curr_lat = (*ped)->latency();
       // don't change merge-switch latencies for checking because they may
       // have dangles coming from entry too
       if (!is_merge((*ped)->src())) {
	 set_edge_latency ( (*ped), es);
         int adj_lat = (*ped)->latency();
         // if the curr latency is 0,
         // then orig_lat is 0, not in map or orig_lat was smaller than dangle
	 if (curr_lat == 0) {
	   assert ((adj_lat == 0) || (adj_lat <= map_lat));
	 }
         // if the curr latency is not 0,
         // then there were constraints coming through from exit
	 else {
	   assert(curr_lat == adj_lat - map_lat);
         }
         (*ped)->set_latency(curr_lat);
	 if (exit_use_map->is_bound(oper)) exit_use_map->unbind(oper);
       }
     }
   }

   oper = (*op_iter)->dest(DEST1);
   if (!oper.is_undefined()) {
     map_lat = exit_def_map->value(oper);
     Edge_style es = ES_MELD;
     for (Op_inedges ped (*op_iter, DEST, DEST1); ped != 0; ped++) {
       int curr_lat = (*ped)->latency();
       // leave merge-switch edges alone; cannot reconstruct dangle into merge
       if (!is_merge((*ped)->src())) {
	 set_edge_latency( (*ped), es);
         int adj_lat = (*ped)->latency();
         if (curr_lat == 0) {
           assert ((adj_lat == 0) || (adj_lat <= map_lat));
         }
         else {
           if (curr_lat != (adj_lat -map_lat)) {
             cerr << "	In check_exit_edge_lats switch being asserted on is:\n";
             cerr << (*(*op_iter)) << " with dangle of " << map_lat << endl;
             cerr << "Current latency is " << curr_lat << endl;
             cerr << "Adjusted latency is " << adj_lat << endl;
             cerr << "Region is:\n" << (*(*op_iter)->parent()) << endl;
             assert (curr_lat == (adj_lat - map_lat));
           }
         }
         (*ped)->set_latency(curr_lat);
       }
     }
     for (Op_outedges ded(*op_iter, DEST, DEST1); ded != 0 ; ded++) {
       assert ((*ded)->latency() == MDES_branch_latency());
     }
   }
 }
}

bool check_set_lat_maps(Compound_region* region_sched)
{
 Map<Operand, Op*> use_mg_map, def_mg_map, use_sw_map, def_sw_map;

 if (El_ssched_meld_model != FULL_MELD) return true;
 Region_entry_ops eop(region_sched);
 Op* entry = (*eop);
 // gather def-merges and use-merges into def_mg_map and use_mg_map
 for (List_iterator<Op*> oiter(entry->get_relops());
		oiter != 0; oiter++) {
   int num_opers = 0;
   Operand oper = (*oiter)->src(SRC1);
   if (!oper.is_undefined()) {
     use_mg_map.bind(oper, (*oiter)); num_opers++;
   }
   oper = (*oiter)->dest(DEST1);
   if (!oper.is_undefined()) {
     def_mg_map.bind(oper, (*oiter)); num_opers++;
     //     cerr << "Def merge operand: " << oper << endl;
   }
   assert ( (num_opers == 1) ||
	   ((num_opers == 0) && (*oiter)->num_mem_srcs() > 0));
 }
 
 MS_constraints* msc = get_ms_constraints(region_sched);
 Latency_map* entry_def_map = msc->def_at_entry(entry);
 Latency_map* entry_use_map = msc->use_at_entry(entry);
 
 int default_val = entry_def_map->get_default_value();
 assert (default_val == entry_use_map->get_default_value());

// Disable checking for entry block

 Region_entry_edges ei (region_sched);
 if (ei != 0) {

 // iterate over entry_def_map, 
 for (Latency_map_iterator liter(*entry_def_map); liter != 0; liter++) {
   Operand oper = (*liter).first;
   int lat_in_map = (*liter).second;
   if (!def_mg_map.is_bound(oper)) continue;
   Op* def_mg = def_mg_map.value(oper);
   int min_out_lat = ELCOR_MAX_INT;
   Hash_set<Quad<Op*, Port_type, Port_num, Operand> > refs_to_oper;
   find_refs_to_oper(def_mg, def_mg->first_dest(), oper, &refs_to_oper);
   for (Hash_set_iterator<Quad<Op*, Port_type, Port_num, Operand> >
		 fiter(refs_to_oper); fiter !=0; fiter++) {
     Quad<Op*, Port_type, Port_num, Operand> ref = (*fiter);
     Op* dest_op = ref.first;
     Port_type port_type = ref.second;
     Port_num port_num = ref.third;
     if (is_switch(dest_op)) continue;
     if (port_type == DEST) {
       int oper_time = (real_op(dest_op))->sched_time() +
       		dest_op->anti_time(DEST, port_num);
       min_out_lat = ELCOR_MIN(min_out_lat, oper_time);
     }
   }
   assert(min_out_lat == lat_in_map);
 }

 for (Latency_map_iterator kiter(*entry_use_map); kiter != 0; kiter++) {
   Operand oper = (*kiter).first;
   int lat_in_map = (*kiter).second;
   if (!def_mg_map.is_bound(oper)) continue;
   Op* def_mg = def_mg_map.value(oper);
   int min_out_lat = ELCOR_MAX_INT;
   Hash_set<Quad<Op*, Port_type, Port_num, Operand> > refs_to_oper;
   find_refs_to_oper(def_mg, def_mg->first_dest(), oper, &refs_to_oper);
   for (Hash_set_iterator<Quad<Op*, Port_type, Port_num, Operand> >
		 fiter(refs_to_oper); fiter !=0; fiter++) {
     Quad<Op*, Port_type, Port_num, Operand> ref = (*fiter);
     Op* dest_op = ref.first;
     Port_type port_type = ref.second;
     Port_num port_num = ref.third;
     if (is_switch(dest_op)) continue;
     if (port_type == SRC) {
       int oper_time = (real_op(dest_op))->sched_time() +
       		dest_op->flow_time(SRC, port_num);
       min_out_lat = ELCOR_MIN(min_out_lat, oper_time);
     }
   }
   assert(min_out_lat == lat_in_map); /******/
 }
  
 for (Map_iterator<Operand, Op*> niter(def_mg_map);
 		niter != 0; niter++) {
   Operand oper = (*niter).first;
   Op* def_mg = (*niter).second;
   int min_out_lat = ELCOR_MAX_INT;
   int min_flow_lat = ELCOR_MAX_INT;
   Hash_set<Quad<Op*, Port_type, Port_num, Operand> > refs_to_oper;
   find_refs_to_oper(def_mg, def_mg->first_dest(), oper, &refs_to_oper);
   for (Hash_set_iterator<Quad<Op*, Port_type, Port_num, Operand> >
		 fiter(refs_to_oper); fiter !=0; fiter++) {
     Quad<Op*, Port_type, Port_num, Operand> ref = (*fiter);
     Op* dest_op = ref.first;
     Port_type port_type = ref.second;
     Port_num port_num = ref.third;
     if (is_switch(dest_op) && (!is_call(real_op(dest_op)))
     			  && (!is_return(real_op(dest_op)))) continue;   
     if (port_type == DEST) {
       int oper_time = (real_op(dest_op))->sched_time() +
       		dest_op->anti_time(DEST, port_num);
       min_out_lat = ELCOR_MIN(min_out_lat, oper_time);
     }
     if (port_type == SRC) {
       int oper_time = (real_op(dest_op))->sched_time() +
       		dest_op->flow_time(SRC, port_num);
       min_flow_lat = ELCOR_MIN(min_flow_lat, oper_time);
     }
   }

   if ((min_out_lat < entry_def_map->get_default_value())) {
   //   	&& !oper.is_macro_reg()) {
   // need to handle special case of "implied" write of mac $p15 that occurs
   //   for certain jsr's.
     assert (entry_def_map->is_bound(oper));
     assert (entry_def_map->value(oper) == min_out_lat);
   }

   if (min_flow_lat < entry_use_map->get_default_value()) {
     assert (entry_use_map->is_bound(oper));
//     int compare_value = entry_use_map->value(oper);
//     if (compare_value != min_out_lat) cout << "REGION $$$$:" << endl << *region_sched << endl;
     assert (entry_use_map->value(oper) == min_flow_lat);
   }
 }

 } // end entry edge checking
 
 for (Region_exit_ops eiter (region_sched); eiter != 0; eiter++) {
   Op* exit = (*eiter);
   def_sw_map.clear();
   use_sw_map.clear();
   for (List_iterator<Op*> oiter(exit->get_relops());
		oiter != 0; oiter++) {
     int opers = 0;
     Operand oper = (*oiter)->src(SRC1);
     if (!oper.is_undefined()) {
       use_sw_map.bind(oper, (*oiter)); opers++;
     }
     oper = (*oiter)->dest(DEST1);
     if (!oper.is_undefined()) {
       def_sw_map.bind(oper, (*oiter)); opers++;
     }
     assert ( (opers == 1) ||
	     ((opers == 0) && ((*oiter)->num_mem_srcs() > 0)));
   }
 
   Latency_map* exit_def_map = msc->def_at_exit(exit);
   Latency_map* exit_use_map = msc->use_at_exit(exit);
   int exit_time = exit->sched_time() + MDES_branch_latency();
/* exit def map construction looks at all operations before exit
    even those without outgoing edges into a switch node;
    currently, no op-sw edges for use dangles always
    cannot assert that def_map is a subset of usw switches
   for (Latency_map_iterator liter(*exit_def_map); liter != 0; liter++) {
     Operand oper = (*liter).first;
     assert(use_sw_map.is_bound(oper));
   }
*/
   for (Map_iterator<Operand, Op*> miter(use_sw_map);
 		miter != 0; miter++) {
     Operand oper = (*miter).first;
     Op* use_sw = (*miter).second;
     int max_flow_time = 0;
     for (Op_inedges ed(use_sw); ed != 0; ed++) {
       if (is_CONTROL0(*ed)) continue;
       Op* src = (*ed)->src();
       if (is_merge(src)) continue;
       int oper_time = src->sched_time() 
       		+ src->flow_time(DEST, (*ed)->src_port());
       max_flow_time = ELCOR_MAX(max_flow_time, oper_time);
     }
     int dangle = max_flow_time - exit_time;
     if (dangle > 0) {
       assert(exit_def_map->is_bound(oper));
       int lat_in_map = exit_def_map->value(oper);
       assert(dangle <= lat_in_map);
     }
   }

   Map<Operand, int> oper_ltime_map;
   for (Region_ops_reverse_C0_order piter(region_sched);
   			piter != 0; piter++) {
     Op* op = (*piter);
     if (op->sched_time() >= exit_time) continue;
// switch/merge ops of calls/returns may need to be handled too
     for (Op_all_inputs ii (op); ii != 0; ii++) {
       Operand oper = (*ii);
       Port_num src_p = ii.get_port_num();
       int oper_time = op->sched_time() + op->anti_time(SRC, src_p);
       if (oper_ltime_map.is_bound(oper)) {
         if (oper_ltime_map.value(oper) < oper_time)
	   oper_ltime_map.bind(oper, oper_time);
       }
       else oper_ltime_map.bind(oper, oper_time);
     }
   }

   for (Map_iterator<Operand, int> kiter (*exit_use_map);
   	kiter != 0; kiter++) {
     Operand oper = (*kiter).first;
     assert(oper_ltime_map.is_bound(oper));
   }
   for (Map_iterator<Operand, int> diter(oper_ltime_map);
 		diter != 0; diter++) {
     Operand oper = (*diter).first;
     int dangle_out = (*diter).second - exit_time;
     if (dangle_out > 0) {
       assert(exit_use_map->is_bound(oper));
       assert(exit_use_map->value(oper) == dangle_out);
     }
   }
 }
 return true;
}

bool are_maps_compatible(Latency_map* exit_map, Latency_map* entry_map,
		int& max_dangle_req)
{
 for (Latency_map_iterator miter(*exit_map); miter != 0; miter++) {
  Operand oper = (*miter).first;
  int dangle_req = (*miter).second;
  assert (dangle_req > 0);
  max_dangle_req = ELCOR_MAX(max_dangle_req, dangle_req);
  if (dbg(ss, 5)) {
    assert (dangle_req <= entry_map->value(oper));
  } else {
    if (dangle_req > entry_map->value(oper)) return false;
  }
 }
 return true;
}

void build_lat_map(Latency_map* old_map, Latency_map* new_map,
		int dangle_reduc)
{
 for (Latency_map_iterator miter(*old_map); miter != 0; miter++) {
   Operand oper = (*miter).first;
   int old_dangle = (*miter).second;
   int new_dangle = old_dangle - dangle_reduc;
   if (new_dangle > 0) new_map->bind(oper, new_dangle);
 }
}

bool check_map_propagation (Compound_region* sched_rgn, Op* entry,
	Latency_map* pred_rgn_def_map, Latency_map* pred_rgn_use_map) 
{
  Latency_map* exit_def_map;
  Latency_map* exit_use_map;

  assert(sched_rgn->flag(EL_REGION_SCHEDULED));
  int pred_max_dangle = 0;
  MS_constraints* msc = get_ms_constraints(sched_rgn);
  Latency_map* entry_def_map = msc->def_at_entry( entry);
  Latency_map* entry_use_map = msc->use_at_entry(entry);
  if (dbg(ss, 5)) {
    assert (are_maps_compatible(pred_rgn_def_map, entry_def_map, pred_max_dangle)
    && (are_maps_compatible(pred_rgn_use_map, entry_use_map, pred_max_dangle)));
  } else {
    if (!are_maps_compatible(pred_rgn_def_map, entry_def_map, pred_max_dangle)
    || (!are_maps_compatible(pred_rgn_use_map, entry_use_map, pred_max_dangle)))
    return false;
  }

  for (Region_exit_ops eop(sched_rgn); eop != 0; eop++) {
    Op* exit = (*eop);
    assert(exit->flag(EL_REGION_SCHEDULED));
    int exit_sched_time = exit->sched_time();
    int dangle_reduc = exit_sched_time;
    if (is_dummy_branch(exit))
      dangle_reduc += 1;
    else
      dangle_reduc += MDES_branch_latency();
    if (dangle_reduc >= pred_max_dangle) continue;
    exit_def_map = new Latency_map(0);
    exit_use_map = new Latency_map(0);
    build_lat_map(pred_rgn_def_map, exit_def_map, dangle_reduc);
    build_lat_map(pred_rgn_use_map, exit_use_map, dangle_reduc);
    for (Op_outedges ced(exit, CONTROL, CONTROL0); ced != 0; ced++) {
      if (!sched_rgn->is_exit_edge(*ced)) continue;
      Op* succ_entry = (*ced)->dest();
      Compound_region* succ_rgn = succ_entry->parent();
      while (!succ_rgn->flag(EL_REGION_SCHEDULED))
        succ_rgn = succ_rgn->parent();
      assert(succ_rgn != NULL);
      if (dbg(ss, 5)) {
	assert (check_map_propagation(succ_rgn, succ_entry,
      		exit_def_map, exit_use_map));
      } else {
	if (!check_map_propagation(succ_rgn, succ_entry,
      		exit_def_map, exit_use_map)) return false;
      }
    }
    delete exit_def_map;
    delete exit_use_map;
  }
  return true;
} 
 
bool check_lat_map_propagation (Compound_region* f)
{
 for (Region_subregions riter(f); riter != 0; riter++) {
   Compound_region* blk = (Compound_region*)(*riter);
   assert (blk->is_bb() || blk->is_hb() || blk->is_loopbody());
   assert (blk->flag(EL_REGION_SCHEDULED));
   MS_constraints* msc = get_ms_constraints(blk);
   for (Region_exit_ops eop(blk); eop != 0; eop++) {
     Latency_map* exit_def_map = msc->def_at_exit(*eop);
     Latency_map* exit_use_map = msc->use_at_exit(*eop);
     for (Op_outedges ced(*eop, CONTROL, CONTROL0); ced != 0; ced++) {
       if (!blk->is_exit_edge(*ced)) continue;
       Op* succ_entry = (*ced)->dest();
       Compound_region* succ_rgn = succ_entry->parent();
       while (!succ_rgn->flag(EL_REGION_SCHEDULED))
        succ_rgn = succ_rgn->parent();
       assert(succ_rgn != NULL);
       if (dbg(ss, 5)) {
	 assert(check_map_propagation(succ_rgn, succ_entry,
				    exit_def_map, exit_use_map));
       } else {
	 if (!(check_map_propagation(succ_rgn, succ_entry,
				    exit_def_map, exit_use_map))) return false;
       }
     }
   }
 }
 return true;
}
