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
//      File:           post_bind_prf_move_codegen.cpp                   //
//      Authors:        Rajiv Ravindran                                  //
//      Created:        April 2002                                       //
//      Description:    Insert moves if def-use use physical register    //
//                      file bindings are incompatible                   //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "codegen.h"

static bool check_for_brf(Hash_set<El_ref>& ref_set, eString& home_prf, PRF_Bind_Map *prf_bind_map)
{
    for (Hash_set_iterator<El_ref> iter(ref_set); iter!=0; iter++) {
        El_ref ref = *iter;
        if (!ref.get_operand().is_reg() && !ref.get_operand().is_macro_reg())
            return false;
        if (ref.get_operand().file_type() != PR)
            return false;
        if (ref.get_ref_type() != EXP_DEST)
            continue;
        Op *op = ref.get_op();
        if (is_brf(op)) {
            assert(prf_bind_map->is_bound(ref.get_operand()));
            home_prf = prf_bind_map->get_prf(ref.get_operand());
            return true;
        } else 
            continue;
    }

    return false;
}

// Given a set of references, find the best prf for the home_prf
// look at prf_bind_map to decide the current bindings
static void get_best_bound_prf(Hash_set<El_ref>& ref_set, eString& home_prf, PRF_Bind_Map *prf_bind_map)
{
    // Find the total # of occurances of each prf among all the prfs that are annotated
    // at all refs in ref_set

    // Hack for BRFs
    // For BRFs the staging predicate's home is always that of the defining
    // BRF
    if (check_for_brf(ref_set, home_prf, prf_bind_map))
        return;

    Hash_map<eString, int> prf_count(hash_estring, 32);
    for (Hash_set_iterator<El_ref> h1(ref_set); h1!=0; h1++) {
        El_ref ref = *h1;
        Operand& oprnd = ref.get_operand();
        assert (prf_bind_map->is_bound(oprnd)); 
        eString prf = prf_bind_map->get_prf(oprnd);
        if (!prf_count.is_bound(prf))
            prf_count.bind(prf, 1);
        else
            (prf_count.value(prf))++;
    }
    if (dbg(genrio,5)) {
        cdbg << "In get best ref prf for vr/macro: " ;
        if (ref_set.head().get_operand().is_reg())
            cdbg << ref_set.head().get_operand().vr_num() << endl;
        else
            cdbg << ref_set.head().get_operand().name() << endl;
        for (Hash_map_iterator<eString, int> h2(prf_count); h2!=0; h2++)
            cdbg << "prf: " << (*h2).first << " count: " << (*h2).second << endl;
    }

    // find the prf with the max cost,and return that prf
    int max = 0;
    for (Hash_map_iterator<eString, int> h3(prf_count); h3!=0; h3++) {
        if ((*h3).second > max) {
            max = (*h3).second;
//          home_prf = (*h3).first;
        }
    }

    List<eString> tied_prf_list;
    for (Hash_map_iterator<eString, int> h4(prf_count); h4!=0; h4++) {
        if ((*h4).second == max)
            tied_prf_list.add_tail((*h4).first);
    }

    if (dbg(genrio,7)) {
        if (ref_set.head().get_operand().is_reg())
            cdbg << "Tied prf for vr: " << ref_set.head().get_operand().vr_num();
        else
            cdbg << "Tied prf for macro: " << ref_set.head().get_operand().name();
        for (List_iterator<eString> liter1(tied_prf_list); liter1!=0; liter1++)
            cdbg << " " << *liter1;
        cdbg << endl;
    }
    if (tied_prf_list.size() == 1) {
        home_prf = tied_prf_list.head();
    } else {
        int file = (rand() % tied_prf_list.size()) + 1;
        List_iterator<eString> liter2(tied_prf_list);
        for (int i=1;i<file;i++,liter2++)
            ;
        home_prf = (*liter2);
        if (dbg(genrio,7))
            cdbg << "returning random file: " << home_prf << endl;
    }
}


static Op* get_movpp_if_necessary(Op *mov_op, PRF_Bind_Map *prf_bind_map)
{
    List_set<Op_descr*> clust_uspc_ops;

    assert(Cluster_mgr::op_map.is_bound(mov_op));
    int cid = Cluster_mgr::op_map.value(mov_op);
    Codegen::get_uspc_ops_in_cluster(mov_op, cid, clust_uspc_ops);
    assert(!clust_uspc_ops.is_empty());	   

    Physfile_binding_attr *prf_battr = new Physfile_binding_attr();
    Hash_set<eString> src_prf_set1(hash_estring, 128);
    Hash_set<eString> dest_prf_set1(hash_estring, 128);
    src_prf_set1 += prf_bind_map->get_prf(mov_op->src(SRC1));
    dest_prf_set1 += prf_bind_map->get_prf(mov_op->dest(DEST1));
    prf_battr->set_src(SRC1, src_prf_set1);
    prf_battr->set_dest(DEST1, dest_prf_set1);
    Codegen::annotate_prf_port(DATA_IN, PRED1, mov_op->src(PRED1), prf_battr, clust_uspc_ops, mov_op);
    set_prf_binding_attr(mov_op, prf_battr);

    Hash_map<int, Pair<eString, int> > dummy_vr_pref(hash_int, 8);

    for (Op_explicit_operands oprndi(mov_op); oprndi!=0; oprndi++) {
      Operand& oprnd = *oprndi;
      if (oprnd.is_reg()) {
        Pair<eString, int> p(prf_bind_map->get_prf(oprnd), 1);
        dummy_vr_pref.bind(oprnd.vr_num(), p);
      }
    }

    int max_cost = 0, cost;
    Op_descr *best_opdes = 0;
    for (List_set_iterator<Op_descr*> iter(clust_uspc_ops); iter!=0; iter++) {
        Op_descr *opdes = *iter;
        cost = Codegen::get_opdes_cost(mov_op, opdes, dummy_vr_pref);
        if (cost > max_cost) {
            max_cost = cost;
            best_opdes = opdes;
        }
    }
    if (best_opdes==0) 
        El_punt("get_movpp_if_necessary: couldn't find an opdes to bind prfs of move op: %d", mov_op->id());
    if (dbg(genrio,3))
        cdbg << "get_movpp_if_necessary: Using best opdescr: " << best_opdes->get_aname_ptr() 
             << " for op: " << mov_op->id() << endl;

    
    List_set<Reg_descr*> src_regdescr_set;
    MDES_collect_reg_descrs(best_opdes->get_aname_ptr(), DATA_IN, PRED1, src_regdescr_set);
    Hash_set<eString> src_prf_set(hash_estring, 128);
    for (List_set_iterator<Reg_descr*> liter(src_regdescr_set); liter!=0; liter++)
      src_prf_set += (*liter)->get_aname();

    if (src_prf_set.is_member(prf_bind_map->get_prf(mov_op->src(PRED1)))) {
        Physfile_binding_attr *battr = get_prf_binding_attr(mov_op);
        Hash_set<eString> prf_set(hash_estring, 128);
        prf_set += prf_bind_map->get_prf(mov_op->src(PRED1));
        battr->set_src(PRED1, prf_set);
        return 0;
    } else {
        eString prf = prf_bind_map->get_prf(mov_op->src(PRED1));
        Op* mov_pp_op = new Op(MOVEPP);
        Operand pred_true(new Pred_lit(true));
        mov_pp_op->set_src(PRED1, pred_true);
        Operand new_oprnd(new Reg(EL_STANDARD_PREDICATE));

        mov_pp_op->set_dest(DEST1, new_oprnd);
        mov_pp_op->set_src(SRC1, mov_op->src(PRED1));
        mov_op->set_src(PRED1, new_oprnd);

        eString new_prf = src_prf_set.head();
       
        Codegen::Vr_Home_Prf.bind(new_oprnd.vr_num(), new_prf);
        Hash_set<eString> new_prf_set(hash_estring, 128);
        new_prf_set += new_prf;
        Codegen::Vr_Home_Set.bind(new_oprnd.vr_num(), new_prf_set);
	
        prf_bind_map->bind_operand(mov_pp_op->src(SRC1), prf);
        prf_bind_map->bind_operand(mov_pp_op->dest(DEST1), new_prf);
        prf_bind_map->bind_operand(mov_op->src(PRED1), new_prf);

        Physfile_binding_attr *battr1 = get_prf_binding_attr(mov_op);
        Hash_set<eString> prf_set1(hash_estring, 128);
        prf_set1 += new_prf;
        battr1->set_src(PRED1, prf_set1);

        Physfile_binding_attr *battr2 = new Physfile_binding_attr();
        Hash_set<eString> prf_src_set(hash_estring, 128);
        Hash_set<eString> prf_dest_set(hash_estring, 128);
        prf_src_set += prf;
        prf_dest_set += new_prf;
        battr2->set_src(SRC1, prf_src_set); 
        battr2->set_dest(DEST1, prf_dest_set);
        set_prf_binding_attr(mov_pp_op, battr2);

        assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_pp_op));
        Cluster_mgr::op_map.bind(mov_pp_op, cid);
        Cluster_mgr::operand_map.bind(new_oprnd.vr_num(), cid);                

        mov_pp_op->set_flag(EL_OPER_CGEN_MOVE);
        Codegen::bind_prf_move_ops++;

        return mov_pp_op;
    }
}

static void insert_moves_if_incompatible(Hash_set<El_ref>& ref_set, Hash_set<eString>& home_prf, 
                                         PRF_Bind_Map* prf_bind_map)
{
    for (Hash_set_iterator<El_ref> refi(ref_set); refi!=0; refi++) {
        El_ref ref = *refi;
        Operand oprnd = ref.get_operand();
        Port_num port = ref.get_port_num();
        Op *op = ref.get_op();

        assert(ref.get_operand().assigned_to_physical_file() ||
	       prf_bind_map->is_bound(ref.get_operand()));

        eString prf;
        if (ref.get_operand().assigned_to_physical_file())
            prf = ref.get_operand().physical_file_type();
        else
            prf = prf_bind_map->get_prf(ref.get_operand());

        // process a use...
        if (((ref.get_ref_type()) == PRED_SRC) || ((ref.get_ref_type()) == EXP_SRC)) {

            // bound prf 'prf' and home_prf do not match
            if (!home_prf.is_member(prf)) {

                if (dbg(genrio,7)) {
                    cdbg << "op: " << op->id() << " src port: " << port << endl;
                    cdbg << "prf: " << prf << " home prf: " << home_prf << endl;
                }

                // insert move for src
                Opcode opc = get_move_opcode_for_operand(ref.get_operand());
                Op *mov_op = new Op(opc);
                mov_op->set_src(SRC1, ref.get_operand());
                Reg *new_reg = new Reg(ref.get_operand().dtype(), ref.get_operand().file_type());
                new_reg->rename();
		
                Codegen::Vr_Home_Prf.bind(new_reg->vr_num(), prf);
                Hash_set<eString> prf_set(hash_estring, 128);
                prf_set += prf;
                Codegen::Vr_Home_Set.bind(new_reg->vr_num(), prf_set);
		
                Operand new_oprnd(new_reg);
                mov_op->set_dest(DEST1, new_oprnd);

                prf_bind_map->bind_operand(mov_op->src(SRC1), home_prf.head());
                prf_bind_map->bind_operand(mov_op->dest(DEST1), prf);		

                if (op->flag(EL_OPER_LIVEOUT_DEST))
                    mov_op->set_flag(EL_OPER_LIVEOUT_DEST);
                if (op->flag(EL_OPER_SPILL_CODE))
                    mov_op->set_flag(EL_OPER_SPILL_CODE);
                if (op->flag(EL_OPER_CALLER_SAVE))
                    mov_op->set_flag(EL_OPER_CALLER_SAVE);
                if (op->flag(EL_OPER_CALLEE_SAVE))
                    mov_op->set_flag(EL_OPER_CALLEE_SAVE);

                mov_op->set_flag(EL_OPER_CGEN_MOVE);

                // add the newly inserted mov op to the current op's cluster
                // if the current op is an icmove add the newly inserted mov op
                // to the source (of the icmove) operand's cluster.
                // assert that the mov_op is supported in that cluster
                int cid;
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

                Op *movpp_op=0;
                Operand pred_true(new Pred_lit(true));
                if (!op->predicated() || (port == PRED1) 
                    || (op->predicated() && op->src(PRED1).is_predicate_true())) {
                  mov_op->set_src(PRED1, pred_true);

                  // create a prf attr for the mov_op with prf choices same as the newly bound 
                  // prf
                  Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
                  Hash_set<eString> prf_set_src(hash_estring, 128);
                  prf_set_src += home_prf;
                  Hash_set<eString> prf_set_dest(hash_estring, 128);
                  prf_set_dest += prf;
                  mov_prf_battr->set_src(SRC1, prf_set_src);
                  mov_prf_battr->set_dest(DEST1, prf_set_dest);
                  set_prf_binding_attr(mov_op, mov_prf_battr);

		} else if (port != PRED1 && op->predicated() &&
                         !op->src(PRED1).is_predicate()) {
                    mov_op->set_src(PRED1, op->src(PRED1));
                    eString prf;
                    if (op->src(PRED1).is_reg()) {
                      assert(Codegen::Vr_Home_Prf.is_bound(op->src(PRED1).vr_num()));
                      prf = Codegen::Vr_Home_Prf.value(op->src(PRED1).vr_num());
                    } else if (op->src(PRED1).is_macro_reg()) {
                      assert(Codegen::Macro_Home_Set.is_bound(op->src(PRED1).name()));
                      prf = Codegen::Macro_Home_Set.value(op->src(PRED1).name()).head();
                    } else
                      assert(0);
                    prf_bind_map->bind_operand(mov_op->src(PRED1), prf);
                    movpp_op = get_movpp_if_necessary(mov_op, prf_bind_map);
                } 

                op->set_src(port, new_oprnd);
                prf_bind_map->bind_operand(op->src(port), prf);		
                Hash_set<eString> new_prf_set(hash_estring, 128);
                new_prf_set += prf;
                get_prf_binding_attr(op)->set_src(port, new_prf_set);

                if (dbg(genrio, 1))
                    cdbg << "insert_post_bind_prf_moves: Inserting mov op: " << mov_op->id() 
                         << " for src port: " << port << " in op: " << op->id() << endl;

                El_insert_op_before(mov_op, op);
                if (movpp_op) {
                  El_insert_op_before(movpp_op, mov_op);
                  if (dbg(genrio, 1))
                    cdbg << "insert_post_bind_prf_moves: Inserting movpp op: " << movpp_op->id() 
                         << "for mov op: " << mov_op->id() << endl;
                }


                //if (check_for_valid_opdes(mov_op, cid) == false)
                //    El_punt("insert_post_bind_prf_moves: Cannot find valid opdes for mov_op: %d", mov_op->id());

                Codegen::bind_prf_move_ops++;
            }

        // process the def
        } else if ((ref.get_ref_type()) == EXP_DEST) {

            // bound prf 'prf' and home_prf do not match
            if (!home_prf.is_member(prf)) {

                if (dbg(genrio,7)) {
                    cdbg << "op: " << op->id() << " dest port: " << port << endl;
                    cdbg << "prf: " << prf << " home prf: " << home_prf << endl;
                }

                // insert move for dest
                Opcode opc = get_move_opcode_for_operand(ref.get_operand());
                Op *mov_op = new Op(opc);

                mov_op->set_dest(DEST1, ref.get_operand());
                prf_bind_map->bind_operand(mov_op->dest(DEST1), home_prf.head());
		
                // add the newly inserted mov op to the current op's cluster
                // if the current op is an icmove add the newly inserted mov op
                // to the destination (of the icmove) operand's cluster.
                // assert that the mov_op is supported in that cluster
                int cid;
                if (op->flag(EL_OPER_ICMOVE)) {
                    assert(Cluster_mgr::operand_map.is_bound(oprnd.vr_num()));
                    cid = Cluster_mgr::operand_map.value(oprnd.vr_num());
                } else {
                    assert(Cluster_mgr::op_map.is_bound(op));
                    cid = Cluster_mgr::op_map.value(op);
                }
                assert(Cluster_mgr::hw_cluster_vec[cid]->is_supported(mov_op));
                Cluster_mgr::op_map.bind(mov_op, cid);

                if (op->flag(EL_OPER_LIVEOUT_DEST))
                    mov_op->set_flag(EL_OPER_LIVEOUT_DEST);
                if (op->flag(EL_OPER_SPILL_CODE))
                    mov_op->set_flag(EL_OPER_SPILL_CODE);
                if (op->flag(EL_OPER_CALLER_SAVE))
                    mov_op->set_flag(EL_OPER_CALLER_SAVE);
                if (op->flag(EL_OPER_CALLEE_SAVE))
                    mov_op->set_flag(EL_OPER_CALLEE_SAVE);

                mov_op->set_flag(EL_OPER_CGEN_MOVE);

                Reg *new_reg = new Reg(ref.get_operand().dtype(), ref.get_operand().file_type());
                new_reg->rename();
		
                Codegen::Vr_Home_Prf.bind(new_reg->vr_num(), prf);
                Hash_set<eString> prf_set(hash_estring, 128);
                prf_set += prf;
                Codegen::Vr_Home_Set.bind(new_reg->vr_num(), prf_set);

                Operand new_oprnd(new_reg);

                Operand pred_true(new Pred_lit(true));

                Cluster_mgr::operand_map.bind(new_oprnd.vr_num(), cid);                

                Op *pred_clear_op=0;
                Op *movpp_op=0;
                if (is_U_pred(op, port)) {
                    mov_op->set_src(PRED1, pred_true);
                    mov_op->set_src(SRC1, new_oprnd);
                    prf_bind_map->bind_operand(mov_op->src(SRC1), prf);

                    Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
                    Hash_set<eString> prf_set_src(hash_estring, 128);
                    prf_set_src += prf;
                    Hash_set<eString> prf_set_dest(hash_estring, 128);
                    prf_set_dest += home_prf;
                    mov_prf_battr->set_src(SRC1, prf_set_src);
                    mov_prf_battr->set_dest(DEST1, prf_set_dest);
                    set_prf_binding_attr(mov_op, mov_prf_battr);

		} else if (is_O_pred(op, port)) {
                    mov_op->set_src(PRED1, new_oprnd);
                    mov_op->set_src(SRC1, pred_true);
                    prf_bind_map->bind_operand(mov_op->src(PRED1), prf);

                    Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
                    Hash_set<eString> prf_set_src(hash_estring, 128);
                    prf_set_src += prf;
                    Hash_set<eString> prf_set_dest(hash_estring, 128);
                    prf_set_dest += home_prf;
                    mov_prf_battr->set_src(PRED1, prf_set_src);
                    mov_prf_battr->set_dest(DEST1, prf_set_dest);
                    set_prf_binding_attr(mov_op, mov_prf_battr);

                    // Insert predicate clear
                    pred_clear_op = new Op(PRED_CLEAR);
                    pred_clear_op->set_dest(DEST1, new_oprnd);
                    prf_bind_map->bind_operand(pred_clear_op->dest(DEST1), prf);
                    El_insert_op_after_merge(op->parent(), pred_clear_op);

                    Physfile_binding_attr *pclear_battr = new Physfile_binding_attr();
                    Hash_set<eString> prf_set_1(hash_estring, 128);
                    prf_set_1 += prf;
                    pclear_battr->set_dest(DEST1, prf_set_1);
                    set_prf_binding_attr(pred_clear_op, pclear_battr);

                    Cluster_mgr::op_map.bind(pred_clear_op, cid);

		} else if (!op->predicated() ||
                           (op->predicated() && op->src(PRED1).is_predicate_true())) {

                  Operand pred_true(new Pred_lit(true));
                  mov_op->set_src(PRED1, pred_true);

                  // create a prf attr for the mov_op with prf choices same as the newly bound 
                  // prf
                  Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
                  Hash_set<eString> prf_set_src(hash_estring, 128);
                  prf_set_src += prf;
                  Hash_set<eString> prf_set_dest(hash_estring, 128);
                  prf_set_dest += home_prf;
                  mov_prf_battr->set_src(SRC1, prf_set_src);
                  mov_prf_battr->set_dest(DEST1, prf_set_dest);
                  set_prf_binding_attr(mov_op, mov_prf_battr);
                  mov_op->set_src(SRC1, new_oprnd);
                  prf_bind_map->bind_operand(mov_op->src(SRC1), prf);

		} else if (op->predicated() && !op->src(PRED1).is_predicate()) {

                  mov_op->set_src(SRC1, new_oprnd);
                  prf_bind_map->bind_operand(mov_op->src(SRC1), prf);
                  mov_op->set_src(PRED1, op->src(PRED1));

                  eString prf;
                  if (op->src(PRED1).is_reg()) {
                    assert(Codegen::Vr_Home_Prf.is_bound(op->src(PRED1).vr_num()));
                    prf = Codegen::Vr_Home_Prf.value(op->src(PRED1).vr_num());
                  } else if (op->src(PRED1).is_macro_reg()) {
                    assert(Codegen::Macro_Home_Set.is_bound(op->src(PRED1).name()));
                    prf = Codegen::Macro_Home_Set.value(op->src(PRED1).name()).head();
                  } else
                    assert(0);
                  prf_bind_map->bind_operand(mov_op->src(PRED1), prf);

                  movpp_op = get_movpp_if_necessary(mov_op, prf_bind_map);
                }

                op->set_dest(port, new_oprnd);
                prf_bind_map->bind_operand(op->dest(port), prf);
                Hash_set<eString> new_prf_set(hash_estring, 128);
                new_prf_set += prf;
                get_prf_binding_attr(op)->set_dest(port, new_prf_set);

                if (dbg(genrio, 1))
                    cdbg << "insert_post_bind_prf_moves: Inserting mov op: " << mov_op->id() 
                         << "for dest port: " << port << " in op: " << op->id() << endl;

                if (is_brf(op)) {
                    El_punt("brf! op: %d dest port: %d vr: %d\n", op->id(), port, op->dest(port).vr_num());
                    // El_insert_op_after_merge(op->parent(), mov_op);
                } else if (is_brl(op) && (op->dest(port).name() == RETURN_ADDR)) {
                    El_punt("brl! op: %d ret addr\n", op->id());
		} else
                    El_insert_op_after(mov_op, op);

                if (movpp_op) {
                    El_insert_op_before(movpp_op, mov_op);
                    if (dbg(genrio, 1))
                      cdbg << "insert_post_bind_prf_moves: Inserting movpp op: " << movpp_op->id() 
                           << "for mov op: " << mov_op->id() << endl;
                }
 
                // if (check_for_valid_opdes(mov_op, cid) == false)
                //     El_punt("insert_post_bind_prf_moves: Cannot find valid opdes for mov_op: %d", mov_op->id());

                Codegen::bind_prf_move_ops++;
            }
        }
    }
}

void Codegen::insert_post_bind_prf_moves(Compound_region *r)
{
    Hash_map<int, Hash_set<El_ref> > vr_ref_set(hash_int, 32);
    Hash_map<Macro_name, Hash_set<El_ref> > macro_ref_set(hash_macro_name, 32);

    List<Triple<int, Hash_set<El_ref>, Hash_set<eString> > > vr_triple_list;
    List<Triple<Macro_name, Hash_set<El_ref>, Hash_set<eString> > > macro_triple_list;

    if (dbg(genrio, 3))
        cdbg << "Doing post prf bind move codegen on proc: " << (char*)(el_get_enclosing_procedure(r))->get_name() 
             << " region: " << r->id() << endl;

    if (dbg(genrio, 7)) {
        char buff[256];
        sprintf(buff, "%s_%d_before_post_bind_prf_move.reb", (char*)(el_get_enclosing_procedure(r))->get_name(), 
                                                             r->id());
        cdbg << "Printing Rebel before post bind PRF move" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }


    // create a map of vr_num to <set of refs(of that vr_num)>
    // essentially, a web
    get_ref_set_for_region(r, vr_ref_set, macro_ref_set);
    
    // For VRs...
    for (Hash_map_iterator<int, Hash_set<El_ref> > hiter1(vr_ref_set); hiter1!=0; hiter1++) {
        Pair<int, Hash_set<El_ref> > vr_num_ref = *hiter1;
        int vr_num = vr_num_ref.first;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = vr_num_ref.second;

        Hash_set<eString> home_prf_set(hash_estring, 16);
        eString home_prf;
            
        // if vr_num does not have a home prf, find the ref for that vr_num from its web
        // which has the max cost.
        if (!Vr_Home_Prf.is_bound(vr_num)) {
            get_best_bound_prf(ref_set, home_prf, &prf_bind_map);

            if (dbg(genrio, 7)) 
                cdbg << "insert_post_bind_prf_moves: vr_num: " << vr_num << " best home_prf: " << home_prf << endl;

            Vr_Home_Prf.bind(vr_num, home_prf);

        } else {
            home_prf = Vr_Home_Prf.value(vr_num);
            if (dbg(genrio, 7)) 
                cdbg << "insert_post_bind_prf_moves: bound home_prf for vr: " 
                     << vr_num << " prf: " << home_prf << endl;
        }

        home_prf_set += home_prf;
        Triple<int, Hash_set<El_ref>, Hash_set<eString> > triple(vr_num, ref_set, home_prf_set);
        vr_triple_list.add_tail(triple);
    }

    // For Macros...
    for (Hash_map_iterator<Macro_name, Hash_set<El_ref> > hiter2(macro_ref_set); hiter2!=0; hiter2++) {
        Pair<Macro_name, Hash_set<El_ref> > macro_name_ref = *hiter2;
        Macro_name mn = macro_name_ref.first;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = macro_name_ref.second;

        assert(Macro_Home_Set.is_bound(mn));
        Triple<Macro_name, Hash_set<El_ref>, Hash_set<eString> > triple(mn, ref_set, Macro_Home_Set.value(mn));
        macro_triple_list.add_tail(triple);
    }

    List_iterator<Triple<int, Hash_set<El_ref>, Hash_set<eString> > > liter1(vr_triple_list);
    List_iterator<Triple<Macro_name, Hash_set<El_ref>, Hash_set<eString> > > liter2(macro_triple_list);

    // vrs
    for(;liter1!=0; liter1++) {

        Hash_set<eString> home_prfs = (*liter1).third;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = (*liter1).second;

        insert_moves_if_incompatible(ref_set, home_prfs, &prf_bind_map);
    }
    // macros
    for(;liter2!=0; liter2++) {

        Hash_set<eString> home_prf = (*liter2).third;
        Hash_set<El_ref> ref_set(hash_El_ref, 1024);
        ref_set = (*liter2).second;

        insert_moves_if_incompatible(ref_set, home_prf, &prf_bind_map);
    }

    if (dbg(genrio, 7)) {
        char buff[256];
        sprintf(buff, "%s_%d_after_post_bind_prf_move.reb", (char*)(el_get_enclosing_procedure(r))->get_name(), 
                                                             r->id());
        cdbg << "Printing Rebel after post bind PRF move" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }
}
