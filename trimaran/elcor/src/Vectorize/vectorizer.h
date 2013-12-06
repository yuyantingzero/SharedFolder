/***********************************************************************

Copyright (c) 2007 Massachusetts Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

***********************************************************************/



//===========================================================================
//
//   FILE: vectorizer.h:
//   
//   Author: Sam Larsen
//   Date: Fri Mar 18 11:10:09 2005
//
//   Function:  Selective vectorizer
//
//===========================================================================

#ifndef _VECTORIZER_H_
#define _VECTORIZER_H_

#include "el_main.h"
#include "el_loop.h"
#include "matrix.h"
#include "list.h"
#include "rmsl_ext.h"


struct Op_info {
  Op_info() : stride(0), aligned(true), before(false), can_vectorize(false),
	      do_vectorize(false), do_not_unroll(false), 
	      dtype(EL_STANDARD_INT), scal_op_string(NULL), 
	      vect_op_string(NULL) {}
  int stride;
  bool aligned;
  bool before;
  bool can_vectorize;
  bool do_vectorize;
  List<int> pred_ids;
  List<int> succ_ids;
  bool do_not_unroll;
  DType dtype;
  char* scal_op_string;
  char* vect_op_string;
  char scal_io_pat[MAXIOPATLEN];
  char vect_io_pat[MAXIOPATLEN];
};


class Vectorizer {
public:
  Vectorizer(El_Loop*);
  ~Vectorizer();
  void debug(Matrix<bool>&);

private:
  inline Op_info& op_to_info(Op* op) {
    return op_info[op_to_id.value(op)];
  }

  int num_real_ops();
  void init_op_info();
  bool is_addr_ind_var(Op*, Op*&);

  void find_sig_operands();
  void draw_dep_edges();
  void record_flow_dep(int, int);

  void find_vectorizable();
  int visit(int, int&, Vector<int>&, List<int>&);
  bool is_vectorizable(List<Op*>*);

  bool is_float(Op*);
  int  data_type_width(Op*);
  DType dtype(Op*);

  void transform_loop();
  Basicblock* build_loop_head();
  bool need_scratch_pad();
  Basicblock* build_main_loop();
  Basicblock* build_post_loop();
  Basicblock* build_loop_test(Compound_region*, bool);
  Basicblock* build_main_test(Compound_region* targ) 
    { return build_loop_test(targ, true); }
  Basicblock* build_post_test(Compound_region* targ)
    { return build_loop_test(targ, false); }

  Op* insert_new_vector_op(Op*, Basicblock*);
  Op* insert_vector_store(Op*, Basicblock*);
  Op* insert_vector_load(Op*, Basicblock*);
  bool has_scalar_flow_deps(Op*);

  Op* insert_new_scalar_op(Op*, int, Basicblock*);
  bool has_vector_flow_deps(Op*);

  void add_offset_to_memop(Op*, Op_info&, int);
  void adjust_for_primary(Op*, int&, int);
  bool primary_used_in_loop();

  void insert_op(Op*, Basicblock*);
  void connect(Op*, Op*, bool fallthrough = false);
  Edge* get_nonloop_inedge(Basicblock*);
  Edge* get_nonloop_outedge(Basicblock*);

  Op* copy_op(Op*);
  Op* new_scalar_op(Op*, int);
  Op* new_vector_op(Op*);
  Reg* new_scalar_reg(Operand&, int);
  Reg* new_vector_reg(Operand&);

  bool is_aligned(Op*);
  Op* new_vec_perm(Op*, Operand&, Operand&);
  Opcode get_perm_opcode(DType);

  void insert_v2s_xfr(Op*, Basicblock*);
  void insert_s2v_xfr(Op*, Basicblock*);
  Op* make_new_op(Opcode, Base_operand*, Base_operand* src1=NULL,
		  Base_operand* src2=NULL, Base_operand* src3=NULL,
		  Base_operand* src4=NULL);

  void update_mem_vrs(Basicblock*);

  void mostly_vectorize();
  void partition_ops();
  void opt_partition();

  int switch_one_op(Vector<bool>&, Vector<bool>&, double&);
  void try_scalar_alloc(int, Vector<bool>&);
  void try_vector_alloc(int, Vector<bool>&);
  void place_all_ops(Vector<bool>&);
  void adjust_perm_usage(DType, int);
  void adjust_xfr_usage(bool, int, bool);
  void adjust_xfr_usage(Opcode, int, bool);

  int num_consumers(int, Vector<bool>&, bool);
  int num_scal_consumers(int id, Vector<bool>& p)
    { return num_consumers(id, p, false); }
  int num_vect_consumers(int id, Vector<bool>& p)
    { return num_consumers(id, p, true); }

  void init_mdes_info();
  void init_op_mdes_info();
  void init_op_mdes_info(Op*, bool);
  void init_misc_mdes_info();
  void init_misc_mdes_info(Opcode, Base_operand*, Base_operand* src1=NULL,
			   Base_operand* src2=NULL, Base_operand* src3=NULL,
			   Base_operand* src4=NULL);
  void print_ddg();
  void print_vect_info();

  El_Loop* loop;
  Basicblock* body;

  Dlist<Op*> sig_ops;
  Dlist<Op*> addr_ops;
  Op* branch;
  Op* cond;
  Op* incr;
  int init_stride;

  Reg* ub_reg;

  int num_ops;
  int num_vectorizable;
  int unl_fact;

  Hash_map<int,Op*> id_to_op;
  Hash_map<Op*,int> op_to_id;
  Op_info* op_info;

  Hash_set<Operand> privatizable;
  Hash_set<Operand> invariant;
  Hash_map<Operand,int> vect_vr_num;
  Hash_map<Operand,Vector<int>*> scal_vr_num;
  
  Matrix<bool> adjMat;
  List<List<Op*>*> sccs;

  Reg* scratch_ptr;
  Reg* scratch_inc;
  int scratch_offs;

  Rmsl_ext rmsl;
  static Hash_map<Opcode,char*>* misc_op_string;
  static Hash_map<Opcode,char*>* misc_io_pat;

  int passes;
};

#endif
