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
//      File:           simple_pre.cpp
//      Authors:        Sadun Anik
//      Created:        August 1994
//      Description:    PRE solver
//
/////////////////////////////////////////////////////////////////////////////

#include "simple_pre.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "hash_functions.h"


Expression_data::Expression_data() {}

Expression_data::Expression_data(int n)
   : av_in(n), av_out(n), ant_in(n), ant_out(n),
     transp(n), comp(n), antloc(n), laterin(n), mark_delete(n)
{}

Expression_data::~Expression_data() {}

bool Expression_data::operator==(const Expression_data& d) const
{
   if ((av_in == d.av_in) && (av_out == d.av_out) &&
       (ant_in == d.ant_in) && (ant_out == d.ant_out) &&
       (transp == d.transp) && (comp == d.comp) &&
       (antloc == d.antloc) && (laterin == d.laterin) &&
       (mark_delete == d.mark_delete)) return true ;
   else return false ;
}

///////////////////////////////////////////////////////////////

Edge_expression_data::Edge_expression_data() {}

Edge_expression_data::Edge_expression_data(int n)
   : earliest(n), later(n), insert(n)
{}

Edge_expression_data::~Edge_expression_data() {}

bool Edge_expression_data::operator==(const Edge_expression_data& d) const
{
   if ((earliest == d.earliest) && (later == d.later) &&
       (insert == d.insert) ) return true ;
   else return false ;
}

////////////////////////////////////////////////////////////////

Avail_expr_solver::Avail_expr_solver()
   : operand_map(hash_operand,1000),
     expression_map(hash_el_expr,1000), op_to_expr_map(hash_int,1000)
{}

Avail_expr_solver::~Avail_expr_solver()
{}

Avail_expr_solver::Avail_expr_solver(Region* r)
   : graph(r,GRAPH_OPS), edge_graph(graph), operand_map(hash_operand,1000),
     expression_map(hash_el_expr,1000), op_to_expr_map(hash_int,1000)
{
   int i,j ;
// Construct an Operand to integer map
   int symbol_counter = 0 ;
   Alist_graph_dfs op_iter(graph);
//   cerr << "Declaring the solver" << endl << flush ;

   for(op_iter.reset() ; op_iter != 0 ; op_iter++) {
      Op* cur_op = (Op*)graph.b_map[*op_iter] ;
      for (Op_all_inputs src_oper(cur_op) ; src_oper != 0 ; src_oper++) {
	 Operand* tmpo = &(*src_oper) ;
	 if (!operand_map.is_bound(*tmpo)) {
	    operand_map.bind(*tmpo,symbol_counter) ;
	    symbol_counter++ ;
	 }
      }
      for (Op_all_dests dest_oper(cur_op) ; dest_oper != 0 ; dest_oper++) {
	 Operand* tmpo = &(*dest_oper) ;
	 if (!operand_map.is_bound(*tmpo)) {
	    operand_map.bind(*tmpo,symbol_counter) ;
	    symbol_counter++ ;
	 }
      }
   }
//   cerr << "Solver symbol table completed" << endl << flush ;

// Initialize the reverse map   
   rev_operand_map.reshape(symbol_counter) ;
   for (Hash_map_iterator<Operand,int> mdi(operand_map) ;
	mdi != 0 ; mdi++) {
      rev_operand_map[(*mdi).second] = (*mdi).first ;
   }
//   cerr << "Reverse symbol map done" << endl << flush ;

// Hash expressions
   int expr_counter = 0 ;
   for (op_iter.reset() ; op_iter != 0 ; op_iter++) {
      Op* cur_op = (Op*)graph.b_map[*op_iter] ;
      El_expr* new_expr = get_normalized_expression(cur_op);
      if (!expression_map.is_bound(*new_expr)) {
	 expression_map.bind(*new_expr,expr_counter) ;
	 expr_counter++ ;
      }
      int expr_id = expression_map.value(*new_expr) ;
      op_to_expr_map.bind(*op_iter, expr_id) ;
      delete new_expr ;
   }

//   cerr << "Expression table completed" << endl << flush ;

// Initialize the reverse expression map   
   rev_expression_map.reshape(expr_counter) ;
   for (Hash_map_iterator<El_expr, int> exprmapi(expression_map) ;
	exprmapi != 0 ; exprmapi++) {
      rev_expression_map[(*exprmapi).second] = (*exprmapi).first ;
   }
//   cerr << "Reverse expression map done" << endl << flush ;

// Initialize expression kill Vector
   opernd_to_killed_expr_map.reshape(symbol_counter) ;
   for (i = 0 ; i < rev_expression_map.dim() ; i++) {
      for (j = 0 ; j < rev_expression_map[i].oprnd_count ; j++) {
	 opernd_to_killed_expr_map[rev_expression_map[i].oprnd[j]].add_tail(i) ;
      }
   }
//   cerr << "Expression kill map done" << endl << flush ;
   
// Initialize the transfer functions for ops

   Expression_data expr_init(expr_counter) ;
   expr_info.reshape(graph.succ_list.dim(), expr_init) ;
   Edge_expression_data edge_expr_init(expr_counter) ;
   edge_expr_info.reshape(edge_graph.src_node_map.dim(), edge_expr_init) ;
//   cerr << "Initialized PRE info arrays" << endl << flush ;

   for (i = 0 ; i < expr_info.dim() ; i++) {
      set_transfer_function(i) ;
   }
//   cerr << "Transfer functions done" << endl << flush ;
   
}

void Avail_expr_solver::solve_availability()
{
   bool not_done = true ;

   int iteration_count = 0  ;
//   cerr << "Starting availability" << endl ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(graph,UP) ; iter != 0 ; iter++) {
	 availability_meet_function(*iter, not_done) ;
	 availability_transfer_function(*iter, not_done) ;
      }
   }
}

void Avail_expr_solver::solve_anticipability()
{
   bool not_done = true ;

   int iteration_count = 0  ;
//   cerr << "Starting anticipability" << endl ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(graph) ; iter != 0 ; iter++) {
	 anticipability_meet_function(*iter, not_done) ;
	 anticipability_transfer_function(*iter, not_done) ;
      }
   }
}

void Avail_expr_solver::solve_earliest()
{
   int edge_size = edge_graph.src_node_map.dim() ;
   for (int i = 0 ; i < edge_size ; i++) {
      Edge_expression_data& ei = edge_expr_info[i] ;
      int src_node = edge_graph.src_node_map[i] ;
      int dest_node = edge_graph.dest_node_map[i] ;

      ei.earliest = expr_info[src_node].av_out ;
      ei.earliest.complement() ;
      ei.earliest *= expr_info[dest_node].ant_in ;
      if (src_node != 0) {
	 Bitvector tmp1(expr_info[src_node].transp) ;
	 tmp1 *= expr_info[src_node].ant_out ;
	 tmp1.complement() ;
	 ei.earliest *= tmp1 ;
      }
   }
}

void Avail_expr_solver::solve_later()
{
   bool not_done = true ;

   int iteration_count = 0  ;
//   cerr << "Starting later solution" << endl ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(graph,UP) ; iter != 0 ; iter++) {
	 int node = *iter ;
	 laterin_meet_function(node, not_done) ;
	 for (List_iterator<int> li(edge_graph.exit_edge_map[node]) ;
	      li != 0 ; li++) {
	    int cur_edge = *li ;
	    later_transfer_function(cur_edge, not_done) ;
	 }
      }
   }
}

void Avail_expr_solver::solve_insert()
{
   int edge_size = edge_graph.src_node_map.dim() ;
   for (int i = 0 ; i < edge_size ; i++) {
      Edge_expression_data& ei = edge_expr_info[i] ;
      int dest_node = edge_graph.dest_node_map[i] ;

      ei.insert = expr_info[dest_node].laterin ;
      ei.insert.complement() ;
      ei.insert *= ei.later ;
   }
}

void Avail_expr_solver::solve_delete()
{
   int node_size = graph.succ_list.dim() ;
   for (int i = 0 ; i < node_size ; i++) {
      Expression_data& ed = expr_info[i] ;
      if (i == 0) {
	 ed.mark_delete.clear() ;
      }
      else {
	 ed.mark_delete = ed.laterin ;
	 ed.mark_delete.complement() ;
	 ed.mark_delete *= ed.antloc ;
      }
   }
}

El_expr* Avail_expr_solver::get_normalized_expression(Op* op)
{
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

void Avail_expr_solver::set_transfer_function(int i)
{
   Expression_data& curop_data = expr_info[i] ;

// Set all the transparent bits, they will be reset selectively below
   curop_data.transp.set() ;
// Set the locally anticipatable bit for the expression
// (this is true irrespective of local definitons
   int new_expression = op_to_expr_map.value(i) ;
   curop_data.antloc.set_bit(new_expression) ;
// Set the computed bit for the local expression. If its inputs
// are overwritten, it will be turned off in the loop below
   curop_data.comp.set_bit(new_expression) ;   
			      
   for (Op_all_dests dest_oper((Op*)graph.b_map[i]) ; dest_oper != 0 ; dest_oper++) {
      Operand* tmpo = &(*dest_oper) ;
      int desti = operand_map.value(*tmpo) ;
      // reset the transparent bit for this expression. If it 
      for (List_iterator<int> li1(opernd_to_killed_expr_map[desti]) ;
	   li1 != 0 ; li1++) {
	 int killed_expression = *li1 ;
	 curop_data.transp.reset_bit(killed_expression) ;
	 if (killed_expression == new_expression) {
	    curop_data.comp.reset_bit(new_expression) ;
	 }
      }
   }
   
}

void Avail_expr_solver::availability_meet_function(int n, bool& tag)
{
   Expression_data &ed = expr_info[n] ;

   Bitvector old_av_in ;

   if (tag == false) {
      old_av_in = ed.av_in ;
   }

   List_iterator<int> li(graph.pred_list[n]) ;
   if (li != 0) {
      ed.av_in = expr_info[*li].av_out ;
      li++ ;
   }
   for ( ; li != 0 ; li++) {
      ed.av_in *= expr_info[*li].av_out ;
   }

   if (tag == false) tag = (ed.av_in != old_av_in) ;
   
   return ;
}

void Avail_expr_solver::availability_transfer_function(int n, bool& tag)
{
   Expression_data &ed = expr_info[n] ;

   Bitvector old_av_out ;

   if (tag == false) old_av_out = ed.av_out ;
   
   ed.av_out = ed.av_in ;
   ed.av_out *= ed.transp ;
   ed.av_out += ed.comp ;

   if (tag == false) {
      tag = (ed.av_out != old_av_out) ;
   }
   
   return ;
}

void Avail_expr_solver::anticipability_meet_function(int n, bool& tag)
{
   Expression_data &ed = expr_info[n] ;

   Bitvector old_ant_out ;

   if (tag == false) {
      old_ant_out = ed.ant_out ;
   }
   List_iterator<int> li(graph.succ_list[n]) ; 
   if (li != 0) {
      ed.ant_out = expr_info[*li].ant_in ;
      li++ ;
   }
   for ( ; li != 0 ; li++) {
      ed.ant_out *= expr_info[*li].ant_in ;
   }

   if (tag == false) tag = (ed.ant_out != old_ant_out) ;
   
   return ;
}

void Avail_expr_solver::anticipability_transfer_function(int n, bool& tag)
{
   Expression_data &ed = expr_info[n] ;

   Bitvector old_ant_in ;

   if (tag == false) old_ant_in = ed.ant_in ;
   
   ed.ant_in = ed.ant_out ;
   ed.ant_in *= ed.transp ;
   ed.ant_in += ed.antloc ;

   if (tag == false) {
      tag = (ed.ant_in != old_ant_in) ;
   }
   
   return ;
}

void Avail_expr_solver::laterin_meet_function(int op_num, bool& tag)
{
   Expression_data &ed = expr_info[op_num] ;

   Bitvector old_laterin ;

   if (tag == false) {
      old_laterin = ed.laterin ;
   }

   List_iterator<int> li(edge_graph.entry_edge_map[op_num]) ;
   if (li != 0) {
      ed.laterin = edge_expr_info[*li].later ;
      li++ ;
   }
   for ( ; li != 0 ; li++) {
      ed.laterin *= edge_expr_info[*li].later ;
   }

   if (tag == false) tag = (ed.laterin != old_laterin) ;
   
   return ;
}

void Avail_expr_solver::later_transfer_function(int edge_num, bool& tag) 
{
   Edge_expression_data &ed = edge_expr_info[edge_num] ;
   int src_node = edge_graph.src_node_map[edge_num] ;

   Bitvector old_later;

   if (tag == false) {
      old_later = ed.later ;
   }

   ed.later = expr_info[src_node].antloc ;
   ed.later.complement() ;
   ed.later *= expr_info[src_node].laterin ;
   ed.later += ed.earliest ;
   
   if (tag == false) tag = (ed.later != old_later) ;
   
   return ;
}


void Avail_expr_solver::print_marked_for_deletion() 
{
   int node_size = graph.succ_list.dim() ;
   Bitvector all_clear(expr_info[0].mark_delete.size()) ;
   
   for (int i = 0 ; i < node_size ; i++) {
      Expression_data& ed = expr_info[i] ;
      if (ed.mark_delete != all_clear) {
	 if (!is_redundant(i)) {
	    cout << *(graph.b_map[i]) ;
	 }
      }
   }
}

bool Avail_expr_solver::is_redundant(int i)
{
   Op* cur_op = (Op *) graph.b_map[i] ;
   
   Opcode cur_opcode = cur_op->opcode() ;
   if (is_define(cur_opcode)) {
      return true ;
   }
   if (is_pbr(cur_opcode)) {
      return true ;
   }
   if (is_cmpp(cur_opcode)) {
      return true ;
   }
   if (is_memory(cur_opcode)) {
      return true ;
   }
   if (is_move(cur_opcode)) {
      return true ;
   }
   if ((cur_op->num_dests() > 0) && (cur_op->dest(DEST1).is_macro_reg())) {
      return true ;
   }
   if (is_int_add(cur_op)) {
      List_iterator<int> li1(graph.succ_list[i]) ;
      int succ_index = *li1 ;
      Op* next_op = (Op *) graph.b_map[succ_index] ;
      if (is_memory(next_op) && (cur_op->dest(DEST1) == next_op->src(SRC1))) {
	 return true ;
      }
   }
   return false ;
}

