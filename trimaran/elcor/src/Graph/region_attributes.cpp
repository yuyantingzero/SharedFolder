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
//      File:           region_attributes.cpp
//      Authors:        Sadun Anik, Sumedh Sathaye
//      Created:        December 1994
//      Description:    Region attributes 
//
/////////////////////////////////////////////////////////////////////////////

#include "region_attributes.h"

Lcode_attribute_map_attrib::Lcode_attribute_map_attrib() : map(0) {}

Lcode_attribute_map_attrib::~Lcode_attribute_map_attrib()
{
   delete map ;
}   

Base_attribute* Lcode_attribute_map_attrib::clone_attribute()
{
  Lcode_attribute_map_attrib* tmp_ptr = new  Lcode_attribute_map_attrib() ;
  tmp_ptr->map = new Lcode_attribute_map(*map) ;
  return(tmp_ptr) ;
}

Lcode_attribute_map* get_lcode_attributes(Region* r)
{
   Lcode_attribute_map_attrib* tmp_ptr ;

   tmp_ptr = (Lcode_attribute_map_attrib*)
      (r->attributes->get_attribute(LCODE_ATTRIBUTES));
   if (tmp_ptr == 0) {
      tmp_ptr = new Lcode_attribute_map_attrib;
      tmp_ptr->map = new Lcode_attribute_map;      
      r->attributes->insert_attribute(LCODE_ATTRIBUTES,tmp_ptr) ;
   }
   return (tmp_ptr->map) ;
   
}

void set_lcode_attributes(Region* r, Lcode_attribute_map* lat)
{
   Lcode_attribute_map_attrib* tmp_ptr = (Lcode_attribute_map_attrib*)
      r->attributes->get_attribute(LCODE_ATTRIBUTES);
   if (tmp_ptr == 0) {
      tmp_ptr = new Lcode_attribute_map_attrib;
      tmp_ptr->map = 0 ;
   }
   delete tmp_ptr->map ;
   tmp_ptr->map = lat ;
   r->attributes->insert_attribute(LCODE_ATTRIBUTES, tmp_ptr) ;
}

void remove_lcode_attributes(Region* r)
{
   r->attributes->remove_attribute(LCODE_ATTRIBUTES) ;
}


/////////////////////////////////////////////////////////////////////////////

MemDepProf_attribute_map_attrib::MemDepProf_attribute_map_attrib() : map(0) {}

MemDepProf_attribute_map_attrib::~MemDepProf_attribute_map_attrib()
{
   delete map ;
}   

Base_attribute* MemDepProf_attribute_map_attrib::clone_attribute()
{
   MemDepProf_attribute_map_attrib* tmp_ptr = 
		new MemDepProf_attribute_map_attrib() ;
   tmp_ptr->map = new MemDepProf_attribute_map(*map) ;
   return(tmp_ptr) ;
}

MemDepProf_attribute_map* get_mdp_attributes(Region* r)
{
   MemDepProf_attribute_map_attrib* tmp_ptr ;

   tmp_ptr = (MemDepProf_attribute_map_attrib*)
      (r->attributes->get_attribute(MEM_DEP_PROF_ATTRIBUTES));
   if (tmp_ptr == 0) {
      tmp_ptr = new MemDepProf_attribute_map_attrib;
      tmp_ptr->map = new MemDepProf_attribute_map;      
      r->attributes->insert_attribute(MEM_DEP_PROF_ATTRIBUTES,tmp_ptr) ;
   }
   return (tmp_ptr->map) ;
   
}

void set_mdp_attributes(Region* r, MemDepProf_attribute_map* lat)
{
   MemDepProf_attribute_map_attrib* tmp_ptr = (MemDepProf_attribute_map_attrib*)
      r->attributes->get_attribute(MEM_DEP_PROF_ATTRIBUTES);
   if (tmp_ptr == 0) {
      tmp_ptr = new MemDepProf_attribute_map_attrib;
      tmp_ptr->map = 0 ;
   }
   delete tmp_ptr->map ;
   tmp_ptr->map = lat ;
   r->attributes->insert_attribute(MEM_DEP_PROF_ATTRIBUTES, tmp_ptr) ;
}

void remove_mdp_attributes(Region* r)
{
   r->attributes->remove_attribute(MEM_DEP_PROF_ATTRIBUTES) ;
}

/////////////////////////////////////////////////////////////////////////////

Control_cpr_info::Control_cpr_info()
{
    num_chains = 0;
}

Control_cpr_info::Control_cpr_info(int n_chains)
{
    num_chains = n_chains;
    on_trace_preds.resize(n_chains);
    off_trace_preds.resize(n_chains);
    derived_on_trace_preds.resize(n_chains);
    derived_off_trace_preds.resize(n_chains);
}

Control_cpr_info::~Control_cpr_info()
{
}

Base_attribute *Control_cpr_info::clone_attribute()
{
    Control_cpr_info *tmp_ptr = new Control_cpr_info;

    tmp_ptr->on_trace_preds = on_trace_preds;
    tmp_ptr->off_trace_preds = off_trace_preds;
    tmp_ptr->derived_on_trace_preds = derived_on_trace_preds;
    tmp_ptr->derived_off_trace_preds = derived_off_trace_preds;

    return (tmp_ptr);
}

void Control_cpr_info::set_on_trace_pred(int index, Operand &val)
{
    on_trace_preds[index] = val;
}

void Control_cpr_info::set_off_trace_pred(int index, Operand &val)
{
    off_trace_preds[index] = val;
}

void Control_cpr_info::set_derived_on_trace_pred(int index,
					Hash_set<Operand> &val)
{
    derived_on_trace_preds[index] = val;
}

void Control_cpr_info::set_derived_off_trace_pred(int index,
                                        Hash_set<Operand> &val)
{
    derived_off_trace_preds[index] = val;
}

void Control_cpr_info::add_derived_on_trace_pred(int index, Operand &val)
{
    derived_on_trace_preds[index] += val;
}

void Control_cpr_info::add_derived_off_trace_pred(int index, Operand &val)
{
    derived_off_trace_preds[index] += val;
}

Control_cpr_info* get_control_cpr_info(Region *r)
{
  Control_cpr_info* tmp_ptr ;

  tmp_ptr = (Control_cpr_info*) (r->attributes->get_attribute(CONTROL_CPR_INFO));
  return (tmp_ptr) ;
}

void set_control_cpr_info(Region* r, Control_cpr_info* attr)
{
  r->attributes->insert_attribute(CONTROL_CPR_INFO, attr) ;
}

void remove_control_cpr_info(Region* r)
{
   r->attributes->remove_attribute(CONTROL_CPR_INFO) ;
}

/////////////////////////////////////////////////////////////////////////////

Reg_to_liveness_map_attrib::Reg_to_liveness_map_attrib() : map(0) {}

Reg_to_liveness_map_attrib::~Reg_to_liveness_map_attrib()
{
   delete map ;
}   

Base_attribute* Reg_to_liveness_map_attrib::clone_attribute()
{
   Reg_to_liveness_map_attrib* tmp_ptr = new  Reg_to_liveness_map_attrib() ;
   tmp_ptr->map = new Reg_to_liveness_map(*map) ;
   return(tmp_ptr) ;
}

Reg_to_liveness_map* get_reg_to_liveness_map(Region* r)
{
   Reg_to_liveness_map_attrib* tmp_ptr ;

   tmp_ptr = (Reg_to_liveness_map_attrib*)
      (r->attributes->get_attribute(REG_TO_LIVENESS_MAP));
   if (tmp_ptr == 0) {
      tmp_ptr = new Reg_to_liveness_map_attrib;
      tmp_ptr->map = new Reg_to_liveness_map;
      r->attributes->insert_attribute(REG_TO_LIVENESS_MAP,tmp_ptr) ;
   }
   return (tmp_ptr->map) ;
   
}

void set_reg_to_liveness_map(Region* r, Reg_to_liveness_map* lat)
{
   Reg_to_liveness_map_attrib* tmp_ptr = (Reg_to_liveness_map_attrib*)
      r->attributes->get_attribute(REG_TO_LIVENESS_MAP);
   if (tmp_ptr == 0) {
      tmp_ptr = new Reg_to_liveness_map_attrib;
      tmp_ptr->map = 0 ;
   }
   delete tmp_ptr->map ;
   tmp_ptr->map = lat ;
   r->attributes->insert_attribute(REG_TO_LIVENESS_MAP, tmp_ptr) ;
}

void remove_reg_to_liveness_map(Region* r)
{
   r->attributes->remove_attribute(REG_TO_LIVENESS_MAP) ;
}

/////////////////////////////////////////////////////////////////////////////

Loop_count_attrib::Loop_count_attrib() : loopcount(-1) {}

Loop_count_attrib::~Loop_count_attrib() {}   

Base_attribute* Loop_count_attrib::clone_attribute()
{
   Loop_count_attrib* tmp_ptr = new  Loop_count_attrib() ;
   tmp_ptr->loopcount = loopcount;
   return(tmp_ptr) ;
}

int get_loop_count(Region* r)
{
   Loop_count_attrib* tmp_ptr ;

   tmp_ptr = (Loop_count_attrib*)
      (r->attributes->get_attribute(LOOP_COUNT));
   if (tmp_ptr == 0) return (-1);
   else return (tmp_ptr->loopcount) ;
}

void set_loop_count(Region* r, int lc)
{
   Loop_count_attrib* tmp_ptr = (Loop_count_attrib*)
      r->attributes->get_attribute(LOOP_COUNT);
   if (tmp_ptr == 0) {
      tmp_ptr = new Loop_count_attrib;
   }
   tmp_ptr->loopcount = lc ;
   r->attributes->insert_attribute(LOOP_COUNT, tmp_ptr) ;
}

void remove_loop_count(Region* r)
{
   r->attributes->remove_attribute(LOOP_COUNT) ;
}

/////////////////////////////////////////////////////////////////////////////

Exsym_table_attrib::Exsym_table_attrib() : symtab(0) {}

Exsym_table_attrib::~Exsym_table_attrib()
{
   delete symtab ;
}   

Base_attribute* Exsym_table_attrib::clone_attribute()
{
   Exsym_table_attrib* tmp_ptr = new  Exsym_table_attrib() ;
   tmp_ptr->symtab = new Exsym_table(*symtab) ;
   return(tmp_ptr) ;
}

Exsym_table* get_exsym_table(Region* r)
{
   Exsym_table_attrib* tmp_ptr ;

   tmp_ptr = (Exsym_table_attrib*)
      (r->attributes->get_attribute(EXSYM_TABLE_ATTR));
   if (tmp_ptr == 0) {
      tmp_ptr = new Exsym_table_attrib;
      tmp_ptr->symtab = new Exsym_table(hash_int, 37);
      r->attributes->insert_attribute(EXSYM_TABLE_ATTR,tmp_ptr) ;
   }
   return (tmp_ptr->symtab) ;
   
}

void set_exsym_table(Region* r, Exsym_table* _symtab)
{
   Exsym_table_attrib* tmp_ptr = (Exsym_table_attrib*)
      r->attributes->get_attribute(EXSYM_TABLE_ATTR);
   if (tmp_ptr == 0) {
      tmp_ptr = new Exsym_table_attrib;
      tmp_ptr->symtab = 0 ;
   }
   delete tmp_ptr->symtab ;
   tmp_ptr->symtab = _symtab ;
   r->attributes->insert_attribute(EXSYM_TABLE_ATTR, tmp_ptr) ;
}

void remove_exsym_table(Region* r)
{
   r->attributes->remove_attribute(EXSYM_TABLE_ATTR) ;
}

// Exsym info 
Exsym_info::Exsym_info() {}
Exsym_info::Exsym_info(eString _name, DType _dt, MEMORY_TYPE _type) 
  : var_name(_name), dt(_dt), type(_type) {}

Exsym_info::~Exsym_info() {}

// virtual exsym info constructor
Exsym_info* Exsym_info::exsym_info_allocator(Exsym_type version) {
  Exsym_info* exsym=NULL;
  if (version==BASE_EXSYM) exsym = new Exsym_info();
  else if (version==SUIF_EXSYM) exsym = new Suif_exsym_info();
  else El_punt("invalid external symbol version %d.", (int)version);
  return exsym;
}  

// Suif Exsym info
Suif_exsym_info::Suif_exsym_info() : Exsym_info() {}
Suif_exsym_info::Suif_exsym_info(eString _name, DType _dt, MEMORY_TYPE _type)
  : Exsym_info(_name, _dt, _type), 
    c_livein_base(0), c_liveout_base(0), has_asn(false) {}

Suif_exsym_info::~Suif_exsym_info() {}


/////////////////////////////////////////////////////////////////////////////

Msched_attr::Msched_attr() {}

Msched_attr::~Msched_attr() {}

Base_attribute* Msched_attr::clone_attribute()
{
  Msched_attr* tmp_ptr = new Msched_attr() ;
  tmp_ptr->ii = ii;
  tmp_ptr->esc = esc;
  tmp_ptr->liveness_gen = liveness_gen;
  tmp_ptr->liveness_kill = liveness_kill ;
  tmp_ptr->down_exposed_use_gen = down_exposed_use_gen ;
  tmp_ptr->down_exposed_use_kill = down_exposed_use_kill ;
  tmp_ptr->up_exposed_def_gen = up_exposed_def_gen ;
  tmp_ptr->up_exposed_def_kill = up_exposed_def_kill ;
  tmp_ptr->down_exposed_def_gen = down_exposed_def_gen ;
  tmp_ptr->down_exposed_def_kill = down_exposed_def_kill ;
  return (tmp_ptr) ;
}

Msched_attr* get_msched_attr(Compound_region* r) {
  Msched_attr* tmp_ptr ;
  tmp_ptr = (Msched_attr*) r->attributes->get_attribute(MSCHED_ATTRIBUTES) ;
//   if (tmp_ptr == NULL) {
//     tmp_ptr = new Msched_attr ;
//     r->attributes->insert_attribute(MSCHED_ATTRIBUTES,tmp_ptr) ;
//   }
  return (tmp_ptr) ;
}

void set_msched_attr(Compound_region* r, Msched_attr* msched_attr) {
  r->attributes->insert_attribute(MSCHED_ATTRIBUTES, msched_attr);
}

void remove_msched_attr(Compound_region* r) {
  r->attributes->remove_attribute(MSCHED_ATTRIBUTES) ;
}

///////////////////////////////////////////////////////////////////////////
