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
//									 //
//      File:           el_local_const_folding.cpp                       //
//      Authors:        Scott A. Mahlke                                  //
//      Created:        August 1999                                      //
//      Description:    Constant folding optimizations                   //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "region_utilities.h"
#include "connect.h"
#include "edge_utilities.h"
#include "el_control.h"
#include "el_opti_utilities.h"

/*============================================================================*/
/*
 *	Constant folding optimizations
 *		Includes ops with all constant operands and constants that
 *		allow identity functions to be exploited, e.g., x+0=x
 */
/*============================================================================*/

/*
 *	y = 0+x, 0|x, 0^x
 *	=>
 *	y = x
 */
int El_opti_local_const_fold_1(Compound_region *reg)
{
    Operand src1, src2, dest, pred;
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 1: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_add(op) || is_or(op) || is_xor(op)))
	    continue;
	src1 = op->src(SRC1);
	if (! El_is_int_zero(src1))
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 1 (0 +|^ x): op " << op->id() << endl;

	src2 = op->src(SRC2);
	dest = op->dest(DEST1);
	pred = op->src(PRED1);
	El_convert_to_move(op, dest, src2, pred);

	count++;
    }

    return (count);
}

/*
 *	y = x+0,x-0,x|0,x^0,x<<0,x>>0
 *	=>
 *	y = x
 */
int El_opti_local_const_fold_2(Compound_region *reg)
{
    Operand src1, src2, dest, pred;
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 2: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_add(op) || is_int_sub(op) || is_or(op) || is_xor(op) ||
	       is_int_lshift(op) || is_int_rshift(op)))
	    continue;
	src2 = op->src(SRC2);
	if (! El_is_int_zero(src2))
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 2 (x +-|^<<>> 0): op " << op->id() << endl;

	src1 = op->src(SRC1);
	dest = op->dest(DEST1);
	pred = op->src(PRED1);
	El_convert_to_move(op, dest, src1, pred);

	count++;
    }

    return (count);
}

/*
 *	y = 0*x,0/x,0<<x,0>>x,0&x
 *	=>
 *	y = 0
 */
int El_opti_local_const_fold_3(Compound_region *reg)
{
    Operand src1, dest, pred;
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 3: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_mul(op) || is_int_div(op) || is_and(op) ||
	       is_int_lshift(op) || is_int_rshift(op)))
	    continue;
	src1 = op->src(SRC1);
	if (! El_is_int_zero(src1))
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 3 (0 */&<<>>): op " << op->id() << endl;

	Operand lit0(new Int_lit(0));
	dest = op->dest(DEST1);
	pred = op->src(PRED1);
	El_convert_to_move(op, dest, lit0, pred);

	count++;
    }

    return (count);
}

/*
 *	y = x*0,x&0
 *	=>
 *	y = 0
 */
int El_opti_local_const_fold_4(Compound_region *reg)
{
    Operand src2, dest, pred;
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 4: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_mul(op) || is_and(op)))
	    continue;
	src2 = op->src(SRC2);
	if (! El_is_int_zero(src2))
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 4 (x *& 0): op " << op->id() << endl;

	Operand lit0(new Int_lit(0));
	dest = op->dest(DEST1);
	pred = op->src(PRED1);
	El_convert_to_move(op, dest, lit0, pred);

	count++;
    }

    return (count);
}

/*
 *	y = 1*x
 *	=>
 *	y = x
 */
int El_opti_local_const_fold_5(Compound_region *reg)
{
    Operand src1, src2, dest, pred;
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 5: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_mul(op)))
	    continue;
	src1 = op->src(SRC1);
	if (! El_is_int_one(src1))
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 5 (1 * x): op " << op->id() << endl;

	src2 = op->src(SRC2);
	dest = op->dest(DEST1);
	pred = op->src(PRED1);
	El_convert_to_move(op, dest, src2, pred);

	count++;
    }

    return (count);
}

/*
 *	y = x*1,x/1
 *	=>
 *	y = x
 */
int El_opti_local_const_fold_6(Compound_region *reg)
{
    Operand src1, src2, dest, pred;
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 6: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_mul(op) || is_int_div(op)))
	    continue;
	src2 = op->src(SRC2);
	if (! El_is_int_one(src2))
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 6 (x */ 1): op " << op->id() << endl;

	src1 = op->src(SRC1);
	dest = op->dest(DEST1);
	pred = op->src(PRED1);
	El_convert_to_move(op, dest, src1, pred);

	count++;
    }

    return (count);
}

/*
 *	y = x-x,x/x
 *	=>
 *	y = 0,1
 */
int El_opti_local_const_fold_7(Compound_region *reg)
{
    Operand src1, src2, dest, pred;
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 7: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_sub(op) || is_int_div(op)))
	    continue;
	src1 = op->src(SRC1);
	src2 = op->src(SRC2);
	if (src1 != src2)
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 7 (x -/ x): op " << op->id() << endl;

	dest = op->dest(DEST1);
	pred = op->src(PRED1);
	if (is_int_sub(op)) {
	    Operand lit0(new Int_lit(0));
	    El_convert_to_move(op, dest, lit0, pred);
	}
	else {
	    Operand lit1(new Int_lit(1));
	    El_convert_to_move(op, dest, lit1, pred);
	}

	count++;
    }

    return (count);
}

/*
 *	y = C1 op C2 if p
 *	=>
 *	y = C3 if p
 */
int El_opti_local_const_fold_8(Compound_region *reg)
{
    Operand src1, src2, dest, pred;
    Op *op, *new_op, *tgt_op;
    List<Op*> dead_list;
    Edge *edge;
    int count=0;

    if (dbg(opti, 3))
        cdbg << "\tEnter local constant folding 8: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
        op = *opi;

	if (is_pseudo(op))
	    continue;
	if (is_move(op))
	    continue;
	if (is_load(op))
	    continue;
	if (is_store(op))
	    continue;
	if (is_brct(op) || is_brcf(op)) {
	    /* This opti handles the always taken brct/brcf */
	    src2 = op->src(SRC2);
	    bool ok = false;
	    if (is_brct(op) && src2.is_predicate_true())
		ok = true;
	    if (is_brcf(op) && src2.is_predicate_false())
		ok = true;
	    if (! ok)
		continue;
	}
	else  {
	    /* Normal case checks for all constant operands */
	    if (! El_all_sources_are_compiletime_constants(op))
	        continue;
	}

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 8 (C1 o C2): op " << op->id() << endl;

	if (is_int_computation_op(op)) {
	    int res = El_evaluate_int_computation_op(op);
	    Operand new_src(new Int_lit(res));
	    dest = op->dest(DEST1);
	    pred = op->src(PRED1);
	    El_convert_to_move(op, dest, new_src, pred);
	}
	else if (is_flt_computation_op(op)) {
	    float res = El_evaluate_flt_computation_op(op);
	    Operand new_src(new Float_lit(res));
	    dest = op->dest(DEST1);
	    pred = op->src(PRED1);
	    El_convert_to_move(op, dest, new_src, pred);
	}
	else if (is_dbl_computation_op(op)) {
	    double res = El_evaluate_dbl_computation_op(op);
	    Operand new_src(new Double_lit(res));
	    dest = op->dest(DEST1);
	    pred = op->src(PRED1);
	    El_convert_to_move(op, dest, new_src, pred);
	}
	else if (is_int_conversion_op(op)) {
	    int res = El_evaluate_int_conversion_op(op);
	    Operand new_src(new Int_lit(res));
	    dest = op->dest(DEST1);
	    pred = op->src(PRED1);
	    El_convert_to_move(op, dest, new_src, pred);
	}
	else if (is_flt_conversion_op(op)) {
	    float res = El_evaluate_flt_conversion_op(op);
	    Operand new_src(new Float_lit(res));
	    dest = op->dest(DEST1);
	    pred = op->src(PRED1);
	    El_convert_to_move(op, dest, new_src, pred);
	}
	else if (is_dbl_conversion_op(op)) {
	    double res = El_evaluate_dbl_conversion_op(op);
	    Operand new_src(new Double_lit(res));
	    dest = op->dest(DEST1);
	    pred = op->src(PRED1);
	    El_convert_to_move(op, dest, new_src, pred);
	}
	else if (is_cmpr(op)) {
	    int res = El_evaluate_cmpr_op(op);
	    Operand new_src(new Int_lit(res));
	    dest = op->dest(DEST1);
	    pred = op->src(PRED1);
	    El_convert_to_move(op, dest, new_src, pred);
	}
	else if (is_cmpp(op)) {
	    bool res = El_evaluate_base_cmpp_op(op);
	    /* DEST1 */
	    new_op = El_create_move_for_cmpp_with_constant_condition(op, DEST1, res);
	    if (new_op != NULL)
		El_insert_op_before(new_op, op);
	    /* DEST2 */
	    new_op = El_create_move_for_cmpp_with_constant_condition(op, DEST2, res);
	    if (new_op != NULL)
		El_insert_op_before(new_op, op);
	    /* Now, mark the cmpp for deletion */
	    dead_list.add_tail(op);
	}
	else if (is_brct(op) || is_brcf(op)) {
            // remove all ops after this branch. leaving them in will cause
            // problems because ops after a BRU are unreachable via the
            // Region_ops_C0_order iterator. -KF 12/2004
            Region_ops_C0_order opi2(reg, op);
            opi2++;
            while (opi2 != 0) {
              Op* dead_op = *opi2;
              opi2++;
              if (is_branch(dead_op)) {
                edge = get_outgoing_CONTROL0_non_fall_through_edge(dead_op);
                // only want to remove this edge if there was also a fall-through
                // edge. (otherwise El_remove_op will take care of it)
                if (edge && get_outgoing_CONTROL0_fall_through_edge(dead_op)) {
                  tgt_op = edge->dest();
                  // do i also need to remove inops/outops?  most elcor code
                  // doesn't seem to do it, but the code above does.
                  dead_op->remove_outedge_recursively(edge);
                  tgt_op->remove_inedge_recursively(edge);
                  delete edge;
                }
              }
              El_remove_op(dead_op);
              delete dead_op;
            }

	    /* First remove the fallthru edge */
	    edge = get_outgoing_CONTROL0_fall_through_edge(op);
	    tgt_op = edge->dest();
	    op->remove_outop_recursively(edge);
            op->remove_outedge_recursively(edge);
	    // Unless have unreachable code elimination, removing inops not a good idea
            // tgt_op->remove_inop_recursively(edge);
            tgt_op->remove_inedge_recursively(edge);

	    /* convert the brct/cf to a bru */
	    src1 = op->src(SRC1);
	    pred = op->src(PRED1);
	    El_convert_to_bru(op, src1, pred);
	}
	count++;
    }

    for (List_iterator<Op*> dli(dead_list); dli!=0; dli++) {
	op = *dli;
	C0_remove_op(op);
	op->parent()->remove_region(op);
	delete op;
    }

    return (count);
}

/*
 *	Any op predicated on the literal false can be deleted.  Perhaps this is
 *	redundant with dead code elimination, I wasn't sure, so just impelemented
 *	it anyways..
 */
int El_opti_local_const_fold_9(Compound_region *reg)
{
    Operand pred, brcond;
    Op *op, *new_op, *tgt_op;
    List<Op*> dead_list;
    Edge *edge;
    int count=0;

    if (dbg(opti, 3))
        cdbg << "\tEnter local constant folding 9: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
        op = *opi;

	if (is_pseudo(op))
	    continue;
	if (! op->predicated())
	    continue;
	pred = op->src(PRED1);
	/* For brct/cf if either guard or branch cond is always false/true,
	   the branch will never be taken */
	if (is_brct(op)) {
	    brcond = op->src(SRC2);
	    if (! (pred.is_predicate_false() || brcond.is_predicate_false()))
	        continue;
	}
	else if (is_brcf(op)) {
	    brcond = op->src(SRC2);
	    if (! (pred.is_predicate_false() || brcond.is_predicate_true()))
	         continue;
	}
	else {
	    if (! pred.is_predicate_false())
	        continue;
	}

	if (dbg(opti,3))
	    cdbg << "\t\tApply constant folding 9 (op if F): op " << op->id() << endl;

	dead_list.add_tail(op);
	count++;
    }

    for (List_iterator<Op*> dli(dead_list); dli!=0; dli++) {
	op = *dli;
	/* Special treatment of Unconditional cmpps is required as the guard is
	   treated as a data input */
	if (is_cmpp(op)) {
	    /* DEST1 */
	    new_op = El_create_move_for_cmpp_with_false_guard(op, DEST1);
	    if (new_op != NULL)
		El_insert_op_before(new_op, op);
	    /* DEST2 */
	    new_op = El_create_move_for_cmpp_with_false_guard(op, DEST2);
	    if (new_op != NULL)
		El_insert_op_before(new_op, op);
	    /* Now, nuke the cmpp op */
	    C0_remove_op(op);
	    op->parent()->remove_region(op);
	    delete op;
	}
	else if (is_branch(op)) {
	    for (Op_outedges edgei(op, CONTROL0_OUTEDGES); edgei!=0; edgei++) {
		edge = *edgei;
		if (is_fall_through(edge))
		    continue;
		tgt_op = edge->dest();
		op->remove_outop_recursively(edge);
		op->remove_outedge_recursively(edge);
		// Unless have unreachable code elim, removing inops not a good idea
		// tgt_op->remove_inop_recursively(edge);
		tgt_op->remove_inedge_recursively(edge);
	    }
	    if (get_last_region_op_from_subregions(reg) == op) {
		op->set_opcode(DUMMY_BR);
		op->update_num_operands();
	    }
	    else {
                // first, delete subsequent C_MERGE or else later phases
                // get confused -KF 12/2004
                Edge* ft_edge = get_outgoing_CONTROL0_fall_through_edge(op);
                Op* ft_op = ft_edge->dest();
                assert(is_control_merge(ft_op));
                El_remove_op(ft_op);
                delete ft_op;

	        C0_remove_op(op);
	        op->parent()->remove_region(op);
		delete op;
	    }
	}
	else {
	    C0_remove_op(op);
	    op->parent()->remove_region(op);
	    delete op;
	}
    }

    return (count);
}

/*
 * Res<n> = saturating_op<m> (..)
 * =>
 * Res<n> = op(..) if (m < n)
 */

int El_opti_local_const_fold_10(Compound_region *reg)
{
    Operand dest;
    int width; // width of the result 
    int sp;    // saturation point
    Op *op;
    int count=0;

    if (dbg(opti, 3))
	cdbg << "\tEnter local constant folding 10: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! (is_int_satadd(op) || is_int_satsub(op) || is_int_satmul(op) ||
               is_int_satmuladd(op) || is_int_satmulsub(op) ) )
	    continue;
	dest = op->dest(DEST1);
	width = dest.dtype().width();
        sp    = extract_saturate_point(op->opcode());

        if (width < sp)  {
	  if (dbg(opti,3))
	     cdbg << "\t\tApply constant folding 10 (saturation - normal: op " << op->id() << endl;
          Opcode new_opc = convert_saturating_to_normal(op->opcode());
          op -> set_opcode(new_opc);

	  count++;
        }
    }

    return (count);
}

/*============================================================================*/
/*
 *	External routine 
 */
/*============================================================================*/

/*
 *	Returns the number of applications
 */

int El_opti_local_constant_folding(Compound_region *reg)
{
    int count=0;

    if (reg == NULL)
	El_punt("El_opti_local_constant_folding: region is NULL");

    if (reg->is_hb() || reg->is_bb()) {
        /* Do the various forms of const folding */

	/* 1. Convert 0 +,|,^ to anything to a move of x */
	count += El_opti_local_const_fold_1(reg);

	/* 2. Convert x +,-,|,^,<<,>> to anything to a move of x */
	count += El_opti_local_const_fold_2(reg);

	/* 3. Convert 0 *,/,&,<<,>> to a move of 0 */
	count += El_opti_local_const_fold_3(reg);

	/* 4. Convert x *,& 0 to a move of 0 */
	count += El_opti_local_const_fold_4(reg);

	/* 5. Convert 1 * x to a move of x */
	count += El_opti_local_const_fold_5(reg);

	/* 6. Convert x *,/ 1 to a move of x */
	count += El_opti_local_const_fold_6(reg);

	/* 7. Convert x -,/ x to a move of 0,1 */
	count += El_opti_local_const_fold_7(reg);

	/* 8. Op with all constant source operands */
	count += El_opti_local_const_fold_8(reg);

	/* 9. Op with constant guarding predicate */
	count += El_opti_local_const_fold_9(reg);

	/* 10. Op with opcode saturation point is less than the resultant width */
	count += El_opti_local_const_fold_10(reg);

    }
    else {
	/* We only do local opti so if region is larger than bb/hb, go
	   through the subregions */

	if (dbg(opti, 1))
	    cdbg << "-- Local constant folding" << endl;

	for (Region_subregions subri(reg); subri!=0; subri++) {
	    Compound_region *subr = (Compound_region *) *subri;
	    count += El_opti_local_constant_folding(subr);
	}
    }

    return (count);
}
