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
//                                                                       //
//      File:           el_opti_cvt_elim.cpp                             //
//      Authors:        Scott Mahlke                                     //
//      Created:        January 2001                                     //
//      Description:    Elimination of unnecessary CVTs (converts)       //
//                                                                       //
///////////////////////////////////////////////////////////////////////////
#include "dbg.h"
#include "op.h"
#include "region.h"
#include "reaching_defs_solver.h"
#include "attributes.h"
#include "op_attributes.h"
#include "width_utilities.h"
#include "el_opti_utilities.h"
#include "iterators.h"
#include "operand.h"
#include "el_global_cvt_elim.h"

// Return success or failure
bool El_do_cvt_elim_for_literal(Op *cvt_op)
{
    DType cvt_dest_dt, cvt_src_dt;
    Operand cvt_dest, cvt_src, new_src;
    int new_val, old_val;

    cvt_src = cvt_op->src(SRC1);
    cvt_src_dt = cvt_src.dtype();	// Here use the actual lit width
    old_val = cvt_src.int_value();
    cvt_dest = cvt_op->dest(DEST1);
    cvt_dest_dt = El_get_dtype(cvt_op, DEST, DEST1);

    // Here we will just chop the literal off according to the type of the dest!
    // Warn if the actual value is changed
    if (cvt_dest_dt.width() <= cvt_src_dt.width()) {
        new_val = update_value_for_width(old_val, cvt_dest_dt.width(),
						cvt_dest_dt.is_signed());
        if (new_val != old_val)
	    El_warn("El_do_cvt_elim_for_literal: change lit for op %d from %d to %d",
			    cvt_op->id(), old_val, new_val);
	if (cvt_dest_dt.is_signed()) {
            Operand temp_src(new Int_lit((int) new_val));
	    new_src = temp_src;
	}
	else {
            Operand temp_src(new Int_lit((unsigned int) new_val));
	    new_src = temp_src;
	}
    }
    else {
	if (cvt_src_dt.is_signed()) {
	    // If positive, just convert the literal to unsigned with the same value
	    if (old_val >= 0) {
		Operand temp_src(new Int_lit((unsigned) old_val));
		new_src = temp_src;
	    }
	    // If negative, enlarge the literal to match the dest width
	    else {
		new_val = update_value_for_width(old_val, cvt_dest_dt.width(),
						 cvt_dest_dt.is_signed());
		El_warn("El_do_cvt_elim_for_literal: change lit for op %d from %d to %x",
			      cvt_op->id(), old_val, new_val);
        	Operand temp_src(new Int_lit(new_val, cvt_dest_dt.width(),
					       cvt_dest_dt.is_signed()));
		new_src = temp_src;
	    }
	}
	else {	// unsigned
	    Operand temp_src(new Int_lit((int) old_val));
	    new_src = temp_src;
	}
    }

    if (dbg(opti, 6))
        cdbg << "\tCVT op " << cvt_op->id() << " replace " << cvt_src << " with "
             << new_src << endl;

    cvt_op->set_src(SRC1, new_src);
    cvt_op->reset_flag(EL_OPER_CVT);
    return (true);
}

bool El_do_cvt_elim_for_register(Op *cvt_op, List<El_ref> *cvt_defs,
		List<El_ref> *cvt_uses, Reaching_defs_info *rdef)
{
    DType cvt_dest_dt, cvt_src_dt;
    Operand cvt_dest, cvt_src;
    List<El_ref> *uses_of_def;
    List<El_ref> *defs_of_use;

    cvt_src = cvt_op->src(SRC1);
    cvt_src_dt = El_get_dtype(cvt_op, SRC, SRC1);
    cvt_dest = cvt_op->dest(DEST1);
    cvt_dest_dt = El_get_dtype(cvt_op, DEST, DEST1);

    // Convert from a wide to a narrow (wide def, narrow use)
    if (cvt_dest_dt.width() <= cvt_src_dt.width()) {
	// Here we will substitute the dest dtype at the producer
	if (cvt_defs->size() != 1) {
	    if (dbg(opti, 6))
	        cdbg << "Cannot elim a wide to narrow, multiple defs" << endl;
	    return (false);
	}

/* Shivarama Rao K. $$ Added on 23rd May $$
 * The check cvt_defs->size() !=1 checks only the multiple definitions
 * of this use. We also need to make sure that there are no multiple
 * uses of the definition of this use. This will lead to inconsistency
 * as we are changing the definition and this use to new operands
 */
	El_ref def_ref = cvt_defs->head();
	uses_of_def = rdef->get_du_chain(def_ref);
	if (uses_of_def->size() != 1) {
	    if (dbg(opti, 6))
	        cdbg << "Cannot elim a wide to narrow, multiple uses of def" << endl;
	    return (false);
	}


	Op *def_op = def_ref.get_op();

	Operand new_dest(new Reg(cvt_dest_dt));
	if (dbg(opti, 6)) {
	    cdbg << "Transform CVT, narrow <- wide" << endl;
	    cdbg << "Ops before:" << endl;
	    cdbg << *def_op << endl;
	    cdbg << *cvt_op << endl;
	}
	def_op->set_dest(def_ref.get_port_num(), new_dest);
	cvt_op->set_src(SRC1, new_dest);
	if (dbg(opti, 6)) {
	    cdbg << "Ops after:" << endl;
	    cdbg << *def_op << endl;
	    cdbg << *cvt_op << endl;
	}
	return (true);
    }
    // Convert from a narrow to a wide (narrow def, wide use)
    else {
	if (cvt_src_dt.is_signed()) {
	    if (dbg(opti, 6))
	        cdbg << "Cannot elim a narrow signed to wide unsigned cvt" << endl;
	    return (false);
	}
	// Otherwise this is a narrow unsigned to a wide signed conversion!
	if (cvt_uses->size() != 1) {
	    if (dbg(opti, 6))
	        cdbg << "Cannot elim a narrow unsigned to wide signed, mult uses of cvt" << endl;
	    return (false);
	}

/* Shivarama Rao K. $$ Added on 23rd May $$
 * The check cvt_uses->size() != 1 will check that there are no multiple
 * uses of this definition.But, we also need to make sure that there are
 * no multiple definitions for the use. Since we will be changing the 
 * operands of definition and use this scenario will lead to inconsistency
 */
        El_ref use_ref = cvt_uses->head();
	defs_of_use = rdef->get_ud_chain(use_ref);
	if (defs_of_use->size() != 1) {
	    if (dbg(opti, 6))
	        cdbg << "Cannot elim a narrow unsigned to wide signed, mult uses of cvt" << endl;
	    return (false);
        }

	Op *use_op = use_ref.get_op();

	Operand new_src(new Reg(cvt_src_dt));
	if (dbg(opti, 6)) {
	    cdbg << "Transform CVT, wide <- narrow" << endl;
	    cdbg << "Ops before:" << endl;
	    cdbg << *cvt_op << endl;
	    cdbg << *use_op << endl;
	}
	cvt_op->set_dest(DEST1, new_src);
	use_op->set_src(use_ref.get_port_num(), new_src);
	if (dbg(opti, 6)) {
	    cdbg << "Ops after:" << endl;
	    cdbg << *cvt_op << endl;
	    cdbg << *use_op << endl;
	}
	return (true);
    }
}

/*
 *	Width analysis is smart enough to eliminate CVTs except
 *	when they go from signed/unsigned or the reverse.  So,
 *	this opti tackles removing these more difficult ones.
 */
int El_do_cvt_elim(Compound_region *reg, Reaching_defs_info *rdef)
{
    Op *cvt_op;
    Operand *cvt_src, *cvt_dest;
    int count = 0;
    bool cvt_src_is_int_lit, cvt_src_is_reg;
    List<El_ref> *cvt_defs, *cvt_uses;
    DType cvt_dest_dt, cvt_src_dt;
    bool success;

    for (Region_all_ops opi(reg); opi!=0; opi++) {
	cvt_op = *opi;
	success = false;
	cvt_src_is_int_lit = false;
	cvt_src_is_reg = false;

	if (! is_sign_change_cvt(cvt_op))
	    continue;

	if (dbg(opti, 4)) {
	    cdbg << endl;
	    cdbg << "**Found a sign-change CVT: op " << cvt_op->id() << endl;
	    cdbg << "\tDEST dtype: " << El_get_dtype(cvt_op, DEST, DEST1) << endl;
	    cdbg << "\tSRC dtype: " << El_get_dtype(cvt_op, SRC, SRC1) << endl;
	}

	cvt_dest = &(cvt_op->dest(DEST1));
	El_ref cvt_dest_ref(cvt_op, cvt_dest, EXP_DEST);
	cvt_uses = rdef->get_du_chain(cvt_dest_ref);

	cvt_src = &(cvt_op->src(SRC1));
	El_ref cvt_src_ref(cvt_op, cvt_src, EXP_SRC);

	cvt_defs = NULL;
	if (cvt_src->is_int()) {
	    cvt_src_is_int_lit = true;
	}
	else if (cvt_src->is_reg()) {
	    cvt_src_is_reg = true;
	    cvt_defs = rdef->get_ud_chain(cvt_src_ref);
	}
	// Integer Literal CVT can always be converted into a copy 
	// Note.  Right now other types of literals will NOT be constant propagated.
	// This needs to be fixed in the future as this is too conservative of an approach.
	if (cvt_src_is_int_lit) {
	    if (dbg(opti, 4))
	        cdbg << "\tCVT src is an integer literal" << endl;
	    success = El_do_cvt_elim_for_literal(cvt_op);
	}
	// Otherwise CVT src is a reg, more conservative handling required here.
	else if (cvt_src_is_reg) {
	    if (dbg(opti, 4))
	        cdbg << "\tCVT src is a reg" << endl;
	    success = 
                 El_do_cvt_elim_for_register(cvt_op, cvt_defs, cvt_uses, rdef);
	}

	if (! success) {
	    if (dbg(opti, 4))
	        cdbg << "--------No opti, CVT elim failed! --------" << endl;
	    continue;
	}
	else {
	    if (dbg(opti, 4))
	        cdbg << "--------CVT elim succeeded! --------" << endl;
	}
	
	count++;
    }

    return (count);
}

#if 0
void bogus_cvt_elim(Compound_region *reg)
{
    Op *op;
    Operand cvt_src, cvt_dest;
    DType cvt_src_dt, cvt_dest_dt;
    Base_operand *destptr, *srcptr;

    for (Region_all_ops opi(reg); opi!=0; opi++) {
	op = *opi;
	if (! is_sign_change_cvt(op))
	    continue;

	cvt_src = op->src(SRC1);
	cvt_src_dt = El_get_dtype(op, SRC, SRC1);
	cvt_dest = op->dest(DEST1);
	cvt_dest_dt = El_get_dtype(op, DEST, DEST1);

	if (cvt_src.is_lit())
	    El_punt("bogus_cvt_elim: lit for op %d", op->id());
	if (cvt_src_dt.width() < cvt_dest_dt.width()) {
	    // reset the dest dtype
	    destptr = cvt_dest.get_ptr();
	    destptr->set_dtype(cvt_src_dt);
	}
	else {
	    // reset the src dtype
	    srcptr = cvt_src.get_ptr();
	    srcptr->set_dtype(cvt_dest_dt);
	}
    }
}
#endif

static void El_correct_cvt_flags(Compound_region *reg)
{
    Op *op;

    for (Region_all_ops opi(reg); opi!=0; opi++) {
	op = *opi;
	if (is_cvt(op))
	    op->set_flag(EL_OPER_CVT);
	else 
	    op->reset_flag(EL_OPER_CVT);
    }
}

// This is just for internal statistic keeping
#define MAX_CVT_COUNTS 3
static Vector<int> cvt_count(MAX_CVT_COUNTS, 0);

void El_opti_count_cvts(Compound_region *reg, int place)
{
    Op *op;
    int count = 0;
    El_correct_cvt_flags(reg);
    for (Region_all_ops opi(reg); opi!=0; opi++) {
	op = *opi;
	if (op->flag(EL_OPER_CVT)) {
	    if (dbg(opti,8))
	        cdbg << "Found a CVT" << endl << *op << endl;
	    count++;
	}
    }

    cvt_count[place] = count;
}

void El_opti_report_cvts()
{
    cdbg << "-----------------------------------" << endl;
    cdbg << "CVT op report" << endl;
    cdbg << "CVT count in input code: " << cvt_count[0] << endl;
    cdbg << "CVT count after width analysis: " << cvt_count[1] << endl;
    cdbg << "CVT count after cvt elim: " << cvt_count[2] << endl;
    cdbg << "-----------------------------------" << endl;
}

/*
 *      External routine, assume global liveness done before this!!
 */
int El_opti_cvt_elim(Compound_region *reg)
{
    Reaching_defs_info *rdef=NULL;
    int count=0;

    if (dbg(opti, 2))
        cdbg << "-- Global cvt elimination" << endl;

    if (reg == NULL)
	El_punt("El_opti_cvt_elim: region is NULL");

    /* Do local reaching def analysis */
    create_local_analysis_info_for_all_hbs_bbs(reg) ;
    El_do_reaching_defs(reg, ANALYZE_ALLREG);
    rdef = get_reaching_defs_info(reg);

    count += El_do_cvt_elim(reg, rdef);

    /* Cleanup reaching defs info */
    remove_reaching_defs_info(reg);

    return (count);
}
