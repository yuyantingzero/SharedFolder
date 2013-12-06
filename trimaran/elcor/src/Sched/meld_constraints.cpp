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
//      File:           meld_constraints.cpp
//      Authors:        Brian Dietrich, Santosh G. Abraham, Vinod Kathail
//      Created:        July 1995
//      Description:    Transfers meld constraints to/from scheduling edges
//
/////////////////////////////////////////////////////////////////////////////

#include "operand.h"
#include "opcode_properties.h"
#include "el_error.h"
#include "iterators.h"
#include "meld.h"
#include "meld_constraints.h"
#include "attributes.h"
#include "core_edge_drawing.h"
#include "edge_drawing.h"
#include "connect.h"
#include "op_info.h"
#include "intf.h"	// defines MDES_max_op_lat
#include "el_ssched_init.h"
#include "slist.h"
#include "hash_map.h"
#include "hash_functions.h"
#include "tuples.h"
#include "edge.h"
#include "edge_utilities.h"
#include "sched_functions.h"
#include "dbg.h"

// Finds all references to an oper by following edges recursively
// Assumes that the only transitive reduction of edges
// is performed on def-def edges
void find_refs_to_oper(Op* op, Port_num port, Operand oper,
	Hash_set<Quad<Op*, Port_type, Port_num, Operand> >* refs_list)
{
// removed comment - SGA 3/1/96
 // commented out for now since meld uses all 4 dataflow sets, this
   // will be updated later. Shail + Vinod 01/22/96
  for (Op_outedges eiter(op, DEST, port); eiter != 0; eiter++) {
    Op* sink_op = (*eiter)->dest();
    if ((*eiter)->dest_port_type() == SRC) {
      Port_num port = (*eiter)->dest_port();
      bool oper_found = false;
      if (sink_op->src(port) == oper) {
	oper_found = true;
      } else {
        assert (is_brl(op) || is_rts(op) ||
		is_brl(sink_op) || is_rts(sink_op));
	if (is_brl(sink_op) || is_rts(sink_op)) {
	  for (Op_implicit_sources si (sink_op); si != 0; si++) {
	    if ((*si) == oper) {
	      oper_found = true; break;
	    }
	  }
	}
      }
      if (oper_found) {
	Quad<Op*, Port_type, Port_num, Operand> ref(sink_op, SRC, port, oper);
	(*refs_list) += ref;
      } else {
	assert (is_brl(op) || is_rts(op));
      }
      continue;
    }
    if ((*eiter)->dest_port_type() == DEST) {
      Port_num port = (*eiter)->dest_port();
      bool oper_found = false;
      if (sink_op->dest(port) == oper) {
	oper_found = true;
      } else {
        assert (is_brl(op) || is_rts(op) ||
		is_brl(sink_op) || is_rts(sink_op));
	if (is_brl(sink_op) || is_rts(sink_op)) {
	  for (Op_implicit_dests di (sink_op); di != 0; di++) {
	    if ((*di) == oper) {
	      oper_found = true; break;
	    }
	  }
	}
      }
      if (oper_found) {
	Quad<Op*, Port_type, Port_num, Operand> ref(sink_op, DEST, port, oper);
	(*refs_list) += ref;
	find_refs_to_oper(sink_op, port, oper, refs_list);
      } else {
	assert (is_brl(op) || is_rts(op));
      }
      continue;
    }
    assert (0); // port_type of edge from oper has to be SRC or DEST
  }
}

void pseudo_def_use_maps (Op* op, Hash_map<Operand, Op*>& def_map,
				  Hash_map<Operand, Op*>& use_map) {

 for (List_iterator<Op*> related(op->get_relops()); related != 0;
       related++) {
   Op* related_op = *related;
   Port_num dest_port = related_op->first_dest();
   Port_num src_port  = related_op->first_src();
   if (!(related_op->dest(dest_port)).is_undefined()) {
     def_map.bind (related_op->dest(dest_port), related_op);
     assert (related_op->src(src_port).is_undefined());
   } else {
     assert (!related_op->src(src_port).is_undefined() ||
	related_op->num_mem_srcs() > 0);
     use_map.bind (related_op->src(src_port), related_op);
   }
 }
}
 
void insert_pass_through_edges (Compound_region *region)
{
 
 int edge_lat = 0;
 Hash_map<Operand, Op*> mg_def_map (hash_operand, 211);
 Hash_map<Operand, Op*> mg_use_map (hash_operand, 211);

 Region_entry_ops ei(region);
 Op* entry_op = (*ei);
 pseudo_def_use_maps (entry_op, mg_def_map, mg_use_map);

 Op* merge_op;
 Edge_style es = ES_MELD;
 MS_constraints* msc = get_ms_constraints(region);
 Latency_map* entry_def_map = msc->import_def_at_entry(entry_op);
 Latency_map* entry_use_map = msc->import_use_at_entry(entry_op);
 for (Region_exit_ops exi(region); exi != 0; exi++) {
   Op* exit_op = (*exi);
   Hash_map<Operand, Op*> sw_def_map (hash_operand, 211);
   Hash_map<Operand, Op*> sw_use_map (hash_operand, 211);
   pseudo_def_use_maps (exit_op, sw_def_map, sw_use_map);

   Latency_map* exit_def_map = msc->import_def_at_exit(exit_op);
   Latency_map* exit_use_map = msc->import_use_at_exit(exit_op);

   Latency_map_iterator ldi ;
   for (ldi(*entry_def_map); ldi != 0; ldi++) {
     Operand oper = (*ldi).first;
     edge_lat = (*ldi).second;
     bool merge_waspresent = mg_def_map.is_bound(oper);
     bool merge_needed = false;
     if (merge_waspresent) {
       merge_op =  mg_def_map.value(oper);
     } else {
       merge_op = new Op(D_MERGE);
       merge_op->dest(DEST1) = oper;
     }   
     if (exit_def_map->is_bound(oper)) {
       merge_needed = true;
       Op* switch_op;
       bool switch_present = sw_def_map.is_bound(oper);
       if (!switch_present) {
         switch_op = new Op(D_SWITCH);
         switch_op->dest(DEST1) = oper;
         Operand& cond = exit_op->src(SRC1);
         if (cond.is_reg() && cond.file_type() == PR) {
           switch_op->src(PRED1) = cond;
         }
         add_switch_to_control_switch(exit_op, switch_op);
         sw_def_map.bind (oper, switch_op);
       } else {
         switch_op = sw_def_map.value(oper);
       }
       assert (oper.is_reg() || oper.is_macro_reg());
       // use direct call to connect_ops to get the edge*
       // reg_out_edge (merge_op,   merge_op->first_dest(),
       //		       switch_op, switch_op->first_dest(), edge_lat);
       Reg_out* edge_out = new Reg_out();
       connect_ops(edge_out, merge_op, merge_op->first_dest(),
			switch_op, switch_op->first_dest());
       set_edge_latency ( (Edge *) edge_out, es);
     }
     if (exit_use_map->is_bound(oper)) {       
       merge_needed = true;
       Op* switch_op;
       bool switch_present = sw_use_map.is_bound(oper);
       if (!switch_present) {
         switch_op = new Op(D_SWITCH);
         switch_op->src(SRC1) = oper;
         Operand& cond = exit_op->src(SRC1);
         if (cond.is_reg() && cond.file_type() == PR) {
           switch_op->src(PRED1) = cond;
         }
         add_switch_to_control_switch(exit_op, switch_op);
         sw_use_map.bind (oper, switch_op);
       } else {
         switch_op = sw_use_map.value(oper);
       }
       assert (oper.is_reg() || oper.is_macro_reg());

       // use direct call to connect_ops to get the edge*
       // reg_flow_edge  (merge_op,   merge_op->first_dest(),
       //		       switch_op, switch_op->first_dest(), edge_lat);
       Reg_flow* edge_flow = new Reg_flow();
       connect_ops(edge_flow, merge_op, merge_op->first_dest(),
			switch_op, switch_op->first_dest());
       set_edge_latency ((Edge *) edge_flow, es);
     }         
     if (!merge_waspresent) {
       if (merge_needed) {
         add_merge_to_control_merge (entry_op, merge_op);
         mg_def_map.bind (oper, merge_op);
       } else {
         delete merge_op;
       }
     }
   }

   for (Latency_map_iterator lui(*entry_use_map); lui != 0; lui++) {
     Operand oper = (*ldi).first;
     bool merge_waspresent = mg_use_map.is_bound(oper);
     bool merge_needed = false;
     if (merge_waspresent) {
       merge_op =  mg_use_map.value(oper);
     } else {
       merge_op = new Op(D_MERGE);
       merge_op->src(SRC1) = oper;
     }   
     if (exit_def_map->is_bound(oper)) {
       merge_needed = true;
       Op* switch_op;
       bool switch_present = sw_def_map.is_bound(oper);
       if (!switch_present) {
         switch_op = new Op(D_SWITCH);
         switch_op->dest(DEST1) = oper;
         Operand& cond = exit_op->src(SRC1);
         if (cond.is_reg() && cond.file_type() == PR) {
           switch_op->src(PRED1) = cond;
         }
         add_switch_to_control_switch(exit_op, switch_op);
         sw_def_map.bind (oper, switch_op);
       } else {
         switch_op = sw_def_map.value(oper);
       }
       assert (oper.is_reg() || oper.is_macro_reg());
       // use direct call to connect_ops to get the edge*
       // reg_anti_edge (merge_op,   merge_op->first_src(),
       //		       switch_op, switch_op->first_dest(), edge_lat);
       Reg_anti* edge_anti = new Reg_anti();
       connect_ops(edge_anti, merge_op, merge_op->first_dest(),
			switch_op, switch_op->first_dest());
       set_edge_latency ((Edge *) edge_anti, es);
     }
     if (!merge_waspresent) {
       if (merge_needed) {
         add_merge_to_control_merge (entry_op, merge_op);
         mg_use_map.bind (oper, merge_op);
       } else {
         delete merge_op;
       }
     }
   }
 }
}

bool corresponding_merge_present (Compound_region *region,
                                  Operand operand_to_check)
{
  Region_entry_ops e_op(region);
  Op* entry_op = *e_op;

  for (List_iterator<Op*> related(entry_op->get_relops()); related != 0;
       related++) {
    Op* related_op = *related;
    if (related_op->src(related_op->first_src()) == operand_to_check)
      return (true); // found
    if (related_op->dest(related_op->first_dest()) == operand_to_check)
      return (true); // found
  }

  return (false); // didn't find a merge for this operand.
}


void set_entry_edge_latencies (Compound_region *region, Op *entry_op,
                                Latency_map *def_constraints,
                                Latency_map *use_constraints)
{
  Op  *merge_op;
  int  dangle_latency;
  bool  found_correct_edge;
  Hash_map<Operand, Op*> def_map(hash_operand, 211);
  Hash_map<Operand, Op*> use_map(hash_operand, 211);
  Port_num  src_port, dest_port;

  if (El_ssched_meld_model == NO_MELD) return;
 
  // Consider replacing this code section with a call to pseudo_def_use_maps
  // iterate over merge ops to create map of operand to op* for quick access
  //   to merge ops when traversing through input constraint latency maps.
  for (List_iterator<Op*> related(entry_op->get_relops());
       related != 0; related++) {
    Op *related_op = *related;
    if (!is_merge (related_op))
      El_punt ("SCHEDULER: set_entry_edge_latencies expected a merge op");

    if ((related_op->num_srcs() != 1) && (related_op->num_dests() != 1))
      El_punt ("SCHEDULER: set_entry_edge_latencies found wrong number of \
operands in a merge op.");

    int opers = 0;
    src_port = related_op->first_src();
    dest_port = related_op->first_dest();
    if (!related_op->dest(dest_port).is_undefined ()) {
      def_map.bind (related_op->dest(dest_port), related_op);
      opers++;
    }
    if (!related_op->src(src_port).is_undefined ()) {
      use_map.bind (related_op->src(src_port), related_op);
      opers++;     
    }
    assert ( (opers == 1) ||
            ((opers == 0) && (related_op->num_mem_srcs() > 0)));
  }

  // iterate over def_constraints latency map and set edge latencies 
  //   accordingly.
  // this map contains dangles into this region that must be accommadated given
  //   the way that a previous region was scheduled.
  for (Latency_map_iterator mi(*def_constraints); mi != 0; mi++) {
    Operand curr_operand = (*mi).first;
    dangle_latency = (*mi).second;
    if (def_map.is_bound (curr_operand)) {
      merge_op = def_map.value (curr_operand);
      found_correct_edge = false;
      for (Op_outedges out(merge_op, DEST, merge_op->first_dest());
           out != 0; out++) {
        Edge* curr_edge = *out;
        found_correct_edge = true;
        if (curr_edge->latency() < dangle_latency) { // use new constraint.
          curr_edge->set_latency (dangle_latency);
          // decrease edge latency by 1 or br_lat if edge dest is switch
          int adj_lat = adjust_edge_latency_scalar(curr_edge);
          curr_edge->set_latency (adj_lat);
        }
      }

      if (!found_correct_edge)
        El_punt ("SCHEDULER: set_entry_edge_latencies didn't find a flow or \
output arc.");
    }
  }

  // iterate over use_constraints latency map and set edge latencies 
  //   accordingly.
  for (Latency_map_iterator mi1(*use_constraints); mi1 != 0; mi1++) {
    Operand curr_operand = (*mi1).first;
    dangle_latency = (*mi1).second;

    if (use_map.is_bound (curr_operand)) {
      merge_op = use_map.value (curr_operand);

      for (Op_outedges out(merge_op, SRC, merge_op->first_src());
           out != 0; out++) {
        Edge *curr_edge = *out;
        if (curr_edge->latency() < dangle_latency) { // use new constraint.
          curr_edge->set_latency (dangle_latency);
          // decrease edge latency by 1 or br_lat if edge dest is switch
          int adj_lat = adjust_edge_latency_scalar(curr_edge);
          curr_edge->set_latency (adj_lat);
        }
      }
    }
  }
}

void set_prelim_entry_export_latency_maps (Compound_region *region,
                                   Hash_map<Op*, int> *early_map)
// same assumptions and similar functionality as set_latency_map_constraints
//   function.  major difference is that only entry maps need to be made, and
//   Etime is used instead of sched time when determining the values that
//   should be present in the constraint maps.
{
  Latency_map  *entry_use_map, *entry_def_map;
  MS_constraints  *msc;

  if (El_ssched_meld_model == NO_MELD) return;
  msc = get_ms_constraints (region);
  Region_entry_ops e_op (region);
  Op *entry_op = *e_op;
  e_op++;
  if (e_op != 0)   // ensure that there is only one entrance into this region.
    El_punt ("More than one entry op found in regon.");

  entry_use_map = msc->use_at_entry (entry_op);
  entry_def_map = msc->def_at_entry (entry_op);

  int entry_default = MDES_max_op_lat;
  // iterate over all the ops in order to set the values of the latency maps.
  for (Region_subregions iter(region); iter != 0; iter++) {
    assert ((*iter)->is_op());
    Op *curr_op = (Op*) (*iter);
    Op* real = real_op(curr_op);
    if (is_merge(curr_op)) assert (real->opcode() == C_MERGE);
    if (is_switch(curr_op) || is_merge(curr_op)) continue;
    // || is_merge(curr_op)) && ((!is_call(real))  && (!is_return(real))))
    int etime = early_map->value(real);
    assert (etime >= 0);

    // look at all source operands and set entry use maps accordingly.
    for (Op_all_inputs oi (curr_op); oi != 0; oi++) {
      Operand curr_operand = (*oi);
      Port_num src_p = oi.get_port_num();
      if (!(curr_operand.is_reg() || curr_operand.is_macro_reg())) continue;
      int new_can_dangle = etime + curr_op->flow_time (SRC, src_p);
                      // = etime + earliest sample time
      if (new_can_dangle < entry_default) {
        if (!entry_use_map->is_bound (curr_operand) ||
            (entry_use_map->value (curr_operand) > new_can_dangle))
          entry_use_map->bind (curr_operand, new_can_dangle);
        }
    }

    // look at all dest operands and set entry def maps accordingly.
    for (Op_all_dests oo (curr_op); oo != 0; oo++) {
      Operand curr_operand = (*oo);
      Port_num dest_p = oo.get_port_num();
      if (!(curr_operand.is_reg() || curr_operand.is_macro_reg())) continue;
      int new_can_dangle = etime + curr_op->anti_time (DEST, dest_p);
                      // = etime + earliest write time
      if (new_can_dangle < entry_default) {
        if (!entry_def_map->is_bound (curr_operand) ||
            (entry_def_map->value (curr_operand) > new_can_dangle)) {
          entry_def_map->bind (curr_operand, new_can_dangle);
	}
      }
    }

    if (region_op_at_exit (region, curr_op)) { // exit encountered.  need to
                // set entry-exit pair distance (based on etimes) accordingly.
      int op_distance = etime + MDES_branch_latency();
      msc->set_min_distance (entry_op, curr_op, op_distance);
      if (is_return(curr_op)) {
        int new_can_dangle = etime + MDES_branch_latency();
        if (new_can_dangle < (entry_def_map->get_default_value())) {
          entry_def_map->set_default_value (new_can_dangle);
          entry_use_map->set_default_value (new_can_dangle);
	  entry_default = new_can_dangle;
	  for (Latency_map_iterator mi(*entry_def_map); mi != 0;) {
	    Pair<Operand, int> op_lat = (*mi);
	    mi++;
	    if (op_lat.second >= new_can_dangle)
	      entry_def_map->unbind(op_lat.first);
	  }
	  for (Latency_map_iterator ni(*entry_use_map); ni != 0;) {
	    Pair<Operand, int> op_lat = (*ni);
	    ni++;
	    if (op_lat.second >= new_can_dangle)
	      entry_use_map->unbind(op_lat.first);
	  }
	}
      }
    }
  }
}

void determ_ops_above_branch (Op *exit_op, Op *curr_op,
                              Hash_set<Op*> *ops_before_exit,
                              Hash_map<Op*, int> *early_map,
                              Slist<Stuple<int, Op*> > *ops_to_move)
// recursive function that adds curr_op to ops_to_move as long as it is located
//   above the branch.  if it is located above the branch, it recursively calls
//   itself for all dests of edges going out of the curr_op.
{
  if (!ops_before_exit->is_member (curr_op)) return; // located below branch.
  Stuple<int, Op*> etime_and_op (- early_map->value(curr_op), curr_op);
  if (ops_to_move->is_member (etime_and_op)) return;
  ops_to_move->add (etime_and_op);

  for (Op_outedges out_edge(curr_op); out_edge != 0; out_edge++) {
    Edge *curr_edge = *out_edge;
    Op* dest_op = real_op(curr_edge->dest());
    if (!is_CONTROL0(curr_edge)) {
       determ_ops_above_branch (exit_op, dest_op, ops_before_exit, 
                                early_map, ops_to_move);
// Need to prevent 0-edge latency dependencies going backwards in C0 order.
// Long term fix is to completely rethread C0.
// Short term fix implmentation here relies on Slist.add details
// Check if outgoing latency is 0 and etimes are same on src and dest
// if so, remove and reinsert the dest to enforce
// correct C0 threading in the presence of 0-edge latencies
//	SGA 8/95
      Stuple<int, Op*> et_op (- early_map->value(dest_op), dest_op);
      if ((curr_edge->latency() == 0) &&
    	(early_map->value(dest_op) == early_map->value(curr_op)) &&
	(ops_to_move->is_member(et_op))) {
        ops_to_move->remove (etime_and_op);
        ops_to_move->add (etime_and_op);
      }
    }
  }
}


// Move op_to_move above exit_op in C0 order
// Also move other ops in C0 order so that all edges are flowing forward
//	in C0 order
void move_op_below_branch_in_c0_order (Op *exit_op, Op *op_to_move,
                                       Hash_set<Op*> *ops_before_exit,
                                       Hash_map<Op*, int> *early_map,
                                       Compound_region *region)

{
  Op *prev_op;

  Slist<Stuple<int, Op*> > ops_to_move;
  determ_ops_above_branch (exit_op, op_to_move, ops_before_exit, early_map,
                           &ops_to_move);

  Edge *c0_out_of_branch = NULL;
  for (Op_outedges in(exit_op, CONTROL0_INEDGES); in != 0; in++) {
    Edge *edge = *in;
    if (!region->is_exit_edge(edge)) {
      if (c0_out_of_branch) // found too many c0 edges.
        El_punt ("invalid assumption made on outgoing control edges from branch\
.");
      c0_out_of_branch = edge;
    }
  }
  if (!c0_out_of_branch) {
    cerr << "Exit op is:\n" << (*exit_op) << endl;
    cerr << "Op triggering move is:\n" << (*op_to_move) << endl;
    cerr << "Ops to move below branch are:\n" << (ops_to_move) << endl;
    cerr << "Region involved is:\n" << (*region) << endl;
    El_punt ("did not find a valid control edge exiting branch.");
  }

  for (Slist_iterator<Stuple<int, Op*> > iter(ops_to_move); iter != 0; iter++) {
    Stuple<int, Op*> etime_and_op = *iter;
    Op* op = etime_and_op.second;

    ///////// remove op from its current location in c0 order.
    // remove c0 edge located between previous op and op.
    Op_inedges curr_in(op, CONTROL0_INEDGES);
    Edge *op_c0_in_edge = *curr_in;
    curr_in++;
    if (curr_in != 0) // there should be only 1 c0 edge into this op.
      El_punt ("Invalid assumption made on c0 edges into op.");
    prev_op = op_c0_in_edge->src();
    prev_op->remove_outedge(op_c0_in_edge, CONTROL, CONTROL0,
                            op_c0_in_edge->src_alt());
    op->remove_inedge(op_c0_in_edge, CONTROL, CONTROL0,
                      op_c0_in_edge->dest_alt());
    delete op_c0_in_edge;

    // alter c0 edge located between op and next op.  make previous op become
    //   source of this c0 edge.  removes op from its place in c0 order.
    Op_outedges curr_out(op, CONTROL0_OUTEDGES);
    Edge *op_c0_out_edge = *curr_out;
    curr_out++;
    if (curr_out != 0) // there should be only 1 c0 edge out of this op.
      El_punt ("Invalid assumption made on c0 edges out of op.");
//    src_alt = get_new_out_alt_num (prev_op);
    op->remove_outedge(op_c0_out_edge, CONTROL, CONTROL0,
                       op_c0_out_edge->src_alt());
    op_c0_out_edge->set_src (prev_op, CONTROL0);
    prev_op->add_outedge(op_c0_out_edge, CONTROL, CONTROL0);

    ///////// place op into its new position located right after exit_op.
    // replace c0 edge going from op located immediately below branch to op.
    //   the branch exit op still remains connected to this edge.
    Op* next_op = c0_out_of_branch->dest();
//    dest_alt = get_new_in_alt_num (op);
    next_op->remove_inedge(c0_out_of_branch, CONTROL, CONTROL0,
                           c0_out_of_branch->dest_alt());
    c0_out_of_branch->set_dest (op, CONTROL0);
    op->add_inedge(c0_out_of_branch, CONTROL, CONTROL0);

    // add c0 edge going from prev_op to op.
    Control *new_edge = new Control();
//    src_alt = get_new_out_alt_num (op);
//    dest_alt = get_new_in_alt_num (next_op);
    new_edge->set_src (op, CONTROL0);
    new_edge->set_dest (next_op, CONTROL0);
    op->add_outedge(new_edge, CONTROL, CONTROL0);
    next_op->add_inedge(new_edge, CONTROL, CONTROL0);

    *ops_before_exit -= op; // op is no longer located above the branch.
  }

}


void determ_ops_below_branch (Op *exit_op, Op *curr_op,
                              Hash_set<Op*> *ops_before_exit,
                              Hash_map<Op*, int> *early_map,
                              Slist<Stuple<int, Op*> > *ops_to_move)
// recursive function that adds curr_op to ops_to_move as long as it is located
//   below the branch.  if it is located below the branch, it recursively calls
//   itself for all sources of edges coming into the curr_op.
{
  if (ops_before_exit->is_member (curr_op)) return; // located above branch.
  Stuple<int, Op*> etime_and_op (early_map->value(curr_op), curr_op);
  if (ops_to_move->is_member(etime_and_op)) return;
  ops_to_move->add (etime_and_op);

  for (Op_inedges in_edge(curr_op); in_edge != 0; in_edge++) {
    Edge *curr_edge = *in_edge;
    if (!is_CONTROL0(curr_edge)) {
    // don't traverse control edges or ops that don't have an associated etime.
       Op* src_op = real_op(curr_edge->src());
       determ_ops_below_branch (exit_op, src_op, ops_before_exit, 
                                early_map, ops_to_move);
    }
  }
// Code to account for 0-cycle edge latencies
  for (Op_outedges out_edge(curr_op); out_edge != 0; out_edge++) {
    Edge* curr_edge = (*out_edge);
    if (!is_CONTROL0(curr_edge)) {
      Op* dest_op = real_op(curr_edge->dest());
      Stuple<int, Op*> et_op (early_map->value(dest_op), dest_op);
      if ((curr_edge->latency() == 0) &&
      	(early_map->value(dest_op) == early_map->value(curr_op)) &&
		(ops_to_move->is_member(et_op))) {
        ops_to_move->remove (et_op);
        ops_to_move->add (et_op);
      }
    }
  }
}


void move_op_above_branch_in_c0_order (Op *exit_op, Op *op_to_move,
                                       Hash_set<Op*> *ops_before_exit,
                                       Hash_map<Op*, int> *early_map)
// this function is called when set_exit_edge_latencies decided to put a
//   constraint for an upward exposed def which forces a new edge to be
//   created which goes in a direction opposite of the c0 order.  for the
//   calculate earliest and latest routines to function correctly, this
//   cannot occur.  cycles are not a problem because the calling function
//   ensured that they cannot occur.
{
  Op *prev_op;
  Alt_num src_alt, dest_alt;

  Slist<Stuple<int, Op*> > ops_to_move;
  determ_ops_below_branch (exit_op, op_to_move, ops_before_exit, early_map,
                           &ops_to_move);

  Op_inedges in(exit_op, CONTROL0_INEDGES);
  Edge *c0_into_branch = *in;
  in++;
  if (in != 0) // there should be only 1 c0 edge into the exit op.
    El_punt ("Invalid assumption made on c0 edges into the exit_op.");

  for (Slist_iterator<Stuple<int, Op*> > iter(ops_to_move); iter != 0; iter++) {
    Stuple<int, Op*> etime_and_op = *iter;
    Op* op = etime_and_op.second;

    ///////// remove op from its current location in c0 order.
    // remove c0 edge located between previous op and op.
    Op_inedges curr_in(op, CONTROL0_INEDGES);
    Edge *op_c0_in_edge = *curr_in;
    curr_in++;
    if (curr_in != 0) // there should be only 1 c0 edge into this op.
      El_punt ("Invalid assumption made on c0 edges into op.");
    prev_op = op_c0_in_edge->src();
    prev_op->remove_outedge(op_c0_in_edge, CONTROL, CONTROL0, 
                            op_c0_in_edge->src_alt());
    op->remove_inedge(op_c0_in_edge, CONTROL, CONTROL0,
                      op_c0_in_edge->dest_alt());
    delete op_c0_in_edge;

    // alter c0 edge located between op and next op.  make previous op become
    //   source of this c0 edge.  removes op from its place in c0 order.
    Op_outedges curr_out(op, CONTROL0_OUTEDGES);
    Edge *op_c0_out_edge = *curr_out;
    curr_out++;
    if (curr_out != 0) // there should be only 1 c0 edge out of this op.
      El_punt ("Invalid assumption made on c0 edges out of op.");
    src_alt = get_new_out_alt_num (prev_op);
    op->remove_outedge(op_c0_out_edge, CONTROL, CONTROL0, 
                       op_c0_out_edge->src_alt());
    op_c0_out_edge->set_src (prev_op, CONTROL0);
    prev_op->add_outedge(op_c0_out_edge, CONTROL, CONTROL0, src_alt);

    ///////// place op into its new position located right before exit_op.
    // replace c0 edge going from op located immediately above branch to op.
    //   the branch exit op still remains connected to this edge.
    prev_op = c0_into_branch->src();
    src_alt = get_new_out_alt_num (op);
    prev_op->remove_outedge(c0_into_branch, CONTROL, CONTROL0, 
                            c0_into_branch->src_alt());
    c0_into_branch->set_src (op, CONTROL0);
    op->add_outedge(c0_into_branch, CONTROL, CONTROL0, src_alt);

    // add c0 edge going from prev_op to op.
    Control *new_edge = new Control();
    src_alt = get_new_out_alt_num (prev_op);
    dest_alt = get_new_in_alt_num (op);
    new_edge->set_src (prev_op, CONTROL0);
    new_edge->set_dest (op, CONTROL0);
    prev_op->add_outedge(new_edge, CONTROL, CONTROL0, src_alt);
    op->add_inedge(new_edge, CONTROL, CONTROL0, dest_alt);

    *ops_before_exit += op; // op is now located above the branch.
  }
}

// Reorders ops to handle incoming def dangle constraints from exit op
bool reorder_ops_around_branches(Compound_region* region_sched, Op* exit_op,
		Latency_map* import_def_map, Hash_map<Op*, int> *early_map,
		List<Operand>& swdeflist) {

 bool redraw_edges = false;
 int br_lat = MDES_branch_latency();
 if (is_dummy_branch(exit_op)) br_lat = 1;

// First, put ops before exit_op into a Hash_set
// We'll need to quickly see if op is before/after exit_op in C0 order
    Hash_set<Op*> ops_before_exit(hash_op_ptr, 128);
    for (Region_ops_reverse_C0_order citer(region_sched, exit_op);
          citer != 0; citer++)
      ops_before_exit += (*citer);

/* Don't need the def_mg_map computed in this section for now
// Next, put def merges into a map
// We assume that all ops that use or def operand will be connected to
// the corresponding def merge.
    Map<Operand, Op*> def_mg_map;
    Region_entry_ops eiter(region_sched);
    Op* entry = (*eiter);
    for (List_iterator<Op*> miter(entry->get_relops());
          miter != 0; miter++) {
      Op* merge = (*miter);
      assert (is_merge(merge));
      if (!(merge->dest(DEST1)).is_undefined())
          def_mg_map.bind(merge->dest(DEST1), merge);
    }
*/

// For each operand in import_def_map,
//  generate a list of all refs to that operand and install in map
//  
 Pair <Op*, Port_num> ref;
 List <Pair <Op*, Port_num> >* reflist;
 Map <Operand,  List <Pair <Op*, Port_num> >* > oper2defmap;
 Map <Operand,  List <Pair <Op*, Port_num> >* > oper2usemap;
 
 // Initialize oper2defmap and oper2usemap with entry per operand
 //	 in import_def_map
 for (Latency_map_iterator ni(*import_def_map); ni != 0; ni++) {
   Operand oper = (*ni).first;
   reflist = new  List <Pair <Op*, Port_num> >;
   oper2defmap.bind (oper, reflist);
   reflist = new  List <Pair <Op*, Port_num> >;
   oper2usemap.bind (oper, reflist);
 }

 // Identify and put operations whose latency for an operand
 //   exceeds allowed dangle, and put them in oper2defmap or oper2usemap list
 for (Region_ops_C0_order oi (region_sched); oi != 0; oi++) {
   Op* op = (*oi);
   for (Op_all_dests pri (op); pri != 0; pri++) {
     Operand oper = (*pri);
     if (oper2defmap.is_bound(oper)) {
       assert (import_def_map->is_bound(oper));
       int lat_in_map = import_def_map->value(oper);
       Port_num pn = pri.get_port_num();
       if (op->flow_time(DEST, pn) > lat_in_map + br_lat) {
         reflist = oper2defmap.value(oper);
         ref.first = op;
         ref.second = pn;
         reflist->add_tail(ref);
	 // on equals latency, we should never/rarely enter here
	 // print out details
	 if (dbg (ss, 5)) {
	   cdbg << "Possible error: Def-def dangle detected in region: "
		<< region_sched->id()
		  << " on Op: " << op->id() << " with latency "
		  << op->flow_time(DEST, pn) << endl;
	   cdbg << " Operand is: " << oper << endl;
	 }
         redraw_edges = true;
       }
     }
   }
   for (Op_all_inputs ori (op); ori != 0; ori++) {
     Operand oper = (*ori);
     if (oper2usemap.is_bound(oper)) {
       assert (import_def_map->is_bound(oper));
       int lat_in_map = import_def_map->value(oper);
       Port_num pn = ori.get_port_num();
       if (op->anti_time(SRC, pn) > lat_in_map + br_lat) {
         reflist = oper2usemap.value(oper);
         ref.first = op;
         ref.second = pn;
         reflist->add_tail(ref);
	 // on equals latency, we should never/rarely enter here
	 // print out details
	 if (dbg (ss, 5)) {
	   cdbg << "Possible error: Use-def dangle detected in region: "
		<< region_sched->id()
		  << " on Op: " << op->id() << " with latency "
		  << op->anti_time(SRC, pn) << endl;
	   cdbg << " Operand is: " << oper << endl;
	 }
         redraw_edges = true;
       }
     }
   }
 }

 // generate swdeflist
 // an oper in import_def_map is put in swdeflist if
 // it is in list of oper2defmap or oper2usemap or
 // it is in the entry import def/use map and
 //     the required entry dangle > exit available dangle
 MS_constraints* msc = get_ms_constraints(region_sched);
 Region_entry_ops ei (region_sched);
 Op* entry = *(ei);
 Latency_map* entry_import_def_map = msc->import_def_at_entry(entry);
 Latency_map* entry_import_use_map = msc->import_use_at_entry(entry);

 for (Latency_map_iterator ki(*import_def_map); ki != 0; ki++) {
   bool is_swdef = false;
   Operand oper = (*ki).first;
   int avail_dangle = (*ki).second;
   // check if def refs to oper with exposed dangles
   reflist = oper2defmap.value(oper);
   if (!reflist->is_empty()) is_swdef = true;
   // check if use refs to oper with exposed dangles
   reflist = oper2usemap.value(oper);
   if (!reflist->is_empty()) is_swdef = true;
   // check if incoming dangle > available dangle
   int req_dangle = -1;
   if (entry_import_def_map->is_bound(oper)) {
     req_dangle = entry_import_def_map->value(oper);
   }
   if (entry_import_use_map->is_bound(oper)) {
     req_dangle = ELCOR_MAX (req_dangle, entry_import_use_map->value(oper));
   }
   if (req_dangle > avail_dangle) {
     is_swdef = true;
     redraw_edges = true;
   }
   if (is_swdef) swdeflist.add_tail(oper);
 }

// Reorder (in C0 order) those operations in oper2defmap and oper2usemap
// if op is above exit but op_etime > exit_op OR
// if op is below exit but op_etime < exit_op
 for (Map_iterator <Operand, List <Pair <Op*, Port_num> >* > mdi(oper2defmap);
	 mdi != 0; mdi++) {
   Operand oper = (*mdi).first;
   reflist = (*mdi).second;
// Assumption that there is no D_SWITCH def to start with and a new one
// has to be created
   for (List_iterator <Pair <Op*, Port_num> > li (*reflist); li != 0; li++)
   {
     Pair <Op*, Port_num> def = (*li);
     Op* op = def.first;
//     Port_num port_num = def.second;
     assert (!is_merge(op) && !is_switch(op));
     if (ops_before_exit.is_member(op) && 
       (early_map->value(exit_op) < early_map->value(op))) {
//	 + op->flow_time(DEST, port_num) - br_lat)) {
        move_op_below_branch_in_c0_order (exit_op, op, &ops_before_exit,
                                          early_map, region_sched);
     }
     if ((!ops_before_exit.is_member(op)) && 
          (early_map->value(exit_op) > early_map->value(op))) {
//	 + op->flow_time(DEST, port_num) - br_lat)) {
// This will never move branches above other branches because
//  without FRPs branches are sequentialized by C1 edges and
//	etime of branches are monotonically increasing in C0 order
// With FRPs we need to revisit this issue again if there is no
//  sequentialization of branches
          move_op_above_branch_in_c0_order (exit_op, op, &ops_before_exit,
                                                early_map);
     }
   }
 }
 for (Map_iterator <Operand, List <Pair <Op*, Port_num> >* > mui(oper2usemap);
	 mui != 0; mui++) {
   Operand oper = (*mui).first;
   reflist = (*mui).second;
   for (List_iterator <Pair <Op*, Port_num> > ji (*reflist); ji != 0; ji++)
   {
     Pair <Op*, Port_num> use = (*ji);
     Op* op = use.first;
//     Port_num port_num = use.second;
     assert (!is_merge(op) && !is_switch(op));
     if (ops_before_exit.is_member(op) && 
            (early_map->value(exit_op) <  early_map->value(op))) {
//	 + op->anti_time(SRC, port_num))) {
          move_op_below_branch_in_c0_order (exit_op, op, &ops_before_exit,
                                                early_map, region_sched);
     }
     if ((!ops_before_exit.is_member(op)) && 
            (early_map->value(exit_op) > early_map->value(op))) {
//		 + op->anti_time(SRC, port_num))) {
          move_op_above_branch_in_c0_order (exit_op, op, &ops_before_exit,
                                                early_map);
     }
   }
 }

  for (Map_iterator <Operand, List <Pair <Op*, Port_num> >* > mi(oper2defmap);
				 mi != 0; mi++) {
      reflist = (*mi).second;
      delete reflist;
  }
  for (Map_iterator <Operand, List <Pair <Op*, Port_num> >* > pi(oper2usemap);
				 pi != 0; pi++) {
      reflist = (*pi).second;
      delete reflist;
  }
  return (redraw_edges);
}


void set_exit_edge_latencies (Compound_region *region, Op *exit_op,
                                 Latency_map *def_constraints,
                                 Latency_map *use_constraints)
{
  bool  found_an_edge;
  Hash_map<Operand, Op*> def_map(hash_operand, 211);
  Hash_map<Operand, Op*> use_map(hash_operand, 211);
  Port_num  src_port, dest_port;

  if (El_ssched_meld_model == NO_MELD) return;

  // Meld is not done interprocedurally;
  // do not relax output constraints if exit is an rts.
  if (is_return (exit_op)) return;

  if (El_ssched_meld_model == FLOW_MELD) {
  // for FLOW MELD, outgoing constraints are only reset to 0
  // if all the successor regions are unscheduled and there is no back
  // edge from this exit onto this region.
  // Otherwise, the original latencies on the edges are used.
    bool constraints_found = false;
    for (Op_outedges out(exit_op, CONTROL0_OUTEDGES); out != 0; out++) {
      Op *next_entry = (*out)->dest();
      if (next_entry->parent()->flag(EL_REGION_SCHEDULED)
	|| (region_contains_op(region, next_entry)))
        constraints_found = true;
    }

    if (constraints_found) return;

    // reset all latencies associated with the exit op to 0, except those that
    //   have been constrained previously by an incoming constraint.
    for (List_iterator<Op*> related1(exit_op->get_relops());
         related1 != 0; related1++) {
      Op *switch_op = *related1;
      if (!is_switch (switch_op))
        El_punt ("SCHEDULER: set_exit_edge_latencies expected a switch op");

      found_an_edge = false;
      for (Op_inedges in(switch_op, SRC, switch_op->first_src());
           in != 0; in++) {
        Edge *curr_edge = *in;
        found_an_edge = true;
        if (!is_merge (curr_edge->src())) // reset latency only if it is
                                          // not constrained on the input.
          curr_edge->set_latency (0);
      }
      for (Op_inedges in1(switch_op, DEST, switch_op->first_dest());
           in1 != 0; in1++) {
        El_punt ("set_exit_edge_latencies found a SW-DEF on FLOW_MELD run.");
        Edge *curr_edge = *in1;
        found_an_edge = true;
        if (!is_merge (curr_edge->src())) // reset latency only if it is
                                          // not constrained on the input.
          curr_edge->set_latency (0);
      }
      if (!found_an_edge)
        El_punt ("SCHEDULER: set_exit_edge_latencies didn't find an edge.");
    }
  }

  else if (El_ssched_meld_model == FULL_MELD) {
    // determine minimum allowable dangle into this region.  this defaults to
    //   max unless the RTS_OPERAND was specified.  this value is only set if a 
    //   region containing an rts has been scheduled previously. 
    int max_def_relaxation_allowed = def_constraints->get_default_value();
    int max_use_relaxation_allowed = use_constraints->get_default_value();

    // Create def_map, mapping operands to associated switch defs
    //    and use_map, mapping operands to associated switch uses
    for (List_iterator<Op*> related(exit_op->get_relops());
         related != 0; related++) {
      Op *related_op = *related;
      if (!is_switch (related_op))
        El_punt ("SCHEDULER: set_exit_edge_latencies expected a switch op");
      if ((related_op->num_srcs() != 1) && (related_op->num_dests() != 1))
        El_punt ("SCHEDULER: set_exit_edge_latencies found wrong number of \
operands in a switch op.");

      int opers = 0;
      src_port  = related_op->first_src();
      dest_port = related_op->first_dest();
      if (!related_op->dest(dest_port).is_undefined ()) {
        def_map.bind (related_op->dest(dest_port), related_op);
        opers++;
      }
      if (!related_op->src(src_port).is_undefined ()) {
        use_map.bind (related_op->src(src_port), related_op);
        opers++;
      }
      if (! ( (opers == 1) ||
              ((opers == 0) && (related_op->num_mem_srcs() > 0))) ){
        cerr << "Region with incorrect operands on switch: " << endl;
        cerr << *related_op << endl;
        cerr << (*(related_op->parent())) << endl;
        assert ( (opers == 1) ||
              ((opers == 0) && (related_op->num_mem_srcs() > 0)));
      }
    }

    // iterate over switches at the region's exit, and relax the constraints if
    //   possible.  first, relax constraints based on defs.
    for (Hash_map_iterator<Operand, Op*> hi(def_map); hi != 0; hi++) {
      Operand curr_operand = (*hi).first;
      Op* switch_op = (*hi).second;
      found_an_edge = false;
      int necessary_dangle = def_constraints->value (curr_operand);
      // the only time the following statement will ever be true is whn
      //   a region with an RTS has been scheduled already and
      //   it constrains the allowable relaxation on the output.
      if (necessary_dangle > max_def_relaxation_allowed)
          necessary_dangle = max_def_relaxation_allowed;
	  
      for (Op_inedges in(switch_op, DEST, switch_op->first_dest()); in != 0; 
           in++) {
        Edge *curr_edge = *in;
        found_an_edge = true;

        int necessary_latency = curr_edge->latency() - necessary_dangle;
        if (necessary_latency <= 0)
          curr_edge->set_latency (0);
        else if (curr_edge->latency() > necessary_latency)
          curr_edge->set_latency (necessary_latency);

      }
    }

    //   next, relax constraints based on uses.
    for (Hash_map_iterator<Operand, Op*> hi1(use_map); hi1 != 0; hi1++) {
      Operand curr_operand = (*hi1).first;
      Op* switch_op = (*hi1).second;
      found_an_edge = false;
      
      int necessary_dangle = use_constraints->value (curr_operand);
      // the only time the following statement will ever be true is when an
      //   a region with an RTS has been scheduled already, and it constrains
      //   the allowable relaxation on the output.
      if (necessary_dangle > max_use_relaxation_allowed)
          necessary_dangle = max_use_relaxation_allowed;
      for (Op_inedges in(switch_op, SRC, switch_op->first_src()); in != 0; 
           in++) {
        Edge *curr_edge = *in;
        found_an_edge = true;

        int necessary_latency = curr_edge->latency() - necessary_dangle;

        if (necessary_latency <= 0)
          curr_edge->set_latency (0);
        else if (curr_edge->latency() > necessary_latency)
          curr_edge->set_latency (necessary_latency);
      }
    } 
  }

  else
    El_punt ("invalid meld_mode specified.");
}


void create_exit_latency_maps (Latency_map* pseudo_exit_def_map,
                               Latency_map* pseudo_exit_use_map,
                               int sched_time, MS_constraints* msc,
                               Op* exit_op, Op* entry_op)
{
  Latency_map *curr_exit_def_map = msc->def_at_exit (exit_op);
  Latency_map *curr_exit_use_map = msc->use_at_exit (exit_op);

  for (Latency_map_iterator mi(*pseudo_exit_def_map); mi != 0; mi++) {
    Operand curr_operand = (*mi).first;
    int value = (*mi).second;
    int true_value = value - sched_time;
    if ((true_value > 0) && (true_value <= MDES_max_op_lat)) {
      curr_exit_def_map->bind (curr_operand, true_value);
    }
  }

  for (Latency_map_iterator mi1(*pseudo_exit_use_map); mi1 != 0; mi1++) {
    Operand curr_operand = (*mi1).first;
    int value = (*mi1).second;
    int true_value = value - sched_time;
    if ((true_value > 0) && (true_value <= MDES_max_op_lat)) {
      curr_exit_use_map->bind (curr_operand, true_value);
    }
  }

  // set distance from entry to this exit.
  int op_distance = sched_time; // assuming that entry_op's sched_time is 0.
   // branch latency, if present, is taken into account by the calling routine.
  msc->set_min_distance (entry_op, exit_op, op_distance);
}


void set_export_latency_maps (Compound_region *region)
// assuming one entry and multiple exits.
// also, all registers in the function are included in the entry_use and
//   entry_def maps unless the MDES_max_op_lat value is exceeded.
// live-outs are not considered at all in the current approach.
{
  Latency_map  *entry_use_map, pseudo_exit_use_map, *entry_def_map,
               pseudo_exit_def_map;
  Map<Op*, int>  exit_map;
  MS_constraints  *msc;
  int  sched_time;

  if (!has_ms_constraints(region)) return;

  msc = get_ms_constraints (region);

  Region_entry_ops e_op (region);
  Op *entry_op = *e_op;
  // ensure that there is only one entrance into this region.
  e_op++;
  if (e_op != 0)
    El_punt ("More than one entry op found in regon.");

  // need to delete current entries in entry maps.  they were temporarily
  //   specified with etimes.  now the real values based on the schedule are
  //   used.
  entry_use_map = msc->use_at_entry (entry_op);
  msc->unbind_use_at_entry (entry_op);
  delete entry_use_map;
  entry_use_map = new Latency_map(MDES_max_op_lat);
  msc->set_use_at_entry (entry_op, entry_use_map);

  entry_def_map = msc->def_at_entry (entry_op);
  msc->unbind_def_at_entry (entry_op);
  delete entry_def_map;
  entry_def_map = new Latency_map(MDES_max_op_lat);
  msc->set_def_at_entry (entry_op, entry_def_map);
  int entry_default = MDES_max_op_lat;

  if (entry_op->sched_time() != 0)
    El_punt ("assuming that entry_op's sched_time is 0.");

  // iterate over all the ops in order to set the values of the latency maps.
  for (Region_subregions iter(region); iter != 0; iter++) {
    assert ((*iter)->is_op());
    Op *curr_op = (Op*) (*iter);
    Op* real = real_op(curr_op);
    if ((is_switch(curr_op)   || is_merge(curr_op)) &&
        ((!is_call(real))  && (!is_return(real)))) continue;
    sched_time = real->sched_time();

    // need to check exits that are pending to see if one of them is now
    //   applicable.  if it is, calculate the exit latency maps.
    List<Op*> exits_removed;
    for (Map_iterator<Op*, int> mi(exit_map); mi != 0; mi++) {
      int exit_end_time = (*mi).second;

      if (sched_time >= exit_end_time) {
        Op* exit_op = (*mi).first;
        create_exit_latency_maps (&pseudo_exit_def_map, &pseudo_exit_use_map,
                                  exit_end_time, msc, exit_op, entry_op);
        exits_removed.push (exit_op);
      }
    }
    while (!exits_removed.is_empty()) {
      Op* exit_op = exits_removed.pop();
      exit_map.unbind (exit_op);
    }

    // look at all source operands and set entry and exit use maps accordingly.
    for (Op_all_inputs ii (curr_op); ii != 0; ii++) {
      Operand curr_operand = (*ii);
      Port_num src_p = ii.get_port_num();
      if (!(curr_operand.is_reg() || curr_operand.is_macro_reg())) continue;
      int new_can_dangle = sched_time + curr_op->flow_time (SRC, src_p);
                      // = sched_time + earliest sample time
      if (new_can_dangle < entry_default) {
        if (!entry_use_map->is_bound (curr_operand) ||
            (entry_use_map->value (curr_operand) > new_can_dangle))
          entry_use_map->bind (curr_operand, new_can_dangle);
      }

      int new_need_to_dangle = sched_time + curr_op->anti_time (SRC, src_p);
                          // = sched_time + latest sample time
      if ((!pseudo_exit_use_map.is_bound (curr_operand)) ||
          (pseudo_exit_use_map.value (curr_operand) < new_need_to_dangle))
        pseudo_exit_use_map.bind (curr_operand, new_need_to_dangle);
    }

    // look at all dest operands and set entry and exit def maps accordingly.
    for (Op_all_dests oi (curr_op); oi != 0; oi++) {
      Operand curr_operand = (*oi);
      Port_num dest_p = oi.get_port_num();
      if (!(curr_operand.is_reg() || curr_operand.is_macro_reg())) continue;
      int new_can_dangle = sched_time + curr_op->anti_time (DEST, dest_p);
                  // = sched_time + earliest write time
      if (new_can_dangle < entry_default) {
        if (!entry_def_map->is_bound (curr_operand) ||
            (entry_def_map->value (curr_operand) > new_can_dangle)) {
          entry_def_map->bind (curr_operand, new_can_dangle);
	}
      }
      int new_need_to_dangle = sched_time + curr_op->flow_time (DEST, dest_p);
                          // = sched_time + latest write time
      if ((!pseudo_exit_def_map.is_bound (curr_operand)) ||
          (pseudo_exit_def_map.value (curr_operand) < new_need_to_dangle))
        pseudo_exit_def_map.bind (curr_operand, new_need_to_dangle);
    }

    if (region_op_at_exit (region, curr_op)) { // exit encountered, so translate
                         // pseudo_exit_use_map and pseudo_exit_def_map into the
                         // correct maps for this exit.  dangle_latency is 
                         // calculated by subtracting time in current 
                         // pseudo_exit_[def,use]_map from sched_time of 
                         // exit_op.
      // the following sets default_value to allowable dangle in the case where
      //   an RTS is present in this region.  this is done because latencies
      //   are not allowed to dangle outside of a function.
      if (is_return (curr_op)) {
        int new_can_dangle = sched_time + MDES_branch_latency();
        if (new_can_dangle < (entry_def_map->get_default_value())) {
          entry_def_map->set_default_value (new_can_dangle);
          entry_use_map->set_default_value (new_can_dangle);
	  entry_default = new_can_dangle;
	  for (Latency_map_iterator mi(*entry_def_map); mi != 0;) {
	    Pair<Operand, int> op_lat = (*mi);
	    mi++;
	    if (op_lat.second >= new_can_dangle)
	      entry_def_map->unbind(op_lat.first);
	  }
	  for (Latency_map_iterator ni(*entry_use_map); ni != 0;) {
	    Pair<Operand, int> op_lat = (*ni);
	    ni++;
	    if (op_lat.second >= new_can_dangle)
	      entry_use_map->unbind(op_lat.first);
	  }	  
        }
      }

      int time_for_resolution = sched_time + MDES_branch_latency();
      exit_map.bind (curr_op, time_for_resolution);
    }
  }

  // need to account for all exits that are still pending, even after all the
  //   ops in the region have been looked at.
  for (Map_iterator<Op*, int> mi(exit_map); mi != 0; mi++) {
    int exit_end_time = (*mi).second;
    Op* exit_op = (*mi).first;

    create_exit_latency_maps (&pseudo_exit_def_map, &pseudo_exit_use_map,
                              exit_end_time, msc, exit_op, entry_op);
  }
}


