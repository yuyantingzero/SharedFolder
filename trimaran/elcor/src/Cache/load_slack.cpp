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
//      File:           load_slack.cpp
//      Authors:        Santosh G. Abraham, Teresa Johnson
//      Created:        July 1997
//      Description:    Load slack adjustment during scheduling
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "el_error.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "sched_functions.h"
#include "ls_get_profile.h"
#include "ls_prefetch.h"
#include "op_info.h"
#include "load_info.h"
#include "core_edge_drawing.h"

int recompute_earliest_times(Compound_region* region, 
			Hash_map<Op*,int > *etime_map) {
 Op *current_op, *src_op;
 int new_early_time, max_early_time = -1;

 for (Region_ops_C0_order iter(region); iter != 0; ++iter)
 {
   current_op = *iter;
   max_early_time = 0;

   assert (opInfoMap->is_bound(current_op));
   OpInfo *info = opInfoMap->value(current_op);

   // only update etime if not yet scheduled
   if (info->is_scheduled()) continue;

   for (Op_inedges_rel ei(current_op, NON_CONTROL0_INEDGES); ei != 0; ei++)
   {
     src_op = real_op((*ei)->src());
     assert (opInfoMap->is_bound(src_op));
     OpInfo *info2 = opInfoMap->value(src_op);
     new_early_time = info2->get_etime() + (*ei)->latency();
     if (new_early_time > max_early_time)
       max_early_time = new_early_time;
   }

   info->set_etime(max_early_time);
 }

 // figure out increase in exit's etime
 int diff = max_early_time - etime_map->value(current_op);
 return diff;
}

void recompute_latest_times(Compound_region* region) {
 Op *current_op, *dest_op;
 int new_late_time, min_late_time;

 for (Region_ops_reverse_C0_order iter(region); iter != 0; ++iter)
 {
   current_op = *iter;
   min_late_time = -1;

   assert (opInfoMap->is_bound(current_op));
   OpInfo *info = opInfoMap->value(current_op);

   // only update ltime if not yet scheduled
   if (info->is_scheduled()) continue;

   for (Op_outedges_rel eo(current_op, NON_CONTROL0_OUTEDGES); eo != 0; eo++)
   {
     dest_op = real_op((*eo)->dest());
     assert (opInfoMap->is_bound(dest_op));
     OpInfo *info2 = opInfoMap->value(dest_op);
     new_late_time = info2->get_ltime() - (*eo)->latency();
     if (min_late_time == -1 || new_late_time < min_late_time)
       min_late_time = new_late_time;
   }

   if (min_late_time != -1)
     info->set_ltime(min_late_time);
 }
}

void adjust_load_use_latency(Compound_region* region, 
		Hash_map<Op*,int > *etime_map, Op *op, int diff, int time) {
 int C1_latency = 0,C2_latency,res_dist_slack = 0;
 int lat;
 LoadInfo* ldinfo = NULL;
 OpInfo* info;

 C2_latency = load_miss_lat(op);

 if(loadInfoMap->is_bound(op)) {
  ldinfo = loadInfoMap->value(op);
  res_dist_slack = ELCOR_MIN(ldinfo->get_res_dist_slack(),C2_latency);
 }

 Edge_style es = ES_SCALAR;
 if (!ldinfo || !ldinfo->get_prefetch()) {
  // check if we need to adjust latency
  if (diff || time > etime_map->value(op)) {
   if (!is_prefetch(op)) {

     for (Op_outedges_rel eiter(op,new Outedge_port_filter(DEST));
							eiter!=0;eiter++) {
       Edge* ed = (*eiter);

      // save the current edge latency and reset it (to hit latency)

       int curr_latency = ed->latency();

       // new edge latency is hit latency
       set_edge_latency(ed, es);
       C1_latency = ed->latency();

       // if latency was not slack-limited, don't need to change it
       info = opInfoMap->value(op);
       if (curr_latency > C1_latency && 
	   curr_latency - C1_latency < info->get_ltime() - info->get_etime())
	 lat = curr_latency;
       else lat = 0;

       // calc new latency
       lat = ELCOR_MAX(res_dist_slack, 
		 ELCOR_MAX(lat, 
		     ELCOR_MAX(C1_latency,
			 ELCOR_MIN(curr_latency-(time-etime_map->value(op))+diff,
			     C2_latency))));
      ed->set_latency(lat);

      // fill in ldinfo final_lat field
      if (ldinfo)
	ldinfo->set_final_lat(lat);
    }
   }
   else {
    Op_outedges_rel eitr(op,new Outedge_port_filter(MEM));
    for (;eitr!=0;eitr++) {
      // should only be one MEM edge from pre (to load) currently
      assert(is_load((*eitr)->dest()));
      break;
    }
    assert(eitr!=0);

    if (ldinfo) {
      C1_latency = ldinfo->get_presched_lat() - (*eitr)->latency();
      assert(C1_latency >= 0);
    }

    // save the current edge latency and reset it (to 0 latency)
    int curr_latency = (*eitr)->latency();
    set_edge_latency((*eitr), es);

    // if latency was not slack-limited, don't need to change it
    info = opInfoMap->value(op);
    if (curr_latency > 0 && 
	   curr_latency < info->get_ltime() - info->get_etime())
      lat = curr_latency;
    else lat = 0;

    // calc new latency
    lat = ELCOR_MAX(res_dist_slack, 
		ELCOR_MAX(lat, 
	   	    ELCOR_MIN(curr_latency-(time-etime_map->value(op))+diff,
		        C2_latency)));
    (*eitr)->set_latency(lat);

    // fill in ldinfo final_lat field
    if (ldinfo)
      ldinfo->set_final_lat(lat+C1_latency);
   }
  }
  else {
    // still need to fill in the final_lat field
    if (ldinfo)
      ldinfo->set_final_lat(ldinfo->get_presched_lat());
  }
 }
}
