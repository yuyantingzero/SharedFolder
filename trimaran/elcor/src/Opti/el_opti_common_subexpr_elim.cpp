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
//									 //
//      File:           el_opti_common_subexpr_elim.cpp                  //
//      Authors:        Matt Jennings, Sadun Anik        		 //
//      Created:        June 1997        				 //
//      Description:    Loop Invariant Code Removal interface		 //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "dbg.h"
#include "iterators.h"
#include "pred_analysis.h"
#include "el_opti_common_subexpr_elim.h" 
#include "el_available_expression.h"
#include "opcode_properties.h"
#include "el_control.h"
#include "el_bb_tools.h"
#include "hash_map.h"
#include "hash_functions.h"
#include "el_main.h"
#include "ir_writer.h"

using namespace std;

static bool is_CSE_opcode(Op* op_ptr) 
{
  // Exclude opcodes which are not suitable for CSE removal

  // Use global flag to determine if PBR instructions should be 
  // included in CSE removal 
  if (El_include_pbr_during_CSE) {
    if ( !(is_control_switch(op_ptr) ||
	   is_control_merge(op_ptr) ||
           is_store(op_ptr) ||
	   is_cmpp(op_ptr) ||
	   is_remap(op_ptr) ||
	   is_predicate_set(op_ptr) ||
	   is_predicate_clear(op_ptr) ||
	   is_clear_all_rotating(op_ptr) ||
	   is_define(op_ptr) ||
	   (is_move(op_ptr) && !is_pbr(op_ptr))) )
      return true ;
    else return false ;
  }
  else {
    if ( !(is_control_switch(op_ptr) ||
	   is_control_merge(op_ptr) ||
           is_store(op_ptr) ||
	   is_cmpp(op_ptr) ||
	   is_remap(op_ptr) ||
	   is_predicate_set(op_ptr) ||
	   is_predicate_clear(op_ptr) ||
	   is_clear_all_rotating(op_ptr) ||
	   is_define(op_ptr) ||
	   is_move(op_ptr)) )
      return true ;
    else return false ;
  }
}

void el_opti_common_subexpr_eliminate(Compound_region* R) 
{
  if (dbg(opti, 1) || dbg(status, 3)) 
    cdbg << "-- Global common subexpression elimination " << endl;
  if (dbg(opti, 2)) {
    cdbg << "****** Printing Rebel before CSE" << endl;
    IR_outstream dbg_out("REB_before_CSE");
    ir_write(dbg_out, R);
  }

  create_local_analysis_info_for_all_hbs_bbs(R) ;
  
  // Use available expressions to identify common subexpressions 
  // and to check redundancy to determine which can be eliminated
  Available_expression_solver avexp(R, AV_EXPR_BINARY) ;
  avexp.solve() ;
  Hash_map<Op*, bool> was_op_fully_redundant(hash_op_ptr);
  Hash_map<int, bool> any_fully_redundant(hash_int);
  Hash_map<int, Operand> first_temp(hash_int);
  Hash_map<int, Operand> last_temp(hash_int);
  List<int> exp_list = avexp.get_expression_list();

  // Pass One: Determine redundancy and create temporary registers to prepare for 
  // CSE where redundancy exists. 
  // Iterate over each expression in the Compound Region
  for (List_iterator<int> exp_iter(exp_list); exp_iter != 0; exp_iter++) {
    int cur_exp = *exp_iter;
    any_fully_redundant.bind(cur_exp, false);
    bool temps_created = false;
//    cerr<<"iterating exp is : "<<cur_exp<<"\n";
    // Iterate over each operation of a common subexpression
    List<Op*>& ops_for_exp = avexp.get_ops_for_expression(cur_exp) ;
//    cerr<<"iterating ops are : \n";
    for (List_iterator<Op*> ops_for_exp_iter(ops_for_exp); ops_for_exp_iter != 0; ops_for_exp_iter++) {
      Op* cur_op = *ops_for_exp_iter; 
//      cerr<<*cur_op<<"\n";
      // Do CSE removal if opcode is eligible
      if (is_CSE_opcode(cur_op)) {
	was_op_fully_redundant.bind(cur_op, avexp.is_op_fully_redundant(cur_op));
      	if (was_op_fully_redundant.value(cur_op)) {
	   if (dbg(opti,3))
	      cdbg<<"\top "<<cur_op->id()<<" is fully redundant."<<endl;
	  // if any operations of a common subexpression are fully redundant, temps are 
	  // created to allow removal of this and any other fully redundant expressions 
	  // with the same common subexpression. Only need to create one set of temps 
	  // for each common subexpression
      	  if (!temps_created) {
	    //   create unique temp(s)
      	    Port_num first_port = cur_op->first_dest();
      	    Operand first_dest_operand = cur_op->dest(first_port);
	    Operand first_Reg_temp = new Reg(first_dest_operand.dtype());
	    first_temp.bind(cur_exp, first_Reg_temp);
      	    
      	    if (cur_op->num_dests() == 2) {
	    Port_num last_port = cur_op->last_dest();
	    Operand last_dest_operand = cur_op->dest(last_port);
	    Operand last_Reg_temp = new Reg(last_dest_operand.dtype());
	    last_temp.bind(cur_exp, last_Reg_temp);	  
	    }
      	    temps_created = true;
      	  }
	  any_fully_redundant.value(cur_exp) = true;
      	}
      }
    }
  }

  // Pass Two: perform CSE on expressions for which Pass One showed any redundancy
  // Iterate over each expression in the Compound Region
  for (List_iterator<int> exp_iter2(exp_list); exp_iter2 != 0; exp_iter2++) {
    int cur_exp = *exp_iter2;
//    cerr<<"any_fully_redundant = "<<any_fully_redundant.value(cur_exp)<<" for exp "<<cur_exp<<"\n";
    // Perform CSE for this expression if any redundancy found in Pass One
    if (any_fully_redundant.value(cur_exp)) {
      if (dbg(opti,4)) cdbg<<"    ****Current redundant expression id "<<cur_exp<<endl;
      // Iterate over each operation of a common subexpression
      List<Op*>& ops_for_exp = avexp.get_ops_for_expression(cur_exp) ;
      List_iterator<Op*> ops_for_exp_iter(ops_for_exp); 
      while (ops_for_exp_iter != 0) {
	Op* cur_op = *ops_for_exp_iter; 
        if (dbg(opti,2)) cdbg<<"\tConsidering cs op "<<cur_op->id()<<endl;
	if (dbg(opti,4)) cdbg<<*cur_op<<endl;
	assert(is_CSE_opcode(cur_op) == true) ;
	ops_for_exp_iter++;
	// Insert a move operation for all operations with a common subexpression 
	// For operations that are fully redundant, the original operation will 
	// later be removed. Local copy propagation and dead-code elimination will
	// then be used to eliminate these move operations. 	
	// cerr<<"Move(s) inserted after operation for expression : "<<cur_exp<<"\n";
	Opcode first_move_opcode = get_move_opcode_for_operand(first_temp.value(cur_exp));
	Op* first_move_op = new Op(first_move_opcode);
	Port_num first_port = cur_op->first_dest();
	first_move_op->set_dest(DEST1, cur_op->dest(first_port));
	first_move_op->set_src(SRC1, first_temp.value(cur_exp));
	first_move_op->set_src(PRED1, cur_op->src(PRED1)) ;
	El_insert_op_after(first_move_op, cur_op);
	if (dbg(opti,2)) 
	   cdbg<<"\t  inserting move op "<<first_move_op->id()
	       <<" after op "<<cur_op->id()<<endl;
	
	if (cur_op->num_dests() == 2) {
	  Opcode last_move_opcode = get_move_opcode_for_operand(last_temp.value(cur_exp));
	  Op* last_move_op = new Op(last_move_opcode);
	  Port_num last_port = cur_op->last_dest();
	  last_move_op->set_dest(DEST1, cur_op->dest(last_port));
	  last_move_op->set_src(SRC1, last_temp.value(cur_exp));
	  last_move_op->set_src(PRED1, cur_op->src(PRED1)) ;
	  El_insert_op_after(last_move_op, first_move_op);
	  if (dbg(opti,2))
	     cdbg<<"\t  inserting move op "<<last_move_op->id()
		 <<" after op "<<first_move_op->id()<<endl;	}
	// Original operation is no longer needed if it was fully redundant
	if (was_op_fully_redundant.value(cur_op)) {
	   if (dbg(opti,2)) 
	      cdbg<<"\t  removing fully redundant op "<<cur_op->id()<<endl;
           // parkhc TEST
	   delete El_remove_op(cur_op);
	}
	// If original operation was not fully redundant, 
	// it's destination(s) must be modifed to the new temp(s)
       	else {
	   if (dbg(opti,2))
	      cdbg<<"\t  updating destinations of op "<<cur_op->id()<<endl;
	  Port_num first_port = cur_op->first_dest();
	  Port_num last_port = cur_op->last_dest();
	  // replace all these destination(s) with the unique temp(s)
//	  cerr<<"Destination(s) modified for non-fully redundant expression : "<<cur_exp<<"\n";
	  //Op_complete_dests dest_iter(cur_op); 
          // pretty sure we don't want complete_dests. the rest of the code
          // seems to assume only the 1 or 2 explicit dests. -KF 12/2004
          Op_explicit_dests dest_iter(cur_op);
          while (dest_iter != 0) {
	    El_ref def = dest_iter.get_ref();
 	    Port_num def_port = def.get_port_num();
	    dest_iter++;
	    if (def_port == first_port) {
	      cur_op->set_dest(first_port, first_temp.value(cur_exp));
	    }
	    else {
	      cur_op->set_dest(last_port, last_temp.value(cur_exp));
	    }
	  }
	}
      }
    }      
  }
  delete_local_analysis_info_for_all_hbs_bbs(R) ;
}


