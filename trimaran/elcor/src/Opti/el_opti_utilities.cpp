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




///////////////////////////////////////////////////////////////////////////
//									 //
//      File:           el_opti_utilities.cpp                            //
//      Authors:        Scott A. Mahlke                                  //
//      Created:        August 1999                                      //
//      Description:    Misc utility functions used by optis             //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "dbg.h"
#include "el_error.h"
#include "operand.h"
#include "op.h"
#include "region.h"
#include "iterators.h"
#include "region_utilities.h"
#include "pred_analysis.h"
#include "pred_interface.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "el_control.h"
#include "width_utilities.h"
#include "el_opti_utilities.h"


static unsigned long pw2[32] = {
        1,                      /* 2^0 */
        2,                      /* 2^1 */
        4,                      /* 2^2 */
        8,                      /* 2^3 */
        16,                     /* 2^4 */
        32,                     /* 2^5 */
        64,                     /* 2^6 */
        128,                    /* 2^7 */
        256,                    /* 2^8 */
        512,                    /* 2^9 */
        1024,                   /* 2^10 */
        2*1024,                 /* 2^11 */
        4*1024,                 /* 2^12 */
        8*1024,                 /* 2^13 */
        16*1024,                /* 2^14 */
        32*1024,                /* 2^15 */
        64*1024,                /* 2^16 */
        128*1024,               /* 2^17 */
        256*1024,               /* 2^18 */
        512*1024,               /* 2^19 */
        1024*1024,              /* 2^20 */
        2*1024*1024,            /* 2^21 */
        4*1024*1024,            /* 2^22 */
        8*1024*1024,            /* 2^23 */
        16*1024*1024,           /* 2^24 */
        32*1024*1024,           /* 2^25 */
        64*1024*1024,           /* 2^26 */
        128*1024*1024,          /* 2^27 */
        256*1024*1024,          /* 2^28 */
        512*1024*1024,          /* 2^29 */
        1024*1024*1024,         /* 2^30 */
	// this should fix the overflow problem -sherwood 9/99
        2*((unsigned long)1024*1024*1024)  /* 2^31 */
};

bool El_is_power_of_two(int val)
{
    int i;

    for (i=0; i<32; i++) {
	if ((unsigned)val == pw2[i])
	    return (true);
    }

    return (false);
}

bool El_is_power_of_two(Operand &opnd)
{
    if (! opnd.is_int())
	return (false);

    return (El_is_power_of_two(opnd.int_value()));
}

int El_int_log2(int val)
{
    int i;

    for (i=0; i<32; i++) {
	if ((unsigned)val == pw2[i])
	    return (i);
    }

    El_punt("El_int_log2: non-power of 2 value: %d", val);
    return (-1);
}

int El_int_pow2(int val)
{
    if ((val < 0) || (val > 31))
	El_punt("El_int_pow2: illegal value: %d", val);

    return (pw2[val]);
}

bool El_is_int_zero(Operand &opnd)
{
    if (! opnd.is_int())
	return (false);

    return (opnd.int_value() == 0);
}

bool El_is_int_one(Operand &opnd)
{
    if (! opnd.is_int())
	return (false);

    return (opnd.int_value() == 1);
}

bool El_is_int_negone(Operand &opnd)
{
    if (! opnd.is_int())
	return (false);

    return (opnd.int_value() == -1);
}

bool El_all_sources_are_compiletime_constants(Op *op)
{
    Operand src;

    for (Op_explicit_sources src_i(op); src_i!=0; src_i++) {
	src = *src_i;
	if (! (src.is_int() || src.is_predicate() ||
	       src.is_float() || src.is_double()))
	    return (false);
    }

    return (true);
}

// This is a local search, op1 comes before op2
// Currently this routine is not predicate sensitive!!
bool El_no_defs_between(Compound_region *reg, Op *op1, Op *op2, Operand &val)
{
    // This code doesn't work with larger regions.
    assert(reg->is_bb() || reg->is_hb());

    Op *op;
    Operand dest;

    if (! region_contains_op(reg, op1))
	El_punt("El_no_defs_between: op %d not in reg %d", op1->id(), reg->id());
    if (! region_contains_op(reg, op2))
	El_punt("El_no_defs_between: op %d not in reg %d", op2->id(), reg->id());

    for (Region_ops_C0_order op_i(reg, op1->next()); op_i!=0; op_i++) {
	op = *op_i;
	if (op == op2)
	    return (true);
	for (Op_all_dests d_iter(op); d_iter!=0; d_iter++) {
	    dest = *d_iter;
	    if (dest == val)
		return (false);
	}
    }
    El_punt("El_no_defs_between: op2 %d not found after op1 %d",
			op2->id(), op1->id());

    return (false);
}

// This is a local search, op1 comes before op2
// Currently this routine is not predicate sensitive!!
bool El_no_uses_between(Compound_region *reg, Op *op1, Op *op2, Operand &val)
{
    Op *op;
    Operand src;

    if (! region_contains_op(reg, op1))
        El_punt("El_no_uses_between: op %d not in reg %d", op1->id(), reg->id());
    if (! region_contains_op(reg, op2))
        El_punt("El_no_uses_between: op %d not in reg %d", op2->id(), reg->id());

    for (Region_ops_C0_order op_i(reg, op1->next()); op_i!=0; op_i++) {
        op = *op_i;
        if (op == op2)
            return (true);
        for (Op_all_inputs s_iter(op); s_iter!=0; s_iter++) {
            src = *s_iter;
            if (src == val)
                return (false);
        }
    }
    El_punt("El_no_uses_between: op2 %d not found after op1 %d",
                        op2->id(), op1->id());

    return (false);
}

bool El_predicate_dominates(Compound_region *reg, El_ref &first, El_ref &second)
{
    Pred_cookie first_pc, second_pc;

    Pred_jar pj(reg);

    first_pc = pj.get_glb_guard(first);
    second_pc = pj.get_lub_guard(second);
    if ((first_pc == second_pc) || (second_pc.is_subset(first_pc)))
        return (true);
    else
	return (false);
}

/*
 *	Evaluate ops in Table 2 of the PlayDoh document
 */
int El_evaluate_int_computation_op(Op *op)
{
    Opcode opc;
    Operand src1, src2, src3;
    
    opc = op->opcode();
    switch (opc) {
	case ABS_W: {
	    int s1;
	    src1 = op->src(SRC1);
	    s1 = src1.int_value();
	    if (s1 < 0)
		return (-s1);
	    else
		return (s1);
	}

	case ADD_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1+s2);
	}

	case ADDL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return (s1+s2);
	}

	case AND_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1&s2);
	}

	case ANDCM_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1&(~s2));
	}

	case DIV_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1/s2);
	}

	case DIVL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return (s1/s2);
	}

	case MPY_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1*s2);
	}

	case MPYL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return (s1*s2);
	}

	case NAND_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (~(s1&s2));
	}

	case NOR_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (~(s1|s2));
	}

	case OR_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1|s2);
	}

	case ORCM_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1|(~s2));
	}

	case REM_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1%s2);
	}

	case REML_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return (s1%s2);
	}

	case SH1ADDL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return ((s1<<1)+s2);
	}

	case SH2ADDL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return ((s1<<2)+s2);
	}

	case SH3ADDL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return ((s1<<3)+s2);
	}

	case SHL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return (s1<<s2);
	}

	case SHR_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return (s1>>s2);
	}

	case SHLA_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1<<s2);
	}

	case SHRA_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1>>s2);
	}

	case SUB_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1-s2);
	}

	case SUBL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
	    return (s1-s2);
	}

	case XOR_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1^s2);
	}

	case XORCM_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    return (s1^(~s2));
	}

	case MIN_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    if (s1 > s2)
		return (s2);
	    else
		return (s1);
	}

	case MINL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned int) src1.int_value();
	    s2 = (unsigned int) src2.int_value();
	    if (s1 > s2)
		return (s2);
	    else
		return (s1);
	}

	case MAX_W: {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    if (s1 > s2)
		return (s1);
	    else
		return (s2);
        }

	case MAXL_W: {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned int) src1.int_value();
	    s2 = (unsigned int) src2.int_value();
	    if (s1 > s2)
		return (s1);
	    else
		return (s2);
        }

        // Shivaram:17:01:2001:Added the operation for saturating operations

	case (SADD1_W):  case (SADD2_W):  case (SADD3_W):  case (SADD4_W): 
        case (SADD5_W):  case (SADD6_W):  case (SADD7_W):  case (SADD8_W): 
        case (SADD9_W):  case (SADD10_W): case (SADD11_W): case (SADD12_W): 
        case (SADD13_W): case (SADD14_W): case (SADD15_W): case (SADD16_W): 
        case (SADD17_W): case (SADD18_W): case (SADD19_W): case (SADD20_W): 
	case (SADD21_W): case (SADD22_W): case (SADD23_W): case (SADD24_W): 
        case (SADD25_W): case (SADD26_W): case (SADD27_W): case (SADD28_W): 
        case (SADD29_W): case (SADD30_W): case (SADD31_W): case (SADD32_W): 
        {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
            return (El_evaluate_satadd_op(s1,s2,extract_saturate_point(opc)));
	}

	case (SADDL1_W):  case (SADDL2_W):  case (SADDL3_W):  case (SADDL4_W): 
        case (SADDL5_W):  case (SADDL6_W):  case (SADDL7_W):  case (SADDL8_W): 
        case (SADDL9_W):  case (SADDL10_W): case (SADDL11_W): case (SADDL12_W): 
        case (SADDL13_W): case (SADDL14_W): case (SADDL15_W): case (SADDL16_W): 
        case (SADDL17_W): case (SADDL18_W): case (SADDL19_W): case (SADDL20_W): 
	case (SADDL21_W): case (SADDL22_W): case (SADDL23_W): case (SADDL24_W): 
        case (SADDL25_W): case (SADDL26_W): case (SADDL27_W): case (SADDL28_W): 
        case (SADDL29_W): case (SADDL30_W): case (SADDL31_W): case (SADDL32_W): 
        {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
            return (El_evaluate_sataddl_op(s1,s2,extract_saturate_point(opc)));
	}

	case (SSUB1_W):  case (SSUB2_W):  case (SSUB3_W):  case (SSUB4_W): 
        case (SSUB5_W):  case (SSUB6_W):  case (SSUB7_W):  case (SSUB8_W): 
        case (SSUB9_W):  case (SSUB10_W): case (SSUB11_W): case (SSUB12_W): 
        case (SSUB13_W): case (SSUB14_W): case (SSUB15_W): case (SSUB16_W): 
        case (SSUB17_W): case (SSUB18_W): case (SSUB19_W): case (SSUB20_W): 
	case (SSUB21_W): case (SSUB22_W): case (SSUB23_W): case (SSUB24_W): 
        case (SSUB25_W): case (SSUB26_W): case (SSUB27_W): case (SSUB28_W): 
        case (SSUB29_W): case (SSUB30_W): case (SSUB31_W): case (SSUB32_W): 
        {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
            return (El_evaluate_satsub_op(s1,s2,extract_saturate_point(opc)));
	}

	case (SSUBL1_W):  case (SSUBL2_W):  case (SSUBL3_W):  case (SSUBL4_W): 
        case (SSUBL5_W):  case (SSUBL6_W):  case (SSUBL7_W):  case (SSUBL8_W): 
        case (SSUBL9_W):  case (SSUBL10_W): case (SSUBL11_W): case (SSUBL12_W): 
        case (SSUBL13_W): case (SSUBL14_W): case (SSUBL15_W): case (SSUBL16_W): 
        case (SSUBL17_W): case (SSUBL18_W): case (SSUBL19_W): case (SSUBL20_W): 
	case (SSUBL21_W): case (SSUBL22_W): case (SSUBL23_W): case (SSUBL24_W): 
        case (SSUBL25_W): case (SSUBL26_W): case (SSUBL27_W): case (SSUBL28_W): 
        case (SSUBL29_W): case (SSUBL30_W): case (SSUBL31_W): case (SSUBL32_W): 
        {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
            return (El_evaluate_satsubl_op(s1,s2,extract_saturate_point(opc)));
	}
	case (SMPY1_W):  case (SMPY2_W):  case (SMPY3_W):  case (SMPY4_W): 
        case (SMPY5_W):  case (SMPY6_W):  case (SMPY7_W):  case (SMPY8_W): 
        case (SMPY9_W):  case (SMPY10_W): case (SMPY11_W): case (SMPY12_W): 
        case (SMPY13_W): case (SMPY14_W): case (SMPY15_W): case (SMPY16_W): 
        case (SMPY17_W): case (SMPY18_W): case (SMPY19_W): case (SMPY20_W): 
	case (SMPY21_W): case (SMPY22_W): case (SMPY23_W): case (SMPY24_W): 
        case (SMPY25_W): case (SMPY26_W): case (SMPY27_W): case (SMPY28_W): 
        case (SMPY29_W): case (SMPY30_W): case (SMPY31_W): case (SMPY32_W): 
        {
	    int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
            return (El_evaluate_satmpy_op(s1,s2,extract_saturate_point(opc)));
	}

	case (SMPYL1_W):  case (SMPYL2_W):  case (SMPYL3_W):  case (SMPYL4_W): 
        case (SMPYL5_W):  case (SMPYL6_W):  case (SMPYL7_W):  case (SMPYL8_W): 
        case (SMPYL9_W):  case (SMPYL10_W): case (SMPYL11_W): case (SMPYL12_W): 
        case (SMPYL13_W): case (SMPYL14_W): case (SMPYL15_W): case (SMPYL16_W): 
        case (SMPYL17_W): case (SMPYL18_W): case (SMPYL19_W): case (SMPYL20_W): 
	case (SMPYL21_W): case (SMPYL22_W): case (SMPYL23_W): case (SMPYL24_W): 
        case (SMPYL25_W): case (SMPYL26_W): case (SMPYL27_W): case (SMPYL28_W): 
        case (SMPYL29_W): case (SMPYL30_W): case (SMPYL31_W): case (SMPYL32_W): 
        {
	    unsigned int s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = (unsigned) src1.int_value();
	    s2 = (unsigned) src2.int_value();
            return (El_evaluate_satmpyl_op(s1,s2,extract_saturate_point(opc)));
	}
	case (SMPYADD1_W):  case (SMPYADD2_W):  case (SMPYADD3_W):  case (SMPYADD4_W): 
        case (SMPYADD5_W):  case (SMPYADD6_W):  case (SMPYADD7_W):  case (SMPYADD8_W): 
        case (SMPYADD9_W):  case (SMPYADD10_W): case (SMPYADD11_W): case (SMPYADD12_W): 
        case (SMPYADD13_W): case (SMPYADD14_W): case (SMPYADD15_W): case (SMPYADD16_W): 
        case (SMPYADD17_W): case (SMPYADD18_W): case (SMPYADD19_W): case (SMPYADD20_W): 
	case (SMPYADD21_W): case (SMPYADD22_W): case (SMPYADD23_W): case (SMPYADD24_W): 
        case (SMPYADD25_W): case (SMPYADD26_W): case (SMPYADD27_W): case (SMPYADD28_W): 
        case (SMPYADD29_W): case (SMPYADD30_W): case (SMPYADD31_W): case (SMPYADD32_W): 
        {
	    int s1,s2,s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    s3 = src3.int_value();
            return (El_evaluate_satmpyadd_op(s1,s2,s3,extract_saturate_point(opc)));
	}
	case (SMPYADDN1_W):case (SMPYADDN2_W):case (SMPYADDN3_W):case (SMPYADDN4_W): 
        case (SMPYADDN5_W):case (SMPYADDN6_W):case (SMPYADDN7_W):case (SMPYADDN8_W): 
        case (SMPYADDN9_W):case (SMPYADDN10_W):case (SMPYADDN11_W):case (SMPYADDN12_W): 
        case (SMPYADDN13_W):case (SMPYADDN14_W):case (SMPYADDN15_W):case (SMPYADDN16_W): 
        case (SMPYADDN17_W):case (SMPYADDN18_W):case (SMPYADDN19_W):case (SMPYADDN20_W): 
	case (SMPYADDN21_W):case (SMPYADDN22_W):case (SMPYADDN23_W):case (SMPYADDN24_W): 
        case (SMPYADDN25_W):case (SMPYADDN26_W):case (SMPYADDN27_W):case (SMPYADDN28_W): 
        case (SMPYADDN29_W):case (SMPYADDN30_W):case (SMPYADDN31_W):case (SMPYADDN32_W): 
        {
	    int s1,s2,s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    s3 = src3.int_value();
            return (El_evaluate_satmpyaddn_op(s1,s2,s3,extract_saturate_point(opc)));
	}
	case (SMPYSUB1_W):  case (SMPYSUB2_W):  case (SMPYSUB3_W):  case (SMPYSUB4_W): 
        case (SMPYSUB5_W):  case (SMPYSUB6_W):  case (SMPYSUB7_W):  case (SMPYSUB8_W): 
        case (SMPYSUB9_W):  case (SMPYSUB10_W): case (SMPYSUB11_W): case (SMPYSUB12_W): 
        case (SMPYSUB13_W): case (SMPYSUB14_W): case (SMPYSUB15_W): case (SMPYSUB16_W): 
        case (SMPYSUB17_W): case (SMPYSUB18_W): case (SMPYSUB19_W): case (SMPYSUB20_W): 
	case (SMPYSUB21_W): case (SMPYSUB22_W): case (SMPYSUB23_W): case (SMPYSUB24_W): 
        case (SMPYSUB25_W): case (SMPYSUB26_W): case (SMPYSUB27_W): case (SMPYSUB28_W): 
        case (SMPYSUB29_W): case (SMPYSUB30_W): case (SMPYSUB31_W): case (SMPYSUB32_W): 
        {
	    int s1,s2,s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    s3 = src3.int_value();
            return (El_evaluate_satmpysub_op(s1,s2,s3,extract_saturate_point(opc)));
	}
	case (SMPYRSUB1_W):case (SMPYRSUB2_W):case (SMPYRSUB3_W):  case (SMPYRSUB4_W): 
        case (SMPYRSUB5_W):case (SMPYRSUB6_W):case (SMPYRSUB7_W):  case (SMPYRSUB8_W): 
        case (SMPYRSUB9_W):case (SMPYRSUB10_W):case (SMPYRSUB11_W):case (SMPYRSUB12_W): 
        case (SMPYRSUB13_W):case (SMPYRSUB14_W):case (SMPYRSUB15_W):case (SMPYRSUB16_W): 
        case (SMPYRSUB17_W):case (SMPYRSUB18_W):case (SMPYRSUB19_W):case (SMPYRSUB20_W): 
	case (SMPYRSUB21_W):case (SMPYRSUB22_W):case (SMPYRSUB23_W):case (SMPYRSUB24_W): 
        case (SMPYRSUB25_W):case (SMPYRSUB26_W):case (SMPYRSUB27_W):case (SMPYRSUB28_W): 
        case (SMPYRSUB29_W):case (SMPYRSUB30_W):case (SMPYRSUB31_W):case (SMPYRSUB32_W): 
        {
	    int s1,s2,s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.int_value();
	    s2 = src2.int_value();
	    s3 = src3.int_value();
            return (El_evaluate_satmpyrsub_op(s1,s2,s3,extract_saturate_point(opc)));
	}

	default:
	    El_punt("El_evaluate_int_computation_op: unknown opcode");
	    return (0);
    }
}

/*
 *	Evaluate single precision ops in Table 3 of the PlayDoh document
 */
float El_evaluate_flt_computation_op(Op *op)
{
    Opcode opc;
    Operand src1, src2, src3;
    
    opc = op->opcode();
    switch (opc) {
	case FADD_S: {
	    float s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    return (s1+s2);
	}

	case FABS_S: {
	    float s1;
	    src1 = op->src(SRC1);
	    s1 = src1.float_value();
	    if (s1 > 0.0)
		return (s1);
	    else
		return (-s1);
	}

	case FDIV_S: {
	    float s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    return (s1/s2);
	}

	case FMAX_S: {
	    float s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    if (s1 > s2)
		return (s1);
	    else
		return (s2);
	}

	case FMIN_S: {
	    float s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    if (s1 > s2)
		return (s2);
	    else
		return (s1);
	}

	case FMPY_S: {
	    float s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    return (s1*s2);
	}

	case FMPYADD_S: {
	    float s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    s3 = src3.float_value();
	    return (s1*s2+s3);
	}

	case FMPYADDN_S: {
	    float s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    s3 = src3.float_value();
	    return (-(s1*s2+s3));
	}

	case FMPYRSUB_S: {
	    float s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    s3 = src3.float_value();
	    return (s3-s1*s2);
	}

	case FMPYSUB_S: {
	    float s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    s3 = src3.float_value();
	    return (s1*s2-s3);
	}

	case FRCP_S: {
	    float s1;
	    src1 = op->src(SRC1);
	    s1 = src1.float_value();
	    return ((float)1.0/s1);
	}

	case FSQRT_S: {
	    float s1;
	    src1 = op->src(SRC1);
	    s1 = src1.float_value();
	    return ((float)sqrt((double)s1));
	}

	case FSUB_S: {
	    float s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.float_value();
	    s2 = src2.float_value();
	    return (s1-s2);
	}

	default:
	    El_punt("El_evaluate_flt_arithmetic_op: unknown opcode");
	    return (0.0);
    }
}

/*
 *	Evaluate double precision ops in Table 3 of the PlayDoh document
 */
double El_evaluate_dbl_computation_op(Op *op)
{
    Opcode opc;
    Operand src1, src2, src3;
    
    opc = op->opcode();
    switch (opc) {
	case FADD_D: {
	    double s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    return (s1+s2);
	}

	case FABS_D: {
	    double s1;
	    src1 = op->src(SRC1);
	    s1 = src1.double_value();
	    if (s1 > 0.0)
		return (s1);
	    else
		return (-s1);
	}

	case FDIV_D: {
	    double s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    return (s1/s2);
	}

	case FMAX_D: {
	    double s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    if (s1 > s2)
		return (s1);
	    else
		return (s2);
	}

	case FMIN_D: {
	    double s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    if (s1 > s2)
		return (s2);
	    else
		return (s1);
	}

	case FMPY_D: {
	    double s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    return (s1*s2);
	}

	case FMPYADD_D: {
	    double s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    s3 = src3.double_value();
	    return (s1*s2+s3);
	}

	case FMPYADDN_D: {
	    double s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    s3 = src3.double_value();
	    return (-(s1*s2+s3));
	}

	case FMPYRSUB_D: {
	    double s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    s3 = src3.double_value();
	    return (s3-s1*s2);
	}

	case FMPYSUB_D: {
	    double s1, s2, s3;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    src3 = op->src(SRC3);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    s3 = src3.double_value();
	    return (s1*s2-s3);
	}

	case FRCP_D: {
	    double s1;
	    src1 = op->src(SRC1);
	    s1 = src1.double_value();
	    return ((double)1.0/s1);
	}

	case FSQRT_D: {
	    double s1;
	    src1 = op->src(SRC1);
	    s1 = src1.double_value();
	    return (sqrt(s1));
	}

	case FSUB_D: {
	    double s1, s2;
	    src1 = op->src(SRC1);
	    src2 = op->src(SRC2);
	    s1 = src1.double_value();
	    s2 = src2.double_value();
	    return (s1-s2);
	}

	default:
	    El_punt("El_evaluate_dble_arithmetic_op: unknown opcode");
	    return (0.0);
    }
}

/*
 *	Evaluate conversion ops which return an int value in Table 4 of the PlayDoh document
 */
int El_evaluate_int_conversion_op(Op *op)
{
    Opcode opc;
    Operand src1;
    
    opc = op->opcode();
    switch (opc) {
	case CONVSW: {
	    float s1;
	    src1 = op->src(SRC1);
	    s1 = src1.float_value();
	    return ((int)s1);
	}
	case CONVLSW: {
	    float s1;
	    src1 = op->src(SRC1);
	    s1 = src1.float_value();
	    return ((unsigned int)s1);
	}
	case CONVDW: {
	    double s1;
	    src1 = op->src(SRC1);
	    s1 = src1.double_value();
	    return ((int)s1);
	}
	case CONVLDW: {
	    double s1;
	    src1 = op->src(SRC1);
	    s1 = src1.double_value();
	    return ((unsigned int)s1);
	}
	case EXTS_B: {
	    int s1, shift_amount;
	    src1 = op->src(SRC1);
	    s1 = src1.int_value();
	    shift_amount = sizeof(int) - sizeof(char);
	    return (((s1 << shift_amount) >> shift_amount));
	}
	case EXTS_H: {
	    int s1, shift_amount;
	    src1 = op->src(SRC1);
	    s1 = src1.int_value();
	    shift_amount = sizeof(int) - sizeof(short);
	    return (((s1 << shift_amount) >> shift_amount));
	}
	case EXTZ_B: {
	    unsigned int s1;
	    src1 = op->src(SRC1);
	    s1 = src1.int_value();
	    return (s1 & 0xFF);
	}
	case EXTZ_H: {
	    unsigned int s1;
	    src1 = op->src(SRC1);
	    s1 = src1.int_value();
	    return (s1 & 0xFFFF);
	}
	default:
	    El_punt("El_evaluate_int_conversion_op: unknown opcode");
	    return (0);
    }
}

/*
 *      Evaluate conversion ops which return an float value in Table 4 of the PlayDoh document
 */
float El_evaluate_flt_conversion_op(Op *op)
{
    Opcode opc;
    Operand src1;

    opc = op->opcode();
    switch (opc) {
        case CONVWS: {
            int s1;
            src1 = op->src(SRC1);
            s1 = src1.int_value();
            return ((float)s1);
        }
        case CONVLWS: {
            unsigned int s1;
            src1 = op->src(SRC1);
            s1 = (unsigned int) src1.int_value();
            return ((float)s1);
        }
        case CONVDS: {
            double s1;
            src1 = op->src(SRC1);
            s1 = src1.double_value();
            return ((float)s1);
        }
	default:
	    El_punt("El_evaluate_flt_conversion_op: unknown opcode");
	    return (0);
    }
}

/*
 *      Evaluate conversion ops which return an double value in Table 4 of the PlayDoh document
 */
double El_evaluate_dbl_conversion_op(Op *op)
{
    Opcode opc;
    Operand src1;

    opc = op->opcode();
    switch (opc) {
        case CONVWD: {
            int s1;
            src1 = op->src(SRC1);
            s1 = src1.int_value();
            return ((double)s1);
        }
        case CONVLWD: {
            unsigned int s1;
            src1 = op->src(SRC1);
            s1 = (unsigned int) src1.int_value();
            return ((double)s1);
        }
        case CONVSD: {
            float s1;
            src1 = op->src(SRC1);
            s1 = src1.float_value();
            return ((double)s1);
        }
	default:
	    El_punt("El_evaluate_dbl_conversion_op: unknown opcode");
	    return (0);
    }
}

int El_evaluate_cmpr_op(Op *op)
{
    Opcode opc;
    Operand src1, src2;

    opc = op->opcode();
    switch (opc) {
        case CMPR_W_FALSE: {
	    return (0);
	}
        case CMPR_W_EQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 == s2);
        }
        case CMPR_W_LT: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 < s2);
        }
        case CMPR_W_LEQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 <= s2);
        }
        case CMPR_W_GT: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 > s2);
        }
        case CMPR_W_GEQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 >= s2);
        }
	/* Skip CMPR_W_SV, CMPR_W_OD, don't know how to evaluate these in C++ */
	case CMPR_W_TRUE: {
	    return (1);
	}
        case CMPR_W_NEQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 != s2);
        }
        case CMPR_W_LLT: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 < s2);
        }
        case CMPR_W_LLEQ: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 <= s2);
        }
        case CMPR_W_LGT: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 > s2);
        }
        case CMPR_W_LGEQ: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 >= s2);
        }
	/* Skip CMPR_W_NSV, CMPR_W_EV, don't know how to evaluate these in C++ */
        case FCMPR_S_FALSE: {
	    return (0);
	}
        case FCMPR_S_EQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 == s2);
        }
        case FCMPR_S_LT: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 < s2);
        }
        case FCMPR_S_LEQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 <= s2);
        }
        case FCMPR_S_GT: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 > s2);
        }
        case FCMPR_S_GEQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 >= s2);
        }
        case FCMPR_S_NEQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 != s2);
        }
        case FCMPR_S_TRUE: {
	    return (1);
        }
        case FCMPR_D_FALSE: {
	    return (0);
	}
        case FCMPR_D_EQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 == s2);
        }
        case FCMPR_D_LT: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 < s2);
        }
        case FCMPR_D_LEQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 <= s2);
        }
        case FCMPR_D_GT: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 > s2);
        }
        case FCMPR_D_GEQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 >= s2);
        }
        case FCMPR_D_NEQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 != s2);
        }
        case FCMPR_D_TRUE: {
            return (1);
        }
        default:
            El_punt("El_evaluate_cmpr_op: unknown opcode");
            return (0);
    }
}

// Return just the result of the comparison, caller responsible for interpretting
// the value based on predicate type(s)
bool El_evaluate_base_cmpp_op(Op *op)
{
    Opcode opc;
    IR_BASE_OPCODE baseopc;
    Operand src1, src2;

    opc = op->opcode();
    baseopc = get_base(opc);
    switch (baseopc) {
        case BASE_CMPP_W_FALSE: {
	    return (false);
	}
        case BASE_CMPP_W_EQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 == s2);
        }
        case BASE_CMPP_W_LT: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 < s2);
        }
        case BASE_CMPP_W_LEQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 <= s2);
        }
        case BASE_CMPP_W_GT: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 > s2);
        }
        case BASE_CMPP_W_GEQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 >= s2);
        }
	/* Skip BASE_CMPP_W_SV, BASE_CMPP_W_OD, don't know how to evaluate these in C++ */
	case BASE_CMPP_W_TRUE: {
	    return (true);
	}
        case BASE_CMPP_W_NEQ: {
            int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.int_value();
            s2 = src2.int_value();
            return (s1 != s2);
        }
        case BASE_CMPP_W_LLT: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 < s2);
        }
        case BASE_CMPP_W_LLEQ: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 <= s2);
        }
        case BASE_CMPP_W_LGT: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 > s2);
        }
        case BASE_CMPP_W_LGEQ: {
            unsigned int s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = (unsigned) src1.int_value();
            s2 = (unsigned) src2.int_value();
            return (s1 >= s2);
        }
	/* Skip BASE_CMPP_W_NSV, BASE_CMPP_W_EV, don't know how to evaluate these in C++ */
        case BASE_FCMPP_S_FALSE: {
	    return (false);
	}
        case BASE_FCMPP_S_EQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 == s2);
        }
        case BASE_FCMPP_S_LT: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 < s2);
        }
        case BASE_FCMPP_S_LEQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 <= s2);
        }
        case BASE_FCMPP_S_GT: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 > s2);
        }
        case BASE_FCMPP_S_GEQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 >= s2);
        }
        case BASE_FCMPP_S_NEQ: {
            float s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.float_value();
            s2 = src2.float_value();
            return (s1 != s2);
        }
        case BASE_FCMPP_S_TRUE: {
	    return (true);
        }
        case BASE_FCMPP_D_FALSE: {
	    return (false);
	}
        case BASE_FCMPP_D_EQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 == s2);
        }
        case BASE_FCMPP_D_LT: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 < s2);
        }
        case BASE_FCMPP_D_LEQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 <= s2);
        }
        case BASE_FCMPP_D_GT: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 > s2);
        }
        case BASE_FCMPP_D_GEQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 >= s2);
        }
        case BASE_FCMPP_D_NEQ: {
            double s1, s2;
            src1 = op->src(SRC1);
            src2 = op->src(SRC2);
            s1 = src1.double_value();
            s2 = src2.double_value();
            return (s1 != s2);
        }
        case BASE_FCMPP_D_TRUE: {
            return (true);
        }
        default:
            El_punt("El_evaluate_base_cmpp_op: unknown opcode");
            return (false);
    }
}

Op *El_create_move_for_cmpp_with_false_guard(Op *cmpp, Port_num p)
{
    Operand dest;
    Op *new_op=NULL;

    dest = cmpp->dest(p);
    if (dest.is_undefined())
	return (NULL);
    if (! is_U_pred(cmpp, p))
	return (NULL);

    Operand predF(new Pred_lit(0));
    Operand predT(new Pred_lit(1));
    new_op = new Op(MOVEPP);
    new_op->set_dest(DEST1, dest);
    new_op->set_src(SRC1, predF);
    new_op->set_src(PRED1, predT);

    return (new_op);
}

Op *El_create_move_for_cmpp_with_constant_condition(Op *cmpp, Port_num p, bool cond)
{
    Operand dest, pred;
    Op *new_op = NULL;

    dest = cmpp->dest(p);
    if (dest.is_undefined())
	return (NULL);

    Operand predF(new Pred_lit(0));
    Operand predT(new Pred_lit(1));
    pred = cmpp->src(PRED1);

    /* Unconditional type */
    /* UN, cond=1, dest = p if T; cond=0, dest = 0 if T */
    /* UC, cond=0, dest = p if T; cond=1, dest = 0 if T */
    if (is_U_pred(cmpp, p)) {
	new_op = new Op(MOVEPP);
	new_op->set_dest(DEST1, dest);
	new_op->set_src(PRED1, predT);
	if (is_UN_pred(cmpp, p)) {
	    if (cond)
		new_op->set_src(SRC1, pred);
	    else
		new_op->set_src(SRC1, predF);
	}
	else {
	    if (cond)
		new_op->set_src(SRC1, predF);
	    else
		new_op->set_src(SRC1, pred);
	}
    }

    /* Conditional type */
    /* CN: dest = cond if p; CC: dest = !cond if p */
    else if (is_C_pred(cmpp, p)) {
	new_op = new Op(MOVEPP);
	new_op->set_dest(DEST1, dest);
	new_op->set_src(PRED1, pred);
	if (is_CN_pred(cmpp, p)) {
	    Operand new_src(new Pred_lit(cond));
	    new_op->set_src(SRC1, new_src);
	}
	else {
	    Operand new_src(new Pred_lit(!cond));
	    new_op->set_src(SRC1, new_src);
	}
    }

    /* OR type */
    /* ON, cond=1: dest = T if p; OC, cond=0, dest = T if p, rest is NOOP */
    else if (is_O_pred(cmpp, p)) {
// 	if ((is_ON_pred(cmpp, DEST1) && cond) || (is_OC_pred(cmpp, DEST1) && !cond)) {
 	if ((is_ON_pred(cmpp, p) && cond) || (is_OC_pred(cmpp, p) && !cond)) {
	    new_op = new Op(MOVEPP);
	    new_op->set_dest(DEST1, dest);
	    new_op->set_src(PRED1, pred);
	    new_op->set_src(SRC1, predT);
	}
    }

    /* AND type */
    /* AN, cond=0: dest = F if p, AC, cond=1, dest = F if p, rest is NOOP */
    else if (is_A_pred(cmpp, p)) {
// 	if ((is_AN_pred(cmpp, DEST1) && !cond) || (is_AC_pred(cmpp, DEST1) && cond)) {
	if ((is_AN_pred(cmpp, p) && !cond) || (is_AC_pred(cmpp, p) && cond)) {
	    new_op = new Op(MOVEPP);
	    new_op->set_dest(DEST1, dest);
	    new_op->set_src(PRED1, pred);
	    new_op->set_src(SRC1, predF);
	}
    }

    else {
	El_punt("El_create_move_for_cmpp_with_constant_condition: unknown pred type");
    }

    return (new_op);
}


bool is_copy(Op *op)
{
    Opcode opc = op->opcode();
    if (! (opc == MOVE ||
            opc == MOVEF_S ||
            opc == MOVEF_D ||
            opc == MOVEPP ||
            opc == MOVEBB ||
            opc == MOVEGG
           ))
        return (false);

    /* SAM 1-2001 */
    /* Now check that this is truly a copy, and not an implicit
        conversion operation, don't copy propagate conversions */

    DType dt1 = El_get_dtype(op, DEST, DEST1);
    DType dt2 = El_get_dtype(op, SRC, SRC1);
    if (dt1.width() != dt2.width())
        return (false);
    if (dt1.is_signed() != dt2.is_signed())
        return (false);

    return (true);
}

bool is_cvt(Op *op)
{
    Opcode opc = op->opcode();
    if (! (opc == MOVE ||
            opc == MOVEF_S ||
            opc == MOVEF_D ||
            opc == MOVEPP ||
            opc == MOVEBB
           ))
        return (false);

    DType dt1 = El_get_dtype(op, DEST, DEST1);
    DType dt2 = El_get_dtype(op, SRC, SRC1);
    if ( (dt1.width() == dt2.width()) &&
         (dt1.is_signed() == dt2.is_signed()))
        return (false);

    return (true);
}

bool is_sign_change_cvt(Op *op)
{
    Opcode opc = op->opcode();
    if (! (opc == MOVE ||
            opc == MOVEF_S ||
            opc == MOVEF_D ||
            opc == MOVEPP ||
            opc == MOVEBB
           ))
        return (false);

    DType dt1 = El_get_dtype(op, DEST, DEST1);
    DType dt2 = El_get_dtype(op, SRC, SRC1);
    if (dt1.is_signed() == dt2.is_signed())
        return (false);

    return (true);
}

// Shivaram: 17:01:2000 Added the code for computation of saturation operations
int El_evaluate_satadd_op(int s1,int s2,int sp)
{
    int max = El_int_pow2(sp-1)-1;
    int min = -(El_int_pow2(sp-1)-1);
    int res = (s1+s2);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_sataddl_op(unsigned int s1,unsigned int s2,int sp)
{
    int max = El_int_pow2(sp)-1;
    int min = 0;
    int res = (s1+s2);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satsub_op(int s1,int s2,int sp)
{
    int max = El_int_pow2(sp-1)-1;
    int min = -(El_int_pow2(sp-1)-1);
    int res = (s1-s2);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satsubl_op(unsigned int s1,unsigned int s2,int sp)
{
    int max = El_int_pow2(sp)-1;
    int min = 0;
    int res = (s1-s2);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satmpy_op(int s1,int s2,int sp)
{
    int max = El_int_pow2(sp-1)-1;
    int min = -(El_int_pow2(sp-1)-1);
    int res = (s1*s2);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satmpyl_op(unsigned int s1,unsigned int s2,int sp)
{
    int max = El_int_pow2(sp)-1;
    int min = 0;
    int res = (s1*s2);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satmpyadd_op(int s1,int s2,int s3,int sp)
{
    int max = El_int_pow2(sp-1)-1;
    int min = -(El_int_pow2(sp-1)-1);
    int res = (s1*s2+s3);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satmpyaddn_op(int s1,int s2,int s3,int sp)
{
    int max = El_int_pow2(sp-1)-1;
    int min = -(El_int_pow2(sp-1)-1);
    int res = (-(s1*s2+s3));
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satmpysub_op(int s1,int s2,int s3,int sp)
{
    int max = El_int_pow2(sp-1)-1;
    int min = -(El_int_pow2(sp-1)-1);
    int res = (s1*s2-s3);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
int El_evaluate_satmpyrsub_op(int s1,int s2,int s3,int sp)
{
    int max = El_int_pow2(sp-1)-1;
    int min = -(El_int_pow2(sp-1)-1);
    int res = (s3-s1*s2);
    if (res>max) return max;
    if (res<min) return min;
    return res;
}
// Shivaram: 23-01-2000 Added the code for conversion of saturation opcode to
// normal opcode
Opcode convert_saturating_to_normal(Opcode opcode)
{
   IR_ROOT_OPCODE t = get_root(opcode) ;
   if (t == ROOT_SADD)   return ADD_W;
   if (t == ROOT_SADDL)  return ADDL_W;
   if (t == ROOT_SSUB)   return SUB_W;
   if (t == ROOT_SSUBL)  return SUBL_W;
   if (t == ROOT_SMPY)   return MPY_W;
   if (t == ROOT_SMPYL)  return MPYL_W;
   if (t == ROOT_SMPYADD)   return MPYADD_W;
   if (t == ROOT_SMPYADDN)  return MPYADDN_W;
   if (t == ROOT_SMPYSUB)   return MPYSUB_W;
   if (t == ROOT_SMPYRSUB)  return MPYRSUB_W;
  
   cerr << "\n*** convert_saturating_to_normal: warning unknown opcode:\n ";
   cerr << opcode << endl;

   return opcode;

}

/*
 *	SAM 3-2001.  A stupid n^2 implementation.  Just check if there is
 *	a single flow dependence if the ops are were sequential, so src_op
 *	produces an operand that is consumed by dst_op.  The function will
 *	punt if there are multiple flow deps.  Two refs corresponding to
 *	the end points of the flow dep are set if a flow dep is found.
 */
bool single_flow_dependence(Op *src_op, Op *dst_op, El_ref &src_ref, El_ref &dst_ref)
{
    Operand dest, src;
    bool found;

    found = false;
    for (Op_explicit_dests desti(src_op); desti!=0; desti++) {
        dest = *desti;
        for (Op_explicit_inputs srci(dst_op); srci!=0; srci++) {
            src = *srci;
            if (dest == src) {
                if (found)	// Multiple flow deps if found true
                    return (false);
                found = true;
                src_ref = desti.get_ref();
                dst_ref = srci.get_ref();
            }
        }
    }

    return (found);
}


