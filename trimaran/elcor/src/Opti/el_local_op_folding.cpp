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
//      File:           el_local_op_folding.cpp                          //
//      Authors:        Scott A. Mahlke                                  //
//      Created:        July 1999                                        //
//      Description:    Operation folding optimizations                  //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "operand.h"
#include "op.h"
#include "opcode_properties.h"
#include "width_utilities.h"
#include "el_opti_utilities.h"

/*
 *	Combine cmpr/cmpp into single cmpp, assume cmpps have been normalized
 *	so any literals are in src2.
 *
 *	a = cmpr_cc (x,y) if p1
 *	q = cmpp_eq/ne (a, (1 or 0)) if p1
 *  =>
 *	a = cmpp_cc (x,y) if p1 (later deleted by dead code elim)
 *	q = cmpp_cc/~cc (x,y) if p1
 */
int El_opti_local_op_fold_1(Compound_region *reg, Reaching_defs_info* rdef)
{
    int count=0, const_val;
    Op *op, *def_op;
    Operand *src1, *src2, *const_src, *other_src;
    bool opposite;
    Opcode new_opc, new_base_opc;

    if (dbg(opti, 3))
        cdbg << "\tEnter local op fold 1 for reg " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! is_cmpp(op))
	    continue;
	if (! (is_int_eq_cmpp(op) || is_int_neq_cmpp(op)))
	    continue;
	src1 = &(op->src(SRC1));
	src2 = &(op->src(SRC2));
	if (src1->is_int()) {
	    const_src = src1;
	    other_src = src2;
	}
	else if (src2->is_int()) {
	    const_src = src2;
	    other_src = src1;
	}
	else {
	    continue;
	}

	const_val = const_src->int_value();
	if (! ((const_val == 0) || (const_val == 1)))
	    continue;

        if (!other_src->is_reg() && !other_src->is_macro_reg())
            continue;

	El_ref other_ref (op, other_src, EXP_SRC);
	List<El_ref> *defs = rdef->get_ud_chain(other_ref);
	if (defs->size() != 1)
	    continue;

	El_ref def_ref = defs->head();
	if (def_ref.get_ref_type() != EXP_DEST)
	    continue;

	def_op = def_ref.get_op();
	if (! is_cmpr(def_op))
	    continue;

  // parkhc 02/01/2005
  // El_no_defs_between function doesn't catch the case when
  // CMPR is self modifying op.
  // So, let's check that here before calling the function
  bool self_modifying = false;
  for (Op_all_dests d_iter(def_op); d_iter != 0; d_iter++) {
    Operand dest = *d_iter;
    if (dest == def_op->src(SRC1) || dest == def_op->src(SRC2)) {
      self_modifying = true;
      break;
    }
  }
  if (self_modifying)
    continue;

	/* Check that the srcs of the cmpr are not redefined between
		the cmpr and cmpp */
	if (! El_no_defs_between(reg, def_op, op, def_op->src(SRC1)))
	    continue;
	if (! El_no_defs_between(reg, def_op, op, def_op->src(SRC2)))
	    continue;

	/* Check that the predicate of the cmpr dominates/equal to the
		predicate of the cmpp */
	if (! El_predicate_dominates(reg, def_ref, other_ref))
	    continue;

	/* Looks good, we have an opti opportunity, woo woo!! */
	if (dbg(opti,3))
	    cdbg << "\t\t Apply op folding 1 (cmpr/cmpp): op " << def_op->id()
		 << " : " << op->id() << " region " << reg->id() << endl;

	/* Copy the sources of the cmpr into the cmpp */
	op->set_src(SRC1, def_op->src(SRC1));
	op->set_src(SRC2, def_op->src(SRC2));

	/* update the rdt_attr needed because its src operands are changed */
	Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
	if (rdt_attr != NULL)
	    rdt_attr->set_srcs_conservatively(op);

	/* Adjust the compare condition of the cmpp */
	if (is_int_eq_cmpp(op)) {
	    if (const_val == 0)
		opposite = true;
	    else
		opposite = false;
	}
	else {
	    if (const_val == 0)
		opposite = false;
	    else
		opposite = true;
	}
	new_base_opc = convert_CMPR_to_base_CMPP(def_op->opcode());
	if (opposite)
	    new_base_opc = get_opposite_cond(new_base_opc);
	new_opc = change_base_opcode(op->opcode(), new_base_opc);
	op->set_opcode(new_opc);

	count++;
    }

    return (count);
}

/*
 *	Create mpyadd and mpysub by combining ops
 *
 *	d = a * b if p1
 *	e = d +- c if p2
 *  =>
 *	d = a * b if p1 (dead code will nuke this)
 *	e = a * b +- c if p2
 */
int El_opti_local_op_fold_2(Compound_region *reg, Reaching_defs_info* rdef)
{
    int count=0;
    Op *op, *use_op;
    Operand *mul_dest, src1, src2, src3, pred, dest;
    bool rflag;
    Opcode new_opc;


    if (dbg(opti, 3))
        cdbg << "\tEnter local op fold 2 for reg " << reg->id() << endl;

    for (Region_ops_C0_order opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! is_mul(op))
	    continue;
	mul_dest = &(op->dest(DEST1));

	El_ref mul_dest_ref(op, mul_dest, EXP_DEST);
	List<El_ref> *uses = rdef->get_du_chain(mul_dest_ref);
	if (uses->size() != 1)
	    continue;

	El_ref use_ref = uses->head();
	if (use_ref.get_ref_type() != EXP_SRC)
	    continue;

	use_op = use_ref.get_op();
	if (! (is_add(use_op) || is_sub(use_op)))
	    continue;

        // Shivarama Rao K
        // 17-01-2001.. Added the code to support saturating operations
        /*
	if ( (is_int_satadd(use_op) || is_int_satsub(use_op)))
	    continue;
        */
        int sp1 = 33;  // maximum integer width support
        int sp2 = 33;  // maximum integer width support
        if (is_int_satmul(op->opcode()) )
        {
           sp1 = extract_saturate_point(op->opcode());
        } 

        if (is_int_satadd(use_op->opcode()) ||
           is_int_satsub(use_op->opcode()))
        {
           sp2 = extract_saturate_point(use_op->opcode());
        }

        if (sp1 < sp2)  {

        // In this case the first operation saturates before 2nd
        // operation. The combined result will be incorrect
        // e.g: saturate_add6(saturate_mul3(5*5), 5) == 9 is different
        // from saturatempyadd6(5,5,5) == 30

        // e.g: saturate_add3(saturate_mul6(5*5), 5) == 30 is different
        // from saturatempyadd6(5,5,5) == 30
           return false;
        }

	/* Check that the srcs of the multiply are not redefined between
	   the multiply and the use */
	if (! El_no_defs_between(reg, op, use_op, op->src(SRC1)))
	    continue;
	if (! El_no_defs_between(reg, op, use_op, op->src(SRC2)))
	    continue;

	/* Check that the predicate of the mul dominates/equal to the
                predicate of the add/sub */
        if (! El_predicate_dominates(reg, mul_dest_ref, use_ref))
            continue;

	/* Looks good, make the mpyadd/mpysub */
        if (dbg(opti,3))
            cdbg << "\t\t Apply op folding 2 (mpy/add,sub): op " << op->id()
                 << " : " << use_op->id() << " region " << reg->id() << endl;

	// The attributes for the three operands will be stored here.
	// These will only be used if attributes are defined on
	// use_op, since that is the operation that will become the
	// multadd/multsub.  dcronqui 12/29/00.
	DType src1_dt, src2_dt, src3_dt;
	Reference_dtype_attr *rdt_attr_use_op = get_reference_dtype_attr(use_op);

	// Identify all the operands for the mpyadd/sub
	if (use_ref.get_port_num() == SRC1) {
	    src3 = use_op->src(SRC2);
	    src3_dt = El_get_dtype(use_op, SRC, SRC2);
	    rflag = false;
	}
	else {
	    src3 = use_op->src(SRC1);
	    src3_dt = El_get_dtype(use_op, SRC, SRC1);
	    rflag = true;
	}
	src1 = op->src(SRC1);
	src2 = op->src(SRC2);
	src1_dt = El_get_dtype(op, SRC, SRC1);
	src2_dt = El_get_dtype(op, SRC, SRC2);
	pred = use_op->src(PRED1);
	dest = use_op->dest(DEST1);

	if (is_add(use_op))
	    new_opc = convert_ADD_to_MPYADD(use_op->opcode(), false);
	else
	    new_opc = convert_SUB_to_MPYSUB(use_op->opcode(), rflag);

	// update_num_operands() blows away all operands, so need to reset
	// everything after calling this.
	use_op->set_opcode(new_opc);
	use_op->update_num_operands();

	use_op->set_dest(DEST1, dest);
	use_op->set_src(PRED1, pred);
	use_op->set_src(SRC1, src1);
	use_op->set_src(SRC2, src2);
	use_op->set_src(SRC3, src3);

	// if there are port attributes, create new attributes for the
	// multadd/multsub ports.  dcronqui, 12/29/00.
	if (rdt_attr_use_op != NULL) {
	   rdt_attr_use_op->resize(use_op);
	   rdt_attr_use_op->set_src(SRC1, src1_dt);
	   rdt_attr_use_op->set_src(SRC2, src2_dt);
	   rdt_attr_use_op->set_src(SRC3, src3_dt);
	}

	count++;
    }

    return (count);
}


/*
 *	External routine, assume global liveness done before this!!
 */
int El_opti_local_op_folding(Compound_region *reg)
{
    Reaching_defs_info *rdef=NULL;
    int count=0;

    if (reg == NULL)
	El_punt("El_opti_local_op_folding: region is NULL");

    if (reg->is_hb() || reg->is_bb()) {
	/* Do local reaching def analysis for the hb/bb */
	El_do_reaching_defs(reg, ANALYZE_ALLREG);
	rdef = get_reaching_defs_info(reg);

        /* Do the various forms of operation folding */
	/* 1. Merge cmpr/cmpp combo into a single cmpp */
	count += El_opti_local_op_fold_1(reg, rdef);

	/* 2. Merge cmpr/cmpp combo into a single cmpp */
        /* Shivarama Rao K: 28-02-2001. pattern matcher is in place 
         * for mpyadd and mpysub
         */
	/* count += El_opti_local_op_fold_2(reg, rdef); */

	/* Cleanup reaching defs info */
	remove_reaching_defs_info(reg);
    }
    else {
	/* We only do local op folding, so if region is larger than bb/hb, go
	   through the subregions */

	if (dbg(opti, 1))
	    cdbg << "-- Local operation folding" << endl;

	for (Region_subregions subri(reg); subri!=0; subri++) {
	    Compound_region *subr = (Compound_region *) *subri;
	    count += El_opti_local_op_folding(subr);
	}
    }

    return (count);
}
