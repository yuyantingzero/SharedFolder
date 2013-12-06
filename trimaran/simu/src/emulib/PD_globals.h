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
 * File:    PD_globals.h
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: general constansts and types
 *****************************************************************************/


#ifndef __PD_GLOBALS_H_
#define __PD_GLOBALS_H_


typedef unsigned long long u64bitint;
typedef unsigned char      ubyte;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;


#if !defined(__GNUC__) || !defined(__cplusplus)
  typedef char bool;
# define __PD_true  1
# define __PD_false 0
#endif


#define __PD_MAX_SRC   5
#define __PD_MAX_DEST  4


/* bits for specifying statistic to track; bits 0-3 */
#define __PD_OPTYPES  15
#define __PD_NO_OP    0x0000 /* pseudop */
#define __PD_IA_OP    0x0001 /* integer */
#define __PD_FA_OP    0x0002 /* float   */
#define __PD_LD_OP    0x0003 /* load    */
#define __PD_ST_OP    0x0004 /* store   */
#define __PD_CP_OP    0x0005 /* compare */
#define __PD_PB_OP    0x0006 /* pbranch */
#define __PD_BR_OP    0x0007 /* branch  */
#define __PD_ICM_OP   0x0008 /* inter-cluster move */
/* RMR { vector (SIMD) ops */
#define __PD_VIA_OP   0x0009 /* vector integer */
#define __PD_VFA_OP   0x000A /* vector float   */
#define __PD_VLD_OP   0x000B /* vector load    */
#define __PD_VST_OP   0x000C /* vector store   */
#define __PD_VXF_OP   0x000D /* vector-scalar transfer */
#define __PD_VPR_OP   0x000E /* permute, splat, etc */
/* } RMR */

/* bits for specifying general, caller and callee save operations; bits 4-5 */
#define __PD_OPCLASS  3
#define __PD_CLRINDEX 1
#define __PD_CLEINDEX 2
#define __PD_CLRSAVE  0x0020          /* 0000 0000 0010 0000 caller save */
#define __PD_CLESAVE  0x0030          /* 0000 0000 0011 0000 callee save */
#define __PD_XXX_OPS  0x0010          /* 0000 0000 0001 0000 all others  */

/* bits for specifying semantics of the operation; bits 6-8 */
#define __PD_PREDICATED_OP   0x0040   /* 0000 0000 0100 0000 predicated   */
#define __PD_SPECULATED_OP   0x0080   /* 0000 0000 1000 0000 speculated   */
#define __PD_NOTRAPPING_OP   0x0100   /* 0000 0001 0000 0000 non-trapping */


/* bits for specifying simulation flags */
#define __PD_BINTRACE  0x01
#define __PD_MEMTRACE  0x02
#define __PD_CFGTRACE  0x04
#define __PD_DYNSTATS  0x08
#define __PD_VIRTREGS  0x10
#define __PD_MEMSIM    0x20
#define __PD_MEM_HIGH_TO_LOW               0x40
#define __PD_MEM_LOW_TO_HIGH               0x80
#define __PD_STACK_MANIPULATED_IN_ELCOR    0x100
#define __PD_PER_OP_VALUE_TRACE            0x200

#endif /* __PD_GLOBALS_H_ */
