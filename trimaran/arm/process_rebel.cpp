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
#include <string.h>
#include <ctype.h>
#include <sstream>

#include "process_rebel.h"
#include "data_segment.h"
#include "peephole_opti.h"
#include "process_custom_ops.h"

#include "core_edge_drawing.h"
#include "el_expression.h"
#include "codegen.h"
#include "hash_functions.h"
#include "util.h"
#include "opcode_properties.h"
#include "el_jumptbl.h"


using namespace std;

string itos(int i)	// convert int to string
{
  stringstream s;
  s << i;
  return s.str();
}


unsigned ops_printed = 0;
const unsigned ops_between_ltorg = 500;

static bool is_worthless_op(Op*);

Procedure*
process_function(Procedure* p)
{
  if(p == NULL)
    return NULL;

  create_local_analysis_info_for_all_hbs_bbs(p);
  el_flow_compute_liveness(p, ANALYZE_ALLREG);
  insert_region_scalar_edges(p, EL_SCALAR_SCHED_POSTPASS);


  // not strictly necessary unless we start using the id->op map for
  // pbr attributes
  El_setup_op_id_map(p);

  bool outline = 0;

  bool push_flag;
  Dlist<Region*> rstack;
  Region *rtmp;
  rstack.push_tail(p);     

  string proc_name = p->get_name();    
  // emit a type specifier for the procedure name symbol        
  cout << "\t.text" << endl;                                    
  proc_name = proc_name.substr(1, proc_name.length());          
  cout << "\t.type\t" << proc_name << ",function" << endl;      
  // emit a label for the procedure entry                       

                                                                     
  Hash_map<Op*, string> op_to_output_string(hash_op_ptr, 256);

  cout << proc_name << ":" << endl;

  rstack.push_tail(p);
  while (!rstack.is_empty()) {

    rtmp = rstack.pop();
    push_flag = true;
    if (rtmp->is_hb() || rtmp->is_bb()) {

      
      unsigned ltorg_number = 0;

      // Certain operations in ARM don't have an equivalent opcode in HPL-PD.
      // Store multiple is a good example. This will appear as several individual
      // stores in the rebel file. This pass sets up a hash_map with the unsupported
      // opcodes combined into strings, enabling us to still print out one Op
      // at a time in the main loop below.

      // emit a label for branch instructions in case they target this bb
      cout << p->get_name() << "_bb_" <<rtmp->id() << ":" << endl;
      for(Region_all_ops rops((Compound_region*)rtmp); rops != 0; rops++){
	Op* op = *rops;

	if ( op_to_output_string.is_bound(op)) {
	  cout << op_to_output_string[op];
	} else if ( ! is_pseudo(op) && ! is_worthless_op(op)) {
	  translate_rebel_to_ARM(op);
	  ops_printed++;

	  // leave sections in the code to place literals
	  if(ops_printed > ops_between_ltorg) {
	    // need a branch over the literals.
	    cout << "\tb " << p->get_name() << "_bb_" << rtmp->id()
		 << "_ltorg_" << ltorg_number << endl;
	    cout << "\t.ltorg" << endl;
	    cout << p->get_name() << "_bb_" << rtmp->id() << "_ltorg_"
		 << ltorg_number++ << ":" << endl;
	    ops_printed = 0;
	  }

	}
      }

      push_flag = false;  // Only ops below, so don't waste time!
    }

    if (push_flag==true) {
      for (Region_subregions subri(rtmp); subri!=0; subri++) {
	if ((*subri)->is_compound())
	  rstack.push_tail (*subri);
      }
    }
  }

  // Need to put an ltorg at the end or we might get problems.
  cout << ".ltorg" << endl;

  delete_region_edges(p);  
  delete_local_analysis_info_for_all_hbs_bbs(p);

  // handle any jump tables that may exist
  El_jumptbl* tbl;
  El_jumptbl_info* tbl_info;
  List<El_jumptbl*> tbl_list;

  tbl_info = p->get_jumptbl_info();

  // Caveat emptor: this jumptable code is not well tested.
  if(tbl_info && tbl_info->get_num_tables() > 0) {
    cerr << "processing jump table information for " << p->get_name() << endl;
    // make sure the jump table goes in a data segment
    cout << ".data" << endl;

    // iterate through the list of jump tables and print them out
    tbl_list = tbl_info->get_tbl_list();
    for (List_iterator<El_jumptbl*> li(tbl_list); li != 0; li++) {
      tbl = *li;
      string name = tbl->get_name();

      if (name.substr(0, 1) == "_")
        name = name.substr(1, name.length() - 1);

      cerr << "emitting jump table: " << name << endl;

      cout << "\t.align " << tbl->get_align() << endl;
      cout << "\t.comm " << name << ", " << tbl->get_reserve() << endl;
      cout << name << ":" << endl;

      for (int i = 0; i < tbl->get_n_entries(); i++) {
        cout << "\t.word " << p->get_name() << "_bb_" <<
          tbl->get_op_target(i)->parent()->id() << endl;
      }
    }
  }

  return p;
}

static bool
is_worthless_op(Op* op)
{
  // Do some simple dead/useless code elimination. Most of these are
  // inserted into the IR through codegen inefficiencies, but there's
  // not a really good way to remove them in elcor. Not that this is
  // a good way, either...

  // Get rid of r1 = r1 + 0;
  if(is_add(op))
    if(op->dest(DEST1) == op->src(SRC1))
      if(op->src(SRC2).is_lit() && op->src(SRC2).is_int())
	if(op->src(SRC2).int_value() == 0)
	  return true;

  // Get rid of useless address label loads.
  if(is_move(op)) {
    if(op->src(SRC1).is_label()) {
      for(Op_outedges oe (op, FLOW_EDGES); oe != 0; oe++) {
	Op* dest = (*oe)->dest();
	if(is_brl(dest)) {
	  if(get_branch_target_attr(dest) != NULL)
	    return true;
	} else {
	  // dest is probably a PBR
	  for(Op_outedges oe2 (dest, FLOW_EDGES); oe2 != 0; oe2++) {
	    Op* dest2 = (*oe2)->dest();
	    if(is_brl(dest2)) {
	      if(get_branch_target_attr(dest2) != NULL)
		return true;
	    }
	  }
	}
      }
    }
    // and moves to the same register.
    else if(op->dest(DEST1) == op->src(SRC1))
      return true;
    else if(op->dest(DEST1).is_macro_reg() && op->src(SRC1).is_macro_reg()) {
      if((op->dest(DEST1).name() == INT_RETURN) &&
	 (op->src(SRC1).name() == INT_PARAM_1))
	return true;
      else if((op->dest(DEST1).name() == INT_PARAM_1) &&
	      (op->src(SRC1).name() == INT_RETURN))
	return true;
      else if((op->dest(DEST1).name() == FLT_PARAM_1) &&
	      (op->src(SRC1).name() == FLT_RETURN))
	return true;
      else if((op->dest(DEST1).name() == FLT_RETURN) &&
	      (op->src(SRC1).name() == FLT_PARAM_1))
	return true;
    }
  }

  return false;
}

void
output_register(Operand& oper)
{
  if (oper.is_reg()){
    if (!strcmp(oper.physical_file_type(),"GPR"))
      cout << 'r' << oper.mc_num();
    else if (!strcmp(oper.physical_file_type(),"FPR"))
      cout << 'f' << oper.mc_num();
    else
      cout << oper.physical_file_type() << ":" << oper.mc_num();
  }
  else if (oper.is_macro_reg()){
    //cout << oper.physical_file_type();
    switch (oper.name()){
    case SP_REG:
    case RGS_REG:{ //stack pointer
      //sp==r13
      cout << "sp";
    } break;
    case RETURN_ADDR:{
      //lr==r14
      cout << "lr";
    } break;
    case INT_RETURN:{ //return value r0 - r3
      cout << "r0";
    } break;
    case INT_PARAM_1:{ //parameter 1
      cout << "r0";    //a1 = r0
    } break;
    case INT_PARAM_2:{ //parameter 2
      cout << "r1";    //a2 = r1
    } break;
    case INT_PARAM_3:{ //parameter 3
      cout << "r2";    //a3 = r2
    } break;
    case INT_PARAM_4:{ //parameter 4
      cout << "r3";    //a4 = r3
    } break;

    case FLT_RETURN:
    case FLT_PARAM_1:{ //parameter 1
      cout << "f0";
    } break;
    case FLT_PARAM_2:{ //parameter 2
      cout << "f1";
    } break;
    case FLT_PARAM_3:{ //parameter 3
      cout << "f2";
    } break;
    case FLT_PARAM_4:{ //parameter 4
      cout << "f3";
    } break;
    case DBL_RETURN:
    case DBL_PARAM_1:{ //parameter 1
      cout << "f0";
    } break;
    case DBL_PARAM_2:{ //parameter 2
      cout << "f1";
    } break;
    case DBL_PARAM_3:{ //parameter 3
      cout << "f2";
    } break;
    case DBL_PARAM_4:{ //parameter 4
      cout << "f3";
    } break;
    case ICMOVE_REG: {
      cout << "r11";
    } break;
    default:{
      cerr << "ERROR: Unmapped macro register: " << oper.name() << endl;
      exit(-1);
    } break;
    }
  }
  else if (oper.is_lit()){
    if (oper.is_int())
      cout << '#' << oper.int_value();
    else if(oper.is_label()) {
      string label = oper.label_value();

      // strip the prefix _$fn_ off of library calls in the rebel code
      if (!strncmp(label.c_str(), "_$fn_", 5))
	label = label.substr(5, label.length() - 1);
      else
	label = label.substr(1, label.length()-1);

      // FIX: this is a really bad hack to get alloca working quickly
      if( ! strcmp(label.c_str(), "__builtin_alloca"))
	label = "malloc";
      if( ! strcmp(label.c_str(), "__ctype_b_loc"))
	label = "__ctype_b";

      cout << '=' << label;
    }
    else {
      cerr << "ERROR: Untranslated literal " << oper;
      exit(-1);
    }
  }

  else {
    cerr << "ERROR: Unknown operand type in "
      "process_rebel.output_register()   " << oper << endl;
    exit(-1);
  }
}

inline void
output_dst1_src1_src2(string opstr,Op* op)
{
  cout << opstr << "  ";
  output_register(op->dest(DEST1));
  cout << ", ";
  output_register(op->src(SRC1));
  cout << ", ";
  output_register(op->src(SRC2));
  cout << endl;
}

inline void
not_destination(Op* op)
{
  cout << "\tmvn  ";
  output_register(op->dest(DEST1));
  cout << ", ";
  output_register(op->dest(DEST1));
  cout << endl;
}


void
find_PBR(Op* op, string flags)
{
  string label = "*ERROR*";

  Branch_target_attr* bta = get_branch_target_attr(op);
  if(bta == NULL) {

    // stash the return address in the link register if necessary
    // NB: the pc points to the instruction which is 2 instructions
    // ahead of the one currently executing, hence the link address
    // is the value in the PC, not PC + 4 or something like that.
    if (flags == "l")
      cout << "\tmov  lr, pc" << endl;

    // loop through the inedges to find the operand for this branch
    // to find out what type of instruction we should use
    Operand branch_operand;

    // Mini-hack: if we see a restore immediately before the brl,
    // assume the register being restored is for the brl. The problem
    // is if the register used to write the PBR is spilled, then the
    // register with the branch target is not necessarily the one
    // that's writing the PBR. There's better ways to solve this
    // problem.
    if(is_restore(op->prev())) {
      branch_operand = op->prev()->dest(DEST1);
    } else {
      for(Op_inedges ie(op); ie != 0; ie++) {
	Edge* ed = *ie;
	if(ed->is_reg_flow() && ed->dest_port() == SRC1) {
	  branch_operand = ed->src()->src(ed->src_port());
	  break;
	}
      }
    }

    if(branch_operand.is_cb()) {
      cout << "\tb" << flags << "  " <<
	(char*)(el_get_enclosing_procedure(op)->get_name()) <<
	"_bb_" << branch_operand.id() <<endl;
    }
    else {
      if(flags != "l")
	cout << "\tmov" << flags << "  pc, ";
      else
	cout << "\tmov" << "  pc, ";

      output_register(branch_operand);
    }
    cout << endl;
  }
  else {
    label = bta->label;

    // FIX: this is a really bad hack to get alloca and ctype_b working quickly
    if( ! strcmp(label.c_str(), "__builtin_alloca"))
      label = "malloc";
    if( ! strcmp(label.c_str(), "__ctype_b_loc")) {
      cout << "\tldr r0, = __ctype_b" << endl;
      return;
    }

    cout << "\tb" << flags << "  " << label << endl;
  }
}

void
handle_mem_op(Op* op)
{
  char mem_size_classifier = 'w';
  string instr = "";
  Operand op1;

  if (op->opcode() & IR_BYTE)
    mem_size_classifier = 'b';
  else if (op->opcode() & IR_HALFWORD)
    mem_size_classifier = 'h';
  else if (op->opcode() & IR_QUADWORD) {
    cerr << "Error: process_rebel.cpp: unhandled memory size IR_QUADWORD"
	 << endl;
    exit (-1);
  }

  if (is_load (op)) {
    op1 = op->dest(DEST1);

    // make sure this isn't a spill or fill to a BTR
    if (!strncmp(op1.physical_file_type(), "BTR", 3) && !op1.is_macro_reg())
      return;

    // setup the load opcode
    if (op1.dtype().is_float()) {
      if (op->opcode() & IR_DOUBLE)
	instr += "\tldfd ";
      else
	instr += "\tldfs ";
    }
    else {
      if (is_sign_extend_load(op))
	instr += "\tldrs";
      else
	instr += "\tldr";

      if (mem_size_classifier != 'w')
	instr += mem_size_classifier;

      instr += "  ";
    }

    // check if the location being loaded is from a label that we have moved
    // and renamed
    if (op->src(SRC1).is_label()) {
      string label = op->src(SRC1).label_value();
      Pair<string, string> *pair;

      // strip off the underscore prefix
      label = label.substr(1, label.length()-1);

      pair = global_rename_map->find_entry(label);

      if (pair) {
	// the label used for this load has been renamed, so we need
	// to insert an indirect load and then change the source for
	// the current load.  since the destination is going to be
	// killed and we know it is not a source, we'll use it as a
	// temp here

	cerr << "$$$$$ renaming load of " << label << " $$$$" << endl;

	if (op1.dtype().is_single() || op1.dtype().is_double()) {
	  cerr << "Error: could not handle renamed floating point label"
	       << endl;
	  exit (-1);
	}

	string tmp = "ldr  r";
	tmp += op1.mc_num();
	instr = "\n"+instr;
	instr = label + instr;
	instr = ", " + instr;
	instr = tmp + instr;
	//  	  "\tXXX ldr r"+op1.mc_num();
	//+", "+label+"\n"+instr;

	// now replace the source for the original load with the
	// temp register
	Operand reg_operand(op1);
	op->set_src(SRC1, reg_operand);
      }
    }
  }

  else if (is_store (op)) {
    if (is_base_plus_offset_store(op))
      op1 = op->src(SRC3);
    else
      op1 = op->src(SRC2);

    // make sure this isn't a spill or fill to a BTR
    if (!strncmp(op1.physical_file_type(), "BTR", 3) && !op1.is_macro_reg())
      return;

    // write the store opcode
    if (op1.dtype().is_float()) {
      if (op->opcode() & IR_DOUBLE)
	instr += "\tstfd  ";
      else
	instr += "\tstfs  ";
    }
    else {
      instr += "\tstr";

      if (mem_size_classifier != 'w')
	instr += mem_size_classifier;

      instr += "  ";
    }
  }
  else {
    // sanity check
    cerr << "ERROR: Unknown memory operation: " << *op << endl;
    exit(-1);
  }

  // emit the memory instruction
  cout << instr;
  output_register(op1);
  cout << ", ";

  if (op->src(SRC1).is_label()) {
    output_register(op->src(SRC1));
    if (is_base_plus_offset_load(op) || is_base_plus_offset_store(op)) {
      cout << ", ";
      output_register(op->src(SRC2));
    }
    cout << endl;
  }
  else {
    cout << "[";
    output_register(op->src(SRC1));

    cout << ", ";
    if (is_base_plus_offset_load(op) || is_base_plus_offset_store(op))
      output_register(op->src(SRC2));
    else
      cout << "#0";
    cout << "]" << endl;
  }
}


void
handle_cmpp_op(Op* op, string flags)
{
  int val = op->src(SRC2).int_value();

  if (is_fcmpp(op))
    cout << "\tcmf" << flags << "  ";
  else
    cout << "\tcmp" << flags << "  ";

  if (op->src(SRC1).is_lit()) {
    output_register(op->src(SRC2));
    cout << ", ";
    output_register(op->src(SRC1));

  }
  else {
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
  }

  cout << endl;
}



void
output_unary_float(Op* op) {
  if(is_int_fltdbl_conversion_op(op)) {
    cout << "  ";
  } else {
    if(is_double(op->opcode()))
      cout << "d  ";
    else
      cout << "s  ";
  }
  output_register(op->dest(DEST1));
  cout << ", ";
  output_register(op->src(SRC1));
  cout << endl;
}

void
output_binary_float(Op* op) {
  if(is_double(op->opcode()))
    cout << "d  ";
  else
    cout << "s  ";
  output_register(op->dest(DEST1));
  cout << ", ";
  output_register(op->src(SRC1));
  cout << ",  ";
  output_register(op->src(SRC2));
  cout << endl;
}


void
translate_rebel_to_ARM(Op* op)

{

  if (is_memory(op)) {
    handle_mem_op(op);
    return;
  }

  if (is_cmpp(op) || is_fcmpp(op)) {
    // print compares next to branches to avoid saving condition flags
    // so do nothing here
    handle_cmpp_op(op, "");
    return;
  }

  // translate from rebel opcodes to arm opcodes, this is really all
  // that the arm translator should do, but in some cases things get
  // hairy

  switch (op->opcode()) {
    // add operations
  case ADDL_W:
  case ADD_W:
    output_dst1_src1_src2("\tadd",op);
    break;

    // logical and operation
  case AND_W:
    output_dst1_src1_src2("\tand",op);
    break;

    // logical nand operation
  case NAND_W:
    output_dst1_src1_src2("\tand",op);
    not_destination(op);
    break;

    // logical nor operation
  case NOR_W:
    output_dst1_src1_src2("\torr",op);
    not_destination(op);
    break;


    // logical or operation
  case OR_W:
    output_dst1_src1_src2("\torr",op);
    break;

    // return operation -- branch to the value in the lr
  case RTS:
    // pop lr off of stack, update stack pointer
    cout << "\tmov  pc, lr" << endl;
    break;

    // subtract operations
  case SUBL_W:
  case SUB_W:
    output_dst1_src1_src2("\tsub",op);
    break;

    // logical xor operation
  case XOR_W:
    output_dst1_src1_src2("\teor",op);
    break;

    /***************************************************************/
    /*  CHECK ME BELOW HERE! */
    /***************************************************************/

  case BRU:
    find_PBR(op,"");
    break;

  case ABS_W:
    if(op->dest(DEST1) == op->src(SRC1)) {
      cerr << "ERROR: ABS not allowed to have same source and dest!" << endl
	   << *op << endl;
      exit(-1);
    }
    cout << "\teor  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << ", asr #31" << endl;
    cout << "\tsub  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << ", asr #31" << endl;
    break;

  case EXTS_B:
    cerr << "Shouldn't be seeing any EXTS_Bs!!\n" << *op << endl;
    exit(-1);
    break;
  case SHR_W:
    cout << "\tmov  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << ", lsr ";
    output_register(op->src(SRC2));
    cout << endl;
    break;
  case SHRA_W:
    cout << "\tmov  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << ", asr ";
    output_register(op->src(SRC2));
    cout << endl;
    break;
  case SHL_W:
    cout << "\tmov  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << ", lsl ";
    output_register(op->src(SRC2));
    cout << endl;
    break;
  case MPY_W:
  case MPYL_W:
    // this case is unsupported.  we can swap src1 and src2 if they are
    // not the same
    if (op->dest(DEST1).mc_num() == op->src(SRC1).mc_num()) {
      if (op->src(SRC1).mc_num() == op->src(SRC2).mc_num()) {
	//        cerr << *op << endl;
	//        cerr << "Error: Rd == Rm has unpredictable results for mul instructions"
	// 	    << endl;
	// FIX: what do you do when you have vr1 = vr1 * vr1? adding a
	// move will just get thrown away in the elcor optis, unless we
	// make them cognizant of the multiply wierdness, which is an
	// ugly architecture specific hack. Of course this solution
	// isn't very pretty, either.

	Operand icmove = new Macro_reg(ICMOVE_REG);

	cout << "\tmov  ";
	output_register(icmove);
	cout << ", ";
	output_register(op->src(SRC1));
	cout << endl;

	cout << "\tmul  ";
	output_register(op->dest(DEST1));
	cout << ", ";
	output_register(icmove);
	cout << ", ";
	output_register(op->src(SRC2));
	cout << endl;
      }
      else {
	cout << "\tmul  ";
	output_register(op->dest(DEST1));
	cout << ", ";
	output_register(op->src(SRC2));
	cout << ", ";
	output_register(op->src(SRC1));
	cout << endl;
      }
    }
    else {
      cout << "\tmul  ";
      output_register(op->dest(DEST1));
      cout << ", ";
      output_register(op->src(SRC1));
      cout << ", ";
      output_register(op->src(SRC2));
      cout << endl;
    }
    break;
//   case PBRR:
//   case PBRA:
  case PBR:
    // do nothing
    break;
  case CONVWS:
    cout << "\tflts";
    output_unary_float(op);
    break;

  case CONVWD:
    cout << "\tfltd";
    output_unary_float(op);
    break;

  case CONVDS: // double to single
    cout << "\tmvfs";
    output_unary_float(op);
    break;

  case CONVSD: // single to double
    cout << "\tmvfd";
    output_unary_float(op);
    break;

  case CONVSW:
  case CONVDW:
    cout << "\tfix";
    output_unary_float(op);
    break;

  case FABS_D:
  case FABS_S:
    cout << "\tabs";
    output_unary_float(op);
    break;

  case FSQRT_D:
  case FSQRT_S:
    cout << "\tsqt";
    output_unary_float(op);
    break;

  case FADD_S:
  case FADD_D:
    cout << "\tadf";
    output_binary_float(op);
    break;

  case FSUB_S:
  case FSUB_D:
    cout << "\tsuf";
    output_binary_float(op);
    break;

  case FMPY_S:  // only works with single precision
    cout << "\tfml";
    output_binary_float(op);
    break;

  case FMPY_D:
    cout << "\tmuf";
    output_binary_float(op);
    break;

  case FDIV_S:  // only works with single precision
    cout << "\tfdv";
    output_binary_float(op);
    break;

  case FDIV_D:
    cout << "\tdvf";
    output_binary_float(op);
    break;

  case MOVEF_S:
    cout << "\tmvfs  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << endl;
    break;
  case MOVEF_D:
    cout << "\tmvfd  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << endl;
    break;

  case FMPYADD_S:
    cout << "\tfmls  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;
    cout << "\tadfs  ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->dest(DEST1));
    cout << ", ";
    output_register(op->src(SRC3));
    cout << endl;
    break;

  case NO_OP:
    cout << "\tmov  r0, r0\n";
    break;

  case BRL:
    find_PBR(op, "l");
    break;

  case BRCT:
    {
      string flags = "";
      string label = "";
      int offset = 0;
      for(Op_inedges oie(op); oie != 0; oie++){
	if((*oie)->is_reg_flow()){
	  Op* asdf = (*oie)->src();
	  if (is_cmpp(asdf)){
	    //found compare instr
	    string cmp_op = get_mdes_opcode_string(asdf);
	    if(cmp_op.substr(0,7) == "CMPP_W_"){
	      cmp_op.erase(0,7);

	      int index = 0;
	      index = cmp_op.find("_", 0);
	      flags = cmp_op.substr(0, index);
	    }
	    else {
	      if (is_fcmpp(asdf)){
		//found floating point compare
		string cmp_op = get_mdes_opcode_string(asdf);
		if(cmp_op.substr(0,6) == "FCMPP_"){
		  // the first character here is going to be the precision
		  string cmf_flags = "";
		  cmf_flags += cmp_op[6];

		  cmp_op.erase(0,8);
		  int index = 0;
		  index = cmp_op.find("_", 0);
		  flags = cmp_op.substr(0, index) + flags;
		}
	      }
	    }
	  }
	}
      }

      // FIXME.JB: this code is ugly, but it works for now
      if (flags == "LGT")
	flags = "hi";
      else if (flags == "LLEQ")
	flags = "ls";
      else if (flags == "LEQ")
	flags = "le";
      else if (flags == "GEQ")
	flags = "ge";
      else if (flags == "SV")
	flags = "vs";
      else if (flags == "OD") {
	cerr << "who uses compare odd?  somebody did and i'm quiting" << endl;
	exit (-1);
      }
      else if (flags == "FALSE")
	break;
      else if (flags == "TRUE")
	flags = "";
      else if (flags == "NEQ")
	flags = "ne";
      else if (flags == "LGEQ" || flags == "LLT") {
	cerr << "Error: unsupported compare operation: " << flags << endl;
	exit (-1);
      }
      else if (flags == "NSV")
	flags = "vc";
      else if (flags == "") {
	cerr << "Couldn't find the CMP op for this BRCT!\n" << *op << endl;
	exit(-1);
      }

      find_PBR(op, flags);
    }
    break;
  case CMPR_W_FALSE:
    cout << "\tmov" << " ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;
    break;
  case CMPR_W_EQ:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmoveq ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;

    // this would be a good way to do it without using cmp's if the
    // clz instruction worked in gcc.
    //     cout << "\tsub  ";
    //     output_register(op->dest(DEST1));
    //     cout << ", ";
    //     output_register(op->src(SRC1));
    //     cout << ", ";
    //     output_register(op->src(SRC2));
    //     cout << endl;

    //     cout << "\tclz  ";
    //     output_register(op->dest(DEST1));
    //     cout << ", ";
    //     output_register(op->dest(DEST1));
    //     cout << endl;

    //     cout << "\tmov  ";
    //     output_register(op->dest(DEST1));
    //     cout << ", ";
    //     output_register(op->dest(DEST1));
    //     cout << ", lsr #5" << endl;

    break;
  case CMPR_W_LT:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovlt ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_LEQ:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovle ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_GT:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovgt ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_GEQ:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovge ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_SV:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovvs ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_OD:
    cerr << "who uses compare odd?  somebody did and i'm quiting" << endl;
    exit (-1);
    break;
  case CMPR_W_TRUE:
    cout << "\tmov" << " ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_NEQ:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovne ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_LLT:
    cerr << "Error: CMPR_LLT not implemented" << endl;
    exit (-1);
  case CMPR_W_LLEQ:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovls ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_LGT:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovhi ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_LGEQ:
    cerr << "Error: CMPR_LGEQ not implemented" << endl;
    exit (-1);
    break;
  case CMPR_W_NSV:
    cout << "\tcmp  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovvc ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case CMPR_W_EV:
    cerr << "Error: CMPR_EV not implemented" << endl;
    exit (-1);
    break;


  case FCMPR_S_FALSE:
  case FCMPR_D_FALSE:
    cout << "\tmov" << " ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;
    break;
  case FCMPR_S_EQ:
  case FCMPR_D_EQ:
    cout << "\tcmf  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmoveq ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;

    break;
  case FCMPR_S_LT:
  case FCMPR_D_LT:
    cout << "\tcmf  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovlt ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case FCMPR_S_LEQ:
  case FCMPR_D_LEQ:
    cout << "\tcmf  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovle ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case FCMPR_S_GT:
  case FCMPR_D_GT:
    cout << "\tcmf  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovgt ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case FCMPR_S_GEQ:
  case FCMPR_D_GEQ:
    cout << "\tcmf  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovge ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case FCMPR_S_TRUE:
  case FCMPR_D_TRUE:
    cout << "\tmov" << " ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;
  case FCMPR_S_NEQ:
  case FCMPR_D_NEQ:
    cout << "\tcmf  ";
    output_register(op->src(SRC1));
    cout << ", ";
    output_register(op->src(SRC2));
    cout << endl;

    // mov a zero to the destination and then overwrite it if
    // necessary with a predicated move
    cout << "\tmov ";
    output_register(op->dest(DEST1));
    cout << ", #0" << endl;

    cout << "\tmovne ";
    output_register(op->dest(DEST1));
    cout << ", #1" << endl;
    break;


    // case MOVELGS:
  case MOVEGG:
  case MOVE: {
    if (!strcmp(op->dest(DEST1).physical_file_type(), "BTR") ||
	!strcmp(op->src(SRC1).physical_file_type(), "BTR")) {

      // don't do anything with this
      return;
    }

    Operand oper = op->src(SRC1);

    bool is_mvn = false;
    if (op->src(SRC1).is_label())
      // move address of label to register
      cout << "\tldr  ";
    else {
      if(op->src(SRC1).is_int()) {
	if(is_valid_dp_const(op->src(SRC1).int_value())) {
	  cout << "\tmov  ";
	} else if(is_valid_dp_const(~(op->src(SRC1).int_value()))) {
	  cout << "\tmvn  ";
	  is_mvn = true;
	} else {
	  cout << "\tldr  ";
	  output_register(op->dest(DEST1));
	  cout << ", =";
	  cout << op->src(SRC1).int_value() << endl;
	  return;
	}
      } else {
	cout << "\tmov  ";
      }
    }

    output_register(op->dest(DEST1));
    cout << ", ";
    if( ! is_mvn) {
      output_register(op->src(SRC1));
    } else {
      cout << "#" << ~(op->src(SRC1).int_value());
    }
    cout << endl;
  }
    break;

  case CUSTOM: {
    process_custom_op(op);
  }
    break;

  default:
    cerr << "ERROR: Undefined Opcode: " << op->opcode()
	 << " in Instruction: " << endl;
    cerr << *op << endl;
    exit(-1);
  }
}

// read in all of the data segments and puts them into
// global_data_segments
void
process_data(El_datalist* datalist, int section_number)
{
  Data_segment *current_segment = NULL;
  Data_segment_element *current_element = NULL;

  for (List_iterator<El_data *> it(datalist->elements);it!=0;it++){
    El_data * eldata = *it;

    // first handle memory segment specifiers
    if (eldata->t_type == EL_DATA_TOKEN_MS) {
      switch (eldata->ms_type) {
      case EL_DATA_MS_TEXT:
	current_segment = new Data_segment(".text", section_number);
	assert(current_segment && "out of memory");
	global_data_segments->add_tail(current_segment);
	break;
      case EL_DATA_MS_DATA:
	current_segment = new Data_segment(".data", section_number);
	assert(current_segment && "out of memory");
	global_data_segments->add_tail(current_segment);
	break;
      case EL_DATA_MS_DATA1:
	current_segment = new Data_segment(".data 1", section_number);
	assert(current_segment && "out of memory");
	global_data_segments->add_tail(current_segment);
	break;
      case EL_DATA_MS_DATA2:
	current_segment = new Data_segment(".data 2", section_number);
	assert(current_segment && "out of memory");
	global_data_segments->add_tail(current_segment);
	break;
      case EL_DATA_MS_BSS:
	current_segment = new Data_segment(".bss", section_number);
	assert(current_segment && "out of memory");
	global_data_segments->add_tail(current_segment);
	break;
      default:
	cerr << "Error: process_rebel.cpp: unkown memory segment specifier"
	     << endl;
	exit(-1);
	break;
      }
      continue;
    }

    if (current_segment == NULL) {
      cerr << "Error: first data token was not a segment specifier, aborting"
	   << endl;
      exit(-1);
    }

    // gather information about the elements of the current data section
    switch (eldata->t_type){
    case EL_DATA_TOKEN_VOID:
      current_segment->handle_void_token(eldata);
      break;
    case EL_DATA_TOKEN_BYTE:
      current_segment->handle_byte_token(eldata);
      break;
    case EL_DATA_TOKEN_ALIGN:
      current_segment->handle_alignment_token(eldata);
      break;
    case EL_DATA_TOKEN_ASCII:
      current_segment->handle_ascii_token(eldata);
      break;
    case EL_DATA_TOKEN_ASCIZ:
      cerr << "processing token: EL_DATA_TOKEN_ASCIZ " << endl;
      current_segment->handle_asciz_token(eldata);
      break;
    case EL_DATA_TOKEN_RESERVE:
      current_segment->handle_reserve_token(eldata);
      break;
    case EL_DATA_TOKEN_GLOBAL:
      current_segment->handle_global_token(eldata);
      break;
    case EL_DATA_TOKEN_WB:
      current_segment->handle_wb_token(eldata);
      break;
    case EL_DATA_TOKEN_WW:
      current_segment->handle_ww_token(eldata);
      break;
    case EL_DATA_TOKEN_WORD:
      current_segment->handle_word_token(eldata);
      break;
    case EL_DATA_TOKEN_LONG:
      current_segment->handle_long_token(eldata);
      break;
    case EL_DATA_TOKEN_WI:
      current_segment->handle_wi_token(eldata);
      break;
    case EL_DATA_TOKEN_WF:
      current_segment->handle_wf_token(eldata);
      break;
    case EL_DATA_TOKEN_WF2:
      current_segment->handle_wf2_token(eldata);
      break;
    case EL_DATA_TOKEN_FLOAT:
      current_segment->handle_float_token(eldata);
      break;
    case EL_DATA_TOKEN_DOUBLE:
      current_segment->handle_double_token(eldata);
      break;
    case EL_DATA_TOKEN_WS:
      current_segment->handle_ws_token(eldata);
      break;
    case EL_DATA_TOKEN_ELEMENT_SIZE:
      current_segment->handle_element_size_token(eldata);
      break;
    case EL_DATA_TOKEN_DEF_STRUCT:
      current_segment->handle_def_struct_token(eldata);
      break;
    case EL_DATA_TOKEN_DEF_UNION:
      current_segment->handle_def_union_token(eldata);
      break;
    case EL_DATA_TOKEN_DEF_ENUM:
      cerr << "processing token: EL_DATA_TOKEN_DEF_ENUM " << endl;
      current_segment->handle_enum_token(eldata);
      break;
    case EL_DATA_TOKEN_FIELD:
      current_segment->handle_field_token(eldata);
      break;
    case EL_DATA_TOKEN_ENUMERATOR:
      cerr << "processing token: EL_DATA_TOKEN_ENUMERATOR " << endl;
      current_segment->handle_enumerator_token(eldata);
      break;
    default:
      cerr << "Error: process_rebel.cpp: unkown data token type" <<
	endl;
      exit(-1);
      break;
    }
  }
}

void
emit_data_section(int section_number)
{
  // iterate over the stored sections and emit those with the
  // specified section number
  for (List_iterator<Data_segment *> li(*global_data_segments); li!=0; li++){
    Data_segment* current_segment = *li;

    if (current_segment->section_number == section_number)
      current_segment->emit_segment();
  }
}
