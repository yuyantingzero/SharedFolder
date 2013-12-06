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
//      File:           IR_symbol.cpp
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    constant symbols used in Rebel Lang
//
///////////////////////////////////////////////////////////////////////////

#include "IR_symbol.h"

/*
 *	constant character decls
 */
const char L_PAREN = '(';
const char R_PAREN = ')';
const char L_BRACKET = '[';
const char R_BRACKET = ']';
const char L_ANGLE = '<';
const char R_ANGLE = '>';
const char L_CURLY = '{';
const char R_CURLY = '}';
const char EQUALS = '=';
const char CARET = '^';
const char COLON = ':';
const char COMMA = ',';
const char DOUBLE_QUOTE = '"';
const char DASH = '-';
const char SPACE = ' ';
const char TAB = '\t';
const char QUESTION_MARK = '?';
const char BANG = '!';
const char QUOTE = '"';
const char BACKSLASH = '\\';
const char BSLASH = '\\';

/*
 *	constant string decls
 */
const char *OP_STRING = "op";
const char *ATTR_STRING = "attr";
const char *BB_STRING = "bb";
const char *HB_STRING = "hb";
const char *LOOPBODY_STRING = "loop_body";
const char *PROC_STRING = "proc";
const char *TREE_STRING = "tree";
const char *COMPOUND_REGION_STRING = "compound_region";

/* Region field strings */
const char *REGION_WEIGHT_STRING = "weight";
const char *REGION_ENTRY_OPS_STRING = "entry_ops";
const char *REGION_EXIT_OPS_STRING = "exit_ops";
const char *REGION_ENTRY_EDGES_STRING = "entry_edges";
const char *REGION_EXIT_EDGES_STRING = "exit_edges";
const char *REGION_SUBREGIONS_STRING = "subregions";
const char *REGION_FLAGS_STRING = "flags";

/* Attribute type strings */
const char *BB_ID_ATTR_STRING = "bb_id";
const char *LCODE_ATTR_STRING = "lc";
const char *MELD_ATTR_STRING = "ms";
const char *CFREQ_ATTR_STRING = "freq";
const char *LIVENESS_ATTR_STRING = "live";
const char *DOWN_USE_ATTR_STRING = "d_use";
const char *UP_DEF_ATTR_STRING = "u_def";
const char *DOWN_DEF_ATTR_STRING = "d_def";
const char *JUMPTBL_NAME_ATTR_STRING = "tbl_name";
const char *REFERENCE_DTYPE_ATTR_STRING = "it";
const char *REFERENCE_DTYPE_DEST_STRING = "d";
const char *REFERENCE_DTYPE_SRC_STRING = "s";
const char *REFERENCE_DTYPE_OP_STRING = "op";
const char *OP_CHAINING_ATTR_STRING = "opc";
const char *REG_TO_LIVENESS_MAP_STRING = "rtlm";
const char *LOOP_COUNT_STRING = "lcount";
const char *EXSYM_TABLE_STRING = "exsymtab";
const char *NAMETAG_ATTR_STRING = "nametag";
const char *SPATIAL_REF_ATTR_STRING ="spc";
const char *SPATIAL_REF_DEST_STRING = "d";
const char *SPATIAL_REF_SRC_STRING = "s";
const char *PHYSFILE_BIND_ATTR_STRING = "prf_bind";
const char *PHYSFILE_BIND_DEST_STRING = "d";
const char *PHYSFILE_BIND_SRC_STRING = "s";
const char *CLUSTER_ID_ATTR_STRING = "cid";
const char *USCHED_TIME_ATTR_STRING = "us_time";
const char *MSCHED_ATTR_STRING = "msched";
const char *BRANCH_TARGET_ATTR_STRING = "b_targ";
const char *ACC_SPEC_ATTR_STRING = "acc_spec";

/* Edge type strings */
const char *CONTROL_EDGE_STRING = "ctrl";
const char *REG_FLOW_EDGE_STRING = "reg_flow";
const char *REG_ANTI_EDGE_STRING = "reg_anti";
const char *REG_OUTPUT_EDGE_STRING = "reg_out";
const char *MEM_EDGE_STRING = "mem";

/* Port type strings */
const char *CONTROL_PORT_STRING = "ctl";
const char *MEMORY_PORT_STRING = "mem";
const char *SRC_PORT_STRING = "src";
const char *DEST_PORT_STRING = "dst";

const char *OP_IN_EDGES_STRING = "in_edges";
const char *OP_OUT_EDGES_STRING = "out";

const char *EDGE_LATENCY_STRING = "lat";
const char *SCHED_TIME_STRING = "s_time";
const char *SCHED_OPCODE_STRING = "s_opcode";
const char *CUSTOM_OPCODE_STRING = "custom_opcode";
const char *RELATED_OPS_STRING = "rel_ops";

const char *MEMVR_IN_STRING = "mem_in";
const char *MEMVR_OUT_STRING = "mem_out";

/* Operand types, note literals except for strings and labels are printed
   with just their data type */
const char *INT_OPERAND_STRING = "i";
const char *DOUBLE_OPERAND_STRING = "d";
const char *FLOAT_OPERAND_STRING = "f";
const char *PREDICATE_OPERAND_STRING = "p";
const char *CB_OPERAND_STRING = "b";
const char *UNDEFINED_OPERAND_STRING = "u";
const char *STRING_OPERAND_STRING = "s";
const char *LABEL_OPERAND_STRING = "l";
const char *MACRO_OPERAND_STRING = "m";
const char *UNBOUND_REG_OPERAND_STRING = "vr";
const char *FILEBOUND_REG_OPERAND_STRING = "r";
const char *BOUND_REG_OPERAND_STRING = "br";
const char *MEMVR_OPERAND_STRING = "mvr";
const char *VR_NAME_OPERAND_STRING = "nvr";
const char *ROTATING_OPERAND_STRING = "r";

/* Data types */
//const char *VOID_DATA_TYPE_STRING = "vo";
//const char *bool_DATA_TYPE_STRING = "bo";
//const char *CHAR_DATA_TYPE_STRING = "ch";
//const char *UCHAR_DATA_TYPE_STRING = "uch";
//const char *SHORT_DATA_TYPE_STRING = "sh";
//const char *USHORT_DATA_TYPE_STRING = "ush";
const char *INT_DATA_TYPE_STRING = "i";
const char *UINT_DATA_TYPE_STRING = "ui";
//const char *LONG_DATA_TYPE_STRING = "lng";
//const char *ULONG_DATA_TYPE_STRING = "ulng";
//const char *LLONG_DATA_TYPE_STRING = "llng";
//const char *ULLONG_DATA_TYPE_STRING = "ullng";
//const char *LLLONG_DATA_TYPE_STRING = "lllng";
//const char *ULLLONG_DATA_TYPE_STRING = "ulllng";
const char *POINTER_DATA_TYPE_STRING = "ptr";
const char *FLOAT_DATA_TYPE_STRING = "f";
const char *DOUBLE_DATA_TYPE_STRING = "d";
const char *BRANCH_DATA_TYPE_STRING = "b";
const char *PREDICATE_DATA_TYPE_STRING = "p";
const char *LOCAL_ABS_DATA_TYPE_STRING = "l_abs";
const char *LOCAL_GP_DATA_TYPE_STRING = "l_gp";
const char *GLOBAL_ABS_DATA_TYPE_STRING = "g_abs";
const char *GLOBAL_GP_DATA_TYPE_STRING = "g_gp";
/* RMR { adding support for vector file type */
const char *VEC_INT_DATA_TYPE_STRING = "vi";
const char *VEC_FLOAT_DATA_TYPE_STRING = "vf";
const char *VEC_DOUBLE_DATA_TYPE_STRING = "vd";
/* } RMR */

/* Jump table fields */
const char *JUMPTBL_STRING = "table";
const char *JUMPTBL_NAME_STRING = "name";
const char *JUMPTBL_ENTRIES_STRING = "entries";
const char *JUMPTBL_RESERVE_STRING = "reserve";
const char *JUMPTBL_ALIGN_STRING = "align";
const char *JUMPTBL_DFLT_CC_STRING = "DFLT";

/* Dictionary strings */
const char *EDGE_DICT_STRING = "edges";
const char *ATTR_DICT_STRING = "attributes";
const char *MEMVR_DICT_STRING = "mem_vrs";
const char *JUMPTBL_DICT_STRING = "jump_tables";

const char *SECTION_DIVIDER_STRING = "//++++++++++++++++++++++++++++++++++++++++++++++++++";
