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
//      File:           process_function.cpp
//      Authors:        Vinod Kathail
//      Created:        November 1995
//      Description:    Default Elcor Compiler Driver
//
/////////////////////////////////////////////////////////////////////////////



#include <iostream>
#include <iomanip>
#include "process_function.h"

#include "defs.h"
#include "operand.h"
#include "connect.h"
#include "map.h"
#include "stdlib.h"
#include "opcode.h"
#include "edge.h"
#include "edge_drawing.h"
#include "dbg.h"
#include "attributes.h"
#include "pred_analysis.h"
#include "opcode_load_store.h"
#include "hash_map.h"
#include "list.h"
#include "hash_set.h"
#include "el_loop_tools.h"
#include "el_swp_dsa_convert.h"
#include "stdio.h"
#include "matrix.h"
#include "vectorize_driver.h"
#include "msched_driver.h"
#include "iterators.h"
#include "el_lsched_init.h"

#include "graph_filters.h"
#include "mdes.h"
#include "mdes_attributes.h"
#include "intf.h"
#include "hash_functions.h"
#include "schedule.h"
#include "scalar_sched.h"
#include "el_stat_driver.h"
#include "el_main.h"
#include "el_opcode_usage.h"
#include "el_opti_driver.h"
#include "cluster.h"
#include "el_cluster_init.h"
#include "el_dot_tools.h"
#include "ir_writer.h"
#include "el_clock.h"
#include "el_driver_init.h"
#include "scalar_reg_alloc.h"
#include "el_lit_histogram.h"
#include "el_cpr_driver.h"
#include "el_cpr_prep.h"
#include "el_width_analysis.h"
#include "codegen.h"
#include "regfile_bind.h"
#include "pattern_match.h"
#include "example_driver.h"


static Procedure *null_process_function_for_simu(Procedure *f);
static Procedure *gui_view_ir(Procedure *f);

//----------------------------------------------------------------------------
// This is the new Elcor compiler driver. The user-based process functions may
// wrap around this one or be completely separate. 
//--------------------------------------------------------------------------

Procedure *El_process_function(Procedure *f)
{
  if (El_do_null_processing==1)
    return (f);

  if (El_do_null_processing_for_simu == 1)
    return null_process_function_for_simu(f);

  if(El_gui_view_ir_cfg == 1 || El_gui_view_ir_dfg == 1) {
    return gui_view_ir(f);
  }

  /* Selective processing skips functions if they meet a certain criteria */
  if (El_sel_processing_mode == EL_SEL_PROC_WEIGHT) {
    if (f->weight <= El_sel_processing_weight) {
      if (dbg (status, 1))
        cout << "Skipping procedure '" << f->get_name() << "'\n";
      if (El_do_stats_always) {
        El_collect_pre_processing_stats(f);
        El_collect_pre_scheduling_stats(f);
        El_collect_post_processing_stats(f);
      }
      return(f);
    }
  }
  else if (El_sel_processing_mode == EL_SEL_PROC_NAME) {
    if (! El_sel_processing_name_table->is_member(f->get_name())) {
      if (dbg (status, 1))
        cout << "Skipping procedure '" << f->get_name() << "'\n";
      if (El_do_stats_always) {
        El_collect_pre_processing_stats(f);
        El_collect_pre_scheduling_stats(f);
        El_collect_post_processing_stats(f);
      }
      return (f);
    }
  }

  f = common_process_function (f);

  return (f);
}


static Procedure *null_process_function_for_simu(Procedure *f)
{
  if (strcasecmp(El_arch, "hpl-pd"))
    El_punt("El_do_null_processing_for_simu only intended for HPL-PD");

  Codegen::do_initial_codegen(f);

  return f;
}

static Procedure *gui_view_ir(Procedure *f)
{
  /*
  if (strcasecmp(El_arch, "hpl-pd"))
    El_punt("El_do_null_processing_for_simu only intended for HPL-PD");

  Codegen::do_initial_codegen(f);
  */

  if(0 == strcmp(f->get_name(), El_gui_view_ir_func_name)) {
    int temp = El_print_dot_graph;
    El_print_dot_graph = 1;
    if(El_gui_view_ir_cfg == 1) {
      el_dot_display_cfg(f);
    }
    if(El_gui_view_ir_dfg == 1) {
      for (Region_subregions subreg(f); subreg != 0; subreg++) {
        if((*subreg)->id() == El_gui_view_ir_block_id)
          el_dot_display_dfg(*subreg);
      }
    }
    El_print_dot_graph = temp;
  }

  return f;
}

Procedure *common_process_function(Procedure *f)
{
  if (dbg (status, 1)) {
    cdbg << "Compiling procedure '" << f->get_name() << "'\n";
  }

  if (El_do_codegen) {
    // Architecture specific transformations that need to be done.
    Codegen::MachineSpecificConversion(f);
  }

  // Calculated operand widths.  Should not be done for normal VLIW compiles.

  if (El_do_width_analysis) {
    if (dbg(status,1))
      cdbg << "Doing bitwidth analysis" << endl;
    do_width_analysis(f, true);
    if (dbg(status,5))
      ir_write(f, "%s_width_analysis.reb", (char*)f->get_name());
  }

  // Initialize da-vinci. We need some common way of initializing guis
  if (El_ssched_graph_display) {
    gui.initconnect();
    gui.test();
  }

  // Pre process statistic collection
  El_collect_pre_processing_stats(f); 

  // Classic local and global optimizations
  if (El_do_classic_opti) {
    if (dbg(status,1))
      cdbg << "Performing classic optimizations" << endl;
    el_opti_driver(f);
    if (dbg(status,5)) {
      cdbg << "Printing Rebel after optimization" << endl;
      ir_write(f, "%s_opti.reb", (char*)f->get_name());
    }
  }

  // Code generation
  if (dbg(status,1))
    cdbg << "Performing restricted io codegen(initial pass)" << endl;
  Codegen::do_initial_codegen(f);

  if (dbg(genrio,5)) {
    cdbg << "Printing Rebel after Initial Codegen" << endl;
    ir_write(f, "%s_initial_codegen.reb", (char*)f->get_name());
  }

  if (El_do_examples) {
    run_examples(f);
  }

  if (El_do_prepass_custom_ops) {
    pattern_match_procedure(f);
  }

  // Control CPR
  if (El_do_scalar_control_cpr) {
    if (dbg(status, 1))
      cdbg << "Performing scalar control CPR" << endl;
    El_scalar_control_cpr(f);
  }

  // SLARSEN: Vectorize before setting up swp regions
  if (El_do_vectorize) {
    if (dbg(status,2))
      cdbg << "Vectorizing" << endl;
    cout << "--------------------------------------------------------------------\n" 
	 << f->get_name() << "\n"
	 << "--------------------------------------------------------------------\n" ;
    vectorize_driver(f);
  }

  // call control-flow analysis to detect swp loops 
  if (El_do_modulo_scheduling) {
    if (dbg(status,2))
      cdbg << "Finding SWP loops" << endl;
    El_setup_swp_loop_regions(f);
  }

  if(El_do_lit_histogram)
    do_literal_histogram(f);

  // Stats after xforms, but before scheduling since analyzing scheduled
  // code may not be possible
  El_collect_pre_scheduling_stats(f); 
  // cluster manager - this could be constructed anywhere before
  // scheduling
  Cluster_mgr cmgr(f);

  // Compute local vr_maps and PQS for dataflow analysis
  create_local_analysis_info_for_all_hbs_bbs(f);
  
  // compute global liveness and verify edge drawing
  if (dbg(status,2))
    cdbg << "Performing Liveness Analysis" << endl;
  if (El_show_edges) {
    el_flow_compute_four_dataflow_sets(f);
    insert_region_scalar_edges(f);
    for (Region_subregions subreg(f); subreg != 0; subreg++)
      el_dot_display_dfg(*subreg);
    delete_region_edges(f);
    el_flow_delete_three_dataflow_sets(f);
  } else {
    el_flow_compute_liveness(f, ANALYZE_ALLREG);
  }

  // Modulo schedule and allocate all loops in frequency order
  if (El_do_modulo_scheduling) {
    if (dbg(status,2))
      cdbg << "Performing modulo scheduling" << endl;
    modulo_schedule_all_loops(f, cmgr);
    if (dbg(status,5)) {
      cdbg << "Printing Rebel after modulo scheduling" << endl;
      ir_write(f, "%s_modsched.reb", (char*)f->get_name());
    }
  }

  // Always call scalar_schedule_all_hbs_bbs in case clustering or codegen
  // needs to be done. -KF 1/2005
  scalar_schedule_all_hbs_bbs (f, EL_SCALAR_SCHED_PREPASS, cmgr);

  if (El_do_codegen) {
    bind_operands_of_pseudo_ops(Codegen::prf_bind_map, f);
    bind_implicit_operands(Codegen::prf_bind_map, f);

    // Initalizes the home physical register file bindings for operands
    // which are to be honored during postpass codegen
    Codegen::postpass_codegen_initialize(f);
  }

  // Scalar register allocation
  if (El_do_scalar_regalloc) {

    El_fix_all_pbr_info(f, false);

    Scalar_reg_alloc reg_alloc(f);
    f->set_flag(EL_PROC_REGISTER_ALLOCATED);

    create_local_analysis_info_for_all_hbs_bbs(f);
    el_flow_compute_liveness(f,ANALYZE_ALLREG);
    if (dbg(status,5)) {
      cdbg << "Printing Rebel after scalar register allocation" << endl;
      ir_write(f, "%s_post_regalloc.reb", (char*)f->get_name());
    }
  }

  if (El_do_codegen)
    Codegen::post_regalloc_codegen_initialize(f);

  // Creates a stack frame where all parameters including return address
  // are passed through the stack. Changes all occurances of OP/LV/IP/SP(spill)
  // to SP. -- Pracheeti 08/02
  if (El_create_stack_frame)
     Codegen::create_stack_frame(f, cmgr);

  // Always call scalar_schedule_all_hbs_bbs in case clustering or codegen
  // needs to be done. -KF 1/2005
  scalar_schedule_all_hbs_bbs (f, EL_SCALAR_SCHED_POSTPASS, cmgr);

  // check that all registers/macros are bound to physical files
  // cmgr.check_files_bound(f, false);
  if (dbg(genrio,3)) 
    Codegen::print_move_stats();
  
  if (El_do_postpass_custom_ops) {
    pattern_match_procedure(f);
  }

  // Delete liveness
  el_flow_delete_liveness(f);

  // delete vr_maps and PQS  
  delete_local_analysis_info_for_all_hbs_bbs(f);

  // Control CPR fixup code, delete unnecessary dummy brs, this may be necessary
  // even when not doing CPR
  if (El_do_scalar_control_cpr) {
    if (dbg(status,2))
      cdbg << "Performing DUMMY_BR fixup for scalar control CPR" << endl;
      El_fixup_dummy_branches(f);
  }

  // Post process statistic collection
  El_collect_post_processing_stats(f); 

  return (f);
}

//---------------------------------------------------------------------------
//  Iterate over a compound region and schedule all hbs and bbs 
//---------------------------------------------------------------------------
void scalar_schedule_all_hbs_bbs(Procedure* f, El_Scalar_Sched_Phase phase,
                                 Cluster_mgr& cmgr)
{
  if (El_do_run_time) {
    El_clock2.reset();
    El_clock3.reset();
    El_clock4.reset();
    El_clock1.start();
  }

  bool do_scheduling =
    (phase == EL_SCALAR_SCHED_PREPASS && El_do_prepass_scalar_scheduling) ||
    (phase == EL_SCALAR_SCHED_POSTPASS && El_do_postpass_scalar_scheduling);

  if (do_scheduling) {    
    if (dbg(status,2)) {
      cdbg << "Performing "
           << ((phase == EL_SCALAR_SCHED_PREPASS) ? "prepass" : "postpass")
           << " acyclic scheduling" << endl;
    }
    scalar_sched_prolog(f);
  }

  // Currently, this loop performs clustering and codegen on all subregions
  // unconditionally, and only performs scheduling if the parm is set. -KF 1/2005
  for (Region_subregions_freq sub(f, HB_BB_FILTER, HB_BB_SCHED_FILTER ); 
       sub != 0; sub++) {
    Compound_region* subregion = (Compound_region*)*sub;
    
    // SAM 4-97, temporary hack, so don't postpass sched SWPs where the 
    // loopbody indicator is lost going back thru Impact for scalar regalloc
    if (subregion->flag(EL_REGION_SOFTPIPE) &&
        subregion->flag(EL_REGION_VIOLATES_LC_SEMANTICS)) {
        subregion->set_flag(EL_REGION_SCHEDULED);
        continue;
    }
    
    if (phase == EL_SCALAR_SCHED_PREPASS) {

      cmgr.do_block_clustering(subregion, El_cluster_method);

      // Assign and bind physical files
      if (El_do_codegen)
        Codegen::do_prepass_post_cluster_codegen(subregion, cmgr);
    } else {
      // Postpass
      cmgr.do_block_postpass_clustering(subregion, El_cluster_method);
      if (El_do_codegen)
        Codegen::do_postpass_post_cluster_codegen(subregion, cmgr);
    }
  }

  if (do_scheduling) {

    for (Region_subregions_freq sub(f, HB_BB_FILTER, HB_BB_SCHED_FILTER ); 
	 sub != 0; sub++) {
      Compound_region* subregion = (Compound_region*)*sub;
      // SAM 4-97, temporary hack, so don't postpass sched SWPs where the 
      // loopbody indicator is lost going back thru Impact for scalar regalloc
      if (subregion->flag(EL_REGION_SOFTPIPE) &&
	  subregion->flag(EL_REGION_VIOLATES_LC_SEMANTICS)) {
        subregion->set_flag(EL_REGION_SCHEDULED);
        continue;
      }

      create_local_analysis_info_for_all_hbs_bbs(subregion);	
      
      scalar_scheduler_driver(subregion, phase);
    }
  }

  if (do_scheduling) {
    scalar_sched_epilog(f);
    if (dbg(status,5)) {
      const char* phasename = ((phase == EL_SCALAR_SCHED_PREPASS) ? "pre" : "post");
      cdbg << "Printing Rebel after " << phasename << "-pass scheduling" << endl;
      ir_write(f, "%s_%ssched.reb", (char*)f->get_name(), phasename);
    }
  }

  if (El_do_run_time) {
    El_clock1.stop();
    El_stopwatch_print_time("total_edge_drawing", El_clock2.total_elapsed());
    El_stopwatch_print_time("total_scalar_scheduling", El_clock3.total_elapsed());
    El_stopwatch_print_time("total_delete_edges", El_clock4.total_elapsed());
    El_stopwatch_print_time("scalar_schedule_all_hbs_bbs",
				El_clock1.last_elapsed());
  }

}

//---------------------------------------------------------------------------
//  Iterate over a compound region and schedule all loops
//---------------------------------------------------------------------------
void modulo_schedule_all_loops(Compound_region* r, Cluster_mgr& cmgr)
{
  if (El_do_run_time)
    El_clock1.start();

  for (Region_subregions_freq sub(r, LOOPBODY_FILTER, LOOPBODY_HB_BB_FILTER); 
      sub != 0; sub++) {
    LoopBody* loop = (LoopBody*)*sub;

    if (El_block != 0 && El_block != loop->kernel()->id())
      continue;

    r->set_flag(EL_PROC_ROT_REG_ALLOCATED);
    // Added SAM 4-98, Impact requires this flag to be set to properly analyze
    // the staging predicates created by msched
    r->set_flag(EL_PROC_HYPERBLOCK);
    modulo_scheduler_driver(loop, cmgr);
  }

  if (El_do_run_time) {
    El_clock1.stop();
    El_stopwatch_print_time("modulo_schedule_all_loops",
                                El_clock1.last_elapsed());
  }
}
