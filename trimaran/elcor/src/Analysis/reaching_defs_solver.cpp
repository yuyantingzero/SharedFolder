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
//      File:           reaching_defs_solver.cpp
//      Authors:        Sadun Anik, Sumedh Sathaye, Richard Johnson
//      Created:        August 1994
//      Description:    Reaching definitons analyzer
//                      As usual, interface functions are at the end.
//
//      August 1997     Added destructors to fix 2 memory leaks  - Marnix.
//      May/June 1996   Added the reaching definition analysis code
//                      upto formation of use-def chains.
//      June 26, 1996   Lost the file
//      June 27         Recreated from Sadun's seed code
//      July 2, 1996    Completed recovery and construction of UD chains
//                      without the support for use/defs due to remaps.
//      July 3, 1996    trivial modifications for compilation with HP-CC.
//      July 11, 1996   addition of the reaching defs info attribute.
//
///////////////////////////////////////////////////////////////////////////

#include "reaching_defs_solver.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "iterators.h"
#include "attributes.h"
#include "pred_analysis.h"
#include "list.h"
#include "dbg.h"
#include "pqs.h"
#include "pqs-1bit-triv.h"
#include "pqs-1sym-triv.h"
#include "el_stats_init.h"
#include "el_clock.h"
#include "pred_interface.h"
#include "edge_utilities.h"
#include "pqs-1dnf.h"

Reaching_defs_solver_data::Reaching_defs_solver_data(void)
  : rd_in (0), rd_out (0), rd_in_initial (0), rd_gen (0), rd_kill (0)
{
}

Reaching_defs_solver_data::Reaching_defs_solver_data(int n)
  : rd_in (n), rd_out (n), rd_in_initial (n), rd_gen (n), rd_kill (n)
{
}

Reaching_defs_solver_data::~Reaching_defs_solver_data(void)
{
}

bool
Reaching_defs_solver_data::operator==(const Reaching_defs_solver_data& d) const
{
  if ((rd_in == d.rd_in)
      && (rd_in_initial == d.rd_in_initial)
      && (rd_out == d.rd_out)
      && (rd_gen == d.rd_gen)
      && (rd_kill == d.rd_kill))
    return true;
  else
    return false;
}
//////////////////////////////////////////////////////////////////////////////

Reaching_defs_solver::~Reaching_defs_solver(void)
{
  delete rdm;
}

Reaching_defs_solver::Reaching_defs_solver(Region* r, unsigned int filter_flag)
  : graph(r, GRAPH_HBS_BBS_SESE),
    def_use(hash_El_ref, 10000), use_def(hash_El_ref, 10000), filter_mask(filter_flag)
{
   rdm = new Reaching_defs_maps(r,filter_flag) ;
   
   unsigned i;

   if (dbg(rdef, 1))
      cdbg << "*\t\tInstantiating the Reaching Defs solver" << endl;

   // initialize the data and stuff:
   Reaching_defs_solver_data dinit(rdm->def_counter);
   rd_info.resize(graph.succ_list.size(), dinit);
   
   // do the gen-kill per HB/BB computation:
   if (dbg(rdef, 1))
      cdbg << "*\t\tGenerating the gen-kill info:" << endl;
   
   for (i=0; i < rd_info.size(); i++)
      compute_gen_kill_info(i);

   // initialize live-in defs for entire region
   initialize_live_ins(r);
   
   if (dbg (rdef, 1))
      cdbg << "*\t\tDone instantiating the Reaching Defs solver" << endl;
}

void Reaching_defs_solver::initialize_live_ins(Region *r) 
{
  // parkhc 1/25/2005
  // initialize rd_in Vector for live-in defs for this region
  for (Region_entry_ops renti(r); renti != 0; renti++) {
    Op* entry_op = *renti;
    Region* entry_blk = entry_op->parent();

    bool found = false;
    // when a loop is in the HB and it is reformulated into BBs, 
    // the parent of entry_op is BB while BBs are not in the graph.f_map
    // Instead, HB and its exit_ops are entries in graph.f_map
    // So, if the entry_blk is not in the f_map, look for its parent region.
    while (true) {
      if (graph.f_map.is_bound(entry_blk)) {
        found = true;
        break;
      }
      if (entry_blk == r || entry_blk == NULL)
        break;
      entry_blk = entry_blk->parent();
    }
    assert(found);
    
    int blk_index = graph.f_map.value(entry_blk);

    for (Op_livein_dests desti(entry_op); desti != 0; desti++) {
      El_ref def = desti.get_ref();
      Operand* oprnd = &(*desti);
      if (rdm->is_interesting(*oprnd)) {
        // set rd_in_init Vector
        assert(rdm->def_to_i_map.is_bound(def));
        int def_index = rdm->def_to_i_map.value(def);
        rd_info[blk_index].rd_in_initial.set_bit(def_index);
      }
    }
  }
}

void Reaching_defs_solver::compute_gen_kill_info (int i)
{
  //
  // This is borrowed and tailored from flow_analysis:
  //
  // A hyperblock with N exit ops is represented as a tree
  // with N+1 nodes.
  //
  // One node is the root of the tree which accouts for the
  // entry to the hyperblock. The handle of this node will be the
  // corresponding HB*
  //
  // There are N nodes for N exit ops, the handles on these nodes are
  // the exit ops.
  //
  // 1) If graph.b_map[i] is a hyperblock then it is the entry node with
  //    empty transfer function.
  // 2) If it is an op then it is the node corresponding the path from
  //    the hyperblock entry to the hyperblock exit (for the hyperblock
  //    it sits in)

  Region* r = graph.b_map[i];

  if (r->is_hb())
    return;

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

    while (!blk->is_hb ())
      blk = blk->parent ();

    exit_op = (Op*)r;
  }

  RD_gen_kill (blk, exit_op, *this, i);
}

void Reaching_defs_solver::meet_function (int n, bool& tag)
{
  Reaching_defs_solver_data &rd = rd_info[n];

  Bitvector old_rd_in;

  if (tag == false)
    old_rd_in = rd.rd_in;

  rd.rd_in = rd.rd_in_initial;
  for (List_iterator<int> li (graph.pred_list[n]); li != 0; li++)
    rd.rd_in += rd_info[*li].rd_out;

  if (tag == false)
    tag = (rd.rd_in != old_rd_in);
   
  return;
}

void Reaching_defs_solver::transfer_function (int n, bool& tag)
{
  Reaching_defs_solver_data &rd = rd_info[n];
   
  Bitvector old_rd_out;

  if (tag == false)
    old_rd_out = rd.rd_out;

  rd.rd_out = rd.rd_in;
  rd.rd_out -= rd.rd_kill;
  rd.rd_out += rd.rd_gen;

  if (tag == false)
    tag = (rd.rd_out != old_rd_out);
   
  return;
}

void Reaching_defs_solver::solve_reaching_defs (void)
{
  bool not_done = true;
  int iteration_count = 0;

  if (dbg (rdef,1))
    cdbg << "*\t\tSolving for reaching defs info:" << endl;

  while (not_done) {
    iteration_count++;
    not_done = false;

    for (Alist_graph_dfs iter (graph, DOWN); iter != 0; iter++) {
      meet_function (*iter, not_done);
      transfer_function (*iter, not_done);
    }
  }
}

void Reaching_defs_solver::construct_uddu_chains (void)
{
  unsigned i=0;

  if (dbg (rdef,1))
    cdbg << "*\t\tConstructing ud/du chains:" << endl;

  // instantiate the chain maps:
  for (i=1; i < (unsigned)rdm->def_counter; i++)
    def_use.bind (rdm->rev_def_map[i], new List<El_ref>);

  for (i=1; i < (unsigned)rdm->use_counter; i++)
    use_def.bind (rdm->rev_use_map[i], new List<El_ref>);

  // similarly:
  for (i=0; i < rd_info.size(); i++) {
    Region* r = graph.b_map[i];

    if (r->is_op())
      continue;
    else
      do_chains ((Compound_region*)r, i);
  }
  for (i=0; i < rd_info.size(); i++) {
    Region* r = graph.b_map[i];

    if (r->is_op())
      continue;
    else
      do_remap_transitive_chain_fixup ((Compound_region*)r, i);
  }
  
}

static Vector<Pred_cookie>* pck_gen_ptr = NULL ;

void Reaching_defs_solver::do_chains (Compound_region* blk, int rd_idx)
{
   Region_ops_C0_order opi2;
   List_iterator<int> il;
   List_iterator<int> li;
   List_iterator<int> odli;
   List<Op*> remap_op_list ;

   PQS* pqs = get_pqs (blk);

   Pred_jar pj(blk) ;

   Pred cond = pqs->true_pred ();
   Op*  exit_op = get_last_region_op_from_subregions (blk);

   // make a local copy of incoming reaching defs:
   if (dbg (rdef, 3))
      cdbg << "\t\trd_index=" << rd_idx << " local_rd=" << rd_info[rd_idx].rd_in << endl;

   // if the exit op is a SWPipelinable branch:
   if (is_swp_branch (exit_op)) {
      Region_ops_C0_order opi (blk);
      assert (is_control_merge (*opi));
      opi++;

      Op* first_real_op = *opi;
      assert (!is_switch (first_real_op) && !is_merge (first_real_op));
      cond = pqs->lub_src_guard (first_real_op, SRC1);
    }

   Pred_cookie cond_cq = pj.get_lub_path_guard(get_first_region_op_from_subregions(blk), exit_op) ;

   Pred_cookie fpq = Pred_jar::get_false() ;
   Pred_cookie tpq = Pred_jar::get_true() ;
   //
   if (!pck_gen_ptr) {
      pck_gen_ptr = new Vector<Pred_cookie>(rdm->def_counter, fpq) ;
   } else {
      if ((int)pck_gen_ptr->size() != rdm->def_counter) {
         delete pck_gen_ptr ;
         pck_gen_ptr = new Vector<Pred_cookie>(rdm->def_counter, fpq) ;
      }
   }
   
   Bitvector& rd_in = rd_info[rd_idx].rd_in;
   for (int j=0; j < rdm->def_counter; j++) {
      if (rd_in.bit(j)) (*pck_gen_ptr)[j].reset_to_true() ;
      else (*pck_gen_ptr)[j].reset_to_false() ;
   }
   
   //
   
   Vector<Pred_cookie>& pck_gen = *pck_gen_ptr ;

   //
   // traverse ops in hyperblock in linear order from entry to exit_op
   //
   bool done = false;
   for (opi2 (blk); done != true; opi2++) {
      // there should be no next time around:
      if ((*opi2) == exit_op)
	 done = true;

      assert ((*opi2)->is_op());
      Op* op = (Op*)(*opi2);

      if (is_swp_branch (op)) {
	 // TODO:(Sumedh+Richard) Sanity checks needed on this piece of code.
	 //
	 // Special case for SWP branch so that src and dest guards
	 // are the staging predicate. This code needs to be fixed once
	 // the PQS interface takes care of this internally
	 // process the srcs to compute the use-defs:
	 //
	 // Maybe this discreminatory treatment in code will end
	 // once the bug in PQS is fixed:
	 //
	 for (Op_complete_and_pseudo_inputs src_oper1 (op); src_oper1 != 0; src_oper1++) {
	    Operand oper = (*src_oper1) ;
	    if (!is_interesting(oper)) continue ;
	    // for this operand, get the operand int:
	    El_ref useref =  src_oper1.get_ref() ;
	    int o_int = rdm->operand_map.value (*src_oper1);

	    // check all the defs associated with this operand:
	    for (odli (rdm->operand_to_def_map[o_int]); odli != 0; odli++) {
	       El_ref& defref = rdm->rev_def_map[*odli];
		  
	       if (!cond_cq.is_disjoint(pck_gen[*odli])) {
		  use_def.value(useref)->add_tail (defref);
		  def_use.value(defref)->add_tail (useref);
		     
		  if (dbg (rdef, 2))
		     cdbg << "\t\tdef-" << defref << " reaches" << endl
			  << "\t\t\tuse-" << useref << endl;
	       }
	    }
	 }

	 for (Op_complete_and_pseudo_dests dest_oper1 (op); dest_oper1 != 0; dest_oper1++) {
	    Operand oper = (*dest_oper1);
	    if (is_interesting(oper)) {
	       int operi = rdm->operand_map.value (oper);
	       for (li (rdm->operand_to_def_map[operi]); li != 0; li++) {
                  int idx = *li ;
		  // only the def belonging to this op is genned:
		  if (rdm->rev_def_map[idx].get_op() == op) {
                     pck_gen[idx].lub_sum(cond_cq) ;
		  } else { // all other defs get killed
		     pck_gen[idx].lub_diff(cond_cq) ;
		  }
	       }
	    }
	 }
      }
      else if (is_remap (op)) {
	 Pred remap_cond =  pqs->true_pred () ;
         Pred_cookie remap_cond_cq(pj.get_true()) ;
	 remap_op_list.add_tail(op) ;

	 List<Operand>* remap_uses = (List<Operand>*) 
	    get_generic_attribute (op, "remap_expanded_uses") ;
	 for (List_iterator<Operand> use_oper(*remap_uses) ;
	      use_oper != 0 ; use_oper++) {
	    int o_int = rdm->operand_map.value(*use_oper);
	    // find the useref corresponding to this use
	    El_ref remap_useref (op, &(*use_oper), IMP_SRC);
	    
	    // check all the defs associated with this operand:
	    for (odli(rdm->operand_to_def_map[o_int]); odli != 0; odli++) {
	       El_ref& defref = rdm->rev_def_map[*odli];

	       if (!tpq.is_disjoint(pck_gen[*odli])) {
		  use_def.value(remap_useref)->add_tail (defref);
		  def_use.value(defref)->add_tail (remap_useref);
	      
		  if (dbg(rdef, 2))
		     cdbg << "\t\tdef-" << defref << " reaches" << endl
			  << "\t\t\tuse-" << remap_useref << endl;
	       }
	    }
	 }
	  
	 List<Operand>* remap_defs = (List<Operand>*) 
	    get_generic_attribute (op, "remap_expanded_defs") ;
       
	 for (List_iterator<Operand> dest_oper(*remap_defs) ;
	      dest_oper != 0; dest_oper++) { 
	    // process the defs only:
	    Operand oper = (*dest_oper);

	    if (is_interesting(oper)) {
	       int oi = rdm->operand_map.value (oper);

	       for (List_iterator<int> li (rdm->operand_to_def_map[oi]);
		    li != 0; li++) {
                  int idx = *li ;
		  // only the def belonging to this op is genned:
		  if (rdm->rev_def_map[idx].get_op() == op) {
                     pck_gen[idx].lub_sum(remap_cond_cq) ;
		  } else { // all other defs get killed 
		     pck_gen[idx].lub_diff(remap_cond_cq) ;
		  }
	       }
	    }
	 }
	  
	 Operand oper = op->src(SRC1) ;
	 int min_omega = oper.min_omega() ;
	 Operand new_reg = new Reg(oper);
	 new_reg.incr_omega(min_omega);
	 int oi = rdm->operand_map.value(new_reg);
	 for (List_iterator<int> li2 (rdm->operand_to_def_map[oi]);
	      li2 != 0; li2++) {
            int idx = *li2 ;
	    // all defs get killed:
            pck_gen[idx].lub_diff(remap_cond_cq) ;
	 }
      }
  
      else {
	 // process the srcs to compute the use-defs:
	 for (Op_complete_and_pseudo_inputs src_oper2(op); src_oper2 != 0; src_oper2++) {
	    Operand oper = *src_oper2 ;
	    if (!is_interesting(oper)) continue ;
	    // for this operand, get the operand int:

	    El_ref useref = src_oper2.get_ref() ;
	    Pred_cookie use_guard = pj.get_lub_guard(useref) ;
	    int o_int = rdm->operand_map.value(*src_oper2);

	    // check all the defs associated with this operand:
	    for (odli (rdm->operand_to_def_map[o_int]); odli != 0; odli++) {
	       El_ref& defref = rdm->rev_def_map[*odli];

	       if (!use_guard.is_disjoint(pck_gen[*odli])) {
		  use_def.value(useref)->add_tail (defref);
		  def_use.value(defref)->add_tail (useref);
	      
		  if (dbg(rdef, 2))
		     cdbg << "\t\tdef-" << defref << " reaches" << endl
			  << "\t\t\tuse-" << useref << endl;
	       }
	    }
	 }

	  
	 for (Op_complete_and_pseudo_dests dest_oper2(op); dest_oper2 != 0; dest_oper2++) {
	    // process the defs only:
	    Operand oper = (*dest_oper2);

	    if (is_interesting(oper)) {
                El_ref tmp1 = dest_oper2.get_ref() ;
                Pred_cookie pq1 = pj.get_lub_guard(tmp1) ;
                Pred_cookie pq2 = pj.get_glb_guard(tmp1) ;

	       int operi = rdm->operand_map.value(oper);

	       for (li (rdm->operand_to_def_map[operi]); li != 0; li++) {
                  int idx = *li ;
                  // only the def belonging to this op is genned:
                  if (rdm->rev_def_map[idx].get_op() == op) {
                     pck_gen[idx].lub_sum(pq1) ;
                  }
                  else {
                     // all other defs get killed:
                     pck_gen[idx].lub_diff(pq2) ;
                  }
	       }
	    }
	 }
      }
   }
}

void Reaching_defs_solver::do_remap_transitive_chain_fixup(Compound_region* blk, int rd_idx)
{
   
   List_iterator<El_ref> aui ;
   List_iterator<El_ref> adi ;
   
   Region_ops_C0_order r_opi;
   for (r_opi (blk); r_opi != 0 ; r_opi++) {
      if (!is_remap(*r_opi)) continue ;
      Op* cur_remap = *r_opi ;
      List<Operand>* remap_uses = (List<Operand>*) 
	 get_generic_attribute (cur_remap, "remap_expanded_uses") ;
      List<Operand>* remap_defs = (List<Operand>*) 
	 get_generic_attribute (cur_remap, "remap_expanded_defs") ;
      List_iterator<Operand> r_usei(*remap_uses) ;
      List_iterator<Operand> r_defi(*remap_defs) ;

      List<El_ref> running_remap_use_list ; 
      List_iterator<El_ref> rruli ;
      while (r_usei != 0) {
	 El_ref use_ref(cur_remap,&(*r_usei), IMP_SRC);
	 El_ref def_ref(cur_remap,&(*r_defi), IMP_DEST);
	 if (dbg(rdef, 2))
	    cdbg << "\tbypassing chains from " << use_ref <<
	       " to "<< def_ref << endl ;
	 // Sparser implemenation:
	 // 1) construct a running list of expanded uses whose
	 //    reaching defs will be propagated
	 // 2) when an expanded def is seen who reaches anything other than
	 //    an expanded use for this remap add all the new edges
	 // 3) if there is no du edge from an expanded def to the next expanded
	 //    use, clear the running list of expanded uses
	 //    otherwise add new expanded use to the running list and continue

	 running_remap_use_list.add_tail(use_ref) ;
	 //
	 // Find all the links coming to use_ref and connect them
	 // to uses the def_ref is connected to.
	 //
	 List<El_ref>* actual_uses = def_use.value(def_ref) ;
	 //
	 bool chain_continues = false ;
	 for (aui(*actual_uses) ; aui != 0 ; aui++) {
	    El_ref& au = *aui ;
	    if (au.get_op() == cur_remap) {
	       chain_continues = true ;
	       continue ;
	    }
	    else {
	       List<El_ref>* au_def_list = use_def.value(*aui) ;
	       for ( rruli(running_remap_use_list) ; rruli != 0 ; rruli++) {
		  El_ref& cur_remap_use = *rruli ;
		  List<El_ref>* actual_defs = use_def.value(cur_remap_use) ;
		  for (adi(*actual_defs) ; adi != 0 ; adi++) {
		     El_ref& ad = *adi ;
		     if (ad.get_op() == cur_remap) continue ;
		     List<El_ref>* ad_use_list = def_use.value(ad) ;
		     ad_use_list->add_tail(au) ;
		     au_def_list->add_tail(ad) ;
		     if (dbg(rdef, 1))
			cdbg << "\t*\tdef-" << ad << " now reaches" << endl
			     << "\t\t\tuse-" << au << endl;
		  }
	       }
	    }
	 }
	 if (chain_continues == false) {
	    running_remap_use_list.clear() ;
	 }
	 r_usei++ ;
	 r_defi++ ;
      }
      for (r_usei(*remap_uses) ; r_usei != 0 ; r_usei++) {
	 El_ref use_ref(cur_remap,&(*r_usei), IMP_SRC);
	 List<El_ref>* actual_defs = use_def.value(use_ref) ;
	 for (adi(*actual_defs) ; adi != 0 ; adi++) {
	    El_ref& ad = *adi ;
	    List<El_ref>* ad_use_list = def_use.value(ad) ;
	    ad_use_list->remove(use_ref) ;
	 }
	 actual_defs->clear() ;
      }
      for (r_defi(*remap_defs) ; r_defi != 0 ; r_defi++) {
	 El_ref def_ref(cur_remap,&(*r_defi), IMP_DEST);
	 List<El_ref>* actual_uses = def_use.value(def_ref) ;
	 for (aui(*actual_uses) ; aui != 0 ; aui++) {
	    El_ref& au = *aui ;
	    List<El_ref>* au_def_list = use_def.value(au) ;
	    au_def_list->remove(def_ref) ;
	 }
	 actual_uses->clear() ;
      }
   }
}
	     
		  
// 	       }
// 	       List<El_ref>* au_def_list = use_def.value(*aui) ;
// 	       au_def_list->remove(def_ref) ;
// 	       au_def_list->add_tail(ad) ;
// 	       ad_use_list->add_tail(au) ;
// 	       if (dbg(rdef, 2))
// 		  cdbg << "\t*\tdef-" << ad << " now reaches" << endl
// 		       << "\t\t\tuse-" << au << endl;
// 	    }
// 	 for (List_iterator<El_ref> adi(*actual_defs) ; adi != 0 ; adi++) {
// 	    El_ref& ad = *adi ;
// 	    List<El_ref>* ad_use_list = def_use.value(ad) ;
// 	    ad_use_list->remove(use_ref) ;
// 	    for (List_iterator<El_ref> aui(*actual_uses) ; aui != 0 ; aui++) {
// 	       El_ref& au = *aui ;
// 	       List<El_ref>* au_def_list = use_def.value(*aui) ;
// 	       au_def_list->remove(def_ref) ;
// 	       au_def_list->add_tail(ad) ;
// 	       ad_use_list->add_tail(au) ;
// 	       if (dbg(rdef, 2))
// 		  cdbg << "\t*\tdef-" << ad << " now reaches" << endl
// 		       << "\t\t\tuse-" << au << endl;
// 	    }
// 	 }
// 	 r_usei++ ;
// 	 r_defi++ ;
//       }
//    }
// }


void Reaching_defs_solver::clone_uddu_chains (Reaching_defs_info* rdi)
{
  rdi->use_def = use_def;
  rdi->def_use = def_use;
}
///////////////////////////////////////////////////////////////////////////////

Reaching_defs_maps::~Reaching_defs_maps (void)
{
}

Reaching_defs_maps::Reaching_defs_maps (Region* r, unsigned int filter_flag)
  : operand_counter (1), def_counter (1), use_counter (1),
    def_to_i_map (hash_El_ref, 1000), use_to_i_map (hash_El_ref, 1000),
    operand_map (hash_operand, 1000), def_to_operand_map (hash_int, 1000),
    use_to_operand_map (hash_int, 1000)
{

   filter_mask = filter_flag ;
   // iterators:
   Region_all_ops op_iter;
   Region_entry_ops en_op_iter;
   Region_exit_ops ex_op_iter;
   Region_entry_edges en_edge_iter;
   Region_exit_edges ex_edge_iter;

   List_iterator<Op*> vri;
   List_iterator<int> li;
   int i;
   Hash_map_iterator<Operand,int> mdi;
   Hash_map_iterator<El_ref,int> edi;
   List<Op*> local_vr_ops;

   if (dbg (rdef, 1))
    cdbg << "*\t\tConstructing the Reaching Defs solver maps" << endl;


   //
   // Use liveness information for entry/exit processing
   //

   for (en_op_iter(r) ; en_op_iter != 0 ; en_op_iter++) {
      Op* cur_op = *en_op_iter ;
      List<Operand>* entry_pseudo_defs = 
         (List<Operand>*)get_generic_attribute(cur_op, "entry_pseudo_defs") ;
      if (entry_pseudo_defs == NULL) {
         entry_pseudo_defs = new List<Operand>();
      }
      else {
         entry_pseudo_defs->clear() ;
      }
      for(en_edge_iter(r) ; en_edge_iter != 0 ; en_edge_iter++) {
         Edge* cur_edge = *en_edge_iter ;
         if (cur_edge->dest() != cur_op) continue ;
         Liveness_info* linfo ;
         Liveness_info_iterator li ;
         linfo = get_liveness_info(cur_edge) ;
         if (linfo) {
            for (li(*linfo) ; li != 0 ; li++) {
               Operand& tmpoper = *li ;
	       if (!is_interesting(tmpoper)) continue ;
               if (!operand_map.is_bound(tmpoper)) {
                  operand_map.bind(tmpoper,operand_counter) ;
                  operand_counter++ ;
               }
               if (!entry_pseudo_defs->is_member(tmpoper)){
                  entry_pseudo_defs->add_tail(tmpoper) ;
               }
            }
         }
      }
      if (entry_pseudo_defs->is_empty()) {
         delete entry_pseudo_defs ;
      }
      else {
         set_generic_attribute (cur_op, "entry_pseudo_defs", entry_pseudo_defs) ;
      }
   }
   for(ex_op_iter(r) ; ex_op_iter != 0 ; ex_op_iter++) {
      Op* cur_op = *ex_op_iter ;
      List<Operand>* taken_exit_pseudo_uses = 
         (List<Operand>*)get_generic_attribute(cur_op, "taken_exit_pseudo_uses") ;
      List<Operand>* fallthrough_exit_pseudo_uses = 
         (List<Operand>*)get_generic_attribute(cur_op, "fallthrough_exit_pseudo_uses") ;
      if (taken_exit_pseudo_uses == NULL) {
         taken_exit_pseudo_uses = new List<Operand>();
      }
      else {
         taken_exit_pseudo_uses->clear() ;
      }
      if (fallthrough_exit_pseudo_uses == NULL) {
         fallthrough_exit_pseudo_uses = new List<Operand>();
      }
      else {
         fallthrough_exit_pseudo_uses->clear() ;
      }
      for(ex_edge_iter(r) ; ex_edge_iter != 0 ; ex_edge_iter++) {
         Edge* cur_edge = *ex_edge_iter ;
         if (cur_edge->src() != cur_op) continue ;

	 List<Operand>* tmp_exit_pseudo_uses ;
	 if (is_fall_through(cur_edge)) {
	    tmp_exit_pseudo_uses = fallthrough_exit_pseudo_uses ;
	 }
	 else {
	    tmp_exit_pseudo_uses = taken_exit_pseudo_uses ;
	 }
	 
         Liveness_info* linfo ;
         Liveness_info_iterator li ;
         
         linfo = get_liveness_info(cur_edge) ;
         if (linfo) {
            for (li(*linfo) ; li != 0 ; li++) {
               Operand& tmpoper = *li ;
	       if (!is_interesting(tmpoper)) continue ;
               if (!operand_map.is_bound(tmpoper)) {
                  operand_map.bind(tmpoper,operand_counter) ;
                  operand_counter++ ;
               }
               if (!tmp_exit_pseudo_uses->is_member(tmpoper)){
                  tmp_exit_pseudo_uses->add_tail(tmpoper) ;
               }
            }
         }
      }
      if (taken_exit_pseudo_uses->is_empty()) {
         delete taken_exit_pseudo_uses ;
      }
      else {
         set_generic_attribute (cur_op, "taken_exit_pseudo_uses", taken_exit_pseudo_uses) ;
      }
      if (fallthrough_exit_pseudo_uses->is_empty()) {
         delete fallthrough_exit_pseudo_uses ;
      }
      else {
         set_generic_attribute (cur_op, "fallthrough_exit_pseudo_uses", fallthrough_exit_pseudo_uses) ;
      }
   }

   //
   // Create the operand map
   // 

   for (op_iter (r); op_iter != 0; op_iter++) {
      Op* cur_op = *op_iter;
      // should be able to track the remaps down:
      if (is_remap (cur_op))
      local_vr_ops.add_tail (cur_op);

      for (Op_complete_and_pseudo_inputs src_oper1 (cur_op); src_oper1 != 0; src_oper1++) {
         Operand& tmpo = *src_oper1;

         if (is_interesting(tmpo)) {
	    if (!operand_map.is_bound (tmpo)) {
	       operand_map.bind (tmpo, operand_counter);
	       operand_counter++;
	    }
         }
      }
      for (Op_complete_and_pseudo_dests dest_oper1 (cur_op); dest_oper1!=0; dest_oper1++) {
         Operand& tmpo = *dest_oper1;

         if (is_interesting(tmpo)) {
	    if (!operand_map.is_bound (tmpo)) {
	       operand_map.bind (tmpo, operand_counter);
	       operand_counter++;
	    }
         }
      }
   }


   //
   // Create the definition map
   //
   for (op_iter (r); op_iter != 0; op_iter++) {
      Op* cur_op = *op_iter;

      for (Op_complete_and_pseudo_dests dest_oper2 (cur_op); dest_oper2 != 0; dest_oper2++){
         Operand* cur_oprnd = &(*dest_oper2);

         El_ref new_def = dest_oper2.get_ref() ;
         if (is_interesting(*cur_oprnd)) {
      	    if (!def_to_i_map.is_bound (new_def)) {
               def_to_i_map.bind (new_def, def_counter);
               def_counter++;
	    }
	 }
      }
   }

   //
   // Create the use map
   //
   for (op_iter (r); op_iter != 0; op_iter++) {
      Op* cur_op = *op_iter;

      for (Op_complete_and_pseudo_inputs src_oper2(cur_op); src_oper2 != 0; src_oper2++) {
	 Operand* cur_oprnd = &(*src_oper2);
	 El_ref new_use = src_oper2.get_ref() ;
	 if (is_interesting(*cur_oprnd)) {
	    if (!use_to_i_map.is_bound(new_use)) {
	       use_to_i_map.bind(new_use, use_counter);
	       use_counter++;
	    }
	 }
      }
   }

   //
   // Expand the vr_names into full names:
   //

   for (vri (local_vr_ops); vri != 0; vri++) {
     Op* cur_remap = *vri ;
     Operand oper = cur_remap->src (SRC1);
     List<Operand>* remap_expanded_uses = new List<Operand>();
     List<Operand>* remap_expanded_defs = new List<Operand>();    

     int min_omega = oper.min_omega() ;
     int max_omega = oper.max_omega() ;
     for (int i = min_omega ; i <= max_omega ; i++) { // define idx...max_idx
   	Operand new_reg = new Reg (oper);
   	new_reg.incr_omega (i);
   	if (i != max_omega) {
           remap_expanded_uses->add_tail(new_reg) ;
   	}
   	if (i != min_omega) {
           remap_expanded_defs->add_tail(new_reg) ;
   	}
    
   	// add to the operand map:
   	if (!operand_map.is_bound (new_reg)) {
           operand_map.bind (new_reg, operand_counter);
           operand_counter++;
   	}
     }

     List_iterator<Operand> operi ;
  
     // Add to the def map:
     for (operi(*remap_expanded_defs) ; operi != 0 ; operi++) {
   	El_ref new_def (cur_remap, &(*operi), IMP_DEST);
   	if (!def_to_i_map.is_bound (new_def)) {
           def_to_i_map.bind (new_def, def_counter);
           def_counter++;
   	}
     }

     // Add to the use map:
     for (operi(*remap_expanded_uses) ; operi != 0 ; operi++) {
   	El_ref new_use (cur_remap, &(*operi), IMP_SRC);
   	if (!use_to_i_map.is_bound (new_use)) {
           use_to_i_map.bind (new_use, use_counter);
           use_counter++;
   	}
     }

     set_generic_attribute (cur_remap, "remap_expanded_uses",
			    remap_expanded_uses);
     set_generic_attribute (cur_remap, "remap_expanded_defs",
			    remap_expanded_defs);
   }

   //
   // Create the reverse operand map
   //
   rev_operand_map.resize (operand_counter);
   for (mdi (operand_map); mdi != 0; mdi++)
    if ((*mdi).second > 0)
      rev_operand_map[(*mdi).second] = (*mdi).first;
   
   //
   // create the reverse definition map:
   //
   rev_def_map.resize (def_counter);
   for (edi (def_to_i_map); edi != 0; edi++)
    if ((*edi).second > 0)
      rev_def_map[(*edi).second] = (*edi).first;

   //
   // create the reverse use map:
   //
   rev_use_map.resize (use_counter);
   for (edi (use_to_i_map); edi != 0; edi++)
    if ((*edi).second > 0)
      rev_use_map[(*edi).second] = (*edi).first;

   //
   // create the def-int to operand-int and
   // operand-int to def-int-list maps:
   //
   operand_to_def_map.resize (operand_counter);
   for (edi (def_to_i_map); edi != 0; edi++) {
    if ((*edi).second > 0) {
      int opint = operand_map.value ((*edi).first.get_operand());
      def_to_operand_map.bind ((*edi).second, opint);
      operand_to_def_map[opint].add_tail ((*edi).second);
    }
   }

   //
   // create the use-int to operand-int and
   // operand-int to use-int-list maps:
   //
   operand_to_use_map.resize (operand_counter);
   for (edi (use_to_i_map); edi != 0; edi++) {
    if ((*edi).second > 0) {
      int opint = operand_map.value ((*edi).first.get_operand());
      use_to_operand_map.bind ((*edi).second, opint);
      operand_to_use_map[opint].add_tail ((*edi).second);
    }
   }

   //
   // Print all if asked to:
   //
   if (dbg (rdef, 4)) {
    cdbg << "*\t\t\tPrinting the operand map:" << endl;
    for (mdi (operand_map); mdi != 0; mdi++)
      if ((*mdi).second > 0)
   	cdbg << "*\t\t\t{" << (*mdi).second << ", "
	     << (*mdi).first << "}" << endl;
    cdbg << "*" << endl;

    cdbg << "*\t\t\tPrinting the reverse operand map:" << endl;
    for (i=1; i < (int)rev_operand_map.size(); i++)
      cdbg << "*\t\t\t{" << i << ", "
           << rev_operand_map[i] << "}" << endl;
    cdbg << "*" << endl;

    cdbg << "*\t\t\tPrinting the def map:" << endl;
    for (edi (def_to_i_map); edi != 0; edi++)
      if ((*edi).second > 0)
   	cdbg << "*\t\t\t{" << (*edi).second << ", "
	     << (*edi).first << "}" << endl;
    cdbg << "*" << endl;

    cdbg << "*\t\t\tPrinting the reverse def map:" << endl;
    for (i=1; i < (int)rev_def_map.size(); i++)
      cdbg << "*\t\t\t{" << i << ", " << rev_def_map[i] << "}" << endl;
    cdbg << "*" << endl;

    cdbg << "*\t\t\tPrinting the use map:" << endl;
    for (edi (use_to_i_map); edi != 0; edi++)
      if ((*edi).second > 0)
   	cdbg << "*\t\t\t{" << (*edi).second << ", "
	     << (*edi).first << "}" << endl;
    cdbg << "*" << endl;

    cdbg << "*\t\t\tPrinting the reverse use map:" << endl;
    for (i=1; i < (int)rev_use_map.size(); i++)
      cdbg << "*\t\t\t{" << i << ", " << rev_use_map[i] << "}" << endl;
    cdbg << "*" << endl;

    cdbg << "*\t\t\tPrinting the operand_int-to-def_int map:" << endl;
    for (i=1; i < (int)operand_to_def_map.size(); i++) {
      cdbg << "*\t\t\t{" << i << ", ";
      cdbg << "List=[";
      for (li (operand_to_def_map[i]); li != 0;) {
   	cdbg << *li;
   	li++;
   	if (li != 0)
          cdbg << ", ";
      }
      cdbg << "]";
      cdbg << "}" << endl;
    }
    cdbg << "*" << endl;

    cdbg << "*\t\t\tPrinting the operand_int-to-use_int map:" << endl;
    for (i=1; i < (int)operand_to_use_map.size(); i++) {
       cdbg << "*\t\t\t{" << i << ", ";
       cdbg << "List=[";
       for (li (operand_to_use_map[i]); li != 0;) {
          cdbg << *li;
          li++;
          if (li != 0)
             cdbg << ", ";
       }
       cdbg << "]";
       cdbg << "}" << endl;
    }
    cdbg << "*" << endl;
   }
   
   if (dbg (rdef, 1))
      cdbg << "*\t\tDone constructing the Reaching Defs solver maps" << endl;
}


/* SAM 7-97: changed from using data types to use virtual register file types */
bool Reaching_defs_maps::is_interesting(Operand& oper) 
{
   if ((filter_mask & ANALYZE_MEMVR) && oper.is_mem_vr()) return true ;
   if ((filter_mask & ANALYZE_ALLREG) && (oper.is_reg() || oper.is_macro_reg())) {
      if ((filter_mask & ANALYZE_ALLREG) == ANALYZE_ALLREG) return true ;
      Reg_file cur_rfile = oper.file_type() ;
      if ((filter_mask & ANALYZE_INT) && (cur_rfile == GPR)) return true ;
      if ((filter_mask & ANALYZE_FP) && (cur_rfile == FPR)) return true ;
      if ((filter_mask & ANALYZE_BTR) && (cur_rfile == BTR)) return true ;
      if ((filter_mask & ANALYZE_PR) && (cur_rfile == PR)) return true ;
      if ((filter_mask & ANALYZE_CR) && (cur_rfile == CR)) return true ;
   }
   return false ;
}



/* SAM 7-97: changed from using data types to use virtual register file types */
bool Reaching_defs_solver::is_interesting(Operand& oper) 
{
   if ((filter_mask & ANALYZE_MEMVR) && oper.is_mem_vr()) return true ;
   if ((filter_mask & ANALYZE_ALLREG) && (oper.is_reg() || oper.is_macro_reg())) {
      if ((filter_mask & ANALYZE_ALLREG) == ANALYZE_ALLREG) return true ;
      Reg_file cur_rfile = oper.file_type() ;
      if ((filter_mask & ANALYZE_INT) && (cur_rfile == GPR)) return true ;
      if ((filter_mask & ANALYZE_FP) && (cur_rfile == FPR)) return true ;
      if ((filter_mask & ANALYZE_BTR) && (cur_rfile == BTR)) return true ;
      if ((filter_mask & ANALYZE_PR) && (cur_rfile == PR)) return true ;
      if ((filter_mask & ANALYZE_CR) && (cur_rfile == CR)) return true ;
   }
   return false ;
}

/* SAM 7-97: changed from using data types to use virtual register file types */
bool Reaching_defs_info::is_interesting(Operand& oper) 
{
   if ((filter_mask & ANALYZE_MEMVR) && oper.is_mem_vr()) return true ;
   if ((filter_mask & ANALYZE_ALLREG) && (oper.is_reg() || oper.is_macro_reg())) {
      if ((filter_mask & ANALYZE_ALLREG) == ANALYZE_ALLREG) return true ;
      Reg_file cur_rfile = oper.file_type() ;
      if ((filter_mask & ANALYZE_INT) && (cur_rfile == GPR)) return true ;
      if ((filter_mask & ANALYZE_FP) && (cur_rfile == FPR)) return true ;
      if ((filter_mask & ANALYZE_BTR) && (cur_rfile == BTR)) return true ;
      if ((filter_mask & ANALYZE_PR) && (cur_rfile == PR)) return true ;
      if ((filter_mask & ANALYZE_CR) && (cur_rfile == CR)) return true ;
   }
   return false ;
}



///////////////////////////////////////////////////////////////////////////////

Reaching_defs_info::Reaching_defs_info (unsigned int filter_flag)
  : def_use (hash_El_ref, 1000), use_def (hash_El_ref, 1000), filter_mask(filter_flag)
{
   
}

Reaching_defs_info::~Reaching_defs_info (void)
{
  for(Hash_map_iterator<El_ref, List<El_ref>*> mapit1(def_use);
      mapit1 != 0; mapit1++)
    delete (*mapit1).second;
  for(Hash_map_iterator<El_ref, List<El_ref>*> mapit2(use_def);
      mapit2 != 0; mapit2++)
    delete (*mapit2).second;
}

List<El_ref>* Reaching_defs_info::get_du_chain (El_ref& def)
{
  assert (is_interesting_ref(def)) ;
  return (def_use.value(def));
}

List<El_ref>* Reaching_defs_info::get_ud_chain (El_ref& use)
{
  assert (is_interesting_ref(use)) ;
  return (use_def.value(use));
}

void Reaching_defs_info::remove_use(El_ref& use)
{
   if (!is_interesting_ref(use)) return ;
   // Remove the use from wherever it appears in the du lists
   List<El_ref>* rd_list = get_ud_chain(use) ;
   for (List_iterator<El_ref> li(*rd_list) ; li != 0 ; li++) {
      El_ref& cur_def = *li ;
      List<El_ref>* uses_list = get_du_chain(cur_def) ;
      uses_list->remove(use) ;
   }
   // Remove the use from the ud chain
   use_def.unbind(use) ;
}

void Reaching_defs_info::remove_def(El_ref& def)
{
   if (!is_interesting_ref(def)) return ;
   // Remove the def from wherever it appears in the ud lists
   List<El_ref>* ru_list = get_du_chain(def) ;
   for (List_iterator<El_ref> li(*ru_list) ; li != 0 ; li++) {
      El_ref& cur_use = *li ;
      List<El_ref>* defs_list = get_ud_chain(cur_use) ;
      defs_list->remove(def) ;
   }
   // Remove the def from the du chain
   def_use.unbind(def) ;
}

void Reaching_defs_info::clone_ud_for_new_use(El_ref& new_use, El_ref& old_use)
{
   if (!is_interesting_ref(old_use)) return ;
   List<El_ref>* rd_list = get_ud_chain(old_use) ;
   // Clone the ud list
   List<El_ref>* new_rd_list = new List<El_ref>(*rd_list) ;
   use_def.bind(new_use,new_rd_list) ;
   // Add this use to the du list of reaching defs
   for (List_iterator<El_ref> li(*rd_list) ; li != 0 ; li++) {
      El_ref& cur_def = *li ;
      List<El_ref>* uses_list = get_du_chain(cur_def) ;
      uses_list->add_tail(new_use) ;
   }
}

void Reaching_defs_info::clone_du_for_new_def(El_ref& new_def, El_ref& old_def)
{
   if (!is_interesting_ref(old_def)) return ;   
   List<El_ref>* ru_list = get_du_chain(old_def) ;
   // Clone the du list
   List<El_ref>* new_ru_list = new List<El_ref>(*ru_list) ;
   def_use.bind(new_def,new_ru_list) ;
   // Add this def to the ud list of reaching uses
   for (List_iterator<El_ref> li(*ru_list) ; li != 0 ; li++) {
      El_ref& cur_use = *li ;
      List<El_ref>* defs_list = get_ud_chain(cur_use) ;
      defs_list->add_tail(new_def) ;
   }
}

void Reaching_defs_info::add_du_ud_links(El_ref& def, El_ref& use)
{
   if (!is_interesting_ref(use)) return ;
   List<El_ref>* rd_list ;
   if (use_def.is_bound(use)) {
      rd_list = get_ud_chain(use) ;
   }
   else {
      rd_list = new List<El_ref>() ;
      use_def.bind(use,rd_list) ;
   }
   rd_list->add_tail(def) ;
   
   List<El_ref>* ru_list ;
   if (def_use.is_bound(def)) {
      ru_list = get_du_chain(def) ;
   }
   else {
      ru_list = new List<El_ref>() ;
      def_use.bind(def,ru_list) ;
   }
   ru_list->add_tail(use) ;
}

void Reaching_defs_info::remove_du_ud_links(El_ref& def, El_ref& use)
{
   if (!is_interesting_ref(use)) return ;
   List<El_ref>* rd_list = get_ud_chain(use) ;
   rd_list->remove(def) ;
   List<El_ref>* ru_list = get_du_chain(def) ;
   ru_list->remove(use) ;
}

bool Reaching_defs_info::is_interesting_ref(El_ref& ref)
{
   Operand& oper = ref.get_operand() ;
   if (is_interesting(oper)) {
      return true ;
   }
   else return false ;
}

///////////////////////////////////////////////////////////////////////////////

Reaching_defs_info_attrib::Reaching_defs_info_attrib (void)
{
}

Reaching_defs_info_attrib::~Reaching_defs_info_attrib (void)
{
}

Base_attribute* Reaching_defs_info_attrib::clone_attribute (void)
{
   El_punt("Cannot clone Reaching_defs_info.");
   return NULL;
  //  Reaching_defs_info_attrib* tmp_ptr = new Reaching_defs_info_attrib ();
  //  tmp_ptr->info = new Reaching_defs_info (*info);
  //  return (tmp_ptr);
}

///////////////////////////////////////////////////////////////////////////////
//
// Reaching definitions info attribute:
//
Reaching_defs_info* get_reaching_defs_info (Region* regn)
{
  Reaching_defs_info_attrib* tmp_ptr = (Reaching_defs_info_attrib*)
    (regn->attributes->get_attribute (REACHING_DEFS));

  if (tmp_ptr == NULL)
    return (NULL);

  return (tmp_ptr->info);
}

void set_reaching_defs_info (Region* regn, Reaching_defs_info* rd)
{
   Reaching_defs_info_attrib* tmp_ptr = (Reaching_defs_info_attrib*)
      (regn->attributes->get_attribute (REACHING_DEFS));
   
   if (tmp_ptr == NULL) {
      tmp_ptr = new Reaching_defs_info_attrib;
      tmp_ptr->info = NULL;
      regn->attributes->insert_attribute (REACHING_DEFS, tmp_ptr);
   }
   
   tmp_ptr->info = rd;
}

void remove_reaching_defs_info (Region* regn)
{
   // fix small leak KF 4-2003   
   Reaching_defs_info_attrib* tmp_ptr = (Reaching_defs_info_attrib*)
      (regn->attributes->get_attribute(REACHING_DEFS));
   if (tmp_ptr && tmp_ptr->info) {
      delete tmp_ptr->info;
      tmp_ptr->info = NULL;
   }

   regn->attributes->remove_attribute (REACHING_DEFS);
   List<Operand>* tmp_list ;
   for(Region_entry_ops rent(regn) ; rent != 0 ; rent++) {
      Op* cur_op = *rent ;
      tmp_list = (List<Operand>*) get_generic_attribute (cur_op, "entry_pseudo_defs") ;
      if (tmp_list) {
         delete tmp_list ;
         remove_generic_attribute (cur_op, "entry_pseudo_defs") ;
      }
   }
   for(Region_exit_ops rex(regn) ; rex != 0 ; rex++) {
      Op* cur_op = *rex ;
      tmp_list = (List<Operand>*) get_generic_attribute (cur_op, "taken_exit_pseudo_uses") ;
      if (tmp_list) {
         delete tmp_list ;
         remove_generic_attribute (cur_op, "taken_exit_pseudo_uses") ;
      }
      tmp_list = (List<Operand>*) get_generic_attribute (cur_op, "fallthrough_exit_pseudo_uses") ;
      if (tmp_list) {
         delete tmp_list ;
         remove_generic_attribute (cur_op, "fallthrough_exit_pseudo_uses") ;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void El_do_reaching_defs (Region* rgn)
{
   El_do_reaching_defs(rgn, ANALYZE_ALL) ;
}


void El_do_reaching_defs (Region* rgn, unsigned int flag)
{
  if (El_do_run_time)
    El_clock2.start();

  if (dbg (rdef, 1))
    cdbg << "*\tInvoking Reaching-Defs solver:" << endl;

  // Reaching defs is done by using hash_maps, and unfortunately,
  // macros can be == but not hash to the same bucket. To make the
  // reaching defs correct, we need to make all macros that are ==
  // have a canonical name.
  make_macros_canonical(rgn);

  remove_reaching_defs_info(rgn) ;
  // solve for the reaching defs:
  Reaching_defs_solver rdef_solver_object (rgn, flag);
  rdef_solver_object.solve_reaching_defs ();

  if (dbg (rdef, 3)) {
    for (int i=0; i < (int)rdef_solver_object.rd_info.size(); i++) {
      cdbg << "*\t\t\trd_in[" << i << "]="
	   << rdef_solver_object.rd_info[i].rd_in << endl;
      cdbg << "*\t\t\trd_out[" << i << "]="
	   << rdef_solver_object.rd_info[i].rd_out << endl;
    }
  }

  // construct the chains:
  rdef_solver_object.construct_uddu_chains ();

  // clone the chains into the info object, set info as region attrib:
  Reaching_defs_info* rdef_info_object = new Reaching_defs_info (flag);
  rdef_solver_object.clone_uddu_chains (rdef_info_object);
  set_reaching_defs_info (rgn, rdef_info_object);

  if (dbg (rdef, 1))
    cdbg << "*\tReaching-Defs solver done:" << endl;

  if (El_do_run_time) {
    El_clock2.stop();
    El_stopwatch_print_time("El_do_reaching_defs", El_clock2.last_elapsed());
  }
}
///////////////////////////////////////////////////////////////////////////////
