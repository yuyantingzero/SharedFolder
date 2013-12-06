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
//      File:           rr_allocation.cpp
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    Rotating Register Allocator Class Implementation
//
/////////////////////////////////////////////////////////////////////////////

/* 
   Question:
     1) how to handle both theta and EPS 
	for the moment theta is ignored.

*/

#include "intf.h"
#include "el_swp_dsa_convert.h"
#include "el_lreg_init.h"
#include "dbg.h"
#include "rr_file.h"
#include "mymath.h"
#include "el_error.h"
#include "intf.h"
#include "mdes.h"

/*
  interface:
    program:       dbg_set(ra, 3);
    command line:  elcor -Frr_debug=3

  dbg levels for "ra"
    1: check solution, print info messages (errors are printed as warnings)
    2: print reg allocation
    3: print code before and after commit
    4: print internal data (small)
    5: print internal data (large)
    6: check ir reading
    7: print trace of allocation procedure
    8: open loop additional blade is one cycle wide (normally: 0 cycle wide)

*/

ostream& operator << (ostream& os, RR_allocation& a)
{
  if (!a.allocated) {
    El_warn("Allocation not yet performed");
    return os;
  }
  
  for(int i = 0; i < a.num_reg_files; i++) {
    if (!MDES_supports_rot_reg(MDES_reg_name(i))) continue;
    if (a.reg_file_data[i] != 0) os << *a.reg_file_data[i];
  }
  return os;
}

RR_allocation::RR_allocation() 
  : loop(0), 
    initialized(false), allocated(false), committed(false), open_loop(false),
    LPS(-infinity), LES(-infinity)
 {
    num_reg_files = MDES_num_reg_types();
    int init = 0;
    RR_file_data* rr_init = NULL;
    reg_file_data.resize(num_reg_files, rr_init);
    reg_requirements.resize(num_reg_files, init);
    reg_max.resize(num_reg_files, init);
    reg_commited.resize(num_reg_files, init);
}

RR_allocation::RR_allocation(LoopBody* loopbody, const bool open_loop_model) 
  : loop(loopbody), 
    initialized(false), allocated(false), committed(false), open_loop(open_loop_model),
    LPS(-infinity), LES(-infinity) 
{
    num_reg_files = MDES_num_reg_types();
    int init = 0;
    RR_file_data* rr_init = NULL;
    reg_file_data.resize(num_reg_files, rr_init);
    reg_requirements.resize(num_reg_files, init);
    reg_max.resize(num_reg_files, init);
    reg_commited.resize(num_reg_files, init);

    for (int i = 0; i < num_reg_files; i++) {
       char* phys_name = MDES_reg_name(i);
       if (!MDES_supports_rot_reg(phys_name)) continue;

       if (reg_file_data[i] != 0) delete reg_file_data[i];
       reg_file_data[i] = new RR_file_data(loop, phys_name, open_loop);
       reg_max[i] = MDES_reg_rotating_size(phys_name);
    }
   
//   // scan the code for each register file
//   for(int i = (int) FIRST_FILE; i<(int) NUM_REG_FILE; i++) {
//     if (!MDES_supports_rot_reg(regfile_to_char((Reg_file)i))) continue;

//     if (reg_file_data[i] != 0) delete reg_file_data[i];
//     reg_file_data[i] = new RR_file_data(loop, (Reg_file) i, open_loop);
//     reg_max[i] = MDES_reg_rotating_size(regfile_to_char((Reg_file)i));
//   }
    
    initialized = true;
}

RR_allocation::~RR_allocation() 
{
  for(int i = 0; i < num_reg_files; i++) {
    if (reg_file_data[i] != 0) {
       delete reg_file_data[i];
       reg_file_data[i] = 0;
    }
  }
}

bool RR_allocation::allocate(const int II, const int ESC,
  const bool predicated_strategy, const Alloc_strategy alloc_strategy, 
  const Ordering_strategy& ordering_strategy, const int prologue_peeling, 
  const int epilogue_peeling, const int theta)
{
  return perform_allocation(II, ESC, false, predicated_strategy,
    alloc_strategy, ordering_strategy, prologue_peeling, epilogue_peeling, theta);
}
  
bool RR_allocation::allocate_default(const int II, const int ESC, 
  const int prologue_peeling, const int epilogue_peeling, const int theta)
{
  // set defaults strategies similar to kernel only in PLDI-92 paper
  bool predicated_strategy = El_lreg_rr_pred_alloc;
  Alloc_strategy alloc = best_fit;
  Ordering_strategy order;
  order.add_tail(adjacency); 
  order.add_tail(start_time); 
    
  // call allocation
  return perform_allocation(II, ESC, false, predicated_strategy, 
    alloc, order, prologue_peeling, epilogue_peeling, theta);  
}

Pair<int, int>* RR_allocation::allocate_wand_only(const int II, const int ESC, 
  const bool predicated_strategy, 
  const Alloc_strategy alloc_strategy, const Ordering_strategy& ordering_strategy, 
  const int theta)
{
  bool success = perform_allocation(II, ESC, true, predicated_strategy,
    alloc_strategy, ordering_strategy, 0, 0, theta);
  if (success) {
    return new Pair<int, int>(LPS, LES);
  } else {
    return 0;
  }
}

Pair<int, int>* RR_allocation::allocate_wand_only_default(const int II, const int ESC, 
  const int theta)
{
  // set defaults strategies similar to wand only in PLDI-92 paper
  bool predicated_strategy = El_lreg_rr_pred_alloc;
  Alloc_strategy alloc = end_fit;
  Ordering_strategy order;
  order.add_tail(adjacency); 
  order.add_tail(start_time); 
    
  // call allocation
  bool success = perform_allocation(II, ESC, true, predicated_strategy, 
    alloc, order, 0, 0, theta);  
  if (success) {
    return new Pair<int, int>(LPS, LES);
  } else {
    return 0;
  }
}

int RR_allocation::register_requirements(int regfile)
{
  assert(allocated);
  return reg_requirements[regfile];
}

Pair<int, int>* RR_allocation::peeling_for_wand_only(const int II, const int ESC)
{
  assert(initialized);
  int LPS = -infinity;
  int LES = -infinity;
  int SC = ESC+1;

  for(int i = 0; i < num_reg_files; i++) {
    if (!MDES_supports_rot_reg(MDES_reg_name(i))) continue;

    reg_file_data[i]->Construct_lifetimes(II, SC, 0, 0, 0);
    int p, e;
    reg_file_data[i]->Peeling_for_wand_only(p, e);
    LPS = ELCOR_MAX(LPS, p);
    LES = ELCOR_MAX(LPS, e);
  }
  Pair<int, int>* res = new Pair<int, int>(LPS, LES);
  return res;
}

bool RR_allocation::commit(const Vector<int>* reg_file_size)
{
  assert(allocated);
  if (dbg(ra, 3)) cdbg << "@code before commit:" << endl << *loop;

  // Old Code: we must wrap at the physical regfile boundary for
  // computing the physical registers even for open loop.
  // -- Shail Aditya 02/26/2001

  //   if (open_loop) {
  //     reg_commited = reg_requirements;
  //   } else if (reg_file_size != 0) {
  //     reg_commited = *reg_file_size;
  //   } else {
  //     reg_commited = reg_max;
  //   }

  if (reg_file_size != 0) {
    reg_commited = *reg_file_size;
  } else {
    reg_commited = reg_max;
  }

  for(int i = 0; i < num_reg_files; i++) {
    if (!MDES_supports_rot_reg(MDES_reg_name(i))) continue;

    assert(reg_commited[i] >= reg_requirements[i]);
    reg_file_data[i]->Commit_register_allocation(reg_commited[i]);
  }

  if (dbg(ra, 1)) {
    cdbg << "end allocation and commit" << endl;
    verify(false);
  }
  if (dbg(ra, 3)) cdbg << "@code after commit:" << endl << *loop;

  committed = true;
  return committed;
}

bool RR_allocation::get_live_register_allocation(Map<Operand,int>& in, Map<Operand,int>&out)
{
  assert(committed);

  in.clear();
  out.clear();
  for(int i = 0; i < num_reg_files; i++) {
    if (!MDES_supports_rot_reg(MDES_reg_name(i))) continue;

    reg_file_data[i]->Get_live_in_register_allocation(in);
    reg_file_data[i]->Get_live_out_register_allocation(out);
  }
  return true;
}  

int RR_allocation::get_entry_register_allocation(Operand& operand)
{
  assert(committed);
  if (operand.allocated()) return operand.mc_num();
  char* phys_file = (char*)operand.physical_file_type();
  int i = MDES_reg_index(phys_file);
  assert(MDES_supports_rot_reg(phys_file));
  return (reg_file_data[i]->Get_entry_register_allocation(operand));
}

int RR_allocation::get_exit_register_allocation(Operand& operand)
{
  assert(committed);
  if (operand.allocated()) return operand.mc_num();
  char* phys_file = (char*)operand.physical_file_type();
  int i = MDES_reg_index(phys_file);
  assert(MDES_supports_rot_reg(phys_file));
  return (reg_file_data[i]->Get_exit_register_allocation(operand));
}

bool RR_allocation::verify(const bool check_minimality)
{  
  assert(allocated);
  for(int i = 0; i < num_reg_files; i++) {
    if (!MDES_supports_rot_reg(MDES_reg_name(i))) continue;

    if (!reg_file_data[i]->Check_allocation(check_minimality)) {
      El_warn("Check rotating register allocation FAILED");
      return false;
    }
  } 
  if (dbg(ra, 1))
    cdbg << "Check rotating register allocation passed" << endl;
  return true;
}

bool RR_allocation::perform_allocation(const int II, const int ESC, const bool wand_only, 
  const bool predicated_strategy, const Alloc_strategy alloc_strategy, 
  const Ordering_strategy& ordering_strategy, const int prologue_peeling, 
  const int epilogue_peeling, const int theta)
{
  if (dbg(ra, 1)) cdbg << "allocation of rotating registers" << endl;
  if (!initialized) {
    El_warn("RR: error, non initialized RR_allocation object");
    return false;
  }
  int SC = ESC+1;
  allocated = false;
  committed = false;
  
  if (wand_only) {
    LPS = -infinity;
    LES = -infinity;
  }


  // assign operand to static/rotating files
  // done now just after el_swp_dsa_convert_loop because the modulo scheduler
  // make el_swp_map_vr_to_rr_file to cycle indefinitly
  // el_swp_map_vr_to_rr_file((Compound_region*) loop);

  for(int i = 0; i < num_reg_files; i++) {
    if (!MDES_supports_rot_reg(MDES_reg_name(i))) continue;

    reg_file_data[i]->Construct_lifetimes(II, SC, prologue_peeling, epilogue_peeling, theta);
    reg_requirements[i] = reg_file_data[i]->Allocate_register(wand_only, predicated_strategy, 
      alloc_strategy, ordering_strategy);
    if (wand_only) {
      int p, e;
      reg_file_data[i]->Peeling_for_wand_only(p, e);
      LPS = ELCOR_MAX(LPS, p);
      LES = ELCOR_MAX(LPS, e);
    }
    if (reg_requirements[i] > reg_max[i]) return false;
  }

  allocated = true;
  if (dbg(ra, 1)) 
    cdbg << "allocation of rotating registers completed" << endl;
  if (dbg(ra, 2)) cdbg << *this;
  return true;
}


int RR_allocation::get_longest_lifetime(const int II, const int ESC)
{
  unsigned SC = ESC+1;

  int longest_life = -1;
  for(int i = 0; i < num_reg_files; i++) {
    char* phys_name = MDES_reg_name(i);
    if( ! MDES_supports_rot_reg(phys_name))
      continue;
    reg_file_data[i]->Construct_lifetimes(II, SC, 0, 0, 0);
    longest_life = ELCOR_MAX(longest_life, reg_file_data[i]->get_longest_lifetime());
  }

  if(longest_life < 0) {
    El_punt("RR_allocation is having problems determining the longest live time!\n");
  }
  return longest_life;
}

