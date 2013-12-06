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

Copyright 2007 University of Michigan

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




#include <iostream>

#include "region.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "op.h"
#include "opcode_load_store.h"
#include "intf.h"

#include "peephole_opti.h"
#include "util.h"


using namespace std;


void
combine_ops_to_primitives(Region* reg, Hash_map<Op*, string>& op_to_output_string)
{
  // It is assumed that 'reg' is a single block !
  assert(reg->is_bb() || reg->is_hb());


  ////////////////////////////////////////////////////////////////////////////
  // Compress the callee save code into load/store multiples
  //
  // There's three types of callee save code:
  // 1. base + offset stores
  // 2. base + offset loads
  // 3. moves for when the offset is too big.
  // We'll assume that the saves and restores are contiguous in memory,
  // so we need to find the lowest address written-to/read-from, the list
  // of registers to be stored/loaded, and we need to remove all of those
  // ops.

  if(reg->flag(EL_REGION_HAS_PROLOGUE_OP)) {
    int lowest_address = ELCOR_MAX_INT;
    Op* last_store = NULL;
    List<int> regs_to_store;
    bool has_a_label_load = false;

    for(Region_all_ops rops((Compound_region*)reg); rops != 0; rops++) {
      Op* op = *rops;
      if(op->flag(EL_OPER_CALLEE_SAVE)) {

	// Ignore the icmove register save/restore. These go to a
	// special stack location, therefore can't be part of the STM/LDM.
	if(is_base_plus_offset_store(op) && op->src(SRC3).is_macro_reg() &&
	   op->src(SRC3).name() == ICMOVE_REG)
	  continue;

	if(is_move(op)) {
	  if(op->src(SRC1).is_int()) {
	    if(op->src(SRC1).int_value() < lowest_address)
	      lowest_address = op->src(SRC1).int_value();
	  } else if(op->src(SRC1).is_label()) {
	    has_a_label_load = true;
	  } else {
	    cerr << "I don't understand callee save code that has a move without an int!\n"
		 << *op << endl;
	    exit(1);
	  }

	  // This should effectively remove the op from the code.
	  if( ! op->src(SRC1).is_label() && ! has_a_label_load)
	    op_to_output_string.bind(op, string(""));
	}

	if(is_base_plus_offset_store(op)) {
	  if(op->opcode() == (Opcode)SAVEG) {
	    last_store = op;
	    if(op->src(SRC2).is_int()) {
	      if(op->src(SRC2).int_value() < lowest_address)
		lowest_address = op->src(SRC2).int_value();
	    }
	    regs_to_store.add_head(op->src(SRC3).mc_num());
	    if( ! has_a_label_load)
	      op_to_output_string.bind(op, string(""));
	  }
	}

      } // if callee_save
    } // for all ops

    // If the callee save code uses a label, then we don't know how to
    // find the address to start the STM at.
    if( ! has_a_label_load) {
      string new_code_sequence = "";
      if(lowest_address > 0) {
	char laddr[80];
	sprintf(laddr, "%d", lowest_address);
	if( ! is_valid_dp_const(lowest_address)) {
	  new_code_sequence += "\tldr  r11, =";
	  new_code_sequence += laddr;
	  new_code_sequence += "\n\tadd  r11, r11, sp\n";
	} else {
	  new_code_sequence += "\tadd  r11, sp, #";
	  new_code_sequence += laddr;
	  new_code_sequence += '\n';
	}
	new_code_sequence += "\tstmia  r11, {";
      } else {
	new_code_sequence += "\tstmia  sp, {";
      }
      bool first_trip = true;
      for(List_iterator<int> li(regs_to_store); li != 0; li++) {
	int the_reg = *li;
	if(first_trip) {
	  first_trip = false;
	} else {
	  new_code_sequence += ',';
	}

	char reg[80];
	sprintf(reg, " r%d", the_reg);
	new_code_sequence += reg;
      }
      new_code_sequence += " }\n";
      if(op_to_output_string.is_bound(last_store)) {
	op_to_output_string.unbind(last_store);
      }
      op_to_output_string.bind(last_store, new_code_sequence);
    }

  } // if prologue region



  if(reg->flag(EL_REGION_HAS_EPILOGUE_OP)) {
    int lowest_address = ELCOR_MAX_INT;
    Op* last_load = NULL;
    List<int> regs_to_load;
    bool has_a_label_load = false;

    for(Region_all_ops rops((Compound_region*)reg); rops != 0; rops++) {
      Op* op = *rops;
      if(op->flag(EL_OPER_CALLEE_SAVE)) {

	// Ignore the icmove register save/restore. These go to a
	// special stack location, therefore can't be part of the STM/LDM.
	if(is_base_plus_offset_load(op) && op->dest(DEST1).is_macro_reg() &&
	   op->dest(DEST1).name() == ICMOVE_REG)
	  continue;

	if(is_move(op)) {
	  if(op->src(SRC1).is_int()) {
	    if(op->src(SRC1).int_value() < lowest_address)
	      lowest_address = op->src(SRC1).int_value();
	  } else if(op->src(SRC1).is_label()) {
	    has_a_label_load = true;
	  } else {
	    cerr << "I don't understand callee save code that has a move without an int!\n"
		 << *op << endl;
	    exit(1);
	  }

	  // This should effectively remove the op from the code.
	  if( ! op->src(SRC1).is_label() && ! has_a_label_load)
	    op_to_output_string.bind(op, string(""));
	}

	if(is_base_plus_offset_load(op)) {
	  // FIX: what to do about float...
	  if(op->opcode() == (Opcode)RESTOREG) {
	    last_load = op;
	    if(op->src(SRC2).is_int()) {
	      if(op->src(SRC2).int_value() < lowest_address)
		lowest_address = op->src(SRC2).int_value();
	    }
	    regs_to_load.add_tail(op->dest(DEST1).mc_num());
	    if( ! has_a_label_load)
	      op_to_output_string.bind(op, string(""));
	  }
	}

      } // if callee_save
    } // for all ops

    if( ! has_a_label_load) {
      string new_code_sequence = "";
      if(lowest_address > 0) {
	char laddr[80];
	sprintf(laddr, "%d", lowest_address);
	if(lowest_address > 255) {
	  new_code_sequence += "\tldr  r11, =";
	  new_code_sequence += laddr;
	  new_code_sequence += "\n\tadd  r11, r11, sp\n";
	} else {
	  new_code_sequence += "\tadd  r11, sp, #";
	  new_code_sequence += laddr;
	  new_code_sequence += '\n';
	}
	new_code_sequence += "\tldmia  r11, {";
      } else {
	new_code_sequence += "\tldmia  sp, {";
      }
      bool first_trip = true;
      for(List_iterator<int> li(regs_to_load); li != 0; li++) {
	int the_reg = *li;
	if(first_trip) {
	  first_trip = false;
	} else {
	  new_code_sequence += ',';
	}

	char reg[80];
	sprintf(reg, " r%d", the_reg);
	new_code_sequence += reg;
      }
      new_code_sequence += " }\n";
      if(op_to_output_string.is_bound(last_load)) {
	op_to_output_string.unbind(last_load);
      }
      op_to_output_string.bind(last_load, new_code_sequence);
    }
  } // if epilogue region


  // We could do a peephole for caller save code, but it never comes
  // up in the current compiler. r0-r3 are caller save, but those are
  // parameter macros and the register allocator guarantees those are
  // bound first, meaning they will never have a useful value at a
  // function call boundary. r4-r10 are all callee saved, and the rest
  // of the registers are never used, or handled in a special way
  // (r11). -ntclark 10/6/05


  ////////////////////////////////////////////////////////////////////////////
  // Combine shift instructions that can be part of the data
  // processing instructions.
  //
  // FIX: there's absolutely no reason to do this optimization
  // here. it should be done using the pattern matcher in elcor prior
  // to register allocation. i'm only doing it here out of shear
  // laziness, and frustration at difficulty in making the pattern
  // matcher work right now.

  Hash_set<Op*> already_combined(hash_op_ptr, 37);
  for(Region_all_ops rops((Compound_region*)reg); rops != 0; rops++) {
    Op* op = *rops;
    
    if(is_int_lshift(op)) {
      Op* dest = NULL;
      bool ok_to_replace = true;
      bool need_to_switch_operands = false;
      for(Op_outedges oe (op, FLOW_EDGES); oe != 0; oe++) {
	if(dest) {
	  ok_to_replace = false;
	}
	dest = (*oe)->dest();
	if((*oe)->dest_port() != SRC2) {
	  if(MDES_commutative(el_opcode_to_string_map[dest->opcode()]) &&
	     (dest->src(SRC2).is_reg() || dest->src(SRC2).is_macro_reg())) {
	    need_to_switch_operands = true;
	  } else {
	    ok_to_replace = false;
	  }
	}
      }
      if(dest && ! already_combined.is_member(dest)) {
	if(is_dp_op(dest)) {
	  if(op->src(SRC2).is_reg() || op->src(SRC2).is_macro_reg()) {
	    // Make sure src1 & src2 aren't overwritten between the shift and DP op
	    for(Region_ops_C0_order ri((Compound_region*)reg, op); ri != 0; ri++) {
	      Op* betw_op = *ri;
	      if(betw_op == op)
		continue;
	      if(betw_op == dest)
		break;
	      for(Op_all_dests di(betw_op); di != 0; di++) {
		if(*di == op->src(SRC1) || *di == op->src(SRC2))
		  ok_to_replace = false;
	      }
	    }
	  } else if(op->src(SRC2).is_int() && is_valid_sl_const(op->src(SRC2).int_value())) {
	    // Make sure src1 isn't overwritten between the shift and DP op
	    for(Region_ops_C0_order ri((Compound_region*)reg, op); ri != 0; ri++) {
	      Op* betw_op = *ri;
	      if(betw_op == op)
		continue;
	      if(betw_op == dest)
		break;
	      for(Op_all_dests di(betw_op); di != 0; di++) {
		if(*di == op->src(SRC1))
		  ok_to_replace = false;
	      }
	    }
	  } else {
	    ok_to_replace = false;
	  }

	  if(ok_to_replace) {
	    // get rid of the shift
	    op_to_output_string.bind(op, string(""));

	    if(need_to_switch_operands) {
	      Operand tmp = dest->src(SRC1);
	      dest->set_src(SRC1, dest->src(SRC2));
	      dest->set_src(SRC2, tmp);
	    }

	    already_combined += dest;

	    // generate code for the combined shift+dp op
	    string new_code = get_dp_opc(dest);
	    output_register(dest->dest(DEST1), new_code);
	    new_code += ", ";
	    output_register(dest->src(SRC1), new_code);
	    new_code += ", ";
	    output_register(op->src(SRC1), new_code);
	    new_code += ", LSL ";
	    output_register(op->src(SRC2), new_code);
	    new_code += "\n";
	    op_to_output_string.bind(dest, new_code);
	  }
	}
      }
    } // if is lshift

    if(is_int_rshift(op)) {
      Op* dest = NULL;
      bool ok_to_replace = true;
      bool need_to_switch_operands = false;
      for(Op_outedges oe (op, FLOW_EDGES); oe != 0; oe++) {

	if(op_to_output_string.is_bound((*oe)->dest()))
	  continue;

	if(dest) {
	  ok_to_replace = false;
	}
	dest = (*oe)->dest();
	if((*oe)->dest_port() != SRC2) {
	  if(MDES_commutative(el_opcode_to_string_map[dest->opcode()]) &&
	     (dest->src(SRC2).is_reg() || dest->src(SRC2).is_macro_reg())) {
	    need_to_switch_operands = true;
	  } else {
	    ok_to_replace = false;
	  }
	}
      }
      if(dest && ! already_combined.is_member(dest)) {
	if(is_dp_op(dest)) {
	  if(op->src(SRC2).is_reg() || op->src(SRC2).is_macro_reg()) {
	    // Make sure src1 & src2 aren't overwritten between the shift and DP op
	    for(Region_ops_C0_order ri((Compound_region*)reg, op); ri != 0; ri++) {
	      Op* betw_op = *ri;
	      if(betw_op == op)
		continue;
	      if(betw_op == dest)
		break;
	      for(Op_all_dests di(betw_op); di != 0; di++) {
		if(*di == op->src(SRC1) || *di == op->src(SRC2)) {
		  ok_to_replace = false;
		}
	      }
	    }
	  } else if(op->src(SRC2).is_int() && is_valid_sr_const(op->src(SRC2).int_value())) {
	    // Make sure src1 isn't overwritten between the shift and DP op
	    for(Region_ops_C0_order ri((Compound_region*)reg, op); ri != 0; ri++) {
	      Op* betw_op = *ri;
	      if(betw_op == op)
		continue;
	      if(betw_op == dest)
		break;
	      for(Op_all_dests di(betw_op); di != 0; di++) {
		if(*di == op->src(SRC1)) {
		  ok_to_replace = false;
		}
	      }
	    }
	  } else {
	    ok_to_replace = false;
	  }

	  if(ok_to_replace) {
	    // get rid of the shift
	    op_to_output_string.bind(op, string(""));

	    if(need_to_switch_operands) {
	      Operand tmp = dest->src(SRC1);
	      dest->set_src(SRC1, dest->src(SRC2));
	      dest->set_src(SRC2, tmp);
	    }

	    already_combined += dest;

	    // generate code for the combined shift+dp op
	    string new_code = get_dp_opc(dest);
	    output_register(dest->dest(DEST1), new_code);
	    new_code += ", ";
	    output_register(dest->src(SRC1), new_code);
	    new_code += ", ";
	    output_register(op->src(SRC1), new_code);
	    if(op->opcode() == SHR_W)
	      new_code += ", LSR ";
	    else if(op->opcode() == SHRA_W)
	      new_code += ", ASR ";
	    else
	      // I don't know what kind of right shift this is.
	      assert(0);
	    output_register(op->src(SRC2), new_code);
	    new_code += "\n";
	    op_to_output_string.bind(dest, new_code);
	  }
	}
      }
    } // if is rshift
  } // for all ops
}
