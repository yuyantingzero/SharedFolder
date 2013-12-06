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
//      File:           move_opti.cpp
//      Author:         Kevin Fan
//      Created:        May 2003
//      Description:    Optimize inter-cluster moves
//
/////////////////////////////////////////////////////////////////////////////

#include "cluster.h"

// procedure-level move optimization
void Cluster_mgr::do_proc_move_opti()
{
   bool push_flag;
   Dlist<Region*> rstack;
   Region *rgn;
   int num_optis = 0;
   
   assert(m_proc != NULL);

   rstack.push(m_proc);
   while (!rstack.is_empty()) {
      rgn = rstack.pop();
      push_flag = true;
      if (rgn->is_hb() || rgn->is_bb()) {
	 num_optis += do_block_move_opti((Compound_region*)rgn);
	 push_flag = false;
      }
      if (push_flag == true) {
	 for (Region_subregions subri(rgn); subri != 0; subri++) {
	    if ((*subri)->is_compound())
	       rstack.push(*subri);
	 }
      }
   }

   if (dbg(clst, 1))
      cdbg << num_optis << " optimizations done." << endl;
}

// block-level move optimization
int Cluster_mgr::do_block_move_opti(Compound_region* block)
{
   if (!block->is_hb() && !block->is_bb())
      El_punt("do_block_move_opti: region %d not bb or hb", block->id());

   int num_optis = 0, opti1, opti2, opti3;

   create_local_analysis_info_for_all_hbs_bbs(block);

   El_do_reaching_defs(block, ANALYZE_ALLREG);
   Reaching_defs_info* rdi = get_reaching_defs_info(block);

   opti1 = do_block_move_opti_1(block, rdi);

   // redo reaching defs if necessary
   if (opti1 > 0) {
      remove_reaching_defs_info(block);
      El_do_reaching_defs(block, ANALYZE_ALLREG);
      rdi = get_reaching_defs_info(block);
      num_optis += opti1;
   }

   opti2 = do_block_move_opti_2(block, rdi);

   // redo reaching defs if necessary
   if (opti2 > 0) {
      remove_reaching_defs_info(block);
      El_do_reaching_defs(block, ANALYZE_ALLREG);
      rdi = get_reaching_defs_info(block);
      num_optis += opti2;
   }

   opti3 = do_block_move_opti_3(block, rdi);

   remove_reaching_defs_info(block);
   num_optis += opti3;

   el_opti_eliminate_dead_code(block, ANALYZE_ALLREG, true, true);

   if (dbg(clst, 2))
      check_moves(block);

   return num_optis;
}

// just print out a debug message if we find any moves that
// look like they could have been optimized out
void Cluster_mgr::check_moves(Compound_region* block) 
{
   Operand *src1, *src2, *dest1, *dest2;
   for (Region_ops_C0_order opi1(block); opi1 != 0; opi1++) {
      Op* op1 = *opi1;

      if (!op1->flag(EL_OPER_ICMOVE)) continue;

      src1  = &op1->src(SRC1);
      dest1 = &op1->dest(DEST1);

      // find an interesting matching icmove
      for (Region_ops_C0_order opi2(block, op1); opi2 != 0; opi2++) {
	 Op* op2 = *opi2;

	 // skip the first one
	 if (op1 == op2) continue;

	 if (!op2->flag(EL_OPER_ICMOVE)) continue;

	 src2  = &op2->src(SRC1);
	 dest2 = &op2->dest(DEST1);

	 if (*src1 == *src2 || *dest1 == *src2 || 
	     ((*dest1 == *dest2) && dest1->file_type() == PR))
	    if (dbg(clst, 2))
	       cdbg << "not optimized: " << op1->id() << " " << op2->id() << endl;
      }
   }	 
}

// do_block_move_opti_1:
// solves problem of multiple intercluster moves for multiple uses
// in the same cluster of the same def.

int Cluster_mgr::do_block_move_opti_1(Compound_region* block, Reaching_defs_info* rdi)
{
   Operand *src1, *src2, *dest1, *dest2;
   //Hash_set<Op*> skip_ops(hash_op_ptr, HASH_SIZE_0);
   int num_optis = 0;

   for (Region_ops_C0_order opi1(block); opi1 != 0; opi1++) {
      Op* op1 = *opi1;

      if (!op1->flag(EL_OPER_ICMOVE)) continue;

      src1  = &op1->src(SRC1);
      dest1 = &op1->dest(DEST1);

      // skip macro destinations and literal sources
      if (dest1->is_macro_reg()) continue;
      if (src1->is_predicate()) continue;

      // skip ops we've looked at before
      //if (skip_ops.is_member(op1)) continue;

      if (!operand_map.is_bound(dest1->vr_num())) continue;

      // find an interesting matching icmove
      for (Region_ops_C0_order opi2(block, op1); opi2 != 0; opi2++) {
	 Op* op2 = *opi2;

	 // skip the first one
	 if (op1 == op2) continue;

	 if (!op2->flag(EL_OPER_ICMOVE)) continue;

	 src2  = &op2->src(SRC1);
	 dest2 = &op2->dest(DEST1);

	 // no macro destinations
	 if (dest2->is_macro_reg()) continue;

	 // check if they have the same source
	 if (*src1 != *src2) continue;

	 if (!operand_map.is_bound(dest2->vr_num())) continue;

	 // check if they write to same cluster
	 if (operand_map.value(dest1->vr_num()) != operand_map.value(dest2->vr_num()))
	    continue;

	 if (dbg(clst, 2))
	    cdbg << "opti1 found: op "<<op1->id()<<" and op "<<op2->id()<<endl;

	 // check reaching defs of both regs are the same
	 El_ref use_ref1(op1, src1, EXP_SRC);
	 El_ref use_ref2(op2, src2, EXP_SRC);
	 List<El_ref>* defs1 = rdi->get_ud_chain(use_ref1);
	 List<El_ref>* defs2 = rdi->get_ud_chain(use_ref2);
	 if (!same_contents(*defs1, *defs2)) continue;

	 El_ref def_ref1(op1, dest1, EXP_DEST);
	 El_ref def_ref2(op2, dest2, EXP_DEST);

	 // get uses of second temp
	 List<El_ref>* uses2 = rdi->get_du_chain(def_ref2);
	 if (uses2->size() < 1) {
	    if (dbg(clst, 2))
	       cdbg << "no uses found(2)" << endl;
	    continue;
	 }

	 List<El_ref> uses2_delete;

	 for (List_iterator<El_ref> li(*uses2); li != 0; li++) {
	    El_ref use_ref2 = *li;
	    El_ref_type rt2 = use_ref2.get_ref_type();
	    if (rt2 != EXP_SRC && rt2 != PRED_SRC) continue;
	    Op* use_op = use_ref2.get_op();
	    Port_num pnum = use_ref2.get_port_num();

	    // check that 1st temp is not redef'd
	    if (! El_no_defs_between(block, op1, use_op, *dest1))
	       continue;

	    // check that first predicate dominates second
	    if (! El_predicate_dominates(block, def_ref1, use_ref2))
	       continue;

	    // check 2nd temp is only defnd by the 2nd move
	    List<El_ref>* defs_uses2 = rdi->get_ud_chain(use_ref2);
	    if (defs_uses2->size() != 1) continue;
	    El_ref def_use_ref2 = defs_uses2->head();
	    if (def_use_ref2.get_op() != op2) continue;

	    // OK
	    use_op->set_src(pnum, *dest1);
	    // skip this move from now on since we've effectively deleted it
	    //skip_ops += op2;
	    defs_uses2->remove(def_use_ref2);
	    uses2_delete.add_tail(use_ref2);
	    if (dbg(clst, 2))
	       cdbg << "move opti done"<<endl;
	    num_optis++;
	 }

	 for (List_iterator<El_ref> li2(uses2_delete); li2 != 0; li2++)
	    uses2->remove(*li2);
      }
   }
   return num_optis;
}

// do_block_move_opti_2:
// solves problem of def followed by use of a register, where both ops
// are on same cluster but register is homed on another cluster.

int Cluster_mgr::do_block_move_opti_2(Compound_region* block, Reaching_defs_info* rdi)
{
   Operand *src1, *src2, *dest1, *dest2;
   int num_optis = 0;

   for (Region_ops_C0_order opi1(block); opi1 != 0; opi1++) {
      Op* op1 = *opi1;

      if (!op1->flag(EL_OPER_ICMOVE)) continue;

      src1  = &op1->src(SRC1);
      dest1 = &op1->dest(DEST1);

      // skip moves with macro sources and pred_lit sources (ie O-type stuff)
      if (src1->is_macro_reg()) continue;
      if (src1->is_predicate()) continue;

      if (!operand_map.is_bound(src1->vr_num())) {
	 if (dbg(clst, 2))
	    cdbg << "opti2: src1 wasn't bound......... op1:" <<op1->id()<< endl;
	 continue;
      }

      for (Region_ops_C0_order opi2(block, op1); opi2 != 0; opi2++) {
	 Op* op2 = *opi2;
	 
	 // skip the first one
	 if (op1 == op2) continue;

	 if (!op2->flag(EL_OPER_ICMOVE)) continue;
	 
	 src2  = &op2->src(SRC1);
	 dest2 = &op2->dest(DEST1);

	 // skip macro destinations
	 if (dest2->is_macro_reg()) continue;

	 // check dest(1) == src(2)
	 if (*dest1 != *src2) continue;

	 if (!operand_map.is_bound(dest2->vr_num())) {
	    if (dbg(clst, 2))
	       cdbg << "opti2: dest2 wasn't bound........ op1:"<<op1->id()<<" op2:"<<op2->id()<<endl;
	    continue;
	 }

	 // check source cluster of 1 == dest cluster of 2
	 if (operand_map.value(src1->vr_num()) != operand_map.value(dest2->vr_num()))
	    continue;

	 if (dbg(clst, 2))
	    cdbg << "opti2 found: op "<<op1->id()<<" and op "<<op2->id()<<endl;

	 // check that the reg is only def'd by the move
	 El_ref reg_ref(op2, src2, EXP_SRC);
	 List<El_ref>* defs = rdi->get_ud_chain(reg_ref);
	 if (defs->size() != 1) continue;
	 El_ref reg_def = defs->head();
	 if (reg_def.get_op() != op1)
	    continue;

	 // first predicate dominates second
	 El_ref def_ref1(op1, dest1, EXP_DEST);
	 El_ref def_ref2(op2, dest2, EXP_DEST);

	 // get the user of the 2nd temp
	 List<El_ref>* uses = rdi->get_du_chain(def_ref2);
	 if (uses->size() < 1) {
	    if (dbg(clst, 2))
	       cdbg << "no uses found" << endl;
	    continue;
	 }

	 for (List_iterator<El_ref> li(*uses); li != 0; li++) {
	    El_ref use_ref = *li;
	    El_ref_type rt = use_ref.get_ref_type();
	    if (rt != EXP_SRC && rt != PRED_SRC) continue;
	    Op* use_op = use_ref.get_op();
	    Port_num pnum = use_ref.get_port_num();
	 
	    if (! El_predicate_dominates(block, def_ref1, use_ref)) {
	       if (dbg(clst, 2))
		  cdbg << "failed due to pred not dominant" << endl;
	       continue;
	    }

	    // check 2nd temp is only defnd by the 2nd move
	    List<El_ref>* defs_uses2 = rdi->get_ud_chain(use_ref);
	    if (defs_uses2->size() != 1) continue;
	    El_ref def_of_use_ref = defs_uses2->head();
	    if (def_of_use_ref.get_op() != op2) continue;

	    // no redefns of 1st temp
	    if (!El_no_defs_between(block, op1, use_op, *src1)) 
	       continue;

	    // OK
	    use_op->set_src(pnum, *src1);
	    if (dbg(clst, 2))
	       cdbg << "move opti done"<<endl;
	    num_optis++;
	 }
      }
   }
   return num_optis;
}

// do_block_move_opti_3:
// or-type predicate move optimizations

// this may not be capturing all opti opportunities in grid-style architectures,
// because in those machines the or-type moves may have multiple steps, and the
// intermediate steps look different than what this function expects. -KF 10/2004
int Cluster_mgr::do_block_move_opti_3(Compound_region* block, Reaching_defs_info* rdi)
{
   Op* pclear_op;
   Operand *dest1, *dest2;
   int clstr1;
   bool break_flag, continue_flag;
   Hash_set<Op*> to_nuke(hash_op_ptr, HASH_SIZE_0);
   int num_optis = 0;

   for (Region_ops_C0_order opi1(block); opi1 != 0; opi1++) {
      Op* op1 = *opi1;
      
      if (!is_cmpp(op1)) continue;

      clstr1 = op_map.value(op1);

      for (Op_explicit_dests desti1(op1); desti1 != 0; desti1++) {
	 break_flag = false;

	 Port_num pnum = desti1.get_port_num();
	 if (!is_O_pred(op1, pnum)) continue;
	 
	 dest1 = &(*desti1);

         if (dest1->is_undefined()) continue;

	 // see if there is a move
	 El_ref def_ref1(op1, dest1, EXP_DEST);
	 List<El_ref>* uses1 = rdi->get_du_chain(def_ref1);
	 if (uses1->size() != 1) continue; // be more general?
	 El_ref use_ref1 = uses1->head();
	 Op* use_op1 = use_ref1.get_op();
	 if (!use_op1->flag(EL_OPER_ICMOVE)) continue;
         // these next two checks may be too restrictive in grid-style
         // architectures -KF 10/2004
	 if (use_ref1.get_ref_type() != PRED_SRC) continue;
	 if (!use_op1->src(SRC1).is_predicate_true()) continue;
	 
	 Operand& pred_target = use_op1->dest(DEST1);

	 // now go find another cmpp
	 for (Region_ops_C0_order opi2(block, op1); opi2 != 0; opi2++) {
	    Op* op2 = *opi2;

	    // skip first
	    if (op1 == op2) continue;

	    // break if we've already found a cmpp for the outer op.
	    if (break_flag) break;

	    if (!is_cmpp(op2)) continue;

	    // same cluster?
	    if (op_map.value(op2) != clstr1) continue;

	    for (Op_explicit_dests desti2(op2); desti2 != 0; desti2++) {
	       Port_num pnum = desti2.get_port_num();
	       if (!is_O_pred(op2, pnum)) continue;

	       dest2 = &(*desti2);

               if (dest2->is_undefined()) continue;

	       // see if there is a move
	       El_ref def_ref2(op2, dest2, EXP_DEST);
	       List<El_ref>* uses2 = rdi->get_du_chain(def_ref2);

	       // check only one use of 2nd temp
	       if (uses2->size() != 1) continue;

	       El_ref use_ref2 = uses2->head();
	       Op* use_op2 = use_ref2.get_op();
	       if (!use_op2->flag(EL_OPER_ICMOVE)) continue;
	       if (use_ref2.get_ref_type() != PRED_SRC) continue;
	       if (!use_op2->src(SRC1).is_predicate_true()) continue;

	       // make sure they target the same pred register
	       if (use_op2->dest(DEST1) != pred_target) continue;

	       if (dbg(clst, 2))
		  cdbg << "opti3 found: op "<<op1->id()<<" and op "<<op2->id()<<endl;

	       // no redefns of 1st temp
	       if (!El_no_defs_between(block, op1, use_op2, *dest1))
		  continue;
	       
	       // second temp is only defined by the pclear and the cmpp
	       List<El_ref>* defs2 = rdi->get_ud_chain(use_ref2);
	       if (defs2->size() != 2) continue;

	       continue_flag = false;
	       pclear_op = NULL;

	       for (List_iterator<El_ref> li(*defs2); li!=0; li++) {
		  Op* check_op = (*li).get_op();
		  if (is_predicate_clear(check_op))
		     pclear_op = check_op;
		  else if (*li != def_ref2) {
		     continue_flag = true;
		     break;
		  }
	       }
	       
	       if (continue_flag) continue;
	       if (pclear_op == NULL) {
		  if (dbg(clst, 2))
		     cdbg << "failed due to no pclear found" << endl;
		  continue;
	       }

	       // OK
	       // change 2nd temp to 1st temp
	       op2->set_dest(pnum, *dest1);
	       use_op2->set_src(PRED1, *dest1);

	       // remove first move & 2nd pred clear
	       to_nuke += use_op1;
	       to_nuke += pclear_op;

	       if (dbg(clst, 2))
		  cdbg << "move opti done" << endl;
	       num_optis++;
	       break_flag = true;
	       break;
	    }
	 }
      }
   }

   update_graph_attributes_for_dead_ops(block, to_nuke);

   for (Hash_set_iterator<Op*> hi(to_nuke); hi != 0; hi++) {
      Op* op = *hi;
      op_map.unbind(op);
      C0_remove_op(op);
      block->remove_region(op);
      delete op;
   }

   return num_optis;
}

// returns true if two lists contain the same El_refs. assumes no duplicates
bool Cluster_mgr::same_contents(List<El_ref>& list1, List<El_ref>& list2)
{
   if (list1.size() != list2.size())
      return false;

   for (List_iterator<El_ref> li(list1); li != 0; li++) {
      if (! list2.is_member(*li))
	 return false;
   }
   
   return true;
}
