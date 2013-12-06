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
//      File:           el_dfg.cpp
//      Authors:        Sadun Anik
//      Created:        May 1997
//      Description:    Data flow graph implementation
//
/////////////////////////////////////////////////////////////////////////////

#include "el_dfg.h"

El_dfg::El_dfg(Region* r)
   : g(r) 
{
   latency.resize(g.edge_count) ;
   etime.resize(g.node_count) ;
   ltime.resize(g.node_count) ;   
   slack.resize(g.node_count) ;
   
   for(int ei = 0 ; ei < g.edge_count ; ei++) {
      latency[ei] = g.b_edge_map[ei]->latency() ;
   }
}

void El_dfg::compute_e_time()
{
   int i ;
   List_iterator<int> inedgei ;

   for (i = 0 ; i < g.node_count ; i++) etime[i] = 0 ;
   for (Alist_graph_post_dfs ai(g, UP) ; ai != 0 ; ai++) {
      int cur_op = *ai ;
      for(inedgei(g.entry_edge_map[cur_op]) ; inedgei != 0 ; inedgei++) {
	 int cur_edge = *inedgei ;
	 int cur_src_op = g.edge_src_node_map[cur_edge] ;
	 int etime_candidate = etime[cur_src_op] + latency[cur_edge] ;
	 if (etime[cur_op] < etime_candidate) etime[cur_op] = etime_candidate;
      }
   }
}
void El_dfg::compute_l_time()
{
   List_iterator<int> oedgei ;

   for (Alist_graph_post_dfs ai(g) ; ai != 0 ; ai++) {
      int cur_op = *ai ;
      oedgei(g.exit_edge_map[cur_op]) ;
      if (oedgei == 0) ltime[cur_op] = etime[cur_op] ;  // exit ops get initialized
      else ltime[cur_op] = ELCOR_MAX_INT ; // others get initialized
      for( ; oedgei != 0 ; oedgei++) {
	 int cur_edge = *oedgei ;
	 int cur_dest_op = g.edge_dest_node_map[cur_edge] ;
	 int ltime_candidate = ltime[cur_dest_op] - latency[cur_edge] ;
	 if (ltime[cur_op] > ltime_candidate) ltime[cur_op] = ltime_candidate;
      }
   }
}

void El_dfg::compute_slack()
{
   compute_e_time() ;
   compute_l_time() ;
   for(int i = 0 ; i < g.node_count ; i++) {
      slack[i] = ltime[i] - etime[i] ; 
   }
}

void El_dfg::eliminate_redundant_edges()
{
   //    If there are more than one edge between to ops,
   //    remove all but the longest latency edge
   int src_id, i ;
   List_iterator<int> ei ;

   // Eliminate simple redundant edges
   Vector<int> cur_op_dest_max_latency(g.node_count) ;
   Vector<int> cur_op_dest_max_edge_index(g.node_count) ;
   for (src_id = 0 ; src_id < g.node_count ; src_id++) {
      for (i = 0 ; i < g.node_count ; i ++) {
	 cur_op_dest_max_latency[i] =  ELCOR_MIN_INT ;
	 cur_op_dest_max_edge_index[i] = -1 ;
      }
      for (ei(g.exit_edge_map[src_id]) ; ei != 0 ; ei++) {
	 int cur_edge = *ei ;
	 int dest_id = g.edge_dest_node_map[cur_edge] ;
	 if (latency[cur_edge] > cur_op_dest_max_latency[dest_id]) {
	    if (cur_op_dest_max_edge_index[dest_id] >= 0) {
	       latency[cur_op_dest_max_edge_index[dest_id]] = ELCOR_MIN_INT ;
	    }
	    cur_op_dest_max_latency[dest_id] = latency[cur_edge] ;
	    cur_op_dest_max_edge_index[dest_id] = cur_edge ;
	 }
	 else {
	    latency[cur_edge] = ELCOR_MIN_INT ;
	 }
      }
   }
   // Now all the redundant edges have latency ELCOR_MIN_INT
   // Remove them from the graph data structures entry_edge_map
   // exit_edge_map, f_edge_map. Redo the edge_to  Resize and garbage collect
   // the vectors edge_src_node_map edge_dest_node_map and
   // b_edge_map to eliminate these edges

   Bitvector mask(g.edge_count) ;
   for (i = 0 ; i < g.edge_count ; i++) {
      if (latency[i] != ELCOR_MIN_INT) mask.set_bit(i) ;
   }
   eliminate_edges(mask) ;
}

void El_dfg::transitively_reduce_edges()
{
   //    Compute longest paths on a reduced graph and
   //    remove edges which are superceded by longer paths
   //    from it src to its dest.
   // 1) Iterate in reverse post-order dfs to traverse
   //    edges in a forward direction. This guarantees
   //    that an out edge does not go to an already visited
   //    node.
   // 2) For the n-th node, assume that the previous n-1
   //    nodes are connected with reduced set of edges. Eliminate
   //    the redundant incoming edges.

   // The following array keeps partially computed longest path
   // lengths. MIN_ELCOR_INT implies no path. A row i of this
   // Vector is the Vector of longest path lengths to node i from 
   // predecessor nodes, where entry j is dist(j,i). 
   //
   Vector<int> t(g.node_count,ELCOR_MIN_INT) ; // for initialization only
   Vector<Vector<int> > longest_paths(g.node_count,t) ;
   
   Alist_graph_post_dfs gi(g, UP) ;

   // mask we will construct
   Bitvector edge_mask(g.edge_count) ;
   
   // Start the iterative longest path calculator

   // This for the longest path from node_i to cur_node,
   // the entry edge_on_path_to_cur_node[node_i] contains
   // the name of the inedge to cur_node. The inedges to
   // cur_node which survive at the end make it to the edge_mask.
   Vector<int> edge_on_path_to_cur_node(g.node_count, -1) ;
   List_iterator<int> inedgei ;
   for (gi.reset() ; gi != 0 ; gi++) {
      int cur_node = *gi ;
      // path_length_to_cur_node will contain the new longest
      // path lengths to cur_node from its predecessors 
      Vector<int>& path_length_to_cur_node = longest_paths[cur_node] ;
      for (inedgei(g.entry_edge_map[*gi]) ; inedgei!= 0 ; inedgei++) {
	 int cur_inedge = *inedgei ;
	 int cur_edge_lat = latency[cur_inedge] ;
	 int cur_src = g.edge_src_node_map[cur_inedge] ;
	 if (path_length_to_cur_node[cur_src] < cur_edge_lat) {
	    // a new path is created from cur_src to cur_dest
	    path_length_to_cur_node[cur_src] = cur_edge_lat ;
	    edge_on_path_to_cur_node[cur_src] = cur_inedge ;
	    Vector<int>& path_length_to_cur_src = longest_paths[cur_src] ;
	    for (int i = 0 ; i < g.node_count ; i++) {
	       if(path_length_to_cur_src[i] > ELCOR_MIN_INT) {
		  // there is a path from i to cur_src, see if cur_edge
		  // makes a longest path from i to cur_node.
		  int i_to_cur_node_length = path_length_to_cur_src[i] +
		                             cur_edge_lat ;
		  if (path_length_to_cur_node[i] < i_to_cur_node_length) {
		     path_length_to_cur_node[i] = i_to_cur_node_length ;
		     edge_on_path_to_cur_node[i] = cur_inedge ;
		  }
	       }
	    }
	 }
      }
      for (int j = 0 ; j <  g.node_count ; j++) {
	 if (edge_on_path_to_cur_node[j] >= 0 ) {
	    // There is a path from j to cur_node, set the bit for the
	    // edge on this path in the edge_mask.
	    edge_mask.set_bit(edge_on_path_to_cur_node[j]) ;
	    edge_on_path_to_cur_node[j] = -1 ;
	 }
      }
   }
   eliminate_edges(edge_mask) ;
}

void El_dfg::eliminate_edges(Bitvector& edge_mask)
{
   g.eliminate_edges(edge_mask) ;
   shrink_vector(latency, edge_mask) ;
}
