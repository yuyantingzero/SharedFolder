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
// File:   vectorizer.cpp
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


#include "simu.h"
#include "streamBuffer.h"
#include "mdes.h"


// list of functions for which to generate vectors
extern Hash_table<eString> vectors;

// List of all functions and parameter info
extern Hash_map<eString, hc_aggregate*> prototypes_table;


// function declarations
void  gen_vectors(ofstream &outf);
void  write_vector(streamBuffer &out_defs, hc_aggregate& func);
bool  pass_SU_by_value(hc_param& p);
eString print(Macro_name macro);

// return typedef value of function pointers return by other functions
extern void  sprint_C_prototype(char *output, char *name, char *info);
extern void  print_function_declaration(streamBuffer&, hc_aggregate&);
extern char* typecast(hc_param&);
extern char* promote_param(const hc_param& params);

extern int S_stack_manipulated_in_elcor;
extern int S_return_value_thru_stack;
extern int S_return_value_offset;

// Write out a Vector for all Vector candidates
void gen_vectors(ofstream &outf)
{
  streamBuffer out(outf);
  S_log("Writing vectors...");
    
  // write the header part
  out << endl << endl
      << "/*********************************************************/" << endl
      << "/******************** Vector Functions *******************/" << endl
      << "/*********************************************************/" << endl
      << endl << endl;
  
  for (Hash_table_iterator<eString> iter(vectors); iter!=0 ; iter++) {
    eString e = *iter;
    hc_aggregate& func = *prototypes_table.value(e);
    
    write_vector(out, func);
  }
    
  out.commit();
}


// Write out a Vector for one function
void write_vector(streamBuffer &out_defs, hc_aggregate& func)
{
  // print out the actual function to be called as a parameter to the
  // Vector
  char  out_buf[MAX_BUF_SIZE];
  char* call_info = func.call_info.strdup();
  
  sprint_C_prototype(out_buf, "(*fn)", call_info);
  delete call_info;
  out_defs << "static void __PD_vec_" << func.name << "(" << out_buf << ")"
	     << endl << "{" << endl;
  
  bool flag = false;

  /* KVM : Declare place holders for the long long params, if any.
   */
  for(int index = 1; index < func.params.dim(); index++) {
    hc_param& param = func.params[index];
    if(param.type == LONGLONG) {
      out_defs << STAB << "union { struct { int lo, hi; } parts; long long whole; } "
               << "longlongparam_" << index << ";" << endl;
    }
  }

  for(int index = 1; index < func.params.dim(); index++) {
    hc_param& param = func.params[index];
    if(param.desc != DATA_POINTER) {
      if(param.type == LONGLONG) {
        if (param.macro_reg) {
          out_defs << STAB << "longlongparam_" << index << ".parts.lo = (" << typecast(param) << ") (" 
                   << promote_param(param) << ")" << print(param.macro_name) 
                   << ";" << endl;
          out_defs << STAB << "longlongparam_" << index << ".parts.hi = (" << typecast(param) << ") (" 
                   << promote_param(param) << ")" << print(param.second_macro_name) 
                   << ";" << endl;
        }

      }
    }
  }
  
  // Write the return type
  DATA_DESC desc = func.params[0].desc;
  DATA_TYPE type = func.params[0].type;
  switch(desc) {
  case CODE_POINTER:
  case DATA_POINTER:
    out_defs << STAB << print(INT_RETURN) << "= (long) " << endl;
    break;
	
  default:
    switch(type) {
    case CHAR:
    case SHORT:
    case LONG:
    case INT:
      out_defs << STAB << print(INT_RETURN) << "= (long) " << endl;
      break;
    case LONGLONG:
      out_defs << STAB << "const long long retval " << "= (long long) " << endl;
      break;
    case FLOAT:
      out_defs << STAB << print(FLT_RETURN) << "= (float) " << endl;
      break;
	    
    case DOUBLE:
      out_defs << STAB << print(DBL_RETURN) << "= (double) " << endl;
      break;
	    
	// return by value
	// store the pointer to the struct/union in register
    case STRUCT:
      out_defs << STAB << "const struct " << func.params[0].name 
		   << " retval = " << endl;

      flag = true;
      break;
    case UNION:
      out_defs << STAB << "const union " << func.params[0].name 
		   << " retval = " << endl;

      flag = true;
      break;
	    
    default:
      break;
    }
  }

  // call the actual function
  out_defs << STAB << "(*fn) (" << endl << STAB << " ";
    
  // now print out the parameters
  for(int index = 1; index < func.params.dim(); index++) {
    hc_param& param = func.params[index];

    if (pass_SU_by_value(param)) {
      if (param.macro_reg) {
	  out_defs << STAB << "*((" << typecast(param) << "*) "
		     << print(param.macro_name) << ")" 
		     << endl;
      }
      
      else {
	  int actual_offset = param.has_soffset ? param.soffset : param.offset;
          if (S_stack_manipulated_in_elcor)
	    out_defs << STAB << "*((" << typecast(param) << "*) " 
		     << "(__PD_SP_REG + " << actual_offset << "))" << endl;
          else
	    out_defs << STAB << "*((" << typecast(param) << "*) " 
		     << "(__PD_OP_REG + " << actual_offset << "))" << endl;
      }
    }
      
    else {
      if (param.macro_reg) {
        if(param.type == LONGLONG) {
          out_defs << STAB << "longlongparam_" << index << ".whole" << endl;
        }
        else {
	  out_defs << STAB << "(" << typecast(param) << ") (" 
                   << promote_param(param) << ")" << print(param.macro_name) 
                   << endl;
        }
      }
      
      else {
          if (S_stack_manipulated_in_elcor)
	    out_defs << STAB << "(" << typecast(param) << ") " 
		     << "(*((" << promote_param(param) << "*) "
		     << "(__PD_SP_REG + " << param.offset << ")))" << endl;
          else
	    out_defs << STAB << "(" << typecast(param) << ") " 
		     << "(*((" << promote_param(param) << "*) "
		     << "(__PD_OP_REG + " << param.offset << ")))" << endl;
      }
    }
    
    if (index < func.params.dim()-1)
      out_defs << STAB << ",";
  }

  out_defs << STAB << ");" << endl;

  if (flag) {
    if (S_return_value_thru_stack) {
      if (S_stack_manipulated_in_elcor)
        out_defs << endl << STAB
                 << "*((" << typecast(func.params[0]) << "*)"
                 << "(*(( long *) "
                 << "(__PD_SP_REG + " << S_return_value_offset << ")))) = retval;"
                 << endl;
      else
        out_defs << endl << STAB
                 << "*((" << typecast(func.params[0]) << "*)"
                 << "(*(( long *) "
                 << "(__PD_OP_REG + " << S_return_value_offset << ")))) = retval;"
                 << endl;
    } else
      out_defs << endl << STAB << print(INT_RETURN) << " = (long) &retval;" << endl;
  } else {
    /* !flag */
    if(type == LONGLONG) {
      if (S_stack_manipulated_in_elcor)
        out_defs << endl << STAB << "(*(( long long *) "
                 << "(__PD_SP_REG + " << S_return_value_offset << "))) = "<< "retval;" << endl;
      else
        out_defs << endl << STAB << "(*(( long long *) "
                 << "(__PD_OP_REG + " << S_return_value_offset << "))) = "<< "retval;" << endl;

    }
    if (S_return_value_thru_stack) {
      switch(desc) {
        case CODE_POINTER:
        case DATA_POINTER:
          if (S_stack_manipulated_in_elcor)
            out_defs << endl << STAB << "(*(( long *) "
                     << "(__PD_SP_REG + " << S_return_value_offset << "))) = "<< print(INT_RETURN) <<";" << endl;
          else
            out_defs << endl << STAB << "(*(( long *) "
                     << "(__PD_OP_REG + " << S_return_value_offset << "))) = "<< print(INT_RETURN) <<";" << endl;
          break;
        default:
          switch(type) {
            case CHAR:
            case SHORT:
            case LONG:
            case INT:
              if (S_stack_manipulated_in_elcor)
                out_defs << endl << STAB << "(*(( long *) "
                         << "(__PD_SP_REG + " << S_return_value_offset << "))) = "<< print(INT_RETURN) <<";" << endl;
              else
                out_defs << endl << STAB << "(*(( long *) "
                         << "(__PD_OP_REG + " << S_return_value_offset << "))) = "<< print(INT_RETURN)<<";" << endl;
              break;
            case FLOAT:
              if (S_stack_manipulated_in_elcor)
                out_defs << endl << STAB << "(*(( float *) "
                         << "(__PD_SP_REG + " << S_return_value_offset << "))) ="<< print(FLT_RETURN) << ";" << endl;
              else
                out_defs << endl << STAB << "(*(( float *) "
                         << "(__PD_OP_REG + " << S_return_value_offset << "))) ="<< print(FLT_RETURN) <<";" << endl;
              break;
            case DOUBLE:
              if (S_stack_manipulated_in_elcor)
                out_defs << endl << STAB << "(*(( double *) "
                         << "(__PD_SP_REG + " << S_return_value_offset << "))) ="<< print(DBL_RETURN)<< ";" << endl;
              else
                out_defs << endl << STAB << "(*(( double *) "
                         << "(__PD_OP_REG + " << S_return_value_offset << "))) ="<< print(DBL_RETURN)<<" ;" << endl;
              break;
            default:
              break;
          }
      }
    }
  }

  out_defs << "}" << endl << endl;
}


// returns true is the current parameter is a structure or union passed
// by value
bool pass_SU_by_value(hc_param& p) 
{
  return ((p.desc == NIL) & ((p.type == STRUCT) | (p.type == UNION)));
}

extern int S_virtual_regs;
extern Hash_map<eString, int> stat_phys_offset;

// convert the macro register name to the string equivalent
eString print(Macro_name macro)
{
  if (S_virtual_regs) {
    switch (macro) {
      case INT_PARAM_1: return "__PD_INT_P1_REG";
      case INT_PARAM_2: return "__PD_INT_P2_REG";
      case INT_PARAM_3: return "__PD_INT_P3_REG";
      case INT_PARAM_4: return "__PD_INT_P4_REG";
      case FLT_PARAM_1: return "__PD_FLT_P1_REG";
      case FLT_PARAM_2: return "__PD_FLT_P2_REG";
      case FLT_PARAM_3: return "__PD_FLT_P3_REG";
      case FLT_PARAM_4: return "__PD_FLT_P4_REG";
      case DBL_PARAM_1: return "__PD_DBL_P1_REG";
      case DBL_PARAM_2: return "__PD_DBL_P2_REG";
      case DBL_PARAM_3: return "__PD_DBL_P3_REG";
      case DBL_PARAM_4: return "__PD_DBL_P4_REG";
      case INT_RETURN: return "__PD_INT_RET_REG";
      case FLT_RETURN: return "__PD_FLT_RET_REG";
      case DBL_RETURN: return "__PD_DBL_RET_REG";
      default:
			S_punt("Unknown macro register during Vector generation!");
			return null_string;
    }
  } else {
    Hash_set<eString> physical_files;
    Hash_set<eString> phys_strs;
    eString physical_file;
    int index,offset,reg_num;
    Reg_descr* rd;
    char* virt_file_name;
    Reg_file vrf;
    eString reg_file_str, reg_type;
    switch (macro) {
      case INT_PARAM_1: case INT_PARAM_2: case INT_PARAM_3: case INT_PARAM_4:
      case INT_RETURN: case FLT_RETURN: case DBL_RETURN:
      case FLT_PARAM_1: case FLT_PARAM_2: case FLT_PARAM_3: case FLT_PARAM_4:
      case DBL_PARAM_1: case DBL_PARAM_2: case DBL_PARAM_3: case DBL_PARAM_4:
	break;
      default:
			S_punt("Unknown macro register during Vector generation!");
			return null_string;
    }

    if (!current_MDES->is_macro_present(macro)) {
      switch (macro) {
        case INT_RETURN: return "__PD_INT_RET_REG";
        case FLT_RETURN: return "__PD_FLT_RET_REG";
        case DBL_RETURN: return "__PD_DBL_RET_REG";
        default:
			S_punt("Unknown macro register during Vector generation!");
			return null_string;
       }
    }
    physical_files = current_MDES -> get_MDES_pf_for_macro (macro);
    if (physical_files.size() != 1) {
      S_punt ("No physical file or more than one physical file for macro.\n");
    }
    physical_file = physical_files.head();
    index = MDES_reg_index(physical_file);
    rd = current_MDES->get_MDES_reg_descr(index);
    virt_file_name = rd->get_vname();
    vrf = mdesname_to_vregfile(virt_file_name[0]);
    reg_file_str = reg_file_to_string (vrf);

    /*TODO: we might need to check and make sure that physical_file is static*/
    offset = stat_phys_offset.value(physical_file);
    phys_strs = rd->get_physical_regs(macro);
    reg_num = current_MDES->get_MDES_reg_number_for_pf(phys_strs.head());
    reg_num += offset;

    switch (macro) {
      case INT_PARAM_1: case INT_PARAM_2: case INT_PARAM_3: case INT_PARAM_4:
      case INT_RETURN:
	reg_type = "gpr";
	break;
      case FLT_PARAM_1: case FLT_PARAM_2: case FLT_PARAM_3: case FLT_PARAM_4:
      case FLT_RETURN:
	reg_type = "fpr_S";
	break;
      case DBL_PARAM_1: case DBL_PARAM_2: case DBL_PARAM_3: case DBL_PARAM_4:
      case DBL_RETURN:
	reg_type = "fpr_D";
	break;
      default:
	S_punt("Unknown macro register during Vector generation!");
	return (char*) null_string;
    }

    return "CAST(" | reg_file_str | ")[" | int_to_str(reg_num) | "].reg." | reg_type;
  }
}

eString get_macro_file (Macro_name macro) {
  if(S_virtual_regs){
    return "&__PD_macro_file";
  } else {
    if (!current_MDES->is_macro_present(macro))
      return "&__PD_macro_file";
    Hash_set<eString> physical_files;
    Hash_set<eString> phys_strs;
    eString physical_file;
    int index;
    Reg_descr* rd;
    char* virt_file_name;
    Reg_file vrf;
    eString reg_file_str;

    physical_files = current_MDES -> get_MDES_pf_for_macro (macro);
    physical_file = physical_files.head();
    index = MDES_reg_index(physical_file);
    rd = current_MDES->get_MDES_reg_descr(index);
    virt_file_name = rd->get_vname();
    vrf = mdesname_to_vregfile(virt_file_name[0]);
    reg_file_str = reg_file_to_string (vrf);

    return reg_file_str;
  }
}

int get_macro_reg_num (Macro_name macro) {
  if(S_virtual_regs){
    return macro_to_pd (macro);
  } else {
    if (!current_MDES->is_macro_present(macro))
      return macro_to_pd (macro);
    Hash_set<eString> physical_files;
    Hash_set<eString> phys_strs;
    eString physical_file;
    int offset,reg_num;
    int index;
    Reg_descr* rd;
    eString reg_file_str, reg_type;
    
    physical_files = current_MDES -> get_MDES_pf_for_macro (macro);
    physical_file = physical_files.head();
    index = MDES_reg_index(physical_file);
    rd = current_MDES->get_MDES_reg_descr(index);

    offset = stat_phys_offset.value(physical_file);
    phys_strs = rd->get_physical_regs(macro);
    reg_num = current_MDES->get_MDES_reg_number_for_pf(phys_strs.head());
    reg_num += offset;
    return reg_num;
  }
}
