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
//      File:           region.cpp
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson,
//                      Santosh Abraham, Scott Mahlke, Dave August,
//                      Brian Deitrich,Joel Jones
//      Created:        January 1994
//      Description:    Region class hierearchy implementation
//
/////////////////////////////////////////////////////////////////////////////

#include "region.h"
#include "iterators.h"
#include "hash_set.h"
#include "attributes.h"
#include "edge.h"
#include "op.h"
#include "el_jumptbl.h"
#include "dlist.h"
#include "opcode_properties.h"
#include "region_purge.h"

Procedure *default_scope = NULL;

/* Region class */

Region::Region()
  :region_flags(EL_REGION_LAST_FLAG), parent_region(NULL), dlist_node(NULL)
{
  attributes = new Graph_attribute ;
}

Region::Region(const Region& rgn)
  :region_flags(rgn.region_flags), parent_region(rgn.parent_region), dlist_node(NULL)
{
  attributes = new Graph_attribute(*rgn.attributes) ;
}

// Region& Region::operator=(const Region& rgn)
// {
//    region_flags = rgn.region_flags ;
//    parent_region = rgn.parent_region ;
//    delete attributes ; // This is safe if attributes is NULL
//    attributes = new Graph_attribute(*rgn.attributes) ;
//    return *this ;
// }

Region::~Region() 
{
  delete attributes ;  
}

Compound_region* Region::parent() const {return parent_region;}

Region& Region::set_parent(Compound_region* parent){
  parent_region = parent;
  return *this;
}

Dnode<Region*>* Region::dnode() const {return dlist_node ;}

Region& Region::set_dnode(Dnode<Region*>* dn){
  dlist_node = dn ;
  return *this;
}

bool Region::is_op() const {return false;}
bool Region::is_compound() const {return false;}
bool Region::is_bb() const {return false;}
bool Region::is_hb() const { return false; }
bool Region::is_procedure() const {return false;}
bool Region::is_loopbody() const {return false;}
bool Region::is_tree () const {return false;}

bool Region::is_single_entry() const {return true;}
bool Region::is_single_exit() const {return true;}

Hash_set<Region*> Region::insubregions()
{
  Hash_set<Region*> insr(hash_region_ptr, 16);
  for (Hash_set_iterator<Op*> si(this->inops()) ; si != 0 ; si++) {
    Region* tmpr = (*si) ;
    while((tmpr != 0) && (tmpr->parent() != this)) tmpr = tmpr->parent() ;
    if (tmpr) insr += tmpr ;
    else assert(0) ;  // Parent chain is not set correctly for (*si) 
  }
  return insr ;
}

Hash_set<Region*> Region::outsubregions()
{
  Hash_set<Region*> outsr(hash_region_ptr, 16);
  for (Hash_set_iterator<Op*> si(this->outops()) ; si != 0 ; si++) {
    Region* tmpr = (*si) ;
    while((tmpr != 0) && (tmpr->parent() != this)) tmpr = tmpr->parent() ;
    if (tmpr) outsr += tmpr ;
    else assert(0) ;  // Parent chain is not set correctly for (*si) 
  }
  return outsr ;
}

// Region Copying functions 

Region* Region::copy_with_cf_interface_edges(){
  Region_map region_map;
  Edge_map edge_map;
  return (copy_with_specified_edges(region_map, edge_map, CONTROL0_INEDGES, 
				    CONTROL0_OUTEDGES));
}

Region* Region::copy_with_specified_edges(Region_map& region_map, 
					  Edge_map& edge_map,
					  const Edge_filter* in_filter,
					  const Edge_filter* out_filter,
					  const Edge_filter* internal_filter){
  Region* region = this;
  //Clone the required edges and put them in the map
  for(Region_all_inedges in(region, in_filter); in != 0; in++)
    ::region_copy_edge_clone_and_bind(*in, edge_map);
  for(Region_all_outedges out(region, out_filter); out != 0; out++)
    ::region_copy_edge_clone_and_bind(*out, edge_map);
  for(Region_all_internal_edges internal(region, internal_filter); 
      internal != 0; internal++)
    ::region_copy_edge_clone_and_bind(*internal, edge_map);

  // Recursively clone the regions
  Region* new_region = region->copy_with_edges(region_map, edge_map);
  new_region->set_parent(NULL);
  return(new_region);
}


bool Region::flag(Elcor_region_flag f) const 
{
  return(region_flags.bit((unsigned int) f)) ;
}

void Region::set_flag(Elcor_region_flag f)
{
  region_flags.set_bit((unsigned int) f) ;
}

void Region::reset_flag(Elcor_region_flag f)
{
  region_flags.reset_bit((unsigned int) f) ;
}

void Region::copy_flags(Region *src_reg)
{
  region_flags += src_reg->region_flags;
}

int Region::get_next_op_id_num (Procedure *scope) const
{
  if(!scope)
    scope = ::default_scope;
  return scope->get_max_op_id();
  
  /*
  Region* cur_reg = (Region*) this ;
  while (!cur_reg->is_procedure()) {
    cur_reg = cur_reg->parent() ;
    if (!cur_reg) {
      El_punt("Region hierarchy does not have a procedure that contains this region") ;
    }
  }
  return (((Procedure*)cur_reg)->get_max_op_id()) ;
  */
}

int Region::get_next_op_id_num_inc(Procedure *scope)
{
  int id;

  if(!scope)
    scope = ::default_scope;
  id = scope->get_max_op_id();
  scope->set_max_op_id(id + 1);
  return id;
}

// Region print function 
ostream& operator<<(ostream& os, const Region& region) {
  region.print(os);
  return os;
}


////////////////////////////////////////////////////////////////////
//
// Compound_region class :
//
////////////////////////////////////////////////////////////////////

//int Compound_region::id_num = 1;

Compound_region::Compound_region(Procedure *scope)
   : embedded_entry_ops(hash_op_ptr),
     embedded_exit_ops(hash_op_ptr)
{
  id_rep = get_next_region_id_num_inc(scope);
}

Compound_region::Compound_region(int reg_id, double w, Procedure *scope)
   : embedded_entry_ops(hash_op_ptr),
     embedded_exit_ops(hash_op_ptr)
{
  weight = w ;
  id_rep = reg_id ;
  if(reg_id > 0) // hack, for global(!) 'placeholder' BBs  --Marnix
    {
      if(!scope)
	scope = ::default_scope;
      if(scope->get_max_region_id() <= reg_id)
	scope->set_max_region_id(reg_id + 1);
    }
}

Compound_region::~Compound_region() {}

void Compound_region::renumber(Procedure *scope) 
{
  id_rep = get_next_region_id_num_inc(scope);
}

int Compound_region::id() const {return id_rep;}
void Compound_region::set_id(int id) {id_rep = id;}
void Compound_region::reset_id_counter(int id, Procedure *scope) 
{
  if(!scope)
    scope = ::default_scope;
  scope->set_max_region_id(id);
}

bool Compound_region::is_compound() const {return true;}

Compound_region& Compound_region::add_region(Region* region){
  assert(region->dnode() == NULL) ;
  embedded_regions.push_tail(region);
  region->set_dnode(embedded_regions.tail_node) ;
  region->set_parent(this);
  return *this;
}
  
Compound_region& Compound_region::remove_region(Region* region) {
  embedded_regions.remove(region->dnode()) ;
  region->set_dnode(NULL) ;
  return *this;
}

Compound_region& Compound_region::clear_region() {
  for (Dlist_iterator<Region*> dli(embedded_regions) ; dli != 0 ; dli++) {
    (*dli)->set_dnode(NULL) ;
  }
  embedded_regions.clear() ;
  return *this;
}

Compound_region& Compound_region::add_entry(Op* op) {
  embedded_entry_ops += op;
  return *this;
}

Compound_region& Compound_region::remove_entry(Op* op) {
  embedded_entry_ops -= op;
  return *this;
}
Compound_region& Compound_region::add_exit(Op* op) {
  embedded_exit_ops += op;
  return *this;
}
  
Compound_region& Compound_region::remove_exit(Op* op) {
  embedded_exit_ops -= op;
  return *this;
}

Compound_region& Compound_region::add_entry(Edge* edge) {
  entry_edges.add_tail(edge);
  return *this;
}

Compound_region& Compound_region::add_entry_safely(Edge* edge) {
  if (!entry_edges.is_member(edge)) entry_edges.add_tail(edge);
  return *this;
}

Compound_region& Compound_region::remove_entry(Edge* edge){
  entry_edges.remove(edge);
  return *this;
}

Compound_region& Compound_region::add_exit(Edge* edge){
  exit_edges.add_tail(edge);
  return *this;
}

Compound_region& Compound_region::add_exit_safely(Edge* edge){
  if (!exit_edges.is_member(edge)) exit_edges.add_tail(edge);
  return *this;
}

Compound_region& Compound_region::remove_exit(Edge* edge) {
  exit_edges.remove(edge);
  return *this;
}

Compound_region&
Compound_region::insert_before_region(Region* r, Region* target)
{
  Dnode<Region*>* nd =  target->dnode() ;
  if (target->is_op()) {
    if ((!is_switch((Op*)target) && (!is_merge((Op*)target)))) {
      Dnode<Region*>* prev_nd = nd->prev;
      Op* prev_op = (prev_nd != NULL) ? (Op*) prev_nd->value() : (Op*) NULL;
         
      // while prev node is a relop of target do
      while (prev_op != NULL &&
	     !prev_op->get_relops().is_empty() &&
	     prev_op->get_relops().head() == target) {
	nd = prev_nd;
	prev_nd = prev_nd->prev;
	prev_op = (prev_nd != NULL) ? (Op*) prev_nd->value() : (Op*) NULL;
      }
    }
  }
  embedded_regions.insert_before(nd, r);
  r->set_dnode(nd->prev) ;
  return *this;
}
  
Compound_region&
Compound_region::insert_immediately_before_region(Region* r, Region* target)
{
  Dnode<Region*>* nd =  target->dnode() ;
  embedded_regions.insert_before(nd, r);
  r->set_dnode(nd->prev) ;
  return *this;
}
  
Compound_region&
Compound_region::insert_after_region(Region* r, Region* target)
{
  Dnode<Region*>* nd = target->dnode() ;
  if (target->is_op()) {
    if ((!is_switch((Op*)target) && (!is_merge((Op*)target)))) {
      Dnode<Region*>* next_nd = nd->next;
      Op* next_op = (next_nd != NULL) ? (Op*) next_nd->value() : (Op*) NULL;
         
      // while next node is a relop of target do
      while (next_op != NULL &&
	     !next_op->get_relops().is_empty() &&
	     next_op->get_relops().head() == target) {
	nd = next_nd;
	next_nd = next_nd->next;
	next_op = (next_nd != NULL) ? (Op*) next_nd->value() : (Op*) NULL;
      }
    }
  }
  embedded_regions.insert_after(nd, r);
  r->set_dnode(nd->next) ;
  return *this;
}

Compound_region&
Compound_region::insert_immediately_after_region(Region* r, Region* target)
{
  Dnode<Region*>* nd = target->dnode() ;
  embedded_regions.insert_after(nd, r);
  r->set_dnode(nd->next) ;
  return *this;
}


void Compound_region::replace_op_recursively(Op* old_op, Op* new_op)
{
  if(embedded_exit_ops.is_member(old_op))
    {
      embedded_exit_ops -= old_op;
      embedded_exit_ops += new_op;
    }
  if(embedded_entry_ops.is_member(old_op))
    {
      embedded_entry_ops -= old_op;
      embedded_entry_ops += new_op;
    }
  if(embedded_regions.is_member(old_op))
    {
      Dnode<Region*>* nd = old_op->dnode() ;
      embedded_regions.insert_after(nd, new_op);
      new_op->set_dnode(nd->next) ;
      embedded_regions.remove(old_op);
    }
  if(parent())
    {
      parent()->replace_op_recursively(old_op, new_op);
    }
}

bool Compound_region::succeeds_in_subregion_list(Region *second,Region* first)
{
  Dnode<Region*>* nptr= embedded_regions.find(first);
  if(nptr==NULL)
    return false;
  else if(nptr->next == NULL) 
    return false;
  else		   
    return (nptr->next->value() == second);
}


Region* Compound_region::successor_in_subregion_list(Region* first)
{
  Dnode<Region*>* nptr= embedded_regions.find(first);
  if(nptr==NULL)
    return NULL;
  else if(nptr->next == NULL) 
    return NULL;
  else		   
    return (nptr->next->value());
}


Region* Compound_region::last_subregion()
{
  return embedded_regions.tail();
}

Region* Compound_region::first_subregion()
{
  return embedded_regions.head();
}

int Compound_region::get_next_region_id_num(Procedure *scope)
{
  if(!scope)
    scope = ::default_scope;
  return scope->get_max_region_id();
}

int Compound_region::get_next_region_id_num_inc(Procedure *scope)
{
  int id = 0;
  
  if(!scope)
    scope = ::default_scope;

  if(scope)  // if there isn't a scope, we shouldn't do this.
    {
      id = scope->get_max_region_id();
      scope->set_max_region_id(id + 1);
    }
  return id;
}

bool Compound_region::is_entry_edge(Edge* edge) {
  return entry_edges.is_member(edge);
}

bool Compound_region::is_exit_edge(Edge* edge) {
  return exit_edges.is_member(edge);
}

bool Compound_region::is_entry_op(Op* op) {
  return embedded_entry_ops.is_member(op);
}

bool Compound_region::is_exit_op(Op* op) {
  return embedded_exit_ops.is_member(op);
}

Dlist<Region*>& Compound_region::subregions() {return embedded_regions;}

Hash_set<Op*>& Compound_region::inops() {return embedded_entry_ops;}

Hash_set<Op*>& Compound_region::outops() {return embedded_exit_ops;}

List<Edge*>& Compound_region::inedges() {return entry_edges;}
List<Edge*>& Compound_region::outedges() {return exit_edges;}

//-------- Copy a compound region; set entry-exit edges, and set entry-exit ops
Region* Compound_region::copy_with_edges(Region_map& region_map, 
					 Edge_map& edge_map) {
  Compound_region* new_region = new Compound_region;
  copy_with_edges_aux(new_region, region_map, edge_map);
  return new_region;
}

void Compound_region::copy_with_edges_aux(Compound_region* new_region, 
					  Region_map& region_map, 
					  Edge_map& edge_map){
  Edge* edge;
  Op* new_op;
  Region* temp;
  Region* region = this;
  region_map.bind(region, new_region);

  //Clone all subregions recursively and add them to current region
  for(Region_subregions sub(region); sub != 0; sub++) {
    temp = (*sub)->copy_with_edges(region_map, edge_map);
    new_region->add_region(temp);
    temp->set_parent(new_region);
  }

  //Set related_ops for direct descendent ops
  for(Region_subregions sub1(region); sub1 != 0; sub1++) {
    temp = *sub1;
    if (temp->is_op()) {
      new_op = (Op*)(region_map.value(temp));
      for(List_iterator<Op*> iter(((Op*)temp)->get_relops()); iter != 0; iter++)
	new_op->add_relops((Op*)(region_map.value(*iter)));
    }
  }

  // Set entry and exit edges for the current region
  for(Region_entry_edges in(region); in != 0; in++) {
    edge = *in;
    if(edge_map.is_bound(edge))
      new_region->add_entry_safely(edge_map.value(edge));
  }
  for(Region_exit_edges out(region); out != 0; out++){
    edge = *out;
    if(edge_map.is_bound(edge))
      new_region->add_exit_safely(edge_map.value(*out));
  }

  // Set entry and exit ops for the current region;
  for(Region_entry_ops oin(region); oin != 0; oin++)
    new_region->add_entry((Op*)(region_map.value(*oin)));
  for(Region_exit_ops oout(region); oout != 0; oout++)
    new_region->add_exit((Op*)(region_map.value(*oout)));

  // Set attributes 					
  delete new_region->attributes ;
  new_region->attributes = new Graph_attribute(*(region->attributes)) ;

  return ;
}

void Compound_region::print(ostream& os) const {
  Region* region;
  Compound_region* dummy;
  
  dummy = (Compound_region*) this ;
  os << "   --- Parent = ";
  if (parent_region == NULL) os << "NULL";
  else os <<  parent_region->id();
  os << endl;
  os << "   --- Predecessors = " ;
  for(Region_control_flow_pred pred(dummy); pred != 0; ++pred) {
    region = *pred;
    if (region == NULL) os << "NULL ";
    else os <<  region->id() << " ";
  }
  os << endl;
  os << "   --- Successors = " ;
  for(Region_control_flow_succ succ(dummy); succ != 0; ++succ){
    region = *succ;
    if (region == NULL) os << "NULL ";
    else os <<  region->id() << " ";
  }
  os << endl;
  os << "--- Subregions:  " << endl;
  for(Region_subregions sub(dummy); sub != 0; ++sub)
    os << *(*sub);
}


/* Class Basicblock */

Basicblock::Basicblock() {}
Basicblock::Basicblock(int id, double w)
  : Compound_region(id, w)
{}

Basicblock::~Basicblock(){}

bool Basicblock::is_bb() const {return true;}

// Copy a BB and set freq
Region* Basicblock::copy_with_edges(Region_map& region_map, Edge_map& edge_map) {
  Basicblock* new_region = new Basicblock;
  copy_with_edges_aux(new_region, region_map, edge_map);
  return new_region;
}

void Basicblock::print(ostream& os) const {
  os << "Basicblock:" << id_rep << "{" ;
  os << endl ;
  Compound_region::print(os);
  os << "}" << endl;
}


/* Class Hyperblock */

Hyperblock::Hyperblock() {}

Hyperblock::Hyperblock(int id, const double& w)
  : Compound_region(id, w)
{}

Hyperblock::~Hyperblock(){}

bool Hyperblock::is_hb() const {return true;}

bool Hyperblock::is_single_exit() const {return false;}

Region*
Hyperblock::copy_with_edges(Region_map& region_map, Edge_map& edge_map) 
{
  Hyperblock* new_region = new Hyperblock;
  copy_with_edges_aux(new_region, region_map, edge_map);
  return new_region;
}

void
Hyperblock::print(ostream& os) const
{
  os << "Hyperblock:" << id_rep << "{" ;
  os << endl ;
  Compound_region::print(os);
  os << "}" << endl;
}


/* Class LoopBody */
LoopBody::LoopBody()
  : cFlowBackEdges(hash_edge_ptr),
    staging_pred(new Int_lit(0))
{}

LoopBody::LoopBody(int id, const double& w)
  : Compound_region(id, w),
    cFlowBackEdges(hash_edge_ptr),
    staging_pred(new Int_lit(0))
{}

LoopBody::~LoopBody() {}

bool LoopBody::is_loopbody() const { return true; }

void
LoopBody::print(ostream& os) const {
  os << "LoopBody:" << id_rep << "{";
  os << endl;
  Compound_region::print(os);
  os << "}" << endl;
}

Region *
LoopBody::copy_with_edges(Region_map& region_map, Edge_map& edge_map)
{
  LoopBody *new_region = new LoopBody;
  copy_with_edges_aux(new_region, region_map, edge_map);
  for (Hash_set_iterator<Edge *> edge(cFlowBackEdges); edge != 0; edge++) {
    new_region->addBackEdge(edge_map.value(*edge));
  }
  return new_region;
}

Hash_set<Edge *> *
LoopBody::controlFlowBackEdges()
{
  return &cFlowBackEdges;
}

void
LoopBody::addBackEdge(Edge *edge)
{
  cFlowBackEdges += edge;
}

Compound_region *
LoopBody::preloop()
{
  assert(is_single_entry());
  Region_entry_edges onlyC0inEdge(this);
  Compound_region* pre = (Compound_region*)((*onlyC0inEdge)->src()->parent());
  return pre;
}

Compound_region *
LoopBody::postloop()
{
  Op_outedges edge;
   
  assert(cFlowBackEdges.size() == 1);
  Edge *backEdge = cFlowBackEdges.head();
  Op *srcOp = backEdge->src();
  for (edge(srcOp); edge != 0 && (*edge) == backEdge; edge++)
    ;
  Compound_region* post = (Compound_region*)((*edge)->dest()->parent());
  return post;
}

Compound_region *
LoopBody::remainder()
{
  for(Region_entry_edges edges(preloop()); edges != 0; edges++) {
    Compound_region* blk = (*edges)->src()->parent();
    if(blk->flag(EL_REGION_REMAINDER_BLOCK))
      return blk;
  }
  return NULL;
}

Compound_region *
LoopBody::preremainder()
{
  Compound_region *rem = NULL, *prerem = NULL;
  for(Region_entry_edges edges(preloop()); edges != 0; edges++) {
    Compound_region* blk = (*edges)->src()->parent();
    if(blk->flag(EL_REGION_REMAINDER_BLOCK))
      rem = blk;
    else
      prerem = blk;
  }

  // If we're doing modulo variable expansion and we didn't see a
  // remainder block with an edge to the preloop than something is
  // very wrong. Print out the IR at the end of
  // create_preremainder_blocks() in el_loop.cpp to make sure things
  // look ok.

  if(rem != NULL)
    return prerem;
  else
    return NULL;
}

Compound_region *
LoopBody::kernel()
{
  Compound_region* body = (Compound_region*)first_subregion();
  return body;
#if 0
  assert(cFlowBackEdges.size() == 1);
  Compound_region* body = (Compound_region*)
    (cFlowBackEdges.head()->src()->parent());
  return body;
#endif
}

/* Class Procedure */

Procedure::Procedure(bool save_scope)
{
  weight = 0;
  id_rep = 1;
  region_id_num = 2;
  op_id_num = 1;
  jumptbl_info = NULL;
  if(!save_scope)
    ::default_scope = this ;
}

Procedure::Procedure(const eString& s, const double& w, bool save_scope)
  : procedure_name(s) 
{
  weight = w;
  id_rep = 1;
  region_id_num = 2;
  op_id_num = 1;
  jumptbl_info = NULL;
  if(!save_scope)
    ::default_scope = this ;
}

Procedure::~Procedure()
{
  if (jumptbl_info) delete jumptbl_info;
  if (::default_scope == this) ::default_scope = NULL ;
}

bool Procedure::is_procedure() const {return true;}

eString Procedure::get_name() const { return procedure_name ; }
void Procedure::set_name(const eString& s) { procedure_name = s ;}

void Procedure::set_max_op_id(int n)  { op_id_num = n ;}

int Procedure::get_max_op_id()  { return(op_id_num) ;}

void Procedure::set_max_region_id(int n)  { region_id_num = n ;}

int Procedure::get_max_region_id()  { return(region_id_num) ;}


void Procedure::set_jumptbl_info(El_jumptbl_info *info)
{
  jumptbl_info = info;
}

El_jumptbl_info* Procedure::get_jumptbl_info()
{
  return (jumptbl_info);
}


// Copy a procedure, set name,  proc-entry and proc-exit
Region* Procedure::copy_with_edges(Region_map& region_map, Edge_map& edge_map) {
  Procedure* new_region = new Procedure;
  copy_with_edges_aux(new_region, region_map, edge_map);
  new_region->set_name(procedure_name);
  new_region->op_id_num = op_id_num;
  return new_region;
}
void Procedure::print(ostream& os) const {
  os << "Procedure:" << procedure_name << " {" << endl;
  Compound_region::print(os);
  os << "}" << endl;
}

/* Class Tree */

Tree::Tree ()
{
}

Tree::Tree (int id, const double& w) : Compound_region (id, w)
{
}

Tree::~Tree ()
{
}

bool Tree::is_tree () const {return true;}

Region* Tree::copy_with_edges (Region_map& region_map, Edge_map& edge_map) 
{
  Tree* new_region = new Tree;
  copy_with_edges_aux (new_region, region_map, edge_map);
  return new_region;
}

void Tree::print (ostream& os) const
{
  os << "Tree:" << id_rep << "{" ;

  os << endl ;

  Compound_region::print (os);

  os << "}" << endl;
}

Program::Program() : name_to_proc(hash_estring, 128)
{
  num_procs = 0;
  name = eString("");
}

Program::Program(const eString& est) : name_to_proc(hash_estring, 128)
{
  num_procs = 0;
  name = est;
}

Program::Program(Procedure* p) : name_to_proc(hash_estring, 128)
{
  if(!p) return;
  num_procs = 1;
  name = eString("");
  procs.add_tail(p);
  name_to_proc.bind(p->get_name(), p);
}

Program::Program(Procedure* p, const eString& e) : name_to_proc(hash_estring, 128)
{
  assert(p != NULL);
  num_procs = 1;
  name = e;
  procs.add_tail(p);
  name_to_proc.bind(p->get_name(), p);
}

Program::~Program()
{
  for(Program_iterator pi = this->get_iterator(); pi != 0; pi++)
    region_purge(*pi);
  for(Data_iterator di = this->get_data_iterator(); di != 0; di++)
    delete *di;
  procs.clear();
}

void Program::add_procedure(Procedure* p)
{
  if(!p) return;
  num_procs++;
  procs.add_tail(p);
  name_to_proc.bind(p->get_name(), p);
}

void Program::add_data(El_datalist* datum)
{
  if(!datum) return;
  data.add_tail(datum);
}

void Program::remove_data(El_datalist* datum)
{
  if(!datum) return;
  data.remove(datum);
}

void Program::remove_procedure(Procedure* p)
{
  if(!p) return;
  num_procs--;
  procs.remove(p);
  name_to_proc.unbind(p->get_name());
}

void Program::remove_procedure(const eString& name)
{
  Procedure* p = name_to_proc.value(name);
  num_procs--;
  procs.remove(p);
  name_to_proc.unbind(name);
}

Procedure* Program::get_procedure(const eString& name)
{
  return name_to_proc.value(name);
}

void Program::clear()
{
  num_procs = 0;
  procs.clear();
  data.clear();
  name_to_proc.clear();
}

eString Program::get_name()
{
  return name;
}

void Program::set_name(const eString& new_name)
{
  name = new_name;
}

Program_iterator Program::get_iterator() const
{
  Program_iterator pi(procs);
  return pi;
}

Data_iterator Program::get_data_iterator() const
{
  Data_iterator di(data);
  return di;
}

int Program::get_num_procedures()
{
  return num_procs;
}


ostream& operator<<(ostream& os, const Program& prog)
{
  os << "Program " << prog.name << ":\n";
  for(Program_iterator pi = prog.get_iterator(); pi != 0; pi++) {
    os << "  " << (*pi)->get_name() << endl;
  }
  os << endl;
  return os;
}

//----------------------------------------------------------------------------
// Clone an edge and bind it in the map
Edge* region_copy_edge_clone_and_bind(Edge* original, Edge_map& edge_map) {
  Edge* new_edge = original->clone();
  edge_map.bind(original, new_edge);
  delete new_edge->attributes ;
  new_edge->attributes = new Graph_attribute(*(original->attributes));
  return(new_edge);
}


