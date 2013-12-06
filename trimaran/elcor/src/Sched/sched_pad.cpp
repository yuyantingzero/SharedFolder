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
//      File:           sched_pad.cpp
//      Authors:        Santosh G. Abraham
//      Created:        July 1997
//      Description:    Padding for calls and returns
//
/////////////////////////////////////////////////////////////////////////////

#include "intf.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "dbg.h"
#include "meld.h"
#include "meld_propagate.h"
#include "meld_constraints.h"

bool region_has_call (Compound_region* region) {
  for (Region_ops_C0_order oi (region); oi != 0; oi++) {
    if (is_call(*oi) || is_rts(*oi)) return true;
  }
  return false;
}

bool region_has_rts (Compound_region* region) {
  for (Region_ops_C0_order oi (region); oi != 0; oi++) {
    if (is_rts(*oi)) return true;
  }
  return false;
}

void insert_noop_pad (Compound_region* region, Op* op, int pad_cycles) {

  for (Region_ops_C0_order oi (region, op); oi != 0; oi++) {
    int new_sched_time = (*oi)->sched_time() + pad_cycles;
    (*oi)->set_sched_time (new_sched_time);
  }
}

void meld_pad_rts (Compound_region* region) {

 int dangle = 0;
 MS_constraints* msc = get_ms_constraints (region);
 Region_entry_ops ei (region);
 Op* entry_op = (*ei);
 Latency_map* entry_import_def_map = msc->import_def_at_entry(entry_op);
 Latency_map* entry_import_use_map = msc->import_use_at_entry(entry_op);
   
 for (Latency_map_iterator di (*entry_import_def_map); di != 0; di++) {
   assert ((*di).second > 0);
   dangle = ELCOR_MAX (dangle, (*di).second);
 }
 for (Latency_map_iterator ui (*entry_import_use_map); ui != 0; ui++) {
   assert ((*ui).second > 0);
   dangle = ELCOR_MAX (dangle, (*ui).second);
 }
 if (dangle > 0)  {
   for (Region_ops_C0_order oi (region); oi != 0; oi++) {
     if (is_rts(*oi)) {
       insert_noop_pad (region, (*oi), dangle);
       return;
     }
   }
 }
}

void calc_dangles_and_pad_calls (Compound_region* region) {

 int dangle = 0;

 // if meld is on, initialize dangle to max incoming dangle
 if (has_ms_constraints(region)) {
   Region_entry_ops ei (region);
   Op* entry_op = (*ei);
   int cutoff_dist = MDES_max_op_lat;
   Map<Pair<Compound_region*, Op*>, Pair<int, bool> > dist_map;
   MS_constraints* msc = get_ms_constraints(region);
   Latency_map* entry_import_def_map = msc->import_def_at_entry(entry_op);
   Latency_map* entry_import_use_map = msc->import_use_at_entry(entry_op);
   min_dist_to_entry (region, cutoff_dist, dist_map);
   propagate_to_entry_lat_maps (region, dist_map,
			entry_import_def_map, entry_import_use_map);
   
   for (Latency_map_iterator di (*entry_import_def_map); di != 0; di++) {
     assert ((*di).second > 0);
     dangle = ELCOR_MAX (dangle, (*di).second);
   }
   for (Latency_map_iterator ui (*entry_import_use_map); ui != 0; ui++) {
     assert ((*ui).second > 0);
     dangle = ELCOR_MAX (dangle, (*ui).second);
   }
 }    

 bool padded = false;
 for (Region_ops_C0_order oi (region); oi != 0; oi++) {
   Op* curr_op = (*oi);
   if (is_call(curr_op)) {
     int pad_cycles = dangle - (curr_op->sched_time() + 1);
     if (pad_cycles > 0) {
       insert_noop_pad (region, curr_op, pad_cycles);
       padded = true;
     }
   }
   if (is_rts(curr_op)) {
     //int pad_cycles = (dangle + 1) - curr_op->sched_time();
     // why was there a +1? removed it to fix partial bypass problems. KF 2-2003
     int pad_cycles = dangle - curr_op->sched_time();
     if (pad_cycles > 0) {
       if (dbg(ss, 10)) {
	 cdbg << "Padding RTS op: " << curr_op->id() << " in region: " <<
	   region->id() << " with: " << pad_cycles << " cycles" << endl;
       }
       insert_noop_pad (region, curr_op, pad_cycles);
       padded = true;
     }
   }
   int sched_time = curr_op->sched_time();
   for (Op_all_inputs ii (curr_op); ii != 0; ii++) {
     Operand curr_operand = (*ii);
     Port_num src_p = ii.get_port_num();
     if (!(curr_operand.is_reg() || curr_operand.is_macro_reg())) continue;
     dangle = ELCOR_MAX (dangle, (sched_time + curr_op->anti_time (SRC, src_p)));
     // = sched_time + latest read time
   }
   for (Op_all_dests di (curr_op); di != 0; di++) {
     Operand curr_operand = (*di);
     Port_num dest_p = di.get_port_num();
     if (!(curr_operand.is_reg() || curr_operand.is_macro_reg())) continue;
     dangle = ELCOR_MAX (dangle, (sched_time + curr_op->flow_time (DEST, dest_p)));
     // = sched_time + latest write time
   }
 }
 // if doing meld and padding cycles are inserted,
 // readjust exported latency maps
 if ((has_ms_constraints(region)) && padded) {
   MS_constraints* msc = get_ms_constraints(region);
   // clear exit import maps, entry will be cleared in set_export_latency_maps
   for (Region_exit_ops exi (region); exi != 0; exi++) {
     Op* exit = *(exi);
     Latency_map* exit_import_def_map = msc->import_def_at_exit(exit);
     Latency_map* exit_import_use_map = msc->import_use_at_exit(exit);
     exit_import_def_map->clear();
     exit_import_use_map->clear();
   }
   set_export_latency_maps (region);
 }
}

void pad_calls_and_rts (Compound_region* f) {
  for(Region_subregions ri(f); ri != 0; ri++){
    Compound_region* region = (Compound_region*) (*ri);
    if (region->is_op()) continue;
    if (region->is_hb() || region->is_bb()) {
      if (region_has_call(region))
	calc_dangles_and_pad_calls(region);
    } else {
      for(Region_subregions ti(region); ti != 0; ti++){	
	Compound_region* subreg = (Compound_region*) (*ti);
	pad_calls_and_rts (subreg);
      }
    }
  }
}
