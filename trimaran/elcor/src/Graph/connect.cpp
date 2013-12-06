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
//      File:           connect.cpp
//      Authors:        Sadun Anik, Scott Mahlke, Santosh Abraham,
//                      Vinod Kathail, Dave Agust
//      Created:        December 1994
//      Description:    Utilities to wire and rewire the graph
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "connect.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "edge.h"
#include "op.h"

using namespace std;

void connect_ops(Edge* edge, Op* src_op, Port_num src_port, 
                         Op* dest_op, Port_num dest_port,
                         Alt_num src_alt, Alt_num dest_alt) {
  edge->set_src(src_op, src_port, src_alt);
  edge->set_dest(dest_op, dest_port, dest_alt);
  src_op->add_outedge(edge, edge->src_port_type(), src_port, src_alt);
  dest_op->add_inedge(edge, edge->dest_port_type(), dest_port, dest_alt);
}

Edge* C0_connect(Op* src_op, Op* dest_op) // connect ops with CONTROL0 edge
{
   Alt_num src_alt,dest_alt ;
   
   src_alt = get_new_out_alt_num(src_op) ;
   dest_alt = get_new_in_alt_num(dest_op) ;
   Edge* edge = new Control() ;
   edge->set_src(src_op, CONTROL0, src_alt);
   edge->set_dest(dest_op, CONTROL0, dest_alt);
   src_op->add_outedge(edge, CONTROL, CONTROL0, src_alt);
   dest_op->add_inedge(edge, CONTROL, CONTROL0, dest_alt);
   return edge ;
}

Edge* C0_connect_fallthrough(Op* src_op, Op* dest_op) 
{
   Edge* edge = new Control() ;
   edge->set_src(src_op, CONTROL0, 0);
   edge->set_dest(dest_op, CONTROL0, 0);
   src_op->add_outedge(edge, CONTROL, CONTROL0, 0);
   dest_op->add_inedge(edge, CONTROL, CONTROL0, 0);
   return edge ;
}

Edge* C0_connect_taken(Op* src_op, Op* dest_op) 
{
   Edge* edge = new Control() ;
   edge->set_src(src_op, CONTROL0, 1);
   edge->set_dest(dest_op, CONTROL0, 1);
   src_op->add_outedge(edge, CONTROL, CONTROL0, 1);
   dest_op->add_inedge(edge, CONTROL, CONTROL0, 1);
   return edge ;
}

Edge* C0_connect_insert_between(Op* src_op, Op* dest_op, Op* new_op)
{
   Edge* old_edge = get_connecting_CONTROL0_edge(src_op, dest_op) ;
   dest_op->remove_inedge(old_edge, CONTROL, CONTROL0, 0);
   old_edge->set_dest(new_op, CONTROL0, 0);
   new_op->add_inedge(old_edge, CONTROL, CONTROL0, 0);

   Edge* new_edge = new Control() ;
   new_edge->set_src(new_op, CONTROL0, 0);
   new_edge->set_dest(dest_op, CONTROL0, 0);
   new_op->add_outedge(new_edge, CONTROL, CONTROL0, 0);
   dest_op->add_inedge(new_edge, CONTROL, CONTROL0, 0);
   return new_edge ;
}

Op* C0_remove_op(Op* bad_op)
{
   // SAM 6-97, Why is this asserting here???  Commented it out.
   // assert(!is_control_merge(bad_op)) ;
   Edge* src_edge = get_incoming_CONTROL0_edge(bad_op) ;
   Edge* dest_edge = get_outgoing_CONTROL0_edge(bad_op) ;
   bad_op->remove_outedge(dest_edge, CONTROL, CONTROL0, 0);
   bad_op->remove_inedge(src_edge, CONTROL, CONTROL0, 0);
   Op* dest_op = dest_edge->dest();
  
   dest_op->remove_inedge(dest_edge, CONTROL, CONTROL0, 0);
   src_edge->set_dest(dest_op, CONTROL0, 0);
   dest_op->add_inedge(src_edge, CONTROL, CONTROL0, 0);

   delete dest_edge;
  
   return dest_op ;
}

Edge* C1_connect(Op* src_op, Op* dest_op) // connect ops with CONTROL1 edge
{
   Alt_num src_alt,dest_alt ;

// Choose dest_alt and src_alt that are one more than current alt_nums
// Cannot use get_new_{in/out}_alt_num because they are specific
//    to CONTROL0 edges
   
   src_alt = -1 ;
   for (Op_outedges outedge_i(src_op, CONTROL, CONTROL1) ; outedge_i!= 0; outedge_i++) {
      int cur_alt = (*outedge_i)->src_alt() ;
      src_alt = ELCOR_MAX(src_alt,cur_alt) ;
   }
   src_alt++;
   
   dest_alt = -1 ;
   for (Op_inedges inedge_i(dest_op, CONTROL, CONTROL1) ; inedge_i!= 0; inedge_i++) {
      int cur_alt = (*inedge_i)->dest_alt() ;
      dest_alt = ELCOR_MAX(dest_alt,cur_alt) ;
   }
   dest_alt++;
   
   Edge* edge = new Control() ;
   edge->set_src(src_op, CONTROL1, src_alt);
   edge->set_dest(dest_op, CONTROL1, dest_alt);
   src_op->add_outedge(edge, CONTROL, CONTROL1, src_alt);
   dest_op->add_inedge(edge, CONTROL, CONTROL1, dest_alt);
   return edge ;
}
