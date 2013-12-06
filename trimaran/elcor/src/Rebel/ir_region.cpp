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
//      File:           ir_region.cpp
//      Authors:        Greg Snider, Scott A. Mahlke,
//			Sumedh Sathaye, Sadun Anik
//      Created:        August 1995
//      Description:    Elcor Text IR reader-writer for class Region
//
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "ir_region.h"
#include "dlist.h"
#include "ir_op.h"
#include "ir_edge.h"
#include "IR_symbol.h"
#include "meld.h"
#include "el_jumptbl.h"
#include "attribute_types.h"
#include "ir_attribute.h"
#include <iostream>

//-------------------------- Writer -------------------------------

IR_outstream &operator<<(IR_outstream &out, const Elcor_region_flag flag);
IR_outstream &operator<<(IR_outstream &out, const Hash_set<Edge*> &edges);


IR_outstream &operator<<(IR_outstream &out, const Region *region)
{
    // Write out a pointer to a region;
    if (region->is_op())
	out << (const Op*) region;
    else if (region->is_bb())
	out << BB_STRING << region->id();
    else if (region->is_hb())
	out << HB_STRING << region->id();
    else if (region->is_loopbody())
	out << LOOPBODY_STRING << region->id();
    else if (region->is_procedure())
	out << PROC_STRING << (char*)(((Procedure*)region)->get_name())
	    << region->id();
    else if (region->is_tree())
	out << TREE_STRING << region->id();
    else if (region->is_compound())
	out << COMPOUND_REGION_STRING << region->id();
    else {
	cerr << "unknown region type" << endl;
	exit(1);
    }
    return out;
}


IR_outstream &operator<<(IR_outstream &out, const Region &reg)
{
    // Write out a region;
    if (reg.is_op())
	out << (const Op&) reg;
    else {
	// compound region
	const Compound_region &region = (const Compound_region &) reg;
	out << &reg << SPACE;
	out << L_PAREN << indent << endline << tab;
	out << REGION_WEIGHT_STRING << L_PAREN << region.weight
	    << R_PAREN << endline << tab;
    
	// entry ops and exit ops;
	out << REGION_ENTRY_OPS_STRING << L_PAREN;
	Hash_set_iterator<Op*> op_ptr;
	for (op_ptr(region.embedded_entry_ops); op_ptr != 0; op_ptr++)
	    out << (*op_ptr)->id();
	out << R_PAREN << REGION_EXIT_OPS_STRING << L_PAREN;
	for (op_ptr(region.embedded_exit_ops);  op_ptr != 0;  op_ptr++)
	    out << (*op_ptr)->id();
	out << R_PAREN << endline << tab;

	// entry edges and exit edges;
	out << REGION_ENTRY_EDGES_STRING << L_PAREN;
	List_iterator<Edge*> edge_ptr;
	for (edge_ptr(region.entry_edges);  edge_ptr != 0;  edge_ptr++)
	    out << *edge_ptr;
	out << R_PAREN << REGION_EXIT_EDGES_STRING << L_PAREN;
	for (edge_ptr(region.exit_edges);  edge_ptr != 0;  edge_ptr++)
	    out << *edge_ptr;
	out << R_PAREN << endline << tab; 

	// region flags;
	if (region.region_flags.ones_count() > 0)
	    out << region.region_flags << endline << tab;
    
	// attributes;
	bool print_header = false;
	for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
	  Base_attribute* attr = region.attributes->get_attribute((Attribute_type)type);
 	  // check if the attribute exists and supports Rebel IO
	  if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
	  ir_assert(attr->is_compound_region_attribute());
	  // print header on first time
	  if (!print_header) {
	    print_header = true;
	    out << ATTR_STRING << L_PAREN;
	  }
	  // print pointer or inline the attribute
	  if (attr->is_inline()) out << *attr;
	  else out << attr;
	}
	if (print_header)
	  out << R_PAREN << endline << tab;

	// subregions;
	out << REGION_SUBREGIONS_STRING << L_PAREN << indent;
	Dlist_iterator<Region*> region_ptr;
	for (region_ptr(region.embedded_regions); region_ptr!=0; region_ptr++)
	    out << endline << tab << **region_ptr;
	out << endline << outdent << tab << R_PAREN;  // end subregions;
	out << endline << outdent << tab << R_PAREN;  // end region;
    }
    return out;
}


void name_edges(IR_outstream &out, Region *reg)
{
    // Scan recursively for all edges in a region and create IR_outstream
    // names (of the form "^integer") for them;

    if (reg->is_op())
	name_edges(out, (Op *) reg);
    else {
	Compound_region *creg = (Compound_region*) reg;
	Dlist_iterator<Region*> region_ptr;
	for (region_ptr(creg->embedded_regions); region_ptr!=0; region_ptr++)
	    name_edges(out, *region_ptr);
    }
}


void print_edges(IR_outstream &out, Region *reg)
{
    // Scan recursively for all edges in a region and print them;

    if (reg->is_op())
	print_edges(out, (Op *) reg);
    else {
	Compound_region *creg = (Compound_region*) reg;
	Dlist_iterator<Region*> region_ptr;
	for (region_ptr(creg->embedded_regions); region_ptr!=0; region_ptr++)
	    print_edges(out, *region_ptr);
    }
}


void name_attributes(IR_outstream &out, Region *reg)
{
  // Scan recursively for all attributes in a region and create IR_outstream
  // names (of the form "^integer") for them;

  if (reg->is_op()) {
    name_attributes(out, (Op *) reg);
  } else {
    Compound_region *creg = (Compound_region*) reg;
    // First get attributes of this region
    for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
      Base_attribute* attr = creg->attributes->get_attribute((Attribute_type)type);
      // check if the attribute exists and supports Rebel IO
      if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
      ir_assert(attr->is_compound_region_attribute());
      // map its pointer unless the attribute is to be inlined
      if (!attr->is_inline()) out.map_pointer((void *) attr);
    }
	
    // Name all attributes in subregions;
    Dlist_iterator<Region*> region_ptr;
    for (region_ptr(creg->embedded_regions); region_ptr!=0; region_ptr++)
      name_attributes(out, *region_ptr);
  }
}

void print_attributes(IR_outstream &out, Region *reg)
{
  // Scan recursively for all attributes in a region and print the
  // ones that go into a separate dictionary;

  if (reg->is_op()) {
    print_attributes(out, (Op *) reg);
  } else {
    Compound_region *creg = (Compound_region*) reg;
    // First get attributes of this region
    for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
      Base_attribute* attr = creg->attributes->get_attribute((Attribute_type)type);
      // check if the attribute exists and supports Rebel IO
      if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
      ir_assert(attr->is_compound_region_attribute());
      // print the attributes that are pointers, inlined ones are already printed
      if (!attr->is_inline()) out << *attr << endline << tab;
    }

    // Print all attributes in subregions
    Dlist_iterator<Region*> region_ptr;
    for (region_ptr(creg->embedded_regions); region_ptr!=0; region_ptr++)
      print_attributes(out, *region_ptr);
  }
}


void print_mem_vrs(IR_outstream &out, Region *reg)
{
    int count;
    // Scan recursively for all mem_vrs in a region and print them;

    if (reg->is_op()) {
	Op *op = (Op *) reg;
	if (op->num_mem_dests()>0 || op->num_mem_srcs()>0) {
           Hash_set_iterator<Operand> ptr;
           out << OP_STRING << op->id() << SPACE << L_PAREN;
           out << MEMVR_OUT_STRING << L_PAREN;
           for(ptr(op->mem_dests()); ptr!=0; ptr++)
              out << *ptr;
           out << R_PAREN << SPACE;
           out << MEMVR_IN_STRING << L_PAREN;
           count = 0;
           for(ptr(op->mem_srcs()); ptr!=0; ptr++) {
              if (count == 21) {
                 out << endline;
                 out.indent();
                 out.tab();
                 out.outdent();
                 count = 0;
              }
              out << *ptr;
              count++;
           }
           out << R_PAREN << R_PAREN << endline << tab;
	}
    } else {
	Compound_region *creg = (Compound_region*) reg;

	// Recursion;
	Dlist_iterator<Region*> region_ptr;
	for (region_ptr(creg->embedded_regions); region_ptr!=0; region_ptr++)
	    print_mem_vrs(out, *region_ptr);
    }
}


//---------------- support operators -------------------------------

// hongtaoz: return if a flag is persistent, only print out persistent flags
bool is_persistent_flag (Elcor_region_flag flag) {
  if (flag == EL_OPER_MARK1 || flag == EL_OPER_MARK2)
    return false;
  else 
    return true;
}

IR_outstream &operator<<(IR_outstream &out, const Bitvector &flags)
{
    // Write out region flags;

    if (flags.ones_count() > 0) {
	Elcor_region_flag flag;
	out << REGION_FLAGS_STRING << L_PAREN;
	for (flag = Elcor_region_flag(0);
	     flag != EL_REGION_LAST_FLAG;
	     flag = Elcor_region_flag(int(flag) + 1))
	{
	    if (flags.bit(int(flag)) == 1 && is_persistent_flag (flag))
		out << flag;
	}
	out << R_PAREN;
    }
    return out;
}
	

IR_outstream &operator<<(IR_outstream &out, const Elcor_region_flag flag)
{
    // Writes out an Elcor region flag;

    switch (flag) {

      case EL_OPER_CHECK: out << "check";  break;
      case EL_OPER_LABEL_REFERENCE: out << "label";  break;
      case EL_OPER_PROMOTED: out << "prom";  break;
      case EL_OPER_SQUASHING: out << "squash";  break;
      case EL_OPER_DATA_SPECULATIVE: out << "dspec";  break;
      case EL_OPER_SPILL_CODE: out << "spill";  break;
      case EL_OPER_CALLEE_SAVE: out << "callee";  break;
      case EL_OPER_CALLER_SAVE: out << "caller";  break;
      case EL_OPER_SIDE_EFFECT_FREE: out << "sef";  break;
      case EL_OPER_SPECULATIVE: out << "spec";  break;
      case EL_OPER_MASK_PE: out << "mask";  break;
      case EL_OPER_SAFE_PEI: out << "safe";  break;
      case EL_OPER_PROBE_MARK: out << "probe";  break;
      case EL_OPER_SYNC: out << "sync"; break;
      case EL_OPER_PROCESSOR_SPECIFIC: out << "pspec"; break;
      case EL_OPER_VOLATILE: out << "vol"; break;
      case EL_OPER_NO_SPECULATION: out << "nospec"; break;
      case EL_OPER_STACK_REFERENCE: out << "stack"; break;
      case EL_OPER_ROTATE_REGISTERS: out << "rotate"; break;
      case EL_OPER_SUPER_SPECULATION: out << "superspec"; break;

      case EL_OPER_TABLE_JUMP: out << "jrg";  break;
      case EL_OPER_REAL_MERGE: out << "real_merge";  break;
      case EL_OPER_NONSEQ_DUMMY_BR: out << "nonseq_db";  break;
      case EL_OPER_LIVEOUT_DEST: out << "liveout";  break;
      case EL_OPER_CVT: out << "cvt";  break;
      case EL_OPER_ICMOVE: out << "icmove";  break;
      case EL_OPER_SWP_BRANCH: out << "swp_brn";  break;
      case EL_OPER_CGEN_MOVE: out << "cgen_move"; break;
      case EL_OPER_STACK_MOD: out << "stack_mod"; break;

      case EL_REGION_HYPERBLOCK: out << "hb";  break;
      case EL_REGION_HYPERBLOCK_NO_FALLTHRU: out << "hb_no_ft";  break;
      case EL_REGION_SOFTPIPE: out << "softpipe";  break;
      case EL_REGION_REMAINDER_BLOCK: out << "remainder";  break;
      case EL_REGION_SUPERBLOCK: out << "sb";  break;
      case EL_REGION_UNROLLED: out << "unrolled";  break;
      case EL_REGION_ENTRANCE_BOUNDARY:	out << "entr_bndry";  break;
      case EL_REGION_EXIT_BOUNDARY: out << "exit_bndry";  break;
      case EL_REGION_HAS_PROLOGUE_OP: out << "prologue";  break;
      case EL_REGION_HAS_EPILOGUE_OP: out << "epilogue";  break;
      case EL_REGION_VIOLATES_LC_SEMANTICS: out << "violates_lc";  break;
      case EL_REGION_ROT_REG_ALLOCATED: out << "rot_regalloc";  break;

      case EL_REGION_HYPERBLOCK_FRP: out << "hb_frp"; break;
      case EL_REGION_REGISTER_ALLOCATED: out << "regalloc";  break;
      case EL_REGION_SCHEDULED: out << "sched";  break;
      case EL_REGION_PRELOOP_BLOCK: out << "preloop";  break;
      case EL_REGION_POSTLOOP_BLOCK: out << "postloop";  break;
      case EL_REGION_HAS_EXPLICIT_FT: out << "explicit_ft";  break;
      case EL_REGION_CPR_BLOCK: out << "cpr_blk"; break;
      case EL_REGION_CPR_COMP_BLOCK: out << "cpr_comp_blk"; break;
      case EL_REGION_LOOP_CACHE: out << "loop_cache"; break;
      case EL_REGION_HAS_ANALYSIS_INFO: out << "analysis_info"; break;

      case EL_PROC_HYPERBLOCK: out << "has_hb";  break;
      case EL_PROC_LEAF: out << "leaf";  break;
      case EL_PROC_SIDE_EFFECT_FREE: out << "all_sef";  break;
      case EL_PROC_REGISTER_ALLOCATED: out << "all_regalloc";  break;
      case EL_PROC_SUPERBLOCK: out << "has_sb";  break;
      case EL_PROC_MASK_PE: out << "has_mask";  break;
      case EL_PROC_COMPILATION_COMPLETE: out << "comp_done";  break;
      case EL_PROC_SCHEDULED: out << "all_sched";  break;
      case EL_PROC_CC_IN_PREDICATE_REGS: out << "all_cc_in_pregs";  break;
      case EL_PROC_PRED_REGS_IN_ATTR: out << "all_pregs_in_attr";  break;

      case EL_PROC_NEEDS_SUIF_FIXUP: out << "needs_suif_input_fixup"; break;
      case EL_PROC_ROT_REG_ALLOCATED: out << "all_rot_regalloc"; break;

      case EL_TREE_HYPERBLOCK: out << "tree_has_hb"; break;
      case EL_TREE_SUPERBLOCK: out << "tree_has_sb"; break;
      case EL_TREE_REGISTER_ALLOCATED: out << "tree_regalloc"; break;
      case EL_TREE_SCHEDULED: out << "tree_sched"; break;

      // RMR 021702 - adder MARK1 and MARK2
      case EL_OPER_MARK1:
      case EL_OPER_MARK2:

      // SLARSEN - don't swp flag
      case EL_DONT_SWP: out << "dont_swp"; break;

      case EL_REGION_LAST_FLAG: break;

      default:
	cout << "unknown flag: " << flag << endl;
	out << "??_unknown_region_flag??";  break;
    }
    return out;
}
    

IR_outstream &operator<<(IR_outstream &out, const Hash_set<Edge*> &edges)
{
    // Writes out all edges
    Hash_set_iterator<Edge*> ptr;
    for (ptr(edges);  ptr != 0;  ptr++)
	out << **ptr << endline;
    return out;
}

//-------------------------- Reader -------------------------------

Basicblock *bb(IR_instream &in);
Hyperblock *hb(IR_instream &in);
LoopBody *loop_body(IR_instream &in);
Procedure *procedure(IR_instream &in);
Tree *tree(IR_instream &in);
void compound_region_parameters(Compound_region *region, IR_instream &in);
bool weight(IR_instream &in, Compound_region *region);
bool entry_ops(IR_instream &in, Compound_region *region);
bool exit_ops(IR_instream &in, Compound_region *region);
bool entry_edges(IR_instream &in, Compound_region *region);
bool exit_edges(IR_instream &in, Compound_region *region);
bool attr(IR_instream &in, Compound_region *region);
bool subregions(IR_instream &in, Compound_region *region);
bool elcor_region_flag(IR_instream &in, Elcor_region_flag &flag);


Region *region(IR_instream &in)
{
    // Extracts a region from the input, if there is one;

    Region *reg;
    if ((reg = op(in)))
	return reg;
    else if ((reg = bb(in)))
	return reg;
    else if ((reg = hb(in)))
	return reg;
    else if ((reg = loop_body(in)))
	return reg;
    else if ((reg = procedure(in)))
	return reg;
    else if ((reg = tree(in)))
	return reg;
    else
	return NULL;
}


Basicblock *bb(IR_instream &in)
{
    // Extracts a BB from the input, if there is one;

    IR_token token;
    in >> token;
    if (token == BB_STRING) {
	int id;
	in >> id;
	Basicblock* bb = new Basicblock(id, 0.0);
	/* bb->set_id(id); */
	compound_region_parameters(bb, in);
	return bb;
    } else {
	in.unget_token();
	return NULL;
    }
}


Hyperblock *hb(IR_instream &in)
{
    // Extracts an HB from the input, if there is one;

    IR_token token;
    in >> token;
    if (token == HB_STRING) {
	int id;
	in >> id;
	Hyperblock* hb = new Hyperblock(id, 0.0);
	/* hb->set_id(id); */
	compound_region_parameters(hb, in);
	return hb;
    } else {
	in.unget_token();
	return NULL;
    }
}


LoopBody *loop_body(IR_instream &in)
{
    // Extracts a LoopBody from the input, if there is one;

    IR_token token;
    in >> token;
    if (token == LOOPBODY_STRING) {
	int id;
	in >> id;
	LoopBody *loop_body = new LoopBody(id, 0.0);
	/* loop_body->set_id(id); */
	compound_region_parameters(loop_body, in);
	return loop_body;
    } else {
	in.unget_token();
	return NULL;
    }
}


Procedure *procedure(IR_instream &in)
{
    // Extracts a Procedure from the input, if there is one;

    IR_token token;
    in >> token;
    if (token == PROC_STRING) {
	Procedure *procedure = new Procedure();
	char *name;
	int id;
	in >> name >> id;
	procedure->set_name(eString(name));
	procedure->set_id(id);
	compound_region_parameters(procedure, in);
	return procedure;
    } else {
	in.unget_token();
	return NULL;
    }
}


Tree *tree(IR_instream &in)
{
    // Extracts a Tree from the input, if there is one;

    IR_token token;
    in >> token;
    if (token == TREE_STRING) {
	Tree *tree = new Tree();
	int id;
	in >> id;
	tree->set_id(id);
	compound_region_parameters(tree, in);
	return tree;
    } else {
	in.unget_token();
	return NULL;
    }
}

void
compound_region_parameters(Compound_region *region, IR_instream &in)
{
    // Extract compound region parameters and install them in region;

    in >> L_PAREN;
    for (;;) {
	if (flags(in, region))
	    continue;
	else if (weight(in, region))
	    continue;
	else if (entry_ops(in, region))
	    continue;
	else if (exit_ops(in, region))
	    continue;
	else if (entry_edges(in, region))
	    continue;
	else if (exit_edges(in, region))
	    continue;
	else if (attr(in, region))
	    continue;
	else if (subregions(in, region))
	    continue;
	else
	    break;
    }
    in >> R_PAREN;
}


bool flags(IR_instream &in, Region *region)
{
    // Extract flags from the input stream and update the region with them.
    // Returns true if flags found, false if no flags found;

    IR_token token;
    in >> token;
    if (token != REGION_FLAGS_STRING) {
	in.unget_token();
	return false;
    } else {
	in >> L_PAREN;
	Elcor_region_flag flag;
	while (elcor_region_flag(in, flag))
	    region->set_flag(flag);
	in >> R_PAREN;
	return true;
    }
}


bool weight(IR_instream &in, Compound_region *region)
{
    // Extract weight from the input stream and update the region with it.
    // Returns true if weight found, false if not found;

    IR_token token;
    in >> token;
    if (token != REGION_WEIGHT_STRING) {
	in.unget_token();
	return false;
    } else {
	double wt;
	in >> L_PAREN >> wt >> R_PAREN;
	region->weight = wt;
	return true;
    }
}


bool entry_ops(IR_instream &in, Compound_region *region)
{
    // Extract entry ops from the input stream and update the region with it.
    // Returns true if found, false if not found;

    IR_token token;
    in >> token;
    if (token != REGION_ENTRY_OPS_STRING) {
	in.unget_token();
	return false;
    } else {
	IR_token token;
	in >> L_PAREN >> token;
	while (token.token_type() == NUMBER) {
	    int id = int(token);
	    Op *op;
	    if (op_table[id] != NULL)
		op = (Op*) op_table[id];
	    else 
		op_table[id] = op = new Op(id);
	    region->add_entry(op);
	    in >> token;
	}
	if (token !=  R_PAREN)
	    in.syntax_error(token);
	return true;
    }
}


bool exit_ops(IR_instream &in, Compound_region *region)
{
    // Extract exit ops from the input stream and update the region with it.
    // Returns true if found, false if not found;

    IR_token token;
    in >> token;
    if (token != REGION_EXIT_OPS_STRING) {
	in.unget_token();
	return false;
    } else {
	IR_token token;
	in >> L_PAREN >> token;
	while (token.token_type() == NUMBER) {
	    int id = int(token);
	    Op *op;
	    if (op_table[id] != NULL)
		op = (Op*) op_table[id];
	    else 
		op_table[id] = op = new Op(id);
	    region->add_exit(op);
	    in >> token;
	}
	if (token !=  R_PAREN)
	    in.syntax_error(token);
	return true;
    }
}


bool entry_edges(IR_instream &in, Compound_region *region)
{
    // Extract entry edges from the input stream and update the region with it.
    // Returns true if found, false if not found;

    IR_token token;
    in >> token;
    if (token != REGION_ENTRY_EDGES_STRING) {
	in.unget_token();
	return false;
    } else {
	Edge *edge;
	in >> L_PAREN;
	while (edge_ptr(in, edge))
	    region->add_entry_safely(edge);
	in >> R_PAREN;
	return true;
    }
}


bool exit_edges(IR_instream &in, Compound_region *region)
{
    // Extract exit edges from the input stream and update the region with it.
    // Returns true if found, false if not found;

    IR_token token;
    in >> token;
    if (token != REGION_EXIT_EDGES_STRING) {
	in.unget_token();
	return false;
    } else {
	Edge *edge;
	in >> L_PAREN;
	while (edge_ptr(in, edge))
	    region->add_exit_safely(edge);
	in >> R_PAREN;
	return true;
    }
}


bool attr(IR_instream &in, Compound_region *region)
{
  // Extract attributes from the input stream and update the region with it.
  // Returns true if found, false if not found;

  IR_token token;
  Base_attribute* attr;
  in >> token;
  if (token != ATTR_STRING) {
    in.unget_token();
    return false;
  } else {
    in >> L_PAREN;
    while ((attr = scan_attribute_ptr(in))) {
      region->attributes->insert_attribute(attr->attribute_type(), attr);
    }
    in >> R_PAREN;
    return true;
  }
}

bool subregions(IR_instream &in, Compound_region *reg)
{
    // Extracts subregions from the input stream and attaches them to region;

    IR_token token;
    in >> token;
    if (token != REGION_SUBREGIONS_STRING) {
	in.unget_token();
	return false;
    } else {
	in >> L_PAREN;
	Region *subregion;
	while ((subregion = region(in))) {
	    reg->add_region(subregion);
	    subregion->set_parent(reg);
	}
	in >> R_PAREN;
	return true;
    }
}


bool elcor_region_flag(IR_instream &in, Elcor_region_flag &flag)
{
    // Extracts an Elcor region flag from the input;

    flag = EL_OPER_CHECK;
    IR_token token;
    in >> token;
    if (token.token_type() != IDENTIFIER) {
	in.unget_token();
	return false;
    }
    else if (token == (const char*)"check") flag = EL_OPER_CHECK;
    else if (token == (const char*)"label") flag = EL_OPER_LABEL_REFERENCE;
    else if (token == (const char*)"prom") flag = EL_OPER_PROMOTED;
    else if (token == (const char*)"squash") flag = EL_OPER_SQUASHING;
    else if (token == (const char*)"dspec") flag = EL_OPER_DATA_SPECULATIVE;
    else if (token == (const char*)"spill") flag = EL_OPER_SPILL_CODE;
    else if (token == (const char*)"callee") flag = EL_OPER_CALLEE_SAVE;
    else if (token == (const char*)"caller") flag = EL_OPER_CALLER_SAVE;
    else if (token == (const char*)"sef") flag = EL_OPER_SIDE_EFFECT_FREE;
    else if (token == (const char*)"spec") flag = EL_OPER_SPECULATIVE;
    else if (token == (const char*)"mask") flag = EL_OPER_MASK_PE;
    else if (token == (const char*)"safe") flag = EL_OPER_SAFE_PEI;
    else if (token == (const char*)"probe") flag = EL_OPER_PROBE_MARK;
    else if (token == (const char*)"sync") flag = EL_OPER_SYNC;
    else if (token == (const char*)"pspec") flag = EL_OPER_PROCESSOR_SPECIFIC;
    else if (token == (const char*)"vol") flag = EL_OPER_VOLATILE;
    else if (token == (const char*)"nospec") flag = EL_OPER_NO_SPECULATION;
    else if (token == (const char*)"stack") flag = EL_OPER_STACK_REFERENCE;
    else if (token == (const char*)"rotate") flag = EL_OPER_ROTATE_REGISTERS;
    else if (token == (const char*)"superspec") flag = EL_OPER_SUPER_SPECULATION;

    else if (token == (const char*)"jrg") flag = EL_OPER_TABLE_JUMP;
    else if (token == (const char*)"real_merge") flag = EL_OPER_REAL_MERGE;
    else if (token == (const char*)"nonseq_db") flag = EL_OPER_NONSEQ_DUMMY_BR;
    else if (token == (const char*)"liveout") flag = EL_OPER_LIVEOUT_DEST;
    else if (token == (const char*)"cvt") flag = EL_OPER_CVT;
    else if (token == (const char*)"icmove") flag = EL_OPER_ICMOVE;
    else if (token == (const char*)"swp_brn") flag = EL_OPER_SWP_BRANCH;
    else if (token == (const char*)"cgen_move") flag = EL_OPER_CGEN_MOVE;
    else if (token == (const char*)"stack_mod") flag = EL_OPER_STACK_MOD;

    else if (token == (const char*)"hb") flag = EL_REGION_HYPERBLOCK;
    else if (token == (const char*)"hb_no_ft") flag = EL_REGION_HYPERBLOCK_NO_FALLTHRU;
    else if (token == (const char*)"softpipe") flag = EL_REGION_SOFTPIPE;
    else if (token == (const char*)"remainder") flag = EL_REGION_REMAINDER_BLOCK;
    else if (token == (const char*)"sb") flag = EL_REGION_SUPERBLOCK;
    else if (token == (const char*)"unrolled") flag = EL_REGION_UNROLLED;
    else if (token == (const char*)"entr_bndry") flag = EL_REGION_ENTRANCE_BOUNDARY;
    else if (token == (const char*)"exit_bndry") flag = EL_REGION_EXIT_BOUNDARY;
    else if (token == (const char*)"prologue") flag = EL_REGION_HAS_PROLOGUE_OP;
    else if (token == (const char*)"epilogue") flag = EL_REGION_HAS_EPILOGUE_OP;
    else if (token == (const char*)"violates_lc") flag = EL_REGION_VIOLATES_LC_SEMANTICS;
    else if (token == (const char*)"rot_regalloc") flag = EL_REGION_ROT_REG_ALLOCATED;

    else if (token == (const char*)"hb_frp") flag = EL_REGION_HYPERBLOCK_FRP;
    else if (token == (const char*)"regalloc") flag = EL_REGION_REGISTER_ALLOCATED;
    else if (token == (const char*)"sched") flag = EL_REGION_SCHEDULED;
    else if (token == (const char*)"preloop") flag = EL_REGION_PRELOOP_BLOCK;
    else if (token == (const char*)"postloop") flag = EL_REGION_POSTLOOP_BLOCK;
    else if (token == (const char*)"explicit_ft") flag = EL_REGION_HAS_EXPLICIT_FT;
    else if (token == (const char*)"cpr_blk") flag = EL_REGION_CPR_BLOCK;
    else if (token == (const char*)"cpr_comp_blk") flag = EL_REGION_CPR_COMP_BLOCK;
    else if (token == (const char*)"analysis_info") flag = EL_REGION_HAS_ANALYSIS_INFO;

    else if (token == (const char*)"has_hb") flag = EL_PROC_HYPERBLOCK;
    else if (token == (const char*)"leaf") flag = EL_PROC_LEAF;
    else if (token == (const char*)"all_sef") flag = EL_PROC_SIDE_EFFECT_FREE;
    else if (token == (const char*)"all_regalloc") flag = EL_PROC_REGISTER_ALLOCATED;
    else if (token == (const char*)"has_sb") flag = EL_PROC_SUPERBLOCK;
    else if (token == (const char*)"has_mask") flag = EL_PROC_MASK_PE;
    else if (token == (const char*)"comp_done") flag = EL_PROC_COMPILATION_COMPLETE;
    else if (token == (const char*)"all_sched") flag = EL_PROC_SCHEDULED;
    else if (token == (const char*)"all_cc_in_pregs") flag = EL_PROC_CC_IN_PREDICATE_REGS;
    else if (token == (const char*)"all_pregs_in_attr") flag = EL_PROC_PRED_REGS_IN_ATTR;

    else if (token == (const char*)"needs_suif_input_fixup") flag = EL_PROC_NEEDS_SUIF_FIXUP;
    else if (token == (const char*)"all_rot_regalloc") flag = EL_PROC_ROT_REG_ALLOCATED;

    else if (token == (const char*)"tree_has_hb") flag = EL_TREE_HYPERBLOCK;
    else if (token == (const char*)"tree_has_sb") flag = EL_TREE_SUPERBLOCK;
    else if (token == (const char*)"tree_regalloc") flag = EL_TREE_REGISTER_ALLOCATED;
    else if (token == (const char*)"tree_sched") flag = EL_TREE_SCHEDULED;

    // SLARSEN - don't swp flag
    else if (token == (const char*)"dont_swp") flag = EL_DONT_SWP;

    else {
	in.unget_token();
	return false;
    }
    return true;
}
