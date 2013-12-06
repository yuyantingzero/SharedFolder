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
//      File:           rr_life.cpp
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    lifetime analysis class impementation
//
/////////////////////////////////////////////////////////////////////////////

#include "map.h"
#include "el_error.h"
#include "attributes.h"
#include "region_attributes.h"
#include "el_port.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "dbg.h"

#include "rr_allocation.h"
#include "rr_file.h"
#include "rr_life.h"
#include "rr_util.h"

////////////////////////////////////////////////////
//  RR_ref_data class
////////////////////////////////////////////////////

ostream& operator<< (ostream& os, RR_ref_data& a)		// output
{
  os << "sched_time(" << a.sched_time << ")";
  os << " issue_time(" << a.issue_time << ")";
  os << " flow_time(" << a.flow_time << ")";
  os << " anti_time(" << a.anti_time << ")";
  os << " evr(" << a.evr <<")";
  os << " iter apart(" << a.iter_apart << ")";
  if (a.mc_loc != undefined) os << " mc loc(" << a.mc_loc << ")";
  if (a.ref.pt == SRC) {
    // use
    if (dbg(ra, 5)) os << " reaching_def(" << a.reaching_def << ")";
  } else {
    // def
    if (dbg(ra, 5)) os << " predicated_def(" << a.predicated_def << ")";
  }
  return os;
}

void RR_ref_data::Init()
{
  sched_time = issue_time = anti_time = flow_time = 0;
  evr = 0;

  ref = Reference();
  ref_dir = IN;
  operand_ptr = 0;
  predicated_def = false;
  reaching_def = Hash_set<int>(hash_int, 16);

  iter_apart = mc_loc = undefined;
}

RR_ref_data::RR_ref_data() : reaching_def(hash_int)
{
  Init();
}

RR_ref_data::RR_ref_data(Op* op, Operand* operand, Port_type type, Port_num num, Port_dir dir)
   : reaching_def(hash_int)
{
  Init();

  // evr information
  evr = operand->omega();
  if (is_brf(op) || is_brw(op)) {
    // special handling of BRTOP occurs here
    if (type == DEST) {
      evr--;
    }
  }

  // determine if the def is potentially unconiditional (reset later if needed)

  // RCJ -- this may be wrong for unconditional cmpp.  What is this for?
  predicated_def = false;
  if (op->predicated() && type == DEST) {
    predicated_def = true;
  }

  ref = Reference(op, type, num);
  ref_dir = dir;
  operand_ptr = operand;

  // results
  iter_apart=mc_loc=undefined; 
}

RR_ref_data::RR_ref_data(const RR_ref_data& a)
 : sched_time(a.sched_time), issue_time(a.issue_time), 
   flow_time(a.flow_time), anti_time(a.anti_time),
   evr(a.evr), 
   ref(a.ref), ref_dir(a.ref_dir), operand_ptr(a.operand_ptr), 
   predicated_def(a.predicated_def),
   reaching_def(a.reaching_def),
   iter_apart(a.iter_apart), mc_loc(a.mc_loc) {} 

RR_ref_data::~RR_ref_data() {}

RR_ref_data& RR_ref_data::operator= (const RR_ref_data& a) 
{
  if (this == &a) return *this;
  sched_time = a.sched_time; issue_time = a.issue_time; 
  flow_time = a.flow_time; anti_time = a.anti_time;
  evr = a.evr;
  ref = a.ref;
  ref_dir = a.ref_dir;
  operand_ptr = a.operand_ptr;
  predicated_def = a.predicated_def;
  reaching_def = a.reaching_def;
  iter_apart = a.iter_apart;
  mc_loc = a.mc_loc;
  return *this;
}

bool RR_ref_data::operator== (const RR_ref_data& a) const
{ return (sched_time == a.sched_time && 
    issue_time == a.issue_time && flow_time == a.flow_time && 
    anti_time == a.anti_time &&
    evr == a.evr && 
    ref == a.ref && ref_dir == a.ref_dir && operand_ptr == a.operand_ptr &&
    iter_apart == a.iter_apart && 
    predicated_def ==  a.predicated_def &&
    reaching_def == a.reaching_def &&
    mc_loc == a.mc_loc ); 
}

bool RR_ref_data::operator!= (const RR_ref_data& a) const
{
  return !(operator==(a));
}

void RR_ref_data::Init_times(const int II)
{
  sched_time = ref.o->sched_time();
  if (sched_time > 1000) {
    El_warn("RR: large scheduling times (%d)", sched_time);
  }

  if (is_swp_branch(ref.o)) {
    // special handling of BRTOP occurs here
  }

  issue_time = sched_time;
  flow_time = sched_time + ref.o->flow_time(ref.pt, ref.pn, ref_dir);
  anti_time = sched_time + ref.o->anti_time(ref.pt, ref.pn, ref_dir);

  // results
  iter_apart=mc_loc=undefined; 
}

void RR_ref_data::Init_open_loop_times()
{
  sched_time=issue_time=anti_time=0;
  flow_time=-1;
  // to see a wand of one wide, for debugging purpose, we define a wand of 1 (flow time of 0)
  if (dbg(ra, 8)) flow_time = 0;
}

void RR_ref_data::Commit(const int II, const int location, const int reg_num)
{
  int delta_stage;

  delta_stage = floor(issue_time, II);
  mc_loc = mod(location + delta_stage, reg_num);
  assert(operand_ptr);
  operand_ptr->bind_reg(mc_loc);
}

////////////////////////////////////////////////////
// RR_lifetime operations
////////////////////////////////////////////////////

ostream& operator<< (ostream& os, RR_lifetime& l)		// output
{  
  os << "RR_lifetime" <<
        " start(" << l.start_time << ")" <<
        " end(" << l.stop_time << ")" <<
        " omega(" << l.omega << ")" <<
        " alpha(" << l.alpha << ")" << 
        " first(" << l.first << ")" <<
        " k-first(" << l.k_first << ")" <<
        " last(" << l.last << ")"  <<
        " k-last(" << l.k_last << ")"  <<
        " leading_blade(" << l.leading_blade << ")" <<
        " trailing_blade(" << l.trailing_blade << ")" <<
        " all_predicated_defs(" << l.all_predicated_defs << ")" <<
        " base_evr(" << l.base_evr << ")";
  
  if (l.liveout_omega != -infinity) os << " liveout omega(" << l.liveout_omega << ")";
  if (l.allocated) os << " location(" << l.location << ")";
  if (l.defs.size() && l.defs[0].operand_ptr) {
    os << " def_operand: " << *l.defs[0].operand_ptr;
  }
  if (dbg(ra, 5)) {
    if (l.defs.size()) os << endl << "  def ops: " << l.defs;
    if (l.uses.size()) os << "  use ops: " << l.uses;
  }
  return os;
}

void RR_lifetime::Init() 
{
  defs = Vector<RR_ref_data>();
  uses = Vector<RR_ref_data>();
  defs_num=uses_num=undefined;
  liveout_omega=-infinity;
  base_evr=start_time=stop_time=undefined;
  alpha=omega=undefined;
  first=last=k_first=k_last=undefined;
  leading_blade=trailing_blade=false;
  all_predicated_defs=false;
  location = undefined;
  allocated = false;
}

RR_lifetime::RR_lifetime() 
{
  Init();
}

RR_lifetime::~RR_lifetime() {}	

RR_lifetime& RR_lifetime::operator= (const RR_lifetime& l)	
{
  if (this == &l) return *this;
  defs = l.defs;
  uses = l.uses;
  defs_num = l.defs_num;
  uses_num = l.uses_num;
  liveout_omega = l.liveout_omega;
  base_evr = l.base_evr;
  start_time = l.start_time;
  stop_time = l.stop_time;
  alpha = l.alpha;
  omega = l.omega;
  first = l.first;
  last = l.last;
  k_first = l.k_first;
  k_last = l.k_last;
  leading_blade = l.leading_blade;
  trailing_blade = l.trailing_blade;
  all_predicated_defs = l.all_predicated_defs;
  location = l.location;
  allocated = l.allocated;
  return (*this);
}
 
bool RR_lifetime::operator== (const RR_lifetime& l) const
{
  return (defs == l.defs && uses == l.uses &&
    defs_num == l.defs_num && uses_num == l.uses_num &&
    liveout_omega == l.liveout_omega && base_evr == l.base_evr &&
    start_time == l.start_time && stop_time == l.stop_time &&
    alpha == l.alpha && omega == l.omega &&
    first == l.first && last == l.last &&
    k_first == l.k_first && k_last == l.k_last &&
    leading_blade == l.leading_blade &&
    trailing_blade == l.trailing_blade &&
    all_predicated_defs == l.all_predicated_defs &&
    location == l.location && allocated == l.allocated);
}

bool RR_lifetime::operator!= (const RR_lifetime& l) const
{ 
  return (!operator== (l));
}

void RR_lifetime::Init_open_loop()
{
  Init();

  defs = Vector<RR_ref_data>(1);
  defs[0] = RR_ref_data();
  defs_num = 1;
  uses_num = 0;
  liveout_omega = -infinity;
}  


// omega: defined as the largest diff between the smallest omega of
// def opers and the largest omega of the use opers.

void RR_lifetime::Process_input_data(int II, int SC, int theta, int PS, int ES)
{
  int d, u;

  // check that rotating registers is defined at least once
  // necessary, not sufficient (defined under partial predicate is a problem)
  assert(defs_num > 0);

  //////////////////////////////////////////////////////////////////////
  // compute the min omega for defs; serves as base for all references
  //////////////////////////////////////////////////////////////////////

  int max_def_evr(-infinity), min_def_evr(infinity);
  for(d=0; d<defs_num; d++){
    // read time from ir
    defs[d].Init_times(II);

    min_def_evr = ELCOR_MIN(min_def_evr, defs[d].evr);
    max_def_evr = ELCOR_MAX(max_def_evr, defs[d].evr);
  }
  if (max_def_evr != min_def_evr) {
    // should tolerate several distinct evr for defs
    El_punt("lifetime cannot currently be defined under 2 differents evr\n");
  }
  base_evr = min_def_evr;
  
  // update times to be relative to the base_evr
  for(d=0; d<defs_num; d++){
    defs[d].iter_apart = defs[d].evr - base_evr; 
    defs[d].issue_time += defs[d].iter_apart * II;
    defs[d].flow_time += defs[d].iter_apart * II;
    defs[d].anti_time += defs[d].iter_apart * II;
  }

  for(u=0; u<uses_num; u++){
    // read time from ir
    uses[u].Init_times(II);

    uses[u].iter_apart = uses[u].evr - base_evr; 
    uses[u].issue_time += uses[u].iter_apart * II;
    uses[u].flow_time += uses[u].iter_apart * II;
    uses[u].anti_time += uses[u].iter_apart * II;
  }

  //////////////////////////////////////////////////////////////////////
  // compute the relevent min and max times
  //////////////////////////////////////////////////////////////////////

  int min_anti_def(infinity), max_flow_def(-infinity);
  int min_issue_def(infinity), max_issue_def(-infinity);
  int max_unpred_issue_def(-infinity);

  all_predicated_defs = true;
  for(d=0; d<defs_num; d++){
    min_anti_def = ELCOR_MIN(min_anti_def, defs[d].anti_time);
    max_flow_def = ELCOR_MAX(max_flow_def, defs[d].flow_time);
    min_issue_def = ELCOR_MIN(min_issue_def, defs[d].issue_time);
    max_issue_def = ELCOR_MAX(max_issue_def, defs[d].issue_time);
    if (!defs[d].predicated_def) {
      all_predicated_defs = false;
      max_unpred_issue_def = ELCOR_MAX(max_unpred_issue_def, defs[d].issue_time);
    }
  }

  int max_anti_use(-infinity);
  int min_issue_use(infinity), max_issue_use(-infinity);;
  int max_evr_use(-infinity);
  for(u=0; u<uses_num; u++){
    max_anti_use = ELCOR_MAX(max_anti_use, uses[u].anti_time);
    min_issue_use = ELCOR_MIN(min_issue_use, uses[u].issue_time);
    max_issue_use = ELCOR_MAX(max_issue_use, uses[u].issue_time);
    max_evr_use = ELCOR_MAX(max_evr_use, uses[u].evr);
  }

  int max_issue = ELCOR_MAX(max_issue_use, max_issue_def); 
  int min_issue = ELCOR_MIN(min_issue_use, min_issue_def);
  int max_evr = ELCOR_MAX(max_evr_use, max_def_evr);

  //////////////////////////////////////////////////////////////////////
  // compute start, stop, alpha, and omega
  //////////////////////////////////////////////////////////////////////

  // max_use +1 is the last instant of time where the reg is used
  start_time = min_anti_def;
  stop_time = ELCOR_MAX(max_anti_use, max_flow_def) +1;

  // alpha and omega relative to base evr 
  omega = ELCOR_MAX(0, max_evr - base_evr);
  alpha = ELCOR_MAX(0, liveout_omega - base_evr);

  //////////////////////////////////////////////////////////////////////
  // compute first, last (as computed by Rau)
  //////////////////////////////////////////////////////////////////////

  // L term is set to an arbitrary value because all L terms cancels each others
  // could have eliminated the L term altogehter, but the formulas would 
  // not be as readable. This value must be consistant with the one in
  // RR_loop::Compute_distance
  int L = 1;

  if (all_predicated_defs) {
    first = - omega;
  } else {
    first = - floor(max_unpred_issue_def, II);
    if (first != 0 && omega != 0) {
      El_warn("RR: have an upredicated lifetime with live-in, which is erronerous");
    }
    first = ELCOR_MIN(first, -omega);
  }

  if (all_predicated_defs) {
    int TIPF = II * (theta + 1) + II * (L -1);
    last =  ceil(TIPF - min_issue_def, II) -1;
    last = ELCOR_MAX(last, L-1);
  } else {
    last = SC + L - 1;
  }

  //////////////////////////////////////////////////////////////////////
  // compute k_first, k_last (as computed by Rau)
  //////////////////////////////////////////////////////////////////////
  int FKS = PS;
  int LSC = SC + L - 1;
  int LKS = LSC - ES - 1;

  k_first = FKS - floor(max_issue, II);
  k_first = ELCOR_MAX(first, k_first);
  leading_blade = (k_first < 0) && (omega > 0);

  k_last = LKS - floor(min_issue, II);
  k_last = ELCOR_MIN(last, k_last);
  trailing_blade = (k_last >= L - alpha) && (alpha > 0);
}

void RR_lifetime::Process_open_loop_data(int II, int SC, int theta, int PS, int ES)
{
  defs[0].Init_open_loop_times();
  base_evr = 0;
  all_predicated_defs = false;

  //////////////////////////////////////////////////////////////////////
  // compute start, stop, alpha, and omega
  //////////////////////////////////////////////////////////////////////

  start_time = 0;
  stop_time = 0;
  omega = 0;
  alpha = 0;

  //////////////////////////////////////////////////////////////////////
  // compute first, last (as computed by Rau)
  //////////////////////////////////////////////////////////////////////
  // L term is set to an arbitrary value because all L terms cancels each others
  // could have eliminated the L term altogehter, but the formulas would 
  // not be as readable. This value must be consistant with the one in
  // RR_loop::Compute_distance
  int L = 1;

  first = 0;
  last = SC + L - 1;

  //////////////////////////////////////////////////////////////////////
  // compute k_first, k_last (as computed by Rau)
  //////////////////////////////////////////////////////////////////////
  int FKS = PS;
  int LSC = SC + L - 1;
  int LKS = LSC - ES - 1;

  k_first = FKS;
  leading_blade = false;

  // increase it by one because a remap will occur on the last BRTOP, eventhough its
  // outside the boundaries
  k_last = LKS +1;
  trailing_blade = false;;
}

/*
   "location" corresponds to the physical register in which the first
   def op possibly writes into at "start_time" time.

   The physical register is also incremented by the omega of the operand.
*/

void RR_lifetime::Commit(const int II, const int reg_num)
{
  for(int d=0; d<defs_num; d++){
    defs[d].Commit(II, location, reg_num);
  }
  
  for(int u=0; u<uses_num; u++){
    uses[u].Commit(II, location, reg_num);
  }
}

// end of rr_life.cpp
