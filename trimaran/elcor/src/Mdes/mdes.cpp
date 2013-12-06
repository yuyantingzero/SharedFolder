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
//      File:           mdes.cpp
//      Authors:        Mike Schlansker, Vinod Kathail, Shail Aditya
//      Created:        December 1994
//      Description:    The MDES database implementation
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include "mdes.h"
#include "intf.h"
#include "defs.h"
#include "dbg.h"
#include "el_error.h"
#include "hash_functions.h"
#include "op.h"

using namespace std;

int IOCODE_SIZE = 0;
static int mod_inc(int x,int modu);
static int mod_dec(int x,int modu);
static int ceiling(int x, int y);

/////////////////////////////////////////////////////////////////////////////
// MDES

MDES::~MDES() {
  if (input_file) free(input_file);  // malloc'ed by strdup, so use free
}

MDES::MDES(int hashsize)
  : input_file(NULL), hashtable(new MdesHashtable(this, hashsize)), num_vreg_types(0), 
    macro_to_index_map(hash_estring, 1000), 
    res_str_to_descr_map(hash_estring, 1000),
    name_to_const_desc_map(hash_estring, 32),
    macro_to_pf_map(hash_macro, 32),
    alias_macros(hash_estring, 8),
    pseudo_macros(hash_estring, 8),
    read_only_macros(hash_estring, 8),
    multialias_macros(hash_estring, 8),
    coherent_macros(hash_estring, 8)
{}

#define MDES_copy_tab(TYPE,mdes,tab) \
if ((mdes).tab) { \
  tab = new Vector<TYPE*>(*((mdes).tab)); }   

MDES::MDES(MDES& mdes) 
  : input_file(strdup(mdes.input_file)), hashtable(new MdesHashtable(*(mdes.hashtable))), 
    lat_descr_tab(mdes.lat_descr_tab), reg_descr_tab(mdes.reg_descr_tab),
    macro_descr_tab(mdes.macro_descr_tab),
    res_descr_tab(mdes.res_descr_tab), res_use_descr_tab(mdes.res_use_descr_tab),
    io_descr_tab(mdes.io_descr_tab), ft_descr_tab(mdes.ft_descr_tab), 
    num_vreg_types(mdes.num_vreg_types),  custom_op_tab(mdes.custom_op_tab),
    macro_to_index_map(hash_estring, 1000),
    res_str_to_descr_map(hash_estring, 1000),
    name_to_const_desc_map(hash_estring, 32),
    macro_to_pf_map(hash_macro, 32),
    alias_macros(hash_estring, 8),
    pseudo_macros(hash_estring, 8),
    read_only_macros(hash_estring, 8),
    multialias_macros(hash_estring, 8),
    coherent_macros(hash_estring, 8)
{}

char* MDES::get_MDES_input_file() const { return input_file; }

MdesHashtable* MDES::get_MDES_hashtable() const { return hashtable; }

int MDES::get_MDES_num_vreg_types() const { return num_vreg_types; }

int MDES::get_MDES_num_resources() const { return res_descr_tab.size(); }

int MDES::get_MDES_num_res_use() const { return res_use_descr_tab.size(); }

int MDES::get_MDES_num_reg_types() const { return reg_descr_tab.size(); }

int MDES::get_MDES_num_lat_classes() const { return lat_descr_tab.size(); }

int MDES::get_MDES_num_io_formats() const { return io_descr_tab.size(); }

int MDES::get_MDES_num_field_types() const { return ft_descr_tab.size(); }

int MDES::get_MDES_num_const_sets() const { return name_to_const_desc_map.size(); }

Lat_descr* MDES::get_MDES_lat_descr(int index) const { return lat_descr_tab[index]; }

Reg_descr* MDES::get_MDES_reg_descr(int index) const { return reg_descr_tab[index]; }

Reg_descr* MDES::get_MDES_reg_descr(const char* prf) const {
  for(unsigned int i = 0; i < reg_descr_tab.size(); i++) {
    if(strcmp(prf, (reg_descr_tab[i])->get_aname()) == 0)
      return reg_descr_tab[i]; 
  } 
  El_punt("MDES: RF with prf %s not found", prf);
  return NULL;
}

Const_set_descr* MDES::get_MDES_const_descr(const char* cset) const
{
  eString name(cset);
  return name_to_const_desc_map[name];
}

Res_descr* MDES::get_MDES_res_descr(int index) const { return res_descr_tab[index]; }

Res_use_descr* MDES::get_MDES_res_use_descr(int index) const { return res_use_descr_tab[index]; }

Io_descr* MDES::get_MDES_io_descr(int index) const { return io_descr_tab[index]; }

Ft_descr* MDES::get_MDES_ft_descr(int index) const { return ft_descr_tab[index]; }

bool MDES::is_macro_present(eString mn) const
{
  for (unsigned int i = 0; i < macro_descr_tab.size(); i++) {
    if (macro_descr_tab[i]->get_name() == mn)
      return true;
  }

  return false;
}

bool MDES::is_macro_present(Macro_name mn) const
{
  return is_macro_present(el_macro_to_string_map.value(mn));
}
  
Hash_set<eString>& MDES::get_MDES_pf_for_macro(Macro_name mn) const
{
  if (!macro_to_pf_map.is_bound(mn)) {
    El_punt("There's a macro (%s) in your code that isn't in the MDES!",  
            el_macro_to_string_map.value(mn).strdup());
  }
  return macro_to_pf_map.value(mn);
}

Hash_set<eString>& MDES::get_MDES_pseudo_macros()  { return pseudo_macros; }

Hash_set<eString>& MDES::get_MDES_read_only_macros() { return read_only_macros; }

Hash_set<eString>& MDES::get_MDES_alias_macros() { return alias_macros; }

Hash_set<eString>& MDES::get_MDES_multialias_macros() { return multialias_macros; }

Hash_set<eString>& MDES::get_MDES_coherent_macros() { return coherent_macros; }

bool MDES::is_pv_macro(Macro_name mn) {
  eString macro_string = el_macro_to_string_map.value(mn);
  Macro_descr* mdescr = macro_descr_tab[macro_to_index_map.value(macro_string)];
  return mdescr->is_pv();
}

bool MDES::is_pseudo_macro(eString name) {
  return pseudo_macros.is_member(name);
}

bool MDES::is_read_only_macro(eString name) {
  return read_only_macros.is_member(name);
}

bool MDES::is_alias_macro(eString name) {
  return alias_macros.is_member(name);
}

bool MDES::is_multialias_macro(eString name) {
  return multialias_macros.is_member(name);
}

bool MDES::is_coherent_macro(eString name) {
  return coherent_macros.is_member(name);
}

// returns macro aliased by mac
eString MDES::get_macro_aliased(eString mac)
{
  return (macro_descr_tab[macro_to_index_map.value(mac)]->get_macro_aliased());
}

int MDES::get_MDES_reg_number_for_pf(eString pf) {
  eString num = pf.substr(pf.charrpos('_')+1, pf.length()-1);
  return str_to_int(num);
}

Custom_op_descr* MDES::get_MDES_custom_op_descr(int index) const
{
  return custom_op_tab[index];
}

unsigned MDES::get_num_custom_ops() const
{
  return custom_op_tab.size();
}

void MDES::set_MDES_input_file(char* file) { input_file = file; }

void MDES::set_MDES_num_vreg_types(int numvreg) { num_vreg_types = numvreg; }

void MDES::resize_MDES_lat_table(int size) { 
  static Lat_descr* null_lat = NULL;
  lat_descr_tab.resize(size, null_lat);
}

void MDES::resize_MDES_reg_table(int size) { 
  static Reg_descr* null_reg = NULL;
  reg_descr_tab.resize(size, null_reg);
}

void MDES::resize_MDES_res_table(int size) { 
  static Res_descr* null_res = NULL;
  res_descr_tab.resize(size, null_res);
}

void MDES::resize_MDES_res_use_table(int size) { 
  static Res_use_descr* null_res_use = NULL;
  res_use_descr_tab.resize(size, null_res_use);
}

void MDES::resize_MDES_io_table(int size) { 
  static Io_descr* null_io = NULL;
  io_descr_tab.resize(size, null_io);
}

void MDES::resize_MDES_ft_table(int size) { 
  static Ft_descr* null_ft = NULL;
  ft_descr_tab.resize(size, null_ft);
}

void MDES::resize_MDES_custom_op_table(int size)
{
  custom_op_tab.resize(size, NULL);
}

void MDES::set_MDES_lat_descr(int index, Lat_descr* descr){
  static Lat_descr* null_lat = NULL;
  if ((unsigned int)index>=lat_descr_tab.size()) lat_descr_tab.resize(index+1, null_lat);
  lat_descr_tab[index] = descr ;
}

void MDES::set_MDES_res_descr(int index, Res_descr* descr){
  static Res_descr* null_res = NULL;
  if ((unsigned int) index>=res_descr_tab.size()) res_descr_tab.resize(index+1, null_res);
  res_descr_tab[index] = descr ;
}

void MDES::set_MDES_reg_descr(int index, Reg_descr* descr){
  static Reg_descr* null_reg = NULL;
  if ((unsigned int) index>=reg_descr_tab.size()) reg_descr_tab.resize(index+1, null_reg);
  reg_descr_tab[index] = descr ;
}

void MDES::set_MDES_res_use_descr(int index, Res_use_descr* descr){
  static Res_use_descr* null_res_use = NULL;
  if ((unsigned int) index>=res_use_descr_tab.size()) res_use_descr_tab.resize(index+1, null_res_use);
  res_use_descr_tab[index] = descr ;
}

void MDES::set_MDES_io_descr(int index, Io_descr* descr){
  static Io_descr* null_io = NULL;
  if ((unsigned int) index>=io_descr_tab.size()) io_descr_tab.resize(index+1, null_io);
  io_descr_tab[index] = descr ;
}

void MDES::set_MDES_ft_descr(int index, Ft_descr* descr){
  static Ft_descr* null_ft = NULL;
  if ((unsigned int) index>=ft_descr_tab.size()) ft_descr_tab.resize(index+1, null_ft);
  ft_descr_tab[index] = descr ;
}

void MDES::set_MDES_custom_op_descr(int index, Custom_op_descr* desc)
{
  if((unsigned int) index >= custom_op_tab.size())
    custom_op_tab.resize(index + 1, NULL);
  custom_op_tab[index] = desc;
}

void MDES::add_MDES_const_set_descr(eString name, Const_set_descr* desc)
{
  if (name_to_const_desc_map.is_bound(name))
    El_warn("Overwriting existing Const_set");
  name_to_const_desc_map.bind(name, desc);
}

char* MDES::find_MDES_rf_name(const Bitvector& iocode, IO_kind kind, Bitvector *iocodeptr) {
  if (kind==IO_VIRTUAL) {
    Bitvector matchcode(IOCODE_SIZE);
    Ft_descr* ftdesc=NULL;
    for (int i=0; i<get_MDES_num_field_types(); i++) {
      Ft_descr* desc = ft_descr_tab[i];
      const Bitvector& desc_iocode = desc->get_iocode();
      // find largest match (integral value)
      // note, using ones_count() instead of integral value. -KF 9-2004
      if ((iocode*desc_iocode)==desc_iocode &&
          matchcode.ones_count() < desc_iocode.ones_count()) {
        matchcode = desc_iocode;
        ftdesc = desc;
      }
    }
    if (iocodeptr) *iocodeptr = iocode - matchcode;
    if (ftdesc) return ftdesc->get_name();
  } else {
    for (int i=0; i<get_MDES_num_reg_types(); i++) {
      Reg_descr* desc = reg_descr_tab[i];
      const Bitvector& desc_iocode = desc->get_iocode();
      // find exact match
      if ((iocode*desc_iocode)==desc_iocode) {
        if (iocodeptr) *iocodeptr = iocode - desc_iocode;
        return desc->get_aname();
      }
    }
  }
  cdbg << iocode;
  El_punt("MDES::find_MDES_rf_name: RF with above iocode not found.");
  return NULL;
}

int MDES::find_MDES_rf_index(char* name, IO_kind kind, bool flag_not_found) {
  if (kind==IO_VIRTUAL) {
    for (int i=0; i<get_MDES_num_field_types(); i++) {
      Ft_descr* desc = ft_descr_tab[i];
      if (!strcmp(name,desc->get_name())) return i;
    }
  } else {
    for (int i=0; i<get_MDES_num_reg_types(); i++) {
      Reg_descr* desc = reg_descr_tab[i];
      if (!strcmp(name,desc->get_aname())) return i;
    }
  }
  if (flag_not_found) El_punt("MDES::find_MDES_rf_index: RF with name '%s' not found.", name);
  return (-1);
}

int MDES::find_MDES_rf_index(Bitvector& iocode, IO_kind kind, bool flag_not_found) {
  if (kind==IO_VIRTUAL) {
    for (int i=0; i<get_MDES_num_field_types(); i++) {
      Ft_descr* desc = ft_descr_tab[i];
      if (iocode==desc->get_iocode()) return i;
    }
  } else {
    for (int i=0; i<get_MDES_num_reg_types(); i++) {
      Reg_descr* desc = reg_descr_tab[i];
      if (iocode==desc->get_iocode()) return i;
    }
  }
  if (flag_not_found) {
    cdbg << iocode;
    El_punt("MDES::find_MDES_rf_index: RF with above iocode not found.");
  }
  return (-1);
}

void MDES::find_insert_MDES_ft_descr(Bitvector& iocode) {
  char buff[1024];
  Bitvector code(iocode);
  // check for iocode coverage with one ft descr.
  strcpy(buff, find_MDES_rf_name(iocode, IO_VIRTUAL, &iocode));
  if (!iocode.is_allclear()) {
    // collect the set of ft descr (IO_VIRTUAL) covering full iocode
    // and emit a new ftdescr as a concatenation. 
    while (!iocode.is_allclear()) {
      strcat(buff, "_");
      strcat(buff, find_MDES_rf_name(iocode, IO_VIRTUAL, &iocode));
    }
    Ft_descr* newdesc = new Ft_descr(this);
    newdesc->set_ft_descr(strdup(buff), code);
    set_MDES_ft_descr(get_MDES_num_field_types(), newdesc);
  }
}

Bitvector MDES::find_MDES_rf_iocode(char* iospec, IO_kind kind, int* numcharptr) {
  //int iocode=0;
  Bitvector iocode(IOCODE_SIZE);
  int matchlen=0;
  // If iospec is '_' we return iocode=-1, which is supposed to match
  // anything.  -- Shail Aditya 07/09/2001
  if (iospec && *iospec=='_') {
    matchlen = 1; iocode.set();
  } else if (kind==IO_VIRTUAL) {
    Ft_descr* ftdesc=NULL;
    for (int i=0; i < get_MDES_num_field_types() ; i++) {
      Ft_descr *desc = ft_descr_tab[i];
      char* descname = desc->get_name();
      int len = strlen(descname);
      if (!strncmp(descname, iospec, len) && matchlen<len) {
        matchlen = len; 
        ftdesc = desc;
      }
    }
    if (ftdesc) iocode = ftdesc->get_iocode();
  } else {   // IO_PHYSICAL
    Reg_descr* regdesc=NULL;
    //int viocode=0;
    Bitvector viocode(IOCODE_SIZE);
    for (int i=0; i < get_MDES_num_reg_types() ; i++) {
      Reg_descr *desc = reg_descr_tab[i];
      char* descname = desc->get_aname();
      int len = strlen(descname);
//      cdbg<<descname<<","<<desc->get_iocode()<<endl;
      // check for the maximum length match with physical reg names
      if (!strncmp(descname, iospec, len) && matchlen<len) {
        matchlen = len;
        regdesc = desc;
      }
      // check for iocode match with virtual reg name (single char)
      // collect all physical iocodes that match input virtual name
      // -- Shail Aditya 07/11/2001
      if (strchr(desc->get_vname(), *iospec))
        //viocode |= desc->get_iocode();
        viocode += desc->get_iocode();
    }
    // return physical match if found, otherwise return virtual match
    if (regdesc) iocode = regdesc->get_iocode();
    else if (!viocode.is_allclear()) {
      matchlen = 1; iocode = viocode;
    }
  }
  if (iocode.is_allclear()) El_punt("MDES: Illegal iospec encountered '%s'", iospec);
  if (numcharptr) *numcharptr = matchlen;
  return iocode;
}

void MDES::collect_io_descrs(char* opcode, List_set<Io_descr*>& descset) {
  Io_list* iol_elem;
  Io_descr* iod_elem;
  Op_descr* operation = hashtable->find_op(opcode);
  if (!operation)
    El_punt("MDES::collect_io_descrs: Couldn't find operation '%s'", opcode);

  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    iod_elem=iol_elem->get_iod();
    descset += iod_elem;
  }
}

void MDES::collect_reg_descrs(char* opcode, IO_Portkind portkind, int portnum,
            List_set<Reg_descr*>& descset) {
  Io_list* iol_elem;
  Io_descr* iod_elem;
  assert(get_MDES_kind()==MDES_PHYSICAL);
  Op_descr* operation = hashtable->find_op(opcode);
  if (!operation) {
    if (El_punt_on_unknown_op)
      El_punt("MDES::collect_reg_descrs: Couldn't find operation '%s'", opcode);
    return;
  }

  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    iod_elem=iol_elem->get_iod();
    iod_elem->collect_reg_descrs(portkind, portnum, descset);
  }
}

void MDES::collect_reg_descrs(Op_descr* operation, IO_Portkind portkind, int portnum,
            List_set<Reg_descr*>& descset) {
  Io_list* iol_elem;
  Io_descr* iod_elem;
  assert(get_MDES_kind()==MDES_PHYSICAL);
  assert(operation);
  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    iod_elem=iol_elem->get_iod();
    iod_elem->collect_reg_descrs(portkind, portnum, descset);
  }
}

void MDES::collect_ft_descrs(char* opcode, IO_Portkind portkind, int portnum,
           List_set<Ft_descr*>& descset) {
  Io_list* iol_elem;
  Io_descr* iod_elem;
  assert(get_MDES_kind()==MDES_VIRTUAL);
  Op_descr* operation = hashtable->find_op(opcode);
  if (!operation)
    El_punt("MDES::collect_ft_descrs: Couldn't find operation '%s'", opcode);

  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    iod_elem=iol_elem->get_iod();
    iod_elem->collect_ft_descrs(portkind, portnum, descset);
  }
}

Op_descr *MDES::collect_generic_op_descrs(char* opcode) {
  Op_descr *opdescr = hashtable->find_op(opcode);
  return opdescr;
}

void MDES::collect_op_descrs(char* opcode, List_set<Op_descr*>& descset) {
  Alt_descr* altd_elem;
  Io_list* iol_elem;
  Op_descr* operation = hashtable->find_op(opcode);
  if (!operation) {
    if (El_punt_on_unknown_op)
      El_punt("MDES::collect_op_descrs: Couldn't find operation '%s'", opcode);
    return;
  }

  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    while((altd_elem=iol_elem->get_next_alt()) != NULL){
      descset += altd_elem->get_op();
    }
  }
}

void MDES::collect_alt_descrs(char *opcode, List_set<Alt_descr*>& descset) {
  Alt_descr* altd_elem;
  Io_list* iol_elem;
  Op_descr* operation = hashtable->find_op(opcode);
  if (!operation)
    El_punt("MDES::collect_alt_descrs: Couldn't find operation '%s'", opcode);

  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    while((altd_elem=iol_elem->get_next_alt()) != NULL){
      descset += altd_elem;
    }
  }
}

void MDES::collect_res_use_descrs(char *opcode, List_set<Res_use_descr*>& descset) {
  Alt_descr* altd_elem;
  Io_list* iol_elem;
  Op_descr* operation = hashtable->find_op(opcode);
  if (!operation)
    El_punt("MDES::collect_res_use_descrs: Couldn't find operation '%s'", opcode);

  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    while((altd_elem=iol_elem->get_next_alt()) != NULL){
      descset += altd_elem->get_res_use();
    }
  }
}

ostream& operator<<(ostream& os, const MDES& md) {
  md.print_stream(os);
  return os;
}


void MDES::print(void) const { 

// #if __GNUC__ >= 3
// #if __GNUC_MINOR__ >= 1
//   __gnu_cxx::stdio_filebuf<char> stream_buf(MDES_out, ios::out);
// #else
//   filebuf stream_buf(MDES_out, ios::out);
// #endif
// #else
//   stdiobuf stream_buf(MDES_out);
// #endif
//   ostream os(&stream_buf);
//   os << *this << flush;
  *MDES_out << *this << flush;
}

void MDES::print_stream(ostream& os) const {
  int i ;

  os << "Resources:\n";
  for (i = 0; i < get_MDES_num_resources() ; i++) {
    Res_descr* desc = res_descr_tab[i];
    os << desc->get_name() << "_" << i << " ";
  }
  os << endl;

  os << "\nLatencies:\n";
  for (i = 0; i < get_MDES_num_lat_classes() ; i++) {
    Lat_descr* desc = lat_descr_tab[i];
    os << i << " " << *desc;
  }

  os << "\nRegister Files:\n";
  for (i = 0; i < get_MDES_num_reg_types() ; i++) {
    Reg_descr* desc = reg_descr_tab[i];
    os << i << " " << *desc;
  }

  os << "\nOperations: (use dbg_mdes=4 for full details)\n" << *hashtable;
}

/////////////////////////////////////////////////////////////////////////////
// MdesHashtable

MdesHashtable::MdesHashtable(MDES* mdes, int modu)
  : md(mdes), modulus(modu), numops(0),
    htable(Vector<char*>(modu, NULL)),
    htable_ptr(Vector<Op_descr*>(modu, NULL))
{
}

MdesHashtable::MdesHashtable(MdesHashtable& table) 
  : md(table.md), modulus(table.modulus), numops(table.numops),
    htable(Vector<char*>(table.htable)),
    htable_ptr(Vector<Op_descr*>(table.htable_ptr))
{
}

void MdesHashtable::expand() {
  modulus = modulus*2;
  if (dbg(mdes)) El_warn("MDES: expanding hashtable to %d.", modulus);

  // save current table
  Vector<char*> oldhtable(htable);
  Vector<Op_descr*> oldhtable_ptr(htable_ptr);

  // resize and clean the table (get around gcc bug)
  char* null_str = (char*)NULL;
  Op_descr* null_ptr = (Op_descr*)NULL;
  htable.resize(modulus, null_str);
  htable_ptr.resize(modulus, null_ptr);

  // Reinsert all elements
  numops = 0;
  for(int j=0; j<modulus/2; j++) 
    if (oldhtable[j]!=NULL) insert_op(oldhtable[j], oldhtable_ptr[j]);
}

void MdesHashtable::clear() {
  numops = 0;
  for (int i=0; i<modulus ; i++) {
    htable[i] = NULL;
    htable_ptr[i] = NULL;
  }
}  

MDES* MdesHashtable::get_mdes() const { return md; }

void MdesHashtable::set_mdes(MDES* mdes) { md = mdes; }

int MdesHashtable::hash(char *string)
{
  int sum,index, t1, t2;

  sum=0;
  for (index=0; string[index]!= '\0' ; index++)
    {
      t1=string[index];
      t2=sum<<1;
      t2=t2+sum;
      t2=t2+t1;
      sum=ELCOR_INT_ABS(t2);
    }
  sum=ELCOR_INT_ABS((sum<<4)+sum);
  sum=sum%modulus;
  return(sum);
}

int MdesHashtable::insert_op(char *string,Op_descr *location) {
  int hash_val, save_val;
  
  save_val=hash_val=hash(string);
  while(true) {
    if (htable[hash_val] == 0)  {            /*hashes to empty bin*/
      // We use the supplied key without copying
      // -- Shail Aditya 02/07/97
      numops++;
      htable[hash_val]=string;               /*establish key*/
      htable_ptr[hash_val]=location;       /*establish pointer*/
      return 1;                              /*return with new entry*/
    }
    if (strcmp(htable[hash_val],string)==0) {/*hashes to full & matching bin*/
      if (dbg(mdes))
        El_warn("MDES: operation %s already exists in the hashtable.", string);
      htable_ptr[hash_val]=location;
      return 1;                              /*return with overwritten entry*/
    }
      
    hash_val = mod_inc(hash_val,modulus);    /*hashes to full non-matching bin*/
    //if (hash_val == save_val) {              /*hash table full--expand and retry*/
    // use more aggressive hash table expansion. shouldn't be too much space since
    // each entry is just 2 pointers. -- KF 5/2004
    if (numops * 4 >= modulus) {
      expand();
      save_val=hash_val=hash(string);
    }
  }
  return(0);
}

Op_descr*  MdesHashtable::find_op(char *string)
{
  int hash_val, save_val;
  Op_descr *op;
  
  hash_val=hash(string);
  save_val=hash_val;
  while(true)  {
    if (htable[hash_val] == 0) 
      return(0); /* found empty slot */
       
    if (strcmp(string,htable[hash_val])==0) {
      op=htable_ptr[hash_val]; op->io_list_init(); return(op); /* found it*/ } 
    hash_val = mod_inc(hash_val,modulus);
    if (hash_val == save_val) return NULL; /* wrapped */
  }
  return (NULL);
}

void MdesHashtable::init_next_op() { index = 0; }

Op_descr* MdesHashtable::get_next_op() {
  assert(index>=0);
  for (; index<modulus; index++)
    if (htable_ptr[index]!=NULL) return (htable_ptr[index++]);
  return(NULL);
}

int MdesHashtable::get_num_ops() const { return numops; }

ostream& operator<<(ostream& os, const MdesHashtable& table) {
  table.print_stream(os);
  return os;
}

void MdesHashtable::print(void) const { 
// #if __GNUC__ >= 3
// #if __GNUC_MINOR__ >= 1
//   __gnu_cxx::stdio_filebuf<char> stream_buf(MDES_out, ios::out);
// #else
//   filebuf stream_buf(MDES_out, ios::out);
// #endif
// #else
//   stdiobuf stream_buf(MDES_out);
// #endif
//   ostream os(&stream_buf);
//   os << *this << flush;
  *MDES_out << *this << flush;
}

//void MdesHashtable::print_stream(FILE *stream) const {
//void MdesHashtable::print_stream(ostream *stream) const {
// #if __GNUC__ >= 3
// #if __GNUC_MINOR__ >= 1
//   __gnu_cxx::stdio_filebuf<char> stream_buf(stream, ios::out);
// #else
//   filebuf stream_buf(stream, ios::out);
// #endif
// #else
//   stdiobuf stream_buf(stream);
// #endif
//   ostream os(&stream_buf);
//   os << *this;
//   *stream << *this;
// }

void MdesHashtable::print_stream(ostream& os) const {
  int i;
  // print all op descrs
  // print all alternatives for generic ops
  for (i=0; i<modulus; i++) {
    if (htable[i]) {
      Op_descr* opdesc = htable_ptr[i];
      os << htable[i] << " ";
      if (dbg(mdes,3)) {
  os << ":" << *opdesc << endl;
  if (opdesc->get_binding() == UNITOP) continue;
  os << "Alternatives:\n";
  opdesc->io_list_init();
  Io_list* iol_elem = opdesc->get_next_io();
  for (;iol_elem;iol_elem=opdesc->get_next_io()) {
    Alt_descr* alt = iol_elem->get_next_alt();
    for (;alt;alt=iol_elem->get_next_alt()) {
      Op_descr* aopdesc = alt->get_op();
      if (dbg(mdes,4)) os << *alt;
      else os << " " << aopdesc->get_aname_ptr();
    }
  }
  os << endl;
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Op_descr

Op_descr::Op_descr(MDES* mdes) 
  : md(mdes), io_ptr(NULL), lat_ptr(NULL) {}

void Op_descr::fill_op(char* lnam,char* anam, Io_list* io_list_alt,
           OpLevel binding, int numin, bool predin, int numout, 
           OpClass opclass, ResClass resclass, bool eager, int priority ) {

   Lname=lnam;        /*set lcode name*/
   Aname=anam;        /*set assembly name*/
   name_eq = strcmp(lnam,anam)==0;
   num_input=numin;
   pred=predin;
   num_output=numout;
   io_ptr=io_list_alt;      /*connect op to IO descriptor*/
   op_binding=binding;            /*set operation binding*/
   less_than=true;      /*set less than*/
   op_class=opclass;            /*set class*/
   res_class=resclass;                  /*set resource class*/
   spec_version=eager;      /*this operation supports speculation*/
   op_priority=priority;

   //
   // less_than = leop;                    /* If true, this operation is less/equal */
   // is_adv_load = adv_load;              /* If true, this operation is advanced load */
   // assoc = is_assoc;                    /* If true, this operation is associative */
   // comm  = is_comm;                     /* If true, this operation is commutative */
   // 
}

ostream& operator<<(ostream& os, const Op_descr& desc){
  desc.print_stream(os);
  return os;
}

void Op_descr::print(void) const { 
// #if __GNUC__ >= 3
// #if __GNUC_MINOR__ >= 1
//   __gnu_cxx::stdio_filebuf<char> stream_buf(MDES_out, ios::out);
// #else
//   filebuf stream_buf(MDES_out, ios::out);
// #endif
// #else
//   stdiobuf stream_buf(MDES_out);
// #endif
//   ostream os(&stream_buf);
//   os << *this << flush;
  *MDES_out << *this << flush;
}

void Op_descr::print_stream(ostream& os) const {
  os << "Ln=" << Lname << " An=" << Aname << " level=" << oplevel(op_binding)
     << " #_in=" << num_input << " pred=" << (int)pred << " #_out=" << num_output
     << " opclass=" << opclass(op_class) << " resclass=" << resclass(res_class)
     << " spec=" << eagerclass(spec_version) << " prio=" << op_priority 
     << " assoc=" << assoc << " comm=" << comm << " two_operand=" << two_operand 
     << " unsupported=" << unsupported << endl;
}

MDES* Op_descr::get_mdes() const { return md; }

char* Op_descr::get_aname_ptr(){return(Aname);}

char* Op_descr::get_lname_ptr(){return(Lname);}

OpClass Op_descr::get_opclass(void){ return(op_class);}

ResClass Op_descr::get_resclass(void){ return(res_class);}

OpLevel Op_descr::get_binding(void){ return(op_binding);}

int Op_descr::get_num_in(void) { return num_input; }

int Op_descr::get_num_out(void) { return num_output; }

bool Op_descr::is_predicated(void) { return pred; }

bool Op_descr::has_spec(){return(spec_version);}

int Op_descr::get_priority(){return(op_priority);}

void Op_descr::io_list_init(){ curr_io=io_ptr; }

//Shivaram: Added on 9th February to return the commutativity of an operation
bool Op_descr::is_commutative() { return comm; }
bool Op_descr::is_associative() { return assoc; }
bool Op_descr::is_advanced_load() { return adv_load; }
bool Op_descr::is_le_op() { return less_than; }
bool Op_descr::is_two_operand() { return two_operand; }
bool Op_descr::is_unsupported() { return unsupported; }

void Op_descr::set_commutative(bool arg_comm)  { comm = arg_comm; }
void Op_descr::set_associative(bool arg_assoc) { assoc = arg_assoc; }
void Op_descr::set_advanced_load(bool arg_advload) { adv_load = arg_advload; }
void Op_descr::set_le_op(bool arg_le_op) { less_than = arg_le_op; }
void Op_descr::set_predicated(bool arg_pred)  { pred = arg_pred; }
void Op_descr::set_two_operand(bool arg_two_oprnd) { two_operand = arg_two_oprnd; }
void Op_descr::set_unsupported(bool arg_unsupported) { unsupported = arg_unsupported; }

Io_list* Op_descr::get_next_io(){
   Io_list *val;
   val=curr_io;
   if(val) val->init_alt();
   if(curr_io) curr_io=curr_io->get_next();
   return(val); }

Alt_descr* Op_descr::find_alt(Io_descr* iod, Res_use_descr* resv) {
  Io_list* io;
  Alt_descr* alt;
  io_list_init();
  for (io=get_next_io(); io!=NULL; io=get_next_io()) {
    Io_descr* this_iod = io->get_iod();
    if (*iod == *this_iod) {
      for (alt=io->get_next_alt(); alt!=NULL; alt=io->get_next_alt()) {
        Res_use_descr* this_resv = alt->get_res_use();
  if (*resv == *this_resv) return alt;
      }
    }
  }
  El_punt("MDES: No alternatives match the given criteria within op '%s'", Aname); 
  return NULL;
}

void Op_descr::add_alts(Io_descr* iodesc, Alt_list* alts) {
  io_list_init();
  Io_list* iohead = get_next_io();
  if (iohead) iohead->find_or_insert(iodesc, alts);
  else {   
    iohead = new Io_list(iodesc, alts);
    io_ptr = iohead;
  }
}

void Op_descr::set_mdes(MDES* mdes) { md=mdes; }

void Op_descr::set_io_list(Io_list* iolist) { io_ptr=iolist; }

int Op_descr::get_encoding() { return encoding; }

void Op_descr::set_encoding(int enc) { encoding = enc; }

unsigned hash_Op_descr_ptr(Op_descr*& ptr) 
{
   return (ELCOR_INT_HASH(ptr)) ;
}


////////////////////////////////////////////////////////////////////////
// Class Pattern_edge

Pattern_edge::Pattern_edge(MDES* mdes) : md(mdes) { }

Pattern_edge::~Pattern_edge() { }

void Pattern_edge::add_src(Pattern_node* n)  { srcs.push_back(n); }

void Pattern_edge::add_dest(Pattern_node* n) { dests.push_back(n); }

unsigned Pattern_edge::num_srcs() const { return srcs.size(); }

unsigned Pattern_edge::num_dests() const { return dests.size(); }

Pattern_node* Pattern_edge::get_src(unsigned n)  const { return  srcs[n]; }

Pattern_node* Pattern_edge::get_dest(unsigned n) const { return dests[n]; }


///////////////////////////////////////////////////////////////////////
// Class Pattern_node

unsigned hash_pattern_node_ptr(Pattern_node*& v) 
{
  return (ELCOR_INT_HASH(v)) ;
}

ostream& operator<<(ostream& os, const Pattern_node& node)
{
  if(node.type == COP_LIVE_IN)
    os << "Live in" << endl;
  else if(node.type == COP_LIVE_OUT)
    os << "Live out" << endl;
  else if(node.type == COP_OPERATION)
    os << node.opcodes << endl;
  else if(node.type == COP_LITERAL)
    os << *(node.consts) << endl;
  return os;
}

Pattern_node::Pattern_node(MDES* mdes) : md(mdes), opcodes(hash_estring, 8),
					 pred(NULL), srcs(0), dests(0),
					 consts(NULL)
{ }

Pattern_node::~Pattern_node() { }

void Pattern_node::add_opcode(const eString& opc) { opcodes += opc; }

bool Pattern_node::is_opcode_supported(const eString& opc) const
{ return opcodes.is_member(opc); }

void Pattern_node::set_pred(Pattern_edge* pe) { pred = pe; }

void Pattern_node::set_type(PatternNodeType t) { type = t; }

void Pattern_node::set_const_descr(Const_set_descr* cd) { consts = cd; }

Pattern_edge* Pattern_node::get_pred() const { return pred; }

Pattern_edge* Pattern_node::get_src(unsigned n)  const { return srcs[n]; }

Pattern_edge* Pattern_node::get_dest(unsigned n) const { return dests[n]; }

void Pattern_node::set_src(Pattern_edge* pe, unsigned n)
{
  if(n >= srcs.size())
    srcs.resize(n + 1, NULL);
  srcs[n] = pe;
}

void Pattern_node::set_dest(Pattern_edge* pe, unsigned n)
{
  if(n >= dests.size())
    dests.resize(n + 1, NULL);
  dests[n] = pe;
}

unsigned Pattern_node::num_srcs() const  { return srcs.size();  }

unsigned Pattern_node::num_dests() const { return dests.size(); }

bool Pattern_node::is_valid(int n) const
{
  if(type != COP_LITERAL)
    return false;
  return consts->is_valid(n);
}

bool Pattern_node::is_live_in()  const { return(type == COP_LIVE_IN);  }

bool Pattern_node::is_live_out() const { return(type == COP_LIVE_OUT); }


/////////////////////////////////////////////////////////////////////////////
// Custom_op_descr

unsigned hash_custom_op_descr(Custom_op_descr*& v)
{
  return (ELCOR_INT_HASH(v));
}

Custom_op_descr::Custom_op_descr(MDES* mdes) : md(mdes), nodes(0) { }

Custom_op_descr::~Custom_op_descr() { }

void Custom_op_descr::set_opcode(char* str) { opcode = str; }

void Custom_op_descr::set_pattern_size(unsigned n) { nodes.resize(n); }

void Custom_op_descr::add_pattern_node(Pattern_node* n, unsigned idx)
{
  nodes[idx] = n;
}

eString Custom_op_descr::get_opcode() const { return opcode; }

unsigned Custom_op_descr::get_pattern_size() const { return nodes.size(); }

Pattern_node* Custom_op_descr::get_pattern_node(unsigned i) const
{
  return nodes[i];
}


/////////////////////////////////////////////////////////////////////////////
// Alt_list

Alt_list::Alt_list(Alt_descr* altd, Alt_list* nxt, int priority)
  : alt(altd), next(nxt), alt_priority(priority) {}

Alt_list *Alt_list::get_next() { return next; }

Alt_descr *Alt_list::get_alt() { return alt; }

int Alt_list::get_alt_priority() { return alt_priority; }

Alt_list* Alt_list::merge(Alt_list* inlist) {
  Alt_list *altlist1, *altlist2, althead(NULL), *alttail=&althead;
  // merge this list with the incoming list in the order of increasing
  // alternative priority (lower number is higher priority). The
  // elements of the incoming list are added behind those of the
  // current list when the priorities are the same.

  for (altlist1=this, altlist2=inlist; altlist1!=NULL && altlist2!=NULL; ) {
    // compare the priorities of the heads and merge the larger
    if (altlist1->alt_priority <= altlist2->alt_priority) {
      alttail->next = altlist1;
      alttail = altlist1;
      altlist1 = altlist1->next;
    } else {
      alttail->next = altlist2;
      alttail = altlist2;
      altlist2 = altlist2->next;
    }
  }
  // add on remaining lists
  alttail->next = (altlist1) ? altlist1 : altlist2;

  return althead.next;
}

Alt_list* Alt_list::copy() {
  Alt_list *altlist, althead(NULL), *alttail=&althead;
  for (altlist=this; altlist!=NULL; altlist=altlist->next) {
    Alt_list *newtail = new Alt_list(altlist->alt, NULL, altlist->alt_priority);
    alttail->next = newtail;
    alttail = newtail;
  }
  return althead.next;
}

/////////////////////////////////////////////////////////////////////////////
// Alt_descr

Alt_descr::Alt_descr(MDES* mdes, Op_descr *opdesc, 
         Lat_descr *latdesc, Res_use_descr *resdesc)
  : md(mdes), op(opdesc), lat(latdesc), res(resdesc) {}

MDES* Alt_descr::get_mdes() const { return md; }

void Alt_descr::set_mdes(MDES* mdes) { md = mdes; }

Lat_descr * Alt_descr::get_lat(){return(lat);}

Op_descr *Alt_descr::get_op(){return(op);}

Res_use_descr *Alt_descr::get_res_use(){return(res);}

ostream& operator<<(ostream& os, const Alt_descr& desc){
  desc.print_stream(os);
  return os;
}

void Alt_descr::print(void) const { 
// #if __GNUC__ >= 3
// #if __GNUC_MINOR__ >= 1
//   __gnu_cxx::stdio_filebuf<char> stream_buf(MDES_out, ios::out);
// #else
//   filebuf stream_buf(MDES_out, ios::out);
// #endif
// #else
//   stdiobuf stream_buf(MDES_out);
// #endif
//   ostream os(&stream_buf);
//   os << *this << flush;
  *MDES_out << *this << flush;
}

void Alt_descr::print_stream(ostream& os) const {
  os << "Op: " << op->get_aname_ptr() << endl;
  os << "Lat: " << *lat;
  os << "Res: ";
  for (Res_use_descr* ru=res; ru!=0;) {
    int resnum = ru->get_res();
    int time = ru->get_time();
    int count = ru->get_count();
    ru = ru->get_next();
    os << md->get_MDES_res_descr(resnum)->get_name() << "_" << resnum
       << "(t" << time << ",#" << count << ") ";
  }
  os << endl;
}

/////////////////////////////////////////////////////////////////////////////
// Io_list

Io_list::Io_list(Io_descr *iodesc, Alt_list *altlist, Io_list *nxt)
  : io(iodesc), alt(altlist), next(nxt), curr_alt(NULL) {}

void Io_list::init_alt(){ curr_alt=alt; }

Io_descr *Io_list::get_iod(){return io;}

Alt_list *Io_list::get_altlist() { return alt; }

Alt_descr *Io_list::get_next_alt(){
   Alt_list *save;
   save=curr_alt;
   if(curr_alt) {
     curr_alt=curr_alt->get_next(); 
     return(save->get_alt());
   }
   return(NULL);
}

Io_list* Io_list::get_next(){return(next);}

void Io_list::set_altlist(Alt_list* altlist) { alt = altlist; }

Io_list* Io_list::find_or_insert(Io_descr* iodesc, Alt_list* altlist) {
  Io_list *iotail, *iolast;
  for (iolast=iotail=this; iotail!=0; iolast=iotail, iotail=iotail->next) {
    Io_descr* desc = iotail->io;
    // compare descriptors (all fields)
    if ((*iodesc)==(*desc)) { 
      iotail->alt = (iotail->alt) ? iotail->alt->merge(altlist) : altlist;
      return iotail;
    }
  }
  // add a new iolist to the end if no matching iolist was found
  iotail = new Io_list(iodesc, altlist);
  iolast->next = iotail;
  return iotail;
}  

/////////////////////////////////////////////////////////////////////////////
// Io_descr

Io_descr::Io_descr() :
  md(NULL), numin(0), numout(0), pred(false) {}

Io_descr::~Io_descr() {}

Io_descr::Io_descr(const Io_descr& desc) : 
  md(desc.md), numin(desc.numin), numout(desc.numout),
  in_reg_id(desc.in_reg_id), out_reg_id(desc.out_reg_id),
  pred(desc.pred), pred_id(desc.pred_id), iokind(desc.iokind) {}

Io_descr::Io_descr(MDES* mdes, IO_kind _iokind, bool _pred, int _numin, int _numout) :
  md(mdes), numin(_numin), numout(_numout),
  in_reg_id(_numin), out_reg_id(_numout), pred(_pred), iokind(_iokind) {}

bool Io_descr::operator==(const Io_descr& desc) const{ 
  int i;
  if (md!=desc.md || numin!=desc.numin || numout!=desc.numout || pred!=desc.pred || 
      pred_id != desc.pred_id || iokind != desc.iokind) return false;
  for (i=0; i<numin; i++) if (in_reg_id[i]!=desc.in_reg_id[i]) return false;
  for (i=0; i<numout; i++) if (out_reg_id[i]!=desc.out_reg_id[i]) return false;
  return(true);
}

bool Io_descr::operator!=(const Io_descr& desc) const{ 
  return !operator==(desc);
}

MDES* Io_descr::get_mdes() const { return md; }

int Io_descr::get_num_in(){return(numin);}

int Io_descr::get_num_out(){return(numout);}

bool Io_descr::is_predicated(){return(pred);}

const Bitvector& Io_descr::get_pred() const {return pred_id;}

const Bitvector& Io_descr::get_in_reg(int idx) const {return in_reg_id[idx];}

const Bitvector& Io_descr::get_out_reg(int idx) const {return out_reg_id[idx];}

IO_kind Io_descr::get_iokind() const {return iokind;}

void Io_descr::set_numin(int n) {numin=n;};

void Io_descr::set_numout(int n) {numout=n;};

void Io_descr::set_in_reg(int idx,Bitvector& v) {in_reg_id[idx]=v;};

void Io_descr::set_out_reg(int idx,Bitvector& v) {out_reg_id[idx]=v;};

void Io_descr::set_pred(Bitvector& v) {pred=true;pred_id=v;};

void Io_descr::set_mdes(MDES* mdes) { md=mdes; }

void Io_descr::set_iokind(IO_kind kind) { iokind=kind; }

#define SKIP_WSPACE(ptr) \
while(*(ptr) && isspace(*(ptr))) (ptr)++ 

void Io_descr::parse_iospec(char *iospec){ 
  // parse string and return an ioset - bit representation of register hookup
  char *ptr;
  int i, num, pos;
  Bitvector iocode(IOCODE_SIZE);
  bool has_in, has_out;

  // make the arrays in_reg_id and out_reg_id dynamically allocated to
  // support arbitrary number of inputs and outputs.
  // -- Shail Aditya 07/12/2001
  // find #in and allocate space to encode it
  bool found=false;
  for (num=0, ptr=iospec; *ptr!=':'; ptr++) {
    if (isspace(*ptr)) continue;
    if (found && *ptr==',') { num++; found = false; }
    else found = true;
  }
  if (found) { num++; found = false; }
  in_reg_id.resize(num);
  for(i=0; i<num;i++) in_reg_id[i].clear();         /*initially null hookup*/
  // find #out and allocate space to encode it
  for (num=0, ptr++; *ptr!=0; ptr++) {
    if (isspace(*ptr)) continue;
    if (found && *ptr==',') { num++; found = false; }
    else found = true;
  }
  if (found) { num++; found = false; }
  out_reg_id.resize(num);
  for(i=0; i<num;i++) out_reg_id[i].clear();       /*initially null hookup*/

  // iokind is based upon the mdes kind
  iokind = (md->get_MDES_kind()==MDES_VIRTUAL) ? IO_VIRTUAL : IO_PHYSICAL;

  ptr=iospec; pred=false; 
  SKIP_WSPACE(ptr);

  // search for predicate ?
  char* qmark = strchr(ptr, '?');
  if (qmark) {
    pred=true; 
    pred_id = md->find_MDES_rf_iocode(ptr, iokind);
    ptr=qmark+1;
  }
  
  // search remaining inputs
  numin=0; has_in=false;
  while( *ptr!=':' && *ptr!=0 ) {
    SKIP_WSPACE(ptr);
    // check inputs until ":" (after ":" move to output)
    while((strchr(",:",*ptr)==0) && *ptr!=0) {          /* end an input on ,:*/
      iocode = md->find_MDES_rf_iocode(ptr,iokind,&pos);/* tx iospec, get iocode*/
      in_reg_id[numin] += iocode;                       /* sum in iocodes */
      ptr += pos; has_in=true;
      SKIP_WSPACE(ptr);
    }
    if (has_in) numin++;                                /*record new input and continue*/
    if(*ptr==',') ptr++;
    has_in=false;
  }
  assert(*ptr==':');
  ptr++;                                    /*this skips across : */

  // search outputs
  numout=0; has_out=false;
  while(*ptr!=0 ) {
    SKIP_WSPACE(ptr);
    // check outputs until end of string
    while((strchr(",",*ptr)==0) && *ptr!=0) {          /*end an output on ,*/
      iocode = md->find_MDES_rf_iocode(ptr,iokind,&pos);/* tx iospec, get iocode*/
      out_reg_id[numout] += iocode;                     /* sum in iocodes */
      ptr += pos; has_out=true;
      SKIP_WSPACE(ptr);
    }
    if (has_out) numout++;                              /*record new output and continue*/
    if(*ptr==',') ptr++;
    has_out=false;
  }
}

ostream& operator<<(ostream& os, const Io_descr& desc){
  desc.print_stream(os);
  return os;
}

void Io_descr::print(void) const { 
// #if __GNUC__ >= 3
// #if __GNUC_MINOR__ >= 1
//   __gnu_cxx::stdio_filebuf<char> stream_buf(MDES_out, ios::out);
// #else
//   filebuf stream_buf(MDES_out, ios::out);
// #endif
// #else
//   stdiobuf stream_buf(MDES_out);
// #endif
//   ostream os(&stream_buf);
//   os << *this << endl << flush;
  *MDES_out << *this << endl << flush;
}

void Io_descr::print_stream(ostream& os) const {
  int i;

  if(pred) {
    // find the field which matches iocode
    os << md->find_MDES_rf_name(pred_id, iokind) << " ? ";
  }
  for (i=0;i<numin;i++){
    // find the field which matches iocode
    Bitvector iocode = in_reg_id[i];
    while (!iocode.is_allclear())
      os << md->find_MDES_rf_name(iocode, iokind, &iocode) << " ";
    // print comma separator for all but last input
    if(i<numin-1) os << ", "; 
  }
  os << " : ";
  for (i=0;i<numout;i++){
    // find the field which matches iocode
    Bitvector iocode = out_reg_id[i];
    while (!iocode.is_allclear())
      os << md->find_MDES_rf_name(iocode, iokind, &iocode) << " ";
    // print comma separator for all but last output
    if(i<numout-1) os << ", "; 
  }
}

bool Io_descr::test_io(Io_descr *request){
  int i;
  if ((numin!=request->numin)||(numout!=request->numout)||(pred!=request->pred))
    El_punt("Io_descr::test_io: number of io's do not match.");
  if (iokind!=request->iokind) El_punt("Io_descr::test_io: IO_kind do not match.");
  if (pred) {
    Bitvector pred_tmp = pred_id;
    pred_tmp *= request->pred_id;
    if (pred_tmp.is_allclear())
      return false;
  }
  for (i=0;i<numin;i++) {
    Bitvector reg_tmp = in_reg_id[i];
    reg_tmp *= request->in_reg_id[i];
    if(reg_tmp.is_allclear())
      return false;
  }
  for (i=0;i<numout;i++) {
    Bitvector reg_tmp = out_reg_id[i];
    reg_tmp *= request->out_reg_id[i];
    if(reg_tmp.is_allclear())
      return false;
  }
  return(true);
}

Bitvector Io_descr::get_iocode(IO_Portkind portkind, int portnum) {
  Bitvector port_iocode(IOCODE_SIZE);
  // Obtain appropriate ioset.  predicate is taken to be the zeroth
  // data input, and zeroth data output is unused -- Shail Aditya
  switch (portkind) {
  case DATA_IN: 
    port_iocode = (portnum==0) ? get_pred() : get_in_reg(portnum-1);
    break;
  case DATA_OUT:
    port_iocode = get_out_reg(portnum-1);
    break;
  default:
    El_punt("Io_descr::get_iocode: Portkind must be one of DATA_IN or DATA_OUT.");
  }
  return port_iocode;
}


void Io_descr::collect_reg_descrs(IO_Portkind portkind, int portnum,
          List_set<Reg_descr*>& descset) {
  Bitvector port_iocode = get_iocode(portkind, portnum);
  // collect corresponding reg_descrs
  for (int i=0; i<md->get_MDES_num_reg_types(); i++) {
    Reg_descr* regdesc = md->get_MDES_reg_descr(i);
    if (regdesc->match_ioset(port_iocode))
      descset += regdesc;
  }
}

void Io_descr::collect_ft_descrs(IO_Portkind portkind, int portnum,
         List_set<Ft_descr*>& descset) {
  Bitvector port_iocode = get_iocode(portkind, portnum);
  // collect corresponding ft_descrs
  for (int i=0; i<md->get_MDES_num_field_types(); i++) {
    Ft_descr* ftdesc = md->get_MDES_ft_descr(i);
    if (ftdesc->match_ioset(port_iocode))
      descset += ftdesc;
  }
}

unsigned hash_Io_descr_ptr(Io_descr*& v) 
{
  return (ELCOR_INT_HASH(v)) ;
}


/////////////////////////////////////////////////////////////////////////////
// Ft_descr

Ft_descr::Ft_descr(){}

Ft_descr::Ft_descr(MDES* mdes, IO_kind kind) : md(mdes), iokind(kind) {}

MDES* Ft_descr::get_mdes() const { return md; }

char* Ft_descr::get_name() const {return name;}

const Bitvector& Ft_descr::get_iocode() const {return iocode;}

IO_kind Ft_descr::get_iokind() const {return iokind;}

bool Ft_descr::match_ioset(Bitvector& ioset) const {
  return ((iocode * ioset)==iocode);
}

bool Ft_descr::operator==(const Ft_descr& desc) const{ 
  return (md==desc.md && iocode==desc.iocode && iokind==desc.iokind);
}

bool Ft_descr::operator!=(const Ft_descr& desc) const{ 
  return !operator==(desc);
}

void Ft_descr::set_mdes(MDES* mdes) { md = mdes; }

void Ft_descr::set_iokind(IO_kind kind) { iokind = kind; }

void Ft_descr::set_ft_descr(char* ftname, Bitvector& ioc) { 
  name = ftname;
  iocode = ioc;
}


/////////////////////////////////////////////////////////////////////////////
// Const_set_descr

Const_set_descr::Const_set_descr(MDES* mdes) : md(mdes), name(""), mins(),
					       maxes(), steps()
{}

void Const_set_descr::set_mdes(MDES* mdes) { md = mdes; }
void Const_set_descr::set_name(const eString& n) { name = n; }
MDES* Const_set_descr::get_mdes() const { return md; } 
eString Const_set_descr::get_name() const { return name; }

void Const_set_descr::set_mins(const Vector<int>& v)
{
  mins = v;
}

void Const_set_descr::set_maxes(const Vector<int>& v)
{
  maxes = v;
}

void Const_set_descr::set_steps(const Vector<int>& v)
{
  steps = v;
}

bool Const_set_descr::is_valid(int n) const
{
  for(unsigned i = 0; i < mins.size(); i++) {
    if(n < mins[i] || n > maxes[i])
      continue;
    
    int mod = mins[i] % steps[i];
    if(mod < 0)
      mod += steps[i];

    int nmod = n % steps[i];
    if(nmod < 0)
      nmod += steps[i];
    
    if(nmod == mod)
      return true;
  }
  return false;
}

unsigned Const_set_descr::get_size() const
{
  unsigned total = 0;
  for(unsigned i = 0; i < mins.size(); i++) {
    total += (((maxes[i] - mins[i]) / steps[i]) + 1);
  }
  return total;
}

bool Const_set_descr::overlaps(Const_set_descr* csd)
{
  Const_set_descr *small, *big;
  if(get_size() < csd->get_size()) {
    small = this;
    big = csd;
  } else {
    small = csd;
    big = this;
  }

  for(unsigned i = 0; i < small->mins.size(); i++) {
    for(int j = small->mins[i]; j <= small->maxes[i]; j += small->steps[i]) {
      if(big->is_valid(j))
	return true;
    }
  }
  return false;
}

ostream& operator<<(ostream& os, const Const_set_descr& desc)
{
  desc.print_stream(os);
  return os;
}

void Const_set_descr::print_stream(ostream& os) const
{
  os << "Const_set_descr: " << endl;
  for(unsigned i = 0; i < mins.size(); i++) {
    os << '\t' << mins[i] << '\t' << maxes[i] << '\t' << steps[i] << '\n';
  }
  cdbg << endl;
}

void Const_set_descr::print(void) const
{
  *MDES_out << *this << flush;
}


/////////////////////////////////////////////////////////////////////////////
// Reg_descr

Reg_descr::Reg_descr(MDES* mdes) 
  : md(mdes), vname(NULL), aname(NULL), s_cap(0), r_cap(0), width(0), vector_length(0),
    has_tag(false), iocode(0), regkind(REG_PHYSICAL), dlits(NULL), ilits(NULL),
    caller_save(hash_estring), callee_save(hash_estring), macros(hash_estring),
    physical_register_map(hash_estring, 1000)
{}

void Reg_descr::set_mdes(MDES* mdes) { md=mdes; }

void Reg_descr::set_reg(char* vnam, char* anam, int scap, 
      int rcap, int wid, int vlen, bool tag, int ioc) {
  vname = vnam;
  aname = anam;
  s_cap = scap;
  r_cap = rcap;
  width = wid;
  /* RMR { */
  vector_length = vlen;
  /* } RMR */
  has_tag = tag;
  iocode = ioc;
}

MDES* Reg_descr::get_mdes() const { return md; }
char* Reg_descr::get_vname() const { return vname; }
char* Reg_descr::get_aname() const { return aname; }
int Reg_descr::get_scap() const { return s_cap; }
int Reg_descr::get_rcap() const { return r_cap; }
int Reg_descr::get_width() const { return width; }
/* RMR { */
int Reg_descr::get_vector_length() const { return vector_length; }
/* } RMR */
bool Reg_descr::get_tag() const { return has_tag; }
Bitvector Reg_descr::get_iocode() const
{
  Bitvector bv(IOCODE_SIZE);
  bv.set_bit(iocode);
  return bv;
}
int Reg_descr::get_io_id() const { return iocode; }
Reg_kind Reg_descr::get_regkind() const { return regkind; }

bool Reg_descr::match_ioset(Bitvector& ioset) const {
  //return ((iocode * ioset)==iocode);
  return ioset.bit(iocode);
}

ostream& operator<<(ostream& os, const Reg_descr& desc) {
  desc.print_stream(os);
  return os;
}

void Reg_descr::print(void) const {
  *MDES_out << *this << flush;
}

void Reg_descr::print_stream(ostream& os) const {
  os << "Reg File: vname=" << vname << " aname=" << aname << " scap=" 
     << s_cap << " rcap=" << r_cap << " width=" << width << " iocode=" 
     << iocode;
  /*os << "Reg File: vname=" << vname << " aname=" << aname << " scap=" << s_cap
     << " rcap=" << r_cap << " width=" << width << " iocode=0x" << hex 
     << iocode << dec;*/
  if (has_tag) os << " SPEC";
  os << endl;
}

bool Reg_descr::is_undefined() const { 
  return (regkind==REG_UNDEFINED); 
}

bool Reg_descr::is_lit() const { 
  return (regkind!=REG_PHYSICAL && regkind!=REG_UNDEFINED); 
}

bool Reg_descr::is_intlit() const { 
  return (regkind==REG_LITINT || regkind==REG_LITINTRANGE); 
}

bool Reg_descr::is_doublelit() const { 
  return (regkind==REG_LITDOUBLE); 
}

bool Reg_descr::is_valid(int litval) const {
  switch (regkind) {
  case REG_LITINT:
  case REG_LITINTRANGE:
    assert(ilits);
    return ilits->is_valid(litval);
    break;
  default: break;
  }
  return false;
}

bool Reg_descr::is_valid(double litval) const {
  unsigned int i;
  switch (regkind) {
  case REG_LITDOUBLE:
    assert(dlits);
    for (i=0; i<dlits->size(); i++) {
      if (litval==(*dlits)[i]) return true;
    }
    break;
  default: break;
  }
  return false;
}

void Reg_descr::set_regkind(Reg_kind kind) { regkind = kind; }
void Reg_descr::set_ilits(Const_set_descr* lits) { ilits = lits; }
void Reg_descr::set_dlits(Vector<double>* vect) { dlits = vect; }

Vector<double>* Reg_descr::get_dlits() { return dlits; }
Const_set_descr* Reg_descr::get_ilits() { return ilits; }

bool Reg_descr::is_caller_save(eString f) { return (caller_save.is_member(f)); }
bool Reg_descr::is_callee_save(eString f) { return (callee_save.is_member(f)); }

// bool Reg_descr::is_read_only(eString mn) 
// { 
//   Hash_set<eString> read_onlys = md->get_MDES_read_only_macros();
//   return read_onlys.is_member(mn);
// }

// bool Reg_descr::is_pseudo(eString mn)
// {
//   Hash_set<eString> pseudos = md->get_MDES_pseudo_macros();
//   return pseudos.is_member(mn);
// }

// bool Reg_descr::is_alias(eString mn)
// {
//   Hash_set<eString> aliases = md->get_MDES_alias_macros();
//   return aliases.is_member(mn);
// }

bool Reg_descr::is_macro(eString f) { 
  if(macros.is_member(f)) return true;
  Hash_set<eString>& aliases = md->get_MDES_alias_macros();
  if(aliases.is_member(f) && macros.is_member(md->get_macro_aliased(f)))
    return true;
  
  return false;
}

void Reg_descr::set_caller_save(eString f) { caller_save += f; }
void Reg_descr::set_callee_save(eString f) { callee_save += f; }
void Reg_descr::add_macro(eString f) { macros += f; }

Hash_set<eString>& Reg_descr::get_caller_save() { return caller_save; }
Hash_set<eString>& Reg_descr::get_callee_save() { return callee_save; }

// Hash_set<eString> Reg_descr::get_read_only() { 
//   Hash_set<eString> ro;

//   for(Hash_set_iterator<eString> hsi(macros); hsi!=0; hsi++) {
//     eString macro = *hsi;
//     if(is_read_only(macro))
//       ro += macro;
//   }

//   return ro;
// }

// Hash_set<eString> Reg_descr::get_pseudo() { 
//   Hash_set<eString> pseudos = md->get_MDES_pseudo_macros();
//   return pseudos;
// }
 
// Hash_set<eString> Reg_descr::get_alias()  { 
//   Hash_set<eString> aliases;

//   for(Hash_set_iterator<eString> hsi(macros); hsi!=0; hsi++) {
//     eString macro = *hsi;
//     if(is_alias(macro))
//       aliases += macro;
//   }

//   return aliases;
// }

void Reg_descr::get_macros(Hash_set<eString>& return_macros) { 
  return_macros = macros;
  Hash_set<eString>& aliases = md->get_MDES_alias_macros();
  for(Hash_set_iterator<eString> hsi(aliases); hsi!=0; hsi++) {
    eString mac = *hsi;
    if(is_macro(mac)) {
      return_macros += mac;
    }
  }
}

Hash_set<eString>& Reg_descr::get_physical_regs(eString macro) 
{ 
  Hash_set<eString>& aliases = md->get_MDES_alias_macros();
  if(aliases.is_member(macro))
    macro = md->get_macro_aliased(macro);

  if(!physical_register_map.is_bound(macro))
    El_punt("Reg_descr::get_physical_regs: Macro %s does not have a physical register map entry in reg file %s", macro.strdup(), aname);

  return physical_register_map.value(macro); 
}

Hash_set<eString>& Reg_descr::get_physical_regs(Macro_name mn) 
{ 
  eString macro = el_macro_to_string_map.value(mn);

  Hash_set<eString>& aliases = md->get_MDES_alias_macros();
  if(aliases.is_member(macro))
    macro = md->get_macro_aliased(macro);

  if(!physical_register_map.is_bound(macro))
    El_punt("Reg_descr::get_physical_regs: Macro %s does not have a physical register map entry in reg file %s", macro.strdup(), aname);

  return physical_register_map.value(macro);
}

void Reg_descr::set_physical_reg_map(eString key, eString value) 
{ 
  if(physical_register_map.is_bound(key)) {
    Hash_set<eString> physical_regs = physical_register_map.value(key);
    physical_register_map.unbind(key);
    physical_regs += value;
    physical_register_map.bind(key, physical_regs);
  }
  else {
    Hash_set<eString> new_physical_regs(hash_estring);
    new_physical_regs += value;
    physical_register_map.bind(key, new_physical_regs);
  }
}

Hash_map<eString, Hash_set<eString> >& Reg_descr::get_physical_reg_map()
{
  return physical_register_map;
}


unsigned hash_Reg_descr_ptr(Reg_descr*& v) 
{
  return (ELCOR_INT_HASH(v)) ;
}

/////////////////////////////////////////////////////////////////////////////
// Res_descr

Res_descr::Res_descr(MDES* mdes) : md(mdes), res_width(0) {}

void Res_descr::set(char* name, int count, bool v){
  res_name = name;
  res_count = count;
  is_virtual = v;
}

void Res_descr::set_width(int width) {
  res_width = width;
}

MDES* Res_descr::get_mdes() const { return md; }

void Res_descr::set_mdes(MDES* mdes) { md = mdes; }

char *Res_descr::get_name(){return(res_name);}
  
int Res_descr::get_count(){return(res_count);}

int Res_descr::get_width() {return (res_width); }

bool Res_descr::is_virtual_res() { return (is_virtual); }

/////////////////////////////////////////////////////////////////////////////
// Res_use_descr

Res_use_descr::Res_use_descr(MDES* mdes) : md(mdes) {}

bool Res_use_descr::operator==(const Res_use_descr& desc) const{ 
  bool eqflag = md==desc.md && res==desc.res && pred==desc.pred && 
                rel_time==desc.rel_time && res_count==desc.res_count;
  if (next==desc.next) return eqflag;
  else return (eqflag && next!=NULL && desc.next!=NULL && *next==*(desc.next));
}

bool Res_use_descr::operator!=(const Res_use_descr& desc) const{ 
  return !operator==(desc);
}

MDES* Res_use_descr::get_mdes() const { return md; }

int Res_use_descr::get_res() const { return res; }

int Res_use_descr::get_time() const { return rel_time; }

int Res_use_descr::get_count() const { return res_count; }

Res_use_descr *Res_use_descr::get_next() const { return(next); }

void Res_use_descr::set_mdes(MDES* mdes) { md = mdes; }

void Res_use_descr::set_next(Res_use_descr *res) { next = res; }

void Res_use_descr::set_res_use(int res_index, int time, int count){
  res=res_index; pred=false; rel_time=time; res_count=count; }

/////////////////////////////////////////////////////////////////////////////
// Lat_descr

Lat_descr::Lat_descr(MDES* mdes) : md(mdes) {}

bool Lat_descr::operator==(const Lat_descr& desc) const{ 
  int i;
  // ignore mdes affiliation, rsv0 and do0 -- Shail Aditya 02/26/98
  if (exception!=desc.exception) return false;
  for (i=1; i<MDES_maxout; i++) if (rsv[i]!=desc.rsv[i]) return false;
  for (i=0; i<MDES_maxin; i++) if (di[i]!=desc.di[i]) return false;
  for (i=0; i<MDES_maxsynchin; i++) if (si[i]!=desc.si[i]) return false;
  for (i=1; i<MDES_maxout; i++) if (dox[i]!=desc.dox[i]) return false;
  for (i=0; i<MDES_maxsynchout; i++) if (so[i]!=desc.so[i]) return false;
  return(true);
}

bool Lat_descr::operator!=(const Lat_descr& desc) const{ 
  return !operator==(desc);
}

MDES* Lat_descr::get_mdes() const { return md; }

void Lat_descr::set_mdes(MDES* mdes) { md = mdes; }

void Lat_descr::set_lat_descr(int* a){
  //di0 is the predicate input and rsv0, do0 are unused
  //exc rsv1 rsv2 rsv3 rsv4 di0 di1 di2 di3 di4 si0 si1 do1 do2 do3 do4 so0 so1
  int inx, i;

  inx=0;
  exception=a[inx++];
  for(i=1; i<MDES_maxout;i++) rsv[i]=a[inx++];
  for(i=0;i<MDES_maxin; i++) di[i]=a[inx++];
  for(i=0; i<MDES_maxsynchin;i++) si[i]=a[inx++];
  for(i=1; i<MDES_maxout;i++) dox[i]=a[inx++];
  for(i=0;i<MDES_maxsynchout;i++) so[i]=a[inx++];
}

ostream& operator<<(ostream& os, const Lat_descr& desc) {
  desc.print_stream(os);
  return os;
}

void Lat_descr::print(void) const { 
// #if __GNUC__ >= 3
// #if __GNUC_MINOR__ >= 1
//   __gnu_cxx::stdio_filebuf<char> stream_buf(MDES_out, ios::out);
// #else
//   filebuf stream_buf(MDES_out, ios::out);
// #endif
// #else
//   stdiobuf stream_buf(MDES_out);
// #endif
//   ostream os(&stream_buf);
//   os << *this << flush;
  *MDES_out << *this << flush;
}

void Lat_descr::print_stream(ostream& os) const {
  int i;
  os << "ex:" << exception << " rsv:";
  for(i=1;i<MDES_maxout; i++) os << rsv[i] << " ";
  os << "di:"; 
  for(i=0;i<MDES_maxin; i++) os << di[i] << " ";
  os << "si:"; 
  for(i=0;i<MDES_maxsynchin; i++) os << si[i] << " ";
  os << "do:"; 
  for(i=1;i<MDES_maxout; i++) os << dox[i] << " ";
  os << "so:"; 
  for(i=0;i<MDES_maxsynchout; i++) os << so[i] << " ";
  os << endl;
}

void Lat_descr::get_lat_descr(int *a){
  int inx, i;
  inx=0;
  a[inx++]=exception;
  for(i=1; i<MDES_maxout;i++) a[inx++]=rsv[i];
  for(i=0;i<MDES_maxin;i++) a[inx++]=di[i];
  for(i=0; i<MDES_maxsynchin;i++) a[inx++]=si[i];
  for(i=1; i<MDES_maxout;i++) a[inx++]=dox[i];
  for(i=0;i<MDES_maxsynchout;i++) a[inx++]=so[i];
}

int Lat_descr::get_flow_lat(IO_Portkind portkind, int portnum){ 
  assert(portnum>=0);
  switch(portkind){
  case DATA_IN: 
    assert(portnum<MDES_maxin);
    return di[portnum];
  case DATA_OUT: 
    assert(portnum>=1 && portnum<MDES_maxout);
    return dox[portnum];
  case SYNC_IN: 
    assert(portnum<MDES_maxsynchin);
    return si[portnum];
  case SYNC_OUT :
    assert(portnum<MDES_maxsynchout);
    return so[portnum];
  default: El_punt("illegal spec in get_flow_lat");
  }
  return(-1); // never reach this
}

int Lat_descr::get_anti_lat(IO_Portkind portkind, int portnum){
  assert(portnum>=0);
  switch(portkind){
  case DATA_IN: return(exception);
  case DATA_OUT: 
    assert(portnum>=1 && portnum<MDES_maxout);
    return rsv[portnum];
  case SYNC_IN: return(exception);
  case SYNC_OUT: 
    assert(portnum<MDES_maxsynchout);
    return so[portnum];
  default: El_punt("illegal spec in get_anti_lat");
  }
  return(-1); // never reach this
}

//////////////////////////////////////////////////////////////////////
// Resource Usage Management

RU_mgr::RU_mgr() {}

RU_mgr::RU_mgr(MDES* mdes, int length, int num_resources) 
  : md(mdes), ru_map(length+MDES_max_op_lat, num_resources), request_iod(mdes) {}

RU_mgr::~RU_mgr() {}

void RU_mgr::init(bool mode, int length){
  int i,j;
  // length is the last time at which it is legal to schedule an operation
  if(length > ru_map.dim1()) 
    El_punt("RU_mgr::init: init length for the RU map exceeds its allocated length");
  sched_length = length;
  modulus = length;
  loop_mode = mode;
  for(i=0;i<ru_map.dim1();i++) 
    for(j=0;j<ru_map.dim2();j++) 
      ru_map[i][j].clear();
}

void RU_mgr::init_op(char* opcode, void *op, char *io, int input_time){
  operation = (md->get_MDES_hashtable())->find_op(opcode);

  if (!operation)
    El_punt("RU_mgr::init_op: Couldn't find op '%s' with iospec '%s'",opcode,io);

  request_iod.parse_iospec(io); // place iospec into Io_descr representation
  opsched_time = input_time;    // keep operation time for later placement
  op_ptr = op;
  operation->io_list_init();
  iol_elem = operation->get_next_io();
  iod_elem = iol_elem->get_iod();
  io_match = false;

  // This check is for function outlining. When we do function
  // outlining, groups of operations are scheduled as a single
  // node. This node does not appear in the MDES, and even if it did,
  // the IO wouldn't match because we don't know how many/what type of
  // inputs and outputs there are until runtime.
  if(strcmp(opcode, "CUSTOM"))
    check_io = true;
  else
    check_io = false;
}

bool RU_mgr::get_next_nonconfl_alt(char **opcode, int *priority) {
  while(iol_elem) {
    if( ! check_io || iod_elem->test_io(&request_iod)){       // test for io match 
      io_match = true;

      while((altd_elem = iol_elem->get_next_alt()) != NULL){
	Op_descr* alt_op = altd_elem->get_op();
	char* aname = alt_op->get_aname_ptr();
   
	Res_use_descr* res_use = altd_elem->get_res_use();
	// SAM 3-98: flag to bypass resource check
	if(El_assume_infinite_resources || check_resources_free(res_use)){
	  *opcode = aname;                     // return unit specific op name
	  *priority = alt_op->get_priority();  // return op priority
	  return(true);
        }
      }                                        // search next alternative
    }
    iol_elem = operation->get_next_io();
    if (iol_elem) iod_elem = iol_elem->get_iod();
  }

  // It is an error if there are no IO descriptors (and alternative)
  // to match the give IO binding at all. -- Shail Aditya 08/12/98
  if(!io_match) {
    ostringstream buffstr;
    buffstr << "Requested IO binding: " << request_iod << "\nMDES contains:\n";
    operation->io_list_init();
    iol_elem = operation->get_next_io();
    for (; iol_elem!=0; iol_elem=operation->get_next_io())
      buffstr << *(iol_elem->get_iod());
    El_punt("RU_mgr::get_next_nonconfl_alt: illegal iospec for op '%s'\n%s", 
      operation->get_lname_ptr(), buffstr.str().c_str());
  }
  return(false);
}

void RU_mgr::place(){
  Res_use_descr *current = altd_elem->get_res_use();
  while(current){
    int res_index = current->get_res();
    int time_index = opsched_time + current->get_time();
    if(loop_mode) time_index = time_index%modulus;
    ru_map[time_index][res_index].add_tail(op_ptr);
    current = current->get_next();
    if (dbg(ms,3) || dbg(ss,6)) {
      cdbg << "\t\tres " << res_index << " placed at time " << time_index 
        << " op id = " << ((Op*)op_ptr)->id() << endl;
    }
  }
}

bool RU_mgr::next_alt(char **opcode, int *priority) {
  while(iol_elem){
    if(!check_io || iod_elem->test_io(&request_iod)){     // test for io match
      io_match = true;
      while((altd_elem = iol_elem->get_next_alt()) != NULL){
        Op_descr* alt_op = altd_elem->get_op();
        char* aname = alt_op->get_aname_ptr();
        *opcode = aname;                     // return unit specific op name
        *priority = alt_op->get_priority();  // return op priority
        return(true);
      }                                      // search next alternative
    }
    iol_elem = operation->get_next_io();
    if (iol_elem) iod_elem = iol_elem->get_iod();
  }
  // It is an error if there are no IO descriptors (and alternative)
  // to match the give IO binding at all. -- Shail Aditya 08/12/98
  if(!io_match) {
    ostringstream buffstr;
    buffstr << "Requested IO binding: " << request_iod << "\nMDES contains:\n";
    operation->io_list_init();
    iol_elem=operation->get_next_io();
    for (; iol_elem!=0; iol_elem=operation->get_next_io())
      buffstr << *(iol_elem->get_iod());
    El_punt("RU_mgr::next_alt: illegal iospec for op '%s'\n%s", 
      operation->get_lname_ptr(), buffstr.str().c_str());
  }
  return(false) ;
}


void RU_mgr::get_conflicting_ops(Hash_set<void*>& ops)
{
  // collect all scheduled ops that conflict with any alternative of
  // the current operation -- Shail Aditya 08/12/98
  operation->io_list_init();
  iol_elem = operation->get_next_io();
  while (iol_elem) {
    iod_elem = iol_elem->get_iod();
    if (!check_io || iod_elem->test_io(&request_iod)) {
      while((altd_elem = iol_elem->get_next_alt()) !=NULL){
        collect_conflicting_ops(altd_elem->get_res_use(), ops);
      }
    }
    iol_elem = operation->get_next_io();
  }
}

// This is a very ineffcient algorithm, but is a quick hack to get
// stuff going. What should be done is to find the operation and the
// appropriate iopattern specfic op and then use the resource usage
// information there to remove op from the map. Currenly, we iterate
// over the entire ru_map, removing op where-ever it is found. At
// least restrict the search to a smaller window of time -- Shail
// Aditya 07/09/97
void RU_mgr::remove(void *op, int scheduled_time)
{
  int lower = scheduled_time;
  int upper = scheduled_time+MDES_max_op_lat;
  if (dbg(ms,3) || dbg(ss,6)) {
    cdbg << "Op " << ((Op*)op)->id() << " removed from stime " 
      << scheduled_time << endl;
  }

  // We need to deal with wrap around with modulo scheduled code
  if (loop_mode) {
    if (modulus<=MDES_max_op_lat) {
      lower = 0; upper = modulus;
    } else {
      lower = lower % modulus;
      upper = upper % modulus; 
      if (upper < lower) {
        for (int i = 0; i < upper; i++) {
          for (int j = 0; j < md->get_MDES_num_resources(); j++) {
            ru_map[i][j].remove(op);
          }
        }
        upper = modulus;
      }
    }
  }
    
  for (int i = lower; i < upper; i++) {
    for (int j = 0; j < md->get_MDES_num_resources(); j++) {
      ru_map[i][j].remove(op);
    }
  }
}

void RU_mgr::at(int cycle, int res_index, List<void*>& ops)
{
  ops = ru_map[cycle][res_index];
}

void RU_mgr::print() { print_stream(stdout); }

void RU_mgr::print_stream(FILE *mout){
  int print_window=0, maxlen=0;
  char buff[4096];
  int i,j;
  int num_resources = md->get_MDES_num_resources();

  // print the header
  fprintf(mout,"resource table\n");
  // print the resource names vertically with 1 space
  for(i=0;i<num_resources;i++) {
    int len = strlen((md->get_MDES_res_descr(i))->get_name());
    maxlen = ELCOR_MAX(maxlen, len);
  }
  for(i=0;i<num_resources;i++) {
    char* name = (md->get_MDES_res_descr(i))->get_name();
    for (j=0;j<maxlen; j++) {
      buff[2*(num_resources*j+i)] = (*name != '\0') ? *name++ : ' ';
      buff[2*(num_resources*j+i)+1] = ' ';
    }
  }
  for (j=0; j<maxlen; j++) buff[2*num_resources*(j+1)-1] = '\0';
  for (j=0; j<maxlen; j++) fprintf(mout,"%s\n",&(buff[2*num_resources*j]));
  fprintf(mout,"\n");
  // print resource counts
  for(i=0; i<num_resources; i++) 
    fprintf(mout, "%-2d", md->get_MDES_res_descr(i)->get_count());
  fprintf(mout,"\n");
  for(i=0; i<num_resources; i++) fprintf(mout, "--");
  fprintf(mout,"\n");

  print_window = cur_sched_length();

  for(i=0;i<print_window;i++){
    for(j=0;j<num_resources; j++){
      int num = ru_map[i][j].size();
      if(num>0)
  fprintf(mout,"%-2d", num);
      else
  fprintf(mout,"  ");
    }
    fprintf(mout,"\n");
  }
  fflush(mout);
}

// find the length of the current schedule
int RU_mgr::cur_sched_length() {
  if (loop_mode) return modulus;
  else {
    int i,j;
    int num_resources = md->get_MDES_num_resources();
    for(i=ru_map.dim1()-1;i>=0;i--) {
      for(j=0;j<num_resources;j++) {
  if(!ru_map[i][j].is_empty())
    return i+1;
      }
    }
  }
  return 0;
}

bool RU_mgr::check_resources_free(Res_use_descr *res_use_in){
  Res_use_descr *current = res_use_in;
  while (current) {
    int res_index = current->get_res();
    Res_descr* resdesc = md->get_MDES_res_descr(res_index);
    int time_index = opsched_time + current->get_time();
    if (loop_mode) time_index = time_index%modulus;
    List<void*>& res_used = ru_map[time_index][res_index];
    int used_count = current->get_count()+res_used.size();
    if (used_count>resdesc->get_count()) return false;
    current = current->get_next();
  }
  return true;
}

bool RU_mgr::check_resources_free(Res_use_descr *res_use_in, int sched_time){
  Res_use_descr *current = res_use_in;
  while (current) {
    int res_index = current->get_res();
    Res_descr* resdesc = md->get_MDES_res_descr(res_index);
    int time_index = sched_time + current->get_time();
    if (loop_mode) time_index = time_index%modulus;
    List<void*>& res_used = ru_map[time_index][res_index];
    int used_count = current->get_count()+res_used.size();
    if (used_count>resdesc->get_count())
      return false;
    current = current->get_next();
  }
  return true;
}

void RU_mgr::collect_conflicting_ops(Res_use_descr *res_use_in, 
             Hash_set<void*>& users) {
  Res_use_descr *current = res_use_in;
  while (current) {
    int res_index = current->get_res();
    int time_index = opsched_time + current->get_time();
    if (loop_mode) time_index = time_index%modulus;
    for (List_iterator<void*> iter(ru_map[time_index][res_index]); 
      iter!=0; iter++) users += *iter;
    current = current->get_next();
  }
}

/////////////////////////////////////////////////////////////////////////////
// Rmsl

Rmsl::Rmsl(MDES* mdes, int lbound) 
  : md(mdes), cum_vect(mdes->get_MDES_num_resources(),0), lower_bound(lbound) {}

Rmsl::~Rmsl() {}

MDES* Rmsl::get_mdes() const { return md; }

void Rmsl::set_mdes(MDES* mdes) { 
  md = mdes; 
  cum_vect.resize(mdes->get_MDES_num_resources());
}

void Rmsl::init(int lbound){
  int i;
  lower_bound = lbound;
  // initialize cumulative time for this ResMII measurement
  for(i=0;i<(md->get_MDES_num_resources());i++) cum_vect[i]=0;
}

void Rmsl::nextop_io(char* opcode, char* io){
  Res_use_descr *res_use, *descr_head, *min_res_use;
  Io_descr request_iod(md);
  Io_list *iol_elem;
  Io_descr* iod_elem;
  Alt_descr* altd_elem;
  char* min_aname = NULL;
  int ival, ires, min_ival, i;
  bool found = false;

  // We tally the opcode and its resource usage into the cumulative
  // resource requirement Vector. The strategy is to select that
  // alternative that minimizes number of cycles required. However, if
  // the lower_bound is non-zero, we consume at least that many
  // resource-cycles before moving to the next alternative in priority.

  // parse operation and its io descr
  int num_resources = md->get_MDES_num_resources();
  Op_descr *operation = (md->get_MDES_hashtable())->find_op(opcode);
  if(!operation) El_punt("Rmsl::nextop_io: couldn't find operation %s", opcode);
  request_iod.parse_iospec(io); 

  min_ival=infinity;                      // initialize minimum resource count
  min_res_use=(Res_use_descr*) NULL;      // initial best resource
  Vector<int> total_vect(num_resources);  // scratch resource usage totals

  // change this to look at alternatives across different io descriptors.
  // otherwise, we are only finding single-cluster RMSL. -KF 8/2004

  while((iol_elem=operation->get_next_io())!=0) { 
    iod_elem=iol_elem->get_iod();         // iterate over io_desc of operation
    if(!strcmp(opcode, "CUSTOM") || iod_elem->test_io(&request_iod)){  // match the requested iod
      found = true;
      // We pick a "best" alternative (that minimally increases ResMII)
      iol_elem->init_alt(); 
      while((altd_elem=iol_elem->get_next_alt())!=0){
      
  // Calculate resource accumulation from this alternative
  descr_head=altd_elem->get_res_use(); 
  for(i=0;i<num_resources; i++) total_vect[i]=cum_vect[i];
  res_use=descr_head;
  while(res_use) { 
    ires = res_use->get_res();
    // increment count irrespective of time
    total_vect[ires] += res_use->get_count();
    res_use = res_use->get_next();
  } 

  // Calculate ResMII induced from this alt and save if minimum.
  // For counted resources, we take the ceiling of used_res/avail_res
  for (ival=0, i=0; i<num_resources; i++) {
    int cycles = ceiling(total_vect[i], md->get_MDES_res_descr(i)->get_count());
    if(cycles>ival) ival = cycles;
  }
  if(ival<min_ival) {
    min_ival=ival; 
    min_res_use=descr_head;
    min_aname = altd_elem->get_op()->get_aname_ptr();
  }
  // do not need to look at other alternatives until lower_bound is exceeded
  if (min_ival<=lower_bound) break;
      }
    }
  }

  if (!found)
    El_punt("MDES (Rmsl::nextop_io): No alternative of '%s' matches io format '%s'.",
            opcode, io);

  if (dbg(mdes,2)) 
    printf("RMSL: op '%s' : Choosing alt '%s' with ResMII=%d\n", 
           opcode, min_aname, min_ival);

  // Accumulate the best case pattern
  res_use=min_res_use; 
  while(res_use){ 
    ires = res_use->get_res();
    cum_vect[ires] += res_use->get_count();
    res_use = res_use->get_next();
  }
  if (dbg(mdes)) {
    printf("cum_vect:");
    for(i=0; i<num_resources; i++) 
      printf(" %d", cum_vect[i]);
    printf("\n");
  }
}

int Rmsl::value(){
  int i, max=0;
  // for counted resources, we take the ceiling of used_res/avail_res
  for(i=0;i<(md->get_MDES_num_resources());i++) {
    int cycles = ceiling(cum_vect[i], md->get_MDES_res_descr(i)->get_count());
    if(cycles>max) max = cycles;
  }
  return(max);
}

void Rmsl::print() { print_stream(stdout); }

void Rmsl::print_stream(FILE* mout) {
  int maxlen=0;
  char buff[4096];
  int i,j;
  int num_resources = md->get_MDES_num_resources();

  // print the header
  fprintf(mout,"resource table\n");
  // print the resource names vertically with 1 space
  for(i=0;i<num_resources;i++) {
    int len = strlen((md->get_MDES_res_descr(i))->get_name());
    maxlen = ELCOR_MAX(maxlen, len);
  }
  for(i=0;i<num_resources;i++) {
    char* name = (md->get_MDES_res_descr(i))->get_name();
    for (j=0;j<maxlen; j++) {
      buff[2*(num_resources*j+i)] = (*name != '\0') ? *name++ : ' ';
      buff[2*(num_resources*j+i)+1] = ' ';
    }
  }
  for (j=0; j<maxlen; j++) buff[2*num_resources*(j+1)-1] = '\0';
  for (j=0; j<maxlen; j++) fprintf(mout,"%s\n",&(buff[2*num_resources*j]));
  fprintf(mout,"\n");
  // print resource counts
  for(i=0; i<num_resources; i++) 
    fprintf(mout, "%-2d", md->get_MDES_res_descr(i)->get_count());
  fprintf(mout,"\n");
  for(i=0; i<num_resources; i++) fprintf(mout, "--");
  fprintf(mout,"\n");
  
  // print RMSL counts
  for(i=0; i<num_resources; i++)
    fprintf(mout, "%-2d", cum_vect[i]);
  fprintf(mout,"\n");
}  

StackDescr::StackDescr() 
{
  _dir = HIGH_TO_LOW;
  _ret_addr_thru_stack = false;
  _ret_addr_size = 8;
  _alignment = 8;
}

void StackDescr::setStackDir(StackDir dir)
{
  _dir = dir;
}

void StackDescr::setRetAddrThruStack()
{
  _ret_addr_thru_stack = true;
}

void StackDescr::setAlignment(unsigned int align)
{
  _alignment = align;
}

void StackDescr::setRetAddrSize(unsigned int size)
{
  _ret_addr_size = size;
}

StackDir StackDescr::getStackDir()
{
  return _dir;
}

bool StackDescr::isRetAddrThruStack()
{
  return _ret_addr_thru_stack;
}

unsigned int StackDescr::getRetAddrSize()
{
  return _ret_addr_size;
}

unsigned int StackDescr::getAlignment()
{
  return _alignment;
}

StackDir MDES::getStackDir()
{
  return stack_descr_tab->getStackDir();
}

bool MDES::isRetAddrThruStack()
{
  return stack_descr_tab->isRetAddrThruStack();
}

unsigned int MDES::getRetAddrSize()
{
  return stack_descr_tab->getRetAddrSize();
}

unsigned int MDES::getAlignment()
{
  return stack_descr_tab->getAlignment();
}

/////////////////////////////////////////////////////////////////////////////
// Utilities

static int mod_inc(int x,int modu) {
  int temp;
  temp=x+1;
  if (temp>=modu) temp=0;
  return(temp);
}

static int mod_dec(int x,int modu) {
  int temp;
  temp=x-1;
  if (x<0) temp=modu-1;
  return(temp);
}

static int ceiling(int x, int y) {
  int result = x / y;
  if (result*y<x) result++;
  return result;
}
  
OpLevel oplevel(char* level)
{
  if (level) {
    if (!strcmp(level, "UNITOP"))
      return UNITOP;
    else if (!strcmp(level, "CLSTROP"))
      return CLSTROP;
    else if (!strcmp(level, "GENERICOP"))
      return GENERICOP;
    else if (!strcmp(level, "MACROOP"))
      return MACROOP;
  }
  El_punt("MDES: Unknown operation level %s", level);
  return UNITOP;  // make CC happy
}

char* oplevel(OpLevel level)
{
  switch(level) {
  case UNITOP: return "UNITOP";
  case CLSTROP: return "CLSTROP";
  case GENERICOP: return "GENERICOP";
  case MACROOP: return "MACROOP";
  default: break;
  }

  El_punt("MDES: Unknown operation level '%d'", level);
  return NULL; // make CC happy
}

OpClass opclass(char* cls)
{
  if (cls) {
    if (!strcmp(cls, "NULLOP"))
      return NULLOP;
    else if (!strcmp(cls, "LOADOP"))
      return LOADOP;
    else if (!strcmp(cls, "STOREOP"))
      return STOREOP;
    else if (!strcmp(cls, "BRANCHOP"))
      return BRANCHOP;
    else if (!strcmp(cls, "ARITHOP"))
      return ARITHOP;
    else if (strcmp(cls, "CUSTOMOP"))
      return CUSTOMOP;
  }
  El_punt("MDES: Unknown operation class %s", cls);
  return NULLOP;  // make CC happy
}

char* opclass(OpClass cls)
{
  switch(cls) {
  case NULLOP: return "NULLOP";
  case LOADOP: return "LOADOP";
  case STOREOP: return "STOREOP";
  case BRANCHOP: return "BRANCHOP";
  case ARITHOP: return "ARITHOP";
  case CUSTOMOP: return "CUSTOMOP";

  default: break;
  }

  El_punt("MDES: Unknown operation class '%d'", cls);
  return NULL; // make CC happy
}

ResClass resclass(char* cls)
{
  if (cls) {
    if (!strcmp(cls, "NULL"))
      return RES_NULL;
    else if (!strcmp(cls, "INT"))
      return RES_INT;
    else if (!strcmp(cls, "FLOAT"))
      return RES_FLOAT;
    else if (!strcmp(cls, "MEMORY"))
      return RES_MEM;
    else if (!strcmp(cls, "BRANCH"))
      return RES_BRANCH;
    else if (!strcmp(cls, "LOCALMEMORY"))
      return RES_LMEM;
  }
  El_punt("MDES: Unknown resource class %s", cls);
  return RES_NULL;  // make CC happy
}

char* resclass(ResClass cls)
{
  switch(cls) {
  case RES_NULL: return "NULL";
  case RES_INT: return "INT";
  case RES_FLOAT: return "FLOAT";
  case RES_MEM: return "MEMORY";
  case RES_BRANCH: return "BRANCH";
  case RES_LMEM: return "LOCALMEMORY";
  default: break;
  }

  El_punt("MDES: Unknown resource class '%d'", cls);
  return NULL; // make CC happy
}

bool eagerclass(char* cls)
{
  if (cls) {
    if (!strcmp(cls, "NONSPECULATIVE"))
      return false;
    else if (!strcmp(cls, "SPECULATIVE"))
      return true;
  }
  El_punt("MDES: Unknown eager class %s", cls);
  return false;  // make CC happy
}

char* eagerclass(bool cls)
{
  if (cls) return "SPECULATIVE";
  else return "NONSPECULATIVE";
}



