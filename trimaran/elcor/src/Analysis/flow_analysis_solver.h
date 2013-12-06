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
//      File:           flow_analysis_solver.h
//      Authors:        Sadun Anik, Richard Johnson
//      Created:        April 1994
//      Description:    Global liveness analyzer
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _FLOW_ANALYSIS_SOLVER
#define _FLOW_ANALYSIS_SOLVER

#include "bit_vector.h"
#include "adjlist_graph.h"
#include "hash_map.h"
#include "hash_set.h"
#include "operand.h"

const unsigned int ANALYZE_MEMVR  = 32  ;
const unsigned int ANALYZE_INT    = 16  ;
const unsigned int ANALYZE_FP     =  8  ;
const unsigned int ANALYZE_BTR    =  4  ;
const unsigned int ANALYZE_PR     =  2  ;
const unsigned int ANALYZE_CR     =  1  ;
const unsigned int ANALYZE_ALLREG = 31  ;
const unsigned int ANALYZE_ALL    = 63  ;


class Region ;

typedef List<Operand> Liveness_info ;
typedef List_iterator<Operand> Liveness_info_iterator ;

class Liveness_solver_data {
public:
   Liveness_solver_data() ;
   Liveness_solver_data(int) ;
   ~Liveness_solver_data() ;

   bool operator==(const Liveness_solver_data&) const ;
   
   Bitvector live_in ;
   Bitvector live_out ;   
   Bitvector live_out_initial ;   
   Bitvector liveness_gen ;
   Bitvector liveness_kill ;

   Bitvector down_exposed_use_in ;
   Bitvector down_exposed_use_in_initial ;
   Bitvector down_exposed_use_out ;   
   Bitvector down_exposed_use_gen ;
   Bitvector down_exposed_use_kill ;

   Bitvector up_exposed_def_in ;
   Bitvector up_exposed_def_out ;   
   Bitvector up_exposed_def_out_initial ;
   Bitvector up_exposed_def_gen ;
   Bitvector up_exposed_def_kill ;
   
   Bitvector down_exposed_def_in ;
   Bitvector down_exposed_def_in_initial ;   
   Bitvector down_exposed_def_out ;   
   Bitvector down_exposed_def_gen ;
   Bitvector down_exposed_def_kill ;

} ;

class Liveness_solver {
public:
   Liveness_solver() ;
   Liveness_solver(Region*,unsigned int filter_flag = ANALYZE_ALL) ;
   ~Liveness_solver() ;

//   void solve() ;
   
   void solve_liveness() ;
   void solve_down_exposed_uses() ;   
   void solve_up_exposed_defs() ;   
   void solve_down_exposed_defs() ;
   
   void print_liveness_to_graph() ;
   void print_down_exposed_uses_to_graph() ;
   void print_up_exposed_defs_to_graph() ;
   void print_down_exposed_defs_to_graph() ;   

   void set_list_to_liveins(List<Operand>& list, Basicblock* bb) ;   

   bool is_interesting(Operand&) ;

   Alist_region_tiling_graph graph ;
   Vector<Liveness_solver_data> live_info ;
   Hash_map<Operand,int> operand_map ;
   Vector<Operand> rev_operand_map ;

   unsigned int filter_mask ;
      
private:
   void initialize_flow_info() ;
   void set_transfer_function(int) ;

   void liveness_transfer_function(int, bool&) ;
   void liveness_meet_function(int, bool&) ;
   
   void down_exposed_uses_transfer_function(int, bool&) ;
   void down_exposed_uses_meet_function(int, bool&) ;
   
   void up_exposed_defs_transfer_function(int, bool&) ;
   void up_exposed_defs_meet_function(int, bool&) ;

   void down_exposed_defs_transfer_function(int, bool&) ;
   void down_exposed_defs_meet_function(int, bool&) ;

public:
  int  get_region_id(Region* r);
  void get_liveness_gen(List<Operand>& liveness_gen, Region* reg);
  void get_liveness_kill(List<Operand>& liveness_kill, Region* reg);
  void get_down_exposed_use_gen(List<Operand>& down_exposed_use_gen, Region* reg);
  void get_down_exposed_use_kill(List<Operand>& down_exposed_use_kill, Region* reg);
  void get_up_exposed_def_gen(List<Operand>& up_exposed_def_gen, Region* reg);
  void get_up_exposed_def_kill(List<Operand>& up_exposed_def_kill, Region* reg);
  void get_down_exposed_def_gen(List<Operand>& down_exposed_def_gen, Region* reg);
  void get_down_exposed_def_kill(List<Operand>& down_exposed_def_kill, Region* reg);
} ;

void el_flow_compute_liveness(Compound_region* r)  ;
void el_flow_compute_four_dataflow_sets(Compound_region* r)  ;
void el_flow_compute_three_dataflow_sets(Compound_region* r)  ;

void el_flow_compute_liveness(Compound_region* r, unsigned int flag)  ;
void el_flow_compute_four_dataflow_sets(Compound_region* r, unsigned int flag)  ;
void el_flow_compute_three_dataflow_sets(Compound_region* r, unsigned int flag)  ;

void el_flow_delete_liveness(Compound_region* r)  ;
void el_flow_delete_four_dataflow_sets(Compound_region* r)  ;
void el_flow_delete_three_dataflow_sets(Compound_region* r)  ;

void el_flow_compute_four_dataflow_sets(Compound_region* r, unsigned int flag,
                                        List<Operand>&, List<Operand>&, List<Operand>&,
                                        List<Operand>&, List<Operand>&, List<Operand>&,
                                        List<Operand>&, List<Operand>&);

#endif
