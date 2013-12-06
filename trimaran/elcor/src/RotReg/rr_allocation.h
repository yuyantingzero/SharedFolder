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
//      File:           rr_allocation.h
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    Rotating Register Allocator Class Definition
//
/////////////////////////////////////////////////////////////////////////////

/* 
   OPERATION:
     try to allocate the register files one after another, and
     allocate all rotating registers in case of success.

   RESTRICTION:
     1) one BRF (remap) operation per loop, occuring at the end of the loop.
     2) at least one def for each lifetime for all rotating register.
     3) all def operations of rotating register must have the same omega value
     4) schedule time must range from 0 .. schedule_length-1

   MODIFS:
     1) sept 94: created by alex
     2) sept 95: new methods to support unpredicated ops
*/

#ifndef _ROTREG_ALLOC_H_  
#define _ROTREG_ALLOC_H_

#include "region.h"
#include "operand.h"
#include "list.h"
#include "defs.h"
#include "vector.h"
#include <iostream> 
#include <fstream>

using namespace std;

class RR_file_data;

// order parameter
enum 	Order_strategy {start_time, adjacency, conflict, no_order};
typedef	List<Order_strategy> Ordering_strategy;

// allocation parameters
enum 	Alloc_strategy {first_fit, end_fit, best_fit};

class RR_allocation {
  friend ostream& operator << (ostream&, RR_allocation&);

public:
  RR_allocation();
  RR_allocation(LoopBody* loopbody, const bool open_loop_model);
  ~RR_allocation();

  // allocate 
  bool 	allocate(
          const int II,
          const int ESC,
    	  const bool predicated_strategy,
  	  const Alloc_strategy alloc_strategy, 
  	  const Ordering_strategy& ordering_strategy, 
  	  const int prologue_peeling=0, 
  	  const int epilogue_peeling=0,
  	  const int theta=0);
  bool 	allocate_default(
          const int II,
          const int ESC,
  	  const int prologue_peeling=0, 
  	  const int epilogue_peeling=0,
  	  const int theta=0);
  
  // wand only allocation
  Pair<int, int>* allocate_wand_only(
          const int II,
          const int ESC,
    	  const bool predicated_strategy,
  	  const Alloc_strategy alloc_strategy, 
  	  const Ordering_strategy& ordering_strategy, 
  	  const int theta=0);
  Pair<int, int>* allocate_wand_only_default(
          const int II,
          const int ESC,
  	  const int theta=0);

  // register requirements (after allocate)
  int register_requirements(int regfile);

  // amount of peeling for wond only kernel
  Pair<int, int>* peeling_for_wand_only(const int II, const int ESC);  

  // verify solutions (after allocate)
  bool	verify(const bool check_minimality = false);

  // commit all rotating register files of valid allocation (after allocate)
  bool  commit(const Vector<int>* reg_file_size = 0); 

  // get live in/out (after commit)
  bool	get_live_register_allocation(Map<Operand,int>& in, Map<Operand,int>&out);
  int get_entry_register_allocation(Operand& operand);
  int get_exit_register_allocation(Operand& operand);
  
  int get_longest_lifetime(const int II, const int ESC);

private:

  bool 	perform_allocation(
          const int II,
          const int ESC,
          const bool wand_only, 
    	  const bool predicated_strategy,
  	  const Alloc_strategy alloc_strategy, 
  	  const Ordering_strategy& ordering_strategy, 
  	  const int prologue_peeling, 
  	  const int epilogue_peeling,
  	  const int theta);

  Vector<RR_file_data *> reg_file_data;
  Vector<int>       reg_requirements;
  Vector<int>       reg_max;
  Vector<int>       reg_commited;
  
  LoopBody* 	loop;
  bool          initialized;
  bool		allocated;
  bool          committed;
  bool          open_loop;
  int           LPS, LES;

  int           num_reg_files;  // added for multicluster KF 8-2001
};

#endif







