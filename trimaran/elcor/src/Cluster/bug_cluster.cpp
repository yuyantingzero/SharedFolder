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
//      File:           bug_cluster.cpp
//      Author:         Kevin Fan
//      Created:        July 2001
//      Description:    Bottom-Up Greedy cluster assignment
//
/////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "list.h"
#include "hash_map.h"
#include "slist.h"
#include "bug_cluster.h"
#include "cluster.h"
#include "dep_len.h"
#include "edge_drawing.h"
#include "pred_analysis.h"
#include "flow_analysis_solver.h"
#include "intf.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "hw_cluster.h"

Hash_map<Opcode, Op*> BUG_cluster::pseudo_moves(hash_opcode, HASH_SIZE_0);

BUG_cluster::BUG_cluster(Cluster_mgr* clust_mgr)
  : Cluster_algorithm(clust_mgr),
    estart_map(hash_op_ptr, HASH_SIZE_1),
    cycle_map(hash_op_ptr, HASH_SIZE_1),
    debug_depth(0)
{
  if (!is_init) {
    static_init();
    is_init = true;
  }
}

BUG_cluster::~BUG_cluster()
{}

void BUG_cluster::static_init()
{
  if(!MDES_unsupported(el_opcode_to_string_map.value(MOVEGG)))
    pseudo_moves.bind(MOVEGG, new Op(MOVEGG));
  if(!MDES_unsupported(el_opcode_to_string_map.value(MOVEF_S)))
    pseudo_moves.bind(MOVEF_S, new Op(MOVEF_S));
  if(!MDES_unsupported(el_opcode_to_string_map.value(MOVEF_D)))
    pseudo_moves.bind(MOVEF_D, new Op(MOVEF_D));
  if(!MDES_unsupported(el_opcode_to_string_map.value(MOVEPP)))
    pseudo_moves.bind(MOVEPP, new Op(MOVEPP));
  if(!MDES_unsupported(el_opcode_to_string_map.value(MOVEBB)))
    pseudo_moves.bind(MOVEBB, new Op(MOVEBB));
  if(!MDES_unsupported(el_opcode_to_string_map.value(MOVEGC)))
    pseudo_moves.bind(MOVEGC, new Op(MOVEGC));
  if(!MDES_unsupported(el_opcode_to_string_map.value(MOVECG)))
    pseudo_moves.bind(MOVECG, new Op(MOVECG));
  if(!MDES_unsupported(el_opcode_to_string_map.value(VMOVEGG)))
    pseudo_moves.bind(VMOVEGG, new Op(VMOVEGG));
  if(!MDES_unsupported(el_opcode_to_string_map.value(VMOVEFF_S)))
    pseudo_moves.bind(VMOVEFF_S, new Op(VMOVEFF_S));
  if(!MDES_unsupported(el_opcode_to_string_map.value(VMOVEFF_D)))
    pseudo_moves.bind(VMOVEFF_D, new Op(VMOVEFF_D));
  if(!MDES_unsupported(el_opcode_to_string_map.value(VMOVEMM)))
    pseudo_moves.bind(VMOVEMM, new Op(VMOVEMM));
}

void BUG_cluster::build_estart_map(Compound_region* rgn)
{
   Hash_map<Op*, DepLen_OpInfo*> dep_info(hash_op_ptr, HASH_SIZE_1);
   estart_map.clear();

   DepLen::get_estart(rgn, dep_info);
   
   // copy info into estart_map
   for (Hash_map_iterator<Op*, DepLen_OpInfo*> hmi(dep_info);
	hmi != 0; hmi++) {
      Pair<Op*, DepLen_OpInfo*> pair = *hmi;
      estart_map.bind(pair.first, pair.second->m_Estart);
   }

   // cleanup
   DepLen::clear_opinfo_map(rgn, dep_info);
}

void BUG_cluster::do_block_setup_and_clustering(Compound_region* rgn)
{
   create_local_analysis_info_for_all_hbs_bbs(rgn);
   el_flow_compute_liveness(rgn, ANALYZE_ALLREG);
   insert_region_scalar_edges(rgn);
   build_estart_map(rgn);

   /* 
      Count up number of ops and allocate RU_map accordingly
   */
   int seq_length, no_ops = 0;

   for(Region_ops_C0_order iter(rgn); iter != 0; ++iter)
	no_ops++;
   seq_length = no_ops * BUG_MAX_CYCLES;

   RU_alloc_map(seq_length);
   RU_init_map(false, seq_length);

   cmgr->pre_assign_map.clear();
   
   do_block_bug_clustering(rgn);
   cmgr->do_block_operand_assignment(rgn);
   //cmgr->do_block_smarter_operand_assignment(rgn);

   RU_delete_map();
   
   delete_region_edges(rgn);
}

void BUG_cluster::do_block_bug_clustering(Compound_region* reg)
{
   List<int> dummy;
   Slist<Stuple<int, Op*> > sorted_op_list;
   
   if (dbg(clst, 3))
      cdbg << "* Printing prebound operands in region: " << reg->id() << endl;

   // sort ops by estart
   for (Region_all_ops r(reg); r != 0; r++) {
      Op* cur_op = *r;
      if (!estart_map.is_bound(cur_op))
	 El_punt("bug: no estart found for op %d", cur_op->id());
      Stuple<int, Op*> st(-estart_map.value(cur_op), cur_op);
      sorted_op_list.add(st);

      if (dbg(clst, 3)) {
	 for (Op_explicit_operands opri(cur_op); opri!=0; opri++) {
	    Operand& opr = *opri;
	    if (opr.is_reg() && cmgr->operand_map.is_bound(opr.vr_num()))
	       cdbg << opr << " was bound on cluster " << cmgr->operand_map.value(opr.vr_num()) << endl;
	    else if (opr.is_macro_reg() && cmgr->macro_vec[(int)opr.name()].get_current_home() != -1) {
	      cdbg << opr << " was bound on cluster " << cmgr->macro_vec[(int)opr.name()].get_current_home() << endl;
            }
	 }
      }
   }

   for (Slist_iterator<Stuple<int, Op*> > sli(sorted_op_list); sli != 0; sli++) {
      Op* op = (*sli).second;
      if (!cmgr->op_map.is_bound(op))
	 assign(op, dummy, NULL);
   }

   if (dbg(clst, 2)) {
      // get last start time
      int max_start = 0;
      for (Region_all_ops o(reg); o != 0; o++) {
	 if (cycle_map.is_bound(*o)) {
	    int start = cycle_map.value(*o);
	    if (start > max_start)
	       max_start = start;
	 }
      }
      cdbg << "latest start cycle: " << max_start << endl;
   }
}

void BUG_cluster::do_postpass_operand_check()
{
   for (Region_all_ops opi(cmgr->m_proc); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;
      if (!cmgr->op_map.is_bound(cur_op))
	 El_punt("bug operand assignment: unbound op %d", cur_op->id());
      
      int clstr = cmgr->op_map.value(cur_op);

      // bound all unbound source operands to same cluster as cur_op
      // and give a warning.
      for (Op_explicit_inputs ii(cur_op); ii != 0; ii++) {
	 Operand& src = *ii;
	 if (src.is_reg() && !cmgr->operand_map.is_bound(src.vr_num())) {
	    cmgr->operand_map.bind(src.vr_num(), clstr);
	    El_warn("bug postpass operand check: use found with no def in op %d", cur_op->id());
	 }
	 else if (src.is_macro_reg()) {
	    int mn = (int)src.name();
	    El_ref cur_ref(cur_op, &src, ii.ref_type());

	    if(!cmgr->macro_vec[mn].is_bound(cur_ref)) {
	       if (src.is_hardwired())
		  El_punt("bug postpass operand check: unbound hardwired macro found in op %d", cur_op->id());

	       El_punt("bug postpass operand check: a macro source for op %d was bound in the postpass check", cur_op->id());
	    }
	 }
      }
   }
}

// main recursive BUG function
void BUG_cluster::assign(Op* op, List<int>& dests, Op* dest_op)
{
   if (dbg(clst, 4))
      cdbg << "=> assign op " << op->id() << " ["<< ++debug_depth << "]" << endl;

   if (is_pseudo(op)) {
      if (dbg(clst, 4))
	 cdbg << "<= assign op " << op->id() << " [" << debug_depth-- << "]" << endl;
      return;
   }

   if (cmgr->op_map.is_bound(op)) { // if is already assigned
      if (dbg(clst, 4))
	 cdbg << "<= assign op " << op->id() << " [" << debug_depth-- << "]" << endl;
      return;
   }

   Slist<Stuple<int, Op*> > src_op_list;
   List<int> est_fus, est_cycles;
   List<BUG_move_info> src_moves;
   List<List<BUG_move_info > > src_move_list;
   int op_cid, op_cycle;
   char op_iopat[BUG_MAX_IOPAT_LENGTH];
   char* opc;
   char* opc_dummy; int pri_dummy;
 
   // sort predecessor ops by estart
   for (Op_inedges_rel edgei(op, NON_CONTROL0_INEDGES); edgei != 0; edgei++) {
      Op* cur_src_op = (*edgei)->src();
      Stuple<int, Op*> st(-estart_map.value(cur_src_op), cur_src_op);
      src_op_list.add(st);
   }

   for (Slist_iterator<Stuple<int, Op*> > sli(src_op_list); sli != 0; sli++) {
      Op* src_op = (*sli).second;
      
      // check for pseudo or already-assigned ops here, to save on unnecessary
      // calls to likely_fus
      if (is_pseudo(src_op) || cmgr->op_map.is_bound(src_op))
	 continue;

      likely_fus(op, dests, dest_op, est_fus, est_cycles, src_move_list);
      assign(src_op, est_fus, op);
   }

   likely_fus(op, dests, dest_op, est_fus, est_cycles, src_move_list);

   // take the first one with the lowest start cycle, because dest moves aren't
   // always necessary and earlier start cycle means earlier completion cycle
   // if the dest move gets optimized out.
   int min_start = ELCOR_MAX_INT;
   int cur_start;
   while (!est_cycles.is_empty()) {
      cur_start = est_cycles.pop();
      if (cur_start < min_start) {
	 min_start = op_cycle = cur_start;
	 op_cid = est_fus.pop();
	 src_moves = src_move_list.pop();
      }
      else {
	 est_fus.pop();
	 src_move_list.pop();
      }
   }

   cmgr->op_map.bind(op, op_cid);
   cycle_map.bind(op, op_cycle);

   // remember which cluster a predicate register is assigned to.  (or SVMOVE dest)
   // all the cmpps write to that predicate will be assigned to the same cluster later.
   // same for all svmoves that write to the same dest.
   if (is_cmpp(op) || is_svmove(op)) {
      for (Op_explicit_dests desti(op); desti!=0; desti++) {
         Operand &dest = *desti;
         if (dest.is_reg())
            cmgr->pre_assign_map.bind(dest, op_cid);
      }
   }

   // RU_place moves
   if (dbg(clst, 4)) {
      cdbg << "assign: source moves needed:";
      for (List_iterator<BUG_move_info> a(src_moves); a!=0; a++) {
        cdbg << " {";
        List_iterator<BUG_single_move> b(*a);
        while (b != 0) {
          cdbg << " " << (*b).first << " " << (*b).second << " -> " << (*b).third 
               << " " << (*b).fourth;
          b++;
          if (b != 0)
            cdbg << " ==> ";
        }
        cdbg << "}";
      }
      cdbg << endl;
   }
   
   for (List_iterator<BUG_move_info > movi(src_moves);
	movi != 0; movi++) {
      schedule_move(*movi);
   }

   // opc = (char*)el_opcode_to_string_map.value(op->opcode());
   opc = get_mdes_opcode_string(op);
   
   build_io(op_iopat, op, op_cid);
   RU_init_iterator(opc, op, op_iopat, op_cycle);
   if (!RU_get_next_nonconfl_alt(&opc_dummy, &pri_dummy)) {
      //      RU_print_map(stddbg);
      El_punt("bug assign: unexpected - trying to assign op %d to cluster %d - %s not available at cycle %d", op->id(), op_cid, op_iopat, op_cycle);
   }
   RU_place();

   if (dbg(clst, 3))
      cdbg << "assign: op " << op->id() << " assigned to clstr " << op_cid << " at cycle " << op_cycle <<endl;

// don't print RU_map - its buffer isn't big enough to print out all the resources
//    if (dbg(clst, 8))
//       RU_print_map(stddbg);

   if (dbg(clst, 4))
      cdbg << "<= assign op " << op->id() << " [" << debug_depth-- << "]" << endl;
}

// arrival_cycle = when result of src_op arrives on clstr, and dest_op can
// start using it
int BUG_cluster::arrival_cycle(Op* op, Edge* edge, int clstr, 
			       List<BUG_move_info>& src_moves)
{
   Op* src_op = edge->src();
   Op* dest_op = edge->dest();        // may not be equal to op

   // note: the is_pseudo check shouldn't be necessary, but 
   // spurious D_MERGEs are getting into the op_map and i don't know where
   // they're coming from ... KF 5-2003
   // found out where they're probably coming from (corruption caused by move
   // opti 3). keep the check around, won't hurt anyone. -KF 11/2004
   if (!is_pseudo(src_op) && cmgr->op_map.is_bound(src_op)) {
      int avail_cyc = available_cycle(src_op, dest_op);

      // check move resources if this is an interesting edge
      if (edge->is_reg_flow() &&
	  dest_op == op) { // ignore related ops when doing move bandwidth

	 Port_num src_pnum = edge->src_port();
	 Operand& oprnd1 = src_op->dest(src_pnum);
	 int src_clstr = cmgr->op_map.value(src_op);
	 
	 Port_num dst_pnum = edge->dest_port();
	 Operand& oprnd2 = dest_op->src(dst_pnum);
	 if (oprnd1 != oprnd2) // this means one of the operands is
	    // an implicit operand. don't know how to handle this.
	    // just return for now. should work anyway because implicit
	    // operands are usu. macros and macro move b/w is handled
	    // in start_cycle.
	    return avail_cyc;

	 // if this operand is already bound, let start_cycle handle its move
	 if (cmgr->operand_map.is_bound(oprnd1.vr_num()))
	    return avail_cyc;
         
         int dist = distance(src_clstr, clstr, oprnd1.file_type());
	 if (dist > 0) {
	    BUG_move_info bmi = get_move_info(oprnd1, src_clstr, clstr, avail_cyc, true);
	    int first_move_cycle = find_earliest_move_cycle(bmi);

	    if (dbg(clst, 6) && (avail_cyc != first_move_cycle))
	       cdbg << "    arrival_cycle: move b/w for op " << op->id()
		    << ".. cycle " << avail_cyc << " becomes "
		    << first_move_cycle + dist << endl;

	    src_moves.add_tail(bmi);
	    if (dbg(clst, 7))
	       cdbg << "    arrival_cycle: trying clstr " << clstr
                    << ": adding move because of edge from op " << src_op->id()
                    << src_op->opcode() << " to op " << dest_op->id()
		    << dest_op->opcode() << endl;

	    return first_move_cycle + dist;
	 }
      }

      return avail_cyc;
   }

   // predecessor not assigned
   else {
      if (!estart_map.is_bound(src_op))
         El_punt("bug arrival_cycle: no estart found for op %d", src_op->id());

      int depth = estart_map.value(src_op) + delay(src_op, dest_op);
      int min_distance;

      // find min distance from feasible locations if this is an interesting edge
      if (edge->is_reg_flow() && dest_op == op) {  // ignore related ops

	 min_distance = ELCOR_MAX_INT;
	 List<int>* feas_locations;

	 cmgr->collect_valid_clusters_op(src_op, feas_locations);

	 if (feas_locations->size() == 0) {
	    El_punt("bug arrival_cycle: no feasible locations for op %d",
		    src_op->id());
	 }
	 else {
	    for (List_iterator<int> li(*feas_locations); li != 0; li++) {
	       int dist = distance(*li, clstr, op->src(edge->dest_port()).file_type());
	       if (dist < min_distance)
		  min_distance = dist;
	    }
	 }
      }
      else
	 min_distance = 0;

      return depth + min_distance;
   }
}

// available cycle - when the result of src_op is available
int BUG_cluster::available_cycle(Op* src_op, Op* op)
{
   if (cycle_map.is_bound(src_op))
      return cycle_map.value(src_op) + delay(src_op, op);

   El_punt("bug available_cycle: not valid for unassigned op %d", src_op->id());
   return 0;
}

// completion cycle - the cycle when op finishes executing on clstr and writes
// its results to dests
int BUG_cluster::completion_cycle(Op* op, int clstr, List<int>& dests,
				  Op* dest_op, 
				  List<BUG_move_info>& src_moves,
				  int* start_cyc)
{
   if (dbg(clst, 6)) {
      cdbg << "  completion_cycle: finding CC for op " << op->id()
           << " on clst " << clstr << " with dests";
      for (List_iterator<int> a(dests); a!=0; a++) cdbg << " " << *a;
      cdbg << endl;
   }

   int temp_start_cyc, prebound_incr = 0, compl_cyc;
   //int temp_start_cyc, finish_cyc, compl_cyc, move_done_cycle;
   int min_distance = ELCOR_MAX_INT;

   temp_start_cyc = start_cycle(op, clstr, src_moves);
   if (start_cyc != NULL)
      *start_cyc = temp_start_cyc;

   int finish_cyc = temp_start_cyc + delay(op, dest_op);

   if (dests.size() == 0)
      min_distance = 0;
   else {     // find the earliest a dest move could be scheduled.
     // doesn't take into consideration shared register files
     // .. does now. -KF 10/2004
     for (List_iterator<int> li(dests); li != 0; li++) {
       //cur_dist = (clstr == *li) ? 0 : BUG_MOVE_LATENCY;
       // find the edges between op and dest_op
       int clst_max_dist = 0;
       for (Op_outedges_rel edgei(op, FLOW_EDGES); edgei != 0; edgei++) {
         Edge* e = *edgei;
         if (e->dest() == dest_op) {
           int cur_dist = distance(clstr, *li, op->dest(e->src_port()).file_type());
           if (cur_dist > clst_max_dist)
             clst_max_dist = cur_dist;
         }
       }
       if (clst_max_dist < min_distance)
         min_distance = clst_max_dist;
     }
   }

   // only if no other move is needed ...
   if (min_distance == 0) {
      // add a cycle for differring prebound dest operand
      for (Op_explicit_dests desti(op); desti != 0; desti++) {
	 Operand& opr = *desti;
	 if (opr.is_reg() && cmgr->operand_map.is_bound(opr.vr_num())) {
	    if (clstr != cmgr->operand_map.value(opr.vr_num()))
              prebound_incr = distance(clstr, cmgr->operand_map.value(opr.vr_num()), opr.file_type());
	 }
	 else if (opr.is_macro_reg()) {
           int mn = (int)opr.name();
           Macro_info& mac = cmgr->macro_vec[mn];

           if(mac.is_read_only()) {
             if(!mac.is_potential_home(clstr))
               // just get the first potential home, that's what's done in 
               // operand binding anyway.
               prebound_incr = distance(clstr, mac.get_first_potential_home(), opr.file_type());
           }
           else {
             int home = mac.get_current_home();
             if(home != -1) {
               if (home != clstr)
                 prebound_incr = distance(clstr, home, opr.file_type());
             }
             else
               if(!mac.is_potential_home(clstr))
                 // just get the first potential home, that's what's done in 
                 // operand binding anyway.
                 prebound_incr = distance(clstr, mac.get_first_potential_home(), opr.file_type());
           }
           
	 }
      }
   }

   if (dbg(clst, 6))
      cdbg << "  completion_cycle: SC=" << temp_start_cyc << ", delay="
	   << delay(op, dest_op) << ", min_dist=" << min_distance
	   << ", prebound_dest=" << prebound_incr << endl;

   compl_cyc = finish_cyc + min_distance + prebound_incr;
   return compl_cyc;
}

// delay - latency of op
int BUG_cluster::delay(Op* op, Op* dest_op)
{
   int max_delay = 0;

   if (dest_op == NULL) { // find the max outedge latency
      for (Op_outedges_rel edgei(op, NON_CONTROL0_OUTEDGES); edgei != 0; edgei++) {
	 int del = (*edgei)->latency();
	 if (del > max_delay)
	    max_delay = del;
      }
      return max_delay;
   }

   // find the max latency between the specified ops
   for (Op_outedges_rel edgei(op, NON_CONTROL0_OUTEDGES); edgei != 0; edgei++) {
      if ((*edgei)->dest() == dest_op) {
	 int cur_delay = (*edgei)->latency();
	 if (cur_delay > max_delay)
	    max_delay = cur_delay;
      }
   }

   return max_delay;
}

// distance - cycles to move an operand from src_clstr to dest_clstr.
// note: when moving a CR this function may mis-estimate the distance, since CRs
// are typically moved to/from GPRs.
int BUG_cluster::distance(int src_clstr, int dst_clstr, Reg_file file_type)
{
   int src_rf_id = cmgr->hw_cluster_vec[src_clstr]->get_physical_file_id(file_type);
   int dest_rf_id = cmgr->hw_cluster_vec[dst_clstr]->get_physical_file_id(file_type);
   int dist = cmgr->min_latency[src_rf_id][dest_rf_id];

   return dist;
}

// find first cycle >= 'cycle' that op can execute on clstr
int BUG_cluster::find_earliest_available_cycle(Op* op, int clstr, int cycle)
{
   char io[BUG_MAX_IOPAT_LENGTH];
   char* opc_dummy; int pri_dummy;

   //char* opc = (char*)el_opcode_to_string_map.value(op->opcode());
   char* opc = get_mdes_opcode_string(op);
   
   build_io(io, op, clstr);

   bool found_flag = false;
   while (found_flag == false) {
      RU_init_iterator(opc, op, io, cycle);
      found_flag = RU_get_next_nonconfl_alt(&opc_dummy, &pri_dummy);
      if (found_flag == false)
	 cycle++;
   }
   return cycle;
}

// likely fus - list of clusters with minimal completion time for
// op to execute in and send results to dests
void BUG_cluster::likely_fus(Op* op, List<int>& dests, Op* dest_op,
			     List<int>& estimated_fus, List<int>& estimated_cycles,
			     List<List<BUG_move_info > >& src_move_list)
{

   int cur_clstr, start_cyc, compl_cyc;
   List<int>* feas_locations;
   int min_completion_cycle = ELCOR_MAX_INT;
   
   cmgr->collect_valid_clusters_op(op, feas_locations);

   for (List_iterator<int> li(*feas_locations); li != 0; li++) {
      cur_clstr = *li;
      List<BUG_move_info > temp_moves;

      if (dbg(clst, 5))
	 cdbg << " likely_fus: trying op " << op->id() << " on cluster "
	      << cur_clstr << ".. " << endl;

      compl_cyc = completion_cycle(op, cur_clstr, dests, dest_op, temp_moves, &start_cyc);

      // HACK - want to put clear_all_rotating on same cluster as all_rot_pred macro.
      // it seems to confuse the predicate analysis otherwise

      if (is_clear_all_rotating(op) && cur_clstr != cmgr->macro_vec[ALL_ROT_PRED].get_current_home())
	 compl_cyc += 100;

      if (compl_cyc < min_completion_cycle) {
	 min_completion_cycle = compl_cyc;
	 estimated_fus.clear();
	 estimated_cycles.clear();
	 src_move_list.clear();
	 estimated_fus.add_tail(cur_clstr);
	 estimated_cycles.add_tail(start_cyc);
	 src_move_list.add_tail(temp_moves);
      }
      else if (compl_cyc == min_completion_cycle) {
	 // sort-of suggest higher priority to cluster that requires less moves
	 if (temp_moves.size() < src_move_list.head().size()) {
	    estimated_fus.add_head(cur_clstr);
	    estimated_cycles.add_head(start_cyc);
	    src_move_list.add_head(temp_moves);
	 }
	 else {
	    estimated_fus.add_tail(cur_clstr);
	    estimated_cycles.add_tail(start_cyc);
	    src_move_list.add_tail(temp_moves);
	 }
      }
      if (dbg(clst,5))
	 cdbg << " likely_fus: CC = " << compl_cyc 
	      << " for SC = " << start_cyc
	      << ", " << temp_moves.size() << " source moves." << endl;
   }

   if (dbg(clst, 5)) {
      cdbg << " likely_fus: likely fus for op " << op->id() << " were:";
      for (List_iterator<int> a(estimated_fus); a!=0; a++) cdbg << " " << *a;
      cdbg << endl;
   }
}

// start cycle - earliest cycle op can start executing in clstr, subject to
// operand availability and resource availability
int BUG_cluster::start_cycle(Op* op, int clstr, List<BUG_move_info >& src_moves)
{
   int max_arrival_cycle = 0;
   int arr_cycle;

   for (Op_inedges_rel edgei(op, NON_CONTROL0_INEDGES); edgei != 0; edgei++) {
      arr_cycle = arrival_cycle(op, *edgei, clstr, src_moves);
      if (dbg(clst, 7))
	 cdbg << "   start_cycle: arrival cycle for edge " << **edgei << " was " 
	      << arr_cycle << endl;
      if (arr_cycle > max_arrival_cycle)
	 max_arrival_cycle = arr_cycle;
   }

   for (Op_explicit_inputs oi(op); oi != 0; oi++) {
      Operand& in_opr = *oi;
      if (in_opr.is_reg() && cmgr->operand_map.is_bound(in_opr.vr_num()) &&
	  cmgr->operand_map.value(in_opr.vr_num()) != clstr) {

	 // add a move
        int dist = distance(cmgr->operand_map.value(in_opr.vr_num()), clstr, in_opr.file_type());
        BUG_move_info bmi = get_move_info(in_opr, cmgr->operand_map.value(in_opr.vr_num()),
                                          clstr, 0, true);
        int operand_arr_cycle = find_earliest_move_cycle(bmi) + dist;
        src_moves.add_tail(bmi);

	 // only increment arrival cycle if it's the first cycle
	 //	 if (max_arrival_cycle == 0)
	 //   max_arrival_cycle++;
	 if (operand_arr_cycle > max_arrival_cycle) {
	    if (dbg(clst, 6))
	       cdbg << "** move b/w and prebound source in op " << op->id()
		    << ".. cycle " << max_arrival_cycle << " becomes " 
		    << operand_arr_cycle << endl;
	    max_arrival_cycle = operand_arr_cycle;
	 }

	 if (dbg(clst, 7))
	    cdbg << "   start_cycle: arrival cycle for operand " << in_opr << " was "
		 << operand_arr_cycle << endl;
      }
      else if (in_opr.is_macro_reg() &&
	       !cmgr->macro_vec[(int)in_opr.name()].is_potential_home(clstr)) {

	// add a move if we know where the macro is
        Macro_info& mac = cmgr->macro_vec[in_opr.name()];
        if (mac.get_current_home() != -1) {
          int dist = distance(mac.get_current_home(), clstr, in_opr.file_type());
          BUG_move_info bmi = get_move_info(in_opr, /*cmgr->macro_to_cluster[in_opr.name()],*/
                                            mac.get_current_home(),
                                            clstr, 0, true);

          int macro_arr_cycle = find_earliest_move_cycle(bmi) + dist;
          src_moves.add_tail(bmi);

          if (macro_arr_cycle > max_arrival_cycle) {
            if (dbg(clst, 6))
              cdbg << "** move b/w and prebound macro in op " << op->id()
                   << ".. cycle " << max_arrival_cycle << " becomes " 
                   << macro_arr_cycle << endl;
            max_arrival_cycle = macro_arr_cycle;
          }

          if (dbg(clst, 7))
            cdbg << "   start_cycle: arrival cycle for macro " << in_opr << " was "
                 << macro_arr_cycle << endl;
        }

        // otherwise just add a cycle for a move if it would affect schedule length
        else if (max_arrival_cycle == 0) {
          int dist = distance(mac.get_first_potential_home(), clstr, in_opr.file_type());
          max_arrival_cycle = dist;
          if (dbg(clst, 7))
            cdbg << "   start_cycle: add " << dist << " cycles for macro" << endl;
        }
      }
   }

   if (dbg(clst, 7))
      cdbg << "   start_cycle: latest arrival before resources: " << max_arrival_cycle << endl;

   int start_cyc = find_earliest_available_cycle(op, clstr, max_arrival_cycle);

   if (dbg(clst, 7))
      cdbg << "   start_cycle: start cycle: " << start_cyc << endl;

   return start_cyc;
}

void BUG_cluster::print_estart_map()
{
   for (Hash_map_iterator<Op*, int> hmi(estart_map); hmi != 0; hmi++) {
      cdbg << "op " << (*hmi).first->id() << " estart: "
	   << (*hmi).second << endl;
   }
}

// return a BUG_move_info structure with a list of BUG_single_moves.
// currently only works for source moves (the only kind BUG wants). -KF 10/2004
BUG_move_info BUG_cluster::get_move_info(Operand& operand, int clstr1, int clstr2, int cycle, bool is_source)
{
  if (!is_source)
    El_punt("BUG_cluster::get_move_info: dest moves not supported.");

   BUG_move_info info;

   Reg_file rf_type = operand.file_type();
   int src_idx, dest_idx;

   src_idx = cmgr->hw_cluster_vec[clstr1]->get_physical_file_id(rf_type);
   if (rf_type == CR)
     dest_idx = cmgr->hw_cluster_vec[clstr2]->get_physical_file_id(GPR);
   else
     dest_idx = cmgr->hw_cluster_vec[clstr2]->get_physical_file_id(rf_type);

   Opcode opc = cmgr->get_icmove_opcode(operand, is_source);
     
   List<int>* move_list = cmgr->min_path[src_idx][dest_idx];
   if (move_list == NULL || move_list->size() == 0) {
     // should be more intelligent here than punting, but for now an empty move_list will
     // break other parts anyway
     El_punt("Move of vr %d not possible from cluster %d to cluster %d",
             operand.vr_num(), clstr1, clstr2);
   }
   eString src(cmgr->hw_cluster_vec[clstr1]->get_physical_file(rf_type));
   int prev_cid = clstr1;
   int prev_idx = src_idx;
   bool first_move = true;

   for (List_iterator<int> li(*move_list); li!=0; li++) {
     int cur_dest_cid = *li;
     int cur_dest_idx = cmgr->hw_cluster_vec[cur_dest_cid]->get_physical_file_id(rf_type);
     eString dest(cmgr->hw_cluster_vec[cur_dest_cid]->get_physical_file(rf_type));
     if (rf_type == CR) {
       dest = cmgr->hw_cluster_vec[cur_dest_cid]->get_physical_file(GPR);
       cur_dest_idx = cmgr->hw_cluster_vec[cur_dest_cid]->get_physical_file_id(GPR);
       if (!first_move)
         opc = MOVEGG;
     }
     BUG_single_move move(opc, src, dest, cycle);
     info.add_tail(move);
     
     int latency = cmgr->min_latency[prev_idx][cur_dest_idx];
     cycle += latency;

     src = dest;
     prev_cid = cur_dest_cid;
     prev_idx = cur_dest_idx;
     if (first_move) first_move = false;
   }

   return info;
}

// place icmove in RU_map
void BUG_cluster::schedule_move(BUG_move_info move_info)
{
   find_earliest_move_cycle(move_info, true);
}

// find first cycle that move can execute, given resource constraints.
// updates the 'fourth' (cycle) field of move_info.
// may be useful to return last_move_cycle as well, for multiple-step moves...
int BUG_cluster::find_earliest_move_cycle(BUG_move_info& move_info, bool place)
{
   if (!El_bug_model_move_resources)
      return move_info.head().fourth;
  
   char move_iopat[BUG_MAX_IOPAT_LENGTH];
   char* opc_dummy; int pri_dummy;

   for (List_iterator<BUG_single_move> li(move_info); li!=0; li++) {
     BUG_single_move move = *li;

     // build io pattern
     char* move_opc = (char*)el_opcode_to_string_map.value(move.first);
     char* src = (char*)move.second;
     char* dest = (char*)move.third;
     strcpy(move_iopat, vregfile_to_mdesname(PR));
     strcat(move_iopat, "? ");
     strcat(move_iopat, src);
     strcat(move_iopat, ":");
     strcat(move_iopat, dest);
      
     Op* tmp_move_op = pseudo_moves.value(move.first);

     int cycle = move.fourth;
     bool found_flag = false;
     do {
       RU_init_iterator(move_opc, tmp_move_op, move_iopat, cycle);
       found_flag = RU_get_next_nonconfl_alt(&opc_dummy, &pri_dummy);
       if (!found_flag)
	 cycle++;
     } while (!found_flag);
   
     if (place)
       RU_place();

     move.fourth = cycle;
   }

   return move_info.head().fourth;
}

