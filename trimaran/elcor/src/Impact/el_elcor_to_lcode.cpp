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
//      File:           el_elcor_to_lcode.cpp
//      Author:         Scott A. Mahlke
//      Created:        May 1995
//      Description:    Convert Elcor IR to Lcode IR
//
/////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "el_main.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "edge.h"
#include "intf.h"
#include "mdes_attributes.h"
#include "el_stats_init.h"
#include "el_clock.h"
#include "edge_utilities.h"

#define DEBUG

/*============================================================================*/
/*
 *	Flags
 */
/*============================================================================*/

unsigned int El_elcor_to_lcode_flags(Region *region)
{
    unsigned int flags = 0;

    /* Oper flags */
    if (region->flag(EL_OPER_CHECK))
	flags |= L_OPER_CHECK;
    if (region->flag(EL_OPER_LABEL_REFERENCE))
	flags |= L_OPER_LABEL_REFERENCE;
    if (region->flag(EL_OPER_PROMOTED))
	flags |= L_OPER_PROMOTED;
    if (region->flag(EL_OPER_SQUASHING))
	flags |= L_OPER_SQUASHING;
    if (region->flag(EL_OPER_DATA_SPECULATIVE))
	flags |= L_OPER_DATA_SPECULATIVE;
    if (region->flag(EL_OPER_SPILL_CODE))
	flags |= L_OPER_SPILL_CODE;
    if (region->flag(EL_OPER_SIDE_EFFECT_FREE))
	flags |= L_OPER_SIDE_EFFECT_FREE;
    if (region->flag(EL_OPER_SPECULATIVE))
	flags |= L_OPER_SPECULATIVE;
    if (region->flag(EL_OPER_MASK_PE))
	flags |= L_OPER_MASK_PE;
    if (region->flag(EL_OPER_SAFE_PEI))
	flags |= L_OPER_SAFE_PEI;
    if (region->flag(EL_OPER_PROBE_MARK))
	flags |= L_OPER_PROBE_MARK;
    if (region->flag(EL_OPER_SYNC))
	flags |= L_OPER_SYNC;
    if (region->flag(EL_OPER_PROCESSOR_SPECIFIC))
	flags |= L_OPER_PROCESSOR_SPECIFIC;
    if (region->flag(EL_OPER_VOLATILE))
	flags |= L_OPER_VOLATILE;
    if (region->flag(EL_OPER_NO_SPECULATION))
	flags |= L_OPER_NO_SPECULATION;
    if (region->flag(EL_OPER_STACK_REFERENCE))
	flags |= L_OPER_STACK_REFERENCE;
    if (region->flag(EL_OPER_ROTATE_REGISTERS))
	flags |= L_OPER_ROTATE_REGISTERS;
    if (region->flag(EL_OPER_SUPER_SPECULATION))
	flags |= L_OPER_SUPER_SPECULATION;

    /* Cb flags */
    if (region->flag(EL_REGION_HYPERBLOCK))
	flags |= L_CB_HYPERBLOCK;
    if (region->flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU))
	flags |= L_CB_HYPERBLOCK_NO_FALLTHRU;
    if (region->flag(EL_REGION_SOFTPIPE))
	flags |= L_CB_SOFTPIPE;
    if (region->flag(EL_REGION_SUPERBLOCK))
	flags |= L_CB_SUPERBLOCK;
    if (region->flag(EL_REGION_UNROLLED))
	flags |= L_CB_UNROLLED;
    if (region->flag(EL_REGION_ENTRANCE_BOUNDARY))
	flags |= L_CB_ENTRANCE_BOUNDARY;
    if (region->flag(EL_REGION_EXIT_BOUNDARY))
	flags |= L_CB_EXIT_BOUNDARY;
    if (region->flag(EL_REGION_HAS_PROLOGUE_OP))
	flags |= L_CB_PROLOGUE;
    if (region->flag(EL_REGION_HAS_EPILOGUE_OP))
	flags |= L_CB_EPILOGUE;
    if (region->flag(EL_REGION_VIOLATES_LC_SEMANTICS))
	flags |= L_CB_VIOLATES_LC_SEMANTICS;
    if (region->flag(EL_REGION_ROT_REG_ALLOCATED))
	flags |= L_CB_ROT_REG_ALLOCATED;

    /* Function flags */
    if (region->flag(EL_PROC_HYPERBLOCK))
	flags |= L_FUNC_HYPERBLOCK;
    if (region->flag(EL_PROC_LEAF))
	flags |= L_FUNC_LEAF;
    if (region->flag(EL_PROC_SIDE_EFFECT_FREE))
	flags |= L_FUNC_SIDE_EFFECT_FREE;
    if (region->flag(EL_PROC_REGISTER_ALLOCATED))
	flags |= L_FUNC_REGISTER_ALLOCATED;
    if (region->flag(EL_PROC_SUPERBLOCK))
	flags |= L_FUNC_SUPERBLOCK;
    if (region->flag(EL_PROC_MASK_PE))
	flags |= L_FUNC_MASK_PE;
    if (region->flag(EL_PROC_COMPILATION_COMPLETE))
	flags |= L_FUNC_COMPILATION_COMPLETE;
    if (region->flag(EL_PROC_SCHEDULED))
	flags |= L_FUNC_SCHEDULED;
    if (region->flag(EL_PROC_CC_IN_PREDICATE_REGS))
	flags |= L_FUNC_CC_IN_PREDICATE_REGS;
    if (region->flag(EL_PROC_PRED_REGS_IN_ATTR))
	flags |= L_FUNC_PRED_REGS_IN_ATTR;
    if (region->flag(EL_PROC_ROT_REG_ALLOCATED))
	flags |= L_FUNC_ROT_REG_ALLOCATED;

    return (flags);
}

// Not all Elcor flags have an lcode equivalent, for those, create an
// attribute and pass that.
L_Attr *El_elcor_flag_to_lcode_attr(Region *region)
{
    L_Attr *new_attr, *attr_list=NULL;

    if (region->flag(EL_REGION_HYPERBLOCK_FRP)) {
	new_attr = L_new_attr("hb_frp", 0);
	attr_list = L_concat_attr(attr_list, new_attr);
    }
    if (region->flag(EL_REGION_PRELOOP_BLOCK)) {
	new_attr = L_new_attr("preloop", 0);
	attr_list = L_concat_attr(attr_list, new_attr);
    }
    if (region->flag(EL_REGION_POSTLOOP_BLOCK)) {
	new_attr = L_new_attr("postloop", 0);
	attr_list = L_concat_attr(attr_list, new_attr);
    }
    if (region->flag(EL_OPER_NONSEQ_DUMMY_BR)) {
        new_attr = L_new_attr("nonseq_db", 0);
        attr_list = L_concat_attr(attr_list, new_attr);
    }
    if (region->flag(EL_REGION_HAS_EXPLICIT_FT)) {
        new_attr = L_new_attr("explicit_ft", 0);
        attr_list = L_concat_attr(attr_list, new_attr);
    }
    if (region->flag(EL_OPER_LIVEOUT_DEST)) {
        new_attr = L_new_attr("liveout", 0);
        attr_list = L_concat_attr(attr_list, new_attr);
    }

    return (attr_list);
}


/*============================================================================*/
/*
 *	Operand conversion
 */
/*============================================================================*/

unsigned char El_operand_ctype(Operand *cur_operand)
{
    DType data_type;
    DType_info info;
    unsigned int width, orig_width;
    bool s;

    /* Special case here since Impact aliases ctype with data type and
	register file assignment */
    if (cur_operand->is_reg() || cur_operand->is_macro_reg()) {
        if (cur_operand->file_type() == CR)
            return (L_CTYPE_CONTROL);
    }

    data_type = cur_operand->dtype();
    info = data_type.info();

    switch (info) {
	case EL_DT_INFO_INT:
	    orig_width = data_type.width();
	    if (orig_width == 0)
		return (L_CTYPE_VOID);
	    // width = data_type.upconv_width();
	    width = 32;
	    s = data_type.is_signed();
	    if (width == EL_DEFAULT_CHAR_WIDTH) {
		if (s)
		    return (L_CTYPE_CHAR);
		else
		    return (L_CTYPE_UCHAR);
	    }
	    else if (width == EL_DEFAULT_SHORT_WIDTH) {
		if (s)
		    return (L_CTYPE_SHORT);
		else
		    return (L_CTYPE_USHORT);
	    }
	    else if (width == EL_DEFAULT_INT_WIDTH) {
		if (s)
		    return (L_CTYPE_INT);
		else
		    return (L_CTYPE_UINT);
	    }
	    else if (width == EL_DEFAULT_LONG_WIDTH) {
		if (s)
		    return (L_CTYPE_LONG);
		else
		    return (L_CTYPE_ULONG);
	    }
	    else if (width == EL_DEFAULT_LONGLONG_WIDTH) {
		if (s)
		    return (L_CTYPE_LLONG);
		else
		    return (L_CTYPE_ULLONG);
	    }
	    else if (width == EL_DEFAULT_LONGLONGLONG_WIDTH) {
		if (s)
		    return (L_CTYPE_LLLONG);
		else
		    return (L_CTYPE_ULLLONG);
	    }
	    else {
		El_punt("El_operand_ctype: unsupported int width");
		return (0);
	    }
	case EL_DT_INFO_FLOAT:
	    width = data_type.upconv_width();
	    if (width == EL_DEFAULT_SINGLE_WIDTH)
		return (L_CTYPE_FLOAT);
	    else if (width == EL_DEFAULT_DOUBLE_WIDTH)
		return (L_CTYPE_DOUBLE);
	case EL_DT_INFO_BRANCH:
	    return (L_CTYPE_BTR);
	case EL_DT_INFO_LOCAL_ABS:
	    return (L_CTYPE_LOCAL_ABS);
	case EL_DT_INFO_LOCAL_GP:
	    return (L_CTYPE_LOCAL_GP);
	case EL_DT_INFO_GLOBAL_ABS:
	    return (L_CTYPE_GLOBAL_ABS);
	case EL_DT_INFO_GLOBAL_GP:
	    return (L_CTYPE_GLOBAL_GP);
	case EL_DT_INFO_POINTER:
	    return (L_CTYPE_POINTER);
	case EL_DT_INFO_PREDICATE:
	    return (L_CTYPE_PREDICATE);
	default:
	    El_punt("El_elcor_to_lcode_operand: unknown data type!");
	    return (0);
    }
}

void El_set_dest_ptypes(Op *cur_op, L_Oper *oper)
{
    int ptype = 0;
    Opcode cur_opcode, mod;

    cur_opcode = cur_op->opcode();

    if (oper->dest[0]!=NULL) {
	mod = get_cmpp_dest1_modifier(cur_opcode);
	switch (mod) {
	    case IR_DEST1_UN:
		ptype = L_PTYPE_UNCOND_T;
		break;
	    case IR_DEST1_UC:
		ptype = L_PTYPE_UNCOND_F;
		break;
	    case IR_DEST1_CN:
		ptype = L_PTYPE_COND_T;
		break;
	    case IR_DEST1_CC:
		ptype = L_PTYPE_COND_F;
		break;
	    case IR_DEST1_ON:
		ptype = L_PTYPE_OR_T;
		break;
	    case IR_DEST1_OC:
		ptype = L_PTYPE_OR_F;
		break;
	    case IR_DEST1_AN:
		ptype = L_PTYPE_AND_T;
		break;
	    case IR_DEST1_AC:
		ptype = L_PTYPE_AND_F;
		break;
	    default:
		El_punt("El_set_dest_ptypes: illegal dest1 modifier");
	}
	oper->dest[0]->ptype = ptype;
    }

    if (oper->dest[1]!=NULL) {
	mod = get_cmpp_dest2_modifier(cur_opcode);
	switch (mod) {
	    case IR_DEST2_UN:
		ptype = L_PTYPE_UNCOND_T;
		break;
	    case IR_DEST2_UC:
		ptype = L_PTYPE_UNCOND_F;
		break;
	    case IR_DEST2_CN:
		ptype = L_PTYPE_COND_T;
		break;
	    case IR_DEST2_CC:
		ptype = L_PTYPE_COND_F;
		break;
	    case IR_DEST2_ON:
		ptype = L_PTYPE_OR_T;
		break;
	    case IR_DEST2_OC:
		ptype = L_PTYPE_OR_F;
		break;
	    case IR_DEST2_AN:
		ptype = L_PTYPE_AND_T;
		break;
	    case IR_DEST2_AC:
		ptype = L_PTYPE_AND_F;
		break;
	    default:
		El_punt("El_set_dest_ptypes: illegal dest2 modifier");
	}
	oper->dest[1]->ptype = ptype;
    }
}

void El_fix_pred_dests(L_Oper *oper)
{
    int i;
    L_Operand *dest;

    for (i=0; i<L_max_dest_operand; i++) {
	dest = oper->dest[i];
	if (! L_is_macro(dest))
	    continue;
	if ((dest->value.mac == PLAYDOH_MAC_PRED_FALSE) ||
		(dest->value.mac == PLAYDOH_MAC_PRED_TRUE)) {
	    L_delete_operand(dest);
	    oper->dest[i] = NULL;
	}
    }
}

void El_fix_pred_attrs(L_Oper *oper)
{
    int i;
    L_Attr *attr;
    L_Operand *dest;

    attr = L_find_attr(oper->attr, "vpd");
    if (attr != NULL) {
	for (i=0; i<L_max_dest_operand; i++) {
	    dest = oper->dest[i];
	    if (dest == NULL)
		continue;
	    attr->field[i]->ptype = dest->ptype;
	}
    }
}

int El_elcor_to_lcode_macro_name(Macro_name elcor_macro)
{
  switch(elcor_macro)
    {
    case LOCAL:
      return L_MAC_LOCAL_SIZE;
    case PARAM:
      return L_MAC_PARAM_SIZE;
    case SWAP:
      return L_MAC_SWAP_SIZE;
    case INT_RETURN_TYPE:
      return L_MAC_RET_TYPE;
    case FLT_RETURN_TYPE:
      return L_MAC_RET_TYPE;
    case DBL_RETURN_TYPE:
      return L_MAC_RET_TYPE;
    case INT_RETURN:
      return L_MAC_P15;
    case INT_PARAM_1:
      return L_MAC_P0;
    case INT_PARAM_2:
      return L_MAC_P1;
    case INT_PARAM_3:
      return L_MAC_P2;
    case INT_PARAM_4:
      return L_MAC_P3;
    case FLT_RETURN:
      return L_MAC_P4;
    case FLT_PARAM_1:
      return L_MAC_P5;
    case FLT_PARAM_2:
      return L_MAC_P6;
    case FLT_PARAM_3:
      return L_MAC_P7;
    case FLT_PARAM_4:
      return L_MAC_P8;
    case DBL_RETURN:
      return L_MAC_P4;
    case DBL_PARAM_1:
      return L_MAC_P5;
    case DBL_PARAM_2:
      return L_MAC_P6;
    case DBL_PARAM_3:
      return L_MAC_P7;
    case DBL_PARAM_4:
      return L_MAC_P8;
    case INT_TM_TYPE:
      return L_MAC_TM_TYPE;
    case FLT_TM_TYPE:
      return L_MAC_TM_TYPE;
    case DBL_TM_TYPE:
      return L_MAC_TM_TYPE;
    case SP_REG:
      return L_MAC_SP;
    case FP_REG:
      return L_MAC_FP;
    case IP_REG:
      return L_MAC_IP;
    case OP_REG:
      return L_MAC_OP;
    case LV_REG:
      return L_MAC_LV;
    case RGS_REG:
      return L_MAC_RS;
    case LC_REG:
      return PLAYDOH_MAC_LC;
    case ESC_REG:
      return PLAYDOH_MAC_ESC;

    case ALL_PRED:
      return L_MAC_PRED_ALL;
    case ALL_ROT_PRED:
      return PLAYDOH_MAC_PRED_ALL_ROT;
    case ALL_STATIC_PRED:
      return PLAYDOH_MAC_PRED_ALL_STATIC;

    case RRB:
      return PLAYDOH_MAC_RRB;
    case RETURN_ADDR:
      return PLAYDOH_MAC_RETADDR;
    case FLT_ZERO:
      return PLAYDOH_MAC_FZERO;
    case FLT_ONE:
      return PLAYDOH_MAC_FONE;
    case DBL_ZERO:
      return PLAYDOH_MAC_FZERO;
    case DBL_ONE:
      return PLAYDOH_MAC_FONE;
    case INT_ZERO:
      return PLAYDOH_MAC_ZERO;
    case PRED_FALSE:
      return PLAYDOH_MAC_PRED_FALSE;
    case PRED_TRUE:
      return PLAYDOH_MAC_PRED_TRUE;
    case SPILL_TEMPREG:
      return PLAYDOH_MAC_TEMPREG;

    case PV_0:
      return PLAYDOH_MAC_PV_0;
    case PV_1:
      return PLAYDOH_MAC_PV_1;
    case PV_2:
      return PLAYDOH_MAC_PV_2;
    case PV_3:
      return PLAYDOH_MAC_PV_3;
    case PV_4:
      return PLAYDOH_MAC_PV_4;
    case PV_5:
      return PLAYDOH_MAC_PV_5;
    case PV_6:
      return PLAYDOH_MAC_PV_6;
    case PV_7:
      return PLAYDOH_MAC_PV_7;
    default:
      El_punt("El_elcor_to_lcode_macro_name: Incorrect macro type");
    }
  return 0;
}

L_Operand *El_elcor_to_lcode_operand(L_Func *fn, Operand *cur_operand)
{
    L_Operand *operand=NULL;
    unsigned char ctype;

    /* Undefined is essentially a NULL operand, so ignore */
    if (cur_operand->is_undefined()) {
	return NULL;
    }

    ctype = El_operand_ctype(cur_operand);

    /* Integer literal */
    if (cur_operand->is_int()) {
	operand = L_new_int_operand(cur_operand->int_value(), ctype);
    }
    /* Float literal */
    else if (cur_operand->is_float()) {
	operand = L_new_float_operand((double)cur_operand->float_value());
    }
    /* Double literal */
    else if (cur_operand->is_double()) {
	operand = L_new_double_operand(cur_operand->double_value());
    }
    /* String literal */
    else if (cur_operand->is_string()) {
	operand = L_new_string_operand(cur_operand->string_value(), ctype);
    }
    /* Label literal */
    else if (cur_operand->is_label()) {
	operand = L_new_label_operand(cur_operand->label_value(), ctype);
    }
    /* Cb literal */
    else if (cur_operand->is_cb()) {
        L_Cb *cb;
	cb = L_cb_hash_tbl_find_and_alloc(fn->cb_hash_tbl, cur_operand->id());
	operand = L_new_cb_operand(cb);
    }
    /* Predicate literal - In Lcode the absence of a predicate means true,
	thus do not set operand */
    else if (cur_operand->is_predicate()) {
	bool val = cur_operand->pred_value();
	if (val!=true)
	    El_punt("El_elcor_to_lcode_operand: unable to convert predicate lit");
    }
    /* Register */
    else if (cur_operand->is_reg()) {
	/* use the physical register number if it is allocated */
	if (cur_operand->allocated()) {
	    if (cur_operand->is_rotating()) {
	        operand = L_new_rregister_operand(cur_operand->mc_num(),
				 ctype, L_PTYPE_NULL);
	    }
	    else {
	        operand = L_new_register_operand(cur_operand->mc_num(),
				 ctype, L_PTYPE_NULL);
	    }
	}
	else {
	    if (cur_operand->is_rotating()) {
	        operand = L_new_evr_operand(cur_operand->vr_num(),
				 cur_operand->omega(), ctype , L_PTYPE_NULL);
	    }
	    else {
	        operand = L_new_register_operand(cur_operand->vr_num(),
				 ctype, L_PTYPE_NULL);
	    }
	}
    }
    /* Macro */
    else if (cur_operand->is_macro_reg()) {
	int mac_id;
	mac_id = El_elcor_to_lcode_macro_name(cur_operand->name());
	operand = L_new_macro_operand(mac_id, ctype, L_PTYPE_NULL);
    }
    else {
	El_punt("El_elcor_to_lcode_operand: unknown operand type");
    }

    return (operand);
}

/*============================================================================*/
/*
 *	Attr conversion
 */
/*============================================================================*/

L_Attr *El_elcor_to_lcode_attr(L_Func *fn, eString& name, List<Operand>& operand_list)
{
    L_Attr *attr;
    int index;
    Operand cur_operand;

    attr = L_new_attr((char *) name, operand_list.size());
    index = 0;
    for (List_iterator<Operand> li(operand_list); li!=0; li++) {
	cur_operand = *li;
	L_set_attr_field(attr, index++, El_elcor_to_lcode_operand(fn, &cur_operand));
    }

    return (attr);
}

/*============================================================================*/
/*
 *	Op conversion
 */
/*============================================================================*/

void El_set_compare_conds(Op *cur_op, L_Oper *oper)
{
    bool is_signed = false;
    IR_MODIFIERS opcode_mod = get_cond_modifier(cur_op->opcode());
    int comp = 0;

    if(is_fcmpp(cur_op) || is_fcmpr(cur_op)) {
      switch (opcode_mod) {
	case IR_F_EQ:
	    comp = Lcmp_COM_EQ;
	    break;
	case IR_F_NEQ:
	    comp = Lcmp_COM_NE;
	    break;
	case IR_F_GT:
	    comp = Lcmp_COM_GT;
	    break;
	case IR_F_GEQ:
	    comp = Lcmp_COM_GE;
	    break;
	case IR_F_LT:
	    comp = Lcmp_COM_LT;
	    break;
	case IR_F_LEQ:
	    comp = Lcmp_COM_LE;
	    break;
	default:
	    El_punt("El_set_compare_conds: unknown opcode modifier for op %d", cur_op->id());
      }
    }
    else {
      switch (opcode_mod) {
	case IR_EQ:
	    is_signed = true;
	    comp = Lcmp_COM_EQ;
	    break;
	case IR_NEQ:
	    is_signed = true;
	    comp = Lcmp_COM_NE;
	    break;
	case IR_GT:
	    is_signed = true;
	    comp = Lcmp_COM_GT;
	    break;
	case IR_GEQ:
	    is_signed = true;
	    comp = Lcmp_COM_GE;
	    break;
	case IR_LT:
	    is_signed = true;
	    comp = Lcmp_COM_LT;
	    break;
	case IR_LEQ:
	    is_signed = true;
	    comp = Lcmp_COM_LE;
	    break;
	case IR_LGT:
	    is_signed = false;
	    comp = Lcmp_COM_GT;
	    break;
	case IR_LGEQ:
	    is_signed = false;
	    comp = Lcmp_COM_GE;
	    break;
	case IR_LLT:
	    is_signed = false;
	    comp = Lcmp_COM_LT;
	    break;
	case IR_LLEQ:
	    is_signed = false;
	    comp = Lcmp_COM_LE;
	    break;
	default:
	    El_punt("El_set_compare_conds: unknown opcode modifier for op %d", cur_op->id());
      }
    }

    // Handle floating point
    if (is_fcmpp(cur_op) || is_fcmpr(cur_op)) {
	if (is_double(cur_op))
	    L_set_compare(oper, L_CTYPE_DOUBLE, comp);
	else
	    L_set_compare(oper, L_CTYPE_FLOAT, comp);
    }

    // Handle integer
    else {
	if (is_signed)
	    L_set_compare(oper, L_CTYPE_INT, comp);
	else
	    L_set_compare(oper, L_CTYPE_UINT, comp);
    }
}

void El_install_sched_opcode_attr(Op *cur_op, L_Oper *oper)
{
    L_Attr *attr;

    attr = L_find_attr(oper->attr, EL_SCHED_OPCODE_ATTR);
    if (attr != NULL) {
        L_set_label_attr_field(attr, 0, cur_op->sched_opcode());
    }
    else {
        attr = L_new_attr(EL_SCHED_OPCODE_ATTR, 1);
        L_set_label_attr_field(attr, 0, cur_op->sched_opcode());
        oper->attr = L_concat_attr(oper->attr, attr);
    }
}

L_Oper *El_elcor_to_lcode_nonseq_dummy_br(L_Func *fn, Op *cur_op)
{
    L_Oper *oper;
    Opcode cur_opcode;
    int proc_opc;
    Lcode_attribute_map* map;
    L_Attr *attr, *flag_attr;
    Compound_region *parent;
    Edge *edge, *matching_edge;
    Op *branch;
    int val;

    cur_opcode = BRU;
    proc_opc = el_elcor_to_lcode_opcode_map.value(cur_opcode);
    oper = L_oper_hash_tbl_find_and_alloc_oper(fn->oper_hash_tbl, cur_op->id());
    oper->proc_opc = proc_opc;

    if (proc_opc < PLAYDOHop_FIRST_OPC)
        oper->opc = proc_opc;
    else
        oper->opc = M_opc_from_proc_opc(proc_opc);

    oper->opcode = L_opcode_name(oper->opc);

    /*
     *  Flags
     */
    oper->flags = L_SET_BIT_FLAG(oper->flags, El_elcor_to_lcode_flags(cur_op));
    flag_attr = El_elcor_flag_to_lcode_attr(cur_op);
    if (flag_attr != NULL)
        oper->attr = L_concat_attr(oper->attr, flag_attr);

    /*
     *  Attributes brought over from Lcode
     */
    map = get_lcode_attributes(cur_op);
    for (Map_iterator<eString,List<Operand> > mi(*map); mi!=0; mi++) {
        oper->attr = L_concat_attr(oper->attr,
                          El_elcor_to_lcode_attr(fn, (*mi).first, (*mi).second));
    }

    edge = get_outgoing_CONTROL0_edge(cur_op);
    if (edge == NULL)
        El_punt("El_elcor_to_lcode_nonseq_dummy_br: dummy br %d missing edge",
                        cur_op->id());
    parent = cur_op->parent();
    matching_edge = get_exit_edge_to_same_target(parent, edge);
    if (matching_edge == NULL)
        El_punt("El_elcor_to_lcode_nonseq_dummy_br: matching edge not found");
    branch = matching_edge->src();
    oper->src[0] = El_elcor_to_lcode_operand(fn, &(branch->src(SRC1)));

    map = get_lcode_attributes(branch);
    if (! map->is_bound("pbr_id"))
        El_punt("El_elcor_to_lcode_nonseq_dummy_br: op %d has no pbr_id attr",
                        branch->id());
    val = map->value("pbr_id").head().int_value();

    attr = L_new_attr("pbr_id", 1);
    L_set_int_attr_field(attr, 0, val);
    oper->attr = L_concat_attr(oper->attr, attr);

    return (oper);
}

L_Oper *El_elcor_to_lcode_op (L_Func *fn, Op *cur_op, int last_sched_time,
                              bool scheduled, bool insert_nop_flag)
{
    L_Oper *oper;
    Opcode cur_opcode;
    Operand *cur_operand;
    Port_num dest_p, src_p;
    int proc_opc, index;
    Lcode_attribute_map* map;
    Jump_table_name *tbl_attr;
    L_Attr *attr, *flag_attr;

    /*
     *	Opcode
     */
    cur_opcode = cur_op->opcode();

    if ((cur_opcode == DUMMY_BR) && (cur_op->flag(EL_OPER_NONSEQ_DUMMY_BR))) {
        return (El_elcor_to_lcode_nonseq_dummy_br(fn, cur_op));
    }

    else if ((cur_opcode == DUMMY_BR) && scheduled && 
        (cur_op->sched_time() != last_sched_time) && insert_nop_flag) {
        L_Oper *new_oper;
        new_oper = L_create_new_op (Lop_NO_OP);
        attr = L_new_attr ("DUMMY_BR", 0);
        new_oper->attr = L_concat_attr (attr, new_oper->attr);
        return (new_oper);
    }

    else if (cur_opcode == DUMMY_BR || cur_opcode == C_MERGE ||
             cur_opcode == D_SWITCH || cur_opcode == D_MERGE || cur_opcode == REMAP) {
#if 0
	cerr << "Internal Elcor opcode, not converted!  ";
	cerr << el_opcode_to_string_map->value(cur_opcode) << endl;
#endif
	return NULL;
    }

    /* only base CMPP opcodes represented in Lcode */
    if (is_cmpp(cur_opcode) || is_cmpr(cur_opcode))
	cur_opcode = (Opcode) get_root(cur_opcode);
    proc_opc = el_elcor_to_lcode_opcode_map.value(cur_opcode);

    /* Special case for FSAVE, FRESTORE, they have different conversions for single
       vs double precision, ie PLAYDOHop_FSAVE_S or PLAYDOHop_FSAVE_D */
    if (is_single_fpr_save_restore(cur_op)) {
	if (is_store(cur_opcode))
	    proc_opc = PLAYDOHop_FSAVE_S;
	else
	    proc_opc = PLAYDOHop_FRESTORE_S;
    }

    oper = L_oper_hash_tbl_find_and_alloc_oper(fn->oper_hash_tbl, cur_op->id());
    oper->proc_opc = proc_opc;

    if (proc_opc < PLAYDOHop_FIRST_OPC)
	oper->opc = proc_opc;
    else if ((proc_opc==PLAYDOHop_BRU) && (cur_op->flag(EL_OPER_TABLE_JUMP)))
	oper->opc = Lop_JUMP_RG;
    else
        oper->opc = M_opc_from_proc_opc(proc_opc);

    oper->opcode = L_opcode_name(oper->opc);

    /* Special fixup for New style impact branches */
    if (is_branch(cur_opcode) && !is_bru(cur_opcode) && !is_rts(cur_opcode))
	L_set_compare(oper, L_CTYPE_INT, Lcmp_COM_EQ);

    /*
     *	Flags
     */
    oper->flags = L_SET_BIT_FLAG(oper->flags, El_elcor_to_lcode_flags(cur_op));
    flag_attr = El_elcor_flag_to_lcode_attr(cur_op);
    if (flag_attr != NULL)
	oper->attr = L_concat_attr(oper->attr, flag_attr);

    /*
     *	Destination operands
     */
    index = 0;
    for (dest_p = cur_op->first_dest() ; dest_p <= cur_op->last_dest() ;
                  dest_p = (Port_num)(dest_p+1)) {
	cur_operand = &(cur_op->dest(dest_p));
	oper->dest[index++] = El_elcor_to_lcode_operand(fn, cur_operand);
    }

    /*
     *	Source operands
     */
    index = 0;
    for (src_p = cur_op->first_src() ; src_p <= cur_op->last_src() ;
                  src_p = (Port_num)(src_p+1)) {
	cur_operand = &(cur_op->src(src_p));
	oper->src[index++] = El_elcor_to_lcode_operand(fn, cur_operand);
    }

    /*
     *	Predicate operand
     */
    if (cur_op->predicated()) {
	cur_operand = &(cur_op->src(cur_op->pred()));
	oper->pred[0] = El_elcor_to_lcode_operand(fn, cur_operand);
    }

    /* Set compare conditions */
    if (is_cmpp(cur_opcode) || is_cmpr(cur_opcode))
	El_set_compare_conds(cur_op, oper);

    /* Ptypes of dest operands for cmpp's are not set at this point */
    if (is_cmpp(cur_opcode))
        El_set_dest_ptypes(cur_op, oper);


    /*
     *	Attributes brought over from Lcode
     */
    map = get_lcode_attributes(cur_op);
    for (Map_iterator<eString,List<Operand> > mi(*map); mi!=0; mi++) {
	oper->attr = L_concat_attr(oper->attr,
			  El_elcor_to_lcode_attr(fn, (*mi).first, (*mi).second));
    }

    /* If there are predicates in the attributes, need to fix ptypes */
    if (is_cmpp(cur_opcode)) {
	El_fix_pred_dests(oper);
	El_fix_pred_attrs(oper);
    }

    /*
     *	Jump table attributes, create Lcode attrs from Elcor ones
     */
    tbl_attr = get_jump_table_name(cur_op);
    if (tbl_attr != NULL)
	L_install_jumptbl_op_attr(oper, (char *) tbl_attr->tbl_name);

    /*
     *	Install the sched_opcode as an attribute on the Lcode oper
     */
    if (cur_op->sched_opcode() != NULL)
	El_install_sched_opcode_attr(cur_op, oper);

    return (oper);
}

/*============================================================================*/
/*
 *	Cb conversion
 */
/*============================================================================*/

void El_insert_sched_time_attr(Op *cur_op, L_Oper *oper, int &last_sched_time,
				int &last_slot, L_Cb *cb, bool insert_nop_flag)
{
    int latency_time[5], sched_time, slot;
    L_Attr *attr;

    if (cur_op->opcode() == DEFINE)  // do not desire isl attribute on defines.
	return;
    // No isls on pred_clear/sets that are issued in the first cycle,
    // these really happen at time=-1 the way we treat them now.
    if (((cur_op->opcode() == PRED_CLEAR) ||
	 (cur_op->opcode() == PRED_SET)) &&
	(cur_op->sched_time() <= 0))
	    return; // do not desire isl attribute on defines.

    sched_time =  cur_op->sched_time();

    if (sched_time!=last_sched_time) {
	if (insert_nop_flag == true) {
            for (int time = last_sched_time + 1; (time < sched_time) &&
                                   (sched_time > last_sched_time); time++) {
            // add nops for cycles not containing any operations.
                L_Oper *new_oper;
                new_oper = L_create_new_op (Lop_NO_OP);
                L_insert_oper_before(cb, oper, new_oper);
                attr = L_new_attr("isl", 2);
                L_set_int_attr_field(attr, 0, time);
                L_set_int_attr_field(attr, 1, 0);
                new_oper->attr = L_concat_attr(new_oper->attr, attr);
            }
	}

	slot = 0;
        last_slot = 0;
	last_sched_time = sched_time;
    }
    else {
	slot = last_slot+1;
	last_slot++;
    }

    int dst = 1;
    if (cur_op->opcode() != DUMMY_BR)
    // dummy branch has an operand, but its "corresponding" nop doesn't.
      for (dst = 1; dst <= 5; dst++) {
          latency_time[dst-1] = cur_op->relative_dest_latency(dst);
          if (latency_time[dst-1] == -1)
            break;
      }

    attr = L_find_attr(oper->attr, "isl");
    if (attr == NULL) {
        attr = L_new_attr("isl", dst+1);
        oper->attr = L_concat_attr(oper->attr, attr);
    }
    L_set_int_attr_field(attr, 0, sched_time);
    L_set_int_attr_field(attr, 1, slot);
    for (int j = 0; j < dst-1; j++)
        L_set_int_attr_field(attr, j+2, latency_time[j]);
}

/*
 *	Place jrg flows in a well defined order.  This is mainly a debugging
 *	aid and does not affect correctness.
 */
void El_normalize_dest_flows_for_jrg(L_Cb *cb)
{
    int i, j, num_jrg_flow, found;
    L_Flow *flow, *ptr, *next_flow, **buf, *tmp, *prev_flow, *fallthru_flow;
    L_Oper *oper, *jrg = NULL;

    /* find the jrg */
    for (oper=cb->last_op; oper!=NULL; oper=oper->prev_op) {
	if (L_register_branch_opcode(oper)) {
	    jrg = oper;
	    break;
	}
    }
    if (jrg == NULL)
	L_punt("El_normalize_dest_flows_for_jrg: no jrg found in cb %d", cb->id);

    /*
     *  Break down the dest flows into 2 groups [start..prev_flow] = non_jrg flows
     *  and [flow..end] = jrg flows
     */
    flow = L_find_flow_for_branch(cb, jrg);
    prev_flow = NULL;
    found = 0;
    num_jrg_flow = 0;
    for (ptr=cb->dest_flow; ptr!=NULL; ptr=ptr->next_flow) {
        if (ptr==flow)
            found = 1;
        if (! found)
            prev_flow = ptr;
        else
            num_jrg_flow++;
    }

    /* Disconnect jrg flows and insert them into the buf array */
    if (prev_flow != NULL)
        prev_flow->next_flow = NULL;
    else
        cb->dest_flow = NULL;

    buf = (L_Flow **) Lcode_malloc(sizeof(L_Flow *)*num_jrg_flow);
    i = 0;
    for (ptr=flow; ptr!=NULL; ptr=next_flow) {
        next_flow = ptr->next_flow;
        buf[i++] = ptr;
        ptr->prev_flow = NULL;
        ptr->next_flow = NULL;
    }

    /* if a fallthru flow exists, pull it out of buf */
    fallthru_flow = NULL;
    if (buf[num_jrg_flow-1]->cc == L_JUMPTBL_DEFAULT_CC) {
        fallthru_flow = buf[num_jrg_flow-1];
        num_jrg_flow--;
    }

    /* Sort flows using 2 keys: 1. highest to lowest weight, 2. lowest to
       highest cc */
    for (i=0; i<num_jrg_flow; i++) {
        for (j=i; j<num_jrg_flow; j++) {
            if (buf[j]->weight > buf[i]->weight) {
                tmp = buf[i];
                buf[i] = buf[j];
                buf[j] = tmp;
            }
            else if ((buf[j]->weight == buf[i]->weight) &&
                     (buf[j]->cc < buf[i]->cc)) {
                tmp = buf[i];
                buf[i] = buf[j];
                buf[j] = tmp;
            }
        }
    }

    for (i=0; i<num_jrg_flow; i++) {
        cb->dest_flow = L_concat_flow(cb->dest_flow, buf[i]);
    }

    if (fallthru_flow != NULL)
        cb->dest_flow = L_concat_flow(cb->dest_flow, fallthru_flow);

    Lcode_free(buf);
}


/*
 *	reg is guaranteed to be a BB or HB when this is called
 */
L_Cb *El_elcor_to_lcode_cb (L_Func *fn, Compound_region *reg, bool insert_nop_flag)
{
    L_Cb *cb, *dest_cb;
    L_Oper *oper;
    L_Flow *flow, *fallthru_flow;
    Op *cur_op;
    Lcode_attribute_map* map;
    bool scheduled;
    int last_sched_time = -1, last_slot = 0, has_jrg = 0;
    Region_all_ops op_iter;
    Region_exit_edges ei;
    L_Attr *flag_attr;

    cb = L_cb_hash_tbl_find_and_alloc(fn->cb_hash_tbl, reg->id());
    cb->weight = reg->weight;

    /*
     *  Flags
     */
    cb->flags = L_SET_BIT_FLAG(cb->flags, El_elcor_to_lcode_flags(reg));
    flag_attr = El_elcor_flag_to_lcode_attr(reg);
    if (flag_attr != NULL)
	cb->attr = L_concat_attr(cb->attr, flag_attr);

    /*
     *	Flow arcs
     */
    for (op_iter(reg); op_iter!=0; op_iter++) {
	cur_op = (Op *) *op_iter;
	if (! is_branch(cur_op))
	    continue;
	if (is_table_jump(cur_op))
	   has_jrg = 1;
	fallthru_flow = NULL;
	for (ei(cur_op); ei!=0; ei++) {
	    Edge *edge = *ei;
	    if (! reg->is_exit_edge(edge))
		continue;
	    Region *dest_region = edge->dest();
            Region *parent = dest_region->parent();
            Control_flow_freq *cfreq = get_control_flow_freq(edge);
            dest_cb = L_cb_hash_tbl_find_and_alloc(fn->cb_hash_tbl, parent->id());
	    if ((edge->src_alt()==0) || (cfreq->cc == EL_JUMPTBL_DEFAULT_CC)) {
		if (cfreq->cc!=EL_UNKNOWN_CC_VAL)
                    fallthru_flow = L_new_flow(cfreq->cc, cb, dest_cb, cfreq->freq);
		else
                    fallthru_flow = L_new_flow(0, cb, dest_cb, cfreq->freq);
	    }
	    else if (cfreq->cc!=EL_UNKNOWN_CC_VAL) {
                flow = L_new_flow(cfreq->cc, cb, dest_cb, cfreq->freq);
                cb->dest_flow = L_concat_flow(cb->dest_flow, flow);
	    }
	    else {	/* use a cc of 1 if none exists for taken br */
                flow = L_new_flow(1, cb, dest_cb, cfreq->freq);
                cb->dest_flow = L_concat_flow(cb->dest_flow, flow);
	    }
	}
	if (fallthru_flow!=NULL)
	    cb->dest_flow = L_concat_flow(cb->dest_flow, fallthru_flow);
    }

    /*
     *	Attributes
     */
    map = get_lcode_attributes(reg);
    for (Map_iterator<eString,List<Operand> > mi(*map); mi!=0; mi++) {
        cb->attr = L_concat_attr(cb->attr,
                          El_elcor_to_lcode_attr(fn, (*mi).first, (*mi).second));
    }

    /*
     *	Opers
     */
    scheduled = reg->flag(EL_REGION_SCHEDULED);
    for (op_iter(reg); op_iter!=0; op_iter++) {
	cur_op = (Op *) *op_iter;
	oper = El_elcor_to_lcode_op(fn, cur_op, last_sched_time, scheduled,
				    insert_nop_flag);
	if (oper!=NULL) {
	    L_insert_oper_after(cb, cb->last_op, oper);
	    if (scheduled) {
	        El_insert_sched_time_attr(cur_op, oper, last_sched_time, 
                                         last_slot, cb, insert_nop_flag);
	    }
	}
    }
    if (has_jrg)
	El_normalize_dest_flows_for_jrg(cb);

    return (cb);
}

/*============================================================================*/
/*
 *	Function conversion
 */
/*============================================================================*/

L_Func *El_elcor_to_lcode_func (Procedure *f, bool insert_nop_flag)
{
    L_Func *fn;
    L_Cb *cb;
    Compound_region *reg;
    Lcode_attribute_map* map;
    El_jumptbl_info *tbl_info;
    L_Attr *flag_attr;

    if (El_do_run_time)
        El_clock2.start();
   
    fn = L_new_func((char *) f->get_name(), f->weight);
    L_fn = fn;

    /* Adjust the register names for f if any register allocation is done */
    if (f->flag(EL_PROC_REGISTER_ALLOCATED) || f->flag(EL_PROC_ROT_REG_ALLOCATED))
      El_punt("Sorry, translation of elcor to lcode with allocated registers is unimplemented.");
      // This doesn't work and hasn't worked for 6 years.  -KF 1/2007
      //El_rename_regs_for_lcode(f);

    fn->max_oper_id = f->get_max_op_id() ; // ********* This is an interim solution that
                     // should be looked at in the future.  Its purpose is to
                     // allow nops to be added in empty cycles during the
                     // processing of the regions.  This is an issue because
                     // elcor_to_lcode uses the elcor ids as the lcode ids.
                     // A nop added during this process is given an id that
                     // would overlap if this statement was not present.
                     // op_id_num contains the max elcor id that has been used.

    /*
     *  Flags
     */
    fn->flags = L_SET_BIT_FLAG(fn->flags, El_elcor_to_lcode_flags(f));
    flag_attr = El_elcor_flag_to_lcode_attr(f);
    if (flag_attr != NULL)
	fn->attr = L_concat_attr(fn->attr, flag_attr);

    /*
     *	Attributes
     */
    map = get_lcode_attributes(f);
    for (Map_iterator<eString,List<Operand> > mi(*map); mi!=0; mi++) {
        fn->attr = L_concat_attr(fn->attr,
                          El_elcor_to_lcode_attr(fn, (*mi).first, (*mi).second));
    }

    /*
     *	Special handling of the jump table attr
     */
    tbl_info = f->get_jumptbl_info();
    if (tbl_info != NULL)
	L_install_jumptbl_func_attr(fn, tbl_info->get_num_tables(),
					tbl_info->get_max_id());

    /* Walk the Elcor regions in linear order */
    for (Region_subregions reg_iter(f); reg_iter!=0; reg_iter++) {
	reg = (Compound_region *) *reg_iter;
	if (reg->is_hb() || reg->is_bb()) {

	  // Encapsulate the region in the correct MDES
          // SAM 6-96 handle case where don't have MDES attr, in this case just
          //          use the mdes that was read in
          MDES *reg_MDES = get_MDES(reg);
          if (reg_MDES!=NULL)
	    push_MDES(reg_MDES);

	  cb = El_elcor_to_lcode_cb(fn, reg, insert_nop_flag);

          if (reg_MDES!=NULL)
	    pop_MDES();

	  L_insert_cb_after(fn, fn->last_cb, cb);
	}
	else {
	  L_punt("El_elcor_to_lcode_func: illegal region type");
	}
    }

    if (f->get_jumptbl_info()) {
	fn->jump_tbls = El_elcor_to_lcode_jumptbl_info(f->get_jumptbl_info());
    }

    if (El_do_run_time) {
	El_clock2.stop();
	El_stopwatch_print_time("El_elcor_to_lcode_func",
				El_clock2.last_elapsed());
    }

    return (fn);
}

/*============================================================================*/
/*
 *	Data conversion
 */
/*============================================================================*/

L_Expr *El_elcor_to_lcode_expr(El_expression *e)
{
    L_Expr *expr=NULL;

    if (e == NULL)
        return (NULL);

    if (e->is_int_expr()) {
	expr = L_new_expr(L_EXPR_INT);
	expr->value.i = ((El_value_expression*)e)->value.i;
    }
    else if (e->is_predicate_expr()) {
	El_punt("El_elcor_to_lcode_expr: unsupported expression type");
    }
    else if (e->is_float_expr()) {
	expr = L_new_expr(L_EXPR_FLOAT);
	expr->value.f = ((El_value_expression*)e)->value.f;
    }
    else if (e->is_double_expr()) {
	expr = L_new_expr(L_EXPR_DOUBLE);
	expr->value.f2 = ((El_value_expression*)e)->value.f2;
    }
    else if (e->is_symbol_expr()) {
	expr = L_new_expr(L_EXPR_LABEL);
	expr->value.l = L_add_string(L_string_table, ((El_symbol_expression*)e)->value);
    }
    else if (e->is_string_expr()) {
	expr = L_new_expr(L_EXPR_STRING);
	expr->value.s = L_add_string(L_string_table, ((El_string_expression*)e)->value);
    }
    else if (e->is_op_expr()) {
	El_punt("El_elcor_to_lcode_expr: should not encouter op_expr at this level");
    }
    else if (e->is_compound_expr()) {
	El_expression *op_e = ((El_compound_expression*)e)->args[0];
	switch (((El_op_expression*)op_e)->opcode) {
	    case ELEX_ADD:
		expr = L_new_expr(L_EXPR_ADD);
		expr->A = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[1]);
		expr->B = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[2]);
		break;
	    case ELEX_SUB:
		expr = L_new_expr(L_EXPR_SUB);
		expr->A = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[1]);
		expr->B = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[2]);
		break;
	    case ELEX_MUL:
		expr = L_new_expr(L_EXPR_MUL);
		expr->A = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[1]);
		expr->B = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[2]);
		break;
	    case ELEX_DIV:
		expr = L_new_expr(L_EXPR_DIV);
		expr->A = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[1]);
		expr->B = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[2]);
		break;
	    case ELEX_NEG:
		expr = L_new_expr(L_EXPR_NEG);
		expr->A = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[1]);
		break;
	    case ELEX_COM:
		expr = L_new_expr(L_EXPR_COM);
		expr->A = El_elcor_to_lcode_expr(((El_compound_expression*)e)->args[1]);
		break;
	    default:
		El_punt("El_elcor_to_lcode_expr: illegal opcode for compound_expr");
	}
    }
    else {
	El_punt("El_elcor_to_lcode_expr: unknown expression type");
    }

    return (expr);
}

int El_lcode_data_token_type(El_data_token_type el_t_type)
{
    int type=0;

    switch (el_t_type) {
	case EL_DATA_TOKEN_MS:
	    type = L_INPUT_MS;
	    break;
	case EL_DATA_TOKEN_VOID:
	    type = L_INPUT_VOID;
	    break;
	case EL_DATA_TOKEN_BYTE:
	    type = L_INPUT_BYTE;
	    break;
	case EL_DATA_TOKEN_WORD:
	    type = L_INPUT_WORD;
	    break;
	case EL_DATA_TOKEN_LONG:
	    type = L_INPUT_LONG;
	    break;
	case EL_DATA_TOKEN_FLOAT:
	    type = L_INPUT_FLOAT;
	    break;
	case EL_DATA_TOKEN_DOUBLE:
	    type = L_INPUT_DOUBLE;
	    break;
	case EL_DATA_TOKEN_ALIGN:
	    type = L_INPUT_ALIGN;
	    break;
	case EL_DATA_TOKEN_ASCII:
	    type = L_INPUT_ASCII;
	    break;
	case EL_DATA_TOKEN_ASCIZ:
	    type = L_INPUT_ASCIZ;
	    break;
	case EL_DATA_TOKEN_RESERVE:
	    type = L_INPUT_RESERVE;
	    break;
	case EL_DATA_TOKEN_GLOBAL:
	    type = L_INPUT_GLOBAL;
	    break;
	case EL_DATA_TOKEN_WB:
	    type = L_INPUT_WB;
	    break;
	case EL_DATA_TOKEN_WW:
	    type = L_INPUT_WW;
	    break;
	case EL_DATA_TOKEN_WI:
	    type = L_INPUT_WI;
	    break;
	case EL_DATA_TOKEN_WF:
	    type = L_INPUT_WF;
	    break;
	case EL_DATA_TOKEN_WF2:
	    type = L_INPUT_WF2;
	    break;
	case EL_DATA_TOKEN_WS:
	    type = L_INPUT_WS;
	    break;
	case EL_DATA_TOKEN_ELEMENT_SIZE:
	    type = L_INPUT_ELEMENT_SIZE;
	    break;
        case EL_DATA_TOKEN_DEF_STRUCT:
	    type = L_INPUT_DEF_STRUCT;
	    break;
        case EL_DATA_TOKEN_DEF_UNION:
	    type = L_INPUT_DEF_UNION;
	    break;
        case EL_DATA_TOKEN_DEF_ENUM:
	    type = L_INPUT_DEF_ENUM;
	    break;
        case EL_DATA_TOKEN_FIELD:
	    type = L_INPUT_FIELD;
	    break;
        case EL_DATA_TOKEN_ENUMERATOR:
	    type = L_INPUT_ENUMERATOR;
	    break;
	default:
	    El_punt("El_lcode_data_token_type: illegal token type %d", el_t_type);
	    type = -1;
	    break;
    }

    return (type);
}


int El_lcode_to_elcor_ms_type(El_data_memory_segment_type el_ms_type)
{
    switch (el_ms_type) {
	case EL_DATA_MS_TEXT:
	    return (L_MS_TEXT);
	case EL_DATA_MS_DATA:
	    return (L_MS_DATA);
	case EL_DATA_MS_DATA1:
	    return (L_MS_DATA1);
	case EL_DATA_MS_DATA2:
	    return (L_MS_DATA2);
	case EL_DATA_MS_BSS:
	    return (L_MS_BSS);
	case EL_DATA_MS_SYNC:
	    return (L_MS_SYNC);
	default:
	    El_punt("El_lcode_to_elcor_ms_type: unknown elcor ms type");
	    return (-1);
    }
}

unsigned int El_elcor_to_lcode_stype_flags(El_data_source_type_info *el_stype)
{
    unsigned int flags = 0;

    if (el_stype->flags.bit(EL_DATA_ST_CONST))
        flags |= L_DATA_CONST;
    if (el_stype->flags.bit(EL_DATA_ST_VOLATILE))
        flags |= L_DATA_VOLATILE;
    if (el_stype->flags.bit(EL_DATA_ST_NOALIAS))
        flags |= L_DATA_NOALIAS;
    if (el_stype->flags.bit(EL_DATA_ST_REGISTER))
        flags |= L_DATA_REGISTER;
    if (el_stype->flags.bit(EL_DATA_ST_AUTO))
        flags |= L_DATA_AUTO;
    if (el_stype->flags.bit(EL_DATA_ST_STATIC))
        flags |= L_DATA_STATIC;
    if (el_stype->flags.bit(EL_DATA_ST_EXTERN))
        flags |= L_DATA_EXTERN;
    if (el_stype->flags.bit(EL_DATA_ST_GLOBAL))
        flags |= L_DATA_GLOBAL;
    if (el_stype->flags.bit(EL_DATA_ST_PARAMETER))
        flags |= L_DATA_PARAMETER;
    if (el_stype->flags.bit(EL_DATA_ST_VOID))
        flags |= L_DATA_VOID;
    if (el_stype->flags.bit(EL_DATA_ST_CHAR))
        flags |= L_DATA_CHAR;
    if (el_stype->flags.bit(EL_DATA_ST_SHORT))
        flags |= L_DATA_SHORT;
    if (el_stype->flags.bit(EL_DATA_ST_INT))
        flags |= L_DATA_INT;
    if (el_stype->flags.bit(EL_DATA_ST_LONG))
        flags |= L_DATA_LONG;
    if (el_stype->flags.bit(EL_DATA_ST_FLOAT))
        flags |= L_DATA_FLOAT;
    if (el_stype->flags.bit(EL_DATA_ST_DOUBLE))
        flags |= L_DATA_DOUBLE;
    if (el_stype->flags.bit(EL_DATA_ST_SIGNED))
        flags |= L_DATA_SIGNED;
    if (el_stype->flags.bit(EL_DATA_ST_UNSIGNED))
        flags |= L_DATA_UNSIGNED;
    if (el_stype->flags.bit(EL_DATA_ST_STRUCT))
        flags |= L_DATA_STRUCT;
    if (el_stype->flags.bit(EL_DATA_ST_UNION))
        flags |= L_DATA_UNION;
    if (el_stype->flags.bit(EL_DATA_ST_ENUM))
        flags |= L_DATA_ENUM;
    if (el_stype->flags.bit(EL_DATA_ST_VARARG))
        flags |= L_DATA_VARARG;

    return (flags);
}

int El_elcor_to_lcode_access_meth(El_data_access_method el_method)
{
    switch (el_method) {
	case EL_DATA_ACCESS_ARRAY:
	    return (L_D_ARRY);
	case EL_DATA_ACCESS_POINTER:
	    return (L_D_PTR);
	case EL_DATA_ACCESS_FUNCTION:
	    return (L_D_FUNC);
	default:
            El_punt("El_elcor_to_lcode_access_meth: unknown data access method");
	    return (-1);
    }
}

L_Type *El_elcor_to_lcode_src_type(El_data_source_type_info *el_s_type)
{
    L_Type *l_type;
    L_Dcltr *l_dcltr;
    El_data_access_declarator *el_dcltr;

    if (el_s_type == NULL)
        return (NULL);

    l_type = L_new_type();
    l_type->type = El_elcor_to_lcode_stype_flags(el_s_type);
    l_type->struct_name = L_add_string(L_string_table, (char *) el_s_type->struct_name);
    for (List_iterator<El_data_access_declarator*> li(el_s_type->access_dcltrs);
						   li!=0; li++) {
	el_dcltr = *li;
        l_dcltr = L_new_dcltr();
	l_dcltr->method = El_elcor_to_lcode_access_meth(el_dcltr->method);
	l_dcltr->index = El_elcor_to_lcode_expr(el_dcltr->index);
	l_type->dcltr = L_concat_dcltr(l_type->dcltr, l_dcltr);
    }

    return (l_type);
}

L_Data *El_elcor_to_lcode_data(El_data *d)
{
    L_Data *data=NULL;

    data = L_new_data(El_lcode_data_token_type(d->t_type));
    L_data = data;

    /* Fill in the appropriate fields based on the type */
    switch (d->t_type) {
	case EL_DATA_TOKEN_MS:
	    data->N = El_lcode_to_elcor_ms_type(d->ms_type);
            if (d->src_type != NULL)
                El_punt("El_elcor_to_lcode_data: Elcor data has unexpected src type!");
	    break;
	case EL_DATA_TOKEN_RESERVE:
	    data->N = d->N;
            if (d->src_type != NULL)
                El_punt("El_elcor_to_lcode_data: Elcor data has unexpected src type!");
	    break;
	case EL_DATA_TOKEN_GLOBAL:
	    if (d->address==NULL)
		El_punt("El_elcor_to_lcode_data: bad address");
	    data->address = El_elcor_to_lcode_expr(d->address);
            data->h_type = El_elcor_to_lcode_src_type(d->src_type);
	    break;
	case EL_DATA_TOKEN_VOID:
	case EL_DATA_TOKEN_BYTE:
	case EL_DATA_TOKEN_WORD:
	case EL_DATA_TOKEN_LONG:
	case EL_DATA_TOKEN_FLOAT:
	case EL_DATA_TOKEN_DOUBLE:
	case EL_DATA_TOKEN_ALIGN:
	case EL_DATA_TOKEN_ELEMENT_SIZE:
	    data->N = d->N;
	    if (d->address==NULL)
		El_punt("El_elcor_to_lcode_data: bad address");
	    data->address = El_elcor_to_lcode_expr(d->address);
	    if (d->value!=NULL)
		data->value = El_elcor_to_lcode_expr(d->value);
            if (d->src_type != NULL)
                El_punt("El_elcor_to_lcode_data: Elcor data has unexpected src type!");
	    break;
	case EL_DATA_TOKEN_ASCII:
	case EL_DATA_TOKEN_ASCIZ:
	case EL_DATA_TOKEN_WB:
	case EL_DATA_TOKEN_WW:
	case EL_DATA_TOKEN_WI:
	case EL_DATA_TOKEN_WF:
	case EL_DATA_TOKEN_WF2:
	case EL_DATA_TOKEN_WS:
	    if ((d->address==NULL) || (d->value==NULL))
		El_punt("El_elcor_to_lcode_data: bad pointer");
	    data->address = El_elcor_to_lcode_expr(d->address);
	    data->value = El_elcor_to_lcode_expr(d->value);
            if (d->src_type != NULL)
                El_punt("El_elcor_to_lcode_data: Elcor data has unexpected src type!");
	    break;
        case EL_DATA_TOKEN_DEF_STRUCT:
        case EL_DATA_TOKEN_DEF_UNION:
        case EL_DATA_TOKEN_DEF_ENUM:
            if (d->address==NULL)
                El_punt("El_elcor_to_lcode_data: bad address");
            data->address = El_elcor_to_lcode_expr(d->address);
            if (d->src_type != NULL)
                El_punt("El_elcor_to_lcode_data: Elcor data has unexpected src type!");
            break;
        case EL_DATA_TOKEN_FIELD:
            if (d->address==NULL)
                El_punt("El_elcor_to_lcode_data: bad address");
            data->address = El_elcor_to_lcode_expr(d->address);
	    if (d->value!=NULL)
		data->value = El_elcor_to_lcode_expr(d->value);
            data->h_type = El_elcor_to_lcode_src_type(d->src_type);
            break;
        case EL_DATA_TOKEN_ENUMERATOR:
            if (d->address==NULL)
                El_punt("El_elcor_to_lcode_data: bad address");
            data->address = El_elcor_to_lcode_expr(d->address);
	    if (d->value!=NULL)
		data->value = El_elcor_to_lcode_expr(d->value);
            if (d->src_type != NULL)
                El_punt("El_elcor_to_lcode_data: Elcor data has unexpected src type!");
	    break;
	default:
	    El_punt("El_elcor_to_lcode_data: illegal data type");
    }

    return (data);
}

L_Datalist *El_elcor_to_lcode_datalist(El_datalist *d)
{
    L_Datalist *datalist;
    L_Datalist_Element *element;
    L_Data *data;

    if (d == NULL)
	return (NULL);

    datalist = L_new_datalist();
    for (List_iterator<El_data *> li(d->elements); li!=0; li++) {
	data = El_elcor_to_lcode_data(*li);
	element = L_new_datalist_element(data);
	L_concat_datalist_element(datalist, element);
    }

    return (datalist);
}


/*============================================================================*/
/*
 *	Jumptbl conversion
 */
/*============================================================================*/

L_Datalist *El_elcor_to_lcode_jumptbl_info(El_jumptbl_info *jumptbl_info)
{
    L_Data *data;
    L_Datalist_Element *element;
    L_Datalist *data_list;
    L_Expr *expr, *expr2, *exprA, *exprB;
    El_jumptbl *tbl;
    char tmp[1024], *proc_name, *tbl_name;
    int i, align, cb_id;

    if (jumptbl_info==NULL)
	return (NULL);

    data_list = L_new_datalist();
    proc_name = (char *) jumptbl_info->get_proc_name();

    /* First put a (ms data) data item on the list */
    data = L_new_data(L_INPUT_MS);
    data->N = L_MS_DATA;
    element = L_new_datalist_element(data);
    L_concat_datalist_element(data_list, element);

    for (List_iterator<El_jumptbl *> li(jumptbl_info->get_tbl_list()); li!=0; li++) {
	tbl = *li;
	tbl_name = (char *) tbl->get_name();

	/* Make sure the cbid targets are correct */
	tbl->construct_cbid_targets_from_op_targets();

	/* First item is the align data */
	expr = L_new_expr(L_EXPR_LABEL);
	expr->value.l = L_add_string(L_string_table, tbl_name);
	data = L_new_data(L_INPUT_ALIGN);
	align = tbl->get_align();
	data->N = align;
	data->address = expr;
	element = L_new_datalist_element(data);
	L_concat_datalist_element(data_list, element);

	/* Second item is the reserve data */
	data = L_new_data(L_INPUT_RESERVE);
	data->N = tbl->get_reserve();
	element = L_new_datalist_element(data);
	L_concat_datalist_element(data_list, element);

	/* Now for the actual table entries */
	for (i=0; i<tbl->get_n_entries(); i++) {
	    cb_id = tbl->get_cbid_target(i);
	    if (i==0) {
	        expr = L_new_expr(L_EXPR_LABEL);
		expr->value.l = L_add_string(L_string_table, tbl_name);
	    }
	    else {
		exprA = L_new_expr(L_EXPR_LABEL);
		exprA->value.l = L_add_string(L_string_table, tbl_name);
		exprB = L_new_expr(L_EXPR_INT);
		exprB->value.i = (i*align);
		expr = L_new_expr(L_EXPR_ADD);
		expr->A = exprA;
		expr->B = exprB;
	    }

	    expr2 = L_new_expr(L_EXPR_LABEL);
	    M_cb_label_name(proc_name, cb_id, &tmp[0], 0);
	    expr2->value.l = L_add_string(L_string_table, tmp);

	    data = L_new_data(L_INPUT_WI);
	    data->address = expr;
	    data->value = expr2;
	    element = L_new_datalist_element(data);
	    L_concat_datalist_element(data_list, element);
	}
    }

    return (data_list);
}
