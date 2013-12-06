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
//      File:           graph_filters.h
//      Authors:        Vinod Kathail, Sadun Anik, Santosh Abraham
//      Created:        May 1995
//      Description:    Filters for IR data structures
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _GRAPH_FILTERS_
#define _GRAPH_FILTERS_

#include "defs.h"
#include "filter.h" 
#include "el_port.h"
#include "edge.h"

class Region;
class Op;
class Operand;
/* --------- Operand_filter should be used as the base to derive all 
             filters for operands */

typedef Filter<Operand> Operand_filter;
extern const Operand_filter* ALL_OPERANDS;  // True for all operands

/* --------- Edge_filter should be used as the base to derive all 
             filters for edges */
 
typedef Filter<Edge*> Edge_filter;
extern const Edge_filter* ALL_EDGES;  // True for all edges 


/* --------- OP_filter should be used as the base to derive all 
             filters for edges */
 
typedef Filter<Op*> Op_filter;
extern const Op_filter* ALL_OPS;  // True for all ops

/* --------- Region_filter should be used as the base to derive all 
             filters for edges */
 
typedef Filter<Region*> Region_filter;
extern const Region_filter* ALL_REGIONS;  // True for all regions

/* Filters to filter on edge types */

// Filter to filter in only flow-dependent edges
class Flow_edge_filter : public Edge_filter {
public:
  bool filter(Edge* const & ) const;
};

// Filter to filter in only anti-dependent edges
class Anti_edge_filter : public Edge_filter {
public:
  bool filter(Edge* const & ) const;
};

// Filter to filter in only output-dependent edges
class Outdep_edge_filter : public Edge_filter {
public:
  bool filter(Edge* const & ) const;
};

// Filter to filter in only mem edges
class Mem_edge_filter : public Edge_filter {
public:
  bool filter(Edge* const & ) const;
};

/* ------------------------------ Filter constants ------------------------- */
extern const Edge_filter* FLOW_EDGES;
extern const Edge_filter* ANTI_EDGES;
extern const Edge_filter* OUTDEP_EDGES;
extern const Edge_filter* MEM_EDGES;
extern const Edge_filter* REAL_EDGES;

/* -------- Filter to filter on port properties for inedges */

class Inedge_port_filter : public Edge_filter {
public:
  Inedge_port_filter(Port_type type);
  Inedge_port_filter(Port_type type, Port_num num);
  Inedge_port_filter(Port_type type, Port_num num, Alt_num alt);
  ~Inedge_port_filter();
  bool filter(Edge* const & ) const;
private:
  Port_type type_rep;
  Port_num num_rep;
  Alt_num alt_rep;
};
/* ---------------------- Useful constants ---------------------------------- */
extern const Inedge_port_filter* CONTROL0_INEDGES;
extern const Edge_filter* NON_CONTROL0_INEDGES;
extern const Inedge_port_filter* CONTROL_INEDGES;
extern const Edge_filter* NON_CONTROL_INEDGES;

extern const Inedge_port_filter* CONTROL1_INEDGES;
extern const Edge_filter* NON_CONTROL1_INEDGES;

/* -------- Filter to filter on port properties for outedges*/

class Outedge_port_filter : public Edge_filter {
public:
  Outedge_port_filter(Port_type type);
  Outedge_port_filter(Port_type type, Port_num num);
  Outedge_port_filter(Port_type type, Port_num num, Alt_num alt);
  ~Outedge_port_filter();
  bool filter(Edge* const & ) const;
private:
  Port_type type_rep;
  Port_num num_rep;
  Alt_num alt_rep;
};
/* ---------------------- Useful constants ---------------------------------- */
extern const Outedge_port_filter* CONTROL0_OUTEDGES;
extern const Edge_filter* NON_CONTROL0_OUTEDGES;
extern const Outedge_port_filter* CONTROL_OUTEDGES;
extern const Edge_filter* NON_CONTROL_OUTEDGES;

extern const Outedge_port_filter* CONTROL1_OUTEDGES;
extern const Edge_filter* NON_CONTROL1_OUTEDGES;

////////////////////////////////////////////////////////////////////////
//  Filter to filter according to parent of src of an edge
////////////////////////////////////////////////////////////////////////

class Parent_of_src_filter : public Edge_filter {
public:
   Parent_of_src_filter(Region* p) ;
   ~Parent_of_src_filter() ;
   bool filter(Edge* const &) const;
private:
   Region* filter_parent ;
} ;

////////////////////////////////////////////////////////////////////////
//  Filter to filter according to parent of dest of an edge
////////////////////////////////////////////////////////////////////////

class Parent_of_dest_filter : public Edge_filter {
public:
   Parent_of_dest_filter(Region* p) ;
   ~Parent_of_dest_filter() ;
   bool filter(Edge* const &) const;
private:
   Region* filter_parent ;
} ;

////////////////////////////////////////////////////////////////////////
//  Filter to filter edges contained within a region
////////////////////////////////////////////////////////////////////////

class Edge_is_contained_in_region_filter: public Edge_filter {
public:
   Edge_is_contained_in_region_filter(Region* r) ;
   ~Edge_is_contained_in_region_filter() ;
   bool filter(Edge* const &) const;
private:
   Region* reg_ptr ;
};


////////////////////////////////////////////////////////////////////////
//  Filter to filter ops contained within a region
////////////////////////////////////////////////////////////////////////

class Op_is_contained_in_region_filter: public Op_filter {
public:
   Op_is_contained_in_region_filter(Region* r) ;
   ~Op_is_contained_in_region_filter() ;
   bool filter(Op* const &) const;
private:
   Region* reg_ptr ;
};

// ----------------------- Same_src_alt_filter-------------------------------
//  Filter to get all edges with same source alt number as a given edge  

class Same_src_alt_filter : public Edge_filter {
 public:
  Same_src_alt_filter(Edge* edge);
  ~Same_src_alt_filter();
  bool filter(Edge* const& edge) const;
 private: 
  Alt_num alt_num;
   
};

// ---------------------- Same_dest_alt_filter -------------------------------
// Filter to get all edges with same destination alt number as a  given edge  

class Same_dest_alt_filter : public Edge_filter {
 public:
  Same_dest_alt_filter(Edge* edge);
  ~Same_dest_alt_filter();
  bool filter(Edge* const& edge) const;
 private: 
  Alt_num alt_num;
};

/* ----------------------------- Filters for regions -----------------------
   Filter for HB or BB
   Filter for loops 
   Filter for loops, HB or BB
----------------------------------------------------------------------------*/

class HB_or_bb_filter: public Region_filter {
public:
    HB_or_bb_filter();
    ~HB_or_bb_filter();
    bool filter(Region* const & region) const;
};

class Loopbody_filter: public Region_filter {
public:
    Loopbody_filter();
    ~Loopbody_filter();
    bool filter(Region* const & region) const;
};

class Scheduled_filter: public Region_filter {
public:
    Scheduled_filter();
    ~Scheduled_filter();
    bool filter(Region* const & region) const;
};

/* ---------------------- Useful constants---------------------------------- */
extern const Region_filter* HB_BB_FILTER;
extern const Region_filter* LOOPBODY_FILTER;
extern const Region_filter* LOOPBODY_HB_BB_FILTER;
extern const Region_filter* HB_BB_SCHED_FILTER;

// ---------------------- Filters used by ModuloScheduler -------------------------  
class Real_edge_filter: public Edge_filter {
public:
  bool filter(Edge * const & edge) const; 
};

class Real_op_filter: public Op_filter {
public:
  bool filter(Op * const & op) const;
};

#endif



