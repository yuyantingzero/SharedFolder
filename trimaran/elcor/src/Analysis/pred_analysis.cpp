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
//      File:           pred_analysis.cpp
//      Authors:        Richard Johnson, Sadun Anik, Sumedh Sathaye
//      Created:        January 1995
//      Description:    Predicate-cognizant data flow analysis for hyperblocks
//
//      Notes:
//      June 5, 1996    Added the Reaching Defs gen/kill computation interface
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include "connect.h"
#include "region.h"
#include "iterators.h"
#include "tuples.h"
#include "attributes.h"
#include "region_attributes.h"
#include "opcode_properties.h"
#include "string_class.h"
#include "hash_functions.h"
#include "pqs.h"
#include "pqs-1bit-triv.h"
#include "pqs-1sym-triv.h"
#include "pqs-1dnf.h"
#include "pqs-1sym.h"

#include "pred_interface.h"
#include "pred_analysis.h"

#include "dfa.h"
#include "dbg.h"
#include "el_anal_init.h"
#include "el_stats_init.h"
#include "el_clock.h"
#include "ir_writer.h"


using namespace std;

#define PRIVATE static
#define PUBLIC

// compute local liveness gen/kill for a BB/SB/HB region.  For SB/HB regions,
// we compute local liveness gen/kill between a specified exit and the region
// entry.
//
// Notes:
// - this routine now computes gen/kill vectors for
//   up/down-exposed def/use analysis.
// - don't compute up-exposed mem_vr defs or down-exposed mem_vr uses.
static Vector<Pred_cookie>* pq_up_use_gen_ptr = NULL ;
static Vector<Pred_cookie>* pq_up_use_kill_ptr = NULL ;

static Vector<Pred_cookie>* pq_up_def_gen_ptr = NULL ;
static Vector<Pred_cookie>* pq_up_def_kill_ptr = NULL ;

static Vector<Pred_cookie>* pq_dn_use_gen_ptr = NULL;
static Vector<Pred_cookie>* pq_dn_use_kill_ptr = NULL ;

static Vector<Pred_cookie>* pq_dn_def_gen_ptr = NULL ;
static Vector<Pred_cookie>* pq_dn_def_kill_ptr = NULL ;


PUBLIC void
free_liveness_gen_kill_mem()
{
  if(pq_up_use_gen_ptr) { delete pq_up_use_gen_ptr; pq_up_use_gen_ptr  = NULL; }
  if(pq_up_use_kill_ptr) { delete pq_up_use_kill_ptr; pq_up_use_kill_ptr = NULL; }
  if(pq_up_def_gen_ptr) { delete pq_up_def_gen_ptr; pq_up_def_gen_ptr = NULL; }
  if(pq_up_def_kill_ptr) { delete pq_up_def_kill_ptr; pq_up_def_kill_ptr = NULL; }
  if(pq_dn_use_gen_ptr) { delete pq_dn_use_gen_ptr; pq_dn_use_gen_ptr = NULL; }
  if(pq_dn_use_kill_ptr) { delete pq_dn_use_kill_ptr; pq_dn_use_kill_ptr = NULL; }
  if(pq_dn_def_gen_ptr) { delete pq_dn_def_gen_ptr; pq_dn_def_gen_ptr = NULL; }
  if(pq_dn_def_kill_ptr) { delete pq_dn_def_kill_ptr; pq_dn_def_kill_ptr = NULL; }
}

void set_bits_from_operand_list(Liveness_solver* dfa,
                                List<Operand>& opnd_list, Bitvector& bv)
{
  for (List_iterator<Operand> li(opnd_list); li!=0; li++) {
    Operand& opnd = *li;
    if (dfa->operand_map.is_bound(opnd)) {
      int idx = dfa->operand_map.value(opnd);
      bv.set_bit(idx);
    } else {
      // could warn here, but it isn't really a problem
    }
  }
}

PUBLIC void
liveness_gen_kill (Compound_region* blk, Op* exit_op,
		   Liveness_solver* global_dfa, int live_idx)
{
   if (dbg(dfa))
      cdbg << "liveness_gen_kill, block " << blk->id() << endl;

   if (dbg(dfa,1)) cdbg << "." << flush;

   PQS* pqs = get_pqs(blk);
   VR_map* vr_map = get_local_vr_map (blk);

   if(pqs == NULL || vr_map == NULL) {
     create_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(blk));
     pqs = get_pqs(blk);
     vr_map = get_local_vr_map (blk);
   }
   Pred_jar pj(blk);

   assert(pqs != NULL && vr_map != NULL);

   int& vr_count = vr_map->vr_count;
   Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;
   Vector<Operand>& index_to_vr = vr_map->index_to_vr;

   // This is a hack to get around the inability to analyze modulo-scheduled
   // loops.  Dataflow information is computed before modulo scheduling and
   // saved as an attribute.  Then future dataflow queries get information from
   // this attribute. -KF 2/2005
   if (blk->parent()->is_loopbody() && blk->flag(EL_REGION_ROT_REG_ALLOCATED)) {

     Msched_attr* ms_attr = get_msched_attr(blk->parent());

     if (ms_attr == NULL) {
       El_punt("Cannot perform dataflow analysis on modulo scheduled loops.");
     }
     
     set_bits_from_operand_list(global_dfa, ms_attr->liveness_gen,
                                global_dfa->live_info[live_idx].liveness_gen);
     set_bits_from_operand_list(global_dfa, ms_attr->liveness_kill,
                                global_dfa->live_info[live_idx].liveness_kill);
     set_bits_from_operand_list(global_dfa, ms_attr->down_exposed_use_gen,
                                global_dfa->live_info[live_idx].down_exposed_use_gen);
     set_bits_from_operand_list(global_dfa, ms_attr->down_exposed_use_kill,
                                global_dfa->live_info[live_idx].down_exposed_use_kill);
     set_bits_from_operand_list(global_dfa, ms_attr->up_exposed_def_gen,
                                global_dfa->live_info[live_idx].up_exposed_def_gen);
     set_bits_from_operand_list(global_dfa, ms_attr->up_exposed_def_kill,
                                global_dfa->live_info[live_idx].up_exposed_def_kill);
     set_bits_from_operand_list(global_dfa, ms_attr->down_exposed_def_gen,
                                global_dfa->live_info[live_idx].down_exposed_def_gen);
     set_bits_from_operand_list(global_dfa, ms_attr->down_exposed_def_kill,
                                global_dfa->live_info[live_idx].down_exposed_def_kill);
     return;
   }

   Bitvector interesting(vr_count) ;
   for (int vic = 0 ; vic < vr_count ; vic++) {
      if (global_dfa->is_interesting(index_to_vr[vic])) interesting.set_bit(vic) ;
   }

   Pred_cookie cond_cq = pj.get_lub_path_guard(get_first_region_op_from_subregions(blk), exit_op) ;

   // create vectors of pred expressions for gen/kill

   Pred_cookie fpq = Pred_jar::get_false() ;
   Pred_cookie tpq = Pred_jar::get_true() ;

   if (!pq_up_use_gen_ptr) {
      pq_up_use_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
      pq_up_use_kill_ptr = new Vector<Pred_cookie>(vr_count, tpq) ;
      pq_up_def_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
      pq_up_def_kill_ptr = new Vector<Pred_cookie>(vr_count, tpq) ;
      pq_dn_use_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
      pq_dn_use_kill_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
      pq_dn_def_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
      pq_dn_def_kill_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
   }
   else {
      if ((int)pq_up_use_gen_ptr->size() < vr_count) {
         delete pq_up_use_gen_ptr ;
         delete pq_up_use_kill_ptr ;
         delete pq_up_def_gen_ptr ;
         delete pq_up_def_kill_ptr ;
         delete pq_dn_use_gen_ptr ;
         delete pq_dn_use_kill_ptr ;
         delete pq_dn_def_gen_ptr ;
         delete pq_dn_def_kill_ptr ;
         pq_up_use_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
	 pq_up_use_kill_ptr = new Vector<Pred_cookie>(vr_count, tpq) ;
	 pq_up_def_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
	 pq_up_def_kill_ptr = new Vector<Pred_cookie>(vr_count, tpq) ;
	 pq_dn_use_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
	 pq_dn_use_kill_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
	 pq_dn_def_gen_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
	 pq_dn_def_kill_ptr = new Vector<Pred_cookie>(vr_count, fpq) ;
      }
      else {
	 int j ;
         for (j=0 ; j < vr_count ; j++) {
	    (*pq_up_use_gen_ptr)[j].reset_to_false() ;
	    (*pq_up_use_kill_ptr)[j].reset_to_true() ;
	    (*pq_up_def_gen_ptr)[j].reset_to_false() ;
	    (*pq_up_def_kill_ptr)[j].reset_to_true() ;
	    (*pq_dn_use_gen_ptr)[j].reset_to_false() ;
	    (*pq_dn_use_kill_ptr)[j].reset_to_false() ;
	    (*pq_dn_def_gen_ptr)[j].reset_to_false() ;
	    (*pq_dn_def_kill_ptr)[j].reset_to_false() ;
	 }
      }
   }
   
   Vector<Pred_cookie>& pq_up_use_gen = *pq_up_use_gen_ptr ;
   Vector<Pred_cookie>& pq_up_use_kill = *pq_up_use_kill_ptr;

   Vector<Pred_cookie>& pq_up_def_gen = *pq_up_def_gen_ptr ;
   Vector<Pred_cookie>& pq_up_def_kill = *pq_up_def_kill_ptr ;


   Operand_iter* dest_oper_ptr = NULL ;
   Operand_iter* src_oper_ptr = NULL ;
   if (global_dfa->filter_mask & ANALYZE_MEMVR) {
      dest_oper_ptr = new Op_complete_dests() ;
      src_oper_ptr = new Op_complete_inputs() ;
   }
   else {
      dest_oper_ptr = new Op_all_dests() ;
      src_oper_ptr = new Op_all_inputs() ;
   }
   

   Operand_iter& dest_oper = *dest_oper_ptr ;
   Operand_iter& src_oper = *src_oper_ptr ;   
   
   // traverse ops in hyperblock in reverse linear order from exit_op to entry
   for (Region_ops_reverse_C0_order opi(blk, exit_op); opi != 0; opi++) {
      assert ( (*opi)->is_op() );
      Op* op = (Op*) (*opi);
    
      if (is_remap(op)) {
	 // get VR_name
	 Operand& vr = op->src(SRC1);
	 assert (vr.is_vr_name());

	 // remap uses vr_min .. vr_{max-1}, defs vr_min .. vr_max
	 int idx = vr.min_omega();
	 int max_idx = vr.max_omega();
	 int i ;
	 for (i=idx; i<=max_idx; i++) { // define idx..max_idx
	    Operand def = new Reg(vr);
	    def.incr_omega(i);

	    int ii = vr_to_index.value(def);

	    pq_up_use_gen[ii].lub_diff(cond_cq) ;
	    pq_up_use_kill[ii].lub_diff(cond_cq);
	    pq_up_def_gen[ii].lub_sum(cond_cq) ;

	 }
	 for (i=idx; i<max_idx; i++) { // use idx..max_idx-1
	    Operand use = new Reg(vr);
	    use.incr_omega(i);

	    int ii = vr_to_index.value(use);

	    pq_up_use_gen[ii].lub_sum(cond_cq) ;
	    pq_up_use_kill[ii].lub_sum(cond_cq) ;
	 }

	 // take care of the high end of the evr range (high end for backward)
	 Operand k_ll = new Reg(vr);
	 k_ll.incr_omega(max_idx);
      
	 int kii = vr_to_index.value(k_ll);
      
	 pq_up_use_gen[kii] = fpq ;
	 pq_up_use_kill[kii] = fpq ;      
	 pq_up_def_gen[kii] = fpq ;
	 pq_up_def_kill[kii] = fpq;      
	 pq_up_use_kill[kii].lub_sum(cond_cq);   // live under exit predicate
	 pq_up_def_kill[kii].lub_sum(cond_cq);   // live under exit predicate
      }
      else if (is_swp_branch(op)) {
	 // Special case for SWP branch so that src and dest guards
	 // are the staging predicate. This code needs to be fixed once
	 // the PQS interface takes care of this internally

	 // process defs
	 for (dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
	    Operand oper = (*dest_oper) ;
	
	    if (global_dfa->is_interesting(oper)) {
	       int ii = vr_to_index.value(oper);

               pq_up_use_gen[ii].lub_diff(cond_cq) ;
	       pq_up_use_kill[ii].lub_diff(cond_cq) ;
	       pq_up_def_gen[ii].lub_sum(cond_cq) ;
	    }
	 }
	 // process uses
	 for (src_oper(op) ; src_oper != 0 ; src_oper++) {
	    Operand& oper = (*src_oper) ;

	    if (global_dfa->is_interesting(oper)) {
	       int ii = vr_to_index.value(oper);

	       pq_up_use_gen[ii].lub_sum(cond_cq) ;
	       pq_up_use_kill[ii].lub_sum(cond_cq) ;

	    }
	 }
      }
      else {
	 // for each op, process defs, then uses

	 //process defs
	 for (dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
	    Operand oper = (*dest_oper) ;
	
	    if (global_dfa->is_interesting(oper)) {
               int ii = vr_to_index.value(oper);
               El_ref tmp1 = dest_oper.get_ref() ;
               Pred_cookie pq1 = pj.get_glb_guard(tmp1) ;
               Pred_cookie pq2 = pj.get_lub_guard(tmp1) ;

               pq_up_use_gen[ii].lub_diff(pq1) ;
               pq_up_use_kill[ii].lub_diff(pq1) ;
               pq_up_def_gen[ii].lub_sum(pq2) ;
            }
	 }
	 // process uses
	 for (src_oper(op) ; src_oper != 0 ; src_oper++) {
	    Operand& oper = (*src_oper) ;

	    if (global_dfa->is_interesting(oper)) {
               int ii = vr_to_index.value(oper);
               El_ref tmp1 = src_oper.get_ref() ;
               Pred_cookie pq1 = pj.get_lub_guard(tmp1) ;

               pq_up_use_gen[ii].lub_sum(pq1) ;
               pq_up_use_kill[ii].lub_sum(pq1) ;

	    }
	 }
      }
   }


   Vector<Pred_cookie>& pq_dn_use_gen = *pq_dn_use_gen_ptr ;
   Vector<Pred_cookie>& pq_dn_use_kill = *pq_dn_use_kill_ptr ;

   Vector<Pred_cookie>& pq_dn_def_gen = *pq_dn_def_gen_ptr ;
   Vector<Pred_cookie>& pq_dn_def_kill = *pq_dn_def_kill_ptr ;

   //
   // traverse ops in hyperblock in linear order from entry to exit_op
   //
   for (Region_ops_C0_order opi2(blk); opi2 != 0 ; opi2++) {
      assert ( (*opi2)->is_op() );
      Op* op = (Op*) (*opi2);
    
      if (is_remap(op)) {
	 // get VR_name
	 Operand& vr = op->src(SRC1);
	 assert (vr.is_vr_name());

	 // remap uses vr_min .. vr_{max-1}, defs vr_min .. vr_max

	 int idx = vr.min_omega();
	 int max_idx = vr.max_omega();
	 int i;
	 for (i=idx; i<=max_idx; i++) { // use idx..max_idx-1
	    Operand use = new Reg(vr);
	    use.incr_omega(i);

	    int ii = vr_to_index.value(use);

	    pq_dn_use_gen[ii].lub_sum(cond_cq) ;
	    pq_dn_use_kill[ii].lub_sum(cond_cq) ;
	 }
	 for (i=idx+1 ; i<=max_idx; i++) { // define idx..max_idx
	    Operand def = new Reg(vr);
	    def.incr_omega(i);

	    int ii = vr_to_index.value(def);

	    pq_dn_use_gen[ii].lub_diff(cond_cq) ;
	    pq_dn_use_kill[ii].lub_diff(cond_cq) ; 
	    pq_dn_def_gen[ii].lub_sum(cond_cq) ;

	 }

	 // take care of the low end of the evr range (low end for forward)
	 Operand k_ll = new Reg(vr);
	 k_ll.incr_omega(idx);
      
	 int kii = vr_to_index.value(k_ll);

	 pq_dn_use_gen[kii] = fpq;
	 pq_dn_use_kill[kii] = fpq;
	 pq_dn_def_gen[kii] = fpq;
	 pq_dn_def_kill[kii] = fpq;
	 pq_dn_use_kill[kii].lub_sum(cond_cq);   // down_exposed under exit predicate
	 pq_dn_def_kill[kii].lub_sum(cond_cq);   // down_exposed under exit predicate

      }
      else if (is_swp_branch(op)) {
	 // Special case for SWP branch so that src and dest guards
	 // are the staging predicate. This code needs to be fixed once
	 // the PQS interface takes care of this internally

	 // process uses
	 for (src_oper(op) ; src_oper != 0 ; src_oper++) {
	    Operand& oper = (*src_oper) ;

	    // don't compute dn_use for mem_vr's
	    if (global_dfa->is_interesting(oper)) {
	       int ii = vr_to_index.value(oper);

	       pq_dn_use_gen[ii].lub_sum(cond_cq) ;
	       pq_dn_use_kill[ii].lub_sum(cond_cq) ;
	    }
	 }
	 // process defs
	 for (dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
	    Operand oper = (*dest_oper) ;

	    if (global_dfa->is_interesting(oper)) {
	       int ii = vr_to_index.value(oper);

	       pq_dn_use_gen[ii].lub_diff(cond_cq) ;
	       pq_dn_use_kill[ii].lub_diff(cond_cq) ;
	       pq_dn_def_gen[ii].lub_sum(cond_cq) ;
            }
	 }
      }

      else {
	 // for each op, process uses, then defs

	 // process uses
	 for (src_oper(op) ; src_oper != 0 ; src_oper++) {
	    Operand& oper = (*src_oper) ;

	    // don't compute dn_use for mem_vr's
	    if (oper.is_reg() || oper.is_macro_reg()) {
	       int ii = vr_to_index.value(oper);
               El_ref tmp1 = src_oper.get_ref() ;
               Pred_cookie pq1 = pj.get_lub_guard(tmp1) ;

	       pq_dn_use_gen[ii].lub_sum(pq1) ;
	       pq_dn_use_kill[ii].lub_sum(pq1) ;
	    }
	 }
	 // process defs
	 for (dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
	    Operand oper = (*dest_oper) ;

	    if (global_dfa->is_interesting(oper)) {
	       int ii = vr_to_index.value(oper);
               El_ref tmp1 = dest_oper.get_ref() ;
               Pred_cookie pq1 = pj.get_glb_guard(tmp1) ;
               Pred_cookie pq2 = pj.get_lub_guard(tmp1) ;

	       pq_dn_use_gen[ii].lub_diff(pq1) ;
	       pq_dn_use_kill[ii].lub_diff(pq1) ;
	       pq_dn_def_gen[ii].lub_sum(pq2) ;
	    }
	 }
      }
   }
   
   if (global_dfa == NULL) return;  // when timing pqs, global_dfa is null

   // expand local information into global gen/kill vectors
   if (dbg(dfa,5)) cdbg << "up_use_gen for blk " << blk->id();
   int ii ;
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
	 if (!tpq.is_disjoint(pq_up_use_gen[ii])) {  // gen if live on some path
	    int idx = global_dfa->operand_map.value(vr);
	    if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].liveness_gen.set_bit(idx);
	 }
      }
   }
   if (dbg(dfa,5)) cdbg << "\nup_use_kill for blk " << blk->id();
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
	 if (cond_cq.is_disjoint(pq_up_use_kill[ii])) {  // kill if dead on all paths 
	    int idx = global_dfa->operand_map.value(vr);
	    if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].liveness_kill.set_bit(idx);
	 }
      }
   }
   if (dbg(dfa,5)) cdbg << "\nup_def_gen for blk " << blk->id();
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
         if (!tpq.is_disjoint(pq_up_def_gen[ii])) {  // output if def on any paths
	    int idx = global_dfa->operand_map.value(vr);
	    if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].up_exposed_def_gen.set_bit(idx);
	 }
      }
   }
   if (dbg(dfa,5)) cdbg << "\nup_def_kill for blk " << blk->id();
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
         if (cond_cq.is_disjoint(pq_up_def_kill[ii])) {  // kill if killed on all paths
	    int idx = global_dfa->operand_map.value(vr);
	    if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].up_exposed_def_kill.set_bit(idx);
	 }
      }
   }
   if (dbg(dfa,5)) cdbg << "\ndn_use_gen for blk " << blk->id();
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
         if (!cond_cq.is_disjoint(pq_dn_use_gen[ii])) {  // gen if live on some path
	    int idx = global_dfa->operand_map.value(vr);
	    if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].down_exposed_use_gen.set_bit(idx);
	 }
      }
   }
   if (dbg(dfa,5)) cdbg << "\ndn_use_kill for blk " << blk->id();
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
         if (cond_cq.is_disjoint(pq_dn_use_kill[ii])) {  // kill if dead on all paths
	    int idx = global_dfa->operand_map.value(vr);
            if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].down_exposed_use_kill.set_bit(idx);
	 }
      }
   }
   if (dbg(dfa,5)) cdbg << "\ndn_def_gen for blk " << blk->id();
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
         if (!cond_cq.is_disjoint(pq_dn_def_gen[ii])) {  // output if def on any paths
            int idx = global_dfa->operand_map.value(vr);
	    if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].down_exposed_def_gen.set_bit(idx);
	 }
      }
   }
   if (dbg(dfa,5)) cdbg << "\ndn_def_kill for blk " << blk->id();
   for (ii = 0; ii<vr_count; ii++) {
      if(interesting.bit(ii)) {
	 Operand& vr = index_to_vr[ii];
         if (cond_cq.is_disjoint(pq_dn_def_kill[ii])) {  // kill if killed on all paths
            int idx = global_dfa->operand_map.value(vr);
	    if (dbg(dfa,5)) cdbg << " " << vr;
	    global_dfa->live_info[live_idx].down_exposed_def_kill.set_bit(idx);
         }
      }	 
   }
   if (dbg(dfa,5)) cdbg << endl;
   // ntclark 3/25/03 - fix memory leaks
   delete dest_oper_ptr;
   delete src_oper_ptr;
}


//
// Compute local liveness gen/kill for a BB/SB/HB region.  For SB/HB regions,
// we compute local definitions gen/kill between a specified exit and
// the region entry.
// This is quite different from the way things are handled in liveness
// analysis (gen/kill).
//

static Vector<Pred_cookie>* pq_gen_ptr = NULL ;
static Vector<Pred_cookie>* pq_kill_ptr = NULL ;

PUBLIC void
RD_gen_kill (Compound_region* blk, Op* exit_op,
             Reaching_defs_solver& global_rdefa, int rd_idx)
{
   int ii=0;
   
   if (dbg (rdef, 1))
      cdbg << "*\t\t\tRD_gen_kill, block-" << blk->id()
      << " exit_op-" << exit_op->id() << endl;
   
   // da PQS:
   PQS* pqs = get_pqs (blk);
   
   Pred_jar pj(blk)  ;
   
   if (dbg (rdef, 2))
      cdbg << "\t\t\t\tPQS is:" << *pqs;
   
   // get predicate which controls the path 
   Pred_cookie cond_cq = pj.get_lub_path_guard(get_first_region_op_from_subregions(blk), exit_op) ;
   
   
   // create vectors of pred cookies for gen/kill, initialize them:
   Pred_cookie fpq = Pred_jar::get_false() ;
   Pred_cookie tpq = Pred_jar::get_true() ;
   
   if (!pq_gen_ptr) {
      pq_gen_ptr = new Vector<Pred_cookie>(global_rdefa.rdm->def_counter, fpq) ;
   }
   else {
      if ((int)pq_gen_ptr->size() != global_rdefa.rdm->def_counter) {
         delete pq_gen_ptr ;
         pq_gen_ptr = new Vector<Pred_cookie>(global_rdefa.rdm->def_counter, fpq) ;
      }
      else {
         for (int j=0; j < global_rdefa.rdm->def_counter; j++) {
            (*pq_gen_ptr)[j].reset_to_false() ;
         }
      }
   }
   
   
   //
   if (!pq_kill_ptr) {
      pq_kill_ptr = new Vector<Pred_cookie>(global_rdefa.rdm->def_counter, tpq) ;
   }
   else {
      if ((int)pq_kill_ptr->size() != global_rdefa.rdm->def_counter) {
         delete pq_kill_ptr ;
         pq_kill_ptr = new Vector<Pred_cookie>(global_rdefa.rdm->def_counter, tpq) ;
      }
      else {
         for (int j=0; j < global_rdefa.rdm->def_counter; j++) {
            (*pq_kill_ptr)[j].reset_to_true() ;
         }
      }
   }
   
   
   Vector<Pred_cookie>& pq_gen = *pq_gen_ptr ;
   Vector<Pred_cookie>& pq_kill = *pq_kill_ptr ;

   Op_complete_and_pseudo_dests dest_oper ;
   
   //
   // traverse ops in hyperblock in linear order from entry to exit_op
   //
   bool done = false;
   for (Region_ops_C0_order opi2 (blk); done != true; opi2++) {
      // there should be no next time around:
      if ((*opi2) == exit_op)
         done = true;
      
      assert ((*opi2)->is_op());
      Op* op = (Op*)(*opi2);
      
      // TODO:
      // Any additions/changes here must be replicated in Reaching_defs_solver
      // chain construction code too.
      if (is_remap (op)) {
         Pred_cookie remap_cond_cq(true) ;

         List<Operand>* remap_defs = (List<Operand>*) 
            get_generic_attribute (op, "remap_expanded_defs") ;
         
         for (List_iterator<Operand> dest_oper(*remap_defs) ;
         dest_oper != 0; dest_oper++) { 
            // process the defs only:
            Operand oper = (*dest_oper);
            
            if (global_rdefa.is_interesting(oper)) {
               int oi = global_rdefa.rdm->operand_map.value (oper);
               
               for (List_iterator<int> li (global_rdefa.rdm->operand_to_def_map[oi]);
               li != 0; li++) {
                  int idx = *li ;
                  
                  // all defs get killed:
                  pq_gen[idx].lub_diff(remap_cond_cq) ;
                  pq_kill[idx].lub_diff(remap_cond_cq) ;
                  
                  // only the def belonging to this op is genned:
                  if (global_rdefa.rdm->rev_def_map[idx].get_op() == op) {                    
                     pq_gen[idx].lub_sum(remap_cond_cq) ;
                     pq_kill[idx].lub_sum(remap_cond_cq) ;
                  }
               }
            }
         }
         // Add one more kill for vrname[min_omega]
         Operand oper = op->src(SRC1) ;
         int min_omega = oper.min_omega() ;
         Operand new_reg = new Reg(oper);
         new_reg.incr_omega(min_omega);
         int oi = global_rdefa.rdm->operand_map.value(new_reg);
         for (List_iterator<int> li2 (global_rdefa.rdm->operand_to_def_map[oi]);
         li2 != 0; li2++) {
            int idx = *li2 ;
            // all defs get killed:
            pq_gen[idx].lub_diff(remap_cond_cq) ;
            pq_kill[idx].lub_diff(remap_cond_cq) ;
            
         }      
      }
      else if (is_swp_branch (op)) {
         // Special case for SWP branch so that src and dest guards
         // are the staging predicate. This code needs to be fixed once
         // the PQS interface takes care of this internally
         //
         // TODO:(Sumedh+Richard) Sanity checks needed on this piece of code.
         // Any additions/changes here must be replicated in Reaching_defs_solver
         // chain construction code too.
         
         for (dest_oper (op); dest_oper != 0; dest_oper++) {
            Operand oper = (*dest_oper);
            
            if (global_rdefa.is_interesting(oper)) {
               int oi = global_rdefa.rdm->operand_map.value (oper);
               
               for (List_iterator<int> li (global_rdefa.rdm->operand_to_def_map[oi]);
               li != 0; li++) {
                  int idx = *li ;
                  // all defs get killed:
                  pq_gen[idx].lub_diff(cond_cq) ;
                  pq_kill[idx].lub_diff(cond_cq) ;
                  
                  // only the def belonging to this op is genned:
                  if (global_rdefa.rdm->rev_def_map[*li].get_op() == op) {
                     pq_gen[idx].lub_sum(cond_cq) ;
                     pq_kill[idx].lub_sum(cond_cq) ;
                  }
               }
            }
         }
      }
      else {
         for (dest_oper (op); dest_oper != 0; dest_oper++) {
            // process the defs only:
            Operand oper = (*dest_oper);
            El_ref cur_ref = dest_oper.get_ref() ;

            if (global_rdefa.is_interesting(oper)) {
               int oi = global_rdefa.rdm->operand_map.value (oper);
               
	       Op* cur_def_op = cur_ref.get_op() ;
	       Pred_cookie pq1 = pj.get_lub_guard(cur_ref) ;
	       Pred_cookie pq2 = pj.get_glb_guard(cur_ref) ;
	       for (List_iterator<int> li (global_rdefa.rdm->operand_to_def_map[oi]);
		    li != 0; li++) {
                  int idx = *li ;
		  Op* other_def_op = (global_rdefa.rdm->rev_def_map[idx]).get_op() ;
                  // only the def belonging to this op is genned:
                  if (cur_def_op == other_def_op) {
                     pq_gen[idx].lub_sum(pq1) ;
                     pq_kill[idx].lub_sum(pq1) ;
                  }
                  else {
                     // all other defs get killed:
                     pq_gen[idx].lub_diff(pq1) ;
                     pq_kill[idx].lub_diff(pq2) ;
                  }
               }
            }
         }
      }
   }
   // expand local info into global gen Vector:
   if (dbg (rdef, 3))
      cdbg << "*\t\t\t\tIntegrate gen info:" << endl;
   for (ii=1; ii < global_rdefa.rdm->def_counter; ii++) {
      // output if def on any paths
      if (!cond_cq.is_disjoint(pq_gen[ii])) {
         El_ref& ref = global_rdefa.rdm->rev_def_map[ii];
         
         int idx = global_rdefa.rdm->def_to_i_map.value (ref);
         
         if (dbg (rdef, 3))
            cdbg << "\t\t\t\t" << "<" << idx << ", " << ref << ">, " << endl;
         
         global_rdefa.rd_info[rd_idx].rd_gen.set_bit (idx);
      }
   }
   if (dbg (rdef, 3)) cdbg << endl;
   
   // expand local info into global kill Vector:
   if (dbg (rdef, 3))
      cdbg << "*\t\t\t\tIntegrate kill info:" << endl;
   for (ii=1; ii < global_rdefa.rdm->def_counter; ii++) {
      // output if def killed on any path
      if (cond_cq.is_disjoint(pq_kill[ii])) {
         El_ref& ref = global_rdefa.rdm->rev_def_map[ii];
         int idx = global_rdefa.rdm->def_to_i_map.value (ref);
         if (dbg (rdef, 3))
            cdbg << "\t\t\t\t" << "<" << idx << ", " << ref << ">, " << endl;
         global_rdefa.rd_info[rd_idx].rd_kill.set_bit (idx);
      }
   }
   if (dbg (rdef, 3)) cdbg << endl;
}

// compute min coloring (using Chaitin-style elimination) assuming no spill
PRIVATE void
min_color_no_spill (Vector<Bitvector> &interfere)
{
  int n = 0;
  int size = interfere.size();

  // count edges in interference graph
  int edges = 0;
  for (int i=0; i<size; i++) {
    edges += interfere[i].ones_count(); // each edge counted twice
  }
  edges = edges >> 1;
  cout << "interference graph has " << edges << " edges." << endl;

  // while edges exit, reduce graph
  while (edges > 0) {
    n += 1; // increase min color threshold
    if (dbg(dfa,5)) cdbg << "n: " << n << endl;

    // iteratively remove nodes with degree less than n
    bool changed = true;
    while (changed) {
      changed = false;

      // remove nodes with degree less than n
      for (int i=0; i<size; i++) {
	int bits = interfere[i].ones_count();
	if (bits > 0 && bits < n) {
	  for (int j=0; j<size; j++) {
	    if (interfere[i].bit(j)) {
	      interfere[j].reset_bit(i);
	      edges -= 1;
	    }
	  }
	  interfere[i].clear();
	  if (dbg(dfa,5)) cdbg << "node " << i << " removed at n = " << n << endl;
	  changed = true;
	}
      }
    }
  }
  assert (edges == 0);  // sanity check

  cout << "color w/o spill: " << n << endl;
}

//PRIVATE void
void
local_liveness (Compound_region* blk)
{
  PQS* pqs = get_pqs (blk);
  VR_map* vr_map = get_local_vr_map (blk);
  if(pqs == NULL || vr_map == NULL) {
    create_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(blk));
    pqs = get_pqs(blk);
    vr_map = get_local_vr_map (blk);
  }
  assert(pqs != NULL && vr_map != NULL);

  int& vr_count = vr_map->vr_count;
  Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;

  // create Vector of liveness predicate expressions
  Vector<Pred_expr> live(vr_count);
  int i ;
  for (i=0; i<vr_count; i++) {
    live[i] = pqs->false_expr();
  }

  // create Vector of bitvectors for interference graph
  Vector<Bitvector> interfere(vr_count);
  for (i=0; i<vr_count; i++) {
    interfere[i].resize(vr_count);
  }

  // traverse ops in hyperblock in reverse linear order
  for (Region_ops_reverse_C0_order opi(blk); opi != 0; ++opi) {
    assert ( (*opi)->is_op() );
    Op* op = (Op*) (*opi);
    
    // for each op, process defs, then uses
    {
      for (Op_all_dests dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
	Operand oper = (*dest_oper) ;
	Port_num pn = dest_oper.get_port_num();

	if (oper.is_reg() || oper.is_macro_reg()) {
	  int ii = vr_to_index.value(oper);
	  pqs->lub_diff (live[ii], pqs->glb_dest_guard(op, pn));

	  // check for interference with other vars
	  for (int jj=0; jj<vr_count; jj++) {
	    if (jj == ii) continue;

#undef PDFA
//#define PDFA
#ifdef PDFA
	    if (!pqs->is_disjoint (pqs->lub_dest_guard(op, pn), live[jj])) {
	      cout << " interference between " << ii << ", " << jj << endl;
	      interfere[ELCOR_MAX(ii,jj)].set_bit(ELCOR_MIN(ii,jj));
	      interfere[ELCOR_MIN(ii,jj)].set_bit(ELCOR_MAX(ii,jj));
	    }
#else
	    if (!pqs->is_disjoint (pqs->true_pred(), live[jj])) {
	      cout << " interference between " << ii << ", " << jj << endl;
	      interfere[ELCOR_MAX(ii,jj)].set_bit(ELCOR_MIN(ii,jj));
	      interfere[ELCOR_MIN(ii,jj)].set_bit(ELCOR_MAX(ii,jj));
	    }
#endif
	  }
	}
      }
      for (Op_all_inputs src_oper(op) ; src_oper != 0 ; src_oper++) {
	Operand& oper = (*src_oper) ;
	Port_num pn = src_oper.get_port_num();

	if (oper.is_reg() || oper.is_macro_reg()) {
	  int ii = vr_to_index.value(oper);
	  pqs->lub_sum (live[ii], pqs->lub_src_guard(op, pn));
	}
      }
    }
  }

  // for now, assume all vars live-at-entry interfere w/each other
  for (int ii=0; ii<vr_count; ii++) {
    if (!pqs->is_disjoint (pqs->true_pred(), live[ii])) {  // if i live at entry
      for (int jj=ii+1; jj<vr_count; jj++) {
	if (!pqs->is_disjoint (pqs->true_pred(), live[jj])) { // j live at entry
	  cout << " interference between " << ii << ", " << jj << endl;
	  interfere[ELCOR_MAX(ii,jj)].set_bit(ELCOR_MIN(ii,jj));
	  interfere[ELCOR_MIN(ii,jj)].set_bit(ELCOR_MAX(ii,jj));
	}
      }
    }
  }

  min_color_no_spill(interfere);
}


PRIVATE VR_map*
create_vr_map_for_block (Compound_region* blk)
{
  assert (blk->is_bb() || blk->is_hb());

  if (dbg(dfa))
    cdbg << "create_vr_map_for_block " << blk->id() << endl;

  VR_map* vr_map = new VR_map;
  int& vr_count = vr_map->vr_count;
  int& vrnum_count = vr_map->vrnum_count;
  Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;
  Vector<Operand>& index_to_vr = vr_map->index_to_vr;

  Hash_map<int,int>& vrnum_to_index = vr_map->vrnum_to_index;
  Vector<int>& index_to_vrnum = vr_map->index_to_vrnum;
  List<Operand> local_vr_names ;
  
  vr_count = 0;
  vrnum_count = 0;

  // traverse ops in hyperblock in forward linear order
  for (Region_ops_C0_order opi(blk) ; opi != 0 ; opi++) {
    assert ( (*opi)->is_op() );
    Op* op = (Op*) (*opi);

    // if op is a remap, add its SRC1 to the list of names to be expanded
    if (is_remap(op)) {
       local_vr_names.add_tail(op->src(SRC1)) ;
    }

    // process uses
    for (Op_complete_inputs src_oper(op) ; src_oper != 0 ; src_oper++) {
      Operand tmpo = (*src_oper) ;
      if (!vr_to_index.is_bound(tmpo)) {
	if (tmpo.is_reg() || tmpo.is_mem_vr() || tmpo.is_macro_reg()) {
	  if (dbg(dfa,4)) cdbg << "vr_to_index " << vr_count << " " << tmpo << endl;
	  vr_to_index.bind(tmpo, vr_count++);
	}
      }
      if (tmpo.is_reg() || tmpo.is_mem_vr() || tmpo.is_macro_reg()) {
        if (!vrnum_to_index.is_bound(tmpo.vr_num())) {
	  if (dbg(dfa,4)) cdbg << "vrnum_to_index " << vrnum_count << " " << tmpo.vr_num() << endl;
	  vrnum_to_index.bind(tmpo.vr_num(), vrnum_count++);
	}
      }

    }
    // process defs
    for (Op_complete_dests dest_oper(op) ; dest_oper != 0 ; dest_oper++) {
      Operand tmpo = (*dest_oper) ; 
      if (!vr_to_index.is_bound(tmpo)) {
	if (tmpo.is_reg() || tmpo.is_mem_vr() || tmpo.is_macro_reg()) {
	  if (dbg(dfa,4)) cdbg << "vr_to_index " << vr_count << " " << tmpo << endl;
	  vr_to_index.bind(tmpo, vr_count++);
	}
      }
      if (tmpo.is_reg() || tmpo.is_mem_vr() || tmpo.is_macro_reg()) {
        if (!vrnum_to_index.is_bound(tmpo.vr_num())) {
	  if (dbg(dfa,4)) cdbg << "vrnum_to_index " << vrnum_count << " " << tmpo.vr_num() << endl;
	  vrnum_to_index.bind(tmpo.vr_num(), vrnum_count++);
	}
      }

    }
  }

  //  expand the vr_names over the min...max range
  for (List_iterator<Operand> vri(local_vr_names) ; vri != 0 ; vri++) {
     Operand oper = *vri ;
     int min_omega = oper.min_omega() ;
     int max_omega = oper.max_omega() ;
     for (int i=min_omega ; i<=max_omega ; i++) { // define idx..max_idx
  	Operand new_reg = new Reg(oper);
	new_reg.incr_omega(i);
	if (!vr_to_index.is_bound(new_reg)) {
	   vr_to_index.bind(new_reg, vr_count++);
	}
        // mchu
	if (!vrnum_to_index.is_bound(new_reg.vr_num())) {
	   vrnum_to_index.bind(new_reg.vr_num(), vrnum_count++);
	}

     }
  }
  
  index_to_vr.resize(vr_count);
  index_to_vrnum.resize(vrnum_count);
  for (Hash_map_iterator<Operand, int> mi(vr_to_index); mi != 0; mi++) {
    index_to_vr[(*mi).second] = (*mi).first;
  }

  for (Hash_map_iterator<int, int> mi(vrnum_to_index); mi != 0; mi++) {
    index_to_vrnum[(*mi).second] = (*mi).first;
  }

  return vr_map;
}


PUBLIC void
create_local_analysis_info (Compound_region* r)
{
  assert(r->is_bb() || r->is_hb());

  if (r->flag(EL_REGION_HAS_ANALYSIS_INFO))
    delete_local_analysis_info(r);

  // Create VR map and set attribute
  VR_map* vr_map = (VR_map*) get_local_vr_map(r);
  assert (vr_map == NULL);
  vr_map = create_vr_map_for_block (r);
  set_local_vr_map(r, vr_map);

  // Create PQS and set attribute
  PQS* pqs = get_pqs(r);
  assert (pqs == NULL);

  char *pqs_type;

  if (r->flag(EL_REGION_SOFTPIPE) &&
      r->flag(EL_REGION_SCHEDULED)) {
    pqs_type = (char*)"1bit_triv";
  } else if (r->is_hb()) {
    pqs_type = El_anal_pqs_hb;
  } else {
    pqs_type = El_anal_pqs_default;
  }
  if (strcmp(pqs_type, "1bit_triv") == 0) {
    pqs = new PQS_1bit_triv (r);
  } else if (strcmp(pqs_type, "1sym_triv") == 0) {
    pqs = new PQS_1sym_triv (r);
  } else if (strcmp(pqs_type, "1sym") == 0) {
    pqs = new PQS_1sym (r);
  } else if (strcmp(pqs_type, "1dnf") == 0) {
    pqs = new PQS_1dnf (r);
  } else {
    El_punt("unknown pqs type.");
  }
  set_pqs(r, pqs);

  r->set_flag(EL_REGION_HAS_ANALYSIS_INFO);
}


void create_local_analysis_info_for_all_hbs_bbs_real(Compound_region* r)
{
    if(r->is_op()) return;
    else if (r->is_bb() || r->is_hb()) {
        create_local_analysis_info(r);
    }
    else {
        for (Region_subregions subr(r); subr != 0; subr++) {
            Compound_region* subregion = (Compound_region*)*subr;
            create_local_analysis_info_for_all_hbs_bbs_real(subregion);
        }
    }
}

/* SAM 4-97, just a wrapper for the real function, so that timer
   instrumentation can be added easily */
void create_local_analysis_info_for_all_hbs_bbs(Compound_region* r)
{
   if (El_do_run_time)
      El_clock2.start();

   create_local_analysis_info_for_all_hbs_bbs_real(r);

   if (El_do_run_time) {
      El_clock2.stop();
      El_stopwatch_print_time("create_local_analysis_info_for_all_hbs_bbs",
                                El_clock2.last_elapsed());
   }
}

PUBLIC void
delete_local_analysis_info (Compound_region* r)
{
  // commented this out: why is it allowed to create info on
  // non-hb/non-bb regions, but deleting is forbidden?  -Marnix.
  // assert(r->is_bb() || r->is_hb());

  // Remove  VR map attribute and delete the object
  remove_local_vr_map(r) ;    
  // Remove  PQS attribute and delete the object
  remove_pqs(r);

  r->reset_flag(EL_REGION_HAS_ANALYSIS_INFO);
}

void delete_local_analysis_info_for_all_hbs_bbs_real(Compound_region* r)
{
    if(r->is_op()) return;
    else if (r->is_bb() || r->is_hb()) {
        delete_local_analysis_info(r);
    }
    else {
        for (Region_subregions subr(r); subr != 0; subr++) {
            Compound_region* subregion = (Compound_region*)*subr;
            delete_local_analysis_info_for_all_hbs_bbs_real(subregion);
        }
    }
}

/* SAM 4-97, Just a wrapper function */
void delete_local_analysis_info_for_all_hbs_bbs(Compound_region* r)
{
   if (El_do_run_time)
      El_clock2.start();

   delete_local_analysis_info_for_all_hbs_bbs_real(r);

   // ntclark 3/26/03 - trying to plug up memory leaks.
   free_liveness_gen_kill_mem();

   if (El_do_run_time) {
      El_clock2.stop();
      El_stopwatch_print_time("delete_local_analysis_info_for_all_hbs_bbs",
                                El_clock2.last_elapsed());
   }
}

// end of pred_analysis.cpp
