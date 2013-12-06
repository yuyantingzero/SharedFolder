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



/*===========================================================================
 *
 *      File:           el_promote_preds.cpp
 *      Author:         Richard Johnson, Scott Mahlke
 *      Created:        Sept 1996, March 1997
 *      Description:    A simple liveness-based predicate promoter.
 *
 *===========================================================================*/

#include <iostream>
#include "dbg.h"
#include "dlist.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "hash_set.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "edge_utilities.h"
#include "pqs.h"
#include "dfa.h"
#include "flow_analysis_solver.h"
#include "op.h"
#include "attributes.h"
#include "edge_attributes.h"
#include "pred_analysis.h"
#include "edge_utilities.h"
#include "el_cpr_tools.h"
#include "cont_cpr_match.h"
#include "el_promote_preds.h"

using namespace std;

#undef DEBUG
#undef DEBUG2

void El_check_promotion_for_frpized_sb(Hyperblock* hb)
{
    Op *op;
    Operand pred;

    for (Region_ops_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	if (is_pseudo(op))
	    continue;
	if (! op->predicated())
	    continue;
	if (is_store(op) || is_control_switch(op) || is_cmpp(op))
	    continue;
	pred = op->src(PRED1);
	if (pred.is_predicate_true())
	    continue;

	/* Now we have something that could be a problem */
	cdbg << "POTENTIAL PRED SPEC problem for op " << op->id() << endl;
	cdbg << *op;
	cdbg << endl;
    }
}

void El_record_original_pred(Hyperblock* hb, Hash_map<Op*, Operand> &orig_pmap)
{
    Op *op;

    for (Region_ops_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	if (is_pseudo(op))
	    continue;
	if (! op->predicated())
	    continue;
	orig_pmap.bind(op, op->src(PRED1));
    }
}

/*===========================================================================*/


void El_promote_preds_pass1 (Hyperblock* hb)
{
  Operand new_pred = new Pred_lit(true);
  Operand old_pred;
  Pred cond, ft_pred;
  Pred_expr ft_pred_expr;
  Op *op;

  if (dbg(cpr, 2))
    cdbg << "promote_preds, block " << hb->id() << endl;

  PQS* pqs = get_pqs(hb);
  VR_map* vr_map = get_local_vr_map (hb);
  assert(pqs != NULL && vr_map != NULL);

  int& vr_count = vr_map->vr_count;
  Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;

  // create vectors of pred expressions for live vars
  Vector<Pred_expr> liveness(vr_count);
  for (int i=0; i<vr_count; i++) {
    liveness[i] = pqs->false_expr();
  }

  // Create the ft predicate of the block
  ft_pred_expr = pqs->true_expr();
  for (Region_exit_ops eop_i(hb); eop_i!=0; eop_i++) {
    op = *eop_i;
    if (is_dummy_branch(op))
      continue;
    cond = pqs->glb_branch_pred(op);
    pqs->lub_diff(ft_pred_expr, cond);
  }
  ft_pred = pqs->lub(ft_pred_expr);


  // traverse ops in hyperblock in reverse linear order from exit to entry
  for (Region_ops_reverse_C0_order opi(hb); opi != 0; opi++) {
    assert ( (*opi)->is_op() );
    op = (Op*) (*opi);
    
    if (is_remap(op)) {
      El_punt("predicate speculation can't handle remap.\n");
    }
    else if (is_swp_branch(op)) {
      El_punt("predicate speculation can't handle swp branch.\n");
    }

    if (is_branch(op)) {

      // Subtract off the branch cond from the current liveness set
      for (int i=0; i<vr_count; i++) {
        cond = pqs->glb_branch_pred(op);
        pqs->lub_diff (liveness[i], cond);
      }

      // Sum in live-out of each edge of the branch
      for (Op_outedges oe(op, CONTROL0_OUTEDGES); oe != 0; oe++) {
	// get info for each edge (only exit edges have info)
	Edge* ed = (Edge*) (*oe);
	Liveness_info_iterator li ;
	Liveness_info* uu = get_liveness_info(ed);
	if (uu) {
          // Note, only ft edge of last br should have liveness attached to it
          if (is_fall_through(ed)) {
            cond = ft_pred; /* was pqs->true_pred(); */
          }
          else {
            cond = pqs->glb_branch_pred(op);
          }
	  for (li(*uu) ; li != 0 ; li++) {
	    Operand oper = *li;
	    if (vr_to_index.is_bound(oper)) {
#ifdef DEBUG
	      cdbg << "oper: " << oper << endl;
#endif
	      int ii = vr_to_index.value(oper);
	      pqs->lub_sum (liveness[ii], cond);
	    }
	  }
	}
      }
    }

    /* Consider an op guarded under predicate p.  To speculate to
     * predicate q s.t. p subset q, the op's defs must not be live
     * under q-p at this point.
     */
	
    // process defs
    for (Op_complete_dests dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
      Operand oper = (*dest_oper) ;
      Port_num pn = dest_oper.get_port_num();
	  
      if (oper.is_reg() || oper.is_macro_reg() || oper.is_mem_vr()) {
        int ii = vr_to_index.value(oper);
#ifdef DEBUG
        cdbg << "before def: " << oper << " live: " << liveness[ii] << endl;
#endif
        pqs->lub_diff (liveness[ii], pqs->glb_dest_guard(op, pn));
#ifdef DEBUG
        cdbg << "after def: " << oper << " live: " << liveness[ii] << endl;
#endif
      }
    }
	
    // test promotion of ops
    bool can_promote = false;
    if (is_pbr(op) || is_ialu(op) || is_falu(op) || is_load(op)) {
#ifdef DEBUG
      cdbg << "Considering op " << op->id() << " ("
	//<< (char *) el_opcode_to_string_map.value(op->opcode()) << ")"
	//tangw (06-03-02)
	<< get_mdes_opcode_string(op) << ")"


	   << endl;
#endif
	  
      can_promote = true;
      Pred target_pred = pqs->true_pred();
      for (Op_complete_dests d_oper(op) ; d_oper != 0 ; d_oper++) {
        Operand oper = (*d_oper) ;
	    
        if (oper.is_reg() || oper.is_macro_reg() /* || oper.is_mem_vr() */ ) {
	  int ii = vr_to_index.value(oper);
	  can_promote &= pqs->is_disjoint (target_pred, liveness[ii]);
	}
      }
    }

    // process uses (use original pred, even if op is speculated)
    for (Op_complete_inputs src_oper(op) ; src_oper != 0 ; src_oper++) {
      Operand& oper = (*src_oper) ;
      Port_num pn = src_oper.get_port_num();
	  
      if (oper.is_reg() || oper.is_macro_reg() || oper.is_mem_vr()) {
        int ii = vr_to_index.value(oper);
#ifdef DEBUG
        cdbg << "before use: " << oper << " live: " << liveness[ii] << endl;
#endif
        pqs->lub_sum (liveness[ii], pqs->lub_src_guard(op, pn));
#ifdef DEBUG
        cdbg << "after use: " << oper << " live: " << liveness[ii] << endl;
#endif
      }
    }

    old_pred = op->src(PRED1);
    if (can_promote && !old_pred.is_predicate_true()) {
      /* Promote the bloody thing */
#ifdef DEBUG
      cdbg << "> Promoting op " << op->id() << endl;
#endif
      El_promote_op(op, new_pred);
    }
  }

  // LIVENESS, PQS, etc have CHANGED!!!
}

void El_promote_preds_pass2 (Hyperblock* hb, Hash_map<Op*, Operand> &orig_pmap)
{
    Cont_cpr_match_result match_result;
    Dlist< Pair<Op*,Op*> > cpr_blk;
    Pair <Op*,Op*> cur_pair;
    Op *start_op, *branch, *cmpp, *op;
    Operand branch_pred, ft_pred, operand;
    Edge *edge;
    Liveness_info *uu;
    Hash_set<Op*> demoted_ops(hash_op_ptr, 211);
    int ii;
    Port_num pn;
    Pred branch_cond, operand_cond;
    bool should_demote;

#ifdef DEBUG2
    cdbg << "Enter promote preds pass2, HB " << hb->id() << endl;
#endif
    PQS *pqs = get_pqs(hb);
    VR_map* vr_map = get_local_vr_map (hb);
    assert(pqs != NULL && vr_map != NULL);

    int& vr_count = vr_map->vr_count;
    Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;
    Vector<Pred_expr> liveness(vr_count);

    match_result = cpr_match_hb_suitability(hb);

    for (Dlist_iterator<Dlist< Pair<Op*,Op*> > > dl_i(match_result.cpr_blk_list, true);
						dl_i!=0; dl_i--) {
	cpr_blk = *dl_i;

	start_op = NULL;
	for (Dlist_iterator<Pair<Op*,Op*> > dl2_i(cpr_blk, true);
						dl2_i!=0; dl2_i--) {
	    cur_pair = *dl2_i;
	    branch = cur_pair.first;
	    cmpp = cur_pair.second;
	    if (start_op == NULL)
		start_op = branch;
	    if (cmpp == NULL)
		continue;
	    branch_pred = El_get_branch_condition(branch);
	    if (branch_pred == cmpp->dest(DEST1))
	        ft_pred = cmpp->dest(DEST2);
	    else if (branch_pred == cmpp->dest(DEST2))
	        ft_pred = cmpp->dest(DEST1);
	    else
		El_punt("El_promote_preds_pass2: branch_pred not defined by cmpp");
	    edge = get_outgoing_CONTROL0_non_fall_through_edge(branch);
	    uu = get_liveness_info(edge);
	    assert(uu != NULL);

	    branch_cond = pqs->glb_branch_pred(branch);
	    liveness.assign(liveness.size(), pqs->false_expr());
	    for (Liveness_info_iterator li(*uu); li!=0; li++) {
		operand = *li;
		if (vr_to_index.is_bound(operand)) {
		    ii = vr_to_index.value(operand);
		    pqs->lub_sum(liveness[ii], branch_cond);
		}
	    }

	    for (Region_ops_reverse_C0_order op_i(hb, start_op); op_i!=0; op_i++) {
		op = *op_i;
		if (op == branch)
		    break;
		if (! op->flag(EL_OPER_PROMOTED))
		    continue;
		if (demoted_ops.is_member(op))
		    continue;
#ifdef DEBUG2
		cdbg << "Considering op for demotion: " << op->id() << endl;
#endif
		should_demote = false;
		for (Op_all_dests dest_oper(op); dest_oper!=0; dest_oper++) {
		    operand = *dest_oper;
		    pn = dest_oper.get_port_num();
		    if (! vr_to_index.is_bound(operand)) 
			continue;
		    ii = vr_to_index.value(operand);
		    operand_cond = pqs->glb_dest_guard(op, pn);
		    should_demote |= (! pqs->is_disjoint(operand_cond, liveness[ii]));
		}

		if (should_demote == true) {
		    /* Is it legal to demote this op to ft_pred?, for now assume
			yes, really need to test if ft_pred is a superset of the 
			ops original predicate, not sure how to do this yet!! */
#ifdef DEBUG2
		    cdbg << "Demoting op " << op->id()
			 << " orig pred: " << op->src(PRED1)
			 << " new pred: " << ft_pred << endl;
#endif
		    El_promote_op(op, ft_pred);
		    demoted_ops += op;
		}
	    }
	}
    }
}

/*===========================================================================*/
/*
 *	External routines
 */
/*===========================================================================*/

/*
 *	Steps:
 *	    Record original predicate of each op in orig_pmap
 *	    Pass1 raises the predicate of each op to highest legal value
 *	    Pass2 lowers the predicate
 *	    Sanity checking
 */
void El_promote_preds(Hyperblock* hb)
{
    Hash_map<Op*, Operand> orig_pmap(hash_op_ptr, 211);

    El_record_original_pred(hb, orig_pmap);
    El_promote_preds_pass1(hb);
    El_promote_preds_pass2(hb, orig_pmap);
#if 0
    El_check_promotion_for_frpized_sb(hb);
#endif
}


void El_promote_preds (Compound_region *reg)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rtmp;

    create_local_analysis_info_for_all_hbs_bbs(reg);
    el_flow_compute_liveness(reg, ANALYZE_ALLREG);

    rstack.push(reg);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
	    El_promote_preds((Hyperblock*)rtmp);
            push_flag = false;  // Only ops below, so don't waste time!
        }
        else if (rtmp->is_bb()) {
            push_flag = false;  // Only ops below, so don't waste time!
        }
        if (push_flag==true) {
            for (Region_subregions subri(rtmp); subri!=0; subri++) {
                if ((*subri)->is_compound())
                    rstack.push(*subri);
            }
        }
    }

    delete_local_analysis_info_for_all_hbs_bbs(reg);
}
