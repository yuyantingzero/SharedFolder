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
//      File:           bind_physical_files.cpp                          //
//      Authors:        Rajiv Ravindran                                  //
//      Created:        April 2002                                       //
//      Description:    Do Physical file bindings from list of           //
//                      valid choices for each operand                   //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "codegen.h"

Operand *Codegen::get_new_operand(Operand& oprnd)
{
    Operand *new_oprnd;
    if (oprnd.is_reg()) {
        Reg *reg = new Reg(oprnd.dtype(), oprnd.vr_num(), oprnd.omega());
        reg->bind_file(oprnd.file_type(), oprnd.is_static() ? STATIC_REG : ROTATING);
        if (oprnd.allocated()) 
            reg->bind_reg(oprnd.mc_num());
        new_oprnd = new Operand(reg);
        return new_oprnd;
    } else if (oprnd.is_macro_reg()) {
        Macro_reg *mreg = new Macro_reg(oprnd.name());
        new_oprnd = new Operand(mreg);
        return new_oprnd;
    } else
        El_punt("get_new_reg_macro: operand has to be reg or macro");

    return 0;
}

// For each VR_num return the preferred prf where that VR_num
// would like to be bound
static void find_preferred_prf(Compound_region *r, Hash_map<int, Pair<eString, int> >& vr_preferred_prf)
{
    Hash_map<int, Hash_set<El_ref> > vr_ref_set(hash_int, 1024);
    Hash_map<Macro_name, Hash_set<El_ref> > macro_ref_set(Codegen::hash_macro_name, 1024);

    if (dbg(genrio, 3))
        cdbg << "Finding preferred prfs in region: " << r->id() << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;

        if (is_pseudo(op) || is_no_op(op))
            continue;

        // create a map of vr_num to <set of refs(of that vr_num)>
        // also for macro names
        for (Op_explicit_inputs srci(op); srci!=0; srci++) {
            Operand& src_oprnd = *srci;
            El_ref ref = srci.get_ref();

            if (src_oprnd.is_reg()) {
                if (vr_ref_set.is_bound(src_oprnd.vr_num())) 
                    (vr_ref_set.value(src_oprnd.vr_num())) += ref;
                else {
                    Hash_set<El_ref> refset(hash_El_ref, 16);
                    refset += ref;
                    vr_ref_set.bind(src_oprnd.vr_num(), refset);
                }
            } else if (src_oprnd.is_macro_reg()) {
                if (macro_ref_set.is_bound(src_oprnd.name())) 
                    (macro_ref_set.value(src_oprnd.name())) += ref;
                else {
                    Hash_set<El_ref> refset(hash_El_ref, 16);
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
                    Hash_set<El_ref> refset(hash_El_ref, 16);
                    refset += ref;
                    vr_ref_set.bind(dest_oprnd.vr_num(), refset);
                }
            } else if (dest_oprnd.is_macro_reg()) {
                if (macro_ref_set.is_bound(dest_oprnd.name())) 
                    (macro_ref_set.value(dest_oprnd.name())) += ref;
                else {
                    Hash_set<El_ref> refset(hash_El_ref, 16);
                    refset += ref;
                    macro_ref_set.bind(dest_oprnd.name(), refset);
                }
            }
        }
    }

    if (dbg(genrio,7)) {
        cdbg << "find_preferred_prf:.." << endl;
        for (Hash_map_iterator<int, Hash_set<El_ref> > hiter11(vr_ref_set); hiter11!=0; hiter11++) {
            Pair<int, Hash_set<El_ref> > vr_num_ref = *hiter11;
            int vr_num = vr_num_ref.first;
            Hash_set<El_ref> ref_set = vr_num_ref.second;
            cdbg << endl << "vr_num: " << vr_num << " Refs: ";
            for (Hash_set_iterator<El_ref> iter(ref_set); iter!=0; iter++)
                cdbg << " " << *iter;
        }
        for (Hash_map_iterator<Macro_name, Hash_set<El_ref> > hiter21(macro_ref_set); hiter21!=0; hiter21++) {
            Pair<Macro_name, Hash_set<El_ref> > macro_name_ref = *hiter21;
            Macro_name mn = macro_name_ref.first;
            Hash_set<El_ref> ref_set = macro_name_ref.second;
            cdbg << endl << "macro_name: " << macro_reg_to_text(mn) << "Refs: ";
            for (Hash_set_iterator<El_ref> iter(ref_set); iter!=0; iter++)
                cdbg << " " << *iter;
        }
    }

    // Either the VR has a home prf (from some previous region) or 
    // it finds the best prf from its references

    // For VRs...
    for (Hash_map_iterator<int, Hash_set<El_ref> > hiter1(vr_ref_set); hiter1!=0; hiter1++) {
        Pair<int, Hash_set<El_ref> > vr_num_ref = *hiter1;
        int vr_num = vr_num_ref.first;
        Hash_set<El_ref> ref_set = vr_num_ref.second;

        Pair<eString, int> home_prf;

        if (!Codegen::Vr_Home_Prf.is_bound(vr_num)) 
            home_prf = Codegen::get_best_ref_prf(ref_set);
        else {
            // set the count to a high value so that this prf is picked
            home_prf.first = Codegen::Vr_Home_Prf.value(vr_num);
            home_prf.second = 1000;
        }

        vr_preferred_prf.bind(vr_num, home_prf);
    }

    if (dbg(genrio,5)) {
        cdbg << endl << "Preferred VRs..." << endl;
        for (Hash_map_iterator<int, Pair<eString, int> > hiter3(vr_preferred_prf); hiter3!=0; hiter3++)
            cdbg << "vr: " << (*hiter3).first << " prf: " 
                 << (*hiter3).second.first << " with count: " << (*hiter3).second.second << endl;
    }
}

#if 0
static void find_def_list(El_ref use, List<El_ref>& def_list)
{
    Operand& src_oprnd = use.get_operand();
    Region *r = use.get_op()->parent();
    if (src_oprnd.is_reg()) {
        for (Region_all_ops opi(r); opi!=0; opi++) {
            Op *op = *opi;
            if (is_pseudo(op))
                continue;
            for (Op_explicit_dests desti(op); desti!=0; desti++) {
                El_ref dest_ref = desti.get_ref();
                Operand& dest_oprnd = dest_ref.get_operand();
                if (dest_oprnd.is_reg() && (dest_oprnd.vr_num() == src_oprnd.vr_num()))
                    def_list.add_tail(dest_ref);
            }
        }
    } else if (src_oprnd.is_macro_reg()) {
        for (Region_all_ops opi(r); opi!=0; opi++) {
            Op *op = *opi;
            if (is_pseudo(op))
                continue;
            for (Op_explicit_dests desti(op); desti!=0; desti++) {
                El_ref dest_ref = desti.get_ref();
                Operand& dest_oprnd = dest_ref.get_operand();
                if (dest_oprnd.is_macro_reg() && (dest_oprnd.name() == src_oprnd.name()))
                    def_list.add_tail(dest_ref);
            }
        }
    }
}

static void find_use_list(El_ref def, List<El_ref>& use_list)
{
    Operand& dest_oprnd = def.get_operand();
    Region *r = def.get_op()->parent();
    if (dest_oprnd.is_reg()) {
        for (Region_all_ops opi(r); opi!=0; opi++) {
            Op *op = *opi;
            if (is_pseudo(op))
                continue;
            for (Op_explicit_inputs srci(op); srci!=0; srci++) {
                El_ref use_ref = srci.get_ref();
                Operand& src_oprnd = use_ref.get_operand();
                if (src_oprnd.is_reg() && (src_oprnd.vr_num() == dest_oprnd.vr_num()))
                    use_list.add_tail(use_ref);
            }
        }
    } else if (dest_oprnd.is_macro_reg()) {
        for (Region_all_ops opi(r); opi!=0; opi++) {
            Op *op = *opi;
            if (is_pseudo(op))
                continue;
            for (Op_explicit_inputs srci(op); srci!=0; srci++) {
                El_ref use_ref = srci.get_ref();
                Operand& src_oprnd = use_ref.get_operand();
                if (src_oprnd.is_macro_reg() && (src_oprnd.name() == dest_oprnd.name()))
                    use_list.add_tail(use_ref);
            }
        }
    }
}
#endif


// If src_oprnd is bound to a prf, look at its ud chain and bind if possible the defines(if they
// are not bound) to the same prf as the source
// return true, if a new binding has been created
static bool bind_ud_src_oprnd(Op_explicit_inputs srci, Reaching_defs_info *rdinfo, PRF_Bind_Map* prf_bind_map)
{
    Operand& src_oprnd = *srci;
    bool new_binding = false;

    if (src_oprnd.is_reg() || src_oprnd.is_macro_reg()) {
      if (src_oprnd.assigned_to_physical_file() || prf_bind_map->is_bound(src_oprnd)) {
        El_ref use = srci.get_ref();
        List<El_ref>* def_list = rdinfo->get_ud_chain(use);
        for (List_iterator<El_ref> liter(*def_list); liter!=0; liter++) {
            El_ref def = *liter;

            if (def.get_ref_type() == LIVEIN_DEST)
                continue;

            if (is_pseudo(def.get_op()))
                continue;

            Operand& def_oprnd = def.get_operand();
            if (!def_oprnd.assigned_to_physical_file() && !prf_bind_map->is_bound(def_oprnd)) {
                Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(def.get_op());
                assert(prf_bind_attr!=0);
                Hash_set<eString>& def_prf_set = prf_bind_attr->dest(def.get_port_num());
                assert(!def_prf_set.is_empty());
                if (src_oprnd.assigned_to_physical_file()) {
                  if (def_prf_set.is_member(src_oprnd.physical_file_type())) {

                    // Operand *new_def_oprnd = Codegen::get_new_operand(def_oprnd);
                    // def.get_op()->set_dest(def.get_port_num(), *new_def_oprnd);
                    prf_bind_map->bind_operand(def_oprnd, src_oprnd.physical_file_type());

                    if (dbg(genrio, 3)) 
                      cdbg << "bind_ud_src_oprnd: Binding dest oprnd at port: " << def.get_port_num() << " with prf: " 
                           << src_oprnd.physical_file_type() << " for op: " << def.get_op()->id() <<  endl;
                    new_binding = true;
                  } else {
                    if (dbg(genrio, 3)) 
                        cdbg << "bind_ud_src_oprnd: src port: " << srci.get_port_num() << " bound to: " 
                             << src_oprnd.physical_file_type() << "which is not supported by def op: " << def.get_op()->id() 
                             << "at port: " << def.get_port_num() << endl;
                  } 
                } else if (prf_bind_map->is_bound(src_oprnd)) {
                  if (def_prf_set.is_member(prf_bind_map->get_prf(src_oprnd))) {

                    //Operand *new_def_oprnd = Codegen::get_new_operand(def_oprnd);
                    //def.get_op()->set_dest(def.get_port_num(), *new_def_oprnd);
                    prf_bind_map->bind_operand(def_oprnd, prf_bind_map->get_prf(src_oprnd));

                    if (dbg(genrio, 3)) 
                      cdbg << "bind_ud_src_oprnd: Binding dest oprnd at port: " << def.get_port_num() << " with prf: " 
                           << prf_bind_map->get_prf(src_oprnd) << " for op: " << def.get_op()->id() <<  endl;
                    new_binding = true;
                  } else {
                    if (dbg(genrio, 3)) 
                        cdbg << "bind_ud_src_oprnd: src port: " << srci.get_port_num() << " bound to: " 
                             << prf_bind_map->get_prf(src_oprnd) << "which is not supported by def op: " << def.get_op()->id() 
                             << "at port: " << def.get_port_num() << endl;
                  } 
                }
            }
        }
      }
    }

    return new_binding;
}

// If dest_oprnd is bound to a prf, look at its du chain and bind if possible the uses(if they
// are not bound) to the same prf as the dest
// return true, if a new binding has been created
static bool bind_du_dest_oprnd(Op_explicit_dests desti, Reaching_defs_info *rdinfo, PRF_Bind_Map* prf_bind_map)
{
    bool new_binding = false;
    Operand& dest_oprnd = *desti;

    if (dest_oprnd.is_macro_reg() || dest_oprnd.is_reg()) {
      if (dest_oprnd.assigned_to_physical_file() || prf_bind_map->is_bound(dest_oprnd)) {
        El_ref def = desti.get_ref();
        List<El_ref>* use_list = rdinfo->get_du_chain(def);
        for (List_iterator<El_ref> liter(*use_list); liter!=0; liter++) {
            El_ref use = *liter;

            if ((use.get_ref_type() == LIVEOUT_TAKEN_SRC) || 
                (use.get_ref_type() == LIVEOUT_FALLTHROUGH_SRC))
                continue;

            if (is_pseudo(use.get_op()))
                continue;

            Operand& use_oprnd = use.get_operand();
            if (!use_oprnd.assigned_to_physical_file() && !prf_bind_map->is_bound(use_oprnd)) {
                Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(use.get_op());
                assert(prf_bind_attr!=0);
                Hash_set<eString>& use_prf_set = prf_bind_attr->src(use.get_port_num());
                assert(!use_prf_set.is_empty());

                if (dest_oprnd.assigned_to_physical_file()) {
                  if (use_prf_set.is_member(dest_oprnd.physical_file_type())) {

                    //Operand *new_use_oprnd = Codegen::get_new_operand(use_oprnd);
                    //use.get_op()->set_src(use.get_port_num(), *new_use_oprnd);
                    prf_bind_map->bind_operand(use_oprnd, dest_oprnd.physical_file_type());

                    if (dbg(genrio, 3))
                        cdbg << "bind_du_dest_oprnd: Binding src oprnd at port: " << use.get_port_num() << " with prf: " 
                             << dest_oprnd.physical_file_type() << " for op: " << use.get_op()->id() <<  endl;
                     new_binding = true;
                  } else {
                    if (dbg(genrio, 3))
                        cdbg << "bind_du_dest_oprnd: dest port: " << desti.get_port_num() << " bound to: " 
                             << dest_oprnd.physical_file_type() << "which is not supported by use op: " << use.get_op()->id() 
                             << "at port: " << use.get_port_num() << endl;
                  }
                } else if (prf_bind_map->is_bound(dest_oprnd)) {
                  if (use_prf_set.is_member(prf_bind_map->get_prf(dest_oprnd))) {

                    //Operand *new_use_oprnd = Codegen::get_new_operand(use_oprnd);
                    //use.get_op()->set_src(use.get_port_num(), *new_use_oprnd);
                    prf_bind_map->bind_operand(use_oprnd, prf_bind_map->get_prf(dest_oprnd));

                    if (dbg(genrio, 3))
                        cdbg << "bind_du_dest_oprnd: Binding src oprnd at port: " << use.get_port_num() << " with prf: " 
                             << prf_bind_map->get_prf(dest_oprnd) << " for op: " << use.get_op()->id() <<  endl;
                     new_binding = true;
                  } else {
                    if (dbg(genrio, 3))
                        cdbg << "bind_du_dest_oprnd: dest port: " << desti.get_port_num() << " bound to: " 
                             << prf_bind_map->get_prf(dest_oprnd) << "which is not supported by use op: " << use.get_op()->id() 
                             << "at port: " << use.get_port_num() << endl;
                  }
                }
            }
        }
      }
    }

    return new_binding;
}

static int get_literal_width(eString prf)
{
    for (Slist_iterator<Stuple<int, char *> > sliter(Codegen::Litfile_List); sliter!=0; sliter++) {
        Stuple<int, char *> litfile_elem = *sliter;
        if (!strcmp(litfile_elem.second, prf))
            return litfile_elem.first;
    }

    El_punt("get_literal_width: literal file %s not found", (char*)prf);
    return 0;
}
    
static void bind_literal_file(Operand& src_oprnd, Hash_set<eString>& src_physfile_set)
{
    eString prf;
    if (src_oprnd.is_cb()) {
        int max_width = Codegen::Litfile_List.tail().first;
        int best_width=0;
        for (Hash_set_iterator<eString> hiter(src_physfile_set); hiter!=0; hiter++) {
            int width = get_literal_width(*hiter);
            if (width <= max_width && (width > best_width)) {
                best_width = width;
                prf = *hiter;
            }
        }
        assert(best_width != 0);
    } else {
        int min_width = 99999;
        for (Hash_set_iterator<eString> hiter(src_physfile_set); hiter!=0; hiter++) {
            int width = get_literal_width(*hiter);
            if (width < min_width) {
                min_width = width;
                prf = *hiter;
            }
        }
    }

    if (dbg(genrio,3))
        cdbg << endl << "bind_literal_file: src_oprnd: " << src_oprnd 
             << " being bound to: " << prf << endl;

    src_oprnd.bind_physical_file(prf);
}

// Evaluate the cost of binding op to op_descr opdes.
// cost is the function of number of overlapping prf at each port
// if the prf set at a port for opdes contains the preferred prf
// increase that opdes weight
int Codegen::get_opdes_cost(Op *op, Op_descr *opdes, Hash_map<int, Pair<eString, int> >& vr_preferred_prf)
                              
{
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    assert(prf_battr != 0);

    int total_cost = 0, port_cost = 0;
    for (Op_explicit_inputs srci(op); srci!=0; srci++) {
        Operand& src_oprnd = *srci;
        Port_num port = srci.get_port_num();

        if (src_oprnd.is_undefined() || src_oprnd.is_predicate())
            continue;

        Hash_set<eString> prf_set = prf_battr->src(port);
        assert(!prf_set.is_empty());
        if (dbg(genrio,7)) {
            cdbg << endl << "op: " << op->id() << " src port: " << port;
            for (Hash_set_iterator<eString> hiter(prf_set); hiter!=0; hiter++) 
                cdbg << " " << *hiter;
        }

        List_set<Reg_descr*> src_regdescr_set;
        MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_IN, port, src_regdescr_set);
        Hash_set<eString> src_prf_set(hash_estring, CGEN_HASH_SIZE_1);
        if (dbg(genrio,7)) 
            cdbg << endl <<  "opdes: " << opdes->get_aname_ptr() << " src port: " << port; 
        for (List_set_iterator<Reg_descr*> liter(src_regdescr_set); liter!=0; liter++) {
            src_prf_set += (*liter)->get_aname();
            if (dbg(genrio,7))
                cdbg << " " << (*liter)->get_aname();
        }

        prf_set *= src_prf_set;
        if (prf_set.is_empty())
            port_cost = -99999;
        else 
            port_cost = prf_set.size();
        
        eString preferred_prf;
        int count;
        if (src_oprnd.is_reg()) {
            assert(vr_preferred_prf.is_bound(src_oprnd.vr_num()));
            preferred_prf = vr_preferred_prf.value(src_oprnd.vr_num()).first;
            count = vr_preferred_prf.value(src_oprnd.vr_num()).second;
            if (prf_set.is_member(preferred_prf)) {
                port_cost += count;
                if (dbg(genrio,7))
                    cdbg << endl << "preferred_prf: " << preferred_prf 
                         << " found in intersect set " 
                         << "of src port: " << port << " in op: " << op->id();
            }
        } else if (src_oprnd.is_macro_reg()) {
            assert(Codegen::Macro_Home_Set.is_bound(src_oprnd.name())); 
            Hash_set<eString>& home_prfs = Codegen::Macro_Home_Set.value(src_oprnd.name());
            if (home_prfs.is_subset(prf_set)) {
                port_cost += 1000;
                if (dbg(genrio,7))
                    cdbg << endl << "preferred_prf: " << home_prfs
                         << " found in intersect set " 
                         << "of src port: " << port << " in op: " << op->id();
            }
        }
        
        total_cost += port_cost;
    }

    for (Op_explicit_dests desti(op); desti!=0; desti++) {
        Operand& dest_oprnd = *desti;
        Port_num port = desti.get_port_num();

        if (dest_oprnd.is_undefined())
            continue;

        Hash_set<eString> prf_set = prf_battr->dest(port);
        assert(!prf_set.is_empty());
        if (dbg(genrio,7)) {
            cdbg << endl << "op: " << op->id() << " dest port: " << port;
            for (Hash_set_iterator<eString> hiter(prf_set); hiter!=0; hiter++) 
                cdbg << " " << *hiter;
        }

        List_set<Reg_descr*> dest_regdescr_set;
        MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_OUT, port, dest_regdescr_set);
        Hash_set<eString> dest_prf_set(hash_estring, CGEN_HASH_SIZE_1);
        if (dbg(genrio,7)) 
            cdbg << endl <<  "opdes: " << opdes->get_aname_ptr() << " dest port: " << port; 
        for (List_set_iterator<Reg_descr*> liter(dest_regdescr_set); liter!=0; liter++) {
            dest_prf_set += (*liter)->get_aname();
            if (dbg(genrio,7))
                cdbg << " " << (*liter)->get_aname();   
        }

        prf_set *= dest_prf_set;
        if (prf_set.is_empty())
            port_cost = -99999;
        else 
            port_cost = prf_set.size();

        eString preferred_prf;
        int count;
        if (dest_oprnd.is_reg()) {
            assert(vr_preferred_prf.is_bound(dest_oprnd.vr_num()));
            preferred_prf = vr_preferred_prf.value(dest_oprnd.vr_num()).first;
            count = vr_preferred_prf.value(dest_oprnd.vr_num()).second;
            if (prf_set.is_member(preferred_prf)) {
                port_cost += count;
                if (dbg(genrio,7))
                    cdbg << endl << "preferred_prf: " << preferred_prf 
                         << " found in intersect set " 
                         << "of dest port: " << port << " in op: " << op->id();
            }
        } else if (dest_oprnd.is_macro_reg()) {
            assert(Codegen::Macro_Home_Set.is_bound(dest_oprnd.name())); 
            Hash_set<eString>& home_prfs = Codegen::Macro_Home_Set.value(dest_oprnd.name());
            if (home_prfs.is_subset(prf_set)) {
                port_cost += 1000;
                if (dbg(genrio,7))
                    cdbg << endl << "preferred_prf: " << home_prfs 
                         << " found in intersect set " 
                         << "of dest port: " << port << " in op: " << op->id();
            }
        }
        
        total_cost += port_cost;
    }

    if (dbg(genrio,3)) 
        cdbg << endl << "bind_physical_files: Trying op: " << op->id() << " on opdes: " << opdes->get_aname_ptr() 
             << " cost: " << total_cost << endl;
       
    return total_cost;
}

static void do_prf_prune_using_opdes(Op *op, Op_descr* opdes)
{
    Physfile_binding_attr *prf_battr = get_prf_binding_attr(op);
    assert(prf_battr != 0);

    for (Op_explicit_inputs srci(op); srci!=0; srci++) {
        Operand& src_oprnd = *srci;
        Port_num port = srci.get_port_num();

        if (src_oprnd.is_undefined() || src_oprnd.is_predicate())
            continue;

        Hash_set<eString>& prf_set = prf_battr->src(port);
        if (prf_set.is_empty()) {
	  cdbg << "ERROR: Can't find a scheduling alternative to match op description of op:\n"
	       << *op << "\nThis is either because of a buggy MDES, or you inserted an op that\n"
	       << "doesn't exist (e.g. most machines don't have opcodes that can read inputs\n"
	       << "from physical files in different clusters, or floating point loads can't\n"
	       << "write to integer register files)" << endl;
          assert(0);
        }

        List_set<Reg_descr*> src_regdescr_set;
        MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_IN, port, src_regdescr_set);
        Hash_set<eString> src_prf_set(hash_estring, CGEN_HASH_SIZE_1);
        for (List_set_iterator<Reg_descr*> liter(src_regdescr_set); liter!=0; liter++) 
            src_prf_set += (*liter)->get_aname();

        if (dbg(genrio,5)) {
            cdbg << endl << "bind_physical_files: prf prune phase: op: " << op->id() 
                 << " src port: " << port << " old prf: " ;
            for (Hash_set_iterator<eString> hiter(prf_set); hiter!=0; hiter++)
                cdbg << " " << *hiter;
        }

        prf_set *= src_prf_set;
        // Normally, the alternative 'opdes' should satisfy the op's prf choices,
        // as get_best_opdes which is called prior to this function gets the best matching
        // alternative. But in fast mode, we assume that all the alternatives are ok, and 
        // so get_best_opdes just returns the first alternative. In case the MDES is not set
        // correctly, pruning the op's io format using this alternative might cause the
        // prf_set to be empty.
        if (prf_set.is_empty()) {
	  cdbg << "ERROR: Can't find a scheduling alternative to match op description of op:\n"
	       << *op << "\nThis is either because of a buggy MDES, or you inserted an op that\n"
	       << "doesn't exist (e.g. most machines don't have opcodes that can read inputs\n"
	       << "from physical files in different clusters, or floating point loads can't\n"
	       << "write to integer register files)" << endl;
          assert(0);
        }

        if (dbg(genrio,5)) {
            cdbg << " new prf set: ";
            for (Hash_set_iterator<eString> hiter1(prf_set); hiter1!=0; hiter1++)
                cdbg << " " << *hiter1;
        }
    }

    for (Op_explicit_dests desti(op); desti!=0; desti++) {
        Operand& dest_oprnd = *desti;
        Port_num port = desti.get_port_num();

        if (dest_oprnd.is_undefined())
            continue;

        Hash_set<eString>& prf_set = prf_battr->dest(port);
	if(prf_set.is_empty()) {
	  cdbg << "ERROR: Can't find a scheduling alternative to match op description of op:\n"
	       << *op << "\nThis is either because of a buggy MDES, or you inserted an op that\n"
	       << "doesn't exist (e.g. most machines don't have opcodes that can read inputs\n"
	       << "from physical files in different clusters, or floating point loads that can\n"
	       << "write to integer register files)" << endl;
	  assert(0);
	}

        List_set<Reg_descr*> dest_regdescr_set;
        MDES_collect_reg_descrs(opdes->get_aname_ptr(), DATA_OUT, port, dest_regdescr_set);
        Hash_set<eString> dest_prf_set(hash_estring, CGEN_HASH_SIZE_1);
        for (List_set_iterator<Reg_descr*> liter(dest_regdescr_set); liter!=0; liter++) 
            dest_prf_set += (*liter)->get_aname();

        prf_set *= dest_prf_set;
        // Normally, the alternative 'opdes' should satisfy the op's prf choices,
        // as get_best_opdes which is called prior to this function gets the best matching
        // alternative. But in fast mode, we assume that all the alternatives are ok, and 
        // so get_best_opdes just returns the first alternative. In case the MDES is not set
        // correctly, pruning the op's io format using this alternative might cause the
        // prf_set to be empty.
        if (prf_set.is_empty()) {
	  cdbg << "ERROR: Can't find a scheduling alternative to match op description of op:\n"
	       << *op << "\nThis is either because of a buggy MDES, or you inserted an op that\n"
	       << "doesn't exist (e.g. most machines don't have opcodes that can read inputs\n"
	       << "from physical files in different clusters, or floating point loads that can\n"
	       << "write to integer register files)" << endl;
          assert(0);
        }

        prf_battr->set_dest(port, prf_set);
        if (dbg(genrio,5)) {
            cdbg << endl << "bind_physical_files: prf prune phase: op: " << op->id() 
                 << " dest port: " << port << " old prf: " ;
            cdbg << " new prf set: ";
            for (Hash_set_iterator<eString> hiter1(prf_set); hiter1!=0; hiter1++)
                cdbg << " " << *hiter1;
        }
    }
}

// check whether all operands of op are bound
bool Codegen::all_oprnd_bound(Op *op)
{
    for (Op_explicit_operands oprndi(op); oprndi!=0; oprndi++) {
        Operand& oprnd = *oprndi;
        if (oprnd.is_undefined() || oprnd.is_predicate())
            continue;
        if (!oprnd.assigned_to_physical_file()) 
            return false;
    }
    return true;
}

// Among all the unit specific ops that map to a particular op, select the best unit specfic op
// and prune the ops prf choices based on that unit specific opcodes MDES prf choices for each 
// of its sources and dests.
static void prune_op_prf_based_on_opdes(Compound_region *r, Hash_map<int, Pair<eString, int> >& vr_preferred_prf,
                                        Cluster_mgr& cmgr)
{
    if (dbg(genrio,3)) 
        cdbg << "bind_physical_files: prune_op_prf_based_on_opdes" << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;

        // If all operands of op are bound, continue
        if (Codegen::all_oprnd_bound(op))
            continue;

        int cid;
        List_set<Op_descr*> clust_uspc_ops;

        if (op->flag(EL_OPER_ICMOVE)) {
            Hash_set<eString>& uspc_icmoves = cmgr.get_ic_move_opcodes();

            List_set<Op_descr*> uspc_op_descrs;
            MDES_collect_op_descrs(el_opcode_to_string_map.value(op->opcode()), uspc_op_descrs);

            for (List_set_iterator<Op_descr*> liter(uspc_op_descrs); liter!=0; liter++) {
                Op_descr *op_descr = *liter;
                if (uspc_icmoves.is_member(op_descr->get_aname_ptr()))
                    clust_uspc_ops += op_descr;
            }
        } else {
	  if(!Cluster_mgr::op_map.is_bound(op)) {
	    cdbg << "Operation not assigned to a cluster!" << endl
		 << *op << endl;
	    cdbg << "It's likely that you either didn't run clustering"
		 << ", or you added some ops since clustering was performed."
		 << endl;
	  }
	  assert(Cluster_mgr::op_map.is_bound(op));
	  cid = Cluster_mgr::op_map.value(op);
	  Codegen::get_uspc_ops_in_cluster(op, cid, clust_uspc_ops);
        }

        assert(!clust_uspc_ops.is_empty());

        Op_descr *best_opdes = 0;
        // In the fast mode, we assume that all the cluster unit specific opcodes have
        // the same IOformat, so there is no really any need to pick the best opdes,
        // Hence return the first alternative. For an ICMOVE, this is not true, so 
        // use opdes_cost to evaluate all alternatives
        if (El_codegen_fast_mode) {
          if (op->flag(EL_OPER_ICMOVE)) {
            int max_cost = 0, cost;
            for (List_set_iterator<Op_descr*> iter(clust_uspc_ops); iter!=0; iter++) {
              Op_descr *opdes = *iter;
              cost = Codegen::get_opdes_cost(op, opdes, vr_preferred_prf);
              if (cost > max_cost) {
                max_cost = cost;
                best_opdes = opdes;
              }
            }
         } else
            best_opdes = clust_uspc_ops.head();
        } else {
          // Find the unit specific opcode with max cost
          // we ignore any opdes with negative cost
          int max_cost = 0, cost;
          for (List_set_iterator<Op_descr*> iter(clust_uspc_ops); iter!=0; iter++) {
            Op_descr *opdes = *iter;
            cost = Codegen::get_opdes_cost(op, opdes, vr_preferred_prf);
            if (cost > max_cost) {
                max_cost = cost;
                best_opdes = opdes;
            }
          }
        }

        if (best_opdes == 0) {
	  cdbg << "ERROR: Can't find a scheduling alternative to match op description of op:\n"
	       << *op << "\nThis is either because of a buggy MDES, or you inserted an op that\n"
	       << "doesn't exist (e.g. most machines don't have opcodes that can read inputs\n"
	       << "from multiple clusters).\n";
	  // assert(is_move(op) && !is_pbr(op));
	  El_punt("bind_physical_files: couldn't find an opdes to bind prfs of move op: %d", op->id());
        }

        if (dbg(genrio,5))
            cdbg << "bind_physical_files: Using best opdescr: " << best_opdes->get_aname_ptr()
                 << " for op: " << op->id() << endl;

        // and use the best_opdes to prune the ops choices
        do_prf_prune_using_opdes(op, best_opdes);
    }
}

static void bind_preferred_prf(Compound_region *r, Hash_map<int, Pair<eString, int> >& vr_preferred_prf,
                               PRF_Bind_Map* prf_bind_map)
{
    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;

        if (is_pseudo(op) || is_no_op(op))
            continue;

        Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(op);
        assert(prf_bind_attr != 0);

        for (Op_explicit_inputs srci(op); srci!=0; srci++) {
            Operand& src_oprnd = (*srci);
            if (src_oprnd.is_predicate() || src_oprnd.is_undefined())
                continue;

	    // if(src_oprnd.is_undefined())
	    //   El_punt("Cannot bind physical file for op %d: %s",
	    //	         op->id(), "There's an undefined operand!\n");
	    
            Hash_set<eString>& src_physfile_set = prf_bind_attr->src(srci.get_port_num());
            assert(!src_physfile_set.is_empty());

            if (!src_oprnd.assigned_to_physical_file() && src_oprnd.is_lit() && !src_oprnd.is_predicate()) {
                bind_literal_file(src_oprnd, src_physfile_set);
            } else if (!src_oprnd.assigned_to_physical_file() && (src_oprnd.is_reg() || src_oprnd.is_macro_reg())) {

                if (src_oprnd.is_reg()) {
                    assert(vr_preferred_prf.is_bound(src_oprnd.vr_num()));
                    eString preferred_prf = vr_preferred_prf.value(src_oprnd.vr_num()).first;
                    if (src_physfile_set.is_member(preferred_prf)) {
                      if (dbg(genrio,3))
                        cdbg << "Binding src oprnd at port: " << srci.get_port_num() << " of op: " 
                             << (srci.get_op())->id() << " being bound to preferred prf: " 
                             << preferred_prf << endl;

                      //Operand *new_src_oprnd = Codegen::get_new_operand(src_oprnd);
                      //op->set_src(srci.get_port_num(), *new_src_oprnd);
                      prf_bind_map->bind_operand(src_oprnd, preferred_prf);
                    }
                } else if (src_oprnd.is_macro_reg()) {
                    assert(Codegen::Macro_Home_Set.is_bound(src_oprnd.name()));
                    Hash_set<eString>& home_prfs = Codegen::Macro_Home_Set.value(src_oprnd.name());
                    // Since we are not doing any preferred file selection for macros
                    // we expect that clustering has narrowed the macros choices to a singleton
                    assert (src_physfile_set.size() == 1);
                    if (home_prfs.is_subset(src_physfile_set)) {
                      eString bind_prf = src_physfile_set.head();
                      if (dbg(genrio,3))
                        cdbg << "Binding src oprnd at port: " << srci.get_port_num() << " of op: " 
                             << (srci.get_op())->id() << " being bound to prf: " 
                             << bind_prf << endl;

                      //Operand *new_src_oprnd = Codegen::get_new_operand(src_oprnd);
                      //op->set_src(srci.get_port_num(), *new_src_oprnd);
                      prf_bind_map->bind_operand(src_oprnd, bind_prf);
                    }
                }
            }
        }

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
            Operand& dest_oprnd = (*desti);
            if (!dest_oprnd.assigned_to_physical_file() && (dest_oprnd.is_reg() || dest_oprnd.is_macro_reg())) {
                Hash_set<eString>& dest_physfile_set = prf_bind_attr->dest(desti.get_port_num());
                assert(!dest_physfile_set.is_empty());
                if (dest_oprnd.is_reg()) {
                    assert(vr_preferred_prf.is_bound(dest_oprnd.vr_num()));
                    eString preferred_prf = vr_preferred_prf.value(dest_oprnd.vr_num()).first;
                    if (dest_physfile_set.is_member(preferred_prf)) {
                      if (dbg(genrio,3))
                        cdbg << "Binding dest oprnd at port: " << desti.get_port_num() << " of op: " 
                             << (desti.get_op())->id() << " being bound to preferred prf: " 
                             << preferred_prf << endl;

                      //Operand *new_dest_oprnd = Codegen::get_new_operand(dest_oprnd);
                      //op->set_dest(desti.get_port_num(), *new_dest_oprnd);
                      prf_bind_map->bind_operand(dest_oprnd, preferred_prf);
                    }
                } else if (dest_oprnd.is_macro_reg()) {
                    assert(Codegen::Macro_Home_Set.is_bound(dest_oprnd.name()));
                    Hash_set<eString>& home_prfs = Codegen::Macro_Home_Set.value(dest_oprnd.name());
                    // Since we are not doing any preferred file selection for macros
                    // we expect that clustering has narrowed the macros choices to a singleton
                    assert (dest_physfile_set.size() == 1);
                    if (home_prfs.is_subset(dest_physfile_set)) {
                      eString bind_prf = dest_physfile_set.head();
                      if (dbg(genrio,3))
                        cdbg << "Binding dest oprnd at port: " << desti.get_port_num() << " of op: " 
                             << (desti.get_op())->id() << " being bound to prf: " 
                             << bind_prf << endl;

                      //Operand *new_dest_oprnd = Codegen::get_new_operand(dest_oprnd);
                      //op->set_dest(desti.get_port_num(), *new_dest_oprnd);
                      prf_bind_map->bind_operand(dest_oprnd, bind_prf);
                    }
                }
            }
        }
    }
}

// Top level routine to bind physical file for region 'r'
void Codegen::bind_physical_files(Compound_region *r, Cluster_mgr& cmgr)
{
    if (dbg(genrio, 1))
        cdbg << "Binding Physical Files for region: " << r->id() 
             << " in proc: " << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;

    Hash_map<int, Pair<eString, int> > vr_preferred_prf(hash_int, 1024);
    find_preferred_prf(r, vr_preferred_prf);

    prune_op_prf_based_on_opdes(r, vr_preferred_prf, cmgr);
    if (dbg(genrio, 10)) {
      char buff[256];
      sprintf(buff, "%s_%d_prune_op_prf_based_on_opdes.reb", 
                    (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
      cdbg << "Printing Rebel prune op based on opdes on region: " << r->id()
           << " in proc: " << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;
      IR_outstream dbg_out(buff);
      ir_write(dbg_out, el_get_enclosing_procedure(r)); 
    }

    // Now, bind the preferred prf
    if (dbg(genrio,3))
      cdbg << "binding_preferred_prf: for reg: " << r->id() << "in proc: " 
           << (char*)(el_get_enclosing_procedure(r)->get_name()) << endl;
    bind_preferred_prf(r, vr_preferred_prf, &prf_bind_map);
    
    // For operands, that could not be bound from above...
    if (dbg(genrio,3))
        cdbg << "bind_physical_files phase 2: for reg: " << r->id() << "in proc: " 
             << (char*)(el_get_enclosing_procedure(r)->get_name()) << endl;
    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;

        if (is_pseudo(op) || is_no_op(op))
            continue;

        Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(op);
        assert(prf_bind_attr != 0);

        for (Op_explicit_inputs srci(op); srci!=0; srci++) {
            Operand& src_oprnd = (*srci);
            if (src_oprnd.is_predicate() || src_oprnd.is_undefined())
                continue;
            Hash_set<eString>& src_physfile_set = prf_bind_attr->src(srci.get_port_num());
            assert(!src_physfile_set.is_empty());

            if (!src_oprnd.assigned_to_physical_file() && src_oprnd.is_lit() && !src_oprnd.is_predicate()) {
                bind_literal_file(src_oprnd, src_physfile_set);
            } else if (src_oprnd.is_reg() || src_oprnd.is_macro_reg()) {
                if (!src_oprnd.assigned_to_physical_file() 
		        && !prf_bind_map.is_bound(src_oprnd)) {
                  if (src_physfile_set.size() > 1) {
                    if (dbg(genrio, 1))
                      cdbg << "op: " << op->id() << " has multiple PRF choices for src oprnd at port: " 
                           << srci.get_port_num() << endl;
                  } else {
                    if (dbg(genrio, 3))
                      cdbg << endl << "Binding src oprnd at port: " << srci.get_port_num() << " with prf: " 
                           << src_physfile_set.head() << " for op: " << op->id() <<  endl;
                    
                    //Operand *new_src_oprnd = Codegen::get_new_operand(src_oprnd);
                    //op->set_src(srci.get_port_num(), *new_src_oprnd);
                    prf_bind_map.bind_operand(src_oprnd, src_physfile_set.head());
                  }		    
                }
            }
        }

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
          Operand& dest_oprnd = (*desti);
          if (dest_oprnd.is_reg() || dest_oprnd.is_macro_reg()) {
            if (!dest_oprnd.assigned_to_physical_file() && !prf_bind_map.is_bound(dest_oprnd)) {
              Hash_set<eString>& dest_physfile_set = prf_bind_attr->dest(desti.get_port_num());
              assert(!dest_physfile_set.is_empty());
              if (dest_physfile_set.size() > 1) {
                if (dbg(genrio, 1))
                  cdbg << "op: " << op->id() << "has multiple PRF choices for dest oprnd at port" 
                       << desti.get_port_num() << endl;
              } else {
                if (dbg(genrio, 3))
                  cdbg << "Binding dest oprnd at port: " << desti.get_port_num() << " with prf: " 
                       << dest_physfile_set.head() << " for op: " << op->id() <<  endl;

                //Operand *new_dest_oprnd = Codegen::get_new_operand(dest_oprnd);
                //op->set_dest(desti.get_port_num(), *new_dest_oprnd);
                prf_bind_map.bind_operand(dest_oprnd, dest_physfile_set.head());
              }
            }
          }
        }
    }

    if (!El_codegen_fast_mode) {

      if (dbg(genrio,3))
        cdbg << "bind_physical_files phase 3: for reg: " << r->id() << "in proc: " 
             << (char*)(el_get_enclosing_procedure(r)->get_name()) << endl;

      create_local_analysis_info_for_all_hbs_bbs(r);

      El_do_reaching_defs (r, ANALYZE_ALLREG) ;
      Reaching_defs_info* rdinfo = get_reaching_defs_info (r);

      bool new_binding = true;
      while (new_binding == true) {
        new_binding = false;

        for (Region_all_ops opi1(r); opi1!=0; opi1++) {
            Op *op = *opi1;

            if (is_pseudo(op) || is_no_op(op)) 
                continue;

            Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(op);
            assert(prf_bind_attr != 0);

            bool new_binding_ud = false, new_binding_du = false;
            for (Op_explicit_inputs srci(op); srci!=0; srci++)  
                new_binding_ud = bind_ud_src_oprnd(srci, rdinfo, &prf_bind_map);

            for (Op_explicit_dests desti(op); desti!=0; desti++) 
                new_binding_du = bind_du_dest_oprnd(desti, rdinfo, &prf_bind_map);

            if (new_binding == false)
                new_binding = new_binding_ud | new_binding_du;
        }

        if (dbg(genrio, 3))
          if (new_binding == true)
            cdbg << "generated new bindings...retrying region..." << endl;
         
      }

      delete_local_analysis_info_for_all_hbs_bbs(r);
      remove_reaching_defs_info (r) ;
    }


#if 0
    // Now do a random binding of prfs for remaining unbound operands 
    // that could not be binded from above
    for (Region_all_ops opi1(r); opi1!=0; opi1++) {
        Op *op = *opi1;

        if (is_pseudo(op))
            continue;

        Physfile_binding_attr *prf_bind_attr = get_prf_binding_attr(op);
        assert(prf_bind_attr != 0);

        for (Op_explicit_inputs srci1(op); srci1!=0; srci1++) {
            Operand& src_oprnd = (*srci1);
            if (src_oprnd.is_macro_reg() || src_oprnd.is_reg()) {
              if (!src_oprnd.assigned_to_physical_file() && !prf_bind_map.is_bound(src_oprnd)) {
                Hash_set<eString> src_prf_set = prf_bind_attr->src(srci1.get_port_num());
                // at this point, we would have multiple phys files choices..
                // pick the first phys file and do the binding..

                // Operand *new_src_oprnd = Codegen::get_new_operand(src_oprnd);
                // op->set_src(srci1.get_port_num(), *new_src_oprnd);
                prf_bind_map.bind_operand(src_oprnd, src_prf_set.head());
                if (dbg(genrio, 3)) 
                    cdbg << "random binding src port: " << srci1.get_port_num() << " of op: " << op->id() 
                         << " with prf: " << prf_bind_map.get_prf(src_oprnd) << endl;
              }
            }
        }

        for (Op_explicit_dests desti1(op); desti1!=0; desti1++) {
            Operand& dest_oprnd = (*desti1);
            if (dest_oprnd.is_macro_reg() || dest_oprnd.is_reg()) {
              if (!dest_oprnd.assigned_to_physical_file() && !prf_bind_map.is_bound(dest_oprnd)) {
                Hash_set<eString> dest_prf_set = prf_bind_attr->dest(desti1.get_port_num());
                // at this point, we would have multiple phys files choices..
                // pick the first phys file and do the binding..

                // Operand *new_dest_oprnd = Codegen::get_new_operand(dest_oprnd);
                // op->set_dest(desti1.get_port_num(), *new_dest_oprnd);
                prf_bind_map.bind_operand(dest_oprnd, dest_prf_set.head());
                if (dbg(genrio, 3)) 
                    cdbg << "random binding dest port: " << desti1.get_port_num() << " of op: " << op->id() 
                         << " with prf: " << prf_bind_map.get_prf(dest_oprnd) << endl;
              }
            }
        }
    }
#endif

    if (dbg(genrio,7))
        prf_bind_map.print_map();

     if (!El_codegen_fast_mode)
       insert_post_bind_prf_moves(r);
}

