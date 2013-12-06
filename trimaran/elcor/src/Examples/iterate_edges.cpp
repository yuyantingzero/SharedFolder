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
//      File:           iterate_edges.cpp
//      Author:         Nate Clark
//      Created:        August 2006
//      Description:    Examples of how to iterate through control/dataflow
//                      edges in the IR.
//
/////////////////////////////////////////////////////////////////////////////


#include "iterate_edges.h"

// IR definitions, including the Procedure class.
#include "region.h"

// Edge and Op classes
#include "edge.h"
#include "op.h"

// This has all the edge iterators.
#include "iterators.h"

// Definition of 'cdbg', elcor's debug output stream
#include "dbg.h"

// Analysis routines
#include "pred_analysis.h"
#include "edge_drawing.h"

// For is_add()
#include "opcode_properties.h"



void iterate_edges_example(Procedure* p)
{
  cdbg << "************************************************************\n"
       << "* Starting edge iteration example\n"; 

  // First we need to do some analysis and draw edges in the
  // procedure. Note that we did not perform liveness analysis on this
  // procedure, so the only edges inserted are intra-block.

  create_local_analysis_info_for_all_hbs_bbs(p);
  insert_region_scalar_edges(p);


  // Iterate through the ops in this procedure
  for(Region_all_ops rops(p); rops != 0; rops++) {
    Op* the_op = *rops;

    // Examine the dataflow edges
    for(Op_outedges oe(the_op, FLOW_EDGES); oe != 0; oe++) {
      Edge* edge = *oe;
      cdbg << "There's a dataflow edge from port "
	   << port_type_to_text(edge->src_port_type())
	   << edge->src_port() << " in op " << edge->src()->id() << " to port "
	   << port_type_to_text(edge->dest_port_type()) << edge->dest_port()
	   << " in op " << edge->dest()->id() << endl;
      cdbg << "   Its latency is " << edge->latency() << endl;
    }

    // Or examine everything except dataflow edges
    for(Op_inedges oe(the_op); oe != 0; oe++) {
      Edge* edge = *oe;
      if( ! edge->is_reg_flow()) {
	cdbg << "Some other type of edge is coming into op " << the_op->id() << endl;
	// Only print once, even if there are multiple edges.
	break;
      }
    }

    // Or examing the memory edges between ops.
    // There is a memory dependence edge between op A and op B if 
    //   (1) op A and op B are load or store or function call (BRL) operations
    //   (2) op A and op B could access the same memory location. 
    //      The the set of addresses access by a BRL operation is the union of
    //      all memory address accessed in the callee function.
    //      The memory alias analysis is conservative.  If the compiler can't figure out if two 
    //      memory operation access 
    //      the same location, it will conservatively assume they are aliased.
    //      The analysis is performed in openimpact. The memory dependence 
    //      information are passed to elcor.
    //      the same address.
    //   (3) Op A and op B can't both be loads.
    //      If two load operations access the same memory address,
    //      there is not edge between them.
    for(Op_outedges oe(the_op, MEM_EDGES); oe != 0; oe++) {
      Edge* edge = *oe;
      cdbg << "There's a memory edge from op "
	   << edge->src()->id() << " to op "
	   << edge->dest()->id() << endl;
    }
    

    // If an op is commutative, we can switch the operands without
    // changing the program semantics. However, doing this screws up
    // the edges. After moving operands we have to either manually
    // adjusting the edges, or delete and redraw them. The code below
    // adjusts the edges manually.

    // Add is a convenient commutative op.
    if(is_add(the_op)) {
      // Make sure there's only 2 sources (sanity check) and that we
      // aren't swapping literals. Depending on the MDES, moving
      // literals could create operations which are unschedulable.
      if(the_op->num_srcs() == 2 &&
	 ! the_op->src(SRC1).is_lit() && ! the_op->src(SRC2).is_lit()) {
	cdbg << "Swapping the operands of op " << the_op->id() << endl;
	cdbg << "Before: " << *the_op << endl;

	// The edges that need to be moved
	List<Edge*> to_src_1;
	List<Edge*> to_src_2;

	for(Op_inedges ie(the_op, NON_CONTROL0_INEDGES); ie != 0; ie++) {
	  Edge* ed = *ie;
	  if(ed->dest_port() == SRC1)
	    to_src_2.add_tail(ed);
	  else if(ed->dest_port() == SRC2)
	    to_src_1.add_tail(ed);
	}

	// Move the edges
	for(List_iterator<Edge*> li(to_src_1); li != 0; li++) {
	  Edge* ed = *li;
	  the_op->remove_inedge(ed, ed->dest_port_type(), ed->dest_port(),
				ed->dest_alt());
	  ed->set_dest(the_op, SRC1, ed->dest_alt());
	  the_op->add_inedge(ed, ed->dest_port_type(), ed->dest_port(), 
			     ed->dest_alt());
	}
	for(List_iterator<Edge*> li(to_src_2); li != 0; li++) {
	  Edge* ed = *li;
	  the_op->remove_inedge(ed, ed->dest_port_type(), ed->dest_port(),
				ed->dest_alt());
	  ed->set_dest(the_op, SRC2, ed->dest_alt());
	  the_op->add_inedge(ed, ed->dest_port_type(), ed->dest_port(),
			     ed->dest_alt());
	}

	// Switch the operands
	Operand tmp_oprnd = the_op->src(SRC1);
	the_op->set_src(SRC1, the_op->src(SRC2));
	the_op->set_src(SRC2, tmp_oprnd);

	cdbg << "After: " << *the_op << endl;
      }
    }
  }

  delete_region_edges(p);


  // Now we'll do liveness analysis. This marks which registers are
  // used across block boundaries. Edge drawing will now insert more
  // edges (to D_SWITCHes and from D_MERGEs) that represent operands
  // coming from or writing to operations outside a block.

  el_flow_compute_liveness(p, ANALYZE_ALLREG);
  insert_region_scalar_edges(p);

  cdbg << "\nStarting second pass (with Liveness analysis this time)." << endl;
  for(Region_all_ops rops(p); rops != 0; rops++) {
    Op* the_op = *rops;

    // Examine the dataflow edges
    for(Op_outedges oe(the_op, FLOW_EDGES); oe != 0; oe++) {
      Edge* edge = *oe;
      if(is_merge(edge->src()) || is_switch(edge->dest())) {
	cdbg << "There's an interblock dataflow edge from port "
	     << port_type_to_text(edge->src_port_type())
	     << edge->src_port() << " in op " << edge->src()->id() << " to port "
	     << port_type_to_text(edge->dest_port_type()) << edge->dest_port()
	     << " in op " << edge->dest()->id() << endl;
	cdbg << "   Its latency is " << edge->latency() << endl;
      }
    }
  }

  delete_region_edges(p);

  // This will also remove liveness analysis.
  delete_local_analysis_info_for_all_hbs_bbs(p);
}
