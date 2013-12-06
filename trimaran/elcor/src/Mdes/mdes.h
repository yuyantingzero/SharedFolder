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
//      File:           mdes.h
//      Authors:        Mike Schlansker, Vinod Kathail, Shail Aditya
//      Created:        December 1994
//      Description:    The MDES database definition
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MDES_H
#define MDES_H

#include <stdio.h>
#include <iostream>
#include <deque>
#include "defs.h"
#include "vector.h"
#include "matrix.h"
#define MD_DEBUG_MACROS
#include "md.h"
#include "intf.h"
#include "operand.h"
#include "macro.h"

using namespace std;

extern int IOCODE_SIZE;

// this is 5 sources and 1 predicate. Note, if you want to change
// this, the Operation_Latency section (specifically rsv and src) in
// hpl_pd_pristine.hmdes2 must be changed so that there are
// MDES_maxin-1 entries.
#define MDES_maxin 6
#define MDES_maxout 5
#define MDES_maxsynchin 2
#define MDES_maxsynchout 2
// the formula for MDES_latencies_per_class seems to be:
// 1+maxin+2*(maxout-1)+maxsyncin+maxsyncout (see Lat_descr::set_lat_descr)
#define MDES_latencies_per_class 19

class MDES;
class MdesHashtable;
class Op_descr;
class Op_list;
class Alt_descr;
class Alt_list;
class Ft_descr;
class Io_descr;
class Io_list;
class Res_descr;
class Res_use_descr;
class Reg_descr;
class Lat_descr;
class Rmsl;
class RU_mgr;
class StackDescr;

class Custom_op_descr;
class Pattern_node;
class Pattern_edge;

class Const_set_descr;

enum MDES_kind {MDES_PHYSICAL, MDES_VIRTUAL, MDES_MINI};
enum IO_kind {IO_PHYSICAL, IO_VIRTUAL};
enum OpLevel {UNITOP, CLSTROP, GENERICOP, MACROOP};

enum OpClass {NULLOP, LOADOP, STOREOP, BRANCHOP, ARITHOP, CUSTOMOP};

enum ResClass { RES_NULL, RES_INT, RES_FLOAT, RES_MEM, RES_BRANCH, RES_LMEM, RES_MAX };

enum PatternNodeType {COP_LIVE_IN, COP_LIVE_OUT, COP_LITERAL, COP_OPERATION};

//stack descriptor
enum StackDir {HIGH_TO_LOW, LOW_TO_HIGH};

// print support functions
OpLevel oplevel(char*);
char* oplevel(OpLevel);
OpClass opclass(char*);
char* opclass(OpClass);
ResClass resclass(char*);
char* resclass(ResClass);
bool eagerclass(char*);
char* eagerclass(bool);

// MD extensions
MD* El_read_md(char* mdfile);         // Read and check a HMDES2 file 
int get_entry_index (MD_Entry *entry);// obtain the index of an MD entry 

// extern FILE *MDES_out;
// void push_MDES_out(FILE*);
extern ofstream *MDES_out;
void push_MDES_out(ofstream*);
void pop_MDES_out(void);


/////////////////////////////////////////////////////////////////////////////
// MDES data structures

// Added a Field Type descr table that keeps the iocode of each field
// type. This is useful in virtual or mini mdes where there are no
// register descriptors -- Shail Aditya 12/11/96

class MDES{
  friend class Mdes_writer;
  friend class Io_descr;
  friend ostream& operator<<(ostream& os, const MDES& md);
public:
  MDES(int hashsize = 16384);       // simple constructor
  MDES(MDES& mdes);                // copy internal tables, if present
  virtual ~MDES(void);

  // Common methods among all subclasses
  virtual MDES_kind get_MDES_kind(void) { return MDES_PHYSICAL; };
  char* get_MDES_input_file(void) const;
  int get_MDES_num_resources(void) const;
  int get_MDES_num_res_use(void) const;
  int get_MDES_num_reg_types(void) const;
  int get_MDES_num_lat_classes(void) const;
  int get_MDES_num_io_formats(void) const;
  int get_MDES_num_field_types(void) const;
  int get_MDES_num_vreg_types(void) const;
  int get_MDES_num_const_sets(void) const;
  MdesHashtable* get_MDES_hashtable(void) const;
  Lat_descr* get_MDES_lat_descr(int index) const;
  Reg_descr* get_MDES_reg_descr(int index) const;
  Reg_descr* get_MDES_reg_descr(const char* prf) const;
  Res_descr* get_MDES_res_descr(int index) const;
  Res_use_descr* get_MDES_res_use_descr(int index) const;
  Io_descr* get_MDES_io_descr(int index) const;
  Ft_descr* get_MDES_ft_descr(int index) const;
  Const_set_descr* get_MDES_const_descr(const char* cset) const;
  bool is_macro_present(eString mn) const;
  bool is_macro_present(Macro_name mn) const;
  Hash_set<eString>& get_MDES_pf_for_macro(Macro_name mn) const;
  Hash_set<eString>& get_MDES_pseudo_macros();
  Hash_set<eString>& get_MDES_read_only_macros();
  Hash_set<eString>& get_MDES_alias_macros(void);
  Hash_set<eString>& get_MDES_multialias_macros(void);
  Hash_set<eString>& get_MDES_coherent_macros(void);
  eString get_macro_aliased(eString);
  
  bool is_pv_macro(Macro_name);

  bool is_pseudo_macro(eString);
  bool is_read_only_macro(eString);
  bool is_alias_macro(eString);
  bool is_multialias_macro(eString);
  bool is_coherent_macro(eString);
  
  // returns the register number given a physical file name
  int get_MDES_reg_number_for_pf(eString pf);

  unsigned get_num_custom_ops(void) const;
  Custom_op_descr* get_MDES_custom_op_descr(int index) const;
  int find_MDES_custom_op_descr_index(char *name);
  
  void set_MDES_input_file(char* file);
  void set_MDES_num_vreg_types(int numvreg);
  void set_MDES_lat_descr(int index, Lat_descr*);
  void set_MDES_reg_descr(int index, Reg_descr*);
  void set_MDES_res_descr(int index, Res_descr*);
  void set_MDES_res_use_descr(int index, Res_use_descr*);
  void set_MDES_io_descr(int index, Io_descr*);
  void set_MDES_ft_descr(int index, Ft_descr*);
  void set_MDES_custom_op_descr(int index, Custom_op_descr*);
  void add_MDES_const_set_descr(eString, Const_set_descr*);

  void resize_MDES_lat_table(int);
  void resize_MDES_reg_table(int);
  void resize_MDES_res_table(int) ;
  void resize_MDES_res_use_table(int);
  void resize_MDES_io_table(int) ;
  void resize_MDES_ft_table(int) ;
  void resize_MDES_custom_op_table(int);

  // find virtual or physical RF descr by name or by iocode
  int find_MDES_rf_index(char* name, IO_kind kind, bool flag_not_found=true);
  int find_MDES_rf_index(Bitvector& iocode, IO_kind kind, bool flag_not_found=true);
  void find_insert_MDES_ft_descr(Bitvector& iocode);

  // direct access functions
  void collect_io_descrs(char* opcode, List_set<Io_descr*>& descset);
  void collect_reg_descrs(char* opcode, IO_Portkind portkind, int portnum,
        List_set<Reg_descr*>& descset);
  void collect_reg_descrs(Op_descr* operation, IO_Portkind portkind, int portnum,
        List_set<Reg_descr*>& descset);
  void collect_ft_descrs(char* opcode, IO_Portkind portkind, int portnum,
       List_set<Ft_descr*>& descset);
  void collect_op_descrs(char* opcode, List_set<Op_descr*>& descset);
  void collect_alt_descrs(char *opcode, List_set<Alt_descr*>& descset);
  void collect_res_use_descrs(char *opcode, List_set<Res_use_descr*>& descset);

  Op_descr *collect_generic_op_descrs(char* opcode);

  // stack descriptor
  StackDir getStackDir();
  bool isRetAddrThruStack();
  unsigned int getRetAddrSize();
  unsigned int getAlignment();
  
  // print functions
  void print(void) const;
  void print_stream(ostream& os) const;

  // The following methods are defined separately as part of mdes reader
  MDES(char *mdes_input, char *mdes_output = "/dev/null", int hashsize = 16384);
  virtual void build_resource_table(MD* md);
  virtual void build_reservation_table(MD* md);
  virtual void build_latency_table(MD* md);
  virtual void build_register_table(MD* md);
  virtual void build_macro_table(MD* md);
  virtual void build_stack_descr_table(MD* md);
  virtual void build_iodescr_table(MD* md, IO_kind kind=IO_PHYSICAL);
  virtual void build_ftdescr_table(MD* md, IO_kind kind=IO_PHYSICAL);
  virtual void build_opdescr_table(MD* md);

  virtual void build_custom_op_table(MD* md);
  virtual void build_custom_opdescr_table(MD* md);
  virtual void build_const_set_table(MD* md);

  virtual Res_descr* get_res_descr(char* res_name);

  virtual void check_mdes();
  
private:
  // match full iocode or its subsets, iocodeptr is set to the
  // remaining unmatched iocode
  char* find_MDES_rf_name(const Bitvector& iocode, IO_kind kind, Bitvector *iocodeptr=NULL);
  // match from the beginning of the iospec, numcharptr returns the
  // number of chars matched
  Bitvector find_MDES_rf_iocode(char* iospec, IO_kind kind, int* numcharptr=NULL);

protected:
  char *input_file;
  MdesHashtable* hashtable;
  Vector<Lat_descr*> lat_descr_tab;         // latency classes 
  Vector<Reg_descr*> reg_descr_tab;         // physical register files
  Vector<Macro_descr*> macro_descr_tab;     // macros
  Vector<Res_descr*> res_descr_tab;         // resources 
  Vector<Res_use_descr*> res_use_descr_tab; // reservation tables
  Vector<Io_descr*> io_descr_tab;           // io formats
  Vector<Ft_descr*> ft_descr_tab;           // field types 
  int num_vreg_types;                       // virtual register files
  Vector<Custom_op_descr*> custom_op_tab;
  Hash_map<eString, int> macro_to_index_map;// index into macro_descrs

  // Stack specific stuff
  StackDescr *stack_descr_tab;

  Hash_map<eString, Res_descr*> res_str_to_descr_map;
  Hash_map<eString, Const_set_descr*> name_to_const_desc_map;
  Hash_map<Macro_name, Hash_set<eString> > macro_to_pf_map;
  Hash_set<eString> alias_macros;
  Hash_set<eString> pseudo_macros;
  Hash_set<eString> read_only_macros;
  Hash_set<eString> multialias_macros;
  Hash_set<eString> coherent_macros;

};

class MdesHashtable{
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const MdesHashtable& table);
public:
  MdesHashtable(MDES* mdes, int modu);
  MdesHashtable(MdesHashtable& table); // copy constructor, shares op descr

  MDES* get_mdes() const;
  void set_mdes(MDES* mdes);
  void clear();
  int hash(char string[]);
  int insert_op(char string[],Op_descr *location);
  Op_descr* find_op(char string[]);    // return pointer to op descr
  void init_next_op();
  Op_descr* get_next_op();
  int get_num_ops() const;

  // print functions
  void print(void) const;
  void print_stream(ostream& os) const;

private:
  MDES* md;
  int modulus;
  int numops;
  Vector<char*> htable;
  Vector<Op_descr*> htable_ptr;
  int index;
  void expand();                       // Double the size of the hashtable
};

class Op_descr{
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const Op_descr& desc);
public:
  Op_descr(MDES* mdes);
  void fill_op(char *lname, char * aname, Io_list *io_list_alt,
         OpLevel binding, int numin, bool predin, int numout, 
         OpClass opclass, ResClass resclass, bool eager, int priority);
   
  void io_list_init(void);      /*initialize for io list iteration*/
  Io_list *get_next_io(void);   /*gets io list element*/
  char * get_lname_ptr(void);   /*returns lcode (compiler) name*/
  char * get_aname_ptr(void);   /*returns assembly (architecture) name*/
  int get_priority(void);       /*returns the priority of an operation*/
  OpClass get_opclass(void);/*returns operation class */
  OpLevel get_binding(void);    /*returns operation binding*/
  ResClass get_resclass(void);  /*return resource class*/
  int get_num_in(void);         /* returns number of non-pred inputs */
  int get_num_out(void);        /* returns number of non-pred outs */
  bool is_predicated(void);     /* returns whether op is predicated or not */
  bool has_spec(void);/* returns whether op has speculative version*/

  MDES* get_mdes() const;
  void set_mdes(MDES* mdes);

  // print an operation descriptor
  void print(void) const;
  void print_stream(ostream& os) const;

  // Update the entire iolist structure
  void set_io_list(Io_list* iolist);
  // add a list of alternatives at a given iodescr
  // the order of the given alternatives is preserved 
  void add_alts(Io_descr* iodesc, Alt_list* alts);
  // search for the alternative that match a given Io_descr and a
  // reservation pattern.  
  Alt_descr* find_alt(Io_descr* iod, Res_use_descr* resv);

  // op encodings are a temporary hack. Ultimately a separate
  // pass should assign proper encodings based on AVLIW name.
  // Shail Aditya - 07/24/96
  int get_encoding(void);
  void set_encoding(int enc);

  // Shivarama Rao K - 09-02-2001
  // Added the functions to return additional fields of an op descriptor
  bool is_commutative(void);     /* returns whether op is commutative or not */
  bool is_associative(void);     /* returns whether the op has associative property */
  bool is_advanced_load(void);   /* returns true if the op is of advanced load type*/
  bool is_le_op(void);           /* returns true if the op has less-than-equals semantics */
  bool is_two_operand(void);     /* returns true if the op has dest = dest + src ioformat */
  bool is_unsupported(void);     /* returns true if the op is not supported in the target architecture */
  
  // functions to set the fields `commutative', `associative' `advanced load' and `le_op'  
  void set_commutative(bool comm);      /* sets the commutative property of op as `comm' */
  void set_associative(bool assoc);     /* sets the associative property of op as `assoc' */
  void set_advanced_load(bool adv);     /* sets the advanced load property of op to `adv' */
  void set_le_op(bool le_op);           /* sets the le_op field to `le_op' */
  void set_predicated(bool pred);       /* sets the predicate property of op as `pred' */
  void set_two_operand(bool two_operand);       /* sets the two operand property as 'two_operand' */
  void set_unsupported(bool unsupported);       /* sets the unsupported property as 'unsupported' */

private:
  MDES* md;             /*MDES affiliation*/
  char * Lname;         /*Lcode name */
  char *Aname;          /*AVLIW name*/
  bool name_eq;         /*Lcode name same as AVLIW name*/
  bool pred;            /* needs predicate input*/
  int num_input;        /* number of non predicate inputs */
  int num_output;       /* number of non predicate outputs*/
  Io_list *io_ptr;      /*ptr into io list table*/
  Io_list *curr_io;     /*pointer to current io list alternative*/
  Lat_descr *lat_ptr;  /*pointer into table of latency descriptors*/
  OpLevel op_binding;  /* 0-unit specific,1-cluster specific ,2-generic, 3-macro */
  OpClass op_class;     /* 0-null, 1-load, 2-store, 3-branch, 4-arith*/
  ResClass res_class;   /* 0-int, 1-float, 2-mem, 3-branch*/
  int op_priority;      /*operation (port) priority - for sequential semantics*/
  int encoding;         /*encoding of arch. op (temporary soln.)*/
  Op_descr *rev_br;     /*ptr to reverse branch */
  bool spec_version;    /*has a speculative version*/
  bool less_than;       /* less than or equals op*/
  bool is_adv_load;     /*this is an advanced load operation*/
  bool adv_load;        /*must be one for advanced load*/
  bool assoc;           /*associative operation*/
  bool comm;            /*commutative operation*/
  bool two_operand;     /*two operand*/
  bool unsupported;     /*unsupported*/
};

unsigned hash_Op_descr_ptr(Op_descr*& i) ;

class Pattern_edge {
public:
  Pattern_edge(MDES* mdes);
  ~Pattern_edge();
  void add_src(Pattern_node*);
  void add_dest(Pattern_node*);
  unsigned num_srcs() const;
  unsigned num_dests() const;
  Pattern_node* get_src(unsigned) const;
  Pattern_node* get_dest(unsigned) const;

private:
  MDES* md;

  // These are edges in the hypergraph sense, where multiple srcs and
  // dests are possible. You can equate a pattern edge with an
  // operand.
  deque<Pattern_node*> srcs;
  deque<Pattern_node*> dests;
};


unsigned hash_pattern_node_ptr(Pattern_node*& v);

class Pattern_node {
  friend bool node_compat(void*, void*);
  friend bool node_compat(Pattern_node*, Pattern_node*);
  friend ostream& operator<<(ostream& os, const Pattern_node& pattern);
 public:
  Pattern_node(MDES* mdes);
  ~Pattern_node();

  void add_opcode(const eString& opc);
  bool is_opcode_supported(const eString& opc) const;
  const Hash_set<eString>& get_opcodes() const { return opcodes; }
  void set_pred(Pattern_edge* predicate);
  void set_src(Pattern_edge* src, unsigned idx);
  void set_dest(Pattern_edge* dest, unsigned idx);
  void set_type(PatternNodeType t);
  void set_const_descr(Const_set_descr* cd);

  Pattern_edge* get_pred() const;
  Pattern_edge* get_src(unsigned n) const;
  Pattern_edge* get_dest(unsigned n) const;
  unsigned num_srcs() const;
  unsigned num_dests() const;
  PatternNodeType get_type() const { return type; }
  Const_set_descr* get_consts() const { return consts; }

  bool is_live_in() const;
  bool is_live_out() const;

  bool is_valid(int n) const;

 private:
  MDES* md;
  PatternNodeType type;

  // For nodes that perform computation
  Hash_set<eString> opcodes;
  Pattern_edge* pred;
  Vector<Pattern_edge*> srcs;
  Vector<Pattern_edge*> dests;

  // For nodes that are literals
  Const_set_descr* consts;
};


unsigned hash_custom_op_descr(Custom_op_descr*& v);

class Custom_op_descr {
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const Custom_op_descr& pattern);
 
 public:
  Custom_op_descr(MDES* mdes);
  ~Custom_op_descr();

  void set_opcode(char* opcode);
  void set_pattern_size(unsigned size);
  void add_pattern_node(Pattern_node* node, unsigned index);

  eString get_opcode() const;
  unsigned get_pattern_size() const;
  Pattern_node* get_pattern_node(unsigned idx) const;

 private:
  MDES* md;
  eString opcode;
  Vector<Pattern_node*> nodes;
  
};

class Alt_descr{   /*alternative descriptor*/
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const Alt_descr& desc);
public:
  Alt_descr(MDES* mdes, Op_descr *opdesc, Lat_descr *latdesc, Res_use_descr *resdesc);

  MDES* get_mdes() const;
  void set_mdes(MDES* mdes);
  Op_descr *get_op(void);
  Lat_descr *get_lat(void);
  Res_use_descr *get_res_use(void);

  // print an Alt descriptor
  void print(void) const;
  void print_stream(ostream& os) const;

private:
  MDES* md;                       /*MDES affiliation*/
  Op_descr *op;              /*pointer into table of operations*/
  Lat_descr *lat;            /*pointer into table of latency descriptors*/
  Res_use_descr *res;            /*pointer into table of resource usage descriptors */
};

class Alt_list{                    /*alternative list element*/
  friend class Mdes_writer;
public:
  Alt_list(Alt_descr *altd, Alt_list* nxt=NULL, int priority=0);

  Alt_descr *get_alt(void);
  Alt_list *get_next(void);
  int get_alt_priority(void);

  Alt_list *merge(Alt_list* altlist);  /* merge lists in priority order*/
  Alt_list *copy();                    /* copies this list : shares descs */
private:
  Alt_descr *alt;                 /* ptr to alternative descriptor*/
  Alt_list *next;                 /* ptr to next alternatives list element*/
  int alt_priority;                    /* priority order of alternatives*/
};

class Ft_descr{         /*IO field descriptor*/
  friend class Mdes_writer;
public:
  Ft_descr();
  Ft_descr(MDES* mdes, IO_kind kind=IO_PHYSICAL);

  MDES* get_mdes() const;
  char* get_name() const;
  const Bitvector& get_iocode() const;
  IO_kind get_iokind() const;
  bool match_ioset(Bitvector& ioset) const;
  bool operator==(const Ft_descr&) const; /* equality test ignores name */
  bool operator!=(const Ft_descr&) const; 

  void set_mdes(MDES* mdes);
  void set_iokind(IO_kind kind);
  void set_ft_descr(char* ftname, Bitvector& ioc);

private:
  MDES* md;                   /* MDES affiliation*/
  char* name;                 /* name of the field */
  Bitvector iocode;           /* its packed encoding for io_descr */
  IO_kind iokind;             /* kind of field - physical or virtual RF */
};

class Io_descr {         /*input output operand descriptor*/
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const Io_descr& desc);
 public:
  Io_descr();
  ~Io_descr();
  Io_descr(const Io_descr&);
  Io_descr(MDES* mdes, IO_kind _iokind=IO_PHYSICAL, 
  bool _pred=false, int _numin=0, int _numout=0);

  bool operator==(const Io_descr&) const; /* full field equality test */
  bool operator!=(const Io_descr&) const; 
  void parse_iospec(char *iospec);
  bool test_io(Io_descr *request);
  Bitvector get_iocode(IO_Portkind portkind, int portnum);
  void collect_reg_descrs(IO_Portkind portkind, int portnum,
  List_set<Reg_descr*>& descset);
  void collect_ft_descrs(IO_Portkind portkind, int portnum,
        List_set<Ft_descr*>& descset);

  MDES* get_mdes() const;
  int get_num_in(void);
  int get_num_out(void);
  bool is_predicated(void);
  const Bitvector& get_pred() const;
  const Bitvector& get_in_reg(int idx) const;
  const Bitvector& get_out_reg(int idx) const;
  IO_kind get_iokind() const;

  void set_mdes(MDES* mdes);
  void set_numin(int n);
  void set_numout(int n);
  void set_in_reg(int idx,Bitvector& v);
  void set_out_reg(int idx,Bitvector& v);
  void set_pred(Bitvector& v);
  void set_iokind(IO_kind kind);

  // print an IO descriptor
  void print(void) const;
  void print_stream(ostream& os) const;

 private:
  MDES* md;                   /* MDES affiliation*/
  int numin;
  int numout;
  Vector<Bitvector> in_reg_id;      /* allowed input operands (union of alternatives)*/
  Vector<Bitvector> out_reg_id;     /* allowed output operands (union of alternatives)*/
  bool pred;                  /* specifies whether operation is predicated */
  Bitvector pred_id;                /* allowed predicate operand (for completeness)*/
  IO_kind iokind;             /* kind of IO descr - physical or virtual RF */ 
};

unsigned hash_Io_descr_ptr(Io_descr*& v) ;

class Io_list{         /*list of  io description alternatives */
  friend class Mdes_writer;
public:
  Io_list(Io_descr *iodesc, Alt_list *altlist, Io_list *nxt=NULL);

  void init_alt(void);
  Io_descr *get_iod(void);
  Alt_list *get_altlist(void);
  Alt_descr *get_next_alt(void);
  Io_list *get_next(void);

  void set_altlist(Alt_list* altlist);
  // Find the Io_list cell matching iodesc. If found, concatenate to
  // its altlist and return that cell. Otherwise, create a new cell at
  // the end of the list, initialize it, and return it.
  Io_list *find_or_insert(Io_descr *iodesc, Alt_list *altlist);

private:
  Io_descr *io;        /*ptr to io descriptor*/
  Alt_list *alt;  /*index into table of alternative lists*/
  Io_list *next;  /*ptr to next record*/

  Alt_list *curr_alt;   /*pointer to current alternative*/
};

class Res_descr{     /*resource descriptor*/
  friend class Mdes_writer;
public:
  Res_descr(MDES* mdes);

  MDES* get_mdes() const;
  void set_mdes(MDES* mdes);
  void set(char* name, int count, bool v);
  void set_width(int width);
  char *get_name(void);
  int get_count(void);
  int get_width(void);
  // parkhc 03/17/2004
  bool is_virtual_res();

private:
  MDES* md;          /*MDES affiliation*/
  char * res_name;   /*resource name correspinding to res_name[inx]*/
  int res_count;     /*it indicates the number of identical resources*/
  int res_width;
  // parkhc 03/17/2004
  bool is_virtual;
};

class Res_use_descr{  /*resource usage descriptor*/
  friend class Mdes_writer;
public:
  Res_use_descr(MDES* mdes);

  bool operator==(const Res_use_descr&) const; /* complete reservation equality test */
  bool operator!=(const Res_use_descr&) const; 

  MDES* get_mdes() const;
  int get_res() const;
  int get_time() const;
  int get_count() const;
  Res_use_descr *get_next() const;
  void set_mdes(MDES* mdes);
  void set_res_use(int res_index, int time, int count=1);
  void set_next(Res_use_descr *res);

private:
  MDES* md;             /*MDES affiliation*/
  int res;    /*index of resource used*/
  int rel_time;    /*relative time at which resource is used*/
  int res_count;        /*number of resources of this kind used*/
  bool pred;    /*this usage is predicated*/
  Res_use_descr *next;  /*ptr to next resusage item for this pattern*/
};

class Const_set_descr{
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const Const_set_descr& desc);
public:
  Const_set_descr(MDES* mdes);
  void set_mdes(MDES* mdes);
  void set_name(const eString& s);
  void set_mins(const Vector<int>& v);
  void set_maxes(const Vector<int>& v);
  void set_steps(const Vector<int>& v);
  MDES* get_mdes() const;
  eString get_name() const;
  unsigned get_size() const;
  bool is_valid(int n) const;
  bool overlaps(Const_set_descr* csd);

  // print a const set descriptor
  void print(void) const;
  void print_stream(ostream& os) const;

private:
  MDES* md;             /*MDES affiliation*/
  eString name;
  Vector<int> mins;
  Vector<int> maxes;
  Vector<int> steps;
};

enum Reg_kind { REG_UNDEFINED, REG_PHYSICAL, 
    REG_LITINT, REG_LITINTRANGE, REG_LITDOUBLE };

class Reg_descr{        /* register descriptor */
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const Reg_descr& desc);
public:
  Reg_descr(MDES* mdes);
  void set_mdes(MDES* mdes);
  void set_reg(char* vnam, char* anam, int scap, 
         int rcap, int wid, int vlen, bool tag, int ioc);

  MDES* get_mdes() const;
  char* get_vname() const;
  char* get_aname() const;
  int get_scap() const;
  int get_rcap() const;
  int get_width() const;
  /* RMR { */
  int get_vector_length() const;
  /* } RMR */
  bool get_tag() const;
  Bitvector get_iocode() const;
  int get_io_id() const; // KF 9-2004
  IO_kind get_iokind() const { return IO_PHYSICAL; };
  Reg_kind get_regkind() const;
  bool match_ioset(Bitvector& ioset) const; 

  // print a register descriptor
  void print(void) const;
  void print_stream(ostream& os) const;

  // Methods for literal register files
  bool is_undefined() const;
  bool is_lit() const; 
  bool is_intlit() const;
  bool is_doublelit() const;
  bool is_valid(int litval) const;
  bool is_valid(double litval) const;

  void set_regkind(Reg_kind kind);
  void set_ilits(Const_set_descr* lits);
  void set_dlits(Vector<double>* vect);
  Vector<double>* get_dlits();
  Const_set_descr* get_ilits();

  bool is_caller_save(eString);
  bool is_callee_save(eString);
  bool is_macro(eString);

  void set_caller_save(eString);
  void set_callee_save(eString);
  void add_macro(eString);

  inline bool donot_allocate() const {return do_not_allocate;}
  inline void set_donot_allocate(bool flag) {do_not_allocate = flag;}

  Hash_set<eString>& get_caller_save();
  Hash_set<eString>& get_callee_save();
  void get_macros(Hash_set<eString>& macros);
  Hash_set<eString>& get_physical_regs(eString macro);
  Hash_set<eString>& get_physical_regs(Macro_name mn);

  void set_physical_reg_map(eString, eString);
  Hash_map<eString, Hash_set<eString> >& get_physical_reg_map();

private:
  MDES* md;             /*MDES affiliation*/
  char* vname;          /* virtual mnemonic for reg file type*/
  char* aname;          /* assembler mnemonic for reg file*/
  int s_cap;    /* number static of registers in file */
  int r_cap;    /*number of rotating registers in file*/
  int width;    /* width in bits */
  /* RMR { */
  int vector_length;    /* vector length */
  /* } RMR */
  bool has_tag;          /*has an exception tag bit */
  int iocode;           /*IO encoding for the register file*/
                        /* note, this is an id, not a bitvector. -KF 9-2004 */
  Reg_kind regkind;     /* tag to classify register files */
  Vector<double>* dlits;/* double literal specifier */
  Const_set_descr* ilits; /* specifies supported int literals */

  bool do_not_allocate;

  Hash_set<eString> caller_save; /* caller-save registers */
  Hash_set<eString> callee_save; /* callee-save registers */
  Hash_set<eString> macros;      /* macro registers */
  Hash_map<eString, Hash_set<eString> > physical_register_map;
};


unsigned hash_Reg_descr_ptr(Reg_descr*& v) ;

class Lat_descr{   /*latency descriptor*/
  friend class Mdes_writer;
  friend ostream& operator<<(ostream& os, const Lat_descr& desc);
public:
  Lat_descr(MDES* mdes);

  bool operator==(const Lat_descr&) const; /* test all fields except mdes */
  bool operator!=(const Lat_descr&) const; 

  MDES* get_mdes() const;
  void set_mdes(MDES* mdes);
  void set_lat_descr(int* lat_array);
  void get_lat_descr(int * lat_array);
  int get_flow_lat(IO_Portkind portkind, int portnum);
  int get_anti_lat(IO_Portkind portkind, int portnum);

  // print a latency descriptor
  void print(void) const;
  void print_stream(ostream& os) const;

private:
  MDES* md;             /*MDES affiliation*/
  int exception;  /*exception time (maximum across alternatives) */
  int rsv[MDES_maxout]; /*earliest output allocation time (minimum)*/
  int di[MDES_maxin];  /*sampling time for input operand i1 (minimum)*/
  int si[MDES_maxsynchin]; /*sampling time for synch operand i1 (minimum)*/
  int dox[MDES_maxout];     /*result availability time for result r1 (maximum)*/
  int so[MDES_maxsynchout];/*synch availablity time  (maximum)*/
};

//////////////////////////////////////////////////////////////////////
// Resource Usage Management

class RU_mgr {
public:
  RU_mgr();
  RU_mgr(MDES* mdes, int length, int num_resources);
  ~RU_mgr();
  void init(bool mode, int length);

  void init_op(char* opcode, void *op, char *io, int input_time);
  bool next_alt(char **opcode, int *priority);
  bool get_next_nonconfl_alt(char **opcode, int *priority);
  void get_conflicting_ops(Hash_set<void*>& ops);
  void place(void);
  void remove(void *op, int sched_time);
  void at(int cycle, int res_index, List<void*>& ops);

  void print(void);
  void print_stream(FILE *mout);
  int cur_sched_length();
  // KVM : Check freeness at time sched_time
  bool check_resources_free(Res_use_descr *res_use_in,int sched_time);

private:
  MDES* md;
  // Global scheduling state
  Matrix<List<void*> > ru_map; // The time X resource reservation map
                               // keeps a list of operations using each slot
  int sched_length;            // length for dag schedule
  bool loop_mode;              // dag=false, loop=true
  int modulus;                 // maintains II for modulo-scheduling

  // Operation scheduling state
  Op_descr* operation;         // MDES operation to schedule
  Io_descr request_iod;        // requested IO binding to schedule with
  int opsched_time;            // requested time to schedule at
  void* op_ptr;                // the back pointer to the Graph op
  Io_list* iol_elem;           // keep track of next alternative to look at
  Io_descr* iod_elem;          // keep track of next alternative to look at
  Alt_descr* altd_elem;        // keep track of next alternative to look at
  bool io_match;               // whether any matching IO descriptor was found

  bool check_io;

  // scheduling helper fns
  bool check_resources_free(Res_use_descr *res_use_in);
  void collect_conflicting_ops(Res_use_descr *res_use_in, 
             Hash_set<void*>& users);
};

//////////////////////////////////////////////////////////////////////
// Resource Minimum Schedule Length Calculation

class Rmsl{            // used to query resource effects on min time schedules
public:
  Rmsl(MDES* mdes, int lbound=0);
  ~Rmsl();
  MDES* get_mdes() const;
  void set_mdes(MDES* mdes);
  void init(int lbound=0);
  void nextop_io(char* opcode, char* io);
  int value(void);

  void print(void);
  void print_stream(FILE* mout);

private:
  MDES* md;               // MDES affiliation
  Vector<int> cum_vect;   // cumulative resource usage
  int lower_bound;        // initial lower bound
};

class StackDescr {
  public:
    StackDescr();
    void setStackDir(StackDir dir);
    void setRetAddrThruStack();
    void setRetAddrSize(unsigned int);
    void setAlignment(unsigned int);
    StackDir getStackDir();
    bool isRetAddrThruStack();
    unsigned int getRetAddrSize();
    unsigned int getAlignment();
  private:
    StackDir _dir;
    bool _ret_addr_thru_stack;
    unsigned int _ret_addr_size;
    unsigned int _alignment;
};

#endif

