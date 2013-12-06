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
//      File:           edge_utilities.cpp
//      Authors:        Sadun Anik, Richard Johnson
//      Created:        May 1994
//      Description:    Misc. utilities for the Edge class
//
/////////////////////////////////////////////////////////////////////////////

#include "edge_utilities.h"
#include "region_utilities.h"
#include "opcode_properties.h"
#include "iterators.h"
#include "op.h"
#include "edge.h"

bool is_CONTROL0(Edge* e)
{
  return (e->is_control() && (e->src_port() == CONTROL0));
}

bool is_ctl_dep(Edge* e)
{
  return (e->is_control() && (e->src_port() == CONTROL1));
}

bool is_fall_through(Edge* e)
{
   Edge* tmpe = get_corresponding_CONTROL0_edge(e) ;
   if (tmpe->src_alt() == 0) return true ;
   else return false ;
}

Edge*  get_corresponding_CONTROL0_edge(Edge* e)
{
   if (is_CONTROL0(e)) return(e) ;
   else {
      Op *src_control_switch = NULL, *src_op = NULL ;
      Alt_num s_alt = e->src_alt() ;
      src_op = e->src() ;
      if (is_control_switch(src_op)) {
	 src_control_switch = src_op ;
      } else if (is_switch(src_op)) {
	 src_control_switch = get_relop_of_merge_switch(src_op) ;
      } else {
	 // The query is nonsensical if the source of the non-controlflow
	 // edge is not a switch or a control-switch op
	 assert(0) ;
      }
      // src_control_switch points to the control_switch and s_alt
      // points to the alt number. The next step is finding
      // the C0 edge which fits the description
      Op_outedges opout_i(src_control_switch, CONTROL0_OUTEDGES) ;
      for ( ; (*opout_i)->src_alt() != s_alt ; opout_i++) ;
      return (*opout_i) ;
   }
}

Edge* get_connecting_CONTROL0_edge(Region* src_r, Region* dest_r)
{
   if (src_r->is_op()) {
      for (Op_outedges ei1((Op*)src_r, CONTROL0_OUTEDGES) ; ei1!=0 ; ei1++) {
	 Edge* e = *ei1 ;
	 Region* tmpr = e->dest() ;
	 while ((tmpr != NULL) && (tmpr != dest_r)) {
	    tmpr = tmpr->parent() ;
	 }
	 if (tmpr != NULL) {
	    return (e) ;
	 }
      }
   }
   else {
      for (Region_exit_edges ei1(src_r) ; ei1!=0 ; ei1++) {
	 Edge* e = *ei1 ;
	 Region* tmpr = e->dest() ;
	 while ((tmpr != NULL) && (tmpr != dest_r)) {
	    tmpr = tmpr->parent() ;
	 }
	 if (tmpr != NULL) {
	    return (e) ;
	 }
      }
   }
   return(NULL) ; // This is probably better than asserting 0
}

Edge* get_outgoing_CONTROL0_edge(Op* op)
{
   if(is_merge(op) || is_switch(op)) {
      return(get_outgoing_CONTROL0_edge(get_relop_of_merge_switch(op))) ;
   }
   else {
      Op_outedges e_i(op, CONTROL0_OUTEDGES) ;
      if(e_i == 0)
	return (NULL);
      else
	return(*e_i) ;
   }
}

Edge* get_incoming_CONTROL0_edge(Op* op)
{
   if(is_merge(op) || is_switch(op)) {
      return(get_incoming_CONTROL0_edge(get_relop_of_merge_switch(op))) ;
   }
   else {
      Op_inedges e_i(op, CONTROL0_INEDGES) ;
      if(e_i == 0)
	return (NULL);
      else
	return(*e_i) ;
   }
}

Edge* get_outgoing_CONTROL0_fall_through_edge(Op* op)
{
   if(is_merge(op) || is_switch(op)) {
      return(get_outgoing_CONTROL0_fall_through_edge(get_relop_of_merge_switch(op))) ;
   }
   else {
      for (Op_outedges e_i(op, CONTROL0_OUTEDGES); e_i!=0; e_i++) {
	if (is_fall_through(*e_i))
          return(*e_i) ;
      }
   }
   return (NULL);
}

Edge* get_incoming_CONTROL0_fall_through_edge(Op* op)
{
   if(is_merge(op) || is_switch(op)) {
      return(get_incoming_CONTROL0_fall_through_edge(get_relop_of_merge_switch(op))) ;
   }
   else {
      for (Op_inedges e_i(op, CONTROL0_INEDGES); e_i!=0; e_i++) {
	if (is_fall_through(*e_i))
          return(*e_i) ;
      }
   }
   return (NULL);
}

Edge* get_outgoing_CONTROL0_non_fall_through_edge(Op *op)
{
   if(is_merge(op) || is_switch(op)) {
      return(get_outgoing_CONTROL0_non_fall_through_edge(get_relop_of_merge_switch(op)));
   }
   else {
      for (Op_outedges e_i(op, CONTROL0_OUTEDGES); e_i!=0; e_i++) {
        if (! is_fall_through(*e_i))
          return(*e_i) ;
      }
   }
   return (NULL);
}

Edge* get_incoming_CONTROL0_non_fall_through_edge(Op* op)
{
   if(is_merge(op) || is_switch(op)) {
      return(get_incoming_CONTROL0_non_fall_through_edge(get_relop_of_merge_switch(op)));
   }
   else {
      for (Op_inedges e_i(op, CONTROL0_INEDGES); e_i!=0; e_i++) {
        if (! is_fall_through(*e_i))
          return(*e_i) ;
      }
   }
   return (NULL);
}

Edge *get_exit_edge_to_same_target(Compound_region *r, Edge *e)
{
    Op *target_op;
    Edge *edge;

    target_op = e->dest();
    for (Region_exit_edges edge_i(r); edge_i!=0; edge_i++) {
	edge = *edge_i;
	if (edge == e)
	    continue;
	if (edge->dest() == target_op)
	    return (edge);
    }

    return (NULL);
}

void redirect_edge_to(Op *oldop, Op* newop, Edge *edge)
{
    Op *dst_op;

    dst_op = edge->dest();
    if (dst_op != oldop)
	El_punt("redirect_edge_to: edge does not go to %d", oldop->id());

    oldop->remove_inedge(edge, edge->dest_port_type(), edge->dest_port(),
			edge->dest_alt());
    newop->add_inedge(edge, edge->dest_port_type(), edge->dest_port(),
			edge->dest_alt());
    edge->set_dest(newop, edge->dest_port(), edge->dest_alt());
}

void redirect_edge_from(Op *oldop, Op* newop, Edge *edge)
{
    Op *src_op;

    src_op = edge->src();
    if (src_op != oldop)
	El_punt("redirect_edge_from: edge does not come from %d", oldop->id());

    oldop->remove_outedge(edge, edge->src_port_type(), edge->src_port(),
			edge->src_alt());
    newop->add_outedge(edge, edge->src_port_type(), edge->src_port(),
			edge->src_alt());
    edge->set_src(newop, edge->src_port(), edge->src_alt());
}

