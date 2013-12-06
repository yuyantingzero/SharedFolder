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
//									 //
//      File:           el_opti_dead_code.cpp                            //
//      Authors:        Sadun Anik					 //
//      Created:        July 1996					 //
//      Description:    Dead Code Eliminator Implementation		 //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include "el_opti_dead_code.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "connect.h"
#include "dbg.h"
#include "attributes.h"

static bool is_analyzed(Operand &oper, unsigned int filter_flag)
{
   if ((filter_flag & ANALYZE_MEMVR) && oper.is_mem_vr()) return true ;
   if ((filter_flag & ANALYZE_ALLREG) && (oper.is_reg() || oper.is_macro_reg())) {
      if ((filter_flag & ANALYZE_ALLREG) == ANALYZE_ALLREG) return true ;
      Reg_file cur_rfile = oper.file_type() ;
      /* RMR { make VIR and VFR and VMR (should VMR be treated like an INT?) interesting */
      if ((filter_flag & ANALYZE_INT) && (cur_rfile == GPR || cur_rfile == VIR ||
          cur_rfile == VMR)) return true ;
      if ((filter_flag & ANALYZE_FP) && (cur_rfile == FPR || cur_rfile == VFR)) return true ;
      /* } RMR */
      if ((filter_flag & ANALYZE_BTR) && (cur_rfile == BTR)) return true ;
      if ((filter_flag & ANALYZE_PR) && (cur_rfile == PR)) return true ;
      if ((filter_flag & ANALYZE_CR) && (cur_rfile == CR)) return true ;
   }
   return false ;
}

static bool is_analyzed(El_ref& cur_ref, unsigned int filter_flag)
{
   if (cur_ref.get_operand().is_reg() ||
       cur_ref.get_operand().is_mem_vr() ||
       cur_ref.get_operand().is_macro_reg())
      return (is_analyzed(cur_ref.get_operand(), filter_flag));
   else
      return false ;
}

/*
 *	SAM 8-3-2000
 *	Trivial ops are ones which really do nothing, and thus can be deleted.
 *	This is irregardless as to whether the operand was analyzed or whether
 *	it is marked as LIVEOUT.
 *	Such ops include:
 *		copy with dest=src
 */
static bool is_trivial_op(Op *cur_op)
{
    if ((is_move(cur_op)) && (cur_op->dest(DEST1) == cur_op->src(SRC1)))
	return (true);

    return (false);
}

static bool is_deletable_op(Op* cur_op, unsigned int filter_flag, bool be_conservative)
{
   if (cur_op->num_dests() == 0) return (false) ;
   if (is_control_switch(cur_op)) return (false) ;
   if (is_store(cur_op)) return (false) ;
   if (is_remap(cur_op)) return (false) ;   
   if (is_pseudo(cur_op)) return (false) ;
   if (is_custom(cur_op)) return (false) ;
//    if (is_predicate_set(cur_op) ||
//        is_predicate_clear(cur_op) ||
// Why are pred_sets and pred_clears undeletable?  Commenting out this check seems to fix
// a lot of benchmarks.  -KF 1/2006
   if (is_clear_all_rotating(cur_op)) return (false) ;
   if (cur_op->flag(EL_OPER_LIVEOUT_DEST)) return (false);

#if 1
   // HACK because analysis cannot yet handle vector registers.  -KF 1/2007
   for (Op_complete_dests desti(cur_op); desti != 0; desti++) {
     Operand& dest = *desti;
     if (dest.is_reg() &&
         (dest.dtype().is_vec_int() || dest.dtype().is_vec_float()))
       return false;
   }
#endif

   /* SAM, 3-98 check filter flag to make more fine grain dead code decisions */
   /* If ANALYZE_ALLREG or ANALYZE_ALL, then full analysis done, so deleteable */
   if (!be_conservative && 
       (filter_flag == ANALYZE_ALLREG) || (filter_flag == ANALYZE_ALL))
      return (true) ;

   /* Otherwise, partial analysis was done, so check the dest operands of cur_op */
   for (Op_complete_dests dest_i(cur_op); dest_i!=0; dest_i++) {
      Operand dest = *dest_i;
      if (dest.is_undefined())
         continue;
      if (! is_analyzed(dest, filter_flag | ANALYZE_MEMVR))
        // Allow mem_vrs here.  Example: mvr<1> vr<2> = LOAD , if vr<2> is dead
        // we should be allowed to delete the load.  If we don't delete the
        // load, register allocation will produce wrong code.  Also stores and
        // brls (the other ops that define mem_vrs) are taken care of at the top
        // of the function.  -KF 1/2006
         return (false);
      if (be_conservative && (dest.is_macro_reg() || dest.is_rotating()))
	 return (false);
   }

   /* if being conservative, don't delete a rotating source (ie postloop move) */
   if (be_conservative) {
      for (Op_explicit_sources src_i(cur_op); src_i!=0; src_i++) {
	 Operand src = *src_i;
	 if (src.is_reg() && src.is_rotating())
	    return false;
      }
   }

   return (true);
}

/*
 *  SAM 8-96, added to fix opcodes for ops which have several dead dest
 *  operands, but the op could not be completely deleted
 */
static void fix_opcode_for_undefined_operands(Compound_region* R)
{
  Op *cur_op;
  Operand dest, dest1, dest2, undef;
  Opcode new_opc;
  int num_undef;

  for (Region_all_ops reg_opi(R) ; reg_opi != 0 ; reg_opi++) {
    cur_op = *reg_opi ;
    if (is_pseudo(cur_op))
      continue;
    //hack for custom op, hongtaoz 05/26/2003
    if(cur_op->opcode()==CUSTOM)
      continue;
    if (cur_op->num_dests() <= 0)
      continue;
    /* KVM : Don't dead-code-eliminate for ADD_CARRY_W or MPY_WIDE*/
    if(is_add_carry(cur_op) || is_mul_wide(cur_op) || is_sub_carry(cur_op)) {
      continue;
    }
    /* count up the number of undefined dests */
    num_undef = 0;
    for (Op_explicit_dests desti(cur_op) ; desti != 0 ; desti++) {
      dest = *desti;
      if (dest.is_undefined())
        num_undef++;
    }
    if (num_undef == 0)
      continue;
    /* Ok, we have some undefined dests, so try to update the opcode */
    if (is_cmpp(cur_op)) {
      /* For cmpp, make sure the real operand is in dest1, if not swap dests */
      if (cur_op->dest(DEST1).is_undefined()) {
        if (cur_op->dest(DEST2).is_undefined())
          El_punt("fix_opcode_for_undefined_operands: both dests of cmpp undefined");
        dest1 = cur_op->dest(DEST1);
        dest2 = cur_op->dest(DEST2);
	cur_op->set_dest(DEST1, dest2);
	cur_op->set_dest(DEST2, dest1);
        new_opc = exchange_cmpp_dest_modifiers(cur_op->opcode());
        cur_op->set_opcode(new_opc);
        // SAM 4-2000 Need to swap the Reference_dtype_attr width annots too!!!
        Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(cur_op);
        if (rdt_attr != NULL) {
          rdt_attr->swap_dests(DEST1, DEST2);
        }
        if (dbg(opti,3))
          cdbg << "\tDead code elim: swapping dests of cmpp " << cur_op->id() << endl;
      }
    }
    else if (is_load(cur_op)) {
      /* For a load, if DEST1 is undefined, convert op to an increment, if
         DEST2 is undefined convert it to a simple load */
      if (cur_op->dest(DEST1).is_undefined()) {
        if (cur_op->dest(DEST2).is_undefined())
          El_punt("fix_opcode_for_undefined_operands: both dests of li undefined");
        new_opc = extract_increment_from_post_increment(cur_op->opcode());
        cur_op->set_dest(DEST1, cur_op->dest(DEST2));
        cur_op->set_dest(DEST2, undef);
        cur_op->set_opcode(new_opc);
        cur_op->update_num_operands();
        if (dbg(opti,3))
          cdbg << "\tDead code elim: Converting " << cur_op->id()
               << " from a post inc load to an increment" << endl;
      }
      else if (cur_op->dest(DEST2).is_undefined()) {
        new_opc = remove_post_increment(cur_op->opcode());
        cur_op->set_dest(DEST2, undef);
        cur_op->set_src(SRC2, undef);
        cur_op->set_opcode(new_opc);
        cur_op->update_num_operands();
        if (dbg(opti,3))
          cdbg << "\tDead code elim: Converting " << cur_op->id()
               << " from a post inc load to a regular load" << endl;
      }
    }
    else if (is_store(cur_op)) {
      /* For a store, convert post increment store to regular store */
      if (cur_op->dest(DEST1).is_undefined()) {
        new_opc = remove_post_increment(cur_op->opcode());
        cur_op->set_dest(DEST1, undef);
        cur_op->set_src(SRC3, undef);
        cur_op->set_opcode(new_opc);
        cur_op->update_num_operands();
        if (dbg(opti,3))
          cdbg << "\tDead code elim: Converting " << cur_op->id()
               << " from a post inc store to a regular store" << endl;
      }
    }
    else {
      El_punt("fix_opcode_for_undefined_operands: unknown op with multiple dests");
    }
  }
}


/*
 *	SAM 8-8-2000
 *	New version of dead code elimination below.  Slightly smarter than
 *	the old version as it handles dead code in loops smarter, ie it can
 *	detect k++ as dead if there are no consumers of k other than itself.
 *	This should work (in theory) for any cycles of ops that are self
 *	sustaining.
 */

static void mark_all_op_outputs_as_useful(Op *op, Hash_set<El_ref> &useful,
				unsigned int filter_flag)
{
    for (Op_all_dests dest_i(op); dest_i!=0; dest_i++) {
        El_ref ref = dest_i.get_ref();
        if (is_analyzed(ref, filter_flag))
	    useful += ref;
    }
}

static void mark_all_op_inputs_for_processing(Op *op, Hash_set<El_ref> &to_process,
				unsigned int filter_flag)
{
    for (Op_all_inputs src_i(op); src_i!=0; src_i++) {
        El_ref ref = src_i.get_ref();
        if (is_analyzed(ref, filter_flag))
	    to_process += ref;
    }
}

/*
 *	to_process = SRC refs that must be examined
 *	processed = SRC refs that have been examined
 *	useful = DEF refs that produce useful values
 *	When done, any DEF refs that were analyzed that are not in useful
 *	are dead.
 */
static void construct_useful_refs_list(Compound_region* R, Hash_set<El_ref> &useful,
			Reaching_defs_info* rd_info, unsigned int filter_flag, bool be_conservative)
{
    Hash_set<El_ref> to_process(hash_El_ref, 4096);
    Hash_set<El_ref> processed(hash_El_ref, 4096);

    /* First add all the non-deletable ops and those ops which produce live-out values
       to the useful lists and to_process lists */
    for (Region_all_ops op_i(R); op_i!=0; op_i++) {
	Op *op = *op_i;
	if (! is_deletable_op(op, filter_flag, be_conservative)) {
	    mark_all_op_outputs_as_useful(op, useful, filter_flag);
	    mark_all_op_inputs_for_processing(op, to_process, filter_flag);
	}
	else { // Check if it produces a liveout value
	    for (Op_all_dests dest_i(op); dest_i!=0; dest_i++) {
	        El_ref ref = dest_i.get_ref();
	        if (! is_analyzed(ref, filter_flag))
		    continue;
		List<El_ref>* uses = rd_info->get_du_chain(ref);
	        bool liveout = false;
		for (List_iterator<El_ref> li(*uses); li!=0; li++) {
		    El_ref use = *li;
		    El_ref_type use_type = use.get_ref_type();
		    if ((use_type == LIVEOUT_TAKEN_SRC) ||
			(use_type == LIVEOUT_FALLTHROUGH_SRC)) {
			liveout = true;
			break;
		    }
		}
		if (liveout) {
		    useful += ref;
		    mark_all_op_inputs_for_processing(op, to_process, filter_flag);
		}
	    }
	}
    }

    /*
     * For each op in to_process, add the reach defs to useful and the source operands
     * for the def to the to_process list
     */
    while (! to_process.is_empty()) {
	El_ref cur_ref = to_process.pop();
	processed += cur_ref;
	List<El_ref>* defs = rd_info->get_ud_chain(cur_ref);
	for (List_iterator<El_ref> li(*defs); li!=0; li++) {
	    El_ref def = *li;
	    El_ref_type def_type = def.get_ref_type();
	    if (!((def_type == EXP_DEST) || (def_type == IMP_DEST)))
	        continue;
	    useful += def;
	    for (Op_all_inputs src_i(def.get_op()); src_i!=0; src_i++) {
	        El_ref src_ref = src_i.get_ref();
	        if (is_analyzed(src_ref, filter_flag) &&
	    	    !processed.is_member(src_ref))
		    to_process += src_ref;
   	    }
	}
    }
}

//
// SAM 8-2000, Conservative elimination of remaps to which there is no reference
// to the register being remapped.
//
static void find_useful_reg_nums(Compound_region *R, Hash_set<El_ref> &useful,
			Hash_set<int> &useful_regs)
{

    // First handle references in the region
    for (Hash_set_iterator<El_ref> h_i(useful); h_i!=0; h_i++) {
	El_ref ref = *h_i;
	Operand operand = ref.get_operand();
	Op *op = ref.get_op();
	// Use/def by remap or define itself does not make the reg useful
	if (is_remap(op) || is_define(op))
	    continue;
	if (operand.is_reg())
	    useful_regs += operand.vr_num();
    }

    // Otherwise, worry about references external to the region
    for (Region_entry_edges entry_i(R, CONTROL0_INEDGES); entry_i!=0; entry_i++) {
	Edge *edge = *entry_i;
	Liveness_info* uu = get_liveness_info(edge);
	if (uu) {
	    for (Liveness_info_iterator li(*uu); li!=0; li++) {
		Operand operand = *li;
		if (operand.is_reg())
		    useful_regs += operand.vr_num();
	    }
	}
    }

    for (Region_exit_edges exit_i(R, CONTROL0_OUTEDGES); exit_i!=0; exit_i++) {
	Edge *edge = *exit_i;
	Liveness_info* uu = get_liveness_info(edge);
	if (uu) {
	    for (Liveness_info_iterator li(*uu); li!=0; li++) {
		Operand operand = *li;
		if (operand.is_reg())
		    useful_regs += operand.vr_num();
	    }
	}
    }
}

static void identify_useless_remaps_and_defines(Compound_region* R,
		Hash_set<El_ref> &useful, Hash_set<Op*> &to_nuke)
{
    Hash_set<int> useful_regs(hash_int, 128);
    find_useful_reg_nums(R, useful, useful_regs);

    for (Region_all_ops op_i(R); op_i!=0; op_i++) {
	Op *op = *op_i;
	// it is not correct to skip defines here.  The problem is
	// that useful only contains def refs.  So a variable that is
	// live-in and read-only has its define deleted!  What we
	// need to do is only delete defines when there are no uses.
	// dcronqui, 11/2/01.

	//if (! (is_remap(op) || is_define(op)))
	if (! (is_remap(op)))
	    continue;
	Operand dest = op->dest(DEST1);
	if (dest.is_macro_reg())
	    continue;
	if (useful_regs.is_member(dest.vr_num()))
	    continue;
	to_nuke += op;
	if (dbg(opti,3)) {
	    if (is_remap(op))
                cdbg << "\t\tNEW Dead code elim: Deleting REMAP " << op->id() << endl;
	    else
                cdbg << "\t\tNEW Dead code elim: Deleting DEFINE " << op->id() << endl;
	}
    }
}

// SAM 3-2001
// Attributes that are persistent over long periods of time may become
// invalid after dead code elim because the op they refer to has been
// nuked.  So, this routine should invalidate any such attributes.  
// Currently we only worry about the Op_chaining_attr, but more may need
// to be added in the future.
//static 
void update_graph_attributes_for_dead_ops(Compound_region *R,
		Hash_set<Op*> &to_nuke)
{
    Op *op;
    Op_chaining_attr *opc_attr;

    for (Region_all_ops opi(R); opi!=0; opi++) {
	op = *opi;
	opc_attr = get_op_chaining_attr(op);
	if (opc_attr == NULL)
	    continue;
	if (dbg(opti, 6))
	    cdbg << "Invalidating chains for Op: " << op->id() << endl;
	opc_attr->invalidate_chains(to_nuke, (dbg(opti, 6)));
    }
}

// be_conservative:
// prevents deletion of rotating registers and macro registers.
// this allows us to run dead code elim on preloop of a modulo-scheduled
// loop kernel, eg LC, ESC defines won't get deleted. this is necessary
// because we can't analyze the modulo scheduled kernel.
// A better fix would be to incrementally update the liveness analysis
// after modulo scheduling and rotating register allocation to reflect
// the register bindings and swp macros. However that's hard.
void el_opti_eliminate_dead_code(Compound_region* R, unsigned int filter_flag,
				 bool run_rdefs, bool be_conservative)
{
   if (dbg(opti, 1) || dbg(status, 3)) 
    cdbg << "-- Global dead code elimination " << endl;

    if (run_rdefs == true) {
	create_local_analysis_info_for_all_hbs_bbs(R) ;
	El_do_reaching_defs (R, filter_flag) ;
    }
    Reaching_defs_info* rdinfo = get_reaching_defs_info (R) ;

    Hash_set<El_ref> useful(hash_El_ref, 4096);
    construct_useful_refs_list(R, useful, rdinfo, filter_flag, be_conservative);

    Hash_set<Op*> to_nuke(hash_op_ptr, 128);
    for (Region_all_ops op_i(R); op_i!=0; op_i++) {
        Op *op = *op_i;
	if (! is_deletable_op(op, filter_flag, be_conservative))
	    continue;
	bool nuke = true;
	for (Op_all_dests dest_i(op); dest_i!=0; dest_i++) {
	    Operand dest = *dest_i;
	    if (dest.is_undefined())
		continue;
	    El_ref ref = dest_i.get_ref();
	    if (useful.is_member(ref)) {
		nuke = false;
	    }
	    else if (ref.get_ref_type() == EXP_DEST) {
                Operand undefined;
                op->set_dest(dest_i.get_port_num(), undefined);
	        if (dbg(opti,3))
                    cdbg << "\tNEW Dead code elim: Deleting dest operand "
		         << dest_i.get_port_num() << " of op " << op->id() << endl;
	    }
	}
	if (nuke || is_trivial_op(op)) {
	   to_nuke += op;
	   if (dbg(opti,3))
               cdbg << "\t\tNEW Dead code elim: Deleting op " << op->id() << endl;
	}
    }

    // SAM 8-2000, added
    // Currently it seems too aggressive when it operates on individual basic blocks
    // with remaps in them, and it deletes stuff it should not.  It should work
    // fine for the whole loop or procedure
    if (R->is_procedure() || R->is_loopbody())
        identify_useless_remaps_and_defines(R, useful, to_nuke);

    // SAM 3-2001, added
    update_graph_attributes_for_dead_ops(R, to_nuke);

    for (Hash_set_iterator<Op*> hi(to_nuke); hi!=0; hi++) {
	Op *op = *hi;
        C0_remove_op(op) ;
        op->parent()->remove_region(op) ;
        delete op ;
    }
      
    fix_opcode_for_undefined_operands(R);

    if (run_rdefs == true) {
       delete_local_analysis_info_for_all_hbs_bbs(R) ; 
       remove_reaching_defs_info(R);
    }
}
