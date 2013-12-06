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
//      File:           ir_op.cpp
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    Elcor Text IR reader-writer for class Op
//
///////////////////////////////////////////////////////////////////////////

#include "op.h"
#include "opcode.h"
#include "opcode_properties.h"
#include "op_attributes.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "IR_array.h"
#include "ir_op.h"
#include "ir_region.h"
#include "IR_symbol.h"
#include "ir_attribute.h"
    


//-------------------------- Writer -------------------------------

void print_edge_dests(IR_outstream &out, const List<Edge*>& edges);
void print_edge_srcs(IR_outstream &out, const List<Edge*>& edges);


IR_outstream &operator<<(IR_outstream &out, const Op *op)
{
    // Write out a "pointer" to an op;

    out << OP_STRING << op->id();
    return out;
}

    
IR_outstream &operator<<(IR_outstream &out, const Op &op)
{
   int port ;
    // Write out a "pointer" to and "body" of an op;

    // Pointer; start body; id; opcode;
    out << &op << SPACE << L_PAREN 
      << (char *) el_opcode_to_string_map.value(op.opcode()) << SPACE;
      //<< get_mdes_opcode_string(op.opcode) << SPACE;      

    // Dest operands;
    out << L_BRACKET;
    for (port = int(op.first_dest()); port <= int(op.last_dest()); port++)
	out << op.dest(Port_num(port));
    out << R_BRACKET << SPACE;

    // Src operands;
    out << L_BRACKET;
    for (port = int(op.first_src()); port <= int(op.last_src()); port++)
	out << op.src(Port_num(port));
    out << R_BRACKET << SPACE;
    
    // Optional fields (not written if they have default values);
    if (op.predicated()) out << op.src(PRED1);
    if (op.sched_time()!=-1) out << SCHED_TIME_STRING << L_PAREN
	<< op.sched_time() << R_PAREN;
    if (op.sched_opcode()) out 
	<< SCHED_OPCODE_STRING << L_PAREN << op.sched_opcode()
	<< R_PAREN;
    if (op.custom_opcode()) out 
	<< CUSTOM_OPCODE_STRING << L_PAREN << op.custom_opcode()
	<< R_PAREN;
    if ((op.related_ops) && (op.related_ops->size() > 0)) {
	out << RELATED_OPS_STRING << L_PAREN;
	for(List_iterator<Op*> ptr(*op.related_ops);  ptr != 0;  ptr++)
	    out << (*ptr)->id();
	out << R_PAREN;
    }

    // attributes;
    bool print_header = false;
    for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
      Base_attribute* attr = op.attributes->get_attribute((Attribute_type)type);
      // check if the attribute exists and supports Rebel IO
      if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
      ir_assert(attr->is_op_attribute());
      // print header on first time
      if (!print_header) {
	print_header = true;
	out << ATTR_STRING << L_PAREN;
      }
      // print pointer or inline the attribute
      if (attr->is_inline()) out << *attr;
      else out << attr;
    }
    if (print_header) out << R_PAREN;

    Hash_set_iterator<Edge*> ptr;
    if (/* is_merge((Op*) &op) ||*/ is_control_merge((Op*) &op)) {
	out << OP_IN_EDGES_STRING << L_PAREN;
	print_edge_srcs(out, op.edges_in);
	out << R_PAREN;
    } else if (is_branch((Op*) &op)) {
	out << OP_OUT_EDGES_STRING << L_PAREN;
	print_edge_dests(out, op.edges_out);
	out << R_PAREN;
    }
    if (op.region_flags.ones_count() > 0)
	out << op.region_flags;
    out << R_PAREN;
    return out;
}



void print_edge_dests(IR_outstream &out, const List<Edge*>& edges)
{
    // Write out the destinations of a list of edges (an op is the dest of
    // an edge), in ascending order of op id.
    
    List_iterator<Edge*> ptr;
    const int MAX_DESTS = 10000;
    Edge *edge_ptrs[MAX_DESTS];
    int edge_count = 0;
    for (ptr(edges);  ptr != 0;  ptr++) {
	Edge *edge = *ptr;
        if (! edge->is_seq()) continue;

	// Find slot to place edge so that they will be sorted;
	int slot;
	for (slot = 0;  slot < edge_count;  slot++) {
	    if(edge->dest()->id() < edge_ptrs[slot]->dest()->id())
		break;
	}

	// Make room for new edge
	int s;
	for (s = edge_count;  s > slot;  s--)
	    edge_ptrs[s] = edge_ptrs[s-1];

	// Add new edge into slot;
	edge_ptrs[s] = edge;

	edge_count++;
	ir_assert(edge_count < MAX_DESTS);
    }

    // Now print the sorted pointers;
    for (int i = 0; i < edge_count;  i++) {
	Edge *edge = (Edge*)edge_ptrs[i];
	Control_flow_freq *cfreq = get_control_flow_freq(edge);
	if (cfreq != NULL)
	    out << OP_STRING << DASH << edge->dest()->id() << L_PAREN
	        << cfreq->freq << R_PAREN;
	else
	    out << OP_STRING << DASH << edge->dest()->id();
    }
}


void print_edge_srcs(IR_outstream &out, const List<Edge*>& edges)
{
    // Write out the sources of a list of edges (an op is the src of
    // an edge), in ascending order of op id.
    
    List_iterator<Edge*> ptr;
    const int MAX_SRCS = 10000;
    Edge *edge_ptrs[MAX_SRCS];
    int edge_count = 0;
    for (ptr(edges);  ptr != 0;  ptr++) {
	Edge *edge = *ptr;
        if (! edge->is_seq()) continue;

	// Find slot to place edge so that they will be sorted;
	int slot;
	for (slot = 0;  slot < edge_count;  slot++) {
	    if(edge->src()->id() < edge_ptrs[slot]->src()->id())
		break;
	}

	// Make room for new edge
	int s;
	for (s = edge_count;  s > slot;  s--)
	    edge_ptrs[s] = edge_ptrs[s-1];

	// Add new edge into slot;
	edge_ptrs[s] = edge;

	edge_count++;
	ir_assert(edge_count < MAX_SRCS);
    }

    // Now print the sorted pointers;
    for (int i = 0; i < edge_count;  i++) {
	Edge *edge = (Edge*)edge_ptrs[i];
	Control_flow_freq *cfreq = get_control_flow_freq(edge);
	if (cfreq != NULL)
	    out << OP_STRING << DASH << edge->src()->id() << L_PAREN
	        << cfreq->freq << R_PAREN;
	else
	    out << OP_STRING << DASH << edge->src()->id();
    }
}


void name_edges(IR_outstream &out, Op *op)
{
    // Scan for all edges in an op and create IR_outstream
    // names (of the form "^integer") for them.  To make sure
    // we name each edge only once, we follow these rules:
    //	1. name all inedges
    //  2. name outedges only if they have no destination.

    List_iterator<Edge*> ptr;
    for(ptr(op->edges_in);  ptr != 0;  ptr++) {
	(void) out.map_pointer((void*)*ptr);
    }
    for(ptr(op->edges_out);  ptr != 0;  ptr++) { 
	if ((*ptr)->dest() == NULL) {
	    (void) out.map_pointer((void*)*ptr);
	}
    }
}


void print_edges(IR_outstream &out, Op *op)
{
    // Print all edges in an op. To make sure
    // we print each edge only once, we follow these rules:
    //	1. print all inedges
    //  2. print outedges only if they have no destination.

    List_iterator<Edge*> ptr;
    for(ptr(op->edges_in);  ptr != 0;  ptr++) {
	out << endline << tab << (**ptr);
    }
    for(ptr(op->edges_out);  ptr != 0;  ptr++) { 
	if ((*ptr)->dest() == NULL) {
	    out << endline << tab << (**ptr);
	}
    }
}


void name_attributes(IR_outstream &out, Op *op)
{
  // Scan for all attributes in an op and create IR_outstream
  // names (of the form "^integer") for them;

  // First get attributes of this op
  for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
    Base_attribute* attr = op->attributes->get_attribute((Attribute_type)type);
    // check if the attribute exists and supports Rebel IO
    if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
    ir_assert(attr->is_op_attribute());
    // map its pointer unless the attribute is to be inlined
    if (!attr->is_inline()) out.map_pointer((void *) attr);
  }

  // We now have to scan all edges and name all attributes attached to the
  // edges.
  List_iterator<Edge*> ptr;
  for(ptr(op->edges_in);  ptr != 0;  ptr++)
    name_attributes(out, *ptr);
  for(ptr(op->edges_out);  ptr != 0;  ptr++)
    name_attributes(out, *ptr);
}


void print_attributes(IR_outstream &out, Op *op)
{
  // Scan for all attributes in an op and print them;

  // First get attributes of this op
  for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
    Base_attribute* attr = op->attributes->get_attribute((Attribute_type)type);
    // check if the attribute exists and supports Rebel IO
    if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
    ir_assert(attr->is_op_attribute());
    // print the attributes that are pointers, inlined ones are already printed
    if (!attr->is_inline()) out << *attr << endline << tab;
  }

  // We now have to scan all edges and name all attributes attached to the
  // edges.

  List_iterator<Edge*> ptr;
  for(ptr(op->edges_in);  ptr != 0;  ptr++)
    print_attributes(out, *ptr);
  for(ptr(op->edges_out);  ptr != 0;  ptr++)
    print_attributes(out, *ptr);
}




//-------------------------- Reader -------------------------------

void dest_operands(IR_instream &in, Op *op);
void src_operands(IR_instream &in, Op *op);
void predicate(IR_instream &in, Op *op);
void op_parameters(IR_instream &in, Op *op);
bool sched_opcode(IR_instream &in, Op *op);
bool custom_opcode(IR_instream &in, Op *op);
bool related_ops(IR_instream &in, Op *op);
bool sched_time(IR_instream &in, Op *op);
bool in_edges(IR_instream &in, Op *op);
bool out_edges(IR_instream &in, Op *op);
bool bb_id(IR_instream &in, Op *op);
bool attr(IR_instream &in, Op *op);


// The following table stores pointer to all ops recovered from input stream;

IR_array op_table;


Op *op(IR_instream &in)
{
    // Return an op from the input stream if there is one, otherwise
    // return NULL;


    IR_token token;
    in >> token;
    if (token == OP_STRING) {
	int id;
	char *opcode_string;
	in >> id >> L_PAREN >> opcode_string;

	// Without this check, a nasty segfault occurs that wipes out
	// the gdb stack so you can't even debug it!  And this is for
	// a rebel source file with an unrecognized operation.  To
	// make sure this doesn't happen again, I added this simple
	// test.  dcronqui, 10/19/99
	eString e_opcode(opcode_string);
	if (! el_string_to_opcode_map.is_bound(e_opcode)) {
	   in.syntax_error(token);
	}
	Opcode opcode = el_string_to_opcode_map.value(e_opcode);
	Op *op = new Op(opcode, id);

	// If the op already exists in the op table (because some other op
	// referenced before it was defined) we need to copy this new op
	// into the one that already exists;

	if (op_table[id] != NULL) {
	    *((Op*)op_table[id]) = *op;
	    delete op;
	    op = (Op*) op_table[id];
	} else
	    op_table[id] = op;
	op->set_sched_time(-1);	// default;
	/* op->set_id(id); */
	dest_operands(in, op);
	src_operands(in, op);
	predicate(in, op);
        op_parameters(in, op);
	in >> R_PAREN;	      
	return op;
    } else {
	in.unget_token();
	return NULL;
    }
}
	

void dest_operands(IR_instream &in, Op *op)
{
    // Fetch dest operands from instream and install in op;

    in >> L_BRACKET;
    Operand *opnd;
    Port_num port = op->first_dest();
    while ((opnd = read_operand(in))) {
	op->set_dest(port, *opnd);
	port = Port_num((int)(port+1));
    }
    in >> R_BRACKET;
}


void src_operands(IR_instream &in, Op *op)
{
    // Fetch src operands from instream and install in op;

    in >> L_BRACKET;
    Operand *opnd;
    Port_num port = op->first_src();
    while ((opnd = read_operand(in))) {
	op->set_src(port, *opnd);
	port = Port_num((int)(port+1));
    }
    in >> R_BRACKET;
}


void predicate(IR_instream &in, Op *op)
{
    // Fetch (optional) predicate from instream and install in op;
    
    Operand *opnd = read_operand(in);
    if (opnd)
	op->set_src(PRED1, *opnd);
}


void op_parameters(IR_instream &in, Op *op)
{
    // Fetch op parameters from instream and install in op;

    for (;;) {
	if (sched_opcode(in, op))
	    continue;
	else if (related_ops(in, op))
	    continue;
	else if (sched_time(in, op))
	    continue;
	else if (in_edges(in, op))
	    continue;
	else if (out_edges(in, op))
	    continue;
	else if (flags(in, op))
	    continue;
	else if (attr(in, op))
	    continue;
	else if (custom_opcode(in, op))
	    continue;
	else
	    break;
    }
}    


bool sched_opcode(IR_instream &in, Op *op)
{
    // Extract a sched_opcode from the input stream and update the op
    // with that info;

    char opcode_string[1000], *opcode_str_ptr;
    IR_token token;
    opcode_str_ptr = opcode_string;

    in >> token;
    if (token == SCHED_OPCODE_STRING)  {
	in >> L_PAREN >> opcode_str_ptr >> R_PAREN;
	op->set_sched_opcode(opcode_str_ptr);
	return true;
    } else {
	in.unget_token();
	return false;
    }
}

bool custom_opcode(IR_instream &in, Op *op)
{
    // Extract a custom_opcode from the input stream and update the op
    // with that info;

    char opcode_string[1000], *opcode_str_ptr;
    IR_token token;
    opcode_str_ptr = opcode_string;

    in >> token;
    if (token == CUSTOM_OPCODE_STRING)  {
	in >> L_PAREN >> opcode_str_ptr >> R_PAREN;
	op->set_custom_opcode(opcode_str_ptr);
	op->update_num_operands();
	return true;
    } else {
	in.unget_token();
	return false;
    }
}

bool related_ops(IR_instream &in, Op *op)
{
    // Extract related ops from the input stream and update the op with that
    // info;

    IR_token token;
    in >> token;
    if (token == RELATED_OPS_STRING) {
	in >> L_PAREN;
	for (;;) {
	    in >> token;
	    if (token.token_type() == NUMBER) {
		int id = int(token);
		if (op_table[id] == NULL)
		    op_table[id] = new Op(id);
		Op *relop = (Op*) op_table[id];
		op->add_relops(relop);
	    } else {
		in.unget_token();
		break;
	    }
	}
	in >> R_PAREN;
	return true;
    } else {
	in.unget_token();
	return false;
    }
}


bool sched_time(IR_instream &in, Op *op)
{
    // Extract a sched_time from the input stream and update the op
    // with that info;

    IR_token token;
    in >> token;
    if (token == SCHED_TIME_STRING)  {
	int time;
	in >> L_PAREN >> time >> R_PAREN;
	op->set_sched_time(time);
	return true;
    } else {
	in.unget_token();
	return false;
    }
}


bool in_edges(IR_instream &in, Op *op)
{
    // Extract in_edges from the input stream. We can discard them here because
    // they will be entered in the edge dictionary;

    IR_token token;
    in >> token;
    if (token == OP_IN_EDGES_STRING) {
	in >> L_PAREN;
	for (;;) {
	    in >> token;
	    if (token == OP_STRING) {
		int neg_id;
		double flow_freq;
		in >> neg_id;
	        in >> token;
		if (token == L_PAREN)
		    in >> flow_freq >> R_PAREN;
		else
		    in.unget_token();
	    } else {
		in.unget_token();
		break;
	    }
	}
	in >> R_PAREN;
	return true;
    } else {
	in.unget_token();
	return false;
    }
}


bool out_edges(IR_instream &in, Op *op)
{
    // Extract outedges from the input stream. We can discard them here because
    // they will be entered in the edge dictionary;

    IR_token token;
    in >> token;
    if (token == OP_OUT_EDGES_STRING) {
	in >> L_PAREN;
	for (;;) {
	    in >> token;
	    if (token == OP_STRING) {
		int neg_id;
		double flow_freq;
		in >> neg_id;
		in >> token;
		if (token == L_PAREN)
		    in >> flow_freq >> R_PAREN;
		else
		    in.unget_token();
	    } else {
		in.unget_token();
		break;
	    }
	}
	in >> R_PAREN;
	return true;
    } else {
	in.unget_token();
	return false;
    }
}


bool attr(IR_instream &in, Op *op)
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
      op->attributes->insert_attribute(attr->attribute_type(), attr);
    }
    in >> R_PAREN;
    return true;
  }
}

bool mem_vr_dictionary_entry(IR_instream &in)
{
    // Extracts a mem_vr from the input stream, if any, and updates the
    // relevent op.  Returns true if a mem_vr was found;

    IR_token token;
    Operand *opnd;
    int id;
    in >> token;
    if (token == OP_STRING) {
	in >> id >> L_PAREN >> MEMVR_OUT_STRING >> L_PAREN;
	Op *op = (Op*) op_table[id];
	ir_assert(op);
	while ((opnd = read_operand(in)))
	    op->add_mem_dest(*opnd);
	in >> R_PAREN >> MEMVR_IN_STRING >> L_PAREN;
	while ((opnd = read_operand(in)))
	    op->add_mem_src(*opnd);
	in >> R_PAREN >> R_PAREN;
	return true;
    } else {
	in.unget_token();
	return false;
    }
}

