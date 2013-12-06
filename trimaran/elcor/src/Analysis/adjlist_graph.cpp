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




///////////////////////////////////////////////////////////////////////////
//
//      File:        adjlist_graph.cpp
//      Authors:     Sadun Anik, David August
//      Created:     March, 1995   
//      Description: Graph implemented as adjacency list. Used to build 
//                   abstract control-flow graphs for analysis.
//
///////////////////////////////////////////////////////////////////////////

#include "adjlist_graph.h"
#include "iterators.h"
#include "control_analysis_solver.h"
#include "hash_set.h"
#include "hash_functions.h"
#include "op.h"
#include "edge.h"
#include "el_dot_tools.h"
#include "opcode_properties.h"


////////////////////////////////////////////////////////////
//  Class Alist_graph definition
////////////////////////////////////////////////////////////

Alist_graph::Alist_graph()
{}

Alist_graph::~Alist_graph()
{}

void Alist_graph::eliminate_edges(Bitvector& mask)
{
   int new_edge_count ;
   
   int i ;
   Vector<int> old_to_new_map(edge_count) ; // Map from old names to new names

   new_edge_count = 0 ;
   for (i = 0 ; i < edge_count ; i++) {
      if (mask.bit(i)) {
	 old_to_new_map[i] = new_edge_count ;
	 new_edge_count++ ;
      }
      else {
	 old_to_new_map[i] = ELCOR_MIN_INT ;
      }
   }

   // First set the entry_edge_map and exit_edge_map and rename
   // edge ids in f_edge_map
   entry_edge_map.resize(node_count) ; // clear the Vector
   exit_edge_map.resize(node_count) ;  // clear the Vector
   for (i = 0 ; i < edge_count ; i++) {
      if (mask.bit(i)) {
	 int src_nodei = edge_src_node_map[i] ;
	 int dest_nodei = edge_dest_node_map[i] ;
	 int new_edge_name = old_to_new_map[i] ;
	 entry_edge_map[dest_nodei].add_tail(new_edge_name) ;
	 exit_edge_map[src_nodei].add_tail(new_edge_name) ;
      }
   }
   shrink_vector(edge_src_node_map, mask) ;
   shrink_vector(edge_dest_node_map, mask) ;
   edge_count = new_edge_count ;
   
   // We can do a consistency check here
   //
   // check if map is coherent, edge node linkage is correct
   //
   for (i = 0 ; i < edge_count ; i++) {
      assert(entry_edge_map[edge_dest_node_map[i]].is_member(i)) ;
      assert(exit_edge_map[edge_src_node_map[i]].is_member(i)) ;
   }
}


////////////////////////////////////////////////////////////////////
//
// Alist_control_graph   A graph for control flow analysis
//
////////////////////////////////////////////////////////////////////

Alist_control_graph::Alist_control_graph(Region* r)
{
   int hash_set_size = 1007 ;
   // Construct a list of regions and their successors
   Hash_set<Region*> region_set(hash_region_ptr, hash_set_size) ;
   Dlist<Region*> rstack ;
   rstack.push(r) ;
   while (!rstack.is_empty()) {
      Region* tmpr = rstack.pop() ;
      if (tmpr->is_bb()) {
         region_set += tmpr ;
         
      }
      else {
         for (Region_subregions ri(tmpr) ; ri != 0 ; ri++) {
            rstack.push(*ri) ;
         }
      }
   }
   Hash_set<Region*> region_entry_set(hash_region_ptr,7) ;
   Hash_set<Region*> region_exit_set(hash_region_ptr,7) ;
   Hash_set<Region*> region_orig_exit_set(hash_region_ptr,7) ;
   
   for (Region_entry_ops rent(r) ; rent != 0 ; rent++) {
      Region* tmpr = *rent ;
      Region* largest_parent = 0 ;
      while (tmpr) {
	 if (region_set.is_member(tmpr)) {
	    largest_parent = tmpr ;
	 }
	 tmpr = tmpr->parent() ;
      }
      assert(largest_parent) ;
      region_entry_set += largest_parent ;
   }
   for (Region_exit_ops rex(r) ; rex != 0 ; rex++) {
      Region* tmpr = *rex ;
      Region* largest_parent = 0 ;
      while (tmpr) {
	 if (region_set.is_member(tmpr)) {
	    largest_parent = tmpr ;
	 }
	 tmpr = tmpr->parent() ;
      }
      assert(largest_parent) ;
      region_exit_set += largest_parent ;
   }
   region_orig_exit_set = region_exit_set;
   region_exit_set -= region_entry_set ;
   region_set -= region_entry_set ;
   region_set -= region_exit_set ;

   Hash_set_iterator<Region*> rsit ;
   List<Region*> region_list ;
   
   region_list.add_tail(symbolic_bb_top);
   for (rsit(region_entry_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   for (rsit(region_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   for (rsit(region_exit_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   region_list.add_tail(symbolic_bb_bottom);
   
   region_set += region_entry_set ;
   region_set += region_exit_set ;
   
   // Assign dense int's to regions
   node_count = region_list.size() ;
   
   b_map.resize(node_count) ;
   succ_list.resize(node_count) ;
   pred_list.resize(node_count) ;
   
   int counter = 0 ;
   for (List_iterator<Region*> si2(region_list) ; si2 != 0 ; si2++) {
      f_map.bind(*si2,counter) ;
      b_map[counter] = *si2 ;
      counter++ ;
   }
   for (int nodei = 0 ; nodei < node_count ; nodei++)
   {
      Region* tmp_reg = b_map[nodei] ;
      int r_num = f_map.value(tmp_reg) ;
      for (Region_exit_edges rex(tmp_reg) ; rex != 0 ; rex++)
      {
         Region* tr = (*rex)->dest() ;
         assert(tr) ;
         while (!region_set.is_member(tr))
         {
            tr = tr->parent() ;
            if (tr == 0)
            {
               if (region_contains_edge(r,*rex)) {
                  El_punt("ERROR: The region hierarchy does not contain a tiling of the requested type") ;
               }
               else {
                  break ;
               }
            }
         }
         if (tr == 0) continue ;
         int node_number = f_map.value(tr) ;
         if (!succ_list[r_num].is_member(node_number))
         {
            succ_list[r_num].add_tail(node_number) ;
         }
         if (!pred_list[node_number].is_member(r_num))
         {
            pred_list[node_number].add_tail(r_num) ;
         }
      }
   }
   // Add top and bottom
   top = f_map.value(symbolic_bb_top);
   bottom = f_map.value(symbolic_bb_bottom);
   for (rsit(region_entry_set) ; rsit != 0 ; rsit++)
   {
      int r_num = f_map.value(*rsit) ;
      succ_list[top].push(r_num) ;
      pred_list[r_num].push(top) ;
   }
   for (rsit(region_orig_exit_set) ; rsit != 0 ; rsit++)
   {
      int r_num = f_map.value(*rsit) ;
      pred_list[bottom].push(r_num) ;
      succ_list[r_num].push(bottom) ;
   }
   succ_list[top].push(bottom) ;
   pred_list[bottom].push(top) ;
   
   node_count = succ_list.size() ;
   included.resize(b_map.size(), true) ;  // include all nodes 
   
}

void Alist_control_graph::constrict_graph(Bitvector include_list, int head, bool redirect)
{
   orig_succ_list = succ_list;
   orig_pred_list = pred_list;
   orig_included = included;
   included = include_list;
  
   /*
   ** boolean Redirect only affects successors list
   */
 
   Bit_iterator bi;
   for(bi(included) ; bi != 0 ; bi++) {
      int cur_node = *(bi);

      /*
      ** First do predicessor list
      */
      List<int> new_pred_list;
      bool add_top = false;
      for(List_iterator<int> pred_iter(pred_list[cur_node]); pred_iter != 0; pred_iter++)
      {
	 int cur_pred = *pred_iter;
	 if(include_list.bit(cur_pred) == true)
	 {
	    new_pred_list.add_tail(cur_pred);
	 }
	 else
	 {
	    add_top = true;
	 }
      }
      if(add_top)
      {
	 add_top = false;
	 new_pred_list.add_tail(top);
	 succ_list[top].add_tail(cur_node);
      }
      pred_list[cur_node] = new_pred_list;
   }

   for(bi(included) ; bi != 0 ; bi++) {
      int cur_node = *(bi);

      /*
      ** Now do successor list
      */
      List<int> new_succ_list;
      bool add_bottom = false;
      for(List_iterator<int> succ_iter(succ_list[cur_node]); succ_iter != 0; succ_iter++)
      {
	 int cur_succ = *succ_iter;
	 if(cur_succ == head && cur_node != top)
	 { 
	    pred_list[head].remove(cur_node);
	    add_bottom = true;
	 }
	 else
	 {
	    if(include_list.bit(cur_succ) == true)
	    {
	       if ((redirect) || (cur_node == top) || (cur_succ != bottom))
		  new_succ_list.add_tail(cur_succ);
	    }
	    else
	    {
	       if(redirect && cur_node != top)
	       {
		  add_bottom = true;
	       }
	    }
	 }
      }
      if(add_bottom || (new_succ_list.is_empty() && cur_node != bottom && !redirect))
      {
	 add_bottom = false;
	 new_succ_list.add_tail(bottom);
	 pred_list[bottom].add_tail(cur_node);
      }
      if(new_succ_list.is_empty() && cur_node != bottom)
      {
	 El_punt("Alist_graph::Constrict_graph: Broken Graph.");
      }
      succ_list[cur_node] = new_succ_list;
   }
   node_count = succ_list.size() ;
}


void Alist_control_graph::restore_graph()
{
   succ_list = orig_succ_list;
   pred_list = orig_pred_list;
   node_count = succ_list.size() ;
   included = orig_included;
}

////////////////////////////////////////////////////////////////////
//
// Alist_region_hierarchy_graph   Tree of region hierarchy
//
///////////////////////////////////////////////////////////////////

Alist_region_hierarchy_graph::Alist_region_hierarchy_graph(Region* r)
   : f_map(hash_region_ptr, 1007)
{
   Dlist<Region*> rstack ;
   List<Region*> non_leaf_region_list ;
   List<Region*> leaf_region_list ;
   
   List_iterator<Region*> si ;

   rstack.push(r) ;
   non_leaf_region_list.add_tail(r) ;
   while (!rstack.is_empty()) {
      Region* tmpr = rstack.pop() ;
      Region_subregions ri(tmpr) ;
      if (ri != 0) { // Region has subregions
         for (; ri != 0 ; ri++) {
            rstack.push_tail(*ri) ;
	    non_leaf_region_list.add_tail(*ri) ;
         }
      }
      else {
	 leaf_region_list.add_tail(tmpr) ;
      }
   }
   // Assign dense int's to regions
   node_count = non_leaf_region_list.size() + leaf_region_list.size() ;
   
   b_map.resize(node_count) ;
   succ_list.resize(node_count) ;
   pred_list.resize(node_count) ;
   
   int counter = 0 ;
   for (si( non_leaf_region_list) ; si != 0 ; si++) {
      f_map.bind(*si,counter) ;
      b_map[counter] = *si ;
      counter++ ;
   }
   for (si(leaf_region_list) ; si != 0 ; si++) {
      f_map.bind(*si,counter) ;
      b_map[counter] = *si ;
      counter++ ;
   }
   // Find the adjacency list
   for (int nodei = 0 ; nodei < node_count ; nodei++)
   {
      Region* tmp_reg = b_map[nodei] ;
      if (tmp_reg != r) { // Root's parent is not in this tree (if it exists)
	                  // Don't process root
	 Region* parent_reg = tmp_reg->parent() ;
	 int parent_nodei = f_map.value(parent_reg) ;
	 // The two assertions below catch bugs in region hierarchy
	 // or assigning ints to nodes. 
	 assert (!succ_list[parent_nodei].is_member(nodei)) ;
	 succ_list[parent_nodei].add_tail(nodei) ;
	 assert (!pred_list[nodei].is_member(parent_nodei)) ;
	 pred_list[nodei].add_tail(parent_nodei) ;
      }
   }
}

////////////////////////////////////////////////////////////////////
//
// Alist_dfg_graph   Dataflow graph
//
///////////////////////////////////////////////////////////////////

Alist_dfg_graph::Alist_dfg_graph(Region* r)
   : f_map(hash_region_ptr, 1007), f_edge_map(hash_edge_ptr, 1007)
{
   int hash_set_size = 1007 ;
   // Construct a list of regions and their successors
   Hash_set<Region*> region_set(hash_region_ptr, hash_set_size) ;
   edge_count = 0 ;
   for(Region_all_ops ri(r) ; ri != 0 ; ri++) {
      Op* cur_op = *ri ;
      if ((!is_switch(cur_op)) &&(!is_merge(cur_op))) {
         region_set += (cur_op) ;
      }
   }
   // Sort the regions into graph_entry_regions + intermediate_regions +
   // graph_exit_regions
   Hash_set<Region*> region_entry_set(hash_region_ptr,7) ;
   Hash_set<Region*> region_exit_set(hash_region_ptr,7) ;
   Hash_set<Region*> region_orig_exit_set(hash_region_ptr,7) ;
   
   for (Region_entry_ops rent(r) ; rent != 0 ; rent++) {
      Region* tmpr = *rent ;
      Region* largest_parent = 0 ;
      while (tmpr) {
         if (region_set.is_member(tmpr)) {
            largest_parent = tmpr ;
         }
         tmpr = tmpr->parent() ;
      }
      assert(largest_parent) ;
      region_entry_set += largest_parent ;
   }
   for (Region_exit_ops rex(r) ; rex != 0 ; rex++) {
      Region* tmpr = *rex ;
      Region* largest_parent = 0 ;
      while (tmpr) {
         if (region_set.is_member(tmpr)) {
            largest_parent = tmpr ;
         }
         tmpr = tmpr->parent() ;
      }
      assert(largest_parent) ;
      region_exit_set += largest_parent ;
   }
   region_orig_exit_set = region_exit_set;
   region_exit_set -= region_entry_set ;
   region_set -= region_entry_set ;
   region_set -= region_exit_set ;
   
   Hash_set_iterator<Region*> rsit ;
   List<Region*> region_list ;
   for (rsit(region_entry_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   for (rsit(region_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   for (rsit(region_exit_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   region_set += region_entry_set ;
   region_set += region_exit_set ;
   
   // Assign dense int's to regions
   node_count = region_list.size() ;
   
   b_map.resize(node_count) ;
   succ_list.resize(node_count) ;
   pred_list.resize(node_count) ;
   entry_edge_map.resize(node_count) ;
   exit_edge_map.resize(node_count) ;
   int counter = 0 ;
   for (List_iterator<Region*> si2(region_list) ; si2 != 0 ; si2++) {
      f_map.bind(*si2,counter) ;
      b_map[counter] = *si2 ;
      counter++ ;
   }
   
   // Find the adjacency list
   // For the special HB case (dataflow analysis treatment of HB's,
   //     make HB node point to the HB exit ops and HB exit ops to
   //     the true successors of HB
   //
   for (int nodei = 0 ; nodei < node_count ; nodei++)
   {
      Region* tmp_reg = b_map[nodei] ;
      assert (tmp_reg->is_op()) ;
      Op* tmp_src_op = (Op *)tmp_reg ;
      for (Op_outedges_rel di(tmp_src_op) ; di!= 0 ; di++) {
         Edge* cur_edge = *di ;
         if (!cur_edge->is_seq()) {
            Op* tmp_init_dest_op = cur_edge->dest() ;
            Op* tmp_dest_op = real_op(tmp_init_dest_op) ;
            if (f_map.is_bound(tmp_dest_op)) {
               //
               // A new edge: source is nodei, dest will be dest_nodei
               //             id will be edge_count
               // Add this edge to entry_edge_map  exit_edge_map and
               //     f_edge_map
               // After all the ops are processed, b_edge_map,
               //     edge_src_node_map and edge_dest_node_map will
               //     be assigned to
               //
               int dest_nodei = f_map.value(tmp_dest_op) ;
               if (!succ_list[nodei].is_member(dest_nodei))
               {
                  succ_list[nodei].add_tail(dest_nodei) ;
               }
               if (!pred_list[dest_nodei].is_member(nodei))
               {
                  pred_list[dest_nodei].add_tail(nodei) ;
               }
               f_edge_map.bind(cur_edge,edge_count) ;
               entry_edge_map[dest_nodei].add_tail(edge_count) ;
               exit_edge_map[nodei].add_tail(edge_count) ;
               edge_count++ ;
            }
         }
      }
   }
   // finish setting up the edge graph 
   
   b_edge_map.resize(edge_count) ;
   edge_src_node_map.resize(edge_count) ;
   edge_dest_node_map.resize(edge_count) ;
   for (Hash_map_iterator<Edge*, int> ei(f_edge_map) ; ei != 0 ; ei++) {
      b_edge_map[(*ei).second] = (*ei).first ;
   }
   
   List_iterator<int> li ; 
   for (int opi = 0 ; opi < node_count ; opi++) {
      for (li(entry_edge_map[opi]) ; li != 0 ; li++) {
         edge_dest_node_map[*li] = opi ;
      }
      for (li(exit_edge_map[opi]) ; li != 0 ; li++) {
         edge_src_node_map[*li] = opi ;
      }
   }
   
   node_count = succ_list.size() ;
}

void Alist_dfg_graph::eliminate_edges(Bitvector& mask)
{
   int new_edge_count ;
   
   int i ;
   Vector<int> old_to_new_map(edge_count) ; // Map from old names to new names

   new_edge_count = 0 ;
   for (i = 0 ; i < edge_count ; i++) {
      if (mask.bit(i)) {
	 old_to_new_map[i] = new_edge_count ;
	 new_edge_count++ ;
      }
      else {
	 old_to_new_map[i] = ELCOR_MIN_INT ;
      }
   }

   // First set the entry_edge_map and exit_edge_map and rename
   // edge ids in f_edge_map
   entry_edge_map.resize(node_count) ; // clear the Vector
   exit_edge_map.resize(node_count) ;  // clear the Vector
   for (i = 0 ; i < edge_count ; i++) {
      if (mask.bit(i)) {
	 int src_nodei = edge_src_node_map[i] ;
	 int dest_nodei = edge_dest_node_map[i] ;
	 int new_edge_name = old_to_new_map[i] ;
	 entry_edge_map[dest_nodei].add_tail(new_edge_name) ;
	 exit_edge_map[src_nodei].add_tail(new_edge_name) ;
	 // rename edge in f_edge_map
	 f_edge_map.bind(b_edge_map[i], new_edge_name) ;
      }
      else {
	 // remove edge from f_edge_map
	 f_edge_map.unbind(b_edge_map[i]) ; 
      }
   }
   shrink_vector(edge_src_node_map, mask) ;
   shrink_vector(edge_dest_node_map, mask) ;
   shrink_vector(b_edge_map, mask) ;
   edge_count = new_edge_count ;
   
   // We can do a consistency check here
   //
   // check if map is coherent, edge node linkage is correct
   //
   for (i = 0 ; i < edge_count ; i++) {
      assert(i == f_edge_map.value(b_edge_map[i])) ;
      assert(entry_edge_map[edge_dest_node_map[i]].is_member(i)) ;
      assert(exit_edge_map[edge_src_node_map[i]].is_member(i)) ;
   }
}
   
////////////////////////////////////////////////////////////////////
//
// Alist_region_tiling_graph   Graph of a tiling of region hierarchy
//
///////////////////////////////////////////////////////////////////

Alist_region_tiling_graph::Alist_region_tiling_graph() 
   :f_map(hash_region_ptr, 1007)
{}

Alist_region_tiling_graph::Alist_region_tiling_graph(Region* r, GRAPH_CATEGORY gc)
   :f_map(hash_region_ptr, 1007)
{
      int hash_set_size = 1007 ;
   if (gc == GRAPH_OPS) {
      int op_count = 0 ;
      for(Region_all_ops ri(r) ; ri != 0 ; ri++) op_count++ ;
      hash_set_size = op_count ;
   }
   // Construct a list of regions and their successors
   Hash_set<Region*> region_set(hash_region_ptr, hash_set_size) ;

   switch(gc) {
      case GRAPH_SUBREGIONS:
      {
	 for(Region_subregions ri(r) ; ri != 0 ; ri++) {
	    region_set += (*ri) ;
	 }
      }
      break ;
      case GRAPH_OPS:
      {
	 for(Region_all_ops ri(r) ; ri != 0 ; ri++) {
	    region_set += (*ri) ;
	 }
      }
      break ;
      case GRAPH_OPS_PLUS:
      {
	 for(Region_all_ops ri(r) ; ri != 0 ; ri++) {
	    region_set += ((*ri)->parent()) ;
	 }
      }
      break ;
      case GRAPH_BBS:
      case GRAPH_HBS:
      case GRAPH_HBS_SESE:      
      case GRAPH_HBS_BBS:
      case GRAPH_HBS_BBS_SESE:      
      {
	 Dlist<Region*> rstack ;
	 rstack.push(r) ;
	 while (!rstack.is_empty()) {
	    Region* tmpr = rstack.pop() ;
	    bool check_cond = false;
	    switch(gc) {
	       case GRAPH_BBS:
               check_cond = tmpr->is_bb() ;
	       break ;
	       case GRAPH_HBS:
	       case GRAPH_HBS_SESE:
	       check_cond = tmpr->is_hb() ;
	       break ;
	       case GRAPH_HBS_BBS:
	       case GRAPH_HBS_BBS_SESE:
	       check_cond = (tmpr->is_hb() || tmpr->is_bb());	     
	       break ;
	       default :
	       assert(0) ;
	       ;
	    }
	    if (check_cond) {
	       region_set += tmpr ;
	       if ((tmpr->is_hb()) && 
		   ((gc == GRAPH_HBS_SESE) || (gc == GRAPH_HBS_BBS_SESE))) {
		  for (Region_exit_ops rxop(tmpr) ; rxop != 0 ; rxop++) {
		     region_set += *rxop ;
		  }
	       }
	    }
	    else {
	       for (Region_subregions ri(tmpr) ; ri != 0 ; ri++) {
		  rstack.push(*ri) ;
	       }
	    }
	 }
      }
      break ;
      default:
      assert(0) ;
      ;
   }
   // Sort the regions into graph_entry_regions + intermediate_regions +
   // graph_exit_regions
   Hash_set<Region*> region_entry_set(hash_region_ptr,7) ;
   Hash_set<Region*> region_exit_set(hash_region_ptr,7) ;
   Hash_set<Region*> region_orig_exit_set(hash_region_ptr,7) ;
   
   for (Region_entry_ops rent(r) ; rent != 0 ; rent++) {
      Region* tmpr = *rent ;
      Region* largest_parent = 0 ;
      while (tmpr) {
	 if (region_set.is_member(tmpr)) {
	    largest_parent = tmpr ;
	 }
	 tmpr = tmpr->parent() ;
      }
      assert(largest_parent) ;
      region_entry_set += largest_parent ;
   }
   for (Region_exit_ops rex(r) ; rex != 0 ; rex++) {
      Region* tmpr = *rex ;
      Region* largest_parent = 0 ;
      while (tmpr) {
	 if (region_set.is_member(tmpr)) {
	    largest_parent = tmpr ;
	 }
	 tmpr = tmpr->parent() ;
      }
      assert(largest_parent) ;
      region_exit_set += largest_parent ;
   }
   region_orig_exit_set = region_exit_set;
   region_exit_set -= region_entry_set ;
   region_set -= region_entry_set ;
   region_set -= region_exit_set ;

   Hash_set_iterator<Region*> rsit ;
   List<Region*> region_list ;

   for (rsit(region_entry_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   for (rsit(region_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }
   for (rsit(region_exit_set) ; rsit != 0 ; rsit++) {
      region_list.add_tail(*rsit) ;
   }

   region_set += region_entry_set ;
   region_set += region_exit_set ;

   // Assign dense int's to regions
   node_count = region_list.size() ;
   
   b_map.resize(node_count) ;
   succ_list.resize(node_count) ;
   pred_list.resize(node_count) ;
   int counter = 0 ;
   for (List_iterator<Region*> si2(region_list) ; si2 != 0 ; si2++) {
      f_map.bind(*si2,counter) ;
      b_map[counter] = *si2 ;
      counter++ ;
   }

   // Find the adjacency list
   // For the special HB case (dataflow analysis treatment of HB's,
   //     make HB node point to the HB exit ops and HB exit ops to
   //     the true successors of HB
   //

   
   for (int nodei = 0 ; nodei < node_count ; nodei++)
   {
      Region* tmp_reg = b_map[nodei] ;
      int r_num = f_map.value(tmp_reg) ;
      if ((tmp_reg->is_hb() || tmp_reg->is_op()) &&
	  ((gc == GRAPH_HBS_SESE) || (gc == GRAPH_HBS_BBS_SESE)))
      {
	 if (tmp_reg->is_hb()) {
	    for (Region_exit_ops rxop(tmp_reg) ; rxop != 0 ; rxop++)
	    {
	       // hook up the HB node to the exit op nodes
	       int node_number =  f_map.value(*rxop) ;
	       if (!succ_list[r_num].is_member(node_number))
	       {
		  succ_list[r_num].add_tail(node_number) ;
	       }
	       if (!pred_list[node_number].is_member(r_num))
	       {
		  pred_list[node_number].add_tail(r_num);
	       }
	       // hook up the exit op node to its out-of-HB successor. 
               // The exit edge cannot leave the region itself
	       for (Op_outedges edgei(*rxop,CONTROL0_OUTEDGES) ;
		    edgei != 0 ; edgei++) {
		  if ((region_edge_crosses_boundary(tmp_reg, *edgei)) &&
                      (!region_edge_crosses_boundary(r, *edgei))) { 
		     Region* tr = (*edgei)->dest() ;
		     assert(tr) ;
		     while (!region_set.is_member(tr))
		     {
			tr = tr->parent() ;
			if (tr == 0)
			{
			   //
			   // Check if this edge leaves the top level region
			   //
			   if (region_contains_edge(r,*edgei)) {
			      El_punt("ERROR: The region hierarchy does not contain a tiling of the requested type") ;
			   }
			   else {
			      break ;
			   }
			}
		     }
		     if (tr == 0) continue ;
		     int node_number2 = f_map.value(tr) ;
		     if (!succ_list[node_number].is_member(node_number2))
		     {
			succ_list[node_number].add_tail(node_number2) ;
		     }
		     if (!pred_list[node_number2].is_member(node_number))
		     {
			pred_list[node_number2].add_tail(node_number) ;
		     }
		     
		  }
	       }
	    }
	 } // else skip the ops
      }
      else {
	 for (Region_exit_edges rex(tmp_reg) ; rex != 0 ; rex++)
	 {
            if (region_edge_crosses_boundary(r, *rex)) continue; // don't consider edges that leave the region
	    Region* tr = (*rex)->dest() ;
	    assert(tr) ;
	    while (!region_set.is_member(tr))
	    {
	       tr = tr->parent() ;
	       if (tr == 0)
	       {
		  if (region_contains_edge(r,*rex)) {
		     El_punt("ERROR: The region hierarchy does not contain a tiling of the requested type") ;
		  }
		  else {
		     break ;
		  }
	       }
	    }
	    if (tr == 0) continue ;
	    int node_number = f_map.value(tr) ;
	    if (!succ_list[r_num].is_member(node_number))
	    {
	       succ_list[r_num].add_tail(node_number) ;
	    }
	    if (!pred_list[node_number].is_member(r_num))
	    {
	       pred_list[node_number].add_tail(r_num) ;
	    }
	 }
      }
   }
   node_count = succ_list.size() ;
}

////////////////////////////////////////////////////////////////////
//
// Alist_edge_graph   A graph of edges (build upon Alist_graph)
//
///////////////////////////////////////////////////////////////////

Alist_edge_graph::Alist_edge_graph() {}

Alist_edge_graph::Alist_edge_graph(Alist_graph& ng)
{
   unsigned i ;
   List_iterator<int> li ;
   
   entry_edge_map.resize(ng.succ_list.size()) ;
   exit_edge_map.resize(ng.succ_list.size()) ;   

   int edge_counter = 0 ;
   for (Alist_graph_post_dfs gi(ng, UP) ; gi!= 0 ; gi++) {
      int node = *gi ;
      for (li(ng.succ_list[node]) ; li != 0 ; li++) {
	 exit_edge_map[node].add_tail(edge_counter) ;
	 entry_edge_map[*li].add_tail(edge_counter) ;
	 edge_counter++ ;
      }
   }
   src_node_map.resize(edge_counter) ;
   dest_node_map.resize(edge_counter) ;
   for (i = 0 ; i < entry_edge_map.size() ; i++) {
      for (li(exit_edge_map[i]) ; li != 0 ; li++) {
	 int edge_id = *li ;
	 src_node_map[edge_id] = i ;
      }
   }
   for (i = 0 ; i < entry_edge_map.size() ; i++) {
      for (li(entry_edge_map[i]) ; li != 0 ; li++) {
	 int edge_id = *li ;
	 dest_node_map[edge_id] = i ;
      }
   }
}

Alist_edge_graph::~Alist_edge_graph() {}


////////////////////////////////////////////////////////////////////
//
// Alist_graph_iterator   Abstract base class definition
//
///////////////////////////////////////////////////////////////////

Alist_graph_iterator::Alist_graph_iterator()
   : graph(0), size(0), iter_list(0), iter_index(0) 
{
}

Alist_graph_iterator::Alist_graph_iterator(Alist_graph& g)
   : graph(&g), iter_list(g.succ_list.size())
{
   size = iter_list.size() ;
   iter_index = 0 ;
}

Alist_graph_iterator::~Alist_graph_iterator() {} 

bool Alist_graph_iterator::operator==(const int i) const
{
   assert(i == 0) ;
   if (iter_index < size) return false ;
   else return true ;
}

bool Alist_graph_iterator::operator!=(const int i) const
{
   assert(i == 0) ;
   if (iter_index < size) return true ;
   else return false ;
   
}

void Alist_graph_iterator::operator++()
{
   iter_index++ ;
}

void Alist_graph_iterator::operator++(int)
{
   iter_index++ ;
}

int Alist_graph_iterator::operator*()
{
   return (iter_list[iter_index]) ;
}

void Alist_graph_iterator::reset()
{
   iter_index = 0 ;
}

////////////////////////////////////////////////////////////////////
//
// Alist_graph_dfs iterator
//
///////////////////////////////////////////////////////////////////

Alist_graph_dfs::Alist_graph_dfs()
{
}

Alist_graph_dfs::Alist_graph_dfs(Alist_graph& g, SEARCH_DIRECTION sd, int start_node, int stop_node)
   : Alist_graph_iterator(g), val(g.succ_list.size()) 
{
   if (sd == DOWN) adj_list = &(graph->succ_list) ;
   else adj_list = &(graph->pred_list) ;
   listdfs(start_node, stop_node) ;
}

Alist_graph_dfs::~Alist_graph_dfs()
{
}

void Alist_graph_dfs::listdfs(int start_node, int stop_node)
{
   int i ;
   id = 0 ;
   iter_index = 0 ;
   for (i=0 ; i<size ; i++) {
      val[i] = -1 ;
      iter_list[i] = -1 ;
   }
   if(start_node == -1)
   {
      for (i=0 ; i<size ; i++)
      {
	 if (val[i] == -1) visit(i, stop_node) ;
      }
   }
   else
   {
      visit(start_node, stop_node);
   }
   
   size = id;
   iter_index = 0 ;
}

void Alist_graph_dfs::visit(int k, int stop_node)
{
   iter_list[iter_index++] = k ;
   val[k] = id ;
   id++;
   if(k != stop_node)
   {
      for (List_iterator<int> li((*adj_list)[k]) ; li != 0 ; li++)
      {
	 if (val[*li] == -1) visit(*li, stop_node) ;
      }
   }
}
   
///////////////////////////////////////////////////////////////////
//
// class Alist_graph_post_dfs
//
///////////////////////////////////////////////////////////////////

Alist_graph_post_dfs::Alist_graph_post_dfs()
{
}

Alist_graph_post_dfs::Alist_graph_post_dfs(Alist_graph& g, SEARCH_DIRECTION sd, int start_node, int stop_node)
   : Alist_graph_iterator(g), val(g.succ_list.size()) 
{
   if (sd == DOWN) adj_list = &(graph->succ_list) ;
   else adj_list = &(graph->pred_list) ;
   listdfs(start_node, stop_node) ;
}

Alist_graph_post_dfs::~Alist_graph_post_dfs()
{
}

void Alist_graph_post_dfs::listdfs(int start_node, int stop_node)
{
   int i ;
   id = 0 ;
   iter_index = 0 ;
   for (i=0 ; i<size ; i++) {
      val[i] = -1 ;
      iter_list[i] = -1 ;
   }
   if(start_node == -1)
   {
      for (i=0 ; i<size ; i++)
      {
	 if (val[i] == -1) visit(i, stop_node) ;
      }
   }
   else
   {
      visit(start_node, stop_node);
   }
   
   size = id;
   iter_index = 0 ;
}

void Alist_graph_post_dfs::visit(int k, int stop_node)
{
   val[k] = id ;
   id++;
   if(k != stop_node)
   {
      for (List_iterator<int> li((*adj_list)[k]) ; li != 0 ; li++)
      {
	 if (val[*li] == -1) visit(*li, stop_node) ;
      }
   }
   iter_list[iter_index++] = k ;
}


////////////////////////////////////////////////////////////////////
//
// Alist_graph_bfs iterator
//
/////////////////////////////////////////////////////////////////

Alist_graph_bfs::Alist_graph_bfs()
{
}

Alist_graph_bfs::Alist_graph_bfs(Alist_graph& g, SEARCH_DIRECTION sd, int start_node, int stop_node)
   : Alist_graph_iterator(g), val(g.succ_list.size()) 
{
   if (sd == DOWN) adj_list = &(graph->succ_list) ;
   else adj_list = &(graph->pred_list) ;
   listbfs(start_node, stop_node) ;
}

Alist_graph_bfs::~Alist_graph_bfs()
{
}

void Alist_graph_bfs::listbfs(int start_node, int stop_node)
{
   int i ;
   id = 0 ;
   iter_index = 0 ;
   for (i=0 ; i<size ; i++)
   {
      val[i] = -1 ;
      iter_list[i] = -1 ;
   }
   if(start_node == -1)
   {
      for (i=0 ; i<size ; i++)
      {
	 if (val[i] == -1) do_search(i, stop_node) ;
      }
   }
   else
   {
      do_search(start_node, stop_node);
   }
   size = id;
   iter_index = 0 ;
}

void Alist_graph_bfs::do_search(int start_node, int stop_node)
{
   List<int> node_queue;
   int cur_node;
   
   node_queue.add_tail(start_node);
   
   while(!node_queue.is_empty())
   {
      cur_node = node_queue.pop();
      iter_list[iter_index++] = cur_node;
      val[cur_node] = id;
      id++;
      if(cur_node != stop_node)
      {
	 for (List_iterator<int> li((*adj_list)[cur_node]) ; li != 0 ; li++)
	 {
	    if (val[*li] == -1)
	    {
	       val[*li] = -2;   // make the node grey
	       node_queue.add_tail(*li) ;
	    }
	 }
      }
   }
}


////////////////////////////////////////////////////////////////////
//
// Alist_graph_lazy_bfs   
//
///////////////////////////////////////////////////////////////////

Alist_graph_lazy_bfs::Alist_graph_lazy_bfs()
   : graph(0), size(0), cur_node(0)
{
}

Alist_graph_lazy_bfs::Alist_graph_lazy_bfs(Alist_graph& g, SEARCH_DIRECTION sd, int start_node, int stop_node)
   : graph(&g)
{
   if (sd == DOWN) adj_list = &(graph->succ_list) ;
   else adj_list = &(graph->pred_list) ;
   size = g.succ_list.size() ;
   visited.resize(size, false) ;
   // Set first_node
   if (start_node == -1) first_node = 0 ;
   else first_node = start_node ;
   // Set last node
   if (stop_node == -1) last_node = size - 1;
   else last_node = stop_node ;
   
   cur_node = first_node ;
   node_queue.add_tail(first_node);
}

Alist_graph_lazy_bfs::~Alist_graph_lazy_bfs() {} 

bool Alist_graph_lazy_bfs::operator==(const int i) const
{
   assert(i == 0) ;
   if (node_queue.is_empty()) return true ;
   else return false ;
}

bool Alist_graph_lazy_bfs::operator!=(const int i) const
{
   assert(i == 0) ;
   if (node_queue.is_empty()) return false ;
   else return true ;
}

void Alist_graph_lazy_bfs::operator++()
{
   advance() ;
}

void Alist_graph_lazy_bfs::operator++(int)
{
   advance() ;
}

int Alist_graph_lazy_bfs::operator*()
{
   return (cur_node) ;
}

void Alist_graph_lazy_bfs::reset()
{
   visited.clear() ;
   cur_node = first_node ;
   node_queue.clear() ;
   node_queue.add_tail(first_node);
}



void Alist_graph_lazy_bfs::advance()
{
   cur_node = node_queue.pop();
   visited.set_bit(cur_node) ;
   if(cur_node == last_node) return ;
   for (List_iterator<int> li((*adj_list)[cur_node]) ; li != 0 ; li++)
   {
      int node_id = *li ;
      if (visited.bit(node_id) == false) {
	 visited.set_bit(node_id) ;   // make the node grey
	 node_queue.add_tail(node_id) ;
      }
   }
}
