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

Copyright  2004  Massachusetts  Institute  of Technology.  All  rights
reserved by the foregoing, respectively.

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
 * File:    PD_instrument_memops.c
 * Authors: Rodric M. Rabbah
 *
 * Description: simple hooks for intercepting memory operations
 *****************************************************************************/


#include "PD_globals.h"
#include "PD_op.h"
#include "PD_parms.h"
#include "PD_trace.h"
#include "PD_trace_defs.h"
#ifdef __PD_CACHE_
#  include "PD_cache.h"
#endif /* __PD_CACHE_ */


bool __PD_instrument_load(const __PD_OP* op, __PD_width w, __PD_cache sc, __PD_cache dc, __PD_addressing_type t)
{
  bool miss = __PD_false;

#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_MEMTRACE) __PD_trace_load(op, w, sc, dc, t);
#endif /* __PD_TRACE_  */

#ifdef __PD_CACHE_
  if (__PD_parameters->flags & __PD_MEMSIM) miss = __PD_cache_data_load(op, w, t);
#endif /* __PD_CACHE_ */

  return miss;
}


void __PD_instrument_lds(const __PD_OP* op, __PD_width w, __PD_cache sc, __PD_cache dc, __PD_addressing_type t)
{
#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_MEMTRACE) __PD_trace_lds(op, w, sc, dc, t);
#endif /* __PD_TRACE_  */

#ifdef __PD_CACHE_
  if (__PD_parameters->flags & __PD_MEMSIM) __PD_cache_data_load(op, w, t);
#endif /* __PD_CACHE_ */
}


void __PD_instrument_ldv(const __PD_OP* op, __PD_width w, __PD_addressing_type t)
{
#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_MEMTRACE) __PD_trace_ldv(op, w, t);
#endif /* __PD_TRACE_  */

#ifdef __PD_CACHE_
  if (__PD_parameters->flags & __PD_MEMSIM) __PD_cache_data_load(op, w, t);
#endif /* __PD_CACHE_ */
}


void __PD_instrument_store(const __PD_OP* op, __PD_width w, __PD_cache dc, __PD_addressing_type t)
{
#ifdef __PD_TRACE_
  if (__PD_parameters->flags & __PD_MEMTRACE) __PD_trace_store(op, w, dc, t);
#endif /* __PD_TRACE_  */

#ifdef __PD_CACHE_
  if (__PD_parameters->flags & __PD_MEMSIM) __PD_cache_data_store(op, w, t);
#endif /* __PD_CACHE_ */
}
