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
//      File:           opcode_properties.h
//      Authors:        Sadun Anik, Richard Johnson, Scott Mahlke,
//                      Dave August, Sumedh Sathaye
//      Created:        December 1994
//      Description:    Functions specific to opcode values 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OPCODE_PROPERTIES
#define _OPCODE_PROPERTIES

#include "opcode.h"
#include "op.h"
#include "el_port.h"
class Op ;

///////////////////////////////////////////////////////////////////////////////
//
//      Macros
//
//////////////////////////////////////////////////////////////////////////////

IR_ROOT_OPCODE get_root(Opcode);

IR_BASE_OPCODE get_base(Opcode);

IR_MODIFIERS get_cond_modifier(Opcode);

///////////////////////////////////////////////////////////////////////////////
//
//      Branch queries
//
//////////////////////////////////////////////////////////////////////////////

// Anything that changes control flow + dummy branches
bool is_control_switch(Op *op_ptr) ;
bool is_control_switch(Opcode opc) ;

// All of the above except for BRL
bool is_branch(Op *op_ptr) ;
bool is_branch(Opcode opc) ;

// All of the above except for dummy branches and RTS
bool is_proper_branch(Op* op_ptr) ;
bool is_proper_branch(Opcode opc) ;

// Dummmy branch
bool is_dummy_branch(Op* op_ptr);
bool is_dummy_branch(Opcode opc);

// Conditional, unconditional and dummy branches, no SWP branches
bool is_traditional_branch(Op* op_ptr) ;
bool is_traditional_branch(Opcode opc) ;

// BRCT, BRCF and multiple-exit-edge brdirs
bool is_conditional_branch(Op* op_ptr) ;
bool is_conditional_branch(Opcode opc) ;

// BRU, dummy branch, BRINDS and single-exit-edge brdirs
bool is_unconditional_branch(Op* op_ptr) ;
bool is_unconditional_branch(Opcode opc) ;

//Normalized branches (BRDIRs and BRINDs)
bool is_normalized_branch(Op* op_ptr);
bool is_normalized_branch(Opcode opc);

//Normalized form of register jump
bool is_brind(Op* op_ptr) ;
bool is_brind(Opcode opc) ;

//Normalized forms of conditional/unconditional branches
bool is_brdir(Op* op_ptr) ;
bool is_brdir(Opcode opc) ;

bool is_brct(Op* op_ptr) ;
bool is_brct(Opcode opc) ;


bool is_brcf(Op* op_ptr) ;
bool is_brcf(Opcode opc) ;

bool is_bru(Op* op_ptr) ;
bool is_bru(Opcode opc) ;

bool is_brl(Op* op_ptr) ;
bool is_brl(Opcode opc) ;

bool is_brlp(Op* op_ptr) ;
bool is_brlp(Opcode opc) ;

bool is_rts(Op* op_ptr) ;
bool is_rts(Opcode opc) ;

bool is_brlc(Op* op_ptr) ;
bool is_brlc(Opcode opc) ;

bool is_brdv(Op* op_ptr) ;
bool is_brdv(Opcode opc) ;

bool is_brf(Op *op_ptr);
bool is_brf(Opcode opc);

bool is_brw(Op *op_ptr);
bool is_brw(Opcode opc);

bool is_swp_branch(Op *op_ptr);
bool is_swp_branch(Opcode opc);

bool is_call(Op *op_ptr) ;
bool is_call(Opcode opc) ;

bool is_return(Op *op_ptr) ;
bool is_return(Opcode opc) ;

bool is_movelb(Op * op_ptr) ;
bool is_movelb(Opcode opc) ;

bool is_movelbx(Op * op_ptr) ;
bool is_movelbx(Opcode opc) ;

bool is_movelbs(Op * op_ptr) ;
bool is_movelbs(Opcode opc) ;

bool is_pbr(Op * op_ptr) ;
bool is_pbr(Opcode opc) ;

bool is_pbrr(Op * op_ptr) ;
bool is_pbrr(Opcode opc) ;

bool is_pbra(Op* op_ptr) ;
bool is_pbra(Opcode opc) ;

bool requires_pbr(Op * op_ptr) ;
bool requires_pbr(Opcode opc) ;

///////////////////////////////////////////////////////////////////////////////
//
//      Comparison queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_cmpr(Op *op_ptr) ;
bool is_cmpr(Opcode opc) ;

bool is_icmpr(Op *op_ptr) ;
bool is_icmpr(Opcode opc) ;

bool is_fcmpr(Op *op_ptr) ;
bool is_fcmpr(Opcode opc) ;

bool is_int_eq_cmpr(Op *op_ptr);
bool is_int_eq_cmpr(Opcode opc);

bool is_int_neq_cmpr(Op *op_ptr);
bool is_int_neq_cmpr(Opcode opc);

bool is_int_leq_cmpr(Op* op_ptr) ;
bool is_int_leq_cmpr(Opcode opc) ;

bool is_int_geq_cmpr(Op* op_ptr) ;
bool is_int_geq_cmpr(Opcode opc) ;

bool is_int_lt_cmpr(Op* op_ptr) ;
bool is_int_lt_cmpr(Opcode opc) ;

bool is_int_gt_cmpr(Op* op_ptr) ;
bool is_int_gt_cmpr(Opcode opc) ;

///////////////////////////////////////////////////////////////////////////////
//
//      Predicate comparison/setting queries
//
///////////////////////////////////////////////////////////////////////////////

Opcode get_cmpp_dest1_modifier(Opcode opc);
Opcode get_cmpp_dest2_modifier(Opcode opc);

bool is_cmpp(Op *op_ptr) ;
bool is_cmpp(Opcode opc) ;

bool is_icmpp(Op *op_ptr) ;
bool is_icmpp(Opcode opc) ;

bool is_fcmpp(Op *op_ptr) ;
bool is_fcmpp(Opcode opc) ;

bool is_predicate_clear(Op *op_ptr) ;
bool is_predicate_clear(Opcode opc) ;

bool is_predicate_clear_all_rotating(Op *op_ptr) ;
bool is_predicate_clear_all_rotating(Opcode opc) ;

bool is_predicate_set(Op *op_ptr) ;
bool is_predicate_set(Opcode opc) ;

bool is_clear_all_rotating(Op *op_ptr) ;
bool is_clear_all_rotating(Opcode opc) ;

bool is_U_pred(Op* op, Port_num dest);
bool is_U_pred(Opcode opc, Port_num dest);

bool is_UN_pred(Op* op, Port_num dest);
bool is_UN_pred(Opcode opc, Port_num dest);

bool is_UC_pred(Op* op, Port_num dest);
bool is_UC_pred(Opcode opc, Port_num dest);

bool is_C_pred(Op* op, Port_num dest);
bool is_C_pred(Opcode opc, Port_num dest);

bool is_CN_pred(Op* op, Port_num dest);
bool is_CN_pred(Opcode opc, Port_num dest);

bool is_CC_pred(Op* op, Port_num dest);
bool is_CC_pred(Opcode opc, Port_num dest);

bool is_O_pred(Op* op, Port_num dest);
bool is_O_pred(Opcode opc, Port_num dest);

bool is_ON_pred(Op* op, Port_num dest);
bool is_ON_pred(Opcode opc, Port_num dest);

bool is_OC_pred(Op* op, Port_num dest);
bool is_OC_pred(Opcode opc, Port_num dest);

bool is_A_pred(Op* op, Port_num dest);
bool is_A_pred(Opcode opc, Port_num dest);

bool is_AN_pred(Op* op, Port_num dest);
bool is_AN_pred(Opcode opc, Port_num dest);

bool is_AC_pred(Op* op, Port_num dest);
bool is_AC_pred(Opcode opc, Port_num dest);

bool is_normal_pred(Op* op, Port_num dest);
bool is_normal_pred(Opcode opc, Port_num dest);

bool is_complement_pred(Op* op, Port_num dest);
bool is_complement_pred(Opcode opc, Port_num dest);

Opcode cmpp_dest1_to_dest2(Opcode opc);
Opcode cmpp_dest2_to_dest1(Opcode opc);

Opcode make_complement_pred(Op* op, Port_num dest);
Opcode make_complement_pred(Opcode opc, Port_num dest);

extern Opcode make_UN_pred(Opcode opc, Port_num dest);
extern Opcode make_UC_pred(Opcode opc, Port_num dest);
extern Opcode make_ON_pred(Opcode opc, Port_num dest);
extern Opcode make_OC_pred(Opcode opc, Port_num dest);
extern Opcode make_AN_pred(Opcode opc, Port_num dest);
extern Opcode make_AC_pred(Opcode opc, Port_num dest);

extern Opcode make_U_pred(Opcode opc, Port_num dest);
extern Opcode make_O_cmpp(Opcode opc);

bool is_int_leq_cmpp(Op* op_ptr) ;
bool is_int_leq_cmpp(Opcode opc) ;

bool is_int_geq_cmpp(Op* op_ptr) ;
bool is_int_geq_cmpp(Opcode opc) ;

bool is_int_lt_cmpp(Op* op_ptr) ;
bool is_int_lt_cmpp(Opcode opc) ;

bool is_int_gt_cmpp(Op* op_ptr) ;
bool is_int_gt_cmpp(Opcode opc) ;

bool is_int_neq_cmpp(Op* op_ptr) ;
bool is_int_neq_cmpp(Opcode opc) ;

bool is_int_eq_cmpp(Op* op_ptr) ;
bool is_int_eq_cmpp(Opcode opc) ;

bool is_conditionally_executed(Op* op_ptr) ;

extern Opcode exchange_cmpp_dest_modifiers(Opcode opc);

///////////////////////////////////////////////////////////////////////////////
//
//      Load/Store queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_postincrement(Opcode opc);
bool is_postincrement(Op *op_ptr);

Opcode get_mem_src_modifier(Opcode opc);
Opcode get_mem_dest_modifier(Opcode opc);
Opcode get_local_mem_modifier(Opcode opc);

Opcode get_C1_dest_load(Opcode);

// Loop cache extensions: Pracheeti
bool is_lc_copy(Op *op_ptr);
bool is_lc_copy(Opcode opc);

bool is_load(Op *op_ptr) ;
bool is_load(Opcode opc) ;

bool is_sign_extend_load(Op *op_ptr);
bool is_sign_extend_load(Opcode opc);

bool is_base_plus_offset_load(Op *op_ptr);
bool is_base_plus_offset_load(Opcode opc);

bool is_load_from_C1(Op *op_ptr) ;
bool is_load_from_C1(Opcode opc) ;

bool is_store(Op *op_ptr) ;
bool is_store(Opcode opc) ;

bool is_base_plus_offset_store(Op *op_ptr);
bool is_base_plus_offset_store(Opcode opc);

// load, store ops
bool is_memory(Op *op_ptr) ;
bool is_memory(Opcode opc) ;

bool is_mem_base_plus_offset(Op *op_ptr);
bool is_mem_base_plus_offset(Opcode opc);

// load, store, brl, rts ops
bool is_memref(Op *op_ptr) ;
bool is_memref(Opcode opc) ;

bool is_save(Op *op_ptr);
bool is_save(Opcode opc);

bool is_restore(Op *op_ptr);
bool is_restore(Opcode opc);

bool is_gpr_save_restore(Opcode opc);
bool is_gpr_save_restore(Op *op_ptr);
bool is_fpr_save_restore(Opcode opc);
bool is_fpr_save_restore(Op *op_ptr);
bool is_single_fpr_save_restore(Op *op_ptr);
bool is_btr_save_restore(Opcode opc);
bool is_btr_save_restore(Op *op_ptr);
bool is_pr_save_restore(Opcode opc);
bool is_pr_save_restore(Op *op_ptr);
bool is_save_restore(Opcode opc);
bool is_save_restore(Op *op_ptr);


extern Opcode remove_post_increment(Opcode opc);
extern Opcode extract_increment_from_post_increment(Opcode opc);

bool is_pred_load_all(Op *op_ptr) ;
bool is_pred_load_all(Opcode opc) ;

bool is_pred_store_all(Op *op_ptr) ;
bool is_pred_store_all(Opcode opc) ;

bool is_local_mem_load(Op *op_ptr);
bool is_local_mem_load(Opcode opc);

bool is_local_mem_store(Op *op_ptr);
bool is_local_mem_store(Opcode opc);

bool is_local_mem(Op *op_ptr);
bool is_local_mem(Opcode opc);


///////////////////////////////////////////////////////////////////////////////
//
//      Arithmetic queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_no_op(Op*) ;
bool is_no_op(Opcode opc) ;

bool is_or(Op*) ;
bool is_or(Opcode opc) ;

bool is_and(Op*) ;
bool is_and(Opcode opc) ;

bool is_xor(Op*) ;
bool is_xor(Opcode opc) ;

bool is_abs(Op*) ;
bool is_abs(Opcode opc) ;

bool is_move(Op*) ;
bool is_move(Opcode opc) ;

bool is_movepp(Op*) ;
bool is_movepp(Opcode opc) ;

bool is_int_add(Op*) ;
bool is_int_add(Opcode opc) ;

bool is_int_sub(Op*) ;
bool is_int_sub(Opcode opc) ;

bool is_int_mul(Op*) ;
bool is_int_mul(Opcode opc) ;

bool is_int_muladd(Op*) ;
bool is_int_muladd(Opcode opc) ;

bool is_int_mulsub(Op*) ;
bool is_int_mulsub(Opcode opc) ;

bool is_int_div(Op*) ;
bool is_int_div(Opcode opc) ;

bool is_int_rem(Op*) ;
bool is_int_rem(Opcode opc) ;

bool is_int_lshift(Op*) ;
bool is_int_lshift(Opcode opc) ;

bool is_int_rshift(Op*) ;
bool is_int_rshift(Opcode opc) ;

bool is_signed_extract(Op* op_ptr);
bool is_signed_extract(Opcode opc);
bool is_unsigned_extract(Op* op_ptr);
bool is_unsigned_extract(Opcode opc);
bool is_extract(Op* op_ptr);
bool is_extract(Opcode opc);

Opcode get_saturate_modifier(Opcode opc);
int extract_saturate_point(Opcode opc);
int extract_saturate_point(IR_MODIFIERS mod);

bool is_int_satadd(Op*) ;
bool is_int_satadd(Opcode opc) ;
bool is_int_satsub(Op*) ;
bool is_int_satsub(Opcode opc) ;
bool is_int_satmul(Op*) ;
bool is_int_satmul(Opcode opc) ;
bool is_int_satmuladd(Op*) ;
bool is_int_satmuladd(Opcode opc) ;
bool is_int_satmulsub(Op*) ;
bool is_int_satmulsub(Opcode opc) ;

bool is_flt_add(Op*) ;
bool is_flt_add(Opcode opc) ;

bool is_flt_sub(Op*) ;
bool is_flt_sub(Opcode opc) ;

bool is_flt_mul(Op*) ;
bool is_flt_mul(Opcode opc) ;

bool is_flt_muladd(Op*) ;
bool is_flt_muladd(Opcode opc) ;

bool is_flt_mulsub(Op*) ;
bool is_flt_mulsub(Opcode opc) ;

bool is_flt_div(Op*) ;
bool is_flt_div(Opcode opc) ;

bool is_add(Op*) ;
bool is_add(Opcode opc) ;

bool is_add_carry(Op*) ;
bool is_add_carry(Opcode opc) ;

bool is_sub_carry(Op*) ;
bool is_sub_carry(Opcode opc) ;

bool is_mul_wide(Op*) ;
bool is_mul_wide(Opcode opc) ;

bool is_sub(Op*) ;
bool is_sub(Opcode opc) ;

bool is_mul(Op*) ;
bool is_mul(Opcode opc) ;

bool is_muladd(Op*) ;
bool is_muladd(Opcode opc) ;

bool is_mulsub(Op*) ;
bool is_mulsub(Opcode opc) ;

bool is_div(Op*) ;
bool is_div(Opcode opc) ;

bool is_ialu(Op*) ;
bool is_ialu(Opcode opc) ;
bool is_ialu_simple_binary_op (Op* op_ptr); // sumedh
bool is_ialu_simple_binary_op (Opcode opc);
bool is_ialu_complex_binary_op (Op* op_ptr);
bool is_ialu_complex_binary_op (Opcode opc);

bool is_falu(Op*) ;
bool is_falu(Opcode opc) ;
bool is_falu_simple_binary_op (Op* op_ptr); // sumedh
bool is_falu_simple_binary_op (Opcode opc);
bool is_falu_complex_binary_op (Op* op_ptr);
bool is_falu_complex_binary_op (Opcode opc);

bool is_literal_move(Op* op_ptr);
bool is_literal_move(Opcode opc);

bool is_literal_move_with_shift(Op* op_ptr);
bool is_literal_move_with_shift(Opcode opc);

bool is_int_computation_op(Opcode opc);
bool is_int_computation_op(Op *op_ptr);
bool is_flt_computation_op(Opcode opc);
bool is_flt_computation_op(Op *op_ptr);
bool is_dbl_computation_op(Opcode opc);
bool is_dbl_computation_op(Op *op_ptr);
bool is_int_conversion_op(Opcode opc);
bool is_int_conversion_op(Op *op_ptr);
bool is_flt_conversion_op(Opcode opc);
bool is_flt_conversion_op(Op *op_ptr);
bool is_dbl_conversion_op(Opcode opc);
bool is_dbl_conversion_op(Op *op_ptr);
bool is_int_fltdbl_conversion_op(Opcode opc);
bool is_int_fltdbl_conversion_op(Op *op_ptr);

bool is_bit_extraction_op(Opcode opc);
bool is_bit_extraction_op(Op *op_ptr);

/* RMR { */
bool is_vector(Op*) ;
bool is_vector(Opcode opc) ;
bool is_vector_ialu(Op*) ;
bool is_vector_ialu(Opcode opc) ;
bool is_vector_falu(Op*) ;
bool is_vector_falu(Opcode opc) ;
bool is_vector_load(Op*) ;
bool is_vector_load(Opcode opc) ;
bool is_vector_store(Op*) ;
bool is_vector_store(Opcode opc) ;
bool is_vector_perm(Op*) ;
bool is_vector_perm(Opcode opc) ;
bool is_svmove(Op*) ;
bool is_svmove(Opcode opc) ;
bool is_vsmove(Op*) ;
bool is_vsmove(Opcode opc) ;
/* } RMR */



Opcode convert_MEMW_to_MEMHW(Opcode opc); // for WIMS SIMU, pnagarka 4/03
Opcode convert_ADD_to_MPYADD(Opcode opc, bool nflag);
Opcode convert_SUB_to_MPYSUB(Opcode opc, bool rflag);

///////////////////////////////////////////////////////////////////////////////
//
//      Pseudo Op queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_pseudo(Op* op);
bool is_pseudo(Opcode opc);

bool is_control_merge(Op *) ;
bool is_control_merge(Opcode opc) ;

bool is_merge(Op *) ;
bool is_merge(Opcode opc) ;

bool is_switch(Op *) ;
bool is_switch(Opcode opc) ;

bool is_use(Op* op_ptr) ;
bool is_use(Opcode opc) ;

bool is_def(Op* op_ptr) ;
bool is_def(Opcode opc) ;

bool is_remap(Op* op_ptr) ;
bool is_remap(Opcode opc) ;

bool is_define(Op* op_ptr) ;
bool is_define(Opcode opc) ;

bool is_pseudo_load(Op* op_ptr) ;
bool is_pseudo_load(Opcode opc) ;

/* Shivarama Rao; Added the code on 06-06-2001
 * This code is defined as new pseudo_load operations are 
 * added, which loads explicit contents instead of earlier
 * overloaded PSEUDO_LOAD operation
 */
bool is_pseudo_load_literal(Op* op_ptr) ;
bool is_pseudo_load_literal(Opcode opc) ;

bool is_pseudo_load_scalar(Op* op_ptr) ;
bool is_pseudo_load_scalar(Opcode opc) ;

bool is_pseudo_load_scalar_array(Op* op_ptr) ;
bool is_pseudo_load_scalar_array(Opcode opc) ;

bool is_pseudo_load_evr(Op* op_ptr) ;
bool is_pseudo_load_evr(Opcode opc) ;

bool is_pseudo_store(Op* op_ptr) ;
bool is_pseudo_store(Opcode opc) ;

/* Shivarama Rao; Added the code on 06-06-2001
 * This code is defined as new pseudo_store operations are 
 * added, which stores to explicit contents instead of earlier
 * overloaded PSEUDO_STORE operation
 */
bool is_pseudo_store_scalar(Op* op_ptr) ;
bool is_pseudo_store_scalar(Opcode opc) ;

bool is_pseudo_store_scalar_array(Op* op_ptr) ;
bool is_pseudo_store_scalar_array(Opcode opc) ;

bool is_pseudo_store_evr(Op* op_ptr) ;
bool is_pseudo_store_evr(Opcode opc) ;

bool is_pseudo_print(Op* op_ptr) ;
bool is_pseudo_print(Opcode opc) ;

bool is_pseudo_init_mem(Op* op_ptr) ;
bool is_pseudo_init_mem(Opcode opc) ;

///////////////////////////////////////////////////////////////////////////////
//
//      Miscellaneous queries
//
///////////////////////////////////////////////////////////////////////////////

Op* real_op (Op* op) ;

bool is_double(Op *op);
bool is_double(Opcode opc);

bool has_side_effect(Op* op);
bool has_side_effect(Opcode opc);

Opcode get_reverse_op(Opcode opc);

Opcode get_opposite_cond(Opcode opc);

Opcode convert_CMPP_to_CMPR(Opcode opc);
Opcode convert_CMPR_to_base_CMPP(Opcode opc);

Opcode change_base_opcode(Opcode opc, Opcode new_base);

bool is_excepting_op(Op *op_ptr);
bool is_excepting_op(Opcode opc);

int extract_local_memory_id(Opcode opc);
int extract_local_memory_id(IR_MODIFIERS mod);

Opcode convert_to_local_memory(Opcode opc, IR_MODIFIERS lm_spec);
Opcode convert_to_local_memory(Opcode opc, int lm_id);

bool is_logical_int_cmp(Op *op_ptr) ;
bool is_logical_int_cmp(Opcode opc) ;

bool is_logical_op(Op *op_ptr);
bool is_logical_op(Opcode opc) ;

/*modification for custom opcode tangw (06-04-02)*/
bool is_custom(Op *op_ptr);
bool is_custom(Opcode opc);


/* streams */

bool is_stream_access(Op *op_ptr) ;
bool is_stream_access(Opcode opc) ;

// parkhc 02/2007
int opcode_data_width(Op *op_ptr);
int opcode_data_width(Opcode opc);




///////////////////////////////////////////////////////////////////////////////
//
//      Queries/Functions to assist with forming ops
//
///////////////////////////////////////////////////////////////////////////////

Opcode get_move_opcode_for_operand(Operand &operand);
Opcode get_move_opcode_for_operands(const Operand &dest, const Operand &src);
Opcode get_add_opcode_for_operand(Operand &operand);
Opcode get_sub_opcode_for_operand(Operand &operand);
Opcode get_mpy_opcode_for_operand(Operand &operand);
Opcode get_load_opcode_for_operand(Operand &operand);
Opcode get_store_opcode_for_operand(Operand &operand);
Opcode get_C2_load_opcode_for_operand(Operand &operand);
Opcode get_C2_store_opcode_for_operand(Operand &operand);

Op *El_generate_move_op(Operand &d, Operand &s, Operand &p);
Op *El_generate_add_op(Operand &d, Operand &s1, Operand &s2, Operand &p);
Op *El_generate_sub_op(Operand &d, Operand &s1, Operand &s2, Operand &p);
Op *El_generate_mpy_op(Operand &d, Operand &s1, Operand &s2, Operand &p);

void El_convert_to_move(Op *op, Operand &d, Operand &s1, Operand &p);
void El_convert_to_bru(Op* op, Operand &s1, Operand &p);

#endif
