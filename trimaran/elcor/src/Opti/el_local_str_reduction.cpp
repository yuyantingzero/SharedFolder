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
//      File:           el_local_str_reduction.cpp                       //
//      Authors:        Scott A. Mahlke                                  //
//      Created:        August 1999                                      //
//      Description:    Strength red optimizations                       //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "el_opti_utilities.h"

/*============================================================================*/
/*
 *	Strength reduction optimizations
 */
/*============================================================================*/

/*
 *	Strength reduction of multiply by a power of 2 to left shift
 *
 *	a = mpy (x,K) if p1  (where K is a power of 2)
 *	=>
 *	a = shl (x,log2(K)) if p1
 */
int El_opti_local_str_red_1(Compound_region *reg)
{
    Op *op;
    Operand src1, src2, *new_src, temp;
    int new_value, count=0;

    if (dbg(opti,3))
        cdbg << "\tEnter local strength reduction 1: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! is_int_mul(op))
	    continue;
	src1 = op->src(SRC1);
	src2 = op->src(SRC2);
	if (! (El_is_power_of_two(src1) ||
		El_is_power_of_two(src2)))
	    continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply strength reduction 1 (mul to shift): op " << op->id()
                 << " region " << reg->id() << endl;

	/* Put the constant in src2 */
	if (El_is_power_of_two(src1)) {
	    temp = op->src(SRC1);
	    op->src(SRC1) = op->src(SRC2);
	    op->src(SRC2) = temp;

	    // dcronqui 12-19-2000 Need to swap the Reference_dtype_attr width annots too!!!
	    Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
	    if (rdt_attr != NULL) {
	       rdt_attr->swap_srcs(SRC1, SRC2);
	    }
	}

	/* Change the opcode to left shift */
        if (op->opcode() == MPY_W)
	    op->set_opcode(SHLA_W);
	else
	    op->set_opcode(SHL_W);

	/* Change src2 to log2(src2) */
	new_value = El_int_log2(op->src(SRC2).int_value());
	new_src = new Operand (new Int_lit((unsigned) new_value));
        op->set_src(SRC2, *new_src);

	count++;
    }

    return (count);
}

/*
 *	Strength reduction of remainder by a power of 2 to logical and
 *
 *	a = rem (x,K) if p1  (where K is a power of 2)
 *	=>
 *	a = and (x,K-1) if p1
 *
 *	Note, this xform only valid if x is unsigned or positive signed.   Now,
 *	only perform for unsigned operations to be conservative.
 */
int El_opti_local_str_red_2(Compound_region *reg)
{
    Op *op;
    Operand src1, src2, *new_src, temp;
    DType dt;
    int new_value, count=0;

    if (dbg(opti,3))
        cdbg << "\tEnter local strength reduction 2: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;

	if (! is_int_rem(op))
	    continue;

	src2 = op->src(SRC2);
	if (!El_is_power_of_two(src2))
	    continue;

        // do this opti on unsigned REM only
        if (op->opcode() == REM_W)
            continue;

	if (dbg(opti,3))
	    cdbg << "\t\tApply strength reduction 2 (rem to and): op " << op->id()
                 << " region " << reg->id() << endl;

	/* Change the opcode to left shift */
	op->set_opcode(AND_W);

	/* Change src2 to src2-1 */
	new_value = op->src(SRC2).int_value() - 1;
	new_src = new Operand (new Int_lit(new_value));
        op->set_src(SRC2, *new_src);

	count++;
    }

    return (count);
}

/*
 *	Strength reduction of divide by a power of 2 to right shift
 *
 *	a = div (x,K) if p1  (where K is a power of 2)
 *	=>
 *	a = lsr (x,log2(K)) if p1
 *      a = asr (x,log2(K)) if p1 (when x is signed)
 *
 *	Note, this xform only valid if x is unsigned or positive signed.   Now,
 *	only perform for unsigned operations to be conservative.
 *      
 *      relaxed the constraint on x. x can be negative signed number.
 *      in that case, do Arithmetic shift. - parkhc 02/08/2005
 */
int El_opti_local_str_red_3(Compound_region *reg)
{
    Op *op;
    Operand src1, src2, *new_src, temp;
    DType dt;
    int new_value, count=0;

    if (dbg(opti,3))
        cdbg << "\tEnter local strength reduction 3: region " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;

	if (! is_int_div(op))
	    continue;

	src2 = op->src(SRC2);
	if (!El_is_power_of_two(src2))
	    continue;

	/* Change the opcode to right shift or right shift arithmetic */
        if (op->opcode() == DIV_W)
            op->set_opcode(SHRA_W);
        else 
            op->set_opcode(SHR_W);

	/* Change src2 to log2(src2) */
	new_value = El_int_log2(op->src(SRC2).int_value());
	new_src = new Operand (new Int_lit((unsigned) new_value));
        op->set_src(SRC2, *new_src);

	count++;
    }

    return (count);
}
/*============================================================================*/
/*
 *	External routine 
 */
/*============================================================================*/

/*
 *	Returns the number of applications
 */

int El_opti_local_strength_reduction(Compound_region *reg)
{
    int count=0;

    if (reg == NULL)
	El_punt("El_opti_local_strength_reduction: region is NULL");

    if (reg->is_hb() || reg->is_bb()) {
        /* Do the various forms of str reduction */

        /* 1. Convert multiply by power of 2 constant into left shift */
	count += El_opti_local_str_red_1(reg);

	/* 2. Convert remainder by power of 2 to logical and */
	count += El_opti_local_str_red_2(reg);

	/* 3. Convert divide by power of 2 into right shift */
	count += El_opti_local_str_red_3(reg);

    }
    else {
	/* We only do local opti, so if region is larger than bb/hb, go
	   through the subregions */

	if (dbg(opti, 1))
	    cdbg << "-- Local strength reduction" << endl;

	for (Region_subregions subri(reg); subri!=0; subri++) {
	    Compound_region *subr = (Compound_region *) *subri;
	    count += El_opti_local_strength_reduction(subr);
	}
    }

    return (count);
}
