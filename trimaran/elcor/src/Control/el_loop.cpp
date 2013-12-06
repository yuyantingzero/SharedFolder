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
//      File:           el_loop.cpp
//      Author:         David I. August
//      Created:        May 1995
//      Description:    Loop Class Definition
//
/////////////////////////////////////////////////////////////////////////////

#include "op.h"
#include "opcode_cmpp.h"
#include "adjlist_graph.h"
#include "control_analysis_solver.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "el_induction_var.h"
#include "el_loop.h"
#include "el_loop_iterators.h"
#include "el_loop_filters.h"
#include "el_loop_tools.h"
#include "iterators.h"
#include "attributes.h"
#include "connect.h"
#include "edge_attributes.h"
#include "opcode_properties.h"
#include "list_ofp.h"
#include "edge.h"
#include "hash_functions.h"
#include "el_tail_duplication.h"
#include "graph_filters.h"
#include "el_lsched_init.h"
#include "ir_writer.h"

#undef DEBUG


////////////////////////////////////////////////////////////
//  Class El_Loop
////////////////////////////////////////////////////////////

El_Loop::El_Loop() :
  back_edge_blocks(hash_bb_ptr, 16),
  exit_blocks(hash_bb_ptr, 16),
  postloop_blocks(hash_bb_ptr, 16),
  basic_ind_var(hash_operand, 16),
  basic_ind_var_ops(hash_op_ptr, 16),
  primary_ind_var_ops(hash_op_ptr, 16)
{
  id = -1;
  nesting_level = -1;
  category = UNCLASSIFIED;
  swp_status = UNKNOWN;
  preloop_block = NULL;
  header_block = NULL;
  remainder_block = NULL;
  preremainder_block = NULL;
  num_invocation = 0;
  num_iteration = 0;
  loopcount = -1;
  basic_ind_var_init_vals = new Hash_map<Operand,Operand>(hash_operand, 128);
  basic_ind_var_init_ops = new Hash_map<Operand,Op*>(hash_operand, 128);
}

El_Loop::El_Loop(int new_id, Basicblock* new_head) :
  back_edge_blocks(hash_bb_ptr, 16),
  exit_blocks(hash_bb_ptr, 16),
  postloop_blocks(hash_bb_ptr, 16),
  basic_ind_var(hash_operand, 16),
  basic_ind_var_ops(hash_op_ptr, 16),
  primary_ind_var_ops(hash_op_ptr, 16)
{
  id = new_id;
  header_block = new_head;

  nesting_level = -1;
  category = UNCLASSIFIED;
  swp_status = UNKNOWN;
  preloop_block = NULL;
  remainder_block = NULL;
  preremainder_block = NULL;
  num_invocation = 0;
  num_iteration = 0;
  loopcount = -1;
  basic_ind_var_init_vals = new Hash_map<Operand,Operand>(hash_operand, 128);
  basic_ind_var_init_ops = new Hash_map<Operand,Op*>(hash_operand, 128);

  // Make the reference to the original C source code easily accessible,
  // when available.
  Lcode_attribute_map* lam =
    get_lcode_attributes(get_enclosing_analysis_scope(header_block));
  if (lam != NULL && lam->is_bound("LOOP") && lam->is_bound("FILE")) {

    assert(lam->value("FILE")[0].is_string());
    assert(lam->value("LOOP")[1].is_int());

    source_filename = lam->value("FILE")[0].string_value();
    source_line_num = lam->value("LOOP")[1].int_value();
  }
}

El_Loop::~El_Loop()
{
  delete basic_ind_var_init_vals;
  delete basic_ind_var_init_ops;
}


// Loop print function 
ostream& operator<<(ostream& os, const El_Loop& loop)
{
  loop.print(os);
  return os;
}


void El_Loop::print(ostream& os) const
{

  os << "LOOP:" << endl;
  os << "      ID: " << id << endl;
  os << "      Header: " << header_block->id() << endl;
  os << "      Invocations: " << num_invocation << endl;
  os << "      Iterations: " << num_iteration << endl;
  
  os << "      Category: ";
  switch(category)
    {
    case UNCLASSIFIED:
      cout << "UNCLASSIFIED" << endl;
      break;
    case MARKED_FOR_DELETION:
      cout << "MARKED_FOR_DELETION" << endl;
      break;
    case DO_LOOP:
      cout << "DO_LOOP" << endl;
      break;
    case WHILE_LOOP:
      cout << "WHILE_LOOP" << endl;
      break;
    default:
      El_punt("El_Loop::print: unknown category.");
    }

  os << "      SWP Status: ";
  switch(swp_status)
    {
    case UNKNOWN:
      cout << "UNKNOWN" << endl;
      break;
    case NOT_SWP_ABLE:
      cout << "NOT_SWP_ABLE" << endl;
      break;
    case SWP_ABLE:
      cout << "SWP_ABLE" << endl;
      break;
    default:
      El_punt("El_Loop::print: unknown swp status.");
    }
  
  os << "      Nest Level: " << nesting_level << endl;
  if(preloop_block)
    {
      os << "      Preloop Block: " << preloop_block->id() << endl;
    }
  else
    {
      os << "      Preloop Block: NONE" << endl;
    }

  os << "      Loop Blocks: ";
  Hash_set_iterator<Basicblock*> bb_iter ;
  for(List_set_iterator<Basicblock*> bb_iter2(loop_blocks); bb_iter2 != 0; bb_iter2++)
    {
      os << (*bb_iter2)->id() << " ";
    }
  os << endl;
  os << "      Back Edge Blocks: ";
  for(bb_iter(back_edge_blocks); bb_iter != 0; bb_iter++)
    {
      os << (*bb_iter)->id() << " ";
    }
  os << endl;
  os << "      Exit Blocks: ";
  for(bb_iter(exit_blocks); bb_iter != 0; bb_iter++)
    {
      os << (*bb_iter)->id() << " ";
    }
  os << endl;
  os << "      Postloop Blocks: ";
  for(bb_iter(postloop_blocks); bb_iter != 0; bb_iter++)
    {
      os << (*bb_iter)->id() << " ";
    }
  os << endl;

  os << "      Basic Induction Variables: " << endl;
  for(Hash_set_iterator<Operand> oper_iter(basic_ind_var); oper_iter != 0; oper_iter++)
    {
      os << "            " << (*oper_iter) << endl;
    }
  os << endl;
  
  os << "      Basic Induction Variable Ops: ";
  Hash_set_iterator<Op*> op_iter ;
  for(op_iter(basic_ind_var_ops); op_iter != 0; op_iter++)
    {
      os << (*op_iter)->id() << " ";
    }
  os << endl << endl;
  
  os << "      Primary Induction Variable:" << primary_ind_var << endl;
  
  os << "      Primary Induction Variable Ops: ";
  for(op_iter(primary_ind_var_ops); op_iter != 0; op_iter++)
    {
      os << (*op_iter)->id() << " ";
    }
  os << endl << endl;
  os << "      Source Filename: " << source_filename << endl;
  os << "      Source Line Number: " << source_line_num << endl;
  os << endl;
  
}

////////////////////////////////////////////////////////////
//  Class El_Loop_Graph
////////////////////////////////////////////////////////////


void El_Loop_Graph::Find_Blocks_In_Loop(El_Loop* loop)
{
  Hash_set<Basicblock*> temp_loop_blocks(hash_bb_ptr, 64);
  
  if(!loop)
    {
      El_punt("El_Loop_Graph::Find_Blocks_In_Loop: Passed NULL pointer.");
    }

  for(Hash_set_iterator<Basicblock*> back_iter(loop->back_edge_blocks); back_iter != 0; back_iter++)
    {
      int start = control->graph->f_map.value(*back_iter);
      temp_loop_blocks += loop->back_edge_blocks.head();
      int stop = control->graph->f_map.value(loop->header_block);
      temp_loop_blocks += loop->header_block;

      for(Alist_graph_bfs node_iter(*(control->graph), UP, start, stop); node_iter != 0; node_iter++)
	{
	  temp_loop_blocks += (Basicblock*) control->graph->b_map[*node_iter];
#ifdef DEBUG
	  cout << "Adding Block to Loop " << ((Basicblock*) control->graph->b_map[*node_iter])->id()
	       << endl;
#endif
	}
    }

  loop->loop_blocks.clear();
  /*
  **  Go through all the ops in layout order to make the loop_blocks list be in layout order
  */
  /* WARNING: Seems dangerous
   * The List_set interface doesn't allow us to control the order of items, 
   * and doesn't explicitly guarantee the order items will be returned in. 
   *
   * This will be broken for preheader/postheader creation -- by default they
   * are added to the end of the List_set.
   *
   * Hopefully this was here for debugging purposes, and not because other
   * code relies on it.                         <lieberm 06/29/2006>
   */
  for(List_set_iterator<Basicblock*> bb_iter(bb_set) ; bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter ;
      
      if (temp_loop_blocks.is_member(cur_bb))
	{
	  loop->loop_blocks += cur_bb;
	}
    }
   
}

void El_Loop_Graph::Find_Exit_Blocks_In_Loop(El_Loop* loop)
{
  if(!loop)
    {
      El_punt("El_Loop_Graph::Find_Exit_Blocks_In_Loop: Passed NULL pointer.");
    }
  loop->exit_blocks.clear();
  loop->postloop_blocks.clear();

  if(loop->loop_blocks.is_empty())
    {
      El_punt("El_Loop_Graph::Find_Exit_Blocks_In_Loop: Empty Loop.");
    }
  
  for(List_set_iterator<Basicblock*> bb_iter(loop->loop_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
#ifdef DEBUG2
      cout << "Loop Block " << cur_bb->id() << endl;
#endif
      for (Region_exit_edges exit_iter(cur_bb); exit_iter!=0; exit_iter++)
	{
#ifdef DEBUG2
	  cout << "Dest Block " << (*exit_iter)->dest()->parent()->id() << endl;
#endif
	  Basicblock* dest_bb = (Basicblock*) (*exit_iter)->dest()->parent();
	  
	  if (!loop->loop_blocks.is_member(dest_bb))
	    {
	      loop->exit_blocks += cur_bb;
	      loop->postloop_blocks += dest_bb;
	    }
	}
    }
}

void El_Loop_Graph::Delete_Marked_Loops()
{
  Listofp<El_Loop*> loop_list;
    
  for(Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      loop_list.add_tail((*loop_iter).second);
    }
  
  for(Listofp_iterator<El_Loop*> loop_iter1(loop_list); loop_iter1 != 0; loop_iter1++)
    {
      El_Loop* loop1 = *loop_iter1;
      
      // Delete Marked Loops
      if(loop1->category == MARKED_FOR_DELETION)
	{
	  id.unbind(loop1->id);
	  child.unbind(loop1);
	  parent.unbind(loop1);
	  sibling.unbind(loop1);
	  delete loop1;
	}
    }
  
}


void El_Loop_Graph::Merge_Loops()
{
  Listofp<El_Loop*> loop_list;
  
  for(Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      loop_list.add_tail((*loop_iter).second);
    }
  
  for(Listofp_iterator<El_Loop*> loop_iter1(loop_list); loop_iter1 != 0; loop_iter1++)
    {
      Listofp_iterator<El_Loop*> loop_iter2;
      for((loop_iter2 = loop_iter1), loop_iter2++; loop_iter2 != 0; loop_iter2++)
	{
	  El_Loop* loop1 = *loop_iter1;
	  El_Loop* loop2 = *loop_iter2;
	  
	  // Merge Loops if their headers are equal
	  if(loop1->category == MARKED_FOR_DELETION ||
	     loop2->category == MARKED_FOR_DELETION)
	    {
	      continue;
	    }
	  if(loop1->header_block == loop2->header_block)
	    {
	      // Both loop get merged into Loop 1
	      loop1->loop_blocks += loop2->loop_blocks;
	      loop1->back_edge_blocks += loop2->back_edge_blocks;
	      loop2->category = MARKED_FOR_DELETION;
	    } 
	}
    }
  Delete_Marked_Loops();
}

void El_Loop_Graph::Find_Num_Invocation(El_Loop* loop)
{
  if(!loop)
    {
      El_punt("El_Loop_Graph::Find_Num_Invocation: Passed NULL pointer.");
    }
  loop->num_iteration = loop->header_block->weight;
  loop->num_invocation = loop->header_block->weight;
  for (Region_entry_edges entry_iter(loop->header_block); entry_iter!=0; entry_iter++)
    {
      Basicblock* src_bb = (Basicblock*) (*entry_iter)->src()->parent();
      
      if (loop->loop_blocks.is_member(src_bb))
	{
	  loop->num_invocation -= get_control_flow_freq(*entry_iter)->freq;
	}
    }
  
}


void El_Loop_Graph::Loop_Detection()
{
  Basicblock* cur_bb;
  Basicblock* dest_bb;

  for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
    {
      cur_bb = *bb_iter;

      for (Region_exit_edges exit_iter(cur_bb); exit_iter!=0; exit_iter++)
	{
	  dest_bb = (Basicblock*) (*exit_iter)->dest()->parent();
	  
	  if (control->is_dom(dest_bb, cur_bb))
	    {
#ifdef DEBUG
	      cout << "FOUND A LOOP " << cur_bb->id() << " : " << dest_bb->id() << endl;
#endif
	      El_Loop* new_loop = new El_Loop(cur_id, dest_bb);
	      id.bind(cur_id, new_loop);
	      new_loop->back_edge_blocks += cur_bb;


	      Find_Blocks_In_Loop(new_loop);
	      Find_Exit_Blocks_In_Loop(new_loop);
	      cur_id++;
	    }
	}
    }
  
  Merge_Loops();

  for(Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      El_Loop* loop = (*loop_iter).second;
	  
      Find_Exit_Blocks_In_Loop(loop);
      Find_Num_Invocation(loop);
    }
}

Basicblock* El_Loop_Graph::Create_Loop_Preheader(El_Loop* loop)
{
  Basicblock* preheader = new Basicblock;
  Op* merge_op;
  Op* switch_op;
  Op* header_op;
  Op* last_op;
  Compound_region* insert_region;
  Edge* edge;
  Compound_region* header_region;
  bool fall_through;
  Control_flow_freq *cfreq;
  
  preheader->weight = loop->num_invocation;
  preheader->set_parent(region);
  preheader->set_flag(EL_REGION_PRELOOP_BLOCK);

  merge_op = new Op(C_MERGE) ;
  preheader->add_region(merge_op) ;
  merge_op->set_parent(preheader) ;
  preheader->add_entry(merge_op);
  // for systolic multi-loop (i.e., if systolic loop's entry C_MERGE
  // op has nametag of type SYSTOLIC_BODY), create nametag attribute
  // for preloop region's entry C_MERGE op to be <loopname, SYSTOLIC_INIT> 
  // because this region will be part of the init code.
  Nametag_attr* ntag = get_nametag_attr(loop->header_block);
  if ((ntag) && (ntag->type == SYSTOLIC_BODY)) {
    Nametag_attr* new_ntag = (Nametag_attr*)(ntag->clone_attribute());
    new_ntag->type = SYSTOLIC_INIT;
    set_nametag_attr(merge_op, new_ntag);
  }

  fall_through = true;
  Region_entry_edges entry_iter ;
  for (entry_iter(loop->header_block); entry_iter!=0; entry_iter++)
    {
      Basicblock* prev_bb = (Basicblock*) (*entry_iter)->src()->parent();
      Edge* edge = (*entry_iter);

      if(edge->dest_alt() == 0)
	{
	  if(loop->loop_blocks.is_member(prev_bb))
	    {
	      fall_through = false;
	    }
	}
    }

  header_op = get_first_region_op_from_subregions(loop->header_block);
  
  if(fall_through)
    {
      switch_op = new Op(DUMMY_BR) ;
      preheader->add_region(switch_op) ;
      switch_op->set_parent(preheader) ;
      preheader->add_exit(switch_op);
      
      C0_connect_fallthrough(merge_op, switch_op) ;

      header_region = loop->header_block;
      while (header_region->parent() != region)
	{
	  header_region = header_region->parent();
	}
      region->insert_before_region(preheader, header_region);
      
      Region_entry_edges entry_iter ;
      for (entry_iter(loop->header_block); entry_iter!=0; entry_iter++)
	{
	  Basicblock* prev_bb = (Basicblock*) (*entry_iter)->src()->parent();
	  Edge* edge = (*entry_iter);
	  
	  if((!loop->loop_blocks.is_member(prev_bb)) && (preheader != prev_bb))
	    {
	      edge->set_dest(merge_op, edge->dest_port(), edge->dest_alt());
	      merge_op->add_inedge_recursively(edge);
	    }
	}

      edge = C0_connect_fallthrough(switch_op, header_op) ;
      switch_op->add_outedge_recursively(edge);
      header_op->add_inedge_recursively(edge);
      cfreq = new Control_flow_freq();
      cfreq->freq = preheader->weight;
      set_control_flow_freq(edge, cfreq);
      
      for (entry_iter(preheader); entry_iter!=0; entry_iter++)
	{
	  Edge* edge = (*entry_iter);
	  header_op->remove_inedge_recursively(edge);
	}
    }
  else
    {
      last_op = get_last_region_op_from_subregions(region);
      insert_region = last_op->parent();
      while (insert_region->parent() != region)
	{
	  insert_region = insert_region->parent();
	}
      region->insert_after_region(preheader, insert_region);

      El_add_jump_to_region(preheader, header_op);
      switch_op = El_get_branch_op(preheader);

      Region_entry_edges entry_iter ;
      for (entry_iter(loop->header_block); entry_iter!=0; entry_iter++)
	{
	  Basicblock* prev_bb = (Basicblock*) (*entry_iter)->src()->parent();
	  Edge* edge = (*entry_iter);
	  
	  if((!loop->loop_blocks.is_member(prev_bb)) && (preheader != prev_bb))
	    {
	      if(edge->dest_alt() == 0)
		{
		   edge->set_dest(merge_op, edge->dest_port(),
				  get_new_in_alt_num(edge->dest()));
		}
	      else
		{
		  edge->set_dest(merge_op, edge->dest_port(), edge->dest_alt());
		}
	      merge_op->add_inedge_recursively(edge);
	    }
	}

#if 0 // hongtaoz: the edge is already created and connected in El_add_jump_to_region. The following code
      // adds redundent edges between same ops and causes problem in CPR.
      edge = C0_connect(switch_op, header_op) ;
      switch_op->add_outedge_recursively(edge);
      header_op->add_inedge_recursively(edge);
      cfreq = new Control_flow_freq();
      cfreq->freq = preheader->weight;
      set_control_flow_freq(edge, cfreq);
#endif

      for (entry_iter(preheader); entry_iter!=0; entry_iter++)
	{
	  Edge* edge = (*entry_iter);

	  header_op->remove_inedge_recursively(edge);
	}
    }
  
  bb_set += preheader;
  return preheader;
}


// This function exists because all the Op copy functions
// (Op::operator=, Op::clone, etc) copy more information than I want
// copied, and it's not easy to undo some of it.
static Op* clone_op (Op* op)
{
  Op* new_op = new Op(op->opcode(), op->num_srcs(), op->num_dests(), op->predicated());
  assert(new_op && "memory is severely lacking");

  for(int i = op->first_src(); i <= op->last_src(); i++) {
    new_op->set_src(i, op->src(i));
  }

  for(int i = op->first_dest(); i <= op->last_dest(); i++) {
    new_op->set_dest(i, op->dest(i));
  }

  if(op->predicated())
    new_op->set_src(PRED1, op->src(PRED1));

  new_op->copy_flags(op);

  Lcode_attribute_map* attr_map = get_lcode_attributes(op);
  if(attr_map != NULL) {
    Lcode_attribute_map* new_attr_map = new Lcode_attribute_map(*attr_map);
    set_lcode_attributes(new_op , new_attr_map);
  }

  return new_op;
}


// In modulo variable expansion, the MSched kernel potentially needs
// to be unrolled in order for lifetimes not to overlap (i.e., so we
// don't need rotating registers). If we unroll the kernel, and the LC
// is not a multiple of the unroll factor, then we need a copy of the
// kernel either before or after to handle the remaining iterations
// that unrolling would miss. E.g., if our loop had 50 iterations, and
// we unroll 3 times, then we'd change the kernel to iterate 16 times
// (16 * 3 = 48) and create a non-modulo-scheduled copy to handle the
// remaining 2 iterations.
//
// This function creates that copy of the kernel; during modulo
// variable expansion, the trip count of the copy is updated based on
// the unrolling factor. If the loop required no unrolling, then the
// kernel copy is deleted.
void El_Loop_Graph::create_remainder_blocks(const Loop_filter* filter)
{
  Map_iterator<int, El_Loop*> loop_iter;
  for(loop_iter(id); loop_iter != 0; loop_iter++) {
    El_Loop* loop = (*loop_iter).second;

    if(!filter_match(filter, loop)) {
      continue;
    }

    // This code assumes that there is only one block comprising the
    // loop. I believe this is a restriction on the present modulo
    // scheduler, and so I'm not writing code more general than it
    // needs to be.
    assert(loop->loop_blocks.size() == 1);

    Basicblock* bb = loop->loop_blocks.head();


    ////////////////////////////////////////////////////////////////////////////
    // Make a new block, and copy all of the ops to it.

    Basicblock* copy_block = new Basicblock();
    assert(copy_block && "out of memory");
    loop->remainder_block = copy_block;

    // Mark the block, so the modulo scheduler knows which block to
    // update during MVE.
    copy_block->set_flag(EL_REGION_REMAINDER_BLOCK);
    copy_block->weight = 0;
    copy_block->set_parent(bb->parent());

    Op* prev_op = NULL;
    Op* merge_op = NULL;
    Op* switch_op = NULL;
    Op* pbr_op = NULL;
    for(Region_ops_C0_order rops(bb); rops != 0; rops++) {
      Op* op = *rops;
      Op* copy_op = clone_op(op);

      if(prev_op == NULL) {
	copy_block->add_region(copy_op);
      } else {
	El_insert_op_after(copy_op, prev_op);
      }

      // Need to change the loop branch to target the copy instead of
      // the actual modulo scheduled loop.
      if(is_pbr(copy_op)) {
	pbr_op = copy_op;
	Operand& target = copy_op->src(SRC1);
	if(target.is_cb()) {
	  target.set_id(copy_block->id());
	}
      }

      if(bb->is_entry_op(op)) {
	copy_block->add_entry(copy_op);
	merge_op = copy_op;
      }
      if(bb->is_exit_op(op)) {
	copy_block->add_exit(copy_op);
	switch_op = copy_op;

	if(pbr_op) {
	  Lcode_attribute_map *lc_attr = get_lcode_attributes(switch_op);
	  if(lc_attr && lc_attr->is_bound("pbr_id")) {
	    (*lc_attr)["pbr_id"].clear();
	    Operand tmp_oper(new Int_lit(pbr_op->id()));
	    (*lc_attr)["pbr_id"].add_tail(tmp_oper);
	  }
	}
      }
      
      prev_op = copy_op;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Create a backedge for the new loop. Using a copy, because it's
    // easier than setting all the fields by hand.

    Region_entry_edges entry_iter(bb);
    Edge* backedge = (*entry_iter)->clone();
    // Alt = 1 means this is the taken edge. 0 means fallthrough.
    backedge->set_src(switch_op, backedge->src_port(), 1);
    backedge->set_dest(merge_op, backedge->dest_port(), 1);
    backedge->set_src_alt(1);
    backedge->set_dest_alt(1);

    copy_block->add_entry_safely(backedge);
    copy_block->add_exit_safely(backedge);
    merge_op->add_inedge_recursively(backedge);
    switch_op->add_outedge_recursively(backedge);

    // Set the frequency to zero, because we have no idea if this will
    // ever iterate. If it does it's likely to be a small number of times.
    Control_flow_freq* cfreq = new Control_flow_freq();
    assert(cfreq && "out of memory");
    cfreq->freq = 0;
    set_control_flow_freq(backedge, cfreq);


    ////////////////////////////////////////////////////////////////////////////
    // Now put that block into the IR before the loop.

    Op* header_op = get_first_region_op_from_subregions(loop->header_block);
  
    Region* header_region = loop->header_block;
    while(header_region->parent() != region) {
      header_region = header_region->parent();
    }
    region->insert_before_region(copy_block, header_region);
      
    for(entry_iter(loop->header_block); entry_iter != 0; entry_iter++) {
      Basicblock* prev_bb = (Basicblock*) (*entry_iter)->src()->parent();
      Edge* edge = (*entry_iter);
      if(( ! loop->loop_blocks.is_member(prev_bb)) && (copy_block != prev_bb)) {
	edge->set_dest(merge_op, edge->dest_port(), edge->dest_alt());
	merge_op->add_inedge_recursively(edge);
	((Compound_region*)header_region)->remove_entry(edge);
	copy_block->add_entry_safely(edge);
      }
    }

    Edge* edge = C0_connect_fallthrough(switch_op, header_op) ;
    switch_op->add_outedge_recursively(edge);
    header_op->add_inedge_recursively(edge);
    cfreq = new Control_flow_freq();
    assert(cfreq && "out of memory");
    cfreq->freq = copy_block->weight;
    set_control_flow_freq(edge, cfreq);
      
    for(entry_iter(copy_block); entry_iter != 0; entry_iter++) {
      Edge* edge = (*entry_iter);
      header_op->remove_inedge_recursively(edge);
    }

  }
}


// This creates a block that sets the loop counter for the remainder
// block constructed in the function above.
void El_Loop_Graph::create_preremainder_blocks(const Loop_filter* filter)
{
  Map_iterator<int, El_Loop*> loop_iter;
  for(loop_iter(id); loop_iter != 0; loop_iter++) {
    El_Loop* loop = (*loop_iter).second;

    if(!filter_match(filter, loop)) {
      continue;
    }


    ////////////////////////////////////////////////////////////////////////////
    // First do some intialization stuff.
    
    Basicblock* preloop = loop->preloop_block;
    Basicblock* remainder = loop->remainder_block;
    Operand rem_lc(new Reg(EL_STANDARD_INT));
    Operand zero(new Int_lit(0));
    Operand one(new Int_lit(1));


    ////////////////////////////////////////////////////////////////////////////
    // Now create the preremainder block and copy ops from the preloop
    // block.

    Basicblock* prerem_block = new Basicblock();
    assert(prerem_block && "out of memory");
    loop->preremainder_block = prerem_block;

    prerem_block->weight = 0;
    prerem_block->set_parent(preloop->parent());

    Op* prev_op = NULL;
    Op* merge_op = NULL;
    Op* switch_op = NULL;
    for(Region_ops_C0_order rops(preloop); rops != 0; rops++) {
      Op* op = *rops;

      Op* copy_op = clone_op(op);

      if(prev_op == NULL) {
	prerem_block->add_region(copy_op);
      } else {
	El_insert_op_after(copy_op, prev_op);
      }

      if(preloop->is_entry_op(op)) {
	prerem_block->add_entry(copy_op);
	merge_op = copy_op;
      }
      if(preloop->is_exit_op(op)) {
	prerem_block->add_exit(copy_op);
	switch_op = copy_op;
      }

      // Change all occurrences of LC to our rem_lc. There's no
      // correctness reason for doing this, but many parts of the
      // compiler don't like random control registers showing up where
      // they're not expected.
      for(Op_explicit_sources regs(copy_op); regs != 0; regs++) {
	Operand& oper = *regs;
	if(oper.is_macro_reg() && oper.name() == LC_REG) {
	  copy_op->set_src(regs.get_port_num(), rem_lc);
	}
      }
      for(Op_explicit_dests regs(copy_op); regs != 0; regs++) {
	Operand& oper = *regs;
	if(oper.is_macro_reg() && oper.name() == LC_REG) {
	  copy_op->set_dest(regs.get_port_num(), rem_lc);
	}
      }

      if(is_int_div(copy_op)) {
	// Need a REM and an absolute value op after the divide. The
	// REM sets up the number of iterations needed after
	// unrolling, and the ABS is needed in case the the operands
	// are negative (i.e., the loop goes from high to low). E.g.,
	// -2 % -3 = -2, but we need +2 iterations.

	// After modulo scheduling, the 1 in the REM operation will by
	// multiplied by the number of times the loop was unrolled.
	Opcode rem_opc;
	if(copy_op->opcode() == DIV_W)
	  rem_opc = REM_W;
	else
	  rem_opc = REML_W;
	Op* rem = new Op(rem_opc);
	assert(rem && "out of memory");
	rem->set_src(PRED1, op->src(PRED1));
	rem->set_src(SRC1, copy_op->dest(DEST1));
	rem->set_src(SRC2, Operand(new Int_lit(1)));
	rem->set_dest(DEST1, copy_op->dest(DEST1));
	El_insert_op_after(rem, copy_op);

	Op* abs = new Op((Opcode) ABS_W);
	assert(abs && "out of memory");
	abs->set_src(PRED1, op->src(PRED1));
	abs->set_src(SRC1, copy_op->dest(DEST1));
	abs->set_dest(DEST1, copy_op->dest(DEST1));
	El_insert_op_after(abs, rem);
	prev_op = abs;
      } else {
	prev_op = copy_op;
      }
    }


    ////////////////////////////////////////////////////////////////////////////
    // Remove the last subtract by one.

    for(Region_ops_reverse_C0_order rops(prerem_block); rops != 0; rops++) {
      Op* op = *rops;

      if(is_int_sub(op) && op->src(SRC2).is_int() && (op->src(SRC2).int_value() == 1)) {
	El_remove_op(op);
	delete op;
	break;
      }
    }


    ////////////////////////////////////////////////////////////////////////////
    // Once the preremainder is setup, stick it in the IR.
    
    Op* header_op = get_first_region_op_from_subregions(remainder);
  
    Region* remainder_region = remainder;
    while(remainder_region->parent() != region) {
      remainder_region = remainder_region->parent();
    }
    region->insert_before_region(prerem_block, remainder_region);
      
    for(Region_entry_edges entry_iter(remainder); entry_iter != 0; entry_iter++) {
      Basicblock* prev_bb = (Basicblock*) (*entry_iter)->src()->parent();
      Edge* edge = (*entry_iter);
      if(remainder != prev_bb) {
	edge->set_dest(merge_op, edge->dest_port(), edge->dest_alt());
	merge_op->add_inedge_recursively(edge);
	((Compound_region*)remainder_region)->remove_entry(edge);
	prerem_block->add_entry_safely(edge);
      }
    }

    Edge* ft_edge = C0_connect_fallthrough(switch_op, header_op) ;
    switch_op->add_outedge_recursively(ft_edge);
    header_op->add_inedge_recursively(ft_edge);
    Control_flow_freq* cfreq = new Control_flow_freq();
    assert(cfreq && "out of memory");
    cfreq->freq = prerem_block->weight;
    set_control_flow_freq(ft_edge, cfreq);
      
    for(Region_entry_edges entry_iter(prerem_block); entry_iter != 0; entry_iter++) {
      Edge* edge = (*entry_iter);
      header_op->remove_inedge_recursively(edge);
    }

    
    ////////////////////////////////////////////////////////////////////////////
    // Adjust CMPP in the remainder block to use the newly computed
    // loop bounds. Also need to insert a subtract operation for the
    // LC, because the remainder loop doesn't have an implicit
    // decrement from the BRF.

    // FIX: this is a bad hack. Assuming that the last cmpp
    // corresponds to the loop branch. Should do reaching defs, and
    // call get_cmpp_for_branch.
    Op* cmpp_op = NULL;
    Op* brct_op = NULL;
    for(Region_ops_reverse_C0_order rops(remainder); rops != 0; rops++) {
      Op* op = *rops;

      if(brct_op == NULL) {
	brct_op = op;
      }

      if(is_cmpp(op)) {
	cmpp_op = op;
	op->set_opcode((Opcode) CMPP_W_GT_UN_UN);
	op->set_src(SRC1, rem_lc);
	op->set_src(SRC2, zero);

	Op* sub = new Op((Opcode)SUB_W);
	sub->set_dest(DEST1, rem_lc);
	sub->set_src(SRC1, rem_lc);
	sub->set_src(SRC2, one);
	sub->set_src(PRED1, cmpp_op->src(PRED1));

	// Place it before the cmpp
	El_insert_op_before(sub, op);

	break;
      }
    }


    ////////////////////////////////////////////////////////////////////////////
    // Find a random PBR op for cloning purposes.

    Op* pbr_op = NULL;
    for(Region_ops_reverse_C0_order rops(remainder); rops != 0; rops++) {
      Op* op = *rops;

      if(is_pbr(op)) {
	pbr_op = op;
	break;
      }
    }


    ////////////////////////////////////////////////////////////////////////////
    // And lastly, add a branch to the preremainder block that skips
    // the remainder block if there are zero remainder iterations.

    // We could create new registers here instead of reusing the
    // operands. This would potentially make code quality better.
    Op* new_cmpp = clone_op(cmpp_op);
    new_cmpp->set_opcode((Opcode) CMPP_W_EQ_UN_UN);
    Op* new_pbr = clone_op(pbr_op);
    Op* new_brct = clone_op(brct_op);
    prerem_block->add_exit(new_brct);

    Operand& target = new_pbr->src(SRC1);
    if(target.is_cb()) {
      target.set_id(preloop->id());
    }

    Lcode_attribute_map *lc_attr = get_lcode_attributes(brct_op);
    if(lc_attr && lc_attr->is_bound("pbr_id")) {
      (*lc_attr)["pbr_id"].clear();
      Operand tmp_oper(new Int_lit(new_pbr->id()));
      (*lc_attr)["pbr_id"].add_tail(tmp_oper);
    }

    El_insert_op_before_switch(prerem_block, new_pbr);
    El_insert_op_before_switch(prerem_block, new_cmpp);
    El_insert_op_before_switch(prerem_block, new_brct);

    Op* dummy_br = get_last_region_op_from_subregions(prerem_block);
    prerem_block->remove_exit(dummy_br);

    // Adjust the fall through edge.
    dummy_br->remove_outedge(ft_edge, ft_edge->src_port_type(),
			     ft_edge->src_port(), ft_edge->src_alt());
    ft_edge->set_src(new_brct, ft_edge->src_port(), ft_edge->src_alt());
    new_brct->add_outedge_recursively(ft_edge);

    // Set the taken edge.
    Op* preloop_header = get_first_region_op_from_subregions(preloop);
    Edge* tk_edge = C0_connect_taken(new_brct, preloop_header);
    new_brct->add_outedge_recursively(tk_edge);
    preloop_header->add_inedge_recursively(tk_edge);

    // FIX: Make the region weights correct.
    cfreq = new Control_flow_freq();
    assert(cfreq && "out of memory");
    cfreq->freq = prerem_block->weight;
    set_control_flow_freq(tk_edge, cfreq);

    Edge* bad_edge = get_incoming_CONTROL0_edge(dummy_br);
    new_brct->remove_outedge(bad_edge, CONTROL, CONTROL0, bad_edge->src_alt());

    prerem_block->remove_region(dummy_br);
    delete dummy_br;

//     cdbg << "Added preremainder block: " << prerem_block->id() << endl;
//     ir_write(prerem_block, "%d_prerem", prerem_block->id());
//     ir_write(preloop, "%d_preloop", prerem_block->id());
  }
}


void El_Loop_Graph::create_preloop_blocks(const Loop_filter* filter)
{
   Map_iterator<int, El_Loop*> loop_iter ;
   for(loop_iter(id); loop_iter != 0; loop_iter++)
      {
	 El_Loop* loop = (*loop_iter).second;

	 if(!filter_match(filter, loop))
	 {
	    continue;
	 }
      
	 for (Region_entry_edges entry_iter(loop->header_block); entry_iter!=0; entry_iter++)
	 {
	    Basicblock* prev_bb = (Basicblock*) (*entry_iter)->src()->parent();
	  
	    if (prev_bb->flag(EL_REGION_PRELOOP_BLOCK))
	    {
	       loop->preloop_block = prev_bb;
	    }
	 }
	 if(loop->preloop_block == NULL)
	 {
	    loop->preloop_block = Create_Loop_Preheader(loop);
	 }
      }
  
   delete control;
   control = new Dominator_CFA_solver(region);
   control->solve();

   for(loop_iter(id); loop_iter != 0; loop_iter++)
   {
      El_Loop* loop = (*loop_iter).second;
      
      Find_Blocks_In_Loop(loop);
      Find_Exit_Blocks_In_Loop(loop);
   }
  
}

Basicblock* El_Loop_Graph::Create_Loop_Postheader(El_Loop* loop, Basicblock* exit_bb, Basicblock* dest_bb)
{
  Basicblock* postheader = new Basicblock;
  Edge* orig_edge = NULL;
  Edge* new_edge;
  Edge* fall_through_edge = NULL;
  Op* merge_op;
  Op* switch_op;
  Op* dest_op;
  Op* last_op;
  Compound_region* insert_region;
  Control_flow_freq *cfreq;
    
  postheader->set_flag(EL_REGION_POSTLOOP_BLOCK);
  
  merge_op = new Op(C_MERGE) ;
  postheader->add_region(merge_op) ;
  merge_op->set_parent(postheader) ;
  postheader->add_entry(merge_op);
  // for systolic multi-loop (i.e., if systolic loop's entry C_MERGE
  // op has nametag of type SYSTOLIC_BODY), create nametag attribute
  // for postloop region's entry C_MERGE op to be <loopname, SYSTOLIC_FINAL> 
  // because this region will be part of the final code.
  Nametag_attr* ntag = get_nametag_attr(loop->header_block);
  if ((ntag) && (ntag->type == SYSTOLIC_BODY)) {
    Nametag_attr* new_ntag = (Nametag_attr*)(ntag->clone_attribute());
    new_ntag->type = SYSTOLIC_FINAL;
    set_nametag_attr(merge_op, new_ntag);
  }
  
  fall_through_edge = NULL;
  for (Region_entry_edges entry_iter(dest_bb); entry_iter!=0; entry_iter++)
    {
      Basicblock* src_bb = (Basicblock*) (*entry_iter)->src()->parent();
      
      if (src_bb == exit_bb)
	{
	  postheader->weight = get_control_flow_freq(*entry_iter)->freq;
	  orig_edge = *entry_iter;
	  
	  if((*entry_iter)->dest_alt() == 0)
	    {
	      fall_through_edge = (*entry_iter);
	    }
	}
    }

  if (fall_through_edge)
    {
      switch_op = new Op(DUMMY_BR) ;
      postheader->add_region(switch_op) ;
      switch_op->set_parent(postheader) ;
      postheader->add_exit(switch_op);
      
      C0_connect_fallthrough(merge_op, switch_op);
      
      Compound_region* dest_bb_region;

      dest_op = get_first_region_op_from_subregions(dest_bb);
      dest_bb_region = dest_bb;
      while (dest_bb_region->parent() != region &&
	     dest_op == get_first_region_op_from_subregions(dest_bb_region->parent()))
	{
	  dest_bb_region = dest_bb_region->parent();
	}

      postheader->set_parent(dest_bb_region->parent());
      dest_bb_region->parent()->insert_before_region(postheader, dest_bb_region);
      
      new_edge = C0_connect_fallthrough(switch_op, dest_op) ;
      switch_op->add_outedge_recursively(new_edge);
      dest_op->add_inedge_recursively(new_edge);
      cfreq = new Control_flow_freq();
      cfreq->freq = postheader->weight;
      set_control_flow_freq(new_edge, cfreq);
      
      orig_edge->set_dest(merge_op, orig_edge->dest_port(), orig_edge->dest_alt());
      merge_op->add_inedge_recursively(orig_edge);
      dest_op->remove_inedge_recursively(orig_edge);
    }
  else
    {
      postheader->set_parent(region);

      dest_op = get_first_region_op_from_subregions(dest_bb);

      last_op = get_last_region_op_from_subregions(region);
      insert_region = last_op->parent();
      while (insert_region->parent() != region)
	{
	  insert_region = insert_region->parent();
	}
      region->insert_after_region(postheader, insert_region);

      El_add_jump_to_region(postheader, dest_op);
      
      orig_edge->set_dest(merge_op, orig_edge->dest_port(), orig_edge->dest_alt());
      merge_op->add_inedge_recursively(orig_edge);
      dest_op->remove_inedge_recursively(orig_edge);
    }
  
  bb_set += postheader;
  return postheader;
}

void El_Loop_Graph::create_postloop_blocks(const Loop_filter* filter)
{
   Map_iterator<int, El_Loop*> loop_iter ;
   for(loop_iter(id); loop_iter != 0; loop_iter++)
   {
      El_Loop* loop = (*loop_iter).second;

      if(!filter_match(filter, loop))
      {
	 continue;
      }
      
      for(Hash_set_iterator<Basicblock*> exit(loop->exit_blocks); exit != 0; exit++)
      {
	 Basicblock* exit_bb = *exit;
	  
	 for (Region_exit_edges exit_iter(exit_bb); exit_iter!=0; exit_iter++)
	 {
	    Basicblock* dest_bb = (Basicblock*) (*exit_iter)->dest()->parent();

	    if (!loop->loop_blocks.is_member(dest_bb))
	    {
	       if (dest_bb->flag(EL_REGION_POSTLOOP_BLOCK))
	       {
#ifdef DEBUG
		  cout << "This is no good it's id is " << dest_bb->id() << endl;
#endif
		  break;
	       }
#ifdef DEBUG
	       cout << "SHOULD CREATE POSTHEADER" << endl;
#endif
	       loop->postloop_blocks += Create_Loop_Postheader(loop, exit_bb, dest_bb);
	    }
	 }
      }
   }
  
  
   delete control;
   control = new Dominator_CFA_solver(region);
   control->solve();
  
   for(loop_iter(id); loop_iter != 0; loop_iter++)
   {
      El_Loop* loop = (*loop_iter).second;

      Find_Blocks_In_Loop(loop);
      Find_Exit_Blocks_In_Loop(loop);
   }
  
}

void El_Loop_Graph::Find_Nesting_Levels()
{
   Map_iterator<int, El_Loop*> loop_iter1 ;
   for(loop_iter1(id); loop_iter1 != 0; loop_iter1++)
   {
      El_Loop* loop1 = (*loop_iter1).second;
      
      loop1->nesting_level = 1;
   }
   for(loop_iter1(id); loop_iter1 != 0; loop_iter1++)
   {
      El_Loop* loop1 = (*loop_iter1).second;
      
      for(Map_iterator<int, El_Loop*> loop_iter2(id); loop_iter2 != 0; loop_iter2++)
      {
	 El_Loop* loop2 = (*loop_iter2).second;

	 if (loop1 == loop2)
	 {
	    continue;
	 }
	  
	 if (El_loop_is_nested(loop1, loop2))
	 {
	    loop2->nesting_level += 1;
	 }
      }
   }
}

void El_Loop_Graph::Connect_Loop_Hierarchy()
{
  for(Map_iterator<int, El_Loop*>loop_iter2(id); loop_iter2 != 0; loop_iter2++)
    {
      El_Loop* loop2 = (*loop_iter2).second;
      if(loop2->nesting_level == 1)
	{
	  continue;
	}
      
      for(Map_iterator<int, El_Loop*> loop_iter1(id); loop_iter1 != 0; loop_iter1++)
	{
	  El_Loop* loop1 = (*loop_iter1).second;

	  if (loop1 == loop2)
	    {
	      continue;
	    }

	  if (loop2->nesting_level == (loop1->nesting_level + 1))
	    {
	      if(El_loop_is_nested(loop1, loop2))
		{
		  parent.bind(loop2, loop1);
		  if(child.is_bound(loop1))
		    {
		      sibling.bind(loop2, child.value(loop1));
		    }
		  child.bind(loop1, loop2);
		  break;
		}
	    }
	}
    }
  
  first_loop = NULL;
  El_Loop* loop2 = NULL;
  for(Map_iterator<int, El_Loop*>loop_iter1(id); loop_iter1 != 0; loop_iter1++)
    {
      El_Loop* loop1 = (*loop_iter1).second;
      if(loop1->nesting_level == 1)
	{
	  if(!first_loop)
	    {
	      first_loop = loop1;
	    }
	  else
	    {
	      sibling.bind(loop2, loop1);
	    }
	  loop2 = loop1;
	}
    }
}

void El_Loop_Graph::Build_Loop_Hierarchy()
{
  Find_Nesting_Levels();
  Connect_Loop_Hierarchy();

  num_loops = 0;
  for(Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      num_loops++;
    }
}


El_Loop_Graph::El_Loop_Graph()
{
}

El_Loop_Graph::El_Loop_Graph(Compound_region *reg)
{
  region = reg;
  cur_id = 0;
  first_loop = NULL;

  if(!El_confirm_BB_tiling(region, bb_set)) { // this not only confirms tiling
                                              // but also fills bb_set
    // Try to form basic blocks, and retry confirming the BB tiling.
    El_form_basic_blocks(region);
    if(!El_confirm_BB_tiling(region, bb_set)) {
      El_punt("El_Loop_Graph::El_Loop_Graph: Not a full BB tiling");
    }
  }
  control = new Dominator_CFA_solver(reg);
  control->solve();
  
  Loop_Detection();
  
  Build_Loop_Hierarchy();
       
}


El_Loop_Graph::~El_Loop_Graph()
{
  delete control;
  for(Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      delete((*loop_iter).second);
    }
  
}

// Loop Graph print function 
ostream& operator<<(ostream& os, const El_Loop_Graph& loop_graph)
{
  loop_graph.print(os);
  return os;
}

void El_Loop_Graph::print(ostream& os) const
{
  os << "LOOP GRAPH:" << endl << endl;

  for(Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      os << *((*loop_iter).second) << endl;
    }
}




////////////////////////////////////////////////////////////
//  El_Loop_Graph: Induction variable functions
////////////////////////////////////////////////////////////

bool El_Loop::is_invariant(Operand* operand)
{
  for(List_set_iterator<Basicblock*> bb_iter(loop_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      if(!El_is_invariant(cur_bb, operand))
	{
	  return false;
	}
    }
  return true;
}


bool El_Loop::is_unique_def(Op* op)
{
  for(List_set_iterator<Basicblock*> bb_iter(loop_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      if(!El_is_unique_def(cur_bb, op))
	{
	  return false;
	}
    }
  return true;
}

void El_Loop::find_induction_variables()
{
  for (List_set_iterator<Basicblock*> bb_iter(loop_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      El_normalize_operands_by_type(cur_bb);

      for (Region_all_ops op_iter(cur_bb); op_iter != 0; op_iter++)
	{
	  Op* cur_op = *op_iter;

	  /*
	  ** BRLC ops are special since they LC = LC + 1 implicitely
	  */
	  if(is_brlc(cur_op))
	    {
	      basic_ind_var += cur_op->dest(DEST1);
	      continue;
	    }
	  if (!El_op_has_induction_pattern(cur_op))
	    {
	      continue;
	    }
	  if (basic_ind_var.is_member(cur_op->dest(DEST1)))
	    {
	      continue;
	    }
	  if(!is_invariant(&cur_op->src(SRC2)))
	    {
	      continue;
	    }
	  if(!is_unique_def(cur_op))
	    {
	      continue;
	    }
	  
	  /*
	   *  Record as basic induction variable
	   */
	  
	  basic_ind_var += cur_op->dest(DEST1);
	}
    }
}

void El_Loop::find_induction_ops()
{
  for (List_set_iterator<Basicblock*> bb_iter(loop_blocks); bb_iter != 0; bb_iter++)
    {
      for (Region_all_ops op_iter(*bb_iter); op_iter != 0; op_iter++)
	{
	  Op* cur_op = *op_iter;

	    if (cur_op->num_dests() < 1 || !cur_op->dest(DEST1).is_reg())
	      {
		continue;
	      }
            if (!basic_ind_var.is_member(cur_op->dest(DEST1)))
	      {
		continue;
	      }
	  	  
	  /*
	   *  Record as basic induction variable op
	   */
	  
	  basic_ind_var_ops += cur_op;
	}
    }
}

/*
 *	SAM 8-2000.  We will handle 1 special case.  The loop header has a single
 *	predecessor block outside the loop, if so, look in here for initializations
 *	of the induction vars.
 */
void El_Loop::find_induction_init_vals()
{
    Operand dest, src, pred;
    Op *op;
    Hash_set<Operand> defs(hash_operand, 16);
    Basicblock *src_bb, *init_bb = NULL;
    Edge *edge;
    int count;

    count = 0;
    for (Region_entry_edges edgei(header_block, CONTROL0_INEDGES); edgei!=0; edgei++) {
	edge = *edgei;
	src_bb = (Basicblock*) edge->src()->parent();
	if (back_edge_blocks.is_member(src_bb))
	    continue;
	init_bb = src_bb;
	count++;
    }

    if (count == 1) {
        for (Region_ops_reverse_C0_order opi(init_bb); opi!=0; opi++) {
	    op = *opi;
	    if (is_move(op)) {
	        dest = op->dest(DEST1);
	        src = op->src(SRC1);
	        pred = op->src(PRED1);
	        if (basic_ind_var.is_member(dest) && !defs.is_member(dest) &&
		    pred.is_predicate_true()) {
		    basic_ind_var_init_vals->bind(dest, src);
		    basic_ind_var_init_ops->bind(dest, op);
	        }
	    }
	    for (Op_all_dests desti(op); desti!=0; desti++) {
	        dest = *desti;
	        defs += dest;
	    }
        }
    }
}


int El_Loop::potential_primary_ind_op_count(Basicblock* bb)
{
  Hash_set<Basicblock*> visited(hash_bb_ptr, 197);
  Hash_set<Basicblock*> to_visit(hash_bb_ptr, 197);
  int count=0;
  
  to_visit += bb;

  while(!to_visit.is_empty())
    {
      Basicblock* cur_bb = to_visit.pop();
      visited += cur_bb;

      if(cur_bb != header_block)
	{
	  for (Region_entry_edges entry_iter(cur_bb); entry_iter != 0; entry_iter++)
	    {
	      Basicblock* src_bb = (Basicblock*)(*entry_iter)->src()->parent();
	      
	      if (loop_blocks.is_member(src_bb) && !visited.is_member(src_bb))
		{
		  to_visit += src_bb;
		}
	    }
	}

      for(Region_all_ops op_iter(cur_bb); op_iter != 0; op_iter++)
	{
	  Op* cur_op = (Op*) *op_iter;

	  if(primary_ind_var_ops.is_member(cur_op))
	    {
	      count++;
	    }
	}
    }
  
  return count;
  
}

void El_Loop::find_primary_ind_var(Dominator_CFA_solver *control)
{
  Op* branch_op;
  Hash_set<Operand> potential_primary_ind_var(hash_operand, 16);
  Hash_set_iterator<Basicblock*> bb_iter;
  Hash_set_iterator<Op*> op_iter;
  int can_encounter_ops;

  if(category != UNCLASSIFIED)
    {
      El_punt("find_primary_ind_var: Can only be run on unclassified loops");
    }
  
  for (bb_iter(back_edge_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      El_normalize_operands_by_list(cur_bb, basic_ind_var);

      branch_op = El_get_branch_op(cur_bb);

      /*
      ** This may already be a brlc'ified loop
      */
      if(is_brlc(branch_op))
	{
	  primary_ind_var = branch_op->dest(DEST1);
	  find_primary_ind_var_ops();
	  category = DO_LOOP;
	  return;
	}

      if(!is_conditional_branch(branch_op))
	{
	  category = WHILE_LOOP;
	  return;
	}

      Op* cond_op = El_get_cond_for_branch(branch_op);

      // If condition is not invariant, we have a while_loop
      if(!is_invariant(&cond_op->src(SRC2)))
	{
	  category = WHILE_LOOP;
	  return;
	}

      if(basic_ind_var.is_member(cond_op->src(SRC1)))
	{
	  potential_primary_ind_var += cond_op->src(SRC1);
	}
    }
  
  if(potential_primary_ind_var.size() != 1)
    {
      category = WHILE_LOOP;
      return;
    }
  
  primary_ind_var = potential_primary_ind_var.head();
  find_primary_ind_var_ops();

  /*
   *  Ensure that all invariants are the same and that all the
   *  ops are not predicated.
   */

  Operand invariant = primary_ind_var_ops.head()->src(SRC2);
  for(op_iter(primary_ind_var_ops); op_iter != 0; op_iter++)
    {
      Op* cur_op = *op_iter;

      if(!cur_op->src(PRED1).is_predicate_true())
	{
	  category = WHILE_LOOP;
	  return;
	}

      if(cur_op->src(SRC2) != invariant)
	{
	  category = WHILE_LOOP;
	  return;
	}
    }
    
  /*
   *  Ensure all backedges have the same number of potential
   *  primary induction ops.
   */
  
  can_encounter_ops = -1;
  for(bb_iter(back_edge_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      
      if(can_encounter_ops == -1)
	{
	  can_encounter_ops = potential_primary_ind_op_count(cur_bb);
	}
      else
	{
	  if(can_encounter_ops != potential_primary_ind_op_count(cur_bb))
	    {
	      category = WHILE_LOOP;
	      return;
	    }
	}
    }
    
  /*
   *  Ensure that all backedges are dominated by the same number of
   *  potential primary induction ops.
   */
  
  for(bb_iter(back_edge_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      int must_encounter_ops = 0;
      for(op_iter(primary_ind_var_ops); op_iter != 0; op_iter++)
	{
	  Op* cur_op = *op_iter;

	  if(control->is_dom((Basicblock*)cur_op->parent(), cur_bb))
	    {
	      must_encounter_ops++;
	    }
	}
      if(must_encounter_ops != can_encounter_ops)
	{
	  category = WHILE_LOOP;
	  return;
	}
    }

  /*
  **  Ensure that all backedges have the same branch cond
  */
  
  for(bb_iter(back_edge_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      Op* cond_op = NULL;
      if(!cond_op)
	{
	  Op* branch_op = El_get_conditional_branch_op(cur_bb);
	  cond_op = El_get_cond_for_branch(branch_op);
          if (!cond_op->src(PRED1).is_predicate_true()) {
            category = WHILE_LOOP;
            return;
          }
	}
      else
	{
	  Op* branch_op = El_get_conditional_branch_op(cur_bb);
	  if(cond_op != El_get_cond_for_branch(branch_op))
	    {
	      category = WHILE_LOOP;
	      return;
	    }
	}
    }

  /*
  **  We made it, this must be a do loop.
  */
  category = DO_LOOP;
    
}

void El_Loop::find_primary_ind_var_ops()
{

  for (List_set_iterator<Basicblock*> bb_iter(loop_blocks); bb_iter != 0; bb_iter++)
    {
      for (Region_all_ops op_iter(*bb_iter); op_iter != 0; op_iter++)
	{
	  Op* cur_op = *op_iter;

	    if (cur_op->num_dests() < 1 || !cur_op->dest(DEST1).is_reg())
	      {
		continue;
	      }
            if (primary_ind_var != (cur_op->dest(DEST1)))
	      {
		continue;
	      }
	  	  
	  /*
	   *  Record as basic induction variable op
	   */
	  
	  primary_ind_var_ops += cur_op;
	}
    }
}

void El_Loop_Graph::find_induction_variable_info(const Loop_filter* filter)
{
  for (Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      El_Loop* loop = (*loop_iter).second;

      if (!filter_match(filter, loop))
	{
	  continue;
	}

      loop->find_induction_variables();
      loop->find_induction_ops(); 
      loop->find_induction_init_vals();

      loop->find_primary_ind_var(control);
    }
}



/*
 *  The following is used to determine if a loop is SWP
 */

void El_Loop::mark_swp_able()
{
  // SLARSEN: Don't swp if flag is set
  if(header_block->flag(EL_DONT_SWP))
    {
      swp_status = NOT_SWP_ABLE;
      return;
    }


  // If it is not a counted loop, it is not swpable
  if(category == WHILE_LOOP)
    {
      swp_status = NOT_SWP_ABLE;
      return;
    }

  if(category != DO_LOOP)
    {
      El_punt("El_Loop::mark_swp_able: You must run find_induction_variable_info first");
    }

  // if it doesn't have exactly 1 back edge, it is not swpable
  if(back_edge_blocks.size() != 1)
    {
      swp_status = NOT_SWP_ABLE;
      return;
    }
  
  // if it doesn't have exactly 1 exit edges, it is not swpable
  if(exit_blocks.size() != 1)
    {
      swp_status = NOT_SWP_ABLE;
      return;
    }

  // if it contains a jsr, it is not swpable
  int tot_num_ops = 0; // total number of operations in the loop
  Real_op_filter real_op_filter;
  for(List_set_iterator<Basicblock*> bb_iter(loop_blocks); bb_iter!=0; bb_iter++)
    {
      if(El_region_has_jsr(*bb_iter))
	{
	  swp_status = NOT_SWP_ABLE;
	  return;
	}
    for (Region_all_ops opi1(*bb_iter, &real_op_filter); opi1 != 0; opi1++)
      tot_num_ops++;
    }

  // If it is not contigous, it is not swpable

  if(!is_contiguous())
    {
      swp_status = NOT_SWP_ABLE;
      return;
    } 

  // If it contains any internal control flow it's not swpable
  // This check is too pessimistic, need to fix

  /* SLARSEN: { Making this less pessimistic */
  // Actually the modulo scheduler doesn't work with loops containing > 1 blocks.
  // Making it pessimistic again.  -KF 1/2007
  if (loop_blocks.size() != 1)
    {
      swp_status = NOT_SWP_ABLE;
      return;
    } 
//   bool found_br = false;
//   for (List_set_iterator<Basicblock*> iter(loop_blocks); iter != 0; iter++)
//     {
//       for (Region_all_ops op_iter(*iter); op_iter != 0; op_iter++)
// 	{
// 	  if (is_proper_branch(*op_iter))
// 	    {
// 	      if (found_br) 
// 	        {
// 		  swp_status = NOT_SWP_ABLE;
// 		  return;
// 		}
// 	      found_br = true;
// 	    }
// 	}
//     }
//   /* } SLARSEN */
  
  // SAM 6-99, fixed bug with hyperblock containing multiple BB's, with the
  // first one appearing to be a loop.  This not allowed to be SWP'd.
  Compound_region *parent = header_block->parent();
  if ((parent->is_hb()) &&
	(parent->first_subregion() != parent->last_subregion()))
    {
      swp_status = NOT_SWP_ABLE;
      return;
    }

  // check to see if loop is too large to be pipelined
  if (tot_num_ops >= El_swploop_size_threshold) 
  {
     swp_status = NOT_SWP_ABLE;
     return;
  }



  swp_status = SWP_ABLE;
  return;
  
}


void El_Loop_Graph::find_swp_able_loops(const Loop_filter* filter)
{
  for (Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      El_Loop* loop = (*loop_iter).second;

      if (!filter_match(filter, loop))
	{
	  continue;
	}
      
      // if it is not an INNERMOST loop then it is not swpable
      if (child.is_bound(loop))
	{
	  loop->swp_status = NOT_SWP_ABLE;
	  continue;
	}

      loop->mark_swp_able();

    }
}


Reference_dtype_attr *create_and_set_rdt_attr(Op *op)
{
  int i;
  Reference_dtype_attr *rdt_attr;

  rdt_attr = get_reference_dtype_attr(op);
  if (rdt_attr != NULL)
    El_punt("create_and_set_rdt_attr: op %d already has rdt_attr!!", op->id());

  rdt_attr = new Reference_dtype_attr;
  rdt_attr->resize(op);

  for (i=1; i<=op->num_dests(); i++) {
    rdt_attr->set_dest(((Port_num)i), op->dest((Port_num)(i)).dtype());
  }
  for (i=1; i<=op->num_srcs(); i++) {
    rdt_attr->set_src(((Port_num)i), op->src((Port_num)(i)).dtype());
  }

  set_reference_dtype_attr(op, rdt_attr);
  return (rdt_attr);
}

void static set_lc_width_attr(Op *op, int lc, int last_lc)
{
  // Insert a width annotation on the new op if width analysis has been done
  Reference_dtype_attr *op_rdt_attr = create_and_set_rdt_attr(op);

  if (last_lc != -1) {
      DType ndt;
      ndt.calc_and_set_width(last_lc, true);
      op_rdt_attr->set_src(SRC1, ndt);
  }
  if (lc != -1) {
      DType ndt;
      ndt.calc_and_set_width(lc, true);
      op_rdt_attr->set_dest(DEST1, ndt);
  }
}

/*
 *  The following inserts brlc into counted loops
 */

void El_Loop::insert_brlc()
{
  if (preloop_block == NULL)
    {
      El_punt("El_Loop::insert_brlc: Loop must have preheader.");
    }
  
  if (category != DO_LOOP)
    {
      El_punt("El_Loop::insert_brlc: BRLC can only be inserted into counted loops");
    }
  
  Op* branch_op = El_get_branch_op(back_edge_blocks.head());

  if(is_brlc(branch_op))
    {
      /* BRLC already created for this loop */
      return;
    }

  Op* cond_op = El_get_cond_for_branch(branch_op);
  Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(branch_op);
  
  Opcode test_cond = cond_op->opcode();

  Operand final_var = cond_op->src(SRC2);
  Operand ind_var = cond_op->src(SRC1);
  Operand pred_true = new Pred_lit(true);

  if(ind_var != primary_ind_var)
    {
      El_punt("El_Loop::insert_brlc: Something funny is going on with primary_ind_var's");
    }

  Operand step_var = primary_ind_var_ops.head()->src(SRC2);

  Operand loop_count_var = new Macro_reg(LC_REG);
  Operand literal_one = new Int_lit(1);
  Operand literal_zero = new Int_lit(0);
  Operand literal_negone = new Int_lit(-1);

  bool is_ind_op_sub = is_sub(primary_ind_var_ops.head());

  int last_lc = -1, lc = -1;

  // To set LC to the desired tripcount if machine supports predicates
  // Step 1) lc = final - inital or lc = initial - final (if induction op is SUB)
  // Step 2) lc = lc + step
  // Step 3) if < or > loop, lc = lc + (step>=0 ? -1 : 1)
  // Step 4) lc = lc / step
  // Step 5) lc = lc - 1

  // Step 1
  // lc = final - initial  or
  // lc = initial - final (if induction op is SUB)
  Opcode subtract = El_subtract_opcode_for_induction_op(primary_ind_var_ops.head());
  Op* sub_op = new Op(subtract);
  sub_op->set_dest(DEST1, loop_count_var);
  sub_op->set_src(PRED1, pred_true);
  if (!is_ind_op_sub) {
    sub_op->set_src(SRC1, final_var);
    sub_op->set_src(SRC2, ind_var);
  } else {
    sub_op->set_src(SRC2, final_var);
    sub_op->set_src(SRC1, ind_var);
  }
  El_insert_op_before_switch(preloop_block, sub_op);

  // Insert a width annotation on the new subtract if width analysis has been done
  if (rdt_attr != NULL)  {
    if (!basic_ind_var_init_vals->is_bound(ind_var))
      lc = -1;
    else {
      Operand init_var = basic_ind_var_init_vals->value(ind_var);
      if (!final_var.is_int() || !init_var.is_int())
        lc = -1;
      else {
        if (is_ind_op_sub)
          lc = init_var.int_value() - final_var.int_value();
        else
	  lc = final_var.int_value() - init_var.int_value();
      }
    }
    set_lc_width_attr(sub_op, lc, last_lc);
    last_lc = lc;
  }
  
  // Step 2
  // lc = lc + step
  Opcode add = El_add_opcode_for_induction_op(primary_ind_var_ops.head());
  Op *add_op = new Op(add);
  add_op->set_src(SRC1, loop_count_var);
  add_op->set_src(SRC2, step_var);
  add_op->set_src(PRED1, pred_true);
  add_op->set_dest(DEST1, loop_count_var);
  El_insert_op_before_switch(preloop_block, add_op);

  // Insert a width annotation on the new add if width analysis has been done
  if (rdt_attr != NULL)  {
    if (last_lc != -1 && step_var.is_int()) 
      lc = last_lc + step_var.int_value();
    else
      lc = -1;
    set_lc_width_attr(add_op, lc, last_lc);
    last_lc = lc;
  }

  // If predicates not supported
  // x = (step < 0) // x is sign of step
  // y = x << 1
  // z = y - 1
  // lc = lc + z

  // Step 3, if predicates supported
  // if < or > loop 
  if(El_sub_one_from_loop_count(test_cond)) {
    Op *cmpp_op = new Op((Opcode)CMPP_W_GEQ_UN_UC);
    cmpp_op->set_src(SRC1, step_var);
    cmpp_op->set_src(SRC2, literal_zero);
    cmpp_op->set_src(PRED1, pred_true);
    Operand step_is_pos = new Reg(EL_STANDARD_PREDICATE);
    Operand step_is_neg = new Reg(EL_STANDARD_PREDICATE);
    cmpp_op->set_dest(DEST1, step_is_pos);
    cmpp_op->set_dest(DEST2, step_is_neg);
    El_insert_op_before_switch(preloop_block, cmpp_op);

    // if step is positive (from above cmpp), lc = lc - 1
    add = El_add_opcode_for_induction_op(primary_ind_var_ops.head());
    add_op = new Op(add);
    add_op->set_src(SRC1, loop_count_var);
    add_op->set_src(SRC2, literal_negone);
    add_op->set_src(PRED1, step_is_pos);
    add_op->set_dest(DEST1, loop_count_var);
    El_insert_op_before_switch(preloop_block, add_op);

    // if step is negative (from above cmpp), lc = lc + 1
    add = El_add_opcode_for_induction_op(primary_ind_var_ops.head());
    add_op = new Op(add);
    add_op->set_src(SRC1, loop_count_var);
    add_op->set_src(SRC2, literal_one);
    add_op->set_src(PRED1, step_is_neg);
    add_op->set_dest(DEST1, loop_count_var);
    El_insert_op_before_switch(preloop_block, add_op);

    // Insert a width annotation on the new add if width analysis has been done
    if (rdt_attr != NULL)  {
      if (step_var.is_int() && (step_var.int_value() >= 0)) {
        if (last_lc != -1)
          lc = last_lc - 1;
        else
          lc = -1;
      } else if (step_var.is_int() && (step_var.int_value() < 0)) {
        if (last_lc != -1)
          lc = last_lc + 1;
        else
          lc = -1;
      }
      set_lc_width_attr(add_op, lc, last_lc);
      last_lc = lc;
    }
  }

  // Step 4
  // lc = lc / step
  Op* div_op = NULL;

  // NTC: This divide must be here until the end of modulo scheduling
  // for modulo variable expansion to work. There is a postpass after
  // modulo scheduling to get rid of it.

  // if ((!step_var.is_int()) || (step_var.int_value()!=1)) {

  // This needs to be a signed divide. If it's not, then loops
  // counting down using unsigned induction variables will have
  // incorrect trip counts. E.g., in the loop 'for(unsigned i = 16; i
  // > 0; i--)' the div will do 15 / -1, and if the divide treats -1
  // as unsigned, then the trip count is completely screwed
  // up. This happens in cjpeg/djpeg. NTC 2/1/07

  // BAD:
  // Opcode divide = El_divide_opcode_for_induction_op(primary_ind_var_ops.head());
  // div_op = new Op(divide);

  div_op = new Op(DIV_W);
  assert(div_op && "out of memory");
  div_op->set_src(SRC1, loop_count_var);
  div_op->set_src(SRC2, step_var);
  div_op->set_dest(DEST1, loop_count_var);
  div_op->set_src(PRED1, pred_true);
  El_insert_op_before_switch(preloop_block, div_op);
  // }

  // Insert a width annotation on the new divide if width analysis has been done
  if (rdt_attr != NULL)  {
    if (last_lc != -1 && step_var.is_int()) 
      lc = last_lc / step_var.int_value();
    else
      lc = -1;
    set_lc_width_attr(div_op, lc, last_lc);
    last_lc = lc;
  }

  // Step 5
  // lc = lc - 1
  subtract = El_subtract_opcode_for_induction_op(primary_ind_var_ops.head());
  sub_op = new Op(subtract);
  sub_op->set_src(SRC1, loop_count_var);
  sub_op->set_src(SRC2, literal_one);
  sub_op->set_dest(DEST1, loop_count_var);
  sub_op->set_src(PRED1, pred_true);
  El_insert_op_before_switch(preloop_block, sub_op);

  // Insert a width annotation on the new divide if width analysis has been done
  if (rdt_attr != NULL)  {
    if (last_lc != -1)
      lc = last_lc - 1;
    else
      lc = -1;
    set_lc_width_attr(sub_op, lc, last_lc);
    last_lc = lc;
  }

  loopcount = lc;

  /*
  ** Remove BRANCH and COMPARE, replace with BRLC
  */
  for(Hash_set_iterator<Basicblock*> bb_iter(back_edge_blocks); bb_iter!=0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      Op* branch_op = El_get_conditional_branch_op(cur_bb);
      Operand pbr_dest = branch_op->src(SRC1);
      Pred_lit *br_pred_lit = new Pred_lit(true);
      Operand br_pred(br_pred_lit);

      Op* cond_op = El_get_cond_for_branch(branch_op);
      El_remove_op(cond_op);
      delete cond_op;

      Op* brlc_op = new Op(BRLC);
      brlc_op->set_src(PRED1, br_pred);
      brlc_op->set_src(SRC1, pbr_dest);
      brlc_op->set_src(SRC2, loop_count_var);
      brlc_op->set_dest(DEST1, loop_count_var);
      // Insert a width annotation on the new divide if width analysis has been done
      if (rdt_attr != NULL) {
        Reference_dtype_attr *brlc_rdt_attr = create_and_set_rdt_attr(brlc_op);
        // For the dest width, detect special case of constant final and initial value
        // for the induction variable, if so size appropriately, otherwise set to LC width
        if (lc != -1) {
          DType ndt;
	  ndt.calc_and_set_width(lc, true);
          brlc_rdt_attr->set_dest(DEST1, ndt);
          brlc_rdt_attr->set_src(SRC2, ndt);
	}
      }
      El_replace_op(cur_bb, branch_op, brlc_op);
      delete branch_op;
    }
}


void El_Loop_Graph::insert_brlc(const Loop_filter* filter)
{
  for (Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      El_Loop* loop = (*loop_iter).second;

      if (!filter_match(filter, loop))
	{
	  continue;
	}

      if(loop->category == DO_LOOP)
	{
	  loop->insert_brlc();
	}
    }
}


bool El_Loop::region_is_subset(Compound_region* reg)
{
  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++) 
    {
      Basicblock* second_layer = (Basicblock*)(*op_iter)->parent();
      if(!second_layer->is_bb())
	{
	  El_punt("El_Loop::region_is_subset: What happened to the bb tiling?");
	}

      /*
      ** if the bb in the region is not a member of the loop it is not a subset
      */
      if(!loop_blocks.is_member(second_layer))
	{
	  return false;
	}
    }
  /*
  ** If we get here we must be a subset
  */
  return true;
}

bool El_Loop::region_is_subset(Op* op)
{
  Basicblock* second_layer = (Basicblock*)op->parent();
  if(!second_layer->is_bb())
    {
      El_punt("El_Loop::region_is_subset: What happened to the bb tiling?");
    }
  
  /*
  ** if the bb in the region is not a member of the loop it is not a subset
  */
  if(!loop_blocks.is_member(second_layer))
    {
      return false;
    }
      
  return true;
}

/*
 *  The following inserts loop regions
 */

void El_Loop_Graph::insert_loop_regions(const Loop_filter* filter)
{
  for (Map_iterator<int, El_Loop*> loop_iter(id); loop_iter != 0; loop_iter++)
    {
      El_Loop* loop = (*loop_iter).second;

      if (!filter_match(filter, loop))
	{
	  continue;
	}

      loop->insert_loop_region();

    }
}


bool El_Loop::is_contiguous()
{
  /*
  ** We can assume loop blocks are in order.
  */

  Compound_region* outer_most = header_block;
  while (outer_most->parent())
    {
      outer_most = outer_most->parent();
    }
  
  Basicblock* cur_block = NULL;
  List_set<Basicblock*> blocks(loop_blocks);
  for(Region_all_ops op_iter(outer_most); op_iter != 0; op_iter++)
    {
      Basicblock* cur_bb = (Basicblock*)((*op_iter)->parent());

      if (loop_blocks.is_member(cur_bb))
	{
	  if(!cur_block)
	    {
	      if (blocks.head() == cur_bb)
		{
		  cur_block = cur_bb;
		  blocks -= cur_bb;
		}
	      else
		{
		  El_punt("El_Loop::is_contiguous: loop blocks are not in order.");
		}
	    }
	  else
	    {
	      if (cur_block != cur_bb)
		{
		  if (blocks.head() == cur_bb)
		    {
		       cur_block = cur_bb;
		       blocks -= cur_bb;
		    }
		  else
		    {
		      return false;
		    }
		}
	    }
	}
      else
	{
	  if(cur_block)
	    {
	      return false;
	    }
	}
      if (blocks.is_empty())
	{
	  return true;
	}
    }
  return true;
}


void El_Loop::insert_loop_region()
{
  /*
  **  Check to make sure region has not been already created
  */

  Op* entry_op = get_first_region_op_from_inops(header_block);
  Compound_region* parent_indx = entry_op->parent();
  while(parent_indx && !parent_indx->is_loopbody())
    {
      parent_indx = parent_indx->parent();
    }
  if(parent_indx && parent_indx->is_loopbody() &&
     entry_op == get_first_region_op_from_inops(parent_indx))
    {
      /* Loop region already exists for this loop */
      return;
    }

  
  /*
  ** Assert loop is contiguous
  */
  if(!is_contiguous())
    {
      El_punt("El_Loop::insert_loop_region: can only handle contiguous loop bodies");
    }
  
  /*
  ** Compute a list of all loop immediate subregions
  */
  Hash_set<Compound_region*> loop_imm_subregions(hash_cmpd_region_ptr, 16);
  Hash_set_iterator<Basicblock*> bb_iter ;
  for(List_set_iterator<Basicblock*> bb_iter2(loop_blocks); bb_iter2 != 0; bb_iter2++)
    {
      Compound_region* parent = (*bb_iter2);
      while(region_is_subset(parent->parent()))
	{
	  parent = parent->parent();
	}
      loop_imm_subregions += parent;
      parent->set_flag(EL_REGION_SOFTPIPE);
      parent->set_flag(EL_REGION_VIOLATES_LC_SEMANTICS);
    }
  
  LoopBody* loop_region = new LoopBody();
  
  loop_region->weight = num_invocation;
  if (loopcount != -1) 
    set_loop_count(loop_region, loopcount);

  /*
  ** Set parent child/relationship with parent of loop region
  */
  Compound_region* head = loop_imm_subregions.head();
  head->parent()->insert_before_region(loop_region, head);
  loop_region->set_parent(head->parent());

  /*
  ** Set parent child/relationship with subregions of loop region
  */
  for(Hash_set_iterator<Compound_region*> reg_iter(loop_imm_subregions);
      reg_iter != 0; reg_iter++)
    {
      Compound_region* reg = *reg_iter;
      reg->parent()->remove_region(reg);
      reg->set_parent(loop_region);
      loop_region->add_region(reg);
    }
      
  /*
  ** Set the in and out ops/edges
  */
  for(bb_iter(exit_blocks); bb_iter != 0; bb_iter++)
    {
      Basicblock* exit = (*bb_iter);
      Op* out_op = get_last_region_op_from_outops(exit);
      loop_region->add_exit(out_op);
      for(Region_all_outedges edge_iter(out_op); edge_iter != 0; edge_iter++)
	{
	  Edge* edge = *edge_iter;
	  if(!loop_blocks.is_member((Basicblock*)edge->dest()->parent()))
	    {
	      out_op->add_outedge_recursively(edge);
	    }
	  else
	    {
	      loop_region->addBackEdge(edge);
	    }
	}
    }
  Op* in_op = get_first_region_op_from_inops(header_block);
  loop_region->add_entry(in_op);
  for(Region_all_inedges edge_iter(in_op); edge_iter != 0; edge_iter++)
    {
      Edge* edge = *edge_iter;
      if(!loop_blocks.is_member((Basicblock*)edge->src()->parent()))
	{
	  in_op->add_inedge_recursively(edge);
	}
    }
}

// Note: when inserting compound region in a bb-tiled hyperblock, compound
// region is always inserted at the end, which may not be the right place.
Compound_region* El_Loop::insert_compound_region() 
{
   int i ;
   List_iterator<int> li ;
   // Find where in the region hierarchy this compond
   // region will be inserted, also find which regions
   // will be in it

   // find the root of the region hierarchy
   Compound_region* root = header_block ;
   while(root->parent() != NULL) root = root->parent() ;
				  
   Alist_region_hierarchy_graph gr(root) ;
   Bitvector in_comp_reg(gr.node_count) ;
   for(List_set_iterator<Basicblock*> bbi(loop_blocks) ; bbi != 0 ; bbi++) {
      in_comp_reg.set_bit(gr.f_map.value(*bbi)) ;
   }
   for(Alist_graph_post_dfs nodei(gr) ; nodei != 0 ; nodei++) {
      int cur_node = *nodei ;
      bool all_succ_in = true ;
      if (!in_comp_reg.bit(cur_node)) {
	 // cur_node will not be included in the set of regions if
	 // (i) it is a leaf node and it is not included in the initial
	 //     set of subregions
	 // (ii) it has subregions and not all of the subregions are marked
	 //      to be included in the set
	 li(gr.succ_list[cur_node]) ;
	 if (li == 0) all_succ_in = false ; 
	 for ( ; li != 0 ; li++) {
	    int cur_succ = *li ;
	    if (!in_comp_reg.bit(cur_succ)) {
	       all_succ_in = false ;
	    }
	 }
      }
      if (all_succ_in) {
	 // insert this node to the set and remove its subregions
	 // this will ensure that the set at he end of this loop will
	 // include only the covering regions and does not include
	 // any regions which are covered
	 in_comp_reg.set_bit(cur_node) ;
	 // reset all successors bits so that we have the coverage set
	 for (li(gr.succ_list[cur_node]) ; li != 0 ; li++) {
	    int cur_succ = *li ;
	    in_comp_reg.reset_bit(cur_succ) ;
	 }
      }
   }
   // At this point in_comp_reg is a coverage of the original set
   // Check if all elements of the set have the same parent
   // if that is the case insertion is successful, otherwise
   // either return NULL or exit with an error
   Compound_region* common_parent = NULL ;
   i = 0 ;
   while ((!in_comp_reg.bit(i)) && (i < gr.node_count)) i++ ;
   if (i < gr.node_count) {
      common_parent = gr.b_map[i]->parent() ;
      i++ ;
   }
   while (i < gr.node_count) {
     if (in_comp_reg.bit(i)) {
       if (common_parent != gr.b_map[i]->parent()) {
	 // assert(0) ; // The nodes don't share a common parent
	 return NULL ;
       }
     }
     i++ ;
   }
   
   // first fix the parent subregion pointers
   Compound_region* new_cr = new Compound_region() ;
   new_cr->set_parent(common_parent) ;
   common_parent->add_region(new_cr) ;
   for (i = 0 ; i < gr.node_count ; i++) {
      if (in_comp_reg.bit(i)) {
	 Region* cur_reg = gr.b_map[i] ;
	 common_parent->remove_region(cur_reg) ;
	 new_cr->add_region(cur_reg) ;
	 cur_reg->set_parent(new_cr) ;
      }
   }
   // second set the region entry edges and ops for new_cr
   Hash_set_iterator<Basicblock*> bb_iter ;
   for(bb_iter(exit_blocks); bb_iter != 0; bb_iter++)
   {
      Basicblock* exit = (*bb_iter);
      Op* out_op = get_last_region_op_from_outops(exit);
      new_cr->add_exit(out_op);
      for(Region_all_outedges edge_iter(out_op); edge_iter != 0; edge_iter++)
      {
	 Edge* edge = *edge_iter;
	 if(!loop_blocks.is_member((Basicblock*)edge->dest()->parent()))
	 {
	    // This handles all loop exits. Loop-back edges are not
	    // region exits
	    new_cr->add_exit(edge);
	 }
      }
   }
  
   Op* in_op = get_first_region_op_from_inops(header_block);
   new_cr->add_entry(in_op);
   for(Region_all_inedges edge_iter(in_op); edge_iter != 0; edge_iter++)
   {
      Edge* edge = *edge_iter;
      if(!(loop_blocks.is_member((Basicblock*)edge->src()->parent()))) {
	 new_cr->add_entry(edge);
      }
   }
   return(new_cr) ;
}


/*
 * Loop filter - This determines which loops get processed when a filter is passed
 */

bool El_Loop_Graph::filter_match(const Loop_filter* filter, El_Loop* loop)
{
  return(filter->filter(loop));
}


El_Loop*  el_find_innermost_loop_containing_bb(El_Loop_Graph& loops,Basicblock* bb)
{
  
 for(El_Loop_Graph_Post_DFS l_iter(loops);l_iter!=0;l_iter++)
   {
     El_Loop* loop=*l_iter;
     if(loop->loop_blocks.is_member(bb))
       return loop;
   }
 return NULL; 
}



//Given a loop, find all the bbs in that loop that are not in
//one of it nested loops; in practice, we only need to test
//the bb is not in any loop of nesting level greater than that of
//the current loop
void el_find_loop_bbs_not_in_nested_loops(El_Loop* loop, El_Loop_Graph& loops,
				       List_set<Basicblock*>& todo_bbs)
{
  for(List_set_iterator<Basicblock*> lb_iter(loop->loop_blocks);lb_iter!=0;lb_iter++)
    {
      Basicblock* bb = *lb_iter;

      
      bool include=true;
      //bb shouldn't be in _any_ loop of higher nesting level
      for(El_Loop_Graph_Post_DFS l_iter(loops);l_iter!=0;l_iter++)
	{
	  El_Loop* other_loop=*l_iter;
	  if((other_loop->nesting_level>loop->nesting_level)&&
	     (other_loop->loop_blocks.is_member(bb)))
	    include=false;
	}
      if(include)
	todo_bbs+=bb;

    }
  return;
}

