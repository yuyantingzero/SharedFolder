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
//      File:           multilevel.h
//      Author:         Mike Chu
//      Created:        September 2002
//      Description:    Graph partitioning through coarsening
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MULTILEVEL_H
#define _MULTILEVEL_H

#include "string_class.h"
#include "opcode.h"
#include "operand.h"
#include "slist.h"
#include "matrix.h"
#include "coarse_op.h"

#define XXX_MAX_RU_LENGTH 1000
#define XXX_MAX_IOPAT 100
#define XXX_INF 10000
#define XXX_MOVE_LATENCY 1
#define XXX_HIGHEST_EDGE_WT 10
#define XXX_HIGHER_EDGE_WT 8
#define XXX_MED_EDGE_WT 4
#define XXX_LOW_EDGE_WT 1

class Cluster_mgr;
class Compound_region;
class Op;
class DepLen_OpInfo;

///////////////////////////////////////////////////////////////////////
//
// Multilevel
//   

enum Resource_type {
  I_unit,
  F_unit,
  M_unit,
  B_unit,
  pseudo,
};

class MultiLevel : public Cluster_algorithm {

public:

   // constructor / destructor
   MultiLevel(Cluster_mgr*);
   ~MultiLevel();
   void static_init();

   // Weight calculation functions
   void find_global_move_weight();
   void setup_block(Compound_region*);
   void get_slacks_and_ranges(Compound_region*);
   void set_control_dep_lat_to_zero(Compound_region*);
   void insert_block_pseudo_defs_and_uses(Compound_region*);
   void find_block_node_weights(Compound_region* rgn);
   double compute_node_weight(Op* op, int clstr);

   void find_block_edge_weights(Compound_region*);
   void fix_defs_and_uses(Compound_region*);
   void find_block_start_edges(Compound_region*, Slist<Stuple<int, Edge*> >&);
   void process_edge_up(Edge*, Slist<Stuple<int, Edge*> >&);
   void process_edge_down(Edge*);
   void find_block_max_estart(Compound_region*);
   void find_block_total_node_weights(Compound_region*);   

   void print_node_weights();
   void print_edge_weights();

   // coarsening/refining functions
   void do_block_setup_and_clustering(Compound_region*);
   void do_block_ml_coarsening(Compound_region*, Vector<Coarse_op>&, bool);
   void do_block_ml_refine_partition(Compound_region*, Vector<Coarse_op>&);
   void do_block_ml_final_pass(Compound_region*, Vector<Coarse_op>&);

   // binding functions
   void do_block_bind_ops(Compound_region*);
   void do_postpass_operand_check();
   void remove_block_pseudo_defs_and_uses(Compound_region*);

   // utility functions
   void count_icmoves(Hash_set<Op*>&);
   double calculate_edge_gain(Coarse_op* cop, int, int);
   int new_calculate_weight(Hash_set<Op*>&, int);
   int even_newer_calculate_weight(Hash_set<Op*>&, int);
   double calculate_weight(Hash_set<Op*>&, int);
   double calculate_system_load(Vector<Hash_set<Op*> >&);
   double new_calculate_system_load(Vector<Hash_set<Op*> >&);
   double even_newer_calculate_system_load(Vector<Hash_set<Op*> >&);
   int find_imbalanced_cluster(Hash_set<int>&);
   Resource_type get_resource_type(Op*);

private:
   Vector< Hash_set<Op*> > clusters;
   Hash_map<Opcode, List<Op*> > opcode_map;

   // Kevin
   Vector<double> total_node_weights;
   int max_estart;
   Hash_map<Op*, Vector<double> > node_weights;
   Hash_map<Edge*, float> edge_weights;

   Vector<List<Op*> > per_cycle_ops;
   Hash_map<Op*, int> slack_map;

   Hash_map<Op*, DepLen_OpInfo*> dep_info;

   static double move_weight;

   int ops;
   int num_icmoves;
};

unsigned hash_ml_op(Op*&);

#endif
