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
 * File:   PD_longjmp.c
 * Author: Rodric M. Rabbah
 *
 * Description: support for longjmp
 *****************************************************************************/


#include "PD_sl.h"
#include <string.h>
#include "PD_error.h"
#include "PD_stack.h"
#include "PD_trace.h"
#include "PD_kernel.h"


bool __PD_longjmp_executed = __PD_false;
uint __PD_longjmp_depth    = 0;


void __PD_longjmp(uint* buffer_key, int val)
{
  ulong bytes;

  __PD_jmpbuf_node* jump_buffer = __PD_in_jmpbuf_list(buffer_key);

  if (jump_buffer == NULL) {
    __PD_punt("PD_longjmp encountered a null jmpbuf!");
  }

  /* sanity check: if emulating physical registers, make sure
   * that existing register file size is the same as that saved
   * by setjmp checkpoint
   */
  if ((__PD_parameters->flags & __PD_VIRTREGS) == 0) {
    __PD_assert(__PD_reginfo->total == jump_buffer->buffer.reginfo->total);
  }

  /* restore register file info */
  __PD_reginfo = jump_buffer->buffer.reginfo;
  /* calculate size of register file in bytes */
  bytes = __PD_reginfo->total * sizeof(__PD_reg);

  /* if emulating virtual registers */
  if (__PD_parameters->flags & __PD_VIRTREGS) {
    /* delete current register file */
    free(__PD_registers);
    /* create new register file */
    __PD_registers = (__PD_reg*) malloc(bytes);
  }
  /* copy saved register checkpoint */
  memcpy(__PD_registers, jump_buffer->buffer.registers, bytes);
  /* set register file pointers */
  __PD_set_regfile_pointers(__PD_reginfo);

  /* restore macro registers */
  bytes = __PD_NUM_MACRO_REGISTERS * sizeof(__PD_reg);
  memcpy(__PD_macro_file, jump_buffer->buffer.mregisters, bytes);

  /* restore simulation parameters, statistics and miscellaneous */
  __PD_set_mstack_address(jump_buffer->buffer.mstack);
  __PD_parameters       = jump_buffer->buffer.params;
  __PD_statistics       = jump_buffer->buffer.stats;
  __PD_exception_raised = jump_buffer->buffer.except;
  __PD_done             = jump_buffer->buffer.done;
  __PD_function_name    = jump_buffer->buffer.fname;

  /* so that the program  counter is not  clobbered later
   * and the correct number of function returns is forced
   */
  __PD_longjmp_executed = __PD_true;
  __PD_longjmp_depth    = jump_buffer->depth;
  jump_buffer->depth    = 0;
  
#ifdef __PD_TRACE_
  /* emmitt trace operation if necessary */  
  __PD_edge = jump_buffer->buffer.edge;
  if (__PD_parameters->flags & __PD_CFGTRACE) {
    __PD_trace_longjmp(jump_buffer->buffer.fname, __PD_PC_REG);
  }
#endif /* __PD_TRACE_ */

  /* update the number of visits to the target block */
  if (__PD_parameters->flags & __PD_DYNSTATS) {
    __PD_statistics->freq++;
  }

  /* this is the value that setjmp would normally return */
  CAST(int_ret_file)[int_ret_reg_num].reg.gpr = val;

}
