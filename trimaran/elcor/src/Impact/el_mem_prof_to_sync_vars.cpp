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




////////////////////////////////////////////////////////////////////////////
//
//      File:           el_mem_prof_to_sync_vars.cpp
//      Author:         Santosh G. Abraham
//      Created:        February 1996
//      Description:    Translating from Lcode memory profile to synch vars
//
////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>

#include "el_mem_prof_to_sync_vars.h"
#include "opcode_properties.h"
#include "attributes.h"
#include "list.h"
#include "el_io.h"
#include "dbg.h"
#include "slist.h"
#include "hash_map.h"
#include "hash_functions.h"
#include "iterators.h"

// from Lcode memory profile annotations,
// get a map of load_ids to number of times they aliased
void get_load_dep(Op* mem_op, Map <int, int>& opid_alias_cnt)
{
  const char* dep = "dep";
  const char* ll = "_ll_";
  const char* ls = "_ls_";

 assert (is_memory(mem_op));
 Lcode_attribute_map* lam = get_lcode_attributes((Region *) mem_op);
 for (Map_iterator <eString, List<Operand> > miter(*lam); miter !=0; miter++) {
   char* dep_xx = (char*) ((*miter).first);
   assert (dep_xx != 0);

   // skip rest if annotation doesn't begin with "dep"
   if (strstr(dep_xx, dep) != dep_xx) continue;

   // if annotation begins with "dep_ll_" or "dep_ls_",
   //	 it is a load sourcing dependence, ignore others
   dep_xx = dep_xx + 3;
   if ((strstr(dep_xx, ll) == dep_xx) || (strstr(dep_xx, ls) == dep_xx)) {
     List_iterator <Operand> mprof_info ((*miter).second);
     // first part of annotation is load id
     Operand ld_oper = (*mprof_info); mprof_info++;
     int ldid = ld_oper.int_value();
     // second part is number of times it aliased
     Operand alias_cnt_oper = (*mprof_info); mprof_info++;
     int alias_cnt = alias_cnt_oper.int_value();
     assert (mprof_info == 0);	// only two fields after "dep_xx_y"
     // should see only one annotation per sourcing load
     assert (!opid_alias_cnt.is_bound(ldid));
     opid_alias_cnt.bind(ldid, alias_cnt);
   }
 }
 if (dbg(ss, 9)) cdbg << opid_alias_cnt;
}

// from Lcode memory profile annotations,
// get a map of store_ids to number of times they aliased
void get_store_dep(Op* mem_op, Map <int, int>& opid_alias_cnt)
{
  const char dep[] = "dep";
  const char str_sl[] = "_sl_";
  const char str_ss[] = "_ss_";

 assert (is_memory(mem_op));
 Lcode_attribute_map* lam = get_lcode_attributes((Region *) mem_op);
 for (Map_iterator <eString, List<Operand> > miter(*lam); miter !=0; miter++) {
   char* dep_xx = (char*) ((*miter).first);
   assert (dep_xx != 0);

   // skip rest if annotation doesn't begin with "dep"
   if (strstr(dep_xx, dep) != dep_xx) continue;

   // if annotation begins with "dep_sl_" or "dep_ss_",
   //	 it is a store sourcing dependence, ignore others
   dep_xx = dep_xx + 3;
   if ((strstr(dep_xx, str_sl) == dep_xx) ||
       (strstr(dep_xx, str_ss) == dep_xx)) {
     List_iterator <Operand> mprof_info ((*miter).second);
     // first part of annotation is load id
     Operand st_oper = (*mprof_info); mprof_info++;
     int stid = st_oper.int_value();
     // second part is number of times it aliased
     Operand alias_cnt_oper = (*mprof_info); mprof_info++;
     int alias_cnt = alias_cnt_oper.int_value();
     assert (mprof_info == 0);	// only two fields after "dep_xx_y"
     // should see only one annotation per sourcing load
     assert (!opid_alias_cnt.is_bound(stid));
     opid_alias_cnt.bind(stid, alias_cnt);
   }
 }
 if (dbg(ss, 9)) cdbg << opid_alias_cnt;
}

// print op ids of ops that have profiled mem dep into mem_op
void print_mem_dep_map(Op* mem_op, Map <int, int>& opid_alias_cnt) {

  if (opid_alias_cnt.is_empty()) return;
  cdbg << "Printing memory dependences to Op Id: " << endl;
  if (is_load(mem_op)) cdbg << "L";
  else cdbg << "S";
  cdbg << mem_op->id();
  Map_iterator <int, int> miter(opid_alias_cnt);
  if (miter != 0) cdbg << " <-";
  for (; miter != 0; miter++) {
    cdbg << " " << (*miter).first;
  }
  cdbg << endl;
}

// from Lcode memory profile annotations,
// get a map of  memory op ids to number of times they aliased
void get_mem_dep(Op* mem_op, Map <int, int>& opid_alias_cnt) {
  get_load_dep(mem_op,  opid_alias_cnt);
  get_store_dep(mem_op, opid_alias_cnt);
  if (dbg(ss, 8)) print_mem_dep_map(mem_op, opid_alias_cnt);
}

MprofSvars::MprofSvars ()
 : opid2mr (hash_int, 1023),
   opid2pop (hash_int, 1023),
   memvr2alist (hash_operand, 1023)

{}

MprofSvars::~MprofSvars ()
{}

MprofSvars::MprofSvars (Procedure* f)
 : graph (f, GRAPH_OPS),      // Create graph in Alist domain
   graph_iter (graph, UP),     // Reverse postorder iterator for forward flow
   opid2mr (hash_int, 1023),
   opid2pop (hash_int, 1023),
   memvr2alist (hash_operand, 1023)
{
 init (f);
}

void
MprofSvars::init (Procedure* f) {

 int alisti, mri;
 num_ops = graph.b_map.size();

 // Init alist2optype Vector
 alist2optype.resize(num_ops);
 num_mrops = 0;
 for (alisti = 0; alisti < num_ops; alisti++) {
   Op* op = (Op* ) graph.b_map[alisti];
   Lcode_attribute_map* lam = get_lcode_attributes((Region *) op);
   if (lam->is_bound("dep_mem_barrier"))
			{ alist2optype[alisti] = OTHER_MEM_BARRIER;
				num_mrops++; continue; }
   if (is_load(op))	{ alist2optype[alisti] = LOAD;
   				num_mrops++; continue; }
   if (is_store(op))	{ alist2optype[alisti] = STORE;
   				num_mrops++; continue; }
   if (is_brl(op) || is_rts(op))
			{ alist2optype[alisti] = CALLRET;
				num_mrops++; continue; }

   assert(!is_memref(op));
   alist2optype[alisti] = NOT_MEMREF;
 }

 // Resize bitmatrix Vector dimension
 // Leave individual bitvectors at size 0 for now for efficiency
 orig_memdep.resize(num_mrops);
 cl_orig_memdep.resize(num_mrops);
 prof_memdep.resize(num_mrops);

 // if in debug mode, generate mr_ids sorted by their op ids
 // for easy association in print out, otherwise,
 // generate them in reverse postorder for efficiency
 mr2alist.resize(num_mrops);
 alist2mr.resize(num_ops);

 if (dbg(ss, 6)) {
   Slist <Stuple <int, int> > opid_slist;
   Stuple <int, int> opid_vec;

   // generate sorted list of memref'ing op ids' sorted by their op id's
   // list entry also contains index of op in Alist_graph domain
   mri = 0;
   for (alisti = 0; alisti < num_ops; alisti++) {
     Op* op = (Op *) graph.b_map[alisti];
     if (alist2optype[alisti] != NOT_MEMREF) {
       assert (is_memref(op));
       int id = op->id();
       opid_vec.first = id;
       opid_vec.second = alisti;
       assert (!opid_slist.is_member(opid_vec));
       opid_slist.add(opid_vec);
       mri++;
     } else {
       alist2mr[alisti] = -1;
     }
   }
   assert (mri == num_mrops);

   // now iterate over sorted list and generate both mr2alist and alist2mr
   mri = 0;
   for (Slist_iterator<Stuple <int, int> > si(opid_slist); si != 0; si++) {
     opid_vec = (*si);
     int alist_id = opid_vec.second;
     mr2alist[mri] = alist_id;
     alist2mr[alist_id] = mri;
     mri++;
   }
   assert (mri == num_mrops);

 } else {
   // when not debugging, use efficient mrops numbering
   mri = 0;
   for (alisti = 0; alisti < num_ops; alisti++) {
     if (alist2optype[alisti] != NOT_MEMREF) {     
       mr2alist[mri] = alisti;
       alist2mr[alisti] = mri;
       mri++;
     } else {
       alist2mr[alisti] = -1;
     }
   }
   assert (mri == num_mrops);
 }

 // generate maps from mr_ids to op_ids and vice-versa and opid2pop
 mr2opid.resize(num_mrops);
 for (mri = 0; mri < num_mrops; mri++) {
   alisti = mr2alist[mri];
   Op* op = (Op *) graph.b_map[alisti];
   int op_id = op->id();
   mr2opid[mri] = op_id;
   opid2mr.bind(op_id, mri);
   opid2pop.bind(op_id, op);
 }

 // generate list of calls/returns
 for (mri = 0; mri < num_mrops; mri++) {
   alisti = mr2alist[mri];
   Op* op = (Op *) graph.b_map[alisti];
   if (alist2optype[alisti] == CALLRET) {
     assert (is_call(op) || is_rts(op));
     membarriers.add_tail(alisti);
   } else if (alist2optype[alisti] == OTHER_MEM_BARRIER) {
     membarriers.add_tail(alisti);
   } else {
     assert (!(is_call(op) || is_rts(op)));
   }
 }
}
 
void
MprofSvars::read_orig_mem_deps() {

 // generate memvr2alist map, mapping mem vr to its
 // defining Op's Alist domain id
 int alisti, mri;
 for (alisti=0; alisti<num_ops; alisti++) {
   if (alist2optype[alisti] != NOT_MEMREF) {
     Op* op = (Op *) graph.b_map[alisti];
     for (Op_mem_dests mvri(op); mvri != 0; mvri++) {
       Operand memvr = *(mvri);
       assert (!memvr2alist.is_bound(memvr));
       memvr2alist.bind(memvr, alisti);
     }
   }
 }

 // read out mem sources for each mem ref'ing op and set bits
 // corresponding to ops it is dependent on
 for (alisti=0; alisti<num_ops; alisti++) {
   if (alist2optype[alisti] != NOT_MEMREF) {
     mri = alist2mr[alisti];
     Bitvector& bv = orig_memdep[mri];
     bv.resize(num_mrops);
     Op* op = (Op *) graph.b_map[alisti];
     for (Op_mem_sources mvri(op); mvri != 0; mvri++) {
       Operand memvr = *(mvri);
       assert (memvr2alist.is_bound(memvr));
       int alistj = memvr2alist.value(memvr);
       assert (alist2optype[alistj] != NOT_MEMREF);
       int mrj = alist2mr[alistj];
       bv.set_bit(mrj);
     }
   }
 }

 if (dbg(ss, 6)) {
   cdbg << "Original dependence encoding bit matrix before closure:" << endl;
   print_bit_matrix(orig_memdep);
 }
}

void
MprofSvars::closure_orig_deps(int& iter_count)
{

 // input is bitmatrix orig_memdep and output is cl_orig_memdep
 // if B->A and C->B in orig_memdep then mark C->A and iterate
 int alisti, mri ;
 bool not_done = true;
 Bitvector old_bv;

 // copy orig_memdep into cl_orig_memdep
 for (mri = 0; mri < num_mrops; mri++) {
   Bitvector& bvo = orig_memdep[mri];
   Bitvector& bvc = cl_orig_memdep[mri];
   bvc = bvo;
 }

 // repeat until convergence (i.e. no change in Bitvectors)
 while (not_done) {
   iter_count++;
   not_done = false;
   // iterate over all ops
   for(graph_iter.reset(); graph_iter != 0; graph_iter++) {
     alisti = *graph_iter;
     if (alist2optype[alisti] != NOT_MEMREF) {
       // read out bitvector of this memref'ing op
       mri = alist2mr[alisti];
       Bitvector& bva = cl_orig_memdep[mri];
       if (bva.is_allclear()) continue;
       // if any bit is set, make a copy in old_bv
       old_bv = bva;
       // iterate over all ops, B, that A depends on
       for (Bit_iterator bi(bva) ; bi != 0 ; bi++) {
	 int mrb = *(bi);
	 Bitvector& bvb = cl_orig_memdep[mrb];
	 // for all ops, C, that B depends on, mark C as depending on A
	 bva += bvb;
       }
       // if there is any change in old_rmo, repeat while loop
       if (bva != old_bv) not_done = true;
     }
   }
 }
 if (dbg(ss, 6)) {
   cdbg << "Original dependence encoding bit matrix after closure" << endl;
   print_bit_matrix(cl_orig_memdep);
 }
}


void
MprofSvars::read_prof_mem_deps()
{
 // for each memory op (load/store), get map of ops it is dependent upon
 // (and alias count, which is unused now)
 // iterate over this map,
 //  and set corresponding bit of the memory op's bitvector
 int alisti, mri;

 for (alisti = 0; alisti < num_ops; alisti++) {
   if (alist2optype[alisti] != NOT_MEMREF) {
     mri = alist2mr[alisti];
     Bitvector& bv = prof_memdep[mri];
     bv.resize(num_mrops);
     if (alist2optype[alisti] == LOAD || alist2optype[alisti] == STORE) {
       Op* op = (Op *) graph.b_map[alisti];
       Map <int, int> *opdep_alias_cnt = new Map <int, int>;
       get_mem_dep(op, *opdep_alias_cnt);
       // TLJ - assign the mapping to the mdp attributes attached to op
       //if (alist2optype[alisti] == STORE)
       	set_mdp_attributes(op,opdep_alias_cnt);
       if (opdep_alias_cnt->is_empty()) continue;
       for (Map_iterator<int, int> miter(*opdep_alias_cnt);
	    miter != 0; miter++) {
	 int opidj = (*miter).first;
	 assert ((*miter).second > 0);
	 assert (opid2mr.is_bound(opidj));
       	 bv.set_bit(opid2mr.value(opidj));
       }
     }
   }
 }
 if (dbg(ss, 6)) {
   cdbg << "Profiled dep bit matrix after setting from Lcode input:" << endl;
   print_bit_matrix(prof_memdep);
 }
}

void
MprofSvars::process_prof_deps ()
{
 // Determine closure of memory dependencies
 // i.e., if loadA -> loadB and loadB -> storeC, record loadA -> storeC
 // and if  storeA -> loadB and loadB -> load C, record storeA-> loadC

 // maps selected ls/sl from a block and maps them to an external load op
 // in another block through which the profiled dependence is threaded
 // ls/sl pair is product of l/s mrids; extop id is also mr id
 Hash_map<int, int> extop_map (hash_int, 211);
 // maps mr ids to its block (hb/bb) id
 Hash_map<int, int> mr2blockid (hash_int, 211);
 Bitvector old_bv;
 int alisti, mri;

 assert (num_mrops*num_mrops < ELCOR_MAX_INT);

 // map each load/store op to its hb/sb region id
 for (mri = 0; mri < num_mrops; mri++) {
   alisti = mr2alist[mri];
   if (alist2optype[alisti] == LOAD || alist2optype[alisti] == STORE) {
     Op* op = (Op *) graph.b_map[alisti];
     Compound_region* ancestor = op->parent();
     assert (ancestor->is_hb() || ancestor->is_bb());
     Compound_region* next_ancestor = ancestor->parent();
     while (next_ancestor->is_hb() || next_ancestor->is_bb()) {
       ancestor = next_ancestor;
       next_ancestor = ancestor->parent();
     }
     assert (next_ancestor != NULL);
     mr2blockid.bind (mri, ancestor->id());
   }
 }
  
 // get dependence closure for profiled dependences
 // L1->L2->S => L1->S and S->L1->L2 => S->L2
 // no other closure except above two
 int iter_count = 0;
 bool not_done = true;
 while (not_done) {
   iter_count++;
   not_done = false;
   for(graph_iter.reset(); graph_iter != 0; graph_iter++) {
     alisti = *graph_iter;
     mri = alist2mr[alisti];
     // if Sk->Lj and Lj->Li then Sk->Li also
     if (alist2optype[alisti] == LOAD) {
       Bitvector& bvi = prof_memdep[mri];
       if (bvi.is_allclear()) continue;
       old_bv = bvi;
       for (Bit_iterator bi(old_bv) ; bi != 0; bi++) {
	 int mrj = *(bi);
	 int alistj = mr2alist[mrj];
	 if (alist2optype[alistj] != LOAD) continue;
	 Bitvector& bvj = prof_memdep[mrj];
	 if (bvj.is_allclear()) continue;
	 for (Bit_iterator bj(bvj); bj != 0; bj++) {
	   int mrk = *(bj);
	   int alistk = mr2alist[mrk];
	   if (alist2optype[alistk] != STORE) continue;
	   // if Sk and Li are in the same hb/bb
	   int hbk = mr2blockid.value(mrk);
	   if (mr2blockid.value(mri) == hbk) {
	     int sk_lj = mrk*num_mrops + mrj;
	     // if Lj is also in same block or transitive dep from Sk to Lj
	     // through an external op
	     if (mr2blockid.value(mrj) != hbk || extop_map.is_bound(sk_lj)) {
	       // if no direct dependence from Sk->lI already
	       if (!bvi.bit(mrk)) {
		 int ext_op = mrj;
		 if (extop_map.is_bound(sk_lj))
		   ext_op = extop_map.value(sk_lj);
		 int sk_li = mrk*num_mrops + mri;
		 extop_map.bind(sk_li, ext_op);
	       }
	     }
	     else {
	       int sk_li = mrk*num_mrops + mri;
	       if (extop_map.is_bound(sk_li))
		 extop_map.unbind(sk_li);
	     }
	   }
	   bvi.set_bit(mrk);
	   // TLJ - make a new attr for Li that gives 
	   // alias freq of Sk->Li 
	   // (set to min of freq of Sk->Lj and Lj->Li for now)
	   Op *st_op = (Op *) graph.b_map[alistk];
	   Op *ldi_op = (Op *) graph.b_map[alisti];
	   MemDepProf_attribute_map *ldimap = get_mdp_attributes(ldi_op);
	   Op *ldj_op = (Op *) graph.b_map[alistj];
	   MemDepProf_attribute_map *ldjmap = get_mdp_attributes(ldj_op);
	   //cerr << "collapse deps from S " << st_op->id() << " to L " <<
	//			ldj_op->id() << " to L " << ldi_op->id() << endl;
	   int current = 0;
	   if(ldimap->is_bound(st_op->id())) 
	     current = ldimap->value(st_op->id());
	   assert(ldimap->is_bound(ldj_op->id()));
	   assert(ldjmap->is_bound(st_op->id()));
	   int new_freq = ELCOR_MIN(ldjmap->value(st_op->id()),
				ldimap->value(ldj_op->id()));
	   ldimap->bind(st_op->id(),current+new_freq);
	 }
       }
       if (bvi != old_bv) not_done = true;
     }
     // Lk->Lj>Si => Lk->Si
     if (alist2optype[alisti] == STORE) {
       Bitvector& bvi = prof_memdep[mri];
       if (bvi.is_allclear()) continue;
       old_bv = bvi;
       for (Bit_iterator bi(old_bv) ; bi != 0; bi++) {
	 int mrj = *(bi);
	 int alistj = mr2alist[mrj];
	 if (alist2optype[alistj] != LOAD) continue;
	 Bitvector& bvj = prof_memdep[mrj];
	 if (bvj.is_allclear()) continue;
         for (Bit_iterator bj(bvj); bj != 0; bj++) {
	   int mrk = *(bj);
	   int alistk = mr2alist[mrk];
	   if (alist2optype[alistk] != LOAD) continue;
	   // if Lk and Si are in the same hb/bb
	   int hbk = mr2blockid.value(mrk);
	   if (mr2blockid.value(mri) == hbk) {
	     int lk_lj = mrk*num_mrops + mrj;
	     if (mr2blockid.value(mrj) != hbk || extop_map.is_bound(lk_lj)) {
	       if (!bvi.bit(mrk)) {
		 int ext_op = mrj;
		 if (extop_map.is_bound(lk_lj))
		   ext_op = extop_map.value(lk_lj);
		 int lk_si = mrk*num_mrops + mri;
		 extop_map.bind(lk_si, ext_op);
	       }
	     }
	     else {
	       int lk_si = mrk*num_mrops + mri;
	       if (extop_map.is_bound(lk_si))
		 extop_map.unbind(lk_si);
	     }
	   }
	   bvi.set_bit(mrk);
	   //cerr << "collapsed deps from L to L to S" << endl;
	 }
       }
       if (bvi != old_bv) not_done = true;
     }
   }
   if (not_done && dbg(ss, 6)) {
     cdbg << "Profiled dep bit matrix after " << iter_count << " iterations:" << endl;
     print_bit_matrix(prof_memdep);
   }
 }
 if (dbg(ss, 6)) {
   cdbg << "Profiled dep bit matrix after closure:" << endl;
   print_bit_matrix(prof_memdep);
 }


 // remove remaining load-load dependences
 // these were put in only to ensure that dependences lost due to transitivity
 // through load-load deps are restored by closure
 for (alisti = 0; alisti < num_ops; alisti++) {
   if (alist2optype[alisti] == LOAD) {
     mri = alist2mr[alisti];
     Bitvector& bv = prof_memdep[mri];
     if (bv.is_allclear()) continue;
     for (Bit_iterator bi(bv); bi!=0; bi++) {
       int mrj = *(bi);
       int alistj = mr2alist[mrj];
       assert (alist2optype[alistj] == LOAD
	    || alist2optype[alistj] == STORE);
       if (alist2optype[alistj] == LOAD) bv.reset_bit(mrj);
     }
   }
 }
 if (dbg(ss, 6)) {
   cdbg << "Profiled dep bit matrix after removing load-load deps:" << endl;
   print_bit_matrix(prof_memdep);
 }

 // for transitively closed edges S->L or L->S,
 // where both S and L are in the same hb
 // replace edge S->L by S->Lx->L where Lx is external to hb
 for (alisti = 0; alisti < num_ops; alisti++) {
   if (alist2optype[alisti] != LOAD && alist2optype[alisti] != STORE)
     continue;
   Op* dest_op = (Op *) graph.b_map[alisti];
   mri = alist2mr[alisti];
   Bitvector& bvi = prof_memdep[mri];
   if (bvi.is_allclear()) continue;
   // warning: the bit Vector is modified while iterating over it
   // may not work when bit_iterator is optimized
   for (Bit_iterator bi(bvi); bi!=0; bi++) {
     int mrj = *(bi);
     int alistj = mr2alist[mrj];
     Op* src_op = (Op *) graph.b_map[alistj];     
     assert  (alist2optype[alistj] == LOAD || alist2optype[alistj] == STORE);
     int sd = mrj*num_mrops + mri;
     if (extop_map.is_bound(sd)) {
       assert (mr2blockid.value(mri) == mr2blockid.value(mrj));
       int mr_extop = extop_map.value(sd);
       assert (mr2blockid.value(mri) != mr2blockid.value(mr_extop));
       int alist_extop = mr2alist[mr_extop];
       assert (alist2optype[alist_extop] == LOAD);
       bvi.reset_bit(mrj);
       Op* ext_op = (Op *) graph.b_map[alist_extop];
       Bitvector& bve = prof_memdep[mr_extop];
       if (alist2optype[alistj] == LOAD) {
	 assert (alist2optype[alisti] == STORE);
	 assert (bvi.bit(mr_extop));
	 if (dbg (ss, 6)) {
	   cerr << "	Removing L->S edge from " << src_op->id() << " to "
	     << dest_op->id() << endl;
	 }

	 if (!bve.bit(mrj)) {
	   if (dbg(ss, 6)) {
	     cerr << "	Adding L->L edge from " << src_op->id() << " to "
		  << ext_op->id() << endl;
	   }
	   bve.set_bit(mrj);
	 }
       } else {
	 assert (alist2optype[alistj] == STORE);
	 assert (alist2optype[alisti] == LOAD);
	 assert (bve.bit(mrj));
	 if (dbg(ss, 6)) {
	   cerr << "	Removing S->L edge from " << src_op->id() << " to "
		<< dest_op->id() << endl;
	 }

	 if (!bvi.bit(mr_extop)) {
	   if (dbg(ss, 6)) {
	     cerr << "Adding L->L edge from " << ext_op->id() << " to "
		  << dest_op->id() << endl;
	   }
	   bvi.set_bit(mr_extop);
	 }
       }
     }
   }
 }
}

void
MprofSvars::insert_callret_deps ()
{
 // insert original dependencies to/from calls/returns (and other mem barriers)
 // Also, for each dependence eliminated through profiling
 //   insert deps from source of dep to all reachable calls/returns
 //    and from reaching calls/returns to dest of dep
 int mri, alisti;
 for (mri = 0; mri < num_mrops; mri++) {
   alisti = mr2alist[mri];
   assert (alist2optype[alisti] != NOT_MEMREF);
   // Op* op = (Op *) graph.b_map[alisti];
   Bitvector& bvo = orig_memdep[mri];
   // Bitvector& bvc = cl_orig_memdep[mri];
   Bitvector& bvp = prof_memdep[mri];
   // bvo(X->CR) => bvp(X->CR) where X is any op and CR is call/return op
   if (alist2optype[alisti] == CALLRET || 
       alist2optype[alisti] == OTHER_MEM_BARRIER) {
     bvp += bvo;
   }
   if (alist2optype[alisti] == LOAD || alist2optype[alisti] == STORE) {
     for (int mrj = 0; mrj < num_mrops; mrj++) {
       int alistj = mr2alist[mrj];
       // bvo(CRj->Mi) => bvp(CRj->Mi) where Mi is load/store op
       if ((alist2optype[alistj] == CALLRET || 
	     alist2optype[alistj] == OTHER_MEM_BARRIER) && bvo.bit(mrj)) 
	 bvp.set_bit(mrj);
       if (alist2optype[alistj] != LOAD && alist2optype[alistj] != STORE)
	 continue;
       // if (bvo(Xj->Mi) and (not bvp(Xj->Mi))) then
       if (bvo.bit(mrj) && (!bvp.bit(mrj))) {
         // for all bvc(Cii->Xj), set bvp(Cii->Mi)
	 for (List_iterator<int> lk (membarriers); lk != 0; lk++) {
	   int alistk = *(lk);
	   int mrk = alist2mr[alistk];
	   Bitvector& bvcj = cl_orig_memdep[mrj];
	   if (bvcj.bit(mrk)) bvp.set_bit(mrk);
	 }
	 // for all CRk, if bvc(Mi->CRk), set bvp(Xj->CRk)
	 for (List_iterator<int> li(membarriers); li != 0; li++) {
	   int alistk = *(li);
	   int mrk = alist2mr[alistk];
	   Bitvector& bvck = cl_orig_memdep[mrk];
	   if (bvck.bit(mri)) {
	     Bitvector& bvpk = prof_memdep[mrk];
	     bvpk.set_bit(mrj);
	   }
	 }
       }
     }
   }
 }
 if (dbg(ss, 6)) {
   cerr << "Profiled bit matrix with call/rts constraints" << endl;
   print_bit_matrix(prof_memdep);
 }
}


void
MprofSvars::read_prof_mem_and_analyze ()
{
 int iter_count = 0;
 read_orig_mem_deps();
 closure_orig_deps(iter_count); 
 read_prof_mem_deps();
 process_prof_deps();
 insert_callret_deps();
}

void
MprofSvars::set_memvrs ()
{
 // remove uses of memvrs from all memref'ing operations and
 // substitute with memvr uses if profiled mem dep info
 // indicate a dependence
 int alisti;
 for (alisti = 0; alisti < num_ops; alisti++) {
   if (alist2optype[alisti] != NOT_MEMREF) {
     int mri = alist2mr[alisti];
     Bitvector& bvp = prof_memdep[mri];
     Op* op = (Op *) graph.b_map[alisti];
     Hash_set<Operand> mem_oper_set (op->mem_srcs());
     for (Hash_set_iterator<Operand> oli(mem_oper_set); oli != 0; oli++) {
        op->remove_mem_src(*oli);
     }
     for (Bit_iterator bi(bvp); bi != 0; bi++) {
       int mrj = *(bi);
       int alistj = mr2alist[mrj];
       Op* src_op = (Op *) graph.b_map[alistj];
       for (Op_mem_dests moi(src_op); moi != 0; moi++) {
	 op->add_mem_src(*moi);
       }
     }
   }
 }
}
    
void
MprofSvars::print_bit_matrix(Vector<Bitvector>& bitmatrix)
{
 int alisti, mri;	// generic integer iterator

 // print out header info
 cdbg << "       ";	// seven spaces (5 for Lxxxx + 2 spaces)
 // print out letter code for type of op in top row
 for (mri = 0; mri < num_mrops; mri++) {
   int alisti = mr2alist[mri];
   Op* op = (Op* ) graph.b_map[alisti];
   if (alist2optype[alisti] == LOAD)  cdbg << "L";
   if (alist2optype[alisti] == STORE) cdbg << "S";
   if (alist2optype[alisti] == CALLRET) {
     if (is_brl(op))   cdbg << "C";
     if (is_rts(op))   cdbg << "R";
     assert(is_brl(op) || is_rts(op));
   }
   if (alist2optype[alisti] == OTHER_MEM_BARRIER) cdbg << "B";
 }
 cdbg << endl;

 // print out id of op in vertical format
 int div = 10000;
 for (int r = 0; r < 4; r++) {
   div = div/10;
   cdbg << "       ";	// seven spaces (5 for Lxxxx + 2 spaces)   
   for (mri = 0; mri < num_mrops; mri++) {
     int opid = mr2opid[mri];
     if ((opid/div) == 0) cdbg << " ";
     else {
       int digit = ((opid/div) % 10);
       cdbg << digit;
     }
   }
   cdbg << endl;
 }

 // print out one row per iteration
 for (mri = 0; mri < num_mrops; mri++) {
   alisti = mr2alist[mri];
   Bitvector& bv = bitmatrix[mri];
   // begin each row with letter code of op followed by four digit op id
   Op* op = (Op* ) graph.b_map[alisti];
   Memref_type dest_mrtype = alist2optype[alisti];
   switch (dest_mrtype) {
   case LOAD:
     {
       cdbg << "L"; break;
     }
   case STORE:
     {
       cdbg << "S"; break;
     }
   case CALLRET:
     {
       if (is_brl(op)) cdbg << "C";
       else {
	 assert (is_rts(op));
	 cdbg << "R";
       }
       break;
     }
   case OTHER_MEM_BARRIER:
     {
       cdbg << "B"; break;
     }
   default:
     {
     assert (is_memref(op)); assert(0);
     }
   }
   int opid = mr2opid[mri];
   cdbg.width(4); cdbg << opid << "  ";
   // print out 'letter' when a dependence exists, else blank, ' '
   // L->S F;	L->L L;	L->CR m;
   // S->S O;	S->L A;	S->CR t;
   // C->S s;	C->L l; C->C c;
   for (int mrj = 0; mrj < num_mrops; mrj++) {
     int alistj = mr2alist[mrj];
     if (bv.bit(mrj)) {
       Memref_type src_mrtype = alist2optype[alistj];
       switch (dest_mrtype) {
       case LOAD:
	 {
	   switch (src_mrtype) {
	   case STORE:
	     {
	       cdbg << "F"; break;
	     }
	   case LOAD:
	     {
	       cdbg << "L"; break;
	     }
	   case CALLRET:
	     {
	       cdbg << "m"; break;
	     }
	   default:
	     assert(0);
	   }
	   break;
	 }
       case STORE:
	 {
	   switch (src_mrtype) {
	   case STORE:
	     {
	       cdbg << "O"; break;
	     }
	   case LOAD:
	     {
	       cdbg << "A"; break;
	     }
	   case CALLRET:
	     {
	       cdbg << "t"; break;
	     }
	   default:
	     assert(0);
	   }
	   break;
	 }
       case CALLRET:
	 {
	   switch (src_mrtype) {
	   case STORE:
	     {
	       cdbg << "s"; break;
	     }
	   case LOAD:
	     {
	       cdbg << "l"; break;
	     }
	   case CALLRET:
	     {
	       cdbg << "c"; break;
	     }
	   default:
	     assert(0);
	   }
	   break;
	 }
       default:
	 {
	   assert (is_memref(op)); assert(0);
	 }
       }
     } else {
       cdbg << " ";
     }
   }
   cdbg << endl;
 }
}






