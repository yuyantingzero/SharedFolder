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
 * File:    PD_signals.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *
 * Description: exception handlers to support speculative execution
 *****************************************************************************/


#include "PD_signals.h"
#include <signal.h>
#include <setjmp.h>
#include "PD_error.h"
#include "PD_stats.h"
#include "PD_trace.h"
#include "PD_timer.h"
#ifdef __PD_CACHE_
#  include "PD_cache.h"
#endif /* __PD_CACHE_ */

const uint __PD_delayed_exception = -1;

uint __PD_do_not_trap_raised_exception = 0;
bool __PD_exception_raised = __PD_false;

jmp_buf __PD_jump_buffer; 


/* intercept exceptions */
void __PD_initialize_signal_handler()
{
  /* call delay exceptions in case of speculation */
  signal(SIGINT,   __PD_delay_exception);
  signal(SIGILL,   __PD_delay_exception);
  signal(SIGTERM,  __PD_delay_exception);
  signal(SIGSEGV,  __PD_delay_exception);
  signal(SIGFPE,   __PD_delay_exception);
#ifdef HPUX
  signal(_SIGQUIT, __PD_delay_exception);
  signal(_SIGHUP,  __PD_delay_exception);
  signal(_SIGBUS,  __PD_delay_exception);
#else /* LINUX OR WINUX OR SUNOS */
  signal(SIGQUIT, __PD_delay_exception);
  signal(SIGHUP,  __PD_delay_exception);
  signal(SIGBUS,  __PD_delay_exception);
#endif
}


/* raise the exception and quit */
void __PD_raise_exception(int id, int sig)
{
  if (id == -1) {
    __PD_punt("Exception raised while simulating %s!", 
		  __PD_function_name);
  }
  else {
    __PD_punt("Exception raised at operation %d of %s!", 
		  id, __PD_function_name);
  }
}


/* delay the raised exception */
void __PD_delay_exception(int sig)
{
  sigset_t sig_mask;

  /* if an  exception was  raised by a  non-speculated operation,  raise it
   * immediately (non-speculated ops set  the trap-flag to non-zero values)
   */
  if (__PD_do_not_trap_raised_exception) {
    __PD_raise_exception(__PD_do_not_trap_raised_exception-1, sig);
  }
  
  /* expection raised by speculated operation, and should be masked */
  __PD_exception_raised = __PD_true;
  
  /* reinstall trap signal handler (uninstalls itself when signal called) */
  signal(sig, __PD_delay_exception);
  
  /* Something changed either in glibc 2.3.3 or kernel 2.6.. now we
     have to explicitly unmask the signal as well. -KF 5/2004 */
  sigemptyset(&sig_mask);
  sigaddset(&sig_mask, sig);
  sigprocmask(SIG_UNBLOCK, &sig_mask, 0);

  /* resume simulation */
  longjmp(__PD_jump_buffer, __PD_delayed_exception);
}


/* clean up simulator buffers - print statistics/trace */
void __PD_atexit()
{
  /* stop simulator timer */
  char* simulation_time = __PD_stop_timer();

  /* write any chained statistics nodes */
  if (__PD_write_stats) {
    __PD_commit_stats(simulation_time);
  }

#ifdef __PD_TRACE_
  /* write out end of simulation token */
  if (__PD_trace_file) {
    __PD_trace_endsim();
  }
#endif /* __PD_TRACE_ */

#ifdef __PD_CACHE_
  /* emmit the memory hierarchy performance statistics */
  __PD_cache_write_stats();
#endif /* __PD_CACHE_ */
}
