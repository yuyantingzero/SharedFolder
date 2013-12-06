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
//      File:           opcode.h
//      Authors:        Sadun Anik, Scott Mahlke, Richard Johnson
//      Created:        December 1994
//      Description:    Enums for the opcodes 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OPCODE_H
#define _OPCODE_H

#include<iostream>
#include "map.h"
#include "hash_map.h"
#include "string_class.h"

using namespace std;

enum IR_ROOT_OPCODE {
   ROOT_NO_OP,
   ROOT_ABS,
   ROOT_ADD, 
   ROOT_ADDL,
   ROOT_AND,
   ROOT_ANDCM,
   ROOT_DIV,
   ROOT_DIVL,
   ROOT_MAX,
   ROOT_MAXL,
   ROOT_MIN,
   ROOT_MINL,
   ROOT_MPY,
   ROOT_MPYL,
   ROOT_MPYADD,
   ROOT_MPYADDN,
   ROOT_MPYRSUB,
   ROOT_MPYSUB,
   ROOT_NAND,
   ROOT_NOR,
   ROOT_OR,
   ROOT_ORCM,
   ROOT_REM,
   ROOT_REML,
   ROOT_SH1ADDL,
   ROOT_SH2ADDL,
   ROOT_SH3ADDL,
   ROOT_SHL,
   ROOT_SHR,
   ROOT_SHLA,
   ROOT_SHRA,
   ROOT_SUB,
   ROOT_SUBL,
   ROOT_XOR,
   ROOT_XORCM,
   ROOT_FADD,	// S|D
   ROOT_FABS,	// S|D
   ROOT_FDIV,	// S|D
   ROOT_FMAX,	// S|D
   ROOT_FMIN,	// S|D
   ROOT_FMPY,	// S|D
   ROOT_FMPYADD,	// S|D	
   ROOT_FMPYADDN,	// S|D
   ROOT_FMPYRSUB,	// S|D
   ROOT_FMPYSUB,	// S|D
   ROOT_FRCP,	// S|D
   ROOT_FSQRT,	// S|D
   ROOT_FSUB,	// S|D

   ROOT_CONVWS,
   ROOT_CONVWD,
   ROOT_CONVSW,
   ROOT_CONVDW,
   ROOT_CONVSD,
   ROOT_CONVDS,
   ROOT_EXTS,	// B|H

   ROOT_MOVE,
   ROOT_MOVEGF_L,
   ROOT_MOVEGF_U,
   ROOT_MOVEF, // 50
   ROOT_MOVEFG_L,
   ROOT_MOVEFG_U,
   ROOT_MOVEPG,
   ROOT_MOVEGP,
   ROOT_LDCM,  // This operation is no longer supported.
                        // It is replaced by MOVEGCM. We should remove it soon
   ROOT_CMPR,	// <I-cond>
   ROOT_FCMPR,	// S|D | <F-cond> 
   ROOT_CMPP,	// <I-cond> <D-action> <D-action>
   ROOT_FCMPP,	// S|D | <F-cond> <D-action> <D-action>
   ROOT_LD,		// INC B|H|W <cache> <cache>
   ROOT_FLD,		// INC S|D <cache> <cache>      60
   ROOT_ST,		// INC B|H|W <cache> <cache>
   ROOT_FST,		// INC S|D <cache> <cache>
   ROOT_DSLD,
   ROOT_FDSLD,
   ROOT_DVLD,
   ROOT_FDVLD,

   ROOT_BRDVI,
   ROOT_BRDVF,
   ROOT_PBR,
   ROOT_BRU,
   ROOT_RTS,
   ROOT_BRCT,
   ROOT_BRCF,
   ROOT_BRL,
   ROOT_BRLC,
   ROOT_BRF,
   ROOT_BRW,
   
   // Extensions from IMPACT
   ROOT_PROLOGUE,
   ROOT_EPILOGUE,
   ROOT_DEFINE,
   ROOT_ALLOC,
   ROOT_SIM_DIR,
   
   // IR Extensions
   ROOT_D_SWITCH,
   ROOT_D_MERGE,
   ROOT_C_MERGE,
   ROOT_USE,
   ROOT_DEF,
   ROOT_REMAP,
   ROOT_PRED_CLEAR,
   ROOT_PRED_CLEAR_ALL,
   ROOT_PRED_SET,
   ROOT_PRED_AND,
   ROOT_PRED_COMPL,
   ROOT_BRDIR,   // Pseudo branch for control-flow xforms
   ROOT_BRIND,   // Pseudo branch for control-flow xforms
   ROOT_PRED_LOAD_ALL,
   ROOT_PRED_STORE_ALL,

   // Playdoh 2.0, Literal codegen extensions
   ROOT_MOVELG,
   ROOT_MOVELGS,
   ROOT_MOVELGX,
   ROOT_MOVELF,
   ROOT_MOVELFS,
   ROOT_MOVELB,
   ROOT_MOVELBS,
   ROOT_MOVELBX,

   // Playdoh 2.0, Memory ops with base + offset
   ROOT_LG,
   ROOT_SG,
   ROOT_FLG,
   ROOT_FSG,
   ROOT_LDX, // with sign extension
   ROOT_LGX,

   // Playdoh 2.0, move ops to support restricted IO formats
   ROOT_MOVEGG,
   ROOT_MOVEGC,
   ROOT_MOVECG,
   ROOT_MOVEBB,	// previously MOVEB in Playdoh1.1 document
   ROOT_MOVEPP,

   // New opocodes for register save/restore and spill/unspill -- VK/5/22/98
   // New load/store operations for GPRs, FPRs and BTRs. These operations 
   // ignore the speculative tag bit of the register being loaded or stored.
   ROOT_SAVE,
   ROOT_RESTORE,
   ROOT_FSAVE,
   ROOT_FRESTORE,
   ROOT_BSAVE,
   ROOT_BRESTORE,
   ROOT_PSAVE,
   ROOT_PRESTORE,
   ROOT_SAVEG,
   ROOT_RESTOREG,
   ROOT_FSAVEG,
   ROOT_FRESTOREG,
   ROOT_BSAVEG,
   ROOT_BRESTOREG,
   ROOT_PSAVEG,
   ROOT_PRESTOREG,

   // parkhc 01/2007 : vector support
   ROOT_VSAVE,
   ROOT_VRESTORE,
   ROOT_VFSAVE,
   ROOT_VFRESTORE,
   ROOT_VSAVEG,
   ROOT_VRESTOREG,
   ROOT_VFSAVEG,
   ROOT_VFRESTOREG,

   // New move operations used for predicate registers.
   ROOT_MOVEGBP,
   ROOT_MOVEGCM,
   // New move operations for speculative tags
   ROOT_MOVEGBGT,
   ROOT_MOVEGBFT,
   ROOT_MOVEGBPT,
   ROOT_MOVEGBBT,

   // PlayDoh 2.0 local memory loads and stores.  Added SAM 10-98.
   ROOT_LL, // INC B|H|W <lm>
   ROOT_LLX, // INC B|H|W <lm>
   ROOT_FLL, // INC S|D <lm>
   ROOT_SL, // INC B|H|W <lm>
   ROOT_FSL, // INC S|D <lm>

   ROOT_LLG, // INC B|H|W <lm>
   ROOT_LLGX, // INC B|H|W <lm>
   ROOT_FLLG, // INC B|H|W <lm>
   ROOT_SLG, // INC B|H|W <lm>
   ROOT_FSLG, // INC B|H|W <lm>



/*
 * Shivarama Rao, Added the code on 06-06-2001. New opcodes are
 * added for different types of PSEUDO_lOAD and PSEUDO_STORE
 * operations.
 */
   // Simulator pseudo-ops
   ROOT_PSEUDO_LOAD_LITERAL,
   ROOT_PSEUDO_LOAD_SCALAR,
   ROOT_PSEUDO_LOAD_SCALAR_ARRAY,
   ROOT_PSEUDO_LOAD_EVR,
   ROOT_PSEUDO_STORE_SCALAR,
   ROOT_PSEUDO_STORE_SCALAR_ARRAY,
   ROOT_PSEUDO_STORE_EVR,
   ROOT_PSEUDO_PRINT, 
   ROOT_PSEUDO_INIT_MEM, 
 
   // Playdoh 2.0, Saturated Opcodes
   ROOT_SADD,
   ROOT_SADDL,
   ROOT_SSUB,
   ROOT_SSUBL,
   ROOT_SMPY,
   ROOT_SMPYL,
   ROOT_SMPYADD,
   ROOT_SMPYADDN,
   ROOT_SMPYSUB,
   ROOT_SMPYRSUB,

   // Unsigned conversions, added SAM 7-01
   ROOT_CONVLWS,
   ROOT_CONVLWD,
   ROOT_CONVLSW,
   ROOT_CONVLDW,
   ROOT_EXTZ,

   // Extract/Deposit, added mchu 9-03
   ROOT_EXTRACT,
   ROOT_EXTRACT_U,
   ROOT_DEPOSIT,

   /*modification for custom opcode tangw (06-04-02)*/
   ROOT_CUSTOM,
   
   // extension for distributed VLIW, added Hongtao (11-17-03)
   ROOT_EBR,
   ROOT_EBRL,
   ROOT_ERTS,

   ROOT_BCAST,
   ROOT_BCASTF,


   // Stream opcode
   /* <fbouchez> */

   ROOT_S_PUSH,
   ROOT_S_POP,
   ROOT_S_PEEK,
   ROOT_S_NEW,
   ROOT_S_SYNONYM,

   /* </fbouchez> */

   // CCA outlined function call (brl')
   ROOT_BRLP,

   /* KVM : ADD with carry opcodes
    */
   ROOT_ADD_CARRY,
   ROOT_ADDL_CARRY,

   /* KVM : WIDE multiply opcodes
    */
   ROOT_MPY_WIDE,
   ROOT_MPYL_WIDE,

   //hongtaoz: sleep and wake
   ROOT_SLEEP,
   ROOT_SLEEP_FT,
   ROOT_SLEEP_UC,
   ROOT_WAKE,
   ROOT_WAKE_FT,
   ROOT_WAKE_UC,

   //hongtaoz: sends and recvs
   ROOT_SEND_I,
   ROOT_SEND_F,
   ROOT_SEND_P,
   ROOT_SEND_B,
   ROOT_RECV_I,
   ROOT_RECV_F,
   ROOT_RECV_P,
   ROOT_RECV_B,
   ROOT_EBRF,

   // SLARSEN: Vector instructions
   ROOT_VADD,
   ROOT_VADDL,
   ROOT_VSUB,
   ROOT_VSUBL,
   ROOT_VMPY,
   ROOT_VMPYL,
   ROOT_VDIV,
   ROOT_VDIVL,
   ROOT_VREM,
   ROOT_VREML,
   ROOT_VMIN,
   ROOT_VMAX,

   ROOT_VOR,
   ROOT_VAND,

   ROOT_VFADD,
   ROOT_VFSUB,
   ROOT_VFMPY,
   ROOT_VFDIV,
   ROOT_VFMAX,
   ROOT_VFMIN,
   ROOT_VFABS,
   ROOT_VFSQRT,

   ROOT_VCONVWS,
   ROOT_VCONVWD,
   ROOT_VCONVSW,
   ROOT_VCONVDW,
   ROOT_VCONVSD,
   ROOT_VCONVDS,

   ROOT_VSMOVE,
   ROOT_SVMOVE,
   ROOT_VSFMOVE,
   ROOT_SVFMOVE,

   ROOT_VEXTS,
   ROOT_VMOVE,
   ROOT_VMOVEF,

   ROOT_VMOVEGG,
   ROOT_VMOVEFF,
   ROOT_VMOVEMM,

   ROOT_VCMPP,
   ROOT_VFCMPP,

   ROOT_VLD,
   ROOT_VFLD,
   ROOT_VST,
   ROOT_VFST,

   ROOT_VLG,
   ROOT_VFLG,
   ROOT_VSG,
   ROOT_VFSG,

   ROOT_VLDE,
   ROOT_VFLDE,
   ROOT_VSTE,
   ROOT_VFSTE,

   ROOT_VLGE,
   ROOT_VFLGE,
   ROOT_VSGE,
   ROOT_VFSGE,

   ROOT_VPERM,
   ROOT_VFPERM,

   ROOT_VSPLAT,
   ROOT_VFSPLAT,

   /* KVM : SUB with carry opcodes
    */
   ROOT_SUB_CARRY,
   ROOT_SUBL_CARRY,
   ROOT_MASK = 0x1FF
} ;

enum IR_MODIFIERS {
   //IR_PSEUDO =                0x100,
   IR_PSEUDO =            0x8000000,

   IR_DOUBLE =                0x200,
   IR_BYTE =                  0x400,
   IR_HALFWORD =              0x800,
   IR_QUADWORD =             0x1000,
   IR_INCREMENT =            0x2000,

   IR_MULTI_CYCLE =          0x2000,	// modifier for no-ops

   IR_FALSE =                0x2000,
   IR_EQ =                   0x4000,
   IR_LT =                   0x8000,
   IR_LEQ =                  0xc000,
   IR_GT =                  0x10000,
   IR_GEQ =                 0x14000,
   IR_SV =                  0x18000,
   IR_OD =                  0x1c000,
   IR_TRUE =                0x20000,
   IR_NEQ =                 0x24000,
   IR_LLT =                 0x28000,
   IR_LLEQ =                0x2c000,
   IR_LGT =                 0x30000,
   IR_LGEQ =                0x34000,
   IR_NSV =                 0x38000,
   IR_EV =                  0x3c000,

   IR_COND_MASK =           0x3c000,
   IR_CMPP_BASE_MASK =      0x3ffff,

   // Unordered comparison currently not supported!!
   // See SAM if you have questions.
   IR_F_FALSE =              0x2000,
   // IR_F_FALSE_UNO =          0x4000,
   IR_F_EQ =                 0x8000,
   // IR_F_EQ_UNO =             0xc000,
   IR_F_LT =                0x10000,
   // IR_F_LT_UNO =            0x14000,
   IR_F_LEQ =               0x18000,
   // IR_F_LEQ_UNO =           0x1c000,
   IR_F_GT =                0x20000,
   // IR_F_GT_UNO =            0x24000,
   IR_F_GEQ =               0x28000,
   // IR_F_GEQ_UNO =           0x2c000,
   IR_F_NEQ =               0x30000,
   // IR_F_NEQ_UNO =           0x34000,
   IR_F_TRUE =              0x38000,
   // IR_F_TRUE_UNO =          0x3c000,

   // CLEAR_ALL modifiers
   IR_ALL =	    0x2000,
   IR_STATIC =	    0x4000,
   IR_ROTATING =    0x8000,
    

   // destination modifiers for compare-to-predicate ops
   //   - 3 bits for kind, 1 bit for normal/complement

   IR_DEST1_XX =            0x00000, // no dest1 modifier
   IR_DEST1_UN =            0x80000,
   IR_DEST1_UC =            0xc0000,
   IR_DEST1_CN =           0x100000,
   IR_DEST1_CC =           0x140000,
   IR_DEST1_ON =           0x180000,
   IR_DEST1_OC =           0x1c0000,
   IR_DEST1_AN =           0x200000,
   IR_DEST1_AC =           0x240000,
   IR_DEST1_CBIT_MASK =     0x40000, // mask for complement bit
   IR_DEST1_MASK =         0x3c0000,
   
   IR_DEST2_XX =           0x000000, // no dest2 modifier
   IR_DEST2_UN =           0x800000,
   IR_DEST2_UC =           0xc00000,
   IR_DEST2_CN =          0x1000000,
   IR_DEST2_CC =          0x1400000,
   IR_DEST2_ON =          0x1800000,
   IR_DEST2_OC =          0x1c00000,
   IR_DEST2_AN =          0x2000000,
   IR_DEST2_AC =          0x2400000,
   IR_DEST2_CBIT_MASK =    0x400000, // mask for complement bit
   IR_DEST2_MASK =        0x3c00000,
   
   IR_SRC_C1 =              0x40000, 
   IR_SRC_C2 =              0x80000,
   IR_SRC_C3 =              0xC0000,
   IR_SRC_V1 =             0x100000,
   IR_SRC_MASK =	   0x1C0000,  // mask for isolating IR_SRC_C/V field
   
   IR_DEST_C1 =            0x200000,
   IR_DEST_C2 =            0x400000,
   IR_DEST_C3 =            0x600000,
   IR_DEST_V1 =            0x800000,
   IR_DEST_MASK =          0xE00000,  // mask for removing IR_DEST_C/V modifiers

   IR_LM_L1 =              0x040000,   // Local memory specifier, any numbered are allowed,
   IR_LM_L2 =              0x080000,
   IR_LM_L3 =              0x0C0000,
   IR_LM_L4 =              0x100000,
   IR_LM_L5 =              0x140000,
   IR_LM_L6 =              0x180000,
   IR_LM_L7 =              0x1c0000,
   IR_LM_L8 =              0x200000,
   IR_LM_L9 =              0x0240000,
   IR_LM_L10 =             0x0280000,
   IR_LM_L11 =             0x02C0000,
   IR_LM_L12 =             0x0400000,
   IR_LM_L13 =             0x0440000,
   IR_LM_L14 =             0x0480000,
   IR_LM_L15 =             0x04C0000,
   IR_LM_L16 =             0x0800000,
   IR_LM_L17 =             0x0840000,
   IR_LM_L18 =             0x0880000,
   IR_LM_L19 =             0x08C0000,
   IR_LM_L20 =             0x0C00000,
   IR_LM_L21 =             0x0C40000,
   IR_LM_L22 =             0x0C80000,
   IR_LM_L23 =             0x0CC0000,
   IR_LM_L24 =             0x1000000,
   IR_LM_L25 =             0x1040000,
   IR_LM_L26 =             0x1080000,
   IR_LM_L27 =             0x10C0000,
   IR_LM_L28 =             0x1100000,
   IR_LM_L29 =             0x1140000,
   IR_LM_L30 =             0x1180000,
   IR_LM_L31 =             0x11C0000,
   IR_LM_L32 =             0x1200000,

   IR_LM_MASK =            0x1FC0000,  // mask for isolating LM specifier

   IR_SAT_1 =              0x0040000,
   IR_SAT_2 =              0x0080000,
   IR_SAT_3 =              0x00C0000,
   IR_SAT_4 =              0x0100000,
   IR_SAT_5 =              0x0140000,
   IR_SAT_6 =              0x0180000,
   IR_SAT_7 =              0x01C0000,
   IR_SAT_8 =              0x0200000,
   IR_SAT_9 =              0x0240000,
   IR_SAT_10 =             0x0280000,
   IR_SAT_11 =             0x02C0000,
   IR_SAT_12 =             0x0400000,
   IR_SAT_13 =             0x0440000,
   IR_SAT_14 =             0x0480000,
   IR_SAT_15 =             0x04C0000,
   IR_SAT_16 =             0x0800000,
   IR_SAT_17 =             0x0840000,
   IR_SAT_18 =             0x0880000,
   IR_SAT_19 =             0x08C0000,
   IR_SAT_20 =             0x0C00000,
   IR_SAT_21 =             0x0C40000,
   IR_SAT_22 =             0x0C80000,
   IR_SAT_23 =             0x0CC0000,
   IR_SAT_24 =             0x1000000,
   IR_SAT_25 =             0x1040000,
   IR_SAT_26 =             0x1080000,
   IR_SAT_27 =             0x10C0000,
   IR_SAT_28 =             0x1100000,
   IR_SAT_29 =             0x1140000,
   IR_SAT_30 =             0x1180000,
   IR_SAT_31 =             0x11C0000,
   IR_SAT_32 =             0x1200000,

   IR_SAT_MASK =           0x1FC0000, // Mask for Saturation Operations

   IR_LOOP_CONTINUE =     0x1000000,
   IR_LOOP_RAMP =         0x2000000,
   IR_LOOP_STOP =         0x4000000

   // Note, bit 0x8000000 is reserved for IR_PSEUDO (see top of this enum).
} ;

enum IR_BASE_OPCODE {
   BASE_CMPP_W_FALSE = ROOT_CMPP | IR_FALSE,
   BASE_CMPP_W_EQ = ROOT_CMPP | IR_EQ ,
   BASE_CMPP_W_LT = ROOT_CMPP | IR_LT,
   BASE_CMPP_W_LEQ = ROOT_CMPP | IR_LEQ,
   BASE_CMPP_W_GT = ROOT_CMPP | IR_GT ,
   BASE_CMPP_W_GEQ = ROOT_CMPP | IR_GEQ ,
   BASE_CMPP_W_SV = ROOT_CMPP | IR_SV ,
   BASE_CMPP_W_OD = ROOT_CMPP | IR_OD ,
   BASE_CMPP_W_TRUE = ROOT_CMPP | IR_TRUE ,
   BASE_CMPP_W_NEQ = ROOT_CMPP | IR_NEQ,
   BASE_CMPP_W_LLT = ROOT_CMPP | IR_LLT,
   BASE_CMPP_W_LLEQ = ROOT_CMPP | IR_LLEQ,
   BASE_CMPP_W_LGT = ROOT_CMPP | IR_LGT ,
   BASE_CMPP_W_LGEQ = ROOT_CMPP | IR_LGEQ ,
   BASE_CMPP_W_NSV = ROOT_CMPP | IR_NSV ,
   BASE_CMPP_W_EV = ROOT_CMPP | IR_EV ,

   BASE_FCMPP_S_FALSE = ROOT_FCMPP | IR_F_FALSE ,
   BASE_FCMPP_S_EQ = ROOT_FCMPP | IR_F_EQ ,
   BASE_FCMPP_S_LT = ROOT_FCMPP | IR_F_LT,
   BASE_FCMPP_S_LEQ = ROOT_FCMPP | IR_F_LEQ,
   BASE_FCMPP_S_GT = ROOT_FCMPP | IR_F_GT ,
   BASE_FCMPP_S_GEQ = ROOT_FCMPP | IR_F_GEQ ,
   BASE_FCMPP_S_NEQ = ROOT_FCMPP | IR_F_NEQ,
   BASE_FCMPP_S_TRUE = ROOT_FCMPP | IR_F_TRUE,
   
   BASE_FCMPP_D_FALSE = ROOT_FCMPP | IR_DOUBLE | IR_F_FALSE ,
   BASE_FCMPP_D_EQ = ROOT_FCMPP | IR_DOUBLE | IR_F_EQ ,
   BASE_FCMPP_D_LT = ROOT_FCMPP | IR_DOUBLE | IR_F_LT,
   BASE_FCMPP_D_LEQ = ROOT_FCMPP | IR_DOUBLE | IR_F_LEQ,
   BASE_FCMPP_D_GT = ROOT_FCMPP | IR_DOUBLE | IR_F_GT ,
   BASE_FCMPP_D_GEQ = ROOT_FCMPP | IR_DOUBLE | IR_F_GEQ ,
   BASE_FCMPP_D_NEQ = ROOT_FCMPP | IR_DOUBLE | IR_F_NEQ,
   BASE_FCMPP_D_TRUE = ROOT_FCMPP | IR_DOUBLE | IR_F_TRUE,

   // SLARSEN: Vector compare ops
   BASE_VCMPP_W_EQ = ROOT_VCMPP | IR_EQ,
   BASE_VCMPP_W_NEQ = ROOT_VCMPP | IR_NEQ,
   BASE_VCMPP_W_GT = ROOT_VCMPP | IR_GT,
   BASE_VCMPP_W_GEQ = ROOT_VCMPP | IR_GEQ,
   BASE_VFCMPP_S_EQ = ROOT_VFCMPP | IR_EQ,
   BASE_VFCMPP_S_NEQ = ROOT_VFCMPP | IR_NEQ,
   BASE_VFCMPP_S_GT = ROOT_VFCMPP | IR_GT,
   BASE_VFCMPP_S_GEQ = ROOT_VFCMPP | IR_GEQ,
   BASE_VFCMPP_D_EQ = ROOT_VFCMPP | IR_DOUBLE | IR_EQ,
   BASE_VFCMPP_D_NEQ = ROOT_VFCMPP | IR_DOUBLE | IR_NEQ,
   BASE_VFCMPP_D_GT = ROOT_VFCMPP | IR_DOUBLE | IR_GT,
   BASE_VFCMPP_D_GEQ = ROOT_VFCMPP | IR_DOUBLE | IR_GEQ
} ;

enum Opcode {

//
//	Standard INT Arithmetic
//

   NO_OP = ROOT_NO_OP,
   M_NO_OP = ROOT_NO_OP | IR_MULTI_CYCLE,
   ABS_W = ROOT_ABS,
   ADD_W = ROOT_ADD, 
   ADD_CARRY_W = ROOT_ADD_CARRY,
   ADDL_CARRY_W = ROOT_ADDL_CARRY,
   ADDL_W = ROOT_ADDL,
   AND_W = ROOT_AND,
   ANDCM_W = ROOT_ANDCM,
   DIV_W = ROOT_DIV,
   DIVL_W = ROOT_DIVL,
   MAX_W = ROOT_MAX,
   MAXL_W = ROOT_MAXL,
   MIN_W = ROOT_MIN,
   MINL_W = ROOT_MINL,
   MPY_W = ROOT_MPY,
   MPYL_W = ROOT_MPYL,
   MPY_WIDE_W = ROOT_MPY_WIDE,
   MPYL_WIDE_W = ROOT_MPYL_WIDE,
   MPYADD_W = ROOT_MPYADD,
   MPYADDN_W = ROOT_MPYADDN,
   MPYRSUB_W = ROOT_MPYRSUB,
   MPYSUB_W = ROOT_MPYSUB,
   NAND_W = ROOT_NAND,
   NOR_W = ROOT_NOR,
   OR_W = ROOT_OR,
   ORCM_W = ROOT_ORCM,
   REM_W = ROOT_REM,
   REML_W = ROOT_REML,
   SH1ADDL_W = ROOT_SH1ADDL,
   SH2ADDL_W = ROOT_SH2ADDL,
   SH3ADDL_W = ROOT_SH3ADDL,
   SHL_W = ROOT_SHL,
   SHR_W = ROOT_SHR,
   SHLA_W = ROOT_SHLA,
   SHRA_W = ROOT_SHRA,
   SUB_W = ROOT_SUB,
   SUBL_W = ROOT_SUBL,
   SUB_CARRY_W = ROOT_SUB_CARRY,
   SUBL_CARRY_W = ROOT_SUBL_CARRY,
   XOR_W = ROOT_XOR,
   XORCM_W = ROOT_XORCM,

//
//	Saturation Operations
//

   SADD1_W = ROOT_SADD | IR_SAT_1,
   SADD2_W = ROOT_SADD | IR_SAT_2,
   SADD3_W = ROOT_SADD | IR_SAT_3,
   SADD4_W = ROOT_SADD | IR_SAT_4,
   SADD5_W = ROOT_SADD | IR_SAT_5,
   SADD6_W = ROOT_SADD | IR_SAT_6,
   SADD7_W = ROOT_SADD | IR_SAT_7,
   SADD8_W = ROOT_SADD | IR_SAT_8,
   SADD9_W = ROOT_SADD | IR_SAT_9,
   SADD10_W = ROOT_SADD | IR_SAT_10,
   SADD11_W = ROOT_SADD | IR_SAT_11,
   SADD12_W = ROOT_SADD | IR_SAT_12,
   SADD13_W = ROOT_SADD | IR_SAT_13,
   SADD14_W = ROOT_SADD | IR_SAT_14,
   SADD15_W = ROOT_SADD | IR_SAT_15,
   SADD16_W = ROOT_SADD | IR_SAT_16,
   SADD17_W = ROOT_SADD | IR_SAT_17,
   SADD18_W = ROOT_SADD | IR_SAT_18,
   SADD19_W = ROOT_SADD | IR_SAT_19,
   SADD20_W = ROOT_SADD | IR_SAT_20,
   SADD21_W = ROOT_SADD | IR_SAT_21,
   SADD22_W = ROOT_SADD | IR_SAT_22,
   SADD23_W = ROOT_SADD | IR_SAT_23,
   SADD24_W = ROOT_SADD | IR_SAT_24,
   SADD25_W = ROOT_SADD | IR_SAT_25,
   SADD26_W = ROOT_SADD | IR_SAT_26,
   SADD27_W = ROOT_SADD | IR_SAT_27,
   SADD28_W = ROOT_SADD | IR_SAT_28,
   SADD29_W = ROOT_SADD | IR_SAT_29,
   SADD30_W = ROOT_SADD | IR_SAT_30,
   SADD31_W = ROOT_SADD | IR_SAT_31,
   SADD32_W = ROOT_SADD | IR_SAT_32,
   SADDL1_W = ROOT_SADDL | IR_SAT_1,
   SADDL2_W = ROOT_SADDL | IR_SAT_2,
   SADDL3_W = ROOT_SADDL | IR_SAT_3,
   SADDL4_W = ROOT_SADDL | IR_SAT_4,
   SADDL5_W = ROOT_SADDL | IR_SAT_5,
   SADDL6_W = ROOT_SADDL | IR_SAT_6,
   SADDL7_W = ROOT_SADDL | IR_SAT_7,
   SADDL8_W = ROOT_SADDL | IR_SAT_8,
   SADDL9_W = ROOT_SADDL | IR_SAT_9,
   SADDL10_W = ROOT_SADDL | IR_SAT_10,
   SADDL11_W = ROOT_SADDL | IR_SAT_11,
   SADDL12_W = ROOT_SADDL | IR_SAT_12,
   SADDL13_W = ROOT_SADDL | IR_SAT_13,
   SADDL14_W = ROOT_SADDL | IR_SAT_14,
   SADDL15_W = ROOT_SADDL | IR_SAT_15,
   SADDL16_W = ROOT_SADDL | IR_SAT_16,
   SADDL17_W = ROOT_SADDL | IR_SAT_17,
   SADDL18_W = ROOT_SADDL | IR_SAT_18,
   SADDL19_W = ROOT_SADDL | IR_SAT_19,
   SADDL20_W = ROOT_SADDL | IR_SAT_20,
   SADDL21_W = ROOT_SADDL | IR_SAT_21,
   SADDL22_W = ROOT_SADDL | IR_SAT_22,
   SADDL23_W = ROOT_SADDL | IR_SAT_23,
   SADDL24_W = ROOT_SADDL | IR_SAT_24,
   SADDL25_W = ROOT_SADDL | IR_SAT_25,
   SADDL26_W = ROOT_SADDL | IR_SAT_26,
   SADDL27_W = ROOT_SADDL | IR_SAT_27,
   SADDL28_W = ROOT_SADDL | IR_SAT_28,
   SADDL29_W = ROOT_SADDL | IR_SAT_29,
   SADDL30_W = ROOT_SADDL | IR_SAT_30,
   SADDL31_W = ROOT_SADDL | IR_SAT_31,
   SADDL32_W = ROOT_SADDL | IR_SAT_32,
   SSUB1_W = ROOT_SSUB | IR_SAT_1,
   SSUB2_W = ROOT_SSUB | IR_SAT_2,
   SSUB3_W = ROOT_SSUB | IR_SAT_3,
   SSUB4_W = ROOT_SSUB | IR_SAT_4,
   SSUB5_W = ROOT_SSUB | IR_SAT_5,
   SSUB6_W = ROOT_SSUB | IR_SAT_6,
   SSUB7_W = ROOT_SSUB | IR_SAT_7,
   SSUB8_W = ROOT_SSUB | IR_SAT_8,
   SSUB9_W = ROOT_SSUB | IR_SAT_9,
   SSUB10_W = ROOT_SSUB | IR_SAT_10,
   SSUB11_W = ROOT_SSUB | IR_SAT_11,
   SSUB12_W = ROOT_SSUB | IR_SAT_12,
   SSUB13_W = ROOT_SSUB | IR_SAT_13,
   SSUB14_W = ROOT_SSUB | IR_SAT_14,
   SSUB15_W = ROOT_SSUB | IR_SAT_15,
   SSUB16_W = ROOT_SSUB | IR_SAT_16,
   SSUB17_W = ROOT_SSUB | IR_SAT_17,
   SSUB18_W = ROOT_SSUB | IR_SAT_18,
   SSUB19_W = ROOT_SSUB | IR_SAT_19,
   SSUB20_W = ROOT_SSUB | IR_SAT_20,
   SSUB21_W = ROOT_SSUB | IR_SAT_21,
   SSUB22_W = ROOT_SSUB | IR_SAT_22,
   SSUB23_W = ROOT_SSUB | IR_SAT_23,
   SSUB24_W = ROOT_SSUB | IR_SAT_24,
   SSUB25_W = ROOT_SSUB | IR_SAT_25,
   SSUB26_W = ROOT_SSUB | IR_SAT_26,
   SSUB27_W = ROOT_SSUB | IR_SAT_27,
   SSUB28_W = ROOT_SSUB | IR_SAT_28,
   SSUB29_W = ROOT_SSUB | IR_SAT_29,
   SSUB30_W = ROOT_SSUB | IR_SAT_30,
   SSUB31_W = ROOT_SSUB | IR_SAT_31,
   SSUB32_W = ROOT_SSUB | IR_SAT_32,
   SSUBL1_W = ROOT_SSUBL | IR_SAT_1,
   SSUBL2_W = ROOT_SSUBL | IR_SAT_2,
   SSUBL3_W = ROOT_SSUBL | IR_SAT_3,
   SSUBL4_W = ROOT_SSUBL | IR_SAT_4,
   SSUBL5_W = ROOT_SSUBL | IR_SAT_5,
   SSUBL6_W = ROOT_SSUBL | IR_SAT_6,
   SSUBL7_W = ROOT_SSUBL | IR_SAT_7,
   SSUBL8_W = ROOT_SSUBL | IR_SAT_8,
   SSUBL9_W = ROOT_SSUBL | IR_SAT_9,
   SSUBL10_W = ROOT_SSUBL | IR_SAT_10,
   SSUBL11_W = ROOT_SSUBL | IR_SAT_11,
   SSUBL12_W = ROOT_SSUBL | IR_SAT_12,
   SSUBL13_W = ROOT_SSUBL | IR_SAT_13,
   SSUBL14_W = ROOT_SSUBL | IR_SAT_14,
   SSUBL15_W = ROOT_SSUBL | IR_SAT_15,
   SSUBL16_W = ROOT_SSUBL | IR_SAT_16,
   SSUBL17_W = ROOT_SSUBL | IR_SAT_17,
   SSUBL18_W = ROOT_SSUBL | IR_SAT_18,
   SSUBL19_W = ROOT_SSUBL | IR_SAT_19,
   SSUBL20_W = ROOT_SSUBL | IR_SAT_20,
   SSUBL21_W = ROOT_SSUBL | IR_SAT_21,
   SSUBL22_W = ROOT_SSUBL | IR_SAT_22,
   SSUBL23_W = ROOT_SSUBL | IR_SAT_23,
   SSUBL24_W = ROOT_SSUBL | IR_SAT_24,
   SSUBL25_W = ROOT_SSUBL | IR_SAT_25,
   SSUBL26_W = ROOT_SSUBL | IR_SAT_26,
   SSUBL27_W = ROOT_SSUBL | IR_SAT_27,
   SSUBL28_W = ROOT_SSUBL | IR_SAT_28,
   SSUBL29_W = ROOT_SSUBL | IR_SAT_29,
   SSUBL30_W = ROOT_SSUBL | IR_SAT_30,
   SSUBL31_W = ROOT_SSUBL | IR_SAT_31,
   SSUBL32_W = ROOT_SSUBL | IR_SAT_32,
   SMPY1_W = ROOT_SMPY | IR_SAT_1,
   SMPY2_W = ROOT_SMPY | IR_SAT_2,
   SMPY3_W = ROOT_SMPY | IR_SAT_3,
   SMPY4_W = ROOT_SMPY | IR_SAT_4,
   SMPY5_W = ROOT_SMPY | IR_SAT_5,
   SMPY6_W = ROOT_SMPY | IR_SAT_6,
   SMPY7_W = ROOT_SMPY | IR_SAT_7,
   SMPY8_W = ROOT_SMPY | IR_SAT_8,
   SMPY9_W = ROOT_SMPY | IR_SAT_9,
   SMPY10_W = ROOT_SMPY | IR_SAT_10,
   SMPY11_W = ROOT_SMPY | IR_SAT_11,
   SMPY12_W = ROOT_SMPY | IR_SAT_12,
   SMPY13_W = ROOT_SMPY | IR_SAT_13,
   SMPY14_W = ROOT_SMPY | IR_SAT_14,
   SMPY15_W = ROOT_SMPY | IR_SAT_15,
   SMPY16_W = ROOT_SMPY | IR_SAT_16,
   SMPY17_W = ROOT_SMPY | IR_SAT_17,
   SMPY18_W = ROOT_SMPY | IR_SAT_18,
   SMPY19_W = ROOT_SMPY | IR_SAT_19,
   SMPY20_W = ROOT_SMPY | IR_SAT_20,
   SMPY21_W = ROOT_SMPY | IR_SAT_21,
   SMPY22_W = ROOT_SMPY | IR_SAT_22,
   SMPY23_W = ROOT_SMPY | IR_SAT_23,
   SMPY24_W = ROOT_SMPY | IR_SAT_24,
   SMPY25_W = ROOT_SMPY | IR_SAT_25,
   SMPY26_W = ROOT_SMPY | IR_SAT_26,
   SMPY27_W = ROOT_SMPY | IR_SAT_27,
   SMPY28_W = ROOT_SMPY | IR_SAT_28,
   SMPY29_W = ROOT_SMPY | IR_SAT_29,
   SMPY30_W = ROOT_SMPY | IR_SAT_30,
   SMPY31_W = ROOT_SMPY | IR_SAT_31,
   SMPY32_W = ROOT_SMPY | IR_SAT_32,
   SMPYL1_W = ROOT_SMPYL | IR_SAT_1,
   SMPYL2_W = ROOT_SMPYL | IR_SAT_2,
   SMPYL3_W = ROOT_SMPYL | IR_SAT_3,
   SMPYL4_W = ROOT_SMPYL | IR_SAT_4,
   SMPYL5_W = ROOT_SMPYL | IR_SAT_5,
   SMPYL6_W = ROOT_SMPYL | IR_SAT_6,
   SMPYL7_W = ROOT_SMPYL | IR_SAT_7,
   SMPYL8_W = ROOT_SMPYL | IR_SAT_8,
   SMPYL9_W = ROOT_SMPYL | IR_SAT_9,
   SMPYL10_W = ROOT_SMPYL | IR_SAT_10,
   SMPYL11_W = ROOT_SMPYL | IR_SAT_11,
   SMPYL12_W = ROOT_SMPYL | IR_SAT_12,
   SMPYL13_W = ROOT_SMPYL | IR_SAT_13,
   SMPYL14_W = ROOT_SMPYL | IR_SAT_14,
   SMPYL15_W = ROOT_SMPYL | IR_SAT_15,
   SMPYL16_W = ROOT_SMPYL | IR_SAT_16,
   SMPYL17_W = ROOT_SMPYL | IR_SAT_17,
   SMPYL18_W = ROOT_SMPYL | IR_SAT_18,
   SMPYL19_W = ROOT_SMPYL | IR_SAT_19,
   SMPYL20_W = ROOT_SMPYL | IR_SAT_20,
   SMPYL21_W = ROOT_SMPYL | IR_SAT_21,
   SMPYL22_W = ROOT_SMPYL | IR_SAT_22,
   SMPYL23_W = ROOT_SMPYL | IR_SAT_23,
   SMPYL24_W = ROOT_SMPYL | IR_SAT_24,
   SMPYL25_W = ROOT_SMPYL | IR_SAT_25,
   SMPYL26_W = ROOT_SMPYL | IR_SAT_26,
   SMPYL27_W = ROOT_SMPYL | IR_SAT_27,
   SMPYL28_W = ROOT_SMPYL | IR_SAT_28,
   SMPYL29_W = ROOT_SMPYL | IR_SAT_29,
   SMPYL30_W = ROOT_SMPYL | IR_SAT_30,
   SMPYL31_W = ROOT_SMPYL | IR_SAT_31,
   SMPYL32_W = ROOT_SMPYL | IR_SAT_32,
   SMPYADD1_W = ROOT_SMPYADD | IR_SAT_1,
   SMPYADD2_W = ROOT_SMPYADD | IR_SAT_2,
   SMPYADD3_W = ROOT_SMPYADD | IR_SAT_3,
   SMPYADD4_W = ROOT_SMPYADD | IR_SAT_4,
   SMPYADD5_W = ROOT_SMPYADD | IR_SAT_5,
   SMPYADD6_W = ROOT_SMPYADD | IR_SAT_6,
   SMPYADD7_W = ROOT_SMPYADD | IR_SAT_7,
   SMPYADD8_W = ROOT_SMPYADD | IR_SAT_8,
   SMPYADD9_W = ROOT_SMPYADD | IR_SAT_9,
   SMPYADD10_W = ROOT_SMPYADD | IR_SAT_10,
   SMPYADD11_W = ROOT_SMPYADD | IR_SAT_11,
   SMPYADD12_W = ROOT_SMPYADD | IR_SAT_12,
   SMPYADD13_W = ROOT_SMPYADD | IR_SAT_13,
   SMPYADD14_W = ROOT_SMPYADD | IR_SAT_14,
   SMPYADD15_W = ROOT_SMPYADD | IR_SAT_15,
   SMPYADD16_W = ROOT_SMPYADD | IR_SAT_16,
   SMPYADD17_W = ROOT_SMPYADD | IR_SAT_17,
   SMPYADD18_W = ROOT_SMPYADD | IR_SAT_18,
   SMPYADD19_W = ROOT_SMPYADD | IR_SAT_19,
   SMPYADD20_W = ROOT_SMPYADD | IR_SAT_20,
   SMPYADD21_W = ROOT_SMPYADD | IR_SAT_21,
   SMPYADD22_W = ROOT_SMPYADD | IR_SAT_22,
   SMPYADD23_W = ROOT_SMPYADD | IR_SAT_23,
   SMPYADD24_W = ROOT_SMPYADD | IR_SAT_24,
   SMPYADD25_W = ROOT_SMPYADD | IR_SAT_25,
   SMPYADD26_W = ROOT_SMPYADD | IR_SAT_26,
   SMPYADD27_W = ROOT_SMPYADD | IR_SAT_27,
   SMPYADD28_W = ROOT_SMPYADD | IR_SAT_28,
   SMPYADD29_W = ROOT_SMPYADD | IR_SAT_29,
   SMPYADD30_W = ROOT_SMPYADD | IR_SAT_30,
   SMPYADD31_W = ROOT_SMPYADD | IR_SAT_31,
   SMPYADD32_W = ROOT_SMPYADD | IR_SAT_32,
   SMPYADDN1_W = ROOT_SMPYADDN | IR_SAT_1,
   SMPYADDN2_W = ROOT_SMPYADDN | IR_SAT_2,
   SMPYADDN3_W = ROOT_SMPYADDN | IR_SAT_3,
   SMPYADDN4_W = ROOT_SMPYADDN | IR_SAT_4,
   SMPYADDN5_W = ROOT_SMPYADDN | IR_SAT_5,
   SMPYADDN6_W = ROOT_SMPYADDN | IR_SAT_6,
   SMPYADDN7_W = ROOT_SMPYADDN | IR_SAT_7,
   SMPYADDN8_W = ROOT_SMPYADDN | IR_SAT_8,
   SMPYADDN9_W = ROOT_SMPYADDN | IR_SAT_9,
   SMPYADDN10_W = ROOT_SMPYADDN | IR_SAT_10,
   SMPYADDN11_W = ROOT_SMPYADDN | IR_SAT_11,
   SMPYADDN12_W = ROOT_SMPYADDN | IR_SAT_12,
   SMPYADDN13_W = ROOT_SMPYADDN | IR_SAT_13,
   SMPYADDN14_W = ROOT_SMPYADDN | IR_SAT_14,
   SMPYADDN15_W = ROOT_SMPYADDN | IR_SAT_15,
   SMPYADDN16_W = ROOT_SMPYADDN | IR_SAT_16,
   SMPYADDN17_W = ROOT_SMPYADDN | IR_SAT_17,
   SMPYADDN18_W = ROOT_SMPYADDN | IR_SAT_18,
   SMPYADDN19_W = ROOT_SMPYADDN | IR_SAT_19,
   SMPYADDN20_W = ROOT_SMPYADDN | IR_SAT_20,
   SMPYADDN21_W = ROOT_SMPYADDN | IR_SAT_21,
   SMPYADDN22_W = ROOT_SMPYADDN | IR_SAT_22,
   SMPYADDN23_W = ROOT_SMPYADDN | IR_SAT_23,
   SMPYADDN24_W = ROOT_SMPYADDN | IR_SAT_24,
   SMPYADDN25_W = ROOT_SMPYADDN | IR_SAT_25,
   SMPYADDN26_W = ROOT_SMPYADDN | IR_SAT_26,
   SMPYADDN27_W = ROOT_SMPYADDN | IR_SAT_27,
   SMPYADDN28_W = ROOT_SMPYADDN | IR_SAT_28,
   SMPYADDN29_W = ROOT_SMPYADDN | IR_SAT_29,
   SMPYADDN30_W = ROOT_SMPYADDN | IR_SAT_30,
   SMPYADDN31_W = ROOT_SMPYADDN | IR_SAT_31,
   SMPYADDN32_W = ROOT_SMPYADDN | IR_SAT_32,
   SMPYSUB1_W = ROOT_SMPYSUB | IR_SAT_1,
   SMPYSUB2_W = ROOT_SMPYSUB | IR_SAT_2,
   SMPYSUB3_W = ROOT_SMPYSUB | IR_SAT_3,
   SMPYSUB4_W = ROOT_SMPYSUB | IR_SAT_4,
   SMPYSUB5_W = ROOT_SMPYSUB | IR_SAT_5,
   SMPYSUB6_W = ROOT_SMPYSUB | IR_SAT_6,
   SMPYSUB7_W = ROOT_SMPYSUB | IR_SAT_7,
   SMPYSUB8_W = ROOT_SMPYSUB | IR_SAT_8,
   SMPYSUB9_W = ROOT_SMPYSUB | IR_SAT_9,
   SMPYSUB10_W = ROOT_SMPYSUB | IR_SAT_10,
   SMPYSUB11_W = ROOT_SMPYSUB | IR_SAT_11,
   SMPYSUB12_W = ROOT_SMPYSUB | IR_SAT_12,
   SMPYSUB13_W = ROOT_SMPYSUB | IR_SAT_13,
   SMPYSUB14_W = ROOT_SMPYSUB | IR_SAT_14,
   SMPYSUB15_W = ROOT_SMPYSUB | IR_SAT_15,
   SMPYSUB16_W = ROOT_SMPYSUB | IR_SAT_16,
   SMPYSUB17_W = ROOT_SMPYSUB | IR_SAT_17,
   SMPYSUB18_W = ROOT_SMPYSUB | IR_SAT_18,
   SMPYSUB19_W = ROOT_SMPYSUB | IR_SAT_19,
   SMPYSUB20_W = ROOT_SMPYSUB | IR_SAT_20,
   SMPYSUB21_W = ROOT_SMPYSUB | IR_SAT_21,
   SMPYSUB22_W = ROOT_SMPYSUB | IR_SAT_22,
   SMPYSUB23_W = ROOT_SMPYSUB | IR_SAT_23,
   SMPYSUB24_W = ROOT_SMPYSUB | IR_SAT_24,
   SMPYSUB25_W = ROOT_SMPYSUB | IR_SAT_25,
   SMPYSUB26_W = ROOT_SMPYSUB | IR_SAT_26,
   SMPYSUB27_W = ROOT_SMPYSUB | IR_SAT_27,
   SMPYSUB28_W = ROOT_SMPYSUB | IR_SAT_28,
   SMPYSUB29_W = ROOT_SMPYSUB | IR_SAT_29,
   SMPYSUB30_W = ROOT_SMPYSUB | IR_SAT_30,
   SMPYSUB31_W = ROOT_SMPYSUB | IR_SAT_31,
   SMPYSUB32_W = ROOT_SMPYSUB | IR_SAT_32,
   SMPYRSUB1_W = ROOT_SMPYRSUB | IR_SAT_1,
   SMPYRSUB2_W = ROOT_SMPYRSUB | IR_SAT_2,
   SMPYRSUB3_W = ROOT_SMPYRSUB | IR_SAT_3,
   SMPYRSUB4_W = ROOT_SMPYRSUB | IR_SAT_4,
   SMPYRSUB5_W = ROOT_SMPYRSUB | IR_SAT_5,
   SMPYRSUB6_W = ROOT_SMPYRSUB | IR_SAT_6,
   SMPYRSUB7_W = ROOT_SMPYRSUB | IR_SAT_7,
   SMPYRSUB8_W = ROOT_SMPYRSUB | IR_SAT_8,
   SMPYRSUB9_W = ROOT_SMPYRSUB | IR_SAT_9,
   SMPYRSUB10_W = ROOT_SMPYRSUB | IR_SAT_10,
   SMPYRSUB11_W = ROOT_SMPYRSUB | IR_SAT_11,
   SMPYRSUB12_W = ROOT_SMPYRSUB | IR_SAT_12,
   SMPYRSUB13_W = ROOT_SMPYRSUB | IR_SAT_13,
   SMPYRSUB14_W = ROOT_SMPYRSUB | IR_SAT_14,
   SMPYRSUB15_W = ROOT_SMPYRSUB | IR_SAT_15,
   SMPYRSUB16_W = ROOT_SMPYRSUB | IR_SAT_16,
   SMPYRSUB17_W = ROOT_SMPYRSUB | IR_SAT_17,
   SMPYRSUB18_W = ROOT_SMPYRSUB | IR_SAT_18,
   SMPYRSUB19_W = ROOT_SMPYRSUB | IR_SAT_19,
   SMPYRSUB20_W = ROOT_SMPYRSUB | IR_SAT_20,
   SMPYRSUB21_W = ROOT_SMPYRSUB | IR_SAT_21,
   SMPYRSUB22_W = ROOT_SMPYRSUB | IR_SAT_22,
   SMPYRSUB23_W = ROOT_SMPYRSUB | IR_SAT_23,
   SMPYRSUB24_W = ROOT_SMPYRSUB | IR_SAT_24,
   SMPYRSUB25_W = ROOT_SMPYRSUB | IR_SAT_25,
   SMPYRSUB26_W = ROOT_SMPYRSUB | IR_SAT_26,
   SMPYRSUB27_W = ROOT_SMPYRSUB | IR_SAT_27,
   SMPYRSUB28_W = ROOT_SMPYRSUB | IR_SAT_28,
   SMPYRSUB29_W = ROOT_SMPYRSUB | IR_SAT_29,
   SMPYRSUB30_W = ROOT_SMPYRSUB | IR_SAT_30,
   SMPYRSUB31_W = ROOT_SMPYRSUB | IR_SAT_31,
   SMPYRSUB32_W = ROOT_SMPYRSUB | IR_SAT_32,

//
//	Standard FP Arithmetic
//

   FADD_S = ROOT_FADD,
   FADD_D = ROOT_FADD | IR_DOUBLE,

   FABS_S = ROOT_FABS,
   FABS_D = ROOT_FABS | IR_DOUBLE,

   FDIV_S = ROOT_FDIV,
   FDIV_D = ROOT_FDIV | IR_DOUBLE,

   FMAX_S = ROOT_FMAX,
   FMAX_D = ROOT_FMAX | IR_DOUBLE,

   FMIN_S = ROOT_FMIN,
   FMIN_D = ROOT_FMIN | IR_DOUBLE,

   FMPY_S = ROOT_FMPY,
   FMPY_D = ROOT_FMPY | IR_DOUBLE,

   FMPYADD_S = ROOT_FMPYADD,
   FMPYADD_D = ROOT_FMPYADD | IR_DOUBLE,

   FMPYADDN_S = ROOT_FMPYADDN,
   FMPYADDN_D = ROOT_FMPYADDN | IR_DOUBLE,

   FMPYRSUB_S = ROOT_FMPYRSUB,
   FMPYRSUB_D = ROOT_FMPYRSUB | IR_DOUBLE,

   FMPYSUB_S = ROOT_FMPYSUB,
   FMPYSUB_D = ROOT_FMPYSUB | IR_DOUBLE,

   FRCP_S = ROOT_FRCP,
   FRCP_D = ROOT_FRCP | IR_DOUBLE,

   FSQRT_S = ROOT_FSQRT,
   FSQRT_D = ROOT_FSQRT | IR_DOUBLE,

   FSUB_S = ROOT_FSUB,
   FSUB_D = ROOT_FSUB | IR_DOUBLE,

//
//	Conversion operations
//

   CONVWS = ROOT_CONVWS,
   CONVWD = ROOT_CONVWD,
   CONVSW = ROOT_CONVSW,
   CONVDW = ROOT_CONVDW,
   CONVSD = ROOT_CONVSD,
   CONVDS = ROOT_CONVDS,
   CONVLWS = ROOT_CONVLWS,
   CONVLWD = ROOT_CONVLWD,
   CONVLSW = ROOT_CONVLSW,
   CONVLDW = ROOT_CONVLDW,

   EXTS_B = ROOT_EXTS | IR_BYTE,
   EXTS_H = ROOT_EXTS | IR_HALFWORD,
   EXTS_W = ROOT_EXTS,

   EXTZ_B = ROOT_EXTZ | IR_BYTE,
   EXTZ_H = ROOT_EXTZ | IR_HALFWORD,
   EXTZ_W = ROOT_EXTZ,

//
//      Extract/Deposit operations
//
   EXTRACT_W = ROOT_EXTRACT,
   EXTRACTU_W = ROOT_EXTRACT_U,
   DEPOSIT_W = ROOT_DEPOSIT,

//
//	Move operations
//

   MOVE = ROOT_MOVE,

   MOVEGF_L = ROOT_MOVEGF_L,
   MOVEGF_U = ROOT_MOVEGF_U,
   MOVEF_S = ROOT_MOVEF,
   MOVEF_D = ROOT_MOVEF | IR_DOUBLE,
   MOVEFG_L = ROOT_MOVEFG_L,
   MOVEFG_U = ROOT_MOVEFG_U,
   MOVEPG = ROOT_MOVEPG,
   MOVEGP = ROOT_MOVEGP,
   LDCM = ROOT_LDCM,


//
//      Playdoh 2.0, Long int literal synthesis operations
//

   MOVELG  = ROOT_MOVELG,
   MOVELGS = ROOT_MOVELGS,
   MOVELGX = ROOT_MOVELGX,

//
//      Playdoh 2.0, Long float literal synthesis operations
//

   MOVELF  = ROOT_MOVELF,
   MOVELFS = ROOT_MOVELFS,

//
//      Playdoh 2.0, Move ops to support restricted IO formats
//

   MOVEGG = ROOT_MOVEGG,
   MOVEGC = ROOT_MOVEGC,
   MOVECG = ROOT_MOVECG,
   MOVEBB = ROOT_MOVEBB,	// Previously MOVEB in Playdoh1.1 document
   MOVEPP = ROOT_MOVEPP,

// New move operations used to save/restore and spill/unspill predicate 
// registers -- VK/5/22/98
   MOVEGBP = ROOT_MOVEGBP,
   MOVEGCM = ROOT_MOVEGCM,

// New move operations for speculative tags, SAM 7-01
   MOVEGBGT = ROOT_MOVEGBGT,
   MOVEGBFT = ROOT_MOVEGBFT,
   MOVEGBPT = ROOT_MOVEGBPT,
   MOVEGBBT = ROOT_MOVEGBBT,


//
//  Compare to register operations
//	Note: currently the list has not been fully populated with Unordered
//	comparisons because I got sick of typing all this crap out.. SAM 5-95
//

   CMPR_W_FALSE = ROOT_CMPR | IR_FALSE,
   CMPR_W_EQ = ROOT_CMPR | IR_EQ,
   CMPR_W_LT = ROOT_CMPR | IR_LT,
   CMPR_W_LEQ = ROOT_CMPR | IR_LEQ,
   CMPR_W_GT = ROOT_CMPR | IR_GT,
   CMPR_W_GEQ = ROOT_CMPR | IR_GEQ,
   CMPR_W_SV = ROOT_CMPR | IR_SV,
   CMPR_W_OD = ROOT_CMPR | IR_OD,
   CMPR_W_TRUE = ROOT_CMPR | IR_TRUE,
   CMPR_W_NEQ = ROOT_CMPR | IR_NEQ,
   CMPR_W_LLT = ROOT_CMPR | IR_LLT,   
   CMPR_W_LLEQ = ROOT_CMPR | IR_LLEQ,   
   CMPR_W_LGT = ROOT_CMPR | IR_LGT,      
   CMPR_W_LGEQ = ROOT_CMPR | IR_LGEQ,
   CMPR_W_NSV = ROOT_CMPR | IR_NSV,
   CMPR_W_EV = ROOT_CMPR | IR_EV,
   
   FCMPR_S_FALSE = ROOT_FCMPR | IR_F_FALSE,
   FCMPR_S_EQ = ROOT_FCMPR | IR_F_EQ,
   FCMPR_S_LT = ROOT_FCMPR | IR_F_LT,
   FCMPR_S_LEQ = ROOT_FCMPR | IR_F_LEQ,
   FCMPR_S_GT = ROOT_FCMPR | IR_F_GT,
   FCMPR_S_GEQ = ROOT_FCMPR | IR_F_GEQ,
   FCMPR_S_NEQ = ROOT_FCMPR | IR_F_NEQ,
   FCMPR_S_TRUE= ROOT_FCMPR | IR_F_TRUE,
   
   FCMPR_D_FALSE = ROOT_FCMPR | IR_DOUBLE | IR_F_FALSE,
   FCMPR_D_EQ = ROOT_FCMPR | IR_DOUBLE | IR_F_EQ,
   FCMPR_D_LT = ROOT_FCMPR | IR_DOUBLE | IR_F_LT,
   FCMPR_D_LEQ = ROOT_FCMPR | IR_DOUBLE | IR_F_LEQ,
   FCMPR_D_GT = ROOT_FCMPR | IR_DOUBLE | IR_F_GT,
   FCMPR_D_GEQ = ROOT_FCMPR | IR_DOUBLE | IR_F_GEQ,
   FCMPR_D_NEQ = ROOT_FCMPR | IR_DOUBLE | IR_F_NEQ,
   FCMPR_D_TRUE= ROOT_FCMPR | IR_DOUBLE | IR_F_TRUE,

////////////////////////////////////////////////////////////////////////////////////
//
// Playdoh's mongo set of compare to predicate operations
//  ==> moved to opcode_cmpp.h
//
////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//
// Playdoh's mongo set of memory system opcodes
//  ==> moved to opcode_load_store.h
//
////////////////////////////////////////////////////////////////////////////////////

//	Data verify loads to GPR
//

   LDV_B = ROOT_DVLD | IR_BYTE,
   LDV_H = ROOT_DVLD | IR_HALFWORD,
   LDV_W = ROOT_DVLD,
   LDV_Q = ROOT_DVLD | IR_QUADWORD,

//
//	Data verify loads to FPR
//

   FLDV_S = ROOT_FDVLD,
   FLDV_D = ROOT_FDVLD | IR_DOUBLE,
  
////////////////////////////////////////////////////////////////////////////////////
//
// Playdoh branch opcodes
//
////////////////////////////////////////////////////////////////////////////////////

//
//	Data verify branches
//
 
   BRDVI = ROOT_BRDVI,
   BRDVF = ROOT_BRDVF,

//	Prepare to branch ops
//

   PBR = ROOT_PBR,

//       Address literal synthesis to BTR
//

   MOVELB  = ROOT_MOVELB,
   MOVELBS = ROOT_MOVELBS,
   MOVELBX = ROOT_MOVELBX,

//
//
//	Standard branches
//

   BRU = ROOT_BRU,
   BRCT = ROOT_BRCT,
   BRCF = ROOT_BRCF,
   BRL = ROOT_BRL,
   BRLC = ROOT_BRLC,
   RTS = ROOT_RTS,
   BRDIR = ROOT_BRDIR | IR_PSEUDO,
   BRIND = ROOT_BRIND | IR_PSEUDO,

//
//	SWP branches
//

   BRF_B_B_F = ROOT_BRF | IR_LOOP_CONTINUE | IR_LOOP_RAMP,
   BRF_B_F_F = ROOT_BRF | IR_LOOP_CONTINUE,
   BRF_F_B_B = ROOT_BRF | IR_LOOP_RAMP | IR_LOOP_STOP,
   BRF_F_F_B = ROOT_BRF | IR_LOOP_STOP,
   BRF_F_F_F = ROOT_BRF,
   
   BRW_B_B_F = ROOT_BRW | IR_LOOP_CONTINUE | IR_LOOP_RAMP,
   BRW_B_F_F = ROOT_BRW | IR_LOOP_CONTINUE,
   BRW_F_B_B = ROOT_BRW | IR_LOOP_RAMP | IR_LOOP_STOP,
   BRW_F_F_B = ROOT_BRW | IR_LOOP_STOP,
   BRW_F_F_F = ROOT_BRW,

//
//	Ops to support Lcode/Elcor IRs
//

   PROLOGUE = ROOT_PROLOGUE | IR_PSEUDO,
   EPILOGUE = ROOT_EPILOGUE | IR_PSEUDO,
   DEFINE = ROOT_DEFINE | IR_PSEUDO,
   ALLOC = ROOT_ALLOC | IR_PSEUDO,
   SIM_DIR = ROOT_SIM_DIR | IR_PSEUDO,
   
   DUMMY_BR = ROOT_BRU | IR_PSEUDO,
   C_MERGE = ROOT_C_MERGE | IR_PSEUDO,
   D_SWITCH = ROOT_D_SWITCH | IR_PSEUDO,
   D_MERGE = ROOT_D_MERGE | IR_PSEUDO,
   
   USE = ROOT_USE | IR_PSEUDO,
   DEF = ROOT_DEF | IR_PSEUDO,

   REMAP = ROOT_REMAP | IR_PSEUDO,
   PRED_CLEAR = ROOT_PRED_CLEAR,
   PRED_CLEAR_ALL = ROOT_PRED_CLEAR_ALL | IR_ALL,
   PRED_CLEAR_ALL_STATIC = ROOT_PRED_CLEAR_ALL | IR_STATIC,
   PRED_CLEAR_ALL_ROTATING = ROOT_PRED_CLEAR_ALL | IR_ROTATING,
   PRED_SET = ROOT_PRED_SET,
   PRED_AND = ROOT_PRED_AND,
   PRED_COMPL = ROOT_PRED_COMPL,
   PRED_LOAD_ALL = ROOT_PRED_LOAD_ALL,
   PRED_STORE_ALL = ROOT_PRED_STORE_ALL,

   PSEUDO_LOAD_LITERAL = ROOT_PSEUDO_LOAD_LITERAL | IR_PSEUDO,
   PSEUDO_LOAD_SCALAR  = ROOT_PSEUDO_LOAD_SCALAR  | IR_PSEUDO,
   PSEUDO_LOAD_SCALAR_ARRAY  = ROOT_PSEUDO_LOAD_SCALAR_ARRAY  | IR_PSEUDO,
   PSEUDO_LOAD_EVR     = ROOT_PSEUDO_LOAD_EVR  | IR_PSEUDO,
   PSEUDO_STORE_SCALAR = ROOT_PSEUDO_STORE_SCALAR | IR_PSEUDO,
   PSEUDO_STORE_SCALAR_ARRAY = ROOT_PSEUDO_STORE_SCALAR_ARRAY | IR_PSEUDO,
   PSEUDO_STORE_EVR    = ROOT_PSEUDO_STORE_EVR | IR_PSEUDO,
   PSEUDO_PRINT = ROOT_PSEUDO_PRINT | IR_PSEUDO,
   PSEUDO_INIT_MEM = ROOT_PSEUDO_INIT_MEM | IR_PSEUDO,

   /*modification for custom opcode tangw (06-04-02)*/
   CUSTOM = ROOT_CUSTOM,

   // extension for distributed VLIW, added Hongtao (11-17-03)
   EBR			= ROOT_EBR,
   EBRL			= ROOT_EBRL,
   ERTS			= ROOT_ERTS,
   BCAST		= ROOT_BCAST,
   BCASTF		= ROOT_BCASTF,
   
   SLEEP        	= ROOT_SLEEP,
   SLEEP_FT        	= ROOT_SLEEP_FT,
   SLEEP_UC        	= ROOT_SLEEP_UC,
   WAKE	        	= ROOT_WAKE,
   WAKE_FT		= ROOT_WAKE_FT,
   WAKE_UC		= ROOT_WAKE_UC,

   SEND_I		= ROOT_SEND_I,
   SEND_F		= ROOT_SEND_F,
   SEND_P		= ROOT_SEND_P,
   SEND_B		= ROOT_SEND_B,
   RECV_I		= ROOT_RECV_I,
   RECV_F		= ROOT_RECV_F,
   RECV_P		= ROOT_RECV_P,
   RECV_B		= ROOT_RECV_B,

   EBRF_B_B_F = ROOT_EBRF | IR_LOOP_CONTINUE | IR_LOOP_RAMP,
   EBRF_F_F_F = ROOT_EBRF,

   ////////////////////////////////////////////////////////
   // streams
   ////////////////////////////////////////////////////////

   // <fbouchez>

   S_PUSH      = ROOT_S_PUSH,
   S_POP       = ROOT_S_POP,
   S_PEEK      = ROOT_S_PEEK,
   S_NEW       = ROOT_S_NEW      | IR_PSEUDO,
   S_SYNONYM   = ROOT_S_SYNONYM  | IR_PSEUDO,

   // </fbouchez>

   // CCA outlined function call

   BRLP        = ROOT_BRLP,

   // SLARSEN: Vector instructions
   VADD_W = ROOT_VADD,
   VADDL_W = ROOT_VADDL,
   VSUB_W = ROOT_VSUB,
   VSUBL_W = ROOT_VSUBL,
   VMPY_W = ROOT_VMPY,
   VMPYL_W = ROOT_VMPYL,
   VDIV_W = ROOT_VDIV,
   VDIVL_W = ROOT_VDIVL,
   VREM_W = ROOT_VREM,
   VREML_W = ROOT_VREML,
   VMIN_W = ROOT_VMIN,
   VMAX_W = ROOT_VMAX,

   VOR_W = ROOT_VOR,
   VAND_W = ROOT_VAND,

   VFADD_S = ROOT_VFADD,
   VFSUB_S = ROOT_VFSUB,
   VFMPY_S = ROOT_VFMPY,
   VFDIV_S = ROOT_VFDIV,
   VFMAX_S = ROOT_VFMAX,
   VFMIN_S = ROOT_VFMIN,
   VFABS_S = ROOT_VFABS,
   VFSQRT_S = ROOT_VFSQRT,

   VFADD_D = ROOT_VFADD | IR_DOUBLE,
   VFSUB_D = ROOT_VFSUB | IR_DOUBLE,
   VFMPY_D = ROOT_VFMPY | IR_DOUBLE,
   VFDIV_D = ROOT_VFDIV | IR_DOUBLE,
   VFMAX_D = ROOT_VFMAX | IR_DOUBLE,
   VFMIN_D = ROOT_VFMIN | IR_DOUBLE,
   VFABS_D = ROOT_VFABS | IR_DOUBLE,
   VFSQRT_D = ROOT_VFSQRT | IR_DOUBLE,

   VCONVWS = ROOT_VCONVWS,
   VCONVWD = ROOT_VCONVWD,
   VCONVSW = ROOT_VCONVSW,
   VCONVDW = ROOT_VCONVDW,
   VCONVSD = ROOT_VCONVSD,
   VCONVDS = ROOT_VCONVDS,

   VSMOVE = ROOT_VSMOVE,
   SVMOVE = ROOT_SVMOVE,
   VSFMOVE_S = ROOT_VSFMOVE,
   VSFMOVE_D = ROOT_VSFMOVE | IR_DOUBLE,
   SVFMOVE_S = ROOT_SVFMOVE,
   SVFMOVE_D = ROOT_SVFMOVE | IR_DOUBLE,

   VEXTS_B = ROOT_VEXTS | IR_BYTE,
   VEXTS_H = ROOT_VEXTS | IR_HALFWORD,

   VMOVE = ROOT_VMOVE,
   VMOVEF_S = ROOT_VMOVEF,
   VMOVEF_D = ROOT_VMOVEF | IR_DOUBLE,

   VMOVEGG = ROOT_VMOVEGG,
   VMOVEFF_S = ROOT_VMOVEFF,
   VMOVEFF_D = ROOT_VMOVEFF | IR_DOUBLE,
   VMOVEMM = ROOT_VMOVEMM,
   
   VPERM = ROOT_VPERM,
   VFPERM_S = ROOT_VFPERM,
   VFPERM_D = ROOT_VFPERM | IR_DOUBLE,

   VSPLAT = ROOT_VSPLAT,
   VFSPLAT_S = ROOT_VFSPLAT,
   VFSPLAT_D = ROOT_VFSPLAT | IR_DOUBLE,

   LAST_OPCODE = 0xF000000
};

/*
 * Shivaram, Added on 07th June,2001. The enum MEMORY_TYPE is used
 * for pseudo load and pseudo store operations. The exact memory
 * type referenced by the PSEUDO load and store operations are
 * represented through the enum. If the type is TYPE_LITERAL, it means
 * that the operation is to load a literal from the memory. If type
 * is TYPE_SCALAR, the reference is from a scalar i.e *a; If type is
 * TYPE_EVR, then the reference is from a array element.
 */
enum MEMORY_TYPE {
     TYPE_LITERAL=0,
     TYPE_SCALAR,
     TYPE_SCALAR_ARRAY,
     TYPE_EVR
};
extern Hash_map<eString,Opcode> el_string_to_opcode_map;
extern Hash_map<Opcode,eString> el_opcode_to_string_map;
extern char *get_mdes_opcode_string(Op *op);

// SLARSEN: maps scalar opcodes to vector opcodes
extern Hash_map<int,int> el_opcode_to_vector_opcode_map;

unsigned hash_opcode(Opcode& i) ; 
unsigned hash_IR_BASE_OPCODE(IR_BASE_OPCODE& i) ; 

extern void el_init_elcor_opcode_maps(void);
extern void el_deinit_elcor_opcode_maps();

ostream& operator<<(ostream&,const Opcode) ;
ostream& operator<<(ostream&,const IR_BASE_OPCODE) ;
istream& operator>>(istream&,Opcode&) ;

extern Opcode opcode_or(Opcode& opc, Opcode& modifier);
   
#endif
