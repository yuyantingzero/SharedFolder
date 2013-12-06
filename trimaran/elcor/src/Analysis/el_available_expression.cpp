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
//      File:           el_available_expression.cpp
//      Authors:        Sadun Anik
//      Created:        April 1997
//      Description:    Available expressions solver
//
/////////////////////////////////////////////////////////////////////////////

#include "el_available_expression.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "el_bb_tools.h"

Available_expression_data::Available_expression_data() {}

Available_expression_data::Available_expression_data(int n)
   : av_in(n), av_out(n), av_in_initial(n),
     gen(n), kill(n)
{}

bool Available_expression_data::operator==(const Available_expression_data& ad) const
{
   if ((av_in == ad.av_in) &&
      (av_out == ad.av_out) &&
      (av_in_initial == ad.av_in_initial) &&
      (gen == ad.gen) &&
      (kill == ad.kill)) return true ;
   else 
      return false ;
}


/////////////////////////////////////////////////////////////////////////////

Available_expression_map::Available_expression_map(Region* r, AV_EXPR_FLAG fl,
    bool use_normalized_expr)
: operand_map(hash_operand, 1007), expression_map(hash_el_expr, 1007),
  op_to_expression_map(hash_op_ptr, 1007), filter_flag(fl) 
{
   Region_all_ops op_iter;
   
   // Construct an Operand to integer map
   
   int symbol_counter = 0 ;

   is_normalized = use_normalized_expr;
   
   for(op_iter(r) ; op_iter != 0 ; op_iter++) {
      Op* cur_op = *op_iter ;
      for (Op_complete_inputs src_oper(cur_op) ; src_oper != 0 ; src_oper++) {
         Operand* tmpo = &(*src_oper) ;
         if (!operand_map.is_bound(*tmpo)) {
            operand_map.bind(*tmpo,symbol_counter) ;
            symbol_counter++ ;
         }
      }
      for (Op_complete_dests dest_oper(cur_op) ; dest_oper != 0 ; dest_oper++) {
         Operand* tmpo = &(*dest_oper) ;
         if (!operand_map.is_bound(*tmpo)) {
            operand_map.bind(*tmpo,symbol_counter) ;
            symbol_counter++ ;
         }
      }
   }
   
   // Initialize the reverse map
   
   rev_operand_map.resize(symbol_counter) ;
   for (Hash_map_iterator<Operand,int> mdi(operand_map) ;
   mdi != 0 ; mdi++) {
      rev_operand_map[(*mdi).second] = (*mdi).first ;
   }

   // parkhc 1/18/2005
   // is_normalized flag : 
   // The original av_expr analysis seems to be made for CSE.
   // It works fine with CSE, but showed some bugs when used with 
   // copy propagation opti.
   // In the original opti, all the ops with same normalized expr have 
   // same expr_id.
   // However, in copy propagation, each op need to have unique expr_id.
   // Here's the example why this is an issue.
   //
   // mov r1, m(int_ret)    // inst 1
   // ..
   // BRL                   // inst 2
   // ..
   // mov r4, m(int_ret)    // inst 3
   // ..
   // mov r3, r1            // inst 4
   //
   // In this example, copy propagation opti checks if inst 1 is 
   // available at the point before inst 4 by using av_expr analysis.
   // However, as inst 1 and inst 3' RHSs have same form of expression,
   // they are pointing to the same expr_id.
   // So, expr of inst 1 is killed by inst 2 and when it reaches inst 3,
   // it gets valid again. 
   // As a result, the analysis thinks inst 1 is an available expression 
   // at inst 4 although inst 3 is the one which is really available
   // at inst 4. Yeah, this is all because inst 1 and inst 3 have same
   // expr_id.
   //
   // This is why 'is_normalized' flag was introduced. 
   // When it is true, the original av_expr analysis is called.
   // When it is false, instead of using normalized expression to assign
   // expr_id to each op, it just assign an unique expr id to each op.
   //
   // So, CSE calls av_expr analysis with use_normalized_expr flag TRUE,
   // whereas global copy propagation calls it with flag FALSE

   if (is_normalized) {
     // For CSE opti
     
     // Construct an Expression to integer map
     // and op to expresion_int map
     
     int expr_counter = 0 ;
     for (op_iter(r) ; op_iter != 0 ; op_iter++) {
        Op* cur_op = *op_iter ;
        if (is_interesting(cur_op)) {
           El_expr* new_expr = get_normalized_expression(cur_op);
           if (!expression_map.is_bound(*new_expr)) {
              expression_map.bind(*new_expr,expr_counter) ;
              expr_counter++ ;
           }
           int expr_id = expression_map.value(*new_expr) ;
           op_to_expression_map.bind(*op_iter, expr_id) ;
           delete new_expr ;
        }
     }
     symbol_count = expr_counter ;
     
     // Initialize the reverse expression map   
     
     rev_expression_map.resize(expr_counter) ;
     for (Hash_map_iterator<El_expr, int> exprmapi(expression_map) ;
     exprmapi != 0 ; exprmapi++) {
        rev_expression_map[(*exprmapi).second] = (*exprmapi).first ;
     }

     // Initialize expression to op map
     
     expression_to_op_map.resize(expr_counter) ;
     for (Hash_map_iterator<Op*,int> mdi2(op_to_expression_map) ;
     mdi2 != 0 ; mdi2++) {
        expression_to_op_map[(*mdi2).second].add_tail((*mdi2).first) ;
     }
   
     // Initialize expression kill Vector
     int i;
     Hash_set<int> dummy_set(hash_int, 128); // to set the hash function
     operand_to_killed_expr_map.resize(symbol_counter, dummy_set) ;
     for (i = 0 ; i < (int)rev_expression_map.size() ; i++) {

  //      for (j = 0 ; j < rev_expression_map[i].oprnd_count ; j++) {
  //        operand_to_killed_expr_map[rev_expression_map[i].oprnd[j]].add_tail(i) ;
  //      }

       // make this aware of memvrs. -KF 12/2004
       // somewhat conservative because a memvr will kill an expression if any
       // ops of that expression use the memvr.
       for (List_iterator<Op*> opi(expression_to_op_map[i]); opi!=0; opi++) {
         for (Op_complete_inputs iter(*opi); iter!=0; iter++) {
           int opnd_idx = operand_map.value(*iter);
           operand_to_killed_expr_map[opnd_idx] += i;
         }
       }
     }
   } else {
     // For Copy Propagation Opti

     // rev_expression_map, op_to_expression_map, expressoin_to_op_map
     // are not used.

     // op to expr_int map
     int op_counter = 0;
     for (op_iter(r); op_iter != 0; op_iter++) {
       Op* cur_op = *op_iter;
       if (is_interesting(cur_op))
         op_to_expression_map.bind(cur_op, op_counter++);
     }

     symbol_count = op_counter;
  
     // Initialize op kill Vector
     Hash_set<int> dummy_set(hash_int, 128);
     operand_to_killed_expr_map.resize(symbol_counter, dummy_set);
     for (op_iter(r); op_iter != 0; op_iter++) {
       Op* cur_op = *op_iter;
       if (is_interesting(cur_op)) {
         for (Op_complete_inputs iter(cur_op); iter != 0; iter++) {
           int opnd_idx = operand_map.value(*iter);
           operand_to_killed_expr_map[opnd_idx] += 
              op_to_expression_map.value(cur_op);
         }
       }
     }
   }
}

El_expr* Available_expression_map::get_normalized_expression(Op* op)
{
   assert (is_normalized);

   int num_of_src, src_count ;

   num_of_src = op->num_srcs() ;
   El_expr* nexp = new El_expr(num_of_src) ;
   nexp->opc = op->opcode() ;
   src_count = 0 ;
   for(Op_explicit_sources oprndi(op) ; oprndi != 0 ; oprndi++) {
      nexp->oprnd[src_count] = operand_map.value(*oprndi) ;
      src_count++ ;
   }
// Normalize the operands and opcode if necessary
   if ((num_of_src == 2) && (nexp->oprnd[0] > nexp->oprnd[1])) {
      Opcode rev_opcode = get_reverse_op(nexp->opc) ;
      if (rev_opcode != NO_OP) {
	 nexp->opc = rev_opcode ;
	 int tmp = nexp->oprnd[0] ;
	 nexp->oprnd[0] = nexp->oprnd[1] ;
	 nexp->oprnd[1] = tmp ;
      }
   }
   return nexp ;
}

bool Available_expression_map::is_interesting(Op* op)
{
   int num_of_src ;

   num_of_src = op->num_srcs() ;
   switch (filter_flag) {
   case AV_EXPR_UNARY:
      if (num_of_src == 1) return true ;
      else return false ;
      break ;
   case AV_EXPR_BINARY:
      if (num_of_src == 2) return true ;
      else return false ;
      break ;
   case AV_EXPR_UNARY_OR_BINARY:
      if ((num_of_src == 1) || (num_of_src == 2)) return true ;
      else return false ;
      break ;
   default:
      El_punt("Unkown expression filter") ;
      return false ;
   }
}


/////////////////////////////////////////////////////////////////////////////

Available_expression_solver::~Available_expression_solver()
{
   if (tmp_gen) delete tmp_gen ;
   if (tmp_kill) delete tmp_kill ;
}

Available_expression_solver::Available_expression_solver(Region* r, AV_EXPR_FLAG fl, bool use_normalized_expr)
   : node_graph(r,GRAPH_HBS_BBS_SESE), emap(r, fl, use_normalized_expr), 
  tmp_gen(NULL), tmp_kill(NULL)
{
   int i ;

   is_normalized = use_normalized_expr;

   make_macros_canonical(r);
   
// Initialize the transfer functions for ops

   Available_expression_data expr_init_top(emap.symbol_count) ;
   expr_init_top.av_in.set() ;
   expr_init_top.av_out.set() ;
   Available_expression_data expr_init_bottom(emap.symbol_count) ;
   expr_init_bottom.av_out.set() ;

   node_data.resize(node_graph.node_count, expr_init_top) ;

   for (Region_entry_ops renti(r) ; renti != 0 ; renti++) {
     Op* cur_entry_op = *renti ;
     Compound_region* cur_entry_region = get_enclosing_analysis_scope(cur_entry_op) ;
     int entry_region_id = node_graph.f_map.value(cur_entry_region) ;
     node_data[entry_region_id] = expr_init_bottom ;
   }

   for (i = 0 ; i < node_graph.node_count ; i++) {
      compute_local_transfer_function(i) ;
   }
}

void Available_expression_solver::solve()
{
   bool not_done = true ;

   int iteration_count = 0  ;
//   cerr << "Starting availability" << endl ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(node_graph,DOWN) ; iter != 0 ; iter++) {
	 meet_function(*iter, not_done) ;
	 transfer_function(*iter, not_done) ;
      }
   }
}

Pred_cookie Available_expression_solver::get_available_expression_cookie_before_op
   (Compound_region* scope, Op* src_expr_op, Op* before_this_op) 
{
   reset_pred_cookie_vector(tmp_gen, El_false_cookie) ;
   // initialize the expression we are interested in to its block
   // entry value
   int expr_id = emap.op_to_expression_map.value(src_expr_op) ;
   int region_id = node_graph.f_map.value(scope) ;
   
   (*tmp_gen)[expr_id] = node_data[region_id].av_in.bit(expr_id) ;
   
   Region_entry_ops entry(scope);
   Op* entry_op = *entry;
   Op* exit_op = before_this_op->prev() ;
   Pred_jar pj(scope) ;


   forward_local_block_walk(scope, entry_op, exit_op, tmp_gen, tmp_kill,
      pj, AV_EXPR_LOCAL_QUERY) ;
      
   return ((*tmp_gen)[expr_id]) ;
}

bool Available_expression_solver::is_op_fully_redundant(Op* expr_op)
{
  // parkhc 1/18/2005
  assert (is_normalized);

  Compound_region* cur_reg = get_enclosing_analysis_scope(expr_op) ;
  Op_explicit_sources src_iter(expr_op);
  if (src_iter == 0) {
    return false;
  }  
  else {
    El_ref expr_ref = src_iter.get_ref();
    Pred_cookie copy_available_cond = 
      this->get_available_expression_cookie_before_op(cur_reg, 
						       expr_op, expr_op) ;
    Pred_jar pj(cur_reg) ;
    Pred_cookie use_cond = pj.get_lub_guard(expr_ref) ;
    return use_cond.is_subset(copy_available_cond); 
  }
}

List<int> Available_expression_solver::get_expression_list()
{
  // parkhc 1/18/2005
  assert (is_normalized);

  List<int> return_list;
  for (Hash_map_iterator<El_expr,int> mdi(emap.expression_map) ;
       mdi != 0 ; mdi++) {
    return_list.add_tail((*mdi).second);
  }
  return return_list;
}

List<Op*>& Available_expression_solver::get_ops_for_expression(int expr_id) 
{
  // parkhc 1/18/2005
  assert (is_normalized);

  return emap.expression_to_op_map[expr_id];
}

void Available_expression_solver::compute_local_transfer_function(int n)
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

   Pred_jar pj(blk) ;

   reset_pred_cookie_vector(tmp_gen, El_false_cookie) ;
   reset_pred_cookie_vector(tmp_kill, El_false_cookie) ;

   Region_entry_ops entry(blk);
   Op* entry_op = *entry;


   forward_local_block_walk(blk, entry_op, exit_op, tmp_gen, tmp_kill, 
      pj, AV_EXPR_TRANSFER_FUNCTION);
      // Now the local walk is done, set the global vectors
   Pred_cookie path_predicate = pj.get_lub_path_guard(entry_op, exit_op) ;
   Available_expression_data &ad = node_data[n];
   int ii_bound = emap.symbol_count ;
   int ii ;

   for (ii=1 ; ii < ii_bound ; ii++) {
      // gen if def on all paths
      if (path_predicate.is_subset((*tmp_gen)[ii])) {
         ad.gen.set_bit(ii) ;
      }
   }
   for (ii=1 ; ii < ii_bound ; ii++) {
      // kill if def on all path
      if (!(path_predicate.is_disjoint((*tmp_kill)[ii]))) {
         ad.kill.set_bit(ii) ;
      }
   }

   
}

void Available_expression_solver::meet_function(int n, bool& tag)
{
   Available_expression_data &ed = node_data[n] ;

   Bitvector old_av_in ;

   if (tag == false) {
      old_av_in = ed.av_in ;
   }

   List_iterator<int> li(node_graph.pred_list[n]) ;
   if (li != 0) {
      ed.av_in = node_data[*li].av_out ;
      li++ ;
   }
   for ( ; li != 0 ; li++) {
      ed.av_in *= node_data[*li].av_out ;
   }

   if (tag == false) tag = (ed.av_in != old_av_in) ;
   
   return ;
}

void Available_expression_solver::transfer_function(int n, bool& tag)
{
   Available_expression_data &ed = node_data[n] ;

   Bitvector old_av_out ;

   if (tag == false) old_av_out = ed.av_out ;
   
   ed.av_out = ed.av_in ;
   ed.av_out -= ed.kill ;
   ed.av_out += ed.gen ;

   if (tag == false) {
      tag = (ed.av_out != old_av_out) ;
   }
   
   return ;
}

void Available_expression_solver::forward_local_block_walk
     (Compound_region* r, Op* entry_op, Op* exit_op, 
     Vector<Pred_cookie>* p_gen_ptr,
     Vector<Pred_cookie>* p_kill_ptr, Pred_jar& pj, 
     AV_EXPR_LOCAL_WALK_FLAG flg)
{
   Region_ops_C0_order opi1;

   Vector<Pred_cookie>& p_gen = *p_gen_ptr ;
   Vector<Pred_cookie>& p_kill = *p_kill_ptr ;

   bool done = false;
   for (opi1(r,entry_op) ; done != true; opi1++) {
      if ((*opi1) == exit_op) {
         done = true;
      }
      assert ((*opi1)->is_op());
      Op* cur_op = (Op*)(*opi1);
      
      // Processed genned expression
      if (emap.is_interesting(cur_op)) {
         int cur_index = emap.op_to_expression_map.value(cur_op) ;
         Pred_cookie cur_pc = pj.get_glb_src_expression_guard(cur_op) ;
         p_gen[cur_index].glb_sum(cur_pc) ;
         if (flg == AV_EXPR_TRANSFER_FUNCTION)
           p_kill[cur_index].lub_diff(cur_pc) ;
      }
      
      // Process killed expressions
      for (Op_complete_dests desti(cur_op) ; desti != 0 ; desti++) {
         if ((*desti).is_undefined())
            continue;
         int dest_id = emap.operand_map.value(*desti) ;
         El_ref cur_ref = desti.get_ref() ;
         Pred_cookie pq1 = pj.get_lub_guard(cur_ref) ;
         for (Hash_set_iterator<int> killi(emap.operand_to_killed_expr_map[dest_id]) ; killi != 0 ; killi++) {
            int cur_index = *killi ;
            p_gen[cur_index].glb_diff(pq1) ;
            if (flg == AV_EXPR_TRANSFER_FUNCTION)
              p_kill[cur_index].lub_sum(pq1) ;
         }
      }
   }
}


void Available_expression_solver::reset_pred_cookie_vector(Vector<Pred_cookie>*& pcptr, Pred_cookie& value)
{
   if (value == El_false_cookie) {
      if (!pcptr) { // the Vector does not exist
         pcptr = new Vector<Pred_cookie>(emap.symbol_count, El_false_cookie);
      }
      else {  // reset the Vector
         int vector_size = pcptr->size() ;
         for (int i=0; i < vector_size; i++) {
            Pred_cookie& cur_cookie = (*pcptr)[i] ;
            cur_cookie.set_to_false() ;
         }
      }
   }
   else if (value == El_true_cookie) {
      if (!pcptr) { // the Vector does not exist
         pcptr = new Vector<Pred_cookie>(emap.symbol_count, El_true_cookie);
      }
      else {  // reset the Vector
         int vector_size = pcptr->size() ;
         for (int i=0; i < vector_size; i++) {
            Pred_cookie& cur_cookie = (*pcptr)[i] ;
            cur_cookie.set_to_true() ;
         }
      }
   }
   else {
      if (!pcptr) { // the Vector does not exist
         pcptr = new Vector<Pred_cookie>(emap.symbol_count, El_true_cookie);
      }
      else {  // reset the Vector
         int vector_size = pcptr->size() ;
         for (int i=0; i < vector_size; i++) {
            Pred_cookie& cur_cookie = (*pcptr)[i] ;
            if (cur_cookie != value) cur_cookie = value ;
         }
      }
   }
}


