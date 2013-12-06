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
//      File:          control_analysis_solver.cpp
//      Authors:       David August
//      Created:       June, 1995
//      Description:   Control-flow analysis -- dominators and conrol 
//                     dependences.
//
///////////////////////////////////////////////////////////////////////////

#include "control_analysis_solver.h"
#include "iterators.h"
#include "attributes.h"
#include "list.h"
#include "el_bb_tools.h"
#include "el_dot_tools.h"
#include "map.h"
#include "op.h"
#include "edge.h"
#include "dbg.h"


// Define bogus top and bottom basic blocks as place holders
Basicblock* symbolic_bb_top = new Basicblock(-1, 0.0);
Basicblock* symbolic_bb_bottom = new Basicblock(-2, 0.0);

bool El_is_symbolic_bb_top(Compound_region* region)
{
  if (region == symbolic_bb_top)
    {
      return true;
    }
  return false;
}

bool El_is_symbolic_bb_bottom(Compound_region* region)
{
  if (region == symbolic_bb_bottom)
    {
      return true;
    }
  return false;
}


////////////////////////////////////////////////////////////////
//
// class Dominator_CFA_solver_data
//
////////////////////////////////////////////////////////////////

Dominator_CFA_solver_data::Dominator_CFA_solver_data()
   : dominator(hash_bb_ptr),
     post_dominator(hash_bb_ptr)
{
  internal_imm_dominator = -1;
  internal_imm_post_dominator = -1;
  imm_dominator = NULL;
  imm_post_dominator = NULL;
}

Dominator_CFA_solver_data::Dominator_CFA_solver_data(int n)
   : internal_dominator(n) , internal_post_dominator(n),
     dominator(hash_bb_ptr),
     post_dominator(hash_bb_ptr)
{
  internal_imm_dominator = -1;
  internal_imm_post_dominator = -1;
  imm_dominator = NULL;
  imm_post_dominator = NULL;
}

Dominator_CFA_solver_data::~Dominator_CFA_solver_data() {}

bool Dominator_CFA_solver_data::operator==(const Dominator_CFA_solver_data& d) const {
   return((internal_dominator == d.internal_dominator) &&
	  (internal_post_dominator == d.internal_post_dominator) &&
	  (internal_imm_dominator == d.internal_imm_dominator) &&
	  (internal_imm_post_dominator == d.internal_imm_post_dominator)) ;
   
}

////////////////////////////////////////////////////////////////
//
// class Dominator_CFA_solver 
//
////////////////////////////////////////////////////////////////

Dominator_CFA_solver::Dominator_CFA_solver() {}

Dominator_CFA_solver::Dominator_CFA_solver(Region* r)
{
   int num_bb;

   graph = new Alist_control_graph(r);
   
   // Initialize Dominator_CFA_solver_date with 2 extra BB's - Top and Bottom
   num_bb = graph->b_map.size();
   top = graph->top;
   bottom = graph->bottom;

#if 0
   cerr << "In Declaring the solver" << endl << flush ;
#endif

// Initialize the data 

   Dominator_CFA_solver_data dinit(num_bb) ; 
   dinit.internal_dominator.clear() ;
   dinit.internal_post_dominator.clear() ;
   dinit.internal_imm_dominator = -1 ;
   dinit.internal_imm_post_dominator = -1 ;
   dominator_info.resize(num_bb, dinit) ;
   if (dbg(cda,1)) el_dot_display_alist_region_tiling_graph(graph) ;

}

Dominator_CFA_solver::~Dominator_CFA_solver()
{
   delete(graph) ;
}

void Dominator_CFA_solver::constrict_region(List_set<Basicblock*> bb_set, Basicblock* head, bool redirect)
{
   Bitvector internal_list(graph->b_map.size());

   if (graph->top >= 0) internal_list.set_bit(graph->top) ;
   if (graph->bottom >= 0) internal_list.set_bit(graph->bottom) ;   

   for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
   {
      Basicblock* cur_bb = *bb_iter;
      
      if (graph->f_map.is_bound(cur_bb)) // add it to the set
      {
	 int tmp1 = graph->f_map.value(cur_bb) ;
	 internal_list.set_bit(tmp1) ;
      }
      else
      {
	 El_punt("Dominator_CFA_solver::constrict_region: Not Bound") ;
      }
   }
   graph->constrict_graph(internal_list, graph->f_map.value(head), redirect);
}

void Dominator_CFA_solver::restore_region()
{
  graph->restore_graph();
}

void Dominator_CFA_solver::solve()
{
   bool not_done = true ;
   int node_indx;
   int bit_indx;
   Bitvector dom_intersection(bottom+1, false);

   Bitvector include_vect(bottom+1, false);

   for(Bit_iterator bi(graph->included); bi != 0 ; bi++) {
      int int_iter = *(bi);
      include_vect.set_bit(int_iter);
   }

//
// Initialize the data structures
//  
   for(node_indx = graph->top; node_indx <= graph->bottom; node_indx++)
   {
      if(node_indx == graph->top)
      {
	 dominator_info[node_indx].internal_dominator.clear();
	 dominator_info[node_indx].internal_dominator.set_bit(node_indx);
      }
      else if(node_indx == graph->bottom)
      {
	 dominator_info[node_indx].internal_post_dominator.clear();
	 dominator_info[node_indx].internal_post_dominator.set_bit(node_indx);
      }
      else
      {
	 dominator_info[node_indx].internal_dominator = include_vect;
	 dominator_info[node_indx].internal_post_dominator = include_vect;
      }
      dominator_info[node_indx].internal_imm_dominator = -1;
      dominator_info[node_indx].internal_imm_post_dominator = -1;
   }
//
// Initialization done
//  
   not_done = true;
   while(not_done)
   {
      not_done = false;
      for(node_indx = graph->top + 1; node_indx <= graph->bottom; node_indx++)
      {
	 dom_intersection.set();
	 if(graph->included.bit(node_indx) == false)
	 {
	    continue;
	 }

         // parkhc 10/9/2005
         // When a block has no predecessor : This happens when opti removes
         //                                   some branch operations.
         // Clear dom_intersection Vector here.
         // Otherwire, it assumes that every block is its dominator.
         if (graph->pred_list[node_indx].is_empty()) {
           dom_intersection.clear();
         } else {
           for(List_iterator<int> pred_iter(graph->pred_list[node_indx]);
               pred_iter != 0; pred_iter++)
           {
              dom_intersection *= dominator_info[*pred_iter].internal_dominator;
           }
         }
	 dom_intersection.set_bit(node_indx);
	 if(dominator_info[node_indx].internal_dominator != dom_intersection)
	 {
	    dominator_info[node_indx].internal_dominator = dom_intersection;
	    not_done = true;
	 }
      }
   }
   if (dbg(cda,3)) {
      cdbg << "Dominators:\n" ;
      int node ; 
      for (node = graph->top ; node <= graph->bottom ; node++) {
	 if(graph->included.bit(node) == false) continue ;
	 cdbg << node << ": "  ;
	 for(int k = graph->top ; k <= graph->bottom ; k++) {
	    if (dominator_info[node].internal_dominator.bit(k))
	       cdbg <<  " " << k ;
	 }
	 cdbg << "\n" ;
      }
   }
//
// Forward propogation of dominator info is done 
// 
   not_done = true;
   while(not_done)
   {
      not_done = false;
      for(node_indx = graph->top; node_indx < graph->bottom; node_indx++)
      {
	 dom_intersection.set();
	 if(graph->included.bit(node_indx) == false) continue;
	 for(List_iterator<int> succ_iter(graph->succ_list[node_indx]);
	     succ_iter != 0; succ_iter++) {
#if 0
	    int id = graph->b_map[*succ_iter]->id();
	    cout << graph->b_map[node_indx]->id() << " : " << id << endl;
#endif
	    dom_intersection *= dominator_info[*succ_iter].internal_post_dominator;
	 }
	 dom_intersection.set_bit(node_indx);
	 if(dominator_info[node_indx].internal_post_dominator != dom_intersection)
	 {
	    dominator_info[node_indx].internal_post_dominator = dom_intersection;
	    not_done = true;
	 }
      }
   } 
   if (dbg(cda,3)) {
      cdbg << "Post Dominators:\n" ;
      int node ; 
      for (node = graph->top ; node <= graph->bottom ; node++) {
	 if(graph->included.bit(node) == false) continue ;
	 cdbg << node << ": "  ;
	 for(int k = graph->top ; k <= graph->bottom ; k++) {
	    if (dominator_info[node].internal_post_dominator.bit(k))
	       cdbg <<  " " << k ;
	 }
	 cdbg << "\n" ;
      }
   }
//
// Backward propogation of post_dominator info is done 
// 

//
// Now construct the internal lists of Basicblock* (Why are these kept as sets ?)
//   
   for(node_indx = graph->top; node_indx <= graph->bottom; node_indx++)
   {
      if(graph->included.bit(node_indx) == false) continue ;
      for(bit_indx = graph->top; bit_indx <= graph->bottom; bit_indx++)
      {
	 if(graph->included.bit(bit_indx) == false) continue ;
	 if(dominator_info[node_indx].internal_dominator.bit(bit_indx))
	 {
	    dominator_info[node_indx].dominator += (Basicblock*) graph->b_map[bit_indx];
	 }
	 if(dominator_info[node_indx].internal_post_dominator.bit(bit_indx))
	 {
	    dominator_info[node_indx].post_dominator += (Basicblock*) graph->b_map[bit_indx];
	 }
      }
   }


   // Immediate Dominator Analysis
   dominator_info[top].internal_imm_dominator = top;
   dominator_info[top].imm_dominator = symbolic_bb_top;
   dominator_info[bottom].internal_imm_post_dominator = bottom;
   dominator_info[bottom].imm_dominator = symbolic_bb_bottom;

   for(node_indx = graph->top; node_indx < graph->bottom; node_indx++) {
      if(graph->included.bit(node_indx) == false) continue;
      Alist_graph_bfs node_bfs(*graph, DOWN, node_indx);
      while ((dominator_info[node_indx].internal_imm_post_dominator == -1) &&
	     (node_bfs != 0)) {
	 int cur_node = *node_bfs ;
	 if((dominator_info[node_indx].internal_post_dominator.bit(cur_node)) &&
	    (cur_node != node_indx)) {
	    dominator_info[node_indx].internal_imm_post_dominator = cur_node ;
	    dominator_info[node_indx].imm_post_dominator = (Basicblock*) graph->b_map[cur_node];
	 }
	 else {
	    node_bfs++;
	 }
      }
   }
   if (dbg(cda,2)) {
      cdbg << "Immediate Post Dominators:\n" ;
      int node ; 
      for (node = graph->top ; node < graph->bottom ; node++) {
	 if(graph->included.bit(node) == false) continue ;
	 cdbg << node << ": "  ;
	 cdbg << dominator_info[node].internal_imm_post_dominator ;
	 cdbg << "\n" ;
      }
   }
   for(node_indx = graph->top + 1; node_indx <= graph->bottom; node_indx++) {
      if(graph->included.bit(node_indx) == false)  continue;
      Alist_graph_bfs node_bfs(*graph, UP, node_indx);
      while ((dominator_info[node_indx].internal_imm_dominator == -1) &&
	     (node_bfs != 0)) {
	 int cur_node = *node_bfs ;
	 if((dominator_info[node_indx].internal_dominator.bit(cur_node)) &&
	    (cur_node != node_indx)) {
	    dominator_info[node_indx].internal_imm_dominator = cur_node ;
	    dominator_info[node_indx].imm_dominator = (Basicblock*) graph->b_map[cur_node];
	 }
	 else {
	    node_bfs++;
	 }
      }
   }
   if (dbg(cda,2)) {
      cdbg << "Immediate Dominators:\n" ;
      int node ; 
      for (node = graph->top+1 ; node <= graph->bottom ; node++) {
	 if(graph->included.bit(node) == false) continue ;
	 cdbg << node << ": "  ;
	 cdbg << dominator_info[node].internal_imm_dominator ;
	 cdbg << "\n" ;
      }
   }
}

Bitvector& Dominator_CFA_solver::get_internal_dom(Basicblock* bb)
{
   int index = 0;

   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_internal_dominator(Region*) : Not Bound") ;
     }
   return (dominator_info[index].internal_dominator) ;
}

Bitvector& Dominator_CFA_solver::get_internal_post_dom(Basicblock* bb)
{
   int index = 0;

   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_internal_post_dominator(Region*) : Not Bound") ;
     }
   return (dominator_info[index].internal_post_dominator) ;
}

Hash_set<Basicblock*>& Dominator_CFA_solver::get_dom(Region* bb)
{
   int index = 0;
   
   if (!bb->is_bb())
     {
       bb = get_first_region_op_from_inops(bb)->parent();
       if(bb->inops().size() != 1)
	 {
	   El_punt("Dominator_CFA_solver::get_dom: region is not single entry.");
	 }
     }
   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_dom(Region*) : Not Bound") ;
     }
   return (dominator_info[index].dominator) ;
}

Hash_set<Basicblock*>& Dominator_CFA_solver::get_post_dom(Region* bb)
{
   int index = 0;

   if (!bb->is_bb())
     {
       //bb = get_last_region_op_from_outops(bb)->parent();
       // hongtaoz: post_dom of a hb should be the post dom of the first
       // bb in the hb, not the postdom of the last bb.
       bb = get_first_region_op_from_subregions(bb)->parent();
       if(bb->outops().size() != 1)
	 {
	   El_punt("Dominator_CFA_solver::get_post_dom: region is not single exit.");
	 }
     }
   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_post_dom(Region*) : Not Bound") ;
     }
   return (dominator_info[index].post_dominator) ;
}

int Dominator_CFA_solver::get_internal_imm_dom(Basicblock* bb)
{
   int index = 0;

   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_internal_imm_dom(Region*) : Not Bound") ;
     }
   return (dominator_info[index].internal_imm_dominator) ;
}

int Dominator_CFA_solver::get_internal_imm_post_dom(Basicblock* bb)
{
   int index = 0;

   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_internal_imm_post_dom(Region*) : Not Bound") ;
     }
   return (dominator_info[index].internal_imm_post_dominator) ;
}

Basicblock* Dominator_CFA_solver::get_imm_dom(Region* bb)
{
   int index = 0;

   if (!bb->is_bb())
     {
       bb = get_first_region_op_from_inops(bb)->parent();
       if(bb->inops().size() != 1)
	 {
	   El_punt("Dominator_CFA_solver::get_imm_dom: region is not single entry.");
	 }
     }
   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_imm_dom(Region*) : Not Bound") ;
     }
   return (dominator_info[index].imm_dominator) ;
}

Basicblock* Dominator_CFA_solver::get_imm_post_dom(Region* bb)
{
   int index = 0;
   
   if (!bb->is_bb())
     {
       //bb = get_last_region_op_from_outops(bb)->parent();
       // hongtaoz: post_dom of a hb should be the post dom of the first
       // bb in the hb, not the postdom of the last bb.
       bb = get_first_region_op_from_subregions(bb)->parent();
       if(bb->outops().size() != 1)
	 {
	   El_punt("Dominator_CFA_solver::get_imm_post_dom: region is not single exit.");
	 }
     }
   if (graph->f_map.is_bound(bb))
     {
       index = graph->f_map.value(bb) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::get_imm_post_dom(Region*) : Not Bound") ;
     }
   return (dominator_info[index].imm_post_dominator) ;
}

bool Dominator_CFA_solver::is_dom(Basicblock* maybe_dom, Region* node)
{
   int index = 0;

   if (!node->is_bb())
     {
       node = get_first_region_op_from_inops(node)->parent();
       if(node->inops().size() != 1)
	 {
	   El_punt("Dominator_CFA_solver::is_dom: region is not single entry.");
	 }
     }
   
   if (graph->f_map.is_bound(node))
     {
       index = graph->f_map.value(node) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::is_dom : Not Bound") ;
     }
   if(dominator_info[index].dominator.is_member(maybe_dom))
     {
       return true;
     }
   else
     {
       return false;
     }
}

bool Dominator_CFA_solver::is_post_dom(Basicblock* maybe_post_dom, Region* node)
{
   int index = 0;

   if (!node->is_bb())
     {
       node = get_last_region_op_from_outops(node)->parent();
       if(node->outops().size() != 1)
	 {
	   El_punt("Dominator_CFA_solver::is_post_dom: region is not single exit.");
	 }
     }
   
   if (graph->f_map.is_bound(node))
     {
       index = graph->f_map.value(node) ;
     }
   else
     {
       El_punt("Dominator_CFA_solver::is_post_dom : Not Bound") ;
     }
   if(dominator_info[index].post_dominator.is_member(maybe_post_dom))
     {
       return true;
     }
   else
     {
       return false;
     }
}




////////////////////////////////////////////////////////////////
//
// class Control_dep_CFA_solver_data
//
////////////////////////////////////////////////////////////////

Control_dep_CFA_solver_data::Control_dep_CFA_solver_data()
   : control_dep(hash_edge_ptr)
{}

Control_dep_CFA_solver_data::~Control_dep_CFA_solver_data() {}

bool Control_dep_CFA_solver_data::operator==(const Control_dep_CFA_solver_data& d) const
{
  return(control_dep == d.control_dep);
}



////////////////////////////////////////////////////////////////
//
// class Control_dep_CFA_solver 
//
////////////////////////////////////////////////////////////////

Control_dep_CFA_solver::Control_dep_CFA_solver() {}

Control_dep_CFA_solver::Control_dep_CFA_solver(Region* r)
{
  region = r;
  if(!El_confirm_BB_tiling(region, bb_set))
    {
      El_punt("Control_dep_CFA_solver::solve: Not a BB tiling.");
    }

  for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
   {
     Basicblock* cur_bb = *bb_iter;
     Control_dep_CFA_solver_data control_dep_data;
     control_dep_info.bind(cur_bb, control_dep_data);
   }

  dom_info = new Dominator_CFA_solver(region);
}

Control_dep_CFA_solver::~Control_dep_CFA_solver()
{
}

void Control_dep_CFA_solver::solve()
{
  dom_info->solve();
  for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
   {
     Basicblock* cur_bb = *bb_iter;
     Hash_set<Basicblock*> post_dom = dom_info->get_post_dom(cur_bb);
     Hash_set<Basicblock*> dom = dom_info->get_dom(cur_bb);

     //cout << "DOM " << cur_bb->id() << endl;
     Hash_set_iterator<Basicblock*> dom_iter ;
     for(dom_iter(dom); dom_iter!=0; dom_iter++)
       {
	 ////cout << (*dom_iter)->id() << endl;
       }
     //cout << "POST DOM" << endl;
     for(dom_iter(post_dom); dom_iter!=0; dom_iter++)
       {
	 //cout << (*dom_iter)->id() << endl;
       }
     
     for(Region_all_outedges edge_iter(cur_bb); edge_iter!=0; edge_iter++)
       {
	 Edge* cur_edge = *edge_iter;
	 Basicblock* dest_bb = (Basicblock*) cur_edge->dest()->parent();
	 
	 if(bb_set.is_member(dest_bb) &&
	    !post_dom.is_member(dest_bb) &&
	    !dom.is_member(dest_bb))
	   {
	     Basicblock* lub = dom_info->get_imm_post_dom(cur_bb);

	     Basicblock* temp_bb = dest_bb;
	     while((temp_bb != lub) && (temp_bb != NULL) &&
		   (lub != NULL) && (temp_bb != symbolic_bb_bottom))
	       {
		 (control_dep_info.value(temp_bb)).control_dep += cur_edge;
		 temp_bb = dom_info->get_imm_post_dom(temp_bb);
	       }
	   }
       }
   }
}

// Control dependence print function 
ostream& operator<<(ostream& os, const Control_dep_CFA_solver& control_dep)
{
  control_dep.print(os);
  return os;
}


void Control_dep_CFA_solver::print(ostream& os) const
{
  
  os << "Control Dependence Info: " << endl;
  
  for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
    {
      Basicblock* cur_bb = *bb_iter;
      //cout << "-- BB #: " << cur_bb->id() << endl;
      Hash_set<Edge*> cont_dep_info = control_dep_info.value(cur_bb).control_dep;
      for(Hash_set_iterator<Edge*> edge_iter(cont_dep_info); edge_iter!=0; edge_iter++)
	{
	  Edge* cur_edge = *edge_iter;
	  cout << "    BB Src: " << cur_edge->src()->parent()->id();
	  cout << "    BB Dest: " << cur_edge->dest()->parent()->id() << endl;
	}
    }
  os << endl;
}

Hash_set<Edge*>& Control_dep_CFA_solver::control_dep(Region* node)
{
  if(!node->is_bb())
    {
      node = get_first_region_op_from_inops(node)->parent();
      if(node->inops().size() != 1)
	{
	  El_punt("Control_dep_CFA_solver::control_dep: region is not single entry.");
	} 
    }
  return control_dep_info.value((Basicblock*)node).control_dep;
}

void Control_dep_CFA_solver::constrict_region(List_set<Basicblock*> new_bb_set, Basicblock* head, bool redirect)
{
  dom_info->constrict_region(new_bb_set, head, redirect);
  bb_set = new_bb_set;
}

void Control_dep_CFA_solver::restore_region()
{
  dom_info->restore_region();
}

// end of control_analysis_solver.cpp
