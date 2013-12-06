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
//      File:           cluster.cpp
//      Author:         Kevin Fan
//      Created:        June 2001
//      Description:    Perform cluster assignment and move insertion
//
/////////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "list.h"
#include "list_set.h"
#include "hash_functions.h"
#include "hash_map.h"
#include "hash_set.h"
#include "matrix.h"
#include "attributes.h"
#include "edge_utilities.h"
#include "opcode_properties.h"
#include "op.h"
#include "operand.h"
#include "region.h"
#include "cluster.h"
#include "hw_cluster.h"
#include "single_cluster.h"
#include "random_cluster.h"
#include "bug_cluster.h"
#include "coarse_op.h"
#include "multilevel.h"
#include "rhop.h"
#include "iterators.h"
#include "el_control.h"
#include "intf.h"
#include "mdes.h"
#include "flow_analysis_solver.h"
#include "reaching_defs_solver.h"
#include "el_opti_utilities.h"
#include "el_opti_dead_code.h"
#include "pred_analysis.h"
#include "connect.h"
#include "el_codegen_init.h"
#include "dep_len.h"
#include "regfile_bind.h"
#include "el_io.h"
#include "ir_writer.h"
#include "local_copy_propagate.h"

static bool machine_discovered = false;
static bool network_discovered = false;
int Cluster_mgr::moves_inserted = 0;
Macro_name Cluster_mgr::spill_macro;
Vector<HW_cluster*>    Cluster_mgr::hw_cluster_vec;
Hash_map<Op*, int>     Cluster_mgr::op_map(hash_op_ptr, HASH_SIZE_1);
Hash_map<int, int>     Cluster_mgr::operand_map(hash_int, HASH_SIZE_2);
Vector<Macro_info>     Cluster_mgr::macro_vec((int) LAST_MACRO);
Hash_set<eString>      Cluster_mgr::ic_move_opcodes(hash_estring, HASH_SIZE_3);
Hash_map<eString, int> Cluster_mgr::physical_file_to_cluster_map(hash_estring, HASH_SIZE_2);
Matrix<int>            Cluster_mgr::min_latency;
Matrix< List<int>* >   Cluster_mgr::min_path;

Cluster_mgr::Cluster_mgr()
  : weight_map  (hash_op_ptr,  HASH_SIZE_1),
    op_valid_cluster_map (hash_op_ptr, HASH_SIZE_1),
    pre_assign_map(hash_operand, 128)
{
   m_proc = NULL;
   cluster_alg = NULL;
   freqs_found = false;
   op_map.clear();
   operand_map.clear();
   spill_macro = (El_create_stack_frame ? SP_REG : RGS_REG);
}

Cluster_mgr::Cluster_mgr(Procedure* proc)
  : weight_map  (hash_op_ptr,  HASH_SIZE_1),
    op_valid_cluster_map (hash_op_ptr, HASH_SIZE_1),
    pre_assign_map(hash_operand, 128)
{
   m_proc = proc;
   cluster_alg = NULL;
   freqs_found = false;
   op_map.clear();
   operand_map.clear();

   for(int i = 0; i != (int) LAST_MACRO; i++)
     macro_vec[i].clear_refs();

   spill_macro = (El_create_stack_frame ? SP_REG : RGS_REG);
}

Cluster_mgr::~Cluster_mgr()
{
   for (Hash_map_iterator<Op*, List<int>*> hmi(op_valid_cluster_map);
	hmi != 0; hmi++) {
      List<int>* list_ptr = (*hmi).second;
      if (list_ptr != NULL)
	 delete list_ptr;
   }

   if (cluster_alg) delete cluster_alg;
}

// bind a dest to the same cluster as its op. then bind unbound inputs to the
// same cluster as their op.
void Cluster_mgr::do_block_operand_assignment(Compound_region* rgn)
{
   // first bind all unbound dests
   for (Region_all_ops opi(rgn); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;
      if (!op_map.is_bound(cur_op))
	 El_punt("operand assignment: unbound op %d", cur_op->id());
      
      int clstr = op_map.value(cur_op);

      // go through all dests, adding references for macros when necessary
      for (Op_explicit_dests desti(cur_op); desti != 0; desti++) {
	 Operand& dest = *desti;
	 if (dest.is_reg() && !operand_map.is_bound(dest.vr_num()))
	    operand_map.bind(dest.vr_num(), clstr);
	 else if (dest.is_macro_reg()) {
	    int mn = (int)dest.name();
	    El_ref cur_ref(cur_op, &dest, desti.ref_type());
	 
	    if(macro_vec[mn].is_read_only()) {
	      if(macro_vec[mn].is_potential_home(clstr)) 
		macro_vec[mn].add_reference(cur_ref, clstr);
	      else {
		int first_home = macro_vec[mn].get_first_potential_home();
		macro_vec[mn].add_reference(cur_ref, first_home);
	      }
	    }
	    else { // read-write
	      if(!macro_vec[mn].is_bound(cur_ref)) {
		if(macro_vec[mn].is_potential_home(clstr)) 
		  macro_vec[mn].add_reference(cur_ref, clstr);
		else {
		  int first_home = macro_vec[mn].get_first_potential_home();
		  macro_vec[mn].add_reference(cur_ref, first_home);
		}
	      }
	    }
	 }
      }
   }

   // NOTE bind ALL unbounds so we can do bb-at-a-time 8-17-01
   for (Region_all_ops opi2(rgn); opi2 != 0; opi2++) {
      Op* cur_op = *opi2;
      if (is_pseudo(cur_op)) continue;
      //      if (cycle_map.value(cur_op) != 0) continue;

      int clstr = op_map.value(cur_op);

      for (Op_explicit_inputs oi(cur_op); oi != 0; oi++) {
	 Operand& opr = *oi;

	 // for regular registers, only bind a source if it happens on cycle 0
	 if (opr.is_reg() && //cycle_map.value(cur_op) == 0 && 
	     !operand_map.is_bound(opr.vr_num()))
	    operand_map.bind(opr.vr_num(), clstr);

	 // for macros we bind every ref that we can..
	 // if a macro isn't valid in a certain reg file, then postpone binding
	 // until "postpass_operand_check"; hopefully a good location will have
	 // been picked for it by then. if not, it will get a random
	 else if (opr.is_macro_reg()) {
	    int mn = (int)opr.name();
	    El_ref cur_ref(cur_op, &opr, oi.ref_type());

	    if(macro_vec[mn].is_read_only()) {
	      if(macro_vec[mn].is_potential_home(clstr)) 
		macro_vec[mn].add_reference(cur_ref, clstr);
	      else {
		int first_home = macro_vec[mn].get_first_potential_home();
		macro_vec[mn].add_reference(cur_ref, first_home);
	      }
	    }
	    else { // read-write
	      if(!macro_vec[mn].is_bound(cur_ref)) {
		if(macro_vec[mn].is_potential_home(clstr)) 
		  macro_vec[mn].add_reference(cur_ref, clstr);
		else {
		  int first_home = macro_vec[mn].get_first_potential_home();
		  macro_vec[mn].add_reference(cur_ref, first_home);
		}
	      }
	    }
	 }
      }
   }
}

// bind in this order: dests of critical ops; srcs of critical ops;
// other dests; other srcs.
void Cluster_mgr::do_block_smarter_operand_assignment(Compound_region* rgn)
{
  // blah. stupid way.
  Hash_map<Op*, DepLen_OpInfo*> dep_info(hash_op_ptr, HASH_SIZE_1);
  DepLen::get_estart(rgn, dep_info);
  DepLen::get_lstart(rgn, dep_info);
  Hash_set<Op*> critical_ops(hash_op_ptr, HASH_SIZE_1);

  for (Region_all_ops opi2(rgn); opi2!=0; opi2++) {
    Op* op = *opi2;
    if (is_pseudo(op)) continue;
    if (dep_info.value(op)->m_Estart == dep_info.value(op)->min_lstart())
      critical_ops += op;
  }
  
  // cleanup. -KF 1/2005
  DepLen::clear_opinfo_map(rgn, dep_info);

  Region_all_ops opi;
   for (opi(rgn); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;
      if (!critical_ops.is_member(cur_op)) continue;
      if (!op_map.is_bound(cur_op))
	 El_punt("smarter operand assignment: unbound op %d", cur_op->id());
      
      int clstr = op_map.value(cur_op);

      // go through all dests, adding references for macros when necessary
      for (Op_explicit_dests desti(cur_op); desti != 0; desti++) {
	 Operand& dest = *desti;
	 if (dest.is_reg() && !operand_map.is_bound(dest.vr_num()))
	    operand_map.bind(dest.vr_num(), clstr);
	 else if (dest.is_macro_reg()) {
	    int mn = (int)dest.name();
	    El_ref cur_ref(cur_op, &dest, desti.ref_type());
	 
	    if(macro_vec[mn].is_read_only()) {
	      if(macro_vec[mn].is_potential_home(clstr)) 
		macro_vec[mn].add_reference(cur_ref, clstr);
	      else {
		int first_home = macro_vec[mn].get_first_potential_home();
		macro_vec[mn].add_reference(cur_ref, first_home);
	      }
	    }
	    else { // read-write
	      if(!macro_vec[mn].is_bound(cur_ref)) {
		if(macro_vec[mn].is_potential_home(clstr)) 
		  macro_vec[mn].add_reference(cur_ref, clstr);
		else {
		  int first_home = macro_vec[mn].get_first_potential_home();
		  macro_vec[mn].add_reference(cur_ref, first_home);
		}
	      }
	    }
	 }
      }
   }

   for (opi(rgn); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;
      if (!critical_ops.is_member(cur_op)) continue;

      int clstr = op_map.value(cur_op);

      for (Op_explicit_inputs oi(cur_op); oi != 0; oi++) {
	 Operand& opr = *oi;

	 // for regular registers, only bind a source if it happens on cycle 0
	 if (opr.is_reg() && //cycle_map.value(cur_op) == 0 && 
	     !operand_map.is_bound(opr.vr_num()))
	    operand_map.bind(opr.vr_num(), clstr);

	 // for macros we bind every ref that we can..
	 // if a macro isn't valid in a certain reg file, then postpone binding
	 // until "postpass_operand_check"; hopefully a good location will have
	 // been picked for it by then. if not, it will get a random
	 else if (opr.is_macro_reg()) {
	    int mn = (int)opr.name();
	    El_ref cur_ref(cur_op, &opr, oi.ref_type());

	    if(macro_vec[mn].is_read_only()) {
	      if(macro_vec[mn].is_potential_home(clstr)) 
		macro_vec[mn].add_reference(cur_ref, clstr);
	      else {
		int first_home = macro_vec[mn].get_first_potential_home();
		macro_vec[mn].add_reference(cur_ref, first_home);
	      }
	    }
	    else { // read-write
	      if(!macro_vec[mn].is_bound(cur_ref)) {
		if(macro_vec[mn].is_potential_home(clstr)) 
		  macro_vec[mn].add_reference(cur_ref, clstr);
		else {
		  int first_home = macro_vec[mn].get_first_potential_home();
		  macro_vec[mn].add_reference(cur_ref, first_home);
		}
	      }
	    }
	 }
      }
   }

   for (opi(rgn); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;
      if (critical_ops.is_member(cur_op)) continue;

      int clstr = op_map.value(cur_op);

      // go through all dests, adding references for macros when necessary
      for (Op_explicit_dests desti(cur_op); desti != 0; desti++) {
	 Operand& dest = *desti;
	 if (dest.is_reg() && !operand_map.is_bound(dest.vr_num()))
	    operand_map.bind(dest.vr_num(), clstr);
	 else if (dest.is_macro_reg()) {
	    int mn = (int)dest.name();
	    El_ref cur_ref(cur_op, &dest, desti.ref_type());
	 
	    if(macro_vec[mn].is_read_only()) {
	      if(macro_vec[mn].is_potential_home(clstr)) 
		macro_vec[mn].add_reference(cur_ref, clstr);
	      else {
		int first_home = macro_vec[mn].get_first_potential_home();
		macro_vec[mn].add_reference(cur_ref, first_home);
	      }
	    }
	    else { // read-write
	      if(!macro_vec[mn].is_bound(cur_ref)) {
		if(macro_vec[mn].is_potential_home(clstr)) 
		  macro_vec[mn].add_reference(cur_ref, clstr);
		else {
		  int first_home = macro_vec[mn].get_first_potential_home();
		  macro_vec[mn].add_reference(cur_ref, first_home);
		}
	      }
	    }
	 }
      }
   }

   for (opi(rgn); opi != 0; opi++) {
      Op* cur_op = *opi;
      if (is_pseudo(cur_op)) continue;
      if (critical_ops.is_member(cur_op)) continue;

      int clstr = op_map.value(cur_op);

      for (Op_explicit_inputs oi(cur_op); oi != 0; oi++) {
	 Operand& opr = *oi;

	 // for regular registers, only bind a source if it happens on cycle 0
	 if (opr.is_reg() && //cycle_map.value(cur_op) == 0 && 
	     !operand_map.is_bound(opr.vr_num()))
	    operand_map.bind(opr.vr_num(), clstr);

	 // for macros we bind every ref that we can..
	 // if a macro isn't valid in a certain reg file, then postpone binding
	 // until "postpass_operand_check"; hopefully a good location will have
	 // been picked for it by then. if not, it will get a random
	 else if (opr.is_macro_reg()) {
	    int mn = (int)opr.name();
	    El_ref cur_ref(cur_op, &opr, oi.ref_type());

	    if(macro_vec[mn].is_read_only()) {
	      if(macro_vec[mn].is_potential_home(clstr)) 
		macro_vec[mn].add_reference(cur_ref, clstr);
	      else {
		int first_home = macro_vec[mn].get_first_potential_home();
		macro_vec[mn].add_reference(cur_ref, first_home);
	      }
	    }
	    else { // read-write
	      if(!macro_vec[mn].is_bound(cur_ref)) {
		if(macro_vec[mn].is_potential_home(clstr)) 
		  macro_vec[mn].add_reference(cur_ref, clstr);
		else {
		  int first_home = macro_vec[mn].get_first_potential_home();
		  macro_vec[mn].add_reference(cur_ref, first_home);
		}
	      }
	    }
	 }
      }
   }
}

// do clustering on a block
void Cluster_mgr::do_block_clustering(Compound_region* block, Cluster_method cm)
{
   if (dbg(status,2))
     cdbg << "Performing prepass clustering on region: " << block->id() 
          << " of procedure " << (char*)(el_get_enclosing_procedure(block)->get_name()) << endl;

   discover_machine();
   check_macros(block);

   discover_network();

   // print_macro_vec();

   // Commented out clearing of op_map; the op to cluster information for earlier region
   // is required by codegen. This should never be cleared in the first place. Rajiv
   // op_map.clear(); // fix problems with printing out hash map that has old ops

   if (cluster_alg) delete cluster_alg;

   if (cm == EL_CLSTR_NONE) {
     cluster_alg = new Single_cluster(this);
   } else if (cm == EL_CLSTR_BUG) {
     cluster_alg = new BUG_cluster(this);
   } else if (cm == EL_CLSTR_ML) {
     cluster_alg = new MultiLevel(this);
   } else if (cm == EL_CLSTR_RHOP) {
     cluster_alg = new RHOP(this);
   } else if (cm == EL_CLSTR_RANDOM) {
     cluster_alg = new Random_cluster (this);
   } else {
     El_punt("Unsupported cluster method.");
   }

   if (dbg(clst, 1))
     cdbg << "doing clustering on block " << block->id() << "..." << endl;
   cluster_alg->do_block_setup_and_clustering(block);

   if (dbg(clst, 1))
      cdbg << "inserting moves..." << endl;
   insert_block_moves(block);

   if (dbg(clst, 4)) {
      print_op_clusters();
      print_operand_clusters();
   }

   if (dbg(clst, 1))
      cdbg << moves_inserted << " total moves inserted so far." << endl;

   if (moves_inserted != 0) {
     bool optis_found = true;
     for (int pass = 0; pass < El_move_opti_passes && optis_found; pass++) {
       if (dbg(clst, 1))
         cdbg << "optimizing moves (pass " << pass << ")..." << endl;
       int num_optis = do_block_move_opti(block);
       if (dbg(clst, 1) && num_optis > 0)
         cdbg << num_optis << " move optis done." << endl;
       optis_found = (num_optis > 0);
     }
   }

   // Hongtao 7-03 - add cluster id attributes to all ops in block
   install_cluster_id_attr(block);
}

// This function was only written to work pre-register allocation. It
// MAY work post-allocation, but it wasn't designed to, and it hasn't
// been tested.
void Cluster_mgr::undo_block_clustering(Compound_region* r)
{
  ////////////////////////////////////////////////////////////////////////////
  // First, unbind all registers from physical files. Physical files
  // are cluster specific, so they must be undone.

  for(Region_all_ops rops(r); rops != 0; rops++) {
    Op* op = *rops;
    for(Op_all_inputs ins(op); ins != 0; ins++) {
      Operand& oper = *ins;
      if(oper.is_reg() || oper.is_macro_reg()) {
	oper.unbind_physical_file();
      }
    }
    for(Op_all_dests outs(op); outs != 0; outs++) {
      Operand& oper = *outs;
      if(oper.is_reg() || oper.is_macro_reg()) {
	oper.unbind_physical_file();
      }
    }
    remove_prf_binding_attr(op);
  }


  ////////////////////////////////////////////////////////////////////////////
  // Now remove all the intercluster moves, and adjust the register
  // names appropriately. Copy propogation should work now that
  // physical register files are unbound.

  local_copy_propagation(r);


  ////////////////////////////////////////////////////////////////////////////
  // Note that this function doesn't reset the state of the
  // cluster_manager. I consulted the clustering experts around UMich,
  // and the consensus was that "that's hard." Instead, it's assumed
  // that in order to redo clustering, it's necessary to create a copy
  // of the cluster manager before clustering, and then restore it
  // before redoing the clustering.

}

void Cluster_mgr::install_cluster_id_attr(Compound_region *r)
{
   for (Region_all_ops opi(r); opi!=0; opi++) {
      Op *op = *opi;
      int cid = -1;
      if (op->flag(EL_OPER_ICMOVE))
	 cid = -1;
      else if (op_map.is_bound(op))
	 cid = op_map.value(op);
      else if (is_pseudo(op))
         continue;
      else
         El_punt("Cluster_mgr::install_cluster_id_attr: op %d has no cluster id",
			 op->id());
      Cluster_id_attr *attr = new Cluster_id_attr();
      attr->cluster_id = cid;
      set_cluster_id_attr(op, attr);
   }
}

// per block
void Cluster_mgr::insert_block_moves(Compound_region* block)
{
   if (! block->is_hb() && ! block->is_bb())
      El_punt("insert_block_moves: region %d not bb or hb", block->id());
   
   int operand_cid = -1;
   bool is_macro;
   Macro_name mn = UNDEFINED;
   El_ref macro_ref;

   for (Region_all_ops opi(block); opi != 0; opi++) {
      Op* op = *opi;
      if (is_pseudo(op) || op->flag(EL_OPER_ICMOVE))
	 continue;

      if (!op_map.is_bound(op))
	 El_punt("insert_block_moves: op %d is not bound to a cluster", op->id());

      int op_cid = op_map.value(op);

      // check source operands
      for (Op_explicit_inputs inputi(op); inputi != 0; inputi++) {
	 Operand& src = *inputi;
	    
	 is_macro = false;

	 if (src.is_reg()) {
	    if (operand_map.is_bound(src.vr_num()))
	       operand_cid = operand_map.value(src.vr_num());
	    else
	       El_punt("insert_block_moves: cluster id not found for source in op %d", op->id());
	 }
	 else if (src.is_macro_reg()) {
	    macro_ref = inputi.get_ref();
	    is_macro = true;

	    mn = src.name();

	    if (dbg(clst, 4))	    
	      cdbg << "In insert_block_moves SRC Macro: " << mn << endl;

	    if(macro_vec[mn].is_bound(macro_ref))
	      operand_cid = macro_vec[mn].get_binding(macro_ref);
	    else
              El_punt("insert_block_moves: cluster id not found for macro ref in op %d", op->id());
	 }
	 else
	    continue;

	 // if they are on different clusters ..
	 if (operand_cid != op_cid) {
	    Port_num pnum = inputi.get_port_num();
	    Operand opr_temp;
            List<Op*> move_ops;
            get_src_moves_for_operand(op, pnum, operand_cid, op_cid, move_ops);

 	    // also need to move the guard for the move over.
// 	    if (pnum != PRED1 && 
// 		op->predicated() && !op->src(PRED1).is_lit()) {
                 
// 	       // check current binding of the predicate
//   	       int pred_cid = -1;
//  	       if (operand_map.is_bound(op->src(PRED1)))
// 	         pred_cid = operand_map.value(op->src(PRED1));
// 	       else
// 		 El_punt("insert_block_moves: cluster id not found for predicate in op %d", op->id());
            // Above code assumes that get_moves_for_operand predicates moves with the same
            // guard as that of the original op.  This is no longer true; have to explicitly
            // check each move's guard (inside for loop below).  -KF 1/2006

            if (pnum != PRED1) {
	       // if the bindings of the operand and the predicate are
               // different, predicate move is necessary.
               // note: predicate moves could be inserted more intelligently.
               // but for now just insert all needed moves independently, and assume
               // that move opti can remove redundant moves.
               bool first_opnd = true;
               for (List_iterator<Op*> li(move_ops); li!=0; li++) {
                 Op* move_op = *li;

                 // get cluster of operand being moved
                 int cur_cid;
                 if (first_opnd) {
                   cur_cid = operand_cid;
                   first_opnd = false;
                 } else
                   cur_cid = operand_map.value(move_op->src(SRC1).vr_num());

                 if (move_op->src(PRED1).is_lit())
                   continue;

                 // get cluster of guarding predicate
                 int pred_cid = -1;
                 if (operand_map.is_bound(move_op->src(PRED1).vr_num()))
                   pred_cid = operand_map.value(move_op->src(PRED1).vr_num());
                 else {
                   cdbg << *move_op;
                   cdbg << *op;
                   El_punt("insert_block_moves: cluster id not found for predicate of move op %d",
                           move_op->id());
                 }

                 // insert move if necessary
                 if (pred_cid != cur_cid) {
                   List<Op*> move_pp_ops;
                   get_src_moves_for_operand(move_op, PRED1, pred_cid, cur_cid, move_pp_ops);
                   for (List_iterator<Op*> li2(move_pp_ops); li2!=0; li2++) {
                     El_insert_op_before(*li2, op);
                     moves_inserted++;
                   }
                 }
               }
	    }

	    if (is_macro) {
              Op* move_op = move_ops.head();
              El_ref new_ref(move_op, &move_op->src(SRC1), EXP_SRC);

              macro_vec[mn].remove_reference(macro_ref);
              macro_vec[mn].add_reference(new_ref, operand_cid);

	    }
            for (List_iterator<Op*> li(move_ops); li!=0; li++) {
              El_insert_op_before(*li, op);
              moves_inserted++;
            }
            if (dbg(clst, 4))
              cdbg << "move src " << src << " from "<<operand_cid<<" to "<<op_cid<<endl;
	 }
      }
	 
      // check destination operands
      for (Op_explicit_dests desti(op); desti != 0; desti++) {
	 Operand& dest = *desti;
	    
	 is_macro = false;
	 if (dest.is_reg()) {
	    if (operand_map.is_bound(dest.vr_num()))
	       operand_cid = operand_map.value(dest.vr_num());
	    else
	       El_punt("insert_block_moves: cluster id not found for dest in op %d", op->id());
	 }
	 else if (dest.is_macro_reg()) {
	    macro_ref = desti.get_ref();
	    is_macro = true;

	    mn = dest.name();
	    if (dbg(clst, 4))
	      cdbg << "In insert_block_moves DEST Macro: " << mn << endl;

	    if(macro_vec[mn].is_bound(macro_ref))
	      operand_cid = macro_vec[mn].get_binding(macro_ref);
	    else
	       El_punt("insert_block_moves: cluster id not found for macro ref in op %d", op->id());	 

	 }
	 else
	    continue;

	 // if they are on different clusters ..
	 if (operand_cid != op_cid) {
	    Port_num pnum = desti.get_port_num();
	    Operand opr_temp;
            List<Op*> move_ops;
            get_dest_moves_for_operand(op, pnum, op_cid, operand_cid, move_ops);
            
	    if (is_macro) {
              Op* move_op = move_ops.tail();
              El_ref new_ref(move_op, &move_op->dest(DEST1), EXP_DEST);

              macro_vec[mn].remove_reference(macro_ref);
              macro_vec[mn].add_reference(new_ref, operand_cid);	    
	    }
            Op* prev_op = op;
            for (List_iterator<Op*> li(move_ops); li!=0; li++) {
              El_insert_op_after(*li, prev_op);
              prev_op = *li;
              moves_inserted++;
            }
            if (dbg(clst, 4))
              cdbg << "move dest "<<dest<<" from "<<op_cid<<" to "<<operand_cid<<endl;
            
            // need to insert guards for moves; with grid architectures they are no longer
            // guaranteed to be on the same cluster as the srcs of moves.
            // but don't change guards for moves of A-type and O-type cmpp dests because they
            // are weird. (and btw, ic A-type moves have been converted to O-type by this point,
            // so don't need to check for those.) -KF 10/2004
	    if (op->predicated() && !op->src(PRED1).is_lit() &&
                !is_O_pred(op, pnum)) {
              // check current binding of the predicate
              int pred_cid = -1;
              if (operand_map.is_bound(op->src(PRED1).vr_num()))
                pred_cid = operand_map.value(op->src(PRED1).vr_num());
              else
                El_punt("insert_block_moves: cluster id not found for predicate in op %d", op->id());

              // if the bindings of the operand and the predicate are different,
              // predicate move is necessary
              for (List_iterator<Op*> li(move_ops); li!=0; li++) {
                Op* move_op = *li;
                int cur_cid;
                cur_cid = operand_map.value(move_op->src(SRC1).vr_num());
                if (pred_cid != cur_cid) {
                  List<Op*> move_pp_ops;
                  get_src_moves_for_operand(move_op, PRED1, pred_cid, cur_cid, move_pp_ops);
                  for (List_iterator<Op*> li2(move_pp_ops); li2!=0; li2++) {
                    El_insert_op_before(*li2, move_op);
                    moves_inserted++;
                  }
                }
              }
            }

	    // check if a pred_clear is needed for or-type cmpps
            // (and and-type cmpps that have been converted to or-types)
	    if (is_O_pred(op, pnum)) {
	       Op* pred_clear_op = get_pred_clear_op(op->dest(pnum));
	       op_map.bind(pred_clear_op, op_cid);
	       El_insert_op_after_merge(block, pred_clear_op);
	    } else if (is_movepp(op)) {
              // parkhc 10/4/2005
              // check if a pred_clear is needed for movepp operations
              // This is the case when guarding predicate is non-literal
              Operand guard = op->src(PRED1);
              if (!guard.is_lit()) {
                Op* pred_clear_op = get_pred_clear_op(op->dest(DEST1));
                op_map.bind(pred_clear_op, op_cid);
                El_insert_op_after_merge(block, pred_clear_op);
              }
            }
	 }
      }
   }
}

// Returns a list of move ops and changes the src of op to use the new temporary
// operand.
void Cluster_mgr::get_src_moves_for_operand(Op* op, Port_num pnum, int from_cid,
                                            int to_cid, List<Op*>& move_ops)
{
  Operand opnd_to_move = op->src(pnum);
  if (opnd_to_move.is_lit())
    return;

  int from_idx = hw_cluster_vec[from_cid]->get_physical_file_id(opnd_to_move.file_type());
  int to_idx = hw_cluster_vec[to_cid]->get_physical_file_id(opnd_to_move.file_type());
  if (opnd_to_move.file_type() == CR)
    to_idx = hw_cluster_vec[to_cid]->get_physical_file_id(GPR);

  List<int>* move_list = min_path[from_idx][to_idx];
  if (move_list == NULL || move_list->size() == 0)
    El_punt("get_src_moves_for_operand: unable to move op %d SRC %d from cluster %d to cluster %d",
            op->id(), pnum, from_cid, to_cid);

  for (List_iterator<int> li(*move_list); li!=0; li++) {
    int cur_dest_cid = *li;
    Opcode opcode = get_icmove_opcode(opnd_to_move, true);
    Op* newop = new Op(opcode);
    newop->set_flag(EL_OPER_ICMOVE);
    Operand opnd_temp(new Reg(opnd_to_move.dtype()));  // new temp operand
    operand_map.bind(opnd_temp.vr_num(), cur_dest_cid);

     // set source and destination operands of new move op
    if (!op->predicated() || pnum == PRED1) {
      Operand pred_true(new Pred_lit(true));
      newop->set_src(PRED1, pred_true);
    } else {
      // guard may be overwritten in insert_block_moves

      // SAM 12-05: New code, predicate the move on True as you know this will be legal
      // guard the move by true can reduce an icmove for the predicate

      // KF 1/2006: Addendum: in a swp loop, use the staging predicate rather
      // than true, otherwise the rr allocator will do the wrong thing.
      if (op->parent()->flag(EL_REGION_SOFTPIPE)) {
        Operand staging_pred = get_staging_predicate(op->parent());
        newop->set_src(PRED1, staging_pred);
      } else {
        Operand pred_true(new Pred_lit(true));
        newop->set_src(PRED1, pred_true);
      }
    }
    newop->set_src(SRC1, opnd_to_move);
    newop->set_dest(DEST1, opnd_temp);

    move_ops.add_tail(newop);
    opnd_to_move = opnd_temp;
  }
  op->set_src(pnum, opnd_to_move);
}

// Returns a list of move ops and also changes the dest of op to the new
// temporary operand.
void Cluster_mgr::get_dest_moves_for_operand(Op* op, Port_num pnum, int from_cid,
                                             int to_cid, List<Op*>& move_ops)
{
  Operand dest_opnd = op->dest(pnum);
  if (dest_opnd.is_lit())
    return;

  int from_idx = hw_cluster_vec[from_cid]->get_physical_file_id(dest_opnd.file_type());
  if (dest_opnd.file_type() == CR)
    from_idx = hw_cluster_vec[from_cid]->get_physical_file_id(GPR);
  int to_idx = hw_cluster_vec[to_cid]->get_physical_file_id(dest_opnd.file_type());

  List<int>* move_list = min_path[from_idx][to_idx];
  if (move_list == NULL || move_list->size() == 0)
    El_punt("get_dest_moves_for_operand: unable to move op %d DEST %d from cluster %d to cluster %d",
            op->id(), pnum, from_cid, to_cid);

  Operand opnd_to_move = Operand(new Reg(dest_opnd.dtype()));
  operand_map.bind(opnd_to_move.vr_num(), from_cid);
  op->set_dest(pnum, opnd_to_move);

  bool last_move = false;
  Operand pred_true(new Pred_lit(true));
  Operand pred_false(new Pred_lit(false));

  for (List_iterator<int> li(*move_list); li!=0; li++) {
    if (last_move) {
      El_warn("get_dest_moves_for_operand: skipping seemingly unnecessary moves in list!");
      break;
    }
    
    int cur_dest_cid = *li;
    Opcode opcode = get_icmove_opcode(opnd_to_move, false);
    Op* newop = new Op(opcode);
    newop->set_flag(EL_OPER_ICMOVE);

    if (cur_dest_cid == to_cid)
      last_move = true;           // this will be the last move

    // special case for destinations of U_preds: guard always true
    // also, intermediate moves of O_pred and A_pred dests are always done
    if (is_U_pred(op, pnum) ||
        !last_move && (is_O_pred(op, pnum) || is_A_pred(op, pnum))) {
      newop->set_src(PRED1, pred_true);
      newop->set_src(SRC1, opnd_to_move);
    }
    // for final move of O_preds: guard is temp, src always true
    else if (last_move && is_O_pred(op, pnum)) {
      newop->set_src(PRED1, opnd_to_move);
      newop->set_src(SRC1, pred_true);
    }
    // for A_preds:
    // original:  p1 = cmp_AN    w/icmoves: t1 = cmp_OC
    //                                      t2 = t1 if T
    //                                        etc
    //                                      p1 = F if t2
    else if (last_move && is_A_pred(op, pnum)) {
      Opcode new_opc;
      if(is_AN_pred(op, pnum)) {
        new_opc = make_OC_pred(op->opcode(), pnum);
        op->set_opcode(new_opc);
      }
      else if(is_AC_pred(op, pnum)) {
        new_opc = make_ON_pred(op->opcode(), pnum);
        op->set_opcode(new_opc);
      }
      newop->set_src(PRED1, opnd_to_move);
      newop->set_src(SRC1, pred_false);
    }
    // all others: guard is original guard (may be overwritten by
    // insert_block_moves if original guard needs to be moved)
    else {
      if (op->predicated())
        newop->set_src(PRED1, op->src(PRED1));
      else
        newop->set_src(PRED1, pred_true);
      newop->set_src(SRC1, opnd_to_move);
    }

    // set the dest of the move
    if (last_move) {
      newop->set_dest(DEST1, dest_opnd);
      // re-set the opcode because the dest may be a CR
      newop->set_opcode(get_icmove_opcode(dest_opnd, false));
    } else {
      Operand opnd_temp(new Reg(opnd_to_move.dtype()));  // new temp operand
      operand_map.bind(opnd_temp.vr_num(), cur_dest_cid);
      newop->set_dest(DEST1, opnd_temp);
      opnd_to_move = opnd_temp;
    }

    move_ops.add_tail(newop);
  }
}

Op* Cluster_mgr::get_pred_clear_op(Operand opr)
{
   Op* newop = new Op(PRED_CLEAR);
   newop->set_dest(DEST1, opr);
   return newop;
}

Op* Cluster_mgr::get_pred_set_op(Operand opr)
{
   Op* newop = new Op(PRED_SET);
   newop->set_dest(DEST1, opr);
   return newop;
}

// get a list of clusters supporting this op.
// also cache it so future lookup of the same op is faster.
void Cluster_mgr::collect_valid_clusters_op(Op* op, List<int>*& return_list)
{
  if (is_cmpp(op) || is_svmove(op)) {
    // assign all the cmpps and svmoves that write to the same operand to the
    // same cluster.  This is an optimization to reduce ICMOVEs for predicates.
    // For SVMOVES it's a correctness issue.
    for (Op_explicit_dests desti(op); desti!=0; desti++) {
      Operand &dest = *desti;
      if (pre_assign_map.is_bound(dest)) {
	if (op_valid_cluster_map.is_bound(op)) {
	  return_list = op_valid_cluster_map.value(op);
	  return_list->clear();
	} else {
	  return_list = new List<int>;
	  op_valid_cluster_map.bind(op, return_list);
	}
	return_list->add_tail(pre_assign_map.value(dest));
	return;
      }
    }
  } 

   if (op_valid_cluster_map.is_bound(op)) {
      return_list = op_valid_cluster_map.value(op);
   }
   else {
     return_list = new List<int>;

     if (return_list -> is_empty()) {
       for (unsigned c = 0; c < hw_cluster_vec.size(); c++) {
	 HW_cluster* hwc = hw_cluster_vec[c];
	 if (hwc->is_supported(op))
	   return_list->add_tail(c);
       }
     }
	    
     op_valid_cluster_map.bind(op, return_list);
   }
}

// list of clusters supporting this operand.
// only called by do_block_operand_clustering which is in turn
// only called when doing random (not BUG) clustering
void Cluster_mgr::collect_valid_clusters_operand(Reg_file rf, int width, 
						 List<int>& cluster_list)
{
   char* prf;
   for (unsigned c = 0; c < hw_cluster_vec.size(); c++) {
      HW_cluster* hwc = hw_cluster_vec[c];
      prf = hwc->get_physical_file(rf);
      if (prf != NULL && MDES_reg_width(prf) >= width)
	 cluster_list.add_tail(c);
   }
}

void Cluster_mgr::print_machine()
{
   cdbg << "total clusters: " << hw_cluster_vec.size() << endl;
   cdbg << "macro register locations:" << endl;
   for (int i = FIRST_MACRO; i < LAST_MACRO; i++) {
      Macro_reg mr((Macro_name)i);
      cdbg << mr << "\tvalid on clusters: ";
      for(Hash_set_iterator<int> macro_iter(macro_vec[i].get_potential_homes()); 
	  macro_iter!=0; macro_iter++) {
	int clust = *macro_iter;
	cdbg << clust << ", ";
      }
      cdbg << endl;
   }
   print_clusters();

   cdbg << "Inter-cluster move opcodes: " ;
   for(Hash_set_iterator<eString> hsi(ic_move_opcodes); hsi!=0; hsi++) {
     cdbg << (char *) *hsi << " ";
   }
   cdbg << endl;
}

void Cluster_mgr::print_clusters()
{
   for (unsigned c = 0; c < hw_cluster_vec.size(); c++) {
      cdbg << "cluster " << c << endl;
      cdbg << *hw_cluster_vec[c];
   }
}

void Cluster_mgr::print_op_clusters()
{
   cdbg << "* Printing op clusters..." << endl;
   for (Hash_map_iterator<Op*, int> mapi(op_map); mapi != 0; mapi++) {
      Op* op = (*mapi).first;
      int clstr = (*mapi).second;
      cdbg << "op " << op->id() << " " << op->opcode() << "\ton cluster " 
	   << clstr << endl;
   }
}

void Cluster_mgr::print_operand_clusters()
{
   cdbg << "* Printing operand clusters..." << endl;
   for (Hash_map_iterator<int, int> mapi(operand_map); mapi != 0; mapi++) {
      int opr = (*mapi).first;
      int clstr = (*mapi).second;
      cdbg << "VR" << opr << "\ton cluster " << clstr << endl;
   }
}

void Cluster_mgr::print_operand_freq()
{
   for (int i=0; i<operand_freq.dim1(); i++) {
      cdbg << "vr "<<i<<":\t";
      for (int j=0; j<operand_freq.dim2(); j++)
	 cdbg << operand_freq[i][j] << '\t';
      cdbg << endl;
   }
}

void Cluster_mgr::print_macro_vec()
{
  cdbg << "Macro Vector contents:" << endl;
  for(unsigned i = 1; i < macro_vec.size(); i++) {
    cdbg << i << ": " << el_macro_to_string_map.value((Macro_name) i);
    cdbg << " current home: " << macro_vec[i].get_current_home() << 
      " potential homes: { " ;
    Hash_set<int> temp = macro_vec[i].get_potential_homes();
    for(Hash_set_iterator<int> hsi(temp); hsi != 0; hsi++) {
      int clust = *hsi;
      cdbg << clust << " ";
    }
    cdbg << "}" << endl;
  }
}

Hash_set<eString>& Cluster_mgr::get_ic_move_opcodes()
{
  return ic_move_opcodes;
}

char* Cluster_mgr::get_physical_file_for_operand(Operand oper)
{
  if(!operand_map.is_bound(oper.vr_num()))
    El_punt("Operand is not bound");
  else {    
    int clust = get_cluster(oper);
    return hw_cluster_vec[clust]->get_physical_file(oper.file_type());
  }   
  return NULL;
}

int Cluster_mgr::get_cluster(Op* op)
{
  if(!op_map.is_bound(op))
    return -1;
  else
    return op_map.value(op);
}

int Cluster_mgr::get_cluster(Operand& oper)
{
  if(!operand_map.is_bound(oper.vr_num()))
    return -1;
  else
    return operand_map.value(oper.vr_num());
}

int Cluster_mgr::get_cluster_for_physical_file(eString physfile)
{
  if(!physical_file_to_cluster_map.is_bound(physfile))
    return -1;
  else
    return physical_file_to_cluster_map.value(physfile);
}

Hash_set<eString>& Cluster_mgr::get_physical_files_for_macro(Macro_name mn)
{
  return macro_vec[mn].get_physical_files();
}

// Return the cluster id of the BRF in kernel. Rajiv 07/03
int Cluster_mgr::get_staging_pred_cid(Compound_region *kernel)
{
    for (Region_all_ops opi(kernel); opi!=0; opi++) {
      Op *op = *opi;
      if (is_brf(op) || is_branch(op)) {
        assert(op_map.is_bound(op));
        return op_map.value(op);
      }
    }

    El_punt("get_staging_pred_cid: brf not found in kernel: %d\n", kernel->id());
    return -1;
}

Opcode Cluster_mgr::get_icmove_opcode(Operand& oprnd, bool is_source)
{
   DType dt = oprnd.dtype();

   if (dt.is_int()){
     if(oprnd.file_type() == CR) {
       if(is_source)
	 return MOVECG;
       else
	 return MOVEGC;
     } 
     else
       return MOVEGG;
   }
   else if (dt.is_pointer())    return MOVEGG;
   else if (dt.is_single())     return MOVEF_S;
   else if (dt.is_double())     return MOVEF_D;
   else if (dt.is_predicate())  return MOVEPP;
   else if (dt.is_branch())     return MOVEBB;
   else if (dt.is_vec_int())    return VMOVEGG;
   else if (dt.is_vec_single()) return VMOVEFF_S;
   else if (dt.is_vec_double()) return VMOVEFF_D;
   else if (dt.is_vec_mask())   return VMOVEMM;
   // moving vec masks is currently unsupported.  -KF 1/2007
   else El_punt("get_icmove_opcode: unknown data type");

   return (Opcode) 0;
}

Operand Cluster_mgr::get_staging_predicate(Compound_region* rgn)
{
  if (!rgn->flag(EL_REGION_SOFTPIPE))
    El_punt("Cluster_mgr::get_staging_predicate: rgn is not a SWP loop");

  while(( ! rgn->is_loopbody()) && (rgn != NULL)) {
    rgn = rgn->parent();
  }

  if(rgn == NULL) {
    El_punt("Cluster_mgr::get_staging_predicate: can't find the loopbody for region %d", rgn->id());
  }

  return ((LoopBody*)rgn)->get_staging_pred();;
}
