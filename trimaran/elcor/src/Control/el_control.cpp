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
//      File:           el_control.cpp
//      Author:         David I. August, Matthai Philipose
//      Created:        May 1995
//      Description:    Control Tools
//
/////////////////////////////////////////////////////////////////////////////

#include "iterators.h"
#include "opcode_properties.h"
#include "hash_set.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "edge_utilities.h"
#include "connect.h"
#include "op.h"
#include "edge.h"
#include "attributes.h"
#include "edge_attributes.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "el_normalize_branches.h"
#include "dbg.h"


Op* El_replace_op(Compound_region* reg, Op* old_op, Op* new_op)
{
  new_op->set_parent(reg);
  
  reg->replace_op_recursively(old_op, new_op);
  
  Hash_set<Edge*> inedges(hash_edge_ptr, 16);
  for(Region_all_inedges inedge_iter(old_op); inedge_iter != 0; inedge_iter++)
    {
      Edge* edge = *inedge_iter;
      inedges += edge;
    }
  Hash_set_iterator<Edge*> edge_iter ;
  for(edge_iter(inedges); edge_iter != 0; edge_iter++)
    {
      Edge* edge = *edge_iter;
      old_op->remove_inedge(edge, edge->src_port_type(), 
			    edge->src_port(), edge->src_alt());
      new_op->add_inedge(edge, edge->src_port_type(), 
			 edge->src_port(), edge->src_alt());
      edge->set_dest(new_op, edge->dest_port(), edge->dest_alt());
    }
  
  Hash_set<Edge*> outedges(hash_edge_ptr, 16);
  for(Region_all_outedges outedge_iter(old_op); outedge_iter != 0; outedge_iter++)
    {
      Edge* edge = *outedge_iter;
      outedges += edge;
    }
  for(edge_iter(outedges); edge_iter != 0; edge_iter++)
    {
      Edge* edge = *edge_iter;
      old_op->remove_outedge(edge, edge->dest_port_type(), 
			    edge->dest_port(), edge->dest_alt());
      new_op->add_outedge(edge, edge->dest_port_type(), 
			 edge->dest_port(), edge->dest_alt());
      edge->set_src(new_op, edge->src_port(), edge->src_alt());
    }

  return new_op ;
}

void El_insert_op_before_switch(Compound_region* reg, Op* op)
{
  Op* switch_branch = get_last_region_op_from_subregions(reg);
  Op* src_op = get_incoming_CONTROL0_edge(switch_branch)->src() ;
  
  C0_connect_insert_between(src_op, switch_branch, op);
  reg->insert_before_region(op, switch_branch);
  
  op->set_parent(reg);
}

// SAM 10-97, handle case where before is the first op of the region
// Note this is generally dangerous, assume caller knows what is going on
void El_insert_op_before(Op* op, Op* before)
{
  Edge *c0_edge;
  Op* src_op;
  Compound_region* reg = before->parent();

  c0_edge = get_incoming_CONTROL0_edge(before);
  if (c0_edge != NULL) {
    src_op = c0_edge->src();
    C0_connect_insert_between(src_op, before, op);
  }
  else {
    C0_connect_fallthrough(op, before);
  }

  reg->insert_before_region(op, before);
  op->set_parent(reg);
}


void El_insert_op_after_merge(Compound_region* reg, Op* op)
{
  Op* merge = get_first_region_op_from_subregions(reg);
  Op* dest_op = get_outgoing_CONTROL0_fall_through_edge(merge)->dest() ;
  
  C0_connect_insert_between(merge, dest_op, op);
  reg->insert_after_region(op, merge);
  
  op->set_parent(reg);
}

// SAM 10-97, handle case where before is the first op of the region
// Note this is generally dangerous, assume caller knows what is going on
void El_insert_op_after(Op* op, Op* after)
{
  Edge *c0_edge;
  Op *dest_op;
  Compound_region* reg = after->parent();
  
  c0_edge = get_outgoing_CONTROL0_fall_through_edge(after);
  if (c0_edge != NULL) {
    dest_op = get_outgoing_CONTROL0_fall_through_edge(after)->dest() ;
    C0_connect_insert_between(after, dest_op, op);
  }
  else {
    C0_connect_fallthrough(after, op);
  }
  
  reg->insert_after_region(op, after);
  op->set_parent(reg);
}


Op* El_find_branch_op(Basicblock* reg)
{
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = (Op*) *op_iter;
      if(is_branch(cur_op))
	{
	  return cur_op;
	}
    }
  return NULL;
}

Op* El_get_branch_op(Basicblock* reg)
{
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = (Op*) *op_iter;
      if(is_branch(cur_op))
	{
	  return cur_op;
	}
    }
  El_punt("El_get_branch_op: no proper branch found.");
  return NULL;
}

Op* El_find_conditional_branch_op(Basicblock* reg)
{
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = (Op*) *op_iter;
      if(is_conditional_branch(cur_op))
	{
	  return cur_op;
	}
    }
  return NULL;
}

Op* El_get_conditional_branch_op(Basicblock* reg)
{
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = (Op*) *op_iter;
      if(is_conditional_branch(cur_op))
	{
	  return cur_op;
	}
    }
  El_punt("El_get_conditional_branch_op: no conditional branch found.");
  return NULL;
}

Op* El_find_traditional_branch_op(Basicblock* reg)
{
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = (Op*) *op_iter;
      if(is_traditional_branch(cur_op))
	{
	  return cur_op;
	}
    }
  return NULL;
}

Op* El_get_traditional_branch_op(Basicblock* reg)
{
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = (Op*) *op_iter;
      if(is_traditional_branch(cur_op))
	{
	  return cur_op;
	}
    }
  El_punt("El_get_traditional_branch_op: no traditional branch found.");
  return NULL;
}
  
bool El_region_has_jsr(Compound_region* reg)
{
  for (Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if(is_call(cur_op))
	{
	  return true;
	}
    }
  return false;
}

/*========================================================================*/
/*
 *	Routines to find pbr/cmpp for a branch
 */
/*========================================================================*/

/*
 *	Simple routine, only does backward search of parent region
 */
Op* El_find_cond_for_branch(Op* branch)
{
  Compound_region* reg = branch->parent();
  if(reg->parent()->is_hb())
    {
      //reg = reg->parent();
      /* RMR { only point the region to the parent if this region 
       * is already formed (i.e., not undergoing if-conversion) 
       */
      Region_exit_ops e_op_iter(reg);
      if (e_op_iter != 0) {
        // null
      }
      else {
        reg = reg->parent();
      }
      /* } RMR */
    }

  bool found_branch = false;

  assert(reg->is_bb() || reg->is_hb());
  
  if(!is_conditional_branch(branch))
    {
      return NULL;
    }

  for (Region_ops_reverse_C0_order op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if(!is_pseudo(cur_op)) {

	// the previous code is wrong 1. because custom ops can create
	// the predicate and 2. because DEST2, 3, etc. of cur_op could
	// also be the branch guard. -ntclark 3/9/04

	// BAD!
	//  if(found_branch && is_cmpp(cur_op) &&
	//     (cur_op->dest(DEST1) == branch->src(SRC2)))
	//    return cur_op;

	if(found_branch) {
	  for(Op_explicit_dests oed(cur_op); oed != 0; oed++) {
	    if((*oed) == branch->src(SRC2))
	      return cur_op;
	  }
	}

	if(cur_op == branch)
	  {
	    found_branch = true;
	  }
      }
    }
  return NULL;
}

Op* El_get_cond_for_branch(Op* branch)
{
  Op* cond;
  if((cond = El_find_cond_for_branch(branch)))
    {
      return cond;
    }
  El_punt("El_get_cond_for_branch: Currently only searches local region");
  return NULL;
}

/*
 *	Simple routine, only does backward search of parent region
 */
Op* El_find_pbr_for_branch(Op* branch)
{
  Compound_region* reg = branch->parent();
  if(reg->parent() && reg->parent()->is_hb())
    {
      reg = reg->parent();
    }
  bool found_branch = false;

  assert(is_proper_branch(branch) || is_brl(branch));

  for (Region_ops_reverse_C0_order op_iter(reg); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;
      if(found_branch && is_pbr(cur_op) &&
	(cur_op->dest(DEST1) == branch->src(SRC1)))
	{
	  return cur_op;
	}
      if(cur_op == branch)
	{
	  found_branch = true;
	}
    }
  El_punt("El_find_pbr_for_branch: Currently only searches local region");
  return NULL;
}

//Given a branch, find the cmpp that feeds into it
//
Op*  El_new_get_cmpp_for_branch(Op* br,Reaching_defs_info* rdi)
{
  //Iterate through the defs of the src ops in the branch,
  //looking for the pbr Op
  int cmpp_count=0;
  Op* cmpp_op = NULL;
  Operand *pred_oprnd;

  if (is_brct(br) || is_brcf(br))
    pred_oprnd = &(br->src(SRC2));
  else
    pred_oprnd = &(br->src(PRED1));

  if (rdi->is_interesting(*pred_oprnd)) {
    El_ref new_use (br,pred_oprnd,EXP_SRC);
    List<El_ref>* defs = rdi->get_ud_chain(new_use);

    for(List_iterator<El_ref> defs_li(*defs);defs_li!=0;defs_li++)
      {
        El_ref def=*defs_li;
        Op* def_op=def.get_op();
        if(is_cmpp(def_op))
          {
            cmpp_count++;
            cmpp_op=def_op;
          }
      }
  }

  //We expect a unique cmpp corresponding to the branch
  assert(cmpp_count==1);
  return cmpp_op;

}

// This code was changed to allow us to find PBRs with intervening MOVEs.
// Changed to allow multiple PBRs reaching one branch. -KF 12/2004
void El_find_pbrs_recursively(El_ref &new_use, Reaching_defs_info *rdi, List<Op*>& pbr_list)
{
   Operand src = new_use.get_operand();

   List<El_ref>* defs = rdi->get_ud_chain(new_use);

   for (List_iterator<El_ref> defs_li(*defs); defs_li != 0; defs_li++) {
      El_ref def = *defs_li;
      Op* def_op = def.get_op();
      if(is_pbr(def_op) || is_custom(def_op)) {
         pbr_list.add_tail(def_op);
      }
      else if (is_move(def_op)) {
	 Operand* src = &(def_op->src(SRC1));
	 if (rdi->is_interesting(*src)) {
	    El_ref new_src(def_op, src, EXP_SRC);
	    El_find_pbrs_recursively(new_src, rdi, pbr_list);
	 }
	 else
	    El_punt("El_find_pbrs_recursively: unknown source for op %d.", def_op->id());
      }
      else
         assert(is_define(def_op) && src.is_macro_reg() && src.name() == RETURN_ADDR);
   }
}    

//Given a branch, find the pbr that feeds into it
// Changed to allow multiple PBRs reaching one branch. -KF 12/2004
void El_new_find_pbrs_for_branch(Op *br, Reaching_defs_info* rdi, List<Op*>& pbr_list)
{
  //Iterate through the defs of the src ops in the branch,
  //looking for the pbr Op
  Operand *btr_oprnd;

  btr_oprnd = &(br->src(SRC1));
  El_ref new_use (br,btr_oprnd,EXP_SRC);
  El_find_pbrs_recursively(new_use, rdi, pbr_list);
}

// Changed to allow multiple PBRs reaching one branch. -KF 12/2004
void El_fix_pbr_dests_with_edges(Compound_region* region,
				 Reaching_defs_info* rdi)
{
  for(Region_all_ops op_iter(region); op_iter != 0; op_iter++)
    {
      Op* cur_op = (Op*) *op_iter;
      for (Region_exit_edges exit_iter(cur_op); exit_iter != 0; exit_iter++)
	{
	  Edge* edge = *exit_iter;

	  if (edge->src_alt() != 0)
	    {
	      Compound_region* dest_reg = edge->dest()->parent();
	      Op* branch_op = edge->src();
	      if(!(branch_op->flag(EL_OPER_TABLE_JUMP) ||
		   is_brdv(branch_op)))  // TLJ 8-96 - temp hack while we are
					 // not generating compensation code.
		 {
                   List<Op*> pbr_list;
                   El_new_find_pbrs_for_branch(branch_op, rdi, pbr_list);
                   for (List_iterator<Op*> pbri(pbr_list); pbri!=0; pbri++) {
                     Op* pbr_op = *pbri;
		     if(is_custom(pbr_op)) {
		       // FIX:
		       cdbg << "ERROR: there's a custom op creating a branch "
			    << "target, and I'm not sure how to handle it!"
			    << endl;
		       assert(0);
		     } else {
		       pbr_op->src(SRC1).set_id(dest_reg->id());

		       Branch_target_attr* bta = get_branch_target_attr(branch_op);
		       if(bta) {
			 char bbnum_str[80];
			 int bbnum = dest_reg->id();
			 sprintf(bbnum_str, "%d", bbnum);
 			 Region *parent = pbr_op->parent();

			 while (!parent->is_procedure())
			   parent = parent->parent();
			 string proc_name = ((Procedure*)parent)->get_name();

			 string label = (proc_name + "_bb_" + bbnum_str);
			 bta->label = label; 
		       }
		     }
                   }
		 }
	    }
	}
    }
}

// Changed to allow multiple PBRs reaching one branch. -KF 12/2004
void El_record_pbr_ids(Compound_region  *region, Reaching_defs_info* rdi)
{
    Op *br_op;
    List<Operand> tmp_list;
    Base_operand *base_oper;
    Lcode_attribute_map *lc_attr;

    for (Region_all_ops op_iter(region); op_iter!=0; op_iter++) {
	br_op = *op_iter;
	if (! requires_pbr(br_op))
	    continue;
        List<Op*> pbr_list;
	El_new_find_pbrs_for_branch(br_op, rdi, pbr_list);
	lc_attr = get_lcode_attributes(br_op);
	if (lc_attr->is_bound("pbr_id")) {
	    lc_attr->unbind("pbr_id");
	}
        if (pbr_list.is_empty()) return;
	tmp_list.clear();
        for (List_iterator<Op*> pbr_iter(pbr_list); pbr_iter!=0; pbr_iter++) {
	   base_oper = new Int_lit((*pbr_iter)->id());
	   Operand tmp_oper(base_oper);
           tmp_list.add_tail(tmp_oper);
        }
	lc_attr->bind("pbr_id", tmp_list);
    }
}

/*
 *	1. Fix pbr values with the appropariate edge target
 *	2. record the pbr id associated w/ each branch as a branch attr 
 */
void El_fix_all_pbr_info(Compound_region *region, bool analysis_info_avail)
{
    Reaching_defs_info *rdi;

    /* This may be called outside the scope of common_process_function(),
      so local analysis info may not exist */
    if (analysis_info_avail == false) {
        create_local_analysis_info_for_all_hbs_bbs(region);
    }
    El_do_reaching_defs(region, ANALYZE_BTR|ANALYZE_PR);
    rdi = get_reaching_defs_info((Region*)region);

    El_fix_pbr_dests_with_edges(region, rdi);
    El_record_pbr_ids(region, rdi);

    remove_reaching_defs_info(region);
    if (analysis_info_avail == false)
        delete_local_analysis_info_for_all_hbs_bbs(region);
}

/*========================================================================*/
/*
 *
 */
/*========================================================================*/

//reconnect edge to have src new instead of old
void el_reconnect_edge_src(Edge* edge,Basicblock* new_src_bb)
{
  Op* old_src_op=edge->src();
  Op* new_src_op=el_get_bb_exit_op(new_src_bb);
  
  //disconnect edge from op
  old_src_op->remove_outop_recursively(edge);
  old_src_op->remove_outedge_recursively(edge);
  
  //reconnect edge
  edge->set_src(new_src_op, edge->src_port(), edge->dest_alt());

  //connect edge to op
  new_src_op->add_outop_recursively(edge);
  new_src_op->add_outedge_recursively_restricted(edge);
  
  return;
}


//reconnect edge to have dest new instead of old
void el_reconnect_edge_dest(Edge* edge,Basicblock* new_succ_bb)
{
  
  
  Op* new_dest_op=el_get_bb_entry_op(new_succ_bb);
  Op* old_dest_op=edge->dest();


  //disconnect edge from old dest op
  new_dest_op->add_inedge_recursively_restricted(edge);
  old_dest_op->remove_inedge_recursively(edge);
  

  //redirect the edge
  edge->set_dest(new_dest_op, edge->dest_port(), edge->dest_alt());
  
  
  //connect edge into new dest op
  new_dest_op->add_inop_recursively(edge);
  old_dest_op->remove_inop_recursively(edge);  
  
  return;
}

void El_add_jump_to_region(Compound_region* region, Op* dest_op)
{
  Edge *edge;
  Op* pbr_op;
  Op* bru_op;
  Base_operand* regist;
  Op* last_op;
  Control_flow_freq *cfreq;
  
  for (Region_exit_edges exit_iter(region); exit_iter!=0; exit_iter++)
    {
      El_punt("Add_Jump_To_Region: Region already has an edge.");
    }

  last_op = get_last_region_op_from_subregions(region);
  

  // create pbr op
  pbr_op = new Op(PBR);
  region->add_region(pbr_op);
  pbr_op->set_parent(region);

  regist = new Reg(EL_STANDARD_BRANCH, BTR, virtual_register_number);
  Operand pbr_dest1(regist);
  pbr_op->set_dest(DEST1, pbr_dest1);

  Operand pbr_src1(new Cb_operand(dest_op->parent()->id()));
  pbr_op->set_src(SRC1, pbr_src1);

  Operand pbr_src2(new Int_lit(1));
  pbr_op->set_src(SRC2, pbr_src2);

  Operand pbr_pred(new Pred_lit(true));
  pbr_op->set_src(PRED1, pbr_pred);

  // create bru op 
  bru_op = new Op(BRU);
  region->add_region(bru_op);
  bru_op->set_parent(region);
  region->add_exit(bru_op);
  
  Operand bru_src1(new Reg(EL_STANDARD_BRANCH, BTR, ((Reg*)regist)->vr_num()));
  bru_op->set_src(SRC1, bru_src1);

  Operand bru_pred(new Pred_lit(true));
  bru_op->set_src(PRED1, bru_pred);
  
  C0_connect_fallthrough(last_op, pbr_op);
  C0_connect_fallthrough(pbr_op, bru_op);
  edge = C0_connect(bru_op, dest_op);
  cfreq = new Control_flow_freq();
  cfreq->freq = region->weight;
  set_control_flow_freq(edge, cfreq);
  
  bru_op->add_outedge_recursively(edge);
  dest_op->add_inedge_recursively(edge);
      
}


//same as above, except that both pbr and bru
//are predicated on the "pred" argument
void El_add_predicated_jump_to_region(Compound_region* region, Op* dest_op, Operand& pred)
{
  Edge *edge;
  Op* pbr_op;
  Op* bru_op;
  Base_operand* regist;
  Op* last_op;
  Control_flow_freq *cfreq;
  
  for (Region_exit_edges exit_iter(region); exit_iter!=0; exit_iter++)
    {
      El_punt("Add_Jump_To_Region: Region already has an edge.");
    }

  last_op = get_last_region_op_from_subregions(region);
  

  // create pbr op
  pbr_op = new Op(PBR);
  region->add_region(pbr_op);
  pbr_op->set_parent(region);

  regist = new Reg(EL_STANDARD_BRANCH, BTR, virtual_register_number);
  Operand pbr_dest1(regist);
  pbr_op->set_dest(DEST1, pbr_dest1);

  Operand pbr_src1(new Cb_operand(dest_op->parent()->id()));
  pbr_op->set_src(SRC1, pbr_src1);

  Operand pbr_src2(new Int_lit(1));
  pbr_op->set_src(SRC2, pbr_src2);

  pbr_op->set_src(PRED1, pred);
  

  // create bru op
  bru_op = new Op(BRU);
  region->add_region(bru_op);
  bru_op->set_parent(region);
  region->add_exit(bru_op);
  
  Operand bru_src1(new Reg(EL_STANDARD_BRANCH, BTR, ((Reg*)regist)->vr_num()));
  bru_op->set_src(SRC1, bru_src1);

  bru_op->set_src(PRED1, pred);
  
  C0_connect_fallthrough(last_op, pbr_op);
  C0_connect_fallthrough(pbr_op, bru_op);
  edge = C0_connect(bru_op, dest_op);
  edge->set_src_alt(1);
  cfreq = new Control_flow_freq();
  cfreq->freq = region->weight;
  set_control_flow_freq(edge, cfreq);

  bru_op->add_outop_recursively(edge);
  bru_op->add_outedge_recursively_restricted(edge);
  dest_op->add_inedge_recursively_restricted(edge);
  
  return;
}

void El_remove_region(Compound_region* region, bool delete_flag)
{
  
  Compound_region* parent = region->parent();

  /*
  ** Find immediate subregions of the region
  */
  List_set<Compound_region*> imm_subregions;
  for(Region_all_ops op_iter(region); op_iter != 0; op_iter++)
    {
      Compound_region* parent = (*op_iter)->parent();
      while(parent->parent() != region)
	{
	  parent = parent->parent();
	}
      imm_subregions += parent;
    }
  
  /*
  ** Set parent child/relationship between subregions of region and parent
  */
  for(List_set_iterator<Compound_region*> reg_iter(imm_subregions);
      reg_iter != 0; reg_iter++)
    {
      Compound_region* reg = *reg_iter;
      reg->set_parent(parent);
      region->remove_region(reg);
      parent->insert_before_region(reg, region);
    }

  parent->remove_region(region);

  if (delete_flag)
    delete region;
}

void El_remove_all_inedges(Region* region)
{
  Hash_set<Edge*> remove_edges(hash_edge_ptr, 16);
  Op* first_op = get_first_region_op_from_subregions(region);

  for (Region_all_inedges entry_iter(region); entry_iter != 0; entry_iter++)
    {
      remove_edges += *entry_iter;
    }
  for(Hash_set_iterator<Edge*> edge_iter(remove_edges); edge_iter != 0; edge_iter++)
    {
      Edge* edge = (*edge_iter);
      first_op->remove_inedge_recursively(edge);
    }
}

void El_delete_all_inedges(Region* region)
{
  Hash_set<Edge*> delete_edges(hash_edge_ptr, 16);
  Op* first_op = get_first_region_op_from_subregions(region);

  for (Region_all_inedges entry_iter(region); entry_iter != 0; entry_iter++)
    {
      delete_edges += *entry_iter;
    }
  for(Hash_set_iterator<Edge*> edge_iter(delete_edges); edge_iter != 0; edge_iter++)
    {
      Edge* edge = (*edge_iter);
      first_op->remove_inedge_recursively(edge);
      delete edge;
    }
}

/*
** Makes no assumptions about edges.  Previous region fall through edges are
** removed.
*/

void El_connect_region_fallthrough(Compound_region* prev_reg, Compound_region* next_reg)
{
  Op* last_op = get_last_region_op_from_subregions(prev_reg);
  
  Op* first_op = get_first_region_op_from_subregions(next_reg);
  
  Edge* bad_edge = NULL;
  for(Region_all_outedges exit_iter(prev_reg); exit_iter != 0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->src_alt() == 0)
	{
	  bad_edge = cur_edge;
	}
    }
  last_op->remove_outedge_recursively(bad_edge);
 
  El_remove_all_inedges(next_reg);

  Edge* edge = C0_connect_fallthrough(last_op, first_op) ;
  last_op->add_outedge_recursively_restricted(edge);
  first_op->add_inedge_recursively_restricted(edge);
  Control_flow_freq *cfreq = new Control_flow_freq();
  cfreq->freq = prev_reg->weight;
  set_control_flow_freq(edge, cfreq);
}


/*
** Same as above, except it is more careful about when
** an op/edge is added (recursively) as an out op/edge
*/

void El_reconnect_region_fallthrough(Compound_region* prev_reg, Compound_region* next_reg)
{
  Op* last_op = get_last_region_op_from_subregions(prev_reg);
  
  Op* first_op = get_first_region_op_from_subregions(next_reg);
  
  Edge* bad_edge = NULL;
  for(Region_all_outedges exit_iter(prev_reg); exit_iter != 0; exit_iter++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->src_alt() == 0)
	{
	  bad_edge = cur_edge;
	}
    }

  Control_flow_freq *old_cfreq = get_control_flow_freq(bad_edge);    
  last_op->remove_outedge_recursively(bad_edge);
 
  El_remove_all_inedges(next_reg);

  Edge* edge = C0_connect_fallthrough(last_op, first_op) ;
  last_op->add_outop_recursively(edge);
  first_op->add_inop_recursively(edge);
  last_op->add_outedge_recursively_restricted(edge);
  first_op->add_inedge_recursively_restricted(edge);

  Control_flow_freq *cfreq = new Control_flow_freq();
  cfreq->freq = old_cfreq->freq;

  set_control_flow_freq(edge, cfreq);
}



/*
** Makes no assumptions about edges; works for brct/bru ops only (i.e.
** no multiple(i.e. >2)-outedge branches.  Previous taken edge is removed
*/

void El_reconnect_region_taken(Compound_region* prev_reg, Compound_region* next_reg)
{
  Op* last_op = get_last_region_op_from_subregions(prev_reg);
  
  Op* first_op = get_first_region_op_from_subregions(next_reg);
  
  Edge* bad_edge = NULL;
  int count=0;
  for(Region_all_outedges exit_iter(prev_reg); exit_iter != 0; exit_iter++,count++)
    {
      Edge* cur_edge = *exit_iter;
      if(cur_edge->src_alt() != 0)
	{
	  bad_edge = cur_edge;
	}
    }
  assert(is_brdir(last_op)||count==2);
  last_op->remove_outedge_recursively(bad_edge);
  
  El_remove_all_inedges(next_reg);

  Edge* edge = C0_connect(last_op, first_op) ;
  last_op->add_outedge_recursively(edge);
  first_op->add_inedge_recursively(edge);

  Control_flow_freq *cfreq = new Control_flow_freq();
 
  set_control_flow_freq(edge, cfreq);
  return;
}


void El_reconnect_region_single_edge(Compound_region* first,Compound_region* second)
{
  Edge* single_edge = NULL;
  int count=0;

  for(Region_exit_edges e_iter(first);e_iter!=0;e_iter++,count++)
    {
      single_edge=*e_iter;
    }
  assert(count==1);
  if(single_edge->src_alt() == 0)
    El_reconnect_region_fallthrough(first,second);
  else
    El_reconnect_region_taken(first,second);
}

/*
** This function moves the bb from its original position
** and places it at the end of dest_reg.
*/
void El_move_bb(Basicblock* bb, Compound_region* dest_reg)
{
  Compound_region* old_parent = bb->parent();
  old_parent->remove_region(bb);
  bb->set_parent(dest_reg);
  dest_reg->add_region(bb);

  /*
  ** Fixing the in/out ops and edges for the old parent is
  ** VERY hard.  It shouldn't happen.
  */
  // if(!old_parent->is_procedure())
  //     {
  //       El_punt("!!!!! El_move_bb: old parent not procedure !!!!!") ;
  //     }
    
}

Op* El_next_op(Region* reg)
{
  Op* last_op = get_last_region_op_from_subregions(reg); 
  Compound_region* parent = reg->parent();
  while(parent->parent())
    {
      parent = parent->parent();
    }

  bool next = false;
  for(Region_all_ops reg_iter(parent); reg_iter != 0; reg_iter++)
    {
      Op* cur_op = (*reg_iter);
      if(next)
	{
	  return cur_op;
	}
      if(cur_op == last_op)
	{
	  next = true;
	}
    }
  El_punt("El_next_region: IR currupted.");
  return NULL;
}



Basicblock *El_create_jump_bb(Compound_region *parent, double weight, Op *target_op)
{
    Basicblock *new_BB;
    Op *merge_op;

    new_BB = new Basicblock;
    new_BB->weight = weight;
    new_BB->set_parent(parent);

    merge_op = new Op(C_MERGE) ;
    new_BB->add_region(merge_op) ;
    merge_op->set_parent(new_BB) ;
    new_BB->add_entry(merge_op);

    parent->add_region(new_BB);	

    El_add_jump_to_region(new_BB, target_op);

    return (new_BB);
}

void El_redirect_fallthrough_path(Compound_region *reg, Edge *ft_edge, Op *target_op)
{
    Compound_region *parent, *jump_r;
    Op *first_op, *pbr_op, *src_op, *prev_op;
    Control_flow_freq *cfreq;
    Reg *regist;
    Base_operand *base_oper;
    Opcode new_opcode;
    Edge *edge;

    parent = reg->parent();
    src_op = ft_edge->src();

    /* Only 1 br in BB, so need to carefully handle */
    if (reg->is_bb()) {

	/* Change DUMMY_BR to jump */
        if (src_op->opcode()==DUMMY_BR) {

	    /* Create PBR */
	    regist = new Reg(EL_STANDARD_BRANCH, BTR, virtual_register_number);
            Operand pbr_dest1(regist);

            pbr_op = new Op(PBR);
            reg->insert_before_region(pbr_op, src_op);
	    edge = get_incoming_CONTROL0_edge(src_op);
	    prev_op = edge->src();
	    C0_connect_insert_between(prev_op, src_op, pbr_op);
            pbr_op->set_parent(reg);

            pbr_op->set_dest(DEST1, pbr_dest1);
            base_oper = new Cb_operand(target_op->parent()->id());
            Operand pbr_src1(base_oper);
            pbr_op->set_src(SRC1, pbr_src1);
            base_oper = new Int_lit(1);
            Operand pbr_src2(base_oper);
            pbr_op->set_src(SRC2, pbr_src2);
            base_oper = new Pred_lit(true);
            Operand pbr_pred1(base_oper);
            pbr_op->set_src(PRED1, pbr_pred1);

	    new_opcode = BRU;
	    src_op->set_opcode(new_opcode);
            base_oper = new Reg(*regist);
            Operand src1(base_oper);
	    src_op->update_num_operands();
	    src_op->set_src(SRC1, src1);

	    ft_edge->set_dest(target_op, CONTROL0, get_new_out_alt_num(target_op));
	    target_op->add_inedge_recursively(ft_edge);
        }

	/* Create a new BB subsequent to reg to hold jump to target_op */
        else {
	    cfreq = get_control_flow_freq(ft_edge);
            jump_r = El_create_jump_bb(parent, cfreq->freq, target_op);
            parent->insert_after_region(jump_r, reg);
            first_op = get_first_region_op_from_subregions(jump_r);
            ft_edge->set_dest(first_op, CONTROL0, 0);
            first_op->add_inedge_recursively(ft_edge);
        }
    }

    /* Non-BB compound region */
}


void El_eliminate_dead_dummy_branches(Compound_region* R)
{
   Region_all_ops reg_opi ;
   Op_pred pred_opi ;
   List<Op*> dead_op_list ;
   List_iterator<Op*> lopi ;

   for (reg_opi(R) ; reg_opi != 0 ; reg_opi++) {
      Op* cur_op = *reg_opi ;
      if (is_control_merge(cur_op)) {
	 int pred_count = 0 ;
	 bool dummy_branch_pred = false ;
	 Op* db_op = NULL ;
	 for (pred_opi(cur_op) ; pred_opi != 0 ; pred_opi++) {
	    Op* pred_op = *pred_opi ;
	    pred_count++ ;
	    if (is_dummy_branch(pred_op)) {
	       dummy_branch_pred = true ;
	       db_op = pred_op ;
	    }
	 }
	 if ((pred_count == 1) && (dummy_branch_pred == true) &&
	     (cur_op->parent() == db_op->parent()) ) {
	    dead_op_list.add_tail(db_op) ;
	    dead_op_list.add_tail(cur_op) ;	    
	 }
      }
   }
   for (lopi(dead_op_list) ; lopi != 0 ; lopi++) {
      Op* cur_op = *lopi ;
      C0_remove_op(cur_op) ;
      cur_op->parent()->remove_region(cur_op) ;
      delete cur_op ;
   }
}


 
int el_num_region_predecessors(Region* reg)
{
  int count=0;
  for(Region_entry_edges e_iter(reg);e_iter!=0;e_iter++,count++)
    ;
  return count;
}

 
bool is_region_exit_edge(Compound_region* reg,Edge* edge)
{
  
  for(Region_exit_edges e_iter(reg);e_iter!=0;e_iter++)
    {
      Edge* new_edge=*e_iter;
      if(edge==new_edge)
	return true;
    }

  return false;
}

