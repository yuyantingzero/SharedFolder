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
//      File:           priority.cpp
//      Authors:        Santosh G. Abraham, Vinod Kathail
//			Brian Dietrich, Srini Mantripragada
//      Created:        April 1994
//      Description:    Priority class and derived classes for op priority
//
/////////////////////////////////////////////////////////////////////////////

#include "defs.h"
#include "priority.h"
#include "iterators.h"
#include "sched_attributes.h"
#include "sched_functions.h"
#include "op_info.h"
#include "slist.h"
#include "region_utilities.h"
#include "el_ssched_init.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "dbg.h"

void display_sched (Compound_region *r);

Priority::Priority(Compound_region* region, const Relax_dep_constr* relax_dep, 
  const Op_filter* ofilter, const Edge_filter* efilter, bool delete_flag)
   : reg(region), relax_rep(relax_dep), ofilter_rep(ofilter), 
     efilter_rep(efilter), delete_rep(delete_flag) {
     oplist.clear();
     ready_list.clear();
    }

Priority::~Priority() {
  for (Hash_map_iterator<Op*, OpInfo*> iter(*opInfoMap);
  		iter != 0; iter++) {
    OpInfo* info = (*iter).second;
    delete info;
  }
  opInfoMap->clear();
  assert(opInfoMap->is_empty());
  if(delete_rep) {
    delete (Relax_dep_constr*) relax_rep;
    delete (Op_filter*) ofilter_rep;
    delete (Edge_filter*) efilter_rep;
  }
}

void Priority::initialize() {
  cerr << "Derived classes must always override this routine" << endl;
  assert (0);
}

bool Priority::is_early() const { return false; }
bool Priority::is_list() const { return false; }
bool Priority::is_dep_pri() const { return false; }
bool Priority::is_dep_pri_bt() const { return false; }
bool Priority::is_pri_bt() const { return false; }
bool Priority::is_slack() const { return false; }
bool Priority::is_topo() const { return false; }

const Relax_dep_constr* Priority::get_relax_constr_function() { return relax_rep; }

/* gets the list of all ops which are ready (whose data dependences are met)
   to be scheduled at the current time (passed as argument). Among the ready 
   ops, the ops are prioritized using the number of successors that are 
   dependent on this node to be scheduled */

void Priority::get_ready_list(const List<Op*>& op_list, 
	                           const Op_filter* filter, int time)
{
  List<Op*> oprs;
  Op* op_node;
  ready_list.clear();
  Ops_sched_time time_filter(time);

 /* filtering out operations which can be potentially be executed at "time" */

  for(List_filterator<Op*> iter(op_list, &time_filter);iter !=0;++iter)
    oprs.add_head(*iter);

 /* sorting the ready nodes based on sucessors count */
  while ((op_node = get_next_ready_node(oprs, filter, MT_LIST)) != 0)
  {
    ready_list.add_tail(op_node);
    oprs.remove(op_node);
  }
}

/* List of all unscheduled ops in the region */

void Priority::create_op_list(Compound_region* region, const Op_filter* filter)
{
  for(Region_ops_C0_order iter(region); iter != 0; ++iter)
  {
    if (filter->filter(*iter)) continue;
    if(opInfoMap->is_bound(*iter))
    {
      OpInfo* info = opInfoMap->value(*iter);
      if(!info->is_scheduled()) oplist.add_head(*iter);
    }
    else oplist.add_head(*iter);
  }
}

void Priority::op_list_print(List<Op*> plist)
{
   int id, stime, earlytime, latetime;
   char c;
   char* s;
   for(List_iterator<Op*> liter(plist); liter != 0; ++liter) {
      OpInfo* info = opInfoMap->value(*liter);
      id = (*liter)->id();
      stime = (*liter)->sched_time();
      if (info->is_scheduled()) c = 'Y';
      else c = 'N';
      // s = el_opcode_to_string_map.value((*liter)->opcode());
      s = get_mdes_opcode_string((*liter));
      earlytime = info->get_etime();
      latetime = info->get_ltime();
      
      cdbg << id << "\t" << s << "\t" << stime << "\t" << c << "\t"
	   << earlytime << "\t" << latetime << "\n";
   }
   cdbg << endl;
}

void Priority::op_slist_print(Slist<Stuple <double, Op*> > plist)
{
   int id, stime, earlytime, latetime;
   double sortkey, lastkey;
   char c;
   char* s;
   
   cdbg << "Sort\tId\tOp\tStime\tSched?\tEtime\tLtime" << endl;
   lastkey = -(ELCOR_MAX_DOUBLE);
   for(Slist_iterator<Stuple<double, Op* > > siter(plist); siter != 0; ++siter) {
      sortkey = (*siter).first;
      assert((lastkey < sortkey) || lastkey == sortkey);
      lastkey = sortkey;
      Op* op = (*siter).second;
      OpInfo* info = opInfoMap->value(op);
      id = op->id();
      stime = op->sched_time();
      if (info->is_scheduled()) c = 'Y';
      else c = 'N';
      // s = el_opcode_to_string_map.value(op->opcode());
      s = get_mdes_opcode_string(op);
      earlytime = info->get_etime();
      latetime = info->get_ltime();
      
      cdbg  << sortkey << "\t" << id << "\t" << s << "\t" << stime << "\t"
      	   << c << "\t" << earlytime << "\t" << latetime << endl;
   }
   cdbg << endl;
}


/* This priority function is naive and is used to schedule all operations as
   soon as they are ready. Among the operations which are ready at the same
   time interval, the one with more breadth (successor count) is considered.
   The schedules obtained are better than the one obtained from a simple
   topological sort as it considers height/ breadth ratio when compared to
   a topological sort which does not consider breadth into account.
*/
Early::Early(Compound_region* region, const Relax_dep_constr* relax_dep, 
       const Op_filter* ofilter, const Edge_filter* efilter, bool delete_filter)
    :Priority(region, relax_dep, ofilter, efilter, delete_filter)
{
  // calculate_earliest_times(reg, relax_rep);
}

Early::~Early() {}

bool Early::is_early() const { return true; }

bool Early::operator==(const void* x) const {
  return (x == nextop);
}

bool Early::operator!=(const void* x) const {
  return !operator==(x);
}

void Early::operator++() {
  get_next_ready_op();
}

Op* Early::operator*() {
   return nextop;
}

void Early::get_next_ready_op()
{
  if(oplist.is_empty()) { nextop = NULL; return; }
  iter++;
  if(iter == 0) 
  { 
    ready_list.clear();
    while(ready_list.is_empty()) 
      get_ready_list(oplist, ofilter_rep, time++);
    iter = List_filterator<Op*>(ready_list, ofilter_rep);
  }
  nextop = *iter;
  oplist.remove(nextop);
}

void Early::reschedule(Op* op, Compound_region* reg)
{
//  OpInfo* info = opInfoMap->value(op);
  calculate_earliest_times(reg, relax_rep);
  initialize();
}

void Early::initialize()
{
  time = 0;
  ready_list.clear(); oplist.clear();
  create_op_list(reg, ofilter_rep);  /* list of all unsched ops in the region */
  while(ready_list.is_empty()) 
    get_ready_list(oplist, ofilter_rep, time++);
  iter = List_filterator<Op*>(ready_list, ofilter_rep);
  nextop = *iter;
  oplist.remove(nextop);
}
  
Listt::Listt(Compound_region* region, const Relax_dep_constr* relax_dep, 
   const Op_filter* ofilter, const Edge_filter* efilter,
   const Edge_filter* eoutfilter, bool delete_filter)
    : Priority(region, relax_dep, ofilter, efilter, delete_filter),
      eoutfilter_rep(eoutfilter)    	
{
  data_ready_slist.clear();
}

Listt::~Listt() {
  if(delete_rep) {
    delete (Edge_filter*) eoutfilter_rep;
  }
}

bool Listt::is_list() const { return true; }

bool Listt::operator==(const void* x) const {
  return (x == nextop);
}

bool Listt::operator!=(const void* x) const {
  return !operator==(x);
}

void Listt::operator++() {
  get_next_ready_op();
}

Op* Listt::operator*() {
   return nextop;
}

/* List of all unscheduled ops in the region */

void Listt::create_op_list(Compound_region* region)
{
   OpInfo* info;
   Stuple<double, Op*> item;
   Slist_iterator< Stuple <double, Op* > > siter;

  for(Region_ops_C0_order iter(region); iter != 0; ++iter)
  {
     assert (!is_switch(*iter) && !is_merge(*iter));
     info = opInfoMap->value(*iter);
     assert (!(info->is_scheduled()));
     if (!(info->is_scheduled())) {
	  item.first = info->get_priority();
	  item.second = (*iter);
	  op_slist.add(item);
     }
  }
  if (dbg (ss, 8)) {
    cdbg << "\nList of operations to be scheduled in region:\n";
    op_slist_print(op_slist);
  }
}

/* gets the list of all ops which are ready (whose data dependences are met)
   Among the ready ops, the ops are prioritized using the number of successors that are 
   dependent on this node to be scheduled */

void Listt::get_ready_list()
{
  Op* op;
  Op* source;
  OpInfo* info;
  OpInfo* src_info;
  bool all_sched = true, pred_sched;
  int min_etime = ELCOR_MAX_INT, op_etime, t;
  Slist_iterator <Stuple<double, Op*> > siter;

  ready_slist.clear();
  data_ready_slist.clear();
  for (siter(op_slist); siter != 0; ++siter) {
     op = (*siter).second;
     assert(opInfoMap->is_bound(op));
     info = opInfoMap->value(op);
     if (!info->is_scheduled()) {
	all_sched = false;
	pred_sched = true;
	op_etime = info->get_etime();
	for (Op_inedges_rel ei(op, efilter_rep); ei != 0; ++ei) {
 	  source = real_op((*ei)->src());
	  if (!region_contains_op(reg, source)) continue;
	  assert (opInfoMap->is_bound(source));
	  src_info = opInfoMap->value(source);
       	  if (src_info->is_scheduled()) {
	    t = (*ei)->latency() + source->sched_time();
	    if (t > op_etime) op_etime = t;
	  }
	  else
	  { pred_sched = false; break; }
	}
	  
	if (pred_sched) {
	   info->set_etime(op_etime);
	   if (op_etime < min_etime) min_etime = op_etime;
	   data_ready_slist.add_tail(*siter);
	}
     }
  }

  if (all_sched) { nextop = NULL; return; }
  assert (!data_ready_slist.is_empty());
  
  for (siter(data_ready_slist); siter !=0; ++siter)
  {
  	  op = (*siter).second;
          assert(opInfoMap->is_bound(op));
	  info = opInfoMap->value(op);
	  if (info->get_etime () == min_etime) ready_slist.add_tail(*siter);
  }
  update_info();
     
  assert(!ready_slist.is_empty());
  if (dbg (ss, 8)) {
     cdbg << "READY_LIST in get_ready_list:\n\n";
     op_slist_print(ready_slist);
  }
  nextop = (ready_slist.pop()).second;
}
	
	
void Listt::get_next_ready_op()
{
  if (dbg (ss, 8)) {
    cdbg << "DATA_READY_SLIST on entering get_next_ready_op:\n\n";
    op_slist_print(data_ready_slist);
  }
  if (nextop != NULL) {
     assert(opInfoMap->is_bound(nextop));
     OpInfo* info = opInfoMap->value(nextop);
     if (info->is_scheduled()) {

#ifdef NO_FILL_DELAY_SLOTS
       if (is_control_switch(nextop)) {
	 int cycle_after_delay_slots =
	   nextop->sched_time() + NUM_DELAY_SLOTS + 1;
	 for  (Slist_iterator <Stuple<double, Op*> > siter (op_slist);
	       siter != 0; siter++) {
	   Op* op = (*siter).second;
	   OpInfo* info = opInfoMap->value(op);
	   if (!info->is_scheduled()) {
	     info->set_etime(ELCOR_MAX (info->get_etime(),cycle_after_delay_slots));
	   }
	 }
       }
#endif				  
	get_ready_list();
	return;
     }
  }
  if (ready_slist.is_empty()) {get_ready_list(); return;}
  nextop = (ready_slist.pop()).second;
  return;
}

void Listt::reschedule(Op* op, Compound_region* reg)
{
  OpInfo* info = opInfoMap->value(op);
  if (dbg (ss, 8)) {
    cdbg << "Cannot schedule at: " << info->get_etime()
    	<< " retry at: " << (info->get_etime() + 1) << "\n\n";
    }
  info->set_etime(info->get_etime() + 1);
}

void Listt::initialize()
{
  time = 0;
  ready_slist.clear(); data_ready_slist.clear(); op_slist.clear();
  create_op_list(reg);
  get_ready_list();
}

void Listt::update_info()
{
  Op* op;
  OpInfo* info;
  Slist_iterator <Stuple<double, Op*> > siter;
  if (!El_ssched_graph_display) return;
  for (siter(op_slist); siter != 0; ++siter) {
     op = (*siter).second;
     assert(opInfoMap->is_bound(op));
     info = opInfoMap->value(op);
     info->set_data_ready(false);
     info->set_data_ready_now(false);
  }
  for (siter(data_ready_slist); siter !=0; ++siter) {
     op = (*siter).second;
     assert(opInfoMap->is_bound(op));
     info = opInfoMap->value(op);
     info->set_data_ready(true);
  }
  for (siter(ready_slist); siter !=0; ++siter) {
     op = (*siter).second;
     assert(opInfoMap->is_bound(op));
     info = opInfoMap->value(op);
     info->set_data_ready_now(true);
  }  
}

Dep_pri::Dep_pri(Compound_region* region, const Relax_dep_constr* relax_dep, 
   const Op_filter* ofilter, const Edge_filter* efilter,
   const Edge_filter* eoutfilter, bool delete_filter)
    : Priority(region, relax_dep, ofilter, efilter, delete_filter),
      eoutfilter_rep(eoutfilter)    	
{}

Dep_pri::~Dep_pri() {
  if(delete_rep) {
    delete (Edge_filter*) eoutfilter_rep;
  }
}

bool Dep_pri::is_dep_pri() const { return true; }

bool Dep_pri::operator==(const void* x) const {
  return (x == nextop);
}

bool Dep_pri::operator!=(const void* x) const {
  return !operator==(x);
}

void Dep_pri::initialize()
{
  ready_slist.clear();

  /*
  // following to reproduce earlier scheduling implementation
  // priorities are faked to produce previous implementation's ordering
  // priorites after this point do not correspond to real priorities
  int num_ops = 0;
  Slist<Stuple <double, Op*> >pr_op_slist;
  for (Region_ops_C0_order oi (reg); oi != 0; oi++) {
    num_ops++ ;
    Op* op = (*oi);
    OpInfo* info = opInfoMap->value(op);
    double priority = info->get_priority();
    Stuple <double, Op* > pr_op (priority, op);
    pr_op_slist.add(pr_op);
  }
  double fake_pr = - num_ops;
  for (Slist_iterator <Stuple <double, Op*> > si (pr_op_slist); si != 0; si++) {
    Op* op = (*si).second;
    OpInfo* info = opInfoMap->value(op);
    info->set_priority(fake_pr);
    fake_pr += 1.0;
  }
  // remove above section after testing
  */

  for(Region_ops_C0_order iter(reg); iter != 0; ++iter) {
    Op* op = (*iter);
    int num_inedges = 0;
    for (Op_inedges_rel ei(op, efilter_rep); ei != 0; ei++) {
      num_inedges++;
    }
    opInfoMap->value(op)->set_unsch_inedges(num_inedges);
  }

  Region_entry_ops entry(reg);
  Op* cm_op = (*entry);
  // assert (opInfoMap->value(cm_op)->get_unsch_inedges() == 0);
  // range of priority values is [ -cm_op_priority, 0]
  max_stat_priority = opInfoMap->value(cm_op)->get_priority() * (-1.01);
  
  nextop = cm_op;
}

void Dep_pri::operator++() {
  if (nextop == NULL) return;
  OpInfo* src_info = opInfoMap->value(nextop);
  assert (src_info->is_scheduled());
  int src_sched_time = nextop->sched_time();
  int src_exec_priority = src_info->get_exec_priority();
  for (Op_outedges_rel ei(nextop, eoutfilter_rep); ei != 0; ei++) {
    Op* dest_op = real_op((*ei)->dest());
    OpInfo* dest_info = opInfoMap->value(dest_op);
    int dest_etime = dest_info->get_etime();
    int edge_lat = (*ei)->latency();
    int new_etime = src_sched_time + (*ei)->latency();
    // if dest etime has advanced, no restrictions on its exec_priority
    // from previously scheduled predecessors
    if (new_etime > dest_etime) {
      dest_info->set_etime(new_etime);
      dest_info->set_exec_priority(-1);
    }
    // if latency 0 edge and the src constrains dest,
    // then dest's eventual scheduled exec priority must exceed src's priority
    if ((El_ssched_priority_model != FREEZE_PRIORITIZED) &&
	(edge_lat == 0) && (new_etime >= dest_etime)) {
      int dest_exec_priority = dest_info->get_exec_priority();
      if (src_exec_priority > dest_exec_priority) 
	dest_info->set_exec_priority(src_exec_priority);
    }

    dest_info->decr_unsch_inedges();
    if (dest_info->get_unsch_inedges() <= 0) {
      assert (dest_info->get_unsch_inedges() == 0);
      double dyn_priority = calculate_dyn_priority (dest_op, dest_info);
      Stuple <double, Op*> pr_op (dyn_priority, dest_op);
      ready_slist.add (pr_op);
    }
  }
  if (ready_slist.is_empty()) {
    nextop = NULL;
  } else {
    nextop = (ready_slist.pop()).second;
  }
  return;
}

Op* Dep_pri::operator*() {
   return nextop;
}

void Dep_pri::reschedule(Op* op, Compound_region* reg)
{
  cerr << "Rescheduling of op unsupported: " << (*op) << endl;
  assert(0);
}

double Dep_pri::calculate_dyn_priority (Op* op, OpInfo* info ) {
  //int etime = info->get_etime();
  double stat_priority = info->get_priority();
  return stat_priority;
  //  return (etime * max_stat_priority + stat_priority);
}

Dep_pri_bt::Dep_pri_bt(Compound_region* region,
			const Relax_dep_constr* relax_dep, 
			const Op_filter* ofilter, const Edge_filter* efilter,
			const Edge_filter* eoutfilter, bool delete_filter)
    : Dep_pri(region, relax_dep, ofilter, efilter, eoutfilter, delete_filter)
{}

void Dep_pri_bt::initialize()
{
  ready_slist.clear();

  for(Region_ops_C0_order iter(reg); iter != 0; ++iter) {
    Op* op = (*iter);
    OpInfo* info = opInfoMap->value(op);
    int etime = info->get_etime();
    int num_inedges = 0;
    for (Op_inedges_rel ei(op, efilter_rep); ei != 0; ei++) {
      num_inedges++;
      if ((*ei)->latency() < 0) info->set_attempted_time(etime - 1);
    }
    info->set_unsch_inedges(num_inedges);
  }

  Region_entry_ops entry(reg);
  Op* cm_op = (*entry);
  max_stat_priority = opInfoMap->value(cm_op)->get_priority() * (-1.01);
  
  nextop = cm_op;
}

Dep_pri_bt::~Dep_pri_bt() {}

bool Dep_pri_bt::is_dep_pri()		const { return false; }
bool Dep_pri_bt::is_dep_pri_bt()	const { return true; }

void Dep_pri_bt::operator++() {
  if (nextop == NULL) return;
  OpInfo* src_info = opInfoMap->value(nextop);
  assert (src_info->is_scheduled());
  int src_sched_time = nextop->sched_time();
  for (Op_outedges_rel ei(nextop, eoutfilter_rep); ei != 0; ei++) {
    Op* dest_op = real_op((*ei)->dest());
    OpInfo* dest_info = opInfoMap->value(dest_op);
    dest_info->decr_unsch_inedges();
    if (dest_info->is_scheduled()) {
      continue;
    }
    int dest_etime = dest_info->get_etime();
    int new_etime = src_sched_time + (*ei)->latency();
    if (new_etime > dest_etime) dest_info->set_etime(new_etime);
    if (dest_info->get_unsch_inedges() <= 0) {
      assert (dest_info->get_unsch_inedges() == 0);
      double dyn_priority = calculate_dyn_priority (dest_op, dest_info);
      Stuple <double, Op*> pr_op (dyn_priority, dest_op);
      ready_slist.add (pr_op);
    }
  }

  // op_slist_print(ready_slist);

  if (ready_slist.is_empty()) {
    nextop = NULL;
  } else {
    nextop = (ready_slist.pop()).second;
  }
  return;
}

void Dep_pri_bt::reschedule(Op* op, Compound_region* reg)
{
  OpInfo* info = opInfoMap->value(op);
  if (!info->is_attempted())    info->set_attempted_time(op->sched_time() - 1);
  int unsch_inedges = 0;
  for (Op_inedges_rel ei(op, efilter_rep); ei != 0; ei++) {
    Op* src_op = real_op((*ei)->src());
    OpInfo* src_info = opInfoMap->value(src_op);
    if (!src_info->is_scheduled())      unsch_inedges++;
  }
  if (unsch_inedges > 0) {
    info->set_unsch_inedges(unsch_inedges);
  } else {
    Stuple<double, Op*> item;
    item.first = info->get_priority();
    item.second = op;

    if (dbg (ss, 7)) assert (!ready_slist.is_member(item));

    ready_slist.add(item);
  }
  for (Op_outedges_rel eo (op, eoutfilter_rep); eo != 0; eo++) {
    Op* dest_op = real_op((*eo)->dest());
    OpInfo* dest_info = opInfoMap->value(dest_op);
    if ((dest_info->is_scheduled()) || (dest_op == nextop)) continue;
    int dest_unsch_inedges = dest_info->get_unsch_inedges();
    Stuple<double, Op*> item;
    item.first = dest_info->get_priority();
    item.second = dest_op;
    if (ready_slist.is_member(item)) {
      assert (dest_unsch_inedges == 0);
      ready_slist.remove(item);
    }
    dest_unsch_inedges++;
    dest_info->set_unsch_inedges(dest_unsch_inedges);
  }
}

// Pri_bt delivers operations in strict static priority order
// from currently unscheduled operations.
// Backtracking (BT) permitted; scheduled operations can be unscheduled
// and put back in the priority structure for rescheduling

Pri_bt::Pri_bt(Compound_region* region, const Relax_dep_constr* relax_dep, 
   const Op_filter* ofilter, const Edge_filter* efilter,
   const Edge_filter* eoutfilter, bool delete_filter)
    : Priority(region, relax_dep, ofilter, efilter, delete_filter),
      eoutfilter_rep(eoutfilter)    	
{}

Pri_bt::~Pri_bt() {
  if(delete_rep) {
    delete (Edge_filter*) eoutfilter_rep;
  }
}

bool Pri_bt::is_pri_bt() const { return true; }

bool Pri_bt::operator==(const void* x) const {
  return (x == nextop);
}

bool Pri_bt::operator!=(const void* x) const {
  return !operator==(x);
}

void Pri_bt::initialize()
{
  Stuple<double, Op*> item;

  unsched_slist.clear();
  Region_ops_C0_order iter(reg);
  Op* cm_op = (*iter);
  assert (is_control_merge (cm_op));
  iter++;

  for(; iter != 0; ++iter) {
    Op* op = (*iter);
    OpInfo* info = opInfoMap->value(op);
    assert (!info->is_scheduled());
    item.first = info->get_priority();
    item.second = op;
    unsched_slist.add(item);
  }
  nextop = cm_op;
}

void Pri_bt::operator++() {
  if (nextop == NULL) {
    assert (unsched_slist.is_empty());
    return;
  }
  OpInfo* info = opInfoMap->value(nextop);
  assert (info->is_scheduled());
  if (unsched_slist.is_empty()) {
    nextop = NULL;
  } else {
    nextop = (unsched_slist.pop()).second;
  }
  return;
}


Op* Pri_bt::operator*() {
   return nextop;
}

void Pri_bt::reschedule(Op* op, Compound_region* reg)
{
  Stuple<double, Op*> item;
  OpInfo* info = opInfoMap->value(op);
  if (!info->is_attempted())    info->set_attempted_time(op->sched_time() - 1);
  item.first = info->get_priority();
  item.second = op;
  unsched_slist.add(item);
}

/* This priority function considers slack into account. It prioritizes 
   operations according to their corresponding slack values. Operations 
   with lesser slack values are given more priority. 
*/

Slack::Slack(Compound_region* region, const Relax_dep_constr* relax_dep, 
  const Op_filter* ofilter, const Edge_filter* efilter, bool delete_filter)
    : Priority(region, relax_dep, ofilter, efilter, delete_filter)
{}

Slack::~Slack() {}

bool Slack::is_slack() const { return true; }

bool Slack::operator==(const void* x) const {
  return (x == nextop);
}

bool Slack::operator!=(const void* x) const {
  return !operator==(x);
}

void Slack::operator++() {
  get_next_ready_op();
}

Op* Slack::operator*() {
   return nextop;
}

void Slack::get_next_ready_op()
{
  if(oplist.is_empty()) { nextop = NULL; return; }
  nextop = get_next_ready_node(ready_list, ofilter_rep, MT_SLACK);
  ready_list.remove( nextop);
  oplist.remove(nextop);
  ready_list = add_ready_nodes(nextop, ready_list, reg);
}

void Slack::reschedule(Op* op, Compound_region* reg)
{
  //  OpInfo* info = opInfoMap->value(op);
  //   calculate_slack(reg, relax_rep);
}

void Slack::initialize()
{
  time = 0;
  ready_list.clear(); oplist.clear();
  create_op_list(reg, ofilter_rep);
  while(ready_list.is_empty()) 
    get_ready_list(oplist, ofilter_rep, time++);
  nextop = get_next_ready_node(ready_list, ofilter_rep, MT_SLACK);
  ready_list.remove( nextop);
  oplist.remove(nextop);
  ready_list = add_ready_nodes(nextop, ready_list, reg);
}

/* This priority function is a topological walk of the graph present in
   "region". Among operations at the same height, they are selected in any
   random order. all operations at current height are traversed before 
   operations at the next lower height are considered.
*/


Topo::Topo(Compound_region* region, const Relax_dep_constr* relax_dep, 
  const Op_filter* ofilter, const Edge_filter* efilter, bool delete_filter)
  :Priority(region, relax_dep, ofilter, efilter, delete_filter) {

  /* list of all head nodes to begin with */
/*
	Doesn't compile with updated interface;
	(*iter)->dest() needs a Port num argument
	Need to iterate through all dests of incoming edges and
	add to ready_list only those that are in region -- SGA 2/95
  for(Region_all_inedges iter(reg); iter != 0; ++iter)
  {
     Op* dest = new Op;
     dest = (*iter)->dest();
     ready_list.add_tail(dest);
  }
*/
     
  nextop = get_next_ready_node(ready_list, ofilter_rep);
  if(nextop != NULL) {
    ready_list.remove( nextop);
    ready_list = add_ready_nodes(nextop, ready_list, reg);
  }
}

Topo::~Topo() {}

void Topo::initialize() {}

bool Topo::is_topo() const { return true; }

bool Topo::operator==(const void* x) const {
  return (x == nextop);
}

bool Topo::operator!=(const void* x) const {
  return !operator==(x);
}

void Topo::operator++() {
 forward();
}

Op* Topo::operator*() {
 return nextop;
}

   void Topo::reschedule(Op* op, Compound_region* reg) {}

void Topo::forward() 
{
 nextop = get_next_ready_node(ready_list, ofilter_rep);
 if(nextop != NULL) {
   ready_list.remove( nextop);
   ready_list = add_ready_nodes(nextop, ready_list, reg);
 }
}
