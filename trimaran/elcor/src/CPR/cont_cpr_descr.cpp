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



/*===========================================================================
 *
 *      File:           cont_cpr_descr.cpp
 *      Author:         Mike Schlansker
 *      Created:        March 1997
 *      Description:    control cpr pattern match for HBs
 *
 *===========================================================================*/

#include "defs.h"
#include "vector.h"
#include "list.h"
#include "hash_functions.h"
#include "iterators.h"
#include <iostream>
#include "hash_set.h"
#include "dbg.h"
#include "cont_cpr_match.h"
#include "cont_cpr_descr.h"
#include "opcode_properties.h"
#include "attributes.h"
#include "region_utilities.h"
#include "edge_utilities.h"
#include "el_cpr_init.h"
#include "pred_interface.h"

#define EL_CPR_CONST_BLOCKING_FACTOR	El_cpr_const_blocking_factor
#define EL_CPR_MAX_EXIT_RATIO 		El_cpr_max_exit_ratio
#define EL_CPR_MAX_BLOCK_LENGTH		El_cpr_max_block_length



/*===========================================================================*/
/*
 *	Utility fncts
 */
/*===========================================================================*/

//static Operand pred_true=new Pred_lit(true);

unsigned hash_cmp_descr_ptr(Compare_descr*& i)
{
   return(ELCOR_INT_HASH(i)) ;
}

void print_set_op(ostream& os, Hash_set<Op*> set){
  for(Hash_set_iterator<Op*> iter(set); iter!=0; iter++)
    {
      os << (*iter)->id() << endl;
    }
}

Pair<Operand, Operand> identify_compare_targets(Op* cmp){
  // returns unconditional compare targets in result list
  Pair<Operand, Operand> result; // create undefined operand pair
  Opcode code=cmp->opcode();
  Opcode dest1_modifier = Opcode( code & (IR_DEST1_MASK) );
  Opcode dest2_modifier = Opcode( code & (IR_DEST2_MASK) );
  // record first target if unconditional and second if complementary
  if(dest1_modifier==(Opcode) IR_DEST1_UN)
    {
    result.first=cmp->dest(DEST1);
    if(dest2_modifier==(Opcode)IR_DEST2_UC) result.second=cmp->dest(DEST2);
    }
  if(dest1_modifier==(Opcode) IR_DEST1_UC)
    {
    result.first=cmp->dest(DEST1);
    if(dest2_modifier==(Opcode)IR_DEST2_UN) result.second=cmp->dest(DEST2);
    }
  return result;
}


/*===========================================================================*/
/*
 *	Compare_descr methods
 */
/*===========================================================================*/


Compare_descr::Compare_descr(Op* op):
  link_to_compare(op),
  link_to_branch(NULL),
  link_to_guard_cmp(NULL)
{}

Compare_descr::~Compare_descr()
{}


/*===========================================================================*/
/*
 *	HB_cont_cpr_descr methods
 */
/*===========================================================================*/


HB_cont_cpr_descr::HB_cont_cpr_descr(Compound_region* hyper_blk):
  num_branches(0),
  live_compares(hash_operand,223),
  all_compares(),
  successors(hash_op_ptr,223),
  cmp_set(hash_cmp_descr_ptr,223),
  region_ptr(hyper_blk)
{
  int num_br, num_cmpp, num_elem;
  Op *op;

  /* Figure out how many compares/branches are in this HB */
  num_br = num_cmpp = 0;
  for (Region_ops_C0_order op_i(hyper_blk); op_i!=0; op_i++) {
    op = *op_i;
    if (is_control_switch(op))
      num_br++;
    else if (is_cmpp(op))
      num_cmpp++;
  }

  num_elem = (ELCOR_MAX(num_br, num_cmpp)) + 1;
  branch_vect.resize(num_elem);
  branch_vect.assign(branch_vect.size(), 0);
  compare_vect.resize(num_elem);
  compare_vect.assign(compare_vect.size(), NULL);
  weight_vect.resize(num_elem);
  weight_vect.assign(weight_vect.size(), 0.0);
  incoming_weight = -1.0;
  first_branch_index = -1;
// insert fictitious compare descr generating true operand
  pred_true=new Pred_lit(true);
  true_compare_descr = new Compare_descr(NULL);
  true_compare_descr->link_to_guard_cmp=true_compare_descr; 
  live_compares.bind(pred_true, true_compare_descr);
  all_compares.add_head(true_compare_descr);
}
 
HB_cont_cpr_descr::~HB_cont_cpr_descr(){
   clear();
}

void HB_cont_cpr_descr::clear(){
  for(int i=0; i<=num_branches-1; i++){
    branch_vect[i] = NULL;
    compare_vect[i] = NULL;
    weight_vect[i] = 0.0;
  }
  for(List_iterator<Compare_descr*> iter(all_compares); iter!=0; iter++)
    delete *iter;
  num_branches=0;
  all_compares.clear();
  live_compares.clear();
  successors.clear();
  cmp_set.clear();
  incoming_weight = 0.0;
}
 
void HB_cont_cpr_descr::insert_branch(Op* branch_op){
  Operand cond_pred; // construct null operand as default guard;
  if(is_brct(branch_op)){  
    cond_pred = branch_op->src(SRC2); //src2 input is condition for brct
  }
  if(is_bru(branch_op)) {cond_pred=pred_true;}
  //true serves as cond. for unconditonal BRU
  num_branches++;
  branch_vect[num_branches-1] = branch_op;
  compare_vect[num_branches-1] = NULL; 
  //no corresponding compare until identified
  //find compare which reaches this branch and install it into compare list
  if(live_compares.is_bound(cond_pred))
    {
      Compare_descr* corr = live_compares.value(cond_pred);
      compare_vect[num_branches-1] = corr;
      corr->link_to_branch=branch_op;
      //cout<<"corresponding compare: "<<corr->link_to_compare->id() << endl;
    }

  // Set the appropriate field in the weight_vect
  if (is_proper_branch(branch_op)) {
    Edge *edge = get_outgoing_CONTROL0_non_fall_through_edge(branch_op);
    Control_flow_freq *cfreq = get_control_flow_freq(edge);
    if (cfreq == NULL)
      El_punt("HB_cont_cpr_descr::insert_branch: missing control flow freq - op %d",
		branch_op->id());
    weight_vect[num_branches-1] = cfreq->freq;
  }
  else {
    weight_vect[num_branches-1] = 0.0;
  }
}
  
void HB_cont_cpr_descr::insert_compare(Op* cmp){
  Operand guard_pred=cmp->src(PRED1);
  Compare_descr* guard_cmp_descr=NULL; 
  Compare_descr* new_compare_descr=new Compare_descr(cmp); //create compare descr
  all_compares.add_head(new_compare_descr); //add to list of allocated compare descrs
  if(live_compares.is_bound(guard_pred)){ // cmp guarded by a visible compare
    // cout << "bound in live_compares: " << pred << endl;
    guard_cmp_descr=live_compares.value(guard_pred);
    //get guarding descr
    new_compare_descr->link_to_guard_cmp=guard_cmp_descr;
    //cout << " final branch: " << (branch_vect[num_branches-1])->id() << endl;
    //cout << "guarding compare descr: " << guard_cmp_descr << endl;
    //Op* prev_branch = guard_cmp_descr->link_to_branch; //corresponding branch
    //cout << "branch reached: " << branch_reached << endl;
    //if(branch_reached) cout << " id: " << branch_reached->id() << endl;
  }
  else {
    //create fictitious guarding compare descriptor for unknown compare
    guard_cmp_descr =new Compare_descr((Op*) NULL);
    all_compares.add_head(guard_cmp_descr); //add to list of allocated compare descrs
    new_compare_descr->link_to_guard_cmp=guard_cmp_descr;
    live_compares.bind(guard_pred, guard_cmp_descr);
  }
  Pair<Operand, Operand> targets = identify_compare_targets(cmp);
  Operand& t1=targets.first; Operand& t2=targets.second;
  // invalidate any previous target bindings
  if(live_compares.is_bound(t1)) live_compares.unbind(t1);
  if(live_compares.is_bound(t2)) live_compares.unbind(t2);
  if(!t1.is_undefined()) live_compares.bind(t1,new_compare_descr);
  if(!t2.is_undefined()) live_compares.bind(t2,new_compare_descr);
}

void HB_cont_cpr_descr::append_successors(int org_branch)
{
  // compute successor list
  Filter<Edge*>* efilter  = new Filter_not <Edge*> (CONTROL0_OUTEDGES,  false);
  Compare_descr* cmp_descr=compare_vect[org_branch-1];
  //compute successors if corresponding compare for a branch is defined
  // and branch not final branch in hyperblock
  if(cmp_descr && (org_branch < num_branches) ){
    Op* compare_op=cmp_descr->link_to_compare;
    Compare_descr* next_compare_descr=compare_vect[org_branch];
    Edge* curr_out_edge=NULL;
    Op* next_compare=NULL;
    if(next_compare_descr){
      next_compare=next_compare_descr->link_to_compare;
      //identify edge to remove
      if(dbg(cpr,5)){
	cout << "testing for need to remove edge" << endl;
	cout << "source compare" << compare_op->id() << endl;
	if (next_compare != NULL)
	  cout << "destination compare" << next_compare->id() << endl;
	else
	  cout << "destination compare NULL" << endl;
      }
      //should be generalized to delete edges to all suitable
      //compares which control subsequent branches 
      bool deleted_edge=false;
      for(Op_outedges oe_iter(compare_op, efilter); oe_iter!=0; oe_iter++)
	{
	  curr_out_edge=*oe_iter;
	  if(dbg(cpr,5)){
	    cout << "tested_edge: " << curr_out_edge << endl;
	  }
	  Op* dest= curr_out_edge->dest();
	  if(dest==next_compare){
	    if(dbg(cpr,5)){
	      cout << "removing edge: " << *curr_out_edge << endl;
	    }
	    deleted_edge=true;
	    compare_op->remove_outedge(curr_out_edge, DEST, DEST1);
	    next_compare->remove_inedge(curr_out_edge, SRC, PRED1);
	    break;
	  }
	}
      // compute descendents
      for (Op_descendants succ(compare_op, region_ptr, efilter); succ!=0; succ++)
	{
	  successors+=*succ;
	}
      if(dbg(cpr,5)){
	cout << "computed new successor set" << endl;
	print_set_op(cout, successors);
      }
      // repair graph
      if(deleted_edge){
         compare_op->add_outedge(curr_out_edge, DEST, DEST1);
         next_compare->add_inedge(curr_out_edge, SRC, PRED1);
      }
    }
  }
}

void HB_cont_cpr_descr::suitability_init(int br){
  cmp_set.clear();
  Compare_descr* branch_guard_cmp=compare_vect[br-1]; //zero origin index
  if(!branch_guard_cmp) return;
  //can't find guarding compare, cmp_set empty & cpr block terminates
  Compare_descr* cmp_guarding_cmp = branch_guard_cmp->link_to_guard_cmp;
  cmp_set += branch_guard_cmp;
  cmp_set += cmp_guarding_cmp; 
  cmp_set += true_compare_descr; // true is always a suitable guard
}


bool HB_cont_cpr_descr::suitability_test(int br){
  Op* branch_ptr = branch_vect[br-1];
  if(is_brl(branch_ptr)) return false; //brl never appended to existing block
  Op* prev_branch_ptr = branch_vect[br-2];
  if(is_brl(prev_branch_ptr)) return false; //never append to brl singleton block
  Compare_descr* branch_guard_cmp = compare_vect[br-1]; //zero origin index
  if(!branch_guard_cmp) return false;  //can't find guarding compare

  Compare_descr* controlling_compare = branch_guard_cmp->link_to_guard_cmp;
  if(!cmp_set.is_member(controlling_compare)) return false;
  cmp_set += branch_guard_cmp;
  return true;
  //the suitability test can be generalized to include any case where a compares
  //guarding predicate can be shown to dominate the compares execution condition
  //in the original CFG
}

void HB_cont_cpr_descr::separability_init(int branch){
  successors.clear();
  append_successors(branch);
}

bool HB_cont_cpr_descr::separability_test(int branch){
  // following special case needed for unconditional final loop-back jump
  if(is_bru(branch_vect[branch-1])) return true;

  Compare_descr* cmp=compare_vect[branch-1];

  if(cmp == NULL) return false; //brl or unknown predicate reaching brc(t/f)
  if(successors.is_member(cmp->link_to_compare)) return false;//fails separability
  append_successors(branch);
  return true;
}


Pair<Op*, Op*> HB_cont_cpr_descr::get_tuple(int br){
  Pair<Op*, Op*> result_tuple;
  result_tuple.first=branch_vect[br-1];
  Compare_descr* descr=compare_vect[br-1];
  Op* corresponding_compare=NULL;
  if(descr) corresponding_compare=descr->link_to_compare;
  result_tuple.second=corresponding_compare;
  return result_tuple;
}

bool HB_cont_cpr_descr::predict_taken(int br){
  Op *exit_branch_op;
  Edge  *exit_edge, *ft_edge;
  Control_flow_freq *exit_freq_attr;
  double exit_freq;
  exit_branch_op=branch_vect[br-1]; //zero origin index
  if (is_brl(exit_branch_op))  return (false);
  exit_edge = get_outgoing_CONTROL0_non_fall_through_edge(exit_branch_op);
  exit_freq_attr = get_control_flow_freq(exit_edge);
  if (exit_freq_attr == NULL)
	El_punt("HB_cont_cpr_descr::predict_taken: missing exit freq on branch %d",
			exit_branch_op->id());
  exit_freq=exit_freq_attr->freq;
  if((exit_freq / incoming_weight) >= El_cpr_predict_taken_ratio) return true;

  // SAM 11/2005, if ends in a branch with no fallthru edge, also consider it a predict taken
  ft_edge = get_outgoing_CONTROL0_fall_through_edge(exit_branch_op);
  if (ft_edge == NULL) {
    cdbg << "******************returning true for branch " << exit_branch_op->id() << endl;
    return true;
  }
  return false;
}

bool HB_cont_cpr_descr::predict_break(int br){
  Op *exit_branch_op;
  Edge  *exit_edge;
  Control_flow_freq *exit_freq_attr;
  double exit_freq;
  exit_branch_op=branch_vect[br-1]; //zero origin index
  if (is_brl(exit_branch_op))  return (false);
  exit_edge = get_outgoing_CONTROL0_non_fall_through_edge(exit_branch_op);
  exit_freq_attr = get_control_flow_freq(exit_edge);
  if (exit_freq_attr == NULL)
	El_punt("HB_cont_cpr_descr::predict_break: missing exit freq on branch %d",
			exit_branch_op->id());
  exit_freq=exit_freq_attr->freq;
  if((exit_freq / incoming_weight) >= El_cpr_predict_break_ratio) return true;
  return false;
}

bool HB_cont_cpr_descr::predict_pauseb(int br){
  Op *exit_branch_op;
  Edge  *exit_edge;
  Control_flow_freq *exit_freq_attr;
  double exit_freq;
  exit_branch_op=branch_vect[br-1]; //zero origin index
  if (is_brl(exit_branch_op))  return (false);
  exit_edge = get_outgoing_CONTROL0_non_fall_through_edge(exit_branch_op);
  exit_freq_attr = get_control_flow_freq(exit_edge);
  if (exit_freq_attr == NULL)
	El_punt("HB_cont_cpr_descr::predict_pauseb: missing exit freq on branch %d",
			exit_branch_op->id());
  exit_freq=exit_freq_attr->freq;
  if((exit_freq / incoming_weight) >= El_cpr_predict_pauseb_ratio) return true;
  return false;
}

bool HB_cont_cpr_descr::predict_pausea(int br){
  Op *exit_branch_op;
  Edge  *exit_edge;
  Control_flow_freq *exit_freq_attr;
  double exit_freq;
  exit_branch_op=branch_vect[br-1]; //zero origin index
  if (is_brl(exit_branch_op))  return (false);
  exit_edge = get_outgoing_CONTROL0_non_fall_through_edge(exit_branch_op);
  exit_freq_attr = get_control_flow_freq(exit_edge);
  if (exit_freq_attr == NULL)
	El_punt("HB_cont_cpr_descr::predict_pausea: missing exit freq on branch %d",
			exit_branch_op->id());
  exit_freq=exit_freq_attr->freq;
  if((exit_freq / incoming_weight) >= El_cpr_predict_pausea_ratio) return true;
  return false;
}

bool HB_cont_cpr_descr::can_move_back(int beg_br, int end_br) {
  Op* first_op, *last_op;

  // get first op of current block

  int last_br = beg_br-1;  // index of last branch in previous block
  if (last_br == 0) { // no previous block; use control merge at top of region
    Region_entry_ops entry(region_ptr);
    first_op = *entry;
    assert(is_control_merge(first_op));
  } else {
    first_op = branch_vect[beg_br-1];
  }
  last_op = branch_vect[end_br-1];

  // get pqs, get predicate corresponding to block taken exit
  Pred_jar pj(region_ptr);
  Pred_cookie pt = pj.get_lub_taken_branch_guard(last_op);

  // main test:
  //   consider all ops in region that will move off trace
  //   - for each of these ops, verify that the guarding predicate
  //     dominates the region's fall-through predicate (which is the
  //     same as the end branch's fall-through predicate)

  for (Region_ops_C0_order ri(region_ptr, first_op); ri != 0; ri++) {
    Op* curr_op = *ri;

    if (curr_op == last_op) break;
    if (is_pseudo(curr_op)) continue;

    // continue if current op not in code motion opset
    if (!successors.is_member(curr_op)) {
      continue;
    }

    Pred_cookie guard = pj.get_lub_src_expression_guard(curr_op) ;
    if (!pt.is_subset(guard)) {
      return false;
    }
  }

  return true;
}



/*===========================================================================*/
/*
 *	HB_cont_cpr_descr methods related to blocking cpr blocks
 */
/*===========================================================================*/

void HB_cont_cpr_descr::blocking_init(int br)
{
  int i;
  double wgt;

  /* Figure out the incoming weight of the CPR block starting with br */
  wgt = region_ptr->weight;
  for (i=0; i<=(br-2); i++) {
    wgt -= weight_vect[i];
  }
  if (wgt < 0.0)
    wgt = 0.0;

  incoming_weight = wgt;

  first_branch_index = br-1;
}

/*
 *	These should return false if blocking says to terminate the cpr block,
 *	true if blocking says to continue the cpr block.
 */

bool HB_cont_cpr_descr::sum_exit_freq_test(int br)
{
  int i;
  double cum_exit_weight, taken_weight, ft_weight, cur_weight,
	off_trace_ratio;

  if (region_ptr->weight <= 0.0)
    return (true);
  if (incoming_weight <= 0.0)
    return (true);

  cum_exit_weight = 0.0;
  cur_weight = incoming_weight;
  // Assume the exit weight is the smaller of the taken/ft weight of each
  // branch, meaning that we assume CPR is occuring on the preferred direction
  // of each branch.
  for (i=first_branch_index; i<=(br-1); i++) {
    taken_weight = weight_vect[i];
    ft_weight = cur_weight - taken_weight;
    cum_exit_weight += taken_weight;
    cur_weight -= taken_weight;
  }

  off_trace_ratio = cum_exit_weight / incoming_weight;
 
  if (off_trace_ratio > EL_CPR_MAX_EXIT_RATIO)
    return (false);
  else
    return (true);
}

bool HB_cont_cpr_descr::max_block_length_test(int br)
{
  int num_br;

  num_br = (br - 1) - first_branch_index + 1;
  if ((EL_CPR_MAX_BLOCK_LENGTH != -1) && (num_br > EL_CPR_MAX_BLOCK_LENGTH))
    return (false);
  else
    return (true);
}
