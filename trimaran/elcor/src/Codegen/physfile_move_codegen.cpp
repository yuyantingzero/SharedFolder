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
//      File:           physfile_move_codegen.cpp                        //
//      Authors:        Rajiv Ravindran                                  //
//      Created:        April 2002                                       //
//      Description:    inserts moves between incompatible defs          //
//                      and use                                          //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "codegen.h"

static Op *get_movpp_if_necessary(Op *mov_op, bool post_cluster)
{ 
    List_set<Op_descr*> clust_uspc_ops;

    int cid;
    if (post_cluster == true) {
        assert(Cluster_mgr::op_map.is_bound(mov_op));
        cid = Cluster_mgr::op_map.value(mov_op);
        Codegen::get_uspc_ops_in_cluster(mov_op, cid, clust_uspc_ops);
        assert(!clust_uspc_ops.is_empty());
    } else {
        MDES_collect_op_descrs(el_opcode_to_string_map.value(mov_op->opcode()), clust_uspc_ops);
    }
 
    Physfile_binding_attr dummy_pattr;
    Codegen::annotate_prf_port(DATA_IN, PRED1, mov_op->src(PRED1), &dummy_pattr, clust_uspc_ops, mov_op);

    Hash_set<eString> home_prf_set(hash_estring, 128);
    if (mov_op->src(PRED1).is_macro_reg()) {
        assert(Codegen::Macro_Home_Set.is_bound(mov_op->src(PRED1).name()));
        home_prf_set = Codegen::Macro_Home_Set.value(mov_op->src(PRED1).name());
    } else if (mov_op->src(PRED1).is_reg()) { 
        assert(Codegen::Vr_Home_Set.is_bound(mov_op->src(PRED1).vr_num()));
        home_prf_set = Codegen::Vr_Home_Set.value(mov_op->src(PRED1).vr_num());
    }

    Physfile_binding_attr *mov_op_prfattr = get_prf_binding_attr(mov_op);
    assert(mov_op_prfattr != 0);

    Hash_set<eString> prf_set(hash_estring, 1024);
    prf_set = dummy_pattr.src(PRED1);
    Hash_set<eString> temp_set(hash_estring, 1024);
    temp_set = prf_set;
    temp_set *= home_prf_set;
    if (!temp_set.is_empty()) {
        mov_op_prfattr->set_src(PRED1, prf_set);
        return 0;
    } else {
        Op* mov_pp_op = new Op(MOVEPP);
        Operand pred_true(new Pred_lit(true));
        mov_pp_op->set_src(PRED1, pred_true);
        Operand new_oprnd(new Reg(EL_STANDARD_PREDICATE));

        mov_pp_op->set_dest(DEST1, new_oprnd);
        mov_pp_op->set_src(SRC1, mov_op->src(PRED1));
        mov_op->set_src(PRED1, new_oprnd);

        Codegen::Vr_Home_Set.bind(new_oprnd.vr_num(), prf_set);

        mov_op_prfattr->set_src(PRED1, prf_set);
      
        Physfile_binding_attr *movpp_prfattr = new Physfile_binding_attr();
        movpp_prfattr->set_src(SRC1, home_prf_set);
        movpp_prfattr->set_dest(DEST1, prf_set);
        set_prf_binding_attr(mov_pp_op, movpp_prfattr);

        if (post_cluster == true) {
          Cluster_mgr::op_map.bind(mov_pp_op, cid);
          Cluster_mgr::operand_map.bind(new_oprnd.vr_num(), cid);
        }

        Codegen::prf_move_ops++;

        return mov_pp_op;
    }
}

// Given a set of refs, return the prf set of the best ref
static El_ref get_best_ref_prf_set(Hash_set<El_ref>& ref_set, Hash_set<eString>& home_prf_set)
{
    Hash_map<El_ref, Hash_set<eString> > ref_to_prf_set(hash_El_ref, 32);

    // Create a map of ref to <set of prfs> that support that ref

    if (dbg(genrio,7))
        cdbg << endl << "do_physfile_move_codegen: ref to prf set: " << endl;
    for (Hash_set_iterator<El_ref> h1(ref_set); h1!=0; h1++) {
        El_ref ref = *h1;
        if (dbg(genrio,7))
            cdbg << endl << "ref: " << ref; 
        Port_num port = ref.get_port_num();
        Op *op = ref.get_op();
        Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
        Hash_set<eString> prf_set(hash_estring, 128);
        if (((ref.get_ref_type()) == PRED_SRC) || ((ref.get_ref_type()) == EXP_SRC)) {
            prf_set = prf_battr->src(port);
            ref_to_prf_set.bind(ref, prf_set);
        } else if ((ref.get_ref_type()) == EXP_DEST) {
            prf_set = prf_battr->dest(port);
            ref_to_prf_set.bind(ref, prf_set);
        }
        if (dbg(genrio,7)) {
            for (Hash_set_iterator<eString> iter(prf_set); iter!=0; iter++) 
                cdbg << " " << *iter;
        }
    }

    // Find the total # of occurances of each prf from the above created map
    Hash_map<eString, int> prf_count(hash_estring, 32);

    for (Hash_map_iterator<El_ref, Hash_set<eString> > h2(ref_to_prf_set); h2!=0; h2++) {
        Hash_set<eString>& prf_set = (*h2).second;
        for (Hash_set_iterator<eString> prfi(prf_set); prfi!=0; prfi++) {
            eString prf = *prfi;
            if (!prf_count.is_bound(prf))
                prf_count.bind(prf, 1);
            else
                (prf_count.value(prf))++;
        }
    }
    if (dbg(genrio,7)) {
        cdbg << endl << "do_physfile_move_codegen: prf_count " << endl;
        for (Hash_map_iterator<eString, int> iter(prf_count); iter!=0; iter++) 
            cdbg << "prf: " << (*iter).first << " count: " << (*iter).second << endl;
    }

    // find the cost of each ref; cost of a ref = sum of the number of occurances of each prf that support
    // that ref
    Hash_map<El_ref, int> ref_count(hash_El_ref, 32);

    for (Hash_map_iterator<El_ref, Hash_set<eString> > h3(ref_to_prf_set); h3!=0; h3++) {
        El_ref ref = (*h3).first;
        ref_count.bind(ref, 0);
        Hash_set<eString>& prf_set = (*h3).second;
        for (Hash_set_iterator<eString> prfi(prf_set); prfi!=0; prfi++) {
            eString prf = *prfi;
            assert(prf_count.is_bound(prf));
            (ref_count.value(ref)) += prf_count.value(prf);
        }
    }
    if (dbg(genrio,7)) {
        for (Hash_map_iterator<El_ref, int> iter(ref_count); iter!=0; iter++) 
            cdbg << "ref: " << (*iter).first << " prf: " << ref_to_prf_set.value((*iter).first) 
                 << " count: " << (*iter).second << endl;
    }

    // find the ref with the max cost,and return the prf set that supports it
    int max = 0;
    El_ref home_prf_ref;
    for (Hash_map_iterator<El_ref, int> h4(ref_count); h4!=0; h4++) {
        if ((*h4).second > max) {
            max = (*h4).second;
            home_prf_ref = (*h4).first;
        }
    }

    assert(ref_to_prf_set.is_bound(home_prf_ref));
    home_prf_set = ref_to_prf_set.value(home_prf_ref);

    return home_prf_ref;
}

// Given a ref set(for a vrnum) and a home prf set (for that vrnum), 
// insert moves if the intersect of the refs choices with home prf set is null
static void insert_moves_if_incompatible(Hash_set<El_ref>& ref_set, Hash_set<eString>& home_prf_set, 
                                         bool post_cluster)
{
    if (dbg(genrio,7))
        cdbg  << endl << "insert_du_ud_moves..."; 

    for (Hash_set_iterator<El_ref> refi(ref_set); refi!=0; refi++) {
        El_ref ref = *refi;
        Operand oprnd = ref.get_operand();
        Port_num port = ref.get_port_num();
        Op *op = ref.get_op();
        Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);

        Hash_set<eString> prf_set(hash_estring, 128);
        // process a use...
        if (((ref.get_ref_type()) == PRED_SRC) || ((ref.get_ref_type()) == EXP_SRC)) {

            prf_set = prf_battr->src(port);

            if (dbg(genrio,7)) {
                cdbg << endl << "ref: " << ref << " prf_set: "; 
                for (Hash_set_iterator<eString> iter(prf_set); iter!=0; iter++) 
                    cdbg << " " << *iter;
                cdbg << " home_set: ";
                for (Hash_set_iterator<eString> iter1(home_prf_set); iter1!=0; iter1++) 
                    cdbg << " " << *iter1;
            }
         
            Hash_set<eString> tmp_set(hash_estring, 1024);
            tmp_set = prf_set;
            tmp_set *= home_prf_set;

            // no common files between ref's prf_set and home_prf_set
            if (tmp_set.is_empty()) {

                // r1 = r2 + r3 is converted to
                // r1000 = r2                
                // r1 = r1000 + r3

                // insert move for src
                Opcode opc = get_move_opcode_for_operand(ref.get_operand());
                Op *mov_op = new Op(opc);
                mov_op->set_src(SRC1, ref.get_operand());
                Operand new_oprnd = new Reg(ref.get_operand().dtype());
                Codegen::Vr_Home_Set.bind(new_oprnd.vr_num(), prf_set);
                mov_op->set_dest(DEST1, new_oprnd);
                op->set_src(port, new_oprnd);
                if (op->flag(EL_OPER_LIVEOUT_DEST))
                    mov_op->set_flag(EL_OPER_LIVEOUT_DEST);
                if (op->flag(EL_OPER_SPILL_CODE))
                    mov_op->set_flag(EL_OPER_SPILL_CODE);
                if (op->flag(EL_OPER_CALLER_SAVE))
                    mov_op->set_flag(EL_OPER_CALLER_SAVE);
                if (op->flag(EL_OPER_CALLEE_SAVE))
                    mov_op->set_flag(EL_OPER_CALLEE_SAVE);
                mov_op->set_flag(EL_OPER_CGEN_MOVE);

                int cid;
                if (post_cluster == true) {
                    // add the newly inserted mov op to the current op's cluster
                    // if the current op is an icmove add the newly inserted mov op
                    // to the src (of the icmove) operand's cluster.
                    // assert that the mov_op is supported in that cluster

                    if (op->flag(EL_OPER_ICMOVE)) {
                        assert(Cluster_mgr::operand_map.is_bound(oprnd.vr_num()));
                        cid = Cluster_mgr::operand_map.value(oprnd.vr_num());
                    } else {
                        assert(Cluster_mgr::op_map.is_bound(op));
                        cid = Cluster_mgr::op_map.value(op);
                    }
                    assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_op));
                    Cluster_mgr::op_map.bind(mov_op, cid);
                    Cluster_mgr::operand_map.bind(new_oprnd.vr_num(), cid);
                }

                // create prf_attr for mov op
                Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
                mov_prf_battr->set_src(SRC1, home_prf_set);
                mov_prf_battr->set_dest(DEST1, prf_set);
                set_prf_binding_attr(mov_op, mov_prf_battr);

                Op *movpp_op=0;
                Operand pred_true(new Pred_lit(true));
		if (!op->predicated() || (port == PRED1) || (op->predicated() && op->src(PRED1).is_predicate_true())) {
                  mov_op->set_src(PRED1, pred_true);

                  // Note: move op is not annotated using the MDES, instead it is annotated
                  // using the prf choices of the producer and consumer refs
                  // We need to check whether the resulting annotation is a valid, i.e does the
                  // MDES support moves between the src and dest prf choices
                  // For now, there is a post bind check in do_prf_bind(check_for_valid_bind)
                  // in regfile_bind.cpp

                  if (dbg(genrio, 3))
                    cdbg << endl << "do_physfile_move_codegen: Inserting mov op: " << mov_op->id() 
                         << "for port: " << port << " in op: " << op->id() << endl;

               
                } else if (port != PRED1 && op->predicated() &&
                           !op->src(PRED1).is_predicate()) {
                    mov_op->set_src(PRED1, op->src(PRED1));

                    movpp_op = get_movpp_if_necessary(mov_op, post_cluster);
                }
                   
                El_insert_op_before(mov_op, op);
                if (movpp_op) {
                  El_insert_op_before(movpp_op, mov_op);
                  if (dbg(genrio, 1))
                    cdbg << "physfile_move_codegen: Inserting movpp op: " << movpp_op->id()
                         << "for mov op: " << mov_op->id() << endl;
                }

                Codegen::prf_move_ops++;
            }

        // process the def
        } else if ((ref.get_ref_type()) == EXP_DEST) {

            prf_set = prf_battr->dest(port);

            if (dbg(genrio,7)) {
                cdbg << endl << "ref: " << ref << " prf_set: "; 
                for (Hash_set_iterator<eString> iter(prf_set); iter!=0; iter++) 
                    cdbg << " " << *iter;
                cdbg << " home_set: ";
                for (Hash_set_iterator<eString> iter1(home_prf_set); iter1!=0; iter1++) 
                    cdbg << " " << *iter1;
            }

            Hash_set<eString> tmp_set(hash_estring, 1024);
            tmp_set = prf_set;
            tmp_set *= home_prf_set;

            // no common files between ref's prf_set and home_prf_set
            if (tmp_set.is_empty()) {
                
                // Convert r1 = r2 + r3 to
                // r1000 = r2 + r3
                // r1 = r1000

                // insert move for dest
                Opcode opc = get_move_opcode_for_operand(ref.get_operand());
                Op *mov_op = new Op(opc);
                mov_op->set_dest(DEST1, ref.get_operand());
                Operand new_oprnd = new Reg(ref.get_operand().dtype());
                Codegen::Vr_Home_Set.bind(new_oprnd.vr_num(), prf_set);
                op->set_dest(port, new_oprnd);
                if (op->flag(EL_OPER_LIVEOUT_DEST))
                    mov_op->set_flag(EL_OPER_LIVEOUT_DEST);
                if (op->flag(EL_OPER_SPILL_CODE))
                    mov_op->set_flag(EL_OPER_SPILL_CODE);
                if (op->flag(EL_OPER_CALLER_SAVE))
                    mov_op->set_flag(EL_OPER_CALLER_SAVE);
                if (op->flag(EL_OPER_CALLEE_SAVE))
                    mov_op->set_flag(EL_OPER_CALLEE_SAVE);
                mov_op->set_flag(EL_OPER_CGEN_MOVE);

                int cid;
                if (post_cluster == true) {
                    // add the newly inserted mov op to the current op's cluster
                    // if the current op is an icmove add the newly inserted mov op
                    // to the destination (of the icmove) operand's cluster.
                    // assert that the mov_op is supported in that cluster

                    if (op->flag(EL_OPER_ICMOVE)) {
                        assert(Cluster_mgr::operand_map.is_bound(oprnd.vr_num()));
                        cid = Cluster_mgr::operand_map.value(oprnd.vr_num());
                    } else {
                        assert(Cluster_mgr::op_map.is_bound(op));
                        cid = Cluster_mgr::op_map.value(op);
                    }
                    assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_op));
                    Cluster_mgr::op_map.bind(mov_op, cid);
                    Cluster_mgr::operand_map.bind(new_oprnd.vr_num(), cid);
                }


                // create prf_attr for mov op
                Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
                mov_prf_battr->set_dest(DEST1, home_prf_set);
                set_prf_binding_attr(mov_op, mov_prf_battr);

                Operand pred_true(new Pred_lit(true));
                Op *pred_clear_op = 0;
                Op *movpp_op = 0;

                if (is_U_pred(op, port)) {
                  mov_op->set_src(PRED1, pred_true);
                  mov_op->set_src(SRC1, new_oprnd);
                  Physfile_binding_attr *p1 = get_prf_binding_attr(mov_op);
                  p1->set_src(SRC1, prf_set);
                } else if (is_O_pred(op, port)) {
                  mov_op->set_src(PRED1, new_oprnd);
                  mov_op->set_src(SRC1, pred_true);
                  Physfile_binding_attr *p1 = get_prf_binding_attr(mov_op);
                  p1->set_src(PRED1, prf_set);

                  // Insert predicate clear
                  pred_clear_op = new Op(PRED_CLEAR);
                  pred_clear_op->set_dest(DEST1, new_oprnd);
                  El_insert_op_after_merge(op->parent(), pred_clear_op);
                  Physfile_binding_attr *pclear_battr = new Physfile_binding_attr();
                  pclear_battr->set_dest(DEST1, prf_set);
                  set_prf_binding_attr(pred_clear_op, pclear_battr);

                  if (post_cluster)
                    Cluster_mgr::op_map.bind(pred_clear_op, cid);
                } else {
                    Physfile_binding_attr *p1 = get_prf_binding_attr(mov_op);
                    p1->set_src(SRC1, prf_set);
                    mov_op->set_src(SRC1, new_oprnd);
                    if (op->predicated())
                      mov_op->set_src(PRED1, op->src(PRED1));
                    else
                      mov_op->set_src(PRED1, pred_true);

                    if (!mov_op->src(PRED1).is_predicate()) {
                      movpp_op = get_movpp_if_necessary(mov_op, post_cluster);
                    }
                }

                if (dbg(genrio, 1)) {
                    cdbg << endl << "do_physfile_move_codegen: Inserting mov op: " 
                         << mov_op->id() << "for port: " << port << " in op: " 
                         << op->id() << endl;
                    if (movpp_op)
                      cdbg << "physfile_move_codegen: Inserting movpp op: " << movpp_op->id() 
                           << "for mov op: " << mov_op->id() << endl;
                }

                if (is_brf(op))
                    El_insert_op_after_merge(op->parent(), mov_op);
                else
                    El_insert_op_after(mov_op, op);

                if (movpp_op)
                    El_insert_op_before(movpp_op, mov_op);

                Codegen::prf_move_ops++;
            }
        }
    }
}

// For region 'r', return vr_num to <set of refs> for register
// and macro_name to <set of refs> for macros
void Codegen::get_ref_set_for_region(Compound_region *r, 
                                   Hash_map<int, Hash_set<El_ref> >& vr_ref_set,
                                   Hash_map<Macro_name, Hash_set<El_ref> >& macro_ref_set)
{
    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;

        if (is_pseudo(op) || is_no_op(op))
            continue;

        assert(get_prf_binding_attr(op) != 0);

        // create a map of vr_num to <set of refs(of that vr_num)>
        for (Op_explicit_inputs srci(op); srci!=0; srci++) {
            Operand& src_oprnd = *srci;
            El_ref ref = srci.get_ref();

            if (src_oprnd.is_reg()) {
                if (vr_ref_set.is_bound(src_oprnd.vr_num())) 
                    (vr_ref_set.value(src_oprnd.vr_num())) += ref;
                else {
                    Hash_set<El_ref> refset(hash_El_ref, 1024);
                    refset += ref;
                    vr_ref_set.bind(src_oprnd.vr_num(), refset);
                }
            } else if (src_oprnd.is_macro_reg()) {
                if (macro_ref_set.is_bound(src_oprnd.name())) 
                    (macro_ref_set.value(src_oprnd.name())) += ref;
                else {
                    Hash_set<El_ref> refset(hash_El_ref, 1024);
                    refset += ref;
                    macro_ref_set.bind(src_oprnd.name(), refset);
                }
            }
        }

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
            Operand& dest_oprnd = *desti;
            El_ref ref = desti.get_ref();

            if (dest_oprnd.is_reg()) {
                if (vr_ref_set.is_bound(dest_oprnd.vr_num())) 
                    (vr_ref_set.value(dest_oprnd.vr_num())) += ref;
                else {
                    Hash_set<El_ref> refset(hash_El_ref, 1024);
                    refset += ref;
                    vr_ref_set.bind(dest_oprnd.vr_num(), refset);
                }
            } else if (dest_oprnd.is_macro_reg()) {
                if (macro_ref_set.is_bound(dest_oprnd.name())) 
                    (macro_ref_set.value(dest_oprnd.name())) += ref;
                else {
                    Hash_set<El_ref> refset(hash_El_ref, 1024);
                    refset += ref;
                    macro_ref_set.bind(dest_oprnd.name(), refset);
                }
            }
        }
    }

    if (dbg(genrio,7)) {
        cdbg << "get_ref_set_for_region:.." << endl;
        for (Hash_map_iterator<int, Hash_set<El_ref> > hiter1(vr_ref_set); hiter1!=0; hiter1++) {
            Pair<int, Hash_set<El_ref> > vr_num_ref = *hiter1;
            int vr_num = vr_num_ref.first;
            Hash_set<El_ref> ref_set(hash_El_ref, 1024);
            ref_set = vr_num_ref.second;
            cdbg << endl << "vr_num: " << vr_num << " Refs: ";
            for (Hash_set_iterator<El_ref> iter(ref_set); iter!=0; iter++) 
                cdbg << " " << *iter;
        }
        for (Hash_map_iterator<Macro_name, Hash_set<El_ref> > hiter2(macro_ref_set); hiter2!=0; hiter2++) {
            Pair<Macro_name, Hash_set<El_ref> > macro_name_ref = *hiter2;
            Macro_name mn = macro_name_ref.first;
            Hash_set<El_ref> ref_set(hash_El_ref, 1024);
            ref_set = macro_name_ref.second;
            cdbg << endl << "macro_name: " << macro_reg_to_text(mn) << "Refs: ";
            for (Hash_set_iterator<El_ref> iter(ref_set); iter!=0; iter++) 
                cdbg << " " << *iter;
        }
    }
}

#if 0
static void swap_if_du_incompatible(Compound_region *r, 
                                   Hash_map<int, Hash_set<El_ref> >& vr_ref_set,
                                   Hash_map<Macro_name, Hash_set<El_ref> >& macro_ref_set,
                                   bool post_cluster)
{
    Hash_map<int, Hash_set<eString> > vr_num_to_home_set(hash_int, 32);
    Hash_map<Macro_name, Hash_set<eString> > macro_name_to_home_set(Codegen::hash_macro_name, 32);

    // For VR and macros, find the home prf set. Either the VR/macro has a home set
    // from some previous region binding or compute the home set from the list of refs
    // for that VR/macro
    // If intersection of prf_set for VR/macro ref with the home prf set of that VR_num/macro_name
    // is empty, then try swapping the src operands (if it is not two_operand and is commutative)
    // If swapping creates a non-null intersect for both the sources, then do the actual swap

    // For VRs...
    for (Hash_map_iterator<int, Hash_set<El_ref> > hiter1(vr_ref_set); hiter1!=0; hiter1++) {
        Pair<int, Hash_set<El_ref> > vr_num_ref = *hiter1;
        int vr_num = vr_num_ref.first;
        Hash_set<El_ref> ref_set = vr_num_ref.second;

        Hash_set<eString> home_prf_set;
            
        // if vr_num does not have a home set, find the ref for that vr_num from its web
        // which has the max cost.
        if (!Codegen::Vr_Home_Set.is_bound(vr_num)) {
            El_ref best_ref = get_best_ref_prf_set(ref_set, home_prf_set);

            if (dbg(genrio, 7)) {
                cdbg << "swap_if_du_incompatible: best home_prf_set for ref: " << best_ref;
                for (Hash_set_iterator<eString> h(home_prf_set); h!=0; h++)
                    cdbg << " " << *h;
            }
        } else {
            home_prf_set = Codegen::Vr_Home_Set.value(vr_num);
            if (dbg(genrio, 7)) {
                cdbg << endl << "swap_if_du_incompatible: bound home_prf_set for vr: " << vr_num; 
                for (Hash_set_iterator<eString> h(home_prf_set); h!=0; h++)
                    cdbg << " " << *h;
            }
        }
        vr_num_to_home_set.bind(vr_num, home_prf_set);
    }

    // For Macros...
    for (Hash_map_iterator<Macro_name, Hash_set<El_ref> > hiter2(macro_ref_set); hiter2!=0; hiter2++) {
        Pair<Macro_name, Hash_set<El_ref> > macro_name_ref = *hiter2;
        Macro_name mn = macro_name_ref.first;
        Hash_set<El_ref> ref_set = macro_name_ref.second;

        Hash_set<eString> home_prf_set;
            
        // if mn does not have a home set, find the ref for that mn from its web
        // which has the max cost.
        if (!Codegen::Macro_Home_Set.is_bound(mn)) {
            El_ref best_ref = get_best_ref_prf_set(ref_set, home_prf_set);

            if (dbg(genrio, 7)) {
                cdbg << endl << "swap_if_du_incompatible: best home_prf_set for ref: "; 
                for (Hash_set_iterator<eString> h(home_prf_set); h!=0; h++)
                    cdbg << " " << *h;
            }

            Codegen::Macro_Home_Set.bind(mn, home_prf_set);

        } else {
            home_prf_set = Codegen::Macro_Home_Set.value(mn);
            if (dbg(genrio, 7)) {
                cdbg << endl << "swap_if_du_incompatible: bound home_prf_set for mn: " << macro_reg_to_text(mn);
                for (Hash_set_iterator<eString> h(home_prf_set); h!=0; h++)
                    cdbg << " " << *h;
            }
        }
        macro_name_to_home_set.bind(mn, home_prf_set);
    }

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || MDES_two_operand(el_opcode_to_string_map.value(op->opcode()))
            || !MDES_commutative(el_opcode_to_string_map.value(op->opcode())))
            continue;

        List_set<Op_descr*> uspc_ops;
        if (post_cluster == true) {
            assert(Cluster_mgr::op_map.is_bound(op));
            int cid = Cluster_mgr::op_map.value(op);
            Codegen::get_uspc_ops_in_cluster(op, cid, uspc_ops);
        } else 
            MDES_collect_op_descrs(el_opcode_to_string_map.value(op->opcode()), uspc_ops);

        // Try intersecting the prf choices of each src with their corresponding home set.
        // If either of them is empty, try swapping the src operands.
        Operand src_oprnd_1 = op->src(SRC1);
        Operand src_oprnd_2 = op->src(SRC2);
        Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
        assert(prf_battr != 0);
        Hash_set<eString> prf_set_1 = prf_battr->src(SRC1);
        Hash_set<eString> prf_set_2 = prf_battr->src(SRC2);
        assert(!prf_set_1.is_empty()); assert(!prf_set_2.is_empty());

        Hash_set<eString> home_prf_set_1;
        Hash_set<eString> home_prf_set_2;
        if (src_oprnd_1.is_reg()) {
            int vr_num = src_oprnd_1.vr_num();
            assert(vr_num_to_home_set.is_bound(vr_num));
            home_prf_set_1 = vr_num_to_home_set.value(vr_num);
        } else if (src_oprnd_1.is_macro_reg()) {
            Macro_name mn = src_oprnd_1.name();
            assert(macro_name_to_home_set.is_bound(mn));
            home_prf_set_1 = macro_name_to_home_set.value(mn);
        }
        if (src_oprnd_2.is_reg()) {
            int vr_num = src_oprnd_2.vr_num();
            assert(vr_num_to_home_set.is_bound(vr_num));
            home_prf_set_2 = vr_num_to_home_set.value(vr_num);
        } else if (src_oprnd_2.is_macro_reg()) {
            Macro_name mn = src_oprnd_2.name();
            assert(macro_name_to_home_set.is_bound(mn));
            home_prf_set_2 = macro_name_to_home_set.value(mn);
        }
            
        Hash_set<eString> tmp_set_1(prf_set_1);
        Hash_set<eString> tmp_set_2(prf_set_2);

        tmp_set_1 *= home_prf_set_1; tmp_set_2 *= home_prf_set_2;
        if (tmp_set_1.is_empty() || tmp_set_2.is_empty()) {
            
           Reference_dtype_attr *rdt_attr;
           Op *new_op = op->clone();
           new_op->set_src(SRC1, src_oprnd_2);
           new_op->set_src(SRC2, src_oprnd_1);
           rdt_attr = get_reference_dtype_attr(new_op);
           if (rdt_attr != NULL) {
               rdt_attr->swap_srcs(SRC1, SRC2);
           }

           Physfile_binding_attr *new_prf_battr = new Physfile_binding_attr();
           Codegen::annotate_prf_port(DATA_IN, SRC1, new_op->src(SRC1), new_prf_battr, uspc_ops, new_op);
           Codegen::annotate_prf_port(DATA_IN, SRC2, new_op->src(SRC2), new_prf_battr, uspc_ops, new_op);

           Hash_set<eString> swap_prf_set_1 = new_prf_battr->src(SRC1);
           Hash_set<eString> swap_prf_set_2 = new_prf_battr->src(SRC2);
           // swap_prf_set_1, swap_prf_set_2 contains the prf choices for SRC1, SRC2 for the swapped op
           // We need to make sure that these sets are not empty
           if (swap_prf_set_1.is_empty() || swap_prf_set_2.is_empty())
               continue;

           // After swapping, if the intersects of the prf choices for the swapped operands
           // with the home prf set is not empty then do the actual swap
           Hash_set<eString> tmp_swap_set_1(swap_prf_set_1);
           Hash_set<eString> tmp_swap_set_2(swap_prf_set_2);
           tmp_swap_set_1 *= home_prf_set_1;
           tmp_swap_set_2 *= home_prf_set_2;
 
           delete new_prf_battr; delete new_op;

           if (tmp_swap_set_1.is_empty() || tmp_swap_set_2.is_empty())
               continue;
            
            // Swapping has resulted in non-null intersect, so do the actual swapping
            if (dbg(genrio,3))
                cdbg << "swap_if_du_incompatible: swapping op: " << op->id() << endl;

            op->set_src(SRC1, src_oprnd_2);
            op->set_src(SRC2, src_oprnd_1);
            rdt_attr = get_reference_dtype_attr(op);
            if (rdt_attr != NULL) {
                rdt_attr->swap_srcs(SRC1, SRC2);
            }
            Codegen::annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op);
            Codegen::annotate_prf_port(DATA_IN, SRC2, op->src(SRC2), prf_battr, uspc_ops, op);
            Physfile_binding_attr *swap_prf_battr = get_prf_binding_attr(op);
            assert (!swap_prf_battr->src(SRC1).is_empty()); assert(!swap_prf_battr->src(SRC2).is_empty());
        }
    }
}
#endif

// Inserts moves if def-use have incompatible register file choices
// Called at various points in codegen
// For a region, it first creates a home prf set for each distinct VR_num and macro

// Post register allocation, the home sets are already computed ..honour the earlier bindings..

void Codegen::do_physfile_move_codegen(Compound_region *r, bool post_cluster)
{
    Hash_map<int, Hash_set<El_ref> > vr_ref_set(hash_int, 32);
    Hash_map<Macro_name, Hash_set<El_ref> > macro_ref_set(hash_macro_name, 32);

    List<Triple<int, Hash_set<El_ref>, Hash_set<eString> > > vr_triple_list;
    List<Triple<Macro_name, Hash_set<El_ref>, Hash_set<eString> > > macro_triple_list;

    if (dbg(genrio, 1))
        cdbg << "Doing physical file move codegen on region: " << r->id() 
             << " in proc: " << (char*)(el_get_enclosing_procedure(r))->get_name()<< endl;

    // for each vr/macro, get the list of references within region r.
    get_ref_set_for_region(r, vr_ref_set, macro_ref_set);

    // swap_if_du_incompatible(r, vr_ref_set, macro_ref_set, post_cluster);
    // redo the ref_sets....
    // vr_ref_set.clear(); macro_ref_set.clear();
    // get_ref_set_for_region(r, vr_ref_set, macro_ref_set);

    // Create the home prf sets for VRs and Macros
    for (Hash_map_iterator<int, Hash_set<El_ref> > hiter1(vr_ref_set); hiter1!=0; hiter1++) {
        Pair<int, Hash_set<El_ref> > vr_num_ref = *hiter1;
        int vr_num = vr_num_ref.first;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = vr_num_ref.second;

        Hash_set<eString> home_prf_set(hash_estring, 1024);
            
        // if vr_num does not have a home set, find the ref for that vr_num from its web
        // which has the max cost.
        if (!Vr_Home_Set.is_bound(vr_num)) {
            El_ref best_ref = get_best_ref_prf_set(ref_set, home_prf_set);

            if (dbg(genrio, 7)) {
                cdbg << "do_physfile_move_codegen: best home_prf_set for ref: " << best_ref;
                for (Hash_set_iterator<eString> h(home_prf_set); h!=0; h++)
                    cdbg << " " << *h;
            }

            Vr_Home_Set.bind(vr_num, home_prf_set);

        } else {
            home_prf_set = Vr_Home_Set.value(vr_num);
            if (dbg(genrio, 7)) {
                cdbg << endl << "do_physfile_move_codegen: bound home_prf_set for vr: " << vr_num;
                for (Hash_set_iterator<eString> h(home_prf_set); h!=0; h++)
                    cdbg << " " << *h;
            }
        }

        Triple<int, Hash_set<El_ref>, Hash_set<eString> > triple(vr_num, ref_set, home_prf_set);
        vr_triple_list.add_tail(triple);
    }

    for (Hash_map_iterator<Macro_name, Hash_set<El_ref> > hiter2(macro_ref_set); hiter2!=0; hiter2++) {
        Pair<Macro_name, Hash_set<El_ref> > macro_name_ref = *hiter2;
        Macro_name mn = macro_name_ref.first;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = macro_name_ref.second;

        Hash_set<eString> home_prf_set(hash_estring, 1024);
            
        assert (Macro_Home_Set.is_bound(mn));
        home_prf_set = Macro_Home_Set.value(mn);
        if (dbg(genrio, 7)) {
          cdbg << endl << "do_physfile_move_codegen: bound home_prf_set for mn: ";
          for (Hash_set_iterator<eString> h(home_prf_set); h!=0; h++)
            cdbg << " " << *h;
        }

        Triple<Macro_name, Hash_set<El_ref>, Hash_set<eString> > triple(mn, ref_set, home_prf_set);
        macro_triple_list.add_tail(triple);
    }

    List_iterator<Triple<int, Hash_set<El_ref>, Hash_set<eString> > > liter1(vr_triple_list);
    List_iterator<Triple<Macro_name, Hash_set<El_ref>, Hash_set<eString> > > liter2(macro_triple_list);

    // vrs
    for(;liter1!=0; liter1++) {
        Hash_set<eString> home_prf_set = (*liter1).third;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = (*liter1).second;

        insert_moves_if_incompatible(ref_set, home_prf_set, post_cluster);
    }
    // Macro
    for(;liter2!=0; liter2++) {
        Hash_set<eString> home_prf_set = (*liter2).third;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = (*liter2).second;

        insert_moves_if_incompatible(ref_set, home_prf_set, post_cluster);
    }
}
