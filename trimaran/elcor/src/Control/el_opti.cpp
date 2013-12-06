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
//      File:           el_opti.cpp
//      Author:         David I. August
//      Created:        May 1995
//      Description:    Classic Optimizations
//
/////////////////////////////////////////////////////////////////////////////

#include "iterators.h"
#include "opcode_properties.h"
#include "hash_set.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "edge_utilities.h"
#include "flow_analysis_solver.h"
#include "op.h"
#include "attributes.h"
#include "edge_attributes.h"

bool El_is_used_between(Compound_region* reg, Op* op1, Op* op2, Operand operand)
{
  bool found = false;
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if((! found) && (cur_op == op1 || cur_op == op2))
	{
	  found = true;
	  continue;
	}
      if(!found)
	{
	  continue;
	}
      for(Op_all_inputs operand_iter(cur_op); operand_iter!=0; operand_iter++)
	{
	  if(*operand_iter == operand)
	    {
	      return true;
	    }
	}  
      if(cur_op == op1 || cur_op == op2)
	{
	  break;
	}
    }

  return false;
}

bool El_is_redefined_between(Compound_region* reg, Op* op1, Op* op2, Operand operand)
{
  bool found = false;
  for(Region_ops_C0_order op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if((! found) && (cur_op == op1 || cur_op == op2))
	{
	  found = true;
	  continue;
	}
      if(!found)
	{
	  continue;
	}
      for(Op_all_dests operand_iter(cur_op); operand_iter!=0; operand_iter++)
	{
	  if(*operand_iter == operand)
	    {
	      return true;
	    }
	} 
      if(cur_op == op1 || cur_op == op2)
	{
	  break;
	}
    }

  return false;
}


bool El_is_live(Compound_region* block, Op* op, Operand operand)
{ 
  if(operand.is_undefined())
    {
      return false;
    }

  bool found_op = false;  
  for(Region_all_outedges edge_iter(block); edge_iter != 0; edge_iter++)
    {
      Edge* cur_edge = *edge_iter;
      Liveness_info* edge_live = get_liveness_info(cur_edge);

      if(edge_live->is_member(operand))
	{
	  return true;
	}
    }
  for(Region_all_ops  op_iter(block); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if(!found_op && cur_op != op)
	{
	  continue;
	}
      if(cur_op == op)
	{
	  found_op = true;
	  continue;
	}
      for(Op_all_inputs operand_iter(cur_op); operand_iter!=0; operand_iter++)
	{
	  if(*operand_iter == operand)
	    {
	      return true;
	    }
	}
    }

  return false;
}


bool El_cmpp_same_test(Op* op1, Op* op2)
{
  if(!is_cmpp(op1) || !is_cmpp(op2))
    {
      return false;
    }
  if(op1->src(SRC1) != op2->src(SRC1))
    {
      return false;
    }
  if(op1->src(SRC2) != op2->src(SRC2))
    {
      return false;
    }

  IR_BASE_OPCODE base1 = get_base(op1->opcode());
  IR_BASE_OPCODE base2 = get_base(op2->opcode());
  if(base1 != base2)
    {
      return false;
    }

  return true;
}

bool El_cmpp_opposite_test(Op* op1, Op* op2)
{
  if(!is_cmpp(op1) || !is_cmpp(op2))
    {
      return false;
    }
  if(op1->src(SRC1) != op2->src(SRC1))
    {
      return false;
    }
  if(op1->src(SRC2) != op2->src(SRC2))
    {
      return false;
    }

  IR_BASE_OPCODE base1 = get_base(op1->opcode());
  IR_BASE_OPCODE base2 = get_base(get_opposite_cond(op2->opcode()));
  if(base1 != base2)
    {
      return false;
    }

  return true;
}


/*
** Removes dead code - Assumes existance of liveness attribute on the region
*/

void El_dead_code_elimination(Compound_region* reg)
{
  Hash_set<Compound_region*> blocks(hash_cmpd_region_ptr, 128);
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      blocks += cur_op->parent();
    }

  /*
  ** Find all dead ops
  */
  for(Hash_set_iterator<Compound_region*> block_iter(blocks); block_iter != 0; block_iter++)
    {
      Hash_set<Op*> del_list(hash_op_ptr, 128);
      Compound_region* cur_block = *block_iter;
      for(Region_ops_reverse_C0_order op_iter(cur_block); op_iter != 0; op_iter++)
	{
	  Op* cur_op = *op_iter;

	  if(!has_side_effect(cur_op) && !is_pseudo(cur_op))
	    {
	      bool live = false;
	      for(Port_num port_indx = cur_op->first_dest();
		  port_indx <= cur_op->last_dest();
		  port_indx++)
		{
		  Operand cur_operand = cur_op->dest(port_indx);
		  
		  if(El_is_live(cur_block, cur_op, cur_operand))
		    {
		      live = true;
		    }
		  else
		    {
		      /* Kill dead operand */
		      Operand undef;
		      cur_op->set_dest(port_indx, undef);
		    }
		}
	      if (!live)
		{
		  del_list += cur_op;
		}
	    }
	} 

      /* 
      ** Remove all ops in del_list
      */
      for(Hash_set_iterator<Op*> del_iter(del_list); del_iter!=0; del_iter++)
	{
	  Op* del_op = *del_iter;
	  cout << *del_op;
       	  delete El_remove_op(del_op);
	}
    }
}

