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
//                                                                       //
//      File:           move_codegen.cpp                                 //
//      Authors:        Rajiv Ravindran                                  //
//      Created:        April 2002                                       //
//      Description:    move codegen:  Inserts move ops when             //
//                      prf annotation is empty                          //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "codegen.h"

static Op *get_mdes_move(eString dest_vrf, Operand& mov_src_oprnd)
{
  Reg *dummy_reg = new Reg(mov_src_oprnd.dtype(), mdesname_to_vregfile(*dest_vrf));

  Operand dummy_dest_oprnd(dummy_reg);
  Opcode opc = get_move_opcode_for_operands(dummy_dest_oprnd, mov_src_oprnd);
  Op *mov_op = new Op(opc);

  return mov_op;
}

static Op *get_move_to_new_reg(Hash_set<eString>& vrf_list, Operand& mov_src_oprnd,
			       eString& dest_vrf, Op* op)
{
  Op *mov_op = NULL;
    
  assert(!mov_src_oprnd.is_predicate());

  if (mov_src_oprnd.is_int())
    dest_vrf = "I";
  else if (mov_src_oprnd.is_float())
    dest_vrf = "F";
  else if (mov_src_oprnd.is_double())
    dest_vrf = "F";

  if (mov_src_oprnd.is_int() || mov_src_oprnd.is_float() || mov_src_oprnd.is_double())
    if (!vrf_list.is_member(dest_vrf)) {
      cdbg << "\nERROR: This operand: " << mov_src_oprnd << "\nIn this op: " << *op
	   << "\ndoes not fit in the op it is being used in (according to the MDES).\n"
	   << "Codegen is trying to create a move for this operand, but doing that\n"
	   << "creates an op that isn't in the MDES, either.\n\n"
	   << "For example, if a floating point base+displacement load was created that had\n"
	   << "a large offset:\n\nf1 = FLD [r1 + #big_number]\n\nIf the MDES did not support "
	   << "loads with a large offset, codegen would\ntry to turn that into\n\nr2 = #big_number\n"
	   << "f1 = FLD [r1 + r2]\n\nIf this second sequence is also unsupported, then you will get "
	   << "this error.\nEssentially codegen doesn't know how to help you.\n"
	   << "The best way to fix this problem (usually) is to add a machine specific\n"
	   << "op conversion in Codegen/<arch>_convert.cpp. There are some examples in\n"
	   << "Codegen/arm_convert.cpp\n\n";
      El_punt("");
    }

  if (mov_src_oprnd.is_int() || mov_src_oprnd.is_float() || mov_src_oprnd.is_double())
    mov_op = get_mdes_move(dest_vrf, mov_src_oprnd);
  else {
    for (Hash_set_iterator<eString> hiter(vrf_list); hiter!=0; hiter++) {
      dest_vrf = *hiter;
      mov_op = get_mdes_move(dest_vrf, mov_src_oprnd);
      if (mov_op)
	break;
    }
  }
  if (!mov_op)
    El_punt("make_move_to_new_reg: move not supported!");


  return mov_op;
}

// Called from initial codegen
void Codegen::do_move_codegen(Compound_region *r)
{
  if (dbg(genrio,1))
    cdbg << "Move Codegen for region: " << r->id() << " in proc: " 
	 << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;

  for (Region_all_ops opi(r); opi!=0; opi++) {
    Op *op = *opi;

    if (is_pseudo(op) || is_no_op(op))
      continue;

    assert(get_prf_binding_attr(op));

    // Since this is called before clustering, get the set of all
    // compatible alternatives from the MDES instead of on a per
    // cluster basis.
    List_set<Op_descr *> unit_specific_opdes_set;
    MDES_collect_op_descrs(el_opcode_to_string_map.value(op->opcode()), unit_specific_opdes_set);

    insert_move_ops(op, unit_specific_opdes_set, false);
  }
}

void Codegen::do_post_cluster_move_codegen(Compound_region *r, Cluster_mgr& cmgr)
{
  if (dbg(genrio, 1))
    cdbg << "Post cluster move codegen for region: " << r->id() << endl;

  for (Region_all_ops opi(r); opi!=0; opi++) {
    Op *op = *opi;

    if (is_pseudo(op) || is_no_op(op))
      continue;

    assert(get_prf_binding_attr(op));

    int cid;
    List_set<Op_descr*> clust_uspc_ops;

    if (op->flag(EL_OPER_ICMOVE)) {
      Hash_set<eString> uspc_icmoves(hash_estring,1024);
      uspc_icmoves = cmgr.get_ic_move_opcodes();

      List_set<Op_descr*> uspc_op_descrs;
      MDES_collect_op_descrs(el_opcode_to_string_map.value(op->opcode()), uspc_op_descrs);

      for (List_set_iterator<Op_descr*> liter(uspc_op_descrs); liter!=0; liter++) {
	Op_descr *op_descr = *liter;
	if (uspc_icmoves.is_member(op_descr->get_aname_ptr()))
	  clust_uspc_ops += op_descr;
      }
    } else {
      assert(Cluster_mgr::op_map.is_bound(op));
      cid = Cluster_mgr::op_map.value(op);
      get_uspc_ops_in_cluster(op, cid, clust_uspc_ops);
    }

    assert(!clust_uspc_ops.is_empty());
    insert_move_ops(op, clust_uspc_ops, true);
  }
}

// This assumes that a move can satisfy the ops choices(if they are empty)
// else it asserts.
void Codegen::do_actual_move_insertion(Op *op, List_set<Op_descr*>& uspc_ops, bool post_cluster)
{
  List_set<Op_descr*> ld_uspc_ops;
  Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
  assert(prf_battr != 0);

  for (Op_explicit_inputs srci(op); srci!=0; srci++) {
    Operand& src_oprnd = *srci;

    if (src_oprnd.is_reg() || src_oprnd.is_macro_reg() 
	|| (src_oprnd.is_lit() && !src_oprnd.is_predicate())) {
      Hash_set<eString>& prf_set = prf_battr->src(srci.get_port_num());
      if (prf_set.is_empty()) {

	if (dbg(genrio, 3))
	  cdbg << "insert_move_ops: prf annotations empty for op: " << op->id() 
	       << " at port: " << srci.get_port_num() << endl;

	List_set<Reg_descr *> src_reg_descset;
	MDES_collect_reg_descrs(el_opcode_to_string_map.value(op->opcode()), DATA_IN, 
				srci.get_port_num(), src_reg_descset);
	Hash_set<eString> vrf_set(hash_estring, 128);
	for (List_set_iterator<Reg_descr *> iter(src_reg_descset); iter!=0; iter++) {
	  Reg_descr *reg_desc = *iter;
	  vrf_set += reg_desc->get_vname();
	}

	// Create the mov_op
	eString dest_vrf;
	Op *mov_op = get_move_to_new_reg(vrf_set, src_oprnd, dest_vrf, op);

	// zzz
	Reg *new_reg;
	if (src_oprnd.is_int()) {
	  DType dtype(src_oprnd.dtype().width(), src_oprnd.dtype().is_signed(), EL_DT_INFO_INT);
	  new_reg = new Reg(dtype, mdesname_to_vregfile(*dest_vrf));
	} else if (src_oprnd.is_float() || src_oprnd.is_double()) {
	  DType dtype(src_oprnd.dtype().width(), src_oprnd.dtype().is_signed(), EL_DT_INFO_FLOAT);
	  new_reg = new Reg(dtype, mdesname_to_vregfile(*dest_vrf));
	} else if (src_oprnd.is_label() || src_oprnd.is_cb() || src_oprnd.is_string()) {
	  DType dtype = DType(src_oprnd.dtype().width(), src_oprnd.dtype().is_signed(), EL_DT_INFO_INT);
	  new_reg = new Reg(dtype, mdesname_to_vregfile(*dest_vrf));
	} else
	  new_reg = new Reg(src_oprnd.dtype(), mdesname_to_vregfile(*dest_vrf), 0);

	new_reg->rename(); // move to a new virtual register
	Operand new_oprnd(new_reg);

	// Caution: if the predicates were empty, then a later mov
	// for the predicate would cause this mov_op to be bound to
	// wrong predicate!
	mov_op->set_src(SRC1, src_oprnd);
	mov_op->set_dest(DEST1, new_oprnd);
	Operand pred_true(new Pred_lit(true));
	if (!op->predicated() || (srci.get_port_num() == PRED1))
	  mov_op->set_src(PRED1, pred_true);
	else
	  mov_op->set_src(PRED1, op->src(PRED1));
	if (op->flag(EL_OPER_LIVEOUT_DEST))
	  mov_op->set_flag(EL_OPER_LIVEOUT_DEST);
	if (op->flag(EL_OPER_SPILL_CODE))
	  mov_op->set_flag(EL_OPER_SPILL_CODE);
	if (op->flag(EL_OPER_CALLEE_SAVE))
	  mov_op->set_flag(EL_OPER_CALLEE_SAVE);
	if (op->flag(EL_OPER_CALLER_SAVE))
	  mov_op->set_flag(EL_OPER_CALLER_SAVE);
	mov_op->set_flag(EL_OPER_CGEN_MOVE);

	op->set_src(srci.get_port_num(), new_oprnd); 
	Codegen::annotate_prf_port(DATA_IN, srci.get_port_num(), new_oprnd, prf_battr, uspc_ops, op);
	assert(!(prf_battr->src(srci.get_port_num()).is_empty()));

	// If the op is bound to a cluster, bind the mov_op to the same cluster
	// The mov_op get its possible alternatives from this cluster id
	// But if the op is a ICMOVE, use the source operand of ICMOVE as mov_op's cluster
	// else the op is not bound to any cluster in which case get all the alternatives from MDES
	List_set<Op_descr*> mov_uspc_ops;
	if (Cluster_mgr::op_map.is_bound(op)) 
	  Codegen::get_uspc_ops_in_cluster(mov_op, Cluster_mgr::op_map.value(op), mov_uspc_ops);
	else if (op->flag(EL_OPER_ICMOVE)) 
	  Codegen::get_uspc_ops_in_cluster(mov_op, Cluster_mgr::operand_map.value(src_oprnd.vr_num()), mov_uspc_ops);
	else
	  MDES_collect_op_descrs(el_opcode_to_string_map.value(mov_op->opcode()), mov_uspc_ops);

	// Annotate the mov op
	Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
	set_prf_binding_attr(mov_op, mov_prf_battr);
	Codegen::annotate_physical_file_op(mov_op, mov_uspc_ops);

	// The literal in the mov could not be supported by any
	// literal file in the MDES.  Insert the literal into the
	// operand_to_label_map
	if (mov_op->src(SRC1).is_lit()) {
	  Operand& lit_oprnd = mov_op->src(SRC1);
	  if (mov_prf_battr->src(SRC1).is_empty()) {
	    eString label;
	    bool found = false;
	    for (Hash_map_iterator<Operand, eString> iter(Codegen::operand_to_label_map); iter!=0; iter++) {
	      if ((*iter).first == lit_oprnd) {
		label = (*iter).second;
		found = true;
		if (dbg(genrio, 3))
		  cdbg << "found label: " << label << " for lit oprnd: " << lit_oprnd << endl;
		break;
	      }
	    }
	    if (!found) {
	      // create a new label
	      char buff[512];
	      memset(buff, 0, 512);
	      sprintf(buff, "%s_%d_%d", (char*)el_get_enclosing_procedure(op)->get_name(),
		      op->id(), srci.get_port_num());
	      label.cat(buff);
	      Codegen::operand_to_label_map.bind(lit_oprnd, label);
	      if (dbg(genrio, 3))
		cdbg << "creating label: " << label << " for lit oprnd: " << lit_oprnd << endl;
	    }
	    // insert a new load op which loads the literal from the label
	    // replace the just created mov op by the ld op
	    Op *ld_op = NULL;
	    if (lit_oprnd.is_int()) {
	      ld_op = new Op((Opcode)L_W_C1_C1);
	    } else if (lit_oprnd.is_float()) {
	      ld_op = new Op((Opcode) FL_S_C1_C1);
	    } else if (lit_oprnd.is_double()) {
	      ld_op = new Op((Opcode) FL_D_C1_C1);
	    } else
	      assert(0);
	    ld_op->set_dest(DEST1, new_oprnd);
	    ld_op->set_src(PRED1, mov_op->src(PRED1));
	    Operand labeloprnd(new Label_lit(label));
	    ld_op->set_src(SRC1, labeloprnd);

	    if (op->flag(EL_OPER_LIVEOUT_DEST))
	      ld_op->set_flag(EL_OPER_LIVEOUT_DEST);
	    if (op->flag(EL_OPER_SPILL_CODE))
	      ld_op->set_flag(EL_OPER_SPILL_CODE);
	    if (op->flag(EL_OPER_CALLEE_SAVE))
	      ld_op->set_flag(EL_OPER_CALLEE_SAVE);
	    if (op->flag(EL_OPER_CALLER_SAVE))
	      ld_op->set_flag(EL_OPER_CALLER_SAVE);
	    ld_op->set_flag(EL_OPER_CGEN_MOVE);

	    if (Cluster_mgr::op_map.is_bound(op)) 
	      Codegen::get_uspc_ops_in_cluster(ld_op, Cluster_mgr::op_map.value(op), ld_uspc_ops);
	    else
	      MDES_collect_op_descrs(el_opcode_to_string_map.value(ld_op->opcode()), ld_uspc_ops);
	    // Annotate the ld op
	    Physfile_binding_attr *ld_prf_battr = new Physfile_binding_attr();
	    set_prf_binding_attr(ld_op, ld_prf_battr);
	    Codegen::annotate_physical_file_op(ld_op, ld_uspc_ops);

	    delete mov_op;

	    // set the mov_op
	    mov_op = ld_op;
	    mov_prf_battr = ld_prf_battr;
	  }
	}

        // If a load from a literal was generated, then it is possible
        // that the load might not be supported. A later pass of 
        // insert_move_ops (scroll down) fixes this
        bool need_move_again = false;
        if (!is_load(mov_op)) {
	  // the mov op can be a regular mov of a literal or a load op(created above)
	  if ((mov_prf_battr->src(SRC1)).is_empty())
            El_punt("insert_move_ops: Trying to insert mov for op: %d for src port %d; Failed to find a mov (SRC1 empty) to satisfy the i/o format constraints of this op. Check the mdes for op: %s\n", op->id(), srci.get_port_num(),
                    (char *)el_opcode_to_string_map.value(op->opcode()));
	  if ((mov_prf_battr->dest(DEST1)).is_empty())
            El_punt("insert_move_ops: Trying to insert mov for op: %d for src port %d; Failed to find a mov (DEST1 empty) to satisfy the i/o format constraints of this op. Check the mdes for op: %s\n", op->id(), srci.get_port_num(),
                    (char *)el_opcode_to_string_map.value(op->opcode()));
	  if (!mov_op->src(PRED1).is_lit())
	    if ((mov_prf_battr->src(PRED1)).is_empty())
              El_punt("insert_move_ops: Trying to insert mov for op: %d for src port %d; Failed to find a mov (PRED1 empty) to satisfy the i/o format constraints of this op. Check the mdes for op: %s\n", op->id(), srci.get_port_num(),
                      (char *)el_opcode_to_string_map.value(op->opcode()));
        } else {
          if ((mov_prf_battr->src(SRC1)).is_empty() ||
              (mov_prf_battr->dest(DEST1)).is_empty() ||
              (!mov_op->src(PRED1).is_lit() && (mov_prf_battr->src(PRED1)).is_empty()))
            need_move_again = true;
          else
            need_move_again = false;
        }

	Codegen::move_ops++;
	if (dbg(genrio, 1))
	  cdbg << "move_codegen: Inserting mov op " << mov_op->id() 
	       << " for op " << op->id() << endl;

	El_insert_op_before(mov_op, op);
	if (post_cluster == true) {
	  // add the newly inserted mov op to the current op's cluster
	  // if the current op is an icmove add the newly inserted mov op
	  // to the source (of the icmove) operand's cluster.
	  // assert that the mov_op is supported in that cluster

	  int cid;
	  if (op->flag(EL_OPER_ICMOVE)) {
	    assert(Cluster_mgr::operand_map.is_bound(src_oprnd.vr_num()));
	    cid = Cluster_mgr::operand_map.value(src_oprnd.vr_num());
	  } else {
	    assert(Cluster_mgr::op_map.is_bound(op));
	    cid = Cluster_mgr::op_map.value(op);
	  }
	  assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_op));
	  Cluster_mgr::op_map.bind(mov_op, cid);
	  Cluster_mgr::operand_map.bind(new_oprnd.vr_num(), cid);

#if 0
	  if( ! strcasecmp(El_arch, "arm")) {
	    if(mem_fixup) {
	      Cluster_mgr::op_map.bind(fixup_ld_op, cid);
	      Cluster_mgr::operand_map.bind(fixup_ld_op->dest(DEST1).vr_num(), cid);
	    }
	  }
#endif
	}
        // It is possible (eg: ARM), that the ld from the label
        // generated above might not be valid if the label is not
        // allowed to be the src operand. In which case, an additional
        // mov gets inserted here if necessary.
        if (need_move_again)
          do_actual_move_insertion(mov_op, ld_uspc_ops, post_cluster);
      }
    } 
  }

  for (Op_explicit_dests desti(op); desti!=0; desti++) {
    Operand& dest_oprnd = *desti;
    assert(!dest_oprnd.is_lit());

    if (dest_oprnd.is_reg() || dest_oprnd.is_macro_reg()) {
      Hash_set<eString>& prf_set = prf_battr->dest(desti.get_port_num());
      if (prf_set.is_empty()) {
	List_set<Reg_descr *> dest_reg_descset;
	MDES_collect_reg_descrs(el_opcode_to_string_map.value(op->opcode()), DATA_OUT, 
				desti.get_port_num(), dest_reg_descset);
	Hash_set<eString> vrf_set(hash_estring, 128);
	for (List_set_iterator<Reg_descr *> iter(dest_reg_descset); iter!=0; iter++) {
	  Reg_descr *reg_desc = *iter;
	  vrf_set += reg_desc->get_vname();
	}

	eString dest_vrf;
	Op *mov_op = get_move_to_new_reg(vrf_set, dest_oprnd, dest_vrf, op);

	Reg *new_reg;
	new_reg = new Reg(dest_oprnd.dtype(), mdesname_to_vregfile(*dest_vrf), 0);
	new_reg->rename(); // move to a new virtual register

	Operand new_oprnd(new_reg);

	Operand pred_true(new Pred_lit(true));
	if (is_U_pred(op, desti.get_port_num())) {
	  mov_op->set_src(SRC1, new_oprnd);
	  mov_op->set_src(PRED1, pred_true);
	} else if (is_O_pred(op, desti.get_port_num())) {
	  mov_op->set_src(PRED1, new_oprnd);
	  mov_op->set_src(SRC1, pred_true);
	} else {
	  mov_op->set_src(SRC1, new_oprnd);
	  if (op->predicated())
	    mov_op->set_src(PRED1, op->src(PRED1));
	  else
	    mov_op->set_src(PRED1, pred_true);
	}
	mov_op->set_dest(DEST1, dest_oprnd);
	if (op->flag(EL_OPER_LIVEOUT_DEST))
	  mov_op->set_flag(EL_OPER_LIVEOUT_DEST);
	if (op->flag(EL_OPER_SPILL_CODE))
	  mov_op->set_flag(EL_OPER_SPILL_CODE);
	if (op->flag(EL_OPER_CALLEE_SAVE))
	  mov_op->set_flag(EL_OPER_CALLEE_SAVE);
	if (op->flag(EL_OPER_CALLER_SAVE))
	  mov_op->set_flag(EL_OPER_CALLER_SAVE);
	mov_op->set_flag(EL_OPER_CGEN_MOVE);

	op->set_dest(desti.get_port_num(), new_oprnd);
	Codegen::annotate_prf_port(DATA_OUT, desti.get_port_num(), new_oprnd, prf_battr, uspc_ops, op);
	assert(!(prf_battr->dest(desti.get_port_num())).is_empty());

	// Same as the input operands above
	// Instead use the dest_oprnd of the ICMOVE(if op is ICMOVE)
	List_set<Op_descr*> mov_uspc_ops;
	if (Cluster_mgr::op_map.is_bound(op)) 
	  Codegen::get_uspc_ops_in_cluster(mov_op, Cluster_mgr::op_map.value(op), mov_uspc_ops);
	else if (op->flag(EL_OPER_ICMOVE)) 
	  Codegen::get_uspc_ops_in_cluster(mov_op, Cluster_mgr::operand_map.value(dest_oprnd.vr_num()), 
					   mov_uspc_ops);
	else
	  MDES_collect_op_descrs(el_opcode_to_string_map.value(mov_op->opcode()), mov_uspc_ops);

	Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
	set_prf_binding_attr(mov_op, mov_prf_battr);
	Codegen::annotate_physical_file_op(mov_op, mov_uspc_ops);

	if (!mov_op->src(SRC1).is_predicate()) 
	  if ((mov_prf_battr->src(SRC1)).is_empty())
            El_punt("insert_move_ops: Trying to insert mov for op: %d for dest port %d; Failed to find a mov (SRC1 empty) to satisfy the i/o format constraints of this op. Check the mdes for op: %s\n", op->id(), desti.get_port_num(),
                    (char *)el_opcode_to_string_map.value(op->opcode()));
	if ((mov_prf_battr->dest(DEST1)).is_empty())
          El_punt("insert_move_ops: Trying to insert mov for op: %d for dest port %d; Failed to find a mov (DEST1 empty) to satisfy the i/o format constraints of this op. Check the mdes for op: %s\n", op->id(), desti.get_port_num(),
                  (char *)el_opcode_to_string_map.value(op->opcode()));
	if (!mov_op->src(PRED1).is_predicate())
	  if ((mov_prf_battr->src(PRED1)).is_empty())
            El_punt("insert_move_ops: Trying to insert mov for op: %d for dest port %d; Failed to find a mov (PRED1 empty) to satisfy the i/o format constraints of this op. Check the mdes for op: %s\n", op->id(), desti.get_port_num(),
                    (char *)el_opcode_to_string_map.value(op->opcode()));

	Codegen::move_ops++;
	if (dbg(genrio, 3))
	  cdbg << "move_codegen: Inserting mov op " << mov_op->id() 
	       << "for op " << op->id() << endl;

	El_insert_op_after(mov_op, op);
	if (post_cluster == true) {
	  // add the newly inserted mov op to the current op's cluster
	  // if the current op is an icmove add the newly inserted mov op
	  // to the destination (of the icmove) operand's cluster.
	  // assert that the mov_op is supported in that cluster

	  int cid;
	  if (op->flag(EL_OPER_ICMOVE)) {
	    assert(Cluster_mgr::operand_map.is_bound(dest_oprnd.vr_num()));
	    cid = Cluster_mgr::operand_map.value(dest_oprnd.vr_num());
	  } else {
	    assert(Cluster_mgr::op_map.is_bound(op));
	    cid = Cluster_mgr::op_map.value(op);
	  }
	  assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_op));
	  Cluster_mgr::op_map.bind(mov_op, cid);
	  Cluster_mgr::operand_map.bind(new_oprnd.vr_num(), cid);
	}
      }
    } 
  }
}

// This tries to compute how many moves are required if the op has
// empty prf choices for its operands. If the choices are empty for an
// operand, try inserting a move and check whether the move can create
// a non-null prf set for the operand If not, return -1
int Codegen::compute_estimated_moves(Op *op, List_set<Op_descr*>& uspc_ops)
{
  int estimated_moves = 0;

  Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
  assert(prf_battr != 0);

  for (Op_explicit_inputs srci(op); srci!=0; srci++) {
    Operand& src_oprnd = *srci;

    if (src_oprnd.is_reg() || src_oprnd.is_macro_reg() 
	|| (src_oprnd.is_lit() && !src_oprnd.is_predicate())) {
      Hash_set<eString>& prf_set = prf_battr->src(srci.get_port_num());
      if (prf_set.is_empty()) {

	estimated_moves++;

	if (dbg(genrio, 3))
	  cdbg << "compute_estimated_moves: prf annotations empty for op: " << op->id() 
	       << " at port: " << srci.get_port_num() << endl;

	// Now see if the move can create a non-empty prf choice for src_oprnd.
	List_set<Reg_descr *> src_reg_descset;
	MDES_collect_reg_descrs(el_opcode_to_string_map.value(op->opcode()), DATA_IN, 
				srci.get_port_num(), src_reg_descset);
	Hash_set<eString> vrf_set(hash_estring, 128);
	for (List_set_iterator<Reg_descr *> iter(src_reg_descset);
	     iter!=0; iter++) {
	  Reg_descr *reg_desc = *iter;
	  vrf_set += reg_desc->get_vname();
	}

	// we create the mov_op for the dest_vrf which is used
	// for creating the new operand.
	eString dest_vrf;
	Op *mov_op = get_move_to_new_reg(vrf_set, src_oprnd, dest_vrf, op);

	Reg *reg;
	if (src_oprnd.is_int()) {
	  DType dtype(src_oprnd.dtype().width(),
		      src_oprnd.dtype().is_signed(), EL_DT_INFO_INT);
	  reg = new Reg(dtype, mdesname_to_vregfile(*dest_vrf));
                    
	} else if (src_oprnd.is_float() || src_oprnd.is_double()) {
	  DType dtype(src_oprnd.dtype().width(), src_oprnd.dtype().is_signed(), EL_DT_INFO_FLOAT);
	  reg = new Reg(dtype, mdesname_to_vregfile(*dest_vrf));
	} else if (src_oprnd.is_label() || src_oprnd.is_cb() || src_oprnd.is_string()) {
	  DType dtype = DType(src_oprnd.dtype().width(), src_oprnd.dtype().is_signed(), EL_DT_INFO_INT);
	  reg = new Reg(dtype, mdesname_to_vregfile(*dest_vrf));
	} else {
	  reg = new Reg(src_oprnd.dtype(), mdesname_to_vregfile(*dest_vrf), 0);
	}

	Operand dest_oprnd(reg);

	// create a dummy prf_battr and try annotating it(for this src port)
	// if its empty, it means the move cannot satify the src operand
	// hence return -1.
	Physfile_binding_attr dummy_prf_battr;
	Codegen::annotate_prf_port(DATA_IN, srci.get_port_num(), dest_oprnd, &dummy_prf_battr, uspc_ops, op);

	if (dummy_prf_battr.src(srci.get_port_num()).is_empty()) {
	  estimated_moves = -1;
	  delete mov_op;
	  break;
	} 

	delete mov_op;
      }
    }
  }

  if (estimated_moves == -1) return -1;

  for (Op_explicit_dests desti(op); desti!=0; desti++) {
    Operand& dest_oprnd = *desti;
    assert(!dest_oprnd.is_lit());

    if (dest_oprnd.is_reg() || dest_oprnd.is_macro_reg()) {
      Hash_set<eString>& prf_set = prf_battr->dest(desti.get_port_num());
      if (prf_set.is_empty()) {
	List_set<Reg_descr *> dest_reg_descset;
	MDES_collect_reg_descrs(el_opcode_to_string_map.value(op->opcode()), DATA_OUT, 
				desti.get_port_num(), dest_reg_descset);
	Hash_set<eString> vrf_set(hash_estring, 128);
	for (List_set_iterator<Reg_descr *> iter(dest_reg_descset); iter!=0; iter++) {
	  Reg_descr *reg_desc = *iter;
	  vrf_set += reg_desc->get_vname();
	}

	eString dest_vrf;
	Op *mov_op = get_move_to_new_reg(vrf_set, dest_oprnd, dest_vrf, op);

	Reg *dummy_reg = new Reg(dest_oprnd.dtype(), mdesname_to_vregfile(*dest_vrf), 0);
	Operand new_oprnd(dummy_reg);
	new_oprnd.rename(); // move to a new virtual register

	// create a dummy prf_battr and try annotating it(for this dest port)
	// if its empty, it means the move cannot satify the src operand
	// hence return -1.
	Physfile_binding_attr dummy_prf_battr;
	Codegen::annotate_prf_port(DATA_OUT, desti.get_port_num(), new_oprnd, &dummy_prf_battr, uspc_ops, op);

	if (dummy_prf_battr.dest(desti.get_port_num()).is_empty()) {
	  estimated_moves = -1;
	  delete mov_op;
	  break;
	} 
	delete mov_op;
      }
    }
  }

  return estimated_moves;
}
    
// If the prf annotation is empty, do a move
// First see if a move can satisfy the ops choices, if not try swapping
// the src operands 
// Finally call do_actual_move_insertion to do the moves
void Codegen::insert_move_ops(Op *op, List_set<Op_descr*>& uspc_ops, bool post_cluster)
{
  int estimated_moves;
  int swap_estimated_moves;

  estimated_moves = compute_estimated_moves(op, uspc_ops);
  // If no moves can satisfy this op, try swapping operands
  if (estimated_moves == -1) {
    // swap operands
    if ((MDES_commutative(el_opcode_to_string_map.value(op->opcode())) && (op->num_srcs() == 2)) || 
	(is_load(op) && is_mem_base_plus_offset(op))) {
      Operand src1 = op->src(SRC1);
      Operand src2 = op->src(SRC2);

      Op *new_op = Codegen::clone_op(op);
      new_op->set_src(SRC1, src2);
      new_op->set_src(SRC2, src1);
      Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(new_op);
      if (rdt_attr != NULL) {
	rdt_attr->swap_srcs(SRC1, SRC2);
      }

      Physfile_binding_attr *new_prf_battr = new Physfile_binding_attr();
      annotate_prf_port(DATA_IN, SRC1, new_op->src(SRC1), new_prf_battr, uspc_ops, new_op);
      annotate_prf_port(DATA_IN, SRC2, new_op->src(SRC2), new_prf_battr, uspc_ops, new_op);

      // for load/store set the SRC3 prf attributes of new_op same as that of the original op
      if (is_load(op) || is_store(op))
	new_prf_battr->set_src(SRC3, (get_prf_binding_attr(op))->src(SRC3));

      // set the dest prf attributes of new_op same as that of the original op
      for (int p=op->first_output(); p<=op->last_output(); p++)
	new_prf_battr->set_dest(p, (get_prf_binding_attr(op))->dest(p));

      set_prf_binding_attr(new_op, new_prf_battr);

      swap_estimated_moves = compute_estimated_moves(new_op, uspc_ops);

      remove_prf_binding_attr(new_op);
      delete new_op;

      // Moves cannot satisfy the ops choices even for swapped case..!
      if (swap_estimated_moves == -1) 
	El_punt("insert_move_ops: op: %d not supported in mdes even through moves!\n", op->id());
      else {
	// swap operands
	if (dbg(genrio, 3))
	  cdbg << "insert_move_ops: swapping srcs for op: " << op->id() 
	       << " because of invalid move choice.." << endl;

	Operand src1 = op->src(SRC1);
	Operand src2 = op->src(SRC2);

	op->set_src(SRC1, src2);
	op->set_src(SRC2, src1);
	Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
	if (rdt_attr != NULL) {
	  rdt_attr->swap_srcs(SRC1, SRC2);
	}
	Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
	assert(prf_battr != 0);
	annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op);
	annotate_prf_port(DATA_IN, SRC2, op->src(SRC2), prf_battr, uspc_ops, op);

	// If the swap resulted in 0 moves do not do move insertion
	// This case would not i presume arise..as the annotate_physical_file phase
	// tries to do a swap if it finds that the swap estimate is less than
	// the actual estimate
	if (swap_estimated_moves > 0)
	  do_actual_move_insertion(op, uspc_ops, post_cluster);
      } 
    } else 
      El_punt("insert_move_ops: op: %d not supported in mdes even through moves!\n", op->id());
  } else if (estimated_moves > 0) {
    do_actual_move_insertion(op, uspc_ops, post_cluster);
  }
}

