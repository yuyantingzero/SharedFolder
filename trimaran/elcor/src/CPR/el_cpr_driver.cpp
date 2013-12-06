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



/*===========================================================================
 *
 *      File:           el_cpr_driver.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        February 1997
 *      Description:    Driver for CPR xforms
 *
 *===========================================================================*/
#include "dbg.h"
#include "region.h"
#include "iterators.h"
#include "dlist.h"
#include "el_cpr_init.h"
#include "el_debug_init.h"
#include "el_cpr_driver.h"
#include "el_frpize.h"
#include "el_promote_preds.h"
#include "pred_analysis.h"
#include "flow_analysis_solver.h"
#include "edge_drawing.h"
#include "opcode_properties.h"
#include "el_if_converter.h"
#include "el_if_conv_opti.h"
#include "el_normalize_branches.h"
#include "el_control.h"
#include "el_bb_tools.h"
#include "cont_cpr_descr.h"
#include "cont_cpr_match.h"
#include "el_cpr_prep.h"
#include "el_bypass_branch.h"
#include "el_cpr_blocking.h"
#include "el_code_motion.h"
#include "el_cmp_reduce.h"
#include "el_cpr_stats.h"
#include "el_cpr_driver.h"
#include "el_opti_dead_code.h"
#include "el_stats_init.h"
#include "ir_writer.h"


/*===========================================================================*/
/*
 *	Driver functions for original CPR method
 */
/*===========================================================================*/
#define EL_CPR_MIN_WEIGHT	El_min_cpr_weight

bool El_hb_is_suitable_for_cpr(Hyperblock* hb)
{
    Op *op;
    Operand src, dst;
    Hash_set<Operand> unrenamable_operands(hash_operand);

    /* Minimum weight check */
    if (hb->weight < EL_CPR_MIN_WEIGHT)
	return (false);

    /* check for jsrs, they introduce dependences that are not really accounted
	for in the xform, so the code may break. */
    for (Region_ops_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	if (is_brl(op) || is_rts(op))
	    return (false);
    }

    return (true);
}


/*
 *	Should only be called for frpized blocks
 */
void El_hb_scalar_control_cpr_original(Hyperblock* hb)
{
    Cont_cpr_match_result match_result;
    El_cpr_chain_list *hb_chain_list;

    //if (hb->id() != 4 &&
    //   hb->id()!=61)return;
     // && hb->id()!=34) return;
    //if(hb->id()!=64 && hb->id()!=63
//	 &&hb->id()!=19 && hb->id()!=58)return;
    
    if (El_dbg_block_id !=0 && hb -> id() != El_dbg_block_id) return;

    if (dbg(cpr, 2))
        cdbg << "Doing scalar control cpr (Original method) to HB "
	     << hb->id() << endl;

    // Steps
    // 1. FRPize (done before entering this procedure)
    // 2. Predicate speculation
    // 3. Mike's pattern match
    // 4. Heuristic blocking (this is done within step 3 now)
    // 5. wired-AND/FT branch insertion
    // 6. Tree region insertion ??
    // 7. Code motion off trace/predicate splitting

    // Step 2 (assume live-variable analysis done?)
    if (El_predicate_speculation) {
        if (dbg(cpr, 2))
            cdbg << "Doing pred spec (hb " << hb->id() << ")" << endl;
	El_promote_preds(hb);
    }

    // Make sure some criteria are met before applying further Xform */
    if (! El_hb_is_suitable_for_cpr(hb)) {
        if (dbg(cpr, 2))
            cdbg << "HB " << hb->id() << " NOT eligible for CPR" << endl;
	return;
    }

    // Draw edges on the HB
    insert_region_scalar_edges(hb);

    // Step3: (mike) assume edges drawn
    if (El_do_find_chains) {
        if (dbg(cpr, 2))
            cdbg << "Doing pattern match (hb " << hb->id() << ")" << endl;
	match_result = cpr_match_hb_original(hb);
    }

    // Remove the edges
    delete_region_edges(hb);

    // Collect match statistics (size/num cpr blocks)
    if (El_do_cpr_blk_info)
	El_compute_cpr_block_stats(hb, match_result);

    // Step4b, Get rid of blocks that are not profitable to perform cpr on
    if (El_do_prune_cpr_blocks) {
        if (dbg(cpr, 2))
            cdbg << "Pruning cpr blocks (hb " << hb->id() << ")" << endl;
	El_prune_cpr_blocks(hb, match_result);
    }

    El_insert_dummy_branch(hb);

    // Step5
    hb_chain_list = new El_cpr_chain_list(hb);
    if (El_do_bypass_branch) {
        if (dbg(cpr, 2))
            cdbg << "Inserting bypass branches (hb " << hb->id() << ")" << endl;
	El_insert_bypass_branches(hb, match_result, hb_chain_list);
    }

    // Step6
    // Not done right now

    // New predicates added by bypass br insertion, so need to redeclare the pqs's
    create_local_analysis_info_for_all_hbs_bbs(hb->parent());
    el_flow_compute_liveness(hb->parent(), ANALYZE_ALLREG);

    // Draw edges on the HB
    insert_region_scalar_edges(hb);

    // Step7
    if (El_do_off_trace_motion) {
        if (dbg(cpr, 2))
            cdbg << "Moving code off-trace (hb " << hb->id() << ")" << endl;
	El_off_trace_code_motion(hb, hb_chain_list);
    }

    if (hb_chain_list) {
	delete hb_chain_list;
	hb_chain_list = NULL;
    }

    // Remove the edges
    delete_region_edges(hb);

#if 0
    if (hb->id()==5) {
        cout << "EDGE DUMP for HB " << hb->id() << endl;
        create_local_analysis_info_for_all_hbs_bbs(hb->parent());
        el_flow_compute_liveness(hb->parent(), ANALYZE_ALLREG);
        insert_region_scalar_edges(hb);
        cout << *hb;
        delete_region_edges(hb);
    }
#endif
}


/*===========================================================================*/
/*
 *	Driver functions for max reduce method
 */
/*===========================================================================*/

void El_hb_scalar_control_cpr_maxreduce(Hyperblock* hb)
{
    Cont_cpr_match_result match_result;

    if (dbg(cpr, 2))
        cdbg << "Doing scalar control cpr (Max reduce method) to HB "
             << hb->id() << endl;

    // Steps
    // 1. FRPize (done before entering this procedure)
    // 1.5. Explicit FT branch
    // 2. Predicate speculation
    // 3. Identify CPR blocks
    // 4. Insert stop nodes for scheduling
    // 5. Wide-and predicate insertion

    // Step1.5
    El_insert_explicit_ft_branch(hb);
    create_local_analysis_info_for_all_hbs_bbs(hb);

    // Step 2 (assume live-variable analysis done?)
    if (El_predicate_speculation) {
        if (dbg(cpr, 2))
            cdbg << "Doing pred spec (hb " << hb->id() << ")" << endl;
        El_promote_preds(hb);
    }

    // Make sure some criteria are met before applying further Xform */
    if (! El_hb_is_suitable_for_cpr(hb)) {
        if (dbg(cpr, 2))
            cdbg << "HB " << hb->id() << " NOT eligible for CPR" << endl;
	return;
    }

    // Draw edges on the HB
    insert_region_scalar_edges(hb);

    // Step3: (mike) assume edges drawn
    if (El_do_find_chains) {
        if (dbg(cpr, 2))
            cdbg << "Doing pattern match (hb " << hb->id() << ")" << endl;
        match_result = cpr_match_hb_maxreduce(hb);
    }

    // Remove the edges
    delete_region_edges(hb);

    // Collect match statistics (size/num cpr blocks)
    if (El_do_cpr_blk_info)
	El_compute_cpr_block_stats(hb, match_result);

    // Step4b, Get rid of blocks that are not profitable to perform cpr on
    if (El_do_prune_cpr_blocks) {
        if (dbg(cpr, 2))
            cdbg << "Pruning cpr blocks (hb " << hb->id() << ")" << endl;
        El_prune_cpr_blocks(hb, match_result);
    }

    // Step4
    El_insert_dummy_branch(hb);

    // Step5
    El_max_reduce_cpr_blocks(hb, match_result);

#if 0
    if (hb->id()==4) {
        cout << "EDGE DUMP for HB " << hb->id() << endl;
        create_local_analysis_info_for_all_hbs_bbs(hb->parent());
        el_flow_compute_liveness(hb->parent(), ANALYZE_ALLREG);
        insert_region_scalar_edges(hb);
        cout << *hb;
        delete_region_edges(hb);
    }
#endif

}


/*===========================================================================*/
/*
 *	Invoke the appropriate real CPR driver module
 */
/*===========================================================================*/

void El_hb_scalar_control_cpr(Hyperblock* hb)
{
    switch (El_cpr_strategy) {
	case EL_CPR_STRATEGY_ORIGINAL:
	    El_hb_scalar_control_cpr_original(hb);
	    break;
	case EL_CPR_STRATEGY_MAX_REDUCE:
	    El_hb_scalar_control_cpr_maxreduce(hb);
	    break;
	default:
	    El_punt("El_hb_scalar_control_cpr: uknown CPR strategy");
    }
}

/*===========================================================================*/
/*
 *	CPR pre/post processing
 */
/*===========================================================================*/

void El_control_cpr_preprocess(Procedure *f)
{
}

void El_control_cpr_postprocess(Procedure *f)
{
    if (El_cpr_strategy == EL_CPR_STRATEGY_MAX_REDUCE) {
	create_local_analysis_info_for_all_hbs_bbs(f);
	el_flow_compute_liveness(f, ANALYZE_ALLREG);
	delete_local_analysis_info_for_all_hbs_bbs(f);
	el_opti_eliminate_dead_code(f);
    }
    else if (El_cpr_strategy == EL_CPR_STRATEGY_ORIGINAL) {
	delete_local_analysis_info_for_all_hbs_bbs(f);
    }
}

/*===========================================================================*/
/*
 *	External function
 */
/*===========================================================================*/

void El_scalar_control_cpr(Procedure *f)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rtmp;

    if (dbg(cpr, 1))
        cdbg << "Enter scalar control cpr (fn " << f->get_name() << ")" << endl;

    El_control_cpr_preprocess(f);

    El_form_basic_blocks(f);
    el_normalize_branches(f);

    if (El_do_frpize) {
        if (dbg(cpr, 2))
            cdbg << "FRPizing all SBs" << endl;
	El_frpize_sbs(f);
    }
	

    el_denormalize_branches(f);
    El_remove_basic_blocks(f);
    El_combine_single_dest_pred_defines(f);

    create_local_analysis_info_for_all_hbs_bbs(f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);

    rstack.push(f);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
	    if (rtmp->flag(EL_REGION_HYPERBLOCK_FRP))
                El_hb_scalar_control_cpr((Hyperblock*)rtmp);
            push_flag = false;  // Only ops below, so don't waste time!
        }
        else if (rtmp->is_bb()) {
            push_flag = false;  // Only ops below, so don't waste time!
        }
        if (push_flag==true) {
            for (Region_subregions subri(rtmp); subri!=0; subri++) {
                if ((*subri)->is_compound())
                    rstack.push(*subri);
            }
        }
    }

    El_control_cpr_postprocess(f);

}
