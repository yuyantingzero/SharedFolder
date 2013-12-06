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
// File:   utils.cpp
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////

#include "simu.h"
#include "streamBuffer.h"

// null string variable
const eString null_string = "";

// external data variables and prototypes information
extern Hash_map<eString, hc_aggregate*> prototypes_table;
extern Hash_table<eString> extern_functions;

// Vector and wrapper candidates
extern Hash_map<eString, int> wrappers;
extern Hash_table<eString>    vectors;

// extern function declarations
extern void convert_type_to_C_format(char *, char *, char *);
extern void sprint_C_prototype(char *output, char *name, char *info);
extern void print_function_declaration(streamBuffer&, hc_aggregate&);


// function declarations
void  gen_prototypes(streamBuffer*);
void  write_externals(streamBuffer*, const Hash_table<eString>&, 
			    enum EXTERN_TYPE);

void  normalize(eString& src, eString& dest);
char* typecast(hc_param& param);


char* typecast(hc_param& param)
{
  static eString retVal;
  
  char  out_buf[MAX_BUF_SIZE];
  char* call_info = param.call_info.strdup();
  
  convert_type_to_C_format(out_buf, call_info, "");
  delete call_info;
  retVal = out_buf;
  
  return (char*) retVal;
}


// generate prototypes for all wrappers and vectors
void gen_prototypes(streamBuffer* out)
{
  static char output_buffer[MAX_BUF_SIZE];
  
  S_log("Writing function prototypes...");
    
  // output the wrappers (functions declared in the file being processed)
  if (!wrappers.is_empty()) {
    S_log("Writing wrapper prototypes...");

    // print out header
    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
		"/*******************************************************/",
		"/************* Wrapper Prototype Declarations **********/",
		"/*******************************************************/");
    (*out) << output_buffer;
    
    for (Hash_map_iterator<eString, int> iterW(wrappers); iterW != 0 ; iterW++) {
      eString function_name = (*iterW).first;
      hc_aggregate& function = *prototypes_table.value(function_name);

      print_function_declaration(*out, function);
      (*out) << ";" << endl;
    
      // remove the current function from the extern fuctions list
      if (extern_functions.is_member(function_name)) {
	  extern_functions.remove(function_name);
      }
    } (*out) << endl;
  }

  // write extern functions prototype declarations for all other functions
  if (!extern_functions.is_empty()) {
    S_log("Writing external function declarations...");

    // print out header
    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
		"/*******************************************************/",
		"/************* Extern Functions Declarations ***********/",
		"/*******************************************************/");
    (*out) << output_buffer;
    
    for (Hash_table_iterator<eString> iterE(extern_functions); 
	   iterE != 0; iterE++) {
      (*out) << "extern " << *iterE << "();" << endl;
    } (*out) << endl;
  }

  // output Vector prototypes
  if (!vectors.is_empty()) {
    S_log("Writing Vector prototypes...");

    // print out header
    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
		"/*******************************************************/",
		"/************* Vector Functions Declarations ***********/",
		"/*******************************************************/");
    (*out) << output_buffer;
  
    for (Hash_table_iterator<eString> iterV(vectors); iterV!=0 ; iterV++) {
      eString vector_name = *iterV;
      hc_aggregate& Vector = *prototypes_table.value(vector_name);
      char* call_info = Vector.call_info.strdup();
      
      sprint_C_prototype(output_buffer, "(*fn)", call_info);
      delete call_info;
      
      (*out) << "static void __PD_vec_" << Vector.name 
		 << "(" << output_buffer << ");" << endl; 
    } (*out) << endl << endl;
  }
}    


//  writes out all external variables
void write_externals(streamBuffer* out, const Hash_table<eString>& externs, 
			   enum EXTERN_TYPE buffer_type)
{
  if (!externs.is_empty()) {
    S_log("Writing external variables...");
    
    for (Hash_table_iterator<eString> iter(externs); iter!=0; iter++) {
      switch (buffer_type) {
      case IS_DATA:
	  (*out) << "extern char* " << *iter << ";" << endl;
	  break;
      case IS_FUNCTION:
	  (*out) << "extern " << *iter << "();" << endl;
	  break;
      case IS_SU:
	  (*out) << *iter << ";" << endl;
	  break;
      default:
	  S_punt("unexpected type while writing externals!");
	  break;
      }
    }
  }
}


// normalize the input file name and converts it to a sequencing id
// used for creating a unique function that initializes global data
void normalize(eString& src, eString& dest)
{
  dest = src.substr(src.charrpos('/', 1)+1, src.length()-1);
  dest = dest.substr(0, dest.charpos('.')-1);

  char* buf = dest.strdup();
  char* ptr = buf;

  // sanity check
  assert (*ptr != 0);
  
  // make sure buffer only contains alphanumeric characters
  while (*ptr != 0) {
    if ((*ptr == '.') | (*ptr == '$') | (*ptr == '-')) {
      *ptr = '_'; // normalize with a legal C character
    }
    ptr++;
  }
  
  dest = buf;
}
