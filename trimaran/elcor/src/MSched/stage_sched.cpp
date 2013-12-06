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
//      File:           stage_sched.cpp
//      Authors:        Alex Eichenberger, Vinod Kathail
//      Created:        June 1995
//      Description:    Stage scheduling of operations in a swp loop 
//                      to reduce register lifetimes
//
//
/////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <string.h>
 
#include "operand.h"
#include "filter.h"
#include "graph_filters.h"
#include "iterators.h"
#include "list.h"
#include "opcode_properties.h"
#include "matrix.h"
#include "region.h"
#include "stage_sched.h"
#include "mymath.h"
#include "dbg.h"
#include "el_error.h"

using namespace std;


//////////////////////////////////////////////////////////////////////
// dbg degrees
//  0: nothing 
//  1: errors + heuristic called
//  2: log of transformations done by heuristics
//  3: log of computation of ordering (compute_up_order)
//  4: trace of computation of biconnected algo
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// filters for stage scheduling
//////////////////////////////////////////////////////////////////////

bool Edge_filter_for_stage_scheduling::filter(Edge * const & edge) const 
{ 
  return !(
    // discard edges to pseudo ops 
    edge->src()->is_pseudo_op() || edge->dest()->is_pseudo_op() ||
    // discard C0 edges
    // discard all control edges for the time being - Shail + Vinod 09/04/96
    edge->is_control() ||
    // discard self loop edges (put no constraints on stage scheduling)
    (edge->src() == edge->dest())
    );
}

bool SS_edge_not_in_ucycle_filter::filter(SS_edge * const & ss_edge) const 
{ 
  return (ss_edge->in_underlying_cycle);
}

Edge_filter_for_reg_edges::Edge_filter_for_reg_edges()
  : reg_file_on(NUM_REG_FILE, true) {}

Edge_filter_for_reg_edges::Edge_filter_for_reg_edges(Vector<bool>& reg_file_considered)
  : reg_file_on(reg_file_considered) {}

Edge_filter_for_reg_edges::~Edge_filter_for_reg_edges()
{
  reg_file_on.clear();
}

bool Edge_filter_for_reg_edges::filter(Edge * const & edge) const
{
  if (!edge->is_reg_dep()) return false;

  // fetch operand assoc with reg edge
  Op* dest_op = edge->dest();
  Port_num dest_num = edge->dest_port();
  Operand& dest_operand = (edge->is_reg_flow()) ? 
     dest_op->src(dest_num) : dest_op->dest(dest_num);
  assert(dest_operand.is_reg());
  Reg_file reg_file = dest_operand.file_type();

  return (reg_file_on[reg_file] && dest_operand.is_rotating());
}

//////////////////////////////////////////////////////////////////////
// SS_edge
//////////////////////////////////////////////////////////////////////

ostream& operator<< (ostream& os, SS_edge& e)
{
  os << "ss_edge id(" << e.id << ")";
  if (e.violate_latency) os << " ERROR";
  os << " [" << e.src->id << "->" << e.dest->id << "]";
  os << " '" << *e.ir_edge << "'";
  os << " lat(" << e.latency << ")";
  os << " omega(" << e.omega << ")";
  os << " p(" << e.p << ")";
  os << " p_initial(" << e.p_initial << ")";
  os << " s(" << e.s << ")";
  os << " r(" << e.r << ")";
  os << " in_ucycle(" << e.in_underlying_cycle << ")";
  os << endl;
  return os;
}

SS_edge::SS_edge()
{
  init();
}

SS_edge::SS_edge(Edge* e, int o, SS_vertex* v_src, SS_vertex* v_dest, int II,
  Edge_filter* reg_edge)
{
  init();
  ir_edge = e;
  latency = ir_edge->latency();
  omega = o;

  int time_from = v_src->initial_sched_time;
  int time_to = v_dest->initial_sched_time;

  r = mod(time_to - time_from, II);
  s = ceil(latency - r, II) - omega;
  p_initial = p = floor(time_to - time_from - latency, II) + omega;
  reg_attribute = reg_edge->filter((Edge * const &) e);

  stacked = false;

  src = v_src;
  dest = v_dest;
}

SS_edge::~SS_edge() {}

static int ss_edge_id_num = 0;

void SS_edge::init()
{
  ir_edge = 0;
  latency = 0;
  omega = 0;
  id = ss_edge_id_num++;
  p_initial = p = s = r = 0;
  in_underlying_cycle = reg_attribute = sched_attribute = true;
  violate_latency = false;
  src = 0;
  dest = 0;
}

//////////////////////////////////////////////////////////////////////
// SS_vertex
//////////////////////////////////////////////////////////////////////

ostream& operator<< (ostream& os, SS_vertex& v)
{
  os << "ss_vertex id(" << v.id << ")";
  os << " op-id(" << v.ir_op->id() << ")";
  os << " prev_st(" << v.initial_sched_time << ")";
  if (v.sched_time != infinity) os << " st(" << v.sched_time << ")";
  os << " in_edges(";
  for(List_iterator<SS_edge*> i1(v.in_edges); i1 != 0; i1++) {
    SS_edge* curr_ss_edge = *i1;
    os << " " << curr_ss_edge->id;
  }
  os << ")";
  os << " out_edges(";
  for(List_iterator<SS_edge*> i2(v.out_edges); i2 != 0; i2++) {
    SS_edge* curr_ss_edge = *i2;
    os << " " << curr_ss_edge->id;
  }
  os << ")";  
  os << endl;
  return os;
}

SS_vertex::SS_vertex()
{ 
  init();
}

SS_vertex::SS_vertex(Op* op, int II)
{
  init();
  ir_op = op;
  initial_sched_time = sched_time = ir_op->sched_time();
  r = mod(sched_time, II);
}

SS_vertex::~SS_vertex() 
{
  in_edges.clear();
  out_edges.clear();
}

static int ss_vertex_id_num = 0;

void SS_vertex::init()
{
  ir_op = 0;
  in_edges.clear();
  out_edges.clear();
  id = ss_vertex_id_num++;

  visited = false;
  df_num = low = -infinity;
  father = 0;

  color = SS_no_pred;
  sink_vertex = source_vertex = false;
  reg_sink_vertex = reg_source_vertex = false;

  r = initial_sched_time = sched_time = -infinity;
}

//////////////////////////////////////////////////////////////////////
// SS_stage_sched external methods
//////////////////////////////////////////////////////////////////////

ostream& operator<< (ostream& os, SS_stage_sched& ss)
{
  os << "Stage scheduling main datastructure:" << endl;
  os << "  ss_edges: " << endl;
  for(List_iterator<SS_edge*> i1(ss.edge_list); i1!=0; i1++) {
    SS_edge* curr_ss_edge = *i1;
    os << "  " << *curr_ss_edge;
  }
  os << "  ss_vertices: " << endl;
  for(List_iterator<SS_vertex*> i2(ss.vertex_list); i2!=0; i2++) {
    SS_vertex* curr_v = *i2;
    os << "  " << *curr_v;
  } 
  os << endl;
  return os;
}

SS_stage_sched::SS_stage_sched() 
{
  init();
}

SS_stage_sched::SS_stage_sched(LoopBody* loop, int ii, 
  Edge_filter* reg_edge_filter) 
{
  init();
  loopbody = loop;
  II = ii;
  // if reg_edge_filter is unspecified, use the default one
  if (reg_edge_filter == 0) {
    register_edge_filter = new Edge_filter_for_reg_edges();
  } else {
    register_edge_filter = reg_edge_filter;
  }
  if (!read_ir()) {
    El_warn("SS: cannot build a stage schedule");
    return;
  }
  find_biconnected_edges();
  initalized_graph = valid_stage_sched = true;
}

SS_stage_sched::~SS_stage_sched() 
{
  reset();
}

bool SS_stage_sched::transform_default()
{
  SS_transform_ac ac;
  SS_transform_ss ss;
  SS_transform_up up(1);

  return (transform(ac) && transform(ss) && transform(up));
}

bool SS_stage_sched::transform(SS_transform& transform)
{
  if (!initalized_graph) return false;
  valid_stage_sched = false;

  // transform
  transform.transform(this);

  // compute sched time
  if (!compute_sched_time()) {
    El_warn("SS: invalid stage schedule (1)");
    vcg_graph("graph_error");
    return false;
  }

  // verify sched time
  if (!verify_sched_time()) {
    El_warn("SS: invalid stage schedule (2)");
    vcg_graph("graph_error");
    return false;
  }
  
  commit();
  return true;    
}

/*
   graph: view using xvcg on fast3
   color: green -> edges that violate its latency
          red -> edges with decreased schedule time along it
          blue -> edges with increased schedule time along it
   vertex info: "op:xx(yy)" "opcode" "st(aa->bb)"
          xx: internal number
          yy: op->id()
          aa: schedule time before stage scheduling
          bb: schedule time after stage scheduling
   edge info "xyy" "o(zz)" "p(aa->bb)" "l(cc)"
          x:  "u" if in underlying cycle, "e" otherwise
              "*" if latency is violated
          yy: internal number of the edge
          zz: dependence distance
          aa: initial p-factor before stage scheduling
          bb: initial p-factor after stage scheduling
          cc: latency of the operation

   if fussy is false, edges from brtop are not printed
*/

void SS_stage_sched::vcg_graph(char* name, bool fussy)
{
  assert(initalized_graph);
  static int count = 0;
  char real_name[100];

  sprintf(real_name, "%s.%i.vcg", name, count++);
  ofstream fout(real_name);
  if (!fout) {
    El_warn("cannot open file in vcg_graph: %s", real_name);
    return;
  }
  cerr << "SS: call 'xvcg " << real_name << "' to see stage scheduling graph";
  if (!fussy) cerr << " (edge from brtop ignored)";
  cerr << endl;

  fout << "graph: {" << '\n' ;

  for(List_iterator<SS_vertex*> i2(vertex_list); i2!=0; i2++) {
    SS_vertex* curr_v = *i2;
    fout << "node: { title: \"" << curr_v->id << "\"";
    fout << " label: \"Op:" << curr_v->id << "(" << curr_v->ir_op->id() << ")";
    fout << "\\n" << curr_v->ir_op->sched_opcode();
    fout << "\\nst(" << curr_v->initial_sched_time;
    if (curr_v->sched_time != -infinity && 
        curr_v->initial_sched_time != curr_v->sched_time) {
          fout << "->" << curr_v->sched_time;
    }
    fout << ")";
    fout << "\"";
    fout << " }" << endl;
  }
  for(List_iterator<SS_edge*> i1(edge_list); i1!=0; i1++) {
    SS_edge* curr_ss_edge = *i1;

    // if not fussy, skip non register edges from brtop 
    if (!fussy && is_swp_branch(curr_ss_edge->src->ir_op)) continue;

    fout << "edge: { sourcename: \"" << curr_ss_edge->src->id << "\"";
    fout << " targetname: \"" << curr_ss_edge->dest->id << "\"";
    fout << " label: \"";
    if (curr_ss_edge->in_underlying_cycle) {
      fout << "u";
    } else {
      fout << "e";
    }
    if (curr_ss_edge->violate_latency) fout << "*";

    fout << curr_ss_edge->id;
    fout << "\\no(" << curr_ss_edge->omega << ")";
    fout << "\\np(" << curr_ss_edge->p_initial;
    if (curr_ss_edge->p_initial != curr_ss_edge->p) {
      fout << "->" << curr_ss_edge->p;
    }
    fout << ")";
    fout << "\\nl(" << curr_ss_edge->latency << ")";
    Port_num dest_num = curr_ss_edge->ir_edge->dest_port();
    Operand& dest_operand = curr_ss_edge->dest->ir_op->src(dest_num);
    fout << "\\nvr" << dest_operand.vr_num();
    fout << "\"";
    if (curr_ss_edge->violate_latency) {
      fout << " color: green";
    } else if (curr_ss_edge->p_initial > curr_ss_edge->p) {
      fout << " color: red";
    } else if (curr_ss_edge->p_initial < curr_ss_edge->p) {
      fout << " color: blue";
    }
    if (!curr_ss_edge->reg_attribute) {
      fout << " linestyle: dashed";
    }
    fout << " }" << endl;
  }
  fout << "}" << endl ;
}

//////////////////////////////////////////////////////////////////////
// SS_stage_sched internal methods
//////////////////////////////////////////////////////////////////////

void SS_stage_sched::init()
{
  II = 0;
  loopbody = 0;
  edge_list.clear();
  vertex_list.clear();
  vertex_map = 0;
  brtop_v = 0;
  pbr_v = 0;
  stack.clear();
  up_order.clear();

  // define state variables to false
  initalized_graph = valid_stage_sched = false;
  up_order_defined = sink_source_defined = reg_sink_source_defined = false;

  edge_filter = 0;
  ss_edge_in_ucycles = 0;

  // init id variables
  ss_edge_id_num = ss_vertex_id_num = 0;
}

void SS_stage_sched::reset()
{
  // delete all data edges, ptr included
  for(List_iterator<SS_edge*> i1(edge_list); i1!=0; i1++) {
    SS_edge* curr_ss_edge = *i1;
    delete curr_ss_edge;
  }
  edge_list.clear();
  // delete all data vertices, ptr included
  for(List_iterator<SS_vertex*> i2(vertex_list); i2!=0; i2++) {
    SS_vertex* curr_v = *i2;
    delete curr_v;
  }
  vertex_list.clear();
  if (vertex_map != 0) vertex_map->clear();
  stack.clear();
  up_order.clear();
  delete edge_filter;
  delete register_edge_filter;
  delete ss_edge_in_ucycles;
}


// assumption: 1 brtop, no more than one pbr operation, and
// no more than one edge from bpr to brtop
// return true in case of success
bool SS_stage_sched::read_ir()
{
  if (initalized_graph) return true;

  edge_filter = new Edge_filter_for_stage_scheduling;
  ss_edge_in_ucycles = new SS_edge_not_in_ucycle_filter;
  vertex_map = new Map<Op*, SS_vertex*>;

  for(Region_all_internal_edges i1(loopbody, edge_filter); i1 != 0; i1++) {
    Edge* curr_edge = *i1;

    // danger: use of scrach int to get omega
    int omega = curr_edge->scratch_int;

    // construct or fetch dest vertex
    Op* dest_op = curr_edge->dest();
    SS_vertex* dest_v;
    if (!vertex_map->is_bound(dest_op)) {
      dest_v = new SS_vertex(dest_op, II);
      vertex_map->bind(dest_op, dest_v);
      vertex_list.add_tail(dest_v);
      if (is_swp_branch(dest_op)) {
        if (brtop_v != 0) {
          El_warn("SS: found two brtop in loop kernel");
          return false;
	}
        brtop_v = dest_v;
      } else if (is_pbr(dest_op)) {
        if (pbr_v != 0) {
          El_warn("SS: found two pbr in loop kernel");
          return false;
	}
        pbr_v = dest_v;
      }
    } else {
      dest_v = vertex_map->value(dest_op);
    }

    // construct or fetch src vertex
    Op* src_op = curr_edge->src();
    SS_vertex* src_v;
    if (!vertex_map->is_bound(src_op)) {
      src_v = new SS_vertex(src_op, II);
      vertex_map->bind(src_op, src_v);
      vertex_list.add_tail(src_v);
      if (is_swp_branch(src_op)) {
        if (brtop_v != 0) {
          El_warn("SS: found two brtop in loop kernel");
          return false;
	}
        brtop_v = src_v;
      } else if (is_pbr(src_op)) {
        if (pbr_v != 0) {
          El_warn("SS: found two pbr in loop kernel");
          return false;
	}
        pbr_v = src_v;
      }
    } else {
      src_v = vertex_map->value(src_op);
    }
    
    // discard edges coming out of the loop branch, since they are
    // internal. The modulo-scheduler adds direct loopback edges
    // between branch and operations that would otherwise appear to be
    // internal. -- Shail Aditya 11/09/98
    if (src_v == brtop_v) continue;

    // construct new edge
    SS_edge* curr_ss_edge = new SS_edge(curr_edge, omega, src_v, dest_v, II, register_edge_filter);
    assert(!edge_list.is_member(curr_ss_edge));
    edge_list.add_tail(curr_ss_edge);
 
    // connect edge
    src_v->out_edges.add_tail(curr_ss_edge);
    dest_v->in_edges.add_tail(curr_ss_edge);    
  }

  // we must have one brtop
  if (brtop_v == 0) {
    El_warn("SS: missing brtop in loop kernel");
    return false;
  }
  initalized_graph = true;

  // deallocate the map
  delete vertex_map;
  vertex_map = 0;

  return true;
}

void SS_stage_sched::commit()
{
  assert(valid_stage_sched);

  for(List_iterator<SS_vertex*> i1(vertex_list); i1!=0; i1++) {
    SS_vertex* curr_v = *i1;
    Op* curr_op = curr_v->ir_op;

    curr_op->set_sched_time(curr_v->sched_time);
  }
}

//////////////////////////////////////////////////////////////////////
// find biconnected edges
// algo: "The design and analysis of computer algorithms"
//        A.V. Aho, J.E. Hopcroft J.D. Ullman
//        1974, chap 5, pp 179-187
//////////////////////////////////////////////////////////////////////

void SS_stage_sched::handle_pair(SS_vertex* v, SS_vertex* w, SS_edge* e) 
{
  if (dbg(ms,4)) {
    cdbg << "handle pair (" << v->id << " " << w->id << ")"
	 << " ss-edge-id(" << e->id << ")" << endl;
  }

  // put edge on stack, if edge is not already on the stack
  // Warning: cannot use the book algo, to determine if an edge was already in the stack
  // because the graph may not be simple, i.e. they may be more than one edges between a
  // pair of vertices. I think the present method, which mark each edge by a 'stacked'
  // field works, but its not a formal proof!!!
  // the book test:
  //
  //   if (!(v->df_num < w->df_num && w->visited) && !(v->df_num > w->df_num && w == v->father)) 

  if (!e->stacked) {
    // we have a new edge, put it on the stack
    e->stacked = true;
    stack.push(e);
  }

  if (!w->visited) {
    w->father = v;
    searchb(w);
    if (w->low >= v->df_num) {
      // biconnected component has been found
      // pop all edges on the stack upto edge (v, w), included
      int i = 0;
      SS_edge* curr_ss_edge = 0;
      if (dbg(ms, 4)) {
	cdbg << endl << "Edges in a biconnected components" << endl;
      }
      for(i=0; curr_ss_edge != e; i++) {
        assert(!stack.is_empty());
        curr_ss_edge = stack.pop();
        curr_ss_edge->in_underlying_cycle = true;
	if (dbg(ms, 4)) cdbg << "  " << *curr_ss_edge;
      } 
      if (i == 1) {
        // had an edge that was by itself in a biconnected component
        // which is thus a edge not in any underlying cycle
        e->in_underlying_cycle = false;
        if (dbg(ms, 4)) {
	  cdbg << "  PREV EDGE IS NOT IN UNDERLYING CYCLES" << endl;
	}
      }
    }
    v->low = ELCOR_MIN(v->low, w->low);
  } else if (w != v->father) {
    v->low = ELCOR_MIN(v->low, w->df_num);
  }
}

void SS_stage_sched::searchb(SS_vertex* v)
{ 
  if (dbg(ms, 4)) cdbg << "visit " << v->id << endl;
  v->visited = true;
  v->df_num = count++;
  v->low = v->df_num;

  for(List_iterator<SS_edge*> i1(v->in_edges); i1 != 0; i1++) {
    SS_edge* curr_ss_edge = *i1;
    SS_vertex* w = curr_ss_edge->src;
    handle_pair(v, w, curr_ss_edge);
  }
  for(List_iterator<SS_edge*> i2(v->out_edges); i2 != 0; i2++) {
    SS_edge* curr_ss_edge = *i2;
    SS_vertex* w = curr_ss_edge->dest;
    handle_pair(v, w, curr_ss_edge);
  }
}

void SS_stage_sched::find_biconnected_edges()
{
  assert(initalized_graph);

  stack.clear();
  count = 0;

  // init
  for(List_iterator<SS_edge*> i0(edge_list); i0!=0; i0++) {
    SS_edge* curr_ss_edge = *i0;
    curr_ss_edge->stacked = false;
  }
  for(List_iterator<SS_vertex*> i1(vertex_list); i1!=0; i1++) {
    SS_vertex* curr_v = *i1;
    curr_v = NULL;
  }

  // detect
  for(List_iterator<SS_vertex*> i2(vertex_list); i2!=0; i2++) {
    SS_vertex* curr_v = *i2;
    if (!curr_v->visited) {
      searchb(curr_v);
    }
  }
  if (dbg(ms, 4)) cdbg << *this << endl;
}

//////////////////////////////////////////////////////////////////////
// assist transforms
//////////////////////////////////////////////////////////////////////

void SS_stage_sched::construct_sink_source()
{
  assert(initalized_graph);
  if (sink_source_defined) return;

  for(List_iterator<SS_vertex*> i1(vertex_list); i1!=0; i1++) {
    SS_vertex* curr_v = *i1;

    // if there are no incomming edges -> it is a source vertex
    // edges outside underlying cycles are ignored
    curr_v->source_vertex = true;
    for(List_filterator<SS_edge*> i2(curr_v->in_edges, ss_edge_in_ucycles); 
        i2 != 0; i2++) {
      curr_v->source_vertex = false;
      break;
    }

    // if there are no outgoing edges -> it is a sink vertex
    // edges outside underlying cycles are ignored
    curr_v->sink_vertex = true;
    for(List_filterator<SS_edge*> i3(curr_v->out_edges, ss_edge_in_ucycles); 
	i3 != 0; i3++) {
      curr_v->sink_vertex = false;
      break;
    }
  }
  sink_source_defined = true;
}

void SS_stage_sched::construct_reg_sink_source()
{
  if (reg_sink_source_defined) return;
  assert(initalized_graph);

  for(List_iterator<SS_vertex*> i1(vertex_list); i1!=0; i1++) {
    SS_vertex* curr_v = *i1;

    // if there are no incomming register edges -> it is a reg-source vertex
    // edges outside underlying cycles are ignored
    curr_v->reg_source_vertex = true;
    for(List_filterator<SS_edge*> i2(curr_v->in_edges, ss_edge_in_ucycles); 
        i2 != 0; i2++) {
      SS_edge* curr_ss_edge = *i2;
      if (curr_ss_edge->reg_attribute) {
        curr_v->reg_source_vertex = false;
        break;
      }
    }

    // if there are no outgoing register edges -> it is a reg-sink vertex
    // edges outside underlying cycles are ignored
    curr_v->reg_sink_vertex = true;
    for(List_filterator<SS_edge*> i3(curr_v->out_edges, ss_edge_in_ucycles); 
      i3 != 0; i3++) {
      SS_edge* curr_ss_edge = *i3;
      if (curr_ss_edge->reg_attribute) {
        curr_v->reg_sink_vertex = false;
        break;
      }
    }
  }
  reg_sink_source_defined = true;
}

void SS_stage_sched::construct_up_order()
{
  if (up_order_defined) return;
  assert(initalized_graph);

  // init
  List<SS_vertex*> queue;
  up_order.clear();
  construct_sink_source();

  for(List_iterator<SS_vertex*> i1(vertex_list); i1!=0; i1++) {
    SS_vertex* curr_v = *i1;
    if (curr_v->sink_vertex) {
      // sink vertiex have no predecessor ->
      // we consider that all its predecessors have been inspected
      curr_v->color = SS_all_pred;
      queue.add_tail(curr_v);
    } else {
      // we consider that none of its predecessors have been inspected
      curr_v->color = SS_no_pred;
    }
  }
 
  do {
    while (!queue.is_empty()) {
      SS_vertex* curr_v = queue.pop();
      assert(curr_v->color == SS_all_pred);
      if (curr_v->sink_vertex && curr_v->source_vertex) {
        // not interested in doing anything a vertex all alone by itself
        continue;
      }
      // process that vertex and put on up_order list
      if (dbg(ms, 3)) cdbg << "processed vertex " << *curr_v;
      up_order.add_tail(curr_v);

      // find predecessor from curr_v that have only SS_all_pred successor
      for(List_iterator<SS_edge*> i2(curr_v->in_edges); i2!=0; i2++) {
        SS_edge* prev_e = *i2;
        SS_vertex* prev_v = prev_e->src;

        // update status of prev_v
        if (prev_v->color == SS_all_pred) {
          // nothing to do
          continue;
	}
        prev_v->color = SS_some_pred;

        // should it be SS_all_pred? 
        // yes, if all prev_v successor are themselves SS_all_pred
        bool all_pred = true;
        for(List_iterator<SS_edge*> i3(prev_v->out_edges); i3!=0; i3++) {
          SS_edge* prev_next_e = *i3;
          SS_vertex* prev_next_v = prev_next_e->dest;
         
          if (prev_next_v->color != SS_all_pred) {
            all_pred = false;
            break;
	  }
	}
        if (all_pred) {
          prev_v->color = SS_all_pred;
          queue.add_tail(prev_v);
	}
      }
    }

    // we know the queue empty, but it does not mean that we succeeded
    // in handling all vertices, because of cycles in the dependence
    // graph. 
    SS_vertex* victim_v = 0;
    for(List_iterator<SS_vertex*> i4(vertex_list); i4!=0; i4++) {
      SS_vertex* curr_v = *i4;
      // could implement more sofisticated scheme here
      if (curr_v->color == SS_no_pred) {
        victim_v = curr_v;
      } else if (curr_v->color == SS_some_pred) {
        victim_v = curr_v;
        break;
      }
    }
    if (victim_v != 0) {
      // indeed, there is a vertex that is not SS_all_pred
      // we force it to be SS_all_pred, and start again
      victim_v->color = SS_all_pred;
      queue.add_tail(victim_v);
      if (dbg(ms, 3)) cdbg << "victim seleceted: " << *victim_v;
    }
  } while (!queue.is_empty());

  up_order_defined = true;
}

void SS_stage_sched::propagate_up(SS_vertex* v) 
{
  // find ELCOR_MIN(p) among outgoing edges
  // neglect edges outside underlying cycles
  int min_p = infinity;
  for(List_filterator<SS_edge*> i1(v->out_edges, ss_edge_in_ucycles); i1 != 0; i1++) {
    SS_edge* curr_ss_edge = *i1;
    if (dbg(ms, 3)) cdbg << "  out edge:" << *curr_ss_edge;
    min_p = ELCOR_MIN(min_p, curr_ss_edge->p);
    if (min_p == 0) return;
  }
  if (min_p == infinity) return;
  if (dbg(ms, 3)) cdbg << "handle up by " << min_p << ": " << *v;

  // decrease outgoing edges by min_p
  // neglect edges outside underlying cycles
  for(List_filterator<SS_edge*> i2(v->out_edges, ss_edge_in_ucycles); i2 != 0; i2++) {
    SS_edge* curr_ss_edge = *i2;
    curr_ss_edge->p -= min_p;
    assert(curr_ss_edge->p >= 0);
  }

  // increase incomming edges by min_p
  // neglect edges outside underlying cycles
  for(List_filterator<SS_edge*> i3(v->in_edges, ss_edge_in_ucycles); i3 != 0; i3++) {
    SS_edge* curr_ss_edge = *i3;
    curr_ss_edge->p += min_p;
  }
}

void SS_stage_sched::propagate_down(SS_vertex* v) 
{
  // find ELCOR_MIN(p) among incomming edges
  // neglect edges outside underlying cycles
  int min_p = infinity;
  for(List_filterator<SS_edge*> i1(v->in_edges, ss_edge_in_ucycles); i1 != 0; i1++) {
    SS_edge* curr_ss_edge = *i1;
    min_p = ELCOR_MIN(min_p, curr_ss_edge->p);
    if (min_p == 0) return;
  }
  if (min_p == infinity) return;

  // decrease incomming edges by min_p
  // neglect edges outside underlying cycles
  for(List_filterator<SS_edge*> i2(v->in_edges, ss_edge_in_ucycles); i2 != 0; i2++) {
    SS_edge* curr_ss_edge = *i2;
    curr_ss_edge->p -= min_p;
    assert(curr_ss_edge->p >= 0);
  }

  // increase outgoing edges by min_p
  // neglect edges outside underlying cycles
  for(List_filterator<SS_edge*> i3(v->out_edges, ss_edge_in_ucycles); i3 != 0; i3++) {
    SS_edge* curr_ss_edge = *i3;
    curr_ss_edge->p += min_p;
  }
}

//////////////////////////////////////////////////////////////////////
// compute sched times (from p, s, r to sched times)
//////////////////////////////////////////////////////////////////////

bool SS_stage_sched::compute_sched_time()
{
  assert(initalized_graph);

  // init sched times
  for(List_iterator<SS_vertex*> i1(vertex_list); i1!=0; i1++) {
    SS_vertex* curr_v = *i1;
    curr_v->sched_time = -infinity;
  }
  int max_time = -infinity;

  while (true) {
    // find one vertexnot yet assigned to a time
    SS_vertex* unassigned_v = 0;
    for(List_iterator<SS_vertex*> i2(vertex_list); i2!=0; i2++) {
      SS_vertex* curr_v = *i2;
      if (curr_v->sched_time == -infinity) {
        unassigned_v = curr_v;
        break;
      }
    }

    // all updated? yes -> finished 
    if (unassigned_v == 0) {
      // finished this entire procedure
      break;
    }
    
    // handle a connected component
    List<SS_vertex*> queue;
    List<SS_vertex*> processed;

    // arbitrarly schedule unassigned_v in stage 0 (will be normalized later)
    unassigned_v->sched_time = unassigned_v->r;
    queue.add_tail(unassigned_v);

    while (!queue.is_empty()) {
      SS_vertex* curr_v = queue.pop();
      processed.add_tail(curr_v);
      // eq for edge from i to j: timej - timei = rij + (sij + pij) * II

      // handle all output edges
      for(List_iterator<SS_edge*> i3(curr_v->out_edges); i3 != 0; i3++) {
        SS_edge* curr_ss_edge = *i3;
        SS_vertex* dest_v = curr_ss_edge->dest;
        int dist = curr_ss_edge->r + (curr_ss_edge->s + curr_ss_edge->p) * II;
        if (dest_v->sched_time == -infinity) {
          // unvisited vertex: define its time
          dest_v->sched_time = curr_v->sched_time + dist;
          queue.add_tail(dest_v);
	} else {
          // already visited: check consitency
          if (dest_v->sched_time != curr_v->sched_time + dist) {
	    cerr << "problem with edge: " << *curr_ss_edge;
	    cerr << *this;
	  }
          // return false;
          assert(dest_v->sched_time == curr_v->sched_time + dist);
	}
      }
      // handle all input edges
      for(List_iterator<SS_edge*> i4(curr_v->in_edges); i4 != 0; i4++) {
        SS_edge* curr_ss_edge = *i4;
        SS_vertex* src_v = curr_ss_edge->src;
        int dist = curr_ss_edge->r + (curr_ss_edge->s + curr_ss_edge->p) * II;
        if (src_v->sched_time == -infinity) {
          // unvisited vertex: define its time
          src_v->sched_time = curr_v->sched_time - dist;
          queue.add_tail(src_v);
	} else {
          // already visited: check consitency
          if (src_v->sched_time != curr_v->sched_time - dist) {
	    cerr << "problem with edge: " << *curr_ss_edge;
	    cerr << *this;
	  }
          // return false;
          assert(src_v->sched_time == curr_v->sched_time - dist);
	}
      }
    }

    // normalize all sched times such that the earliest sched time is in 
    // stage 0
    int min_time = infinity;
    for(List_iterator<SS_vertex*> i5(processed); i5 != 0; i5++) {
      SS_vertex* curr_v = *i5;
      min_time = ELCOR_MIN(min_time, curr_v->sched_time);
    }
    assert(min_time != infinity);
    int offset = -min_time + mod(min_time, II);

    // sched time here are definitive, except for BRTOP and PBR
    for(List_iterator<SS_vertex*> i6(processed); i6 != 0; i6++) {
      SS_vertex* curr_v = *i6;
      curr_v->sched_time += offset;
      max_time = ELCOR_MAX(max_time, curr_v->sched_time);
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////
// verify solution
//////////////////////////////////////////////////////////////////////

bool SS_stage_sched::verify_sched_time()
{
  assert(initalized_graph);
  valid_stage_sched = true;

  // check that no edges be violated
  for(List_iterator<SS_edge*> i0(edge_list); i0!=0; i0++) {
    SS_edge* curr_ss_edge = *i0;
    Edge* curr_edge = curr_ss_edge->ir_edge;
    SS_vertex* src_v = curr_ss_edge->src;
    int src_time = src_v->sched_time;
    SS_vertex* dest_v = curr_ss_edge->dest;
    int dest_time = dest_v->sched_time + curr_ss_edge->omega * II;
    if (src_time + curr_edge->latency() <= dest_time) {
      curr_ss_edge->violate_latency = false;
    } else {
      curr_ss_edge->violate_latency = true;
      valid_stage_sched = false;
      // return false;
      cerr << "### edge violated in stage schedule:" << endl;
      cerr << "  " << *curr_ss_edge << endl;
    }
  }
  return true;
}

// make a big (and ugly) assumption of computing the register requirements as
// if there is only one def per value. Also neglect predicated bundling
// shoud definitly be improved

int approx_max_live(LoopBody* loop, int II, Vector<int>& max_live)
{
  int zero = 0;
  max_live = Vector<int>((int) NUM_REG_FILE, zero);
  Matrix<int> reg_per_row((int) NUM_REG_FILE, II, zero);
  Edge_filter local_edges = Edge_is_contained_in_region_filter(loop);

  for(Region_all_ops i1(loop); i1!=0; i1++) {
    Op* curr_op = *i1;
    if (is_pseudo(curr_op)) continue;

    // for all destintation ports
    for(Port_num port_num = curr_op->first_dest(); port_num<=curr_op->last_dest(); port_num++) {
      Operand& curr_operand = curr_op->dest(port_num);
      int reg_file = (int) curr_operand.file_type();

      // find the first and last cycle in which the operand is reserved
      int min_start = infinity;
      int max_end = -infinity;

      for(Op_outedges i2(curr_op, &local_edges); i2 != 0; i2++) {
	Edge* edge = *i2;
	Op* dest_op = edge->dest();
	if (is_pseudo(dest_op) || !edge->is_reg_dep()) continue;

	// compute usefull info assoc with this edge
	Port_num dest_num = edge->dest_port();
	Port_num src_num = edge->src_port();
        Operand& src_operand = curr_op->dest(src_num);
        int omega = edge->scratch_int;

        if (curr_operand != src_operand) {
          // we are not targetting the same operand (since def ops may have several defs...)
          // note that we cannot compare curr_operand to dest_operand because their
          // omega may differ (we are considering here the operands of a unique iteration)
          continue;
	}
     
	// compute min and max start time
	int start = curr_op->sched_time()+ curr_op->anti_time(DEST, port_num, OUT);
	int end = dest_op->sched_time()+ omega * II + dest_op->anti_time(SRC, dest_num, IN);

	// update the min/max
	min_start = ELCOR_MIN(min_start, start);
	max_end = ELCOR_MAX(max_end, end);
      }
      if (min_start == infinity && max_end == -infinity) continue;

      // have effectively a value flowing in a reg from cycle
      // min_start to max_end, inclusively
      int full_II_rows = floor(max_end - min_start +1, II);
      max_live[reg_file] += full_II_rows;
      
      // handle fractional parts
      int partial_dist = mod(max_end - min_start +1, II);
      for(int d=0; d<partial_dist; d++) {
        int row = mod(min_start+d, II);
        reg_per_row[reg_file][row]++;
      }
    }
  }
  // compute max among all rows
  int tot = 0;
  for(int reg_file = (int) FIRST_FILE; reg_file<(int) NUM_REG_FILE; reg_file++) {
    int max_value = -infinity;
    for(int row=0; row<II; row++) {
      max_value = ELCOR_MAX(max_value, reg_per_row[reg_file][row]);
    }
    max_live[reg_file] += max_value;
    tot += max_live[reg_file];
  }
  return tot;
}

//////////////////////////////////////////////////////////////////////
// SS_transform
//////////////////////////////////////////////////////////////////////

SS_transform::SS_transform() {}
SS_transform::~SS_transform() {}
void SS_transform::transform(SS_stage_sched* stage_sched) { assert(0); }

//////////////////////////////////////////////////////////////////////
// SS_transform_ac
//////////////////////////////////////////////////////////////////////

SS_transform_ac::SS_transform_ac() {}
SS_transform_ac::~SS_transform_ac() {}
void SS_transform_ac::transform(SS_stage_sched* stage_sched) 
{
  if (dbg(ms, 1)) cdbg << "Stage Sched Transform: acyclic" << endl;
  for(List_iterator<SS_edge*> i0(stage_sched->edge_list); i0!=0; i0++) {
    SS_edge* curr_ss_edge = *i0;
    if (!curr_ss_edge->in_underlying_cycle) {
      curr_ss_edge->p = 0;
    }
  }
}
  
//////////////////////////////////////////////////////////////////////
// SS_transform_ss
//////////////////////////////////////////////////////////////////////

SS_transform_ss::SS_transform_ss() {}
SS_transform_ss::~SS_transform_ss() {}
void SS_transform_ss::transform(SS_stage_sched* stage_sched) 
{
  if (dbg(ms, 1)) cdbg << "Stage Sched Transform: sink/source" << endl;
  stage_sched->construct_reg_sink_source();

  for(List_iterator<SS_vertex*> i1(stage_sched->vertex_list); i1!=0; i1++) {
    SS_vertex* curr_v = *i1;

    if (curr_v->reg_source_vertex && curr_v->reg_sink_vertex) {
      // do nothing
    } else if (curr_v->reg_source_vertex) {
      if (dbg(ms, 2)) cdbg << "reg source: " << *curr_v;
      stage_sched->propagate_up(curr_v);
    } else if (curr_v->reg_sink_vertex) {
      if (dbg(ms, 2)) cdbg << "reg sink: " << *curr_v;
      stage_sched->propagate_down(curr_v);
    }
  }  
}
  
//////////////////////////////////////////////////////////////////////
// SS_transform_up
//////////////////////////////////////////////////////////////////////

SS_transform_up::SS_transform_up(int repeat) : num(repeat) {}
SS_transform_up::~SS_transform_up() {}
void SS_transform_up::transform(SS_stage_sched* stage_sched) 
{
  if (dbg(ms, 1)) {
    cdbg << "Stage Sched Transform: up propagation " << num << " times" << endl;
  }
  stage_sched->construct_sink_source();
  stage_sched->construct_up_order();

  for(int i0=0; i0<num; i0++) {
    for(List_iterator<SS_vertex*> i1(stage_sched->up_order); i1!=0; i1++) {
      SS_vertex* curr_v = *i1;
      if (!curr_v->sink_vertex){
	if (dbg(ms, 2)) cdbg << "handle " << *curr_v;
	stage_sched->propagate_up(curr_v);
      }
    }
  }
}




