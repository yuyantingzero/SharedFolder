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
//      File:           meld_propagate.cpp
//      Authors:        Santosh G. Abraham, Vinod Kathail, Brian Dietrich
//      Created:        July 1995
//      Description:    Accumulate meld constraints from other regions
//
/////////////////////////////////////////////////////////////////////////////

#include "meld_propagate.h"
#include "meld.h"
#include "iterators.h"
#include "op_info.h"
#include "intf.h"	// defines MDES_max_op_lat
#include "tuples.h"
#include "map.h"
#include "attributes.h"
#include "meld_constraints.h"
#include "meld_check.h"
#include "hash_map.h"
#include "edge.h"

Hash_map<Edge*, bool>* cfEdgeIsPredicted;

void init_cfEdgeIsPredicted (Compound_region* reg) {
  if (reg->is_bb() || reg->is_hb()) {
    double freq = reg->weight;
    for (Region_ops_C0_order oiter (reg); oiter != 0; oiter++) {
      Op* op = *oiter;
      if (! reg->is_exit_op (op)) continue;
      for (Op_outedges ei (op, CONTROL, CONTROL0); ei != 0; ei++) {
        Edge* edge = *ei;
	if (! reg->is_exit_edge(edge))
	  continue;
	Control_flow_freq *cfreq = get_control_flow_freq (edge);
	freq -= cfreq->freq;
	if (cfreq->freq > freq) {
	  cfEdgeIsPredicted->bind (edge, true);
	} else {
	  cfEdgeIsPredicted->bind (edge, false);
	}
      }
    }
  } else {
    assert (! reg->is_op());
    for (Region_subregions ri (reg); ri != 0; ri++) {
      init_cfEdgeIsPredicted ((Compound_region*) (*ri));
    }
  }
}

void min_dist_to_entry (Compound_region* region_sched, int cutoff_dist,
		Map <Pair<Compound_region*, Op*>, Pair<int, bool> >& dist_map)
{
 Pair <Compound_region*, Op*> reg_op;
 Pair <int, bool> dist_vis;
 
 for (Region_entry_edges eiter(region_sched); eiter !=0; eiter++) {
   if (SUPERSCALAR_MELD && (!cfEdgeIsPredicted->value(*eiter))) continue;
   Op* exit_op = (*eiter)->src();
   if (exit_op->flag(EL_REGION_SCHEDULED)) {
     Compound_region* ancestor = exit_op->parent();
     assert (ancestor->is_bb() || ancestor->is_hb());
     Compound_region* next_ancestor = ancestor->parent();
     while (next_ancestor->is_bb() || next_ancestor->is_hb()) {
       ancestor = next_ancestor;
       next_ancestor = ancestor->parent();
     }
     assert (next_ancestor != NULL);
     reg_op.first = ancestor;
     reg_op.second = exit_op;
     dist_vis.first = 0;
     dist_vis.second = false;
     dist_map.bind(reg_op, dist_vis);
   }
   else {
     if (region_contains_op (region_sched, exit_op)) {
       reg_op.first = region_sched;
       reg_op.second = exit_op;
       dist_vis.first = 0;
       dist_vis.second = true;
       dist_map.bind(reg_op, dist_vis); 
     }
   }
 }
  
 bool all_visited = false;
 int min_dist;
 Compound_region* visit_reg = NULL;
 Op* visit_exit_op = NULL;
 while (!all_visited) {
   min_dist = ELCOR_MAX_INT;
   all_visited = true;
   for (Map_iterator <Pair <Compound_region*, Op*>, Pair<int, bool> > miter(dist_map);
    		miter !=0; miter++) {
     dist_vis = (*miter).second;
     if ((dist_vis.second == false) && (dist_vis.first < min_dist)) {
       all_visited = false;
       min_dist = dist_vis.first;
       reg_op = (*miter).first;
       visit_reg = reg_op.first;
       visit_exit_op = reg_op.second;
     }
   }
   if (all_visited) break;
   reg_op.first = visit_reg;
   reg_op.second = visit_exit_op;
   assert (dist_map.is_bound(reg_op));
   dist_vis = dist_map.value(reg_op);
   assert (dist_vis.first == min_dist);
   dist_vis.second = true;
   dist_map.bind (reg_op, dist_vis);
    
   Region_entry_ops eoiter (visit_reg);
   Op* entry_op = (*eoiter);
   MS_constraints* msc = get_ms_constraints(visit_reg);
   int dist_to_my_entry = min_dist +
    		msc->get_min_distance(entry_op, visit_exit_op);
   if (dist_to_my_entry > cutoff_dist) continue;
   for (Region_entry_edges eiter (visit_reg); eiter != 0; eiter++) {
     if (SUPERSCALAR_MELD && (!cfEdgeIsPredicted->value(*eiter))) continue;
     Op* exit_op = (*eiter)->src();
     if (exit_op->flag(EL_REGION_SCHEDULED)) {
       Compound_region* ancestor = exit_op->parent();
       while (!ancestor->flag(EL_REGION_SCHEDULED))
	 ancestor = ancestor->parent();
       assert (ancestor != NULL);
       reg_op.first = ancestor;
       reg_op.second = exit_op;
     }
     if (region_contains_op (region_sched, exit_op)) {
       reg_op.first = region_sched;
       reg_op.second = exit_op;
     }  
     if ((exit_op->flag(EL_REGION_SCHEDULED)) ||
       region_contains_op (region_sched, exit_op)) {
       if (dist_map.is_bound (reg_op)) { 
         dist_vis = dist_map.value(reg_op);
	 if (dist_to_my_entry < dist_vis.first) {
	   dist_vis.first = dist_to_my_entry;
	   dist_map.bind (reg_op, dist_vis);
	 }
       }
       else {
	 if (region_contains_op(region_sched, exit_op)) {
	   dist_vis.first = dist_to_my_entry;
           dist_vis.second = true;
	 } else {
           dist_vis.first = dist_to_my_entry;
	   dist_vis.second = false;
         }
	 dist_map.bind (reg_op, dist_vis);
       }
     }
   }
 }
}

void propagate_to_entry_lat_maps(Compound_region* region_sched,
  Map<Pair<Compound_region*, Op*>, Pair<int, bool> >& dist_map,
  Latency_map* entry_def_map, Latency_map* entry_use_map) {
  
 for (Map_iterator <Pair <Compound_region*, Op*>, Pair<int, bool> >
 	miter(dist_map); miter !=0; miter++) {
   Pair <Compound_region*, Op*> reg_op = (*miter).first;
   Compound_region* prop_region = reg_op.first;
   Op* prop_exit_op = reg_op.second;
   Pair <int, bool> dist_vis = (*miter).second;
   int distance = dist_vis.first;
   assert (dist_vis.second);
   if (prop_region == region_sched) continue;
   MS_constraints* msc = get_ms_constraints(prop_region);
   Latency_map* prop_exit_def_map = msc->def_at_exit(prop_exit_op);
   Latency_map* prop_exit_use_map = msc->use_at_exit(prop_exit_op);
   for (Latency_map_iterator oiter(*prop_exit_def_map); oiter != 0; oiter++) {
     Operand opnd = (*oiter).first;
     int dangle = (*oiter).second;
     if ((dangle - distance) < 1) continue;
     if (entry_def_map->is_bound(opnd)) {
       if ((dangle - distance) > entry_def_map->value(opnd)) {
	  entry_def_map->bind(opnd, (dangle-distance));
       }
     } else {
       entry_def_map->bind(opnd, (dangle-distance));
     }
   }
   for (Latency_map_iterator piter(*prop_exit_use_map); piter != 0; piter++) {
     Operand opnd = (*piter).first;
     int dangle = (*piter).second;
     if ((dangle - distance) < 1) continue;
     if (entry_use_map->is_bound(opnd)) {
       if ((dangle - distance) > entry_use_map->value(opnd)) {
	  entry_use_map->bind(opnd, (dangle-distance));
       }
     } else {
       entry_use_map->bind(opnd, (dangle-distance));
     }
   }
 }
}

// this routine is a empty stub for now till we decide
// put operands in entry_def_map as up_exposed_defs
// add operands in entry_use_map to up_exposed_uses
// should we put this info as part of MS_constraints
// should we save current liveness info and restore them at end of scheduling
// should we augment info only on one incoming edge or on all
// 
void set_merge_list (Compound_region* blk, Latency_map* entry_def_map,
			Latency_map* entry_use_map)
{
 MS_constraints* msc = get_ms_constraints(blk);
 Region_entry_ops ei (blk);
 Op* entry_op = (*ei);
 List<Operand>* dmlist = msc->def_merges_at_entry(entry_op);
 assert (dmlist->is_empty());
 for (Latency_map_iterator di (*entry_def_map); di != 0; di++) {
   Operand oper = (*di).first;
   assert ((*di).second > 0);
   dmlist->add_tail(oper);
 }
 List<Operand>* umlist = msc->use_merges_at_entry(entry_op);
 assert (umlist->is_empty());
 for (Latency_map_iterator ui (*entry_use_map); ui != 0; ui++) {
   Operand oper = (*ui).first;
   assert ((*ui).second > 0);
   umlist->add_tail(oper);
 }
}

void min_dist_from_exit (Compound_region* region_sched, Op* exit_op_sched,
		int cutoff_dist,
		Map <Pair<Compound_region*, Op*>, int >& entry_dist_map)
{
 Map <Pair<Compound_region*, Op*>, Pair<int, bool> > exits_dist_map;
 Pair <Compound_region*, Op*> reg_op;
 Pair <int, bool> dist_vis;
 int min_dist;
 Compound_region* visit_reg;
 Op* visit_op;
 
 reg_op.first = region_sched;
 reg_op.second = exit_op_sched;
 dist_vis.first = 0;
 dist_vis.second = false;
 exits_dist_map.bind(reg_op, dist_vis);
  
 bool all_visited = false;
 while (!all_visited) {
   min_dist = ELCOR_MAX_INT;
   all_visited = true;
   for (Map_iterator <Pair <Compound_region*, Op*>, Pair <int, bool> >
	miter(exits_dist_map); miter !=0; miter++) {
     dist_vis = (*miter).second;
     if ((dist_vis.second == false) && (dist_vis.first < min_dist)) {
       all_visited = false;
       min_dist = dist_vis.first;
       reg_op = (*miter).first;
     }
   }
   if (all_visited) break;
   visit_reg = reg_op.first;
   visit_op = reg_op.second;
   assert (exits_dist_map.is_bound(reg_op));
   dist_vis = exits_dist_map.value(reg_op);
   assert (dist_vis.first == min_dist);
   dist_vis.second = true;
   exits_dist_map.bind (reg_op, dist_vis);
   
   for (Op_outedges eiter (visit_op); eiter != 0; eiter++) {
     if (!visit_reg->is_exit_edge(*eiter)) continue;
     if (SUPERSCALAR_MELD && (!cfEdgeIsPredicted->value (*eiter))) continue;

     Compound_region* next_reg = NULL;
     Op* entry_op = (*eiter)->dest();
     if (entry_op->flag(EL_REGION_SCHEDULED)) {
       Compound_region* ancestor = entry_op->parent();
       while (!ancestor->flag(EL_REGION_SCHEDULED))
         ancestor = ancestor->parent();
       assert (ancestor != NULL);
       next_reg = ancestor;
     }
     if (region_contains_op (region_sched, entry_op)) {
       next_reg = region_sched;
     }  
     if ((entry_op->flag(EL_REGION_SCHEDULED)) ||
		region_contains_op (region_sched, entry_op)) {
       reg_op.first = next_reg;
       reg_op.second = entry_op;
       if (!entry_dist_map.is_bound (reg_op) ||
       		(min_dist < entry_dist_map.value(reg_op)))
	   entry_dist_map.bind (reg_op, min_dist);
       for (Region_exit_ops eoiter (next_reg); eoiter != 0; eoiter++) {
         Op* next_exit_op = (*eoiter);
         MS_constraints* msc = get_ms_constraints(next_reg);
         int dist_to_this_exit = min_dist +
    		msc->get_min_distance(entry_op, next_exit_op);
         if (dist_to_this_exit > cutoff_dist) continue;
         reg_op.first = next_reg;
         reg_op.second = next_exit_op;
         if (exits_dist_map.is_bound(reg_op)) {
           dist_vis = exits_dist_map.value(reg_op);
           if (dist_to_this_exit < dist_vis.first) {
             dist_vis.first = dist_to_this_exit;
	     assert (dist_vis.second == false);
             exits_dist_map.bind(reg_op, dist_vis);
           }
         }
         else {
           dist_vis.first = dist_to_this_exit;
           dist_vis.second = false;
	   exits_dist_map.bind(reg_op, dist_vis);
         }
       }
     }
   }
 }
}

void propagate_to_exit_lat_maps(Compound_region* region_sched,
  Map<Pair<Compound_region*, Op*>, int >& dist_map,
  Latency_map* exit_def_map, Latency_map* exit_use_map) {
  
 int min_def_default_value = MDES_max_op_lat;
 int min_use_default_value = MDES_max_op_lat;
 for (Map_iterator <Pair <Compound_region*, Op*>, int >
 	miter1(dist_map); miter1 !=0; miter1++) {
   Pair <Compound_region*, Op*> reg_op = (*miter1).first;
   Compound_region* prop_region = reg_op.first;
   Op* prop_entry_op = reg_op.second;
   int distance = (*miter1).second;
   MS_constraints* msc = get_ms_constraints(prop_region);
   Latency_map* prop_entry_def_map = msc->def_at_entry(prop_entry_op);
   Latency_map* prop_entry_use_map = msc->use_at_entry(prop_entry_op);
   min_def_default_value = ELCOR_MIN(min_def_default_value,
		(distance + prop_entry_def_map->get_default_value()));
   min_use_default_value = ELCOR_MIN(min_use_default_value,
		(distance + prop_entry_use_map->get_default_value()));
 }
 exit_def_map->set_default_value(min_def_default_value);
 exit_use_map->set_default_value(min_use_default_value);

 for (Map_iterator <Pair <Compound_region*, Op*>, int >
 	miter(dist_map); miter !=0; miter++) {
   Pair <Compound_region*, Op*> reg_op = (*miter).first;
   Compound_region* prop_region = reg_op.first;
   Op* prop_entry_op = reg_op.second;
   int distance = (*miter).second;
   MS_constraints* msc = get_ms_constraints(prop_region);
   Latency_map* prop_entry_def_map = msc->def_at_entry(prop_entry_op);
   Latency_map* prop_entry_use_map = msc->use_at_entry(prop_entry_op);
   for (Latency_map_iterator oiter(*prop_entry_def_map); oiter != 0; oiter++) {
     Operand opnd = (*oiter).first;
     int dangle = (*oiter).second;
     if ((dangle + distance) >= min_def_default_value) continue;
     if (exit_def_map->is_bound(opnd)) {
       if ((dangle + distance) < exit_def_map->value(opnd)) {
	  exit_def_map->bind(opnd, (dangle+distance));
       }
     } else {
       exit_def_map->bind(opnd, (dangle+distance));
     }
   }
   for (Latency_map_iterator piter(*prop_entry_use_map); piter != 0; piter++) {
     Operand opnd = (*piter).first;
     int dangle = (*piter).second;
     if ((dangle + distance) >= min_use_default_value) continue;     
     if (exit_use_map->is_bound(opnd)) {
       if ((dangle + distance) < exit_use_map->value(opnd)) {
	  exit_use_map->bind(opnd, (dangle+distance));
       }
     } else {
       exit_use_map->bind(opnd, (dangle+distance));
     }
   }
 }
}

void set_switch_list(Compound_region* region, Op* exit_op,
	 List <Operand>& swdeflist) {
  MS_constraints* msc = get_ms_constraints (region);
  List<Operand>* operlist = msc->def_switches_at_exit(exit_op);
  (*operlist) = swdeflist;
}
