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
//      File:           el_opti_forward_copy_prop.cpp                    //
//      Authors:        Sadun Anik					 //
//      Created:        July 1996					 //
//      Description:    Dead Code Eliminator Implementation		 //
// 									 //
///////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "el_opti_forward_copy_prop.h"
#include "reaching_defs_solver.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "el_available_expression.h"
#include "local_copy_propagate.h"
#include "el_opti_utilities.h"
#include "el_bb_tools.h"

void el_opti_forward_copy_propagate(Compound_region* R)
{
  if (dbg(opti,1) || dbg(status, 3))
	cdbg << "-- Global forward copy propagation " << endl;

   create_local_analysis_info_for_all_hbs_bbs(R) ;
  
   Region_all_ops reg_opi ;
   List<Op*> copy_op_list ;
   for (reg_opi(R) ; reg_opi != 0 ; reg_opi++) {
      Op* cur_op = *reg_opi ;
      if (is_copy(cur_op)) {
         copy_op_list.add_tail(cur_op) ;
      }
   }
   // Use available expressions to see if the right hand side
   // of the copies get clobbered before the uses of the 
   // left hand sides
   
   // use_normalized_expr flag is set FALSE
   // see Analysis/el_available_expression.cpp for more information
   Available_expression_solver avexp(R, AV_EXPR_UNARY_OR_BINARY, false) ;
   avexp.solve() ;

   // for each use of a destination of a copy, check if the 
   // copy is the only source
   // if this is true then check if the RHS of copy reaches
   // the use. 
   // If both are true replace the RHS of the use with RHS of copy

   El_do_reaching_defs (R, ANALYZE_ALLREG) ;
   Reaching_defs_info* rdinfo = get_reaching_defs_info (R) ;

   for (List_iterator<Op*> opi(copy_op_list); opi != 0 ; opi++) {
      Op* cur_copy_op = *opi ;
      for (Op_explicit_dests di(cur_copy_op) ; di != 0 ; di++) {
         El_ref cur_def = di.get_ref() ;
         List<El_ref>* uses_list = rdinfo->get_du_chain(cur_def) ;
         for (List_iterator<El_ref> usei(*uses_list) ; usei != 0 ; usei++) {
            El_ref& cur_use = *usei ;
            El_ref_type t = cur_use.get_ref_type() ;
	    // check if copy is the only def that reaches
	    List<El_ref>* use_defs = rdinfo->get_ud_chain(cur_use) ;
	    if (use_defs->size() > 1) continue ;
	    // can't copy propagate implicit sources !
            if ((t == EXP_SRC) || (t == PRED_SRC)) {
               Op* cur_use_op = cur_use.get_op() ;
               Compound_region* cur_reg = get_enclosing_analysis_scope(cur_use_op) ;
               Pred_cookie copy_available_cond = 
                  avexp.get_available_expression_cookie_before_op(cur_reg, 
                  cur_copy_op, cur_use_op) ;
               Pred_jar pj(cur_reg) ;
               Pred_cookie use_cond = pj.get_lub_guard(cur_use) ;
               if (use_cond.is_subset(copy_available_cond)) {
                  cur_use_op->src(cur_use.get_port_num()) = cur_copy_op->src(SRC1) ;
		  
                  if (copy_op_list.is_member(cur_use_op)) {
                     copy_op_list.remove(cur_use_op) ;
                  }
		  if (dbg (opti, 3)) {
		     cdbg << "\tCopy propagate" << cur_copy_op->id() ;
		     cdbg << " to " <<  cur_use_op->id() << "\n" ;
		  }
               }
            }
         }
      }  
   }
   
   delete_local_analysis_info_for_all_hbs_bbs(R) ; 
}



