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
//      File:           load_info.cpp
//      Authors:        Santosh G. Abraham
//      Created:        October 1995
//      Description:    LoadInfo class for managing info on each load op
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "load_info.h"

using namespace std;

// array bkts_lim contains upper limits for each bucket 0..(NUM_MR_BUCKETS-1)
const float bkts_lim[NUM_MR_BUCKETS] = {0.05f, 0.25f, 0.5f, 0.75f, 0.9f, 1.0f};

LSBlockInfo::LSBlockInfo() {}
LSBlockInfo::~LSBlockInfo() {}

LoadInfo::LoadInfo(Op* op)
 : op_rep(op), miss_ratio(-1.0f), exec_freq(-1), data_miss_count(-1),
   num_ops_in_block(-1), etime(-1), nomem_etime(-1), ltime(-1), slack(-1),
   block_path_len(-1), 
   entry_res_dist(-1), exit_res_dist(-1), res_dist_slack(-1),
   last_jsr(NULL), prefetch(NULL), ldv(NULL),
   presched_lat(-1), updated_ltime(-1), final_lat(-1), sched_lat(-1) {
  init_dep_ops_above();
  init_dep_ops_below();
  init_entry_load_dist();
  init_load_exit_dist();
}

LoadInfo::~LoadInfo() { }

Op* LoadInfo::get_load_op() const { return op_rep; }

float LoadInfo::get_miss_ratio() const { return miss_ratio; }
int LoadInfo::get_exec_freq() const { return exec_freq; }
int LoadInfo::get_data_miss_count() const { return data_miss_count; }

int LoadInfo::get_dep_ops_above(int model) const {
  return dep_ops_above[model]; }
int LoadInfo::get_dep_ops_below(int model) const {
  return dep_ops_below[model]; }
int LoadInfo::get_pos_in_block() const { return pos_in_block; }
int LoadInfo::get_num_ops_in_block() const { return num_ops_in_block; }

Op* LoadInfo::get_last_jsr() const { return last_jsr; }
Op* LoadInfo::get_prefetch() const { return prefetch; }
Op* LoadInfo::get_ldv() const { return ldv; }

int LoadInfo::get_etime() const { return etime; }
int LoadInfo::get_nomem_etime() const { return nomem_etime; }
int LoadInfo::get_ltime() const { return ltime; }
int LoadInfo::get_slack() const { return slack; }
int LoadInfo::get_block_path_len() const { return block_path_len; }
int LoadInfo::get_entry_res_dist() const {return entry_res_dist; }
int LoadInfo::get_exit_res_dist() const {return exit_res_dist; }
int LoadInfo::get_res_dist_slack() const { return res_dist_slack; }

int LoadInfo::get_entry_load_dist(int model) const {
  return entry_load_dist[model]; }
int LoadInfo::get_load_exit_dist(int model) const {
  return load_exit_dist[model]; }

int LoadInfo::get_presched_lat() const { return presched_lat; }
int LoadInfo::get_updated_ltime() const {return updated_ltime; }
int LoadInfo::get_final_lat() const { return final_lat; }
int LoadInfo::get_sched_lat() const { return sched_lat; }

void LoadInfo::set_load_op(Op* load_op) { op_rep = load_op; }

void LoadInfo::set_miss_ratio(float value) { miss_ratio = value; }
void LoadInfo::set_exec_freq(int value) {  exec_freq = value; }
void LoadInfo::set_data_miss_count(int value) { data_miss_count = value; }

void LoadInfo::init_dep_ops_above(int num_models, int value) {
 dep_ops_above.resize (num_models, value); }
void LoadInfo::set_dep_ops_above (int model, int value) { 
 dep_ops_above[model] = value ; }
void LoadInfo::init_dep_ops_below(int num_models, int value) {
 dep_ops_below.resize (num_models, value); }
void LoadInfo::set_dep_ops_below (int model, int value) {
 dep_ops_below[model] = value ; }
void LoadInfo::set_pos_in_block(int value) { pos_in_block = value; }
void LoadInfo::set_num_ops_in_block(int value) { num_ops_in_block = value; }

void LoadInfo::set_etime(int time) { etime = time; }
void LoadInfo::set_nomem_etime(int time) { nomem_etime = time; }
void LoadInfo::set_ltime(int time) { ltime = time ; }
void LoadInfo::set_slack(int value) { slack = value ; }
void LoadInfo::set_block_path_len(int length) { block_path_len = length; }
void LoadInfo::set_entry_res_dist(int dist) {entry_res_dist = dist; }
void LoadInfo::set_exit_res_dist(int dist) {exit_res_dist = dist; }
void LoadInfo::set_res_dist_slack(int value) { res_dist_slack = value ; }

void LoadInfo::set_last_jsr(Op *jsr) { last_jsr = jsr; }
void LoadInfo::set_prefetch(Op *pre) { prefetch = pre; }
void LoadInfo::set_ldv(Op *ld) { ldv = ld; }
 
void LoadInfo::init_entry_load_dist(int num_models, int value)
 { entry_load_dist.resize (num_models, value); }
void LoadInfo::set_entry_load_dist (int model, int value) { 
   entry_load_dist[model] = value ; }
void LoadInfo::init_load_exit_dist(int num_models, int value)
 { load_exit_dist.resize (num_models, value) ; }
void LoadInfo::set_load_exit_dist (int model, int value) {
   load_exit_dist[model] = value  ; }

void LoadInfo::set_presched_lat(int lat)  { presched_lat = lat; }
void LoadInfo::set_updated_ltime(int ltime) { updated_ltime = ltime; }
void LoadInfo::set_final_lat(int lat) { final_lat = lat; }
void LoadInfo::set_sched_lat(int lat) { sched_lat = lat; }

ostream&
operator << (ostream& os, const LoadInfo& ldi)
{
  const char TAB = '\t';	// separator between fields
  const char SPC = ' ';		// seperator between sub-fields
  const int WOP = 5;	// width of op id field
  const int WMR = 6;	// width of miss-ratio field
  const int WEF = 7;	// width of exec freq field
  const int WMC = 7;	// width of miss count field
  const int WDO = 3;	// width dependent ops field
  const int WPB = 3;	// width of pos in block field
  const int WNB = 4;	// width of num ops in block field
  const int WTM = 3;	// width of time field
  
  int old_precision = os.precision();

  // load resource (LR) info
  os.width(WOP); os << "LR: " << ldi.op_rep->id() << TAB;
  os.width(WMR); os.precision(WMR); os << ldi.miss_ratio << SPC;
  os.width(WMC); os << ldi.data_miss_count << SPC;
  os.width(WEF); os << ldi.exec_freq << TAB;
  os.width(WDO);
  for (int i=0; i<MAX_MODELS; i++) {
    os << ldi.dep_ops_above[i] << SPC;
  }
  os << TAB;
  for (int j=0; j<MAX_MODELS; j++) {
    os << ldi.dep_ops_below[j] << SPC;
  }
  os.width(WPB); os << TAB << ldi.pos_in_block << SPC;
  os.width(WNB); os << ldi.num_ops_in_block << TAB;
  os << endl;

  // load dependence (LD) info
  os.width(WTM); os << "LD: "   << ldi.etime << SPC
		    		<< ldi.ltime << SPC
				<< ldi.slack << SPC
				<< ldi.block_path_len << TAB;
  os.width(WDO);
  for (int k=0; k<MAX_MODELS; k++) {
    os << ldi.entry_load_dist[k] << SPC;
  }
  os << TAB;
  for (int l=0; l<MAX_MODELS; l++) {
    os << ldi.load_exit_dist[l] << SPC;
  }
  os << endl;

  // load schedule (LS) info
  os << "LS: ";
  os.width (WTM); os << (ldi.etime - ldi.nomem_etime);
  (ldi.prefetch) ? os << " pf-" << ldi.prefetch->id() << " " : os << " pf-N ";
  (ldi.ldv) ? os << " ldv-" << ldi.ldv->id() << " " : os << " ldv-N ";
  os << ldi.res_dist_slack << SPC
     << ldi.presched_lat << SPC
     << ldi.updated_ltime << SPC
     << ldi.final_lat << SPC
     << ldi.sched_lat;
  os << endl;

  os.precision(old_precision);
  return os;
}

ostream&
operator << (ostream& os, const LSBlockInfo& lbi)
{
  const char TAB = '\t';	// separator between fields
  const char SPC = ' ';		// separator between sub-fields
  const int WEF = 8;
  const int PEF = 4;
  const int WRG = 4;
  const int WNO = 3;
  const int WNE = 2;
  const int WTM = 4;
  const int WST = 6;
  const int PST = 3;
  const int WNL = 3;

  int old_precision = os.precision();

  os.width(WRG); os <<"B: " << lbi.region->id() << TAB;
  os.width(WEF); os.precision(PEF); os << lbi.cb_exec_freq << TAB;
  os.width(WNO); os << lbi.num_ops << SPC;
  os.width(WNE); os << lbi.num_exits << SPC;
  os.width(WTM); os << lbi.crit_path_len << SPC << lbi.comp_cycles << SPC;
  os.width(WST); os.precision(PST); os << lbi.norm_stall_cycles << TAB;

  os.width(WNL);
  for (int j=0; j<NUM_MR_BUCKETS; j++) {
    os << lbi.num_loads_mr[j] << SPC;
  }
  os << TAB;
  for (int k=0; k<MAX_RES; k++) {
    os << lbi.res_use[k] << SPC;
  }
  os << endl;

  os.precision(old_precision);

  for (int l=0; l<lbi.num_exits; l++) {
    os << (*(lbi.exitInfoVec[l]));
  }

  return os;
}

ostream&
operator << (ostream& os, const LSExitInfo& lxi)
{
  const char TAB = '\t';	// separator between fields
  const char SPC = ' ';		// separator between sub-fields
  const int WOP = 5;
  const int WEF = 8;
  const int PEF = 4;
  const int WRG = 4;
  const int WTM = 4;
  const int WRS = 3;

  int old_precision = os.precision();

  os.width(WOP); os <<"E: " << lxi.exit_op->id() << TAB;
  os.width(WRG); os << lxi.parent_region->id() << SPC;
  if (lxi.target_region == NULL) {
    os << -1 << TAB;
  } else {
    os << lxi.target_region->id() << TAB;
  }
  os.width(WEF); os.precision(PEF); os << lxi.exit_freq << SPC;
  os.width(WTM); os << lxi.crit_path_len << TAB;

  os.width(WRS);
  for (int k=0; k<MAX_RES; k++) {
    os << lxi.res_use[k] << SPC;
  }

  os << endl;
  os.precision(old_precision);
  return os;
}

LSExitInfo::LSExitInfo() {}
LSExitInfo::~LSExitInfo() {}

Hash_map<Op*, LoadInfo*>* loadInfoMap;
LSBlockInfo* lsBlockInfo;
