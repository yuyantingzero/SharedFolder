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
//      File:           msched_driver.cpp
//      Authors:        Vinod Kathail, Shail Aditya
//      Created:        September 1995
//      Description:    Driver for modulo-scheduling/rotating register allocation 
//
//
/////////////////////////////////////////////////////////////////////////////

#include "msched_driver.h"
#include "ModuloScheduler.h"
#include "stage_sched.h"
#include "pre_post.h"
#include "rr_allocation.h"
#include "el_swp_dsa_convert.h"
#include "edge_drawing.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "el_control.h"
#include "el_lreg_init.h"
#include "el_lsched_init.h"
#include "el_ssched_init.h"
#include "modulo_variable_expansion.h"
#include "dbg.h"
#include "intf.h"
#include "el_dot_tools.h"
#include "el_main.h"
#include "attributes.h"
#include "pred_analysis.h"
#include "cluster.h"
#include "codegen.h"
#include "el_codegen_init.h"
#include "regfile_bind.h"
#include "ir_writer.h"
#include "el_width_analysis.h"


ModuloScheduler *msched_glb;
extern void collect_bypass_stats_for_region(Compound_region* r);

// from delete_region_edges (core_edge_drawing.cpp), except only delete edges
// going to/from a sw/mg node
void ms_delete_region_edges(Compound_region* r)
{ 
   Op_inedges inei ;
   Op_outedges outei ;

   for (Region_all_ops opi(r) ; opi != 0 ; opi++) {
      Op* cur_op = *opi ;

      outei(cur_op) ;
      while(outei != 0) {
         Edge* edge = *outei;
         outei++ ;
         if (edge->is_seq()) continue ;
         if (region_contains_edge(r, edge)) {
            Op* dest = edge->dest();

            if (!is_pseudo(cur_op) && !is_pseudo(dest)) continue;
            
            cur_op->remove_outedge(edge, edge->src_port_type(),
               edge->src_port(), edge->src_alt());
            dest->remove_inedge(edge, edge->dest_port_type(),
               edge->dest_port(), edge->dest_alt());
            delete edge;

         }
      }
   }
   delete_region_sw_mg_nodes (r);
}

// ----------------------------------------------------------------------------
// This is the main driver for modulo scheduler and rotating register allocator
// ----------------------------------------------------------------------------
void modulo_scheduler_driver(LoopBody* loop, Cluster_mgr& cmgr)
{
    // Check that it is a counted loop; otherwise return.
    Region_exit_ops exo(loop);
    if (!is_brlc(*exo)) return;

    // Create an enclosing region for loop, preloop and postloop
    Compound_region* ppl = msched_construct_ppl_region(loop);

    // Hack to get around inability to analyze modulo-scheduled loops.  Compute
    // dataflow information and save it as an attribute.  Future dataflow
    // queries will get info from this attribute.  -KF 2/2005
    Msched_attr* ms_attr = new Msched_attr;
    assert(ms_attr && "no more memory");

    el_flow_compute_four_dataflow_sets(loop->kernel(), ANALYZE_ALL,
                                       ms_attr->liveness_gen,
                                       ms_attr->liveness_kill,
                                       ms_attr->down_exposed_use_gen,
                                       ms_attr->down_exposed_use_kill,
                                       ms_attr->up_exposed_def_gen,
                                       ms_attr->up_exposed_def_kill,
                                       ms_attr->down_exposed_def_gen,
                                       ms_attr->down_exposed_def_kill);
    set_msched_attr(loop, ms_attr);

    // Insert Brtop and staging predicate in loop. 
    msched_preprocess(loop);

    if (El_do_width_analysis) {
      cdbg << "Doing bitwidth analysis" << endl;
      do_width_analysis(loop->kernel(), true);
    }

    // cluster assignment - KF 8-2001

    // First save the pre-clustered state, so it can be restored if
    // the loop needs unrolling for modulo variable expansion.
    Hash_map<Op*, int> pre_clst_op_map(hash_op_ptr, 1);
    if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
      pre_clst_op_map = cmgr.op_map;
    }
    cmgr.do_block_clustering(loop->kernel(), El_cluster_method);

    // Assign physical files and bind them
    Codegen::do_prepass_post_cluster_codegen(loop->kernel(), cmgr);

    // Convert to DSA form (needs an updated PQS)
    if (dbg(status,2))
      cdbg << "DSA conversion, Flow Analysis, Edge Drawing for the loop " 
	   << loop->kernel()->id() << endl;
    create_local_analysis_info_for_all_hbs_bbs(loop);
    el_swp_dsa_convert_loop(loop);

    if (dbg(ms,1)) {
      cdbg << "After remap insertion" << endl;
      cdbg << *loop;
    }

    // Reanalyze the loop, preloop, and postloop
    create_local_analysis_info_for_all_hbs_bbs(ppl);
    el_flow_compute_four_dataflow_sets(ppl);

    if (dbg(ms,1)) {
      Edge* cf_edge = (loop->controlFlowBackEdges())->head();
      cdbg << "Liveness on backedge: \n" << *get_liveness_info(cf_edge) << endl;
      cdbg << "Down exposed uses : \n" << *get_down_exposed_uses_info(cf_edge) << endl;
      cdbg << "Up exposed defs : \n" << *get_up_exposed_defs_info(cf_edge) << endl;
      cdbg << "Down exposed defs : \n" << *get_down_exposed_defs_info(cf_edge) << endl;
    }


    // Construct dependence graph
    insert_region_modulo_edges(loop);
    if (dbg(ms)) el_dot_display_dfg(loop);

    // Do the actual scheduling and allocation
    modulo_schedule_and_allocate(loop, cmgr, pre_clst_op_map, El_ii);

    // Insert esc set, predicate clear and set in preloop  
    // and register copies in preloop and postloop
    msched_postprocess(loop, cmgr);

    // Update the liveness information 
    msched_map_liveness_info(loop);
    msched_delete_region_attributes(loop);
    
    delete_region_edges(loop);
    el_swp_strip_remaps(loop);
    el_flow_delete_three_dataflow_sets(ppl);
    El_remove_region(ppl, true);
    
    // Update the local analysis information for pre and post loop
    create_local_analysis_info(loop->preloop());
    create_local_analysis_info(loop->postloop());
}

// ---------------------------------------------------------------------------
// The actual modulo scheduling and rotating register allocation
// ---------------------------------------------------------------------------
bool modulo_schedule_and_allocate(LoopBody *loop, Cluster_mgr& cmgr,
				  Hash_map<Op*, int>& pre_clst_op_map, int desired_II)
{
  if (dbg(status,2))
    cdbg << "Doing Modulo Scheduling and Rot Reg Allocation" << endl;

  if (dbg(status, 1)) {
    int id = loop->kernel()->id();
    cdbg << (loop->kernel()->is_bb() ? "(ms)bb: " : "(ms)hb: ") << id << endl;
  }

  int budget_ratio = El_budget_ratio;
  bool sched_success = false;
  el_swp_map_vr_to_rr_file(loop);
  ModuloScheduler msched(loop);
  msched_glb = &msched;
  if (dbg(ms)) el_dot_display_dfg(loop);

  int II;
  if (desired_II == 0)
    II = msched.MII();
  else
    II = desired_II;

  // this loop can potentially iterate more than once only if El_bump_ii is true.
  for (; II < msched.Numnodes()*MDES_max_op_lat; II++) {

    if(msched.schedule(II, budget_ratio, 
		       (El_ssched_meld_model == NO_MELD ? false : true))) {
      sched_success = true;
      // Stage Scheduling
      if (El_do_stage_scheduling) {
	SS_stage_sched stage(loop, II);
	stage.transform_default();
      }

      // recalculate ESC because stage sched may change it
      msched.calculate_esc();

      // Register Allocation
      RR_allocation rr_alloc(loop, El_lreg_rr_open_loop);
      if(El_msched_allocator == ROTATING_REGISTERS) {
        bool alloc_success = rr_alloc.allocate_default(II, msched.ESC());
        if (!alloc_success) {
          // Try bumping II rather than punting.  -KF 2/2007
          if (El_bump_ii) {
            El_warn("Rotating register allocation FAILED.  Trying next II %d", (II+1));
            sched_success = false;
            continue;
          } else
            El_punt("Rotating register allocation FAILED at II %d", II);
        }
        loop->set_flag(EL_REGION_ROT_REG_ALLOCATED);
        loop->kernel()->set_flag(EL_REGION_ROT_REG_ALLOCATED);
        // commit schedule
        rr_alloc.commit();
        // meld info is computed before sched commit in order to use 
        // full iteration sched times
        if (El_ssched_meld_model != NO_MELD) msched.attach_meld_info(rr_alloc);
      } else if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
	Mod_var_expansion mve(loop);
	El_Stopwatch mve_clock;
        mve_clock.reset();
	mve_clock.start();
	mve.expand(msched, cmgr, pre_clst_op_map);
	mve_clock.stop();
	cdbg << "msched: " << loop->id() << " MVE " << setprecision(10)
	     << mve_clock.total_elapsed() << endl;
      } else {
	cdbg << "Both rotating register allocation and modulo variable expansion\n"
	     << "are turned off! Simu can't handle virtual rotating registers,\n"
	     << "so you need to turn one of the two back on, or fix simu.\n";
	assert(0);
      }

      if (dbg(status,2)) {
	cdbg << "After successful schedule: RecMII = " << msched.RecMII()
	     << " ResMII = " << msched.ResMII() << " II = " << II 
	     << " ESC = " << msched.ESC() << endl;  
	if (dbg(ms, 2)) cdbg <<  *loop;
      }

      msched.commit();

      // output schedule
      if (dbg(status, 3)) {
	cdbg << "Time\t| Compiler Op\t| Sched Op" << endl;
	cdbg << "----------------------------------------" << endl;
	Real_op_filter real_op_filter;
	for (Region_all_ops ops(loop->kernel(), &real_op_filter); 
	     ops != 0; ops++) {
	  Op* op = *ops;
	  int* time = (int*)get_generic_attribute(op, "unrolled_sched_time");
	  cdbg << *time << "\t| op " << op->id() << " \t| " 
	       << op->sched_opcode() << endl;
	}
      }

      if(El_msched_allocator == ROTATING_REGISTERS) {
	// compute and adjust live reg info
	Map<Operand, int>* inmap = new Map<Operand, int>;
	Map<Operand, int>* outmap = new Map<Operand, int>;
	rr_alloc.get_live_register_allocation(*inmap, *outmap);
	if (dbg(ms,1)) {
	  cdbg << "Loop live-in map:\n" << *inmap;
	  cdbg << "Loop live-out map:\n" << *outmap;
	}
	set_generic_attribute(loop, "Entry_alloc_map", (void*)inmap);
	set_generic_attribute(loop, "Exit_alloc_map", (void*)outmap);
      }
      break;
    }
    else if (El_bump_ii) { 
      if (dbg(ms,1) || dbg(status,2))
        cdbg << "MS FAILED Trying next II " << (II+1) << endl;
    } else {
      El_punt("MS FAILED at II %d", II);
    }
  }
  //cdbg << endl;
  //cdbg <<  "FINAL SCHEDULE: " << endl; 

  //cdbg << "ResMII: " << msched.ResMII() << " RecMII:" << 
  //     msched.RecMII() << " MII: " << msched.MII() << " II: " << 
  //     msched.II() <<  " Numnodes: " << msched.Numnodes() << endl;

  //RU_print_map(stderr);
  //cdbg << endl;

  return (sched_success);
}


