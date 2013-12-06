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
//      File:           el_crit_path.cpp
//      Authors:        Suren Talla
//      Created:        August 1997
//      Description:    Dump  the ELCOR_CRIT_PATH file data (format as
//                      specified in the pico overview document)
//
//      Copyright (C) 1996 Hewlett-Packard Company 
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

#include "region.h"
#include "edge_drawing.h"
#include "iterators.h"
#include "opcode.h"
#include "el_stats_init.h"
#include "el_io.h"
#include "pred_analysis.h"
#include "flow_analysis_solver.h"
#include "dep_len.h"
#include "attributes.h"
#include "el_data_cpr_init.h"
#include "sched_functions.h"
#include "intf.h"


void El_compute_elcor_crit_path(Procedure* f)
{
    Hash_map<Op*, DepLen_OpInfo*>   dep_info(hash_op_ptr, 1028); 

    Region_exit_ops eops;
    Op* exitop;
    int j, cpl, push_flag;
    Dlist<Region*> rstack;
    rstack.push(f);

    create_local_analysis_info_for_all_hbs_bbs((Compound_region*)f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);
    insert_region_scalar_edges((Compound_region *)f);
    

    while (! rstack.is_empty()) {
	Region* rgn = rstack.pop();
	push_flag = true;
	if (rgn->is_hb() || rgn->is_bb()) {
            DepLen::get_estart((Compound_region*)rgn, dep_info);

	    int hb_id = rgn->id();
	    double hb_weight = 0.0; //initialize weighted crit path for block
	    for(eops(rgn); eops!=0; eops++){
		exitop = *eops;
		int exitop_id = exitop->id();

		assert(dep_info.is_bound(exitop));
		//critical path length to this exit op;
		cpl = dep_info.value(exitop)->m_Estart +
		  //MDES_branch_latency(el_opcode_to_string_map.value(exitop->opcode()));
		  //tangw (06-03-02)
		  MDES_branch_latency(get_mdes_opcode_string(exitop));


		//zero latency used for dummy branches

		j = 0;
		for(Op_outedges_rel oe(exitop, CONTROL0_OUTEDGES); oe!=0; oe++){
		    Edge* e = *oe;
		    if (! ((Compound_region *)rgn)->is_exit_edge(e))
			continue;
		    Control_flow_freq *cfreq = get_control_flow_freq(e);
		    j++;

		    if (El_do_crit_path) {
		        (*EL_CRIT_PATH_STREAM) << "exit { ";
		        (*EL_CRIT_PATH_STREAM) << f->get_name();
		        (*EL_CRIT_PATH_STREAM) << " " << hb_id << " " 
			    << exitop_id << " " << j << " freq = " 
			    << cfreq->freq << " cpl = " << cpl << "}\n";
		    }
		    hb_weight=hb_weight + cpl*(cfreq->freq);
		}
	    }
	    if(El_do_wgt_crit_path){
	      double rgn_wgt = ((Compound_region*)rgn)->weight;
	      if(rgn_wgt > 0.0) hb_weight = hb_weight / rgn_wgt;
	      (*EL_STAT_STREAM) << "Weighted_critical_path\t" ;
	      (*EL_STAT_STREAM) << hb_id << " " << hb_weight << "\n";		
	    }

	    DepLen::clear_opinfo_map((Compound_region*)rgn, dep_info);
	    dep_info.clear();
	      
	    push_flag = false; 
	}
	if (push_flag==true) {
	    for (Region_subregions subri(rgn); subri!=0; subri++) {
		if ((*subri)->is_compound())
		    rstack.push(*subri);
	    }
	}
    }
    
    delete_region_edges((Compound_region*)f);
    el_flow_delete_liveness(f);
    delete_local_analysis_info_for_all_hbs_bbs(f);
    
}
