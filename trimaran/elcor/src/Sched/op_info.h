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
//      File:           op_info.h
//      Authors:        Santosh G. Abraham, Vinod Kathail
//			Srini Mantripragada
//      Created:        April 1994
//      Description:    OpInfo class holding info per op basis for scheduling
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OP_INFO_
#define _OP_INFO_

#include "defs.h"
#include "op.h"
#include <stdio.h>

typedef enum { scheduled, attempted } sched_attr;

class OpInfo {
public:
  OpInfo(Op* op);
  ~OpInfo();

  int get_etime() const; /* gets dag earliest time */
  int get_ltime() const; /* gets dag latest time */
  int get_ltime(int pos) const;	// ltime from pos'th location from Vector
  int get_num_ltimes() const;	// number of ltimes (0 <= pos < get_num_ltimes
  int get_entry_res_dist() const;  // resource cycles of ops above
  int get_exit_res_dist() const;	// exit_res_dist 
  int get_exit_res_dist(int pos) const;	// exit_res_dist from pos'th location
  double get_priority() const;		// scheduling priority of operation
  int get_home_block() const;	// home block of operation (block before sched)
  int get_succ() const;  /* no. of dependent successors */
  int get_attempted_time() const; /* time at which this op was previously attempted
				to be scheduled */
  int get_slack() const; /* gets the slack value */
  int get_unsch_times() const; /* no. of times this op has been unscheduled */
  int get_unsch_inedges() const; /* num incoming edges whose srcs are still unsched */
  // scheduled execution priority of op among ops in instruction
  // also, returns minimum required priority under LIST_SCHED before scheduling
  int get_exec_priority() const;
  bool is_visited() const; 
  bool is_scheduled() const;
  bool is_attempted() const;
  bool is_data_spec() const; /* is data speculated */
  bool is_control_spec() const; /* is control speculated */
  bool is_data_ready() const;	/* is data ready (all preds scheduled) */
  bool is_data_ready_now() const; /* just became data ready (last pred sched) */
  List<Op*> get_tc_set() const; /* get top control set */
  List<Op*> get_bc_set() const; /* get bottom control set */

  void set_etime(int time);
  void set_ltime(int time);
  void init_ltimes (int length, int value = -1);
  void set_ltime(int pos, int value);
  void set_entry_res_dist(int dist);
  void set_exit_res_dist(int dist);
  void init_exit_res_dist (int length, int dist = -1);
  void set_exit_res_dist(int pos, int dist);
  void set_priority(double value);
  void set_home_block(int value);
  void set_succ(int value);
  void incr_succ();
  void incr_unsch_time();
  void set_unsch_inedges(int value);
  void decr_unsch_inedges();
  void set_attempted_time(int pos);
  void set_slack(int value);
  void set_exec_priority (int value);
  void set_visited(bool visit);
  void set_scheduled(bool sched);
  void set_data_spec(bool spec);
  void set_control_spec(bool spec);
  void set_data_ready(bool ready);
  void set_data_ready_now(bool ready_now);
 

  void add_top_control_points(Op* op);
  void add_bottom_control_points(Op* op);
private:
  Op* op_rep;    /* operation id */
  int etime; /* earliest time the op can be scheduled (only dependence) */
  int ltime; /* latest time the op can be scheduled (only dependence) */
  double priority;      // scheduling priority of op
  int home_block;       // home block of op in initial C0 order
  bool visited;  /* used for traversing the graph */
  int succ;     /* no. of successors dependent on this node */
  int slack;         /* slack value */
  int attempted_time; /* time at which the op has been attempted to schedule */
  bool scheduled; /* is scheduled or not */
  int unsch_times; /* no. of times this op has been previously unscheduled  */
  int unsch_inedges; /* num incoming edges whose srcs are unscheduled */
  int exec_priority; // of op within an instruction
  bool attempted; /* is previously attempted or not */
  bool data_spec; /* is data speculated or not */
  bool control_spec; /* is control speculated or not */
  List<Op*> top_list; /* list of all control points upto the highest control
                         point this op can move without breaking the program
                         semantics
                      */
  List<Op*> bottom_list; /* list of all control points upto the lowest control
                            point. */
  Vector<int> ltimes;	// latest time  for op wrt to each branch exit
  bool data_ready;	/* predecessors already scheduled */
  bool data_ready_now;	/* last predecessor scheduled in previous sched cycle */
  int entry_res_dist;   // minimum resource cycles for ops trapped above this op
  int exit_res_dist; // minimum resource cycles for ops trapped below
  Vector<int> exit_res_dists; // minimum resource cycles for ops trapped below
};

extern Hash_map<Op*, OpInfo*>* opInfoMap;
#endif
