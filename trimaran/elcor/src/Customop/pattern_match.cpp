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

Copyright 2007 University of Michigan

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
//      File:           pattern_match.cpp
//      Author:         Hongtao Zhong
//      Created:        December 2002
//      Description:    Finds dataflow patterns defined in the MDES and
//                      replaces them with custom instructions.
//
/////////////////////////////////////////////////////////////////////////////


#include "dbg.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "operand.h"
#include "op.h"
#include "opcode_properties.h"
#include "el_control.h"
#include "mdes.h"
#include "intf.h"
#include "el_init.h"
#include "edge_drawing.h"
#include "connect.h"
#include "el_bb_tools.h"
#include "edge_utilities.h"
#include "codegen.h"
#include "bnb_mapper.h"
#include "el_dot_tools.h"
#include "graph.h"
#include "pattern_match_node.h"
#include "pattern_match.h"
#include <deque>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

Hash_set<Op*> ugly_hack_ops(hash_op_ptr, 7853);

// A list of all the patterns (custom ops)
List<DirectedGraph<Pattern_match_node>* > cop_patterns;
deque<deque<bool> > pattern_node_valid;
#define NO_OPERAND (-1)
deque<deque<int> >  pattern_src_num, pattern_dest_num;


// global statistics
Vector<int> cop_match_count; //# of matches for each custom op.
Vector<double> cop_dmatch_count; //# of dynamic instructions that match
Hash_map<Custom_op_descr*, int> cop_static_count(hash_custom_op_descr,128);
Hash_map<Custom_op_descr*, double> cop_dynamic_count(hash_custom_op_descr,128);
Hash_map<Custom_op_descr*, double> cop_dynamic_cycles(hash_custom_op_descr,128);

bool cop_patterns_initialized = false;
int bb_pattern_count;
Hash_map<Op*,int> op_order_map(hash_op_ptr,256);


// compare two nodes, used for the graph to compare attribute of nodes
// op1 and op2 are Pattern_node*s
bool new_node_compat(Pattern_match_node* n1, Pattern_match_node* n2)
{
  return ((*n1) == (*n2));
}


DirectedGraph<Pattern_match_node>*
build_region_graph(Compound_region* reg, Hash_map<int, Op*>& node_to_op_map,
		  List<Pattern_match_node*>& to_delete)
{
  DirectedGraph<Pattern_match_node>* new_graph =
    new DirectedGraph<Pattern_match_node>();
  assert(new_graph && "out of memory");

//   cdbg << "Build region " << reg->id() << endl;

  // add nodes into graph
  Hash_map<int, Pattern_match_node*> literal_nodes(hash_int, 71);
  Hash_map<int, unsigned> literal_ids(hash_int, 71);
  Hash_map<Operand, Pattern_match_node*> register_nodes(hash_operand, 71);
  Hash_map<Operand, unsigned> register_ids(hash_operand, 71);
  Hash_map<Op*, Vector<unsigned> > op_dest_ids(hash_op_ptr, 1021);
  Hash_map<Op*, Vector<unsigned> > op_src_ids(hash_op_ptr, 1021);

  for(Region_ops_C0_order opi(reg); opi != 0; opi++) {
    Op* op = *opi;

    if(is_pseudo(op))
      continue;

    // Each operand gets a node in the graph so we can differentiate
    // which dataflow edges line up where in the custom op. It's also
    // necessary to tell which patterns are legal when an op's inputs
    // aren't commutative.
    deque<unsigned> src_ids;
    for(Op_explicit_inputs oes(op); oes != 0; oes++) {
      Operand& oper = *oes;

      Pattern_match_node* pmn = new Pattern_match_node(PM_OPERATION_INPUT);
      assert(pmn && "out of memory");
      to_delete.add_tail(pmn);
      pmn->add_opcode(op->opcode());
      pmn->set_operand_num(oes.get_port_num());
      unsigned src_id = new_graph->insert_node(pmn);
      node_to_op_map.bind(src_id, op);
      src_ids.push_back(src_id);

//       cdbg << src_id << "\top: " << op->id() << "  src: " << oes.get_port_num() << endl;

      if( ! op_src_ids.is_bound(op)) {
	op_src_ids.bind(op, Vector<unsigned>(MAX_SRC_PORTS, 0));
      }

      if(op_src_ids[op][oes.get_port_num()] != 0) {
	// This breaks one of my assumptions.
// 	cdbg << "This op has multiple operands in the same input port!\n"
// 	     << *op << endl;
	assert(0);
      }
      op_src_ids[op][oes.get_port_num()] = src_id;

      // Create a node for a literal value or an externally defined
      // register, if necessary.
      if(oper.is_int()) {
	int value = oper.int_value();

	if(literal_nodes.is_bound(value)) {
	  new_graph->insert_edge(literal_ids[value], src_id);
	} else {
	  Pattern_match_node* new_lit = new Pattern_match_node(PM_CONST);
	  assert(new_lit && "out of memory");
	  to_delete.add_tail(new_lit);

	  Const_set_descr* desc = new Const_set_descr(NULL);
	  assert(desc && "out of memory");
	  desc->set_mins(Vector<int>(1, value));
	  desc->set_maxes(Vector<int>(1, value));
	  desc->set_steps(Vector<int>(1, 1));

	  new_lit->set_consts(desc);
	  literal_nodes.bind(value, new_lit);
	  unsigned lit_id = new_graph->insert_node(new_lit);

// 	  cdbg << lit_id << "\tlit: " << value << endl;

	  literal_ids.bind(value, lit_id);
	  node_to_op_map.bind(lit_id, NULL);
	  new_graph->insert_edge(lit_id, src_id);
// 	  cdbg << "\tLit edge from  " << lit_id << " to   " << src_id << endl;
	}
      } else if(oper.is_reg() || oper.is_macro_reg()) {
	if( ! register_nodes.is_bound(oper)) {

	  // Need to create nodes for register inputs created outside
	  // the block. This is necessary if we want to be able to
	  // identify when a custom operation has multiple sources
	  // that come from the same register (e.g. 2 parallel ADDs
	  // that both shared an input value, meaning the custom
	  // operation can have at most 3 inputs).

	  Pattern_match_node* new_reg = new Pattern_match_node(PM_ANY_OUTPUT);
	  assert(new_reg && "out of memory");
	  to_delete.add_tail(new_reg);

	  register_nodes.bind(oper, new_reg);
	  unsigned reg_id = new_graph->insert_node(new_reg);
	  register_ids.bind(oper, reg_id);
	  node_to_op_map.bind(reg_id, NULL);

// 	  cdbg << "GI  " << reg_id << "\top: " << op->id() << "  src: " << oes.get_port_num() << endl;

	  new_graph->insert_edge(reg_id, src_id);
// 	  cdbg << "\tGI1 edge from  " << reg_id << " to   " << src_id << endl;
	} else {
	  if(register_nodes[oper]) {
	    new_graph->insert_edge(register_ids[oper], src_id);
// 	    cdbg << "\tGI2 edge from  " << register_ids[oper] << " to   " << src_id << endl;
	  } else {
	    // there will be a flow edge
	  }
	}
      }

    } // end for all explicit sources


    for(Op_explicit_dests oed(op); oed != 0; oed++) {
      Operand& oper = *oed;

      if(register_nodes.is_bound(oper))
	register_nodes.unbind(oper);

      Pattern_match_node* pmn = new Pattern_match_node(PM_OPERATION_OUTPUT);
      assert(pmn && "out of memory");
      to_delete.add_tail(pmn);
      pmn->add_opcode(op->opcode());
      pmn->set_operand_num(oed.get_port_num());
      unsigned dst_id = new_graph->insert_node(pmn);
      node_to_op_map.bind(dst_id, op);

//       cdbg << dst_id << "\top: " << op->id() << "  dst: " << oed.get_port_num() << endl;

      // Add an edge from each input to each output.
      for(deque<unsigned>::iterator it = src_ids.begin(); it != src_ids.end(); it++) {
	new_graph->insert_edge(*it, dst_id);
// 	cdbg << "\tedge from  " << *it << " to   " << dst_id << endl;
      }

      if( ! op_dest_ids.is_bound(op)) {
	op_dest_ids.bind(op, Vector<unsigned>(MAX_DEST_PORTS, 0));
      }

      if(op_dest_ids[op][oed.get_port_num()] != 0) {
	// This breaks one of my assumptions.
// 	cdbg << "This op has multiple operands in the same output port!\n"
// 	     << *op << endl;
	assert(0);
      }
      op_dest_ids[op][oed.get_port_num()] = dst_id;

      // Mark that we don't need to generate a external region input
      // node for this register.
      if((oper.is_reg() || oper.is_macro_reg()) && ! register_nodes.is_bound(oper))
	register_nodes.bind(oper, NULL);

    } // end for all explicit dests

  } // end for all ops

  // We should now have a node in the graph for the input and output
  // operands of every operation in the region, with the inputs
  // connected to the outputs within an operation. Now we need to
  // set up the inter-node operand connections.

  // insert flow edges, for every node, add the outgoing flow edge
  for(Region_ops_C0_order opj(reg); opj != 0; opj++) {
    Op* src = *opj;

    if(is_pseudo(src))
      continue;

    for(Op_outedges oe(src, FLOW_EDGES); oe != 0; oe++) {
      Edge* ed = *oe;
      Op* dest = ed->dest();

      // Look up the map to find out the node for ops.  The code looks
      // kind of confusing, because the maps are named op-centric, but
      // the edge data structures are edge-centric. Thus the dest of
      // an op is the src of an edge.
      int node_i = op_dest_ids[src][ed->src_port()];
      int node_j = op_src_ids[dest][ed->dest_port()];

      // insert the edge
      new_graph->insert_edge(node_i, node_j);
//       cdbg << "\tedge from  " << node_i << " to   " << node_j << endl;
    }
  }

//   cdbg << endl << endl;

  return new_graph;
}


DirectedGraph<Pattern_match_node>* build_pattern_graph(Custom_op_descr* pattern,
						       deque<bool>& valid_nodes,
						       deque<int>& src_ports,
						       deque<int>& dest_ports)
{
  DirectedGraph<Pattern_match_node>* new_graph
    = new DirectedGraph<Pattern_match_node>();
  assert(new_graph && "out of memory");

  Hash_map<Pattern_node*, Vector<unsigned> > node_to_src_id(hash_pattern_node_ptr, 71);
  Hash_map<Pattern_node*, Vector<unsigned> > node_to_dest_id(hash_pattern_node_ptr, 71);
  // This catches node ids, that aren't in the two hash maps above.
  Hash_map<Pattern_node*, unsigned> node_to_some_other_id(hash_pattern_node_ptr, 71);


  // FIX: DON'T LEAK MEMORY, DUMBASS!
  for(unsigned i = 0; i < pattern->get_pattern_size(); i++) {
    Pattern_node* node = pattern->get_pattern_node(i);
    if(node->get_type() == COP_OPERATION) {
      Vector<unsigned> src_ids(MAX_SRC_PORTS, 0);
      Vector<unsigned> dest_ids(MAX_DEST_PORTS, 0);
      for(unsigned j = 0; j < node->num_srcs(); j++) {
	Pattern_match_node* pmn = new Pattern_match_node(PM_OPERATION_INPUT);
	assert(pmn && "out of memory");
	const Hash_set<eString> opcodes = node->get_opcodes();
	for(Hash_set_iterator<eString> hsi(opcodes); hsi != 0; hsi++) {
	  pmn->add_opcode(el_string_to_opcode_map[*hsi]);
	}
	pmn->set_operand_num(SRC1 + j);
	src_ids[SRC1 + j] = new_graph->insert_node(pmn);
	valid_nodes.push_back(true);
	src_ports.push_back(SRC1 + j);
	dest_ports.push_back(NO_OPERAND);

      }
      for(unsigned j = 0; j < node->num_dests(); j++) {
	Pattern_match_node* pmn = new Pattern_match_node(PM_OPERATION_OUTPUT);
	assert(pmn && "out of memory");
	const Hash_set<eString> opcodes = node->get_opcodes();
	for(Hash_set_iterator<eString> hsi(opcodes); hsi != 0; hsi++) {
	  pmn->add_opcode(el_string_to_opcode_map[*hsi]);
	}
	pmn->set_operand_num(DEST1 + j);
	dest_ids[DEST1 + j] = new_graph->insert_node(pmn);
	valid_nodes.push_back(true);
	src_ports.push_back(NO_OPERAND);
	dest_ports.push_back(DEST1 + j);

	// Connect this dest with all the sources.
	for(unsigned k = 0; k < node->num_srcs(); k++) {
	  new_graph->insert_edge(src_ids[SRC1 + k], dest_ids[DEST1 + j]);
	}
      }
      node_to_src_id.bind(node, src_ids);
      node_to_dest_id.bind(node, dest_ids);

    } else if(node->get_type() == COP_LITERAL) {
      Pattern_match_node* pmn = new Pattern_match_node(PM_CONST);
      assert(pmn && "out of memory");
      pmn->set_consts(node->get_consts());
      unsigned this_id = new_graph->insert_node(pmn);
      valid_nodes.push_back(false);
      src_ports.push_back(NO_OPERAND);
      dest_ports.push_back(NO_OPERAND);
      node_to_some_other_id.bind(node, this_id);

    } else if(node->get_type() == COP_LIVE_IN) {

      // A live in is the output of any node
      Pattern_match_node* pmn = new Pattern_match_node(PM_ANY_OUTPUT);
      assert(pmn && "out of memory");
      unsigned this_id = new_graph->insert_node(pmn);
      valid_nodes.push_back(false);
      src_ports.push_back(NO_OPERAND);
      dest_ports.push_back(NO_OPERAND);
      node_to_some_other_id.bind(node, this_id);

    } else if(node->get_type() == COP_LIVE_OUT) {

      // A live out is the input of any node
      Pattern_match_node* pmn = new Pattern_match_node(PM_ANY_INPUT);
      assert(pmn && "out of memory");
      unsigned this_id = new_graph->insert_node(pmn);
      valid_nodes.push_back(false);
      src_ports.push_back(NO_OPERAND);
      dest_ports.push_back(NO_OPERAND);
      node_to_some_other_id.bind(node, this_id);

    } else {
      cdbg << "I don't know what to do with this kind of node:\n"
 	   << (*node) << endl;
      assert(0);
    }
  } // end for all nodes

  // Add inter-node edges into the graph
  for(unsigned i = 0; i < pattern->get_pattern_size(); i++) {
    Pattern_node* node = pattern->get_pattern_node(i);

    unsigned from, to;

    // Only adding edges for srcs, since, presumably, every src will
    // have a dest somewhere in the graph.
    for(unsigned j = 0; j < node->num_srcs(); j++) {
      
      bool oper_is_internal = true;
      if(node->get_type() == COP_LIVE_OUT) {
	oper_is_internal = false;
	to = node_to_some_other_id[node];
      } else if(node->get_type() == COP_OPERATION) {
	to = node_to_src_id[node][SRC1 + j];
      } else {
	cdbg << "I don't know why this node has sources:\n" << *node << endl;
	assert(0);
	// To get rid of a gcc warning.
	to = 0;
      }

      bool any_external_refs = false;
      Pattern_edge* edge = node->get_src(j);
      for(unsigned k = 0; k < edge->num_srcs(); k++) {
	Pattern_node* src_node = edge->get_src(k);
	if(node_to_some_other_id.is_bound(src_node)) {
	  oper_is_internal = false;
	  from = node_to_some_other_id[src_node];
	} else {
	  // Need to figure out which dest port this edge came
	  // from. FIX: should probably just add the ports into the Edge
	  // class.
	  unsigned dport = 0;
	  for(; dport < src_node->num_dests(); dport++)
	    if(src_node->get_dest(dport) == edge)
	      break;
	  assert(dport < src_node->num_dests());
	  from = node_to_dest_id[src_node][DEST1 + dport];
	}
	
	if(oper_is_internal) {
	  // Mark that this operand does not need to be in the list of
	  // srcs and dests in the custom instruction we create for this
	  // pattern.
	  dest_ports[from] = NO_OPERAND;
	} else {
	  any_external_refs = true;
	}
	
	new_graph->insert_edge(from, to);
      }

      if( ! any_external_refs) {
	src_ports[to] = NO_OPERAND;
      }

    }

  }

  return new_graph;
}


static Op* create_custom_op(Custom_op_descr* cop_descr,
			    Vector<Op*>& match_instance,
			    const deque<Operand>& real_srcs,
			    const deque<Operand>& real_dests)
{
  char* cop_name = cop_descr->get_opcode();
  int src_num = MDES_src_num(cop_name);
  int dest_num = MDES_dest_num(cop_name);
  bool predicated = MDES_predicated(cop_name);
  Op* cop = new Op(CUSTOM, src_num, dest_num, predicated);
  cop->set_custom_opcode(cop_name);

  for(unsigned i = 0; i < real_srcs.size(); i++) {
    cop->set_src(SRC1 + i, real_srcs[i]);
  }

  for(unsigned i = 0; i < real_dests.size(); i++) {
    cop->set_dest(DEST1 + i, real_dests[i]);
  }

  // FIX: this is wrong.
  // pred of the cop is the pred of the first matched op.
  cop->set_src(PRED1, match_instance[0]->src(PRED1));

  return cop;
}


// FIX: is all this really neccesary?
void collect_true_dests(const Vector<Op*>& match_instance,
			Hash_set<Operand>& dests)
{
  dests.clear();
  //  Hash_set<Operand> written_to(hash_operand, 13);
  for(unsigned i = 0; i < match_instance.size(); i++) {
    Op* op = match_instance[i];
    for(Op_explicit_dests oad(op); oad != 0; oad++) {
      if((*oad).is_reg() || (*oad).is_macro_reg()) 
        dests += *oad;
    }
  }
  
  // changed to just return all dests.
  // there are problems with reordering otherwise
  // nate will account for it somewhere else - mchu 3/18
  return;

#if 0

  bool pay_attention = false;
  List<Region*> a_queue;
  a_queue.add_tail(el_get_enclosing_procedure(match_instance[0]));
  while( ! a_queue.is_empty()) {
    Region* r = a_queue.pop();
    Alist_region_tiling_graph sub_regs(r, GRAPH_OPS_PLUS);
    for (Alist_graph_bfs r_iter(sub_regs, DOWN); r_iter != 0; r_iter++) {
      int index = *r_iter;
      Region* reg = sub_regs.b_map[index];
      if(reg->is_bb() || reg->is_hb()) {
	for(Region_ops_C0_order rops((Compound_region*)reg); rops != 0; rops++) {
	  Op* op = *rops;
	  if( ! pay_attention) {
            // match_instance Vector isn't ordered anymore, can't depend on [0]
            // being the earliest op in the custom_op  - mchu 3/18
            //	    if(op == match_instance[0]) {
            if(match_instance.find(op) != -1) {
	      pay_attention = true;
	    }
	  } else {
	    if(match_instance.find(op) != -1)
	      continue;

	    for(Op_all_sources oas(op); oas != 0; oas++) {
	      if((*oas).is_reg() || (*oas).is_macro_reg()) {
		if(written_to.is_member(*oas)) {
		  dests += *oas;
		  written_to -= *oas;
		}
	      }
	    }

	    for(Op_explicit_dests oed(op); oed != 0; oed++) {
	      if((*oed).is_reg() || (*oed).is_macro_reg()) {
		if(written_to.is_member(*oed)) {
		  written_to -= *oed;
		}
	      }
	    }

	    if(written_to.size() == 0) {
	      return;
	    }

	  }
	}
      } else {
	a_queue.add_tail(reg);
      }
    }
  }
#endif
}

/*=========================================================================*/
/* New version of reorder function + helper routines - SAM 11-04 */
/*=========================================================================*/

static void print_match_instance(Vector<Op*> &match_instance)
{
  cdbg << "Match instance: ";
  for (int i=0; i<match_instance.dim(); i++) {
    cdbg << match_instance[i]->id() << ", ";
  }
  cdbg << endl;
}

static void construct_op_order_map(Compound_region *region, Hash_map<Op*,int> &op_order)
{
  op_order.clear();

  int index = 0;
  for (Region_ops_C0_order opi(region); opi!=0; opi++) {
    Op *op = *opi;
    op_order.bind(op, index);
    index++;
  }
}
static void find_first_last_match_ops(const Vector<Op*> &match_instance, 
				      Hash_map<Op*,int> &op_order, 
				      Op* &match_first, Op* &match_last)
{
  int min_val = ELCOR_MAX_INT, min_index = -1;
  int max_val = ELCOR_MIN_INT, max_index = -1;
  for (int i=0; i<match_instance.dim(); i++) {
    Op *op = match_instance[i];
    int pos = op_order.value(op);
    if (pos < min_val) {
      min_val = pos;
      min_index = i;
    }
    if (pos > max_val) {
      max_val = pos;
      max_index = i;
    }
  }

  match_first = match_instance[min_index];
  match_last = match_instance[max_index];

#ifdef DEBUG
  cdbg << "find_first_last_match_ops: Instance first = " << match_first->id()
       << ", last = " << match_last->id() << endl;
#endif
}

// FIX: ugly hack
Hash_set<Compound_region*> bbs_created(hash_cmpd_region_ptr, 37);

static Basicblock *insert_cop_compensation_bb(Compound_region *reg, Op *branch)
{
#ifdef DEBUG
  cdbg << "Enter insert_cop_compensation_bb" << endl;
#endif
  Compound_region *parent = reg->parent();
  assert(parent->is_procedure());

  /* Create new bb with a jump to the target of branch */
  Edge *tk_edge = get_outgoing_CONTROL0_non_fall_through_edge(branch);
  assert(tk_edge  !=  NULL);
  assert(tk_edge->src() == branch);
  Control_flow_freq *cfreq = get_control_flow_freq(tk_edge);
  assert(cfreq != NULL);
  Op *dest_op = tk_edge->dest();
  Basicblock *new_bb = El_create_jump_bb(parent, cfreq->freq, dest_op);

#ifdef DEBUG
  cdbg << "\tCreating new basic block " << new_bb->id() << endl;
#endif

  /* Remove tk_edge from original target region_inedges */
  dest_op->remove_inedge_recursively(tk_edge);
  
  /* Now adjust branch to target first op of compensation block */
  /* Note the pbr can be fixed later, just get the edges right */
  Op *first_op = get_first_region_op_from_subregions(new_bb);
  tk_edge->set_dest(first_op, CONTROL0, tk_edge->dest_alt());
  first_op->add_inedge_recursively(tk_edge);

  bbs_created += new_bb;
  return new_bb;
}

// Note, this routine should not insert any branches!
static void insert_copy_of_ops_at_top_of_region(Compound_region *reg, Dlist<Op*> &to_insert)
{
#ifdef DEBUG
  cdbg << "Inserting copy of operations at top of " << reg->id() << endl;
#endif
  for (Dlist_iterator<Op*> li(to_insert, true); li != 0; li--) {
    Op *op = *li;
    assert (! is_branch(op));
    Op *copy_op = new Op(*op);
    copy_op->renumber();

    ugly_hack_ops += op;
    ugly_hack_ops += copy_op;

    Custom_op_list_attr* cop_attr = get_custom_op_list_attr(op), *new_attr;
    if(cop_attr != NULL) {
      new_attr = new Custom_op_list_attr;
      assert(new_attr && "ran out of memory");
      for(List_iterator<Op*> opi(cop_attr->op_list); opi != 0; opi++) {
	Op* new_op = new Op(**opi);
	new_op->renumber();
	new_attr->op_list.add_tail(new_op);
	ugly_hack_ops += new_op;
      }
      set_custom_op_list_attr(copy_op, new_attr);
    }

    /* When you clone an op you also get all the edges, so we need to go delete
    the redundant edges */
    List<Edge*> to_remove;
    for (Op_outedges oe(copy_op); oe != 0; oe++) {
      Edge *e = *oe;
      to_remove.add_tail(e);
    }
    for (List_iterator<Edge*> li(to_remove); li != 0; li++) {
      Edge *e = *li;
      copy_op->remove_outedge(e, e->src_port_type(), e->src_port(), e->src_alt());
    }
    to_remove.clear();
    for (Op_inedges ie(copy_op); ie != 0; ie++) {
      Edge *e = *ie;
      to_remove.add_tail(e);
    }
    for (List_iterator<Edge*> li(to_remove); li != 0; li++) {
      Edge *e = *li;
      copy_op->remove_inedge(e, e->dest_port_type(), e->dest_port(), e->dest_alt());
    }
#ifdef DEBUG
    cdbg << "\tInsert: copy " << copy_op->id() << " original " << op->id() << endl;
#endif
    El_insert_op_after_merge(reg, copy_op);
  }
}

static void copy_ops_to_compensation_block(Compound_region *reg, Op *branch_op,
					   Dlist<Op*> &to_insert)
{
  Edge *tk_edge = get_outgoing_CONTROL0_non_fall_through_edge(branch_op);
  assert(tk_edge != NULL);
  assert(tk_edge->src() == branch_op);
  Op *dest_op = tk_edge->dest();
  int num_predecessors = 0;
  for (Op_inedges ini(dest_op, CONTROL0_INEDGES); ini != 0; ini++) {
    num_predecessors++;
  }

  Compound_region *target_reg = NULL;
  assert (num_predecessors > 0);
  if (num_predecessors > 1)
    target_reg = insert_cop_compensation_bb(reg, branch_op);
  else
    target_reg = dest_op->parent();

  insert_copy_of_ops_at_top_of_region(target_reg, to_insert);

  // fix branch target attribute
  remove_branch_target_attr(branch_op);

  // mark the branch target for ARM translator
  char bbnum_str[80];		  
  sprintf(bbnum_str, "%d", target_reg->id());
  Region *parent = branch_op;
    
  while ( ! parent->is_procedure())
    parent = parent->parent();
  string proc_name = ((Procedure*)parent)->get_name();  
  string label = (proc_name + "_bb_" + bbnum_str);
  Branch_target_attr* bta = new Branch_target_attr();
  assert(bta);
  bta->label = label;
  set_branch_target_attr(branch_op, bta);
}

void create_predecessors_flow(const Vector<Op*>& ops, Hash_set<Op*>& preds)
{
  preds.clear();
  if( ! ops.size())
    return;

  Hash_map<Op*,int> op_order(hash_op_ptr, 32);
  /* Record original sequential order of operations in region */
  construct_op_order_map(ops[0]->parent(), op_order);

  /* Find all direct/indirect predecessors of the match instance */
  Hash_set<Op*> to_process(hash_op_ptr, 32);
  for (unsigned i = 0; i < ops.size(); i++) {
    to_process += ops[i];
  }
  while (! to_process.is_empty()) {
    Op *op = to_process.pop();
    preds += op;
    for (Op_inedges ine(op, FLOW_EDGES); ine != 0; ine++) {
      Edge *e = *ine;
      // Skip C1 edges...
      if (e->is_control())
        continue;

      // and backedges.
      if(is_switch(e->src()))
	continue;

      Op *pred_op = e->src();
      if (! preds.is_member(pred_op)) {
	to_process += pred_op;
      }
    }
  }

  /* Take away the ops in match_instance from the set */
  for (unsigned i = 0; i < ops.size(); i++) {
    preds -= ops[i];
  }
}

void create_successors_flow(const Vector<Op*>& ops, Hash_set<Op*>& succs)
{
  succs.clear();
  if( ! ops.size())
    return;

  Hash_map<Op*,int> op_order(hash_op_ptr, 32);
  /* Record original sequential order of operations in region */
  construct_op_order_map(ops[0]->parent(), op_order);

  /* Find all direct/indirect predecessors of the match instance */
  Hash_set<Op*> to_process(hash_op_ptr, 32);
  for (unsigned i = 0; i < ops.size(); i++) {
    to_process += ops[i];
  }
  while (! to_process.is_empty()) {
    Op *op = to_process.pop();
    succs += op;
    for (Op_outedges oute(op, FLOW_EDGES); oute != 0; oute++) {
      Edge *e = *oute;
      // Skip C1 edges...
      if (e->is_control())
        continue;

      // and backedges.
      if(is_merge(e->dest()))
	continue;

      Op *succ_op = e->dest();
      if (! succs.is_member(succ_op)) {
	to_process += succ_op;
      }
    }
  }

  /* Take away the ops in match_instance from the set */
  for (unsigned i = 0; i < ops.size(); i++) {
    succs -= ops[i];
  }
}

void create_predecessors(const Vector<Op*>& ops, Hash_set<Op*>& preds)
{
  preds.clear();
  if( ! ops.size())
    return;

  Hash_map<Op*,int> op_order(hash_op_ptr, 32);
  /* Record original sequential order of operations in region */
  construct_op_order_map(ops[0]->parent(), op_order);

  /* Find all direct/indirect predecessors of the match instance */
  Hash_set<Op*> to_process(hash_op_ptr, 32);
  for (unsigned i = 0; i < ops.size(); i++) {
    to_process += ops[i];
  }
  while (! to_process.is_empty()) {
    Op *op = to_process.pop();
    preds += op;
    for (Op_inedges ine(op, NON_CONTROL0_INEDGES); ine != 0; ine++) {
      Edge *e = *ine;
      // Skip C1 edges...
      if (e->is_control())
        continue;

      // and backedges.
      if(is_switch(e->src()))
	continue;

      Op *pred_op = e->src();
      if (! preds.is_member(pred_op)) {
	to_process += pred_op;
      }
    }
  }

  /* Take away the ops in match_instance from the set */
  for (unsigned i = 0; i < ops.size(); i++) {
    preds -= ops[i];
  }
}

void create_successors(const Vector<Op*>& ops, Hash_set<Op*>& succs)
{
  succs.clear();
  if( ! ops.size())
    return;

  Hash_map<Op*,int> op_order(hash_op_ptr, 32);
  /* Record original sequential order of operations in region */
  construct_op_order_map(ops[0]->parent(), op_order);

  /* Find all direct/indirect predecessors of the match instance */
  Hash_set<Op*> to_process(hash_op_ptr, 32);
  for (unsigned i = 0; i < ops.size(); i++) {
    to_process += ops[i];
  }
  while (! to_process.is_empty()) {
    Op *op = to_process.pop();
    succs += op;
    for (Op_outedges oute(op, NON_CONTROL0_OUTEDGES); oute != 0; oute++) {
      Edge *e = *oute;
      // Skip C1 edges...
      if (e->is_control())
        continue;

      // and backedges.
      if(is_merge(e->dest()))
	continue;

      Op *succ_op = e->dest();
      if (! succs.is_member(succ_op)) { 
	to_process += succ_op;
      }
    }

  }

  /* Take away the ops in match_instance from the set */
  for (unsigned i = 0; i < ops.size(); i++) {
    succs -= ops[i];
  }
}


bool is_replaceable_together(const Vector<Op*>& v1, const Vector<Op*>& v2)
{
  Vector<Op*> match_instance(v1.size() + v2.size());
  unsigned i = 0;
  for(unsigned j = 0; j < v1.size(); j++) {
    match_instance[i++] = v1[j];
  }
  for(unsigned j = 0; j < v2.size(); j++) {
    match_instance[i++] = v2[j];
  }

  return is_replaceable(match_instance);
}


bool is_replaceable(const Vector<Op*>& match_instance)
{
  Hash_set<Op*> preds(hash_op_ptr,32);
  Hash_set<Op*> succs(hash_op_ptr,32);
  Hash_set<Op*> span(hash_op_ptr,32);
  Hash_map<Op*,int> op_order(hash_op_ptr, 32);
  Op *match_first, *match_last;
  Compound_region* region = match_instance[0]->parent();

  bool has_compare = false;

  for(unsigned j = 0; j < match_instance.size(); j++) {
    Op* tmp_op = match_instance[j];
    if(is_cmpp(tmp_op) || is_cmpr(tmp_op) || is_fcmpp(tmp_op) || is_fcmpr(tmp_op)) {
      has_compare = true;
      break;
    }
  }

  /* Record original sequential order of operations in region */

  construct_op_order_map(region, op_order);
  find_first_last_match_ops(match_instance, op_order, match_first, match_last);

  create_predecessors(match_instance, preds);
  create_successors(match_instance, succs);

  /* Now bound both sets (preds/succs) by [match_first, match_last] as
     other ops don't matter */
  for (Region_ops_C0_order opi(region, match_first); opi!=0; opi++) {
    Op *op = *opi;
    if (op == match_last)
      break;
    // can't have patterns that span BRLs.
    if(is_brl(op)) {
//       cdbg << "Spans BRL:\t";
      return false;
    }
    // Can't move a compare above (or below) other compares. This is
    // an ARM problem, because there are no predicate registers, just
    // an MSR.
    if(has_compare && ! match_instance.is_member(op) &&
       (is_cmpp(op) || is_cmpr(op) || is_fcmpp(op) || is_fcmpr(op))) {
//        cdbg << "Move CMP:\t";
      return false;
    }
    span += op;
  }
  preds *= span;
  succs *= span;

#ifdef DEBUG
  cdbg << "Predecessors of instance: ";
  for (Hash_set_iterator<Op*> hsi(preds); hsi!=0; hsi++) {
    cdbg << (*hsi)->id() << ", ";
  }
  cdbg << endl;
  cdbg << "Successors of instance: ";
  for (Hash_set_iterator<Op*> hsi(succs); hsi!=0; hsi++) {
    cdbg << (*hsi)->id() << ", ";
  }
  cdbg << endl;
#endif

  /* Check whether its legal to do the code motion.  Only legal if
     preds and succs sets are disjoint AND there are no branches in
     succs */
  Hash_set<Op*> temp = preds;
  temp *= succs;
  bool has_branch = false;
  for (Hash_set_iterator<Op*> hsi(succs); hsi!=0; hsi++) {
    Op *op = *hsi;
    if (is_branch(op) || is_brl(op)) {
      has_branch = true;
      break;
    }
  }

  if(has_branch || ! temp.is_empty()) {
    if(has_branch) {
        //cdbg << "Brn dep:\t" << endl;;
    } else {
        //cdbg << "Concave:\t" << endl;
    }
    return false;
  }
  return true;
}


/* This routine assumes that liveness analysis is done, so that edges
can be drawn  */
bool reorder_nodes_for_cop(Compound_region* region, Vector<Op*>& match_instance,
			   Op*& insert_point)
{
  Hash_set<Op*> preds(hash_op_ptr,32);
  Hash_set<Op*> succs(hash_op_ptr,32);
  Hash_set<Op*> span(hash_op_ptr,32);
  Hash_map<Op*,int> op_order(hash_op_ptr, 32);
  Op *match_first, *match_last;
  // FIX: make it so that elcor realizes that we can't reorder
  // cmpp/cmpr instructions.
  bool has_compare = false;
  for(unsigned j = 0; j < match_instance.size(); j++) {
    if(is_cmpp(match_instance[j]) || 
       is_cmpr(match_instance[j]) || 
       is_fcmpp(match_instance[j]) || 
       is_fcmpr(match_instance[j]))
      has_compare = true;
  }
  // FIX: This is so innefficient I want to puke. We have to
  // update edges after every pattern replacement in order for
  // the node reordering to generate correct code. We should
  // have some way to just update the edges that need to be
  // updated, instead having to redraw edges for the entire
  // region (this is non-trivial).
  insert_region_scalar_edges(region);

  if( ! is_replaceable(match_instance)) {
    insert_point = NULL;  
    return false;
  } 

#ifdef DEBUG
  cdbg << "\n****************Enter reorder_nodes_for_cop: " << endl;
#endif

  /* Record original sequential order of operations in region */
  construct_op_order_map(region, op_order);
  find_first_last_match_ops(match_instance, op_order, match_first, match_last);

  create_predecessors(match_instance, preds);

  // find the last predecessor, so we know where to insert the cop.
  int max_order = -1;
  insert_point = match_first;
  for(Hash_set_iterator<Op*> hsi(preds); hsi != 0; hsi++) {
    if( ! is_pseudo(*hsi) && op_order.is_bound(*hsi)) {
      if(op_order[*hsi] > max_order) {
	max_order = op_order[*hsi];
	insert_point = *hsi;
      }
    }
  }

  for (Region_ops_C0_order opi(region, insert_point); opi != 0; opi++) {
    Op *op = *opi;
    if (op == match_last)
      break;
    if(is_branch(op)) {
      opi++; // get past C_MERGE
      insert_point = *opi;
    } else if(is_brl(op)) {
      insert_point = *opi;
    }      
  }
  // Amir: if there is a cmp between previous insert point and pattern,
  //       move insert point after cmp.
  for (Region_ops_C0_order opi(region, insert_point); opi != 0; opi++) {
    Op *op = *opi;
    if (op == match_last)
      break;
    if(has_compare && (is_cmpp(op) || is_cmpr(op) ||
		       is_fcmpp(op) || is_fcmpr(op))) {
      opi++;
      insert_point = *opi;
    }
  }
  create_successors(match_instance, succs);

  /* Now bound both sets (preds/succs) by [match_first, match_last] as other ops don't matter */
  for (Region_ops_C0_order opi(region, match_first); opi != 0; opi++) {
    Op *op = *opi;
    if (op == match_last)
      break;
    if(has_compare && (is_cmpp(op) || is_cmpr(op) ||
		       is_fcmpp(op) || is_fcmpr(op))) {
      delete_region_edges(region);
      return false;
    }
    span += op;
  }
  preds *= span;
  succs *= span;

#ifdef DEBUG
  cdbg << "Predecessors of instance: ";
  for (Hash_set_iterator<Op*> hsi(preds); hsi != 0; hsi++) {
    cdbg << (*hsi)->id() << ", ";
  }
  cdbg << endl;
  cdbg << "Successors of instance: ";
  for (Hash_set_iterator<Op*> hsi(succs); hsi != 0; hsi++) {
    cdbg << (*hsi)->id() << ", ";
  }
  cdbg << endl;
#endif


  /* Our objective is to move all the ops in the pattern to the point
     of the last op in the pattern */
  /* When we reach this point, the motion is assumed legal, so earlier checks
     should return false out of this routine when its illegal. */
#ifdef DEBUG
  cdbg << "Performing code motion for matched pattern instance: " << endl;
  cdbg << "\t";
  print_match_instance(match_instance);
#endif

  /* For each branch that we crossed, copy the successor ops from the
    range [start of pattern, branch) to the target of the branch */
  Dlist<Op*> to_move;
  for (Region_ops_C0_order opi(region, match_first); opi != 0; opi++) {
    Op *op = *opi;
    if (op == match_last)
      break;
    if (is_branch(op)) {
#ifdef DEBUG
      cdbg << "Found a branch that was moved below!! " << op->id() << endl;
#endif
      /* Need to place copy of current to_move list at the target of
	 the branch */
      copy_ops_to_compensation_block(region, op, to_move);
    }
    if(is_brl(op)) {
      // technically they could legally cross BRLs, but there's no
      // point, since the BRL is unconditionally taken.
      insert_point = NULL;
      delete_region_edges(region);
      return false;
    }
    if (succs.is_member(op) || match_instance.find(op) != -1) {
      if (is_branch(op) || is_brl(op))
        El_punt("reorder_nodes_for_cop: why is branch (op %d) a in member of succs??", op->id());
#ifdef DEBUG
      cdbg << "Adding op " << op->id() << " to to_move set" << endl;
#endif
      to_move.push_tail(op);
    }
  }

  /* The last thing to do is to update the actual region.  All the
     match_instance ops are inserted at the point of match_last and
     all other successor ops are inserted afterwards in their order
     from the original region */
  for (Dlist_iterator<Op*> dli(to_move); dli != 0; dli++) {
    Op *op = *dli;
    if(match_instance.find(op) != -1) continue;
#ifdef DEBUG
    cdbg << "Moving op downwards: " << op->id() << endl;
#endif
    El_remove_op(op);
    El_insert_op_before(op, match_last);
  }

  delete_region_edges(region);
  return true;
}

/*================================================================================================*/
/* End new reorder function */
/*================================================================================================*/

/*
  Replace matched ops with a new custom op.
  The custom op must appear after the last predecessor and before the first successor of
  the matched ops in the graph.
*/
unsigned replace_ctr = 0;
bool replace(Compound_region* region, Custom_op_descr* cop_descr, 
	     Vector<Op*>& match_instance, const deque<Operand>& real_srcs,
	     const deque<Operand>& real_dests)
{
  
  // Defensive programming...
  Hash_set<Op*> ops_seen(hash_op_ptr, 13);
  for(unsigned q = 0; q < match_instance.size(); q++) {
    Op* op = match_instance[q];
    if(ops_seen.is_member(op)) {
      cdbg << "ERROR: the match you're asking me to replace has duplicate\n"
	   << "ops in it! This will cause all kinds of things to break, and\n"
	   << "you're probably doing something wrong, so I'm going to quit.\n" << endl;
      cdbg << "Block: " << region->id() << "\tMatch ops: ";
      for(unsigned i = 0; i < match_instance.size(); i++) {
	cdbg << match_instance[i]->id() << " ";
      }
      cdbg << endl << endl;
      assert(0);
    }
    ops_seen += op;
  }

  const int n = match_instance.dim();
  Op *last_pred;

  extern unsigned SOME_MAX;

  if(replace_ctr++ >= SOME_MAX)
    return false;

#if 0
  // FIX: this should use a dbg variable instead of a #if.

  // print debug information
  cdbg << "Trying to replace match: ";
  for(int j = 0; j < n; j++) {
    Op* op = match_instance[j];
    cdbg << op->id() << ", ";
  }
  if(cop_descr)
    cdbg << "\twith " << cop_descr->get_opcode();
  else
    cdbg << "\twith a default custom op";
  cdbg << endl;

#endif

  bool everythings_fine = reorder_nodes_for_cop(region, match_instance, last_pred);

  if(! everythings_fine) {
    return false;
  }

  Hash_map<Op*, int> op_order(hash_op_ptr, 77);
  construct_op_order_map(region, op_order);

  Op* cop = NULL;
  if(cop_descr) {
    cop = create_custom_op(cop_descr, match_instance, real_srcs, real_dests);
  } else {
    assert(0);
  }

//   cdbg << "  Replaced with op " << cop->id() << endl << endl;

  if(everythings_fine) {
    El_insert_op_after(cop, last_pred);
  }

  // reuse the hash_set out of laziness.
  for(int q = 0; q < n; q++) {
    Op* op = match_instance[q];
    El_remove_op(op);
    delete op;
  }

  //  ir_write(region, "%d_after_reorder.reb", match_instance[0]->id());

  // Do statitics
  if(cop_descr) {
    if(cop_dynamic_count.is_bound(cop_descr)) {
      cop_dynamic_count.value(cop_descr) += region->weight;
    } else {
      cop_dynamic_count.bind(cop_descr, region->weight);
    }
  }

  return true;
}


// Do pattern match within a hb/bb
void pattern_match_bb_hb(Compound_region* reg)
{
  unsigned cop_size = current_MDES->get_num_custom_ops();
  Vector<List<Vector<Op*> > > matched_ops(cop_size);
  Hash_set<Op*> replaced_ops(hash_op_ptr, 128);
  replaced_ops.clear();
  List<Pattern_match_node*> to_delete;

  // build the graph for the region
  DirectedGraph<Pattern_match_node> *reg_graph = NULL, *pattern_graph = NULL;

  Hash_map<Op*, int> op_node_map(hash_op_ptr, 128);
  Hash_map<int, Op*> node_op_map(hash_int, 128);

  if(El_mapping_algorithm == GREEDY_MAPPING ||
     El_mapping_algorithm == BRANCH_AND_BOUND_MAPPING) {

    insert_region_scalar_edges(reg);
    reg_graph = build_region_graph(reg, node_op_map, to_delete);

    // match all the custom ops in the bb/hb
    int i = 0;
    for(List_iterator<DirectedGraph<Pattern_match_node>*> gi(cop_patterns); gi != 0;
	gi++, i++) {
      bb_pattern_count = 0;
      pattern_graph = *gi;

      deque<Vector<unsigned> > matches;
      if(reg_graph->num_nodes() > 0) {
	cdbg << "Finding matches for cop pattern " << i << endl;
	cdbg << "Reg id: " << reg->id() << "\tReg_graph size " << reg_graph->num_nodes()
	     << "\tsubg. size: " << pattern_graph->num_nodes() << endl;
      }
      reg_graph->find_isomorphic_subgraphs(*pattern_graph, new_node_compat, matches);
      if(reg_graph->num_nodes() > 0) {
	cdbg << "Found " << matches.size() << endl << endl;
      }

      // need to translate the node ids in the matches to Op*'s 
      for(deque<Vector<unsigned> >::iterator li = matches.begin();
	  li != matches.end(); li++) {
	Vector<unsigned>& v = *li;
	Vector<Op*> ops(v.size());
	for(unsigned j = 0; j < v.size(); j++) {
	  ops[j] = node_op_map[v[j]];
	}
	if(is_replaceable(ops)) {
	  matched_ops[i].add_tail(ops);
	}
      }

    }

    delete_region_edges(reg);
  }

  if(El_mapping_algorithm == NO_MAPPING) {
    return;
  } else if(El_mapping_algorithm == GREEDY_MAPPING) {
    // give each op to the cop with the best esimated cycle savings,
    // just like leproc. this helps avoid conflicts between leproc's
    // selection heuristic and the scheduler.
    Vector<int> cop_priority(cop_size);
    for(unsigned k = 0; k < cop_size; k++)
      cop_priority[k] = k;
    // bubble sort
    for(unsigned k = 0; k < cop_size; k++) {
      for(unsigned m = 0; m < cop_size - 1; m++) {
	Custom_op_descr* cop1 = current_MDES->get_MDES_custom_op_descr(m);
	Custom_op_descr* cop2 = current_MDES->get_MDES_custom_op_descr(m + 1);

	// FIX: this prioritization is wrong now that the patterns
	// include input and output nodes.
	int c1 = cop1->get_pattern_size() - 
	  MDES_flow_time(cop1->get_opcode(), DATA_OUT, DEST1);
	int c2 = cop2->get_pattern_size() - 
	  MDES_flow_time(cop2->get_opcode(), DATA_OUT, DEST1);
	if(c1 < c2) {
	  int tmp = cop_priority[m];
	  cop_priority[m] = cop_priority[m + 1];
	  cop_priority[m + 1] = tmp;
	}
      }
    }

    // Pick which patterns to replace
    deque<Vector<Op*> > real_match_deque;
    deque<deque<Operand> > real_srcs_deque, real_dests_deque;
    deque<Custom_op_descr*> cop_deque;
    for(unsigned j = 0; j < cop_size; j++) {
      int cur_cop_num = cop_priority[j];
      const deque<bool>& node_valid = pattern_node_valid[cur_cop_num];
      const deque<int>& src_ports = pattern_src_num[cur_cop_num];
      const deque<int>& dest_ports = pattern_dest_num[cur_cop_num];
      Custom_op_descr* cop = current_MDES->get_MDES_custom_op_descr(cur_cop_num);
      for(List_iterator<Vector<Op*> > mi(matched_ops[cur_cop_num]); mi != 0; mi++) {
	Vector<Op*> match_instance = *mi;

	// FIX: most of this should be moved back into the replace() function.

	deque<Operand> real_srcs;
	deque<Operand> real_dests;

	// We only replace the matched nodes that correspond to
	// actual operations, not the input/output nodes.
	Hash_set<Op*> ops_to_combine(hash_op_ptr, 17);
	for(unsigned m = 0; m < match_instance.size(); m++) {
	  if(match_instance[m] && node_valid[m]) {
	    ops_to_combine += match_instance[m];
	    if(src_ports[m] != NO_OPERAND) {
	      real_srcs.push_back( match_instance[m]->src(src_ports[m]));
	    }
	    if(dest_ports[m] != NO_OPERAND) {
	      real_dests.push_back(match_instance[m]->dest(dest_ports[m]));
	    }
	  }
	}

	unsigned num_nodes = 0;
	Vector<Op*> real_match(ops_to_combine.size());
	for(Hash_set_iterator<Op*> hsi(ops_to_combine); hsi != 0; hsi++) {
	  real_match[num_nodes++] = *hsi;
	}

	// make sure all the important ops haven't been replaced;
	bool valid = true;
	for(unsigned k = 0; k < real_match.size(); k++) {
	  if(replaced_ops.is_member(real_match[k]))	
	    valid = false;
	}

	if(valid) {
	  cop_match_count[cur_cop_num]++;
	  Op* op;
	  for(unsigned k = 0; k < real_match.size(); k++) {
	    op = real_match[k];
	    replaced_ops += op;
	    cop_dmatch_count[cur_cop_num] += El_calc_op_weight(op);
	  }

	  real_match_deque.push_back(real_match);
	  real_srcs_deque.push_back(real_srcs);
	  real_dests_deque.push_back(real_dests);
	  cop_deque.push_back(cop);

	}
      }
    }

    // Replace the selected matches. This is done in a separate loop
    // from the selection because many Ops appear in multiple
    // pattern matches. If we replace (and delete) an Op before we
    // look at all the matches, it's very easy to write code that
    // segfaults. Replacing in a separate loop makes this harder.
    deque<deque<Operand> >::iterator the_srcs = real_srcs_deque.begin();
    deque<deque<Operand> >::iterator the_dests = real_dests_deque.begin();
    deque<Vector<Op*> >::iterator the_match = real_match_deque.begin();
    deque<Custom_op_descr*>::iterator the_cop = cop_deque.begin();
    for(; the_match != real_match_deque.end(); the_match++, the_srcs++,
	  the_dests++, the_cop++) {
      replace(reg, *the_cop, *the_match, *the_srcs, *the_dests);
    }

  } else if (El_mapping_algorithm == BRANCH_AND_BOUND_MAPPING) {
    Hash_set<Op*> ops_in_matches(hash_op_ptr, 211);
    for(unsigned k = 0; k < matched_ops.size(); k++) {
      for(List_iterator<Vector<Op*> > li(matched_ops[k]); li != 0; li++) {
	Vector<Op*>& the_match = (*li);
	for(unsigned m = 0; m < the_match.size(); m++) {
	  ops_in_matches += the_match[m];
	}
      }
    }

    bnb_real_work(matched_ops, ops_in_matches, reg);
  } else {
    cdbg << "ERROR: I don't know what type of mapping algorithm to use for\n"
	 << "custom ops!" << endl;
    El_punt("");
  }

  if(El_mapping_algorithm == GREEDY_MAPPING ||
     El_mapping_algorithm == BRANCH_AND_BOUND_MAPPING) {
    for(List_iterator<Pattern_match_node*> li(to_delete); li != 0; li++) {
      Pattern_match_node* n = *li;
      delete n;
    }
    delete reg_graph;
  }
}


void pattern_match(Compound_region* reg) {
  if(reg->is_hb() || reg->is_bb()) {
    if( ! bbs_created.is_member(reg))
      pattern_match_bb_hb(reg);
  } else {
    for(Region_subregions subri(reg); subri != 0; subri++) {
      Compound_region *subr = (Compound_region*) *subri;
      pattern_match(subr);
    }
  }
}

void print_cop_stats(Compound_region* reg) {
  ofstream* COP_STAT_STREAM = new ofstream("COP_STAT", ios::app);
  COP_STAT_STREAM->setf(ios::fixed);
  COP_STAT_STREAM->precision(2);

  *COP_STAT_STREAM << "procedure " << ((Procedure*)reg)->get_name() << ":" << endl;
  int cop_size = current_MDES->get_num_custom_ops();
  for (int i =0; i<cop_size;i++) {
    Custom_op_descr* cop = current_MDES->get_MDES_custom_op_descr(i);
    *COP_STAT_STREAM << cop->get_opcode() << ":";
    if(cop_dynamic_count.is_bound(cop)) {
      *COP_STAT_STREAM << cop_dynamic_count.value(cop) << endl;
    } else {
      *COP_STAT_STREAM << 0 << endl;
    }

  }

  *COP_STAT_STREAM << "end procedure" << endl;

  delete COP_STAT_STREAM;
}


void pattern_match_procedure(Compound_region* reg) 
{
  // Read all the custom ops from MDES
  if( ! cop_patterns_initialized) {
    unsigned cop_size = current_MDES->get_num_custom_ops();
    cop_match_count.resize(cop_size);
    cop_dmatch_count.resize(cop_size);
    for (unsigned i = 0; i < cop_size; i++) {
      Custom_op_descr* cop_descr = current_MDES->get_MDES_custom_op_descr(i);
      deque<bool> valid_nodes;
      deque<int>  dest_nums, src_nums;
      DirectedGraph<Pattern_match_node>* pattern_graph =
	build_pattern_graph(cop_descr, valid_nodes, src_nums, dest_nums);
      pattern_node_valid.push_back(valid_nodes);
      pattern_src_num.push_back(src_nums);
      pattern_dest_num.push_back(dest_nums);
      cop_patterns.add_tail(pattern_graph);
      cop_match_count[i] = 0;
      cop_dmatch_count[i] = 0;
    }
    cop_patterns_initialized = true;
  }
  
  ugly_hack_ops.clear();

  // Edge drawing needs to know when macros alias, and
  // unfortunately it doesn't check this until after scheduling.
  make_macros_canonical(reg);

  bbs_created.clear();

  create_local_analysis_info_for_all_hbs_bbs(reg);

  // do pattern match in the procedure
  pattern_match(reg);

  delete_local_analysis_info_for_all_hbs_bbs(reg);

  // ir_write(reg, "%s_after_convert.reb", (char*)((Procedure*)reg)->get_name());

  // print statistics for the procedure
  if(El_print_cop_stats)
    print_cop_stats(reg);

}

