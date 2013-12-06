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
//      File:           analysis_template.cpp
//      Authors:        
//      Created:        February 1997
//      Description:    Predicated analysis template
//
/////////////////////////////////////////////////////////////////////////////

#include "analysis_template.h"
#include "iterators.h"

#define FORWARD_PROBLEM
#define BITVECTOR_ANALYSIS


/////////////////////////////////////////////////////////////////////////////

ANALYSIS_data::ANALYSIS_data() 
{}

ANALYSIS_data::ANALYSIS_data(int n) 
{ 
   // WARNING: replace this function
   assert(0) ;
}

ANALYSIS_data::~ANALYSIS_data() 
{ 
   // WARNING: replace this function
   assert(0) ;
}


/////////////////////////////////////////////////////////////////////////////

ANALYSIS_map::ANALYSIS_map(Region* r)
{
   // WARNING: replace this function
   assert(0) ;
}

ANALYSIS_map::~ANALYSIS_map() 
{
   // WARNING: replace this function
   assert(0) ;
}

int ANALYSIS_map::symbol_count()
{
   // WARNING: replace this function
   assert(0) ;
   return(0) ;
}

bool ANALYSIS_map::is_valid_symbol(SYMBOL&) 
{
   // WARNING: replace this function
   assert(0) ;
   return (false) ;
}

int ANALYSIS_map::symbol_to_int(SYMBOL&) 
{
   // WARNING: replace this function
   assert(0) ;
   return (0) ;
}

SYMBOL ANALYSIS_map::int_to_symbol(int) 
{
   // WARNING: replace this function
   assert(0);
   SYMBOL* s; 
   return (*s) ;
}


/////////////////////////////////////////////////////////////////////////////

ANALYSIS_solver::ANALYSIS_solver(Region* r)
: node_graph(r, GRAPH_HBS_BBS_SESE), symbol_map(r)
{
   int graph_size = node_graph.succ_list.dim() ;

   // initilaize data
   ANALYSIS_data dinit(symbol_map.symbol_count()) ;
   node_data.reshape(graph_size,dinit) ;

   // computer transfer functions
   int i ;
   for (i=0; i < graph_size; i++) {
      compute_local_remap_function(i) ;
   }
   for (i=0; i < graph_size; i++) {
      compute_local_transfer_function(i) ;
   }
}

void ANALYSIS_solver::solve() 
{
   bool not_done = true;
   int iteration_count = 0;
   
   while (not_done) {
      iteration_count++;
      not_done = false;
#ifdef FORWARD_PROBLEM
      for (Alist_graph_dfs iter(node_graph, DOWN) ; iter != 0; iter++) {
         int cur_node = *iter;
         meet_function(cur_node, not_done);
         transfer_function(cur_node, not_done);
         remap_function(cur_node, not_done) ;
      }
#else 
      for (Alist_graph_dfs iter(node_graph, UP) ; iter != 0; iter++) {
         int cur_node = *iter;
         meet_function(cur_node, not_done);
         remap_function(cur_node, not_done) ;
         transfer_function(cur_node, not_done);
      }
#endif
   }
}

void ANALYSIS_solver::compute_local_transfer_function(int n) 
{
   Region* r = node_graph.b_map[n];
   
   if (r->is_hb()) return;
   
   Compound_region* blk;
   Op* exit_op;
   
   if (r->is_bb()) {
      blk = (Compound_region*)r;
      Region_exit_ops exit (blk);
      exit_op = *exit;
   }
   else {
      assert (r->is_op ());
      blk = r->parent ();
      while (!blk->is_hb ()) {
         blk = blk->parent ();
      }
      exit_op = (Op*)r;
   }
   local_block_walk(blk, exit_op, n);
}

void ANALYSIS_solver::compute_local_remap_function(int n) 
{
   Region* r = node_graph.b_map[n];
   
   if (r->is_hb()) return;
   
   Compound_region* blk;
   Op* exit_op;
   
   if (r->is_bb()) {
      blk = (Compound_region*)r;
      Region_exit_ops exit (blk);
      exit_op = *exit;
   }
   else {
      assert (r->is_op ());
      blk = r->parent ();
      while (!blk->is_hb ()) {
         blk = blk->parent ();
      }
      exit_op = (Op*)r;
   }
   assert(0) ;
//   local_block_walk(blk, exit_op, n);
}




void ANALYSIS_solver::meet_function(int n, bool& tag)
{
   ANALYSIS_data &ad = node_data[n];

#ifdef BITVECTOR_ANALYSIS   
   Bitvector old_ad_in;

   if (tag == false)
      old_ad_in = ad.ad_in;
   
   ad.ad_in = ad.ad_in_initial;
   for (List_iterator<int> li(node_graph.pred_list[n]); li != 0; li++)
 #ifdef OR_MEET_FUNCTION
      ad.ad_in += node_data[*li].ad_out ;
 #else 
      ad.ad_in *= node_data[*li].ad_out ;
   
   if (tag == false)
      tag = (ad.ad_in != old_ad_in) ;
 #endif OR_MEET_FUNCTION
#else 
// Other represenations go here
#endif BITVECTOR_ANALYSIS
      return;

}

void ANALYSIS_solver::transfer_function(int n, bool& tag)
{
  ANALYSIS_data &ad = node_data[n];
   
#ifdef BITVECTOR_ANALYSIS   
  Bitvector old_ad_out;

  if (tag == false)
    old_ad_out = ad.ad_out;

  ad.ad_out = ad.ad_in;
  ad.ad_out -= ad.ad_kill;
  ad.ad_out += ad.ad_gen;

  if (tag == false)
    tag = (ad.ad_out != old_ad_out);
#else 
// Other represenations go here
#endif BITVECTOR_ANALYSIS
  return;
}

void ANALYSIS_solver::remap_function(int n, bool& tag)
{
  ANALYSIS_data &ad = node_data[n];
  if (ad.ad_remap == NULL) return;

#ifdef BITVECTOR_ANALYSIS   
  Bitvector old_ad_in;
  
  if (tag == false)
    old_ad_in = ad.ad_in;

#ifdef DENSE_REMAPS
// Dense remap implementation
  int vector_dim = ad.ad_in.size() ;
  
  for (int to = 0 ; to < vector_dim ; to++) {
     int from = (*(ad.ad_remap))[to] ;
     if (from == -1) {
        continue ;
     }
     else if (from >= 0) { // ad.ad_in[to] = old_ad_in[from]
        if (old_ad_in.bit(from) == true) {
           ad.ad_in.set_bit(to) ;
        }
        else {
           ad.ad_in.reset_bit(to) ;
        }
     }
     else { // from <= -2  so   ad.ad_in[to] = ad_in_initial[to]
        if (ad.ad_in_initial.bit(to) == true) {
           ad.ad_in.set_bit(to) ;
        }
        else {
           ad.ad_in.reset_bit(to) ;
        }
     }
     
  }
#else 
  for (List_iterator<Pair<int,int> > toi(*(ad.ad_remap)) ; toi!=0 ; toi++) {
     Pair<int,int> tmp_pair = *toi ;
     int to = tmp_pair.first ;
     int from = tmp_pair.second ;
     if (from >= 0) { // ad.ad_in[to] = old_ad_in[from]
        if (old_ad_in.bit(from) == true) {
           ad.ad_in.set_bit(to) ;
        }
        else {
           ad.ad_in.reset_bit(to) ;
        }
     }
     else { // from <= -2  so   ad.ad_in[to] = ad_in_initial[to]
        if (ad.ad_in_initial.bit(to) == true) {
           ad.ad_in.set_bit(to) ;
        }
        else {
           ad.ad_in.reset_bit(to) ;
        }
     }
  }
  // Sparse remap implementation goes here
#endif DENSE_REMAPS
  if (tag == false)
     tag = (ad.ad_in != old_ad_in);
#else 
  // Other represenations go here
#endif BITVECTOR_ANALYSIS
  return;
  
}

static Vector<Pred_cookie>* p_gen_ptr = NULL ;
static Vector<Pred_cookie>* p_kill_ptr = NULL ;

void ANALYSIS_solver::local_block_walk(Compound_region* r, Op* exit_op, int i) 
{
   Region_entry_ops entry(r);
   Op* entry_op = *entry;

#ifdef FORWARD_PROBLEM
   Region_ops_C0_order opi1;
   Op* start_op = entry_op ;
   Op* terminal_op = exit_op;
#else 
   Region_ops_reverse_C0_order opi1;
   Op* start_op = exit_op ;
   Op* terminal_op = entry_op ;
#endif FORWARD_PROBLEM
   
   Pred_jar pj(r) ;
   
   reset_pred_cookie_vector(p_gen_ptr, El_false_cookie) ;
   reset_pred_cookie_vector(p_kill_ptr, El_true_cookie) ;
   
   Vector<Pred_cookie>& p_gen = *p_gen_ptr ;
   Vector<Pred_cookie>& p_kill = *p_kill_ptr ;

   //
   // traverse ops in hyperblock in linear order from entry to exit_op
   // or from exit to entry op depending on the problem
   //
   bool done = false;
   for (opi1(r,start_op) ; done != true; opi1++) {
      if ((*opi1) == terminal_op) {
         done = true;
      }
      assert ((*opi1)->is_op());
      Op* cur_op = (Op*)(*opi1);
      
#ifdef FORWARD_PROBLEM   
      for (ANALYSIS_op_genned_names namei1(cur_op) ; namei1 != 0 ; namei1++) {
         SYMBOL& cur_symbol = *namei1 ;
         if (symbol_map.is_valid_symbol(cur_symbol)) {
            int cur_index = symbol_map.symbol_to_int(cur_symbol) ;
            Pred_cookie cur_pc = ANALYSIS_get_lub_guard(pj,cur_symbol) ;
            p_gen[cur_index].lub_sum(cur_pc) ;
            p_kill[cur_index].lub_sum(cur_pc) ;
         }
      }
      for (ANALYSIS_op_killed_names namei2(cur_op) ; namei2 != 0 ; namei2++) {
         SYMBOL& cur_symbol = *namei1 ;
         if (symbol_map.is_valid_symbol(cur_symbol)) {
            int cur_index = symbol_map.symbol_to_int(cur_symbol) ;
            p_gen[cur_index].lub_diff(ANALYSIS_get_lub_guard(pj,cur_symbol)) ;
            p_kill[cur_index].lub_diff(ANALYSIS_get_glb_guard(pj,cur_symbol)) ;
         }
         
      }
#else 
      for (ANALYSIS_op_killed_names namei2(cur_op) ; namei2 != 0 ; namei2++) {
         SYMBOL& cur_symbol = *namei1 ;
         if (symbol_map.is_valid_symbol(cur_symbol)) {
            int cur_index = symbol_map.symbol_to_int(cur_symbol) ;
            p_gen[cur_index].lub_diff(ANALYSIS_get_lub_guard(pj,cur_symbol)) ;
            p_kill[cur_index].lub_diff(ANALYSIS_get_glb_guard(pj,cur_symbol)) ;
         }
      }
      for (ANALYSIS_op_genned_names namei1(cur_op) ; namei1 != 0 ; namei1++) {
         SYMBOL& cur_symbol = *namei1 ;
         if (symbol_map.is_valid_symbol(cur_symbol)) {
            int cur_index = symbol_map.symbol_to_int(cur_symbol) ;
            Pred_cookie cur_pc = ANALYSIS_get_lub_guard(pj,cur_symbol) ;
            p_gen[cur_index].lub_sum(cur_pc) ;
            p_kill[cur_index].lub_sum(cur_pc) ;
         }
      }
#endif FORWARD_PROBLEM
   }

   // Now the local walk is done, set the global vectors
   Pred_cookie path_predicate = pj.get_lub_path_guard(start_op, terminal_op) ;
   ANALYSIS_data &ad = node_data[i];
   int ii_bound = symbol_map.symbol_count() ;
   int ii ;

   for (ii=1 ; ii < ii_bound ; ii++) {
      // gen if def on any path
      if (!(path_predicate.is_disjoint(p_gen[ii]))) {
         ad.ad_gen.set_bit(ii) ;
      }
   }
   for (ii=1 ; ii < ii_bound ; ii++) {
      // kill if def on all path
      if (path_predicate.is_disjoint(p_kill[ii])) {
         ad.ad_gen.set_bit(ii) ;
      }
   }
}

void ANALYSIS_solver::reset_pred_cookie_vector(Vector<Pred_cookie>*& pcptr, Pred_cookie& value)
{
   if (value == El_false_cookie) {
      if (!pcptr) { // the Vector does not exist
         p_gen_ptr = new Vector<Pred_cookie>(symbol_map.symbol_count(), El_false_cookie);
      }
      else {  // reset the Vector
         int vector_size = p_gen_ptr->dim() ;
         for (int i=0; i < vector_size; i++) {
            Pred_cookie& cur_gen = (*p_gen_ptr)[i] ;
            cur_gen.set_to_false() ;
         }
      }
   }
   else if (value == El_true_cookie) {
      if (!p_kill_ptr) { // the Vector does not exist
         p_kill_ptr = new Vector<Pred_cookie>(symbol_map.symbol_count(), El_true_cookie);
      }
      else {  // reset the Vector
         int vector_size = p_kill_ptr->dim() ;
         for (int i=0; i < vector_size; i++) {
            Pred_cookie& cur_kill = (*p_kill_ptr)[i] ;
            cur_kill.set_to_true() ;
         }
      }
   }
   else {
      if (!p_kill_ptr) { // the Vector does not exist
         p_kill_ptr = new Vector<Pred_cookie>(symbol_map.symbol_count(), El_true_cookie);
      }
      else {  // reset the Vector
         int vector_size = p_kill_ptr->dim() ;
         for (int i=0; i < vector_size; i++) {
            Pred_cookie& cur_kill = (*p_kill_ptr)[i] ;
            if (cur_kill != value) cur_kill = value ;
         }
      }
   }
}


/////////////////////////////////////////////////////////////////////////////

Pred_cookie ANALYSIS_get_lub_guard(Pred_jar&, SYMBOL&) 
{
   // WARNING: replace this function
   assert(0); 
   return (El_false_cookie);
}

Pred_cookie ANALYSIS_get_glb_guard(Pred_jar&, SYMBOL&) 
{
   // WARNING: replace this function
   assert(0); 
   return (El_false_cookie);
}
