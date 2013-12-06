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



/*===========================================================================
 *
 *      File:           cont_cpr_descr.h
 *      Author:         Mike Schlansker
 *      Created:        March 1997
 *      Description:    control cpr pattern match for HB
 *
 *===========================================================================*/
#ifndef CONT_CPR_DESCR_H
#define CONT_CPR_DESCR_H

#include "op.h"
#include "vector.h"
#include "hash_map.h"
#include "hash_set.h"
#include "dlist.h"
#include "tuples.h"
#include "region.h"

#define MAX_VECT 200

class Compare_descr{
  friend class HB_cont_cpr_descr;
public:
  Compare_descr(Op*);
  ~Compare_descr();
  void clear();
  Op* link_to_compare; // link to this compare op
  Op* link_to_branch; // link to subsequent directly controlled branch
  Compare_descr* link_to_guard_cmp; //link to cmp guarding this cmp
private:
};

class HB_cont_cpr_descr{
public:
  HB_cont_cpr_descr(Compound_region* hyper_blk);
  ~HB_cont_cpr_descr();
  void clear();
  void insert_branch(Op* branch);
  void insert_compare(Op* cmp);
  // int provides index of branch being tested
  // true if compare corresponding to indexed branch not in successor set
  void append_successors(int br);
    //compute successor list of compare corresponding to given branch
  Pair<Op*, Op*> get_tuple(int br);
  void suitability_init(int br);
  bool suitability_test(int br);
  void separability_init(int br); // initializer for separability test
  bool separability_test(int br); // test & update for separability test
  bool predict_break(int exit_br); //causes CPR block to break both before and after cand branch
  bool predict_taken(int exit_br);
  bool predict_pauseb(int exit_br);//causes CPR block to break before cand branch
  bool predict_pausea(int exit_br);//causes CPR block to break after cand branch
  bool can_move_back(int beg_br, int end_br);
  void blocking_init(int br);
  bool sum_exit_freq_test(int br);
  bool max_block_length_test(int br);


private:
  int num_branches;
  Vector<Op*> branch_vect;
  Vector<Compare_descr*> compare_vect;
  Hash_map<Operand,Compare_descr*> live_compares;
  //maps live operand to op which computes it
  List<Compare_descr*> all_compares; //used to deallocate when done
  Hash_set< Op* > successors;
  Hash_set<Compare_descr*> cmp_set; //cmps which can guard new cmp in cpr block

  Compound_region* region_ptr;

  Vector<double> weight_vect;  // HB exit frequencies
  int first_branch_index;      // first branch of the current cpr block
  double incoming_weight;      // incoming weight of current cpr block
  Compare_descr* true_compare_descr; //fictitions compare which gens true
  Operand pred_true;
};





#endif
