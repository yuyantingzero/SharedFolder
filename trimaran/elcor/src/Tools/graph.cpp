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
//      File:           graph.cpp
//      Author:         Nate Clark
//      Created:        May, 2005
//      Description:    Some graph algorithms. Originally written to replace
//                      the vflib graph isomorphism algorithms.
//
/////////////////////////////////////////////////////////////////////////////


#include "graph.h"
#include "el_error.h"
#include "dbg.h"

extern "C" {
#include "metis.h"
}

using namespace std;


template <class T>
inline unsigned Graph<T>::insert_node(T* new_node, int weight)
{
  nodes.push_back(new_node);
  edges.push_back(set<unsigned>());
  node_weights.push_back(weight);
  return (nodes.size() - 1);
}


template <class T>
inline bool Graph<T>::is_edge_between(unsigned from, unsigned to) const
{
  return (edges[from].find(to) != edges[from].end());
}


template <class T>
void Graph<T>::set_edge_weight(unsigned from, unsigned to, int weight)
{
  if( ! is_edge_between(from, to)) {
    El_warn("Not setting edge weight, because the edge doesn't exist!\n");
  }

  while(edge_weights.size() < from + 1)
    edge_weights.push_back(Hash_map<unsigned, int>(hash_unsigned, 32));

  edge_weights[from].bind(to, weight);
}


template <class T>
inline int Graph<T>::get_edge_weight(unsigned from, unsigned to) const
{
  if(edge_weights.size() <= from)
    return 1;
  return edge_weights[from][to];
}


template <class T>
void Graph<T>::find_reachable_nodes(unsigned node, set<unsigned>& reachable_nodes,
				    bool search_forward) const
{
  reachable_nodes.clear();

  // Do a breadth first search up, and find all the nodes.
  deque<unsigned> some_queue;
  some_queue.push_back(node);
  reachable_nodes.insert(node);
  while( ! some_queue.empty()) {
    unsigned cur_node = some_queue.front();
    some_queue.pop_front();

    if(search_forward) {
      for(set<unsigned>::iterator it = edges[cur_node].begin();
	  it != edges[cur_node].end(); it++) {
	if(reachable_nodes.count(*it) == 0) {
	  reachable_nodes.insert(*it);
	  some_queue.push_back(*it);
	}	
      }
    } else {
      // There's no good way to follow edges backwards with the
      // current data structure. Consider adding this information if
      // this algorithm becomes a performance limiter.
      for(unsigned j = 0; j < num_nodes(); j++) {
	if(reachable_nodes.count(j) == 0) {
	  if(is_edge_between(j, cur_node)) {
	    reachable_nodes.insert(j);
	    some_queue.push_back(j);
	  }
	}
      } // for j
    } // else

  } // while( ! some_queue.empty())
}


template <class T>
unsigned Graph<T>::find_dependence_height(unsigned node, bool search_forward) const
{
  Vector<int> heights(nodes.size(), -1);
  heights[node] = 0;
  deque<unsigned> some_queue;
  some_queue.push_back(node);
  while( ! some_queue.empty()) {
    unsigned cur_node = some_queue.front();
    some_queue.pop_front();

    if(search_forward) {
      for(set<unsigned>::iterator it = edges[cur_node].begin();
	  it != edges[cur_node].end(); it++) {
	if(heights[*it] == -1) {
	  some_queue.push_back(*it);
	  heights[*it] = heights[cur_node] + 1;
	}
      }
    } else {
      // There's no good way to follow edges backwards with the
      // current data structure. Consider adding this information if
      // this algorithm becomes a performance limiter.
      for(unsigned j = 0; j < num_nodes(); j++) {
	if(heights[j] == -1) {
	  if(is_edge_between(j, cur_node)) {
	    some_queue.push_back(j);
	    heights[j] = heights[cur_node] + 1;
	  }
	}
      } // for j
    }
  }

  int max = 0;
  for(unsigned i = 0; i < heights.size(); i++) {
    max = ELCOR_MAX(max, heights[i]);
  }
  return (unsigned)max;
}


template <class T>
bool Graph<T>::is_subgraph_isomorphic(const Graph<T>& small_graph,
				      bool (*comparison_function)(T*, T*))
{
  if(small_graph.num_nodes() > num_nodes())
    return false;

  if(small_graph.num_edges() > num_edges())
    return false;

  // the_answer[id from small_graph] = the node in 'this'
  // corresponding to the small_graph id.
  Vector<unsigned> the_answer(small_graph.num_nodes(), ELCOR_MAX_INT);
  set<unsigned> large_graph_nodes_assigned;

  // Data structures used to prune the candidate list.  Essentially
  // find the set of nodes reachable from a given node. This set in a
  // large graph candidate must be at least as large as the small
  // graph set for the large graph node to be a candidate.
  deque<set<unsigned> > small_graph_above_set(small_graph.num_nodes()),
    small_graph_below_set(small_graph.num_nodes()),
    large_graph_above_set(num_nodes()),
    large_graph_below_set(num_nodes());

  for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
    small_graph.find_reachable_nodes(i, small_graph_above_set[i], false);
    small_graph.find_reachable_nodes(i, small_graph_below_set[i], true);
  }

  // And repeat the whole thing for 'this', the large graph.
  for(unsigned i = 0; i < num_nodes(); i++) {
    find_reachable_nodes(i, large_graph_above_set[i], false);
    find_reachable_nodes(i, large_graph_below_set[i], true);
  }

  // FIX: This only works for acyclic graphs.
  Vector<unsigned> small_graph_above_height(small_graph.num_nodes()),
    small_graph_below_height(small_graph.num_nodes()),
    large_graph_above_height(num_nodes()),
    large_graph_below_height(num_nodes());

  for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
    small_graph_above_height[i] = small_graph.find_dependence_height(i, false);
    small_graph_below_height[i] = small_graph.find_dependence_height(i, true);
  }

  for(unsigned i = 0; i < num_nodes(); i++) {
    large_graph_above_height[i] = find_dependence_height(i, false);
    large_graph_below_height[i] = find_dependence_height(i, true);
  }

  // candidates[i] contains the group of nodes in the large graph that
  // could potentially match up with node i in the small graph.
  Vector<set<unsigned> > candidates(small_graph.num_nodes());
  for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
    T* small_node = small_graph.nodes[i];
    for(unsigned j = 0; j < nodes.size(); j++) {
      T* large_node = nodes[j];
      if(comparison_function(small_node, large_node)) {
	if(small_graph_above_set[i].size() <= 
	   large_graph_above_set[j].size()) {
	  if(small_graph_below_set[i].size() <= 
	     large_graph_below_set[j].size()) {
	    if(small_graph_above_height[i] <= large_graph_above_height[j]) {
	      if(small_graph_below_height[i] <= large_graph_below_height[j]) {
		candidates[i].insert(j);
	      }
	    }
	  }
	}
      }
    }
    if(candidates[i].size() == 0)
      return false;
  }

  // assignment_order[0] = node id to assign first,
  // assignment_order[1] = node id to assign second, etc. We do this
  // to allow an ordering which (hopefully) reduces the number of states to be searched.
  Vector<unsigned> assignment_order(small_graph.num_nodes(), ELCOR_MAX_INT);

  // Do a depth first search on the graph. Assigning the nodes in a
  // connected order will prune bad paths quickly. I'm assuming a
  // fully connected graph using this algorithm.
  deque<unsigned> assn_stack;
  Vector<bool> assigned(small_graph.num_nodes(), false);
  unsigned assn_number = 0;
  // Start with the last node. This is empirically a good choice,
  // based on the typical shape of dataflow graphs.
  assn_stack.push_back(small_graph.num_nodes() - 1);
  assigned[small_graph.num_nodes() - 1] = true;
  while( ! assn_stack.empty()) {
    unsigned cur_node = assn_stack.front();
    assignment_order[assn_number++] = cur_node;
    assn_stack.pop_front();
    for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
      if((i != cur_node) && ( ! assigned[i])) {
	if(small_graph.is_edge_between(i, cur_node) ||
	   small_graph.is_edge_between(cur_node, i)) {
	  assn_stack.push_front(i);
	  assigned[i] = true;
	}
      }
    }
  }

  // Make sure all the nodes are on the stack. This is necessary for
  // disconnected graphs.
  for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
    if( ! assigned[i]) {
      assignment_order[assn_number++] = i;
    }
  }

  start_time = time(NULL);
  bool answer = assign_nodes(small_graph, the_answer, assignment_order,
		      large_graph_nodes_assigned, candidates, 0, NULL);
  return answer;
}


template <class T>
void Graph<T>::find_isomorphic_subgraphs(const Graph<T>& small_graph,
					 bool (*comparison_function)(T*, T*),
					 deque<Vector<unsigned> >& matches)
{
  if(small_graph.num_nodes() > num_nodes()) {
    return;
  }

  if(small_graph.num_edges() > num_edges()) {
    return;
  }

  // the_answer[id from small_graph] = the node in 'this'
  // corresponding to the small_graph id.
  Vector<unsigned> the_answer(small_graph.num_nodes(), ELCOR_MAX_INT);
  set<unsigned> large_graph_nodes_assigned;

  // Data structures used to prune the candidate list.  Essentially
  // find the set of nodes reachable from a given node. This set in a
  // large graph candidate must be at least as large as the small
  // graph set for the large graph node to be a candidate.
  deque<set<unsigned> > small_graph_above_set(small_graph.num_nodes()),
    small_graph_below_set(small_graph.num_nodes()),
    large_graph_above_set(num_nodes()),
    large_graph_below_set(num_nodes());

  for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
    small_graph.find_reachable_nodes(i, small_graph_above_set[i], false);
    small_graph.find_reachable_nodes(i, small_graph_below_set[i], true);
  }

  // And repeat the whole thing for 'this', the large graph.
  for(unsigned i = 0; i < num_nodes(); i++) {
    find_reachable_nodes(i, large_graph_above_set[i], false);
    find_reachable_nodes(i, large_graph_below_set[i], true);
  }

  // candidates[i] contains the group of nodes in the large graph that
  // could potentially match up with node i in the small graph.
  Vector<set<unsigned> > candidates(small_graph.num_nodes());
  for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
    T* small_node = small_graph.nodes[i];
    for(unsigned j = 0; j < nodes.size(); j++) {
      T* large_node = nodes[j];
      if(comparison_function(small_node, large_node)) {
	if(small_graph_above_set[i].size() <= 
	   large_graph_above_set[j].size()) {
	  if(small_graph_below_set[i].size() <= 
	     large_graph_below_set[j].size()) {
	    candidates[i].insert(j);
	  }
	}
      }
    }
    if(candidates[i].size() == 0) {
      return;
    }
  }

  // assignment_order[0] = node id to assign first,
  // assignment_order[1] = node id to assign second, etc. We do this
  // to allow an ordering which (hopefully) reduces the number of states to be searched.
  Vector<unsigned> assignment_order(small_graph.num_nodes());

  // Do a depth first search on the graph. Assigning the nodes in a
  // connected order will prune bad paths quickly. I'm assuming a
  // fully connected graph using this algorithm.
  deque<unsigned> assn_stack;
  Vector<bool> assigned(small_graph.num_nodes(), false);
  unsigned assn_number = 0;
  // start with node 0 for no particular reason.
  assn_stack.push_back(small_graph.num_nodes() - 1);
  assigned[small_graph.num_nodes() - 1] = true;
  while( ! assn_stack.empty()) {
    unsigned cur_node = assn_stack.front();
    assignment_order[assn_number++] = cur_node;
    assn_stack.pop_front();
    for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
      if((i != cur_node) && ( ! assigned[i])) {
	if(small_graph.is_edge_between(i, cur_node) ||
	   small_graph.is_edge_between(cur_node, i)) {
	  assn_stack.push_front(i);
	  assigned[i] = true;
	}
      }
    }
  }

  // Make sure all the nodes are on the stack. This is necessary for
  // disconnected graphs.
  for(unsigned i = 0; i < small_graph.num_nodes(); i++) {
    if( ! assigned[i]) {
      assignment_order[assn_number++] = i;
    }
  }

  start_time = time(NULL);
  assign_nodes(small_graph, the_answer, assignment_order, large_graph_nodes_assigned,
	       candidates, 0, &matches);
}


// This algorithm was based on "Common subgraph isomorphism detection
// by backtracking search" by Krissinel and Henrick, Software -
// Practice and Experience 2004 34:591-607
template <class T>
bool Graph<T>::assign_nodes(const Graph<T>& small_graph,
			    Vector<unsigned>& current_answer,
			    const Vector<unsigned>& assn_order,
			    set<unsigned>& large_graph_nodes_assigned,
			    Vector<set<unsigned> >& candidates,
			    unsigned curr_node_num,
			    deque<Vector<unsigned> >* matches) const
{
  // 5 Minute timout
  if(300 < (time(NULL) - start_time))
    return false;

  // If we're trying to assign a node in the small graph larger than
  // the number of nodes in the graph, that means all the nodes have
  // successfully been assigned, and we're done.
  if(curr_node_num >= small_graph.num_nodes()) {
    if(matches)
      matches->push_back(current_answer);
    return true;
  }

  unsigned node_to_assign = assn_order[curr_node_num];

  // Pick a node from 'this' that hasn't been assigned, and matches
  // the current node from small_graph we're trying to assign.
  for(set<unsigned>::iterator it = candidates[node_to_assign].begin();
      it != candidates[node_to_assign].end(); it++) {
    unsigned node_num = *it;

    // Check that the large node doesn't already map to a node in the
    // small graph.
    if(large_graph_nodes_assigned.find(node_num) == large_graph_nodes_assigned.end()) {

      // Check that all edges from this node in 'small_graph' match
      // up with the edges in 'this'. Since I'm assigning the nodes
      // in ascending numerical order, it's only necessary to check
      // that previous nodes' edges match.
      bool the_edges_are_ok = true;
      for(unsigned j = 0; j < current_answer.size(); j++) {

	if((j != node_to_assign) && (current_answer[j] != ELCOR_MAX_INT)) {
	  // if there's an edge in the small graph...
	  if(small_graph.is_edge_between(node_to_assign, j)) {
	    // then there needs to be an equivalent edge in the larger
	    // graph.
	    if( ! is_edge_between(node_num, current_answer[j])) {
	      the_edges_are_ok = false;
	      break;
	    }
	  }

	  // FIX: this check is only necessary for the directed
	  // graph. Maybe define the function in the derived class to
	  // make it a bit more efficient.
	  if(small_graph.is_edge_between(j, node_to_assign)) {
	    // then there needs to be an equivalent edge in the larger
	    // graph.
	    if( ! is_edge_between(current_answer[j], node_num)) {
	      the_edges_are_ok = false;
	      break;
	    }
	  }
	}
      }


      // If the edges seem to match up, then this could work. Update
      // the data structures, and see if the rest of the graph
      // works. If it doesn't, just try a new node.
      if(the_edges_are_ok) {

	current_answer[node_to_assign] = node_num;
	large_graph_nodes_assigned.insert(node_num);

	// Prune the search space.
	deque<deque<unsigned> > removed_candidates;
	for(unsigned k = 0; k < current_answer.size(); k++) {

	  if((k != node_to_assign) && (current_answer[k] != ELCOR_MAX_INT)) {
	    deque<unsigned> to_remove;

	    for(set<unsigned>::iterator it2 = candidates[k].begin();
		it2 != candidates[k].end(); it2++) {
	      unsigned cand_num = *it2;
	      if(small_graph.is_edge_between(k, node_to_assign)) {
		if( ! is_edge_between(cand_num, current_answer[node_to_assign])) {
		  to_remove.push_back(cand_num);
		}
	      }

	      // FIX: this check is only necessary for the directed
	      // graph. Maybe define this function in the derived class to
	      // make it a bit more efficient.
	      if(small_graph.is_edge_between(node_to_assign, k)) {
		if( ! is_edge_between(current_answer[node_to_assign], cand_num)) {
		  to_remove.push_back(cand_num);
		}
	      }
	    }
	    
	    for(deque<unsigned>::iterator li = to_remove.begin(); li != to_remove.end(); li++) {
	      candidates[k].erase(*li);
	    }
	    
	    // If pruning left the set of candidates empty, then this
	    // node assignment can't possibly work.
	    if(candidates[k].size() == 0) {
	      the_edges_are_ok = false;
	    }
	    removed_candidates.push_back(to_remove);
	  } else {
	    removed_candidates.push_back(deque<unsigned>());
	  }
	}

	if(the_edges_are_ok) {
	  if(assign_nodes(small_graph, current_answer, assn_order,
			  large_graph_nodes_assigned,
			  candidates, curr_node_num + 1, matches)) {
	    // This assignment worked. The subgraph is isomorphic.

	    // When matches is NULL, we're just checking if a subgraph
	    // isomorphism exists, and can terminate the search. If
	    // matches is not NULL, then we're recording all instances
	    // of subgraph isomorphism, and need to keep searching.
	    if( ! matches)
	      return true;
	  }
	}

	// This assignment of nodes was not isomorphic. Undo the
	// assignment and try another node.
	current_answer[node_to_assign] = ELCOR_MAX_INT;
	large_graph_nodes_assigned.erase(node_num);

	// Put the pruned nodes back into the candidates list.
	unsigned k = 0;
	for(deque<deque<unsigned> >::iterator it = removed_candidates.begin();
	    it != removed_candidates.end(); it++, k++) {
	  const deque<unsigned>& deq = *it;
	  for(deque<unsigned>::const_iterator it2 = deq.begin(); it2 != deq.end();
	      it2++) {
	    candidates[k].insert(*it2);
	  }
	}
      }
    }
  }

  // Couldn't find a node in 'this' to map 'node_to_assign' to.
  return false;
}


template<class T>
void Graph<T>::partition_graph(int ways, deque<deque<unsigned> >& partitions) const
{
  partitions.clear();

  assert(ways >= 1);

  // Convert the internal graph representation into a format Metis
  // understands. This code is sparsly commented, but follows the same
  // convention used in the Metis documentation on page 18
  // (cccp/elcor/metis-4.0/Doc/manual.ps).

  idxtype* xadj = new idxtype[num_nodes() + 1];
  assert (xadj && "out of memory");

  idxtype* vwgt = new idxtype[num_nodes()];
  assert (vwgt && "out of memory");
  for(unsigned i = 0; i < num_nodes(); i++) {
    vwgt[i] = node_weights[i];
  }

  unsigned n_edges = num_edges();

  idxtype* adjncy = new idxtype[n_edges + 1];
  assert (adjncy && "out of memory");

  idxtype* adjwgt = new idxtype[n_edges + 1];
  assert (adjwgt && "out of memory");

  // FIX: this only works for directed graphs
  unsigned edge_num = 0;
  for(unsigned i = 0; i < num_nodes(); i++) {
    xadj[i] = edge_num;
    for(set<unsigned>::iterator it = edges[i].begin(); it != edges[i].end(); it++) {
      unsigned from = i, to = *it;
      adjncy[edge_num] = to;
      adjwgt[edge_num] = get_edge_weight(from, to);
      edge_num++;
    }
  }
  xadj[num_nodes()] = edge_num;


  int num_vert = (int)num_nodes();
  int wgtflag  = 3;          // This indicates that both edge and node weights are used.
  int numflag  = 0;          // This indicates that C-style (0 relative) numbering is used.
  int options[5] = {0, 0, 0, 0, 0};   // Use default partitioning algorithm.
  int edgecut = 0;           // Returns how many edges were cut in the answer.
  idxtype* the_partition = new idxtype[num_nodes()];  // Holds the answer
  assert(the_partition && "out of memory");

  METIS_PartGraphRecursive(&num_vert, xadj, adjncy, vwgt, adjwgt, &wgtflag, &numflag,
			   &ways, options, &edgecut, the_partition);

//   cdbg << "Edge cut: " << edgecut << endl;
  
  for(int i = 0; i < ways; i++) {
    partitions.push_back(deque<unsigned>());
    for(unsigned j = 0; j < num_nodes(); j++) {
      if(the_partition[j] == i) {
	partitions[i].push_back(j);
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////
// UndirectedGraph

template <class T>
void UndirectedGraph<T>::insert_edge(unsigned from, unsigned to)
{
  if(this->edges[from].find(to) == this->edges[from].end())
    this->edges[from].insert(to);
  if(this->edges[to].find(from) == this->edges[to].end())
    this->edges[to].insert(from);
}


template <class T>
unsigned UndirectedGraph<T>::num_edges() const
{
  unsigned total_edges = 0;
  for(deque<set<unsigned> >::const_iterator it = this->edges.begin();
      it != this->edges.end(); it++) {
    total_edges += (*it).size();
  }
  // Need to divide by two because each edge is stored redundantly as
  // being from A to B and from B to A.
  return (total_edges >> 1);
}



////////////////////////////////////////////////////////////////////////////
// DirectedGraph


template <class T>
void DirectedGraph<T>::insert_edge(unsigned from, unsigned to)
{
  if(this->edges[from].find(to) == this->edges[from].end())
    this->edges[from].insert(to);
}


template <class T>
unsigned DirectedGraph<T>::num_edges() const
{
  unsigned total_edges = 0;
  for(deque<set<unsigned> >::const_iterator it = this->edges.begin();
      it != this->edges.end(); it++) {
    total_edges += (*it).size();
  }
  return total_edges;
}


template <class T>
void DirectedGraph<T>::find_all_reachable_nodes(unsigned node,
						set<unsigned>& reachable_nodes) const
{
  reachable_nodes.clear();

  // Do a breadth first search up, and find all the nodes.
  deque<unsigned> some_queue;
  some_queue.push_back(node);
  reachable_nodes.insert(node);
  while( ! some_queue.empty()) {
    unsigned cur_node = some_queue.front();
    some_queue.pop_front();

    // There's no good way to follow edges backwards with the
    // current data structure. Consider adding this information if
    // this algorithm becomes a performance limiter.
    for(unsigned j = 0; j < this->num_nodes(); j++) {
      if(reachable_nodes.count(j) == 0) {
	if(this->is_edge_between(j, cur_node) || this->is_edge_between(cur_node, j)) {
	  reachable_nodes.insert(j);
	  some_queue.push_back(j);
	}
      }
    } // for j

  } // while( ! some_queue.empty())  
}



