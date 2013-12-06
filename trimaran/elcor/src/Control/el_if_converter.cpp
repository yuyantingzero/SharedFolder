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
//      File:           el_if_converter.cpp
//      Author:         David I. August, Scott A. Mahlke, Matthai Philipose
//      Created:        July 1995
//      Description:    If conversion class definition
//
/////////////////////////////////////////////////////////////////////////////

#include "control_analysis_solver.h"
#include "iterators.h"
#include "list.h"
#include "el_bb_tools.h"
#include "map.h"
#include "el_if_converter.h"
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
#include "dbg.h"
#include "pred_analysis.h"
#include "el_bb_tools.h"
#include "el_if_conv_opti.h"

void El_if_convert(Compound_region* reg, bool frp)
{
  El_If_Converter if_conv(reg);

  Hash_set<Compound_region*> bb_set(hash_cmpd_region_ptr, 128);

  Op* entry_op = NULL;
  int count=0;
  for(Region_entry_ops eop_iter(reg); eop_iter!=0; eop_iter++,count++)
    {
      entry_op= *eop_iter;
    }
  //Can only if-convert regions with a singole
  assert(count==1);
  assert(entry_op->parent()->is_bb());

  for(Region_all_ops op_iter(reg); op_iter!=0; op_iter++)
    {
      Op* op=*op_iter;
      bb_set += op->parent();
    }
  Hyperblock* hb = if_conv.if_convert(bb_set, entry_op->parent(), frp);

  El_reduce_pred_defines(hb);

  // create_local_analysis_info_for_all_hbs_bbs(reg);
//   el_flow_compute_liveness(reg);
//   El_dead_code_elimination(hb);
//   // delete vr_maps and PQS  
//   delete_local_analysis_info_for_all_hbs_bbs(reg);
  
  El_combine_single_dest_pred_defines(hb);

}


/*
** Inserts a pred clear at top of bb
*/
void El_insert_pred_clear(Basicblock* bb, Operand pred)
{
  /* Insert pred clear after merge op in bb */
  
  Op* def_op = new Op(PRED_CLEAR);
  def_op->set_dest(DEST1, pred);

  Pred_lit* new_pred_lit = new Pred_lit(true);
  Operand pred_true(new_pred_lit);
  def_op->set_src(PRED1, pred_true);

  El_insert_op_after_merge(bb, def_op);
}

/*
** Inserts a predicate AND operation sequence
*/
void El_insert_AND(Op* op, Operand src1, Operand src2, Operand dest)
{
  Op* and_op = new Op(PRED_SET);
  and_op->set_dest(DEST1, dest);
  El_insert_op_before(and_op, op);
  
  and_op = new Op(PRED_AND);
  and_op->set_src(SRC1, src1);
  and_op->set_dest(DEST1, dest);
  El_insert_op_before(and_op, op);
  
  and_op = new Op(PRED_AND);
  and_op->set_src(SRC1, src2);
  and_op->set_dest(DEST1, dest);
  El_insert_op_before(and_op, op);
}


/*
** Returns predicate on branch
*/
Operand El_find_predicate_on_branch(Op* branch)
{
  if(!is_conditional_branch(branch))
    {
      El_punt("El_find_predicate_on_branch: Only works on conditional branches.");
    }
  return branch->src(PRED1);
}

/*
** Returns branch condition predicate
*/
Operand El_find_branch_predicate(Op* branch)
{
  if(!is_conditional_branch(branch))
    {
      El_punt("El_find_branch_predicate: Only works on conditional branches.");
    }
  return branch->src(SRC2);
}


/*
** Switches taken and fall through path
*/
void El_reverse_bb_branch(Basicblock* bb)
{
  Op* branch = El_find_branch_op(bb);

  assert(is_conditional_branch(branch));

  if(is_bru(branch))
    {
      El_punt("El_reverse_bb_branch: Unsupported operation.");
    }

  
  if(!branch->src(PRED1).is_predicate_true())
    {
      /* FIX LATER: This can be fixed using a precicate AND instruction */
      El_punt("El_reverse_bb_branch: Unsupported operation.");
    }

  // Reverse branch op 
  if(is_brcf(branch))
    {
      branch->set_opcode(BRCT);
    }
  else
    {
      branch->set_opcode(BRCF);
    }

  Edge* taken_edge = NULL;
  Edge* fall_through_edge = NULL;
  
  for(Region_exit_edges exit_iter(bb); exit_iter!=0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->dest_alt() == 0)
	{
	  fall_through_edge = cur_edge;
	}
      else
	{
	  taken_edge = cur_edge;
	}
    }
  
  // Switch alt_nums on branch edges
  int temp_alt = taken_edge->dest_alt(); 
  taken_edge->set_dest(taken_edge->dest(), taken_edge->dest_port(), fall_through_edge->dest_alt());
  fall_through_edge->set_dest(fall_through_edge->dest(), fall_through_edge->dest_port(), temp_alt);
  
  temp_alt = taken_edge->src_alt(); 
  taken_edge->set_src(taken_edge->src(), taken_edge->src_port(), fall_through_edge->src_alt());
  fall_through_edge->set_src(fall_through_edge->src(), fall_through_edge->src_port(), temp_alt);
}


/*
** This function updates the list of predicates that are guarenteed to be defined
** in the hyperblock.
*/
void El_update_define_predicate(Op* op, Hash_set<Operand>& defined_predicates)
{
  if(is_predicate_clear(op) || is_predicate_set(op))
    {
      if(!op->src(PRED1).is_predicate_true())
	{
	  El_punt("El_update_define_predicate: Predicat set/clear cannot be predicated");
	}
      defined_predicates += op->dest(DEST1);
    }
  else
    {
      for(Port_num port_indx = op->first_dest();
	  port_indx <= op->last_dest();
	  port_indx++)
	{
	  if(op->dest(port_indx).is_predicate() && is_U_pred(op, port_indx))
	    {
	      defined_predicates += op->dest(port_indx);
	    }
	}
    }
}

void El_update_all_bb_defined_predicates(Basicblock* bb, Hash_set<Operand>& defined_predicates)
{
  for(Region_all_ops op_iter(bb); op_iter!=0; op_iter++)
    {
      El_update_define_predicate(*op_iter, defined_predicates);
    }
}


/*
** This function inserts OR type predicate defines in current block and
** inserts predicate clears at top of HB.
*/
void El_insert_predicate_defines(Basicblock* bb,
				 bool taken,
				 Hash_set<Operand>& predicates,
				 Hash_set<Operand>& defined_predicates,
				 Basicblock* header)
{

  Op* branch = El_get_conditional_branch_op(bb);
  Op* cond = El_get_cond_for_branch(branch);
  /* DO THIS */
  /*if(El_cond_is_redefined(bb, cond, branch))
    {
    El_punt("El_insert_predicate_defines: cond sources redefined");
    }
    */
  bool cond_sense = taken;

  if(is_brcf(branch))
    {
      cond_sense = !cond_sense;
    }
  
  for(Hash_set_iterator<Operand> pred_iter(predicates); pred_iter!=0; pred_iter++)
    {
      Operand cur_pred = *pred_iter;
      if(!defined_predicates.is_member(cur_pred))
	{
	  El_insert_pred_clear(header, cur_pred);
	  defined_predicates += cur_pred;
	}
      
      Op* pred_def_op;
      if(cond_sense)
	{
	  pred_def_op = new Op(make_ON_pred(cond->opcode(), DEST1));
	}
      else
	{
	  pred_def_op = new Op (make_ON_pred(get_opposite_cond(cond->opcode()), DEST1));
	}
	  
      pred_def_op->set_dest(DEST1, cur_pred);
      pred_def_op->set_src(SRC1, cond->src(SRC1));
      pred_def_op->set_src(SRC2, cond->src(SRC2));
      Pred_lit* new_pred_lit = new Pred_lit(true);
      Operand pred_true(new_pred_lit);
      pred_def_op->set_src(PRED1, pred_true);
      El_insert_op_before_switch(bb, pred_def_op);
    }

}

/*
** This function inserts OR type predicate defines in current block and
** inserts predicate clears at top of HB.
*/
void El_insert_predicate_defines_normalized(Basicblock* bb,
					    bool edge_val,
					    Hash_set<Operand>& predicates,
					    Hash_set<Operand>& defined_predicates,
					    Basicblock* header)
{
  
  Op* branch = El_get_conditional_branch_op(bb);
  assert(is_brdir(branch));//since this is fn is for the normalized form

  for(Hash_set_iterator<Operand> pred_iter(predicates); pred_iter!=0; pred_iter++)
    {
      Operand cur_pred = *pred_iter;
      if(!defined_predicates.is_member(cur_pred))
	{
	  El_insert_pred_clear(header, cur_pred);
	  defined_predicates += cur_pred;
	}
      
      Op* pred_def_op;
      IR_BASE_OPCODE new_opc= el_denormalize_br_opcode((branch->src(SRC1)).int_value());
      
      if(edge_val)
	{
	  pred_def_op = new Op(make_UN_pred(make_ON_pred((Opcode)new_opc, DEST1),
					    DEST2));
	}
      else
	{
	  pred_def_op = new Op(make_UN_pred(make_ON_pred(get_opposite_cond((Opcode)new_opc), DEST1),
					    DEST2));
	}
      
      pred_def_op->set_dest(DEST1, cur_pred);
      pred_def_op->set_src(SRC1, branch->src(SRC2));
      pred_def_op->set_src(SRC2, branch->src(SRC3));
      Pred_lit* new_pred_lit = new Pred_lit(true);
      Operand pred_true(new_pred_lit);
      pred_def_op->set_src(PRED1, pred_true);
      El_insert_op_before_switch(bb, pred_def_op);
    }
  
  return;
}


void El_predicate_basic_block(Basicblock* bb, Operand pred, Hash_set<Operand>& defined_predicates)
{
  Map<Operand, Operand> conjunction;
  
  /*
  ** Make sure that all existing predicates have a pred define/ otherwise redifine.
  */
  for(Region_all_ops op_iter(bb); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;

      /* IF pred set or pred_clear don't predicate */
      if(cur_op->predicated() && !is_predicate_clear(cur_op) && !is_predicate_set(cur_op))
	{
	  if(cur_op->src(PRED1).is_predicate_true())
	    {
	      Operand new_pred(pred);
	      cur_op->set_src(PRED1, new_pred);
	    }
	  else
	    {
	      if(!defined_predicates.is_member(cur_op->src(PRED1)))
		{
		  /* If conjunction does not exist, make one. */
		  if(!conjunction.is_bound(cur_op->src(PRED1)))
		    {
		      Reg* new_reg = new Reg(EL_STANDARD_PREDICATE);
		      Operand new_operand(new_reg);
		      conjunction.bind(cur_op->src(PRED1), new_operand);

		      /* Insert new AND instruction */
		      El_insert_AND(cur_op, pred, cur_op->src(PRED1),
				    new_operand);
		    }
		  cur_op->set_src(PRED1, conjunction.value(cur_op->src(PRED1)));
		}
	    }
	}
      El_update_define_predicate(cur_op, defined_predicates);
    }
}


El_If_Converter::El_If_Converter()
{}

El_If_Converter::El_If_Converter(Compound_region* r)
{
  region = r;
  control_dep = new Control_dep_CFA_solver(region);
}

El_If_Converter::~El_If_Converter()
{
  delete control_dep;
}


void el_create_bb_list_from_cr_list(Hash_set<Compound_region*> blocks,
		    List_set<Basicblock*>& bb_blocks)
{
  for(Hash_set_iterator<Compound_region*> block_iter(blocks);
      block_iter != 0; block_iter++)
    {
      Compound_region* cur_block = *block_iter;
      if(!cur_block->is_bb() && !cur_block->is_hb())
	{
	  El_punt("El_If_Converter::if_convert: blocks may only be HB or BB.");
	}
      for(Region_all_ops op_iter(cur_block); op_iter != 0; op_iter++)
	{
	  Op* cur_op = *op_iter;
	  bb_blocks += (Basicblock*) cur_op->parent();
	}
      if(cur_block->is_hb())
	{
	  El_remove_region(cur_block, true);
	}
      if(cur_block->parent()->is_hb())
	{
	  El_remove_region(cur_block->parent(), true);
	  
	}
    }
}

void el_assign_preds(List_set<Basicblock*>& bb_blocks,
		     Map< Edge*, Hash_set<Operand> >& edge_pred_map,
		     Control_dep_CFA_solver* control_dep,
		      Map< Basicblock*, Operand >& bb_pred_map)
{
  List_set< Hash_set<Edge*> > pred_control;
  Map< Hash_set<Edge*>, Operand > control_pred_map;
  Dlist_iterator<Basicblock*> bb_iter ;
  List_set_iterator<Basicblock*> bb_iter_hs ;
  for (bb_iter_hs(bb_blocks); bb_iter_hs != 0; bb_iter_hs++)
    {
      pred_control += control_dep->control_dep(*bb_iter_hs);
    }
  
  /*
  ** Assign predicates to each edge.
  */
  for (List_set_iterator< Hash_set<Edge*> > pred_iter(pred_control); 
       pred_iter != 0; pred_iter++)
    {
      Reg* new_reg = new Reg(EL_STANDARD_PREDICATE);
      Operand new_operand(new_reg);
      control_pred_map.bind(*pred_iter, new_operand); 
      for (Hash_set_iterator<Edge*> edge_iter(*pred_iter); 
	   edge_iter != 0; edge_iter++)
	{
	  if(!edge_pred_map.is_bound(*edge_iter))
	    {
	      Hash_set<Operand> new_oper_list(hash_operand, 16);
	      edge_pred_map.bind(*edge_iter, new_oper_list);
	    }
	  edge_pred_map.value(*edge_iter) += new_operand;
	}
    }
  
  /*
  ** Assign a predicate to each block
  */

  for (bb_iter_hs(bb_blocks); bb_iter_hs != 0; bb_iter_hs++)
    {
      Basicblock* cur_bb = *bb_iter_hs;
      if(control_dep->control_dep(cur_bb).size() != 0)
	{
	  Hash_set<Edge*>& control_edge = control_dep->control_dep(cur_bb);
	  if(!control_pred_map.is_bound(control_edge))
	    {
	      El_punt("El_If_Converter::if_convert: Problem with control predicate map.");
	    }
	  else
	    {
	      bb_pred_map.bind(cur_bb, control_pred_map.value(control_edge));
	    }
	}
    }
  
}
 
void El_create_hb(Hyperblock* new_hyperblock,Dlist<Basicblock*>& bb_order,Basicblock*& lst_bb,Basicblock* entry_bb,
		  Map< Edge*,Hash_set<Operand> >& edge_pred_map)
{
  Basicblock* last_bb=lst_bb;
  Hash_set<Operand> defined_predicates(hash_operand, 128);
  Dlist_iterator<Basicblock*> bb_iter ;


  for (bb_iter(bb_order); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      
      El_move_bb(cur_bb, new_hyperblock);
      if(cur_bb != entry_bb)
	{
	  El_connect_region_fallthrough(last_bb, cur_bb);
	}
      
      El_update_all_bb_defined_predicates(cur_bb, defined_predicates);
      
      /* Fix Branch and Edges */

      int edges = 0;
      Hash_set<Operand> pred_fall_through(hash_operand, 16);
      Hash_set<Operand> pred_taken(hash_operand, 16);
      bool exit_fall_through = false;
      bool exit_taken = false;
      pred_fall_through.clear();
      pred_taken.clear();
      Region_exit_edges exit_iter ;
      for(exit_iter(cur_bb); exit_iter!=0; exit_iter++)
	{
	  if(edges >= 2)
	    {
	      El_punt("El_If_Converter::if_convert: can't handle more than 2 edges.");
	    }
	  Edge* cur_edge = *exit_iter;

	  if(bb_order.is_member((Basicblock*)cur_edge->dest()->parent()) &&
	     cur_edge->dest()->parent() != entry_bb)
	    {
	      if(edge_pred_map.is_bound(cur_edge))
		{
		  if(cur_edge->src_alt() == 0)
		    {
		      pred_fall_through = edge_pred_map.value(cur_edge);
		    }
		  else
		    {
		      pred_taken = edge_pred_map.value(cur_edge);
		    }
		}
	    }
	  else
	    {
	      if(cur_edge->src_alt() == 0)
		{
		  exit_fall_through = true;
		}
	      else
		{
		  exit_taken = true;
		}
	    }

	  edges++;
	}
  
      if(edges == 1)
	{
	  if(!exit_fall_through && !exit_taken)
	    {
	      /*
	      ** Remove jump and replace it with a dummy branch
	      */
	      Op* branch = El_get_branch_op(cur_bb);
	      Edge* bad_edge = NULL;
	      for(Region_all_outedges ei(cur_bb); ei != 0; ei++)
		{
		  Edge* cur_edge = *ei;
		  bad_edge = cur_edge;
		}
	      branch->remove_outedge_recursively(bad_edge);
	      Op* switch_op = new Op(DUMMY_BR) ;
	      El_replace_op(cur_bb, branch, switch_op);
	      delete branch;
	    }
	}
      else
	{
	  if(exit_fall_through)
	    {
	      if(exit_taken)
		{
		  /*
		  ** In last block of hyperblock.  Do nothing now.
		  */
		}
	      else
		{
		  /*
		  ** Must reverse the branch condition and
		  ** create pred define on pred_taken
		  */
		  El_reverse_bb_branch(cur_bb);
		    		    
		  El_insert_predicate_defines(cur_bb, false, pred_taken,
					      defined_predicates, entry_bb);
		}
	    }
	  else
	    {
	      if(exit_taken)
		{
		  /*
		  ** Just need to create pred define on pred_fall_through
		  */
		  El_insert_predicate_defines(cur_bb, false, pred_fall_through,
					      defined_predicates, entry_bb);
		}
	      else
		{
		  /*
		  ** Need to create pred define on both pred_fall_through and pred_taken
		  */
		  El_insert_predicate_defines(cur_bb, false, pred_fall_through,
					      defined_predicates, entry_bb);
		  El_insert_predicate_defines(cur_bb, true, pred_taken,
					      defined_predicates, entry_bb);

		  /*
		  ** Remove Branch and replace it with a dummy branch
		  */
		  Op* branch = El_get_conditional_branch_op(cur_bb);
		    
		  Edge* bad_edge = NULL;
		  for(exit_iter(cur_bb); exit_iter != 0; exit_iter++)
		    {
		      Edge* cur_edge = *exit_iter;
		      if(cur_edge->src_alt() != 0)
			{
			  bad_edge = cur_edge;
			}
		    }
		  branch->remove_outedge_recursively(bad_edge);
		  Op* switch_op = new Op(DUMMY_BR) ;
		  El_replace_op(cur_bb, branch, switch_op);
		  delete branch;
		    
		}
	    }
	}
      last_bb = cur_bb;
    }

  lst_bb=last_bb;
  return;
}

//Removes the true branch and clears the cond-specific fields of the
//cond_brdir
void el_replace_cond_brdir_with_uncond(Op* branch)
{

  Basicblock* cur_bb=(Basicblock*)branch->parent();
  assert(cur_bb->is_bb());

  Edge* bad_edge = NULL;
  for(Region_exit_edges exit_iter(cur_bb); exit_iter != 0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->src_alt() != 0)
	{
	  bad_edge = cur_edge;
	}
    }
  branch->remove_outedge_recursively(bad_edge);
  Op* switch_op = new Op(BRDIR,3,0,true);
  switch_op->set_id(branch->id());
  switch_op->set_src(PRED1,branch->src(PRED1));
  el_set_attrs_in_br_info(switch_op,
			  get_lcode_attributes(branch),
			  BR_INFO_BR);
  
  El_replace_op(cur_bb, branch, switch_op);
  delete branch;		
  return;
}

void El_connect_normalized_bbs_pred_false(Basicblock* src_bb,Basicblock* dest_bb)
{
  
  Op* src_op=get_last_region_op_from_subregions(src_bb);
  
  //We don't know yet how to assign pred_false alt numbers to brind outedges
  assert(!is_brind(src_op));
  Op* dest_op=get_first_region_op_from_subregions(dest_bb);
  
  El_remove_all_inedges(dest_bb);

  Edge* edge = C0_connect(src_op,dest_op);
  edge->set_src_alt((Alt_num)2);

  src_op->add_outop_recursively(edge);
  dest_op->add_inop_recursively(edge);
  src_op->add_outedge_recursively_restricted(edge);
  dest_op->add_inedge_recursively_restricted(edge);
  
  Control_flow_freq *cfreq = new Control_flow_freq();
  //cdbg << "REMINDER: No control-flow frequency yet for fall-through edge\n";
  cfreq->freq = 0;
  set_control_flow_freq(edge, cfreq);  
  
  return;
}

void El_connect_non_hb_exit_edge(Basicblock* last_bb,Basicblock* cur_bb,
				 bool exit_true,int final_num_edges)
{
  Op* br_op=El_get_branch_op(last_bb);
  assert(!is_brind(br_op));

  if(final_num_edges==1)
    {
      El_reconnect_region_single_edge(last_bb,cur_bb);
    }
  else if(final_num_edges==2)
    {
      if(is_conditional_branch(br_op))
	{
	  if(exit_true)
	    El_reconnect_region_fallthrough(last_bb,cur_bb);
	  else
	    El_reconnect_region_taken(last_bb,cur_bb);
	}
      else
	{
	  assert(is_unconditional_branch(br_op));
	  El_connect_normalized_bbs_pred_false(last_bb,cur_bb);
	}
    }
  else
    {
      assert(final_num_edges==3);
      El_connect_normalized_bbs_pred_false(last_bb,cur_bb);
    }
  return;
}

void El_create_hb_normalized(Hyperblock* new_hyperblock,Dlist<Basicblock*>& bb_order,Basicblock*& lst_bb,
			     Basicblock* entry_bb,Map< Edge*,Hash_set<Operand> >& edge_pred_map)
{
  Basicblock* last_bb=lst_bb;
  Hash_set<Operand> defined_predicates(hash_operand, 128);
  Dlist_iterator<Basicblock*> bb_iter ;
  bool exit_false = false;
  bool exit_true = false;
  int final_num_edges = 0;
  Hash_set<Operand> pred_false(hash_operand, 16);
  Hash_set<Operand> pred_true(hash_operand, 16);
      
  for (bb_iter(bb_order); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      int edges = 0;
      pred_false.clear();
      pred_true.clear();
      Region_exit_edges exit_iter ;
      
      El_move_bb(cur_bb, new_hyperblock);
      if(cur_bb != entry_bb)
	{
	  //Attach the previous bb in the hb to the current bb
	  //If final_num_edges==1, the single edge is used for attachment
	  //If final_num_edges==2, 
	  //                       for a conditional brdir,
	  //                          if exit_true, the false edge is used for attachment
	  //                          else the true edge is used
	  //                       for an ucond brdir, attach with a new pred_false fall-through edge
	  //If final_num_edges==3, (happens only for cond brdirs)
	  //                       attach with the pred-false edge
	  El_connect_non_hb_exit_edge(last_bb, cur_bb, exit_true, final_num_edges);
	}
      
      exit_false = false;
      exit_true = false;

      El_update_all_bb_defined_predicates(cur_bb, defined_predicates);
      
      /* Fix Branch and Edges */
      for(exit_iter(cur_bb); exit_iter!=0; exit_iter++)
	{
	  if(edges >= 2)
	    {
	      El_punt("El_If_Converter::if_convert: can't handle more than 2 edges.");
	    }
	  Edge* cur_edge = *exit_iter;

	  if(bb_order.is_member((Basicblock*)cur_edge->dest()->parent()) &&
	     cur_edge->dest()->parent() != entry_bb)
	    {
	      if(edge_pred_map.is_bound(cur_edge))
		{
		  if(cur_edge->src_alt() == 0)
		    {
		      //is this a copy? MP
		      pred_false = edge_pred_map.value(cur_edge);
		    }
		  else
		    {
		      pred_true = edge_pred_map.value(cur_edge);
		    }
		}
	    }
	  else
	    {
	      if(cur_edge->src_alt() == 0)
		{
		  exit_false = true;
		}
	      else
		{
		  exit_true = true;
		}
	    }

	  edges++;
	}
      
      //Can't deal with register jumps right now
      assert(!is_brind(El_get_branch_op(cur_bb)));
      
      if(edges == 1)
	{
	  //
	  if(exit_true||exit_false)
	    final_num_edges=2;//need to add a pred_false branch
	  else
	    final_num_edges=1;
	}
      else
	{
	  assert(edges==2);

	  if(exit_false&&exit_true)
	    {
	      //no predicate needs to be inserted, since all
	      //edges exit the hb

	      //need to add a pred_false branch, though, so that
	      //the fall-through stays within the hb
	      final_num_edges=3;
	    }
	  else if(exit_true)
	    {
	      //The true branch exits the hb; the predicate that guards the
	      //brdir should therefore be false, for control for the next bb
	      //in the hb to be executed
	      El_insert_predicate_defines_normalized(cur_bb, false, pred_false,
						     defined_predicates, entry_bb);
	      final_num_edges=2;
	    }
	  else if(exit_false)
	    {
	      //The false branch exits the hb; the predicate that guards the
	      //brdir should therefore be true, for control for the next bb
	      //in the hb to be executed
	      El_insert_predicate_defines_normalized(cur_bb, true, pred_true,
							 defined_predicates, entry_bb);
	      final_num_edges=2;
	    }	    
	  else
	    {
	      /*
	      ** Need to create pred define on both pred_false and pred_true
	      */
	      El_insert_predicate_defines_normalized(cur_bb, false, pred_false,
							 defined_predicates, entry_bb);
	      El_insert_predicate_defines_normalized(cur_bb, true, pred_true,
						     defined_predicates, entry_bb);
	      
	      /*
	      ** Replace the conditional brdir with an unconditional brdir
	      */
	      Op* branch = El_get_conditional_branch_op(cur_bb);
	      assert(is_brdir(branch));
	      el_replace_cond_brdir_with_uncond(branch);
	      final_num_edges=1;
	    }
	  
	}
      last_bb = cur_bb;
    }
  
  lst_bb=last_bb;
  return;
}

bool el_check_if_normalized(Hash_set<Compound_region*>& blocks)
{
  bool found_normalized=false;
  bool found_denormalized=false;
  
  for(Hash_set_iterator<Compound_region*> cr_iter(blocks);cr_iter!=0;cr_iter++)
    {
      Basicblock* bb=(Basicblock*)*cr_iter;
      assert(bb->is_bb());
      int count=0;
      for(Region_exit_ops e_op_iter(bb);e_op_iter!=0;e_op_iter++,count++)
	{
	  Op* branch=*e_op_iter;
	  if(is_normalized_branch(branch)) 
	    found_normalized=true;
	  else if(is_brct(branch)||
		  is_bru(branch)||
		  is_brcf(branch))
	    found_denormalized=true;
	}	
    }
  assert(!(found_normalized&&found_denormalized));
  return (found_normalized ? true:false);
}

void El_fix_hb_bottom(Hyperblock* new_hyperblock,Basicblock* last_bb,bool fully_resolved_predicates,
		  Map< Basicblock*, Operand > bb_pred_map, Dlist<Basicblock*> bb_order, List_set<Basicblock*> bb_blocks)
{
  Op* next_op = El_next_op(new_hyperblock);
  Edge* old_fall_through_edge = NULL;
  Edge* new_fall_through_edge = NULL;
  new_hyperblock->reset_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU);
  
  for(Region_all_outedges exit_iter(last_bb); exit_iter != 0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->src_alt() == 0)
	{
	  old_fall_through_edge = cur_edge;
	}
      if(cur_edge->dest() == next_op)
	{
	  new_fall_through_edge = cur_edge;
	}
    }
  if(!new_fall_through_edge)
    {
      new_hyperblock->set_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU);
    }
  if(old_fall_through_edge && new_fall_through_edge != old_fall_through_edge)
    {
      /* Something needs fixing. */
      if(!new_fall_through_edge)
	{
	  /* Create new bb with jump in it */
	  Op* target_op = old_fall_through_edge->dest();
	  Control_flow_freq* cfreq = get_control_flow_freq(old_fall_through_edge);
	  Basicblock* new_jump_bb = El_create_jump_bb(new_hyperblock, cfreq->freq, target_op);
	  
	  Op* first_op = get_first_region_op_from_subregions(new_jump_bb);
	  old_fall_through_edge->set_dest(first_op, CONTROL0, 0);
	  first_op->add_inedge_recursively(old_fall_through_edge);

	  if(fully_resolved_predicates && bb_pred_map.is_bound(last_bb))
	    {
	      bb_pred_map.bind(new_jump_bb, bb_pred_map.value(last_bb));
	    }
	  bb_order.push_tail(new_jump_bb);
	  bb_blocks += new_jump_bb; 

	  Op* last_op = get_last_region_op_from_subregions(new_jump_bb);
	  cout << *last_op;
	  last_op = get_last_region_op_from_subregions(new_hyperblock);
	  cout << *last_op;
	  cout << *new_hyperblock;
	}
      else
	{
	  if(new_fall_through_edge->src_alt() != 0)
	    {
	      /* switch destinations */
	      Op* temp_dest = old_fall_through_edge->dest();
	      old_fall_through_edge->set_dest(new_fall_through_edge->dest());
	      new_fall_through_edge->set_dest(temp_dest);
	      
	      /* reverse branch condition */
	      Op* branch = El_find_branch_op(last_bb);
	      Op* cond = El_get_cond_for_branch(branch);
	
	      Op* new_op = new Op(get_opposite_cond(cond->opcode()));
	      new_op->set_src(SRC1, cond->src(SRC1));
	      new_op->set_src(SRC2, cond->src(SRC2));
	      new_op->set_dest(DEST1, cond->dest(DEST1));
	      new_op->set_src(PRED1, cond->src(PRED1));
	      El_replace_op(last_bb, cond, new_op);
	      delete cond;
	    }
	}
    }
}


void El_fix_hb_bottom_normalized(Hyperblock* new_hyperblock,Basicblock* last_bb)
{
  new_hyperblock->reset_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU);
}


Hyperblock* El_If_Converter::if_convert(Hash_set<Compound_region*> blocks,
				 Compound_region* entry_block,
				 bool fully_resolved_predicates)
{
  List_set<Basicblock*> bb_blocks;
  Dlist<Basicblock*> bb_order;
  Map< Edge*, Hash_set<Operand> > edge_pred_map;
  Map< Basicblock*, Operand > bb_pred_map;
  Dlist_iterator<Basicblock*> bb_iter ;
  Hash_set_iterator<Basicblock*> bb_iter_hs ;
  Basicblock* entry_bb = (Basicblock*) get_first_region_op_from_subregions(entry_block)->parent();

  //Check if the blocks are in normalized form
  bool is_normalized= el_check_if_normalized(blocks);

  /*
  ** Create bb list, remove surrounding HBs
  */
  el_create_bb_list_from_cr_list(blocks, bb_blocks);


  /*
  ** Perform control dependence analysis
  */  
  control_dep->constrict_region(bb_blocks, entry_bb, fully_resolved_predicates);
  control_dep->solve();
  //cout << *control_dep;
  

  /*
  ** Assign predicates to all unique control dependence sets, edges and bbs
  */
  el_assign_preds(bb_blocks,edge_pred_map,control_dep,bb_pred_map);
  

  /*
  ** Determine order of block placement in hyperblock
  */
  el_topo_sort_bbs(bb_order,bb_blocks,entry_bb);


  /*
  ** Create Hyperblock
  */  
  Hyperblock* new_hyperblock = new Hyperblock;
  new_hyperblock->weight = entry_bb->weight;
  entry_bb->parent()->insert_before_region(new_hyperblock, entry_bb);
  new_hyperblock->set_parent(entry_bb->parent());
  delete new_hyperblock->attributes;
  new_hyperblock->attributes = new Graph_attribute(*(entry_bb->attributes)) ;

  Basicblock* last_bb;
  if(is_normalized)
    El_create_hb_normalized(new_hyperblock,bb_order,last_bb,entry_bb,edge_pred_map);
  else
    El_create_hb(new_hyperblock,bb_order,last_bb,entry_bb,edge_pred_map);


  /*
  ** Fix the bottom of the last block.
  ** Insert a new bb with a jump in it or make the hyperblock fall through properly.
  */
  if(!is_normalized)
    El_fix_hb_bottom(new_hyperblock,last_bb,fully_resolved_predicates,bb_pred_map,
		     bb_order,bb_blocks);
  else
    El_fix_hb_bottom_normalized(new_hyperblock,last_bb);


  /*
  ** Fix all hyperblock in/out edges and ops
  */
  for(Region_entry_ops op_iter(entry_bb); op_iter != 0; op_iter++)
    {
      new_hyperblock->add_entry(*op_iter);
      for(Region_all_inedges edge_iter(entry_bb); edge_iter != 0; edge_iter++)
	{
	  (*op_iter)->add_inedge_recursively(*edge_iter);
	}
    }
  for(bb_iter(bb_order); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      
      for(Region_all_outedges edge_iter(cur_bb); edge_iter != 0; edge_iter++)
	{
	  Edge* cur_edge = *edge_iter;
	  if(!bb_order.is_member((Basicblock*)cur_edge->dest()->parent()) ||
	     cur_edge->dest()->parent() == entry_bb)
	    {
	      Op* cur_op = cur_edge->src();
	      new_hyperblock->add_exit(cur_op);
	      cur_op->add_outedge_recursively(cur_edge);
	    }
	}
    }

  // Op* tlast_op = get_last_region_op_from_subregions(new_hyperblock);
  //cdbg << "Last region is:\n" << *tlast_op;


  /*
  ** Predicate the hyperblock BBs
  */
  Hash_set<Operand> defined_predicates(hash_operand, 128);    
  defined_predicates.clear();
  bool is_predicated=false;
  for (bb_iter(bb_order); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      if(bb_pred_map.is_bound(cur_bb))
	{
	  is_predicated=true;
	  Operand pred = bb_pred_map.value(cur_bb);
	  El_predicate_basic_block(cur_bb, pred, defined_predicates);
	}
      else
	{
	  El_update_all_bb_defined_predicates(cur_bb, defined_predicates);
	}
    }

  control_dep->restore_region();
  if(is_predicated)
    new_hyperblock->set_flag(EL_REGION_HYPERBLOCK);

  if(fully_resolved_predicates)
    {
      new_hyperblock->set_flag(EL_REGION_HYPERBLOCK_FRP);
    }
  else
    {
      new_hyperblock->reset_flag(EL_REGION_HYPERBLOCK_FRP);
    }
  
  //cdbg << "HHHHHHHHHHHHHHHHH new hyperblock is HHHHHHHHHHHHHHH\n" << *new_hyperblock;
  return new_hyperblock;
}

// end of el_if_converter.cpp


