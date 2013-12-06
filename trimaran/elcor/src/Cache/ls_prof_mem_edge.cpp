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
//      File:           ls_prof_mem_edge.cpp
//      Authors:        Santosh G. Abraham
//      Created:        September 1995
//      Description:    Set weights of mem edges not seen during profiling
//				to zero
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "ls_prof_mem_edge.h"
#include "map.h"
#include "region_attributes.h"
#include "attributes.h"
#include "string.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "tuples.h"
#include "connect.h"
#include "ls_get_profile.h"
#include "op.h"

const int MAX_PROF_WEIGHT = 1000;
const int MIN_PROF_WEIGHT = 1;

// Sets profiled weights on store-load mem edges depending on aliases/exec_freq
// seen during profiling. All other mem edges set to MIN_PROF_WEIGHT.
void set_prof_weight_mem_edges(Compound_region* region) {
 
// map from store-id to (store-op-ptr, ith store in region) used to locate
// store from its store-id when setting/drawing edges into load from aliasing st
 Map<int, Pair<Op*, int> > st_map;
// list of other ops with outgoing mem edges, e.g. jsr
 List<Op*> gen_mem_ops; 
 int num_st = 0;	// number of stores seen so far
 int num_gen_mem = 0;	// non-store ops that are sources of mem deps, e.g. jsr
 for (Region_ops_C0_order oiter(region); oiter !=0; oiter++) {
   Op* op = (*oiter);
   if (is_store(op)) {
     num_st++;
     // bind store op ptr, num_st to store id in st_map
     int id = op->id();
     Pair <Op*, int>* p_op_int = new Pair<Op*, int> (op, num_st);
     st_map.bind(id, *p_op_int);
     // set profile weights for edges with  non-load dests to MIN_PROF_WEIGHT
     for (Op_outedges eiter(op, MEM, MEM0); eiter != 0; eiter++) {
       Edge* edge = (*eiter);
       if (!is_load(edge->dest())) edge->scratch_int = MIN_PROF_WEIGHT;
     }
     continue;
   }
   if (is_load(op)) {
     // need not do anything if you haven't seen sources of mem edges so far
     if ((num_st == 0) && (num_gen_mem == 0)) continue;
     // if you have seen prior stores
     if (num_st > 0) {
     // get aliasing info from profile annotations
     Map <int, int>* stid_aliases = get_mem_dep(op);
     int exec_freq = get_load_exec_freq(op);
     // iterate over incoming mem edges into load
     for (Op_inedges eiter (op, MEM, MEM0); eiter != 0; eiter++) {
       Edge* edge = (*eiter);
       // preliminary set to MIN_PROF+WEIGHT for edges with non-store sources,
       //  overriden later if needed for sourcing stores
       edge->scratch_int = MIN_PROF_WEIGHT;
       Op* src = edge->src();
       if (is_store(src)) {
	 // if store appears as aliasing,
	 // set weight based on ratio aliases/exec_freq
	 if (stid_aliases->is_bound(src->id())) {
	   int aliases = stid_aliases->value(src->id());
	   assert ((aliases > 0) && (aliases <= exec_freq));
	   edge->scratch_int =
		ELCOR_MAX(MIN_PROF_WEIGHT, (aliases*MAX_PROF_WEIGHT)/exec_freq);
	   stid_aliases->unbind(src->id());
	 } else {
           // if sourcing store does not alias, set edge weight to 0.
	   edge->scratch_int = 0;
	 }
       }
     }
     // if some aliasing stores' edges weren't seen (because of transitive
     // reduction of mem deps through chained stores) draw some more edges
     if (!(stid_aliases->is_empty())) {
       for (Map_iterator <int, int> liter(*stid_aliases); liter !=0; liter++) {
	 Pair <int, int> lcid_als = (*liter);
	 int lcid = lcid_als.first;
	 int aliases =  lcid_als.second;
	 assert ((aliases > 0) && (aliases <= exec_freq));
	 if (!st_map.is_bound(lcid)) continue;
	 Pair <Op*, int>  op_int = st_map.value(lcid);
	 Op* st_op = op_int.first;
	 Mem* mem = new Mem();
	 connect_ops(mem, st_op, MEM0, op, MEM0);
	 ((Edge *)mem)->set_latency();
	 ((Edge *)mem)->scratch_int =
		ELCOR_MAX(MIN_PROF_WEIGHT, (aliases*MAX_PROF_WEIGHT)/exec_freq);
       }
     }
     delete stid_aliases;
     }
     // we don't know for sure constraints from non-store ops to load
     // were eliminated through setting store-load weight to 0
     // so connect all prior non-stores to this load
     if (num_gen_mem > 0) {
       for (List_iterator<Op*> liter(gen_mem_ops); liter != 0; liter++) {
         Mem* mem = new Mem();
	 connect_ops(mem, (*liter), MEM0, op, MEM0);
	 ((Edge *)mem)->set_latency();
	 ((Edge *)mem)->scratch_int = MIN_PROF_WEIGHT;
       }
     }
     continue;
   }
// neither store nor load; if op sourcing a mem dep e.g., jsr,
//  need to make sure that all succeeding load ops are constrained by it
   Op_outedges eiter(op, MEM, MEM0);
   if (eiter != 0) {
     gen_mem_ops.add_tail(op);
     num_gen_mem++;
     while (eiter != 0) {
       (*eiter)->scratch_int = MIN_PROF_WEIGHT;
       eiter++;
     }
   }
 }
 // check if delete st_map is needed to prevent a memory leak
}    
