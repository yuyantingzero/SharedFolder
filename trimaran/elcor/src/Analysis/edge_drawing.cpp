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




///////////////////////////////////////////////////////////////////////////
//
//      File:          edge_drawing.cpp
//      Authors:       Richard Johnson 
//      Created:       March 1995 
//      Description:   Predicate-cognizant data dependence edge drawing 
//                     for regions.
//
///////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
// edge_drawing.cpp
//
//   predicate-cognizant data dependence edge drawing for regions.
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include "connect.h"
#include "region.h"
#include "iterators.h"
#include "tuples.h"
#include "attributes.h"
#include "region_attributes.h"
#include "opcode_properties.h"
#include "string_class.h"
#include "pqs.h"
#include "pred_interface.h"
#include "dfa.h"
#include "dbg.h"
#include "flow_analysis_solver.h"
#include "edge_drawing.h"
#include "el_ssched_init.h"
#include "edge_utilities.h"
#include "edge.h"
#include "hash_map.h"
#include "hash_set.h"
#include "hash_functions.h"
#include "opcode.h"
#include "meld.h"
#include "el_dot_tools.h"
#include "el_error.h"
#include "el_io.h"
#include "el_init.h"
#include "scalar_sched.h"

#include "core_edge_drawing.h"

#include "intf.h"

// flag to indicate which edges get drawn
// Uses:
//   - SCALAR and MELD edge drawing supresses ALL intra-op edges,
//     including edges from a branch to its related switch ops.
//
//   - MODULO edge drawing threads branches but omits full C1 edges.
//

 void add_additional_edges(Compound_region* r);


 void
insert_block_sw_mg_nodes_meld (Compound_region* blk)
{
  assert (edge_style == MELD_EDGES);

  VR_map* vr_map = get_local_vr_map (blk);
  assert(vr_map != NULL);
  Hash_map<Operand,int>& vr_to_index = vr_map->vr_to_index;
  
  Region_entry_ops entry(blk);
  Op* entry_op = *entry;    // get the top entry op
  assert (is_control_merge(entry_op));

  assert(has_ms_constraints(blk));
  MS_constraints *ms = get_ms_constraints(blk);

  Hash_set<Operand> meld_mg_defs(hash_operand, 1021);
  Hash_set<Operand> meld_mg_uses(hash_operand, 1021);
  Hash_set<Operand> meld_sw_defs(hash_operand, 1021);
  Hash_set<Operand> meld_sw_uses(hash_operand, 1021);

  // NOTES:
  // no mem_vr's in meld sets
  //
  // mg def: added if meld_mg_def ^ (meld_sw_ref + local_ref)
  // mg use: added if meld_mg_use ^ (meld_sw_def + local_ref)
  //                                                     ^^^ local_def better.
  //
  // sw def: added if meld_sw_def ^ (meld_mg_ref + local_ref)
  // sw use: added if meld_sw_use ^ (meld_mg_def + local_ref)
  //                                                     ^^^ local_def better.
  // OVERVIEW:
  // - get candidates for merge def/use nodes
  // - process branch ops, inserting switch def/use nodes and collecting
  //   sets of switch defs/uses.
  // - process merge op, inserting merge def/use nodes
  //   (using switch def/use sets)

  List<Operand>* dmlist = ms->def_merges_at_entry (entry_op);
  for (List_iterator<Operand> li(*dmlist); li != 0; li++) {
    Operand oper = *li;
    assert (!oper.is_mem_vr());
    meld_mg_defs += oper;
  }
  List<Operand>* umlist = ms->use_merges_at_entry(entry_op);
  for (List_iterator<Operand> ui(*umlist); ui != 0; ui++) {
    Operand oper = *ui;
    assert (!oper.is_mem_vr());
    meld_mg_uses += oper;
  }

  Op* last_op = get_last_region_op_from_subregions(blk);
  assert (is_branch(last_op));
  for (Region_exit_ops exit(blk); exit != 0; exit++) {
    Op* exit_op = *exit;
    assert (is_branch(exit_op));

    Hash_set<Operand> sw_defs(hash_operand, 1021);
    Hash_set<Operand> sw_uses(hash_operand, 1021);

    MS_constraints *ms = get_ms_constraints(blk);
    List<Operand>* dslist = ms->def_switches_at_exit (exit_op);
    for (List_iterator<Operand> li(*dslist); li != 0; li++) {
      Operand oper = *li;
      sw_defs += oper;
    }

    for (Op_outedges oe(exit_op, CONTROL0_OUTEDGES); oe != 0; oe++) {
      // get info for each edge (only exit edges have info)
      Edge* ed = (Edge*) (*oe);

      Liveness_info_iterator li ;
      Liveness_info* uu = get_liveness_info(ed);
      if (uu) {
	for (li(*uu) ; li != 0 ; li++) {
	  Operand oper = *li ;
	  sw_uses += oper;
	}
      }
    }

    //
    // add switch def/use nodes
    //

    // sw def: added if sw_def ^ (meld_mg_ref + local_ref)
    Hash_set_iterator<Operand> sw;
    for (sw(sw_defs); sw != 0; sw++) {
      Operand oper = *sw;

      if (oper.is_mem_vr()) continue;  // only flow deps for mem_vr's

      if (meld_mg_defs.is_member(oper) || meld_mg_uses.is_member(oper) ||
	  vr_to_index.is_bound(oper)) {
	Op* new_sw = new Op(D_SWITCH,1,1,0) ;
	new_sw->dest(DEST1) = oper ;  // def
	add_switch_to_control_switch(exit_op, new_sw) ;
	meld_sw_defs += oper;
      }
    }
    
    // sw use: added if sw_use ^ (meld_mg_def + local_ref)
    for (sw(sw_uses); sw != 0; sw++) {
      Operand oper = *sw;
      if (meld_mg_defs.is_member(oper) || vr_to_index.is_bound(oper)) {
	Op* new_sw = new Op(D_SWITCH,1,1,0) ;
	if (oper.is_mem_vr()) {
	  new_sw->add_mem_src(oper);
	} else {
	  new_sw->src(SRC1) = oper ;  // use
	}
	add_switch_to_control_switch(exit_op, new_sw) ;
	meld_sw_uses += oper;
      }
    }
  }

  //
  // finally, add merge def/use nodes
  //

  // mg def: added if meld_mg_def ^ (meld_sw_ref + local_ref)
  Hash_set_iterator<Operand> mg;
  for (mg(meld_mg_defs); mg != 0; mg++) {
    Operand oper = *mg;
    if (meld_sw_defs.is_member(oper) || meld_sw_uses.is_member(oper) ||
	vr_to_index.is_bound(oper)) {
      Op* new_mg = new Op(D_MERGE,1,1,0) ;
      if (oper.is_mem_vr()) {
	new_mg->add_mem_dest(oper);
      } else {
	new_mg->dest(DEST1) = oper ;  // def
      }
      add_merge_to_control_merge(entry_op, new_mg) ;
    }
  }

  // mg use: added if meld_mg_use ^ (meld_sw_def + local_ref)
  for (mg(meld_mg_uses); mg != 0; mg++) {
    Operand oper = *mg;

    if (oper.is_mem_vr()) continue;  // only flow deps for mem_vr's

    if (meld_sw_defs.is_member(oper) || vr_to_index.is_bound(oper)) {
      Op* new_mg = new Op(D_MERGE,1,1,0) ;
      new_mg->src(SRC1) = oper ;  // use
      add_merge_to_control_merge(entry_op, new_mg) ;
    }
  }
}
// assumes a subregion tiling of BBs/HBs
 void
insert_region_meld_edges (Compound_region* r)
{
  if (dbg(sed)) cdbg << "SED: Region " << r->id() << endl;

  edge_style = MELD_EDGES;
  insert_region_sw_mg_nodes (r);
  insert_region_intrablock_edges (r);
  insert_region_interblock_edges (r, r);
}


// end of edge_drawing.cpp
