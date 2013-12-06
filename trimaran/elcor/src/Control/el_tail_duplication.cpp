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
//      File:           el_tail_duplication.cpp
//      Author:         Matthai Philipose
//      Created:        July 1996
//      Description:    Given an acyclic subgraph (D) of the cfg and a set 
//                      of edges into D, tail duplicate the part of D 
//                      downstream of the edges
//                      
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "dbg.h"
#include "edge_utilities.h"
#include "region_utilities.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "op.h"
#include "edge.h"
#include "attributes.h"
#include "edge_attributes.h"
#include "op_attributes.h"
#include "operand.h"
#include "map.h"
#include "tuples.h"
#include "el_tail_duplication.h"
#include <fstream>
#include "el_if_converter.h"
#include "el_loop.h"
#include "el_loop_iterators.h"
#include "hash_functions.h"
#include "el_control_init.h"

using namespace std;

//Yet another test for the if converter; f should already be in normalized form
void el_if_convert_innermost_loops(Procedure *f)
{
  Compound_region* cr;

  El_If_Converter if_conv(f);
  El_Loop_Graph loops(f);

  List_set<Basicblock*> loop_bbs;
  Hash_set<Edge*> entry_edges(hash_edge_ptr, 16);

  //Go through the innermost loops, if converting them
  for(El_Loop_Graph_Innermost loop_iter(loops); loop_iter != 0; loop_iter++)
    {
      El_Loop* loop=*loop_iter;

      //gather blocks in the loop body
      for(List_set_iterator<Basicblock*> lb_iter(loop->loop_blocks);lb_iter!=0;lb_iter++)
	{
	  Basicblock* bb = *lb_iter;
	  loop_bbs += bb;
	}
      
      //gather edges into the entry bblock of the loop body
      for(Region_entry_edges e_iter(loop->header_block);e_iter!=0;e_iter++)
	{
	  Edge* edge = *e_iter;
	  entry_edges += edge;
	}
      
      //form the cr corresponding to this loop body
      cr=el_mark_cr_on_cfg(f,loop_bbs,entry_edges);
      
      //Do the actual if-conversion
      El_if_convert(cr, El_frp_model);

      //nuke the region now it's not needed
      El_remove_region(cr, true);     
      
      //clear data structure for the next iteration
      loop_bbs.clear();
      entry_edges.clear();
    } 
  
  return;
}

//hack to test the tail duplicator on functions which
//contain hbs
void el_old_test_tail_duplicator(Procedure *proc)
{
  List_set<Basicblock*> bb_set;
  Basicblock* cur_bb;
  Hash_set<Edge*> inedges(hash_edge_ptr, 16);
  
  //Extract all the bblocks in the cfg for the procedure
  El_form_basic_blocks(proc);
  El_confirm_BB_tiling(proc,bb_set);
  
  //Go over all the bbs in the procedure trying to find one with a 
  //hb as its parent
  for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
    {
      cur_bb=*bb_iter;
      if(cur_bb->parent()->is_hb())
	{
	  Region_entry_edges entry_iter ;
	  for (entry_iter(cur_bb->parent()); entry_iter!=0; entry_iter++)
	    {
	      inedges+=(*entry_iter);
	    }
	  
	  //The moment you find a hyperblock, test it and return
	  List_set<Basicblock*> new_tail_bbs;
	  Hash_map<Basicblock*,Basicblock*> tail_map(hash_bb_ptr,MAX_BBS_IN_TAIL);
	  el_tail_duplicate((Compound_region*)(cur_bb->parent()),inedges,
			    inedges,new_tail_bbs,tail_map);
	  return;
	}
    }
  
  return;
}

//test routine
void get_cr_bb_list_from_file(List_set<Basicblock*> bbs,
			      List_set<Basicblock*>& region_bbs,
			      Hash_set<Edge*>& top_entries)
{
  //read in the filename where the bbs of the region are listed
  char fname[100];
  cout << "please enter fname\n";
  cin >> fname;
  
  //read in the bb numbers
  ifstream inFile(fname,ios::in);
  if (!inFile){cerr << "could not open" << fname <<endl;exit(-1);}

  Hash_set<int> bb_nums(hash_int, 128); 
  int bb_num = 0,entry_bb_num = 0;
  bool first_time=true;
  
  while (inFile >> bb_num)
    {
      if(first_time)
	{
	  //By convention, all entries of the first bb in the bb files
	  //are the top entries
	  first_time=false;
	  entry_bb_num=bb_num;
	}
      bb_nums+=bb_num;
    }
  assert(bb_nums.size()!=0);
  
  //find the bbs and edges corresponding to these bb nums
  for(List_set_iterator<Basicblock*> bb_iter(bbs); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb=*bb_iter;
      int id=cur_bb->id();
      
      //If the bb is one named in the file,
      //put it in the list of region constituent bbs
      if(bb_nums.is_member(id))
	region_bbs+=cur_bb;
      
      //Record the top entries (as opposed to
      //side entries) of the region
      if(id==entry_bb_num)
	{
	  for (Region_entry_edges entry_iter(cur_bb); entry_iter!=0; entry_iter++)
	    top_entries+= *entry_iter;
	}
    }
  return;
}


void el_if_convert_file_specified_region(Procedure *proc)
{
  List_set<Basicblock*> bb_set;
  Hash_set<Edge*> inedges(hash_edge_ptr, 16);
  
  //Extract all the bblocks in the cfg for the procedure
  El_confirm_BB_tiling(proc,bb_set);
  
  //Read in (from file) the list of bblocks in the region
  List_set<Basicblock*> region_bbs;
  Hash_set<Edge*> top_entries(hash_edge_ptr, 16);
  Compound_region* cr;
  
  //Figure out the region bbs
  get_cr_bb_list_from_file(bb_set,region_bbs,top_entries);
  
  //Mark the region bbs as being in the region (i.e. set the appropriate
  //parent fields)
  cr=el_mark_cr_on_cfg(proc,region_bbs,top_entries);
  
  //Call the tail duplicator over the given region & side entries
  //(side entries == all_entries - top_entries)
  Hash_set<Edge*> side_entries=*compound_region_entry_edges(cr);
  side_entries-= top_entries;
  
  List_set<Basicblock*> new_tail_bbs;
  Hash_map<Basicblock*,Basicblock*> tail_map(hash_bb_ptr,MAX_BBS_IN_TAIL);
  el_tail_duplicate(cr,side_entries,top_entries,new_tail_bbs,tail_map);

  //if convert
    El_if_convert(cr, El_frp_model);
    
  //nuke the region now it's not needed
  El_remove_region(cr, true);

  return;
}

void el_remove_cr(Compound_region* cr)
{
  assert(!cr->is_procedure());

  Compound_region* parent=cr->parent();
 
  //Disconnect cr from its parent
  parent->remove_region(cr);
  
  //Reconnect cr's parent to its children
  for(Region_subregions sbr(cr);sbr!=0;sbr++)
    {
      Region* sr=*sbr;
      parent->add_region(sr);
      sr->set_parent(parent);
    }
  
  delete cr;

  return;
}


Compound_region* el_mark_cr_on_cfg(Procedure* proc,
				   List_set<Basicblock*>& region_bbs,
				   Hash_set<Edge*>& top_entries)
{
  Compound_region* new_cr=new Compound_region;
  bool first_time=true;

  //the top entries are always region entries (though their sources
  //may be in region_bbs)... this accounts for backedges that span
  //the region, but are not in the region
  for(Hash_set_iterator<Edge*> e_iter(top_entries); e_iter!=0; e_iter++)
    {
      Edge* edge=*e_iter;
      Op* dest_op=edge->dest();
      new_cr->add_entry_safely(edge);
      new_cr->add_entry(dest_op);
    }
  
  //The beginning of a function is also an entry op
  Basicblock* proc_entry_bb=el_get_proc_entry_bb(proc);
  if(region_bbs.is_member(proc_entry_bb))
    {
      Op* entry_op=el_get_bb_entry_op(proc_entry_bb);
      new_cr->add_entry(entry_op);
    }



  //iterate over region bbs setting:
  //1)The entry ops and edges of the new compound region (cr)
  //2)The exit ops and edges of the new compound region (cr)
  //3)The parent field (if the bb has an hb parent, we want to set 
  //  the parent field of the hb instead) of bbs and their appropriate
  //  ancestors.
  for(List_set_iterator<Basicblock*> bb_iter(region_bbs); bb_iter!=0; bb_iter++)
    {
      Basicblock* bb=*bb_iter;
      Compound_region* child=bb;//child of cr
      Compound_region* parent=bb->parent();//parent of cr
      
      //add to the entry ops and edges of cr
      for(Region_entry_edges entry_iter(bb);
	  entry_iter!=0; 
	  entry_iter++)
	{
	  Edge *edge=*entry_iter;
	  Op* src_op=edge->src();
	  Op* dest_op=edge->dest();
	  Basicblock* src_bb=(Basicblock*)src_op->parent();
	  assert(src_bb->is_bb());

	  //an entry-edge of a bb in region_bbs whose source
	  //bb is not in region_bbs is an entry of cr
	  if(!region_bbs.is_member(src_bb))
	    {
	      new_cr->add_entry(dest_op);
	      new_cr->add_entry_safely(edge);	      
	    }
	}

      //add to the exit ops and edges of cr
      for(Region_exit_edges exit_iter(bb);
	  exit_iter!=0; 
	  exit_iter++)
	{
	  Edge *edge=*exit_iter;
	  Op* src_op=edge->src();
	  Op* dest_op=edge->dest();
	  Basicblock* dest_bb=(Basicblock*)dest_op->parent();
	  
	  //an exit-edge of a bb in region_bbs whose dest
	  //bb is not in region_bbs is an exit of cr
	  if((!region_bbs.is_member(dest_bb))||
	     top_entries.is_member(edge))
	    {
	      new_cr->add_exit(src_op);
	      new_cr->add_exit_safely(edge);	      
	    }
	}
      
      
      if (parent->is_hb())
	{
	  child=parent;
	  parent=parent->parent();
	}
     
      if(first_time)
	{
	  first_time=false;
	  parent->insert_before_region(new_cr,child);
	  new_cr->set_parent(parent);
	}

      parent->remove_region(child);

     
      child->set_parent(new_cr);
      new_cr->add_region(child);
      
    }
  
  return new_cr;
}

#define TD_MAX_REGION_ENTRIES 1000
//get all entry edges of a region in a hash set
Hash_set<Edge*>* compound_region_entry_edges(Compound_region* cr)
{
  
  Hash_set<Edge*>* result = 
    new Hash_set<Edge*> (hash_edge_ptr,TD_MAX_REGION_ENTRIES);
  
  for (Region_entry_edges entry_iter(cr); entry_iter!=0; entry_iter++)
    {
      Edge* edge=*entry_iter;
      *result += edge;
    }
  return result;
}


//Assumes all branches are normalized and that a basic block
//identification routine has been run (see el_normalize_branches.cpp[h])
void el_tail_duplicate(Compound_region* cr,
		       Hash_set<Edge*>& side_entries,
		       Hash_set<Edge*>& top_entries,
		       List_set<Basicblock*>& new_tail_bbs,
		       Hash_map<Basicblock*,Basicblock*>& new_to_old_tail_bb_map)
{
  Hash_set<Basicblock*> tail(hash_bb_ptr, 16);

  //cout << "Reminder: tail duplication assumes branches are normalized\n";

  //identify the tail of cr (BBs downstream of side_entries)
  //Find the bblocks at the entry to the region  
  List_set<Basicblock*> top_entry_bbs;
  for(Hash_set_iterator<Edge*> in_edge(top_entries); in_edge!=0; in_edge++)
    {
      Edge* edge = *in_edge;
      Basicblock* entry_bb = (Basicblock*)edge->dest()->parent();
      assert(entry_bb->is_bb());
      top_entry_bbs += entry_bb;
    }

  for(Hash_set_iterator<Edge*> in_edge1(side_entries); in_edge1!=0; in_edge1++)
    {
      Edge* edge = *in_edge1;
      
      el_add_downstream_bblocks_in_region(edge,cr,tail,top_entry_bbs);
    }

  
  //Hash_map<Basicblock*,Basicblock*> tail_copy(hash_bb_ptr,MAX_BBS_IN_TAIL);
  Region_map tail_copy;
  Edge_map map_for_edges;


  Compound_region* parent_region=(Compound_region*)el_get_enclosing_procedure(cr);
  

  //duplicate the tail set and insert duplicates into a 
  //hash table keyed by the originals (given bblock b,
  //we can find the copied bblock b' easily)
  for(Hash_set_iterator<Basicblock*> tail_bb(tail);tail_bb!=0;tail_bb++)
    {
      Basicblock* bb=*tail_bb;

      //copy the bblock
      Basicblock* new_bb=(Basicblock*)copy_with_specified_edges(bb,tail_copy, map_for_edges,
						ALL_EDGES,ALL_EDGES,ALL_EDGES);

      //Remove all entry edges into the bb copy to prevent duplication
      //I'd prefer to do copy_with_specified_edges(...,NO_EDGES,ALL_EDGES,ALL_EDGES);

      // Change for-loop to while-loop because we are deleting entry edges while
      // iterating over them; the for-loop can coredump. -KF 12/2004
      Region_entry_edges e_iter(new_bb);
      while (e_iter != 0)
        {
          Edge* edge=*e_iter;
          ++e_iter;
          Op *dest_op=edge->dest();	  
          dest_op->remove_inedge_recursively(edge);
        }

      for (Map_iterator<Region*, Region*>  miter(tail_copy);  
	   miter != 0; miter++){
	Region* temp_new;
	
	temp_new = (*miter).second;
	temp_new->renumber();
      }

      //For now, the duplicated code has the procedure as its parent
      parent_region->add_region(new_bb);
      new_bb->set_parent(parent_region);
      el_clear_brdir_attrs(new_bb);

      tail_copy.bind(bb,new_bb);
      new_to_old_tail_bb_map.bind(new_bb,bb);
    }
  
  //hook up the duplicated tail set
  //iterate through the tail set, and
  //for each bblock bb in tail...
  for(Hash_set_iterator<Basicblock*> tail_bb1(tail);tail_bb1!=0;tail_bb1++)
    {
      Basicblock* bb=*tail_bb1;
      
      //we want to keep around the duplicated tail blocks in the result set
      //new_tail_bbs
      new_tail_bbs+=(Basicblock*)tail_copy.value(bb);

      //(deal with entry edges)
      //...for predecessor pred_bb of bb

      // Change for-loop to while-loop because we are deleting entry edges while
      // iterating over them; the for-loop can coredump. -KF 12/2004
      Region_entry_edges entry_iter(bb);
      while (entry_iter != 0)
	{
          Edge* edge=*entry_iter;
          ++entry_iter;
          Basicblock* pred_bb=(Basicblock*)edge->src()->parent();
	  
	  //...if pred_bb is not in cr, the region being duplicated
	  if(!el_region_contains_bblock(cr,pred_bb))
	    {
	      //...re-connect the pred_bb->bb edge  to bb' 
	      el_reconnect_edge_dest(edge,(Basicblock*)tail_copy.value(bb));
	    }
	}

      //(deal with exit edges)
      //...for each successor (succ_bb) of bb
      for(Region_exit_edges exit_iter(bb);
	  exit_iter!=0; 
	  exit_iter++)
	{
	  Edge *edge=*exit_iter;
	  Basicblock* new_bb=(Basicblock*)tail_copy.value(bb);
	  Edge *new_edge=map_for_edges.value(edge);
	  Basicblock* succ_bb=(Basicblock*)edge->dest()->parent();
	  assert(succ_bb->is_bb());
	  
	  //...if succ_bb is in tail
	  if(tail.is_member(succ_bb))
	    //...connect bb'->succ_bb'
	    el_connect_edge(new_bb,(Basicblock*)tail_copy.value(succ_bb),new_edge);	  
	  else
	    //...else connect bb'->succ_bb
	    el_connect_edge(new_bb,succ_bb,new_edge);
	}
      
    }

  //fix the weights of the duplicated tail
  Dlist<Basicblock*> topo_sorted_bbs;
  el_topo_sort_bbs(tail_copy,topo_sorted_bbs,top_entry_bbs);
  el_fix_weights(topo_sorted_bbs,side_entries,tail_copy,map_for_edges);

  return;
} 

//Some things shouldn't be copied
void el_clear_brdir_attrs(Basicblock* new_bb)
{
  int count=0;

  //How do the various fields of the branch change during a copy?
  //cdbg << "reminder: Semantics of copy needs to be clarified..\n";


  for(Region_exit_ops eo(new_bb);eo!=0;eo++,count++)
    {
      Op* br=*eo;
      Opcode br_opcode=br->opcode();
      Branch_info_attr* binfo = get_branch_info_attr(br);
      switch(br_opcode){
      case BRDIR:
	{
	  //cdbg << "reminder: need to consult region map to find this pbr_op in bb copy\n";
	  binfo->pbr_op=0;
	  binfo->pbr_id=-1;
	  binfo->cmpp_op=0;
	  binfo->cmpp_id=-1;
	  binfo->ld_op=0;
	  binfo->ld_id=-1;
	  binfo->addr_calc_op=0;
	  binfo->addr_calc_id=-1;
	  binfo->cmpp_tgt=0;
	  binfo->pbr_tgt=0;
	  binfo->ld_tgt=0;

	}
	break;
      case BRIND:
	assert(0);//no indirect calls for now
	break;
	
	//subroutine calls and return statements are not changed
      case BRL:
      case RTS:
	break;
	
      default:
	;
      }
    }
  return;
}

//Find any basic blocks in the compound region "cr" that are
//control dependent on "edge" into the result set "bbs"
void el_add_downstream_bblocks_in_region(Edge* edge,
						Compound_region* cr,
						Hash_set<Basicblock*> &bbs,
						List_set<Basicblock*> &top_entry_bbs)
{
  Basicblock* dest_bb=(Basicblock*)(edge->dest()->parent());
  Hash_set<Basicblock*> to_do(hash_bb_ptr, 128);
  Basicblock* todo_bb;
  
  assert(dest_bb->is_bb());
  to_do+=dest_bb;
  
  //plug away till the todo list is empty
  while(!to_do.is_empty())
    {
      todo_bb=to_do.pop();
      bbs+= todo_bb;
      
      //place all successor BBs on the todo list if they are in cr
      for (Region_exit_edges exit_edge(todo_bb);exit_edge!=0;exit_edge++)
	{
	  Edge* ex_edge=*exit_edge;
	  //ensure that the bblock exit edge is not a compound region exit
	  if(!cr->is_exit_edge(ex_edge))
	    {
	      Basicblock* succ_bb=(Basicblock*)(ex_edge->dest()->parent());
	      assert(succ_bb->is_bb());	      	      	    
	      if(!top_entry_bbs.is_member(succ_bb))
		to_do+=succ_bb;
	    }
	}
    }
  
  return;
}

//connect src_bb to dest_bb
void el_connect_edge_clone(Basicblock* src,Basicblock* dest,Edge* edge)
{
  Edge* new_edge=edge->clone();
  Op* exit_op=el_get_bb_exit_op(src);
  Op* entry_op=el_get_bb_entry_op(dest);

  //fix new edge source
  new_edge->set_src(exit_op,edge->src_port(), edge->src_alt());
  exit_op->add_outedge_recursively(new_edge);

  //fix new edge dest
  new_edge->set_dest(entry_op,edge->dest_port(), edge->dest_alt());
  entry_op->add_inedge_recursively(new_edge);

  return;
}

void el_connect_edge(Basicblock* src,Basicblock* dest,Edge* edge)
{
  Op* exit_op=el_get_bb_exit_op(src);
  Op* entry_op=el_get_bb_entry_op(dest);
  
  //fix new edge source
  edge->set_src(exit_op,edge->src_port(), edge->src_alt());
  exit_op->add_outedge_recursively(edge);

  //fix new edge dest
  edge->set_dest(entry_op,edge->dest_port(), edge->dest_alt());
  entry_op->add_inedge_recursively(edge);
  
  return;
}



bool el_region_contains_bblock(Compound_region* cr,Basicblock* pred_bb)
{
  Region* parent=pred_bb;
  
  do
    {
      if (parent==cr)
	return true;
      
      parent=parent->parent();
    }
  while(parent != NULL);
  
  return false;
}



Edge* region_copy_edge_clone_and_bind_if_needed(Edge* original, Edge_map& edge_map)
{ 
  if(!edge_map.is_bound(original))
    {
      Edge* new_edge = original->clone();
      edge_map.bind(original, new_edge);
      delete new_edge->attributes ;
      new_edge->attributes = new Graph_attribute(*(original->attributes));
      return(new_edge);
    }
  else
    return edge_map.value(original);
}
 
//similar to the function in region.cpp, except that if the
//out, in and internal edges are already in the map, an
//additional clone of these edges is not added into the map
Region* copy_with_specified_edges(Region* original_reg,
				  Region_map& region_map, 
				  Edge_map& edge_map,
				  const Edge_filter* in_filter,
				  const Edge_filter* out_filter,
				  const Edge_filter* internal_filter)
{
  Region* region = original_reg;
  //Clone the required edges and put them in the map
  for(Region_all_inedges in(region, in_filter); in != 0; in++)
    ::region_copy_edge_clone_and_bind_if_needed(*in, edge_map);
  for(Region_all_outedges out(region, out_filter); out != 0; out++)
    ::region_copy_edge_clone_and_bind_if_needed(*out, edge_map);
  for(Region_all_internal_edges internal(region, internal_filter); 
              internal != 0; internal++)
    ::region_copy_edge_clone_and_bind_if_needed(*internal, edge_map);

  // Recursively clone the regions
  Region* new_region = region->copy_with_edges(region_map, edge_map);
  new_region->set_parent(NULL);
  return(new_region);
}

void el_topo_sort_bbs(Region_map& cr, Dlist<Basicblock*>& result, List_set<Basicblock*>& top_entry_bbs)
{
  Hash_set<Basicblock*> visited_bbs(hash_bb_ptr, 128);
  Dlist<Basicblock*> ni_bbs; //No-Inedge bbs
  
  //find all bbs in cr with  no inedges within cr
  for(Region_map_iterator miter(cr);miter!=0;miter++)
    {
      Basicblock* bb=(Basicblock*)(*miter).first;
      bool is_ni=true;
      
      //The region map contains ops too, not just bbs
      if(bb->is_bb())
	{
	  //Iterate through predecessors, checking if all are outside the cr
	  for(Region_entry_edges e_edge(bb);e_edge!=0;e_edge++)
	    {
	      Edge* edge=*e_edge;
	      if(cr.is_bound(edge->src()->parent()))
		{
		  is_ni=false;
		  break;
		}
	    }
	  if(is_ni)ni_bbs.push(bb);      
	}
    }

  //loop till no more no-inedge nodes are left
  while(!ni_bbs.is_empty())
    {
      Basicblock* bb=ni_bbs.pop();
      result.push_tail(bb);
      visited_bbs+=bb;

      //check if each successor of bb can be added to ni_bbs
      for(Region_exit_edges e_edge(bb);e_edge!=0;e_edge++)
	{
	  Edge* edge=*e_edge;
	  Basicblock* succ=(Basicblock*)edge->dest()->parent();
	  
	  if((!cr.is_bound(succ))||
	     (top_entry_bbs.is_member(succ)))
	    continue;
	  
	  bool all_preds_visited=true;

	  //check if every predecessor of succ is already visited
	  for(Region_entry_edges e_edge(succ);e_edge!=0;e_edge++)
	    {
	      Edge* edge=*e_edge;
	      Basicblock* pred=(Basicblock*)edge->src()->parent();
	      
	      //a predecessor that is in cr _and_ not yet
	      //visited means that succ is no longer ready
	      //to be inducted into the topo order
	      if((!visited_bbs.is_member(pred))&&
		 (cr.is_bound(pred)))
		{
		  all_preds_visited=false;
		  break;
		}
	    }
	  if(all_preds_visited)ni_bbs.push_tail(succ);
	}
    }
  
  return;
}

void el_fix_weights( Dlist<Basicblock*> topo_sorted_bbs,Hash_set<Edge*>& side_entries, 
		     Region_map& bb_map,Edge_map& edge_map)
{
  
  //Iterate through the blocks of the tail in topological order
  for(Dlist_iterator<Basicblock*> bb_iter(topo_sorted_bbs);bb_iter!=0;bb_iter++)
    {
      Basicblock* bb=*bb_iter;
      assert(bb_map.is_bound(bb));
      Basicblock* copy_bb=(Basicblock*)bb_map.value(bb);
      double copy_edge_wt=0.0,orig_edge_wt=0.0,total_edge_wt;
      double orig_edge_ratio,copy_edge_ratio;

      //Figure out the relative shares of the copied bblock
      //and the original bblock, based on the weights of the
      //side edges into the original bblock; note that after
      //tail duplication, all side entries are disconnected
      //from the original bblock and reconnected to the 
      //copy; every entry of the  copy is a reconnected side
      //edge
      for(Region_entry_edges e_iter(bb);e_iter!=0;e_iter++)
	{
	  Edge *edge=*e_iter;
	  Control_flow_freq *cfreq = get_control_flow_freq(edge);
	  
	  assert(!side_entries.is_member(edge));
	  orig_edge_wt+=cfreq->freq;
	}
      
      for(Region_entry_edges e_iter2(copy_bb);e_iter2!=0;e_iter2++)
	{
	  Edge *edge=*e_iter2;
	  Control_flow_freq *cfreq = get_control_flow_freq(edge);
	  
	  copy_edge_wt+=cfreq->freq;
	}

      //Assume the original bb has its weight set right (equal to the
      //sum of the original incoming edges
      total_edge_wt=orig_edge_wt+copy_edge_wt;
      
      assert((bb->weight-total_edge_wt<.001)||
	     (bb->weight-total_edge_wt>-.001));
      if(total_edge_wt==0)
	{
	  orig_edge_ratio=0;
	  copy_edge_ratio=0;
	}
      else
	{
	  orig_edge_ratio=orig_edge_wt/total_edge_wt;
	  copy_edge_ratio=copy_edge_wt/total_edge_wt;
	}
      
      //fix exit edge weights 
      for(Region_exit_edges e_iter1(bb);e_iter1!=0;e_iter1++)
	{
	  Edge *edge=*e_iter1;
	  assert(edge_map.is_bound(edge));
	  Edge *edge_copy=edge_map.value(edge);
	  Control_flow_freq *cfreq = get_control_flow_freq(edge);
	  Control_flow_freq *cfreq_copy = get_control_flow_freq(edge_copy);
	  double orig_weight=cfreq->freq;
	  
	  cfreq->freq= orig_weight*orig_edge_ratio;
	  cfreq_copy->freq= orig_weight*copy_edge_ratio;	  
	}      
    }

  //Fix the weights of the regions (given that the edges into/
  //out of them have the right values)
  for(Region_map_iterator bb_iter1(bb_map);bb_iter1!=0;bb_iter1++)
    {
      Compound_region* reg=(Compound_region*)(*bb_iter1).first;
      Hash_set<Compound_region*> done_regs(hash_cmpd_region_ptr, 128);
      
      if(!reg->is_bb())continue;

      for(;((reg!=NULL)&&(!reg->is_procedure()));reg=reg->parent())
	{
	  if(!done_regs.is_member(reg))
	    {
	      reg->weight=0;
	      done_regs+=reg;
	      for(Region_entry_edges e_iter(reg);e_iter!=0;e_iter++)
		{
		  Edge* edge=*e_iter;
		  Control_flow_freq *cfreq = get_control_flow_freq(edge);
		  reg->weight+=cfreq->freq;
		}
	    }
	  
	}
      

      Compound_region* reg_copy=(Compound_region*)(*bb_iter1).second;
      for(;((reg_copy!=NULL) && (!reg_copy->is_procedure()));reg_copy=reg_copy->parent())
	{
	  if(!done_regs.is_member(reg_copy))
	    {
	      reg_copy->weight=0;
	      done_regs+=reg_copy;
	      for(Region_entry_edges e_iter(reg_copy);e_iter!=0;e_iter++)
		{
		  Edge* edge=*e_iter;
		  Control_flow_freq *cfreq = get_control_flow_freq(edge);
		  reg_copy->weight+=cfreq->freq;
		}
	    }
	  
	}
    }

  return;
}

