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
 *      File:           el_cmp_reduce.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        February 1998
 *      Description:    Compare reduce
 *
 *===========================================================================*/

#include <iostream>
#include "defs.h"
#include "dbg.h"
#include "tuples.h"
#include "dlist.h"
#include "operand.h"
#include "op.h"
#include "region.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "opcode_cmpp.h"
#include "attributes.h"
#include "el_control.h"
#include "cont_cpr_match.h"

using namespace std;

/* This is pretty much a replica of El_normalize_cmpp_chain() in
 * el_bypass_branch.cpp
 */
void El_normalize_cprblock(Dlist< Pair<Op*, Op*> > &cpr_block)
{
    Pair<Op*, Op*> cur_tuple;
    Op *branch_op, *cmpp_op;
    bool swap_dests = false, is_UN_UC, is_UC_UN, is_UN_UN;
    Opcode opp_opc;
    Operand pred_src, tmp_operand;

    for (Dlist_iterator< Pair<Op*, Op*> > g_iter(cpr_block); g_iter!=0; g_iter++) {
	cur_tuple = *g_iter;
        cmpp_op = cur_tuple.second;
        branch_op = cur_tuple.first;

	// if the branch is unconditional, the cmpp_op is empty
        if (cmpp_op == NULL)
            continue;

        if (is_brct(branch_op) || is_brcf(branch_op))
            pred_src = branch_op->src(SRC2);
        else if (is_bru(branch_op))
            pred_src = branch_op->src(PRED1);
        else
            El_punt("El_normalize_cmpp_group: unknown branch %d",
                        branch_op->id());

	/* First make dest1 of the cmpp be the branch condition */
        if (pred_src == cmpp_op->dest(DEST1)) {
            swap_dests = false;
        }
        else if (pred_src == cmpp_op->dest(DEST2)) {
            swap_dests = true;
        }
        else
            El_punt("El_normalize_cmpp_group: cmpp %d doesn't write branch reg",
                        cmpp_op->id());
        if (swap_dests == true) {
            opp_opc = exchange_cmpp_dest_modifiers(cmpp_op->opcode());
            cmpp_op->set_opcode(opp_opc);
            tmp_operand = cmpp_op->dest(DEST1);
            cmpp_op->dest(DEST1) = cmpp_op->dest(DEST2);
            cmpp_op->dest(DEST2) = tmp_operand;
	    // SAM 4-2000 Need to swap the Reference_dtype_attr width annots too!!!
	    Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(cmpp_op);
	    if (rdt_attr != NULL) {
		rdt_attr->swap_dests(DEST1, DEST2);
	    }
        }

        /* Now make sure DEST1 is UN and dest2 is UC */
        is_UN_UN = (is_UN_pred(cmpp_op, DEST1) && is_UN_pred(cmpp_op, DEST2));
        is_UN_UC = (is_UN_pred(cmpp_op, DEST1) && is_UC_pred(cmpp_op, DEST2));
        is_UC_UN = (is_UC_pred(cmpp_op, DEST1) && is_UN_pred(cmpp_op, DEST2));
        if (is_UC_UN) {
            opp_opc = get_opposite_cond(cmpp_op->opcode());
            opp_opc = (Opcode) make_UN_pred(opp_opc, DEST1);
            opp_opc = (Opcode) make_UC_pred(opp_opc, DEST2);
            cmpp_op->set_opcode(opp_opc);
        }
        else if (is_UN_UN) {
            if (! ((cmpp_op->dest(DEST2)).is_undefined()))
                El_punt("El_normalize_cmpp_group: cmpp op %d is improper UN/UN",
                        cmpp_op->id());
        }
        else if (!is_UN_UC) {
            El_punt("El_normalize_cmpp_group: cmpp op %d is not UC/UN or UN/UC",
                        cmpp_op->id());
        }
    }
}

void El_create_collapsed_predicates(Operand &br_pred, Operand &ft_pred)
{
    Reg *new_reg;

    new_reg = new Reg(EL_STANDARD_PREDICATE);
    Operand new_operand(new_reg);
    br_pred = new_operand;

    new_reg = new Reg(EL_STANDARD_PREDICATE);
    Operand new_operand2(new_reg);
    ft_pred = new_operand2;
}


void El_insert_initializer_for_collapsed_predicate(Hyperblock *hb, Operand &br_pred,
			Operand &ft_pred, Operand &incoming_pred, Op *first_cmpp)
{
    Op *new_op;
    Pred_lit *new_pred_lit;

    new_pred_lit = new Pred_lit(true);
    Operand pred_true(new_pred_lit);

    if (incoming_pred.is_predicate_true()) {
	new_op = new Op(PRED_SET);
	new_op->set_dest(DEST1, br_pred);
	new_op->set_src(PRED1, pred_true);
	El_insert_op_after_merge(hb, new_op);

	new_op = new Op(PRED_SET);
        new_op->set_dest(DEST1, ft_pred);
        new_op->set_src(PRED1, pred_true);
        El_insert_op_after_merge(hb, new_op);
    }

    else {
        // Temporarily we will insert both a PRED_SET, and the CMPP, the PRED_SET
        // is because the initialization does not fit impact stylization, argh!!
        new_op = new Op(PRED_SET);
        new_op->set_dest(DEST1, br_pred);
        new_op->set_src(PRED1, pred_true);
        El_insert_op_after_merge(hb, new_op);

	new_op = new Op(PRED_SET);
	new_op->set_dest(DEST1, ft_pred);
	new_op->set_src(PRED1, pred_true);
	El_insert_op_after_merge(hb, new_op);


        new_op = new Op((Opcode)CMPP_W_EQ_UN_UN);
        new_op->set_dest(DEST1, br_pred);
        new_op->set_dest(DEST2, ft_pred);
        new_op->set_src(PRED1, incoming_pred);
        Int_lit* new_int_lit = new Int_lit(0);
        Operand new_src(new_int_lit);
        new_op->set_src(SRC1, new_src);
        new_op->set_src(SRC2, new_src);
        El_insert_op_before(new_op, first_cmpp);
    }
}


void El_insert_collapsed_cmpp_op(Hyperblock *hb, Op *cmpp_op, Operand &br_pred,
			Operand &ft_pred, Operand &incoming_pred, bool last_br)
{
    Opcode opc;
    Op *new_op;

    if (cmpp_op == NULL)
	return;

    /* Create AC opcode with same root opc as cmpp_op */
    opc = cmpp_op->opcode();
    if (last_br)
        opc = make_AN_pred(opc, DEST1);
    else
        opc = make_AC_pred(opc, DEST1);
    opc = make_AC_pred(opc, DEST2);

    new_op = new Op(opc);
    new_op->set_dest(DEST1, br_pred);
    new_op->set_dest(DEST2, ft_pred);
    new_op->set_src(SRC1, cmpp_op->src(SRC1));
    new_op->set_src(SRC2, cmpp_op->src(SRC2));
    new_op->set_src(PRED1, incoming_pred);

    El_insert_op_after(new_op, cmpp_op);
}

void El_reassociate_branch_pred(Op *branch, Operand &br_pred)
{
    if (is_brct(branch) || is_brcf(branch)) {
	/* Set the guard to true, set the cond to br_pred */
        Pred_lit *new_pred_lit = new Pred_lit(true);
        Operand pred_true(new_pred_lit);
	branch->set_src(PRED1, pred_true);
	branch->set_src(SRC2, br_pred);
    }
    else if (is_bru(branch)) {
	/* Set the guard to br_pred */
	branch->set_src(PRED1, br_pred);
    }
    else {
	El_punt("El_reassociate_branch_pred: unknown branch op: %d", branch->id());
    }
}

void El_reassociate_input_operand(Hyperblock *hb, Operand &orig, Operand &replace)
{
    Op *op;
    Operand operand;
    Port_num port;

    if (orig.is_undefined())
	return;

    for (Region_ops_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	for (Op_explicit_inputs operand_i(op); operand_i!=0; operand_i++) {
	    operand = *operand_i;
	    if (operand != orig)
		continue;
	    port = operand_i.get_port_num();
	    op->set_src(port, replace);
	}
    }
}

Dlist<Operand> hb_br_preds;
Dlist<Operand> hb_ft_preds;

/*
 *	Note, to_branch is included in the collapse!!
 */
void El_collapse_compare_chain(Hyperblock *hb, Dlist< Pair<Op*, Op*> > &cpr_block,
				Op *target_branch, Op *to_branch)
{
    Pair<Op*,Op*> cur_pair;
    Op *target_cmpp, *to_cmpp, *cur_branch, *cur_cmpp;
    Operand incoming_pred, br_pred, ft_pred;
    bool flag, last_br;

    target_cmpp = to_cmpp = NULL;
    for (Dlist_iterator<Pair<Op*, Op*> > dl_i(cpr_block); dl_i!=0; dl_i++) {
	cur_pair = *dl_i;
	cur_branch = cur_pair.first;
	cur_cmpp = cur_pair.second;
	if (cur_branch == target_branch)
	    target_cmpp = cur_cmpp;
	if (cur_branch == to_branch)
	    to_cmpp = cur_cmpp;
    }
    
    /* Figure out the value of incoming predicate */
    if (to_cmpp != NULL)
	incoming_pred = to_cmpp->src(PRED1);
    else {
	Pred_lit* new_pred_lit = new Pred_lit(true);
        Operand pred_true(new_pred_lit);
	incoming_pred = pred_true;
    }

    /* Create the collapsed pred and initialize it */
    El_create_collapsed_predicates(br_pred, ft_pred);
    El_insert_initializer_for_collapsed_predicate(hb, br_pred, ft_pred,
							incoming_pred, to_cmpp);

    /* Walk the chain in reverse order, insert collapsed cmpp ops */
    flag = false;
    for (Dlist_iterator<Pair<Op*, Op*> > dl_i2(cpr_block, true); dl_i2!=0; dl_i2--) {
	cur_pair = *dl_i2;
	cur_branch = cur_pair.first;
        cur_cmpp = cur_pair.second;
	last_br = false;
	if (cur_branch == target_branch) {
	    flag = true;
	    last_br = true;
	}
	if (flag == false)
	    continue;
	El_insert_collapsed_cmpp_op(hb, cur_cmpp, br_pred, ft_pred, incoming_pred,
					last_br);
	if (cur_branch == to_branch)
	    break;
    }

    El_reassociate_branch_pred(target_branch, br_pred);
    if (target_cmpp != NULL) {
        El_reassociate_input_operand(hb, target_cmpp->dest(DEST1), br_pred);
        El_reassociate_input_operand(hb, target_cmpp->dest(DEST2), ft_pred);
    }
    hb_br_preds.push_tail(br_pred);
    hb_ft_preds.push_tail(ft_pred);
}

void El_compute_maxreduce_derived_preds(Hyperblock *hb, Operand &on_pred, Operand &off_pred,
		Hash_set<Operand> &cpr_preds,
		Hash_set<Operand> &derived_on_preds,
		Hash_set<Operand> &derived_off_preds)
{
    Op *op;
    Operand pred, dst_pred;

    derived_on_preds.clear();
    derived_off_preds.clear();

    derived_on_preds += on_pred;
    derived_off_preds += off_pred;

    for (Region_ops_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	if (!is_cmpp(op))
	    continue;
	pred = op->src(PRED1);
	if (derived_on_preds.is_member(pred)) {
	    for (Op_explicit_dests dst_i(op); dst_i!=0; dst_i++) {
		dst_pred = *dst_i;
		if (! dst_pred.is_reg())
		    continue;
		if (cpr_preds.is_member(dst_pred))
		    continue;
		derived_on_preds += dst_pred;
		if (dbg(cpr,3))
		    cdbg << "Adding " << dst_pred << " to derived ON preds" << endl;
	    }
	}
	else if (derived_off_preds.is_member(pred)) {
	    for (Op_explicit_dests dst_i(op); dst_i!=0; dst_i++) {
		dst_pred = *dst_i;
		if (! dst_pred.is_reg())
		    continue;
		if (cpr_preds.is_member(dst_pred))
		    continue;
		derived_off_preds += dst_pred;
		if (dbg(cpr,3))
		    cdbg << "Adding " << dst_pred << " to derived OFF preds" << endl;
	    }
	}
    }

    derived_on_preds -= on_pred;
    derived_off_preds -= off_pred;
}

void El_record_maxreduce_info_in_attr(Hyperblock *hb)
{
    Control_cpr_info *attr;
    int size, i;
    Operand br_pred, ft_pred;
    Hash_set<Operand> cpr_preds(hash_operand), derived_on_preds(hash_operand), derived_off_preds(hash_operand);

    if (dbg(cpr, 3))
	cdbg << "Enter El_record_maxreduce_info_in_attr: " << hb->id() << endl;
    attr = get_control_cpr_info(hb);
    if (attr != NULL)
        El_punt("El_record_maxreduce_info_in_attr: HB %d already has an attr",
			hb->id());

    size = hb_br_preds.size();
    attr = new Control_cpr_info(size);

    // Compute set of all preds used for cpr, cpr_preds
    for (Dlist_iterator<Operand> dl_i(hb_br_preds); dl_i!=0; dl_i++) {
	cpr_preds += (*dl_i);
    }
    for (Dlist_iterator<Operand> dl_i2(hb_ft_preds); dl_i2!=0; dl_i2++) {
	cpr_preds += (*dl_i2);
    }

    for (i=0; i<size; i++) {
	br_pred = hb_br_preds.pop();
	ft_pred = hb_ft_preds.pop();
	if (dbg(cpr, 3))
	    cdbg << "i " << i << " on_trace_pred " << ft_pred
                 << " off_trace_pred " << br_pred << endl;
	attr->set_on_trace_pred(i, ft_pred);
	attr->set_off_trace_pred(i, br_pred);
	El_compute_maxreduce_derived_preds(hb, ft_pred, br_pred, cpr_preds,
						derived_on_preds, derived_off_preds);
	if (derived_on_preds.size() > 0)
	    attr->set_derived_on_trace_pred(i, derived_on_preds);
	if (derived_off_preds.size() > 0)
	    attr->set_derived_off_trace_pred(i, derived_off_preds);
    }

    set_control_cpr_info(hb, attr);
}

/*==========================================================================*/
/*
 *	Interface routines
 */
/*==========================================================================*/

void El_max_reduce_cpr_blocks(Hyperblock *hb, Cont_cpr_match_result &match_result)
{
    Dlist<Dlist< Pair<Op*, Op*> > > hb_cprblocks;
    Dlist< Pair<Op*, Op*> > cur_cprblock;
    Pair<Op*, Op*> cur_tuple;
    Op *first_branch, *cur_branch, *cur_cmpp;

    hb_cprblocks = match_result.cpr_blk_list;

    if (hb_cprblocks.size() == 0) {
        if (dbg(cpr, 2))
            cdbg << "HB " << hb->id() << " has no cpr blocks" << endl;
        return;
    }

    hb_br_preds.clear();
    hb_ft_preds.clear();

    for (Dlist_iterator<Dlist< Pair<Op*, Op*> > > d_iter(hb_cprblocks);
			d_iter!=0; d_iter++) {
	cur_cprblock = *d_iter;
	El_normalize_cprblock(cur_cprblock);

	first_branch = NULL;
	for (Dlist_iterator<Pair<Op*, Op*> > d2_iter(cur_cprblock);
			d2_iter!=0; d2_iter++) {
	    cur_tuple = *d2_iter;
	    cur_branch = cur_tuple.first;
	    cur_cmpp = cur_tuple.second;
	    if (first_branch == NULL) {
		first_branch = cur_branch;
		hb_br_preds.push_tail(cur_cmpp->dest(DEST1));
		hb_ft_preds.push_tail(cur_cmpp->dest(DEST2));
		continue;
	    }
	    El_collapse_compare_chain(hb, cur_cprblock, cur_branch, first_branch);
	}
    }

    El_record_maxreduce_info_in_attr(hb);
}

