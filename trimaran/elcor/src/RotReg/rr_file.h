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
//      File:           rr_file.h
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    class definition for register allocation per file 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ROTREG_LOOP_H  
#define _ROTREG_LOOP_H

#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include "defs.h" 
#include "vector.h"
#include "matrix.h"
#include "string_class.h"
#include "list.h"
#include "hash_set.h"
#include "region.h"
#include "op.h"
#include "operand.h"
#include "pqs.h"
#include "mymath.h"
#include "matrix.h" 

// for rotating registers
#include "rr_allocation.h"
#include "rr_life.h"

using namespace std;

class RR_file_data;
class RR_ref_data;
class RR_order;
class RR_state;
class RR_space_time;

const int undefined = -100000;

////////////////////////////////////////////////////
// class RR_file_data:
//   Register allocation for modulo scheduled loop
//   This is the interface class with the IR.
//   All other classes are inaccessible from the IR
//   because their are hidden by this class
////////////////////////////////////////////////////

class RR_file_data {
  friend ostream& operator << (ostream&, RR_file_data&);
  friend ostream& operator<< (ostream&, RR_space_time&);
  friend class RR_lifetime;
  friend class RR_order;
  friend class RR_state;
  friend class RR_space_time;

public: 
  RR_file_data();
  ~RR_file_data();

  // constructor
  RR_file_data(LoopBody* loopbody, char* phys_file,
    const bool open_loop_model);

  // scan the ir, and build lifetimes
  void Construct_lifetimes(
    const int ii, 
    const int sc,
    const int prologue_peeling=0, 
    const int epilogue_peeling=0,
    const int theta_par=0);

  // allocation of registers (return # registers)
  int 	Allocate_register(
    const bool wand_only_strategy,
    const bool predicated_strategy,
    const Alloc_strategy alloc_strategy, 
    const Ordering_strategy& ordering_strategy); 

  // amount of peeling for wond only kernel
  void	Peeling_for_wand_only(int& lps, int& les);  

  // register requirement (return # registers)
  int	Register_requirement() {return reg_num;}

  // commit the register alloc solution in the IR (return # registers)
  bool	Commit_register_allocation(const int tot_reg);

  // get live in/out physical relocatable address
  bool	Get_live_in_register_allocation(Map<Operand,int>& map);
  bool	Get_live_out_register_allocation(Map<Operand,int>& map);
  int Get_entry_register_allocation(Operand& operand);
  int Get_exit_register_allocation(Operand& operand);

  // debuging purpose: check the result of the allocation 
  bool 	Check_allocation(const bool check_minimality = false);

  // debugging purpose: return the distance matrix
  Matrix<int> Distance_matrix() {return dist;}

  int get_longest_lifetime() const;

private:
  void 	Init();
  void 	Init_alloc(const bool, const Ordering_strategy&);
  int 	Compute_distance(const int below, const int over);
  bool 	Disjoint(RR_ref_data* u, RR_ref_data* d, RR_ref_data* d1, const int iter_diff);
  bool 	Disjoint_with_redef(RR_ref_data* u, RR_ref_data* d, RR_ref_data* d1, 
          Hash_set<RR_ref_data*>& redef_set, const int iter_diff);
  bool 	Live_at_def(const int i, const int j, const int iter_i, const int iter_j);
  bool 	Live_at_def_with_redef(const int i, const int j, 
          const int iter_i, const int iter_j);
  bool 	Interference(const int i, const int j, const int iter_i, const int iter_j);
  int 	Compute_overlap_no_interference_set(const int i, const int j);
  bool 	Overlap_no_interference(const int i, const int j, 
          const int iter_i, const int iter_j);
  int 	Count_forbidden_loc(const int, const int);
  int 	Compute_fom(const int, const int);
  void 	Update_forbidden_loc(const int, const int);
  void 	Soft_allocate_reg(const int, const int);
  void 	Allocate_reg(const int, const int);
  int 	Best_fit(const int);
  int 	First_fit(const int, const int);
  int 	End_fit(const int, const int);
  bool  compatible_file_type(Operand& oprnd);

// parameters and input at allocation time
  int 			  II; 
  int 			  stage_count;
  int 			  life_num;
  Vector<RR_lifetime>     life;
  Region*		  IR_kernel;
  LoopBody*		  IR_loopbody;
  bool                    open_loop;
  bool 			  wand_only;
  bool			  predicated_allowed;
  int 			  PS;
  int 			  ES;
  int 			  theta; 
   //  Reg_file		  reg_file_to_alloc;
   //eString		  reg_file_str;
  eString                 phys_file;

// parameter passed when commit is performed
  int			  commit_reg_num;

// temporary results
  PQS*                    pqs;
  List<int>		  allocated_lt;
  List<int> 		  unallocated_lt;
  Vector< Hash_set<int> > forbidden_loc;
  RR_state*		  reg_state;
  RR_state*		  soft_reg_state;
  int 			  soft_alloc_last_sel;
  Matrix<int> 		  dist;
  Matrix<int> 		  conf;
  Matrix<Hash_set<int> >  overlap_no_interference_loc; 
  Map<int,int>		  vreg_to_lifetime_index; 
  int                     open_loop_lifetime;

// results
  int 			  reg_num;
  int                     rotate_alloc;
  int 			  LPS;
  int			  LES;
  bool			  allocated, commited;
};

////////////////////////////////////////////////////
// global data
////////////////////////////////////////////////////

#define DEBUG_TITLE(title) endl << \
   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << \
   "<< " <<  title << endl << \
   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl

#define DEBUG_TITLE2(title, title2) endl << \
   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << \
   "<< " <<  title << " " << title2 << endl << \
   "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl << endl


#endif





