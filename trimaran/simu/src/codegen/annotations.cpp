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
// File:   annotations.cpp
// Author: Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************
 * inspired by:
 * impact/src/Lcode/tools/Lbuild_prototype_info/l_build_prototype_info.c
 * (see file for copyright notice. used with permission)
 * sprint_C_prototype, subtype_to_C_string, convert_type_to_C_format
 * and get_next_param_type originally authored by JCG.
 *****************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simu.h"


// function prototypes
void parse_info_string(char *raw_info_string, hc_aggregate& fn);
int  parse_type_prefix(char *raw_buf, hc_param& p);
void parse_type_posfix(char *raw_buf, hc_param& p);
void parse_subt_posfix(char *type_buf,hc_param& p);
void get_next_param_type(char *buf, char **info_ptr);
void sprint_C_prototype(char *out_buf, char *function, char *raw_info_string);
void sprint_KRC_prototype(char* output, char *function, char *raw_info_string);
void subtype_to_C_string(char *dest_buf, char *type_buf, char *incoming_buf);
void convert_type_to_C_format(char *formatted_buf, char *raw_buf, char *param);


// Copies the next parameter type (strips of terminating %) into buf.
// buf will be empty if there are not more parameter types left. 
void get_next_param_type (char *buf, char **info_ptr)
{
  char *ptr;

  ptr = *info_ptr;

  // Copy over next parm type into buf 
  while ((*ptr != 0) && (*ptr != '%')) {
    *buf = *ptr;
    buf++;
    ptr++;
  }
    
  // Skip % 
  if (*ptr == '%')
    ptr ++;
    
  // Terminate buf 
  *buf = 0;
    
  // Move info_ptr to beginning of next parameter 
  *info_ptr = ptr;
}


void parse_info_string(char *raw_info_string, hc_aggregate& fn)
{
  char info_string[MAX_BUF_SIZE], raw_buf[MAX_BUF_SIZE];
  char return_type_buf[MAX_BUF_SIZE];
  char *parse_ptr;
    
  // If string is quoted, strip of quotes 
  if (raw_info_string[0] == '"') {
    // Copy raw_info_string into info_string, stripping off quotes 
    strcpy(info_string, &raw_info_string[1]);
    info_string[strlen(info_string)-1] = 0;
  }

  // Otherwise, just copy string 
  else strcpy(info_string, raw_info_string);
    
  // Get "raw" return type for function 
  parse_ptr = info_string;
  get_next_param_type(return_type_buf, &parse_ptr);

  // set return type
  fn.params.resize(1);
  fn.count     = 1;
  fn.params[0].call_info = return_type_buf;
  //    cout << "[" << return_type_buf << "]";
  fn.isVararg  = parse_type_prefix(return_type_buf, fn.params[0]);
  parse_type_posfix(return_type_buf, fn.params[0]);

  // the first parameter is the return type
  int p_count = 1;
  // Process each parameter 
  while (*parse_ptr != 0) {
    // Get the type of the next parameter 
    get_next_param_type(raw_buf, &parse_ptr);
	
    fn.params.resize(p_count+1);

    // Convert type to formatted string using hcode conventions
    // (which separates the prefix and the postfix).
    // cout << " [" << raw_buf << "]";
    fn.params[p_count].call_info = raw_buf;
    fn.isVararg = parse_type_prefix(raw_buf, fn.params[p_count]);
    parse_type_posfix(raw_buf, fn.params[p_count]);

    fn.count++;
    p_count++;
  }
  // cout << endl;
}


// Store the type prefix (the stuff before the '+') into hc_param
int parse_type_prefix(char *raw_buf, hc_param& p)
{
  char temp_buf[MAX_BUF_SIZE], *temp_ptr, *raw_ptr;
    
  // Sanity check, raw_buf should not be NULL or empty 
  if ((raw_buf == NULL) || (*raw_buf == 0)) {
    S_punt("parse_type_prefix: raw_buf empty or NULL!");
  }

  // Copy type_name into temp_buf (string before '+') 
  temp_ptr = temp_buf;
  raw_ptr  = raw_buf;
  while ((*raw_ptr != 0) && (*raw_ptr != '+')) {
    *temp_ptr = *raw_ptr;
    temp_ptr++;
    raw_ptr++;
  }
  *temp_ptr = 0;  // Terminate temp_buf 

  // Move raw_ptr past '+', if exists 
  if (*raw_ptr == '+') raw_ptr++;

  // Handle struct names 
  if ((temp_buf[0] == 'S') && (temp_buf[1] == '_')) {
    p.type = STRUCT;
    p.name = (char*) &temp_buf[2];
  }
    
  // Handle union names 
  else if ((temp_buf[0] == 'U') && (temp_buf[1] == '_')) {
    p.type = UNION;
    p.name = (char*) &temp_buf[2];
  }
    
  // Handle all the other type types 
  else if (strcmp (temp_buf, "void")   == 0)	 
    p.type = VOID;	
  else if (strcmp (temp_buf, "char")   == 0)	 
    p.type = CHAR;
  else if (strcmp (temp_buf, "uchar")  == 0) 
    { p.type = CHAR;  p.sgnd = false; }
  else if (strcmp (temp_buf, "short")  == 0)	 
    p.type = SHORT;
  else if (strcmp (temp_buf, "ushort") == 0) 
    { p.type = SHORT; p.sgnd = false; }
  else if (strcmp (temp_buf, "int")    == 0)	
    p.type = INT;
  else if (strcmp (temp_buf, "uint")   == 0) 
    { p.type = INT;	 p.sgnd = false; }
  else if (strcmp (temp_buf, "long")   == 0)	
    p.type = LONG;
  else if (strcmp (temp_buf, "longlong")   == 0)	
    p.type = LONGLONG;
  else if (strcmp (temp_buf, "ulonglong")   == 0)	
    { p.type = LONGLONG; p.sgnd = false; }
  else if (strcmp (temp_buf, "ulong")  == 0) 
    { p.type = LONG;	 p.sgnd = false; }
  else if (strcmp (temp_buf, "float")  == 0)	
    p.type = FLOAT;
  else if (strcmp (temp_buf, "double") == 0)	
    p.type = DOUBLE;
  else if (strcmp (temp_buf, "vararg") == 0) 
    { p.type = ELLIPSIS; return true; }
  else {
    S_punt("parse_type_prefix: Unknown type %s in %s", temp_buf, raw_buf);
  }
  return false;
}


// Parse the stuff after the '+' and hands it off to parse_subt_posfix().
void parse_type_posfix(char *raw_buf, hc_param& p)
{
  char *raw_ptr;

  // Sanity check, raw_buf should not be NULL or empty 
  if ((raw_buf == NULL) || (*raw_buf == 0)) {
    S_punt("parse_type_postfix: raw_buf empty or NULL!");
  }

  // Skip to postfix (after '+', if any) 
  raw_ptr = raw_buf;
  while ((*raw_ptr != 0) && (*raw_ptr != '+')) raw_ptr++;

  // Move raw_ptr past '+', if exists 
  if (*raw_ptr == '+') raw_ptr++;
    
  // Handle the rest of the type info 
  parse_subt_posfix(raw_ptr, p);
}


// Store the part of the type 'postfix' (the stuff after the '+') into
// hc_param.  
void parse_subt_posfix(char *type_buf, hc_param& p)
{
  // nothing left to parse, just set some final flags.
  if (*type_buf == 0) {
    p.isArray = p.vector_size && 1;

    if ( p.desc != CODE_POINTER ) {
      if ( p.isArray || p.depth ) p.desc = DATA_POINTER;
      else p.desc = NIL; // not necessary (by defualt)
    }

    return;
  }

  char temp_buf[MAX_BUF_SIZE], *type_ptr, *temp_ptr;

    // Point at type_buf for parsing 
  type_ptr = type_buf;

  // Handle array types (first index unspecified).
  // Converts 'PA10A20' into '(A) (A (signed 10)) (A (signed 20))'
    
  if (type_ptr[0] == 'P' && type_ptr[1] == 'A') {
    // Specify 'array with unspecified index' 
    p.array_info_vector.resize(p.vector_size+1);
    p.array_info_vector[p.vector_size++] = -1;	
	
    // Skip the P 
    type_ptr++;
	
    // Process all the Array specifications 
    while (type_ptr[0] == 'A') {
      // Skip the A 
      type_ptr++;
	    
      // Copy number into temp buf 
      temp_ptr = temp_buf;
      while ((*type_ptr != 0) && isdigit(*type_ptr))  {
	  *temp_ptr = *type_ptr;
	  temp_ptr++;
	  type_ptr++;
      }
      temp_ptr[0] = 0;
      p.array_info_vector.resize(p.vector_size+1);
      p.array_info_vector[p.vector_size++] = atoi(temp_buf);
    }

    // Convert the rest of the subtype recursively 
    parse_subt_posfix(type_ptr, p);
  }

  // Handle function pointers. 
  // Converts 'PF' into 'P (F)' 
  else if ( (type_ptr[0] == 'F') || 
		((type_ptr[0] == 'P') && (type_ptr[1] == 'F'))) {
    // Consume optional leading P (but should always be there anyways)
    if (type_ptr[0] == 'P') type_ptr++;
	
    // Skip the F 
    type_ptr++;
	
    p.desc = CODE_POINTER;
    p.code_depth++;
    // Convert the rest of the subtype recursively 
    parse_subt_posfix(type_ptr, p);
  }
    
  // Handle normal pointers 
  else if ((type_ptr[0] == 'P') && (type_ptr[1] != 'F')) {
    // Skip P 
    type_ptr++;
	
    // increment the pointer depth count in the object
    p.depth++; 

    // Convert the rest of the subtype recursively 
    parse_subt_posfix(type_ptr, p);
  }

  // Otherwise, punt since I don't know what this is 
  else {
    S_punt("parse_subtype_postfix: unexected modifier %s in %s.", 
	     type_ptr, type_buf);
  }
}


// Print out C prototype given the function name
// and the call_info.  Handles both quoted and unquoted raw_info_strings.
void sprint_C_prototype(char *out_buf, char *function, char *raw_info_string)
{
  char info_string[MAX_BUF_SIZE], raw_buf[MAX_BUF_SIZE];
  char formatted_buf[MAX_BUF_SIZE], name_buf[MAX_BUF_SIZE];
  char return_type_buf[MAX_BUF_SIZE], main_buf[MAX_BUF_SIZE];
  char *parse_ptr;
  int index;
    
  // If string is quoted, strip of quotes 
  if (raw_info_string[0] == '"') {
    // Copy raw_info_string into info_string, stripping off quotes
    strcpy(info_string, &raw_info_string[1]);
    info_string[strlen(info_string)-1] = 0;
  }

  else {
    strcpy(info_string, raw_info_string);
  }

  // Get "raw" return type for function
  parse_ptr = info_string;
  get_next_param_type(return_type_buf, &parse_ptr);
    

  // Print function name and later parameters to formatted_buf, return 
  // type will be wrapped around everything (necessary for returning 
  // function pointers) at the end.
  sprintf(formatted_buf, "%s (", function);

  index = 1;
  while (*parse_ptr != 0) {
    // Get the next parameter type
    get_next_param_type(raw_buf, &parse_ptr);
      
    // Make up a name for the parameter
    sprintf(name_buf, "p%i", index);
      
    // Convert type to formatted string using C conventions,
    // using name_bud as the 'parameter'
    convert_type_to_C_format(main_buf, raw_buf, name_buf);
	    
    // Add to end of formatted string
    strcat(formatted_buf, main_buf);
	    
    // Increment parameter id
    index++;
	    
    // Add comma if not at end 
    if (*parse_ptr != 0)
      strcat(formatted_buf, ", ");
  }

  // Add closing )
  strcat(formatted_buf, ")");
    
  // Convert return type to formatted string using C conventions,
  // using function name + parms as the 'parameter'
  // Copy formatted_buf into main_buf because buffers must not
  // overlap and I want the result in formatted_buf.
  strcpy(main_buf, formatted_buf);
  convert_type_to_C_format(formatted_buf, return_type_buf, main_buf);

  sprintf(out_buf, "%s", formatted_buf);
}


// Print out C prototype given the function name
// and the call_info.  Handles both quoted and unquoted raw_info_strings.
void sprint_KRC_prototype(char *out_buf, char *function, char *raw_info_string)
{
  char info_string[MAX_BUF_SIZE], raw_buf[MAX_BUF_SIZE];
  char formatted_buf[MAX_BUF_SIZE], name_buf[MAX_BUF_SIZE];
  char return_type_buf[MAX_BUF_SIZE], main_buf[MAX_BUF_SIZE];
  char parameter_buf[MAX_BUF_SIZE], all_parameters_buf[MAX_BUF_SIZE];
  char *parse_ptr;
  int index;
    
  // If string is quoted, strip of quotes 
  if (raw_info_string[0] == '"') {
    // Copy raw_info_string into info_string, stripping off quotes
    strcpy(info_string, &raw_info_string[1]);
    info_string[strlen(info_string)-1] = 0;
  }

  else {
    strcpy(info_string, raw_info_string);
  }

  // Get "raw" return type for function
  parse_ptr = info_string;
  get_next_param_type(return_type_buf, &parse_ptr);
    

  // Print function name and later parameters to formatted_buf, return 
  // type will be wrapped around everything (necessary for returning 
  // function pointers) at the end.
  sprintf(formatted_buf, "%s (", function);
  sprintf(all_parameters_buf, "\t");

  index = 1;
  while (*parse_ptr != 0) {
    // Get the next parameter type
    get_next_param_type(raw_buf, &parse_ptr);
      
    // Make up a name for the parameter
    sprintf(name_buf, "p%i", index);

    // Convert type to formatted string using C conventions,
    // using name_bud as the 'parameter'
    convert_type_to_C_format(parameter_buf, raw_buf, name_buf);
    strcat(all_parameters_buf, parameter_buf);
	    
    // Add to end of formatted string
    strcat(formatted_buf, name_buf);
	    
    // Increment parameter id
    index++;
	    
    // Add comma if not at end 
    if (*parse_ptr != 0) {
      strcat(formatted_buf, ", ");
      strcat(all_parameters_buf, "; ");
    }
  }

  // Add closing )
  strcat(formatted_buf, ")\n");
    
  // Convert return type to formatted string using C conventions,
  // using function name + parms as the 'parameter'
  // Copy formatted_buf into main_buf because buffers must not
  // overlap and I want the result in formatted_buf.
  strcpy(main_buf, formatted_buf);
  convert_type_to_C_format(formatted_buf, return_type_buf, main_buf);

  sprintf(out_buf, "%s", formatted_buf);
  strcat(out_buf, all_parameters_buf);
}


// Convert the type postfix (the stuff after the '+') into C format.
// Do this recursively and piece by piece, since the ordering
// of the types makes a non-recursive routine difficult to write.
void subtype_to_C_string(char *dest_buf, char *type_buf, char *incoming_buf)
{
  char temp_buf[MAX_BUF_SIZE], *type_ptr, *temp_ptr;
  char sub_buf[MAX_BUF_SIZE];

  // Point at type_buf for parsing
  type_ptr = type_buf;

  // Terminate dest_buf initially
  dest_buf[0] = 0;
  
  // If nothing, just copy the incoming buf to the outgoing buf
  if (*type_ptr == 0)
    strcat(dest_buf, incoming_buf);

  // Handle an array type.
  // Converts 'PA10A20' into '(*name)[10][20]'
  //
  else if (type_ptr[0] == 'P' && type_ptr[1] == 'A') {
    // Wrap (* ) around whatever has already been parsed
    // or the parameter name. -ITI/JCG 4/99
    sprintf(sub_buf, "(*%s)", incoming_buf);

    // Skip the P
    type_ptr++;

    // Process all the Array specifications
    while (type_ptr[0] == 'A') {
      // Print out this array def
      // Skip the A 
      type_ptr++;
	    
      // Add openning [
      strcat (sub_buf, "[");
	    
      // Copy number into temp buf
      temp_ptr = temp_buf;
      while ((*type_ptr != 0) && isdigit(*type_ptr)) {
	  *temp_ptr = *type_ptr;
	  temp_ptr++;
	  type_ptr++;
      }
      // Add closing ]
      temp_ptr[0] = ']';
      temp_ptr[1] = 0;

      // Add to the end of the existing format string 
      strcat (sub_buf, temp_buf);
    }

    // Convert the rest of the subtype recursively
    subtype_to_C_string (dest_buf, type_ptr, sub_buf);
  }

  // Handle function pointers
  else if ((type_ptr[0] == 'F') ||
	     ((type_ptr[0] == 'P') && (type_ptr[1] == 'F'))) {
    // Consume optional leading P (should always be there now)
    if (type_ptr[0] == 'P')
      type_ptr++;

    // Skip the F 
    type_ptr++;

    // Wrap parameter name (or previously parsed type) in function 
    // pointer specification 
    sprintf(sub_buf, "(*%s)()", incoming_buf);

    // Convert the rest of the subtype recursively
    subtype_to_C_string(dest_buf, type_ptr, sub_buf);
  }

  // Handle normal pointers
  else if ((type_ptr[0] == 'P') && (type_ptr[1] != 'F')) {
    // Skip P
    type_ptr++;

    // Add * to front of incoming_buf
    sprintf(sub_buf, "*%s", incoming_buf);

    // Convert the rest of the subtype recursively
    subtype_to_C_string(dest_buf, type_ptr, sub_buf);
  }
   
  // Otherwise, punt since I don't know what this is
  else
    S_punt("subtype_to_C_string: unexected modifier '%s' in '%s'.",
	     type_ptr, type_buf);
}


// Convert "raw" type string into C-style format.
// Uses parm_name to create formatted_buf.  Parm name should be
// the function name WITH parameters when creating return type.
void convert_type_to_C_format(char *formatted_buf, char *raw_buf, char *param)
{
  char temp_buf[MAX_BUF_SIZE], *temp_ptr, *raw_ptr;

  // Sanity check, raw_buf should not be NULL or empty
  if ((raw_buf == NULL) || (*raw_buf == 0))
    S_punt ("convert_type_to_C_format: raw_buf empty or NULL!");

  // Copy type_name into temp_buf (string before '+')
  temp_ptr = temp_buf;
  raw_ptr = raw_buf;
  while ((*raw_ptr != 0) && (*raw_ptr != '+')) {
    *temp_ptr = *raw_ptr;
    temp_ptr++;
    raw_ptr++;
  }

  // Terminate temp_buf
  *temp_ptr = 0;

  // Move raw_ptr past '+', if exists
  if (*raw_ptr == '+')
    raw_ptr++;

  // Handle struct names
  if ((temp_buf[0] == 'S') && (temp_buf[1] == '_')) {
    sprintf(formatted_buf, "struct %s ", &temp_buf[2]);
  }

  // Handle union names 
  else if ((temp_buf[0] == 'U') && (temp_buf[1] == '_')) {
    sprintf(formatted_buf, "union %s ", &temp_buf[2]);
  }
  
  // Handle vararg type 
  else if (strcmp (temp_buf, "vararg") == 0) {
    sprintf (formatted_buf, "...");
    /* No function name, so return now */
    return;
  }

  else if (strcmp (temp_buf, "uchar") == 0) {
    sprintf (formatted_buf, "unsigned char ");
  }

  else if (strcmp (temp_buf, "ushort") == 0) {
    sprintf (formatted_buf, "unsigned short ");
  }

  else if (strcmp (temp_buf, "uint") == 0) {
    sprintf (formatted_buf, "unsigned int ");
  }

  else if (strcmp (temp_buf, "ulong") == 0) {
    sprintf (formatted_buf, "unsigned long ");
  }

  else if (strcmp (temp_buf, "longlong") == 0) {
    sprintf (formatted_buf, "long long ");
  }

  else if (strcmp (temp_buf, "ulonglong") == 0) {
    sprintf (formatted_buf, "unsigned long long ");
  }

  // Handle all the other type types
  else if ((strcmp (temp_buf, "void") == 0) ||
	     (strcmp (temp_buf, "char") == 0) ||
	     (strcmp (temp_buf, "short") == 0) ||
	     (strcmp (temp_buf, "int") == 0) ||
	     (strcmp (temp_buf, "long") == 0) ||
	     (strcmp (temp_buf, "float") == 0) ||
	     (strcmp (temp_buf, "double") == 0)) {
    sprintf (formatted_buf, "%s ", temp_buf);
  }

  // Otherwise, punt on unknown type
  else {
    S_punt("convert_type_to_C_format: Unknown type '%s' in '%s' string!",
	     temp_buf, raw_buf);
  }

  // Handle the rest of the type info
  subtype_to_C_string (temp_buf, raw_ptr, param);

  strcat (formatted_buf, temp_buf);
}
