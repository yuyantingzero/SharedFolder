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
//      File:           operand.cpp
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson,
//                      Santosh Abraham, Scott Mahlke, Dave August,
//                      Joel Jones, Greg Snider, Shail Aditya
//      Created:        December 1994
//      Description:    Operand class declaration 
//
/////////////////////////////////////////////////////////////////////////////

#include "operand.h"
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include "hash_map.h"
#include "hash_functions.h"
#include "string_class.h"
#include "intf.h"

/* Initialize virtual register number */
int virtual_register_number = 1;

void adjust_vr_num(int number) {
  if (virtual_register_number <= number) 
    virtual_register_number = number + 1;
}

Reg_file mdesname_to_vregfile(char vf)
{
   switch(vf) {
   case 'I' : return GPR;
   case 'F' : return FPR;
   case 'P' : return PR;
   case 'B' : return BTR;
   case 'C' : return CR;
   // parkhc 01/16/2007
   // support for vector register files
   case 'X': return VIR;
   case 'Y': return VFR;
   case 'Z': return VMR;
   //case 'W': return WPR;
   //case 'R': return PCBR;
   default : El_punt("mdesname_to_vregfile: no way to translate virtual file %c", vf);
   }
   return (Reg_file)0;
}

char* vregfile_to_mdesname(Reg_file vrf)
{
   switch (vrf) {
   case GPR: return "I";
   case FPR: return "F";
   case PR : return "P";
   case BTR: return "B";
   case CR : return "C";
   case VIR: return "X";
   case VFR: return "Y";
   case VMR: return "Z";
   //case WPR: return "W";
   //case PCBR:return "R"; 
   default : El_punt("vregfile_to_mdesname: no way to translate regfile %d", vrf);
   }
   return "";
}

char* operand_to_mdesname(Operand& o)
{
   if (o.is_reg() || o.is_macro_reg())
      return vregfile_to_mdesname(o.file_type());

   else if (o.is_predicate()) // predicate literals are different from other lits
      return vregfile_to_mdesname(PR);

   else if (o.is_lit())
      return "L";

   else if (o.is_undefined())
      return "U";

   else   // should never see this case.. but put in 
          // for compatiblity with append_io_pat
      return "U";
}

eString data_type_to_text (DType d)
{
   eString s;
   char temp[1024];

   if ( d.info() == EL_DT_INFO_INT && d.is_signed()==false )
   {
	s.cat("U");
   }
   switch(d.info()) {
	case EL_DT_INFO_INT: s.cat("I"); break;
        case EL_DT_INFO_FLOAT: s.cat("F"); break;
        case EL_DT_INFO_BRANCH: s.cat("BR"); break;
        case EL_DT_INFO_LOCAL_ABS: s.cat("L_ABS"); break;
        case EL_DT_INFO_LOCAL_GP: s.cat("L_GP"); break;
        case EL_DT_INFO_GLOBAL_ABS: s.cat("G_ABS"); break;
        case EL_DT_INFO_GLOBAL_GP: s.cat("G_GP"); break;
        case EL_DT_INFO_POINTER: s.cat("PTR"); break;
        case EL_DT_INFO_PREDICATE: s.cat("PRED"); break;
        case EL_DT_INFO_VEC_INT: s.cat("N"); break;
        case EL_DT_INFO_VEC_FLOAT: s.cat("L"); break;
        case EL_DT_INFO_VEC_MASK: s.cat("A"); break;
        //case EL_DT_INFO_WPREDICATE: s.cat("WPRED"); break;
        default: assert(0); 
   }
   sprintf(temp, "%d", d.width());
   s.cat(temp);
   return s;
}

Reg_file data_type_to_file (DType d)
{
   switch(d.info()) {
	case EL_DT_INFO_INT: return(GPR);
	case EL_DT_INFO_PREDICATE: return(PR);
	//case EL_DT_INFO_WPREDICATE: return(WPR);
        case EL_DT_INFO_FLOAT: return(FPR);
        case EL_DT_INFO_BRANCH: return(BTR);
        case EL_DT_INFO_LOCAL_ABS: return(GPR);
        case EL_DT_INFO_LOCAL_GP: return(GPR);
        case EL_DT_INFO_GLOBAL_ABS: return(GPR);
        case EL_DT_INFO_GLOBAL_GP: return(GPR);
        case EL_DT_INFO_POINTER: return(GPR);
        case EL_DT_INFO_VEC_INT: return(VIR);
        case EL_DT_INFO_VEC_FLOAT: return(VFR);
        case EL_DT_INFO_VEC_MASK: return(VMR);
        default: assert(0); return (GPR);
   }
}

////////////////////////////////////////////////////////////////////////////
//  Symboltable_entry class  :  An envelope for Base_operand for symbol table
////////////////////////////////////////////////////////////////////////////

Symboltable_entry::Symboltable_entry()
{}
Symboltable_entry::~Symboltable_entry() // Do not delete base_operand
{}

Symboltable_entry::Symboltable_entry(Base_operand * ptr)
   : operand_ptr(ptr)
{}

Symboltable_entry::Symboltable_entry(const Symboltable_entry& op)
   : operand_ptr(op.operand_ptr)
{}

Symboltable_entry& Symboltable_entry::operator=(const Symboltable_entry& op)
{
   operand_ptr = op.operand_ptr ;
   return (*this) ;
} 
bool Symboltable_entry::operator==(const Symboltable_entry& op) const
{
   return (operand_ptr->is_identical(*op.operand_ptr)) ;
}

bool Symboltable_entry::operator!=(const Symboltable_entry& op) const
{
   return (operand_ptr->operator!=(*op.operand_ptr)) ;
} 

bool Symboltable_entry::operator<(const Symboltable_entry& oprnd) const
{
   ostringstream ost1, ost2;
   ost1 << *this;
   ost2 << oprnd;   
   return (ost1.str() < ost2.str());
}



/* ------------ Base_operand Class */

Base_operand::Base_operand() {}
Base_operand::Base_operand(const Base_operand& bop) {}
Base_operand::~Base_operand() {}

Base_operand& Base_operand::operator=(const Base_operand& bop) 
{
  return *this;
}

bool Base_operand::operator!=(const Base_operand& operand) const{
   return !operator==(operand);
}

bool Base_operand::same_operand_class(const Base_operand& oprnd) const
{ assert(0) ; return false ;}

bool Base_operand::is_undefined() const {return false;}
bool Base_operand::is_reg() const {return false;}
bool Base_operand::is_vr_name() const {return false;}
bool Base_operand::is_mem_vr() const {return false;}
bool Base_operand::is_macro_reg() const {return false;}
bool Base_operand::is_lit() const {return false;}
bool Base_operand::is_int() const {return false;}
bool Base_operand::is_float() const {return false;}
bool Base_operand::is_double() const {return false;}
bool Base_operand::is_string() const {return false;}
bool Base_operand::is_label() const {return false;}
bool Base_operand::is_predicate() const {return false;}
bool Base_operand::is_predicate_true() const {return false;}
bool Base_operand::is_predicate_false() const {return false;}
bool Base_operand::is_cb() const {return false;}
bool Base_operand::assigned_to_file() const {return true;} 
bool Base_operand::allocated() const {return true;} 

int  Base_operand::omega() const { return (0); }
void  Base_operand::set_omega(int) { assert(0); }

DType Base_operand::dtype() const { assert(0); return(EL_STANDARD_INT); }
void Base_operand::set_dtype(DType t) {assert(0); }

// Override for Reg, Macro, VR_name and Undefined
Reg_file Base_operand::file_type() const { assert(0); return(GPR); }

// override for Reg, Int_lit, Float_lit, Double_lit
eString Base_operand::physical_file_type() const {assert(0);return(eString)"";}
void Base_operand::bind_physical_file(eString phys_file) { assert(0); }
void Base_operand::unbind_physical_file() { assert(0); }

void Base_operand::print_for_pqs(ostream& os) const
{
  print(os);
}

ostream& operator<<(ostream& os, const Base_operand& operand){
  operand.print(os);
  return os;
}

/* -------------  Reg  class */
Reg::Reg(DType dt)
  :dtype_rep(dt), file(data_type_to_file(dt)), physical_file("")
{
   vr_rep = virtual_register_number++;
   omega_num = 0;
   history = vr_rep;
   assigned = true;
   alloc = false;
   sr = STATIC_REG ;
   mc_rep = 0 ;
}

Reg::Reg(DType dt, int vr_number, int omega)
  :dtype_rep(dt), file(data_type_to_file(dt)), physical_file(""),
   vr_rep(vr_number), omega_num(omega)
{
   history = vr_rep;
   assigned = true;
   alloc = false;
   sr = STATIC_REG ;
   mc_rep = 0 ;
   ::adjust_vr_num(vr_rep);
}

Reg::Reg(DType dt, Reg_file r_file, int vr_number)
   :dtype_rep(dt), file(r_file), physical_file(""), vr_rep(vr_number)
{
   omega_num = 0 ;
   history = vr_rep;
   assigned = true;
   alloc = false;
   sr = STATIC_REG ;
   mc_rep = 0 ;
   ::adjust_vr_num(vr_rep);
}
   

Reg::Reg(const Reg& reg)
  :dtype_rep(reg.dtype_rep), file(reg.file), 
   physical_file(reg.physical_file),
   vr_rep(reg.vr_rep), omega_num(reg.omega_num), 
   history(reg.history), assigned(reg.assigned), 
   alloc(reg.alloc), sr(reg.sr), mc_rep(reg.mc_rep)
{}

Reg::Reg(const Operand& r)
{
  assert (r.is_reg() || r.is_vr_name());

  dtype_rep = r.dtype();
  file = data_type_to_file(r.dtype());
  vr_rep = r.vr_num();
  if (r.is_reg()) {
     Reg& reg = *((Reg*)((Operand&)r).get_ptr()) ;
     omega_num = reg.omega_num;
     physical_file = reg.physical_file;
     history = reg.history;
     assigned = reg.assigned;
     alloc = reg.alloc;
     sr = reg.sr ;
     mc_rep = reg.mc_rep ;
 }
  else {
     omega_num = 0 ;
     physical_file = r.physical_file_type();
     history = vr_rep ;
     assigned = true ;
     alloc = false ;
     sr = STATIC_REG ;
     mc_rep = 0 ;
  }
}

Base_operand* Reg::clone() const{
   return (new Reg(*this));
}

Reg::~Reg() 
{}

Reg& Reg::operator=(const Reg& reg){
  dtype_rep = reg.dtype_rep;
  vr_rep = reg.vr_rep;
  omega_num = reg.omega_num;
  history = reg.history;
  assigned = reg.assigned;
  alloc = reg.alloc;
  file = reg.file;
  physical_file = reg.physical_file;
  sr = reg.sr;
  mc_rep = reg.mc_rep;
  return *this;
}

bool Reg::operator==(const Base_operand& operand) const {
   if (operand.is_reg()) {
      Reg& temp = (Reg&)operand;
      if (alloc && temp.alloc) {
         return (equal_mc(operand)) ;
      }
      else if (assigned_to_physical_file() && temp.assigned_to_physical_file()) {
	 return (equal_filevr(operand));
      }
      else {
         return (equal_vr(operand));
      }
   }
   else if (operand.is_macro_reg())
      // call Macro_reg::operator==()
      return (((Macro_reg&)operand) == *this);
   else {
      return false ;
   }
}

bool Reg::is_identical(const Base_operand& operand) const {
   if (operand.is_reg()) {
      Reg& r = (Reg&)operand ;
      if ((dtype_rep == r.dtype_rep) && (file == r.file) && 
	  (physical_file == r.physical_file) && 
	  (vr_rep == r.vr_rep) && (omega_num == r.omega_num) && 
	  (history == r.history) && (assigned == r.assigned) && 
	  (alloc == r.alloc) && (sr == r.sr) && 
	  (mc_rep == r.mc_rep)) {
	 return true ;
      }
   }
   return false ;
}

bool Reg::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_reg()) ;
}

bool Reg::is_reg() const {return true;}

bool Reg::assigned_to_file() const {
  return (assigned ? true : false);
}

bool Reg::allocated() const {
  return (alloc ? true : false);
}

bool Reg::equal_vr(const Base_operand& operand) const {
  if (operand.is_reg()) {
    Reg& temp = (Reg&)operand;
    if ((dtype_rep == temp.dtype_rep) && (vr_rep == temp.vr_rep) &&
	(omega_num == temp.omega_num) && (file == temp.file))
      return true;
    else return false;
  }
  else return false;
}

bool Reg::equal_filevr(const Base_operand& operand) const {
  if (operand.is_reg()) {
    Reg& temp = (Reg&)operand;
    if ((dtype_rep == temp.dtype_rep) && (vr_rep == temp.vr_rep) &&
	(omega_num == temp.omega_num) && (physical_file == temp.physical_file))
      return true;
    else return false;
  }
  else return false;
}
	
bool Reg::equal_mc(const Base_operand& operand) const {
  if (operand.is_reg()) {
    Reg& temp = (Reg&)operand;
    if (alloc && temp.alloc)
       //      if ((file == temp.file) && (sr == temp.sr) && (mc_rep == temp.mc_rep))
       // check for same physical file to distinguish btwn gpr1, gpr2
      if ((physical_file == temp.physical_file) && (sr == temp.sr) && (mc_rep == temp.mc_rep))
	return true;
      else return false;
    else return false;
  }
  else return false;
}

void Reg::set_dtype(DType t) { dtype_rep = t ; } 
DType Reg::dtype() const {return dtype_rep;}
int Reg::vr_num() const {return vr_rep;}
int Reg::omega() const {return omega_num;}
Reg_file Reg::file_type() const {return file;}

bool Reg::is_static() const {
  if (assigned && sr == STATIC_REG) return true;
  else return false;
}

bool Reg::is_rotating() const {
  if (assigned && sr == ROTATING) return true;
  else return false;
}

int Reg::mc_num() const {return mc_rep;}

Reg& Reg::rename() {
  vr_rep = virtual_register_number++;
  return *this;
}

Reg& Reg::rename(int new_vr_rep) {
  vr_rep = new_vr_rep;
  return *this;
}

Reg& Reg::incr_omega(int incr) {
  omega_num += incr;
  return *this;
}

Reg& Reg::bind_file(Reg_file reg_file, Reg_sr s_or_r) {
  assigned = true;
  file = reg_file;
  sr = s_or_r;
  return *this;
}

Reg& Reg::bind_reg(int mc_number) {
  if (!assigned) assert(0);
  alloc = true;
  mc_rep = mc_number;
  return *this;
}

Reg& Reg::unbind_reg() {
  alloc = false;
  return *this;
}

void Reg::set_vr_num(int new_val)
{
   vr_rep = new_val;
}

void Reg::set_omega(int new_val)
{
   omega_num = new_val;
}

void Reg::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void Reg::unbind_physical_file()
{
  physical_file = "";
}

eString Reg::physical_file_type() const
{
  return physical_file;
}

bool Reg::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

// This is used for symbol table hashing and has nothing to do with equality
int Reg::hash() const
{
  // return (vr_rep + omega_num) ; Original code, changed SAM 1-98

  if (alloc) {
    return (mc_rep); 
  }
  else {
    return (ELCOR_INT_ABS(vr_rep + omega_num));
  }
}

void Reg::print(ostream& os) const {
    if (alloc) {
      os << physical_file << " ";
      // Print the physical file name to distinguish between gpr1 gpr2
      //os << vregfile_to_mdesname(file);
      if (sr == STATIC_REG) os << "r" << mc_rep;
      else if (sr == ROTATING) os << "vr" << vr_rep << " " << "[r" << mc_rep << "]";
      else assert(0);
    }
    else {
      os << "vr" << vr_rep;
      if (omega_num != 0) os << "[" << omega_num << "]";
      if (assigned) {
        os << ":" << vregfile_to_mdesname(file);
        if (sr == STATIC_REG) os << ".s";
        else if (sr == ROTATING) os << ".r";
      }
      os << ":" << data_type_to_text(dtype_rep);
      if(physical_file != "")
	os << " " << physical_file;
    }
}

void Reg::print_for_pqs(ostream& os) const {
  os << "vr" << vr_rep;
  if (omega_num != 0) os << "[" << omega_num << "]";
  if (assigned) {
    os << ":" << vregfile_to_mdesname(file);
    if (sr == STATIC_REG) os << ".s";
    else if (sr == ROTATING) os << ".r";
  }
  os << ":" << data_type_to_text(dtype_rep);
}


////////////////////////////////////////////////////////////////////////////
//
//  VR_name 
//
////////////////////////////////////////////////////////////////////////////

VR_name::VR_name(const Reg evr)
  : dtype_rep(evr.dtype()), vr_rep(evr.vr_num()), 
    physical_file(evr.physical_file_type())
{}

VR_name::VR_name(const Operand opr)
{
  assert(opr.is_reg());
  dtype_rep = opr.dtype();
  vr_rep = opr.vr_num();
  physical_file = opr.physical_file_type();
}

VR_name::VR_name(DType dt, int vr_number)
  :dtype_rep(dt), vr_rep(vr_number)
{
  physical_file = "";
}

Base_operand* VR_name::clone() const {
   return (new VR_name(*this));
}

VR_name::~VR_name() {}

bool VR_name::operator==(const Base_operand& bo) const {
   if (bo.is_vr_name()) {
      VR_name& tmp_bo = (VR_name&) bo ;
      if (vr_rep == tmp_bo.vr_rep) {
	 return true ;
      }
      else {
	 return false ;
      }
   }
   else {
      return false ;
   }
}

bool VR_name::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_vr_name()) ;
}

bool VR_name::is_identical(const Base_operand& operand) const {
   if (operand.is_vr_name()) {
      VR_name& r = (VR_name&)operand ;
      if ((dtype_rep == r.dtype_rep) && (vr_rep == r.vr_rep) && 
	  (physical_file == r.physical_file)) {
	 return true ;
      }
   }
   return false ;
}

bool VR_name::is_vr_name() const { return true ; }

bool VR_name::same_name(const Base_operand& bo) const
{
   if (bo.is_reg()) {
      Reg& tmp_bo = (Reg&) bo ;
      if (vr_rep == tmp_bo.vr_num()) {
	 return true ;
      }
      else {
	 return false ;
      }
      
   }
   else if (bo.is_vr_name()){
      return (operator==(bo)) ;
   }
   else {
      return false ;
   }
}

DType VR_name::dtype() const { return dtype_rep ;}

void VR_name::set_dtype(DType t) {dtype_rep = t; }

int VR_name::vr_num() const {return vr_rep ; }

VR_name& VR_name::rename(const Reg& evr)
{
   vr_rep = evr.vr_num() ;
   return (*this) ;
}

void VR_name::set_vr_num(int new_val)
{
   vr_rep = new_val;
}

int VR_name::hash() const { return (vr_rep) ;}

void VR_name::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void VR_name::unbind_physical_file()
{
  physical_file = "";
}

eString VR_name::physical_file_type() const
{
  return physical_file;
}

bool VR_name::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

void VR_name::print(ostream& os) const {
  os << "EVR_name" << vr_rep;
}

////////////////////////////////////////////////////////////////////////////


/* -------------  Mem_vr  class */

Mem_vr::Mem_vr(){
  vr_rep = virtual_register_number++;
  history = vr_rep;
  omega_num = 0;
}
  
Mem_vr::Mem_vr(int vr_number, int omega)
  :vr_rep(vr_number), omega_num(omega)
{
  history = vr_rep;
  ::adjust_vr_num(vr_rep);
}

Mem_vr::Mem_vr(const Mem_vr& reg)
  :vr_rep(reg.vr_rep), omega_num(reg.omega_num), history(reg.history) {}

Base_operand* Mem_vr::clone() const{
  return (new Mem_vr(*this));
}

Mem_vr::~Mem_vr() {}

Mem_vr& Mem_vr::operator=(const Mem_vr& reg){
  vr_rep = reg.vr_rep;
  history = reg.history;
  omega_num = reg.omega_num;
  return *this;
}

bool Mem_vr::operator==(const Base_operand& operand) const {
  if (operand.is_mem_vr())
    return ((vr_rep == ((Mem_vr&)operand).vr_rep) &&
	    (omega_num == ((Mem_vr&)operand).omega_num));
  else return false;
}

bool Mem_vr::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_mem_vr()) ;
}

bool Mem_vr::is_identical(const Base_operand& operand) const {
   if (operand.is_mem_vr()) {
      Mem_vr& r = (Mem_vr&)operand ;
      if ((vr_rep == r.vr_rep) && (omega_num == r.omega_num) &&
	  (history == r.history)) {
	 return true ;
      }
   }
   return false ;
}

bool Mem_vr::is_mem_vr() const {return true;}

int Mem_vr::vr_num() const {return vr_rep;}
int Mem_vr::omega() const {return omega_num;}

Mem_vr& Mem_vr::rename() {
  vr_rep = virtual_register_number++;
  return *this;
}

Mem_vr& Mem_vr::rename(int newnum) {
  vr_rep = newnum;
  return *this;
}

Mem_vr& Mem_vr::incr_omega(int incr) {
  omega_num += incr;
  return *this;
}

void Mem_vr::set_vr_num(int new_val)
{
   vr_rep = new_val;
}

void Mem_vr::set_omega(int new_val)
{
   omega_num = new_val;
}

int Mem_vr::hash() const { return (vr_rep + omega_num) ;} 

void Mem_vr::print(ostream& os) const {
  os << "mvr" << vr_rep;
  if (omega_num != 0) os << "[" << omega_num << "]";
}
  
/* ------------- Macro registers */
Hash_map<Macro_name, eString> el_macro_to_string_map (hash_macro, 64);
Hash_map<eString, Macro_name> el_string_to_macro_map (hash_estring, 64);

void el_init_elcor_macro_maps() 
{
  el_macro_to_string_map.bind(LOCAL, "LOCAL");
  el_string_to_macro_map.bind("LOCAL", LOCAL);
  el_macro_to_string_map.bind(PARAM, "PARAM");
  el_string_to_macro_map.bind("PARAM", PARAM);
  el_macro_to_string_map.bind(SWAP, "SWAP");
  el_string_to_macro_map.bind("SWAP", SWAP);
  el_macro_to_string_map.bind(INT_RETURN_TYPE, "INT_RETURN_TYPE");
  el_string_to_macro_map.bind("INT_RETURN_TYPE", INT_RETURN_TYPE);
  el_macro_to_string_map.bind(FLT_RETURN_TYPE, "FLT_RETURN_TYPE");
  el_string_to_macro_map.bind("FLT_RETURN_TYPE", FLT_RETURN_TYPE);
  el_macro_to_string_map.bind(DBL_RETURN_TYPE, "DBL_RETURN_TYPE");
  el_string_to_macro_map.bind("DBL_RETURN_TYPE", DBL_RETURN_TYPE);
  el_macro_to_string_map.bind(INT_RETURN, "INT_RETURN");
  el_string_to_macro_map.bind("INT_RETURN", INT_RETURN);
  // many of these?
  el_macro_to_string_map.bind(INT_PARAM_1, "INT_PARAM_1");
  el_string_to_macro_map.bind("INT_PARAM_1", INT_PARAM_1);
  el_macro_to_string_map.bind(INT_PARAM_2, "INT_PARAM_2");
  el_string_to_macro_map.bind("INT_PARAM_2", INT_PARAM_2);
  el_macro_to_string_map.bind(INT_PARAM_3, "INT_PARAM_3");
  el_string_to_macro_map.bind("INT_PARAM_3", INT_PARAM_3);
  el_macro_to_string_map.bind(INT_PARAM_4, "INT_PARAM_4");
  el_string_to_macro_map.bind("INT_PARAM_4", INT_PARAM_4);
  el_macro_to_string_map.bind(FLT_RETURN, "FLT_RETURN");
  el_string_to_macro_map.bind("FLT_RETURN", FLT_RETURN);
  // many of these?
  el_macro_to_string_map.bind(FLT_PARAM_1, "FLT_PARAM_1");
  el_string_to_macro_map.bind("FLT_PARAM_1", FLT_PARAM_1);
  el_macro_to_string_map.bind(FLT_PARAM_2, "FLT_PARAM_2");
  el_string_to_macro_map.bind("FLT_PARAM_2", FLT_PARAM_2);
  el_macro_to_string_map.bind(FLT_PARAM_3, "FLT_PARAM_3");
  el_string_to_macro_map.bind("FLT_PARAM_3", FLT_PARAM_3);
  el_macro_to_string_map.bind(FLT_PARAM_4, "FLT_PARAM_4");
  el_string_to_macro_map.bind("FLT_PARAM_4", FLT_PARAM_4);
  el_macro_to_string_map.bind(DBL_RETURN, "DBL_RETURN");
  el_string_to_macro_map.bind("DBL_RETURN", DBL_RETURN);
  // many of these?
  el_macro_to_string_map.bind(DBL_PARAM_1, "DBL_PARAM_1");
  el_string_to_macro_map.bind("DBL_PARAM_1", DBL_PARAM_1);
  el_macro_to_string_map.bind(DBL_PARAM_2, "DBL_PARAM_2");
  el_string_to_macro_map.bind("DBL_PARAM_2", DBL_PARAM_2);
  el_macro_to_string_map.bind(DBL_PARAM_3, "DBL_PARAM_3");
  el_string_to_macro_map.bind("DBL_PARAM_3", DBL_PARAM_3);
  el_macro_to_string_map.bind(DBL_PARAM_4, "DBL_PARAM_4");
  el_string_to_macro_map.bind("DBL_PARAM_4", DBL_PARAM_4);
  el_macro_to_string_map.bind(INT_TM_TYPE, "INT_TM_TYPE");
  el_string_to_macro_map.bind("INT_TM_TYPE", INT_TM_TYPE);
  el_macro_to_string_map.bind(FLT_TM_TYPE, "FLT_TM_TYPE");
  el_string_to_macro_map.bind("FLT_TM_TYPE", FLT_TM_TYPE);
  el_macro_to_string_map.bind(DBL_TM_TYPE, "DBL_TM_TYPE");
  el_string_to_macro_map.bind("DBL_TM_TYPE", DBL_TM_TYPE);
  el_macro_to_string_map.bind(SP_REG, "SP_REG");
  el_string_to_macro_map.bind("SP_REG", SP_REG);
  el_macro_to_string_map.bind(FP_REG, "FP_REG");
  el_string_to_macro_map.bind("FP_REG", FP_REG);
  el_macro_to_string_map.bind(IP_REG, "IP_REG");
  el_string_to_macro_map.bind("IP_REG", IP_REG);
  el_macro_to_string_map.bind(OP_REG, "OP_REG");
  el_string_to_macro_map.bind("OP_REG", OP_REG);
  el_macro_to_string_map.bind(LV_REG, "LV_REG");
  el_string_to_macro_map.bind("LV_REG", LV_REG);
  el_macro_to_string_map.bind(RGS_REG, "RGS_REG");
  el_string_to_macro_map.bind("RGS_REG", RGS_REG);
  el_macro_to_string_map.bind(LC_REG, "LC_REG");
  el_string_to_macro_map.bind("LC_REG", LC_REG);
  el_macro_to_string_map.bind(ESC_REG, "ESC_REG");
  el_string_to_macro_map.bind("ESC_REG", ESC_REG);
  el_macro_to_string_map.bind(ALL_PRED, "ALL_PRED");
  el_string_to_macro_map.bind("ALL_PRED", ALL_PRED);
  el_macro_to_string_map.bind(ALL_ROT_PRED, "ALL_ROT_PRED");
  el_string_to_macro_map.bind("ALL_ROT_PRED", ALL_ROT_PRED);
  el_macro_to_string_map.bind(ALL_STATIC_PRED, "ALL_STATIC_PRED");
  el_string_to_macro_map.bind("ALL_STATIC_PRED", ALL_STATIC_PRED);
  el_macro_to_string_map.bind(RRB, "RRB");
  el_string_to_macro_map.bind("RRB", RRB);
  el_macro_to_string_map.bind(RETURN_ADDR, "RETURN_ADDR");
  el_string_to_macro_map.bind("RETURN_ADDR", RETURN_ADDR);
  el_macro_to_string_map.bind(FLT_ZERO, "FLT_ZERO");
  el_string_to_macro_map.bind("FLT_ZERO", FLT_ZERO);
  el_macro_to_string_map.bind(FLT_ONE, "FLT_ONE");
  el_string_to_macro_map.bind("FLT_ONE", FLT_ONE);
  el_macro_to_string_map.bind(DBL_ZERO, "DBL_ZERO");
  el_string_to_macro_map.bind("DBL_ZERO", DBL_ZERO);
  el_macro_to_string_map.bind(DBL_ONE, "DBL_ONE");
  el_string_to_macro_map.bind("DBL_ONE", DBL_ONE);
  el_macro_to_string_map.bind(INT_ZERO, "INT_ZERO");
  el_string_to_macro_map.bind("INT_ZERO", INT_ZERO);
  el_macro_to_string_map.bind(PRED_FALSE, "PRED_FALSE");
  el_string_to_macro_map.bind("PRED_FALSE", PRED_FALSE);
  el_macro_to_string_map.bind(PRED_TRUE, "PRED_TRUE");
  el_string_to_macro_map.bind("PRED_TRUE", PRED_TRUE);
  el_macro_to_string_map.bind(VEC_MASK_TRUE, "VEC_MASK_TRUE");
  el_string_to_macro_map.bind("VEC_MASK_TRUE", VEC_MASK_TRUE);
  el_macro_to_string_map.bind(VEC_SCRATCH_PAD, "VEC_SCRATCH_PAD");
  el_string_to_macro_map.bind("VEC_SCRATCH_PAD", VEC_SCRATCH_PAD);
  el_macro_to_string_map.bind(SPILL_TEMPREG, "SPILL_TEMPREG");
  el_string_to_macro_map.bind("SPILL_TEMPREG", SPILL_TEMPREG);
  el_macro_to_string_map.bind(ICMOVE_REG, "ICMOVE_REG");
  el_string_to_macro_map.bind("ICMOVE_REG", ICMOVE_REG);
  el_macro_to_string_map.bind(PV_0, "PV_0");
  el_string_to_macro_map.bind("PV_0", PV_0);
  el_macro_to_string_map.bind(PV_1, "PV_1");
  el_string_to_macro_map.bind("PV_1", PV_1);
  el_macro_to_string_map.bind(PV_2, "PV_2");
  el_string_to_macro_map.bind("PV_2", PV_2);
  el_macro_to_string_map.bind(PV_3, "PV_3");
  el_string_to_macro_map.bind("PV_3", PV_3);
  el_macro_to_string_map.bind(PV_4, "PV_4");
  el_string_to_macro_map.bind("PV_4", PV_4);
  el_macro_to_string_map.bind(PV_5, "PV_5");
  el_string_to_macro_map.bind("PV_5", PV_5);
  el_macro_to_string_map.bind(PV_6, "PV_6");
  el_string_to_macro_map.bind("PV_6", PV_6);
  el_macro_to_string_map.bind(PV_7, "PV_7");
  el_string_to_macro_map.bind("PV_7", PV_7);
  //support distributed instruction fetch
  el_macro_to_string_map.bind(EBR_BUS, "EBR_BUS");
  el_string_to_macro_map.bind("EBR_BUS", EBR_BUS);
  el_macro_to_string_map.bind(RESERVE, "RESERVE");
  el_string_to_macro_map.bind("RESERVE", RESERVE);
  el_macro_to_string_map.bind(UNDEFINED, "UNDEFINED");
  el_string_to_macro_map.bind("UNDEFINED", UNDEFINED);
  
}

unsigned hash_macro(Macro_name& i)
{
   return(ELCOR_INT_HASH(i)) ;
}


void el_deinit_elcor_macro_maps()
{
   el_macro_to_string_map.clear();
   el_string_to_macro_map.clear();
}

Macro_reg::Macro_reg()
{
  name_rep = UNDEFINED;
  alloc = false;
  mc_rep = 0;
}

Macro_reg::Macro_reg(const Macro_reg& mr)
   :dtype_rep(mr.dtype_rep), file(mr.file), physical_file(mr.physical_file),
    name_rep(mr.name_rep), alloc(mr.alloc), mc_rep(mr.mc_rep) {}

Macro_reg::Macro_reg(Macro_name new_name)
  : physical_file("")
{
  name_rep = new_name;
  alloc = false;
  mc_rep = 0;

  switch(name_rep)
    {
    case UNDEFINED:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case LOCAL:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case PARAM:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case SWAP:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case INT_RETURN_TYPE:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_RETURN_TYPE:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case DBL_RETURN_TYPE:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case INT_RETURN:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case INT_PARAM_1:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case INT_PARAM_2:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case INT_PARAM_3:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case INT_PARAM_4:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_RETURN:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_PARAM_1:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_PARAM_2:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_PARAM_3:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_PARAM_4:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case DBL_RETURN:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case DBL_PARAM_1:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case DBL_PARAM_2:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case DBL_PARAM_3:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case DBL_PARAM_4:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case INT_TM_TYPE:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_TM_TYPE:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case DBL_TM_TYPE:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case SP_REG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FP_REG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case IP_REG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case OP_REG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case LV_REG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case RGS_REG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case LC_REG:
      dtype_rep = EL_STANDARD_INT;
      file = CR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case ESC_REG:
      dtype_rep = EL_STANDARD_INT;
      file = CR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case ALL_PRED:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case ALL_ROT_PRED:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case ALL_STATIC_PRED:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case RRB:
      dtype_rep = EL_STANDARD_INT;
      file = CR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case RETURN_ADDR:
      dtype_rep = EL_STANDARD_BRANCH;
      file = BTR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case FLT_ZERO:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = false;
      hardwired_rep = true;
      break;
    case FLT_ONE:
      dtype_rep = EL_STANDARD_FLOAT;
      file = FPR;
      global_rep = false;
      hardwired_rep = true;
      break;
    case DBL_ZERO:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = false;
      hardwired_rep = true;
      break;
    case DBL_ONE:
      dtype_rep = EL_STANDARD_DOUBLE;
      file = FPR;
      global_rep = false;
      hardwired_rep = true;
      break;
    case INT_ZERO:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = false;
      hardwired_rep = true;
      break;
    case PRED_FALSE:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = true;
      break;
    case PRED_TRUE:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = true;
      break;
    /* RMR { */
    /* default all true vector mask */
    case VEC_MASK_TRUE:
	dtype_rep = EL_STANDARD_VEC_MASK;
	file = VMR;
        global_rep = false;
        hardwired_rep = true;
	break;
    /* temp scratch pad memory location */
    case VEC_SCRATCH_PAD:
        dtype_rep = EL_STANDARD_INT;
	file = GPR;
        global_rep = true;
        hardwired_rep = false;
	break;
    /* } RMR */
    case SPILL_TEMPREG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case ICMOVE_REG:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_0:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_1:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_2:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_3:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_4:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_5:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_6:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case PV_7:
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = false;
      hardwired_rep = false;
      break;
    case EBR_BUS:	//hongtaoz: support for distributed instruction fetch
      dtype_rep = EL_STANDARD_PREDICATE;
      file = PR;
      global_rep = true;
      hardwired_rep = false;
      break;
    case RESERVE:
      dtype_rep = EL_STANDARD_INT;
      file = GPR;
      global_rep = false;
      hardwired_rep = false;
      break;

    default:
	El_punt("Macro_reg::Macro_reg(): unknown macro");
    }
}

Base_operand* Macro_reg::clone() const 
{
   Base_operand* tmp = new Macro_reg(*this);
   assert(tmp && "out of memory");
   return (tmp);
}
   
Macro_reg::~Macro_reg() {} 

Macro_reg& Macro_reg::operator=(const Macro_reg& mr) 
{
   name_rep = mr.name_rep ;
   dtype_rep = mr.dtype_rep ;
   file = mr.file ;
   physical_file = mr.physical_file;
   alloc = mr.alloc;
   mc_rep = mr.mc_rep;
   return(*this) ;
}

bool Macro_reg::operator==(const Base_operand& operand) const
{
   if (operand.is_macro_reg()) {
      Macro_reg& temp = (Macro_reg&)operand;
      if (assigned_to_physical_file() && temp.assigned_to_physical_file()) {
         if (allocated() && temp.allocated()) {
            return (physical_file == temp.physical_file &&
                    mc_rep == temp.mc_rep);
         } else {
	   if(physical_file == temp.physical_file) {
	     if(name_rep == temp.name_rep) {
	       return true;
	     } else {
	       // need to check the macro aliases
	       Hash_set<eString>& aliases = current_MDES->get_MDES_alias_macros();
	       eString& my_name = el_macro_to_string_map[name_rep];
	       eString& that_name = el_macro_to_string_map[temp.name_rep];
	       if (aliases.is_member(my_name) &&
		   current_MDES->get_macro_aliased(my_name) == that_name)
		 return true;
	       else if(aliases.is_member(that_name) &&
		       current_MDES->get_macro_aliased(that_name) == my_name)
		 return true;
	       else
		 return false;
	     }
	   } else {
	     return false;
	   }
         }
      }
      else {
	if (name_rep == temp.name_rep)
	  return true;

	// need to check the macro aliases
	Hash_set<eString>& aliases = current_MDES->get_MDES_alias_macros();
	eString& my_name = el_macro_to_string_map[name_rep];
	eString& that_name = el_macro_to_string_map[temp.name_rep];
	if (aliases.is_member(my_name) &&
	    current_MDES->get_macro_aliased(my_name) == that_name)
	  return true;
	else if(aliases.is_member(that_name) &&
		current_MDES->get_macro_aliased(that_name) == my_name)
	  return true;
	else
	  return false;
      }
   }
   else if (operand.is_reg()) {
      Reg& temp = (Reg&)operand;
      if (allocated() && temp.is_static() && temp.allocated()) {
         return (physical_file == temp.physical_file_type() &&
                 mc_rep == temp.mc_num());
      } else
         return false;
   }
   else {
      return false ;
   }
   // upgraded to handle binding of macros to physical files.
   // we allow multiple copies of the same macro in different
   // physical files, and they should not be ==.

//    if (operand.is_macro_reg())
//      return (name_rep == ((Macro_reg&)operand).name_rep);
//    else return(false) ;
}

bool Macro_reg::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_macro_reg()) ;
}

bool Macro_reg::is_identical(const Base_operand& operand) const {
   if (operand.is_macro_reg()) {
      Macro_reg& r = (Macro_reg&)operand ;
      if ((dtype_rep == r.dtype_rep) && (file == r.file) &&
	  (name_rep == r.name_rep) && (physical_file == r.physical_file) &&
          (alloc == r.alloc) && (mc_rep == r.mc_rep)) {
	 return true ;
      }
   }
   return false ;
}

bool Macro_reg::is_fragile_macro() const
{
  if(name_rep == INT_ZERO) return false;
  if(name_rep == FLT_ZERO) return false;
  if(name_rep == FLT_ONE) return false;
  if(name_rep == DBL_ZERO) return false;
  if(name_rep == DBL_ONE) return false;
  if(name_rep == SP_REG) return false;
  if(name_rep == FP_REG) return false;
  if(name_rep == LV_REG) return false;
  if(name_rep == RGS_REG) return false;
  if(name_rep == IP_REG) return false;
  if(name_rep == OP_REG) return false;
  if(name_rep == LOCAL) return false;
  if(name_rep == PARAM) return false;
  if(name_rep == SWAP) return false;
  if(name_rep == PRED_FALSE) return false;
  if(name_rep == PRED_TRUE) return false;
  if(name_rep == VEC_MASK_TRUE) return false;

  return true;
}

bool Macro_reg::is_predicate_true() const
{
  return name_rep == PRED_TRUE;
}

bool Macro_reg::is_predicate_false() const
{
  return name_rep == PRED_FALSE;
}

bool Macro_reg::is_global() const { return global_rep; }

bool Macro_reg::is_hardwired() const { return hardwired_rep; }

bool Macro_reg::is_macro_reg() const { return true;}
Macro_name Macro_reg::name() const { return name_rep;}

DType Macro_reg::dtype() const {return dtype_rep;}
void Macro_reg::set_dtype(DType t) {dtype_rep = t;}

Reg_file Macro_reg::file_type() const {return file;}

Macro_reg& Macro_reg::bind_file(Reg_file reg_file) {
  file = reg_file;
  return *this;
}

void Macro_reg::set_name(Macro_name new_name)
{
   name_rep = new_name;
}

eString macro_reg_to_text (Macro_name macro)
{
  if (!el_macro_to_string_map.is_bound(macro))
    El_punt("macro_reg_to_text(): Macro not found. Did you initialize the macro maps?");
  return el_macro_to_string_map.value(macro);
}

void Macro_reg::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void Macro_reg::unbind_physical_file()
{
  physical_file = "";
}

eString Macro_reg::physical_file_type() const
{
  return physical_file;
}

bool Macro_reg::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

bool Macro_reg::allocated() const { return alloc; }

int Macro_reg::mc_num() const { return mc_rep; }

Macro_reg& Macro_reg::bind_reg(int mc_number) {
  alloc = true;
  mc_rep = mc_number;
  return *this;
}

Macro_reg& Macro_reg::unbind_reg() {
  alloc = false;
  mc_rep = 0;
  return *this;
}

int Macro_reg::hash() const
{
  if (alloc)
    return mc_rep;
  else
    return (int)name_rep;
}

void Macro_reg::print(ostream& os) const {
   os << "mr" << ":" << macro_reg_to_text(name_rep) << ":" << vregfile_to_mdesname(file);
   os << ":" << data_type_to_text(dtype_rep) 
     << ":" << physical_file;
   if (alloc)
      os << ":r" << mc_rep;
}

/* ------------- Integer literals */

Int_lit::Int_lit(int value)
  :rep(value), physical_file("") 
{
  int width; 
  bool sign = true;
  width = get_int_lit_width(value,sign);
  dtype_rep = DType( width, sign, EL_DT_INFO_INT );
}

Int_lit::Int_lit(unsigned int value)
  :rep(value), physical_file("") 
{
  int width; 
  bool sign = false;
  width = get_int_lit_width(value,sign);
  dtype_rep = DType( width, sign, EL_DT_INFO_INT );
}

Int_lit::Int_lit(int value, int width, bool sign)
  :rep(value), physical_file("") 
{
  int calc_width;
  calc_width = get_int_lit_width(value,sign);
  if (calc_width != width) {
    if ( width > calc_width )
      El_warn("Int_lit::Int_lit: Integer literal desired width (%d) larger than necessary to hold value (%d, w = %d, s = %d)", width, value, calc_width, sign);
    else
      El_punt("Int_lit::Int_lit: Integer literal desired width (%d) smaller than necessary to hold value (%d, w = %d, s = %d)", width, value, calc_width, sign);
  }

  dtype_rep = DType( width, sign, EL_DT_INFO_INT );
}

Int_lit::Int_lit(const Int_lit& literal)
  :rep(literal.rep), dtype_rep(literal.dtype_rep), physical_file(literal.physical_file) {} 

Base_operand* Int_lit::clone() const{
  return (new Int_lit(*this));
}

Int_lit::~Int_lit() {
  unbind_physical_file();
}

Int_lit& Int_lit::operator=(const Int_lit& literal) {
  rep = literal.rep;
  dtype_rep = literal.dtype_rep;
  physical_file = literal.physical_file;
  return *this;
}

bool Int_lit::operator==(const Base_operand& operand) const{
  if (operand.is_int()) 
    return ((((Int_lit&)operand).rep == rep) &&
	    (physical_file == ((Int_lit&)operand).physical_file));
  return false;
}

bool Int_lit::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_int()) ;
}

bool Int_lit::is_identical(const Base_operand& operand) const {
   if (operand.is_int()) {
     Int_lit& r = (Int_lit&)operand ;
     if (rep == r.rep &&
	 dtype_rep == r.dtype_rep &&
	 physical_file == r.physical_file)
       return true ;
   }
   return false ;
}

bool Int_lit::is_lit() const {return true;}
bool Int_lit::is_int() const {return true;}
int Int_lit::value() const {return rep;}
int Int_lit::int_value() const {return rep;}

void Int_lit::set_value(int new_val)
{
   rep = new_val;

   //added ajust width on set value --sherwood 7/99
   int width; 
   bool sign = true;
   width = get_int_lit_width(new_val,sign);
   dtype_rep = DType( width, sign, EL_DT_INFO_INT );
}

//added unsigned set value
void Int_lit::set_value(unsigned int new_val)
{
   rep = new_val;

   //added ajust width on set value --sherwood 7/99
   int width; 
   bool sign = false;
   width = get_int_lit_width(new_val,sign);
   dtype_rep = DType( width, sign, EL_DT_INFO_INT );
}

DType Int_lit::dtype() const {return dtype_rep;}
void Int_lit::set_dtype(DType t) {dtype_rep = t;}

void Int_lit::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void Int_lit::unbind_physical_file()
{
  physical_file = "";
}

eString Int_lit::physical_file_type() const
{
  return physical_file;
}

bool Int_lit::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

int Int_lit::hash() const {return rep;}

void Int_lit::print(ostream& os) const {
  if (dtype_rep.is_signed())
    os << "I:" << rep << ":" << ::data_type_to_text(dtype_rep);
  else
    os << "I:" << (unsigned)rep << ":" << ::data_type_to_text(dtype_rep);
}

/* ------------- Predicate literals */

Pred_lit::Pred_lit(bool value) 
   :rep(value), dtype_rep(EL_STANDARD_PREDICATE) {}
   
Pred_lit::Pred_lit(const Pred_lit& plit)   
   :rep(plit.rep), dtype_rep(plit.dtype_rep) {} 
   
   Base_operand* Pred_lit::clone() const
{
   return(new Pred_lit(*this)) ;
}

Pred_lit::~Pred_lit(){} ;

Pred_lit& Pred_lit::operator=(const Pred_lit& plit)
{
   rep = plit.rep ;
   dtype_rep = plit.dtype_rep;
   return(*this) ;
}

bool Pred_lit::operator==(const Base_operand& operand) const { 
  if (operand.is_predicate()) 
    return (((Pred_lit&)operand).rep == rep);
  else return false;
}

bool Pred_lit::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_predicate()) ;
}

bool Pred_lit::is_identical(const Base_operand& operand) const {
   if (operand.is_predicate()) {
      Pred_lit& r = (Pred_lit&)operand ;
      if ((rep == r.rep) && (dtype_rep == r.dtype_rep)) {
	 return true ;
      }
   }
   return false ;
}

bool Pred_lit::is_lit() const { return true;}
bool Pred_lit::is_predicate() const { return true;}
bool Pred_lit::is_predicate_true() const { return rep == true;}
bool Pred_lit::is_predicate_false() const { return rep == false;}

bool Pred_lit::value() const {return rep;}

void Pred_lit::set_value(bool new_val)
{
   rep = new_val;
}

DType Pred_lit::dtype() const {return dtype_rep;}
void Pred_lit::set_dtype(DType t) {dtype_rep = t;}

int Pred_lit::hash() const { return (rep) ; }

void Pred_lit::print(ostream& os) const {
  os << "P:" << rep;
}

/* ----------------- Floating-point literals */

Float_lit::Float_lit(float value)
  :rep(value), dtype_rep(EL_STANDARD_FLOAT), physical_file("") {}

Float_lit::Float_lit(const Float_lit& literal)
  :rep(literal.rep), dtype_rep(literal.dtype_rep), physical_file(literal.physical_file) {} 

  Base_operand* Float_lit::clone() const{
  return (new Float_lit(*this));
}

Float_lit::~Float_lit() {};

Float_lit& Float_lit::operator=(const Float_lit& literal){
  rep = literal.rep;
  dtype_rep = literal.dtype_rep;
  physical_file = literal.physical_file;
  return *this;
}

bool Float_lit::operator==(const Base_operand& operand) const{
 if (operand.is_float()) 
    return ((((Float_lit&)operand).rep == rep) &&
	    (physical_file == ((Float_lit&)operand).physical_file));
 else return false;
}

bool Float_lit::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_float()) ;
}

bool Float_lit::is_identical(const Base_operand& operand) const {
   if (operand.is_float()) {
     Float_lit& r = (Float_lit&)operand ;
     if ((rep == r.rep) && (dtype_rep == r.dtype_rep) &&
	 (physical_file == r.physical_file))
       return true ;
   }
   return false ;
}

bool Float_lit::is_lit() const {return true;}
bool Float_lit::is_float() const {return true;}
float Float_lit::value() const {return rep;}

void Float_lit::set_value(float new_val)
{
   rep = new_val;
}

DType Float_lit::dtype() const {return dtype_rep;}
void Float_lit::set_dtype(DType t) {dtype_rep = t;}

void Float_lit::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void Float_lit::unbind_physical_file()
{
  physical_file = "";
}

eString Float_lit::physical_file_type() const
{
  return physical_file;
}

bool Float_lit::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

int Float_lit::hash() const {return ((int) rep);}

void Float_lit::print(ostream& os) const {
  os << "F:" << rep;
}



/* ---------------- Double floating-point literals */

Double_lit::Double_lit(double value)
  :rep(value), dtype_rep(EL_STANDARD_DOUBLE), physical_file("") {}

Double_lit::Double_lit(const Double_lit& literal)
  :rep(literal.rep), dtype_rep(literal.dtype_rep), physical_file(literal.physical_file) {} 

  Base_operand* Double_lit::clone() const{
  return (new Double_lit(*this));
}

Double_lit::~Double_lit(){};

Double_lit& Double_lit::operator=(const Double_lit& literal){
  rep = literal.rep;
  dtype_rep = literal.dtype_rep;
  physical_file = literal.physical_file;
  return *this;
}

bool Double_lit::operator==(const Base_operand& operand) const{
  if (operand.is_double()) 
    return ((((Double_lit&)operand).rep == rep) &&
	    (physical_file == ((Double_lit&)operand).physical_file));
  else return false;
}

bool Double_lit::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_double()) ;
}

bool Double_lit::is_identical(const Base_operand& operand) const {
   if (operand.is_double()) {
     Double_lit& r = (Double_lit&)operand ;
     if ((rep == r.rep) && (dtype_rep == r.dtype_rep) &&
	 (physical_file == r.physical_file))
       return true ;
   }
   return false ;
}

bool Double_lit::is_lit() const {return true;}
bool Double_lit::is_double() const {return true;}
double Double_lit::value() const {return rep;}

void Double_lit::set_value(double new_val)
{
   rep = new_val;
}

DType Double_lit::dtype() const {return dtype_rep;}
void Double_lit::set_dtype(DType t) {dtype_rep = t;}

void Double_lit::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void Double_lit::unbind_physical_file()
{
  physical_file = "";
}

eString Double_lit::physical_file_type() const
{
  return physical_file;
}

bool Double_lit::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

int Double_lit::hash() const {return ((int) rep);}

void Double_lit::print(ostream& os) const {
  os << "D:" << rep;
}

/* ----------------- String literal class */

String_lit::String_lit(const eString& value, DType_info dti)
  :rep(value), dtype_rep(EL_STANDARD_STRING), physical_file("") 
{
  dtype_rep.set_info(dti);
}

String_lit::String_lit(const String_lit& literal)
  :rep(literal.rep), dtype_rep(literal.dtype_rep), physical_file(literal.physical_file) {} 

Base_operand* String_lit::clone() const{
  return (new String_lit(*this));
}

String_lit::~String_lit(){};

String_lit& String_lit::operator=(const String_lit& literal){
  rep = literal.rep;
  dtype_rep = literal.dtype_rep;
  physical_file = literal.physical_file;
  return *this;
}

bool String_lit::operator==(const Base_operand& operand) const{
  if (operand.is_string()) 
    return (((String_lit&)operand).rep == rep);
  else return false;
}

bool String_lit::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_string()) ;
}

bool String_lit::is_identical(const Base_operand& operand) const {
   if (operand.is_string()) {
      String_lit& r = (String_lit&)operand ;
      if ((rep == r.rep) && (dtype_rep == r.dtype_rep) &&
	  physical_file == r.physical_file) {
	 return true ;
      }
   }
   return false ;
}

bool String_lit::is_lit() const {return true;}
bool String_lit::is_string() const {return true;}
eString String_lit::value() const {return rep;}

void String_lit::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void String_lit::unbind_physical_file()
{
  physical_file = "";
}

eString String_lit::physical_file_type() const
{
  return physical_file;
}

bool String_lit::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

int String_lit::hash() const { return (hash_estring((eString&)rep)) ; }

void String_lit::set_value(eString new_val)
{
   rep = new_val;
}

DType String_lit::dtype() const {return dtype_rep;}
void String_lit::set_dtype(DType t) {dtype_rep = t;}

void String_lit::print(ostream& os) const {
 os << "S:" << "\""<< rep << "\"";
}


/* ---------------- Label literal class */

Label_lit::Label_lit(const eString& value, DType_info dti)
  :rep(value), dtype_rep(EL_STANDARD_LABEL), physical_file("") 
{
  dtype_rep.set_info(dti);
}

Label_lit::Label_lit(const Label_lit& literal)
  :rep(literal.rep), dtype_rep(literal.dtype_rep), physical_file(literal.physical_file) {} 

Base_operand* Label_lit::clone() const {
  return (new Label_lit(*this));
}

Label_lit::~Label_lit(){};

Label_lit& Label_lit::operator=(const Label_lit& literal){
  rep = literal.rep;
  dtype_rep = literal.dtype_rep;
  physical_file = literal.physical_file;
  return *this;
}


bool Label_lit::operator==(const Base_operand& operand) const{
  if (operand.is_label())
    return (((Label_lit&)operand).rep == rep);
  else return false;
}

bool Label_lit::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_label()) ;
}

bool Label_lit::is_identical(const Base_operand& operand) const {
   if (operand.is_label()) {
      Label_lit& r = (Label_lit&)operand ;
      if ((rep == r.rep) && (dtype_rep == r.dtype_rep) &&
	  physical_file == r.physical_file) {
	 return true ;
      }
   }
   return false ;
}

bool Label_lit::is_lit() const {return true;}
bool Label_lit::is_label() const {return true;}
eString Label_lit::value() const {return rep;}

void Label_lit::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void Label_lit::unbind_physical_file()
{
  physical_file = "";
}

eString Label_lit::physical_file_type() const
{
  return physical_file;
}

bool Label_lit::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

int Label_lit::hash() const {return (hash_estring((eString&)rep)); }

void Label_lit::set_value(eString new_val)
{
   rep = new_val;
}

DType Label_lit::dtype() const {return dtype_rep;}
void Label_lit::set_dtype(DType t) {dtype_rep = t;}

void Label_lit::print(ostream& os) const {
 os << "L:" << rep;
}

/* ----------- Control Block class */

Cb_operand::Cb_operand(int i)
   :id_rep(i), dtype_rep(EL_STANDARD_BRANCH), physical_file("") {}
   
Cb_operand::Cb_operand(const Cb_operand& cb)
   :id_rep(cb.id_rep), dtype_rep(cb.dtype_rep), physical_file(cb.physical_file) {}
   
Base_operand* Cb_operand::clone() const
{
   return(new Cb_operand(*this)) ;
}

Cb_operand::~Cb_operand(){}

Cb_operand& Cb_operand::operator=(const Cb_operand& cb) 
{
   id_rep = cb.id_rep ;
   dtype_rep = cb.dtype_rep ;
   physical_file = cb.physical_file;
   return *this ;
}

bool Cb_operand::operator==(const Base_operand& operand) const {
  if (operand.is_cb())
    return (id_rep == ((Cb_operand&)operand).id_rep);
  else return false;
}

bool Cb_operand::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_cb()) ;
}

bool Cb_operand::is_identical(const Base_operand& operand) const {
   if (operand.is_cb()) {
      Cb_operand& r = (Cb_operand&)operand ;
      if ((id_rep == r.id_rep) && (dtype_rep == r.dtype_rep) &&
	  physical_file == r.physical_file) {
	 return true ;
      }
   }
   return false ;
}

bool Cb_operand::is_lit() const { return true;}
bool Cb_operand::is_cb() const { return true;}

int Cb_operand::id() const { return id_rep;}

void Cb_operand::set_id(int new_id)
{
   id_rep = new_id;
}

DType Cb_operand::dtype() const {return dtype_rep;}
void Cb_operand::set_dtype(DType t) {dtype_rep = t;}

void Cb_operand::bind_physical_file(eString phys_file)
{
  physical_file = phys_file;
}

void Cb_operand::unbind_physical_file()
{
  physical_file = "";
}

eString Cb_operand::physical_file_type() const
{
  return physical_file;
}

bool Cb_operand::assigned_to_physical_file() const
{
  if(physical_file != "")
    return true;
  return false;
}

int Cb_operand::hash() const { return id_rep ; }

void Cb_operand::print(ostream& os) const
{
    os << "CB:" << id_rep;
}


/* ---------- Undefined class */

Undefined::Undefined() {}
Undefined::Undefined(const Undefined&) {}

Base_operand* Undefined::clone() const{
  return (new Undefined(*this));
}

Undefined::~Undefined() {}

Undefined& Undefined::operator=(const Undefined&) { return *this;}

bool Undefined::operator==(const Base_operand& operand) const {
  return (operand.is_undefined() ? true : false);
}

bool Undefined::same_operand_class(const Base_operand& oprnd) const
{
   return(oprnd.is_undefined()) ;
}

bool Undefined::is_undefined() const {return true;}

bool Undefined::is_identical(const Base_operand& operand) const {
   return(operand.is_undefined()) ;
}

int Undefined::hash() const { return (1) ;} 

void Undefined::print(ostream& os) const {
  os << "undefined?";
}

////////////////////////////////////////////////////////////////////////////
//  Operand class  :  An envelope for Base_operand
////////////////////////////////////////////////////////////////////////////

Operand::Operand()
{
   if (undefined_base == NULL) {
      Undefined* tmpu = new Undefined ;
      Symboltable_entry* undefined_op = new Symboltable_entry(tmpu) ;
      global_symbol_table.bind(*undefined_op, tmpu) ;
      undefined_base = &global_symbol_table.value(*undefined_op) ; 
   }
   
   sptr = undefined_base ;
}
Operand::~Operand() 
{
}

Operand::Operand(Base_operand * ptr)
{
   validate_and_point_to(ptr) ;
}

Operand::Operand(const Operand& op)
   : sptr(op.sptr)
{}

Operand& Operand::operator=(const Operand& op)
{
   sptr = op.sptr ;
   return (*this) ;
} 

// WARNING: This checks only the logical equality of Base_operands
//          For deep structural equality, you must compare the embedded
//          Symboltable_entrys as done for the symbol table.
bool Operand::operator==(const Operand& op) const
{
   Base_operand* bo1 = sptr->optr ;
   Base_operand* bo2 = op.sptr->optr ;
   if ((bo1->is_reg() || bo1->is_macro_reg()) &&
       (bo2->is_reg() || bo2->is_macro_reg())) {
     // (((Reg*)bo1)->allocated() || ((Reg*)bo2)->allocated() ||
     //  ((Reg*)bo1)->is_rotating() || ((Reg*)bo2)->is_rotating())) {
      if (*bo1 == *bo2) return true ;
      else return false ;
   }
   else if (bo1->is_mem_vr() && bo2->is_mem_vr()) {
     // Also call the derived class operator== for mem_vrs.  -KF 2/2007
     return (*bo1 == *bo2);
   }
   else {
      if (bo1 == bo2) return true ;
      else return false ;
   }
}

bool Operand::operator!=(const Operand& op) const
{
   if (*this == op) return false ;
   else return true ;
} 

bool Operand::operator<(const Operand& oprnd) const
{
   if (sptr < oprnd.sptr) return true ;
   else return false ;
}

bool Operand::same_operand_class(const Operand& oprnd) const
{
   return(sptr->optr->same_operand_class(*(oprnd.sptr->optr))) ;
}
   
bool Operand::is_identical(const Operand& op) const
{
   return(sptr == op.sptr) ;
}
bool Operand::is_undefined() const
{
   return(sptr->optr->is_undefined()) ;
} 
bool Operand::is_reg() const
{
   return(sptr->optr->is_reg()) ;
} 
bool Operand::is_vr_name() const
{
   return(sptr->optr->is_vr_name()) ;
} 
bool Operand::is_mem_vr() const
{
   return(sptr->optr->is_mem_vr()) ;
} 
bool Operand::is_macro_reg() const
{
   return(sptr->optr->is_macro_reg()) ;
} 
bool Operand::is_lit() const
{
   return(sptr->optr->is_lit()) ;
} 
bool Operand::is_int() const
{
   return(sptr->optr->is_int()) ;
} 
bool Operand::is_predicate() const
{
   return(sptr->optr->is_predicate()) ;
} 
bool Operand::is_predicate_true() const
{
   return(sptr->optr->is_predicate_true()) ;
} 
bool Operand::is_predicate_false() const
{
   return(sptr->optr->is_predicate_false()) ;
} 
bool Operand::is_float() const
{
   return(sptr->optr->is_float()) ;
} 
bool Operand::is_double() const
{
   return(sptr->optr->is_double()) ;
} 
bool Operand::is_string() const
{
   return(sptr->optr->is_string()) ;
} 
bool Operand::is_label() const
{
   return(sptr->optr->is_label()) ;
} 
bool Operand::is_cb() const
{
   return(sptr->optr->is_cb()) ;
} 
bool Operand::assigned_to_file() const
{
   return(sptr->optr->assigned_to_file()) ;
} 
bool Operand::allocated() const
{
   return(sptr->optr->allocated()) ;
}
DType Operand::dtype() const
{
   return (sptr->optr->dtype()) ;
}
Reg_file Operand::file_type() const
{
   return (sptr->optr->file_type()) ;
}

void Operand::set_dtype(DType t)  // 
{
   if (sptr->optr->is_reg()) {
      Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
      new_reg->set_dtype(t) ;
      validate_and_point_to(new_reg) ;
   }
   else if (sptr->optr->is_macro_reg()) {
      Macro_reg* new_mreg = (Macro_reg*) ((Macro_reg*)sptr->optr)->clone() ;
      new_mreg->set_dtype(t) ;
      validate_and_point_to(new_mreg) ;
   }
   else if (sptr->optr->is_vr_name()) {
      VR_name* new_vr_name = (VR_name*) ((VR_name*)sptr->optr)->clone() ;
      new_vr_name->set_dtype(t) ;
      validate_and_point_to(new_vr_name) ;
   }
   else if (sptr->optr->is_mem_vr()) {
      assert(0);
   }
   else if (sptr->optr->is_int()) {
      Int_lit* new_int_lit = (Int_lit*) ((Int_lit*)sptr->optr)->clone() ;
      new_int_lit->set_dtype(t) ;
      validate_and_point_to(new_int_lit) ;
   }
   else if (sptr->optr->is_predicate()) {
      Pred_lit* new_pred = (Pred_lit*) ((Pred_lit*)sptr->optr)->clone() ;
      new_pred->set_dtype(t) ;
      validate_and_point_to(new_pred) ;
   }
   else if (sptr->optr->is_float()) {
      Float_lit* new_float = (Float_lit*) ((Float_lit*)sptr->optr)->clone() ;
      new_float->set_dtype(t) ;
      validate_and_point_to(new_float) ;
   }
   else if (sptr->optr->is_double()) {
      Double_lit* new_double = (Double_lit*) ((Double_lit*)sptr->optr)->clone() ;
      new_double->set_dtype(t) ;
      validate_and_point_to(new_double) ;
   }
   else if (sptr->optr->is_string()) {
      String_lit* new_string = (String_lit*) ((String_lit*)sptr->optr)->clone() ;
      new_string->set_dtype(t) ;
      validate_and_point_to(new_string) ;
   }
   else if (sptr->optr->is_label()) {
      Label_lit* new_label = (Label_lit*) ((Label_lit*)sptr->optr)->clone() ;
      new_label->set_dtype(t) ;
      validate_and_point_to(new_label) ;
   }
   else if (sptr->optr->is_cb()) {
      Cb_operand* new_cb = (Cb_operand*) ((Cb_operand*)sptr->optr)->clone() ;
      new_cb->set_dtype(t) ;
      validate_and_point_to(new_cb) ;
   }
   else {
      assert(0);
   }
}

int Operand::data_type_width() const {
  return dtype().width();
}    

/// Class specific Operand methods

// Reg class

// you can't just cast these to Reg!!! this accesses invalid parts of
// memory! -ntclark

bool Operand::equal_vr(const Operand& op) const
{
  if(is_reg())
    return(((Reg*)sptr->optr)->equal_vr(*op.sptr->optr)) ;
  else 
    return false;
}
bool Operand::equal_filevr(const Operand& op) const
{
  if(is_reg())
    return(((Reg*)sptr->optr)->equal_filevr(*op.sptr->optr)) ;
  else
    return false;
}
bool Operand::equal_mc(const Operand& op) const
{
  if(is_reg())
    return(((Reg*)sptr->optr)->equal_mc(*op.sptr->optr)) ;
  else
    return false;
}
bool Operand::is_static() const
{
  if(is_reg())
    return(((Reg*)sptr->optr)->is_static()) ;
  else return false;
}
bool Operand::is_rotating() const
{
  if(is_reg())
    return(((Reg*)sptr->optr)->is_rotating()) ;
  else
    return false;
}
int Operand::mc_num() const
{
  if(is_reg())
    return(((Reg*)sptr->optr)->mc_num()) ;
  else if(is_macro_reg())
    return(((Macro_reg*)sptr->optr)->mc_num()) ;
  else {
    El_punt("Operand::mc_num(): this method can only be called on registers and macros.");
    return 0;
  }
}

Operand& Operand::bind_file(Reg_file rf, Reg_sr rs)
{
  if(!is_reg())
    El_punt("can't call this method if it's not a register!\nthis would corrupt memory.");
  Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
  new_reg->bind_file(rf,rs) ;
  validate_and_point_to(new_reg) ;
  return *this;
}

Operand& Operand::bind_reg(int x)
{
  if (is_reg()) {
    Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
    new_reg->bind_reg(x) ;
    validate_and_point_to(new_reg) ;
  } else if (is_macro_reg()) {
    Macro_reg* new_reg = (Macro_reg*) ((Macro_reg*)sptr->optr)->clone() ;
    new_reg->bind_reg(x) ;
    validate_and_point_to(new_reg) ;
  } else
    El_punt("Operand::bind_reg(): this method can only be called on registers and macros.");
  return *this;
}

Operand& Operand::unbind_reg()
{
  if (is_reg()) {
    Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
    new_reg->unbind_reg() ;
    validate_and_point_to(new_reg) ;
  } else if (is_macro_reg()) {
    Macro_reg* new_reg = (Macro_reg*) ((Macro_reg*)sptr->optr)->clone() ;
    new_reg->unbind_reg() ;
    validate_and_point_to(new_reg) ;
  } else
    El_punt("Operand::unbind_reg(): this method can only be called on registers and macros.");
  return *this;
}

// Reg and Mem_vr class

int Operand::vr_num() const
{
   if(sptr->optr->is_reg()) {
      return(((Reg*)sptr->optr)->vr_num()) ;
   } else if (sptr->optr->is_vr_name()) {
      return(((VR_name*)sptr->optr)->vr_num()) ;
   } else if (sptr->optr->is_mem_vr()) {
      return(((Mem_vr*)sptr->optr)->vr_num()) ;
   } else if (sptr->optr->is_macro_reg()) {
      return(0-(((Macro_reg*)sptr->optr)->name())) ;
   } else {
      El_punt("This operand type is not supported for this method") ;
   }
   return (0) ;
}

int Operand::omega() const
{
   if(!(sptr->optr->is_reg() || sptr->optr->is_mem_vr())) {
      El_punt("This operand type is not supported for this method") ;
   }
   return(sptr->optr->omega());
}

int Operand::min_omega() const
{
   if(!(sptr->optr->is_reg() ||
	sptr->optr->is_vr_name() ||
	sptr->optr->is_mem_vr())) {
      El_punt("This operand type is not supported for this method") ;
   }
   if (sptr->optr->is_vr_name()) {
      Operand oprnd(new Reg(*this)) ;
      return(oprnd.min_omega()) ;
   }
   if (omega() == 0) {
      return(sptr->min_omega) ;
   }
   else {
      Operand oprnd(*this) ;
      oprnd.set_omega(0) ;
      return(oprnd.sptr->min_omega) ;
   }
}

int Operand::max_omega() const
{
   if(!(sptr->optr->is_reg() ||
	sptr->optr->is_vr_name() ||
	sptr->optr->is_mem_vr())) {
      El_punt("This operand type is not supported for this method") ;
   }
   if (sptr->optr->is_vr_name()) {
      Operand oprnd(new Reg(*this)) ;
      return(oprnd.max_omega()) ;
   }
   if (omega() == 0) {
      return(sptr->max_omega) ;
   }
   else {
      Operand oprnd(*this) ;
      oprnd.set_omega(0) ;
      return(oprnd.sptr->max_omega) ;
   }
}

Operand& Operand::rename()
{
   if(sptr->optr->is_reg()) {
      Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
      new_reg->rename() ;
      validate_and_point_to(new_reg) ;
   } else if (sptr->optr->is_mem_vr()) {
      Mem_vr* new_memvr = (Mem_vr*) ((Mem_vr*)sptr->optr)->clone() ;
      new_memvr->rename() ;
      validate_and_point_to(new_memvr) ;
   } else {
      El_punt("This operand type is not supported for this method") ;
   }
   return(*this) ;
}

Operand& Operand::rename(int newnum)
{
   if(sptr->optr->is_reg()) {
      Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
      new_reg->rename(newnum) ;
      validate_and_point_to(new_reg) ;
   } else if (sptr->optr->is_mem_vr()) {
      Mem_vr* new_memvr = (Mem_vr*) ((Mem_vr*)sptr->optr)->clone() ;
      new_memvr->rename(newnum) ;
      validate_and_point_to(new_memvr) ;
   } else {
      El_punt("This operand type is not supported for this method") ;
   }
   return(*this) ;
}

Operand& Operand::incr_omega (int incr) 
{
   if(sptr->optr->is_reg()) {
      Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
      new_reg->incr_omega(incr) ;
      validate_and_point_to(new_reg) ;
   } else if (sptr->optr->is_mem_vr()) {
      Mem_vr* new_memvr = (Mem_vr*) ((Mem_vr*)sptr->optr)->clone() ;
      new_memvr->incr_omega(incr) ;
      validate_and_point_to(new_memvr) ;
   } else {
      El_punt("This operand type is not supported for this method") ;
   }
   return(*this) ;
}
void Operand::set_vr_num(int new_val)
{
   if(sptr->optr->is_reg()) {
      Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
      new_reg->set_vr_num(new_val) ;
      validate_and_point_to(new_reg) ;
   } else if (sptr->optr->is_mem_vr()) {
      Mem_vr* new_memvr = (Mem_vr*) ((Mem_vr*)sptr->optr)->clone() ;
      new_memvr->set_vr_num(new_val) ;
      validate_and_point_to(new_memvr) ;
   } else if (sptr->optr->is_vr_name()) {
      VR_name* new_vr_name = (VR_name*) ((VR_name*)sptr->optr)->clone() ;
      new_vr_name->set_vr_num(new_val) ;
      validate_and_point_to(new_vr_name) ;
   } else {
      El_punt("This operand type is not supported for this method") ;
   }
}
void Operand::set_omega(int new_val) 
{
   if(sptr->optr->is_reg()) {
      Reg* new_reg = (Reg*) ((Reg*)sptr->optr)->clone() ;
      new_reg->set_omega(new_val) ;
      validate_and_point_to(new_reg) ;
   } else if (sptr->optr->is_mem_vr()) {
      Mem_vr* new_memvr = (Mem_vr*) ((Mem_vr*)sptr->optr)->clone() ;
      new_memvr->set_omega(new_val) ;
      validate_and_point_to(new_memvr) ;
   } else {
      El_punt("This operand type is not supported for this method") ;
   }
}


bool Operand::is_same_name(const Operand& op) const
{
   if(sptr->optr->is_vr_name()) {
      return (((VR_name*) sptr->optr)->same_name(*op.sptr->optr)) ;
   }
   else {
      El_punt("This operand type is not supported for this method") ;
      return false ;
   }
}

// Macro_reg class functions

Macro_name Operand::name() const
{
   if (!sptr->optr->is_macro_reg()) {
      El_punt("This operand type is not supported for this method") ;
   }
   return(((Macro_reg*)sptr->optr)->name()) ;
}

void Operand::set_name(Macro_name new_val)
{
   if (!sptr->optr->is_macro_reg()) {
      El_punt("This operand type is not supported for this method") ;
   }
   ((Macro_reg*)sptr->optr)->set_name(new_val);
}

bool Operand::is_fragile_macro()
{
   if (!sptr->optr->is_macro_reg()) {
      El_punt("This operand type is not supported for this method") ;
   }
   return(((Macro_reg*)sptr->optr)->is_fragile_macro()) ;
}

bool Operand::is_global() const
{
   if (!sptr->optr->is_macro_reg()) {
      El_punt("This operand type is not supported for this method") ;
   }
   return(((Macro_reg*)sptr->optr)->is_global()) ;
}

bool Operand::is_hardwired() const
{
   if (!sptr->optr->is_macro_reg()) {
      El_punt("This operand type is not supported for this method") ;
   }
   return(((Macro_reg*)sptr->optr)->is_hardwired()) ;
}


int Operand::id() const
{
   if (!sptr->optr->is_cb()) {
      El_punt("This operand type is not supported for this method") ;
   }
   return(((Cb_operand*)sptr->optr)->id()) ;
}

void Operand::set_id(int new_val) 
{
   if (!sptr->optr->is_cb()) {
      El_punt("This operand type is not supported for this method") ;
   }
   Cb_operand* new_cb = ( Cb_operand*) (( Cb_operand*)sptr->optr)->clone() ;
   new_cb->set_id(new_val) ;
   validate_and_point_to(new_cb) ;
}

// value methods for literal classes

bool Operand::pred_value()
{
   if (!sptr->optr->is_predicate()) {
      El_punt("This operand type is not supported for this method") ;
   }
   return((Pred_lit*)sptr->optr)->value() ;
}

void Operand::set_pred_value(bool new_val) 
{
   if (!sptr->optr->is_predicate()) {
      El_punt("This operand type is not supported for this method") ;
   }
   Pred_lit* new_lit = (Pred_lit*) ((Pred_lit*)sptr->optr)->clone() ;
   new_lit->set_value(new_val) ;
   validate_and_point_to(new_lit) ;
}

int  Operand::int_value()
{
   return((Int_lit*)sptr->optr)->value() ;
}

void Operand::set_int_value(int new_val) 
{
  if(! (is_lit() && is_int()))
    El_punt("can't call this method if it's not a register!\nthis would corrupt memory.");
  Int_lit* new_lit = (Int_lit*) ((Int_lit*)sptr->optr)->clone() ;
  new_lit->set_value(new_val) ;
  validate_and_point_to(new_lit) ;
}

double Operand::float_value()
{
  if(! (is_lit() && is_float()))
    El_punt("can't call this method if it's not a register!\nthis would corrupt memory.");  
  return((Float_lit*)sptr->optr)->value() ;
}

void Operand::set_float_value(double new_val) 
{
   Float_lit* new_lit = (Float_lit*) ((Float_lit*)sptr->optr)->clone() ;
   new_lit->set_value((float)new_val) ;
   validate_and_point_to(new_lit) ;
}

double Operand::double_value()
{
   return((Double_lit*)sptr->optr)->value() ;
}

void Operand::set_double_value(double new_val) 
{
   Double_lit* new_lit = (Double_lit*) ((Double_lit*)sptr->optr)->clone() ;
   new_lit->set_value(new_val) ;
   validate_and_point_to(new_lit) ;
}

eString Operand::string_value()
{
   return((String_lit*)sptr->optr)->value() ;
}

void Operand::set_string_value(eString new_val) 
{
   String_lit* new_lit = (String_lit*) ((String_lit*)sptr->optr)->clone() ;
   new_lit->set_value(new_val) ;
   validate_and_point_to(new_lit) ;
}

eString Operand::label_value()
{
   return((Label_lit*)sptr->optr)->value() ;
}

void Operand::set_label_value(eString new_val) 
{
   Label_lit* new_lit = (Label_lit*) ((Label_lit*)sptr->optr)->clone() ;
   new_lit->set_value(new_val) ;
   validate_and_point_to(new_lit) ;
}

eString Operand::physical_file_type() const
{
   return sptr->optr->physical_file_type();
}

void Operand::bind_physical_file(eString phys_file)
{
  Base_operand* new_opd = sptr->optr->clone();
  new_opd->bind_physical_file(phys_file);
  validate_and_point_to(new_opd);
}


void Operand::unbind_physical_file()
{
  sptr->optr->unbind_physical_file();
}

bool Operand::assigned_to_physical_file() const
{
  return sptr->optr->assigned_to_physical_file();
}

void Operand::validate_and_point_to(Base_operand* base_ptr)
{
   // Make sure the Symboltable_entry version of base_ptr is in
   // Symbol table. If it is, base_ptr is deleted. Otherwise, it
   // is permanently insered in the global symbol table.

   if (base_ptr == NULL) {
      // first make sure undefined_base was initialized
      if (undefined_base == NULL) {
	 Operand dummy ;
      }
      assert (undefined_base != NULL) ;
      sptr = undefined_base ;
      return ;
   }
   Symboltable_entry oprnd(base_ptr) ;  
   if (!global_symbol_table.is_bound(oprnd)) {
      // New operand in the symbol table. 
      global_symbol_table.bind(oprnd,oprnd.operand_ptr) ;
      sptr = &(global_symbol_table.value(oprnd)) ;

      // Fix min_omega and max_omega
      int new_omega = sptr->optr->omega() ;
      if ( new_omega != 0) {
	 Symboltable_entry dummy0(oprnd) ;

	 // Save the omega so that we restore it right away
	 int old_omega = dummy0.operand_ptr->omega() ;
	 dummy0.operand_ptr->set_omega(0) ;
	 if (!global_symbol_table.is_bound(dummy0)) {
	    Base_operand* entry_base = oprnd.operand_ptr->clone() ;
	    Symboltable_entry entry_operand(entry_base) ;
	    global_symbol_table.bind(entry_operand,entry_operand.operand_ptr) ;
	 }
	 Symbol_table_info& si =  global_symbol_table.value(dummy0) ;
	 if (si.min_omega > new_omega) si.min_omega = new_omega ;
	 if (si.max_omega < new_omega) si.max_omega = new_omega ;

	 // restore the omega IMPORTANT
	 dummy0.operand_ptr->set_omega(old_omega) ;
      }	 
   }
   else {
      sptr = &(global_symbol_table.value(oprnd)) ;
      delete (oprnd.operand_ptr) ;
   }
   return ;
}


Hash_map<Symboltable_entry,Symbol_table_info>
        Operand::global_symbol_table(hash_symboltable_entry,100017) ;

Symbol_table_info* Operand::undefined_base = NULL ;

void Operand::init_global_symbol_table()
{
  //   undefined_base = new Undefined ;
  //   Symboltable_entry* undefined_op = new Symboltable_entry(undefined_base) ;
  //   global_symbol_table.bind(*undefined_op, undefined_base) ;
}

void Operand::clear_global_symbol_table()
{
  // First delete all base operands pointed from the table
  // Shail Aditya, Vinod Kathail 08/10/98
  for (Hash_map_iterator<Symboltable_entry,Symbol_table_info> 
	 iter(global_symbol_table); iter!=0; iter++) {
    Symbol_table_info si = (*iter).second;
    delete si.optr;
  }
  global_symbol_table.clear();
  undefined_base = NULL;
}

///////  Output method

ostream& operator<<(ostream& os, const Operand& operand){
  if (operand.sptr->optr)
    operator<<(os, *operand.sptr->optr);
  else os << "??";
  return os;
}

ostream& operator<<(ostream& os, const Symboltable_entry& operand){
  if (operand.operand_ptr)
    operator<<(os, *operand.operand_ptr);
  else os << "??";
  return os;
}

void Operand::print_for_pqs(ostream& os) const
{
  if (sptr->optr)
    sptr->optr->print_for_pqs(os);
  else
    os << "??";
}

unsigned hash_operand_2(Operand& oprnd)
{
    if (oprnd.is_reg()) {
        return ELCOR_INT_ABS(oprnd.vr_num() + oprnd.omega());
    } else if (oprnd.is_macro_reg()) {
        return oprnd.name();
    } else if (oprnd.is_mem_vr()) {
      return oprnd.vr_num();  // don't hash on omega since it may change.
    } else {
      return (ELCOR_INT_HASH(oprnd.get_ptr())) ;
    }
}

///////////////////////////////////////////////////////////////////

unsigned hash_operand(Operand& oprnd)
{
   if (oprnd.is_reg()) {
      if (oprnd.allocated()) {
	 return (oprnd.mc_num()) ;
      }
      else {
	 return (ELCOR_INT_ABS(oprnd.vr_num() + oprnd.omega())) ;
      }
   }
   else if (oprnd.is_macro_reg()) {
     if (oprnd.allocated())
       return oprnd.mc_num();
     else
       return oprnd.name();
   }
   else if (oprnd.is_mem_vr()) {
     return oprnd.vr_num();  // don't hash on omega since it may change.
   }
   else {
      return (ELCOR_INT_HASH(oprnd.get_ptr())) ;
   }
//      char p[16384] ;
//      ostrstream ost(p,16384) ;
//      ost << oprnd << '\0';
//      char* tmp_c = &(p[0]) ;
//      return (hash_char_ptr(tmp_c)) ;
}

///////////////////////////////////////////////////////////////////

unsigned hash_symboltable_entry(Symboltable_entry& oprnd)
{
   return (ELCOR_INT_HASH(oprnd.operand_ptr->hash())) ;
//   ostrstream ost(p,16384) ;
//   ost << oprnd << '\0';
//   char* tmp_c = &(p[0]) ;
//   return (hash_char_ptr(tmp_c)) ;
}

// Debug parameters

Operand* oper1 = NULL ;
Operand* oper2 = NULL ;
Operand* oper3 = NULL ;

// end of operand.cpp
