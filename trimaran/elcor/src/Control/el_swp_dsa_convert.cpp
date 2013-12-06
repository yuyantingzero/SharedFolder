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
//      File:           el_swp_dsa_convert.cpp
//      Authors:        Sadun Anik
//      Created:        July 1995
//      Description:    DSA conversion for software pipelinable loops
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "el_swp_dsa_convert.h"
#include "adjlist_graph.h"
#include "list_ofp.h"
#include "iterators.h"
#include "connect.h"
#include "opcode_properties.h"
#include "reaching_defs_solver.h"
#include "pred_interface.h"
#include "attributes.h"
#include "pqs.h"

using namespace std;

void el_swp_dsa_convert_loop(LoopBody* loop)
{
   Compound_region* r = loop->kernel();
   Pred_jar pj(r) ;
   
//   cout << "BEFORE\n" ;
//   cout << *r << "\n\n" ;
   typedef Pair<Operand, Pred_cookie> El_rd_pred_ref ;

   List<El_rd_pred_ref> rd_var_list ;
   List<El_ref> defs_to_rename ;
   List<El_ref> uses_to_rename ;
   List<El_ref> uses_to_check  ;
   List<Operand> do_not_rename ;

//
//  Scan the region for variables that are assigned to
//  1) If a use comes after the definition, remap the use
//  2) Remap the definitions
//  3) Map the the VR's to ROTATING file
//

//
// Go over all the remaps and evr's that are associated with them.
// Put them into the do not rename list   
//

   List<Operand> remap_operands ;
   Region_ops_C0_order opi;

   Op* last_branch = get_last_region_op_from_subregions(r) ;

   for (opi(r) ; opi != 0 ; opi++) {
      Op* cur_op = *opi ;
      if (cur_op == last_branch) break ;
      if (is_remap(cur_op)) {
	 remap_operands.add_tail(cur_op->src(SRC1)) ;
      }
   }
   
   for (opi(r) ; opi != 0 ; opi++) {
      Op* cur_op = *opi ;
      if (cur_op == last_branch) break ;
      for (Op_explicit_inputs oi(cur_op) ; oi != 0 ; oi++) {
	 Operand& tmp_input = (*oi) ;
	 if (do_not_rename.is_member(tmp_input)) continue ;
	 if (tmp_input.is_reg()) {
	    Operand new_evr = new VR_name(tmp_input) ;
	    if (remap_operands.is_member(new_evr)) {
	       do_not_rename.add_tail(tmp_input) ;
	    }
	 }
      }
      for (Op_explicit_dests od(cur_op) ; od != 0 ; od++) {
	 Operand& tmp_dest = (*od) ;
	 if (do_not_rename.is_member(tmp_dest)) continue ; 
	 if (tmp_dest.is_reg()) {
	    Operand new_evr = new VR_name(tmp_dest) ;
	    if (remap_operands.is_member(new_evr)) {
	       do_not_rename.add_tail(tmp_dest) ;
	    }
	 }
      }
   }

   // Now go over all ops of the region accumulating operands to
   // remap. The condition for remapping an operand is that (the
   // predicate of) its uses should be completely covered by (the sum
   // of predicates of) all the defs occurring in the loop (around the
   // back edge) since we do not insert dummy move operations
   // currently. We also check which uses are covered by previous defs
   // (remapped) vs. those that are upward exposed (not remapped) even
   // though they will be covered around the backedge. The latter
   // means that the use is a live-in.  -- Shail Aditya 08/23/2000

   for (opi(r) ; opi != 0 ; opi++) {
      Op* cur_op = *opi ;
      if (cur_op == last_branch) break ;

      // check uses
      for (Op_explicit_inputs oi(cur_op) ; oi != 0 ; oi++) {
	 Operand& tmp_input = (*oi) ;
	 // validate src operand
	 if (!tmp_input.is_reg()) continue;
	 Reg_file ftype = tmp_input.file_type() ;
         // Adding vector register files.  -KF 2/2007
	 if ((ftype != GPR) && (ftype != FPR) && (ftype != PR) &&
             (ftype != VIR) && (ftype != VFR) && (ftype != VMR)) 
	   do_not_rename.add_head(tmp_input) ;
	 // skip if this operand is already marked as DO NOT RENAME
	 if (do_not_rename.is_member(tmp_input)) continue ; 

	 bool found_def = false ;
	 El_ref tmpref = oi.get_ref() ;
	 Pred_cookie src_guard(pj.get_lub_guard(tmpref)) ; 
	 for (List_iterator<El_rd_pred_ref> li(rd_var_list) ; li != 0 ; li++) {
	    El_rd_pred_ref& cur_var = *li ;
	    // If this use is not covered at all it is neutral and not renamed
	    // If it is fully covered, it is neutral and potentially renamed
	    //    with all of its reaching defs
	    // If it is partially covered, it is not renamed, neither are its
	    //    partial defs.
	    if (cur_var.first == tmp_input) {
	       found_def = true;
	       if (src_guard.is_disjoint(cur_var.second)) {
		  // do nothing, this will not be renamed
		  uses_to_check.add_tail(tmpref);
	       }
	       else if (src_guard.is_subset(cur_var.second)) {
		  uses_to_rename.add_tail(tmpref) ;
	       }
	       else {
		  do_not_rename.add_head(tmp_input) ;
	       }
	       break ;
	    }
	 }
	 if (!found_def) uses_to_check.add_tail(tmpref);
      }

      // check dests
      for (Op_explicit_dests od(cur_op) ; od != 0 ; od++) {
	 Operand& tmp_dest = (*od) ;

	 if (!tmp_dest.is_reg()) {
	    do_not_rename.add_tail(tmp_dest) ;
	    continue ;
	 }
	    
	 Reg_file ftype = tmp_dest.file_type() ;
         /* RMR { adding vector register files as well } */
	 if ((ftype != GPR) && (ftype != FPR) && (ftype != PR) &&
             (ftype != VIR) && (ftype != VFR) && (ftype != VMR)) {
	    do_not_rename.add_tail(tmp_dest) ;
	    continue ;
	 }
	 
	 El_ref tmp_ref = od.get_ref() ;
	 Pred_cookie dest_guard(pj.get_glb_guard(tmp_ref)) ;

	 if (do_not_rename.is_member(tmp_dest)) continue ;
	 bool found_def = false ;
	 for (List_iterator<El_rd_pred_ref> li(rd_var_list) ; li != 0 ; li++) {
	    El_rd_pred_ref& cur_var = *li ;
	    if (cur_var.first == tmp_dest) {
	       found_def = true ;
	       cur_var.second.glb_sum(dest_guard) ;
	       break ;
	    }
	 }
	 if (!found_def) { // create a new definition reference
	    El_rd_pred_ref new_var(tmp_dest,dest_guard) ;
	    rd_var_list.add_tail(new_var) ;
	 }
	 defs_to_rename.add_tail(tmp_ref) ;
      }
   }

   // Now check for full coverage of uses -- Shail Aditya 08/23/2000
   for (List_iterator<El_ref> urefi(uses_to_check); urefi!=0; urefi++) {
     El_ref& tmpref = *urefi;
     Operand& tmp_src = tmpref.get_operand();
     Pred_cookie src_guard(pj.get_lub_guard(tmpref)) ; 
     bool found_def = false ;
     for (List_iterator<El_rd_pred_ref> li(rd_var_list) ; li != 0 ; li++) {
       El_rd_pred_ref& cur_var = *li ;
       if (cur_var.first == tmp_src) {
	 found_def = true ;
	 if (!src_guard.is_subset(cur_var.second)) {
	   do_not_rename.add_head(tmp_src);
         }
	 break ;
       }
     }
     if (!found_def) do_not_rename.add_head(tmp_src);
   }


// This is the list for remap insertion.    
   List<Operand> do_rename ;
   
//
// Do the branch for the loop.
// No definitions are renamed. The definitions that are used by the
// branch will have the uses incremented by one
// No other uses will be renamed
// For all the definitions, there needs to be a remap   
//

   List<Operand> branch_dests ;
   for (Op_explicit_dests dest_i(last_branch) ; dest_i != 0 ; dest_i++) {
      Operand& tmp_dest = *dest_i ;
      if (tmp_dest.is_reg()) {
	 // Consider only GPR FPR or PR files
	 Reg_file ftype = tmp_dest.file_type() ;
         /* RMR { adding vector register files as well } */
	 if ((ftype == GPR) || (ftype == FPR) || (ftype == PR) ||
             (ftype == VIR) || (ftype != VFR) || (ftype != VMR)) {
	    if (!do_rename.is_member(tmp_dest)) {
	       do_rename.add_tail(tmp_dest) ;
	    }
	    branch_dests.add_tail(tmp_dest) ;
	 }
      }
   }

   for (Op_explicit_inputs src_i(last_branch) ; src_i != 0 ; src_i++) {
      Operand& tmp_src = *src_i ;
      if (branch_dests.is_member(tmp_src)) {
	 tmp_src.incr_omega(1) ;
      }
   }

//
// Process the live outs. They are just like inputs under true 
// predicate.   
//
   Pred_cookie lo_guard ;
   
   Region_ops_C0_order sp_opi(r) ;
   assert(is_control_merge(*sp_opi)) ;
   sp_opi++ ;

   // Ignore movepps(that are inserted by clustering and so always guarded under true)
   // to get to the first op that is guarded under the staging predicate, Rajiv 08/03
   while (is_movepp(*sp_opi)) sp_opi++;

   // WARNING: The code below appears to be bogus for several reasons, assuming
   // its goal is to find the Pred_cookie corresponding to the staging predicate:
   // 1. Though the above loop skips movepps, there are potentially other ops
   //    (e.g. pred_clears) which are not guarded by the staging predicate.
   // 2. If the first real op is a U-type cmpp, then the lub_guard of the dest is true.
   // 3. If an op has no dests and we get to the else clause that iterates
   //    through inputs: the first input is the guard, which is always read.  thus
   //    its lub_guard is true.
   // Since the above bogusness only results in overly conservative DSA
   // conversion and I don't know how to fix it, I'm just going to write this
   // comment.  -KF 2/2006

   Op* first_real_op = *sp_opi ;
   assert(!is_switch(first_real_op) && !is_merge(first_real_op)) ;
   Op_explicit_dests operand_i(first_real_op) ;
   if (operand_i != 0) {
      Operand& sp_tmp_input = (*operand_i) ;
      El_ref tmp_reference(first_real_op, &sp_tmp_input, operand_i.ref_type()) ;
      lo_guard = pj.get_lub_guard(tmp_reference) ;
   }
   else {
      Op_explicit_inputs op_x_i(first_real_op) ;
      assert (op_x_i != 0) ; // Otherwise this op has no inputs or outputs
      Operand& sp_tmp_input = (*op_x_i) ;
      El_ref tmp_reference(first_real_op, &sp_tmp_input, op_x_i.ref_type()) ;
      lo_guard = pj.get_lub_guard(tmp_reference) ;
   }
      

   for (Op_live_sources livei(last_branch) ; livei != 0 ; livei++) {
      Operand& tmp_lo = *livei ;
      if (do_not_rename.is_member(tmp_lo)) continue ;
      for (List_iterator<El_rd_pred_ref> li(rd_var_list) ; li!=0 ; li++) {
	 El_rd_pred_ref& cur_var = *li ;
	 if (cur_var.first == tmp_lo) {
	    if (lo_guard.is_subset(cur_var.second)) {
	    }
	    else {
	       do_not_rename.add_head(tmp_lo) ;
	    }
	 }
      }
   }

// At this point the lists defs_to_rename and uses_to_rename
// contain the list of references to be renamed subject to the following
// + They do not apear in the do_not_rename list
   
   List_iterator<El_ref> ref_i ;
   for (ref_i(defs_to_rename) ; ref_i != 0 ; ref_i++){
      El_ref& cur_ref = *ref_i ;
      if (do_not_rename.is_member(cur_ref.get_operand())) {
	 continue ;
      }
      else {
	 Operand& cur_opernd = cur_ref.get_operand() ;
	 cur_opernd.incr_omega(-1) ;
	 if (!do_rename.is_member(cur_opernd)) {
	    do_rename.add_head(cur_opernd) ;
	 }
      }
   }
   for (ref_i(uses_to_rename) ; ref_i != 0 ; ref_i++){
      El_ref& cur_ref = *ref_i ;
      if (do_not_rename.is_member(cur_ref.get_operand())) {
	 continue ;
      }
      else {
	 cur_ref.get_operand().incr_omega(-1) ;
      }
   }
  
// Iterate over the do_rename list and insert remaps
   List<Operand> remap_args ;
   while (!do_rename.is_empty()) {
      Operand cur_oprnd = do_rename.pop() ;
      Operand new_evr = new VR_name(cur_oprnd) ;
      if (!remap_args.is_member(new_evr)) {
	 remap_args.add_tail(new_evr) ;
      }
   }

   Region_subregions_bidir rbi(r,true) ;
   assert((*rbi)->is_op()) ;
   Op* last_op = (Op*) (*rbi) ;
   rbi-- ;
   Op* prev_op = (Op*) (*rbi) ;
   while (!remap_args.is_empty()) {
      Operand cur_vrname = remap_args.pop() ;
      Op* new_remap = new Op(REMAP) ;
      new_remap->src(SRC1) = cur_vrname ;
      new_remap->dest(DEST1) = cur_vrname ;
      new_remap->set_parent(r) ;
      C0_connect_insert_between(prev_op,last_op,new_remap) ;
      prev_op = new_remap ;
      r->insert_before_region(new_remap,last_op) ;
   }
//   cout << "AFTER\n" ;
//   cout << *r << "\n\n" ;
//   cout << "**********************************************\n\n" ;
}

void el_swp_map_vr_to_rr_file(LoopBody* loop)
{
   Compound_region* r = loop->kernel();
   List<Operand> renamed_oprnd ;
   List_iterator<Operand> li ;

//
//  Scan the region for remaps and constuct a list of remapped VR names
//
   Region_ops_C0_order opi ;
   for (opi(r) ; opi != 0 ; opi++) {
      Op* cur_op = *opi ;
      if (is_remap(cur_op)) {
	 assert(!renamed_oprnd.is_member(cur_op->src(SRC1))) ;
	 renamed_oprnd.add_tail(cur_op->src(SRC1)) ;
      }
   }
//
//  Bind all variables whose names match remapped names to RR
//
   for (opi(r) ; opi != 0 ; opi++) {
      Op* cur_op = *opi ;
      for (Op_explicit_inputs src_i(cur_op) ; src_i != 0 ; src_i++) {
	 Operand& tmp_src = *src_i ;
	 if (tmp_src.is_reg()) {
	    int reg_num = tmp_src.vr_num() ;
	    for (li(renamed_oprnd) ; li != 0 ; li++) {
	       Operand& vrname = *li ;
	       if (vrname.vr_num() == reg_num) {
		  tmp_src.bind_file(tmp_src.file_type(),ROTATING) ;
		  break ;
	       }
	    }
	 }
      }
      for (Op_explicit_dests dest_i(cur_op) ; dest_i != 0 ; dest_i++) {
	 Operand& tmp_dest = *dest_i ;
	 if (tmp_dest.is_reg()) {
	    int reg_num = tmp_dest.vr_num() ;
	    for (li(renamed_oprnd) ; li != 0 ; li++) {
	       Operand& vrname = *li ;
	       if (vrname.vr_num() == reg_num) {
		  tmp_dest.bind_file(tmp_dest.file_type(),ROTATING) ;
		  break ;
	       }
	    }
	 }
      }
   }
}

void el_swp_strip_remaps(LoopBody* loop)
{
   Compound_region* r = loop->kernel();
   for (Region_ops_C0_order opi(r) ; opi != 0 ;) {
      Op* cur_op = *opi ;
      opi++ ;
      if (is_remap(cur_op)) {
	 cur_op->parent()->remove_region(cur_op) ;
	 C0_remove_op(cur_op) ;
	 delete cur_op ;
      }
   }
}
