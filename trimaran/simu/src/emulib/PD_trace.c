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

/******************************************************************************
 * File:    PD_trace.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: control flow and memory tracing routines
 *****************************************************************************/


#include "PD_trace.h"
#include <stdlib.h>
#include <stdio.h>
#include "PD_error.h"
#include "PD_stats.h"
#include "PD_parms.h"
#include "PD_kernel.h"
#include "PD_sl.h"


void*  __PD_trace_file          = NULL;
char*  __PD_trace_file_name     = "PD_TRACE";
bool   __PD_binary_trace_format = __PD_false;
ushort __PD_edge                = 0;


inline static char  width_to_char(__PD_width w);
inline static char* cache_to_string(__PD_cache c);
inline static char* caches_to_string(__PD_caches sd);


/* macro for writing even packet to file in binary format */
#define binwrite(packet, type, size) \
          fwrite(&packet, sizeof(type), size, __PD_trace_file)


/* trace function entry */
void __PD_trace_fn_entry(const char* fname)
{
  if (__PD_binary_trace_format) {
    static ushort packet = __PD_TRACE_FN_ENTRY;
    binwrite(packet, ushort, 1);
    /* append function name */
    fprintf(__PD_trace_file, "<%s>", fname);
  }
  else {
    fprintf(__PD_trace_file, "p %s\n", fname);
  }
}


/* trace function return */
void __PD_trace_rts()
{
  if (__PD_binary_trace_format) {
    static ushort packet = __PD_TRACE_RTS;
    binwrite(packet, ushort, 1);
  }
  else {
    fprintf(__PD_trace_file, "^ %s\n", __PD_function_name);
  }
}


/* trace block entry */
void __PD_trace_cb_entry(const __PD_OP* op)
{
  /* __PD_edge holds the id of the last exit edge taken */
  if (__PD_binary_trace_format) {
    ulong packet[3]  = {__PD_TRACE_CB_ENTRY, OP_ID(op), __PD_edge};
    binwrite(packet[0], ulong, 3);
  }
  else {
    fprintf(__PD_trace_file, "c %lu %d\n", OP_ID(op), __PD_edge);
  }
}


/* trace longjmp */
void __PD_trace_longjmp(const char* fname, ulong pc)
{
  if (__PD_binary_trace_format) {
    static ushort packet = __PD_TRACE_LONGJMP;
    binwrite(packet, ushort, 1);
    binwrite(pc, ulong, 1);
    /* append target function name */
    fprintf(__PD_trace_file, "<%s>", fname);
  }
  else {
    fprintf(__PD_trace_file, "j %s %lu (%d)\n", fname, pc, __PD_longjmp_depth);
  }
}

void __PD_trace_operand(__PD_reg opd, char type) {
  switch (type) {
    case 'P':
      if (opd.reg.pr)
        fprintf((FILE *) __PD_trace_file, "T ");
      else
        fprintf((FILE *) __PD_trace_file, "F ");
      break;
    case 'B':
      fprintf((FILE *) __PD_trace_file, "0x%lx", opd.reg.btr.addr);
      if (opd.reg.btr.pred)
        fprintf((FILE *) __PD_trace_file, "(T) ");
      else
        fprintf((FILE *) __PD_trace_file, "(F) ");
      break;
    case 'C':
      fprintf((FILE *) __PD_trace_file, "0x%lx(%lu) ", opd.reg.cr, opd.reg.cr);
      break;
    case 'I':
      fprintf((FILE *) __PD_trace_file, "0x%lx(%ld) ", opd.reg.gpr, opd.reg.gpr);
      break;
    case 'H':
      fprintf((FILE *) __PD_trace_file, "0x%lx ", opd.reg.fpr_W.lo);
      break;
    case 'U':
      fprintf((FILE *) __PD_trace_file, "0x%lx ", opd.reg.fpr_W.hi);
      break;
    case 'D':
      fprintf((FILE *) __PD_trace_file, "%e ", opd.reg.fpr_D);
      break;
    case 'F':
      fprintf((FILE *) __PD_trace_file, "%e ", opd.reg.fpr_S);
      break;
  }
}

/* trace op */
void __PD_trace_op(__PD_OP* op, char name[], char src[], char dest[], __PD_reg darray[])
{
  if (__PD_binary_trace_format) {
    ulong packet[2] = {__PD_TRACE_OP_ENTRY, OP_ID(op)};
    binwrite(packet[0], ulong, 2);
  }
  else {
    // fprintf(__PD_trace_file, "o %lu\n", OP_ID(op));
    int i;
    fprintf((FILE *) __PD_trace_file, "%-12llu %lu ",
            __PD_global_clock, op->rebel_id);
    /* trace destinations from the provided array */
    for (i=0; *dest; dest++, i++)
      __PD_trace_operand(darray[i], *dest);
    /* write opcode name */
    fprintf((FILE *) __PD_trace_file, "<- %s ", name);
    /* trace sources - literals are treated specially */
    for (i=0; *src; src++, i++) {
      if (*src == 'L')
        fprintf((FILE *) __PD_trace_file, "0x%lx(%ld) ", LIT(op->src[i]), LIT(op->src[i]));
      else __PD_trace_operand(SREG(op->src[i]), *src);
    }
    fprintf((FILE *) __PD_trace_file, "\n");
    fflush((FILE *) __PD_trace_file);
  }
}


/* trace op nullification */
void __PD_trace_op_null(const __PD_OP* op)
{
  if (__PD_binary_trace_format) {
    ulong packet[2] = {__PD_TRACE_OP_NULL, OP_ID(op)};
    binwrite(packet[0], ulong, 2);	
  }
  else {
    fprintf(__PD_trace_file, "! %lu\n", OP_ID(op));
  }
}


/* trace load op */
void __PD_trace_load(const __PD_OP* op, __PD_width w, __PD_cache s, __PD_cache d, __PD_addressing_type t)
{
  ulong address;
 
  if (OP_SRC0(op).vec)
   address =  VREG(OP_SRC0(op), 0).reg.gpr;
  else if (t == __PD_I)
    address    = SREG(OP_SRC0(op)).reg.gpr;
  else if (t == __PD_L)
    address    = LIT(OP_SRC0(op));
  else if (t == __PD_II)
    address    = SREG(OP_SRC0(op)).reg.gpr + SREG(OP_SRC1(op)).reg.gpr;
  else if (t == __PD_IL)
    address    = SREG(OP_SRC0(op)).reg.gpr + LIT(OP_SRC1(op));
  else if (t == __PD_LL)
    address    = LIT(OP_SRC0(op))+ LIT(OP_SRC1(op));
  
  if (__PD_binary_trace_format) {
    ulong packet[6]  = {__PD_TRACE_LOAD, w, s, d, OP_ID(op), LAT(OP_DEST0(op))};
    u64bitint info[2] = {address, __PD_global_clock};
    
    binwrite(packet[0], ulong, 6);
    binwrite(info[0], u64bitint, 2);
  }
  else {
    fprintf(__PD_trace_file, "L %c %s %s %lu %llu %d %s %lu\n", 
		width_to_char(w), cache_to_string(s), cache_to_string(d), 
		address, __PD_global_clock, LAT(OP_DEST0(op)),
		__PD_function_name, OP_ID(op));
  }
}


void __PD_trace_lds(const __PD_OP* op, __PD_width w, __PD_cache s, __PD_cache d, __PD_addressing_type t)
{
  ulong address ;
  
  if (OP_SRC0(op).vec)
   address =  VREG(OP_SRC0(op), 0).reg.gpr;
  else if (t == __PD_I)
    address    = SREG(OP_SRC0(op)).reg.gpr;
  else if (t == __PD_L)
    address    = LIT(OP_SRC0(op));
  else if (t == __PD_II)
    address    = SREG(OP_SRC0(op)).reg.gpr + SREG(OP_SRC1(op)).reg.gpr;
  else if (t == __PD_IL)
    address    = SREG(OP_SRC0(op)).reg.gpr + LIT(OP_SRC1(op));
  else if (t == __PD_LL)
    address    = LIT(OP_SRC0(op))+ LIT(OP_SRC1(op));
  
  if (__PD_binary_trace_format) {
    ulong packet[6]  = {__PD_TRACE_LDS, w, s, d, OP_ID(op), LAT(OP_DEST0(op))};
    u64bitint info[2] = {address, __PD_global_clock};
    
    binwrite(packet[0], ulong, 6);
    binwrite(info[0], u64bitint, 2);
  }
  else {
    fprintf(__PD_trace_file, "A %c %s %s %lu %llu %d %s %lu\n", 
		width_to_char(w), cache_to_string(s), cache_to_string(d), 
		address, __PD_global_clock, LAT(OP_DEST0(op)),
		__PD_function_name, OP_ID(op));
  }
}


void __PD_trace_ldv(const __PD_OP* op, __PD_width w, __PD_addressing_type t)
{
  ulong address ;
  
  if (OP_SRC0(op).vec)
   address =  VREG(OP_SRC0(op), 0).reg.gpr;
  else if (t == __PD_I)
    address    = SREG(OP_SRC0(op)).reg.gpr;
  else if (t == __PD_L)
    address    = LIT(OP_SRC0(op));
  else if (t == __PD_II)
    address    = SREG(OP_SRC0(op)).reg.gpr + SREG(OP_SRC1(op)).reg.gpr;
  else if (t == __PD_IL)
    address    = SREG(OP_SRC0(op)).reg.gpr + LIT(OP_SRC1(op));
  else if (t == __PD_LL)
    address    = LIT(OP_SRC0(op))+ LIT(OP_SRC1(op));
  
  
  if (__PD_binary_trace_format) {
    ulong packet[4]  = {__PD_TRACE_LDV, w, OP_ID(op), LAT(OP_DEST0(op))};
    u64bitint info[2] = {address, __PD_global_clock};
    
    binwrite(packet[0], ulong, 4);
    binwrite(info[0], u64bitint, 2);
  }
  else {
    fprintf(__PD_trace_file, "V %c %lu %llu %d %s %lu\n", 
		width_to_char(w), address, __PD_global_clock, LAT(OP_DEST0(op)),
		__PD_function_name, OP_ID(op));
  }
}


/* trace store op */
void __PD_trace_store(const __PD_OP* op, __PD_width w, __PD_cache d, __PD_addressing_type t)
{
  ulong address ;
  
  if (OP_SRC0(op).vec)
   address =  VREG(OP_SRC0(op), 0).reg.gpr;
  else if (t == __PD_I)
    address    = SREG(OP_SRC0(op)).reg.gpr;
  else if (t == __PD_L)
    address    = LIT(OP_SRC0(op));
  else if (t == __PD_II)
    address    = SREG(OP_SRC0(op)).reg.gpr + SREG(OP_SRC1(op)).reg.gpr;
  else if (t == __PD_IL)
    address    = SREG(OP_SRC0(op)).reg.gpr + LIT(OP_SRC1(op));
  else if (t == __PD_LL)
    address    = LIT(OP_SRC0(op))+ LIT(OP_SRC1(op));
  
  if (__PD_binary_trace_format) {
    ulong packet[4]  = {__PD_TRACE_STORE, w, d, OP_ID(op)};
    u64bitint info[2] = {address, __PD_global_clock};
    
    binwrite(packet[0], ulong, 4);
    binwrite(info[0], u64bitint, 2);
  }
  else {
    fprintf(__PD_trace_file, "S %c %s %lu %llu %s %lu\n", 
		width_to_char(w), cache_to_string(d),
		address, __PD_global_clock, __PD_function_name, OP_ID(op));
  }
}


/* trace end of simulation */
void __PD_trace_endsim()
{
  if (__PD_binary_trace_format) {
    u64bitint packet[2] = {__PD_TRACE_ENDSIM, __PD_global_clock};
    binwrite(packet[0], u64bitint, 2);
  }
  else {
    fprintf(__PD_trace_file, ". %llu\n", __PD_global_clock);
  }
}


inline static char width_to_char(__PD_width w)
{
  switch (w) {
  case __PD_B: return 'B';
  case __PD_H: return 'H';
  case __PD_W: return 'W';
  case __PD_S: return 'S';
  case __PD_D: return 'D';
  }
  
  __PD_punt("Unrecognized width specification!");
  return '\0';
}


inline static char* cache_to_string(__PD_cache c)
{
  switch (c) {
  case __PD_V1: return "V1";
  case __PD_C1: return "C1";
  case __PD_C2: return "C2";
  case __PD_C3: return "C3";
  default:      break;
  }
  
  __PD_punt("Unrecognized cache specification!");
  return NULL;
}


inline static char* caches_to_string(__PD_caches sd)
{
  switch (sd) {
  case __PD_V1_V1: return "V1 V1";
  case __PD_V1_C1: return "V1 C1";
  case __PD_V1_C2: return "V1 C2";
  case __PD_V1_C3: return "V1 C3";
    
  case __PD_C1_V1: return "C1 V1";
  case __PD_C1_C1: return "C1 C1";
  case __PD_C1_C2: return "C1 C2";
  case __PD_C1_C3: return "C1 C3";
    
  case __PD_C2_V1: return "C2 V1";
  case __PD_C2_C1: return "C2 C1";
  case __PD_C2_C2: return "C2 C2";
  case __PD_C2_C3: return "C2 C3";
    
  case __PD_C3_V1: return "C3 V1";
  case __PD_C3_C1: return "C3 C1";
  case __PD_C3_C2: return "C3 C2";
  case __PD_C3_C3: return "C3 C3";
  }
  
  __PD_punt("Unrecognized cache specification!");
  return NULL;
}
