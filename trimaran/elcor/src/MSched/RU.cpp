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
//      File:           RU.cpp
//      Authors:        Joel Jones, Vinod Kathail, Shail Aditya
//      Created:        June 1995
//      Description:    Resource Usage Interface to Modulo-Scheduler
//
/////////////////////////////////////////////////////////////////////////////

#include "RU.h"
#include "mdes.h"
#include "opcode.h"
#include "string_class.h"
#include "attributes.h"
#include "edge_utilities.h"

extern ModuloScheduler *msched_glb;

static bool ok_to_schedule(Op *op, char *sched_op, int stime) {
  return true;
}

RU::RU(int length){
  RU_alloc_map(length);
}




void RU::init(Mode mode, int cycles){
  RU_init_map(mode == LOOP ? 1 : 0, cycles);
}

RU::~RU(){
  RU_delete_map();
}

void RU::get_nonconflicting_patterns(Op *node, int time, 
				     List<pattern> *patterns){
  char iopat[MAXIOPATLEN];
  char *specific_op;
  pattern newPattern;
  int priority;
  List<char *> nonconfl_alt_opcodes;
  List<int> priority_list;
  List_iterator<int> pi;
  
  node->build_io_pat(iopat);
  RU_init_iterator(get_mdes_opcode_string(node), node, iopat, time);
  while(RU_get_next_nonconfl_alt(&specific_op, &priority)) {
   // cdbg << "NONCONF for op " << node->id()  << "  " << specific_op << endl;
   nonconfl_alt_opcodes.add_tail(specific_op);
   priority_list.add_tail(priority);
  }

  List_iterator<char*> li(nonconfl_alt_opcodes); 
  /*
  for(li(nonconfl_alt_opcodes); li != 0; li++)
  {
   cdbg << "NONCONF for op " << node->id()  << "  " << (*li) << endl;
   if(node->id()==50) {
    for (Op_inedges_rel ei1(node, FLOW_EDGES); ei1 != 0; ei1++) {
      Edge *edge = (*ei1);
      if(edge->src() != edge->dest()) continue;

      assert(edge->src() == edge->dest());
      int effstime = time - edge->scratch_int * msched_glb->II();
      // cdbg << "OKTOSCHED: " << ok_to_schedule_src_dest((*li), (*li), edge,
      //                         effstime , time) << endl;
    }
   }
   
  }
  */

  pi(priority_list);
  for(li(nonconfl_alt_opcodes); li != 0; li++, pi++) {
    specific_op = (*li);
    priority = (*pi);
    if(ok_to_schedule(node, specific_op, time)) {
      newPattern = new pattern_struct;
      // directly use mdes opcode (consistent with scalar sched) 
      // -- Shail Aditya 08/06/97
      // newPattern->op = new char[strlen(specific_op)+1];
      // strcpy(newPattern->op, specific_op);
      newPattern->op = specific_op;
      newPattern->priority = priority;
      patterns->add_tail(newPattern);

      // parkhc 04/30/2004
      // caller of this function uses only head node in the list.
      // so, make it return as soon as it finds one pattern.
      //return;

    }
  }
}

void RU::get_conflicting_Ops(Op *node, char *specific_op, 
			     int time, Hash_set<Op*>& ops){
  char iopat[MAXIOPATLEN];

  node->build_io_pat(iopat);
  RU_init_iterator(specific_op, node, iopat, time);
  RU_get_conflicting_ops((Hash_set<void*>&)ops);
}

void RU::remove(Op *node, int sched_time){
  char iopat[MAXIOPATLEN];

  node->build_io_pat(iopat);
  //RU_remove(el_opcode_to_string_map->value(node->opcode()), iopat, sched_time);
  RU_remove(node, iopat, sched_time);
}

bool RU::place(Op* node, char *specific_opcode, int res_index, int time){
  char iopat[MAXIOPATLEN], *potential_opcode;
  bool found = false;
  int potential_index;

  node->build_io_pat(iopat);
  // RU_init_iterator(el_opcode_to_string_map.value(node->opcode()), node, iopat, time);
  RU_init_iterator(get_mdes_opcode_string(node), node, iopat, time);
  while(RU_get_next_nonconfl_alt(&potential_opcode, &potential_index)) {
    if (!strcmp(specific_opcode, potential_opcode) 
        && (potential_index == res_index)) {
      RU_place();
      found = true;
      break;
    }
  }
  return found;
}

// For reconstructing RUmap
bool RU::place(Op* node, char *specific_opcode, int time){
  char iopat[MAXIOPATLEN], *potential_opcode;
  bool found = false;
  int potential_index;

  node->build_io_pat(iopat);
  // RU_init_iterator(el_opcode_to_string_map.value(node->opcode()), node, iopat, time);
  RU_init_iterator(get_mdes_opcode_string(node), node, iopat, time);
  while(RU_get_next_nonconfl_alt(&potential_opcode, &potential_index)) {
    if (!strcmp(specific_opcode, potential_opcode)){
      RU_place();
      found = true;
      break;
    }
  }
  return found;
}

Op* RU::at(int cycle, int res_index){
  return (Op *) RU_at(cycle, res_index);
}
