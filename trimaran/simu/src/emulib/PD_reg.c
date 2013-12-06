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
 * File:    PD_reg.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: initialize register files
 *****************************************************************************/


#include "PD_reg.h"
#include <stdlib.h>
#include <string.h>
#include "PD_queue.h"
#include "PD_error.h"


/* register information */
__PD_reg_file_info* __PD_reginfo = NULL;

/* all registers are allocated as one contiguous file */
__PD_reg* __PD_registers  = NULL;

/* pointers to physical registers files */
__PD_reg* __PD_s_gpr_file = NULL;
__PD_reg* __PD_r_gpr_file = NULL;
__PD_reg* __PD_s_fpr_file = NULL;
__PD_reg* __PD_r_fpr_file = NULL;
__PD_reg* __PD_s_btr_file = NULL;
__PD_reg* __PD_s_pr_file  = NULL;
__PD_reg* __PD_r_pr_file  = NULL;
__PD_reg* __PD_s_cr_file  = NULL;
__PD_reg* __PD_r_cr_file  = NULL;
/* RMR { support vector file type */
__PD_reg* __PD_s_vir_file = NULL;
__PD_reg* __PD_r_vir_file = NULL;
__PD_reg* __PD_s_vfr_file = NULL;
__PD_reg* __PD_r_vfr_file = NULL;
__PD_reg* __PD_s_vmr_file = NULL;
__PD_reg* __PD_r_vmr_file = NULL;
/* } RMR */

/* pointer to macro register vector */
__PD_reg* __PD_macro_file = NULL;

bool __PD_physical = __PD_true;


/* correlate register file pointers */
inline void __PD_set_regfile_pointers(__PD_reg_file_info* reginfo);


/* allocate physical register files */
void __PD_create_physical_regfile(__PD_reg_file_info* reginfo)
{
  static bool created = __PD_false;

  /* physical register file is created only once */
  if (!created) {
    __PD_registers = (__PD_reg*) calloc(reginfo->total, sizeof(__PD_reg));

    /* set the register file pointers */
    __PD_set_regfile_pointers(reginfo);


    created = __PD_true;
  }

  __PD_physical = __PD_true;

  return;
}


/* implements an infinite register view (used for virtual register emulation) */
void __PD_create_virtual_regfile(__PD_reg_file_info* reginfo, 
    __PD_reg** saved_registers)
{
  /* sanity check - dont want to clober any saved registers */
  __PD_assert(*saved_registers == NULL);

  /* save current state of registers */
  if (__PD_registers) {
    *saved_registers = __PD_registers;
  }

  /* create new register files */
  __PD_registers = (__PD_reg*) calloc(reginfo->total, sizeof(__PD_reg));
  /* check if allocation succeeded */
  __PD_assert(__PD_registers);

  /* set the register file pointers */
  __PD_set_regfile_pointers(reginfo);  
}


/* implements an infinite register view (used for virtual register emulation) */
void __PD_restore_regfile(__PD_reg_file_info* reginfo, __PD_reg* saved_registers)
{
  ulong i;

  /* RMR { need to check that no operations are in flight
   * before freeing the register file
   */
  for (i = 0; i < __PD_reginfo->total; i++) {
    /* this will force all operations in flight to complete */
    __PD_check_interlock(&(__PD_registers[i]));
  }
  /* free current register file */
  __PD_assert(__PD_registers != saved_registers);
  free(__PD_registers);
  /* } RMR */

  /* point new register file to one saved from previous context */
  __PD_registers = saved_registers;

  /* set the register file pointers */
  if (reginfo) {
    __PD_set_regfile_pointers(reginfo);
  }
}


/* correlate register file pointers */
inline void __PD_set_regfile_pointers(__PD_reg_file_info* reginfo)
{
  uint s_gpr_offset, r_gpr_offset;
  uint s_fpr_offset, r_fpr_offset;
  uint s_btr_offset;
  uint s_pr_offset,  r_pr_offset;
  uint s_cr_offset,  r_cr_offset;
  /* RMR { support vector file type */
  uint s_vir_offset, r_vir_offset;
  uint s_vfr_offset, r_vfr_offset;
  uint s_vmr_offset, r_vmr_offset;
  /* } RMR */

  s_gpr_offset = 0;
  r_gpr_offset = s_gpr_offset + reginfo->s_gpr_file_size;
  s_fpr_offset = r_gpr_offset + reginfo->r_gpr_file_size;
  r_fpr_offset = s_fpr_offset + reginfo->s_fpr_file_size;
  s_btr_offset = r_fpr_offset + reginfo->r_fpr_file_size;
  s_pr_offset  = s_btr_offset + reginfo->s_btr_file_size;
  r_pr_offset  = s_pr_offset  + reginfo->s_pr_file_size;
  s_cr_offset  = r_pr_offset  + reginfo->r_pr_file_size;
  r_cr_offset  = s_cr_offset  + reginfo->s_cr_file_size;
  /* RMR { support vector file type */
  s_vir_offset = r_cr_offset  + reginfo->r_cr_file_size;
  r_vir_offset = s_vir_offset + reginfo->s_vir_file_size;
  s_vfr_offset = r_vir_offset + reginfo->r_vir_file_size;
  r_vfr_offset = s_vfr_offset + reginfo->s_vfr_file_size;
  s_vmr_offset = r_vfr_offset + reginfo->r_vfr_file_size;
  r_vmr_offset = s_vmr_offset + reginfo->s_vmr_file_size;
  /* } RMR */

  __PD_reginfo    = reginfo;
  __PD_s_gpr_file = __PD_registers ? &__PD_registers[s_gpr_offset] : NULL;
  __PD_r_gpr_file = __PD_registers ? &__PD_registers[r_gpr_offset] : NULL;
  __PD_s_fpr_file = __PD_registers ? &__PD_registers[s_fpr_offset] : NULL;
  __PD_r_fpr_file = __PD_registers ? &__PD_registers[r_fpr_offset] : NULL;
  __PD_s_btr_file = __PD_registers ? &__PD_registers[s_btr_offset] : NULL;
  __PD_s_pr_file  = __PD_registers ? &__PD_registers[s_pr_offset]  : NULL;
  __PD_r_pr_file  = __PD_registers ? &__PD_registers[r_pr_offset]  : NULL;
  __PD_s_cr_file  = __PD_registers ? &__PD_registers[s_cr_offset]  : NULL;
  __PD_r_cr_file  = __PD_registers ? &__PD_registers[r_cr_offset]  : NULL;
  /* RMR { support vector file type */
  __PD_s_vir_file = __PD_registers ? &__PD_registers[s_vir_offset] : NULL;
  __PD_r_vir_file = __PD_registers ? &__PD_registers[r_vir_offset] : NULL;
  __PD_s_vfr_file = __PD_registers ? &__PD_registers[s_vfr_offset] : NULL;
  __PD_r_vfr_file = __PD_registers ? &__PD_registers[r_vfr_offset] : NULL;
  __PD_s_vmr_file = __PD_registers ? &__PD_registers[s_vmr_offset] : NULL;
  __PD_r_vmr_file = __PD_registers ? &__PD_registers[r_vmr_offset] : NULL;
  /* } RMR */
}
