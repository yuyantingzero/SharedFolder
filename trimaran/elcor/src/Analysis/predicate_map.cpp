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
//      File:         predicate_map.cpp
//      Authors:      Richard Johnson  
//      Created:      March 1995
//      Description:  Creates and holds mapping from source to predicate 
//                    symbols; generates list of partition relations from 
//                    source.
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include "region.h"
#include "iterators.h"
#include "attributes.h"
#include "opcode_properties.h"
#include "string_class.h"
#include "hash_map.h"
#include "hash_functions.h"
#include "partition_graph.h"
#include "predicate_map.h"
#include "dbg.h"
#include "dfa.h"


using namespace std;

//-----------------------------------------------------------------------------
PredicateMap::PredicateMap (Compound_region* r) :
  src_to_sym(hash_operand_ptr, 199),
  or_dest_guard(hash_int, 199),
  and_dest_guard(hash_int, 199)
{
  blk = r;
  _sym_count = 0;

  // allocate names for true,false
  _sym_false = sym_name();  // 0
  _sym_true = sym_name();   // 1

  scan_blk ();
}

PredicateMap::~PredicateMap (void)
{
}

int
PredicateMap::false_sym (void) const
{
  return _sym_false;
}

int
PredicateMap::true_sym (void) const
{
  return _sym_true;
}

int
PredicateMap::sym_count (void) const
{
  return _sym_count;
}

List<Partition>*
PredicateMap::get_sw_list (void)
{
  return &sw_list;
}

List<Partition>*
PredicateMap::get_mg_list (void)
{
  return &mg_list;
}

void
PredicateMap::remap(int x, int y) {

  for(Hash_map_iterator<Operand*, int> mi(src_to_sym); mi != 0; mi++){
    Pair <Operand*,int> &p = *mi;
    if (p.second == x) {
      p.second = y;
    }
  }
  Hash_map_iterator<int, int> mi2;
  for(mi2(or_dest_guard); mi2 != 0; mi2++){
    Pair <int,int> &p = *mi2;
    if (p.second == x) {
      p.second = y;
    }
  }
  for(mi2(and_dest_guard); mi2 != 0; mi2++){
    Pair <int,int> &p = *mi2;
    if (p.second == x) {
      p.second = y;
    }
  }

  if (or_dest_guard.is_bound(x)) {
    int z = or_dest_guard.value(x);
    or_dest_guard.unbind(x);
    or_dest_guard.bind(y,z);
  }  

  if (and_dest_guard.is_bound(x)) {
    int z = and_dest_guard.value(x);
    and_dest_guard.unbind(x);
    and_dest_guard.bind(y,z);
  }  

}

//-----------------------------------------------------------------------------
/*
 *  ALU       x <- op(y) if q          w(x) = Q       r(y) = Q   r(q) = T
 *  cmpp U    p <- c^q   if true       w(p) = T       r(c) = T   r(q) = T
 *  cmpp C    p <- c     if q          w(p) = Q       r(c) = Q   r(q) = T
 *  cmpp O    p <- 1     if c^q        w(p) = C^Q     r(c) = Q   r(q) = T
 *  cmpp A    p <- 0     if (!c)^q     w(p) = (!C)^Q  r(c) = Q   r(q) = T
 *  branch    p <- br(r) if q          w(p) = T       r(r) = Q   r(q) = T
 *  merge     x <- y                   w(x) = T       r(y) = T
 *  switch    x <- y                   w(x) =         r(y) = 
 */
int
PredicateMap::src_guard_sym (Op* op, Port_num pn) const
{
   // guarding predicate always read, ie. src_guard is true
   if (pn == PRED1) {
//      assert (op->predicated()); PRED1 aliases with MEM0
      return true_sym();
   }
   
   // data merge.  Sources always read, ie. guarded under true
   if (is_merge(op)) {
      return true_sym();
   }
   
   // data switch.  use taken condition, unless last branch.
   if (is_switch(op)) {
      List_iterator<Op*> relsmop (op->get_relops());
      Op* sw = *relsmop ;
      
      // if branch is last op in region, return true
      if (sw == get_last_region_op_from_subregions(blk)) {
         return true_sym();
      }
      
      // else return taken condition
      return this->branch_sym (sw);
   }
   
   if (op->predicated()) {
      Operand p = op->src(PRED1);
      if (p.is_reg()) {
         return src_to_sym.value(&op->src(PRED1));
      }
   }
   return true_sym();
}

int
PredicateMap::dest_guard_sym (Op* op, Port_num pn) const
{
  // data merge
  if (is_merge(op)) return true_sym();

  // data switch
  if (is_switch(op)) {
    List_iterator<Op*> relsmop (op->get_relops());
    Op* sw = *relsmop ;

    // if branch is last op in region, return true
    if (op == get_last_region_op_from_subregions(blk)) {
      return true_sym();
    }

    // else return taken condition
    return this->branch_sym (sw);
  }

  if (is_U_pred(op, pn)) return true_sym();
  if (is_O_pred(op, pn)) {
    return or_dest_guard.value(src_to_sym.value(&op->dest(pn)));
  }
  if (is_A_pred(op, pn)) {
    return and_dest_guard.value(src_to_sym.value(&op->dest(pn)));
  }
  if (op->predicated()) {
    Operand p = op->src(PRED1);
    if (p.is_reg()) {
      return src_to_sym.value(&op->src(PRED1));
    }
  }
  return true_sym();
}

// for Impact code, br p if q, p = p^q, ie. q is redundant.  In any case,
// p is an upper-bound of p^q, so p is correct for lub_guard
int
PredicateMap::branch_sym (Op* op) const
{
  // if op is relop, get main op
  if (is_switch(op)) {
    List_iterator<Op*> relsmop (op->get_relops());
    op = *relsmop ;
  }

  assert (is_control_switch(op));

  if (is_bru(op) || is_brl(op) || is_brlc(op) ||
      is_brdv(op) || is_rts(op)) {
    if (op->predicated()) {
      Operand q = op->src(PRED1);
      if (q.is_reg()) {
	return src_to_sym.value(&op->src(PRED1));
      }
    }
    return true_sym();
  } else if (is_dummy_branch(op)) {
    return false_sym();
  } else if (is_brct(op)) {
    Operand p = op->src(SRC2);
    if (p.is_predicate_true())
      return true_sym();
    if (p.is_predicate_false())
      return false_sym();
    assert((p.is_reg() || p.is_macro_reg()) && (p.file_type() == PR || p.file_type() == CR));
    return src_to_sym.value(&op->src(SRC2));
  } else if (is_brf(op)) {
    return true_sym();
  } else if (is_brw(op) || is_brcf(op)) {
    El_punt("branch_sym: these cases not handled yet\n");
    return true_sym();
  } else {
    El_punt("Unknown branch type.");
    return true_sym();  // make compiler happy
  }
}

//-----------------------------------------------------------------------------
int
PredicateMap::sym_name (void)
{
  return _sym_count++;
}

void
PredicateMap::switch_relation(int a, int b, int c)  // a partitions into b U c
{
  if (dbg(pqs))
    cdbg << "switch " << a << " = " << b << " U " << c << endl;

  Partition p(a, b, c);
  sw_list.add_tail(p);
}

void
PredicateMap::merge_relation(int a, int b, int c)  // c partitions into a U b
{
  if (dbg(pqs))
    cdbg << "merge " << c << " = " << a << " U " << b << endl;

  Partition p(c, a, b);
  mg_list.add_tail(p);
}

/*
 *
 */
Op*
PredicateMap::normalize_cmpp (Op* op)
{
  //
  // sort source operands lexicographically by swapping if necessary
  //
  Operand src1 = op->src(SRC1);
  Operand src2 = op->src(SRC2);
  assert (!src1.is_undefined() && !src2.is_undefined());

  ostringstream os1, os2;
  src1.print_for_pqs(os1);
  src2.print_for_pqs(os2);
  bool swap = (os2.str() < os1.str());

  //
  // normalize opcode; keep track of whether dest mods need to be complemented.
  //
  Opcode opc = op->opcode();
  Opcode normalized_opc;          // normal form of compare-to-pred opcode
  int complement_mod = false;     // if true, complement destination mod flags

  switch (get_base (opc)) {
  case BASE_CMPP_W_EQ:
    normalized_opc = (Opcode)BASE_CMPP_W_EQ;
    complement_mod = false;
    break;
  case BASE_CMPP_W_LT:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LEQ : BASE_CMPP_W_LT);
    complement_mod = swap;
    break;
  case BASE_CMPP_W_LLT:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LLEQ : BASE_CMPP_W_LLT);
    complement_mod = swap;
    break;
  case BASE_CMPP_W_LEQ:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LT : BASE_CMPP_W_LEQ);
    complement_mod = swap;
    break;
  case BASE_CMPP_W_LLEQ:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LLT : BASE_CMPP_W_LLEQ);
    complement_mod = swap;
    break;
  case BASE_CMPP_W_GT:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LT : BASE_CMPP_W_LEQ);
    complement_mod = !swap;
    break;    
  case BASE_CMPP_W_LGT:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LLT : BASE_CMPP_W_LLEQ);
    complement_mod = !swap;
    break;
  case BASE_CMPP_W_GEQ:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LEQ : BASE_CMPP_W_LT);
    complement_mod = !swap;
    break;
  case BASE_CMPP_W_LGEQ:
    normalized_opc = (Opcode)(swap ? BASE_CMPP_W_LLEQ : BASE_CMPP_W_LLT);
    complement_mod = !swap;
    break;
  case BASE_CMPP_W_NEQ:
    normalized_opc = (Opcode)BASE_CMPP_W_EQ;
    complement_mod = true;
    break;

  case BASE_FCMPP_S_EQ:
    normalized_opc = (Opcode)BASE_FCMPP_S_EQ;
    complement_mod = false;
    break;
  case BASE_FCMPP_S_LT:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_S_LEQ : BASE_FCMPP_S_LT);
    complement_mod = swap;
    break;
  case BASE_FCMPP_S_LEQ:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_S_LT : BASE_FCMPP_S_LEQ);
    complement_mod = swap;
    break;
  case BASE_FCMPP_S_GT:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_S_LT : BASE_FCMPP_S_LEQ);
    complement_mod = !swap;
    break;    
  case BASE_FCMPP_S_GEQ:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_S_LEQ : BASE_FCMPP_S_LT);
    complement_mod = !swap;
    break;
  case BASE_FCMPP_S_NEQ:
    normalized_opc = (Opcode)BASE_FCMPP_S_EQ;
    complement_mod = true;
    break;

  case BASE_FCMPP_D_EQ:
    normalized_opc = (Opcode)BASE_FCMPP_D_EQ;
    complement_mod = false;
    break;
  case BASE_FCMPP_D_LT:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_D_LEQ : BASE_FCMPP_D_LT);
    complement_mod = swap;
    break;
  case BASE_FCMPP_D_LEQ:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_D_LT : BASE_FCMPP_D_LEQ);
    complement_mod = swap;
    break;
  case BASE_FCMPP_D_GT:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_D_LT : BASE_FCMPP_D_LEQ);
    complement_mod = !swap;
    break;    
  case BASE_FCMPP_D_GEQ:
    normalized_opc = (Opcode)(swap ? BASE_FCMPP_D_LEQ : BASE_FCMPP_D_LT);
    complement_mod = !swap;
    break;
  case BASE_FCMPP_D_NEQ:
    normalized_opc = (Opcode)BASE_FCMPP_D_EQ;
    complement_mod = true;
    break;

  default:
    //ntclark
    //cerr << "unknown opcode: " << hex << opc << endl;
    cerr << "unknown opcode: " << ios::hex << opc << endl;
    assert (0);
  }

  //
  // create normalized op
  //

  // first copy dest1/dest2 mods from opc
  normalized_opc = (Opcode) (normalized_opc | (opc & IR_DEST1_MASK));
  normalized_opc = (Opcode) (normalized_opc | (opc & IR_DEST2_MASK));

  // complement mods if necessary
  if (complement_mod) {
    if ((normalized_opc & IR_DEST1_CBIT_MASK) == 0) {
      normalized_opc = (Opcode) (normalized_opc | IR_DEST1_CBIT_MASK);
    } else {
      normalized_opc = (Opcode) (normalized_opc & ~IR_DEST1_CBIT_MASK);
    }
    if (!(op->dest(DEST2)).is_undefined()) {
      if ((normalized_opc & IR_DEST2_CBIT_MASK) == 0) {
	normalized_opc = (Opcode) (normalized_opc | IR_DEST2_CBIT_MASK);
      } else {
	normalized_opc = (Opcode) (normalized_opc & ~IR_DEST2_CBIT_MASK);
      }
    }
  }

  // create new op with normalized opcode
  Op* normal_op = new Op(normalized_opc);

  // copy src/dest operands
  normal_op->set_src(PRED1, op->src(PRED1));
  normal_op->set_src(SRC1, (swap ? src2 : src1));
  normal_op->set_src(SRC2, (swap ? src1 : src2));
  normal_op->set_dest(DEST1, op->dest(DEST1));
  if (!(op->dest(DEST2)).is_undefined()) {
    normal_op->set_dest(DEST2, op->dest(DEST2));
  }

  return normal_op;
}

/*
 *
 */
void
PredicateMap::map_cmpp (Op* src_op, Vector<int>& vr_def_count,
                        //			Hash_map<Operand, int>& vr_to_index,
               	        Hash_map<int, int>& vr_to_index,
			Hash_map<int, int>& current_sym,
			Hash_map<eString, int>& expr_to_sym,
			Hash_map<int, int>& or_dest_guard)
{
  Op* norm_op = normalize_cmpp (src_op);

  //
  // create strings for normal/complemented condition
  //
  ostringstream os;

  Operand guard = norm_op->src(PRED1);
  Operand src1 = norm_op->src(SRC1);
  Operand src2 = norm_op->src(SRC2);

  // write normalized compare condition to string
  src1.print_for_pqs(os);
  if ((src1.is_reg() || src1.is_macro_reg()) &&
      vr_def_count[vr_to_index.value(src1.vr_num())] != 0) {
    os << vr_def_count[vr_to_index.value(src1.vr_num())];
  }
  os << ' ' << (IR_BASE_OPCODE) get_base (norm_op->opcode()) << ' ';
  src2.print_for_pqs(os);
  if ((src2.is_reg() || src2.is_macro_reg()) &&
      vr_def_count[vr_to_index.value(src2.vr_num())] != 0) {
    os << vr_def_count[vr_to_index.value(src2.vr_num())];
  }

  int sym_guard, sym_base, sym_cmpl, sym;

  // get sym_name for guarding predicate
  if (!guard.is_lit()) {
    sym_guard = current_sym.value(guard.vr_num());
  } else {
    sym_guard = _sym_true;
  }

  // append symbolic name for guarding predicate
  os <<  '*' << sym_guard;

  //
  // get (create if necessary) symbolic names for c*q, !c*q
  //
  if (!expr_to_sym.is_bound(os.str())) {
    sym_base = sym_name();
    sym_cmpl = sym_name();  // sym_cmpl = sym_base+1 by convention
    expr_to_sym.bind(os.str(), sym_base); 

    switch_relation(sym_guard, sym_base, sym_cmpl);
  } else {
    sym_base = expr_to_sym.value(os.str());
  }

  //
  // process definitions
  //
  for (int i=DEST1; i <= DEST2; i++) {
    Operand* oper = &norm_op->dest(i);
    if (!oper->is_undefined()) {
      if (is_U_pred(norm_op, i)) {
	// adjust sym if dest is complemented
	sym = sym_base + (is_complement_pred(norm_op,i) ? 1 : 0);
	
	// map vr# -> sym
	current_sym.bind(oper->vr_num(), sym);

	// map predicate reference to symbolic name
	src_to_sym.bind(&src_op->dest(i), sym);
      }
      else if (is_C_pred(norm_op,i)) {
	// adjust sym if dest is complemented
	sym = sym_base + (is_complement_pred(norm_op,i) ? 1 : 0);
	int sym_c, sym_p = current_sym.value(oper->vr_num());
	
	// if p false, sym_c is same as sym (ie. reduces to cmpp.u)
	if (sym_p == _sym_false) {
	  sym_c = sym;
	} else {
	  // else create sym name for one of p*!q+c*q, p*!q+!c*q
          //os.seekp(0); // reset to start of buf
	  os.str("");

	  // supress prefix if it reduces to false
	  if (sym_p != sym_guard) {
	    os << sym_p << "*!" << sym_guard << '+';
	  }
          os << sym;
	  
	  //
	  // get (create if necessary) symbase for C-assignment
	  //
	  if (!expr_to_sym.is_bound(os.str())) {
	    sym_c = sym_name();
	    sym_cmpl = sym_name();  // sym_cmpl = sym_c+1 by convention
	    expr_to_sym.bind(os.str(), sym_c);
	  
	    // emit relation (very conservative for now)
            switch_relation(sym_guard, sym_c, sym_cmpl);
	  } else {
	    sym_c = expr_to_sym.value(os.str());
	  }
	}
	
	// map vr# -> sym
	current_sym.bind(oper->vr_num(), sym_c);
	
	// map predicate reference to symbolic name
	src_to_sym.bind(&src_op->dest(i), sym_c);
      }
      else if (is_O_pred(norm_op,i)) {
	// adjust sym if dest is complemented
	sym = sym_base + (is_complement_pred(norm_op,i) ? 1 : 0);
	int sym_o, sym_p;

        // if the predicate is not in the map, this probably means that the
        // PRED_CLEAR was removed by dead code elimination.  this means the
        // value of sym_p is not actually important, but it should be set to
        // false so the analysis works.  -KF 2/2006
        if (current_sym.is_bound(oper->vr_num()))
          sym_p = current_sym.value(oper->vr_num());
        else {
          El_warn("O-type pred was not bound in current_sym.  Assuming false.  "
                  "Op %d, vr %d", src_op->id(), oper->vr_num());
          sym_p = _sym_false;
        }
	
	// if p false, sym_o is same as sym
	if (sym_p == _sym_false) {
	  sym_o = sym;
	} else {
	  // else create sym name for one of p+(c*q), p+(!c*q)
	  //os.seekp(0); // reset to start of buf
          os.str("");
          os << sym_p << '+' << sym;
	  
	  //
	  // get (create if necessary) symbase for OR-assignment
	  //
	  if (!expr_to_sym.is_bound(os.str())) {
	    sym_o = sym_name();
	    expr_to_sym.bind(os.str(), sym_o);    
	  
	    // emit assertions
	    merge_relation(sym, sym_p, sym_o);
	  } else {
	    sym_o = expr_to_sym.value(os.str());
	  }
	}
	
	// map vr# -> sym
	current_sym.bind(oper->vr_num(), sym_o);
	
	// map predicate reference to symbolic name
	src_to_sym.bind(&src_op->dest(i), sym_o);

	// save guarding predicate for dest pred (ie. c*q)
	or_dest_guard.bind(sym_o, sym);
      }
      else if (is_A_pred(norm_op,i)) {
	// adjust sym if dest is complemented
	sym = sym_base + (is_complement_pred(norm_op,i) ? 1 : 0);
	int sym_a, sym_p;

        // if the predicate is not in the map, this probably means that the
        // PRED_SET was removed by dead code elimination.  this means the value
        // of sym_p is not actually important, but it should be set to true so
        // the analysis works.  -KF 2/2006
        if (current_sym.is_bound(oper->vr_num()))
          sym_p = current_sym.value(oper->vr_num());
        else {
          El_warn("A-type pred was not bound in current_sym.  Assuming true.  "
                  "Op %d, vr %d", src_op->id(), oper->vr_num());
          sym_p = _sym_true;
        }

	// create sym name for one of p*!q+p*c*q, p*!q+p*!c*q
	//os.seekp(0); // reset to start of buf
        os.str("");

	// supress prefix if it reduces to false
	if (sym_p != sym_guard) {
	  os << sym_p << "*!" << sym_guard << '+';
	}
        os << sym_p << '*' << sym;
	
	//
	// get (create if necessary) symbase for AND-assignment
	//
	if (!expr_to_sym.is_bound(os.str())) {
	  sym_a = sym_name();
	  sym_cmpl = sym_name();  // sym_cmpl = sym_a+1 by convention
	  expr_to_sym.bind(os.str(), sym_a);
	  
	  // emit relation (very conservative for now)
	  switch_relation(sym_p, sym_a, sym_cmpl);
	} else {
	  sym_a = expr_to_sym.value(os.str());
	}
	
	// map vr# -> sym
	current_sym.bind(oper->vr_num(), sym_a);
	
	// map predicate reference to symbolic name
	src_to_sym.bind(&src_op->dest(i), sym_a);

	// save guarding predicate for dest pred (ie. !c*q)
	// - note: dest guard is complement of base condition
	sym = sym_base + (is_complement_pred(norm_op,i) ? 0 : 1);
	and_dest_guard.bind(sym_a, sym);
      } else {
	assert(0);
      }
    }
  }

  delete norm_op;
}

void
PredicateMap::map_movepp (Op* src_op, Vector<int>& vr_def_count,
                          //			Hash_map<Operand, int>& vr_to_index,
			Hash_map<int, int>& vr_to_index,
			Hash_map<int, int>& current_sym,
			Hash_map<eString, int>& expr_to_sym,
			Hash_map<int, int>& or_dest_guard)
{
  int sym_base, sym_cmpl, sym_src, sym_guard;

  Operand guard = src_op->src(PRED1);
  Operand src1 = src_op->src(SRC1);
  Operand dest1 = src_op->dest(DEST1);

  /* Handle special case of p = T if T, same as pred_set */
  if (guard.is_predicate_true() && src1.is_predicate_true()) {
    // map vr# -> sym_name
    // parkhc 03/02/2005
    //assert (vr_def_count[vr_to_index.value(dest1)] == 0);
    current_sym.bind(dest1.vr_num(), _sym_true);
    // map predicate reference to symbolic name
    src_to_sym.bind(&src_op->dest(DEST1), _sym_true);
  }

  /* Handle special case of p = F if T, same as pred_clear */
  else if (guard.is_predicate_true() && src1.is_predicate_false()) {
    // map vr# -> sym_name
    // parkhc 03/02/2005
    //assert (vr_def_count[vr_to_index.value(dest1)] == 0);
    current_sym.bind(dest1.vr_num(), _sym_false);
    // map predicate reference to symbolic name
    src_to_sym.bind(&src_op->dest(DEST1), _sym_false);
  }

  /* Handle special case p = q if T, set sym_p = sym_q */
  else if (guard.is_lit()) {
    if (guard.is_predicate_true()) {
      sym_src = current_sym.value(src1.vr_num());
      current_sym.bind(dest1.vr_num(), sym_src);
      src_to_sym.bind(&src_op->dest(DEST1), sym_src);
    }
    else { /* guard is F, hence it is dead code */
      /* Just declare the dest1 symbol as if it were a live-in predicate */
      if (! current_sym.is_bound(dest1.vr_num())) {
        sym_base = sym_name();
        sym_cmpl = sym_name();
        switch_relation(_sym_true, sym_base, sym_cmpl);
        current_sym.bind(dest1.vr_num(), sym_base);
        src_to_sym.bind(&src_op->dest(DEST1), sym_base);
      }
    }
  }

  /* Handle special case p = T if r, set sym_p = sym_r, should be p + r */
  else if (src1.is_predicate_true()) {
    int sym_o;
    sym_guard = current_sym.value(guard.vr_num());

    if( ! current_sym.is_bound(dest1.vr_num())) {
      // This can happen when a cmpp, gets turned into a movepp
      // through constant folding, and augmented with a msched staging
      // predicate. It's equivalent to a pred_set whenever the guard
      // is true.
      current_sym.bind(dest1.vr_num(), sym_guard);
    }

    int sym_p = current_sym.value(dest1.vr_num());

    ostringstream os;
    
    os << sym_p << '+' << sym_guard;
	  
    // get (create if necessary) symbase for OR-assignment
    if (!expr_to_sym.is_bound(os.str())) {
      sym_o = sym_name();
      expr_to_sym.bind(os.str(), sym_o);
	  
      // emit assertions
      merge_relation(sym_guard, sym_p, sym_o);
    } else {
      sym_o = expr_to_sym.value(os.str());
    }
	
    // map vr# -> sym
    current_sym.bind(dest1.vr_num(), sym_o);
	
    // map predicate reference to symbolic name
    src_to_sym.bind(&src_op->dest(DEST1), sym_o);

#if 0
    sym_guard = current_sym.value(guard.vr_num());
    current_sym.bind(dest1.vr_num(), sym_guard);
    src_to_sym.bind(&src_op->dest(DEST1), sym_guard);
#endif
  }

  /* Handle special case p = F if r, set sym_p = !sym_r, should be p*!r */
  else if (src1.is_predicate_false()) {
    int sym_a;
    /* convention is sym for !r is sym r + 1 */
    sym_guard = current_sym.value(guard.vr_num()) + 1;
    int sym_p = current_sym.value(dest1.vr_num());

    ostringstream os;

    os << sym_p << '*' << sym_guard;
	  
    // get (create if necessary) symbase for OR-assignment
    if (!expr_to_sym.is_bound(os.str())) {
      sym_a = sym_name();
      sym_cmpl = sym_name();
      expr_to_sym.bind(os.str(), sym_a);    
	  
      // emit assertions
      switch_relation(sym_p, sym_a, sym_cmpl);
    } else {
      sym_a = expr_to_sym.value(os.str());
    }
	
    // map vr# -> sym
    current_sym.bind(dest1.vr_num(), sym_a);
	
    // map predicate reference to symbolic name
    src_to_sym.bind(&src_op->dest(DEST1), sym_a);

#if 0
    /* convention is sym for !r is sym r + 1 */
    sym_guard = current_sym.value(guard.vr_num()) + 1;
    current_sym.bind(dest1.vr_num(), sym_guard);
    src_to_sym.bind(&src_op->dest(DEST1), sym_guard);
#endif
  }

  /* Handle general case conservatively, just treat dest as a live-in */
  else {
    sym_base = sym_name();
    sym_cmpl = sym_name();
    switch_relation(_sym_true, sym_base, sym_cmpl);
    current_sym.bind(dest1.vr_num(), sym_base);
    src_to_sym.bind(&src_op->dest(DEST1), sym_base);
  }
}

/* current_sym - maps source predicate names (vr#) to a unique
 *   symbolic predicate name (unique to this block)
 *
 * src_to_sym - maps each predicate reference in the source to its
 *   corresponding symbolic predicate name.
 */
void
PredicateMap::scan_blk (void)
{
  if (dbg(pqs)) cdbg << "> scan_blk " << blk->id() << endl;

  Hash_map<eString, int> expr_to_sym(hash_estring, 199);
  Hash_map<int, int> current_sym(hash_int, 199);

  // get mapping from VRs to dense integer range
  VR_map* vr_map = get_local_vr_map(blk);
  assert(vr_map != NULL);
  int& vr_count = vr_map->vrnum_count;
  //  Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;

  Hash_map<int,int>& vr_to_index = vr_map->vrnum_to_index;

  // create Vector of vr definition counters
  Vector<int> vr_def_count(vr_count, 0);

  // create mapping for true predicate
  expr_to_sym.bind("T", _sym_true);
  
  // traverse ops in block in forward linear order
  for (Region_ops_C0_order opi(blk); opi != 0; ++opi) {
    assert ( (*opi)->is_op() );
    Op* op = (Op*) (*opi);
    
    //
    // map any predicate uses to current_sym name for the predicate
    //
    int i ;
    for (i=0; i<op->num_srcs()+1; i++) {
      if ((i==0) && !op->predicated()) continue;
      Operand oper = op->src(i);
      if (oper.is_reg() && oper.file_type() == PR) {

	// get symbolic name for predicate, or create new name if predicate
	// is live-in
	if (current_sym.is_bound(oper.vr_num())) {
	  src_to_sym.bind(&op->src(i), current_sym.value(oper.vr_num()));
	} else {
	  // create symbols for live-in pred and it's complement
	  int sym_base = sym_name();
	  int sym_cmpl = sym_name();  // sym_cmpl = sym_base+1 by convention

	  // live-in pred has no relation to other predicates, so it's
	  // a partition of true
	  switch_relation(_sym_true, sym_base, sym_cmpl);

	  // map vr# -> sym_name
	  if(vr_def_count[vr_to_index.value(oper.vr_num())]) {
	    cdbg << "ERROR: " << oper << " already has a def count!" << endl;
	  }
	  assert (vr_def_count[vr_to_index.value(oper.vr_num())] == 0);

	  current_sym.bind(oper.vr_num(), sym_base);

	  src_to_sym.bind(&op->src(i), sym_base);
	}
      }
    }
    
    //
    // form normalized rhs, get canonical name or create new sym.name
    //
    if (is_cmpp (op)) {
      map_cmpp (op, vr_def_count, vr_to_index,
		current_sym, expr_to_sym, or_dest_guard);
    } else if (is_predicate_clear (op)) {
      assert (op->num_dests() == 1);
      Operand* oper = &op->dest(DEST1);
      if (!oper->is_undefined()) {
	// map vr# -> sym_name
        //	assert (vr_def_count[vr_to_index.value(*oper)] == 0);
	current_sym.bind(oper->vr_num(), _sym_false);
	
	// map predicate reference to symbolic name
	src_to_sym.bind(&op->dest(DEST1), _sym_false);
      }
    } else if (is_predicate_set (op)) {
      assert (op->num_dests() == 1);
      Operand* oper = &op->dest(DEST1);
      if (!oper->is_undefined()) {
	// map vr# -> sym_name
	assert (vr_def_count[vr_to_index.value(oper->vr_num())] == 0);
	current_sym.bind(oper->vr_num(), _sym_true);
	
	// map predicate reference to symbolic name
	src_to_sym.bind(&op->dest(DEST1), _sym_true);
      }
    } else if (is_movepp (op)) {
      map_movepp (op, vr_def_count, vr_to_index,
		current_sym, expr_to_sym, or_dest_guard);
    } else if (is_custom (op)) {
      // FIX: is this right? i basically just copied the predicate set
      // code...
      for(Op_explicit_dests oed(op); oed != 0; oed++) {
	Operand* oper = &(*oed);
	if (oper->is_reg() && oper->dtype().is_predicate()) {
	  // map vr# -> sym_name
	  assert (vr_def_count[vr_to_index.value(oper->vr_num())] == 0);
	  current_sym.bind(oper->vr_num(), _sym_true);
	  
	  // map predicate reference to symbolic name
	  src_to_sym.bind(&op->dest(DEST1), _sym_true);
	}	
      }
    } else if (is_brf (op)) {
      // first destination is predicate register
      Operand oper = op->dest(DEST1);
      assert (oper.is_reg() && oper.file_type() == PR);

      // create predicates for BRTOP's pred and it's complement
      int sym_base = sym_name();
      int sym_cmpl = sym_name();  // sym_cmpl = sym_base+1 by convention

      // BRTOP's pred has no relation to other predicates, so it's
      // a partition of true
      switch_relation(_sym_true, sym_base, sym_cmpl);

      // map predicate reference to symbolic name
      src_to_sym.bind(&op->dest(DEST1), sym_base);
    }
    
    // update def counters
#if 0
	SAM 7-03, need to go over all the implicit dests as well
	as the explicit dests.  The problem is the return value of
	a BRL.  If there are multiple BRLs in an HB, this will make
	sure each ret value is treated differently by PQS.
    for (i=0; i<op->num_dests(); i++) {
      Operand oper = op->dest((Port_num) (i + DEST1));
#endif
    for (Op_all_dests desti(op); desti!=0; desti++) {
      Operand oper = *desti;

      // PRESTOREs and MOVEGPs should be ignored by predicate analysis
      if(op->flag(EL_OPER_SPILL_CODE) || op->flag(EL_OPER_ICMOVE)) continue;

      //      if (is_predicate_clear(op)) continue;
      if (oper.is_reg() || oper.is_macro_reg()) {
        
        //        if(!vr_to_index.is_bound(oper.vr_num()) && oper.is_macro_reg()) continue;
        assert (vr_to_index.is_bound (oper.vr_num()));

	vr_def_count[vr_to_index.value(oper.vr_num())] += 1;
      }
    }
  }

  if (dbg(pqs)) cdbg << "--- expr_to_sym ---" << endl << expr_to_sym << endl;
  if (dbg(pqs)) {
    cdbg << "--- src_to_sym ---" << endl;
    for(Hash_map_iterator<Operand*, int> iter(src_to_sym); iter != 0; iter++){
      Pair <Operand*,int> p = *iter;
      cdbg << "<" << *(p.first) << ", " << p.second << ">" << endl;
    }
    cdbg << "--- end of src_to_sym ---" << endl;
  }
  if (dbg(pqs)) cdbg << "< scan_blk " << blk->id() << endl;
}

// end of predicate_map.cpp
