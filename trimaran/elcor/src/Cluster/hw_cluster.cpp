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
//      File:           hw_cluster.cpp
//      Author:         Kevin Fan
//      Created:        July 2001
//      Description:    HW_cluster represents a hardware cluster
//
/////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include "hw_cluster.h"
#include "mdes.h"
#include "opcode_properties.h"
#include "iterators.h"

HW_cluster::HW_cluster()
   : supported_gen_opcodes(hash_opcode, HASH_SIZE_2),
     supported_us_opcodes(hash_estring, HASH_SIZE_4),
     supported_macros(hash_macro, HASH_SIZE_0),
     reg_files(hash_Reg_descr_ptr, HASH_SIZE_0),
     v_to_p_map(hash_reg_file, HASH_SIZE_0)
{

}

HW_cluster::HW_cluster(const HW_cluster& hwc)
   : supported_gen_opcodes(hwc.supported_gen_opcodes),
     supported_us_opcodes(hwc.supported_us_opcodes),
     supported_macros(hwc.supported_macros),
     reg_files(hwc.reg_files),
     v_to_p_map(hwc.v_to_p_map)
{}

HW_cluster::~HW_cluster() {}
 
bool HW_cluster::is_supported(Opcode opc)
{
   if (supported_gen_opcodes.is_member(opc))
      return true;
   return false;
}

 
bool HW_cluster::is_supported(eString opc)
{
   if (supported_us_opcodes.is_member(opc))
      return true;
   return false;
}

bool HW_cluster::is_supported(Macro_name macro)
{
   if (supported_macros.is_member(macro))
      return true;
   return false;
}

bool HW_cluster::is_supported(Op* op)
{
   if (is_pseudo(op))
      return true;     // pseudo ops on any cluster

   //added by hongtaoz 05/26/2002
   //hack for custom ops, assume custom ops supported on any cluster
   if(op->opcode() == CUSTOM)
	   return true;

   if (!is_supported(op->opcode()))
      return false;

   // check for compatible file types
   bool valid = true;
   for (Op_explicit_operands opri(op); opri != 0; opri++) {
      Operand& opr = *opri;
      if (opr.is_reg() || opr.is_macro_reg()) {
	 Reg_file vrf = opr.file_type();
	 if (get_physical_file(vrf) == NULL) {
	    valid = false;
	    break;
	 }
      }
   }
   return valid;
}

bool HW_cluster::contains_file(Reg_descr* file)
{
   return reg_files.is_member(file);
}

Hash_set<Reg_descr*>& HW_cluster::get_reg_descr_set()
{
   return reg_files;
}

char* HW_cluster::get_physical_file(Reg_file vrf)
{
   if (v_to_p_map.is_bound(vrf))
      return v_to_p_map.value(vrf)->get_aname();
   else
      return NULL;
}

int HW_cluster::get_physical_file_id(Reg_file vrf)
{
  if (v_to_p_map.is_bound(vrf))
    return v_to_p_map.value(vrf)->get_io_id();
  else {
    El_warn("get_physical_file_id(): invalid virtual type");
    return -1;
  }
}

void HW_cluster::add_supported_gen_opcode(Opcode opc)
{
   supported_gen_opcodes += opc;
}

// temporary hack used to remove BRL's from clusters w/o a RETURN_ADDR
// and BRF's from clusters w/o LC or ESC
void HW_cluster::remove_supported_gen_opcode(Opcode opcode)
{
  List<Opcode> to_remove;

  for(Hash_set_iterator<Opcode> hsi(supported_gen_opcodes); hsi!=0; hsi++) {
    Opcode opc = *hsi;
    if(opcode == BRL && is_brl(opc)) 
      to_remove.push(opc);    
    if(opcode == BRF_B_B_F && is_brf(opc))
      to_remove.push(opc);
  }
  
  for(List_iterator<Opcode> li(to_remove); li!=0; li++) {
    Opcode opc = *li;
    //      cdbg << "removing " << opc << endl;
    supported_gen_opcodes -= opc;
  }
}


void HW_cluster::add_supported_us_opcode(eString opc)
{
   supported_us_opcodes += opc;
}

int HW_cluster::get_num_files()
{
  return reg_files.size();
}

void HW_cluster::add_reg_file(Reg_descr* fname)
{
   reg_files += fname;

   // also add mapping to v_to_p_map
   Reg_file rf;
   char* vname = fname->get_vname();
   for (unsigned i = 0; i < strlen(vname); i++) {
      rf = mdesname_to_vregfile(vname[i]);
      if (! v_to_p_map.is_bound(rf))
	 v_to_p_map.bind(rf, fname);      
   }   
   Hash_set<eString> supported_macros_strings(hash_estring, 8);
   fname->get_macros(supported_macros_strings);
   for (Hash_set_iterator<eString> hsi(supported_macros_strings); hsi != 0; hsi++) {
     eString macro = *hsi;
     //     char* macro = *hsi;
     //     cdbg << "Macro: " << macro << " aname: " << fname->get_aname() << endl;
     supported_macros += el_string_to_macro_map.value(macro);

   }
}

bool HW_cluster::is_mergeable_with(const HW_cluster& hwc)
{
   for (Hash_map_iterator<Reg_file, Reg_descr*> hsi(v_to_p_map);
	hsi != 0; hsi++) {
      Reg_file rf = (*hsi).first;
      if (hwc.v_to_p_map.is_bound(rf))
	 return false;
   }
   return true;
}

void HW_cluster::merge_with(const HW_cluster& hwc)
{
   supported_gen_opcodes += hwc.supported_gen_opcodes;
   supported_us_opcodes += hwc.supported_us_opcodes;
   supported_macros += hwc.supported_macros;
   reg_files += hwc.reg_files;
   for (Hash_map_iterator<Reg_file, Reg_descr*> hsi(hwc.v_to_p_map);
	hsi != 0; hsi++) {
      Reg_file rf = (*hsi).first;
      if (! v_to_p_map.is_bound(rf))
	 v_to_p_map.bind(rf, (*hsi).second);
      else
	 El_punt("HW_cluster::merge_with: merging would result in more than one physical file per virtual type");
   }
}

Hash_set<Opcode>& HW_cluster::get_supported_gen_opcodes() 
{
  return(supported_gen_opcodes);
}

Hash_set<eString>& HW_cluster::get_supported_us_opcodes() 
{
  return(supported_us_opcodes);
}

ostream& operator<< (ostream& os, const HW_cluster& hwc)
{
   os << "files: ";
   for (Hash_set_iterator<Reg_descr*> hsi(hwc.reg_files); hsi!=0; hsi++)
      os << (*hsi)->get_aname() << " ";

   os << "\ngeneric opcodes: ";
   for (Hash_set_iterator<Opcode> opi(hwc.supported_gen_opcodes); opi!=0; opi++)
      os << el_opcode_to_string_map.value(*opi) << " ";
   os << endl;

   os << "\nunit specific opcodes: ";
   for (Hash_set_iterator<eString> opi2(hwc.supported_us_opcodes); opi2!=0; opi2++)
      os << (char*) *opi2 << " ";
   os << endl;

   os << "\nReg file map: ";
   for (Hash_map_iterator<Reg_file, Reg_descr*> hmi(hwc.v_to_p_map);
	hmi != 0; hmi++) {
      Pair<Reg_file, Reg_descr*> pair = *hmi;
      os << vregfile_to_mdesname(pair.first) << " => " << pair.second->get_aname() << "; ";
   }
   
   os << endl;
   return os;
}

unsigned hash_hw_cluster_ptr(HW_cluster*& h)
{
   return ELCOR_INT_HASH(h);
}

unsigned hash_reg_file(Reg_file& rf)
{
   return ELCOR_INT_HASH(rf);
}





