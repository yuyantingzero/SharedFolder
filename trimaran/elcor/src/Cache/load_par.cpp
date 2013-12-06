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
//      File:           load_par.cpp
//      Authors:        Santosh G. Abraham
//      Created:        October 1995
//      Description:    Measure parallelism/slack associated with missing loads
//
/////////////////////////////////////////////////////////////////////////////

#include "load_par.h"
#include "load_info.h"
#include "ls_get_profile.h"
#include "ls_efilt.h"
#include "ls_prof_mem_edge.h"

#include "iterators.h"
#include "op_info.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "dbg.h"
#include "intf.h"
#include "sched_functions.h"
#include "opcode.h"

// create a Vector of size MAX_MODELS and an edge filter for each model
Vector<Edge_filter* > * init_load_par() {
  Vector<Edge_filter*> * efilt_arr =
		 new Vector<Edge_filter* > ((int) MAX_MODELS);
  (*efilt_arr)[ALL_DEP] = new No_C0_dep_filter();
  (*efilt_arr)[NO_MEM_DEP] = new No_mem_dep_filter();
  (*efilt_arr)[NO_BR_MEM_DEP] = new No_br_mem_dep_filter();
  (*efilt_arr)[FLOW_DEP] = new Reg_flow_filter();
  return efilt_arr;
}

LSBlockInfo* create_loadinfo_map (Compound_region* region) {

 loadInfoMap = new Hash_map<Op*, LoadInfo*> (hash_op_ptr, 211);
 LSBlockInfo* lsBlockInfo = new LSBlockInfo;
 lsBlockInfo->region = region;
 lsBlockInfo->cb_exec_freq = region->weight;
 int crit_path_len = MDES_branch_latency() - 1;
 for (Region_exit_ops xiter(region); xiter !=0; xiter++) {
   OpInfo* info = opInfoMap->value(*xiter);
   crit_path_len = ELCOR_MAX (crit_path_len,
		 (info->get_etime() + MDES_branch_latency() - 1));
 }
 lsBlockInfo->crit_path_len = crit_path_len;
 int init_val = 0;
 lsBlockInfo->num_loads_mr.resize(NUM_MR_BUCKETS, init_val);
 int num_ops = 0;
 int num_exits = 0;
 float stalls = 0.0f; 
 for (Region_ops_C0_order oiter(region); oiter != 0; oiter++) {
   Op* op = (*oiter);
   if (!is_pseudo(op)) num_ops++;
   assert (opInfoMap->is_bound(op));
   OpInfo* info = opInfoMap->value(op);
   info->init_ltimes(MAX_MODELS, -1);
   if (is_branch(op)) num_exits++;
   if (!is_load(op)) continue;
     double mr = get_load_miss_ratio(op);
     int exec_freq = get_load_exec_freq(op);
     stalls += (float)(mr*exec_freq);
     bool incflag = false;
     for (int i=0; i<NUM_MR_BUCKETS; i++) {
       if (mr <= bkts_lim[i]) {
         lsBlockInfo->num_loads_mr[i] += 1;
         incflag = true;
         break;
       }
     }
     assert (incflag);
     if (mr <= MR_GATHER_STATS) continue;
       assert (!loadInfoMap->is_bound(op));
       LoadInfo* ldinfo = new LoadInfo(op);
       loadInfoMap->bind(op, ldinfo);
       ldinfo->set_load_op(op);
       ldinfo->set_miss_ratio((float)mr);
       ldinfo->set_exec_freq(exec_freq);
       int data_miss_count = get_load_miss_count(op);
       ldinfo->set_data_miss_count(data_miss_count);
       ldinfo->set_pos_in_block(num_ops);
     
       ldinfo->set_etime(info->get_etime());
       ldinfo->set_ltime(info->get_ltime());
       ldinfo->set_slack(info->get_ltime() - info->get_etime());
 }
 lsBlockInfo->num_ops = num_ops;
 lsBlockInfo->num_exits = num_exits;
 lsBlockInfo->norm_stall_cycles = stalls;
 for (Hash_map_iterator<Op*, LoadInfo*> miter(*loadInfoMap);
	miter != 0; miter++) {
   (*miter).second->set_num_ops_in_block(num_ops);
   (*miter).second->set_block_path_len(crit_path_len);
 }
 return lsBlockInfo;
}

void determine_loadpar (Compound_region* region,
	Vector<Edge_filter* >* efilt_arr) {

 Hash_map <Op*, int> early_map (hash_op_ptr, 211);
 Hash_map <Op*, int> late_map  (hash_op_ptr, 211);

 if (loadInfoMap == NULL)
   create_loadinfo_map(region); // KF 12/2004

 for (int dep_model = 0; dep_model < MAX_MODELS; dep_model++) {
   Edge_filter* efilt = (*efilt_arr)[dep_model];
   early_map.clear();
   Region_entry_ops ei (region);
   Op* entry_op = (*ei);
   calculate_earliest_times_from_entry_op (region, entry_op, early_map, efilt);
   late_map.clear();
   Region_ops_reverse_C0_order exi(region);
   assert (region->is_exit_op(*exi));
   Op* fall_thru_exit = (*exi);
   calculate_latest_times_from_exit_op (region, fall_thru_exit,
					late_map, efilt);
   int exit_op_ltime = late_map.value(fall_thru_exit);
   //  MDES_branch_latency does not currently set branch latency of
   //  Dummy_branch to 1
   //   int br_lat = MDES_branch_latency
   //     (el_opcode_to_string_map.value(fall_thru_exit->opcode()));
   int br_lat;
   if (is_dummy_branch(fall_thru_exit)) {
     br_lat = 1;
   } else {
     br_lat = MDES_branch_latency();
   }
   for (Region_ops_C0_order oiter(region); oiter != 0; oiter++) {
     Op* op = (*oiter);
     if (is_load(op) && loadInfoMap->is_bound(op)) {
       LoadInfo* ldinfo = loadInfoMap->value(op);
       int num_ancestors = 0;
       for (Op_ancestors ai (op, region, efilt); ai != 0; ai++) {
	 if (!is_pseudo(*ai)) num_ancestors++;
       }
       ldinfo->set_dep_ops_above(dep_model, num_ancestors);
       int num_descend = 0;
       for (Op_descendants di (op, region, efilt); di != 0; di++) {
	 if (!is_pseudo(*di)) num_descend++;
       }
       ldinfo->set_dep_ops_below(dep_model, num_descend);
       int etime_under_dep_model = early_map.value(op);
       ldinfo->set_entry_load_dist (dep_model, etime_under_dep_model);
       if (late_map.is_bound (op)) {
	 int load_exit_dist = (exit_op_ltime + br_lat) - late_map.value(op);
	 assert (load_exit_dist >= 0);
	 ldinfo->set_load_exit_dist (dep_model, load_exit_dist);
       } else {
	 int load_exit_dist = 0;
	 ldinfo->set_load_exit_dist (dep_model, load_exit_dist);
       }
     }
   }
 }
}
