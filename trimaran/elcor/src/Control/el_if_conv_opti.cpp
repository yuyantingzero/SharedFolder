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
//      File:           el_if_conv_opti.cpp
//      Author:         David I. August, Scott A. Mahlke, Matthai Philipose
//      Created:        July 1995
//      Description:    Optimizations closely coupled w/ if-conv
//
/////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "iterators.h"
#include "el_bb_tools.h"
#include "hash_map.h"
#include "hash_set.h"
#include "el_control.h"
#include "edge_utilities.h"
#include "op.h"
#include "edge.h"
#include "el_opti.h"
#include "connect.h"
#include "opcode_properties.h"
#include "attributes.h"
#include "edge_attributes.h"
#include "el_normalize_branches.h"
#include "pred_analysis.h"
#include "el_bb_tools.h"
#include "el_if_conv_opti.h"


/*
 *	Currently there are 3 if-conv optimizations
 *		1. exit combining
 *		2. compare combining (2 target cmpps)
 *		3. OR-type to unconditional cmpp conversion
 */



/*=======================================================================*/
/*
 *	Exit combining
 */
/*=======================================================================*/

void El_exit_combine(Compound_region* reg)
{
  Hash_set<Edge*> exit_edges(hash_edge_ptr, 16);
  Op* first_op = get_first_region_op_from_subregions(reg);
  for (Region_exit_edges exit_iter(reg); exit_iter!=0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->dest() == first_op)
	{
	  continue;
	}
      if(cur_edge->dest_alt() != 0)
	{
	  exit_edges += cur_edge;
	}
    }

  El_exit_combine(reg, exit_edges);

}


void El_exit_combine(Compound_region* reg, Hash_set<Edge*> exit_edges)
{
  if(!reg->flag(EL_REGION_HYPERBLOCK_FRP))
    {
      El_punt("El_exit_combine: Function only works on fully resolved prediate HB's");
    }

  Edge* fall_through_edge = NULL;
  Op* last_op = get_last_region_op_from_subregions(reg);
  Op* first_op = get_first_region_op_from_subregions(reg);
  
  Region_exit_edges exit_iter ;
  for (exit_iter(reg); exit_iter!=0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->dest() == first_op)
	{
	  continue;
	}
      if(cur_edge->dest_alt() == 0)
	{
	  fall_through_edge = cur_edge;
	  exit_edges -= cur_edge;
	}
    }
  
  if(exit_edges.size() == 0)
    {
      return;
    }

  Hyperblock* exit_block = new Hyperblock;
  Compound_region* parent = reg->parent();
  exit_block->set_parent(parent);
  parent->insert_after_region(exit_block, reg);

  exit_block->set_flag(EL_REGION_HYPERBLOCK);
  if(reg->flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU))
    {
      reg->reset_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU); 
      exit_block->set_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU); 
    }

  Op* merge_op = new Op(C_MERGE);
  exit_block->add_region(merge_op);
  merge_op->set_parent(exit_block);
  exit_block->add_entry(merge_op);
  
  Op* dummy_branch = new Op(DUMMY_BR);
  exit_block->add_region(dummy_branch);
  dummy_branch->set_parent(exit_block);
  exit_block->add_exit(dummy_branch);

  C0_connect_fallthrough(merge_op, dummy_branch);
  
  if(fall_through_edge)
    {
      last_op->remove_outedge_recursively(fall_through_edge);
      dummy_branch->add_outedge_recursively(fall_through_edge);
      fall_through_edge->set_src(dummy_branch, fall_through_edge->src_port(), 
				 fall_through_edge->src_alt());
    }

  if(!is_dummy_branch(last_op))
    {
      Op* new_dummy = new Op(DUMMY_BR);
      reg->add_region(new_dummy);
      new_dummy->set_parent(reg);
      reg->add_exit(new_dummy);
      Edge* new_edge = C0_connect_fallthrough(new_dummy, merge_op);
      merge_op->add_inedge_recursively(new_edge);
      new_dummy->add_outedge_recursively(new_edge);

      C0_connect_fallthrough(last_op, new_dummy);
    }

  for(Hash_set_iterator<Edge*> edge_iter(exit_edges); edge_iter != 0; edge_iter++)
    {
      Edge* cur_edge = *edge_iter;
      Op* branch = cur_edge->src();
      
      Op* cond = El_find_cond_for_branch(branch);
      Op* pbr = El_find_pbr_for_branch(branch);
      
      /*
      ** Move branch and edge to exit_block
      */
      //cout << "CUR_EDGE " << *cur_edge << endl;
      for(Region_all_outedges ei(cur_edge->src()); ei!=0; ei++)
	{
	  //cout << "EDGE " << *(*ei) << endl;
	}
      branch->remove_outedge_recursively(cur_edge);
      El_remove_op(branch);
      reg->remove_exit(branch);

      El_insert_op_before_switch(exit_block, branch);
      exit_block->add_exit(branch);
      branch->add_outedge_recursively(cur_edge);
      branch->set_parent(exit_block);

      /* 
      ** Copy pbr to exit_block
      */
      Op* new_op = new Op;
      int id = new_op->id();
      *new_op = *pbr;
      new_op->set_id(id);
      El_insert_op_before(new_op, branch);

      /*
      ** Copy cond to exit_block
      */
      if(cond &&
	 !El_is_redefined_between(reg, cond, last_op, cond->src(SRC1)) &&
	 !El_is_redefined_between(reg, cond, last_op, cond->src(SRC2)))
	{  
	  new_op = new Op;
	  int id = new_op->id();
	  *new_op = *cond;
	  new_op->set_id(id);
	  El_insert_op_before(new_op, branch);
	}
    }

  /*
  ** Remove dummy branch from exit_block
  */
  fall_through_edge = NULL;
  for (exit_iter(exit_block); exit_iter!=0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->dest_alt() == 0)
	{
	  fall_through_edge = cur_edge;
	}
    }
  if(!fall_through_edge)
    {
      Op* bad_dummy = get_last_region_op_from_subregions(exit_block);
      Edge* src_edge = get_incoming_CONTROL0_edge(bad_dummy);
      src_edge->src()->remove_outedge(src_edge, src_edge->src_port_type(), src_edge->src_port(), src_edge->src_alt());
      exit_block->remove_region(bad_dummy);
      exit_block->remove_exit(bad_dummy);
      delete src_edge;
      delete bad_dummy;
    }
  else
    {
      Op* bad_dummy = get_last_region_op_from_subregions(exit_block);
      Edge* dest_edge = get_outgoing_CONTROL0_edge(bad_dummy);
      Edge* src_edge = get_incoming_CONTROL0_edge(bad_dummy);
      Op* new_last_op = src_edge->src();
      new_last_op->remove_outedge(src_edge, src_edge->src_port_type(), src_edge->src_port(), src_edge->src_alt());
      new_last_op->add_outedge(dest_edge, dest_edge->src_port_type(), dest_edge->src_port(), dest_edge->src_alt());
      dest_edge->set_src(new_last_op, dest_edge->src_port(), dest_edge->src_alt());
      exit_block->remove_region(bad_dummy);
      exit_block->remove_exit(bad_dummy);
      delete src_edge;
      delete bad_dummy; 
    }

  /* 
  ** Remove dummy branch from reg if necessary
  */
  Op* bad_dummy = get_last_region_op_from_subregions(reg);
  Edge* src_edge = get_incoming_CONTROL0_edge(bad_dummy);
  if(is_branch(src_edge->src()))
    {
      Op* new_last_op = src_edge->src(); 
      Edge* dest_edge = get_outgoing_CONTROL0_edge(bad_dummy);
      new_last_op->remove_outedge(src_edge, src_edge->src_port_type(), src_edge->src_port(), src_edge->src_alt());
      new_last_op->add_outedge(dest_edge, dest_edge->src_port_type(), dest_edge->src_port(), dest_edge->src_alt());
      dest_edge->set_src(new_last_op, dest_edge->src_port(), dest_edge->src_alt());
      reg->remove_exit(bad_dummy);
      reg->remove_region(bad_dummy);
      delete src_edge;
      delete bad_dummy; 
    } 

  for(Region_all_ops rxop(exit_block); rxop!=0 ; rxop++)
    {
      cout << "NEW OP" << endl;
      cout << **rxop << endl;
      for (Op_outedges edgei(*rxop,CONTROL0_OUTEDGES) ;
	   edgei != 0 ; edgei++) 
	{
	  cout << **edgei << endl;
	}
    }
  for(Region_exit_ops rop(exit_block); rop!=0 ; rop++)
    {
      cout << "NEW OP" << endl;
      cout << **rop << endl;
      for (Op_outedges edgei(*rop,CONTROL0_OUTEDGES) ;
	   edgei != 0 ; edgei++) 
	{
	  cout << **edgei << endl;
	}
    }
  
}


/*=======================================================================*/
/*
 *	Combining single target cmpps to dual target
 */
/*=======================================================================*/

/*
** Combines single destination predicate defines.
*/
void El_combine_single_dest_pred_defines(Hyperblock* reg)
{
  /* 
  ** Make a list of all ops that should be considered.
  */
  Hash_set<Op*> pred_defs(hash_op_ptr, 16);
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if(is_cmpp(cur_op))
	{
	  if(cur_op->dest(DEST1).is_undefined())
	    {
	      Operand temp = cur_op->dest(DEST2);
	      cur_op->set_dest(DEST2, cur_op->dest(DEST1));
	      cur_op->set_dest(DEST1, temp);
	      Opcode new_opc = cmpp_dest2_to_dest1(cur_op->opcode());
	      new_opc = (Opcode)(new_opc | (cur_op->opcode() & ~IR_DEST1_MASK));
	      cur_op->set_opcode(new_opc);
	    }
	  if(cur_op->dest(DEST2).is_undefined())
	    {
	      pred_defs += cur_op;
	    }
	}
    }

  /*
  ** Of the considered ops, match up the ones that can be combined.
  */
  Hash_set<Op*> merged_ops(hash_op_ptr, 16);
  Map<Op*, Op*> merges;
  for(Hash_set_iterator<Op*> indx1(pred_defs); indx1 != 0; indx1++)
    {
      for(Hash_set_iterator<Op*> indx2(pred_defs); indx2 != 0; indx2++)
	{
	  Op* op1 = *indx1;
	  Op* op2 = *indx2;
	  if(op1 == op2)
	    {
	      continue;
	    }
	  if(merged_ops.is_member(op1) || merged_ops.is_member(op2))
	    {
	      continue;
	    }
	  if(op1->src(PRED1) != op2->src(PRED1))
	    {
	      continue;
	    }
	  if(El_cmpp_same_test(op1, op2))
	    {
	      if(!El_is_redefined_between(reg, op1, op2, op1->src(SRC1)) &&
		 !El_is_redefined_between(reg, op1, op2, op1->src(SRC2)))
		{
		  merged_ops += op1;
		  merged_ops += op2;
		  merges.bind(op1, op2);
		}
	      continue;
	    }
	  if(El_cmpp_opposite_test(op1, op2) && is_U_pred(op2, DEST1))
	    {
	      if(!El_is_redefined_between(reg, op1, op2, op1->src(SRC1)) &&
		 !El_is_redefined_between(reg, op1, op2, op1->src(SRC2)))
		{
		  merged_ops += op1;
		  merged_ops += op2;
		  merges.bind(op1, op2);
		}
	    }
	}
    }

  /*
  ** Perform actual combination
  */
  for(Map_iterator<Op*, Op*> map_iter(merges); map_iter != 0; map_iter++)
    {
      Op* exist_op = (*map_iter).first;
      Op* del_op = (*map_iter).second;
      
      exist_op->set_dest(DEST2, del_op->dest(DEST1));
      if(El_cmpp_opposite_test(exist_op, del_op))
	{
	  Opcode new_opc = cmpp_dest1_to_dest2(del_op->opcode());
	  new_opc = (Opcode)(new_opc | (exist_op->opcode() & ~IR_DEST2_MASK));
	  new_opc = make_complement_pred(new_opc, DEST2);
	  exist_op->set_opcode(new_opc);
	}
      else
	{
	  Opcode new_opc = cmpp_dest1_to_dest2(del_op->opcode());
	  new_opc = (Opcode)(new_opc | (exist_op->opcode() & ~IR_DEST2_MASK));
	  exist_op->set_opcode(new_opc);
	}
      
      delete El_remove_op(del_op);
    }
}  

void El_combine_single_dest_pred_defines(Compound_region *reg)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rtmp;

    rstack.push(reg);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
            El_combine_single_dest_pred_defines((Hyperblock*)rtmp);
            push_flag = false;  // Only ops below, so don't waste time!
        }
        else if (rtmp->is_bb()) {
            push_flag = false;  // Only ops below, so don't waste time!
        }
        if (push_flag==true) {
            for (Region_subregions subri(rtmp); subri!=0; subri++) {
                if ((*subri)->is_compound())
                    rstack.push(*subri);
            }
        }
    }
}


/*=======================================================================*/
/*
 *	Converting OR-type cmpps to unconditional
 */
/*=======================================================================*/

/*
** Removes useless pred_clear instructions
*/
void El_reduce_pred_defines(Compound_region* reg)
{
  /* 
  ** Find all predicate clears which qualify
  */
  Hash_set<Op*> del_list(hash_op_ptr, 16);
  for(Region_all_ops op_iter1(reg); op_iter1 != 0; op_iter1++)
    {
      Op* cur_op = *op_iter1;
      if(is_predicate_clear(cur_op))
	{
	  Hash_set<Op*> op_list(hash_op_ptr, 16);
	  for(Region_all_ops op_iter2(reg); op_iter2 != 0; op_iter2++)
	    {
	      Op* search_op = *op_iter2;
	      if(is_cmpp(search_op))
		{
		  if(search_op->dest(DEST1) == cur_op->dest(DEST1))
		    {
		      op_list += search_op;
		    }
		  if(search_op->dest(DEST2) == cur_op->dest(DEST1))
		    {
		      op_list += search_op;
		    }
		}
	    }
	  /* 
	  ** If the single op qualifies, make it unconditional
	  */
	  if(op_list.size() == 1)
	    {
	      Op* def_op = op_list.head();
	      if(def_op->dest(DEST1) == cur_op->dest(DEST1))
		{
		  if(is_O_pred(def_op, DEST1))
		    {
		      def_op->set_opcode(make_U_pred(def_op->opcode(), DEST1));
		      del_list += cur_op;
		    }
		}
	      if(def_op->dest(DEST2) == cur_op->dest(DEST1))
		{
		  if(is_O_pred(def_op, DEST2))
		    {
		      def_op->set_opcode(make_U_pred(def_op->opcode(), DEST2));
		      del_list += cur_op;
		    }
		}
	    }
	}
    }

  /* 
  ** Remove all ops in del_list
  */
  for(Hash_set_iterator<Op*> del_iter(del_list); del_iter!=0; del_iter++)
    {
      delete El_remove_op(*del_iter);
    }

}
