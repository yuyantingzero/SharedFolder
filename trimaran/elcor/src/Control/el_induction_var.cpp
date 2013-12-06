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
//      File:           el_induction_var.cpp
//      Author:         David I. August
//      Created:        May 1995
//      Description:    Control Tools
//
/////////////////////////////////////////////////////////////////////////////

#include "el_induction_var.h"
#include "op.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "attributes.h"


bool El_sub_one_from_loop_count(Opcode opcode)
{
  if(is_int_gt_cmpp(opcode) ||
     is_int_lt_cmpp(opcode) ||
     is_int_neq_cmpp(opcode))
    {
      return true;
    }
  else
    {
      return false;
    }
}

Opcode El_add_opcode_for_induction_op(Op* op)
{
  if(op->opcode() == ADD_W ||
     op->opcode() == SUB_W)
    {
      return ADD_W;
    }
  if(op->opcode() == ADDL_W ||
     op->opcode() == SUBL_W)
    {
      return ADDL_W;
    }
  El_punt("El_add_opcode_for_induction_op: unsupported induction op");
  return NO_OP;
}

Opcode El_subtract_opcode_for_induction_op(Op* op)
{
  if(op->opcode() == ADD_W ||
     op->opcode() == SUB_W)
    {
      return SUB_W;
    }
  if(op->opcode() == ADDL_W ||
     op->opcode() == SUBL_W)
    {
      return SUBL_W;
    }
  El_punt("El_subtract_opcode_for_induction_op: unsupported induction op");
  return NO_OP;
}

Opcode El_divide_opcode_for_induction_op(Op* op)
{
  if(op->opcode() == ADD_W ||
     op->opcode() == SUB_W)
    {
      return DIV_W;
    }
  if(op->opcode() == ADDL_W ||
     op->opcode() == SUBL_W)
    {
      return DIVL_W;
    }
  El_punt("El_divide_opcode_for_induction_op: unsupported induction op");
  return NO_OP;
}

bool El_op_has_induction_pattern(Op* op)
{
  if (op->opcode() != ADD_W &&
      op->opcode() != ADDL_W &&
      op->opcode() != SUB_W &&
      op->opcode() != SUBL_W)
    {
      return false;
    }
  
  if(!op->dest(DEST1).is_reg())
    {
      return false;
    }
  
  if(op->dest(DEST1) != op->src(SRC1))
    {
      return false;
    }  

  return true;
  
}

bool El_is_invariant(Compound_region* region, Operand* operand)
{
  if(!operand || !region)
    {
      El_punt("El_is_invariant: passed NULL pointer.");
    }

  if(operand->is_undefined())
    {
      El_punt("El_is_invariant: Operand is undefined.");
    }
  
  if(operand->is_vr_name() || operand->is_mem_vr())
    {
      return false;
    }

  if(operand->is_lit())
    {
      return true;
    }

  assert(operand->is_reg() || operand->is_macro_reg());
  
  for(Region_all_ops op_iter(region); op_iter!=0; op_iter++)
    {
      Op* cur_op = (*op_iter);

      if (operand->is_macro_reg() && operand->is_fragile_macro() && is_call(cur_op))
	{
	  return false;
	}

      for(Port_num port_indx = cur_op->first_dest();
	  port_indx <= cur_op->last_dest();
	  port_indx++)
	{
	  if (*operand == cur_op->dest(port_indx))
	    {
	      return false;
	    }
	}
    }
  
  return true;
  
}

/*
 *  unique_def = 1 or more defs of an operand but all defs are equivalent
 */
bool El_is_unique_def(Compound_region* region, Op* op)
{
  bool match;
  
  for(Port_num port_indx = op->first_dest();
      port_indx <= op->last_dest();
      port_indx++)
    {
      Operand* dest = &op->dest(port_indx);
      
      for(Region_all_ops op_iter(region); op_iter!=0; op_iter++)
	{
	  Op* cur_op = (*op_iter);

	  if(cur_op == op)
	    {
	      continue;
	    }

	  match = false;
	  for(Port_num port_indx2 = cur_op->first_dest();
	      port_indx2 <= cur_op->last_dest();
	      port_indx2++)
	    {
	      if(*dest == cur_op->dest(port_indx2))
		 {
		   match = true;
		   break;
		 }
	    }
	  if(!match)
	    {
	      continue;
	    }
	  if(*op != *cur_op)
	    {
	      return false;
	    }
	}
    }
  return true;
}


void El_switch_operands(Op* op)
{
  Opcode new_opcode = get_reverse_op(op->opcode());

  if(!new_opcode)
    {
      El_punt("El_switch_operands: Cannot switch operands, bad op.");
    }
  op->set_opcode(new_opcode);
  Operand temp_operand(op->src(SRC1)); 
  op->set_src(SRC1, op->src(SRC2));
  op->set_src(SRC2, temp_operand);

  // SAM 4-2000 Need to swap the Reference_dtype_attr width annots too!!!
  Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
  if (rdt_attr != NULL) {
      rdt_attr->swap_srcs(SRC1, SRC2);
  }
}

/*
 *  Operands are placed in this order whenever possible.
 *  Registers, macros, literals
 */

void El_normalize_operands_by_type(Compound_region* region)
{
  for(Region_all_ops op_iter(region); op_iter!=0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if(get_reverse_op(cur_op->opcode()))
	{
	  if(cur_op->src(SRC1).is_lit() && cur_op->src(SRC2).is_reg())
	    {
	      El_switch_operands(cur_op);
	      continue;
	    }
	  if(cur_op->src(SRC1).is_lit() && cur_op->src(SRC2).is_macro_reg())
	    {
	      El_switch_operands(cur_op);
	      continue;
	    }
	  if(cur_op->src(SRC1).is_macro_reg() && cur_op->src(SRC2).is_reg())
	    {
	      El_switch_operands(cur_op);
	      continue;
	    }
	}
    }
}


/*
 *  Operands are placed in this order whenever possible.
 *  Operands in list, other
 */

void El_normalize_operands_by_list(Compound_region* region,
				   Hash_set<Operand>& operands)
{
  for(Region_all_ops op_iter(region); op_iter!=0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if(get_reverse_op(cur_op->opcode()))
	{
	  if(operands.is_member(cur_op->src(SRC2)))
	    {
	      El_switch_operands(cur_op);
	      continue;
	    }
	}
    }
}









