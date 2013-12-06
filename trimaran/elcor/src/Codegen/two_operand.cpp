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
//      File:           two_operand.cpp                                  //
//      Authors:        Rajiv Ravindran                                  //
//      Created:        April 2002                                       //
//      Description:    two operand format conversion                    //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "codegen.h"

bool Codegen::is_two_oprnd_compatible(Op *op, Operand& src_oprnd, Hash_set<eString>& src_prf_set)
{
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    Hash_set<eString> dest_prf_set = prf_battr->dest(DEST1);

    Operand& dest_oprnd = op->dest(DEST1);

    if (src_prf_set == dest_prf_set) {
        if (src_oprnd.is_macro_reg() && dest_oprnd.is_macro_reg() &&
            (src_oprnd.name() == dest_oprnd.name()))
            return true;
        else {
            if (src_oprnd.is_reg() && dest_oprnd.is_reg() &&
                (src_oprnd.vr_num() == dest_oprnd.vr_num()))
                return true;
            else
                return false;
        }
    } else
        return false;
}


// Op is in 2-operand format if
// there is some common PRF between dest1 and src1
// and dest1 and src1 have same vr_num(or macro names)
// else a move is required.
bool Codegen::in_two_operand_format(Op *op)
{
    if (!MDES_two_operand(el_opcode_to_string_map.value(op->opcode())))
        return false;

    Operand src1_oprnd = op->src(SRC1);
    Operand dest_oprnd = op->dest(DEST1);

    // make sure that the prf annotations aren't empty
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    assert(prf_battr != 0);
    Hash_set<eString> src_prf_set = prf_battr->src(SRC1);
    Hash_set<eString> dest_prf_set = prf_battr->dest(DEST1);
    assert(!src_prf_set.is_empty()); assert(!dest_prf_set.is_empty());
    
    Hash_set<eString> tmp_set(src_prf_set);
    tmp_set *= dest_prf_set;
    if (!tmp_set.is_empty()) {
        if (src1_oprnd.is_macro_reg() && dest_oprnd.is_macro_reg() &&
	    (src1_oprnd.name() == dest_oprnd.name())) 
            return true;
        else {
            if (src1_oprnd.is_reg() && dest_oprnd.is_reg() && 
	        (src1_oprnd.vr_num() == dest_oprnd.vr_num()))
                return true;
        }
    }
 
    return false;
}

// convert 
// r1 = r2 + r3 =>
// r1 = r2
// r1 = r1 + r3;
void Codegen::do_two_oprnd_convert(Op *op, List_set<Op_descr *>& uspc_ops, bool post_cluster)
{
    if (!MDES_two_operand(el_opcode_to_string_map.value(op->opcode())))
        return;

    if (Codegen::in_two_operand_format(op))
        return;

    // Handle r1 = r2 - r1
    // This is not commutative
    // r100 = r2
    // r100 = r100 - r1
    // r1 = r100
    if (op->dest(DEST1) == op->src(SRC2)) {
        Opcode opc = get_move_opcode_for_operands(op->dest(DEST1), op->src(SRC1));
        Op *mov_op1 = new Op(opc);
        Op *mov_op2 = new Op(opc);

        Operand new_dest_oprnd = new Reg(op->dest(DEST1).dtype());
        mov_op1->set_dest(DEST1, new_dest_oprnd);
        mov_op1->set_src(PRED1, op->src(PRED1));
        mov_op1->set_src(SRC1, op->src(SRC1));
        mov_op2->set_src(PRED1, op->src(PRED1));
        mov_op2->set_src(SRC1, new_dest_oprnd);
        mov_op2->set_dest(DEST1, op->dest(DEST1));
        op->set_src(SRC1, new_dest_oprnd);
        op->set_dest(DEST1, new_dest_oprnd);

        mov_op1->set_flag(EL_OPER_CGEN_MOVE);
        mov_op2->set_flag(EL_OPER_CGEN_MOVE);

        Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
        Operand dest_oprnd = op->dest(DEST1);

        // Since we have changed src1, dest1 of op, re-annotate src1,dest1;
        Codegen::annotate_prf_port(DATA_OUT, DEST1, op->dest(DEST1), prf_battr, uspc_ops, op); 
        Codegen::annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op); 

        List_set<Op_descr*> mov_uspc_ops1;
        List_set<Op_descr*> mov_uspc_ops2;
        if (post_cluster == true) {
            assert(Cluster_mgr::op_map.is_bound(op));
            int cid = Cluster_mgr::op_map.value(op);
            get_uspc_ops_in_cluster(mov_op1, cid, mov_uspc_ops1);
            get_uspc_ops_in_cluster(mov_op2, cid, mov_uspc_ops2);
        } else {
            MDES_collect_op_descrs(el_opcode_to_string_map.value(mov_op1->opcode()), mov_uspc_ops1);
            MDES_collect_op_descrs(el_opcode_to_string_map.value(mov_op2->opcode()), mov_uspc_ops2);
        }

        Physfile_binding_attr *mov1_prf_battr = new Physfile_binding_attr();
        Physfile_binding_attr *mov2_prf_battr = new Physfile_binding_attr();
        set_prf_binding_attr(mov_op1, mov1_prf_battr);
        set_prf_binding_attr(mov_op2, mov2_prf_battr);
        Codegen::annotate_physical_file_op(mov_op1, mov_uspc_ops1);
        Codegen::annotate_physical_file_op(mov_op2, mov_uspc_ops2);

        if ((mov1_prf_battr->src(SRC1)).is_empty() || (mov2_prf_battr->src(SRC1)).is_empty()) 
          El_punt("two_oprnd_convert: cannot annotate src1 of mov ops: %d,%d with any valid choice",
                  mov_op1->id(), mov_op2->id());
        if ((mov1_prf_battr->dest(DEST1)).is_empty() || (mov2_prf_battr->dest(DEST1)).is_empty()) 
          El_punt("two_oprnd_convert: cannot annotate dest1 of mov op: %d with any valid choice", 
                  mov_op1->id(), mov_op2->id());

        if (dbg(genrio, 5))
          cdbg << "two_oprnd_convert: Inserting move op: " << mov_op1->id() 
               << " and move op: " << mov_op2->id()
               << " for op: " << op->id() << endl;

        El_insert_op_before(mov_op1, op);
        El_insert_op_after(mov_op2, op);
        // if post cluster, add the newly inserted mov op to the current op's cluster
        // assert that the mov_op is supported in that cluster
        if (post_cluster == true) {
            assert(Cluster_mgr::op_map.is_bound(op));
            int cid = Cluster_mgr::op_map.value(op);
            assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_op1));
            Cluster_mgr::op_map.bind(mov_op1, cid);
            Cluster_mgr::op_map.bind(mov_op2, cid);
            Cluster_mgr::operand_map.bind(new_dest_oprnd.vr_num(), cid);
        }
    
        Codegen::two_oprnd_move_ops += 2;

        return;
    }
    
    // if op is not already in 2-operand format, we first check to see
    // whether the dest1 can be src1's position. i.e whether the 2-operand
    // format of the form r1 = r1 + r3 would result in the op having invalid choice
    // (i.e the the new src would have prf choice which is a strict subset of its old choices
    // in which case, we assume 2-operand convert not possible
    if (!Codegen::prf_bind_replace(op, op->dest(DEST1), DATA_IN, SRC1, uspc_ops))
        El_punt("do_two_oprnd_convert: src1 cannot be placed in dest1's position 2-operand conversion not possible for op: %d",
                     op->id());

    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    Operand dest_oprnd = op->dest(DEST1);

    // Convert to 2-operand format, insert the move op;
    Opcode opc = get_move_opcode_for_operands(op->dest(DEST1), op->src(SRC1));
    Op *mov_op = new Op(opc);

    mov_op->set_dest(DEST1, op->dest(DEST1));
    mov_op->set_src(PRED1, op->src(PRED1));
    mov_op->set_src(SRC1, op->src(SRC1));
    op->set_src(SRC1, op->dest(DEST1));
    mov_op->set_flag(EL_OPER_CGEN_MOVE);

    // Since we have changed src1 of op, re-annotate src1;
    Codegen::annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op); 
    assert(!prf_battr->src(SRC1).is_empty());

    List_set<Op_descr*> mov_uspc_ops;
    if (post_cluster == true) {
        assert(Cluster_mgr::op_map.is_bound(op));
        int cid = Cluster_mgr::op_map.value(op);
        get_uspc_ops_in_cluster(mov_op, cid, mov_uspc_ops);
    } else
        MDES_collect_op_descrs(el_opcode_to_string_map.value(mov_op->opcode()), mov_uspc_ops);

    Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
    set_prf_binding_attr(mov_op, mov_prf_battr);
    Codegen::annotate_physical_file_op(mov_op, mov_uspc_ops);

    if ((mov_prf_battr->src(SRC1)).is_empty()) 
        El_punt("two_oprnd_convert: cannot annotate src1 of mov op: %d with any valid choice", mov_op->id());
    if ((mov_prf_battr->dest(DEST1)).is_empty()) 
        El_punt("two_oprnd_convert: cannot annotate dest1 of mov op: %d with any valid choice", mov_op->id());

    if (dbg(genrio, 3))
        cdbg << "two_oprnd_convert: Inserting move op: " << mov_op->id() 
             << " for op: " << op->id() << endl;

    El_insert_op_before(mov_op, op);
    // if post cluster, add the newly inserted mov op to the current op's cluster
    // assert that the mov_op is supported in that cluster
    if (post_cluster == true) {
        assert(Cluster_mgr::op_map.is_bound(op));
        int cid = Cluster_mgr::op_map.value(op);
        assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_op));
        Cluster_mgr::op_map.bind(mov_op, cid);
    }
    
    Codegen::two_oprnd_move_ops++;
}

// Check whether the following conversion is possible 
// r1 = r2 + r3 =>
// r2 = r2 + r3;
// Try reuse of src1
static bool check_reuse1(Op *op, Reaching_defs_info *rdinfo, List_set<Op_descr*>& uspc_ops, bool swap)
{
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    assert(prf_battr != 0);

    int port;
    if (swap)
        port = SRC2;
    else 
        port = SRC1;

    if (!rdinfo->is_interesting(op->src(port)))
        return false;

    // Do not convert to 2-operand for macros like IP,OP etc.
    if (op->src(port).is_macro_reg()) {
      if ((op->src(port).name() == SP_REG) || (op->src(port).name() == IP_REG) ||
          (op->src(port).name() == OP_REG) || (op->src(port).name() == LV_REG))
      return false;
    }

    Operand srcoprnd = op->src(port);

    Compound_region *r = op->parent();

    for (Region_ops_C0_order opi(r, op); opi!=0; opi++) {
      Op *op1 = *opi;
      if (op1 == op) continue;
      for (Op_explicit_inputs inpi(op1); inpi!=0; inpi++) {
        Operand src = *inpi;
        if (src == srcoprnd) {
          if (dbg(genrio,5)) 
             cdbg << "check_reuse1: another use of src oprnd: " << src << " for src port: " << port 
                  << " for op: " << op->id() << " found at op: " << op1->id() << endl;
          return false;
        }
      }
    }

    for (Region_exit_edges edgi(r, CONTROL0_OUTEDGES); edgi!=0; edgi++) {
      Edge *edge = *edgi;
      Liveness_info* edge_live = get_liveness_info(edge);
      if (edge_live == 0) continue;
      for (List_iterator<Operand> oiter(*edge_live); oiter!=0; oiter++) {
        Operand oprnd = *oiter;
        if (oprnd == srcoprnd) {
          if (dbg(genrio,5))
            cdbg << "check_reuse1: op: " << op->id() << " port: " << port
                 << " src oprnd: " << oprnd << " in liveout of region r: " << r->id() << endl;
          return false;
        }
      }
    }

    // Make sure that all uses of the dest of op(r1 in above example),
    // is reached only by this dest
    // Also, make sure that the use op is not in 2-operand format 
    El_ref dest_ref(op, &(op->dest(DEST1)), EXP_DEST);
    List<El_ref> *use_list1 = rdinfo->get_du_chain(dest_ref);
    for (List_iterator<El_ref> liter2(*use_list1); liter2!=0; liter2++) {
        El_ref use = *liter2;
        // if a use is outside rdinfo's scope, ignore
        if ((use.get_ref_type() != PRED_SRC) &&
            (use.get_ref_type() != EXP_SRC))
            return false;
        if (Codegen::in_two_operand_format(use.get_op()))
            return false;

        Op *use_op = use.get_op();

        // check whether srcoprnd(r2) is in livein of the region of use_op
        for (Region_entry_edges edgi(use_op->parent(), CONTROL0_OUTEDGES); edgi!=0; edgi++) {
          Edge *edge = *edgi;
          Liveness_info* edge_live = get_liveness_info(edge);
          if (edge_live == 0) continue;
          for (List_iterator<Operand> oiter(*edge_live); oiter!=0; oiter++) {
            Operand oprnd = *oiter;
            if (oprnd == srcoprnd) {
              if (dbg(genrio,5))
                cdbg << "check_reuse1: use op: " << use_op->id() << " def op: " << op->id()
                     << " for src oprnd: " << oprnd << " in livein of region r: " << use_op->id() << endl;
              return false;
            }
          }
        }
        
        List<El_ref> *def_list_1 = rdinfo->get_ud_chain(use);
        if (def_list_1->size() != 1)
            return false;
    }	
        
    // Try if src1 can be in dest1's position. i.e will the resulting 2-operand
    // format r2 = r2 + r3 cause the dest's prf choices be a strict subset of its original
    // prf choice..this could result in the dest not having a valid alternative which
    // can support it
    if (!Codegen::prf_bind_replace(op, op->src(port), DATA_OUT, DEST1, uspc_ops))
        return false;

    // Make sure that the replaced src does not violate the prf choice decisions
    // the new src's prf choice after replace by op's src1 should not be empty
    // r1 = r2 + r3
    //  .
    //  = r1 //use of r1
    // ==>
    // r2 = r2 + r3
    // .
    // = r2

    List<El_ref> *use_list = rdinfo->get_du_chain(dest_ref);
    for (List_iterator<El_ref> liter1(*use_list); liter1!=0; liter1++) {
        Op *use_op = (*liter1).get_op();
        List_set<Op_descr *> use_uspc_ops;
        MDES_collect_op_descrs(el_opcode_to_string_map.value(use_op->opcode()), use_uspc_ops);
        if (!Codegen::prf_bind_replace(use_op, op->src(port), DATA_IN, 
                              (*liter1).get_port_num(), use_uspc_ops))
            return false;
    }

    return true;
}
      

// Check whether the following conversion is possible 
// r1 = r2 + r3 =>
// r2 = r2 + r3;
// Try reuse of src1
static bool check_reuse(Op *op, Reaching_defs_info *rdinfo, List_set<Op_descr*>& uspc_ops, bool swap)
{
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    assert(prf_battr != 0);

    int port;
    if (swap)
        port = SRC2;
    else 
        port = SRC1;

    if (!rdinfo->is_interesting(op->src(port)))
        return false;

    // make sure that all the defs of the src(r2 in above example), reach only this src.
    El_ref src1_ref(op, &(op->src(port)), EXP_SRC);
    List<El_ref>* def_list = rdinfo->get_ud_chain(src1_ref);

    if (def_list->is_empty())
        return false;
    for (List_iterator<El_ref> liter(*def_list);  liter!=0; liter++) {
        El_ref def = *liter;
        if (def.get_ref_type() != EXP_DEST)
            return false;
        List<El_ref>* use_list = rdinfo->get_du_chain(def);
        if (use_list->size() != 1) 
            return false;
    }        

    // Make sure that all uses of the dest of op(r1 in above example),
    // is reached only by this dest
    // Also, make sure that the use op is not in 2-operand format 
    El_ref dest_ref(op, &(op->dest(DEST1)), EXP_DEST);
    List<El_ref> *use_list1 = rdinfo->get_du_chain(dest_ref);
    for (List_iterator<El_ref> liter2(*use_list1); liter2!=0; liter2++) {
        El_ref use = *liter2;
        // if a use is outside rdinfo's scope, ignore
        if ((use.get_ref_type() != PRED_SRC) &&
            (use.get_ref_type() != EXP_SRC))
            return false;
        if (Codegen::in_two_operand_format(use.get_op()))
            return false;
        List<El_ref> *def_list_1 = rdinfo->get_ud_chain(use);
        if (def_list_1->size() != 1)
            return false;
    }	

    // Try if src1 can be in dest1's position. i.e will the resulting 2-operand
    // format r2 = r2 + r3 cause the dest's prf choices be a strict subset of its original
    // prf choice..this could result in the dest not having a valid alternative which
    // can support it
    if (!Codegen::prf_bind_replace(op, op->src(port), DATA_OUT, DEST1, uspc_ops))
        return false;

    // Make sure that the replaced src does not violate the prf choice decisions
    // the new src's prf choice after replace by op's src1 should not be empty
    // r1 = r2 + r3
    //  .
    //  = r1 //use of r1
    // ==>
    // r2 = r2 + r3
    // .
    // = r2
    List<El_ref> *use_list = rdinfo->get_du_chain(dest_ref);
    for (List_iterator<El_ref> liter1(*use_list); liter1!=0; liter1++) {
        Op *use_op = (*liter1).get_op();
        List_set<Op_descr *> use_uspc_ops;
        MDES_collect_op_descrs(el_opcode_to_string_map.value(use_op->opcode()), use_uspc_ops);
        if (!Codegen::prf_bind_replace(use_op, op->src(port), DATA_IN, 
                              (*liter1).get_port_num(), use_uspc_ops))
            return false;
    }

    return true;
}

// Try reusing the src operands for the dest, if it fails try swapping and reusing
// return false if both fails.
// i.e Convert
// r1 = r2 + r3 =>
// r2 = r2 + r3;
static bool reuse_srcoprnd(Op *op, Reaching_defs_info *rdinfo, List_set<Op_descr*>& uspc_ops)
{
    List<El_ref> dead_uses;
    List<El_ref> new_uses;

    if (!check_reuse1(op, rdinfo, uspc_ops, false)) {

        // reuse unsuccessful..so try swapping
        if (dbg(genrio,5)) 
            cdbg << "reuse_srcoprnd: cannot reuse src1 of op: " << op->id() 
                 << " ..so trying to swap..." << endl;
          
        // try swapping operands
        if (MDES_commutative(el_opcode_to_string_map.value(op->opcode())) && (op->num_srcs() == 2)) {
            Operand src1 = op->src(SRC1);
            Operand src2 = op->src(SRC2);

            Op *new_op = Codegen::clone_op(op);
            new_op->set_src(SRC1, src2);
            new_op->set_src(SRC2, src1);

            Physfile_binding_attr *new_prf_battr = new Physfile_binding_attr();
            set_prf_binding_attr(new_op, new_prf_battr);
            Codegen::annotate_prf_port(DATA_IN, SRC1, new_op->src(SRC1), new_prf_battr, uspc_ops, new_op);
            Codegen::annotate_prf_port(DATA_IN, SRC2, new_op->src(SRC2), new_prf_battr, uspc_ops, new_op);

            // if swap results in either of the src having its prf choices
            // empty or replacing SRC1 in SRC2's position or vice-versa violates the superset condition
	    if ((new_prf_battr->src(SRC1)).is_empty() || 
	        (new_prf_battr->src(SRC2)).is_empty() || 
                (!Codegen::prf_bind_replace(new_op, new_op->src(SRC1), DATA_IN, SRC2, uspc_ops)) ||
                (!Codegen::prf_bind_replace(new_op, new_op->src(SRC2), DATA_IN, SRC1, uspc_ops)))
                return false;

            remove_prf_binding_attr(new_op);
            delete new_op;

            // reuse unsuccessful for swapped op
            if (!check_reuse1(op, rdinfo, uspc_ops, true)) {
                if(dbg(genrio, 3))
                  cdbg << "do_two_operand: reuse unsuccessful!" << endl;
                return false;
            } else {

                if (dbg(genrio,5)) 
                    cdbg << "do_two_oprnd: swapping op: " << op->id() << " for reuse.." << endl;
                // reuse is posssible..so do the actual swap
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
                Codegen::annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op);
                Codegen::annotate_prf_port(DATA_IN, SRC2, op->src(SRC2), prf_battr, uspc_ops, op);
            }
        } else
            return false;

    }

    if (dbg(genrio,5)) 
        cdbg << "do_two_oprnd: reuse_srcoprnd: Found op: " << op->id()
             << " whose src1 can be reused..." << endl;

    // Do the following:
    // r1 = r2 + r3
    //  .
    //  = r1 //use of r1
    // ==>
    // r2 = r2 + r3
    // .
    // = r2
    El_ref dest_ref(op, &(op->dest(DEST1)), EXP_DEST);
    List<El_ref> *use_list = rdinfo->get_du_chain(dest_ref);

    // Change the use(r1 to r2)
    for (List_iterator<El_ref> liter1(*use_list); liter1!=0; liter1++) {
        El_ref use = *liter1;
        Op *use_op = use.get_op();
        Operand new_src_oprnd(op->src(SRC1));
        use_op->set_src(use.get_port_num(), new_src_oprnd);

        List_set<Op_descr*> use_uspc_ops;
        MDES_collect_op_descrs(el_opcode_to_string_map.value(use_op->opcode()), use_uspc_ops);

        Codegen::annotate_prf_port(DATA_IN, use.get_port_num(), use.get_operand(), 
                                   get_prf_binding_attr(use_op), use_uspc_ops, use_op); 
        Physfile_binding_attr *use_prf_battr = get_prf_binding_attr(use_op);
        assert(!use_prf_battr->src(use.get_port_num()).is_empty());
        dead_uses.add_tail(use);

        El_ref new_use_ref(use_op, &(use_op->src(use.get_port_num())), EXP_SRC);
        new_uses.add_tail(new_use_ref);

        if (dbg(genrio,3))
            cdbg << "reuse_srcoprnd: changing port: " << use.get_port_num() << " of op: " << use_op->id() << endl;
    }

    // change dest to src1
    Operand new_dest_oprnd(op->src(SRC1));
    op->set_dest(DEST1, new_dest_oprnd);
    Codegen::annotate_prf_port(DATA_OUT, DEST1, op->dest(DEST1),
                               get_prf_binding_attr(op), uspc_ops, op); 
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    assert(!prf_battr->dest(DEST1).is_empty());

    // Create new du-ud chains using the new uses and def
    El_ref new_dest_ref(op, &(op->dest(DEST1)), EXP_DEST);
    while (!(new_uses.is_empty())) {
        El_ref new_use_ref = new_uses.pop();
        rdinfo->add_du_ud_links(new_dest_ref, new_use_ref);
    }
    
    // Remove the old uses and define
    while(!(dead_uses.is_empty())) {
        El_ref dead_use = dead_uses.pop();
        rdinfo->remove_du_ud_links(dest_ref, dead_use);
    }

    // horrible hack! update liveness info
    // There must be some other low overhead way to do this
    delete_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(op->parent()));
    create_local_analysis_info_for_all_hbs_bbs(el_get_enclosing_procedure(op->parent()));
    el_flow_delete_liveness(el_get_enclosing_procedure(op->parent()));
    el_flow_compute_liveness(el_get_enclosing_procedure(op->parent()), ANALYZE_ALLREG);

    return true;
}

void Codegen::do_two_operand_op(Op *op, Reaching_defs_info *rdinfo, List_set<Op_descr*>& uspc_ops, bool post_cluster)
{
    if (Codegen::in_two_operand_format(op)) {
        if (dbg(genrio, 5))
            cdbg << "do_two_opernd: op: " << op->id() 
                 << " already in 2 operand format" << endl;
            return;
    } else {
        // try swapping the operands
        if (MDES_commutative(el_opcode_to_string_map.value(op->opcode())) && (op->num_srcs() == 2)) {
            Operand src1 = op->src(SRC1);
            Operand src2 = op->src(SRC2);

            Op *new_op = Codegen::clone_op(op);
            new_op->set_src(SRC1, src2);
            new_op->set_src(SRC2, src1);

            Physfile_binding_attr *new_prf_battr = new Physfile_binding_attr();
            set_prf_binding_attr(new_op, new_prf_battr);
            Codegen::annotate_prf_port(DATA_IN, SRC1, new_op->src(SRC1), new_prf_battr, uspc_ops, new_op);
            Codegen::annotate_prf_port(DATA_IN, SRC2, new_op->src(SRC2), new_prf_battr, uspc_ops, new_op);
            // set the dest prf attributes of new_op same as that of the original op
            for (int p=op->first_output(); p<=op->last_output(); p++) {
                if (op->dest(p).is_undefined())
                  continue;
                new_prf_battr->set_dest(p, (get_prf_binding_attr(op))->dest(p));
            }

            // if swap results in either of the src having its prf choices
            // empty or replacing SRC1 in SRC2's position or vice-versa violates the superset condition
            // ignore...else check the swapped op is in 2 operand format , if so do the swap..
            if (!(new_prf_battr->src(SRC1)).is_empty() &&
                !(new_prf_battr->src(SRC2)).is_empty() &&
                (Codegen::prf_bind_replace(new_op, new_op->src(SRC1), DATA_IN, SRC2, uspc_ops)) &&
                (Codegen::prf_bind_replace(new_op, new_op->src(SRC2), DATA_IN, SRC1, uspc_ops))) {

                if (Codegen::in_two_operand_format(new_op)) {
                    if (dbg(genrio, 5))
                        cdbg << "do_two_opernd: swapping op: " << op->id() 
                             << " to convert to 2 operand format" << endl;

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

                    return;
                }
            }

            remove_prf_binding_attr(new_op);
            delete new_op;
        }
    }

    // try reusing the src operands, else do 2-operand convert using moves
    if (!reuse_srcoprnd(op, rdinfo, uspc_ops))  ;
        do_two_oprnd_convert(op, uspc_ops, post_cluster);
}

// This routine is called in initial_codegen phase, prior to clustering
// All ops have been annotated with prf choices and moves inserted
// if the the annotations where empty.
// Any transformations by this routine should make sure that the above
// condition is not violated i.e annotations should never be null and 
// the prf choices of any replacement be equal or greater than original
// choice. The second condition is to ensure that a valid opdes does exist for
// this op. PRF choices should not be reduced as there is some valid opdes
// that can be satisfied by some PRF tuple among all possible PRF choices of its ports(cross product)
void Codegen::do_two_operand(Compound_region *r)
{
    if (dbg(genrio, 1))
        cdbg << "Doing two operand conversion on procedure "
             << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;

    delete_local_analysis_info_for_all_hbs_bbs(r);
    create_local_analysis_info_for_all_hbs_bbs(r);
    el_flow_compute_liveness(r, ANALYZE_ALLREG);
    El_do_reaching_defs (r, ANALYZE_ALLREG);
    Reaching_defs_info* rdinfo = get_reaching_defs_info (r);

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;

        if (is_pseudo(op) || is_no_op(op))
            continue;

        // Since this is called before clustering, we do not need to
        // get clustered unit_specific_ops 
        List_set<Op_descr *> unit_specific_opdes_set;
        MDES_collect_op_descrs(el_opcode_to_string_map.value(op->opcode()), 
		                   unit_specific_opdes_set);

        // only if two_operand flag is set for this op in MDES
        if (MDES_two_operand(el_opcode_to_string_map.value(op->opcode()))) {
            // assert(op->num_dests() == 1);

            do_two_operand_op(op, rdinfo, unit_specific_opdes_set, false);
        }

    }

    delete_local_analysis_info_for_all_hbs_bbs(r);
    remove_reaching_defs_info(r);
}
            

