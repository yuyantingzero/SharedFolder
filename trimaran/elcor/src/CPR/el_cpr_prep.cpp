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
 *      File:           el_cpr_prep.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        October 1997
 *      Description:    Prep/Repair for control CPR
 *
 *===========================================================================*/
#include "defs.h"
#include "dbg.h"
#include "el_error.h"
#include "edge.h"
#include "connect.h"
#include "region.h"
#include "region_utilities.h"
#include "edge_utilities.h"
#include "opcode_properties.h"
#include "attributes.h"
#include "iterators.h"
#include "el_control.h"
#include "el_bb_tools.h"
#include "el_cpr_tools.h"
#include "intf.h"
#include "el_cpr_init.h"
#include "el_cpr_prep.h"

#undef DEBUG

/*==============================================================================*/
/*
 *	CPR Prep consists of:
 *		DUMMY_BR insertion at the end of a block for a block which ends
 *		with a brct/brcf/bru.
 */
/*==============================================================================*/

void El_insert_dummy_branch(Hyperblock* hb)
{
    Op *last_op, *cmerge, *dummy_br, *target_op;
    Edge *ft_edge1, *ft_edge2, *taken_edge1;
    Control_flow_freq *cfreq1, *cfreq2;
    Compound_region *next_reg;

    last_op = get_last_region_op_from_subregions(hb);

    if (is_dummy_branch(last_op))
	return;

    if (is_brct(last_op) || is_brcf(last_op)) {
        /* Remove ft edge from appropriate region exit/entry sets */
        ft_edge1 = get_outgoing_CONTROL0_fall_through_edge(last_op);
        if (ft_edge1 == NULL)
	    El_punt("El_insert_dummy_branch: brct/brcf %d has no ft edge",
			    last_op->id());
        hb->remove_exit(ft_edge1);
        next_reg = ft_edge1->dest()->parent();
        if (next_reg==NULL)
	    El_punt("El_insert_dummy_branch: no FT block subsequent to %d",
			    hb->id());
        next_reg->remove_entry(ft_edge1);
	cfreq1 = get_control_flow_freq(ft_edge1);
	if (cfreq1 == NULL)
	    El_punt("El_insert_dummy_branch: ft edge for brct/f %d no cfreq",
                            last_op->id());

        cmerge = new Op(C_MERGE);
        El_insert_op_after(cmerge, last_op);
        dummy_br = new Op(DUMMY_BR);
        El_insert_op_after(dummy_br, cmerge);

        /* Add cfreq attribute to edge coming out of the dummy br, copy weight from
	    ft edge of the branch */
        cfreq2 = new Control_flow_freq();
        cfreq2->freq = cfreq1->freq;
        cfreq2->cc = 0;
        ft_edge2 = get_outgoing_CONTROL0_fall_through_edge(dummy_br);
        set_control_flow_freq(ft_edge2, cfreq2);
    
        /* Add edge from dummy_br to next hb into region entry/exit sets */
        dummy_br->add_outop_recursively(ft_edge2);
        dummy_br->add_outedge_recursively(ft_edge2);
        next_reg->add_entry(ft_edge2);

	hb->set_flag(EL_REGION_HAS_EXPLICIT_FT);
#ifdef DEBUG
	cdbg << "Adding DUMMY br to HB with brct " << hb->id() << endl;
#endif
    }
    else {
	// This version of the DUMMY_BR insertion for brus adds the edge
	// from the dummy to target the same op as the bru
	taken_edge1 = get_outgoing_CONTROL0_non_fall_through_edge(last_op);
	if (taken_edge1 == NULL)
	    El_punt("El_insert_dummy_branch: no taken edge on %d", last_op->id());
	target_op = taken_edge1->dest();

	cmerge = new Op(C_MERGE);
        El_insert_op_after(cmerge, last_op);
        dummy_br = new Op(DUMMY_BR);
	dummy_br->set_flag(EL_OPER_NONSEQ_DUMMY_BR);
        El_insert_op_after(dummy_br, cmerge);

	// Connect dummy_br to the target op of the bru
	C0_connect_fallthrough(dummy_br, target_op);

        // Add a cfreq attr to the ft edge on last_op
        ft_edge1 = get_outgoing_CONTROL0_fall_through_edge(last_op);
        if (ft_edge1 == NULL)
            El_punt("El_insert_dummy_branch: modified bru %d has no ft edge",
                        last_op->id());
        cfreq1 = new Control_flow_freq();
        cfreq1->freq = 0.0;
        cfreq1->cc = 0;
	set_control_flow_freq(ft_edge1, cfreq1);

        /* Add cfreq attribute to edge coming out of the dummy br, copy weight from
	    ft edge of the branch */
        cfreq2 = new Control_flow_freq();
        cfreq2->freq = cfreq1->freq;
        cfreq2->cc = 0;
        ft_edge2 = get_outgoing_CONTROL0_fall_through_edge(dummy_br);
        set_control_flow_freq(ft_edge2, cfreq2);
    
        /* Add edge from dummy_br to next hb into region entry/exit sets */
        dummy_br->add_outop_recursively(ft_edge2);
        dummy_br->add_outedge_recursively(ft_edge2);
	target_op->add_inedge_recursively(ft_edge2);
#ifdef DEBUG
	cdbg << "Adding DUMMY br to HB with bru " << hb->id() << endl;
#endif
    }
}

void El_insert_dummy_branches(Procedure *proc)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rtmp;

    rstack.push(proc);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
	    El_insert_dummy_branch((Hyperblock* )rtmp);
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
}


/*==============================================================================*/
/*
 *	CPR Repair consists of:
 *		Removing useless dummy branches that are redundant
 *		Retargeting nonseq dummy branches if the corresponding original
 *			real branch got removed/eliminated
 */
/*==============================================================================*/

bool El_dummy_branch_is_required(Hyperblock* hb)
{
    Op *last_op, *last_op_prev, *last_op_prev2;

    last_op = get_last_region_op_from_subregions(hb);
    if (! is_dummy_branch(last_op))
	El_punt("El_dummy_branch_is_required: last_op of HB %d is not a dummy_br",
			hb->id());

    last_op_prev = last_op->prev();
    if (is_control_merge(last_op_prev)) {
	last_op_prev2 = last_op_prev->prev();
	if (! is_branch(last_op_prev2))
            return (true);
	// If the last real branch is not scheduled, then cm/dummy are unnecessary
	if (! last_op_prev2->flag(EL_REGION_SCHEDULED))
	    return (false);
	// If the op is scheduled, then dummy br always needed to mark the cycle at
	// which the block really ends for a non-interlocked machine with non-unit br
	// latency.  For unit br latency, the dummy br is not needed
	if (MDES_branch_latency() > 1)
	    return (true);
	else
	    return (false);
    }
    else {
	if (is_branch(last_op_prev))
	    El_punt("El_dummy_branch_is_required: HB %d is corrupt, missing CM",
			hb->id());
	return (true);
    }
}

void El_fixup_dummy_branch(Hyperblock* hb)
{
    Op *last_op, *op, *target_op, *cm_op, *new_last_op, *next_reg_first_op, *branch;
    Edge *edge, *matching_edge;
    Compound_region *parent, *next_reg;
    Control_flow_freq *cfreq, *new_cfreq;
    double freq;
    bool need_ft = true;

    last_op = get_last_region_op_from_subregions(hb);

    if (! is_dummy_branch(last_op))
        return;

#ifdef DEBUG
    cdbg << "El_fixup_dummy_branch: HB " << hb->id() << endl;
#endif


    if (El_dummy_branch_is_required(hb)) {
#ifdef DEBUG
	cdbg << "\tDummy br is required" << endl;
#endif
	if (! last_op->flag(EL_OPER_NONSEQ_DUMMY_BR)) {
	    ; // Nothing to be done, leave the dummy br alone
	}
	else {
	    /* Do some extra checking to make sure matching br is there,
		this matching branch is needed to xlate to Lcode */
	    edge = get_outgoing_CONTROL0_edge(last_op);
	    assert (edge != NULL);
	    matching_edge = get_exit_edge_to_same_target(hb, edge);
	    if (matching_edge == NULL) {
		/* the original matching br gone, attempt to retarget
		   the dummy to the target of the previous in the Hyperblock */
		branch = NULL;
		for (Region_ops_reverse_C0_order op_i(hb); op_i!=0; op_i++) {
		    op = *op_i;
		    if (is_proper_branch(op)) {
			branch = op;
			break;
		    }
		}
		if (branch == NULL)
		    El_punt("El_fixup_dummy_branch: HB has no real branches!");
		matching_edge = get_outgoing_CONTROL0_non_fall_through_edge(branch);
		assert(matching_edge != NULL);
		edge->set_dest(matching_edge->dest(), CONTROL0, 0);	
#ifdef DEBUG
	        cdbg << "\tRetargeting dummy br to " 
		     << matching_edge->dest()->id() << endl;
#endif
	    }
	}
    }
    else {
	// Nuke the dummy br and its corresonding c_merge
        edge = get_outgoing_CONTROL0_fall_through_edge(last_op);
        target_op = edge->dest();
	cfreq = get_control_flow_freq(edge);
	freq = cfreq->freq;
	cm_op = last_op->prev();
	new_last_op = cm_op->prev();
	if (last_op->flag(EL_OPER_NONSEQ_DUMMY_BR))
	    need_ft = false;

#ifdef DEBUG
	cdbg << "\tDummy br is NOT required" << endl;
	cdbg << "\tNuking ops " << last_op->id() << " and " << cm_op->id() << endl;
#endif

	El_remove_op(cm_op);
	El_remove_op(last_op);
	delete last_op;
	delete cm_op;

	edge = get_outgoing_CONTROL0_fall_through_edge(new_last_op);
	target_op = edge->dest();
	new_last_op->remove_outedge_recursively(edge);
	target_op->remove_inedge_recursively(edge);

	// Is there a ft path?, if so add a C0 edge
	if (need_ft) {
#ifdef DEBUG
	    cdbg << "\tBlock has ft path" << endl;
#endif
            parent = hb->parent();
            next_reg = (Compound_region *) parent->successor_in_subregion_list(hb);
            if (next_reg == NULL)
	        El_punt("El_fixup_dummy_branch: no FT block subsequent to %d",
				hb->id());
            next_reg_first_op = get_first_region_op_from_subregions(next_reg);
	    C0_connect_fallthrough(new_last_op, next_reg_first_op);
	    edge = get_outgoing_CONTROL0_fall_through_edge(new_last_op);
	    new_cfreq = new Control_flow_freq();
	    new_cfreq->freq = freq;
	    new_cfreq->cc = 0;
	    set_control_flow_freq(edge, new_cfreq);
	    new_last_op->add_outedge_recursively(edge);
	    next_reg_first_op->add_inedge_recursively(edge);
	}
	hb->reset_flag(EL_REGION_HAS_EXPLICIT_FT);
    }
}


void El_fixup_dummy_branches(Procedure *proc)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rtmp;

    rstack.push(proc);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
	    El_fixup_dummy_branch((Hyperblock* )rtmp);
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
}

/*==============================================================================*/
/*
 *	insert explicit ft branches, this done for max reduce CPR only!!
 */
/*==============================================================================*/

static Op *create_cmerge_op()
{
    Op *cmerge_op = new Op(C_MERGE);

    return (cmerge_op);
}

static Op *create_pbr_op(Op *target_op)
{
    Reg *btr_reg = new Reg(EL_STANDARD_BRANCH, BTR, virtual_register_number);
    Operand btr_operand(btr_reg);

    Cb_operand *target_cb = new Cb_operand(target_op->parent()->id());
    Operand target_operand(target_cb);

    Int_lit *int_one = new Int_lit(1);
    Operand int_operand(int_one);

    Pred_lit *pred_true = new Pred_lit(true);
    Operand pred_operand(pred_true);

    Op *pbr_op = new Op(PBR);
    pbr_op->set_dest(DEST1, btr_operand);
    pbr_op->set_src(SRC1, target_operand);
    pbr_op->set_src(SRC2, int_operand);
    pbr_op->set_src(PRED1, pred_operand);

    return (pbr_op);
}

static Op *create_bru_op(Operand &btr)
{
    Pred_lit *pred_true = new Pred_lit(true);
    Operand pred_operand(pred_true);

    Op *bru_op = new Op(BRU);
    bru_op->set_src(SRC1, btr);
    bru_op->set_src(PRED1, pred_operand);

    return (bru_op);
}

// This is conservative now as it assumes a very structured HB
static Operand create_pred_for_new_bru(Hyperblock* hb)
{
    Operand undefined_pred, branch_cond;
    Op *op, *branch_op, *cmpp_op;
    bool d1_matches, d2_matches, d1_free, d2_free;

    branch_op = NULL;
    cmpp_op = NULL;
    for (Region_ops_reverse_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	if (is_pseudo(op))
	    continue;
	if (is_branch(op)) {
	    branch_op = op;
	    continue;
	}
	if ((branch_op != NULL) && (is_cmpp(op))) {
	    cmpp_op = op;
	    break;
	}

	/* If find a predicated op, then frp is already created, do nothing */
	if ((branch_op == NULL) && (op->predicated()))
	    return (op->src(PRED1));
	
    }

    if ((branch_op == NULL) && (cmpp_op == NULL))
	return (undefined_pred);

    branch_cond = El_get_branch_condition(branch_op);
    d1_matches = (branch_cond == cmpp_op->dest(DEST1));
    d1_matches &= is_U_pred(cmpp_op, DEST1);
    d2_matches = (branch_cond == cmpp_op->dest(DEST2));
    d2_matches &= is_U_pred(cmpp_op, DEST2);
    d1_free = cmpp_op->dest(DEST1).is_undefined();
    d2_free = cmpp_op->dest(DEST2).is_undefined();
    if (! (d1_matches | d2_matches))
	return (undefined_pred);
    if (! (d1_free | d2_free))
	return (undefined_pred);

    /* Now we're in business, so create a new predicate in free dest of cmpp_op */
    Reg *new_pred_reg = new Reg(EL_STANDARD_PREDICATE);
    Operand new_pred(new_pred_reg);

    if (d1_free) {
	cmpp_op->set_dest(DEST1, new_pred);
	if (is_UN_pred(cmpp_op, DEST2))
	    cmpp_op->set_opcode(make_UC_pred(cmpp_op->opcode(), DEST1));
        else 
	    cmpp_op->set_opcode(make_UN_pred(cmpp_op->opcode(), DEST1));
    }
    else {
	cmpp_op->set_dest(DEST2, new_pred);
	if (is_UN_pred(cmpp_op, DEST1))
	    cmpp_op->set_opcode(make_UC_pred(cmpp_op->opcode(), DEST2));
        else 
	    cmpp_op->set_opcode(make_UN_pred(cmpp_op->opcode(), DEST2));
    }

    return (new_pred);
}

void El_insert_explicit_ft_branch(Hyperblock* hb)
{
    Op *last_op, *last_prev_op, *target_op, *cmerge_op, *pbr_op, *bru_op;
    Edge *edge;
    Control_flow_freq *cfreq;
    double weight;
    Operand bru_pred;

    if (hb->weight < El_min_cpr_weight)
	return;

    last_op = get_last_region_op_from_subregions(hb);
    edge = get_outgoing_CONTROL0_fall_through_edge(last_op);
    if (edge == NULL)
	return;
    cfreq = get_control_flow_freq(edge);
#if 1
    if ((cfreq->freq / hb->weight) < .025)
	return;
#endif

    /* This block has a fallthru path, make it explicit with a bru */
    if (is_bru(last_op))
	El_punt("El_insert_explicit_ft_branch: HB %d has ft path, ends with bru",
			hb->id());

    bru_pred = create_pred_for_new_bru(hb);
    if (bru_pred.is_undefined())
	return;

    target_op = edge->dest();
    weight = cfreq->freq;

    if (dbg(cpr,4))
	cdbg << "Inserting BRU into HB " << hb->id() << endl;

    if (is_dummy_branch(last_op)) {
	last_prev_op = last_op->prev();
	last_op->remove_outedge_recursively(edge);
	target_op->remove_inedge_recursively(edge);
	delete edge;

	edge = get_incoming_CONTROL0_edge(last_op);
	last_op->remove_inedge(edge, CONTROL, CONTROL0, 0);
	last_prev_op->remove_outedge(edge, CONTROL, CONTROL0, 0);
	delete edge;

	hb->remove_exit(last_op);
	hb->remove_region(last_op);
	delete last_op;
	last_op = last_prev_op;
    }
    else {
	last_op->remove_outedge_recursively(edge);
	target_op->remove_inedge_recursively(edge);
	delete edge;
        cmerge_op = create_cmerge_op();
	El_insert_op_after(cmerge_op, last_op);
	edge = get_incoming_CONTROL0_edge(cmerge_op);
	cfreq = new Control_flow_freq();
	cfreq->freq = weight;
	cfreq->cc = 0;
	set_control_flow_freq(edge, cfreq);
	last_op = cmerge_op;
    }

    pbr_op = create_pbr_op(target_op);
    bru_op = create_bru_op(pbr_op->dest(DEST1));
    El_insert_op_after(pbr_op, last_op);
    El_insert_op_after(bru_op, pbr_op);
    hb->add_exit(bru_op);
    edge = C0_connect(bru_op, target_op);
    cfreq = new Control_flow_freq();
    cfreq->freq = weight;
    cfreq->cc = 1;
    set_control_flow_freq(edge, cfreq);
    bru_op->add_outedge_recursively(edge);
    target_op->add_inedge_recursively(edge);

    // Associate proper frp with bru_op, pbr_op
    bru_op->set_src(PRED1, bru_pred);
    pbr_op->set_src(PRED1, bru_pred);
}

void El_insert_explicit_ft_branches(Procedure *proc)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rtmp;

    rstack.push(proc);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
	    El_insert_explicit_ft_branch((Hyperblock* )rtmp);
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
}
