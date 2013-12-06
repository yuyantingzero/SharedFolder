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
//      File:           flow_analysis_solver.cpp
//      Authors:        Sadun Anik, Richard Johnson
//      Created:        April 1995
//      Description:    Global liveness analyzer
//
/////////////////////////////////////////////////////////////////////////////

#include "flow_analysis_solver.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "dfa.h"
#include "attributes.h"
#include "dbg.h"
#include "edge_utilities.h"
#include "attributes.h"
#include "hash_functions.h"
#include "operand.h"
#include "edge.h"
#include "op.h"
#include "el_stats_init.h"
#include "el_clock.h"


////////////////////////////////////////////////////////////////
//
// class Liveness_solver_data
//
////////////////////////////////////////////////////////////////

Liveness_solver_data::Liveness_solver_data() {}

Liveness_solver_data::Liveness_solver_data(int n)
   : live_in(n) , live_out(n), live_out_initial(n),
     liveness_gen(n), liveness_kill(n),
     down_exposed_use_in(n),down_exposed_use_in_initial(n),
     down_exposed_use_out(n),
     down_exposed_use_gen(n), down_exposed_use_kill(n),
     up_exposed_def_in(n), up_exposed_def_out(n),
     up_exposed_def_out_initial(n),
     up_exposed_def_gen(n), up_exposed_def_kill(n),
     down_exposed_def_in(n), down_exposed_def_in_initial(n),
     down_exposed_def_out(n),
     down_exposed_def_gen(n), down_exposed_def_kill(n)
{}

Liveness_solver_data::~Liveness_solver_data() {}

bool Liveness_solver_data::operator==(const Liveness_solver_data& d) const {
   return((live_in == d.live_in) &&
	  (live_out == d.live_out) &&
	  (live_out_initial == d.live_out_initial) &&	  
	  (liveness_gen == d.liveness_gen) &&
	  (liveness_kill == d.liveness_kill) &&
	  (down_exposed_use_in == d.down_exposed_use_in) &&
	  (down_exposed_use_in_initial == d.down_exposed_use_in_initial) &&
	  (down_exposed_use_out == d.down_exposed_use_out) &&
	  (down_exposed_use_gen == d.down_exposed_use_gen) &&
	  (down_exposed_use_kill == d.down_exposed_use_kill) &&
	  (up_exposed_def_in == d.up_exposed_def_in) &&
	  (up_exposed_def_out == d.up_exposed_def_out) &&
	  (up_exposed_def_out_initial == d.up_exposed_def_out_initial) &&
	  (up_exposed_def_gen == d.up_exposed_def_gen) &&
	  (up_exposed_def_kill == d.up_exposed_def_kill) &&
	  (down_exposed_def_in == d.down_exposed_def_in) &&
	  (down_exposed_def_in_initial == d.down_exposed_def_in_initial) &&
	  (down_exposed_def_out == d.down_exposed_def_out) &&
	  (down_exposed_def_gen == d.down_exposed_def_gen) &&
	  (down_exposed_def_kill == d.down_exposed_def_kill)) ;
}

////////////////////////////////////////////////////////////////
//
// class Liveness_solver 
//
////////////////////////////////////////////////////////////////

Liveness_solver::Liveness_solver()
: operand_map(hash_operand, 10007)
{}

Liveness_solver::Liveness_solver(Region* r, unsigned int filter_flag)
   : graph(r,GRAPH_HBS_BBS_SESE), operand_map(hash_operand, 10007),
   filter_mask(filter_flag) 
{
   List<Operand> local_vr_names ;
// Initialize the Operand to integer map
   int symbol_counter = 1 ;

   if (dbg(dfa,1)) cdbg << "Declaring the solver" << endl;

   for(Region_all_ops op_iter(r) ; op_iter != 0 ; op_iter++) {
      Op* cur_op = *op_iter ;
      if (is_remap(cur_op)) {
	 local_vr_names.add_tail(cur_op->src(SRC1)) ; // It is OK to have multiple entry
      }
      for (Op_complete_inputs src_oper(cur_op) ; src_oper != 0 ; src_oper++) {
	 Operand* tmpo = &(*src_oper) ;
	 if (!operand_map.is_bound(*tmpo)) {
	    if (is_interesting(*tmpo)) {
	       operand_map.bind(*tmpo,symbol_counter) ;
	       symbol_counter++ ;
	    }
	    else {
	       operand_map.bind(*tmpo,0) ;
	    }
	 }
      }
      for (Op_complete_dests dest_oper(cur_op) ; dest_oper != 0 ; dest_oper++) {
	 Operand* tmpo = &(*dest_oper) ;
	 if (!operand_map.is_bound(*tmpo)) {
	    if (is_interesting(*tmpo)) {
	       operand_map.bind(*tmpo,symbol_counter) ;
	       symbol_counter++ ;
	    }
	    else {
	       operand_map.bind(*tmpo,0) ;
	    }
	 }
      }
   }
//
// Add symbols form up-exposed-defs and up-exposed uses sets to the
// region symbol table
//   
   for(Region_exit_edges rexi(r) ; rexi != 0 ; rexi++) {
      Edge* cur_edge = *rexi ;
      Liveness_info* linfo ;
      Liveness_info_iterator li ;

      linfo = get_liveness_info(cur_edge) ;
      if (linfo) {
	 for (li(*linfo) ; li != 0 ; li++) {
	    Operand& tmpoper = *li ;
	    if (!operand_map.is_bound(tmpoper)) {
	       operand_map.bind(tmpoper,symbol_counter) ;
	       symbol_counter++ ;
	    }
	 }
      }
      linfo = get_up_exposed_defs_info(cur_edge) ;
      if (linfo) {
	 for (li(*linfo) ; li != 0 ; li++) {
	    Operand& tmpoper = *li ;
	    if (!operand_map.is_bound(tmpoper)) {
	       operand_map.bind(tmpoper,symbol_counter) ;
	       symbol_counter++ ;
	    }
	 }
      }
   }
//
// Add symbols form down-exposed-defs and down-exposed uses sets to the
// region symbol table
//   
   for(Region_entry_edges renti(r) ; renti != 0 ; renti++) {
      Edge* cur_edge = *renti ;
      Liveness_info* linfo ;
      Liveness_info_iterator li ;
      linfo = get_down_exposed_uses_info(cur_edge) ;
      if (linfo) {
	 for (li(*linfo) ; li != 0 ; li++) {
	    Operand& tmpoper = *li ;
	    if (!operand_map.is_bound(tmpoper)) {
	       operand_map.bind(tmpoper,symbol_counter) ;
	       symbol_counter++ ;
	    }
	 }
      }
      linfo = get_down_exposed_defs_info(cur_edge) ;
      if (linfo) {
	 for (li(*linfo) ; li != 0 ; li++) {
	    Operand& tmpoper = *li ;
	    if (!operand_map.is_bound(tmpoper)) {
	       operand_map.bind(tmpoper,symbol_counter) ;
	       symbol_counter++ ;
	    }
	 }
      }
   }

// Expand the vr_names into full names
   for (List_iterator<Operand> vri(local_vr_names) ; vri != 0 ; vri++) {
      Operand oper = *vri ;
      int min_omega = oper.min_omega() ;
      int max_omega = oper.max_omega() ;
      for (int i=min_omega ; i<=max_omega ; i++) { // define idx..max_idx
	 Operand new_reg = new Reg(oper);
	 new_reg.incr_omega(i);
	 if (!operand_map.is_bound(new_reg)) {
	    operand_map.bind(new_reg, symbol_counter);
	    symbol_counter++ ;
	 }
      }
   }
   if (dbg(dfa,1)) cdbg << "Solver symbol table completed" << endl;
   
// Initialize the reverse map   
   rev_operand_map.resize(symbol_counter) ;
   for (Hash_map_iterator<Operand,int> mdi(operand_map) ;
	mdi != 0 ; mdi++) {
      if ((*mdi).second > 0) {
	 rev_operand_map[(*mdi).second] = (*mdi).first ;
      }
   }
   if (dbg(dfa,1)) cdbg << "Reverse symbol map done" << endl;
      
// Initialize the data 
   
   Liveness_solver_data dinit(symbol_counter) ; // Is this one too many?
   live_info.resize(graph.succ_list.size(), dinit) ;

// Now extract boundary info from the graph if it exists
// First the liveness and up exposed defs at exits
   for (Region_exit_edges exi(r) ; exi!= 0 ; exi++) {
      // Find the id of the boundary region which is the source of this exit
      Liveness_info_iterator ii ;
      Edge* cur_edge = *exi ;
      Region* rsrc = cur_edge->src() ;
      while (!graph.f_map.is_bound(rsrc)) {
	 rsrc = rsrc->parent() ;
	 if (!rsrc) {
	    El_punt("Source of a region exit edge is not contained in a tile of the region") ;
	 }
      }
      int region_id = graph.f_map.value(rsrc) ;
      // Initialize the liveout info at boundaries      
      Liveness_info* linfo = get_liveness_info(cur_edge) ;
      if (linfo != NULL) {
	 Bitvector& live_out = live_info[region_id].live_out_initial ;
	 for (ii(*linfo) ; ii != 0 ; ii++) {
	    Operand& tmpoper = *ii ;
	    int pos = operand_map.value(tmpoper) ;
	    live_out.set_bit(pos) ;
	 }
      }
      // Initialize the up_exposed_defs info at boundaries
      linfo = get_up_exposed_defs_info(cur_edge) ;
      if (linfo != NULL) {
	 Bitvector& ued = live_info[region_id].up_exposed_def_out_initial ;
	 for (ii(*linfo) ; ii != 0 ; ii++) {
	    Operand& tmpoper = *ii ;
	    int pos = operand_map.value(tmpoper) ;
	    ued.set_bit(pos) ;
	 }
      }
   }
// Next the down exposed uses and defs
   for (Region_entry_edges eni(r) ; eni!= 0 ; eni++) {
      // Find the id of the boundary region which is the source of this exit
      Liveness_info_iterator ii ;
      Edge* cur_edge = *eni ;
      Region* rdest = cur_edge->dest() ;
      while (!graph.f_map.is_bound(rdest)) {
	 rdest = rdest->parent() ;
	 if (!rdest) {
	    El_punt("Destination of a region entry edge is not contained in a tile of the region") ;
	 }
      }
      int region_id = graph.f_map.value(rdest) ;
      // Initialize the liveout info at boundaries      
      Liveness_info* linfo = get_down_exposed_uses_info(cur_edge) ;
      if (linfo != NULL) {
	 if (dbg(dfa,1)) {
	    cdbg << "Initializing down exposed use in for region " ;
	    cdbg << rdest->id() << "\n"  ;
	    for (Liveness_info_iterator li(*linfo) ; li !=0 ; li++) {
	       cdbg  << *li ;
	    }
	 }
	 Bitvector& deu = live_info[region_id].down_exposed_use_in_initial ;
	 for (ii(*linfo) ; ii != 0 ; ii++) {
	    Operand& tmpoper = *ii ;
	    int pos = operand_map.value(tmpoper) ;
	    deu.set_bit(pos) ;
	 }
      }
      // Initialize the up_exposed_defs info at boundaries
      linfo = get_up_exposed_defs_info(cur_edge) ;
      if (linfo != NULL) {
	 Bitvector& ded = live_info[region_id].down_exposed_def_in_initial ;
	 for (ii(*linfo) ; ii != 0 ; ii++) {
	    Operand& tmpoper = *ii ;
	    int pos = operand_map.value(tmpoper) ;
	    ded.set_bit(pos) ;
	 }
      }
   }
   
// Set the def and use information 
   if (dbg(dfa,1)) cdbg << "Initialized liveness info arrays" << endl;
   
   for (unsigned i = 0 ; i < live_info.size() ; i++) {
      set_transfer_function(i) ;
   }
   if (dbg(dfa,1)) cdbg << "Transfer functions done" << endl;
   
}

Liveness_solver::~Liveness_solver() {}


void Liveness_solver::initialize_flow_info() {
   assert(0) ;
}

void Liveness_solver::solve_liveness() {
   bool not_done = true ;

   int iteration_count = 0  ;
   if (dbg(dfa,1)) cdbg << "Starting liveness" << endl  ;
   //initialize_flow_info() ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(graph) ; iter != 0 ; iter++) {
	 liveness_meet_function(*iter, not_done) ;
	 liveness_transfer_function(*iter, not_done) ;
      }
   }
#ifdef DEBUG
   if (dbg(dfa,1)) {
      cdbg << "Total of " << iteration_count << " iterations \n\n" ;
      cdbg << "Total of " << live_info[1].live_in.size() << " variables \n\n" ;
      cdbg << endl ;

      for (Alist_graph_dfs iter(graph) ; iter != 0 ; iter++) {
	 Compound_region* r = (Compound_region*) graph.b_map[*iter] ;
	 cdbg << "live-in at block " << r->id() << endl;

	 int size = live_info[*iter].live_in.size() ;
	 for (int j = 1 ; j < size ; j++) {
	    if (live_info[*iter].live_in.bit(j)) {
	       cdbg << rev_operand_map[j] << " " ;
	    }
	 }
	 cdbg << endl << endl << endl ; 
      }
   }
#endif // DEBUG   
}

void Liveness_solver::solve_down_exposed_uses() {
   bool not_done = true ;

   int iteration_count = 0  ;
   if (dbg(dfa,1)) cdbg << "Starting down exposed uses" << endl ;
   //initialize_flow_info() ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(graph,UP) ; iter != 0 ; iter++) {
	 down_exposed_uses_meet_function(*iter, not_done) ;
	 down_exposed_uses_transfer_function(*iter, not_done) ;
      }
   }
}

void Liveness_solver::solve_up_exposed_defs() {
   bool not_done = true ;

   int iteration_count = 0  ;
   if (dbg(dfa,1)) cdbg << "Starting up exposed uses" << endl ;
   //initialize_flow_info() ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(graph) ; iter != 0 ; iter++) {
	 up_exposed_defs_meet_function(*iter, not_done) ;
	 up_exposed_defs_transfer_function(*iter, not_done) ;
      }
   }
}

void Liveness_solver::solve_down_exposed_defs() {
   bool not_done = true ;

   int iteration_count = 0  ;
   if (dbg(dfa,1)) cdbg << "Starting down exposed defs" << endl ;
   //initialize_flow_info() ;
   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      for (Alist_graph_post_dfs iter(graph,UP) ; iter != 0 ; iter++) {
	 down_exposed_defs_meet_function(*iter, not_done) ;
	 down_exposed_defs_transfer_function(*iter, not_done) ;
      }
   }
}

void Liveness_solver::print_liveness_to_graph ()
{
  for (Alist_graph_dfs iter(graph); iter != 0; iter++) {
    Region* r1 = graph.b_map[*iter];

    if (r1->is_op () || r1->is_bb ()) {
      Op* src_op;

      if (r1->is_op())
	src_op = (Op*)r1;
      else
	src_op = get_last_region_op_from_subregions (r1);

      for (List_iterator<int> succi (graph.succ_list[*iter]); succi != 0;
	   succi++) {
	Region* r2 = graph.b_map[*succi] ;

	if (r2->is_op ())
	  El_punt ("Liveness_solver: Successor of an op is another op !!!") ;

	Op* dest_op =  get_first_region_op_from_subregions (r2);
	Edge* cur_edge = get_connecting_CONTROL0_edge (src_op, dest_op);
	Liveness_info* live_set = get_liveness_info (cur_edge);

	if (live_set == NULL)
	  live_set = new Liveness_info();
	else
	  live_set->clear() ;

	Bitvector live = live_info[*iter].live_out ;
	live *= live_info[*succi].live_in ;

	for (Bit_iterator bi(live); bi != 0; bi++) {
	  int i = *(bi);
	  (*live_set).add_tail(rev_operand_map[i]) ;
	}

	if (dbg(dfa,1)) {
	  cdbg << "Liveness between " << src_op->parent()->id() ;
	  cdbg << " -> " << dest_op->parent()->id() << "\n"  ;
	  for (Liveness_info_iterator li (*live_set) ; li !=0 ; li++) {
	    cdbg  << *li ;
	    cdbg  << endl;
	  }
	  cdbg << endl;
	}

	set_liveness_info (cur_edge, live_set) ;
      }
    }
  }
}

void Liveness_solver::print_down_exposed_uses_to_graph() {
   for (Alist_graph_dfs iter(graph) ; iter != 0 ; iter++) {
      Region* r1 = graph.b_map[*iter] ;
      if (r1->is_op() || r1->is_bb()) {
	 Op* src_op ;
	 if (r1->is_op()) src_op = (Op*) r1 ;
	 else src_op = get_last_region_op_from_subregions(r1) ;
	 for (List_iterator<int> succi(graph.succ_list[*iter]) ; succi != 0 ;
								 succi++) {
	    Region* r2 = graph.b_map[*succi] ;
	    if (r2->is_op()) {
	       El_punt("Liveness_solver: Successor of an op is another op !!!") ;
	    }
	    Op* dest_op =  get_first_region_op_from_subregions(r2) ;
	    Edge* cur_edge = get_connecting_CONTROL0_edge(src_op, dest_op) ;
	    Liveness_info* down_exposed_use_set = get_down_exposed_uses_info(cur_edge) ;
	    if (down_exposed_use_set == NULL) {
	       down_exposed_use_set = new Liveness_info() ;
	    }
	    else {
	       down_exposed_use_set->clear() ;
	    }
	    Bitvector deu = live_info[*iter].down_exposed_use_out ;
	    deu *= live_info[*succi].down_exposed_use_in ;
	    for (Bit_iterator bi(deu) ; bi != 0 ; bi++) {
	       int i = *(bi);
	       (*down_exposed_use_set).add_tail(rev_operand_map[i]) ;
	    }
	    if (dbg(dfa,1)) {
	       cdbg << "Down exposed uses between " << src_op->parent()->id() ;
	       cdbg  << " -> " << dest_op->parent()->id() << "\n"  ;
	       for (Liveness_info_iterator li(*down_exposed_use_set) ; li !=0 ; li++) {
		  cdbg  << *li ;
		  cdbg  << endl;
	       }
	       cdbg << flush << endl;
	    }
	    set_down_exposed_uses_info(cur_edge,down_exposed_use_set) ;
	 }
      }
   }
}

void Liveness_solver::print_up_exposed_defs_to_graph() {
   for (Alist_graph_dfs iter(graph) ; iter != 0 ; iter++) {
      Region* r1 = graph.b_map[*iter] ;
      if (r1->is_op() || r1->is_bb()) {
	 Op* src_op ;
	 if (r1->is_op()) src_op = (Op*) r1 ;
	 else src_op = get_last_region_op_from_subregions(r1) ;
	 for (List_iterator<int> succi(graph.succ_list[*iter]) ; succi != 0 ;
								 succi++) {
	    Region* r2 = graph.b_map[*succi] ;
	    if (r2->is_op()) {
	       El_punt("Liveness_solver: Successor of an op is another op !!!") ;
	    }
	    Op* dest_op =  get_first_region_op_from_subregions(r2) ;  
	    Edge* cur_edge = get_connecting_CONTROL0_edge(src_op, dest_op) ;
	    Liveness_info* up_exposed_def_set = get_up_exposed_defs_info(cur_edge) ;
	    if (up_exposed_def_set == NULL) {
	       up_exposed_def_set = new Liveness_info() ;
	    }
	    else {
	       up_exposed_def_set->clear() ;
	    }
	    Bitvector ued = live_info[*iter].up_exposed_def_out ;
	    ued *= live_info[*succi].up_exposed_def_in ;
	    for (Bit_iterator bi(ued) ; bi != 0; bi++) {
	       (*up_exposed_def_set).add_tail(rev_operand_map[*(bi)]) ;
	    }
	    if (dbg(dfa,1)) {
	       cdbg<< "Up exposed defs between " << src_op->parent()->id() ;
	       cdbg << " -> " << dest_op->parent()->id() << "\n"  ;
	       for (Liveness_info_iterator li(*up_exposed_def_set) ; li !=0 ; li++) {
		  cdbg  << *li ;
		  cdbg  << endl;
	       }
	       cdbg << flush << endl;
	    }
	    set_up_exposed_defs_info(cur_edge,up_exposed_def_set) ;
	 }
      }
   }
}

void Liveness_solver::print_down_exposed_defs_to_graph() {
   for (Alist_graph_dfs iter(graph) ; iter != 0 ; iter++) {
      Region* r1 = graph.b_map[*iter] ;
      if (r1->is_op() || r1->is_bb()) {
	 Op* src_op ;
	 if (r1->is_op()) src_op = (Op*) r1 ;
	 else src_op = get_last_region_op_from_subregions(r1) ;
	 for (List_iterator<int> succi(graph.succ_list[*iter]) ; succi != 0 ;
								 succi++) {
	    Region* r2 = graph.b_map[*succi] ;
	    if (r2->is_op()) {
	       El_punt("Liveness_solver: Successor of an op is another op !!!") ;
	    }
	    Op* dest_op =  get_first_region_op_from_subregions(r2) ;
	    Edge* cur_edge = get_connecting_CONTROL0_edge(src_op, dest_op) ;
	    Liveness_info* down_exposed_def_set = get_down_exposed_defs_info(cur_edge) ;
	    if (down_exposed_def_set == NULL) {
	       down_exposed_def_set = new Liveness_info() ;
	    }
	    else {
	       down_exposed_def_set->clear() ;
	    }
	    Bitvector ded = live_info[*iter].down_exposed_def_out ;
	    ded *= live_info[*succi].down_exposed_def_in ;
	    for (Bit_iterator bi(ded) ; bi != 0 ; bi++) {
	       (*down_exposed_def_set).add_tail(rev_operand_map[*(bi)]) ;
	    }
	    if (dbg(dfa,1)) {
	       cdbg << "Down exposed defs between " << src_op->parent()->id() ;
	       cdbg << " -> " << dest_op->parent()->id() << "\n"  ;
	       for (Liveness_info_iterator li(*down_exposed_def_set) ; li !=0 ; li++) {
		  cdbg  << *li ;
		  cdbg  << endl;
	       }
	       cdbg << flush << endl;
	    }
	    set_down_exposed_defs_info(cur_edge,down_exposed_def_set) ;
	 }
      }
   }
}


void  Liveness_solver::set_list_to_liveins(List<Operand>& live_list, Basicblock* bb)
{
   int bb_id = graph.f_map.value(bb) ;
   Bitvector& live = live_info[bb_id].live_in ;

   live_list.clear() ;
   for (Bit_iterator bi(live); bi != 0; bi++) {
      int i = *(bi);
      live_list.add_tail(rev_operand_map[i]) ;
   }
}


/* SAM 7-97: changed from using data type to use virtual register file */
bool Liveness_solver::is_interesting(Operand& oper) 
{
   if ((filter_mask & ANALYZE_MEMVR) && oper.is_mem_vr()) return true ;
   if ((filter_mask & ANALYZE_ALLREG) && (oper.is_reg() || oper.is_macro_reg())) {
      if ((filter_mask & ANALYZE_ALLREG) == ANALYZE_ALLREG) return true ;
      Reg_file cur_rfile = oper.file_type() ;
      if ((filter_mask & ANALYZE_INT) && (cur_rfile == GPR)) return true ;
      if ((filter_mask & ANALYZE_FP) && (cur_rfile == FPR)) return true ;
      if ((filter_mask & ANALYZE_BTR) && (cur_rfile == BTR)) return true ;
      if ((filter_mask & ANALYZE_PR) && (cur_rfile == PR)) return true ;
      if ((filter_mask & ANALYZE_CR) && (cur_rfile == CR)) return true ;
   }
   return false ;
}

void Liveness_solver::set_transfer_function(int i) 
{
  Region* r = graph.b_map[i] ;

  //
  // A hyperblock with N exit ops is represented as a tree
  // with N+1 nodes.
  //
  // One node is the root of the tree which accouts for the
  // entry to the hyperblock. The handle of this node will be the
  // corresponding HB*
  //
  // There are N nodes for N exit ops, the handles on these nodes are
  // the exit ops.
  //
  // 1) If graph.b_map[i] is a hyperblock then it is the entry node with
  //    empty transfer function.
  // 2) If it is an op then it is the node corresponding the path from
  //    the hyperblock entry to the hyperblock exit (for the hyperblock
  //    it sits in)
  
  if (r->is_hb()) return;

  Compound_region* blk;
  Op* exit_op;

  if (r->is_bb()) {
    blk = (Compound_region*) r;
    Region_exit_ops exit(blk);
    exit_op = *exit;
  } else {
    assert (r->is_op());
    blk = r->parent();
    while (!blk->is_hb()) blk = blk->parent() ;
    exit_op = (Op*) r;
  }

  liveness_gen_kill (blk, exit_op, this, i);
}

void Liveness_solver::liveness_transfer_function(int n, bool& tag) {

   Liveness_solver_data &ld = live_info[n] ;
   
   Bitvector old_live_in ;

   if (tag == false) old_live_in = ld.live_in ;
   
   ld.live_in = ld.live_out ;
   ld.live_in -= ld.liveness_kill ;
   ld.live_in += ld.liveness_gen ;

   if (tag == false) tag = (ld.live_in != old_live_in) ;
   
   return ;
}

void Liveness_solver::liveness_meet_function(int n, bool& tag) {
   Liveness_solver_data &ld = live_info[n] ;

   Bitvector old_live_out ;

   if (tag == false) {
      old_live_out = ld.live_out ;
   }
   
   ld.live_out = ld.live_out_initial ;
   for (List_iterator<int> li(graph.succ_list[n]) ; li != 0 ; li++) {
      ld.live_out += live_info[*li].live_in ;
   }

   if (tag == false) tag = (ld.live_out != old_live_out) ;
   
   return ;
}

void Liveness_solver::down_exposed_uses_transfer_function(int n, bool& tag) {

   Liveness_solver_data &ld = live_info[n] ;
   
   Bitvector old_down_exposed_use_out ;

   if (tag == false) old_down_exposed_use_out = ld.down_exposed_use_out ;
   
   ld.down_exposed_use_out = ld.down_exposed_use_in ;
   ld.down_exposed_use_out -= ld.down_exposed_use_kill ;
   ld.down_exposed_use_out += ld.down_exposed_use_gen ;

   if (tag == false) {
      tag = (ld.down_exposed_use_out != old_down_exposed_use_out) ;
   }
   
   return ;
}

void Liveness_solver::down_exposed_uses_meet_function(int n, bool& tag) {
   Liveness_solver_data &ld = live_info[n] ;

   Bitvector old_down_exposed_use_in ;

   if (tag == false) {
      old_down_exposed_use_in = ld.down_exposed_use_in ;
   }
   
   ld.down_exposed_use_in = ld.down_exposed_use_in_initial ;
   for (List_iterator<int> li(graph.pred_list[n]) ; li != 0 ; li++) {
      ld.down_exposed_use_in += live_info[*li].down_exposed_use_out ;
   }

   if (tag == false) {
      tag = (ld.down_exposed_use_in != old_down_exposed_use_in) ;
   }
   
   return ;
}


void Liveness_solver::up_exposed_defs_transfer_function(int n, bool& tag) {

   Liveness_solver_data &ld = live_info[n] ;
   
   Bitvector old_up_exposed_def_in ;

   if (tag == false) old_up_exposed_def_in = ld.up_exposed_def_in ;
   
   ld.up_exposed_def_in = ld.up_exposed_def_out ;
   ld.up_exposed_def_in -= ld.up_exposed_def_kill ;
   ld.up_exposed_def_in += ld.up_exposed_def_gen ;

   if (tag == false) {
      tag = (ld.up_exposed_def_in != old_up_exposed_def_in) ;
   }
   
   return ;
}

void Liveness_solver::up_exposed_defs_meet_function(int n, bool& tag) {
   Liveness_solver_data &ld = live_info[n] ;

   Bitvector old_up_exposed_def_out ;

   if (tag == false) {
      old_up_exposed_def_out = ld.up_exposed_def_out ;
   }
   
   ld.up_exposed_def_out = ld.up_exposed_def_out_initial ;
   for (List_iterator<int> li(graph.succ_list[n]) ; li != 0 ; li++) {
      ld.up_exposed_def_out += live_info[*li].up_exposed_def_in ;
   }

   if (tag == false) {
      tag = (ld.up_exposed_def_out != old_up_exposed_def_out) ;
   }
   
   return ;
}

void Liveness_solver::down_exposed_defs_transfer_function(int n, bool& tag) {

   Liveness_solver_data &ld = live_info[n] ;
   
   Bitvector old_down_exposed_def_out ;

   if (tag == false) old_down_exposed_def_out = ld.down_exposed_def_out ;
   
   ld.down_exposed_def_out = ld.down_exposed_def_in ;
   ld.down_exposed_def_out -= ld.down_exposed_def_kill ;
   ld.down_exposed_def_out += ld.down_exposed_def_gen ;

   if (tag == false) {
      tag = (ld.down_exposed_def_out != old_down_exposed_def_out) ;
   }
   
   return ;
}

void Liveness_solver::down_exposed_defs_meet_function(int n, bool& tag) {
   Liveness_solver_data &ld = live_info[n] ;

   Bitvector old_down_exposed_def_in ;

   if (tag == false) {
      old_down_exposed_def_in = ld.down_exposed_def_in ;
   }
   
   ld.down_exposed_def_in = ld.down_exposed_def_in_initial ;
   for (List_iterator<int> li(graph.pred_list[n]) ; li != 0 ; li++) {
      ld.down_exposed_def_in += live_info[*li].down_exposed_def_out ;
   }

   if (tag == false) {
      tag = (ld.down_exposed_def_in != old_down_exposed_def_in) ;
   }
   
   return ;
}

int Liveness_solver::get_region_id(Region* r)
{
  if (r->is_bb()) {
    return graph.f_map.value(r);
  } else if (r->is_hb()) {
    Region_exit_ops exit(r);
    Op* exit_op = *exit;
    exit++;
    if (exit != 0) {
      El_punt("Liveness_solver::get_region_id(): should only call this function on "
              "single-exit loop bodies!");
    }
    return graph.f_map.value(exit_op);
  } else {
    El_punt("Liveness_solver::get_region_id(): region must be bb or hb.");
    return -1;
  }
}

void Liveness_solver::get_liveness_gen(List<Operand>& liveness_gen, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].liveness_gen;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      liveness_gen.add_tail(rev_operand_map[i]);
   }
}

void Liveness_solver::get_liveness_kill(List<Operand>& liveness_kill, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].liveness_kill;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      liveness_kill.add_tail(rev_operand_map[i]);
   }
}

void Liveness_solver::get_down_exposed_use_gen(List<Operand>& down_exposed_use_gen, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].down_exposed_use_gen;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      down_exposed_use_gen.add_tail(rev_operand_map[i]);
   }
}

void Liveness_solver::get_down_exposed_use_kill(List<Operand>& down_exposed_use_kill, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].down_exposed_use_kill;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      down_exposed_use_kill.add_tail(rev_operand_map[i]);
   }
}

void Liveness_solver::get_up_exposed_def_gen(List<Operand>& up_exposed_def_gen, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].up_exposed_def_gen;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      up_exposed_def_gen.add_tail(rev_operand_map[i]);
   }
}

void Liveness_solver::get_up_exposed_def_kill(List<Operand>& up_exposed_def_kill, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].up_exposed_def_kill;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      up_exposed_def_kill.add_tail(rev_operand_map[i]);
   }
}

void Liveness_solver::get_down_exposed_def_gen(List<Operand>& down_exposed_def_gen, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].down_exposed_def_gen;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      down_exposed_def_gen.add_tail(rev_operand_map[i]);
   }
}

void Liveness_solver::get_down_exposed_def_kill(List<Operand>& down_exposed_def_kill, Region* reg)
{
   int reg_id = get_region_id(reg);
   Bitvector& bv = live_info[reg_id].down_exposed_def_kill;

   for (Bit_iterator bi(bv); bi != 0; bi++) {
      int i = *(bi);
      down_exposed_def_kill.add_tail(rev_operand_map[i]);
   }
}

// end of flow_analysis_solver.cpp

////////////////////////////////////////////////////////////////
//
// el_flow_compute_liveness: computes the liveness and
//                           prints it to graphs//
//
// el_flow_compute_four_dataflow_sets: computes and prints
//                                     everything
//
// el_flow_compute_three_dataflow_sets: computes and prints 
//                                     everything except liveness
//
////////////////////////////////////////////////////////////////

void el_flow_compute_liveness(Compound_region* r)
{
   el_flow_compute_liveness(r, ANALYZE_ALL) ;
}

void el_flow_compute_liveness(Compound_region* r, unsigned int flag)
{
   if (El_do_run_time)
     El_clock2.start();

   Liveness_solver s((Region*) r, flag) ;
   s.solve_liveness() ;
   s.print_liveness_to_graph() ;

   if (El_do_run_time) {
      El_clock2.stop();
      El_stopwatch_print_time("el_flow_compute_liveness",
				El_clock2.last_elapsed());
   }
}

void el_flow_delete_liveness(Compound_region* r)
{
   for (Region_all_internal_edges ei(r) ; ei != 0 ; ei++) {
      Edge* e_ptr = *ei ;
      if (is_CONTROL0(e_ptr)) {
	 remove_liveness_info(e_ptr)  ;
      }
   }
}

void el_flow_compute_four_dataflow_sets(Compound_region* r)
{
   el_flow_compute_four_dataflow_sets(r, ANALYZE_ALL) ;
}

void el_flow_compute_four_dataflow_sets(Compound_region* r, unsigned int flag)
{
   if (El_do_run_time)
     El_clock2.start();

   Liveness_solver s((Region*) r, flag) ;
   s.solve_liveness() ;
   s.solve_down_exposed_uses() ;   
   s.solve_up_exposed_defs() ;   
   s.solve_down_exposed_defs() ;
   
   s.print_liveness_to_graph() ;
   s.print_down_exposed_uses_to_graph() ;
   s.print_up_exposed_defs_to_graph() ;
   s.print_down_exposed_defs_to_graph() ;   

   if (El_do_run_time) {
      El_clock2.stop();
      El_stopwatch_print_time("el_flow_compute_four_dataflow_sets",
				El_clock2.last_elapsed());
   }
}

void el_flow_delete_four_dataflow_sets(Compound_region* r)
{
   for (Region_all_internal_edges ei(r) ; ei != 0 ; ei++) {
      Edge* e_ptr = *ei ;
      if (is_CONTROL0(e_ptr)) {
	 remove_liveness_info(e_ptr) ;
	 remove_down_exposed_uses_info(e_ptr)  ;
	 remove_up_exposed_defs_info(e_ptr) ;
	 remove_down_exposed_defs_info(e_ptr) ;
      }
   }
}

void el_flow_compute_three_dataflow_sets(Compound_region* r)
{
   el_flow_compute_three_dataflow_sets(r, ANALYZE_ALL) ;
}

void el_flow_compute_three_dataflow_sets(Compound_region* r, unsigned int flag)
{
   if (El_do_run_time)
     El_clock2.start();

   Liveness_solver s((Region*) r, flag) ;
   s.solve_down_exposed_uses() ;   
   s.solve_up_exposed_defs() ;   
   s.solve_down_exposed_defs() ;
   
   s.print_down_exposed_uses_to_graph() ;
   s.print_up_exposed_defs_to_graph() ;
   s.print_down_exposed_defs_to_graph() ;   

   if (El_do_run_time) {
      El_clock2.stop();
      El_stopwatch_print_time("el_flow_compute_three_dataflow_sets",
				El_clock2.last_elapsed());
   }
}


void el_flow_delete_three_dataflow_sets(Compound_region* r)
{
   for (Region_all_internal_edges ei(r) ; ei != 0 ; ei++) {
      Edge* e_ptr = *ei ;
      if (is_CONTROL0(e_ptr)) {
	 remove_down_exposed_uses_info(e_ptr)  ;
	 remove_up_exposed_defs_info(e_ptr) ;
	 remove_down_exposed_defs_info(e_ptr) ;
      }
   }
}

void el_flow_compute_four_dataflow_sets(Compound_region* r, unsigned int flag,
                                        List<Operand>& liveness_gen,
                                        List<Operand>& liveness_kill,
                                        List<Operand>& down_exposed_uses_gen,
                                        List<Operand>& down_exposed_uses_kill,
                                        List<Operand>& up_exposed_defs_gen,
                                        List<Operand>& up_exposed_defs_kill,
                                        List<Operand>& down_exposed_defs_gen,
                                        List<Operand>& down_exposed_defs_kill
                                        )
{
   liveness_gen.clear();
   liveness_kill.clear();
   down_exposed_uses_gen.clear();
   down_exposed_uses_kill.clear();
   up_exposed_defs_gen.clear();
   up_exposed_defs_kill.clear();
   down_exposed_defs_gen.clear();
   down_exposed_defs_kill.clear();

   Liveness_solver s((Region*) r, flag) ;
   s.solve_liveness() ;
   s.solve_down_exposed_uses() ;   
   s.solve_up_exposed_defs() ;   
   s.solve_down_exposed_defs() ;

   s.get_liveness_gen(liveness_gen, r);
   s.get_liveness_kill(liveness_kill, r);
   s.get_down_exposed_use_gen(down_exposed_uses_gen, r);
   s.get_down_exposed_use_kill(down_exposed_uses_kill, r);
   s.get_up_exposed_def_gen(up_exposed_defs_gen, r);
   s.get_up_exposed_def_kill(up_exposed_defs_kill, r);
   s.get_down_exposed_def_gen(down_exposed_defs_gen, r);
   s.get_down_exposed_def_kill(down_exposed_defs_kill, r);
}
