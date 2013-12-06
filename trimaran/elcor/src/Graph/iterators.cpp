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




////////////////////////////////////////////////////////////////////////////
//
//      File:           iterators.cpp
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson
//                      Santosh Abraham
//      Created:        December 1994
//      Description:    Iterators for the IR structures
//
/////////////////////////////////////////////////////////////////////////////

#include "filter.h"
#include "iterators.h"
#include "region_utilities.h"
#include "edge_utilities.h"
#include "opcode_properties.h"
#include "attributes.h"
#include "dbg.h"
#include "region_attributes.h"
#include "op.h"
#include "edge.h"
#include "hash_functions.h"

#include "mdes.h"

/* ------ Class Op_inedges */

Op_inedges::Op_inedges(){}

Op_inedges::Op_inedges(const Op* op, const Edge_filter* filter)
  :List_filterator<Edge*>(op->edges_in, filter) {}
  
Op_inedges::Op_inedges(const Op* op, Port_type type, Port_num num)
  :List_filterator<Edge*>(op->edges_in, 
			 new Inedge_port_filter(type, num), true) {}

Op_inedges::Op_inedges(const Op* op, Port_type type, Port_num num, Alt_num alt)
  :List_filterator<Edge*>(op->edges_in, 
			 new Inedge_port_filter(type, num, alt), true) {}

void Op_inedges::operator()(const Op* op, const Edge_filter* filter)
{
  List_filterator<Edge*>::operator()(op->edges_in, filter);
}

Op_inedges::~Op_inedges(){}


/* ------ Class Op_inedges_rel */

Op_inedges_rel::Op_inedges_rel(){}

Op_inedges_rel::Op_inedges_rel(const Op* op, const Edge_filter* filter)
: edi(op,filter), opi(((Op*) op)->get_relops()), efilter_rep(filter)
{
   check_initial_state() ;
}

void Op_inedges_rel::operator()(const Op* op, const Edge_filter* filter)
{
   efilter_rep = filter;
   edi(op,filter) ;
   opi(((Op*) op)->get_relops()) ;
   check_initial_state() ;
}

Op_inedges_rel::~Op_inedges_rel(){}

bool Op_inedges_rel::operator==(const void*) const
{
   return(edi == 0) ;
}

bool Op_inedges_rel::operator!=(const void*) const
{
   return(edi != 0) ;
}

void Op_inedges_rel::operator++() 
{
   advance() ;
}

void Op_inedges_rel::operator++(int) 
{
   advance() ;
}

Edge* Op_inedges_rel::operator*() 
{
   return(*edi) ;
}

void Op_inedges_rel::check_initial_state() 
{
   if (edi == 0) {
      advance() ;
   }
}

void Op_inedges_rel::advance()
{
   edi++ ;
   if (edi != 0) return ;
   else { // edi == 0 
      while(1) {
         if (opi == 0) return ;
         Op* tmpop = *opi ;
         edi(tmpop, efilter_rep) ;
         opi++ ;
         if (edi != 0) {
            return ;
         }
      }
   }
}

/* -------- Class Op_outedges */

Op_outedges::Op_outedges(){}

Op_outedges::Op_outedges(const Op* op, const Edge_filter* filter)
  :List_filterator<Edge*>(op->edges_out, filter) {}
  
Op_outedges::Op_outedges(const Op* op, Port_type type, Port_num num)
  :List_filterator<Edge*>(op->edges_out, 
			 new Outedge_port_filter(type, num), true) {}

Op_outedges::Op_outedges(const Op* op, Port_type type, Port_num num, 
	                 Alt_num alt)
  :List_filterator<Edge*>(op->edges_out, 
			 new Outedge_port_filter(type, num, alt), true) {}
void Op_outedges::operator()(const Op* op, const Edge_filter* filter)
{
  List_filterator<Edge*>::operator()(op->edges_out, filter);
}

Op_outedges::~Op_outedges(){}

/* ------ Class Op_outedges_rel */

Op_outedges_rel::Op_outedges_rel(){}

Op_outedges_rel::Op_outedges_rel(const Op* op, const Edge_filter* filter)
: edi(op,filter), opi(((Op*) op)->get_relops()), efilter_rep(filter)
{
   check_initial_state() ;
}

void Op_outedges_rel::operator()(const Op* op, const Edge_filter* filter)
{
   efilter_rep = filter;
   edi(op,filter) ;
   opi(((Op*) op)->get_relops()) ;
   check_initial_state() ;
}

Op_outedges_rel::~Op_outedges_rel(){}

bool Op_outedges_rel::operator==(const void*) const 
{
   return(edi == 0) ;
}

bool Op_outedges_rel::operator!=(const void*) const
{
   return(edi != 0) ;
}

void Op_outedges_rel::operator++() 
{
   advance() ;
}

void Op_outedges_rel::operator++(int) 
{
   advance() ;
}

Edge* Op_outedges_rel::operator*() 
{
   return(*edi) ;
}

void Op_outedges_rel::check_initial_state() 
{
   if (edi == 0) {
      advance() ;
   }
}

void Op_outedges_rel::advance()
{
   edi++ ;
   if (edi != 0) return ;
   else { // edi == 0 
      while(1) {
         if (opi == 0) return ;
         Op* tmpop = *opi ;
         edi(tmpop, efilter_rep) ;
         opi++ ;
         if (edi != 0) {
            return ;
         }
      }
   }
}

/* -------- Class Op_pred */

Op_pred::Op_pred() : pred_set(hash_op_ptr, 16) {}

Op_pred::Op_pred(const Op* op, const Edge_filter* edge_filter,
	         const Op_filter* op_filter)
   : pred_set(hash_op_ptr, 16)
{
  for(Op_inedges iter(op, edge_filter); iter != 0; ++iter)
    pred_set += (*iter)->src();
  Hash_set_filterator<Op*>::operator()(pred_set, op_filter);
}

Op_pred::Op_pred(const Op* op, Port_type type, Port_num num, 
		 const Op_filter* op_filter)
   : pred_set(hash_op_ptr, 16)
{
  for(Op_inedges iter(op, type, num); iter != 0; ++iter)
    pred_set += (*iter)->src();
  Hash_set_filterator<Op*>::operator()(pred_set, op_filter);
}

Op_pred::Op_pred(const Op* op, Port_type type, Port_num num, 
		 Alt_num alt, const Op_filter* op_filter)
   : pred_set(hash_op_ptr, 16)
{
  for(Op_inedges iter(op, type, num, alt); iter != 0; ++iter)
    pred_set += (*iter)->src();
  Hash_set_filterator<Op*>::operator()(pred_set, op_filter);
}

void Op_pred::operator()(const Op* op, const Edge_filter* edge_filter,
		    const Op_filter* op_filter)
{
   pred_set.clear() ;
   for(Op_inedges iter(op, edge_filter); iter != 0; ++iter)
      pred_set += (*iter)->src();
   Hash_set_filterator<Op*>::operator()(pred_set, op_filter);
}

Op_pred::~Op_pred(){}


/* -------- Class Op_ancestors */

Op_ancestors::Op_ancestors() : ancestor_set(hash_op_ptr, 128) {}

Op_ancestors::Op_ancestors(const Op* op, const Compound_region* reg,
		const Edge_filter* edge_filter,
	        const Op_filter* op_filter)
   : ancestor_set(hash_op_ptr, 128)
{
  init_ancestor_set(op, reg, edge_filter);
  Hash_set_filterator<Op*>::operator()(ancestor_set, op_filter);
}

Op_ancestors::Op_ancestors(const Op* op, const Compound_region* reg,
		Port_type type, Port_num num, 
		const Op_filter* op_filter)
   : ancestor_set(hash_op_ptr, 128)
{
  Edge_filter* efilt = new Inedge_port_filter(type, num);
  init_ancestor_set(op, reg, efilt);
  delete efilt;
  Hash_set_filterator<Op*>::operator()(ancestor_set, op_filter);
}

Op_ancestors::Op_ancestors(const Op* op, const Compound_region* reg,
		Port_type type, Port_num num, 
		Alt_num alt, const Op_filter* op_filter)
   : ancestor_set(hash_op_ptr, 128)
{
  Edge_filter* efilt = new Inedge_port_filter(type, num, alt);
  init_ancestor_set(op, reg, efilt);
  delete efilt;
  Hash_set_filterator<Op*>::operator()(ancestor_set, op_filter);
}

void Op_ancestors::operator()(const Op* op, const Compound_region* reg,
		const Edge_filter* edge_filter,
		const Op_filter* op_filter)
{
  init_ancestor_set(op, reg, edge_filter);
  Hash_set_filterator<Op*>::operator()(ancestor_set, op_filter);
}

Op_ancestors::~Op_ancestors(){}

void Op_ancestors::init_ancestor_set(const Op* op, const Compound_region* reg,
			const Edge_filter* efilt)
{
  List<Op*> visit_list;
  visit_list.add_tail(real_op((Op*) op));
  while (!visit_list.is_empty()) {
    Op* visit_op = visit_list.pop();
    for (Op_inedges_rel eiter(visit_op, efilt); eiter != 0; eiter++) {
      if (((Compound_region*) reg)->is_entry_edge(*eiter)) continue;
      Op* pred = real_op((*eiter)->src());
      if (ancestor_set.is_member(pred)) continue;
      assert(region_contains_op((Region*) reg, pred));
      visit_list.add_tail(pred);
      ancestor_set += pred;
    }
  }
}

/* ------------- Class Op_succ */

Op_succ::Op_succ() : succ_set(hash_op_ptr, 16) {}

Op_succ::Op_succ(const Op* op, const Edge_filter* edge_filter,
	         const Op_filter* op_filter)
   : succ_set(hash_op_ptr, 16)
{
  for(Op_outedges iter(op, edge_filter); iter != 0; ++iter)
    succ_set += (*iter)->dest();
  Hash_set_filterator<Op*>::operator()(succ_set, op_filter);
}

Op_succ::Op_succ(const Op* op, Port_type type, Port_num num, 
		 const Op_filter* op_filter)
   : succ_set(hash_op_ptr, 16)
{
  for(Op_outedges iter(op, type, num); iter != 0; ++iter)
    succ_set += (*iter)->dest();
  Hash_set_filterator<Op*>::operator()(succ_set, op_filter);
}

Op_succ::Op_succ(const Op* op, Port_type type, Port_num num, 
		 Alt_num alt, const Op_filter* op_filter)
   : succ_set(hash_op_ptr, 16)
{
  for(Op_outedges iter(op, type, num, alt); iter != 0; ++iter)
    succ_set += (*iter)->dest();
  Hash_set_filterator<Op*>::operator()(succ_set, op_filter);
}

void Op_succ::operator()(const Op* op, const Edge_filter* edge_filter,
			 const Op_filter* op_filter)
{
   succ_set.clear() ;
   for(Op_outedges iter(op, edge_filter); iter != 0; ++iter)
      succ_set += (*iter)->dest();
   Hash_set_filterator<Op*>::operator()(succ_set, op_filter);
}

Op_succ::~Op_succ(){}

/* -------- Class Op_descendants */

Op_descendants::Op_descendants() : descendant_set(hash_op_ptr, 128) {}

Op_descendants::Op_descendants(const Op* op, const Compound_region* reg,
		const Edge_filter* edge_filter,
	        const Op_filter* op_filter)
   : descendant_set(hash_op_ptr, 128)
{
  init_descendant_set(op, reg, edge_filter);
  Hash_set_filterator<Op*>::operator()(descendant_set, op_filter);
}

Op_descendants::Op_descendants(const Op* op, const Compound_region* reg,
		Port_type type, Port_num num, 
		const Op_filter* op_filter)
   : descendant_set(hash_op_ptr, 128)
{
  Edge_filter* efilt = new Outedge_port_filter(type, num);
  init_descendant_set(op, reg, efilt);
  delete efilt;
  Hash_set_filterator<Op*>::operator()(descendant_set, op_filter);
}

Op_descendants::Op_descendants(const Op* op, const Compound_region* reg,
		Port_type type, Port_num num, 
		Alt_num alt, const Op_filter* op_filter)
   : descendant_set(hash_op_ptr, 128)
{
  Edge_filter* efilt = new Outedge_port_filter(type, num, alt);
  init_descendant_set(op, reg, efilt);
  delete efilt;
  Hash_set_filterator<Op*>::operator()(descendant_set, op_filter);
}

void Op_descendants::operator()(const Op* op, const Compound_region* reg,
		const Edge_filter* edge_filter,
		const Op_filter* op_filter)
{
  init_descendant_set(op, reg, edge_filter);
  Hash_set_filterator<Op*>::operator()(descendant_set, op_filter);
}

Op_descendants::~Op_descendants(){}

void Op_descendants::init_descendant_set(const Op* op,
	const Compound_region* reg, const Edge_filter* efilt)
{
  List<Op*> visit_list;
  visit_list.add_tail(real_op((Op*) op));
  while (!visit_list.is_empty()) {
    Op* visit_op = visit_list.pop();
    for (Op_outedges_rel eiter(visit_op, efilt); eiter != 0; eiter++) {
      if (((Compound_region*) reg)->is_exit_edge(*eiter)) continue;
      Op* succ = real_op((*eiter)->dest());
      if (descendant_set.is_member(succ)) continue;
      assert(region_contains_op((Region*) reg, succ));
      visit_list.add_tail(succ);
      descendant_set += succ;
    }
  }
}

///////////////////////////////////////////////////////////////////
// Iterators for op sources
///////////////////////////////////////////////////////////////////

// Class Op_predicate_sources

Op_predicate_sources::Op_predicate_sources()
   : cur_op(0), start_port(0),  end_port(0)
{}

Op_predicate_sources::Op_predicate_sources(const Op* op)
   : cur_op((Op*)op), start_port(op->first_input()), end_port(op->first_src())
{}

void Op_predicate_sources::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   start_port = op->first_input() ;
   end_port = op->first_src() ;
}

Op_predicate_sources::Op_predicate_sources(const Op_predicate_sources& os)
   : cur_op(os.cur_op), start_port(os.start_port), end_port(os.end_port) 
{}

Op_predicate_sources& Op_predicate_sources::operator=
                           (const Op_predicate_sources& os)
{
   cur_op = os.cur_op ;
   start_port = os.start_port ;
   end_port = os.end_port ;
   return *this ;
}

Op_predicate_sources::~Op_predicate_sources()
{}

bool Op_predicate_sources::operator==(const void* i) const
{
   assert(i == NULL) ;
   if (start_port >= end_port) return true ;
   else return false ;
}

bool Op_predicate_sources::operator!=(const void* i) const
{
   assert(i == NULL) ;
   if (start_port < end_port) return true ;
   else return false ;
}

void Op_predicate_sources::operator++()
{
   start_port++ ;
}

void Op_predicate_sources::operator++(int)
{
   start_port++ ;
}

Operand& Op_predicate_sources::operator*()
{
   return cur_op->src(start_port) ;
}

El_ref_type Op_predicate_sources::ref_type() 
{
   return (PRED_SRC) ;
}

Port_num Op_predicate_sources::get_port_num() 
{
   return (start_port) ;
}

Op* Op_predicate_sources::get_op() 
{
   return (cur_op) ;
}

El_ref Op_predicate_sources::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Class Op_explicit_sources

Op_explicit_sources::Op_explicit_sources()
   : cur_op(0), start_port(0),  end_port(0)
{}

Op_explicit_sources::Op_explicit_sources(const Op* op)
   : cur_op((Op*)op), start_port(op->first_src()), end_port(op->last_src())
{}

void Op_explicit_sources::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   start_port = op->first_src() ;
   end_port = op->last_src() ;
}

Op_explicit_sources::Op_explicit_sources(const Op_explicit_sources& os)
   : cur_op(os.cur_op), start_port(os.start_port), end_port(os.end_port) 
{}

Op_explicit_sources& Op_explicit_sources::operator=
                           (const Op_explicit_sources& os)
{
   cur_op = os.cur_op ;
   start_port = os.start_port ;
   end_port = os.end_port ;
   return *this ;
}

Op_explicit_sources::~Op_explicit_sources()
{}

bool Op_explicit_sources::operator==(const void* i) const
{
   assert(i == NULL) ;
   if (start_port > end_port) return true ;
   else return false ;
}

bool Op_explicit_sources::operator!=(const void* i) const
{
   assert(i == NULL) ;
   if (start_port <= end_port) return true ;
   else return false ;
}

void Op_explicit_sources::operator++()
{
   start_port++ ;
}

void Op_explicit_sources::operator++(int)
{
   start_port++ ;
}

Operand& Op_explicit_sources::operator*()
{
   return cur_op->src(start_port) ;
}

El_ref_type Op_explicit_sources::ref_type() 
{
   return (EXP_SRC) ;
}

Port_num Op_explicit_sources::get_port_num() 
{
   return (start_port) ;
}

Op* Op_explicit_sources::get_op() 
{
   return (cur_op) ;
}

El_ref Op_explicit_sources::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Op implicit sources
// tr_aux string added SAM 7-03.  These represent auxiliary through
// register parameters.  FP args passed through reg are also passed
// via the integer regs to deal with varargs functions properly.

static void implicit_call_return_use_list_init(List<Operand>* u, const Op* op)
{
  static eString tr_string("tr") ;
  static eString tr_aux_string("tr_aux") ;

  Lcode_attribute_map* lam = get_lcode_attributes((Op*)op) ;
  if (lam->is_bound(tr_string)) {
    List<Operand>* more_uses = &lam->value(tr_string);
    for (List_iterator<Operand> li(*more_uses) ; li !=0 ; li++) {
      if( ! u->is_member(*li))
	u->add_tail(*li) ;
    }
  }
  if (lam->is_bound(tr_aux_string)) {
    List<Operand>* more_uses = &lam->value(tr_aux_string);
    for (List_iterator<Operand> li(*more_uses) ; li !=0 ; li++) {
      if( ! u->is_member(*li))
	u->add_tail(*li) ;
    }
  }
}

// literal move/shift ops have an implicit source, which is the same as dest1
static void implicit_uses_for_literal_move_with_shift(List<Operand>* u,
						      const Op* op) {
  Operand oper = op->dest(DEST1);
  if( ! u->is_member(oper))
    u->add_tail(oper);
}

static void implicit_save_restore_list_fill(List<Operand>* u, int lb, int ub) {
   static eString pred_file_name(MDES_translate_vreg("P")) ;
   for (int i = lb ; i <= ub ; i++) {
      Reg* new_reg = new Reg(EL_STANDARD_PREDICATE, PR, 0) ;
      new_reg->bind_physical_file(pred_file_name) ;
      new_reg->bind_reg(i) ;
      Operand* new_operand = new Operand(new_reg) ;
      if( ! u->is_member(*new_operand))
	u->add_tail(*new_operand);
   }
}

static void implicit_uses_for_save(List<Operand>* u, const Op* op) {
   assert((op->src(SRC2)).is_macro_reg()) ;
   Macro_name cr = (op->src(SRC2)).name() ;
   if (cr == PV_0) {
      implicit_save_restore_list_fill(u, 0, 31);
   }
   else if (cr == PV_1) {
      implicit_save_restore_list_fill(u, 32, 63);
   }
   else if (cr == PV_2) {
      implicit_save_restore_list_fill(u, 64, 95);
   }
   else if (cr == PV_3) {
      implicit_save_restore_list_fill(u, 96, 127);
   }
   else if (cr == PV_4) {
      implicit_save_restore_list_fill(u, 128, 159);
   }
   else if (cr == PV_5) {
      implicit_save_restore_list_fill(u, 160, 191);
   }
   else if (cr == PV_6) {
      implicit_save_restore_list_fill(u, 192, 223);
   }
   else if (cr == PV_7) {
      implicit_save_restore_list_fill(u, 224, 255);
   }
}

Op_implicit_sources::Op_implicit_sources()
   : cur_op(0)
{}

Op_implicit_sources::Op_implicit_sources(const Op* op)
: cur_op((Op*)op)
{
   if (is_call(cur_op) || is_return(cur_op)) {
      List<Operand>* uses = get_implicit_use_list(cur_op);
      if (uses == NULL) {
         uses = new List<Operand>();
      }
      implicit_call_return_use_list_init(uses, op) ;
      set_implicit_use_list(cur_op,uses) ;
      lics(*uses) ;
   }
   else if (is_literal_move_with_shift(cur_op)) {
      List<Operand>* uses = get_implicit_use_list(cur_op);
      if (uses == NULL) {
         uses = new List<Operand>();
      }
      implicit_uses_for_literal_move_with_shift(uses, op) ;
      set_implicit_use_list(cur_op,uses) ;
      lics(*uses) ;
   }
   else if ((is_save(cur_op)) && ((cur_op->src(SRC2)).is_macro_reg())) {
      List<Operand>* uses = get_implicit_use_list(cur_op);
      if (uses == NULL) {
         uses = new List<Operand>();
      }
      implicit_uses_for_save(uses, op) ;
      set_implicit_use_list(cur_op,uses) ;
      lics(*uses) ;
   }
}

void Op_implicit_sources::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   if (is_call(cur_op) || is_return(cur_op)) {
      List<Operand>* uses = get_implicit_use_list(cur_op);
      if (uses == NULL) {
         uses = new List<Operand>();
      } 
      implicit_call_return_use_list_init(uses, op) ;
      set_implicit_use_list(cur_op,uses) ;
      lics(*uses) ;
   }
   else if (is_literal_move_with_shift(cur_op)) {
      List<Operand>* uses = get_implicit_use_list(cur_op);
      if (uses == NULL) {
         uses = new List<Operand>();
      }
      implicit_uses_for_literal_move_with_shift(uses, op) ;
      set_implicit_use_list(cur_op,uses) ;
      lics(*uses) ;
   }
   else if ((is_save(cur_op)) && ((cur_op->src(SRC2)).is_macro_reg())) {
      List<Operand>* uses = get_implicit_use_list(cur_op);
      if (uses == NULL) {
         uses = new List<Operand>();
      }
      implicit_uses_for_save(uses, op) ;
      set_implicit_use_list(cur_op,uses) ;
      lics(*uses) ;
   }
}

Op_implicit_sources::Op_implicit_sources(const Op_implicit_sources& os)
   : cur_op(os.cur_op), lics(os.lics) 
{}

Op_implicit_sources& Op_implicit_sources::operator=
                           (const Op_implicit_sources& os)
{
   cur_op = os.cur_op ;
   lics = os.lics ;
   return *this ;
}

Op_implicit_sources::~Op_implicit_sources()
{}

bool Op_implicit_sources::operator==(const void* i) const
{
   return (lics == i) ;
}

bool Op_implicit_sources::operator!=(const void* i) const
{
   return (lics != i) ;
}

void Op_implicit_sources::operator++()
{
   lics++ ;
}

void Op_implicit_sources::operator++(int)
{
   lics++ ;
}

Operand& Op_implicit_sources::operator*()
{
   return (*lics) ;
}

El_ref_type Op_implicit_sources::ref_type() 
{
   return (IMP_SRC) ;
}

Port_num Op_implicit_sources::get_port_num() 
{
   return (SRC1) ;
}

Op* Op_implicit_sources::get_op() 
{
   return (cur_op) ;
}

El_ref Op_implicit_sources::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Op taken liveout sources

Op_taken_liveout_sources::Op_taken_liveout_sources()
{}

Op_taken_liveout_sources::Op_taken_liveout_sources(const Op* op)
{
  static eString taken_exit_pseudo_uses("taken_exit_pseudo_uses") ;
  
  cur_op = (Op*)op ;
  if (is_control_switch(cur_op)) {
    List<Operand>* uses = 
      (List<Operand>*) get_generic_attribute(cur_op,taken_exit_pseudo_uses) ;
    if (uses != NULL) {
      lics(*uses) ;
    }
  }
}

void Op_taken_liveout_sources::operator()(const Op* op)
{
  static eString taken_exit_pseudo_uses("taken_exit_pseudo_uses") ;

  cur_op = (Op*)op ;
  if (is_control_switch(cur_op)) {
    List<Operand>* uses = 
      (List<Operand>*) get_generic_attribute(cur_op,taken_exit_pseudo_uses) ;
    if (uses != NULL) {
      lics(*uses) ;
    }
  }
}

El_ref_type Op_taken_liveout_sources::ref_type() 
{
   return (LIVEOUT_TAKEN_SRC) ;
}

El_ref Op_taken_liveout_sources::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Op fallthrough liveout sources

Op_fallthrough_liveout_sources::Op_fallthrough_liveout_sources()
{}

Op_fallthrough_liveout_sources::Op_fallthrough_liveout_sources(const Op* op)
{
  static eString fallthrough_exit_pseudo_uses("fallthrough_exit_pseudo_uses") ;

   cur_op = (Op*)op ;
   if (is_control_switch(cur_op)) {
      List<Operand>* uses = 
         (List<Operand>*) get_generic_attribute(cur_op,fallthrough_exit_pseudo_uses) ;
      if (uses != NULL) {
         lics(*uses) ;
      }
   }
}

void Op_fallthrough_liveout_sources::operator()(const Op* op)
{
  static eString fallthrough_exit_pseudo_uses("fallthrough_exit_pseudo_uses") ;

   cur_op = (Op*)op ;
   if (is_control_switch(cur_op)) {
      List<Operand>* uses = 
         (List<Operand>*) get_generic_attribute(cur_op,fallthrough_exit_pseudo_uses) ;
      if (uses != NULL) {
         lics(*uses) ;
      }
   }
}

El_ref_type Op_fallthrough_liveout_sources::ref_type() 
{
   return (LIVEOUT_FALLTHROUGH_SRC) ;
}

El_ref Op_fallthrough_liveout_sources::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Op mem sources

Op_mem_sources::Op_mem_sources()
   : cur_op(0)
{}

Op_mem_sources::Op_mem_sources(const Op* op)
   : cur_op((Op*)op)
{
   lims(cur_op->mem_srcs()) ;
}

void Op_mem_sources::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   lims(cur_op->mem_srcs()) ;
}

Op_mem_sources::Op_mem_sources(const Op_mem_sources& os)
   : cur_op(os.cur_op), lims(os.lims) 
{}

Op_mem_sources& Op_mem_sources::operator=
                           (const Op_mem_sources& os)
{
   cur_op = os.cur_op ;
   lims = os.lims ;
   return *this ;
}

Op_mem_sources::~Op_mem_sources()
{}

bool Op_mem_sources::operator==(const void* i) const
{
   return (lims == i) ;
}

bool Op_mem_sources::operator!=(const void* i) const
{
   return (lims != i) ;
}

void Op_mem_sources::operator++()
{
   lims++ ;
}

void Op_mem_sources::operator++(int)
{
   lims++ ;
}

Operand& Op_mem_sources::operator*()
{
   return (*lims) ;
}

El_ref_type Op_mem_sources::ref_type() 
{
   return (MEM_SRC) ;
}

Port_num Op_mem_sources::get_port_num() 
{
   return (MEM0) ;
}

Op* Op_mem_sources::get_op() 
{
   return (cur_op) ;
}

El_ref Op_mem_sources::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Op live out sources

Op_live_sources::Op_live_sources()
   : cur_op(0)
{}

Op_live_sources::Op_live_sources(const Op* op)
   : cur_op((Op*)op), edge_iter(op, CONTROL0_OUTEDGES)
{
   init_edge_iter() ;
   //advance() ;
}

void Op_live_sources::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   edge_iter(op, CONTROL0_OUTEDGES) ;
   init_edge_iter() ;
   //advance() ;
}

Op_live_sources::Op_live_sources(const Op_live_sources& os)
   : cur_op(os.cur_op), edge_iter(os.edge_iter), lims(os.lims) 
{}

Op_live_sources& Op_live_sources::operator=
                           (const Op_live_sources& os)
{
   cur_op = os.cur_op ;
   edge_iter = os.edge_iter ;
   lims = os.lims ;
   return *this ;
}

Op_live_sources::~Op_live_sources()
{}

bool Op_live_sources::operator==(const void* i) const
{
   return (lims == i) ;
}

bool Op_live_sources::operator!=(const void* i) const
{
   return (lims != i) ;
}

void Op_live_sources::operator++()
{
   lims++ ;
   if (lims == 0) {
      advance() ;
   }
}

void Op_live_sources::operator++(int)
{
   lims++ ;
   if (lims == 0) {
      advance() ;
   }
}

Operand& Op_live_sources::operator*()
{
   return (*lims) ;
}

El_ref_type Op_live_sources::ref_type() 
{
   if (is_fall_through(*edge_iter)) {
      return (LIVEOUT_FALLTHROUGH_SRC) ;
   }
   else {
      return (LIVEOUT_TAKEN_SRC) ;
   }
}

Port_num Op_live_sources::get_port_num() 
{
   return (SRC1) ;
}

Op* Op_live_sources::get_op() 
{
   return (cur_op) ;
}

void Op_live_sources::init_edge_iter()
{
   while (edge_iter != 0) {
      Edge* tmpe = *edge_iter ;
      Liveness_info* lo = get_liveness_info(tmpe) ;
      if (lo) {
        lims(*lo) ;
        if (lims != 0) {
          return;
        }
      }
      edge_iter++ ;
   }
}

void Op_live_sources::advance()
{
   while (1) {
      edge_iter++ ;
      if (edge_iter == 0) return ;
      Edge* tmpe = *edge_iter ;
      Liveness_info* lo = get_liveness_info(tmpe) ;
      if (lo) {
	 lims(*lo) ;
	 if (lims != 0) {
	    return ;
	 }
      }
   }
}

El_ref Op_live_sources::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

///////////////////////////////////////////////////////////////////
// Iterators for op dests
///////////////////////////////////////////////////////////////////

// Class Op_explicit_dests

Op_explicit_dests::Op_explicit_dests()
   : cur_op(0), start_port(0),  end_port(0)
{}

Op_explicit_dests::Op_explicit_dests(const Op* op)
   : cur_op((Op*)op), start_port(op->first_dest()), end_port(op->last_dest())
{}

void Op_explicit_dests::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   start_port = op->first_dest() ;
   end_port = op->last_dest() ;
}

Op_explicit_dests::Op_explicit_dests(const Op_explicit_dests& os)
   : cur_op(os.cur_op), start_port(os.start_port), end_port(os.end_port) 
{}

Op_explicit_dests& Op_explicit_dests::operator=
                           (const Op_explicit_dests& os)
{
   cur_op = os.cur_op ;
   start_port = os.start_port ;
   end_port = os.end_port ;
   return *this ;
}

Op_explicit_dests::~Op_explicit_dests()
{}

bool Op_explicit_dests::operator==(const void* i) const
{
   assert(i == NULL) ;
   if (start_port > end_port) return true ;
   else return false ;
}

bool Op_explicit_dests::operator!=(const void* i) const
{
   assert(i == NULL) ;
   if (start_port <= end_port) return true ;
   else return false ;
}

void Op_explicit_dests::operator++()
{
   start_port++ ;
}

void Op_explicit_dests::operator++(int)
{
   start_port++ ;
}

Operand& Op_explicit_dests::operator*()
{
   return cur_op->dest(start_port) ;
}

El_ref_type Op_explicit_dests::ref_type() 
{
   return (EXP_DEST) ;
}

Port_num Op_explicit_dests::get_port_num() 
{
   return (start_port) ;
}

Op* Op_explicit_dests::get_op() 
{
   return (cur_op) ;
}

El_ref Op_explicit_dests::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Op implicit dests


static void implicit_call_return_def_list_init(List<Operand>* u, const Op* op)
{
   if (current_MDES->is_macro_present(el_macro_to_string_map.value(SPILL_TEMPREG))) 
     u->add_tail(new Macro_reg(SPILL_TEMPREG));

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(INT_RETURN))) 
     u->add_tail(new Macro_reg(INT_RETURN)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(INT_PARAM_1))) 
     u->add_tail(new Macro_reg(INT_PARAM_1)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(INT_PARAM_2))) 
     u->add_tail(new Macro_reg(INT_PARAM_2)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(INT_PARAM_3))) 
     u->add_tail(new Macro_reg(INT_PARAM_3)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(INT_PARAM_4))) 
     u->add_tail(new Macro_reg(INT_PARAM_4)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(FLT_RETURN))) 
     u->add_tail(new Macro_reg(FLT_RETURN)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(FLT_PARAM_1))) 
     u->add_tail(new Macro_reg(FLT_PARAM_1)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(FLT_PARAM_2))) 
     u->add_tail(new Macro_reg(FLT_PARAM_2)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(FLT_PARAM_3))) 
     u->add_tail(new Macro_reg(FLT_PARAM_3)) ;	 

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(FLT_PARAM_4))) 
     u->add_tail(new Macro_reg(FLT_PARAM_4)) ;	 

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(DBL_RETURN))) 
     u->add_tail(new Macro_reg(DBL_RETURN)) ;	 

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(DBL_PARAM_1))) 
     u->add_tail(new Macro_reg(DBL_PARAM_1)) ;	 

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(DBL_PARAM_2))) 
     u->add_tail(new Macro_reg(DBL_PARAM_2)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(DBL_PARAM_3))) 
     u->add_tail(new Macro_reg(DBL_PARAM_3)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(DBL_PARAM_4))) 
     u->add_tail(new Macro_reg(DBL_PARAM_4)) ;
   // SAM 10-2002, added SP_REG to fix bug with caller code being
   // moved across BRLs in postpass sched.  Note, not sure why I
   // cannot add the SP aliases, but it prodced an error in reg file
   // binding.  Need to ask Mike about this.

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(SP_REG))) 
     u->add_tail(new Macro_reg(SP_REG)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(FP_REG))) 
     u->add_tail(new Macro_reg(FP_REG)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(IP_REG))) 
     u->add_tail(new Macro_reg(IP_REG)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(OP_REG))) 
     u->add_tail(new Macro_reg(OP_REG)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(LV_REG))) 
     u->add_tail(new Macro_reg(LV_REG)) ;

   if (current_MDES->is_macro_present(el_macro_to_string_map.value(RGS_REG))) 
     u->add_tail(new Macro_reg(RGS_REG)) ;
}

static void implicit_defs_for_restore(List<Operand>* u, const Op* op) {
   assert((op->dest(DEST1)).is_macro_reg()) ;
   Macro_name cr = (op->dest(DEST1)).name() ;
   if (cr == PV_0) {
      implicit_save_restore_list_fill(u, 0, 31);
   }
   else if (cr == PV_1) {
      implicit_save_restore_list_fill(u, 32, 63);
   }
   else if (cr == PV_2) {
      implicit_save_restore_list_fill(u, 64, 95);
   }
   else if (cr == PV_3) {
      implicit_save_restore_list_fill(u, 96, 127);
   }
   else if (cr == PV_4) {
      implicit_save_restore_list_fill(u, 128, 159);
   }
   else if (cr == PV_5) {
      implicit_save_restore_list_fill(u, 160, 191);
   }
   else if (cr == PV_6) {
      implicit_save_restore_list_fill(u, 192, 223);
   }
   else if (cr == PV_7) {
      implicit_save_restore_list_fill(u, 224, 255);
   }
}

Op_implicit_dests::Op_implicit_dests()
   : cur_op(0)
{}

Op_implicit_dests::Op_implicit_dests(const Op* op)
   : cur_op((Op*)op)
{
   if (is_call(cur_op)) {
      List<Operand>* defs = get_implicit_def_list(cur_op) ;
      if (defs == NULL) {
	 defs = new List<Operand>() ;
	 implicit_call_return_def_list_init(defs, op) ;
	 set_implicit_def_list(cur_op,defs) ;
      }
      lics(*defs) ;
   }
   else if ((is_restore(cur_op)) && ((cur_op->dest(DEST1)).is_macro_reg())){
      List<Operand>* defs = get_implicit_def_list(cur_op);
      if (defs == NULL) {
         defs = new List<Operand>();
         implicit_defs_for_restore(defs, op) ;
         set_implicit_def_list(cur_op,defs) ;
      }
      lics(*defs) ;
   }
   else {
      List<Operand>* defs = get_implicit_def_list(cur_op);
      if (defs != NULL) {
	lics(*defs);
      }     
   }
}

void Op_implicit_dests::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   if (is_call(cur_op)) {
      List<Operand>* defs = get_implicit_def_list(cur_op) ;
      if (defs == NULL) {
	 defs = new List<Operand>() ;
	 implicit_call_return_def_list_init(defs, op) ;
	 set_implicit_def_list(cur_op,defs) ;
      }
      lics(*defs) ;
   }
   else if ((is_restore(cur_op)) && ((cur_op->dest(DEST1)).is_macro_reg())){
      List<Operand>* defs = get_implicit_def_list(cur_op);
      if (defs == NULL) {
         defs = new List<Operand>();
         implicit_defs_for_restore(defs, op) ;
         set_implicit_def_list(cur_op,defs) ;
      }
      lics(*defs) ;
   }
   else {
      List<Operand>* defs = get_implicit_def_list(cur_op);
      if (defs != NULL) {
	lics(*defs);
      }     
   }
}


Op_implicit_dests::Op_implicit_dests(const Op_implicit_dests& os)
   : cur_op(os.cur_op), lics(os.lics) 
{}

Op_implicit_dests& Op_implicit_dests::operator=
                           (const Op_implicit_dests& os)
{
   cur_op = os.cur_op ;
   lics = os.lics ;
   return *this ;
}

Op_implicit_dests::~Op_implicit_dests()
{}

bool Op_implicit_dests::operator==(const void* i) const
{
   return (lics == i) ;
}

bool Op_implicit_dests::operator!=(const void* i) const
{
   return (lics != i) ;
}

void Op_implicit_dests::operator++()
{
   lics++ ;
}

void Op_implicit_dests::operator++(int)
{
   lics++ ;
}

Operand& Op_implicit_dests::operator*()
{
   return (*lics) ;
}

El_ref_type Op_implicit_dests::ref_type() 
{
   return (IMP_DEST) ;
}

Port_num Op_implicit_dests::get_port_num() 
{
   return (DEST1) ;
}

Op* Op_implicit_dests::get_op() 
{
   return (cur_op) ;
}

El_ref Op_implicit_dests::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Class Op_livein_dests

Op_livein_dests::Op_livein_dests()
{}

Op_livein_dests::Op_livein_dests(const Op* op)
: Op_implicit_dests(op)
{
  static eString entry_pseudo_defs("entry_pseudo_defs") ;

   if (is_control_merge(cur_op)) {
      List<Operand>* defs = 
         (List<Operand>*) get_generic_attribute(cur_op,entry_pseudo_defs) ;
      if (defs != NULL) {
         lics(*defs) ;
      }
   }
}

void Op_livein_dests::operator()(const Op* op)
{
  static eString entry_pseudo_defs("entry_pseudo_defs") ;

   cur_op = (Op*)op ;
   if (is_control_switch(cur_op)) {
      List<Operand>* defs = 
         (List<Operand>*) get_generic_attribute(cur_op,entry_pseudo_defs) ;
      if (defs != NULL) {
         lics(*defs) ;
      }
   }
}

El_ref_type Op_livein_dests::ref_type()
{
   return (LIVEIN_DEST) ;
}

El_ref Op_livein_dests::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

// Class Op_mem_dests

Op_mem_dests::Op_mem_dests()
   : cur_op(0)
{}

Op_mem_dests::Op_mem_dests(const Op* op)
   : cur_op((Op*)op)
{
   limd(cur_op->mem_dests()) ;
}

void Op_mem_dests::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   limd(cur_op->mem_dests()) ;
}

Op_mem_dests::Op_mem_dests(const Op_mem_dests& os)
   : cur_op(os.cur_op), limd(os.limd)
{}

Op_mem_dests& Op_mem_dests::operator=
                           (const Op_mem_dests& os)
{
   cur_op = os.cur_op ;
   limd = os.limd ;
   return *this ;
}

Op_mem_dests::~Op_mem_dests()
{}

bool Op_mem_dests::operator==(const void* i) const
{
   return (limd == i) ;
}

bool Op_mem_dests::operator!=(const void* i) const
{
   return (limd != i) ;
}

void Op_mem_dests::operator++()
{
   limd++ ;
}

void Op_mem_dests::operator++(int)
{
   limd++ ;
}

Operand& Op_mem_dests::operator*()
{
   return (*limd) ;
}

El_ref_type Op_mem_dests::ref_type() 
{
   return (MEM_DEST) ;
}

Port_num Op_mem_dests::get_port_num() 
{
   return (MEM0) ;
}

Op* Op_mem_dests::get_op() 
{
   return (cur_op) ;
}

El_ref Op_mem_dests::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}


///////////////////////////////////////////////////////////////////


// Class Op_mem_kills

Op_mem_kills::Op_mem_kills()
   : cur_op(0)
{}

Op_mem_kills::Op_mem_kills(const Op* op)
   : cur_op((Op*)op)
{
   limd(cur_op->mem_kills()) ;
}

void Op_mem_kills::operator()(const Op* op)
{
   cur_op = (Op*)op ;
   limd(cur_op->mem_kills()) ;
}

Op_mem_kills::Op_mem_kills(const Op_mem_kills& os)
   : cur_op(os.cur_op), limd(os.limd)
{}

Op_mem_kills& Op_mem_kills::operator=
                           (const Op_mem_kills& os)
{
   cur_op = os.cur_op ;
   limd = os.limd ;
   return *this ;
}

Op_mem_kills::~Op_mem_kills()
{}

bool Op_mem_kills::operator==(const void* i) const
{
   return (limd == i) ;
}

bool Op_mem_kills::operator!=(const void* i) const
{
   return (limd != i) ;
}

void Op_mem_kills::operator++()
{
   limd++ ;
}

void Op_mem_kills::operator++(int)
{
   limd++ ;
}

Operand& Op_mem_kills::operator*()
{
   return (*limd) ;
}

El_ref_type Op_mem_kills::ref_type() 
{
   return (MEM_DEST) ;
}

Port_num Op_mem_kills::get_port_num() 
{
   return (MEM0) ;
}

Op* Op_mem_kills::get_op() 
{
   return (cur_op) ;
}

El_ref Op_mem_kills::get_ref()
{
   El_ref new_ref(get_op(), &(operator*()), ref_type()) ;
   return (new_ref) ;
}

///////////////////////////////////////////////////////////////////
/* ------------ Class  Region_entry_edges */

Region_entry_edges::Region_entry_edges(){}

Region_entry_edges::Region_entry_edges(Region* region, 
				       const Edge_filter* filter)
{ 
  init(region, filter);
}


void Region_entry_edges::operator()(Region* region, 
				    const Edge_filter* filter)
{
  init(region, filter);
}

Region_entry_edges::~Region_entry_edges() {}

void Region_entry_edges::init(Region* region, const Edge_filter* filter)
{
  bool is_op = region->is_op();
  List_filterator<Edge*>::operator()
    (region->inedges(), 
     is_op ?  new Filter_and<Edge*>(filter, CONTROL0_INEDGES) : filter,
     is_op);
}

/* ----------- Class  Region_exit_edges */

Region_exit_edges::Region_exit_edges(){}

Region_exit_edges::Region_exit_edges(Region* region, const Edge_filter* filter)
{ 
  init(region, filter);
}


void Region_exit_edges::operator()(Region* region, const Edge_filter* filter)
{
  init(region, filter);
}

Region_exit_edges::~Region_exit_edges(){}

void Region_exit_edges::init(Region* region, const Edge_filter* filter)
{
  bool is_op = region->is_op();
  List_filterator<Edge*>::operator()
    (region->outedges(), 
     is_op ?  new Filter_and<Edge*>(filter, CONTROL0_OUTEDGES) : filter,
     is_op);
}

/* ------------ Class Region_control_flow_pred */

Region_control_flow_pred::Region_control_flow_pred()
   : pred_set(hash_region_ptr, 16)
{}

Region_control_flow_pred::
Region_control_flow_pred(Region* region, 
			 const Edge_filter* edge_filter,
			 const Region_filter* region_filter)
   : pred_set(hash_region_ptr, 16)
{
  init(region, edge_filter, region_filter);
}

void Region_control_flow_pred::operator()(Region* region, 
					  const Edge_filter* edge_filter,
					  const Region_filter* region_filter)
{
  init(region, edge_filter, region_filter);
}

Region_control_flow_pred::~Region_control_flow_pred(){}

void Region_control_flow_pred::init(Region* region, 
	                            const Edge_filter* edge_filter,
				    const Region_filter* region_filter)
{
  for(Region_entry_edges iter(region, edge_filter); iter != 0; ++iter){
    Region* pred = ancestor_with_same_parent((*iter)->src(), region);
    if (pred != 0) pred_set += pred;
  }
  Hash_set_filterator<Region*>::operator()(pred_set, region_filter);
}



/* ------------ Class Region_control_flow_succ */

Region_control_flow_succ::Region_control_flow_succ()
   : succ_set(hash_region_ptr, 16)
{}

Region_control_flow_succ::
Region_control_flow_succ(Region* region, 
			 const Edge_filter* edge_filter,
			 const Region_filter* region_filter)
   : succ_set(hash_region_ptr, 16)
{
  init(region, edge_filter, region_filter);
}

void Region_control_flow_succ::operator()(Region* region, 
					  const Edge_filter* edge_filter,
					  const Region_filter* region_filter)
{
  init(region, edge_filter, region_filter);
}

Region_control_flow_succ::~Region_control_flow_succ(){}

void Region_control_flow_succ::init(Region* region, 
				    const Edge_filter* edge_filter,
				    const Region_filter* region_filter)
{
  for(Region_exit_edges iter(region, edge_filter); iter != 0; ++iter){
    Region* succ = ancestor_with_same_parent((*iter)->dest(), region);
    if (succ != 0) succ_set += succ;
  }
  Hash_set_filterator<Region*>::operator()(succ_set, region_filter);
}

/* -------------- Class Region_subregions */

Region_subregions::Region_subregions(Region* region, 
				     const Region_filter* filter)
   :Dlist_filterator<Region*>(region->subregions(), filter) {}

void Region_subregions::operator()(Region* region, 
				   const Region_filter* filter)
{
  Dlist_filterator<Region*>::operator()(region->subregions(), filter);
}

Region_subregions::~Region_subregions(){}


/* -------------- Class Region_subregions_bidir */

Region_subregions_bidir::Region_subregions_bidir(Region* region, bool end)
   :Dlist_iterator<Region*>(region->subregions(), end) {}

Region_subregions_bidir::Region_subregions_bidir(Region* region, Region* subregion)
   :Dlist_iterator<Region*>(region->subregions(), subregion) {}

void Region_subregions_bidir::operator()(Region* region, bool end)
{
 Dlist_iterator<Region*>::operator()(region->subregions(), end);
}

void Region_subregions_bidir::operator()(Region* region, Region* subregion)
{
 Dlist_iterator<Region*>::operator()(region->subregions(), subregion);
}

Region_subregions_bidir::~Region_subregions_bidir(){}


/* -------------- Class Region_all_ops */

Region_all_ops::Region_all_ops()
   :next_region(NULL), nextop(NULL) {}

Region_all_ops::Region_all_ops(Region* region, const Op_filter* filter)
   :next_region(NULL), nextop(NULL), filter_rep(filter)
{
  init(region);
}

void Region_all_ops::operator()(Region* region, const Op_filter* filter)
{
  if (next_region != NULL) delete next_region;
  next_region = NULL;
  nextop = NULL;
  filter_rep = filter;
  init(region);
}
   
Region_all_ops::~Region_all_ops()
{
   if (next_region != NULL) delete next_region ;
}

bool Region_all_ops::operator==(const void* x) const {
  return (x == (void*)nextop);   // NULL == 0
}

bool Region_all_ops::operator!=(const void* x) const {
  return !operator==(x);
}

void Region_all_ops::operator++() {
  advance();
}

void Region_all_ops::operator++(int) {
  advance();
}

Op* Region_all_ops::operator*() {
  return nextop;
}

void Region_all_ops::advance() {
  if (isop) nextop = NULL;
  else {
    if (next_region != NULL) {++(*next_region); find_in_next_region();}
    else {++top_level; find_in_top_level();}
  }
}

void Region_all_ops::find_in_top_level() {
  while (top_level != 0) {
    Region* next = *top_level;
    if(next->is_op()) {
      Op* temp = (Op*)next;
      if (filter_rep->filter(temp)) {nextop = temp; return;}
      else {++top_level; continue;}
    }
    else {
      if (next_region != NULL) delete next_region;
      next_region = new Region_all_ops(*top_level, filter_rep);
      find_in_next_region();
      return;
    }
  }
  nextop = NULL;
  return;
}
      
void Region_all_ops::find_in_next_region(){
  if (next_region == NULL || *next_region == 0) {
    ++top_level; find_in_top_level();
  }
  else {nextop = *(*next_region);}
}

void Region_all_ops::init(Region* region)     
{
  if (region->is_op()) {
    isop = true;
    Op* temp = (Op*)region;
    if (filter_rep->filter(temp)) nextop = temp;
  }
  else {
    isop = false;
    top_level(region->subregions());
    find_in_top_level();
  }
}



//------------ Class Region_all_inedges ------------------

Region_all_inedges::Region_all_inedges() {}

Region_all_inedges::Region_all_inedges(Region* region, 
				       const Edge_filter* filter)
{
  init(region, filter);
}

void Region_all_inedges::operator()(Region* region, const Edge_filter* filter)
{
  init(region, filter);
}

Region_all_inedges::~Region_all_inedges(){}

void Region_all_inedges::init(Region* region, const Edge_filter* filter)
{
  Edge* entry_edge;
  Op* op;
  Same_dest_alt_filter* same_alt;
  if (region->is_op()) all_edges = region->inedges();
  else {
    for (Region_entry_edges entry(region); entry != 0; entry++){
      entry_edge = *entry;
      op = entry_edge->dest();
      all_edges.add_tail(entry_edge);
      for (List_iterator<Op*> related(op->get_relops()); 
	   related != 0; related++) {
	same_alt = new Same_dest_alt_filter(entry_edge);
	for (Op_inedges in(*related, same_alt); in != 0; in++) {
	  all_edges.add_tail(*in);
	}
	delete same_alt;
      }
    }
  }
  List_filterator<Edge*>::operator()(all_edges, filter);
}

//------------ Class Region_all_outedges ------------------

Region_all_outedges::Region_all_outedges() {}

Region_all_outedges::Region_all_outedges(Region* region, 
					 const Edge_filter* filter)
{
  init(region, filter);
}

void Region_all_outedges::operator()(Region* region, const Edge_filter* filter)
{
  init(region, filter);
}

Region_all_outedges::~Region_all_outedges(){}

void Region_all_outedges::init(Region* region, const Edge_filter* filter)
{
  Edge* exit_edge;
  Op* op;
  Same_src_alt_filter* same_alt;
  if (region->is_op()) all_edges = region->outedges();
  else {
    for (Region_exit_edges exit(region); exit != 0; exit++){
      exit_edge = *exit;
      op = exit_edge->src();
      all_edges.add_tail(exit_edge);
      for (List_iterator<Op*> related(op->get_relops()); 
	   related != 0; related++){
	same_alt = new Same_src_alt_filter(exit_edge);
	for (Op_outedges out(*related, same_alt); out != 0; out++) {
	  all_edges.add_tail(*out);
	}
	delete same_alt;
      }
    }
  }
  List_filterator<Edge*>::operator()(all_edges, filter);
}

//------------ Class Region_all_internal_edges ------------------

Region_all_internal_edges::Region_all_internal_edges()
   : all_edges(hash_edge_ptr,4096)
{}

Region_all_internal_edges::Region_all_internal_edges(Region* region, 
						     const Edge_filter* filter)
   : all_edges(hash_edge_ptr,4096)
{
  init(region, filter);
}

void Region_all_internal_edges::operator()(Region* region, 
					   const Edge_filter* filter)
{
  init(region, filter);
}

Region_all_internal_edges::~Region_all_internal_edges(){}

void Region_all_internal_edges::init(Region* region, const Edge_filter* filter)
{
  init_aux(region, region);
  Hash_set_filterator<Edge*>::operator()(all_edges, filter);
}

void Region_all_internal_edges::init_aux(Region* region, Region* top_level) {
  Region* temp;
/*  Edge* edge; */
  Edge_filter* in_region = new Edge_is_contained_in_region_filter(top_level);
  for (Region_subregions sub(region); sub != 0; sub++) {
    temp = *sub;
    if (temp->is_op()) {
      for (Op_inedges in((Op*)temp, in_region); in != 0; in++){
	all_edges += *in;
/*	edge = *in;
	if (region_contains_edge(top_level, edge)) all_edges += edge;
*/
      }
      for (Op_outedges out((Op*)temp, in_region); out != 0; out++){
	all_edges += *out;
/* 	edge = *out;
	if (region_contains_edge(top_level, edge)) all_edges += edge;
*/
      }
    }
    else init_aux(temp, top_level);
  }

  // don't need the filter anymore KF 4-2003
  delete in_region;
}

/* ------------ Class  Region_entry_ops */

Region_entry_ops::Region_entry_ops(){}

Region_entry_ops::Region_entry_ops(Region* region, const Op_filter* filter)
{ 
  init(region, filter);
}

void Region_entry_ops::operator()(Region* region, const Op_filter* filter)
{
  init(region, filter);
}

Region_entry_ops::~Region_entry_ops() {}

void Region_entry_ops::init(Region* region, const Op_filter* filter)
{
  Hash_set_filterator<Op*>::operator() (region->inops(), filter);
}

/* ----------- Class  Region_exit_ops */

Region_exit_ops::Region_exit_ops(){}

Region_exit_ops::Region_exit_ops(Region* region, const Op_filter* filter)
{ 
  init(region, filter);
}

void Region_exit_ops::operator()(Region* region, const Op_filter* filter)
{
  init(region, filter);
}

Region_exit_ops::~Region_exit_ops(){}

void Region_exit_ops::init(Region* region, const Op_filter* filter)
{
  Hash_set_filterator<Op*>::operator()(region->outops(), filter);
}

//////////////////////////////////////////////////////////////////////
//     Class Region_ops_C0_order
//     ops in forward control flow order: works only for BB, SB, HB
//////////////////////////////////////////////////////////////////////

Region_ops_C0_order::Region_ops_C0_order() : op(NULL)
{
}

Region_ops_C0_order::Region_ops_C0_order(Compound_region* region, Op* start_op)
{
 assert(region->is_hb() || region->is_bb());
 if (start_op == NULL) {
  init(region);
 } else {
  init(region, start_op);
 }
}

void
Region_ops_C0_order::operator()(Compound_region* region, Op* start_op)
{
 assert(region->is_hb() || region->is_bb());
 if (start_op == NULL) {
  init(region);
 } else {
  init(region, start_op);
 }
}

Region_ops_C0_order::~Region_ops_C0_order()
{
}

bool
Region_ops_C0_order::operator==(const void* x) const
{
  return((void*)op == x);
}

bool
Region_ops_C0_order::operator!=(const void* x) const
{
  return((void*)op != x);
}

void 
Region_ops_C0_order::operator++()
{
  advance();
}

void
Region_ops_C0_order::operator++(int)
{
  advance();
}

Op *
Region_ops_C0_order::operator*()
{
  return (op);
}

Region_ops_C0_order::Region_ops_C0_order(const Region_ops_C0_order&)
{
  assert(0);
}

Region_ops_C0_order& 
Region_ops_C0_order::operator=(const Region_ops_C0_order&)
{
  assert(0);
  return *this;
}

void 
Region_ops_C0_order::init(Compound_region* region) 
{
  // save pointer to region
  blk = region;

  // get the top entry op
  Region_entry_ops entry(region);
  op = *entry;
}

void 
Region_ops_C0_order::init(Compound_region* region, Op* start_op) 
{
  // save pointer to region and start_op
  blk = region;
  op = start_op;
}

void 
Region_ops_C0_order::advance()
{
  Edge* edge;
  Op* temp_op = (Op*)NULL;
  for (Op_outedges out(op, CONTROL0_OUTEDGES); out != 0; out++) {
    edge = *out;
    if (!(blk->is_exit_edge(edge))) {
      temp_op = edge->dest();
      break;
    }
  }
  op = temp_op;
}


//////////////////////////////////////////////////////////////////////
//     Class Region_ops_reverse_C0_order
//     ops in reverse control flow order: works only for BB, SB, HB
//////////////////////////////////////////////////////////////////////

Region_ops_reverse_C0_order::Region_ops_reverse_C0_order() : op(NULL)
{
}

Region_ops_reverse_C0_order::Region_ops_reverse_C0_order(Compound_region* region, Op* exit_op)
{
  init(region, exit_op);
}

void
Region_ops_reverse_C0_order::operator()(Compound_region* region, Op* exit_op)
{
  init(region, exit_op);
}

Region_ops_reverse_C0_order::~Region_ops_reverse_C0_order()
{
}

bool
Region_ops_reverse_C0_order::operator==(const void* x) const
{
  return((void*)op == x);
}

bool
Region_ops_reverse_C0_order::operator!=(const void* x) const
{
  return((void*)op != x);
}

void 
Region_ops_reverse_C0_order::operator++()
{
  advance();
}

void
Region_ops_reverse_C0_order::operator++(int)
{
  advance();
}

Op *
Region_ops_reverse_C0_order::operator*()
{
  return (op);
}

Region_ops_reverse_C0_order::Region_ops_reverse_C0_order(const Region_ops_reverse_C0_order&)
{
  assert(0);
}

Region_ops_reverse_C0_order& 
Region_ops_reverse_C0_order::operator=(const Region_ops_reverse_C0_order&)
{
  assert(0);
  return *this;
}

void 
Region_ops_reverse_C0_order::init(Compound_region* region, Op* exit_op)
{
  // save pointer to region
  blk = region;

  if (exit_op != NULL) {
    op = exit_op;
  }
  else {
    // get the bottom exit op
    // Old code below, did not work for HB's with embedded BB's (SAM 8-95)
    // Dlist<Region*> &subregions = ((Region*)blk)->subregions();
    // op = (Op*) subregions.tail();

    // Go thru all exits of the region, the exit with all control0 edges
    // leaving the region is the bottommost exit.  Note this will NOT work
    // for LoopBody regions.

    bool found = false;
    for (Region_exit_ops e_op_iter(region); e_op_iter!=0; e_op_iter++) {
      found = true;
      for (Op_outedges o_edge_iter(*e_op_iter, CONTROL, CONTROL0);
           o_edge_iter!=0;
           o_edge_iter++) {
	Edge *out_edge = *o_edge_iter;
	if (! region->is_exit_edge(out_edge)) {
          found = false;
          break;
        }
      }
      if (found == true) {
        op = *e_op_iter;
        break;
      }
    }
    assert(found);
  }
}

void 
Region_ops_reverse_C0_order::advance()
{
  assert(blk->is_hb() || blk->is_bb());
  Edge* edge;
  Op* temp_op = (Op*)NULL;
  for (Op_inedges in(op, CONTROL0_INEDGES); in != 0; in++) {
    edge = *in;
    if (!(blk->is_entry_edge(edge))) {
      temp_op = edge->src();
      break;
    }
  }
  op = temp_op;
}


/*---------------------------------------------------------------------------
   Region_ops_linear: Implementation
-----------------------------------------------------------------------------*/


Region_ops_linear::Region_ops_linear()
    :the_region(NULL), done(true)
{
}

Region_ops_linear::Region_ops_linear(Region* region, bool end)
    :the_region(region), done(false)
{
  if (the_region->is_compound()){
      init_go_down((Compound_region*)region, end);
  }
}

Region_ops_linear::Region_ops_linear(Region* region, Op* op)
    :the_region(region), done(false)
{
  init_go_up(op);
}

void Region_ops_linear::operator()(Region* region, bool end)
{
  delete_iterator_list();
  the_region = region;
  done = false;
  if (the_region->is_compound()){
      init_go_down((Compound_region*)region, end);
  }
}

void Region_ops_linear::operator()(Region* region, Op* op)
{
  delete_iterator_list();
  the_region = region;
  done = false;
  init_go_up(op);
}

Region_ops_linear::~Region_ops_linear()
{
  delete_iterator_list();
}

bool Region_ops_linear::operator==(const void* x) const
{
   return ((x == NULL) && done);
}


bool Region_ops_linear::operator!=(const void* x) const
{
   return ((x == NULL) && !done) ;
}

void Region_ops_linear::operator++()
{
  increment();
}

void Region_ops_linear::operator++(int)	  // int parameter signals postfix

{
  increment();
}

void Region_ops_linear::operator--()
{
  decrement();
}

void Region_ops_linear::operator--(int)  // int parameter signals postfix
{
  decrement();
}

Op* Region_ops_linear::operator*() 
{
  Dlist_iterator<Region*>* dlist_iter = iterator_list.head();
  Region* to_return = *(*dlist_iter);
  return ((Op*)to_return);
}

void Region_ops_linear::init_go_down(Region* region, bool end)
{
  Dlist_iterator<Region*>* dlist_iter = 
      new Dlist_iterator<Region*>(region->subregions(), end);
  iterator_list.add_head(dlist_iter);
  Region* pointing_at = *(*dlist_iter);
  if (pointing_at->is_compound()) {
      init_go_down(pointing_at, end);
  }
}

void Region_ops_linear::init_go_up(Region* region)
{
  Region* parent = region->parent();
  if (parent != the_region){
      Dlist_iterator<Region*>* dlist_iter = 
      new Dlist_iterator<Region*>(parent->subregions(), region);
      iterator_list.add_tail(dlist_iter);
      init_go_up(parent);
  }
}

void Region_ops_linear::increment()
{
   if (done) El_warn("Region_ops_linear: Can't increment, iteration is over.");
   else if(the_region->is_op()) done = true;
   else {
       while (!(iterator_list.is_empty())){
	 Dlist_iterator<Region*>* dlist_iter = iterator_list.head();
	 (*dlist_iter)++;
	 if((*dlist_iter) == 0) {
	     iterator_list.pop();
	     delete dlist_iter;
	 }
	 else {
	     Region* pointing_at = *(*dlist_iter);
	     if (pointing_at->is_compound()){
		 init_go_down(pointing_at, false);
	     }
	     break;
	 }
       }
       if (iterator_list.is_empty()) done = true;
   }
}

void Region_ops_linear::decrement()
{
   if (done) El_warn("Region_ops_linear: Can't increment, iteration is over.");
   else if(the_region->is_op()) done = true;
   else {
       while (!(iterator_list.is_empty())){
	 Dlist_iterator<Region*>* dlist_iter = iterator_list.head();
	 (*dlist_iter)--;
	 if((*dlist_iter) == 0) {
	     iterator_list.pop();
	     delete dlist_iter;
	 }
	 else {
	     Region* pointing_at = *(*dlist_iter);
	     if (pointing_at->is_compound()){
		 init_go_down(pointing_at, true);
	     }
	     break;
	 }
       }
       if (iterator_list.is_empty()) done = true;
   }
}

void Region_ops_linear::delete_iterator_list()
{
  while (!(iterator_list.is_empty())){
      delete iterator_list.pop();
  }
}



// Unsupported functions 
Region_ops_linear::Region_ops_linear(const Region_ops_linear&)
{
    assert(0); 
}

Region_ops_linear& Region_ops_linear::operator=(const Region_ops_linear&)
{
    assert(0); 
    return *this; //return to make CC happy
} 


/*---------------------------Region_subregions_freq--------------------------
  Iterates over subregions (recursively) of a region in sorted order based on
  the frequencies.
  Return_type filter specifies the types of regions that one is interested
  in, e.g., loops or hyperblocks.
 
  Stop_recursion filter specifies when to stop the recursive walk to collect
  all subregions.
----------------------------------------------------------------------------*/

// Special handling of this template required for stupid compilers.
# include "msort.cpp"
  template void merge_sort_pred(Vector<Region*> &, bool (*)(Region*, Region*));

Region_subregions_freq::Region_subregions_freq()
    :index(0), length(0), sorted_vector(NULL) {}

Region_subregions_freq::Region_subregions_freq
                  (Compound_region* region, const Region_filter* return_type, 
		   const Region_filter* stop_recursion)
{
    init(region, return_type, stop_recursion);
}

void Region_subregions_freq::operator()
                 (Compound_region* region, const Region_filter* return_type, 
		  const Region_filter* stop_recursion)
{
    if (sorted_vector != NULL) delete sorted_vector;
    init(region, return_type, stop_recursion);
}

Region_subregions_freq::~Region_subregions_freq(){
    if (sorted_vector != NULL) delete sorted_vector;
}

bool Region_subregions_freq::operator==(const void* i) const {
   if (i != NULL ) return(false) ;
   if (index >= length) return(true) ;
   return (false) ; 
}

bool Region_subregions_freq::operator!=(const void* i) const{
    return (!operator==(i)); 
}

void Region_subregions_freq::operator++(){
    index++ ;
}

void Region_subregions_freq::operator++(int){
    index++;
}

Region* Region_subregions_freq::operator*(){
   return((*sorted_vector)[index]) ;
}

void Region_subregions_freq::init(Compound_region* region,
				  const Region_filter* return_type,
				  const Region_filter* stop_recursion) {
    int i = 0;
    index = 0;
    length = 0;
    List<Region*> regions;
    region_specified_subregions(region, return_type, stop_recursion,
				regions, length);
    sorted_vector = new Vector<Region*>(length);
    if (dbg(glu)) cout << "Sorting Region @" << region << endl;
    if (dbg(glu)) cout << "Unsorted Vector:" << endl;
    for(List_iterator<Region*> iter(regions); iter != 0;  iter++){
        if (dbg(glu)) cout << "Subregion @" << *iter << " Weight = " << 
	  ((Compound_region*)(*iter))->weight << endl;
	(*sorted_vector)[i++] = *iter;
    }
    merge_sort_pred(*sorted_vector, &region_less_than);
    if (dbg(glu)) cout << "Sorted Vector:" << endl;
    for (i=0 ; i < length ; i++) {
      Region *ptr = (*sorted_vector)[i];
      if (dbg(glu))
	cout << "Subregion @" << ptr << " Weight = " << 
	  ((Compound_region*)ptr)->weight << endl;
    }
}

void Region_subregions_freq::region_specified_subregions 
                    (Region* region, const Region_filter* return_type,
		     const Region_filter* stop_recursion,
		     List<Region*>& regions, int& length){
    Region* temp;
    for(Region_subregions iter(region); iter != 0; iter++){
	temp = *iter;
	if(return_type->filter(temp)){
	    regions.add_tail(temp);
	    length++;
	}
	if (!(stop_recursion->filter(temp) || temp->is_op()))
	    region_specified_subregions(temp, return_type, stop_recursion, 
					regions, length);
    }
}

// Unsupported methods

Region_subregions_freq::Region_subregions_freq(const Region_subregions_freq&){
    assert(0);
}

Region_subregions_freq& 
        Region_subregions_freq::operator=(const Region_subregions_freq&){ 
    assert(0);
    return *this; //return to make CC happy
}
