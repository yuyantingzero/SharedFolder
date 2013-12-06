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
//      File:           rr_file.cpp
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    class implementation for register allocation per file 
//
/////////////////////////////////////////////////////////////////////////////

#include "map.h"
#include "el_error.h"
#include "attributes.h"
#include "region_attributes.h"
#include "el_port.h"
#include "operand.h"
#include "iterators.h"
#include "flow_analysis_solver.h"
#include "opcode_properties.h"
#include "dbg.h"

#include "rr_allocation.h"
#include "rr_file.h"
#include "rr_life.h"
#include "rr_util.h"

//static eString rr_reg_file_to_text (Reg_file file_type) ;

unsigned hash_rr_ref_data(RR_ref_data& i)
{
   return hash_operand_ptr(i.operand_ptr);
}

unsigned hash_rr_ref_data_ptr(RR_ref_data*& i)
{
   return ELCOR_INT_HASH(i);
}

////////////////////////////////////////////////////
// RR_file_data data
////////////////////////////////////////////////////

ostream& operator<< (ostream& os, RR_file_data& l)		// output
{
  if (dbg(ra, 4)) {
    os << DEBUG_TITLE2("Register Data-structure for ", l.phys_file)
       << " II(" << l.II << ")"
       << " stage_count(" << l.stage_count << ")"
       << " theta(" << l.theta << ")"
       << " life_num(" << l.life_num << ")" << endl;
    os << " Shedule par: PS(" << l.PS << ") ES(" << l.ES << ")"
       << " LPS(" << l.LPS << ") LES(" << l.LES << ")"
       << " wand only(" << l.wand_only << ")"
       << " open_loop(" << l.open_loop << ")"
       << " predicated allowed(" << l.predicated_allowed << ")"
       << " reg_file_to_alloc(" << l.phys_file << ")" << endl;
    if (l.reg_num == 0) {
      os << " Register requirement: none" << endl;
      return os;
    }
    os << " Register requirement: " << l.reg_num 
       << " rotate_alloc(" <<  l.rotate_alloc << ")" << endl;
    if (l.reg_state) os << " Register Allocation State : " << *(l.reg_state) << endl;
    os << " Used life: " << endl << l.life << endl;
  }
  if (dbg(ra, 7)) {
    os << " Dist: " << endl << l.dist;
  }
  if (l.allocated) {
    int reg_num = (l.commit_reg_num == undefined) ? l.reg_num : l.commit_reg_num;
    int length = reg_num * l.stage_count;
    int iter = reg_num / 4;
    if (length < 40) iter = 2*reg_num;
    else if (length < 70) iter = reg_num;
    else if (length < 140) iter = reg_num / 2;
    RR_space_time sp(&l, reg_num, iter);
    os << DEBUG_TITLE2("Register Allocation for ", l.phys_file) << sp;
  }
  return os;
}

// /* For converting register files to textual representation */
// static eString rr_reg_file_to_text (Reg_file file_type) {
//   switch (file_type) {
//     case GPR: return "Rotating GPR";
//     case FPR: return "Rotating FPR";
//     case PR:  return "Rotating PR";
//     case BTR: return "BTR";
//     case CR:  return "CR";
//     default: return "?";
//     }
//   return "unkown reg file";
// }

void RR_file_data::Init()
{
  II = stage_count = life_num = 0;
  life = Vector<RR_lifetime>();
  IR_kernel = 0;
  IR_loopbody = 0;
  wand_only = predicated_allowed = open_loop = false;
  PS = ES = theta = 0;
  phys_file = "";
  //  reg_file_to_alloc = GPR;
  //  reg_file_str = rr_reg_file_to_text(reg_file_to_alloc);
  commit_reg_num = undefined;

  allocated_lt =  List<int>();
  unallocated_lt =  List<int>();
  forbidden_loc = Vector< Hash_set<int> >();

  reg_state = soft_reg_state = 0;
  soft_alloc_last_sel = undefined;

  dist = Matrix<int>();
  conf = Matrix<int>();
  overlap_no_interference_loc = Matrix<Hash_set<int> >();
  vreg_to_lifetime_index = Map<int,int>();
  open_loop_lifetime = undefined;

  reg_num = 0;
  rotate_alloc = 0;
  LPS = LES = undefined;
  allocated = commited = false;
}

// shouldn't call this because stuff can't be initialized
RR_file_data::RR_file_data()		
{
  Init();
}

RR_file_data::RR_file_data(LoopBody* loopbody, char* pf_name, 
  const bool open_loop_model)
{
  // graph data
  Op 		*op;
  Operand 	*operand;

  // reg data  
  Map<int,int*> 	    reg_to_liveout;
  Map<int,Hash_set<RR_ref_data>*> reg_to_use_set;
  Map<int,Hash_set<RR_ref_data>*> reg_to_def_set;

  int* liveout;
  Hash_set<RR_ref_data>* use_set;
  Hash_set<RR_ref_data>* def_set;
  int i, j, k, l;

  /* RMR { set of operands that are defined in scalar-to-vector regfile moves */
  Hash_set<int> svmove_defs(hash_int, 211);
  /* } RMR */

  Init();
  IR_loopbody = loopbody;
  IR_kernel = loopbody->kernel();
  open_loop = open_loop_model;
  phys_file = eString(pf_name);
  //  reg_file_to_alloc = reg_file_selected;
  //  reg_file_str = rr_reg_file_to_text(reg_file_to_alloc);
  life_num = 0; 

  for(Region_all_ops it1(IR_kernel); it1!=0; it1++) {
    op = *it1;

    // skip unwanted ops
    if (is_pseudo(op)) continue;

    // for all source operands (including the predicate if any)
    for(i=(int) op->first_input(); i<=op->last_input(); i++) {
      operand = &op->src((Port_num) i);
      if (operand->is_reg() && operand->is_rotating() &&
	  operand->physical_file_type() == phys_file) {
	  //          operand->file_type() == reg_file_to_alloc) {
        // create data if not bound already
        if (!vreg_to_lifetime_index.is_bound(operand->vr_num())) {
          vreg_to_lifetime_index.bind(operand->vr_num(), life_num++);
          reg_to_liveout.bind(operand->vr_num(), new int(-infinity));
          reg_to_def_set.bind(operand->vr_num(), new Hash_set<RR_ref_data>(hash_rr_ref_data, 16));
          reg_to_use_set.bind(operand->vr_num(), new Hash_set<RR_ref_data>(hash_rr_ref_data, 16));
        }
        use_set = reg_to_use_set.value(operand->vr_num());
        *use_set += RR_ref_data(op, operand, SRC, (Port_num) i, IN);
      }
    }

    // for all dest operand 
    for(i=(int) op->first_dest(); i<=op->num_dests(); i++) {
      operand = &op->dest((Port_num) i);
      if (operand->is_reg() && operand->is_rotating() && 
	  operand->physical_file_type() == phys_file) {
	  //          operand->file_type() == reg_file_to_alloc) {
        // create data if not bound already
        if (!vreg_to_lifetime_index.is_bound(operand->vr_num())) {
          vreg_to_lifetime_index.bind(operand->vr_num(), life_num++);
          reg_to_liveout.bind(operand->vr_num(), new int(-infinity));
          reg_to_def_set.bind(operand->vr_num(), new Hash_set<RR_ref_data>(hash_rr_ref_data, 16));
          reg_to_use_set.bind(operand->vr_num(), new Hash_set<RR_ref_data>(hash_rr_ref_data, 16));
        }
        def_set = reg_to_def_set.value(operand->vr_num());
	  /* RMR { track all vector operands defined by scalar-to-vector regfile moves */
	  if (is_svmove(op)) {
	    if (!svmove_defs.is_member(operand->vr_num())) 
		svmove_defs += operand->vr_num();
	  }
	  /* } RMR */
        *def_set += RR_ref_data(op, operand, DEST, (Port_num) i, OUT); 
      }
    }
  } // end all operations of the region

  if (life_num == 0) {
    // no lifetime in rotating registers -> done
    if (dbg(ra, 2)) 
      cdbg << "no rotating registers in " << phys_file << endl;
    reg_num = 0;
    allocated = commited = true;
    return;
  }

  // get the liveouts operand , and record the highest omega 
  Hash_set<Operand> live_out(hash_operand, 16);
  for(Region_exit_edges ii1(IR_loopbody); ii1 != 0; ii1++) {
    Edge* curr_edge = *ii1;
    Liveness_info* curr_live = get_liveness_info(curr_edge);
    for (Liveness_info_iterator li2(*curr_live) ; li2 != 0; li2++) {
       live_out += *li2;
    }
  }
  if (dbg(ra, 4)) cdbg << "live out " << live_out;
     
  for(Hash_set_iterator<Operand> ii2(live_out); ii2 != 0; ii2++) {
    Operand curr_operand = *ii2;
 
    if (curr_operand.is_reg() && 
        reg_to_liveout.is_bound(curr_operand.vr_num())) {
        // SAM 8-03, I believe the above check subsumes all 3 checks below, as
        // reg_to_liveout is not updated unless the physical file matches
        // and the vreg_to_lifetime_index is updated at teh same time
	//        curr_operand.file_type() == reg_file_to_alloc &&
	// curr_operand.physical_file_type() == phys_file &&
        // vreg_to_lifetime_index.is_bound(curr_operand.vr_num())) {
      // find data associated with this vr (should have been created earlier)
      liveout = reg_to_liveout.value(curr_operand.vr_num());
      *liveout = ELCOR_MAX(*liveout, curr_operand.omega());
      if (dbg(ra, 6)) cdbg << "  new live out value " << *liveout 
        << " for operand " << curr_operand << endl;
    }
  }

  // init data 
  life = Vector<RR_lifetime>(life_num);

  for(Map_domain_iterator<int, int> it(vreg_to_lifetime_index); it!=0; it++){
	int vreg = *it ;
    i = vreg_to_lifetime_index.value(vreg);

    // copy RR_lifetime info; (-infinity if undefined)
    liveout = reg_to_liveout.value(vreg);
    life[i].liveout_omega = *liveout;
    delete liveout;

    // copy RR_lifetime def_set info
    def_set = reg_to_def_set.value(vreg);
    life[i].defs_num = def_set->size();
    life[i].defs = Vector<RR_ref_data> (life[i].defs_num);
    j=0;
    for(Hash_set_iterator<RR_ref_data> d(*def_set); d != 0; d++){
      life[i].defs[j++] = *d;
    }
    delete def_set;

    /* RMR { this lifetime is defined by a scalar-to-vector regfile move
     * and must be treated as N distinct definitions where N is the vector width
     */
    if (svmove_defs.is_member(vreg)) {
	life[i].svmove_def = true;
    }
    /* } RMR */

    // copy RR_lifetime use_set info
    use_set = reg_to_use_set.value(vreg);
    life[i].uses_num = use_set->size();
    life[i].uses = Vector<RR_ref_data> (life[i].uses_num);
    j=0;
    for(Hash_set_iterator<RR_ref_data> u(*use_set); u != 0; u++){
      life[i].uses[j++] = *u;
    }
    delete use_set;

    // update reachind def information
    for(k=0; k<life[i].uses_num; k++) {
      life[i].uses[k].reaching_def.clear();

      // assume that all defs reaches all uses
      for(l=0; l<life[i].defs_num; l++){
        life[i].uses[k].reaching_def += l;
      }
    }
  }

  if (open_loop) {
    // create additional empty lifetime
    open_loop_lifetime = life_num++;
    life.resize(life_num);
    life[open_loop_lifetime].Init_open_loop();
  }

  if (dbg(ra, 6)) {
    cdbg << DEBUG_TITLE2("INPUT (unprocessed) for ", phys_file) << life;
  }
}

RR_file_data::~RR_file_data() 				// destructor
{
  delete reg_state;
  delete soft_reg_state;
}

////////////////////////////////////////////////////
// Distance computation
////////////////////////////////////////////////////

int RR_file_data::Compute_distance(const int aa, const int bb)
{
  RR_lifetime& a = life[aa];
  RR_lifetime& b = life[bb];
  int d1, d2(-infinity), d3(-infinity);

  // L term is set to an arbitrary value because all L terms cancels each others
  // could have eliminated the L term altogehter, but the formulas would 
  // not be as readable. This value must be consistant with the one in
  // RR_life::Process_input_data
  int L = 1;

  d1 = ceil(a.stop_time - b.start_time, II);
  if (wand_only) return d1;

  if (b.leading_blade)  d2 = -a.k_first;
  if (a.trailing_blade) d3 = b.k_last - (L-1-a.alpha);
  return ELCOR_MAX(ELCOR_MAX(d1, d2), d3);
}

////////////////////////////////////////////////////
// Disjoint predicates
////////////////////////////////////////////////////

// oper d1 is iter_diff ahead of u, d
bool RR_file_data::Disjoint(RR_ref_data* u, RR_ref_data* d, RR_ref_data* d1,
  const int iter_diff)
{
  if (dbg(ra, 7)) {
    cdbg << "disjoint? " << " iter_diff " << iter_diff 
	 << endl << "  " << *u << " and" 
	 << endl << "  " << *d << " disjoint with" 
	 << endl << "  " << *d1 << endl;
  }

  if (iter_diff != 0) {
    // have only information about predicate of the same iteration
    return false;
  }

  Pred pu = pqs->lub_src_guard(u->ref.o,u->ref.pn);
  Pred pd = pqs->lub_dest_guard(d->ref.o,d->ref.pn);
  Pred pd1 = pqs->lub_dest_guard(d1->ref.o,d1->ref.pn);

  if (pqs->is_disjoint(pu, pd1) || pqs->is_disjoint(pd, pd1)) {
    // d1 is disjoint with either u or d -> no rupture of the life value
    return true;
  }

  return false;
}

/*
   want to test if  P(u) & P(d) & P(d1) & ! P(x1) ... & ! P(xn) 
   (x1..xn member of redef_set) can be true.
 */

bool RR_file_data::Disjoint_with_redef(RR_ref_data* u, RR_ref_data* d, RR_ref_data* d1, 
  Hash_set<RR_ref_data*>& redef_set, const int iter_diff)
{
  RR_ref_data redef;

  // optimization: support currently only same iteration predicate querry!!!
  if (iter_diff != 0) return false;

  // test predicates (quick test)
  Pred pu = pqs->lub_src_guard(u->ref.o,u->ref.pn);
  Pred pd = pqs->lub_dest_guard(d->ref.o,d->ref.pn);
  Pred pd1 = pqs->lub_dest_guard(d1->ref.o,d1->ref.pn);
  if (pqs->is_disjoint(pu, pd1) || pqs->is_disjoint(pd, pd1)) {
    return true;
  }

  // compute liveness under which a value flows from the def d to use u, when
  // the value is recomputed by ops in the redef_set
  Pred_expr live_pred_expr = pqs->false_expr();
  pqs->lub_sum(live_pred_expr, pu);

  for(Hash_set_iterator<RR_ref_data*> it(redef_set); it != 0; it++){
    RR_ref_data* redef_ref = *it;
    Pred p = pqs->lub_dest_guard(redef_ref->ref.o,redef_ref->ref.pn);
    pqs->lub_diff(live_pred_expr, p);
  }
  if (pqs->is_disjoint(pd1, live_pred_expr)) {
    return true;
  }

  // test failed
  return false;
}

////////////////////////////////////////////////////
//  interference method
////////////////////////////////////////////////////

/*
  consider whether the register flow from the defining op d to the use op u
  of lifetime i is ruptured by the def op d1 of lifetime j.

*/

bool RR_file_data::Live_at_def(const int i, const int j,
  const int iter_i, const int iter_j)
{
  int offset, iter_diff; 
  int i1, j1;
  RR_ref_data *u, *d, *d1;

  offset = (iter_j - iter_i) * II;

  // check def of j on liveness of i
  //for each use u
  for(i1=0; i1<life[i].uses_num; i1++){
    u = &life[i].uses[i1];
    //for each reaching def of u: d
    for(Hash_set_iterator<int> i2(u->reaching_def); i2!=0; i2++){
      d = &life[i].defs[*i2];
      for(j1=0; j1<life[j].defs_num; j1++){
        d1 = &life[j].defs[j1];
        if (d->anti_time <= d1->flow_time + offset &&
            d1->anti_time + offset <= u->anti_time) {
          // they do overlap? (u == i; d1 == j)
          iter_diff = (iter_j + d1->iter_apart) - (iter_i + u->iter_apart);
          if (!Disjoint(u, d, d1, iter_diff)) return true;
        }
      }
    }
  }
  return false;
}

////////////////////////////////////////////////////
//  interference method (taking redefine into account
////////////////////////////////////////////////////


bool RR_file_data::Live_at_def_with_redef(const int i, const int j,
  const int iter_i, const int iter_j)
{
  int offset, iter_diff; 
  int i1, i3, j1;
  RR_ref_data *u, *d, *d1, *redef;
  Hash_set<RR_ref_data*> redef_set(hash_rr_ref_data_ptr, 16);

  offset = (iter_j - iter_i) * II;

  // check def of j on liveness of i
  //for each use u
  for(i1=0; i1<life[i].uses_num; i1++){
    u = &life[i].uses[i1];
    //for each reaching def of u: d
    for(Hash_set_iterator<int> i2(u->reaching_def); i2!=0; i2++){
      d = &life[i].defs[*i2];
      for(j1=0; j1<life[j].defs_num; j1++){
        d1 = &life[j].defs[j1];
        if (d->anti_time <= d1->flow_time + offset &&
            d1->anti_time + offset <= u->anti_time) {
          // build set of all defs of i that are between d1 and u
          redef_set.clear();
          for(i3=0; i3<life[i].defs_num; i3++){
            redef = &life[i].defs[i3];
            // redef will overwrite d1, and completes before u
            if((redef->anti_time > d1->flow_time + offset) &&
               (redef->flow_time < u->flow_time)){
		  /* RMR { conditionally add redef: only if this is not a scalar-to-vector move-defined operand 
		   * (since if it is, there will be multiple interveaning defintions of the same lifetime in the IR
		   * although they occur to different parts of the vector register
		   */
		  if (!life[i].svmove_def)
		    redef_set += redef;
              //redef_set += redef;
            }
          }
          // they do overlap? (u == i; d1 == j)
          iter_diff = (iter_j + d1->iter_apart) - (iter_i + u->iter_apart);
          if (!Disjoint_with_redef(u, d, d1, redef_set, iter_diff)) return true;
        }
      }
    }
  }
  return false;
}

////////////////////////////////////////////////////
// Overlap
////////////////////////////////////////////////////

/* 
   if the definitions of i in iteration iter_i occurs when
   the values of lifetime j in iteration iter_j are live, or vice versa,
   then the two lifetimes can not be overlapped safely
*/

bool RR_file_data::Interference(const int i, const int j, 
  const int iter_i, const int iter_j)
{
  if (
      // lifetime with itself never valid
      i == j ||
      // we do not handel cases with trailing_blade
      life[i].trailing_blade || life[j].trailing_blade ||
      // we do not handel cases with leading_blade
      life[i].leading_blade || life[j].leading_blade
     ) {
    return true;
  }

  if (Live_at_def_with_redef(i, j, iter_i, iter_j)) {
    return true;
  }
  if (Live_at_def_with_redef(j, i, iter_j, iter_i)) {
    return true;
  }

  if (dbg(ra, 4)) {
    cdbg << "### Overlap no interf: lifetimes " << i ;
    if (life[i].defs.size() && life[i].defs[0].operand_ptr) {
      cdbg << " def_operand(" << *life[i].defs[0].operand_ptr << ")";
    }
    cdbg << " and " << j ;
    if (life[j].defs.size() && life[j].defs[0].operand_ptr) {
      cdbg << " def_operand(" << *life[j].defs[0].operand_ptr << ")";
    }
    cdbg << endl;
  }
  return false;
}

int RR_file_data::Compute_overlap_no_interference_set(const int i, const int j)
{
  int loc, first, last;

  // assume i at location 0; forbidden range from first to last for j

  first = -(dist[j][i] -1);
  last = (dist[i][j] -1);
  for(loc=first; loc<=last; loc++) {
    if (!Interference(i, j, 0, loc)) {
      overlap_no_interference_loc[i][j] += loc;
    } 
  }  
  if (dbg(ra, 7) && overlap_no_interference_loc[i][j].size()) {
    cdbg << "Forbidden range: "<<first<<" to "<<last<<" "
	 << "Overlap_set("<<i<<","<<j<<") = "
	 << overlap_no_interference_loc[i][j] << endl;
  }
  return overlap_no_interference_loc[i][j].size();
}

bool RR_file_data::Overlap_no_interference(const int i, const int j, 
                         const int loc_i, const int loc_j)
{
  return (overlap_no_interference_loc[i][j].is_member(loc_j -loc_i));
}

////////////////////////////////////////////////////
// Compute Figure of Merit
////////////////////////////////////////////////////

int RR_file_data::Count_forbidden_loc(const int sel, const int loc)
{
  int i, first, last;
  int tot_added_forbidden_loc(0);

  if (dbg(ra, 7))
    cdbg << "added forbiden loc for lt " << sel << " at loc " << loc;

  // iterate over the unallocated lifetime only
  for(List_iterator<int> lit(unallocated_lt); lit != 0; lit++) {
    first = loc -(dist[*lit][sel] -1);
    last = loc + (dist[sel][*lit] -1);
    if (dbg(ra, 7)) cdbg << endl << "with lt " << *lit << ": ";
    for(i=first; i<=last; i++) {
      if (!forbidden_loc[*lit].is_member(i) &&
          !Overlap_no_interference(sel,*lit, loc, i)) {
        tot_added_forbidden_loc++;
        if (dbg(ra, 7)) cdbg << " " << loc+i;
      }
    }
  }
  if (dbg(ra, 7))
    cdbg << endl << "total forbiden loc " << tot_added_forbidden_loc << endl;
  return tot_added_forbidden_loc;
}

// inversely proportional to the number of additional forbidden locations

int RR_file_data::Compute_fom(const int sel, const int loc)
{
  return (infinity - Count_forbidden_loc(sel, loc));
}


////////////////////////////////////////////////////
// Update state when allocating a register to a loc
////////////////////////////////////////////////////

void RR_file_data::Update_forbidden_loc(const int sel, const int loc)
{
  int i, first, last;
  
  if (dbg(ra, 7)) cdbg << endl;

  // iterate over the unallocated lifetime only
  for(List_iterator<int> lit(unallocated_lt); lit != 0; lit++) {
    first = loc - (dist[*lit][sel] -1);
    last = loc + (dist[sel][*lit] -1);
    for(i=first; i<=last; i++) {
      if (!Overlap_no_interference(sel,*lit, loc, i)) {
        forbidden_loc[*lit] += i;
      }
    }
    if (dbg(ra, 7))
      cdbg << "lifetime " << *lit << " forbidden pos " << forbidden_loc[*lit];
  }
}

void RR_file_data::Soft_allocate_reg(const int sel, const int loc)
{
  life[sel].location = loc;		// update lifetime

  *soft_reg_state = *reg_state;
  soft_reg_state->Record(this, sel);

  if (sel != soft_alloc_last_sel) {		// do it only once per sel
    life[sel].allocated = true;
    unallocated_lt.remove(sel);			// update lists
    allocated_lt.add_tail(sel);
    soft_alloc_last_sel = sel;
  }
}
  
void RR_file_data::Allocate_reg(const int sel, const int loc)
 {
  Soft_allocate_reg(sel, loc);
  reg_state->Record(this, sel);
  Update_forbidden_loc(sel, loc);

  if (dbg(ra, 7)) 
    cdbg << "lifetime " << sel << " allocated in reg " << loc << endl;
}

////////////////////////////////////////////////////
// find location 
////////////////////////////////////////////////////

int RR_file_data::Best_fit(const int sel)
{
  int first_loc, last_loc, best_loc = 0, loc;
  int reg, best_reg;
  int fom, best_fom = 0;

  first_loc = infinity;
  last_loc = -infinity;

  if (dbg(ra, 7)) cdbg << endl << "WITH SEL " << sel << endl;

  // find interval of location where the best location is searched
  for(List_iterator<int> lit(allocated_lt); lit != 0; lit++) {
    first_loc = ELCOR_MIN(first_loc, life[*lit].location - dist[sel][*lit]);
    last_loc = ELCOR_MAX(last_loc, life[*lit].location + dist[*lit][sel]);
  } 
  if (dbg(ra, 7))
    cdbg << "first_loc " << first_loc << "; last_loc " << last_loc << endl;

  best_reg = infinity;
  for(loc=first_loc; loc<=last_loc; loc++) {
    if (!forbidden_loc[sel].is_member(loc)) {
      Soft_allocate_reg(sel, loc);
      reg = soft_reg_state->Requirements(this);
      if (dbg(ra, 7)) 
	cdbg << "with loc " << loc << "; reg " << reg << endl;
      if (reg < best_reg) {
        best_reg = reg;
        best_loc = loc;
        best_fom = undefined;
	if (dbg(ra, 7)) cdbg << "BEST REG" << endl;
      } else if (reg == best_reg) {
        if (best_fom == undefined) {
          best_fom = Compute_fom(sel, best_loc);
        }
        fom = Compute_fom(sel, loc);
	if (dbg(ra, 7)) 
	  cdbg << "SAME REG: prev-best<" << best_loc << "," << best_fom << ","
	       << infinity-best_fom << "> cur <" << loc << "," << fom << ","
	       << infinity-fom << ">" << endl;
        if (fom > best_fom) {
          if (dbg(ra, 7)) cdbg << "BEST FOM" << endl;
          best_reg = reg;
          best_loc = loc;
          best_fom = fom;
        }
      }
    }
  }
  assert(best_reg != infinity);
  return best_loc;
}

int RR_file_data::First_fit(const int sel, const int first_sel)
{
  int first_loc, loc;
    
  // conservative bound, assuming that overlap_no_interference_loc can be true for any of
  // the point between -(dist[sel][first_sel]-1) to 
  // (dist[first_sel][sel]-1)

  first_loc = life[first_sel].location - (dist[sel][first_sel]-1);
  for(loc=first_loc; true; loc++) {
    if (!forbidden_loc[sel].is_member(loc)) {
      return loc;
    }
  }
  return(-1) ;
}  

int RR_file_data::End_fit(const int sel, const int last_sel)
{
  int last_loc, loc;

  // conservative bound, assuming that overlap_no_interference_loc can be true for any of
  // the point between -(dist[sel][last_sel]-1) to 
  // (dist[last_sel][sel]-1)

  last_loc = life[last_sel].location - (dist[sel][last_sel]-1);
  for(loc=last_loc; true; loc++) {
    if (!forbidden_loc[sel].is_member(loc)) {
      return loc;
    }
  }
  return (-1);
}  
 
////////////////////////////////////////////////////
// Inialize all data structures
////////////////////////////////////////////////////


void RR_file_data::Init_alloc(const bool predicated_strategy,
  const Ordering_strategy& ordering_strategy)
{
  RR_order order;
  int i, j, c;

  // init pqs data
  pqs = get_pqs((Compound_region*)IR_kernel);
  if (dbg(ra, 7)) cdbg << "pqs: " << endl << *pqs << endl;

  // Initialize_distances
  predicated_allowed = predicated_strategy;
  dist = Matrix<int>(life_num, life_num, 0);
  conf = Matrix<int>(life_num, life_num, 0);
  Hash_set<int> empty_set(hash_int, 8);
  overlap_no_interference_loc = Matrix<Hash_set<int> >(life_num, life_num, empty_set);

  for(i=0; i<life_num; i++) {
    for(j=0; j<life_num; j++) {
      dist[i][j] = Compute_distance(i, j);
    }
  }
  if (predicated_strategy) {
    for(i=0; i<life_num; i++) {
      for(j=0; j<life_num; j++) {
        Compute_overlap_no_interference_set(i, j);
      }
    }
  }

  for(i=0; i<life_num; i++) {
    conf[i][i]  = dist[i][i]-1;
    for(j=0; j<i; j++) {
        // compute conflict matrix (symmetric)
        // substract the extend of the overlap extent, because
        // each of these points are allowed
        c = dist[i][j] + dist[j][i] -1 -
            overlap_no_interference_loc[i][j].size();
        conf[i][j] = conf[j][i] = c;
    }
  }

  // Initialize_lists
  unallocated_lt = order.Order(this, ordering_strategy);
  allocated_lt.clear();				// no lt is allocated

  Hash_set<int> dummy_set(hash_int, 16);
  forbidden_loc = Vector< Hash_set<int> > (life_num, dummy_set);
  for(i=0; i<life_num; i++) {
    forbidden_loc[i].clear();			// all location allowed
  }  

  // init static values
  soft_alloc_last_sel = undefined;

  // init register state
  reg_state = new RR_state();
  soft_reg_state = new RR_state();
}

void RR_file_data::Construct_lifetimes(
    const int ii, 
    const int sc,
    const int prologue_peeling, 
    const int epilogue_peeling,
    const int theta_par)
{
  // switch when no lifetimes
  if (life_num == 0 && allocated) return;

  // initialize file variables
  PS = prologue_peeling;
  ES = epilogue_peeling;
  theta = theta_par;
  II = ii;  
  stage_count =  sc;

  // initalize lifetimes
  int real_life_num = (open_loop) ? life_num-1 : life_num;

  for(int i=0; i<real_life_num; i++) {
    life[i].Process_input_data(II, stage_count, theta, PS, ES);
  }
  if (open_loop) {
    life[open_loop_lifetime].Process_open_loop_data(II, stage_count, theta, PS, ES);
  }
}

////////////////////////////////////////////////////
// Allocate registers
////////////////////////////////////////////////////

int RR_file_data::Allocate_register(
    const bool wand_only_strategy,
    const bool predicated_strategy,
    const Alloc_strategy alloc_strategy, 
    const Ordering_strategy& ordering_strategy)
{
  int sel, first_sel, previous_sel;
  int loc;

  // switch when no lifetimes
  if (life_num == 0 && allocated) return reg_num;

  wand_only = wand_only_strategy;
  Init_alloc(predicated_strategy, ordering_strategy);

  if (dbg(ra, 7)) cdbg << DEBUG_TITLE2("begin alloc for", phys_file); 
  first_sel = sel = unallocated_lt.head();
  Allocate_reg(sel, 0); 			// allocate first reg

  if (alloc_strategy == first_fit) {
    while(!unallocated_lt.is_empty()) {
      sel = unallocated_lt.head();
      loc = First_fit(sel, first_sel);
      Allocate_reg(sel, loc);
    }
  } else if (alloc_strategy == end_fit) {
    while(!unallocated_lt.is_empty()) {
      previous_sel = sel;
      sel = unallocated_lt.head();
      loc = End_fit(sel, previous_sel);
      Allocate_reg(sel, loc);
    }
  } else if (alloc_strategy == best_fit) {
    while(!unallocated_lt.is_empty()) {
      sel = unallocated_lt.head();
      loc = Best_fit(sel);
      Allocate_reg(sel, loc);
    }
  } else {
      El_punt("Unkown allocation strategy\n");
  }

  int prev_reg_num = -1;
  if (open_loop) {
    prev_reg_num = reg_state->Requirements(this);

    // put it so that no lifetime cross the modulo barrier
    // achieved by ensuring the start location to be positive
    // -- Shail Aditya 02/26/2001
    int min_alloc=0;
    for (int j=0; j<life_num; j++)
      if (life[j].location < min_alloc) min_alloc = life[j].location;
    // Old code: still leaves some locations -ve => across modulo boundary.
    // want that blade to be in location 0
    // rotate_alloc = - life[open_loop_lifetime].location ;
    rotate_alloc = - min_alloc ;
    
    // recompute the register shape
    delete reg_state;
    reg_state = new RR_state;
    for(int i=0; i<life_num; i++) {
      life[i].location += rotate_alloc;
      reg_state->Record(this, i);
    }
  }
  reg_num = reg_state->Requirements(this);
  if (open_loop && prev_reg_num != reg_num) {
    cerr << "RR: correct solution but open-loop model after rotation required "
	 << reg_num << " instead of " << prev_reg_num << " registers (diff " 
	 << reg_num - prev_reg_num << ")" << endl;
  }

  allocated = true;
  return reg_num;
}

////////////////////////////////////////////////////
// check result (print)
////////////////////////////////////////////////////

bool RR_file_data::Check_allocation(const bool check_minimality)
{
  int iter, i;

  // switch when no lifetimes
  if (life_num == 0 && allocated) return true;
  
  iter = 2 * reg_num;

  RR_space_time sp(this, reg_num, iter);

  if (!sp.Valid_alloc()) {
    // discovered faulty
    cerr << DEBUG_TITLE2("ERRORS: NON VALID for ", phys_file) << endl;

    // compute how many more register needed
    for(i=1; i<100; i++) {
      RR_space_time sp_debug(this, (reg_num+i), 2*(reg_num+i));
      if (sp_debug.Valid_alloc()) {
        cerr << i << " more register needed: (total  "
	     << reg_num+i << ")" << endl;
        return false;
      }
    }  
    cerr << i << " more register needed: (more than  "
	 << reg_num+i << ")" << endl;
    return false;
  }

  // check_minimality
  if (sp.Valid_alloc() && check_minimality) {
    if (dbg(ra, 1)) cdbg << "check minimality" << endl;
    // check all
    for(i=1; i<reg_num; i++) {
      RR_space_time sp_check(this, i, 2*i);
      if (sp_check.Valid_alloc()) {
        cerr << "reg file VIOLATED the minimality assumtion !!!" << endl
	     << reg_num-i << " less register needed: (totat  "<< i << ")" << endl; 
        return sp.Valid_alloc();
      }
    }
  }  
  return true;
}

////////////////////////////////////////////////////
// commit the register alloc solution in the IR
////////////////////////////////////////////////////

bool RR_file_data::Commit_register_allocation(const int tot_reg)
{ 
  int i;

  assert(allocated);
  if (commited) return true;

  commit_reg_num = tot_reg;

  int real_life_num = (open_loop) ? life_num-1 : life_num;
  for(i=0; i<real_life_num; i++) {
    life[i].Commit(II, commit_reg_num);
  }
  commited = true;
  return commited;
}

////////////////////////////////////////////////////
// Peeling
////////////////////////////////////////////////////
  
void RR_file_data::Peeling_for_wand_only(int& lps, int& les) 
{
  LPS = 0;
  LES = 0;
  for(int i=0; i<life_num; i++) {
    if (life[i].omega>0) {
      int t = ceil(life[i].start_time, II) -1;
      LPS = ELCOR_MAX(LPS, t);
      t = stage_count - floor(life[i].stop_time, II) + life[i].alpha - 1;
      LES = ELCOR_MAX(LES, t);
    }
  }
  lps = LPS;
  les = LES;
}

////////////////////////////////////////////////////
// Export register allocation
////////////////////////////////////////////////////

int RR_file_data::Get_entry_register_allocation(Operand& operand)
{
  assert(operand.is_reg());
  //  assert(operand.file_type() == reg_file_to_alloc);
  //assert(operand.physical_file_type() == phys_file);
  assert(compatible_file_type(operand));
  assert(vreg_to_lifetime_index.is_bound(operand.vr_num()));
  // find data associated with this vr (should have been created earlier)
  int index = vreg_to_lifetime_index.value(operand.vr_num());
  int omega_rel = operand.omega() - life[index].base_evr;
  int mc = mod(life[index].location + omega_rel, commit_reg_num);
  if (dbg(ra, 4)) {
    cdbg << " lifetime-livein(" << index << ")" 
	 << " omega(" << operand.omega() << ")" 
	 << " omega relative(" << omega_rel << ")"
	 << " relocatable addr(" << mc << ")" << endl;
  }
  return mc;
}


int RR_file_data::Get_exit_register_allocation(Operand& operand)
{
  assert(operand.is_reg());
  //  assert(operand.file_type() == reg_file_to_alloc);
//  assert(operand.physical_file_type() == phys_file);
  assert(compatible_file_type(operand));
  assert(vreg_to_lifetime_index.is_bound(operand.vr_num()));
  // find data associated with this vr (should have been created earlier)
  int index = vreg_to_lifetime_index.value(operand.vr_num());
  int omega_rel = operand.omega() - life[index].base_evr;
// This is Alex's original code
//      mc = mod(life[index].location + omega_rel + stage_count, commit_reg_num);
//
// This is a possible modification to fix the live_out indexing problem
// Need to look at it more carefully -- Shail + Vinod 10/09/95
  int mc = mod(life[index].location + omega_rel + stage_count - 1, commit_reg_num);
  if (dbg(ra, 4)) {
    cdbg << " lifetime-liveout(" << index << ")" 
	 << " omega(" << operand.omega() << ")"
	 << " omega relative(" << omega_rel << ")" 
	 << " relocatable addr(" << mc << ")" << endl;
  }
  return mc;
}


bool RR_file_data::Get_live_in_register_allocation(Map<Operand,int>& map)
{
  if (!commited) return false;
  if (commited && life_num == 0) return true;
  
  if (dbg(ra, 4)) 
    cdbg << DEBUG_TITLE2("Live in for ", phys_file) << endl;
  assert(IR_loopbody != 0);

  // map the livein operands that belong to current reg file
  Hash_set<Operand> live_in(hash_operand, 16);
  for(Region_entry_edges ii1(IR_loopbody); ii1 != 0; ii1++) {
    Edge* curr_edge = *ii1;
    Liveness_info* curr_live = get_liveness_info(curr_edge);
    for (Liveness_info_iterator li2(*curr_live) ; li2 != 0; li2++) {
       live_in += *li2;
    }
  }
     
  for(Hash_set_iterator<Operand> ii2(live_in); ii2 != 0; ii2++) {
    Operand& operand = *ii2;
    if (operand.is_reg() && 
	//        operand.file_type() == reg_file_to_alloc &&
	//operand.physical_file_type() == phys_file &&
	compatible_file_type(operand) &&
        vreg_to_lifetime_index.is_bound(operand.vr_num())) {
      int mc = Get_entry_register_allocation(operand);
      if (map.is_bound(operand)) {
        if (mc != map.value(operand)) El_punt("conflicting register alloc mapping\n");
      }
	/* RMR { while some liveranges can reach the loop, they may be
	 * killed in the loop before they are used: in other words, the
	 * omega for the operand is 0. there is a discrepancy between
	 * the liveness analysis done in the allocator (stored in
	 * life[]) and the set of liveins computed before the
	 * allocation: a liverange can exists in the latter but not the
	 * former. here we check if the operand that was live before
	 * the allocator's liveness analysis is still live 
	 * (i.e., omega != 0) and only add the operand to the set of
	 * livein operands if it remains live.
	 */
	else {
	  int index = vreg_to_lifetime_index.value(operand.vr_num());
	  if (life[index].omega == 0) continue;
	}
	/* } RMR */
      map.bind(operand, mc);
    }
  }
  return true;
}

bool RR_file_data::Get_live_out_register_allocation(Map<Operand,int>& map)
{
  if (!commited) return false;
  if (commited && life_num == 0) return true;

  if (dbg(ra, 4)) 
    cdbg << DEBUG_TITLE2("Live out for ", phys_file) << endl;
  assert(IR_loopbody != 0);

  // map the liveout operands that belong to current reg file
  Hash_set<Operand> live_out(hash_operand, 16);
  for(Region_exit_edges ii1(IR_loopbody); ii1 != 0; ii1++) {
    Edge* curr_edge = *ii1;
    Liveness_info* curr_live = get_liveness_info(curr_edge);
    for (Liveness_info_iterator li2(*curr_live) ; li2 != 0; li2++) {
       live_out += *li2;
    }
  }
     
  for(Hash_set_iterator<Operand> ii2(live_out); ii2 != 0; ii2++) {
    Operand& operand = *ii2;

    if (operand.is_reg() && 
	//        operand.file_type() == reg_file_to_alloc &&
	//operand.physical_file_type() == phys_file &&
	compatible_file_type(operand) &&
        vreg_to_lifetime_index.is_bound(operand.vr_num())) {
      int mc = Get_exit_register_allocation(operand);
      if (map.is_bound(operand)) {
        if (mc != map.value(operand)) El_punt("conflicting register alloc mapping\n");
      }
      map.bind(operand, mc);
    }
  }
  return true;
}

bool RR_file_data::compatible_file_type(Operand& oprnd)
{
   if (oprnd.assigned_to_physical_file()) {
      return oprnd.physical_file_type() == phys_file;
   }
   else {
      Reg_file vrf = oprnd.file_type();
      char* vrf_string = vregfile_to_mdesname(vrf);

      int i = MDES_reg_index((char*) phys_file);
      Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
      char* virt_files = rd->get_vname();

      return (strchr(virt_files, vrf_string[0]) != NULL);
   }
}


int RR_file_data::get_longest_lifetime() const
{
  int max_life = -1;
  for(unsigned i = 0; i < life.size(); i++) {
    max_life = ELCOR_MAX(max_life, life[i].stop_time - life[i].start_time);
  }
  return max_life;
}
