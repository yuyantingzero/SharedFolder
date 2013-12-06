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
//      File:           ir_attribute.cpp
//      Authors:        Greg Snider, Scott A. Mahlke
//      Created:        August 1995
//      Description:    Elcor Text IR reader-writer for class Graph_attribute
//
///////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <stdlib.h>
#include <string>
#include "ir_attribute.h"
#include "ir_op.h"
#include "ir_operand.h"
#include "IR_symbol.h"
#include "attributes.h"
#include "op_attributes.h"
#include "edge_attributes.h"
#include "region_attributes.h"

using namespace std;

// global dispatch functions

//-------------------------------------------------------------------
//Print attribute and attribute pointers. These just dispatch
//to respective classes.
//-------------------------------------------------------------------

IR_outstream &operator<<(IR_outstream &out, Base_attribute *attr) {
  // only non-inline attributes need to print a pointer 
  ir_assert(!attr->is_inline());
  switch (attr->attribute_type()) {
  case REFERENCE_DTYPE_ATTR: out << (Reference_dtype_attr*)attr; break;
  case SPATIAL_REF_ATTR: out << (Spatial_ref_attr*)attr; break;
  case PHYSFILE_BIND_ATTR: out << (Physfile_binding_attr*)attr; break;
  case LCODE_ATTRIBUTES: out << (Lcode_attribute_map_attrib*)attr; break;
  case LIVENESS_SET: out << (Liveness_info_attrib*)attr; break;
  case DOWN_EXPOSED_USES_SET: out << (Downward_use_attrib*)attr; break;
  case UP_EXPOSED_DEFS_SET: out << (Upward_def_attrib*)attr; break;
  case DOWN_EXPOSED_DEFS_SET: out << (Downward_def_attrib*)attr; break;
  case OP_CHAINING_ATTR: out << (Op_chaining_attr*)attr; break;
  case REG_TO_LIVENESS_MAP: out << (Reg_to_liveness_map_attrib*)attr; break;
  case EXSYM_TABLE_ATTR: out << (Exsym_table_attrib*)attr; break;
  case MSCHED_ATTRIBUTES: out << (Msched_attr*)attr; break;
  case ACC_SPEC_ATTR: out << (AccSpecAttr*)attr; break;
  default:
    ir_assert(0); break;
  }
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Base_attribute &attr) {
  switch (attr.attribute_type()) {
  case MERGE_OP_BB_ID: out << (Merge_op_BB_id&)attr; break;
  case REFERENCE_DTYPE_ATTR: out << (Reference_dtype_attr&)attr; break;
  case SPATIAL_REF_ATTR: out << (Spatial_ref_attr&)attr; break;
  case PHYSFILE_BIND_ATTR: out << (Physfile_binding_attr&)attr; break;
  case JUMP_TABLE_NAME: out << (Jump_table_name&)attr; break;
  case CONTROL_FLOW_FREQ: out << (Control_flow_freq&)attr; break;
  case LCODE_ATTRIBUTES: out << (Lcode_attribute_map_attrib&)attr; break;
  case NAMETAG_ATTR: out << (Nametag_attr&)attr; break;
  case LIVENESS_SET: out << (Liveness_info_attrib&)attr; break;
  case DOWN_EXPOSED_USES_SET: out << (Downward_use_attrib&)attr; break;
  case UP_EXPOSED_DEFS_SET: out << (Upward_def_attrib&)attr; break;
  case DOWN_EXPOSED_DEFS_SET: out << (Downward_def_attrib&)attr; break;
  case OP_CHAINING_ATTR: out << (Op_chaining_attr&)attr; break;
  case REG_TO_LIVENESS_MAP: out << (Reg_to_liveness_map_attrib&)attr; break;
  case LOOP_COUNT: out << (Loop_count_attrib&)attr; break;
  case EXSYM_TABLE_ATTR: out << (Exsym_table_attrib&)attr; break;
  case CLUSTER_ID_ATTR: out << (Cluster_id_attr&)attr; break;
  case USCHED_TIME_ATTR: out << (Usched_time_attr&)attr; break;
  case MSCHED_ATTRIBUTES: out << (Msched_attr&)attr; break;
  case BRANCH_TARGET_ATTR: out << (Branch_target_attr&)attr; break;
  case ACC_SPEC_ATTR: out << (AccSpecAttr&)attr; break;
  default:
    ir_assert(0); break;
  }

  return out;
}

//-------------------------------------------------------------------
//Static scan attribute pointer function. returns true if a pointer was
//successfully parsed. Inline objects are parsed directly.
//-------------------------------------------------------------------
Base_attribute* scan_attribute_ptr(IR_instream &in) 
{
  Base_attribute* attr;
  // inline attributes scan the full attribute, others scan a pointer

  // region attributes
  if ((attr = scan_lcode_attribute_map_ptr(in))) return attr;
  else if ((attr = scan_nametag_attr(in))) return attr;
  else if ((attr = scan_reg_to_liveness_map_ptr(in))) return attr;
  else if ((attr = scan_loop_count(in))) return attr;
  else if ((attr = scan_merge_op_bb_id(in))) return attr;
  else if ((attr = scan_jump_table_name(in))) return attr;
  else if ((attr = scan_reference_dtype_attr_ptr(in))) return attr;
  else if ((attr = scan_spatial_ref_attr_ptr(in))) return attr;
  else if ((attr = scan_prf_bind_attr_ptr(in))) return attr;
  else if ((attr = scan_op_chaining_attr_ptr(in))) return attr;
  else if ((attr = scan_exsym_table_ptr(in))) return attr;
  else if ((attr = scan_cluster_id_attr(in))) return attr;
  else if ((attr = scan_usched_time_attr(in))) return attr;
  else if ((attr = scan_msched_attr_ptr(in))) return attr;
  else if ((attr = scan_branch_target_attr(in))) return attr;
  else if ((attr = scan_accspec_attr(in))) return attr;
  // edge attributes
  else if ((attr = scan_control_flow_freq(in))) return attr;
  else if ((attr = scan_liveness_info_ptr(in))) return attr;
  else if ((attr = scan_downward_use_ptr(in))) return attr;
  else if ((attr = scan_upward_def_ptr(in))) return attr;
  else if ((attr = scan_downward_def_ptr(in))) return attr;
  // attribute not recognized
  else return NULL;
}  

//-------------------------------------------------------------------
//Static scan attribute dictionary. returns true if an object was
//successfully parsed.
//-------------------------------------------------------------------
bool scan_attribute_dictionary(IR_instream &in) 
{
  // only non-inline attributes are here
  
  // region attributes
  if (scan_lcode_attribute_map_dictionary(in)) return true;
  else if (scan_reg_to_liveness_map_dictionary(in)) return true;
  else if (scan_reference_dtype_attr_dictionary(in)) return true;
  else if (scan_spatial_ref_attr_dictionary(in)) return true;
  else if (scan_prf_bind_attr_dictionary(in)) return true;
  else if (scan_op_chaining_attr_dictionary(in)) return true;
  else if (scan_exsym_table_dictionary(in)) return true;
  else if (scan_msched_attr_dictionary(in)) return true;
  // edge attributes
  else if (scan_liveness_info_dictionary(in)) return true;
  else if (scan_downward_use_dictionary(in)) return true;
  else if (scan_upward_def_dictionary(in)) return true;
  else if (scan_downward_def_dictionary(in)) return true;
  // attribute not recognized
  else return false;
}  


///////////////////////////////////////////////////////////////////////////
// Lcode_attribute_map_attrib

IR_outstream &operator<<(IR_outstream &out, Lcode_attribute_map_attrib *ptr)
{
  // Print out a pointer to this attribute;
  out << LCODE_ATTR_STRING << (void*)ptr;
  return out;
}

// Printing out really long buffers is extremely inefficient, so
// for large numbers of attributes (or attributes with a large number
// of operands, just put some arbitrary line breaks to speed up the process.
// count1 and count2 below serve these purposes.
// SAM 10-96 
IR_outstream &operator<<(IR_outstream &out, Lcode_attribute_map_attrib &attr)
{
  // Print out an Lcode_attribute_map attribute;
  int count1, count2;

  Lcode_attribute_map_attrib* ptr = &attr;
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;
    count1 = 0;
    Map_iterator<eString, List<Operand> > mptr;
    for (mptr(*(attr.map));  mptr != 0;  mptr++) {
      Pair<eString, List<Operand> > entry = *mptr;
      if (count1 == 10) {
	out << endline; out.indent(); out.tab(); out.outdent();
	count1 = 0;
      }
      // The first entry of the pair is a string, not an identifier,
      // so we must quote it;
      count2 = 0;
      out << L_CURLY << DOUBLE_QUOTE << ((char*)entry.first)
	  << DOUBLE_QUOTE << SPACE << L_PAREN;
      List_iterator<Operand> optr;
      for (optr(entry.second);  optr != 0;  optr++) {
	out << *optr;
        count2++;
        if (count2 == 10) {
	  out << endline; out.indent(); out.tab(); out.outdent();
	  count1 = 0;
	  count2 = 0;
        }
      }
      out << R_PAREN << R_CURLY;
      count1++;
    }
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
} 

Lcode_attribute_map_attrib* scan_lcode_attribute_map_ptr(IR_instream& in)
{
    IR_token token;
    void *object_ptr;
    Lcode_attribute_map_attrib *attr;

    in >> token;
    if (token == LCODE_ATTR_STRING) {
	in >> object_ptr;
	if (object_ptr == NULL) {
            attr = new Lcode_attribute_map_attrib();
            in.map_pointer(attr);
        }
	else {
            attr = (Lcode_attribute_map_attrib*) object_ptr;
        }
	return attr;
    }
    else {
	in.unget_token();
	return NULL;
    }
}


bool scan_lcode_attribute_map_dictionary(IR_instream& in)
{
    // Extracts an lcode attribute, if any, from the input stream and fills in
    // its information.  Returns true if an attribute is found;

    IR_token token;
    in >> token;
    if (token == LCODE_ATTR_STRING) {
	void *ptr;
	in >> ptr;
	if (ptr == NULL) {
	    cerr << "orphaned attribute" << endl;
	    exit(1);
	}
	Lcode_attribute_map_attrib* attr = (Lcode_attribute_map_attrib*) ptr;
	attr->map = new Lcode_attribute_map();
	in >> L_PAREN;
	for (in >> token;  token == L_CURLY;  in >> token) {
	    in >> token;
	    eString *ident = new eString((char*) token);
	    in >> L_PAREN;
	    List<Operand> *op_list = new List<Operand>;
	    Operand *op;
	    while ((op = read_operand(in)))
		op_list->add_tail(*op);
	    in >> R_PAREN >> R_CURLY;
	    attr->map->bind(*ident, *op_list);
	}
	if (token != R_PAREN)
	    in.syntax_error(token);
	return true;
    } else {
	in.unget_token();
	return false;
    }
}

///////////////////////////////////////////////////////////////////////////
// Reg_to_liveness_map_attrib

IR_outstream &operator<<(IR_outstream &out, Reg_to_liveness_map_attrib *ptr)
{
  // Print out a pointer to this attribute;
  out << REG_TO_LIVENESS_MAP_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Reg_to_liveness_map_attrib &attr)
{
  // Print out an Reg_to_liveness_map attribute;
  // format: ptr ({regnum [livein liveout]} ...)
  Reg_to_liveness_map_attrib* ptr = &attr;
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;
    Map_iterator<int, Pair<int,int> > mptr;
    for (mptr(*(attr.map));  mptr != 0;  mptr++) {
      Pair<int, Pair<int,int> > entry = *mptr;
	   
      out << L_CURLY << entry.first << SPACE << L_BRACKET << entry.second.first 
	  << SPACE << entry.second.second << R_BRACKET << R_CURLY << SPACE;
    }
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
} 

Reg_to_liveness_map_attrib* scan_reg_to_liveness_map_ptr(IR_instream& in)
{
    IR_token token;
    void *object_ptr;
    Reg_to_liveness_map_attrib *attr;

    in >> token;
    if (token == REG_TO_LIVENESS_MAP_STRING) {
	in >> object_ptr;
	if (object_ptr == NULL) {
            attr = new Reg_to_liveness_map_attrib();
            in.map_pointer(attr);
        }
	else {
            attr = (Reg_to_liveness_map_attrib*) object_ptr;
        }
	return attr;
    }
    else {
	in.unget_token();
	return NULL;
    }
}


bool scan_reg_to_liveness_map_dictionary(IR_instream& in)
{
    // Extracts a reg_to_liveness attribute, if any, from the input
    // stream and fills in its information.  Returns true if an
    // attribute is found;

    IR_token token;
    in >> token;
    if (token == REG_TO_LIVENESS_MAP_STRING) {
	void *ptr;
	in >> ptr;
	if (ptr == NULL) {
	    cerr << "orphaned attribute" << endl;
	    exit(1);
	}
	Reg_to_liveness_map_attrib *attr = (Reg_to_liveness_map_attrib*) ptr;
	attr->map = new Reg_to_liveness_map();
	in >> L_PAREN;
	for (in >> token;  token == L_CURLY;  in >> token) {
	    int reg, livein_ness, liveout_ness;
	    in >> reg;
	    in >> L_BRACKET;
	    in >> livein_ness;
	    in >> liveout_ness;
	    in >> R_BRACKET;
	    in >> R_CURLY;
	    attr->map->bind(reg, Pair<int,int>(livein_ness, liveout_ness));
	}
	if (token != R_PAREN)
	    in.syntax_error(token);
	return true;
    } else {
	in.unget_token();
	return false;
    }
}

///////////////////////////////////////////////////////////////////////////
// Loop_count_attrib

IR_outstream &operator<<(IR_outstream &out, Loop_count_attrib &attr)
{
  // Print out a loop count attribute;
  out << LOOP_COUNT_STRING << SPACE << L_PAREN << attr.loopcount << R_PAREN;
  return out;
}

Loop_count_attrib* scan_loop_count(IR_instream &in)
{
  IR_token token;
  in >> token;
  if (token == LOOP_COUNT_STRING) {
    int value;
    in >> L_PAREN >> value >> R_PAREN;
    Loop_count_attrib *attr = new Loop_count_attrib();
    attr->loopcount = value;
    return attr;
  } else {
    in.unget_token();
    return NULL;
  }
}


///////////////////////////////////////////////////////////////////////////
// Merge_op_BB_id

IR_outstream &operator<<(IR_outstream &out, Merge_op_BB_id &attr)
{
  // Print out a Merge_op_BB_id attribute;
  out << BB_ID_ATTR_STRING << L_PAREN << attr.id << R_PAREN;
  return out;
}

Merge_op_BB_id* scan_merge_op_bb_id(IR_instream &in)
{
    // Remove and assign a Merge_op_BB_id attribute from the input;
    // Returns false if no id is in the input stream;

    IR_token token;
    in >> token;
    if (token == BB_ID_ATTR_STRING) {
	int value;
	in >> L_PAREN >> value >> R_PAREN;
	Merge_op_BB_id *attr = new Merge_op_BB_id();
	attr->id = value;
	return attr;
    } else {
	in.unget_token();
	return NULL;
    }
} 

///////////////////////////////////////////////////////////////////////////
// Reference_dtype_attr

IR_outstream &operator<<(IR_outstream &out, Reference_dtype_attr *ptr)
{
  // Print out a pointer to this attribute;
  out << REFERENCE_DTYPE_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Reference_dtype_attr &attr)
{
  int i;

  Reference_dtype_attr* ptr = &attr;
  //Format:  ptr (d[DType:...:DType] op[Dtype] s[DType:...:DType])
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN << REFERENCE_DTYPE_DEST_STRING << L_BRACKET;
    for( i=1; i<=attr.num_dest(); i++ )
      {
	out << attr.dest((Port_num)i);
	// if this is not the last one, print a seperator
	if ( i!=(attr.num_dest()) ) out << COLON;
      }
    out << R_BRACKET;

    out << SPACE << REFERENCE_DTYPE_SRC_STRING << L_BRACKET;
    for( i=1; i<=attr.num_src(); i++ )
      {
	out << attr.src((Port_num)i);
	// if this is not the last one, print a seperator
	if ( i!=(attr.num_src()) ) out << COLON;
      }
    out << R_BRACKET << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
}

Reference_dtype_attr* scan_reference_dtype_attr_ptr(IR_instream& in)
{
    IR_token token;
    void *object_ptr;
    Reference_dtype_attr *info;

    in >> token;
    if (token == REFERENCE_DTYPE_ATTR_STRING) {
	in >> object_ptr;
	if (object_ptr == NULL) {
            info = new Reference_dtype_attr();
            in.map_pointer(info);
        }
	else {
            info = (Reference_dtype_attr*) object_ptr;
        }
	return info;
    }
    else {
	in.unget_token();
	return NULL;
    }
}

bool scan_reference_dtype_attr_dictionary(IR_instream& in)
{
    // Extracts an reference_dtype attribute, if any, from the input stream 
    // and fills in its information.  Returns true if an attribute is found;

    IR_token token;
    in >> token;
    if (token == REFERENCE_DTYPE_ATTR_STRING) {
	void *ptr;
	in >> ptr;
	if (ptr == NULL) {
	    cerr << "orphaned attribute" << endl;
	    exit(1);
	}
	Reference_dtype_attr *info = (Reference_dtype_attr*) ptr;

	info->resize_src(MAX_SRC_PORTS);
	info->resize_dest(MAX_DEST_PORTS);
	int numsrc=1,numdest=1;

	in >> L_PAREN;

	//read the dests
	in >> REFERENCE_DTYPE_DEST_STRING;
	in >> L_BRACKET;
	if ( in.peek() != R_BRACKET ) {
	    do {
	       // the maxes aren't hard bounds, so increase if necessary
	       // dcronqui, 1/2/01
	        if (numdest > MAX_DEST_PORTS) {
		   info->resize_dest(numdest);
	        }
		in >> info->dest((Port_num)numdest);
		numdest++;
		in >> token;
		if ( token != R_BRACKET && token != COLON )
			in.syntax_error(token);
	    } while( token != R_BRACKET );
	} else {
 	    in >> token;
	}
	assert( token == R_BRACKET );

	//read the srcs
	in >> REFERENCE_DTYPE_SRC_STRING;
	in >> L_BRACKET;
	if ( in.peek() != R_BRACKET ) {
	    do {
	       // the maxes aren't hard bounds, so increase if necessary
	       // dcronqui, 1/2/01
	        if (numsrc > MAX_SRC_PORTS) {
		   info->resize_src(numsrc);
	        }
		in >> info->src((Port_num)numsrc);
		numsrc++;
		in >> token;
		if ( token != R_BRACKET && token != COLON )
			in.syntax_error(token);
	    } while( token != R_BRACKET );
	} else {
 	    in >> token;
	}
	assert( token == R_BRACKET );

	in >> R_PAREN;

	//resize the vectors to be the proper size, numdest/src 1 larger
	//than they need to be when reach this point!
	info->resize_dest(numdest-1);
	info->resize_src(numsrc-1);
	
	return true;

    } else {
	in.unget_token();
	return false;
    }
}

///////////////////////////////////////////////////////////////////////////
// Physfile_binding_attr

IR_outstream &operator<<(IR_outstream &out, Physfile_binding_attr *ptr)
{
  // Print out a pointer to this attribute;
  out << PHYSFILE_BIND_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Physfile_binding_attr &attr)
{
  int i;
  Physfile_binding_attr* ptr = &attr;
  // Format:  ptr (d[pn:prf1 prf2, ... , pn: prf1 prf2] s[pn:prf1 prf2, ,... , pn:prf1 prf2])
  // Example: opc ^1792 (d[4:rr 1 -1, 1:ra -1 17] s[1:aa 3 4])

  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;

    int num_srcs = attr.num_src();
    int num_dests = attr.num_dest();
	
    // if both are zero then you shouldn't have an attribute at all.
    // assert (num_srcs != 0 || num_dests != 0);

    Hash_set<eString> dummy(hash_estring);
    Pair <Port_num, Hash_set<eString> > pn_pr((Port_num)0, dummy);

    out << PHYSFILE_BIND_DEST_STRING << L_BRACKET;
    if (num_dests > 0) {
      // dests
      
      i=1;
      attr.iterate_dests();
      while (attr.get_next_ref(pn_pr)) {
	out << pn_pr.first << COLON << pn_pr.second;
	if (i++ != num_dests) out << COMMA;
      }
    }
    out << R_BRACKET;

    out << SPACE << PHYSFILE_BIND_SRC_STRING << L_BRACKET;
    if (num_srcs > 0) { 
      // srcs
      i=1;
      attr.iterate_srcs();
      while (attr.get_next_ref(pn_pr)) {
	out << pn_pr.first << COLON << pn_pr.second;
	if (i++ != num_srcs) out << COMMA;
      }
    }
    out << R_BRACKET;
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
}

Physfile_binding_attr* scan_prf_bind_attr_ptr(IR_instream& in)
{
    IR_token token;
    void *object_ptr;
    Physfile_binding_attr *info;

    in >> token;
    if (token == PHYSFILE_BIND_ATTR_STRING) {
	in >> object_ptr;
	if (object_ptr == NULL) {
            info = new Physfile_binding_attr();
            in.map_pointer(info);
        }
	else {
            info = (Physfile_binding_attr*) object_ptr;
        }
	return info;
    }
    else {
	in.unget_token();
	return NULL;
    }
}


bool scan_prf_bind_attr_dictionary(IR_instream& in)
{
    // Extracts an spatial_ref attribute, if any, from the input stream 
    // and fills in its information.  Returns true if an attribute is found;

    IR_token token;
    in >> token;
    if (token == PHYSFILE_BIND_ATTR_STRING) {
	void *ptr;
	in >> ptr;
	if (ptr == NULL) {
	    cerr << "orphaned attribute" << endl;
	    exit(1);
	}
	Physfile_binding_attr *info = (Physfile_binding_attr*) ptr;

	Port_num pn;
        Hash_set<eString> pr(hash_estring, 16);
	bool process_src = false;
	
	in >> L_PAREN;

	in >> token;

	do { 
	   // must be at least one
	   if (token == PHYSFILE_BIND_SRC_STRING) {
	      process_src = true; 
	   }
	   else if (token == PHYSFILE_BIND_DEST_STRING) {
	      process_src = false;
	   }
	   else {
	      in.syntax_error(token);
	   }
	   
	   in >> L_BRACKET;
	   // must be at least one
           if (in.peek() != R_BRACKET) {
	     do {
	       in >> pn;
	       in >> COLON;
	       in >> pr;

	       if (process_src) {
		 info->set_src(pn, pr);
	       }
	       else {
		 info->set_dest(pn, pr);
	       }
	       in >> token;
	       if ( token != R_BRACKET && token != COMMA)
		 in.syntax_error(token);
	     } while (token != R_BRACKET );
           } else
             in >> token;

	   in >> token;
	} while (token == PHYSFILE_BIND_DEST_STRING || token == PHYSFILE_BIND_SRC_STRING);

	if (token != R_PAREN) {
	   in.syntax_error(token);
	}

	return true;

    } else {
	in.unget_token();
	return false;
    }
}

///////////////////////////////////////////////////////////////////////////
// Spatial_ref_attr

IR_outstream &operator<<(IR_outstream &out, Spatial_ref_attr *ptr)
{
  // Print out a pointer to this attribute;
  out << SPATIAL_REF_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Spatial_ref_attr &attr)
{
  int i;
  Spatial_ref_attr* ptr = &attr;
  // Format:  ptr (d[pn:Spatial_ref,...,pn:Spatial_ref] s[pn:Spatial_ref,...,pn:Spatial_ref])
  // Example: opc ^1792 (d[4:rr 1 -1, 1:ra -1 17] s[1:aa 3 4])

  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;

    int num_srcs = attr.num_src();
    int num_dests = attr.num_dest();
	
    // if both are zero then you shouldn't have an attribute at all.
    assert (num_srcs != 0 || num_dests != 0);

    Pair <Port_num, Spatial_ref> pn_sr;

    if (num_dests > 0) {
      out << SPATIAL_REF_DEST_STRING << L_BRACKET;

      // dests
      i=1;
      attr.iterate_dests();
      while (attr.get_next_ref(pn_sr)) {
	out << pn_sr.first << COLON << pn_sr.second;
	if (i++ != num_dests) out << COMMA;
      }
	   
      out << R_BRACKET;
    }

    if (num_srcs > 0) { 
      out << SPACE << SPATIAL_REF_SRC_STRING << L_BRACKET;

      // srcs
      i=1;
      attr.iterate_srcs();
      while (attr.get_next_ref(pn_sr)) {
	out << pn_sr.first << COLON << pn_sr.second;
	if (i++ != num_srcs) out << COMMA;
      }
      out << R_BRACKET;
    }
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
}

Spatial_ref_attr* scan_spatial_ref_attr_ptr(IR_instream& in)
{
    IR_token token;
    void *object_ptr;
    Spatial_ref_attr *info;

    in >> token;
    if (token == SPATIAL_REF_ATTR_STRING) {
	in >> object_ptr;
	if (object_ptr == NULL) {
            info = new Spatial_ref_attr();
            in.map_pointer(info);
        }
	else {
            info = (Spatial_ref_attr*) object_ptr;
        }
	return info;
    }
    else {
	in.unget_token();
	return NULL;
    }
}


bool scan_spatial_ref_attr_dictionary(IR_instream& in)
{
    // Extracts an spatial_ref attribute, if any, from the input stream 
    // and fills in its information.  Returns true if an attribute is found;

    IR_token token;
    in >> token;
    if (token == SPATIAL_REF_ATTR_STRING) {
	void *ptr;
	in >> ptr;
	if (ptr == NULL) {
	    cerr << "orphaned attribute" << endl;
	    exit(1);
	}
	Spatial_ref_attr *info = (Spatial_ref_attr*) ptr;

	Port_num pn;
	Spatial_ref sr;
	bool process_src = false;
	
	in >> L_PAREN;

	in >> token;

	do { 
	   // must be at least one
	   if (token == SPATIAL_REF_SRC_STRING) {
	      process_src = true; 
	   }
	   else if (token == SPATIAL_REF_DEST_STRING) {
	      process_src = false;
	   }
	   else {
	      in.syntax_error(token);
	   }
	   
	   in >> L_BRACKET;
	   // must be at least one
	   do {
	      in >> pn;
	      in >> COLON;
	      in >> sr;

	      if (process_src) {
		 info->set_src(pn, sr);
	      }
	      else {
		 info->set_dest(pn, sr);
	      }
	      in >> token;
	      if ( token != R_BRACKET && token != COMMA)
		 in.syntax_error(token);
	   } while (token != R_BRACKET );

	   in >> token;
	} while (token == SPATIAL_REF_DEST_STRING || token == SPATIAL_REF_SRC_STRING);

	if (token != R_PAREN) {
	   in.syntax_error(token);
	}

	return true;

    } else {
	in.unget_token();
	return false;
    }
}

///////////////////////////////////////////////////////////////////////////
// Jump_table_name

IR_outstream &operator<<(IR_outstream &out, Jump_table_name &attr)
{
  // Print out a Jump_table_name attribute;
  out << JUMPTBL_NAME_ATTR_STRING << L_PAREN << attr.tbl_name << R_PAREN;
  return out;
}

Jump_table_name* scan_jump_table_name(IR_instream &in)
{
    // Remove and return a Jump_table_name attribute from the input;
    // Returns NULL if no id is in the input stream;

    IR_token token;
    in >> token;
    if (token == JUMPTBL_NAME_ATTR_STRING) {
	char *value;
	eString str_value;
	in >> L_PAREN >> value >> R_PAREN;
	str_value = value;
	Jump_table_name *attr = new Jump_table_name(str_value);
	return attr; 
    } else {
	in.unget_token();
	return NULL;
    }
} 

///////////////////////////////////////////////////////////////////////////
// Op_chaining_attr

/*
 * The function associates a map_ptr to print the Op_chaining_attr information
 * The map_ptr is used for mapping between Op_chaining_attr pointer present
 * with the op to actual information which will be stored later.
 */
IR_outstream &operator<<(IR_outstream &out, Op_chaining_attr *ptr)
{
  out << OP_CHAINING_ATTR_STRING << (void*)ptr;
  return out;
}

/*
 * This function outputs the op chain attribute information of an operation.
 * The syntax of the output is opc ^[unique-id] ({op1 op2}{op3 op4})
 * The function will be called by the print_attributes(op) function 
 * in ir_op.cpp,which is called by print_attributes(reg) function
 */
IR_outstream &operator<<(IR_outstream &out, Op_chaining_attr &attr)
{
  Op_chaining_attr* ptr = &attr;
  //Format:  opc(op_id) [{op1 op2} {op3 op4} ...]

  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;

    const List< List<Op*> > &op_chain_list = attr.get_op_chain_list();
    List_iterator < List<Op*> > iter(op_chain_list);

    for (; iter!=0; iter++) {
      List<Op*>  &op_chain = (*iter);
      out << L_CURLY;
      List_iterator<Op*> op_iter (op_chain);
      for (; op_iter!=0; op_iter++) {
	out << (*op_iter)->id() << SPACE;
      }
      out << R_CURLY;
    }
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
}

/*
 * This function prints the pointer value of the Op_chain_attr_ptr 
 * of `op'. The pointer value will be used later to read the 
 * complete information and update the Op_chaining_attr.
 */
Op_chaining_attr* scan_op_chaining_attr_ptr(IR_instream& in)
{
    IR_token token;
    void *object_ptr;
    Op_chaining_attr *info;

    in >> token;
    if (token == OP_CHAINING_ATTR_STRING) {
	in >> object_ptr;
	if (object_ptr == NULL) {
            info = new Op_chaining_attr();
            in.map_pointer(info);
        }
	else {
            info = (Op_chaining_attr*) object_ptr;
        }
	return info;
    }
    else {
	in.unget_token();
	return NULL;
    }
}

/*
 * The function will read the operator chaining information stored in the file
 * and updates the information of operations in the region accordingly.
 *
 * The syntax of the input is opc [unique-id] ( {op1,op2} {op3,op4} ). The
 * Op_chain_attr will be constructed by the function op_chaining_attr_ptr while
 * reading the attributes of the `op'. The contents of this list will be updated
 * here.
 *
 */
bool scan_op_chaining_attr_dictionary(IR_instream& in)
{
    IR_token token;
    in >> token;
    if (token == OP_CHAINING_ATTR_STRING) {
	void *ptr;
	in >> ptr;
	if (ptr == NULL) {
	    cerr << "orphaned attribute" << endl;
	    exit(1);
	}
	Op_chaining_attr *info = (Op_chaining_attr*) ptr;

        // get the id some how
        List<List <Op*> > &op_chain_list = info->get_op_chain_list();
        in >> L_PAREN;
        in >> token;
        while (token != R_PAREN) {

           if (token !=  L_CURLY) {
             in.syntax_error(token);
           }

           List<Op *> op_list;
           in >> token;
           while (token != R_CURLY) {
               int op_id = (int) token;
               if (op_table[op_id] != 0 ) {
                 Op *op = (Op*) op_table[op_id];
                 op_list.add_tail(op);
               }
               else {
                 cerr << "Op (" << op_id <<") specified does not exist" << endl;
               }
               in >> token;
           } 
           if (!op_list.is_empty()) {
             op_chain_list.add_tail(op_list);
           }
           in >> token;
        }
	return true;

    } else {
	in.unget_token();
	return false;
    }
}

///////////////////////////////////////////////////////////////////////////
// Nametag_attr

IR_outstream &operator<<(IR_outstream &out, const Nametag_attr &attr)
{
  // Print out a Nametag_attr;
  out << NAMETAG_ATTR_STRING << SPACE << L_PAREN;
  out << attr.name << SPACE;
  switch (attr.type) {
  case SYSTOLIC_INIT:
    out << eString("init");
    break;
  case SYSTOLIC_BODY:
    out << eString("body");
    break;
  case SYSTOLIC_FINAL:
    out << eString("final");
    break;
  default:
    cerr << "Unknown nametag type" << endl;
    exit(0);
  }
  out << R_PAREN;
  return out;
}

Nametag_attr* scan_nametag_attr(IR_instream& in)
{
  IR_token token;

  in >> token;
  if (token == NAMETAG_ATTR_STRING) {
    in >> L_PAREN;

    eString name;
    eString type_str;
    Nametag_type type = SYSTOLIC_INIT;

    in >> token;
    name = token;

    in >> token;
    type_str = token;

    if      (type_str == "init")  type = SYSTOLIC_INIT;
    else if (type_str == "body")  type = SYSTOLIC_BODY;
    else if (type_str == "final") type = SYSTOLIC_FINAL;
    else                          in.syntax_error(token);

    in >> token;
    if (token != R_PAREN)
      in.syntax_error(token);

    Nametag_attr* info = new Nametag_attr();
    info->name  = name;
    info->type = type;
    return info;
  } else {
    in.unget_token();
    return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////
// Liveness_info_attrib

IR_outstream &operator<<(IR_outstream &out, Liveness_info_attrib *ptr)
{
  // Print out a pointer to this attribute;
  out << LIVENESS_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Liveness_info_attrib &attr)
{
  // Print out a Liveness_info attribute;
  Liveness_info_attrib* ptr = &attr;
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;
    Liveness_info_iterator optr;
    for (optr(*(attr.info));  optr != 0;  optr++)
      out << *optr;
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
} 

Liveness_info_attrib* scan_liveness_info_ptr(IR_instream& in)
{
  IR_token token;
  void *object_ptr;
  Liveness_info_attrib *attr;

  in >> token;
  if (token == LIVENESS_ATTR_STRING) {
    in >> object_ptr;
    if (object_ptr==NULL) {
      attr = new Liveness_info_attrib();
      in.map_pointer(attr);
    }
    else {
      attr = (Liveness_info_attrib *) object_ptr;
    }
    return attr;
  }
  else {
    in.unget_token();
    return NULL;
  }
}


bool scan_liveness_info_dictionary(IR_instream& in)
{
  // Extracts an live attribute, if any, from the input stream and fills in
  // its information.  Returns true if an attribute is found;

  IR_token token;
  Operand *opnd;
  void *ptr;

  in >> token;
  if (token == LIVENESS_ATTR_STRING) {
    in >> ptr;
    if (ptr == NULL) {
      cerr << "orphaned attribute" << endl;
      exit(1);
    }
    Liveness_info_attrib *attr = (Liveness_info_attrib*) ptr;
    attr->info = new Liveness_info();
    in >> L_PAREN;
    while ((opnd = read_operand(in))) {
      attr->info->add_tail(*opnd);
    }
    in >> R_PAREN;
    return true;
  } else {
    in.unget_token();
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////
// Downward_use_attrib

IR_outstream &operator<<(IR_outstream &out, Downward_use_attrib *ptr)
{
  // Print out a pointer to this attribute;
  out << DOWN_USE_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Downward_use_attrib &attr)
{
  // Print out a Downward_use attribute;
  Downward_use_attrib* ptr = &attr;
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;
    Liveness_info_iterator optr;
    for (optr(*(attr.info));  optr != 0;  optr++)
      out << *optr;
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
} 

Downward_use_attrib* scan_downward_use_ptr(IR_instream& in)
{
  IR_token token;
  void *object_ptr;
  Downward_use_attrib *attr;

  in >> token;
  if (token == DOWN_USE_ATTR_STRING) {
    in >> object_ptr;
    if (object_ptr==NULL) {
      attr = new Downward_use_attrib();
      in.map_pointer(attr);
    }
    else {
      attr = (Downward_use_attrib *) object_ptr;
    }
    return attr;
  }
  else {
    in.unget_token();
    return NULL;
  }
}


bool scan_downward_use_dictionary(IR_instream& in)
{
  // Extracts an live attribute, if any, from the input stream and fills in
  // its information.  Returns true if an attribute is found;

  IR_token token;
  Operand *opnd;
  void *ptr;

  in >> token;
  if (token == DOWN_USE_ATTR_STRING) {
    in >> ptr;
    if (ptr == NULL) {
      cerr << "orphaned attribute" << endl;
      exit(1);
    }
    Downward_use_attrib *attr = (Downward_use_attrib*) ptr;
    attr->info = new Liveness_info();
    in >> L_PAREN;
    while ((opnd = read_operand(in))) {
      attr->info->add_tail(*opnd);
    }
    in >> R_PAREN;
    return true;
  } else {
    in.unget_token();
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////
// Upward_def_attrib

IR_outstream &operator<<(IR_outstream &out, Upward_def_attrib *ptr)
{
  // Print out a pointer to this attribute;
  out << UP_DEF_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Upward_def_attrib &attr)
{
  // Print out a Upward_def attribute;
  Upward_def_attrib* ptr = &attr;
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;
    Liveness_info_iterator optr;
    for (optr(*(attr.info));  optr != 0;  optr++)
      out << *optr;
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
} 

Upward_def_attrib* scan_upward_def_ptr(IR_instream& in)
{
  IR_token token;
  void *object_ptr;
  Upward_def_attrib *attr;

  in >> token;
  if (token == UP_DEF_ATTR_STRING) {
    in >> object_ptr;
    if (object_ptr==NULL) {
      attr = new Upward_def_attrib();
      in.map_pointer(attr);
    }
    else {
      attr = (Upward_def_attrib *) object_ptr;
    }
    return attr;
  }
  else {
    in.unget_token();
    return NULL;
  }
}


bool scan_upward_def_dictionary(IR_instream& in)
{
  // Extracts an live attribute, if any, from the input stream and fills in
  // its information.  Returns true if an attribute is found;

  IR_token token;
  Operand *opnd;
  void *ptr;

  in >> token;
  if (token == UP_DEF_ATTR_STRING) {
    in >> ptr;
    if (ptr == NULL) {
      cerr << "orphaned attribute" << endl;
      exit(1);
    }
    Upward_def_attrib *attr = (Upward_def_attrib*) ptr;
    attr->info = new Liveness_info();
    in >> L_PAREN;
    while ((opnd = read_operand(in))) {
      attr->info->add_tail(*opnd);
    }
    in >> R_PAREN;
    return true;
  } else {
    in.unget_token();
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////
// Downward_def_attrib

IR_outstream &operator<<(IR_outstream &out, Downward_def_attrib *ptr)
{
  // Print out a pointer to this attribute;
  out << DOWN_DEF_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Downward_def_attrib &attr)
{
  // Print out a Downward_def attribute;
  Downward_def_attrib* ptr = &attr;
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN;
    Liveness_info_iterator optr;
    for (optr(*(attr.info));  optr != 0;  optr++)
      out << *optr;
    out << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
} 

Downward_def_attrib* scan_downward_def_ptr(IR_instream& in)
{
  IR_token token;
  void *object_ptr;
  Downward_def_attrib *attr;

  in >> token;
  if (token == DOWN_DEF_ATTR_STRING) {
    in >> object_ptr;
    if (object_ptr==NULL) {
      attr = new Downward_def_attrib();
      in.map_pointer(attr);
    }
    else {
      attr = (Downward_def_attrib *) object_ptr;
    }
    return attr;
  }
  else {
    in.unget_token();
    return NULL;
  }
}


bool scan_downward_def_dictionary(IR_instream& in)
{
  // Extracts an live attribute, if any, from the input stream and fills in
  // its information.  Returns true if an attribute is found;

  IR_token token;
  Operand *opnd;
  void *ptr;

  in >> token;
  if (token == DOWN_DEF_ATTR_STRING) {
    in >> ptr;
    if (ptr == NULL) {
      cerr << "orphaned attribute" << endl;
      exit(1);
    }
    Downward_def_attrib *attr = (Downward_def_attrib*) ptr;
    attr->info = new Liveness_info();
    in >> L_PAREN;
    while ((opnd = read_operand(in))) {
      attr->info->add_tail(*opnd);
    }
    in >> R_PAREN;
    return true;
  } else {
    in.unget_token();
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////
// Control_flow_freq

IR_outstream &operator<<(IR_outstream &out, Control_flow_freq &attr)
{
  // Print out a Control_flow_freq attribute;
  out << CFREQ_ATTR_STRING << L_PAREN << attr.freq << attr.cc << R_PAREN;
  return out;
}

Control_flow_freq* scan_control_flow_freq(IR_instream &in)
{
  // Extracts and returns a Control_flow_freq from the input stream;

  IR_token token;
  in >> token;
  if (token == CFREQ_ATTR_STRING) {
    int cc;
    double freq;
    in >> L_PAREN >> freq >> token;
    if (token == R_PAREN) {
      cc = EL_UNKNOWN_CC_VAL;
    }
    else {
      cc = (int) token;
      in >> R_PAREN;
    }
    Control_flow_freq *ctrl_freq = new Control_flow_freq;
    ctrl_freq->freq = freq;
    ctrl_freq->cc = cc;
    return ctrl_freq;
  } else {
    in.unget_token();
    return NULL;
  }
}

///////////////////////////////////////////////////////////////////////////
// Exsym_table_attrib

IR_outstream &operator<<(IR_outstream &out, Exsym_table_attrib *ptr)
{
  // Print out a pointer to this attribute;
  out << EXSYM_TABLE_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Exsym_table_attrib &attr)
{
  // Print out a Exsym_table attribute;
  // format: ptr ({regnum version[symbol info]} ...)
  Exsym_table_attrib* ptr = &attr;
  if (!out.has_been_printed((void*) ptr)) {
    out << ptr << SPACE << L_PAREN << indent;
    Exsym_table_iterator iter;
    for (iter(*(attr.symtab));  iter != 0;  iter++) {
      int regnum = (*iter).first;
      Exsym_info* syminfo = (*iter).second;
      out << endline << tab << L_CURLY << regnum << SPACE;
      out << (int)syminfo->exsym_version() << L_BRACKET;
      syminfo->print_exsym_info(out);
      out << R_BRACKET << R_CURLY;
    }
    out << outdent << R_PAREN;
    out.mark_printed((void*) ptr);
  }
  return out;
} 

Exsym_table_attrib* scan_exsym_table_ptr(IR_instream& in)
{
  IR_token token;
  void *object_ptr;
  Exsym_table_attrib *attr;

  in >> token;
  if (token == EXSYM_TABLE_STRING) {
    in >> object_ptr;
    if (object_ptr==NULL) {
      attr = new Exsym_table_attrib();
      in.map_pointer(attr);
    }
    else {
      attr = (Exsym_table_attrib *) object_ptr;
    }
    return attr;
  }
  else {
    in.unget_token();
    return NULL;
  }
}


bool scan_exsym_table_dictionary(IR_instream& in)
{
  // Extracts an live attribute, if any, from the input stream and fills in
  // its information.  Returns true if an attribute is found;

  IR_token token;
  void *ptr;

  in >> token;
  if (token == EXSYM_TABLE_STRING) {
    // format: ptr ({regnum version[symbol info]} ...)
    in >> ptr;
    if (ptr == NULL) {
      cerr << "orphaned attribute" << endl;
      exit(1);
    }
    Exsym_table_attrib *attr = (Exsym_table_attrib*) ptr;
    attr->symtab = new Exsym_table(hash_int, 37);
    in >> L_PAREN;
    in >> token;
    while (token == L_CURLY) {
      int regnum, version;
      Exsym_info* syminfo=NULL;
      in >> regnum >> version >> L_BRACKET;
      syminfo = Exsym_info::exsym_info_allocator((Exsym_type)version);
      syminfo->scan_exsym_info(in);
      in >> R_BRACKET >> R_CURLY;
      attr->symtab->bind(regnum, syminfo);
      in >> token;
    }
    if (token != R_PAREN)
      in.syntax_error(token);
    return true;
  } else {
    in.unget_token();
    return false;
  }
}

// Exsym info read/write methods
void Exsym_info::scan_exsym_info(IR_instream& in) {
  IR_token token;
  int mtype;
  // format: var_name dtype mem_type

  in >> token; 
  var_name = token;
  in >> dt;
  in >> mtype; 
  type = (MEMORY_TYPE)mtype;
}

void Exsym_info::print_exsym_info(IR_outstream& out) {
  out << var_name << SPACE << dt << SPACE << (int)type;
}

// ASN definitions
#define NEVER 0
#define ALWAYS 1
#define SOMETIMES 2

void Suif_exsym_info::scan_exsym_info(IR_instream& in) {
  IR_token token;
  int asn_count;
  // format: var_name dtype mem_type liveinbase liveoutbase asn_count [ASN ...]

  // first read the basic exsym stuff
  Exsym_info::scan_exsym_info(in);
  // read in the livein/liveout base
  in >> c_livein_base >> c_liveout_base;
  // check for ASN info
  in >> asn_count;
  if (asn_count==0) {
    has_asn = false; 
    return;
  }
  // read in ASN info
  has_asn = true;
  asn.resize(asn_count);
  in >> L_BRACKET;
  for (int i=0; i<asn_count; i++) {
    in >> token;
    char* str = (char*)token;
    switch (*str) {
      case 'N': asn[i] = NEVER; break;
      case 'S': asn[i] = SOMETIMES; break;
      case 'A': asn[i] = ALWAYS; break;
      default: in.syntax_error(token);
    }
  }
  in >> R_BRACKET;
}

void Suif_exsym_info::print_exsym_info(IR_outstream& out) {
  // first print the basic exsym stuff
  Exsym_info::print_exsym_info(out);
  // print livein/liveout base and asn count
  assert(has_asn || asn.size()==0);
  out << SPACE << c_livein_base << SPACE << c_liveout_base << SPACE << asn.size();
  // print ASN info, if any
  if (has_asn) {
    out << L_BRACKET;
    for (unsigned i=0; i<asn.size(); i++) {
      switch (asn[i]) {
      case NEVER: out << 'N' << SPACE; break;
      case SOMETIMES: out << 'S' << SPACE; break;
      case ALWAYS: out << 'A' << SPACE; break;
        default: {
	  cerr << "invalid ASN attribute " << asn[i] 
	       << ", only 0,1,2 allowed." << endl;
	  exit(1);
	}
      }
    }
    out << R_BRACKET;
  }
}

///////////////////////////////////////////////////////////////////////////
// Cluster_id_attr

IR_outstream &operator<<(IR_outstream &out, Cluster_id_attr &attr)
{
  // Print out a Cluster_id_attr attribute;
  out << CLUSTER_ID_ATTR_STRING << L_PAREN << attr.cluster_id << R_PAREN;
  return out;
}

Cluster_id_attr* scan_cluster_id_attr(IR_instream &in)
{
    // Remove and assign a Cluster_id_attr attribute from the input;
    // Returns false if no id is in the input stream;

    IR_token token;
    in >> token;
    if (token == CLUSTER_ID_ATTR_STRING) {
	int value;
	in >> L_PAREN >> value >> R_PAREN;
	Cluster_id_attr *attr = new Cluster_id_attr();
	attr->cluster_id = value;
	return attr;
    } else {
	in.unget_token();
	return NULL;
    }
} 

///////////////////////////////////////////////////////////////////////////
// Usched_time_attr

IR_outstream &operator<<(IR_outstream &out, Usched_time_attr &attr)
{
  // Print out a Usched_time_attr attribute;
  out << USCHED_TIME_ATTR_STRING << L_PAREN << attr.usched_time << R_PAREN;
  return out;
}

Usched_time_attr* scan_usched_time_attr(IR_instream &in)
{
    // Remove and assign a Usched_time_attr attribute from the input;
    // Returns false if no id is in the input stream;

    IR_token token;
    in >> token;
    if (token == USCHED_TIME_ATTR_STRING) {
	int value;
	in >> L_PAREN >> value >> R_PAREN;
	Usched_time_attr *attr = new Usched_time_attr();
	attr->usched_time = value;
	return attr;
    } else {
	in.unget_token();
	return NULL;
    }
} 

///////////////////////////////////////////////////////////////////////////
// Msched_attr

IR_outstream &operator<<(IR_outstream &out, Msched_attr *ptr)
{
  out << MSCHED_ATTR_STRING << (void*)ptr;
  return out;
}

IR_outstream &operator<<(IR_outstream &out, Msched_attr &attr)
{
  // format: msched ^123 (ii esc lg[opd opd] lk[opd opd] deug[] deuk[] uedg[] uedk[] dedg[] dedk[])

  Msched_attr* ptr = &attr;
  if (!out.has_been_printed((void*)ptr)) {
    out << ptr << SPACE << L_PAREN << attr.ii << attr.esc;
    out << "lg" << L_BRACKET;
    for (List_iterator<Operand> li(attr.liveness_gen); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << "lk" << L_BRACKET;
    for (List_iterator<Operand> li(attr.liveness_kill); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << "deug" << L_BRACKET;
    for (List_iterator<Operand> li(attr.down_exposed_use_gen); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << "deuk" << L_BRACKET;
    for (List_iterator<Operand> li(attr.down_exposed_use_kill); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << "uedg" << L_BRACKET;
    for (List_iterator<Operand> li(attr.up_exposed_def_gen); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << "uedk" << L_BRACKET;
    for (List_iterator<Operand> li(attr.up_exposed_def_kill); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << "dedg" << L_BRACKET;
    for (List_iterator<Operand> li(attr.down_exposed_def_gen); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << "dedk" << L_BRACKET;
    for (List_iterator<Operand> li(attr.down_exposed_def_kill); li!=0; li++) {
      out << *li;
    }
    out << R_BRACKET << R_PAREN;
    out.mark_printed((void*)ptr);
  }
  return out;
}

Msched_attr* scan_msched_attr_ptr(IR_instream &in)
{
  IR_token token;
  void *object_ptr;
  Msched_attr *info;

  in >> token;
  if (token == MSCHED_ATTR_STRING) {
    in >> object_ptr;
    if (object_ptr == NULL) {
      info = new Msched_attr();
      in.map_pointer(info);
    }
    else {
      info = (Msched_attr*) object_ptr;
    }
    return info;
  } else {
    in.unget_token();
    return NULL;
  }
}

bool scan_msched_attr_dictionary(IR_instream& in)
{
  // Extracts a msched_attr attribute, if any, from the input stream 
  // and fills in its information.  Returns true if an attribute is found.
  IR_token token;
  in >> token;
  if (token == MSCHED_ATTR_STRING) {
    void *ptr;
    in >> ptr;
    if (ptr == NULL) {
      cerr << "orphaned attribute" << endl;
      exit(1);
    }
    Msched_attr *info = (Msched_attr*) ptr;

    in >> L_PAREN;
    in >> info->ii;
    in >> info->esc;
    in >> "lg" >> L_BRACKET;
    Operand* opnd;
    while ((opnd = read_operand(in)))
      info->liveness_gen.add_tail(*opnd);

    in >> R_BRACKET >> "lk" >> L_BRACKET;
    while ((opnd = read_operand(in)))
      info->liveness_kill.add_tail(*opnd);

    in >> R_BRACKET >> "deug" >> L_BRACKET;
    while ((opnd = read_operand(in)))
      info->down_exposed_use_gen.add_tail(*opnd);

    in >> R_BRACKET >> "deuk" >> L_BRACKET;
    while ((opnd = read_operand(in)))
      info->down_exposed_use_kill.add_tail(*opnd);

    in >> R_BRACKET >> "uedg" >> L_BRACKET;
    while ((opnd = read_operand(in)))
      info->up_exposed_def_gen.add_tail(*opnd);

    in >> R_BRACKET >> "uedk" >> L_BRACKET;
    while ((opnd = read_operand(in)))
      info->up_exposed_def_kill.add_tail(*opnd);

    in >> R_BRACKET >> "dedg" >> L_BRACKET;
    while ((opnd = read_operand(in)))
      info->down_exposed_def_gen.add_tail(*opnd);

    in >> R_BRACKET >> "dedk" >> L_BRACKET;
    while ((opnd = read_operand(in)))
      info->down_exposed_def_kill.add_tail(*opnd);
    in >> R_BRACKET >> R_PAREN;

    return true;
  } else {
    in.unget_token();
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////
// Branch_target_attr

IR_outstream &operator<<(IR_outstream &out, Branch_target_attr *ptr)
{
  // Print out a pointer to this attribute;
  out << BRANCH_TARGET_ATTR_STRING << (void*)ptr;
  return out;
}


IR_outstream &operator<<(IR_outstream &out, Branch_target_attr &attr)
{
  // Print out a Branch_target_attr attribute;
  out << BRANCH_TARGET_ATTR_STRING << L_PAREN << attr.label << R_PAREN;
  return out;
}


Branch_target_attr* scan_branch_target_attr(IR_instream &in)
{
    // Remove and assign a Branch_target_attr attribute from the input;
    // Returns false if no id is in the input stream;

    IR_token token;
    in >> token;
    if (token == BRANCH_TARGET_ATTR_STRING) {
      string value;
      in >> L_PAREN >> value >> R_PAREN;
      Branch_target_attr *attr = new Branch_target_attr();
      attr->label = value;
      return attr;
    } else {
      in.unget_token();
      return NULL;
    }
}

///////////////////////////////////////////////////////////////////////////
// AccSpecAttr

IR_outstream &operator<<(IR_outstream &out, AccSpecAttr *ptr)
{
  // Print out a pointer to this attribute;
  out << ACC_SPEC_ATTR_STRING << (void*)ptr;
  return out;
}


IR_outstream &operator<<(IR_outstream &out, AccSpecAttr &attr)
{
  AccSpec *as;
  out << ACC_SPEC_ATTR_STRING << L_CURLY;
  for (List_iterator<AccSpec *> iter(attr.acc_spec_list); iter != 0; iter++) {
    as = *iter;
    out << L_PAREN
        << (as->isDefine() ? 'D' : 'U') << ' '
        << as->getObjectId() << ' '
        << as->getVersion() << ' '
        << as->getOffset() << ' '
        << as->getSize() 
        << R_PAREN;
  }
  out << R_CURLY;
  return out;
}

AccSpecAttr *scan_accspec_attr(IR_instream &in)
{
    IR_token token;
    in >> token;
    if (token == ACC_SPEC_ATTR_STRING) {
      AccSpecAttr *accspec_attr = new AccSpecAttr();    
      in >> L_CURLY;
      in >> token;
      while (token != R_CURLY) {
        AccSpec *as = new AccSpec();
        int value;
        in >> token;
        as->setIsDef((token == "D") ? true : false);;
        in >> value; as->setObjectId(value);
        in >> value; as->setVersion(value);
        in >> value; as->setOffset(value);
        in >> value; as->setSize(value);
        accspec_attr->acc_spec_list.add_tail(as);
        in >> R_PAREN;
        in >> token;
      }
      return accspec_attr;
    } else {
      in.unget_token();
      return NULL;
    }
}
