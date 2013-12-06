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
//      File:           op_attributes.cpp
//      Authors:        Sadun Anik
//      Created:        December 1994
//      Description:    Op attributes 
//
/////////////////////////////////////////////////////////////////////////////
#include "dbg.h"
#include "op_attributes.h"
#include "iterators.h"


Merge_op_BB_id::Merge_op_BB_id() : id(0) {}

Merge_op_BB_id::~Merge_op_BB_id() {}

Base_attribute* Merge_op_BB_id::clone_attribute()
{
   Merge_op_BB_id* tmp_ptr = new Merge_op_BB_id ;
   tmp_ptr->id = id ;
   return (tmp_ptr) ;
}

Merge_op_BB_id* get_existing_merge_op_bb_id(Op* op)
{
  Merge_op_BB_id* tmp_ptr ;
  
  tmp_ptr = (Merge_op_BB_id*) (op->attributes->get_attribute(MERGE_OP_BB_ID));
  return (tmp_ptr) ;
}

Merge_op_BB_id* get_merge_op_bb_id(Op* op)
{
  Merge_op_BB_id* tmp_ptr ;
  
  tmp_ptr = (Merge_op_BB_id*) (op->attributes->get_attribute(MERGE_OP_BB_ID));
  if (tmp_ptr == 0) {
    tmp_ptr = new Merge_op_BB_id;
    op->attributes->insert_attribute(MERGE_OP_BB_ID,tmp_ptr) ;
  }
  return (tmp_ptr) ;
}

void set_merge_op_bb_id(Op* op, Merge_op_BB_id* mobbid) {
  op->attributes->insert_attribute(MERGE_OP_BB_ID, mobbid) ;
}

void remove_merge_op_bb_id(Op* op) {
   op->attributes->remove_attribute(MERGE_OP_BB_ID) ;
}

/////////////////////////////////////////////////////////////////////////////

Branch_info_attr::Branch_info_attr()
{
    pbr_id = -1;
    pbr_op = 0;
    cmpp_id = -1;
    cmpp_op = 0;
    ld_id = -1;
    ld_op = 0;
    addr_calc_id = -1;
    addr_calc_op = 0;
    cmpp_tgt=0;
    pbr_tgt=0;
    ld_tgt=0;
    addr_calc_attrs=0;
    cmpp_attrs=0;
    ld_attrs=0;
    addr_calc_attrs=0;
    br_attrs=0;
}

Branch_info_attr::~Branch_info_attr() {}

Base_attribute* Branch_info_attr::clone_attribute()
{
   Branch_info_attr* tmp_ptr = new Branch_info_attr ;
   tmp_ptr->pbr_id = pbr_id ;
   tmp_ptr->pbr_op = pbr_op ;
   tmp_ptr->cmpp_id = cmpp_id ;
   tmp_ptr->cmpp_op = cmpp_op ;
   tmp_ptr->ld_id = ld_id ;
   tmp_ptr->ld_op = ld_op ;
   tmp_ptr->addr_calc_id = addr_calc_id ;
   tmp_ptr->addr_calc_op = addr_calc_op ;

   tmp_ptr->cmpp_tgt = cmpp_tgt ;
   tmp_ptr->pbr_tgt = pbr_tgt ;
   tmp_ptr->ld_tgt = ld_tgt ;
   tmp_ptr->addr_calc_tgt = addr_calc_tgt ;

   tmp_ptr->pbr_attrs = pbr_attrs ;
   tmp_ptr->cmpp_attrs = cmpp_attrs ;
   tmp_ptr->ld_attrs = ld_attrs ;
   tmp_ptr->addr_calc_attrs = addr_calc_attrs ;
   tmp_ptr->br_attrs = br_attrs ;
   
   return (tmp_ptr) ;
}

Branch_info_attr* get_branch_info_attr(Op* op)
{
  Branch_info_attr* tmp_ptr ;

  tmp_ptr = (Branch_info_attr*) (op->attributes->get_attribute(BRANCH_INFO_ATTR));
  if (tmp_ptr == 0) {
    tmp_ptr = new Branch_info_attr;
    op->attributes->insert_attribute(BRANCH_INFO_ATTR,tmp_ptr) ;
  }
  return (tmp_ptr) ;
}

void set_branch_info_attr(Op* op, Branch_info_attr* data) {
  op->attributes->insert_attribute(BRANCH_INFO_ATTR, data) ;
}

void remove_branch_info_attr(Op* op) {
   op->attributes->remove_attribute(BRANCH_INFO_ATTR) ;
}

/////////////////////////////////////////////////////////////////////////////


Reference_dtype_attr::Reference_dtype_attr()
{ 
}

Reference_dtype_attr::Reference_dtype_attr(Op *op)
{ 
    this->resize(op);
}

Reference_dtype_attr::~Reference_dtype_attr()
{
}

Base_attribute* Reference_dtype_attr::clone_attribute()
{
   Reference_dtype_attr * tmp_ptr = new Reference_dtype_attr;
   tmp_ptr->src_refdtype = src_refdtype;
   tmp_ptr->dest_refdtype = dest_refdtype;
   return (tmp_ptr) ;
}

DType &Reference_dtype_attr::src(Port_num pn)
{
    return (src_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(pn)]);
}

void Reference_dtype_attr::set_src(Port_num pn, DType dt)
{
    src_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(pn)] = dt;
}

int Reference_dtype_attr::num_src()
{
    return (src_refdtype.size());
}

DType &Reference_dtype_attr::dest(Port_num pn)
{
    return (dest_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(pn)]);
}

void Reference_dtype_attr::set_dest(Port_num pn, DType dt)
{
    dest_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(pn)] = dt;
}

int Reference_dtype_attr::num_dest()
{
    return (dest_refdtype.size());
}

// Exchange info related to desta and destb
// Note, vectors are indexed 0 relative, whereas ports are indexed 1 relative.
void Reference_dtype_attr::swap_dests(Port_num a, Port_num b)
{
    DType temp = dest_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(b)];
    dest_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(b)] = dest_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(a)];
    dest_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(a)] = temp;

}

// Exchange info related to srca and srcb
// Note, vectors are indexed 0 relative, whereas ports are indexed 1 relative.
void Reference_dtype_attr::swap_srcs(Port_num a, Port_num b)
{
    DType temp = src_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(b)];
    src_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(b)] = src_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(a)];
    src_refdtype[GET_REF_DTYPE_INDEX_FROM_PN(a)] = temp;
}

void Reference_dtype_attr::resize(Op *op)
{
    int new_src = op->num_srcs();
    int new_dst = op->num_dests();

    int old_src = src_refdtype.size();
    int old_dst = dest_refdtype.size();

    if (new_src != old_src)
	src_refdtype.resize(new_src);
    if (new_dst != old_dst)
	dest_refdtype.resize(new_dst);
}

void Reference_dtype_attr::resize_src(int new_src)
{
    int old_src = src_refdtype.size();

    if (new_src != old_src)
	src_refdtype.resize(new_src);
}

void Reference_dtype_attr::resize_dest(int new_dest)
{
    int old_dest = dest_refdtype.size();

    if (new_dest != old_dest)
	dest_refdtype.resize(new_dest);
}

// Set the reference width attribute directly from the operand
// width.  This is guaranteed to be a conservative setting.
void Reference_dtype_attr::set_srcs_conservatively(Op *op)
{
    Operand src;
    Port_num pn;
    int new_src;

    new_src = op->num_srcs();
    resize_src(new_src);

    for (Op_explicit_sources srci(op); srci!=0; srci++) {
	src = *srci;
	pn = srci.get_port_num();
	set_src(pn, src.dtype());
    }
}

void Reference_dtype_attr::set_dests_conservatively(Op *op)
{
    Operand dest;
    Port_num pn;
    int new_dest;

    new_dest = op->num_dests();
    resize_dest(new_dest);

    for (Op_explicit_dests desti(op); desti!=0; desti++) {
	dest = *desti;
	pn = desti.get_port_num();
	set_dest(pn, dest.dtype());
    }
}

void Reference_dtype_attr::set_all_conservatively(Op *op)
{
    set_srcs_conservatively(op);
    set_dests_conservatively(op);
}

Reference_dtype_attr* get_reference_dtype_attr(Op* op)
{
  Reference_dtype_attr* tmp_ptr ;

  tmp_ptr = (Reference_dtype_attr*) (op->attributes->get_attribute(REFERENCE_DTYPE_ATTR));
  if (tmp_ptr == 0) {
	return NULL;
  }
  return (tmp_ptr) ;
}

void set_reference_dtype_attr(Op* op, Reference_dtype_attr* data) {
  Reference_dtype_attr* tmp_ptr ;
  tmp_ptr = (Reference_dtype_attr*) (op->attributes->get_attribute(REFERENCE_DTYPE_ATTR));
  if ( tmp_ptr != 0 )
  {
	remove_reference_dtype_attr(op);
  }
  op->attributes->insert_attribute(REFERENCE_DTYPE_ATTR, data) ;
}

void remove_reference_dtype_attr(Op* op) {
   op->attributes->remove_attribute(REFERENCE_DTYPE_ATTR) ;
}

/////////////////////////////////////////////////////////////////////////////

Spatial_ref_attr::Spatial_ref_attr() : src_spatial_refs(hash_int), dest_spatial_refs(hash_int)
{ 
}

Spatial_ref_attr::~Spatial_ref_attr()
{
}

Base_attribute* 
Spatial_ref_attr::clone_attribute()
{
   Spatial_ref_attr * tmp_ptr = new Spatial_ref_attr;
   tmp_ptr->src_spatial_refs = src_spatial_refs;
   tmp_ptr->dest_spatial_refs = dest_spatial_refs;
   return (tmp_ptr) ;
}

Spatial_ref 
Spatial_ref_attr::src(Port_num pn)
{
   Spatial_ref sr;

   if (src_spatial_refs.is_bound(pn)) {
      sr = src_spatial_refs.value(pn);
   }
   // Else return an empty spatial_ref.  calling proc will test this
   // with is_null() method.

   return sr;
}

void 
Spatial_ref_attr::set_src(Port_num pn, Spatial_ref sr)
{
   if (src_spatial_refs.is_bound(pn)) {
      src_spatial_refs.unbind(pn);
   }

   // don't add a null src
   if (! sr.is_null()) {
      src_spatial_refs.bind(pn, sr);
   }
}

int 
Spatial_ref_attr::num_src()
{
    return (src_spatial_refs.size());
}

Spatial_ref 
Spatial_ref_attr::dest(Port_num pn)
{
   Spatial_ref sr;

   if (dest_spatial_refs.is_bound(pn)) {
      sr = dest_spatial_refs.value(pn);
   }
   // Else return an empty spatial_ref.  calling proc will test this
   // with is_null() method.

   return sr;
}

void 
Spatial_ref_attr::set_dest(Port_num pn, Spatial_ref sr)
{
   if (dest_spatial_refs.is_bound(pn)) {
      dest_spatial_refs.unbind(pn);
   }

   // don't add a null dest
   if (! sr.is_null()) { 
      dest_spatial_refs.bind(pn, sr);
   }
}

int 
Spatial_ref_attr::num_dest()
{
    return (dest_spatial_refs.size());
}

// Exchange info related to desta and destb
// Note, vectors are indexed 0 relative, whereas ports are indexed 1 relative.
void 
Spatial_ref_attr::swap_dests(Port_num a, Port_num b)
{
    Spatial_ref temp = dest(b);
    set_dest(b, dest(a));
    set_dest(a, temp);
}

// Exchange info related to srca and srcb
// Note, vectors are indexed 0 relative, whereas ports are indexed 1 relative.
void 
Spatial_ref_attr::swap_srcs(Port_num a, Port_num b)
{
    Spatial_ref temp = src(b);
    set_src(b, src(a));
    set_src(a, temp);
}

void 
Spatial_ref_attr::iterate_srcs(void) {
   refsi(src_spatial_refs);
}


void 
Spatial_ref_attr::iterate_dests(void) {
   refsi(dest_spatial_refs);
}

bool
Spatial_ref_attr::get_next_ref(Pair<Port_num, Spatial_ref> &pn_sr) {
   if (refsi == 0) {
      return false;
   }

   pn_sr = *refsi;
   refsi++;
   return true;
}


Spatial_ref_attr* get_spatial_ref_attr(Op* op)
{
  Spatial_ref_attr* tmp_ptr ;

  tmp_ptr = (Spatial_ref_attr*) (op->attributes->get_attribute(SPATIAL_REF_ATTR));
  if (tmp_ptr == 0) {
	return NULL;
  }
  return (tmp_ptr) ;
}

void set_spatial_ref_attr(Op* op, Spatial_ref_attr* data) {
  Spatial_ref_attr* tmp_ptr ;
  tmp_ptr = (Spatial_ref_attr*) (op->attributes->get_attribute(SPATIAL_REF_ATTR));
  if ( tmp_ptr != 0 )
  {
	remove_spatial_ref_attr(op);
  }
  op->attributes->insert_attribute(SPATIAL_REF_ATTR, data) ;
}

void remove_spatial_ref_attr(Op* op) {
   op->attributes->remove_attribute(SPATIAL_REF_ATTR) ;
}

/////////////////////////////////////////////////////////////////////////////

// Physical file binding attribute: Rajiv

Physfile_binding_attr::Physfile_binding_attr() : src_prf_bindings(hash_int), dest_prf_bindings(hash_int)
{
}

Physfile_binding_attr::~Physfile_binding_attr()
{
}

Base_attribute* Physfile_binding_attr::clone_attribute()
{
   Physfile_binding_attr * tmp_ptr = new Physfile_binding_attr;
   tmp_ptr->src_prf_bindings = src_prf_bindings;
   tmp_ptr->dest_prf_bindings = dest_prf_bindings;
   return (tmp_ptr) ;
}

Hash_set<eString>& Physfile_binding_attr::src(Port_num pn)
{
   return src_prf_bindings.value(pn);
}

void Physfile_binding_attr::set_src(Port_num pn, Hash_set<eString>& prf_set)
{
   src_prf_bindings.bind(pn, prf_set);
}

int Physfile_binding_attr::num_src()
{
    return (src_prf_bindings.size());
}

Hash_set<eString>& Physfile_binding_attr::dest(Port_num pn)
{
   return  dest_prf_bindings.value(pn);
}

void Physfile_binding_attr::set_dest(Port_num pn, Hash_set<eString>& prf_set)
{
   dest_prf_bindings.bind(pn, prf_set);
}

int Physfile_binding_attr::num_dest()
{
    return (dest_prf_bindings.size());
}

void Physfile_binding_attr::iterate_srcs(void)
{
   refsi(src_prf_bindings);
}

void Physfile_binding_attr::iterate_dests(void)
{
   refsi(dest_prf_bindings);
}

bool Physfile_binding_attr::get_next_ref(Pair<Port_num, Hash_set<eString> > &pn_pr)
{
   if (refsi == 0) {
      return false;
   }

   pn_pr = *refsi;
   refsi++;
   return true;
}

void Physfile_binding_attr::clear_attr_entries()
{
  Hash_set<eString> dummy(hash_estring);
  Pair<Port_num, Hash_set<eString> > p((Port_num)0, dummy);

  iterate_srcs();
  while (get_next_ref(p)) {
    Hash_set<eString>& prfset = p.second;
    prfset.clear();
  }

  iterate_dests();
  while (get_next_ref(p)) {
    Hash_set<eString>& prfset = p.second;
    prfset.clear();
  }
}
   
  

Physfile_binding_attr *get_prf_binding_attr(Op* op)
{
  Physfile_binding_attr* tmp_ptr ;

  tmp_ptr = (Physfile_binding_attr*) (op->attributes->get_attribute(PHYSFILE_BIND_ATTR));
  if (tmp_ptr == 0) {
    return NULL;
  }
  return (tmp_ptr) ;
}

void set_prf_binding_attr(Op* op, Physfile_binding_attr* data)
{
  Physfile_binding_attr* tmp_ptr ;

  tmp_ptr = (Physfile_binding_attr*) (op->attributes->get_attribute(PHYSFILE_BIND_ATTR));
  if (tmp_ptr != 0) {
    remove_prf_binding_attr(op);
  }
  op->attributes->insert_attribute(PHYSFILE_BIND_ATTR, data) ;
}

void remove_prf_binding_attr(Op* op)
{
   op->attributes->remove_attribute(PHYSFILE_BIND_ATTR);
}

/////////////////////////////////////////////////////////


Call_return_def_list::Call_return_def_list()
   : list(0)
{}

Call_return_def_list::~Call_return_def_list()
{
   delete list ;
}

Base_attribute* Call_return_def_list::clone_attribute()
{
   Call_return_def_list* tmp_ptr = new Call_return_def_list ;
   tmp_ptr->list = new List<Operand>(*list) ;
   return (tmp_ptr) ;
}

List<Operand>* get_implicit_def_list(Op* op)
{
   Call_return_def_list*  tmp_ptr ;
  
   tmp_ptr = (Call_return_def_list*)
      (op->attributes->get_attribute(CALL_RETURN_DEF_LIST));
  if (tmp_ptr == NULL) {
     return (NULL) ;
  }
   return (tmp_ptr->list) ;
}

void set_implicit_def_list(Op* op, List<Operand>* deflist) {
   Call_return_def_list* tmp_ptr = new Call_return_def_list() ;
   tmp_ptr->list = deflist ;
   op->attributes->insert_attribute(CALL_RETURN_DEF_LIST,tmp_ptr) ;
}
void remove_implicit_def_list(Op* op) {
   op->attributes->remove_attribute(CALL_RETURN_DEF_LIST) ;
}

/////////////////////////////////////////////////////////////////////////////

Call_return_use_list::Call_return_use_list()
   : list(0)
{}

Call_return_use_list::~Call_return_use_list()
{
   delete list ;
}

Base_attribute* Call_return_use_list::clone_attribute()
{
   Call_return_use_list* tmp_ptr = new Call_return_use_list ;
   tmp_ptr->list = new List<Operand>(*list) ;
   return (tmp_ptr) ;
}

List<Operand>* get_implicit_use_list(Op* op)
{
   Call_return_use_list*  tmp_ptr ;
  
  tmp_ptr = (Call_return_use_list*)
            (op->attributes->get_attribute(CALL_RETURN_USE_LIST));
  if (tmp_ptr == NULL) {
     return (NULL) ;
  }
  return (tmp_ptr->list) ;
}

void set_implicit_use_list(Op* op, List<Operand>* uselist) {
   Call_return_use_list* tmp_ptr = new Call_return_use_list() ;
   tmp_ptr->list = uselist ;
   op->attributes->insert_attribute(CALL_RETURN_USE_LIST,tmp_ptr) ;
}
void remove_implicit_use_list(Op* op) {
   op->attributes->remove_attribute(CALL_RETURN_USE_LIST) ;
}

/////////////////////////////////////////////////////////////////////////////


Jump_table_name::Jump_table_name(eString name)
{
    tbl_name = name;
}

Jump_table_name::~Jump_table_name()
{
}

Base_attribute* Jump_table_name::clone_attribute()
{
   Jump_table_name* tmp_ptr = new Jump_table_name(tbl_name) ;
   return (tmp_ptr) ;
}

Jump_table_name* get_jump_table_name(Op* op)
{
  Jump_table_name* tmp_ptr ;

  tmp_ptr = (Jump_table_name*) (op->attributes->get_attribute(JUMP_TABLE_NAME));
  return (tmp_ptr) ;
}

void set_jump_table_name(Op* op, Jump_table_name* attr)
{
  op->attributes->insert_attribute(JUMP_TABLE_NAME, attr) ;
}

void remove_jump_table_name(Op* op)
{
   op->attributes->remove_attribute(JUMP_TABLE_NAME) ;
}

/////////////////////////////////////////////////////////////////////////////

/*
 * Shivarama Rao: 20 -02 -2001.
 * added the functions which are related to operator chaining. The Op_chaining_attr
 * class stores the information about the operations that can be chained together
 * with an operation
 *
 * op_chain_list is list < op_chain> where op_chain will be list of operations
 * that can be grouped together. A operation can be part of number of such
 * groups. list<op_chain> is used for this purpose.
 */

/* Constructor function */
Op_chaining_attr:: Op_chaining_attr(const List< List<Op*> >&op_chain_info) 
{
   op_chain_list = op_chain_info;
}

/* Constructor function. Used for compatibility with Reference_dtype_attr */
Op_chaining_attr::Op_chaining_attr(Op *op) 
{
}
Op_chaining_attr::Op_chaining_attr()
{
}

/* Destructor function */
Op_chaining_attr::~Op_chaining_attr() 
{
}
  
/* function to clone the attribute.  */
Base_attribute* Op_chaining_attr::clone_attribute()
{
   Op_chaining_attr *new_attr = new Op_chaining_attr(op_chain_list);
  
   return new_attr;
}

/* returns the op_chain_list */
List< List<Op*> > &Op_chaining_attr::get_op_chain_list() 
{
    return op_chain_list;
}

/* The function sets the op_chain_list */
void Op_chaining_attr::set_op_chain_list(const List< List<Op*> > &op_chain_info)
{
   op_chain_list = op_chain_info;
}

// Remove all chains containing del_op
void Op_chaining_attr::invalidate_chains(Op *del_op, bool d_flag)
{
    List<List<Op*> > to_nuke;

    for (List_iterator<List<Op*> > li1(op_chain_list); li1!=0; li1++) {
	List<Op*> &chain = *li1;
	for (List_iterator<Op*> li2(chain); li2!=0; li2++) {
	    Op *op = *li2;
	    if (op == del_op) {
		to_nuke.add_tail(chain);
		break;
	    }
	}
    }

    // Now remove the chains in to_nuke from op_chain_list
    for (List_iterator<List<Op*> > li3(to_nuke); li3!=0; li3++) {
	List<Op*> &chain = *li3;
	if (d_flag) {
	    cdbg << "\t<";
	    for (List_iterator<Op*> li4(chain); li4!=0; li4++) {
	        Op *op = *li4;
	        cdbg << " " << op->id();
	    }
	    cdbg << " >" <<  endl;
	}
	op_chain_list.remove(chain);
    }
}

// Remove all chains containing an Op in del_ops
void Op_chaining_attr::invalidate_chains(Hash_set<Op*> &del_ops, bool d_flag)
{
    List<List<Op*> > to_nuke;

    for (List_iterator<List<Op*> > li1(op_chain_list); li1!=0; li1++) {
	List<Op*> &chain = *li1;
	for (List_iterator<Op*> li2(chain); li2!=0; li2++) {
	    Op *op = *li2;
	    if (del_ops.is_member(op)) {
		to_nuke.add_tail(chain);
		break;
	    }
	}
    }

    // Now remove the chains in to_nuke from op_chain_list
    for (List_iterator<List<Op*> > li3(to_nuke); li3!=0; li3++) {
	List<Op*> &chain = *li3;
	if (d_flag) {
	    cdbg << "\t<";
	    for (List_iterator<Op*> li4(chain); li4!=0; li4++) {
	        Op *op = *li4;
	        cdbg << " " << op->id();
	    }
	    cdbg << " >" <<  endl;
	}
	op_chain_list.remove(chain);
    }
}

/*
 * Functions to set the Op_chain_attribute of an operation */
Op_chaining_attr* get_op_chaining_attr(Op* op)
{
  Op_chaining_attr* tmp_ptr ;

  tmp_ptr = (Op_chaining_attr*) (op->attributes->get_attribute(OP_CHAINING_ATTR));
  if (tmp_ptr == 0) {
	return NULL;
  }
  return (tmp_ptr) ;
}

void set_op_chaining_attr(Op* op, Op_chaining_attr* data) {
  Op_chaining_attr* tmp_ptr ;
  tmp_ptr = (Op_chaining_attr*) (op->attributes->get_attribute(OP_CHAINING_ATTR));
  if ( tmp_ptr != 0 )
  {
	remove_op_chaining_attr(op);
  }
  op->attributes->insert_attribute(OP_CHAINING_ATTR, data) ;
}

void remove_op_chaining_attr(Op* op) {
   op->attributes->remove_attribute(OP_CHAINING_ATTR) ;
}

/////////////////////////////////////////////////////////////////////////////

Nametag_attr::Nametag_attr() {}

Nametag_attr::~Nametag_attr() {}   

Base_attribute* Nametag_attr::clone_attribute()
{
  Nametag_attr* tmp_ptr = new Nametag_attr() ;
  tmp_ptr->name = name;
  tmp_ptr->type = type;
  return(tmp_ptr) ;
}

/*
Nametag_attr* get_nametag_attr(Op *op)
{
  return( (Nametag_attr*)(op->attributes->get_attribute(NAMETAG_ATTR)) );
  // get_attribute() returns NULL ptr if attr not present.
}
*/
Nametag_attr* get_nametag_attr(Region *r)
{
  if (r->is_op()) {
    // get_attribute() returns NULL ptr if attr not present.
    return( (Nametag_attr*)(r->attributes->get_attribute(NAMETAG_ATTR)) );
  }
  Op* entry_op = get_first_region_op_from_inops(r);
  if (entry_op->opcode() != C_MERGE)
    El_punt("get_nametag_attr(): Entry op for region is not C_MERGE.");
  return( (Nametag_attr*)(entry_op->attributes->get_attribute(NAMETAG_ATTR)) );
}

void set_nametag_attr(Op* op, Nametag_attr* attr)
{
  // if attribute already present, nuke it.
  if ((Nametag_attr*)(op->attributes->get_attribute(NAMETAG_ATTR)))
    remove_nametag_attr(op);
  op->attributes->insert_attribute(NAMETAG_ATTR, attr) ;
}

void remove_nametag_attr(Op* op)
{
  op->attributes->remove_attribute(NAMETAG_ATTR);
  // this will also call destructor method for Nametag_attr.
}

/////////////////////////////////////////////////////////////////////////////

Cluster_id_attr::Cluster_id_attr() {}
Cluster_id_attr::~Cluster_id_attr() {}
Base_attribute* Cluster_id_attr::clone_attribute() {
	Cluster_id_attr* tmp_ptr = new Cluster_id_attr();
	tmp_ptr -> cluster_id =cluster_id;
	return(tmp_ptr);
}
Cluster_id_attr* get_cluster_id_attr(Op *op)
{
  Cluster_id_attr* tmp_ptr ;

  tmp_ptr = (Cluster_id_attr*) (op->attributes->get_attribute(CLUSTER_ID_ATTR));
  if (tmp_ptr == 0) {
	return NULL;
  }
  return (tmp_ptr) ;
}

void set_cluster_id_attr(Op* op, Cluster_id_attr* attr)
{
  // if attribute already present, nuke it.
  if ((Cluster_id_attr*)(op->attributes->get_attribute(CLUSTER_ID_ATTR)))
    remove_cluster_id_attr(op);
  op->attributes->insert_attribute(CLUSTER_ID_ATTR, attr) ;
}

void remove_cluster_id_attr(Op* op)
{
  op->attributes->remove_attribute(CLUSTER_ID_ATTR);
  // this will also call destructor method for Cluster_id_attr.
}

////////////////////////////////////////////////////////////////////////////////

Usched_time_attr::Usched_time_attr() {}
Usched_time_attr::~Usched_time_attr() {}
Base_attribute* Usched_time_attr::clone_attribute() {
	Usched_time_attr* tmp_ptr = new Usched_time_attr();
	tmp_ptr -> usched_time =usched_time;
	return(tmp_ptr);
}
Usched_time_attr* get_usched_time_attr(Op *op)
{
  Usched_time_attr* tmp_ptr ;

  tmp_ptr = (Usched_time_attr*) (op->attributes->get_attribute(USCHED_TIME_ATTR));
  if (tmp_ptr == 0) {
	return NULL;
  }
  return (tmp_ptr) ;
}

void set_usched_time_attr(Op* op, Usched_time_attr* attr)
{
  // if attribute already present, nuke it.
  if ((Usched_time_attr*)(op->attributes->get_attribute(USCHED_TIME_ATTR)))
    remove_usched_time_attr(op);
  op->attributes->insert_attribute(USCHED_TIME_ATTR, attr) ;
}

void remove_usched_time_attr(Op* op)
{
  op->attributes->remove_attribute(USCHED_TIME_ATTR);
  // this will also call destructor method for Usched_time_attr.
}

////////////////////////////////////////////////////////////////////////////////

Custom_op_list_attr::Custom_op_list_attr() {}
Custom_op_list_attr::~Custom_op_list_attr() {}

Base_attribute* Custom_op_list_attr::clone_attribute()
{
  Custom_op_list_attr* tmp_ptr = new Custom_op_list_attr();
  assert(tmp_ptr && "you're out of memory");
  tmp_ptr->op_list = op_list;
  return(tmp_ptr);
}

Custom_op_list_attr* get_custom_op_list_attr(Op *op)
{
  Custom_op_list_attr* tmp_ptr ;
  tmp_ptr = (Custom_op_list_attr*) (op->attributes->get_attribute(CUSTOM_OP_LIST));
  return tmp_ptr;
}

void set_custom_op_list_attr(Op* op, Custom_op_list_attr* attr)
{
  // if attribute already present, nuke it.
  if ((Custom_op_list_attr*)(op->attributes->get_attribute(CUSTOM_OP_LIST)))
    remove_custom_op_list_attr(op);
  op->attributes->insert_attribute(CUSTOM_OP_LIST, attr) ;
}

void remove_custom_op_list_attr(Op* op)
{
  op->attributes->remove_attribute(CUSTOM_OP_LIST);
  // this will also call destructor method for Custom_op_list_attr.
}


////////////////////////////////////////////////////////////////////////////////


Branch_target_attr::Branch_target_attr() {}
Branch_target_attr::~Branch_target_attr() {}
Base_attribute* Branch_target_attr::clone_attribute() {
  Branch_target_attr* tmp_ptr = new Branch_target_attr();
  tmp_ptr->label = label;
  return(tmp_ptr);
}

Branch_target_attr* get_branch_target_attr(Op *op)
{
  Branch_target_attr* tmp_ptr ;

  tmp_ptr = (Branch_target_attr*) (op->attributes->get_attribute(BRANCH_TARGET_ATTR));
  if (tmp_ptr == 0) {
    return NULL;
  }
  return (tmp_ptr) ;
}

void set_branch_target_attr(Op* op, Branch_target_attr* attr)
{
  // if attribute already present, nuke it.
  if ((Branch_target_attr*)(op->attributes->get_attribute(BRANCH_TARGET_ATTR)))
    remove_branch_target_attr(op);
  op->attributes->insert_attribute(BRANCH_TARGET_ATTR, attr) ;
}

void remove_branch_target_attr(Op* op)
{
  op->attributes->remove_attribute(BRANCH_TARGET_ATTR);
  // this will also call destructor method for Branch_target_attr.
}


////////////////////////////////////////////////////////////////////////////////

AccSpecAttr::AccSpecAttr() {}
AccSpecAttr::~AccSpecAttr() {}

Base_attribute *AccSpecAttr::clone_attribute()
{
  AccSpecAttr *tmp_ptr = new AccSpecAttr();
  tmp_ptr->acc_spec_list = acc_spec_list;
  return(tmp_ptr);
}

AccSpecAttr *get_accspec_attr(Op *op)
{
  AccSpecAttr *tmp_ptr ;
  tmp_ptr = (AccSpecAttr *) (op->attributes->get_attribute(ACC_SPEC_ATTR));
  return tmp_ptr;
}

void set_accspec_attr(Op* op, AccSpecAttr* attr)
{
  // if attribute already present, nuke it.
  if ((AccSpecAttr *)(op->attributes->get_attribute(ACC_SPEC_ATTR)))
    remove_accspec_attr(op);
  op->attributes->insert_attribute(ACC_SPEC_ATTR, attr) ;
}

void remove_accspec_attr(Op* op)
{
  op->attributes->remove_attribute(ACC_SPEC_ATTR);
}
