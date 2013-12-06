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




/////////////////////////////////////////////////////////////////////////////
//
//      File:           el_reg_stats.cpp
//      Authors:        Scott Mahlke, Santosh G. Abraham
//      Created:        December 1995
//      Description:    Cycle count estimator for schedule
//
/////////////////////////////////////////////////////////////////////////////

#include "iterators.h"
#include "op.h"
#include "operand.h"
#include "region_utilities.h"
#include "opcode_properties.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "el_io.h"
#include "el_reg_stats.h"
#include "el_stats_init.h"
#include "dbg.h"

//
//	Dynamic counter variables
//

static double sreg_tot_op_count;
static double sreg_spill_count[(int)NUM_REG_FILE+1];
static double sreg_callee_count[(int)NUM_REG_FILE+1];
static double sreg_caller_count[(int)NUM_REG_FILE+1];

//
//	Static counter variables
//

static double static_sreg_tot_op_count;
static double static_sreg_spill_count[(int)NUM_REG_FILE+1];
static double static_sreg_callee_count[(int)NUM_REG_FILE+1];
static double static_sreg_caller_count[(int)NUM_REG_FILE+1];



void El_reset_dynamic_reg_op_counters(void)
{
    int i;

    sreg_tot_op_count = 0.0;

    for (i=0; i<((int)NUM_REG_FILE+1); i++) {
        sreg_spill_count[i] = 0.0;
        sreg_callee_count[i] = 0.0;
        sreg_caller_count[i] = 0.0;
    }
}

void El_reset_static_reg_op_counters(void)
{
    int i;

    static_sreg_tot_op_count = 0.0;

    for (i=0; i<((int)NUM_REG_FILE+1); i++) {
        static_sreg_spill_count[i] = 0.0;
        static_sreg_callee_count[i] = 0.0;
        static_sreg_caller_count[i] = 0.0;
    }
}

void El_print_dynamic_reg_op_counters(void)
{
    int i;
    double tot_spill_frac, tot_caller_frac, tot_callee_frac, tot_regalloc_frac,
	tot_spill_count, tot_caller_count, tot_callee_count, tot_regalloc_count,
	spill_frac[(int)NUM_REG_FILE+1],
	caller_frac[(int)NUM_REG_FILE+1],
	callee_frac[(int)NUM_REG_FILE+1];
    double scale_spill_count, scale_caller_count, scale_callee_count;

    tot_spill_count = tot_caller_count = tot_callee_count = tot_regalloc_count = 0.0;
    for (i=0; i<((int)NUM_REG_FILE+1); i++) {
	tot_spill_count += sreg_spill_count[i];
	tot_caller_count += sreg_caller_count[i];
	tot_callee_count += sreg_callee_count[i];
    }
    tot_regalloc_count = tot_spill_count + tot_caller_count + tot_callee_count;

    /* Now we need to scale the individual spill/caller/callee counts by the
       addr calc counts stored in the last position in the arrays */
    scale_spill_count = tot_spill_count - sreg_spill_count[(int)NUM_REG_FILE];
    scale_caller_count = tot_caller_count - sreg_caller_count[(int)NUM_REG_FILE];
    scale_callee_count = tot_callee_count - sreg_callee_count[(int)NUM_REG_FILE];
    for (i=0; i<(int)NUM_REG_FILE; i++) {
	sreg_spill_count[i] += ((sreg_spill_count[i] / scale_spill_count) *
				sreg_spill_count[(int)NUM_REG_FILE]);
	sreg_caller_count[i] += ((sreg_caller_count[i] / scale_caller_count) *
				sreg_caller_count[(int)NUM_REG_FILE]);
	sreg_callee_count[i] += ((sreg_callee_count[i] / scale_callee_count) *
				sreg_callee_count[(int)NUM_REG_FILE]);
    }

    if (sreg_tot_op_count>0.0) {
        tot_spill_frac = tot_spill_count / sreg_tot_op_count;
        tot_caller_frac = tot_caller_count / sreg_tot_op_count;
        tot_callee_frac = tot_callee_count / sreg_tot_op_count;
        tot_regalloc_frac = tot_regalloc_count / sreg_tot_op_count;
        for (i=0; i<((int)NUM_REG_FILE+1); i++) {
	    spill_frac[i] = sreg_spill_count[i] / sreg_tot_op_count;
	    caller_frac[i] = sreg_caller_count[i] / sreg_tot_op_count;
	    callee_frac[i] = sreg_callee_count[i] / sreg_tot_op_count;
        }
    }
    else {
        tot_spill_frac = 0.0;
        tot_caller_frac = 0.0;
        tot_callee_frac = 0.0;
        tot_regalloc_frac = 0.0;
        for (i=0; i<((int)NUM_REG_FILE+1); i++) {
	    spill_frac[i] = 0.0;
	    caller_frac[i] = 0.0;
	    callee_frac[i] = 0.0;
        }
    }

    (*EL_STAT_STREAM) << "    Dynamic_regalloc_op_overhead:  "
                   << tot_regalloc_count << "   (" << tot_regalloc_frac << ")"
                   << endl;

    (*EL_STAT_STREAM) << "        Dynamic_spill_code:  "
		       << tot_spill_count << "   (" << tot_spill_frac << ")"
		   << endl;

    if (El_do_regalloc_file_overhead) {
        (*EL_STAT_STREAM) << "            Dynamic_spill_code_GPR:  "
		       << sreg_spill_count[(int)GPR]
		       << "   (" << spill_frac[(int)GPR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_spill_code_FPR:  "
		       << sreg_spill_count[(int)FPR]
		       << "   (" << spill_frac[(int)FPR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_spill_code_PR:  "
		       << sreg_spill_count[(int)PR]
		       << "   (" << spill_frac[(int)PR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_spill_code_BTR:  "
		       << sreg_spill_count[(int)BTR]
		       << "   (" << spill_frac[(int)BTR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_spill_code_CR:  "
		       << sreg_spill_count[(int)CR]
		       << "   (" << spill_frac[(int)CR] << ")"
		       << endl;
    }

    (*EL_STAT_STREAM) << "        Dynamic_caller_save:  "
   	           << tot_caller_count << "   (" << tot_caller_frac << ")"
		   << endl;

    if (El_do_regalloc_file_overhead) {
        (*EL_STAT_STREAM) << "            Dynamic_caller_save_GPR:  "
		       << sreg_caller_count[(int)GPR]
		       << "   (" << caller_frac[(int)GPR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_caller_save_FPR:  "
		       << sreg_caller_count[(int)FPR]
		       << "   (" << caller_frac[(int)FPR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_caller_save_PR:  "
		       << sreg_caller_count[(int)PR]
		       << "   (" << caller_frac[(int)PR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_caller_save_BTR:  "
		       << sreg_caller_count[(int)BTR]
		       << "   (" << caller_frac[(int)BTR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_caller_save_CR:  "
		       << sreg_caller_count[(int)CR]
		       << "   (" << caller_frac[(int)CR] << ")"
		       << endl;
    }

    (*EL_STAT_STREAM) << "        Dynamic_callee_save:  "
                   << tot_callee_count << "   (" << tot_callee_frac << ")"
                   << endl;

    if (El_do_regalloc_file_overhead) {
        (*EL_STAT_STREAM) << "            Dynamic_callee_save_GPR:  "
		       << sreg_callee_count[(int)GPR]
		       << "   (" << callee_frac[(int)GPR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_callee_save_FPR:  "
		       << sreg_callee_count[(int)FPR]
		       << "   (" << callee_frac[(int)FPR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_callee_save_PR:  "
		       << sreg_callee_count[(int)PR]
		       << "   (" << callee_frac[(int)PR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_callee_save_BTR:  "
		       << sreg_callee_count[(int)BTR]
		       << "   (" << callee_frac[(int)BTR] << ")"
		       << endl;
        (*EL_STAT_STREAM) << "            Dynamic_callee_save_CR:  "
		       << sreg_callee_count[(int)CR]
		       << "   (" << callee_frac[(int)CR] << ")"
		       << endl;
    }
}

void El_print_static_reg_op_counters(void)
{
    int i;
    double tot_spill_frac, tot_caller_frac, tot_callee_frac, tot_regalloc_frac,
        tot_spill_count, tot_caller_count, tot_callee_count, tot_regalloc_count,
        spill_frac[(int)NUM_REG_FILE+1],
        caller_frac[(int)NUM_REG_FILE+1],
        callee_frac[(int)NUM_REG_FILE+1];
    double scale_spill_count, scale_caller_count, scale_callee_count;

    tot_spill_count = tot_caller_count = tot_callee_count = tot_regalloc_count = 0.0;

    for (i=0; i<((int)NUM_REG_FILE+1); i++) {
        tot_spill_count += static_sreg_spill_count[i];
        tot_caller_count += static_sreg_caller_count[i];
        tot_callee_count += static_sreg_callee_count[i];
    }
    tot_regalloc_count = tot_spill_count + tot_caller_count + tot_callee_count;

    /* Now we need to scale the individual spill/caller/callee counts by the
       addr calc counts stored in the last position in the arrays */
    scale_spill_count = tot_spill_count -
				static_sreg_spill_count[(int)NUM_REG_FILE];
    scale_caller_count = tot_caller_count -
				static_sreg_caller_count[(int)NUM_REG_FILE];
    scale_callee_count = tot_callee_count -
				static_sreg_callee_count[(int)NUM_REG_FILE];
    for (i=0; i<(int)NUM_REG_FILE; i++) {
        static_sreg_spill_count[i] += ((static_sreg_spill_count[i] /
				scale_spill_count) *
                               	static_sreg_spill_count[(int)NUM_REG_FILE]);
        static_sreg_caller_count[i] += ((static_sreg_caller_count[i] /
				scale_caller_count) *
                               	static_sreg_caller_count[(int)NUM_REG_FILE]);
        static_sreg_callee_count[i] += ((static_sreg_callee_count[i] /
				scale_callee_count) *
                               	static_sreg_callee_count[(int)NUM_REG_FILE]);
    }

    if (static_sreg_tot_op_count>0.0) {
        tot_spill_frac = tot_spill_count / static_sreg_tot_op_count;
        tot_caller_frac = tot_caller_count / static_sreg_tot_op_count;
        tot_callee_frac = tot_callee_count / static_sreg_tot_op_count;
        tot_regalloc_frac = tot_regalloc_count / static_sreg_tot_op_count;
	for (i=0; i<((int)NUM_REG_FILE+1); i++) {
            spill_frac[i] = static_sreg_spill_count[i] / static_sreg_tot_op_count;
            caller_frac[i] = static_sreg_caller_count[i] / static_sreg_tot_op_count;
            callee_frac[i] = static_sreg_callee_count[i] / static_sreg_tot_op_count;
        }
    }
    else {
        tot_spill_frac = 0.0;
        tot_caller_frac = 0.0;
        tot_callee_frac = 0.0;
        tot_regalloc_frac = 0.0;
	for (i=0; i<((int)NUM_REG_FILE+1); i++) {
            spill_frac[i] = 0.0;
            caller_frac[i] = 0.0;
            callee_frac[i] = 0.0;
        }
    }

    (*EL_STAT_STREAM) << "    Static_regalloc_op_overhead:  "
                   << tot_regalloc_count << "   (" << tot_regalloc_frac << ")"
                   << endl;

    (*EL_STAT_STREAM) << "        Static_spill_code:  "
                       << tot_spill_count << "   (" << tot_spill_frac << ")"
                   << endl;

    if (El_do_regalloc_file_overhead) {
        (*EL_STAT_STREAM) << "            Static_spill_code_GPR:  "
                       << static_sreg_spill_count[(int)GPR]
                       << "   (" << spill_frac[(int)GPR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_spill_code_FPR:  "
                       << static_sreg_spill_count[(int)FPR]
                       << "   (" << spill_frac[(int)FPR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_spill_code_PR:  "
                       << static_sreg_spill_count[(int)PR]
                       << "   (" << spill_frac[(int)PR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_spill_code_BTR:  "
                       << static_sreg_spill_count[(int)BTR]
                       << "   (" << spill_frac[(int)BTR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_spill_code_CR:  "
                       << static_sreg_spill_count[(int)CR]
                       << "   (" << spill_frac[(int)CR] << ")"
                       << endl;
    }

    (*EL_STAT_STREAM) << "        Static_caller_save:  "
                   << tot_caller_count << "   (" << tot_caller_frac << ")"
                   << endl;

    if (El_do_regalloc_file_overhead) {
        (*EL_STAT_STREAM) << "            Static_caller_save_GPR:  "
                       << static_sreg_caller_count[(int)GPR]
                       << "   (" << caller_frac[(int)GPR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_caller_save_FPR:  "
                       << static_sreg_caller_count[(int)FPR]
                       << "   (" << caller_frac[(int)FPR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_caller_save_PR:  "
                       << static_sreg_caller_count[(int)PR]
                       << "   (" << caller_frac[(int)PR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_caller_save_BTR:  "
                       << static_sreg_caller_count[(int)BTR]
                       << "   (" << caller_frac[(int)BTR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_caller_save_CR:  "
                       << static_sreg_caller_count[(int)CR]
                       << "   (" << caller_frac[(int)CR] << ")"
                       << endl;
    }

    (*EL_STAT_STREAM) << "        Static_callee_save:  "
                   << tot_callee_count << "   (" << tot_callee_frac << ")"
                   << endl;

    if (El_do_regalloc_file_overhead) {
        (*EL_STAT_STREAM) << "            Static_callee_save_GPR:  "
                       << static_sreg_callee_count[(int)GPR]
                       << "   (" << callee_frac[(int)GPR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_callee_save_FPR:  "
                       << static_sreg_callee_count[(int)FPR]
                       << "   (" << callee_frac[(int)FPR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_callee_save_PR:  "
                       << static_sreg_callee_count[(int)PR]
                       << "   (" << callee_frac[(int)PR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_callee_save_BTR:  "
                       << static_sreg_callee_count[(int)BTR]
                       << "   (" << callee_frac[(int)BTR] << ")"
                       << endl;
        (*EL_STAT_STREAM) << "            Static_callee_save_CR:  "
                       << static_sreg_callee_count[(int)CR]
                       << "   (" << callee_frac[(int)CR] << ")"
                       << endl;
    }
}

void El_compute_dynamic_regalloc_overhead(Op *op, double exec_count)
{
    //Lcode_attribute_map *lc_attr;
    Reg_file rfile = UNDEF_VRF;
    Operand oper;
    int rfile_index;

    if (op->flag(EL_OPER_SPILL_CODE) == false)
	return;

    if (is_load(op) || is_pred_load_all(op)) {
	oper = op->dest(DEST1);
	assert(oper.is_reg() || oper.is_macro_reg());
	rfile = oper.file_type();
    }
    else if (is_store(op) || is_pred_store_all(op)) {
        if (is_base_plus_offset_store(op)) {
          oper = op->src(SRC3);
          assert(oper.is_reg() || oper.is_macro_reg());
          rfile = oper.file_type();
        }
        else {
          oper = op->src(SRC2);
          assert(oper.is_reg() || oper.is_macro_reg());
          rfile = oper.file_type();
        }
    }
    else if (is_ialu(op)) {
	rfile = NUM_REG_FILE;
    }
    else {
	El_punt("El_compute_dynamic_regalloc_overhead: op %d unknown regalloc op",
			op->id());
    }
    rfile_index = ((int) rfile);
    
    /* SAM, 3-97, changed the format of the attribute used by the allocator,
       used to use the regalloc1 attr*/
    /*
    lc_attr = get_lcode_attributes(op);
    if (lc_attr->is_bound("sreg_spill")) {
	sreg_spill_count[rfile_index] += exec_count;
    }
    else if (lc_attr->is_bound("sreg_caller")) {
	sreg_caller_count[rfile_index] += exec_count;
    }
    else if (lc_attr->is_bound("sreg_callee")) {
	sreg_callee_count[rfile_index] += exec_count;
    }
    */

    /* gdasika Feb 05 - using region flags now instead of lcode attribs */
    if (op->flag(EL_OPER_SPILL_CODE)) {
        static_sreg_spill_count[rfile_index] += 1;
    }
    else if (op->flag(EL_OPER_CALLER_SAVE)) {
        static_sreg_caller_count[rfile_index] += 1;
    }
    else if (op->flag(EL_OPER_CALLEE_SAVE)) {
        static_sreg_callee_count[rfile_index] += 1;
    }
    else {
	El_punt("El_compute_dynamic_regalloc_overhead: op %d missing regalloc attr",
			op->id());
    }
}

void El_compute_static_regalloc_overhead(Op *op)
{
    //Lcode_attribute_map *lc_attr;
    Reg_file rfile = UNDEF_VRF;
    Operand oper;
    int rfile_index;

    if (op->flag(EL_OPER_SPILL_CODE) == false)
        return;

    if (is_load(op) || is_pred_load_all(op)) {
        oper = op->dest(DEST1);
	assert(oper.is_reg() || oper.is_macro_reg());
        rfile = oper.file_type();
    }
    else if (is_store(op) || is_pred_store_all(op)) {
        if (is_base_plus_offset_store(op)) {
          oper = op->src(SRC3);
          assert(oper.is_reg() || oper.is_macro_reg());
          rfile = oper.file_type();
        }
        else {
          oper = op->src(SRC2);
          assert(oper.is_reg() || oper.is_macro_reg());
          rfile = oper.file_type();
        }
    }   

    /*
    else if (is_store(op) || is_pred_store_all(op)) {
	oper = op->src(SRC2);
        assert(oper.is_reg() || oper.is_macro_reg());
        rfile = oper.file_type();
    }
    */
    else if (is_ialu(op)) {
        rfile = NUM_REG_FILE;
    }
    else {
        El_punt("El_compute_dynamic_regalloc_overhead: op %d unknown regalloc op",
                        op->id());
    }
    rfile_index = ((int) rfile);

    /* SAM, 3-97, changed the format of the attribute used by the allocator,
       used to use the regalloc1 attr*/
    /*
    lc_attr = get_lcode_attributes(op);
    if (lc_attr->is_bound("sreg_spill")) {
        static_sreg_spill_count[rfile_index] += 1;
    }
    else if (lc_attr->is_bound("sreg_caller")) {
        static_sreg_caller_count[rfile_index] += 1;
    }
    else if (lc_attr->is_bound("sreg_callee")) {
        static_sreg_callee_count[rfile_index] += 1;
    }
    */

    /* gdasika Feb 05 - using region flags now instead of lcode attribs */
    if (op->flag(EL_OPER_SPILL_CODE)) {
        static_sreg_spill_count[rfile_index] += 1;
    }
    else if (op->flag(EL_OPER_CALLER_SAVE)) {
        static_sreg_caller_count[rfile_index] += 1;
    }
    else if (op->flag(EL_OPER_CALLEE_SAVE)) {
        static_sreg_callee_count[rfile_index] += 1;
    }
    else {
        El_punt("El_compute_static_regalloc_overhead: op %d missing regalloc attr",
                        op->id());
    }
}

void El_compute_dynamic_reg_op_count (Compound_region *reg)
{
  double freq, cycle_exit_freq;
  int last_sched_time, curr_sched_time;

  if (reg->flag(EL_REGION_SCHEDULED)) { // this region is scheduled. compute the
         // ops necessary to execute it.
    assert(reg->is_loopbody() || reg->is_hb() || reg->is_bb());

    if (reg->is_loopbody()) { // modulo-scheduled loopbody
      freq = reg->weight;
      int esc = get_msched_attr(reg)->esc;
      Region_entry_edges enop (reg);
      Control_flow_freq* lp_entry = get_control_flow_freq(*enop);
      enop++;
      assert(enop == 0); // loop has only one loop entry edge
      Edge* edge = (((LoopBody*)reg)->controlFlowBackEdges())->head();
      Control_flow_freq* lp_bedge = get_control_flow_freq(edge);
      assert (lp_entry->freq == freq);
      // Is this correct??
      double exec_count = lp_entry->freq*(esc+1) + lp_bedge->freq;
      for (Region_all_ops oiter (reg); oiter != 0; oiter++) {
        Op* op = *oiter;

        // Increment op counters by freq
        if (is_pseudo(op))
          continue;

        sreg_tot_op_count += exec_count;

        /* Regalloc overhead op stats */
        El_compute_dynamic_regalloc_overhead(op, exec_count);
      }
    }

    else { // hb or bb
      freq = reg->weight;
      last_sched_time = -99;
      cycle_exit_freq = 0.0;
      for (Region_ops_C0_order oiter (reg); oiter != 0; oiter++) {
        Op* op = *oiter;
        curr_sched_time = op->sched_time();

        // Next cycle, subtract off exit freq for the last cycle from freq
        if (curr_sched_time != last_sched_time) {
          freq -= cycle_exit_freq;
          if (freq<0)	// may go negative due to errors with prof weights
            freq = 0.0;
          cycle_exit_freq = 0.0;
          last_sched_time = curr_sched_time;
        }

        if (is_branch(op)) {
          for (Op_outedges ei (op); ei != 0; ei++) {
            Edge *edge = *ei;
            if (! reg->is_exit_edge(edge))
              continue;
            Control_flow_freq *cfreq = get_control_flow_freq (edge);
            cycle_exit_freq += cfreq->freq;
          }
        }

        // Increment op counters by freq
        if (is_pseudo(op))
          continue;

        sreg_tot_op_count += freq;

        /* Regalloc overhead op stats */
        El_compute_dynamic_regalloc_overhead(op, freq);
      }
    }
  }

  else { // compute op count for all regions "underneath" this region.
    for (Region_subregions sub (reg); sub != 0; sub++) {
      Compound_region* subreg = (Compound_region* ) (*sub);
      El_compute_dynamic_reg_op_count (subreg);
    }
  }
}

void El_compute_static_reg_op_count(Compound_region *reg)
{
    Op *op;

    for (Region_all_ops oiter (reg); oiter != 0; oiter++) {
	op = *oiter;

	if (is_pseudo(op))
          continue;

	static_sreg_tot_op_count += 1;

        /* Regalloc overhead op stats */
        El_compute_static_regalloc_overhead(op);
    }
}


/*=========================================================================*/
/*
 *	Externally visible routine
 */
/*=========================================================================*/


void El_compute_reg_op_stats(Procedure* f)
{
  El_reset_dynamic_reg_op_counters();
  El_reset_static_reg_op_counters();

  El_compute_dynamic_reg_op_count(f);
  El_compute_static_reg_op_count(f);

  El_print_dynamic_reg_op_counters();
  El_print_static_reg_op_counters();
} 
