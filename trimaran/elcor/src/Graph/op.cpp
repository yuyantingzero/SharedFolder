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
//      File:           op.cpp
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson,
//                      Santosh Abraham, Scott Mahlke, Dave August,
//                      Brian Deitrich, Matthai Philipose, Shail Aditya
//      Created:        January 1994
//      Description:    Op class implementation
//
/////////////////////////////////////////////////////////////////////////////


#include "op.h"
#include "iterators.h"
#include "intf.h"
#include "opcode.h"
#include <stdio.h>
#include <string.h>
#include "attributes.h"
#include "operand.h"
#include "opcode_properties.h"
#include "edge.h"
#include "region_utilities.h"

static Dlist<Region*>    EMPTY_REGION_LIST; // Should be const
static Hash_set<Op*>     EMPTY_OP_SET(hash_op_ptr);

Op::Op(Procedure *scope)
   :sched_opcode_rep(NULL), custom_opcode_rep(NULL), mem_out(NULL), mem_in(NULL), mem_kill(NULL), related_ops(NULL)
{ 
  id_rep =  get_next_op_id_num_inc(scope);
  srcs = new Vector<Operand>;
  dests = new Vector<Operand>;
  sched_time_rep = -1;
//  attributes = new Graph_attribute ;
}

Op::Op(const Opcode& opcode, Procedure *scope)
   : sched_opcode_rep(NULL), custom_opcode_rep(NULL), mem_out(NULL), mem_in(NULL), mem_kill(NULL), related_ops(NULL)
{
  id_rep =  get_next_op_id_num_inc(scope);
  opcode_rep = opcode;
  char* tmp_name = el_opcode_to_string_map.value(opcode_rep) ;
  src_num = MDES_src_num(tmp_name);
  dest_num = MDES_dest_num(tmp_name);
  is_predicated = MDES_predicated(tmp_name);
  srcs = new Vector<Operand>(src_num + 1);
  dests = new Vector<Operand>(dest_num + 1);
  if (!is_predicated) (*srcs)[0] = Operand(new Undefined);
  sched_time_rep = -1;
//  attributes = new Graph_attribute ;
}

Op::Op(const Opcode& opcode, int id, Procedure *scope)
   : sched_opcode_rep(NULL), custom_opcode_rep(NULL), mem_out(NULL), mem_in(NULL), mem_kill(NULL), related_ops(NULL)
{
  id_rep = id;
  if(!scope)
    scope = ::default_scope;
  // If still scope==NULL, this Op does not come from source code. E.g. it is a function
  // unit allocated in the MultiLoop. -- Luca 9/24/01.
  if (scope != NULL)
    if(scope->get_max_op_id() <= id) 
      scope->set_max_op_id(id + 1);
  opcode_rep = opcode;
  src_num = MDES_src_num(el_opcode_to_string_map.value(opcode_rep));
  dest_num = MDES_dest_num(el_opcode_to_string_map.value(opcode_rep));
  is_predicated = MDES_predicated(el_opcode_to_string_map.value(opcode_rep));
  srcs = new Vector<Operand>(src_num + 1);
  dests = new Vector<Operand>(dest_num + 1);
  if (!is_predicated) (*srcs)[0] = Operand(new Undefined);
  sched_time_rep = -1;
}

Op::Op(int id, Procedure *scope) 
   : sched_opcode_rep(NULL), custom_opcode_rep(NULL), mem_out(NULL), mem_in(NULL), mem_kill(NULL), related_ops(NULL)
{
  id_rep = id;
  if(!scope)
    scope = ::default_scope;
  if(scope->get_max_op_id() <= id) 
    scope->set_max_op_id(id + 1);
  srcs = new Vector<Operand>;
  dests = new Vector<Operand>;
  sched_time_rep = -1;
}


Op::Op(const Opcode& opcode, int sources, int destinations, bool predicated, Procedure *scope)
   : sched_opcode_rep(NULL), custom_opcode_rep(NULL), mem_out(NULL), mem_in(NULL), mem_kill(NULL), related_ops(NULL)
{
  id_rep =  get_next_op_id_num_inc(scope);
  opcode_rep = opcode;
  src_num = sources;
  dest_num = destinations;
  is_predicated  = predicated;
  srcs = new Vector<Operand>(src_num + 1);
  dests = new Vector<Operand>(dest_num + 1);
  if (!predicated) (*srcs)[0] = Operand(new Undefined);
  sched_time_rep = -1;
//  attributes = new Graph_attribute ;
}

Op::Op(const Op& op)
   : Region(op), id_rep(op.id_rep),
     opcode_rep(op.opcode_rep), src_num(op.src_num),
     dest_num(op.dest_num), is_predicated(op.is_predicated),
     sched_time_rep(op.sched_time_rep), mem_out(NULL),
     mem_in(NULL), mem_kill(NULL), related_ops(NULL)
{
   if (op.mem_out) { mem_out = new Hash_set<Operand>(*(op.mem_out)) ;}
   if (op.mem_in) { mem_in = new Hash_set<Operand>(*(op.mem_in)) ;}
   if (op.mem_kill) { mem_kill = new Hash_set<Operand>(*(op.mem_kill)) ;}
   if (op.related_ops) { related_ops = new List<Op*>(*(op.related_ops)) ;}
   if (op.sched_opcode_rep) {
       int len = strlen(op.sched_opcode_rep);
       sched_opcode_rep = new char[len + 1];
       strcpy(sched_opcode_rep, op.sched_opcode_rep);
   }
   else {
       sched_opcode_rep = NULL;
   }

   /*modification for custom_opcode (scott/wilkin 05-31-02)*/
   if (op.custom_opcode_rep) {
       int len = strlen(op.custom_opcode_rep);
       custom_opcode_rep = new char[len + 1];
       strcpy(custom_opcode_rep, op.custom_opcode_rep);
   }
   else {
       custom_opcode_rep = NULL;
   }
   
   srcs = new Vector<Operand>(*(op.srcs));
   dests = new Vector<Operand>(*(op.dests));
//  attributes = new Graph_attribute ;

   // Need to create new edges instead of copying the pointers!
   for(List_iterator<Edge*> li(op.edges_in); li != 0; li++) {
     Edge* old_edge = *li;
     Edge* new_edge = old_edge->clone();
     assert(new_edge && "out of memory");
     new_edge->set_dest(this, new_edge->dest_port(), new_edge->dest_alt());
     edges_in.add_tail(new_edge);
   }
   for(List_iterator<Edge*> li(op.edges_out); li != 0; li++) {
     Edge* old_edge = *li;
     Edge* new_edge = old_edge->clone();
     assert(new_edge && "out of memory");
     new_edge->set_src(this, new_edge->src_port(), new_edge->src_alt());
     edges_out.add_tail(new_edge);
   }
}

Op* Op::clone() const {
   Op* temp = new Op(*this);
   return temp;
}

Op::~Op() {
  delete srcs;
  delete dests;
  if (mem_out) delete mem_out;
  if (mem_in) delete mem_in;
  if (mem_kill) delete mem_kill;
  if (related_ops) delete related_ops;
  if (sched_opcode_rep) delete [] sched_opcode_rep;
  if (custom_opcode_rep) delete [] custom_opcode_rep;
//  delete attributes;
}

Op& Op::operator=(const Op& op) {
   if (this != &op) {
      { // Do the base class fields first 
	 region_flags = op.region_flags ;
	 parent_region = op.parent_region ;
	 delete attributes ; // This is safe if attributes is NULL
	 attributes = new Graph_attribute(*op.attributes) ;
      }
      id_rep = op.id_rep;
      opcode_rep = op.opcode_rep;
      src_num = op.src_num;
      dest_num = op.dest_num;
      is_predicated = op.is_predicated;
      *srcs = *(op.srcs);
      *dests = *(op.dests);
      if (mem_out) {
         delete mem_out;
         mem_out = NULL;
      }
      if (op.mem_out) { mem_out = new Hash_set<Operand>(*(op.mem_out)) ;}
      if (mem_in) {
         delete mem_in;
         mem_in = NULL ;
      }
      if (op.mem_in) { mem_in = new Hash_set<Operand>(*(op.mem_in)) ;}
      if (mem_kill) {
         delete mem_kill;
         mem_kill = NULL ;
      }
      if (op.mem_kill) { mem_in = new Hash_set<Operand>(*(op.mem_kill)) ;}

      if (related_ops) {
         delete related_ops;
         related_ops = NULL ;
      }
      if (op.related_ops) { related_ops = new List<Op*>(*(op.related_ops)) ;}
      edges_in.clear();
      edges_out.clear();
      sched_time_rep = op.sched_time_rep;
      if (sched_opcode_rep) {
         delete [] sched_opcode_rep;
         sched_opcode_rep = NULL;
      }
      if (op.sched_opcode_rep) {
         int len = strlen(op.sched_opcode_rep);
         sched_opcode_rep = new char[len + 1];
         strcpy(sched_opcode_rep, op.sched_opcode_rep);
      }

      /*modification for custom_opcode (scott/wilkin 05-31-02)*/
      if (custom_opcode_rep) {
         delete [] custom_opcode_rep;
         custom_opcode_rep = NULL;
      }
      if (op.custom_opcode_rep) {
         int len = strlen(op.custom_opcode_rep);
         custom_opcode_rep = new char[len + 1];
         strcpy(custom_opcode_rep, op.custom_opcode_rep);
      }
//    delete attributes ;
//    attributes = new Graph_attribute(*(op.attributes)) ;
   }
   return *this;
}

bool Op::operator==(const Op& op) const {
  return ((opcode_rep == op.opcode_rep) && (*srcs == *(op.srcs)) &&
	  (*dests == *(op.dests)));
}

bool Op::operator!=(const Op& op) const {
  return !operator==(op);
}

bool Op::is_op() const {return true;}
bool Op::is_pseudo_op() { return opcode_rep & IR_PSEUDO; }
int Op::id() const {return id_rep;}
int Op::num_srcs() const {return (src_num);}
int Op::num_dests() const {return dest_num;}

int Op::num_mem_srcs() const
{
   if (mem_in) return (mem_in->size());
   else return 0 ;
}

int Op::num_mem_dests() const
{
   if (mem_out) return (mem_out->size());
   else return 0 ;
}

int Op::num_mem_kills() const
{
   return num_mem_dests() ; //HACK
    if (mem_kill) return (mem_kill->size());
    else return 0 ;
}


bool Op::predicated() const {return is_predicated;}

Operand& Op::src(Port_num port) const { return (*srcs)[port]; }
Operand& Op::dest(Port_num port) const { return (*dests)[port]; }

static Hash_set<Operand> null_mem(hash_operand) ;
Hash_set<Operand>& Op::mem_srcs() 
{ 
   if (mem_in)  return (*mem_in) ; 
   else return null_mem ;
}

Hash_set<Operand>& Op::mem_dests() 
{ 
   if (mem_out) return (*mem_out) ; 
   else return null_mem ;
}

Hash_set<Operand>& Op::mem_kills() 
{
   return Op::mem_dests() ; //HACK
   if (mem_kill) return (*mem_out) ; 
   else return null_mem ;
}

Port_num Op::first_input() const
{
   if (is_predicated) return PRED1 ;
   else return SRC1 ;
}

Port_num Op::last_input() const
{
   return (Port_num)src_num ;
} 

Port_num Op::first_output() const
{
   return DEST1;
}

Port_num Op::last_output() const
{
   return (Port_num)dest_num ;
}

Port_num Op::pred() const
{
    return PRED1;
}

Port_num Op::first_src() const
{
   return SRC1 ;
}

Port_num Op::last_src() const
{
   return (Port_num)src_num ;
}

Port_num Op::first_dest() const
{  
   return DEST1;
}

Port_num Op::last_dest() const
{
   return (Port_num)dest_num ;
}

Opcode Op::opcode() const { return opcode_rep; }
void Op::or_opcode(Opcode mask) { opcode_rep = (Opcode)(opcode_rep | mask); }
void Op::set_opcode(Opcode new_opcode) { opcode_rep = new_opcode; }

// Update src_num, dest_num, is_predicated and corresponding Vectors
// based on current opcode.  Should be used when you do a set_opcode.
void Op::update_num_operands()
{
    int new_src_num, new_dest_num, new_is_predicated;
    Operand undef;

    new_src_num = MDES_src_num(get_mdes_opcode_string(this));
    new_dest_num = MDES_dest_num(get_mdes_opcode_string(this));
    new_is_predicated = MDES_predicated(get_mdes_opcode_string(this));

    if (new_src_num != src_num) {
	srcs->resize((new_src_num+1), undef);
	src_num = new_src_num;
    }
    if (new_dest_num != dest_num) {
	dests->resize((new_dest_num+1), undef);
	dest_num = new_dest_num;
    }
    if (new_is_predicated != is_predicated) {
	is_predicated = new_is_predicated;
    }
}

/*modification for custome_opcode (scott/wilkin 05-31-02)*/
char* Op::custom_opcode() const { return custom_opcode_rep; }

char* Op::sched_opcode() const { return sched_opcode_rep; }
int Op::sched_time() const { return sched_time_rep; }

bool Op::is_speculative() const
{
    if ((Region *)this->flag(EL_OPER_SPECULATIVE))
	return (true);
    else
	return (false);
}

int Op::relative_dest_latency(Port_num dest_pos) {
// used for creation of IMPACT's isl attribute in el_elcor_to_lcode translation.
  char iopat[MAXIOPATLEN];

  // Wilkin
  // if (!el_opcode_to_string_map.is_bound(opcode_rep))
  if (!get_mdes_opcode_string(this))
    El_punt ("invalid to get latency for this oper");

  if (dest_pos <= dest_num) {
    build_io_pat(iopat);
    // Wilkin
    // MDES_init_op_io(el_opcode_to_string_map.value(opcode_rep), iopat);
    MDES_init_op_io(get_mdes_opcode_string(this), iopat);
    return (MDES_flow_time_io(DATA_OUT, dest_pos));
  }
  else
    return (-1);
}

void Op::set_id(int id) {id_rep = id ; } ;

void Op::renumber(Procedure *scope)
{
  id_rep = get_next_op_id_num_inc(scope);
}

Op& Op::set_dest(Port_num port, const Operand& opnd) {
  if (port >= dests->dim()) {
    assert(opnd.is_undefined());
    return *this;
  }
  (*dests)[port] = opnd;
  return *this;
}

Op& Op::set_src(Port_num port, const Operand& opnd) {
  if (port >= srcs->dim()) {
    assert(opnd.is_undefined());
    return *this;
  }
  (*srcs)[port] = opnd; 
  return *this;
}

void
Op::set_dest_spatial_ref(Port_num pn, Spatial_ref sr) {
   Spatial_ref_attr *sra = get_spatial_ref_attr(this);

   // cases:
   // sra==NULL and sr.is_null()  ---> do nothing
   // sra==NULL, !sr.is_null --> create sra and stick in sr
   // sra!=NULL, sr != sra->dest(pn) --> set_dest(pn,sr), if sra->is_null remove
   // sra!=NULL, sr == sra->dest(pn) ---> do nothing

   if (sra == NULL) {
      if (!sr.is_null()) {
	 sra = new Spatial_ref_attr;
	 sra->set_dest(pn, sr);
	 set_spatial_ref_attr(this, sra);
      }
   }
   else {
      if (sr != sra->dest(pn)) {
	 sra->set_dest(pn, sr);
	 // If we set the only spatial_ref to an sr for which is_null is true,
	 // then the spatial_ref_attr becomes null, so remove it.
	 if (sra->is_null()) {
	    remove_spatial_ref_attr(this);
	 }
      }
   }
}


void
Op::set_src_spatial_ref(Port_num pn, Spatial_ref sr) {
   Spatial_ref_attr *sra = get_spatial_ref_attr(this);

   // cases:
   // sra==NULL and sr.is_null()  ---> do nothing
   // sra==NULL, !sr.is_null --> create sra and stick in sr
   // sra!=NULL, sr != sra->src(pn) --> set_src(pn,sr), if sra->is_null remove
   // sra!=NULL, sr == sra->src(pn) ---> do nothing

   if (sra == NULL) {
      if (!sr.is_null()) {
	 sra = new Spatial_ref_attr;
	 sra->set_src(pn, sr);
	 set_spatial_ref_attr(this, sra);
      }
   }
   else {
      if (sr != sra->src(pn)) {
	 sra->set_src(pn, sr);
	 // If we set the only spatial_ref to an sr for which is_null is true,
	 // then the spatial_ref_attr becomes null, so remove it.
	 if (sra->is_null()) {
	    remove_spatial_ref_attr(this);
	 }
      }
   }
}


Op& Op::add_mem_src(Operand& opnd) {
   if (!mem_in) { mem_in = new Hash_set<Operand> (hash_operand, 16); }
   (*mem_in) += opnd;
   return *this;
}

Op& Op::add_mem_src_unsafe(Operand& opnd) {
   if (!mem_in) { mem_in = new Hash_set<Operand> (hash_operand, 16); }
   (*mem_in).add_unsafe(opnd);
   return *this;
}

Op& Op::remove_mem_src(Operand& opnd)
{
   if (!mem_in) {
      return *this ;
   }
   else {
      (*mem_in) -= opnd;
      if (mem_in->is_empty()) {
	 delete mem_in;
	 mem_in = NULL ;
      }
      return *this;
   }
}

Op& Op::add_mem_dest(Operand& opnd)
{
  if (!mem_out) { mem_out = new Hash_set<Operand>(hash_operand, 16) ; }
  (*mem_out) += opnd;
  return *this;
}

Op& Op::remove_mem_dest(Operand& opnd)
{
   if (!mem_out) {
      return *this ;
   }
   else {
      (*mem_out) -= opnd;
      if (mem_out->is_empty()) {
	 delete mem_out;
	 mem_out = NULL ;
      }
      return *this;
   }
}

Op& Op::add_mem_kill(Operand& opnd)
{
   assert(0) ; // kill set needs to be implemented first
   return *this;
}

Op& Op::remove_mem_kill(Operand& opnd)
{
   assert(0) ; // kill set needs to be implemented first
   return *this;
}

/*modification for custome_opcode (scott/wilkin 05-31-02)*/
void Op::set_custom_opcode(char* opc)
{
   if (opc != NULL) {
      int len = strlen(opc);
      custom_opcode_rep = new char[len+1];
      strcpy(custom_opcode_rep, opc);
   }
   else
      custom_opcode_rep = NULL;
}

void Op::set_sched_opcode(char* opc)
{
   if (sched_opcode_rep) delete[] sched_opcode_rep;
   if (opc != NULL) {
      int len = strlen(opc);
      sched_opcode_rep = new char[len+1];
      strcpy(sched_opcode_rep, opc);
   }
   else
      sched_opcode_rep = NULL;
}

void Op::set_sched_time(int scheduled_time) { 
  sched_time_rep = scheduled_time; 
}

void Op::set_speculative(bool val)
{
   if (val==true)
      ((Region *) this)->set_flag(EL_OPER_SPECULATIVE);
   else
      ((Region *) this)->reset_flag(EL_OPER_SPECULATIVE);
}

Op& Op::add_relops(Op* op){
   if (!related_ops) { related_ops = new List<Op*> ; }
   related_ops->add_tail(op) ;
   return *this;
}

Op& Op::remove_relops(Op* op){
   if (!related_ops) {
      return *this ;
   }
   else {
      related_ops->remove(op);
      if (related_ops->is_empty()) {
	 delete related_ops;
	 related_ops = NULL ;
      }
      return *this;
   }
}

Op& Op::remove_all_relops(){
   if (related_ops) {
      delete related_ops ;
   }
   related_ops = NULL ;
   return *this;
}

static List<Op*> null_relops ;

List<Op*>& Op::get_relops() {
   if (related_ops) { return (*related_ops) ; }
   else { return(null_relops) ; }
}

Op& Op::add_inedge(Edge* edge, Port_type type, Port_num num, Alt_num alt){
  if (check_port(type, num))
    edges_in.add_tail(edge);
  else assert(0);
  return *this;
}
Op& Op::add_outedge(Edge* edge, Port_type type, Port_num num, Alt_num alt){
  if (check_port(type, num))
    edges_out.add_tail(edge);
  else {
   cerr << *edge << endl ;
   cerr << *this << endl ;
   assert(0);
  }
  return *this;
}
Op& Op::remove_inedge(Edge* edge, Port_type type, Port_num num, Alt_num alt){
  if (check_port(type, num))
    edges_in.remove(edge);
  else assert(0);
  return *this;
}

Op& Op::remove_outedge(Edge* edge, Port_type type, Port_num num, Alt_num alt){
  if (check_port(type, num))
    edges_out.remove(edge);
  else assert(0);
  return *this;
}

//Given an Op op1, and Edge e (between some Op op0 and op1, say), this
//routine adds e to the entry edge sets of all ancestors of op1
//that are actually entered by e (as opposed to all ancestors for which
// op1 is an entry op);              thus, if op0 and op1 are in
//two basic blocks b0 and b1, which in turn are in a hyperblock hb,
//the edge e would not be added as an inedge of hb (even though
//op1 may be an inop of hb). In many cases, this is the behavior
//we expect/want. MP
Op& Op::add_inedge_recursively_restricted(Edge* edge)
{
  
  Compound_region* reg_ptr;
  
  Compound_region *anc_second,*anc_first;
  Compound_region* ica=innermost_common_ancestor((Region*)this,
						 (Region*)edge->dest(),
						 anc_second,
						 anc_first);
  
 
  reg_ptr = this->parent();
  while(reg_ptr!=ica)
    {
      if (reg_ptr->is_entry_op(this))
	{
	  reg_ptr->add_entry_safely(edge);
	}
      else
	{
	  break;
	}
      reg_ptr = reg_ptr->parent();
    }
   if (!edges_in.is_member(edge)) edges_in.add_tail(edge);
   
   return *this;
}

//see comment above add_inedge_recursively_restricted
Op& Op::add_outedge_recursively_restricted(Edge* edge)
{
  Compound_region* reg_ptr;
  
  Compound_region *anc_second,*anc_first;
  Compound_region* ica=innermost_common_ancestor((Region*)this,
						 (Region*)edge->dest(),
						 anc_second,
						 anc_first);
  
  reg_ptr = this->parent();
  while(reg_ptr!=ica)
    {
      if (reg_ptr->is_exit_op(this))
	{
	  reg_ptr->add_exit_safely(edge);
	}
      else
	{
	  break;
	}
      reg_ptr = reg_ptr->parent();
    }

  if (!edges_out.is_member(edge)) edges_out.add_tail(edge);
  
  return *this;
}

Op& Op::add_inedge_recursively(Edge* edge)
{
  
  Compound_region* reg_ptr;
  
  reg_ptr = this->parent();
  while(reg_ptr)
    {
      if (reg_ptr->is_entry_op(this))
	{
	  reg_ptr->add_entry_safely(edge);
	}
      else
	{
	  break;
	}
      reg_ptr = reg_ptr->parent();
    }
   
   if (!edges_in.is_member(edge)) edges_in.add_tail(edge);
   
   return *this;
}

Op& Op::add_outedge_recursively(Edge* edge)
{
  Compound_region* reg_ptr;
  
   reg_ptr = this->parent();
  while(reg_ptr)
    {
      if (reg_ptr->is_exit_op(this))
	{
	  reg_ptr->add_exit_safely(edge);
	}
      else
	{
	  break;
	}
      reg_ptr = reg_ptr->parent();
    }

  if (!edges_out.is_member(edge)) edges_out.add_tail(edge);
  
  return *this;
}

Op& Op::remove_inedge_recursively(Edge* edge)
{
  Compound_region* reg_ptr;

  reg_ptr = this->parent();
  while(reg_ptr)
    {
      reg_ptr->remove_entry(edge);
      reg_ptr = reg_ptr->parent();
    }
  
  edges_in.remove(edge);

  return *this;
}

Op& Op::remove_outedge_recursively(Edge* edge)
{
  Compound_region* reg_ptr;

  reg_ptr = this->parent();
  while(reg_ptr)
    {
      reg_ptr->remove_exit(edge);
      reg_ptr = reg_ptr->parent();
    }
  
  edges_out.remove(edge);
  
  return *this;
}


//the op is removable from the set of exit ops of a compound region
//only if "edge" is the only exit edge of that compound region from
//that op
bool is_removable_outop(Edge* edge, Compound_region *cr,Op* op)
{
  
  for(Region_exit_edges e_iter(cr);e_iter!=0;e_iter++)
    {
      Edge* new_edge=*e_iter;
      if((edge!=new_edge)&&
	 (new_edge->src()==op))
	return false;  
    } 
  return true;
}

//the op is removable from the set of entry ops of a compound region
//only if "edge" is the only entry edge of that compound region into
//the op
bool is_removable_inop(Edge* edge, Compound_region *cr,Op* op)
{

  for(Region_entry_edges e_iter(cr);e_iter!=0;e_iter++)
    {
      Edge* new_edge=*e_iter;
      if((edge!=new_edge)&&
	 (new_edge->dest()==op))
	return false;
    }

  return true;
}


Op& Op::remove_outop_recursively(Edge* edge)
{
  
  Compound_region *anc_second,*reg,*anc_first;
  Compound_region* ica=innermost_common_ancestor((Region*)this,
						 (Region*)(edge->dest()),
						 anc_second,
						 anc_first);


  for(reg=this->parent();reg!=ica;reg=reg->parent())
    {
      if(is_removable_outop(edge,reg,this))
	reg->remove_exit(this);
    }

  return *this;  
}


Op& Op::remove_inop_recursively(Edge* edge)
{
  
  Compound_region *anc_second,*reg,*anc_first;
  Compound_region* ica=innermost_common_ancestor((Region*)this,
						 (Region*)edge->src(),
						 anc_second,
						 anc_first);
  for(reg=this->parent();reg!=ica;reg=reg->parent())
    {
      if(is_removable_inop(edge,reg,this))
        reg->remove_entry(this);
    }      

  return *this;  
}


Op& Op::add_outop_recursively(Edge* edge)
{
  
  Compound_region *anc_second,*reg,*anc_first;
  Compound_region* ica=innermost_common_ancestor((Region*)this,
						 (Region*)edge->dest(),
						 anc_second,
						 anc_first);

  // For known acyclic regions check if "edge" is a cyclic edge, if so, then
  // need to add that op as an exit op.
  while (ica->is_hb() || ica->is_bb()) {
    Op *first_reg_op = get_first_region_op_from_subregions(ica);
    if (edge->dest() != first_reg_op)
      break;
    ica = ica->parent();
  }

  for(reg=this->parent();reg!=ica;reg=reg->parent())
    reg->add_exit(this);
  
  return *this;
}

Op& Op::add_inop_recursively(Edge* edge)
{
  
  Compound_region *anc_second,*reg,*anc_first;
  Compound_region* ica=innermost_common_ancestor((Region*)this,
						 (Region*)edge->src(),
						 anc_second,
						 anc_first);
  for(reg=this->parent();reg!=ica;reg=reg->parent())
    reg->add_entry(this);

  return *this;
}


Op* Op::prev()
{
   Dnode<Region*>* nd = dnode() ;
   Dnode<Region*>* prev_nd = nd->prev;
   if (prev_nd == NULL) {
      return NULL;
   }
   else {
      Op* prev_op =  (Op*) prev_nd->value() ;
      if (is_switch(prev_op) || is_merge(prev_op)) {
	 return(prev_op->prev()) ;
      }
      else return(prev_op) ;
   }
}

Op* Op::next()
{
   Dnode<Region*>* nd = dnode() ;
   Dnode<Region*>* next_nd = nd->next;
   if (next_nd == NULL) {
      return NULL;
   }
   else {
      Op* next_op =  (Op*) next_nd->value() ;
      if (is_switch(next_op) || is_merge(next_op)) {
	 return(next_op->next()) ;
      }
      return (next_op) ;
   }
}

static void
append_io_pattern(Operand &operand, char *iopat, bool is_dest, Op* op)
{
   if (!operand.assigned_to_physical_file()) {
      if (operand.is_undefined() && is_dest && is_cmpp(op))
	 strcat(iopat, vregfile_to_mdesname(PR));

//     else if (operand.is_predicate()) {
// 	 //	 strcat(iopat, "_");
//       strcat(iopat, vregfile_to_mdesname(PR));
//     }
// above case handled in operand_to_mdesname -KF 12/2004

      else {
	 char* name = operand_to_mdesname(operand);	 
	 strcat(iopat, name);
      }
   }

   else {
      strcat(iopat, (char*) operand.physical_file_type());
   }

   // replaced code below with more compact version that uses
   // operand_to_mdesname func. KF 8-2001

//    if (operand.is_lit()) {
//       if (operand.assigned_to_physical_file())
// 	 strcat(iopat, (char*)operand.physical_file_type());
//       else
// 	 //strcat(iopat, "L");
//    }
//    else if (operand.is_reg() || operand.is_macro_reg()) {
//       if (operand.assigned_to_physical_file())
// 	 strcat(iopat, (char*)operand.physical_file_type());
//       else
// 	 strcat(iopat, vregfile_to_char(operand.file_type()));
//    }
//    else if (operand.is_undefined())  {
//       // Special cases to convert u into proper register formats for 
//       // multi-destination ops. Right now we handle only compare to predicate 
//       // ops.
//       if (is_dest && is_cmpp(op)) {
// 	  strcat(iopat, vregfile_to_char(PR));
//       }
//       else {
// 	  strcat(iopat, "U");
//       }
//    }
//    else  // Why this case -- Are there any cases where operands are bogus???
//       strcat(iopat, "U");
//  Mem_vr's  should not come here from edge_drawing (?) -- SGA
//  cerr << "append_io_pattern: Could not determine io pat for operand"
//  	<< endl << operand << endl;
}

void
Op::build_io_pat(char *iopat)
{
  int i, src_after_pred;

  iopat[0] = '\0';

  if (is_predicated) {
    // We translate "p" directly to avoid getting back a literal file binding
    // -- Shail Aditya 10/28/97

    // Moved all special cases of predicate lits and undefined cmpp dests
    // to append_io_pattern.  - KF 8-2001

    Operand& operand = (*srcs)[first_input()];
    append_io_pattern(operand, iopat, false, this);
    strcat(iopat, "? ");
    src_after_pred = first_input() + 1; 
  } else {
    src_after_pred = first_input();
  }

  for (i = src_after_pred ; i <= last_input(); i++) {
    if (i != src_after_pred) strcat(iopat, ",");
    Operand &operand = (*srcs)[i];
    append_io_pattern(operand, iopat, false, this);
  }
  strcat(iopat,":");
  for (i = first_dest(); i <= last_dest(); i++) {
    if (i != first_dest()) strcat(iopat, ",");
    Operand &operand = (*dests)[i];
    append_io_pattern(operand, iopat, true, this);
  }
}

int Op::flow_time(Port_type type, Port_num num, Port_dir dir)
{
   char iopat[MAXIOPATLEN];

   // FIX: CCA patterns get compressed to custom ops, and they aren't
   // necessarily in the mdes.
   if(opcode_rep == CUSTOM) {
     switch(type) {
     case CONTROL: 
       return 0;
     default:
       return 1;
     }
   }

   // if (el_opcode_to_string_map.is_bound(opcode_rep)) {
   if (get_mdes_opcode_string(this)) {
      build_io_pat(iopat);
      switch(type) {
       case SRC:
	 // MDES_init_op_io(el_opcode_to_string_map.value(opcode_rep), iopat);
	 MDES_init_op_io(get_mdes_opcode_string(this), iopat);
	 return (MDES_flow_time_io(DATA_IN, num));
       case DEST:
	 // MDES_init_op_io(el_opcode_to_string_map.value(opcode_rep), iopat);
	 MDES_init_op_io(get_mdes_opcode_string(this), iopat);
	 return (MDES_flow_time_io(DATA_OUT, num));
       case MEM:
	 // MDES_init_op_io(el_opcode_to_string_map.value(opcode_rep), iopat);
	 MDES_init_op_io(get_mdes_opcode_string(this), iopat);
	 return	((dir == IN) ? MDES_flow_time_io(SYNC_IN, num) :
		 MDES_flow_time_io(SYNC_OUT, num));
;
       case CONTROL:
       // Pessimistically sets op-> br latency to 0 for now instead of
       //  0 - (branch latncy - 1)
	 if ((num == CONTROL1) && (dir == OUT)) {
	    if (is_branch (this)) {
	       // return (MDES_branch_latency(el_opcode_to_string_map.value(opcode_rep)));
	       return (MDES_branch_latency(get_mdes_opcode_string(this)));
	    }
	    else {
	       return (0);
	    }
	 }
	 else {
	    return (0);
	 }
       default:
	 cerr << "Op::flow_time: Could not compute flow_time latency, "
	      << "assuming 0 latency" << endl;
	 return (0);
      }
   }
   else {
      return(0) ;
   }
}

int Op::anti_time(Port_type type, Port_num num, Port_dir dir)
{
   char iopat[MAXIOPATLEN];

   // FIX: CCA patterns get compressed to custom ops, and they aren't
   // necessarily in the mdes.
   if(opcode_rep == CUSTOM) {
     switch(type) {
     case CONTROL: 
       return 0;
     default:
       return 1;
     }
   }

   // if (el_opcode_to_string_map.is_bound(opcode_rep)) {
   if (get_mdes_opcode_string(this)) {
      build_io_pat(iopat);
      switch(type) {
       case SRC:
	 // MDES_init_op_io(el_opcode_to_string_map.value(opcode_rep), iopat);
	 MDES_init_op_io(get_mdes_opcode_string(this), iopat);
	 return (MDES_anti_time_io(DATA_IN, num));
       case DEST:
	 // MDES_init_op_io(el_opcode_to_string_map.value(opcode_rep), iopat);
	 MDES_init_op_io(get_mdes_opcode_string(this), iopat);
	 return (MDES_anti_time_io(DATA_OUT, num));
       case MEM:
	 // MDES_init_op_io(el_opcode_to_string_map.value(opcode_rep), iopat);
	 MDES_init_op_io(get_mdes_opcode_string(this), iopat);
	 return ((dir == IN) ? MDES_anti_time_io(SYNC_IN, num) :
		 MDES_anti_time_io(SYNC_OUT, num));
       case CONTROL:
	 return (0);
       default:
	 cerr << "Op::anti_time: Could not compute anti_time latency, "
	      << "assuming 0 latency" << endl;
	 return (0); 
      }
   }
   else {
      return(0) ;
   }
}

Dlist<Region*>& Op::subregions() {return EMPTY_REGION_LIST;}
Hash_set<Op*>& Op::inops() {return EMPTY_OP_SET;}
Hash_set<Op*>& Op::outops() {return EMPTY_OP_SET;}
List<Edge*>& Op::inedges() {return edges_in;}
List<Edge*>& Op::outedges() {return edges_out;}

// ----- Copy an op and wire edges
Region* Op::copy_with_edges(Region_map& region_map, Edge_map& edge_map) {
  Edge* original;
  Edge* new_edge;
  Op* op = this;

  // clone the op and put it in the map
  Op* new_op = op->clone();
  //Get rid of the edges from the cloned version (MP 7/23/96)
  new_op->edges_in.clear();
  new_op->edges_out.clear();
  
  region_map.bind(op, new_op);

  // wire input edges
  for (Op_inedges ins(op); ins != 0; ++ins){
    original = *ins;
    if(edge_map.is_bound(original)) {
      new_edge = edge_map.value(original);
      new_edge->set_dest(new_op, original->dest_port(), original->dest_alt());
      new_op->add_inedge(new_edge, original->dest_port_type(),
			 original->dest_port(), original->dest_alt());
    }
  }

  // wire output edges
  for (Op_outedges outs(op); outs != 0; ++outs){
    original = *outs;
    if(edge_map.is_bound(original)) {
      new_edge = edge_map.value(original);
      new_edge->set_src(new_op, original->src_port(), original->src_alt());
      new_op->add_outedge(new_edge, original->src_port_type(),
			  original->src_port(), original->src_alt());
    }
  }
  return new_op;
}

bool Op::check_port(Port_type type, Port_num num) {
  return true;
  if (is_pseudo_op())
    return true;
  else
    return ((type == CONTROL && num < MAX_CONTROL_PORTS) ||
	    (type == MEM && num < MAX_MEM_PORTS) ||
	    (type == SRC && num <= src_num) ||
	    (type == DEST && num <= dest_num));
}

void Op::print(ostream& os) const {
  int i;
  os << id_rep << ": ";
  if (mem_out && (!mem_out->is_empty())) {
    Hash_set_iterator<Operand> iter(*mem_out);
    os << "<" << *iter;
    ++iter;
    for(; iter != 0; ++iter)
      os << "," << *iter;
    os << ">";
  }
  if (dest_num > 0) {
     os << (*dests)[DEST1];
     for(i = DEST1 + 1; i < DEST1 + dest_num; ++i)
	os << "," << (*dests)[i];
     os << " = " ;
  }
  os << opcode_rep << "(";
  if (src_num > 0) {
     os << (*srcs)[SRC1];
     for(i = SRC1 + 1; i < SRC1 + src_num; ++i)
        os << "," << (*srcs)[i];
  }
  os << ")";
  if (is_predicated) os << " if " << (*srcs)[PRED1];
  if (mem_in && (!mem_in->is_empty())) {
    Hash_set_iterator<Operand> iter(*mem_in);
    os << "<" << *iter;
    ++iter;
    for(; iter != 0; ++iter)
      os << "," << *iter;
    os << ">";
  }
  os << endl;
  if (sched_time_rep >= 0) {
     os << "Sched_opcode: " << sched_opcode_rep << "\tSched_time: " 
     << sched_time_rep << "\tSpeculative: ";
     if ((Region *)this->flag(EL_OPER_SPECULATIVE)) os << "Y" ;
     else os << "N" ;
     os << endl ;
  }
  if (custom_opcode_rep != NULL) {
     os << "Custom_opcode: " << custom_opcode_rep << endl;
  }
  os << "   -In edges-" << endl;
  for(Op_inedges in(this); in!=0; ++in)
   os << "    " << *(*in) << endl;
  os << "   -Out edges-" << endl;
  for(Op_outedges out(this); out!=0; ++out)
   os << "    " << *(*out) << endl;
}

ostream& operator<<(ostream& os, const Op& op){
  op.print(os);
  return os;
}



