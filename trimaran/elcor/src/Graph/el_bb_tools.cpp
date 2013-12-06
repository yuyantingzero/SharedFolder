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
//      File:           el_form_bb.cpp
//      Author:         David I. August, Marnix Arnold
//      Created:        May 1995
//      Description:    Form Basic Blocks For Later Analysis
//
/////////////////////////////////////////////////////////////////////////////

#include "edge_utilities.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "op.h"
#include "edge.h"
#include "attributes.h"
#include "edge_attributes.h"
#include "op_attributes.h"
#include "el_loop.h"
#include "el_loop_iterators.h"
#include "list_set.h"
#include "dbg.h"

#undef DEBUG



// ------------------------------------------
// Returns false if region is not a complete BB tiling.  bb_set is returned
// with an entry for each basic block.
// ------------------------------------------

bool El_confirm_BB_tiling(Region* region, List_set<Basicblock*>& bb_set)
{
  Region_all_ops op_iter;
  Region *second_layer;
  
  //  Make a list of structures containing the basic blocks to be removed
  bb_set.clear();
  for(op_iter(region); op_iter != 0; op_iter++) 
    {
      second_layer = (*op_iter)->parent();
      if(second_layer->is_bb())
	{
	  bb_set += (Basicblock *)second_layer;
#if DEBUG
	  cout << "ADDING BB " << ((Basicblock *)second_layer)->id() << endl;
#endif
	}
      else
	{
	  return false;
	}
    }
  return true;
}


// ------------------------------------------
// Algorithm to remove unnecessary basic blocks.
//
// Input: An elcor program with or without basic block regions.
//
// Output: An elcor program which contains only basic blocks that are immediate
// children of a procedure.
//
// Method:
//
// 1. Determine the set basic blocks whose parent is not the procedure.
//
// 2. Remove the chosen basic blocks by moving the ops contained in the basic 
// block to its parent region.
// ------------------------------------------

void El_remove_basic_blocks(Compound_region* reg)
{
  El_remove_basic_blocks(reg, false);
}

void El_remove_all_basic_blocks(Compound_region* reg)
{
  El_remove_all_basic_blocks(reg, false);
}

void El_remove_basic_blocks(Compound_region* reg, bool remove_db_cm)
{
  Hash_set<Compound_region*> region_set(hash_cmpd_region_ptr, 1024);
  Region *second_layer;
  Region_all_ops op_iter;

  /*
   * Make a list of structures containing the basic blocks to be removed
   */

  for(op_iter(reg); op_iter != 0; op_iter++) 
    {
      Op* cur_op = *op_iter;
      second_layer = cur_op->parent();
      if(second_layer->is_bb())
	{
	  Region *second_layer_parent = second_layer->parent();
	  if(    (second_layer_parent != reg) 
		 && (!second_layer_parent->is_procedure())
		 && (!second_layer_parent->is_loopbody()))
	    {
	      region_set += (Compound_region *)second_layer_parent;
	    }
	}
    }

  El_remove_basic_blocks_from_set(&region_set);
  if(remove_db_cm)
    El_remove_db_cm(reg);
}

void El_remove_all_basic_blocks(Compound_region* reg, bool remove_db_cm)
{
  Hash_set<Compound_region*> region_set(hash_cmpd_region_ptr, 1024);
  Region *second_layer;
  Region_all_ops op_iter;

  /*
   * Make a list of *all* structures containing basic blocks
   */

  for(op_iter(reg); op_iter != 0; op_iter++) 
    {
      Op* cur_op = *op_iter;
      second_layer = cur_op->parent();
      if(second_layer->is_bb())
	{
	  Region *second_layer_parent = second_layer->parent();
	  region_set += (Compound_region *)second_layer_parent;
	}
    }

  El_remove_basic_blocks_from_set(&region_set);
  if(remove_db_cm)
    El_remove_db_cm(reg);
}

void El_remove_basic_blocks_from_set(Hash_set<Compound_region*> *region_set_ptr)
{
  List_set<Compound_region*> bb_set;
  Hash_set_iterator<Compound_region*> cr_iter;
  List_set_iterator<Compound_region*> bb_iter;
  Compound_region *cur_reg;
  Compound_region *cur_bb;
  List_iterator<Op*> op_list_iter;
  Op *cur_op; 
  List<Op*> op_list;
  Region_all_ops op_iter;
  /*
   * Perform the actual removal of the basic blocks
   */
  
  for(cr_iter(*region_set_ptr); cr_iter!=0; cr_iter++)
    {   
      cur_reg = *cr_iter;
   
      bb_set.clear() ;
      for(Region_subregions bb_sub_iter(cur_reg); bb_sub_iter != 0; bb_sub_iter++)
	{
	  cur_bb = (Compound_region*)(*bb_sub_iter);
	  if(cur_bb->is_bb())
	    {
	      bb_set += cur_bb;
	    }
	}

      for(bb_iter(bb_set); bb_iter != 0; bb_iter++)
	{
	  cur_bb = *bb_iter;
	  cur_reg->remove_region(cur_bb);
	}

      for(bb_iter(bb_set); bb_iter != 0; bb_iter++)
	{
	  cur_bb = *bb_iter;
	  op_list.clear() ;

	  for (op_iter(cur_bb); op_iter!=0; op_iter++)
	    {
	      op_list.add_tail(*op_iter);
	    }

	  for(op_list_iter(op_list); op_list_iter != 0; op_list_iter++) 
	    { 
	      cur_op = *op_list_iter;
	      cur_op->set_parent(cur_reg);
	      cur_bb->remove_region(cur_op);
	      cur_reg->add_region(cur_op);
#ifdef DEBUG
	      cout << "MOVING OP " << cur_op->id() << endl;
#endif
	    }
	}

      for(bb_iter(bb_set); bb_iter != 0; bb_iter++)
	{
	  cur_bb = *bb_iter;
#ifdef DEBUG
	  cout << "Deleting bb..." << endl;
	  cout << *cur_bb << endl;
#endif
	  delete cur_bb;
	}
    }
}

/*
** El_remove_op does handle in/out ops now, but use
** with care! Only remove pairs of in/out ops.
*/

Op* El_remove_op(Op* op)
{
  Compound_region *theparent;
  //  C0_remove_op(op);
  // this is modified from C0_remove_op (connect.cpp)
  Edge* src_edge = get_incoming_CONTROL0_edge(op) ;
  Edge* dest_edge = get_outgoing_CONTROL0_edge(op) ;

  if(src_edge == NULL || dest_edge == NULL) {
    cdbg << "El_remove_op(): It looks like Op " << op->id() << " has already been removed.\n"
	 << "I'm not going to mess with it." << endl;
    return op;
  }

  op->remove_outedge(dest_edge, CONTROL, CONTROL0, 0);
  op->remove_inedge(src_edge, CONTROL, CONTROL0, 0);
  Op* src_op = src_edge->src();
  Op* dest_op = dest_edge->dest();
  
  dest_op->remove_inedge(dest_edge, CONTROL, CONTROL0, 0);
  src_edge->set_dest(dest_op, CONTROL0, 0);
  dest_op->add_inedge(src_edge, CONTROL, CONTROL0, 0);
  // end of C0_remove_op-ish code

  if(is_control_merge(op)) // 'fix' entry edges/ops of hierarchy
    {
      for(theparent = op->parent(); !theparent->is_procedure(); 
	  theparent = theparent->parent())
	{
	  theparent->remove_entry(op);
	  theparent->remove_entry(src_edge);
	}
      for(theparent = src_op->parent(); !theparent->is_procedure(); 
	  theparent = theparent->parent())
	{
	  theparent->remove_exit(src_edge);
	}
    }
  else if(is_control_switch(op)) // 'fix' exit edges/ops of hierarchy
    {
      for(theparent = op->parent(); !theparent->is_procedure(); 
	  theparent = theparent->parent())
	{
	  theparent->remove_exit(op);
	  theparent->remove_exit(dest_edge);
	}
      for(theparent = dest_op->parent(); !theparent->is_procedure(); 
	  theparent = theparent->parent())
	{
	  theparent->remove_entry(dest_edge);
	}
    }

  delete dest_edge;

  theparent = op->parent();
  theparent->remove_region(op);

  return op;
}

void El_remove_db_cm(Compound_region *reg)
{
  Hash_set<Compound_region*> region_set(hash_cmpd_region_ptr, 1024);
  Region *second_layer;
  Region_all_ops op_iter;

  /*
   * Make a list of structures containing operations to be removed
   */
  
  for(op_iter(reg); op_iter != 0; op_iter++) 
    {
      Op* cur_op = *op_iter;
      second_layer = cur_op->parent();
      region_set += (Compound_region *)second_layer;
    } 
     
  /* 
  ** If we find an op to remove.....
  */
  for(Hash_set_iterator<Compound_region*> reg_iter(region_set); reg_iter != 0; reg_iter++)
    {
      Hash_set<Op*> remove_op(hash_op_ptr, 32);
      Compound_region* cur_reg = *reg_iter;
      for(op_iter(cur_reg); op_iter != 0; op_iter++) 
	{
	  Op* cur_op = *op_iter;
	  if(is_control_merge(cur_op))
	    {
	      int count = 0;
	      Op* source = NULL;
	      
	      for(Region_all_inedges edge_iter(cur_op); edge_iter !=0; edge_iter++)
		{
		  count++;
		  source = (*edge_iter)->src();
		}
	      if(count == 1 && is_dummy_branch(source) && source->parent() == cur_reg
		 && !(cur_reg->is_entry_op(cur_op) || source->parent()->is_exit_op(source)))
		{
		  remove_op += source;
		  remove_op += cur_op;
		}
	    }
	}
      /*
      ** ....remove it.
      */
      for(Hash_set_iterator<Op*> remop(remove_op); remop!=0; remop++)
	{
	  Op *cur_op = *remop;
	  El_remove_op(cur_op);
	  delete cur_op;  // We never want to see this op again!
	}
    }
}


// ------------------------------------------
// Algorithm to partition into basic blocks.
//
// Input: An elcor program with or without basic block regions.
//
// Output: An elcor program with basic blocks as the immediate parent of every
// operation.  Previously existing basic blocks are left intact.
//   If not_in_hb is true, then no BBs are inserted below HBs -- Marnix.
//
// Method:
//
// 1. Determine the set of leaders: the leader of every basic block is a
// control_merge_op.  This is a requirement of the elcor IR.
//
// 2. For each leader not already in a basic block, a new basic block is formed
// containing the leader and all operations following the leader up to but not
// including the next leader.
// ------------------------------------------

void El_form_basic_blocks (Compound_region* reg, bool not_in_hb)
{
 Hash_set<Compound_region*> region_set (hash_cmpd_region_ptr, 1024);
 Compound_region *cur_reg;
 Op *cur_op;
 Op *prev_op;
 Hash_set_iterator<Compound_region*> cr_iter;
 List_set_iterator<Compound_region*> new_bb_iter;
 List_iterator<Op*> op_list_iter;
 List<Op*> op_list;
 Region_all_ops op_iter;
 
 /* 
  *  Make a list of all regions above ops that are not BBs.
  */

 for(Region_all_ops r_iter(reg); r_iter != 0; r_iter++) 
   {
     Op* op=*r_iter;

     if(!( (op->parent()->is_bb())
	   || (not_in_hb && op->parent()->is_hb()) ))
       {
	 region_set += (Compound_region *)(op->parent()) ;
       }
   }

#ifdef DEBUG
 for(cr_iter(region_set); cr_iter!=0; cr_iter++)
  {   
   cur_reg = *cr_iter;

   cout << cur_reg->id() << " ";
   cout << cur_reg->is_bb() << endl;
  }
 cout << "DONE PRINTING" << endl;
 cout << *reg;
#endif

 for(cr_iter(region_set); cr_iter!=0; cr_iter++)
  {    
   cur_reg = *cr_iter;
   Basicblock* newBB = NULL;
   List_set<Compound_region*> new_bb_list;
   
#ifdef DEBUG
   cout << "NOW ON REG # " << cur_reg->id() << endl;
#endif

   op_list.clear();
   for (op_iter(cur_reg); op_iter!=0; op_iter++)
    {
      cur_op = *op_iter;
      if(cur_op->parent() == cur_reg) // only ops directly below cur_reg
	op_list.add_tail(cur_op);
    }
   
   cur_op = NULL;
   for (op_list_iter(op_list); op_list_iter!=0; op_list_iter++)
    {
     prev_op = cur_op;
     cur_op = *op_list_iter;
     
     /* 
      * Targets of Branches Taken or Fall Through are leaders 
      */
     if(is_control_merge(cur_op))
      {
       if(newBB)
	{
	 /* Finish off previous BB */
	 
         new_bb_list += newBB;
	 
	 for (Region_all_outedges o_edge_iter(prev_op); o_edge_iter!=0; o_edge_iter++)
	  {
	   newBB->add_exit_safely(*o_edge_iter);
	  }
         newBB->add_exit(prev_op);
	}
       
       /*
	* Start new BB 
	*/
       double weight = 0.0;
       int num_inedge = 0;
       for (Op_inedges in_iter(cur_op); in_iter!=0; in_iter++) {
         Edge *inedge = *in_iter;
         Control_flow_freq* cfreq = get_control_flow_freq(inedge);
	 if(cfreq)
	   weight += cfreq->freq;
         num_inedge += 1;
       }
       if (num_inedge==0) {
         weight = reg->weight;
       }

       /* Edge* outedge = get_outgoing_CONTROL0_edge(cur_op);
       Control_flow_freq* cfreq = get_control_flow_freq(outedge) ;
       double weight = cfreq->freq; */
       Merge_op_BB_id* bbid;
       if((bbid = get_existing_merge_op_bb_id(cur_op)))
	 {
	   newBB = new Basicblock(bbid->id, weight);
	 }
       else
	 {
	   newBB = new Basicblock;
	   newBB->weight = weight;
	   bbid = new Merge_op_BB_id();
	   bbid->id = newBB->id();
	   set_merge_op_bb_id(cur_op, bbid);
	 }

       Lcode_attribute_map *lmap = get_lcode_attributes(cur_reg);
       if (lmap->is_bound("trace")) {
         List<Operand> tr_list = lmap->value("trace");
         Lcode_attribute_map *new_lmap = get_lcode_attributes(newBB);
         new_lmap->bind("trace", tr_list);
       }

       newBB->set_parent(cur_reg);
       newBB->add_entry(cur_op);
       for (Region_all_inedges i_edge_iter(cur_op); i_edge_iter!=0; i_edge_iter++)
	{
	 newBB->add_entry_safely(*i_edge_iter);
	}

#ifdef DEBUG
       cout << "Adding new bb " << newBB->id() << endl;
#endif

      }
     cur_reg->remove_region(cur_op);
     cur_op->set_parent(newBB);
     newBB->add_region(cur_op);

#ifdef DEBUG
     cout << "Adding op " << cur_op->id() << " to bb " << newBB->id() << endl;
#endif     

    }
   /* 
    * Finish off last BB 
    */
   new_bb_list += newBB; 
   for (Region_all_outedges o_edge_iter(cur_op); o_edge_iter!=0; o_edge_iter++)
    {
     newBB->add_exit_safely(*o_edge_iter);
#ifdef DEBUG
     cout << "EDGE: Adding successor " << (*o_edge_iter)->dest()->id() << endl;
#endif 
    }
   newBB->add_exit(cur_op);
	
   for(new_bb_iter(new_bb_list); new_bb_iter != 0; new_bb_iter++)
    {
     cur_reg->add_region(*new_bb_iter);
    }
  }
}


void el_topo_sort_bbs(Dlist<Basicblock*>& bb_order,List_set<Basicblock*>& bb_blocks,Basicblock* entry_bb)
{
  
  bb_order.push(entry_bb);
  bool can_include;
  while(bb_order.size() != bb_blocks.size())
    {
      for(List_set_iterator<Basicblock*> bbi2(bb_blocks); bbi2 != 0; bbi2++)
	{
	  Basicblock* cur_bb = *bbi2;
	  if(!bb_order.is_member(cur_bb))
	    {
	      can_include = true;
	      for(Region_all_inedges entry_iter(cur_bb); entry_iter!=0; entry_iter++)
		{
		  Edge* cur_edge = *entry_iter;
		  
		  if(!bb_order.is_member((Basicblock*) cur_edge->src()->parent()))
		    {
		      can_include = false;
		    }
		}
	      if(can_include)
		{
		  bb_order.push_tail(cur_bb);
		}
	    }
	}
    }    
}

Basicblock* el_get_proc_entry_bb(Procedure *f)
{
  
   Region_entry_ops op_iter(f) ;
   Op* op=*op_iter;
   Compound_region* p = op->parent();
   assert(p->is_bb());
   Basicblock* bb=(Basicblock*) op->parent();
   return bb;
}

Op* el_get_bb_entry_op(Basicblock* bb)
{
  int count=0;
  Op* op = NULL;
  
  for(Region_entry_ops op_iter(bb);op_iter!=0;op_iter++,count++)
    {
      op=*op_iter;
    }
  assert(count==1);
  return op;
}


Op* el_get_bb_exit_op(Basicblock* bb)
{
  int opcount=0;
  Op* result = NULL;
  
  for (Region_exit_ops e_op_iter(bb); e_op_iter!=0; e_op_iter++) 
    {
      opcount++;
      result=*e_op_iter;
    }
  assert(opcount==1);
  return result;
}

/**
 * Finds the outermost bb/hb that encloses the given region.
 * This is useful to find the original hb when a bb tiling exists.
 */
Compound_region*
get_enclosing_analysis_scope(Region *reg)
{
  Region *parent = reg->parent();
  assert(parent);

  while (parent->is_bb() || parent->is_hb()) {
    reg    = parent;
    parent = parent->parent();

    assert(parent);
  }

  return dynamic_cast<Compound_region*>(reg);
}


