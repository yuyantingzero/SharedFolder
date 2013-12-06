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



#include "regfile_bind.h"
#include "hash_functions.h"
#include "el_codegen_init.h"
#include "cluster.h"
#include "codegen.h"
#include "intf.h"
#include "el_driver_init.h"

PRF_Bind_Map::PRF_Bind_Map()
     :oprnd_bind_map(hash_operand_ptr, 1024)
{
}

PRF_Bind_Map::~PRF_Bind_Map()
{
}

void PRF_Bind_Map::clear_prf_map()
{  
  oprnd_bind_map.clear();
}

void PRF_Bind_Map::bind_operand(Operand& oprnd, eString prf)
{
  oprnd_bind_map.bind(&oprnd, prf);
}

bool PRF_Bind_Map::is_bound(Operand& oprnd)
{
  return oprnd_bind_map.is_bound(&oprnd);
}

eString PRF_Bind_Map::get_prf(Operand& oprnd)
{
  if (!oprnd_bind_map.is_bound(&oprnd))
    El_punt("Physical_RegisterFile_Bind: get_prf: unbound oprnd");
  else
    return oprnd_bind_map.value(&oprnd);

    return 0;
}

void PRF_Bind_Map::do_prf_bind(Compound_region *r, Cluster_mgr& cmgr)
{
   if (dbg(genrio,3))
       cdbg << "Binding physical files in region: " << r->id() << " in proc: "
            << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;
        // if all operands bound, ignore ...
        if (Codegen::all_oprnd_bound(op))
            continue;
        for (Op_explicit_operands oprndi(op); oprndi!=0; oprndi++) {
            Operand& oprnd = *oprndi;

            if (oprnd.is_undefined() || oprnd.is_predicate())
                continue;

            if (oprnd.assigned_to_physical_file())
                continue;

            if (!oprnd_bind_map.is_bound(&oprnd)) {
                cdbg << "do_prf_bind: oprnd: " << oprnd << endl;
                El_punt("do_prf_bind: oprnd at port %d not bound in op: %d\n", oprndi.get_port_num(), op->id());
            }
	      
            if (dbg(status,10))
                cdbg << "do_prf_bind: binding operand: " << oprnd 
                     << "to physical file: " << oprnd_bind_map.value(&oprnd) << endl;

            oprnd.bind_physical_file(oprnd_bind_map.value(&oprnd));
        }

        if (!El_codegen_fast_mode) {
          if (Codegen::check_for_valid_bind(op, cmgr) == false) {
            El_punt("do_prf_bind: op: %d bound to invalid opdes!\n", op->id());
          }
        }
    }
}

static eString lookup_oprnd(Operand& oprnd, Hash_map<Operand*, eString>& oprnd_bind_map)
{
    for (Hash_map_iterator<Operand*, eString> iter(oprnd_bind_map); iter!=0; iter++) {
        if (oprnd == *((*iter).first)) {
          return (*iter).second;
        }
    }

    return "";
}

void PRF_Bind_Map::bind_operands_of_pseudo_ops(Procedure *f)
{
   if (dbg(genrio,3))
       cdbg << "Binding operands of pseudo ops in procedure: " << f->get_name() << endl;

    for (Region_all_ops opi(f); opi!=0; opi++) {
        Op *op = *opi;

        if (!is_pseudo(op))
            continue;

        for (Op_all_operands oprndi(op); oprndi!=0; oprndi++) {
            Operand& oprnd = *oprndi;
             
            if (oprnd.is_macro_reg()) {
                
                // ignore pseudo macros
                Hash_set<eString>& pseudo_macros = current_MDES->get_MDES_pseudo_macros();
                if (pseudo_macros.is_member(el_macro_to_string_map.value(oprnd.name())))
                    continue;

                eString prf = lookup_oprnd(oprnd, oprnd_bind_map);
                if (prf == "") {
                    Hash_set<eString>& prfs = get_MDES_pf_for_macro(oprnd.name());
                    if (prfs.size() > 1) {
                        El_punt("bind_operands_of_pseudo_ops: macro %d in op: %d has multiple physical files!\n",
                                 oprnd.name(), op->id());
                    } else
                        oprnd.bind_physical_file(prfs.head());
                } else
                    oprnd.bind_physical_file(prf);
            } else if (oprnd.is_reg()) {
                eString prf = lookup_oprnd(oprnd, oprnd_bind_map);
                assert(prf != "");
                oprnd.bind_physical_file(prf);
            }
        }
    }
}

void PRF_Bind_Map::bind_implicit_operands(Procedure *f)
{
    if (dbg(genrio,3))
       cdbg << "Binding implicit operands in procedure: " << f->get_name() << endl;

    for (Region_all_ops opi(f); opi!=0; opi++) {
        Op *op = *opi;
        
        if (is_pseudo(op) || op->flag(EL_OPER_ICMOVE) || is_no_op(op))
            continue;

        List<Operand>* uses = get_implicit_use_list(op);

        if (uses != NULL) {
            for (List_iterator<Operand> li(*uses); li != 0; li++) {
                Operand& opr = *li;
                if (!opr.assigned_to_physical_file()) {
                    if (opr.is_reg()) {
                        eString prf = lookup_oprnd(opr, oprnd_bind_map);
                        assert(prf != "");
                        opr.bind_physical_file(prf);
                    } else if (opr.is_macro_reg()) {
                        eString prf = lookup_oprnd(opr, oprnd_bind_map);
                        if (prf == "") {
                            Hash_set<eString>& prfs = get_MDES_pf_for_macro(opr.name());
                            if (prfs.size() != 1) {
                                El_punt("bind_implicit_operands: macro %s in op: %d has zero or multiple \
                                         physical files!\n", (char*)el_macro_to_string_map.value(opr.name()), 
                                                             op->id());
                            } else {
                                opr.bind_physical_file(prfs.head());
			    }
                        } else
                            opr.bind_physical_file(prf);
                    }
                }
            }
        }

        List<Operand>* defs = get_implicit_def_list(op);

        if (defs != NULL) {
            for (List_iterator<Operand> li(*defs); li != 0; li++) {
                Operand& opr = *li;
                if (!opr.assigned_to_physical_file()) {
                    if (opr.is_reg()) {
                        eString prf = lookup_oprnd(opr, oprnd_bind_map);
                        assert(prf != "");
                        opr.bind_physical_file(prf);
                    } else if (opr.is_macro_reg()) {
                        eString prf = lookup_oprnd(opr, oprnd_bind_map);
                        if (prf == "") {
                            Hash_set<eString>& prfs = get_MDES_pf_for_macro(opr.name());
                            if (prfs.size() != 1) {
                              El_punt("bind_implicit_operands: macro %s in op: %d has zero or multiple physical files!\n",
                                      (char*)el_macro_to_string_map.value(opr.name()), op->id());
                            } else {
                                opr.bind_physical_file(prfs.head());
                            }
                        } else {
                            opr.bind_physical_file(prf);
                        }
                    }
                }
            }
        }
    }
}

void PRF_Bind_Map::check_bound_reg(Compound_region *r)
{
    if (!El_do_scalar_regalloc)
        return;

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;

        for (Op_explicit_operands oprndi(op); oprndi!=0; oprndi++) {
            Operand& oprnd = *oprndi;
            if (oprnd.dtype().is_predicate())
                continue;
            if (oprnd.is_reg() && !oprnd.allocated())
                El_punt("check_bound_reg: vr %d of op: %d not bound!", oprnd.vr_num(), op->id());
        }
    }
}

// after register allocation and clustering, some macros (like SP for spill code)
// are left unbound.  this function binds them to a register and allocates the
// physical register
void PRF_Bind_Map::allocate_unbound_macros(Compound_region *r)
{
   if (!El_do_scalar_regalloc)
       return;
   
   for (Region_all_ops opi(r); opi!=0; opi++) {
     Op *op = *opi;

     for (Op_explicit_dests desti(op); desti!=0; desti++) {
       Operand& dest_operand = *desti;
       if(!dest_operand.is_macro_reg()) continue;

       eString macro_name = el_macro_to_string_map.value(dest_operand.name());
       
       if(!dest_operand.assigned_to_file())
         El_punt("Have a macro %s that's not assigned to a physical file in op %d",
                 macro_name.strdup(), op->id());

       // temporarily put this in as we're not allocating predicates
       //       if(dest_operand.file_type() == PR) continue;

       // if the macro hasn't been allocated, look up the reg number in the mdes
       if(!dest_operand.allocated()) {
         if(current_MDES->is_pseudo_macro(macro_name)) continue;

         eString prf = dest_operand.physical_file_type();
         Reg_descr *rd = current_MDES->get_MDES_reg_descr(MDES_reg_index(prf));
         Hash_set<eString> regs = rd->get_physical_regs(dest_operand.name());

         // Not sure what to do in this case, if the macro has more than
         // one register, how do you register allocate it?
         if(regs.size() > 1) {
           if(!current_MDES->is_multialias_macro(macro_name))
             El_warn("Op %d: Got a macro %s with more than one physical register.",
                     op->id(), macro_name.strdup());
           continue;
         }
         
         // bind the operand to the register
         dest_operand.bind_reg( current_MDES->get_MDES_reg_number_for_pf(regs.pop()) );

       }       
     }     

     for (Op_explicit_inputs srci(op); srci!=0; srci++) {
       Operand& src_operand = *srci;
       if(!src_operand.is_macro_reg())
         continue;

       eString macro_name = el_macro_to_string_map.value(src_operand.name());

       if(!src_operand.assigned_to_file())
         El_punt("Have a macro %s that's not assigned to a physical file in op %d",
                 macro_name.strdup(), op->id());

       // temporarily put this in as we're not allocating predicates
       //       if(src_operand.file_type() == PR) continue;

       // if the macro hasn't been allocated, look up the reg number in the mdes
       if(!src_operand.allocated()) {
         if(current_MDES->is_pseudo_macro(macro_name)) continue;

         eString prf = src_operand.physical_file_type();
         Reg_descr *rd = current_MDES->get_MDES_reg_descr(MDES_reg_index(prf));
         Hash_set<eString> regs = rd->get_physical_regs(src_operand.name());

         // Not sure what to do in this case, if the macro has more than
         // one register, how do you register allocate it?
         if(regs.size() > 1) {
           if(!current_MDES->is_multialias_macro(macro_name))
             El_warn("Op %d: Got a macro %s with more than one physical register.",
                     op->id(), macro_name.strdup());
           continue;
         }

         // bind the operand to the register
         src_operand.bind_reg( current_MDES->get_MDES_reg_number_for_pf(regs.pop()) );
       }       
     }  
   }

}

void PRF_Bind_Map::allocate_icmove_reg(Compound_region *r)
{
   if (!El_do_scalar_regalloc)
       return;
   if (dbg(genrio,3))
       cdbg << "Allocating ICMOVE register in region: " << r->id() << " in proc: "
            << (char*)(el_get_enclosing_procedure(r))->get_name() << endl;

    delete_local_analysis_info_for_all_hbs_bbs(r);
    create_local_analysis_info_for_all_hbs_bbs(r);

    El_do_reaching_defs (r, ANALYZE_ALLREG) ;
    Reaching_defs_info* rdinfo = get_reaching_defs_info (r);

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op) || is_no_op(op))
            continue;

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
            Operand& dest_oprnd = *desti;
            if (!dest_oprnd.is_reg())
                continue;
            if (dest_oprnd.allocated())
                continue;
            //            if (dest_oprnd.file_type() == PR)
            //                continue;
            El_ref def = desti.get_ref();
            List<El_ref>* use_list = rdinfo->get_du_chain(def);

            if (use_list->size() == 0)
                continue;

            eString prf = dest_oprnd.physical_file_type();
            Reg_descr *rd = current_MDES->get_MDES_reg_descr(MDES_reg_index(prf));
            Hash_set<eString> macros(hash_estring, 8);
            rd->get_macros(macros);
            if (!macros.is_member("ICMOVE_REG"))
              El_punt("allocate_icmove_reg: Need to reserve an icmove register for file: %s", (char*)prf);

            Macro_name icmove_reg = el_string_to_macro_map.value("ICMOVE_REG");
            Hash_set<eString> regs = rd->get_physical_regs(icmove_reg);
            int icmove_reg_num = current_MDES->get_MDES_reg_number_for_pf(regs.pop());

            op->dest(def.get_port_num()).bind_reg(icmove_reg_num);

            for (List_iterator<El_ref> liter(*use_list); liter!=0; liter++) {
                El_ref use = *liter;

                assert(use.get_ref_type() == EXP_SRC || use.get_ref_type() == PRED_SRC);
                Operand& src_oprnd = use.get_operand();
                assert(src_oprnd.is_reg() && !src_oprnd.allocated());
                //assert(use_list->size() == 1);
                // Why must size be 1?  Sometimes it isn't 1, and we're in a
                // for loop after all.  -KF 1/2006
                assert((rdinfo->get_ud_chain(use))->size() == 1);
                assert(src_oprnd.physical_file_type() == dest_oprnd.physical_file_type());

                if (dbg(genrio,1) || dbg(clst,1)) 
                  cdbg << "binding src_oprnd: " << src_oprnd << " at port: " << use.get_port_num() 
                    << " of op: " << use.get_op()->id() << " and dest_oprnd: " << dest_oprnd 
                    << " at port: " << desti.get_port_num() << " of op: " << op->id() 
                    << " with ICMOVE_REG macro reg " << endl;

                Op *use_op = use.get_op();
                use_op->src(use.get_port_num()).bind_reg(icmove_reg_num);
            }
        }
    }

    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_postpass_allocreg_codegen.reb", (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel after Postpass Allocate Registers for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }

    remove_reaching_defs_info (r) ;
    delete_local_analysis_info_for_all_hbs_bbs(r);
}

void PRF_Bind_Map::print_map()
{
    cdbg << "Printing PRF_Bind_Map..." << endl;
    for (Hash_map_iterator<Operand*, eString> iter(oprnd_bind_map); iter!=0; iter++) {
        cdbg << "oprnd : " << *((*iter).first);
        cdbg << " prf: " <<  (*iter).second << endl;
    }
}

void bind_operands_of_pseudo_ops(PRF_Bind_Map& prf_bind_map, Procedure *f)
{
    prf_bind_map.bind_operands_of_pseudo_ops(f);
}

void bind_implicit_operands(PRF_Bind_Map& prf_bind_map, Procedure *f)
{
    prf_bind_map.bind_implicit_operands(f);
}

#if 0
void PRF_Bind_Map::allocate_icmove_reg(Compound_region *r)
{
    if (dbg(genrio,1))
        cdbg << "Postpass allocate registers for region: " << r->id() << endl;

    create_local_analysis_info_for_all_hbs_bbs(r);

    El_do_reaching_defs (r, ANALYZE_ALLREG) ;
    Reaching_defs_info* rdinfo = get_reaching_defs_info (r);

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
        if (is_pseudo(op))
            continue;

        for (Op_explicit_dests desti(op); desti!=0; desti++) {
            Operand& dest_oprnd = *desti;
            if (!dest_oprnd.is_reg())
                continue;
            El_ref def = desti.get_ref();
            List<El_ref>* use_list = rdinfo->get_du_chain(def);
            for (List_iterator<El_ref> liter(*use_list); liter!=0; liter++) {
                El_ref use = *liter;
                Operand& src_oprnd = use.get_operand();
                if (dest_oprnd.is_reg() && dest_oprnd.allocated()) {
                    if (src_oprnd.is_reg() && src_oprnd.allocated())
                        break;
                    else if (src_oprnd.is_reg()) { // && !src_oprnd.allocated())
                        // ignore predicate registers
                        if (src_oprnd.file_type() == PR)
                            continue;

                        assert(use_list->size() == 1);

                        src_oprnd.bind_reg(dest_oprnd.mc_num());
                    }
                } else if (dest_oprnd.is_reg()) { // && !dest_oprnd.allocated())
                    // ignore predicate registers
                    if (dest_oprnd.file_type() == PR)
                        continue;
                    if (src_oprnd.is_reg() && src_oprnd.allocated()) {
                        assert(use_list->size() == 1);

                        dest_oprnd.bind_reg(src_oprnd.mc_num());

                    } else if (src_oprnd.is_reg()) { // && !src_oprnd.allocated())
                        assert(use_list->size() == 1);
                        assert(src_oprnd.physical_file_type() == dest_oprnd.physical_file_type());
                        eString prf = src_oprnd.physical_file_type();
                        int icm_reg = RegisterBankPool::reg_bank((char*)prf).icmove_reg();

                        if (src_oprnd.dtype().is_predicate() || dest_oprnd.dtype().is_predicate())
                            continue;

                        if (dbg(genrio,1) || dbg(clst,1)) 
                            cdbg << "binding src_oprnd: " << src_oprnd << " of op: " << use.get_op()->id() 
                                 << "and dest_oprnd: " << dest_oprnd << " of op: " << op->id() << endl;

                        // ignore predicate registers
                        src_oprnd.bind_reg(icm_reg);
                        dest_oprnd.bind_reg(icm_reg);
                    }
                }
            }
        }
    }

    if (dbg(genrio,5)) {
        char buff[256];
        sprintf(buff, "%s_%d_postpass_allocreg_codegen.reb", (char*)(el_get_enclosing_procedure(r))->get_name(), r->id());
        cdbg << "Printing Rebel after Postpass Allocate Registers for region: " << r->id() << endl;
        IR_outstream dbg_out(buff);
        ir_write(dbg_out, el_get_enclosing_procedure(r));
    }

    delete_local_analysis_info_for_all_hbs_bbs(r);
    remove_reaching_defs_info (r) ;
}
#endif

