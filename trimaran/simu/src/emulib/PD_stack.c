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
 * File:    PD_stack.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: maintain simulation stack on function entry and exit
 *****************************************************************************/


#include "PD_stack.h"
#include <stdlib.h>
#include "PD_error.h"
#include "PD_reg.h"
#include "PD_signals.h"
#include "PD_kernel.h"
#include "PD_parms.h"
#include "PD_stats.h"
#include "PD_trace.h"
#include "PD_sl.h"
#ifdef __PD_CACHE_
#  include "PD_cache.h"
#endif /* __PD_CACHE_ */

int __PD_function_stack_id;

#define __PD_FRAME_MARKER     ~0
#define __PD_STACK_SIZE       (8*65536)
#define mpush(v)              *__PD_mstack++ = (ulong) v
#define mpop()                *(--__PD_mstack)


/* stack used to save macro registers */
static ulong __PD_MSTACK[__PD_STACK_SIZE];
static ulong __PD_mstack_base  = (ulong) &__PD_MSTACK[0];
static ulong __PD_mstack_limit = (ulong) &__PD_MSTACK[__PD_STACK_SIZE];
/* macro stack pointer */
static ulong* __PD_mstack = &__PD_MSTACK[0];


/* pd internal stack */
static ulong __PD_STACK[__PD_STACK_SIZE];
static ulong __PD_stack_base  = (ulong) &__PD_STACK[0];
static ulong __PD_stack_limit = (ulong) &__PD_STACK[__PD_STACK_SIZE];


enum __PD_REGISTER_TAG_HANDLE { 
  __PD_SAVE_TAGS, 
  __PD_RESTORE_TAGS, 
  __PD_RESET_TAGS
};


/* one entry for each flag in PD_reg.h */
typedef struct {
  bool  nat;
  ubyte pending;
  void* user_handler;
} __PD_reg_flags;


static void __PD_preserve_register_bits(enum __PD_REGISTER_TAG_HANDLE method)
{
  uint i, total_regs = __PD_reginfo->total;
  __PD_reg_flags* spill_buf = NULL;

  switch (method) {
  case __PD_SAVE_TAGS: {
    spill_buf  = (__PD_reg_flags*) malloc(sizeof(__PD_reg_flags) * total_regs);
    
    if (spill_buf == NULL) {
      __PD_punt("Could not allocate memory to save register tags!");
    }

    for (i = 0; i < total_regs; i++) {
      /* save the tag bits */
      spill_buf[i].nat          = __PD_registers[i].nat;
      spill_buf[i].pending      = __PD_registers[i].pending;
      spill_buf[i].user_handler = __PD_registers[i].user_handler;

      /* reset the tag bits */
      __PD_registers[i].nat = 0;
	__PD_registers[i].pending = __PD_REG_READY;
	__PD_registers[i].user_handler = NULL;
    }
    
    mpush(spill_buf);
  } break;
  
  case __PD_RESTORE_TAGS: {
    spill_buf = (__PD_reg_flags*) mpop();
	
    if (spill_buf == NULL) {
      __PD_punt("Received a NULL pointer while restoring register tags!");
    }
	
    for (i = 0; i < total_regs; i++) {
      /* restore the tag bits */
      __PD_registers[i].nat          = spill_buf[i].nat;
	__PD_registers[i].pending      = spill_buf[i].pending;
	__PD_registers[i].user_handler = spill_buf[i].user_handler;
    }
    
    /* free the allocated memory */
    free (spill_buf);
  } break;
  
  case __PD_RESET_TAGS: {
    for (i = 0; i < total_regs; i++) {
      /* reset the tag bits */
      __PD_registers[i].nat = 0;
	__PD_registers[i].pending = __PD_REG_READY;
	__PD_registers[i].user_handler = NULL;
    }
  } break;
  
  default: __PD_punt("Unrecognized register tag handle method!");
  }
}


/* initialize the pd stack */
void __PD_initialize_stack()
{
  /* pd stack pointer */
  //__PD_SP_REG = __PD_stack_base;
  // hongtaoz: new stack model, from high to low
  if (__PD_parameters->flags & __PD_MEM_HIGH_TO_LOW) {
    __PD_stack_base  = (ulong) &__PD_STACK[__PD_STACK_SIZE];
    __PD_stack_limit = (ulong) &__PD_STACK[0];
  } else if (__PD_parameters->flags & __PD_MEM_LOW_TO_HIGH) {
    __PD_stack_base  = (ulong) &__PD_STACK[0];
    __PD_stack_limit = (ulong) &__PD_STACK[__PD_STACK_SIZE];
  } else
    __PD_punt("__PD_initialize_stack: stack_growth has to be MEM_LOW_TO_HIGH or MEM_HIGH_TO_LOW");
  __PD_SP_REG = __PD_stack_base;
}


/* when the pd simulation environment is entered fron the native
 * environment, a fake stack area must be allocated for incoming
 * parameters
 */
ulong __PD_allocate_native_stack(long marker_size, long size)
{
#if 0
  __PD_SP_REG += marker_size + size;

  /* return a temporary OP pointer to be used for initializing
   * incoming parameters
   */
  return __PD_SP_REG - marker_size;
#endif
  // hongtaoz: handle new stack model
  if (__PD_parameters->flags & __PD_MEM_HIGH_TO_LOW)
    __PD_SP_REG -= marker_size + size;
  else if (__PD_parameters->flags & __PD_MEM_LOW_TO_HIGH)
    __PD_SP_REG += marker_size + size;
  else
    __PD_punt("__PD_allocate_native_stack: stack_growth has to be MEM_LOW_TO_HIGH or MEM_HIGH_TO_LOW");
  return __PD_SP_REG;
}


void __PD_deallocate_native_stack(long marker_size, long size)
{
  /* when a longjump is executed then it  has restored the stack
   * pointer so this should not be done, the wrapper should have
   * guarded against this
   */
  __PD_assert(__PD_longjmp_executed == 0);
  __PD_assert(__PD_longjmp_depth == 0);
  //__PD_SP_REG = __PD_SP_REG - marker_size - size;
  // hongtaoz: handle new stack model
  if (__PD_parameters->flags & __PD_MEM_HIGH_TO_LOW)
    __PD_SP_REG += marker_size + size;
  else if (__PD_parameters->flags & __PD_MEM_LOW_TO_HIGH)
    __PD_SP_REG -= marker_size + size;
  else
    __PD_punt("__PD_deallocate_native_stack: stack_growth has to be MEM_LOW_TO_HIGH or MEM_HIGH_TO_LOW");
}


ulong __PD_get_mstack_address() 
{ 
  return (ulong) __PD_mstack; 
}

void  __PD_set_mstack_address(ulong address) 
{ 
  __PD_mstack = (ulong*) address; 
}


/* function entry code - allocate stack */
void __PD_prologue(__PD_OP* op) 
{
  __PD_function_stack_id++;
  __PD_increment_longjump_depth_count();

  /* save macro registers */
  /* this is used to verify the stack integrity */
  mpush(__PD_mstack_base);
  
  /* save the current function name */
  mpush(__PD_function_name);
  
  /* save pd stack registers */
  mpush(__PD_RET_REG);
  mpush(__PD_LV_REG);
  mpush(__PD_OP_REG);
  mpush(__PD_IP_REG);
  mpush(__PD_RS_REG);
  mpush(__PD_VARARG_REG);
  /* save exception raised flag */
  mpush(__PD_exception_raised);
  /* save aggregated predicate registers since macro file is shared */
  mpush(__PD_PV0_REG);
  mpush(__PD_PV1_REG);
  mpush(__PD_PV2_REG);
  mpush(__PD_PV3_REG);
  mpush(__PD_PV4_REG);
  mpush(__PD_PV5_REG);
  mpush(__PD_PV6_REG);
  mpush(__PD_PV7_REG);

  /* need  to save  the speculative  bits  and register  flags on  all
   * registers since  the register  allocator does not  properly spill
   * registers and does not preserve the attributed flags on registers
   * note - this is only necessary for physical registers 
   */
  if (__PD_parameters->flags & __PD_VIRTREGS) {
    __PD_preserve_register_bits(__PD_SAVE_TAGS);
  }

  /* frame marker */
  mpush(__PD_FRAME_MARKER);

  if ((ulong) __PD_mstack >= __PD_mstack_limit) {
    __PD_punt("Simulator macro stack overflow!");
  }
  
  /* set the new function name */
  __PD_function_name = (char*) LIT(OP_DEST0(op));

#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_CFGTRACE) {
    __PD_trace_fn_entry(__PD_function_name);
    __PD_edge = 0;
  }
#endif /* __PD_TRACE_ */

  // hongtaoz: new stack model
/*
          Stack model 
       
        ------------------------
             incoming parameters     previous stack frame                    
IP   -> ========================   <--- initial SP before entry into function
             local variables
LVS  -> ------------------------   new stack frame
             spill code
RGS  -> ------------------------  
             output parameters
OP   -> ========================   <--- new SP after call to prologue 


IP,LV,RGS,OP are set in __PD_prologue. They are set relative to the new SP
Spill code, input and output parameters are accessed with a positive
offset when growing form high to low and negative offset for low to high
w.r.t RGS, IP and OP respectively. 
*/
  // Initially, increment/decrement stack pointer to the end of frame
  // depending on the direction of stack growth
  // i.e SP = SP +/- (sizeof(spill) + sizeof(local) + sizeof(output)) /* src[0] */
  // Set OP, RGS, IP as shown above.
  // src[1] = 0,
  // src[2] = sizeof(output),
  // src[3] = sizeof(output) + sizeof(spill),
  // src[4] = sizeof(output) + sizeof(spill) + sizeof(local)
  if (!(__PD_parameters->flags & __PD_STACK_MANIPULATED_IN_ELCOR)) {
    if (__PD_parameters->flags & __PD_MEM_HIGH_TO_LOW) {
      __PD_SP_REG -= LIT(OP_SRC0(op));
      __PD_OP_REG = __PD_SP_REG + LIT(OP_SRC1(op));
      __PD_RS_REG = __PD_SP_REG + LIT(OP_SRC2(op));
      __PD_LV_REG = __PD_SP_REG + LIT(OP_SRC3(op));
      __PD_IP_REG = __PD_SP_REG + LIT(OP_SRC4(op)); 
    } else if (__PD_parameters->flags & __PD_MEM_LOW_TO_HIGH) {
      __PD_SP_REG += LIT(OP_SRC0(op));
      __PD_OP_REG = __PD_SP_REG - LIT(OP_SRC1(op));
      __PD_RS_REG = __PD_SP_REG - LIT(OP_SRC2(op));
      __PD_LV_REG = __PD_SP_REG - LIT(OP_SRC3(op));
      __PD_IP_REG = __PD_SP_REG - LIT(OP_SRC4(op)); 
    } else
      __PD_punt("__PD_prologue: stack growth has to be MEM_HIGH_TO_LOW or MEM_LOW_TO_HIGH");
  }

  //if (__PD_SP_REG >= __PD_stack_limit) {
  //  __PD_punt("Simulator stack overflow!");
  //}
  if (__PD_parameters->flags & __PD_MEM_HIGH_TO_LOW) {
    if (__PD_SP_REG <= __PD_stack_limit) {
      __PD_punt("Simulator stack overflow!");
    }
  } else if (__PD_parameters->flags & __PD_MEM_LOW_TO_HIGH) {
    if (__PD_SP_REG >= __PD_stack_limit) {
      __PD_punt("Simulator stack overflow!");
    }
  } else
      __PD_punt("__PD_prologue: stack growth has to be MEM_HIGH_TO_LOW or MEM_LOW_TO_HIGH");
}


/* function exit code - allocate stack */
void __PD_epilogue(__PD_OP* op) 
{
#ifdef __PD_CACHE_
  __PD_cache_epilogue(op);
#endif 
  __PD_function_stack_id--;
  __PD_decrement_longjump_depth_count();

  /* restore stack registers */
  //__PD_SP_REG -= LIT(OP_SRC1(op));
  // hongtaoz: new stack model, from high to low
  if (!(__PD_parameters->flags & __PD_STACK_MANIPULATED_IN_ELCOR)) {
    if (__PD_parameters->flags & __PD_MEM_HIGH_TO_LOW) {
      __PD_SP_REG += LIT(OP_SRC1(op));
    } else if (__PD_parameters->flags & __PD_MEM_LOW_TO_HIGH) {
      __PD_SP_REG -= LIT(OP_SRC1(op));
    } else
      __PD_punt("__PD_epilogue: stack growth has to be MEM_HIGH_TO_LOW or MEM_LOW_TO_HIGH");
  }

  /* restore macro registers */
  if (__PD_FRAME_MARKER != mpop()) {
    __PD_punt("Internal simulator macro stack integrity compromised");
  }
  
  /* need to  restore the speculative  bits and register flags  on all
   * registers since  the register  allocator does not  properly spill
   * registers and does not preserve the attributed flags on registers
   * note  - this is  only necessary  for physical  registers
   */
  if (__PD_parameters->flags & __PD_VIRTREGS) {
    __PD_preserve_register_bits(__PD_RESTORE_TAGS);
  }

  __PD_PV7_REG    = mpop();
  __PD_PV6_REG    = mpop();
  __PD_PV5_REG    = mpop();
  __PD_PV4_REG    = mpop();
  __PD_PV3_REG    = mpop();
  __PD_PV2_REG    = mpop();
  __PD_PV1_REG    = mpop();
  __PD_PV0_REG    = mpop();

  __PD_exception_raised = mpop();

  __PD_VARARG_REG = mpop();
  __PD_RS_REG     = mpop();
  __PD_IP_REG     = mpop();
  __PD_OP_REG     = mpop();
  __PD_LV_REG     = mpop();
  __PD_RET_REG    = mpop();

  /* restore function name */
  __PD_function_name = (char*) mpop();

  /* verify stack integrity */
  if (__PD_mstack_base != mpop()) {
    __PD_punt("Internal simulator macro stack integrity compromised!");
  }

  __PD_done = __PD_true;

#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_CFGTRACE) {
    __PD_trace_rts();
  }
#endif /* __PD_TRACE_ */
}
