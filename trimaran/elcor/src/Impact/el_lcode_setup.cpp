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
//      File:           el_lcode_setup.cpp
//      Author:         Scott Mahlke
//      Created:        June 1995
//      Description:    Organize Lcode function for input to elcor
//			or for input back to Impact.
//
/////////////////////////////////////////////////////////////////////////////

#include "el_main.h"

//
//	Currently the only ops that are gotten rid of are prologue and
//	epilogue.
//

static void El_remove_unnec_ops(L_Func *fn)
{
    L_Cb *cb;
    L_Oper *oper, *next;
    L_Attr *attr;

    for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
	for (oper=cb->first_op; oper!=NULL; oper=next) {
	    next = oper->next_op;
	    if (L_is_opcode(Lop_PROLOGUE, oper)) {
		attr = L_new_attr("prologue_id", 1);
		L_set_int_attr_field(attr, 0, oper->id);
		fn->attr = L_concat_attr(fn->attr, attr);
		L_delete_oper(cb, oper);
	    }
	    else if (L_is_opcode(Lop_EPILOGUE, oper)) {
		attr = L_new_attr("epilogue_id", 1);
		L_set_int_attr_field(attr, 0, oper->id);
		fn->attr = L_concat_attr(fn->attr, attr);
		L_delete_oper(cb, oper);
	    }
	}
    }
}


//
//	1. Position operands in appropriate locations for elcor
//		(see el_operand_position.cpp)
//	2. Remove prologue and epilogue ops

void El_setup_lcode_for_elcor(L_Func *fn)
{
    El_position_operands_for_elcor(fn);
    El_remove_unnec_ops(fn);
}


/*==========================================================================*/

//
//	Force all defines to be located at the beginning of the first cb.
//	Since defines are scheduled, it is possible for real operations to
//	be placed in between the beginning of the first cb and some defines.
//
static void El_move_defines_and_insert_prologue(L_Func *fn)
{
    L_Cb *cb;
    L_Oper *curr_op, *first_real_op = NULL, *new_op;
    L_Attr *attr;
    int id;

    cb = fn->first_cb;
    for (curr_op = cb->first_op; curr_op!=NULL; curr_op=curr_op->next_op) {
        if (!L_is_opcode(Lop_DEFINE, curr_op) && (first_real_op == NULL))
            first_real_op = curr_op;
        else if (L_is_opcode(Lop_DEFINE, curr_op) && (first_real_op != NULL)) {
            L_remove_oper(cb, curr_op);
            L_insert_oper_before(cb, first_real_op, curr_op);
        }
    }

    attr = L_find_attr(fn->attr, "prologue_id");
    if (attr!=NULL) {
	id = attr->field[0]->value.i;
	fn->attr = L_delete_attr(fn->attr, attr);
        new_op = L_create_new_op_with_id(Lop_PROLOGUE, id);
    }
    else {
        new_op = L_create_new_op(Lop_PROLOGUE);
    }

    L_insert_oper_before(cb, first_real_op, new_op);
}

#if 0
//
//	Insert the prologue after the last Lop_DEFINE in the first cb
//	of a function.
//

static void El_insert_prologue(L_Func *fn)
{
    L_Cb *cb;
    L_Oper *new_op;

    cb = fn->first_cb;
    new_op = L_create_new_op(Lop_PROLOGUE);
    L_insert_oper_before(cb, cb->first_op, new_op);
}
#endif


//
//	Insert epilogue right before the rts
//

static void El_insert_epilogue(L_Func *fn)
{
    L_Cb *cb;
    L_Oper *oper, *new_op;
    L_Attr *attr;
    int id;

    for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
	for (oper=cb->first_op; oper!=NULL; oper=oper->next_op) {
	    if (L_is_opcode(Lop_RTS, oper)) {
    		attr = L_find_attr(fn->attr, "epilogue_id");
		if (attr!=NULL) {
		    id = attr->field[0]->value.i;
		    fn->attr = L_delete_attr(fn->attr, attr);
		    new_op = L_create_new_op_with_id(Lop_EPILOGUE, id);
		}
		else {
		    new_op = L_create_new_op(Lop_EPILOGUE);
		}
		L_insert_oper_before(cb, oper, new_op);
		return;
	    }
	}
    }

    El_punt("El_insert_epilogue: function does not contain an RTS");
}


static void El_insert_mask_exception_flags(L_Func *fn)
{
    L_Cb *cb;
    L_Oper *oper;

    for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
	for (oper=cb->first_op; oper!=NULL; oper=oper->next_op) {
	    if (! L_EXTRACT_BIT_VAL(oper->flags, L_OPER_SPECULATIVE|L_OPER_PROMOTED))
		continue;
	    if (L_EXTRACT_BIT_VAL(oper->flags, L_OPER_MASK_PE))
		continue;
	    if (! L_safe_for_speculation(oper))
		oper->flags = L_SET_BIT_FLAG(oper->flags, L_OPER_MASK_PE);
	}
    }
}

//
//	1. Position operands in appropriate locations for impact
//		(see el_operand_position.cpp)
//	2. Insert prologue and epilogue ops
//	3. Insert <M> (mask exception) oper flags for all speculative
//		which may cause an exception so emulation will suppress
//		the exception.

void El_setup_lcode_for_impact(L_Func *fn)
{
    El_position_operands_for_impact(fn);
    El_move_defines_and_insert_prologue(fn);
    El_insert_epilogue(fn);
    El_insert_mask_exception_flags(fn);
}


