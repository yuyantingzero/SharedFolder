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
//      File:           op_attributes.h
//      Authors:        Sadun Anik
//      Created:        December 1994
//      Description:    Op attributes 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OP_ATTRIBUTES_H
#define _OP_ATTRIBUTES_H

#include "list.h"
#include "operand.h"
#include "attribute_types.h"
#include "op.h"
#include "region_attributes.h"
#include "spatial_ref.h"
#include "acc_spec.h"

class Merge_op_BB_id : public Base_attribute {
public:
  Merge_op_BB_id() ;
  ~Merge_op_BB_id() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return MERGE_OP_BB_ID; }
  bool is_op_attribute() { return true; }
  bool is_inline() { return true; } 
      
  int id ;
} ;

Merge_op_BB_id* get_existing_merge_op_bb_id(Op*) ;
Merge_op_BB_id* get_merge_op_bb_id(Op*) ;
void            set_merge_op_bb_id(Op*, Merge_op_BB_id*) ;
void            remove_merge_op_bb_id(Op*) ;

/////////////////////////////////////////////////////////////////////////////

class Branch_info_attr : public Base_attribute {
public:
  Branch_info_attr() ;
  ~Branch_info_attr() ;
  
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return BRANCH_INFO_ATTR; }
  bool is_op_attribute() { return true; }
  
  // Op = NULL, id = -1 : need to create a new op with new id
  // Op = NULL, id = K : create a new op with id = K
  // Op !=NULL, id = dont care: no need to create new op, already exists
  int pbr_id;
  Op *pbr_op;
  int cmpp_id;
  Op *cmpp_op;
  int ld_id;
  Op *ld_op;
  int addr_calc_id;
  Op *addr_calc_op;
      
  //target registers of cmpp and pbr
  Operand *cmpp_tgt;
  Operand *pbr_tgt;
  Operand *ld_tgt;
  Operand *addr_calc_tgt;
  
  //Clones of pbr,cmpp and br attributes
  Lcode_attribute_map *pbr_attrs;
  Lcode_attribute_map *cmpp_attrs;
  Lcode_attribute_map *ld_attrs;
  Lcode_attribute_map *addr_calc_attrs;
  Lcode_attribute_map *br_attrs;  
};

Branch_info_attr* get_branch_info_attr(Op*);
void	          set_branch_info_attr(Op*, Branch_info_attr*);
void              remove_branch_info_attr(Op*);

/////////////////////////////////////////////////////////////////////////////

class Reference_dtype_attr : public Base_attribute {
public:
  Reference_dtype_attr() ;
  Reference_dtype_attr(Op *op) ;
  ~Reference_dtype_attr() ;
  
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return REFERENCE_DTYPE_ATTR; }
  bool is_op_attribute() { return true; }

  DType &src(Port_num pn);
  void set_src(Port_num pn, DType dt);
  int num_src();

  DType &dest(Port_num pn);
  void set_dest(Port_num pn, DType dt);
  int num_dest();

  void swap_dests(Port_num a, Port_num b);
  void swap_srcs(Port_num a, Port_num b);

  void resize(Op *op);
  void resize_src(int new_src);
  void resize_dest(int new_dest);

  void set_srcs_conservatively(Op *op);
  void set_dests_conservatively(Op *op);
  void set_all_conservatively(Op *op);

private:
  Vector<DType> src_refdtype;
  Vector<DType> dest_refdtype;
};

Reference_dtype_attr *get_reference_dtype_attr(Op*);
void	          set_reference_dtype_attr(Op*, Reference_dtype_attr*);
void              remove_reference_dtype_attr(Op*);

/////////////////////////////////////////////////////////////////////////////

// New attribute to hold the spatial reference of each operation operand.
// dcronqui, 5/8/01
class Spatial_ref_attr : public Base_attribute {
public:
  Spatial_ref_attr() ;
  ~Spatial_ref_attr() ;
  
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return SPATIAL_REF_ATTR; }
  bool is_op_attribute() { return true; }

  Spatial_ref src(Port_num pn);
  void set_src(Port_num pn, Spatial_ref sr);
  int num_src();

  Spatial_ref dest(Port_num pn);
  void set_dest(Port_num pn, Spatial_ref sr);
  int num_dest();

  void swap_dests(Port_num a, Port_num b);
  void swap_srcs(Port_num a, Port_num b);

  bool is_null(void) { return src_spatial_refs.size() == 0 && dest_spatial_refs.size() == 0; }

  void iterate_srcs(void);
  void iterate_dests(void);

  bool get_next_ref(Pair<Port_num, Spatial_ref> &pn_sr);

private:
  Hash_map_iterator<Port_num, Spatial_ref> refsi;
  Hash_map<Port_num, Spatial_ref> src_spatial_refs;
  Hash_map<Port_num, Spatial_ref> dest_spatial_refs;
};

Spatial_ref_attr *get_spatial_ref_attr(Op*);
void	          set_spatial_ref_attr(Op*, Spatial_ref_attr*);
void              remove_spatial_ref_attr(Op*);

/////////////////////////////////////////////////////////////////////////////
// New attribute to hold physical register file bindings for codegen
// Rajiv

class Physfile_binding_attr: public Base_attribute {
public:
  Physfile_binding_attr();
  ~Physfile_binding_attr();

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return PHYSFILE_BIND_ATTR; }
  bool is_op_attribute() { return true; }

  Hash_set<eString>& src(Port_num pn);
  void set_src(Port_num pn, Hash_set<eString>& pr);
  int num_src();

  Hash_set<eString>& dest(Port_num pn);
  void set_dest(Port_num pn, Hash_set<eString>& pr);
  int num_dest();

  void iterate_srcs(void);
  void iterate_dests(void);

  bool get_next_ref(Pair<Port_num, Hash_set<eString> >& pn_pr);

  void clear_attr_entries();

private:
  Hash_map_iterator<Port_num, Hash_set<eString> > refsi;
  Hash_map<Port_num, Hash_set<eString> > src_prf_bindings;
  Hash_map<Port_num, Hash_set<eString> > dest_prf_bindings;
};

Physfile_binding_attr *get_prf_binding_attr(Op*);
void set_prf_binding_attr(Op*, Physfile_binding_attr*);
void remove_prf_binding_attr(Op*);

/////////////////////////////////////////////////////////////////////////////
/*
 *
 * Shivaram: added the class Op_chaining_attr on 20-02-2001 to support operator chaining.
 */
class Op_chaining_attr : public Base_attribute {
public:
  Op_chaining_attr();
  Op_chaining_attr(Op *op) ;
  Op_chaining_attr(const List< List<Op*> > &op_list) ;
  ~Op_chaining_attr() ;
  
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return OP_CHAINING_ATTR; }
  bool is_op_attribute() { return true; }

  List< List<Op*> > &get_op_chain_list();

  void set_op_chain_list(const List< List<Op*> > &op_list);

  void invalidate_chains(Op *del_op, bool d_flag = false);
  void invalidate_chains(Hash_set<Op*> &del_set, bool d_flag = false);

private:
  List< List <Op*> > op_chain_list;  /* list of operations in the chain */
};

Op_chaining_attr *get_op_chaining_attr(Op*);
void	          set_op_chaining_attr(Op*, Op_chaining_attr*);
void              remove_op_chaining_attr(Op*);

/////////////////////////////////////////////////////////////////////////////

class Call_return_def_list : public Base_attribute {
public:
  Call_return_def_list() ;

  ~Call_return_def_list() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return CALL_RETURN_DEF_LIST; }
  bool is_op_attribute() { return true; }

  List<Operand>* list ;
} ;

List<Operand>* get_implicit_def_list(Op*) ;
void           set_implicit_def_list(Op*, List<Operand>*) ;
void           remove_implicit_def_list(Op*) ;

/////////////////////////////////////////////////////////////////////////////

class Call_return_use_list : public Base_attribute {
public:
  Call_return_use_list() ;

  ~Call_return_use_list() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return CALL_RETURN_USE_LIST; }
  bool is_op_attribute() { return true; }

  List<Operand>* list ;
} ;

List<Operand>* get_implicit_use_list(Op*) ;
void           set_implicit_use_list(Op*, List<Operand>*) ;
void           remove_implicit_use_list(Op*) ;

/////////////////////////////////////////////////////////////////////////////

class Jump_table_name : public Base_attribute {
public:
  Jump_table_name(eString name) ;
  ~Jump_table_name() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return JUMP_TABLE_NAME; }
  bool is_op_attribute() { return true; }
  bool is_inline() { return true; }

  eString tbl_name;
} ;

Jump_table_name* get_jump_table_name(Op*) ;
void             set_jump_table_name(Op*, Jump_table_name*) ;
void             remove_jump_table_name(Op*) ;

/////////////////////////////////////////////////////////////////////////////

enum Nametag_type {SYSTOLIC_INIT, SYSTOLIC_BODY, SYSTOLIC_FINAL};

class Nametag_attr : public Base_attribute {
public:
  Nametag_attr() ;
  ~Nametag_attr() ;
  
  Base_attribute* clone_attribute();
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return NAMETAG_ATTR; }
  bool is_inline() { return true; }
  bool is_op_attribute() { return true; }

  eString name;
  Nametag_type type;
};

/* Nametag_attr* get_nametag_attr(Op* op); */
Nametag_attr* get_nametag_attr(Region* r);
void set_nametag_attr(Op* op, Nametag_attr* attr);
void remove_nametag_attr(Op* op);

/////////////////////////////////////////////////////////////////////////////

class Cluster_id_attr : public Base_attribute {
public:
  Cluster_id_attr () ;
  ~Cluster_id_attr() ;
  
  Base_attribute* clone_attribute();
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return CLUSTER_ID_ATTR; }
  bool is_inline() { return true; }
  bool is_op_attribute() { return true; }

  int cluster_id;
};

Cluster_id_attr* get_cluster_id_attr(Op* r);
void set_cluster_id_attr(Op* op, Cluster_id_attr* attr);
void remove_cluster_id_attr(Op* op);

/////////////////////////////////////////////////////////////////////////////

class Usched_time_attr : public Base_attribute {
public:
  Usched_time_attr () ;
  ~Usched_time_attr() ;
  
  Base_attribute* clone_attribute();
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return USCHED_TIME_ATTR; }
  bool is_inline() { return true; }
  bool is_op_attribute() { return true; }

  int usched_time;
};

Usched_time_attr* get_usched_time_attr(Op* r);
void set_usched_time_attr(Op* op, Usched_time_attr* attr);
void remove_usched_time_attr(Op* op);

/////////////////////////////////////////////////////////////////////////////

class Custom_op_list_attr : public Base_attribute {
public:
  Custom_op_list_attr ();
  ~Custom_op_list_attr();
  
  Base_attribute* clone_attribute();
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return CUSTOM_OP_LIST; }
  bool is_op_attribute() { return true; }

  List<Op*> op_list;
};

Custom_op_list_attr* get_custom_op_list_attr(Op* r);
void set_custom_op_list_attr(Op* op, Custom_op_list_attr* attr);
void remove_custom_op_list_attr(Op* op);


/////////////////////////////////////////////////////////////////////////////

class Branch_target_attr : public Base_attribute {
public:
  Branch_target_attr ();
  ~Branch_target_attr();
  
  Base_attribute* clone_attribute();
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return BRANCH_TARGET_ATTR; }
  bool is_op_attribute() { return true; }
  bool is_inline() { return true; }

  string label;
};

Branch_target_attr* get_branch_target_attr(Op* r);
void set_branch_target_attr(Op* op, Branch_target_attr* attr);
void remove_branch_target_attr(Op* op);

///////////////////////////////////////////////////////////////////////////////

class AccSpecAttr : public Base_attribute {
  public:
  AccSpecAttr();
  ~AccSpecAttr();

  Base_attribute *clone_attribute();
  bool has_rebel_io() { return true; }
  bool is_inline() { return true; }
  Attribute_type attribute_type() { return ACC_SPEC_ATTR; }
  bool is_op_attribute() { return true; }

  List<AccSpec *> acc_spec_list;  
};

AccSpecAttr* get_accspec_attr(Op* op);
void set_accspec_attr(Op* op, AccSpecAttr *attr);
void remove_accspec_attr(Op* op);

#endif

