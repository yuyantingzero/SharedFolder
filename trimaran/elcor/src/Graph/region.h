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
//      File:           region.h
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson,
//                      Santosh Abraham, Scott Mahlke, Dave August,
//                      Brian Deitrich,Joel Jones, Sumedh Sathaye
//      Created:        January 1994
//      Description:    Region class hierearchy declaration
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _REGION_H
#define _REGION_H

#include <iostream>
#include "defs.h"
#include "hash_set.h"
#include "map.h"
#include "dlist.h"
#include "list.h"
#include "graph_filters.h"
#include "bit_vector.h"
#include "el_data.h"

// I think this is bad.
#include "operand.h"

using namespace std;

class Edge;
class Op;
class Procedure;
class Graph_attribute;
class El_jumptbl_info;

//extern int op_id_num;
extern Procedure *default_scope;

typedef Map<Edge*, Edge*> Edge_map;
typedef Map<Region*, Region*> Region_map;
typedef List_iterator<Procedure*> Program_iterator;
typedef List_iterator<El_datalist*> Data_iterator;
class Region_entry_edges;
class Region_exit_edges;
class Region_subregions;
class Region_subregions_bidir;
class Region_all_ops;

enum Elcor_region_flag {
  // Impact OP flags
  EL_OPER_CHECK,            // Check operation as in sentinel scheduling
  EL_OPER_LABEL_REFERENCE,  // See the label attribute for memory dependence
  EL_OPER_PROMOTED,         // Predicate of this op has been promoted
  EL_OPER_SQUASHING,        // WE SHOULDN'T SEE SUCH AN OP
  EL_OPER_DATA_SPECULATIVE, // Data speculative operation
  EL_OPER_SPILL_CODE,       // Operation is load/store and spill code
  EL_OPER_CALLEE_SAVE,      // Operation is a CALLEE save/restore
  EL_OPER_CALLER_SAVE,      // Operation is a CALLER save/restore
  EL_OPER_SIDE_EFFECT_FREE, // Operation is a call which is side effect free
  EL_OPER_SPECULATIVE,      // Operation is speculative
  EL_OPER_MASK_PE,          // Emulator should mask a possible exception 
  EL_OPER_SAFE_PEI,         // An oper which doesn't except inspite of opcode
  EL_OPER_PROBE_MARK,       // An oper which need to be simulated
  EL_OPER_SYNC,	            // Oper should be treated like a sync op
  EL_OPER_PROCESSOR_SPECIFIC, // Op doesn't obey Lcode semantics
  EL_OPER_VOLATILE,         // Oper accesses a violatile mem locn
  // New flags from Impact as of 2-03 SAM
  EL_OPER_NO_SPECULATION,   // Oper cannot be speculated
  EL_OPER_STACK_REFERENCE,  // Ld/st op refers to the stack
  EL_OPER_ROTATE_REGISTERS, // Op performs a rotate (like BRF)
  EL_OPER_SUPER_SPECULATION, // I have no clue what the heck this means

  // Elcor OP flags
  EL_OPER_TABLE_JUMP,	    // BRU which is a table jump (jump_rg)
  EL_OPER_REAL_MERGE,	    // C_MERGE which has multiple CF edges in
  EL_OPER_NONSEQ_DUMMY_BR,  // DUMMY_BR with non-sequential control flow
  EL_OPER_LIVEOUT_DEST,     // Op produces reg which is live-out of proc
  EL_OPER_CVT,              // MOVE op that does integer conversion, info only!
  EL_OPER_ICMOVE,           // Inter-cluster move
  EL_OPER_CGEN_MOVE,        // Codegen move
  EL_OPER_STACK_MOD,        // Stack modification, Pracheeti
  EL_OPER_UNCACHABLE,       // Prevent this op from getting cached

  EL_OPER_MARK1,            // Non-persistentent marker bits for IR traversal
  EL_OPER_MARK2,            // Non-persistentent marker bits for IR traversal
  EL_OPER_SWP_BRANCH,       // Op or ops responsible for modulo
			    // scheduling control flow

   // Impact Region/CB flags
  EL_REGION_HYPERBLOCK,             // Region is an Impact hyperblock
  EL_REGION_HYPERBLOCK_NO_FALLTHRU, // Region does not fall thru
  EL_REGION_SOFTPIPE,               // Region is a software pipelinable loop
  EL_REGION_SUPERBLOCK,             // Region is an Impact superblock
  EL_REGION_UNROLLED,               // Region has been unrolled by Impact
  EL_REGION_ENTRANCE_BOUNDARY,      // Region is a boundary cb for a function
  // encapsulated region
  EL_REGION_EXIT_BOUNDARY,          // Region is a boundary cb for a function
  // encapsulated region
  EL_REGION_HAS_PROLOGUE_OP,        // Region has prologue operation
  EL_REGION_HAS_EPILOGUE_OP,        // Region has prologue operation
  EL_REGION_VIOLATES_LC_SEMANTICS,  // Region violates standard Lcode semantics
  EL_REGION_ROT_REG_ALLOCATED,      // Region has rotating registers allocated

   // Elcor Region/CB flags
  EL_REGION_HYPERBLOCK_FRP,         // Region is FRP
  EL_REGION_REGISTER_ALLOCATED,     // Region has been register allocated
  EL_REGION_SCHEDULED,	            // Region has been scheduled
  EL_REGION_PRELOOP_BLOCK,          // Preheader to a loop
  EL_REGION_POSTLOOP_BLOCK,         // post loop block
  EL_REGION_REMAINDER_BLOCK,        // Block needed by modulo variable expansion
                                    //  to properly unroll modulo scheduled loops.
  EL_REGION_HAS_EXPLICIT_FT,        // has an explicit fall thru path (dummy br),
				    // even though it does not require one
  EL_REGION_CPR_BLOCK,              // Control CPR has been applied to this block
  EL_REGION_CPR_COMP_BLOCK,         // Region is a CPR compensation block
  EL_REGION_LOOP_CACHE,		    // Block to be relocated to loop cache (WIMS Pracheeti) 
  EL_REGION_HAS_ANALYSIS_INFO,      // create_local_analysis_info has been called

  // Impact PROC flags   
  EL_PROC_HYPERBLOCK,               // Procedure contains hyperlocks
  EL_PROC_LEAF,                     // Leaf procedure
  EL_PROC_SIDE_EFFECT_FREE,         // Procedure doesn't have side effects
  EL_PROC_REGISTER_ALLOCATED,       // Procedure has been register allocated (scalar)
  EL_PROC_SUPERBLOCK,               // Procedure contains superblocks
  EL_PROC_MASK_PE,                  // Emulator should mask exceptions for this proc
  EL_PROC_COMPILATION_COMPLETE,     // no more compilation needed for this proc
  EL_PROC_SCHEDULED,	             // Procedure has been scheduled
  EL_PROC_CC_IN_PREDICATE_REGS,     // branch compare conditions in pred regs
  EL_PROC_PRED_REGS_IN_ATTR,	     // Original predicate regs stored in attrs
  EL_PROC_ROT_REG_ALLOCATED,        // Procedure has rotating registers allocated

  // Elcor PROC flags
  EL_PROC_NEEDS_SUIF_FIXUP,	     // Procedure need to fixed up before
  // any processing

   // Elcor TREE flags
  EL_TREE_HYPERBLOCK,
  EL_TREE_SUPERBLOCK,
  EL_TREE_REGISTER_ALLOCATED,
  EL_TREE_SCHEDULED,

  // SLARSEN: don't swp the region
  EL_DONT_SWP,

   // Last element marker
  EL_REGION_LAST_FLAG		     // DO NOT DELETE!!
} ;

class Compound_region ;
class IR_outstream;

class Region {
  friend ostream& operator<<(ostream& os, const Region& region);
  friend IR_outstream &operator<<(IR_outstream &out, const Region &region);
  friend class Region_entry_edges;
  friend class Region_exit_edges;
  friend class Region_subregions;
  friend class Region_subregions_bidir;
  friend class Region_all_ops;
  friend class Region_ops_reverse_C0_order ;

public:
  Region();
  Region(const Region&) ;
  //   virtual Region& operator=(const Region&);
  virtual ~Region();
  
  virtual int id() const = 0;
  virtual void set_id(int id) = 0 ;
  virtual void renumber(Procedure *scope = NULL) = 0 ;
  Compound_region* parent() const;
  Region& set_parent(Compound_region*);
  Dnode<Region*>* dnode() const;
  Region& set_dnode(Dnode<Region*>*);

  virtual bool is_op() const;
  virtual bool is_compound() const;
  virtual bool is_bb() const;
  virtual bool is_hb() const;
  virtual bool is_procedure() const;
  virtual bool is_loopbody() const;
  virtual bool is_tree () const;

  virtual bool is_single_entry() const ;
  virtual bool is_single_exit() const ;   

  virtual Dlist<Region*>& subregions() = 0;
  virtual Hash_set<Op*>& inops() = 0;
  virtual Hash_set<Op*>& outops() = 0;
  virtual List<Edge*>& inedges() = 0;
  virtual List<Edge*>& outedges() = 0;

  Hash_set<Region*> insubregions() ;
  Hash_set<Region*> outsubregions() ;

  Graph_attribute* attributes;
  
  // Copying a region
  // Copy all subcomponents recursively.
  // The first copies all internal edges but only control-flow interface edges.
  // The second copies edges based on three filters: one for input interface
  // edges, one for output interface edges, and one for internal edges.
  // For inteface edges, the unconnected src or dest is set to NULL.
  // region_map is filled in with mappings from regions (including ops) in the
  // old region to  regions in the  new region.  
  // Similarly, edge_map is filled in with mappings from edges in
  // the old region to the new region. The edge_map contains only the copied
  // edges. 

  Region* copy_with_cf_interface_edges();
  Region* copy_with_specified_edges
  (Region_map& region_map, Edge_map& edge_map,
   const Edge_filter* in_filter = ALL_EDGES,
   const Edge_filter* out_filter = ALL_EDGES,
   const Edge_filter* internal_filter = ALL_EDGES);
  // Auxiliary function for copying
  virtual Region* copy_with_edges(Region_map& region_map, 
				  Edge_map& edge_map) = 0;
  
  bool flag(Elcor_region_flag) const ;
  void set_flag(Elcor_region_flag) ;
  void reset_flag(Elcor_region_flag) ;
  void copy_flags(Region *src_reg);

  // returns (largest op id+1) for this region
  int get_next_op_id_num(Procedure *scope = NULL) const ;
  // same, but increments max_id number
  int get_next_op_id_num_inc(Procedure *scope = NULL);
  
protected:
  Bitvector region_flags ;
  virtual void print(ostream& os) const = 0 ;
  Compound_region* parent_region;
  Dnode<Region*>* dlist_node;
};


class Compound_region : public Region {
  friend IR_outstream &operator<<(IR_outstream &out, const Region &region);
  friend void name_edges(IR_outstream &out, Region *reg);
  friend void print_edges(IR_outstream &out, Region *reg);
  friend void name_attributes(IR_outstream &out, Region *reg);
  friend void print_attributes(IR_outstream &out, Region *reg);
  friend void print_mem_vrs(IR_outstream &out, Region *reg);
  friend bool Op_check_integrity(Op *theop);
public:
  Compound_region(Procedure *scope = NULL);
  Compound_region(int reg_id, double profile_weight = 0, Procedure *scope = NULL) ;
  virtual ~Compound_region();

  void renumber(Procedure *scope = NULL) ;
  int id() const;
  void set_id(int id);
  void reset_id_counter(int id = 1, Procedure *scope = NULL) ;

  bool is_compound() const;

  Compound_region& add_region(Region*);
  Compound_region& remove_region(Region*);
  Compound_region& clear_region();
  Compound_region& add_entry(Op*);
  Compound_region& remove_entry(Op*);
  Compound_region& add_exit(Op*);
  Compound_region& remove_exit(Op*);
  Compound_region& add_entry(Edge*);
  Compound_region& add_entry_safely(Edge*);
  Compound_region& remove_entry(Edge*);
  Compound_region& add_exit(Edge*);
  Compound_region& add_exit_safely(Edge*);
  Compound_region& remove_exit(Edge*);

  void clear_subregions() ;
  Compound_region& insert_before_region(Region* r, Region* t); // insert r before t
  Compound_region& insert_immediately_before_region(Region* r, Region* t); // insert r right before t
  Compound_region& insert_after_region(Region* r, Region* t);// insert r after t
  Compound_region& insert_immediately_after_region(Region* r, Region* t);// insert r right after t
								   
  void replace_op_recursively(Op* old_op, Op* new_op);
  
  bool is_entry_edge(Edge*);
  bool is_exit_edge(Edge*);
  bool is_entry_op(Op*);
  bool is_exit_op(Op*);
  bool succeeds_in_subregion_list(Region*,Region*);   //does arg2 succed arg1?
  Region* successor_in_subregion_list(Region* first);
  Region* last_subregion();
  Region* first_subregion();

  // returns (largest op id+1) for this region
  int get_next_region_id_num(Procedure *scope = NULL);
  // same, but increments max_id number
  int get_next_region_id_num_inc(Procedure *scope = NULL);

  double weight ;

protected:
  Dlist<Region*>& subregions();
  Hash_set<Op*>& inops();
  Hash_set<Op*>& outops();
  List<Edge*>& inedges();
  List<Edge*>& outedges();
  virtual Region* copy_with_edges(Region_map& region_map, Edge_map& edge_map);
  void copy_with_edges_aux(Compound_region* new_region,
			   Region_map& region_map, Edge_map& edge_map);
  virtual void print(ostream& os) const;

  //  static int id_num;
  int id_rep;

private:
  Dlist<Region*> embedded_regions;
  Hash_set<Op*> embedded_entry_ops;
  Hash_set<Op*> embedded_exit_ops;
  List<Edge*> entry_edges;
  List<Edge*> exit_edges;
};


class Basicblock : public Compound_region {
public:
  Basicblock();
  Basicblock(int id, double w = 0.0);
  ~Basicblock();
   
  bool is_bb() const;

protected:
  Region* copy_with_edges(Region_map& region_map, Edge_map& edge_map);
  virtual void print(ostream& os) const;
};


// superblock/hyperblock class - built up from basic blocks
//
class Hyperblock : public Compound_region {
public:
  Hyperblock();
  Hyperblock(int id, const double& w);
  ~Hyperblock();
      
  bool is_hb() const;
  bool is_single_exit() const ;
protected:
  Region* copy_with_edges(Region_map& region_map, Edge_map& edge_map);
  virtual void print(ostream& os) const;
};


class LoopBody : public Compound_region {
public:
  LoopBody();
  LoopBody(int id, const double& w);
  ~LoopBody();

  bool is_loopbody() const;
  Hash_set<Edge *> *controlFlowBackEdges();
  void addBackEdge(Edge *);
  Compound_region *preloop();
  Compound_region *postloop();
  Compound_region *kernel();
  Compound_region *remainder();
  Compound_region *preremainder();

  inline Operand get_staging_pred() const { return staging_pred; }
  inline void set_staging_pred(const Operand& oper) { staging_pred = oper; }
protected:
  Hash_set<Edge *> cFlowBackEdges;
  Region* copy_with_edges(Region_map& region_map, Edge_map& edge_map);
  virtual void print(ostream& os) const;

  Operand staging_pred;
};

class Procedure : public Compound_region {
public:
  Procedure(bool save_scope = false);
  Procedure(const eString& s,const double& w, bool save_scope = false);
  ~Procedure();
   
  bool is_procedure() const;
  eString get_name() const;
  void set_name(const eString& s) ;
   
  void set_max_op_id(int n) ;
  int get_max_op_id() ;
   
  void set_max_region_id(int n) ;
  int get_max_region_id() ;
   
  void set_jumptbl_info(El_jumptbl_info *info);
  El_jumptbl_info* get_jumptbl_info();
   
protected:
  Region* copy_with_edges(Region_map& region_map, Edge_map& edge_map);
  virtual void print(ostream& os) const;
   
private:
  eString procedure_name ;
  El_jumptbl_info *jumptbl_info;
  int op_id_num;
  int region_id_num;
};

class Tree:public Compound_region {
public:
  Tree ();
  Tree (int id, const double& w);
  ~Tree ();

  bool is_tree () const;
  
protected:
  Region* copy_with_edges (Region_map& region_map, Edge_map& edge_map);
  virtual void print (ostream& os) const;

private:
};

class Program {
  friend ostream& operator<<(ostream& os, const Program& prog);
 public:
  Program();
  Program(const eString&);
  Program(Procedure*);
  Program(Procedure*, const eString&);
  ~Program();

  void add_procedure(Procedure*);
  void remove_procedure(Procedure*);
  void remove_procedure(const eString&);
  void add_data(El_datalist*);
  void remove_data(El_datalist*);
  Procedure* get_procedure(const eString&);
  void clear();

  eString get_name();
  void set_name(const eString&);

  Program_iterator get_iterator() const;
  Data_iterator get_data_iterator() const;
  int get_num_procedures();
 private:
  int num_procs;
  eString name;
  List<Procedure*> procs;
  List<El_datalist*> data;
  Hash_map<eString, Procedure*> name_to_proc;
};


// ----------------------- Auxiliary function used in copying --------------
Edge* region_copy_edge_clone_and_bind(Edge* original, Edge_map& edge_map);

#endif

