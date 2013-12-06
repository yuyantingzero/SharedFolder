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
//      File:           operand.h
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson,
//                      Santosh Abraham, Scott Mahlke, Dave August,
//                      Joel Jones, Greg Snider, Sumedh Sathaye
//      Created:        December 1994
//      Description:    Operand class declaration 
//
/////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------  
  Operand and its derived classes.
    Operand is a pure class and cannot be instantiated, only its subclasses
    can be. Reg class is mutable; all others are not. 

    Reg: This represents an EVR and its binding to a physical register.
    VR_name: This reperesents the name of an EVR (argument to remap())
    Mem_vr: Virtual registers for annotating  memory depndences 
    Macro_reg: Ala Lcode

    Int_lit: Integer literals
    Pred_lit: Predicate literal
    Float_lit: Single precision floating point literals
    Double_lit: Double precision floating point literals
    String_lit: String literals
    Label_lit: Labels 
    Cb: Control block ala Lcode

    Undefined: To denote operands that have not been specified.

    Derived classes have class specific functions, that can be accessed only
    through these classes. An Operand class must be first cast to one of
    these classes to make use of class specific functions.
    (Do we want to change this?) 

----------------------------------------------------------------------------*/
    

#ifndef _OPERAND_H
#define _OPERAND_H

#include <iostream>
#include "defs.h"
#include "string_class.h"
#include "hash_map.h"
#include "el_symbol_table.h"
#include "dtype.h"
#include "macro.h"

using namespace std;

class Symbol_table_info ;

extern int virtual_register_number; /* Virtual_register_number to generate 
                                       unique numbers */
void adjust_vr_num(int number);     /* set virtual_register_number to
                                    max of number and itself */

extern eString data_type_to_text(DType d);

enum Reg_file {
  UNDEF_VRF=-1,
  FIRST_FILE=0,
  GPR=0,
  FPR=1,
  PR=2,
  BTR=3,
  CR=4,
  /* RMR { adding VIR, VFR, and VMR */
  VIR=5,
  VFR=6,
  VMR=7,
  /* } RMR */
  LAST_FILE=7,
  NUM_REG_FILE=8
};

extern Reg_file mdesname_to_vregfile(char vf);
extern char* vregfile_to_mdesname(Reg_file vrf);
extern char* operand_to_mdesname(Operand& o);

enum Reg_sr {
  STATIC_REG,
  ROTATING
};

extern Hash_map<Macro_name, eString> el_macro_to_string_map;
extern Hash_map<eString, Macro_name> el_string_to_macro_map;

extern void el_init_elcor_macro_maps(void);
extern void el_deinit_elcor_macro_maps();

unsigned hash_macro(Macro_name& i);
eString macro_reg_to_text (Macro_name macro);


//
//	Operand for generic branch opcodes used for control flow analysis (BRDIR)
//
enum General_Branch_Cond {
  COND_W_FALSE=1,
  COND_W_EQ,
  COND_W_LT,
  COND_W_LEQ,
  COND_W_GT,
  COND_W_GEQ,
  COND_W_SV,
  COND_W_OD,
  COND_W_TRUE,
  COND_W_NEQ,
  COND_W_LLT,
  COND_W_LLEQ,
  COND_W_LGT,
  COND_W_LGEQ,
  COND_W_NSV,
  COND_W_EV,
  COND_S_FALSE,
  COND_S_EQ,
  COND_S_LT,
  COND_S_LEQ,
  COND_S_GT,
  COND_S_GEQ,
  COND_S_NEQ,
  COND_S_TRUE,
  COND_D_FALSE,
  COND_D_EQ,
  COND_D_LT,
  COND_D_LEQ,
  COND_D_GT,
  COND_D_GEQ,
  COND_D_NEQ,
  COND_D_TRUE,
  COND_LAST	// Not used!!
};

Reg_file data_type_to_file (DType d) ;

class Operand ;
class Base_operand ;

//////////////////////////////////////////////////////////////////////////
//
// Symboltable_entry class (like and operand butprovides deep equality for symbol table)
//
//////////////////////////////////////////////////////////////////////////

class Symboltable_entry {
   friend ostream& operator<<(ostream& os, const Symboltable_entry& operand);
   friend unsigned hash_symboltable_entry(Symboltable_entry&) ;
   friend class Operand ;
public:
   Symboltable_entry() ;
   Symboltable_entry(const Symboltable_entry&) ;
   Symboltable_entry(Base_operand*) ;
   ~Symboltable_entry() ;
   
   Symboltable_entry& operator=(const Symboltable_entry&) ;
   bool operator==(const Symboltable_entry&) const;
   bool operator!=(const Symboltable_entry&) const;
   bool operator<(const Symboltable_entry&) const;
   
private:
   Base_operand * operand_ptr ;
};

/*
   ---------------------------------------------------------------------------
   Base_operand base class
   ----------------------------------------------------------------------------
   */

class Base_operand {
   friend ostream& operator<<(ostream& os, const
			      Base_operand& operand);
public:
   Base_operand(); Base_operand (const Base_operand&);
   virtual Base_operand* clone() const = 0; //  Polymorphic copy operation.
   virtual ~Base_operand();

   Base_operand& operator=(const Base_operand&);
   virtual bool operator==(const Base_operand&) const = 0;
   virtual bool operator!=(const Base_operand&) const;

   virtual void print_for_pqs(ostream&) const;

   virtual bool same_operand_class(const Base_operand&) const ;
   virtual bool is_identical(const Base_operand&) const = 0;

   virtual bool is_undefined() const;
   virtual bool is_reg() const;
   virtual bool is_vr_name() const;
   virtual bool is_mem_vr() const;
   virtual bool is_macro_reg() const;
   virtual bool is_lit() const;
   virtual bool is_int() const;
   virtual bool is_predicate() const;
   virtual bool is_predicate_true() const;
   virtual bool is_predicate_false() const;
   virtual bool is_float() const;
   virtual bool is_double() const;
   virtual bool is_string() const;
   virtual bool is_label() const; 
   virtual bool is_cb() const ;
   virtual bool assigned_to_file() const; // For literals,always returns true. 
   virtual bool allocated() const;        // For literals, always returns true.

   virtual int hash() const = 0 ;

   virtual int omega() const;
   virtual void set_omega(int) ;
   
   virtual DType dtype() const;
   virtual void set_dtype(DType);

   // Only for Reg, Macro_reg, VR_name, and Undefined asserts 0 for others
   virtual Reg_file file_type() const;

   // physical register file binding, for reg and lits, asserts 0 for others
   virtual eString physical_file_type() const ;
   virtual void bind_physical_file(eString phys_file) ;
   virtual void unbind_physical_file() ;
   virtual bool assigned_to_physical_file() const { return false; } ;

protected:
   virtual void print(ostream&) const = 0;
};


/*----------------------------------------------------------------------------
  Registers
  This class represents an EVR and its binding to a physical register. There
  is no separate class for just physical registers. 

  The class provides three types of equality operations. Equal_vr is used to
  check if two registers are the same virtual register (including omega).
  Equal_mc is used to check if they are the same physical register. The ==
  operator is somewhat unconvetional and encodes a frequently occuring test(I
  think).  If both registers are bound to physical registers, then == behaves
  like equal_mc. Otherwise, it behaves like equal_vr.

  Printed representation:  vr<int>[<omega>] :<file>
                          | <file><int> | <file>[<int>]
	  The first is used for vrs that are not bound to register. If omega =
	  0, then it is not printed. Similarly, if vr is not assigned to a
	  file, the file part is not printed. Second is used for vrs bound to
	  static registers, and the third for vrs bound to rotating registers.
----------------------------------------------------------------------------*/

class Reg  : public Base_operand {
public:
   Reg(DType);
   Reg(DType dt, int vr_number, int omega = 0);
   Reg(DType dt, Reg_file r_file, int vr_number);
   Reg(const Reg&);
   Reg(const class Operand&);  // initialize from Reg or VR_name
   Base_operand* clone() const;
   ~Reg();

   Reg& operator=(const Reg&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_reg() const;
   bool assigned_to_file() const;
   bool allocated() const;

   /* Class specific functions */

   bool equal_vr(const Base_operand&) const;
   bool equal_filevr(const Base_operand&) const;
   bool equal_mc(const Base_operand&) const;
      
   void set_dtype(DType) ;
   DType dtype() const;
   int vr_num() const;
   int omega() const;
   Reg_file file_type() const;
   bool is_static() const;
   bool is_rotating() const;
   int mc_num() const;
   Reg& rename();
   Reg& rename(int new_vr_rep);
   Reg& incr_omega (int incr = 1);
   Reg& bind_file(Reg_file, Reg_sr);
   Reg& bind_reg(int);
   Reg& unbind_reg();
   void set_vr_num(int);
   void set_omega(int) ;

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

   virtual int hash() const ;

protected:
   void print(ostream&) const;
   void print_for_pqs(ostream&) const;

private:
  DType dtype_rep;		// data type of register
  Reg_file file;		// register file identifier
  eString physical_file;	// physical register file identifier
  int vr_rep;			// virtual register number
  int omega_num;		// omega
  int history;			// number for tracking renamed registers
  bool assigned;		// assigend to a register file
  bool alloc;			// allocated to a machine register
  Reg_sr sr;			// flag for static/dynamic register 
  int mc_rep;			// machine register number
};

////////////////////////////////////////////////////////////////////////////
//
//  VR_name 
//  This class provides the operand for a remap operation. This operand 
//  defines the virtual register name part of an EVR
//
////////////////////////////////////////////////////////////////////////////


class VR_name  : public Base_operand {
public:
   VR_name(const Reg);
   VR_name(const Operand);
   VR_name(DType dt, int vr_number);
   Base_operand* clone() const;
   ~VR_name();

   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_vr_name() const;

   /* Class specific functions */

   bool same_name(const Base_operand&) const;
   DType dtype() const;
   void set_dtype(DType);
   int vr_num() const;
   VR_name& rename(const Reg&);
   void set_vr_num(int);

   virtual int hash() const ;

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

protected:
   void print(ostream&) const;

private:
   DType dtype_rep;			// data type of register
   int vr_rep;			// virtual register number
   eString physical_file;
};


/*----------------------------------------------------------------------------
 Mem_vr
  This class represents EVRs used for memory dependences. It is like Reg class
  except that these are not bound to physical registers.
  Mem_vrs are assumed to be assigned and allocated, i.e., the corresponding 
  queries always return TRUE.

  Printed representation:  vr<int>[<omega>]
                           If omega =  0, then it is not printed. 
----------------------------------------------------------------------------*/

class Mem_vr  : public Base_operand {
public:
   Mem_vr();
   Mem_vr(int vr_number, int omega = 0);
   Mem_vr(const Mem_vr&);
   Base_operand* clone() const;
   ~Mem_vr();

   Mem_vr& operator=(const Mem_vr&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_mem_vr() const;

   /* Class specific functions */
   int vr_num() const;
   int omega() const;
   Mem_vr& rename();
   Mem_vr& rename(int);
   Mem_vr& incr_omega (int incr = 1);
   void set_vr_num(int);
   void set_omega(int);

   virtual int hash() const ;
      
protected:
   void print(ostream&) const;

private:
   int vr_rep;			// virtual register number
   int omega_num;		// omega
   int history;			// number for tracking renamed registers
};

/* -------------------------------------------------------------------------
   Macro Registers  
   ala Lcode
   Printed representation: mr.<string>:<file>
---------------------------------------------------------------------------- */

class Macro_reg : public Base_operand {
public:
   Macro_reg();
   Macro_reg(Macro_name) ;
   Macro_reg(const Macro_reg&) ;
   Base_operand* clone() const;
   ~Macro_reg() ;
   
   Macro_reg& operator=(const Macro_reg&) ;
   bool operator==(const Base_operand&) const ;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   /* class specific functions */

   bool is_macro_reg() const ;
   bool is_fragile_macro() const ;
   bool is_predicate_true() const;
   bool is_predicate_false() const;

   bool is_global() const;
   bool is_hardwired() const;

   DType dtype() const;
   void set_dtype(DType);

   Reg_file file_type() const;
   Macro_reg& bind_file(Reg_file);

   Macro_name name() const;
   void set_name(Macro_name);

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

   // macros can now have physical register numbers. -KF 12/2004
   bool allocated() const;
   int mc_num() const;
   Macro_reg& bind_reg(int);
   Macro_reg& unbind_reg();

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;   

private:
   DType dtype_rep;			// data type of register
   Reg_file file;		// register file identifier   
   eString physical_file;	// physical register file identifier
   Macro_name name_rep;
   bool global_rep;   // global macro = one exists for the whole processor.
                      // otherwise there can be up to 1 per compatible reg file.  
   bool hardwired_rep; // hardwired macro = has constant value, cannot be changed
                       // by writes. ie FLT_ZERO

   // macros can now have physical register numbers. -KF 12/2004
   bool alloc;
   int mc_rep;
};
   


/* --------------------------------------------------------------------------
  Integer literals. 
   Printed represntation: I:<number>
--------------------------------------------------------------------------- */
class Int_lit : public Base_operand{

public:
  Int_lit(int);
  Int_lit(unsigned int);
  Int_lit(int,int width, bool sign);
  Int_lit(const Int_lit&);
  Base_operand* clone() const;
  ~Int_lit();
  
  Int_lit& operator=(const Int_lit&);
  bool operator==(const Base_operand&) const;
  
  bool same_operand_class(const Base_operand&) const ;
  bool is_identical(const Base_operand&) const ;
  
  bool is_lit() const;
  bool is_int() const;
  
  /*   bool is_shortlit() const;
       bool is_mediumlit() const;
       bool is_longlit() const; */
  
  /* Class specific function */
  int value() const;
  int int_value() const;
  void set_value(int); //this now adjusts the width automagically --sherwood 7/99
  void set_value(unsigned int); //added by sherwood 7/99
  DType dtype() const;
  void set_dtype(DType);

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

  virtual int hash() const ;
  
protected:
   void print(ostream&) const;

private:
   int rep;
   DType dtype_rep;		// Data type of literal
   eString physical_file;	// physical register file identifier
};

/* ----------------------------------------------------------------------------
   Predicate literals. 
   Printed representation: P:<number>
---------------------------------------------------------------------------- */
class Pred_lit : public Base_operand{

public:
   Pred_lit(bool);
   Pred_lit(const Pred_lit&);
   Base_operand* clone() const;
   ~Pred_lit() ;

   Pred_lit& operator=(const Pred_lit&);
   bool operator==(const Base_operand& op) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_lit() const;
   bool is_predicate() const;
   bool is_predicate_true() const;
   bool is_predicate_false() const;

   /* Class specific function */
   bool value() const;
   void set_value(bool);
   DType dtype() const;
   void set_dtype(DType);

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;

private:
   bool rep;
   DType dtype_rep;
};


/* --------------------------------------------------------------------------
  Single precision floating point literals. 
   Printed represntation: F:<number> 
--------------------------------------------------------------------------- */
class Float_lit : public Base_operand{
public:
   Float_lit(float);
   Float_lit(const Float_lit&);
   Base_operand* clone() const;
   ~Float_lit();

   Float_lit& operator=(const Float_lit&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_lit() const;
   bool is_float() const;

   /* Class specific function */
   float value() const;
   void set_value(float);
   DType dtype() const;
   void set_dtype(DType);

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;

private:
   float rep;
   DType dtype_rep;
   eString physical_file;	// physical register file identifier
};

/* --------------------------------------------------------------------------
  Double precision floating point literals. 
   Printed represntation: D:<number> 
--------------------------------------------------------------------------- */
class Double_lit : public Base_operand{
public:
   Double_lit(double);
   Double_lit(const Double_lit&);
   Base_operand* clone() const;
   ~Double_lit();

   Double_lit& operator=(const Double_lit&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_lit() const;
   bool is_double() const;

   /* Class specific function */
   double value() const;
   void set_value(double);
   DType dtype() const;
   void set_dtype(DType);

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;

private:
   double rep;
   DType dtype_rep;
   eString physical_file;	// physical register file identifier
};

/* --------------------------------------------------------------------------
  String literals. 
   Printed represntation: S:<string> 
--------------------------------------------------------------------------- */
class String_lit : public Base_operand{
public:
   String_lit(const eString&, DType_info dinfo = EL_DT_INFO_GLOBAL_ABS);
   String_lit(const String_lit&);
   Base_operand* clone() const;
   ~String_lit();

   String_lit& operator=(const String_lit&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_lit() const;
   bool is_string() const;

   /* Class specific function */
   eString value() const;
   void set_value(eString);
   DType dtype() const;
   void set_dtype(DType);

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;

private:
   eString rep;
   DType dtype_rep;		// Data type of string
   eString physical_file;
};

/* --------------------------------------------------------------------------
  Labels
   Printed represntation: L:<string> 
--------------------------------------------------------------------------- */
class Label_lit : public Base_operand {
public:
   Label_lit(const eString&,DType_info dinfo = EL_DT_INFO_GLOBAL_ABS);
   Label_lit(const Label_lit&);
   Base_operand* clone() const;
   ~Label_lit();

   Label_lit& operator=(const Label_lit&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_lit() const;
   bool is_label() const;

   /* Class specific function */
   eString value() const;
   void set_value(eString);
   DType dtype() const;
   void set_dtype(DType);

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;

private:
   eString rep;
   DType dtype_rep;		// Data type of label
   eString physical_file;
};

/* ---------------------------------------------------------------------------
   Control Blocks
   Print CB:<number>
---------------------------------------------------------------------------- */

class Cb_operand : public Base_operand{

public:
   Cb_operand(int);
   Cb_operand(const Cb_operand&);
   Base_operand* clone() const;
   ~Cb_operand();

   Cb_operand& operator=(const Cb_operand&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_lit() const;
   bool is_cb() const;

   /* Class specific function */
   int id() const;
   void set_id(int);
   DType dtype() const;
   void set_dtype(DType);

   // physical register file binding, for reg and lits
   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;

private:
   int id_rep;
   DType dtype_rep;		// Data type of label
   eString physical_file;
};
/* --------------------------------------------------------------------------
   Undefined operand class. 
   Printed representation: undefined?
--------------------------------------------------------------------------  */
class Undefined : public Base_operand {
public:
   Undefined();
   Undefined (const Undefined&);
   Base_operand* clone() const;
   ~Undefined();

   Undefined& operator=(const Undefined&);
   bool operator==(const Base_operand&) const;

   bool same_operand_class(const Base_operand&) const ;
   bool is_identical(const Base_operand&) const ;

   bool is_undefined() const;
   //undefined now returns size zero unsigned
   virtual DType dtype() const { return(EL_STANDARD_UNDEFINED); }

   virtual int hash() const ;
  
protected:
   void print(ostream&) const;
};

////////////////////////////////////////////////////////////////////////////
//  Operand class  :  An envelope for Base_operand
////////////////////////////////////////////////////////////////////////////

class IR_outstream;

class Operand {
   friend IR_outstream &operator<<(IR_outstream &out, const Operand &);
   friend ostream& operator<<(ostream& os, const Operand& operand);
public:
   Operand() ;
   Operand(const Operand&) ;
   Operand(Base_operand*) ;
   ~Operand() ;
   
   Operand& operator=(const Operand&) ;
   bool operator==(const Operand&) const;
   bool operator!=(const Operand&) const;
   bool operator<(const Operand&) const;

   void print_for_pqs(ostream&) const;
   
   bool same_operand_class(const Operand&) const ;
   bool is_identical(const Operand&) const ;

   bool is_undefined() const;
   bool is_reg() const;
   bool is_vr_name() const;
   bool is_mem_vr() const;
   bool is_macro_reg() const;
   bool is_lit() const;
   bool is_int() const;
   bool is_predicate() const;
   bool is_predicate_true() const;
   bool is_predicate_false() const;
   bool is_float() const;
   bool is_double() const;
   bool is_string() const;
   bool is_label() const;
   bool is_cb() const ;
   bool assigned_to_file() const; // For literals,always returns true.
   bool allocated() const;        // For literals, always returns true.
   Reg_file file_type() const;
   DType dtype() const;
   void set_dtype(DType t);
   int data_type_width() const;   // hardwired according to datatypes

   // Reg class methods
   bool equal_vr(const Operand&) const;
   bool equal_filevr(const Operand&) const;
   bool equal_mc(const Operand&) const;
   bool is_static() const;
   bool is_rotating() const;
   Operand& bind_file(Reg_file, Reg_sr);

   // Reg and Macro_reg class methods
   int mc_num() const;
   Operand& bind_reg(int);
   Operand& unbind_reg();
   
   // Reg and Memvr class methods
   int vr_num() const;
   int omega() const;
   int min_omega() const;
   int max_omega() const;   
   Operand& rename();
   Operand& rename(int);
   Operand& incr_omega (int incr = 1);
   void set_vr_num(int);
   void set_omega(int);

   // Vr_name methods
   bool is_same_name(const Operand&) const;
   
   // Macro_reg class methods
   Macro_name name() const;
   void set_name(Macro_name);
   bool is_fragile_macro();
   bool is_global() const;
   bool is_hardwired() const;
  
   // Cb class methods
   int id() const ;
   void set_id(int);
   
   // value mehods for literal classes
   bool pred_value() ;
   void set_pred_value(bool);
   int int_value() ;
   void set_int_value(int);
   double float_value() ;
   void set_float_value(double);
   double double_value() ;
   void set_double_value(double);
   eString string_value() ;
   void set_string_value(eString);
   eString label_value() ;
   void set_label_value(eString);


   eString physical_file_type() const;
   void bind_physical_file(eString phys_file);
   void unbind_physical_file();
   bool assigned_to_physical_file() const;
    
   void validate_and_point_to(Base_operand* base_ptr) ;

   inline Base_operand* get_ptr() const { return (sptr->optr) ; }

  // Global Symbol Table Management
  static void init_global_symbol_table();
  static void clear_global_symbol_table();

private:
   static Hash_map<Symboltable_entry,Symbol_table_info> global_symbol_table ;
   static Symbol_table_info* undefined_base ;
   Symbol_table_info* sptr ;
};


unsigned hash_operand(Operand& oprnd) ;
unsigned hash_operand_2(Operand& oprnd) ;
unsigned hash_symboltable_entry(Symboltable_entry& oprnd) ;




// Debug pointers (for use in gdb)
extern Operand *oper1, *oper2, *oper3 ;

#endif

