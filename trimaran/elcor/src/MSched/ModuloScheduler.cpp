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
//      File:           ModuloScheduler.cpp
//      Authors:        Vinod Kathail, Joel Jones, Shail Aditya
//      Created:        May 1994
//      Description:    Software Pipelining class implementation
//
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "ModuloScheduler.h"
#include "intf.h"
#include "matrix.h"
#include "opcode.h"
#include "sched_filters.h"
#include "graph_filters.h"
#include "connect.h"
#include "opcode_properties.h"
#include "el_control.h"
#include "meld.h"
#include "attributes.h"
#include "floyd.h"
#include "edge_utilities.h"
#include "vector.h"
#include "hash_functions.h"
#include "edge_drawing.h"
#include "rr_allocation.h"
#include "el_lsched_init.h"
#include "el_driver_init.h"
#include "dbg.h"
#include "mdes.h"
#include "ir_writer.h"
#include "el_dot_tools.h"
#include "width_utilities.h"
#include "el_clock.h"


using namespace std;

// ------------ SchedInfo class -------------------------------------------
SchedInfo::SchedInfo(Op *op) { 
  op_rep = op;
  stime = -1;
  estart = minus_infinity;
  placed = false;
  fixed = false;
  height = 0;
  specific_op = 0;
  op_priority = 0;
}

SchedInfo::~SchedInfo(){}

ostream& operator<<(ostream& os, SchedInfo *sInfo) {
  os << "start time: " << sInfo->stime << endl;
  os << "earliest start time: " << sInfo->estart << endl;
  os << "placed? " << sInfo->placed << endl;
  os << "fixed? " << sInfo->fixed << endl;
  os << "height " << sInfo->height << endl;
  os << "specific_op " << sInfo->specific_op << endl;
  os << "op_priority " << sInfo->op_priority << endl;
  return os;
}


//-------------------------- ModuloScheduler class ---------------------------
/* NAME: ModuloScheduler
 * INPUT: if-converted, back-substituted, load-store eliminated IR, budget for
 *  how many times to attempt to schedule
 * OUTPUT: IR annotated with the slot the operation is scheduled at
 * PROCESSING: calculate RecMII, ResMII, then using the resulting II as a 
 *  starting point to determine the real II and a corresponding schedule
 *  N.B. for now, this code only deals with the kernel of a DO loop
 */

ModuloScheduler::ModuloScheduler(LoopBody *loop_to_schedule, int ii_guess,
				 Tracware* tracker)
    : Budget(0), commited(false), the_ii(0),  good_ii(0), the_esc(0),
      meld(false), stretch(false), sInfoMap(NULL),
      nodeMap(hash_op_ptr, 1021), revnodeMap(hash_int, 1021), RUmap(NULL),
      delete_added_edges(true),
      Tracker(tracker), _free_tracware(false)
{
  assert(loop_to_schedule->is_loopbody());
  loopbody = loop_to_schedule;
  kernel = (Compound_region*)loopbody->kernel();
  commited = false;
  numnodes = 0;
  Real_op_filter real_op_filter;  // Ignore "Pseudo ops"

  // Initialize scheduling information map for each operation as well as  node
  // map for RecMII and height calculation. Compute number of real ops.
  sInfoMap = new Hash_map<Op*, SchedInfo*>(hash_op_ptr, 1021);

  // int i=0;
  for (Region_all_ops ops(kernel, &real_op_filter); ops != 0; ops++) {
    Op* op = *ops;
    revnodeMap.bind(numnodes, op);
    nodeMap.bind(op, numnodes++);
    sInfoMap->bind(op, new SchedInfo(op));
    // cdbg << (i++) << " op " << (*op) << endl;
  }
  // Initialize edge filter. It ignores all edges between pseudo ops  
  edge_filter = new Filter_and<Edge *>
                    (new Edge_is_contained_in_region_filter(kernel), 
                     new Real_edge_filter, true, true);
  // Allocate the resource-usage map.  allocates numnodes * (constant 
  // which  is assumed to be > maximum number of cycles for any instruction)
  RUmap = new RU(numnodes*MDES_max_op_lat);

  // Tracware:: 
  // Allocate the hardware tracking module if none was already assigned
  if (Tracker == NULL) {
      // flag to delete the tracker when finished
      _free_tracware = true;
      Tracker = new Tracware(this);
  }

  // Initialize the min-dist matrix for RecMII and height calculation
  minDist = new Matrix<int> (numnodes+1, numnodes+1, minus_infinity);
  nhbrs = new Matrix<int> (numnodes+1, numnodes+1, minus_infinity);

  // Add direct edges with omega (in scratch_int) between real ops
  added_edges = draw_direct_edges_with_omega(loopbody);
  if (dbg(ms,1)) {
    cdbg << "Additional edges are: " << endl;
    for (List_iterator<Edge*> edge_iter(*added_edges); edge_iter!=0; edge_iter++)
      cdbg << **edge_iter << endl;
  }

  manually_adjust_edges();

  // Now compute ResMII and RecMII. Then, set MII and initial II
  El_clock1.reset();
  El_clock1.start();
  the_resmii = calculate_ResMII(ii_guess);
  El_clock1.stop();
//   cdbg << "msched: " << loopbody->id() << " ResMII " << setprecision(10)
//        << El_clock1.total_elapsed() << endl;
  El_clock1.start();
  the_recmii = calculate_RecMII(the_resmii);
  El_clock1.stop();
//   cdbg << "msched: " << loopbody->id() << " RecMII " << setprecision(10)
//        << El_clock1.total_elapsed() << endl;
  the_mii = the_recmii > the_resmii ? the_recmii : the_resmii;
  if (dbg(ms,1)) 
    cdbg << "Target II = " << ii_guess << " RecMII = " << the_recmii
	 << " ResMII = " << the_resmii << endl;
}

void ModuloScheduler::reinit(int ii_guess)
{
  commited = false;
  numnodes = 0;
  Real_op_filter real_op_filter;

  // Initialize scheduling information map for each operation as well as  node
  // map for RecMII and height calculation. Compute number of real ops.
  if(sInfoMap)
    delete sInfoMap;
  sInfoMap = new Hash_map<Op*, SchedInfo*>(hash_op_ptr, 1021);

  for (Region_all_ops ops(kernel, &real_op_filter); ops != 0; ops++) {
    Op* op = *ops;
    revnodeMap.bind(numnodes, op);
    nodeMap.bind(op, numnodes++);
    sInfoMap->bind(op, new SchedInfo(op));
  }

  // Allocate the resource-usage map.  allocates numnodes * (constant 
  // which  is assumed to be > maximum number of cycles for any instruction)

  if(RUmap)
    delete RUmap;
  RUmap = new RU(numnodes * MDES_max_op_lat);

  // Tracware:: 
  // Allocate the hardware tracking module if none was already assigned
  if (Tracker == NULL) {
    // flag to delete the tracker when finished
    _free_tracware = true;
    Tracker = new Tracware(this);
  }

  // Initialize the min-dist matrix for RecMII and height calculation
  minDist = new Matrix<int> (numnodes+1, numnodes+1, minus_infinity);
  nhbrs = new Matrix<int> (numnodes+1, numnodes+1, minus_infinity);

  // Add direct edges with omega (in scratch_int) between real ops
  added_edges = draw_direct_edges_with_omega(loopbody);
  if (dbg(ms,1)) {
    cdbg << "Additional edges are: " << endl;
    for (List_iterator<Edge*> edge_iter(*added_edges); edge_iter!=0; edge_iter++)
      cdbg << **edge_iter << endl;
  }

  manually_adjust_edges();

  // Now compute ResMII and RecMII. Then, set MII and initial II 
  the_resmii = calculate_ResMII(ii_guess);
  the_recmii = calculate_RecMII(the_resmii);
  the_mii = the_recmii > the_resmii ? the_recmii : the_resmii;
  if (dbg(ms,1))
    cdbg << "Target II = " << ii_guess << " RecMII = " << the_recmii
	 << " ResMII = " << the_resmii << endl;
}

// Destructor 
ModuloScheduler::~ModuloScheduler(){
  delete_local_structures();
}

// Various functions to get RecMII etc.
int ModuloScheduler::ResMII(){
  return the_resmii;
}

int ModuloScheduler::RecMII(){
  return the_recmii;
}

int ModuloScheduler::MII(){
  return the_mii;
}

int ModuloScheduler::II(){
  return the_ii;
}

int ModuloScheduler::ESC(){
  return the_esc;
}

int ModuloScheduler::Numnodes(){
  return numnodes;
}

// --------------------- Add direct edges and compute ResMII, RecMII ----------
// Get all switches.
// For each switch 
//   For each op1 that feeds the switch
//       Trace edges until we hit a real op2
//       connect op1 and op2 with the same type of edge as op1 to switch
//       Latency = sum of all latency along the path
//       omega = number of switch and merge edges on the path
//  The complexity is in recursive traversal

List<Edge*>*  ModuloScheduler::draw_direct_edges_with_omega(LoopBody* loop){

    const Edge_filter* efilt = NULL;
    
    Real_op_filter real_ops;
    List<Edge*>* edge_list = new List<Edge *>;
    Edge* cf_edge = (loop->controlFlowBackEdges())->head();
    Op* branch = cf_edge->src();
    Alt_num cf_src_alt = cf_edge->src_alt();
    Alt_num cf_dest_alt = cf_edge->dest_alt();
    Op* one_switch;
    Edge* an_edge;

    bool omega_dep = false;
    Lcode_attribute_map* lc_map = get_lcode_attributes(loop->kernel());
    if (lc_map && lc_map->is_bound("OMEGA_DEP"))
      omega_dep = true;

    for (List_iterator<Op*> iter(branch->get_relops()); iter != 0; iter++){
	one_switch = *iter;
	// Relies on that a switch doesn't have a control-0 edge
	for (Op_inedges in(one_switch); in != 0; in++) {
	    an_edge = *in;
	    // initialize scratch field -- Shail Aditya 11/06/98 
	    an_edge->scratch_int = 0;
	    if (is_pseudo(an_edge->src())) continue;
	    else {
	      // Conduct a DFS traversal from the switch
	      if (FLOW_EDGES->filter(an_edge))   efilt = FLOW_EDGES;
	      if (ANTI_EDGES->filter(an_edge))   efilt = ANTI_EDGES;
	      if (OUTDEP_EDGES->filter(an_edge)) efilt = OUTDEP_EDGES;
	      if (MEM_EDGES->filter(an_edge))    efilt = MEM_EDGES;

	      for (Region_all_ops op(loop); op != 0; op++)
		(*op)->reset_flag(EL_OPER_MARK1);
	      draw_direct_edges_aux(one_switch, an_edge->latency(), 0,
                                    an_edge, cf_src_alt, cf_dest_alt,
                                    efilt, edge_list, omega_dep);
	    }
	}
    }
    return edge_list;
}

void ModuloScheduler::draw_direct_edges_aux(Op* src, int lat, int omega,
					    Edge* initial, Alt_num cf_src_alt,
					    Alt_num cf_dest_alt,
					    const Edge_filter* efilt,
					    List<Edge*>*  edge_list, bool omega_dep){
    Edge* current;
    Op* dest;

    if (src->flag(EL_OPER_MARK1)) return;
    src->set_flag(EL_OPER_MARK1);
    if (is_switch(src)){ 
	// Assumes switches are connected only to merges.
        // call recursively with incremented omega and sum of latencies
	for (Op_outedges out(src, efilt); out != 0; out++){
	    current = *out;
	    // initialize scratch field -- Shail Aditya 11/06/98 
	    current->scratch_int = 0;
	    if (current->src_alt() == cf_src_alt){
		draw_direct_edges_aux(current->dest(), 
				      lat + current->latency(),
				      omega + 1, initial, 
				      cf_src_alt, cf_dest_alt,
				      efilt, edge_list, omega_dep);
	    }
	}
    }
    else { // Must be a merge
	for (Op_outedges out(src, efilt); out != 0; out++){
	    current = *out;
	    // initialize scratch field -- Shail Aditya 11/06/98 
	    current->scratch_int = 0;
	    dest = current->dest();
	    if (!is_pseudo(dest)){
	      // found a real op, create new summary edge
		Edge* new_edge = initial->clone();
		connect_ops(new_edge, initial->src(), initial->src_port(),
			    dest, current->dest_port(), 
			    cf_src_alt, cf_dest_alt); 
		new_edge->set_latency(lat + current->latency());
		int new_lat = adjust_edge_latency_modulo(new_edge);
		new_edge->set_latency(new_lat);
		// new edges record omega
		new_edge->scratch_int = omega;

                // if edge is a mem_edge, we need to add in the distance of the
                // memory dependence.  -KF 3/2006
                if (omega_dep && new_edge->is_mem()) {
                  // need to get the mem_vr operand.  this is hard.  all mem_vrs
                  // share the same port_num and alt_num so you can't directly
                  // get the right one.  so, get the mem_srcs and mem_dests,
                  // take the intersection, and hope that the first element is
                  // the right one.
                  Hash_set<Operand> srcs = dest->mem_srcs();
                  Hash_set<Operand>& dests = new_edge->src()->mem_dests();
                  srcs *= dests;
                  if (srcs.size() == 0)
                    El_punt("Could not find Mem_vr corresponding to mem edge from op %d to op %d",
                            new_edge->src()->id(), dest->id());
                  if (srcs.size() > 1)
                    El_warn("Don't know how to handle multiple mem_vrs");
                  Operand mem_vr = srcs.head();
                  assert(mem_vr.is_mem_vr());
                  if (dbg(status,2) || dbg(ms,1))
                    cdbg << "Replacing scratch_int on edge " << *new_edge << " with "
                         << mem_vr.omega() << endl;
                  if (mem_vr.omega() < new_edge->scratch_int)
                    El_warn("Mem_vr omega is less than the original scratch_int!\n"
                            "Don't be surprised if msched hangs.");
                  new_edge->scratch_int = mem_vr.omega();
                }
		edge_list->add_head(new_edge);
	    }
	    else {
		draw_direct_edges_aux(dest, lat + current->latency(),
				      omega, initial, cf_src_alt, cf_dest_alt, 
				      efilt, edge_list, omega_dep);
	    }
	}
    }
}

/* NAME: ResMII
 * INPUT: if-converted, back-substituted, load-store eliminated IR
 * OUTPUT: integer representing the maximum II possible given the resource 
 *  bounds
 * PROCESSING: calculate the maximum of the operations performed on each class
 *  of resource
 */
int ModuloScheduler::calculate_ResMII(int guess){
  char iopat[MAXIOPATLEN];
  Real_op_filter real_op_filter;
  RMSL_alloc(guess);
  for (Region_all_ops opi(kernel, &real_op_filter); opi != 0; opi++) {
    /* present the op to RMSL */
    Op* op = *opi;

    if(get_mdes_opcode_string(op)) {
      op->build_io_pat(iopat);
      RMSL_nextop(get_mdes_opcode_string(op), iopat);
    }
  }
  int resmii = RMSL_value();
  RMSL_dealloc();
  return resmii;
}


/* NAME: RecMII
 * OUTPUT: RecMII, representing the minimum II which will satisfy the recursive
 *  expression constraints
 * PROCESSING: calculate the maximum latency of all elementary circuits, where
 *  the latency of a circuit is defined as the ceiling of the latency of all 
 *  operations in the circuit divided by the number of iterations spanned by 
 *  the circuit
 */
int ModuloScheduler::calculate_RecMII(int guess){
  int lowlim = 1;
  int uplim = guess;
  assert(guess>=1);         // guess must be at least 1

  // Binary doubling to find a failure and a success point
  while (failFloyd(uplim)){
    if (dbg(ms,1)) cdbg << "Failed Floyd (limit search) at " << uplim << endl;
    if (dbg(ms,1)) print_failed_cycles(uplim, cdbg);
    if (dbg(ms,5)) cdbg << "Mindist matrix (limit search):" << endl << *minDist;
    lowlim = uplim;
    uplim = 2*uplim;
  }

  // Binary search for RecMII  
  int recmii = (uplim + lowlim) / 2;
  while (uplim != lowlim) {
    if (failFloyd(recmii)) {
      lowlim = recmii + 1;
      if (dbg(ms,1)) cdbg << "Failed Floyd (binary search) at " << recmii << endl;
      if (dbg(ms,1)) print_failed_cycles(recmii, cdbg);
      if (dbg(ms,5)) cdbg << "Mindist matrix (binary search):" << endl << *minDist;
    } else {
      uplim = recmii;
    }
    recmii = (uplim + lowlim) / 2;
  }
  if (dbg(ms,3)) print_longest_cycles(uplim, cdbg);
  /*
  cdbg << "LONGEST CYCLE\n";
  print_longest_cycles(uplim, cdbg);
  */
  return recmii;
}

/* NAME: failFloyd
 * INPUT: compute the matrix of minimum distances for the specified II.  return
 *  true if there is a positive cycle.
 * OUTPUT:
 * PROCESSING:
 */

bool ModuloScheduler::failFloyd(int II) {
  int delay;
  Real_edge_filter real_edge_filter;
  minDist->set(minus_infinity);
  nhbrs->set(minus_infinity);

  // initialize the matrix with the one-step distances between nodes that
  // have an edge between them.
  Edge* edge;
  int src_index;
  int dest_index;
  for (Region_all_internal_edges edges(loopbody->kernel(), &real_edge_filter); 
       edges != 0; edges++) {
      edge = *edges;
      delay = edge->latency() - II * edge->scratch_int;
      src_index = nodeMap.value(edge->src());
      dest_index = nodeMap.value(edge->dest());
      if (delay > (*minDist)[src_index][dest_index]) {
	  (*minDist)[src_index][dest_index] = delay;
	  (*nhbrs)[src_index][dest_index] = src_index; 
      }
  }
  // set up a STOP pseudo node which all real nodes have a zero latency 
  // delay to
  for (int i = 0; i < numnodes; i++) {
    (*minDist)[i][numnodes] = 0;
  }
  // that was the easy part.  Now let someone else do the the hard work
  if (dbg(ms,5)) cdbg << "Matrix before Floyd:" << endl << *minDist;
  bool result = floyd(*minDist, *nhbrs);

  if (dbg(ms,1)) { 
     cdbg << endl << "Searching for positive cycles for II=" << II << endl; 
     print_cycles(cdbg);
     print_longest_paths(cdbg);
  }

  return result;
}

// Prints the positive cycles that cause the failure of failFloyd
// algorithm above at a particular II.
void ModuloScheduler::print_failed_cycles(int II, ostream& os) {
  for (int i=0; i<numnodes; i++) 
    if ((*minDist)[i][i] > 0) {
      os << "II = " << II << " : Found positive cycle with weight " 
	 << (*minDist)[i][i] << endl;
      os << "\t" << revnodeMap.value(i)->id();
      print_failed_cycles_aux(os, i,i);
      os << endl;
    }
}

// prints the longest cycle for each node after running the floyd algorithm
void ModuloScheduler::print_longest_cycles(int II, ostream& os) {
  for (int i=0; i<numnodes; i++) 
    if ((*minDist)[i][i] > minus_infinity) {
      os << "Weight: " << (*minDist)[i][i] << " :\t" << revnodeMap.value(i)->id();
      print_failed_cycles_aux(os, i,i);
      os << endl;
    }
}  

void ModuloScheduler::print_failed_cycles_aux(ostream& os, int fromnode, int tonode) {
  int intermnode = (*nhbrs)[fromnode][tonode];
  // if there is a direct edge between fromnode and tonode, output it
  if (intermnode == fromnode) {
    os << " --";
    Real_edge_filter real_edge_filter;
    Edge* edge;
    for (Region_all_internal_edges edges(loopbody->kernel(), &real_edge_filter); 
         edges != 0; edges++) {
      edge = *edges;
      if(nodeMap.value(edge->src()) == fromnode && nodeMap.value(edge->dest()) == tonode) {
        os << "(" << edge->latency() << "," << edge->scratch_int << ")";
      }
    }
    os << "--> " << revnodeMap.value(tonode)->id();
  }
  else {
    // otherwise, recurse with the intermediate node
    print_failed_cycles_aux(os, fromnode, intermnode);
    print_failed_cycles_aux(os, intermnode, tonode);
  }
}


// prints all cycles
void ModuloScheduler::print_cycles(ostream& os) {
   os << "\nPrinting all cycles:\n";
   for (int i=0; i<numnodes; i++) {
      int delay = (*minDist)[i][i];
      if (delay != minus_infinity) {
	 os << "Cycle delay = " << delay << ": " ;
	 print_path(i,i,os);
	 Op *dest = revnodeMap.value(i);
	 os << dest->id() << ":" << dest->opcode() << "\n\n";
      }
   }
}


// prints the longest path for each node
void ModuloScheduler::print_longest_paths(ostream& os) {
   os << "\nPrinting a longest path for each OP:\n";
   for (int i=0; i<numnodes; i++) {
      // find longest path
      int longest_path_node;
      int longest_path_length = minus_infinity;
      // Find a maximum length path.  This takes the first one.
      for (int j=0; j < numnodes; j++) {
	 if ((*minDist)[i][j] > longest_path_length) {
	    longest_path_node = j;
	    longest_path_length = (*minDist)[i][j];
	 }
      }
      // Some nodes might not have any out edges, so don't print them
      if (longest_path_length > minus_infinity) {
	 os << "Path delay = " << (*minDist)[i][longest_path_node] << ": ";
	 print_path(i,longest_path_node,os);
	 Op *dest = revnodeMap.value(longest_path_node);
	 os << dest->id() << ":" << dest->opcode() << "\n\n";
      }
   }
}




// prints a path found in floyd, from node i upto but not including node k
void ModuloScheduler::print_path(int i, int k, ostream& os) {
   int j = (*nhbrs)[i][k];

   if (j >= 0 && j != i) {
      print_path(i,j,os);
      print_path(j,k,os);
   }
   else {
      Op *src = revnodeMap.value(i);
      int delay = (*minDist)[i][k];
      os << src->id() << ":" << src->opcode() << " --(";
      if (delay == minus_infinity)
	 os << "INF";
      else os << delay;
      os << ")--> ";
   }
}


// --------------------- schedule and related functions -----------------------
// schedule is responsible for attempting to find a schedule with an II of
// potentialII
// It does this by unscheduling any already scheduled ops by
// removing them from the resource map, recalculating the priority and placing
// all the ops into the list of unscheduled ops, calling iterative_scheduler
// to do the actual scheduling, then finding the length of the schedule
// based upon the selected meld model and returning whether or not a schedule
// was found

bool ModuloScheduler::schedule(int potentialII, int budget_ratio, 
			       bool meld_allowed, bool stretch_scheduling)
{
  Budget = budget_ratio * numnodes;
  meld = meld_allowed;
  stretch = stretch_scheduling;
  if (!check_if_schedule(potentialII)) return false;
  the_ii = potentialII;

  //cdbg << "*** II is " << the_ii << endl;

  init_sched_state();
  
  if(El_msched_algorithm == ITERATIVE_MSCHED) {
    iterative_scheduler();    // Iteratively schedule nodes
  } else if(El_msched_algorithm == SWING_MSCHED) {
    El_clock1.reset();
    El_clock1.start();
    swing_scheduler();        // Different algorithm
    El_clock1.stop();
//     cdbg << "msched: " << loopbody->id() << " sched " << setprecision(10)
// 	 << El_clock1.total_elapsed() << endl;
  }


  // If scheuling fails, don't update esc, good_ii and IR
  if (!unscheduledNodes.is_empty()) {
    return false;
  }

  // If we're doing MVE, the branch can't be placed until all other
  // ops are scheduled, so we do it here.
  if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
    Region_exit_ops exits(kernel);
    Op* brn = *exits;

    // This is a sanity check, as I don't know if any other type of
    // branch is possible.
    assert(is_brct(brn));

    // The branch must at a cycle where all of its predecessors have
    // completed and ((cycle) % II) == 0 for the code to execute
    // correctly.
    int sched_time = 0;

    for(Op_inedges ine(brn, REAL_EDGES); ine != 0; ine++) {
      Edge* edge = *ine;
      Op* src = edge->src();
      if( ! is_pseudo(src)) {
	int this_estart = (*sInfoMap)[src]->stime + edge->latency();
	if(this_estart > sched_time) {
	  sched_time = this_estart;
	}
      }
    }
    
    sched_time += (II() - (sched_time % II()));

    // FIX: this is wrong. I assume there's a scheduling spot free for
    // the branch without ever reserving it in the schedule. This
    // works fine for IFMB machines, because the branch slot is never
    // used, but it won't work for machines without a dedicated branch
    // unit.
    place_branch(brn, sched_time);
    // RU_print_map(stddbg);
  }

  good_ii = the_ii;         // This is a valid II
  put_sched_info_in_ir();   // Put information in IR
  calculate_esc();          // Calculate schedule length, esc
  return true;
}

// Check if we should continue scheduling
bool ModuloScheduler::check_if_schedule(int ii){
    if (commited){
 	El_warn("Modulo Scheduler: Schedule is already commited. Quiting.");
	return false;
    }
    if (ii < the_mii){
	El_warn("Modulo Scheduler: Given II %d is less than MII %d. Quiting.",
                ii, the_mii);
	return false;
    }
    return true;
}	    

// Initialize scheduling state for new II
void ModuloScheduler::init_sched_state()
{
  // Initialize  the resource-usage map to be of type loop and length II
  RUmap->init(LOOP,the_ii);
  // Tracware:: init the hardware tracking system
  Tracker->init(this);

  // empty the list of unscheduled ops
  unscheduledNodes.clear();
  // compute the minDist matrix for the current value of II. This is to
  // calculate height -- MAKE height calculation more effieient.
  failFloyd(the_ii);
  // Reset op scheduling data and put all ops in unscheduled list
  Real_op_filter real_op_filter;
  Op* op;
  int height;
  if(El_msched_algorithm == ITERATIVE_MSCHED) {
    for (Region_all_ops ops(kernel, &real_op_filter); ops != 0; ops++) {
      op = *ops;
      if (is_swp_branch(op)) {
	place_branch(op, the_ii);
      } else {
	height = -CalcHeight(op);
	SchedInfo* info = sInfoMap->value(op);
	info->estart = -1;
	info->height = height;
	info->stime = -1;
	info->specific_op = NULL;
	info->placed = false;
	Stuple<int, Op *> item(height, op);
	// If we're doing MVE, then the branch needs to be placed
	// last, after all its dependant operations have been scheduled.
	if( ! is_branch(op)) {
	  unscheduledNodes.add(item);
	}
      }
    }
  } else if(El_msched_algorithm == SWING_MSCHED) {

    Hash_map<Op*, int> priorities(hash_op_ptr, 71);
    El_clock1.reset();
    El_clock1.start();
    CalcSwingPriorities(priorities);
    El_clock1.stop();
//       cdbg << "msched: " << loopbody->id() << " priorities " << setprecision(10)
// 	   << El_clock1.total_elapsed() << endl;

    for (Region_all_ops ops(kernel, &real_op_filter); ops != 0; ops++) {
      op = *ops;
      if (is_swp_branch(op)) {
	place_branch(op, the_ii);
      } else {
	// cdbg << "Looking up priority for op " << op->id() << endl;
	height = -priorities[op];
	SchedInfo* info = sInfoMap->value(op);
	info->estart = -1;
	info->height = height;
	info->stime = -1;
	info->specific_op = NULL;
	info->placed = false;
	Stuple<int, Op *> item(height, op);
	// If we're doing MVE, then the branch needs to be placed
	// last, after all its dependant operations have been scheduled.
	if( ! is_branch(op)) {
	  unscheduledNodes.add(item);
	}
      }
    }
  } else {
    El_punt("I don't know what MSched priority function to use!\n");
  }
}


// Place the branch at the_ii. Assumes latency = 1. Also, the code is taken
// the way currently ops are placed--works but inefficient.
void ModuloScheduler::place_branch(Op* op, int time)
{
  List<pattern> patterns;
  
  RUmap->get_nonconflicting_patterns(op, time - 1, &patterns);
  // Assumes pattern list is not empty
  // arbitrarily assign node to the first pattern (unit specific op)
  RUmap->place(op, patterns.head()->op, patterns.head()->priority, time -1);
  SchedInfo* info = sInfoMap->value(op);
  info->specific_op = patterns.head()->op;
  info->op_priority = patterns.head()->priority;
  info->stime = time;   // stime is 1-based
  info->fixed = true;
  info->placed = true;
}


// Assumes branch latency = 1.
void ModuloScheduler::calculate_esc()
{
  Real_op_filter real_op_filter;
  int length = minus_infinity;
  Op* op;
  int issue_time;
  for (Region_all_ops ops(kernel, &real_op_filter); ops != 0; ops++) {
    op = *ops;
    issue_time = op->sched_time();
    if (meld) {
      length = ELCOR_MAX(length, issue_time);
    }
    else {
      int max_latency = 0;
      for (int dpos = op->first_dest(); dpos <= op->last_dest(); dpos++) {
	// last cycle to consider
	max_latency = ELCOR_MAX(max_latency, op->flow_time(DEST, dpos) - 1);
      }
      for (int spos = op->first_input(); spos <= op->last_input(); spos++) {
	// last cycle to consider
        max_latency = ELCOR_MAX(max_latency, op->anti_time(SRC, spos) - 1);
      }
      length = ELCOR_MAX(length, issue_time + max_latency);
    }
  }
  length++;    // Convert the finish time (zero based) to the actual length 
  int sc = length / good_ii;
  if (sc*good_ii == length) the_esc = sc -1;
  else the_esc = sc;
}

// Put information in IR if schedule is successful -- Assumes loop body (i.e.,
// kernel ) contains only ops and no compound regions (e.g., BBs)
void ModuloScheduler::put_sched_info_in_ir(){
  
  Region_subregions_bidir ops(kernel, true);
  Op* branch = (Op*)*ops;                                       // branch op
  ops--;
  SchedInfo* branch_info = sInfoMap->value(branch);
  int branch_time = (branch_info->stime - 1);  
  //                      + the_esc * good_ii           shift branch time
  branch->set_sched_time(branch_time); 
  branch->set_sched_opcode(branch_info->specific_op);
  
  // Now process all other ops
  Op* op;
  SchedInfo* info;
  for ( ; ops != 0; ops--) {
      op = (Op*) *ops;
      // The first two clauses are for my sanity
      if (is_control_merge(op) || is_merge(op))
	  op->set_sched_time(0);
      else if (is_switch(op) || is_remap(op))
	  op->set_sched_time(branch_time);
      else {
	  info = sInfoMap->value(op);
	  op->set_sched_time(info->stime - 1);
	  op->set_sched_opcode(info->specific_op);
      }
  }
}

// --------------------- Iterative scheduling --------------------------------
void ModuloScheduler::iterative_scheduler(){
  int curbudget = Budget;
  int MinTime, SchedSlot;
  Op *node;
  SchedInfo *sInfo;
  List<pattern> patterns;
  Stuple<int, Op*> snode;
  // static int II_=0;

  /* while there are nodes to be scheduled and Budget isn't exhausted */
  while (!unscheduledNodes.is_empty() && (curbudget > 0)) {
		
    snode = unscheduledNodes.head();
    node = snode.second;
    if (dbg(ms,1))
      cdbg << endl << "scheduling op " << node->id() << endl;
    sInfo = sInfoMap->value(node);

    /* estart is the earliest start for this node as constrained by */
    /* already scheduled predecessors. */
    // CalcEstart(node, &sInfo->estart);
    // MinTime = sInfo->estart > 1 ? sInfo->estart : 1;
    // MaxTime = MinTime + the_ii - 1;

		/* Tracware:: select time and pattern for scheduling current node */
    Tracker->get_sched_info(unscheduledNodes, sInfo, SchedSlot);

    /* If a legal slot was not found, then pick (in decreasing order of */
    /* priority) the first available option from the following: */
    /*   - MinTime, if this is the first time that this node is being */
    /*     scheduled, or if MinTime is greater than stime, where stime is */
    /*     the time at which this node was last scheduled */
    /*   - at stime + 1 */
    if (SchedSlot == 0) {
      // cdbg << "schedslot was not found for op " << node->id() << endl;
      CalcEstart(node, &sInfo->estart);
      MinTime = sInfo->estart > 1 ? sInfo->estart : 1;
      if ((sInfo->stime == -1) || (MinTime > sInfo->stime)) {
        SchedSlot = MinTime;
      } else {
        SchedSlot = sInfo->stime + 1;
        SchedSlot = SchedSlot - II() * ((SchedSlot - MinTime) / II());
      }
    }
    if (dbg(ms,1)) {
      cdbg << "Trying to place node "  << snode.second->id()
	   << " at time " << (SchedSlot-1) <<"("<< ((SchedSlot-1)%II())<< ")"
	   << " on alternate " <<
                  ((sInfo->specific_op)?(sInfo->specific_op):"NULL") << endl;
      if (dbg(ms,3)) cdbg << *(snode.second) << endl;
    }

    put(snode, SchedSlot);

    /*
    if(II_ > 19690 || II_ < 5) {
     cdbg << "II_=" <<II_ << endl;
     cdbg << "AFTER PUT"<<endl;
     RU_print_map(stddbg);
    }
    II_++;
    */

    
    if (!(sInfo->placed)) {
      sInfo->stime = sInfo->stime + 1;
      if (dbg(ms,2)) {
	cdbg << "Can't place node\n";
      }
    }
    /* print statement for printing out the resource map after each */
    /* attempt to schedule an Op at a particular time */
    // if (dbg(ms,4)) RU_print_map(stddbg);
    // if (dbg(ms,1)) RU_print_map(stddbg);
    curbudget--;
  }
}


// --------------------- Swing scheduling --------------------------------
static void construct_op_order_map(Compound_region *region, Hash_map<Op*,int> &op_order)
{
  op_order.clear();

  int index = 0;
  for (Region_all_ops opi(region); opi!=0; opi++) {
    Op *op = *opi;
    op_order.bind(op, index);
    index++;
  }
}


int ModuloScheduler::swing_estart(Op* op, const Hash_map<Op*, int>& op_order)
{
  int estart = minus_infinity;
  for(Op_inedges ine(op, REAL_EDGES); ine != 0; ine++) {
    Edge* edge = *ine;
    Op* src = edge->src();
    if( ! is_pseudo(src)) {
      // ignore backedges
      if(op_order[src] < op_order[op]) {
	if((*sInfoMap)[src]->placed) {
	  int this_estart = (*sInfoMap)[src]->stime + edge->latency() -
	    (edge->scratch_int * II());
	  if(this_estart > estart && this_estart > 0) {
	    estart = this_estart;
	  }
	}
      }
    }
  }
  return estart;
}


int ModuloScheduler::swing_lstart(Op* op, const Hash_map<Op*, int>& op_order)
{
  int lstart = infinity;
  for(Op_outedges oute(op, REAL_EDGES); oute != 0; oute++) {
    Edge* edge = *oute;
    Op* dest = edge->dest();
    if( ! is_pseudo(dest)) {
      // ignore backedges
      if(op_order[dest] > op_order[op]) {
	if((*sInfoMap)[dest]->placed) {
	  int this_lstart = (*sInfoMap)[dest]->stime - edge->latency() +
	    (edge->scratch_int * II());
	  if(this_lstart < lstart) {
	    lstart = this_lstart;
	  }
	}
      }
    }
  }
  return lstart;
}


void ModuloScheduler::swing_scheduler()
{
  int slot;
  Op *node;
  SchedInfo *sInfo;
  Stuple<int, Op*> snode;

  Hash_map<Op*, int> asap_times(hash_op_ptr, 71);
  CalcASAP(asap_times);

  Hash_map<Op*, int> op_order(hash_op_ptr, 71);
  construct_op_order_map(kernel, op_order);

  // Find which ops are used for computation
  List_set<Op*> non_computation_nodes;

  /* while there are nodes to be scheduled and Budget isn't exhausted */
  while ( ! unscheduledNodes.is_empty()) {
		
    snode = unscheduledNodes.head();
    node = snode.second;
    if (dbg(ms,1))
      cdbg << endl << "scheduling op " << node->id() << " " 
	   << node->opcode() << endl;

    sInfo = sInfoMap->value(node);

    Hash_set<Op*> conflicts(hash_op_ptr, 16);
    bool found_a_slot = false;

    int min_time, max_time;

    min_time = swing_estart(node, op_order);
    max_time = swing_lstart(node, op_order);

    // If true, look for a slot from min to max, else go from max to min.
    bool search_forward;
    if(min_time == minus_infinity) {
      // This means no predecessors have been scheduled

      if(max_time == infinity) {
	// This means no successors have been scheduled either.
	min_time = asap_times[node];
	max_time = min_time + II() - 1;
	if(dbg(ms,2)) {
	  cdbg << "* ALAP\n";
	}

	// FIX: The paper says this should search forward, but this
	// can lead to impossible schedules, because the ASAP
	// calculation doesn't take resource restrictions into
	// account. Searching backwards makes SC longer than it should
	// be, though. Maybe the correct fix is to make ASAP look at
	// resources?
	search_forward = false;
      } else {
	// Successors have been scheduled
	if(dbg(ms,2)) {
	  cdbg << "* Only SUCC\n";
	}
	min_time = swing_lstart(node, op_order) - II() + 1;
	search_forward = false;
      }
    } else {
      // Predecessors have been scheduled
      
      if(max_time == infinity) {
	// No successors
	if(dbg(ms,2)) {
	  cdbg << "* Only PRED\n";
	}
	max_time = min_time + II() - 1;
	search_forward = true;
      } else {
	// Successors have been scheduled, too
	if(dbg(ms,2)) {
	  cdbg << "* PRED and SUCC\n";
	}
	max_time = ELCOR_MIN(max_time, min_time + II() - 1);
	search_forward = true;
      }
    }

    if(max_time < 1) {
      // Dependencies have been placed so high in the schedule, that
      // it's no longer possible to legally schedule this op. Bump II
      // and try again.
      if(dbg(ms,1)) {
	cdbg << "No legal slot because of previously placed dependences!" << endl;
      }
      return;
    }

    if(min_time < 1) {
      min_time = 1;
    }

    if(dbg(ms,2)) {
      cdbg << "  min: " << min_time << "   max: " << max_time << endl;
    }

    // If this is not true, then something is broken.
    assert(min_time <= max_time);

    // This for loop looks kind of unusual, because I bastardized it
    // to go both forward and backward based on 'search_forward'. If
    // this is confusing, we can just make two loops instead.
    for(slot = (search_forward ? min_time : max_time);
	(search_forward ? (slot <= max_time) : (slot >= min_time)) &&
	  slot > 0;
	slot = (search_forward ? slot + 1 : slot - 1)) {

      if(dbg(ms,1)) {
	if(search_forward) {
	  cdbg << "Trying fwd: " << slot << endl;
	} else {
	  cdbg << "Trying bwd: " << slot << endl;
	}
      }

      found_a_slot = true;
      conflicts.clear();

      List<pattern> patterns;

      rumap()->get_nonconflicting_patterns(node, slot - 1, &patterns);

      if ( ! (patterns.is_empty())) {
	// there are conflict-free resources at current_time.
	sInfo->specific_op = patterns.head()->op;
	sInfo->op_priority = patterns.head()->priority;
      } else {
	found_a_slot = false;
	if(dbg(ms,1)) {
	  cdbg << "No non-conflicting patterns!" << endl;
	}
      }

      if(found_a_slot) {
	getResourceConflicts(node, slot, conflicts);
	if( ! conflicts.is_empty()) {
	  if(dbg(ms,1)) {
	    cdbg << "Resource conflicts!" << endl << "  ";
	    for(Hash_set_iterator<Op*> hsi(conflicts); hsi != 0; hsi++) {
	      cdbg << (*hsi)->id() <<  " ";
	    }
	    cdbg << endl;
	  }
	  found_a_slot = false;
	}
      }
	
      if(found_a_slot) {
	getDependencyConflicts(node, slot, conflicts);
	if( ! conflicts.is_empty()) {
	  if(dbg(ms,1)) {
	    cdbg << "Dependency conflicts!" << endl << "  ";
	    for(Hash_set_iterator<Op*> hsi(conflicts); hsi != 0; hsi++) {
	      cdbg << (*hsi)->id() <<  " ";
	    }
	    cdbg << endl;
	  }
	  found_a_slot = false;
	}
      }

      if(found_a_slot) {
	break;
      }
    }

    // In the Swing algorithm, if a slot is not found, just bump
    // II. This removes the 'iterative' part of iterative modulo
    // scheduling, and should yield faster runtimes.
    if( ! found_a_slot) {
      return;
    }

    if (dbg(ms,1)) {
      cdbg << "Placing node "  << snode.second->id()
	   << " at time " << (slot-1) <<"("<< ((slot-1)%II())<< ")"
	   << " on alternate " <<
                  ((sInfo->specific_op)?(sInfo->specific_op):"NULL") << endl;
      if (dbg(ms,3)) cdbg << *(snode.second) << endl;
    }

    // Place the node in the schedule.
    swing_put(snode, slot);

    if ( ! (sInfo->placed)) {
      // If this assert is encountered, then the checks inside the
      // loop were not sufficient for some reason. Look in
      // swing_put(), find out why it failed, and add that check into
      // the scheduling loop in this function.
      assert(0);
    }

    // Debug print out
    if(dbg(ms,1)) {
      RU_print_map(stddbg);
    }
  }
}


/* NAME: getResourceConflicts
 * INPUT: region with code to consider, scheduling info map, the mode, the node
 *  to check for resource conflicts, and the list to add the conflicting
 *  ops to
 * OUTPUT: ResConfl list filled with Op* that conflict with node
 * PROCESSING:
 */
void ModuloScheduler::getResourceConflicts(Op *node, int time, 
					   Hash_set<Op*>& ResConfl){
  /* The current version of RU_get_conflicting_ops (which is called by */
  /* RU::get_conflicting_Ops), returns a non-empty list of conflicting */
  /* ops when supplied a non-unit specific op, even if there is an */
  /* unoccupied unit of the appropriate type.  To get around this */
  /* shortcoming, this member function is responsible for determing whether */
  /* or not a unit of the appropriate type is available at time.  If so, */
  /* simply return.  If not, go ahead and call RU::get_conflicting_Ops to */
  /* retrieve the conflicting ops */
  if ( ! sInfoMap->value(node)->specific_op 
       && get_mdes_opcode_string(node)) {
    RUmap->get_conflicting_Ops(node, get_mdes_opcode_string(node),
                               time - 1, ResConfl);
  }
}

/* NAME: getDependencyConflicts
 * INPUT: region with code to consider, scheduling info map, the mode, the node
 *  to check for dependency conflicts, and the list to add the conflicting
 *  ops to
 * OUTPUT: DepConfl list filled with Op* that conflict with node
 * PROCESSING: iterate through the in and out edges of node, finding those
 *  nodes that conflict given the current II and schedule
 */
void ModuloScheduler::getDependencyConflicts(Op *node, 
					     int time, Hash_set<Op*>& DepConfl){
  SchedInfo *sInfo;
  int effstime;
  Edge* edge;
  int extra_latency = 0; 

  for (Op_outedges outEdge(node, edge_filter); outEdge != 0; outEdge++) {
    edge = *outEdge;
    sInfo = sInfoMap->value(real_op(edge->dest()));
    effstime = sInfo->stime + edge->scratch_int * the_ii;

    extra_latency = 0; 

    if ((time>effstime-edge->latency()) && sInfo->placed){
      // cdbg << "To unplace consumer node " << (edge->dest())->id() << endl;
      DepConfl += edge->dest();
    }
  }

  for (Op_inedges inEdge(node, edge_filter); inEdge != 0; inEdge++) {
    edge = *inEdge;
    sInfo = sInfoMap->value(edge->src());
    effstime = sInfo->stime - edge->scratch_int * the_ii;

    extra_latency = 0; 

    if ((time<effstime+edge->latency()) && sInfo->placed) {
      DepConfl += edge->src();
      // cdbg << "To unplace producer node " << (edge->dest())->id() << endl;
    }
  }
}

/* NAME: markAsScheduled
 * INPUT:
 * OUTPUT:
 * PROCESSING:
 */
void ModuloScheduler::markAsScheduled(Op *node, int SchedSlot){
 sInfoMap->value(node)->stime = SchedSlot;
 sInfoMap->value(node)->placed = true;
}

/* NAME: markAsUnScheduled
 * INPUT:
 * OUTPUT:
 * PROCESSING:
 */
void ModuloScheduler::markAsUnScheduled(Op *node){
 sInfoMap->value(node)->stime = -1;
 sInfoMap->value(node)->placed = false;

 /* RMR 9/30/99
  * the specific opcode field must be reset as well, otherwise
  * it is inherited on the next attempt to schedule this operation
  */
 sInfoMap->value(node)->specific_op = 0;
}

bool ModuloScheduler::place(Stuple<int, Op *> snode, int SchedSlot, bool tentative){
  Op *node = snode.second;
  SchedInfo *sInfo = sInfoMap->value(node);

  /* RMR 9/30/99
   * this operation has caused all conflicting operations to be unscheduled,
   * and will now be scheduled - however, it has not been assigned a specific
   * opcode, and so we must do so now
   */
  if (sInfo->specific_op == 0) {
    //cdbg << "no specific opcode in place() for op " << node->id() << endl;
      assert (!tentative);
      if (! Tracker->get_specific_sched_info(unscheduledNodes, sInfo, SchedSlot))
	El_punt("ModuloScheduler::place(): Unable to find feasible resource/alternative.");
  }
  
  /* RMR 9/30/99
   * sanity check 
   */
  assert (sInfo->specific_op);
  
//   cdbg << "placing " << node->id() << " " << sInfo->specific_op << " at time "
//        << SchedSlot-1 << endl;

  /* use SchedSlot - 1 since I use 1-based schedules, while RU_* uses */
  /* 0-based ones */
  if (!RUmap->place(node, sInfo->specific_op, sInfo->op_priority, SchedSlot-1)) {
    return false;
  }

  // avoid updating the tracking module or removing the operation 
  // from the queue if this is a tentative operation binding
  if (!tentative) {
      // Tracware:: update the hardware tracking module
      Tracker->commit(node, sInfo->specific_op, SchedSlot);
      unscheduledNodes.remove(snode);
  }

  markAsScheduled(node, SchedSlot);

  return true;
}

void ModuloScheduler::unplace(Stuple<int, Op *> snode, int SchedSlot, bool tentative){
  Op *node = snode.second;
  /* use SchedSlot - 1 since I use 1-based schedules, while RU_* uses */
  /* 0-based ones */
  RUmap->remove(node, SchedSlot - 1);

  // avoid adding the operation back into the queue if this is unplacement of a 
  // tentative binding - otherwise the order of the operations with the same 
  // priority will be permuted
  if (!tentative) {
      // Tracware:: update the hardware tracking module
      Tracker->remove(node);
      unscheduledNodes.add(snode);
  }

  markAsUnScheduled(node);
}

/* NAME: put
 * INPUT: kernel, schedule info map, mode, node to be placed, position to place
 *  node at.
 * OUTPUT: modified schedule info
 * PROCESSING: put node at SchedSlot on the schedule, but only if possible to
 *  do so by only displacing unfixed nodes if need be
 */
bool ModuloScheduler::put(Stuple<int, Op *> snode, int SchedSlot){
  Stuple<int, Op *> item;
  Hash_set<Op*> ResConfl(hash_op_ptr, 16);
  Hash_set<Op*> DepConfl(hash_op_ptr, 16);
  Op *node = snode.second;
  /* union of Ops in ResConfl and DepConfl which conflict with node.  These */
  /* will be unscheduled.  This Set is used to avoid unplacing an Op twice */
  List_set<Stuple<int, Op*> > unplaceSet;

  if (SchedSlot < 1) {
    cerr << "put() error: start time cannot be " << SchedSlot << " for node "
         << node->id() << ". It must be greater than 0" << endl;
    return false;
  }

  /* get resource conflicting instructions */
  getResourceConflicts(node, SchedSlot, ResConfl);
  // cdbg << "put ResConfl call: " << ResConfl.is_empty() << endl;

  /* for each pattern searately */
  /*   get dependency conflicting instructions */
  getDependencyConflicts(node, SchedSlot, DepConfl);
  // cdbg << "put DepConfl call: " << DepConfl.is_empty() << endl;

  /* if (resource conflicting instructions is empty */
  /*     and (at least one pattern's dependency conflicting instructions is */
  /*          empty) */
  /* then */
  /*   place node on resource usage map using that pattern */

  if (ResConfl.is_empty() && DepConfl.is_empty())
    return place(snode, SchedSlot);

  /* The following is an unsophisticated approach to unplacement.  For a */
  /* more sophisticated one, see the file rauput */

  /*  for all dependency conflicting instructions */
  /*    if instruction is fixed */
  /*      return failure */
  /*    else */
  /*      remember to unplace it */

  Hash_set_iterator<Op *> instr;
  for (instr(DepConfl); instr != 0; instr++)
    if(sInfoMap->value(*instr)->fixed)
      return false;
    else {
      item.first = sInfoMap->value(*instr)->height;
      item.second = *instr;
      unplaceSet += item;
    }

  /* for all resource conflicint instructions */
  /*   if instruction is fixed */
  /*     return failure */
  /*   else */
  /*     remember to unplace it. */

  for (instr(ResConfl); instr != 0; instr++)
    if(sInfoMap->value(*instr)->fixed)
      return false;
    else {
      item.first = sInfoMap->value(*instr)->height;
      item.second = *instr;
      unplaceSet += item;
    }

  /* for all conflicting instructions */
  /*   remove instruction from resource usage map */
  /*   add instruction to unschedule instructions */

  for (List_set_iterator<Stuple<int, Op*> > instr2(unplaceSet); instr2 != 0; 
       instr2++) {
    // the sched_time of Op has not been committed yet. -- Shail Aditya 08/10/98
    // unplace(*instr2, (*instr2).second->sched_time());
    if (dbg(ms, 2)) {
      int _time_ = sInfoMap->value((*instr2).second)->stime;
      cdbg << "Unplacing node " << (*instr2).second->id() 
           << " at time " << _time_ << "(" << ((_time_-1)%II()) << ") "
	   << " to make room for node " << node->id() << endl;
    }
    unplace(*instr2, sInfoMap->value((*instr2).second)->stime);
  }

  /* place node on resource usage map and remove from unscheduled */
  /* instructions */
  /* if unsuccessful */
  /*   panic */
  if (!place(snode, SchedSlot)) {
    El_warn("MSched: couldn't place node %d after unscheduling what it conflicted with.", 
	    node->id());
    return false;
  }
  return true;
}


// Special ::put() for the Swing algorithm. Because Swing doesn't do
// any unscheduling, this is greatly simplified.
bool ModuloScheduler::swing_put(Stuple<int, Op *> snode, int SchedSlot)
{
  Op *node = snode.second;

  if (SchedSlot < 1) {
    cdbg << "swing_put() error: start time cannot be " << SchedSlot << " for node "
         << node->id() << ". It must be greater than 0" << endl;
    return false;
  }

  return place(snode, SchedSlot);
}


/* NAME: CalcEstart
 * INPUT: kernel, the node to find the start time for, which type of schedule
 *  this is, and where to put the earliest start time
 * OUTPUT: the calculated earliest start time
 * PROCESSING: find the earliest start time for this node as constrained by 
 *  already scheduled predecessors, which is equivalent to the latest finish 
 *  time of all predecessors, taking into account inter-iteration arcs
 */
void ModuloScheduler::CalcEstart(Op *node, int *estart){
  SchedInfo *predInfo;
  int max = minus_infinity;
  int effftime;

  for(Op_inedges inEdge(node, edge_filter); inEdge != 0; inEdge++) {
      predInfo = sInfoMap->value((*inEdge)->src());
      if (predInfo->placed)
        effftime = predInfo->stime + (*inEdge)->latency();
      else
        effftime = minus_infinity;

      /* make adjustment for inter-iteration arcs */
      if (effftime > minus_infinity)
          effftime = effftime - (*inEdge)->scratch_int*the_ii;

      /* check if max needs to be adjusted */
      if ((effftime > minus_infinity) && (max < effftime))
        max = effftime;
  }
  *estart = max;
}

int ModuloScheduler::CalcHeight(Op *op){
  // q.v. failFloyd for construction of minDist
  return (*minDist)[nodeMap.value(op)][numnodes];
}


void ModuloScheduler::get_path_ops(int i, int k, List_set<Op*>& ops)
{
  int j = (*nhbrs)[i][k];

  if (j >= 0 && j != i) {
    get_path_ops(i, j, ops);
    get_path_ops(j, k, ops);
  } else {
    Op *src = revnodeMap.value(i);
    ops += src;
  }
}


// Support functions for the Swing prioritization
void ModuloScheduler::get_pred_set(Hash_map<Op*, int>& priorities, List_set<Op*>& answer,
				   Hash_map<Op*, int>& op_order)
{
  answer.clear();
  // priorities contains the ops that are bound. This function finds
  // the predecessors of these ops which are not bound.
  for(Hash_map_iterator<Op*, int> hmi(priorities); hmi != 0; hmi++) {
    Op* op = (*hmi).first;
    for(Op_inedges ine(op, REAL_EDGES); ine != 0; ine++) {
      Op* src = (*ine)->src();
      if( ! is_pseudo(src)) {
	if(op_order[src] < op_order[op]) {
	  if( ! priorities.is_bound(src) && ! is_pseudo(src)) {
	    answer += src;
	  }
	}
      }
    }
  }
}

void ModuloScheduler::get_succ_set(Hash_map<Op*, int>& priorities, List_set<Op*>& answer,
				   Hash_map<Op*, int>& op_order)
{
  answer.clear();
  for(Hash_map_iterator<Op*, int> hmi(priorities); hmi != 0; hmi++) {
    Op* op = (*hmi).first;
    for(Op_outedges oute(op, REAL_EDGES); oute != 0; oute++) {
      Op* dest = (*oute)->dest();
      if( ! is_pseudo(dest)) {
	if(op_order[dest] > op_order[op]) {
	  if( ! priorities.is_bound(dest)) {
	    answer += dest;
	  }
	}
      }
    }
  }
}

void ModuloScheduler::CalcASAP(Hash_map<Op*, int>& asap)
{
  asap.clear();
  Real_op_filter real_op_filter;
  for (Region_all_ops ops(kernel, &real_op_filter); ops != 0; ops++) {
    Op* op = *ops;
    // Unlike everything else in the entirety of computer science, the
    // stupid ModuloScheduler is 1 relative, not 0 relative, so the
    // earliest an operation can go is slot 1.
    int this_asap = 1;
    for(Op_inedges ine(op, REAL_EDGES); ine != 0; ine++) {
      Edge* edge = *ine;
      Op* src = edge->src();
      if(asap.is_bound(src)) {
	int op_asap = asap[src] + edge->latency() - (edge->scratch_int * II());
	if(op_asap > this_asap)
	  this_asap = op_asap;
      }
    }
    asap.bind(op, this_asap);
  }
}

void ModuloScheduler::CalcALAP(Hash_map<Op*, int>& alap, int asap_max)
{
  alap.clear();
  for (Region_ops_reverse_C0_order ops(kernel); ops != 0; ops++) {
    Op* op = *ops;
    if ( ! is_pseudo(op)) {
      int this_alap = asap_max;
      for(Op_outedges oute(op, REAL_EDGES); oute != 0; oute++) {
	Edge* edge = *oute;
	Op* dest = edge->dest();
	if(alap.is_bound(dest)) {
	  int op_alap = alap[dest] - edge->latency() +
	    (edge->scratch_int * II());
	  if(op_alap < this_alap)
	    this_alap = op_alap;
	}
      }
      alap.bind(op, this_alap);
    }
  }
}


// All ops that lie on dataflow edges between ops that have already
// been bound and ops we are going to bind, must be added to the set
// or else the priorities will lead to scheduling conflicts at any II.
static void add_ops_between_already_bound_ops(const List_set<Op*>& priorities,
					      List_set<Op*>& path_ops,
					      const Hash_map<Op*, int>& op_order)
{
  // priorities contains the ops which have already been
  // bound. path_ops is the set of ops to be bound. This function adds
  // all ops between the two sets.

  // Find the upward and downward exposed sets, and add their
  // intersections.
  Hash_set<Op*> path_down_set(hash_op_ptr, 27);
  Hash_set<Op*> path_up_set(hash_op_ptr, 27);
  Hash_set<Op*> priority_down_set(hash_op_ptr, 27);
  Hash_set<Op*> priority_up_set(hash_op_ptr, 27);
  for(List_set_iterator<Op*> lsi(path_ops); lsi != 0; lsi++) {
    Op* op = *lsi;

    List<Op*> to_process;
    to_process.add_tail(op);
    while( ! to_process.is_empty()) {
      Op* processee = to_process.pop();

      for(Op_outedges oute(processee, REAL_EDGES); oute != 0; oute++) {
	Op* dest = (*oute)->dest();
	// only look at forward edges
	if(op_order[processee] < op_order[dest]) {
	  if( ! path_ops.is_member(dest) && ! priorities.is_member(dest) &&
	      ! path_down_set.is_member(dest) && ! is_pseudo(dest)) {
	    path_down_set += dest;
	    to_process.add_tail(dest);
	  }
	}
      }
    } // while ! to_process.is_empty()


    to_process.add_tail(op);
    while( ! to_process.is_empty()) {
      Op* processee = to_process.pop();

      for(Op_inedges ine(processee, REAL_EDGES); ine != 0; ine++) {
	Op* src = (*ine)->src();
	// only look at backward edges
	if(op_order[processee] > op_order[src]) {
	  if( ! path_ops.is_member(src) && ! priorities.is_member(src) &&
	      ! path_up_set.is_member(src) && ! is_pseudo(src)) {
	    path_up_set += src;
	    to_process.add_tail(src);
	  }
	}
      }
    } // while ! to_process.is_empty()

  } // for(List_set_iterator<Op*> lsi(path_ops);

  // Now do the same thing for 'priorities'.
  for(List_set_iterator<Op*> lsi(priorities); lsi != 0; lsi++) {
    Op* op = *lsi;

    List<Op*> to_process;
    to_process.add_tail(op);
    while( ! to_process.is_empty()) {
      Op* processee = to_process.pop();

      for(Op_outedges oute(processee, REAL_EDGES); oute != 0; oute++) {
	Op* dest = (*oute)->dest();
	// only look at forward edges
	if(op_order[processee] < op_order[dest]) {
	  if( ! path_ops.is_member(dest) && ! priorities.is_member(dest) &&
	      ! priority_down_set.is_member(dest) && ! is_pseudo(dest)) {
	    priority_down_set += dest;
	    to_process.add_tail(dest);
	  }
	}
      }
    } // while ! to_process.is_empty()


    to_process.add_tail(op);
    while( ! to_process.is_empty()) {
      Op* processee = to_process.pop();

      for(Op_inedges ine(processee, REAL_EDGES); ine != 0; ine++) {
	Op* src = (*ine)->src();
	// only look at backward edges
	if(op_order[processee] > op_order[src]) {
	  if( ! path_ops.is_member(src) && ! priorities.is_member(src) &&
	      ! priority_up_set.is_member(src) && ! is_pseudo(src)) {
	    priority_up_set += src;
	    to_process.add_tail(src);
	  }
	}
      }
    } // while ! to_process.is_empty()

  } // for(List_set_iterator<Op*> lsi(priorities);

  // Now take the intersections of the sets to find ops between paths
  // ops and priorities.
  path_up_set *= priority_down_set;
  path_down_set *= priority_up_set;

  // And finally add the ops to the set of ops that needs bound.
  for(Hash_set_iterator<Op*> hsi(path_up_set); hsi != 0; hsi++) {
    path_ops += (*hsi);
  }
  for(Hash_set_iterator<Op*> hsi(path_down_set); hsi != 0; hsi++) {
    path_ops += (*hsi);
  }
}


// This is the priority function proposed in "Swing Modulo Scheduling:
// A Lifetime-Sensitive Approach" by Llosa, Gonzalez, Ayguade, and
// Valero. I use list_sets all over the place, because order IS
// important in most of the cases.
void ModuloScheduler::CalcSwingPriorities(Hash_map<Op*, int>& priorities)
{
  priorities.clear();

  Hash_map<Op*, int> op_order(hash_op_ptr, 71);
  construct_op_order_map(kernel, op_order);

  // First group the recurrences.
  Slist<Stuple<int, List_set<Op*> > > partial_order;
  List_set<Op*> partially_ordered;
  List_set<Op*> acyclic_ops;
  for (int i = 0; i < numnodes; i++) {
    int delay = (*minDist)[i][i];
    if (delay != minus_infinity) {
      List_set<Op*> path_ops;
      get_path_ops(i, i, path_ops);
      if(path_ops.size() > 1) {

// 	cdbg << "path_ops: ";
// 	for(List_set_iterator<Op*> lsi(path_ops); lsi != 0; lsi++) {
// 	  cdbg << (*lsi)->id() << " ";
// 	}
// 	cdbg << endl;

	// If this recurrence intersects already added recurrences,
	// then they have to be combined. FIX: this is not very good
	// algorithmically.
	bool combined_this_rec = false;
	for(Slist_iterator<Stuple<int, List_set<Op*> > > sli(partial_order); sli != 0; sli++) {
	  List_set<Op*>& prev_recurrence = (*sli).second;
	  
// 	  cdbg << "  compare with: ";
// 	  for(List_set_iterator<Op*> lsi(prev_recurrence); lsi != 0; lsi++) {
// 	    cdbg << (*lsi)->id() << " ";
// 	  }
// 	  cdbg << endl;

	  if(path_ops.intersects(prev_recurrence) && (path_ops != prev_recurrence)) {
// 	    cdbg << "    **Combining" << endl;
	    prev_recurrence += path_ops;
	    partially_ordered += path_ops;
	    combined_this_rec = true;
	    break;
	  }
	}

	if( ! combined_this_rec) {
	  add_ops_between_already_bound_ops(partially_ordered, path_ops,
					    op_order);
	  partially_ordered += path_ops;
	  Stuple<int, List_set<Op*> > item(-delay, path_ops);

// 	  cdbg << "adding: ";
// 	  for(List_set_iterator<Op*> lsi(path_ops); lsi != 0; lsi++) {
// 	    cdbg << (*lsi)->id() << " ";
// 	  }
// 	  cdbg << endl;

	  partial_order.add(item);
	}
      } else {
	// Recurrences of size 1 are no different than acyclic ops.
	acyclic_ops += revnodeMap[i];
      }
    } else {
      // Delay == minus_infinity means this op is not part of a cycle.
      acyclic_ops += revnodeMap[i];
    }
  }
  partial_order.add(Stuple<int, List_set<Op*> >(-minus_infinity, acyclic_ops));

  // Start prioritizing ops with the highest latency recurrence first,
  // moving down the list. The set names correspond to the ones used
  // in the paper.
  int current_priority = numnodes + 1;
  for(Slist_iterator<Stuple<int, List_set<Op*> > > sli(partial_order); sli != 0; sli++) {
    List_set<Op*>& S_ops = (*sli).second;

    List<Op*> to_remove;
    for(List_set_iterator<Op*> lsi(S_ops); lsi != 0; lsi++) {
      if(priorities.is_bound(*lsi)) {
	to_remove.add_tail(*lsi);
      }
    }

    for(List_iterator<Op*> li(to_remove); li != 0; li++) {
      S_ops -= *li;
    }

    if(dbg(ms,2)) {
      cdbg << "Starting set: ";
      for(List_set_iterator<Op*> lsi(S_ops); lsi != 0; lsi++) {
	cdbg << (*lsi)->id() << " ";
      }
      cdbg << endl;
    }

    Hash_map<Op*, int> asap_time(hash_op_ptr, 71);
    Hash_map<Op*, int> alap_time(hash_op_ptr, 71);
    CalcASAP(asap_time);

    int asap_max = 0;
    for(Hash_map_iterator<Op*, int> hmi(asap_time); hmi != 0; hmi++) {
      int a_time = (*hmi).second;
      if(a_time > asap_max)
	asap_max = a_time;
    }
    CalcALAP(alap_time, asap_max);

    while( ! S_ops.is_empty()) {

      if(dbg(ms,2)) {
	cdbg << "Loop S_ops: ";
	for(List_set_iterator<Op*> lsi(S_ops); lsi != 0; lsi++) {
	  cdbg << (*lsi)->id() << " ";
	}
	cdbg << endl;
      }

      List_set<Op*> pred_L;
      List_set<Op*> succ_L;
      get_pred_set(priorities, pred_L, op_order);
      get_succ_set(priorities, succ_L, op_order);

      List_set<Op*> R_ops;
      bool bottom_up_order = true;  // false means top-down
      if(pred_L.size() != 0 && S_ops.is_subset(pred_L)) {
	if(dbg(ms,2)) {
	  cdbg << "R_ops = pred: ";
	  for(List_set_iterator<Op*> lsi(pred_L); lsi != 0; lsi++) {
	    cdbg << (*lsi)->id() << " ";
	  }
	  cdbg << endl;
	}
	R_ops = pred_L;
	bottom_up_order = true;
      } else if(succ_L.size() != 0 && S_ops.is_subset(succ_L)) {
	if(dbg(ms,2)) {
	  cdbg << "R_ops = succ: ";
	  for(List_set_iterator<Op*> lsi(succ_L); lsi != 0; lsi++) {
	    cdbg << (*lsi)->id() << " ";
	  }
	  cdbg << endl;
	}
	R_ops = succ_L;
	bottom_up_order = false;
      } else {
	// Find op with the highest ASAP that's not bound
	Op* highest_asap = NULL;
	for(List_set_iterator<Op*> lsi(S_ops); lsi != 0; lsi++) {
	  Op* op = *lsi;
	  if( ! priorities.is_bound(op)) {
	    if((highest_asap == NULL) || asap_time[op] > asap_time[highest_asap]) {
	      highest_asap = op;
	    }
	  }
	}
	if(highest_asap) {
	  if(dbg(ms,2)) {
	    cdbg << "Highest asap: " << highest_asap->id() << endl;
	  }
	  R_ops += highest_asap;
	}
	bottom_up_order = true;
      }

      // Now prioritize all ops in the R_ops set.
      while( ! R_ops.is_empty()) {
	if(bottom_up_order == false) {
	  // top-down

	  while( ! R_ops.is_empty()) {
	    // Find element of R with highest height. Use slack as a
	    // tie-breaker.
	    Op* place_me = R_ops.head();
	    for(List_set_iterator<Op*> lsi(R_ops); lsi != 0; lsi++) {
	      Op* op = *lsi;
	      if(CalcHeight(op) > CalcHeight(place_me)) {
		place_me = op;
	      } else if(CalcHeight(op) == CalcHeight(place_me)) {
		if((asap_time[op] - alap_time[op]) <
		   (asap_time[place_me] - alap_time[place_me])) {
		  place_me = op;
		}
	      }
	    }
      
	    if( ! priorities.is_bound(place_me)) {
	      if(dbg(ms,2)) {
		cdbg << "  * 1 Binding " << place_me->id() << "\t" << current_priority << endl;
	      }
	      priorities.bind(place_me, current_priority--);
	    }
	
	    // Remove the op that was just placed, and add it's successors
	    S_ops -= place_me;
	    R_ops -= place_me;
	    for(Op_outedges oute(place_me, REAL_EDGES); oute != 0; oute++) {
	      Edge* edge = *oute;
	      Op* dest = edge->dest();
	      if(S_ops.is_member(dest) && ! priorities.is_bound(dest) &&
		 (op_order[dest] > op_order[place_me])) {
		if( ! priorities.is_bound(dest))
		  R_ops += dest;
	      }
	    }

	  } // while ! R_ops.is_empty()

	  // Change the direction of placement, and add predecessors to
	  // the list.
	  bottom_up_order = true;
	  get_pred_set(priorities, pred_L, op_order);
	  for(List_set_iterator<Op*> lsi(S_ops); lsi != 0; lsi++) {
	    Op* op = *lsi;
	    if(pred_L.is_member(op)) {
	      if( ! priorities.is_bound(op))
		R_ops += op;
	    }
	  }
	
	} else {
	  // bottom up

	  while( ! R_ops.is_empty()) {
	    // FIX: this is supposed to be distance (using dataflow
	    // edges). reverse_C0_order is close enough.
	    
	    // Find element of R with highest distance. Use slack as a
	    // tie-breaker.
	    Op* place_me = R_ops.head();
	    for(List_set_iterator<Op*> lsi(R_ops); lsi != 0; lsi++) {
	      Op* op = *lsi;
	      if(op_order[op] > op_order[place_me]) {
		place_me = op;
	      } else if((op != place_me) && 
			(op_order[op] == op_order[place_me])) {
		if((asap_time[op] - alap_time[op]) <
		   (asap_time[place_me] - alap_time[place_me])) {
		  place_me = op;
		}
	      }
	    }
      
	    if( ! priorities.is_bound(place_me)) {
	      if(dbg(ms,2)) {
		cdbg << "  * 2 Binding " << place_me->id() << "\t" << current_priority << endl;
	      }
	      priorities.bind(place_me, current_priority--);
	    }
	
	    // Remove the op that was just placed, and add it's successors
	    S_ops -= place_me;
	    R_ops -= place_me;
	    for(Op_inedges ine(place_me, REAL_EDGES); ine != 0; ine++) {
	      Edge* edge = *ine;
	      Op* src = edge->src();
	      if(S_ops.is_member(src) && ! priorities.is_bound(src) &&
		 (op_order[src] < op_order[place_me])) {
		if( ! priorities.is_bound(src))
		  R_ops += src;
	      }
	    }

	  } // while ! R_ops.is_empty()

	  // Change the direction of placement, and add successors to
	  // the list.
	  bottom_up_order = false;
	  get_succ_set(priorities, succ_L, op_order);
	  for(List_set_iterator<Op*> lsi(S_ops); lsi != 0; lsi++) {
	    Op* op = *lsi;
	    if(succ_L.is_member(op)) {
	      if( ! priorities.is_bound(op)) {
		R_ops += op;
		if(dbg(ms,2)) {
		  cdbg << "  2 adding " << op->id() << " to R_ops" << endl;
		}
	      }
	    }
	  }
	
	} // bottom_up
      } // while ! R_ops.is_empty
    } // while ! S_ops.is_empty
  }
}


//---------------------------- Commiting the schedule ------------------------
// Commit relies only on the information in IR for last successful II, since
// last scheduling attempt may not be succesful.

void ModuloScheduler::commit(bool do_sorting) {
    if (good_ii == 0){
	El_warn("Modulo Scheduler: No valid schedule to commit. Quiting.");
	return ;
    }
    if (good_ii != the_ii) 
      El_warn("Modulo Scheduler: Last II was no good. Commiting with last valid II.");
    
    // sort ops according to sched times
    if (do_sorting) sort_ops();

    // verify schedule
    Real_op_filter real_op_filter;
    Region_all_ops ops;

    // update the omega of operands in the kernel based on unrolled sched times.
    update_omegas();

    // record loop information
    for (ops(kernel); ops != 0; ops++) {
      Op *curr_op = *ops;
      curr_op->set_flag(EL_REGION_SCHEDULED);    // Mark it scheduled
      // record unrolled sched time
      //int* sched_time = new int(curr_op->sched_time());
      int sched_time = curr_op->sched_time();
      if (is_swp_branch(curr_op)) {
	sched_time += good_ii*the_esc;
	// SAM 8-2000, update width annot on the BRF to account for finite ESC
	Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(curr_op);
	if ((rdt_attr != NULL) && is_swp_branch(curr_op)) {
	  // We know ESC is kept in DEST3/SRC3 of all BRFs
	  DType esc_dt = rdt_attr->dest(DEST3);
	  esc_dt.calc_and_set_width(the_esc, true);
	  rdt_attr->set_dest(DEST3, esc_dt);
	  rdt_attr->set_src(SRC3, esc_dt);
	}
      }
      //set_generic_attribute(curr_op, "unrolled_sched_time", (void*)sched_time);
      Usched_time_attr* us_attr = new Usched_time_attr;
      us_attr->usched_time = sched_time;
      set_usched_time_attr(curr_op, us_attr);
      
      // roll the sched time modulo ii
      if(El_msched_allocator == ROTATING_REGISTERS) {
	curr_op->set_sched_time(curr_op->sched_time() % good_ii);
      }
    }
    kernel->set_flag(EL_REGION_SCHEDULED);       // Mark kernel scheduled
    loopbody->set_flag(EL_REGION_SCHEDULED);     // Mark loop scheduled
    Msched_attr* attr = get_msched_attr(loopbody);
    attr->ii = good_ii;
    attr->esc = the_esc;
    

    commited = true;
}

// Update the operand omegas to reflect actual iteration distances after modulo
// scheduling.  Assumes direct edges exist, and sched_times have not yet been
// rolled.  Omegas are updated by dec/incrementing them rather than setting
// their values directly; this lets us account for omegas set earlier by dsa
// conversion due to recurrences.  -KF 3/2005
void ModuloScheduler::update_omegas()
{
  // special case for target predicate of BRF. There's no BRF if we're
  // doing modulo variable expansion.
  if(El_msched_allocator == ROTATING_REGISTERS) {
    Region_exit_ops exit(kernel);
    Op* brf_op = *exit;
    assert(is_swp_branch(brf_op));
    brf_op->dest(DEST1).set_omega(-1);
  }

  // keep track of updated refs
  Hash_map<El_ref, int> updated_refs(hash_El_ref, numnodes * 3);

  for (Region_ops_C0_order opi(kernel); opi!=0; opi++) {
    Op* def_op = *opi;
    if (is_pseudo(def_op)) continue;

    int def_iter = def_op->sched_time() / good_ii;

    for (Op_explicit_dests desti(def_op); desti!=0; desti++) {
      Operand& def_opnd = *desti;
      if (!def_opnd.is_rotating()) continue;

      int old_def_omega = def_opnd.omega();

      Filter_and<Edge*> filt(new Outedge_port_filter(DEST, desti.get_port_num()),
                             FLOW_EDGES, true, false);

      // first go through uses and see if any of them have already been
      // updated (potentially requiring the def omega to be updated).
      for (Op_outedges ei(def_op, &filt); ei!=0; ei++) {
        Edge* edge = *ei;

        Op* use_op = edge->dest();
        if (is_pseudo(use_op)) continue;
        
        Operand& use_opnd = use_op->src(edge->dest_port());
        if (!use_opnd.is_rotating()) continue;

        El_ref use_ref(use_op, &use_opnd, (edge->dest_port() == PRED1) ? PRED_SRC : EXP_SRC);

        if (updated_refs.is_bound(use_ref)) {
          // if this use has already been updated, we may have to update the def.
          int use_iter = use_op->sched_time() / good_ii;
          int incr = use_iter - def_iter;
          int use_incr = updated_refs.value(use_ref);
          // update the def omega, taking into account the previous update to
          // the use omega.  -KF 8/2005
          def_opnd.incr_omega(use_incr - incr);

          // doesn't make sense to have more than one unique omega, so assume
          // remaining uses either have not yet been updated, or have been
          // updated to the same omega.  could put a sanity check here to ensure
          // this, but it would require a prepass to keep track of the original
          // omegas.
          break;
        }
      }

      int def_omega_diff = def_opnd.omega() - old_def_omega;

      // next go through uses again and update their omegas.
      for (Op_outedges ei(def_op, &filt); ei!=0; ei++) {
        Edge* edge = *ei;
        Op* use_op = edge->dest();
        if (is_pseudo(use_op)) continue;
        
        Operand& use_opnd = use_op->src(edge->dest_port());
        if (!use_opnd.is_rotating()) continue;

        El_ref use_ref(use_op, &use_opnd, (edge->dest_port() == PRED1) ? PRED_SRC : EXP_SRC);

        // if use has already been updated, assume it is already correct. once
        // again, could have a sanity check here, but see comment above.
        if (updated_refs.is_bound(use_ref)) continue;

        int use_iter = use_op->sched_time() / good_ii;
        int incr = use_iter - def_iter + def_omega_diff;
        use_opnd.incr_omega(incr);
        updated_refs.bind(use_ref, incr);
      }
    }
  }
}

void ModuloScheduler::sort_ops()
{
  // Modulo-scheduling effectively shuffles the original linear
  // sequence of ops, therefore we need to sort ops according to the
  // modulo schedule. But operations within each cycle must maintain
  // the original order to allow subsequent analysis. 
  // -- Shail Aditya 08/26/98

  int vector_size = 0;
  if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
    for(Region_all_ops rops(kernel); rops != 0; rops++) {
      vector_size = ELCOR_MAX(vector_size, (*rops)->sched_time() + 1);
    }
  } else {
    vector_size = good_ii;
  }

  List<Op*> remap_list;
  Vector<List<Op*> > real_ops(vector_size);
  Vector<List<Op*> > mem_ops(vector_size);
  Op *cm_op = NULL, *branch_op = NULL;
  Op *op;
  for(Region_all_ops ops(kernel); ops!= 0; ops++) {
    op = (Op*) *ops;
    if (is_merge(op) ||  is_switch(op)) continue;
    if (is_control_merge(op)) {
      cm_op = op;
      continue;
    }
    remove_incoming_c0_edge(op);
    if (is_remap(op))
      remap_list.add_tail(op);
    else if (is_pseudo(op))
      El_warn("Modulo Scheduler: Can't handle some pseudo op. Ignoring");
    else if (is_swp_branch(op) || is_branch(op)) 
      branch_op = op;
    else {
      (real_ops[op->sched_time() % vector_size]).add_tail(op);
    }
    // memory ops are also recorded separately to allow 
    // sorting according to unit execution priority
    if(is_memory(op))  
      (mem_ops[op->sched_time() % vector_size]).add_tail(op);

  }

  // Clear list of all Ops in region and put back CM-op and its related ops
  // back on the list, followed later by other ops and their related ops
  kernel->clear_region();
  kernel->add_region(cm_op);
  for (List_iterator<Op*> rel1(cm_op->get_relops()); rel1 !=0; rel1++) {
    kernel->add_region(*rel1);
  }

  // Add real ops sans branch
  Op *prev = cm_op;
  Op* current;
  int i;
  for (i = 0; i < vector_size; i++) {
    // reassign the sched opcodes of memory ops in execution unit
    // priority order. This assumes that all memory units can
    // perform both load/store operations.
    fix_zero_cycle_problems(mem_ops[i], i);

    // Add ops back for current cycle
    for(List_iterator<Op*> real(real_ops[i]); real != 0; real++) {
      current = *real;
      connect_and_add_op(current, prev);
      prev = current;
    }
  }
  // Add remaps
  for(List_iterator<Op*> remaps(remap_list); remaps != 0; remaps++) {
    current = *remaps;
    connect_and_add_op(current, prev);
    prev = current;
  }
  // Add switches and the branch
  for (List_iterator<Op*> rel2(branch_op->get_relops()); 
       rel2 !=0; rel2++) {
    kernel->add_region(*rel2);
  }	    
  connect_and_add_op(branch_op, prev);
  return ;
}

void ModuloScheduler::connect_and_add_op(Op* current, Op* previous){
  kernel->add_region(current);
  C0_connect_fallthrough(previous, current); 
}

void ModuloScheduler::remove_incoming_c0_edge(Op* op){
    Edge* edge = *(Op_inedges(op, CONTROL, CONTROL0));
    op->remove_inedge(edge, CONTROL, CONTROL0);
    (edge->src())->remove_outedge(edge, CONTROL, CONTROL0);
    delete edge;
    return ;
}

// Find ops with 0 cycle dependences that are scheduled out of order,
// prioritize them, and reschedule to get correct opcodes. This is a very
// crudy way of doing things -- We need a better way.
void ModuloScheduler::fix_zero_cycle_problems(List<Op*>& mem_ops, int time){
  Real_edge_filter real_edge_filter;
  List<Edge*> edge_list;  // List of all zero dependence edges -- not just
  // the out of order ones.
  for(List_iterator<Op*> mems(mem_ops); mems !=0; mems++){
    Op* dest = *mems;
    for (Op_inedges edges(dest, &real_edge_filter); edges != 0; edges++){
      Edge* edge = *edges;
      Op* src = edge->src();
      int src_sc  = src->sched_time() / good_ii;  
      int src_time = src->sched_time() % good_ii;
      int dest_sc = dest->sched_time() / good_ii;
      if (is_memory(src) && 
	  (src != dest) &&           // Ignore self cycles
	  (edge->latency() == 0) && 
	  (src_time == time) &&      // This and next check for same time
	  ((src_sc - dest_sc) == edge->scratch_int)){
	edge_list.add_tail(edge);
      }
    }
  }
  // Go over the list and fix the problems. Lower priority number means
  // earlier execution semantics. Therefore, exchange ops if the src of
  // a dependence edge has higher priority number than its dest.
  bool exchange;
  do{
    exchange = false;
    for (List_iterator<Edge*> iter (edge_list); iter != 0; iter++){
      Edge* edge = *iter;
      Op* src_op = edge->src();
      Op* dest_op = edge->dest();
      char* src = src_op->sched_opcode();
      char* dest = dest_op->sched_opcode();
      if (MDES_priority(src) > MDES_priority(dest)) {
	assert((src_op->sched_time()%good_ii)==(dest_op->sched_time()%good_ii));
	MDES_exchange_resources(src_op->sched_time(), &src, &dest);
	src_op->set_sched_opcode(src);
	dest_op->set_sched_opcode(dest);
	exchange = true;
      }
    }
  }
  while (exchange);
}

void ModuloScheduler::attach_meld_info(RR_allocation& rr_alloc)
{
  Latency_map  entry_use_map, entry_def_map, exit_use_map, exit_def_map;

  // ensure that loop has exactly one entry and one exit
  Op *entry_op = *(Region_entry_ops(loopbody));
  Op *exit_op = *(Region_exit_ops (loopbody));

  // initialize various things
  MS_constraints *msc = new MS_constraints(loopbody);
  set_ms_constraints(loopbody, msc);
  msc->set_min_distance(entry_op, exit_op, (the_esc + 1)* good_ii);

  // The sched_time of branch op needs to be shifted to the end of the iteration
  int branch_end_time = exit_op->sched_time() + the_esc*good_ii + 
    MDES_branch_latency(get_mdes_opcode_string(exit_op)); // branch latency
  Real_op_filter real_op_filter;
  int sched_time; 

  // iterate over all the ops in order to set the values of the latency maps.
  for (Region_all_ops ops(kernel, &real_op_filter); ops != 0; ops++) {
    Op *curr_op = *ops;
    sched_time = curr_op->sched_time();

    // look at all source operands and set entry and exit use maps accordingly.
    for (Op_explicit_inputs src_oprnds(curr_op) ; src_oprnds !=0 ; src_oprnds++) {
      Operand& curr_operand = *src_oprnds;
      Port_num curr_port = src_oprnds.get_port_num();

      if (!curr_operand.is_reg() && !curr_operand.is_macro_reg())
	continue;

      // allowed entry dangle = sched_time + earliest sample time 
      int new_can_dangle = sched_time + curr_op->flow_time (SRC, curr_port);
      if (new_can_dangle < MDES_max_op_lat)
        if (!entry_use_map.is_bound (curr_operand) ||
            (entry_use_map.value (curr_operand) > new_can_dangle))
          entry_use_map.bind (curr_operand, new_can_dangle);

      // needed exit dangle = sched_time + latest sample time - branch end time
      int new_need_to_dangle = sched_time + curr_op->anti_time (SRC, curr_port) - branch_end_time;
      if (new_need_to_dangle > 0)
        if ((!exit_use_map.is_bound (curr_operand)) ||
            (exit_use_map.value (curr_operand) < new_need_to_dangle))
          exit_use_map.bind (curr_operand, new_need_to_dangle);
    }

    // look at all dest operands and set entry and exit def maps accordingly.
    for (Op_explicit_dests dest_oprnds(curr_op) ; dest_oprnds !=0 ; dest_oprnds++) {
      Operand& curr_operand = *dest_oprnds;
      Port_num curr_port = dest_oprnds.get_port_num();

      if (!curr_operand.is_reg() && !curr_operand.is_macro_reg()) continue;

      // We need to adjust the omega by -1 for the staging predicate definition
      // in the final branch to account for the logical remap occurring earlier.
      if ((curr_op == exit_op) && (curr_port == exit_op->first_dest())) {
	Operand new_operand(curr_operand);
	new_operand.incr_omega(-1);
	curr_operand = new_operand;
      }

      // allowed entry dangle = sched_time + earliest write time
      int new_can_dangle = sched_time + curr_op->anti_time (DEST, curr_port);
      if (new_can_dangle < MDES_max_op_lat)
        if (!entry_def_map.is_bound (curr_operand) ||
            (entry_def_map.value (curr_operand) > new_can_dangle))
          entry_def_map.bind (curr_operand, new_can_dangle);

      // needed exit dangle = sched_time + latest write time - branch end time
      int new_need_to_dangle = sched_time + curr_op->flow_time (DEST, curr_port) - branch_end_time;
      if (new_need_to_dangle > 0)
        if ((!exit_def_map.is_bound (curr_operand)) ||
            (exit_def_map.value (curr_operand) < new_need_to_dangle))
          exit_def_map.bind (curr_operand, new_need_to_dangle);
    }
  }

  // Need to copy current dangles to the final maps and 
  // add additional dangles for rotating register operands
  copy_entry_meld_info(rr_alloc, entry_use_map, msc->use_at_entry(entry_op));
  copy_entry_meld_info(rr_alloc, entry_def_map, msc->def_at_entry (entry_op));
  copy_exit_meld_info(rr_alloc, exit_use_map, msc->use_at_exit (exit_op));
  copy_exit_meld_info(rr_alloc, exit_def_map, msc->def_at_exit (exit_op));

  if (dbg(ms,2)) {
    cdbg << "Meld Use at Entry:\n" << *(msc->use_at_entry(entry_op)) << endl;
    cdbg << "Meld Def at Entry:\n" << *(msc->def_at_entry(entry_op)) << endl;
    cdbg << "Meld Use at Exit:\n" << *(msc->use_at_exit(exit_op)) << endl;
    cdbg << "Meld Def at Exit:\n" << *(msc->def_at_exit(exit_op)) << endl;
  }
}

void ModuloScheduler::copy_entry_meld_info 
(RR_allocation& rr_alloc, Latency_map &init_map, Latency_map *final_map){
  for (Latency_map_iterator mi(init_map) ; mi != 0 ; mi++) {
    Operand& curr_operand = (*mi).first;
    int value = (*mi).second;
    if (!final_map->is_bound (curr_operand) ||
	(final_map->value (curr_operand) > value))
      final_map->bind (curr_operand, value);

    // Compute additional dangles for other iterations
    if (curr_operand.is_reg() && curr_operand.is_rotating()) {
      assert(curr_operand.allocated());
      // make a copy for evr manipulation
      Operand evr_operand(curr_operand);
      evr_operand.unbind_reg();
      for (value += good_ii; (value > 0) && (value <= MDES_max_op_lat); 
	   value += good_ii) {
	evr_operand.incr_omega(-1);
	// make another copy to compute register allocation
	Operand new_operand(evr_operand);
	new_operand.bind_reg(rr_alloc.get_entry_register_allocation(evr_operand));
	// no need to continue if the new operand is already included
	if (!final_map->is_bound (new_operand) ||
	    (final_map->value (new_operand) > value))
	  final_map->bind (new_operand, value);
      }
    }
  }
}

void ModuloScheduler::copy_exit_meld_info 
(RR_allocation& rr_alloc, Latency_map &init_map, Latency_map *final_map){
  for (Latency_map_iterator mi(init_map) ; mi != 0 ; mi++) {
    Operand& curr_operand = (*mi).first;
    int value = (*mi).second;

    // Compute additional dangles for other iterations
    if (curr_operand.is_reg() && curr_operand.is_rotating()) {
      assert(curr_operand.allocated());
      // make a copy for evr manipulation
      Operand evr_operand(curr_operand);
      evr_operand.unbind_reg();
      for (; (value > 0); value -= good_ii) {
	evr_operand.incr_omega(1);
	// make another copy to compute register allocation
	Operand new_operand(evr_operand);
	new_operand.bind_reg(rr_alloc.get_exit_register_allocation(evr_operand));
	// no need to continue if the new operand is already included
	if (!final_map->is_bound (new_operand) ||
	    (final_map->value (new_operand) < value))
	  final_map->bind (new_operand, value);
      }
    }
  }
}

void ModuloScheduler::delete_local_structures(){
    delete_added_edges_if_necessary();
    delete minDist;
    delete nhbrs;
    delete RUmap;
    delete edge_filter;

    for (Hash_map_iterator<Op*, SchedInfo*> hmi(*sInfoMap); hmi!=0; hmi++)
      delete (*hmi).second;

    delete sInfoMap;

    if (_free_tracware) delete Tracker;
}

void ModuloScheduler::delete_added_edges_if_necessary(){
  if (!delete_added_edges)
    return;

  // delete added_edges
  while (!added_edges->is_empty()) {
    Edge *edge = added_edges->pop();
    edge->src()->remove_outedge(edge, edge->src_port_type(), edge->src_port());
    edge->dest()->remove_inedge(edge, edge->dest_port_type(), 
                                edge->dest_port());
    delete edge;
  }
  delete added_edges;
  return ;
}



void ModuloScheduler::manually_adjust_edges(void) {
  ifstream fin;
  fin.open("latency_adjustment.txt");
  if (fin.fail()) return;

  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";
  cdbg << "STARTING MANUAL ADJUSTMENT!!!!!!!\n";

  do {
    bool found = false;
    int src=-1, dest=-1;
    fin >> src >> dest;
    if (fin.fail()) break;
    for (Region_all_ops opi(loopbody); opi!=0; opi++) {
      Op* curr_op = *opi;
      if (curr_op->id() != src) continue;

      for (Op_outedges outedgei(curr_op); outedgei!=0; outedgei++) {
        Edge* e = *outedgei;
        if (e->dest()->id() != dest) continue;

        cdbg << "Edge " << *e << " latency being changed from " << e->latency() << " to " << e->latency() + 1 << endl;

        e->set_latency(e->latency() + 1);
        found = true;

        break;
      }

      break;
    }

    if (!found) El_punt("Didn't find an edge between op %d and %d",src,dest);

  }while (!fin.eof());
  fin.close();
}



/*
static void print_minDist(Hash_map<Op*, int> *nodeMap, Matrix<int> *minDist);

// this routine prints out the minDist array in LaTeX format
static void print_minDist(Hash_map<Op*, int> *nodeMap, Matrix<int> *minDist){
  int i, j;
  Vector<eString> opcodes(numnodes);

  for (Hash_map_iterator<Op*, int> iter(*nodeMap); iter != 0; iter++) {
    opcodes[(*iter).second] = (*iter).first->sched_opcode();
  }
  cout << "\\documentstyle[landscape]{report}\n";
  cout << "\\setlength{\\topmargin}{-.50in}\n";
  cout << "\\setlength{\\oddsidemargin}{-.5in}\n";
  cout << "\\setlength{\\evensidemargin}{-.5in}\n";
  cout << "\\setlength{\\textwidth}{8.30in}\n";

  cout << "\\begin{document}\n\\[\n\\begin{array}{";
  for (i = 0; i <= numnodes; i++) cout << "l";
  cout << "}" << endl;
  for (i = 0; i <= numnodes; i++) {
    for (j = 0; j <= numnodes; j++) {
      if (i == 0) {   // print column headings
        if (j != 0) cout << opcodes[j-1];
      } else {
        if (j == 0) {  // print row heading
          cout << opcodes[i-1];
        } else {
          if ((*minDist)[i-1][j-1] == minus_infinity) {
            cout << "-\\infty";
          } else {
            cout << (*minDist)[i-1][j-1];
          }
        }
      }
      if (j < numnodes) cout << " & ";
    }
    if (i != numnodes) cout << "\\\\" << endl;
  }
  cout << "\n\\end{array}\n\\]\n\\end{document}" << endl;
}

// this routine prints out  an iteration history in LaTeX format
// it depends upon there being a simple unit model
static void printSched(RU *RUmap, int II, int length){
  int numstages = (length / II) + 1;
  int schedlength = numstages * II;
  int num_units = MDES_num_resources();
  Matrix<eString *> schedule(schedlength, num_units, 0);
  for (int stage = 0; stage < numstages; stage++) {
    for (int cycle = 0; cycle < II; cycle++) {
      for (int unit = 0; unit < num_units; unit++) {
        Op *op = RUmap->at(cycle, unit);
        if (!op) continue;
        int row = stage*II + op->sched_time();
        int column = unit;
        if (row < schedlength) {
          if (schedule[row][column]) {
            cout << "WARNING! overlap in schedule detected" << endl;
          } else {
            char buf[30];
            sprintf(buf, "%d:%d ", stage, op->id());
            eString *opstring = new eString(buf);
            schedule[row][column] = opstring;
          }
        }
      }
    }
  }
  cout << "%%Iteration record\n";
  cout << "\\documentstyle[landscape]{report}\n";
  cout << "\\setlength{\\topmargin}{-.50in}\n";
  cout << "\\setlength{\\oddsidemargin}{-.5in}\n";
  cout << "\\setlength{\\evensidemargin}{-.5in}\n";
  cout << "\\setlength{\\textwidth}{8.30in}\n";

  cout << "\\begin{document}\n\\begin{tabular}{";
  for (int column = 0; column < num_units ; column++) cout << "|l";
  cout << "|}\n\\hline\n";
  for (int row = 0; row < schedlength; row++) {
    for (column = 0; column < num_units; column++) {
      if (schedule[row][column]) {
        char *str = (char *) *schedule[row][column];
        cout << str;
      } else {
        cout << " ";
      }
      if (column < num_units - 1) cout << "&";
    }
    if (row < schedlength - 1) {
      if ((row + 1) % II) {
        cout << "\\\\ \\hline" << endl;
      } else {
        cout << "\\\\ \\hline \\hline" << endl; 
      }
    }
  }
  cout << "\\\\ \\hline\n\\end{tabular}\n\\end{document}" << endl;
}

*/
