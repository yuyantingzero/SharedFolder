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
 * File:    PD_op.h
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: traditional and remapped op table layout
 *****************************************************************************/


#ifndef __PD_OP_H_
#define __PD_OP_H_


#include "PD_globals.h"
#include "PD_reg.h"


typedef struct __PD_table  __PD_table;
typedef struct __PD_opnode __PD_opnode;


#ifndef __PD_REMAP_OP_LAYOUT_
  typedef __PD_opnode __PD_OP;
#else
  typedef __PD_table  __PD_OP;
#endif /* __PD_REMAP_OP_LAYOUT_ */

typedef void (*__PD_impl) (__PD_OP*);


/* a single opnode represent an equivalent REBEL operation */
struct __PD_opnode {
  const ulong  rebel_id;               /* rebel op id */
  const ulong  index;                  /* op index within a block */
  const ushort flags;                  /* debugging and statistics; only lower 8 bits current used
						    * lower 4 bits encode the operation type and the 
						    * upper 4 bits specify if the operation is predicated, speculated or masked
						    */
  const uint address;                  /* address of the op in memory */
  const int cid;                       /* cluster id */
  const bool gen_trace;                /* Set to true to trace this op */
  const __PD_impl op;                  /* implementation */
  const __PD_port pred;                /* predicate port */
  const __PD_port src [__PD_MAX_SRC];  /* source ports */
  const __PD_port dest[__PD_MAX_DEST]; /* destination ports */
}; /* __PD_opnode */


/* locality-organized operation */
struct __PD_table {
  const ulong*     rebel_id;
  const ulong*     index;
  const ushort*    flags;
  const uint*      address;
  const __PD_impl* op;
  const __PD_port* pred;
  const __PD_port* src[__PD_MAX_SRC];
  const __PD_port* dest[__PD_MAX_DEST];
}; /* __PD_table */


#ifndef __PD_REMAP_OP_LAYOUT_
/* macros for retrieving opnode fields */
#  define OP_ID(op)    op->rebel_id
#  define OP_BBIDX(op) op->index
#  define OP_FLAGS(op) op->flags
#  define OP_ADDR(op)  op->address
#  define OP_IMPL(op)  op->op
#  define OP_PRED(op)  op->pred
#  define OP_SRC0(op)  op->src[0]
#  define OP_SRC1(op)  op->src[1]
#  define OP_SRC2(op)  op->src[2]
#  define OP_SRC3(op)  op->src[3]
#  define OP_SRC4(op)  op->src[4]
#  define OP_DEST0(op) op->dest[0]
#  define OP_DEST1(op) op->dest[1]
#  define OP_DEST2(op) op->dest[2]
#  define OP_DEST3(op) op->dest[3]
#else /* remapped op layout */
/* macros for retrieving opnode fields from table */
#  define OP_ID(op)    op->rebel_id[__PD_PC_REG]
#  define OP_BBIDX(op) op->index[__PD_PC_REG]
#  define OP_FLAGS(op) op->flags[__PD_PC_REG]
#  define OP_ADDR(op)  op->address[__PD_PC_REG]
#  define OP_IMPL(op)  op->op[__PD_PC_REG]
#  define OP_PRED(op)  op->pred[__PD_PC_REG]
#  define OP_SRC0(op)  op->src[0][__PD_PC_REG]
#  define OP_SRC1(op)  op->src[1][__PD_PC_REG]
#  define OP_SRC2(op)  op->src[2][__PD_PC_REG]
#  define OP_SRC3(op)  op->src[3][__PD_PC_REG]
#  define OP_SRC4(op)  op->src[4][__PD_PC_REG]
#  define OP_DEST0(op) op->dest[0][__PD_PC_REG]
#  define OP_DEST1(op) op->dest[1][__PD_PC_REG]
#  define OP_DEST2(op) op->dest[2][__PD_PC_REG]
#  define OP_DEST3(op) op->dest[3][__PD_PC_REG]
/* special  handler for  when  the index  of  an operation  is not  PC
 * based, for example when looking up the address of a branch target
 */
#  define OP_ADDR_I(op, i)  op->address[i]
#endif /* __PD_REMAP_OP_LAYOUT_ */

#endif /* __PD_OP_H_ */
