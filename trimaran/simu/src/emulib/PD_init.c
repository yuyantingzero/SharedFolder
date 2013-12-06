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
 * File:    PD_init.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: initialize internal state
 *****************************************************************************/


#include "PD_init.h"
#include <stdlib.h>
#include <stdio.h>
#include "PD_error.h"
#include "PD_reg.h"
#include "PD_queue.h"
#include "PD_stack.h"
#include "PD_parms.h"
#include "PD_stats.h"
#include "PD_trace.h"
#include "PD_signals.h"
#include "PD_kernel.h"
#include "PD_timer.h"
#ifdef __PD_CACHE_
#  include "PD_cache.h"
#endif /* __PD_CACHE_ */

/* RMR { scratch memory for scalar-vector operand transport */
int __PD_VEC_SCRATCH_MEMORY[1024] = {0};
/* } RMR */


/* disambiguation tools - defined in PD_ops.list */
extern void __PD_initialize_LDS();


/* flag simulator as initialized */
bool __PD_initialized = __PD_false;


/* initialize the simulator's internal state */
void __PD_initialize()
{
  /* write dynamic stats files if required  */
  __PD_write_stats |= __PD_parameters->flags & __PD_DYNSTATS;

  /* the first initialization pass may not initialize the trace stream
   * since it is possible to subset the functions that are traced
   */
#ifdef __PD_TRACE_
  /* create dynamic trace output file if required */
  if (__PD_parameters->flags & (__PD_CFGTRACE | __PD_MEMTRACE | __PD_PER_OP_VALUE_TRACE)) {
    if (__PD_trace_file == NULL) {
      __PD_trace_file = (void*) fopen(__PD_trace_file_name, "w+");
    }

    /* the trace can only be printed in one format - either verbose or binary */
    if (__PD_initialized) {
	if (__PD_binary_trace_format != (__PD_parameters->flags & __PD_BINTRACE)) {
	  __PD_warn("Conflicting trace formats specified!");
	}
    }
  }
#endif /* __PD_TRACE_ */
  
  if (__PD_initialized) return;

  /* start simulator timer */
  __PD_start_timer();

  /* cleanup simulator at exit */
  atexit(__PD_atexit);

#ifdef __PD_TRACE_
  __PD_binary_trace_format = __PD_parameters->flags & __PD_BINTRACE;
#endif /* __PD_TRACE_ */

  /* register file must have created already */
  __PD_assert(__PD_registers);
  
  /* create macro register file */
  __PD_macro_file = (__PD_reg*) calloc(__PD_NUM_MACRO_REGISTERS, sizeof(__PD_reg));
  
  /* exception handling */
  __PD_initialize_signal_handler(); 
  /* action and data queue */
  __PD_initialize_queue();
  /* speculative load table */
  __PD_initialize_LDS();
  /* initialize internal stack */
  __PD_initialize_stack();

  /* initialize the memory hiearrchy simulator */
#ifdef __PD_CACHE_
  __PD_cache_initialize();
#endif /* __PD_CACHE_ */

  /* initialize RRB */
  __PD_RRB_REG = 0;
  /* RMR { support vector rotating files */
  __PD_VRRB_REG = 0;
  /* } RMR */

  /* initialize bit buckets */
  __PD_BLACK_HOLE_REG = 0;
  CAST(int_zero_file)[int_zero_reg_num].reg.gpr   = 0;
  CAST(flt_zero_file)[flt_zero_reg_num].reg.fpr_S   = 0;
  CAST(dbl_zero_file)[dbl_zero_reg_num].reg.fpr_D   = 0;
  CAST(flt_one_file)[flt_one_reg_num].reg.fpr_S   = 1.0;
  CAST(dbl_one_file)[dbl_one_reg_num].reg.fpr_D   = 1.0;
  __PD_PRED_F_REG     = 0;
  __PD_PRED_T_REG     = 1;
  /* RMR { add default all true vector mask */
  __PD_VEC_MASK_T_REG = 0xFFFFFFFF;
  /* } RMR */
  /* RMR { scratch vector register for operand transport */
  __PD_VEC_SCRATCH_REG  = (int) &__PD_VEC_SCRATCH_MEMORY[0];
  /* } RMR */

  /* initialize simulation-done flag */
  __PD_done = __PD_false;

  /* mark simulator as initialized */
  __PD_initialized = __PD_true;
}

#include "PD_debug.h"

/* make the debug functions available for debugging */
void* __PD_debug_functions[] = {__opid, __srcoper, __destoper, __srcreg,
                                __destreg, __sgpr, __rgpr, __sfpr, __rfpr,
                                __btr, __spr, __rpr, __scr, __rcr};
