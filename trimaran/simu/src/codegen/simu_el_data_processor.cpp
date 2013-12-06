/******************************************************************************
LICENSE NOTICE
--------------

IT IS  A BREACH OF  THE LICENSE AGREEMENT  TO REMOVE THIS  NOTICE FROM
THIS  FILE  OR SOFTWARE  OR  ANY MODIFIED  VERSIONS  OF  THIS FILE  OR
SOFTWARE.

Copyright notices/Licensor(s) Identification
--------------------------------------------
Each of  the entity(ies) whose name properly  appear immediately below
in connection with a copyright notice is a Licensor(s) under the terms
that follow.

Copyright  1998  New  York  University.  All rights  reserved  by  the
foregoing, respectively.

Copyright 2001 Center for  Research on Embedded Systems and Technology
at the  Georgia Institute  of Technology. All  rights reserved  by the
foregoing, respectively.

License agreement
-----------------

The  code contained  in this  file  including both  binary and  source
(hereafter,  Software)  is subject  to  copyright  by Licensor(s)  and
ownership remains with Licensor(s).

Licensor(s)  grants you  (hereafter, Licensee)  a license  to  use the
Software for  academic, research and internal  business purposes only,
without  a  fee.  "Internal  business  use"  means  that Licensee  may
install, use and execute the Software for the purpose of designing and
evaluating products.   Licensee may also disclose  results obtained by
executing  the  Software,  as  well as  algorithms  embodied  therein.
Licensee may  distribute the Software  to third parties  provided that
the copyright notice and this statement appears on all copies and that
no  charge  is  associated  with  such copies.   No  patent  or  other
intellectual property license is granted or implied by this Agreement,
and this  Agreement does not  license any acts except  those expressly
recited.

Licensee may  make derivative works,  which shall also be  governed by
the  terms of  this  License Agreement.  If  Licensee distributes  any
derivative work based  on or derived from the  Software, then Licensee
will abide by the following terms.  Both Licensee and Licensor(s) will
be  considered joint  owners of  such derivative  work  and considered
Licensor(s) for  the purpose of distribution of  such derivative work.
Licensee shall  not modify this  agreement except that  Licensee shall
clearly  indicate  that  this  is  a  derivative  work  by  adding  an
additional copyright notice in  the form "Copyright <year> <Owner>" to
other copyright notices above, before the line "All rights reserved by
the foregoing, respectively".   A party who is not  an original author
of such derivative works within  the meaning of US Copyright Law shall
not modify or add his name to the copyright notices above.

Any Licensee  wishing to  make commercial use  of the  Software should
contact each and every Licensor(s) to negotiate an appropriate license
for  such  commercial  use;  permission  of all  Licensor(s)  will  be
required for such a  license.  Commercial use includes (1) integration
of all or part  of the source code into a product  for sale or license
by or on  behalf of Licensee to third parties,  or (2) distribution of
the Software  to third  parties that need  it to utilize  a commercial
product sold or licensed by or on behalf of Licensee.

LICENSOR (S)  MAKES NO REPRESENTATIONS  ABOUT THE SUITABILITY  OF THIS
SOFTWARE FOR ANY  PURPOSE.  IT IS PROVIDED "AS  IS" WITHOUT EXPRESS OR
IMPLIED WARRANTY.   LICENSOR (S) SHALL  NOT BE LIABLE FOR  ANY DAMAGES
SUFFERED BY THE USERS OF THIS SOFTWARE.

IN NO EVENT UNLESS REQUIRED BY  APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY  COPYRIGHT HOLDER, OR ANY  OTHER PARTY WHO  MAY MODIFY AND/OR
REDISTRIBUTE THE  PROGRAM AS PERMITTED  ABOVE, BE LIABLE  FOR DAMAGES,
INCLUDING  ANY GENERAL, SPECIAL,  INCIDENTAL OR  CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OR  INABILITY TO USE THE PROGRAM (INCLUDING BUT
NOT  LIMITED TO  LOSS OF  DATA OR  DATA BEING  RENDERED  INACCURATE OR
LOSSES SUSTAINED BY  YOU OR THIRD PARTIES OR A  FAILURE OF THE PROGRAM
TO  OPERATE WITH ANY  OTHER PROGRAMS),  EVEN IF  SUCH HOLDER  OR OTHER
PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

By using  or copying  this Software, Licensee  agrees to abide  by the
copyright law and all other applicable laws of the U.S., and the terms
of  this license  agreement. Any  individual Licensor  shall  have the
right  to terminate this  license immediately  by written  notice upon
Licensee's   breach   of,  or   non-compliance   with,   any  of   its
terms.  Licensee may  be held  legally responsible  for  any copyright
infringement  that is caused  or encouraged  by Licensee's  failure to
abide     by    the     terms    of     this     license    agreement.
******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
// File:   simu_el_data_processor.cpp
// Author: Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


#include "simu.h"
#include "simu_el_processor.h"
#include "streamBuffer.h"

#include "el_data.h"
#include "attributes.h"
#include "iterators.h"

static const int SIZE = 1000;

static const char LEFT_BRACE  = '{';
static const char LEFT_PAREN  = '(';
static const char RIGHT_BRACE = '}';
static const char RIGHT_PAREN = ')';
static const char COMMA1       = ',';
static const char SEMICOLON   = ';';


// list of all labels that are not functions encountered in the
// the emulation tables
extern Hash_table<eString> extern_data_labels;
extern Hash_table<eString> extern_functions;
extern Hash_table<eString> extern_struct_or_union_defs_table;
extern Hash_table<eString> global_struct_or_union_defs_table;

// list of all global variables encountered
Hash_table<eString> global_data_labels(hash_estring, SIZE);

// list of all local functions, should be the same as the wrappers list
// currently, this is unused, but the information is collected regardless
Hash_table<eString> local_functions(hash_estring, SIZE);


// function prototypes
streamBuffer& operator<<(streamBuffer&, El_data_source_type_info*);
streamBuffer& operator<<(streamBuffer&, El_data_access_declarator*);
streamBuffer& operator<<(streamBuffer&, El_expression_opcode&);
streamBuffer& operator<<(streamBuffer&, El_expression*);
void  print_symbol_expr_value(streamBuffer&, El_expression*);
void  print_string_expr_value(streamBuffer&, El_expression*);
void  print_data_stype_flags(streamBuffer&, El_data_source_type_info*);
void  print_struct_or_union(streamBuffer&, List_iterator<El_data *>&);
void  el_to_C_conversion(char*, List<El_data_access_declarator*>&, char*);
char* print_expression_opcode(El_expression_opcode& opc);
void  extract_struct_or_union_info(char* raw_call_info);
void  print_init_code(streamBuffer&, const char*, 
			    El_expression*, El_expression*);


// Writes the data part of the simulator assembly(low-level C) file
void el_Buffer::write_data(El_datalist *datalist)
{
  static char output_buffer[MAX_BUF_SIZE];

  static bool global_data_header_printed = false;
  streamBuffer& out = *data_outbuf;
  streamBuffer  global_def_buf;
  if (datalist == NULL)
    return;
  
  // only print out the header once
  if (!global_data_header_printed) {
    global_data_header_printed = true;

    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
		"/*******************************************************/",
		"/****************** Global Variables *******************/",
		"/*******************************************************/");
    
    (*data_outbuf) << output_buffer;
  }

  List_iterator<El_data *> li(datalist->elements);
  while (li !=0) {
    El_data* data = (*li);
    
    switch (data->t_type) {
    case EL_DATA_TOKEN_VOID:
      out << "void" << SPACE;
      print_symbol_expr_value(out, data->address);
      out << SEMICOLON << endl;
      li++; 
      break;

    case EL_DATA_TOKEN_BYTE:
      out << "char" << SPACE;
      print_symbol_expr_value(out, data->address);
      out << SEMICOLON << endl;

      if (data->value) {
	  initialize_global_data = true;
	  print_init_code(*datainit_outbuf, "char", data->address, data->value);
      }
      li++; 
      break;

    case EL_DATA_TOKEN_WORD:
      out << "short" << SPACE;
      print_symbol_expr_value(out, data->address);
      out << SEMICOLON << endl;

      if (data->value) {
	  initialize_global_data = true;
	  print_init_code(*datainit_outbuf, "short", data->address, data->value);
      }
      li++; 
      break;

    case EL_DATA_TOKEN_LONG:
      out << "long" << SPACE;
      print_symbol_expr_value(out, data->address);
      out << SEMICOLON << endl;

      if (data->value) {
	  initialize_global_data = true;
	  print_init_code(*datainit_outbuf, "long", data->address, data->value);
      }
      li++; 
      break;

    case EL_DATA_TOKEN_LLONG:
      out << "long long" << SPACE;
      print_symbol_expr_value(out, data->address);
      out << SEMICOLON << endl;

      if (data->value) {
	  initialize_global_data = true;
	  print_init_code(*datainit_outbuf, "long long", data->address, data->value);
      }
      li++; 
      break;

    case EL_DATA_TOKEN_FLOAT:
      out << "float" << SPACE;
      print_symbol_expr_value(out, data->address);
      out << SEMICOLON << endl;

      if (data->value) {
	  initialize_global_data = true;
	  print_init_code(*datainit_outbuf, "float", data->address, data->value);
      }
      li++; 
      break;

    case EL_DATA_TOKEN_DOUBLE:
      out << "double" << SPACE;
      print_symbol_expr_value(out, data->address);
      out << SEMICOLON << endl;

      if (data->value) {
	  initialize_global_data = true;
	  print_init_code(*datainit_outbuf, "double", data->address, data->value);
      }
      li++; 
      break;

    case EL_DATA_TOKEN_DEF_STRUCT:
    case EL_DATA_TOKEN_DEF_UNION:
      print_struct_or_union(out, li);
      out << endl;
      break;

    case EL_DATA_TOKEN_GLOBAL: {
      // reset the output buffer
      global_def_buf.clear();

      // convert the variable name to its original declaration
      char* var = (char *) ((El_symbol_expression *) (data->address))->value;
      if (*var == '_') var++;

      if (data->src_type != NULL) {
	  static char buf[MAX_BUF_SIZE];
	
	  print_data_stype_flags(global_def_buf, data->src_type);
	
	  el_to_C_conversion(buf, data->src_type->access_dcltrs, var);
	  global_def_buf << buf << SEMICOLON << endl;

	  // since this variable is encountered as a global data declaration
	  // is the current file, then remove it from the extern_data_labels
	  // list
	  // may need to do this once all procedures have been processed
	  // currently assuming that conflicts will not occur
	  if (extern_data_labels.is_member(var)) {
	    extern_data_labels.remove(var);
	  }

	  // make sure it is added to the list of global_data_labels
	  // this list will be written as the list of externs in the
	  // program data initialization header
	  if (!global_data_labels.is_member(var)) {
	    global_data_labels.add_head(var);
	  }
      }
      else local_functions.add_head(var);

      li++; 
    } break;

    case EL_DATA_TOKEN_ALIGN: {
      // flush out to file any existing declaration
      out << global_def_buf;
      global_def_buf.clear();

      // check to see if a declaration exists for current variable
      char* var = (char *) ((El_symbol_expression *) (data->address))->value;

      if (strncmp(var, "_$$$_", 5) == 0) var += 5;
      else if (strncmp(var, "f_$$$_", 6) == 0) var += 6;
      if (*var == '_') ++var;

      // variable has not yet been declared, create a declaration
      if (!global_data_labels.is_member(var)) {
	  global_data_labels.add_head(var);

	  int   align = data->N;
	  assert (align > 0);

	  // advance the iterator to the next element
	  li++; 
	  data = *li;
	
	  // expecting EL_DATA_TOKEN_RESERVE
	  if (data->t_type != EL_DATA_TOKEN_RESERVE) {
	    S_punt("expecting RESERVE size after ALIGN for %s", var);
	  }
	
	  int struct_size  = data->N;
	  int reserve_size = struct_size / align;
	  char* field_type;
	
	  if ((struct_size % align) != 0) {
	    S_punt("struct size for %s is not aligned by %d, total size was %d",
		     var, align, struct_size);
	  }

	  switch (align) {
	  case 2:
	  case 4:  field_type = "long";   
	    break;
	  case 8:  field_type = "double"; 
	    break;
	  default: field_type = "char";   
	    break;
	  }

	  sprintf(output_buffer, 
		    "struct %s%s\n%c\n%s%s field[%d];\n%c\n%s;\n\n",
		    "__PD_data_", var, LEFT_BRACE, STAB, field_type, 
		    reserve_size, RIGHT_BRACE, var);
	  out << output_buffer;
      }

      li++; 
    } break;

    case EL_DATA_TOKEN_WB:
      initialize_global_data = true;
      print_init_code(*datainit_outbuf, "char", data->address, data->value);
      li++; 
      break;

    case EL_DATA_TOKEN_WW:
      initialize_global_data = true;
      print_init_code(*datainit_outbuf, "short", data->address, data->value);
      li++; 
      break;

    case EL_DATA_TOKEN_WI:
      initialize_global_data = true;
      print_init_code(*datainit_outbuf, "long", data->address, data->value);
      li++; 
      break;

    case EL_DATA_TOKEN_WF:
      initialize_global_data = true;
      print_init_code(*datainit_outbuf, "float", data->address, data->value);
      li++; 
	break;

    case EL_DATA_TOKEN_WF2:
      initialize_global_data = true;
      print_init_code(*datainit_outbuf, "double", data->address, data->value);
      li++; 
      break;

    case EL_DATA_TOKEN_WS:
      initialize_global_data = true;
      (*datainit_outbuf) << STAB << "strcpy((char*)(" << data->address << "), "
				 << "(char*) (" << data->value << "));" 
				 << endl;
      li++; 
      break;

    case EL_DATA_TOKEN_ASCII:
      S_warn("EL_DATA_TOKEN_ASCII is not supported!");
      li++; break;
    case EL_DATA_TOKEN_ASCIZ:
      S_warn("EL_DATA_TOKEN_ASCIZ is not supported!");
      li++; break;
    case EL_DATA_TOKEN_RESERVE:
      li++; break;
    case EL_DATA_TOKEN_MS:
      S_warn("EL_DATA_TOKEN_MZ ignored!");
      li++; 
      break;
    case EL_DATA_TOKEN_ELEMENT_SIZE:
      S_warn("EL_DATA_TOKEN_ELEMENT_SIZE ignored!");
      li++;
      break;

    case EL_DATA_TOKEN_DEF_ENUM:
      S_punt("EL_DATA_TOKEN_DEF_ENUM not expected!");
      break;
    case EL_DATA_TOKEN_FIELD:
      S_punt("EL_DATA_TOKEN_FIELD not expected, handled elsewhere!");
      break;
    case EL_DATA_TOKEN_ENUMERATOR:
      S_punt("EL_DATA_TOKEN_ENUMERATOR not expected!");
      break;
    default:
      S_punt("UNKNOWN EL_DATA_TOKEN TYPE!");
      break;
    }
  }
}


void print_struct_or_union(streamBuffer& out, 
				   List_iterator<El_data *>& field_list)
{
  static streamBuffer out_buf;
  static streamBuffer pre_out_buf;
  static streamBuffer post_out_buf;
  static char dest_buf[MAX_BUF_SIZE];
  static char field_name[MAX_BUF_SIZE];

  El_data* data = (*field_list);
  El_data_source_type_info *stype;

  int field_index = 0;

  if (data->t_type == EL_DATA_TOKEN_DEF_STRUCT)
    out << "struct" << SPACE;
  else if (data->t_type == EL_DATA_TOKEN_DEF_UNION)
    out << "union"  << SPACE;
  else assert (0);

  char* name = (char *) ((El_symbol_expression *) (data->address))->value;
  out << name << SPACE << LEFT_BRACE << endl;
  
  // since a struct/union will be generated, no need to declare this
  // struct/union as an extern
  if (!global_struct_or_union_defs_table.is_member(name)) {
    global_struct_or_union_defs_table.add_head(name);
  }

  while (true) {
    out_buf.clear();
    pre_out_buf.clear();
    post_out_buf.clear();

    field_list++;
    field_index++;

    // all fields for current structure have been defined
    // terminate type definition and return
    if (field_list == 0) break;

    // retrieve current field
    data  = *field_list;
    stype = data->src_type;

    // if this is not a structure field, then all fields for current 
    // structure have been defined - terminate type definition and return
    if (data->t_type != EL_DATA_TOKEN_FIELD) {
      break;
    }

    if (stype == NULL) 
      continue;

    out << STAB;
    print_data_stype_flags(out, stype);

    sprintf(field_name, "field_%d", field_index);
    el_to_C_conversion(dest_buf, stype->access_dcltrs, field_name);
    out << SPACE << dest_buf;

    // add bit field specifier if present
    if (data->value != NULL) {
      out << COLON << data->value;
    }

    // terminate the current field declaration
    out << SEMICOLON << endl;
  }

  out << RIGHT_BRACE << SEMICOLON << endl;
}

// rewrote this function to not use recursion and not require members of List to
// be public.  -KF 5/2005
void el_to_C_conversion(char* dest_buf, 
                        List<El_data_access_declarator*>& info_list,
                        char* incoming_buf)
{
  char cur_buf[MAX_BUF_SIZE];

  strncpy(dest_buf, incoming_buf, MAX_BUF_SIZE);

  for (List_iterator<El_data_access_declarator*> iter(info_list);
       iter != 0; iter++) {
    
    El_data_access_declarator* node = *iter;

    // get the next item in the list
    List_iterator<El_data_access_declarator*> tmp_iter(iter);
    tmp_iter++;
    El_data_access_declarator* next_node;
    if (tmp_iter == 0)
      next_node = NULL;
    else
      next_node = *tmp_iter;

    strncpy(cur_buf, dest_buf, MAX_BUF_SIZE);

    // Handle an array type
    if (node->method == EL_DATA_ACCESS_ARRAY) {
      // If have array with no size info, wrap concatenate [] to 
      // the incoming buffer
      if (node->index == NULL) {
        sprintf(dest_buf, "%s[]", cur_buf);
      }

      // Otherwise, add [size] to end
      else {
        int val = ((El_value_expression *) (node->index))->value.i;
        sprintf(dest_buf, "%s[%i]", cur_buf, val);
      }
    }

    // Handle a pointer followed by an array type
    else if ((node->method == EL_DATA_ACCESS_POINTER) && 
	     (next_node != NULL) &&
	     (next_node->method == EL_DATA_ACCESS_ARRAY)) {
      // Wrap (* ) around whatever has already been parsed or the field/var name
      sprintf(dest_buf, "(*%s)", cur_buf);
    }

    // Handle function pointers (consume the optional leading pointer if present)
    else if ((node->method == EL_DATA_ACCESS_FUNCTION) ||
	     ((node->method == EL_DATA_ACCESS_POINTER)) &&
	     (next_node != NULL) &&
	     (next_node->method == EL_DATA_ACCESS_FUNCTION)) {
      // Consume optional leading pointer
      if (node->method == EL_DATA_ACCESS_POINTER)
        //info_node = info_node->next();
        iter++;
  
      // Wrap field/var name (or previously parsed type) in function
      // pointer specification
      sprintf(dest_buf, "(*%s)()", cur_buf);
    }

    // Handle normal pointers
    else if (node->method == EL_DATA_ACCESS_POINTER) {
      /// Add * to front of incoming_buf
      sprintf(dest_buf, "*%s", cur_buf);
    }

    // Otherwise, punt!
    else {
      S_punt("el_to_C_conversion: unexpected modifier '%i' at '%s'",
	     node->method, cur_buf);
    }
  }
}


// binds all structs or unions to table of encountered structs/unions
void extract_struct_or_union_info(char* raw_call_info)
{
  char  info_string[MAX_BUF_SIZE];
  char  formatted_buf[MAX_BUF_SIZE];
  char* def_ptr;
  char* term_ptr;
  bool  found_struct = false;
  bool  found_union  = false;

  if (raw_call_info[0] == '"') {
    // Copy raw_info_string into info_string, stripping off quotes
    strcpy(info_string, &raw_call_info[1]);
    info_string[strlen(info_string)-1] = 0;
  }
  else {
    strcpy(info_string, raw_call_info);
  }

  def_ptr = info_string;
  while (*def_ptr != 0) {
    if ((def_ptr[0] == 'S') && (def_ptr[1] == '_')) {
      found_struct = true;
    }
    else if ((def_ptr[0] == 'U') && (def_ptr[1] == '_')) {
      found_union = true;
    }

    if (found_struct || found_union) {
      // sanity check
      if ((def_ptr[2] == '+') || (def_ptr[2] == '%') || (def_ptr[2] == '\0')) {
	  S_punt("encountered struct/union has no name in call_info string <%s>",
		   raw_call_info);
      }

      // find the terminating character for the current struct/union
      // the terminating characted is either a '+' (subtype information)
      // a '%' next paramter delimiter or end of string
      term_ptr = &def_ptr[3];
      while ((*term_ptr != '+') && (*term_ptr != '%') && (*term_ptr != '\0')) {
	  term_ptr++;
      }
      // terminate the current structure definition
      char delimiter = *term_ptr;
      *term_ptr = 0;

      // copy struct name
      if (found_struct) {
	  sprintf(formatted_buf, "struct %s", &def_ptr[2]);
      }
      else { // found_union
	  sprintf(formatted_buf, "union %s", &def_ptr[2]);
      }

      // bind the struct/union name to the extern struct/union table 
      // if it is not defined in the current file
      eString name  = &def_ptr[2];
      eString entry = formatted_buf;

      if (!global_struct_or_union_defs_table.is_member(name)) {
	  if (!extern_struct_or_union_defs_table.is_member(entry))
	    extern_struct_or_union_defs_table.add_head(entry);
      }
      
      // advance def_ptr to extract next definition 
      if (delimiter != '\0') {
	  def_ptr = ++term_ptr;
      }
      else break; // done

      found_struct = false;
      found_union  = false;
    }
    
    // advance pointer
    def_ptr++;
  }
}


void print_init_code(streamBuffer& out, const char* cast, 
			   El_expression* id, El_expression* val)
{
  out << STAB
      << "(* (" << cast << "*)(" << id << ")) = " 
      << "(" << cast << ") (" << val << ");" << endl;
}


char* print_expression_opcode(El_expression_opcode& opc)
{
  switch (opc) {
  case ELEX_ADD: return "+";
  case ELEX_SUB: return "-";
  case ELEX_MUL: return "*";
  case ELEX_DIV: return "/";
    
  case ELEX_NEG:
  case ELEX_COM:
  default:
    S_punt("El_expression_opcode not supported!");
    return null_string;
  }
}


streamBuffer& operator<<(streamBuffer& out, El_expression* expr)
{
  if (expr != NULL) {
    if (expr->is_int_expr()) {
      out << ((El_value_expression *) expr)->value.i;
    }
    else if (expr->is_float_expr()) {
      out << ((El_value_expression *) expr)->value.f;
    }
    else if (expr->is_double_expr()) {
      out << ((El_value_expression *) expr)->value.f2;
    }
    else if (expr->is_symbol_expr()) {
      bool is_function   = false;
      bool is_data_label = false;
      char *val;

      val = (char *) ((El_symbol_expression *) expr)->value;

      // convert the label back to the original declaration
      if (strncmp(val, "_$$$_", 5) == 0) val += 5;
      else if (strncmp(val, "f_$$$_", 6) == 0) val += 6;
      else if (strncmp(val, "_$fn",  4) == 0) {
	  val += 4; 
	  is_function = true;
      }
      else is_data_label = true;

      if (*val == '_') ++val;
      
      out << "(long) (&" << val << ")";

      // if this is a function, then add it to the list for extern declaration
      if (is_function && (!extern_functions.is_member(val))) {
	  extern_functions.add_head(val);
      }
      // if this is a data label, then add it to the list for extern data
      // declaration
      else if (is_data_label && 
		   !extern_data_labels.is_member(val) &&
		   !global_data_labels.is_member(val)) {
	  extern_data_labels.add_head(val);
      }
    }
    else if (expr->is_string_expr()) {
      out << (char *) ((El_string_expression *) expr)->value;	
    }
    else if (expr->is_compound_expr()) {
      Vector<El_expression *> &args = ((El_compound_expression *) expr)->args;
      if (args.dim() != 3) {
	  S_punt("unsupported compound expression conversion");
      }

      // convert the opcode to a string
      El_expression_opcode opcode = ((El_op_expression *) args[0])->opcode;
      char* op = print_expression_opcode(opcode);

      out << LEFT_PAREN 
	    << args[1] << SPACE << op << SPACE << args[2] 
	    << RIGHT_PAREN;
    }
    else {
      S_punt("expression type not supported/should be handled elsewhere");
    }
  }

  return out;
}


void print_symbol_expr_value(streamBuffer& out, El_expression* expr)
{
  char *val;

  if (expr == NULL)
    S_punt("print_symbol_expr_value: expression is null");
  if (! expr->is_symbol_expr())
    S_punt("print_symbol_expr_value: expression must be a symbol");

  val = (char *) ((El_symbol_expression *) expr)->value;

  if (*val == '_') val++;

  out << val;
}


void print_string_expr_value(streamBuffer& out, El_expression* expr)
{
  char *val;
  
  if (expr == NULL)
    S_punt("print_string_expr_value: expression is null");
  if (! expr->is_string_expr())
    S_punt("print_string_expr_value: expression must be a string");
  
  val = (char *) ((El_string_expression *) expr)->value;
  out << val;
}


void print_data_stype_flags(streamBuffer& out, El_data_source_type_info* stype)
{
  char* name;
  El_data_stype_flags flag;

  for (flag = El_data_stype_flags(0); flag != EL_DATA_ST_LAST;
       flag = El_data_stype_flags(int(flag) + 1)) {
    if (stype->flags.bit(int(flag)) == 0)
      continue;
    switch (flag) {
    case EL_DATA_ST_VOID:
      out << "void" << SPACE;
      break;
    case EL_DATA_ST_CHAR:
      out << "char" << SPACE;
      break;
    case EL_DATA_ST_SHORT:
      out << "short" << SPACE;
      break;
    case EL_DATA_ST_INT:
      out << "int" << SPACE;
      break;
    case EL_DATA_ST_LONG:
      out << "long" << SPACE;
      break;
    case EL_DATA_ST_LLONG:
      out << "long long" << SPACE;
      break;
    case EL_DATA_ST_FLOAT:
      out << "float" << SPACE;
      break;
    case EL_DATA_ST_DOUBLE:
      out << "double" << SPACE;
      break;
    case EL_DATA_ST_SIGNED:
      out << "signed" << SPACE;
      break;
    case EL_DATA_ST_UNSIGNED:
      out << "unsigned" << SPACE;
      break;
    case EL_DATA_ST_STRUCT:
      name = stype->struct_name;
      out << "struct " << name << SPACE;
      break;
    case EL_DATA_ST_UNION:
      name = stype->struct_name;
      out << "union " << name << SPACE;
      break;

	// ignore the following
    case EL_DATA_ST_CONST:
    case EL_DATA_ST_VOLATILE:
    case EL_DATA_ST_NOALIAS:
    case EL_DATA_ST_REGISTER:
    case EL_DATA_ST_AUTO:
    case EL_DATA_ST_STATIC:
    case EL_DATA_ST_EXTERN:
    case EL_DATA_ST_GLOBAL:
    case EL_DATA_ST_PARAMETER:
    case EL_DATA_ST_ENUM:
    case EL_DATA_ST_VARARG:
      break;
      
    default:
      S_punt("print_data_stype_flags: unknown flag value: %d", flag);
      break;
    }
  }
}


streamBuffer& operator<<(streamBuffer& out, El_data_access_declarator* dcltr)
{
  if (dcltr != NULL) {
    switch (dcltr->method) {
    case EL_DATA_ACCESS_ARRAY:
      out << "[";
      if (dcltr->index != NULL) {
	  assert (dcltr->index->is_int_expr());
	  out << ((El_value_expression *) dcltr->index)->value.i;
      }
      out << "]";
      break;
    case EL_DATA_ACCESS_POINTER:
      out << "*";
      break;
    case EL_DATA_ACCESS_FUNCTION:
    default:
      S_punt("El_data_access_declarator unknown method: %d", dcltr->method);
      break;
    }
  }

  return out;
}


streamBuffer& operator<<(streamBuffer& out, El_data_source_type_info* stype)
{
  if (stype != NULL) {
    print_data_stype_flags(out, stype);

    for (List_iterator<El_data_access_declarator*> li(stype->access_dcltrs);
	   li!=0; li++) {
      El_data_access_declarator *dcltr = *li;
      out << dcltr;
    }
  }

  return out;
}

