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




///////////////////////////////////////////////////////////////////////////
//									 //
//      File:           el_opti_driver.cpp                              //
//      Authors:        Matt Jennings        		                 //
//      Created:        June 1997        				 //
//      Description:    driver for optimizations controlled by global 	 //
// 			flags. called from common_process_function.cpp	 //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "dbg.h"
#include "pred_analysis.h"
#include "el_driver_init.h"
#include "el_opti_init.h"
#include "el_io.h"
#include "el_opti_loopinvariant.h"
#include "el_opti_common_subexpr_elim.h" 
#include "el_opti_forward_copy_prop.h"
#include "el_opti_dead_code.h"
#include "local_copy_propagate.h"
#include "el_local_const_folding.h"
#include "el_local_str_reduction.h"
#include "el_local_op_folding.h"
#include "el_global_cvt_elim.h"
#include "el_width_analysis.h"
#include "ir_region.h"

using namespace std;

#define EL_LOCAL_OPTI_MAX_ITER 5


/*
 *	The goal for setting these up is to do global liveness in the
 *	driver, then local reaching defs w/i each opti if it is needed.
 */
void el_opti_local_driver(Compound_region* f)
{
  int i, change;

  for (i=0; i<EL_LOCAL_OPTI_MAX_ITER; i++) {
    change = 0;

    delete_local_analysis_info_for_all_hbs_bbs(f);

    if (El_do_local_copy_propagation)
      local_copy_propagation(f);

    if (El_do_local_constant_folding)
      change += El_opti_local_constant_folding(f);

    if (El_do_local_strength_reduction)
      change += El_opti_local_strength_reduction(f);

    // Perform global liveness analysis
    create_local_analysis_info_for_all_hbs_bbs(f);
    el_flow_compute_liveness(f, ANALYZE_ALLREG);

    if (El_do_local_operation_folding)
      change += El_opti_local_op_folding(f);

    if (change == 0)
      break;
  }
}

void el_opti_loop_driver(Compound_region* f)
{
  if (El_do_loop_invariant_code_removal) {
    create_local_analysis_info_for_all_hbs_bbs(f);
    el_flow_compute_liveness(f, ANALYZE_ALL);
    El_do_reaching_defs(f, ANALYZE_ALL);
    Reaching_defs_info* rdinfo = get_reaching_defs_info(f);

    el_opti_remove_loop_invariants(f, rdinfo) ;

    remove_reaching_defs_info(f) ;
    delete_local_analysis_info_for_all_hbs_bbs(f);
    el_flow_delete_liveness(f);
  }
  
}

/*
 *	So many wierd and different analyses are required for each
 *	of these opti's, I decided to leave it so each opti just does
 *	all the required analysis it needs.
 */
void el_opti_global_driver(Compound_region* f)
{
  if (El_do_common_subexpression_elimination) {
    el_opti_common_subexpr_eliminate(f);  
    if (El_include_pbr_during_CSE) {
      // CSE must be followed with forward copy propagation and dead-code 
      // elimination if pbr instructions are included in CSE 
      el_opti_forward_copy_propagate(f) ;
      el_opti_eliminate_dead_code(f) ;
    }
  }

  // forward copy propagation and dead-code elimination do not need to 
  // repeat if they were done previously in conjunction with CSE
  if (El_do_forward_copy_propagation &&
	!(El_do_common_subexpression_elimination && El_include_pbr_during_CSE)) {
    el_opti_forward_copy_propagate(f) ;
  }

  if (El_do_dead_code_elimination && !(El_do_common_subexpression_elimination && El_include_pbr_during_CSE)) {
    el_opti_eliminate_dead_code(f) ;
  }
}

/*
 *	Main driver for all optimizations
 */
void el_opti_driver(Compound_region* f)
{
  if (dbg(status,2) || dbg(opti,1))
    cdbg << "*** Elcor optimizer entering warp speed Mr. Sulu *** " << endl;

  /*
   *	CVT elim before any optimizations are done
   */
  El_opti_count_cvts(f, 1);
  if (El_do_global_cvt_elim) {
    El_opti_cvt_elim(f);
    if(El_do_width_analysis)
      do_width_analysis(f, true);
  }

  /*
   *	Local optimizations
   */
  el_opti_local_driver(f);
  /* Local opti cleanup */
  if (El_do_dead_code_elimination)
    el_opti_eliminate_dead_code(f) ;

  /*
   * Loop optimizations
   */
  el_opti_loop_driver(f);

  /*
   * Global optimizations
   */
  el_opti_global_driver(f);

  if (El_do_dead_code_elimination)
    el_opti_eliminate_dead_code(f) ;

  El_opti_count_cvts(f, 2);
  if (dbg(status,2) || dbg(opti,1))
    El_opti_report_cvts();

  if (dbg(status,2) || dbg(opti,1))
    cdbg << "*** Optimizer done, back to your regularly scheduled program *** " << endl;
}

