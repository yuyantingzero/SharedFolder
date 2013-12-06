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
// File:     emitter.h
// Author:   Rodric M. Rabbah, Marc Saint
///////////////////////////////////////////////////////////////////////////////


#ifndef _TABLE_EMITTER_H
#define _TABLE_EMITTER_H


#include <strings.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <ctype.h>

#include "simu.h"
#include "streamBuffer.h"


/* These masks are defined in PD_globals.h */
#include "PD_globals.h"
typedef enum {
  PSEUDO_OP = __PD_NO_OP,
  IA_OP = __PD_IA_OP,
  FA_OP = __PD_FA_OP,
  LD_OP = __PD_LD_OP,
  ST_OP = __PD_ST_OP,
  CP_OP = __PD_CP_OP,
  PB_OP = __PD_PB_OP,
  BR_OP = __PD_BR_OP,
  ICM_OP = __PD_ICM_OP,
  /* RMR { vector/simd ops */
  VIA_OP = __PD_VIA_OP,
  VFA_OP = __PD_VFA_OP,
  VLD_OP = __PD_VLD_OP,
  VST_OP = __PD_VST_OP,
  VXF_OP = __PD_VXF_OP,
  VPR_OP = __PD_VPR_OP
  /* } RMR */
} Optype;

#define PREDICATED_OP __PD_PREDICATED_OP
#define SPECULATED_OP __PD_SPECULATED_OP
#define NOTRAPPING_OP __PD_NOTRAPPING_OP

#define CLRSAVE  __PD_CLRSAVE
#define CLESAVE  __PD_CLESAVE
#define XXX_OPS  __PD_XXX_OPS

#define MAX_SRC  __PD_MAX_SRC
#define MAX_DEST __PD_MAX_DEST


class Port
{
public:
  inline Port();
  inline ~Port();
  
public: 
  eString file; 
  int offset; //support for multi-cluster simulation
  int  num;
  int  rot;
  int  vec;
  bool is_reg;
};


class OpEntry
{
public:
  inline OpEntry();
  inline ~OpEntry();

  inline void clear();

public:
  int  id;           // Rebel_id
  int  index;        // operation index within a block
  int  flags;        // caller or callee save
  bool predicated;   // true if operation is predicated
  bool speculative;  // true if operation is speculative
  bool mask;         // true if operation is masked speculative operation
  eString op;        // emulation  function
  Optype  type;      // statistics tracking (categorize op)
  eString attribute; // name of the emulation table
  Port dest[MAX_DEST];
  Port src[MAX_SRC];
  Port pred;
  int  lat[MAX_DEST];
  bool gen_trace;
  int cid;           // cluster id
};


void print(const OpEntry&);
void print_table(const Vector<OpEntry*>&, int, streamBuffer*);

extern int S_remap_optables;
void classic_layout(const Vector<OpEntry*>&, int, streamBuffer*);
void remapped_layout(const Vector<OpEntry*>&, int, streamBuffer*);


////////////////////////////////////////////////////////////////
//          definitions for class Port
////////////////////////////////////////////////////////////////
inline Port::Port()
  :
  offset(0), num(0), rot(0), vec(0), is_reg(false)
{
  // null
}

inline Port::~Port()
{
  // null
}


////////////////////////////////////////////////////////////////
//          definitions for class OpEntry
////////////////////////////////////////////////////////////////


///////////////  OpEntry::OpEntry ////////////////////////
//
// Initializes OpEntry to zeros and blanks
//
inline OpEntry::OpEntry()
  :
  id(0), index(0), flags(0), predicated(false), speculative(false), mask(false)
{
  lat[0] = lat[1] = lat[2] = lat[3] = 0;
}

inline OpEntry::~OpEntry()
{
  // null
}

///////////////  OpEntry::clear ////////////////////////
//
// Clears OpEntry to zeros and blanks
//
inline void OpEntry::clear() 
{
  int n;
  
  id = index = flags = predicated = speculative = mask = false;

  type = PSEUDO_OP;

  // prefer not to deallocate strings
  op = attribute = null_string;   
  
  for (n = 0; n < MAX_DEST; n++) { 
    dest[n].file   = null_string;
    dest[n].num    = lat[n] = 0;
    dest[n].rot    = 0;
    dest[n].vec    = 0;
    dest[n].is_reg = false;
  }
  
  for (n = 0; n < MAX_SRC; n++) {
    src[n].file   = null_string;
    src[n].num    = 0;
    src[n].rot    = 0;
    src[n].vec    = 0;
    src[n].is_reg = false;
  }
  
  pred.file   = null_string;
  pred.num    = 0;
  pred.rot    = 0;
  pred.vec    = 0;
  pred.is_reg = false;
}

#endif /* _TABLE_EMMITTER_H */
