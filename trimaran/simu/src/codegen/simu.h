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
// File:   simu.h
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


#ifndef _SIMU_H
#define _SIMU_H


#include <stdio.h>
#include <string.h>

#include "string_class.h"
#include "hash_functions.h"
#include "hash_map.h"
#include "stdlib.h"
#include <fstream>
#include "fcntl.h"
#include "IR_outstream.h"
#include "operand.h"
#include "region.h"
#include "error.h"

// used by the code part of the el_processor
#include "hash_functions.h"
#include "ir_operand.h"
#include "opcode_properties.h"

// used by the data part of the el_processor
#include <stdlib.h>
#include "IR_symbol.h"
#include "el_expression.h"
#include "el_data.h"
#include "list.h"
#include "slist.h"

#include "simu_timer.h"

// needed for call_info parsing
#define MAX_BUF_SIZE 8100

// needed for compilation
#define endl      "\n"

// number of bytes that can be allowed to pass thru regs
#define abi_BYTES 16

#define MEM_HIGH_TO_LOW 0
#define MEM_LOW_TO_HIGH 1

enum DATA_TYPE { 
  VOID, INT, CHAR, FLOAT, DOUBLE, 
  STRUCT, UNION, ELLIPSIS, SHORT, LONG, LONGLONG
}; /* DATA_TYPE */

enum DATA_DESC { 
  NIL, DATA_POINTER, CODE_POINTER 
}; /* DATA_DESC */

enum EXTERN_TYPE { 
  IS_FUNCTION, IS_DATA, IS_SU 
}; /* EXTERN_TYPE */

enum HOST_TYPES { 
  UNSUPPORTED, HP, X86LIN, X86WIN, SUNOS
}; /* HOST_TYPE */

extern const char*   STAB;
extern const eString null_string;


class Host_Info
{
public:
  Host_Info():platform(UNSUPPORTED){};
  ~Host_Info(){};

public:
  enum HOST_TYPES platform;
};


class hc_param
{
public:
  inline hc_param()
    :
    type(VOID), sgnd(true), desc(NIL), depth(0), code_depth(0), 
    macro_reg(false), offset(0), has_soffset(false), soffset(0), 
    isArray(false), vector_size(0) 
  { /* null */ }

  inline ~hc_param()
  { /* null */ }

public:
  enum DATA_TYPE type;   // will hold the DATA_TYPE
  bool           sgnd;   // signed is false if type is unsigned, else true
  eString        name;   // name of struct, union, or function
  eString        call_info;
  enum DATA_DESC desc;   // pointer descriptors 
  int            depth;  // depth of pointer  
  int            code_depth;  // depth of code pointer  
    
  bool           macro_reg;  // parameter information
  Macro_name     macro_name; // used to properly pass
  int            offset;     // parameters on function calls

  Macro_name     second_macro_name; // KVM : used for storing the second
                                    // macro name, when one longlong arg is
                                    // passed using 2 macros

  bool           has_soffset;    
  int            soffset;    

  bool           isArray;           // true if this is an array
  int            vector_size;       // size of array_info_vector
  Vector<int>    array_info_vector; // holds the size of each array declaration
  // -1 entry is made if size is unspecified

public:
  inline unsigned hash_hc_param(hc_param& p) {
    return hash_estring(p.name);
  }


public:
  inline hc_param& operator=(const hc_param& x) {    
    if (this != &x) {
      type = x.type; sgnd = x.sgnd; name = x.name; call_info = x.call_info;
      desc = x.desc; depth = x.depth; code_depth = x.code_depth; 
      macro_reg = x.macro_reg; macro_name = x.macro_name; offset = x.offset;
      has_soffset = x.has_soffset; soffset = x.soffset;
      isArray = x.isArray; vector_size = x.vector_size; 
      array_info_vector = x.array_info_vector;
      second_macro_name = x.second_macro_name;
    }
    return (*this);
  }

  // to make the compiler happy
  inline bool operator==(const hc_param& p)  {
    // to make the compiler happy, else warning is generated
    if(&p == NULL) return true; 
    return true;     // return true anyway for now
  }
};

class hc_aggregate
{
public:
  inline hc_aggregate()
    :
    count(0), visited(false), isVararg(false), isKANDRC(false),
    isRecord(false), vec_temp(0)
  { /* null */ }
  
  inline ~hc_aggregate()
  { /* null */ }

public:
  eString name;          // key
  eString call_info;     // call info for fucntion declarations
  int     count;         // num of params, for fns -1 => ellipsis
  bool    visited;       // For graph traversal
  bool    isVararg;      // TRUE if function has vararg argument
  bool    isKANDRC;      // TRUE if function is not standard vararg
  bool    isRecord;      // TRUE if STRUCT or UNION
  int     vec_temp;      // for varargs, used to extract function name

  Vector<hc_param> params;

  inline hc_aggregate& operator=(const hc_aggregate& x) {
    if (this != &x) {
      name = x.name; call_info = x.call_info; count = x.count;
      visited = x.visited; isVararg = x.isVararg; isKANDRC = x.isKANDRC;
      isRecord = x.isRecord; vec_temp = x.vec_temp; params = x.params;
    }
    return (*this);
  }
};


// describes a stackframe
class Frame_params
{
public:
  int local;
  int param;
  int swap;
  int fma;
  int size;
};

struct PD_static_stats
{
  inline PD_static_stats()
  {
    reset();
  }
    
  inline void reset()
  {
    Static_branch = 0;
    Static_load   = 0;
    Static_store  = 0;
    Static_ialu   = 0;
    Static_falu   = 0;
    Static_cmpp   = 0;
    Static_pbr    = 0;
    Static_icm    = 0;
    /* RMR { add category for SIMD ops */
    Static_vialu   = 0;
    Static_vfalu   = 0;
    Static_vload   = 0;
    Static_vstore  = 0;
    Static_vxfr    = 0;
    Static_vperm   = 0;
    /* } RMR */
    Static_caller_spill = 0;
    Static_callee_spill = 0;
    Static_ops    = 0;
    sched_len     = 0;
    real_op_count = 0;
  }

  unsigned int sched_len;  /* Scheduling length */
  unsigned int real_op_count;

  /* Static Instruction Mix */
  unsigned long long Static_branch;
  unsigned long long Static_load;
  unsigned long long Static_store;
  unsigned long long Static_ialu;
  unsigned long long Static_falu;
  unsigned long long Static_cmpp;
  unsigned long long Static_pbr;
  unsigned long long Static_icm;
  /* RMR { vector (SIMD) ops */
  unsigned long long Static_vialu;
  unsigned long long Static_vfalu;
  unsigned long long Static_vload;
  unsigned long long Static_vstore;
  unsigned long long Static_vxfr;
  unsigned long long Static_vperm;
  /* } RMR */
  unsigned long long Static_ops;
  unsigned long long Static_caller_spill;
  unsigned long long Static_callee_spill;
};

// Structure that hold register file parameters
// Eg.
// gpr = start point of gpr file
// gpr_stat = start point of stat gpr file
// gpr_rot  = start point of rot gpr file
struct PD_regfile
{
  inline void PD_regile()
  {
    reset();
  }
    
  inline void reset()
  {
    gpr_stat_size = 0;
    gpr_rot_size  = 0;
    fpr_stat_size = 0;
    fpr_rot_size  = 0;
    btr_stat_size = 0;
    pr_stat_size  = 0;
    pr_rot_size   = 0;
    cr_stat_size  = 0;
    cr_rot_size   = 0;
    /* RMR { adding support for vector file type */
    vir_stat_size = 0;
    vir_rot_size  = 0;
    vfr_stat_size = 0;
    vfr_rot_size  = 0;
    vmr_stat_size = 0;
    vmr_rot_size  = 0;
    /* } RMR */
    init = false;
  }
    
  // sizes of static regfile is set is 1 more than the largest 
  // static register number accessed
  inline int increment(int vector_length)
  {
    gpr_stat_size++;
    fpr_stat_size++;
    btr_stat_size++;
    pr_stat_size++;
    cr_stat_size++;
    vir_stat_size += vector_length;
    vfr_stat_size += vector_length;
    //vmr_stat_size += vector_length;
    vmr_stat_size++;

    return (gpr_stat_size +  
            fpr_stat_size +
            btr_stat_size +
            pr_stat_size +
            cr_stat_size +
            vir_stat_size +
            vfr_stat_size +
            vmr_stat_size);
  }
    
  int gpr_stat_size;
  int gpr_rot_size;
  int fpr_stat_size;
  int fpr_rot_size;
  int btr_stat_size;
  int pr_stat_size;
  int pr_rot_size;
  int cr_stat_size;
  int cr_rot_size;
  /* RMR { adding support for vector file type */
  int vir_stat_size;
  int vir_rot_size;  
  int vfr_stat_size;
  int vfr_rot_size;  
  int vmr_stat_size;
  int vmr_rot_size;
  /* } RMR */
  bool init;
};

extern ostream& operator<<(ostream& os, hc_param& s);
extern ostream& operator<<(ostream& os, hc_aggregate* s);
extern ostream& operator<<(ostream& os, hc_aggregate& s);

#endif /* _SIMU_H */
