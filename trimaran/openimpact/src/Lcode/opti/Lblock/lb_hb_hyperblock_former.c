/*****************************************************************************\
 *
 *		      Illinois Open Source License
 *                     University of Illinois/NCSA
 *                         Open Source License
 *
 * Copyright (c) 2004, The University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:             
 *
 *		IMPACT Research Group
 *
 *		University of Illinois at Urbana-Champaign
 *
 *              http://www.crhc.uiuc.edu/IMPACT
 *              http://www.gelato.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal with the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimers.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimers in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the names of the IMPACT Research Group, the University of
 * Illinois, nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without specific
 * prior written permission.  THE SOFTWARE IS PROVIDED "AS IS",
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
 *
\*****************************************************************************/
/*===========================================================================
 *      File :          l_hyperblock_former.c
 *      Description :   Driver for hyperblock formation
 *      Creation Date : February 1998
 *      Authors :       Kevin Crozier
 *        modified from Scott Mahlke's l_hyperblock.c
 *
 *==========================================================================*/
/* 10/29/02 REK Adding config.h */
#include <config.h>
#include "lb_hb_hyperblock.h"
#include "lb_hb_peel.h"

LB_HB_Stat LB_hb_stat;

#undef DEBUG_HB_FORMER

void
LB_split_exit_block (L_Func *fn)
{
  L_Cb *cb, *newcb;

  for (cb = fn->first_cb; 
       cb && !L_subroutine_return_opcode (cb->last_op);
       cb = cb->next_cb);

  if (!cb)
    return;

  if (cb->src_flow && cb->src_flow->next_flow)
    {
      L_Flow *fl, *mfl;
      L_Cb *scb;
      L_Oper *sbr;

      /* Multiple in-flows to return block.  Need to separate. */

      newcb = L_create_cb (cb->weight);

      L_insert_cb_before (fn, cb, newcb);

      newcb->src_flow = cb->src_flow;

      fl = L_new_flow (0, newcb, cb, cb->weight);
      newcb->dest_flow = fl;
      fl = L_copy_single_flow (fl);
      cb->src_flow = fl;

      for (fl = newcb->src_flow; fl; fl = fl->next_flow)
	{
	  scb = fl->src_cb;
	  mfl = L_find_matching_flow (scb->dest_flow, fl);
	  if ((sbr = L_find_branch_for_flow (scb, mfl)))
	    L_change_branch_dest (sbr, cb, newcb);
	  mfl->dst_cb = newcb;
	  fl->dst_cb = newcb;
	}

      sbr = L_create_new_op (Lop_JUMP);
      sbr->src[0] = L_new_cb_operand (cb);
      L_insert_oper_after (newcb, NULL, sbr);
    }
  return;
}

static int
LB_mark_ncycle_regions (LB_TraceRegion_Header *header)
{
  LB_TraceRegion *tr;
  int found = 0;

  List_start (header->traceregions);
  while ((tr = List_next (header->traceregions)))
    {
      Set tr_set = LB_return_cbs_region_as_set (tr);
      if (LB_hb_region_contains_cycle (tr_set, tr->header))
	{
	  tr->flags |= L_TRACEREGION_FLAG_NESTED_CYCLE;
	  found++;
	}
      Set_dispose (tr_set);
    }

  return found;
}

void
LB_hyperblock_formation (L_Func * fn)
{
  LB_TraceRegion_Header *header;
  LB_TraceRegion *tr;
  L_Cb *cb;
  L_Oper *op;
  int do_peel, final_ops;

  if (fn->n_cb == 0)
    return;

  memset (&LB_hb_stat, 0, sizeof (LB_HB_Stat));

  LB_split_exit_block (fn);

  header = LB_function_init (fn);

  L_breakup_pre_post_inc_ops (fn);

  LB_clr_hyperblock_flag (fn);

  L_compute_oper_weight (fn, 0, 1);

  /* make sure all cbs contain at most 2 targets */

  LB_convert_to_strict_basic_block_code (fn, L_CB_SUPERBLOCK | 
					 L_CB_HYPERBLOCK |
					 L_CB_ENTRANCE_BOUNDARY | 
					 L_CB_EXIT_BOUNDARY);

  for (cb = fn->first_cb; cb; cb = cb->next_cb)
    {
      L_Oper *next_op;
      for (op = cb->first_op; op; op = next_op)
	{
	  next_op = op->next_op;

	  if (op->opc == Lop_NO_OP)
	    L_delete_oper (cb, op);
	  else
	    LB_hb_stat.orig_ops++;
	}
    }

  L_do_flow_analysis (fn, DOMINATOR | POST_DOMINATOR | LIVE_VARIABLE);
  L_loop_detection (fn, 0);

  if (LB_hb_verbose_level >= 7)
    {
      fprintf (stderr, "Initial Loop Detection Phase for (fn %s):\n",
	       fn->name);
      L_print_loop_data (fn);
    }

  LB_elim_all_loop_backedges (fn);

  L_delete_unreachable_blocks (fn);

#if 0
  /*
   * Prepass loop peeling (new 20021015)
   * ----------------------------------------------------------------------
   */

  {
    L_Loop *lp;
    int pl_id = 0;

    for (lp = fn->first_loop; lp; lp = lp->next_loop)
      {
	if (!Set_empty(lp->nested_loops))
	  continue;

	if (LB_hb_find_num_peel (lp, lp->loop_cb))
	  {
	    L_warn ("PREPASS PEELING %s loop hdr %d",
		    fn->name, lp->header->id);

	    LB_peel_loop (fn, lp, lp->loop_cb, NULL, pl_id++, 1, 
			  NULL, NULL);
	  }
      }

    if (pl_id)
      {
	L_do_flow_analysis (fn, DOMINATOR | POST_DOMINATOR | LIVE_VARIABLE);
	L_loop_detection (fn, 0);

	LB_elim_all_loop_backedges (fn);
	L_delete_unreachable_blocks (fn);
      }
  }
  
  /* end 20021015 additions */
#endif

  L_do_flow_analysis (fn, DOMINATOR | POST_DOMINATOR | LIVE_VARIABLE);
  L_reset_loop_headers (fn);
  L_loop_detection (fn, 0);

  if (LB_hb_verbose_level >= 7)
    {
      fprintf (stderr, "Final Loop Detection Phase for (fn %s):\n", fn->name);
      L_print_loop_data (fn);
    }

  L_compute_oper_weight (fn, 0, 1);
  LB_mark_jrg_flag (fn);

  /*
   * Hyperblock formation phase 1
   * ----------------------------------------------------------------------
   */

  L_partial_dead_code_removal (fn);
  L_do_flow_analysis (fn, DOMINATOR | POST_DOMINATOR | 
		      LIVE_VARIABLE | SUPPRESS_PG);

  if (!LB_hb_form_simple_hammocks_only)
    LB_hb_find_loop_regions (fn, header);

  LB_hb_find_hammock_regions (fn, header);

  LB_hb_reset_max_oper_id (fn);

  LB_remove_partially_subsumed_traceregions (header);

  /* Mark regions requiring peeling */

  do_peel = LB_mark_ncycle_regions (header);
  
  if (LB_hb_verbose_level >= 5)
    {
      fprintf (stderr, "Regions computed in Run 1 of HB formation for "
	      "(fn %s):\n", fn->name);
      LB_summarize_traceregions (stderr, header);
    }

  if (do_peel)
    {
      /* Peel or delete regions containing nested cycles */

      List_start (header->traceregions);
      while ((tr = List_next (header->traceregions)))
	{
	  if (tr->flags & L_TRACEREGION_FLAG_NESTED_CYCLE)
	    {
	      if (!LB_hb_peel_enable)
		header->traceregions = List_remove (header->traceregions, tr);
	      else
		LB_hb_do_loop_peel (fn, tr, header);
	    }
	}
    }

  LB_remove_conflicting_traceregions (header);

  LB_set_hyperblock_flag (fn);
  LB_set_hyperblock_func_flag (fn);

  LB_tail_duplicate (fn, header, LB_DUP_OUTSIDE_REGION);

#ifdef DEBUG_HB_FORMER
  L_check_func (fn);
#endif

  LB_predicate_traceregions (fn, header);

  /*  Mark all cbs with any type of hyperblock flag with L_CB_HYPERBLOCK */
  LB_set_hyperblock_flag (fn);
  LB_remove_unnec_hyperblock_flags (fn);
  LB_set_hyperblock_func_flag (fn);

  LB_free_all_traceregions (header);

  LB_remove_empty_cbs (fn);
  L_delete_unreachable_blocks (fn);

  LB_convert_to_strict_basic_block_code (fn, L_CB_HYPERBLOCK | 
					 L_CB_SUPERBLOCK);

#ifdef DEBUG_HB_FORMER
  DB_spit_func (fn, "PH1");
  L_check_func (fn);
#endif

  /*
   * Hyperblock formation phase 2
   * ----------------------------------------------------------------------
   */

  L_partial_dead_code_removal (fn);
  L_do_flow_analysis (fn, DOMINATOR | POST_DOMINATOR | 
		      LIVE_VARIABLE | SUPPRESS_PG);

  LB_hb_find_hammock_regions (fn, header);

  if (!LB_hb_form_simple_hammocks_only)
    LB_hb_find_general_regions (fn, LB_BLOCK_SEL_NO_NESTED_HAMMOCKS |
				LB_BLOCK_SEL_NO_NESTED_LOOPS, header);

  LB_hb_reset_max_oper_id (fn);

  LB_remove_partially_subsumed_traceregions (header);
  LB_remove_subsumed_traceregions (header);

  if (LB_hb_verbose_level >= 5)
    {
      fprintf (stderr, "Regions computed in Run 2 of HB formation for "
	      "(fn %s):\n", fn->name);
      LB_summarize_traceregions (stderr, header);
    }

  LB_tail_duplicate (fn, header, LB_DUP_OUTSIDE_REGION);

  LB_predicate_traceregions (fn, header);

  L_delete_unreachable_blocks (fn);

  LB_remove_unnec_hyperblock_flags (fn);

  LB_free_all_traceregions (header);

  /*
   *  Split branches into pred defines and predicated jumps
   */

  if (LB_hb_branch_split)
    LB_branch_split_func (fn);

  /*
   *        Generate multiple defn pred defines, generate Uncond pred defines
   *  (Note initially only OR-type pred defines are created!
   */

  L_create_uncond_pred_defines (fn);

  PG_setup_pred_graph (fn);
  if (LB_do_lightweight_pred_opti)
    L_lightweight_pred_opti (fn);
  L_combine_pred_defines (fn);

  /*
   *        remove unnecessary uncond jumps
   */

  LB_uncond_branch_elim (fn);

  /*
   *        Merge ops on opposite predicates (partial redundancy elim)
   */

  L_unmark_all_pre_post_increments (fn);

  if (LB_hb_do_pred_merge)
    {
      PG_setup_pred_graph (fn);
      L_do_flow_analysis (fn, LIVE_VARIABLE|REACHING_DEFINITION);
      LB_hb_pred_merging (fn);
    }

#ifdef DEBUG_HB_FORMER
  L_check_func (fn);
#endif

  PG_destroy_pred_graph ();
  D_delete_dataflow (fn);

  /* For each source code function we process deinit */
  LB_function_deinit (header);

  {
    double rat = 1.0;
    L_Attr *eattr;

    final_ops = 0;

    for (cb = fn->first_cb; cb; cb = cb->next_cb)
      {
	L_Oper *next_op;
	for (op = cb->first_op; op; op = next_op)
	  {
	    next_op = op->next_op;
	    
	    if (op->opc != Lop_NO_OP)
	      final_ops ++;
	  }
      }

    if (LB_hb_stat.orig_ops)
      rat = (double) final_ops / LB_hb_stat.orig_ops;

    eattr = L_new_attr ("hbe", 2);
    L_set_int_attr_field (eattr, 0, LB_hb_stat.orig_ops);
    L_set_double_attr_field (eattr, 1, rat);
    fn->attr = L_concat_attr (fn->attr, eattr);
  }

  return;
}
