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



//////////////////////////////////////////////////////////////////
//                                                              //
//      File:           create_stack_frame.cpp                  //      
//      Authors:        Pracheeti Nagarkar                      //
//      Created:        August 2002                             //
//      Description:    Replaces instances of IP/OP/LV/SP       //
//                      in Rebel Code with SP and new offsets.  //
//                      Creates path for function calls.        //
//                      For WIMS.                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "codegen.h"

/*      Stack Model 
 *
 *      The offsets from IP/LV/RGS/OP are positive for high to low
 *      while the offsets are negative for low to high
 *      The IP/LV/RGS/OP macros always point as shown below irrespective
 *      of the direction of stack growth.
 *      The ordering among the regions are assumed to be fixed
 *
 *      Input Parameters
 *      Return Value
 *      -----------------   <-- IP, SP (before decrement)
 *      ICMOVE_REG
 *      -----------------
 *      Return Address
 *      -----------------   
 *      Local Variables
 *      -----------------   <-- LV
 *      Spill Code
 *      -----------------   <-- RGS
 *      Output Parameters
 *      Return Value
 *      -----------------   <-- OP, SP' (after decrement)
 */


/* 
 *   Returns value associated with macro in a DEFINE op
 *   If specified macro is not in define, returns -1    
 */
static int get_component(Op *op, Macro_name mname) 
{
  assert(is_define(op));
  Operand doprnd = op->dest(DEST1);
  Operand soprnd = op->src(SRC1);

  if (doprnd.is_macro_reg()) {
    if ((doprnd.name() == mname) && (soprnd.is_int()) ) 
      return (soprnd.int_value());
  }
  return -1;
}


/* 
 *   Creates store op: stores Return Address macro to stack 
 */
static Op *generate_bd_store_of_RA(int offset_from_sp) 
{
  Op * bd_store_op = new Op((Opcode)SG_W_C1);
   
  Operand sp_operand(new Macro_reg(SP_REG));
  Operand ra_operand(new Macro_reg(RETURN_ADDR));
  Operand offset_operand(new Int_lit(offset_from_sp));
  Operand pred_operand(new Pred_lit(true));

  bd_store_op->set_src(SRC1, sp_operand);
  bd_store_op->set_src(SRC2, offset_operand);
  bd_store_op->set_src(SRC3, ra_operand);
  bd_store_op->set_src(PRED1, pred_operand);
  bd_store_op->set_flag(EL_OPER_STACK_MOD);

  return bd_store_op;
}


/*
 *   Creates add op: SP increment/decrement dep. on to_add value    
 */
static Op *generate_sp_add(int to_add) 
{
  Op *add_op = new Op((Opcode)ADD_W);

  Operand dest_sp_operand(new Macro_reg(SP_REG));
  Operand src_sp_operand(new Macro_reg(SP_REG));
  Operand toadd_operand(new Int_lit(to_add));
  Operand pred_operand(new Pred_lit(true));

  add_op->set_src(SRC1, src_sp_operand);
  add_op->set_src(SRC2, toadd_operand);
  add_op->set_dest(DEST1, dest_sp_operand);
  add_op->set_src(PRED1, pred_operand);
  add_op->set_flag(EL_OPER_STACK_MOD);

  return add_op;
}


/* 
 *   Creates base + disp. load op: loads Return Address macro from stack 
 */
static Op *generate_bd_load_of_RA(int offset_from_sp) {
   
  Op *bd_load_op = new Op((Opcode)LG_W_C1_C1);
   
  Operand dest_ra_operand(new Macro_reg(RETURN_ADDR));
  Operand src_sp_operand(new Macro_reg(SP_REG));
  Operand src_offset_operand(new Int_lit(offset_from_sp));
  Operand pred_operand(new Pred_lit(true));
   
  bd_load_op->set_src(SRC1, src_sp_operand);
  bd_load_op->set_src(SRC2, src_offset_operand);
  bd_load_op->set_src(PRED1, pred_operand);
  bd_load_op->set_dest(DEST1, dest_ra_operand);
  bd_load_op->set_flag(EL_OPER_STACK_MOD);

  return bd_load_op;
}


/*
 * Replaces the macro_regs IP/OP/LV/RGS with SP given the op and port_num of macro_reg
 * Since SP aliases with the these macros, they are in the right clusters
 */
static void replace_macro_with_SP(Op *op, Port_num replace_port) 
{       
  Operand temp_operand(new Macro_reg(SP_REG));
  op->set_src(replace_port, temp_operand);     
}


static void codegen_fixup(Op *op)
{
  Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(op);
  if (prf_bind_attr)
    remove_prf_binding_attr(op);
}


Op* make_bd_from_non_bd(Op* ld_st_op)
{
  Op *bd_op = NULL;
  Operand offset_operand(new Int_lit(0));
  Operand pred_operand(ld_st_op->src(PRED1));
  Operand macro_operand(ld_st_op->src(SRC1));

  Opcode new_opc = NO_OP;
  switch(ld_st_op->opcode()) {
  case L_W_C1_C1:
    new_opc = (Opcode)LG_W_C1_C1;
    break;
  case L_H_C1_C1:
    new_opc = (Opcode)LG_H_C1_C1;
    break;
  case L_B_C1_C1:
    new_opc = (Opcode)LG_B_C1_C1;
    break;
  case LX_W_C1_C1:
    new_opc = (Opcode)LGX_W_C1_C1;
    break;
  case LX_H_C1_C1:
    new_opc = (Opcode)LGX_H_C1_C1;
    break;
  case LX_B_C1_C1:
    new_opc = (Opcode)LGX_B_C1_C1;
    break;
  case FL_S_C1_C1:
    new_opc = (Opcode)FLG_S_C1_C1;
    break;
  case FL_D_C1_C1:
    new_opc = (Opcode)FLG_D_C1_C1;
    break;
  case S_W_C1:
    new_opc = (Opcode)SG_W_C1;
    break;
  case S_H_C1:
    new_opc = (Opcode)SG_H_C1;
    break;
  case S_B_C1:
    new_opc = (Opcode)SG_B_C1;
    break;
  case FS_S_C1:
    new_opc = (Opcode)FSG_S_C1;
    break;
  case FS_D_C1:
    new_opc = (Opcode)FSG_D_C1;
    break;
  default:
    cdbg << "ERROR: I don't know how to turn this op into a base plus\n"
	 << "displacement load or store:\n" << *ld_st_op << endl;
    assert(0);
  }

  if (is_load(ld_st_op)) {
    bd_op = new Op(new_opc);
    Operand dest_operand(ld_st_op->dest(DEST1));
    bd_op->set_dest(DEST1, dest_operand);
    bd_op->set_src(SRC1, macro_operand);
    bd_op->set_src(SRC2, offset_operand);
    bd_op->set_src(PRED1, pred_operand);
      
  } else if (is_store(ld_st_op)) {
    bd_op = new Op(new_opc);
    bd_op->set_src(SRC1, macro_operand);
    bd_op->set_src(SRC2, offset_operand);
    bd_op->set_src(SRC3, ld_st_op->src(SRC2));
    bd_op->set_src(PRED1, pred_operand);
  }

  // need to copy the memory dependences, too. Don't make this a
  // Hash_set<Operand>& ! Weird segfaults will ensue.
  Hash_set<Operand> tmp_set = ld_st_op->mem_srcs();
  for(Hash_set_iterator<Operand> hsi(tmp_set); hsi != 0; hsi++) {
    bd_op->add_mem_src(*hsi);
  }
  tmp_set = ld_st_op->mem_dests();
  for(Hash_set_iterator<Operand> hsi(tmp_set); hsi != 0; hsi++) {
    bd_op->add_mem_dest(*hsi);
  }
  // Should add the kills, but if you try to, then elcor will assert
  // because the functionality isn't in the rest of the compiler.
  //    tmp_set = ld_st_op->mem_kills();
  //    for(Hash_set_iterator<Operand> hsi(tmp_set); hsi != 0; hsi++) {
  //      bd_op->add_mem_kill(*hsi);
  //    }

  return bd_op;
}


/*
 * From a non bd_load or non_bd_store op, creates a bd_op. 
 * Creates a bd_load or bd_store op. Displacement value set to 0. 
 * Macros stay the same as that in ld_st_op. 
 */
static Op *generate_bd_op(Op *ld_st_op)
{
  Op *bd_op = make_bd_from_non_bd(ld_st_op);

  remove_prf_binding_attr(ld_st_op);

  List_set<Op_descr *> uspc_ops;
  MDES_collect_op_descrs(el_opcode_to_string_map.value(bd_op->opcode()), uspc_ops);
  Physfile_binding_attr *prf_battr = new Physfile_binding_attr();
  set_prf_binding_attr(bd_op, prf_battr);
  Codegen::annotate_physical_file_op(bd_op, uspc_ops);

  return bd_op;
}


/*
 *   Replace int_lit value of offset with new offset from SP dep. on whether the
 *   the offset is used w.r.t IP/OP/LV/RGS
 *   Given: offset_lit_ref - reference containing the literal value of the offset 
 *          mname - macro_reg that the offset was used with 
 */
static void replace_offset(El_ref offset_lit_ref, Macro_name mname, 
                           int spillSize, int localSize, int max_paramSize,
			   int ipSize) 
{       
  Op *use_op = offset_lit_ref.get_op();
  assert(is_sub(use_op) || is_add(use_op) || is_move(use_op) ||
	 (is_mem_base_plus_offset(use_op) && (is_load(use_op) || is_store(use_op))));

  Port_num replace_port = offset_lit_ref.get_port_num();
  int new_offset = 0;

  int orig_offset = offset_lit_ref.get_operand().int_value();

  int stack_off = 0;

  if (mname == IP_REG)
    stack_off = spillSize + localSize + max_paramSize + ipSize +
      (current_MDES->isRetAddrThruStack() ? current_MDES->getRetAddrSize() : 0);
  else if (mname == LV_REG)    
    stack_off = max_paramSize + spillSize;
  else if (mname == RGS_REG)             
    stack_off = max_paramSize;
  else if (mname == OP_REG)
    stack_off = 0;

  if (current_MDES->getStackDir() == LOW_TO_HIGH) 
    stack_off = -stack_off;

  if (is_sub(use_op) && use_op->src(SRC1).is_macro_reg()) {
    if((use_op->src(SRC1).name() == RGS_REG) ||
       (use_op->src(SRC1).name() == IP_REG) ||
       (use_op->src(SRC1).name() == OP_REG) ||
       (use_op->src(SRC1).name() == LV_REG))
      ;
    else 
      El_punt("");
    stack_off = -stack_off;
  } else if (is_sub(use_op) && use_op->src(SRC2).is_macro_reg()) {
    assert((use_op->src(SRC2).name() == RGS_REG) ||
	   (use_op->src(SRC2).name() == IP_REG) ||
	   (use_op->src(SRC2).name() == OP_REG) ||
	   (use_op->src(SRC2).name() == LV_REG));
    stack_off = -stack_off;
  }

  new_offset = orig_offset + stack_off;

  Operand offset_operand(new Int_lit(new_offset));
  use_op->set_src(replace_port, offset_operand);
}


static Op* fix_the_offset(Op *use_op, Port_num port)
{
  assert(use_op->src(port).is_reg() || use_op->src(port).is_macro_reg());

  Operand pred(use_op->src(PRED1));

  Reg* reg = new Reg(use_op->src(port).dtype(), use_op->src(port).file_type());
  assert(reg && "out of memory");
  reg->rename();
  Operand tmp_reg(reg);
  Operand offset_oper(new Int_lit(0));
  
  Op *fix_op = new Op((Opcode)ADD_W);
  assert(fix_op && "out of memory");

  fix_op->set_dest(DEST1, tmp_reg);
  fix_op->set_src(SRC1, use_op->src(port));
  fix_op->set_src(SRC2, offset_oper);
  fix_op->set_src(PRED1, pred);

  use_op->set_src(port, tmp_reg);

  List_set<Op_descr *> uspc_ops;
  MDES_collect_op_descrs(el_opcode_to_string_map.value(use_op->opcode()), uspc_ops);
  Physfile_binding_attr *prf_battr = get_prf_binding_attr(use_op);
  assert(prf_battr != 0);
  Codegen::annotate_prf_port(DATA_IN, port, use_op->src(port), prf_battr, uspc_ops, use_op);

  List_set<Op_descr*> fix_uspc_ops;
  MDES_collect_op_descrs(el_opcode_to_string_map.value(fix_op->opcode()), fix_uspc_ops);

  Physfile_binding_attr *fix_prf_battr = new Physfile_binding_attr();
  set_prf_binding_attr(fix_op, fix_prf_battr);
  Codegen::annotate_physical_file_op(fix_op, fix_uspc_ops);

  El_insert_op_before(fix_op, use_op);
  return fix_op;
}


/*
 *              DRIVER ROUTINE                                          
 *                                                              
 *      For each procedure f:                                   
 *      Stores return address onto stack                
 *      Increments stack pointer by (spill + local + RA + maxParamSize)     
 *      Replaces IP/OP/LV/SPs with SP and new offset    
 *      Decrements stack pointer by (spill + local + RA + maxParamSize) before rts_op    
 */
void Codegen::create_stack_frame(Procedure *f, Cluster_mgr& cmgr)
{
  int spillSize = 0;
  int localSize = 0;
  int opSize = 0;
  int ipSize = 0;
  unsigned int sp_increment = 0;

  if (dbg(status,1))
    cdbg << "Creating stack frame for proc: "  << f->get_name() << endl;

  if (dbg(stack,5)) {
    char buff[256];
    sprintf(buff, "%s_before_stack.reb", (char*)f->get_name());
    cdbg << "Printing Rebel after Stack Processing" << endl;
    IR_outstream dbg_out(buff);
    ir_write(dbg_out, f);
  }

  bool prologue = false;

  // Extract spill sizes and local sizes from prologue's define ops
  for (Region_subregions subregi(f); subregi != 0; subregi++) {
    Region *subreg = *subregi;

    if (subreg->flag(EL_REGION_HAS_PROLOGUE_OP)) {
      prologue = true;
      for (Region_all_ops opi(subreg); opi != 0; opi++) {
	Op *op = *opi;
	if (is_pseudo(op) && (op->opcode() == DEFINE)) {
	  int size1, size2, size3;
	  size1 = get_component(op, SWAP);
	  size2 = get_component(op, LOCAL);
	  size3 = get_component(op, PARAM);
          int align = current_MDES->getAlignment();
          int mod = 0;
	  if (size1 != -1) {
	    spillSize = size1;
            mod = spillSize % align;
            if (mod != 0)
              spillSize += (align - mod);
	  } else if (size2 != -1) {
	    localSize = size2;
            mod = localSize % align;
            if (mod != 0)
              localSize += (align - mod);
	  } else if (size3 != -1) {
	    opSize = size3;
            mod = opSize % align;
            if (mod != 0)
              opSize += (align - mod);
          }
	}

	// ARM needs to put some variables passed through registers on
	// the stack on function entry, thus, we need to leave space
	// for those.
	if( ! strcasecmp(El_arch, "arm")) {
	  // Find the IP relative store with the lowest offset to
	  // determine how much additional space is needed.
	  if(is_base_plus_offset_store(op)) {
	    if(op->src(SRC1).is_macro_reg() && op->src(SRC1).name() == IP_REG) {
	      if(op->src(SRC2).is_int()) {
		ipSize = ELCOR_MAX(ipSize, -(op->src(SRC2).int_value()));
	      } else {
		cdbg << "ERROR: I'm seeing an IP relative store in the function\n"
		     << "prologue, but I can't figure out what the offset is.\n"
		     << "This will likely mean your stack will be corrupted!\n"
		     << *op << endl;
		El_punt("");
	      }
	    }
	  }
	} // if arch == ARM

      } // for all ops

      // ipSize has already been allocated in the local space in
      // Impact. We can remove it here so we don't use too much stack
      // space. FIX: I'm not 100% sure that Impact does this correctly.
//       localSize -= ipSize;

      if(El_need_to_save_icmove) {
	ipSize += 4;
      }

      sp_increment = (current_MDES->isRetAddrThruStack() ?
		      current_MDES->getRetAddrSize() : 0) + 
	spillSize + localSize + opSize + ipSize;

      if (dbg(stack, 3)) {
	cdbg << "spillSize: " << spillSize << endl;
	cdbg << "localSize: " << localSize << endl;
	cdbg << "opSize: " << opSize << endl;
	cdbg << "ipSize: " << ipSize << endl;
	cdbg << "retAddrSize: " << (current_MDES->isRetAddrThruStack() ?
				    current_MDES->getRetAddrSize() : 0) << endl;
	cdbg << "sp_increment: " << sp_increment << endl;
      }
      break;
    }
  }
  assert(prologue == true);

  /* Parse through instances of IP/OP/LV/SP in code and replace them with SP + offset
   * Add op for decrementing SP before RTS
   * Instances of these macro registers could be in:
   *          add/sub ops (address calculation, before a load/store)
   *          base plus displacement ops (address calculation & load/store)
   *          move ops (inserted by codegen/clustering?)
   */
  for (Region_subregions subregni(f); subregni != 0; subregni++) {
    Region * subreg = *subregni;
    for (Region_all_ops opi(subreg); opi != 0; opi++) {
      Op *op = *opi;
      if (dbg(stack,5))
	cdbg << "Op under analysis: " << *op << endl;

      if ((is_add(op) || is_sub(op) || is_mem_base_plus_offset(op)) &&
	  op->src(SRC1).is_macro_reg()) {
	Macro_name mn = op->src(SRC1).name();

	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG) || 
	    ((mn == RGS_REG) && (op->flag(EL_OPER_SPILL_CODE) ||
				 op->flag(EL_OPER_LIVEOUT_DEST)))) {

	  El_ref offset_ref(op, &(op->src(SRC2)), EXP_SRC);
	  if (op->src(SRC2).is_reg() || op->src(SRC2).is_macro_reg()) {
	    cdbg << "ERROR: trying to adjust a stack parameter, but it has"
		 << "an improper\nformat. Was prepass_create_stack_frame run?"
		 << "\n" << *op << endl;
	    El_punt("");
	  }
	  replace_offset(offset_ref, mn, spillSize, localSize, opSize, ipSize);
	  replace_macro_with_SP(op, SRC1);
	  codegen_fixup(op);
	}
      } else if ((is_add(op) || is_sub(op) || is_mem_base_plus_offset(op)) &&
		 op->src(SRC2).is_macro_reg()) {
	Macro_name mn = op->src(SRC2).name();

	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG) || 
	    ((mn == RGS_REG) && (op->flag(EL_OPER_SPILL_CODE) ||
				 op->flag(EL_OPER_LIVEOUT_DEST)))) {

	  El_ref offset_ref(op, &(op->src(SRC1)), EXP_SRC);
	  if (op->src(SRC1).is_reg() || op->src(SRC1).is_macro_reg()) {
	    cdbg << "ERROR: trying to adjust a stack parameter, but it has"
		 << "an improper\nformat. Was prepass_create_stack_frame run?"
		 << "\n" << *op << endl;
	    El_punt("");
	  }
	  replace_offset(offset_ref, mn, spillSize, localSize, opSize, ipSize);
	  replace_macro_with_SP(op, SRC2);
	  codegen_fixup(op);
	}
      } else if (is_move(op) && op->src(SRC1).is_macro_reg()) {
	// Macro use not found in add/bd_ops. Recursively go through moves till use is found.
	Macro_name mn = op->src(SRC1).name();
	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG) || 
	    ((mn == RGS_REG) && (op->flag(EL_OPER_SPILL_CODE) || 
				 op->flag(EL_OPER_LIVEOUT_DEST)))) {

          if (op->flag(EL_OPER_ICMOVE)) {
            Op *new_op = new Op(ADD_W);
            Reg* reg = new Reg(op->dest(DEST1).dtype(), op->dest(DEST1).file_type());
            Operand dest(reg);
            new_op->set_dest(DEST1, dest);
            new_op->set_src(SRC1, op->src(SRC1));
            Operand lit0(new Int_lit(0));
            new_op->set_src(SRC2, lit0);
            new_op->set_src(PRED1, op->src(PRED1));
	    El_ref offset_ref(new_op, &(new_op->src(SRC2)), EXP_SRC);
	    replace_offset(offset_ref, mn, spillSize, localSize, opSize, ipSize);
	    replace_macro_with_SP(new_op, SRC1);
	    El_insert_op_before(new_op, op);
            int cid = cmgr.get_cluster_for_physical_file(op->src(SRC1).physical_file_type());
            Cluster_mgr::op_map.bind(new_op, cid);
            op->set_src(SRC1, dest);
            cdbg << "op: " << op->id() << " new_op: " << new_op->id() << endl;
	    codegen_fixup(op);
          } else {
            op->set_opcode(ADD_W);
            op->update_num_operands();
            Operand lit0(new Int_lit(0));
            op->set_src(SRC2, lit0);
	    codegen_fixup(op);
	    El_ref offset_ref(op, &(op->src(SRC2)), EXP_SRC);
	    replace_offset(offset_ref, mn, spillSize, localSize, opSize, ipSize);
	    replace_macro_with_SP(op, SRC1);
          }
	  // El_punt("");	  
	}
      } else if (is_rts(op)) {
	// Insert load op (before RTS) to move RA value from stack
	// into RA macro.
	if (current_MDES->getStackDir() == HIGH_TO_LOW) {
	  if (current_MDES->isRetAddrThruStack()) {
 	    int some_offset = opSize + spillSize + localSize;
	    Op* bd_ld_ra = generate_bd_load_of_RA(some_offset);
	    El_insert_op_before(bd_ld_ra, op);
	  }
	  El_insert_op_before(generate_sp_add(sp_increment), op);
	} else if (current_MDES->getStackDir() == LOW_TO_HIGH) {
	  if (current_MDES->isRetAddrThruStack())
	    El_insert_op_before(generate_bd_load_of_RA(sp_increment), op);
	  El_insert_op_before(generate_sp_add(-sp_increment), op);
	}

	if(El_need_to_save_icmove) {
	  Op* load_op = generate_bd_load_of_RA(-ipSize);
	  Operand icm(new Macro_reg(ICMOVE_REG));
	  load_op->set_dest(DEST1, icm);
	  load_op->set_flag(EL_OPER_CALLEE_SAVE);
	  load_op->set_flag(EL_OPER_SPILL_CODE);
	  El_insert_op_before(load_op, op);
	}

      } else {
	// Maybe this should be commented out for speed.
	if( ! is_pseudo(op)) {
	  for(Op_all_sources oas(op); oas != 0; oas++) {
	    Operand oper = *oas;
	    if(oper.is_macro_reg()) {
	      Macro_name mn = oper.name();
	      if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
		cdbg << "ERROR: this op uses a stack parameter, but I don't "
		     << "know how\nto do postpass stack adjustments for it\n"
		     << *op << endl;
		El_punt("");
	      }
	    }
	  }
	}
      }
    } // region all ops
  } // region sub_regions

  // Inserting ops (Store of return address and sp increment at the
  // beginning of procedure)
  prologue = false;
  for (Region_subregions subri(f); subri != 0; subri++) {
    Region *subr = *subri;
    if (subr->flag(EL_REGION_HAS_PROLOGUE_OP)) {
      prologue = true;
      // Create new add op (add
      // ret_addr_size+spillsize+localsize+paramSize to stack pointer)
      // Increment SP first, to take care of interrupt case

      int ret_addr_offset = opSize + spillSize + localSize;
      Op *store_op = NULL;
      Op *add_op = NULL;
      if (current_MDES->getStackDir() == HIGH_TO_LOW) {
	if (current_MDES->isRetAddrThruStack())
	  store_op = generate_bd_store_of_RA(ret_addr_offset);
	add_op = generate_sp_add(-sp_increment);
      } else if (current_MDES->getStackDir() == LOW_TO_HIGH) {
	if (current_MDES->isRetAddrThruStack())
	  store_op = generate_bd_store_of_RA(ret_addr_offset);
	add_op = generate_sp_add(sp_increment);
      }
      El_insert_op_after_merge((Compound_region *)subr, add_op);
      if (current_MDES->isRetAddrThruStack())
	// Insert store op after the add op
	El_insert_op_after(store_op, add_op);
      
      if(El_need_to_save_icmove) {
	store_op = generate_bd_store_of_RA(-ipSize);
	Operand icm(new Macro_reg(ICMOVE_REG));
	store_op->set_src(SRC3, icm);
	store_op->set_flag(EL_OPER_CALLEE_SAVE);
	store_op->set_flag(EL_OPER_SPILL_CODE);
	El_insert_op_after_merge((Compound_region *)subr, store_op);
      }

      break;
    }
  }
  assert(prologue == true);

  fix_op_formats(f, false);

  if (dbg(stack,5)) {
    char buff[256];
    sprintf(buff, "%s_stack.reb", (char*)f->get_name());
    cdbg << "Printing Rebel after Stack Processing" << endl;
    IR_outstream dbg_out(buff);
    ir_write(dbg_out, f);
  }

}


//////////////////////////////////////////////////////////////////////////////
// After allocation, we may need to adjust operations that use stack
// relative macros (i.e. IP/OP/LV). Before register allocation, we
// need to create operations that allow us to insert the offsets
// later. For example, MOV r2, LV would be turned into ADD r2, LV, #0,
// so that the offset can be changed after allocation. Sometimes
// additional ops need to be added: LD r2 = [OP + r3] would become
// ADD r100 = OP, #0
// LD r2 = [r100 + r3]

void Codegen::create_stack_frame_prepass(Procedure *f) 
{
  if (dbg(status,1))
    cdbg << "Prepass Creating stack frame for proc: "  << f->get_name() << endl;

  if (dbg(stack, 5))
    cdbg << "In procedure: " << f->get_name() << endl;

  Hash_set<Op*> ignore_opset(hash_op_ptr, 32);

  List<Op*> non_bd_mem_ops, to_remove;

  /* Initially, convert the non base+displ. ld/st with
   * IP/OP/LV offsets into base+displ ones so that their offsets
   * can be adjusted. 
   */
  for (Region_subregions subregi(f); subregi != 0; subregi++) {
    Region *subreg = *subregi;

    for (Region_all_ops opi(subreg); opi != 0; opi++) {
      Op *op = *opi;

      if ((is_load(op) || is_store(op)) && op->src(SRC1).is_macro_reg() && 
	  !is_mem_base_plus_offset(op)) {

	Macro_name mn = op->src(SRC1).name();
	if ((mn == IP_REG) || (mn == OP_REG) || (mn == LV_REG)) {
	  non_bd_mem_ops.add_tail(op);
	}
      }
    }
  }

  // Run through list of loads-stores and change them to bd-loads-stores.
  for (List_iterator<Op*> li(non_bd_mem_ops); li != 0; li++) {
    Op* op = (*li);
    Op* bd_op = generate_bd_op(op);

    El_replace_op(op->parent(), op, bd_op);
  }

  // FIX: clean this code up.

  for (Region_subregions subregni(f); subregni != 0; subregni++) {
    Region * subreg = *subregni;
    for (Region_all_ops opi(subreg); opi != 0; opi++) {
      Op *op = *opi;
      if (dbg(stack,5))
	cdbg << "Prepass Op under analysis: " << *op << endl;

      if (ignore_opset.is_member(op))
	continue;

      if ((is_add(op) || is_sub(op) || is_mem_base_plus_offset(op)) &&
	  op->src(SRC1).is_macro_reg()) {
	Macro_name mn = op->src(SRC1).name();
	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	  if( ! op->src(SRC2).is_int()) {
	    if(op->src(SRC2).is_reg() || op->src(SRC2).is_macro_reg()) {
	      ignore_opset += fix_the_offset(op, SRC1);
	      if(op->src(SRC2).is_macro_reg()) {
		mn = op->src(SRC2).name();
		if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
		  ignore_opset += fix_the_offset(op, SRC2);
		}
	      }
	    }
	  }
	}
	if(is_store(op) && op->src(SRC3).is_macro_reg()) {
	  Macro_name mn = op->src(SRC3).name();
	  if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	    ignore_opset += fix_the_offset(op, SRC3);
	  }
	}
      } else if ((is_add(op) || is_sub(op) || is_mem_base_plus_offset(op)) &&
		 op->src(SRC2).is_macro_reg()) {
	Macro_name mn = op->src(SRC2).name();
	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	  if( ! op->src(SRC1).is_int()) {
	    if (op->src(SRC1).is_reg() || op->src(SRC1).is_macro_reg()) {
	      ignore_opset += fix_the_offset(op, SRC2);
	    }
	  }
	}
	if(is_store(op) && op->src(SRC3).is_macro_reg()) {
	  Macro_name mn = op->src(SRC3).name();
	  if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	    ignore_opset += fix_the_offset(op, SRC3);
	  }
	}
      } else if ((is_store(op) && is_mem_base_plus_offset(op)) &&
		 op->src(SRC3).is_macro_reg()) {
	Macro_name mn = op->src(SRC3).name();
	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	  ignore_opset += fix_the_offset(op, SRC3);
	}
      } else if ((is_store(op) && ! is_mem_base_plus_offset(op)) &&
		 op->src(SRC2).is_macro_reg()) {
	Macro_name mn = op->src(SRC2).name();
	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	  ignore_opset += fix_the_offset(op, SRC2);
	}
      } else if (is_move(op) && op->src(SRC1).is_macro_reg()) {
	Macro_name mn = op->src(SRC1).name();
	if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	  // Moves need to be turned into ADDs. The offset fix will
	  // insert a similar ADD; we just need to do a little fixup.
	  Op* new_op = fix_the_offset(op, SRC1);
	  new_op->set_dest(DEST1, op->dest(DEST1));
	  to_remove.add_tail(op);
	}
      } else if(is_cmpp(op)) {
	if(op->src(SRC1).is_macro_reg()) {
	  Macro_name mn = op->src(SRC1).name();
	  if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	    ignore_opset += fix_the_offset(op, SRC1);
	  }
	}
	if(op->src(SRC2).is_macro_reg()) {
	  Macro_name mn = op->src(SRC2).name();
	  if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
	    ignore_opset += fix_the_offset(op, SRC2);
	  }
	}
      } else {
	// Maybe this should be commented out for speed.
	if( ! is_pseudo(op)) {
	  for(Op_all_sources oas(op); oas != 0; oas++) {
	    Operand oper = *oas;
	    if(oper.is_macro_reg()) {
	      Macro_name mn = oper.name();
	      if ((mn == IP_REG) || (mn == LV_REG) || (mn == OP_REG)) {
		cdbg << "ERROR: this op uses a stack parameter, but I don't "
		     << "know how\nto prepare it for postpass stack adjustments\n"
		     << *op << endl;
		El_punt("");
	      }
	    }
	  }
	}
      } // else
    } // region all ops

    for(List_iterator<Op*> li(to_remove); li != 0; li++) {
      Op* op = *li;
      El_remove_op(op);
      delete op;
    }
    to_remove.clear();

  } // region sub_regions

  fix_op_formats(f, true);
}
