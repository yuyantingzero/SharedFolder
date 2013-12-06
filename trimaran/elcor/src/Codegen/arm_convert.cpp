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
//      File:           arm_op_convert.cpp
//      Authors:        Jason Blome
//      Created:        February 2005
//      Description:    functions to exchange a single operation unsupported 
//                      by the arm architecture into an equivalent set of
//                      operations that are supported
//
/////////////////////////////////////////////////////////////////////////////


#include "codegen.h"
#include "opcode_cmpp.h"

extern Op* make_bd_from_non_bd(Op*);


// valid literals in ARM data processing instructions are any value
// that can be expressed with 8-bits rotated an even number of places.
static bool is_valid_dp_const(int n)
{
  // This code doesn't work unless ints are 32-bit.
  assert(sizeof(int) == 4);

  if((n & 0xFFFFFF00) == 0)
    return true;
  if((n & 0xFFFFFC03) == 0)
    return true;
  if((n & 0xFFFFF00F) == 0)
    return true;
  if((n & 0xFFFFC03F) == 0)
    return true;
  if((n & 0xFFFF00FF) == 0)
    return true;
  if((n & 0xFFFC03FF) == 0)
    return true;
  if((n & 0xFFF00FFF) == 0)
    return true;
  if((n & 0xFFC03FFF) == 0)
    return true;
  if((n & 0xFF00FFFF) == 0)
    return true;
  if((n & 0xFC03FFFF) == 0)
    return true;
  if((n & 0xF00FFFFF) == 0)
    return true;
  if((n & 0xC03FFFFF) == 0)
    return true;
  if((n & 0x00FFFFFF) == 0)
    return true;
  if((n & 0x03FFFFFC) == 0)
    return true;
  if((n & 0x0FFFFFF0) == 0)
    return true;
  if((n & 0x3FFFFFC0) == 0)
    return true;

  return false;
}


//////////////////////////////////////////////////////////////////////////////
// FIX: there's so much redundant code in this file, it makes me want to puke.

void arm_opc_table_initialize(Hash_map<Opcode, FuncImpl> **opc_table)
{
  Codegen::initialize_opc_table(opc_table);

  // Add the opcodes that are not supported
  // Make sure the unsupported flag is set in the MDES  
  (*opc_table)->bind((Opcode) CMPP_W_LLT_UN_UN, arm_op_convert_llt);
  (*opc_table)->bind((Opcode) CMPR_W_LLT, arm_op_convert_cmpr_llt);
  (*opc_table)->bind((Opcode) CMPP_W_LGEQ_UN_UN, arm_op_convert_lgeq);
  (*opc_table)->bind((Opcode) CMPR_W_LGEQ, arm_op_convert_cmpr_lgeq);
  (*opc_table)->bind((Opcode) DIV_W, arm_op_convert_div_w);
  (*opc_table)->bind((Opcode) DIVL_W, arm_op_convert_divl_w);
  (*opc_table)->bind((Opcode) EXTS_B, arm_op_convert_exts_b);  
  (*opc_table)->bind((Opcode) EXTS_H, arm_op_convert_exts_h);
  (*opc_table)->bind((Opcode) EXTZ_B, arm_op_convert_extz_b);
  (*opc_table)->bind((Opcode) EXTZ_H, arm_op_convert_extz_h);
  (*opc_table)->bind((Opcode) REM_W, arm_op_convert_rem_w);
  (*opc_table)->bind((Opcode) REML_W, arm_op_convert_reml_w);
}


List<Op*>
arm_op_convert_deposit(Op* op)
{
  cdbg << "ERROR: I don't know how to convert this op!\n" << *op << endl;
  assert(0);
  return List<Op*>();
}


List<Op*> 
arm_op_convert_extractu_b(Op *op) 
{
  List<Op*> op_list;

  Operand ext_src1 = op->src(SRC1);
  Operand ext_src2 = op->src(SRC2);
  
  Op* lsr = new Op((Opcode)SHR_W);
  Op* and_op = new Op((Opcode)AND_W);
  assert(lsr && and_op && "out of memory");
  lsr->set_src(PRED1, op->src(PRED1));
  and_op->set_src(PRED1, op->src(PRED1));

  Operand tmp(new Reg(EL_STANDARD_INT));
  lsr->set_dest(DEST1, tmp);
  lsr->set_src(SRC1, op->src(SRC1));
  lsr->set_src(SRC2, op->src(SRC2));

  and_op->set_dest(DEST1, op->dest(DEST1));
  and_op->set_src(SRC1, tmp);

  int mask = ~(-1 << op->src(SRC3).int_value());
  mask &= 255;
  Operand mask_oper(new Int_lit(mask));
  and_op->set_src(SRC2, mask_oper);

  op_list.add_tail(lsr);
  op_list.add_tail(and_op);

  return op_list;
}


List<Op*> 
arm_op_convert_extractu_h(Op *op) 
{
  List<Op*> op_list;

  Operand ext_src1 = op->src(SRC1);
  Operand ext_src2 = op->src(SRC2);
  
  Op* lsr = new Op((Opcode)SHR_W);
  Op* and_op = new Op((Opcode)AND_W);
  assert(lsr && and_op && "out of memory");
  lsr->set_src(PRED1, op->src(PRED1));
  and_op->set_src(PRED1, op->src(PRED1));

  Operand tmp(new Reg(EL_STANDARD_INT));
  lsr->set_dest(DEST1, tmp);
  lsr->set_src(SRC1, op->src(SRC1));
  lsr->set_src(SRC2, op->src(SRC2));

  and_op->set_dest(DEST1, op->dest(DEST1));
  and_op->set_src(SRC1, tmp);

  int mask = ~(-1 << op->src(SRC3).int_value());
  mask &= 65535;
  Operand mask_oper(new Int_lit(mask));
  and_op->set_src(SRC2, mask_oper);

  op_list.add_tail(lsr);
  op_list.add_tail(and_op);

  return op_list;
}


List<Op*> 
arm_op_convert_extractu_w(Op *op) 
{
  List<Op*> op_list;

  Operand ext_src1 = op->src(SRC1);
  Operand ext_src2 = op->src(SRC2);
  
  Op* lsr = new Op((Opcode)SHR_W);
  Op* and_op = new Op((Opcode)AND_W);
  assert(lsr && and_op && "out of memory");
  lsr->set_src(PRED1, op->src(PRED1));
  and_op->set_src(PRED1, op->src(PRED1));

  Operand tmp(new Reg(EL_STANDARD_INT));
  lsr->set_dest(DEST1, tmp);
  lsr->set_src(SRC1, op->src(SRC1));
  lsr->set_src(SRC2, op->src(SRC2));

  and_op->set_dest(DEST1, op->dest(DEST1));
  and_op->set_src(SRC1, tmp);

  int mask = ~(-1 << op->src(SRC3).int_value());
  Operand mask_oper(new Int_lit(mask));
  and_op->set_src(SRC2, mask_oper);

  op_list.add_tail(lsr);
  op_list.add_tail(and_op);

  return op_list;
}


/* 
 * Convert compare unsigned less than or equal to an unsigned greater
 * than with the operands swapped
 */
List<Op*> 
arm_op_convert_llt(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();
  Operand cmp_src1 = op->src(SRC1);
  Operand cmp_src2 = op->src(SRC2);
  
  Op* cmpp = new Op((Opcode)CMPP_W_LGT_UN_UN);
  assert(cmpp && "out of memory");
  Operand dst_pred(op->dest(DEST1));
  Operand dst_pred2(op->dest(DEST2));
  cmpp->set_dest(DEST1, dst_pred);
  cmpp->set_dest(DEST2, dst_pred2);
  cmpp->set_src(SRC1, cmp_src2);
  cmpp->set_src(SRC2, cmp_src1);
  cmpp->set_src(PRED1, op->src(PRED1));
  op_list.add_tail(cmpp);  

  return op_list;
}

List<Op*> 
arm_op_convert_lgeq(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();
  Operand cmp_src1 = op->src(SRC1);
  Operand cmp_src2 = op->src(SRC2);
  
  Op* cmpp = new Op((Opcode)CMPP_W_LLEQ_UN_UN);
  assert(cmpp && "out of memory");
  Operand dst_pred(op->dest(DEST1));
  Operand dst_pred2(op->dest(DEST2));
  cmpp->set_dest(DEST1, dst_pred);
  cmpp->set_dest(DEST2, dst_pred2);
  cmpp->set_src(SRC1, cmp_src2);
  cmpp->set_src(SRC2, cmp_src1);
  cmpp->set_src(PRED1, op->src(PRED1));
  op_list.add_tail(cmpp);  

  return op_list;
}

List<Op*> 
arm_op_convert_cmpr_lgeq(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();
  Operand cmp_src1 = op->src(SRC1);
  Operand cmp_src2 = op->src(SRC2);
  
  Op* cmpr = new Op((Opcode)CMPR_W_LLEQ);
  assert(cmpr && "out of memory");
  Operand dst_pred(op->dest(DEST1));
  cmpr->set_dest(DEST1, dst_pred);
  cmpr->set_src(SRC1, cmp_src2);
  cmpr->set_src(SRC2, cmp_src1);
  cmpr->set_src(PRED1, op->src(PRED1));
  op_list.add_tail(cmpr);  

  return op_list;
}


List<Op*> 
arm_op_convert_cmpr_llt(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();
  Operand cmp_src1 = op->src(SRC1);
  Operand cmp_src2 = op->src(SRC2);
  
  Op* cmpr = new Op((Opcode)CMPR_W_LGT);
  assert(cmpr && "out of memory");
  cmpr->set_dest(DEST1, op->dest(DEST1));
  cmpr->set_src(SRC1, cmp_src2);
  cmpr->set_src(SRC2, cmp_src1);
  cmpr->set_src(PRED1, op->src(PRED1));
  op_list.add_tail(cmpr);

  return op_list;
}


/* 
 * Convert div_w r1, r2, r3 to a bl which calls gcc's __divsi3 function
 */
List<Op*> 
arm_op_convert_div_w(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  Operand div_src1 = op->src(SRC1);
  Operand div_src2 = op->src(SRC2);
  Operand true_pred(new Pred_lit(true));

  // generate a mov op for DIV_W src1 to a1
  Operand move_src1_a1_src(op->src(SRC1));
  Operand move_src1_a1_dest(new Macro_reg(INT_PARAM_1));
  Operand move_src1_a1_pred(true_pred);

  Op *move_src1_a1_op = new Op(MOVE);
  move_src1_a1_op->set_src(SRC1, move_src1_a1_src);
  move_src1_a1_op->set_src(PRED1, move_src1_a1_pred);
  move_src1_a1_op->set_dest(DEST1, move_src1_a1_dest);
  op_list.add_tail(move_src1_a1_op);  

  // generate a mov op for div src2 to a a2
  Operand move_src2_a2_src(op->src(SRC2));
  Operand move_src2_a2_dest(new Macro_reg(INT_PARAM_2));
  Operand move_src2_a2_pred(true_pred);

  Op *move_src2_a2_op = new Op(MOVE);
  move_src2_a2_op->set_src(SRC1, move_src2_a2_src);
  move_src2_a2_op->set_src(PRED1, move_src2_a2_pred);
  move_src2_a2_op->set_dest(DEST1, move_src2_a2_dest);
  op_list.add_tail(move_src2_a2_op);  

  const eString target_name("_$fn___divsi3");
  Reg* btr_reg = new Reg(EL_STANDARD_BRANCH, BTR, 
			 virtual_register_number++);
  Int_lit *int_lit = new Int_lit(1);
  Label_lit *target = new Label_lit(target_name);
  
  Op *pbr_op = new Op(PBR);
  Operand pbr_src1(target);
  Operand pbr_src2(int_lit);
  Operand pbr_pred(new Pred_lit(true));
  Operand pbr_dest(btr_reg);

  pbr_op->set_dest(DEST1, pbr_dest);
  pbr_op->set_src(SRC1, pbr_src1);
  pbr_op->set_src(SRC2, pbr_src2);
  pbr_op->set_src(PRED1, pbr_pred);
  op_list.add_tail(pbr_op);  

  Operand brl_src(new Reg(EL_STANDARD_BRANCH, BTR, 
			  (btr_reg)->vr_num()));
  Operand brl_pred(new Pred_lit(true));
  Operand brl_dest(new Macro_reg(RETURN_ADDR));
  Op *brl_op = new Op(BRL);

  brl_op->set_src(SRC1, brl_src);
  brl_op->set_src(PRED1, brl_pred);
  brl_op->set_dest(DEST1, brl_dest);

  // set up the brl attributes
  Lcode_attribute_map* attr_map = new Lcode_attribute_map ();

  eString tr_attr_str("tr");
  List<Operand> tr_operand_list;
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_1));
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_2));
  attr_map->bind(tr_attr_str, tr_operand_list);

  eString ret_attr_str("ret");
  List<Operand> ret_operand_list;
  ret_operand_list.add_tail(new Macro_reg(INT_RETURN));
  attr_map->bind(ret_attr_str, ret_operand_list);

  set_lcode_attributes(brl_op, attr_map);

  Branch_target_attr* bta = new Branch_target_attr();
  assert(bta && "no more memory");
  bta->label = "__divsi3";
  set_branch_target_attr(brl_op, bta);

  op_list.add_tail(brl_op);

  // move the result into the dest register from the div instruction
  Operand move_a1_dest_src(new Macro_reg(INT_RETURN));
  Operand move_a1_dest_dest(op->dest(DEST1));
  Operand move_a1_dest_pred(true_pred);
  
  Op *move_a1_dest_op = new Op(MOVE);
  move_a1_dest_op->set_src(SRC1, move_a1_dest_src);
  move_a1_dest_op->set_src(PRED1, move_a1_dest_pred);
  move_a1_dest_op->set_dest(DEST1, move_a1_dest_dest);
  op_list.add_tail(move_a1_dest_op);  

  return op_list;
}

/* 
 * Convert divl_w r1, r2, r3 to a bl which calls gcc's __udivsi3 function
 */
List<Op*> 
arm_op_convert_divl_w(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  Operand div_src1 = op->src(SRC1);
  Operand div_src2 = op->src(SRC2);
  Operand true_pred(new Pred_lit(true));

  // generate a mov op for DIV_W src1 to a1
  Operand move_src1_a1_src(op->src(SRC1));
  Operand move_src1_a1_dest(new Macro_reg(INT_PARAM_1));
  Operand move_src1_a1_pred(true_pred);

  Op *move_src1_a1_op = new Op(MOVE);
  move_src1_a1_op->set_src(SRC1, move_src1_a1_src);
  move_src1_a1_op->set_src(PRED1, move_src1_a1_pred);
  move_src1_a1_op->set_dest(DEST1, move_src1_a1_dest);
  op_list.add_tail(move_src1_a1_op);  

  // generate a mov op for div src2 to a a2
  Operand move_src2_a2_src(op->src(SRC2));
  Operand move_src2_a2_dest(new Macro_reg(INT_PARAM_2));
  Operand move_src2_a2_pred(true_pred);

  Op *move_src2_a2_op = new Op(MOVE);
  move_src2_a2_op->set_src(SRC1, move_src2_a2_src);
  move_src2_a2_op->set_src(PRED1, move_src2_a2_pred);
  move_src2_a2_op->set_dest(DEST1, move_src2_a2_dest);
  op_list.add_tail(move_src2_a2_op);  

  const eString target_name("_$fn___udivsi3");
  Reg* btr_reg = new Reg(EL_STANDARD_BRANCH, BTR, 
			 virtual_register_number++);
  Int_lit *int_lit = new Int_lit(1);
  Label_lit *target = new Label_lit(target_name);
  
  Op *pbr_op = new Op(PBR);
  Operand pbr_src1(target);
  Operand pbr_src2(int_lit);
  Operand pbr_pred(new Pred_lit(true));
  Operand pbr_dest(btr_reg);

  pbr_op->set_dest(DEST1, pbr_dest);
  pbr_op->set_src(SRC1, pbr_src1);
  pbr_op->set_src(SRC2, pbr_src2);
  pbr_op->set_src(PRED1, pbr_pred);
  op_list.add_tail(pbr_op);  

  Operand brl_src(new Reg(EL_STANDARD_BRANCH, BTR, 
			  (btr_reg)->vr_num()));
  Operand brl_pred(new Pred_lit(true));
  Operand brl_dest(new Macro_reg(RETURN_ADDR));
  Op *brl_op = new Op(BRL);

  brl_op->set_src(SRC1, brl_src);
  brl_op->set_src(PRED1, brl_pred);
  brl_op->set_dest(DEST1, brl_dest);

  // set up the brl attributes
  Lcode_attribute_map* attr_map = new Lcode_attribute_map ();

  eString tr_attr_str("tr");
  List<Operand> tr_operand_list;
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_1));
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_2));
  attr_map->bind(tr_attr_str, tr_operand_list);

  eString ret_attr_str("ret");
  List<Operand> ret_operand_list;
  ret_operand_list.add_tail(new Macro_reg(INT_RETURN));
  attr_map->bind(ret_attr_str, ret_operand_list);

  set_lcode_attributes(brl_op, attr_map);

  Branch_target_attr* bta = new Branch_target_attr();
  assert(bta && "no more memory");
  bta->label = "__udivsi3";
  set_branch_target_attr(brl_op, bta);

  op_list.add_tail(brl_op);

  // move the result into the dest register from the div instruction
  Operand move_a1_dest_src(new Macro_reg(INT_RETURN));
  Operand move_a1_dest_dest(op->dest(DEST1));
  Operand move_a1_dest_pred(true_pred);
  
  Op *move_a1_dest_op = new Op(MOVE);
  move_a1_dest_op->set_src(SRC1, move_a1_dest_src);
  move_a1_dest_op->set_src(PRED1, move_a1_dest_pred);
  move_a1_dest_op->set_dest(DEST1, move_a1_dest_dest);
  op_list.add_tail(move_a1_dest_op);  

  return op_list;
}


/* 
 * Convert exts_b r1, r2 to:
 *   r1 = r2 << 24 (logical shift)
 *   r1 = r2 >> 24 (arithmetic shift)
 */
List<Op*> 
arm_op_convert_exts_b(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  Operand exts_src = op->src(SRC1);
  Operand exts_dest = op->dest(DEST1);
  Operand true_pred(new Pred_lit(true));
  Operand tmp_reg(new Reg(EL_STANDARD_INT));

  // generate the first shift operation
  Operand shift1_src1(exts_src);
  Operand shift1_src2(new Int_lit(24));
  Operand shift1_dest(tmp_reg);
  Operand shift1_pred(op->src(PRED1));

  Op *shift1_op = new Op(SHL_W);
  shift1_op->set_src(SRC1, shift1_src1);
  shift1_op->set_src(SRC2, shift1_src2);
  shift1_op->set_src(PRED1, shift1_pred);
  shift1_op->set_dest(DEST1, shift1_dest);
  op_list.add_tail(shift1_op);

  // generate the second shift operation
  Operand shift2_src2(new Int_lit(24));
  Operand shift2_dest(exts_dest);
  Operand shift2_pred(op->src(PRED1));

  Op *shift2_op = new Op(SHRA_W);
  shift2_op->set_src(SRC1, tmp_reg);
  shift2_op->set_src(SRC2, shift2_src2);
  shift2_op->set_src(PRED1, shift2_pred);
  shift2_op->set_dest(DEST1, shift2_dest);
  op_list.add_tail(shift2_op);
    
  return op_list;
}

/* 
 * Convert exts_h r1, r2 to:
 *   r1 = r2 << 16 (logical shift)
 *   r1 = r2 >> 16 (arithmetic shift)
 */
List<Op*> 
arm_op_convert_exts_h(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  Operand exts_src = op->src(SRC1);
  Operand exts_dest = op->dest(DEST1);
  Operand true_pred(new Pred_lit(true));
  Operand tmp_reg(new Reg(EL_STANDARD_INT));

  // generate the first shift operation
  Operand shift1_src1(exts_src);
  Operand shift1_src2(new Int_lit(16));
  Operand shift1_dest(tmp_reg);
  Operand shift1_pred(op->src(PRED1));

  Op *shift1_op = new Op(SHL_W);
  shift1_op->set_src(SRC1, shift1_src1);
  shift1_op->set_src(SRC2, shift1_src2);
  shift1_op->set_src(PRED1, shift1_pred);
  shift1_op->set_dest(DEST1, tmp_reg);
  op_list.add_tail(shift1_op);

  // generate the second shift operation
  Operand shift2_src1(tmp_reg);
  Operand shift2_src2(new Int_lit(16));
  Operand shift2_dest(exts_dest);
  Operand shift2_pred(op->src(PRED1));

  Op *shift2_op = new Op(SHRA_W);
  shift2_op->set_src(SRC1, shift2_src1);
  shift2_op->set_src(SRC2, shift2_src2);
  shift2_op->set_src(PRED1, shift2_pred);
  shift2_op->set_dest(DEST1, shift2_dest);
  op_list.add_tail(shift2_op);
    
  return op_list;
}

/* 
 * Convert extz_b r1, r2 to:
 *   r1 = r2 & 255
 */
List<Op*> 
arm_op_convert_extz_b(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  // create the left shift operation
  Op *and_op = new Op(AND_W);
  assert(and_op && "no memory");
  Operand and_src1 = op->src(SRC1);
  Operand and_src2(new Int_lit(255));

  and_op->set_src(SRC1, and_src1);
  and_op->set_src(SRC2, and_src2);
  and_op->set_dest(DEST1, op->dest(DEST1));
  and_op->set_src(PRED1, op->src(PRED1));
  op_list.add_tail(and_op);  

  return op_list;
}

/* 
 * Convert extz_h r1, r2 to:
 *   r1 = r2 & 65535
 */
List<Op*> 
arm_op_convert_extz_h(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  // create the left shift operation
  Op *and_op = new Op(AND_W);
  assert(and_op && "no memory");
  Operand and_src1 = op->src(SRC1);
  Operand and_src2(new Int_lit(65535));

  and_op->set_src(SRC1, and_src1);
  and_op->set_src(SRC2, and_src2);
  and_op->set_dest(DEST1, op->dest(DEST1));
  and_op->set_src(PRED1, op->src(PRED1));
  op_list.add_tail(and_op);  
    
  return op_list;
}

/* 
 * Convert rem_w r1, r2, r3 to a bl which calls gcc's _modsi3 function
 */
List<Op*> 
arm_op_convert_rem_w(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  Operand rem_src1 = op->src(SRC1);
  Operand rem_src2 = op->src(SRC2);
  Operand rem_pred(op->src(PRED1));

  // generate a mov op for REM_W src1 to a1
  Operand move_src1_a1_src(op->src(SRC1));
  Operand move_src1_a1_dest(new Macro_reg(INT_PARAM_1));
  Operand move_src1_a1_pred(rem_pred);

  Op *move_src1_a1_op = new Op(MOVE);
  move_src1_a1_op->set_src(SRC1, move_src1_a1_src);
  move_src1_a1_op->set_src(PRED1, move_src1_a1_pred);
  move_src1_a1_op->set_dest(DEST1, move_src1_a1_dest);
  op_list.add_tail(move_src1_a1_op);  

  // generate a mov op for REM src2 to a a2
  Operand move_src2_a2_src(op->src(SRC2));
  Operand move_src2_a2_dest(new Macro_reg(INT_PARAM_2));
  Operand move_src2_a2_pred(rem_pred);

  Op *move_src2_a2_op = new Op(MOVE);
  move_src2_a2_op->set_src(SRC1, move_src2_a2_src);
  move_src2_a2_op->set_src(PRED1, move_src2_a2_pred);
  move_src2_a2_op->set_dest(DEST1, move_src2_a2_dest);
  op_list.add_tail(move_src2_a2_op);  

  const eString target_name("_$fn___modsi3");
  Reg* btr_reg = new Reg(EL_STANDARD_BRANCH, BTR, 
			 virtual_register_number++);
  Int_lit *int_lit = new Int_lit(1);
  Label_lit *target = new Label_lit(target_name);
  
  Op *pbr_op = new Op(PBR);
  Operand pbr_src1(target);
  Operand pbr_src2(int_lit);
  Operand pbr_pred(op->src(PRED1));
  Operand pbr_dest(btr_reg);

  pbr_op->set_dest(DEST1, pbr_dest);
  pbr_op->set_src(SRC1, pbr_src1);
  pbr_op->set_src(SRC2, pbr_src2);
  pbr_op->set_src(PRED1, pbr_pred);
  op_list.add_tail(pbr_op);  

  Operand brl_src(new Reg(EL_STANDARD_BRANCH, BTR, 
			  (btr_reg)->vr_num()));
  Operand brl_pred(op->src(PRED1));
  Operand brl_dest(new Macro_reg(RETURN_ADDR));
  Op *brl_op = new Op(BRL);

  brl_op->set_src(SRC1, brl_src);
  brl_op->set_src(PRED1, brl_pred);
  brl_op->set_dest(DEST1, brl_dest);

  // set up the brl attributes
  Lcode_attribute_map* attr_map = new Lcode_attribute_map ();

  eString tr_attr_str("tr");
  List<Operand> tr_operand_list;
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_1));
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_2));
  attr_map->bind(tr_attr_str, tr_operand_list);

  eString ret_attr_str("ret");
  List<Operand> ret_operand_list;
  ret_operand_list.add_tail(new Macro_reg(INT_RETURN));
  attr_map->bind(ret_attr_str, ret_operand_list);

  set_lcode_attributes(brl_op, attr_map);

  Branch_target_attr* bta = new Branch_target_attr();
  assert(bta && "no more memory");
  bta->label = "__modsi3";
  set_branch_target_attr(brl_op, bta);

  op_list.add_tail(brl_op);

  // move the result into the dest register from the REM instruction
  Operand move_a1_dest_src(new Macro_reg(INT_RETURN));
  Operand move_a1_dest_dest(op->dest(DEST1));
  Operand move_a1_dest_pred(rem_pred);
  
  Op *move_a1_dest_op = new Op(MOVE);
  move_a1_dest_op->set_src(SRC1, move_a1_dest_src);
  move_a1_dest_op->set_src(PRED1, move_a1_dest_pred);
  move_a1_dest_op->set_dest(DEST1, move_a1_dest_dest);
  op_list.add_tail(move_a1_dest_op);  

  return op_list;
}


/* 
 * Convert rem_w r1, r2, r3 to a bl which calls gcc's _umodsi3 function
 */
List<Op*> 
arm_op_convert_reml_w(Op *op) 
{
  List<Op*> op_list;
  op_list.clear();

  Operand rem_src1 = op->src(SRC1);
  Operand rem_src2 = op->src(SRC2);
  Operand rem_pred(op->src(PRED1));

  // generate a mov op for REM_W src1 to a1
  Operand move_src1_a1_src(op->src(SRC1));
  Operand move_src1_a1_dest(new Macro_reg(INT_PARAM_1));
  Operand move_src1_a1_pred(rem_pred);

  Op *move_src1_a1_op = new Op(MOVE);
  move_src1_a1_op->set_src(SRC1, move_src1_a1_src);
  move_src1_a1_op->set_src(PRED1, move_src1_a1_pred);
  move_src1_a1_op->set_dest(DEST1, move_src1_a1_dest);
  op_list.add_tail(move_src1_a1_op);  

  // generate a mov op for REM src2 to a a2
  Operand move_src2_a2_src(op->src(SRC2));
  Operand move_src2_a2_dest(new Macro_reg(INT_PARAM_2));
  Operand move_src2_a2_pred(rem_pred);

  Op *move_src2_a2_op = new Op(MOVE);
  move_src2_a2_op->set_src(SRC1, move_src2_a2_src);
  move_src2_a2_op->set_src(PRED1, move_src2_a2_pred);
  move_src2_a2_op->set_dest(DEST1, move_src2_a2_dest);
  op_list.add_tail(move_src2_a2_op);  

  const eString target_name("_$fn___umodsi3");
  Reg* btr_reg = new Reg(EL_STANDARD_BRANCH, BTR, 
			 virtual_register_number++);
  Int_lit *int_lit = new Int_lit(1);
  Label_lit *target = new Label_lit(target_name);
  
  Op *pbr_op = new Op(PBR);
  Operand pbr_src1(target);
  Operand pbr_src2(int_lit);
  Operand pbr_pred(op->src(PRED1));
  Operand pbr_dest(btr_reg);

  pbr_op->set_dest(DEST1, pbr_dest);
  pbr_op->set_src(SRC1, pbr_src1);
  pbr_op->set_src(SRC2, pbr_src2);
  pbr_op->set_src(PRED1, pbr_pred);
  op_list.add_tail(pbr_op);  

  Operand brl_src(new Reg(EL_STANDARD_BRANCH, BTR, 
			  (btr_reg)->vr_num()));
  Operand brl_pred(op->src(PRED1));
  Operand brl_dest(new Macro_reg(RETURN_ADDR));
  Op *brl_op = new Op(BRL);

  brl_op->set_src(SRC1, brl_src);
  brl_op->set_src(PRED1, brl_pred);
  brl_op->set_dest(DEST1, brl_dest);

  // set up the brl attributes
  Lcode_attribute_map* attr_map = new Lcode_attribute_map ();

  eString tr_attr_str("tr");
  List<Operand> tr_operand_list;
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_1));
  tr_operand_list.add_tail(new Macro_reg(INT_PARAM_2));
  attr_map->bind(tr_attr_str, tr_operand_list);

  eString ret_attr_str("ret");
  List<Operand> ret_operand_list;
  ret_operand_list.add_tail(new Macro_reg(INT_RETURN));
  attr_map->bind(ret_attr_str, ret_operand_list);

  set_lcode_attributes(brl_op, attr_map);

  Branch_target_attr* bta = new Branch_target_attr();
  assert(bta && "no more memory");
  bta->label = "__umodsi3";
  set_branch_target_attr(brl_op, bta);

  op_list.add_tail(brl_op);

  // move the result into the dest register from the REM instruction
  Operand move_a1_dest_src(new Macro_reg(INT_RETURN));
  Operand move_a1_dest_dest(op->dest(DEST1));
  Operand move_a1_dest_pred(rem_pred);
  
  Op *move_a1_dest_op = new Op(MOVE);
  move_a1_dest_op->set_src(SRC1, move_a1_dest_src);
  move_a1_dest_op->set_src(PRED1, move_a1_dest_pred);
  move_a1_dest_op->set_dest(DEST1, move_a1_dest_dest);
  op_list.add_tail(move_a1_dest_op);  

  return op_list;
}


void mark_branch_targets(Procedure* f)
{
  Dlist<Region*> rstack;
  Region *rtmp;

  rstack.push_tail(f);
  while (!rstack.is_empty()) {
    rtmp = rstack.pop();
     
    if (rtmp->is_hb() || rtmp->is_bb()) {       

      for(Region_ops_C0_order rops((Compound_region*)rtmp);rops != 0;rops++){
	Op* op = *rops;

	if (!is_pseudo(op)) {

	  if((is_brl(op) || is_branch(op)) && !is_rts(op)) {
	      
	    if(get_branch_target_attr(op) != NULL)
	      continue;

	    // find the target and put it in an attribute.
	    Operand operand = op->src(SRC1);
	    string label;
	    if(operand.is_label()) {
	      label = operand.label_value().strdup();
		
	      // strip the prefix _$fn_ off of library calls in the 
	      // rebel code
	      if (!strncmp(label.c_str(), "_$fn_", 5)) {
		label = label.substr(5, label.length() - 1);
	      }
	      Branch_target_attr* bta = new Branch_target_attr();
	      assert(bta);
	      bta->label = label;
	      set_branch_target_attr(op, bta);
	    }
	    else if(operand.is_lit()) {
	      if(operand.is_label()) {
		int bbnum = op->src(SRC1).int_value();
		char bbnum_str[80];
		  
		sprintf(bbnum_str, "%d", bbnum);
		  
		Region *parent = op->parent();
		  
		while (!parent->is_procedure())
		  parent = parent->parent();
		string proc_name = ((Procedure*)parent)->get_name();
		  
		label = (proc_name + "_bb_" + bbnum_str);
		Branch_target_attr* bta = new Branch_target_attr();
		assert(bta);
		bta->label = label;
		set_branch_target_attr(op, bta);
	      }
		
	    } else {
	      Op* in_op = El_find_pbr_for_branch(op);
	      if(is_pbr(in_op)) {
		if(in_op->src(SRC1).is_label()) {
		  label = in_op->src(SRC1).label_value().strdup();
		    
		  // strip the prefix _$fn_ off of library calls 
		  // in the rebel code
		  if (!strncmp(label.c_str(), "_$fn_", 5)) {
		    label = label.substr(5, label.length() - 1);
		  }
		  Branch_target_attr* bta = new Branch_target_attr();
		  assert(bta);
		  bta->label = label;
		  set_branch_target_attr(op, bta);
		}
		else if (in_op->src(SRC1).is_lit()) {
		  int bbnum =  in_op->src(SRC1).int_value();
		  char bbnum_str[20];
		    
		  sprintf(bbnum_str, "%d", bbnum);
		    
		  Region *parent = op->parent();
		    
		  while (!parent->is_procedure())
		    parent = parent->parent();
		  string proc_name = ((Procedure*)parent)->get_name();
		    
		  label = (proc_name + "_bb_" + bbnum_str);
		  Branch_target_attr* bta = new Branch_target_attr();
		  assert(bta);
		  bta->label = label;
		  set_branch_target_attr(op, bta);
		}
		else if(in_op->src(SRC1).is_reg()) {
		  // This is an indirect branch. Since PBRs don't
		  // exist in ARM, we need to make sure the register
		  // sourced here is still live when it gets to the
		  // BRL.
		  List<Operand>* impl_uses = get_implicit_use_list(op);
		  if(impl_uses == NULL) {
		    impl_uses = new List<Operand>();
		    assert(impl_uses && "no more memory");
		  }
		  impl_uses->add_tail(in_op->src(SRC1));
		  set_implicit_use_list(op, impl_uses);
		}
	      }
	    }
	  }
	}
      }
    }
    else {
      for (Region_subregions subri(rtmp); subri!=0; subri++) {
	if ((*subri)->is_compound())
	  rstack.push_tail (*subri);
      }
    }
  }

}


static bool is_fp_param(const Operand& oper)
{
  if(oper.is_macro_reg()) {
    if(oper.name() == FLT_PARAM_1 || oper.name() == FLT_PARAM_2 ||
       oper.name() == FLT_PARAM_3 || oper.name() == FLT_PARAM_4)
      return true;
  }
  return false;
}


static bool is_dbl_param(const Operand& oper)
{
  if(oper.is_macro_reg()) {
    if(oper.name() == DBL_PARAM_1 || oper.name() == DBL_PARAM_2 ||
       oper.name() == DBL_PARAM_3 || oper.name() == DBL_PARAM_4)
      return true;
  }
  return false;
}


void arm_replace_fp_params(Procedure* f)
{
  Dlist<Region*> rstack;
  Region *rtmp;
  int fp_param_offset = -1;
  Op* local_define_op = NULL;
  int memvr_num = virtual_register_number++;
  Operand memvr_oper(new Mem_vr(memvr_num));
  memvr_num = virtual_register_number++;
  Operand memvr_oper2(new Mem_vr(memvr_num));

  Hash_set<int> IP_offset_fix(hash_int, 7);

  rstack.push_tail(f);
  while (!rstack.is_empty()) {
    rtmp = rstack.pop();
     
    if (rtmp->is_hb() || rtmp->is_bb()) {       
      Hash_set<Op*> to_remove(hash_op_ptr, 64), dont_modify(hash_op_ptr, 64);

      ////////////////////////////////////////////////////////////////////////
      // First, impact does some weird stuff with vararg arguments
      // that are floating point, in that they are redundantly stored
      // on the stack and in int param registers. ARM only puts them
      // in the int params, so we have to get rid of all the spurious
      // stack stores.
      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;
	Lcode_attribute_map* lc_map = get_lcode_attributes(op);
	if(lc_map)
	  if(lc_map->is_bound("param_spill"))
	     to_remove += op;

      }

      for(Hash_set_iterator<Op*> hmi(to_remove); hmi != 0; hmi++)
	El_remove_op(*hmi);

      to_remove.clear();


      ////////////////////////////////////////////////////////////////////////
      // Find the amount of space allocated to local variables and add
      // 8. This will be used to store/load double parameters into the
      // appropriate int param registers and back.

      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;
	if(is_define(op)) {
	  if(op->dest(DEST1).name() == LOCAL) {
	    local_define_op = op;
	    fp_param_offset = op->src(SRC1).int_value();
	    Operand new_size(new Int_lit(fp_param_offset + 8));
	    op->set_src(SRC1, new_size);
	  }
	}
      }

      if(fp_param_offset == -1) {
	cdbg << "ERROR: couldn't figure out how much local space there is!"
	     << endl;
	assert(0);
      }
      Operand param_offset_oper(new Int_lit(fp_param_offset));
      Operand param_plus_four(new Int_lit(fp_param_offset + 4));
      Operand lv_oper(new Macro_reg(LV_REG));


      ////////////////////////////////////////////////////////////////////////
      // There's a problem in that Impact thinks dbl_param1 is
      // different than flt_param1 which creates some RAW errors. This
      // pass puts some moves in the code to eliminate these
      // errors. The moves will get eliminated during copy
      // propogation, so they don't hurt performance.

      Op* first_param_write = NULL;
      Map<Operand, List<Op*> > param_users;
      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;

	if(first_param_write) {
	  for(Op_explicit_sources od(op); od != 0; od++) {
	    Operand& oper = *od;
	    if(oper.is_macro_reg()) {
	      switch(oper.name()) {
	      case FLT_PARAM_1:
	      case FLT_PARAM_2:
	      case FLT_PARAM_3:
	      case FLT_PARAM_4: {
		if( ! param_users.is_bound(oper)) {
		  param_users.bind(oper, List<Op*>());
		}
		param_users[oper].add_tail(op);
	      }
	      default:
		break;
	      }
	    }
	  }
	}

	for(Op_explicit_dests od(op); od != 0; od++) {
	  Operand& oper = *od;
	  if(oper.is_macro_reg()) {
	    switch(oper.name()) {
	    case DBL_PARAM_1:
	    case DBL_PARAM_2:
	    case DBL_PARAM_3:
	    case DBL_PARAM_4: {
	      if(first_param_write == NULL) {
		first_param_write = op;
	      }
	    }
	    default:
	      break;
	    }
	  }
	}

	// At function calls, if any parameters are doubles, and the
	// doubles have single params as the srcs, then insert some
	// moves.
	if(is_brl(op)) {
	  for(Map_iterator<Operand, List<Op*> > mi(param_users); mi != 0; mi++) {
	    List<Op*>& readers = (*mi).second;
	    Operand& param = (*mi).first;
	    
	    Op* move = new Op(MOVEF_S);
	    assert(move && "out of memory");
	    move->set_src(PRED1, readers.head()->src(PRED1));
	    move->set_src(SRC1, param);
	    Operand temp;
	    temp = new Reg(EL_STANDARD_FLOAT);
	    move->set_dest(DEST1, temp);
	    El_insert_op_before(move, first_param_write);

	    for(List_iterator<Op*> li(readers); li != 0; li++) {
	      Op* reader = *li;
	      for(Op_explicit_sources os(reader); os != 0; os++) {
		Operand& oper = *os;
		if(oper == param) {
		  reader->set_src(os.get_port_num(), temp);
		}
	      }
	    }
	  }

	  first_param_write = NULL;
	  param_users.clear();
	}
      }


      ////////////////////////////////////////////////////////////////////////
      // Look at the parameters for function calls. If any of the
      // parameters are floating point, we need to set up the
      // appropriate int params and stack stores. Stack space is
      // already reserved, we just need to fill it. In addition, we
      // update the tr attribute so register allocation knows that the
      // int params are used by BRLs.

      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;
	if(is_brl(op)) {
	  Lcode_attribute_map* attr_map = get_lcode_attributes(op);
	  eString tr_attr_str("tr");
	  if(attr_map->is_bound(tr_attr_str)) {
	    List<Operand>& tr_operand_list = (*attr_map)[tr_attr_str];
	    Hash_set<Operand> to_add(hash_operand, 8);
	    for(List_iterator<Operand> li(tr_operand_list); li != 0; li++) {
	      Operand& oper = *li;
	      if(oper.is_macro_reg()) {
		switch (oper.name()) {
		case FLT_PARAM_1:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_1));
		    to_add += new_oper;

		    Op* new_store = new Op((Opcode) FSG_S_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_load && "out of memory");
		    new_store->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);

		    new_store->add_mem_dest(memvr_oper);
		    new_load->add_mem_src(memvr_oper);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    break;
		  }
		case FLT_PARAM_2:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_2));
		    to_add += new_oper;

		    Op* new_store = new Op((Opcode) FSG_S_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_load && "out of memory");
		    new_store->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);

		    new_store->add_mem_dest(memvr_oper);
		    new_load->add_mem_src(memvr_oper);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    break;
		  }
		case FLT_PARAM_3:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_3));
		    to_add += new_oper;

		    Op* new_store = new Op((Opcode) FSG_S_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_load && "out of memory");
		    new_store->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);

		    new_store->add_mem_dest(memvr_oper);
		    new_load->add_mem_src(memvr_oper);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    break;
		  }
		case FLT_PARAM_4:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_4));
		    to_add += new_oper;

		    Op* new_store = new Op((Opcode) FSG_S_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_load && "out of memory");
		    new_store->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);

		    new_store->add_mem_dest(memvr_oper);
		    new_load->add_mem_src(memvr_oper);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    break;
		  }
		case DBL_PARAM_1:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_1));
		    Operand new_oper_2(new Macro_reg(INT_PARAM_2));
		    to_add += new_oper;
		    to_add += new_oper_2;

		    Op* new_store = new Op((Opcode) FSG_D_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    Op* new_load2 = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_load && new_load2 && "out of memory");
		    new_store->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_load2->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);
		    new_load2->set_src(SRC1, lv_oper);
		    new_load2->set_src(SRC2, param_plus_four);
		    new_load2->set_dest(DEST1, new_oper_2);

		    new_store->add_mem_dest(memvr_oper);
		    new_store->add_mem_dest(memvr_oper2);
		    new_load->add_mem_src(memvr_oper);
		    new_load2->add_mem_src(memvr_oper2);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    El_insert_op_before(new_load2, op);
		    break;
		  }
		case DBL_PARAM_2:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_2));
		    Operand new_oper_2(new Macro_reg(INT_PARAM_3));
		    to_add += new_oper;
		    to_add += new_oper_2;

		    Op* new_store = new Op((Opcode) FSG_D_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    Op* new_load2 = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_load && new_load2 && "out of memory");
		    new_store->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_load2->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);
		    new_load2->set_src(SRC1, lv_oper);
		    new_load2->set_src(SRC2, param_plus_four);
		    new_load2->set_dest(DEST1, new_oper_2);

		    new_store->add_mem_dest(memvr_oper);
		    new_store->add_mem_dest(memvr_oper2);
		    new_load->add_mem_src(memvr_oper);
		    new_load2->add_mem_src(memvr_oper2);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    El_insert_op_before(new_load2, op);
		    break;
		  }
		case DBL_PARAM_3:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_3));
		    Operand new_oper_2(new Macro_reg(INT_PARAM_4));
		    to_add += new_oper;
		    to_add += new_oper_2;

		    Op* new_store = new Op((Opcode) FSG_D_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    Op* new_load2 = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_load && new_load2 && "out of memory");
		    new_store->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_load2->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);
		    new_load2->set_src(SRC1, lv_oper);
		    new_load2->set_src(SRC2, param_plus_four);
		    new_load2->set_dest(DEST1, new_oper_2);

		    new_store->add_mem_dest(memvr_oper);
		    new_store->add_mem_dest(memvr_oper2);
		    new_load->add_mem_src(memvr_oper);
		    new_load2->add_mem_src(memvr_oper2);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    El_insert_op_before(new_load2, op);
		    break;
		  }
		case DBL_PARAM_4:
		  {
		    Operand new_oper(new Macro_reg(INT_PARAM_4));
		    to_add += new_oper;

		    Operand tmp_reg(new Reg(EL_STANDARD_INT));
		    Op* new_store = new Op((Opcode) FSG_D_C1);
		    Op* new_store2 = new Op((Opcode) S_W_C1);
		    Op* new_load = new Op((Opcode) LG_W_C1_C1);
		    Op* new_load2 = new Op((Opcode) LG_W_C1_C1);
		    assert(new_store && new_store2 && new_load && new_load2 &&
			   "out of memory");

		    new_store->set_src(PRED1, op->src(PRED1));
		    new_store2->set_src(PRED1, op->src(PRED1));
		    new_load->set_src(PRED1, op->src(PRED1));
		    new_load2->set_src(PRED1, op->src(PRED1));
		    new_store->set_src(SRC1, lv_oper);
		    new_store->set_src(SRC2, param_offset_oper);
		    new_store->set_src(SRC3, oper);
		    new_load->set_src(SRC1, lv_oper);
		    new_load->set_src(SRC2, param_offset_oper);
		    new_load->set_dest(DEST1, new_oper);
		    new_load2->set_src(SRC1, lv_oper);
		    new_load2->set_src(SRC2, param_plus_four);
		    new_load2->set_dest(DEST1, tmp_reg);
		    new_store2->set_src(SRC2, tmp_reg);
		    Operand op_reg(new Macro_reg(OP_REG));
		    new_store2->set_src(SRC1, op_reg);

		    new_store->add_mem_dest(memvr_oper);
		    new_store->add_mem_dest(memvr_oper2);
		    new_load->add_mem_src(memvr_oper);
		    new_load2->add_mem_src(memvr_oper2);

		    El_insert_op_before(new_store, op);
		    El_insert_op_before(new_load, op);
		    El_insert_op_before(new_load2, op);
		    El_insert_op_before(new_store2, op);
		    break;
		  }
		default:
		  break;
		}
	      }
	    }
	    
	    for(Hash_set_iterator<Operand> hsi(to_add); hsi != 0; hsi++) {
	      tr_operand_list.add_tail(*hsi);
	    }

	  }
	} // if is_brl(op)
      }

      ////////////////////////////////////////////////////////////////////////
      // At function call entry, re-setup the floating point parameters.
      
      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;
	Operand pred_t(new Pred_lit(true));

	// floating point parameters are passed through the int
	// params, so if this function uses fp params, we need to have
	// them set up properly.
	if(is_define(op)) {
	  Operand pred_t(new Pred_lit(true));
	  if(op->dest(DEST1).is_macro_reg()) {
	    switch(op->dest(DEST1).name()) {
	    case FLT_PARAM_1:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_S_C1_C1);
		assert(new_store && new_load && "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_1));
		new_store->set_src(SRC3, new_oper);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
	      }
	      break;
	    case FLT_PARAM_2:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_S_C1_C1);
		assert(new_store && new_load && "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_2));
		new_store->set_src(SRC3, new_oper);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
	      }
	      break;
	    case FLT_PARAM_3:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_S_C1_C1);
		assert(new_store && new_load && "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_3));
		new_store->set_src(SRC3, new_oper);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
	      }
	      break;
	    case FLT_PARAM_4:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_S_C1_C1);
		assert(new_store && new_load && "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_4));
		new_store->set_src(SRC3, new_oper);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
	      }
	      break;
	    case DBL_PARAM_1:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_store2 = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_D_C1_C1);
		assert(new_store && new_store2 && new_load && "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_store2->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_1));
		new_store->set_src(SRC3, new_oper);
		new_store2->set_src(SRC1, lv_oper);
		new_store2->set_src(SRC2, param_plus_four);
		Operand new_oper2(new Macro_reg(INT_PARAM_2));
		new_store2->set_src(SRC3, new_oper2);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);
		new_store2->add_mem_dest(memvr_oper2);
		new_load->add_mem_src(memvr_oper2);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
		El_insert_op_before(new_store2, new_load);
	      }
	      break;
	    case DBL_PARAM_2:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_store2 = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_D_C1_C1);
		assert(new_store && new_store2 && new_load && "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_store2->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_2));
		new_store->set_src(SRC3, new_oper);
		new_store2->set_src(SRC1, lv_oper);
		new_store2->set_src(SRC2, param_plus_four);
		Operand new_oper2(new Macro_reg(INT_PARAM_3));
		new_store2->set_src(SRC3, new_oper2);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);
		new_store2->add_mem_dest(memvr_oper2);
		new_load->add_mem_src(memvr_oper2);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
		El_insert_op_before(new_store2, new_load);
	      }
	      break;
	    case DBL_PARAM_3:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_store2 = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_D_C1_C1);
		assert(new_store && new_store2 && new_load && "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_store2->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_3));
		new_store->set_src(SRC3, new_oper);
		new_store2->set_src(SRC1, lv_oper);
		new_store2->set_src(SRC2, param_plus_four);
		Operand new_oper2(new Macro_reg(INT_PARAM_4));
		new_store2->set_src(SRC3, new_oper2);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);
		new_store2->add_mem_dest(memvr_oper2);
		new_load->add_mem_src(memvr_oper2);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
		El_insert_op_before(new_store2, new_load);
	      }
	      break;
	    case DBL_PARAM_4:
	      {
		Op* new_store = new Op((Opcode) SG_W_C1);
		Op* new_store2 = new Op((Opcode) SG_W_C1);
		Op* new_load = new Op((Opcode) FLG_D_C1_C1);
		Op* new_load2 = new Op((Opcode) L_W_C1_C1);
		assert(new_store && new_store2 && new_load && new_load2 && 
		       "out of memory");
		new_store->set_src(PRED1, pred_t);
		new_store2->set_src(PRED1, pred_t);
		new_load->set_src(PRED1, pred_t);
		new_load2->set_src(PRED1, pred_t);
		new_store->set_src(SRC1, lv_oper);
		new_store->set_src(SRC2, param_offset_oper);
		Operand new_oper(new Macro_reg(INT_PARAM_4));
		new_store->set_src(SRC3, new_oper);
		Operand new_oper2(new Reg(EL_STANDARD_INT));
		new_load2->set_dest(DEST1, new_oper2);
		Operand ip_reg(new Macro_reg(IP_REG));
		new_load2->set_src(SRC1, ip_reg);
		new_store2->set_src(SRC1, lv_oper);
		new_store2->set_src(SRC2, param_plus_four);
		new_store2->set_src(SRC3, new_oper2);
		new_load->set_src(SRC1, lv_oper);
		new_load->set_src(SRC2, param_offset_oper);
		new_load->set_dest(DEST1, op->dest(DEST1));
	      
		new_store->add_mem_dest(memvr_oper);
		new_load->add_mem_src(memvr_oper);
		new_store2->add_mem_dest(memvr_oper2);
		new_load->add_mem_src(memvr_oper2);

		El_insert_op_after(new_load, op);
		El_insert_op_before(new_store, new_load);
		El_insert_op_before(new_load2, new_load);
		El_insert_op_before(new_store2, new_load);
	      }
	      break;
	    default:
	      break;
	    }
	  }
	}
      }

      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;
	// change all the double params into their single precision equivalent
	for(Op_explicit_sources oes(op); oes != 0; oes++) {
	  if((*oes).is_macro_reg()) {
	    switch ((*oes).name()) {
	    case DBL_PARAM_1:
	      {
		Operand new_src(new Macro_reg(FLT_PARAM_1));
		oes.get_op()->set_src(oes.get_port_num(), new_src);
		break;
	      }
	    case DBL_PARAM_2:
	      {
		Operand new_src(new Macro_reg(FLT_PARAM_2));
		oes.get_op()->set_src(oes.get_port_num(), new_src);
		break;
	      }
	    case DBL_PARAM_3:
	      {
		Operand new_src(new Macro_reg(FLT_PARAM_3));
		oes.get_op()->set_src(oes.get_port_num(), new_src);
		break;
	      }
	    case DBL_PARAM_4:
	      {
		Operand new_src(new Macro_reg(FLT_PARAM_4));
		oes.get_op()->set_src(oes.get_port_num(), new_src);
		break;
	      }
	    case DBL_RETURN:
	      {
		Operand new_src(new Macro_reg(FLT_RETURN));
		oes.get_op()->set_src(oes.get_port_num(), new_src);
		break;		
	      }
	    default:
	      break;
	    }
	  }
	}

	for(Op_explicit_dests oed(op); oed != 0; oed++) {
	  if((*oed).is_macro_reg()) {
	    switch ((*oed).name()) {
	    case DBL_PARAM_1:
	      {
		Operand new_dst(new Macro_reg(FLT_PARAM_1));
		oed.get_op()->set_dest(oed.get_port_num(), new_dst);
		break;
	      }
	    case DBL_PARAM_2:
	      {
		Operand new_dst(new Macro_reg(FLT_PARAM_2));
		oed.get_op()->set_dest(oed.get_port_num(), new_dst);
		break;
	      }
	    case DBL_PARAM_3:
	      {
		Operand new_dst(new Macro_reg(FLT_PARAM_3));
		oed.get_op()->set_dest(oed.get_port_num(), new_dst);
		break;
	      }
	    case DBL_PARAM_4:
	      {
		Operand new_dst(new Macro_reg(FLT_PARAM_4));
		oed.get_op()->set_dest(oed.get_port_num(), new_dst);
		break;
	      }
	    case DBL_RETURN:
	      {
		Operand new_dst(new Macro_reg(FLT_RETURN));
		oed.get_op()->set_dest(oed.get_port_num(), new_dst);
		break;		
	      }
	    default:
	      break;
	    }
	  }
	}
      } // end for all ops in this block

      for(Hash_set_iterator<Op*> hsi(to_remove); hsi != 0; hsi++) {
	El_remove_op(*hsi);
	delete *hsi;
      }


      ////////////////////////////////////////////////////////////////////////
      // insert memvrs for LV relative spills, so scheduling doesn't
      // improperly reorder our new loads/stores. We already added a
      // bunch for flow dependences; these are for output
      // dependences. FIX: this is overly conservative, and just a bad
      // hack to get things working.

      int memvr_num = virtual_register_number++;
      Operand cur_memvr(new Mem_vr(memvr_num));
      bool first_time = true;
      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;
	if(is_store(op) || is_load(op)) {
	  if(op->src(SRC1).is_macro_reg() && (op->src(SRC1).name() == LV_REG)) {
	    if(first_time)
	      first_time = false;
	    else
	      op->add_mem_src(cur_memvr);
	    memvr_num = virtual_register_number++;
	    cur_memvr = new Mem_vr(memvr_num);
	    op->add_mem_dest(cur_memvr);
	  }
	} // if is_store || is_load
      } // for all ops

    } else {
      for (Region_subregions subri(rtmp); subri!=0; subri++) {
	if ((*subri)->is_compound())
	  rstack.push_tail (*subri);
      }
    }
  }
}


static void bind_and_cluster(Op* new_op, Op* old_op)
{
  // Assign to physical file
  List_set<Op_descr*> mv_uspc_ops;
  if (Cluster_mgr::op_map.is_bound(old_op)) {
    Codegen::get_uspc_ops_in_cluster(new_op,
				     Cluster_mgr::op_map.value(old_op),
				     mv_uspc_ops);
  } else {
    eString& opc = el_opcode_to_string_map.value(new_op->opcode());
    MDES_collect_op_descrs(opc, mv_uspc_ops);
  }
  Physfile_binding_attr *mv_prf_battr = new Physfile_binding_attr();
  set_prf_binding_attr(new_op, mv_prf_battr);
  Codegen::annotate_physical_file_op(new_op, mv_uspc_ops);

  // Cluster the damn thing.
  Cluster_mgr::op_map.bind(new_op, 0);
  Cluster_mgr::operand_map.bind(new_op->dest(DEST1).vr_num(), 0);
}


void arm_fix_op_formats(Compound_region* f, bool prepass)
{
  Dlist<Region*> rstack;
  Region *rtmp;

  rstack.push_tail(f);
  while (!rstack.is_empty()) {
    rtmp = rstack.pop();
     
    if (rtmp->is_hb() || rtmp->is_bb()) {       
      Hash_set<Op*> to_remove(hash_op_ptr, 64), dont_modify(hash_op_ptr, 64);
      for(Region_ops_C0_order rops((Compound_region*)rtmp); rops != 0; rops++) {
	Op* op = *rops;

	////////////////////////////////////////////////////////////////////////
	// Some op formats are not supported in the mdes, but
	// impact/codegen/register-allocation give them to us
	// anyway. Instead of marking the entire operation
	// unsupported, we add some fixup here.

	if(is_base_plus_offset_store(op)) {
	  if(op->src(SRC3).is_float() || op->src(SRC3).is_double() ||
	     (( ! op->src(SRC3).is_lit()) && (op->src(SRC3).file_type() == FPR))) {
	    if( ! op->src(SRC2).is_lit()) {
	      // we can't have stores that look like [r1 + r2] <- f1
	      Op* new_add = new Op((Opcode) ADD_W);
	      assert(new_add);
	      Operand new_addr;
	      if(prepass)
		new_addr = new Reg(EL_STANDARD_INT);
	      else {
		new_addr = new Macro_reg(ICMOVE_REG);
		// FIX: blindly binding to the GPR file without
		// querying the mdes to see where ICMOVE actually is.
		new_addr.bind_physical_file("GPR");
		Hash_set<int> phys_regs(hash_int, 8);
		MDES_get_phys_regs(new_addr, phys_regs);
		new_addr.bind_reg(phys_regs.head());
	      }
	      new_add->set_src(PRED1, op->src(PRED1));
	      new_add->set_dest(DEST1, new_addr);
	      new_add->set_src(SRC1, op->src(SRC1));
	      new_add->set_src(SRC2, op->src(SRC2));
	      if( ! prepass) {
		bind_and_cluster(new_add, op);
	      }
              
              Lcode_attribute_map* lam = get_lcode_attributes(op);
              if (lam->is_bound("cca_bit_width"))
              {
                List<Operand> attr = lam->value("cca_bit_width");
                Lcode_attribute_map* attr_map = new Lcode_attribute_map ();
                attr_map->bind("cca_bit_width", attr);
                //set_lcode_attributes(new_add, attr_map);
              }
	      El_insert_op_before(new_add, op);
	      op->set_src(SRC1, new_addr);
	      Operand new_offset(new Int_lit(0));
	      op->set_src(SRC2, new_offset);
 	    } else if(op->src(SRC2).is_int() && (op->src(SRC2).int_value() > 255)) {
	      Op* new_add = new Op((Opcode) ADD_W);
	      assert(new_add);
	      Operand new_addr;
	      if(prepass)
		new_addr = new Reg(EL_STANDARD_INT);
	      else {
		new_addr = new Macro_reg(ICMOVE_REG);
		// FIX: blindly binding to the GPR file without
		// querying the mdes to see where ICMOVE actually is.
		new_addr.bind_physical_file("GPR");
		Hash_set<int> phys_regs(hash_int, 8);
		MDES_get_phys_regs(new_addr, phys_regs);
		new_addr.bind_reg(phys_regs.head());
	      }
	      new_add->set_src(PRED1, op->src(PRED1));
	      new_add->set_dest(DEST1, new_addr);
	      new_add->set_src(SRC1, op->src(SRC1));
	      Operand b(new Int_lit(op->src(SRC2).int_value()));
	      new_add->set_src(SRC2, b);
              Lcode_attribute_map* lam = get_lcode_attributes(op);
              if (lam->is_bound("cca_bit_width"))
              {
                List<Operand> attr = lam->value("cca_bit_width");
                Lcode_attribute_map* attr_map = new Lcode_attribute_map ();
                attr_map->bind("cca_bit_width", attr);
                //set_lcode_attributes(new_add, attr_map);
              }
	      El_insert_op_before(new_add, op);
	      op->set_src(SRC1, new_addr);
	      Operand new_offset(new Int_lit(0));
	      op->set_src(SRC2, new_offset);
	      if( ! prepass) {
		Op* new_move = new Op((Opcode)MOVE);
		assert(new_move && "no memory");
		new_move->set_src(PRED1, op->src(PRED1));
		new_move->set_src(SRC1, b);
		new_move->set_dest(DEST1, new_addr);
		new_add->set_src(SRC2, new_addr);
		bind_and_cluster(new_move, op);
 		bind_and_cluster(new_add, op);
		El_insert_op_before(new_move, new_add);
	      }
	    } else if(op->src(SRC2).is_label()) {
	      Op* new_add = new Op((Opcode) ADD_W);
	      Op* new_move = new Op((Opcode) MOVE);
	      assert(new_add && new_move && "out of memory");
	      Operand new_addr;
	      if(prepass)
		new_addr = new Reg(EL_STANDARD_INT);
	      else {
		new_addr = new Macro_reg(ICMOVE_REG);
		// FIX: blindly binding to the GPR file without
		// querying the mdes to see where ICMOVE actually is.
		new_addr.bind_physical_file("GPR");
		Hash_set<int> phys_regs(hash_int, 8);
		MDES_get_phys_regs(new_addr, phys_regs);
		new_addr.bind_reg(phys_regs.head());
	      }
	      new_move->set_src(PRED1, op->src(PRED1));
	      new_move->set_src(SRC1, op->src(SRC2));
	      new_move->set_dest(DEST1, new_addr);

	      new_add->set_src(PRED1, op->src(PRED1));
	      new_add->set_dest(DEST1, new_addr);
	      new_add->set_src(SRC1, op->src(SRC1));
	      new_add->set_src(SRC2, new_addr);
	      if( ! prepass) {
		bind_and_cluster(new_move, op);
		bind_and_cluster(new_add, op);
	      }
              Lcode_attribute_map* lam = get_lcode_attributes(op);
              if (lam->is_bound("cca_bit_width"))
              {
                List<Operand> attr = lam->value("cca_bit_width");
                Lcode_attribute_map* attr_map = new Lcode_attribute_map ();
                attr_map->bind("cca_bit_width", attr);
                //set_lcode_attributes(new_add, attr_map);
              }
	      El_insert_op_before(new_move, op);
	      El_insert_op_before(new_add, op);
	      op->set_src(SRC1, new_addr);
	      Operand new_offset(new Int_lit(0));
	      op->set_src(SRC2, new_offset);	      
	    }
	  }
	}
	if(is_base_plus_offset_load(op)) {
	  if(op->dest(DEST1).file_type() == FPR) {
	    if( ! op->src(SRC2).is_lit()) {
	      // we can't have loads that look like f1 <- [r1 + r2]
	      Op* new_add = new Op((Opcode) ADD_W);
	      assert(new_add);
	      Operand new_addr;
	      if(prepass)
		new_addr = new Reg(EL_STANDARD_INT);
	      else {
		new_addr = new Macro_reg(ICMOVE_REG);
		// FIX: blindly binding to the GPR file without
		// querying the mdes to see where ICMOVE actually is.
		new_addr.bind_physical_file("GPR");
		Hash_set<int> phys_regs(hash_int, 8);
		MDES_get_phys_regs(new_addr, phys_regs);
		new_addr.bind_reg(phys_regs.head());
	      }
	      new_add->set_src(PRED1, op->src(PRED1));
	      new_add->set_dest(DEST1, new_addr);
	      new_add->set_src(SRC1, op->src(SRC1));
	      Operand b = op->src(SRC2);
	      new_add->set_src(SRC2, b);
              Lcode_attribute_map* lam = get_lcode_attributes(op);
              if (lam->is_bound("cca_bit_width"))
              {
                List<Operand> attr = lam->value("cca_bit_width");
                Lcode_attribute_map* attr_map = new Lcode_attribute_map ();
                attr_map->bind("cca_bit_width", attr);
                //set_lcode_attributes(new_add, attr_map);
              }
	      El_insert_op_before(new_add, op);
	      if( ! prepass) {
		bind_and_cluster(new_add, op);
	      }
	      op->set_src(SRC1, new_addr);
	      Operand new_offset(new Int_lit(0));
	      op->set_src(SRC2, new_offset);
	    } else if(op->src(SRC2).is_int() && (op->src(SRC2).int_value() > 255)) {
	      Op* new_add = new Op((Opcode) ADD_W);
	      assert(new_add);
	      Operand new_addr;
	      if(prepass)
		new_addr = new Reg(EL_STANDARD_INT);
	      else {
		new_addr = new Macro_reg(ICMOVE_REG);
		// FIX: blindly binding to the GPR file without
		// querying the mdes to see where ICMOVE actually is.
		new_addr.bind_physical_file("GPR");
		Hash_set<int> phys_regs(hash_int, 8);
		MDES_get_phys_regs(new_addr, phys_regs);
		new_addr.bind_reg(phys_regs.head());
	      }
	      new_add->set_src(PRED1, op->src(PRED1));
	      new_add->set_dest(DEST1, new_addr);
	      new_add->set_src(SRC1, op->src(SRC1));
	      Operand b(new Int_lit(op->src(SRC2).int_value()));
	      new_add->set_src(SRC2, b);
              Lcode_attribute_map* lam = get_lcode_attributes(op);
              if (lam->is_bound("cca_bit_width"))
              {
                List<Operand> attr = lam->value("cca_bit_width");
                Lcode_attribute_map* attr_map = new Lcode_attribute_map ();
                attr_map->bind("cca_bit_width", attr);
                //set_lcode_attributes(new_add, attr_map);
              }
	      El_insert_op_before(new_add, op);
	      op->set_src(SRC1, new_addr);
	      Operand new_offset(new Int_lit(0));
	      op->set_src(SRC2, new_offset);
	      if( ! prepass) {
		Op* new_move = new Op((Opcode)MOVE);
		assert(new_move && "no memory");
		new_move->set_src(PRED1, op->src(PRED1));
		new_move->set_src(SRC1, b);
		new_move->set_dest(DEST1, new_addr);
		new_add->set_src(SRC2, new_addr);
		bind_and_cluster(new_move, op);
 		bind_and_cluster(new_add, op);
		El_insert_op_before(new_move, new_add);
	      }
	    } else if(op->src(SRC2).is_label()) {
	      Op* new_add = new Op((Opcode) ADD_W);
	      Op* new_move = new Op((Opcode) MOVE);
	      assert(new_add && new_move && "out of memory");
	      Operand new_addr;
	      if(prepass)
		new_addr = new Reg(EL_STANDARD_INT);
	      else {
		new_addr = new Macro_reg(ICMOVE_REG);
		// FIX: blindly binding to the GPR file without
		// querying the mdes to see where ICMOVE actually is.
		new_addr.bind_physical_file("GPR");
		Hash_set<int> phys_regs(hash_int, 8);
		MDES_get_phys_regs(new_addr, phys_regs);
		new_addr.bind_reg(phys_regs.head());
	      }
	      new_move->set_src(PRED1, op->src(PRED1));
	      new_move->set_src(SRC1, op->src(SRC2));
	      new_move->set_dest(DEST1, new_addr);

	      new_add->set_src(PRED1, op->src(PRED1));
	      new_add->set_dest(DEST1, new_addr);
	      new_add->set_src(SRC1, op->src(SRC1));
	      new_add->set_src(SRC2, new_addr);
	      if( ! prepass) {
		bind_and_cluster(new_move, op);
		bind_and_cluster(new_add, op);
	      }
              Lcode_attribute_map* lam = get_lcode_attributes(op);
              if (lam->is_bound("cca_bit_width"))
              {
                List<Operand> attr = lam->value("cca_bit_width");
                Lcode_attribute_map* attr_map = new Lcode_attribute_map ();
                attr_map->bind("cca_bit_width", attr);
                //set_lcode_attributes(new_add, attr_map);
              }
	      El_insert_op_before(new_move, op);
	      El_insert_op_before(new_add, op);
	      op->set_src(SRC1, new_addr);
	      Operand new_offset(new Int_lit(0));
	      op->set_src(SRC2, new_offset);	      
	    }
	  }
	}

      } // end for all ops
    } else {
      for (Region_subregions subri(rtmp); subri!=0; subri++) {
	if ((*subri)->is_compound())
	  rstack.push_tail (*subri);
      }
    }
  }
}

