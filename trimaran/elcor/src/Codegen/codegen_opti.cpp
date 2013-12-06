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
//      File:           codegen.cpp                                      //
//      Authors:        Rajiv Ravindran                                  //
//      Created:        April 2002                                       //
//      Description:    Codegen Optimizations                            //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "codegen.h"

using namespace std;

bool Codegen::global_constant_cse_elimination2(Compound_region *f)
{
    bool found = false;

    if (dbg(genrio,1)) {
      if (f->is_procedure())
        cdbg << "global constant cse elimination in procedure: "
	     << (char*)(((Procedure*)f)->get_name()) << endl;
      else
        cdbg << "global constant cse elimination in block: " << f->id() << endl;
    }

    create_local_analysis_info_for_all_hbs_bbs(f);

    // Use available expressions to identify common subexpressions
    // and to check redundancy to determine which can be eliminated
    Available_expression_solver avexp(f, AV_EXPR_UNARY) ;
    avexp.solve() ;
    Hash_map<Op*, bool> was_op_fully_redundant(hash_op_ptr);
    Hash_map<int, bool> any_fully_redundant(hash_int);
    Hash_map<int, Operand> temp_map(hash_int);
    List<int> exp_list = avexp.get_expression_list();

    // Pass One: Determine redundancy and create temporary registers to prepare for
    // CSE where redundancy exists.
    // Iterate over each expression in the Compound Region
    for (List_iterator<int> exp_iter(exp_list); exp_iter != 0; exp_iter++) {
        int cur_exp = *exp_iter;
        any_fully_redundant.bind(cur_exp, false);
        bool temps_created = false;
        if (dbg(genrio,3)) 
            cdbg << "iterating exp is : " << cur_exp << endl;

        // Iterate over each operation of a common subexpression
        List<Op*>& ops_for_exp = avexp.get_ops_for_expression(cur_exp) ;

        for (List_iterator<Op*> ops_for_exp_iter(ops_for_exp); ops_for_exp_iter != 0; ops_for_exp_iter++) {
            Op* cur_op = *ops_for_exp_iter;
            if (is_pbr(cur_op) || !is_move(cur_op))
                continue;
            if (cur_op->src(SRC1).is_reg() || cur_op->src(SRC1).is_macro_reg())
                continue;

            was_op_fully_redundant.bind(cur_op, avexp.is_op_fully_redundant(cur_op));
            if (was_op_fully_redundant.value(cur_op)) {
                if (dbg(genrio,5))
                    cdbg << "op " << cur_op->id() << " is fully redundant." << endl;
                // if any operations of a common subexpression are fully redundant, temps are
                // created to allow removal of this and any other fully redundant expressions
                // with the same common subexpression. Only need to create one set of temps
                // for each common subexpression
                if (!temps_created) {
                    // create unique temp(s)
                    Operand reg_temp = new Reg(cur_op->dest(DEST1).dtype());
                    temp_map.bind(cur_exp, reg_temp);
                    temps_created = true;
                }
                any_fully_redundant.value(cur_exp) = true;
            }
        }
    }

    // Pass Two: perform CSE on expressions for which Pass One showed any redundancy
    // Iterate over each expression in the Compound Region
    for (List_iterator<int> exp_iter2(exp_list); exp_iter2 != 0; exp_iter2++) {
        int cur_exp = *exp_iter2;
        // Perform CSE for this expression if any redundancy found in Pass One
        if (any_fully_redundant.value(cur_exp)) {
            if (dbg(genrio,0)) cdbg << "    ****Current redundant expression id " << cur_exp << endl;
            // Iterate over each operation of a common subexpression

            List<Op*>& ops_for_exp = avexp.get_ops_for_expression(cur_exp) ;
            for (List_iterator<Op*> ops_for_exp_iter(ops_for_exp); ops_for_exp_iter!=0; ops_for_exp_iter++) {
                Op* cur_op = *ops_for_exp_iter;

                if (dbg(genrio,0)) cdbg << "Considering cs op: " << cur_op->id() <<endl;
                assert(is_move(cur_op) && !is_pbr(cur_op));

                found = true;

                if (!was_op_fully_redundant.value(cur_op)) {
                  if (dbg(genrio,0))
                    cdbg << "non fully redundant op: " << cur_op->id() << endl;
                  Op *mov_op = new Op(MOVE);
                  mov_op->set_dest(DEST1, cur_op->dest(DEST1));
                  mov_op->set_src(SRC1, temp_map.value(cur_exp));
                  mov_op->set_src(PRED1, cur_op->src(PRED1)) ;
                  El_insert_op_after(mov_op, cur_op);

                  if (dbg(genrio, 0))
                    cdbg << " inserting mov op " << mov_op->id() << " after op " << cur_op->id() << endl;

                  // Annotate the mov op
                  List_set<Op_descr*> mov_uspc_ops;
                  MDES_collect_op_descrs(el_opcode_to_string_map.value(mov_op->opcode()), mov_uspc_ops);
                  Physfile_binding_attr *mov_prf_battr = new Physfile_binding_attr();
                  set_prf_binding_attr(mov_op, mov_prf_battr);
                  annotate_physical_file_op(mov_op, mov_uspc_ops);
                  cur_op->set_dest(DEST1, temp_map.value(cur_exp));
                } else {
                  if (dbg(genrio,0))
                    cdbg << "fully redundant op: " << cur_op->id() << endl;
                  cur_op->set_src(SRC1, temp_map.value(cur_exp));
                }
            }
        }                    
    }

    delete_local_analysis_info_for_all_hbs_bbs(f);
    return found;
}

bool Codegen::global_constant_cse_elimination(Compound_region *f)
{
    bool found = false;

    if (dbg(genrio,1)) {
      if(f->is_procedure())
        cdbg << "global constant cse elimination in procedure: "
	     << (char*)(((Procedure*)f)->get_name()) << endl;
      else
        cdbg << "global constant cse elimination in block: " << f->id() << endl;
    }

    create_local_analysis_info_for_all_hbs_bbs(f);

    El_do_reaching_defs (f, ANALYZE_ALLREG);
    Reaching_defs_info* rdinfo = get_reaching_defs_info(f);

    // Use available expressions to identify common subexpressions
    // and to check redundancy to determine which can be eliminated
    Available_expression_solver avexp(f, AV_EXPR_UNARY) ;
    avexp.solve() ;
    Hash_map<Op*, bool> was_op_fully_redundant(hash_op_ptr);
    Hash_map<int, bool> any_fully_redundant(hash_int);
    Hash_map<int, Reg> temp_map(hash_int);
    List<int> exp_list = avexp.get_expression_list();

    // Pass One: Determine redundancy and create temporary registers to prepare for
    // CSE where redundancy exists.
    // Iterate over each expression in the Compound Region
    for (List_iterator<int> exp_iter(exp_list); exp_iter != 0; exp_iter++) {
        int cur_exp = *exp_iter;
        any_fully_redundant.bind(cur_exp, false);
        bool temps_created = false;
        if (dbg(genrio,5)) 
            cdbg << "iterating exp is : " << cur_exp << endl;

        // Iterate over each operation of a common subexpression
        List<Op*>& ops_for_exp = avexp.get_ops_for_expression(cur_exp) ;

        for (List_iterator<Op*> ops_for_exp_iter(ops_for_exp); ops_for_exp_iter != 0; ops_for_exp_iter++) {
            Op* cur_op = *ops_for_exp_iter;
            if (is_pbr(cur_op) || !is_move(cur_op))
                continue;
            if (cur_op->src(SRC1).is_macro_reg())
                continue;

            was_op_fully_redundant.bind(cur_op, avexp.is_op_fully_redundant(cur_op));
            if (was_op_fully_redundant.value(cur_op)) {
                if (dbg(genrio,5))
                    cdbg << "op " << cur_op->id() << " is fully redundant." << endl;
                // if any operations of a common subexpression are fully redundant, temps are
                // created to allow removal of this and any other fully redundant expressions
                // with the same common subexpression. Only need to create one set of temps
                // for each common subexpression
                if (!temps_created) {
                    // create unique temp(s)
                    Reg temp_reg(cur_op->dest(DEST1).dtype(), cur_op->dest(DEST1).file_type(), 0);
                    temp_reg.rename();
                    temp_map.bind(cur_exp, temp_reg);

                    temps_created = true;
                }
                any_fully_redundant.value(cur_exp) = true;
            }
        }
    }

    // Pass Two: perform CSE on expressions for which Pass One showed any redundancy
    // Iterate over each expression in the Compound Region
    for (List_iterator<int> exp_iter2(exp_list); exp_iter2 != 0; exp_iter2++) {
        int cur_exp = *exp_iter2;
        // Perform CSE for this expression if any redundancy found in Pass One
        if (any_fully_redundant.value(cur_exp)) {
            if (dbg(genrio,5)) cdbg << "    ****Current redundant expression id " << cur_exp << endl;
            // Iterate over each operation of a common subexpression

            // do we need to do cse?
            bool donot_perform_cse = false;
            List<Op*>& ops_for_exp = avexp.get_ops_for_expression(cur_exp) ;
            for (List_iterator<Op*> ops_for_exp_iter(ops_for_exp); ops_for_exp_iter!=0; ops_for_exp_iter++) {
                Op* cur_op = *ops_for_exp_iter;

                if (dbg(genrio,5)) cdbg << "Considering cs op: " << cur_op->id() <<endl;
                assert(is_move(cur_op) && !is_pbr(cur_op));

                if (cur_op->dest(DEST1).is_macro_reg()) {
                    donot_perform_cse = true;
                    break;
                }
		/*
                if (!cur_op->flag(EL_OPER_CGEN_MOVE)) {
                    donot_perform_cse = true;
                    break;
                }
		*/

                Op_explicit_dests desti(cur_op);
                El_ref def_ref = desti.get_ref();
                List<El_ref> *use_list = rdinfo->get_du_chain(def_ref);
                for (List_iterator<El_ref> liter(*use_list); liter!=0; liter++) {
                    El_ref use = *liter;
                    if ((use.get_ref_type() != EXP_SRC) && 
                        (use.get_ref_type() != PRED_SRC))
                        continue;

                    List<El_ref> *def_list = rdinfo->get_ud_chain(use);

                    // there is more than one define reaching this use..
                    if (def_list->size() != 1) {
                        donot_perform_cse = true;
                        break;
                    }
		    
                    List_set<Op_descr*> uspc_ops;
                    MDES_collect_op_descrs(el_opcode_to_string_map.value((use.get_op())->opcode()), uspc_ops);

                    // can i replace the use by temp?
                    Reg *reg = new Reg(temp_map.value(cur_exp));		    
                    Operand temp_oprnd(reg);
                    if (Codegen::prf_bind_replace(use.get_op(), temp_oprnd,
                                                  DATA_IN, use.get_port_num(), uspc_ops)) {
                        // is the use.op in 2 operand format?
                        if (in_two_operand_format(use.get_op())) {
                            if (dbg(genrio,3)) 
                                cdbg << "trying to replace src: " << use.get_port_num() 
                                     << " of op: " << use.get_op()->id() 
                                     << " with temporary but could not as op was in 2 operand format.." << endl;
                            donot_perform_cse = true;
                        }
                    } else {
                        donot_perform_cse = true;
                        if (dbg(genrio,3)) 
                            cdbg << "trying to replace src: " << use.get_port_num() 
                                 << " of op: " << use.get_op()->id()
                                 << " with temporary but could not due to prf set incompatibility.." << endl;
                    }

                    if (donot_perform_cse) break;
                }

                if (donot_perform_cse)
                    break;
            }

            if (donot_perform_cse) {
                if (dbg(genrio,1))
                    cdbg << "ignoring cse for current redundant expression: " << cur_exp << endl;
                continue;
            }

            List<El_ref> dead_uses;
            List<El_ref> new_uses;
            Op *def_op = 0;
            for (List_iterator<Op*> ops_for_exp_iter1(ops_for_exp); ops_for_exp_iter1!=0; ops_for_exp_iter1++) {
                Op* cur_op = *ops_for_exp_iter1;
                El_ref def(cur_op, &(cur_op->dest(DEST1)), EXP_DEST);

                if (dbg(genrio,5)) cdbg << "Considering cs op: " << cur_op->id() <<endl;
                assert(is_move(cur_op) && !is_pbr(cur_op));

                Op_explicit_dests desti(cur_op);
                El_ref def_ref = desti.get_ref();
                List<El_ref> *use_list = rdinfo->get_du_chain(def_ref);
                for (List_iterator<El_ref> liter(*use_list); liter!=0; liter++) {
                    El_ref use = *liter;
                    if ((use.get_ref_type() != EXP_SRC) && 
                        (use.get_ref_type() != PRED_SRC))
                        continue;

                    List<El_ref> *def_list = rdinfo->get_ud_chain(use);
                    assert(def_list->size() == 1);

                    List_set<Op_descr*> uspc_ops;
                    MDES_collect_op_descrs(el_opcode_to_string_map.value((use.get_op())->opcode()), uspc_ops);
                    dead_uses.add_tail(use);

                    // replace the use
                    Reg *reg = new Reg(temp_map.value(cur_exp));
                    Operand temp_oprnd(reg);
                    (use.get_op())->set_src(use.get_port_num(), temp_oprnd);

                    El_ref new_use(use.get_op(), &((use.get_op())->src(use.get_port_num())), EXP_SRC);
                    new_uses.add_tail(new_use);
                          
                    // Now, reannotate the operand
                    Physfile_binding_attr *use_prf_battr = get_prf_binding_attr(use.get_op());
                    annotate_prf_port(DATA_IN, use.get_port_num(), use.get_operand(), use_prf_battr,
                                      uspc_ops, use.get_op());

                    if (dbg(genrio,3)) 
                        cdbg << " replacing src: " << use.get_port_num() << " of op: " 
                             << use.get_op()->id() << " with temporary" << endl;

                    found = true;
                }

                while (!dead_uses.is_empty()) {
                    El_ref dead = dead_uses.pop();
                    rdinfo->remove_du_ud_links(def, dead);
                }

                if (!was_op_fully_redundant.value(cur_op)) {
                    if (dbg(genrio,3))
                        cdbg << "non fully redundant op: " << cur_op->id() << endl;

                    Reg *reg = new Reg(temp_map.value(cur_exp));
                    Operand temp_oprnd(reg);
                    cur_op->set_dest(DEST1, temp_oprnd);
                    def_op = cur_op;
                }
            }

            assert(def_op!=0);
            El_ref new_def_ref(def_op, &(def_op->dest(DEST1)), EXP_DEST);
            while (!new_uses.is_empty()) {
                El_ref new_use = new_uses.pop();
                rdinfo->add_du_ud_links(new_def_ref, new_use);
            }
        }                    
    }

    delete_local_analysis_info_for_all_hbs_bbs(f);
    remove_reaching_defs_info(f);

    return found;
}

// Convert 
// x = y
// .
// z = x + k=>
// x = y
// .
// z = y + k

// This is called after physical file move codegen
// in the initial_codegen phase.
// Make sure that y can be substituted  in x's position
// When you replace x by y, the prf choices for the new position of y
// should be either equal to old(x) prf choices or a superset
// So that you do not create any prf incompatiblities between defs and uses
// Also do not violate 2-operand format restrictions
bool Codegen::global_copy_propagate(Compound_region *f)
{
    List<El_ref> dead_uses;
    List<El_ref> new_uses;
    bool propagate = false;

    if (dbg(genrio,1)) {
      if(f->is_procedure())
        cdbg << "global copy propagating in procedure: "
	     << (char*)(((Procedure*)f)->get_name()) << endl;
      else
	cdbg << "global copy propagating in block: " << f->id() << endl;
    }

    delete_local_analysis_info_for_all_hbs_bbs(f);
    create_local_analysis_info_for_all_hbs_bbs(f);

    el_flow_compute_liveness(f, ANALYZE_ALLREG);

    El_do_reaching_defs(f, ANALYZE_ALLREG);
    Reaching_defs_info *rdinfo = get_reaching_defs_info(f);

    // Use available expressions to see if the right hand side
    // of the copies get clobbered before the uses of the
    // left hand sides
    Available_expression_solver avexp(f, AV_EXPR_UNARY_OR_BINARY, false) ;
    avexp.solve();

    for (Region_all_ops opi(f); opi!=0; opi++) {
        bool copy =  false;

        new_uses.clear();
        dead_uses.clear();

        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;

        if (!is_copy(op)) 
            continue;

	/*
        if (!op->flag(EL_OPER_CGEN_MOVE))
            continue;
	    */

        if (op->src(SRC1).is_predicate()) continue;

        // get ref to SRC1
        Op_explicit_sources srcs(op);
        El_ref src_ref = srcs.get_ref();

        El_ref def(op, &(op->dest(DEST1)), EXP_DEST);
        List<El_ref>* use_list = rdinfo->get_du_chain(def);
        for (List_iterator<El_ref> liter(*use_list); liter!=0; liter++) {
            El_ref use = *liter;
            if ((use.get_ref_type() != PRED_SRC) &&
                (use.get_ref_type() != EXP_SRC))
                continue;
            List<El_ref>* def_list = rdinfo->get_ud_chain(use);
            if (def_list->size() > 1)
                continue;

            if (def.get_operand() != use.get_operand())
                continue;

            Compound_region* cur_reg = get_enclosing_analysis_scope(use.get_op()) ;
            Pred_cookie copy_available_cond =
                  avexp.get_available_expression_cookie_before_op(cur_reg, op, use.get_op()) ;
            Pred_jar pj(cur_reg) ;
            Pred_cookie use_cond = pj.get_lub_guard(use) ;
            if (!use_cond.is_subset(copy_available_cond)) 
                continue;

           List_set<Op_descr*> uspc_ops;
           MDES_collect_op_descrs(el_opcode_to_string_map.value((use.get_op())->opcode()), uspc_ops);
           if (Codegen::prf_bind_replace(use.get_op(), op->src(SRC1), DATA_IN, use.get_port_num(), uspc_ops)) {
               if (!in_two_operand_format(use.get_op())) {
                   Operand new_src(op->src(SRC1));
                   (use.get_op())->set_src(use.get_port_num(), new_src);
                   (use.get_op())->set_src_spatial_ref(use.get_port_num(), src_ref.spatial_ref());

                   // Now, reannotate the operand
                   Physfile_binding_attr *use_prf_battr = get_prf_binding_attr(use.get_op());
                   annotate_prf_port(DATA_IN, use.get_port_num(), use.get_operand(), use_prf_battr, 
                                     uspc_ops, use.get_op());
                   if (dbg(genrio,3)) 
                       cdbg << "global copy propagating from mov op: " << op->id() 
                            << " to op: " << (use.get_op())->id() << endl;
                   dead_uses.add_tail(use);
                   
                   El_ref new_use_ref(use.get_op(), &((use.get_op())->src(use.get_port_num())), EXP_SRC);
                   new_uses.add_tail(new_use_ref);

                   propagate = true;
                   copy = true;
               } else {
                   if (dbg(genrio,3))
                       cdbg << "trying to global copy propagate to port: " << use.get_port_num() 
                            << " of op: " << (use.get_op())->id() << " from mov op: " << op->id()
                            << " but is in two operand format" << endl;
               }
           } else {
               if (dbg(genrio,3))
                   cdbg << "could not global copy propagate from op: " << op->id() 
                        << " to op: " << (use.get_op())->id() << " because of prf set incompatibility" << endl;
           }
        }

        if (copy == true) {
            // Update du-ud chain for new uses.
            if (!((src_ref.get_operand()).is_lit())) {
                List<El_ref>* def_list = rdinfo->get_ud_chain(src_ref);
                for (List_iterator<El_ref> liter1(*def_list); liter1!=0; liter1++) {
                    El_ref def = *liter1;
                    for (List_iterator<El_ref> liter2(new_uses); liter2!=0; liter2++) {
                        El_ref new_use = *liter2;
                        rdinfo->add_du_ud_links(def, new_use);
                    }
                }
            }

            // Remove the dead uses from reaching defs info
            while(!(dead_uses.is_empty())) {
                El_ref dead = dead_uses.pop();
                rdinfo->remove_du_ud_links(def, dead); 
                // rdinfo->remove_use(dead);
            }
        }
    }
    

    delete_local_analysis_info_for_all_hbs_bbs(f);
    remove_reaching_defs_info(f);

    return propagate;
}

// Same as above,
// It is called for every region after physical file move codegen
// We should make sure that the du-ud compatibility is not violated
// When you replace x by y, the prf choices for the new position of y
// should be either equal to old(x) prf choices or a superset
// So that you do not create any prf incompatiblities between defs and uses
// Also do not violate 2-operand format restrictions
// This could be called after regalloc to optimize the moves generated due to
// codegen on spill code. Do not do copy propagate on reg alloced variables
bool Codegen::local_copy_propagate(Compound_region *r, Cluster_mgr& cmgr, bool post_cluster, bool postpass)
{
    List<El_ref> dead_uses;
    List<El_ref> new_uses;
    bool propagate = false;

    if (dbg(genrio,1))
      cdbg << "copy propagating in region: " << r->id() << endl;

    delete_local_analysis_info_for_all_hbs_bbs(r);
    create_local_analysis_info_for_all_hbs_bbs(r);

    El_do_reaching_defs(r, ANALYZE_ALLREG);
    Reaching_defs_info *rdinfo = get_reaching_defs_info(r);

    for (Region_all_ops opi(r); opi!=0; opi++) {
        bool copy = false;

        Op *op = *opi;
        if (is_pseudo(op))
            continue;

        if (!is_copy(op)) 
            continue;

	/*
        if (!op->flag(EL_OPER_CGEN_MOVE))
            continue;
	    */

        if (op->src(SRC1).is_predicate()) continue;

        // ignore icmove ops
        if (op->flag(EL_OPER_ICMOVE))
            continue;

        if (op->src(SRC1).assigned_to_physical_file() && 
            (op->src(SRC1).is_reg() || op->src(SRC1).is_macro_reg()) && 
            (op->src(SRC1).file_type() == PR))
            continue;

        if (op->dest(DEST1).allocated() || op->src(SRC1).allocated())
            continue;

        new_uses.clear();
        dead_uses.clear();

        Op_explicit_sources srcs(op);
        El_ref src_ref = srcs.get_ref();

        Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
        assert(prf_battr != 0);
        Hash_set<eString> src_prf_set = prf_battr->src(SRC1);
        assert(!src_prf_set.is_empty());

        El_ref def(op, &(op->dest(DEST1)), EXP_DEST);
        List<El_ref>* use_list = rdinfo->get_du_chain(def);
        for (List_iterator<El_ref> liter(*use_list); liter!=0; liter++) {
            El_ref use = *liter;
            if (((use.get_operand()).is_reg()) && ((use.get_operand()).allocated()))
                continue;
            if ((use.get_ref_type() != PRED_SRC) &&
                (use.get_ref_type() != EXP_SRC))
                continue;

            List<El_ref>* def_list = rdinfo->get_ud_chain(use);
            if (def_list->size() > 1)
                continue;

            if (def.get_operand() != use.get_operand())
                continue;

            if (!El_no_defs_between(r, op, use.get_op(), op->src(SRC1)))
                continue;
            if (!El_predicate_dominates(r, def, use))
                continue;

            List_set<Op_descr*> uspc_ops;
            if (post_cluster == true) {
                if ((use.get_op())->flag(EL_OPER_ICMOVE)) {
                    Hash_set<eString> uspc_icmoves = cmgr.get_ic_move_opcodes();

                    List_set<Op_descr*> uspc_op_descrs;
                    MDES_collect_op_descrs(el_opcode_to_string_map.value((use.get_op())->opcode()), uspc_op_descrs);

                    for (List_set_iterator<Op_descr*> liter(uspc_op_descrs); liter!=0; liter++) {
                        Op_descr *op_descr = *liter;
                        if (uspc_icmoves.is_member(op_descr->get_aname_ptr()))
                            uspc_ops += op_descr;
                    }
                } else {
                    assert(Cluster_mgr::op_map.is_bound(use.get_op()));
                    int cid = Cluster_mgr::op_map.value(use.get_op());
                    get_uspc_ops_in_cluster(use.get_op(), cid, uspc_ops);
                }
            } else
                MDES_collect_op_descrs(el_opcode_to_string_map.value(use.get_op()->opcode()), uspc_ops);

            if (Codegen::prf_bind_replace(use.get_op(), op->src(SRC1), DATA_IN, use.get_port_num(), uspc_ops)) {
                // check if there is a common file between the src of mov and the use
                Hash_set<eString> use_prf_set = get_prf_binding_attr(use.get_op())->src(use.get_port_num());
                use_prf_set *= src_prf_set;
                if (use_prf_set.is_empty()) {
                    if (dbg(genrio,3)) 
                        cdbg << "avoiding local copy propagating from mov op: " << op->id() 
                             << " to op: " << (use.get_op())->id() << " as src and use prf sets have nothing in common" << endl;
                    continue;
                }
                if (!in_two_operand_format(use.get_op())) {
                    Operand new_oprnd(op->src(SRC1));
                    (use.get_op())->set_src(use.get_port_num(), new_oprnd);
                    (use.get_op())->set_src_spatial_ref(use.get_port_num(), src_ref.spatial_ref());

                    // Now, reannotate the operand
                    Physfile_binding_attr *use_prf_battr = get_prf_binding_attr(use.get_op());
                    annotate_prf_port(DATA_IN, use.get_port_num(), use.get_operand(), use_prf_battr, 
                                      uspc_ops, use.get_op());
                    if (use.get_op()->flag(EL_OPER_ICMOVE))
                      eliminate_prf_choices_for_icmoves(use.get_op(), cmgr, postpass);

                    if (dbg(genrio,3)) 
                        cdbg << "local copy propagating from mov op: " << op->id() 
                             << " to op: " << (use.get_op())->id() << endl;
                   
                    El_ref new_use_ref(use.get_op(), &((use.get_op())->src(use.get_port_num())), EXP_SRC);
                    new_uses.add_tail(new_use_ref);

                    dead_uses.add_tail(use);
                    propagate = true;
                    copy = true;
                } else {
                    if (dbg(genrio,3))
                        cdbg << "trying to local copy propagate to port: " << use.get_port_num() 
                             << " of op: " << (use.get_op())->id() << " from mov op: " << op->id()
                             << " but is in two operand format" << endl;
                }
            } else {
                if (dbg(genrio,3))
                    cdbg << "could not local copy propagate from op: " << op->id() 
                         << " to op: " << (use.get_op())->id() << " because of prf set incompatibility" << endl;
            }
        }

        if (copy == true) {
            // Update du-ud chain for new uses.
            if (!((src_ref.get_operand()).is_lit())) {
                List<El_ref> *def_list = rdinfo->get_ud_chain(src_ref);
                for (List_iterator<El_ref> liter1(*def_list); liter1!=0; liter1++) {
                    El_ref def = *liter1;
                    for (List_iterator<El_ref> liter2(new_uses); liter2!=0; liter2++) {
                        El_ref new_use = *liter2;
                        rdinfo->add_du_ud_links(def, new_use);
                    }
                }
            }

            // Remove the dead uses from reaching defs info
            while(!(dead_uses.is_empty())) {
                El_ref dead = dead_uses.pop();
                rdinfo->remove_du_ud_links(def, dead); 
                // rdinfo->remove_use(dead);
            }
        }
    }

    delete_local_analysis_info_for_all_hbs_bbs(r);
    remove_reaching_defs_info(r);

    return propagate;
}

