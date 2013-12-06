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
//      File:           slack_computation.cpp
//      Author:         Nate Clark
//      Created:        August 2006
//      Description:    Examples of how to calculate estart/lstart/slack
//                      on the IR.
//
/////////////////////////////////////////////////////////////////////////////


#include "slack_computation.h"

// IR definitions, including the Procedure class.
#include "region.h"

// Definition of 'cdbg', elcor's debug output stream
#include "dbg.h"

// Analysis routines
#include "pred_analysis.h"
#include "edge_drawing.h"

// Functions for calculating dependence height.
#include "dep_len.h"


void slack_computation_example(Procedure* p)
{
  cdbg << "************************************************************\n"
       << "* Starting slack calculation example\n"; 

  // Need to draw edges in order for slack calculation to determine
  // the dependencies.
  create_local_analysis_info_for_all_hbs_bbs(p);
  insert_region_scalar_edges(p);

  
  // Slack can only be computed on a basic/hyper block, so recurse
  // down to the block level of the IR.
  List<Compound_region*> regions_to_process;
  regions_to_process.add_tail(p);
  while( ! regions_to_process.is_empty()) {
    Compound_region* reg = regions_to_process.pop();

    // Region is not a block, so recurse to the next level of IR
    // hierarchy.
    if( ! reg->is_bb() && ! reg->is_hb()) {
      for(Region_subregions subr(reg); subr != 0; subr++) {
	regions_to_process.add_tail( (Compound_region*) *subr);
      }
    } else {
      cdbg << "Analyzing Block " << reg->id() << endl;

      // We have a block. Now calculate estart/lstart/slack. First
      // create a Hash_map to store the results for each op.
      Hash_map<Op*, DepLen_OpInfo*> dinfo(hash_op_ptr, 571);

      // Now fill the Hash_map in.
      DepLen::get_estart(reg, dinfo);
      DepLen::get_lstart(reg, dinfo);
      DepLen::get_slack(reg, dinfo);

      // Iterate through the ops, and print the results. Notice how
      // Lstart and Slack are reported as vectors. This is because a
      // hyperblock can have multiple exits; each entry in the array
      // corresponds to an exit.
      for(Region_all_ops rops(reg); rops != 0; rops++) {
	Op* the_op = *rops;
	DepLen_OpInfo* op_info = dinfo[the_op];

	cdbg << "  Op " << the_op->id() << "  eStart: " << op_info->m_Estart
	     << "  lStart: ";
	for(unsigned i = 0; i < op_info->m_Lstart.size(); i++) {
	  cdbg << op_info->m_Lstart[i] << " "; 
	}
	cdbg << "  Slack: ";
	for(unsigned i = 0; i < op_info->m_Slack.size(); i++) {
	  cdbg << op_info->m_Slack[i] << " "; 
	}
	cdbg << endl;
      }

      // The DepLen analysis creates memory but does not free it, so
      // we must explicitly destory it. I know this is a horrible
      // programming practice. I didn't write the code. You're welcome
      // to fix it and send us a patch.

      for(Hash_map_iterator<Op*, DepLen_OpInfo*> hmi(dinfo); hmi != 0; hmi++) {
	DepLen_OpInfo* to_delete = (*hmi).second;
	delete to_delete;
      }
      
    }
  }  

  // Remove analysis and edges from the procedure.

  delete_region_edges(p);
  delete_local_analysis_info_for_all_hbs_bbs(p);  
}

