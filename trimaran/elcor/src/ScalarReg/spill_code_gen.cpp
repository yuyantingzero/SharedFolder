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

Copyright 2007 University of Michigan

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
//      File:           spill_code_gen.cpp
//      Authors:        Nate Clark
//      Created:        January 2005
//      Description:    Interface to insert spill code into the application.
//
/////////////////////////////////////////////////////////////////////////////

#include "spill_code_gen.h"
#include "op.h"
#include "mdes.h"
#include "dtype.h"
#include "el_control.h"


Hash_map<int, int> vr_to_memvr(hash_int, 128);


// This function generates an ADD instruction to get the address for
// spill code. It's useful if the architecture doesn't support
// base+displacement addressing.

static Op* get_stack_op(Operand oper, Operand pred)
{
  Operand temp = new Reg(EL_STANDARD_INT);

  int stack_loc = Scalar_reg_alloc::spill_stack.get_stack_offset(oper);
  if (current_MDES->getStackDir() == LOW_TO_HIGH)
    stack_loc = -stack_loc;

  Op* add_op = new Op((Opcode) ADD_W);
  Operand sp_operand(new Macro_reg(RGS_REG));
  Operand loc_operand(new Int_lit(stack_loc));
  add_op->set_dest(DEST1, temp);
  add_op->set_src(SRC1, sp_operand);
  add_op->set_src(SRC2, loc_operand);
  add_op->set_src(PRED1, pred);
  add_op->set_flag(EL_OPER_SPILL_CODE);
  return add_op;
}


static Op* get_non_base_disp_op(Operand oper, Operand stack_addr, Operand pred,
				spill_type sp_type)
{
  Op* spill_op = NULL;
  int memvr, vr_num;
  if (oper.is_macro_reg())
    vr_num = macro_to_vr(oper);
  // uses the fake vr_num assigned to macros, since they have no vr_num
  else
    vr_num = Scalar_reg_alloc::spill_stack.get_alias(oper.vr_num());

  if (vr_to_memvr.is_bound(vr_num)) {
    memvr = vr_to_memvr[vr_num];
  } else {
    memvr = virtual_register_number++;
    vr_to_memvr.bind(vr_num, memvr);
  }
	
  switch(oper.file_type()) {
  case GPR:
  case CR:
    if (sp_type == SPILL_STORE)
      spill_op = new Op((Opcode) SAVE);
    else if (sp_type == SPILL_LOAD)
      spill_op = new Op((Opcode) RESTORE);
    break;
  case PR:
    if (sp_type == SPILL_STORE)
      spill_op = new Op((Opcode) PSAVE);
    else if (sp_type == SPILL_LOAD)
      spill_op = new Op((Opcode) PRESTORE);
    break;
  case BTR:
    if (sp_type == SPILL_STORE)
      spill_op = new Op((Opcode) BSAVE);
    else if (sp_type == SPILL_LOAD)
      spill_op = new Op((Opcode) BRESTORE);
    break;
  case FPR:
    if (sp_type == SPILL_STORE) {
      if(oper.dtype().is_single())
	spill_op = new Op((Opcode) FSAVE_S);
      else
	spill_op = new Op((Opcode) FSAVE_D);
    } else if (sp_type == SPILL_LOAD) {
      if(oper.dtype().is_single())
	spill_op = new Op((Opcode) FRESTORE_S);
      else
	spill_op = new Op((Opcode) FRESTORE_D);
    }
    break;
  case VIR:
    if(sp_type == SPILL_STORE) {
      spill_op = new Op((Opcode) VSAVE);
      spill_op->set_src (SRC3, Operand(new Macro_reg(VEC_MASK_TRUE)));
    } else if(sp_type == SPILL_LOAD) {
      spill_op = new Op((Opcode) VRESTORE);
      spill_op->set_src (SRC2, Operand(new Macro_reg(VEC_MASK_TRUE)));
    }
    break;
  case VFR:
    if(sp_type == SPILL_STORE) {
      if (oper.dtype().is_single()) {
        spill_op = new Op((Opcode) VFSAVE_S);
        spill_op->set_src (SRC3, Operand(new Macro_reg(VEC_MASK_TRUE)));
      } else {
        spill_op = new Op((Opcode) VFSAVE_S);
        spill_op->set_src (SRC3, Operand(new Macro_reg(VEC_MASK_TRUE)));
      }
    } else if(sp_type == SPILL_LOAD) {
      if (oper.dtype().is_single()) {
        spill_op = new Op((Opcode) VFRESTORE_D);
        spill_op->set_src (SRC2, Operand(new Macro_reg(VEC_MASK_TRUE)));
      } else {
        spill_op = new Op((Opcode) VFRESTORE_D);
        spill_op->set_src (SRC2, Operand(new Macro_reg(VEC_MASK_TRUE)));
      }
    }
    break;
  default:
    assert(0);
    break;
  }
  assert(spill_op && "you're out of memory!");

  if ( sp_type == SPILL_STORE ) {
    spill_op->set_src(SRC1, stack_addr);
    spill_op->set_src(SRC2, oper);
    Operand memvr_oprnd = new Mem_vr(memvr);
    spill_op->add_mem_dest(memvr_oprnd);
  } else {
    spill_op->set_dest(DEST1, oper);
    spill_op->set_src(SRC1, stack_addr);
    Operand memvr_oprnd = new Mem_vr(memvr);
    spill_op->add_mem_src(memvr_oprnd);
  }
  spill_op->set_src(PRED1, pred);
  spill_op->set_flag(EL_OPER_SPILL_CODE);
  return spill_op;
}


static Op* get_base_disp_op(Operand oper, Operand pred, spill_type sp_type)
{
  Op* bd_op = NULL;

  int memvr;
  int vr_num;
  if (oper.is_macro_reg())
    vr_num = macro_to_vr(oper);
  else
    vr_num = Scalar_reg_alloc::spill_stack.get_alias(oper.vr_num());

  if (vr_to_memvr.is_bound(vr_num)) {
    memvr = vr_to_memvr[vr_num];
  } else {
    memvr = virtual_register_number++;
    vr_to_memvr.bind(vr_num, memvr);
  }

  switch(oper.file_type()) {
  case GPR:
  case CR:
    if (sp_type == SPILL_STORE)
      bd_op = new Op((Opcode) SAVEG);
    else if (sp_type == SPILL_LOAD)
      bd_op = new Op((Opcode) RESTOREG);
    break;
  case PR:
    if (sp_type == SPILL_STORE)
      bd_op = new Op((Opcode) PSAVEG);
    else if (sp_type == SPILL_LOAD)
      bd_op = new Op((Opcode) PRESTOREG);
    break;
  case BTR:
    if (sp_type == SPILL_STORE)
      bd_op = new Op((Opcode) BSAVEG);
    else if (sp_type == SPILL_LOAD)
      bd_op = new Op((Opcode) BRESTOREG);
    break;
  case FPR:
    if (sp_type == SPILL_STORE) {
      if(oper.dtype().is_single())
	bd_op = new Op((Opcode) FSAVEG_S);
      else
	bd_op = new Op((Opcode) FSAVEG_D);
    } else if (sp_type == SPILL_LOAD) {
      if(oper.dtype().is_single())
	bd_op = new Op((Opcode) FRESTOREG_S);
      else
	bd_op = new Op((Opcode) FRESTOREG_D);
    }
    break;
  case VIR:
    if(sp_type == SPILL_STORE) {
      bd_op = new Op((Opcode) VSAVEG);
      bd_op->set_src (SRC4, Operand(new Macro_reg(VEC_MASK_TRUE)));
    } else if(sp_type == SPILL_LOAD) {
      bd_op = new Op((Opcode) VRESTOREG);
      bd_op->set_src (SRC3, Operand(new Macro_reg(VEC_MASK_TRUE)));
    }
    break;
  case VFR:
    if(sp_type == SPILL_STORE) {
      if (oper.dtype().is_single()) {
        bd_op = new Op((Opcode) VFSAVEG_S);
        bd_op->set_src (SRC4, Operand(new Macro_reg(VEC_MASK_TRUE)));
      } else {
        bd_op = new Op((Opcode) VFSAVEG_D);
        bd_op->set_src (SRC4, Operand(new Macro_reg(VEC_MASK_TRUE)));
      }
    } else if(sp_type == SPILL_LOAD) {
      if (oper.dtype().is_single()) {
        bd_op = new Op((Opcode) VFRESTOREG_S);
        bd_op->set_src (SRC3, Operand(new Macro_reg(VEC_MASK_TRUE)));
      } else {
        bd_op = new Op((Opcode) VFRESTOREG_D);
        bd_op->set_src (SRC3, Operand(new Macro_reg(VEC_MASK_TRUE)));
      }
    }

  default:
    cdbg << "I don't recognize your register file, so I'm not sure what type "
	 << "of spill code to generate!\nFile: " << oper.file_type() << endl;
    assert(0);
    break;
  }
  assert(bd_op && "you're out of memory");

  // Instead of figuring out what all the fields in these operands
  // should be, we create spill code as if there's no base+disp, and
  // just steal the approprite fields.
  Op* add_op = get_stack_op(oper, pred);
  Op* mem_op = get_non_base_disp_op(oper, add_op->dest(DEST1), pred, sp_type);

  Operand memvr_oprnd = new Mem_vr(memvr); 

  if (sp_type == SPILL_STORE) {
    bd_op->set_src(SRC1, add_op->src(SRC1));
    bd_op->set_src(SRC2, add_op->src(SRC2));
    bd_op->set_src(SRC3, mem_op->src(SRC2));
    bd_op->add_mem_dest(memvr_oprnd);
  } else {
    // it's a load
    bd_op->set_dest(DEST1, mem_op->dest(DEST1));
    bd_op->set_src(SRC1, add_op->src(SRC1));
    bd_op->set_src(SRC2, add_op->src(SRC2));
    bd_op->add_mem_src(memvr_oprnd);
  }
  if(oper.file_type() == PR)
    bd_op->set_src(PRED1, Operand(new Pred_lit(true)));
  else
    bd_op->set_src(PRED1, pred);
  bd_op->set_flag(EL_OPER_SPILL_CODE);

  delete add_op; delete mem_op;
  return bd_op;
}


////////////////////////////////////////////////////////////////////////
// Spilling predicates is a little different than the other file
// types, because of the macros used to spill them. Additionally, we
// can't directly load into predicates, so special instructions are
// needed to load into a GPR, then mask the appropriate bits, and then
// move the result into the appropriate predicate. This masking and
// the determination of the appropriate macro register cannot be done
// until the spilled predicate is actually bound to a physical
// register, so we insert place-holder spill code, which is filled in
// after predicate allocation (see scalar_reg_alloc.cpp). 
//
// I'm really sorry for all of this. Don't blame me. I didn't come
// with the ISA; I just wrote the allocator.

static Op* insert_spill_for_pred(Operand oper, Operand pred, Compound_region* r,
				 Op* pivot_op, spill_location where_to_put_it,
				 spill_type sp_type, bool bd_supported)
{
  assert(oper.file_type() == PR);

  if(sp_type == SPILL_STORE) {
    if(bd_supported) {
      Op* spill_op = get_base_disp_op(oper, pred, sp_type);
      if (where_to_put_it == BEFORE_OP)
	El_insert_op_before(spill_op, pivot_op);
      else
	El_insert_op_after(spill_op, pivot_op);
      return spill_op;
    } else {
      Op* stack_op = get_stack_op(oper, pred);
      Op* spill_op = get_non_base_disp_op(oper, stack_op->dest(DEST1), pred, sp_type);
      if (where_to_put_it == BEFORE_OP) {
	stack_op->set_flag(EL_OPER_CALLER_SAVE);
	El_insert_op_before(stack_op, pivot_op);
	El_insert_op_before(spill_op, pivot_op);
      } else {
	El_insert_op_after(stack_op, pivot_op);
	El_insert_op_after(spill_op, stack_op);
      }
      return spill_op;
    }
  } else {
    // Predicate load.
    Operand temp = new Reg(EL_STANDARD_INT);
    Op* mask_op = new Op((Opcode) MOVEGBP);
    assert(mask_op && "out of memory");

    // We don't know what the appropriate mask value should be until
    // this register is bound, so just use UNDEFINED, and change it after
    // predicate allocation.
    Operand mask_operand(new Macro_reg(UNDEFINED));
    mask_op->set_dest(DEST1, oper);
    mask_op->set_src(SRC1, temp);
    mask_op->set_src(SRC2, mask_operand);
    mask_op->set_src(PRED1, pred);
    mask_op->set_flag(EL_OPER_SPILL_CODE);
    if(bd_supported) {
      Op* spill_op = get_base_disp_op(oper, pred, sp_type);
      spill_op->set_dest(DEST1, temp);
      if (where_to_put_it == BEFORE_OP) {
	El_insert_op_before(spill_op, pivot_op);
	El_insert_op_before(mask_op, pivot_op);
      } else {
	mask_op->set_flag(EL_OPER_CALLER_SAVE);
	El_insert_op_after(mask_op, pivot_op);
	El_insert_op_after(spill_op, pivot_op);
      }
      return spill_op;
    } else {
      Op* stack_op = get_stack_op(oper, pred);
      Op* spill_op = get_non_base_disp_op(oper, stack_op->dest(DEST1), pred, sp_type);
      spill_op->set_dest(DEST1, temp);
      if (where_to_put_it == BEFORE_OP) {
	El_insert_op_before(stack_op, pivot_op);
	El_insert_op_before(spill_op, pivot_op);
	El_insert_op_before(mask_op, pivot_op);
      } else {
	mask_op->set_flag(EL_OPER_CALLER_SAVE);
	stack_op->set_flag(EL_OPER_CALLER_SAVE);	
	El_insert_op_after(mask_op, pivot_op);
	El_insert_op_after(spill_op, pivot_op);
	El_insert_op_after(stack_op, pivot_op);
      }
      return spill_op;
    }
  }
}


Op* insert_spill_op(Operand oper, Operand pred, Compound_region* r, Op* pivot_op,
		    spill_location where_to_put_it, spill_type sp_type)
{
  bool bd_supported = true;

  // checks to see if this architecture supports base+displacement addressing
  if (MDES_unsupported("LG_W_C1_C1") || MDES_unsupported("FLG_D_C1_C1") ||
      MDES_unsupported("SG_W_C1") || MDES_unsupported("FSG_D_C1")) {
    El_punt("I don't think spill code for architectures without base+displacement addressing works yet...");
    bd_supported = false;
  }

  if (r->flag(EL_REGION_SOFTPIPE)) {
    // Rotating register allocation happens elsewhere in the program,
    // so if we're spilling in a modulo scheduled loop, then there's
    // problems.
    cdbg << "Trying to insert spill op for vr: " << oper.vr_num() 
	 << " in file: " << (char*)oper.physical_file_type() 
	 << " in software pipelined region: " << r->id() << " of proc: "
	 << (el_get_enclosing_procedure(r))->get_name() << endl;
    assert(0);
  }

  Op* spill_op = NULL;

//   if(oper.file_type() == PR) {
//     spill_op = insert_spill_for_pred(oper, pred, r, pivot_op, where_to_put_it,
// 				     sp_type, bd_supported);
//   } else {
    if (bd_supported) {
      spill_op = get_base_disp_op(oper, pred, sp_type);
      if (where_to_put_it == BEFORE_OP)
	El_insert_op_before(spill_op, pivot_op);
      else
	El_insert_op_after(spill_op, pivot_op);

      // insert extra PRESTOREG for spilled OR or AND type CMPPs
      if(is_cmpp(pivot_op)) {
        for (int i=DEST1; i <= DEST2; i++) {
          if(pivot_op->dest(i) == oper) {
            if (is_O_pred(pivot_op, i) || is_A_pred(pivot_op, i)) {
              Op* bd_op = new Op((Opcode) PRESTOREG);
              Op* add_op = get_stack_op(oper, pred);

              int memvr;
              int vr_num = Scalar_reg_alloc::spill_stack.get_alias(oper.vr_num());
              if (vr_to_memvr.is_bound(vr_num)) {
                memvr = vr_to_memvr[vr_num];
              } else {
                memvr = virtual_register_number++;
                vr_to_memvr.bind(vr_num, memvr);
              }
              
              Operand memvr_oprnd = new Mem_vr(memvr); 
              // it's a load
              bd_op->set_dest(DEST1, oper);
              bd_op->set_src(SRC1, add_op->src(SRC1));
              bd_op->set_src(SRC2, add_op->src(SRC2));
              bd_op->add_mem_src(memvr_oprnd);
              bd_op->set_src(PRED1, Operand(new Pred_lit(true)));
              bd_op->set_flag(EL_OPER_SPILL_CODE);

              delete add_op; // delete mem_op;
              El_insert_op_before(bd_op, pivot_op);

              if(dbg(sa, 2))
                cdbg << "inserted extra PRESTORE op for a OR type CMPP" << bd_op->id() << endl;
            }
          }
        }
      }
    } else {
      // If base+disp isn't supported, then we need two operations: one
      // ADD to get the proper stack location, and one load or store.
      Op* stack_op = get_stack_op(oper, pred);
      spill_op = get_non_base_disp_op(oper, stack_op->dest(DEST1), pred, sp_type);
      if (where_to_put_it == BEFORE_OP) {
	El_insert_op_before(stack_op, pivot_op);
	El_insert_op_before(spill_op, pivot_op);
      } else {
	El_insert_op_after(spill_op, stack_op);
	El_insert_op_after(stack_op, pivot_op);
      }
      
      if((sp_type == SPILL_LOAD && where_to_put_it == AFTER_OP) ||
	 (sp_type == SPILL_STORE && where_to_put_it == BEFORE_OP))
	stack_op->set_flag(EL_OPER_CALLER_SAVE);
    }
//   }

  return spill_op;
}


