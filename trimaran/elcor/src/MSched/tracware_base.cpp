/******************************************************************************

                    SOFTWARE LICENSE AGREEMENT NOTICE
                   -----------------------------------

IT IS A BREACH OF THIS LICENSE AGREEMENT TO REMOVE THIS NOTICE FROM THE FILE
OR SOFTWARE, OR ANY MODIFIED VERSIONS OF THIS FILE OR SOFTWARE OR DERIVATIVE
WORKS.
___________________________________________________

Copyright Notices/Identification of Licensor(s) of Original Software
in the File


All rights reserved by the foregoing, respectively.
___________________________________________________

Copyright Notices/Identification of Subsequent Licensor(s)/Contributors of
Derivative Works

Copyright 1994 Hewlett-Packard Company

All rights reserved by the foregoing, respectively.
___________________________________________________

The code contained in this file, including both binary and source [if released
by the owner(s)] (hereafter, Software) is subject to copyright by the
respective Licensor(s) and ownership remains with such Licensor(s).  The
Licensor(s) of the original Software remain free to license their respective
proprietary Software for other purposes that are independent and separate from
this file, without obligation to any party.

Licensor(s) grant(s) you (hereafter, Licensee) a license to use the Software
for academic, research and internal business purposes only, without a fee.
"Internal business purposes" means that Licensee may install, use and execute
the Software for the purpose of designing and evaluating products.  Licensee
may submit proposals for research support, and receive funding from private
and Government sponsors for continued development, support and maintenance of
the Software for the purposes permitted herein.

Licensee may also disclose results obtained by executing the Software, as well
as algorithms embodied therein.  Licensee may redistribute the Software to
third parties provided that the copyright notices and this License Agreement
Notice statement are reproduced on all copies and that no charge is associated
with such copies. No patent or other intellectual property license is granted
or implied by this Agreement, and this Agreement does not license any acts
except those expressly recited.

Licensee may modify the Software to make derivative works (as defined in
Section 101 of Title 17, U.S. Code) (hereafter, Derivative Works), as
necessary for its own academic, research and internal business purposes.
Title to copyrights and other proprietary rights in Derivative Works created
by Licensee shall be owned by Licensee subject, however, to the underlying
ownership interest(s) of the Licensor(s) in the copyrights and other
proprietary rights in the original Software.  All the same rights and licenses
granted herein and all other terms and conditions contained in this Agreement
pertaining to the Software shall continue to apply to any parts of the
Software included in Derivative Works.  Licensee's Derivative Work should
clearly notify users that it is a modified version and not the original
Software distributed by the Licensor(s).

If Licensee wants to make its Derivative Works available to other parties,
such distribution will be governed by the terms and conditions of this License
Agreement.  Licensee shall not modify this License Agreement, except that
Licensee shall clearly identify the contribution of its Derivative Work to
this file by adding an additional copyright notice to the other copyright
notices listed above, to be added below the line "Copyright
Notices/Identification of Subsequent Licensor(s)/Contributors of Derivative
Works."  A party who is not an owner of such Derivative Work within the
meaning of U.S. Copyright Law (i.e., the original author, or the employer of
the author if "work of hire") shall not modify this License Agreement or add
such party's name to the copyright notices above.

Each party who contributes Software or makes a Derivative Work to this file
(hereafter, Contributed Code) represents to each Licensor and to other
Licensees for its own Contributed Code that:

(a)  Such Contributed Code does not violate (or cause the Software to
violate) the laws of the United States, including the export control laws of
the United States, or the laws of any other jurisdiction.

(b)  The contributing party has all legal right and authority to make such
Contributed Code available and to grant the rights and licenses contained in
this License Agreement without violation or conflict with any law.

(c)  To the best of the contributing party's knowledge and belief, the
Contributed Code does not infringe upon any proprietary rights or intellectual
property rights of any third party.

LICENSOR(S) MAKE(S) NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE SOFTWARE
OR DERIVATIVE WORKS FOR ANY PURPOSE.  IT IS PROVIDED "AS IS"    WITHOUT
EXPRESS OR IMPLIED WARRANTY, INCLUDING BUT NOT LIMITED TO THE MERCHANTABILITY,
USE OR FITNESS FOR ANY PARTICULAR PURPOSE AND ANY WARRANTY AGAINST
INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHTS.  LICENSOR(S) SHALL NOT BE
LIABLE FOR ANY DAMAGES SUFFERED BY THE USERS OF THE SOFTWARE OR DERIVATIVE
WORKS.

Any Licensee wishing to make commercial use of the Software or Derivative
Works should contact each and every Licensor to negotiate an appropriate
license for such commercial use, and written permission of all Licensors will
be required for such a commercial license.  Commercial use includes (1)
integration of all or part of the source code into a product for sale by or on
behalf of Licensee to third parties, or (2) distribution of the Software or
Derivative Works to third parties that need it to utilize a commercial product
sold or licensed by or on behalf of Licensee.

By using or copying this Contributed Code, Licensee agrees to abide by the
copyright law and all other applicable laws of the U.S., and the terms of this
License Agreement.  Any individual Licensor shall have the right to terminate
this license immediately by written notice upon Licensee's breach of, or
non-compliance with, any of its terms.  Licensee may be held legally
responsible for any copyright infringement that is caused or encouraged by
Licensee's failure to abide by the terms of this License Agreement.

******************************************************************************/




/////////////////////////////////////////////////////////////////////////////
//
//      File:           tracware_base.cpp
//      Authors:        Rodric M. Rabbah
//      Created:        Sep 1999
//      Description:    Tracware base-class implementation
//
/////////////////////////////////////////////////////////////////////////////

#include "tracware_base.h"
#include "el_driver_init.h"
#include "graph_filters.h"

Tracware::Tracware(ModuloScheduler* msched)
    : _msched(msched)
{}

Tracware::~Tracware() {}

void Tracware::init(ModuloScheduler* msched)
{
  _msched = msched;
}


// if a resource is available between the operation's early sched time
// and late sched time, sched_info and sched_slot are set appropriately
// the return type indicate success if a valid slot is found, otherwise
// failure is indicated
bool Tracware::get_sched_info(const Slist<Op_Info>& nodes, SchedInfo* sched_info, int& slot)
{
  List<pattern> patterns;    
  // Op* node = nodes.head().second;
  Op *node = sched_info->op_rep;

  // reset the incoming time parameter
  slot = 0;

  // the resource manager
  assert (_msched != NULL);
  RU* rm = _msched->rumap();

  // calculate the earliest start for this node as constrained by
  // already scheduled predecessors
  int min_time, max_time, current_time;
  compute_min_max_times(node, min_time, max_time);

  // cdbg << "MIN TIME " << min_time <<" MAXTIME " << max_time << endl;
  for (current_time=min_time; current_time <= max_time; current_time++) {
		//cdbg << "\tget patterns" << endl;
    patterns.clear();

    rm->get_nonconflicting_patterns(node, current_time - 1, &patterns);

    if (!(patterns.is_empty())) { // there are conflict-free resources at current_time.
			//cdbg << "\tpattern found" << endl;
      sched_info->specific_op = patterns.head()->op;
      sched_info->op_priority = patterns.head()->priority;
      slot = current_time;
      return true;
    }
    //cdbg << "\tpattern empty" << endl;

  } // for (current_time yada yada
 
  // could not find any available resource for this operation
  // in the specified time range
  return false;
}


// find the best resource at the specified time slot
// if a resource is found, sched_info is set, and success returned
// and failure is indicated
bool Tracware::get_specific_sched_info(const Slist<Op_Info>& nodes, SchedInfo* sinfo, const int time)
{
    int verify;

    // Mukund, Apr 2001: get_specific_sched_info() is called from
    // ModuloScheduler::place() if no architectural opcode is present for
    // the op being placed (this happens when the earlier call to
    // get_sched_info() was unable to find a scheduling slot without
    // resource conflicts). In this case, a bunch of ops have been
    // unplaced, so a call to get_sched_info() now will return a valid
    // sched slot and alternative (pattern). This new sched slot <verify>
    // can be different from the chosen sched slot <time> because of
    // complex reservation tables etc., but at this point, we'll just core
    // dump!

    if (!get_sched_info(nodes, sinfo, verify)) {
	return false;
    }

    // sanity check - the returned schedule time should be equal to the
    // supplied schedule slot time.
    // ntclark: Swing scheduler doesn't use tracware.
    if(El_msched_algorithm == ITERATIVE_MSCHED) {
      if (verify != time) {
	El_warn("Tracware::get_specific_sched_info(): Found a different sched slot %d instead of chosen slot %d!", 
		verify, time);
      }
    }

    return true;
}


void Tracware::commit(Op* op, char* specific_opcode, int sched_time)
{}


void Tracware::remove(Op* op)
{}


// calculate the early and late start times for the operation
void Tracware::compute_min_max_times(Op* node, int& min_time, int& max_time)
{
    assert (_msched != NULL);

    int slot = 0;
    _msched->CalcEstart(node, &slot);
    min_time = slot > 1 ? slot : 1;
    // Mukund: ELCOR_MAX(..., 1) is used to force the scheduler to
    // break op chains that might have resulted in alternatives being
    // invalidated by Tracware timing. The correct long-term solution
    // is to bump up the edge latency corrspd to the op chain to be
    // broken.
    max_time = min_time + ELCOR_MAX((_msched->II()-1), 1);
}
