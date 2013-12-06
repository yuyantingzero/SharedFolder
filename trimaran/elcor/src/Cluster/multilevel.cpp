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
//      File:           multilevel.cpp
//      Author:         Mike Chu
//      Created:        Sept 2002
//      Description:    Partial Component cluster assignment
//
/////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "list.h"
#include "hash_map.h"
#include "multilevel.h"
#include "slist.h"
#include "cluster.h"
#include "dep_len.h"
#include "edge_drawing.h"
#include "pred_analysis.h"
#include "flow_analysis_solver.h"
#include "intf.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "hw_cluster.h"
#include "el_bb_tools.h"

double MultiLevel::move_weight;

MultiLevel::MultiLevel(Cluster_mgr* clust_mgr)
  : Cluster_algorithm(clust_mgr),
    clusters(clust_mgr->hw_cluster_vec.size()),
    opcode_map(hash_opcode, HASH_SIZE_1),
    total_node_weights((clust_mgr->hw_cluster_vec.size())),
    node_weights(hash_op_ptr, HASH_SIZE_1),
    edge_weights(hash_edge_ptr, HASH_SIZE_1),
    slack_map(hash_op_ptr, HASH_SIZE_1),
    dep_info(hash_op_ptr, HASH_SIZE_1)
{
  RU_alloc_map(XXX_MAX_RU_LENGTH);
  if (!is_init) {
    static_init();
    is_init = true;
  }
}

MultiLevel::~MultiLevel()
{ 
  RU_delete_map();
}

void MultiLevel::static_init()
{
  find_global_move_weight();
  cdbg << "global move weight was " << move_weight << endl;
}

// ****************************************

void MultiLevel::find_global_move_weight()
{
  char move_iopat[XXX_MAX_IOPAT];
  char* opc_dummy; int pri_dummy;
  bool found = false;
  int count = 0;

  // build io pattern
  eString src, dest;
  // find the first two clusters to support ints
  int cluster = 0;
  while ((src = cmgr->hw_cluster_vec[cluster]->get_physical_file(GPR)) == "") {
    cluster++;
    if (cluster >= (int)cmgr->hw_cluster_vec.size() - 1) {
      El_punt("MultiLevel::find_global_move_weight: can't find two clusters supporting GPR.");
    }
  }
  cluster++;
  while ((dest = cmgr->hw_cluster_vec[cluster]->get_physical_file(GPR)) == "") {
    cluster++;
    if (cluster >= (int)cmgr->hw_cluster_vec.size()) {
      El_punt("MultiLevel::find_global_move_weight: can't find two clusters supporting GPR.");
    }
  }

  strcpy(move_iopat, vregfile_to_mdesname(PR));
  strcat(move_iopat, "? ");
  strcat(move_iopat, src);
  strcat(move_iopat, ":");
  strcat(move_iopat, dest);
  cdbg << "the iopat i'm using for finding move weight is " << move_iopat << endl;
  RU_init_map(false, XXX_MAX_RU_LENGTH);
  Op* move_op = new Op(MOVEGG);

  do {
    RU_init_iterator("MOVEGG", move_op, move_iopat, 0);
    found = RU_get_next_nonconfl_alt(&opc_dummy, &pri_dummy);
    if (found) {
      RU_place();
      count++;
    }
  } while (found);
  
  move_weight = 1. / (double) count;
}

void MultiLevel::find_block_node_weights(Compound_region* rgn)
{
   int num_clusters = Cluster_mgr::hw_cluster_vec.size();
   Vector<double> weights(num_clusters);

   for (Region_all_ops opi(rgn); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;

      for (int i = 0; i < num_clusters; i++) {
	 weights[i] = compute_node_weight(cur_op, i);
      }

      node_weights.bind(cur_op, weights);
   }
}

double MultiLevel::compute_node_weight(Op* op, int clstr)
{
   int count = 0;
   char* opc = get_mdes_opcode_string(op);
   char iopat[XXX_MAX_IOPAT];
   bool found_flag;
   char* opc_dummy; int pri_dummy;
   
   build_io(iopat, op, clstr);

   //cdbg << opc << " [" << iopat << "]" << endl;

   RU_init_map(false, XXX_MAX_RU_LENGTH);

   do {
      // try to schedule at time 0
      RU_init_iterator(opc, op, iopat, 0);
      found_flag = RU_get_next_nonconfl_alt(&opc_dummy, &pri_dummy);
      if (found_flag) {
	 RU_place();
	 count++;
	 // blah
	 if (count >= 100) found_flag = false;
      }
   } while (found_flag);
   
   if (count == 0)
      return XXX_INF;

   return 1. / (double) count;
}

void MultiLevel::setup_block(Compound_region* block)
{
   node_weights.clear();
   insert_block_pseudo_defs_and_uses(block);
   find_block_node_weights(block);

   // find edge weights, total node weight Vector, and max estart for this block
   dep_info.clear();
   DepLen::get_estart(block, dep_info);
   DepLen::get_lstart(block, dep_info);
   
   find_block_max_estart(block);

   get_slacks_and_ranges(block);

   fix_defs_and_uses(block);
   find_block_edge_weights(block);

   // total node weight Vector
   find_block_total_node_weights(block);

   cdbg << "total node weight vec: ";
   for (unsigned k=0; k<total_node_weights.size();k++)
      cdbg << total_node_weights[k] << " ";
   cdbg << endl;
}

// you must have run the DepLen functions, find_block_max_estart,
// and insert_block_pseudo_defs_and_uses before calling this function.
void MultiLevel::get_slacks_and_ranges(Compound_region* block)
{
  per_cycle_ops.resize(max_estart+1);

  for (Region_all_ops opi(block); opi!=0; opi++) {
    Op* op = *opi;
    if (is_pseudo(op)) {
      if (op->opcode() == DEF) {
	slack_map.bind(op, 1);
	per_cycle_ops[0].add_tail(op);
      } else if (op->opcode() == USE) {
	slack_map.bind(op, 1);
	per_cycle_ops[max_estart].add_tail(op);
      }
    } else {
      int estart = dep_info.value(op)->m_Estart;
      int lstart = dep_info.value(op)->min_lstart();
      if (lstart > max_estart) {
	cdbg << "  warning.. op " << op->id() << " has an lstart greater than max_estart." << endl;
	lstart = max_estart;
      }
      int slack = lstart - estart + 1;
      slack_map.bind(op, slack);

      for (int i=estart; i<=lstart; i++) {
	per_cycle_ops[i].add_tail(op);
      }
    }
  }

#if 0
  cdbg << "slack_map: " << endl;
  for (Hash_map_iterator<Op*,int> x(slack_map); x!=0; x++) {
    cdbg << "op " << (*x).first->id() << " slack=" << (*x).second << endl;
  }
  cdbg << "per_cycle_ops: " << endl;
  for (int j=0; j <= max_estart; j++) {
    cdbg << "ops at " << j << ": ";
    for (List_iterator<Op*> li(per_cycle_ops[j]); li!=0; li++) {
      cdbg << (*li)->id() << " ";
    }
    cdbg << endl;
  }
#endif
}

void MultiLevel::insert_block_pseudo_defs_and_uses(Compound_region* block)
{
   int num_clusters = Cluster_mgr::hw_cluster_vec.size();
   Vector<double> wts(num_clusters);
   Hash_map<Operand, Op*> def_opds(hash_operand, HASH_SIZE_0),
     use_opds(hash_operand, HASH_SIZE_0);
	 
   for (Region_all_ops opi(block); opi != 0; opi++) {
      Op* op = *opi;
      if (is_pseudo(op)) continue;

      for (Op_explicit_inputs opdi(op); opdi != 0; opdi++) {
	 Operand opd = *opdi;
	 int cid = -1;

	 if (opd.is_reg() && cmgr->operand_map.is_bound(opd.vr_num()))
	    cid = cmgr->operand_map.value(opd.vr_num());
	 else if (opd.is_macro_reg() && cmgr->macro_vec[(int)opd.name()].get_current_home() != -1)
	    cid = cmgr->macro_vec[(int)opd.name()].get_current_home();

	 if (cid != -1) {
	    Op* def_op;
	   
	    if (!def_opds.is_bound(opd)) { // create DEF for this operand
	       def_op = new Op(DEF);
	       def_op->set_dest(DEST1, opd);
	       El_insert_op_after_merge(block, def_op);
	       def_opds.bind(opd, def_op);
	       cdbg << "insert DEF " << def_op->id() << " for prebound opd "<<opd<< " in op " << op->id() <<endl;

	       // give it a node weight Vector
	       for (int i=0; i<num_clusters; i++) {
		 if (i == cid)
		   wts[i] = 0;
		 else
		   wts[i] = XXX_INF;
	       }
	       node_weights.bind(def_op, wts);

	    } else {
	       def_op = def_opds.value(opd);
	       cdbg << "reuse DEF " << def_op->id() << " for prebound opd in op "<<op->id()<<endl;
	    }

	    Edge* def_edge = new Reg_flow();
	    connect_ops(def_edge, def_op, DEST1, op, opdi.get_port_num(), 0, 0);
	    def_edge->set_latency(0);
	 }
      }

      for (Op_explicit_dests opdii(op); opdii != 0; opdii++) {
	 Operand opd = *opdii;
	 int cid = -1;

	 if (opd.is_reg() && cmgr->operand_map.is_bound(opd.vr_num()))
	    cid = cmgr->operand_map.value(opd.vr_num());
	 else if (opd.is_macro_reg() && cmgr->macro_vec[(int)opd.name()].get_current_home() != -1)
	    cid = cmgr->macro_vec[(int)opd.name()].get_current_home();

	 if (cid != -1) {
	    Op* use_op;

	    if (!use_opds.is_bound(opd)) { // create USE for this operand
	       use_op = new Op(USE);
	       use_op->set_src(SRC1, opd);
	       Op* last_op = get_last_region_op_from_subregions(block);
	       El_insert_op_before(use_op, last_op);
	       use_opds.bind(opd, use_op);
	       cdbg << "insert USE " << use_op->id()<< " for prebound opd "<<opd<<" in op "<<op->id() <<endl;

	       // give it a node weight Vector
	       for (int i=0; i<num_clusters; i++) {
		 if (i == cid)
		   wts[i] = 0;
		 else
		   wts[i] = XXX_INF;
	       }
	       node_weights.bind(use_op, wts);

	    } else {
	       use_op = use_opds.value(opd);
	       cdbg << "reuse USE " << use_op->id() << " for prebound opd in op " << op->id() << endl;
	    }

	    Edge* use_edge = new Reg_flow();
	    connect_ops(use_edge, op, opdii.get_port_num(), use_op, SRC1, 0, 0);
	    use_edge->set_latency(0);
         }
      }
   }
}

void MultiLevel::fix_defs_and_uses(Compound_region* block)
{
  int max_lstart = 0;
  for (Region_all_ops opi(block); opi!=0; opi++) {
    Op* op = *opi;
    if (!is_pseudo(op)) {
      int lstart = dep_info.value(op)->min_lstart();
      //cdbg << "fix_defs_and_uses: op " << op->id() << " " << op->opcode()
      //   << ": es="<<dep_info.value(op)->m_Estart << " ls=" << lstart << endl;
      max_lstart = lstart > max_lstart ? lstart : max_lstart;
    }
  }

  for (Region_all_ops opi2(block); opi2!=0; opi2++) {
    Op* op = *opi2;
    if (op->opcode() == DEF)
      dep_info.value(op)->m_Estart = 0;
    else if (op->opcode() == USE)
      dep_info.value(op)->m_Lstart[0] = max_lstart;
  }
}

void MultiLevel::find_block_edge_weights(Compound_region* block)
{
   edge_weights.clear();
   
   // give critical path edges a high weight.
   // also, if doing critical-path weighting, give non-critical edges low wt.
   // (otherwise leave non-critical edges unbound.)
   for (Region_all_internal_edges edgei(block, FLOW_EDGES); edgei != 0; edgei++) {
     Edge* edge = *edgei;
     if ((is_pseudo(edge->src()) && edge->src()->opcode() != DEF) || 
	 (is_pseudo(edge->dest()) && edge->dest()->opcode() != USE)) 
       continue;

     int src_estart = dep_info.value(edge->src())->m_Estart;
     int dest_lstart = dep_info.value(edge->dest())->min_lstart();
     int lat = edge->latency();

     cdbg << *edge << " : src-es=" << src_estart << " lat="<<lat<<" dest-ls="<<dest_lstart<<" gslk="<<dest_lstart-lat-src_estart<<endl;
     
     // HACK - want to put clear_all_rotating on same cluster as all_rot_pred.
     // do this by making edge wt from op to USE XXX_INF.
     if (is_clear_all_rotating(edge->src()))
       edge_weights.bind(edge, XXX_INF);
     
     else if (dest_lstart - lat - src_estart == 0)
       edge_weights.bind(edge, XXX_HIGHEST_EDGE_WT);
     else if (El_rhop_edge_weights == EL_CLSTR_RHOP_CRIT)
       edge_weights.bind(edge, XXX_LOW_EDGE_WT);
   }

   if (El_rhop_edge_weights == EL_CLSTR_RHOP_FCFS || El_rhop_edge_weights == EL_CLSTR_RHOP_FCFS2) {

     Slist<Stuple<int, Edge*> > edges_to_process_up, edges_to_process_down;
     find_block_start_edges(block, edges_to_process_up);

     while (!edges_to_process_up.is_empty()) {
       // pick highest estart node
       // really should get all edges w/ same dest estart, do some intelligent tiebreaking.
       Edge* cur_edge = edges_to_process_up.pop().second;
       process_edge_up(cur_edge, edges_to_process_up);
       
       int src_estart = dep_info.value(cur_edge->src())->m_Estart;
       edges_to_process_down.add(Stuple<int, Edge*>(src_estart, cur_edge));
     }
     
     while (!edges_to_process_down.is_empty()) {
       Edge* cur_edge = edges_to_process_down.pop().second;
       process_edge_down(cur_edge);
     }
   }
}

void MultiLevel::find_block_start_edges(Compound_region *block, Slist<Stuple<int, Edge*> >& edges_to_process)
{
  edges_to_process.clear();
  
  Hash_set<Op*> start_ops(hash_op_ptr, HASH_SIZE_1);
  
  for (Region_all_ops opi(block); opi!=0; opi++) {
    Op *op = *opi;

    bool has_outedges = false;
      
    if(is_pseudo(op) && op->opcode() != USE) continue;

    for (Op_outedges edgei(op, FLOW_EDGES); edgei!=0; edgei++) {
      Edge *edge = *edgei;      
      if (!is_switch(edge->dest()))
	has_outedges = true;     
    }   
        
    if(!has_outedges) {
      start_ops += op;
    }
  }

  for (Hash_set_iterator<Op*> hsi(start_ops); hsi!=0; hsi++) {
    Op* op = *hsi;
    int estart = dep_info.value(op)->m_Estart;

    for (Op_inedges edgei(op, FLOW_EDGES); edgei!=0; edgei++) {
      Edge* edge = *edgei;
      if (!is_pseudo(edge->src()) || edge->src()->opcode() == DEF) {
	Stuple<int, Edge*> st(-estart, edge);
	edges_to_process.add(st);
      }
    }
  }
}

void MultiLevel::process_edge_up(Edge* cur_edge, Slist<Stuple<int, Edge*> >& edges_to_process_up)
{
  // find my slack
  int from_estart = dep_info.value(cur_edge->src())->m_Estart;
  int orig_lat = cur_edge->latency();
  int to_lstart = dep_info.value(cur_edge->dest())->min_lstart();
  int gslack = to_lstart - orig_lat - from_estart;
  
  if (!edge_weights.is_bound(cur_edge)) {

    // if gslack >= move lat, edge lat += move lat
    if (gslack >= XXX_MOVE_LATENCY) {

      if (El_rhop_edge_weights == EL_CLSTR_RHOP_FCFS2) {

	// 2 moves worth of slack gets LOW_WT
	if (gslack >= 2 * XXX_MOVE_LATENCY) {
	  cur_edge->set_latency(orig_lat + 2 * XXX_MOVE_LATENCY);
	  edge_weights.bind(cur_edge, XXX_LOW_EDGE_WT);
	} else { // 1 move worth of slack gets MED_WT
	  cur_edge->set_latency(orig_lat + XXX_MOVE_LATENCY);
	  edge_weights.bind(cur_edge, XXX_MED_EDGE_WT);
	}

      } else {
	// any slack gets LOW_WT
	cur_edge->set_latency(orig_lat + XXX_MOVE_LATENCY);
	edge_weights.bind(cur_edge, XXX_LOW_EDGE_WT);
      }
    }

    else {
      edge_weights.bind(cur_edge, XXX_HIGHER_EDGE_WT);
    }
  }

  // update lstart on src node
  // just update m_Lstart[0] as a HACK
  Op* from_op = cur_edge->src();
  int new_lstart = to_lstart - cur_edge->latency();
  if (new_lstart < dep_info.value(from_op)->min_lstart()) {
    dep_info.value(from_op)->m_Lstart[0] = new_lstart;
  }

  // place predecessor edges in list, if they aren't in there already.
  // but, is_member check is O(n)
  for (Op_inedges edgei(from_op, FLOW_EDGES); edgei!=0; edgei++) {
    Edge* edge = *edgei;
    Op* grand_from_op = edge->src();
    if (is_pseudo(grand_from_op) && grand_from_op->opcode() != DEF)
      continue;
    
    Stuple<int, Edge*> st(-from_estart, edge);

    if (!edges_to_process_up.is_member(st)) {
      edges_to_process_up.add(st);
    }
  }
}

void MultiLevel::process_edge_down(Edge* cur_edge)
{
  // do i need to do anything?
}

void MultiLevel::find_block_max_estart(Compound_region* block)
{
  max_estart = 0;

  for(Region_all_ops opi(block); opi!=0; opi++) {
    Op* op = *opi;
    if (dep_info.is_bound(op)) {
      int cur_estart = dep_info.value(op)->m_Estart;
      if (cur_estart > max_estart)
	max_estart = cur_estart;

      int lstart = dep_info.value(op)->min_lstart();
      cdbg << "op " << op->id()<< " " << op->opcode() << " has estart=" << cur_estart
	   << " and lstart=" << lstart << endl;
    }
  }
  cdbg << "max_estart is " << max_estart << endl;
}

void MultiLevel::find_block_total_node_weights(Compound_region* block)
{
   for (unsigned clstr = 0; clstr < cmgr->hw_cluster_vec.size(); clstr++) {
      //int cycle = 0;
      int op_count = 0;
      char iopat[XXX_MAX_IOPAT];
      bool found_flag;
      char* opc_dummy; int pri_dummy;

      RU_init_map(false, XXX_MAX_RU_LENGTH);

      // place all ops in this block on this cluster
      for (Region_all_ops opi(block); opi != 0; opi++) {
	 Op* cur_op = *opi;
	 if (is_pseudo(cur_op)) continue;

	 // should reset cycle for each op
	 int cycle = 0;
	 
	 //op_count++;

	 char* opc = (char*)el_opcode_to_string_map.value(cur_op->opcode());
	 build_io(iopat, cur_op, clstr);

	 do {
	    RU_init_iterator(opc, cur_op, iopat, cycle);
	    found_flag = RU_get_next_nonconfl_alt(&opc_dummy, &pri_dummy);
	    if (found_flag) {
	       RU_place();
	       op_count++;
	    }
	    else
	       cycle++;
	 } while (!found_flag && cycle < XXX_MAX_RU_LENGTH);
      }

      total_node_weights[clstr] = (double) RU_cur_sched_length() / (double) op_count;
      //total_node_weights[clstr] = (double) (cycle+1) / (double) op_count;
   }
}

void MultiLevel::print_node_weights()
{
   for (Hash_map_iterator<Op*, Vector<double> > hmi(node_weights); hmi != 0; hmi++) {
      cdbg << (*hmi).first->id();
      Vector<double> vec = (*hmi).second;
      for (unsigned i = 0; i < vec.size(); i++) {
	 cdbg << " " << i << ": " << vec[i];
      }
      cdbg << endl;
   }
}

void MultiLevel::print_edge_weights()
{
   for (Hash_map_iterator<Edge*,float> hmi(edge_weights); hmi != 0; hmi++) {
      cdbg << *(*hmi).first << " : " << (*hmi).second << endl;
   }
}

// ****************************************

void MultiLevel::set_control_dep_lat_to_zero(Compound_region* rgn)
{
  for (Region_all_ops opi(rgn); opi!=0; opi++) {
    for (Op_outedges ei(*opi, CONTROL1_OUTEDGES); ei!=0; ei++) {
      Edge* edge = *ei;
      edge->set_latency(0);
    }
  }
}

void MultiLevel::do_block_setup_and_clustering(Compound_region* rgn)
{
   create_local_analysis_info_for_all_hbs_bbs(rgn);
   el_flow_compute_liveness(rgn, ANALYZE_ALLREG);
   insert_region_scalar_edges(rgn);

   set_control_dep_lat_to_zero(rgn);

   cdbg << "\nbb " << rgn->id() << endl;
   setup_block(rgn);
   print_edge_weights();
   cdbg << "max_estart is " << max_estart << endl;
  
#ifndef FANK
   int num_ops = 0;
   float weight;

   Hash_map<int, int> coarse_op_map(hash_int, HASH_SIZE_1);
   Hash_set<Op*> block_operations(hash_ml_op, HASH_SIZE_1);

   // hash all ops to produce random ordering for coarsening
   for (Region_all_ops opi(rgn); opi != 0; opi++) {
     Op* op = *opi;
     if(!op->is_pseudo_op() || (op->opcode() == DEF) || op->opcode() == USE) {
       //       block_operations += op;
     
       // fill opcode_map for weight calculations       
//        if(opcode_map.is_bound(op->opcode())) {
// 	 List<Op*> list_ops = opcode_map.value(op->opcode());     
// 	 list_ops.push(op);
// 	 opcode_map.bind(op->opcode(), list_ops);
//        }
//        else {
//  	 List<Op*> new_list;
// 	 new_list.push(op);
// 	 opcode_map.bind(op->opcode(), new_list);
//        }

       coarse_op_map.bind(op->id(), num_ops);
       num_ops++;
     }
   }
   /*   
   for(Hash_set_iterator<Op*> hsi(block_operations); hsi!=0; hsi++) {
     Op* op = *hsi;
     if(!op->is_pseudo_op() || (op->opcode() == DEF) || op->opcode() == USE) {
       coarse_op_map.bind(op->id(), num_ops);
       num_ops++;
     }
   }
   */

   // create initial coarse_op Vector
   Vector<Coarse_op> all_ops(num_ops);

   int index=0;

   // make all ops coarse_ops
   //   for(Hash_set_iterator<Op*> hsi2(block_operations); hsi2!=0; hsi2++, index++) {
   for (Region_all_ops opi2(rgn); opi2 != 0; opi2++) {
     //     Op* op = *hsi2;
     Op* op = *opi2;

     if(op->is_pseudo_op() && (op->opcode() != DEF) && (op->opcode() != USE))
       continue;

     int op_id;

     all_ops[index].add_member(op);
     all_ops[index].cop_id = index;
     all_ops[index].boundary_weight = 0;
     
     cdbg << "op: " << op->id() << ":" << op->opcode() << " is valid on cluster(s): ";
     for(unsigned idx = 0; idx < cmgr->hw_cluster_vec.size(); idx++) {
       if(node_weights.value(op)[idx] != XXX_INF) {
	 cdbg << idx << " ";
	 all_ops[index].valid_clusters += idx;
       }
     }
     cdbg << endl;

     // create edges for coarse_ops
     for(Op_inedges eitr(op, FLOW_EDGES); eitr!=0; eitr++) {
       Edge* edge = *eitr;                  
       op_id = (edge->src())->id();

       if(coarse_op_map.is_bound(op_id)) {
	 weight = edge_weights.value(edge); 
	 all_ops[index].add_in_edge(weight, coarse_op_map.value(op_id), edge);
       }
     }

     for(Op_outedges eitr2(op, FLOW_EDGES); eitr2!=0; eitr2++) {
       Edge* edge = *eitr2;
       op_id = (edge->dest())->id();
       if(coarse_op_map.is_bound(op_id)) {
	 weight = edge_weights.value(edge); 
	 all_ops[index].add_out_edge(weight, coarse_op_map.value(op_id), edge);
       }
     }    
     index++;
   }

   
//    for(int i = 0; i < all_ops.size(); i++) {
//      cdbg << i << ": ";
//      all_ops[i].print_members();
//    }

   // coarsen ops
   do_block_ml_coarsening(rgn, all_ops, true);

   do_block_ml_final_pass(rgn, all_ops);

   // remove defs/uses
   remove_block_pseudo_defs_and_uses(rgn);
   
   // bind all ops
   do_block_bind_ops(rgn);

#else
   
   remove_block_pseudo_defs_and_uses(rgn);
   for (Region_all_ops opi(rgn); opi!=0; opi++) {
     Op* op = *opi;
     if (is_pseudo(op)) continue;
     if (!cmgr->op_map.is_bound(op))
       cmgr->op_map.bind(op, 0);
   }
   
#endif   

   // operand_assignment
   //cmgr->do_block_operand_assignment(rgn);
   cmgr->do_block_smarter_operand_assignment(rgn);

   delete_region_edges(rgn);
}

void MultiLevel::do_block_ml_coarsening(Compound_region* rgn, Vector<Coarse_op>& ops, bool progress)
{
  Vector< Coarse_op > coarsened_ops(ops.size());
  Hash_map<int, int> coarse_op_map(hash_int, HASH_SIZE_1);

  if((int) ops.size() <= (int) cmgr->hw_cluster_vec.size()) {    
    // simple initial paritioner
    for(unsigned idx = 0; idx < ops.size(); idx++) {
      for(Hash_set_iterator<Op*> hsi(ops[idx].members); hsi != 0; hsi++) {
	Op* op = *hsi;
	clusters[idx] += op;      
      }
      ops[idx].cluster = idx;
    }

    cdbg << "Initial Partition" << endl;
    for(unsigned i = 0; i < ops.size(); i++) {
     cdbg << i << ": ";
     ops[i].print_members();
    }
    cdbg << endl;
    return;
  }
  else {
    // have not yet met fully coarsened state, continue coarsening
    Hash_set<int> coarsened (hash_int, HASH_SIZE_1);

    int smallest_merge = 0;
    int index1, index2;
    bool coarsen_progress = false;
    int old_idx = 0;
    int new_idx = 0;

    // if no progress, find two smallest ops to merge
    if(!progress) {       
      Slist< Stuple<int, int> > smallest_ops_list;
      Stuple<int, int> cop1, cop2;

      for(unsigned clust = 0; clust < cmgr->hw_cluster_vec.size(); clust++) {
	smallest_ops_list.clear();

	for(unsigned idx = 0; idx < ops.size(); idx++) {
	  if((ops[idx].valid_clusters).is_member(clust)) {	    
	    Stuple<int, int> op_tuple(-(ops[idx].members).size(), idx);
	    smallest_ops_list.add(op_tuple);
	  }
	}

	if(smallest_ops_list.size() >= 2) {
	  cop1 = smallest_ops_list.pop();
	  cop2 = smallest_ops_list.pop();
	  int num_members = ops[cop1.second].members.size() + ops[cop2.second].members.size();

	  if(num_members < smallest_merge) {
	    smallest_merge = num_members;
	    index1 = cop1.second;
	    index2 = cop2.second;	    
	  }
	}	
      }   

      // coarsen ops
      while(old_idx < (int) ops.size()) {     
	while(coarsened.is_member(old_idx))
	  old_idx++;
      
	if(old_idx >= (int)ops.size()) break;
	
	// no progress, coarsen two smallest cops
	if(old_idx == index1 || old_idx == index2) {
	  coarsened += index1;
	  coarsened += index2;
	  coarse_op_map.bind(index1, new_idx);
	  coarse_op_map.bind(index2, new_idx);
	  
	  coarsened_ops[new_idx] += ops[index1];
	  coarsened_ops[new_idx] += ops[index2];
	  coarsened_ops[new_idx].cop_id = new_idx;	
  	  coarsened_ops[new_idx].fix_boundary_weight(ops[index1], ops[index2]);   
//  	  cdbg << "\tcoarsening smallest " << index1 << " and " << index2 << endl;
	}
	else { // nothing more to coarsen 
//  	  cdbg << "\tcoarsening smallest " << old_idx << endl;
	  coarsened += old_idx;	  
	  coarse_op_map.bind(old_idx, new_idx);

	  coarsened_ops[new_idx] += ops[old_idx];
	  coarsened_ops[new_idx].cop_id = new_idx;
	}
	old_idx++;      
	new_idx++;
      }
     

    }  
    else {
      Slist< Stuple<int, int> > coarse_op_list;

      for(unsigned idx = 0; idx < ops.size(); idx++) {
	Stuple<int, int> cop(ops[idx].boundary_weight, ops[idx].cop_id);
	coarse_op_list.add(cop);
      }

      int matched_op;
      new_idx = 0;

      while(!coarse_op_list.is_empty()) {
	Stuple<int, int> to_merge = coarse_op_list.pop();
	if(coarsened.is_member(to_merge.second)) continue;

	Coarse_op *op1 = &ops[to_merge.second];
	matched_op = op1->get_highest_weight_edge(coarsened, ops);

	if(matched_op == -1) { 
	  // nothing more to coarsen for this coarse_op
	  coarsened += to_merge.second;
	  coarse_op_map.bind(to_merge.second, new_idx);
	  
	  coarsened_ops[new_idx] += ops[to_merge.second];
	  coarsened_ops[new_idx].cop_id = new_idx;
	  //  	  cdbg << "\tcoarsening " << to_merge.second << endl;	  
	}
	else {
	  // found a pairing for this coarse_op
	  coarsen_progress = true;

	  coarsened += to_merge.second;
	  coarsened += matched_op;
	  coarse_op_map.bind(to_merge.second, new_idx);
	  coarse_op_map.bind(matched_op, new_idx);
	  
	  coarsened_ops[new_idx] += ops[to_merge.second];
	  coarsened_ops[new_idx] += ops[matched_op];
	  coarsened_ops[new_idx].cop_id = new_idx;	  

	  coarsened_ops[new_idx].fix_boundary_weight(ops[to_merge.second], ops[matched_op]);      
	  //  	  cdbg << "\tcoarsening " << to_merge.second << " and " << matched_op << endl;
	}
	new_idx++;
      }
    }
    coarsened_ops.resize(new_idx);

    for(int i = 0; i < new_idx; i++) {
      coarsened_ops[i].fix_coarse_edges(i, &coarse_op_map);
    }
       
//     for(i = 0; i < new_idx; i++) {
//       cdbg << i << ": ";
//       coarsened_ops[i].print_members();
//     }
    
//     cdbg << "--------------------------------------------------" << endl;
	
    do_block_ml_coarsening(rgn, coarsened_ops, coarsen_progress);
  }    

  for(unsigned i = 0; i < ops.size(); i++) {
    ops[i].cluster = coarsened_ops[coarse_op_map.value(i)].cluster;
  }

  // Refine partition
  do_block_ml_refine_partition(rgn, ops);

}

void MultiLevel::do_block_ml_refine_partition(Compound_region* rgn, Vector<Coarse_op>& ops) {
  // find cluster with highest imbalance
  // for all ops pick op that lowers gain in that cluster the most
  //   for all ties, pick op with lowest edge cuts
  //      for all ties pick op with smallest estart
  //        move this op  
  //   cdbg << "refining" << endl;

  Vector< Hash_set<Op*> > new_clusters(cmgr->hw_cluster_vec.size());
  Hash_set<int> chosen(hash_int, HASH_SIZE_1);

  int move_from;
  float gain_sum;
  double system_load;
    
  // figure out which cluster to move from
  move_from = find_imbalanced_cluster(chosen);//       cdbg << "Current State of Machine:" << endl;
//       for(int i = 0; i < clusters.size(); i++) {
// 	cdbg << i << ": ";
// 	for(Hash_set_iterator<Op*> all_ops(clusters[i]); all_ops !=0; all_ops++) {
// 	  Op* op = *all_ops;
// 	  cdbg << op->id() << " ";
// 	}	  
// 	cdbg << endl;
//       }
//       cdbg << endl;
  chosen += move_from;

  num_icmoves = 0;
  for(unsigned idx2 = 0; idx2 < clusters.size(); idx2++) 
    count_icmoves(clusters[idx2]);

  cdbg << "total of " << num_icmoves << " moves." << endl;

  while(move_from != -1 && clusters[move_from].size() != 0) {    
    gain_sum = 0;
   
    while(gain_sum >= 0 && clusters[move_from].size() != 0) {      

      cdbg << "Current State of Machine:" << endl;
      for(unsigned i = 0; i < clusters.size(); i++) {
	cdbg << i << ": ";
	for(Hash_set_iterator<Op*> all_ops(clusters[i]); all_ops !=0; all_ops++) {
	  Op* op = *all_ops;
	  cdbg << op->id() << " ";
	}	  
	cdbg << endl;
      }
      cdbg << endl;

      cdbg << "moving from " << move_from << endl;
      system_load = even_newer_calculate_system_load(clusters);
      
      float best_gain = ELCOR_MIN_INT;
      Coarse_op* best_move = NULL;
      int best_move_to = -1;
      int best_num_icmoves = -1;

      // figure out which op to move
      for(unsigned vidx = 0; vidx < ops.size(); vidx++) {      
	if(ops[vidx].cluster != move_from) continue;
	if(ops[vidx].locked == true) continue;

	Coarse_op* cop = &ops[vidx];
	double cur_gain;

	cdbg << *cop;

	for(int idx = 0; idx < (int)clusters.size(); idx++) {
	  if(idx == move_from) continue;     

	  int ic_moves_before = num_icmoves;
	  cur_gain = calculate_edge_gain(cop, move_from, idx);

	  new_clusters = clusters;
	  for(Hash_set_iterator<Op*> opi(cop->members); opi!=0; opi++) {
	    Op* op = *opi;
	    new_clusters[move_from] -= op;
	    new_clusters[idx] += op;
	  }
	  
	  double new_system_load = even_newer_calculate_system_load(new_clusters);
	  int difference = (int) (system_load - new_system_load);

	  cur_gain += (XXX_HIGHEST_EDGE_WT*(difference));
	  cdbg << endl << "\tload diff: " << difference 
	       << "\ttotal_gain:" << cur_gain 
	       << "\ticmoves: " << num_icmoves << endl;

	  if(cur_gain > best_gain && cop->locked != true) {
	    best_gain = cur_gain;
	    best_move = cop;
	    best_move_to = idx;
	    best_num_icmoves = num_icmoves;
	  }
	  num_icmoves = ic_moves_before;

	}      
      }

      if(best_gain == ELCOR_MIN_INT) break; // didn't have anything to move

      if(best_gain < 5000)  // if its not an infinite move
	gain_sum += best_gain;

      if(gain_sum >= 0) {
	cdbg << "deciding to move: " << *best_move;

	if(best_gain >= 0) 
	  best_move->locked = true;

	best_move->cluster = best_move_to;
	for(Hash_set_iterator<Op*> opi(best_move->members); opi!=0; opi++) {
	  Op* op = *opi;
	  clusters[move_from] -= op;
	  clusters[best_move_to] += op;
	}
	num_icmoves = best_num_icmoves;
      }

      cdbg << endl;

    } // while(gain_sum >= 0 && more_to_move)
    
    int new_move_from = find_imbalanced_cluster(chosen);
    chosen += new_move_from;

    if(new_move_from == move_from)
      move_from = -1;
    else
      move_from = new_move_from;

  } // while(1)

  cdbg << "--------------------------------------------------" << endl;
}



void MultiLevel::do_block_ml_final_pass(Compound_region* rgn, Vector<Coarse_op>& ops) {
  Vector< Hash_set<Op*> > new_clusters(cmgr->hw_cluster_vec.size());

  cdbg << "FINAL PASS" << endl;

  int move_from;
  float gain_sum = 0.0;
  double system_load;
    
  // figure out which cluster to move from
  Slist< Stuple<int, int> > cluster_list;
  for(unsigned i = 0; i < clusters.size(); i++) {
    Stuple<int, int> clust(clusters[i].size(), i);
    cluster_list.add(clust);
  }

  num_icmoves = 0;
  for(unsigned idx2 = 0; idx2 < clusters.size(); idx2++) 
    count_icmoves(clusters[idx2]);

  while(cluster_list.size() != 0) {    
    Stuple<int, int> tuple = cluster_list.pop();
    move_from = tuple.second;
    bool first = true;

    while((gain_sum > 0 || first) && clusters[move_from].size() != 0) {
      first = false;

      cdbg << "moving from " << move_from << endl;
      system_load = even_newer_calculate_system_load(clusters);
      
      float best_gain = ELCOR_MIN_INT;
      Coarse_op* best_move = NULL;
      int best_move_to = -1;
      int best_num_icmoves = -1;

      // figure out which op to move
      for(unsigned vidx = 0; vidx < ops.size(); vidx++) {      
	if(ops[vidx].cluster != move_from) continue;
	//if(ops[vidx].locked == true) continue;

	ops[vidx].locked = false;

	Coarse_op* cop = &ops[vidx];
	double cur_gain;

	cdbg << *cop;

	for(int idx = 0; idx < (int)clusters.size(); idx++) {
	  if(idx == move_from) continue;     

	  int ic_moves_before = num_icmoves;
	  cur_gain = calculate_edge_gain(cop, move_from, idx);

	  new_clusters = clusters;
	  for(Hash_set_iterator<Op*> opi(cop->members); opi!=0; opi++) {
	    Op* op = *opi;
	    new_clusters[move_from] -= op;
	    new_clusters[idx] += op;
	  }
	  
	  double new_system_load = even_newer_calculate_system_load(new_clusters);
	  int difference = (int) (system_load - new_system_load);

	  cur_gain += (XXX_HIGHEST_EDGE_WT*(difference));
	  cdbg << endl << "\tload diff: " << difference 
	       << "\ttotal_gain:" << cur_gain 
	       << "\ticmoves: " << num_icmoves << endl;

	  if(cur_gain > best_gain) { // && cop->locked != true) {
	    best_gain = cur_gain;
	    best_move = cop;
	    best_move_to = idx;
	    best_num_icmoves = num_icmoves;
	  }
	  num_icmoves = ic_moves_before;

	}      
      }

      if(best_gain == ELCOR_MIN_INT) break; // didn't have anything to move

      gain_sum = best_gain;

      if(gain_sum > 0) {
	cdbg << "deciding to move: " << *best_move;

	if(best_gain > 0) 
	  best_move->locked = true;

	best_move->cluster = best_move_to;
	for(Hash_set_iterator<Op*> opi(best_move->members); opi!=0; opi++) {
	  Op* op = *opi;
	  clusters[move_from] -= op;
	  clusters[best_move_to] += op;
	}
	num_icmoves = best_num_icmoves;
      }

      cdbg << endl;

    } // while(gain_sum >= 0 && more_to_move)
    
  } // while(1)

  cdbg << "--------------------------------------------------" << endl;
}



void MultiLevel::count_icmoves(Hash_set<Op*>& cluster)
{
  Hash_set<Op*> counted(hash_op_ptr, HASH_SIZE_1);
  
  for(Hash_set_iterator<Op*> hsi(cluster); hsi !=0; hsi++) {
    Op* op = *hsi;
    for (Op_inedges edgei(op, FLOW_EDGES); edgei!=0; edgei++) {
      Edge* edge = *edgei;
      if(!cluster.is_member(edge->src()) && !counted.is_member(edge->src())) {
	num_icmoves++;
	counted += edge->src();
      }
    }   
  }   
}

double MultiLevel::calculate_edge_gain(Coarse_op* cop, int move_from, int move_to) 
{
  float saved = 0;
  float cut = 0;

  for(Hash_set_iterator<Edge*> edgei(cop->in_edges); edgei != 0; edgei++) {
    Edge* edge = *edgei;
    Op* src = edge->src();

//    bool need_a_cut = true;
//    bool need_a_save = true;

    Hash_set<Op*> counted(hash_op_ptr, HASH_SIZE_1);

    float wgt = edge_weights.value(edge);
    
    if(clusters[move_from].is_member(src)) {
      if(!(cop->members).is_member(src)) {

	if(!counted.is_member(src)) {
	  cut += wgt;
	  //	  num_icmoves++;
	  counted += src;
	}
	  
// 	for(Op_outedges eitr(src, FLOW_EDGES); eitr!=0; eitr++) {
// 	  Edge* edge2 = *eitr;
// 	  Op* dest = edge2->dest();
// 	  if(clusters[move_to].is_member(dest)) {
// 	    need_a_cut = false;
// 	  }
// 	}   	
// 	if(need_a_cut) {
// 	  cut += wgt;
// 	  num_icmoves++;
// 	}

      }    
    }

    counted.clear();

    if(clusters[move_to].is_member(src)) {
      
      if(!counted.is_member(src)) {
	saved += wgt;
	//	num_icmoves--;
	counted += src;
      }

//       for(Op_outedges eitr(src, FLOW_EDGES); eitr!=0; eitr++) {
// 	Edge* edge2 = *eitr;
// 	Op* dest = edge2->dest();
// 	if(clusters[move_from].is_member(dest) && dest != me) {
// 	  need_a_save = false;
// 	}
//       }   	
//       if(need_a_save) {
// 		saved += wgt;
// 	num_icmoves--;
//       }

    }

  }

  float high_cut = 0;
  float high_saved = 0;

  for(Hash_set_iterator<Edge*> edgei2(cop->out_edges); edgei2 != 0; edgei2++) {
    Edge* edge = *edgei2;
    Op* dst = edge->dest();

    float wgt = edge_weights.value(edge);
    
    if(clusters[move_from].is_member(dst)) 
      if(!(cop->members).is_member(dst)) 
	if(wgt > high_cut) 
	  high_cut = wgt;

    if(clusters[move_to].is_member(dst))
      if(wgt > high_saved)
	high_saved = wgt;
  }
  cut += high_cut;
  saved += high_saved;

  if(high_cut > 0)
    num_icmoves++;
  if(high_saved > 0)
    num_icmoves--;
  
  cdbg << "\tto " << move_to << "\tsave: " << saved << "  \tcut: " << cut << " ";      

  return(saved-cut);
}

Resource_type MultiLevel::get_resource_type(Op* op) {

  if(is_ialu(op) || is_icmpp(op) || is_pbr(op))
    return I_unit;
  else if(is_falu(op) || is_fcmpp(op))
    return F_unit;
  else if(is_memory(op))
    return M_unit;
  else if(is_branch(op) || is_brl(op))
    return B_unit;
  else if(op->opcode() == DEF || op->opcode() == USE)
    return pseudo;

  El_punt("Got an op that isn't an IFMB: %d", op->id());
	  
  return I_unit;
}

int MultiLevel::even_newer_calculate_weight(Hash_set<Op*>& ops, int c_idx) {
  int total = 0;
  Hash_map<int, double> resource_weights(hash_int, HASH_SIZE_1);

//   cdbg << endl << "Calculating weight of cluster: " << c_idx << endl;

  for(unsigned idx = 0; idx < per_cycle_ops.size(); idx++) {
//     cdbg << "------------------------------------" << endl;
//     cdbg << "cycle: " << idx << endl;

    resource_weights.clear();

    double wgt = 0.0;
    double tnw = 0.0;
     
//     cdbg << "ops: ";

    List<Op*> cycle_ops = per_cycle_ops[idx];
    for(List_iterator<Op*> li(cycle_ops); li!=0; li++) {
      Op* op = *li;

      if(ops.is_member(op)) {

//    	cdbg << op->id() << " ";	
	Resource_type type = get_resource_type(op);
	
	if(resource_weights.is_bound(type)) {
	  double resource_wgt = resource_weights.value(type);
	  resource_wgt += node_weights.value(op)[c_idx] / slack_map.value(op);
	  resource_weights.bind(type, resource_wgt);
	  if(!op->is_pseudo_op())
	    tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	}
	else {
	  double resource_wgt = node_weights.value(op)[c_idx] / slack_map.value(op);
	  resource_weights.bind(type, resource_wgt);
	  if(!op->is_pseudo_op())
	    tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	}	 
      }      
    }
    
//     cdbg << endl;

    for(Hash_map_iterator<int, double> hmi(resource_weights); hmi!=0; hmi++) {
      Pair<int, double> tuple = *hmi;
//       cdbg << "\t" << tuple.first << ":" << tuple.second << endl;

      if(tuple.second > wgt)
	wgt = tuple.second;
    }

//     cdbg << "tnw: " << tnw << endl;
//     cdbg << "wgt: " << wgt << endl;

    if(tnw > wgt)
      wgt = tnw;

    int temp = (int) (ceil(wgt));
    if(temp == 0)
      temp = 1;

    total += temp-1;
  }

//   cdbg << "total: " << total << endl;

  return total;
}

int MultiLevel::new_calculate_weight(Hash_set<Op*>& ops, int c_idx) {
  int total = 0;
  int real_ops = 0;
  Hash_map<Opcode, double> opcode_weights(hash_opcode, HASH_SIZE_1);

//   cdbg << "Calculating weight of cluster: " << c_idx << endl;

  for(unsigned idx = 0; idx < per_cycle_ops.size(); idx++) {
//     cdbg << "------------------------------------" << endl;
//     cdbg << "cycle: " << idx << endl;

    opcode_weights.clear();
    real_ops = 0;
    double wgt = 0.0;
    double tnw = 0.0;
     
//       cdbg << "ops: ";

    List<Op*> cycle_ops = per_cycle_ops[idx];
    for(List_iterator<Op*> li(cycle_ops); li!=0; li++) {
      Op* op = *li;

      if(ops.is_member(op)) {
//   	cdbg << op->id() << " ";

	if(opcode_weights.is_bound(op->opcode())) {
	  double opcode_wgt = opcode_weights.value(op->opcode());
	  opcode_wgt += node_weights.value(op)[c_idx] / slack_map.value(op);
	  opcode_weights.bind(op->opcode(), opcode_wgt);
	  if(!op->is_pseudo_op())
	    tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	}
	else {
	  double opcode_wgt = node_weights.value(op)[c_idx] / slack_map.value(op);
	  opcode_weights.bind(op->opcode(), opcode_wgt);
	  if(!op->is_pseudo_op())
	    tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	}	 
      }      
    }

//      cdbg << endl;

    for(Hash_map_iterator<Opcode, double> hmi(opcode_weights); hmi!=0; hmi++) {
      Pair<Opcode, double> tuple = *hmi;
//         cdbg << "\t" << tuple.first << ":" << tuple.second << endl;

      if(tuple.second > wgt)
	wgt = tuple.second;
    }

//      cdbg << "tnw: " << tnw << endl;
//      cdbg << "wgt: " << wgt << endl;

    if(tnw > wgt)
      wgt = tnw;

    int temp = (int) (ceil(wgt));
    if(temp == 0)
      temp = 1;

    total += temp-1;
  }
//     cdbg << "total: " << total << endl;
  return total;
}

double MultiLevel::calculate_weight(Hash_set<Op*>& ops, int c_idx)
{
  double high_weight = ELCOR_MIN_DOUBLE;
  double weight = 0.0;
  double total = 0.0;
  int real_ops = 0;

  for(Hash_map_iterator<Opcode, List<Op*> > hmi(opcode_map); hmi !=0; hmi++) {
    weight = 0.0;

    Pair<Opcode, List<Op*> > tuple = *hmi;
    List<Op*> op_list = tuple.second;

    for(List_iterator<Op*> li(op_list); li!=0; li++) {
      Op* op = *li;
      if(ops.is_member(op)) {
	weight += node_weights.value(op)[c_idx];
	if(!is_pseudo(op))
	  real_ops++;
      }
    }

    if(weight > high_weight)
      high_weight = weight;
  }

  double total_node_weight = real_ops * total_node_weights[c_idx];   
  
  if(high_weight > total_node_weight) 
    total = high_weight;  
  else 
    total = total_node_weight;


  total -= max_estart;

  if(0 > total)
    return 0;
 
  return total;
}

int MultiLevel::find_imbalanced_cluster(Hash_set<int>& chosen) {
  int imbalanced_clust = -1;
  int num_ops = 0;
  double high_weight = -1;
  double weight;
  
  // figure out which cluster to move from
  for(unsigned cidx = 0; cidx < clusters.size(); cidx++) {
    weight = even_newer_calculate_weight(clusters[cidx], cidx);   
    cdbg << "cluster " << cidx << " weight: " << weight << endl;

    if(weight > high_weight && clusters[cidx].size() != 0) {
      high_weight = weight;
      imbalanced_clust = cidx;
      num_ops = clusters[cidx].size();
    }
    else if(weight == high_weight) {
      if(chosen.is_member(imbalanced_clust)) {
	imbalanced_clust = cidx;
	num_ops = clusters[cidx].size();
      }
    }
  }
  
  return imbalanced_clust;
}


double MultiLevel::even_newer_calculate_system_load(Vector<Hash_set<Op*> >& my_clusters) {
  double total = 0;

  Hash_map<int, double> resource_weights(hash_int, HASH_SIZE_1);

  for(unsigned idx = 0; idx < per_cycle_ops.size(); idx++) {
    int cycle_wgt = 0;
      
    for(unsigned c_idx = 0; c_idx < my_clusters.size(); c_idx++) {
      resource_weights.clear();
      double wgt = 0.0;
      double tnw = 0.0;

      List<Op*> cycle_ops = per_cycle_ops[idx];
      for(List_iterator<Op*> li(cycle_ops); li!=0; li++) {
	Op* op = *li;

	int type = get_resource_type(op);
      
	if(my_clusters[c_idx].is_member(op)) {
	  if(resource_weights.is_bound(type)) {
	    double resource_wgt = resource_weights.value(type);
	    resource_wgt += node_weights.value(op)[c_idx] / slack_map.value(op);
	    resource_weights.bind(type, resource_wgt);
	    if(!op->is_pseudo_op())
	      tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	  }
	  else {
	    double resource_wgt = node_weights.value(op)[c_idx] / slack_map.value(op);
	    resource_weights.bind(type, resource_wgt);
	    if(!op->is_pseudo_op())
	      tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	  }	 
	}      
      }
      
      for(Hash_map_iterator<int, double> hmi(resource_weights); hmi!=0; hmi++) {
	Pair<int, double> tuple = *hmi;

	if(tuple.second > wgt)
	  wgt = tuple.second;
      }

      if(tnw > wgt)
	wgt = tnw;

      int temp = (int) (ceil(wgt));
      if(temp == 0)
	temp = 1;

      if(temp-1 > cycle_wgt)
	cycle_wgt = temp-1;    
//       cdbg << "cluster " << c_idx << " is: " << temp-1 << endl;
    }
//     cdbg << "Cycle: " << idx << ": " << cycle_wgt << endl;
    total += cycle_wgt;
  }

//   cdbg << "system load is: " << total << endl;
  num_icmoves = 0;
  for(unsigned idx2 = 0; idx2 < my_clusters.size(); idx2++) 
    count_icmoves(my_clusters[idx2]);

  if(((num_icmoves*move_weight)-max_estart) > total) {
    cdbg << "[move limited]";
    total = ((num_icmoves*move_weight)-max_estart);
  }

  return total;
}


double MultiLevel::new_calculate_system_load(Vector<Hash_set<Op*> >& my_clusters) {
  double total = 0;

  int real_ops = 0;
  Hash_map<Opcode, double> opcode_weights(hash_opcode, HASH_SIZE_1);

  for(unsigned idx = 0; idx < per_cycle_ops.size(); idx++) {
    int cycle_wgt = 0;
      
    for(unsigned c_idx = 0; c_idx < my_clusters.size(); c_idx++) {
      opcode_weights.clear();
      real_ops = 0;
      double wgt = 0.0;
      double tnw = 0.0;

      List<Op*> cycle_ops = per_cycle_ops[idx];
      for(List_iterator<Op*> li(cycle_ops); li!=0; li++) {
	Op* op = *li;
      
	if(my_clusters[c_idx].is_member(op)) {
	  if(opcode_weights.is_bound(op->opcode())) {
	    double opcode_wgt = opcode_weights.value(op->opcode());
	    opcode_wgt += node_weights.value(op)[c_idx] / slack_map.value(op);
	    opcode_weights.bind(op->opcode(), opcode_wgt);
	    if(!op->is_pseudo_op())
	      tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	  }
	  else {
	    double opcode_wgt = node_weights.value(op)[c_idx] / slack_map.value(op);
	    opcode_weights.bind(op->opcode(), opcode_wgt);
	    if(!op->is_pseudo_op())
	      tnw += ((total_node_weights[c_idx])/slack_map.value(op));
	  }	 
	}      
      }
      
      for(Hash_map_iterator<Opcode, double> hmi(opcode_weights); hmi!=0; hmi++) {
	Pair<Opcode, double> tuple = *hmi;

	if(tuple.second > wgt)
	  wgt = tuple.second;
      }

      if(tnw > wgt)
	wgt = tnw;

      int temp = (int) (ceil(wgt));
      if(temp == 0)
	temp = 1;

      if(temp-1 > cycle_wgt)
	cycle_wgt = temp-1;    
//       cdbg << "cluster " << c_idx << " is: " << temp-1 << endl;
    }
//     cdbg << "Cycle: " << idx << ": " << cycle_wgt << endl;
    total += cycle_wgt;
  }

//   cdbg << "system load is: " << total << endl;
  num_icmoves = 0;
  for(unsigned idx2 = 0; idx2 < my_clusters.size(); idx2++) 
    count_icmoves(my_clusters[idx2]);

  if(((num_icmoves*move_weight)-max_estart) > total) {
    cdbg << "[move limited]";
    total = ((num_icmoves*move_weight)-max_estart);
  }

  return total;
}


double MultiLevel::calculate_system_load(Vector<Hash_set<Op*> >& my_clusters) {
  double high_weight = -1;
  double weight;
  
  // figure out which cluster to move from
  for(unsigned cidx = 0; cidx < my_clusters.size(); cidx++) {
    weight = new_calculate_weight(my_clusters[cidx], cidx);   
    
    if(weight > high_weight)
      high_weight = weight;
  }

  if(((num_icmoves*move_weight)-max_estart) > high_weight) {
    cdbg << "[move limited]";
    high_weight = ((num_icmoves*move_weight)-max_estart);
  }

  return high_weight;
}


void MultiLevel::do_block_bind_ops(Compound_region* rgn) {  
  for(unsigned idx = 0; idx < clusters.size(); idx++) {
    for(Hash_set_iterator<Op*> hsi(clusters[idx]); hsi != 0; hsi++) {
      Op* op = *hsi;
      cmgr->op_map.bind(op, idx);
    }
  }
}

void MultiLevel::remove_block_pseudo_defs_and_uses(Compound_region* rgn)
{
  Hash_set<Op*> pseudo_defs_and_uses(hash_op_ptr, 32);

  for (Region_all_ops opi(rgn); opi != 0; opi++) {
    Op* op = *opi;
    if((op->opcode() == DEF) || (op->opcode() == USE)) {
      pseudo_defs_and_uses += op;
    }
  }
  
  for (Hash_set_iterator<Op*> hsi(pseudo_defs_and_uses); hsi!=0; hsi++) {
    Op* op = *hsi;
    Op_inedges inei;
    Op_outedges outei;
    Edge *edge;
    Op *dest, *src;

    if(op->opcode() == DEF) {
      outei(op);
      while (outei != 0) {
	edge = *outei;
	outei++ ;
	if (edge->is_seq()) continue ;
	dest = edge->dest();
	op->remove_outedge(edge, edge->src_port_type(),
               edge->src_port(), edge->src_alt());
	dest->remove_inedge(edge, edge->dest_port_type(),
		edge->dest_port(), edge->dest_alt());
	delete edge;
      }      
    }
    else {
      inei(op);
      while (inei != 0) {
	edge = *inei;
	inei++;
	if (edge->is_seq()) continue ;
	src = edge->src();
	op->remove_inedge(edge, edge->dest_port_type(),
			  edge->dest_port(), edge->dest_alt());
	src->remove_outedge(edge, edge->src_port_type(),
			    edge->src_port(), edge->src_alt());
      } 
    }

    Op* removed_op = El_remove_op(op);
    delete(removed_op);
  }

}

void MultiLevel::do_postpass_operand_check()
{
   for (Region_all_ops opi(cmgr->m_proc); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;
      if (!cmgr->op_map.is_bound(cur_op))
	 El_punt("ml operand assignment: unbound op %d", cur_op->id());
      
      int clstr = cmgr->op_map.value(cur_op);

      // bound all unbound source operands to same cluster as cur_op
      // and give a warning.
      for (Op_explicit_inputs ii(cur_op); ii != 0; ii++) {
	 Operand& src = *ii;
	 if (src.is_reg() && !cmgr->operand_map.is_bound(src.vr_num())) {
	    cmgr->operand_map.bind(src.vr_num(), clstr);
	    El_warn("ml postpass operand check: use found with no def in op %d", 
		    cur_op->id());
	 }
	 else if (src.is_macro_reg()) {
	    int mn = (int)src.name();
	    El_ref cur_ref(cur_op, &src, ii.ref_type());

	    if(!cmgr->macro_vec[mn].is_bound(cur_ref)) {
	       if (src.is_hardwired())
		  El_punt("ml postpass operand check: unbound hardwired macro found in op %d", 
			  cur_op->id());

	       El_punt("ml postpass operand check: a macro source for op %d was bound in the postpass check", cur_op->id());
	    }
	 }
      }
   }
}

unsigned hash_ml_op(Op*& op)
{
  //  int hash = (int) &(op->src(SRC1));
  return(ELCOR_INT_HASH(op));
}
