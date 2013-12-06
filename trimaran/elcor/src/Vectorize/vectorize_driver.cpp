/***********************************************************************

Copyright (c) 2007 Massachusetts Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

***********************************************************************/



//===========================================================================
//
//   FILE: vectorize_driver.cpp:
//   
//   Author: Sam Larsen
//   Date: Mon Mar  7 11:21:05 2005
//
//   Function:  Driver for selective vectorizer
//
//===========================================================================

#include "pred_analysis.h"
#include "el_opti_dead_code.h"

// Function to check if this loop already has vector ops
bool has_vector_ops(El_Loop* loop)
{
  Basicblock* body = loop->loop_blocks.head();
  for (Region_all_ops ops(body); ops != 0; ops++)
    if (is_vector(*ops))
      return true;

  return false;
}


void vectorize_driver(Procedure* proc)
{
  create_local_analysis_info_for_all_hbs_bbs(proc);
  el_flow_compute_four_dataflow_sets(proc);

  El_form_basic_blocks(proc);

  El_Loop_Graph loops(proc);
  loops.find_induction_variable_info(ALL_LOOPS);
  loops.find_swp_able_loops(ALL_LOOPS);

  for (Map_iterator<int,El_Loop*> iter(loops.id); iter !=0; iter++)
  {
    El_Loop* loop = (*iter).second;
    if (loop->swp_status == SWP_ABLE)
    {
      assert(loop->category == DO_LOOP);

      // This is probably too conservative, but I'm only supporting
      // loops with a constant increment right now
      if ((loop->primary_ind_var_ops.size() != 1) ||
	  !(loop->primary_ind_var_ops.head()->src(SRC2).is_int()))
	continue;

      // don't vectorize if condition is eq or neq : it's hard to make a 
      // loop test block for this condition cause just testing opposite condition 
      // wouldn't work. : parkhc 02/2007
      Op* branch = El_get_branch_op(loop->loop_blocks.head());
      Op* cond_op = El_get_cond_for_branch(branch);
      if (is_int_neq_cmpp(cond_op) || is_int_eq_cmpp(cond_op))
        continue;

      // fix me if you want to handle hyperblocks
      if (loop->loop_blocks.size() != 1)
	continue;

      // Don't vectorize if a frontend vectorizer has already created
      // vector operations
      if (has_vector_ops(loop))
	continue;

      Vectorizer vectorizer(loop);
    }
  }

  // Adding this so that msched does not choke on extra PBRs later.  -KF 1/2007
  el_opti_eliminate_dead_code(proc, ANALYZE_ALLREG, true, true);
  
  El_remove_basic_blocks(proc);
  el_flow_delete_four_dataflow_sets(proc);
  delete_local_analysis_info_for_all_hbs_bbs(proc);
}
