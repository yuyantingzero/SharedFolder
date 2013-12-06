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
//      File:           el_operand_position.cpp
//      Author:         Scott Mahlke
//      Created:        May 1995
//      Description:    Manipulate the operand positioning for each
//			compiler so the operands are in the appropriate
//			locations.
//
/////////////////////////////////////////////////////////////////////////////

#include "el_main.h"

#define ORIGINAL_PREDICATE_ATTR_NAME	"orig_pred"

/*=============================================================================*/
/*
 *	Operand remapping to go from Impact to Elcor
 */
/*=============================================================================*/

static void El_fix_load_for_elcor(L_Oper *oper)
{
    /* Pre-increment is not allowed for some reason */
    if (L_preincrement_load_opcode(oper)) {
        El_punt("El_fix_load_for_elcor: pre-increment loads not allowed!");
    }

    /* Post increment load
     *	Elcor format load_post (d1, d2) (s1, s2)
     *		d1 = mem(s1)
     *		d2 = s1+s2
     *	Lcode format load_post (d1, d2) (s1, s2, s3)
     *		d1 = mem(s1+s2)
     *		d2 = s1+s3
     */
    else if (L_postincrement_load_opcode(oper)) {
	if (! L_is_int_zero(oper->src[1]))
	    El_punt("El_fix_load_for_elcor: illegal address offset, must be 0");
	L_delete_operand(oper->src[1]);
	oper->src[1] = oper->src[2];
	oper->src[2] = NULL;
    }

    /*
     *	SAM 2-03, now accept base+displ loads from Impact
     *		Note, currently no post inc versions of these are allowed!
     */
    else if (M_base_displ_load_opcode(oper->proc_opc)) {
	//El_warn("El_fix_load_for_elcor: base+displ load found: %d", oper->id);
    }

    /* Load
     *	Elcor format load (d1) (s1)
     *		d1 = mem(s1)
     *	Lcode format load (d1) (s1,s2)
     *		d1 = mem(s1+s2)
     */
    else if (L_load_opcode(oper) || L_is_opcode(Lop_PRED_LD_BLK, oper) ||
                // SLARSEN: Handle vector loads too
                L_vector_load_opcode(oper) ||
		L_is_opcode(Lop_PRED_LD, oper)) {
	if (! L_is_int_zero(oper->src[1]))
	    El_punt("El_fix_load_for_elcor: illegal address offset, must be 0");
	L_delete_operand(oper->src[1]);
	oper->src[1] = NULL;
    }


    else {
	El_punt("El_fix_load_for_elcor: unknown load oper");
    }
}

static void El_fix_store_for_elcor(L_Oper *oper)
{
    /* Pre-increment is not allowed for some reason */
    if (L_preincrement_store_opcode(oper)) {
	El_punt("El_fix_store_for_elcor: pre-increment stores not allowed!");
    }

    /* Store
     *	Elcor format store (d1) (s1, s2, s3)
     *		mem(s1) = s2
     *		d1 = s1 + s3
     *	Lcode format load (d1) (s1,s2, s3, s4)
     *		mem(s1+s2) = s3
     *		d1 = s1 + s4
     */
    else if (L_postincrement_store_opcode(oper)) {
	if (! L_is_int_zero(oper->src[1]))
	    El_punt("El_fix_store_for_elcor: illegal address offset, must be 0");
	L_delete_operand(oper->src[1]);
	oper->src[1] = oper->src[2];
	oper->src[2] = oper->src[3];
	oper->src[3] = NULL;
    }

    /*
     *	SAM 2-03, now accept base+displ loads from Impact
     *		Note, currently no post inc versions of these are allowed!
     */
    else if (M_base_displ_store_opcode(oper->proc_opc)) {
	//El_warn("El_fix_store_for_elcor: base+displ store found: %d", oper->id);
    }

    /* Store
     *	Elcor format store (s1, s2)
     *		mem(s1) = s2
     *	Lcode format load (s1,s2, s3)
     *		mem(s1+s2) = s3
     */
    else if (L_store_opcode(oper) || L_is_opcode(Lop_PRED_ST_BLK, oper) ||
                // SLARSEN: Handle vector stores too
                L_vector_store_opcode(oper) ||
		L_is_opcode(Lop_PRED_ST, oper)) {
	if (! L_is_int_zero(oper->src[1]))
	    El_punt("El_fix_store_for_elcor: illegal address offset, must be 0");
	L_delete_operand(oper->src[1]);
	oper->src[1] = oper->src[2];
	oper->src[2] = NULL;
    }

    else {
	El_punt("El_fix_store_for_elcor: unknown store oper");
    }
}

static int L_brf_opcode(L_Oper *oper)
{
    if (oper==NULL)
	return (0);

    switch (oper->proc_opc) {
	case PLAYDOHop_BRF_B_B_B:
	case PLAYDOHop_BRF_B_B_F:
	case PLAYDOHop_BRF_B_F_B:
	case PLAYDOHop_BRF_B_F_F:
	case PLAYDOHop_BRF_F_B_B:
	case PLAYDOHop_BRF_F_B_F:
	case PLAYDOHop_BRF_F_F_B:
	case PLAYDOHop_BRF_F_F_F:
	    return (1);
	default:
	    return (0);
    }
}

static int L_brw_opcode(L_Oper *oper)
{
    if (oper==NULL)
        return (0);

    switch (oper->proc_opc) {
        case PLAYDOHop_BRW_B_B_B:
        case PLAYDOHop_BRW_B_B_F:
        case PLAYDOHop_BRW_B_F_B:
        case PLAYDOHop_BRW_B_F_F:
        case PLAYDOHop_BRW_F_B_B:
        case PLAYDOHop_BRW_F_B_F:
        case PLAYDOHop_BRW_F_F_B:
        case PLAYDOHop_BRW_F_F_F:
            return (1);
        default:
            return (0);
    }
}

static void El_fix_branch_for_elcor(L_Oper *oper)
{
    /* Brf
     *  Elcor format brf (pred, lc, esc) (btr, lc, esc)
     *
     *  Lcode format cbr (pred) (lc, esc, target_cb, btr)
     */
    if (L_cond_branch_opcode(oper) && L_brf_opcode(oper)) {
	if (oper->src[2]==NULL)
           L_punt("El_fix_branch_for_elcor: missing src(3) for brf");
        L_delete_operand(oper->src[2]);
	oper->src[2] = oper->src[1];
	oper->src[1] = oper->src[0];
        oper->src[0] = oper->src[3];
	oper->src[3] = NULL;
	oper->dest[1] = L_copy_operand(oper->src[1]);
	oper->dest[2] = L_copy_operand(oper->src[2]);
    }

    else if (L_cond_branch_opcode(oper) && L_brw_opcode(oper)) {
        L_punt("El_fix_branch_for_elcor: don't know how to xlate brw's yet");
    }

    /* Cbr
     *	Elcor format cbr () (btr, cond_pred)
     *
     *	Lcode format cbr () (cond_pred,1,target_cb,btr)
     */
    else if (L_cond_branch_opcode(oper)) {
	L_delete_operand(oper->src[1]);
	L_delete_operand(oper->src[2]);
	oper->src[1] = oper->src[0];
	oper->src[0] = oper->src[3];
	oper->src[2] = NULL;
	oper->src[3] = NULL;
    }

    /*
     * Jump
     *	Elcor format jump () (btr)
     *
     *	Lcode format jump () (target_cb,btr)
     */
    else if (L_uncond_branch_opcode(oper)) {
	L_delete_operand(oper->src[0]);
	oper->src[0] = oper->src[1];
	oper->src[1] = NULL;
    }

    /* Jsr
     *	Elcor format jsr (btr) (btr)
     *
     *	Lcode format jsr (btr) (target_fn,btr)
     */
    else if (L_subroutine_call_opcode(oper)) {
	L_delete_operand(oper->src[0]);
	oper->src[0] = oper->src[1];
	oper->src[1] = NULL;
    }

    /*
     * Rts
     *	Elcor format rts () (btr)
     *
     *	Lcode format rts () (btr)
     */
    else if (L_subroutine_return_opcode(oper)) {
        /* Currently the same so no modification necessary!! */
    }

    /* Jump_rg
     *	Elcor format jump_rg () (btr)
     *
     *	Lcode format jump_rg () (reg,btr)
     */
    else if (L_register_branch_opcode(oper)) {
	L_delete_operand(oper->src[0]);
	oper->src[0] = oper->src[1];
	oper->src[1] = NULL;
    }

    else {
	El_punt("El_fix_branch_for_elcor: unknown branch oper");
    }
}

static int El_literal_forming_opcode(L_Oper *oper)
{
    if (oper == NULL)
	return (0);

    switch (oper->proc_opc) {
	case PLAYDOHop_MOVELB:
	case PLAYDOHop_MOVELBX:
	case PLAYDOHop_MOVELBS:
	case PLAYDOHop_MOVELG:
	case PLAYDOHop_MOVELGX:
	case PLAYDOHop_MOVELGS:
	case PLAYDOHop_MOVELF:
	case PLAYDOHop_MOVELFS:
	    return (1);
	default:
	    return (0);
    }
}

static void El_fix_literal_forming_op_for_elcor(L_Oper *oper)
{
    if ((oper->proc_opc == PLAYDOHop_MOVELBS) ||
		(oper->proc_opc == PLAYDOHop_MOVELGS)) {
	/* remove src2 */
	if (oper->src[1] == NULL)
	    El_punt("El_fix_literal_forming_op_for_elcor: src2 is NULL");
	L_delete_operand(oper->src[1]);
	oper->src[1] = NULL;
    }
    else if ((oper->proc_opc == PLAYDOHop_PBRRLBS) ||
		(oper->proc_opc == PLAYDOHop_PBRALBS)) {
	/* remove src3 */
	if (oper->src[2] == NULL)
	    El_punt("El_fix_literal_forming_op_for_elcor: src3 is NULL");
	L_delete_operand(oper->src[2]);
	oper->src[2] = NULL;
    }
}

static void El_save_original_predicate(L_Oper *oper)
{
    L_Attr *attr;

    if (oper->pred[1]==NULL)
	return;

    /* There are a few rare cases where Impact does not set the flag
	correctly, so correct that here... */
#if 0
    if (! L_EXTRACT_BIT_VAL(oper->flags, L_OPER_PROMOTED))
	oper->flags = L_SET_BIT_FLAG(oper->flags, L_OPER_PROMOTED);
#endif
    attr = L_new_attr(ORIGINAL_PREDICATE_ATTR_NAME, 1);
    L_set_attr_field(attr, 0, oper->pred[1]);
    oper->attr = L_concat_attr(oper->attr, attr);
    oper->pred[1] = NULL;
}

void El_position_operands_for_elcor(L_Func *fn)
{
    L_Cb *cb;
    L_Oper *oper;

    for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
	for (oper=cb->first_op; oper!=NULL; oper=oper->next_op) {
	    if (L_general_load_opcode(oper))
		El_fix_load_for_elcor(oper);
	    else if (L_general_store_opcode(oper))
		El_fix_store_for_elcor(oper);
	    else if (L_general_branch_opcode(oper) ||
		     L_subroutine_call_opcode(oper) ||
		     L_subroutine_return_opcode(oper))
		El_fix_branch_for_elcor(oper);
	    else if (El_literal_forming_opcode(oper))
		El_fix_literal_forming_op_for_elcor(oper);

	    if (L_EXTRACT_BIT_VAL(oper->flags, L_OPER_PROMOTED) ||
		(oper->pred[1]!=NULL))
		El_save_original_predicate(oper);
	}
    }
}

/*=============================================================================*/
/*
 *	Operand remapping to go from Elcor to Impact
 */
/*=============================================================================*/

static void El_fix_load_for_lcode(L_Oper *oper)
{
    /* Pre-increment is not allowed for some reason */
    if (L_preincrement_load_opcode(oper)) {
        El_punt("El_fix_load_for_lcode: pre-increment loads not allowed!");
    }

    /* Post increment load
     *	Elcor format load_post (d1, d2) (s1, s2)
     *		d1 = mem(s1)
     *		d2 = s1+s2
     *	Lcode format load_post (d1, d2) (s1, s2, s3)
     *		d1 = mem(s1+s2)
     *		d2 = s1+s3
     */
    else if (L_postincrement_load_opcode(oper)) {
	oper->src[2] = oper->src[1];
	oper->src[1] = L_new_gen_int_operand(0);
    }

    /*
     *	SAM 2-03, now accept base+displ loads from Impact
     *		Note, currently no post inc versions of these are allowed!
     */
    else if (M_base_displ_load_opcode(oper->proc_opc)) {
	//El_warn("El_fix_load_for_elcor: base+displ load found: %d", oper->id);
    }

    /* Load
     *	Elcor format load (d1) (s1)
     *		d1 = mem(s1)
     *	Lcode format load (d1) (s1,s2)
     *		d1 = mem(s1+s2)
     */
    else if (L_load_opcode(oper) || L_is_opcode(Lop_PRED_LD_BLK, oper) ||
		L_is_opcode(Lop_PRED_LD, oper)) {
	oper->src[1] = L_new_gen_int_operand(0);
    }

    else {
	El_punt("El_fix_load_for_lcode: unknown load oper");
    }
}

static void El_fix_store_for_lcode(L_Oper *oper) 
{
    /* Pre-increment is not allowed for some reason */
    if (L_preincrement_store_opcode(oper)) {
	El_punt("El_fix_store_for_lcode: pre-increment stores not allowed!");
    }

    /* Store
     *	Elcor format store (d1) (s1, s2, s3)
     *		mem(s1) = s2
     *		d1 = s1 + s3
     *	Lcode format load (d1) (s1,s2, s3, s4)
     *		mem(s1+s2) = s3
     *		d1 = s1 + s4
     */
    else if (L_postincrement_store_opcode(oper)) {
	oper->src[3] = oper->src[2];
	oper->src[2] = oper->src[1];
	oper->src[1] = L_new_gen_int_operand(0);
    }

    /*
     *	SAM 2-03, now accept base+displ loads from Impact
     *		Note, currently no post inc versions of these are allowed!
     */
    else if (M_base_displ_store_opcode(oper->proc_opc)) {
	//El_warn("El_fix_store_for_elcor: base+displ store found: %d", oper->id);
    }

    /* Store
     *	Elcor format store (s1, s2)
     *		mem(s1) = s2
     *	Lcode format load (s1,s2, s3)
     *		mem(s1+s2) = s3
     */
    else if (L_store_opcode(oper) || L_is_opcode(Lop_PRED_ST_BLK, oper) ||
		L_is_opcode(Lop_PRED_ST, oper)) {
	oper->src[2] = oper->src[1];
	oper->src[1] = L_new_gen_int_operand(0);
    }

    else {
	El_punt("El_fix_store_for_lcode: unknown store oper");
    }
}

static L_Operand *El_find_target_from_attr(L_Oper *br)
{
    int pbr_id;
    L_Attr *attr;
    L_Oper *pbr_op;

    attr = L_find_attr(br->attr, "pbr_id");
    if (attr == NULL)
	return (NULL);

    pbr_id = attr->field[0]->value.i;
    pbr_op = L_oper_hash_tbl_find_oper(L_fn->oper_hash_tbl, pbr_id);
    return (pbr_op->src[0]);
}

static L_Operand *El_find_target(L_Oper *br, L_Operand *btr_reg)
{
    L_Oper *ptr;
    L_Operand *pbr_src;

    pbr_src = El_find_target_from_attr(br);
    if (pbr_src != NULL)
	return (pbr_src);

    for (ptr=br; ptr!=NULL; ptr=ptr->prev_op) {
	if (! L_is_dest_operand(btr_reg, ptr))
	    continue;
	if (L_is_opcode(Lop_PBR, ptr))
	    return (ptr->src[0]);
	else
	    L_punt("El_find_target: uknown def of btr_reg encountered!");
    }

    L_punt("El_find_target: PBR not found for br %d", br->id);
    return (NULL);
}

static void El_fix_branch_for_lcode(L_Cb *cb, L_Oper *oper)
{
    L_Operand *btr_reg;

    /* Brf
     *	Elcor format brf (pred, lc, esc) (btr, lc, esc)
     *
     *	Lcode format cbr (pred) (lc, esc, target_cb, btr)
     */
    if (L_cond_branch_opcode(oper) && L_brf_opcode(oper)) {
	btr_reg = oper->src[0];
	oper->src[0] = oper->src[1];
	oper->src[1] = oper->src[2];
	oper->src[2] = L_copy_operand(El_find_target(oper, btr_reg));
	oper->src[3] = btr_reg;
	if (oper->dest[1]==NULL)
	   L_punt("El_fix_branch_for_lcode: missing dest(2) for brf");
	L_delete_operand(oper->dest[1]);
	oper->dest[1] = NULL;
	if (oper->dest[2]==NULL)
	   L_punt("El_fix_branch_for_lcode: missing dest(3) for brf");
	L_delete_operand(oper->dest[2]);
	oper->dest[2] = NULL;
    }

    else if (L_cond_branch_opcode(oper) && L_brw_opcode(oper)) {
	L_punt("El_fix_branch_for_lcode: don't know how to xlate brw's yet");
    }

    /* Cbr
     *	Elcor format cbr () (btr, cond_pred)
     *
     *	Lcode format cbr () (cond_pred,1,target_cb,btr)
     */
    else if (L_cond_branch_opcode(oper)) {
	btr_reg = oper->src[0];
	oper->src[3] = oper->src[0];
	oper->src[0] = oper->src[1];
	oper->src[1] = L_new_gen_int_operand(1);
	oper->src[2] = L_copy_operand(El_find_target(oper, btr_reg));
    }

    /*
     * Jump
     *	Elcor format jump () (btr)
     *
     *	Lcode format jump () (target_cb,btr)
     */
    else if (L_uncond_branch_opcode(oper)) {
	btr_reg = oper->src[0];
	oper->src[1] = oper->src[0];
	oper->src[0] = L_copy_operand(El_find_target(oper, btr_reg));
    }

    /* Jsr
     *	Elcor format jsr (btr) (btr)
     *
     *	Lcode format jsr (btr) (target_fn,btr)
     */
    else if (L_subroutine_call_opcode(oper)) {
	btr_reg = oper->src[0];
	oper->src[1] = oper->src[0];
	oper->src[0] = L_copy_operand(El_find_target(oper, btr_reg));
    }

    /*
     * Rts
     *	Elcor format rts () (btr)
     *
     *	Lcode format rts () (btr)
     */
    else if (L_subroutine_return_opcode(oper)) {
        /* Currently the same so no modification necessary!! */
    }

    /* Jump_rg
     *	Elcor format jump_rg () (btr)
     *
     *	Lcode format jump_rg () (reg, btr)
     */
    else if (L_register_branch_opcode(oper)) {
	btr_reg = oper->src[0];
	oper->src[1] = oper->src[0];
	oper->src[0] = L_copy_operand(El_find_target(oper, btr_reg));
    }

    else {
	El_punt("El_fix_branch_for_lcode: unknown branch oper");
    }
}

static void El_fix_literal_forming_op_for_lcode(L_Oper *oper)
{
    L_Attr *attr;

    if ((oper->proc_opc == PLAYDOHop_MOVELBS) ||
		(oper->proc_opc == PLAYDOHop_MOVELGS)) {
	/* copy the dest to src2 */
	oper->src[1] = L_copy_operand(oper->dest[0]);
    }
    else if ((oper->proc_opc == PLAYDOHop_PBRRLBS) ||
		(oper->proc_opc == PLAYDOHop_PBRALBS)) {
	/* copy the dest to src3 */
	oper->src[2] = L_copy_operand(oper->dest[0]);
    }

    if (! L_find_attr(oper->attr, "do_not_split")) {
	attr = L_new_attr("do_not_split", 0);
	oper->attr = L_concat_attr(oper->attr, attr);
    }
}

static void El_restore_original_predicate(L_Oper *oper)
{
    L_Attr *attr;

    if (oper->pred[1]!=NULL)
	El_punt("El_restore_original_predicate: op %d already has an orig pred",
		oper->id);

    attr = L_find_attr(oper->attr, ORIGINAL_PREDICATE_ATTR_NAME);
    if (attr==NULL)
	return;

    oper->pred[1] = L_copy_operand(attr->field[0]);
    oper->attr = L_delete_attr(oper->attr, attr);
}

void El_position_operands_for_impact(L_Func *fn)
{
    L_Cb *cb;
    L_Oper *oper;

    for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
	for (oper=cb->first_op; oper!=NULL; oper=oper->next_op) {
	    if (L_general_load_opcode(oper))
		El_fix_load_for_lcode(oper);
	    else if (L_general_store_opcode(oper))
		El_fix_store_for_lcode(oper);
	    else if (L_general_branch_opcode(oper) ||
		     L_subroutine_call_opcode(oper) ||
		     L_subroutine_return_opcode(oper))
		El_fix_branch_for_lcode(cb, oper);
	    else if (El_literal_forming_opcode(oper))
		El_fix_literal_forming_op_for_lcode(oper);

	    El_restore_original_predicate(oper);
	}
    }
}
