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
//      File:           region_attributes.h
//      Authors:        Sadun Anik, Sumedh Sathaye
//      Created:        December 1994
//      Description:    Region attributes 
//
//      Notes:          (Sathaye) Added the Reaching Defs attrib definition
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _REGION_ATTRIBUTES_H
#define _REGION_ATTRIBUTES_H

#include "tuples.h"
#include "string_class.h"
#include "list.h"
#include "map.h"
#include "vector.h"
#include "opcode.h"
#include "dtype.h"
#include "operand.h"
#include "hash_set.h"
#include "hash_map.h"
#include "attribute_types.h" 

typedef Map<eString,List<Operand> > Lcode_attribute_map;

class Lcode_attribute_map_attrib : public Base_attribute {
public:
  Lcode_attribute_map_attrib() ;
  ~Lcode_attribute_map_attrib() ;
      
  Base_attribute* clone_attribute() ;
  bool is_empty() { return (!map || map->is_empty()); }
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return LCODE_ATTRIBUTES; }
  bool is_compound_region_attribute() { return true; }
  bool is_op_attribute() { return true; }

  Lcode_attribute_map* map ;
} ;

Lcode_attribute_map* get_lcode_attributes(Region* r) ;
void set_lcode_attributes(Region* r, Lcode_attribute_map* map) ;
void remove_lcode_attributes(Region* r) ;

///////////////////////////////////////////////////////////////////

typedef Map<int,int> MemDepProf_attribute_map ;

class MemDepProf_attribute_map_attrib : public Base_attribute {
public:
  MemDepProf_attribute_map_attrib() ;
  ~MemDepProf_attribute_map_attrib() ;
      
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return MEM_DEP_PROF_ATTRIBUTES; }
  bool is_compound_region_attribute() { return true; }
  bool is_empty() { return (!map || map->is_empty()); }

  MemDepProf_attribute_map* map ;
} ;

MemDepProf_attribute_map* get_mdp_attributes(Region* r) ;
void set_mdp_attributes(Region* r, MemDepProf_attribute_map* map) ;
void remove_mdp_attributes(Region* r) ;

///////////////////////////////////////////////////////////////////

class Control_cpr_info : public Base_attribute {
public:
  Control_cpr_info();
  Control_cpr_info(int n_chains);
  ~Control_cpr_info();
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return CONTROL_CPR_INFO; }
  bool is_compound_region_attribute() { return true; }

  void set_on_trace_pred(int index, Operand &val);
  void set_off_trace_pred(int index, Operand &val);
  void set_derived_on_trace_pred(int index, Hash_set<Operand> &val);
  void set_derived_off_trace_pred(int index, Hash_set<Operand> &val);
  void add_derived_on_trace_pred(int index, Operand &val);
  void add_derived_off_trace_pred(int index, Operand &val);

  int num_chains;
  Vector<Operand> on_trace_preds;
  Vector<Operand> off_trace_preds;
  Vector<Hash_set<Operand> > derived_on_trace_preds;
  Vector<Hash_set<Operand> > derived_off_trace_preds;
};

Control_cpr_info* get_control_cpr_info(Region*);
void	          set_control_cpr_info(Region*, Control_cpr_info*);
void              remove_control_cpr_info(Region*);

///////////////////////////////////////////////////////////////////

// this attribute is used to override livein/liveout analysis of a
// region. The assumption is that all regs found to be live-in by
// analysis are live-in to the region and no regs are live out.  This
// map allows us to override these default settings.  The map is:
// virtual_reg # --> <livein-ness, liveout-ness>.  So if a reg is
// live-in and live-out, an entry is <1,1>.  If a reg is not in the
// map, it is still assumed to be <1,0>.
typedef Map<int,Pair<int,int> > Reg_to_liveness_map;

class Reg_to_liveness_map_attrib : public Base_attribute {
public:
  Reg_to_liveness_map_attrib() ;
  ~Reg_to_liveness_map_attrib() ;
      
  Base_attribute* clone_attribute() ;
  bool is_empty() { return (!map || map->is_empty()); }
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return REG_TO_LIVENESS_MAP; }
  bool is_compound_region_attribute() { return true; }

  Reg_to_liveness_map* map ;
} ;

Reg_to_liveness_map* get_reg_to_liveness_map(Region* r) ;
void set_reg_to_liveness_map(Region* r, Reg_to_liveness_map* map) ;
void remove_reg_to_liveness_map(Region* r) ;

///////////////////////////////////////////////////////////////////

// for counted loops with compile-time constant bound
class Loop_count_attrib : public Base_attribute {
public:
  Loop_count_attrib() ;
  ~Loop_count_attrib() ;
      
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return LOOP_COUNT; }
  bool is_compound_region_attribute() { return true; }
  bool is_inline() { return true; }

  int loopcount ;
} ;

int get_loop_count(Region* r) ;
void set_loop_count(Region* r, int lc) ;
void remove_loop_count(Region* r) ;

///////////////////////////////////////////////////////////////////
// Exsym Table

class Exsym_info;
typedef Hash_map<int, Exsym_info*> Exsym_table;
typedef Hash_map_iterator<int, Exsym_info*> Exsym_table_iterator;

// This attribute is used to keep an external source-code symbol
// table, a map from virtual register numbers to external source-code
// variables assigned to them in the frontend. The symbol information
// is kept as a separate class that may be derived for additional
// properties.
class Exsym_table_attrib : public Base_attribute {
public:
  Exsym_table_attrib() ;
  ~Exsym_table_attrib() ;
      
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return EXSYM_TABLE_ATTR; }
  bool is_compound_region_attribute() { return true; }

  Exsym_table* symtab ;
} ;

Exsym_table* get_exsym_table(Region* r) ;
void set_exsym_table(Region* r, Exsym_table* _symtab) ;
void remove_exsym_table(Region* r) ;

// Exsym Info

enum Exsym_type { BASE_EXSYM, SUIF_EXSYM };

// We want a virtual constructor for the external symbol info. Based
// on some input, or as a protocol, we wish to use either the base
// symbol_info constructor or one of its derived class constructors.
// The following implements a pseudo-virtual constructor mechanism
// (since C++ does not allow a truely virtual constructor). Basically
// we keep a static allocator function that will either allocate the
// right derived exsym class or do a dynamic dispatch. Then, the
// elements of that data structure are filled using a virtual read
// method.
class IR_instream;
class IR_outstream;
class Exsym_info {
public:
  Exsym_info();
  Exsym_info(eString _name, DType _dt, MEMORY_TYPE _type);
  virtual ~Exsym_info();
  virtual Exsym_type exsym_version() { return BASE_EXSYM; }
  virtual void scan_exsym_info(IR_instream&);  // defined in ir_attribute.cpp
  virtual void print_exsym_info(IR_outstream&);// defined in ir_attribute.cpp

public:
  static Exsym_info* exsym_info_allocator(Exsym_type version);

public:
  eString var_name;   // variable's source code name
  DType dt;           // changed width to DType -- Shail Aditya 11/29/2001
  MEMORY_TYPE type;   // the type of source code storage (literal, scalar, array etc.)
};

// RegMap - Suif symbol table and properties
/*
 * Shivarama Rao: Added the code on 06-06-01. The Symbol_info class is
 * used for storing the live in variables, their size and their reference type
 * the reference type can be any of scalar, literal or evr load.
 */
// Changed struct to a derived class of the Exsym_info in Elcor.
// Also added ASN spatial predicate information. 
// -- Shail Aditya 01/04/2002

class Suif_exsym_info : public Exsym_info {
public:
  Suif_exsym_info();
  Suif_exsym_info(eString _name, DType _dt, MEMORY_TYPE _type);
  virtual ~Suif_exsym_info();
  virtual Exsym_type exsym_version() { return SUIF_EXSYM; }
  virtual void scan_exsym_info(IR_instream&);  // defined in ir_attribute.cpp
  virtual void print_exsym_info(IR_outstream&);// defined in ir_attribute.cpp

public:
  int c_livein_base;
  int c_liveout_base; // lc = tmax - tmin + 1, so tmax = lc + tmin - 1.
  bool has_asn;       // this variable is a spatial predicate or not
  Vector<int> asn;    // ASN spatial predicate info
};

///////////////////////////////////////////////////////////////////

class Msched_attr : public Base_attribute {
public:
  Msched_attr() ;
  ~Msched_attr() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return MSCHED_ATTRIBUTES; }
  bool is_compound_region_attribute() { return true; }

  //Msched_info* msched_ptr ;
  int ii;
  int esc;
  List<Operand> liveness_gen ;
  List<Operand> liveness_kill ;
  List<Operand> down_exposed_use_gen ;
  List<Operand> down_exposed_use_kill ;
  List<Operand> up_exposed_def_gen ;
  List<Operand> up_exposed_def_kill ;
  List<Operand> down_exposed_def_gen ;
  List<Operand> down_exposed_def_kill ;
} ;

Msched_attr* get_msched_attr(Compound_region*);
void         set_msched_attr(Compound_region*, Msched_attr*);
void         remove_msched_attr(Compound_region*) ;

#endif
