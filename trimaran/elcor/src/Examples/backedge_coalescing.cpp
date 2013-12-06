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
//      File:           backedge_coalescing.cpp
//      Author:         Amir H Hormati
//      Created:        August 2006
//      Description:    Example of how to combine more than one backedge in 
//                      a loop.
//
/////////////////////////////////////////////////////////////////////////////
#include "el_control.h"
#include "el_loop.h"
#include "el_loop_iterators.h"
#include "region.h"
#include "bb_utilities.h"
#include "edge_utilities.h"
#include "el_bb_tools.h"

void combine_loop_backedges(Procedure *, El_Loop *);

void backedge_coalescing_example(Procedure *f)
{
  cdbg << "************************************************************\n"
       << "* Starting backedge coalescing example\n"; 

  // Loop detection
  // The Loop Graph constructor calls dominator analysis, performs loop
  // detection, and builds a graph consisting of all the loops it found. 
  El_Loop_Graph all_loops(f); 

  // Iterate through all the innermost loops */
  for (El_Loop_Graph_Innermost loopi(all_loops); loopi!=0; loopi++) {
    El_Loop *loop = *loopi;
    cdbg << "Found an innermost loop" << endl;
    cdbg << "\theader = " << loop->header_block->id() << endl;
    cdbg << "\tblocks = ";
    for (List_set_iterator<Basicblock *> lsi(loop->loop_blocks); lsi != 0; lsi++)
    {
      Basicblock *bb = *lsi;
      cdbg << bb->id() << ", ";
    }
    cdbg << endl;
    
    cdbg << "\tbackedge blocks = ";
    for (Hash_set_iterator<Basicblock*> hsi(loop->back_edge_blocks); hsi!=0; hsi++)
    {
      Basicblock *bb = *hsi;
      cdbg << bb->id() << ", ";
    }
    cdbg << endl;

    combine_loop_backedges(f, loop);
    cdbg << " Backedge coalescing finished for this loop. Going to next Loop... " << endl;
  }
  // Loop detection do bb tiling, so we have to
  // undo that.
  El_remove_basic_blocks(f);
}

void combine_loop_backedges(Procedure *f, El_Loop *loop)
{
  if (loop->back_edge_blocks.size() == 1)
  {
    cdbg << "This loop has one back-edge." << endl;
    return;
  }

  double weight = 0;

  Basicblock *loop_header = loop->header_block;
  Hash_set<Edge *> redirect_edges(hash_edge_ptr, 16);

  // This loop find all of the backedges and add them to redirect_edges Hash_set
  for (Hash_set_iterator<Basicblock*> hsi(loop->back_edge_blocks); hsi!=0; hsi++)
  {
    Basicblock *bb = *hsi; 
    for (Region_exit_edges rei(bb); rei != 0; rei++)
    {
      if (loop_header->is_entry_edge(*rei))
      {
        redirect_edges += *rei;

        // We need to know the total weight for the all of backedges, in order to 
        // set the weight of new backedge.
        weight += get_control_flow_freq(*rei)->freq;
      }
    }
  }

  // Create a new basic block, and insert it into the end of the Procedure f
  Basicblock *new_bb = new Basicblock;
  new_bb->weight = weight;
  f->add_region(new_bb);
  new_bb->set_parent(f);
  cdbg << "New empty block(id = " << new_bb->id() << ") was created. " << endl;
  
  // All BBs start with a CONTROL_MERGE pseudo op, so create one and add to new_bb
  Op* merge_op = new Op(C_MERGE);
  new_bb->add_region(merge_op);
  merge_op->set_parent(new_bb);
  new_bb->add_entry(merge_op);
  cdbg << "New block after adding C_MERGE:\n " << *new_bb;
  
  // Add an unconditional branch to new_BB to target target_op.  Note control
  // flow edges connect 2 operations, a branch and a C_MERGE.  The following function
  // creates the PBR and the BRU ops, adds them to new_bb, and hooks up all the
  // necessary control flow edges. It also sets the weight of the new edge.
  // See Control/el_control.cpp
  Op* target_op = get_first_bb_op(loop_header);
  El_add_jump_to_region(new_bb, target_op);
  
  cdbg << "New block after adding jmp to header block:\n " << *new_bb;

  for (Hash_set_iterator<Edge*> hsi(redirect_edges); hsi!=0; hsi++) {
    Edge *e = *hsi;
    cdbg << "Redirecting edge (" << e->src()->parent()->id() << " ---> "
         << e->dest()->parent()->id() << ") to "  << new_bb->id() << endl;

    // This function changes destination of edges
    el_reconnect_edge_dest(e, new_bb);
  }
  
  // You'll note in the prior code that the PBR source operand was
  // not changed to reflect the new branch target.  If you set all the
  // control flow edges correctly, then you can call the following function
  // to change the PBR operands automatically.
  El_fix_all_pbr_info(f, false);
}
