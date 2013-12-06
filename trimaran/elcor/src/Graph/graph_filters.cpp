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
//      File:           graph_filters.cpp
//      Authors:        Vinod Kathail, Sadun Anik, Santosh Abraham
//      Created:        May 1995
//      Description:    Filters for IR data structures
//
/////////////////////////////////////////////////////////////////////////////

#include "graph_filters.h"
#include "op.h"
#include "edge.h"
#include "region.h"
#include "region_utilities.h"
#include "opcode_properties.h"

/* const Operand_filter* ALL_OPERANDS = new Operand_filter; */
const Edge_filter* ALL_EDGES = new Edge_filter;
const Op_filter* ALL_OPS = new Op_filter;
const Region_filter* ALL_REGIONS = new Region_filter;

/* Filtering Edge types */
bool Flow_edge_filter::filter(Edge* const & t) const{
  return (t->is_reg_flow());
}

bool Anti_edge_filter::filter(Edge* const & t) const{
  return (t->is_reg_anti());
}

bool Outdep_edge_filter::filter(Edge* const & t) const{
  return (t->is_reg_out());
}

bool Mem_edge_filter::filter(Edge* const & t) const{
  return (t->is_mem());
}

/* -------------------------- Filter constants ------------------------- */
const Edge_filter* FLOW_EDGES   = new Flow_edge_filter();
const Edge_filter* ANTI_EDGES   = new Anti_edge_filter();
const Edge_filter* OUTDEP_EDGES = new Outdep_edge_filter();
const Edge_filter* MEM_EDGES    = new Mem_edge_filter();
const Edge_filter* REAL_EDGES   = new Real_edge_filter();

/* Class Inedge_port_filter */

Inedge_port_filter::Inedge_port_filter(Port_type type)
  :type_rep(type), num_rep(-1), alt_rep(-1) {}

Inedge_port_filter::Inedge_port_filter(Port_type type, Port_num num)
  :type_rep(type), num_rep(num), alt_rep(-1) {}

Inedge_port_filter::Inedge_port_filter(Port_type type, Port_num num, 
				       Alt_num alt)
  :type_rep(type), num_rep(num), alt_rep(alt) {}

Inedge_port_filter::~Inedge_port_filter(){}

bool Inedge_port_filter::filter(Edge* const & t) const{ 
  return ((t->dest_port_type() == type_rep) &&
	    (((Port_num) num_rep == -1) || (t->dest_port() == num_rep)) &&
	    ((alt_rep == -1) || (t->dest_alt() == alt_rep)));
}
/* ---------------------- Useful constants ---------------------------------- */
const Inedge_port_filter* CONTROL0_INEDGES = 
                         new Inedge_port_filter(CONTROL, CONTROL0);
const Edge_filter* NON_CONTROL0_INEDGES =
			new Filter_not <Edge*> (CONTROL0_INEDGES, false);
const Inedge_port_filter* CONTROL_INEDGES = 
                         new Inedge_port_filter(CONTROL);
const Edge_filter* NON_CONTROL_INEDGES =
			new Filter_not <Edge*> (CONTROL_INEDGES, false);

const Inedge_port_filter* CONTROL1_INEDGES = 
                         new Inedge_port_filter(CONTROL, CONTROL1);
const Edge_filter* NON_CONTROL1_INEDGES =
			new Filter_not <Edge*> (CONTROL1_INEDGES, false);

/*  Class Outedge_port_filter */

Outedge_port_filter::Outedge_port_filter(Port_type type)
  :type_rep(type), num_rep(-1), alt_rep(-1) {}

Outedge_port_filter::Outedge_port_filter(Port_type type, Port_num num)
  :type_rep(type), num_rep(num), alt_rep(-1) {}

Outedge_port_filter::Outedge_port_filter(Port_type type, Port_num num, 
				       Alt_num alt)
  :type_rep(type), num_rep(num), alt_rep(alt) {}

Outedge_port_filter::~Outedge_port_filter(){}

bool Outedge_port_filter::filter(Edge* const & t) const{ 
  return ((t->src_port_type() == type_rep) &&
	  ((t->src_port() == num_rep) || ((Port_num) num_rep == -1)) &&
	  ((alt_rep == -1) || (t->src_alt() == alt_rep)));
}

/* ---------------------- Useful constants ---------------------------------- */
const Outedge_port_filter* CONTROL0_OUTEDGES = 
                         new Outedge_port_filter(CONTROL, CONTROL0);
const Edge_filter* NON_CONTROL0_OUTEDGES =
			new Filter_not <Edge*> (CONTROL0_OUTEDGES, false);
const Outedge_port_filter* CONTROL_OUTEDGES = 
                         new Outedge_port_filter(CONTROL);
const Edge_filter* NON_CONTROL_OUTEDGES =
			new Filter_not <Edge*> (CONTROL_OUTEDGES, false);

const Outedge_port_filter* CONTROL1_OUTEDGES = 
                         new Outedge_port_filter(CONTROL, CONTROL1);
const Edge_filter* NON_CONTROL1_OUTEDGES =
			new Filter_not <Edge*> (CONTROL1_OUTEDGES, false);

////////////////////////////////////////////////////////////////////////
//  Filter to filter according to parent of src of an edge
////////////////////////////////////////////////////////////////////////

Parent_of_src_filter::Parent_of_src_filter(Region* p)
   : filter_parent(p) {}

Parent_of_src_filter::~Parent_of_src_filter() {}

bool Parent_of_src_filter::filter(Edge* const & e) const
{
   Region* r_src = e->src() ;
   while (r_src != NULL) {
      if (r_src->parent() == filter_parent) return true ;
      else r_src = r_src->parent() ;
   }
   return false ;
}

////////////////////////////////////////////////////////////////////////
//  Filter to filter according to parent of dest of an edge
////////////////////////////////////////////////////////////////////////

Parent_of_dest_filter::Parent_of_dest_filter(Region* p)
   : filter_parent(p) {}

Parent_of_dest_filter::~Parent_of_dest_filter() {}

bool Parent_of_dest_filter::filter(Edge* const & e) const
{
   Region* r_dest = e->dest() ;
   while (r_dest != NULL) {
      if (r_dest->parent() == filter_parent) return true ;
      else r_dest = r_dest->parent() ;
   }
   return false ;

}

////////////////////////////////////////////////////////////////////////
//  Filter to filter edges contained within a region
////////////////////////////////////////////////////////////////////////

Edge_is_contained_in_region_filter::Edge_is_contained_in_region_filter(Region* r)
   : reg_ptr(r) {}

Edge_is_contained_in_region_filter::~Edge_is_contained_in_region_filter() {}

bool Edge_is_contained_in_region_filter::filter(Edge* const & e) const
{
   return(region_contains_edge(reg_ptr, e)) ;
}


////////////////////////////////////////////////////////////////////////
//  Filter to filter ops contained within a region
////////////////////////////////////////////////////////////////////////

Op_is_contained_in_region_filter::Op_is_contained_in_region_filter(Region*
   r) : reg_ptr(r) {}

Op_is_contained_in_region_filter::~Op_is_contained_in_region_filter() {}

bool Op_is_contained_in_region_filter::filter(Op* const & op) const
{
   return(region_contains_op(reg_ptr, op)) ;
}

// ------------- Class Same_src_alt_filter --------------------------	  
	
Same_src_alt_filter::Same_src_alt_filter(Edge* edge)
        : alt_num(edge->src_alt()) {}

Same_src_alt_filter::~Same_src_alt_filter() {}

bool Same_src_alt_filter::filter(Edge* const& edge) const
{
  return (edge->src_alt() == alt_num);
}



// ------------- Class Same_dest_alt_filter --------------------------	  
	
Same_dest_alt_filter::Same_dest_alt_filter(Edge* edge)
        : alt_num(edge->dest_alt()) {}

Same_dest_alt_filter::~Same_dest_alt_filter() {}

bool Same_dest_alt_filter::filter(Edge* const& edge) const
{
  return (edge->dest_alt() == alt_num);
}

/* ----------------------------- Filters for regions -----------------------
   Filter for HB or BB
   Filter for loops 
   Filter for loops, HB or BB
----------------------------------------------------------------------------*/

HB_or_bb_filter::HB_or_bb_filter() {}
HB_or_bb_filter::~HB_or_bb_filter() {}
bool HB_or_bb_filter::filter(Region* const & region) const 
{ 
    return (region->is_hb() || region->is_bb());
}

Loopbody_filter::Loopbody_filter() {}
Loopbody_filter::~Loopbody_filter() {}
bool Loopbody_filter::filter(Region* const & region) const 
{ 
    return (region->is_loopbody());
}

Scheduled_filter::Scheduled_filter() {}
Scheduled_filter::~Scheduled_filter() {}
bool Scheduled_filter::filter(Region* const & region) const 
{ 
    return (region->flag(EL_REGION_SCHEDULED));
}

/* ---------------------- Useful constants---------------------------------- */
const Region_filter* HB_BB_FILTER = new HB_or_bb_filter;
const Region_filter* LOOPBODY_FILTER = new Loopbody_filter;
const Region_filter* LOOPBODY_HB_BB_FILTER = 
    new Filter_or<Region*>(LOOPBODY_FILTER, HB_BB_FILTER); 
const Region_filter* SCHED_FILTER = 
    new Scheduled_filter;
const Region_filter* HB_BB_SCHED_FILTER = 
    new Filter_or<Region*>(HB_BB_FILTER, SCHED_FILTER); 

// ---------------------- Filters used by ModuloScheduler -------------------------  
bool Real_edge_filter::filter(Edge * const & edge) const
{ 
  return !(is_pseudo(edge->src()) || is_pseudo(edge->dest())
	   // is_control should really be is_seq but for the time being
	   // we ignore all control edges rather than just C0 edges.
	   // This was because spurious C1 edges were being inserted 
	   // in edge drawing. -- Shail + Vinod 08/16/96
	   //	   || edge->is_seq());
	   || edge->is_control());
}

bool Real_op_filter::filter(Op * const & op) const
{
  return !(is_pseudo(op));
}
