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



/*===========================================================================
 *
 *      File:           el_process_func.cpp
 *      Author:         Tim Sherwood
 *      Created:        Aug 1999
 *      Description:    Pseudo main routine for Elcor, called for each
 *			procedure in the input.
 *
 *===========================================================================*/

#include "defs.h"
#include "region.h"
#include "op.h"
#include "operand.h"
#include "iterators.h"
#include "el_width_analysis.h"
#include "wconstraint.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "el_width_propagation.h"
#include "attributes.h"
#include "op_attributes.h"
#include "opcode_properties.h"
#include "el_control.h"
#include "opcode.h"
#include "opcode_load_store.h"
#include "region_utilities.h"
#include "width_utilities.h"


//---------------------------------------------------------------------------//
//   Main Loop: do_width_analysis
//---------------------------------------------------------------------------//
void do_width_analysis(Compound_region *r, bool run_rdefs )
{ 
    WidthAnalysis widthanalysis;

    if(widthanalysis.debug_flag) cerr << "begining do_width_analysis\n";

    /*
     *	Preprocessing of all the widths (SAM 2-2000)
     */
    if (El_anal_width_preprocess_level == 0) {
	; /* Do nothing */
    }
    else if (El_anal_width_preprocess_level == 1) {
	El_promote_all_operands_to_C_width(r);
    }
    else if (El_anal_width_preprocess_level == 2) {
	El_promote_all_operands_to_full_width(r);
    }
    else {
	El_punt("do_width_analysis: unknown value for preprocess_width_level");
    }

    /*
     *	Width analysis itself
     */
    if (El_anal_width_analysis_level == 0) {
	; /* Do nothing */
    }
    else if (El_anal_width_analysis_level == 2) {
        if (run_rdefs == true) {
	    create_local_analysis_info_for_all_hbs_bbs(r) ;
	    El_do_reaching_defs (r, widthanalysis.filter_flag) ;
        }
        widthanalysis.rd_info = get_reaching_defs_info (r) ;
    
        //Operands -> reference map
        widthanalysis.init_current_constraint( r );
        //if we are is debug mode then print out a file with cost estimates
        if(widthanalysis.debug_flag) widthanalysis.estimate_width_savings( r, "REB_opti.gate_est" );
    
        //MAIN BODY OF ANALYSIS
        do {
	    widthanalysis.forwardchange = false;
	    if ( widthanalysis.debug_flag ) cerr << "doing width analysis down" << endl;
	    widthanalysis.process_function( r, WidthAnalysis::PROCESS_FORWARD );
    
	    widthanalysis.backwardchange = false;
	    if ( widthanalysis.debug_flag ) cerr << "doing width analysis up" << endl;
	    widthanalysis.process_function( r, WidthAnalysis::PROCESS_BACKWARD );

	    widthanalysis.converge = (!widthanalysis.forwardchange)&&(!widthanalysis.backwardchange);
        } while ( ! widthanalysis.converge );

        //add annotates to all the ops in r with the ref types
        widthanalysis.decorate_ops( r );
    
        //update the registers with the new reduced values
        //reference map -> Operands
        widthanalysis.reduce_region( r );
    
        delete_local_analysis_info_for_all_hbs_bbs(r) ;
    
        //if we are is debug mode then print out a file with cost estimates
        if(widthanalysis.debug_flag) widthanalysis.estimate_width_savings( r,"REB_widthanalysis.gate_est" );
        if(widthanalysis.debug_flag) cerr << "done do_width_analysis\n";
    }
    else {
	El_punt("do_width_analysis: unknown value for width_analysis_level");
    }

    /*
     *	Postprocessing of all the widths (SAM 2-2000)
     */
    if (El_anal_width_postprocess_level == 0) {
	; /* Do nothing */
    }
    else if (El_anal_width_postprocess_level == 1) {
	El_promote_all_operands_to_C_width(r);
	El_promote_all_annotations_to_C_width(r);
    }
    else if (El_anal_width_postprocess_level == 2) {
	El_promote_all_operands_to_full_width(r);
	El_promote_all_annotations_to_full_width(r);
    }
    else {
	El_punt("do_width_analysis: unknown value for postprocess_width_level");
    }

#if 0
    // For Scott's use only right now!
    El_estimate_gate_count_ii1(r);
#endif
}

//---------------------------------------------------------------------------//
//    Shortcut process function that just sets everything to 32 bits
//---------------------------------------------------------------------------//
void El_promote_all_operands_to_full_width(Compound_region *r)
{
    Operand dest, src;
    Base_operand *destptr, *srcptr;
    DType dt;
    Op *op;
    for (Region_all_ops op_i(r); op_i!=0; op_i++) {
        op = *op_i;
        for (Op_explicit_dests dest_i(op); dest_i!=0; dest_i++) {
            dest = *dest_i;
            if (dest.is_predicate())
                continue;
            if (dest.is_reg() && (dest.file_type()==PR))
                continue;
            destptr = dest.get_ptr();
            dt = dest.dtype();
            if (dt.width() < 32) {
                DType ndt(dt);
                ndt.set_width(32);
                destptr->set_dtype(ndt);
            }
        }
        for (Op_explicit_sources src_i(op); src_i!=0; src_i++) {
            src = *src_i;
            if (src.is_predicate())
                continue;
            if (src.is_reg() && (src.file_type()==PR))
                continue;
            srcptr = src.get_ptr();
            dt = src.dtype();
            if (dt.width() < 32) {
                DType ndt(dt);
                ndt.set_width(32);
                srcptr->set_dtype(ndt);
            }
        }
    }
}

void El_promote_all_annotations_to_full_width(Compound_region *r)
{
    DType dt;
    Op *op;
    int i, num_dsts, num_srcs;
    Reference_dtype_attr *width_attr;

    for (Region_all_ops op_i(r); op_i!=0; op_i++) {
        op = *op_i;
	width_attr = get_reference_dtype_attr(op);
	if (width_attr == NULL)
	    continue;
	num_dsts = width_attr->num_dest();
	for (i=1; i<=num_dsts; i++) {
            dt = width_attr->dest((Port_num) i);
	    // SAM 4-2001, backend dies if promote preds to 32-bit
	    if (dt.info() == EL_DT_INFO_PREDICATE)
		continue;
            if (dt.width() < 32) {
                DType ndt(dt);
                ndt.set_width(32);
                width_attr->set_dest(((Port_num) i), ndt);
            }
	}
	num_srcs = width_attr->num_src();
	for (i=1; i<=num_srcs; i++) {
            dt = width_attr->src((Port_num) i);
	    // SAM 4-2001, backend dies if promote preds to 32-bit
	    if (dt.info() == EL_DT_INFO_PREDICATE)
		continue;
            if (dt.width() < 32) {
                DType ndt(dt);
                ndt.set_width(32);
                width_attr->set_src(((Port_num) i), ndt);
            }
	}
    }
}

//---------------------------------------------------------------------------//
//    Shortcut process function that just sets everything to its nearest C width
//---------------------------------------------------------------------------//
void El_promote_all_operands_to_C_width(Compound_region *r)
{
    Operand dest, src;
    Base_operand *destptr, *srcptr;
    DType dt;
    Op *op;
    int w, cw;
    for (Region_all_ops op_i(r); op_i!=0; op_i++) {
        op = *op_i;
        for (Op_explicit_dests dest_i(op); dest_i!=0; dest_i++) {
            dest = *dest_i;
            if (dest.is_predicate())
                continue;
            if (dest.is_reg() && (dest.file_type()==PR))
                continue;
            destptr = dest.get_ptr();
            dt = dest.dtype();
	    w = dt.width();
	    cw = dt.upconv_width();
	    if (w != cw) {
                DType ndt(dt);
                ndt.set_width(cw);
                destptr->set_dtype(ndt);
            }
        }
        for (Op_explicit_sources src_i(op); src_i!=0; src_i++) {
            src = *src_i;
            if (src.is_predicate())
                continue;
            if (src.is_reg() && (src.file_type()==PR))
                continue;
            srcptr = src.get_ptr();
            dt = src.dtype();
	    w = dt.width();
            cw = dt.upconv_width();
            if (w != cw) {
                DType ndt(dt);
                ndt.set_width(cw);
                srcptr->set_dtype(ndt);
            }
        }
    }
}

void El_promote_all_annotations_to_C_width(Compound_region *r)
{
    DType dt;
    Op *op;
    int i, num_dsts, num_srcs, w, cw;
    Reference_dtype_attr *width_attr;

    for (Region_all_ops op_i(r); op_i!=0; op_i++) {
        op = *op_i;
	width_attr = get_reference_dtype_attr(op);
	if (width_attr == NULL)
	    continue;
	num_dsts = width_attr->num_dest();
	for (i=1; i<=num_dsts; i++) {
            dt = width_attr->dest((Port_num)i);
	    // SAM 4-2001, backend dies if promote preds to 32-bit
	    if (dt.info() == EL_DT_INFO_PREDICATE)
		continue;
	    w = dt.width();
	    cw = dt.upconv_width();
	    if (w != cw) {
                DType ndt(dt);
                ndt.set_width(cw);
                width_attr->set_dest(((Port_num)i), ndt);
            }
	}
	num_srcs = width_attr->num_src();
	for (i=1; i<=num_srcs; i++) {
            dt = width_attr->src((Port_num)i);
	    // SAM 4-2001, backend dies if promote preds to 32-bit
	    if (dt.info() == EL_DT_INFO_PREDICATE)
		continue;
	    w = dt.width();
	    cw = dt.upconv_width();
            if (w != cw) {
                DType ndt(dt);
                ndt.set_width(cw);
                width_attr->set_src(((Port_num)i), ndt);
            }
	}
    }
}

//---------------------------------------------------------------------------//
//    Process Function
//---------------------------------------------------------------------------//
void WidthAnalysis::process_function( Region *r, WidthAnalysis::Direction dir )
{
    //intialize the working constraint (to forward or backward)
    init_working_constraint( r, dir ); 

    //iterate untill completion
    do {
	local_converge = true;
	process_region( r, dir );
    } while( ! local_converge );

    //copy over the working constraint to the current constraint
    update_current_constraint( r, dir );
}

//---------------------------------------------------------------------------//
//    Process Region
//---------------------------------------------------------------------------//
// take a region and bust it down into hb and bbs
void WidthAnalysis::process_region( Region *r, WidthAnalysis::Direction dir )
{
    if ( r->is_bb() || r->is_hb() )
    {
	if ( dir == WidthAnalysis::PROCESS_FORWARD )       process_block_forward( r );
	else if ( dir == WidthAnalysis::PROCESS_BACKWARD ) process_block_backward( r );
	else El_punt( "WidthAnalysis::process_region: called with unknown processing direction");
    } 
    else if ( ! r->is_op() ) 
    {
	if ( dir == WidthAnalysis::PROCESS_FORWARD ) { //direction is down
	      //start at top of region and work down
              for (Region_subregions subi(r); subi!=0; subi++) {
		Region *cr = *subi;
	    	process_region( cr, dir );
              }
	} else { // direction is up 
	      //start at bottom of region and work up
              for (Region_subregions_bidir subi(r,true); subi!=0; subi--) {
		Region *cr = *subi;
	    	process_region( cr, dir);
              }
	}
    } 
    else 
    {
	El_punt("WidthAnalysis::process_region: cannot call function on an op");
    }

}

//---------------------------------------------------------------------------//
//    Process Block Forward
//---------------------------------------------------------------------------//
// take a basic or hyper block and do the width analysis on it
// Note: this is the point where we actually do the forward flow equations
// note: literals are checked in the current width
void WidthAnalysis::process_block_forward( Region *block )
{
    assert(  block->is_bb() || block->is_hb() );

    bool differ;
    
    for( Region_ops_C0_order subi((Compound_region*)block); subi!=0; subi++ )
    {
	bool op_differ = false;
	Op *cur_op = *subi;

  	//build up a list of sources
	// must use num_srcs, not MAX.  this is freed at end of the loop.
	// dcronqui, 1/2/01
	// ... but the destructor leads to a seg fault!  damn.  i am just
	// going to hack this for now and alloc 5* the max.
        //WConstraint *srcconstraints= new WConstraint[cur_op->num_srcs()+1];
        WConstraint srcconstraints[5*MAX_SRC_PORTS];
	//fprintf(stderr, "%d AAAAA allocated %d....\n", cur_op->id(), cur_op->num_srcs()+1); 
	int num_constraints = 0;
	
	// for each source
	for (Op_explicit_sources srci(cur_op) ; srci != 0 ; srci++)
	{
	    El_ref cur_src = srci.get_ref();
	    Operand cur_opr = cur_src.get_operand();

	    // source constraint = max ( def of the src ) but not bigger than before
	    WConstraint maxsrc, thissrc;

	    // if the src is interesting
	    if ( rd_info->is_interesting( cur_opr ) )
	    { 
	    	List<El_ref>* deflist = rd_info->get_ud_chain(cur_src);
	    	int lcount = 0;
	    	for (List_iterator<El_ref> defi(*deflist) ; defi != 0 ; defi++) 
	    	{
			El_ref& cur_def = *defi ;

                        // if the def in a live in
                        if ( cur_def.get_ref_type() == LIVEIN_DEST ) {
                                thissrc.set_unconstrained();
                        } else {
                                assert ( working_constraint.is_bound( cur_def ) );
                                thissrc = *(working_constraint.value(cur_def));
                        }

                        //set max to the first in the list
                        if ( lcount == 0 ) maxsrc = thissrc;
                        else maxsrc.set_to_max( thissrc );
                        lcount++;
                }

                //make sure that the max is less than current width
                assert( current_constraint.is_bound( cur_src ) );
                assert( working_constraint.is_bound( cur_src ) );
		WConstraint maxout = *current_constraint.value(cur_src);
                differ = working_constraint.value(cur_src)->ratchet_up( maxsrc, maxout );
	    	if ( differ ) {
		     op_differ = true; 
		     local_converge = false;
		}

	        // add the constraint to the end of the list
	        srcconstraints[num_constraints] = ( *working_constraint.value(cur_src) );
	        num_constraints++;
	    }
	    else // if not interesting it should be a literal,undefined, or vrname
	    {
		assert ( cur_opr.is_undefined() || cur_opr.is_lit() || cur_opr.is_vr_name() );

	        // add the constraint to the end of the list (from the current set not the working set)
	        srcconstraints[num_constraints] = ( *current_constraint.value(cur_src) );
	        num_constraints++;
	    }

	}

	//for each dest
	int destnum=0;
	for (Op_explicit_dests desti(cur_op) ; desti != 0 ; desti++)
	{
	    destnum ++;
	    El_ref cur_dest = desti.get_ref();
	    assert ( working_constraint.is_bound(cur_dest) );
	    WConstraint* orig_constraint= working_constraint.value(cur_dest);
	    assert ( current_constraint.is_bound(cur_dest) );
	    WConstraint* maxout_constraint= current_constraint.value(cur_dest);

	    //////////////////////////////////////////////////////////////////////////////
	    // This is the heart of the analysis, it computes the new constraint from	//
	    // the old constraint and updates orig_constraint.  			//
	    //////////////////////////////////////////////////////////////////////////////
	    compute_width_propagation_down( 
		orig_constraint,
		maxout_constraint,
		destnum, 
		cur_op->opcode(), 
		srcconstraints,
		num_constraints,
		debug_flag, 
		op_differ
	       );

	}

	// free memory allocated for srcconstraints
	// Deallocation causes a seg fault... i don't have
	// time to trace this down right now.  but a "Bad free" seems
	// to occur in the DType destructor
	//        delete srcconstraints;
	//fprintf(stderr, "AAAAA deallocated\n"); 
    }//end for region
	
}

//---------------------------------------------------------------------------//
//    Process Block Backward
//---------------------------------------------------------------------------//
// take a basic or hyper block and do the width analysis on it
// Note: this is the point where we actually do the backward flow equations
void WidthAnalysis::process_block_backward( Region *block )
{
    assert(  block->is_bb() || block->is_hb() );

    bool differ;
    ;

    for( Region_ops_reverse_C0_order subi((Compound_region*)block); subi!=0; subi++ )
    {
	bool op_differ = false;
	Op *cur_op = *subi;

  	//build up a list of dests
	// must use num_dests, not MAX.  this is freed at end of the loop.
	// i allocate one extra to cleanly handle ops with no dests or no srcs.
	// dcronqui, 1/2/01
	// ... but the destructor leads to a seg fault!  damn.  i am just
	// going to hack this for now and alloc 5* the max.
        WConstraint destconstraints[5*MAX_DEST_PORTS];
        //destconstraints = new WConstraint[cur_op->num_dests()+1];
	//fprintf(stderr, "%d BBBBBB allocated %d....\n", cur_op->id(), cur_op->num_dests()+1); 
	int num_destconstraints = 0;

  	//build up a list of sources
	// must use num_srcs, not MAX.  this is freed at end of the loop.
	// dcronqui, 1/2/01
        WConstraint srcconstraints[5*MAX_SRC_PORTS];
	//srcconstraints = new WConstraint[cur_op->num_srcs()+1];
	//fprintf(stderr, "%d CCCCCC allocated %d....\n", cur_op->id(), cur_op->num_srcs()+1); 
	int num_srcconstraints = 0;
	
	// for each dest
	for (Op_explicit_dests desti(cur_op) ; desti != 0 ; desti++)
	{
	    El_ref cur_dest = desti.get_ref();
	    Operand cur_opr = cur_dest.get_operand();

	    // dest constraint = max ( use of the dest ) but not bigger than before
	    WConstraint maxdest, thisdest;

	    // if the dest is interesting
	    if ( rd_info->is_interesting( cur_opr ) )
	    { 
	    	List<El_ref>* uselist = rd_info->get_du_chain(cur_dest);
	    	int lcount = 0;
	    	for (List_iterator<El_ref> usei(*uselist) ; usei != 0 ; usei++) 
	    	{
			El_ref& cur_use = *usei ;


			// if the use in a live out
			if ( cur_use.get_ref_type()==LIVEOUT_TAKEN_SRC ||
			     cur_use.get_ref_type()==LIVEOUT_FALLTHROUGH_SRC ||
			     cur_op->flag(EL_OPER_LIVEOUT_DEST) ) {
				thisdest.set_unconstrained();
			} else { 
				if ( ! working_constraint.is_bound(cur_use) ) {
				     cerr << "\n***\n*** Use " << cur_use << " in " 
					 << *cur_op << " is not bound\n***\n";
				     El_punt("WidthAnalysis::process_block_backward: stopping for above reason\n");
				}

				thisdest = *(working_constraint.value(cur_use));
			}
	
			//set max to the first in the list
			if ( lcount == 0 ) maxdest = thisdest;
			else maxdest.set_to_max( thisdest );
			lcount++;
	    	}

                WConstraint maxout = *current_constraint.value(cur_dest);
                differ = working_constraint.value(cur_dest)->ratchet_up( maxdest, maxout );
                if ( differ ) { 
		     op_differ = true;
                     local_converge = false;
                }
		// add the constraint to the end of the list
	    	destconstraints[num_destconstraints] = ( *working_constraint.value(cur_dest) );
	   	num_destconstraints++;

	    }
	    else // it should be a literal or undefined
	    {
		assert( cur_opr.is_undefined() || cur_opr.is_lit() || cur_opr.is_vr_name() );
		// add the constraint to the end of the list (from the current not the working)
	    	destconstraints[num_destconstraints] = ( *current_constraint.value(cur_dest) );
	   	num_destconstraints++;
	    }

	}


	//copy over the source constraints
	int srcnum=0;
	for (Op_explicit_sources srcl(cur_op) ; srcl != 0 ; srcl++)
	{
	    srcnum ++;
	    El_ref cur_src = srcl.get_ref();
	    Operand cur_opr = cur_src.get_operand();
	    // if the source is uniteresting just copy it over from current constraint 
	    if( cur_opr.is_undefined() || cur_opr.is_lit() || cur_opr.is_vr_name() )
	    {
	    	assert( current_constraint.is_bound(cur_src) );
	    	// add the current constraint to the end of the list 
	    	srcconstraints[num_srcconstraints] = ( *current_constraint.value(cur_src) );
	    } else {
	    	assert( working_constraint.is_bound(cur_src) );
	    	// add the working constraint to the end of the list 
	    	srcconstraints[num_srcconstraints] = ( *working_constraint.value(cur_src) );
	    }
	    num_srcconstraints++;
	}

	//for each source do the analysis
	srcnum=0;
	for (Op_explicit_sources srci(cur_op) ; srci != 0 ; srci++)
	{
	    srcnum ++;
	    El_ref cur_src = srci.get_ref();
	    Operand cur_opr = cur_src.get_operand();
	    if ( ! ( cur_opr.is_undefined() || cur_opr.is_lit() || cur_opr.is_vr_name() ) ) {
	      assert ( working_constraint.is_bound(cur_src) );
	      WConstraint* orig_constraint= working_constraint.value(cur_src);
	      assert ( current_constraint.is_bound(cur_src) );
	      WConstraint* maxout_constraint= current_constraint.value(cur_src);

	      //////////////////////////////////////////////////////////////////////////////
	      // This is the heart of the analysis, it computes the new constraint from	  //
	      // the old constraint and updates cur_constraint.  			  //
	      //////////////////////////////////////////////////////////////////////////////
	      compute_width_propagation_up( 
		orig_constraint, 
		maxout_constraint, 
		srcnum, 
		cur_op->opcode(), 
		destconstraints,
		num_destconstraints,
		srcconstraints,
		num_srcconstraints,
		debug_flag, 
		op_differ,
		cur_op->id()
               );
	    }

	}
	// Deallocation causes a seg fault... i don't have
	// time to trace this down right now.  but a "Bad free" seems
	// to occur in the DType destructor

	//	delete destconstraints;
	//fprintf(stderr, "BBBBB deallocated\n"); 
	//	delete srcconstraints;
	//fprintf(stderr, "CCCCC deallocated\n"); 
    }//end for region

}

//---------------------------------------------------------------------------//
//    Initialize constraints from user
//---------------------------------------------------------------------------//

// Put proper dtype and literal values into the constraints class
void WidthAnalysis::init_current_constraint(Region * r)
{
    WConstraint * wctemp;

    //over all ops
    for (Region_all_ops opi(r); opi!=0; opi++) 
    {
    	Op *cur_op = *opi;

	//over all sources (even implicit and predicate -sherwood 8/99)
	for (Op_all_inputs srci(cur_op) ; srci != 0 ; srci++)
        {
	    wctemp = new WConstraint;
            El_ref cur_src = srci.get_ref();
	    Operand cur_oper = cur_src.get_operand();
	    wctemp->dt =  cur_oper.dtype();
	    if ( cur_oper.is_int() ) { //int lit
		wctemp->is_lit = true;
		wctemp->val = cur_oper.int_value();
	    } else {
		 wctemp->is_lit = false;
		 wctemp->val = 0;
	    }
	    if ( current_constraint.is_bound(cur_src) ) 
		delete current_constraint.value(cur_src);
	    current_constraint.bind( cur_src, wctemp );
	}

	//over all defs ( even implicit  -sherwood 8/99)
	for (Op_all_dests dsti(cur_op) ; dsti != 0 ; dsti++)
        {
	    wctemp = new WConstraint;
            El_ref cur_dst = dsti.get_ref();
	    Operand cur_oper = cur_dst.get_operand();
	    wctemp->dt =  cur_oper.dtype();
	    if ( cur_oper.is_int() ) {
		wctemp->is_lit = true;
		wctemp->val = cur_oper.int_value();
	    } else {
		 wctemp->is_lit = false;
		 wctemp->val = 0;
	    }
	    if ( current_constraint.is_bound(cur_dst) ) 
		delete current_constraint.value(cur_dst);
	    current_constraint.bind( cur_dst, wctemp );
	}
    }
}

//---------------------------------------------------------------------------//
//    Initialize working constraints
//---------------------------------------------------------------------------//
// Put proper dtype and literal values into the constraints class
void WidthAnalysis::init_working_constraint(Region * r, Direction d)
{
    WConstraint * wctemp;

    //over all ops
    for (Region_all_ops opi(r); opi!=0; opi++) 
    {
    	Op *cur_op = *opi;

	//over all sources (even implicit and predicate -sherwood 8/99)
	for (Op_all_inputs srci(cur_op) ; srci != 0 ; srci++)
        {
	    wctemp = new WConstraint;
            El_ref cur_src = srci.get_ref();
	    Operand cur_oper = cur_src.get_operand();
	    wctemp->dt =  cur_oper.dtype();
	    if ( cur_oper.is_int() ) {
		wctemp->is_lit = true;
		wctemp->val = cur_oper.int_value();
	    } else {
		wctemp->is_lit = false;
		wctemp->val = 0;
	    }
	    //only zero out ints and pointers
	    if ( cur_oper.dtype().info() == EL_DT_INFO_INT 
		|| cur_oper.dtype().info() ==EL_DT_INFO_POINTER )
	    {
// FIXME is_macro_reg check is temp hack, SAM 9-03
	        //but not literal widths
	        if ( ! (cur_oper.is_lit() || cur_oper.is_macro_reg()) ) 
		{
		   wctemp->dt.set_width(0);
		}
	    }

	    //do the binding	
	    if ( working_constraint.is_bound(cur_src) )
	    { 
		delete working_constraint.value(cur_src);
	    }
	    working_constraint.bind( cur_src, wctemp );
	}

	//over all defs ( even implicit  -sherwood 8/99)
	for (Op_all_dests dsti(cur_op) ; dsti != 0 ; dsti++)
        {
	    wctemp = new WConstraint;
            El_ref cur_dst = dsti.get_ref();
	    Operand cur_oper = cur_dst.get_operand();
	    wctemp->dt =  cur_oper.dtype();
	    if ( cur_oper.is_lit() && cur_oper.is_int() ) {
		wctemp->is_lit = true;
		wctemp->val = cur_oper.int_value();
	    } else {
		wctemp->is_lit = false;
		wctemp->val = 0;
	    }
	    //only zero out ints and pointers
	    if (  cur_oper.dtype().info() == EL_DT_INFO_INT
		|| cur_oper.dtype().info() == EL_DT_INFO_POINTER )
	    {
	        //but not literal widths on down pass
// FIXME is_macro_reg check is temp hack, SAM 9-03
	        if ( ! ((d==PROCESS_FORWARD && cur_oper.is_lit() ) ||
			cur_oper.is_macro_reg() ))
		{ 
		   wctemp->dt.set_width(0);
		}
	    }

	    //do the binding	
	    if ( working_constraint.is_bound(cur_dst) )
	    { 
		delete working_constraint.value(cur_dst);
	    }
	    working_constraint.bind( cur_dst, wctemp );
	}
    }
}

//---------------------------------------------------------------------------//
//    Copy Working constraints onto the Current Constriants
//---------------------------------------------------------------------------//
void WidthAnalysis::update_current_constraint(Region *r, WidthAnalysis::Direction d )
{
    El_ref * refptr;
    WConstraint * working_wcptr;
    WConstraint * wcptr;

    for (Hash_map_iterator<El_ref,WConstraint*> mi(current_constraint); mi!=0; mi++) 
    {
	refptr = &((*mi).first);
	wcptr  = (*mi).second;
	assert ( working_constraint.is_bound( *refptr ) );
	working_wcptr = working_constraint.value( *refptr );

	int working_width = working_wcptr->dt.width();
	int current_width = wcptr->dt.width();


	if ( working_width < current_width ) {
	    // SAM 10-2000, added detection of 0 width operands.  Note that
	    // 2 sets of spurious 0 width operands need to be ignored: Undefined
	    // operands 0 and all operands of REMAPs  get set to 0.  Perhaps this
	    // should be fixed someday.. :)
	    if ((working_width == 0) &&
		(! refptr->get_operand().is_undefined()) &&
		(! is_remap(refptr->get_op()))) {
		if (El_punt_on_width_zero) {
		    cerr << "Attempting to set width of operand to 0" << endl;
		    cerr << "\top id: " << refptr->get_op()->id() << endl;
		    cerr << "\toperand: " << refptr->get_operand() << endl;
		    El_punt("WidthAnalysis::update_current_constraint: illegal to set operand width to 0");
	        }
	        else {
		    El_warn("WidthAnalysis::update_current_constraint: setting operand width to 0 for op %d", refptr->get_op()->id());
	        }
	    }
	    //copy over the values if they are different
	    *wcptr = *working_wcptr;

	    //if the values are different then set the corresponding change bit
	    if ( d == PROCESS_FORWARD ) forwardchange = true;
	    else backwardchange = true;
	} else if (  working_width > current_width ) {
	    //assert that is is monotonic decreasing
	    cerr << "working: " << *working_wcptr << endl; 
	    cerr << "current: " << *wcptr << endl;
	    El_punt( "WidthAnalysis::update_current_constraint: working_width > current_width");
	}
    }
}


//---------------------------------------------------------------------------//
//    Decorate Ops
//---------------------------------------------------------------------------//

void WidthAnalysis::decorate_ops(Region *r)
{
    if (debug_flag) cerr << "Starting decorate_ops\n";

    //over all ops
    for (Region_all_ops opi(r); opi!=0; opi++) 
    {
    	Op *cur_op = *opi;
	Reference_dtype_attr *rdt_attr = new Reference_dtype_attr(cur_op);

	//over all sources
	for (Op_explicit_sources srci(cur_op) ; srci != 0 ; srci++)
        {
            El_ref cur_src = srci.get_ref();
            Port_num pn = srci.get_port_num();
	    rdt_attr->set_src(pn, current_constraint.value(cur_src)->dt);
	}

	//over all defs
	for (Op_explicit_dests dsti(cur_op) ; dsti != 0 ; dsti++)
        {
            El_ref cur_dst = dsti.get_ref();
            Port_num pn = dsti.get_port_num();
	    rdt_attr->set_dest(pn, current_constraint.value(cur_dst)->dt);
	}

	//hang the rdt_attr on the op
	set_reference_dtype_attr(cur_op,rdt_attr);
    }
    if (debug_flag) cerr << "Finished decorate_ops\n";
}

//---------------------------------------------------------------------------//
//    Reduce Region
//---------------------------------------------------------------------------//
unsigned local_hash_oper_ptr(Base_operand*& v)
{
  unsigned int key = (unsigned) v;
  key += ~(key << 17);
  key ^=  (key >> 10);
  key +=  (key << 3); 
  key ^=  (key >> 6); 
  key += ~(key << 8); 
  key ^=  (key >> 16);
  return key;
}

/*
 *	SAM 2-2-2000, all elements of the same EVR, ie r1[1], r1[2], must
 *	be the same width.  Changed to make this so.
 *	SAM 8-30-2000, changed 2nd loop to go over ops rather than the map,
 *	new_width, so that remap operands would also be modified.
 */
void WidthAnalysis::reduce_region(Region *r)
{
    El_ref * refptr;
    WConstraint * wcptr;
    Operand * opptr;
    Base_operand * baseptr;
    Hash_map<Base_operand*,int> new_width(local_hash_oper_ptr,CONSTRAINT_HASH_SIZE); 
    Hash_map<int,int> new_evr_width(hash_int, CONSTRAINT_HASH_SIZE);
    int w, w2, old_width, reg_num;

    // El_warn("ADD ME: relax the int lits");

    for (Hash_map_iterator<El_ref,WConstraint*> mi(current_constraint); mi!=0; mi++) 
    {
	refptr = &((*mi).first);
	wcptr  = (*mi).second;
	opptr = &(refptr->get_operand());
	baseptr = opptr->get_ptr();
	old_width = wcptr->dt.width();

	if ( new_width.is_bound(baseptr) ) {
		w = new_width.value(baseptr);
		w = WIDTH_MAX( w, old_width );
		new_width.bind(baseptr, w );	
	} else {
		new_width.bind(baseptr, old_width );
	}

	if (opptr->is_reg()) {
	    	reg_num = opptr->vr_num();
		if ( new_evr_width.is_bound(reg_num)) {
			w = new_evr_width.value(reg_num);
			w = WIDTH_MAX(w, old_width);
			new_evr_width.bind(reg_num, w);
		}
		else {
			new_evr_width.bind(reg_num, old_width);
		}
	}
    }

    for (Region_all_ops opi(r); opi!=0; opi++) {
        Op *op = *opi;
	for (Op_explicit_sources srci(op); srci!=0; srci++) {
	    Operand src = *srci;
	    if (src.is_undefined())
		continue;
	    Base_operand *srcptr = src.get_ptr();
	    if (new_width.is_bound(srcptr)) {
		w = new_width.value(srcptr);
		if (src.is_reg() || src.is_vr_name())
		    w2 = new_evr_width.value(src.vr_num());
		else
		    w2 = 0;
		w = WIDTH_MAX(w, w2);
		DType dt = srcptr->dtype();
		if (dt.width() > (unsigned) w) {
		    dt.set_width(w);
		    srcptr->set_dtype(dt);
		}
	    }
	}
	for (Op_explicit_dests desti(op); desti!=0; desti++) {
	    Operand dest = *desti;
	    if (dest.is_undefined())
		continue;
	    Base_operand *destptr = dest.get_ptr();
	    if (new_width.is_bound(destptr)) {
		w = new_width.value(destptr);
		if (dest.is_reg() || dest.is_vr_name())
		    w2 = new_evr_width.value(dest.vr_num());
		else
		    w2 = 0;
		w = WIDTH_MAX(w, w2);
		DType dt = destptr->dtype();
		if (dt.width() > (unsigned) w) {
		    dt.set_width(w);
		    destptr->set_dtype(dt);
		}
	    }
	}
    }
}

//---------------------------------------------------------------------------//
//    Estimate the Gate Count
//---------------------------------------------------------------------------//
void WidthAnalysis::estimate_width_savings( Region *r, char *fname )
{
    //print out the sum of all the references

    El_ref * refptr;
    WConstraint * wcptr;
    Operand * opptr;
    Hash_map<Base_operand*,int> op_first_touch(local_hash_oper_ptr,CONSTRAINT_HASH_SIZE); 

    int ref_width; 
    int ref_total_sum=0; //all refs
    int ref_int_sum=0; //all ints
    int ref_int_num=0; //all ints
    int ref_ptr_sum=0; //all ptr
    int ref_ptr_num=0; //all ptr

    int op_width; 
    int op_total_sum=0; //all ops
    int op_int_sum=0; //all ints
    int op_int_num=0; //all ints
    int op_ptr_sum=0; //all ptr
    int op_ptr_num=0; //all ptr
 
    for (Hash_map_iterator<El_ref,WConstraint*> mi(current_constraint); mi!=0; mi++) 
    {
	refptr = &((*mi).first);
	opptr = &(refptr->get_operand());
	// if it is a register in a non psuedo op
	if ( (!is_pseudo(refptr->get_op())) && opptr->is_reg() ) {

	    //for the reference 

	    wcptr  = (*mi).second;
	    ref_width = wcptr->dt.width();
	    ref_total_sum += ref_width;
	    if ( opptr->dtype().info() == EL_DT_INFO_INT ) {
		ref_int_sum+= ref_width;
		ref_int_num++;
	    } else if ( opptr->dtype().info() == EL_DT_INFO_POINTER ) {
		ref_ptr_sum+= ref_width;
		ref_ptr_num++;
	    }

	    // for the op

	    //if this is the first time we see opptr
	    if ( ! op_first_touch.is_bound(opptr->get_ptr()) )
	    {
		op_width = opptr->dtype().width();
	    	op_total_sum += op_width;
	    	if ( opptr->dtype().info() == EL_DT_INFO_INT ) {
		    op_int_sum+= op_width;
		    op_int_num++;
		} else if ( opptr->dtype().info() == EL_DT_INFO_POINTER ) {
		    op_ptr_sum+= op_width;
		    op_ptr_num++;
		}

		op_first_touch.bind(opptr->get_ptr(),1);
	    }
	}
    }

    FILE *f;
    f = fopen( fname, "w" );
    assert(f);
    fprintf(f,"\n");
    fprintf(f,"*** %s\n",fname);
    fprintf(f,"Total reg ref sum: %d\n", ref_total_sum);
    fprintf(f,"Int reg ref sum: %d\n", ref_int_sum);
    fprintf(f,"Average int reg ref width: %f\n", ref_int_sum/(float)ref_int_num );
    fprintf(f,"Ptr reg ref sum: %d\n", ref_ptr_sum);
    fprintf(f,"Average ptr reg ref width: %f\n", ref_ptr_sum/(float)ref_ptr_num );
    fprintf(f,"\n");
    fprintf(f,"Total reg op sum: %d\n", op_total_sum);
    fprintf(f,"Int reg op sum: %d\n", op_int_sum);
    fprintf(f,"Average int reg op width: %f\n", op_int_sum/(float)op_int_num );
    fprintf(f,"Ptr reg op sum: %d\n", op_ptr_sum);
    fprintf(f,"Average ptr reg op width: %f\n", op_ptr_sum/(float)op_ptr_num );
    fclose(f);
}

int El_estimate_fu_cost(Op *op)
{
    Reference_dtype_attr *rdt_attr;
    DType dest1_dt, src1_dt, src2_dt;
    double w, dw, aw, cost = 0.0;

    rdt_attr = get_reference_dtype_attr(op);

    if (op->num_dests() > 0)
        dest1_dt = El_get_dtype(op, DEST, DEST1);
    if (op->num_srcs() > 0)
        src1_dt = El_get_dtype(op, SRC, SRC1);
    if (op->num_srcs() > 1)
        src2_dt = El_get_dtype(op, SRC, SRC2);

    if (is_int_add(op) || is_int_sub(op)) {
	w = (double) dest1_dt.width();
	cost = 9.5 * w;
    }
    else if (is_int_mul(op)) {
	w = (double) dest1_dt.width();
	cost = (10.5*w*w) + (10.9*w) + 58.8;
    }
    else if (is_int_div(op) || is_int_rem(op)) {
	w = (double) dest1_dt.width();
	cost = pow(10.0, (0.6 * log(w)/log(2.0) + 0.98));
    }
    else if (is_int_lshift(op) || is_int_rshift(op)) {
	w = (double) dest1_dt.width();
	cost = 7.9 * w;
    }
    else if (is_cmpr(op) || is_cmpp(op)) {
	w = (double) WIDTH_MAX(src1_dt.width(), src2_dt.width());
	cost = 6.8 * w;
    }
    else if (is_load(op)) {
	aw = (double) src1_dt.width();
	dw = (double) dest1_dt.width();
	cost = (aw+dw)*10.0 + aw*9.5;
    }
    else if (is_store(op)) {
	aw = (double) src1_dt.width();
	dw = (double) src2_dt.width();
	cost = (aw+dw)*10.0 + aw*9.5;
    }
    else if (is_extract(op)) {
	w = dest1_dt.width();
	cost = 1.75 * w;
    }
    else if (is_move(op)) {
	w = dest1_dt.width();
	cost = 1.75 * w;
    }
    else if (is_and(op) || is_or(op) || is_xor(op)) {
	w = dest1_dt.width();
	cost = 1.0 * w;
    }
    else if (is_branch(op)) {
	cost = 32.0*6.8 + 32.0*9.5 + 32.0*9.5;
    }
    else {
	El_punt("El_fu_cost: unknown opcode");
    }

    return ((int) cost);
}

int El_estimate_reg_cost(Base_operand *o)
{
    double w, cost;

    if (! (o->is_reg() || o->is_macro_reg()))
	El_punt("El_estimate_reg_cost: unknown operand");

    w = (double) o->dtype().width();
    cost = 10.0 * w;

    return ((int) cost);
}

int El_estimate_regsel_cost(Reg *o, int num_reads, int num_writes)
{
    double w, cost, rcost, wcost;

    w = (double) o->dtype().width();


    if (num_reads > 1)
        rcost = w*0.5*(double)num_reads + 0.5*(double)num_reads;
    else
	rcost = 0.0;

    if (num_writes > 1)
        wcost = w*0.5*(double)num_writes + 0.5*(double)num_writes;
    else
	wcost = 0.0;

    cost = rcost + wcost;

    return ((int) cost);
}

void El_estimate_gate_count_ii1(Region *r)
{
    Op *op;
    Operand src, dest;
    Base_operand *bo;
    Hash_set<Base_operand*> op_first_touch(hash_base_operand_ptr, 1024);
    Hash_map<int, int> max_omega(hash_int, 1024);
    Hash_map<int, int> min_omega(hash_int, 1024);
    Hash_map<Base_operand*, int> num_reads(hash_base_operand_ptr, 1024);
    Hash_map<Base_operand*, int> num_writes(hash_base_operand_ptr, 1024);
    Hash_set<int> visited(hash_int, 1024);
    int tot_gate_count = 0;
    int fu_gate_count = 0;
    int reg_gate_count = 0;
    int regsel_gate_count = 0;
    int t, multiplier, regno, omega, old_max, old_min;

    for (Region_all_ops opi(r); opi!=0; opi++) {
	op = *opi;
	if (is_pseudo(op))
	    continue;
	if (is_pbr(op) || is_rts(op))
	    continue;	// F these ops!

	t = El_estimate_fu_cost(op);
	fu_gate_count += t;

	for (Op_explicit_dests desti(op); desti!=0; desti++) {
	    dest = *desti;
	    if (! (dest.is_reg() || dest.is_macro_reg()))
		continue;
	    if (! op_first_touch.is_member(dest.get_ptr())) {
		op_first_touch += dest.get_ptr();
		num_writes.bind(dest.get_ptr(), 1);
		num_reads.bind(dest.get_ptr(), 0);
	    }
	    else {
	        num_writes.bind(dest.get_ptr(), (num_writes.value(dest.get_ptr())+1));
	    }

	    if (dest.is_reg()) {
		regno = dest.vr_num();
		omega = dest.omega();
		if (! max_omega.is_bound(regno)) {
		     max_omega.bind(regno, omega);
		     min_omega.bind(regno, WIDTH_MIN(0,omega));
		}
		else {
		    old_max = max_omega.value(regno);
		    if (omega > old_max)
			max_omega.bind(regno, omega);
		    old_min = min_omega.value(regno);
		    if (omega < old_min)
			min_omega.bind(regno, omega);
		}
	    }
	}
	for (Op_explicit_sources srci(op); srci!=0; srci++) {
	    src = *srci;
	    if (! (src.is_reg() || src.is_macro_reg()))
		continue;

	    if (! op_first_touch.is_member(src.get_ptr())) {
		op_first_touch += src.get_ptr();
		num_writes.bind(src.get_ptr(), 0);
		num_reads.bind(src.get_ptr(), 1);
	    }
	    else {
	        num_reads.bind(src.get_ptr(), (num_reads.value(src.get_ptr())+1));
	    }

	    if (src.is_reg()) {
		regno = src.vr_num();
		omega = src.omega();
		if (! max_omega.is_bound(regno)) {
		     max_omega.bind(regno, omega);
		     min_omega.bind(regno, WIDTH_MIN(0,omega));
		}
		else {
		    old_max = max_omega.value(regno);
		    if (omega > old_max)
			max_omega.bind(regno, omega);
		    old_min = min_omega.value(regno);
		    if (omega < old_min)
			min_omega.bind(regno, omega);
		}
	    }
	}
    }
    for (Hash_set_iterator<Base_operand*> hi(op_first_touch); hi!=0; hi++) {
	bo = *hi;
	t = El_estimate_reg_cost(bo);
	regsel_gate_count += El_estimate_regsel_cost((Reg*)bo,
						num_reads.value(bo),
						num_writes.value(bo));
	if (bo->is_reg()) {
	    regno = ((Reg *)bo)->vr_num();
	    if (visited.is_member(regno))
		continue;
	    visited += regno;
	    multiplier = max_omega.value(regno) - min_omega.value(regno) + 1;
	}
	else {
	    multiplier = 1;
	}
	reg_gate_count += (t*multiplier);
    }

    tot_gate_count = fu_gate_count + reg_gate_count + regsel_gate_count;


    FILE *F = fopen("BLINK", "a");
    if (F == NULL)
	El_punt("El_estimate_gate_count_ii1: cannot open output file");
    fprintf(F, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fprintf(F, "Estimated gate count\n");
    fprintf(F, "\t Total: %d\n", tot_gate_count);
    fprintf(F, "\t\t FU: %d\n", fu_gate_count);
    fprintf(F, "\t\t Reg: %d\n", reg_gate_count);
    fprintf(F, "\t\t Regsel: %d\n", regsel_gate_count);
    fprintf(F, "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
    fclose(F);
}

//---------------------------------------------------------------------------//
//    Constructor and Destructor
//---------------------------------------------------------------------------//

WidthAnalysis::WidthAnalysis()
  : working_constraint(hash_El_ref,CONSTRAINT_HASH_SIZE),
    current_constraint(hash_El_ref,CONSTRAINT_HASH_SIZE)
{
    debug_flag = false; 
    converge = false;
    local_converge = false;
    forwardchange = false; 
    backwardchange = false;
    filter_flag = ANALYZE_ALLREG;
    rd_info = NULL;
}

WidthAnalysis::~WidthAnalysis()
{
    WConstraint * wctemp;
    for (Hash_map_iterator<El_ref,WConstraint*> mi(current_constraint); mi != 0; mi++) 
    {
	wctemp = (*mi).second;
	delete wctemp;
    }
    for (Hash_map_iterator<El_ref,WConstraint*> m2(working_constraint); m2 != 0; m2++) 
    {
	wctemp = (*m2).second;
	delete wctemp;
    }
}
