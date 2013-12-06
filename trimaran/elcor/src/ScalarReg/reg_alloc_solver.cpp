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

Copyright 2007 University of Michigan

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
//      File:           reg_alloc_solver.cpp
//      Authors:        Nate Clark (based on code by Hansoo Kim)
//      Created:        December 2004
//      Description:    
//
/////////////////////////////////////////////////////////////////////////////

#include "reg_alloc_solver.h"
#include "live_range.h"


Reg_alloc_solver::Reg_alloc_solver(char* file_name) 
{
  phys_file_name = file_name;
  if (((reg_file_pool.get_reg_file(file_name)).caller_size() +
       (reg_file_pool.get_reg_file(file_name)).callee_size() < 2)) {
    cdbg << "Not enough registers available in " << file_name 
	 << "!\nAt least " << 2 << " registers are required." << endl;
    assert(0);
    // If you think you can compile with less than two allocatable
    // registers (e.g. you have a stack based machine), then go ahead
    // and comment out this assert and try it.
  }

}


void Reg_alloc_solver::add_live_range(Live_range* a_lr) 
{
  if (phys_file_name == a_lr->get_oper().physical_file_type()) {
    lrs.add_tail(a_lr);
  }
}


void Reg_alloc_solver::remove_live_range(Live_range* a_lr) 
{
  // FIX: this is algorithmically bad. O(N) removal from a list! look
  // into whether this is a performance issue.
  lrs.remove(a_lr);
}


bool Reg_alloc_solver::process_coloring()
{
  if(lrs.size() == 0) {
    return false;
  }

  bool spill_generated = false;

  // Divide lrs into different groups.
  macros.clear(); unspillable.clear(); constrained.clear(); unconstrained.clear();
  for (List_iterator<Live_range*> lr_iter1(lrs); lr_iter1 != 0; ++lr_iter1) {
    Live_range* lr_ptr = *lr_iter1;
    if (lr_ptr->get_oper().is_macro_reg())
      macros.add_tail(lr_ptr);
    else if (lr_ptr->is_unspillable())
      unspillable.add_tail(lr_ptr);
    else if (lr_ptr->is_constrained())
      constrained.add_tail(lr_ptr);
    else
      unconstrained.add_tail(lr_ptr);
  }

  while (1) {
    Live_range* lr = get_next_live_range();
    if (lr == NULL)
      break;

    reg_state state = lr->color();

    if(state == SPILLED)
      spill_generated = true;

    if (dbg(sa, 3)) {
      if (lr->get_oper().is_macro_reg()) {
	cdbg << "Variable " << lr->get_oper() << " is a macro, and "
	     << " was allocated to " << lr->get_oper().mc_num() << endl;
      } else {
	cdbg << "Variable " << lr->get_oper().vr_num();
	if(lr->is_unspillable())
	  cdbg << " is unspillable";

	cdbg << " with priority "
	     << lr->get_priority() << " r " << lr->get_caller_benefit() << ", e "
	     << lr->get_callee_benefit() << "   "; 

	if (state == NOT_BOUND) {
	  cdbg << " NOT_BOUND" << endl;
	} else if (state == SPILLED) {
	  cdbg << " SPILLED" << endl;
	} else if (state == CALLER_BOUND) {
	  cdbg << " CALLER_BOUND with " << lr->get_oper().mc_num() << endl;
	} else if (state == CALLEE_BOUND) {
	  cdbg << " CALLEE_BOUND with " << lr->get_oper().mc_num() << endl;
	}
      }
    } // end of dbg()
  } // end of while()

  return spill_generated;
}


void Reg_alloc_solver::init_interference_graph() 
{
  List<Live_range*> tmp_list;
  for (List_iterator<Live_range*> iter1(lrs); iter1 != 0; ++iter1) {
    Live_range* lr_ptr1 = *iter1;
    for (List_iterator<Live_range*> iter2(tmp_list); iter2 != 0; ++iter2) {
      Live_range* lr_ptr2 = *iter2;
      lr_ptr1->set_interference(lr_ptr2);
    }
    tmp_list.add_tail(lr_ptr1);
  }
}


//////////////////////////////////////////////////////////////////////////////
// Find Live_range with the highest priority and pop it from the
// appropriate queue.

Live_range* Reg_alloc_solver::get_next_live_range() 
{
  double priority;
  Live_range* prior_lr = 0;

  // Always return lrs in this order:
  // Macros, Unspillable, constrained, unconstrained
  if(macros.size())
    return macros.pop();
  if(unspillable.size())
    return unspillable.pop();
  if(constrained.size()) {
    prior_lr = constrained.head();
    priority = prior_lr->get_priority();
    for (List_iterator<Live_range*> iter(constrained); iter != 0; ++iter) {
      Live_range* lr_ptr = *iter;
      if (lr_ptr->get_priority() > priority) {
	priority = lr_ptr->get_priority();
	prior_lr = lr_ptr;
      }
    }
    constrained.remove(prior_lr);
    return prior_lr;
  }
  if (unconstrained.size()) {
    prior_lr = unconstrained.head();
    priority = prior_lr->get_priority();
    for (List_iterator<Live_range*> iter(unconstrained); iter != 0; ++iter) {
      Live_range* lr_ptr = *iter;
      if (lr_ptr->get_priority() >= priority) {
	priority = lr_ptr->get_priority();
	prior_lr = lr_ptr;
      }
    }
    unconstrained.remove(prior_lr);
    return prior_lr;
  }
  return NULL;	// none left
}
