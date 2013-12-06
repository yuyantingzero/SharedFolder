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
//      File:           postpass.cpp
//      Author:         Kevin Fan, Mike Chu
//      Created:        May 2003
//      Description:    Postpass cluster assignment and move insertion
//
///////////////////////////////////////////////////////////////////////////

#include "cluster.h"
#include "opcode_load_store.h"

// Rajiv: assign spill code (add and memory) to cluster 0
void Cluster_mgr::do_block_bogus_postpass_clustering(Compound_region *r)
{
   if (dbg(status,2))
     cdbg << "Performing bogus postpass clustering on region: " << r->id()
          << " of procedure " << (char*)(el_get_enclosing_procedure(r)->get_name()) << endl;

   for (Region_all_ops opi(r); opi!=0; opi++) {
      Op *op = *opi;
      if (op->flag(EL_OPER_SPILL_CODE) || op->flag(EL_OPER_STACK_MOD)) {
         op_map.bind(op, 0);

         for (Op_explicit_operands oprndi(op); oprndi!=0; oprndi++) {
            Operand& oprnd = *oprndi;
            El_ref mref(op, &oprnd, oprndi.ref_type());
            if (oprnd.is_macro_reg()) {
               if ((oprnd.name() == SPILL_TEMPREG) 
                   || (oprnd.name() == spill_macro) 
                   || (oprnd.name() == RETURN_ADDR)) {
	           macro_vec[oprnd.name()].add_reference(mref, 0);
	       }
            }
         }
      }
   }
   
}

// Added 5.31.02 mchu
// We are going to assume that there is either one SP or that they are coherent between
// clusters (so we can pick any of the SP's)
// assign spill code (add and memory) to clusters
void Cluster_mgr::do_block_postpass_clustering(Compound_region* block, Cluster_method cm)
{
  if (cm == EL_CLSTR_NONE) {
    do_block_bogus_postpass_clustering(block);
    return;
  }

  if (dbg(status,2))
    cdbg << "Performing postpass clustering on region: " << block->id()
         << " of procedure " << (char*)(el_get_enclosing_procedure(block)->get_name()) << endl;

  if (!block->is_hb() && !block->is_bb())
    El_punt("block_operand_clustering: region %d not bb or hb", block->id());

  do_block_bd_postpass_clustering(block, cm);

  do_block_stack_mod_postpass_clustering(block, cm);  // pracheeti's stack modification

  // Hongtao 7-03 - add cluster id attributes to all ops in block
  install_cluster_id_attr(block);

  do_block_postpass_move_insertion(block, cm);
}


// base + displacement postpass clustering
void Cluster_mgr::do_block_bd_postpass_clustering(Compound_region* block, Cluster_method cm)
{
  List<int>* valid_clusters; 
  Hash_set<int> mem_clusters(hash_int, 16);
  Hash_set<int> sp_clusters(hash_int, 16);

  for (Region_ops_C0_order opi(block); opi != 0; opi++) {
    Op* op = *opi;
    if(!op->flag(EL_OPER_SPILL_CODE) && !op->flag(EL_OPER_STACK_MOD)) continue;
    
    if(op->flag(EL_OPER_SPILL_CODE)) {
      Operand& src1 = op->src(SRC1);
      
      if(!src1.is_macro_reg()) continue;
      if(src1.name() != spill_macro) continue;

      // the operand being spilled or the one that was spilled
      Operand spill_oper;
      if(is_save(op) || is_store(op)) 
        spill_oper = op->src(SRC3);     
      else if(is_restore(op) || is_load(op))
        spill_oper = op->dest(DEST1);
      else
        El_punt("do_block_bd_postpass_clustering: op %d is spill code that is neither SAVE nor RESTORE\n",
                op->id());
      
      Hash_set<int> placement_set(hash_int, 16);

      int oper_cluster = physical_file_to_cluster_map.value(spill_oper.physical_file_type());
      El_ref sp_ref(op, &src1, EXP_SRC);

      sp_clusters = macro_vec[spill_macro].get_potential_homes();
      collect_valid_clusters_op(op, valid_clusters);

      for(List_iterator<int> li(*valid_clusters); li != 0; li++) {
        if(sp_clusters.is_member(*li))
          placement_set += *li;
      }
      
      if(placement_set.size() >= 1) {
	int clust = placement_set.is_member(oper_cluster) ? oper_cluster : placement_set.head();
	op_map.bind(op, clust);	   
        macro_vec[spill_macro].add_reference(sp_ref, clust);
      }
      else {
	int clust = mem_clusters.is_member(oper_cluster) ? oper_cluster : valid_clusters->head();
	op_map.bind(op, clust);
        macro_vec[spill_macro].add_reference(sp_ref, sp_clusters.head());
      }        
    }    
  }  
}


void Cluster_mgr::do_block_stack_mod_postpass_clustering(Compound_region* block, Cluster_method cm)
{
  Op* add_op = NULL;
  Op* mem_op = NULL;
  El_ref sp_add_ref, tr_add_ref, tr_mem_ref;
  List<int>* valid_add_clusters;
  List<int>* valid_mem_clusters; 
  Hash_set<int> add_clusters(hash_int, 16);
  Hash_set<int> mem_clusters(hash_int, 16);
  Hash_set<int> sp_clusters(hash_int, 16);
  Hash_set<int> ret_clusters(hash_int, 16);

  for (Region_ops_C0_order opi(block); opi != 0; opi++) {
    Op* op = *opi;
    if (!op->flag(EL_OPER_SPILL_CODE) && !op->flag(EL_OPER_STACK_MOD)) continue;
 
    if(op->flag(EL_OPER_STACK_MOD)) {
      // postpass clustering for Pracheeti's ops
      sp_clusters = macro_vec[SP_REG].get_potential_homes();

      if (op->opcode() == ADD_W) {
	add_op = op;
	Operand& src1 = op->src(SRC1);
	Operand& dest1 = op->dest(DEST1);

	// refs
	El_ref src_ref(add_op, &src1, EXP_SRC);
	El_ref dst_ref(add_op, &dest1, EXP_DEST);
      
	collect_valid_clusters_op(add_op, valid_add_clusters);
	for(List_iterator<int> li(*valid_add_clusters); li != 0; li++) 
	  add_clusters += *li;

	Hash_set<int> placement_set(hash_int, 16);
	placement_set = sp_clusters;
	placement_set *= add_clusters;

	if(placement_set.size() >= 1) {
	  int clust = placement_set.head();
	  op_map.bind(op, clust);	   
	  macro_vec[SP_REG].add_reference(src_ref, clust);
	  macro_vec[SP_REG].add_reference(dst_ref, clust);
	}
	else {
	  int clust = add_clusters.head();
	  op_map.bind(op, clust);
	  macro_vec[SP_REG].add_reference(src_ref, sp_clusters.head());
	  macro_vec[SP_REG].add_reference(dst_ref, sp_clusters.head());
	}
      } 
      else if((is_store(op) && is_mem_base_plus_offset(op)) || is_save(op)) {
	Operand& sp = op->src(SRC1);
	Operand& ra = op->src(SRC3);
	     
	// refs
	El_ref src1_ref(op, &sp, EXP_SRC);
	El_ref src3_ref(op, &ra, EXP_SRC);

	collect_valid_clusters_op(op, valid_mem_clusters);
	for(List_iterator<int> li(*valid_mem_clusters); li != 0; li++)
	  mem_clusters += *li;
	     
	ret_clusters = macro_vec[RETURN_ADDR].get_potential_homes();
	sp_clusters = macro_vec[SP_REG].get_potential_homes();
	     
	Hash_set<int> placement_set(hash_int, 16);
	placement_set = sp_clusters;
	placement_set *= ret_clusters;
	placement_set *= mem_clusters;
	     
	if(placement_set.size() >= 1) {
	  int clust = placement_set.head();
	  op_map.bind(op, clust);	   
	  macro_vec[SP_REG].add_reference(src1_ref, clust);
	  macro_vec[RETURN_ADDR].add_reference(src3_ref, clust);
	  continue;
	}

	placement_set = sp_clusters;
	placement_set *= mem_clusters;
	     
	if(placement_set.size() >= 1) {
	  int clust = placement_set.head();
	  op_map.bind(op, clust);	   
	  macro_vec[SP_REG].add_reference(src1_ref, clust);
	  macro_vec[RETURN_ADDR].add_reference(src3_ref, ret_clusters.head());
	  continue;
	}

	placement_set = ret_clusters;
	placement_set *= mem_clusters;
	     
	if(placement_set.size() >= 1) {
	  int clust = placement_set.head();
	  op_map.bind(op, clust);	   
	  macro_vec[SP_REG].add_reference(src1_ref, sp_clusters.head());
	  macro_vec[RETURN_ADDR].add_reference(src3_ref, clust);
	  continue;
	}
	     
	op_map.bind(op, mem_clusters.head());
	macro_vec[SP_REG].add_reference(src1_ref, sp_clusters.head());
	macro_vec[RETURN_ADDR].add_reference(src3_ref, ret_clusters.head());	     
      }
      else if((is_load(op) && is_mem_base_plus_offset(op)) || is_restore(op)) {
	mem_op = op;
	     
	Operand& src1 = op->src(SRC1);
	Operand& dst1 = op->dest(DEST1);

	sp_clusters = macro_vec[SP_REG].get_potential_homes();
	ret_clusters = macro_vec[RETURN_ADDR].get_potential_homes();

	collect_valid_clusters_op(op, valid_mem_clusters);
	for(List_iterator<int> li(*valid_mem_clusters); li != 0; li++)
	  mem_clusters += *li;

	El_ref src1_ref(op, &src1, EXP_SRC);
	El_ref dst1_ref(op, &dst1, EXP_DEST);

	Hash_set<int> placement_set(hash_int, 16);
	placement_set = sp_clusters;
	placement_set *= ret_clusters;
	placement_set *= mem_clusters;
	     
	if(placement_set.size() >= 1) {
	  int clust = placement_set.head();
	  op_map.bind(op, clust);	   
	  macro_vec[SP_REG].add_reference(src1_ref, clust);
	  macro_vec[RETURN_ADDR].add_reference(dst1_ref, clust);
	  continue;
	}

	placement_set = sp_clusters;
	placement_set *= mem_clusters;
	     
	if(placement_set.size() >= 1) {
	  int clust = placement_set.head();
	  op_map.bind(op, clust);	   
	  macro_vec[SP_REG].add_reference(src1_ref, clust);
	  macro_vec[RETURN_ADDR].add_reference(dst1_ref, ret_clusters.head());
	  continue;
	}

	placement_set = ret_clusters;
	placement_set *= mem_clusters;
	     
	if(placement_set.size() >= 1) {
	  int clust = placement_set.head();
	  op_map.bind(op, clust);	   
	  macro_vec[SP_REG].add_reference(src1_ref, sp_clusters.head());
	  macro_vec[RETURN_ADDR].add_reference(dst1_ref, clust);
	  continue;
	}
	     
	op_map.bind(op, mem_clusters.head());
	macro_vec[SP_REG].add_reference(src1_ref, sp_clusters.head());
	macro_vec[RETURN_ADDR].add_reference(dst1_ref, ret_clusters.head());	     

      } // is mem_base_plus_offset(op)
      else
        El_punt("unidentified op: %d with STACK_MOD flag..\n", op->id());
    }	 
  }
}

void Cluster_mgr::do_block_postpass_move_insertion(Compound_region* block, Cluster_method cm)
{
  if (cm == EL_CLSTR_NONE)
    return;

  if (dbg(status,2))
    cdbg << "Performing postpass move insertion clustering on region: " << block->id()
         << " of procedure " << (char*)(el_get_enclosing_procedure(block)->get_name()) << endl;

  if (!block->is_hb() && !block->is_bb())
    El_punt("do_block_postpass_move_insertion: region %d not bb or hb", block->id());

  for (Region_ops_C0_order opi(block); opi != 0; opi++) {
    Op* op = *opi;
    if (!op->flag(EL_OPER_SPILL_CODE) && !op->flag(EL_OPER_STACK_MOD)) continue;
    
    if (op->flag(EL_OPER_SPILL_CODE) && (is_save(op) || is_restore(op))) {

      Operand& src1 = op->src(SRC1);
      if (!src1.is_macro_reg()) continue;
      if (src1.name() != spill_macro) continue;
	
      // get the cluster of the SAVE/RESTORE op
      int cluster = op_map.value(op);

      // get the cluster of the SP
      El_ref sp_ref(op, &src1, EXP_SRC);
      int sp_cluster = macro_vec[spill_macro].get_binding(sp_ref);

      moves_inserted += add_predicate_moves(block, op, cluster);

      // see if SP move is needed to op's cluster
      if (cluster != sp_cluster) {
        List<Op*> move_ops;
        get_postpass_moves(op, SRC, SRC1, sp_cluster, cluster, move_ops);
        for (List_iterator<Op*> li(move_ops); li!=0; li++) {
          El_insert_op_before(*li, op);
          moves_inserted++;
        }
      }

      // see if move is needed to/from op's clustr
      if (is_save(op) || is_store(op)) {
        // check here for PV?
        Operand& operand = op->src(SRC3);
        int data_cluster = get_cluster_for_physical_file(operand.physical_file_type());

        if (data_cluster != cluster) {
          List<Op*> move_ops;

          // handle PV macros
          if(operand.is_macro_reg() && current_MDES->is_pv_macro(operand.name())) {
            Op* pv_move = insert_postpass_pv_moves(op, SRC, SRC3, data_cluster, cluster, move_ops);
            get_postpass_moves(pv_move, DEST, DEST1, data_cluster, cluster, move_ops);
            Op* last_op = move_ops.tail();
            op->set_opcode((Opcode) SAVEG);
            op->set_src(SRC3, last_op->dest(DEST1));
          }
          else {
            get_postpass_moves(op, SRC, SRC3, data_cluster, cluster, move_ops);
          }

          for (List_iterator<Op*> li(move_ops); li!=0; li++) {
            (*li)->set_flag(EL_OPER_SPILL_CODE);
            El_insert_op_before(*li, op);
            moves_inserted++;
          }
        }
      }
      else { 
        Operand& operand = op->dest(DEST1);
        int data_cluster = get_cluster_for_physical_file(operand.physical_file_type());

        if (data_cluster != cluster) {
          List<Op*> move_ops;

          if(operand.is_macro_reg() && current_MDES->is_pv_macro(operand.name())) {
            Op* pv_move = insert_postpass_pv_moves(op, DEST, DEST1, cluster, data_cluster, move_ops);
            pv_move->set_parent(op->parent());
            get_postpass_moves(pv_move, SRC, SRC1, cluster, data_cluster, move_ops);
            move_ops.add_tail(pv_move);            
            op->set_opcode((Opcode) RESTOREG);
          }
          else {
            get_postpass_moves(op, DEST, DEST1, cluster, data_cluster, move_ops);
          }

          Op* prev_op = op;
          for (List_iterator<Op*> li(move_ops); li!=0; li++) {
            (*li)->set_flag(EL_OPER_SPILL_CODE);
            El_insert_op_after(*li, prev_op);
            prev_op = *li;
            moves_inserted++;
          }
        }
      }
    }
  }
}

Op* Cluster_mgr::get_postpass_move_op(Operand& dest, Operand& src, Operand& pred)
{
  Op* move_op = new Op(get_icmove_opcode(dest, false));
  move_op->set_flag(EL_OPER_SPILL_CODE);
  move_op->set_flag(EL_OPER_ICMOVE);
  move_op->set_src(PRED1, pred);
  move_op->set_src(SRC1, src);
  move_op->set_dest(DEST1, dest);
  return move_op;
}

// inserts a MOVEPG or MOVEGP depending on whether a PSAVEG or PRESTOREG 
// with a PV was issued.
Op* Cluster_mgr::insert_postpass_pv_moves(Op* op, Port_type ptype, Port_num pnum,
                                     int src_cid, int dest_cid,
                                     List<Op*>& move_ops) 
{
  Op* newop;

  // create the operand for the GPR destination of the PV
  DType dt = EL_STANDARD_INT;
  Operand opr_temp(new Reg(dt));  // new temp operand

  if (ptype == SRC) {
    Operand operand = op->src(pnum);
    operand_map.bind(opr_temp.vr_num(), dest_cid);  
    Opcode opcode = MOVEPG;

    newop = new Op(opcode);

    if (!op->predicated() || pnum == PRED1) {
      Operand pred_true(new Pred_lit(true));
      newop->set_src(PRED1, pred_true);
    } else {
      newop->set_src(PRED1, op->src(PRED1));
    }

    Operand pred_true(new Pred_lit(true));
    newop->set_src(PRED1, pred_true);
    newop->set_src(SRC1, operand);
    newop->set_dest(DEST1, opr_temp);

    op_map.bind(newop, src_cid);

    move_ops.add_head(newop);      
  } else {
    Operand operand = op->dest(pnum);
    operand_map.bind(opr_temp.vr_num(), src_cid);      
    Opcode opcode = MOVEGP;

    newop = new Op(opcode);

    if (!op->predicated() || pnum == PRED1) {
      Operand pred_true(new Pred_lit(true));
      newop->set_src(PRED1, pred_true);
    } else {
      newop->set_src(PRED1, op->src(PRED1));
    }

    Operand pred_true(new Pred_lit(true));
    newop->set_src(PRED1, pred_true);
    newop->set_src(SRC1, opr_temp);
    newop->set_dest(DEST1, operand);

    op_map.bind(newop, dest_cid);
    op->set_dest(DEST1, opr_temp);
  }

  newop->set_flag(EL_OPER_SPILL_CODE);
    
  return newop;
}

void Cluster_mgr::get_postpass_moves(Op* op, Port_type ptype, Port_num pnum,
                                     int from_cid, int to_cid,
                                     List<Op*>& move_ops)
{
  if (ptype == SRC)
    get_src_moves_for_operand(op, pnum, from_cid, to_cid, move_ops);
  else if (ptype == DEST)
    get_dest_moves_for_operand(op, pnum, from_cid, to_cid, move_ops);
  else
    El_punt("get_postpass_moves: unsupported Port_type");

  bool spill_code  = op->flag(EL_OPER_SPILL_CODE);
  bool callee_save = op->flag(EL_OPER_CALLEE_SAVE);
  bool caller_save = op->flag(EL_OPER_CALLER_SAVE);
  bool stack_mod   = op->flag(EL_OPER_STACK_MOD);

  Operand pred_true(new Pred_lit(true));

  for (List_iterator<Op*> li(move_ops); li!=0; li++) {
    Op* move_op = *li;
    if (spill_code)  move_op->set_flag(EL_OPER_SPILL_CODE);
    if (callee_save) move_op->set_flag(EL_OPER_CALLEE_SAVE);
    if (caller_save) move_op->set_flag(EL_OPER_CALLER_SAVE);
    if (stack_mod)   move_op->set_flag(EL_OPER_STACK_MOD);

    // temporarily, guard source moves on true.  more intelligent move
    // insertion for guards of moves should happen here. -KF 12/2004
    if (ptype == SRC)
      move_op->set_src(PRED1, pred_true);

    // insert moves for guards of dest moves if necessary. this may insert more
    // moves than necessary because there is no opti phase after postpass move
    // insertion. -KF 1/2005
    if (ptype == DEST) {
      if (move_op->predicated() &&
          move_op->src(PRED1).is_reg()) {
        int src_cid = operand_map.value(move_op->src(SRC1).vr_num());
        int guard_cid = operand_map.value(move_op->src(PRED1).vr_num());
        if (guard_cid != src_cid) {
          List<Op*> move_pp_ops;
          get_src_moves_for_operand(move_op, PRED1, guard_cid, src_cid, move_pp_ops);
          for (List_iterator<Op*> li2(move_pp_ops); li2!=0; li2++) {
            Op* move_pp_op = *li2;
            if (spill_code)  move_pp_op->set_flag(EL_OPER_SPILL_CODE);
            if (callee_save) move_pp_op->set_flag(EL_OPER_CALLEE_SAVE);
            if (caller_save) move_pp_op->set_flag(EL_OPER_CALLER_SAVE);
            if (stack_mod)   move_pp_op->set_flag(EL_OPER_STACK_MOD);
          }
          move_pp_ops.append(move_ops);
          move_ops = move_pp_ops;
        }
      }
    }
  }
}

int Cluster_mgr::add_predicate_moves(Compound_region * block, Op * op, int clstr) 
{
  int moves_inserted = 0;

  // see if move of guarding predicate is needed
  if (op->predicated() &&
      op->src(PRED1).is_reg() &&
      clstr != operand_map.value(op->src(PRED1).vr_num())) {
    
    int pred_clstr = operand_map.value(op->src(PRED1).vr_num());
    List<Op*> move_ops;
    get_postpass_moves(op, SRC, PRED1, pred_clstr, clstr, move_ops);

    for (List_iterator<Op*> li(move_ops); li!=0; li++) {
      El_insert_op_before(*li, op);
      moves_inserted++;
    }
  }
  
  return moves_inserted;
}
