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
//      File:           op_info.cpp
//      Authors:        Santosh G. Abraham, Vinod Kathail
//			Srini Mantripragada
//      Created:        April 1994
//      Description:    OpInfo class holding info per op basis for scheduling
//
/////////////////////////////////////////////////////////////////////////////

#include "op_info.h"

  OpInfo::OpInfo(Op* op)
  : op_rep(op), etime(-1), ltime(-1), priority(0.0), home_block(-1),
    visited(false), succ(0), slack(0), attempted_time(-1), scheduled(false),
    unsch_times(0), unsch_inedges(0), exec_priority(-1),
    attempted(false), data_spec(false), control_spec(false),
    entry_res_dist(-1), exit_res_dist(-1) {
    top_list.clear();
    bottom_list.clear();
  }

  OpInfo::~OpInfo()
  {
    bottom_list.clear();
    top_list.clear();
//    delete ltimes;
  }

  int OpInfo::get_etime() const { return etime; }
  int OpInfo::get_ltime() const { return ltime; }
  int OpInfo::get_ltime(int pos) const { return ltimes[pos]; }
  int OpInfo::get_num_ltimes () const { return (int)ltimes.size(); }
  int OpInfo::get_entry_res_dist() const {return entry_res_dist; }
  int OpInfo::get_exit_res_dist() const {return exit_res_dist; }
  int OpInfo::get_exit_res_dist(int pos) const {return exit_res_dists[pos]; }
  double OpInfo::get_priority() const { return priority; }
  int OpInfo::get_home_block() const { return home_block; }
  int OpInfo::get_succ() const { return succ; }
  int OpInfo::get_attempted_time() const { return attempted_time; }
  int OpInfo::get_slack() const { return slack; }
  int OpInfo::get_unsch_times() const { return unsch_times; }
  int OpInfo::get_unsch_inedges() const { return unsch_inedges;} 
  int OpInfo::get_exec_priority() const { return exec_priority; }
  bool OpInfo::is_visited() const { return visited; }
  bool OpInfo::is_scheduled() const { return scheduled; }
  bool OpInfo::is_attempted() const { return attempted; }
  bool OpInfo::is_data_spec() const { return data_spec; }
  bool OpInfo::is_control_spec() const { return control_spec; }
  bool OpInfo::is_data_ready() const { return data_ready; }
  bool OpInfo::is_data_ready_now() const { return data_ready_now; }
  
  List<Op*> OpInfo::get_tc_set() const { return top_list; }
  List<Op*> OpInfo::get_bc_set() const { return bottom_list; }

  void OpInfo::set_etime(int time) { etime = time; }
  void OpInfo::set_ltime(int time) { ltime = time; }
  void OpInfo::init_ltimes(int length, int value) {
     ltimes.resize (length, value); }
  void OpInfo::set_ltime(int pos, int value) { ltimes[pos] = value ; }
  void OpInfo::set_entry_res_dist(int dist) {entry_res_dist = dist; }
  void OpInfo::set_exit_res_dist(int dist) {exit_res_dist = dist; }
  void OpInfo::init_exit_res_dist (int length, int dist) {
	exit_res_dists.resize (length, dist); }
  void OpInfo::set_exit_res_dist(int pos, int dist) {exit_res_dists[pos] = dist; }
  void OpInfo::set_priority(double value) {priority = value; }
  void OpInfo::set_home_block(int value) {home_block = value; }
  void OpInfo::set_succ(int value) { succ = value; }
  void OpInfo::incr_succ() { succ++; }
  void OpInfo::incr_unsch_time() { unsch_times++; }
  void OpInfo::set_unsch_inedges(int value) { unsch_inedges = value; }
  void OpInfo::decr_unsch_inedges() { unsch_inedges--; }
  void  OpInfo::set_attempted_time(int pos) 
    { attempted = true; attempted_time = pos; }
  void OpInfo::set_slack(int value) { slack = value; }
  void OpInfo::set_exec_priority(int value) { exec_priority = value; }
  void OpInfo::set_visited(bool visit) { visited = visit; }
  void OpInfo::set_scheduled(bool sched) { scheduled = sched; }
  void OpInfo::set_data_spec(bool spec) { data_spec = spec; }
  void OpInfo::set_control_spec(bool spec) { control_spec = spec; }
  void OpInfo::set_data_ready(bool ready) {  data_ready = ready; }
  void OpInfo::set_data_ready_now(bool ready_now) {data_ready_now = ready_now; }
    
  void OpInfo::add_top_control_points(Op* op) { top_list.add_tail(op); }
  void OpInfo::add_bottom_control_points(Op* op) { bottom_list.add_tail(op); }

  Hash_map<Op*, OpInfo*>* opInfoMap;
