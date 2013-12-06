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
//      File:           ir_edge.cpp
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    Elcor Text IR reader-writer for class Edge
//
///////////////////////////////////////////////////////////////////////////

#include "attributes.h"
#include "op.h"
#include "ir_edge.h"
#include "ir_op.h"
#include "IR_symbol.h"
#include "ir_attribute.h"

//-------------------------- Writer -------------------------------

IR_outstream &operator<<(IR_outstream &out, const Port_type &port_type);


IR_outstream &operator<<(IR_outstream &out, const Edge *edge)
{
    // Write out a pointer to an edge;
    if (edge->is_control()) {
	out << CONTROL_EDGE_STRING;
    }
    else if (edge->is_reg_flow()) {
	out << REG_FLOW_EDGE_STRING; 
    }
    else if (edge->is_reg_anti()) {
	out << REG_ANTI_EDGE_STRING;
    }
    else if (edge->is_reg_out()) {
	out << REG_OUTPUT_EDGE_STRING;
    }
    else if (edge->is_mem()) {
	out << MEM_EDGE_STRING;
    }
    else {
	out << "(unknown?)";
    }
    out << (void *) edge;
    return out;
}

IR_outstream &operator<<(IR_outstream &out, const Edge &edge)
{
    // Write out an edge;
    out << &edge << SPACE << L_PAREN;
    
    // Source;
    out << OP_STRING << edge.src()->id()
	<< L_BRACKET << int(edge.src_port()) << int(edge.src_alt())
	<< R_BRACKET << edge.src_port_type();		     

    out << SPACE << EQUALS << R_ANGLE;

    // Dest;
    out << OP_STRING << edge.dest()->id()
	<< L_BRACKET << int(edge.dest_port()) << int(edge.dest_alt())
	<< R_BRACKET << edge.dest_port_type();		     

    // Edge parameters;
    if (!edge.is_semantic())
	out << "usage" << L_PAREN << "ann" << R_PAREN;
    if (!edge.is_certain())
	out << "status" << L_PAREN << "maybe" << R_PAREN;
    out << EDGE_LATENCY_STRING << L_PAREN << edge.latency() << R_PAREN;

    // Edge attributes;
    bool print_header = false;
    for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
      Base_attribute* attr = edge.attributes->get_attribute((Attribute_type)type);
      // check if the attribute exists and supports Rebel IO
      if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
      ir_assert(attr->is_edge_attribute());
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
      out << R_PAREN;

    out << R_PAREN;
    return out;
}


IR_outstream &operator<<(IR_outstream &out, const Port_type &port_type)
{
    // Write out a port type;

    Port_type type = port_type;
    if (type == FIRST_PORT)
	type = CONTROL;
    switch (type) {
      case CONTROL:	out << CONTROL_PORT_STRING;  break;
      case MEM:		out << MEMORY_PORT_STRING;  break;
      case SRC:		out << SRC_PORT_STRING;  break;
      case DEST:	out << DEST_PORT_STRING;  break;
      default:
	El_punt("Trying to print out an illegal port name");
    }
    return out;
}


void name_attributes(IR_outstream &out, Edge *edge)
{
  // Name all attributes attached to an edge;

  // First get attributes of this edge
  for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
    Base_attribute* attr = edge->attributes->get_attribute((Attribute_type)type);
    // check if the attribute exists and supports Rebel IO
    if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
    ir_assert(attr->is_edge_attribute());
    // map its pointer unless the attribute is to be inlined
    if (!attr->is_inline()) out.map_pointer((void *) attr);
  }
}
    

void print_attributes(IR_outstream &out, Edge *edge)
{
    // Print all attributes attached to an edge.

  // First get attributes of this op
  for (int type=(int)FIRST_ATTRIBUTE; type < (int)LAST_ATTRIBUTE; type++) {
    Base_attribute* attr = edge->attributes->get_attribute((Attribute_type)type);
    // check if the attribute exists and supports Rebel IO
    if (!attr || attr->is_empty() || !attr->has_rebel_io()) continue;
    ir_assert(attr->is_edge_attribute());
    // print the attributes that are pointers, inlined ones are already printed
    if (!attr->is_inline()) out << *attr << endline << tab;
  }
}

//-------------------------- Reader -------------------------------

void edge_attributes(IR_instream &in, Edge *edge);
void edge_parameters(IR_instream &in, Edge *edge);
Port_type port_type(IR_instream &in);


bool edge_ptr(IR_instream &in, Edge *&edge)
{
    // Extracts a "ptr" to an edge from instream, if any, and returns a
    // pointer to the corresponding edge data structure;

    IR_token token;
    in >> token;
    enum EdgeType {
	CTRL, REG_FLOW, REG_ANTI, REG_OUT, MEM_EDGE
    } edge_type;
    if (token == CONTROL_EDGE_STRING)
	edge_type = CTRL;
    else if (token == REG_FLOW_EDGE_STRING)
	edge_type = REG_FLOW;
    else if (token == REG_ANTI_EDGE_STRING)
	edge_type = REG_ANTI;
    else if (token == REG_OUTPUT_EDGE_STRING)
	edge_type = REG_OUT;
    else if (token == MEM_EDGE_STRING)
	edge_type = MEM_EDGE;
    else {
	in.unget_token();
	return false;
    }
    void *object_ptr;
    in >> object_ptr;
    if (object_ptr == NULL) {
	switch (edge_type) {
	  case CTRL:	edge = new Control();  break;
	  case REG_FLOW:edge = new Reg_flow();  break;
	  case REG_ANTI:edge = new Reg_anti();  break;
	  case REG_OUT:	edge = new Reg_out();  break;
	  case MEM_EDGE:edge = new Mem();  break;
	}
	in.map_pointer(edge);
    } else
	edge = (Edge*)object_ptr;
    return true;
}

Edge *edge(IR_instream &in)
{
    // Extracts an edge from instream, if any, and returns it;
    // pointer to the corresponding edge data structure;

    Edge *e_ptr;
    if (edge_ptr(in, e_ptr)) {
	int src_op_id, src_port_id, alt_src_port_id;
	int dest_op_id, dest_port_id, alt_dest_port_id;

	in >> L_PAREN >> OP_STRING >> src_op_id >> L_BRACKET >> src_port_id
	    >> alt_src_port_id >> R_BRACKET;
	Port_type src_port_type = port_type(in);

	in >> EQUALS >> R_ANGLE;
	   
	in >> OP_STRING >> dest_op_id >> L_BRACKET >> dest_port_id
	   >> alt_dest_port_id >> R_BRACKET;
	Port_type dest_port_type = port_type(in);

	Op *src = (Op *) op_table[src_op_id];
	Op *dest = (Op *) op_table[dest_op_id];

	// Connect ops to edges;
	e_ptr->set_src(src, src_port_id, alt_src_port_id);
	e_ptr->set_dest(dest, dest_port_id, alt_dest_port_id);

	// Connect edges to ops;
	dest->add_inedge(e_ptr, dest_port_type, dest_port_id,alt_dest_port_id);
	src->add_outedge(e_ptr, src_port_type, src_port_id, alt_src_port_id);

	edge_parameters(in, e_ptr);
	edge_attributes(in, e_ptr);
	in >> R_PAREN;	
	return e_ptr;
    } else
	return NULL;
}
    

void edge_parameters(IR_instream &in, Edge *edge)
{
    // Extract edge parameters from the input and attache them to edge;

    IR_token token;
    char *string;
    for (;;) {
	in >> token;
	if (token == (const char*)"usage") {
	    in >> L_PAREN >> string;
	    if (string == "ann")
		edge->set_usage(E_ANNOTATE);
	    else if (string == "sem")
		edge->set_usage(E_SEMANTIC);
	    else
		in.syntax_error(token);
	    in >> R_PAREN;
	} else if (token == (const char*)"status") {
	    in >> L_PAREN >> string;
	    if (string == "certain")
		edge->set_status(E_CERTAIN);
	    else if (string == "maybe")
		edge->set_status(E_MAYBE);
	    else
		in.syntax_error(token);
	    in >> R_PAREN;
	} else if (token == EDGE_LATENCY_STRING) {
	    int time;
	    in >> L_PAREN >> time >> R_PAREN;
	    edge->set_latency(time);
	} else {
	    in.unget_token();
	    break;
	}
    }
}


void edge_attributes(IR_instream &in, Edge *edge)
{
  // Extracts edge attributes from the input and attaches them to edge;
  IR_token token;
  Base_attribute* attr;
  in >> token;
  if (token != ATTR_STRING) {
    in.unget_token();
  } else {
    in >> L_PAREN;
    while ((attr = scan_attribute_ptr(in))) {
      edge->attributes->insert_attribute(attr->attribute_type(), attr);
    }
    in >> R_PAREN;
  }
}

Port_type port_type(IR_instream &in)
{
    // Read in a port type;

    IR_token token;
    in >> token;
    if (token == CONTROL_PORT_STRING)
	return CONTROL;
    else if (token == MEMORY_PORT_STRING)
	return MEM;
    else if (token == SRC_PORT_STRING)
	return SRC;
    else if (token == DEST_PORT_STRING)
	return DEST;
    else {
	in.syntax_error(token);
	return MEM;
    }
}

