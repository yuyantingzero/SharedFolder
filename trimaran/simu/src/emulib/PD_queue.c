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
 * File:    PD_queue.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: action queue
 *****************************************************************************/


#include "PD_queue.h"
#include <stdio.h>
#include <stdio.h>
#include "PD_error.h"
#include "PD_parms.h"
#include "PD_stats.h"
#include "PD_kernel.h"
#include "PD_sl.h"
#ifdef __PD_CACHE_
#  include "PD_cache.h"
#endif /* __PD_CACHE_ */

static ulong __PD_pending = 0;
__PD_call_info __PD_last_call;


__PD_QP __PD_queue  [__PD_QSIZE];
__PD_QR __PD_actions[__PD_QSIZE];


/* initialize queue */
void __PD_initialize_queue() 
{
  uint i;

  for (i = 0; i < __PD_QSIZE; i++) {
    __PD_queue[i].nmoves = __PD_queue[i].nactions = __PD_queue[i].call = 0;
  }
  
  __PD_pending = 0;

  __PD_last_call.address  = 0;
  __PD_last_call.is_valid = __PD_false;
}


/* delay copy until current time + specified latency */
void __PD_delay(void* val, void* reg, void* pending, ubyte lat) 
{
  uint  attime = (__PD_pending + lat - 1) & __PD_QMASK;
  ulong n = __PD_queue[attime].nmoves;

  __PD_assert(lat);
  __PD_debug("\tdelay: *(%p)=%lx until %d\n",
		 (ulong*) reg, *(ulong*) val, attime);

  __PD_actions[attime].moves[n].reg =  (ulong*) reg;
  __PD_actions[attime].moves[n].val = *(ulong*) val;

  __PD_queue  [attime].nmoves++;

  /* for interlocking, mark destination busy */
  /* Only do this if the pending parameter is not NULL.
     PD_mark_dest_regs_available() calls it with NULL during cache simulation.
     mchu 2/07 */
  if(pending != NULL) {
    if(*(ulong*)pending != __PD_REG_BUSY) {
      *(ulong*) pending = __PD_REG_BUSY;
      
      __PD_actions[attime].moves[n+1].reg = (ulong*) pending;
      __PD_actions[attime].moves[n+1].val = __PD_REG_READY;
      
      __PD_queue  [attime].nmoves++;
    }
  }

}


/* delay double copy until current time + specified latency */
void __PD_delay2x(void* val, void* reg, void* pending, ubyte lat) 
{
  uint  attime = (__PD_pending + lat - 1) & __PD_QMASK;
  ulong n = __PD_queue[attime].nmoves;

  __PD_assert(lat);
  __PD_debug("\tdelay2: *(%p)=%lx and *(%p)=%lx until %d\n",
		 (ulong*) reg, *(ulong*) val,
		 (ulong*) reg + 1, *((ulong*) val + 1), attime);
  
  __PD_actions[attime].moves[n].reg =  (ulong*) reg;
  __PD_actions[attime].moves[n].val = *(ulong*) val;
  
  __PD_actions[attime].moves[n+1].reg =   (ulong*) reg + 1;
  __PD_actions[attime].moves[n+1].val = *((ulong*) val + 1);
  
  __PD_queue  [attime].nmoves += 2;
  
  /* for interlocking, mark destination busy */
  if(pending != NULL) {
    if(*(ulong*)pending != __PD_REG_BUSY) {
      *(ulong*) pending = __PD_REG_BUSY;
  
      __PD_actions[attime].moves[n+2].reg = (ulong*) pending;
      __PD_actions[attime].moves[n+2].val = __PD_REG_READY;
      
      __PD_queue  [attime].nmoves++;
    }
  }
}


/* delay a function (vector) call until current time + specified latency */
void __PD_delay_call(__PD_vector target, __PD_vector vector,
			   /* __PD_reg* ret, */ ubyte lat)
{
  uint attime = (__PD_pending + lat - 1) & __PD_QMASK;
  
  if (__PD_queue[attime].call) {
    __PD_punt("Can not have more than one branch operation per instruction!\n");
  }
  
  __PD_assert(lat);
  __PD_debug("\tdelay_call: %p(%p) until %d\n", vector, target, attime);
  
  __PD_actions[attime].call.vec  = vector;
  __PD_actions[attime].call.targ = target;
  //  __PD_actions[attime].call.ret  = ret;
  
  __PD_queue [attime].call = __PD_true;
}


/* dequeue and commit all values and execute all function calls */
void __PD_execute_instructions() 
{
  ulong p;
  bool call;
  __PD_vector vec;
  __PD_vector targ;

  __PD_debug("\texecute_instructions: processing queue at time %ld (%s)\n",
		 __PD_pending, __PD_function_name);

  /* execute move actions */
  for (p = 0; p < __PD_queue[__PD_pending].nmoves; p++) {
    __PD_QM* action = &__PD_actions[__PD_pending].moves[p];

    __PD_debug("\texecute_instructions: %p = %lx\n",
		   action->reg, action->val);
    
    *action->reg = action->val;
  }
  
  /* mark all move actions as committed */
  __PD_queue[__PD_pending].nmoves = 0;
  /* mark all calls as executed -  note that this must be done before
   * the actual function call so that the __PD_pending pointer can be
   * incremented otherwise on reentry the current set of actions will
   * re-executed
   */
  call = __PD_queue[__PD_pending].call;
  vec  = __PD_actions[__PD_pending].call.vec;
  targ = __PD_actions[__PD_pending].call.targ;
  __PD_queue[__PD_pending].call = __PD_false;
  
  /* advance the queue pointer */
  __PD_pending = (__PD_pending + 1) & __PD_QMASK;

  /* execute call actions */
  if (call) {
    /* save PC */
    ulong pc = __PD_PC_REG;
    
    /* used to track where last call originated from */
    __PD_last_call.address  = (ulong) targ;
    __PD_last_call.is_valid = __PD_true;
    
    /* call the function */
    __PD_debug("\texecute_instructions: calling function %p(%p)\n",
		   vec, targ);
    
    if (vec) (*vec)(targ);
    else     (*targ)();
    
    /* if a longjmp is executed, then it has restored the macro
     * registers and set the program counter to the appropriate 
     * value
     */
    if (!__PD_longjmp_executed) {
	__PD_PC_REG = pc;
    }
  }
}


/* advance simulation clock */
void __PD_aclock(__PD_OP* op) 
{
  uint i;
  uint clocks = LIT(OP_SRC0(op));
  
  if (__PD_parameters->flags & __PD_DYNSTATS) {
    __PD_statistics->compute_cycles += clocks;
  }
  
  for (i = 0; i < clocks; i++) {
    /* update the clock */
    __PD_global_clock++;

    /* commit register writes and execute branches */
    __PD_execute_instructions();

#ifdef __PD_CACHE_
    __PD_cache_advance_state_one_step();
#endif /* __PD_CACHE_ */

    /* force an immediate return when a longjmp is exceuted
     * so as not to overwrite the restored registers in the 
     * next call to execute_instructions 
     * note - does the queue need to be cleared since there
     * could be active actions remaining (perhaps the queue
     * pointer shoud be saved in the PD_jumbuf as well)?
     */
    if (__PD_longjmp_executed) return;
  }
}


__PD_reg* __PD_check_interlock(__PD_reg* r)
{
  __PD_OP* op = &(__PD_cur_op_list[__PD_PC_REG]);
  ulong queue_slot = __PD_pending;

  while (r->pending) {
    __PD_debug("\tinterlock stall\n");

    /* update the clock */
    __PD_global_clock++;
    
    /* account for interlock stall */
    if (__PD_parameters->flags & __PD_DYNSTATS) {
	__PD_statistics->stall_cycles++;
#ifdef __PD_TRACK_OP_STATS_
	__PD_statistics->ops[OP_BBIDX(op)].taken_use_stalls++;
#endif /* __PD_TRACK_OP_STATS_ */
    }
    
    /* advance pipeline one step */
    __PD_execute_instructions();
    
    /* check for deadlock (pending is a circular queue) */
    if (queue_slot == __PD_pending) {
	printf("deadlock detected in function %s\n", __PD_function_name);
	printf("operation id is %lu\n", op->rebel_id);
	__PD_assert(queue_slot != __PD_pending);
    }

    assert(!__PD_longjmp_executed);
  }
  return r;
}


/* determine if the last function call is a native or simulator call */
bool __PD_is_simulator_call(__PD_vector this)
{
  /* simulator call */
  if (__PD_last_call.address == (ulong) this &&
	__PD_last_call.is_valid == __PD_true) {
    /* the flag is reset on a call action */
    __PD_last_call.is_valid = __PD_false;
    return __PD_true;
  }
  
  /* native call */
  return __PD_false; 
}
