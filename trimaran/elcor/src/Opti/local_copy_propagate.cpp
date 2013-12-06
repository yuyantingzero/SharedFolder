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
//									 //
//      File:           local_copy_propagate.cpp                         //
//      Authors:        Vinod Kathail, Shail Aditya                      //
//      Created:        August 1996					 //
//      Description:    Simple Copy propagation for demo		 //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include "local_copy_propagate.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "connect.h"
#include "flow_analysis_solver.h"
#include "attributes.h"
#include "dbg.h"
#include "el_opti_dead_code.h"
#include "list.h"
#include "list_set.h"
#include "el_opti_utilities.h"

static void do_forward_copy_propagation(Compound_region* region, 
					Reaching_defs_info* rdef);
static void forward_copy_propagate_hb_bb(Compound_region* region,
					 Reaching_defs_info* rdef);
static bool check_and_substitute(Compound_region* region,
				 Reaching_defs_info* rdef,  Op* copy_op,
				 Operand& src, El_ref& src_ref, 
				 Operand& dest, El_ref& use_ref);
static bool substitute_lit(Reaching_defs_info* rdef, Operand& src, 
			   Operand& dest, El_ref& use_ref);
static void do_backward_copy_propagation(Compound_region* region, 
					 Reaching_defs_info* rdef);
static void backward_copy_propagate_hb_bb(Compound_region* region,
					  Reaching_defs_info* rdef);
static bool success=false;

void local_copy_propagation(Compound_region* region)
{
  // SAM 8-2000, changed to do forward copy first.  This is so data type
  // info coming from suif is first propagated downward as far as possible.
  // Propagating up first can lose some sign/unsign info because backward
  // copy prop does not preserve this accurately.  So, this is a temp fix
  // to avoid fixing the real problem :)

  do {
    success = false;
    forward_local_copy_propagation(region);
    el_opti_eliminate_dead_code(region);
  } while (success);

  do {
    success = false;
    backward_local_copy_propagation(region);
    el_opti_eliminate_dead_code(region);
  } while (success);

}

// The top level driver will change so that we don't do analysis over and 
// over again for different optimizations.  The copy propagation does update
//  reaching definition info. Also reaching definitions will be run on hb 
// basis once reaching defintions capture live ins/outs.

void forward_local_copy_propagation(Compound_region* region)
{
    if (dbg(opti,1) || dbg(status, 3))
	cdbg << "-- Local forward copy propagation " << endl;

   // Do reaching defs over the entire region (assume procedure)
   create_local_analysis_info_for_all_hbs_bbs(region);
   El_do_reaching_defs (region, ANALYZE_ALLREG) ;
   Reaching_defs_info* rdef = get_reaching_defs_info (region) ;

   // Do copy propagation
   do_forward_copy_propagation(region, rdef);

   delete_local_analysis_info_for_all_hbs_bbs(region);
   remove_reaching_defs_info (region) ;
}

// Simple forward copy propgation.
static void do_forward_copy_propagation(Compound_region* region, 
					Reaching_defs_info* rdef)
{
    if (region->is_hb() || region->is_bb()) {
	forward_copy_propagate_hb_bb(region, rdef);
    }
    else {
	for(Region_subregions subs(region); subs != 0; subs++) {
	    do_forward_copy_propagation((Compound_region*)*subs, rdef);
	}
    }
}

static void forward_copy_propagate_hb_bb(Compound_region* region, 
                                          Reaching_defs_info* rdef)
{
  // Go over all ops in forward order looking for a copy op 
  for(Region_ops_C0_order ops(region); ops != 0; ops++) {
    Op* an_op = *ops;
    if (is_copy(an_op)) {
      if (dbg(opti,4))
        cdbg << "\tConsidering move op: " << an_op->id() << endl;

      // Get the source operand as well as reference to it
      Op_explicit_sources srcs(an_op);
      Operand src_opnd = *srcs;
      El_ref src_ref = srcs.get_ref();
      // Get the destination operand as well as reference to it
      Op_explicit_dests dests(an_op);
      Operand dest_opnd = *dests;
      El_ref dest_ref = dests.get_ref();
      if (dest_opnd.is_reg() || dest_opnd.is_macro_reg()) {
        List<El_ref> dead_uses;
        List<El_ref>* uses = rdef->get_du_chain(dest_ref) ;

        // Integer literal case
        if (src_opnd.is_int() || src_opnd.is_float() ||
            src_opnd.is_double() || src_opnd.is_predicate() ||
            src_opnd.is_string() || src_opnd.is_label()) {
          // Now iterate over du chains of the dest operand of copy
          for (List_iterator<El_ref> use_iter1(*uses); 
              use_iter1 != 0; use_iter1++) {
            El_ref& an_use = *use_iter1;
            if (substitute_lit(rdef, src_opnd, dest_opnd, an_use)) {
              dead_uses.add_head(an_use);
              success = true;
              if (dbg(opti,3))
                cdbg << "\t\tPropagating lit: From " << an_op->id()
                    << "  To USE " << (an_use.get_op())->id()
                    << endl;
            }
          }
          
          // parkhc 10/21/2005
          // Remove the old uses from reaching defs info. This is done
          // separately to allow the list iterator above to work.
          while(!(dead_uses.is_empty())) {
            El_ref dead = dead_uses.pop();
            rdef->remove_use(dead);

            // substitute operand after removing use_ref
            Op* use_op = dead.get_op();
            Operand new_oprnd(src_opnd);
            use_op->set_src(dead.get_port_num(), new_oprnd);
          }

        }

        // Register case
        else if (src_opnd.is_reg() || src_opnd.is_macro_reg()) {
          // Now iterate over du chains of the dest operand of copy
          for (List_iterator<El_ref> use_iter(*uses); 
              use_iter != 0; use_iter++) {
            El_ref& an_use = *use_iter;
            if (check_and_substitute(region, rdef, an_op, src_opnd,
                src_ref, dest_opnd, an_use)){
              dead_uses.add_head(an_use);
              success = true;
              if (dbg(opti,3))
                cdbg << "\t\tPropagating: From " << an_op->id()
                    << "  To USE " << (an_use.get_op())->id() << endl;
            }
          }

          // Remove the old uses from reaching defs info. This is done
          // separately to allow the list iterator above to work.
          while(!(dead_uses.is_empty())) {
            El_ref dead = dead_uses.pop();
            rdef->remove_use(dead);
          }
        }
        else ; // There is nothing to do
      }
    } else if (is_predicate_clear(an_op) || is_predicate_set(an_op)) {
      // parkhc 10/13/2005
      // When a pred_clear/pred_set op is the only def, propagate F/T values
      // to the uses.
      // This happens sometimes as a result of optis.
      if (dbg(opti, 4))
        cdbg << "\tConsidering pred clear/set op : " << an_op->id() << endl;
      Op_explicit_dests dests(an_op);
      Operand dest_opnd = *dests;
      El_ref dest_ref = dests.get_ref();
      List<El_ref> dead_uses;
      List<El_ref>* uses = rdef->get_du_chain(dest_ref);
      // look for any use
      for (List_iterator<El_ref> use_iter(*uses);
          use_iter != 0; use_iter++) {
        El_ref& an_use = *use_iter;
        if (an_use.get_ref_type() != PRED_SRC ||
            dest_opnd != an_use.get_operand())
          continue;

        // check if only one def for this use
        List<El_ref>* defs = rdef->get_ud_chain(an_use);

        if (defs->size() > 1) 
          continue;

        dead_uses.add_head(an_use);
        success = true;

        if (dbg(opti,3))
          cdbg << "\t\tPropagating: From " << an_op->id()
              << "  To USE " << an_use.get_op()->id() 
              << " (Predicate clear/set) " << endl;
      }

      /*
      uses = rdef->get_du_chain(dest_ref);
      cdbg << "before removing dead use" << endl;
      for (List_iterator<El_ref> use_iter(*uses);
          use_iter != 0; use_iter++) {
        El_ref& an_use = *use_iter;
        cdbg << "  use of op " << an_use.get_op()->id() << " : "
            << an_use << endl;
      }
      */

      // Remove the old uses from reaching defs info. This is done
      // separately to allow the list iterator above to work.
      while(!(dead_uses.is_empty())) {
        El_ref dead = dead_uses.pop();
        rdef->remove_use(dead);

        // parkhc 10/21/2005
        // substitute operand after removing opearnd
        Op* use_op = dead.get_op();
        if (is_predicate_clear(an_op)) {
          // propagate fales value
          Operand predF(new Pred_lit(0));
          use_op->set_src(PRED1, predF);
        } else {
          // propagate true value
          Operand predT(new Pred_lit(1));
          use_op->set_src(PRED1, predT);
        }

      }

      /*
      uses = rdef->get_du_chain(dest_ref);
      cdbg << "after removing dead use" << endl;
      for (List_iterator<El_ref> use_iter(*uses);
          use_iter != 0; use_iter++) {
        El_ref& an_use = *use_iter;
        cdbg << "  use of op " << an_use.get_op()->id() << " : "
            << an_use << endl;
      }
      */
    }
  }
}

// Check if src of the copy is redefined between copy and use. Currently, we
// don't take predicates into account in doing this. Also, we assume that
// there are no remaps between copy and use.       
static bool check_and_substitute(Compound_region* region,
				 Reaching_defs_info* rdef, Op* copy_op, 
				 Operand& src, El_ref& src_ref, 
				 Operand& dest, El_ref& use_ref)
{
    Op* use_op = use_ref.get_op();
    // Make sure that the use op is in the region and that the 
    // ref is an explicit src
    if (!region_contains_op(region, use_op) || 
	(use_ref.get_ref_type() != EXP_SRC) ||
	(dest != use_ref.get_operand()))  return false; 

    // get the ud chain for the use and check that there is only one def 
    // reaching the use.
    List<El_ref>* defs = rdef->get_ud_chain(use_ref);
    if (defs->size() > 1) return false;

    // Check if it is redefined. Start from the the op after the copy op and 
    // scan until use.

    Region_ops_C0_order ops(region, copy_op);
    ops++;
    while ((ops != 0) && (*ops != use_op)) {
	for(Op_all_dests dests(*ops); dests != 0; dests++) {
	    // Redefinition
	    if (src == *dests) return false;
	}
	ops++;
    }
    // If we didn't reach the use op, then the op is at the back.
    if (ops == 0) return false; 

    // We can do copy propagation.

    Operand new_oprnd(src);
    use_op->set_src(use_ref.get_port_num(), new_oprnd);
    use_op->set_src_spatial_ref(use_ref.get_port_num(), src_ref.spatial_ref());

    

    // Construct reference for the new use and hook it up with defs of the src
    // of the copy op. Old use will be removed by the caller.

// Don't do incremental update of reaching def info for now.
/*
    Operand* oprnd_ptr = &(use_op->src(use_ref.get_port_num()));
    El_ref new_ref(use_op, oprnd_ptr, EXP_SRC);
    List<El_ref>* src_defs = rdef->get_ud_chain(src_ref);
    for (List_iterator<El_ref> di(*src_defs); di != 0; di++) {
	rdef->add_du_ud_links(*di, new_ref);
	if (dbg(opti,1))

	    cdbg << "Adding du-ud: <" 
		 << use_op->id() << "  " << oprnd_ptr->vr_num()
		 << "  To" 
		 << ((*di).get_op())->id() << "  " 
		 <<  ((*di).get_operand()).vr_num()
		 << endl;
    }
    */
    // We are done.
    return true;
}

static bool substitute_lit(Reaching_defs_info* rdef, Operand& src, 
			   Operand& dest, El_ref& use_ref)
{
    // Do only if it is an explicit src and the operands match.
    // For literals, we don't need to check that the op is in region or not, 
    // since literals are never killed. 
    if ((use_ref.get_ref_type() != EXP_SRC) || 
	(dest != use_ref.get_operand()))return false; 

    // get the ud chain for the use and check that there is only one def 
    // reaching the use.
    List<El_ref>* defs = rdef->get_ud_chain(use_ref);
    if (defs->size() > 1) return false;

    // parkhc 10/21/2005
    // substitute the operand after updating DU chains.
    // Problem : If operand is substitued here,
    //   use_ref cannot be removed as its data type is not register any more.
    // see : Liveness_solver::is_iteresting(Operand&)
    /*
    Op* use_op = use_ref.get_op();
    Operand new_oprnd(src);
    use_op->set_src(use_ref.get_port_num(), new_oprnd);
    */
    return true;
}

// Simple backward copy propgation. 
void backward_local_copy_propagation(Compound_region* region)
{
    if (dbg(opti,1) || dbg(status, 3))
	cdbg << "-- Local backward copy propagation " << endl;
   // Do reaching defs over the entire region (assume procedure)
   create_local_analysis_info_for_all_hbs_bbs(region);
   El_do_reaching_defs (region, ANALYZE_ALLREG) ;
   Reaching_defs_info* rdef = get_reaching_defs_info (region) ;

   // Do copy propagation
   do_backward_copy_propagation(region, rdef);

   delete_local_analysis_info_for_all_hbs_bbs(region);
   remove_reaching_defs_info (region) ;
}




static void do_backward_copy_propagation(Compound_region* region, 
					 Reaching_defs_info* rdef)
{
    if (region->is_hb() || region->is_bb()) {
	backward_copy_propagate_hb_bb(region, rdef);
    }
    else {
	for(Region_subregions subs(region); subs != 0; subs++) {
	    do_backward_copy_propagation((Compound_region*)*subs, rdef);
	}
    }
}

static void backward_copy_propagate_hb_bb(Compound_region* region, 
					  Reaching_defs_info* rdef)
{
  // Go over all ops in reverse order looking for a copy op 
  for(Region_ops_reverse_C0_order ops(region); ops != 0; ops++) {
    Op* copy_op = *ops;
    if (!is_copy(copy_op)) continue;
    if (dbg(opti,4))
      cdbg << "\tConsidering move op: " << copy_op->id() << endl;

    // We have an op of the form r = move(s)
    // Get the source operand (s) and reference to it
    bool ok_to_propagate = true;
    Op_explicit_sources srcs(copy_op);
    Operand copy_src = *srcs;
    El_ref copy_src_ref = srcs.get_ref();
    // Get the destination operand (r) and refernce to it
    Op_explicit_dests dests(copy_op);
    Operand copy_dest = *dests;
    El_ref copy_dest_ref = dests.get_ref();

    // Check for suitability
    // 1. Source must be a proper register to do copy propagation. 
    //    Macro registers are't copy propagated. 
    // 2. For each reaching definition of copy's source (s):
    //    a. The defining op itself must not be a copy operation.
    //       (relaxed already -- Shail Aditya)
    //    b. The definition of s must be an explicit dest of the op
    //    c. Copy's source must be the only use of the definition
    //       (relaxed -- find all the reaching uses of s from the
    //       reaching defs of the copy's source. Now, the reaching
    //       defs of these other uses must be a subset of the reaching
    //       defs of the copy's source -- 05/13/99 Shail Aditya)
    //       NOTE: THIS IS WRONG. 2c is not sufficient to allow copy prop
    //       because even if conditions are met, false dependencies may
    //       be introduced by propagating. -KF 12/2004
    // 3. a. All reaching defs for copy's source (s) are within the
    //       block. (no livein def) 
    //    b. All uses of those defs are also within the block. (no
    //       liveout use)
    // 4. For ops above the copy upto the first def of s:
    //    a. There must not be any op defining copy's destination (r)
    //       (leads to incorrect down-exposed def of r).
    //    b. There must not be any op using r (leads to incorrect value
    //       for r).  (added 05/13/99 -- Shail Aditya)
    // 5. For ops below the copy downto the last use of s:
    //    a. There must not be any op defining copy's destination (r)
    //       (leads to incorrect down-exposed def of r).
    //    b. For a use of r, its reaching def should be precisely the 
    //       copy (leads to incorrect merge if there are other defs of r)
    //    c. There are no uses of copy's source (s) after the first
    //       redefinition of copy's destination (r) - parkhc 01/28/2005

    // Check 1
    if (!copy_src.is_reg()) continue;

    // Get reaching defs for the copy's source (s)
    List<El_ref>* copy_src_defs = rdef->get_ud_chain(copy_src_ref);
    List_set<El_ref> copy_src_defs_set(*copy_src_defs);
    // List of defining ops and using ops
    List<Op*> s_def_ops;
    Hash_set<Op*> s_use_ops(hash_op_ptr, 16);

    // analyze all copy's source defs and their uses 
    for (List_iterator<El_ref> defs(*copy_src_defs); defs != 0; defs++ ) {
      El_ref s_def_ref = *defs;
      Op* s_def_op = s_def_ref.get_op();
      s_def_ops.add_tail(s_def_op);
      List<El_ref>* uses_of_s_def = rdef->get_du_chain(s_def_ref);
      if (// Note: 2a is relaxed -- Shail Aditya
	  // is_copy(s_def_ref.get_op()) ||
	  // Check 2b (also checks 3a implicitly)
	  (s_def_ref.get_ref_type() != EXP_DEST) || is_pseudo(s_def_ref.get_op()) 
	  // Note: 2c is relaxed (see below) -- Shail Aditya
	  // Unrelaxed. -KF 12/2004
	  || (uses_of_s_def->size() != 1) 
	  ) {
	ok_to_propagate = false;
	break;
      }

      // iterate over uses
      for (List_iterator<El_ref> uses(*uses_of_s_def); uses!=0; uses++) {
	El_ref s_use_ref = *uses;
	Op* s_use_op = s_use_ref.get_op();
	if (s_use_ref == copy_src_ref) continue;
	s_use_ops += s_use_op;

	// check for 3b. The use must not be a pseudo ref.
	if ((s_use_ref.get_ref_type() != EXP_SRC) || is_pseudo(s_use_ref.get_op())) {
	  ok_to_propagate = false;
	  break;
	}
	// relaxed 2c -- check for reaching defs of all other uses
	// Unrelaxed - this code no longer needed. -KF 12/2004
#if 0
	List<El_ref>* defs_of_s_use = rdef->get_ud_chain(s_use_ref);
	List_set<El_ref> defs_of_s_use_set(*defs_of_s_use);
	if (!copy_src_defs_set.is_subset(defs_of_s_use_set)) {
	  ok_to_propagate = false; 
	  break;
	}
#endif
      }
      if (!ok_to_propagate) break;
    }
    if (!ok_to_propagate) continue;

    // check for 4. Do a reverse walk over ops starting with the
    // copy op, crossing out each encountered s_def_op
    Region_ops_reverse_C0_order ops1(region, copy_op); 
    ops1++ ;  // Skip the copy op
    for (;(ops1 != 0) && !s_def_ops.is_empty(); ops1++) {
      Op* prev_op = *ops1;
      // If it is one of the defining ops, then remove from the list
      if (s_def_ops.is_member(prev_op)) {
	s_def_ops.remove(prev_op);
      }
      else {
	// Go over all destinations of this op to check for 4a
	for(Op_all_dests prev_op_dests(prev_op); prev_op_dests != 0; 
	    prev_op_dests++) {
	  Operand a_dest = *prev_op_dests;
	  // If it is also writing the same register as the 
	  // destination of copy, then we can't do propagation.
	  if (copy_dest == a_dest) {
	    ok_to_propagate = false;
	    break;
	  }
	}
	if (!ok_to_propagate) break;

	// Go over all sources of this op to check for 4b
	// added 05/13/99 Shail Aditya
	for(Op_all_inputs prev_op_srcs(prev_op); prev_op_srcs != 0; 
	    prev_op_srcs++) {
	  Operand a_src = *prev_op_srcs;
	  // If it is using the same register as the 
	  // destination of copy, then we can't do propagation.
	  if (copy_dest == a_src) {
	    ok_to_propagate = false;
	    break;
	  }
	}
	if (!ok_to_propagate) break;
      }
    }
    if (!ok_to_propagate) continue;

    // check for 3a. The list of def_ops must be empty. If not, 
    // we have "loop carried" reaching def.
    if (!s_def_ops.is_empty()) continue;

    // check for 5. Do a forward walk over ops starting with the
    // copy op, crossing out each encountered s_use_op
    // parkhc 01/28/2005
    // checking s_use_op doesn't make sense here.
    // so, removed s_use_ops thing.
    // should check for dest(r)'s use.
    Region_ops_C0_order ops2(region, copy_op); 
    ops2++ ;  // Skip the copy op
    // For 5.c
    bool copy_src_redefined = false;
    for (;ops2 != 0; ops2++) {
      Op* next_op = *ops2;
      // Go over all sources of this op to check for 5b
      // added 06/25/99 Shail Aditya
      // also check for 5c - parkhc 01/28/2005
      for(Op_all_inputs next_op_srcs(next_op); next_op_srcs != 0; 
          next_op_srcs++) {
        Operand a_src = *next_op_srcs;
        // If it is using the same register as the destination of
        // copy, then we need to check for its reaching defs
        if (copy_dest == a_src) {
          // check r's defs to be the same as copy_dest
          El_ref r_use_ref = next_op_srcs.get_ref();
          List<El_ref>* defs_of_r_use = rdef->get_ud_chain(r_use_ref);
          if (defs_of_r_use->size() != 1 || defs_of_r_use->head()!=copy_dest_ref) {
            ok_to_propagate = false;
            break;
          }
        }

        // check for 5c
        // a little bit conservative for HB - doesn't assume predicated ops
        if (copy_src_redefined && copy_src == a_src) {
          ok_to_propagate = false;
          break;
        }
      }
      if (!ok_to_propagate) break;

      // Go over all destinations of this op to check for 5a
      // added 06/25/99 Shail Aditya
      for(Op_all_dests next_op_dests(next_op); next_op_dests != 0; 
          next_op_dests++) {
        Operand a_dest = *next_op_dests;
        // If it is also writing the same register as the 
        // destination of copy, then we can't do propagation.
        if (copy_dest == a_dest) {
          ok_to_propagate = false;
          break;
        }

        // If redefinition of copy's src is found, mark it
        if (copy_src == a_dest)
          copy_src_redefined = true;
      }
      if (!ok_to_propagate) break;

    }
    if (!ok_to_propagate) continue;

    // We have checked all the conditions. Now do the propagation if
    // possible by iterating over all reaching defs of s and their
    // uses and replacing operand s by operand r.
    // We fix the def-use chains incrementally as follows:
    // 1. a. Old r uses are connected to all new r defs (old s defs)
    //    b. old r def is removed
    // 2. a. New r defs (old s defs) are connected to new r uses (old s
    //       uses) and vice versa.
    //    b. old s uses and defs are ignored
    assert(ok_to_propagate);
    for (List_iterator<El_ref> defs2(*copy_src_defs); defs2 != 0; defs2++ ) {
      El_ref s_def_ref = *defs2;
      List<El_ref>* uses_of_s_def =  rdef->get_du_chain(s_def_ref);
      // fix the s def chains
      Op* s_def_op = s_def_ref.get_op();
      Port_num dest_port = s_def_ref.get_port_num();
      // do chains 1a. migrate uses of r
      El_ref new_r_def_ref(s_def_op, &(s_def_op->dest(dest_port)), EXP_DEST);
      rdef->clone_du_for_new_def(new_r_def_ref, copy_dest_ref);
      // rebuild the s def op
      Operand new_dest_oprnd(copy_dest);
      s_def_op->set_dest(dest_port, new_dest_oprnd);
      // SAM 12-99, propagate liveout flag during backward copy prop
      if (copy_op->flag(EL_OPER_LIVEOUT_DEST))
	s_def_op->set_flag(EL_OPER_LIVEOUT_DEST);

      success = true;
      if (dbg(opti,3)) {
	cdbg << "\t\tPropagating: From " 
	     <<  copy_op->id()
	     << "  To DEF " << s_def_op->id() 
	     << endl;
      }
      // propagate to all its other uses
      for (List_iterator<El_ref> uses2(*uses_of_s_def); uses2!=0; uses2++) {
	El_ref s_use_ref = *uses2;
	if (s_use_ref == copy_src_ref) continue;
	// fix the s use chains
	Op* s_use_op = s_use_ref.get_op();
	Port_num src_port = s_use_ref.get_port_num();
	// do chains 2a. new r def to new r use
	El_ref new_r_use_ref(s_use_op, &(s_use_op->src(src_port)), EXP_SRC);
	rdef->add_du_ud_links(new_r_def_ref, new_r_use_ref);
	// rebuild the s use op
	Operand new_src_oprnd(copy_dest);
	s_use_op->set_src(src_port, new_src_oprnd);
	s_use_op->set_src_spatial_ref(src_port, copy_dest_ref.spatial_ref());

	success = true;
	if (dbg(opti, 3)) {
	  cdbg << "\t\tPropagating: From " 
	       <<  copy_op->id()
	       << "  To USE " << s_use_op->id() 
	       << endl;
	}
      }
    }

    // Make copy_op dest to be undefined, so that dead code will 
    // pick it up. 
    // do 1b. remove old r

    // parkhc 10/9/2005
    // This is not proper for movepp operations.
    // As copy_op is used in predicate analysis before it is eliminated
    // by dead code elimination opti.(in create_local_analysis_info.. func)
    // so, just manually delete the operation here.
    if (is_movepp(copy_op)) {
      C0_remove_op(copy_op);
      copy_op->parent()->remove_region(copy_op);
      delete copy_op;
    } else {
      Operand undefined;
      Port_num copy_dest_port = copy_dest_ref.get_port_num();
      rdef->remove_def(copy_dest_ref);
      copy_op->set_dest(copy_dest_port, undefined);
      // SAM 12-99
      // If set the dest to undef, clear the EL_OPER_LIVEOUT_DEST flag
      copy_op->reset_flag(EL_OPER_LIVEOUT_DEST);
    }
  }
}
