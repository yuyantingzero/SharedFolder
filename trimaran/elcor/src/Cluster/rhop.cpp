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
//      File:           rhop.cpp
//      Author:         Mike Chu, Kevin Fan
//      Created:        January 2004
//      Description:    Region-based Hierarchical Operation Partitioner
//
/////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "list.h" 
#include "hash_map.h"
#include "rhop.h"
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
#include <iomanip>

int RHOP::num_ops;
int RHOP::max_estart;
double RHOP::move_weight;

Res_type get_resource_type(Op* op) {

  if(is_ialu(op) || is_icmpp(op) || is_pbr(op))
    return TYPE_INTEGER;
  else if(is_falu(op) || is_fcmpp(op))
    return TYPE_FLOAT;
  else if(is_memory(op))
    return TYPE_MEMORY;
  else if(is_branch(op) || is_brl(op))
    return TYPE_BRANCH;
  else if(op->opcode() == DEF || op->opcode() == USE)
    return TYPE_PSEUDO;
  // SETs and CLEARs of predicates are ignored 
  // in the resource model 
  // MLC (2/24/05)
  else if(op->opcode() == PRED_CLEAR_ALL_ROTATING || op->opcode() == PRED_SET || op->opcode() == PRED_CLEAR)
    return TYPE_PSEUDO;

  El_punt("RHOP -- get_resource_type() got non IFMB op: %d", op->id());
	  
  return TYPE_UNKNOWN;
}

////////////////////////////////////////////////////////////////////////////
//
//  Op_partition
//

Op_partition::Op_partition() 
  : members(hash_op_ptr, 1024),
    external_edges(),
    internal_edges()
{
  locked=false;
}

Op_partition::Op_partition(Op* op, RHOP * rhop_cluster) 
  : members(hash_op_ptr, 1024),
    external_edges(),
    internal_edges()
{
  id_rep = rhop_cluster->get_next_op_part_id_inc();
  members += op;
  rhop = rhop_cluster;
  merge_priority = 0;
  locked=false;

  partition_init();
      
  int estart = rhop->dep_info.value(op)->m_Estart;
  int lstart = rhop->dep_info.value(op)->min_lstart();
  int num_clusters = rhop->cmgr->hw_cluster_vec.dim();

  // need to take care of edges
  for(Op_inedges in_i(op); in_i != 0; in_i++) {
    Edge* iedge = *in_i;
    if(rhop->edge_weights.is_bound(iedge)) {
      external_edges += iedge;
      merge_priority += rhop->edge_weights.value(iedge);
    }
  }
  for(Op_outedges out_i(op); out_i != 0; out_i++) {
    Edge* oedge = *out_i;
    if(rhop->edge_weights.is_bound(oedge)) {
      external_edges += oedge;
      merge_priority += rhop->edge_weights.value(oedge);
    }
  }
  
  int res_type = (int) get_resource_type(op);

  if (is_pseudo(op) && (op->opcode() == USE || op->opcode() == DEF)) {
    if (op->opcode() == DEF) { 
      for(int i = 0; i < num_clusters; i++) 
	partition_load[i][0][res_type] = rhop->node_weights.value(op)[i];
    }
    else if (op->opcode() == USE) {
      for(int i = 0; i < num_clusters; i++) 
	partition_load[i][rhop->max_estart][res_type] = rhop->node_weights.value(op)[i]; 
    }
  } else {    
    for(int i = 0; i < num_clusters; i++) 
      // MLC 2/04 : fixed for ops where estart = lstart
      for(int j = estart; j <= lstart; j++) 	
	partition_load[i][j][res_type] = rhop->node_weights.value(op)[i] / rhop->slack_map.value(op);    
  }
} 

Op_partition::Op_partition(Op_partition* part, RHOP* rhop_cluster) 
  : members(hash_op_ptr, 1024),
    external_edges(),
    internal_edges()
{
  id_rep = rhop_cluster->get_next_op_part_id_inc();
  rhop = rhop_cluster;

  members += part->get_members();
  external_edges += part->external_edges;
  internal_edges += part->internal_edges;
  merge_priority = part->merge_priority;
  locked=false;

  // create partition matrix
  partition_init();
  for(int i = 0; i < rhop->cmgr->hw_cluster_vec.dim(); i++) {
    for(int j = 0; j <= rhop->max_estart; j++) {
      for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
	partition_load[i][j][k] = part->partition_load[i][j][k];
      }
    }
  }
}


Op_partition::Op_partition(Op_partition* part1, Op_partition* part2, RHOP* rhop_cluster) 
  : members(hash_op_ptr, 1024),
    external_edges(),
    internal_edges()
{
  id_rep = rhop_cluster->get_next_op_part_id_inc();
  rhop = rhop_cluster;
  locked=false;

  // combine members
  members += part1->get_members();
  members += part2->get_members();

  // combine and fix edges
  external_edges += part1->external_edges;
  external_edges += part2->external_edges;

  internal_edges += part1->internal_edges;
  internal_edges += part2->internal_edges;

  merge_priority = part1->merge_priority + part2->merge_priority;

  List_set<Edge*> to_remove;
  
  for(List_set_iterator<Edge*> lsi(external_edges); lsi != 0; lsi++){
    Edge * edge = *lsi;
    if(members.is_member(edge->src()) && members.is_member(edge->dest())) {
      //      external_edges -= edge;
      to_remove += edge;
      internal_edges += edge;
      merge_priority -= rhop->edge_weights.value(edge);
    }
  }
  
  external_edges -= to_remove;

  // create partition matrix
  partition_init();
  for(int i = 0; i < rhop->cmgr->hw_cluster_vec.dim(); i++) {
    for(int j = 0; j <= rhop->max_estart; j++) {
      for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
	partition_load[i][j][k] = part1->partition_load[i][j][k] + part2->partition_load[i][j][k];
      }
    }
  }
}

Op_partition::~Op_partition() 
{
  delete [] partition_load;
}

Hash_set<Op*> Op_partition::get_members() 
{
  return members;
}

Op* Op_partition::get_highest_weight_neighbor(Hash_set<Op*> &chosen_ops) 
{
  float max_weight = 0.;
  Edge* max_edge = NULL;

  for(List_set_iterator<Edge*> lsi(external_edges); lsi != 0; lsi++) {
    Edge* edge = *lsi;
    if(members.is_member(edge->src()) && chosen_ops.is_member(edge->dest()))
      continue;
    else if(members.is_member(edge->dest()) && chosen_ops.is_member(edge->src()))
      continue;	

    float weight = rhop->edge_weights.value(edge);
    if(weight > max_weight) {
      max_weight = weight;
      max_edge = edge;
    }      
  }

  if(max_edge == NULL)
    return NULL;
  
  if(members.is_member(max_edge->src()))
    return max_edge->dest();
  else
    return max_edge->src();
}

void Op_partition::print(ostream& os) const {
  os << "[" << id_rep << "] : " << cur_cluster << endl;

  os << "Members: ( ";
  for(Hash_set_iterator<Op*> opi(this->members); opi!=0; opi++) {
    Op* op = *opi;
    os << op->id() << ":" << op->opcode() << " ";
  }
  os << ") {" << merge_priority << "}" << endl;

  if(dbg(clst, 3)) {
    for(int m = 0; m < rhop->cmgr->hw_cluster_vec.dim(); m++) {
      os << "C" << m << ": | ";
      for(int n = 0; n < rhop->max_estart+1; n++) {
	if(partition_load[m][n][4] >= 10000)
	  os << setw(3) << "INF" << " | ";
	else
	  os << setw(3) << setprecision(2) << partition_load[m][n][0] << " | ";
      }
      os << endl;
    }
  }
}

int Op_partition::id() const {
  return id_rep;
}

void Op_partition::lock() {
  locked = true;
}

void Op_partition::unlock() {
  locked = false;
}

bool Op_partition::is_locked() {
  return (locked == true);
}

void Op_partition::set_current_cluster(int cluster) {
  cur_cluster = cluster;
}

int Op_partition::get_current_cluster() {
  return cur_cluster;
}

float Op_partition::get_merge_priority() {
  return merge_priority;
}

float Op_partition::get_edge_cost() {
  float weight = 0.0;

  for(List_set_iterator<Edge*> edgei(external_edges); edgei != 0; edgei++) {
    Edge* edge = *edgei;
    if(members.is_member(edge->src())) {
      if(rhop->clusters[cur_cluster]->is_member(edge->dest())) 
	continue;     
    }
    else if(members.is_member(edge->dest())) {
      if(rhop->clusters[cur_cluster]->is_member(edge->src())) 
	continue;
    }

    weight += rhop->edge_weights.value(edge);	
  }

  return weight;
}

List_set<Edge*>& Op_partition::get_external_edges() {
  return external_edges;
}

List_set<Edge*>& Op_partition::get_internal_edges() {
  return internal_edges;
}

bool Op_partition::is_member(Op* op) {
  return members.is_member(op);
}

int Op_partition::get_prebound_cluster() {
  bool is_prebound = false;
  int prebound_cluster = 0;

  for(int resource = 0; resource < RHOP_MAX_RES_TYPE; resource++) {
    for(int cluster = 0; cluster < rhop->cmgr->hw_cluster_vec.dim(); cluster++) {
      if(partition_load[cluster][0][resource] >= RHOP_INF) {
        is_prebound=true;
        continue;
      }
      if(is_prebound) {
        prebound_cluster = cluster;
      }
    }
    if(is_prebound) {
      if(dbg(clst,3))
        cdbg << "Partition " << id_rep << " is prebound to cluster " << prebound_cluster << endl;
      return prebound_cluster;
    }
  }


  for(int resource = 0; resource < RHOP_MAX_RES_TYPE; resource++) {
    for(int cluster = 0; cluster < rhop->cmgr->hw_cluster_vec.dim(); cluster++) {
      if(partition_load[cluster][rhop->max_estart][resource] >= RHOP_INF) {
        is_prebound=true;
        continue;
      }
      if(is_prebound) {
        prebound_cluster = cluster;
      }
    }
    if(is_prebound) {
      if(dbg(clst,3))
        cdbg << "Partition " << id_rep << " is prebound to cluster " << prebound_cluster << endl;
      return prebound_cluster;
    }
  }


  return -1;
}

ostream& operator<<(ostream& os, const Op_partition& op_part){
  op_part.print(os);
  return os;
}

ostream& operator<<(ostream& os, const Op_partition* op_part){
  op_part->print(os);
  return os;
}

// Initialize the Op_partition by creating the system_load matrix
void Op_partition::partition_init() {
  int num_clusters = rhop->cmgr->hw_cluster_vec.dim();

  partition_load = new double**[num_clusters];

  for(int cluster = 0; cluster < num_clusters; cluster++) {
    partition_load[cluster] = new double*[rhop->max_estart+1];
    for(int cycle = 0; cycle <= rhop->max_estart; cycle++) {
      partition_load[cluster][cycle] = new double[RHOP_MAX_RES_TYPE];
    }
  }

  for(int i = 0; i < num_clusters; i++) {
    for(int j = 0; j <= rhop->max_estart; j++) {
      for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
	partition_load[i][j][k] = 0.;
      }
    }
  }

  return;
}

unsigned hash_op_partition(Op_partition*& op_part)
{
  return(ELCOR_INT_HASH(op_part));
}


////////////////////////////////////////////////////////////////////////////
//
//  RHOP
//

RHOP::RHOP(Cluster_mgr* cmgr)
  : Cluster_algorithm(cmgr),
    dep_info(hash_op_ptr, HASH_SIZE_1),   
    total_node_weights(cmgr->hw_cluster_vec.size()),
    node_weights(hash_op_ptr, HASH_SIZE_1),    
    edge_weights(hash_edge_ptr, HASH_SIZE_1),
    slack_map(hash_op_ptr, HASH_SIZE_1),
    cluster_map(hash_op_ptr, HASH_SIZE_1),    
    clusters(cmgr->hw_cluster_vec.size())
{
  op_part_id_num = 1;
  current_load = -1.;

  RU_alloc_map(RHOP_MAX_RU_LENGTH);
  if (!is_init) {
    static_init();
    is_init = true;
  }

  for(int i = 0; i < cmgr->hw_cluster_vec.dim(); i++) 
    clusters[i] = new Hash_set<Op*>(hash_op_ptr, HASH_SIZE_2);
}

RHOP::~RHOP()
{ 
  RU_delete_map();

  for(int i = 0; i < cmgr->hw_cluster_vec.dim(); i++) 
    delete clusters[i];

  delete [] load;
}

void RHOP::static_init()
{
  find_global_move_weight();

  if (dbg(clst, 3))
    cdbg << "global move weight was " << move_weight << endl;
}

void RHOP::find_global_move_weight()
{
  char move_iopat[RHOP_MAX_IOPAT];
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
      El_punt("RHOP::find_global_move_weight: can't find two clusters supporting GPR.");
    }
  }
  cluster++;
  if (cluster >= (int)cmgr->hw_cluster_vec.size()) {
    El_punt("RHOP::find_global_move_weight: can't find two clusters supporting GPR.");
  }
  while ((dest = cmgr->hw_cluster_vec[cluster]->get_physical_file(GPR)) == "") {
    cluster++;
    if (cluster >= (int)cmgr->hw_cluster_vec.size()) {
      El_punt("RHOP::find_global_move_weight: can't find two clusters supporting GPR.");
    }
  }

  strcpy(move_iopat, vregfile_to_mdesname(PR));
  strcat(move_iopat, "? ");
  strcat(move_iopat, src);
  strcat(move_iopat, ":");
  strcat(move_iopat, dest);
  if (dbg(clst, 3))
    cdbg << "the iopat i'm using for finding move weight is " << move_iopat << endl;

  RU_init_map(false, RHOP_MAX_RU_LENGTH);
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
 

void RHOP::find_block_node_weights(Compound_region* rgn)
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

double RHOP::compute_node_weight(Op* op, int clstr)
{
   int count = 0;
   char* opc = get_mdes_opcode_string(op);
   char iopat[RHOP_MAX_IOPAT];
   bool found_flag;
   char* opc_dummy; int pri_dummy;
   
   build_io(iopat, op, clstr);

   //cdbg << opc << " [" << iopat << "]" << endl;

   // 8-23-06 mchu
   // RHOP needs to check that a specific op is valid on the cluster
   // if not, it is given infinite weight
   if(!(cmgr->hw_cluster_vec[clstr]->is_supported(op))) 
     return RHOP_INF;

   RU_init_map(false, RHOP_MAX_RU_LENGTH);

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
      return RHOP_INF;

   return 1. / (double) count;
}

void RHOP::setup_block(Compound_region* block)
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

   if (dbg(clst, 3)){
     cdbg << "total node weight vec: ";
     for (unsigned k=0; k<total_node_weights.size();k++)
       cdbg << total_node_weights[k] << " ";
     cdbg << endl;
   }
}

// you must have run the DepLen functions, find_block_max_estart,
// and insert_block_pseudo_defs_and_uses before calling this function.
void RHOP::get_slacks_and_ranges(Compound_region* block)
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

void RHOP::insert_block_pseudo_defs_and_uses(Compound_region* block)
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
	       if (dbg(clst, 3))
		 cdbg << "insert DEF " << def_op->id() << " for prebound opd "
		      <<opd<< " in op " << op->id() <<endl;

	       // give it a node weight Vector
	       for (int i=0; i<num_clusters; i++) {
		 if (i == cid)
		   wts[i] = 0;
		 else
		   wts[i] = RHOP_INF;
	       }
	       node_weights.bind(def_op, wts);

	    } else {
	       def_op = def_opds.value(opd);
	       if (dbg(clst, 3))
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
	       if (dbg(clst, 3))
		 cdbg << "insert USE " << use_op->id()<< " for prebound opd "<<opd<<" in op "
		      <<op->id() <<endl;

	       // give it a node weight Vector
	       for (int i=0; i<num_clusters; i++) {
		 if (i == cid)
		   wts[i] = 0;
		 else
		   wts[i] = RHOP_INF;
	       }
	       node_weights.bind(use_op, wts);

	    } else {
	       use_op = use_opds.value(opd);
	       if (dbg(clst, 3))
		 cdbg << "reuse USE " << use_op->id() << " for prebound opd in op " << op->id() << endl;
	    }

	    Edge* use_edge = new Reg_flow();
	    connect_ops(use_edge, op, opdii.get_port_num(), use_op, SRC1, 0, 0);
	    use_edge->set_latency(0);
         }
      }
   }
}

void RHOP::fix_defs_and_uses(Compound_region* block)
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

void RHOP::find_block_edge_weights(Compound_region* block)
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
     if (dbg(clst, 3))
       cdbg << *edge << " : src-es=" << src_estart << " lat="<<lat<<" dest-ls="
	    <<dest_lstart<<" gslk="<<dest_lstart-lat-src_estart<<endl;
     
     // HACK - want to put clear_all_rotating on same cluster as all_rot_pred.
     // do this by making edge wt from op to USE RHOP_INF.
     if (is_clear_all_rotating(edge->src()))
       edge_weights.bind(edge, RHOP_INF);
     
     else if (dest_lstart - lat - src_estart == 0)
       edge_weights.bind(edge, RHOP_HIGHEST_EDGE_WT);
     else if (El_rhop_edge_weights == EL_CLSTR_RHOP_CRIT)
       edge_weights.bind(edge, RHOP_LOW_EDGE_WT);
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

void RHOP::find_block_start_edges(Compound_region *block, 
				  Slist<Stuple<int, Edge*> >& edges_to_process)
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

void RHOP::process_edge_up(Edge* cur_edge, Slist<Stuple<int, Edge*> >& edges_to_process_up)
{
  // find my slack
  int from_estart = dep_info.value(cur_edge->src())->m_Estart;
  int orig_lat = cur_edge->latency();
  int to_lstart = dep_info.value(cur_edge->dest())->min_lstart();
  int gslack = to_lstart - orig_lat - from_estart;
  
  if (!edge_weights.is_bound(cur_edge)) {

    // if gslack >= move lat, edge lat += move lat
    if (gslack >= RHOP_MOVE_LATENCY) {

      if (El_rhop_edge_weights == EL_CLSTR_RHOP_FCFS2) {

	// 2 moves worth of slack gets LOW_WT
	if (gslack >= 2 * RHOP_MOVE_LATENCY) {
	  cur_edge->set_latency(orig_lat + 2 * RHOP_MOVE_LATENCY);
	  edge_weights.bind(cur_edge, RHOP_LOW_EDGE_WT);
	} else { // 1 move worth of slack gets MED_WT
	  cur_edge->set_latency(orig_lat + RHOP_MOVE_LATENCY);
	  edge_weights.bind(cur_edge, RHOP_MED_EDGE_WT);
	}

      } else {
	// any slack gets LOW_WT
	cur_edge->set_latency(orig_lat + RHOP_MOVE_LATENCY);
	edge_weights.bind(cur_edge, RHOP_LOW_EDGE_WT);
      }
    }

    else {
      edge_weights.bind(cur_edge, RHOP_HIGHER_EDGE_WT);
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

void RHOP::process_edge_down(Edge* cur_edge)
{
  // do i need to do anything?
}

void RHOP::find_block_max_estart(Compound_region* block)
{
  max_estart = 0;

  for(Region_all_ops opi(block); opi!=0; opi++) {
    Op* op = *opi;
    if (dep_info.is_bound(op)) {
      int cur_estart = dep_info.value(op)->m_Estart;
      if (cur_estart > max_estart)
	max_estart = cur_estart;

      int lstart = dep_info.value(op)->min_lstart();
      if (dbg(clst, 3))
	cdbg << "op " << op->id()<< " " << op->opcode() << " has estart=" << cur_estart
	     << " and lstart=" << lstart << endl;
    }
  }
  if (dbg(clst, 3))
    cdbg << "max_estart is " << max_estart << endl;
}

void RHOP::find_block_total_node_weights(Compound_region* block)
{
   for (unsigned clstr = 0; clstr < cmgr->hw_cluster_vec.size(); clstr++) {
      //int cycle = 0;
      int op_count = 0;
      char iopat[RHOP_MAX_IOPAT];
      bool found_flag;
      char* opc_dummy; int pri_dummy;

      RU_init_map(false, RHOP_MAX_RU_LENGTH);

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
	 } while (!found_flag && cycle < RHOP_MAX_RU_LENGTH);
      }

      total_node_weights[clstr] = (double) RU_cur_sched_length() / (double) op_count;
      //total_node_weights[clstr] = (double) (cycle+1) / (double) op_count;
   }
}

void RHOP::print_node_weights()
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

void RHOP::print_edge_weights()
{
   for (Hash_map_iterator<Edge*,float> hmi(edge_weights); hmi != 0; hmi++) {
      cdbg << *(*hmi).first << " : " << (*hmi).second << endl;
   }
}


void RHOP::set_control_dep_lat_to_zero(Compound_region* rgn)
{
  for (Region_all_ops opi(rgn); opi!=0; opi++) {
    for (Op_outedges ei(*opi, CONTROL1_OUTEDGES); ei!=0; ei++) {
      Edge* edge = *ei;
      edge->set_latency(0);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//
//  Main RHOP routine
//

void RHOP::do_block_setup_and_clustering(Compound_region* rgn)
{
   create_local_analysis_info_for_all_hbs_bbs(rgn);
   el_flow_compute_liveness(rgn, ANALYZE_ALLREG);
   insert_region_scalar_edges(rgn);

   set_control_dep_lat_to_zero(rgn);

   if (dbg(rhop, 2))
     cdbg << "\nbb " << rgn->id() << endl;

   setup_block(rgn);
   if (dbg(rhop, 3))
     print_edge_weights();
   if (dbg(rhop, 3))
     cdbg << "max_estart is " << max_estart << endl;

   // count up number of ops
   num_ops=0;
   for (Region_all_ops opi(rgn); opi != 0; opi++) {
     Op* op = *opi;
     if(!op->is_pseudo_op() || op->opcode() == DEF || op->opcode() == USE) {
       num_ops++;
     }
   }
   Vector<Op_partition*> partitions(num_ops);

   // initialize by forming a coarse op for each op in the region
   do_rhop_init(rgn, partitions);

   // recursively coarsen and refine
   do_rhop_coarsening(rgn, partitions);
   
   // do a final pass to refine all postitive moves
   do_rhop_final_pass(rgn, partitions);

   // clean up
   for(int i = 0; i < partitions.dim(); i++) 
     delete partitions[i];
   
   // remove DEFs and USEs that were inserted for prebound ops
   remove_block_pseudo_defs_and_uses(rgn);

   // *** DEBUG ***
   // currently, cluster everything to cluster 0
   /*
   for (Region_all_ops dbg_opi(rgn); dbg_opi!=0; dbg_opi++) {
     Op* op = *dbg_opi;
     if (is_pseudo(op)) continue;
     if (!cmgr->op_map.is_bound(op))
       cmgr->op_map.bind(op, 0);
   }
   */

   for(int j = 0; j < clusters.dim(); j++) {
     Hash_set<Op*> op_set = *clusters[j];
     for(Hash_set_iterator<Op*> hsi(op_set); hsi != 0; hsi++) {
       Op* op = (Op*) *hsi;
       cmgr->op_map.bind(op, j);
     }
   }

   for (Region_all_ops opi(rgn); opi != 0; opi++) {
     Op* op = *opi;
     if(op->is_pseudo_op()) 
       continue;
     
     int cur_cluster = cmgr->op_map.value(op);
     List<int>* feas_locations;

     // get a list of feasible locations for this op
     cmgr->collect_valid_clusters_op(op, feas_locations);

     // if the random cluster is not valid for this op, just use the first valid cluster
     if (!feas_locations->is_member(cur_cluster)) {
       El_warn("RHOP assigned Op %d to an invalid cluster %d.  Reassigning to %d\n", 
               op->id(), cur_cluster, feas_locations->head());
       cmgr->op_map.unbind(op);
       cmgr->op_map.bind(op, feas_locations->head());
     }          
   }

   // operand_assignment
   cmgr->do_block_operand_assignment(rgn);
   //cmgr->do_block_smarter_operand_assignment(rgn);

   delete_region_edges(rgn);
}

void RHOP::do_rhop_init(Compound_region* rgn, Vector<Op_partition*> &partitions) 
{
  int i = 0;
  for (Region_all_ops opi(rgn); opi != 0; opi++) {
    Op* op = *opi;
    if(!op->is_pseudo_op() || op->opcode() == DEF || op->opcode() == USE) {
      partitions[i] = new Op_partition(op, this);
      // debug
      //      cdbg << partitions[i] << endl;
      i++;
    }
  }

  int num_clusters = cmgr->hw_cluster_vec.dim();

  // initialize overall system load table for RHOP
  load = new double**[num_clusters];

  for(i = 0; i < num_clusters; i++) {
    load[i] = new double*[max_estart+1];
    for(int j = 0; j < max_estart+1; j++) {
      load[i][j] = new double[RHOP_MAX_RES_TYPE];
    }
  } 
}

void merge(Vector<Op_partition*> &partitions, Vector<Op_partition*> temp, 
	   int left, int mid, int right, bool size)
{
  int i, left_end, num_elements, tmp_pos;

  left_end = mid - 1;
  tmp_pos = left;
  num_elements = right - left + 1;

  while ((left <= left_end) && (mid <= right))
  {
    if (partitions[left]->get_merge_priority() >= partitions[mid]->get_merge_priority() && !size)
    {
      temp[tmp_pos] = partitions[left];
      tmp_pos = tmp_pos + 1;
      left = left +1;
    }
    else if(partitions[left]->get_members().size() <= partitions[mid]->get_members().size() && size)
    {
      temp[tmp_pos] = partitions[left];
      tmp_pos = tmp_pos + 1;
      left = left +1;	
    }
    else
    {
      temp[tmp_pos] = partitions[mid];
      tmp_pos = tmp_pos + 1;
      mid = mid + 1;
    }
  }

  while (left <= left_end)
  {
    temp[tmp_pos] = partitions[left];
    left = left + 1;
    tmp_pos = tmp_pos + 1;
  }
  while (mid <= right)
  {
    temp[tmp_pos] = partitions[mid];
    mid = mid + 1;
    tmp_pos = tmp_pos + 1;
  }

  for (i=0; i < num_elements; i++)
  {
    partitions[right] = temp[right];
    right = right - 1;
  }
}

void m_sort(Vector<Op_partition*> &partitions, Vector<Op_partition*> &temp, 
	    int left, int right, bool size)
{
  int mid;

  if (right > left)
  {
    mid = (right + left) / 2;
    m_sort(partitions, temp, left, mid, size);
    m_sort(partitions, temp, mid+1, right, size);

    merge(partitions, temp, left, mid+1, right, size);
  }
}


void RHOP::do_rhop_coarsening(Compound_region* rgn, Vector<Op_partition*> &partitions)
{
  bool progress = false;

  // check if coarsened enough
  if(partitions.dim() <= cmgr->hw_cluster_vec.dim()) {
    // place in initial (possibly bad) clusters
    for(int i = 0; i < partitions.dim(); i++) {
      *clusters[i] += partitions[i]->get_members();      
      partitions[i]->set_current_cluster(i);      
      for(int j = 0; j <= max_estart; j++) {
	for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
	  //	cdbg << "[" << i << "][" << j << "]: " << partitions[i]->partition_load[i][j] << endl;
	  load[i][j][k] = partitions[i]->partition_load[i][j][k];
	}
      }
    }

    // calculate initial system load;
    calculate_system_load(partitions);

    //    return;
  } 
  else { // try and coarsen them
    Hash_set<Op_partition*> chosen_partitions(hash_op_partition, HASH_SIZE_1);
    Hash_set<Op*> chosen_ops(hash_op_ptr, HASH_SIZE_1);

    int i = 0;
    int j = 0;
    int k = 0;

    // coarsen into coarsened_partitions Vector    
    Vector<Op_partition*> coarsened_partitions(partitions.size());
    
    // sort partitions Vector by weight
    Vector<Op_partition*> temp(partitions.size()); // temporary Vector used for merge sorting
    m_sort(partitions, temp, 0, partitions.size()-1, false);

    // put into coarsened_partitions
    for(i = 0; i < partitions.dim(); i++) {
      if(chosen_partitions.is_member(partitions[i]))
	continue;

      Op* to_merge_op = partitions[i]->get_highest_weight_neighbor(chosen_ops);
      
      if(to_merge_op == NULL) {
	chosen_partitions += partitions[i];
	chosen_ops += partitions[i]->get_members();
	coarsened_partitions[k] = new Op_partition(partitions[i], this);
	k++;

	if (dbg(rhop, 2))
	  cdbg << "Partition " << partitions[i]->id() << " no merging --> "
	       << coarsened_partitions[k-1]->id() << endl;		  


	continue;
      }

      for(j = i; j < partitions.dim(); j++) {
	if(partitions[j]->is_member(to_merge_op))
	  break;
      }

      chosen_partitions += partitions[i];
      chosen_partitions += partitions[j];
      chosen_ops += partitions[i]->get_members();
      chosen_ops += partitions[j]->get_members();

      coarsened_partitions[k] = new Op_partition(partitions[i],partitions[j],this);
      k++;

      if (dbg(rhop, 2))
	cdbg << "Partition " << partitions[i]->id() << " to merge with " 
	     << partitions[j]->id() << " --> " << coarsened_partitions[k-1]->id() << endl;

      progress = true;
    }

    // if there is no progress (i.e. all possible edges coarsened), coarsen smallest edges
    if(!progress) {
      k = 0;
      if (dbg(rhop, 2))
	cdbg << "No Progress!!!" << endl;
      m_sort(coarsened_partitions, temp, 0, partitions.size()-1, true);

      for(int n = 0; n < coarsened_partitions.dim(); n+=2) {
	if(n+1 == coarsened_partitions.dim()) {
	  coarsened_partitions[k] =  coarsened_partitions[n];
	  k++;
	  if (dbg(rhop, 2))
	    cdbg << "Partition " << coarsened_partitions[n]->id() << " no merging ";
	}
	else {
	  Op_partition* part1 = coarsened_partitions[n];
	  Op_partition* part2 = coarsened_partitions[n+1];

	  coarsened_partitions[k] = new Op_partition(part1, part2, this);
	  k++;

	  if (dbg(rhop, 2))	  
	    cdbg << "Partition " << part1->id() << " to merge with " 
		 << part2->id() << " --> " << coarsened_partitions[k-1]->id() << endl;	
	  
	  delete part1;
	  delete part2;
	}
      }

      //      return;
    }
    if (dbg(rhop, 2))
      cdbg << "*******************************************" << endl;

    // resize coarsened_partitions
    coarsened_partitions.resize(k);

    // recursively coarsen (next stage of coarsening)
    do_rhop_coarsening(rgn, coarsened_partitions);

    // clean up
    for(int p = 0; p < coarsened_partitions.dim(); p++) {
      //    cdbg << "deleting partition " << partitions[i]->id() <<  endl;    
      delete coarsened_partitions[p];
    }
    
  }

  // assign current cluster settings
  for(int x = 0; x < partitions.dim(); x++) {
    Op* op = partitions[x]->get_members().head();
    for(int y = 0; y < clusters.dim(); y++) {
      if(clusters[y]->is_member(op)) 
	partitions[x]->set_current_cluster(y);
    }
  }


  // refine coarsened partition
  do_rhop_refinement(rgn, partitions);


}

void RHOP::do_rhop_refinement(Compound_region* rgn, Vector<Op_partition*> &partitions) 
{
  do_rhop_move_prebound_partitions(partitions);

  // find imbalanced cluster
  int move_from = get_imbalanced_cluster();

  if (dbg(rhop, 3)) {
    cdbg << "Available Partitions: " << endl;

    for(int x = 0; x < partitions.dim(); x++) {    
      cdbg << "Partition " << partitions[x] << endl;    
    }
  }

  int next_move_from = -1;
  double gain_sum = 0;
  
  while(move_from != next_move_from) {
    if(next_move_from != -1) move_from = next_move_from;

    while(gain_sum >= 0 && clusters[move_from]->size() != 0) {
      int best_move = -1;
      int best_move_to = -1;
      double best_improvement = -1.0;      
      
      for(int i = 0; i < partitions.dim(); i++) {
	if(!(partitions[i]->get_current_cluster() == move_from)) 
	  continue;

	if(partitions[i]->is_locked()) 
	  continue;
	
	for(int j = 0; j < cmgr->hw_cluster_vec.dim(); j++) {
	  if(move_from == j) continue;	  

	  double new_system_load = test_move(partitions[i], move_from, j);
	  double improvement = current_load - new_system_load;	

	  if(dbg(rhop,2))
	    cdbg << "[" << partitions[i]->id() << "] --> " << j << " load: " << new_system_load << endl;

	  if(improvement > best_improvement || best_move == -1 ) {
	    best_move = i;
	    best_move_to = j;
	    best_improvement = improvement;
	  }
	}    
      }
      
      if((gain_sum + best_improvement) >= 0 && best_move != -1) {
	make_move(partitions[best_move], move_from, best_move_to);
	  
        edge_cost = 0.0;
        for(int x=0; x < partitions.dim(); x++) 
          edge_cost +=  partitions[x]->get_edge_cost();
        edge_cost /= 2;

	if(best_improvement < RHOP_INF) gain_sum += best_improvement;
      }
      else 
	break;
    }
    next_move_from = get_imbalanced_cluster();

  }
  if(dbg(rhop,2)) {
    print_current_load_balance();
    cdbg << "===========================================" << endl;
  }
}

void RHOP::do_rhop_final_pass(Compound_region* rgn, Vector<Op_partition*> &partitions) 
{
  if(dbg(rhop,2)) {
    cdbg << "Performing Final Pass " << endl;
  }

  int move_from = 0;
  int next_move_from = -1;
  double gain_sum = 0;

  while(move_from != next_move_from && next_move_from < clusters.dim()) {
    if(next_move_from != -1) move_from = next_move_from;    
    bool first = true;

    while((gain_sum > 0 || first) && clusters[move_from]->size() != 0) {
      first = false;
      int best_move = -1;
      int best_move_to = -1;
      double best_improvement = -1.0;      
      
      for(int i = 0; i < partitions.dim(); i++) {
	if(!(partitions[i]->get_current_cluster() == move_from)) 
	  continue;
	
	for(int j = 0; j < cmgr->hw_cluster_vec.dim(); j++) {
	  if(move_from == j) continue;	  

	  double new_system_load = test_move(partitions[i], move_from, j);
	  double improvement = current_load - new_system_load;	

	  if(dbg(rhop,2))
	    cdbg << "[" << partitions[i]->id() << "] --> " << j << " load: " << new_system_load << endl;

	  if(improvement > best_improvement || best_move == -1 ) {
	    best_move = i;
	    best_move_to = j;
	    best_improvement = improvement;
	  }
	}    
      }
      
      gain_sum = best_improvement;

      if(best_improvement > 0 && best_move != -1) {
	make_move(partitions[best_move], move_from, best_move_to);

        edge_cost = 0.0;
        for(int x=0; x < partitions.dim(); x++) 
          edge_cost +=  partitions[x]->get_edge_cost();
        edge_cost /= 2;

      }
    }

    next_move_from = move_from + 1;
  }
  if(dbg(rhop,2)) {
    calculate_system_load(partitions);
    cdbg << "===========================================" << endl;
  }
}

void RHOP::do_rhop_move_prebound_partitions(Vector<Op_partition*> &partitions) 
{
  int move_from, move_to;

  for(int i = 0; i < partitions.dim(); i++) {
    move_to =  partitions[i]->get_prebound_cluster();
    move_from = partitions[i]->get_current_cluster();

    if(move_to != -1 && (move_from != move_to)) {
      make_move(partitions[i], move_from, move_to);
      partitions[i]->unlock();
    }
  }

  edge_cost = 0.0;
  for(int x=0; x < partitions.dim(); x++) 
    edge_cost +=  partitions[x]->get_edge_cost();
  edge_cost /= 2;
}

int RHOP::count_icmoves(Op_partition* op_part, int move_from,  int move_to) {
  int moves = 0;
  Hash_set<El_ref> ref_set(hash_El_ref, HASH_SIZE_3);
  Hash_set<Op*> op_set(hash_op_ptr, HASH_SIZE_3);

  for(int i = 0; i < clusters.dim(); i++) {
    op_set += *clusters[i];
    if(i == move_from) op_set -= op_part->get_members();
    else if(i == move_to) op_set += op_part->get_members();

    for(Hash_set_iterator<Op*> opi(op_set); opi != 0; opi++) {
      Op* op = *opi;

      for (Op_inedges in(op, FLOW_EDGES); in!=0; in++) {
	Edge* edge = *in;
	Op* prod = edge->src();

	if (is_pseudo(prod)) continue;
	
	El_ref ref(prod, &(op->src(edge->dest_port())), EXP_SRC);

	if (!ref_set.is_member(ref) && !op_set.is_member(prod)) {
	  moves++;
	  ref_set += ref;
	}
      }
    }
    op_set.clear();
    ref_set.clear();
  }

  return moves;
}

int RHOP::get_imbalanced_cluster() 
{
  int imbalanced_clust = -1;
  double highest_load = -1.0;
  
  for(int i = 0; i < cmgr->hw_cluster_vec.dim(); i++) {
    double cluster_load = 0.0;
    for(int j = 0; j <= max_estart; j++) {
      double cycle_load = 0.0;
      for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
	if(load[i][j][k] > cycle_load)
	  cycle_load = load[i][j][k];
      }
      cluster_load += cycle_load;
    }    
    if(cluster_load > highest_load) {
      highest_load = cluster_load;
      imbalanced_clust = i;      
    }
  }

  return imbalanced_clust;
}

void RHOP::calculate_system_load(Vector<Op_partition*> &partitions) 
{
  current_load = 0.0;

  for(int j = 0; j <= max_estart; j++) {
    double highest_load = 0.0;
    for(int i = 0; i < cmgr->hw_cluster_vec.dim(); i++) {
      for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
	if(load[i][j][k] > highest_load)
	  highest_load = load[i][j][k];
      }
    }
    current_load += highest_load;   
  }

  current_load *= RHOP_HIGHEST_EDGE_WT;
  
  edge_cost = 0.0;

  for(int i=0; i < partitions.dim(); i++) 
    edge_cost +=  partitions[i]->get_edge_cost();

  edge_cost /= 2;

  current_load += edge_cost;

  if(dbg(rhop,2)) {  
    cdbg << "Calculating System Load: " << current_load << endl;
    print_current_load_balance();
  }

  return;
}

double RHOP::test_move(Op_partition* op_part, int move_from,  int move_to) {
  //  cdbg << "try partition " << op_part->id() << " on cluster " << move_to << endl;
  double move_load = 0.0;

  // j: iterate over the cycles
  for(int j = 0; j <= max_estart; j++) {
    double highest_load = 0.0;
    // i: iterate over the clusters
    for(int i = 0; i < cmgr->hw_cluster_vec.dim(); i++) {      
      // k: iterate over each resource per cluster
      for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
	if(i == move_from) {
	  if((load[i][j][k] - op_part->partition_load[i][j][k]) > highest_load)
	    highest_load = (load[i][j][k] - op_part->partition_load[i][j][k]);	  
	}
	else if(i == move_to) {
	  if((load[i][j][k] + op_part->partition_load[i][j][k]) > highest_load)
	    highest_load = (load[i][j][k] + op_part->partition_load[i][j][k]);	  
	}
	else {
	  if(load[i][j][k] > highest_load)
	    highest_load = load[i][j][k];
	}
      }
    }
    move_load += highest_load;   
  }

  move_load *= RHOP_HIGHEST_EDGE_WT;

  double edge_weight = edge_cost;

  List_set<Edge*> external_edges = op_part->get_external_edges();
  for(List_set_iterator<Edge*> lsi(external_edges); lsi!=0; lsi++) {
    Edge* edge = *lsi;
    
    if(op_part->is_member(edge->src())) {
      if(clusters[move_to]->is_member(edge->dest())) {
	edge_weight -= edge_weights.value(edge);
      }
      else if(clusters[move_from]->is_member(edge->dest())) {
      	edge_weight += edge_weights.value(edge);      
      }
    }
    else if(op_part->is_member(edge->dest())) {
      if(clusters[move_to]->is_member(edge->src())) {
	edge_weight -= edge_weights.value(edge);
      }
      else if(clusters[move_from]->is_member(edge->src())) {
	edge_weight += edge_weights.value(edge);      
      }
    }
  }
  move_load += edge_weight;

  float num_icmoves = count_icmoves(op_part, move_from, move_to);
  
  if(((num_icmoves*move_weight)-max_estart) > move_load) {
    if(dbg(rhop,2))    
      cdbg << "[move limited]";
    move_load = ((num_icmoves*move_weight)-max_estart);
  }
  return move_load;
}

void RHOP::make_move(Op_partition* op_part, int move_from,  int move_to) {
  if(dbg(rhop,2))
    cdbg << "moving " << op_part->id() << " from " << move_from << " to " << move_to << endl;
  
  for(int j = 0; j <= max_estart; j++) {
    for(int k = 0; k < RHOP_MAX_RES_TYPE; k++) {
      if(op_part->partition_load[move_from][j][k] > 0 || op_part->partition_load[move_to][j][k] > 0) {
	load[move_from][j][k] -= op_part->partition_load[move_from][j][k];	  
	load[move_to][j][k] += op_part->partition_load[move_to][j][k];	  
      }
    }
  }

  current_load = 0.0;

  for(int x = 0; x <= max_estart; x++) {
    double highest_load = 0.0;
    for(int y = 0; y < cmgr->hw_cluster_vec.dim(); y++) {
      for(int z = 0; z < RHOP_MAX_RES_TYPE; z++) {
	if(load[y][x][z] > highest_load)
	  highest_load = load[y][x][z];
      }
    }
    current_load += highest_load;   
  }

  current_load *= RHOP_HIGHEST_EDGE_WT;

  List_set<Edge*> external_edges = op_part->get_external_edges();
  for(List_set_iterator<Edge*> lsi(external_edges); lsi!=0; lsi++) {
    Edge* edge = *lsi;
    
    if(op_part->is_member(edge->src())) {
      if(clusters[move_to]->is_member(edge->dest())) 
        edge_cost -= edge_weights.value(edge);
      else if(clusters[move_from]->is_member(edge->dest())) 
	edge_cost += edge_weights.value(edge);      
    }
    else if(op_part->is_member(edge->dest())) {
      if(clusters[move_to]->is_member(edge->src())) 
	edge_cost -= edge_weights.value(edge);
      else if(clusters[move_from]->is_member(edge->src())) 
	edge_cost += edge_weights.value(edge);      
    }
  }

  current_load += edge_cost;

  *clusters[move_from] -= op_part->get_members();
  *clusters[move_to] += op_part->get_members();

  op_part->lock();
  op_part->set_current_cluster(move_to);  
}

void RHOP::remove_block_pseudo_defs_and_uses(Compound_region* rgn)
{
  Hash_set<Op*> pseudo_defs_and_uses(hash_op_ptr, HASH_SIZE_2);

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

void RHOP::print_current_load_balance() {
  for(int k = 0; k < cmgr->hw_cluster_vec.dim(); k++)
    cdbg << setw(4) << "C:" << k << "  ";
  cdbg << endl;

  for(int i = 0; i < max_estart+1; i++) {
    for(int j = 0; j < cmgr->hw_cluster_vec.dim(); j++){
      if(load[j][i][4] > 9999)
	cdbg << setw(5) << "INF";
      else
        cdbg << setw(5) << load[j][i][0];
      cdbg << "  ";
    }
    cdbg << endl;
  }
  cdbg << endl;
}

int RHOP::get_next_op_part_id_inc()
{
  return (op_part_id_num++);
}
