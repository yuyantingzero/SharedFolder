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

Copyright 2007 University of Michigan

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
//      File:           live_range.h
//      Authors:        Nate Clark
//      Created:        December 2004
//      Description:    a (hopefully) more intuitive replacement for the 
//                      usLiveRange class.
//
/////////////////////////////////////////////////////////////////////////////


#ifndef __LIVE_RANGE__
#define __LIVE_RANGE__

#include "hash_set.h"
#include "list.h"
#include "operand.h"
#include "edge.h"
#include "ref.h"
#include "pred_interface.h"
#include <iostream>

using namespace std;

class Op;
class Register_file;


enum reg_state {
  NOT_BOUND,
  CALLER_BOUND,
  CALLEE_BOUND,
  SPILLED
};


class Live_range
{
public:
  friend ostream& operator<<(ostream&, const Live_range&);
  friend class Live_range_constructor;

  Live_range(Operand& oprnd);

  inline Operand& get_oper() { return oper; }
  inline reg_state get_state() const { return state; }

  // when two live ranges have the same operand, this function merges
  // the data structure in this. lr can then be removed
  void combine_with(Live_range* lr);

  inline double get_priority() const;
  void calculate_priority();

  // only used for debugging
  inline double get_callee_benefit() const { return callee_benefit; }
  inline double get_caller_benefit() const { return caller_benefit; }

  Register_file& get_reg_file() const;

  // place op in the live range. the Pred_cookie is the predicate that
  // guards the op.
  void add_op(Op*, Pred_cookie);
  inline void add_ref(const El_ref&);
  inline unsigned num_refs() const { return refs.size(); }
  inline bool in_range(Op* op) const { return ops_set.is_member(op); }
  inline Hash_set<Op*>& live_ops() { return ops_set; }
  inline List<El_ref>& live_refs() { return refs; }

  inline void set_unspillable() { unspillable = true; }
  inline bool is_unspillable() const { return unspillable; }

  // check if lr interferes with this, and if so set inf_lrs appropriately.
  void set_interference(Live_range* lr);
  inline bool interferes(Live_range* lr) const { return inf_lrs.is_member(lr); }

  // this function checks if 'this' has been spilled, and if so
  // removes it from the interference graph. the new live ranges from
  // the spill code are inserted in its stead.
  void update_interference_graph();

  // if there's more registers in the machine than interfering live
  // ranges, then you're guaranteed to have a register free. this
  // function returns false if you're guaranteed to have a register
  // free.
  bool is_constrained();

  // try to assign a register to this live range.
  reg_state color();

  // remove the register assignment.
  void unbind();

private:
  Live_range() : pred_map(NULL) { }

  void spill();
  int select_register();
  void bind_register(int reg_num);

  Operand oper;
  reg_state state;

  // each use of oper in the code.
  List<El_ref> refs;

  // the ops that oper is live over.
  Hash_set<Op*> ops_set;

  // which predicate guards each op in the live range.
  Hash_map<Op*, Pred_cookie> pred_map;

  // used to determine the order which live_ranges are assigned to
  // registers. live_ranges with lower priority get spilled first.
  double the_priority;

  // similar to priority. used to determine whether this variable
  // should go in a caller or callee saved register.
  double caller_benefit;
  double callee_benefit;

  bool unspillable;

  // Live ranges that interfere with this.
  Hash_set<Live_range*> inf_lrs;

  // The registers already in use by interfering live ranges
  Bitvector cant_use;

};


ostream& operator<<(ostream&, const Live_range&);

#endif
