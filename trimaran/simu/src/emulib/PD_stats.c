/******************************************************************************
LICENSE NOTICE
--------------

IT IS  A BREACH OF  THE LICENSE AGREEMENT  TO REMOVE THIS  NOTICE FROM
THIS  FILE  OR SOFTWARE  OR  ANY MODIFIED  VERSIONS  OF  THIS FILE  OR
SOFTWARE.

Copyright notices/Licensor(s) Identification
--------------------------------------------
Each of  the entity(ies) whose name properly  appear immediately below
in connection with a copyright notice is a Licensor(s) under the terms
that follow.

Copyright  1998  New  York  University.  All rights  reserved  by  the
foregoing, respectively.

Copyright 2001 Center for  Research on Embedded Systems and Technology
at the  Georgia Institute  of Technology. All  rights reserved  by the
foregoing, respectively.

License agreement
-----------------

The  code contained  in this  file  including both  binary and  source
(hereafter,  Software)  is subject  to  copyright  by Licensor(s)  and
ownership remains with Licensor(s).

Licensor(s)  grants you  (hereafter, Licensee)  a license  to  use the
Software for  academic, research and internal  business purposes only,
without  a  fee.  "Internal  business  use"  means  that Licensee  may
install, use and execute the Software for the purpose of designing and
evaluating products.   Licensee may also disclose  results obtained by
executing  the  Software,  as  well as  algorithms  embodied  therein.
Licensee may  distribute the Software  to third parties  provided that
the copyright notice and this statement appears on all copies and that
no  charge  is  associated  with  such copies.   No  patent  or  other
intellectual property license is granted or implied by this Agreement,
and this  Agreement does not  license any acts except  those expressly
recited.

Licensee may  make derivative works,  which shall also be  governed by
the  terms of  this  License Agreement.  If  Licensee distributes  any
derivative work based  on or derived from the  Software, then Licensee
will abide by the following terms.  Both Licensee and Licensor(s) will
be  considered joint  owners of  such derivative  work  and considered
Licensor(s) for  the purpose of distribution of  such derivative work.
Licensee shall  not modify this  agreement except that  Licensee shall
clearly  indicate  that  this  is  a  derivative  work  by  adding  an
additional copyright notice in  the form "Copyright <year> <Owner>" to
other copyright notices above, before the line "All rights reserved by
the foregoing, respectively".   A party who is not  an original author
of such derivative works within  the meaning of US Copyright Law shall
not modify or add his name to the copyright notices above.

Any Licensee  wishing to  make commercial use  of the  Software should
contact each and every Licensor(s) to negotiate an appropriate license
for  such  commercial  use;  permission  of all  Licensor(s)  will  be
required for such a  license.  Commercial use includes (1) integration
of all or part  of the source code into a product  for sale or license
by or on  behalf of Licensee to third parties,  or (2) distribution of
the Software  to third  parties that need  it to utilize  a commercial
product sold or licensed by or on behalf of Licensee.

LICENSOR (S)  MAKES NO REPRESENTATIONS  ABOUT THE SUITABILITY  OF THIS
SOFTWARE FOR ANY  PURPOSE.  IT IS PROVIDED "AS  IS" WITHOUT EXPRESS OR
IMPLIED WARRANTY.   LICENSOR (S) SHALL  NOT BE LIABLE FOR  ANY DAMAGES
SUFFERED BY THE USERS OF THIS SOFTWARE.

IN NO EVENT UNLESS REQUIRED BY  APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY  COPYRIGHT HOLDER, OR ANY  OTHER PARTY WHO  MAY MODIFY AND/OR
REDISTRIBUTE THE  PROGRAM AS PERMITTED  ABOVE, BE LIABLE  FOR DAMAGES,
INCLUDING  ANY GENERAL, SPECIAL,  INCIDENTAL OR  CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OR  INABILITY TO USE THE PROGRAM (INCLUDING BUT
NOT  LIMITED TO  LOSS OF  DATA OR  DATA BEING  RENDERED  INACCURATE OR
LOSSES SUSTAINED BY  YOU OR THIRD PARTIES OR A  FAILURE OF THE PROGRAM
TO  OPERATE WITH ANY  OTHER PROGRAMS),  EVEN IF  SUCH HOLDER  OR OTHER
PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

By using  or copying  this Software, Licensee  agrees to abide  by the
copyright law and all other applicable laws of the U.S., and the terms
of  this license  agreement. Any  individual Licensor  shall  have the
right  to terminate this  license immediately  by written  notice upon
Licensee's   breach   of,  or   non-compliance   with,   any  of   its
terms.  Licensee may  be held  legally responsible  for  any copyright
infringement  that is caused  or encouraged  by Licensee's  failure to
abide     by    the     terms    of     this     license    agreement.
******************************************************************************/

/******************************************************************************
 * File:    PD_stats.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: maintains statistics per function/control block
 *****************************************************************************/


#include "PD_stats.h"
#include <stdlib.h>
#include <stdio.h>
#include "PD_error.h"
#include "PD_parms.h"
#include "PD_trace.h"


#define div(x)    (x ? (double) x : 1.0)


__PD_stats* __PD_statistics      = NULL;
char*       __PD_function_name   = NULL;
bool        __PD_write_stats     = __PD_false;


#define div(x)    (x ? (double) x : 1.0)

static const char* stats_filename      = "PD_STATS";
static const char* stats_filename_ops  = "PD_STATS_OPS";
static const char* stats_filename_ldst = "PD_STATS_LDST";

static void*       stats_file      = NULL;
static void*       stats_file_ops  = NULL;
static void*       stats_file_ldst = NULL;

/* maintain a linked list of statistic nodes */
static __PD_stats_node* stats_head_node = NULL;

/* complete program stats */
static __PD_stats       aggregate_pd_stats = {0};
static __PD_stats*      pd_stats = &aggregate_pd_stats;

static void initialize_file_streams();
static void close_file_streams();
static void accumulate_cb_stats(__PD_stats*, __PD_stats*);
static void collect_and_print_subregion_stats(__PD_stats*);
static void print_cb_summary(__PD_stats*, char* label);
static void print_ops_summary(__PD_stats*, char* label);
static void print_region_summary(__PD_stats*, char* simulation_time);
static void print_load_store_summary();


/* procedure entry */
void __PD_start_procedure(__PD_OP* op)
{
  if (__PD_parameters->flags & __PD_DYNSTATS) {
    __PD_statistics = (__PD_stats*) LIT(OP_SRC1(op));
    __PD_assert(__PD_statistics->is_procedure);
    __PD_statistics->freq++;
  }
}


/* control block entry */
void __PD_start_block(__PD_OP* op) 
{
#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_CFGTRACE) {
    __PD_trace_cb_entry(op);
  }
#endif /* __PD_TRACE_ */
  
  if (__PD_parameters->flags & __PD_DYNSTATS) {
    __PD_statistics = (__PD_stats*) LIT(OP_SRC1(op));
    __PD_assert(!__PD_statistics->is_procedure);
    __PD_statistics->freq++;
  }
}


/* operation is squashed because of a false predicate */
void __PD_no_op(__PD_OP* op)
{
#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_CFGTRACE) {
    __PD_trace_op_null(op);
  }
#endif /* __PD_TRACE_ */

  if (__PD_parameters->flags & __PD_DYNSTATS) {
    __PD_assert(!__PD_statistics->is_procedure);
    __PD_statistics->nullified++;
#ifdef __PD_TRACK_OP_STATS_
    __PD_statistics->ops[OP_BBIDX(op)].pred_squashed++;
#endif /* __PD_TRACK_OP_STATS_ */
  }
}


/* track  simulation statistics  - note  that predicate  spill operations
 * such  as  RESTORE  and  SAVE  are  accounted  for  in  the  calculated
 * statistics but are not part of the generated trace
 */
void __PD_update_stats(__PD_OP* op)
{
  /* check PD_globals.h for mask settings */
  ushort category = (OP_FLAGS(op) & __PD_OPTYPES);
  ushort class    = (OP_FLAGS(op) >> 4) & __PD_OPCLASS;

  __PD_assert(!__PD_statistics->is_procedure);

  __PD_statistics->dynstats[category - 1]++;
  __PD_statistics->opclass [class    - 1]++;

#ifdef __PD_TRACK_OP_STATS_
  if (__PD_statistics->ops[OP_BBIDX(op)].op_node == NULL) {
    __PD_statistics->ops[OP_BBIDX(op)].op_node = op;
  }
  __PD_statistics->ops[OP_BBIDX(op)].instances++;
#endif /* __PD_TRACK_OP_STATS_ */
}


/* chains all the statistics tables */
void __PD_chain(__PD_stats* link)
{
  __PD_stats_node* temp;
  __PD_stats_node* ptr;
  
  /* this should be a hash set for efficiency */
  for (ptr = stats_head_node; ptr != 0 ; ptr = ptr->next) {
    /* already bound */
    if (ptr->link == link) return;
  }
  
  /* new procedure, bind entry */
  temp = (__PD_stats_node*) malloc(sizeof(__PD_stats_node));
  temp->link = link;
  temp->next = stats_head_node;
  stats_head_node = temp;
}


/* write run-time statistics to file */
void __PD_commit_stats(char* simulation_time)
{
  __PD_stats_node* ptr;
  __PD_stats* fn_stats;

  initialize_file_streams();
  
  /* one iteration per function */
  for (ptr = stats_head_node; ptr != 0 ; ptr = ptr->next) {
    fn_stats = ptr->link;
    fprintf(stats_file, "Function %s {\n", fn_stats->id);
#ifdef __PD_TRACK_OP_STATS_
    fprintf(stats_file_ops, "Function %s {\n", fn_stats->id);
#endif /* __PD_TRACK_OP_STATS_ */
    /* collect subregion statistics */
    collect_and_print_subregion_stats(fn_stats);
    print_region_summary(fn_stats, NULL);
    fprintf(stats_file, "}\n\n");
#ifdef __PD_TRACK_OP_STATS_
    fprintf(stats_file_ops, "}\n\n");
#endif /* __PD_TRACK_OP_STATS_ */
  }

  /* RMR { add a new line after Totals */
  fprintf(stats_file, "Totals {\n");
  /* } RMR */
  pd_stats->is_procedure = __PD_true;
  print_region_summary(pd_stats, simulation_time);
  fprintf(stats_file, "}\n\n");

  /* print out load hit/miss profile */
#ifdef __PD_TRACK_OP_STATS_
  /* print out load hit/miss profile */
  print_load_store_summary();
#endif /* __PD_TRACK_OP_STATS_ */

  close_file_streams();
}


/* collect statistics per control block */
static void collect_and_print_subregion_stats(__PD_stats* fn_stats)
{
  __PD_stats* cb_stats;
  
  for (cb_stats = fn_stats + 1; cb_stats->id != NULL; cb_stats++) {   
    accumulate_cb_stats(fn_stats, cb_stats);
    /* RMR { do not print stats per block since some of the fields are
     * currently not tracked or updated by M5e
     */
    // print_cb_summary(cb_stats, fn_stats->id);
    /* } RMR */
  }
}


static void accumulate_cb_stats(__PD_stats* fn_stats, __PD_stats* stats)
{
  int i;

  /* the dynamic part */
  for (i = 0; i < __PD_OPTYPES; i++) {
    pd_stats->dynstats[i] += stats->dynstats[i];
    fn_stats->dynstats[i] += stats->dynstats[i];
  }
  for (i = 0; i < __PD_OPCLASS; i++) {
    pd_stats->opclass[i] += stats->opclass[i];
    fn_stats->opclass[i] += stats->opclass[i];
  }

  pd_stats->nullified   += stats->nullified;
  fn_stats->nullified   += stats->nullified;

  pd_stats->speculated  += stats->speculated;
  fn_stats->speculated  += stats->speculated;

  pd_stats->notrapping  += stats->notrapping;
  fn_stats->notrapping  += stats->notrapping;

  pd_stats->spec_except += stats->spec_except;
  fn_stats->spec_except += stats->spec_except;
#ifdef __PD_FULL_SPECULATION_SUPPORT_
  pd_stats->spec_squash += stats->spec_squash;
  fn_stats->spec_squash += stats->spec_squash;
#endif /* __PD_FULL_SPECULATION_SUPPORT_ */
  pd_stats->compute_cycles += stats->compute_cycles;
  fn_stats->compute_cycles += stats->compute_cycles;

  pd_stats->stall_cycles  += stats->stall_cycles;
  fn_stats->stall_cycles  += stats->stall_cycles;

  pd_stats->use_stalls    += stats->use_stalls;
  fn_stats->use_stalls    += stats->use_stalls;

  /* the static part */
  pd_stats->static_branch += stats->static_branch;
  fn_stats->static_branch += stats->static_branch;

  pd_stats->static_load   += stats->static_load;
  fn_stats->static_load   += stats->static_load;

  pd_stats->static_store  += stats->static_store;
  fn_stats->static_store  += stats->static_store;

  pd_stats->static_ialu   += stats->static_ialu;
  fn_stats->static_ialu   += stats->static_ialu;

  pd_stats->static_falu   += stats->static_falu;
  fn_stats->static_falu   += stats->static_falu;

  pd_stats->static_cmpp   += stats->static_cmpp;
  fn_stats->static_cmpp   += stats->static_cmpp;

  pd_stats->static_pbr    += stats->static_pbr;
  fn_stats->static_pbr    += stats->static_pbr;

  pd_stats->static_icm    += stats->static_icm;
  fn_stats->static_icm    += stats->static_icm;

  pd_stats->static_vialu  += stats->static_vialu;
  fn_stats->static_vialu  += stats->static_vialu;

  pd_stats->static_vfalu  += stats->static_vfalu;
  fn_stats->static_vfalu  += stats->static_vfalu;

  pd_stats->static_vload  += stats->static_vload;
  fn_stats->static_vload  += stats->static_vload;

  pd_stats->static_vstore += stats->static_vstore;
  fn_stats->static_vstore += stats->static_vstore;

  pd_stats->static_vxfr   += stats->static_vxfr;
  fn_stats->static_vxfr   += stats->static_vxfr;

  pd_stats->static_vprm   += stats->static_vprm;
  fn_stats->static_vprm   += stats->static_vprm;

  pd_stats->static_ops    += stats->static_ops;
  fn_stats->static_ops    += stats->static_ops;

  pd_stats->static_caller_spill += stats->static_caller_spill;
  fn_stats->static_caller_spill += stats->static_caller_spill;

  pd_stats->static_callee_spill += stats->static_callee_spill;
  fn_stats->static_callee_spill += stats->static_callee_spill;
}


/* print statistics to file */
static void print_cb_summary(__PD_stats* stats, char* label)
{
  void* outbuf = stats_file;

  __PD_assert(!stats->is_procedure);

  if (stats->flagme) {
    __PD_assert(label);
    fprintf(outbuf, "!%s", label);
  }

  fprintf(outbuf, "\t%-5s\t%5u\t%5u\t%10llu\t%10llu\t%10llu\t%10llu\t%10llu\t%10llu\t%10llu\n",
	    stats->id,
	    stats->static_ops,
	    stats->sched_len,
	    stats->compute_cycles,
	    stats->stall_cycles,
	    stats->branch_mispredicts,
	    stats->branch_stalls,
	    stats->icache_stalls,
	    stats->wb_stalls,
	    stats->use_stalls);

#ifdef __PD_TRACK_OP_STATS_
  print_ops_summary(stats, label);
#endif /* __PD_TRACK_OP_STATS_ */
}


static void print_ops_summary(__PD_stats* stats, char* label)
{  
  int i;

  void*     outbuf = stats_file_ops;
  u64bitint stalls = 0;

  __PD_assert(!stats->is_procedure);

  for (i = 1; i <= stats->static_ops; i++) {
    /* sanity checks */
    __PD_assert((stats->ops[i].op_node && stats->ops[i].instances) || 
		    (!stats->ops[i].op_node && !stats->ops[i].instances));
    /* ignore untouched ops */
    if (stats->ops[i].op_node == NULL) continue;
    
    if (stats->flagme) {
	__PD_assert(label);
	fprintf(outbuf, "!%s", label);
    }

    stalls = stats->ops[i].branch_stalls    + stats->ops[i].icache_stalls +
             stats->ops[i].taken_use_stalls + stats->ops[i].wb_stalls;
    
    fprintf(outbuf, 
		"\t"
		"op-%-5lu\t"                          /* 1 */
		"%10llu\t"                            /* 2 */
		"%10llu\t"                            /* 3 */
		"%10llu\t"                            /* 4 */
		"%10llu\t"                            /* 5 */
		// "%7llu\t"                             /* 6 */
		// "%7llu\t"                             /* 7 */
		"%10llu\n",                           /* 8 */
		// "%7llu\t"                             /* 9 */
		// "%7llu\t",                            /* 0 */

		OP_ID(stats->ops[i].op_node),         /* 1 */
		stats->ops[i].instances,              /* 2 */
		stalls,                               /* 3 */
		stats->ops[i].pred_squashed,          /* 4 */
		stats->ops[i].spec_squashed,          /* 5 */
		// stats->ops[i].branch_stalls,          /* 6 */
		// stats->ops[i].icache_stalls,          /* 7 */
		stats->ops[i].taken_use_stalls);      /* 8 */
		// stats->ops[i].caused_use_stalls,     /* 9 */
		// stats->ops[i].wb_stalls,             /* 0 */
		// stats->ops[i].branch_mispredicts,
		// stats->ops[i].itlb_misses,
		// stats->ops[i].l1i_misses,
		// stats->ops[i].l2i_misses,
		// stats->ops[i].dtlb_misses,
		// stats->ops[i].l1d_misses,
		// stats->ops[i].l2d_misses);
  }
}


static void print_region_summary(__PD_stats* stats, char* simulation_time)
{
  int i;

  void*     outbuf      = stats_file;
  u64bitint total_ops   = 0;
  u64bitint d_spill_ops = 0;
  u64bitint s_spill_ops = 0;

  /* RMR { do not print extra new line since not printing stats per block */
  // fprintf(outbuf, "\n");
  /* } RMR */

  fprintf(outbuf, "\ttotal_cycles...................................................%-10llu\n",
	    stats->compute_cycles + stats->stall_cycles);
  fprintf(outbuf, "\tcompute_cycles.................................................%-10llu (%6.2f)\n",
	    stats->compute_cycles, 100 * stats->compute_cycles / (double) (stats->compute_cycles + stats->stall_cycles));
  fprintf(outbuf, "\tstall_cycles...................................................%-10llu (%6.2f)\n",
	    stats->stall_cycles,   100 * stats->stall_cycles   / (double) (stats->compute_cycles + stats->stall_cycles));
  
  /* dynamic and static instruction mix */
  for (i = 0; i < __PD_OPCLASS; i++) {
    total_ops += stats->opclass[i];
  }
  
  if (total_ops == 0) total_ops = 1;
  __PD_assert(stats->static_ops);
  
  /* register allocation overhead */
  d_spill_ops = stats->opclass[__PD_CLRINDEX] + stats->opclass[__PD_CLEINDEX];
  if (d_spill_ops == 0) d_spill_ops = 1;

  s_spill_ops = stats->static_caller_spill + stats->static_callee_spill;
  if (s_spill_ops == 0) s_spill_ops = 1;

  fprintf(outbuf, "\n");
  fprintf(outbuf, "\ttotal_dynamic_operations.......................................%-10llu\n", 
	    total_ops);

  fprintf(outbuf, "\ttotal_static_operations........................................%-10u\n", 
	    stats->static_ops);

  fprintf(outbuf, "\ttotal_committed_operations.....................................%-10llu (%6.2f)\n", 
	    total_ops - stats->nullified, 
	    100 * (total_ops - stats->nullified) / (double) total_ops);

  fprintf(outbuf, "\ttotal_speculated_operations....................................%-10llu (%6.2f)\n", 
	    stats->speculated,
	    100 * (stats->speculated) / (double) total_ops);

  fprintf(outbuf, "\ttotal_notrapping_operations....................................%-10llu (%6.2f)\n", 
	    stats->notrapping,
	    100 * (stats->notrapping) / (double) total_ops);

  fprintf(outbuf, "\ttotal_exceptions_encountered...................................%-10llu (%6.2f)\n",
	    stats->spec_except, 
	    100 * stats->spec_except / (double) total_ops);

#ifdef __PD_FULL_SPECULATION_SUPPORT_
  fprintf(outbuf, "\ttotal_speculatively_squashed_operations........................%-10llu (%6.2f)\n",
	    stats->spec_squash,
	    100 * stats->spec_squash / (double) total_ops);
#endif /* __PD_FULL_SPECULATION_SUPPORT_ */

  if (simulation_time) {
    fprintf(outbuf, "\tsimulation_time (user / system)................................%-s\n", 
		simulation_time);
  }

  fprintf(outbuf, "\n");
  fprintf(outbuf, "\taverage_issued_ops/total_cycles................................%-6.2f\n",
	    total_ops / (double) (stats->compute_cycles + stats->stall_cycles));
  fprintf(outbuf, "\taverage_issued_ops/compute_cycles..............................%-6.2f\n",
	    total_ops / (double) stats->compute_cycles);

  fprintf(outbuf, "\n");
  fprintf(outbuf, "\tialu:             %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_IA_OP-1], 
	    100 * stats->dynstats[__PD_IA_OP-1] / (double) total_ops,
	    stats->static_ialu, 
	    100 * stats->static_ialu / (double) stats->static_ops);

  fprintf(outbuf, "\tfalu:             %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_FA_OP-1], 
	    100 * stats->dynstats[__PD_FA_OP-1] / (double) total_ops,
	    stats->static_falu, 
	    100 * stats->static_falu / (double) stats->static_ops);

  fprintf(outbuf, "\tload:             %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_LD_OP-1], 
	    100 * stats->dynstats[__PD_LD_OP-1] / (double) total_ops,
	    stats->static_load,  
	    100 * stats->static_load / (double) stats->static_ops);

  fprintf(outbuf, "\tstore:            %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_ST_OP-1], 
	    100 * stats->dynstats[__PD_ST_OP-1] / (double) total_ops,
	    stats->static_store, 
	    100 * stats->static_store / (double) stats->static_ops);

  fprintf(outbuf, "\tcmp:              %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_CP_OP-1], 
	    100 * stats->dynstats[__PD_CP_OP-1] / (double) total_ops,
	    stats->static_cmpp, 
	    100 * stats->static_cmpp / (double) stats->static_ops);

  fprintf(outbuf, "\tpbr:              %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_PB_OP-1], 
	    100 * stats->dynstats[__PD_PB_OP-1] / (double) total_ops,
	    stats->static_pbr,
	    100 * stats->static_pbr / (double) stats->static_ops);

  fprintf(outbuf, "\tbranch:           %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_BR_OP-1], 
	    100 * stats->dynstats[__PD_BR_OP-1] / (double) total_ops,
	    stats->static_branch,
	    100 * stats->static_branch / (double) stats->static_ops);

  fprintf(outbuf, "\ticm:              %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_ICM_OP-1], 
	    100 * stats->dynstats[__PD_ICM_OP-1] / (double) total_ops,
	    stats->static_icm, 
	    100 * stats->static_icm / (double) stats->static_ops);

  /* RMR { vector (SIMD) ops */
  fprintf(outbuf, "\tvialu:            %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_VIA_OP-1], 
	    100 * stats->dynstats[__PD_VIA_OP-1] / (double) total_ops,
	    stats->static_vialu, 
	    100 * stats->static_vialu / (double) stats->static_ops);

  fprintf(outbuf, "\tvfalu:            %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_VFA_OP-1], 
	    100 * stats->dynstats[__PD_VFA_OP-1] / (double) total_ops,
	    stats->static_vfalu, 
	    100 * stats->static_vfalu / (double) stats->static_ops);

  fprintf(outbuf, "\tvload:            %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_VLD_OP-1], 
	    100 * stats->dynstats[__PD_VLD_OP-1] / (double) total_ops,
	    stats->static_vload,  
	    100 * stats->static_vload / (double) stats->static_ops);

  fprintf(outbuf, "\tvstore:           %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_VST_OP-1], 
	    100 * stats->dynstats[__PD_VST_OP-1] / (double) total_ops,
	    stats->static_vstore, 
	    100 * stats->static_vstore / (double) stats->static_ops);

  fprintf(outbuf, "\tvxfr:             %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_VXF_OP-1], 
	    100 * stats->dynstats[__PD_VXF_OP-1] / (double) total_ops,
	    stats->static_vxfr, 
	    100 * stats->static_vxfr / (double) stats->static_ops);

  fprintf(outbuf, "\tvperm:            %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->dynstats[__PD_VPR_OP-1], 
	    100 * stats->dynstats[__PD_VPR_OP-1] / (double) total_ops,
	    stats->static_vprm, 
	    100 * stats->static_vprm / (double) stats->static_ops);
  /* } RMR */

  fprintf(outbuf, "\tspills_restores:  %10llu (%6.2f).....dynamic\t%5llu (%6.2f).....static\n",
	    d_spill_ops, 
	    100 * d_spill_ops / (double) total_ops,
	    s_spill_ops, 
	    100 * s_spill_ops / (double) stats->static_ops);

  fprintf(outbuf, "\tcaller_save:      %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->opclass[__PD_CLRINDEX], 
	    100 * stats->opclass[__PD_CLRINDEX] / (double) d_spill_ops,
	    stats->static_caller_spill, 
	    100 * stats->static_caller_spill / (double) s_spill_ops);

  fprintf(outbuf, "\tcallee_save:      %10llu (%6.2f).....dynamic\t%5u (%6.2f).....static\n",
	    stats->opclass[__PD_CLEINDEX], 
	    100 * stats->opclass[__PD_CLEINDEX] / (double) d_spill_ops,
	    stats->static_callee_spill, 
	    100 * stats->static_callee_spill / (double) s_spill_ops);
}


void print_load_store_summary()
{
  __PD_stats_node* ptr;
  
  void* outbuf = stats_file_ldst;

  for (ptr = stats_head_node; ptr != 0 ; ptr = ptr->next) {
    __PD_stats* stats;
    __PD_stats* fn_stats = ptr->link;
    
    fprintf(outbuf, "%s {\n", fn_stats->id);
    
    for (stats = fn_stats + 1; stats->id != NULL; stats++) {   
	ulong i;

	for (i = 1; i <= stats->static_ops; i++) {
	  /* sanity checks */
	  __PD_assert((stats->ops[i].op_node && stats->ops[i].instances) || 
			  (!stats->ops[i].op_node && !stats->ops[i].instances));
	  /* ignore untouched ops */
	  if (stats->ops[i].op_node == NULL) continue;
	  if ((OP_FLAGS(stats->ops[i].op_node) & 0x07) != __PD_LD_OP) continue;
	  
	  fprintf(outbuf, 
		    "\t[%s\t%-5lu\t%-8llu\t%8llu\t%8llu\t%6.2f\t%6.2f\t%.5f]\n",
		    fn_stats->id,
		    OP_ID(stats->ops[i].op_node),
		    stats->ops[i].instances,
		    stats->ops[i].caused_use_stalls,
		    stats->ops[i].l1d_misses + stats->ops[i].l2d_misses,
		    stats->ops[i].caused_use_stalls / div(stats->ops[i].instances),
		    stats->ops[i].caused_use_stalls / div(stats->ops[i].l1d_misses + stats->ops[i].l2d_misses),
		    stats->ops[i].caused_use_stalls / div(pd_stats->use_stalls));
	}
    }
    
    fprintf(outbuf, "}\n");
  }
}


void initialize_file_streams()
{
  if (stats_file == NULL) {
    stats_file = (void*) fopen(stats_filename,      "w+");  
  }
  
#ifdef __PD_TRACK_OP_STATS_
  if (stats_file_ops == NULL) {
    stats_file_ops  = (void*) fopen(stats_filename_ops,  "w+");
    
    fprintf(stats_file_ops,
		"#\t"
		"%-9s\t"   /* 1 */
		"%10s\t"   /* 2 */
		"%10s\t"   /* 3 */
		"%10s\t"   /* 4 */
		"%10s\t"   /* 5 */
		"%10s\n",  /* 6 */
		
		"op id",   /* 1 */
		"count",   /* 2 */
		"stall",   /* 3 */
		"!pred",   /* 4 */
		"!spec",   /* 5 */
		"intlck"); /* 6 */
  }
  
  if (stats_file_ldst == NULL) {
    stats_file_ldst = (void*) fopen(stats_filename_ldst, "w+");
  }
#endif /* __PD_TRACK_OP_STATS_ */
}


void close_file_streams()
{
  fclose(stats_file);
#ifdef __PD_TRACK_OP_STATS_
  fclose(stats_file_ops);
  fclose(stats_file_ldst);
#endif /* __PD_TRACK_OP_STATS_ */
}
