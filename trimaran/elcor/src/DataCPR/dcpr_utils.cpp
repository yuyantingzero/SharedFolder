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
//      File:           dcpr_utils.cpp
//      Authors:        Suren Talla
//      Created:        August 1997
//      Description:    utility functions for data cpr modules
//
//      Copyright (C) 1997 Hewlett-Packard Company 
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "region.h"
#include "dcpr_utils.h"
#include "pred_analysis.h"
#include "flow_analysis_solver.h"
#include "edge_drawing.h"
#include "res_len.h"
#include "dep_len.h"
#include "el_bb_tools.h"
#include "iterators.h"
#include "el_critical_region.h"
#include "dbg.h"
#include "el_dcpr_reassoc.h"
#include "el_data_cpr_init.h"

using namespace std;


int get_vrnames_from_anti_edges(List<Edge*>& anti_edges, List<int>& vrnames)
{
    int num=0;
    for(List_iterator<Edge*> li(anti_edges); li!=0; li++){
	Edge* e = *li;
	Operand& o1 = e->src()->src(e->src_port());
	Operand& o2 = e->dest()->dest(e->dest_port());
	if (o1==o2){
	    if (!vrnames.is_member(o1.vr_num())){
		vrnames.add_tail(o1.vr_num());
		num++;
	    }
	}
    }
    return num;
}
//----------------------------------------------------------------------;
// test the Critical_region_info class interface;
//----------------------------------------------------------------------;
void test_critical_region_class(Procedure* f)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region* rgn;
    char proc_name[30];
    for(int k=0; k<30; k++) proc_name[k] = 0;
    sprintf(proc_name, "%s", (char*)(f->get_name()));

    cdbg << "testing critical region <function = " << proc_name	 << ">\n"  << flush;
    
    create_local_analysis_info_for_all_hbs_bbs((Compound_region*)f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);

    rstack.push(f);
    while (!rstack.is_empty()) {
	rgn = rstack.pop();
	push_flag = true;
	if (rgn->is_hb() || rgn->is_bb()) {
	    insert_region_scalar_edges((Compound_region *)rgn);
	    Critical_region_info critical_region;
	    critical_region.compute_critical_region_info((Compound_region *)rgn);
	    //cdbg << critical_region;
	    critical_region.display_critical_region(rgn, DISPLAY_ALL_CR_EDGES, proc_name, true, 0);
	    critical_region.display_critical_region(rgn, DISPLAY_ALL_EDGES, proc_name, false, 1);
	    critical_region.delete_critical_region_info((Compound_region *)rgn);
	    delete_region_edges((Compound_region*)rgn);
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

}

//----------------------------------------------------------------------
int has_scope_for_datacpr(double prev_dep_len, double prev_res_len, 
			  double dep_len, double res_len, 
			  double hb_weight)
{
    if (dbg(dcpr,1)){
	cdbg << "<prev_dep, prev_res,deplen,reslen,hb_weght> = <" 
	     << prev_dep_len << ", " << prev_res_len << ", " 
	     << dep_len << ", " << res_len 
	     << ", " << hb_weight << ">\n";
    }

    if (prev_dep_len>=dep_len || dep_len<=res_len || hb_weight<=1) 
	return false;
    
    double d = (dep_len - res_len)/dep_len;

    if (d < .1) return false;
    return true;

}
//----------------------------------------------------------------------;
// test the reslen and deplen classes;
//----------------------------------------------------------------------;
void test_res_len_dep_len(Procedure* f)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region* rgn;
    double dep_len1, dep_len2, res_len1, res_len2, res_len3,
           res_len4, res_len5, res_len6;

    printf("function : %s\n", (char*)f->get_name());

    create_local_analysis_info_for_all_hbs_bbs((Compound_region*)f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);

    rstack.push(f);
    while (! rstack.is_empty()) {
	rgn = rstack.pop();
	push_flag = true;
	if (rgn->is_hb() || rgn->is_bb()) {
	    insert_region_scalar_edges((Compound_region *)rgn);
	    
	    //------------------------------------;
	    //testing dep len;
	    //------------------------------------;
	    El_deplen_estimate_type = EDGE_LATENCY_DEP_LEN;
	    El_deplen_estimate_method = MAX_DEP;
	    dep_len1   = DepLen::dep_len_estimate((Compound_region *)rgn); 

	    El_deplen_estimate_type = EDGE_LATENCY_DEP_LEN;
	    El_deplen_estimate_method = WEIGHTED_DEP;
	    dep_len2   = DepLen::dep_len_estimate((Compound_region *)rgn); 

	    //------------------------------------;
	    //testing res len;
	    //------------------------------------;
	    El_reslen_estimate_type = RMSL_RES_LEN;
	    El_reslen_estimate_ops = C0_OPS_RES_LEN;
	    El_reslen_estimate_method = MAX_RES;
	    res_len1   = ResLen::res_len_estimate((Compound_region *)rgn);

	    El_reslen_estimate_type = RMSL_RES_LEN;
	    El_reslen_estimate_ops = C0_OPS_RES_LEN;
	    El_reslen_estimate_method = WEIGHTED_RES;
	    res_len2   = ResLen::res_len_estimate((Compound_region *)rgn);

	    El_reslen_estimate_type = IFMB_RES_LEN;
	    El_reslen_estimate_ops = C0_OPS_RES_LEN;
	    El_reslen_estimate_method = MAX_RES;
	    res_len3   = ResLen::res_len_estimate((Compound_region *)rgn);

	    El_reslen_estimate_type = IFMB_RES_LEN;
	    El_reslen_estimate_ops = C0_OPS_RES_LEN;
	    El_reslen_estimate_method = WEIGHTED_RES;
	    res_len4   = ResLen::res_len_estimate((Compound_region *)rgn);

	    El_reslen_estimate_type = IFMB_RES_LEN;
	    El_reslen_estimate_ops = ANSCESTOR_OPS_RES_LEN;
	    El_reslen_estimate_method = MAX_RES;
	    res_len5   = ResLen::res_len_estimate((Compound_region *)rgn);
	    
	    El_reslen_estimate_type = IFMB_RES_LEN;
	    El_reslen_estimate_ops = ANSCESTOR_OPS_RES_LEN;
	    El_reslen_estimate_method = WEIGHTED_RES;
	    res_len6   = ResLen::res_len_estimate((Compound_region *)rgn);

	    printf("%4d   %6g   %2.2g   %2.2g   %2.2g   %2.2g   %2.2g   %2.2g   %2.2g   %2.2g\n",
		   rgn->id(), ((Compound_region *)rgn)->weight, 
		   dep_len1, dep_len2, res_len1, res_len2, 
		   res_len3, res_len4, res_len5, res_len6);

	    delete_region_edges((Compound_region*)rgn);
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

}

//###############################################################################

void flowedge_src_to_elref(Edge* e, El_ref& el)
{
    assert(e->is_reg_flow());
    Operand& oper = e->src()->dest(e->src_port());
    El_ref tref(e->src(), &oper, UNKNOWN_REF_TYPE /* how to get reftype?*/);
    el = tref;
}

void flowedge_dest_to_elref(Edge* e, El_ref& el)
{
    assert(e->is_reg_flow());
    Operand& oper = e->dest()->src(e->dest_port());
    El_ref tref(e->dest(), &oper, UNKNOWN_REF_TYPE /* how to get reftype?*/);
    el = tref;
}


//----------------------------------------------------------------------;
// this has been hardcoded for the execute function of grep just for ;
// testing purposes (in fact the lines pertaining to grep can be deleted;
// and it will work fine for other benchmarks;
//----------------------------------------------------------------------;
void test_flow_chains(Procedure* f)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region* rgn;
    char proc_name[30];
    for(int k=0; k<30; k++) proc_name[k] = 0;
    sprintf(proc_name, "%s", (char*)(f->get_name()));
    
    //if (strcmp(proc_name, "_execute")!=0) return;

    cdbg << "testing flow chains for = " << proc_name  << ">\n"  << flush;
    
    create_local_analysis_info_for_all_hbs_bbs((Compound_region*)f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);

    List<Edge*> flow_edges;
    Hash_map<Op*, DepLen_OpInfo*>   deplen_info(hash_op_ptr, INIT_HASH_MAP_SIZE); 

    rstack.push(f);
    while (!rstack.is_empty()) {
	rgn = rstack.pop();
	push_flag = true;
	if ((rgn->is_hb() || rgn->is_bb()) ) {
	    cdbg << " --- rgn <" << rgn->id() << ">\n";
	    insert_region_scalar_edges((Compound_region *)rgn);

	    //partition flow edges into chains;
	    Critical_region_info critical_region;
	    critical_region.compute_critical_region_info((Compound_region *)rgn);
	    critical_region.get_all_flow_critical_edges(flow_edges);
	    
	    El_do_reaching_defs ((Region*)rgn);
	    Reaching_defs_info* rdefs_info = get_reaching_defs_info ((Region*)rgn);
	    
	    Reassoc_flow_chains rfc;
	    rfc.compute_flow_chains(rgn);
	    rfc.display_flow_chains(cdbg);
	    DepLen::get_estart((Compound_region*)rgn, deplen_info);
	    rfc.gen_code(rgn, rdefs_info, deplen_info);

	    remove_reaching_defs_info ((Region*)rgn);
	    delete_region_edges((Compound_region*)rgn);

	    rfc.delete_dead_flow_chains();
	    rfc.delete_flow_chains();

	    critical_region.delete_critical_region_info((Compound_region *)rgn);

	    flow_edges.clear();
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

}


