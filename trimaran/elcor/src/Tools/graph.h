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
//      File:           graph.h
//      Author:         Nate Clark
//      Created:        May, 2005
//      Description:    Some graph algorithms. Originally written to replace
//                      the vflib graph isomorphism algorithms.
//
/////////////////////////////////////////////////////////////////////////////


#ifndef __GRAPH
#define __GRAPH

#include <deque>
#include <set>
#include "hash_map.h"
#include "vector.h"

using namespace std;

template <class T>
class Graph
{
public:
  Graph() { }
  virtual ~Graph() { }

  // returns the internal id of the node inserted;
  unsigned insert_node(T*, int weight = 1);
  inline int get_node_weight(unsigned n) const { return node_weights[n]; }
  inline T*& operator[](unsigned n) { return nodes[n]; }
  inline const T* operator[](unsigned n) const { return nodes[n]; }
  inline unsigned num_nodes() const { return nodes.size(); }
  virtual void insert_edge(unsigned from, unsigned to) = 0;
  virtual unsigned num_edges() const = 0;
  bool is_edge_between(unsigned from, unsigned to) const;
  void set_edge_weight(unsigned from, unsigned to, int weight);
  inline int get_edge_weight(unsigned from, unsigned to) const;

  // Returns true if there is a subgraph of 'this' isomorphic to
  // 'small_graph'. The comparison_function is used to determine
  // whether or not two nodes are identical, equivalent to operator==,
  // but uses pointers instead of the object. This would be a const
  // function, but there's a timeout variable that records when the
  // algorithm starts running.
  bool is_subgraph_isomorphic(const Graph<T>& small_graph,
			      bool (*comparison_function)(T*, T*));

  // Finds all subgraphs of 'this' that are isomorphic to
  // 'small_graph'. The comparison_function is used to determine
  // whether or not two nodes are identical, equivalent to operator==,
  // but uses pointers instead of the object. 'matches' is filled in
  // with node ids from 'this' that correspond to the isomorphic
  // subgraphs. This would be a const function, but there's a timeout
  // variable that records when the algorithm starts running.
  void find_isomorphic_subgraphs(const Graph<T>& small_graph,
				 bool (*comparison_function)(T*, T*),
				 deque<Vector<unsigned> >& matches);

  // Calls Metis to partition the graph into 'ways' different
  // partitions. The node and edge weights are used to prioritize the
  // partitioning algorithm. 'partitions' is filled in with 'ways'
  // number of deques of node ids, which represent the partitioning
  // results.
  void partition_graph(int ways, deque<deque<unsigned> >& partitions) const; 

  // Fills in the set with all nodes that reachable from 'node'
  // following the edges either forward or backward.
  void find_reachable_nodes(unsigned node, set<unsigned>& reachable_nodes,
			    bool search_forward = true) const;

  unsigned find_dependence_height(unsigned node, bool search_forward = true) const;

protected:
  bool assign_nodes(const Graph<T>& small_graph,
		    Vector<unsigned>& current_answer,
		    const Vector<unsigned>& assignment_order,
		    set<unsigned>& large_graph_nodes_assigned,
		    Vector<set<unsigned> >& candidate_nodes,
		    unsigned node_to_assign,
		    deque<Vector<unsigned> >* matches) const;


  deque<T*> nodes;

  // A priority weighting for the various nodes in the graph. These
  // data are used during graph partitioning.
  deque<int> node_weights;

  // I'm assuming that edges are sparse using this data structure
  // (which is the case in the dataflow graphs I intend to use this
  // data structure to represent). The set contains the node ids
  // connected to the deque index by an edge. I.e., if
  // edges[3].count(8) > 0, then there's an edge between nodes 3 and
  // 8.
  deque<set<unsigned> > edges;
  
  // A priority weighting for the various edges in the graph. These
  // data are used during graph partitioning. If an edge does not have
  // a weight assigned, it's assumed to be weight 0.
  deque<Hash_map<unsigned, int> > edge_weights;

  // Used to prematurely end potentially long running algorithms.
  time_t start_time;
};


template <class T>
class UndirectedGraph : public Graph<T>
{
public:
  virtual void insert_edge(unsigned from, unsigned to);
  virtual unsigned num_edges() const;
};


template <class T>
class DirectedGraph : public Graph<T>
{
public:
  virtual void insert_edge(unsigned from, unsigned to);
  virtual unsigned num_edges() const;
  void find_all_reachable_nodes(unsigned node, set<unsigned>& reachable_nodes) const;
};


#endif
