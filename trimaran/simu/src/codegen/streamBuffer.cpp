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
// File:   streamBuffer.cpp
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


#include "simu.h"
#include "streamBuffer.h"
#include "PD_reg.h"
#include "ir_operand.h"
#include "IR_symbol.h"

const char* STAB = "   ";

// extern function prototypes
extern void sprint_C_prototype(char *output, char *name, char *info);
extern void sprint_KRC_prototype(char* output, char *name, char *info);


// function declarations
void print_function_declaration(streamBuffer& out, hc_aggregate& aggr);
void print_KRC_vararg_function_dec(streamBuffer&, hc_aggregate&);



// constructor
streamBuffer::streamBuffer(void)
{
  out = NULL;
}


// constructor
streamBuffer::streamBuffer(ofstream& out_file)
{
  out = &out_file;
}


// concatenate two buffers
streamBuffer& operator<<(streamBuffer& t, streamBuffer& s)
{
  if (s.buffer != NULL) {
    t.buffer += s.buffer;
  }
  return t;
}


streamBuffer& streamBuffer::operator<<(int i)
{
  char str[50];
  sprintf(str, "%d", i);
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(unsigned int i)
{
  char str[50];
  sprintf(str, "%u", i);
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(long l)
{
  char str[50];
  sprintf(str, "%ld", l);
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(unsigned long long i)
{
  char str[50];
  sprintf(str, "%llu", i);
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(char c)
{
  char str[5];
  str[0] = c;
  str[1] = 0;
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(const char* str)
{
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(const eString& estr)
{
  buffer += ((char *) ((eString&) estr));
  return *this;
}


streamBuffer& streamBuffer::operator<<(float f)
{
  char str[50];
  // match the precision in the elcor IR_oustream
  sprintf(str, "%.8e", f);
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(double d)
{
  char str[50];
  // match the precision in the elcor IR_oustream
  sprintf(str, "%.16e", d);
  buffer += str;
  return *this;
}


streamBuffer& streamBuffer::operator<<(const void *ptr)
{
  char str[50];
  sprintf(str, "%p", ptr);
  buffer += str;
  return *this;
}


char* reg_file_to_string(const Reg_file& reg_file)
{
  switch (reg_file) {
  case GPR: return "__PD_GPR_FILE";
  case FPR: return "__PD_FPR_FILE";
  case PR:  return "__PD_PR_FILE"; 
  case BTR: return "__PD_BTR_FILE";
  case CR:  return "__PD_CR_FILE";    
  /* RMR { adding support for vector file type */
  case VIR: return "__PD_VIR_FILE";
  case VFR: return "__PD_VFR_FILE";
  case VMR: return "__PD_VMR_FILE";
  /* } RMR */
  default:  return "__PD_MACRO_FILE";
  }
}


int macro_to_pd(const Macro_name& name)
{
  switch (name) {
  case RETURN_ADDR: return __PD_RET_ADDR;     
  case INT_RETURN:  return __PD_INT_RET;
  case FLT_RETURN:  return __PD_FLT_RET;
  case DBL_RETURN:  return __PD_DBL_RET;

  case INT_PARAM_1: return __PD_INT_P1; 
  case INT_PARAM_2: return __PD_INT_P2; 
  case INT_PARAM_3: return __PD_INT_P3; 
  case INT_PARAM_4: return __PD_INT_P4; 

  case FLT_PARAM_1: return __PD_FLT_P1; 
  case FLT_PARAM_2: return __PD_FLT_P2; 
  case FLT_PARAM_3: return __PD_FLT_P3; 
  case FLT_PARAM_4: return __PD_FLT_P4; 

  case DBL_PARAM_1: return __PD_DBL_P1; 
  case DBL_PARAM_2: return __PD_DBL_P2; 
  case DBL_PARAM_3: return __PD_DBL_P3; 
  case DBL_PARAM_4: return __PD_DBL_P4; 

    // SP should be interpreted as RS
    // hongtaoz: new stack model SP is SP, RGS is RS
  //case SP_REG:	return __PD_RS; 
  case SP_REG:	return __PD_SP; 
  case RGS_REG:	return __PD_RS; 
  case FP_REG:	return __PD_FP; 
  case IP_REG:	return __PD_IP; 
  case OP_REG:	return __PD_OPS;
  case LV_REG:	return __PD_LV; 

  case LC_REG:	return __PD_LC; 
  case ESC_REG:	return __PD_ESC;
  case RRB:     return __PD_RRB;

  case PV_0: return __PD_PV_0;
  case PV_1: return __PD_PV_1;
  case PV_2: return __PD_PV_2;
  case PV_3: return __PD_PV_3;
  case PV_4: return __PD_PV_4;
  case PV_5: return __PD_PV_5;
  case PV_6: return __PD_PV_6;
  case PV_7: return __PD_PV_7;

  case SPILL_TEMPREG: return __PD_TEMP_REG;
  case UNDEFINED:     return __PD_BLACK_HOLE;

  case INT_ZERO: return __PD_INT_ZERO;
  case FLT_ZERO: return __PD_FLT_ZERO;
  case DBL_ZERO: return __PD_DBL_ZERO;
  case FLT_ONE:	 return __PD_FLT_ONE; 
  case DBL_ONE:	 return __PD_DBL_ONE; 

  case PRED_FALSE: return __PD_PRED_F;
  case PRED_TRUE:  return __PD_PRED_T;

  /* RMR { add default all true vector mask */
  case VEC_MASK_TRUE: return __PD_VEC_MASK_T;
  /* } RMR */

  /* RMR { scratch vector register for operand transport */
  case VEC_SCRATCH_PAD: return __PD_VEC_SCRATCH_PAD;
  /* } RMR */

  //hongtaoz: new macro reg
  //HACK: it's ok to return black hole here because the only ops
  //  that use these macros are CLEAR_ALL_PRED or SET_ALL_PRED etc,
  //  in simu, these ops don't check the operands at all.
  case ALL_PRED:        return __PD_BLACK_HOLE;
  case ALL_ROT_PRED:    return __PD_BLACK_HOLE;
  case ALL_STATIC_PRED: return __PD_BLACK_HOLE;

  case ICMOVE_REG: return __PD_ICMOVE;
									       
  default: 
    S_warn("Unknown macro register!");
    //hongtaoz: assert here to help debug
    assert(0);
    return __PD_BLACK_HOLE;
  }
}



void streamBuffer::commit()
{
  char* b = (char*) buffer;
  if (b != NULL) {
    if (out == NULL) {
      S_punt("Attempting to dump stream to a null file pointer!");
    }
    
    (*out) << b;
  }

  out->flush();
  buffer.reset();
}


void streamBuffer::clear()
{
  buffer.reset();
}


void print_function_declaration(streamBuffer& out, hc_aggregate& aggr)
{
  char  out_buf[MAX_BUF_SIZE];
  char* call_info = aggr.call_info.strdup();

  sprint_C_prototype(out_buf, (char*)aggr.name, call_info);
  delete call_info;
  out << out_buf;
}


void print_KRC_vararg_function_dec(streamBuffer& out, hc_aggregate& func)
{
  char  out_buf[MAX_BUF_SIZE];
  char* call_info = func.call_info.strdup();

  sprint_KRC_prototype(out_buf, (char*)func.name, call_info);
  delete call_info;
  out << out_buf;
}
