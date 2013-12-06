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
//      File:           el_memory_dependence.cpp
//      Authors:        Sadun Anik
//      Created:        July 1994
//      Description:    Memory dependence analysis modules
//
/////////////////////////////////////////////////////////////////////////////

#include "el_memory_dependence.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "op.h"

Alias_type alias_meet(Alias_type a1, Alias_type a2) {
   if (( a1 == NO_ALIAS_INFORMATION ) || ( a2 == NO_ALIAS_INFORMATION )) 
      return NO_ALIAS_INFORMATION ;
   if (( a1 == MAY_ALIAS ) || ( a2 == MAY_ALIAS )) 
      return MAY_ALIAS ;
   switch(a1) {
    case DEFINITE_ALIAS:
      if ((a2 == DEFINITE_ALIAS) ||(a2 == EQUIVALENCE_ALIAS))
	 return DEFINITE_ALIAS ;
      else return MAY_ALIAS ;
      break;
    case DEFINITE_NO_ALIAS:
      if (a2 == DEFINITE_NO_ALIAS) return DEFINITE_NO_ALIAS ;
      else return MAY_ALIAS ;
      break;
    case EQUIVALENCE_ALIAS:
      if (a2 == EQUIVALENCE_ALIAS) return  EQUIVALENCE_ALIAS ;
      else if (a2 == DEFINITE_ALIAS) return DEFINITE_ALIAS ;
      else return MAY_ALIAS ;
      break;
    default:
      assert(0) ;
      return NO_ALIAS_INFORMATION ;
   }
}

/////////////////////////////////////////////////////////////////////////////

Alias_query_system::Alias_query_system()
{}

Alias_query_system::Alias_query_system(Region* r)
{}

Alias_query_system::~Alias_query_system()
{}

Alias_type Alias_query_system::get_alias(Op* op1, Op* op2, Edge* e)
{
//   set1 = get_reaching_locs(op1,e) ;
//   set2 = get_upexposed_locs(op2, e) ;
//   if (set1.size() == 1) && (set1 == set2) then return 
   return MAY_ALIAS ;
}

/////////////////////////////////////////////////////////////////////////////


void el_lcode_reader_fix_memvar_uses(Region* r)
{
   Alist_region_tiling_graph g(r, GRAPH_OPS) ;
   Vector<Bitvector> reaching_mem_in ;
   Vector<Bitvector> reaching_mem_out ;

   int num_of_ops = g.b_map.size() ;
   Bitvector   load_ops(num_of_ops) ;
   Bitvector   store_ops(num_of_ops) ;
   Bitvector   merge_ops(num_of_ops) ;
   Bitvector   transparent_ops(num_of_ops) ;
   reaching_mem_in.resize(num_of_ops, load_ops) ;
   reaching_mem_out.resize(num_of_ops, load_ops) ;
   
//
// Form an op graph for the procedure
// Register the load class and store class operations in
// respective bit vectors
//
   Alist_graph_dfs all_ops(g) ;
   for (all_ops.reset() ; all_ops != 0 ; all_ops++) {
      int opi = *all_ops ; 
      Op* tmp_op = (Op*) g.b_map[opi] ;
//      if (is_memory(tmp_op) || is_brl(tmp_op) || is_rts(tmp_op) ||
//	 (tmp_op->flag(EL_OPER_REAL_MERGE))) {
      if (is_memory(tmp_op) || is_brl(tmp_op) || is_rts(tmp_op)) {
	 // classify load operations to LOAD class and
	 // stores, procedure calls, and returns  to STORE class
	 if (is_load(tmp_op)) {
	    load_ops.set_bit(opi) ;
	 }
	 else {
	    store_ops.set_bit(opi) ;
	 }
      }
      else if (tmp_op->flag(EL_OPER_REAL_MERGE)) {
	 merge_ops.set_bit(opi) ;
      }
      else {
	 transparent_ops.set_bit(opi) ;
      }
   }

   bool not_done = true ;
   int iteration_count = 0  ;
   Alist_graph_post_dfs all_ops_pdfs(g,UP) ;

//
// Computer reaching definitions of mem_vrs with the following rules
//

   while(not_done) {
      iteration_count++ ;
      not_done = false ;
      Bitvector old_rmi ;
      Bitvector old_rmo ;
      for (all_ops_pdfs.reset() ; all_ops_pdfs != 0 ; all_ops_pdfs++) {
	 int opi = *all_ops_pdfs ;
	 if (transparent_ops.bit(opi)) {
	    List_iterator<int> li(g.pred_list[opi]) ;
	    if (li != 0) {
	       reaching_mem_out[opi] = reaching_mem_out[*li] ;
	    }
	    continue ;
	 }

	 {
	    // First implement the meet function for reaching memvr
	    // definitions. This is a simple union (OR)
	    
	    Bitvector& rmi = reaching_mem_in[opi] ;

	    if (not_done == false) {
	       old_rmi = rmi ;
	    }
	    List_iterator<int> li(g.pred_list[opi]) ;
	    if (li != 0 ) {
	       rmi =  reaching_mem_out[*li] ;
	       li++ ;
	    }
	    for ( ; li != 0 ; li++) {
	       rmi += reaching_mem_out[*li] ;
	    }
	    if (not_done == false) not_done = (rmi != old_rmi) ;
	 }
	 {
	    // Apply the transfer function
	    // An uncoditional store will kill all previous reaching defs
	    // A conditional store or a load will add themselves to the
	    // list of reaching defs

	    Bitvector& rmo = reaching_mem_out[opi] ;
	    Bitvector& rmi = reaching_mem_in[opi] ;

	    if (not_done == false) {
	       old_rmo = rmo ;
	    }
	    rmo = rmi ;
	    if (load_ops.bit(opi)) {
	       rmo.set_bit(opi) ;
	    }
	    else if (store_ops.bit(opi)) {
// If op is predicated on true kill prev definitions
// Else add yourself to the list of reaching definitons
	       if (!is_conditionally_executed((Op*) g.b_map[opi])) {
		  rmo.clear() ;
	       }
	       rmo.set_bit(opi) ;
	    }
	    if (not_done == false) {
	       not_done = (rmo != old_rmo) ;
	    }
	 }
      }
   }
//
// Reaching defs for loads and stores is done now. For each memory 
// op, determine the reaching definition and set its mem_vr sources
//
   for (all_ops.reset() ; all_ops!= 0 ; all_ops++) {
      int opi = *all_ops ;
      if (load_ops.bit(opi) || store_ops.bit(opi)) { // ignore other ops
	 Op* tmp_op = (Op*) g.b_map[opi] ;
//	 int dest_is_control_merge = tmp_op->flag(EL_OPER_REAL_MERGE) ;
	 Bitvector& rmi = reaching_mem_in[opi] ;
	 for (Bit_iterator ii(rmi) ; ii != 0 ; ii++) { // For all reaching defs
	    int i = *ii ;
	    if (rmi.bit(i)) {    // A definition is reaching
	       if (store_ops.bit(i)) {   // Source is a store class op
		  for (Op_mem_dests mdi(((Op*)g.b_map[i])); mdi != 0 ;mdi++) {
		     tmp_op->add_mem_src(*mdi) ;
		  }
	       }
	       else { // Source is a load class op
//		  if ((dest_is_control_merge == 0) && (store_ops.bit(opi))) { // The destination is a store 
		  if (store_ops.bit(opi)) { // The destination is a store 
		     for (Op_mem_dests mdi(((Op*)g.b_map[i])); mdi != 0 ;mdi++) {
			tmp_op->add_mem_src_unsafe(*mdi) ;
		     }
		  }
	       }
	    }
	 }
      }
   }
}
