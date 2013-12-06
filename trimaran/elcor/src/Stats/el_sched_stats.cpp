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
//      File:           el_sched_stats.cpp
//      Authors:        Scott Mahlke, Santosh G. Abraham
//      Created:        December 1995
//      Description:    Cycle count estimator for schedule
//
/////////////////////////////////////////////////////////////////////////////

#include "iterators.h"
#include "op.h"
#include "region_utilities.h"
#include "opcode_properties.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "meld_pad.h"
#include "meld_propagate.h"
#include "el_io.h"
#include "el_stats_init.h"
#include "el_sched_stats.h"
#include "intf.h"
#include "edge_utilities.h"
#include "el_cache_init.h"
#include "ls_perf.h"
#include "ls_cacheopt.h"

//
//	Dynamic counter variables
//

double El_tot_cycle_count;
static double scalar_cycle_count;
static double loop_cycle_count;

static double dcache_miss_cycles;
static double branch_mpred_cycles;  // Unrelated to the mispredictions var below

// generated only when MELD_COMPARE is on
static double ub_interlock_cycles;
static double superscalar_stall_cycles;
static double mispredictions;
static double ss_stall_cycles_mpred;
static double self_stall_cycles;
static double rts_stall_cycles;


static void El_reset_dynamic_cycle_counters(void)
{
    El_tot_cycle_count = 0.0;
    scalar_cycle_count = 0.0;
    loop_cycle_count = 0.0;
    dcache_miss_cycles = 0.0;
    branch_mpred_cycles = 0.0;
    ub_interlock_cycles = 0.0;
    superscalar_stall_cycles = 0.0;
    mispredictions = 0.0;
    ss_stall_cycles_mpred = 0.0;
    self_stall_cycles = 0.0;
    rts_stall_cycles  = 0.0;
}

static void show_block_types(Compound_region* reg)
{
  if (reg->is_bb()) {
    assert (!region_has_predicated_ops(reg));
    (*EL_STAT_STREAM) << "    block_type\t" << "bb " << reg->id() << endl;
  } else if (reg->is_hb()) {
    (*EL_STAT_STREAM) << "    block_type\t"
		   << (region_has_predicated_ops(reg) ? "hb " : "sb ")
		   << reg->id() << endl;
    
  } else {
    for (Region_subregions sub (reg); sub != 0; sub++) {
      Compound_region* subreg = (Compound_region* ) (*sub);
      show_block_types (subreg);
    }
  }
}

/*
 *	Just assume the branch is predicted in its predominant direction.
 *	Multiway branches are always predicted wrong.
 */
static double El_calculate_branch_mispredict_penalty(Op *branch)
{
    Edge *ft_edge, *tk_edge;
    Control_flow_freq *ft_cfreq, *tk_cfreq;
    double total = 0.0;

    if (is_dummy_branch(branch))
	return (0.0);

    // had to put this special case in when bypassing Lsuperscalar.
    // ntclark 8/29/03
    if (is_unconditional_branch(branch))
        return (0.0);

    if (branch->flag(EL_OPER_TABLE_JUMP)) {
	for (Op_outedges edge_i(branch, CONTROL0_OUTEDGES); edge_i!=0; edge_i++) {
	    tk_edge = *edge_i;
	    tk_cfreq = get_control_flow_freq(tk_edge);
	    assert(tk_cfreq != NULL);
	    total += tk_cfreq->freq;
	}
	return (total * (double) El_branch_mispredict_penalty);
    }

    ft_edge = get_outgoing_CONTROL0_fall_through_edge(branch);
    if (ft_edge == NULL)
	return (0.0);

    ft_cfreq = get_control_flow_freq(ft_edge);
    tk_edge = get_outgoing_CONTROL0_non_fall_through_edge(branch);
//    assert (tk_edge != NULL);
    if(tk_edge == NULL)	//for unconditional branch to the next op, the tk_edge could be NULL. hongtaoz 12/16/2003
	return (0.0);
    tk_cfreq = get_control_flow_freq(tk_edge);

    if (tk_cfreq->freq > ft_cfreq->freq)
	return (ft_cfreq->freq * (double) El_branch_mispredict_penalty);
    else
	return (tk_cfreq->freq * (double) El_branch_mispredict_penalty);
}

static void El_print_dynamic_cycle_counters(void)
{
    double scalar_frac, loop_frac, compute_frac, dcache_frac, bmpred_frac;
    double grand_total_cycles;

    grand_total_cycles = El_tot_cycle_count + dcache_miss_cycles + branch_mpred_cycles;
    if (grand_total_cycles>0) { 
      loop_frac = loop_cycle_count / grand_total_cycles;
      scalar_frac = scalar_cycle_count / grand_total_cycles;
      compute_frac = El_tot_cycle_count / grand_total_cycles;
      dcache_frac = dcache_miss_cycles / grand_total_cycles;
      bmpred_frac = branch_mpred_cycles / grand_total_cycles;
    }
    else {
      loop_frac = 0.0;
      scalar_frac = 0.0;
      compute_frac = 0.0;
      dcache_frac = 0.0;
      bmpred_frac = 0.0;
    }

    (*EL_STAT_STREAM) << "    Dynamic_total_cycles:  "
		   << grand_total_cycles
		   << endl;
    (*EL_STAT_STREAM) << "        Dynamic_total_compute_cycles:  "
                          << El_tot_cycle_count
                          << "   (" << compute_frac << ")" << endl;
    (*EL_STAT_STREAM) << "            Dynamic_scalar_compute_cycles:  "
                          << scalar_cycle_count
                          << "   (" << scalar_frac << ")" << endl;
    (*EL_STAT_STREAM) << "            Dynamic_loop_compute_cycles:  "
                          << loop_cycle_count
                          << "   (" << loop_frac << ")" << endl;

    if (El_cache_load_sched != NONE) {
      (*EL_STAT_STREAM) << "        Dynamic_dcache_stall_cycles:  "
                              << dcache_miss_cycles
                              << "   (" << dcache_frac << ")" << endl;
    }

    (*EL_STAT_STREAM) << "        Dynamic_bmpred_stall_cycles:  "
                            << branch_mpred_cycles
                            << "   (" << bmpred_frac << ")" << endl;

    if (MELD_COMPARE) {
      (*EL_STAT_STREAM) << "        UB_interlock_cycles:  "
                              << ub_interlock_cycles << endl;
      (*EL_STAT_STREAM) << "        Superscalar_stall_cycles:  "
                              << superscalar_stall_cycles << endl;
      (*EL_STAT_STREAM) << "        Dynamic mispredictions:  "
                              << mispredictions << endl;
      (*EL_STAT_STREAM) << "        Superscalar_stall_cycles_along_mpred_entries:  "
                              << ss_stall_cycles_mpred << endl;
      (*EL_STAT_STREAM) << "        Self_stall_cycles:  "
                              << self_stall_cycles << endl;
      (*EL_STAT_STREAM) << "        RTS_stall_cycles:  "
                              << rts_stall_cycles << endl;
    }

}

double compute_dynamic_cycle_count (Compound_region *reg)
{
  double local_cycle_count = 0;
  double val, weighted_sched_length, pe_weight, pe_ratio;
  int pe_sched_length;

  if (reg->flag(EL_REGION_SCHEDULED)) { // this region is scheduled.  compute the
         // cycles necessary to execute it.
    assert(reg->is_loopbody() || reg->is_hb() || reg->is_bb());
    if (reg->is_loopbody()) { // modulo-scheduled loopbody
      double freq = reg->weight;
      Msched_attr* msattr = get_msched_attr(reg);
      int ii = msattr->ii;
      int esc = msattr->esc;
      Region_entry_edges enop (reg);
      Control_flow_freq* lp_entry = get_control_flow_freq(*enop);
      enop++;
      assert(enop == 0); // loop has only one loop entry edge
      Edge* edge = (((LoopBody*)reg)->controlFlowBackEdges())->head();
      Control_flow_freq* lp_bedge = get_control_flow_freq(edge);
      assert (lp_entry->freq == freq);
      val = ((lp_entry->freq)*ii*(esc+1)) + ((lp_bedge->freq)*ii);
      local_cycle_count += val;
      El_tot_cycle_count += val;
      loop_cycle_count += val;
      branch_mpred_cycles += El_calculate_branch_mispredict_penalty(edge->src());
      if (El_do_block_cycle_count) {
	if (reg->weight > 0.0) {
	  weighted_sched_length = val / reg->weight;
	  pe_ratio = (ELCOR_MAX(lp_bedge->freq, lp_entry->freq)) / reg->weight;
	}
        else {
          weighted_sched_length = 0.0;
	  pe_ratio = 0.0;
          // This is a bit nonstandard, but for 0 profile weight, assume the 
          // last exit is taken 100% of the time.
          weighted_sched_length = (ii*(esc+1));
	  pe_ratio = 1.0;
	}
	(*EL_STAT_STREAM) << "    region_cycles\t"
		           << "lp "
		           << reg->id() << '\t'
		           << "wsl " << weighted_sched_length << "  "
		           << "pesl " << (ii*(esc+1)) << "  "
			   << "per " << pe_ratio << "  "
		           << "wgt " << reg->weight
		           << endl;
      }
    } else { // hb or bb      
      double freq = reg->weight;
      int entry_sched_time, exit_sched_time;
      Region_entry_ops entry(reg);
      Op* entry_op = *entry;    // get the top entry op
      entry_sched_time = entry_op->sched_time();
      weighted_sched_length = 0.0;

      if (MELD_COMPARE) {
	double reg_superscalar_stall_cycles = 0.0;
	double reg_mispredictions = 0.0;
	double reg_ss_stall_cycles_mpred = 0.0;
	int reg_self_stalls = 0;
	ub_interlock_cycles += freq
        * ub_interlock (reg, reg_superscalar_stall_cycles, reg_mispredictions,
			reg_ss_stall_cycles_mpred, reg_self_stalls);
	superscalar_stall_cycles += reg_superscalar_stall_cycles;
	mispredictions += reg_mispredictions;
	ss_stall_cycles_mpred += reg_ss_stall_cycles_mpred;
        self_stall_cycles += freq * reg_self_stalls;
	// determine if blk is the rts_block
	// first find the procedure region and then the exit op
	bool rts_block = false;
	Compound_region* next_ancestor = reg->parent();
	while (!next_ancestor->is_procedure()) {
	  Compound_region* ancestor = next_ancestor;
	  next_ancestor = ancestor->parent();
	}
	assert (next_ancestor);
	Region_exit_ops exi (next_ancestor);
	Op* func_exit = *(exi);
	exi++; assert (exi == 0);
	// now find if exit op belongs to this block
	next_ancestor = func_exit->parent();
	while (next_ancestor->is_bb() || next_ancestor->is_hb()) {
	  if (next_ancestor->id() == reg->id()) {
	    rts_block = true;
	    break;
	  }
	  Compound_region* ancestor = next_ancestor;
	  next_ancestor = ancestor->parent();
	}
	if (rts_block) {
	  assert (rts_stall_cycles == 0.0); // only one rts in function
	  rts_stall_cycles = reg_superscalar_stall_cycles;
	}
      }
      int br_lat = MDES_branch_latency();
      pe_weight = 0.0;
      pe_sched_length = 0;
      for (Region_ops_C0_order oiter (reg); oiter != 0; oiter++) {
	Op* op = *oiter;
	if (is_branch(op)) {
	  if (is_dummy_branch (op)) {
	    exit_sched_time = op->sched_time(); 
	  } else {
	    exit_sched_time = op->sched_time() + br_lat - 1; 
	  }
	  for (Op_outedges ei (op); ei != 0; ei++) {
	    Edge *edge = *ei;
	    if (! reg->is_exit_edge(edge))
	      continue;
	    Control_flow_freq *cfreq = get_control_flow_freq (edge);
	    freq -= cfreq->freq;
	    /* assert (freq >= 0);, may fail w/ inconsisent profile weight */
	    val = cfreq->freq * (exit_sched_time - entry_sched_time + 1);
	    local_cycle_count += val;
	    El_tot_cycle_count += val;
	    scalar_cycle_count += val;
	    weighted_sched_length += val;
	    if (cfreq->freq >= pe_weight) {
	      pe_weight = cfreq->freq;
	      pe_sched_length = exit_sched_time - entry_sched_time + 1;
	    }
	  }
	  /* RTS's have no edges; treat as branch for cycle_count summing */
	  if (is_rts(op)) {
	    val = freq * (exit_sched_time - entry_sched_time + 1);
	    local_cycle_count += val;
	    El_tot_cycle_count += val;
	    scalar_cycle_count += val;
	    weighted_sched_length += val;
	    if (freq >= pe_weight) {
	      pe_weight = freq;
	      pe_sched_length = exit_sched_time - entry_sched_time + 1;
	    }
	  }
          branch_mpred_cycles += El_calculate_branch_mispredict_penalty(op);
	}
      }
      if (El_do_block_cycle_count) {
        if (reg->weight > 0.0) {
          weighted_sched_length = weighted_sched_length / reg->weight;
	  pe_ratio = pe_weight / reg->weight;
	}
        else {
          // This is a bit nonstandard, but for 0 profile weight, assume the 
          // last exit is taken 100% of the time.
          weighted_sched_length = pe_sched_length;
	  pe_ratio = 1.0;
	}
	(*EL_STAT_STREAM) << "    region_cycles\t"
		       << (reg->is_hb() ? "hb " : "bb ")
		       << reg->id() << '\t'
		       << "wsl " << weighted_sched_length << "  "
		       << "pesl " << pe_sched_length << "  "
		       << "per " << pe_ratio << "  "
		       << "wgt " << reg->weight
		       << endl;
      }
    }
  }

  else { // compute cycle count for all regions "underneath" this region.
    for (Region_subregions sub (reg); sub != 0; sub++) {
      Compound_region* subreg = (Compound_region* ) (*sub);
      local_cycle_count += compute_dynamic_cycle_count (subreg);
    } 
  }

  return (local_cycle_count);
}

void El_compute_cpr_cycle_count(Compound_region *reg)
{
  if (reg->flag(EL_REGION_SCHEDULED) && reg->flag(EL_REGION_CPR_BLOCK)) {
    Hash_set<Compound_region *> cpr_blocks;
    Edge *edge;
    Op *dest_op, *op;
    Compound_region *dest_reg, *blk;
    double val, weighted_sched_length, pe_weight, pe_ratio, freq;
    Control_flow_freq *cfreq;
    int pe_sched_length, entry_sched_time, exit_sched_time = -1, br_lat;

    /* reg is the main block, need to collect together all the comp blocks */
    cpr_blocks += reg;
    for (Region_exit_edges edge_i(reg); edge_i!=0; edge_i++) {
      edge = *edge_i;
      dest_op = edge->dest();
      dest_reg = dest_op->parent();
      if (dest_reg->is_bb())
        dest_reg = dest_reg->parent();
      if (dest_reg->flag(EL_REGION_CPR_COMP_BLOCK))
        cpr_blocks += dest_reg;
    }
    /* Now go thru all the blocks and compute the cycles for each */
    /* This is very similar to the HB part of compute_dynamic_cycle_count() */
    br_lat = MDES_branch_latency();
    pe_weight = 0.0;
    pe_sched_length = 0;
    weighted_sched_length = 0.0;
    for (Hash_set_iterator<Compound_region *> blk_i(cpr_blocks); blk_i!=0; blk_i++) {
      blk = *blk_i;
      freq = blk->weight;
      Region_entry_ops entry(reg);
      Op *entry_op = *entry;    // get the top entry op
      entry_sched_time = entry_op->sched_time();
      for (Region_ops_C0_order op_i(blk); op_i!=0; op_i++) {
        op = *op_i;
        if (is_branch(op)) {
          if (is_dummy_branch (op))
            exit_sched_time = op->sched_time();
          else
            exit_sched_time = op->sched_time() + br_lat - 1;
        }
        for (Op_outedges edge_i2 (op); edge_i2 != 0; edge_i2++) {
          edge = *edge_i2;
          if (! blk->is_exit_edge(edge))
            continue;
          cfreq = get_control_flow_freq(edge);
          freq -= cfreq->freq;
          val = cfreq->freq * (exit_sched_time - entry_sched_time + 1);
          weighted_sched_length += val;
          if ((blk == reg) && (cfreq->freq >= pe_weight)) {
            pe_weight = cfreq->freq;
            pe_sched_length = exit_sched_time - entry_sched_time + 1;
          }
        }
        /* RTS's have no edges; treat as branch for cycle_count summing */
        if (is_rts(op)) {
          val = freq * (exit_sched_time - entry_sched_time + 1);
          weighted_sched_length += val;
          if ((blk == reg) && (freq >= pe_weight)) {
            pe_weight = freq;
            pe_sched_length = exit_sched_time - entry_sched_time + 1;
          }
        }
      }
    }

    /* Now print out the relevant info */
    if (reg->weight > 0.0) {
      weighted_sched_length = weighted_sched_length / reg->weight;
      pe_ratio = pe_weight / reg->weight;
    }
    else {
      // This is a bit nonstandard, but for 0 profile weight, assume the 
      // last exit is taken 100% of the time.
      weighted_sched_length = pe_sched_length;
      pe_ratio = 1.0;
    }
    (*EL_STAT_STREAM) << "    cpr_cycles\t"
                   << "hb "
                   << reg->id() << '\t'
                   << "wsl " << weighted_sched_length << "  "
                   << "pesl " << pe_sched_length << "  "
                   << "per " << pe_ratio << "  "
                   << "wgt " << reg->weight << "  "
                   << "cblks (";
    for (Hash_set_iterator<Compound_region *> blk_i2(cpr_blocks);
                                        blk_i2!=0; blk_i2++) {
      blk = *blk_i2;
      if (blk == reg)
        continue;
      (*EL_STAT_STREAM) << " " << blk->id();
    }
    (*EL_STAT_STREAM) << " )" << endl;
  }
  else if (reg->is_hb() || reg->is_bb() || reg->is_loopbody()) {
    ;	/* Nothing of interest underneath these, so do nothing */
  }
  else {
    for (Region_subregions sub (reg); sub != 0; sub++) {
      Compound_region* subreg = (Compound_region* ) (*sub);
      El_compute_cpr_cycle_count(subreg);
    }
  }
}


/*=========================================================================*/
/*
 *	Externally visible routines for cycle/op stat computation
 */
/*=========================================================================*/

void El_show_block_types(Procedure* f)
{
  show_block_types(f);
}

void El_compute_cycle_stats(Procedure* f)
{
  El_reset_dynamic_cycle_counters();
  if (SUPERSCALAR_MELD || MELD_COMPARE) {
    cfEdgeIsPredicted->clear();
    init_cfEdgeIsPredicted (f);
  }
  (void) compute_dynamic_cycle_count(f);

  if (El_cache_load_sched != NONE) {
    if (El_cache_load_sched == USE_STALL)
      dcache_miss_cycles = (double)get_region_stall_cycles(f);
    else {
      assert ((El_cache_load_sched == ANALYZE) ||
	      (El_cache_load_sched == LAT_STALL));
      dcache_miss_cycles = miss_cycle_count(f);
    }
  }

  El_print_dynamic_cycle_counters();
}

void El_compute_cpr_cycle_stats(Procedure *f)
{
    El_compute_cpr_cycle_count(f);
}
 
