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



#include <strstream>
#include <iomanip.h>
#include "data_segment.h"

using namespace std;

// nasty global definitions...
List<Data_segment*> *global_data_segments;
Hash_map<string, string> *global_rename_map;
List<Data_segment*> *segments_to_preinitialize;
Hash_map<string, Struct_or_union_definition*> *global_struct_or_union_definitions;
Struct_or_union_definition *current_struct_or_union_definition = NULL;
int current_offset = 0;

Data_segment_element::Data_segment_element(string element_name, 
					   Data_segment *parent)
{
  this->element_name = element_name;
  this->parent = parent;
  element_size = -1;
  type = DATA_TYPE_NONE;
  num_elements = 0;
  alignment = 2;

  is_global = false;
  is_pointer = false;
  is_inited = false;
  is_array = false;
}

Data_segment_element*
Data_segment::get_element(string name)
{
  for(List_iterator<Data_segment_element *> li(elements); li!=0; li++){
    Data_segment_element *element = *li;
    if (element->element_name == name)
      return element;
  }

  return NULL;
}

void
Data_segment::handle_void_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);

  cerr << "Error: data_segment.cpp: " << element_name << 
    "EL_DATA_TOKEN_VOID not implemented" << endl;
  exit(-1);
}

void
Data_segment::handle_byte_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);

  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  element->type = DATA_TYPE_CHAR;
  element->element_size = 1;
  
  if (eldata->value != NULL) {
    element->is_inited = true;
    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_CHAR, eldata->value, 0));
  }
}

void
Data_segment::handle_alignment_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);


  Data_segment_element *element = get_element(element_name);


  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  element->alignment = eldata->N;
}

void
Data_segment::handle_ascii_token(El_data *eldata)
{
  cerr << "Error: data_segment.cpp EL_DATA_TOKEN_ASCII not implemented" << 
    endl;
  exit(-1);
}

void
Data_segment::handle_asciz_token(El_data *eldata)
{
  cerr << "Error: data_segment.cpp EL_DATA_TOKEN_ASCIZ not implemented" << 
    endl;
  exit(-1);
}

void
Data_segment::handle_reserve_token(El_data *eldata)
{
  // ignore this, the amount of space need is derived from type information
}

void
Data_segment::handle_global_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }
  
  element->is_global = true;
  
  // check to see if there is any reserve information stored in 
  // this global specifier
  if (eldata->src_type) {
    Bitvector bv = 
      ((El_data_source_type_info *)(eldata->src_type))->flags;
    
    if (bv.bit(EL_DATA_ST_CHAR)) {
      element->type = DATA_TYPE_CHAR;
      element->element_size = 1;
    }
    else if (bv.bit(EL_DATA_ST_SHORT)) {
      element->type = DATA_TYPE_SHORT;
      element->element_size = 2;
    }
    else if (bv.bit(EL_DATA_ST_INT)) {
      element->type = DATA_TYPE_INT;
      element->element_size = 4;
    }
    else if (bv.bit(EL_DATA_ST_LONG)) {
      element->type = DATA_TYPE_LONG;
      element->element_size = 4;
    }
    else if (bv.bit(EL_DATA_ST_FLOAT)) {
      element->type = DATA_TYPE_FLOAT;
      element->element_size = 4;
    }
    else if (bv.bit(EL_DATA_ST_DOUBLE)) {
      element->type = DATA_TYPE_DOUBLE;
      element->element_size = 8;
    }
    else if (bv.bit(EL_DATA_ST_VOID)) {
      element->type = DATA_TYPE_VOID;
      element->element_size = 4;
    }
    else if (bv.bit(EL_DATA_ST_STRUCT)) { 
      element->type = DATA_TYPE_STRUCT;
      element->struct_name = (char*)((El_data_source_type_info *)
			      (eldata->src_type))->struct_name;
      
      Pair<string, Struct_or_union_definition*> *pair = global_struct_or_union_definitions->
	find_entry(element->struct_name);
      
      if (!pair) {
	cerr << "Error: data_segment.cpp: couldn't find struct " 
	     << "definition for: " << element->struct_name 
	     << endl;
	exit (-1);
      }
   
      element->element_size = pair->second->size;
//       cerr << "@@ element: " << element->element_name 
//  	   << " struct: " << element->struct_name << " size: " 
//  	   << element->element_size << endl;
    }
    else if (bv.bit(EL_DATA_ST_UNION)) { 
      element->type = DATA_TYPE_UNION;
      element->struct_name = (char*)((El_data_source_type_info *)
			      (eldata->src_type))->struct_name;
      
      Pair<string, Struct_or_union_definition*> *pair = 
	global_struct_or_union_definitions->
	find_entry(element->struct_name);
    
      if (!pair) {
	cerr << "Error: data_segment.cpp: couldn't find struct " 
	     << "definition for: " << element->struct_name 
	     << endl;
	exit (-1);
      }
   
      element->element_size = pair->second->size;
    }
    else {
      cerr << "ERROR: unhandled DATA type in TOKEN_GLOBAL on element: '" 
	   << element_name << "' " << bv << endl;
      exit(-1);
    }

    // check the access declarator list
    for(List_iterator<El_data_access_declarator *> 
	  li(((El_data_source_type_info *)(eldata->src_type))->access_dcltrs);
	li!=0;li++){
      El_data_access_declarator *decl = *li;
      
      switch (decl->method) {
      case EL_DATA_ACCESS_ARRAY:
	if(decl->index) {
	  // for things declared like 'int foo[23];'
	  element->num_elements += 
	    ((El_value_expression*)(decl->index))->value.i;
	  element->is_array = true;
	} else {
	  // for things declared like 'int foo[];'
	  element->is_pointer = true;
	  element->element_size = 4;
	}
	break;
      case EL_DATA_ACCESS_POINTER:
	element->is_pointer = true;
	element->element_size = 4;
	break;
      default:
	cerr << "ERROR: data_segment.cpp: " << element->element_name 
	     << " unhandled access method " << endl;
	exit(-1);
	break;
      }
    }
  }
}


void
Data_segment::handle_wb_token(El_data *eldata)
{
  string element_name;
  int value_offset = 0;

  if (eldata->address) {
    // figure out what type of expression the address is
    if (eldata->address->is_string_expr())
      element_name = (char*)((El_string_expression*)eldata->address)->value;
    else if (eldata->address->is_symbol_expr())
      element_name = (char*)((El_symbol_expression*)eldata->address)->value;
    else if (eldata->address->is_compound_expr()) {
      El_expression *expr = 
	((El_compound_expression*)eldata->address)->args[1];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_symbol_expr())
	element_name = (char*)((El_symbol_expression*)expr)->value;
      else if (expr->is_string_expr()) {
	element_name = (char*)((El_string_expression*)expr)->value;
      }
      else {
	cerr << "Error: data_segment.cpp: could not handle WB token " << endl;
	exit (-1);
      }

      // the value offset is the third element of the compound expr
      expr = ((El_compound_expression*)eldata->address)->args[2];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_int_expr())
	value_offset = ((El_value_expression*)expr)->value.i;
      else {
	cerr << "Error: data_segment.cpp: could not handle WB token " << endl;
	exit (-1);
      }
    }
    else {
      cerr << "Error: data_segment.cpp: unhandled addr expr type in WB token" 
	   << endl;
      exit(-1);
    }
  }
  else {
    cerr << "Error: data_segment.cpp: WB address symbol is null" << endl;
    exit(-1);
  }
  
  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);  
  
  if (eldata->value != NULL) {
    element->is_inited = true;

    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_CHAR, eldata->value, 
					    value_offset));
  }
  else {
    cerr << "Error: data_segment.cpp: val = null" << endl;
    exit(-1);
  }
}


// EL_DATA_TOKEN_WW signifies that a short value is being initialized
void
Data_segment::handle_ww_token(El_data *eldata)
{
  string element_name;
  int value_offset = 0;

  if (eldata->address) {
    // figure out what type of expression the address is
    if (eldata->address->is_string_expr())
      element_name = (char*)((El_string_expression*)eldata->address)->value;
    else if (eldata->address->is_symbol_expr())
      element_name = (char*)((El_symbol_expression*)eldata->address)->value;
    else if (eldata->address->is_compound_expr()) {
      El_expression *expr = 
	((El_compound_expression*)eldata->address)->args[1];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_symbol_expr())
	element_name = (char*)((El_symbol_expression*)expr)->value;
      else if (expr->is_string_expr()) {
	element_name = (char*)((El_string_expression*)expr)->value;
      }
      else {
	cerr << "Error: data_segment.cpp: could not handle WW token " << endl;
	exit (-1);
      }

      // the value offset is the third element of the compound expr
      expr = ((El_compound_expression*)eldata->address)->args[2];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_int_expr())
	value_offset = ((El_value_expression*)expr)->value.i;
      else {
	cerr << "Error: data_segment.cpp: could not handle WB token " << endl;
	exit (-1);
      }

    }
    else {
      cerr << "Error: data_segment.cpp: unhandled addr expr type in WW token" 
	   << endl;
      exit(-1);
    }
  }
  else {
    cerr << "Error: data_segment.cpp: WW address symbol is null" << endl;
    exit(-1);
  }
  
  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);  
  
  if (eldata->value != NULL) {
    element->is_inited = true;

    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_SHORT, eldata->value, 
					    value_offset));
  }
  else {
    cerr << "Error: data_segment.cpp: val = null" << endl;
    exit(-1);
  }
}

void
Data_segment::handle_word_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);

  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  element->type = DATA_TYPE_INT;
  element->element_size = 4;
  
  if (eldata->value != NULL) {
    element->is_inited = true;
    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_WORD, eldata->value, 0));
  }
}

void
Data_segment::handle_long_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);

  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  element->type = DATA_TYPE_LONG;
  element->element_size = 4;
  
  if (eldata->value != NULL) {
    element->is_inited = true;
    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_LONG, eldata->value, 0));
  }
}

void
Data_segment::handle_wi_token(El_data *eldata)
{
  string element_name;
  int value_offset = 0;

  if (eldata->address) {
    if (eldata->address->is_string_expr())
      element_name = (char*)((El_string_expression*)eldata->address)->value;
    else if (eldata->address->is_symbol_expr())
      element_name = (char*)((El_symbol_expression*)eldata->address)->value;
    else if (eldata->address->is_compound_expr()) {
      El_expression *expr = 
	((El_compound_expression*)eldata->address)->args[1];

      // assume the element address is the second element of the compound expr
      if (expr->is_symbol_expr())
	element_name = (char*)((El_symbol_expression*)expr)->value;
      else if (expr->is_string_expr()) {
	element_name = (char*)((El_string_expression*)expr)->value;
      }
      else {
	cerr << "Error: data_segment.cpp: could not handle WI token " << endl;
	exit (-1);
      }

      // the value offset is the third element of the compound expr
      expr = ((El_compound_expression*)eldata->address)->args[2];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_int_expr())
	value_offset = ((El_value_expression*)expr)->value.i;
      else {
	cerr << "Error: data_segment.cpp: could not handle WB token " << endl;
	exit (-1);
      }

    }
    else {
      cerr << "Error: data_segment.cpp: unhandled addr expr type in WI token" 
	   << (eldata->address->is_compound_expr()) << endl;
      exit(-1);
    }
  }
  else {
    cerr << "Error: data_segment.cpp: WI symbol is null" << endl;
    exit(-1);
  }

  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);  

  if (eldata->value != NULL) {
    element->is_inited = true;

    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_LONG, eldata->value,
					    value_offset));
  }
  else {
    cerr << "Error: data_segment.cpp: val = null" << endl;
    exit(-1);
  }
}

void
Data_segment::handle_wf_token(El_data *eldata)
{
  string element_name;
  int value_offset = 0;

  if (eldata->address) {
    if (eldata->address->is_string_expr())
      element_name = (char*)((El_string_expression*)eldata->address)->value;
    else if (eldata->address->is_symbol_expr())
      element_name = (char*)((El_symbol_expression*)eldata->address)->value;
    else if (eldata->address->is_compound_expr()) {
      El_expression *expr = 
	((El_compound_expression*)eldata->address)->args[1];

      // assume the element address is the second element of the compound expr
      if (expr->is_symbol_expr())
	element_name = (char*)((El_symbol_expression*)expr)->value;
      else if (expr->is_string_expr()) {
	element_name = (char*)((El_string_expression*)expr)->value;
      }
      else {
	cerr << "Error: data_segment.cpp: could not handle WF token " << endl;
	exit (-1);
      }

      // the value offset is the third element of the compound expr
      expr = ((El_compound_expression*)eldata->address)->args[2];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_int_expr())
	value_offset = ((El_value_expression*)expr)->value.i;
      else {
	cerr << "Error: data_segment.cpp: could not handle WB token " << endl;
	exit (-1);
      }

    }
    else {
      cerr << "Error: data_segment.cpp: unhandled addr expr type in WF token" 
	   << (eldata->address->is_compound_expr()) << endl;
      exit(-1);
    }
  }
  else {
    cerr << "Error: data_segment.cpp: WF symbol is null" << endl;
    exit(-1);
  }

  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);  

  if (eldata->value != NULL) {
    element->is_inited = true;

    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_FLOAT, eldata->value,
					    value_offset));
  }
  else {
    cerr << "Error: data_segment.cpp: val = null" << endl;
    exit(-1);
  }
}

void
Data_segment::handle_wf2_token(El_data *eldata)
{
  string element_name;
  int value_offset = 0;

  if (eldata->address) {
    if (eldata->address->is_string_expr())
      element_name = (char*)((El_string_expression*)eldata->address)->value;
    else if (eldata->address->is_symbol_expr())
      element_name = (char*)((El_symbol_expression*)eldata->address)->value;
    else if (eldata->address->is_compound_expr()) {
      El_expression *expr = 
	((El_compound_expression*)eldata->address)->args[1];

      // assume the element address is the second element of the compound expr
      if (expr->is_symbol_expr())
	element_name = (char*)((El_symbol_expression*)expr)->value;
      else if (expr->is_string_expr()) {
	element_name = (char*)((El_string_expression*)expr)->value;
      }
      else {
	cerr << "Error: data_segment.cpp: could not handle WF2 token " << endl;
	exit (-1);
      }

      // the value offset is the third element of the compound expr
      expr = ((El_compound_expression*)eldata->address)->args[2];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_int_expr())
	value_offset = ((El_value_expression*)expr)->value.i;
      else {
	cerr << "Error: data_segment.cpp: could not handle WB token " << endl;
	exit (-1);
      }

    }
    else {
      cerr << "Error: data_segment.cpp: unhandled addr expr type in WF2 token" 
	   << (eldata->address->is_compound_expr()) << endl;
      exit(-1);
    }
  }
  else {
    cerr << "Error: data_segment.cpp: WF2 symbol is null" << endl;
    exit(-1);
  }

  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);  

  if (eldata->value != NULL) {
    element->is_inited = true;

    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_DOUBLE, eldata->value, 
					    value_offset));
  }
  else {
    cerr << "Error: data_segment.cpp: val = null" << endl;
    exit(-1);
  }
}

void
Data_segment::handle_float_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);

  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  element->type = DATA_TYPE_FLOAT;
  element->element_size = 4;
  
  if (eldata->value != NULL) {
    element->is_inited = true;
    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_FLOAT,eldata->value,0));
  }
}

void
Data_segment::handle_double_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);

  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  element->type = DATA_TYPE_DOUBLE;
  element->element_size = 8;
  
  if (eldata->value != NULL) {
    element->is_inited = true;
    element->init_values.
      add_tail(new Data_element_initializer(DATA_TYPE_DOUBLE,eldata->value,0));
  }
}

void
Data_segment::handle_ws_token(El_data *eldata)
{
  string element_name;
  int value_offset = 0;

  if (eldata->address) {
    // figure out what type of expression the address is
    if (eldata->address->is_string_expr())
      element_name = (char*)((El_string_expression*)eldata->address)->value;
    else if (eldata->address->is_symbol_expr())
      element_name = (char*)((El_symbol_expression*)eldata->address)->value;
    else if (eldata->address->is_compound_expr()) {
      El_expression *expr = 
	((El_compound_expression*)eldata->address)->args[1];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_symbol_expr())
	element_name = (char*)((El_symbol_expression*)expr)->value;
      else if (expr->is_string_expr()) {
	element_name = (char*)((El_string_expression*)expr)->value;
      }
      else {
	cerr << "Error: data_segment.cpp: could not handle WS token " << endl;
	exit (-1);
      }

      // the value offset is the third element of the compound expr
      expr = ((El_compound_expression*)eldata->address)->args[2];
      
      // assume the address to be the second element of the compound expr
      if (expr->is_int_expr())
	value_offset = ((El_value_expression*)expr)->value.i;
      else {
	cerr << "Error: data_segment.cpp: could not handle WS token " << endl;
	exit (-1);
      }
    }
    else {
      cerr << "Error: data_segment.cpp: unhandled addr expr type in WS token" 
	   << endl;
      exit(-1);
    }
  }
  else {
    cerr << "Error: data_segment.cpp: WS address symbol is null" << endl;
    exit(-1);
  }

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  string text = ((El_string_expression*)(eldata->value))->value;
  
  // the assembler reequires the following ascii replacements:
  // \b -> \010
  // \t -> \011
  // \n -> \012
  // \f -> \014
  // \r -> \015
  // \ digit digit digit 
  // \x hex-digits
  // \"
  
  int index = 0;
  
  index = text.find("\\b", 0);
  while (index != string::npos) {
    // make sure the sequence isn't already escaped
    if (index > 0 && text[index-1] != '\\') 
      text = text.replace(index, 2, "\\010");
    index = text.find("\\b", index+1);
  }	  
  
  index = text.find("\\t", 0);
  while (index != string::npos) {
    // make sure the sequence isn't already escaped
    if (index > 0 && text[index-1] != '\\') 
      text = text.replace(index, 2, "\\011");
    index = text.find("\\t", index+1);
  }	  
  
  index = text.find("\\n", 0);
  while (index != string::npos) {
    // make sure the sequence isn't already escaped
    if (index > 0 && text[index-1] != '\\') 
      text = text.replace(index, 2, "\\012");
    index = text.find("\\n", index+1);
  }	  

  index = text.find("\\f", 0); 
  while (index != string::npos) {
    // make sure the sequence isn't already escaped
    if (index > 0 && text[index-1] != '\\') 
      text = text.replace(index, 2, "\\014");
    index = text.find("\\f", index+1);
  }	  

  index = text.find("\\r", 0); 
  while (index != string::npos) {
    // make sure the sequence isn't already escaped
    if (index > 0 && text[index-1] != '\\') 
      text = text.replace(index, 2, "\\015");
    index = text.find("\\r", index+1);
  }

  // put an explicit NULL at the end of the string
  text = text.insert(text.length()-1, "\\000");

  element->is_inited = true;
  element->init_values.
    add_tail(new Data_element_initializer(DATA_TYPE_STRING, 
					  new El_string_expression(text), 
					  value_offset));

  // FIXME: hack
  // change the element type (should be previously be a char array)
  element->type = DATA_TYPE_STRING;
}

void
Data_segment::handle_element_size_token(El_data *eldata)
{
  string element_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  // strip off the "_" prefix on labels
  element_name = element_name.substr(1, element_name.length()-1);
  Data_segment_element *element = get_element(element_name);

  // create a new element if it does not exist
  if (element == NULL) {
    element = new Data_segment_element(element_name, this);
    assert(element && "out of memory");
    elements.add_tail(element);
  }

  element->element_size = eldata->N;
}

void
Data_segment::handle_def_struct_token(El_data *eldata)
{
  string struct_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  current_struct_or_union_definition = new Struct_or_union_definition (struct_name);
  global_struct_or_union_definitions->bind (struct_name, current_struct_or_union_definition);
  current_offset = 0;
}

void
Data_segment::handle_field_token(El_data *eldata)
{
  bool is_pointer = false;
  bool is_struct = false;
  bool is_array = false;
  int num_elements = 0;
  int element_size = 0;
  Field_definition *fd;

  if (!current_struct_or_union_definition) {
    cerr << "Error: got field token without a previous struct definition" 
	 << endl;
    exit (-1);
  }

  string field_name = (char *)((El_symbol_expression *) 
			       (eldata->address))->value;

  // read the access declarators to find out if this is an array or a pointer
  for(List_iterator<El_data_access_declarator *> 
	li(((El_data_source_type_info *)(eldata->src_type))->access_dcltrs);
      li!=0;li++){
    El_data_access_declarator *decl = *li;
       
    switch (decl->method) {
    case EL_DATA_ACCESS_ARRAY:
      if( ! num_elements)
	num_elements = 1;
      is_array = true;
      num_elements *= ((El_value_expression*)(decl->index))->value.i;
      break;
    case EL_DATA_ACCESS_POINTER:
      is_pointer = true;
      break;
    default:
      cerr << "ERROR: data_segment.cpp: unhandled access method on: "
	   << current_struct_or_union_definition->struct_name << "." 
	   << fd->field_name << endl;
      exit(-1);
      break;
    }
  }


  Bitvector bv = 
    ((El_data_source_type_info *)(eldata->src_type))->flags;
  
  if (bv.bit(EL_DATA_ST_CHAR)) {
    fd = new Field_definition (field_name, DATA_TYPE_CHAR, current_offset);
    element_size = 1;
  }
  else if (bv.bit(EL_DATA_ST_SHORT)) {
    fd = new Field_definition (field_name, DATA_TYPE_SHORT, current_offset);
    element_size = 2;
  }
  else if (bv.bit(EL_DATA_ST_INT)) {
    fd = new Field_definition (field_name, DATA_TYPE_INT, current_offset);
    element_size = 4;
  }
  else if (bv.bit(EL_DATA_ST_LONG)) {
    fd = new Field_definition (field_name, DATA_TYPE_LONG, current_offset);
    element_size = 4;
  }
  else if (bv.bit(EL_DATA_ST_FLOAT)) {
    fd = new Field_definition (field_name, DATA_TYPE_FLOAT, current_offset);
    element_size = 4;
  }
  else if (bv.bit(EL_DATA_ST_DOUBLE)) {
    fd = new Field_definition (field_name, DATA_TYPE_DOUBLE, current_offset);
    element_size = 8;
  }
  else if (bv.bit(EL_DATA_ST_VOID)) {
    fd = new Field_definition (field_name, DATA_TYPE_VOID, current_offset);
    element_size = 4;
  }
  else if (bv.bit(EL_DATA_ST_STRUCT)) {
    fd = new Field_definition (field_name, DATA_TYPE_STRUCT, current_offset);
    string sn = (char*)((El_data_source_type_info *)
				   (eldata->src_type))->struct_name;

    Pair<string, Struct_or_union_definition*> *pair = 
      global_struct_or_union_definitions->find_entry(sn);
    
    if (!pair && !is_pointer) {
      cerr << "Error: data_segment.cpp: couldn't find struct " 
	   << "definition for: " << sn << endl;
      exit (-1);
    }
    if (is_pointer) {
      element_size = 4;
    } else {
      element_size = pair->second->size;
    }
  }
  else if (bv.bit(EL_DATA_ST_UNION)) {
    fd = new Field_definition (field_name, DATA_TYPE_UNION, current_offset);
    Pair<string, Struct_or_union_definition*> *pair = global_struct_or_union_definitions->
      find_entry(current_struct_or_union_definition->struct_name);
    
    if (!pair) {
      cerr << "Error: data_segment.cpp: couldn't find struct " 
	   << "definition for: " << current_struct_or_union_definition->struct_name 
	   << endl;
      exit (-1);
    }
    
    element_size = pair->second->size;
  }
  else { 
    cerr << "Error: data_segment.cpp: could not handle field type" << endl;
    exit (-1);
  }

  if (is_pointer && is_array) {
    // if this is a pointer access, the emitted type will actually be a word
    fd->is_pointer = true;
    fd->is_array = true;
    fd->type = DATA_TYPE_WORD;
    fd->num_elements = num_elements;
    current_offset += (element_size * fd->num_elements);

    if (current_struct_or_union_definition->is_union) {
      if ((element_size * fd->num_elements) > current_struct_or_union_definition->size) {
	current_struct_or_union_definition->size = (element_size * fd->num_elements);
      }
    }
    else {
      current_struct_or_union_definition->size += (element_size * fd->num_elements);
    }
  }
  else if (is_pointer) {
    // if this is a pointer access, the emitted type will actually be a word
    fd->is_pointer = true;
    fd->type = DATA_TYPE_WORD;
    current_offset += 4;

    if (current_struct_or_union_definition->is_union) {
      if (4 > current_struct_or_union_definition->size)
	current_struct_or_union_definition->size = 4;
    }
    else {
      current_struct_or_union_definition->size += 4;
    }
  }
  else if (is_array) {
    fd->is_array = true;
    fd->num_elements = num_elements;
    current_offset += (element_size * fd->num_elements);

    if (current_struct_or_union_definition->is_union) {
      if ((element_size * fd->num_elements) > current_struct_or_union_definition->size) {
	current_struct_or_union_definition->size = (element_size * fd->num_elements);
      }
    }
    else {
      current_struct_or_union_definition->size += (element_size * fd->num_elements);
    }
  }
  else {
    current_offset += element_size;
    if (current_struct_or_union_definition->is_union) {
      if (element_size > current_struct_or_union_definition->size)
	current_struct_or_union_definition->size = element_size;
    }
    else {
      current_struct_or_union_definition->size += element_size;
    }
  }

  current_struct_or_union_definition->fields.add_tail(fd);
}

void
Data_segment::handle_def_union_token(El_data *eldata)
{
  string union_name = (char *)((El_symbol_expression *) 
				 (eldata->address))->value;

  current_struct_or_union_definition = new Struct_or_union_definition (union_name);
  current_struct_or_union_definition->is_union = 1;
  global_struct_or_union_definitions->bind (union_name, current_struct_or_union_definition);
  current_offset = 0;

}

void
Data_segment::handle_enum_token(El_data *eldata)
{
  cerr << "Error: data_segment.cpp EL_DATA_TOKEN_ENUM not implemented" << 
    endl;
  exit(-1);
}

void
Data_segment::handle_enumerator_token(El_data *eldata)
{
  cerr << "Error: data_segment.cpp EL_DATA_TOKEN_ENUMERATOR not implemented" 
       << endl;
  exit(-1);
}

void
output_el_expression(El_expression *expr)
{
  if (expr->is_int_expr()) {
    cout << ((El_value_expression*)expr)->value.i;
  }
  else if (expr->is_float_expr()) {
    cout << setprecision(10) << ((El_value_expression*)expr)->value.f;
  }
  else if (expr->is_double_expr()) {
    cout << setprecision(20) << ((El_value_expression*)expr)->value.f2;
  }
  else if (expr->is_symbol_expr()) {
    string symbol = ((El_symbol_expression*)expr)->value;

    if (!symbol.compare(0, 5, "_$fn_"))
      symbol = symbol.substr(5, symbol.length() -5);
    else
      // strip off the preceding '_'
      symbol = symbol.substr(1, symbol.length()-1);

    cout << symbol;
  }
  else if (expr->is_string_expr()) {
    cout << ((El_string_expression*)expr)->value;
  }
  else if (expr->is_compound_expr()) {
    Vector<El_expression *> &args = ((El_compound_expression *) expr)->args;

    El_expression_opcode opcode = ((El_op_expression *) args[0])->opcode;
    string op;

    switch (opcode) {
    case ELEX_ADD: 
      op = "+";
      break;
    case ELEX_SUB: 
      op = "-";
      break;
    case ELEX_MUL: 
      op = "*";
      break;
    case ELEX_DIV: 
      op = "/";
      break;
      
    case ELEX_NEG:
    case ELEX_COM:
    default:
      cerr << "Error: could not handle opcode " << opcode 
	   << " in compound expression" << endl;
      exit (-1);
    }

    cout << "(";
    output_el_expression(args[1]);
    cout << " " << op << " ";
    output_el_expression(args[2]);
    cout << ")";
  }
  else {
    cerr << "Error: data_segment.cpp: unknown expression type "  << endl; 
    exit(-1);
  }
}

void
Data_segment_element::emit_element()
{
  int next_expected_offset = 0;

  if (alignment != 1)
    cout << "\t.align " << alignment << endl;
  
  if (is_global)
    cout << "\t.global " << element_name << endl;
  
  // if this is a function, etc, the size will be -1
  if (element_size == -1) {
    return;
  }
  
  // calculate the amount of memory to reserve for this data element
  int reserve_amount = is_array ? 
    (num_elements * element_size) : element_size;
  
  // emit the reserve amount for the memory location
  cout << "\t.comm " << element_name << ", " << reserve_amount
       << endl << endl;
  
  if (is_inited) {
    cout << element_name << ":" << endl;
    
    for(List_iterator<Data_element_initializer*> li(init_values); li!=0; li++){
      Data_element_initializer *initializer = *li;
      
      if (initializer->offset != next_expected_offset) {
	cout << "\t.space " << (initializer->offset - next_expected_offset)
	     << endl;
	next_expected_offset = initializer->offset;
      }

      switch (initializer->type) {
      case DATA_TYPE_CHAR:
	cout << "\t.byte ";
	next_expected_offset += 1;
	break;
      case DATA_TYPE_SHORT:
	cout << "\t.short ";
	next_expected_offset += 2;
	break;
      case DATA_TYPE_INT:
      case DATA_TYPE_WORD:
      case DATA_TYPE_LABEL:
	cout << "\t.word ";
	next_expected_offset += 4;
	break;
      case DATA_TYPE_LONG:
	cout << "\t.long ";
	next_expected_offset += 4;
	break;
      case DATA_TYPE_FLOAT:
	cout << "\t.float ";
	next_expected_offset += 4;
	break;
      case DATA_TYPE_DOUBLE:
	cout << "\t.double ";
	next_expected_offset += 8;
	break;
      case DATA_TYPE_STRING:
	cout << "\t.ascii ";
	// note, we tagged 4 characters on the end of every string
	next_expected_offset += (((El_string_expression*)initializer->
				 value_expression)->value.length() - 5);
	break;
	// we should never see a struct-typed initializer
//       case DATA_TYPE_STRUCT:	
// 	// we don't really know what the data type is here, we only know
// 	// that it is a member of the struct, so we're going to have to
// 	// guess based on the expression type, NB: if the struct is a
// 	// short element we're hosed
// 	if (expr->is_int_expr() || expr->is_symbol_expr())
// 	  cout << "\t.word ";
// 	else if (expr->is_float_expr())
// 	  cout << "\t.float ";
// 	else if (expr->is_double_expr())
// 	  cout << "\t.double ";
// 	else {
// 	  cerr << "Error: data_segment.cpp: struct output underdetermined" << endl;
// 	  exit(-1);
// 	}
// 	break;
      default:
	cerr << "Error: data_segment.cpp: could not output El_expression type: " 
	     << initializer->type << endl;
	exit(-1);
      }

      output_el_expression(initializer->value_expression);
      cout << endl;
    }
    cout << endl;
  }
}

void
Data_segment::emit_segment()
{
  // emit the segment type
  cout << segment_name << endl;

  // iterate through all of the elements that we have and spit out the
  // appropriate gcc assembler codes
  for(List_iterator<Data_segment_element *> li(elements); li!=0; li++){
    Data_segment_element *element = *li;

    element->emit_element();
  }
}
