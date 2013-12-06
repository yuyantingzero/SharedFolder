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
 *      File:           el_bypass_branch.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        April 1997
 *      Description:    Bypass branch insertion
 *
 *===========================================================================*/

/*
 *	NOTES: SAM 4-97
 *	1. Normalized form is defined if each cmpp looks like the following:
 *		p1(UN),p2(UC) = cmpp()
 *			p1 = branch cond
 *			p2 = ft frp
 *
 */


#include "defs.h"
#include "dbg.h"
#include "region.h"
#include "iterators.h"
#include "el_cpr_init.h"
#include "hash_set.h"
#include "attributes.h"
#include "region_attributes.h"
#include "dlist.h"
#include "tuples.h"
#include "opcode_properties.h"
#include "opcode_cmpp.h"
#include "el_control.h"
#include "connect.h"
#include "edge_utilities.h"
#include "region_utilities.h"
#include "cont_cpr_match.h"
#include "el_code_motion.h"
#include "el_bypass_branch.h"

/*==============================================================================*/
/*
 *	El_cpr_chain member functions
 */
/*==============================================================================*/

El_cpr_chain::El_cpr_chain(Dlist< Pair<Op*, Op*> > &group)
{
    flags = 0;
    cmpp_branch_ops = group;
    comp_block = NULL;
    cpr_branch_op = NULL;

    // Temporary hack!!!!
    Op *last_cmpp_op, *cmpp_op, *branch_op;
    last_cmpp_op = NULL;
    for (Dlist_iterator<Pair<Op*, Op*> > di(cmpp_branch_ops); di!=0; di++) {
	Pair<Op*,Op*> &cur_pair = *di;
	cmpp_op = cur_pair.second;
	branch_op = cur_pair.first;
	if (is_bru(branch_op) && (cmpp_op == last_cmpp_op)) {
	    cur_pair.second = NULL;
	}
	last_cmpp_op = cur_pair.second;
    }
}

El_cpr_chain::~El_cpr_chain()
{
}

void El_cpr_chain::set_flag(unsigned int f)
{
    flags |= f;
}

void El_cpr_chain::reset_flag(unsigned int f)
{
    flags &= (~f);
}

bool El_cpr_chain::is_flag(unsigned int f)
{
    if (flags & f)
	return (true);
    else
	return (false);
}

Op *El_cpr_chain::get_first_cmpp_op()
{
    if (cmpp_branch_ops.is_empty())
	El_punt("El_cpr_chain::get_first_cmpp_op: cmpp_branch_ops is empty");
    return (cmpp_branch_ops.head().second);
}

Op *El_cpr_chain::get_last_cmpp_op()
{
    if (cmpp_branch_ops.is_empty())
        El_punt("El_cpr_chain::get_last_cmpp_op: cmpp_branch_ops is empty");
    return (cmpp_branch_ops.tail().second);
}

Op *El_cpr_chain::get_first_branch_op()
{
    if (cmpp_branch_ops.is_empty())
        El_punt("El_cpr_chain::get_first_branch_op: cmpp_branch_ops is empty");
    return (cmpp_branch_ops.head().first);
}

Op *El_cpr_chain::get_last_branch_op()
{
    if (cmpp_branch_ops.is_empty())
        El_punt("El_cpr_chain::get_last_branch_op: cmpp_branch_ops is empty");
    return (cmpp_branch_ops.tail().first);
}

Op *El_cpr_chain::get_corres_branch_op(Op *cmpp_op)
{
    Pair<Op*, Op*> cur_pair;

    for (Dlist_iterator<Pair<Op*, Op*> > d_iter(cmpp_branch_ops);
			d_iter!=0; d_iter++) {
	cur_pair = *d_iter;
	if (cur_pair.second == cmpp_op)
	    return (cur_pair.first);
    }

    El_punt("El_cpr_chain::get_corres_branch_op: cmpp_op %d not found",
		cmpp_op->id());
    return (NULL);
}

Op *El_cpr_chain::get_corres_cmpp_op(Op *branch_op)
{
    Pair<Op*, Op*> cur_pair;

    for (Dlist_iterator<Pair<Op*, Op*> > d_iter(cmpp_branch_ops);
                        d_iter!=0; d_iter++) {
	cur_pair = *d_iter;
	if (cur_pair.first == branch_op)
	    return (cur_pair.second);
    }

    El_punt("El_cpr_chain::get_corres_branch_op: branch_op %d not found",
		branch_op->id());
    return (NULL);
}

Dlist<Pair<Op*, Op* > > &El_cpr_chain::get_cmpp_branch_op_list()
{
    return (cmpp_branch_ops);
}

bool El_cpr_chain::is_chain_branch_op(Op *x)
{
    Pair<Op*, Op* > cur_tuple;

    if (! is_branch(x))
	return (false);

    for (Dlist_iterator<Pair<Op*, Op* > > d_iter(cmpp_branch_ops);
			d_iter!=0; d_iter++) {
	cur_tuple = *d_iter;
	if (x == cur_tuple.first)
	    return (true);
    }

    return (false);
}

bool El_cpr_chain::is_chain_cmpp_op(Op *x)
{
    Pair<Op*, Op* > cur_tuple;

    if (! is_cmpp(x))
        return (false);

    for (Dlist_iterator<Pair<Op*, Op* > > d_iter(cmpp_branch_ops);
                        d_iter!=0; d_iter++) {
        cur_tuple = *d_iter;
        if (x == cur_tuple.second)
            return (true);
    }

    return (false);
}

void El_cpr_chain::set_pred_incoming(Operand &val)
{
    if (val.is_undefined())
        El_punt("El_cpr_chain::set_pred_on: pred incoming cannot set to undefined");
    pred_incoming = val;
}

void El_cpr_chain::set_pred_on(Operand &val)
{
    if (val.is_undefined())
	El_punt("El_cpr_chain::set_pred_on: pred on cannot set to undefined");
    pred_on = val;
}

void El_cpr_chain::set_pred_off(Operand &val)
{
    if (val.is_undefined())
        El_punt("El_cpr_chain::set_pred_off: pred off cannot set to undefined");
    pred_off = val;
}

Operand &El_cpr_chain::get_pred_incoming()
{
    return (pred_incoming);
}

Operand &El_cpr_chain::get_pred_on()
{
    return (pred_on);
}

Operand &El_cpr_chain::get_pred_off()
{
    return (pred_off);
}

bool El_cpr_chain::is_cpr_pred(Operand &val)
{
    if ((val == pred_on) || (val == pred_off))
	return (true);
    else
	return (false);
}

void El_cpr_chain::set_comp_block(Hyperblock *hb)
{
    if (hb == NULL)
	El_punt("El_cpr_chain::set_comp_block: hb is NULL");
    comp_block = hb;
}

Hyperblock *El_cpr_chain::get_comp_block()
{
    return (comp_block);
}

void El_cpr_chain::add_cpr_cmpp_op(Op *op)
{
    if (op == NULL)
	El_punt("El_cpr_chain::add_cpr_cmpp_op: op is NULL");
    cpr_cmpp_ops.push_tail(op);
}

void El_cpr_chain::set_cpr_branch_op(Op *op)
{
    if (op == NULL)
        El_punt("El_cpr_chain::set_cpr_branch_op: op is NULL");
    cpr_branch_op = op;
}

Op *El_cpr_chain::get_cpr_branch_op()
{
    return (cpr_branch_op);
}

void El_cpr_chain::print_chain()
{
    Pair<Op*, Op*> cur_pair;
    Op *cmpp_op, *branch_op;

    cdbg << "  Chain flags: ";
    if (flags != 0) {
	if (flags & EL_CPR_LAST_BR_TAKEN)
	    cdbg << "LAST_BR_TAKEN";
	if (flags & EL_CPR_HAS_COMP_BLOCK)
	    cdbg << "HAS_COMP_BLOCK";
	cdbg << endl;
    }
    else {
	cdbg << "NONE" << endl;
    }

    cdbg << "  Original cmpp/branch pairs:" << endl;
    for (Dlist_iterator<Pair<Op*, Op*> > d_iter(cmpp_branch_ops);
                        d_iter!=0; d_iter++) {
	cur_pair = *d_iter;
	cmpp_op = cur_pair.second;
	branch_op = cur_pair.first;
	if (cmpp_op != NULL)
            cdbg << "    CMPP: " << cmpp_op->id()
                 << " / BRANCH: " << branch_op->id() << endl;
	else
	    cdbg << "    CMPP: NONE"
		 << " / BRANCH: " << branch_op->id() << endl;
    }


    cdbg << "  INCOMING chain predicate" << endl;
    cdbg << "    " << pred_incoming << endl;

    cdbg << "  ON-OFF trace predicates" << endl;
    cdbg << "    " << pred_on << "  /  " << pred_off << endl; 

    cdbg << "  CPR cmpps" << endl;
    for (Dlist_iterator<Op*> c_iter(cpr_cmpp_ops); c_iter!=0; c_iter++) {
	cmpp_op = *c_iter;
	if (cmpp_op != NULL)
            cdbg << "    CPR CMPP: " << cmpp_op->id() << endl;
	else
	    cdbg << "    CPR CMPP: NONE" << endl;
    }

    cdbg << "  CPR branch op: ";
    if (cpr_branch_op)
	cdbg << cpr_branch_op->id() << endl;
    else
	cdbg << "none" << endl;

    cdbg << "  Compensation block: ";
    if (comp_block)
	cdbg << comp_block->id() << endl;
    else
	cdbg << "none" << endl;
}


/*==============================================================================*/
/*
 *	El_cpr_chain member functions
 */
/*==============================================================================*/

El_cpr_chain_list::El_cpr_chain_list(Hyperblock *hb)
{
    block = hb;
    num_chains = 0;
}

El_cpr_chain_list::~El_cpr_chain_list()
{
    El_cpr_chain *chain;

    for (Dlist_iterator<El_cpr_chain *> d_iter(chains); d_iter!=0; d_iter++) {
	chain = *d_iter;
	if (chain)
	    delete chain;
    }
}

void El_cpr_chain_list::add_chain(El_cpr_chain *chain)
{
    if (chain == NULL)
	El_punt("El_cpr_chain_list::add_chain: chain is NULL");
    chains.push_tail(chain);
    num_chains++;
}

Dlist<El_cpr_chain*> &El_cpr_chain_list::get_chain_list()
{
    return (chains);
}

int El_cpr_chain_list::get_num_chains()
{
    return (num_chains);
}

void El_cpr_chain_list::print_chain_list()
{
    int i;
    El_cpr_chain *c;

    i = 0;
    cdbg << "<<< Chains for HB " << block->id() << " >>>" << endl;
    for (Dlist_iterator<El_cpr_chain *> d_iter(chains); d_iter!=0; d_iter++) {
	c = *d_iter;
	cdbg << " ========= Chain " << i << " ========" << endl;
	c->print_chain();
	cdbg << " =========================" << endl;
	i++;
    }
}

/*==============================================================================*/
/*
 *	Simple utility routines
 */
/*==============================================================================*/


bool El_branch_is_likely_taken(Op *branch_op)
{
    double taken_w, fallthru_w;
    Edge *edge;
    Control_flow_freq *cfreq;

    taken_w = 0.0;
    fallthru_w = 0.0;
    for (Region_all_outedges e_iter(branch_op, CONTROL0_OUTEDGES);
		e_iter!=0; e_iter++) {
	edge = *e_iter;
	cfreq = get_control_flow_freq(edge);
	if (cfreq == NULL)
	    El_punt("El_branch_is_likely_taken: branch C0 edge has no cfreq attr");
	if (edge->src_alt() == 0)
	    fallthru_w = cfreq->freq;
	else
	    taken_w += cfreq->freq;
	
    }

    return (taken_w > fallthru_w);
}

void El_create_on_off_trace_predicates(El_cpr_chain *chain, Operand &on_pred,
					Operand &off_pred)
{
    Reg *new_reg;

    new_reg = new Reg(EL_STANDARD_PREDICATE);
    Operand new_operand1(new_reg);
    on_pred = new_operand1;
    chain->set_pred_on(on_pred);

    new_reg = new Reg(EL_STANDARD_PREDICATE);
    Operand new_operand2(new_reg);
    off_pred = new_operand2;
    chain->set_pred_off(off_pred);
}


/*==============================================================================*/
/*
 *	HB normalization for CPR
 */
/*==============================================================================*/

void El_normalize_cmpp_chain(El_cpr_chain *chain)
{
    Dlist<Pair<Op*, Op*> > cur_group;
    Pair<Op*, Op*> cur_tuple;
    Op *branch_op, *cmpp_op;
    bool swap_dests = false, is_UN_UC, is_UC_UN, is_UN_UN;
    Opcode opp_opc;
    Operand pred_src, tmp_operand;

    cur_group = chain->get_cmpp_branch_op_list();
    for (Dlist_iterator< Pair<Op*, Op*> > g_iter(cur_group);
		g_iter!=0; g_iter++) {
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


/*==============================================================================*/
/*
 *	CPR transform utilities
 */
/*==============================================================================*/


/*
 *	On-trace predicate is an AND-type.  It should be initialized to
 *	the FRP of the first cmpp in the chain.  Note in a sequence of
 *	chains, this is true for the first block, and for each subsequent
 *	block it is the fallthru pred of the previous chain.
 */
void El_insert_initializer_for_on_trace_pred(Hyperblock *hb, Operand &on_pred,
					Operand &incoming_pred, Op *first_cmpp)
{
    if (incoming_pred.is_predicate_true()) {
        Op *new_op = new Op(PRED_SET);
        new_op->set_dest(DEST1, on_pred);
        Pred_lit* new_pred_lit = new Pred_lit(true);
        Operand pred_true(new_pred_lit);
        new_op->set_src(PRED1, pred_true);

        El_insert_op_after_merge(hb, new_op);
    }
    else {
	// Temporarily we will insert both a PRED_SET, and the CMPP, the PRED_SET
	// is because the initialization does not fit impact stylization, argh!!
	Op *new_op = new Op(PRED_SET);
        new_op->set_dest(DEST1, on_pred);
        Pred_lit* new_pred_lit = new Pred_lit(true);
        Operand pred_true(new_pred_lit);
        new_op->set_src(PRED1, pred_true);
        El_insert_op_after_merge(hb, new_op);

	/*
	 * hongtaoz: CMPP initialze on-trace pred to true if the incoming pred is true, false if the incoming pred is false.
	 * The CMPP is needed because the scheduler assumes on-trace and off-trace predicates are disjoint. If the on-trace
	 * predicate is initialzed by pred_set regardless the incoming predicate, the on-trace and off-trace predicates are
	 * not disjoint and wrong schedule can be produced.
	 */
        new_op = new Op((Opcode)CMPP_W_EQ_UN_UN);
	new_op->set_dest(DEST1, on_pred);
	new_op->set_src(PRED1, incoming_pred);
	Int_lit* new_int_lit = new Int_lit(0);
	Operand new_src(new_int_lit);
	new_op->set_src(SRC1, new_src);
	new_op->set_src(SRC2, new_src);
	El_insert_op_before(new_op, first_cmpp);
    }
}

/*
 *	Off-trace predicate is an OR-type, so insert a pred_clear at the
 *	top of the HB
 */
void El_insert_initializer_for_off_trace_pred(Hyperblock *hb, Operand &off_pred)
{
    Op *new_op = new Op(PRED_CLEAR);
    new_op->set_dest(DEST1, off_pred);
    Pred_lit* new_pred_lit = new Pred_lit(true);
    Operand pred_true(new_pred_lit);
    new_op->set_src(PRED1, pred_true);

    El_insert_op_after_merge(hb, new_op);
} 

void El_insert_cpr_cmpp_op(El_cpr_chain *chain, Hyperblock *hb, Op *cmpp_op, Op *branch_op,
	Operand &on_pred, Operand &off_pred, Operand &incoming_pred, bool invert)
{
    Opcode opc;
    Op *new_op;

    /* Create ON/AC opcode with same root opc as cmpp_op */
    opc = cmpp_op->opcode();
    if (invert)
	opc = get_opposite_cond(opc);
    opc = make_AC_pred(opc, DEST1);
    opc = make_ON_pred(opc, DEST2);

    new_op = new Op(opc);
    new_op->set_dest(DEST1, on_pred);
    new_op->set_dest(DEST2, off_pred);
    new_op->set_src(SRC1, cmpp_op->src(SRC1));
    new_op->set_src(SRC2, cmpp_op->src(SRC2));
    new_op->set_src(PRED1, incoming_pred);

    El_insert_op_after(new_op, cmpp_op);
    chain->add_cpr_cmpp_op(new_op);
}

Hyperblock *El_insert_compensation_hb(Compound_region *cr)
{
    Hyperblock *new_hb;
    Compound_region *parent;
    Procedure *proc;
    Op *cm_op, *target_op, *exit_op;

    new_hb = new Hyperblock();
    new_hb->weight = 0.0;

    /* Insertion will occur in the parent of hb */
    parent = cr->parent();
    if (parent == NULL)
	El_punt("El_insert_bypass_branch_target_hb: cr %d has NULL parent",
			cr->id());
    parent->add_region(new_hb);
    new_hb->set_parent(parent);

    /* Insert a control merge into the new HB */
    cm_op = new Op(C_MERGE);
    new_hb->add_region(cm_op);
    cm_op->set_parent(new_hb);
    new_hb->add_entry(cm_op);

#if 0
    /* Add a pbr/bru to the new HB, target the block subsequent to cr.  This
       branch should never be taken, but do this by convention */
    next_r = parent->successor_in_subregion_list(cr);
    if (next_r==NULL)
	El_punt("El_insert_compensation_hb: cr %d has no subsequent reg",
			cr->id());
    target_op = get_first_region_op_from_subregions(next_r);
#endif

    // SAM 2-98, set target of compensation block to the procedure exit
    // block because the above causes conservative dataflow analysis results
    if (! new_hb->parent()->is_procedure())
	El_punt("El_insert_compensation_hb: new_hb parent not set correctly");
    proc = (Procedure *)new_hb->parent();
    Region_exit_ops exit_i(proc); 
    exit_op = *exit_i;
    if (exit_op == NULL)
	El_punt("El_insert_compensation_hb: procedure has no exit ops");
    target_op = get_first_region_op_from_subregions(exit_op->parent());
    if (! is_control_merge(target_op))
	El_punt("El_insert_compensation_hb: corrupt exit block for proc");

    El_add_jump_to_region(new_hb, target_op);

    return (new_hb);
}

void El_insert_off_trace_branch(El_cpr_chain *chain, Hyperblock *hb, Op *branch_op,
				Operand &off_pred, Compound_region *target_cr)
{
    Op *cm_op, *pbr_op, *bru_op, *target_op;
    Operand operand;
    Edge *edge;
    Control_flow_freq *cfreq;
    double ft_weight;

    /* Create a new control merge op */
    cm_op = new Op(C_MERGE);

    /* Insert point is after the c_merge branch op */
    El_insert_op_after(cm_op, branch_op);

    /* Create the new pbr op and fill in the operands */
    pbr_op = new Op(PBR);
    operand = new Reg(EL_STANDARD_BRANCH, BTR, virtual_register_number);
    pbr_op->set_dest(DEST1, operand);
    operand = new Cb_operand(target_cr->id());
    pbr_op->set_src(SRC1, operand);
    operand = new Int_lit(0);
    pbr_op->set_src(SRC2, operand);
    operand = new Pred_lit(true);
    pbr_op->set_src(PRED1, operand);
    El_insert_op_after(pbr_op, cm_op);

    bru_op = new Op(BRU);
    bru_op->set_src(SRC1, pbr_op->dest(DEST1));
    bru_op->set_src(PRED1, off_pred);
    El_insert_op_after(bru_op, pbr_op);
    hb->add_exit(bru_op);

    target_op = get_first_region_op_from_subregions(target_cr);
    edge = C0_connect(bru_op, target_op);
    cfreq = new Control_flow_freq();
    cfreq->freq = target_cr->weight;
    set_control_flow_freq(edge, cfreq);
    bru_op->add_outedge_recursively(edge);
    target_op->add_inedge_recursively(edge);

    edge = get_outgoing_CONTROL0_fall_through_edge(bru_op);
    ft_weight = El_calc_op_weight(bru_op);
    ft_weight -= target_cr->weight;
    if (ft_weight < 0.0)
	ft_weight = 0.0;
    cfreq = new Control_flow_freq();
    cfreq->freq = ft_weight;
    set_control_flow_freq(edge, cfreq);

    chain->set_cpr_branch_op(bru_op);
}

void El_reassociate_ops_with_new_on_trace_pred(Hyperblock *hb, Op *branch_op,
				Operand &on_pred, Hash_set<Operand> &outgoing_preds)
{
    Op *cur_op, *start_op;

    start_op = branch_op->next();
    if (start_op == NULL)
	return;

    for (Region_ops_C0_order o_iter(hb, start_op); o_iter!=0; o_iter++) {
	cur_op = *o_iter;
	if (outgoing_preds.is_member(cur_op->src(PRED1))) {
	    cur_op->src(PRED1) = on_pred;
	}
    }
}

void El_reassociate_branch_with_new_on_trace_pred(Hyperblock *hb, Op *branch_op,
					Operand &on_pred, Operand &incoming_pred)
{
    if (is_brct(branch_op) || is_brcf(branch_op)) {
	branch_op->src(SRC2) = on_pred;
	branch_op->src(PRED1) = incoming_pred;
    }
    else if (is_bru(branch_op)) {
	branch_op->src(PRED1) = on_pred;
    }
    else {
	El_punt("El_reassociate_branch_with_new_on_trace_pred: unknown branch %d",
			branch_op->id());
    }
}

void El_compute_derived_preds(Hyperblock *hb, El_cpr_chain *last_chain, El_cpr_chain *chain,
		Hash_set<Operand> &derived_on_preds,
		Hash_set<Operand> &derived_off_preds)
{
    Op *op, *start_op, *end_op;
    Operand pred, dst_pred;

    derived_on_preds.clear();
    derived_off_preds.clear();

    if (last_chain == NULL)
	return;

    derived_on_preds += last_chain->get_pred_on();
    derived_off_preds += last_chain->get_pred_off();
    start_op = last_chain->get_last_cmpp_op();
    if (chain != NULL) {
	end_op = chain->get_first_cmpp_op();
    }
    else {
	end_op = NULL;
    }
    for (Region_ops_C0_order op_i(hb, start_op); op_i!=0; op_i++) {
	op = *op_i;
	if (op == end_op)
	    break;
	if (! is_cmpp(op))
	    continue;
	pred = op->src(PRED1);
	if (derived_on_preds.is_member(pred)) {
	    for (Op_explicit_dests dst_i(op); dst_i!=0; dst_i++) {
		dst_pred = *dst_i;
		if (! dst_pred.is_reg())
		    continue;
		if (chain && chain->is_cpr_pred(dst_pred))
		    continue;  // Mini hack to ignore cpr pred initialization ops
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
		if (chain && chain->is_cpr_pred(dst_pred))
		    continue;  // Mini hack to ignore cpr pred initialization ops
		derived_off_preds += dst_pred;
		if (dbg(cpr,3))
		    cdbg << "Adding " << dst_pred << " to derived OFF preds" << endl;
	    }
	}
    }

    derived_on_preds -= last_chain->get_pred_on();
    derived_off_preds -= last_chain->get_pred_on();
}

void El_record_cpr_info_in_attr(Hyperblock *hb, El_cpr_chain_list *hb_chain_list)
{
    Control_cpr_info *attr;
    Dlist<El_cpr_chain*> clist;
    El_cpr_chain *chain, *last_chain;
    Hash_set<Operand> derived_on_preds(hash_operand), derived_off_preds(hash_operand);
    int i;

    attr = get_control_cpr_info(hb);
    if (attr != NULL)
	L_punt("El_record_cpr_info_in_attr: HB %d already has an attr", hb->id());

    attr = new Control_cpr_info(hb_chain_list->get_num_chains());
    clist = hb_chain_list->get_chain_list();
    i = 0;
    chain = NULL;
    for (Dlist_iterator<El_cpr_chain*> d_iter(clist); d_iter!=0; d_iter++) {
	last_chain = chain;
	chain = *d_iter;
	if (dbg(cpr, 3))
	    cdbg << "Considering chain " << i << endl;
	attr->set_on_trace_pred(i, chain->get_pred_on());
	attr->set_off_trace_pred(i, chain->get_pred_off());
	if (dbg(cpr, 3)) {
	    cdbg << "Setting ON trace pred: " << chain->get_pred_on() << endl;
	    cdbg << "Setting OFF trace pred: " << chain->get_pred_off() << endl;
	}

	// Compute the derived predicates if there are any from last_chain
	if (dbg(cpr, 3))
	    cdbg << "Calculating derived preds for last chain: " << (i-1) << endl;
	El_compute_derived_preds(hb, last_chain, chain, derived_on_preds,
					derived_off_preds);

	if (derived_on_preds.size() > 0)
	    attr->set_derived_on_trace_pred(i-1, derived_on_preds);
	if (derived_off_preds.size() > 0)
	    attr->set_derived_off_trace_pred(i-1, derived_off_preds);
	i++;
    }

    // Handle stuff derived from the last chain
    if (dbg(cpr, 3))
	cdbg << "Calculating derived preds for last chain: " << (i-1) << endl;
    El_compute_derived_preds(hb, chain, NULL, derived_on_preds,
                                        derived_off_preds);
    if (derived_on_preds.size() > 0)
	attr->set_derived_on_trace_pred(i-1, derived_on_preds);
    if (derived_off_preds.size() > 0)
	attr->set_derived_off_trace_pred(i-1, derived_off_preds);

    set_control_cpr_info(hb, attr);
}

/*==============================================================================*/
/*
 *	Interface routine
 */
/*==============================================================================*/


void El_insert_bypass_branches(Hyperblock *hb, Cont_cpr_match_result &match_result,
		El_cpr_chain_list *hb_chain_list)
{
    Dlist<Dlist< Pair<Op*, Op*> > > hb_cpr_groups;
    Dlist<bool> cpr_blk_type;
    Dlist< Pair<Op*, Op*> > cur_group;
    Pair<Op* , Op*> cur_tuple;
    Operand on_pred, off_pred, incoming_pred;
    Hash_set<Operand> outgoing_preds(hash_operand);
    Op *first_cmpp_op, *last_cmpp_op, *last_branch_op, *branch_op = NULL, *cmpp_op;
    Compound_region *target_cr;
    bool need_off_trace_branch, need_comp_block, taken_flag;
    El_cpr_chain *cur_chain;

    hb_cpr_groups = match_result.cpr_blk_list;
    if (hb_cpr_groups.size() == 0) {
        if (dbg(cpr, 2))
	    cdbg << "HB " << hb->id() << " has no cpr chains" << endl;
	return;
    } else {
        if (dbg(cpr, 2))
	    cdbg << "insert bypass_branches for HB " << hb->id() << endl;
    }

    cpr_blk_type = match_result.cpr_blk_type;
    for (Dlist_iterator<Dlist< Pair<Op*, Op*> > > d_iter(hb_cpr_groups);
	 d_iter!=0; d_iter++) {
        cur_chain = new El_cpr_chain(*d_iter);
	El_normalize_cmpp_chain(cur_chain);
	
	cur_group = cur_chain->get_cmpp_branch_op_list();

	/* Collect some info about the chain */
	first_cmpp_op = cur_chain->get_first_cmpp_op();
	last_cmpp_op = cur_chain->get_last_cmpp_op();
	last_branch_op = cur_chain->get_last_branch_op();
	incoming_pred = first_cmpp_op->src(PRED1);
	outgoing_preds.clear();
        cur_chain->set_pred_incoming(incoming_pred);

	// Right now need_off_trace_branch, need_comp_block must have the same
	// value, at some future point this may not be true
	taken_flag = cpr_blk_type.pop();
	if (taken_flag) {
	    need_off_trace_branch = false;
	    need_comp_block = false;
	    cur_chain->set_flag(EL_CPR_LAST_BR_TAKEN);
	}
	else {
	    cur_chain->set_flag(EL_CPR_HAS_COMP_BLOCK);
	    need_off_trace_branch = true;
	    need_comp_block = true;
	}

	/* Now insert the new cmpps and the off-trace br (if necc) */
	El_create_on_off_trace_predicates(cur_chain, on_pred, off_pred);
	El_insert_initializer_for_off_trace_pred(hb, off_pred);
	El_insert_initializer_for_on_trace_pred(hb, on_pred, incoming_pred,
						first_cmpp_op);
	for (Dlist_iterator< Pair<Op*, Op*> > g_iter(cur_group);
	     g_iter!=0; g_iter++) {
	    cur_tuple = *g_iter;
	    cmpp_op = cur_tuple.second;
	    branch_op = cur_tuple.first;
	    if (cmpp_op == NULL)
		continue;
	    outgoing_preds += cmpp_op->dest(DEST2);
	    if ((branch_op == last_branch_op) && (! need_off_trace_branch))
	        El_insert_cpr_cmpp_op(cur_chain, hb, cmpp_op, branch_op, on_pred,
					off_pred, incoming_pred, true);
	    else
	        El_insert_cpr_cmpp_op(cur_chain, hb, cmpp_op, branch_op, on_pred,
					off_pred, incoming_pred, false);
	}

	if (need_comp_block) {
	    target_cr = (Compound_region *)El_insert_compensation_hb(hb);
	    cur_chain->set_comp_block((Hyperblock *)target_cr);
	    target_cr->set_flag(EL_REGION_CPR_COMP_BLOCK);
	}
	else {
	    target_cr = NULL;
	}
	if (need_off_trace_branch) {
	    El_insert_off_trace_branch(cur_chain, hb, branch_op, off_pred, target_cr);
	    El_reassociate_ops_with_new_on_trace_pred(hb, branch_op, on_pred,
							outgoing_preds);
	}
	else {
	    cur_chain->set_cpr_branch_op(branch_op);
	    El_reassociate_branch_with_new_on_trace_pred(hb, branch_op, on_pred,
							incoming_pred);
	}

	hb_chain_list->add_chain(cur_chain);
    }

    hb->set_flag(EL_REGION_CPR_BLOCK);
    El_record_cpr_info_in_attr(hb, hb_chain_list);

    if (dbg(cpr, 2))
        hb_chain_list->print_chain_list();
}
