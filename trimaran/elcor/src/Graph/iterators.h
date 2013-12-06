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
//      File:           iterators.h
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson,
//			Santosh Abraham
//      Created:        December 1994
//      Description:    Iterators for the IR structures
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ITERATORS_H
#define _ITERATORS_H

#include "defs.h"
#include "el_port.h"
#include "graph_filters.h" 
#include "filter.h" 
#include "hash_set.h"
#include "dlist.h"
#include "operand_concat.h"
#include "region_utilities.h"
#include "msort.h"
#include "vector.h"
#include "ref.h"
#include "edge_attributes.h"

class Edge ;
class Op ;
class Region ;
class Compound_region ;

/* --------- Filterator for input edges of an operation */

class Op_inedges: public List_filterator<Edge*> {
public:
  Op_inedges();
  Op_inedges(const Op* op, const Edge_filter* filter = ALL_EDGES);
  Op_inedges(const Op* op, Port_type type, Port_num num);
  Op_inedges(const Op* op, Port_type type, Port_num num, Alt_num alt);
  void operator()(const Op* op, const Edge_filter* filter = ALL_EDGES);
  ~Op_inedges();
};

// Iterates over op's inedges as Op_inedges
// Additionally iterates over incoming edges of related ops of op 
class Op_inedges_rel {
public:
   Op_inedges_rel();
   Op_inedges_rel(const Op* op, const Edge_filter* filter = ALL_EDGES);
   void operator()(const Op* op, const Edge_filter* filter = ALL_EDGES);
   ~Op_inedges_rel();

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Edge* operator*() ;
   
private:
   Op_inedges edi ;
   List_iterator<Op*> opi ;
   const Edge_filter* efilter_rep ;
   void check_initial_state() ;
   void advance() ;
};

/* --------- Filterator for output edges of an operation */

class Op_outedges: public List_filterator<Edge*> {
public:
  Op_outedges();
  Op_outedges(const Op* op, const Edge_filter* filter = ALL_EDGES);
  Op_outedges(const Op* op, Port_type type, Port_num num);
  Op_outedges(const Op* op, Port_type type, Port_num num, Alt_num alt);
  void operator()(const Op* op, const Edge_filter* filter = ALL_EDGES);
  ~Op_outedges();
};

// Iterates over op's outedges as Op_inedges
// Additionally iterates over outgoing edges of related ops of op.
class Op_outedges_rel {
public:
   Op_outedges_rel();
   Op_outedges_rel(const Op* op, const Edge_filter* filter = ALL_EDGES);
   void operator()(const Op* op, const Edge_filter* filter = ALL_EDGES);
   ~Op_outedges_rel();
   
   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;
   
   void operator++() ;
   void operator++(int) ;
   Edge* operator*() ;
   
private:
   Op_outedges edi ;
   List_iterator<Op*> opi ;
   const Edge_filter* efilter_rep ;
   void check_initial_state() ;
   void advance() ;
};

/* --------- Filterator for predecessor of an operation. Note that there are 
             two filters passed to the constructor.  */

class Op_pred: public Hash_set_filterator<Op*> {
public:
  Op_pred();
  Op_pred(const Op* op, const Edge_filter* edge_filter = ALL_EDGES,
	  const Op_filter* op_filter = ALL_OPS);
  Op_pred(const Op* op, Port_type type, Port_num num, 
	  const Op_filter* op_filter = ALL_OPS);
  Op_pred(const Op* op, Port_type type, Port_num num, Alt_num alt,
	  const Op_filter* op_filter = ALL_OPS);

  void operator()(const Op* op, const Edge_filter* edge_filter = ALL_EDGES,
		  const Op_filter* op_filter = ALL_OPS);
  ~Op_pred();

private:
  Hash_set<Op*> pred_set;
};


/* --------- Filterator for ancestors of an operation contained within
   an enclosing region. Pseudo-ops are collapsed to corresponding
   real ops. Note that there are two filters passed to the constructor.  */

class Op_ancestors: public Hash_set_filterator<Op*> {
public:
  Op_ancestors();
  Op_ancestors(const Op* op, const Compound_region* reg,
	       const Edge_filter* edge_filter = ALL_EDGES,
	       const Op_filter* op_filter = ALL_OPS);
  Op_ancestors(const Op* op, const Compound_region* reg,
	       Port_type type, Port_num num, 
	       const Op_filter* op_filter = ALL_OPS);
  Op_ancestors(const Op* op, const Compound_region* reg,
	       Port_type type, Port_num num, Alt_num alt,
	       const Op_filter* op_filter = ALL_OPS);

  void operator()(const Op* op, const Compound_region* reg,
		  const Edge_filter* edge_filter = ALL_EDGES,
		  const Op_filter* op_filter = ALL_OPS);
  ~Op_ancestors();

private:
  Hash_set<Op*> ancestor_set;
  void init_ancestor_set(const Op* op, const Compound_region* reg,
			 const Edge_filter* efilt);
};


/* --------- Filterator for successor of an operation. Note that there are 
             two filter arguments to constructors. */

class Op_succ: public Hash_set_filterator<Op*> {
public:
  Op_succ();
  Op_succ(const Op* op, const Edge_filter* edge_filter = ALL_EDGES,
	  const Op_filter* op_filter = ALL_OPS);
  Op_succ(const Op* op, Port_type type, Port_num num, 
	  const Op_filter* op_filter = ALL_OPS);
  Op_succ(const Op* op, Port_type type, Port_num num, Alt_num alt,
	  const Op_filter* op_filter = ALL_OPS);

  void operator()(const Op* op, const Edge_filter* edge_filter = ALL_EDGES,
		  const Op_filter* op_filter = ALL_OPS);
  ~Op_succ();

private:
  Hash_set<Op*> succ_set;
};

/* --------- Filterator for descendants of an operation contained within an
   enclosing region. Pseudo ops are collapsed to corresponding real ops.
   Note that there are two filters passed to the constructor.  */

class Op_descendants: public Hash_set_filterator<Op*> {
public:
   Op_descendants();
   Op_descendants(const Op* op, const Compound_region* reg,
	       const Edge_filter* edge_filter = ALL_EDGES,
	       const Op_filter* op_filter = ALL_OPS);
   Op_descendants(const Op* op, const Compound_region* reg,
	       Port_type type, Port_num num, 
	       const Op_filter* op_filter = ALL_OPS);
   Op_descendants(const Op* op, const Compound_region* reg,
	       Port_type type, Port_num num, Alt_num alt,
	       const Op_filter* op_filter = ALL_OPS);

  void operator()(const Op* op, const Compound_region* reg,
		  const Edge_filter* edge_filter = ALL_EDGES,
		  const Op_filter* op_filter = ALL_OPS);
  ~Op_descendants();

private:
  Hash_set<Op*> descendant_set;
  void init_descendant_set(const Op* op, const Compound_region* reg,
			   const Edge_filter* efilt);
};


///////////////////////////////////////////////////////////////////
// Iterators for op sources
//
// Op_predicate_sources: Iterates over the predicate source(s) for the
// operations. In Playdoh, if the operation is not predicated this
// iterator doesn't do anything. If it is predicated it iterates
// over the controlling predicate operand. e.g. for the op
// z = x + y if p
// this iterator will iterate over p.
//
// Op_explicit_sources: iterates over the sources of an operation.
// e.g. for the op
// z = x + y if p
// this iterator will iterate over x and y.
//
// Op_implicit_sources: Iterates over the implicit sources specified
// by the opcode. Currently these are the Impact annotated source 
// registers for procedure call and return. It is a null
// iterator for other opcodes.
//
// Op_taken_liveout_sources: Iterates over the pseudo sources corresponding
// to the liveouts on the taken path
//
// Op_fallthrough_liveout_sources: Iterates over the pseudo sources
// corresponding to the liveouts on the fallthrough path
//
// Op_mem_sources: Iterates over mem_vrs used by the op
//
// Op_explicit_inputs: Concatanation of Op_predicate_sources and
// Op_explicit_sources. It iterates over the explicitly specified
// (by MDES) inputs of the operation
//
// Op_all_inputs:  Concatanation of Op_predicate_sources and
// Op_explicit_sources and Op_implicit_sources. This is the full set
// of inputs for data-flow analysis (except for memory)
//
// Op_complete_inputs: All the inputs including mem_vrs
//
// Op_all_sources: Concatanation of Op_explicit_sources and
// Op_implicit_sources. Excludes the predicate input(s) and
// mem_vrs
//
///////////////////////////////////////////////////////////////////

class Op_predicate_sources
{
public:
   Op_predicate_sources() ;
   Op_predicate_sources(const Op* op) ;
   void operator()(const Op* op) ;

   Op_predicate_sources(const Op_predicate_sources&) ;
   Op_predicate_sources& operator=(const Op_predicate_sources&) ;
   
   ~Op_predicate_sources() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;

   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;

   El_ref get_ref() ;
private:
   Op* cur_op ;
   Port_num start_port ;
   Port_num end_port ;
} ;

class Op_explicit_sources
{
public:
   Op_explicit_sources() ;
   Op_explicit_sources(const Op* op) ;
   void operator()(const Op* op) ;

   Op_explicit_sources(const Op_explicit_sources&) ;
   Op_explicit_sources& operator=(const Op_explicit_sources&) ;
   
   ~Op_explicit_sources() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;   

   El_ref get_ref() ;
private:
   Op* cur_op ;
   Port_num start_port ;
   Port_num end_port ;
} ;

class Op_implicit_sources
{
public:
   Op_implicit_sources() ;
   Op_implicit_sources(const Op* op) ;
   void operator()(const Op* op) ;

   Op_implicit_sources(const Op_implicit_sources&) ;
   Op_implicit_sources& operator=(const Op_implicit_sources&) ;
   
   ~Op_implicit_sources() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;   

   El_ref get_ref() ;
protected:
   Op* cur_op ;
   List_iterator<Operand> lics ;
} ;

class Op_taken_liveout_sources : public Op_implicit_sources
{
public:
   Op_taken_liveout_sources() ;
   Op_taken_liveout_sources(const Op* op) ;
   void operator()(const Op* op) ;

   El_ref_type ref_type() ;
      
   El_ref get_ref() ;
} ;

class Op_fallthrough_liveout_sources : public Op_implicit_sources
{
public:
   Op_fallthrough_liveout_sources() ;
   Op_fallthrough_liveout_sources(const Op* op) ;
   void operator()(const Op* op) ;

   El_ref_type ref_type() ;
      
   El_ref get_ref() ;
} ;

class Op_mem_sources
{
public:
   Op_mem_sources() ;
   Op_mem_sources(const Op* op) ;
   void operator()(const Op* op) ;

   Op_mem_sources(const Op_mem_sources&) ;
   Op_mem_sources& operator=(const Op_mem_sources&) ;
   
   ~Op_mem_sources() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;   

   El_ref get_ref() ;
private:
   Op* cur_op ;
   Hash_set_iterator<Operand> lims ;
} ;

class Op_live_sources
{
public:
   Op_live_sources() ;
   Op_live_sources(const Op* op) ;
   void operator()(const Op* op) ;

   Op_live_sources(const Op_live_sources&) ;
   Op_live_sources& operator=(const Op_live_sources&) ;
   
   ~Op_live_sources() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;   

   El_ref get_ref() ;
private:
   Op* cur_op ;
   Op_outedges edge_iter ;
   Liveness_info_iterator lims ;

   void init_edge_iter() ;
   void advance() ;
} ;

typedef Operand_2_iter<Op_predicate_sources, Op_explicit_sources> Op_explicit_inputs ;

typedef Operand_3_iter<Op_predicate_sources, Op_explicit_sources, Op_implicit_sources> Op_all_inputs ;

typedef Operand_4_iter<Op_predicate_sources, Op_explicit_sources, Op_implicit_sources, Op_mem_sources> Op_complete_inputs ;

typedef Operand_2_iter<Op_explicit_sources, Op_implicit_sources> Op_all_sources ;

typedef Operand_3_iter<Op_explicit_sources, Op_implicit_sources, Op_mem_sources> Op_complete_sources ;

typedef Operand_2_iter<Op_taken_liveout_sources, Op_fallthrough_liveout_sources> Op_pseudo_sources ;

typedef Operand_2_iter<Op_complete_inputs, Op_pseudo_sources> Op_complete_and_pseudo_inputs ;

typedef Operand_2_iter<Op_all_inputs, Op_pseudo_sources> Op_all_and_pseudo_inputs ;



///////////////////////////////////////////////////////////////////
// Iterators for op dests
//
// Op_explicit_dests: Iterates over the destination specified my MDES
//
// Op_implicit_dests: Iterates over the implicit destinations specified
// by the opcode. Currently these are the Impact annotated destination
// registers for for procedure call and return. It is a null
// iterator for other opcodes.
//
// Op_mem_dests: Iterates over the mem_vrs defined by the op
//
// Op_all_dests : Concatanation of Op_explicit_dests and Op_implicit_dests.
// These are all the destinations of concern for register data-flow.
// Doesn't iterate over memvr destinations
//
// Op_complete_dests: Concatantion of all basic iterators. Iterates over
// the destinations including memvrs.
//
///////////////////////////////////////////////////////////////////

class Op_explicit_dests
{
public:
   Op_explicit_dests() ;
   Op_explicit_dests(const Op* op) ;
   void operator()(const Op* op) ;

   Op_explicit_dests(const Op_explicit_dests&) ;
   Op_explicit_dests& operator=(const Op_explicit_dests&) ;
   
   ~Op_explicit_dests() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;   

   El_ref get_ref() ;
private:
   Op* cur_op ;
   Port_num start_port ;
   Port_num end_port ;
} ;

class Op_implicit_dests
{
public:
   Op_implicit_dests() ;
   Op_implicit_dests(const Op* op) ;
   void operator()(const Op* op) ;

   Op_implicit_dests(const Op_implicit_dests&) ;
   Op_implicit_dests& operator=(const Op_implicit_dests&) ;
   
   ~Op_implicit_dests() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;   

   El_ref get_ref() ;
protected:
   Op* cur_op ;
   List_iterator<Operand> lics ;
} ;

class Op_livein_dests : public Op_implicit_dests
{
public:
   Op_livein_dests() ;
   Op_livein_dests(const Op* op) ;
   void operator()(const Op* op) ;

   El_ref_type ref_type() ;
      
   El_ref get_ref() ;
} ;

class Op_mem_dests
{
public:
   Op_mem_dests() ;
   Op_mem_dests(const Op* op) ;
   void operator()(const Op* op) ;

   Op_mem_dests(const Op_mem_dests&) ;
   Op_mem_dests& operator=(const Op_mem_dests&) ;
   
   ~Op_mem_dests() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ;   

   El_ref get_ref() ;
private:
   Op* cur_op ;
   Hash_set_iterator<Operand> limd ;
} ;

typedef Operand_2_iter<Op_explicit_dests, Op_implicit_dests> Op_all_dests ;
typedef Operand_2_iter<Op_explicit_dests, Op_implicit_dests> Op_all_outputs ;

typedef Operand_3_iter<Op_explicit_dests, Op_implicit_dests,Op_mem_dests> Op_complete_dests ;

typedef Operand_3_iter<Op_explicit_dests, Op_implicit_dests, Op_livein_dests> Op_all_and_pseudo_dests ;

typedef Operand_4_iter<Op_explicit_dests, Op_implicit_dests,Op_mem_dests, Op_livein_dests> Op_complete_and_pseudo_dests ;



///////////////////////////////////////////////////////////////////
// Iterators for op dests
//
// Op_mem_kills: Iterates over the mem_vrs killed by the op
//
///////////////////////////////////////////////////////////////////

class Op_mem_kills
{
public:
   Op_mem_kills() ;
   Op_mem_kills(const Op* op) ;
   void operator()(const Op* op) ;

   Op_mem_kills(const Op_mem_kills&) ;
   Op_mem_kills& operator=(const Op_mem_kills&) ;
   
   ~Op_mem_kills() ;

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Operand& operator*() ;
    
   El_ref_type ref_type() ;
   Port_num get_port_num() ;
   Op* get_op() ; 
   El_ref get_ref() ;
private:
   Op* cur_op ;
   Hash_set_iterator<Operand> limd ;
} ;

///////////////////////////////////////////////////////////////////
// Iterators for operands of a specified op
//
// Op_explicit_operands: Iterates over explicit inputs and dests,
//   including the predicate input.
//
// Op_implicit_operands: Iterates over implicit sources and dests,
//   NOT including the predicate.
//
// Op_all_operands: Iterates over explicit and implicit inputs and
//   dests, including the predicate.
//
// Op_mem_operands: Iterates over mem_vrs defined or used by the op.
//
// Op_complete_operands: Concatenation of Op_all_operands and
//   Op_mem_operands.
//
///////////////////////////////////////////////////////////////////

typedef Operand_2_iter<Op_explicit_inputs, Op_explicit_dests> Op_explicit_operands;

typedef Operand_2_iter<Op_implicit_sources, Op_implicit_dests> Op_implicit_operands;

typedef Operand_2_iter<Op_all_inputs, Op_all_dests> Op_all_operands;

typedef Operand_2_iter<Op_mem_sources, Op_mem_dests> Op_mem_operands;

typedef Operand_2_iter<Op_all_operands, Op_mem_operands> Op_complete_operands;

// --------- Filterator for entry edges, i.e., control-flow edges (CONTROL0)
// entering a region

class Region_entry_edges: public List_filterator<Edge*> {
public:
  Region_entry_edges();
  Region_entry_edges(Region* region, const Edge_filter* filter = ALL_EDGES);
  void operator() (Region* region, const Edge_filter* filter = ALL_EDGES);
  ~Region_entry_edges();

 private:
  void init(Region* region, const Edge_filter* filter);
};

// --------- Filterator for exit edges, i.e., control-flow edges (CONTROL0)
// exiting a region

class Region_exit_edges: public List_filterator<Edge*> {
 public:
  Region_exit_edges();
  Region_exit_edges(Region* region, const Edge_filter* filter = ALL_EDGES);
  void operator()(Region* region, const Edge_filter* filter = ALL_EDGES);
  ~Region_exit_edges();

 private:
  void init(Region* region, const Edge_filter* filter);
};

/* --------- Filterator for control-flow predecessor of a region.
             Note that there are two filters passed to the constructor.  */

class Region_control_flow_pred: public Hash_set_filterator<Region*> {
public:
  Region_control_flow_pred();
  Region_control_flow_pred(Region* region, 
			   const Edge_filter* edge_filter = ALL_EDGES,
			   const Region_filter* region_filter = ALL_REGIONS);
  void operator()(Region* region, 
		  const Edge_filter* edge_filter = ALL_EDGES,
		  const Region_filter* region_filter = ALL_REGIONS);
  ~Region_control_flow_pred();

private:
  void init(Region* region, const Edge_filter* edge_filter,
	    const Region_filter* region_filter);
  Hash_set<Region*> pred_set;

};


/* --------- Filterator for control-flow succecessor of a region.
             Note that there are two filters passed to the constructor.  */

class Region_control_flow_succ: public Hash_set_filterator<Region*> {
public:
  Region_control_flow_succ();
  Region_control_flow_succ(Region* region, 
			   const Edge_filter* edge_filter = ALL_EDGES,
			   const Region_filter* region_filter = ALL_REGIONS);
  void operator()(Region* region, 
		  const Edge_filter* edge_filter = ALL_EDGES,
		  const Region_filter* region_filter = ALL_REGIONS);
  ~Region_control_flow_succ();

private:
  void init(Region* region, const Edge_filter* edge_filter,
	    const Region_filter* region_filter);
  Hash_set<Region*> succ_set;

};

/* --------- Filterator for subregions of a region */

class Region_subregions: public Dlist_filterator<Region*> {
 public:
  Region_subregions(Region* region, const Region_filter* filter = ALL_REGIONS);
  void operator()(Region* region, const Region_filter* filter = ALL_REGIONS);
  ~Region_subregions();
};

class Region_subregions_bidir: public Dlist_iterator<Region*> {
 public:
  Region_subregions_bidir(Region* region, bool end = false);
  Region_subregions_bidir(Region* region, Region* subregion);
  void operator()(Region* region, bool end = false);
  void operator()(Region* region, Region* subregion);
  ~Region_subregions_bidir();
};


/* --------- Filterator for all ops in a region. */

class Region_all_ops {
 public:
   Region_all_ops();
   Region_all_ops(Region* region, const Op_filter* filter = ALL_OPS) ;
   void operator()(Region* region, const Op_filter* filter = ALL_OPS) ;
   ~Region_all_ops();
      
   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ;
   void operator++(int) ;
   Op* operator*() ;

 private:
   void advance();
   void find_in_top_level();
   void find_in_next_region();
   void init(Region* region);

   Dlist_iterator<Region*> top_level;
   Region_all_ops* next_region;
   Op* nextop;
   const Op_filter* filter_rep;
   bool isop;
} ;


// -----------------------Region_all_inedges --------------------------------
// Filterator for iterating over all edges (not only control_flow edges)
// entering into a region. It relies on relop field of the region entry
// operations.

class Region_all_inedges : public List_filterator<Edge*> {
public:
 Region_all_inedges();
 Region_all_inedges(Region* region, const Edge_filter* filter = ALL_EDGES);
 void operator()(Region* region, const Edge_filter* filter = ALL_EDGES);
 ~Region_all_inedges();

private:
 void init(Region* region, const Edge_filter* filter);
 List<Edge*> all_edges;
};


// -----------------------Region_all_outedges --------------------------------
// Filterator for iterating over all edges (not only control_flow edges)
// exiting from a region. It relies on relop field of the region entry
// operations.

class Region_all_outedges : public List_filterator<Edge*> {
public:
  Region_all_outedges();
  Region_all_outedges(Region* region, const Edge_filter* filter = ALL_EDGES);
  void operator()(Region* region, const Edge_filter* filter = ALL_EDGES);
  ~Region_all_outedges();

private:
  void init(Region* region, const Edge_filter* filter);
  List<Edge*> all_edges;
};

//--------- --------- Region_all_internal_edges ------------------------------
// Filterator for iterating over all internal edges of a region. An interal
// edge is one whose source and destination are both in the region.


class Region_all_internal_edges : public Hash_set_filterator<Edge*> {
public:
 Region_all_internal_edges();
 Region_all_internal_edges(Region* region, 
			   const Edge_filter* filter = ALL_EDGES);
 void operator()(Region* region, const Edge_filter* filter = ALL_EDGES);
 ~Region_all_internal_edges();

private:
 void init(Region* region, const Edge_filter* filter);
 void init_aux(Region* region, Region* top_level);
 Hash_set<Edge*> all_edges;
};


// ------------------- Region_entry_ops--------------------------------------
// Filterator for entry operations of a region. Entry operations are the ones
// that are targets of control_flow edges entering the region.

class Region_entry_ops: public Hash_set_filterator<Op*> {
public:
  Region_entry_ops();
  Region_entry_ops(Region* region, const Op_filter* filter = ALL_OPS);
  void operator()(Region* region, const Op_filter* filter = ALL_OPS);
  ~Region_entry_ops();

 private:
  void init(Region* region, const Op_filter* filter);
};

// ------------------- Region_exit_ops--------------------------------------
// Filterator for exit operations of a region. Exit operations are the ones
// that are sources of control_flow edges exiting the region.

class Region_exit_ops: public Hash_set_filterator<Op*> {
 public:
  Region_exit_ops();
  Region_exit_ops(Region* region, const Op_filter* filter = ALL_OPS);
  void operator()(Region* region, const Op_filter* filter = ALL_OPS);
  ~Region_exit_ops();

 private:
  void init(Region* region, const Op_filter* filter);
};

 
//////////////////////////////////////////////////////////////////////
//     Class Region_ops_C0_order
//     ops in forward control flow order: works only for BB, SB, HB
//////////////////////////////////////////////////////////////////////
class Region_ops_C0_order {
public:
  Region_ops_C0_order();
  Region_ops_C0_order(Compound_region*, Op* start_op = NULL) ;
  // Initialize empty iterator
  void operator()(Compound_region*, Op* start_op = NULL) ;
  ~Region_ops_C0_order();
  
  bool operator==(const void*) const ;
  bool operator!=(const void*) const ;
  
  void operator++() ;
  void operator++(int) ;
  Op* operator*() ;
  
// ----- To let the user know that these are not supported. They do assert(0).
  Region_ops_C0_order(const Region_ops_C0_order&);
  Region_ops_C0_order& operator=(const Region_ops_C0_order&);
  
private:
  void init(Compound_region*);
  void init(Compound_region*, Op*);
  void advance();
  
  Compound_region* blk;
  Op* op;
} ;

//////////////////////////////////////////////////////////////////////
//     Class Region_ops_reverse_C0_order
//     ops in reverse control flow order: works only for BB, SB, HB
//////////////////////////////////////////////////////////////////////
class Region_ops_reverse_C0_order {
public:
  Region_ops_reverse_C0_order();
  Region_ops_reverse_C0_order(Compound_region*, Op* exit_op = NULL) ;
  void operator()(Compound_region*, Op* exit_op = NULL) ;
  ~Region_ops_reverse_C0_order();
  
  bool operator==(const void*) const ;
  bool operator!=(const void*) const ;
  
  void operator++() ;
  void operator++(int) ;
  Op* operator*() ;
  
// ----- To let the user know that these are not supported. They do assert(0).
  Region_ops_reverse_C0_order(const Region_ops_reverse_C0_order&);
  Region_ops_reverse_C0_order& operator=(const Region_ops_reverse_C0_order&);
  
private:
  void init(Compound_region*, Op* exit_op);
  void advance();
  
  Compound_region* blk;
  Op* op;
} ;

/*---------------------------------------------------------------------------
   Region_ops_linear: 
   Bi-directional iterator over all operations in a region. 
   The order of iteration is given by the concatenation of D-list ordering at
   each level in the hierarchy. 
   ++ moves in the forward direction, -- in the reverse direction.
   The iteration can be started at:
   1. The beginning: This is the default if boolean input is not specified.
   2. At the end: Specify TRUE as the second argument.
   3. At any middle point: Specify the list element as the second argument
-----------------------------------------------------------------------------*/

class Region_ops_linear {
public:
   Region_ops_linear();
   Region_ops_linear(Region*, bool end = false) ;
   Region_ops_linear(Region*, Op*) ;  // start at an operation
   void operator()(Region*, bool end = false) ;
   void operator()(Region*, Op*) ;
   ~Region_ops_linear();

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ; 
   void operator++(int) ; // int parameter signals postfix

   void operator--() ; 
   void operator--(int) ; // int parameter signals postfix

   Op* operator*() ;

// ----- To let the use know that these are not supported. They do assert(0)
   Region_ops_linear(const Region_ops_linear&);
   Region_ops_linear& operator=(const Region_ops_linear&);

private:
   Region* the_region;
   bool done;
   List<Dlist_iterator<Region*>* > iterator_list;
   void init_go_down(Region*, bool end);
   void init_go_up(Region*);
   void increment();
   void decrement();
   void delete_iterator_list();
} ;


/*---------------------------Region_subregions_freq--------------------------
  Iterates over subregions (recursively) of a region in sorted order based on
  the frequencies.
  Return_type filter specifies the types of regions that one is interested
  in, e.g., loops or hyperblocks.
 
  Stop_recursion filter specifies when to stop the recursive walk to collect
  all subregions.
----------------------------------------------------------------------------*/

class Region_subregions_freq {
 public:
  Region_subregions_freq();
  Region_subregions_freq(Compound_region* region,
			 const Region_filter* return_type = HB_BB_FILTER,
                         const Region_filter* stop_recursion = HB_BB_FILTER);
  void operator()(Compound_region* region,
		  const Region_filter* return_type = HB_BB_FILTER,
		  const Region_filter* stop_recursion = HB_BB_FILTER);
  ~Region_subregions_freq();
   
  bool operator==(const void*) const ;
  bool operator!=(const void*) const ;

  void operator++() ;
  void operator++(int) ;    // This is to allow postfix defn.
  Region* operator*() ;

// ----- To let the user know that these are not supported. They do assert(0).
  Region_subregions_freq(const Region_subregions_freq&);
  Region_subregions_freq& operator=(const Region_subregions_freq&);

 private:
  void init(Compound_region* region, const Region_filter* return_type, 
	    const Region_filter* stop_recursion);
  void region_specified_subregions(Region* region, 
				   const Region_filter* return_type,
				   const Region_filter* stop_recursion,
				   List<Region*>& regions,
				   int& length);
  int index;
  int length;
  Vector<Region*>* sorted_vector;
    
};

#endif


