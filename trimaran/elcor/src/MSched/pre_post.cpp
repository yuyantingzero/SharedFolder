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
//      File:           pre_post.cpp
//      Authors:        Vinod Kathail, Shail Aditya
//      Created:        September 1995
//      Description:    Pre/post processing of swp loop regions
//
///////////////////////////////////////////////////////////////////////////

#include "defs.h"
#include "list.h"
#include "vector.h"
#include "hash_functions.h"

#include "el_error.h"
#include "el_ssched_init.h"
#include "el_lsched_init.h"

#include "region.h"
#include "iterators.h"
#include "el_port.h"
#include "op.h"
#include "opcode.h"
#include "opcode_cmpp.h"
#include "connect.h"
#include "el_bb_tools.h"
#include "opcode_properties.h"
#include "el_control.h"
#include "edge.h"
#include "edge_utilities.h"
#include "edge_drawing.h"
#include "attributes.h"
#include "operand.h"
#include "pre_post.h"
#include "cluster.h"
#include "hw_cluster.h"

//------------------------ Pre processing of loop and preloop --------------
// Functions to do the following:
// 1. Replace BRLC (or BRCT) by Brtop (or wtop) in loop
// 2. Insert staging predicates for all ops in loops
// 3. Hoist pbr to the pre-loop (only if El_lsched_hoist_pbr is true)

static void msched_convert_branch_and_insert_predicates(LoopBody* loop);
static Operand msched_convert_branch(Compound_region* body);
static Operand msched_construct_swp_branch(Op* non_swpbr, Op*& swpbr);
static Operand msched_construct_mve_swp_branch(Op* non_swpbr, Op*& swpbr,
					       List<Op*>& support_ops);
static void msched_insert_predicates(Compound_region* body, const Operand& st_pred);
static void msched_hoist_pbr(LoopBody* loop);

// Do as described above
void msched_preprocess(LoopBody* loop)
{
  // we need to hoist pbr before inserting predicates
  if (El_lsched_hoist_pbr) msched_hoist_pbr(loop);
  msched_convert_branch_and_insert_predicates(loop);
}

// Convert branch in loop to Brtop and insert staging predicates
void msched_convert_branch_and_insert_predicates(LoopBody* loop)
{
  Compound_region* body = loop->kernel();
  Operand st_pred = msched_convert_branch(body);
  loop->set_staging_pred(st_pred);
  msched_insert_predicates(body, st_pred);
}

// Replace branch by Brtop
Operand msched_convert_branch(Compound_region* body)
{
  Op *non_swpbr = *(Region_exit_ops(body));
  Operand st_pred;
  Op* swpbr = NULL;
  List<Op*> support_ops;

  if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
    // The Brtop assumes rotating predicate registers exist, so we
    // need to replace it with different instructions in mve.

    st_pred = msched_construct_mve_swp_branch(non_swpbr, swpbr, support_ops);
  } else if (El_msched_allocator == ROTATING_REGISTERS) {
    // Replace old branch with Brtop
    st_pred = msched_construct_swp_branch(non_swpbr, swpbr);
  }

  if(support_ops.size()) {
    for(List_iterator<Op*> li(support_ops); li != 0; li++) {
      El_insert_op_before(*li, non_swpbr);
    }
  }

  El_replace_op(body, non_swpbr, swpbr);  
  // move the rel_ops of the old branch to the rel_ops of the new branch
  for (List_iterator<Op*> rels(non_swpbr->get_relops()); rels != 0; 
       rels++) {
    Op *rel_op = *rels;
    swpbr->add_relops(rel_op);
    rel_op->add_relops(swpbr);
    non_swpbr->remove_relops(rel_op);
    rel_op->remove_relops(non_swpbr);
  }
  
  // delete the old branch
  delete non_swpbr;
  return st_pred;
}

// Construct a new Brtop op
Operand msched_construct_swp_branch(Op* non_swpbr, Op*& swpbr)
{
  Operand* brtarget = new Operand(non_swpbr->src(non_swpbr->first_src()));
  // Construct staging predicate output of Brtop. It is set to be static. 
  // remap insertion would convert it to be rotating.
  Operand staging_predicate(new Reg(EL_STANDARD_PREDICATE));
  staging_predicate.bind_file(PR, STATIC_REG); 

  // Construct esc and lc operands. These are explicit sources and 
  // destinations to brf in the compiler, but not in PalyDoh assembly
  Operand esc_opnd(new Macro_reg(ESC_REG));
  Operand lc_opnd(new Macro_reg(LC_REG));

  // Construct SWP loop branch
  swpbr =  new Op(BRF_B_B_F);
  assert(swpbr && "out of memory");
  swpbr->set_flag(EL_OPER_SWP_BRANCH);
  swpbr->set_dest(swpbr->first_dest(), staging_predicate);  
  swpbr->set_dest((swpbr->first_dest()) + 1 , lc_opnd);  
  swpbr->set_dest((swpbr->first_dest()) + 2, esc_opnd);  
  swpbr->set_src(swpbr->first_src(), *brtarget);
  swpbr->set_src((swpbr->first_src()) + 1, lc_opnd);
  swpbr->set_src((swpbr->first_src()) + 2, esc_opnd);

  // SAM 8-2000, if original branch had width attribute, make a suitable
  // width attr for the new branch
  Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(non_swpbr);
  if (rdt_attr != NULL) {
    Reference_dtype_attr *swp_rdt_attr = new Reference_dtype_attr(swpbr);
    // With the BRLC, LC is DEST1/SRC2, target is SRC1
    swp_rdt_attr->set_dest(DEST1, staging_predicate.dtype());
    swp_rdt_attr->set_dest(DEST2, rdt_attr->dest(DEST1));
    swp_rdt_attr->set_dest(DEST3, esc_opnd.dtype());
    swp_rdt_attr->set_src(SRC1, rdt_attr->src(SRC1));
    swp_rdt_attr->set_src(SRC2, rdt_attr->dest(DEST1));
    swp_rdt_attr->set_src(SRC3, esc_opnd.dtype());
    set_reference_dtype_attr(swpbr, swp_rdt_attr);
  }
  return staging_predicate;
}


// Constructs a swp branch (and other ops) assuming no hardware
// support for modulo scheduling exists. The branch is returned, and
// support_ops is filled in with the non branch ops needed to perform
// the same function as a BRF.
Operand msched_construct_mve_swp_branch(Op* non_swpbr, Op*& swpbr, List<Op*>& support_ops)
{
  support_ops.clear();

  ////////////////////////////////////////////////////////////////////////////
  // The operation sequence we're creating is...
  //
  // lc = lc - 1 if st_pred
  // st_pred = cmpp lc > 0
  // esc = esc - 1 if ! st_pred
  // br_pred = cmpp esc > 0
  // brct if br_pred


  ////////////////////////////////////////////////////////////////////////////
  // First create some operands to be used later.

  Operand brtarget = non_swpbr->src(non_swpbr->first_src());

  Operand staging_predicate(new Reg(EL_STANDARD_PREDICATE));
  staging_predicate.bind_file(PR, STATIC_REG);
  Operand not_staging_predicate(new Reg(EL_STANDARD_PREDICATE));
  not_staging_predicate.bind_file(PR, STATIC_REG);
  Operand branch_predicate(new Reg(EL_STANDARD_PREDICATE));
  branch_predicate.bind_file(PR, STATIC_REG); 

  Operand esc_opnd(new Macro_reg(ESC_REG));
  Operand lc_opnd(new Macro_reg(LC_REG));
  Operand zero(new Int_lit(0));
  Operand one(new Int_lit(1));


  ////////////////////////////////////////////////////////////////////////////
  // Construct SWP loop branch

  swpbr =  new Op(BRCT);
  assert(swpbr && "out of memory");
  swpbr->set_flag(EL_OPER_SWP_BRANCH);
  swpbr->set_src(SRC1, brtarget);
  swpbr->set_src(SRC2, branch_predicate);
  swpbr->set_src(PRED1, non_swpbr->src(PRED1));


  ////////////////////////////////////////////////////////////////////////////
  // Now the other ops.

  Op* lc_sub = new Op((Opcode)SUB_W);
  assert(lc_sub && "out of memory");
  lc_sub->set_flag(EL_OPER_SWP_BRANCH);
  lc_sub->set_src(PRED1, staging_predicate);
  lc_sub->set_src(SRC1, lc_opnd);
  lc_sub->set_src(SRC2, one);
  lc_sub->set_dest(DEST1, lc_opnd);
  support_ops.add_tail(lc_sub);

  Op* lc_cmp = new Op((Opcode)CMPP_W_GT_UN_UC);
  assert(lc_cmp && "out of memory");
  lc_cmp->set_flag(EL_OPER_SWP_BRANCH);
  // This is the predicate 'true'
  lc_cmp->set_src(PRED1, swpbr->src(PRED1));
  lc_cmp->set_src(SRC1, lc_opnd);
  lc_cmp->set_src(SRC2, zero);
  lc_cmp->set_dest(DEST1, staging_predicate);
  lc_cmp->set_dest(DEST2, not_staging_predicate);
  support_ops.add_tail(lc_cmp);

  Op* esc_sub = new Op((Opcode)SUB_W);
  assert(esc_sub && "out of memory");
  esc_sub->set_flag(EL_OPER_SWP_BRANCH);
  esc_sub->set_src(PRED1, not_staging_predicate);
  esc_sub->set_src(SRC1, esc_opnd);
  esc_sub->set_src(SRC2, one);
  esc_sub->set_dest(DEST1, esc_opnd);
  support_ops.add_tail(esc_sub);

  Op* esc_cmp = new Op((Opcode)CMPP_W_GT_UN_UN);
  assert(esc_cmp && "out of memory");
  esc_cmp->set_flag(EL_OPER_SWP_BRANCH);
  esc_cmp->set_src(PRED1, swpbr->src(PRED1));
  esc_cmp->set_src(SRC1, esc_opnd);
  esc_cmp->set_src(SRC2, zero);
  esc_cmp->set_dest(DEST1, branch_predicate);
  support_ops.add_tail(esc_cmp);

  

  return staging_predicate;
}


static void msched_insert_predicates(Compound_region* body, const Operand& staging_predicate)
{
  /*  for all ops except brtop and wtop
   *  if predicate input is undefined or true
   *    set predicate input to staging predicate
   * Note that remap for staging predicate happens before brtop so
   * evr offset for both brtop dest and pred input for ops is the same.
   */ 
  Op* op;
  for (Region_all_ops ops(body); ops != 0; ops++) {
    op = *ops;
    if(is_pseudo(op) || is_swp_branch(op) || is_pbr(op) || is_branch(op))
      continue;
    
    // In modulo variable expansion, this cmpp is part of the expanded
    // BRF, and thus must be predicated differently to correctly set
    // the branch condition.
    if(is_cmpp(op) && op->src(SRC1).is_macro_reg() && op->src(SRC1).name() == ESC_REG)
      continue;
    if(is_cmpp(op) && op->src(SRC1).is_macro_reg() && op->src(SRC1).name() == LC_REG)
      continue;

    Operand predicate = op->src(PRED1);
    if (predicate.is_undefined() || predicate.is_predicate_true()) {
      op->set_src(PRED1, staging_predicate);
    }
  }
}

static void msched_hoist_pbr(LoopBody* loop) {
  Compound_region* kernel = loop->kernel();
  Compound_region* preloop = loop->preloop();
  Op *op;
  bool found = false;

  for (Region_ops_C0_order ops(kernel); ops != 0; ops++) {
    op = *ops;

    if (is_pbr(op) || is_movelb(op) || is_movelbx(op) || is_movelbs(op)) {
      El_remove_op(op);
      El_insert_op_before_switch(preloop, op);

      if (is_pbr(op)) {
	if (found) El_punt("MSCHED: Can't hoist more than one pbr.");
	found = true;
      }
    }
  }
}

// --------------------------- Post processing of preloop --------------------
// Does only one thing
//   1. Inserts ops to set esc in preloop.
//   2. Clear all rotating registers in preloop.
//   3. Set staging predicate in preloop. 
//   4. Insert copies from vrs to rotating regsiters for live-ins in the
//       preloop
//   5. Insert copies from rotating registers to vrs for live-outs in post-loop
//   Note: 4 and 5 are a crutch . They will not be done when scalar allocator 
//         is in place.

static void msched_insert_esc_op(Compound_region* preloop, int esc);
static void msched_insert_predicates_clear(Compound_region* preloop, LoopBody* loop);
static void msched_insert_loop_skip_branch(LoopBody* loop);
static void msched_adjust_stimes(LoopBody* loop);
static void msched_insert_preloop_copies(Compound_region* preloop,
					 Map<Operand, int>& inmap,
					 Operand& stage_pred, Cluster_mgr& cmgr,
                                         int staging_pred_cid); 
static void msched_insert_postloop_copies(Compound_region* postloop,
					 Map<Operand, int>& outmap,
                                         Cluster_mgr& cmgr); 
static void msched_insert_copy_op(Compound_region* block, 
				  Operand& src, Operand& dest);
static void msched_simplify_preloop(Compound_region* preloop);

void msched_postprocess(LoopBody* loop, Cluster_mgr& cmgr)
{
  Compound_region* preloop = loop->preloop();
  Compound_region* postloop = loop->postloop();
  Operand stage_pred =  loop->get_staging_pred();

  // staging predicate cluster id is same as the cluster id of the 
  // BRF in the kernel. Rajiv 07/03
  int staging_pred_cid = cmgr.get_staging_pred_cid(loop->kernel());

  // Set esc in preloop
  int esc = get_msched_attr(loop)->esc;
  msched_insert_esc_op(preloop, esc);

  // Insert pred clear in preloop
  msched_insert_predicates_clear(preloop, loop);

  if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
    // MVE will insert a remainder loop to handle iterations that aren't
    // a multiple of the unroll factor (e.g., if a loop was unrolled 3
    // times, but you want 4 iterations, then the unrolled loop can do 3
    // of them and a remainder loop has to take care of the 4th). This
    // function inserts a branch over the msched loop if the remainder
    // loop handles everything (e.g., if the loop was unrolled 3 times,
    // but you only want 2 iterations).
    msched_insert_loop_skip_branch(loop);
  }

  if(El_msched_algorithm == SWING_MSCHED) {
    // Because of the swing algorithm, the first op may not always be
    // scheduled at cycle zero. This function adjusts all the ops'
    // sched times.
    msched_adjust_stimes(loop);
  }

  // Insert copies in preloop including staging pred set
  Map<Operand, int>* inmap = (Map<Operand, int>*)
                               get_generic_attribute(loop, "Entry_alloc_map");
  if (inmap) msched_insert_preloop_copies(preloop, *inmap, stage_pred, cmgr, staging_pred_cid);
  // Insert copies in postloop
  Map<Operand, int>* outmap = (Map<Operand, int>*) 
                               get_generic_attribute(loop, "Exit_alloc_map");

//   // postloop copies not supported for loop accelerator. -KF 10-2004
//   if (outmap && !outmap->is_empty())
//     El_warn("register liveouts from modulo scheduled loops not supported");

  msched_simplify_preloop(preloop);

  if (outmap) msched_insert_postloop_copies(postloop, *outmap, cmgr);
}


// WARNING: This code is tightly coupled to the code that creates the
// preloop block in El_Loop::insert_brlc() in Control/el_loop.cpp
void msched_simplify_preloop(Compound_region* preloop)
{
  // Get rid of divides by powers of two.
  for(Region_all_ops rops(preloop); rops != 0; rops++) {
    Op* op = *rops;
    if(is_int_div(op) && (op->dest(DEST1) == op->src(SRC1))) {
      if(op->src(SRC2).is_int()) {
	if(op->src(SRC2).int_value() == 1) {
	  El_remove_op(op);
	  delete op;
	  break;
	} else {
	  int const_val = op->src(SRC2).int_value();
	  // This checks if it's a power of two
	  if((const_val & (const_val - 1)) == 0) {
	    int shift_val = 0;
	    for(; shift_val < 32; shift_val++) {
	      if((const_val >> shift_val) & 1) {
		break;
	      }
	    }
	    Op* shift = new Op((Opcode) SHRA_W);
	    assert(shift && "out of memory");
	    shift->set_src(PRED1, op->src(PRED1));
	    shift->set_src(SRC1,  op->src(SRC1));
	    shift->set_src(SRC2, Operand(new Int_lit(shift_val)));
	    shift->set_dest(DEST1, op->dest(DEST1));
	    El_insert_op_after(shift, op);
	    El_remove_op(op);
	    delete op;
	  } // if power of two
	}
      } // if src2 is an int literal
    }
  } // for all ops

  // Get rid of the CMPP with constant sources.
  for(Region_ops_C0_order rops(preloop); rops != 0; rops++) {
    Op* op = *rops;
    if(op->opcode() == ((Opcode)CMPP_W_GEQ_UN_UC)) {
      // Both srcs are constant; we can evaluate here.
      if(op->src(SRC1).is_int() && op->src(SRC2).is_int()) {
	// Find the two ADDs that go with this compare.
	Op* add_if_t = NULL;
	Op* add_if_f = NULL;

	for(Region_ops_C0_order rops2(preloop, op); rops2 != 0; rops2++) {
	  Op* op2 = *rops2;
	  if(op->dest(DEST1) == op2->src(PRED1)) {
	    add_if_t = op2;
	  }
	  if(op->dest(DEST2) == op2->src(PRED1)) {
	    add_if_f = op2;
	  }
	}

	// we found the corresponding adds.
	if(add_if_t && add_if_f) {
	  // If the CMPP evaluates to true, remove the ADD predicated
	  // on false, and update the predicate. If the CMPP is false,
	  // do vice-versa. In either case remove the CMPP from the
	  // code.

	  if(op->src(SRC1).int_value() >= op->src(SRC2).int_value()) {
	    El_remove_op(add_if_f);
	    delete add_if_f;
	    add_if_t->set_src(PRED1, Operand(new Pred_lit(true)));
	  } else {
	    El_remove_op(add_if_t);
	    delete add_if_t;
	    add_if_f->set_src(PRED1, Operand(new Pred_lit(true)));
	  }
	  El_remove_op(op);
	  delete op;
	  break;
	}
      } // if both CMPP srcs are literals
    } // if this is a CMPP
  } // for all ops

  // Now find strings where the LC is ADDed or SUBed by constants.
  bool change = true;
  while(change) {
    change = false;
    List<Op*> string_of_lc_arith;
    int total_lc_inc = 0;
    for(Region_ops_C0_order rops(preloop); rops != 0; rops++) {
      Op* op = *rops;
      if((is_int_add(op) || is_int_sub(op)) && (op->dest(DEST1) == op->src(SRC1)) &&
	 op->src(SRC1).is_macro_reg() && (op->src(SRC1).name() == LC_REG) &&
	 op->src(SRC2).is_int() && op->src(PRED1).is_predicate_true()) {
	string_of_lc_arith.add_tail(op);
	if(is_int_add(op)) {
	  total_lc_inc += op->src(SRC2).int_value();
	} else {
	  total_lc_inc -= op->src(SRC2).int_value();
	}
      } else {
	// Check for LC uses/defs that don't fit this pattern, meaning
	// the chain is killed.
	bool lc_seen = false;
	for(Op_all_sources srcs(op); (srcs != 0) && (! lc_seen); srcs++) {
	  if((*srcs).is_macro_reg() && (*srcs).name() == LC_REG) {
	    lc_seen = true;
	  }
	}
	for(Op_all_dests dests(op); (dests != 0) && (! lc_seen); dests++) {
	  if((*dests).is_macro_reg() && (*dests).name() == LC_REG) {
	    lc_seen = true;
	  }
	}

	// We found an op that will kill the chain, because this
	// function doesn't know how to statically evaluate it.
	if(lc_seen) {
	  if(string_of_lc_arith.size() >= 2) {
	    change = true;
	    
	    Op* head = string_of_lc_arith.head();
	    if(total_lc_inc) {
	      if(is_int_add(head)) {
		head->set_src(SRC2, Operand(new Int_lit(total_lc_inc)));
	      } else {
		head->set_src(SRC2, Operand(new Int_lit(-total_lc_inc)));
	      }
	      for(List_iterator<Op*> li(string_of_lc_arith); li != 0; li++) {
		if((*li) != head) {
		  El_remove_op(*li);
		  delete *li;
		}
	      }
	    } else {
	      for(List_iterator<Op*> li(string_of_lc_arith); li != 0; li++) {
		El_remove_op(*li);
		delete *li;
	      }
	    }

	    string_of_lc_arith.clear();
	    total_lc_inc = 0;
	    break;
	  }
	  string_of_lc_arith.clear();
	  total_lc_inc = 0;
	}
      }
    } // for all ops

    // This is needed to catch the last chain.
    if(string_of_lc_arith.size() >= 2) {
      Op* head = string_of_lc_arith.head();
      if(total_lc_inc) {
	if(is_int_add(head)) {
	  head->set_src(SRC2, Operand(new Int_lit(total_lc_inc)));
	} else {
	  head->set_src(SRC2, Operand(new Int_lit(-total_lc_inc)));
	}
	for(List_iterator<Op*> li(string_of_lc_arith); li != 0; li++) {
	  if((*li) != head) {
	    El_remove_op(*li);
	    delete *li;
	  }
	}
      } else {
	for(List_iterator<Op*> li(string_of_lc_arith); li != 0; li++) {
	  El_remove_op(*li);
	  delete *li;
	}
      }
      
      string_of_lc_arith.clear();
      total_lc_inc = 0;
      break;
    }
  } // while(change)
}


// set the epilogue stage count register
void msched_insert_esc_op(Compound_region* preloop, int esc){
  Operand esc_opd(new Macro_reg(ESC_REG));
  Operand esc_lit(new Int_lit(esc));
  msched_insert_copy_op(preloop, esc_lit, esc_opd);
}

// Clear all the predicates. This needs to change after we decide how to
// proceed. Also edge drawing has to recognize this situation.

void msched_insert_predicates_clear(Compound_region* preloop, LoopBody* loop)
{
  Op* clear_pred = new Op(PRED_CLEAR_ALL_ROTATING);
  Operand all_rot_pred(new Macro_reg(ALL_ROT_PRED));
  clear_pred->set_dest(clear_pred->first_dest(), all_rot_pred);
  El_insert_op_before_switch(preloop, clear_pred);

  // Need the loop predicate set to true. The ESC cmpp will eventually
  // turn it false, but this cmpp may be scheduled until the same
  // cycle as the branch, meaning it may not execute before the branch
  // on the first iteration.
  if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
    // Find the predicate guarding the branch.
    Region_exit_ops exits(loop);
    Operand pred_to_set = (*exits)->src(SRC2);

    Op* pred_set = new Op((Opcode) CMPP_W_EQ_UN_UN);
    assert(pred_set && "out of memory");

    // Compare LC == LC to set the predicate.
    pred_set->set_src(PRED1, Operand(new Pred_lit(true)));
    pred_set->set_src(SRC1, Operand(new Macro_reg(LC_REG)));
    pred_set->set_src(SRC2, Operand(new Macro_reg(LC_REG)));
    pred_set->set_dest(DEST1, pred_to_set);
    
    // Mark the op, so dead code elimination doesn't get it. This
    // shouldn't need to be done, but there's some major issues doing
    // dataflow analysis on modulo scheduled loops.
    pred_set->set_flag(EL_OPER_LIVEOUT_DEST);

    El_insert_op_before_switch(preloop, pred_set);
  }
}


void msched_insert_loop_skip_branch(LoopBody* loop)
{
  Operand pred_to_set(new Reg(EL_STANDARD_PREDICATE));
  Op* new_cmpp = new Op((Opcode) CMPP_W_LEQ_UN_UN);
  assert(new_cmpp && "out of memory");

  new_cmpp->set_src(PRED1, Operand(new Pred_lit(true)));
  new_cmpp->set_src(SRC1, Operand(new Macro_reg(LC_REG)));
  new_cmpp->set_src(SRC2, Operand(new Int_lit(0)));
  new_cmpp->set_dest(DEST1, pred_to_set);

  Operand br_dest(new Reg(EL_STANDARD_BRANCH));
  Op* new_pbr = new Op((Opcode) PBR);
  assert(new_pbr && "out of memory");
  new_pbr->set_dest(DEST1, br_dest);
  new_pbr->set_src(SRC1, Operand(new Cb_operand(loop->postloop()->id())));
  new_pbr->set_src(SRC2, Operand(new Int_lit(1)));
  new_pbr->set_src(PRED1, new_cmpp->src(PRED1));

  Op* new_brct = new Op((Opcode) BRCT);
  assert(new_brct && "out of memory");
  new_brct->set_src(SRC1, br_dest);
  new_brct->set_src(SRC2, pred_to_set);
  new_brct->set_src(PRED1, new_cmpp->src(PRED1));

  Compound_region* preloop = loop->preloop();

  El_insert_op_before_switch(preloop, new_pbr);
  El_insert_op_before_switch(preloop, new_cmpp);
  El_insert_op_before_switch(preloop, new_brct);

  preloop->add_exit(new_brct);

  // FIX: do I need to set this?
  Lcode_attribute_map *lc_attr = get_lcode_attributes(new_brct);
  if(lc_attr && lc_attr->is_bound("pbr_id")) {
    (*lc_attr)["pbr_id"].clear();
    Operand tmp_oper(new Int_lit(new_pbr->id()));
    (*lc_attr)["pbr_id"].add_tail(tmp_oper);
  }

  Op* dummy_br = get_last_region_op_from_subregions(preloop);
  Edge* ft_edge = get_outgoing_CONTROL0_fall_through_edge(dummy_br);
  preloop->remove_exit(dummy_br);

  dummy_br->remove_outedge(ft_edge, ft_edge->src_port_type(),
			   ft_edge->src_port(), ft_edge->src_alt());
  ft_edge->set_src(new_brct, ft_edge->src_port(), ft_edge->src_alt());
  new_brct->add_outedge_recursively(ft_edge);

  // Set the taken edge.
  Op* postloop_header = get_first_region_op_from_subregions(loop->postloop());
  Edge* tk_edge = C0_connect_taken(new_brct, postloop_header);
  new_brct->add_outedge_recursively(tk_edge);
  postloop_header->add_inedge_recursively(tk_edge);

  // FIX: Make the region weights correct.
  Control_flow_freq* cfreq = new Control_flow_freq();
  assert(cfreq && "out of memory");
  cfreq->freq = preloop->weight;
  set_control_flow_freq(tk_edge, cfreq);

  Edge* bad_edge = get_incoming_CONTROL0_edge(dummy_br);
  new_brct->remove_outedge(bad_edge, CONTROL, CONTROL0, bad_edge->src_alt());

  preloop->remove_region(dummy_br);
  delete dummy_br;
}


void msched_adjust_stimes(LoopBody* loop)
{
  int min_stime = ELCOR_MAX_INT;
  for(Region_all_ops rops(loop->kernel()); rops != 0; rops++) {
    Op* op = *rops;
    if( ! is_pseudo(op)) {
      min_stime = ELCOR_MIN(min_stime, op->sched_time());
    }
  }

  for(Region_all_ops rops(loop->kernel()); rops != 0; rops++) {
    Op* op = *rops;
    if( ! is_pseudo(op)) {
      op->set_sched_time(op->sched_time() - min_stime);
    }
  }
}


void msched_insert_preloop_copies(Compound_region* preloop, 
				  Map<Operand, int>& inmap,
				  Operand& stage_pred, Cluster_mgr& cmgr,
                                  int staging_pred_cid){

  for (Map_iterator<Operand, int> iter(inmap) ; iter != 0 ; iter++) {
    Operand& src = (*iter).first;
    int reg_num = (*iter).second;

    // Test for the staging predicate specially and initialize it with a PRED_SET
    if (src.is_reg() && src.file_type() == PR && 
	src.vr_num() == stage_pred.vr_num()) {
      Op* pred_set_op = new Op(PRED_SET);
      // make a copy and bind the staging predicate operand according to the map
      Operand pred_oprnd = Operand(new Reg(src));
      pred_oprnd.bind_file(PR, ROTATING);

      assert(Codegen::Vr_Home_Prf.is_bound(src.vr_num()));
      eString prf = Codegen::Vr_Home_Prf.value(src.vr_num());
      pred_oprnd.bind_physical_file(prf);
      pred_oprnd.bind_reg(reg_num);

      int cid = cmgr.get_cluster_for_physical_file(prf);
      assert(cid == staging_pred_cid);
      cmgr.operand_map.bind(pred_oprnd.vr_num(), staging_pred_cid);
      
      pred_set_op->set_dest(pred_set_op->first_dest(), pred_oprnd);
      El_insert_op_before_switch(preloop, pred_set_op);
    } else {
      // make a copy and bind it according to the map
      Operand move_src(new Reg(src.dtype(), src.vr_num(), src.omega()));
      Operand move_dest(new Reg(src.dtype(), src.vr_num(), src.omega()));
      int clstr = cmgr.operand_map.value(move_dest.vr_num());

      move_dest.bind_file(move_dest.file_type(),ROTATING);
      
      assert(Codegen::Vr_Home_Prf.is_bound(src.vr_num()));
      eString prf = Codegen::Vr_Home_Prf.value(src.vr_num());

      // bind it temporarily so that cluster lookup for this operand
      // succeeds while clustering the preloop.
      // Codegen later unbinds and rebinds it so that it is compatible with the MDES, Rajiv 07/03
      move_dest.bind_physical_file(prf);

      move_dest.bind_reg(reg_num);

      int cid = cmgr.get_cluster_for_physical_file(prf);
      assert(cid == clstr);
      cmgr.operand_map.bind(move_dest.vr_num(), clstr);

      msched_insert_copy_op(preloop, move_src, move_dest);


      // Operand dest = Operand(src);
      // dest.bind_reg(reg_num);
      // dest.bind_file(dest.file_type(),ROTATING);
      // msched_insert_copy_op(preloop, src, dest);
    }
  }
}

void msched_insert_postloop_copies(Compound_region* postloop, 
				   Map<Operand, int>& outmap,
                                   Cluster_mgr& cmgr){
  for (Map_iterator<Operand, int> iter(outmap) ; iter != 0 ; iter++) {
    Operand& dest = (*iter).first;
    int reg_num = (*iter).second;
    // make a copy and bind it according to the map
    Operand move_src(new Reg(dest.dtype(), dest.vr_num(), dest.omega()));
    int clstr = cmgr.operand_map.value(move_src.vr_num());

    move_src.bind_file(move_src.file_type(),ROTATING);

    assert(Codegen::Vr_Home_Prf.is_bound(dest.vr_num()));
    eString prf = Codegen::Vr_Home_Prf.value(dest.vr_num());

    // bind it temporarily so that cluster lookup for this operand
    // succeeds while clustering the postloop.
    // Codegen later unbinds and rebinds it so that it is compatible with the MDES, Rajiv 07/03
    move_src.bind_physical_file(prf);

    move_src.bind_reg(reg_num);

    int cid = cmgr.get_cluster_for_physical_file(prf);
    assert(cid == clstr);
    cmgr.operand_map.bind(move_src.vr_num(), clstr);

    // Operand src = Operand(dest);
    // src.bind_reg(reg_num);
    // src.bind_file(src.file_type(),ROTATING);

    // This inserts the instructions just before the switch rather
    // than just after the merge. 10/04/95 Change this later.
    msched_insert_copy_op(postloop, move_src, dest);
  }
}

void msched_insert_copy_op(Compound_region* block, 
			   Operand& src, Operand& dest){
  Op* copy_op;

  Opcode copy_opcode = get_move_opcode_for_operands(dest, src);

  copy_op = new Op(copy_opcode);

//   switch (dest.file_type()) {
//   case GPR: //copy_op = new Op(MOVE);
//      copy_op = new Op(MOVEGG);
//               break;
//   case CR:  copy_op = new Op(MOVE);
//               break;
//     case FPR: if (dest.dtype().is_single())
// 		copy_op = new Op(MOVEF_S);
// 	      else if ( dest.dtype().is_double()) 
// 		copy_op = new Op(MOVEF_D);
// 	      else El_punt("msched_insert_copy_op: Wachy floating point width not supported");
// 	      break;
//     case PR: // copy_op = new Op(MOVEPG);
//        // 8-20-01
//        copy_op = new Op(MOVEPP);
//        //              El_warn("msched_insert_copy_op: General Predicate copies not possible");
//        //      return;
//        break;
//     default: El_warn("msched_insert_copy_op: Unsupported file type--BTR");
// 	     return;
// 	    }
  Operand pred_true = new Pred_lit(true);
  copy_op->set_src(PRED1, pred_true);
  copy_op->set_src(copy_op->first_src(), src);
  copy_op->set_dest(copy_op->first_dest(), dest);
  El_insert_op_before_switch(block, copy_op);

}

// -------------------------   Construct region for analysis ------------------
static Compound_region* msched_construct_compound_region(List<Region*>& rlist);

Compound_region* msched_construct_ppl_region(LoopBody* loop)
{
    List<Region*> l;
    Compound_region* preloop = (Compound_region*)loop->preloop();
    Compound_region* postloop = (Compound_region*)loop->postloop();
    l.add_tail(preloop);
    l.add_tail(loop);
    l.add_tail(postloop);
    return (msched_construct_compound_region(l));
}


Compound_region* msched_construct_compound_region(List<Region*>& rlist)
{
    Region* first = rlist.head();
    Region* last = rlist.tail();
    Compound_region* parent = (Compound_region*)(first->parent());
    Compound_region* new_region = new Compound_region;
    parent->insert_before_region(new_region, first);
    new_region->set_parent(parent);
    for(List_iterator<Region*> regs(rlist); regs !=0; regs++){
	Region* reg = *regs; 
	parent->remove_region(reg);
	new_region->add_region(reg);
	reg->set_parent(new_region);
    }

    for(Region_entry_edges in_edges(first); in_edges != 0; in_edges++){
	new_region->add_entry_safely(*in_edges);
    }
    for(Region_exit_edges out_edges(last); out_edges!= 0; out_edges++){
	new_region->add_exit_safely(*out_edges);
    }

    for(Region_entry_ops in_ops(first); in_ops != 0; in_ops++){
	new_region->add_entry(*in_ops);
    }
    for(Region_exit_ops out_ops(last); out_ops!= 0; out_ops++){
	new_region->add_exit(*out_ops);
    }
    return new_region;
}

// ---------------- Map livenes information taking allocation into account ----
static void msched_update_dataflow_set(Liveness_info* dataflow_set,
				       Map<Operand, int>* operand_map);


void msched_map_liveness_info(LoopBody* loop){
  Map<Operand, int>* inmap = (Map<Operand, int>*) 
                               get_generic_attribute(loop, "Entry_alloc_map");
  Map<Operand, int>* outmap = (Map<Operand, int>*)
			       get_generic_attribute(loop, "Exit_alloc_map");
  Edge* inedge = get_connecting_CONTROL0_edge(loop->preloop(), loop);
  Edge* outedge = get_connecting_CONTROL0_edge(loop, loop->postloop());

  msched_update_dataflow_set(get_liveness_info(inedge), inmap);
  msched_update_dataflow_set(get_liveness_info(outedge), outmap);
  msched_update_dataflow_set(get_down_exposed_uses_info(inedge), inmap);
  msched_update_dataflow_set(get_down_exposed_uses_info(outedge), outmap);
  msched_update_dataflow_set(get_up_exposed_defs_info(inedge), inmap);
  msched_update_dataflow_set(get_up_exposed_defs_info(outedge), outmap);
  msched_update_dataflow_set(get_down_exposed_defs_info(inedge), inmap);
  msched_update_dataflow_set(get_down_exposed_defs_info(outedge), outmap);
}

void msched_update_dataflow_set(Liveness_info* dataflow_set,
				Map<Operand, int>* operand_map) {
  if (dataflow_set == NULL || operand_map == NULL) return;
  for (Liveness_info_iterator liter(*dataflow_set) ; liter != 0 ; liter++) {
      // apply the operand allocation map to each live operand
      Operand& liveop = *liter;
      if (operand_map->is_bound(liveop)) {
	  int reg_num = operand_map->value(liveop);
	  liveop.bind_reg(reg_num);
	  liveop.bind_file(liveop.file_type(),ROTATING);
      }
  }
}


// ---------------- delete msched specific attributes ------------------------
void msched_delete_region_attributes(LoopBody* loop){
    Map<Operand, int>* inmap = (Map<Operand, int>*) 
                                get_generic_attribute(loop, "Entry_alloc_map");
    remove_generic_attribute(loop, "Entry_alloc_map");
    if (!(inmap == NULL)) delete inmap;
    Map<Operand, int>* outmap = (Map<Operand, int>*)
	                         get_generic_attribute(loop, "Exit_alloc_map");
    remove_generic_attribute(loop, "Exit_alloc_map");
    if (!(outmap == NULL)) delete outmap;

    // delete unrolled_sched_time attributes. -KF 1/2005
    for (Region_all_ops opi(loop->kernel()); opi!=0; opi++) {
      Op* op = *opi;
      int* ustime = (int*)get_generic_attribute(op, "unrolled_sched_time");
      remove_generic_attribute(op, "unrolled_sched_time");
      if (ustime != NULL) delete ustime;
    }
}
