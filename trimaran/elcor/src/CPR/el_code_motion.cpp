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
 *      File:           el_code_motion.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        April 1997
 *      Description:    Off trace code motion for CPR
 *
 *===========================================================================*/

#include "defs.h"
#include "dbg.h"
#include "region.h"
#include "iterators.h"
#include "el_cpr_init.h"
#include "hash_set.h"
#include "attributes.h"
#include "dlist.h"
#include "tuples.h"
#include "opcode_properties.h"
#include "opcode_cmpp.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "connect.h"
#include "edge_utilities.h"
#include "edge_drawing.h"
#include "region_utilities.h"
#include "el_bypass_branch.h"



/*======================================================================*/
/*
 *      Code motion utilities
 */
/*======================================================================*/

void El_print_cpr_op_set(Hash_set<Op*> &x)
{
    Op *op;

    cdbg << "\t";
    for (Hash_set_iterator<Op*> hi(x); hi!=0; hi++) {
	op = *hi;
	cdbg << op->id() << " ";
    }
    cdbg << endl;
}

void El_init_off_trace_code_motion(Hyperblock *hb)
{
    Op *op;

    for (Region_ops_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	op->set_flag(EL_OPER_MARK1);
    }
}

/*
 *	Returns NULL if the first op of the chain is the start of the HB
 */
Op *El_find_chain_first_op(Hyperblock *hb, El_cpr_chain *chain)
{
    Op *first_branch, *op, *first_op;

    first_branch = chain->get_first_branch_op();
    assert(first_branch != NULL);

    first_branch = first_branch->prev();
    if (first_branch == NULL)
	return (NULL);

    first_op = NULL;
    for (Region_ops_reverse_C0_order o_iter(hb, first_branch); o_iter!=0; o_iter++) {
	op = *o_iter;
	if (is_branch(op)) {
	    first_op = op;
	    break;
	}
    }

    if (first_op == NULL)
	return (NULL);
    else
	return (first_op->next());
}


/*
 *	Calculate the relevant ON trace ops from the perspective of the
 *	last op in the cpr block.
 */
void El_calculate_on_trace_ops(Hyperblock *hb, El_cpr_chain *chain,
				Hash_set<Op*> &on_trace_ops)
{
    Op *op, *op2, *start_op;

    on_trace_ops.clear();

    if (chain->is_flag(EL_CPR_LAST_BR_TAKEN)) {
	start_op = chain->get_last_branch_op();
	List<Op*> &relops = start_op->get_relops();
	for (List_iterator<Op*> l_iter(relops); l_iter!=0; l_iter++) {
	    op = *l_iter;
	    on_trace_ops += op;
	}
    }
    else {
	start_op = chain->get_cpr_branch_op();
	assert(start_op != NULL);

	start_op = start_op->next();
	assert(start_op != NULL);
        for (Region_ops_C0_order o_iter(hb, start_op); o_iter!=0; o_iter++) {
	    op = *o_iter;
	    on_trace_ops += op;
	    if (is_branch(op)) {
		List<Op*> &relops = op->get_relops();
		for (List_iterator<Op*> l_iter(relops); l_iter!=0; l_iter++) {
		    op2 = *l_iter;
		    on_trace_ops += op2;
		}
	    }
	}
    }
}


/*
 *	Calculate the relevant OFF trace ops from the perspective of the
 *	last op in the cpr block.
 */
void El_calculate_off_trace_ops(Hyperblock *hb, El_cpr_chain *chain,
				Hash_set<Op*> &off_trace_ops)
{
    Op *op, *op2, *start_op;

    off_trace_ops.clear();

    if (chain->is_flag(EL_CPR_LAST_BR_TAKEN)) {
	start_op = chain->get_last_branch_op();
	assert(start_op != NULL);

	start_op = start_op->next();
	assert(start_op != NULL);
        for (Region_ops_C0_order o_iter(hb, start_op); o_iter!=0; o_iter++) {
	    op = *o_iter;
	    off_trace_ops += op;
	    if (is_branch(op)) {
		List<Op*> &relops = op->get_relops();
		for (List_iterator<Op*> l_iter(relops); l_iter!=0; l_iter++) {
		    op2 = *l_iter;
		    off_trace_ops += op2;
		}
	    }
	}
    }
    else {
	start_op = chain->get_last_branch_op();
	off_trace_ops += start_op;
	List<Op*> &relops = start_op->get_relops();
	for (List_iterator<Op*> l_iter(relops); l_iter!=0; l_iter++) {
	    op = *l_iter;
	    off_trace_ops += op;
	}
    }
}


/* 
 *	Code motion pass 1 - forward traversal, identify ops which must move
 *	to ensure correctness if the cmpp and branches are moving
 */
void El_do_code_motion_pass1(Hyperblock *hb, Op *first_op, Op *last_op,
			     El_cpr_chain *chain, Hash_set<Op *> &to_move_ops)
{
    Op *op, *op2, *src_op, *last_branch_op;
    Edge *edge;
    bool found_last_op;

    last_branch_op = chain->get_last_branch_op();

    found_last_op = false;
    for (Region_ops_C0_order o_iter(hb, first_op); o_iter!=0; o_iter++) {
	op = *o_iter;

	if (found_last_op == true)
	    break;
	if (op == last_op)
	    found_last_op = true;

	if (is_pseudo(op))
	    continue;
	if (is_predicate_clear(op) || is_predicate_set(op))
	    continue;
	if (dbg(cpr, 3))
	    cdbg << "Code motion pass1 considering op " << op->id() << endl;

	if (chain->is_chain_branch_op(op)) {
	    if ((chain->is_flag(EL_CPR_LAST_BR_TAKEN)) && (op == last_branch_op))
		continue;
	    to_move_ops += op;
	    if (dbg(cpr, 3))
		cdbg << "\tOp added to tomove set, is chain branch op" << endl;
	    List<Op*> &relops = op->get_relops();
	    for (List_iterator<Op*> l_iter(relops); l_iter!=0; l_iter++) {
		op2 = *l_iter;
		to_move_ops += op2;
	    }
	}
	else if (chain->is_chain_cmpp_op(op)) {
	    to_move_ops += op;
	    if (dbg(cpr, 3))
		cdbg << "\tOp added to tomove set, is chain cmpp op" << endl;
	}
	else {
	    /* Check if op is a dependence successor of an op in to_move_ops */
	    for (Op_inedges e_iter(op, NON_CONTROL_INEDGES); e_iter!=0; e_iter++) {
		edge = *e_iter;
		src_op = edge->src();
		if (to_move_ops.is_member(src_op)) {
		    to_move_ops += op;
		    if (dbg(cpr, 3)) {
			cdbg << "\tOp added to tomove set, is succ op" << endl;
			cdbg << *edge << endl;
		    }
		    break;
		}
	    }
	}
    }
}

/*
 *	Code motion pass 2 - backward traversal to compute split set
 *	those ops in tomove that are live on-trace
 */
void El_do_code_motion_pass2(Hyperblock *hb, Op *first_op, Op *last_op,
                             El_cpr_chain *chain, Hash_set<Op *> &to_move_ops,
			     Hash_set<Op *> &to_split_ops)
{
    Op *op, *dest_op;
    Edge *edge;
    Hash_set<Op*> on_trace_ops(hash_op_ptr);
    bool found_first_op;

    El_calculate_on_trace_ops(hb, chain, on_trace_ops);

    if (dbg(cpr, 3)) {
      cdbg << "code motion pass2 on_trace_ops: ";
      El_print_cpr_op_set(on_trace_ops);
    }

    found_first_op = false;
    for (Region_ops_reverse_C0_order o_iter(hb, last_op); o_iter!=0; o_iter++) {
	op = *o_iter;

	if (found_first_op == true)
	    break;
	if (op == first_op)
	    found_first_op = true;

	if (! to_move_ops.is_member(op))
	    continue;
	if (chain->is_chain_cmpp_op(op))
	    continue;
	if (chain->is_chain_branch_op(op))
	    continue;
	if (dbg(cpr, 3))
	    cdbg << "Code motion pass2 considering op " << op->id() << endl;

	// hongtaoz: store op should stay on trace.
	if (is_store(op))
	  to_split_ops += op;

	/* Check if op is a dependence predecessor of an op in to_split_ops or
	   an op in on_trace_ops */
	for (Op_outedges e_iter(op,NON_CONTROL0_OUTEDGES); e_iter!=0; e_iter++) {
	    edge = *e_iter;
	    if (edge->is_reg_anti() || edge->is_reg_out())
		continue;
	    dest_op = edge->dest();
	    if (to_split_ops.is_member(dest_op)) {
	        to_split_ops += op;
	        if (dbg(cpr, 3))
		    cdbg << "\tOp added to tosplit set, is pred of tosplit" 
			 << dest_op->id() << endl;
		break;
	    }
	    else if (on_trace_ops.is_member(dest_op)) {
	        to_split_ops += op;
	        if (dbg(cpr, 3))
		    cdbg << "\tOp added to tosplit set, is pred of ontrace"
			 << dest_op->id() << endl;
		break;
	    }
	}	
    }
}

/*
 *	Code motion pass 3 - reverse traversal to identify ops which are
 *	profitable to move off trace, ie those ops whose results are only
 *	needed off trace.
 */
void El_do_code_motion_pass3(Hyperblock *hb, Op *first_op, Op *last_op,
                             El_cpr_chain *chain, Hash_set<Op *> &to_move_ops,
			     Hash_set<Op *> &to_split_ops)
{
    Op *op, *dest_op;
    Edge *edge;
    Hash_set<Op*> no_split_ops(hash_op_ptr), off_trace_ops(hash_op_ptr);
    bool safe_to_move, found_first_op;

    no_split_ops = to_move_ops;
    no_split_ops -= to_split_ops;
    El_calculate_off_trace_ops(hb, chain, off_trace_ops);

    found_first_op = false;
    for (Region_ops_reverse_C0_order o_iter(hb, last_op); o_iter!=0; o_iter++) {
	op = *o_iter;

	if (found_first_op == true)
	    break;
	if (op == first_op)
	    found_first_op = true;

	if (is_pseudo(op))
	    continue;
	if (is_store(op))
	  continue;
	if (is_predicate_clear(op) || is_predicate_set(op))
            continue;
	if (to_move_ops.is_member(op))
	    continue;
	if (is_branch(op))
	    continue;
	if (dbg(cpr, 3))
	    cdbg << "Code motion pass3 considering op " << op->id() << endl;

	if (! op->flag(EL_OPER_MARK1)) {
	    if (dbg(cpr, 3))
		cdbg << "\tOp not added to tomove set, no outedges" << endl;
	    continue;
	}

	safe_to_move = true;
	for (Op_outedges e_iter(op,NON_CONTROL_OUTEDGES); e_iter!=0; e_iter++) {
	    edge = *e_iter;
	    dest_op = real_op(edge->dest());
	    //if (is_pseudo(dest_op))
	    //	continue;
	    if (! (no_split_ops.is_member(dest_op) ||
		   off_trace_ops.is_member(dest_op))) {
		safe_to_move = false;
		break;
	    }
	}

	if (safe_to_move == true) {
	    to_move_ops += op;
	    no_split_ops += op;
	    if (dbg(cpr, 3))
	        cdbg << "\tOp added to tomove set" << endl;
	}
    }
}

/*
 *	From the to_move_ops set, create a list of these ops based on the
 *	sequential order of the ops.  Note, to_move_ops contains d_switches
 *	for the branches, to_move_op_list does not.
 */
void El_create_to_move_op_list(Hyperblock *hb, Op *first_op, Op *last_op,
		Hash_set<Op *> &to_move_ops, Dlist<Op*> &to_move_op_list)
{
    Op *op;
    bool found_last_op;

    found_last_op = false;
    for (Region_ops_C0_order o_iter(hb, first_op); o_iter!=0; o_iter++) {
	op = *o_iter;

	if (found_last_op == true)
	    break;
	if (op == last_op)
	    found_last_op = true;

	if (to_move_ops.is_member(op))
	    to_move_op_list.push_tail(op);
    }
}

/*
 *	Perform the actual off-trace code motion.  to_move_op_list contains all the
 *	ops which are to be moved, to_split_ops contains the subset of ops which
 *	are to be replicated both on and off trace.   Return the cumulative
 *	exit frequency for branches that are moved off-trace.
 */
double El_do_off_trace_code_motion(Hyperblock *hb, El_cpr_chain *chain,
	Dlist<Op*> &to_move_op_list, Hash_set<Op*> &to_split_ops,
	Op *off_trace_insert_op, Op* on_trace_insert_op)
{
    Compound_region *off_trace_parent, *on_trace_parent;
    Op *op, *copy_op, *next_op;
    Edge *edge;
    Control_flow_freq *cfreq;
    double taken_weight;

    off_trace_parent = off_trace_insert_op->parent();
    on_trace_parent = on_trace_insert_op->parent();
    taken_weight = 0.0;

    for (Dlist_iterator<Op*> o_iter(to_move_op_list, true); o_iter!=0; o_iter--) {
	op = *o_iter;
	delete_op_edges(op);
	op->reset_flag(EL_OPER_MARK1);

	if (to_split_ops.is_member(op)) {
	    if (is_branch(op))
		El_punt("El_do_off_trace_code_motion: to_split_op %d is a br",
				op->id());

	    /* Duplicate op, place copy off-trace.  If the ops pred is the root pred
		change it to the off-trace pred.  This allows the op to move back on
		trace with a global scheduler. It is not a required step */
	    El_remove_op(op);
	    copy_op = new Op(*op);
	    copy_op->renumber();
	    El_insert_op_after(copy_op, off_trace_insert_op);
	    if (copy_op->src(PRED1) == chain->get_pred_incoming())
		copy_op->src(PRED1) = chain->get_pred_off();

	    /* Modify the pred of op to pred_on, move the op to on_trace insert pt */
	    op->src(PRED1) = chain->get_pred_on();
	    El_insert_op_after(op, on_trace_insert_op);
	    
	}
	else if (is_branch(op)) {
	    /* Branch just needs to be moved off-trace like a normal op, but it is
		more messy to get the edges, regions corrected for the move */
	    next_op = op->next();
	    edge = get_outgoing_CONTROL0_non_fall_through_edge(op);
	    if (edge == NULL)
		El_punt("El_do_off_trace_code_motion: outgoing T edge not found for %d",
				op->id());
	    cfreq = get_control_flow_freq(edge);
	    if (cfreq == NULL)
		El_punt("El_do_off_trace_code_motion: edge for op %d has no cfreq attr",
				op->id());
	    taken_weight += cfreq->freq;
	    op->remove_outop_recursively(edge);
	    op->remove_outedge_recursively(edge);
	    El_remove_op(op);
	    El_insert_op_after(op, off_trace_insert_op);
	    op->add_outop_recursively(edge);
	    op->add_outedge_recursively(edge);
#if 0
	    if (op == chain->get_first_branch_op())
		op->src(PRED1) = chain->get_pred_off();
	    if (op->src(PRED1) == chain->get_pred_incoming())
		op->src(PRED1) = chain->get_pred_off();
#endif

	    /* Move the c_merge associated with the branch (next_op) */
	    if (! is_control_merge(next_op))
		El_punt("El_do_off_trace_code_motion: op after %d is not a CM",
				op->id());
	    delete_op_edges(next_op);
	    El_remove_op(next_op);
	    El_insert_op_after(next_op, op);
	}
	else {
	    /* Just move the op off-trace.  Change the predicate of the op if it
		is the root predicate to the off-trace pred. */
	    El_remove_op(op);
	    El_insert_op_after(op, off_trace_insert_op);
#if 0
	    if (op == chain->get_first_cmpp_op())
		op->src(PRED1) = chain->get_pred_off();
	    if (op->src(PRED1) == chain->get_pred_incoming())
		op->src(PRED1) = chain->get_pred_off();
#endif
	}
    }

    return (taken_weight);
}

void El_fixup_comp_block(Hyperblock *comp_block, double off_trace_weight)
{
    comp_block->weight += off_trace_weight;
    El_install_missing_cfreq_attrs(comp_block);
}

void El_fixup_cpr_hb(Hyperblock *hb, El_cpr_chain *chain, double off_trace_weight)
{
    Op *branch_op;
    Edge *ft_edge, *taken_edge;
    Control_flow_freq *ft_cfreq, *taken_cfreq;

    branch_op = chain->get_cpr_branch_op();

    ft_edge = get_outgoing_CONTROL0_fall_through_edge(branch_op);
    ft_cfreq = get_control_flow_freq(ft_edge);
    if (ft_cfreq == NULL)
	El_punt("El_fixup_cpr_hb: missing ft cfreq on branch %d", branch_op->id());

    taken_edge = get_outgoing_CONTROL0_non_fall_through_edge(branch_op);
    taken_cfreq = get_control_flow_freq(taken_edge);
    if (taken_cfreq == NULL)
	El_punt("El_fixup_cpr_hb: missing taken cfreq on branch %d", branch_op->id());

    if (chain->is_flag(EL_CPR_HAS_COMP_BLOCK)) {
	taken_cfreq->freq += off_trace_weight;
    }
    else {
	ft_cfreq->freq += off_trace_weight;
    }

    El_install_missing_cfreq_attrs(hb);
}

/*======================================================================*/
/*
 *      External routines
 */
/*======================================================================*/


/*
 *      Edges are drawn when this function is entered
 */
void El_off_trace_code_motion(Hyperblock *hb, El_cpr_chain_list *hb_chain_list)
{
    Dlist<El_cpr_chain*> chain_list;
    El_cpr_chain *chain;
    Op *first_op, *last_op, *on_trace_insert_op, *off_trace_insert_op;
    Hyperblock *comp_block = NULL;
    Hash_set<Op *> to_move_ops(hash_op_ptr), to_split_ops(hash_op_ptr);
    Dlist<Op *> to_move_op_list;
    double off_trace_weight;
    int i;

    if (hb_chain_list->get_num_chains() == 0)
	return;

    if (dbg(cpr, 2))
	cdbg << "******* Enter code motion for HB: " << hb->id() << endl;

    El_init_off_trace_code_motion(hb);
    chain_list = hb_chain_list->get_chain_list();
    i = 0;
    for (Dlist_iterator<El_cpr_chain *> c_iter(chain_list);
			c_iter!=0; c_iter++) {
	chain = *c_iter;

	first_op = El_find_chain_first_op(hb, chain);
	last_op = chain->get_last_branch_op();
        if (dbg(cpr, 3))
	    cdbg << "<<<<<<<<< Chain " << i++ << " >>>>>>>>>>>" << endl;

	to_move_ops.clear();
	to_split_ops.clear();
	to_move_op_list.clear();

	El_do_code_motion_pass1(hb, first_op, last_op, chain, to_move_ops);
	if (dbg(cpr, 6)) {
	    cdbg << "To move ops after pass1" << endl;
	    El_print_cpr_op_set(to_move_ops);
	}

	El_do_code_motion_pass2(hb, first_op, last_op, chain, to_move_ops,
				to_split_ops);
	if (dbg(cpr, 6)) {
	    cdbg << "To split ops after pass2" << endl;
	    El_print_cpr_op_set(to_split_ops);
	    cdbg << "To move ops after pass2" << endl;
	    El_print_cpr_op_set(to_move_ops);
	}

	El_do_code_motion_pass3(hb, first_op, last_op, chain, to_move_ops,
				to_split_ops);
	if (dbg(cpr, 6)) {
	    cdbg << "To move ops after pass3" << endl;
	    El_print_cpr_op_set(to_move_ops);
	    cdbg << "To split ops after pass3" << endl;
	    El_print_cpr_op_set(to_split_ops);
	}

	El_create_to_move_op_list(hb, first_op, last_op, to_move_ops, to_move_op_list);

	if (chain->is_flag(EL_CPR_HAS_COMP_BLOCK)) {
	    comp_block = chain->get_comp_block();
	    off_trace_insert_op = get_first_region_op_from_subregions(comp_block);
	    on_trace_insert_op = chain->get_cpr_branch_op()->prev();
	}
	else {
	    off_trace_insert_op = chain->get_cpr_branch_op()->next();
	    on_trace_insert_op = chain->get_cpr_branch_op()->prev();
	}

	off_trace_weight = El_do_off_trace_code_motion(hb, chain, to_move_op_list,
				to_split_ops, off_trace_insert_op, on_trace_insert_op);

	if (chain->is_flag(EL_CPR_HAS_COMP_BLOCK))
	    El_fixup_comp_block(comp_block, off_trace_weight);
	El_fixup_cpr_hb(hb, chain, off_trace_weight);

    }
}
