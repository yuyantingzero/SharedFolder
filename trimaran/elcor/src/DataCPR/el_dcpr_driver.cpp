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
//
//      File:           el_dcpr_driver.cpp
//      Authors:        Suren Talla
//      Created:        July 1997
//      Description:    Data CPR optimization main driver
//
///////////////////////////////////////////////////////////////////////////

#include "el_bb_tools.h"
#include "el_data_cpr_init.h"
#include "el_dcpr_driver.h"

#include "pred_analysis.h"
#include "flow_analysis_solver.h"
#include "edge_drawing.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "dbg.h"
#include "res_len.h"
#include "dep_len.h"
#include "el_dot_tools.h"
#include "el_dcpr_renaming.h"
#include "el_critical_region.h"
#include "dcpr_utils.h"


int select_transform_to_apply(int opti_type_prev,Critical_region_info crgn);



//###########################################################################
//  Main driver routine for the Data CPR transforms
//###########################################################################

int El_dcpr_driver(Procedure* f)
{
    int total_renamed =0;
    int total_applications = 0;
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rgn;
    int recompute_annotations = false;
    
    if (dbg(status, 1))
	cdbg << "applying data-cpr on = " << f->get_name() << endl;
    
    
    create_local_analysis_info_for_all_hbs_bbs((Compound_region*)f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);
    
    
    //-----------------------------------------------------------;
    //apply the always profitable transforms - ;
    //   1. disjoint live range renaming (rename without copy);
    //   2. any other transform??;
    //-----------------------------------------------------------;
    if (El_do_always_profitable_datacpr){
	if ((El_rename_method==DO_PROCEDURE_RENAMING) 
	    || (El_rename_method==DO_HB_RENAMING)){
	    total_renamed = El_rename_region((Compound_region*)f);
	} else if (El_rename_method==DO_BLIND_RENAMING) {
	    //NOTE: This is NOT an always profitable transform;
	    //put here for ttesting purposes only - this transform;
            //is no really beneficial so use with caution;
	    total_renamed = El_rename_blindly_region(f);
	}
	cdbg << "no. renamed (always profitable section)= " << total_renamed << endl;
	if (total_renamed>0){
	    el_flow_delete_liveness(f);
	    create_local_analysis_info_for_all_hbs_bbs((Compound_region*)f);
	    el_flow_compute_liveness(f, ANALYZE_ALLREG);
	}
    }


    Critical_region_info critical_region;
    double prev_dep_len, prev_res_len, res_len, dep_len;
    double hb_weight;


    if (!El_do_incremental_datacpr){
	el_flow_delete_liveness(f);
	delete_local_analysis_info_for_all_hbs_bbs(f);
	return 1;
    }


    //-----------------------------------------------------------------;
    //at this point all the HB's/BB's have correct local_analysis      ;
    //and liveness attributes                                          ;
    //main loop for the data-cpr :: for each hyper/basic block         ;
    //apply any/all applicable dcpr transforms until no more profitable;
    //-----------------------------------------------------------------;

    rstack.push(f);
    while (! rstack.is_empty()) {
	rgn = rstack.pop();
	push_flag = true;
	if (rgn->is_hb() || rgn->is_bb()) {
	    
	    //do edge drawing and compute critical regions;
	    insert_region_scalar_edges((Compound_region *)rgn);
	    critical_region.compute_critical_region_info((Compound_region *)rgn);

	    //compute the profitability estimators;
	    dep_len   = DepLen::dep_len_estimate((Compound_region *)rgn); 
	    res_len   = ResLen::res_len_estimate((Compound_region *)rgn);
	    hb_weight = ((Compound_region *)rgn)->weight;

	    int hb_id = rgn->id();
	    int opti_type = DO_HB_RENAMING_WITH_COPY;
	    int num_iters = 0;

	    prev_dep_len = prev_res_len = -1.;
	    //------------------------------------------------------------;
	    // apply data cpr transforms if ;
	    //   - there is scope for improvement   ;
	    //   - there was an improvement in the earlier round ;
	    //------------------------------------------------------------;
	    while (has_scope_for_datacpr(prev_dep_len,prev_res_len,dep_len,res_len,hb_weight))
		{
		    //display dfg before each round of transformation;
		    if (El_display_critical_region && hb_weight>0){
			critical_region.display_critical_region(rgn, 
			      DISPLAY_MAX_CR_EDGES,f->get_name(), true,num_iters);
		    }
		
		    recompute_annotations = false;


		    /*which DataCPR transform you would like to apply - in 
		      theory it can be very smart : look at the critical region,
		      identify what kind of edge dependencies are there and
		      which transform would give the most benefit with least 
		      cost and compile time overhead. For this version, it is
		      really dumb - just cycles through the transforms in seq*/
		    opti_type = select_transform_to_apply(opti_type, critical_region);
		      
		    switch(opti_type){
		      case DO_HB_RENAMING_WITH_COPY:
			{
			    // 1.  renaming with copy;
			    List<Edge*> dep_edges;
			    critical_region.get_all_anti_critical_edges(dep_edges);
			    total_renamed = El_rename_with_copy_region((Compound_region *)rgn, &dep_edges);
			    dep_edges.clear();
			    cdbg << "HB = " << hb_id << " no. renamed = " << total_renamed << endl;
			    if (total_renamed>0)
				recompute_annotations = true;
			}
			break;
		      case DO_HB_ARITHMETIC_REASSOCIATION:
			{
			    //2a.  back substitution - reduction to scalar;
			    //2b.  back substitution - reduction to Vector;
			    // 3.  arithmetic expression reassociation;
			}
			break;
		      case DO_HB_PREDICATE_SPECULATION:
			{
			    // 4.  predicate speculation;
			}
			break;
		      default:
			break;
		    }

		    if (recompute_annotations){
			el_flow_delete_liveness((Compound_region*)rgn);
			delete_local_analysis_info_for_all_hbs_bbs((Compound_region*)rgn);
			delete_region_edges((Compound_region*)rgn);
			critical_region.delete_critical_region_info((Compound_region *)rgn);

			create_local_analysis_info_for_all_hbs_bbs((Compound_region*)rgn);
			el_flow_compute_liveness((Compound_region*)rgn, ANALYZE_ALLREG);
			insert_region_scalar_edges((Compound_region *)rgn);
			critical_region.compute_critical_region_info((Compound_region *)rgn);

			num_iters++;
			total_applications++;
		    } 
		    prev_res_len = res_len;
		    prev_dep_len = dep_len;

		    dep_len = DepLen::dep_len_estimate((Compound_region *)rgn);
		    res_len = ResLen::res_len_estimate((Compound_region *)rgn);

		}
	    //display dfg before each round of transformation;
	    if (El_display_critical_region && num_iters>0 && hb_weight>0 ){
		critical_region.display_critical_region(rgn, 
                         DISPLAY_MAX_CR_EDGES, f->get_name(), true, num_iters);
	    }
	    delete_region_edges((Compound_region*)rgn);
	    critical_region.delete_critical_region_info((Compound_region *)rgn);
	    push_flag = false; 
	}
	if (push_flag==true) {
	    for (Region_subregions subri(rgn); subri!=0; subri++) {
		if ((*subri)->is_compound()) {
		    rstack.push(*subri);
		}
	    }
	}
    }
    
    el_flow_delete_liveness(f);
    delete_local_analysis_info_for_all_hbs_bbs(f);

    return total_applications;
    
}

//----------------------------------------------------------------------------
int select_transform_to_apply(int opti_type_prev,Critical_region_info crgn)
{
    //for now we do not use the critical region infomation;
    switch (opti_type_prev){
      case DO_HB_RENAMING_WITH_COPY:
	return DO_HB_ARITHMETIC_REASSOCIATION;
      case DO_HB_ARITHMETIC_REASSOCIATION:
	return DO_HB_PREDICATE_SPECULATION;
      case DO_HB_PREDICATE_SPECULATION:
	return DO_HB_RENAMING_WITH_COPY;
      default:
	return DO_HB_RENAMING_WITH_COPY;
    }

}
//----------------------------------------------------------------------------
Procedure*  junk_renaming(Procedure* f)
{

    int total_renamed =0;

    bool push_flag;
    Dlist<Region*> rstack;
    Region *rgn;

    create_local_analysis_info_for_all_hbs_bbs((Compound_region*)f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);

    //just testing the tiling printout;
    //Alist_region_tiling_graph g(f,GRAPH_HBS_BBS) ;
    //el_dot_display_alist_region_tiling_graph(&g);

    if (dbg(dcpr, 5)){
	total_renamed = El_rename_blindly_region(f);
    } else if (dbg(dcpr, 4)){
	total_renamed = El_rename_with_copy_region((Compound_region*)f);
    } else if (dbg(dcpr, 3)){
	total_renamed = El_rename_region((Compound_region*)f);
    } else if (dbg(dcpr, 2)) {
	rstack.push(f);
	while (! rstack.is_empty()) {
	    rgn = rstack.pop();
	    push_flag = true;
	    if (rgn->is_hb() || rgn->is_bb()) {
		total_renamed += El_rename_with_copy_region((Compound_region *)rgn);
		push_flag = false; 
	    }
	    if (push_flag==true) {
		for (Region_subregions subri(rgn); subri!=0; subri++) {
		    if ((*subri)->is_compound())
			rstack.push(*subri);
		}
	    }
	}
    } else if (dbg(dcpr, 1)) {
	rstack.push(f);
	while (! rstack.is_empty()) {
	    rgn = rstack.pop();
	    push_flag = true;
	    if (rgn->is_hb() || rgn->is_bb()) {
		//total_renamed += El_rename_region((Compound_region *)rgn);
		
		insert_region_scalar_edges((Compound_region *)rgn);

		Critical_region_info critical_region;
		critical_region.compute_critical_region_info((Compound_region *)rgn);

		for(Region_exit_ops eops((Compound_region *)rgn); 
		    eops != 0; eops++){
		    critical_region.display_critical_region(rgn, DISPLAY_MAX_FREQ_CR_EDGES);
		}
		/*
		  insert_region_scalar_edges ((Compound_region *)rgn);
		  el_dot_display_dfg((Compound_region *)rgn);
                */
		push_flag = false; 
	    }
	    if (push_flag==true) {
		for (Region_subregions subri(rgn); subri!=0; subri++) {
		    if ((*subri)->is_compound())
			rstack.push(*subri);
		}
	    }
	}
    }

    el_flow_delete_liveness(f);
    delete_local_analysis_info_for_all_hbs_bbs(f);
    //f = common_process_function(f);

    cout << "Procedure " << f->get_name() << " renamed : " 
	 << total_renamed << endl << flush;;
    
    return(f);

}
