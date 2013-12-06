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
//      File:           el_loop_tools.cpp
//      Author:         David I. August
//      Created:        May 1995
//      Description:    Loop Tool Functions
//
/////////////////////////////////////////////////////////////////////////////

#include "el_loop.h"
#include "el_loop_tools.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "hash_set.h"
#include "op.h"
#include "iterators.h"
#include "el_loop_filters.h"

#undef DEBUG

/*
 * determines if "inner" is nested in "outer"
 */

bool El_loop_is_nested(El_Loop* inner, El_Loop* outer)
{
  if(!inner || !outer)
    {
      El_punt("El_loop_is_nested: passed a NULL pointer");
    }
  if (inner == outer)
    {
      return false;
    }
  return (inner->loop_blocks.is_subset(outer->loop_blocks));
}


void El_remove_loop_region(Compound_region* loop_region)
{
  if(!loop_region->is_loopbody())
    {
      El_punt("El_remove_loop_region: not passed a loop region.");
    }
  El_remove_region(loop_region, true);
}

unsigned hash_loop_body(LoopBody*& l)
{
   return ELCOR_INT_HASH(l);
}

void El_remove_all_loop_regions(Compound_region* reg)
{
  /*
  ** First find all loop_regions
  */
  Hash_set<LoopBody*> loop_regions(hash_loop_body, 16);
  Hash_set_iterator<LoopBody*> loop_iter;
  LoopBody *lb;
  bool reg_alloc_flag,sched_flag;
  Region *subreg;

  for(Region_all_ops op_iter(reg); op_iter != 0; op_iter++)
    {
      Compound_region* parent = (*op_iter)->parent();
      while(parent != reg)
	{
	  if(parent->is_loopbody())
	    {
	      loop_regions += (LoopBody*)parent;
	    }
	  parent = parent->parent();
	}
    }

  /*
  **  Transfer any Elcor flags that exist down to the immediate subregions
  */

  for(loop_iter(loop_regions); loop_iter != 0; loop_iter++) {
    lb = *loop_iter;
    reg_alloc_flag = lb->flag(EL_REGION_REGISTER_ALLOCATED);
    sched_flag = lb->flag(EL_REGION_SCHEDULED);
    if (reg_alloc_flag || sched_flag) {
      for (Region_subregions sub_iter(lb); sub_iter!=0; sub_iter++) {
        subreg = *sub_iter;
	if (subreg->is_compound()) {
          if (reg_alloc_flag)
            subreg->set_flag(EL_REGION_REGISTER_ALLOCATED);
          if (sched_flag)
            subreg->set_flag(EL_REGION_SCHEDULED);
        }
      }
    }
  }


  /*
  ** Remove them 
  */

  for(loop_iter(loop_regions); loop_iter != 0; loop_iter++)
    {
      El_remove_loop_region(*loop_iter);
    }
}

void El_setup_swp_loop_regions(Compound_region* reg)
{
  El_form_basic_blocks(reg);

  /*
  ** The Loop Graph constructor calls CFA and builds entire loop graph
  */
  El_Loop_Graph all_loops(reg);

  all_loops.find_induction_variable_info(ALL_LOOPS);
  all_loops.find_swp_able_loops(ALL_LOOPS);

  Loop_swp_loop_filter SWP_ABLE;
  if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
    all_loops.create_remainder_blocks(&SWP_ABLE);
  }
  all_loops.create_preloop_blocks(&SWP_ABLE);
  all_loops.create_postloop_blocks(&SWP_ABLE);
  all_loops.insert_brlc(&SWP_ABLE);
  if(El_msched_allocator == MODULO_VARIABLE_EXPANSION) {
    all_loops.create_preremainder_blocks(&SWP_ABLE);
  }
  all_loops.insert_loop_regions(&SWP_ABLE);

#ifdef DEBUG
  cout << "After setup swp loop regions" << endl;
  cout << *reg;
#endif
  
  El_remove_basic_blocks(reg);
}


