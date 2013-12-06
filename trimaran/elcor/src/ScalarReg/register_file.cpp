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

Copyright 2007 University of Michigan

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
//      File:           register_file.cpp
//      Authors:        Nate Clark (based on code by Hansoo Kim)
//      Created:        January 2005
//      Description:    
//
/////////////////////////////////////////////////////////////////////////////


#include "register_file.h"
#include "el_sreg_init.h"


Register_file_pool reg_file_pool;

// Macros don't have vr numbers, but many data structures use vr
// numbers as an index. This function is a mini-hack to allow macros
// to use those data structures.
int macro_to_vr(const Operand& mac) 
{
  static Hash_map<Macro_name, int> mac_to_vr(hash_macro, 32);
  static Hash_map<eString, int> pv_to_vr(hash_estring, 32);

  static int last_assigned_vr = 0;  
  if(current_MDES->is_pv_macro(mac.name())) {
    eString pv_name = el_macro_to_string_map.value(mac.name());
    pv_name |= mac.physical_file_type();
    if(!pv_to_vr.is_bound(pv_name))
      pv_to_vr.bind(pv_name, --last_assigned_vr);

    return pv_to_vr.value(pv_name);
  }

  if ( ! mac_to_vr.is_bound(mac.name())) {
    mac_to_vr.bind(mac.name(), --last_assigned_vr);
  }

  return mac_to_vr.value(mac.name());
}


Register_file::Register_file(const char* file_name_)
{
  init(file_name_);
}


void Register_file::remove_readonly(Reg_descr* rd,
				    Hash_set<eString>& real_caller_regs,
				    Hash_set<eString>& real_callee_regs) 
{
  Hash_set<eString>& ro = current_MDES->get_MDES_read_only_macros();

  Hash_set<eString> ro_thisfile(hash_estring, 8);
  rd->get_macros(ro_thisfile);
  ro_thisfile *= ro;

  Hash_set<eString> ro_physreg(hash_estring, 8);
  for (Hash_set_iterator<eString> hsi(ro_thisfile); hsi != 0; ++hsi)
    ro_physreg += rd->get_physical_regs(*hsi);

  real_caller_regs -= ro_physreg;
  real_callee_regs -= ro_physreg;
}


void Register_file::remove_reserved_macros(Reg_descr* rd,
					   Hash_set<eString>& real_caller_regs,
					   Hash_set<eString>& real_callee_regs)
{
  Hash_set<eString> macros(hash_estring, 8);
  rd->get_macros(macros);
  if(el_use_macros) {
    if (macros.is_member("SP_REG")) {
      Hash_set<eString>& sp_set = rd->get_physical_regs("SP_REG");
      if (sp_set.size() > 1)
	El_warn("More than one SP macro per file?  May cause incorrect behavior.");
      for (Hash_set_iterator<eString> sp_hsi(sp_set); sp_hsi != 0; ++sp_hsi) {
	real_caller_regs -= *sp_hsi;
	real_callee_regs -= *sp_hsi;
      }
    } 
    
    if (macros.is_member("RESERVE")) {   
      Hash_set<eString>& ra_set = rd->get_physical_regs("RESERVE");
      for (Hash_set_iterator<eString> ra_hsi(ra_set); ra_hsi != 0; ++ra_hsi) {
	real_caller_regs -= *ra_hsi;
	real_callee_regs -= *ra_hsi;
      }
    }

    // This is needed for code generation, even if you don't have a
    // multi-cluster machine.
    if (macros.is_member("ICMOVE_REG")) {
      Hash_set<eString>& ra_set = rd->get_physical_regs("ICMOVE_REG");
      for (Hash_set_iterator<eString> ra_hsi(ra_set); ra_hsi != 0; ++ra_hsi) {
	real_caller_regs -= *ra_hsi;
	real_callee_regs -= *ra_hsi;
      }
    }
  } else {
    // Remove all the macros. This is useful when debugging the allocator.
    for(Hash_set_iterator<eString> hsi(macros); hsi != 0; hsi++) {
      Hash_set<eString>& phys_set = rd->get_physical_regs(*hsi);
      for (Hash_set_iterator<eString> p_hsi(phys_set); p_hsi != 0; ++p_hsi) {
	real_caller_regs -= *p_hsi;
	real_callee_regs -= *p_hsi;
      }
    }
  }
}

void
Register_file::init(const char* reg_file_name)
{
  caller_bv.clear();
  callee_bv.clear();
  reg_use_count.clear();

  file_name = reg_file_name;
  int i = MDES_reg_index(file_name);
  Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);

  bit_width = MDES_reg_width((char*)file_name);
  num_regs = MDES_reg_static_size((char*)file_name);
  reg_use_count.resize(num_regs);
  caller_bv.resize(num_regs);
  callee_bv.resize(num_regs);

  Hash_set<eString> real_macros(hash_estring, 8);
  rd->get_macros(real_macros);

  Hash_set<eString> real_caller_regs = rd->get_caller_save();
  Hash_set<eString> real_callee_regs = rd->get_callee_save();

  remove_reserved_macros(rd, real_caller_regs, real_callee_regs);
  remove_readonly(rd, real_caller_regs, real_callee_regs);

  for (Hash_set_iterator<eString> hsi(real_caller_regs); hsi != 0; ++hsi) {
    caller_bv.set_bit(current_MDES->get_MDES_reg_number_for_pf(*hsi));
  }
  num_caller = caller_bv.ones_count();

  for (Hash_set_iterator<eString> hsi(real_callee_regs); hsi != 0; ++hsi) {
    callee_bv.set_bit(current_MDES->get_MDES_reg_number_for_pf(*hsi));
  }
  num_callee = callee_bv.ones_count();

  if(caller_bv.intersects(callee_bv)) {
    cdbg << "WARNING: It appears that some registers are both caller and "
	 << "callee save!\nThis is probably a bug in your MDES.\n\nCaller: "
	 << caller_bv << "\nCallee: " << callee_bv << endl << endl;
  }

  if (dbg(sa, 2)) {
    cdbg << "Register file info" << endl;
    cdbg << "callee: ";
    Vector<int> v;
    v.resize(callee_bv.ones_count());
    int v_ind = 0;
    for(unsigned i = 0; i < callee_bv.size(); i++) {
      if(callee_bv[i])
	v[v_ind++] = i;
    }

    // sort.
    if(v.size()) {
      for(unsigned i = 0; i < (v.size() - 1); i++) {
	for(unsigned j = i + 1; j < v.size(); j++) {
	  if(v[i] > v[j]) {
	    int tmp = v[i];
	    v[i] = v[j];
	    v[j] = tmp;
	  }
	}
      }
      cdbg << "\t={ ";
      for(unsigned i = 0; i < v.size(); i++) {
	cdbg << v[i];
	if(i != (v.size() - 1))
	  cdbg << ", ";
      }
      cdbg << " }" << endl;
    }
    cdbg << endl;


    cdbg << "caller: ";
    v.clear();
    v.resize(caller_bv.ones_count());
    v_ind = 0;
    for(unsigned i = 0; i < caller_bv.size(); i++) {
      if(caller_bv[i])
	v[v_ind++] = i;
    }

    // sort.
    if(v.size()) {
      for(unsigned i = 0; i < (v.size() - 1); i++) {
	for(unsigned j = i + 1; j < v.size(); j++) {
	  if(v[i] > v[j]) {
	    int tmp = v[i];
	    v[i] = v[j];
	    v[j] = tmp;
	  }
	}
      }
      cdbg << "\t={ ";
      for(unsigned i = 0; i < v.size(); i++) {
	cdbg << v[i];
	if(i != (v.size() - 1))
	  cdbg << ", ";
      }
      cdbg << " }" << endl;
    }
    cdbg << endl;
  } // end of dbg message
}


void Register_file::clear_reg_use_count()
{
  for (int i = 0; i < reg_use_count.dim(); ++i)
    reg_use_count[i] = 0;
}


bool Register_file::operator==(const Register_file& rb2) const
{
  if(file_name == rb2.file_name)
    return true;
  else
    return false;
}


bool Register_file::has_free_regs(const Bitvector& cant_use) const
{
  Bitvector tmp_regs = caller_bv;
  tmp_regs += callee_bv;
  tmp_regs -= cant_use;
  if(tmp_regs.first_one() == -1)
    return false;
  else
    return true;
}


// Returns alias Control Register for given predicate register.
int get_PV_num_for_reg(int pred_reg_num)
{
//   int i = -1;
//   while (pred_reg_num >= 0) {
//     pred_reg_num -= 32;
//     // 32 is the width of CR
//     // FIX: need MDES query to find register width given register file type
//     ++i;
//   };
//   return i;

  return (pred_reg_num >> 5);
}


Macro_name get_PV_macro(int num)
{
  switch (num) {
  case 0:
    return PV_0;
    break;
  case 1:
    return PV_1;
    break;
  case 2:
    return PV_2;
    break;
  case 3:
    return PV_3;
    break;
  case 4:
    return PV_4;
    break;
  case 5:
    return PV_5;
    break;
  case 6:
    return PV_6;
    break;
  case 7:
    return PV_7;
    break;
  default:
    // PV macros are used to store predicate registers 32 at a time to
    // make spill code more efficient. By default the compiler only
    // recognizes 8 PV macros. If you get this assert, then you have
    // too many predicates and need to either shrink that number, or
    // make the compiler (and simu) understand that you have more.
    El_punt("PV_num: not enough PV macros in the code");
  }
  return UNDEFINED;
}


void Register_file_pool::initialize()
{
  int num_phys_files = MDES_num_reg_types();
	
  the_files.resize(num_phys_files);
  for (int i = 0; i < num_phys_files; ++i) {
    char* cur_phys_file = MDES_reg_name(i);
    if (!MDES_reg_is_allocatable(cur_phys_file))
      continue;
    the_files[i].init(cur_phys_file);
  }
  clear_reg_use_counts();
}


Register_file& Register_file_pool::get_reg_file(char* phys_file_name) 
{
  return the_files[MDES_reg_index(phys_file_name)];
}


void Register_file_pool::clear_reg_use_counts() 
{
  for (int i = 0; i < the_files.dim(); ++i) {
    the_files[i].clear_reg_use_count();
  }
}
