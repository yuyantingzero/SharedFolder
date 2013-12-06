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
//
//      File:         core_edge_drawing.cpp
//      Authors:      Richard Johnson  
//      Created:      March 1995
//      Description:  Predicate-cognizant data dependence edge drawing 
//                    for regions.
//
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "connect.h"
#include "region.h"
#include "iterators.h"
#include "tuples.h"
#include "attributes.h"
#include "region_attributes.h"
#include "opcode_properties.h"
#include "string_class.h"
#include "intf.h"
#include "pqs.h"
#include "pred_interface.h"
#include "dfa.h"
#include "dbg.h"
#include "flow_analysis_solver.h"
#include "edge_drawing.h"
#include "core_edge_drawing.h"
#include "el_ssched_init.h"
#include "edge_utilities.h"
#include "edge.h"
#include "hash_map.h"
#include "hash_set.h"
#include "hash_functions.h"
#include "opcode.h"
#include "el_dot_tools.h"
#include "el_error.h"
#include "el_io.h"
#include "el_init.h"
#include "scalar_sched.h"
#include "connect.h"
#include "el_control.h"
#include "pred_analysis.h"

// Something is wrong. The following include doesn't work with CC. So declare 
// the prototype explicitly. Once I figure out what is the problem, I will 
// change it -- Vinod
//#include "intf.h"
bool MDES_has_speculative_version(char* opcode);

/*===========================================================================*/
/*
 *  The following functions are used to assist edge drawing after control
 *  CPR.   Currently due to some limitations of predicate analysis, 
 *  the predicates introduced by CPR cannot be analyzed properly, so it
 *  results in conservative edges being drawn.  These functions are
 *  used to make the edge drawing closer to what it should be.
 *  SAM 10-97
 */
/*===========================================================================*/
static Control_cpr_info *current_cpr_info = NULL;

// SAM 10-97, close your eyes, this is for control dependence edge drawing only,
// so make some simplifying assumptions.
bool is_disjoint_cpr_hack(Op *op1, Op *op2, Control_cpr_info *info)
{
  //return false;
    Operand pred1, pred2;
    int i, num_chains, p1_on_trace_index, p1_off_trace_index, p2_on_trace_index,
    p2_off_trace_index;

    if (info == NULL)
      return false;
    if (! (op1->predicated() && op2->predicated()))
      return false;

    // Find approximate guards of the ops
    if (is_brct(op1))
      pred1 = op1->src(SRC2);
    else
      pred1 = op1->src(PRED1);
    if (is_brct(op2))
      pred2 = op2->src(SRC2);
    else
      pred2 = op2->src(PRED1);

    if (pred1.is_reg() && pred2.is_reg()) {
      num_chains = info->num_chains;
      Vector<Operand> &on_preds = info->on_trace_preds;
      Vector<Operand> &off_preds = info->off_trace_preds;
        Vector<Hash_set<Operand> > &derived_on_preds = info->derived_on_trace_preds;
        Vector<Hash_set<Operand> > &derived_off_preds = info->derived_off_trace_preds;
      p1_on_trace_index = p1_off_trace_index = -1;
      p2_on_trace_index = p2_off_trace_index = -1;
      for (i=0; i<num_chains; i++) {
        if ((on_preds[i] == pred1) ||
            (derived_on_preds[i].is_member(pred1)))
          p1_on_trace_index = i;
        else if ((off_preds[i] == pred1) ||
            (derived_off_preds[i].is_member(pred1)))
        p1_off_trace_index = i;
        if ((on_preds[i] == pred2) ||
            (derived_on_preds[i].is_member(pred2)))
          p2_on_trace_index = i;
        else if ((off_preds[i] == pred2) ||
            (derived_off_preds[i].is_member(pred2)))
          p2_off_trace_index = i;
      }

      // Disj combos are on/off, off/on (off index <= on index), off/off (always when not the same chain)
      if ((p1_off_trace_index >= 0) && (p2_off_trace_index >= 0) && (p1_off_trace_index != p2_off_trace_index)) {
	//cout << "disjoint cpr hack returning true A:" << op1->id() << "," << op2->id() << endl;
          return (true);
      }
      else if ((p1_off_trace_index >= 0) && (p2_on_trace_index >=0)) {
          if (p1_off_trace_index <= p2_on_trace_index) {
	    //cout << "disjoint cpr hack returning true B:" << op1->id() << "," << op2->id() << endl;
	    return (true);
	  }
      }
      else if ((p1_on_trace_index >= 0) && (p2_off_trace_index >=0)) {
          if (p2_off_trace_index <= p1_on_trace_index) {
	    //cout << "disjoint cpr hack returning true C:" << op1->id() << "," << op2->id() << endl;
	    return (true);
	  }
      }

    }

    return (false);
}

/* Forward decl needed by the next function */
void control_dep_edge(Op *src_op, Op *dst_op);

// new version of control dependence edge drawing for CPR code
void
insert_intrablock_control_edges_cpr_hack(Compound_region* blk)
{
  // The new FRP control dep edge drawing code is here.  Note it does not
  // work for MODULO scheduling style edge now!!

  bool has_spec, can_be_spec;
  PQS* pqs = get_pqs (blk);
  Control_cpr_info *cpr_info = get_control_cpr_info(blk);
  List<Pair<Op*,Pred_expr*> > pc_list;
  List_iterator<Pair<Op*,Pred_expr*> > lpi;
  Op *last_br;

  //
  // forward control dependence (approximation)
  //
  Region_ops_C0_order opi(blk);
  Op* mg = *opi;    // get the top entry op
  assert (is_control_merge(mg));
  Pair<Op*,Pred_expr*> start_pair(mg, new Pred_expr(pqs->true_expr()));
  pc_list.add_tail(start_pair);
  last_br = mg;

  opi++;  // skip merge
  for (; opi != 0; ++opi) {
    assert ( (*opi)->is_op() );
    Op* op = (Op*) (*opi);

    // has_spec = MDES_has_speculative_version(el_opcode_to_string_map.value(op->opcode()));
    //tangw (06-03-02)
    has_spec = MDES_has_speculative_version(get_mdes_opcode_string(op));



    can_be_spec = ((El_arch_speculation_model == GENERAL_SPECULATION && has_spec) ||
                   (El_arch_speculation_model == RESTRICTED_SPECULATION &&
                    has_spec && !is_excepting_op(op)));

    if (can_be_spec) {
      control_dep_edge (mg, op);
    }

    // Branches define PC, so treat like predicated def
    else if (is_branch(op)) {
      last_br = op;
      Pred dest_gen = pqs->lub_branch_pred(op);
      Pred dest_kill = pqs->glb_branch_pred(op);
      Pair<Op*,Pred_expr*> ref(op, new Pred_expr(pqs->false_expr()));
      pqs->lub_sum(*ref.second, dest_gen);

      // process reaching output dependences
      lpi(pc_list);
      while (lpi != 0) {
        Pair<Op*,Pred_expr*> &def = *lpi;
        lpi++;
        if (! (pqs->is_disjoint(dest_gen, *(def.second)) ||
               is_disjoint_cpr_hack(def.first, op, cpr_info))) {
          control_dep_edge(def.first, op);
          // update old def's predicate expression
          pqs->lub_diff(*(def.second), dest_kill);
          if (pqs->is_false(*def.second)) {
            delete def.second;
            pc_list.remove(def);
          }
        }
      }
      pc_list.add_tail(ref);
    }

    // Just chain internal C_merges to the prior branch, doesn't really matter
    else if (is_control_merge(op)) {
      control_dep_edge(last_br, op);
    }

    // Normal non-speculative op
    else {
      lpi(pc_list);
      while (lpi != 0) {
        Pair<Op*,Pred_expr*> &def = *lpi;
        lpi++;
        // Hardwired src_guard to SRC1, this is hack, really want predicate of op
        Pred src_guard = pqs->lub_src_guard(op, SRC1);
        if (! (pqs->is_disjoint(src_guard, *(def.second)) ||
         is_disjoint_cpr_hack(def.first, op, cpr_info)))
          control_dep_edge (def.first, op);
      }
    }
  }

  pc_list.clear();

  //
  // control anti-dependence edges
  //   - In SCALAR and MELD edge drawing, connect all ops to last branch.
  //
  {
    Region_ops_reverse_C0_order opi(blk);
    Op* sw = *opi;
    assert (is_branch(sw));
    Pair<Op*,Pred_expr*> start_pair(sw, new Pred_expr(pqs->true_expr()));
    pc_list.add_tail(start_pair);
    opi++;
    for (; opi != 0; ++opi) {
      assert ( (*opi)->is_op() );
      Op* op = (Op*) (*opi);

      if (is_branch(op)) {
          Pred dest_gen = pqs->lub_branch_pred(op);
          Pred dest_kill = pqs->glb_branch_pred(op);
          Pair<Op*,Pred_expr*> ref(op, new Pred_expr(pqs->false_expr()));
          pqs->lub_sum(*ref.second, dest_gen);

          // process reaching output dependences
          lpi(pc_list);
          while (lpi != 0) {
            Pair<Op*,Pred_expr*> &def = *lpi;
            lpi++;
            if (! pqs->is_disjoint(dest_gen, *(def.second))) {
              // update old def's predicate expression
              pqs->lub_diff(*(def.second), dest_kill);
              if (pqs->is_false(*def.second)) {
                delete def.second;
                pc_list.remove(def);
              }
            }
          }
          pc_list.add_tail(ref);
      }
      else if (is_store(op) || is_brl(op)) {
        lpi(pc_list);
        while (lpi != 0) {
          Pair<Op*,Pred_expr*> &def = *lpi;
          lpi++;
          // Hardwired src_guard to SRC1, this is hack, really want predicate of op
          Pred src_guard = pqs->lub_src_guard(op, SRC1);
          if (! (pqs->is_disjoint(src_guard, *(def.second)) ||
              is_disjoint_cpr_hack(op, def.first, cpr_info)))
            control_dep_edge (op, def.first);
        }
      }

      // Put an edge between all ops and the last br
      control_dep_edge (op, sw);
    }
  }
}

/*==============================================================================*/
/*
 *  End control CPR related functions
 */
/*==============================================================================*/


// flag to indicate which edges get drawn
// Uses:
//   - SCALAR and MELD edge drawing supresses ALL intra-op edges,
//     including edges from a branch to its related switch ops.
//
//   - MODULO edge drawing threads branches but omits full C1 edges.
//
Edge_style edge_style = NO_EDGES;

void add_additional_edges(Compound_region* r);

// add data merge to agregate; merge has operand of associated VR
void
add_merge_to_control_merge(Op* ctl_mg, Op* new_mg)
{
   Compound_region* r = (Compound_region*) ctl_mg->parent() ;
   
   ctl_mg->add_relops(new_mg) ;
   new_mg->add_relops(ctl_mg) ;

   r->insert_immediately_after_region(new_mg, ctl_mg) ;
   new_mg->set_parent(r) ;
}

// add data switch to agregate; switch has operand of associated VR
void
add_switch_to_control_switch(Op* ctl_sw, Op* new_sw)
{
   Compound_region* r = (Compound_region*) ctl_sw->parent() ;

   ctl_sw->add_relops(new_sw) ;
   new_sw->add_relops(ctl_sw) ;

   r->insert_immediately_before_region(new_sw, ctl_sw) ;
   new_sw->set_parent(r) ;
}

// inserts sw def nodes at JSRs for each physically bound register used
// in the block.  This is required by postpass scheduling.
void
insert_block_jsr_sw_nodes (Compound_region* blk)
{
  assert (edge_style == SCALAR_POSTPASS_EDGES);

  VR_map* vr_map = get_local_vr_map (blk);
  if(vr_map == NULL) {
    create_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(blk));
    vr_map = get_local_vr_map (blk);
  }
  assert(vr_map != NULL);
  Vector<Operand>& index_to_vr = vr_map->index_to_vr;

  Region_entry_ops entry(blk);
  Op* entry_op = *entry;    // get the top entry op
  assert (is_control_merge(entry_op));

  for (Region_ops_C0_order opc(blk); opc != 0; ++opc) {
    Op* op = (Op*) (*opc);

    if (!is_brl(op)) continue;

    // add sw def node for each physically-bound vr in block
    for (int ii = 0; ii<vr_map->vr_count; ii++) {
      Operand& oper = index_to_vr[ii];

      // added check for is_macro_reg to ensure defs/uses of
      // macro regs are not moved across BRL's during
      // postpass sched. KF 8-2001

      if ((oper.is_reg() && oper.allocated()) ||
          oper.is_macro_reg()) {
        Op* new_sw = new Op(D_SWITCH,1,1,0) ;
        new_sw->dest(DEST1) = oper ;  // def
        add_switch_to_control_switch(op, new_sw) ;
      }
    }
  }
}

void
insert_block_sw_mg_nodes_scalar (Compound_region* blk)
{
  assert (edge_style == SCALAR_PREPASS_EDGES ||
    edge_style == SCALAR_POSTPASS_EDGES);

  VR_map* vr_map = get_local_vr_map (blk);
  if(vr_map == NULL) {
    create_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(blk));
    vr_map = get_local_vr_map (blk);
  }

  assert(vr_map != NULL);
  Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;
  Vector<Operand>& index_to_vr = vr_map->index_to_vr;

  Region_entry_ops entry(blk);
  Op* entry_op = *entry;    // get the top entry op
  assert (is_control_merge(entry_op));

  // added insertion of merges mchu 2-2003
  Hash_set<Operand> mg_defs(hash_operand, 1021);
  
  for (Op_inedges ie(entry_op, CONTROL0_INEDGES); ie != 0; ie++) {
    // get info for each edge (only exit edges have info)
    Edge* ed = (Edge*) (*ie);
    if (! blk->is_entry_edge(ed)) continue;   // Not really needed, but done anyway
    Liveness_info_iterator li ;
    Liveness_info* uu = get_liveness_info(ed);
    if (uu) {
      for (li(*uu) ; li != 0 ; li++) {
         Operand oper = *li ;
         if (vr_to_index.is_bound(oper)) {
           mg_defs += oper;
         }
      }
    }
    
  }
  
  Hash_set_iterator<Operand> mg;
  for (mg(mg_defs); mg != 0; mg++) {
    Operand oper = *mg;
    if (oper.is_mem_vr()) continue;
    Op* new_mg = new Op(D_MERGE,1,1,0) ;
    new_mg->dest(DEST1) = oper ;
    add_merge_to_control_merge(entry_op, new_mg) ;
  }
  
  // NOTES:
  // only flow deps for mem_vr's
  // no merge nodes
  // switch use: added if live out and locally referenced
    
  Op* last_op = get_last_region_op_from_subregions(blk);
  assert (is_branch(last_op));
  for (Region_exit_ops exit(blk); exit != 0; exit++) {
    Op* exit_op = *exit;
    assert (is_branch(exit_op));

    //
    // compute up/down exposed def/use sets at switch
    //
    Bitvector up_uses(vr_map->vr_count) ;

    for (Op_outedges oe(exit_op, CONTROL0_OUTEDGES); oe != 0; oe++) {
      // get info for each edge (only exit edges have info)
      Edge* ed = (Edge*) (*oe);
      if (! blk->is_exit_edge(ed)) continue;   // Not really needed, but done anyway
      Liveness_info_iterator li ;

      Liveness_info* uu = get_liveness_info(ed);
      if (uu) {
        for (li(*uu) ; li != 0 ; li++) {
          Operand oper = *li ;
          if (vr_to_index.is_bound(oper)) {
             up_uses.set_bit(vr_to_index.value(oper));
          }
        }
      }
    }
    
    // compute conditions for switch def/use nodes
    for (Bit_iterator sw(up_uses); sw != 0; sw++) {
      Operand oper = index_to_vr[*sw];

      Op* new_sw = new Op(D_SWITCH,1,1,0) ;
      if (oper.is_mem_vr()) {
        new_sw->add_mem_src(oper);
      } else {
        new_sw->src(SRC1) = oper ;  // use
      }
      add_switch_to_control_switch(exit_op, new_sw) ;
    }
  }

  if (edge_style == SCALAR_POSTPASS_EDGES) {
    insert_block_jsr_sw_nodes (blk);
  }
}


void
insert_block_sw_mg_nodes_global (Compound_region* blk)
{
  VR_map* vr_map = get_local_vr_map (blk);
  if(vr_map == NULL) {
    create_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(blk));
    vr_map = get_local_vr_map (blk);
  }
  assert(vr_map != NULL);

  Region_entry_ops entry(blk);
  Op* entry_op = *entry;    // get the top entry op
  assert (is_control_merge(entry_op));

  // added insertion of merges mchu 2-2003
  Hash_set<Operand> mg_defs(hash_operand, 1021);
  
  for (Op_inedges ie(entry_op, CONTROL0_INEDGES); ie != 0; ie++) {
    // get info for each edge (only exit edges have info)
    Edge* ed = (Edge*) (*ie);
    if (! blk->is_entry_edge(ed)) continue;   // Not really needed, but done anyway
    Liveness_info_iterator li ;
    Liveness_info* uu = get_liveness_info(ed);
    if (uu) {
      for (li(*uu) ; li != 0 ; li++) {
         Operand oper = *li ;
         mg_defs += oper;
      }
    }
    
  }
  
  Hash_set_iterator<Operand> mg;
  for (mg(mg_defs); mg != 0; mg++) {
    Operand oper = *mg;
    if (oper.is_mem_vr()) continue;
    Op* new_mg = new Op(D_MERGE,1,1,0) ;
    new_mg->dest(DEST1) = oper ;
    add_merge_to_control_merge(entry_op, new_mg) ;
  }
  
  // NOTES:
  // only flow deps for mem_vr's
  // no merge nodes
  // switch use: added if live out and locally referenced
    
  Op* last_op = get_last_region_op_from_subregions(blk);
  assert (is_branch(last_op));
  for (Region_exit_ops exit(blk); exit != 0; exit++) {
    Op* exit_op = *exit;
    assert (is_branch(exit_op));

    // added insertion of merges mchu 8-2005
    Hash_set<Operand> switches(hash_operand, 1021);

    //
    // compute up/down exposed def/use sets at switch
    //

    for (Op_outedges oe(exit_op, CONTROL0_OUTEDGES); oe != 0; oe++) {
      // get info for each edge (only exit edges have info)
      Edge* ed = (Edge*) (*oe);
      if (! blk->is_exit_edge(ed)) continue;   // Not really needed, but done anyway
      Liveness_info_iterator li ;

      Liveness_info* uu = get_liveness_info(ed);
      if (uu) {
        for (li(*uu) ; li != 0 ; li++) {
          Operand oper = *li ;
          switches += oper;
        }
      }
    }

    Hash_set_iterator<Operand> sw;
    for (sw(switches); sw != 0; sw++) {
      Operand oper = *sw;
      if (oper.is_mem_vr()) continue;
      Op* new_sw = new Op(D_SWITCH,1,1,0) ;
      new_sw->src(SRC1) = oper ;

      add_switch_to_control_switch(exit_op, new_sw) ;
    }
    
  }

}

void insert_block_sw_mg_nodes_modulo (Compound_region* blk,
         Hash_set<Operand>* s)
{
  assert (edge_style == MODULO_EDGES);

  Region_entry_ops entry(blk);
  Op* entry_op = *entry;    // get the top entry op
  assert (is_control_merge(entry_op));

  //
  // compute up/down exposed def/use sets at merge
  //
  Hash_set<Operand> dn_defs(hash_operand, 1021);
  Hash_set<Operand> dn_uses(hash_operand, 1021);
  Hash_set<Operand> up_defs(hash_operand, 1021);
  Hash_set<Operand> up_uses(hash_operand, 1021);

  // NOTES:
  // only flow deps for mem_vr's
  //
  // mg def: added if s ^ dn_defs ^ (up_uses + up_defs)
  // mg use: added if s ^ dn_uses ^ up_defs;
  //
  // sw def: added if s ^ dn_defs ^ up_defs
  // sw use: added if s ^ up_uses
    
  for (Op_inedges oi(entry_op, CONTROL0_OUTEDGES); oi != 0; oi++) {
    // get info for each edge
    Edge* ed = (Edge*) (*oi);
    Liveness_info_iterator li ;

    Liveness_info* dd = get_down_exposed_defs_info(ed);
    if (dd) {
      for (li(*dd) ; li != 0 ; li++) {
        Operand oper = *li ;
        dn_defs += oper;
      }
    }
    Liveness_info* du = get_down_exposed_uses_info(ed);
    if (du) {
      for (li(*du) ; li != 0 ; li++) {
        Operand oper = *li ;
        dn_uses += oper;
      }
    }
    Liveness_info* ud = get_up_exposed_defs_info(ed);
    if (ud) {
      for (li(*ud) ; li != 0 ; li++) {
        Operand oper = *li ;
        up_defs += oper;
      }
    }
    Liveness_info* uu = get_liveness_info(ed);
    if (uu) {
      for (li(*uu) ; li != 0 ; li++) {
        Operand oper = *li ;
        up_uses += oper;
      }
    }
  }

  Hash_set<Operand> mg_defs(hash_operand, 1021);
  Hash_set<Operand> mg_uses(hash_operand, 1021);

  // compute conditions for merge def/use nodes

  // - def if dd^(uu+ud)
  mg_defs  = up_uses;
  mg_defs += up_defs;
  mg_defs *= dn_defs;
  mg_defs *= *s;

  // - use if du^ud
  mg_uses  = dn_uses;
  mg_uses *= up_defs;
  mg_uses *= *s;

  Hash_set_iterator<Operand> mg;
  for (mg(mg_defs); mg != 0; mg++) {
    Operand oper = *mg;
    Op* new_mg = new Op(D_MERGE,1,1,0) ;
    if (oper.is_mem_vr()) {
      new_mg->add_mem_dest(oper);
    } else {
      new_mg->dest(DEST1) = oper ;  // def
    }
    add_merge_to_control_merge(entry_op, new_mg) ;
  }

  for (mg(mg_uses); mg != 0; mg++) {
    Operand oper = *mg;
    if (oper.is_mem_vr()) continue;  // only flow deps for mem_vr's

    Op* new_mg = new Op(D_MERGE,1,1,0) ;
    new_mg->src(SRC1) = oper ;  // use
    add_merge_to_control_merge(entry_op, new_mg) ;
  }

  Op* last_op = get_last_region_op_from_subregions(blk);
  assert (is_branch(last_op));
  for (Region_exit_ops exit(blk); exit != 0; exit++) {
    Op* exit_op = *exit;
    assert (is_branch(exit_op));

    //
    // compute up/down exposed def/use sets at switch
    //
    Hash_set<Operand> dn_defs(hash_operand, 1021);
    Hash_set<Operand> dn_uses(hash_operand, 1021);
    Hash_set<Operand> up_defs(hash_operand, 1021);
    Hash_set<Operand> up_uses(hash_operand, 1021);

    for (Op_outedges oe(exit_op, CONTROL0_OUTEDGES); oe != 0; oe++) {
      // get info for each edge (only exit edges have info)
      Edge* ed = (Edge*) (*oe);
      Liveness_info_iterator li ;

      Liveness_info* dd = get_down_exposed_defs_info(ed);
      if (dd) {
        for (li(*dd) ; li != 0 ; li++) {
          Operand oper = *li ;
          dn_defs += oper;
        }
      }
      Liveness_info* du = get_down_exposed_uses_info(ed);
      if (du) {
        for (li(*du) ; li != 0 ; li++) {
          Operand oper = *li ;
          dn_uses += oper;
        }
      }
      Liveness_info* ud = get_up_exposed_defs_info(ed);
      if (ud) {
        for (li(*ud) ; li != 0 ; li++) {
          Operand oper = *li ;
          up_defs += oper;
        }
      }
      Liveness_info* uu = get_liveness_info(ed);
      if (uu) {
        for (li(*uu) ; li != 0 ; li++) {
          Operand oper = *li ;
          up_uses += oper;
        }
      }
    }
    
    Hash_set<Operand> sw_defs(hash_operand, 1021);
    Hash_set<Operand> sw_uses(hash_operand, 1021);
    
    // compute conditions for switch def/use nodes
    
    // - def if ud^dd (output)
    sw_defs  = up_defs;
    sw_defs *= dn_defs;
    sw_defs *= *s;

    // - use if uu^dd (flow)  what about anti???
    //  okay, for now we'll insert use switches if uu (ie. no intersection)
    sw_uses  = up_uses;
    sw_uses *= *s;

    Hash_set_iterator<Operand> sw;
    for (sw(sw_defs); sw != 0; sw++) {
      Operand oper = *sw;

      if (oper.is_mem_vr()) continue;  // only flow deps for mem_vr's

      Op* new_sw = new Op(D_SWITCH,1,1,0) ;
      new_sw->dest(DEST1) = oper ;  // def
      add_switch_to_control_switch(exit_op, new_sw) ;
    }

    for (sw(sw_uses); sw != 0; sw++) {
      Operand oper = *sw;

      Op* new_sw = new Op(D_SWITCH,1,1,0) ;
      if (oper.is_mem_vr()) {
        new_sw->add_mem_src(oper);
      } else {
        new_sw->src(SRC1) = oper ;  // use
      }
      add_switch_to_control_switch(exit_op, new_sw) ;
    }
  }
}

// - convert up/down-exposed def/use info at boundary into sw/mg nodes
void
insert_region_sw_mg_nodes (Compound_region* r, Hash_set<Operand>* s)
{
  if (r->is_bb() || r->is_hb()) {
    if (edge_style == SCALAR_PREPASS_EDGES) {
      insert_block_sw_mg_nodes_scalar (r);
    } else if (edge_style == SCALAR_POSTPASS_EDGES) {
      insert_block_sw_mg_nodes_scalar (r);
    } else if (edge_style == MODULO_EDGES) {
      insert_block_sw_mg_nodes_modulo (r, s);
    } else if (edge_style == GLOBAL_ANALYSIS_EDGES) {
      insert_block_sw_mg_nodes_global (r);
    } else {
      El_punt("unknown edge drawing model.");
    }
  } else {
    for (Region_subregions sub(r); sub != 0; sub++) {
      Compound_region* sr = (Compound_region*)(*sub);
      insert_region_sw_mg_nodes (sr, s);
    }
  }
}

void
delete_region_sw_mg_nodes (Compound_region* r)
{
  if (r->is_bb() || r->is_hb()) {
    for (Region_ops_linear opi(r); opi != 0;) {
      Op* op = (Op*) (*opi);
      opi++;

      // Delete switch and merge nodes
      if (is_control_switch(op) || is_control_merge(op)) {
        op->remove_all_relops() ;
      }

      if (!is_switch(op) && !is_merge(op)) continue;
      
      r->remove_region(op);
      Op_inedges ei (op);
      Op_outedges eo (op);
      if((ei != 0) || (eo != 0)) {
	cdbg << "ERROR: removing a switch/merge node that still has edges!\n"
	     << "  This usually means that you removed ops from a block that \n"
	     << "had edges drawn, but you didn't adjust all the edges correctly \n"
	     << "before calling delete_region_edges. One example when this error \n"
	     << "appears is when you insert an op at the wrong level (e.g. putting \n"
	     << "it in a procedure instead of a block." << endl;
      }
      if(ei != 0) {
	Edge* ed = *ei;
	cdbg << "Edge from " << ed->src()->id() << ":" << ed->src()->opcode()
	     << " to " << ed->dest()->id() << ":" << ed->dest()->opcode()
	     << endl;
      }
      if(eo != 0) {
	Edge* ed = *eo;
	cdbg << "Edge from " << ed->src()->id() << ":" << ed->src()->opcode()
	     << " to " << ed->dest()->id() << ":" << ed->dest()->opcode()
	     << endl;
      }
      assert (ei == 0);
      assert (eo == 0);
      
      delete op;
    }
  } else {
    for (Region_subregions sub(r); sub != 0; sub++) {
      Compound_region* sr = (Compound_region*)(*sub);
      delete_region_sw_mg_nodes (sr);
    }
  }
}

bool
is_intra_op_edge(Op* src_op, Op* dst_op)
{
  // is it even a candidate intra-op edge?
  if (real_op (src_op) == real_op (dst_op)) {
    // SCALAR and MELD edge drawing never allows intra-op edges
    if (edge_style == SCALAR_PREPASS_EDGES ||
        edge_style == SCALAR_POSTPASS_EDGES ||
        edge_style == MELD_EDGES) {
      return true;
    } else {
      // MODULO edge drawing allows control_switch->switch edges
      if (is_control_switch(src_op) && is_switch(dst_op))
        return false;
      else
        return true;
    }
  }
  return false;
}

void
set_edge_latency(Edge* ed, Edge_style es)
{

  Op* src_op = ed->src();
  Op* dest_op = ed->dest();
  Port_num src_p = ed->src_port();
  Port_num dest_p = ed->dest_port();

  if (ed->is_control()) {
    int lat = src_op->flow_time(CONTROL, src_p, OUT) - 
      dest_op->flow_time(CONTROL, dest_p, IN);
    ed->set_latency(lat);
  } else if (ed->is_reg_flow()) {
    int lat = src_op->flow_time(DEST, src_p) - 
      dest_op->flow_time(SRC, dest_p);
    ed->set_latency(lat);
  } else if (ed->is_reg_anti()) {
    int lat = src_op->anti_time(SRC, src_p) - 
      dest_op->anti_time(DEST, dest_p);
    ed->set_latency(lat);
  } else if (ed->is_reg_out()) {
    int lat = src_op->flow_time(DEST, src_p) - 
      dest_op->anti_time(DEST, dest_p);
    ed->set_latency(lat);
  } else if (ed->is_mem()) {
    int lat = src_op->flow_time(MEM, src_p, OUT) - 
      dest_op->flow_time(MEM, dest_p, IN);
    ed->set_latency(lat);
  } else {
    // this should never be called now!  delete when removing all
    // edge->set_latency() method functions
    ed->set_latency();
    assert(0);
  }

  if (es == ES_MODULO) {
    int lat = adjust_edge_latency_modulo(ed);
    ed->set_latency(lat);     
  }
  else {

    int lat = adjust_edge_latency_scalar(ed);

    if ((lat < 0) && (! El_ssched_allow_negative_latencies))
      lat = 0;
    ed->set_latency(lat);     


    // CPR HACK - SAM 2-98, set outgoing latency on all initialization
    // cmpps to 0 to approximate them not being there.
#if 0
    if (El_do_scalar_control_cpr) {
      Op* src_op = ed->src();
      if (is_cmpp(src_op)) {
        Operand &src1 = src_op->src(SRC1);
        Operand &src2 = src_op->src(SRC2);
        if (src1.is_int() && src2.is_int())
          ed->set_latency(0);
      }
    }
#endif
    // END CPR HACK - SAM 2-98
  }
}

void
control_dep_edge(Op *src_op, Op *dst_op)
{
  src_op = real_op(src_op);
  dst_op = real_op(dst_op);

  if (is_intra_op_edge(src_op, dst_op)) return;
  if (is_disjoint_cpr_hack(src_op, dst_op, current_cpr_info)) return;

  for (Op_outedges ced(src_op, CONTROL1_OUTEDGES); ced != 0; ced++) {
    Edge* ed = *ced;
    if (ed->src() == src_op && ed->dest() == dst_op) return;
  }

  Control* ctl = new Control();
  connect_ops(ctl, src_op, CONTROL1, dst_op, CONTROL1, 0, 0);
  set_edge_latency((Edge *)ctl, edge_style);

  if (dbg(sed))
    cdbg << "Control1 " << src_op->id() << " -> " << dst_op->id() << endl;
}

void
mem_edge(Op *src_op, Op *dst_op,
	 Alt_num src_alt, Alt_num dest_alt )
{
  if (is_intra_op_edge(src_op, dst_op)) return;
  if (is_disjoint_cpr_hack(src_op, dst_op, current_cpr_info)) return;

  Mem* mem = new Mem();
  connect_ops(mem, src_op, MEM0, dst_op, MEM0, src_alt, dest_alt);
  set_edge_latency((Edge *)mem, edge_style);

  if (dbg(sed))
    cdbg << "Mem " << src_op->id() << " -> " << dst_op->id() << endl;
}

void
reg_flow_edge(Op* src_op, Port_num src_port, Op* dst_op, Port_num dst_port, 
        Alt_num src_alt, Alt_num dest_alt)
{
  if (is_intra_op_edge(src_op, dst_op)) return;
  if (is_disjoint_cpr_hack(src_op, dst_op, current_cpr_info)) return;

  Reg_flow* flow = new Reg_flow();
  connect_ops(flow, src_op, src_port, dst_op, dst_port, src_alt, dest_alt);
  set_edge_latency((Edge *)flow, edge_style);

  if (dbg(sed))
    cdbg << "Reg_flow (" << src_op->id() << ", " << (int) src_port
         << ") -> (" << dst_op->id() << ", " << (int) dst_port
         << ")" << endl;
  if (El_ssched_print_elcorid_edges)
  cout << "  CAR ID & latency: (" << src_op->id() << ", " << (int) src_port
          << ") -> (" << dst_op->id() << ", " << (int) dst_port << ")    "
          << ((Edge *)flow)->latency() << endl;
}

void
reg_anti_edge(Op* src_op, Port_num src_port, Op* dst_op, Port_num dst_port,
        Alt_num src_alt, Alt_num dest_alt)
{
  if (is_intra_op_edge(src_op, dst_op)) return;
  if (is_disjoint_cpr_hack(src_op, dst_op, current_cpr_info)) return;

  Reg_anti* anti = new Reg_anti();
  connect_ops(anti, src_op, src_port, dst_op, dst_port, src_alt, dest_alt);
  set_edge_latency((Edge *)anti, edge_style);

  if (dbg(sed))
    cdbg << "Reg_anti (" << src_op->id() << ", " << (int) src_port
         << ") -> (" << dst_op->id() << ", " << (int) dst_port
         << ")" << endl;
  if (El_ssched_print_elcorid_edges)
  cout << "  CAR ID & latency: (" << src_op->id() << ", " << (int) src_port
       << ") -> (" << dst_op->id() << ", " << (int) dst_port << ")    "
       << ((Edge *)anti)->latency() << endl;
  }

void
reg_out_edge(Op* src_op, Port_num src_port, Op* dst_op, Port_num dst_port,
	     Alt_num src_alt, Alt_num dest_alt)
{
  if (is_intra_op_edge(src_op, dst_op)) return;
  if (is_disjoint_cpr_hack(src_op, dst_op, current_cpr_info)) return;

  Reg_out* out = new Reg_out();
  connect_ops(out, src_op, src_port, dst_op, dst_port, src_alt, dest_alt);
  set_edge_latency((Edge *)out, edge_style);

  // SAM 10-97, This is ugly, but couldn't think of a better soln.  Need to
  // make sure U->O and U->A output edges are latency 1, otherwise they
  // incorrectly get scheduling in the same cycle.
  if (is_cmpp(src_op) && is_cmpp(dst_op)) {
    if (is_O_pred(src_op, src_port) && is_O_pred(dst_op, dst_port))
      El_warn("reg_out_edge: adding unnec output dep between %d and %d",
		src_op->id(), dst_op->id());
    else if (is_A_pred(src_op, src_port) && is_A_pred(dst_op, dst_port))
      El_warn("reg_out_edge: adding unnec output dep between %d and %d",
		src_op->id(), dst_op->id());
    else {
      int lat = ((Edge *)out)->latency();
      lat = (lat >= 1 ? lat : 1);
      set_edge_latency((Edge *)out, edge_style);
    }
  }

  if (dbg(sed))
    cdbg << "Reg_out (" << src_op->id() << ", " << (int) src_port
	 << ") -> (" << dst_op->id() << ", " << (int) dst_port
	 << ")" << endl;
  if (El_ssched_print_elcorid_edges)
  cout << "	CAR ID & latency: (" << src_op->id() << ", " << (int) src_port
    << ") -> (" << dst_op->id() << ", " << (int) dst_port << ")		"
    << ((Edge *)out)->latency() << endl;
}

void
insert_intrablock_control_edges(Compound_region* blk)
{
  //
  // forward control dependence (approximation)
  //   - for MODULO edge drawing, only thread branches.
  //   - otherwise, connect all ops to previous branch.
  //
  Region_ops_C0_order opi(blk);
  Op* mg = *opi;    // get the top entry op
  assert (is_control_merge(mg));
  opi++;  // skip merge

  Op* sw = mg;
  for (; opi != 0; ++opi) {
    assert ( (*opi)->is_op() );
    Op* op = (Op*) (*opi);
    
    bool has_spec =
      MDES_has_speculative_version(get_mdes_opcode_string(op));

    if (   (El_arch_speculation_model == GENERAL_SPECULATION && has_spec)
	|| (El_arch_speculation_model == RESTRICTED_SPECULATION &&
	    has_spec && !is_excepting_op(op))) {
      if (edge_style == SCALAR_PREPASS_EDGES ||
	  edge_style == SCALAR_POSTPASS_EDGES ||
	  edge_style == MELD_EDGES) {
	control_dep_edge (mg, op);
      }
    } else {
      control_dep_edge (sw, op);
    }
    
    if (is_branch(op)) {
      sw = op;
    }
  }

  //
  // control anti-dependence edges
  //   - In SCALAR and MELD edge drawing, connect all ops to last branch.
  //
  if (edge_style == SCALAR_PREPASS_EDGES ||
      edge_style == SCALAR_POSTPASS_EDGES ||
      edge_style == MELD_EDGES) {
    Region_ops_reverse_C0_order opi(blk);
    Op* sw = *opi;
    Op* mrb = sw ;   // most recent branch
    assert (is_branch(sw));
    opi++;
    for (; opi != 0; ++opi) {
      assert ( (*opi)->is_op() );
      Op* op = (Op*) (*opi);
      if (is_branch(op)) mrb = op ;
      if (is_branch(op) || op == mg) continue;
      if (is_store(op) || is_brl(op)) control_dep_edge(op,mrb) ; // pin the store above the branch
      control_dep_edge (op, sw);
    }
  }
}



// conservative edge drawing for blocks
//
// - scalar and memory virtual-register edges
//
// - edges only for VRs referenced in region or live-at-exit
//    ie. no edges for pass-through VRs
//
void
insert_intrablock_edges (Compound_region* blk)
{
  PQS* pqs = get_pqs (blk);

  // - create data flow sets for uses and defs.  Each set contains
  //   <Operand,Op*,Port_num,predicate expression> tuples.
  List<Quad<Operand,Op*,Port_num,Pred_expr*> > def_list;
  List<Quad<Operand,Op*,Port_num,Pred_expr*> > use_list;
  List_iterator<Quad<Operand,Op*,Port_num,Pred_expr*> > lpi;

  if (El_do_scalar_control_cpr) {
    current_cpr_info = get_control_cpr_info(blk);
    // insert control (C1) edges
    insert_intrablock_control_edges_cpr_hack (blk);
  }
  else {
    current_cpr_info = NULL;
    // insert control (C1) edges
    insert_intrablock_control_edges (blk);
  }

  // make list of ops "properly" ordered for edge drawing
  // - specifically, use-switches, control switch, def-switches.
  // traverse ops in hyperblock in forward linear order

  int op_count = 0  ;
  for (Region_all_ops ropi(blk) ; ropi != 0 ; ropi++) {
     op_count++ ;
  }

  Op** oplist = new Op*[op_count] ;

  int cur_op_loc = 0 ;
  
  for (Region_ops_C0_order opc(blk); opc != 0; ++opc) {
     Op* op = (Op*) (*opc);

     if (is_control_merge(op)) {
	// add defs, uses, then control  merge
	List_iterator<Op*> mgi ;
	for (mgi(op->get_relops()); mgi != 0; mgi++) {
	   Op *mg = *mgi;
	   Op_all_dests opdef(mg);
	   Op_mem_dests opmdef(mg);
	   if (opmdef != 0 ||
	       (opdef != 0 && !((*opdef).is_undefined()))) {
	      oplist[cur_op_loc] = mg;
	      cur_op_loc++ ;
	   }
	}
	for (mgi(op->get_relops()); mgi != 0; mgi++) {
	   Op *mg = *mgi;
	   Op_all_inputs opuse(mg);
	   Op_mem_sources opmuse(mg);
	   if (opmuse != 0 ||
	       (opuse != 0 && !((*opuse).is_undefined()))) {
	      oplist[cur_op_loc] = mg;
	      cur_op_loc++ ;
	   }
	}
	oplist[cur_op_loc] = op;
	cur_op_loc++ ;
     } else if (is_control_switch(op)) {
	// add branch, use switches, then def switches
	oplist[cur_op_loc] = op;
	cur_op_loc++ ;
	List_iterator<Op*> swi ;
	for (swi(op->get_relops()); swi != 0; swi++) {
	   Op *sw = *swi;
	   Op_all_inputs opuse(sw);
	   Op_mem_sources opmuse(sw);
	   if (opmuse != 0 ||
	       (opuse != 0 && !((*opuse).is_undefined()))) {
	      oplist[cur_op_loc] = sw;
	      cur_op_loc++ ;
	   }
	}
	for (swi(op->get_relops()); swi != 0; swi++) {
	   Op *sw = *swi;
	   Op_all_dests opdef(sw);
	   Op_mem_dests opmdef(sw);
	   if (opmdef != 0 ||
	       (opdef != 0 && !((*opdef).is_undefined()))) {
	      oplist[cur_op_loc] = sw;
	      cur_op_loc++ ;
	   }
	}
     } else {
	oplist[cur_op_loc] = op;
	cur_op_loc++ ;
     }
  }

  int op_count_max = cur_op_loc ;

  // traverse ops in hyperblock in forward linear order
  cur_op_loc = 0 ;
  for (cur_op_loc = 0 ; cur_op_loc < op_count_max ; cur_op_loc++) {
    Op* op = oplist[cur_op_loc] ;
    
    //
    //                REGISTER DEPENDENCE THREADING
    //

    // apply remap to operands in use/def lists
    if (is_remap(op)) {
      // get VR_name
      Operand& vr = op->src(SRC1);
      assert (vr.is_vr_name());

      for (lpi(def_list); lpi != 0; lpi++) {
	Quad<Operand,Op*,Port_num,Pred_expr*>& def = *lpi;
	Operand& oper = def.first;

        if (!(oper.is_reg())) continue;

	if (oper.vr_num()==vr.vr_num() && 
	    oper.dtype()==vr.dtype()) {
	  oper.incr_omega(1);
	}
      }
      for (lpi(use_list); lpi != 0; lpi++) {
	Quad<Operand,Op*,Port_num,Pred_expr*>& use = *lpi;
	Operand& oper = use.first;

        if (!(oper.is_reg())) continue;

	if (oper.vr_num()==vr.vr_num() && 
	    oper.dtype()==vr.dtype()) {
	  oper.incr_omega(1);
	}
      }
      continue;
    }

    // process uses
    for (Op_all_inputs src_oper(op) ; src_oper != 0 ; src_oper++) {
      Operand& oper = (*src_oper) ;
      Port_num pn = src_oper.get_port_num();
      Pred src_guard = pqs->lub_src_guard(op,pn);

      if (!(oper.is_reg() || oper.is_macro_reg()))
	continue;
	
      // create tuple for this reference
      Quad<Operand,Op*,Port_num,Pred_expr*> ref (oper,op,pn,new Pred_expr(pqs->false_expr()));
      pqs->lub_sum (*(ref.fourth), src_guard);
	
      // process reaching flow dependences
      for (lpi(def_list); lpi != 0; lpi++) {
	Quad<Operand,Op*,Port_num,Pred_expr*> def = *lpi;

	// suppress flow deps from sw def nodes
	if (is_switch(def.second)) continue;

	if (def.first == oper &&
	    !pqs->is_disjoint(src_guard, *(def.fourth))) {
	  // create dependence from def.op to current op
	  reg_flow_edge(def.second, def.third, op, pn);

	  // if flow edge is to a switch def node, insert C1 edge to branch
	  // to inforce corresponding issue constraint for op above branch.
	  if (is_switch(op)) {
	    control_dep_edge(def.second, op);
	  }
	}
      }
      use_list.add_tail (ref);
    }

    // process mem_vr uses
    // - mem_vr's are used under the ops guarding predicate, so
    //   get guard for SRC1 operand.
    for (Op_mem_sources opm(op); opm != 0; opm++) {
      Operand& oper = (*opm);
      Port_num pn = opm.get_port_num();
      assert (pn == MEM0);
      Pred src_guard = pqs->lub_src_guard(op,SRC1);

      // process reaching flow dependences
      for (lpi(def_list); lpi != 0; lpi++) {
	Quad<Operand,Op*,Port_num,Pred_expr*>& def = *lpi;

	// suppress mem flow deps between switch nodes
	if (is_switch(def.second) && is_switch(op)) continue;

	if (def.first == oper &&
	    !pqs->is_disjoint(src_guard, *(def.fourth))) {

	  // if src is a sw def (ie. up-exposed def), create control dep
	  if (is_switch(def.second)) {
	    control_dep_edge(def.second, op);
	  } else {
	    // create dependence from def.op to current op
	    mem_edge(def.second, op);
	  }
	}
      }
    }

    // process defs
    for (Op_all_dests dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
      Operand oper = (*dest_oper) ;
      Port_num pn = dest_oper.get_port_num();
      Pred dest_gen  = pqs->lub_dest_guard (op,pn); // predicate for def 'use'
      Pred dest_kill = pqs->glb_dest_guard (op,pn); // predicate for def kill

      if (!oper.is_reg() && !oper.is_macro_reg())
	continue;
	
      // create quad for this reference
      Quad<Operand,Op*,Port_num,Pred_expr*> ref (oper,op,pn,new Pred_expr(pqs->false_expr()));
      pqs->lub_sum (*(ref.fourth), dest_gen);
	
      // process reaching output dependences
      lpi(def_list);
      while (lpi != 0) {
	Quad<Operand,Op*,Port_num,Pred_expr*>& def = *lpi;
	lpi++;

	// suppress output deps between switch nodes
	if (is_switch(def.second) && is_switch(op)) continue;

	// suppress output deps between or-style cmpp ops
	// HACK!  This can be cleaned up by improving dest_guard.
	// if (is_O_pred(def.second, def.third) && is_O_pred(op, pn)) continue;

	if (def.first == oper &&
	    !pqs->is_disjoint(dest_gen, *(def.fourth))) {

	  // if src is a sw def (ie. up-exposed def), create control dep
	  // from branch node to op ( note that sw def doesn't kill! )
	  if (is_switch(def.second)) {
	    control_dep_edge(def.second, op);
	  } else {
	    // create output dependence from def.op to current op
	    reg_out_edge(def.second, def.third, op, pn);

	    // if output edge is to a switch def, insert C1 edge to branch
	    // to inforce corresponding issue constraint for op above branch.
	    if (is_switch(op)) {
	      control_dep_edge(def.second, op);
	    }

	    // or-style cmpp's don't kill
	    // HACK!  This hack was needed because of bug in analysis
	    // if (is_O_pred(op, pn)) continue;

	    // off-trace defs (switch defs) don't kill
	    if (is_switch(op)) continue;

	    // sanity check: should never see a merge def!
	    assert (!is_merge(op));

	    // update old def's predicate expression
	    pqs->lub_diff(*(def.fourth), dest_kill);
	    if (pqs->is_false(*def.fourth)) {
	      delete def.fourth;
	      def_list.remove(def);
	    }
	  }
	}
      }
	
      // process reaching anti dependences
      lpi(use_list);
      while (lpi != 0) {
	Quad<Operand,Op*,Port_num,Pred_expr*>& use = *lpi;
	lpi++;

	// suppress anti deps between switch nodes
	if (is_switch(use.second) && is_switch(op)) continue;

	if (use.first == oper &&
	    !pqs->is_disjoint(dest_gen, *(use.fourth))) {

	  // if source is a switch use (ie. live-out), create control dep
	  // from branch to op.
	  if (is_switch(use.second)) {
	    control_dep_edge(use.second, op);
	  } else {
	    // create anti dependence from use.op to current op
	    reg_anti_edge(use.second, use.third, op, pn);
	    
	    // if anti edge is to a switch def, insert C1 edge to branch
	    // to inforce corresponding issue constraint for op above branch.
	    if (is_switch(op)) {
	      control_dep_edge(use.second, op);
	    }

	    // or-style cmpp's don't kill, same for and-style
	    if (is_O_pred(op, pn)) continue;
	    if (is_A_pred(op, pn)) continue;

	    // off-trace defs (switch defs) don't kill
	    if (is_switch(op)) continue;

	    // sanity check: should never see a merge def!
	    assert (!is_merge(op));

	    // update old def's predicate expression
	    pqs->lub_diff(*(use.fourth), dest_kill);
	    if (pqs->is_false(*use.fourth)) {
	      delete use.fourth;
	      use_list.remove(use);
	    }
	  }
	}
      }
	
      // add new def to def_list
      def_list.add_tail (ref);
    }

    // process mem_vr defs
    // - mem_vr's are defined under the ops guarding predicate, so
    //   get guard for DEST1 operand.
    for (Op_mem_dests opmd(op); opmd != 0; opmd++) {
      Port_num pn = opmd.get_port_num();
      assert (pn == MEM0);
      // get predicates for definition's gen and kill
      Pred dest_gen  = pqs->lub_dest_guard (op,DEST1);
      Pred dest_kill = pqs->glb_dest_guard (op,DEST1);

      // create quad for this reference
      //Quad<Operand,Op*,Port_num,Pred_expr*> ref (oper,op,MEM0,new Pred_expr(pqs->false_expr()));
      //pqs->lub_sum (*(ref.fourth), dest_gen);

      // actually, don't need to new a Pred_expr. KF 5-2003
      Pred_expr pe(pqs->false_expr());
      pqs->lub_sum (pe, dest_gen);
    }

    // process mem_vr kills
    for (Op_mem_kills opmk(op); opmk != 0; opmk++) {
      Operand& oper = (*opmk);
      Port_num pn = opmk.get_port_num();
      // get predicates for definition's gen and kill
      Pred dest_gen  = pqs->lub_dest_guard (op,pn);
      Pred dest_kill = pqs->glb_dest_guard (op,pn);
      
      // create quad for this reference
      Quad<Operand,Op*,Port_num,Pred_expr*> ref (oper,op,MEM0,new Pred_expr(pqs->false_expr()));
      pqs->lub_sum (*(ref.fourth), dest_gen);

      // process kills to mem_vr defs
      for (lpi(def_list); lpi != 0; lpi++) {
	Quad<Operand,Op*,Port_num,Pred_expr*>& def = *lpi;

	if (def.first == oper &&
	    !pqs->is_disjoint(dest_gen, *(def.fourth))) {
	  // update def's predicate expression
	  pqs->lub_diff(*(def.fourth), dest_kill);
	  if (pqs->is_false(*def.fourth)) {
	    delete def.fourth;
	    def_list.remove(def);
	  }
	}
      }

      // add new def to def_list
      def_list.add_tail (ref);
    }
  }

  delete[] oplist;

  // hack -- analysis and edge drawing will be upgraded to subsume this!
  add_additional_edges(blk);

  // destroy bitvectors stored in lists
  for (lpi(def_list); lpi != 0; lpi++) {
    Quad<Operand,Op*,Port_num,Pred_expr*>& ref = *lpi;
    delete ref.fourth;
  }
  for (lpi(use_list); lpi != 0; lpi++) {
    Quad<Operand,Op*,Port_num,Pred_expr*>& ref = *lpi;
    delete ref.fourth;
  }

  if (dbg(sed, 4)) {
    el_dot_display_dfg(blk);
  }
}


// TOTALLY NEW EDGE DRAWING
//
// conservative edge drawing for blocks
//
// - scalar and memory virtual-register edges
//
// - edges only for VRs referenced in region or live-at-exit
//    ie. no edges for pass-through VRs
//
void
insert_intrablock_edges_gen2 (Compound_region* blk)
{

   if (El_do_scalar_control_cpr) {
      current_cpr_info = get_control_cpr_info(blk);
      // insert control (C1) edges
      insert_intrablock_control_edges_cpr_hack (blk);
   }
   else {
      current_cpr_info = NULL;
      // insert control (C1) edges
      insert_intrablock_control_edges (blk);
   }

   // make list of ops "properly" ordered for edge drawing
   // - specifically, use-switches, control switch, def-switches.
   // traverse ops in hyperblock in forward linear order
   
   int op_count = 0  ;
   for (Region_all_ops ropi(blk) ; ropi != 0 ; ropi++) {
      op_count++ ;
   }
   
   Op** oplist = new Op*[op_count] ;
   
   int cur_op_loc = 0 ;
   
   for (Region_ops_C0_order opc(blk); opc != 0; ++opc) {
      Op* op = (Op*) (*opc);
      
      if (is_control_merge(op)) {
         // add defs, uses, then control  merge
         List_iterator<Op*> mgi ;
         for (mgi(op->get_relops()); mgi != 0; mgi++) {
            Op *mg = *mgi;
            Op_all_dests opdef(mg);
            Op_mem_dests opmdef(mg);
            if (opmdef != 0 ||
               (opdef != 0 && !((*opdef).is_undefined()))) {
               oplist[cur_op_loc] = mg;
               cur_op_loc++ ;
            }
         }
         for (mgi(op->get_relops()); mgi != 0; mgi++) {
            Op *mg = *mgi;
            Op_all_inputs opuse(mg);
            Op_mem_sources opmuse(mg);
            if (opmuse != 0 ||
               (opuse != 0 && !((*opuse).is_undefined()))) {
               oplist[cur_op_loc] = mg;
               cur_op_loc++ ;
            }
         }
         oplist[cur_op_loc] = op;
         cur_op_loc++ ;
      } else if (is_control_switch(op)) {
         // add branch, use switches, then def switches
         oplist[cur_op_loc] = op;
         cur_op_loc++ ;
         List_iterator<Op*> swi ;
         for (swi(op->get_relops()); swi != 0; swi++) {
            Op *sw = *swi;
            Op_all_inputs opuse(sw);
            Op_mem_sources opmuse(sw);
            if (opmuse != 0 ||
               (opuse != 0 && !((*opuse).is_undefined()))) {
               oplist[cur_op_loc] = sw;
               cur_op_loc++ ;
            }
         }
         for (swi(op->get_relops()); swi != 0; swi++) {
            Op *sw = *swi;
            Op_all_dests opdef(sw);
            Op_mem_dests opmdef(sw);
            if (opmdef != 0 ||
               (opdef != 0 && !((*opdef).is_undefined()))) {
               oplist[cur_op_loc] = sw;
               cur_op_loc++ ;
            }
         }
      } else {
         oplist[cur_op_loc] = op;
         cur_op_loc++ ;
      }
   }
   
   int op_count_max = cur_op_loc ;
   
   //
   // Beginning of new edge drawing code
   //
   // 1) Create reference maps
   //    def_to_i_map
   //    i_to_def_map
   //    use_to_i_map
   //    i_to_use_map
   //
   //    operand_to_i_map
   //    i_to_operand_map
   //
   //    def_to_operand_map        int-domain
   //    use_to_operand_map        int-domain
   //
   // 2) create remap maps int-to-List<int>
   //
   
   Hash_map<El_ref,int>   def_to_i_map(hash_El_ref, op_count_max);
   Vector<El_ref>         i_to_def_map;
   
   Hash_map<El_ref,int>   use_to_i_map(hash_El_ref, op_count_max);
   Vector<El_ref>         i_to_use_map;
   
   Hash_map<Operand,int>  operand_map(hash_operand, op_count_max);
   Vector<Operand>        rev_operand_map;
   
   Hash_map<int,int>      def_to_operand_map(hash_int, op_count_max); // Def_int to Operand_int
   Vector<List<int> >     operand_to_def_map; // Operand_int to Def_int (list)
   
   Hash_map<int,int>      use_to_operand_map(hash_int, op_count_max); // Use_int to Operand_int
   Vector<List<int> >     operand_to_use_map; // Operand_int to Use_int (list)
   
   Hash_map_iterator<Operand,int> mdi;
   Hash_map_iterator<El_ref,int> edi;

   int operand_counter = 1 ;
   int def_counter = 1 ;
   int use_counter = 1 ;
   
   Region_all_ops op_iter;
   
   //
   // Create the operand map
   // 

   for (op_iter (blk); op_iter != 0; op_iter++) {
      Op* cur_op = *op_iter;

      for (Op_complete_inputs src_oper1 (cur_op); src_oper1 != 0; src_oper1++) {
         Operand& tmpo = *src_oper1;

         if (tmpo.is_reg() || tmpo.is_mem_vr() || tmpo.is_macro_reg()) {
	    if (!operand_map.is_bound (tmpo)) {
	       operand_map.bind (tmpo, operand_counter);
	       operand_counter++;
	    }
         }
      }
      for (Op_complete_dests dest_oper1 (cur_op); dest_oper1!=0; dest_oper1++) {
         Operand& tmpo = *dest_oper1;

         if (tmpo.is_reg() || tmpo.is_mem_vr() || tmpo.is_macro_reg()) {
	    if (!operand_map.is_bound (tmpo)) {
	       operand_map.bind (tmpo, operand_counter);
	       operand_counter++;
	    }
         }
      }
   }


   //
   // Create the definition map
   //
   for (op_iter (blk); op_iter != 0; op_iter++) {
      Op* cur_op = *op_iter;

      for (Op_complete_dests dest_oper2 (cur_op); dest_oper2 != 0; dest_oper2++){
         Operand& cur_oprnd = *dest_oper2;

         El_ref new_def = dest_oper2.get_ref() ;
         if (cur_oprnd.is_reg() || cur_oprnd.is_mem_vr() || cur_oprnd.is_macro_reg()) {
      	    if (!def_to_i_map.is_bound (new_def)) {
               def_to_i_map.bind (new_def, def_counter);
               def_counter++;
	    }
	 }
      }
   }

   //
   // Create the use map
   //
   for (op_iter (blk); op_iter != 0; op_iter++) {
      Op* cur_op = *op_iter;

      for (Op_complete_inputs src_oper2(cur_op); src_oper2 != 0; src_oper2++) {
	 Operand& cur_oprnd = *src_oper2;
	 El_ref new_use = src_oper2.get_ref() ;
	 if (cur_oprnd.is_reg() || cur_oprnd.is_macro_reg()) {
	    if (!use_to_i_map.is_bound(new_use)) {
	       use_to_i_map.bind(new_use, use_counter);
	       use_counter++;
	    }
	 }
      }
   }
   //
   // Create the reverse operand map
   //
   rev_operand_map.resize (operand_counter);
   for (mdi (operand_map); mdi != 0; mdi++)
      if ((*mdi).second > 0)
      rev_operand_map[(*mdi).second] = (*mdi).first;
   
   //
   // create the reverse definition map:
   //
   i_to_def_map.resize (def_counter);
   for (edi (def_to_i_map); edi != 0; edi++)
      if ((*edi).second > 0)
      i_to_def_map[(*edi).second] = (*edi).first;
   
   //
   // create the reverse use map:
   //
   i_to_use_map.resize (use_counter);
   for (edi (use_to_i_map); edi != 0; edi++)
      if ((*edi).second > 0)
      i_to_use_map[(*edi).second] = (*edi).first;
   
   //
   // create the def-int to operand-int and
   // operand-int to def-int-list maps:
   //
   operand_to_def_map.resize (operand_counter);
   for (edi (def_to_i_map); edi != 0; edi++) {
      if ((*edi).second > 0) {
         int opint = operand_map.value ((*edi).first.get_operand());
         def_to_operand_map.bind ((*edi).second, opint);
         operand_to_def_map[opint].add_tail ((*edi).second);
      }
   }
   
   //
   // create the use-int to operand-int and
   // operand-int to use-int-list maps:
   //
   operand_to_use_map.resize (operand_counter);
   for (edi (use_to_i_map); edi != 0; edi++) {
      if ((*edi).second > 0) {
         int opint = operand_map.value ((*edi).first.get_operand());
         use_to_operand_map.bind ((*edi).second, opint);
         operand_to_use_map[opint].add_tail ((*edi).second);
      }
   }

   
   // - create data flow sets for uses and defs.  Each set contains
   //   <Operand,Op*,Port_num,predicate expression> tuples.

   Pred_jar pj(blk) ;
   Pred_cookie fpq = Pred_jar::get_false() ;
   Pred_cookie tpq = Pred_jar::get_true() ;

   Vector<Pred_cookie> pc_defs(def_counter, fpq) ;
   Vector<Pred_cookie> pc_uses(use_counter, fpq) ;
   
   List_iterator<int> li1, li2 ;
   
   // traverse ops in hyperblock in forward linear order
   cur_op_loc = 0 ;
   for (cur_op_loc = 0 ; cur_op_loc < op_count_max ; cur_op_loc++) {
      Op* op = oplist[cur_op_loc] ;
      
      //
      //                REGISTER DEPENDENCE THREADING
      //
      
      // apply remap to operands in use/def lists
      if (is_remap(op)) {
         assert(0) ;
      }
      
      // process uses
      for (Op_all_inputs src_oper(op) ; src_oper != 0 ; src_oper++) {
         Operand& oper = (*src_oper) ;
         if (!(oper.is_reg() || oper.is_macro_reg()))
            continue;
         
         El_ref useref = src_oper.get_ref() ;
         int use_i = use_to_i_map.value(useref) ;
         int oper_i = operand_map.value(oper) ;
         Pred_cookie src_guard = pj.get_lub_guard(useref);
         pc_uses[use_i] = src_guard ;
         
         // process reaching flow dependences
         
         for (li1(operand_to_def_map[oper_i]) ; li1 != 0; li1++) {
            int cur_def_i = *li1 ;
            El_ref& cur_def = i_to_def_map[cur_def_i] ;  
            Op* cur_def_op = cur_def.get_op() ;
            // suppress flow deps from sw def nodes
            if (is_switch(cur_def_op)) continue;
            
            if (!pc_defs[cur_def_i].is_disjoint(src_guard)) {
               // create dependence from def.op to current op
               reg_flow_edge(cur_def_op, cur_def.get_port_num(), op, useref.get_port_num());
               
               // if flow edge is to a switch def node, insert C1 edge to branch
               // to inforce corresponding issue constraint for op above branch.
               if (is_switch(op)) {
                  control_dep_edge(cur_def_op, op);
               }
            }
         }
      }
      
      // process mem_vr uses
      // - mem_vr's are used under the ops guarding predicate, so
      //   get guard for SRC1 operand.
      for (Op_mem_sources opm(op); opm != 0; opm++) {
         Operand& oper = (*opm);

         El_ref useref = opm.get_ref() ;
         int oper_i = operand_map.value(oper) ;
         Pred_cookie src_guard = pj.get_lub_guard(useref);
         
         // process reaching flow dependences
         for (li1(operand_to_def_map[oper_i]) ; li1 != 0; li1++) {
            int cur_def_i = *li1 ;
            El_ref& cur_def = i_to_def_map[cur_def_i] ;  
            Op* cur_def_op = cur_def.get_op() ;
            
            // suppress mem flow deps between switch nodes
            if (is_switch(cur_def_op) && is_switch(op)) continue;
            
            if (!pc_defs[cur_def_i].is_disjoint(src_guard)) {
               
               // if src is a sw def (ie. up-exposed def), create control dep
               if (is_switch(cur_def_op)) {
                  control_dep_edge(cur_def_op, op);
               } else {
                  // create dependence from def.op to current op
                  mem_edge(cur_def_op, op);
               }
            }
            // SAM 5-04
            // Ugly hack needed for memvr profiled code.  The problem is
            // that profiling is not predicate cognizant, it only marks
            // a dependence to the the nearest prior ld/st that accesses
            // the same address.  If the prior ld/st happens to be disjoint,
            // while there is no dependence to that, the transitive dependence
            // to an earlier ld/st to the same address is missed, creating
            // illegal code.  By making disjoint ops dependent, this covers
            // up the problem.  In reality, one should find the nearest prior
            // ld/st that is both non-disjoint and accesses the same addr.
            // However, this is non-trivial to do with the profile info.
            else if ((El_memvr_profiled) && (pc_defs[cur_def_i] != fpq)) {
               // if src is a sw def (ie. up-exposed def), create control dep
               if (is_switch(cur_def_op)) {
                  control_dep_edge(cur_def_op, op);
               } else {
                  // create dependence from def.op to current op
                  mem_edge(cur_def_op, op);
               }
            }
         }
      }
      
      // process defs
      for (Op_all_dests dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
         Operand& oper = *dest_oper ;
         if (!oper.is_reg() && !oper.is_macro_reg())
            continue;
         
         El_ref def_ref = dest_oper.get_ref() ;
         int def_i = def_to_i_map.value(def_ref) ;
         int oper_i = operand_map.value(oper) ;
         Pred_cookie dest_gen  = pj.get_lub_guard (def_ref); // predicate for def 'use'
         Pred_cookie dest_kill = pj.get_glb_guard (def_ref); // predicate for def kill
         
         // process reaching output dependences
         for (li1(operand_to_def_map[oper_i]) ; li1 != 0; li1++) {
            int cur_def_i = *li1 ;
            if (cur_def_i == def_i) continue ;

            El_ref& cur_def = i_to_def_map[cur_def_i] ;
            Op* cur_def_op = cur_def.get_op() ;
            if (is_switch(cur_def_op) && is_switch(op)) continue;

            // suppress output deps between switch nodes
            
            if (!pc_defs[cur_def_i].is_disjoint(dest_gen)) {
               
               // if src is a sw def (ie. up-exposed def), create control dep
               // from branch node to op ( note that sw def doesn't kill! )
               if (is_switch(cur_def_op)) {
                  control_dep_edge(cur_def_op, op);

               // SAM 10-97, dont want output deps between OR or AND style
               // cmpps which target the same reg
               // Also, OR,AND defs don't kill other defs
               } else if ((is_O_pred(cur_def_op, cur_def.get_port_num()) &&
			   is_O_pred(op, def_ref.get_port_num())) ||
			  (is_A_pred(cur_def_op, cur_def.get_port_num()) &&
			   is_A_pred(op, def_ref.get_port_num()))) {
                  continue;

               } else {
                  // create output dependence from def.op to current op
                  reg_out_edge(cur_def_op, cur_def.get_port_num(), op, def_ref.get_port_num());
                  
                  // if output edge is to a switch def, insert C1 edge to branch
                  // to inforce corresponding issue constraint for op above branch.
                  if (is_switch(op)) {
                     control_dep_edge(cur_def_op, op);
                  }
                  
                  // off-trace defs (switch defs) don't kill
                  if (is_switch(op)) continue;
                  
                  // sanity check: should never see a merge def!
                  // assert (!is_merge(op));
                  
                  // update old def's predicate expression
                  pc_defs[cur_def_i].lub_diff(dest_kill);
               }
            }
         }
         	
         // process reaching anti dependences
         for (li1(operand_to_use_map[oper_i]) ; li1 != 0; li1++) {
            int cur_use_i = *li1 ;

            El_ref& cur_use = i_to_use_map[cur_use_i] ;
            Op* cur_use_op = cur_use.get_op() ;
            // suppress anti deps between switch nodes
            if (!pc_uses[cur_use_i].is_disjoint(dest_gen)) {
               
               // if source is a switch use (ie. live-out), create control dep
               // from branch to op.
               if (is_switch(cur_use_op)) {
                  control_dep_edge(cur_use_op, op);
               } else {
                  Port_num pn = def_ref.get_port_num() ;
                  // create anti dependence from use.op to current op
                  reg_anti_edge(cur_use_op, cur_use.get_port_num(), op, pn);
                  
                  // if anti edge is to a switch def, insert C1 edge to branch
                  // to inforce corresponding issue constraint for op above branch.
                  if (is_switch(op)) {
                     control_dep_edge(cur_use_op, op);
                  }
                  
                  // or-style cmpp's don't kill, same for and-style
                  if (is_O_pred(op, pn)) continue;
                  if (is_A_pred(op, pn)) continue;
                  
                  // off-trace defs (switch defs) don't kill
                  if (is_switch(op)) continue;
                  
                  // sanity check: should never see a merge def!
                  assert (!is_merge(op));
                  
                  // update old def's predicate expression
                  pc_uses[cur_use_i].lub_diff(dest_kill);
               }
            }
         }
         // add new def to def_list
         pc_defs[def_i] = dest_gen ;
      }
      
      // process mem_vr defs
      // - mem_vr's are defined under the ops guarding predicate, so
      //   get guard for DEST1 operand.
      
      // process mem_vr kills
      for (Op_mem_kills opmk(op); opmk != 0; opmk++) {
         Operand& oper = *opmk;
         
         // get predicate for definition's kill
         El_ref def_ref = opmk.get_ref() ;
         int oper_i = operand_map.value(oper) ;       // shouldn't there be 
         // a kill to i map ?
         Pred_cookie dest_kill  = pj.get_glb_guard(def_ref);
         
         // process kills to mem_vr defs
         for (li1(operand_to_def_map[oper_i]) ; li1 != 0; li1++) {
            int cur_def_i = *li1 ;
            
            pc_defs[cur_def_i].lub_diff(dest_kill);
         }
      }
      for (Op_mem_dests opmd(op); opmd != 0; opmd++) {
         // get predicate for definition's gen
         El_ref def_ref = opmd.get_ref() ;
         int def_i = def_to_i_map.value(def_ref) ;         
         Pred_cookie dest_gen  = pj.get_lub_guard(def_ref);
         
         pc_defs[def_i] = dest_gen ;
      }

   }

   delete [] oplist;

   // hack -- analysis and edge drawing will be upgraded to subsume this!
   add_additional_edges(blk);
      
   if (dbg(sed, 4)) {
      el_dot_display_dfg(blk);
   }
}

void
insert_region_intrablock_edges (Compound_region* r)
{
  if (r->is_bb() || r->is_hb()) {
    insert_intrablock_edges (r);
  } else {
    // draw edges for each subregion
    for (Region_subregions sub(r); sub != 0; sub++) {
      Compound_region* subrgn = (Compound_region*) (*sub);
      insert_region_intrablock_edges (subrgn);
    }
  }
}

void
insert_region_intrablock_edges_gen2 (Compound_region* r)
{
  if (r->is_bb() || r->is_hb()) {
    insert_intrablock_edges_gen2 (r);
  } else {
    // draw edges for each subregion
    for (Region_subregions sub(r); sub != 0; sub++) {
      Compound_region* subrgn = (Compound_region*) (*sub);
      insert_region_intrablock_edges_gen2 (subrgn);
    }
  }
}


//-----------------------------------------------------------------------------
void
insert_interblock_edges (Edge* e)
{
  Op* s = e->src();
  Op* m = e->dest();
  assert (is_branch(s) && is_control_merge(m));
  Hash_map<Operand, Op*> sw_use(hash_operand, 1021) ;
  Hash_map<Operand, Op*> sw_def(hash_operand, 1021) ;
  Hash_map<Operand, Op*> sw_mem_use(hash_operand, 1021) ;
  Hash_map<Operand, Op*> sw_mem_def(hash_operand, 1021) ;

  // traverse relops, connect use@sw to def@mg (flow edges)
  for (List_iterator<Op*> swi(s->get_relops()); swi != 0; swi++) {
    Op *sw = *swi;
    if (!(sw->src(SRC1).is_undefined())) {
      assert (sw->dest(DEST1).is_undefined());
      sw_use.bind(sw->src(SRC1), sw);
    } else if (!(sw->dest(DEST1).is_undefined())) {
      assert (sw->src(SRC1).is_undefined());
      sw_def.bind(sw->dest(DEST1), sw);
    }
    // mem uses
    for (Op_mem_sources opmu(sw); opmu != 0; opmu++) {
      Operand& oper = (*opmu);
      sw_mem_use.bind(oper, sw);
    }
  }
  for (List_iterator<Op*> mgi(m->get_relops()); mgi != 0; mgi++) {
    Op *mg = *mgi;

    // flow
    if (!mg->dest(DEST1).is_undefined() && sw_use.is_bound(mg->dest(DEST1))) {
      assert (mg->src(SRC1).is_undefined());
      Op *sw = sw_use.value(mg->dest(DEST1));
      reg_flow_edge(sw, SRC1, mg, DEST1, e->src_alt(), e->dest_alt());
    }

    // output
    if (!mg->dest(DEST1).is_undefined() && sw_def.is_bound(mg->dest(DEST1))) {
      assert (mg->src(SRC1).is_undefined());
      Op *sw = sw_def.value(mg->dest(DEST1));
      reg_out_edge(sw, DEST1, mg, DEST1, e->src_alt(), e->dest_alt());
    }

    // anti
    if (!mg->src(SRC1).is_undefined() && sw_def.is_bound(mg->src(SRC1))) {
      assert (mg->dest(DEST1).is_undefined());
      Op *sw = sw_def.value(mg->src(SRC1));
      reg_anti_edge(sw, DEST1, mg, SRC1, e->src_alt(), e->dest_alt());
    }

    // mem flow
    for (Op_mem_dests opmd(mg); opmd != 0; opmd++) {
      Operand& oper = (*opmd);
      if (sw_mem_use.is_bound(oper)) {
	Op *sw = sw_mem_use.value(oper);
	mem_edge(sw, mg, e->src_alt(), e->dest_alt());
      }
    }
  }
}

void
insert_region_interblock_edges (Compound_region* r, Compound_region* root)
{
  if (r->is_bb() || r->is_hb()) {
    // for all interblock C0 edges within root, draw dependence edges
    for (Region_exit_edges edi(r); edi != 0; edi++) {
      Edge* ed = *edi;

      if (region_contains_edge (root, ed)) {
	insert_interblock_edges(ed);
      }
    }
  } else {
    // draw edges for each subregion
    for (Region_subregions sub(r); sub != 0; sub++) {
      Compound_region* subrgn = (Compound_region*)(*sub);
      insert_region_interblock_edges (subrgn, root);
    }
  }
}

void
get_region_vr_set (Compound_region* r, Hash_set<Operand>* s)
{
  if (r->is_bb() || r->is_hb()) {
    // add operands from block's VR_map to operand hash_set
    VR_map* vr_map = get_local_vr_map (r);
    if(vr_map == NULL) {
      create_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(r));
      vr_map = get_local_vr_map (r);
    }
    assert(vr_map != NULL);
    Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;

    for (Hash_map_iterator<Operand, int> mi(vr_to_index); mi != 0; mi++) {
      *s += (*mi).first;
    }
  } else {
    // collect operands from subregions
    for (Region_subregions sub(r); sub != 0; sub++) {
      Compound_region* subrgn = (Compound_region*)(*sub);
      get_region_vr_set (subrgn, s);
    }
  }
}

//-----------------------------------------------------------------------------
// assumes a subregion tiling of BBs/HBs
void
insert_region_scalar_edges (Compound_region* r, El_Scalar_Sched_Phase phase)
{
  if (dbg(sed)) cdbg << "SED: Region " << r->id() << endl;

  if (phase == EL_SCALAR_SCHED_POSTPASS) {
    edge_style = SCALAR_POSTPASS_EDGES;
  } else {
    edge_style = SCALAR_PREPASS_EDGES;
  }
  insert_region_sw_mg_nodes (r);
  insert_region_intrablock_edges_gen2 (r);
  insert_region_interblock_edges (r, r);

  update_edges_for_op_chaining(r);
}

// assumes a subregion tiling of BBs/HBs
void
insert_region_modulo_edges (Compound_region* r)
{
  if (dbg(sed)) cdbg << "SED: Region " << r->id() << endl;

  Hash_set<Operand> s(hash_operand, 16333);
  get_region_vr_set (r, &s);

  edge_style = MODULO_EDGES;
  insert_region_sw_mg_nodes (r, &s);
  insert_region_intrablock_edges (r);
  insert_region_interblock_edges (r, r);

	//cdbg << *r;

  update_edges_for_op_chaining(r);
}


// assumes a subregion tiling of BBs/HBs
void
insert_region_global_edges (Compound_region* r)
{
  edge_style = GLOBAL_ANALYSIS_EDGES;
  insert_region_sw_mg_nodes (r);
  insert_region_intrablock_edges_gen2 (r);
  insert_region_interblock_edges (r, r);

  update_edges_for_op_chaining(r);
}
// assumes a subregion tiling of BBs/HBs
void
delete_region_edges (Compound_region* r)
{
   Op_inedges inei ;
   Op_outedges outei ;

   for (Region_all_ops opi(r) ; opi != 0 ; opi++) {
      Op* cur_op = *opi ;

      outei(cur_op) ;
      while(outei != 0) {
         Edge* edge = *outei;
         outei++ ;
         if (edge->is_seq()) continue ;
         if (region_contains_edge(r, edge)) {
            Op* dest = edge->dest();
            cur_op->remove_outedge(edge, edge->src_port_type(),
               edge->src_port(), edge->src_alt());
            dest->remove_inedge(edge, edge->dest_port_type(),
               edge->dest_port(), edge->dest_alt());
            delete edge;

         }
      }
   }
   delete_region_sw_mg_nodes (r);
}

// Generally this should NOT be called.  However, if you are deleting/moving
// an op from an edge-drawn region, it is useful.
void
delete_op_edges (Op *op)
{
   Op_inedges inei ;
   Op_outedges outei ;
   Edge *edge;
   Op *dest, *src;

    outei(op);
    while (outei != 0) {
	edge = *outei;
	outei++ ;
	if (edge->is_seq()) continue ;
	dest = edge->dest();
	op->remove_outedge(edge, edge->src_port_type(),
               edge->src_port(), edge->src_alt());
	dest->remove_inedge(edge, edge->dest_port_type(),
		edge->dest_port(), edge->dest_alt());
	delete edge;
    }

    inei(op);
    while (inei != 0) {
	edge = *inei;
	inei++;
	if (edge->is_seq()) continue ;
	src = edge->src();
	op->remove_inedge(edge, edge->dest_port_type(),
               edge->dest_port(), edge->dest_alt());
	src->remove_outedge(edge, edge->src_port_type(),
                edge->src_port(), edge->src_alt());
    }

    // Delete switch and merge nodes
    if (is_control_switch(op) || is_control_merge(op)) {
      op->remove_all_relops() ;
    }
}

// TEMP HACK! (called from scalar scheduler)
// Add an edge from pred_clear to pred_set
void
add_additional_edges(Compound_region* r){
    Op* pred_clear_op = NULL;
    Op* pred_set_op = NULL;
    Op* one;
    Region_ops_linear ops(r);
    // Find pred_clear op 
    for( ; ops != 0; ops++){
	one = *ops;
	if(is_clear_all_rotating(one)) {
	    pred_clear_op = one;
	    break;
	}
    }
    if (pred_clear_op != NULL) {
	for( ; ops != 0; ops++){
	   one = *ops;
	   if(is_predicate_set(one)){
	    pred_set_op = one;
	    break;	   
	       
	   }
	}
	if (pred_set_op != NULL) {
	  reg_out_edge(pred_clear_op, DEST1, pred_set_op, DEST1);
	}
    }
}


int adjust_edge_latency_scalar (Edge* curr_edge) {

 int br_lat = MDES_branch_latency();

 Op* src_op  = curr_edge->src();
 Op* dest_op = curr_edge->dest();
 int lat = curr_edge->latency();


 // under interlocking model,
 // set latencies of edges going into and out of switches to 0.
 if (El_ssched_interlocking &&
	((is_switch (dest_op)) || is_switch(src_op)))
      return(0);

 // under MEM_PRIORITIZED (as opposed to ALL_PRIORITIZED) model,
 // ensure that all non-mem edge latencies are at least 1
 // (provided edge is not from merge/switch or to switch)
 if ((El_ssched_priority_model == MEM_PRIORITIZED) && 
     curr_edge->is_reg_dep() && !is_merge(src_op) &&
     !is_switch(src_op)   && !is_switch(dest_op) && (lat == 0))
        return (1);

 // under NONE_PRIORITIZED model,
 // ensure that all non-control edge latencies are at least 1
 if ((El_ssched_priority_model == NONE_PRIORITIZED) && 
     !curr_edge->is_control() && !is_merge(src_op) &&
     !is_switch(src_op)   && !is_switch(dest_op) && (lat == 0))
        return (1);

 // under MEM_NOT_PRIORITIZED model,
 // ensure that all mem edge latencies are at least 1
 if ((El_ssched_priority_model == MEM_NOT_PRIORITIZED) && 
     curr_edge->is_mem() && !is_merge(src_op) &&
     !is_switch(src_op)   && !is_switch(dest_op) && (lat == 0))
        return (1);

 // In current edge model, no outgoing edges from switch op
 // according to mike, this is no longer true. -ntclark 3/4/03
 // if (is_switch(src_op)) assert (0);

 // Adjust edge latencies to account for branch/dummy branch latencies
 // If dest_op is a switch,
 // set edge latency to current_latency - branch_latency
 if (is_switch (dest_op)) {
   Op* branch_op = real_op (dest_op);
   // Switch should have a branch.
   assert (is_control_switch (branch_op));

   if (is_dummy_branch(branch_op))
     // Dummy branch latency is always 1
     lat = ELCOR_MAX (0, lat - 1);
   else if (is_branch(branch_op)) {
     // first component in max is issue constraint,
     // second is dependence constraint
     // no separate edge for enforcing issue constraint
     lat = ELCOR_MAX (1 - br_lat, lat - br_lat);
   } else {
     assert (is_call(branch_op));
     // Since we don't know whether jsr operand is consumed directly by jsr
     // or at its call site,
     // no latency deduction is made for jsr (everything is treated as
     // consumed directly by jsr)
   }
 }

 if (curr_edge->is_control()) {
   // if control_1 edge from non-br/non-jsr to jsr/br,
   // set edge latency to 1 - branch_latency to
   // allow ops to drop into delay slots
   if (!is_control_merge(src_op) && !is_control_switch(src_op)) {
     assert (is_control_switch(dest_op));
     if (!is_dummy_branch(dest_op) && !is_rts(dest_op)) {
       return (1 - br_lat);
     }
   } else {
     if (is_call(src_op) && is_control_switch(dest_op)) return (br_lat);
     // Ensure that jsrs are placed after non-jsr's in an instruction
     // by upgrading zero-cycle latencies of edges from jsrs to 1
     // (so that ops dependent on jsr are not placed in same instruction)
     // and (later during scheduling) moving jsrs to the end of the instruction
     if (is_call(src_op) && lat == 0) return (1);
   }
 } else {
   if ( (is_call (src_op) && lat <= 0) ) return (1);
 }

 // Ensure that all outgoing edges from branches/jsrs/dbrs are at least 1,
 // so that these ops can be moved to end of instruction while still
 // preserving left-right semantics within instruction
 if (is_control_switch (src_op)) {
   if (dbg (ss, 7) && (lat < 1)) {
     cerr << "Src_op: " << (*src_op) << endl;
     cerr << "Edge:   " << (*curr_edge) << endl;
     cerr << "Dest_op:" << (*dest_op) << endl;
   }
   lat = ELCOR_MAX (1, lat);
 }

 return (lat);
}

// SAM 2-28-01: Moved from the modulo scheduler
// Adjust all edge latencies according to the priority model.
// This takes the additional edges into account as well.
// somewhat similar to Santosh's code in Sched/sched_functions.cpp
// -- Shail Aditya 11/07/97
int adjust_edge_latency_modulo (Edge* curr_edge)
{
   Op* dest_op = curr_edge->dest();
   Op* src_op = curr_edge->src();
   int lat = curr_edge->latency();

   // ignore edges from pseudo ops
   if (is_pseudo(src_op) || is_pseudo(dest_op))
       return (lat);

   // ignore edges to switch or already non-zero latency edges
   if (is_switch(dest_op) || lat != 0)
	return (lat);

   // under MEM_PRIORITIZED (as opposed to ALL_PRIORITIZED) model,
   // ensure that all non-mem edge latencies are at least 1
   if ((El_lsched_priority_model == LSCHED_MEM_PRIORITIZED) && curr_edge->is_reg_dep())
        return (1);

   // under NONE_PRIORITIZED model,
   // ensure that all non-control edge latencies are at least 1
   if ((El_lsched_priority_model == LSCHED_NONE_PRIORITIZED) && !curr_edge->is_control())
        return (1);

 // under MEM_NOT_PRIORITIZED model,
 // ensure that all mem edge latencies are at least 1
 if ((El_lsched_priority_model == MEM_NOT_PRIORITIZED) && 
     curr_edge->is_mem() && !is_merge(src_op) &&
     !is_switch(src_op)   && !is_switch(dest_op) && (lat == 0))
        return (1);

   return (lat);
}

// SAM 2-2001.  Operation chaining means that multiple "simple" ops are done
void update_edges_for_op_chaining(Compound_region *r)
{
    Op *op, *first_op, *prv_op, *cur_op;
    Op_chaining_attr *attr;

    // First build up a quick-access data structure
    Hash_map<Op*, Hash_set<Op*> > chained_ops(hash_op_ptr, 4096);
    Hash_set<Op*> processed(hash_op_ptr, 4096);
    Region_all_ops opi;

    for (opi(r); opi!=0; opi++) {
    op = *opi;
    attr = get_op_chaining_attr(op);
    if (attr == NULL) 
      continue;
  
  List<List<Op*> > &chains = attr->get_op_chain_list();
  for (List_iterator<List<Op*> > chaini(chains); chaini!=0; chaini++) {
      List<Op*> &chain = *chaini;
      first_op = chain.head();
      // If the head of the chain has been processed, no need to do again
      if (processed.is_member(first_op))
    continue;

      // Otherwise, process this chain
      prv_op = NULL;
	    cur_op = NULL;
	    for (List_iterator<Op*> opi(chain); opi!=0; opi++) {
		prv_op = cur_op;
		cur_op = *opi;
		if (prv_op == NULL)
		    continue;
		if (! chained_ops.is_bound(prv_op)) {
		    Hash_set<Op*> tmp(hash_op_ptr, 1);
		    chained_ops.bind(prv_op, tmp);
		}
		Hash_set<Op*>& to_set = chained_ops.value(prv_op);
		to_set += cur_op;
	    }
	}
	// Update processed set
	processed += op;
    }

    if (dbg(sed)) {
        for (Hash_map_iterator<Op*, Hash_set<Op*> > hmi(chained_ops);hmi!=0;hmi++) {
	    Op *from_op = (*hmi).first;
	    Hash_set<Op*> &to_set = (*hmi).second;
	    cdbg << "From Op: " << from_op->id() << endl;
	    cdbg << "\tTo Ops:";
	    for (Hash_set_iterator<Op*> hsi(to_set); hsi!=0; hsi++) {
	        cdbg << " " << (*hsi)->id();
	    }
	    cdbg << endl;
        }
    }

    // Now go through and reduce the latency on the flow dependence edges
    for (opi(r); opi!=0; opi++) {
	op = *opi;
	if (! chained_ops.is_bound(op))
	    continue;
	Hash_set<Op*> &to_set = chained_ops.value(op);
	for (Op_outedges edgei(op, FLOW_EDGES); edgei!=0; edgei++) {
	    Edge *edge = *edgei;
	    if (! to_set.is_member(edge->dest()))
		continue;
	    if (dbg(sed))
	        cdbg << "Reducing latency of edge for chaining\n\t" << *edge << endl;
	    int lat = edge->latency();
	    lat--;
	    if (lat < 0) {
		El_warn("update_edges_for_op_chaining: flow edge w/ negative lat!!");
		if (El_show_warnings) cdbg << "\tEdge: " << *edge << endl;
	    }
	    edge->set_latency(lat);
	}
    }
}

// Mukund, Jan. 2002: Update the edge latencies corrspd to op chains
// that were rejected by the scheduler. This is in order to nullify
// the effect of the procedure update_edges_for_op_chaining(). In
// PICO-NPA, this function should be called after modulo scheduling
// but before stage scheduling, so that the stage scheduler does not
// introduce additional op chains.
// BTW, note that a lot of the code here has been copied from the
// procedure update_edges_for_op_chaining().
void incr_edge_latencies_along_rejected_opchains(Compound_region *r)
{
    Op *op, *first_op, *prv_op, *cur_op;
    Op_chaining_attr *attr;

    // First build up a quick-access data structure
    Hash_map<Op*, Hash_set<Op*> > chained_ops(hash_op_ptr, 4096);
    Hash_set<Op*> processed(hash_op_ptr, 4096);
    Region_all_ops opi;

    for (opi(r); opi!=0; opi++) {
	op = *opi;
	attr = get_op_chaining_attr(op);
	if (attr == NULL)
	    continue;
	
	List<List<Op*> > &chains = attr->get_op_chain_list();
	for (List_iterator<List<Op*> > chaini(chains); chaini!=0; chaini++) {
	    List<Op*> &chain = *chaini;
	    first_op = chain.head();
	    // If the head of the chain has been processed, no need to do again
	    if (processed.is_member(first_op))
		continue;

	    // Otherwise, process this chain
	    prv_op = NULL;
	    cur_op = NULL;
	    for (List_iterator<Op*> opi(chain); opi!=0; opi++) {
		prv_op = cur_op;
		cur_op = *opi;
		if (prv_op == NULL)
		    continue;
		if (! chained_ops.is_bound(prv_op)) {
		    Hash_set<Op*> tmp(hash_op_ptr, 1);
		    chained_ops.bind(prv_op, tmp);
		}
		Hash_set<Op*>& to_set = chained_ops.value(prv_op);
		to_set += cur_op;
	    }
	}
	// Update processed set
	processed += op;
    }

    // Now go through and increase the latency on the flow dependence
    // edges corrspd to those chains that were rejected by the modulo
    // scheduler.
    for (opi(r); opi!=0; opi++) {
	op = *opi;
	if (! chained_ops.is_bound(op))
	    continue;
	Hash_set<Op*> &to_set = chained_ops.value(op);
	for (Op_outedges edgei(op, FLOW_EDGES); edgei!=0; edgei++) {
	    Edge *edge = *edgei;
	    // if no op chain attr along this edge, then do nothing.
	    if (! to_set.is_member(edge->dest()))
		continue;
	    // if ops chained along this edge, then do nothing.
	    if ((edge->dest()->sched_time() - op->sched_time()) == edge->latency())
		continue;
	    // otherwise, increment edge latency.
	    if (dbg(ms, 2))
	      cdbg << "Increasing edge latency for rejected chain\n\t" << *edge << endl;
	    int lat = edge->latency();
	    lat++;
	    edge->set_latency(lat);
	}
    }
}


// end of core_edge_drawing.cpp
