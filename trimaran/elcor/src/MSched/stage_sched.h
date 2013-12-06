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
//      File:           stage_sched.h
//      Authors:        Alex Eichenberger, Vinod Kathail
//      Created:        June 1995
//      Description:    Stage scheduling of operations in a swp loop 
//                      to reduce register lifetimes
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _STAGE_SCHED_H_
#define _STAGE_SCHED_H_

#include "edge.h"
#include "region.h"
#include "op.h"
#include "map.h"
#include "filter.h"

class SS_vertex;
class SS_edge;
class SS_stage_sched;
class SS_transform;
class SS_transform_ac;
class SS_transform_ss;
class SS_transform_up;
class SS_edge_not_in_ucycle_filter;
class Edge_filter_for_stage_scheduling;
class Edge_filter_for_reg_edges;

//////////////////////////////////////////////////////////////////////
// externally visible stage scheduling class
//////////////////////////////////////////////////////////////////////

class SS_stage_sched {
  friend ostream& operator<< (ostream&, SS_stage_sched&);
  friend class SS_transform;
  friend class SS_transform_ac;
  friend class SS_transform_ss;
  friend class SS_transform_up;

public: 
  SS_stage_sched();
  SS_stage_sched(LoopBody* loop, int ii, Edge_filter* reg_edge_filter=0);
  ~SS_stage_sched();

  // perform default transform and modify IR, if the transform is valid
  bool transform_default();

  // perform the transform and modify IR, if the transform is valid
  bool transform(SS_transform& transform);

  // return MaxLive per reg file and its sum
  int  approx_max_live(Vector<int> &max_live_vector);

  // print a vcg graph of the current stage schedule
  void vcg_graph(char* name, bool fussy=false);

private:
  void init();
  void reset();
  bool read_ir();

  // find edges in uderlying cycles
  void find_biconnected_edges();
  void searchb(SS_vertex* v);
  void handle_pair(SS_vertex* v, SS_vertex* w, SS_edge* e); 

  // transforms
  void construct_up_order();
  void construct_sink_source();
  void construct_reg_sink_source();
  void propagate_up(SS_vertex* v);
  void propagate_down(SS_vertex* v);

  // convert back to sched time
  bool compute_sched_time();
  bool verify_sched_time();
  void commit();

  // input data
  int                   II; 
  LoopBody*             loopbody;

  // data for stage scheduling
  List<SS_edge*>        edge_list;
  List<SS_vertex*>      vertex_list;
  Map<Op*, SS_vertex*>* vertex_map;
  SS_vertex*            brtop_v;
  SS_vertex*            pbr_v;

  // data for biconnected edges
  int                   count;
  List<SS_edge*>        stack;

  // used by some transforms
  List<SS_vertex*>      up_order;

  // state variables
  bool                  initalized_graph;
  bool                  valid_stage_sched;
  bool                  up_order_defined;
  bool                  sink_source_defined;
  bool                  reg_sink_source_defined;

  // filters
  Edge_filter*                  edge_filter;
  Edge_filter*                  register_edge_filter;
  SS_edge_not_in_ucycle_filter* ss_edge_in_ucycles;
};

// compute MaxLive (assuming one def per value)
int approx_max_live(LoopBody* loop, int ii, Vector<int>& max_live);

//////////////////////////////////////////////////////////////////////
// externally visible transform class
//////////////////////////////////////////////////////////////////////

class SS_transform {
public:
  SS_transform();
  virtual ~SS_transform();
  virtual void transform(SS_stage_sched* stage_sched);
};

class SS_transform_ac : public SS_transform {
public:
  SS_transform_ac();
  ~SS_transform_ac();

  void transform(SS_stage_sched* stage_sched);
};

class SS_transform_ss : public SS_transform {
public:
  SS_transform_ss();
  ~SS_transform_ss();

  void transform(SS_stage_sched* stage_sched);
};

class SS_transform_up : public SS_transform {
public:
  SS_transform_up(int repeat = 1);
  ~SS_transform_up();

  void transform(SS_stage_sched* stage_sched);

private:
  int num;
};

//////////////////////////////////////////////////////////////////////
// externally visible edge filter class
//////////////////////////////////////////////////////////////////////

class Edge_filter_for_reg_edges: public Edge_filter {
public:
  Edge_filter_for_reg_edges();
  Edge_filter_for_reg_edges(Vector<bool>& reg_file_considered);
  ~Edge_filter_for_reg_edges();
  bool filter(Edge * const & edge) const;
private:
  Vector<bool> reg_file_on;
};

//////////////////////////////////////////////////////////////////////
// internal edge class
//////////////////////////////////////////////////////////////////////

class SS_edge {
  friend ostream& operator<< (ostream&, SS_edge&);
public:
  SS_edge();
  SS_edge(Edge* e, int o, SS_vertex* v_src, SS_vertex* v_dest, int i,
    Edge_filter* reg_edge);
  ~SS_edge();

  void init();

  // input
  Edge* ir_edge;
  int   latency;
  int   omega;
  int   id;

  // data for stage scheduling
  int   p_initial;
  int   p;
  int   s;
  int   r;
  bool  in_underlying_cycle;
  bool  reg_attribute;
  bool  sched_attribute;
  bool  violate_latency;

  // data for biconnected edges
  bool  stacked;  

  SS_vertex* src;
  SS_vertex* dest;
};

//////////////////////////////////////////////////////////////////////
// internal vertex class
//////////////////////////////////////////////////////////////////////

enum SS_color {SS_all_pred, SS_some_pred, SS_no_pred};

class SS_vertex {
  friend ostream& operator<< (ostream&, SS_vertex&);
public:
  SS_vertex();
  SS_vertex(Op* o, int II);
  ~SS_vertex();

  void init();

  // input
  Op*            ir_op;
  List<SS_edge*> in_edges;
  List<SS_edge*> out_edges;
  int            id;

  // data for biconnected edges
  bool           visited;
  int            df_num;
  int            low;
  SS_vertex*     father;

  // used to construct up/down order
  SS_color       color;

  // used by transforms
  bool           sink_vertex;
  bool           source_vertex;
  bool           reg_sink_vertex;
  bool           reg_source_vertex;

  // used to convert back the relative sched times
  int            r;
  int            sched_time;
  int            initial_sched_time;
};

//////////////////////////////////////////////////////////////////////
// internal filter classes
//////////////////////////////////////////////////////////////////////

class SS_edge_not_in_ucycle_filter: public Filter<SS_edge*> {
public:
  bool filter(SS_edge * const & ss_edge) const;
};

class Edge_filter_for_stage_scheduling: public Edge_filter {
public:
  bool filter(Edge * const & edge) const;
};

#endif


