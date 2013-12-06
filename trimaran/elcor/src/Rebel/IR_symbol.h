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
//
//      File:           IR_symbol.h
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    constant symbols used in Rebel Lang
//
///////////////////////////////////////////////////////////////////////////

#ifndef IR_SYMBOL_H
#define IR_SYMBOL_H

/*
 *	constant character decls
 */
extern const char L_PAREN;
extern const char R_PAREN;
extern const char L_BRACKET;
extern const char R_BRACKET;
extern const char L_ANGLE;
extern const char R_ANGLE;
extern const char L_CURLY;
extern const char R_CURLY;
extern const char EQUALS;
extern const char CARET;
extern const char COLON;
extern const char COMMA;
extern const char DOUBLE_QUOTE;
extern const char DASH;
extern const char SPACE;
extern const char TAB;
extern const char QUESTION_MARK;
extern const char POUND;
extern const char QUOTE;
extern const char BACKSLASH;
extern const char BSLASH;

/*
 *	constant string decls
 */
extern const char *OP_STRING;
extern const char *ATTR_STRING;
extern const char *BB_STRING;
extern const char *HB_STRING;
extern const char *LOOPBODY_STRING;
extern const char *PROC_STRING;
extern const char *TREE_STRING;
extern const char *COMPOUND_REGION_STRING;

/* Region field strings */
extern const char *REGION_WEIGHT_STRING;
extern const char *REGION_ENTRY_OPS_STRING;
extern const char *REGION_EXIT_OPS_STRING;
extern const char *REGION_ENTRY_EDGES_STRING;
extern const char *REGION_EXIT_EDGES_STRING;
extern const char *REGION_SUBREGIONS_STRING;
extern const char *REGION_FLAGS_STRING;

/* Attribute type strings */
extern const char *BB_ID_ATTR_STRING;
extern const char *LCODE_ATTR_STRING;
extern const char *MELD_ATTR_STRING;
extern const char *CFREQ_ATTR_STRING;
extern const char *LIVENESS_ATTR_STRING;
extern const char *DOWN_USE_ATTR_STRING;
extern const char *UP_DEF_ATTR_STRING;
extern const char *DOWN_DEF_ATTR_STRING;
extern const char *JUMPTBL_NAME_ATTR_STRING;
extern const char *REFERENCE_DTYPE_ATTR_STRING;
extern const char *REFERENCE_DTYPE_DEST_STRING;
extern const char *REFERENCE_DTYPE_SRC_STRING;
extern const char *REFERENCE_DTYPE_OP_STRING;
extern const char *OP_CHAINING_ATTR_STRING;
extern const char *REG_TO_LIVENESS_MAP_STRING;
extern const char *LOOP_COUNT_STRING;
extern const char *EXSYM_TABLE_STRING;
extern const char *NAMETAG_ATTR_STRING; 
extern const char *SPATIAL_REF_ATTR_STRING;
extern const char *SPATIAL_REF_DEST_STRING;
extern const char *SPATIAL_REF_SRC_STRING; 
extern const char *PHYSFILE_BIND_ATTR_STRING;
extern const char *PHYSFILE_BIND_DEST_STRING;
extern const char *PHYSFILE_BIND_SRC_STRING;
extern const char *CLUSTER_ID_ATTR_STRING;
extern const char *USCHED_TIME_ATTR_STRING;
extern const char *BRANCH_TARGET_ATTR_STRING;
extern const char *ACC_SPEC_ATTR_STRING;

/* Edge type strings */
extern const char *CONTROL_EDGE_STRING;
extern const char *REG_FLOW_EDGE_STRING;
extern const char *REG_ANTI_EDGE_STRING;
extern const char *REG_OUTPUT_EDGE_STRING;
extern const char *MEM_EDGE_STRING;

/* Port type strings */
extern const char *CONTROL_PORT_STRING;
extern const char *MEMORY_PORT_STRING;
extern const char *SRC_PORT_STRING;
extern const char *DEST_PORT_STRING;

extern const char *OP_IN_EDGES_STRING;
extern const char *OP_OUT_EDGES_STRING;

extern const char *EDGE_LATENCY_STRING;
extern const char *SCHED_TIME_STRING;
extern const char *SCHED_OPCODE_STRING;
extern const char *CUSTOM_OPCODE_STRING;
extern const char *RELATED_OPS_STRING;

extern const char *MEMVR_IN_STRING;
extern const char *MEMVR_OUT_STRING;

/* Operand/Data types */
extern const char *INT_OPERAND_STRING;
extern const char *DOUBLE_OPERAND_STRING;
extern const char *FLOAT_OPERAND_STRING;
extern const char *PREDICATE_OPERAND_STRING;
extern const char *CB_OPERAND_STRING;
extern const char *UNDEFINED_OPERAND_STRING;
extern const char *STRING_OPERAND_STRING;
extern const char *LABEL_OPERAND_STRING;
extern const char *MACRO_OPERAND_STRING;
extern const char *UNBOUND_REG_OPERAND_STRING;
extern const char *FILEBOUND_REG_OPERAND_STRING;
extern const char *BOUND_REG_OPERAND_STRING;
extern const char *MEMVR_OPERAND_STRING;
extern const char *VR_NAME_OPERAND_STRING;
extern const char *ROTATING_OPERAND_STRING;

/* Data types */
//extern const char *VOID_DATA_TYPE_STRING;
//extern const char *bool_DATA_TYPE_STRING;
//extern const char *CHAR_DATA_TYPE_STRING;
//extern const char *UCHAR_DATA_TYPE_STRING;
//extern const char *SHORT_DATA_TYPE_STRING;
//extern const char *USHORT_DATA_TYPE_STRING;
extern const char *INT_DATA_TYPE_STRING;
extern const char *UINT_DATA_TYPE_STRING;
//extern const char *LONG_DATA_TYPE_STRING;
//extern const char *ULONG_DATA_TYPE_STRING;
//extern const char *LLONG_DATA_TYPE_STRING;
//extern const char *ULLONG_DATA_TYPE_STRING;
//extern const char *LLLONG_DATA_TYPE_STRING;
//extern const char *ULLLONG_DATA_TYPE_STRING;
extern const char *POINTER_DATA_TYPE_STRING;
extern const char *FLOAT_DATA_TYPE_STRING;
extern const char *DOUBLE_DATA_TYPE_STRING;
extern const char *BRANCH_DATA_TYPE_STRING;
extern const char *PREDICATE_DATA_TYPE_STRING;
extern const char *LOCAL_ABS_DATA_TYPE_STRING;
extern const char *LOCAL_GP_DATA_TYPE_STRING;
extern const char *GLOBAL_ABS_DATA_TYPE_STRING;
extern const char *GLOBAL_GP_DATA_TYPE_STRING;
/* RMR { adding support for vector file type */
extern const char *VEC_INT_DATA_TYPE_STRING;
extern const char *VEC_FLOAT_DATA_TYPE_STRING;
extern const char *VEC_DOUBLE_DATA_TYPE_STRING;
/* } RMR */

/* Jump table fields */
extern const char *JUMPTBL_STRING;
extern const char *JUMPTBL_NAME_STRING;
extern const char *JUMPTBL_ENTRIES_STRING;
extern const char *JUMPTBL_RESERVE_STRING;
extern const char *JUMPTBL_ALIGN_STRING;
extern const char *JUMPTBL_DFLT_CC_STRING;

/* Dictionary strings */
extern const char *EDGE_DICT_STRING;
extern const char *ATTR_DICT_STRING;
extern const char *MEMVR_DICT_STRING;
extern const char *JUMPTBL_DICT_STRING;

extern const char *SECTION_DIVIDER_STRING;

#endif
