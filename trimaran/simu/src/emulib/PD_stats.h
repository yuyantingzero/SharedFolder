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
 * File:    PD_stats.h
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: maintains statistics per function/control block
 *****************************************************************************/


#ifndef __PD_STATS_H_
#define __PD_STATS_H_


#include "PD_globals.h"
#include "PD_op.h"


typedef struct __PD_stats      __PD_stats;
typedef struct __PD_opstats    __PD_opstats;
typedef struct __PD_stats_node __PD_stats_node;

/* current function being simulated */
extern char*       __PD_function_name;
/* statistic flags for current function */
extern __PD_stats* __PD_statistics;
/* signal when to write stats to file */
extern bool        __PD_write_stats;

/* update opcode counting */
extern void __PD_update_stats(__PD_OP*);

/* update function statistics and trace function entry */
extern void __PD_start_procedure(__PD_OP*);
/* update block statistics and trace block entry */
extern void __PD_start_block(__PD_OP*);
/* update nullification statistics and trace operation nullification */
extern void __PD_no_op(__PD_OP*);

/* write statistics to file */
extern void __PD_commit_stats(char* simulation_time);

struct __PD_stats
{
  /* Statically defined information */
  char* id;             /* id of this stat block */
  bool  is_procedure;   /* is this a procedure or a bb/hb */
  bool  flagme;         /* flag blocks in stats output file */
  uint  sched_len;      /* scheduling length */

  /* Static instruction mix */
  uint static_ops;      /* number of operations in a block */
  uint static_ialu;
  uint static_falu;
  uint static_load;
  uint static_store;
  uint static_cmpp;
  uint static_pbr;
  uint static_branch;
  uint static_icm;
  /* RMR { vector (SIMD) ops */
  uint static_vialu;
  uint static_vfalu;
  uint static_vload;
  uint static_vstore;
  uint static_vxfr;
  uint static_vprm;
  /* } RMR */
  uint static_caller_spill;
  uint static_callee_spill;

  /* Dynamic instruction mix (same as above) */
  u64bitint dynstats[__PD_OPTYPES]; 
  
  /* Register allocation overhead 
   * not_regalloc, caller_spill, callee_spill
   */  
  u64bitint opclass[__PD_OPCLASS];
  
  /* Total number of nullified operations */
  u64bitint nullified;
  
  /* Total number of speculated operations */
  u64bitint speculated;
  
  /* Total number of notrapping operations */
  u64bitint notrapping;
  
  /* Total number  of exceptions encountered
   * while  executing speculated  operations
   */
  u64bitint spec_except;
  /* Total number of  operations not  issued 
   * because of exception marker on a source 
   * operand 
   */
  u64bitint spec_squash;
  
  /* Overall execution stats */
  u64bitint compute_cycles;
  u64bitint stall_cycles;
  
  /* Stall cycles breakdown */
  u64bitint branch_mispredicts;
  u64bitint branch_stalls;
  u64bitint icache_stalls;
  u64bitint use_stalls;
  u64bitint wb_stalls;
  
  /* Number of visits */
  u64bitint freq;
  
  /* Operation-specific statistics */
  __PD_opstats* ops;
}; /* __PD_stats */


/* operations statistic node */
struct __PD_opstats
{
  __PD_OP*  op_node;
  u64bitint instances;
  
  u64bitint branch_stalls;
  u64bitint icache_stalls;
  u64bitint taken_use_stalls;
  u64bitint caused_use_stalls;
  u64bitint wb_stalls;
  
  u64bitint branch_mispredicts;
  u64bitint itlb_misses;
  u64bitint l1i_misses;
  u64bitint l2i_misses;
  
  u64bitint dtlb_misses;
  u64bitint l1d_misses;
  u64bitint l2d_misses;
  
  u64bitint pred_squashed;
  u64bitint spec_squashed;
}; /* __PD_opstats */


/* linked list of statistic nodes */
struct __PD_stats_node
{
  struct __PD_stats*      link;
  struct __PD_stats_node* next;
}; /* __PD_stats_node */


#endif /* __PD_STATS_H_ */
