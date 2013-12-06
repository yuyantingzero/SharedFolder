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
//      Description:    Main codegen routines                            //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "codegen.h"
#include "el_opti_loopinvariant.h"


int Codegen::move_ops = 0;
int Codegen::two_oprnd_move_ops = 0;
int Codegen::prf_move_ops = 0;
int Codegen::bind_prf_move_ops = 0;

bool Codegen::init = false;

Slist<Stuple<int, char *> > Codegen::Litfile_List;
List<Pair<char*, int> > Codegen::Prf_Width_List;
List<Pair<char *, int> > Codegen::Prf_reg_size_list;
Hash_map<int, Hash_set<eString> > Codegen::Vr_Home_Set(hash_int, CGEN_HASH_SIZE_2);
Hash_map<Macro_name, Hash_set<eString> > Codegen::Macro_Home_Set(hash_macro_name, CGEN_HASH_SIZE_2);
Hash_map<int, eString> Codegen::Vr_Home_Prf(hash_int, CGEN_HASH_SIZE_2);

PRF_Bind_Map Codegen::prf_bind_map;
Hash_map<Operand, eString> Codegen::operand_to_label_map(hash_operand, 1024);

// Machine specific opcode conversion
Hash_map<Opcode, FuncImpl> *Codegen::opcTable;

Hash_set<int> ignore_set(hash_int);

static void unbind_all_operands(Op *op)
{
    for (Op_explicit_inputs srci(op); srci!=0; srci++) {
        Operand src_oprnd = *srci;
        if (src_oprnd.assigned_to_physical_file()) {
            //Operand *new_src_oprnd = Codegen::get_new_operand(src_oprnd);
            //op->set_src(srci.get_port_num(), *new_src_oprnd);
            //(op->src(srci.get_port_num())).unbind_physical_file();
            (op->src(srci.get_port_num())).bind_physical_file("");
        }
    } 
    for (Op_explicit_dests desti(op); desti!=0; desti++) {
        Operand dest_oprnd = *desti;
        if (dest_oprnd.assigned_to_physical_file()) {
            //Operand *new_dest_oprnd = Codegen::get_new_operand(dest_oprnd);
            //op->set_dest(desti.get_port_num(), *new_dest_oprnd);
            (op->dest(desti.get_port_num())).bind_physical_file("");
        }
    } 
}

// return from MDES the set of reg descrs that have the virtual reg file type 'vrf'
// for unit specific opcode uspc_op_name at port port_num of portkind 'portkind'
void Codegen::get_mdes_prf_set(IO_Portkind portkind, int port_num, char *uspc_op_name, char *vrf, Hash_set<Reg_descr*>& reg_descr_set)
{
    List_set<Reg_descr *> reg_descset;
    MDES_collect_reg_descrs(uspc_op_name, portkind, port_num, reg_descset);
    for (List_set_iterator<Reg_descr *> iter(reg_descset); iter!=0; iter++) {
        Reg_descr *reg_descr = *iter;
        //if (!strcmp(reg_descr->get_vname(), vrf))
        // allow for prfs that support multiple vrfs. -KF 3/29/04
        if (strstr(reg_descr->get_vname(), vrf))
          reg_descr_set += (*iter);
    }
}

int Codegen::get_reg_file_size(char *regfile)
{
    for (List_iterator<Pair<char *, int> > iter(Prf_reg_size_list); iter!=0; iter++) {
        if (!strcmp(regfile, (*iter).first))
            return (*iter).second;
    }

    El_punt("reg file %s not present in mdes\n", regfile);
    return 0;
}

// Called once from do_initial_codegen to initialize the list of literal register files and their
// corresponding width(in sorted order of width) in Codegen::Litfile_List
// Also create a list of <PRF name, width> tuples in Codegen::Prf_Width_List.
void Codegen::mdes_regfiles_init()
{
    if (dbg(genrio, 1))
        cdbg << "mdes_regfiles_init..." << endl;

    // get all register files from MDES
    int total_files = current_MDES->get_MDES_num_reg_types();
    for (int file_index = 0; file_index < total_files; file_index++) {
        Reg_descr* reg_descr = current_MDES->get_MDES_reg_descr(file_index);

        Pair<char*, int> prf_regsize_pair(reg_descr->get_aname(), reg_descr->get_scap());
        Prf_reg_size_list.add_tail(prf_regsize_pair);
        
        // Add PRF name to width tuple in Codegen::Prf_Width_List
        Pair<char*, int> prfpair(reg_descr->get_aname(), reg_descr->get_width());
        Prf_Width_List.add_tail(prfpair);

        // sort literal files in increasing order of width
        if (reg_descr->is_lit()) {
            Stuple<int, char *> litfile(reg_descr->get_width(), reg_descr->get_aname());
            Litfile_List.add(litfile);
        }
    }

    if (dbg(genrio, 3)) {
        cdbg << "mdes_regfiles_init: Literal Register Files in MDES..." << endl;
        for (Slist_iterator<Stuple<int, char *> > sliter(Litfile_List); sliter!=0; sliter++) {
            Stuple<int, char *> litfile_elem = *sliter;
            cdbg << "width: " << litfile_elem.first;
            cdbg << " Lit reg file: " << litfile_elem.second << endl;
        }
    }
    if (dbg(genrio, 3)) {
        cdbg << "mdes_regfiles_init: PRF name to width..." << endl;
        for (List_iterator<Pair<char *, int> > liter(Prf_Width_List); liter!=0; liter++) {
            Pair<char *, int> p = *liter;
            cdbg << "prf: " << p.first << " width: " << p.second << endl;
        }
    }

    // Update the Macro_Home_Set with the list of macros in the MDES and
    // their home physical register files
    for (int file_index = 0; file_index < total_files; file_index++) {
        Reg_descr* reg_descr = current_MDES->get_MDES_reg_descr(file_index);
        eString prf = reg_descr->get_aname();
        Hash_set<eString> macros(hash_estring, 32);
        reg_descr->get_macros(macros);
        for (Hash_set_iterator<eString> hiter(macros); hiter!=0; hiter++) {
          assert (el_string_to_macro_map.is_bound(*hiter));
          Macro_name mn = el_string_to_macro_map.value(*hiter);
          if (!Codegen::Macro_Home_Set.is_bound(mn)) {
            Hash_set<eString> prf_set(hash_estring, 32);
            prf_set += prf;
            Codegen::Macro_Home_Set.bind(mn, prf_set);
          } else {
            Codegen::Macro_Home_Set.value(mn) += prf;
            // cdbg << "macro: " << mn << " has multiple prfs: " << Codegen::Macro_Home_Set.value(mn);
          }
        }
    }
    
}

// For literal operand 'oprnd', return the minimum width literal file from MDES
// that can support oprnd. For cb/label/string return the max width literal reg file
// that MDES supports
Pair<int, char *> Codegen::get_lit_operand_prf(Operand& oprnd)
{
    int num_bits;

    // compute width only for literals, ignore predicate literals
    assert(oprnd.is_lit() && !oprnd.is_predicate());

    // max literal width as supported by MDES
    // Litfile_List is sorted in increasing order. So last element is the max value
    int max_width = Litfile_List.tail().first;

    // For now we assume that cb/label/string have max mdes literal file width
    // By default, the dtype().width() is 32 and is hardcoded in elcor
    // How should this change with bitwidth analysis
    if (oprnd.is_int() || oprnd.is_float() || oprnd.is_double()) {
        num_bits = oprnd.dtype().width();
    } else {
        num_bits = max_width;
    }

    // Find the Literal register file from MDES that supports num_bits
    Stuple<int, char *> litfile_elem;
    Slist_iterator<Stuple<int, char *> > sliter;
    for (sliter(Litfile_List); sliter!=0; sliter++) {
        litfile_elem = *sliter;
        if (litfile_elem.first >= num_bits)
            break;
    }

    if (sliter == 0) {
        //El_punt("get_lit_operand_prf: no compatible literal file found for lit oprnd: %d", oprnd.int_value());
        Pair<int, char *> npair(num_bits, "");
        return npair;
    }

    Pair<int, char *> npair(litfile_elem.first, litfile_elem.second);
    return npair;
}

// Annotate port 'port_num' of prf_battr with prfs from unit specific ops 'uspc_ops'
void Codegen::annotate_prf_port(IO_Portkind port_kind, Port_num port_num, Operand& oprnd,
                                Physfile_binding_attr *prf_battr, List_set<Op_descr *>& uspc_ops, Op *op)
{
    Hash_set<eString> prf_set(hash_estring, 128);
    if (oprnd.is_predicate() || oprnd.is_undefined())
        return;

    // If oprnd is a literal, find the minimum width literal register file(min_lit_prf) from MDES that
    // supports this operand. If literal is cb, add all prfs from uspc_ops for port_num, ignoring
    // the widths. For other type of literals, add all literal prfs that have width >= 
    // min_lit_prf

    if (oprnd.is_lit() && !oprnd.is_predicate()) {
        Pair<int, char *> min_lit_prf = get_lit_operand_prf(oprnd);

        if (dbg(genrio, 3))
            cdbg << "annotate_prf_port: literal oprnd for port: " << port_num 
                 << " requires : w: " << min_lit_prf.first << " prf: " << min_lit_prf.second << endl;

        // Now add to this port's prf set, all the literal PRF files(from mdes)
        // which have width >= width of min_lit_prf except for CBs
        for (List_set_iterator<Op_descr*> liter(uspc_ops); liter!=0; liter++) {
            Op_descr *op_des = *liter;
            char *op_aname = op_des->get_aname_ptr();
            Hash_set<Reg_descr*> reg_descr_set(hash_Reg_descr_ptr, 128);
            get_mdes_prf_set(port_kind, port_num, op_aname, operand_to_mdesname(oprnd), reg_descr_set);
            for (Hash_set_iterator<Reg_descr*> hiter(reg_descr_set); hiter!=0; hiter++) {
                Reg_descr *reg_descr = *hiter;
                // for a CB, add all literal prfs ignoring the width(which is always the max width)
                if (oprnd.is_cb())
                    prf_set += reg_descr->get_aname();
                else {
                    if (oprnd.is_int()) {
                      if (reg_descr->get_ilits() == NULL)
                        El_punt("annotate_physical_file_op(): no range found for lit file: %s\n", reg_descr->get_aname());
                      if (reg_descr->get_ilits()->is_valid(oprnd.int_value()))
                        prf_set += reg_descr->get_aname();
                    } else if (oprnd.is_float()) {
                      if (reg_descr->get_dlits() == 0)
                        El_punt("annotate_physical_file_op(): no range found for lit file: %s\n", reg_descr->get_aname());
                      double lb = (*(reg_descr->get_dlits()))[0];
                      double ub = (*(reg_descr->get_dlits()))[1];
                      if ((oprnd.float_value() >= lb) && (oprnd.float_value() <= ub))
                        prf_set += reg_descr->get_aname();
                    } else if (oprnd.is_double()) {
                      if (reg_descr->get_dlits() == 0)
                        El_punt("annotate_physical_file_op(): no range found for lit file: %s\n", reg_descr->get_aname());
                      double lb = (*(reg_descr->get_dlits()))[0];
                      double ub = (*(reg_descr->get_dlits()))[1];
                      if ((oprnd.double_value() >= lb) && (oprnd.double_value() <= ub))
                        prf_set += reg_descr->get_aname();
                    } else if (reg_descr->get_width() >= min_lit_prf.first) 
                      prf_set += reg_descr->get_aname(); 
                }
            }
        }

    // For macros and VRs return all the prfs as specified in the mdes for this port
    } else if (oprnd.is_macro_reg() || oprnd.is_reg()) {
        for (List_set_iterator<Op_descr*> liter(uspc_ops); liter!=0; liter++) {
            Op_descr *op_des = *liter;
            char *op_aname = op_des->get_aname_ptr();
            Hash_set<Reg_descr*> reg_descr_set(hash_Reg_descr_ptr, 128);
            get_mdes_prf_set(port_kind, port_num, op_aname, operand_to_mdesname(oprnd), reg_descr_set);
            for (Hash_set_iterator<Reg_descr*> hiter(reg_descr_set); hiter!=0; hiter++) {
                Reg_descr *reg_descr = *hiter;
                prf_set += reg_descr->get_aname();
            }
            // For macros, the allowed prfs is the intersection of macro home prfs(as specified in MDES) 
            // and this port's PRFs
            if (oprnd.is_macro_reg()) {
                Hash_set<eString>& macro_home_set = get_MDES_pf_for_macro(oprnd.name());
                prf_set *= macro_home_set;
            }
        }
    } else
        return;

    // Set prf bind attribute with prf_set
    if (port_kind == DATA_IN)
        prf_battr->set_src(port_num, prf_set);
    else if (port_kind == DATA_OUT)
        prf_battr->set_dest(port_num, prf_set);
    else
        El_punt("annotate_prf_port: invalid port_kind: %d", port_kind);
}

// estimate the usefulness of this alternative
// for each src and dest port, compute the contribution of 
// this alternative to the op's prf choices.
// if the alternative does not contribute to a port set cost to EMPTY_PRF_SET
// else cost = num of elements in(intersection(alternative's prf & port's annotation))
// if the alternative does not completely match the op's choices, check if inserting moves
// can make this a valid alternative, if not return INVALID_OPDES
static int evaluate_opdes(Op *op, Op_descr *opdes)
{
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    assert(prf_battr != 0);

    int total_cost = 0, port_cost = 0;
    for (Op_explicit_inputs srci(op); srci!=0; srci++) {
        Operand& src_oprnd = *srci;
        Port_num port = srci.get_port_num();

        if (src_oprnd.is_undefined() || src_oprnd.is_predicate())
            continue;

        Hash_set<eString> prf_set(hash_estring, 128);
        prf_set = prf_battr->src(port);
        assert(!prf_set.is_empty());

        List_set<Reg_descr*> src_regdescr_set;
        MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_IN, port, src_regdescr_set);
        Hash_set<eString> src_prf_set(hash_estring, 128);
        for (List_set_iterator<Reg_descr*> liter(src_regdescr_set); liter!=0; liter++) 
            src_prf_set += (*liter)->get_aname();

        Hash_set<eString> tmp_set(hash_estring, 128);
        tmp_set = prf_set;
        tmp_set *= src_prf_set;
        if (tmp_set.is_empty())
            port_cost = EMPTY_PRF_SET;
        else
            port_cost = tmp_set.size();

        total_cost += port_cost;
    }

    for (Op_explicit_dests desti(op); desti!=0; desti++) {
        Operand& dest_oprnd = *desti;
        Port_num port = desti.get_port_num();

        if (dest_oprnd.is_undefined())
            continue;

        Hash_set<eString> prf_set(hash_estring, 128);
        prf_set = prf_battr->dest(port);
        assert(!prf_set.is_empty());

        List_set<Reg_descr*> dest_regdescr_set;
        MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_OUT, port, dest_regdescr_set);
        Hash_set<eString> dest_prf_set(hash_estring, 128);
        for (List_set_iterator<Reg_descr*> liter(dest_regdescr_set); liter!=0; liter++) 
            dest_prf_set += (*liter)->get_aname();

        Hash_set<eString> tmp_set(hash_estring, 128);
        tmp_set = prf_set;
        tmp_set *= dest_prf_set;
        if (tmp_set.is_empty())
            port_cost = EMPTY_PRF_SET;
        else
            port_cost = tmp_set.size();

        total_cost += port_cost;
    }

    // if total_cost < 0, this alternative cannot completely satify this
    // op's choices...then total_cost tells us how close the alternative
    // is to matching the op's choices(since it is negative, the lesser the better)
    // But the alternative should be valid i.e if its choices do not completey match
    // it should be possible to insert moves to satisfy the alternatives.
    if (total_cost < 0) {
        if (dbg(genrio,1))
          cdbg << "evalute_opdes: op: " << op->id() << " has total cost < 0 " 
               << " for opdes: " << *opdes << endl;
        Op *new_op  = Codegen::clone_op(op);
        Physfile_binding_attr *prf_battr = new Physfile_binding_attr();
        set_prf_binding_attr(new_op, prf_battr);
        List_set<Op_descr*> tmp_opdes_list(opdes);
        Codegen::annotate_physical_file_op(new_op, tmp_opdes_list);

        // no moves can satisfy new_op ..so return invalid opdes
        if (Codegen::compute_estimated_moves(new_op, tmp_opdes_list) < 0) {
            remove_prf_binding_attr(new_op);
            delete new_op;
            return INVALID_OPDES;
        }

        remove_prf_binding_attr(new_op);
        delete new_op;
    }

    return total_cost;
}

// Set the prf_bind attr for op
// uspc = unit specific 
void Codegen::annotate_physical_file_op(Op *op, List_set<Op_descr *>& uspc_ops)
{
    int estimated_moves = 0;
    Hash_set<int> empty_port_num(hash_int, 8);

    assert(get_prf_binding_attr(op) != 0);

    if (dbg(genrio, 3))
        cdbg << "Annotating op: " << op->id() << endl;

    if (is_pseudo(op) || is_no_op(op)) 
        return;

    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);

    for (Op_explicit_inputs srci(op); srci!=0; srci++) {
        Operand& src_oprnd = *srci;
        Hash_set<eString> prf_set(hash_estring, 128);

        // ignore predicate true/false literals
        if (src_oprnd.is_predicate() || src_oprnd.is_undefined())
            continue;

        // For literals, get from MDES the set of all literal files for src_oprnd's port
        // that can support its width.
        if (src_oprnd.is_lit() && !src_oprnd.is_predicate()) {
            Pair<int, char *> min_lit_prf = get_lit_operand_prf(src_oprnd);

            if (dbg(genrio, 3))
                cdbg << "annotate_physical_file_op: literal oprnd for port: " << srci.get_port_num() 
                     << " for op: " << op->id() << " requires : width: " << min_lit_prf.first << " prf: " 
                     << min_lit_prf.second << endl;

            for (List_set_iterator<Op_descr*> liter(uspc_ops); liter!=0; liter++) {
                 Op_descr *op_des = *liter;
                 char *op_aname = op_des->get_aname_ptr();
                 Hash_set<Reg_descr*> reg_descr_set(hash_Reg_descr_ptr, 128);
                 get_mdes_prf_set(DATA_IN, srci.get_port_num(), op_aname, operand_to_mdesname(src_oprnd), 
                                  reg_descr_set);
                 for (Hash_set_iterator<Reg_descr*> hiter(reg_descr_set); hiter!=0; hiter++) {
                     Reg_descr *reg_descr = *hiter;
                     // for a cb, add all literal prfs ignoring the width(which is always the max width
                     // literal supported by MDES
                     if (src_oprnd.is_cb())
                         prf_set += reg_descr->get_aname();
                     else {
                         if (src_oprnd.is_int()) {
                           if (reg_descr->get_ilits() == NULL)
                             El_punt("annotate_physical_file_op(): no range found for lit file: %s\n", reg_descr->get_aname());
                           if (reg_descr->get_ilits()->is_valid(src_oprnd.int_value()))
                             prf_set += reg_descr->get_aname();
                         } else if (src_oprnd.is_float()) {
                           if (reg_descr->get_dlits() == 0)
                             El_punt("annotate_physical_file_op(): no range found for lit file: %s\n", reg_descr->get_aname());
                           double lb = (*(reg_descr->get_dlits()))[0];
                           double ub = (*(reg_descr->get_dlits()))[1];
                           if ((src_oprnd.float_value() >= lb) && (src_oprnd.float_value() <= ub))
                             prf_set += reg_descr->get_aname();
                         } else if (src_oprnd.is_double()) {
                           if (reg_descr->get_dlits() == 0)
                             El_punt("annotate_physical_file_op(): no range found for lit file: %s\n", reg_descr->get_aname());
                           double lb = (*(reg_descr->get_dlits()))[0];
                           double ub = (*(reg_descr->get_dlits()))[1];
                           if ((src_oprnd.double_value() >= lb) && (src_oprnd.double_value() <= ub))
                             prf_set += reg_descr->get_aname();
                         } else if (reg_descr->get_width() >= min_lit_prf.first) 
                           prf_set += reg_descr->get_aname(); 
                     }
                 }
            }
        } else if (src_oprnd.is_macro_reg() || src_oprnd.is_reg()) {
            for (List_set_iterator<Op_descr*> liter(uspc_ops); liter!=0; liter++) {
                 Op_descr *op_des = *liter;
                 char *op_aname = op_des->get_aname_ptr();
                 Hash_set<Reg_descr*> reg_descr_set(hash_Reg_descr_ptr, 128);
                 get_mdes_prf_set(DATA_IN, srci.get_port_num(), op_aname, operand_to_mdesname(src_oprnd), 
                                  reg_descr_set);
                 for (Hash_set_iterator<Reg_descr*> hiter(reg_descr_set); hiter!=0; hiter++) {
                     Reg_descr *reg_descr = *hiter;
                     prf_set += reg_descr->get_aname();
                 }
            }
            // For macros, intersect the MDES register files for src_oprnd's port with macro home set.
            if (src_oprnd.is_macro_reg()) {
                Hash_set<eString>& macro_home_set = get_MDES_pf_for_macro(src_oprnd.name());
                prf_set *= macro_home_set;
            }
        }

        if (dbg(genrio,3))
          cdbg << "op: " << op->id() << " src port: " << srci.get_port_num() 
               << " annotated with prf set: " << prf_set << endl;

        // if the prf set is empty, there could be a possible move
        prf_battr->set_src(srci.get_port_num(), prf_set);
        if (prf_set.is_empty()) {
            estimated_moves++;
            empty_port_num += srci.get_port_num();
        }

        if (dbg(genrio, 3))
            cdbg << "annotate_physical_file_op: estimated_moves: " << estimated_moves << endl;
    }
    
    // Try swapping the operands 
    if (estimated_moves > 0) {
        if ((MDES_commutative(get_mdes_opcode_string(op)) &&
            (op->num_srcs() == 2)) || (((is_store(op) && ((empty_port_num.is_member(SRC1)) || (empty_port_num.is_member(SRC2)))) ||
            is_load(op)) && is_mem_base_plus_offset(op))) {
            int swap_estimated_moves = 0;
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
            set_prf_binding_attr(new_op, new_prf_battr);
            annotate_prf_port(DATA_IN, SRC1, new_op->src(SRC1), new_prf_battr, uspc_ops, new_op);

            annotate_prf_port(DATA_IN, SRC2, new_op->src(SRC2), new_prf_battr, uspc_ops, new_op);

            if ((new_prf_battr->src(SRC1)).is_empty())
                swap_estimated_moves++;
            if ((new_prf_battr->src(SRC2)).is_empty())
                swap_estimated_moves++;

            if (dbg(genrio, 3))
                cdbg << "annotate_physical_file_op: swap_estimated_moves: " << swap_estimated_moves << endl;

            // use the estimated moves only of ports SRC1, SRC2, as the swap_estimated_moves
            // are only computed for those
            int e_moves = 0;
            if (empty_port_num.is_member(SRC1))
              e_moves++;
            if (empty_port_num.is_member(SRC2))
              e_moves++;
            // Now do the real swap, as swap decreases the number of moves
            if (swap_estimated_moves < e_moves) {
                if (dbg(genrio, 3))
                    cdbg << "annotate_physical_file_op: swapping srcs for op: " << op->id() << endl;

                Operand src1 = op->src(SRC1);
                Operand src2 = op->src(SRC2);

                op->set_src(SRC1, src2);
                op->set_src(SRC2, src1);
                Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
                if (rdt_attr != NULL) {
                    rdt_attr->swap_srcs(SRC1, SRC2);
                }
                annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op);
                annotate_prf_port(DATA_IN, SRC2, op->src(SRC2), prf_battr, uspc_ops, op);
            }
            remove_prf_binding_attr(new_op);
            delete new_op;
        } else if (get_reverse_op(op->opcode())) {
          Opcode swap_opc = get_reverse_op(op->opcode());
          if (!MDES_unsupported(el_opcode_to_string_map.value(swap_opc))) {
            int swap_estimated_moves = 0;
            Operand src1 = op->src(SRC1);
            Operand src2 = op->src(SRC2);

            Op *new_op = Codegen::clone_op(op);
            new_op->set_src(SRC1, src2);
            new_op->set_src(SRC2, src1);
            new_op->set_opcode(swap_opc);
            Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(new_op);
            if (rdt_attr != NULL) {
                rdt_attr->swap_srcs(SRC1, SRC2);
            }

            Physfile_binding_attr *new_prf_battr = new Physfile_binding_attr();
            set_prf_binding_attr(new_op, new_prf_battr);
            annotate_prf_port(DATA_IN, SRC1, new_op->src(SRC1), new_prf_battr, uspc_ops, new_op);
            annotate_prf_port(DATA_IN, SRC2, new_op->src(SRC2), new_prf_battr, uspc_ops, new_op);

            if ((new_prf_battr->src(SRC1)).is_empty())
                swap_estimated_moves++;
            if ((new_prf_battr->src(SRC2)).is_empty())
                swap_estimated_moves++;

            if (dbg(genrio, 3))
                cdbg << "annotate_physical_file_op: semantic swap_estimated_moves: " << swap_estimated_moves << endl;

            // Now do the real swap, as swap decreases the number of moves
            if (swap_estimated_moves < estimated_moves) {
                if (dbg(genrio, 3))
                    cdbg << "annotate_physical_file_op: swapping srcs for op: " << op->id() << endl;

                Operand src1 = op->src(SRC1);
                Operand src2 = op->src(SRC2);

                op->set_src(SRC1, src2);
                op->set_src(SRC2, src1);
                op->set_opcode(swap_opc);
                Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
                if (rdt_attr != NULL) {
                    rdt_attr->swap_srcs(SRC1, SRC2);
                }
                annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op);
                annotate_prf_port(DATA_IN, SRC2, op->src(SRC2), prf_battr, uspc_ops, op);
            }
            remove_prf_binding_attr(new_op);
            delete new_op;
          }
        }
    }

    for (Op_explicit_dests desti(op); desti!=0; desti++) {
        Operand& dest_oprnd = *desti;
        Hash_set<eString> prf_set(hash_estring, 128);
        if (dest_oprnd.is_undefined())
            continue;
        if (dest_oprnd.is_macro_reg() || dest_oprnd.is_reg()) {
            for (List_set_iterator<Op_descr*> liter(uspc_ops); liter!=0; liter++) {
                 Op_descr *op_des = *liter;
                 char *op_aname = op_des->get_aname_ptr();
                 Hash_set<Reg_descr*> reg_descr_set(hash_Reg_descr_ptr, 128);
                 get_mdes_prf_set(DATA_OUT, desti.get_port_num(), op_aname, operand_to_mdesname(dest_oprnd), 
                                  reg_descr_set);
                 for (Hash_set_iterator<Reg_descr*> hiter(reg_descr_set); hiter!=0; hiter++) {
                     Reg_descr *reg_descr = *hiter;
                     prf_set += reg_descr->get_aname();
                 }
            }

            // for macros, prune based on the macro home set
            if (dest_oprnd.is_macro_reg()) {
                Hash_set<eString>& macro_home_set = get_MDES_pf_for_macro(dest_oprnd.name());
                prf_set *= macro_home_set;
            }
        }

        if (dbg(genrio,3))	
          cdbg << "op: " << op->id() << " dest port: " << desti.get_port_num() 
               << " annotated with prf set: " << prf_set << endl;

        prf_battr->set_dest(desti.get_port_num(), prf_set);
    }
}


// Top level routine, to annotate all ops in region 'r'
// with prf bindings
// Creates new prf bindings
void Codegen::annotate_physical_files(Compound_region *r)
{
    if (dbg(genrio, 3))
        cdbg << "Annotating region: " << r->id() << " in proc: " 
             << (char*)(el_get_enclosing_procedure(r))->get_name() 
             << " with physical register files..." << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;

        if (is_pseudo(op) || is_no_op(op))
            continue;

        // Special case for MPY_WIDE where DEST1 could be undefined.
        if (is_mul_wide(op)) {
          if (op->dest(DEST1).is_undefined()) {
            op->set_dest(DEST1, op->dest(DEST2));
            op->set_dest(DEST2, new Undefined());
          }
        }

        // if the op has a prf bind attribute ignore..
        if (get_prf_binding_attr(op) != 0)
            continue;

        // Since this routine is called before clustering, we look at all possible alternatives from
        // the MDES and not just those within a specific cluster
        List_set<Op_descr *> unit_specific_opdes_set;
        MDES_collect_op_descrs(get_mdes_opcode_string(op), unit_specific_opdes_set);

        Physfile_binding_attr *prf_attr = new Physfile_binding_attr();
        set_prf_binding_attr(op, prf_attr);
        annotate_physical_file_op(op, unit_specific_opdes_set);
    }
}

// get the list of unit specific op descrs for 'op' in cluster 'cid'.
void Codegen::get_uspc_ops_in_cluster(Op *op, int cid, List_set<Op_descr*>& clust_uspc_ops)
{
    Hash_set<eString>& uspc_ops = Cluster_mgr::hw_cluster_vec[cid]->get_supported_us_opcodes();
    assert(!uspc_ops.is_empty());

    List_set<Op_descr*> uspc_op_descrs;
    MDES_collect_op_descrs(get_mdes_opcode_string(op), uspc_op_descrs);

    for (List_set_iterator<Op_descr*> liter(uspc_op_descrs); liter!=0; liter++) {
        Op_descr *op_descr = *liter;
        eString opdesn(op_descr->get_aname_ptr());

        if (uspc_ops.is_member(opdesn)) {
            clust_uspc_ops += op_descr;
	}
    }

    // if this set is empty, codegen couldn't figure out which cluster
    // to put the opcode in. perhaps your mdes has the opcode reading
    // from register files in different clusters.
    assert(!clust_uspc_ops.is_empty());
}


// For an icmove op, based on the cluster assignment of the src and dest operands
// of that op, prune the prf_choices
static void eliminate_prf_choices_for_icmoves(Op *mov_op, Cluster_mgr& cmgr, bool postpass)
{
    Physfile_binding_attr *pattr = get_prf_binding_attr(mov_op);
    assert(pattr != 0);       

    for (Op_explicit_inputs srcoprndi(mov_op); srcoprndi!=0; srcoprndi++) {
      Operand& src = *srcoprndi;
      
      // macros are not bound in operand map...perhaps becuse they have a single home
      if (src.is_undefined() || src.is_predicate() || src.is_macro_reg())
        continue;

      // In postpass, callee save registers are not bound in the operand map
      // and hence cannot be pruned.
      // Maybe the clustering or regalloc should add it to the operand map
      if (postpass) {
        if (!cmgr.operand_map.is_bound(src.vr_num()))
          continue;
      }

      assert(cmgr.operand_map.is_bound(src.vr_num()));
      int cid = cmgr.operand_map.value(src.vr_num());
      
      Port_num port = srcoprndi.get_port_num();
      Hash_set<eString>& prf_choices = pattr->src(port);
      Hash_set<Reg_descr*>& reg_descrs = cmgr.hw_cluster_vec[cid]->get_reg_descr_set();
      Hash_set<eString> valid_prfs(hash_estring, CGEN_HASH_SIZE_1);
      for (Hash_set_iterator<Reg_descr*> iter(reg_descrs); iter!=0; iter++) {
        Reg_descr *rdescr = *iter;
        valid_prfs += rdescr->get_aname();
      }
      prf_choices *= valid_prfs;
    }
    for (Op_explicit_dests destoprndi(mov_op); destoprndi!=0; destoprndi++) {
      Operand& dest = *destoprndi;
      if (dest.is_macro_reg()) continue; // macros are not bound in operand map...perhaps becuse they have a single home

      // In postpass, callee save registers are not bound in the operand map
      // and hence cannot be pruned.
      // So look at the home set and ignore if it is bound there.
      // Maybe the clustering or regalloc should add it to the operand map
      if (postpass) {
        if (Codegen::Vr_Home_Prf.is_bound(dest.vr_num()))
          continue;
       }

      assert(cmgr.operand_map.is_bound(dest.vr_num()));
      int cid = cmgr.operand_map.value(dest.vr_num());
      
      Port_num port = destoprndi.get_port_num();
      Hash_set<eString>& prf_choices = pattr->dest(port);
      Hash_set<Reg_descr*>& reg_descrs = cmgr.hw_cluster_vec[cid]->get_reg_descr_set();
      Hash_set<eString> valid_prfs(hash_estring, CGEN_HASH_SIZE_1);
      for (Hash_set_iterator<Reg_descr*> iter(reg_descrs); iter!=0; iter++) {
        Reg_descr *rdescr = *iter;
        valid_prfs += rdescr->get_aname();
      }
      prf_choices *= valid_prfs;
    }
}

// Top level routine, to prune the prf bindings of ops in region 'r'
// after clustering
void Codegen::post_cluster_prune_prf_bindings(Compound_region *r, Cluster_mgr& cmgr)
{
//     if (El_cluster_method == EL_CLSTR_NONE)
//         return;

    if (dbg(genrio, 1))
        cdbg << "Post cluster PRF pruning on region: " << r->id() << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        
        if (is_pseudo(op) || is_no_op(op))
            continue;

        Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(op);
        if (prf_bind_attr)
            get_prf_binding_attr(op)->clear_attr_entries();
        else if (prf_bind_attr == 0) {
            Physfile_binding_attr *prf_battr = new Physfile_binding_attr();
            set_prf_binding_attr(op, prf_battr);
        }

        int cid;
        List_set<Op_descr*> clust_uspc_ops;	 

        if (op->flag(EL_OPER_ICMOVE)) {
            Hash_set<eString>& uspc_icmoves = cmgr.get_ic_move_opcodes();
            assert(!uspc_icmoves.is_empty());

            List_set<Op_descr*> uspc_op_descrs;
            MDES_collect_op_descrs(get_mdes_opcode_string(op), uspc_op_descrs);

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

        //unbind_all_operands(op); 
        annotate_physical_file_op(op, clust_uspc_ops);

        if (op->flag(EL_OPER_ICMOVE))
          eliminate_prf_choices_for_icmoves(op, cmgr, false);
    }
}

static void clear_internal()
{
    Codegen::move_ops = 0;
    Codegen::two_oprnd_move_ops = 0;
    Codegen::prf_move_ops = 0;
    Codegen::bind_prf_move_ops = 0;

    Codegen::Vr_Home_Set.clear();
    Codegen::Vr_Home_Prf.clear();

    Codegen::prf_bind_map.clear_prf_map();

    static bool label_cleared = false;
    if( ! label_cleared) {
      Codegen::operand_to_label_map.clear();
      label_cleared = true;
    }
}

// Top level routine, called from process_function before clustering and scheduling
void Codegen::do_initial_codegen(Procedure *f)
{
    if (dbg(genrio,1))
        cdbg << "Initial Codegen Phase..." << endl;

    clear_internal();

    // read in from mdes the list of regfiles
    if (!init) {
        mdes_regfiles_init();
        init = true;
    }

    // annotate every op in f with its prf choices. 
    annotate_physical_files(f);
    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_initial_prf_annotate.reb", (char*)f->get_name());
        cdbg << "Printing Rebel after initial PRF annotations" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, f);
    }

    // insert moves if prf choices empty for any operand 
    if (El_do_move_codegen)
      do_move_codegen(f);
    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_initial_movecodegen.reb", (char*)f->get_name());
        cdbg << "Printing Rebel after move codegen" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, f);
    }
    if (dbg(genrio,1)) 
        cdbg << "do_move_codegen: # of moves inserted so far: " << move_ops << endl;

    // At this point, every op has been annotated with non-empty prf choices 
    // But we need to make sure that there is atleast some valid alternative 
    // that can be mapped to this op. 
    if (!El_codegen_fast_mode) {
      check_for_valid_opdes((Compound_region*)f, (Cluster_mgr *)0, false);
      if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_initial_check_for_valid_opdes.reb", (char*)f->get_name());
        cdbg << "Printing Rebel after initial checking for valid opdes" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, f);
      }
    }

    if (El_do_two_operand) {
      do_two_operand(f);
      if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_initial_twooprnd_codegen.reb", (char*)f->get_name());
        cdbg << "Printing Rebel after two operand codegen" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, f);
      }
      if (dbg(genrio,1)) 
        cdbg << "do_two_operand: # of moves inserted so far: " << two_oprnd_move_ops << endl;
    }

    if (!El_codegen_fast_mode) {
      do_physfile_move_codegen(f, false);
      if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_initial_prfmove_codegen.reb", (char*)f->get_name());
        cdbg << "Printing Rebel after PRF move codegen" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, f);
      }
      if (dbg(genrio,1)) 
        cdbg << "do_physfile_move_codegen: # of moves inserted so far: " << prf_move_ops << endl;
    }

    if (El_do_codegen_opti) {
        if (dbg(genrio,1))
            cdbg << "Applying global copy propagation in proc: " 
	         << (char*)f->get_name() << endl; 
 	if (global_copy_propagate(f)) {
 	  el_opti_eliminate_dead_code(f, ANALYZE_ALLREG, true, true);
 	}
        if (dbg(genrio,5)) {
            char buff[256];
            sprintf(buff, "%s_initial_copy_propagate.reb", (char*)f->get_name());
            cdbg << "Printing Rebel after global copy propagation (codegen)" << endl;
            IR_outstream dbg_out(buff);
            ir_write(dbg_out, f);
        }
        if (dbg(genrio,1))
            cdbg << "Applying global constant CSE in proc: " 
	         << (char*)f->get_name() << endl; 
        if (global_constant_cse_elimination(f)) {
           el_opti_eliminate_dead_code(f, ANALYZE_ALLREG, true, true);
        }
        if (dbg(genrio,5)) {
            char buff[256];
            sprintf(buff, "%s_initial_constant_CSE.reb", (char*)f->get_name());
            cdbg << "Printing Rebel after global constant CSE (codegen)" << endl;
            IR_outstream dbg_out(buff);
            ir_write(dbg_out, f);
        }
	if (dbg(genrio,1))
	  cdbg << "Applying loop invariant opti in proc: " 
	       << (char*)f->get_name() << endl;
	create_local_analysis_info_for_all_hbs_bbs(f);
	el_flow_compute_liveness(f, ANALYZE_ALL);
	El_do_reaching_defs(f, ANALYZE_ALL);
	Reaching_defs_info* rdinfo = get_reaching_defs_info(f);
	
 	el_opti_remove_loop_invariants(f, rdinfo) ;
	
	remove_reaching_defs_info(f) ;
	delete_local_analysis_info_for_all_hbs_bbs(f);
	el_flow_delete_liveness(f);
        if (dbg(genrio,5)) {
            char buff[256];
            sprintf(buff, "%s_initial_loop_invariant.reb", (char*)f->get_name());
            cdbg << "Printing Rebel after loop invariant opti (codegen)" << endl;
            IR_outstream dbg_out(buff);
            ir_write(dbg_out, f);
        }
    }

    if (El_create_stack_frame) {
     Codegen::create_stack_frame_prepass(f);
    }

    // A previous pass of do_physfile_move_codegen over the whole procedure
    // has populated the home set. So clear it before calling calling it 
    // on a per region basis during do_prepass_post_cluster_codegen.
    Codegen::Vr_Home_Set.clear();
}

void Codegen::check_files_bound(Compound_region *r)
{
    if (dbg(genrio, 3))
        cdbg << endl << "checking for physical file bindings..." << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;
        for (Op_explicit_operands oprndi(op); oprndi!=0; oprndi++) {
            Operand& oprnd = *oprndi;
            if (oprnd.is_macro_reg() || oprnd.is_reg() || (oprnd.is_lit() && !oprnd.is_predicate())) {
                if (!oprnd.assigned_to_physical_file())
                    El_punt("check_files_bound: op: %d has unbound operands", op->id());
            }
        }
    }
}

// This routine does a round of codegen after clustering and binds physical files
// before scheduling
void Codegen::do_prepass_post_cluster_codegen(Compound_region *r, Cluster_mgr& cmgr)
{
    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_before_prepass_postcluster_prf_annotate_codegen.reb", 
                      (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel before Prepass Postcluster prf pruning for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }

    if (dbg(genrio,1))
        cdbg << "Prepass Post cluster Codegen on region: " << r->id() << endl;

    post_cluster_prune_prf_bindings(r, cmgr);
    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_prepass_postcluster_prf_annotate_codegen.reb", 
                (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << endl << "Printing Rebel after Prepass Postcluster prf pruning for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }

    if (!El_codegen_fast_mode) {
      do_post_cluster_move_codegen(r, cmgr);
      if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_prepass_postcluster_movecodegen.reb", 
                (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel after Prepass Postcluster move codegen for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
      }
      if (dbg(genrio,1))
        cdbg << "do_post_cluster_move_codegen: # moves inserted so far: " << move_ops << endl;
    }

    if (!El_codegen_fast_mode) {
      check_for_valid_opdes(r, &cmgr, true);
      if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_prepass_postcluster_check_for_valid_opdes.reb", 
                (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel after Prepass Postcluster checking for valid opdes" << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
      }
    }

    if (!El_codegen_fast_mode) {
      do_physfile_move_codegen(r, true);
      if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_prepass_postcluster_prfmove_codegen.reb", 
                (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel after Prepass Postcluster phys file move for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
      }
      if (dbg(genrio,1))
        cdbg << "do_physfile_move_codegen: # moves inserted so far: " << prf_move_ops << endl;
    }

    if (El_do_codegen_opti) {
        if (dbg(genrio,1))
            cdbg << "Applying local copy propagation in region: " << r->id() 
                 << " of proc: " << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;
        if (local_copy_propagate(r, cmgr, true, false)) {
            el_opti_eliminate_dead_code(r, ANALYZE_ALLREG, true, true);
        }
        if (dbg(genrio,5)) {
          char buff[256];
          sprintf(buff, "%s_%d_prepass_postcluster_copy_propagate.reb", 
                  (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
          cdbg << "Printing Rebel after Prepass Postcluster copy propagation for region: " << r->id() << endl;
          IR_outstream dbg_out(buff);
          ir_write(dbg_out, el_get_enclosing_procedure(r));
        }
    }

    if (dbg(status,2))
      cdbg << "Performing prepass codegen and bind physical files on region: " << r->id()
           << " within procedure: " << (char*)(el_get_enclosing_procedure(r)->get_name()) << endl;

    bind_physical_files(r, cmgr);
    prf_bind_map.do_prf_bind(r, cmgr);

    if (dbg(genrio,5)) {
      char buff[256];
      sprintf(buff, "%s_%d_prepass_postcluster_prfbind_codegen.reb",
              (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
      cdbg << "Printing Rebel after Prepass Postcluster PRF binding for region: " << r->id() << endl;
      IR_outstream dbg_out(buff);
      ir_write(dbg_out, el_get_enclosing_procedure(r));
    }

    check_files_bound(r);

    // This sets the Home_Sets and Home_Prfs for all VRs and macros in region r
    // so that the subsequent regions can honor these bindings. This is kind of like 
    // postpass_codegen_initialize, but works on a region. Strictly speaking this is not
    // necessary as physfile_move_codegen and insert_post_bind_prf_moves initializes the home sets.
    // But in fast mode, physfile_move_codegen and insert_post_bind_prf_moves are not called.
    // This helps to initialize the Home_Set and Home_Prf
    // Also it acts as a low overhead sanity check
    if (El_codegen_fast_mode)
      update_prf_home(r);
}

// Initialize the Vr_Home_Set/Vr_Home_Prf to be used in postpass post cluster codegen
// during physfile_move_codegen/bind_physical_file with callee saved registers.
void Codegen::init_codegen_homeset(Operand& oprnd, eString prf)
{
    if (oprnd.is_reg()) {
        assert(!Vr_Home_Set.is_bound(oprnd.vr_num()));
        assert(!Vr_Home_Prf.is_bound(oprnd.vr_num()));

        Hash_set<eString> prf_set(hash_estring, 128);
        prf_set += prf;
        Vr_Home_Set.bind(oprnd.vr_num(), prf_set);

        Vr_Home_Prf.bind(oprnd.vr_num(), prf);
    } else if (oprnd.is_macro_reg()) {
        assert(Macro_Home_Set.is_bound(oprnd.name()));
        assert(Macro_Home_Set.value(oprnd.name()).is_member(prf));
    }
}

// Called before reg alloc, creates a home_prf_set and home_prf for register/macros
// The home_prf_set is assigned to macros/vrs in non-spill code as these have already
// been allocated to registers and the prf bindings confirm to MDES and we do not in
// postpass post cluster codegen wish to change their bindings(during physfile_move_codegen 
// and bind_physical_files.
void Codegen::postpass_codegen_initialize(Procedure *f)
{
    // debug. print rebel prior to postpass phase...
    if (dbg(genrio,5)) {
      char buff[256];
      sprintf(buff, "%s_prepass_codegen.reb", (char*)f->get_name());
      cdbg << "Printing Rebel after Prepass Codegen" << endl;
      IR_outstream dbg_out(buff);
      ir_write(dbg_out, f);
    }
 
    if (dbg(genrio, 1))
        cdbg << "Building home prf set before reg alloc..." << endl;

    Codegen::Vr_Home_Set.clear();
    Codegen::Vr_Home_Prf.clear();

    for (Region_all_ops opi(f); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;

        for (Op_explicit_inputs srci(op); srci!=0; srci++) {
	  Operand& src_oprnd = *srci;
	  if (src_oprnd.is_reg()) {
	    Hash_set<eString> prf_set(hash_estring, 128);
	    prf_set += src_oprnd.physical_file_type();
	    if (Vr_Home_Set.is_bound(src_oprnd.vr_num())) {
	      if(Vr_Home_Set.value(src_oprnd.vr_num()) != prf_set) {
		if(prf_set.size() == 0 || ((prf_set.size() == 1) &&
					   prf_set.head() == "")) {
		  // The PRF wasn't bound for some reason, but
		  // it's easy to recover from this.
		  prf_set.clear();
		  assert(Vr_Home_Set.value(src_oprnd.vr_num()).size() == 1);
		  eString prf = Vr_Home_Set.value(src_oprnd.vr_num()).head();
		  src_oprnd.bind_physical_file(prf);
		  prf_set += prf;
		}
	      }

	      assert(Vr_Home_Set.value(src_oprnd.vr_num()) == prf_set);
	    } else {
	      Vr_Home_Set.bind(src_oprnd.vr_num(), prf_set);
	    }
                
	    if (Vr_Home_Prf.is_bound(src_oprnd.vr_num()))
	      assert(Vr_Home_Prf.value(src_oprnd.vr_num()) == src_oprnd.physical_file_type());
	    else
	      Vr_Home_Prf.bind(src_oprnd.vr_num(), src_oprnd.physical_file_type());
	  } else if (src_oprnd.is_macro_reg()) {
	    eString prf = src_oprnd.physical_file_type();
	    if( ! Macro_Home_Set.value(src_oprnd.name()).is_member(prf)) {
	      Hash_set<eString>& home_set = Macro_Home_Set.value(src_oprnd.name());
	      if((home_set.size() == 1) && (prf == "")) {
		src_oprnd.bind_physical_file(home_set.head());
	      } else {
		cdbg << "ERROR: the physical register file, '" << prf << "', "
		     << "for operand\n" << src_oprnd << " is not in the "
		     << "Macro_Home_Set (whatever the hell\nthat means)."
		     << " This op is the culprit: " << *op << "\n\n" << endl;
		assert(0);
	      }
	    }
	  }
        }

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
            Operand& dest_oprnd = *desti;
            if (dest_oprnd.is_reg()) {
                Hash_set<eString> prf_set(hash_estring, 128);
                prf_set += dest_oprnd.physical_file_type();
                if (Vr_Home_Set.is_bound(dest_oprnd.vr_num())) {
		  if(Vr_Home_Set.value(dest_oprnd.vr_num()) != prf_set) {
		    if(prf_set.size() == 0 || ((prf_set.size() == 1) &&
					       prf_set.head() == "")) {
		      prf_set.clear();

		      // The PRF wasn't bound for some reason, but
		      // it's easy to recover from this.
		      assert(Vr_Home_Set.value(dest_oprnd.vr_num()).size() == 1);
		      eString prf = Vr_Home_Set.value(dest_oprnd.vr_num()).head();
		      dest_oprnd.bind_physical_file(prf);
		      prf_set += prf;
		    }
		  }

		  assert(Vr_Home_Set.value(dest_oprnd.vr_num()) == prf_set);
		} else {
                    Vr_Home_Set.bind(dest_oprnd.vr_num(), prf_set);
		}
               
                if (Vr_Home_Prf.is_bound(dest_oprnd.vr_num()))
                    assert(Vr_Home_Prf.value(dest_oprnd.vr_num()) == dest_oprnd.physical_file_type());
                else
                    Vr_Home_Prf.bind(dest_oprnd.vr_num(), dest_oprnd.physical_file_type());

            } else if (dest_oprnd.is_macro_reg()) {
                eString prf = dest_oprnd.physical_file_type();
		Hash_set<eString>& home_set = Macro_Home_Set.value(dest_oprnd.name());
		if((home_set.size() == 1) && (prf == "")) {
		  dest_oprnd.bind_physical_file(home_set.head());
		} else {
		  assert(Macro_Home_Set.value(dest_oprnd.name()).is_member(prf));
		}
            }
        }
    }

    if (dbg(genrio,7))
        print_home_sets();

/*
    move_ops = 0;
    two_oprnd_move_ops = 0;
    prf_move_ops = 0;
    bind_prf_move_ops = 0;
*/
}

// Initialize the Vr_Home_Set/Vr_Home_Prf to be used in postpass post cluster codegen
// during physfile_move_codegen/bind_physical_file. The sets are initialized
// with VRs that were created during register allocation.
// Called after register allocation
void Codegen::post_regalloc_codegen_initialize(Procedure *f)
{
  for (Region_all_ops opi(f); opi!=0; opi++) {
    Op *op = *opi;
    for (Op_all_operands oprndi(op); oprndi!=0; oprndi++) {
      Operand oprnd = *oprndi;
      if (oprnd.is_reg() && !Vr_Home_Prf.is_bound(oprnd.vr_num()))
        init_codegen_homeset(oprnd, oprnd.physical_file_type());
      // Macros created during regalloc are never bound, so needn't be initialized
    }
  }
}

void Codegen::print_home_sets()
{
    cdbg << "VR Home Set..." << endl;
    for (Hash_map_iterator<int,Hash_set<eString> > hm1(Vr_Home_Set); hm1!=0; hm1++) {
        cdbg << endl << "vr: " << (*hm1).first;
        for (Hash_set_iterator<eString> hs1((*hm1).second); hs1!=0; hs1++) 
            cdbg << " " << (*hs1);
    }
    cdbg << endl << "Macro Home Set...";
    for (Hash_map_iterator<Macro_name,Hash_set<eString> > hm2(Macro_Home_Set); hm2!=0; hm2++) {
        cdbg << endl << "Macro: " << macro_reg_to_text((*hm2).first);
        for (Hash_set_iterator<eString> hs1((*hm2).second); hs1!=0; hs1++) 
            cdbg << " " << (*hs1);
    }
    cdbg << endl << "VR Home Prf..." << endl;
    for (Hash_map_iterator<int,eString> hm3(Vr_Home_Prf); hm3!=0; hm3++) 
        cdbg << endl << "vr: " << (*hm3).first << " " << (*hm3).second;
    cdbg << endl;
}

// Called after register allocation and clustering but before post pass
// scheduling to deal with spill ops
void Codegen::do_postpass_post_cluster_codegen(Compound_region *r, Cluster_mgr& cmgr)
{
    if (dbg(genrio,1))
        cdbg << "Postpass Post cluster Codegen on region " << r->id() << endl;

    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_before_postpass_postcluster_prf_annotate_codegen.reb", 
                      (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel before Postpass Postcluster prf pruning for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }

    // Spill code does not have prf_bind annoations, 
    // Also, pracheeti's ops for stack manipulation..
    // So annotate them, insert_move_ops(if annotations are empty)
    // also convert them to 2 operand format
    // Codegen is done for spill code and inter cluster moves inserted
    // after for the spill code. The inter cluster moves also have 
    // EL_OPER_SPILL_CODE flag set.

    for (Region_all_ops opi1(r); opi1!=0; opi1++) {
        Op *op = *opi1;
        if (is_pseudo(op) || is_no_op(op))
            continue;
        Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
        if (!prf_battr) {
            int cid;
            List_set<Op_descr*> clust_uspc_ops;	 

            if (op->flag(EL_OPER_ICMOVE)) {
                Hash_set<eString> uspc_icmoves(hash_estring, 128);
                uspc_icmoves = cmgr.get_ic_move_opcodes();

                List_set<Op_descr*> uspc_op_descrs;
                MDES_collect_op_descrs(get_mdes_opcode_string(op), uspc_op_descrs);

                for (List_set_iterator<Op_descr*> liter(uspc_op_descrs); liter!=0; liter++) {
                    Op_descr *op_descr = *liter;
                    if (uspc_icmoves.is_member(op_descr->get_aname_ptr()))
                        clust_uspc_ops += op_descr;
                }
            } else {
	      if( ! Cluster_mgr::op_map.is_bound(op)) {
		cdbg << "ERROR: This operation isn't assigned to a cluster!\n"
		     << *op << endl << endl;
		assert(0);
	      }
	      cid = Cluster_mgr::op_map.value(op);
	      get_uspc_ops_in_cluster(op, cid, clust_uspc_ops);
            }

            assert(!clust_uspc_ops.is_empty());

            Physfile_binding_attr *prf_attr = new Physfile_binding_attr();
            set_prf_binding_attr(op, prf_attr);

            // since this op had no prf_bind attr undo all regfile bindings
            unbind_all_operands(op);

            annotate_physical_file_op(op, clust_uspc_ops); 

            if (op->flag(EL_OPER_ICMOVE))
              eliminate_prf_choices_for_icmoves(op, cmgr, true);

            insert_move_ops(op, clust_uspc_ops, true);

            if (!El_codegen_fast_mode) {
              check_for_valid_opdes_op(op, clust_uspc_ops, true);
            }
        }
        if (El_do_two_operand) {
          List_set<Op_descr*> clust_uspc_ops;	 
          assert(Cluster_mgr::op_map.is_bound(op));
          int cid = Cluster_mgr::op_map.value(op);
          get_uspc_ops_in_cluster(op, cid, clust_uspc_ops);
          do_two_oprnd_convert(op, clust_uspc_ops, true);
        }
    }

    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_postpass_b4prf_move_codegen.reb", 
                (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel before Postpass physfile_move_codegen for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }
    if (dbg(genrio,1)) 
        cdbg << "Postpass insert_move_ops: # of moves inserted so far: " << move_ops << endl;
    if (dbg(genrio,1)) 
        cdbg << "Postpass do_two_operand: # of moves inserted so far: " << two_oprnd_move_ops << endl;

    if (!El_codegen_fast_mode) {
      do_physfile_move_codegen(r, true);
      if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_postpass_prfmove_codegen.reb", 
                (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel after Postpass physfile_move_codegen for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
      }
      if (dbg(genrio,1))
        cdbg << "Postpass do_physfile_move_codegen: # moves inserted so far: " << prf_move_ops << endl;
    }

    if (El_do_codegen_opti) {
        if (dbg(genrio,1))
            cdbg << "Applying local copy propagation in region: " << r->id() 
                 << " of proc: " << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;
        if (local_copy_propagate(r, cmgr, true, true)) {
            el_opti_eliminate_dead_code(r, ANALYZE_ALLREG, true, true);
        }
        if (dbg(genrio,5)) {
          char buff[256];
          sprintf(buff, "%s_%d_postpass_copy_propagate.reb", 
                        (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
          cdbg << "Printing Rebel after Postpass copy propagation" << endl;
          IR_outstream dbg_out(buff);
          ir_write(dbg_out, el_get_enclosing_procedure(r));
        }
    }

    // Clear the binding map.
    prf_bind_map.clear_prf_map();

    if (dbg(status,2))
      cdbg << "Performing postpass codegen and icmove register assignment on region: " << r->id()
           << " within procedure: " << (char*)el_get_enclosing_procedure(r)->get_name() << endl;

    bind_physical_files(r, cmgr);
    prf_bind_map.do_prf_bind(r, cmgr);

    if (dbg(genrio,5)) {
      char buff[256];
      sprintf(buff, "%s_%d_postpass_prfbind_codegen.reb", (char*)(el_get_enclosing_procedure(r))->get_name(),
              r->id());
      cdbg << "Printing Rebel after Postpass bind prf for region: " << r->id() << endl;
      IR_outstream dbg_out(buff);
      ir_write(dbg_out, el_get_enclosing_procedure(r));
    }

    check_files_bound(r);

    // This sets the Home_Sets and Home_Prfs for all VRs and macros in region r
    // so that the subsequent regions can honor these bindings. This is kind of like 
    // postpass_codegen_initialize, but works on a region. Strictly speaking this is not
    // necessary as physfile_move_codegen and insert_post_bind_prf_moves initializes the home sets.
    // In fast mode, physfile_move_codegen and insert_post_bind_prf_moves are not called.
    // This helps to initialize the Home_Set and Home_Prf
    // Also it acts as a low overhead sanity check
    if (El_codegen_fast_mode)
      update_prf_home(r);

    prf_bind_map.allocate_icmove_reg(r);

    // Removed the call to allocate_unbound_macros.  This was really only binding
    // the icmove_reg in spill code during postpass.  The function 
    // allocate_icmove_reg was changed to actually bind the register in the 
    // appropriate file, so this call was made unncessary, unless we start using 
    // other macros in spill code in the future.  - mchu 1/2006
    //
    // Turned allocate_unbound_macros back on.  Callee-save code also has some
    // unbound macros - mchu 1/2007
    prf_bind_map.allocate_unbound_macros(r);
    prf_bind_map.check_bound_reg(r);

    fix_op_formats(r, false);
}

void Codegen::get_mdes_prf_set_all(Op *op, Port_num port, IO_Portkind portkind, 
                                 char* uspc_op, Hash_set<eString>& prf_set)
{
    Hash_set<Reg_descr*> reg_descr_set(hash_Reg_descr_ptr, 128);
    if (portkind == DATA_IN)
        Codegen::get_mdes_prf_set(portkind, port, uspc_op, operand_to_mdesname(op->src(port)), reg_descr_set);
    else
        Codegen::get_mdes_prf_set(portkind, port, uspc_op, operand_to_mdesname(op->dest(port)), reg_descr_set);
    for (Hash_set_iterator<Reg_descr*> hiter(reg_descr_set); hiter!=0; hiter++) {
        Reg_descr *reg_descr = *hiter;
        prf_set += reg_descr->get_aname();
    }
}

// Can op's operand at port_num(src/dest depending on port_kind) be replaced by oprnd
// Replacement is possible if the resultant prf_choice at port_num is equal to or a 
// proper superset of its original choice.
bool Codegen::prf_bind_replace(Op *op, Operand& oprnd, IO_Portkind port_kind, 
                               Port_num port_num, List_set<Op_descr*>& uspc_ops)
{
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    Hash_set<eString> prf_set(hash_estring, 128);
    if (port_kind == DATA_OUT)
        prf_set = prf_battr->dest(port_num);
    else if (port_kind == DATA_IN)
        prf_set = prf_battr->src(port_num);

    Op *clone_op = Codegen::clone_op(op);
    Physfile_binding_attr *clone_prf_battr = new Physfile_binding_attr();

    if (port_kind == DATA_OUT) 
        clone_op->set_dest(port_num, oprnd);
    else if (port_kind == DATA_IN)
        clone_op->set_src(port_num, oprnd);

    if (port_kind == DATA_OUT) 
        annotate_prf_port(DATA_OUT, port_num, clone_op->dest(port_num), clone_prf_battr, uspc_ops, clone_op);
    else
        annotate_prf_port(DATA_IN, port_num, clone_op->src(port_num), clone_prf_battr, uspc_ops, clone_op);

    Hash_set<eString> clone_prf_set(hash_estring, 128);
    if (port_kind == DATA_OUT)
        clone_prf_set = clone_prf_battr->dest(port_num);
    else if (port_kind == DATA_IN)
        clone_prf_set = clone_prf_battr->src(port_num);

    remove_prf_binding_attr(clone_op);
    delete clone_op; 

    if (clone_prf_set.is_empty())
        return false;

    if (clone_prf_set.is_subset(prf_set))
        return true;
    else
        return false;

#if 0
    if (clone_prf_set != prf_set && (!prf_set.is_subset(clone_prf_set))) 
        return false;
    else
        return true;
#endif
}

// Can oprnd be placed at port_num(src/dest, depending on port_kind) of op
// and the resulting prf choice for that port be not empty?
bool Codegen::check_replace_prf_bind_empty(Op *op, Operand& oprnd, IO_Portkind port_kind, 
                                 Port_num port_num, List_set<Op_descr*>& uspc_ops)
{
    Op *new_op = op->clone();
    if (port_kind == DATA_IN)
        new_op->set_src(port_num, oprnd);
    else if (port_kind == DATA_OUT)
        new_op->set_dest(port_num, oprnd);
    
    Physfile_binding_attr new_prf_battr;
    if (port_kind == DATA_IN)
        Codegen::annotate_prf_port(DATA_IN, port_num, new_op->src(port_num), 
                                   &new_prf_battr, uspc_ops, op);
    else if (port_kind == DATA_OUT)
        Codegen::annotate_prf_port(DATA_OUT, port_num, new_op->dest(port_num), 
                                   &new_prf_battr, uspc_ops, op);

    Hash_set<eString> prf_set;
    if (port_kind == DATA_IN) 
        prf_set = new_prf_battr.src(port_num);
    else if (port_kind == DATA_OUT)
        prf_set = new_prf_battr.dest(port_num);

    delete new_op;

    if (prf_set.is_empty())
        return true;
    else
        return false;
}

void Codegen::check_for_valid_opdes(Compound_region *r, Cluster_mgr* cmgr, bool post_cluster)
{
    if (dbg(genrio,3))
        cdbg << "Checking for valid opdes in region: " << r->id() 
             << " of proc: " << (char*)(el_get_enclosing_procedure(r)->get_name()) << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;

        Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
        assert(prf_battr != 0);

        int cid;
        List_set<Op_descr*> uspc_ops;
    
        // if this is called post-clustering,
        //   if its an ICMOVE op, get its alternatives from MDES
        //   else get its alternatives from the cluster to which it is assigned
        // else get all its alternatives from the MDES
        if (post_cluster == true) {
            if (op->flag(EL_OPER_ICMOVE)) {
                Hash_set<eString> uspc_icmoves(hash_estring, 128);
                uspc_icmoves = cmgr->get_ic_move_opcodes();

                List_set<Op_descr*> uspc_op_descrs;
                MDES_collect_op_descrs(get_mdes_opcode_string(op), uspc_op_descrs);

                for (List_set_iterator<Op_descr*> liter(uspc_op_descrs); liter!=0; liter++) {
                    Op_descr *op_descr = *liter;
                    if (uspc_icmoves.is_member(op_descr->get_aname_ptr()))
                        uspc_ops += op_descr;
                }
            } else {
                assert(Cluster_mgr::op_map.is_bound(op));
                cid = Cluster_mgr::op_map.value(op);
                Codegen::get_uspc_ops_in_cluster(op, cid, uspc_ops);
            }
        } else {
            MDES_collect_op_descrs(get_mdes_opcode_string(op), uspc_ops);
        }

        check_for_valid_opdes_op(op, uspc_ops, post_cluster);
    }
}

Op_descr *Codegen::check_for_valid_opdes_op(Op *op, List_set<Op_descr*>& uspc_ops, bool post_cluster)
{
    assert(!uspc_ops.is_empty());

    int max_cost = INVALID_OPDES, swap_max_cost = INVALID_OPDES;
    Op_descr *best_opdes = 0, *swap_best_opdes = 0, *annot_opdes = 0;

    // Find the alternative
    // cost can be negative.
    for (List_set_iterator<Op_descr*> liter(uspc_ops); liter!=0; liter++) {
        Op_descr *opdes = *liter;
        int cost = evaluate_opdes(op, opdes);
        if (cost > max_cost) {
            best_opdes = opdes;
	    max_cost = cost;
        }
    }

    // if max_cost is negative, no alternative can completely satisfy the op's choices
    // try swapping the op
    if (max_cost < 0) {
        if (dbg(genrio,1)) {
            cdbg << "check_for_valid_opdes: op: " << op->id()
	         << " has no valid opdes!" << endl;
        }

        // try swapping operands
        if ((MDES_commutative(get_mdes_opcode_string(op)) && (op->num_srcs() == 2)) ||
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
            set_prf_binding_attr(new_op, new_prf_battr);
            annotate_prf_port(DATA_IN, SRC1, new_op->src(SRC1), new_prf_battr, uspc_ops, new_op);
            annotate_prf_port(DATA_IN, SRC2, new_op->src(SRC2), new_prf_battr, uspc_ops, new_op);

            // if swap results in empty prf binding...
            if (new_prf_battr->src(SRC1).is_empty() || new_prf_battr->src(SRC2).is_empty()) 
                swap_max_cost = INVALID_OPDES;
            else {
                for (List_set_iterator<Op_descr*> liter(uspc_ops); liter!=0; liter++) {
                    Op_descr *opdes = *liter;
                    int cost = evaluate_opdes(new_op, opdes);
                    if (cost > swap_max_cost) {
                        swap_best_opdes = opdes;
	                swap_max_cost = cost;
                    }
                }
            }
            remove_prf_binding_attr(new_op);
            delete new_op;
        } else 
            swap_max_cost = INVALID_OPDES; 

        if (swap_max_cost < 0) {
            if (dbg(genrio,1)) {
                cdbg << "check_for_valid_opdes: swap(trying) op: " << op->id()
	             << " has no valid opdes!" << endl;
            }
        }
    // if max_cost > 0, then there is a valid alternative which can satify the op's choices.. so return
    } else
        return best_opdes;

    if ((swap_max_cost == INVALID_OPDES) && (max_cost == INVALID_OPDES))
        El_punt("no valid opdes to satisfy op: %d\n", op->id());

    // max_cost is negative..i.e for the normal case we only have the closest matching opdes which is best_opdes
    // if swap_max_cost > max_cost then do swap
    // we need to handle the case when swap_max_cost > 0, i.e swapping resulted in an alternative that
    // can satisfy the swapped op's choices in which case, swap the op and annotate it and return
    // if swap_max_cost < 0, then the closest matching opdes is swap_best_opdes
    if (swap_max_cost > max_cost) {

        if (dbg(genrio, 3))
            cdbg << "check_for_valid_opdes: swapping srcs for op: " << op->id()
                 << " because of invalid move choice.." << endl;

        assert(swap_best_opdes != 0);
        Operand src1 = op->src(SRC1);
        Operand src2 = op->src(SRC2);

        op->set_src(SRC1, src2);
        op->set_src(SRC2, src1);
        Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
        if (rdt_attr != NULL) {
            rdt_attr->swap_srcs(SRC1, SRC2);
        }

        if (swap_max_cost > 0) {
            Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
            assert(prf_battr != 0);
            annotate_prf_port(DATA_IN, SRC1, op->src(SRC1), prf_battr, uspc_ops, op);
            annotate_prf_port(DATA_IN, SRC2, op->src(SRC2), prf_battr, uspc_ops, op);
            return swap_best_opdes;
        }

        annot_opdes = swap_best_opdes;
    } else
        annot_opdes = best_opdes;

    // annot_opdes is the best opdes for this op(swapped/normal)
    // and its cost < 0 i.e it is the best closest matching valid opdes
    assert(annot_opdes!=0);

    get_prf_binding_attr(op)->clear_attr_entries();
    List_set<Op_descr*> new_opdes_list(annot_opdes);

    annotate_physical_file_op(op, new_opdes_list);
    do_actual_move_insertion(op, new_opdes_list, post_cluster);

    if (dbg(genrio,5)) {
        cdbg << "check_for_valid_opdes_op: assigning opdes: " << annot_opdes->get_aname_ptr() 
             << " for op: " << op->id() << endl;
    }

    return annot_opdes;
}

// Return the width of prf
int Codegen::get_prf_width(eString prf)
{
    for(List_iterator<Pair<char*, int> > liter(Prf_Width_List); liter!=0; liter++) {
        char *p = (char *)prf;
        if (!strcmp(p, (*liter).first)) 
            return (*liter).second;
    }

    El_punt("Physical register file %s not found in Prf_Width_List\n", (char*)prf);

    return 0;
}

void Codegen::prune_prf_based_on_width(Operand& oprnd, Hash_set<eString>& prf_set, Op *op)
{
    // for now only for registers..
    if (!oprnd.is_reg() && !oprnd.is_macro_reg())
        return;

    if (!strcasecmp(El_model, "16")) {
#if 0
      // Don't do the pruning. The prf_select_heuristics has been changed to pick a GPR when there
      // is choice of both AR and GPR.
      if ((is_store(op) && (oprnd == op->src(SRC1))) ||
          (is_load(op) && (oprnd == op->src(SRC1))) ||
          (is_load(op) && op->flag(EL_OPER_SPILL_CODE) && (oprnd == op->dest(DEST1))) ||
          (is_store(op) && op->flag(EL_OPER_SPILL_CODE) && (oprnd == op->src(SRC3)))) { 
        return;
      } else {
        eString ar_prf("AR");
        eString gpr_prf("GPR");
        if (prf_set.is_member(ar_prf) && prf_set.is_member(gpr_prf))
          prf_set -= ar_prf;
      }
#endif
    } else if (!strcasecmp(El_model, "24")) {
      DType dt = oprnd.dtype();
      if (dt.width() == 16) {
        eString ar_prf("AR");
        prf_set -= ar_prf;
      } else if (dt.width() == 24) {
        eString gpr_prf("GPR");
        prf_set -= gpr_prf;
      }
    } else
      El_punt("Codegen::prune_prf_based_on_width: unknown model %s\n", El_model);
}

#if 0
// This should be the actual width based prf pruning function
// replacing the above definition
// Eliminates those prfs from prf whose width is lesser than the operand width
// What about literals, specifically cb/label/string
void Codegen::prune_prf_based_on_width(Operand& oprnd, Hash_set<eString>& prf_set)
{
    Hash_set<eString> delete_prf_set;

    // literals have been treated earlier during prf annotate phase. so ignore them
    // For ints/floats/labels the list of prf choices is those which can satisfy the width
    // For CBs all literal files are supported
    // For labels and strings only max literal file is supported
    if (oprnd.is_literal())
        return;

    DType dt = oprnd.dtype();
    for (Hash_set_iterator<eString> hiter(prf_set); hiter!=0; hiter++) {
        eString prf = *hiter;
        unsigned int w = get_prf_width(prf);
        if (dt.width() > w) 
            delete_prf_set += prf;
    }
    prf_set -= delete_prf_set;
}
#endif

// all binding done..check if the bindings are valid..i.e there is a legal alternative
// that satisfies the binding
bool Codegen::check_for_valid_bind(Op *op, Cluster_mgr& cmgr)
{
    bool try_next_uspc;

    List_set<Op_descr*> clust_uspc_ops;

    if (op->flag(EL_OPER_ICMOVE)) {
        Hash_set<eString> uspc_icmoves(hash_estring, 128);
        uspc_icmoves = cmgr.get_ic_move_opcodes();

        List_set<Op_descr*> uspc_op_descrs;
        MDES_collect_op_descrs(get_mdes_opcode_string(op), uspc_op_descrs);

        for (List_set_iterator<Op_descr*> liter(uspc_op_descrs); liter!=0; liter++) {
            Op_descr *op_descr = *liter;
            if (uspc_icmoves.is_member(op_descr->get_aname_ptr()))
                clust_uspc_ops += op_descr;
        }
    } else {
        assert(Cluster_mgr::op_map.is_bound(op));
        int cid = Cluster_mgr::op_map.value(op);
        Codegen::get_uspc_ops_in_cluster(op, cid, clust_uspc_ops);
    }

    for (List_set_iterator<Op_descr*> liter(clust_uspc_ops); liter!=0; liter++) {
        try_next_uspc = false;
        Op_descr* opdes = *liter;
        for (Op_explicit_inputs srci(op); srci!=0; srci++) {
            Operand src_oprnd = *srci;
            Port_num port = srci.get_port_num();

            if (src_oprnd.is_undefined() || src_oprnd.is_predicate())
                continue;

            List_set<Reg_descr*> src_regdescr_set;
            MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_IN, port, src_regdescr_set);
            Hash_set<eString> src_prf_set(hash_estring, 128);
            for (List_set_iterator<Reg_descr*> liter(src_regdescr_set); liter!=0; liter++) {
                src_prf_set += (*liter)->get_aname();
            }

            if (!src_prf_set.is_member(src_oprnd.physical_file_type())) {
                try_next_uspc = true;
                break;
            }
        }

        if (try_next_uspc == true)
            continue;

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
            Operand& dest_oprnd = *desti;
            Port_num port = desti.get_port_num();

            if (dest_oprnd.is_undefined())
                continue;

            List_set<Reg_descr*> dest_regdescr_set;
            MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_OUT, port, dest_regdescr_set);
            Hash_set<eString> dest_prf_set(hash_estring, 128);
            for (List_set_iterator<Reg_descr*> liter(dest_regdescr_set); liter!=0; liter++) {
                dest_prf_set += (*liter)->get_aname();
            }

            if (!dest_prf_set.is_member(dest_oprnd.physical_file_type())) {
                try_next_uspc = true;
                break;
            }
        }

        if (try_next_uspc == true)
            continue;
        else
            return true;
    }

    return false;
}

// This sets the Home_Sets and Home_Prfs for all VRs and macros in region r
// so that the subsequent regions can honor these bindings. This is kind of like 
// postpass_codegen_initialize, but works on a region. Strictly speaking this is not
// necessary as physfile_move_codegen and insert_post_bind_prf_moves initializes the home sets.
// In fast mode, physfile_move_codegen and insert_post_bind_prf_moves are not called.
// This helps to initialize the Home_Set and Home_Prf
// Also it acts as a low overhead sanity check
void Codegen::update_prf_home(Compound_region *r)
{
    if (dbg(genrio, 1))
        cdbg << "Updating home prf set(fast mode)..." << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;

        for (Op_explicit_inputs srci(op); srci!=0; srci++) {
            Operand& src_oprnd = *srci;
            if (src_oprnd.is_reg()) {
                Hash_set<eString> prf_set(hash_estring, 128);
                prf_set += src_oprnd.physical_file_type();
                if (Vr_Home_Set.is_bound(src_oprnd.vr_num())) {
                    if (Vr_Home_Set.value(src_oprnd.vr_num()) != prf_set)  {
                      cdbg << "for op: " << op->id() << " for src vr: " << src_oprnd.vr_num()
                           << " home_prf_set: " << Vr_Home_Set.value(src_oprnd.vr_num())
                           << " bound phys file: " <<  src_oprnd.physical_file_type() << endl;
                      assert(0);
                    }
		} else 
                    Vr_Home_Set.bind(src_oprnd.vr_num(), prf_set);
                
                if (Vr_Home_Prf.is_bound(src_oprnd.vr_num())) {
                    if (Vr_Home_Prf.value(src_oprnd.vr_num()) != src_oprnd.physical_file_type()) {
                      cdbg << "for op: " << op->id() << " for src vr: " << src_oprnd.vr_num()
                           << " home_prf: " << Vr_Home_Prf.value(src_oprnd.vr_num())
                           << " bound phys file: " <<  src_oprnd.physical_file_type() << endl;
                      assert(0);
                    }
		} else
                    Vr_Home_Prf.bind(src_oprnd.vr_num(), src_oprnd.physical_file_type());
            } else if (src_oprnd.is_macro_reg()) {
                eString prf = src_oprnd.physical_file_type();
                if( ! Macro_Home_Set.value(src_oprnd.name()).is_member(prf)) {
		  cdbg << "ERROR: the physical register file, '" << prf
		       << "' of operand:\n" << src_oprnd << "\nis not in the"
		       << " macro home set. I.e. the mdes doesn't think\n"
		       << "that the macro exists in the physical file it's "
		       << "bound to.\nHere's the op:\n" << *op << endl;
		}
                assert(Macro_Home_Set.value(src_oprnd.name()).is_member(prf));
            }
        }

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
            Operand& dest_oprnd = *desti;
            if (dest_oprnd.is_reg()) {
                Hash_set<eString> prf_set(hash_estring, 128);
                prf_set += dest_oprnd.physical_file_type();
                if (Vr_Home_Set.is_bound(dest_oprnd.vr_num())) {
                    if (Vr_Home_Set.value(dest_oprnd.vr_num()) != prf_set) {
                      cdbg << "for op: " << op->id() << " for dest vr: " << dest_oprnd.vr_num()
                           << " home_prf_set: " << Vr_Home_Set.value(dest_oprnd.vr_num())
                           << " bound phys file: " <<  dest_oprnd.physical_file_type() << endl;
                      assert(0);
                    }
		} else 
                    Vr_Home_Set.bind(dest_oprnd.vr_num(), prf_set);
               
                if (Vr_Home_Prf.is_bound(dest_oprnd.vr_num())) {
                    if (Vr_Home_Prf.value(dest_oprnd.vr_num()) != dest_oprnd.physical_file_type()) {
                      cdbg << "for op: " << op->id() << " for dest vr: " << dest_oprnd.vr_num()
                           << " home_prf: " << Vr_Home_Prf.value(dest_oprnd.vr_num())
                           << " bound phys file: " <<  dest_oprnd.physical_file_type() << endl;
                      assert(0);
                    }
                } else
                    Vr_Home_Prf.bind(dest_oprnd.vr_num(), dest_oprnd.physical_file_type());

            } else if (dest_oprnd.is_macro_reg()) {
                eString prf = dest_oprnd.physical_file_type();
                assert(Macro_Home_Set.value(dest_oprnd.name()).is_member(prf));
            }
        }
    }

}

Op *Codegen::clone_op(Op *op)
{
    Op *new_op  = new Op(op->opcode());
    for (Op_explicit_dests doprndi(op); doprndi!=0; doprndi++) {
      Port_num p = doprndi.get_port_num();
      Operand& opr = *doprndi;
      new_op->set_dest(p, opr);
    }
    for (Op_explicit_inputs soprndi(op); soprndi!=0; soprndi++) {
      Port_num p = soprndi.get_port_num();
      Operand& opr = *soprndi;
      new_op->set_src(p, opr);
    }

    return new_op;
}

// Create new data section to hold the newly created labels
// generated during move_codegen
void Codegen::createDataSection(IR_outstream& outs)
{
    if (operand_to_label_map.is_empty())
      return;

    cdbg << "Creating new data section..." << endl;

    El_datalist *dataSec = new El_datalist;

    El_data *ms = new El_data;
    ms->t_type = EL_DATA_TOKEN_MS ;
    ms->ms_type = EL_DATA_MS_DATA;
    dataSec->elements.add_tail(ms);
    
    for (Hash_map_iterator<Operand, eString> hiter(operand_to_label_map); hiter!=0; hiter++) {
      Operand oprnd = (*hiter).first;
      eString label_name = (*hiter).second;
      El_data *global = new El_data;
      global->t_type = EL_DATA_TOKEN_GLOBAL;
      global->address = new El_symbol_expression((char*)label_name);
      global->src_type = new El_data_source_type_info();
      if (oprnd.is_int())
        global->src_type->flags.set_bit(EL_DATA_ST_INT);
      else if (oprnd.is_float())
        global->src_type->flags.set_bit(EL_DATA_ST_FLOAT);
      else if (oprnd.is_double())
        global->src_type->flags.set_bit(EL_DATA_ST_DOUBLE);
      global->src_type->flags.set_bit(EL_DATA_ST_GLOBAL);
      dataSec->elements.add_tail(global);

      El_data *data = new El_data;
      data->N = 1;
      data->address = new El_symbol_expression((char*)label_name);
      El_value_expression *val = new El_value_expression();
      if (oprnd.is_int()) {
        data->t_type = EL_DATA_TOKEN_LONG;
        val->value.i = oprnd.int_value();
        val->type = EL_EXPRESSION_INT_VALUE;
      } else if (oprnd.is_float()) {
        data->t_type = EL_DATA_TOKEN_FLOAT;
        val->value.f = oprnd.float_value();
        val->type = EL_EXPRESSION_FLOAT_VALUE;
      } else if (oprnd.is_double()) {
        data->t_type = EL_DATA_TOKEN_DOUBLE;
        val->value.f2 = oprnd.double_value();
        val->type = EL_EXPRESSION_DOUBLE_VALUE;
      } else
         assert(0);
      data->value = val;
      dataSec->elements.add_tail(data);
    }
    ir_write(outs, dataSec);
    operand_to_label_map.clear();
}

void Codegen::print_move_stats()
{
    cdbg << "total move codegen ops: " << move_ops << endl;
    cdbg << "two_oprnd_move_ops: " << two_oprnd_move_ops << endl;
    cdbg << "prf move ops: " << prf_move_ops << endl;
    cdbg << "bind prf move ops: " << bind_prf_move_ops << endl;
    cdbg << "total ops: " << move_ops + two_oprnd_move_ops + prf_move_ops + bind_prf_move_ops << endl;
}


