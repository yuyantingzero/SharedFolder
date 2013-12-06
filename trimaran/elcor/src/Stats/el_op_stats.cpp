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
//      File:           el_op_stats.cpp
//      Authors:        Scott Mahlke, Santosh G. Abraham
//      Created:        December 1995
//      Description:    Operation count statistics
//
/////////////////////////////////////////////////////////////////////////////

#include "iterators.h"
#include "op.h"
#include "region_utilities.h"
#include "opcode_properties.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "el_io.h"
#include "el_stats_init.h"
#include "el_sched_stats.h"
#include "intf.h"

extern double swap_follows_move;
extern double swap_follows_tk_condbranch;
extern double swap_follows_ft_condbranch;


//
//	Dynamic counter variables
//

static double tot_op_count;
static double branch_count;
static double load_count;
static double store_count;
static double ialu_count;
static double falu_count;
static double pbr_count;
static double icmpp_count;
static double fcmpp_count;
static double pclear_count;

static double dyn_spill_count;
static double dyn_callee_save;

// For vector ops.  Whoever actually wants to use these vector stats should
// probably break this out into subcategories.  -KF 1/2007
static double dyn_vector_count;

//
//	Static counter variables
//

static double static_tot_op_count;
static double static_branch_count;
static double static_load_count;
static double static_store_count;
static double static_ialu_count;
static double static_falu_count;
static double static_pbr_count;
static double static_icmpp_count;
static double static_fcmpp_count;
static double static_pclear_count;

static double static_spill_count;
static double static_callee_save;
static double static_vector_count;

static bool check_gpr(Op *op)
{
    for (Op_explicit_operands oprndi(op); oprndi!=0; oprndi++) {
      Operand opr = *oprndi;
      if (opr.is_reg() || opr.is_macro_reg()) {
        if (opr.file_type() != GPR)
            return false;
      }
    }

    return true;
}


static void El_reset_dynamic_op_counters(void)
{
    tot_op_count = 0.0;
    branch_count = 0.0;
    load_count = 0.0;
    store_count = 0.0;
    ialu_count = 0.0;
    falu_count = 0.0;
    pbr_count = 0.0;
    icmpp_count = 0.0;
    fcmpp_count = 0.0;
    pclear_count = 0.0;

    dyn_spill_count = 0.0;
    dyn_callee_save = 0.0;
    dyn_vector_count = 0.0;
}

static void El_reset_static_op_counters(void)
{
    static_tot_op_count = 0.0;
    static_branch_count = 0.0;
    static_load_count = 0.0;
    static_store_count = 0.0;
    static_ialu_count = 0.0;
    static_falu_count = 0.0;
    static_pbr_count = 0.0;
    static_icmpp_count = 0.0;
    static_fcmpp_count = 0.0;
    static_pclear_count = 0.0;

    static_spill_count = 0.0;
    static_callee_save = 0.0;
    static_vector_count = 0.0;
}

static void El_print_dynamic_op_counters(void)
{
    double branch_frac, load_frac, store_frac, ialu_frac, falu_frac, icmpp_frac,
           fcmpp_frac, pbr_frac, opc, pclear_frac;
    
    double dyn_spillfrac, dyn_calleesavefrac, dyn_vectorfrac;

    if (tot_op_count>0.0) {
      branch_frac = branch_count / tot_op_count;
      load_frac = load_count / tot_op_count;
      store_frac = store_count / tot_op_count;
      ialu_frac = ialu_count / tot_op_count;
      falu_frac = falu_count / tot_op_count;
      icmpp_frac = icmpp_count / tot_op_count;
      fcmpp_frac = fcmpp_count / tot_op_count;
      pclear_frac = pclear_count / tot_op_count;
      pbr_frac = pbr_count / tot_op_count;

      dyn_spillfrac = dyn_spill_count / tot_op_count;
      dyn_calleesavefrac = dyn_callee_save / tot_op_count;
      dyn_vectorfrac = dyn_vector_count / tot_op_count;
    }
    else {
      branch_frac = 0.0;
      load_frac = 0.0;
      store_frac = 0.0;
      ialu_frac = 0.0;
      falu_frac = 0.0;
      icmpp_frac = 0.0;
      fcmpp_frac = 0.0;
      pclear_frac = 0.0;
      pbr_frac = 0.0;

      dyn_spillfrac = 0.0;
      dyn_calleesavefrac = 0.0;
      dyn_vectorfrac = 0.0;
    }
    if (El_tot_cycle_count > 0.0) {
      opc = tot_op_count / El_tot_cycle_count;
    }
    else {
      opc = 0.0;
    }

    (*EL_STAT_STREAM) << "    Dynamic_total_operations:  " << tot_op_count << endl;
    (*EL_STAT_STREAM) << "        Dynamic_branch:  " << branch_count
                   << "   (" << branch_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_load:  " << load_count
                   << "   (" << load_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_store:  " << store_count
                   << "   (" << store_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_ialu:  " << ialu_count
                   << "   (" << ialu_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_falu:  " << falu_count
                   << "   (" << falu_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_icmpp:  " << icmpp_count
                   << "   (" << icmpp_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_fcmpp:  " << fcmpp_count
                   << "   (" << fcmpp_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_pclear:  " << pclear_count
                   << "   (" << pclear_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_pbr:  " << pbr_count
                   << "   (" << pbr_frac << ")" << endl;

    (*EL_STAT_STREAM) << "        Dynamic_spill_code:  " << dyn_spill_count
                   << "   (" << dyn_spillfrac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_callee_save:  " << dyn_callee_save
                   << "   (" << dyn_calleesavefrac << ")" << endl;
    (*EL_STAT_STREAM) << "        Dynamic_vector_ops:  " << dyn_vector_count
                   << "   (" << dyn_vectorfrac << ")" << endl;

    (*EL_STAT_STREAM) << "    Dynamic_average_issued_ops/cycle:  "
		   << opc << endl;
}

static void El_print_static_op_counters(void)
{
    double branch_frac, load_frac, store_frac, ialu_frac, falu_frac, icmpp_frac,
           fcmpp_frac, pbr_frac, pclear_frac;

    double spillfrac, calleesavefrac, vectorfrac;

    if (static_tot_op_count>0.0) {
      branch_frac = static_branch_count / static_tot_op_count;
      load_frac = static_load_count / static_tot_op_count;
      store_frac = static_store_count / static_tot_op_count;
      ialu_frac = static_ialu_count / static_tot_op_count;
      falu_frac = static_falu_count / static_tot_op_count;
      icmpp_frac = static_icmpp_count / static_tot_op_count;
      fcmpp_frac = static_fcmpp_count / static_tot_op_count;
      pclear_frac = static_pclear_count / static_tot_op_count;
      pbr_frac = static_pbr_count / static_tot_op_count;
    
      spillfrac = static_spill_count / static_tot_op_count;
      calleesavefrac = static_callee_save / static_tot_op_count;
      vectorfrac = static_vector_count / static_tot_op_count;
    }
    else {
      branch_frac = 0.0;
      load_frac = 0.0;
      store_frac = 0.0;
      ialu_frac = 0.0;
      falu_frac = 0.0;
      icmpp_frac = 0.0;
      fcmpp_frac = 0.0;
      pclear_frac = 0.0;
      pbr_frac = 0.0;
      
      spillfrac = 0.0;
      calleesavefrac = 0.0;
      vectorfrac = 0.0;
    }
    (*EL_STAT_STREAM) << "    Static_total_operations:  "
                   << static_tot_op_count << endl;
    (*EL_STAT_STREAM) << "        Static_branch:  " << static_branch_count
                   << "   (" << branch_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_load:  " << static_load_count
                   << "   (" << load_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_store:  " << static_store_count
                   << "   (" << store_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_ialu:  " << static_ialu_count
                   << "   (" << ialu_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_falu:  " << static_falu_count
                   << "   (" << falu_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_icmpp:  " << static_icmpp_count
                   << "   (" << icmpp_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_fcmpp:  " << static_fcmpp_count
                   << "   (" << fcmpp_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_pclear:  " << static_pclear_count
                   << "   (" << pclear_frac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_pbr:  " << static_pbr_count
                   << "   (" << pbr_frac << ")" << endl;

    (*EL_STAT_STREAM) << "        Static_spill_code:  " << static_spill_count
                   << "   (" << spillfrac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_callee_save:  " << static_callee_save
                   << "   (" << calleesavefrac << ")" << endl;
    (*EL_STAT_STREAM) << "        Static_vector_ops:  " << static_vector_count
                   << "   (" << vectorfrac << ")" << endl;
}

void increment_dynamic_op_counters(Op *op, double val)
{
    if (is_pseudo(op) || is_no_op(op))
	return;
    if (is_custom(op))
	return;

    tot_op_count += val;

    if (is_control_switch(op)) {
	branch_count += val;
    }
    else if (is_load(op) || is_pred_load_all(op) || is_restore(op)) {
	load_count += val;
    }
    else if (is_store(op) || is_pred_store_all(op) || is_save(op)) {
	store_count += val;
    }
    else if (is_pbr(op)) {
	pbr_count += val;
    }
    else if (is_icmpp(op)) {
	icmpp_count += val;
    }
    else if (is_fcmpp(op)) {
	fcmpp_count += val;
    }
    else if (is_predicate_clear(op) || is_predicate_set(op) ||
		 is_clear_all_rotating(op) ) {
	pclear_count += val;
    }
    else if (is_ialu(op)) {
        ialu_count += val;
    }
    else if (is_falu(op)) {
	falu_count += val;

    } else if (is_vector(op)) {
        dyn_vector_count += val;
    } else {
	El_punt("increment_op_counters: unknown op type for op %d (%s)",
		// op->id(), (char *) el_opcode_to_string_map.value(op->opcode()));
		op->id(), get_mdes_opcode_string(op));
    }

    if (op->flag(EL_OPER_SPILL_CODE) && !op->flag(EL_OPER_CALLEE_SAVE)) {
      dyn_spill_count += val;
    } 
    if (op->flag(EL_OPER_CALLEE_SAVE)) {
      dyn_callee_save += val;
    }
}

double compute_dynamic_op_count (Compound_region *reg)
{
  double local_op_count = 0;
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
	local_op_count += exec_count;
	increment_dynamic_op_counters(op, exec_count);
      }
    }

    else { // Scheduled hb or bb
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

	local_op_count += freq;;
	increment_dynamic_op_counters(op, freq);
      }
    }
  }

  /* Unscheduled HB/BB */
  else if (reg->is_hb() || reg->is_bb()) {
    freq = reg->weight;
    cycle_exit_freq = 0.0;
    for (Region_ops_C0_order oiter (reg); oiter != 0; oiter++) {
      Op *op = *oiter;

      if (cycle_exit_freq > 0.0) {
        freq -= cycle_exit_freq;
        if (freq < 0.0)   // may go negative due to errors with prof weights
          freq = 0.0;
        cycle_exit_freq = 0.0;
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

      local_op_count += freq;;
      increment_dynamic_op_counters(op, freq);
    }
  }

  else { // compute op count for all regions "underneath" this region.
    for (Region_subregions sub (reg); sub != 0; sub++) {
      Compound_region* subreg = (Compound_region* ) (*sub);
      local_op_count += compute_dynamic_op_count (subreg);
    }
  }

  return (local_op_count);
}

double compute_static_op_count(Compound_region *reg)
{
    double local_op_count;
    Op *op;

    local_op_count = 0;
    for (Region_all_ops oiter (reg); oiter != 0; oiter++) {
	op = *oiter;

	if (is_pseudo(op) || is_no_op(op))
          continue;
	if (is_custom(op))
          continue;

	local_op_count += 1;
	static_tot_op_count += 1;
	if (is_control_switch(op)) {
          static_branch_count += 1;
        }
        else if (is_load(op) || is_pred_load_all(op) || is_restore(op)) {
          static_load_count += 1;
        }
        else if (is_store(op) || is_pred_store_all(op) || is_save(op)) {
          static_store_count += 1;
        }
        else if (is_pbr(op)) {
          static_pbr_count += 1;
        }
        else if (is_icmpp(op)) {
          static_icmpp_count += 1;
        }
        else if (is_fcmpp(op)) {
          static_fcmpp_count += 1;
        }
	else if (is_predicate_clear(op) || is_predicate_set(op) ||
		 is_clear_all_rotating(op) ) {
          static_pclear_count += 1;
        }
        else if (is_ialu(op)) {
          static_ialu_count += 1;
        }
        else if (is_falu(op)) {
          static_falu_count += 1;
        } else if (is_vector(op)) {
          static_vector_count += 1;
        } else {
          El_punt("compute_static_op_count: unknown op type for op %d (%s)",
	      // op->id(), (char *) el_opcode_to_string_map.value(op->opcode()));
	      op->id(), get_mdes_opcode_string(op));
        }

        if (op->flag(EL_OPER_SPILL_CODE) && !op->flag(EL_OPER_CALLEE_SAVE) && check_gpr(op)) {
          static_spill_count +=1;
        }
        if (op->flag(EL_OPER_CALLEE_SAVE) && check_gpr(op)) {
          static_callee_save +=1;
        }

    }

    return (local_op_count);
}


/*=========================================================================*/
/*
 *	Externally visible routines for op stat computation
 */
/*=========================================================================*/

void El_compute_op_stats(Procedure* f)
{
  El_reset_dynamic_op_counters();
  El_reset_static_op_counters();
  (void) compute_dynamic_op_count(f);
  (void) compute_static_op_count(f);
  El_print_dynamic_op_counters();
  El_print_static_op_counters();
} 

