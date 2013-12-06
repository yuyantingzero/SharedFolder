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
//      File:           el_lcode_to_elcor.cpp
//      Authors:        Sadun Anik, Scott A. Mahlke,
//                      Dave August, Richard Johnson
//      Created:        April 1995
//      Description:    Convert IMPACT Lcode to Elcor IR
//
/////////////////////////////////////////////////////////////////////////////

#include "el_main.h"
#include "dbg.h"
#include "opcode.h"
#include "hash_functions.h"
#include "el_memory_dependence.h"
#include "el_bb_tools.h"
#include "el_mem_prof_to_sync_vars.h"
#include "edge_utilities.h"
#include "el_stats_init.h"
#include "el_control.h"
#include "mdes.h"
#include "intf.h"
#include "el_clock.h"
#include "el_cache_init.h"
#include "dtype.h"
#include "acc_spec.h"
#include "op_attributes.h"


/*============================================================
 *
 *  Bitwidth attribute map
 *
 *============================================================*/

static Map<int, int> bitwidth_map;

/*============================================================================*/
/*
 *	Conversion of Functions
 */
/*============================================================================*/

static Hash_map<L_Oper*, Op*> op_map(hash_L_Oper_ptr, 1000) ;

static bool scalar_regalloc_done = false;

/*============================================================
 *
 *  Flag conversion
 *
 *============================================================*/

void El_lcode_to_elcor_op_flags(L_Oper *op, Op *el_op)
{
    unsigned int flags;

    flags = op->flags;
    flags = L_CLR_BIT_FLAG(flags, L_OPER_HIDDEN_FLAGS);

    if (L_EXTRACT_BIT_VAL(flags, L_OPER_CHECK))
	el_op->set_flag(EL_OPER_CHECK) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_LABEL_REFERENCE))
	el_op->set_flag(EL_OPER_LABEL_REFERENCE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_PROMOTED))
	el_op->set_flag(EL_OPER_PROMOTED) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_SQUASHING))
	el_op->set_flag(EL_OPER_SQUASHING) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_DATA_SPECULATIVE))
	el_op->set_flag(EL_OPER_DATA_SPECULATIVE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_SPILL_CODE))
	el_op->set_flag(EL_OPER_SPILL_CODE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_SIDE_EFFECT_FREE))
	el_op->set_flag(EL_OPER_SIDE_EFFECT_FREE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_SPECULATIVE))
	el_op->set_flag(EL_OPER_SPECULATIVE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_MASK_PE))
	el_op->set_flag(EL_OPER_MASK_PE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_SAFE_PEI))
	el_op->set_flag(EL_OPER_SAFE_PEI) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_PROBE_MARK))
	el_op->set_flag(EL_OPER_PROBE_MARK) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_SYNC))
	el_op->set_flag(EL_OPER_SYNC) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_PROCESSOR_SPECIFIC))
	el_op->set_flag(EL_OPER_PROCESSOR_SPECIFIC) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_VOLATILE))
	el_op->set_flag(EL_OPER_VOLATILE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_NO_SPECULATION))
	el_op->set_flag(EL_OPER_NO_SPECULATION) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_STACK_REFERENCE))
	el_op->set_flag(EL_OPER_STACK_REFERENCE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_ROTATE_REGISTERS))
	el_op->set_flag(EL_OPER_ROTATE_REGISTERS) ;
    if (L_EXTRACT_BIT_VAL(flags, L_OPER_SUPER_SPECULATION))
	el_op->set_flag(EL_OPER_SUPER_SPECULATION) ;
}

void El_lcode_op_attr_to_elcor_op_flag(L_Oper *op, Op *el_op)
{
    L_Attr *attr;

    attr = L_find_attr(op->attr, "nonseq_db");
    if (attr != NULL) {
        el_op->set_flag(EL_OPER_NONSEQ_DUMMY_BR);
        op->attr = L_delete_attr(op->attr, attr);
    }
    attr = L_find_attr(op->attr, "liveout");
    if (attr != NULL) {
        el_op->set_flag(EL_OPER_LIVEOUT_DEST);
        op->attr = L_delete_attr(op->attr, attr);
    }
}

void El_lcode_to_elcor_cb_flags(L_Cb *cb, Compound_region *el_cb)
{
    unsigned int flags;

    flags = cb->flags;
    flags = L_CLR_BIT_FLAG(flags, L_CB_HIDDEN_FLAGS);

    if (L_EXTRACT_BIT_VAL(flags, L_CB_HYPERBLOCK))
	el_cb->set_flag(EL_REGION_HYPERBLOCK) ;
    if (L_EXTRACT_BIT_VAL(flags, L_CB_HYPERBLOCK_NO_FALLTHRU))
	el_cb->set_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU) ;
    if (L_EXTRACT_BIT_VAL(flags, L_CB_SOFTPIPE))
	el_cb->set_flag(EL_REGION_SOFTPIPE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_CB_SUPERBLOCK))
	el_cb->set_flag(EL_REGION_SUPERBLOCK) ;
    if (L_EXTRACT_BIT_VAL(flags, L_CB_UNROLLED))
	el_cb->set_flag(EL_REGION_UNROLLED) ;
    if (L_EXTRACT_BIT_VAL(flags, L_CB_ENTRANCE_BOUNDARY))
	el_cb->set_flag(EL_REGION_ENTRANCE_BOUNDARY);
    if (L_EXTRACT_BIT_VAL(flags, L_CB_EXIT_BOUNDARY))
	el_cb->set_flag(EL_REGION_EXIT_BOUNDARY);
    if (L_EXTRACT_BIT_VAL(flags, L_CB_PROLOGUE))
	el_cb->set_flag(EL_REGION_HAS_PROLOGUE_OP);
    if (L_EXTRACT_BIT_VAL(flags, L_CB_EPILOGUE))
	el_cb->set_flag(EL_REGION_HAS_EPILOGUE_OP);
    if (L_EXTRACT_BIT_VAL(flags, L_CB_VIOLATES_LC_SEMANTICS))
	el_cb->set_flag(EL_REGION_VIOLATES_LC_SEMANTICS);
    if (L_EXTRACT_BIT_VAL(flags, L_CB_ROT_REG_ALLOCATED))
	el_cb->set_flag(EL_REGION_ROT_REG_ALLOCATED);
}

void El_lcode_cb_attr_to_elcor_cb_flag(L_Cb *cb, Compound_region *el_cb)
{
    L_Attr *attr;

    attr = L_find_attr(cb->attr, "hb_frp");
    if (attr != NULL) {
	el_cb->set_flag(EL_REGION_HYPERBLOCK_FRP);
	cb->attr = L_delete_attr(cb->attr, attr);
    }
    attr = L_find_attr(cb->attr, "preloop");
    if (attr != NULL) {
	el_cb->set_flag(EL_REGION_PRELOOP_BLOCK);
	cb->attr = L_delete_attr(cb->attr, attr);
    }
    attr = L_find_attr(cb->attr, "postloop");
    if (attr != NULL) {
	el_cb->set_flag(EL_REGION_POSTLOOP_BLOCK);
	cb->attr = L_delete_attr(cb->attr, attr);
    }
    attr = L_find_attr(cb->attr, "explicit_ft");
    if (attr != NULL) {
        el_cb->set_flag(EL_REGION_HAS_EXPLICIT_FT);
        cb->attr = L_delete_attr(cb->attr, attr);
    }
}

void El_lcode_to_elcor_func_flags(L_Func *fn, Procedure *el_fn)
{
    unsigned int flags;

    flags = fn->flags;
    flags = L_CLR_BIT_FLAG(flags, L_FUNC_HIDDEN_FLAGS);

    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_HYPERBLOCK))
	el_fn->set_flag(EL_PROC_HYPERBLOCK) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_LEAF))
	el_fn->set_flag(EL_PROC_LEAF) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_SIDE_EFFECT_FREE))
	el_fn->set_flag(EL_PROC_SIDE_EFFECT_FREE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_REGISTER_ALLOCATED))
	el_fn->set_flag(EL_PROC_REGISTER_ALLOCATED) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_SUPERBLOCK))
	el_fn->set_flag(EL_PROC_SUPERBLOCK) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_MASK_PE))
	el_fn->set_flag(EL_PROC_MASK_PE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_COMPILATION_COMPLETE))
	el_fn->set_flag(EL_PROC_COMPILATION_COMPLETE) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_SCHEDULED))
	el_fn->set_flag(EL_PROC_SCHEDULED) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_CC_IN_PREDICATE_REGS))
	el_fn->set_flag(EL_PROC_CC_IN_PREDICATE_REGS) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_PRED_REGS_IN_ATTR))
	el_fn->set_flag(EL_PROC_PRED_REGS_IN_ATTR) ;
    if (L_EXTRACT_BIT_VAL(flags, L_FUNC_ROT_REG_ALLOCATED))
	el_fn->set_flag(EL_PROC_ROT_REG_ALLOCATED) ;
}

void El_lcode_func_attr_to_elcor_func_flag(L_Func *fn, Procedure *el_fn)
{
    /* Currently there are no func flags passed as attrs thru Impact */
}

/*============================================================
 *
 *  Attr conversion
 *
 *============================================================*/

void El_lcode_to_elcor_attr(Region* R, L_Attr *attr)
{
   Lcode_attribute_map* map_ptr ;
   List<Operand> tmp_list ;

   map_ptr = get_lcode_attributes(R) ;
   tmp_list.clear() ;

   for (int i=0; i<attr->max_field; i++) {
      Operand tmp_oper(El_lcode_to_elcor_operand(attr->field[i]));
      tmp_list.add_tail(tmp_oper) ;
   }
   map_ptr->bind(attr->name, tmp_list) ;
}

/*============================================================
 *
 *  Sync arc conversion
 *
 *============================================================*/

void El_lcode_to_elcor_sync(Op* el_op, L_Sync *sync)
{
   if (sync->dep_oper) {
      Op* src_op = op_map.value(sync->dep_oper) ;
      for (Op_mem_dests mdi(src_op) ; mdi != 0 ; mdi++) {
        (*mdi).set_omega(sync->dist);
	 el_op->add_mem_src(*mdi) ;
      }
   }
   else {
      El_warn("Unknown sync arc is ignored, it doesn't have source oper") ;
   }
}

/*
 * AccSpec conversion
 */

void El_lcode_to_elcor_accspec(Op *el_op, L_AccSpec *acc_spec_list)
{
  AccSpecAttr *acc_spec_attr;
  L_AccSpec *acc_spec = acc_spec_list;
  if (acc_spec) {
    acc_spec_attr = new AccSpecAttr();
    set_accspec_attr(el_op, acc_spec_attr);
  } 
  while (acc_spec) {
    AccSpec *as = new AccSpec();
    as->setIsDef(acc_spec->is_def);
    as->setObjectId(acc_spec->id);
    as->setVersion(acc_spec->version);
    as->setSize(acc_spec->size);
    as->setOffset(acc_spec->offset);
    // as->print(cdbg);

    acc_spec_attr->acc_spec_list.add_tail(as);

    acc_spec = acc_spec->next;
  }
}

/*============================================================
 *
 *  Operand conversion
 *
 *============================================================*/

DType L_lcode_to_elcor_data_type(int impact_ctype, bool is_label)
{
    DType dt;
    unsigned int width = 0;
    // s is whether or not a datatype is signed. Thanks for the
    // descriptive variable names and commenting your code...
    bool s = false;
    DType_info info = EL_DT_INFO_INT;

    switch (impact_ctype) {
	case L_CTYPE_VOID:
	    width = 0;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_CHAR:
	    width = EL_DEFAULT_CHAR_WIDTH;
	    s = true;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_UCHAR:
	    width = EL_DEFAULT_CHAR_WIDTH;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_SHORT:
	    width = EL_DEFAULT_SHORT_WIDTH;
	    s = true;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_USHORT:
	    width = EL_DEFAULT_SHORT_WIDTH;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_INT:
	    width = EL_DEFAULT_INT_WIDTH;
	    s = true;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_UINT:
	    width = EL_DEFAULT_INT_WIDTH;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_LONG:
	    width = EL_DEFAULT_LONG_WIDTH;
	    s = true;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_ULONG:
	    width = EL_DEFAULT_LONG_WIDTH;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_LLONG:
	    width = EL_DEFAULT_LONGLONG_WIDTH;
	    s = true;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_ULLONG:
	    width = EL_DEFAULT_LONGLONG_WIDTH;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_LLLONG:
	    width = EL_DEFAULT_LONGLONGLONG_WIDTH;
	    s = true;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_ULLLONG:
	    width = EL_DEFAULT_LONGLONGLONG_WIDTH;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_POINTER:
	    width = EL_DEFAULT_POINTER_WIDTH;
	    s = false;
	    info = EL_DT_INFO_POINTER;
	    break;
	case L_CTYPE_FLOAT:
	    width = EL_DEFAULT_SINGLE_WIDTH;
	    s = true;
	    info = EL_DT_INFO_FLOAT;
	    break;
	case L_CTYPE_DOUBLE:
	    width = EL_DEFAULT_DOUBLE_WIDTH;
	    s = true;
	    info = EL_DT_INFO_FLOAT;
	    break;
	case L_CTYPE_PREDICATE:
	    width = EL_DEFAULT_PREDICATE_WIDTH;
	    s = false;
	    info = EL_DT_INFO_PREDICATE;
	    break;
	case L_CTYPE_CONTROL:
	    width = EL_DEFAULT_INT_WIDTH;
	    s = false;
	    info = EL_DT_INFO_INT;
	    break;
	case L_CTYPE_BTR:
	    width = EL_DEFAULT_BRANCH_WIDTH;
	    s = false;
	    info = EL_DT_INFO_BRANCH;
	    break;
	case L_CTYPE_LOCAL_ABS:
	    if (is_label)
	        width = EL_DEFAULT_LABEL_WIDTH;
	    else
	        width = EL_DEFAULT_STRING_WIDTH;
	    s = false;
	    info = EL_DT_INFO_LOCAL_ABS;
	    break;
	case L_CTYPE_LOCAL_GP:
	    if (is_label)
	        width = EL_DEFAULT_LABEL_WIDTH;
	    else
	        width = EL_DEFAULT_STRING_WIDTH;
	    s = false;
	    info = EL_DT_INFO_LOCAL_GP;
	    break;
	case L_CTYPE_GLOBAL_ABS:
	    if (is_label)
	        width = EL_DEFAULT_LABEL_WIDTH;
	    else
	        width = EL_DEFAULT_STRING_WIDTH;
	    s = false;
	    info = EL_DT_INFO_GLOBAL_ABS;
	    break;
	case L_CTYPE_GLOBAL_GP:
	    if (is_label)
	        width = EL_DEFAULT_LABEL_WIDTH;
	    else
	        width = EL_DEFAULT_STRING_WIDTH;
	    s = false;
	    info = EL_DT_INFO_GLOBAL_GP;
	    break;
        /* RMR { adding support for vector file type */
        case L_CTYPE_VECTOR_INT:
            width = EL_DEFAULT_INT_WIDTH;
            s = true;
            info = EL_DT_INFO_VEC_INT;
        case L_CTYPE_VECTOR_FLOAT:
            width = EL_DEFAULT_SINGLE_WIDTH;
            s = true;
            info = EL_DT_INFO_VEC_FLOAT;
        case L_CTYPE_VECTOR_DOUBLE:
            width = EL_DEFAULT_DOUBLE_WIDTH;
            s = true;
            info = EL_DT_INFO_VEC_FLOAT;
        case L_CTYPE_VECTOR_MASK:
            width = EL_DEFAULT_INT_WIDTH;
            s = true;
            info = EL_DT_INFO_VEC_MASK;
        /* } RMR */
	default:
	    El_punt("L_lcode_to_elcor_data_type: uknown impact data type %d",
			impact_ctype);
	    break;
    }

    dt.set_width(width);
    dt.set_sign(s);
    dt.set_info(info);

    return (dt);
}

Reg_file L_lcode_to_elcor_file_type(int impact_ctype)
{
    switch (impact_ctype) {
	case L_CTYPE_VOID:		return (GPR);
	case L_CTYPE_CHAR:		return (GPR);
	case L_CTYPE_UCHAR:		return (GPR);
	case L_CTYPE_SHORT:		return (GPR);
	case L_CTYPE_USHORT:		return (GPR);
	case L_CTYPE_INT:		return (GPR);
	case L_CTYPE_UINT:		return (GPR);
	case L_CTYPE_LONG:		return (GPR);
	case L_CTYPE_ULONG:		return (GPR);
	case L_CTYPE_LLONG:		return (GPR);
	case L_CTYPE_ULLONG:		return (GPR);
	case L_CTYPE_LLLONG:		return (GPR);
	case L_CTYPE_ULLLONG:		return (GPR);
	case L_CTYPE_POINTER:		return (GPR);
	case L_CTYPE_FLOAT:		return (FPR);
	case L_CTYPE_DOUBLE:		return (FPR);
	case L_CTYPE_PREDICATE:		return (PR);
	case L_CTYPE_CONTROL:		return (CR);
	case L_CTYPE_BTR:		return (BTR);
	case L_CTYPE_LOCAL_ABS:		return (GPR);
	case L_CTYPE_LOCAL_GP:		return (GPR);
	case L_CTYPE_GLOBAL_ABS:	return (GPR);
	case L_CTYPE_GLOBAL_GP:		return (GPR);
        /* RMR { adding support for vector file type */
        case L_CTYPE_VECTOR_INT:        return (VIR);
        case L_CTYPE_VECTOR_FLOAT:      return (VFR);
        case L_CTYPE_VECTOR_DOUBLE:     return (VFR);
        case L_CTYPE_VECTOR_MASK:       return (VMR);
        /* } RMR */
	default:
	    El_punt("L_lcode_to_elcor_file_type: uknown impact data type %d",
			impact_ctype);
	    return ((Reg_file) 0);
   }
}

Macro_name L_lcode_to_elcor_macro_name(L_Operand *macro_operand)
{
  int lcode_macro;

  lcode_macro = macro_operand->value.mac;

  switch(lcode_macro) {
    case L_MAC_LOCAL_SIZE:
      return LOCAL;
    case L_MAC_PARAM_SIZE:
      return PARAM;
    case L_MAC_SWAP_SIZE:
      return SWAP;
    case L_MAC_RET_TYPE:
      if (L_is_ctype_integer(macro_operand))
          return INT_RETURN_TYPE;
      else if (L_is_ctype_flt(macro_operand))
          return FLT_RETURN_TYPE;
      else if (L_is_ctype_dbl(macro_operand))
          return DBL_RETURN_TYPE;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_RET_TYPE");
    case L_MAC_P15:
      if( ! strcasecmp(El_arch, "arm")) {
 	if (L_is_ctype_flt(macro_operand) || L_is_ctype_dbl(macro_operand))
	  return FLT_RETURN;
	else
	  return INT_RETURN;
      } else {
	return INT_RETURN;
      }
    case L_MAC_P0:
      return  INT_PARAM_1;
    case L_MAC_P1:
      return  INT_PARAM_2;
    case L_MAC_P2:
      return  INT_PARAM_3;
    case L_MAC_P3:
      return  INT_PARAM_4;
    case L_MAC_P4:
      if (L_is_ctype_flt(macro_operand))
	  return FLT_RETURN;
      else if (L_is_ctype_dbl(macro_operand))
	  return DBL_RETURN;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P4");
    case L_MAC_P5:
      if (L_is_ctype_flt(macro_operand))
          return FLT_PARAM_1;
      else if (L_is_ctype_dbl(macro_operand))
	  return DBL_PARAM_1;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P5");
    case L_MAC_P6:
      if(L_is_ctype_flt(macro_operand))
          return FLT_PARAM_2;
      else if(L_is_ctype_dbl(macro_operand)) /* KVM: Jun 17th 2003 */
          return DBL_PARAM_2;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P6");
    case L_MAC_P7:
      if (L_is_ctype_flt(macro_operand))
          return FLT_PARAM_3;
      else if (L_is_ctype_dbl(macro_operand))
          return DBL_PARAM_3;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P7");
    case L_MAC_P8:
      if (L_is_ctype_flt(macro_operand))
          return FLT_PARAM_4;
      else if (L_is_ctype_dbl(macro_operand))
          return DBL_PARAM_4;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P8");

    // for the ARM port
    case L_MAC_P9:
      if (L_is_ctype_dbl(macro_operand))
	return DBL_PARAM_1;
      else
	El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P9");
    case L_MAC_P10:
      if (L_is_ctype_dbl(macro_operand))
	return DBL_PARAM_2;
      else
	El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P10");
    case L_MAC_P11:
      if (L_is_ctype_dbl(macro_operand))
	return DBL_PARAM_3;
      else
	El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P11");
    case L_MAC_P12:
      if (L_is_ctype_dbl(macro_operand))
	return DBL_PARAM_4;
      else
	El_punt("L_lcode_to_elcor_macro_name: unknown data type for L_MAC_P11");


    case L_MAC_TM_TYPE:
      if (L_is_ctype_integer(macro_operand))
          return  INT_TM_TYPE;
      else if (L_is_ctype_flt(macro_operand))
	  return FLT_TM_TYPE;
      else if (L_is_ctype_dbl(macro_operand))
          return DBL_TM_TYPE;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for TM_TYPE");
    case L_MAC_SP:
      return SP_REG;
    case L_MAC_FP:
      return FP_REG;
    case L_MAC_IP:
      return IP_REG;
    case L_MAC_OP:
      return OP_REG;
    case L_MAC_LV:
      return LV_REG;
    case L_MAC_RS:
      return RGS_REG;
    case PLAYDOH_MAC_LC:
      return LC_REG;
    case PLAYDOH_MAC_ESC:
      return ESC_REG;
    case L_MAC_PRED_ALL:
      return ALL_PRED;
    case PLAYDOH_MAC_PRED_ALL_ROT:
      return ALL_ROT_PRED;
    case PLAYDOH_MAC_PRED_ALL_STATIC:
      return ALL_STATIC_PRED;
    case PLAYDOH_MAC_RRB:
      return RRB;
    case PLAYDOH_MAC_RETADDR:
    case ARM_MAC_RETADDR:
      return RETURN_ADDR;
    case PLAYDOH_MAC_FZERO:
    case ARM_MAC_FZERO:
      if (L_is_ctype_flt(macro_operand))
	  return FLT_ZERO;
      else if (L_is_ctype_dbl(macro_operand))
	  return DBL_ZERO;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for PLAYDOH_MAC_FZERO");
    case PLAYDOH_MAC_FONE:
    case ARM_MAC_FONE:
      if (L_is_ctype_flt(macro_operand))
	  return FLT_ONE;
      else if (L_is_ctype_dbl(macro_operand))
	  return DBL_ONE;
      else
          El_punt("L_lcode_to_elcor_macro_name: unknown data type for PLAYDOH_MAC_FONE");
    case PLAYDOH_MAC_ZERO:
      return INT_ZERO;
    case PLAYDOH_MAC_PRED_FALSE:
      return PRED_FALSE;
    case PLAYDOH_MAC_PRED_TRUE:
      return PRED_TRUE;
    case PLAYDOH_MAC_TEMPREG:
      return SPILL_TEMPREG;
    case PLAYDOH_MAC_PV_0:
      return PV_0;
    case PLAYDOH_MAC_PV_1:
      return PV_1;
    case PLAYDOH_MAC_PV_2:
      return PV_2;
    case PLAYDOH_MAC_PV_3:
      return PV_3;
    case PLAYDOH_MAC_PV_4:
      return PV_4;
    case PLAYDOH_MAC_PV_5:
      return PV_5;
    case PLAYDOH_MAC_PV_6:
      return PV_6;
    case PLAYDOH_MAC_PV_7:
      return PV_7;
    default:
      El_punt("L_lcode_to_elcor_macro_name: Incorrect macro type");
    }
  return UNDEFINED;
}

Base_operand* El_lcode_to_elcor_operand(L_Operand *operand)
{
   DType dtype ;
   Reg_file rfile ;
   Macro_name mname;
   Base_operand* tmp_oper = NULL;

   if (operand==NULL) {
      tmp_oper = new Undefined ;
      return (tmp_oper);
   }

   dtype = L_lcode_to_elcor_data_type(operand->ctype,
				      (bool)L_is_label(operand));

   if(El_do_read_impact_widths) {
     if (L_is_reg(operand) && bitwidth_map.is_bound(operand->value.r)) {
       dtype.set_width(bitwidth_map.value(operand->value.r));
     }
   }

   /* L_OPERAND_CB */
   if (L_is_cb(operand)) {
      /* Need to overide the dtype of L_CTYPE_INT that Impact computes */
      tmp_oper = new Cb_operand(operand->value.cb->id);
   }
   /* L_OPERAND_IMMED, ctype = [L_CTYPE_VOID, L_CTYPE_POINTER] */
   else if (L_is_int_constant(operand)) {
      if (dtype.is_signed())
          tmp_oper = new Int_lit((int)operand->value.i);
      else
          tmp_oper = new Int_lit((unsigned)operand->value.i);
   }
   /* L_OPERAND_IMMED, ctype = L_CTYPE_FLOAT */
   else if (L_is_flt_constant(operand)) {
      if (dtype.is_signed()) 
          tmp_oper = new Float_lit(operand->value.f);
      else
          tmp_oper = new Float_lit((unsigned)operand->value.f);
   }
   /* L_OPERAND_IMMED, ctype = L_CTYPE_DOUBLE */
   else if (L_is_dbl_constant(operand)) {
      if (dtype.is_signed()) 
          tmp_oper = new Double_lit(operand->value.f2);
      else
          tmp_oper = new Double_lit((unsigned)operand->value.f2);
   }
   /* L_OPERAND_STRING */
   else if (L_is_string(operand)) {
      tmp_oper = new String_lit(operand->value.s, dtype.info()) ;
   }
   /* L_OPERAND_LABEL */
   else if (L_is_label(operand)) {
      tmp_oper = new Label_lit(operand->value.l, dtype.info()) ;
   }
   /* L_OPERAND_MACRO */
   else if (L_is_macro(operand)) {
      mname = L_lcode_to_elcor_macro_name(operand);
      tmp_oper = new Macro_reg(mname);
      tmp_oper->set_dtype(dtype);
   }
   /* L_OPERAND_REGISTER */
   else if (L_is_reg(operand)) {
      rfile = L_lcode_to_elcor_file_type(operand->ctype);
      tmp_oper = new Reg(dtype, rfile, operand->value.r) ;
      if (scalar_regalloc_done == true) {
         ((Reg *) tmp_oper)->bind_reg(operand->value.r);
      }
   }
   /* L_OPERAND_RREGISTER */
   else if (L_is_rregister(operand)) {
      rfile = L_lcode_to_elcor_file_type(operand->ctype);
      tmp_oper = new Reg(dtype, rfile, operand->value.rr) ;
      /* Rotating allocation must be done to go to Lcode */
      ((Reg *) tmp_oper)->bind_file(rfile, ROTATING);
      ((Reg *) tmp_oper)->bind_reg(operand->value.rr);
   }
   /* L_OPERAND_EVR */
   else if (L_is_evr(operand)) {
      El_punt("El_lcode_to_elcor_operand: currently evr is not handled");
   }
   else {
      El_punt("El_lcode_to_elcor_operand: Unknown operand type!!");
   }

   return (tmp_oper) ;
}


/*============================================================
 *
 *  Oper conversion
 *
 *============================================================*/

Op *El_lcode_to_elcor_nonseq_dummy_br(L_Oper *oper)
{
    Op *el_op;
    L_Attr *attr;

    el_op = new Op(DUMMY_BR, oper->id);

    /*
     *  Convert flags
     */
    El_lcode_to_elcor_op_flags(oper, el_op);
    El_lcode_op_attr_to_elcor_op_flag(oper, el_op);

    /* Check for sched_time attribute, if so, initialize sched_time */
    attr = L_find_attr(oper->attr, "isl");
    if (attr != NULL) {
        el_op->set_sched_time(attr->field[0]->value.i);
        el_op->set_flag(EL_REGION_SCHEDULED);
    }

    /*
     *  Print the attributes
     */
    if (oper->attr!=NULL) {
        for (attr=oper->attr; attr!=NULL; attr=attr->next_attr)
            El_lcode_to_elcor_attr(el_op, attr);
    }

    return (el_op);

}

/*
 * Read new Lcode format: (example)
 *
 *     id  op flags    pred      dest        sources         attributes
 * (op 12 add <AFS> <(r 1 p)> [(r 2 i)] [(r 3 i)(r 4 i)] {(int charchar int int)...}
 *	<(issue 4)(i_t 3)>)
 *
 * <> represents optional fields
 * [] represents standard fields
 *
 * Destination, source, predicate and attribute fields only have parenthesis
 * for operands which are present.
 *
 */

void El_lcode_to_elcor_rest_op(Op *el_op, L_Oper *oper)
{
   L_Attr *attr;
   int i, num;

   /*
    *  Convert flags
    */
   El_lcode_to_elcor_op_flags(oper, el_op);
   El_lcode_op_attr_to_elcor_op_flag(oper, el_op);

   /* Mark all table jumps in Elcor since bru's look both like
	jump_rgs and jumps, and these are treated differently in Lcode */
   if (L_register_branch_opcode(oper))
	el_op->set_flag(EL_OPER_TABLE_JUMP);

   /*
    *  Print predicate operands - predicates are optional
    */
   num = L_max_pred_operand;
   for (i=L_max_pred_operand-1; i>=0; i--)
   {
      if (oper->pred[i]!=NULL)
	 break;
      num--;
   }
   if (num != 0)
   {
      if (num == 1) {
	 Operand tmp_pred_oper(El_lcode_to_elcor_operand(oper->pred[0]));
	 el_op->set_src(PRED1, tmp_pred_oper) ;
      }
      for (i=1; i<num; i++) {
	 cerr << "ERROR: El_lcode_to_elcor_rest_op() throwing away surplus predicates" << endl ;
      }
   } else {
     Operand tmp_pred_oper = new Pred_lit(true) ;
     el_op->set_src(PRED1, tmp_pred_oper) ;
   }

   /*
    *  Print dest operands
    */
   num = L_max_dest_operand;
   for (i=L_max_dest_operand-1; i>=0; i--)
   {
      if (oper->dest[i]!=NULL)
	 break;
      num--;
   }
   for (i=0; i<num; i++) {
      Operand tmp_dest_oper(El_lcode_to_elcor_operand(oper->dest[i]));
      el_op->set_dest((Port_num)(i+el_op->first_dest()), tmp_dest_oper) ;
   }

   /*
    *  Print src operands
    */
   num = L_max_src_operand;
   for (i=L_max_src_operand-1; i>=0; i--)
   {
      if (oper->src[i]!=NULL)
	 break;
      num--;
   }
   for (i=0; i<num; i++) {
      Operand tmp_src_oper(El_lcode_to_elcor_operand(oper->src[i])) ;
      el_op->set_src((Port_num)(i+SRC1), tmp_src_oper) ;
   }

   /* RMR { for vector ops, add default all true mask */
   if (is_vector(el_op) && !is_svmove(el_op) && !is_vsmove(el_op)) {
      Operand default_vmt(new Macro_reg(VEC_MASK_TRUE));
      el_op->set_src((Port_num)(i+SRC1), default_vmt);
   }
   /* } RMR */

   /*
    * Search for any processor specific opcode.  If one exists, it
    * will be set as the proc_opc.  Otherwise the current opc will
    * be set as the proc_opc
    */
   attr = L_find_attr(oper->attr, "popc");
   if (attr !=NULL)
   {
      L_set_int_attr_field(attr, 0, oper->proc_opc);
   }
   else
   {
      if (oper->opc != oper->proc_opc)
      {
	 attr = L_new_attr("popc", 1);
	 L_set_int_attr_field(attr, 0, oper->proc_opc);
	 oper->attr = L_concat_attr(oper->attr, attr);
      }
   }

    /* Check for sched_time attribute, if so, initialize sched_time */
   attr = L_find_attr(oper->attr, "isl");
   if (attr != NULL) {
      el_op->set_sched_time(attr->field[0]->value.i);
      el_op->set_flag(EL_REGION_SCHEDULED);
   }

   /* Check for sched_opcode attribute, if so initialize sched_opcode */
   attr = L_find_attr(oper->attr, EL_SCHED_OPCODE_ATTR);
   if (attr != NULL) {
      el_op->set_sched_opcode(attr->field[0]->value.l);
   }

   /*
    *  Print the attributes
    */
   if (oper->attr!=NULL)
   {
      for (attr=oper->attr; attr!=NULL; attr=attr->next_attr)
	 El_lcode_to_elcor_attr(el_op, attr);
   }


   /*
    *	Jump table attributes require special conversion since they
    *	are specifically represented as an Elcor attr. SAM 7-96
    */
    if (L_register_branch_opcode(oper)) {
	eString tbl_name;
	Jump_table_name *tbl_attr;

	attr = L_find_attr(oper->attr, L_JUMPTBL_OP_ATTR);
	if (attr == NULL)
	    El_punt("El_lcode_to_elcor_rest_op: missing jumptbl attr on lcode op %d",
			oper->id);
	tbl_name = attr->field[0]->value.l;
	tbl_attr = new Jump_table_name(tbl_name);
	set_jump_table_name(el_op, tbl_attr);
    }
}

Opcode El_insert_compare_cond_modifiers(Opcode &opc, L_Oper *oper)
{
    if (! (L_general_comparison_opcode(oper) || L_general_pred_comparison_opcode(oper)))
	El_punt("El_opcode_completor_to_compare_condition: called with noncomparison");

    Opcode mask;
     
    if(L_is_ctype_integer_direct(L_get_compare_ctype(oper))) {

      if (L_is_ctype_signed_direct(L_get_compare_ctype(oper))) {
        switch (L_get_compare_type(oper)) {
	    case Lcmp_COM_EQ:
		mask = (Opcode) IR_EQ;
		break;
	    case Lcmp_COM_NE:
		mask = (Opcode) IR_NEQ;
		break;
	    case Lcmp_COM_GT:
		mask = (Opcode) IR_GT;
		break;
	    case Lcmp_COM_LE:
		mask = (Opcode) IR_LEQ;
		break;
	    case Lcmp_COM_GE:
		mask = (Opcode) IR_GEQ;
		break;
	    case Lcmp_COM_LT:
		mask = (Opcode) IR_LT;
		break;
	    // case Lcmp_COM_TZ:
		// mask = (Opcode) IR_TRUE;
		// break;
	    // case Lcmp_COM_TN:
		// mask = (Opcode) IR_FALSE;
		// break;
	    default:
		El_punt("El_opcode_completor_to_compare_condition: unknown signed completor value");
        }
      }
      else { /* Unsigned comparison */
        switch (L_get_compare_type(oper)) {
	    case Lcmp_COM_EQ:
		mask = (Opcode) IR_EQ;
		break;
	    case Lcmp_COM_NE:
		mask = (Opcode) IR_NEQ;
		break;
	    case Lcmp_COM_GT:
		mask = (Opcode) IR_LGT;
		break;
	    case Lcmp_COM_LE:
		mask = (Opcode) IR_LLEQ;
		break;
	    case Lcmp_COM_GE:
		mask = (Opcode) IR_LGEQ;
		break;
	    case Lcmp_COM_LT:
		mask = (Opcode) IR_LLT;
		break;
	    // case Lcmp_COM_TZ:
		// mask = (Opcode) IR_TRUE;
		// break;
	    // case Lcmp_COM_TN:
		// mask = (Opcode) IR_FALSE;
		// break;
	    default:
		El_punt("El_opcode_completor_to_compare_condition: unknown unsigned completor value");
        }
      }
    }
    else if(L_is_ctype_float_direct(L_get_compare_ctype(oper)) || 
	    L_is_ctype_double_direct(L_get_compare_ctype(oper))) {
        switch (L_get_compare_type(oper)) {
	    case Lcmp_COM_EQ:
		mask = (Opcode) IR_F_EQ;
		break;
	    case Lcmp_COM_NE:
		mask = (Opcode) IR_F_NEQ;
		break;
	    case Lcmp_COM_GT:
		mask = (Opcode) IR_F_GT;
		break;
	    case Lcmp_COM_LE:
		mask = (Opcode) IR_F_LEQ;
		break;
	    case Lcmp_COM_GE:
		mask = (Opcode) IR_F_GEQ;
		break;
	    case Lcmp_COM_LT:
		mask = (Opcode) IR_F_LT;
		break;
	    // case Lcmp_COM_TZ:
		// mask = (Opcode) IR_F_TRUE;
		// break;
	    // case Lcmp_COM_TN:
		// mask = (Opcode) IR_F_FALSE;
		// break;
	    default:
		El_punt("El_opcode_completor_to_compare_condition: unknown float/double completor value");
        }
	if(L_is_ctype_double_direct(L_get_compare_ctype(oper))) {
          Opcode temp = (Opcode)IR_DOUBLE;
	  mask = opcode_or(mask, temp);
	}

    }
    

    Opcode new_opc = opcode_or(opc, mask);
    return (new_opc);
}

/*
 *	Impact predicate modifiers are placed on the destination operands.  Elcor places these
 *	modifiers on the opcode.  This converts between the 2 styles.
 */
Opcode El_insert_pred_type_modifiers(Opcode& opc, L_Oper *oper)
{

    Opcode mask; 
    L_Operand *dest1, *dest2;
    Opcode full_opc = opc;

    if (! L_general_pred_comparison_opcode(oper))
	El_punt("El_insert_ptype_modifiers: op is not a pred comparison");

    // DEST1 modifier
    mask = (Opcode) 0;
    dest1 = oper->dest[0];
    if (dest1!=NULL) {
	switch (dest1->ptype) {
	    case L_PTYPE_UNCOND_T:
		mask = (Opcode) IR_DEST1_UN;
		break;
	    case L_PTYPE_UNCOND_F:
		mask = (Opcode) IR_DEST1_UC;
		break;
	    case L_PTYPE_COND_T:
		mask = (Opcode) IR_DEST1_CN;
		break;
	    case L_PTYPE_COND_F:
		mask = (Opcode) IR_DEST1_CC;
		break;
	    case L_PTYPE_OR_T:
		mask = (Opcode) IR_DEST1_ON;
		break;
	    case L_PTYPE_OR_F:
		mask = (Opcode) IR_DEST1_OC;
		break;
	    case L_PTYPE_AND_T:
		mask = (Opcode) IR_DEST1_AN;
		break;
	    case L_PTYPE_AND_F:
		mask = (Opcode) IR_DEST1_AC;
		break;
	    default:
		El_punt("El_insert_opcode_modifiers: illegal ptype for dest[0]");
	}
    }
    else {
	mask = (Opcode) IR_DEST1_UN;
    }

    full_opc = opcode_or(full_opc, mask);

    // DEST2 modifier
    mask = (Opcode) 0;
    dest2 = oper->dest[1];
    if (dest2!=NULL) {
	switch (dest2->ptype) {
	    case L_PTYPE_UNCOND_T:
		mask = (Opcode) IR_DEST2_UN;
		break;
	    case L_PTYPE_UNCOND_F:
		mask = (Opcode) IR_DEST2_UC;
		break;
	    case L_PTYPE_COND_T:
		mask = (Opcode) IR_DEST2_CN;
		break;
	    case L_PTYPE_COND_F:
		mask = (Opcode) IR_DEST2_CC;
		break;
	    case L_PTYPE_OR_T:
		mask = (Opcode) IR_DEST2_ON;
		break;
	    case L_PTYPE_OR_F:
		mask = (Opcode) IR_DEST2_OC;
		break;
	    case L_PTYPE_AND_T:
		mask = (Opcode) IR_DEST2_AN;
		break;
	    case L_PTYPE_AND_F:
		mask = (Opcode) IR_DEST2_AC;
		break;
	    default:
		El_punt("El_insert_opcode_modifiers: illegal ptype for dest[1]");
	}
    }
    else {
	mask = (Opcode) IR_DEST2_UN;
    }

    full_opc = opcode_or(full_opc, mask);
    return full_opc;
}

Op* El_lcode_to_elcor_op(L_Oper *oper)
{
    Opcode el_opcode = (Opcode)0;

    if (oper==NULL) {
       cerr << "ERROR: In El_lcode_to_elcor_op() oper is NULL pointer" << endl ;
        return 0;
    }

    if (L_uncond_branch_opcode(oper) && L_find_attr(oper->attr, "nonseq_db")) {
        return (El_lcode_to_elcor_nonseq_dummy_br(oper));
    }
  
    /* Mapping of pred_clears depends on dest operand */ 
    if (oper->proc_opc == Lop_PRED_CLEAR) {
	if (! L_is_macro(oper->dest[0]))
	    el_opcode = el_lcode_to_elcor_opcode_map.value(oper->proc_opc) ;
	else {
	    switch (oper->dest[0]->value.mac) {
		case L_MAC_PRED_ALL:
		    el_opcode = PRED_CLEAR_ALL;
		    break;
		case PLAYDOH_MAC_PRED_ALL_ROT:
		    el_opcode = PRED_CLEAR_ALL_ROTATING;
		    break;
		case PLAYDOH_MAC_PRED_ALL_STATIC:
		    el_opcode = PRED_CLEAR_ALL_STATIC;
		    break;
		default:
		    El_punt("El_lcode_to_elcor_op: unknow dest of pred_clear");
	    }
	}
	
    }
    else { 
        el_opcode = el_lcode_to_elcor_opcode_map.value(oper->proc_opc) ;
    }

    Opcode el_full_opcode = el_opcode;
    if (is_cmpp(el_opcode) || is_cmpr(el_opcode))
        el_full_opcode = El_insert_compare_cond_modifiers(el_full_opcode, oper);
    if (is_cmpp(el_opcode))
        el_full_opcode = El_insert_pred_type_modifiers(el_full_opcode, oper);

    Op* new_op = new Op(el_full_opcode, oper->id); // preserve the Lcode id
    El_lcode_to_elcor_rest_op(new_op, oper);
    return new_op ;
}


/*============================================================
 *
 *  CB conversion
 *
 *============================================================*/

void El_fixup_scheduled_region(Compound_region *cr)
{
    Op *op, *ref_op = NULL;
    int stime;
    Opcode opc;
    Dlist<Op*> to_delete;
    bool found;

    assert (cr->is_hb() || cr->is_bb());

    cr->set_flag(EL_REGION_SCHEDULED);

    for (Region_ops_C0_order op_i(cr); op_i!=0; op_i++) {
	op = *op_i;
	opc = op->opcode();

	/* For Pseudo ops, elcor treats them as scheduled ops, Impact does
	   not, so give them scheduler info if needed */
	if ((opc == C_MERGE) || (opc == DUMMY_BR) || (opc == DEFINE) ||
		(opc == PRED_CLEAR) || (opc == PRED_SET)) {

	    /* First look upwards for a neighboring op */
	    found = false;
	    for (Region_ops_reverse_C0_order op_i2(cr, op); op_i2!=0; op_i2++) {
		ref_op = *op_i2;
		if (ref_op->flag(EL_REGION_SCHEDULED)) {
		    found = true;
		    break;
	        }
	    }

	    /* If the previous search failed, search downward */
	    if (! found) {
		for (Region_ops_C0_order op_i3(cr, op); op_i3!=0; op_i3++) {
		    ref_op = *op_i3;
		    if (ref_op->flag(EL_REGION_SCHEDULED)) {
			found = true;
			break;
		    }
		}
	    }

	    /* If no other ops, then just assume a sched time of 0 */
	    if (! found)
		stime = 0;
	    else
	        stime = ref_op->sched_time();
	    assert (stime >= 0);

	    op->set_sched_time(stime);
	    op->set_flag(EL_REGION_SCHEDULED);

	    /* CM and DB don't exist in Impact, so also need to give them a
		sched opcode */
	    if ((opc == C_MERGE) || (opc == DUMMY_BR)) {
		List_set<Op_descr*> op_desc_set;
		Op_descr *op_desc;
		//MDES_collect_op_descrs(el_opcode_to_string_map.value(op->opcode()), op_desc_set);
		//tangw (06-03-02)
		MDES_collect_op_descrs(get_mdes_opcode_string(op), op_desc_set);


		assert(! op_desc_set.is_empty());
		op_desc = op_desc_set.head();
		op->set_sched_opcode(op_desc->get_aname_ptr());
	    }
	}

	/* Elcor scheduled code is no-op free, whereas Impact is not */
	else if (opc == NO_OP) {
	   to_delete.push_tail(op);
	}
    }

    for (Dlist_iterator<Op*> dl_i(to_delete); dl_i!=0; dl_i++) {
	op = *dl_i;
	El_remove_op(op);
	delete(op);
    }
}

Compound_region *El_lcode_to_elcor_cb(L_Cb *cb, bool is_scheduled)
{
   Compound_region *el_cb ;

   bool region_is_hyperblock = false ;
   
   L_Oper *oper;
   L_Attr *attr;

   /* first determine if the region is a HB or BB, that is all that
	is currently supported */
   if (L_EXTRACT_BIT_VAL(cb->flags, L_CB_HYPERBLOCK|L_CB_SUPERBLOCK)) {
      region_is_hyperblock = true;
   }
   { // patch for mislabeled superblocks
      int num_branch = 0;
      L_Oper* cur_oper = cb->first_op ;
      L_Oper* last_oper = cb->last_op ;
      while(cur_oper != last_oper) {
	 if (L_general_branch_opcode(cur_oper)) num_branch++ ;
	 cur_oper = cur_oper->next_op ;
      }
      if (num_branch >= 1) {
	 region_is_hyperblock = true;
      }
   }
   if (region_is_hyperblock) {
     el_cb = new Hyperblock(cb->id, cb->weight) ;
   }
   else {
      el_cb = new Basicblock(cb->id, cb->weight) ;
   }

   /* now copy over the Lcode flags to Elcor */
   El_lcode_to_elcor_cb_flags(cb, el_cb);
   El_lcode_cb_attr_to_elcor_cb_flag(cb, el_cb);

   /*
    *  Print attributes.
    */

   for (attr=cb->attr; attr!=NULL; attr=attr->next_attr) {
      El_lcode_to_elcor_attr(el_cb, attr);
   }

   /*
    *  Print all opers.
    */
   Op* prev_op = new Op(C_MERGE);
   el_cb->add_region(prev_op) ;
   prev_op->set_parent(el_cb) ;
   el_cb->add_entry(prev_op) ;

   for (oper=cb->first_op; oper!=NULL; oper=oper->next_op)
   {
      Op* tmp_op = El_lcode_to_elcor_op(oper);
      op_map.bind(oper,tmp_op) ;
      // Set the parent child pointers 
      el_cb->add_region(tmp_op) ;
      tmp_op->set_parent(el_cb) ;
      // Insert a control flow edge between the ops
      C0_connect_fallthrough(prev_op, tmp_op) ;
      prev_op = tmp_op ;

      // If this is a region exit, set it correctly
      // Also add another control merge
      if (is_branch(prev_op)) {
	 el_cb->add_exit(prev_op) ;
	 if (oper!=cb->last_op) {
	     tmp_op = new Op(C_MERGE);
	     el_cb->add_region(tmp_op) ;
	     tmp_op->set_parent(el_cb) ;
	     C0_connect_fallthrough(prev_op, tmp_op) ;
	     prev_op = tmp_op ;
	 }
      }
   }
   // insert a DUMMY_BR if necessary
   if(!is_branch(prev_op)) {
      Op* dum_br = new Op(DUMMY_BR);
      
      el_cb->add_region(dum_br) ;
      dum_br->set_parent(el_cb) ;
// Changed prev_op to dum_br in the next line - SGA
      el_cb->add_exit(dum_br) ;
      C0_connect_fallthrough(prev_op, dum_br) ;
   }
   // insert both a C_MERGE and a DUMMY_BR if necessary
   else if (el_cb->flag(EL_REGION_HAS_EXPLICIT_FT)) {
      Op *cm_op = new Op(C_MERGE);
      el_cb->add_region(cm_op);
      cm_op->set_parent(el_cb);
      C0_connect_fallthrough(prev_op, cm_op);

      Op* dum_br = new Op(DUMMY_BR);
      el_cb->add_region(dum_br) ;
      dum_br->set_parent(el_cb) ;
      el_cb->add_exit(dum_br) ;
      C0_connect_fallthrough(cm_op, dum_br) ;
   }

   if (is_scheduled == true)
       El_fixup_scheduled_region(el_cb);

   return (el_cb) ;
}

/*============================================================
 *
 *  Function conversion
 *
 *============================================================*/

Procedure *El_lcode_to_elcor_func (L_Func *fn)
{
   L_Attr *attr, *next_attr, *dep_pragmas_attr, *jsr_dep_pragmas_attr;
   L_Cb *cb;
   Procedure *R;

   if (El_do_run_time)
      El_clock2.start();

   /*
    * It is possible that the function has been converted from a
    * leaf to a non-leaf function.  Thus, we need to update the
    * function flag.
    */
   L_mark_leaf_func(fn);

   /* Check for sync arcs in the Lcode and verify memvr setup parameters */
   dep_pragmas_attr = L_find_attr(fn->attr, "DEP_PRAGMAS");
   if (dep_pragmas_attr == NULL)
       dep_pragmas_attr = L_find_attr(fn->attr, "DEP_PRAGMAS_LOCAL");
   jsr_dep_pragmas_attr = L_find_attr(fn->attr, "JSR_DEP_PRAGMAS");
   if (jsr_dep_pragmas_attr == NULL)
       jsr_dep_pragmas_attr = L_find_attr(fn->attr, "JSR_DEP_PRAGMAS_LOCAL");

   /* Automatic setup */
   if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_AUTOMATIC) {
      if ((dep_pragmas_attr!=NULL) && (jsr_dep_pragmas_attr!=NULL))
         El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_FULL_LCODE_SYNC_ARCS;
      else if (dep_pragmas_attr!=NULL)
         El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_NO_JSR_LCODE_SYNC_ARCS;
      else
         El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_SEQUENTIAL;
   }
   /* Rest are user specified setup */
   else if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_FULL_LCODE_SYNC_ARCS) {
      if (dep_pragmas_attr==NULL)
         El_warn("El_lcode_to_elcor_func: expecting mem/mem sync arcs but not there!");
      if (jsr_dep_pragmas_attr==NULL) 
         El_warn("El_lcode_to_elcor_func: expecting jsr sync arcs but not there!");
   }
   else if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_NO_JSR_LCODE_SYNC_ARCS) {
      if (dep_pragmas_attr==NULL)
         El_warn("El_lcode_to_elcor_func: expecting mem/mem Lcode sync arcs but not there!");
      if (jsr_dep_pragmas_attr!=NULL)
         El_warn("El_lcode_to_elcor_func: Throwing away Lcode jsr sync arc info.");
   }
   else if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_SEQUENTIAL) {
      if (dep_pragmas_attr!=NULL)
         El_warn("El_lcode_to_elcor_func: Throwing away Lcode mem/mem sync arc info.");
      if (jsr_dep_pragmas_attr!=NULL)
         El_warn("El_lcode_to_elcor_func: Throwing away Lcode jsr sync arc info.");
   }
   else if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_NONE) {
      if (dep_pragmas_attr!=NULL)
         El_warn("El_lcode_to_elcor_func: Throwing away Lcode mem/mem sync arc info.");
      if (jsr_dep_pragmas_attr!=NULL)
         El_warn("El_lcode_to_elcor_func: Throwing away Lcode jsr sync arc info.");
   }
   else {
      El_punt("El_lcode_to_elcor_func: unknown memvr setup mode!");
   }

   /*
    *  Print function header
    */
   R = new Procedure(fn->name, fn->weight);
   R->set_max_op_id(fn->max_oper_id+1);

   /*
    *  Print flags - flags are optional
    *
    *  Remember to clear out any flags that should not print!
    */
   El_lcode_to_elcor_func_flags(fn, R);
   El_lcode_func_attr_to_elcor_func_flag(fn, R);

   scalar_regalloc_done = false;
   if (L_EXTRACT_BIT_VAL(fn->flags, L_FUNC_REGISTER_ALLOCATED))
      scalar_regalloc_done = true;

   if(El_do_read_impact_widths) 
     bitwidth_map.clear();

   /* Print function attributes. */

   /*
    * First we delete any existing attribute for input and
    * output file name.
    */
   for (attr=fn->attr; attr!=NULL; attr=next_attr)
   {
      next_attr = attr->next_attr;
      if (!strncmp(attr->name, "INFILE:", 7))
	 fn->attr = L_delete_attr(fn->attr, attr);

      else if (!strncmp(attr->name, "OUTFILE:", 8))
	 fn->attr = L_delete_attr(fn->attr, attr);
      else if (!strcmp(attr->name, "DEP_PRAGMAS"))
	 fn->attr = L_delete_attr(fn->attr, attr);
      else if (!strcmp(attr->name, "DEP_PRAGMAS_LOCAL"))
	 fn->attr = L_delete_attr(fn->attr, attr);
      else if (!strcmp(attr->name, "JSR_DEP_PRAGMAS"))
	 fn->attr = L_delete_attr(fn->attr, attr);
      else if (!strcmp(attr->name, "JSR_DEP_PRAGMAS_LOCAL"))
	 fn->attr = L_delete_attr(fn->attr, attr);
   }
   /*
    * Create attributes for input and output file name.
    */

   for (attr=fn->attr; attr!=NULL; attr=attr->next_attr)
   {
      El_lcode_to_elcor_attr(R, attr);
   }

   /*
    *  Create map for bitwidth attributes, pass all the way down to register level
    *  Expects function level bitwidth attribute of the form:
    *                    (register)(str:var name)(bitwidth)
    *  Variable name currently unused
    */

   if(El_do_read_impact_widths) {

     Lcode_attribute_map* map_ptr;
     
     map_ptr = get_lcode_attributes(R);
     if(map_ptr->is_bound("bitwidth")) {
       
       List<Operand> attr_list = map_ptr->value("bitwidth");

       for(List_iterator<Operand> li(attr_list); li!=0; li++) {
         Operand reg_oper = *li;
         
         if(!reg_oper.is_reg())
           El_punt("El_lcode_to_elcor_func: Expecting register operand in bitwidth attribute");

         li++;

         Operand bitwidth_oper = *li;
         if(!bitwidth_oper.is_int())
           El_punt("El_lcode_to_elcor_func: Expecting int operand (bitwidth) in bitwidth attribute");       
       
         bitwidth_map.bind(reg_oper.vr_num(), bitwidth_oper.int_value());
       }
       map_ptr->unbind("bitwidth");
     }
   }

   /*
    *  Print all cbs
    */

   Map<L_Cb*, Region*> cb_map ;
   Compound_region *last_cb;
   Compound_region *new_cb = El_lcode_to_elcor_cb(fn->first_cb,
						R->flag(EL_PROC_SCHEDULED));
   R->add_region(new_cb) ;
   new_cb->set_parent(R) ;
   cb_map.bind(fn->first_cb,new_cb) ;
   last_cb = new_cb;
   for (cb=fn->first_cb->next_cb; cb!=NULL; cb=cb->next_cb) {
      new_cb = El_lcode_to_elcor_cb(cb, R->flag(EL_PROC_SCHEDULED));
      R->insert_after_region(new_cb, last_cb) ;
      new_cb->set_parent(R) ;
      cb_map.bind(cb,new_cb) ;
      last_cb = new_cb;
   }


// Hook-up the control flow edges
// src_cb :  pointer to the control-flow source cb
// dest_cb:  pointer to the control-flow destination cb
// branch_list : list of branches in source_cb (This is guaranteed to contain
//               at least one op, e.g. a DUMMY_BR operation)
//   
// The inner loop matches the flow edges from Impact with the ops in
// branch_list. In the case the branch_list has more than one operation, the
// head of the list is popped after every edge hookup   
   
   for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
      double cur_weight;
      Compound_region* src_cb = (Compound_region*) cb_map.value(cb) ;
      List<Op*> branch_list ;
      for (Region_subregions op_iter(src_cb) ; op_iter != 0 ; op_iter++) {
	Op* cur_op = (Op *) *op_iter ;
	if(is_branch(cur_op)) branch_list.add_tail(cur_op) ;
      }
      cur_weight = cb->weight;
      for (L_Flow* flow=cb->dest_flow; flow!=NULL; flow=flow->next_flow) {
	 Control_flow_freq *cfreq;
	 Edge *c_edge;
	 Compound_region* dest_cb = (Compound_region*) cb_map.value(flow->dst_cb) ;
	 if ((flow->next_flow==NULL) && (flow->dst_cb==cb->next_cb)) {
	     c_edge = C0_connect_fallthrough((Op*) (branch_list.head()),
				   get_first_region_op_from_subregions(dest_cb));  
	 }
         else if (((Op *)branch_list.head())->flag(EL_OPER_NONSEQ_DUMMY_BR)) {
             c_edge = C0_connect_fallthrough((Op*) (branch_list.head()),
                                   get_first_region_op_from_subregions(dest_cb));
         }
	 else {
	     c_edge = C0_connect((Op*) (branch_list.head()),
				   get_first_region_op_from_subregions(dest_cb));  
	 }
	 src_cb->add_exit_safely(c_edge) ;
	 dest_cb->add_entry_safely(c_edge) ;
	 cfreq = new Control_flow_freq();
	 cfreq->freq = flow->weight;
	 cfreq->cc = flow->cc;
	 set_control_flow_freq(c_edge, cfreq);

         /* Insert a cfreq on internal fallthru edge */
         cur_weight -= flow->weight;
	 if (cur_weight < 0.0)
	     cur_weight = 0.0;
	 if (branch_list.size() > 1) {
	     c_edge = get_outgoing_CONTROL0_fall_through_edge(branch_list.head());
	     if (c_edge==NULL)
		El_punt("El_lcode_to_elcor_func: missing internal ft edge!");
	     cfreq = new Control_flow_freq();
	     cfreq->freq = cur_weight;
	     cfreq->cc = 0;
	     set_control_flow_freq(c_edge, cfreq);
         }

	 if (branch_list.size() > 1) branch_list.pop() ;
	 
      }
   }

   Op* function_entry_op = get_first_region_op_from_subregions(R) ;
   Op* function_exit_op = 0 ;
   Region_all_ops ri ;
   for (ri(R); ri != 0 ; ri++) {
      Op* cur_op = *ri ;
      Opcode cur_opc = cur_op->opcode() ;
      if (cur_opc == RTS) {
	 function_exit_op = *ri ;
      }
      if (cur_opc == C_MERGE) {
	 int inedge_count = 0 ;
	 for (Op_inedges_rel ei(cur_op) ; ei != 0 ; ei++) {
	    inedge_count++ ;
	 }
	 if (inedge_count > 1) cur_op->set_flag(EL_OPER_REAL_MERGE) ;
      }
   }
   R->add_entry(function_entry_op) ;
   R->add_exit(function_exit_op) ;

   /* Adjust the register names if allocation is done */
   if (R->flag(EL_PROC_REGISTER_ALLOCATED) || R->flag(EL_PROC_ROT_REG_ALLOCATED))
     El_punt("Sorry, translation of lcode to elcor with allocated registers is unimplemented.");
     // This doesn't work and hasn't worked for 6 years.  -KF 1/2007
     //El_rename_regs_for_elcor(R);

//
// Setup the jump tables (SAM 7-96)
//

   if (fn->jump_tbls != NULL) {
       El_lcode_to_elcor_jumptbl_info(R, fn->jump_tbls);
   }

// 
// Set the sync vars corectly
//

   int memvr_counter = 0 ;

   if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_NONE) {
      El_warn("No memory dependence info is being generated");
   }
   else {
      for (ri(R); ri != 0 ; ri++) {
         Op* cur_op = *ri ;
         Opcode cur_opc = cur_op->opcode() ;
         if (is_memory(cur_opc) || is_brl(cur_opc) || is_rts(cur_opc)) {
	    Operand new_syncvar = new Mem_vr(memvr_counter) ;
	    memvr_counter++ ;
	    cur_op->add_mem_dest(new_syncvar) ;
         }
      }
      if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_SEQUENTIAL) {
         if (dbg(status, 2))
            cout << "Sequential memvr setup" << endl;
         el_lcode_reader_fix_memvar_uses(R) ;
      }
      else if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_NO_JSR_LCODE_SYNC_ARCS) {
         if (dbg(status, 2))
            cout << "No Jsr sync arc memvr setup" << endl;
         El_warn("Only partial sync arc info available, may lead to overly aggressive memory dependences");
         for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
	    L_Oper* oper ;
	    for (oper=cb->first_op; oper!=NULL; oper=oper->next_op) {
	       if ( (oper->sync_info != NULL) &&
		    (oper->sync_info->num_sync_in > 0) ) {
	          for (int i = 0; i < oper->sync_info->num_sync_in; i++) {
		     Op* el_op = op_map.value(oper) ;
		     El_lcode_to_elcor_sync(el_op, oper->sync_info->sync_in[i]);
	          }
	       }
	    }
         }
      }
      else if (El_memvr_setup_mode == EL_MEMVR_SETUP_MODE_FULL_LCODE_SYNC_ARCS) {
         if (dbg(status, 2))
            cout << "Full sync arc memvr setup" << endl;
         // for (cb=fn->first_cb->next_cb; cb!=NULL; cb=cb->next_cb) {
         for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
	    L_Oper* oper ;
	    for (oper=cb->first_op; oper!=NULL; oper=oper->next_op) {
	       if ( (oper->sync_info != NULL) &&
		    (oper->sync_info->num_sync_in > 0) ) {
	          for (int i = 0; i < oper->sync_info->num_sync_in; i++) {
		     Op* el_op = op_map.value(oper) ;
		     El_lcode_to_elcor_sync(el_op, oper->sync_info->sync_in[i]);
	          }
	       }
	    }
         }
      }
      else {
         El_punt("Unknown memory dependence specifier") ;
      }

      // If El_memvr_profiled is on, refine memory dependency sync vars further
      // using memory profiling information
      if (El_memvr_profiled ) {
         MprofSvars mpsv(R);
         mpsv.read_prof_mem_and_analyze();
         if (El_memvr_profiled) mpsv.set_memvrs();
      }

      if (!El_memvr_profiled && El_cache_load_sched != NONE) {
         MprofSvars mpsv(R);
         mpsv.read_prof_mem_and_analyze();
      }

   }

   // Annotate with the AccSpec
   for (cb=fn->first_cb; cb!=NULL; cb=cb->next_cb) {
     L_Oper* oper ;
     for (oper=cb->first_op; oper!=NULL; oper=oper->next_op) {
       if (oper->acc_info != NULL) {
         Op* el_op = op_map.value(oper) ;
         El_lcode_to_elcor_accspec(el_op, oper->acc_info);
       }
     }
   }

   if (El_do_run_time) {
      El_clock2.stop();
      El_stopwatch_print_time("El_lcode_to_elcor_func", El_clock2.last_elapsed());
   }

   return (R);
}


/*============================================================================*/
/*
 *	Conversion of Data
 */
/*============================================================================*/




/*
 *      L_Expr
 */


El_expression* El_lcode_to_elcor_expr(L_Expr *expr)
{
   if (expr==0)
      return (0);
   switch (expr->type) {
    case L_EXPR_INT:
    {
       El_value_expression* el_expr = new El_value_expression() ;
       el_expr->type = EL_EXPRESSION_INT_VALUE ;
       el_expr->value.i = expr->value.i ;
       return(el_expr) ;
    }
    break;
    case L_EXPR_FLOAT:
    {
       El_value_expression* el_expr = new El_value_expression() ;
       el_expr->type = EL_EXPRESSION_FLOAT_VALUE ;
       el_expr->value.f = expr->value.f ;
       return(el_expr) ;
    }
    break;
    case L_EXPR_DOUBLE:
    {
       El_value_expression* el_expr = new El_value_expression() ;
       el_expr->type = EL_EXPRESSION_DOUBLE_VALUE ;
       el_expr->value.f2 = expr->value.f2 ;
       return(el_expr) ;
    }
    break;
    case L_EXPR_LABEL:
    {
       El_symbol_expression* el_expr = new El_symbol_expression(expr->value.l) ;
       return(el_expr) ;
    }
    break;
    case L_EXPR_STRING:
    {
       El_string_expression* el_expr = new El_string_expression(expr->value.s) ;
       return(el_expr) ;
    }
    break;
    case L_EXPR_ADD:
    {
       El_compound_expression* el_c_expr = new El_compound_expression(3) ;
       el_c_expr->args[0] = new El_op_expression(ELEX_ADD) ;
       el_c_expr->args[1] = El_lcode_to_elcor_expr(expr->A);
       el_c_expr->args[2] = El_lcode_to_elcor_expr(expr->B);      
       return(el_c_expr) ;
    }
    break;
    case L_EXPR_SUB:
    {
       El_compound_expression* el_c_expr = new El_compound_expression(3) ;
       el_c_expr->args[0] = new El_op_expression(ELEX_SUB) ;
       el_c_expr->args[1] = El_lcode_to_elcor_expr(expr->A);
       el_c_expr->args[2] = El_lcode_to_elcor_expr(expr->B);      
       return(el_c_expr) ;
    }
    break;
    case L_EXPR_MUL:
    {
       El_compound_expression* el_c_expr = new El_compound_expression(3) ;
       el_c_expr->args[0] = new El_op_expression(ELEX_MUL) ;
       el_c_expr->args[1] = El_lcode_to_elcor_expr(expr->A);
       el_c_expr->args[2] = El_lcode_to_elcor_expr(expr->B);      
       return(el_c_expr) ;
    }
    break;
    case L_EXPR_DIV:
    {
       El_compound_expression* el_c_expr = new El_compound_expression(3) ;
       el_c_expr->args[0] = new El_op_expression(ELEX_DIV) ;
       el_c_expr->args[1] = El_lcode_to_elcor_expr(expr->A);
       el_c_expr->args[2] = El_lcode_to_elcor_expr(expr->B);      
       return(el_c_expr) ;
    }
    break;
    case L_EXPR_NEG:
    {
       El_compound_expression* el_c_expr = new El_compound_expression(2) ;
       el_c_expr->args[0] = new El_op_expression(ELEX_NEG) ;
       el_c_expr->args[1] = El_lcode_to_elcor_expr(expr->A);
       return(el_c_expr) ;
    }
    break;
    case L_EXPR_COM:
    {
       El_compound_expression* el_c_expr = new El_compound_expression(2) ;
       el_c_expr->args[0] = new El_op_expression(ELEX_COM) ;
       el_c_expr->args[1] = El_lcode_to_elcor_expr(expr->A);
       return(el_c_expr) ;
    }
    break;
    default:
      El_punt("El_lcode_to_elcor_expr: illegal argument type");
   }
   return (0) ;
}

El_expression* El_lcode_to_elcor_expr_plus_const(L_Expr *expr, int c)
{
  El_value_expression* const_expr = new El_value_expression() ;
  const_expr->type = EL_EXPRESSION_INT_VALUE ;
  const_expr->value.i = c;

  El_expression *el_expr = El_lcode_to_elcor_expr(expr);

  El_compound_expression* el_c_expr = new El_compound_expression(3) ;
  el_c_expr->args[0] = new El_op_expression(ELEX_ADD) ;
  el_c_expr->args[1] = el_expr;
  el_c_expr->args[2] = const_expr;
  return(el_c_expr) ;
}

/*
 *      L_Data
 */

El_data_memory_segment_type El_lcode_to_elcor_ms_type(int lcode_ms_type)
{
    switch (lcode_ms_type) {
	case L_MS_TEXT:
	    return (EL_DATA_MS_TEXT);
	case L_MS_DATA:
	    return (EL_DATA_MS_DATA);
	case L_MS_DATA1:
	    return (EL_DATA_MS_DATA1);
	case L_MS_DATA2:
	    return (EL_DATA_MS_DATA2);
	case L_MS_BSS:
	    return (EL_DATA_MS_BSS);
	case L_MS_SYNC:
	    return (EL_DATA_MS_SYNC);
	default:
	    El_punt("El_lcode_to_elcor_ms_type: unknown lcode ms type: %d",
			lcode_ms_type);
	    return (EL_DATA_MS_UNKNOWN);
    }
}

void El_lcode_to_elcor_stype_flags(int lcode_stype_flags,
				    El_data_source_type_info *stype)
{
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_CONST))
        stype->flags.set_bit(EL_DATA_ST_CONST);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_VOLATILE))
        stype->flags.set_bit(EL_DATA_ST_VOLATILE);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_NOALIAS))
        stype->flags.set_bit(EL_DATA_ST_NOALIAS);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_REGISTER))
        stype->flags.set_bit(EL_DATA_ST_REGISTER);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_AUTO))
        stype->flags.set_bit(EL_DATA_ST_AUTO);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_STATIC))
        stype->flags.set_bit(EL_DATA_ST_STATIC);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_EXTERN))
        stype->flags.set_bit(EL_DATA_ST_EXTERN);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_GLOBAL))
        stype->flags.set_bit(EL_DATA_ST_GLOBAL);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_PARAMETER))
        stype->flags.set_bit(EL_DATA_ST_PARAMETER);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_VOID))
        stype->flags.set_bit(EL_DATA_ST_VOID);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_CHAR))
        stype->flags.set_bit(EL_DATA_ST_CHAR);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_SHORT))
        stype->flags.set_bit(EL_DATA_ST_SHORT);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_INT))
        stype->flags.set_bit(EL_DATA_ST_INT);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_LONG))
        stype->flags.set_bit(EL_DATA_ST_LONG);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_LONGLONG))
        stype->flags.set_bit(EL_DATA_ST_LLONG);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_FLOAT))
        stype->flags.set_bit(EL_DATA_ST_FLOAT);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_DOUBLE))
        stype->flags.set_bit(EL_DATA_ST_DOUBLE);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_SIGNED))
        stype->flags.set_bit(EL_DATA_ST_SIGNED);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_UNSIGNED))
        stype->flags.set_bit(EL_DATA_ST_UNSIGNED);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_STRUCT))
        stype->flags.set_bit(EL_DATA_ST_STRUCT);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_UNION))
        stype->flags.set_bit(EL_DATA_ST_UNION);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_ENUM))
        stype->flags.set_bit(EL_DATA_ST_ENUM);
    if (L_EXTRACT_BIT_VAL(lcode_stype_flags, L_DATA_VARARG))
        stype->flags.set_bit(EL_DATA_ST_VARARG);
}

El_data_access_method El_lcode_to_elcor_access_meth(int lcode_access_meth)
{
    switch (lcode_access_meth) {
	case L_D_ARRY:
		return (EL_DATA_ACCESS_ARRAY);
	case L_D_PTR:
		return (EL_DATA_ACCESS_POINTER);
	case L_D_FUNC:
		return (EL_DATA_ACCESS_FUNCTION);
	default:
		El_punt("El_lcode_to_elcor_access_meth: unknown Lcode access meth: %d",
				lcode_access_meth);
		return (EL_DATA_ACCESS_UNKNOWN);
    }
}

El_data_source_type_info *El_lcode_to_elcor_src_type(L_Type *lcode_h_type)
{
    El_data_source_type_info *el_s_type;
    L_Dcltr *ptr;
    El_data_access_declarator *el_dcltr;

    if (lcode_h_type == NULL)
        return (NULL);

    el_s_type = new El_data_source_type_info();
    El_lcode_to_elcor_stype_flags(lcode_h_type->type, el_s_type);
    if (lcode_h_type->struct_name != NULL)
        el_s_type->struct_name = lcode_h_type->struct_name;
    for (ptr=lcode_h_type->dcltr; ptr!=NULL; ptr=ptr->next) {
       el_dcltr = new El_data_access_declarator();
       el_dcltr->method = El_lcode_to_elcor_access_meth(ptr->method);
       if (ptr->index != NULL)
           el_dcltr->index = El_lcode_to_elcor_expr(ptr->index);
       el_s_type->access_dcltrs.add_tail(el_dcltr);
    }

    return (el_s_type);
}

El_data *El_lcode_to_elcor_data(L_Data *data)
{
   L_Expr *addr, *val;
   if (data==NULL)
      El_punt("El_lcode_to_elcor_data: no argument");

   /*
    *  Insert comments into output file
    */

   El_data *el_data = new El_data ;
    
   switch (data->type) {
    case L_INPUT_MS:
      el_data->t_type = EL_DATA_TOKEN_MS ;
      el_data->ms_type = El_lcode_to_elcor_ms_type(data->N) ;
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break;
    case L_INPUT_GLOBAL:
      el_data->t_type = EL_DATA_TOKEN_GLOBAL ;
      if (data->address==NULL)
	 El_punt("El_lcode_to_elcor_data: bad address");
      el_data->address = El_lcode_to_elcor_expr(data->address) ;
      el_data->src_type = El_lcode_to_elcor_src_type(data->h_type);
      break;
    case L_INPUT_VOID:
    case L_INPUT_BYTE:
    case L_INPUT_WORD:
    case L_INPUT_LONG:
    case L_INPUT_LONGLONG:
    case L_INPUT_FLOAT:
    case L_INPUT_DOUBLE:
    case L_INPUT_ALIGN:
    case L_INPUT_ELEMENT_SIZE:
      switch (data->type) {
       case L_INPUT_VOID:
         el_data->t_type = EL_DATA_TOKEN_VOID ;
	 break;
       case L_INPUT_BYTE:
         el_data->t_type = EL_DATA_TOKEN_BYTE ;
	 break;
       case L_INPUT_WORD:
         el_data->t_type = EL_DATA_TOKEN_WORD ;
	 break;
       case L_INPUT_LONG:
	 el_data->t_type = EL_DATA_TOKEN_LONG ;
	 break;
       case L_INPUT_LONGLONG:
	 el_data->t_type = EL_DATA_TOKEN_LLONG ;
	 break;
       case L_INPUT_FLOAT:
	 el_data->t_type = EL_DATA_TOKEN_FLOAT ;
	 break;
       case L_INPUT_DOUBLE:
         el_data->t_type = EL_DATA_TOKEN_DOUBLE ;
	 break;
       case L_INPUT_ALIGN:
	 el_data->t_type = EL_DATA_TOKEN_ALIGN ;
	 break;
       case L_INPUT_ELEMENT_SIZE:
	 el_data->t_type = EL_DATA_TOKEN_ELEMENT_SIZE;
	 break;
       default:
	 El_punt("El_lcode_to_elcor_data: extremely illegal data type");	 
      }

      el_data->N = data->N ;
      if (data->address==NULL)
         El_punt("El_lcode_to_elcor_data: bad address");
      el_data->address = El_lcode_to_elcor_expr(data->address) ;
      if (data->value != NULL)
         el_data->value = El_lcode_to_elcor_expr(data->value) ;
      if ((data->value != NULL) && (data->value->next_expr != NULL))
         El_punt("El_lcode_to_elcor_data: lcode data has a list of values!!!");
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break;

    case L_INPUT_ASCII:
      el_data->t_type = EL_DATA_TOKEN_ASCII ;
      addr = data->address;
      val = data->value;
      if ((addr==NULL)||(val==NULL)) {
	 El_punt("El_lcode_to_elcor_data: bad pointer");
      }
      el_data->address = El_lcode_to_elcor_expr(data->address) ;
      el_data->value = El_lcode_to_elcor_expr(val) ;
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break ;
    case L_INPUT_ASCIZ:
      el_data->t_type = EL_DATA_TOKEN_ASCIZ ;
      addr = data->address;
      val = data->value;
      if ((addr==NULL)||(val==NULL))
	 El_punt("El_lcode_to_elcor_data: bad pointer");
      el_data->address = El_lcode_to_elcor_expr(data->address) ;
      el_data->value = El_lcode_to_elcor_expr(val) ;
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break;
    case L_INPUT_RESERVE:
      el_data->t_type = EL_DATA_TOKEN_RESERVE ;
      el_data->N = data->N ;
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break;
    case L_INPUT_WB:
    case L_INPUT_WW:
    case L_INPUT_WI:
    case L_INPUT_WQ:
    case L_INPUT_WF:
    case L_INPUT_WF2:
    case L_INPUT_WS:
      switch (data->type) {
       case L_INPUT_WB:
	 el_data->t_type = EL_DATA_TOKEN_WB ;
	 break;
       case L_INPUT_WW:
	 el_data->t_type = EL_DATA_TOKEN_WW ;
	 break;
       case L_INPUT_WI:
	 el_data->t_type = EL_DATA_TOKEN_WI ;
	 break;
       case L_INPUT_WQ:
	 el_data->t_type = EL_DATA_TOKEN_WQ ;
	 break;
       case L_INPUT_WF:
	 el_data->t_type = EL_DATA_TOKEN_WF ;
	 break;
       case L_INPUT_WF2:
	 el_data->t_type = EL_DATA_TOKEN_WF2 ;
	 break;
       case L_INPUT_WS:
	 el_data->t_type = EL_DATA_TOKEN_WS ;
	 break;
       default:
	 El_punt("El_lcode_to_elcor_data: extremely illegal data type");
      }
      addr = data->address;
      val = data->value;
      if ((addr==NULL)||(val==NULL))
         El_punt("El_lcode_to_elcor_data: bad pointer");
      el_data->address = El_lcode_to_elcor_expr(data->address) ;
      el_data->value = El_lcode_to_elcor_expr(val) ;
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break;
    case L_INPUT_DEF_STRUCT:
    case L_INPUT_DEF_UNION:
    case L_INPUT_DEF_ENUM:
      if (data->type == L_INPUT_DEF_STRUCT)
        el_data->t_type = EL_DATA_TOKEN_DEF_STRUCT;
      else if (data->type == L_INPUT_DEF_UNION)
        el_data->t_type = EL_DATA_TOKEN_DEF_UNION;
      else
        el_data->t_type = EL_DATA_TOKEN_DEF_ENUM;
      if (data->address == NULL)
        El_punt("El_lcode_to_elcor_data: lcode data has NULL address");
      el_data->address = El_lcode_to_elcor_expr(data->address);
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break;
    case L_INPUT_FIELD:
      el_data->t_type = EL_DATA_TOKEN_FIELD;
      if (data->address == NULL)
        El_punt("El_lcode_to_elcor_data: lcode data has NULL address");
      el_data->address = El_lcode_to_elcor_expr(data->address);
      el_data->value = El_lcode_to_elcor_expr(data->value) ;
      el_data->src_type = El_lcode_to_elcor_src_type(data->h_type);
      break;
    case L_INPUT_ENUMERATOR:
      el_data->t_type = EL_DATA_TOKEN_ENUMERATOR;
      if (data->address == NULL)
        El_punt("El_lcode_to_elcor_data: lcode data has NULL address");
      el_data->address = El_lcode_to_elcor_expr(data->address);
      el_data->value = El_lcode_to_elcor_expr(data->value) ;
      if (data->h_type != NULL)
         El_punt("El_lcode_to_elcor_data: lcode data has unexpected h_type!");
      break;
    default:
      El_punt("El_lcode_to_elcor_data: illegal data type");
   }

   if (data->id)
     el_data->id = data->id;

   return (el_data);
}

/*
 *      L_Datalist
 */

El_datalist *El_lcode_to_elcor_datalist(L_Datalist *datalist)
{
    L_Datalist_Element *element;
    El_datalist *el_datalist;
    El_data *el_data;

    if (datalist == NULL)
	return (NULL);

    el_datalist = new El_datalist;

    for (element=datalist->first_element; element!=NULL;
	 element=element->next_element) {
      el_data = El_lcode_to_elcor_data(element->data);
      el_datalist->elements.add_tail(el_data);
    }

    return (el_datalist);
}

/*============================================================================*/
/*
 *	Conversion of Jump tbls
 */
/*============================================================================*/

void El_lcode_to_elcor_jumptbl_info(Procedure *proc, L_Datalist *tbl_list)
{
    L_Datalist_Element *element;
    L_Expr *expr;
    El_jumptbl_info *jumptbl_info;
    El_jumptbl *tbl;
    Vector<int> tbl_vals;
    eString tbl_name;
    int i, align_val, reserve_val, num_entries, cb_id;
    char fn_name[1024], *target;

    if (tbl_list==NULL)
	return;

    jumptbl_info =  new El_jumptbl_info();
    jumptbl_info->set_proc_name(proc);
    proc->set_jumptbl_info(jumptbl_info);
    El_setup_hb_bb_id_map(proc);

    /* The first element should be a (ms data), skip over this */
    element = tbl_list->first_element;
    if (element->data->type!=L_INPUT_MS)
	El_punt("El_lcode_to_elcor_jumptbl_info: corrupt tbl_list, no ms found!");
    element = element->next_element;
    
    while (element!=NULL) {
	if (element->data->type != L_INPUT_ALIGN)
	    El_punt("El_lcode_to_elcor_jumptbl_info: corrup tbl_list, no align found!");
	align_val = element->data->N;
	tbl_name = element->data->address->value.l;

	element = element->next_element;
	if (element==NULL)
	    El_punt("El_lcode_to_elcor_jumptbl_info: incomplete jump tbl");

	if (element->data->type != L_INPUT_RESERVE)
	    El_punt("El_lcode_to_elcor_jumptbl_info: corrup tbl_list, no reserve found!");
	reserve_val = element->data->N;

	element = element->next_element;
	if (element==NULL)
	    El_punt("El_lcode_to_elcor_jumptbl_info: incomplete jump tbl");

	num_entries = reserve_val / align_val;
	tbl_vals.resize(num_entries);

	/* Fill tbl_vals Vector */
	for (i=0; i<num_entries; i++) {
	    if (element->data->type != L_INPUT_WI)
	        El_punt("El_lcode_to_elcor_jumptbl_info: corrup tbl_list, no wi found!");
	    expr = element->data->value;
	    if (expr->type != L_EXPR_LABEL)
		El_punt("El_lcode_to_elcor_jumptbl_info: corrupt expr, should be label");
	    target = expr->value.l;
	    if (! M_is_cb_label(target, &fn_name[0], &cb_id))
		El_punt("El_lcode_to_elcor_jumptbl_info: corrupt target label");
	    tbl_vals[i] = cb_id;
	    element = element->next_element;
	}

        /* Create the table, and init the fields */
	tbl = new El_jumptbl(tbl_name, reserve_val, align_val);
	tbl->init_cbid_targets(tbl_vals);
        tbl->construct_op_targets_from_cbid_targets();

	jumptbl_info->add_tbl(tbl);
    }

    jumptbl_info->init_all_jump_op(proc);
    jumptbl_info->construct_all_cc_vals_from_op();
}


