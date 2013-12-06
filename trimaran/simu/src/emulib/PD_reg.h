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
 * File:    PD_reg.h
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: registers and port definitions
 *****************************************************************************/


#ifndef __PD_REG_H_
#define __PD_REG_H_


#include "PD_globals.h"

typedef struct __PD_reg  __PD_reg;
typedef struct __PD_port __PD_port;
typedef struct __PD_reg_file_info __PD_reg_file_info;


/* pointer to current register file information */
extern __PD_reg_file_info* __PD_reginfo;

/* all registers are allocated as one contiguous file */
extern __PD_reg* __PD_registers;

/* pointers to physical registers files */
extern __PD_reg* __PD_s_gpr_file;
extern __PD_reg* __PD_r_gpr_file;
extern __PD_reg* __PD_s_fpr_file;
extern __PD_reg* __PD_r_fpr_file;
extern __PD_reg* __PD_s_btr_file;
extern __PD_reg* __PD_s_pr_file;
extern __PD_reg* __PD_r_pr_file;
extern __PD_reg* __PD_s_cr_file;
extern __PD_reg* __PD_r_cr_file;
/* RMR { support vector file type */
extern __PD_reg* __PD_s_vir_file;
extern __PD_reg* __PD_r_vir_file;
extern __PD_reg* __PD_s_vfr_file;
extern __PD_reg* __PD_r_vfr_file;
extern __PD_reg* __PD_s_vmr_file;
extern __PD_reg* __PD_r_vmr_file;
/* } RMR */
/* pointer to macro register vector */
extern __PD_reg* __PD_macro_file;

/*true if simulating physical register files, false if simulating virtual register files*/
extern bool __PD_physical;


/* register file support */
extern void __PD_create_physical_regfile(__PD_reg_file_info*);
extern void __PD_create_virtual_regfile(__PD_reg_file_info*, __PD_reg**);
extern void __PD_restore_regfile(__PD_reg_file_info*, __PD_reg*);
extern void __PD_set_regfile_pointers(__PD_reg_file_info*);

extern __PD_reg** int_ret_file;
extern int int_ret_reg_num;
extern __PD_reg** lc_file;
extern int lc_reg_num;
extern __PD_reg** esc_file;
extern int esc_reg_num;
extern __PD_reg** int_zero_file;
extern int int_zero_reg_num;
extern __PD_reg** flt_zero_file;
extern int flt_zero_reg_num;
extern __PD_reg** flt_one_file;
extern int flt_one_reg_num;
extern __PD_reg** dbl_zero_file;
extern int dbl_zero_reg_num;
extern __PD_reg** dbl_one_file;
extern int dbl_one_reg_num;

struct __PD_reg {
  union {
	ulong  gpr;
	double fpr_D;
	float  fpr_S;
	struct PD_FPR_W {
	  ulong hi;
	  ulong lo;
	} fpr_W;
	struct PD_BTR {
	  ulong addr;
	  bool  pred; /* we can use bool here because of the union; max size makes this safe */
	} btr;
	bool  pr; /* we can use bool here because of the union; max size makes this safe */
	ulong cr;
  } reg;

  /* speculative tag */
  ulong nat; /* so that we can use the delay() function which assume ulongs */

  /* flags for implementing stalls */
  ulong pending;

  /* intended  to   allow  support  of  memory   simulation  or  other
   * functionality in that a user  can attach data or special handlers
   * to a register without affecting the defition of a register 
   */
  void* user_handler;
};


/* flags for the pending bit of a register */
#define __PD_REG_READY        0x00
#define __PD_REG_BUSY         0x01


struct __PD_port {
  /* indicates port is a register or literal */
  bool  is_reg;
  /* is  port is  a register  this is  a pointer  to the  head  of the
   * register file; otherwise it is the literal value 
   */
  void* file;
  /* used for register ports to indicate register number */
  uint  offset;
  uint  num;
  uint  rot;
  /* RMR { support vector registers */
  uint  vec;
  /* } RMR */
  /* (destination) port latency for write-ports */
  ubyte lat;
};


/* register file related parameters */
struct __PD_reg_file_info
{
  uint s_gpr_file_size;
  uint r_gpr_file_size;
  uint s_fpr_file_size;
  uint r_fpr_file_size;
  uint s_btr_file_size;
  uint s_pr_file_size;
  uint r_pr_file_size;
  uint s_cr_file_size;
  uint r_cr_file_size;
  /* RMR { support vector file type */
  uint s_vir_file_size;
  uint r_vir_file_size;
  uint s_vfr_file_size;
  uint r_vfr_file_size;
  uint s_vmr_file_size;
  uint r_vmr_file_size;
  /* } RMR */
  uint total;

  /* RMR { support vector file type */
  uint vector_length;
  /* } RMR */
};


enum __PD_MACRO_REGISTERS
{
  /* return-value registers */
  __PD_RET_ADDR = 0,
  __PD_INT_RET,
  __PD_FLT_RET,
  __PD_DBL_RET,
  
  /* integer register parameters */
  __PD_INT_P1 = 4,
  __PD_INT_P2,
  __PD_INT_P3,
  __PD_INT_P4,

  /* single precision floating-point register parameters */  
  __PD_FLT_P1 = 8,
  __PD_FLT_P2,
  __PD_FLT_P3,
  __PD_FLT_P4,

  /* double precision floating-point register parameters */    
  __PD_DBL_P1 = 12,
  __PD_DBL_P2,
  __PD_DBL_P3,
  __PD_DBL_P4,

  /* pd external program stack pointers */
  __PD_RS = 16,
  __PD_FP,
  __PD_IP,
  __PD_OPS,
  __PD_LV,

  /* software pipelining support */
  __PD_LC = 21,
  __PD_ESC,
  __PD_RRB,
  /* RMR { support vector rotating files */
  __PD_VRRB,
  /* } RMR */
  
  /* predicate register aggrigates */
  __PD_PV_0 = 25,
  __PD_PV_1,
  __PD_PV_2,
  __PD_PV_3,
  __PD_PV_4,
  __PD_PV_5,
  __PD_PV_6,
  __PD_PV_7,

  /* bit butckets and constants*/
  __PD_TEMP_REG = 33,
  __PD_BLACK_HOLE,

  __PD_INT_ZERO,
  __PD_FLT_ZERO,
  __PD_DBL_ZERO,
  __PD_FLT_ONE,
  __PD_DBL_ONE,
  __PD_PRED_F,
  __PD_PRED_T,
  /* RMR { add default all true vector mask */
  __PD_VEC_MASK_T,
  /* } RMR */

  /* RMR { scratch vector register for operand transport */
  __PD_VEC_SCRATCH_PAD,
  /* } RMR */

  /* pd internal program counter and stack pointers */
  __PD_PC = 44,
  __PD_SP,
  __PD_VARARG,

  __PD_ICMOVE,

  /* do not remove - used for indexing */
  __PD_NUM_MACRO_REGISTERS
}; /* __PD_MACRO_REGISTERS */


/* register file pointers */
#define __PD_GPR_FILE      &__PD_s_gpr_file
#define __PD_GPR_FILE_ROT  &__PD_r_gpr_file
#define __PD_FPR_FILE      &__PD_s_fpr_file
#define __PD_FPR_FILE_ROT  &__PD_r_fpr_file
#define __PD_BTR_FILE      &__PD_s_btr_file
#define __PD_PR_FILE       &__PD_s_pr_file
#define __PD_PR_FILE_ROT   &__PD_r_pr_file
#define __PD_CR_FILE       &__PD_s_cr_file
#define __PD_CR_FILE_ROT   &__PD_r_cr_file
/* RMR { support vector file type */
#define __PD_VIR_FILE      &__PD_s_vir_file
#define __PD_VIR_FILE_ROT  &__PD_r_vir_file
#define __PD_VFR_FILE      &__PD_s_vfr_file
#define __PD_VFR_FILE_ROT  &__PD_r_vfr_file
#define __PD_VMR_FILE      &__PD_s_vmr_file
#define __PD_VMR_FILE_ROT  &__PD_r_vmr_file
/* } RMR */
#define __PD_MACRO_FILE    &__PD_macro_file

/* software pipelining support */
#define __PD_RRB_REG __PD_macro_file[__PD_RRB ].reg.cr
/* RMR { support vector rotating files */
#define __PD_VRRB_REG __PD_macro_file[__PD_VRRB].reg.cr
/* } RMR */
#define __PD_LC_REG  CAST(lc_file)[lc_reg_num].reg.cr
#define __PD_ESC_REG CAST(esc_file)[esc_reg_num].reg.cr
#define __PD_PV0_REG __PD_macro_file[__PD_PV_0].reg.cr
#define __PD_PV1_REG __PD_macro_file[__PD_PV_1].reg.cr
#define __PD_PV2_REG __PD_macro_file[__PD_PV_2].reg.cr
#define __PD_PV3_REG __PD_macro_file[__PD_PV_3].reg.cr
#define __PD_PV4_REG __PD_macro_file[__PD_PV_3].reg.cr
#define __PD_PV5_REG __PD_macro_file[__PD_PV_4].reg.cr
#define __PD_PV6_REG __PD_macro_file[__PD_PV_5].reg.cr
#define __PD_PV7_REG __PD_macro_file[__PD_PV_6].reg.cr
#define __PD_PV8_REG __PD_macro_file[__PD_PV_7].reg.cr

/* internal program counter register */
#define __PD_PC_REG __PD_macro_file[__PD_PC].reg.cr

/* internal stack pointer register */
#define __PD_SP_REG __PD_macro_file[__PD_SP].reg.cr

/* internal varargs stack pointer */
#define __PD_VARARG_REG __PD_macro_file[__PD_VARARG].reg.cr

/* global stack pointer registers */
#define __PD_RS_REG  __PD_macro_file[__PD_RS].reg.cr
#define __PD_IP_REG  __PD_macro_file[__PD_IP].reg.cr
#define __PD_OP_REG  __PD_macro_file[__PD_OPS].reg.cr
#define __PD_LV_REG  __PD_macro_file[__PD_LV].reg.cr

/* pass value in register */
#define __PD_INT_P1_REG __PD_macro_file[__PD_INT_P1].reg.gpr
#define __PD_INT_P2_REG __PD_macro_file[__PD_INT_P2].reg.gpr
#define __PD_INT_P3_REG __PD_macro_file[__PD_INT_P3].reg.gpr
#define __PD_INT_P4_REG __PD_macro_file[__PD_INT_P4].reg.gpr
#define __PD_FLT_P1_REG __PD_macro_file[__PD_FLT_P1].reg.fpr_S
#define __PD_FLT_P2_REG __PD_macro_file[__PD_FLT_P2].reg.fpr_S
#define __PD_FLT_P3_REG __PD_macro_file[__PD_FLT_P3].reg.fpr_S
#define __PD_FLT_P4_REG __PD_macro_file[__PD_FLT_P4].reg.fpr_S
#define __PD_DBL_P1_REG __PD_macro_file[__PD_DBL_P1].reg.fpr_D
#define __PD_DBL_P2_REG __PD_macro_file[__PD_DBL_P2].reg.fpr_D
#define __PD_DBL_P3_REG __PD_macro_file[__PD_DBL_P3].reg.fpr_D
#define __PD_DBL_P4_REG __PD_macro_file[__PD_DBL_P4].reg.fpr_D

/* return value registers */
#define __PD_RET_REG     __PD_macro_file[__PD_RET_ADDR].reg.gpr
#define __PD_INT_RET_REG __PD_macro_file[__PD_INT_RET].reg.gpr
#define __PD_FLT_RET_REG __PD_macro_file[__PD_FLT_RET].reg.fpr_S
#define __PD_DBL_RET_REG __PD_macro_file[__PD_DBL_RET].reg.fpr_D

/* true and false bit registers and other bit constants */
#define __PD_BLACK_HOLE_REG   __PD_macro_file[__PD_BLACK_HOLE].reg.gpr
#define __PD_INT_ZERO_REG     __PD_macro_file[__PD_INT_ZERO].reg.gpr
#define __PD_FLT_ZERO_REG     __PD_macro_file[__PD_FLT_ZERO].reg.fpr_S
#define __PD_DBL_ZERO_REG     __PD_macro_file[__PD_DBL_ZERO].reg.fpr_D
#define __PD_FLT_ONE_REG      __PD_macro_file[__PD_FLT_ONE].reg.fpr_S
#define __PD_DBL_ONE_REG      __PD_macro_file[__PD_DBL_ONE].reg.fpr_D
#define __PD_PRED_F_REG       __PD_macro_file[__PD_PRED_F].reg.pr
#define __PD_PRED_T_REG       __PD_macro_file[__PD_PRED_T].reg.pr
/* RMR { add default all true vector mask */
#define __PD_VEC_MASK_T_REG   __PD_macro_file[__PD_VEC_MASK_T].reg.gpr
/* } RMR */
/* RMR { scratch vector register for operand transport */
#define __PD_VEC_SCRATCH_REG  __PD_macro_file[__PD_VEC_SCRATCH_PAD].reg.gpr
/* } RMR */

/* RMR { support vector file type */
#define __PD_VECTOR_LENGTH  __PD_reginfo->vector_length
/* } RMR */

/* macros for accessing the port */
#define CAST(f)    (*(__PD_reg**)f)
//#define REG(p)     CAST(p.file)[p.rot ? ((p.num + __PD_RRB_REG) % p.rot) : p.num]
/* RMR { support vector file type */
#define SREG(p)     CAST(p.file)[p.rot ? ((p.num + __PD_RRB_REG) % p.rot + p.offset) : p.num]
//#define PREG(p)    CAST(p->file)[p->rot ? ((p->num + __PD_RRB_REG) % p->rot) : p->num]
#define PREG(p)    CAST(p->file)[p->rot ? ((p->num + __PD_RRB_REG) % p->rot + p->offset) : p->num]
//#define VREG(p,i)  CAST(p.file)[p.rot ? (((p.num << p.vec) + i + __PD_VRRB_REG) % p.rot + p.offset) : ((p.num << p.vec) + i)]
// The shift is now done in codegen.  -KF 2/2007
#define VREG(p,i)  CAST(p.file)[p.rot ? ((p.num + i + __PD_VRRB_REG) % p.rot + p.offset) : (p.num + i)]
#define CHKLOCK(p) __PD_check_interlock(&(p))
/* } RMR */
#define NUM(p)     p.num
#define LIT(p)     (ulong) p.file
#define LAT(p)     p.lat
#define IS_REG(p)  p.is_reg


#endif /* __PD_REG_H_ */
