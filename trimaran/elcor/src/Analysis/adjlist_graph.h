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




///////////////////////////////////////////////////////////////////////////
//
//      File:        adjlist_graph.h
//      Authors:     Sadun Anik, David August
//      Created:     March, 1995   
//      Description: Graph implemented as adjacency list. Used to build 
//                   abstract control-flow graphs for analysis.
//
///////////////////////////////////////////////////////////////////////////


#ifndef _ADJLIST_GRAPH_H
#define _ADJLIST_GRAPH_H

#include "hash_map.h"
#include "hash_set.h"
#include "vector.h"
#include "region.h"

enum GRAPH_CATEGORY {
   GRAPH_SUBREGIONS ,  // Graph defined at next level
   GRAPH_OPS ,         // Op level graph
   GRAPH_OPS_PLUS ,    // Graph at layer above Ops
   GRAPH_BBS ,         // BB graph (full tiling required)
   GRAPH_HBS ,         // HB graph (full tiling required)
   GRAPH_HBS_SESE ,    // HB graph (full tiling required)
                       // SESE refers to the "star" representation of a HB
   GRAPH_HBS_BBS ,     // HB+BB tiling where HB's can have BB's (pick HB's)
   GRAPH_HBS_BBS_SESE  // HB+BB tiling where HB's can have BB's (pick HB's)
                       // SESE refers to the "star" representation of a HB
} ;

enum SEARCH_DIRECTION
{
   UP,                  // Search up using predecessors
   DOWN                 // Search down using successors
};

class Alist_graph {
public:
   Alist_graph() ;
   ~Alist_graph() ;
   
   // method for graph with edges
   void eliminate_edges(Bitvector& include_list);
   
   // Data structures for nodes
   Vector<List<int> > succ_list ;
   Vector<List<int> > pred_list ;
   int node_count ;
   
   
   //  Data structures for edges 
   Vector<List<int> > entry_edge_map ; // Node to incoming edge map
   Vector<List<int> > exit_edge_map ;  // Node to outgoing edge map
   Vector<int> edge_src_node_map ;          // Edge to source node map
   Vector<int> edge_dest_node_map ;         // Edge to dest node map
   int edge_count ;
   
} ;

class Alist_region_hierarchy_graph : public Alist_graph {
public:
   Alist_region_hierarchy_graph(Region*) ;
   Hash_map<Region*,int> f_map ;
   Vector<Region*> b_map ;
} ;

class Alist_dfg_graph : public Alist_graph {
public:
   Alist_dfg_graph(Region*) ;

   void eliminate_edges(Bitvector& include_list);

   Hash_map<Region*,int> f_map ;
   Vector<Region*> b_map ;
   
   Hash_map<Edge*,int> f_edge_map ;
   Vector<Edge*> b_edge_map ;
} ;

class Alist_region_tiling_graph : public Alist_graph {
public:
   Alist_region_tiling_graph() ;
   Alist_region_tiling_graph(Region*, GRAPH_CATEGORY gc) ;  
   Hash_map<Region*,int> f_map ;
   Vector<Region*> b_map ;
} ;

class Alist_control_graph : public Alist_region_tiling_graph {
public:
   Alist_control_graph(Region*) ;
   void constrict_graph(Bitvector include_list, int head, bool redirect);
   void restore_graph();
   
   
   int top;
   int bottom;
   
   
//   Hash_map<Region*,int> f_map ;
//   Vector<Region*> b_map ;
   
   Bitvector included;
   
private:
   Bitvector orig_included;
   Vector<List<int> > orig_succ_list ;
   Vector<List<int> > orig_pred_list ;
} ;

class Alist_edge_graph {
public:
   Alist_edge_graph() ;
   Alist_edge_graph(Alist_graph& ng) ;
   ~Alist_edge_graph() ;
   
   Vector<List<int> > entry_edge_map ; // Node to incoming edge map
   Vector<List<int> > exit_edge_map ;  // Node to outgoing edge map
   Vector<int> src_node_map ;          // Edge to source node map
   Vector<int> dest_node_map ;         // Edge to dest node map
} ;

class Alist_graph_iterator {
   public:
      Alist_graph_iterator() ;
      Alist_graph_iterator(Alist_graph&) ;   
      ~Alist_graph_iterator() ;

      bool operator==(const int) const ;
      bool operator!=(const int) const ;

      void operator++() ;
      void operator++(int) ;
      int operator*() ;

      void reset() ;
   protected:
   
      Alist_graph *graph ;
      int size ;
      Vector<int> iter_list ;

      int iter_index ; 
   
} ;

class Alist_graph_dfs : public Alist_graph_iterator {
   public:
      Alist_graph_dfs() ;
      Alist_graph_dfs(Alist_graph&, SEARCH_DIRECTION = DOWN,
		      int start = -1, int stop = -1) ;
      ~Alist_graph_dfs() ;

   private:
      void visit(int, int) ;
      void listdfs(int, int) ;
   
      Vector<List<int> >* adj_list ;
      Vector<int> val ;

      int id ; 
} ;

class Alist_graph_post_dfs : public Alist_graph_iterator {
   public:
      Alist_graph_post_dfs() ;
      Alist_graph_post_dfs(Alist_graph&, SEARCH_DIRECTION = DOWN,
			   int = -1, int = -1) ;
      ~Alist_graph_post_dfs() ;
   private:
      void visit(int, int) ;
      void listdfs(int, int) ;
   
      Vector<List<int> >* adj_list ;
      Vector<int> val ;

      int id ; 
} ;

class Alist_graph_bfs : public Alist_graph_iterator {
   public:
      Alist_graph_bfs() ;
   
      Alist_graph_bfs(Alist_graph&, SEARCH_DIRECTION = DOWN,
		      int start = -1, int stop = -1) ;
      ~Alist_graph_bfs() ;

   private:
      void do_search(int, int) ;
      void listbfs(int, int) ;
   
      Vector<List<int> >* adj_list ;
      Vector<int> val ;

      int id ; 
} ;

class Alist_graph_lazy_bfs {
   public:
      Alist_graph_lazy_bfs() ;
      Alist_graph_lazy_bfs(Alist_graph&, SEARCH_DIRECTION = DOWN,
			   int = -1, int = -1) ;   
      ~Alist_graph_lazy_bfs() ;

      bool operator==(const int) const ;
      bool operator!=(const int) const ;

      void operator++() ;
      void operator++(int) ;
      int operator*() ;

      void reset() ;
   protected:
      void advance () ;
      
      Alist_graph *graph ;
      Vector<List<int> >* adj_list ;
      Bitvector visited ;
      int size ;
      int first_node ;
      int last_node ;

      List<int> node_queue ;
      int cur_node ;
} ;

#endif









