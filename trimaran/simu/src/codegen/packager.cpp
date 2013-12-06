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
// File:   packager.cpp
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


#include "simu.h"
#include "streamBuffer.h"
#include "PD_globals.h"


// name of the global function which initializes all benchmark data
extern const char* SIMU_GLOBAL_DATA_INIT_FUNCTION;

// list of wrapper candidates
extern Hash_map<eString, int> wrappers;

// list of all functions with their parameter info
extern Hash_map<eString, hc_aggregate*> prototypes_table;

// simulator parameters
extern int S_nual_equals;
extern int S_virtual_regs;
extern int S_pipeline_stages;
extern int S_bytes_per_op;
extern int S_performance_monitoring;
extern int S_binary_trace_format;
extern int S_control_flow_trace;
extern int S_per_op_value_trace;
extern char *S_trace_function;
extern int S_address_trace;
extern int S_dynamic_stats;
extern int S_remap_optables;
extern int S_do_mem_simulation;
extern int S_stack_growth;
extern int S_stack_manipulated_in_elcor;
extern int S_return_value_thru_stack;
extern int S_return_value_offset;

// function declarations
void gen_wrappers(ofstream &outf, const Host_Info&);
static void write_wrapper(streamBuffer &, hc_aggregate&, const Host_Info&, const int op_list_size);

static char* print_cast(const hc_param& param);
char* promote_param(const hc_param& params);

// extern function declarations
extern bool  pass_SU_by_value(hc_param& p);
extern eString print(Macro_name macro);
extern char* typecast(hc_param&);
extern void  print_KRC_vararg_function_dec(streamBuffer&, hc_aggregate&);
extern void  print_function_declaration(streamBuffer&, hc_aggregate&);


// Write out wrapper functions for all function declarations
// encountered in the current file
void gen_wrappers(ofstream &outf, const Host_Info& host_info)
{
  streamBuffer out(outf);
  S_log("Writing Wrappers...");
  
  // write the header part
  out << endl << endl
      << "/**********************************************************/" << endl
      << "/******************** Wrapper Functions *******************/" << endl
      << "/**********************************************************/" << endl
      << endl << endl;

  for (Hash_map_iterator<eString, int> iter(wrappers); iter!=0 ; iter++) {
    eString e = (*iter).first;
    hc_aggregate& func = *prototypes_table.value(e);
    
    write_wrapper(out, func, host_info, (*iter).second);
  }

  out.commit();
}


static void gen_restore_code(streamBuffer &out_defs)
{
  out_defs 
   << STAB << "if (__restore) {" << endl;
  out_defs
    << STAB << STAB << "__PD_parameters  = __tmp_parms;" << endl
    << STAB << STAB << "__PD_statistics  = __tmp_stats;" << endl
    << STAB << STAB << "__PD_cur_op_list = __tmp_ops;"   << endl;
  // restore caller's register settings
  if (S_virtual_regs) {
    out_defs 
	<< STAB << STAB << "__PD_restore_regfile(__tmp_reginfo, __saved_registers);" << endl
	<< STAB << "}"  << endl
	<< STAB << "else if (__saved_registers) {"    << endl
	<< STAB << STAB << "free(__saved_registers);" << endl
	<< STAB << STAB << "__saved_registers = 0;"   << endl
	<< STAB << "}"  << endl
	<< endl;
  }
  else {
    out_defs 
	<< STAB << "}"  << endl;
  }
}

// packaging routine
static void write_wrapper(streamBuffer &out_defs, hc_aggregate& func, 
				  const Host_Info& host_info, const int op_list_size)
{
  char *name = (char *) func.name;
  int  last_param_count = -1;

  // old style vararg function declaration
  if (func.isVararg && func.isKANDRC) {    
    print_KRC_vararg_function_dec(out_defs, func); 
    out_defs << "; ...";
  }
  else {
    print_function_declaration(out_defs, func);
  }
  // begin function body
  out_defs 
    << endl << "{" << endl;

  // generate __PD_params mask
  // note that in the future each function and subsequently each block
  // will have different settings
  int parmsmask = 0;
  parmsmask |= S_binary_trace_format ? __PD_BINTRACE : 0;
  parmsmask |= S_address_trace       ? __PD_MEMTRACE : 0;
  parmsmask |= S_control_flow_trace  ? __PD_CFGTRACE : 0;
  parmsmask |= S_per_op_value_trace  ? __PD_PER_OP_VALUE_TRACE : 0;
  parmsmask |= S_dynamic_stats       ? __PD_DYNSTATS : 0;
  parmsmask |= S_virtual_regs        ? __PD_VIRTREGS : 0;
  parmsmask |= S_do_mem_simulation   ? __PD_MEMSIM   : 0;
  parmsmask |= S_stack_manipulated_in_elcor ? __PD_STACK_MANIPULATED_IN_ELCOR   : 0;
  parmsmask |= (S_stack_growth == MEM_HIGH_TO_LOW) ? __PD_MEM_HIGH_TO_LOW : __PD_MEM_LOW_TO_HIGH;

  if ((func.params[0].type == STRUCT) || (func.params[0].type == UNION)) {
    out_defs << STAB << typecast(func.params[0]) << "retstruct;" << endl;
  }
  else if(func.params[0].type == LONGLONG) {
    out_defs << STAB << typecast(func.params[0]) << "retlonglong;" << endl;
  }
  out_defs << STAB << "union longlongparam { long long x; struct { long lo, hi; } parts; } llongparam;" << endl;
  // write out function specific information 
  out_defs
    << STAB << "static bool __initialized = __PD_false;" << endl
    << STAB << "static bool __restore;" << endl
    << endl;
  if (func.count > 1 || ((func.params[0].type != VOID) && S_return_value_thru_stack) ||
      (func.params[0].type == STRUCT) || (func.params[0].type == UNION)) {
    out_defs << STAB << "bool __native_call = !__PD_is_simulator_call(&" << func.name << ");" << endl;
    out_defs << STAB << "long __sp_align = 0;" << endl;
    if (S_stack_manipulated_in_elcor)
      out_defs << STAB << "long __tmp_sp = 0;" << endl;
    else
      out_defs << STAB << "long __tmp_opsp = 0;" << endl;
    out_defs << endl;
  }
  out_defs
    << STAB << "__PD_OP*    __tmp_ops   = __PD_cur_op_list;" << endl
    << STAB << "__PD_stats* __tmp_stats = __PD_statistics;"  << endl
    << STAB << "__PD_parms* __tmp_parms = __PD_parameters;"  << endl
    << STAB << "__PD_parms  __new_parms = {" 
		<< parmsmask      << ", " << S_pipeline_stages << ", " 
		<< S_bytes_per_op << ", " << op_list_size 
		<< "};" << endl
    << endl;

  if (S_virtual_regs) {
    // store a pointer to the current register file pointer 
    // then create a new register file for the current function context
    out_defs 
	<< STAB << "__PD_reg_file_info* __tmp_reginfo = __PD_reginfo;" << endl
	<< STAB << "__PD_reg* __saved_registers = 0;" << endl
	<< endl
	<< STAB << "__PD_create_virtual_regfile(__PD_reginfo_" << name << ", &__saved_registers);" << endl
	<< endl;
  }
  out_defs 
    << STAB << "__PD_cur_op_list = _" << name << "__pd_ops;" << endl
    << STAB << "__PD_parameters  = &__new_parms;" << endl
    << endl;
  
  // checks if the simulator has been initialized

  out_defs 
    << STAB << "if (!__initialized) {" << endl
    << STAB << STAB << SIMU_GLOBAL_DATA_INIT_FUNCTION << ";" << endl;
  if (!S_virtual_regs) {
    out_defs 
	<< STAB << STAB << "__PD_create_physical_regfile(__PD_physical_reginfo);" << endl;

  }
  out_defs
    << STAB << STAB << "__PD_initialize();" << endl;
  if (S_dynamic_stats) {
    out_defs 
	<< STAB << STAB << "__PD_chain(__PD_stats_" << name << ");" << endl;
  }
  out_defs 
    << STAB << STAB << "__initialized = __PD_true;" << endl
    << STAB << "}" << endl << endl;
  
  // analyzing the parameters and set the stack pointers if necessary
  if (func.count > 1 || ((func.params[0].type != VOID) && S_return_value_thru_stack) ||
      (func.params[0].type == STRUCT) || (func.params[0].type == UNION)) {
    out_defs 
	<< STAB << "if (__native_call) {" << endl;

    // if native_call  is true, then  allocate a fake  activation record then
    // execute current function the size of the fake frame is  the equivalent
    // to the max_parameter_offset + 32 (for the frame marker)
    // note that the first four  words are passed through registers but space
    // is reserved on the stack regardless (thus the first max check)
    int extra = max(16, abs(func.params[func.params.dim()-1].offset));
    // check if there is a structure passed by value
    for (int i = 1; i < func.params.dim(); i++) {
      if (func.params[i].has_soffset) {
	  extra = max(abs(func.params[i].soffset), extra);
      }
    }
    assert (!(extra % 4));

    // force the stack pointer to be 16-byte aligned
    out_defs 
      << STAB << STAB << "__sp_align = " << extra << ";" << endl
      << STAB << STAB << "if (__sp_align & 15) {" << endl
      << STAB << STAB << STAB << "__sp_align += 16 - (" << extra << " & 15);" << endl 
      << STAB << STAB << "}" << endl;
    if (S_stack_manipulated_in_elcor)
      out_defs 
        << STAB << STAB << "__tmp_sp = __PD_allocate_native_stack(16, __sp_align);" << endl;
    else
      out_defs 
        << STAB << STAB << "__tmp_opsp = __PD_allocate_native_stack(16, __sp_align);" << endl;
    out_defs << endl;
  }

  // If returning a struct, store the address of the struct in the fake stack
  if ((func.params[0].type == STRUCT) || (func.params[0].type == UNION)) {
    if (S_return_value_thru_stack) {
      if (S_stack_manipulated_in_elcor) {
        out_defs << STAB << STAB << "(*(( long *) (__PD_SP_REG + " << S_return_value_offset << "))) = "
                 << "(" << typecast(func.params[0]) << "*) &retstruct;" << endl;
      } else {
        out_defs << STAB << STAB << "(*(( long *) (__tmp_opsp.reg.cr + " << S_return_value_offset << "))) = "
                 << "(" << typecast(func.params[0]) << "*) &retstruct;" << endl;
      }
    } else {
        out_defs << STAB << STAB << print(INT_RETURN) << " = "
                  "(" << typecast(func.params[0]) << "*) &retstruct;" << endl;
    }
  }
 
  for (int index = 1; index < func.params.dim(); index++) {
    hc_param& param = func.params[index];

    // ignore the vararg parameter
    if (param.type == ELLIPSIS) {
      last_param_count = index - 1;
      assert (index == func.params.dim()-1);
      break;
    }

    if (pass_SU_by_value(param)) {
      if (param.macro_reg) {
	  // when a structure is passed by  value, and it is being passed through a
	  // register, a pointer to the actual structure is passed in the register,
	  // and the  actual structure resides  somewhere in OP, and  the structure
	  // offset must be present - otherwise, something is wrong
	  assert (param.has_soffset);
	  // set the macro registers to contain the address of the location
	  // where the actual structure will be copied
          if (S_stack_manipulated_in_elcor) {
	    out_defs 
	      << STAB << STAB << print(param.macro_name) 
	      << " = __PD_SP_REG + " <<  param.soffset << ";" << endl
	      // copy the struct to the fake stack
	      << STAB << STAB << "*((" << typecast(param) << "*) ("
	      << print(param.macro_name) << ")) = p" << index << ";" << endl
	      << endl;
          } else {
	    out_defs 
	      << STAB << STAB << print(param.macro_name) 
	      << " = __tmp_opsp + " <<  param.soffset << ";" << endl
	      // copy the struct to the fake stack
	      << STAB << STAB << "*((" << typecast(param) << "*) ("
	      << print(param.macro_name) << ")) = p" << index << ";" << endl;
          }
      } // structure passed in register
      else {
	  if (param.has_soffset) {
	    // this is an indirect structure passed by value, set offset to 
	    // contain the address of the location where the actual 
	    // structure will be copied
            if (S_stack_manipulated_in_elcor) {
	      out_defs 
		<< STAB << STAB << "*((int *) (__PD_SP_REG + " 
		<< param.offset << ")) = __PD_SP_REG + " <<  param.soffset << ";" << endl
		// copy the struct to the fake stack
		<< STAB << STAB << "*((" << typecast(param) << "*) ("
		<< "__PD_SP_REG + " << param.soffset << ")) = p" << index << ";"  << endl;
            } else {
	      out_defs 
		<< STAB << STAB << "*((int *) (__tmp_opsp + " 
		<< param.offset << ")) = __tmp_opsp + " <<  param.soffset << ";" << endl
		// copy the struct to the fake stack
		<< STAB << STAB << "*((" << typecast(param) << "*) ("
		<< "__tmp_opsp + " << param.soffset << ")) = p" << index << ";"  << endl;
            }
	  } // indirect structure pass by value
	  else {
            if (S_stack_manipulated_in_elcor) {
	      out_defs 
		<< STAB << STAB << "*((" << typecast(param) << "*) ("
		<< "__PD_SP_REG + " << param.offset << ")) = p" << index << ";"  << endl;
            } else {
	      out_defs 
		<< STAB << STAB << "*((" << typecast(param) << "*) ("
		<< "__tmp_opsp + " << param.offset << ")) = p" << index << ";"  << endl;
            }
	  }
      } // structure passed in memory
    } // special structure handling 
    else if (param.macro_reg) {

      if(param.type == LONGLONG) {
        if(param.desc != DATA_POINTER) {
          out_defs
            << STAB << STAB << "llongparam.x = "
            << print_cast(param) << " p" << index << ";" << endl;
          out_defs 
            << STAB << STAB << print(param.macro_name) << " = " 
            << "(long)" << " llongparam.parts.lo" << ";" << endl;
          out_defs 
            << STAB << STAB << print(param.second_macro_name) << " = " 
            << "(long)" << " llongparam.parts.hi" << ";" << endl;
        }
      }
      else {
          out_defs 
            << STAB << STAB << print(param.macro_name) << " = " 
            << print_cast(param) << " p" << index << ";" << endl;
        }
    }
    else {
	eString type = promote_param(param);
        if (S_stack_manipulated_in_elcor) {
	  out_defs 
	    << STAB << STAB << "*((" << type << "*) "
	    << "(__PD_SP_REG + " << param.offset << ")) = ((" 
	    << type << ") p" << index << ");" << endl;
        } else {
	  out_defs 
	    << STAB << STAB << "*((" << type << "*) "
	    << "(__tmp_opsp + " << param.offset << ")) = ((" 
	    << type << ") p" << index << ");" << endl;
        }
    }
  }
  
  // end outgoing parameter stack initialization
  if (func.count > 1 || ((func.params[0].type != VOID) && S_return_value_thru_stack) ||
      (func.params[0].type == STRUCT) || (func.params[0].type == UNION))
    out_defs << STAB << "}" << endl << endl;
  
  // special handle for vararg functions
  if (func.isVararg) {
    switch (host_info.platform) {
    case SUNOS: 
    case HP: {
      if (func.isKANDRC) {
	  S_warn("varargs may not be accurately simulated!");

	  if (func.count != 2) {
	    S_punt("[%s] expected to specify a %s and exactly %s!", 
		     name, "return type", "one parameter");
	  }
      }
      else {
	  // sanity check
	  assert (last_param_count == func.count - 2);
      }
    } break;
    case X86LIN:
    case X86WIN: {
      if (func.isKANDRC) {
	  S_warn("varargs may not be accurately simulated!");
	
	  if (func.count != 2) {
	    S_punt("[%s] expected to specify a %s and exactly %s!", 
		     name, "return type", "one parameter");
	  }
      }
      else {
	  // sanity check
	  assert (last_param_count == func.count - 2);
      }
    } break;
    default: S_punt("Unsupported Host Platform!");
    }
  }

  // write out the call to simulator
  if (S_remap_optables) {
    out_defs 
	<< STAB << "__restore = __PD_simulate(&_" << name << "__pd_ops);" << endl
	<< endl;
  }
  else {
    out_defs 
	<< STAB << "__restore = __PD_simulate(_" << name << "__pd_ops);" << endl
	<< endl;
  }

  // lastly Analyze the return type
  switch (func.params[0].desc) {
  case DATA_POINTER:
  case CODE_POINTER:
    if (S_return_value_thru_stack)
      out_defs << STAB << print(INT_RETURN) << " = (" << typecast(func.params[0]) << ")(*( long *)"
               << "(__PD_SP_REG + " << S_return_value_offset << "));" << endl;
    // restore the original stack ptr
    if (func.count > 1 || ((func.params[0].type != VOID) && S_return_value_thru_stack) ||
      (func.params[0].type == STRUCT) || (func.params[0].type == UNION)) {
      out_defs 
	<< STAB <<  "if (__native_call) {" << endl
	<< STAB <<  STAB << "__PD_deallocate_native_stack(16, __sp_align);" << endl
	<< STAB <<  "}" << endl
	<< endl;
    }
    gen_restore_code(out_defs);
    out_defs 
	<< STAB << "return (" << typecast(func.params[0]) << ")" << endl
	<< STAB << STAB << print(INT_RETURN) << ";" << endl;
    break;
    
  default:
    switch (func.params[0].type) {
    case STRUCT:
    case UNION:
      // again force return by value using the pattern *((struct *) )
      if (S_return_value_thru_stack) {
        if (S_stack_manipulated_in_elcor)
          out_defs 
	    << STAB << "retstruct = *((" << typecast(func.params[0]) << "*)" 
            << "(*(( long *) (__PD_SP_REG + " << S_return_value_offset << "))));" << endl;
        else 
          out_defs 
	    << STAB << "retstruct = *((" << typecast(func.params[0]) << "*)" 
            << "(*(( long *) (__tmp_opsp + " << S_return_value_offset << "))));" << endl;
      } else {
        out_defs 
	  << STAB << "retstruct = *((" << typecast(func.params[0]) << "*)" 
	  << endl << STAB << STAB << print(INT_RETURN) << ");" << endl;
      }
      // restore the original stack ptr
      if (func.count > 1 || S_return_value_thru_stack) {
        out_defs 
	  << STAB << "if (__native_call) {" << endl
	  << STAB << STAB << "__PD_deallocate_native_stack(16, __sp_align);" << endl
	  << STAB << "}" << endl
	  << endl;
      }
      gen_restore_code(out_defs);
      out_defs << STAB << "return retstruct; " << endl;
      break;

    default:
      if (func.params[0].type != VOID) {
        if (S_return_value_thru_stack) {
          switch(func.params[0].type) {
            case CHAR:
            case SHORT:
            case LONG:
            case INT:
                out_defs << endl << STAB << print(INT_RETURN) <<"= (" << typecast(func.params[0]) << ")(*( long *)"
                         << "(__PD_SP_REG + " << S_return_value_offset << "));" << endl;
              break;
            case LONGLONG:
                out_defs << endl << STAB << "retlonglong " <<"= (" << typecast(func.params[0]) << ")(*( long long*)"
                         << "(__PD_SP_REG + " << S_return_value_offset << "));" << endl;
              break;
            case FLOAT:
                out_defs << endl << STAB << print(FLT_RETURN) <<"= (" << typecast(func.params[0])
                         << ")(*( float *)" << "(__PD_SP_REG + " << S_return_value_offset << "));" << endl;
              break;
            case DOUBLE:
                out_defs << endl << STAB << print(DBL_RETURN) << "= (" << typecast(func.params[0])
                         << ")(*( double *)" << "(__PD_SP_REG + " << S_return_value_offset << "));" << endl;
              break;
            default:
              break;
          }
        }
        if(func.params[0].type == LONGLONG) {
                out_defs << endl << STAB << "retlonglong " <<"= (" << typecast(func.params[0]) << ")(*( long long*)"
                         << "(__PD_SP_REG + " << S_return_value_offset << "));" << endl;
        }
        // restore the original stack ptr
        if (func.count > 1 || S_return_value_thru_stack) {
          out_defs 
	    << STAB << "if (__native_call) {" << endl
	    << STAB << STAB << "__PD_deallocate_native_stack(16, __sp_align);" << endl
	    << STAB << "}" << endl
	    << endl;
        }
        gen_restore_code(out_defs);
        out_defs << STAB << "return (" << typecast(func.params[0])  << ")"
                 << endl;
        switch(func.params[0].type) {
        case CHAR:
        case SHORT:
        case LONG:
        case INT:
          out_defs << STAB << STAB << print(INT_RETURN) << ";" << endl;
          break;
        case LONGLONG:
          out_defs << STAB << STAB << "retlonglong" << ";" << endl;
          break;
        case FLOAT:
          out_defs << STAB << STAB << print(FLT_RETURN) << ";" << endl;
          break;
        case DOUBLE:
          out_defs << STAB << STAB << print(DBL_RETURN) << ";" << endl;
          break;
        default:
          break;
        }
      } else {
        // added by hongtaoz: 5/25/2004
        // When the return type is VOID, the orginal stack ptr still need to
        // be restored after a native call
        // restore the original stack ptr
        if (func.count > 1) {
          out_defs 
	    << STAB << "if (__native_call) {" << endl
	    << STAB << STAB << "__PD_deallocate_native_stack(16, __sp_align);" << endl
	    << STAB << "}" << endl
	    << endl;
        }
        gen_restore_code(out_defs);
      }
    }
  }
  
  out_defs << "}" << endl << endl;
}


static char* print_cast(const hc_param& param)
{
  switch(param.desc) {
  case CODE_POINTER:  case DATA_POINTER:  return "(long)";
  default:
    switch(param.type) {
    case CHAR: case SHORT: case LONG: case INT:  return "(long)";
    case LONGLONG: return "(long long)";
    case FLOAT:  return "(float)";
    case DOUBLE: return "(double)";
    case STRUCT: case UNION: return "(long)&";
    default: return "";
    }
  }
}


char* promote_param(const hc_param& param)
{
  switch(param.desc) {
  case CODE_POINTER: case DATA_POINTER: return "long";
  default:
    switch(param.type) {
    case CHAR: case SHORT: case LONG: case INT: return "long";
    case LONGLONG: return "long long";
    case FLOAT:  return "float";
    case DOUBLE: return "double";

    case STRUCT: case UNION:
      // structure and unions passed by value are handled
      // by a special case in the wrapper writer
      assert(0);
    default:
      return "";
    }
  }
}
