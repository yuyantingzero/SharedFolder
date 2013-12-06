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
//      File:           bnb_mapper.cpp
//      Author:         Nate Clark
//      Created:        March 2005 (code was written long before that)
//      Description:    branch and bound algorithm for selecting which ops
//                      go into custom instructions.
//
/////////////////////////////////////////////////////////////////////////////

#include "bnb_mapper.h"


// FIX: move this where it belongs.
void bnb_covering(bool, int, const Vector<Vector<Op*> >&,
		  const Hash_map<Op*, int>&, const Vector<unsigned>&,
		  const Vector<unsigned>&, Bitvector, unsigned, Bitvector,
		  Bitvector&);
void bisect_block(Hash_map<Op*, int>& op_to_index, Vector<Op*>& index_to_op,
		  Vector<List<Vector<Op*> > >& matches, Bitvector& valid_matches,
		  Hash_set<Op*>& blk1, Hash_set<Op*>& blk2);

// FIX: this is bad programming
unsigned best_value = 0;
long bnb_calls = 0;


void bnb_real_work(Vector<List<Vector<Op*> > >& matches, 
		   Hash_set<Op*>& ops_in_matches, Compound_region* reg)
{

  unsigned asdf = 0;
  for(unsigned k = 0; k < matches.size(); k++) {
    asdf += matches[k].size();
  }

  unsigned total_matches = 0, cur_match = 0;
  Bitvector valid_matches(asdf);
  unsigned k;
  for(k = 0; k < matches.size(); k++) {
    for(List_iterator<Vector<Op*> > li(matches[k]); li != 0; li++) {
      bool is_in_this_subset = true;
      Vector<Op*>& the_match = (*li);
      for(unsigned m = 0; m < the_match.size(); m++) {
	if(!ops_in_matches.is_member(the_match[m])) {
	  is_in_this_subset = false;
	  break;
	}
      }
      if(is_in_this_subset) {
	valid_matches.set_bit(cur_match);
	total_matches++;
      }
      cur_match++;
    }
  }

  if(total_matches == 0)
    return;

  Hash_map<Op*, int> op_to_index(hash_op_ptr, ops_in_matches.size());
  Vector<Op*> index_to_op(ops_in_matches.size());
  int cur_index = 0;
  for(Hash_set_iterator<Op*> hsi(ops_in_matches); hsi != 0; hsi++) {
    index_to_op[cur_index] = *hsi;
    op_to_index.bind(*hsi, cur_index++);
  }

  if(total_matches > 40) {
    Hash_set<Op*> blk1(hash_op_ptr, ops_in_matches.size() / 2),
      blk2(hash_op_ptr, ops_in_matches.size() / 2);
    cdbg << "Partitioning    ";
    bisect_block(op_to_index, index_to_op, matches, valid_matches, blk1, blk2);
    cdbg << " cuts" << endl;
    
    bnb_real_work(matches, blk1, reg);
    bnb_real_work(matches, blk2, reg);
    return;
  }

  // 3. flatten out the matches into a Vector. this is just easier
  // to manipulate.
  Vector<Vector<Op*> > flat_matches(total_matches);
  Vector<unsigned> match_to_cop_num(total_matches);
  unsigned match_ind = 0;
  cur_match = 0;
  for(k = 0; k < matches.size(); k++) {
    for(List_iterator<Vector<Op*> > li(matches[k]); li != 0; li++) {
      if(valid_matches.bit(match_ind)) {
	Vector<Op*> the_match = (*li);
	match_to_cop_num[cur_match] = k;
	flat_matches[cur_match++] = the_match;
      }
      match_ind++;
    }
  }

  // 4. set up a goodness function for each match. the sum of the
  // goodness across all matches will try to be maximized.
  Vector<unsigned> goodness(total_matches, 0);

  for(k = 0; k < total_matches; k++) {
    Custom_op_descr* cop1 = current_MDES->
      get_MDES_custom_op_descr(match_to_cop_num[k]);
    // FIX: this should incorporate some notion of slack.
    goodness[k] = 1 + cop1->get_pattern_size() -
      MDES_flow_time(cop1->get_opcode(), DATA_OUT, DEST1);
  }

  Vector<unsigned> goodness_below(total_matches);
  unsigned running_total = 0;
  for(int m = total_matches - 1; m > 0; m--) {
    goodness_below[m] = running_total;
    running_total += goodness[m];
  }


  // 5. figure out the optimal set of replacements through branch-and-bound.
  Bitvector best_soln(total_matches), cur_soln(total_matches),
    ops_used(op_to_index.size());
  // FIX:
#if 0
  cdbg << "Starting bnb for size: " << total_matches << "x" 
       << op_to_index.size() << endl;
#endif

  bnb_calls = 0;
  best_value = 0;
  bnb_covering(true,  0, flat_matches, op_to_index, goodness, goodness_below,
	       cur_soln, 0, ops_used, best_soln);
  bnb_covering(false, 0, flat_matches, op_to_index, goodness, goodness_below,
	       cur_soln, 0, ops_used, best_soln);
#if 0
  cdbg << "Done! " << best_soln << endl;
#endif

  // 6. do the replacing.

  for(int m = 0; m < (int)total_matches; m++) {
    Custom_op_descr* cop = current_MDES->
      get_MDES_custom_op_descr(match_to_cop_num[m]);
    if(best_soln.bit(m)) {
      Vector<Op*> match_instance = flat_matches[m];
      // make sure all the ops haven't been replaced;
      // this is no longer a valid check with the partitioning step...
//       bool valid = true;
//       for(int k = 0; k < match_instance.dim(); k++) {
// 	if(replaced_ops.is_member(match_instance[k]))	
// 	  valid = false;
//       }
//       if(valid) {
	cop_match_count[match_to_cop_num[m]]++;
	for(int k = 0; k < match_instance.dim(); k++) {
	  Op* op = match_instance[k];
// 	  replaced_ops += op;
	  cop_dmatch_count[match_to_cop_num[m]] += El_calc_op_weight(op);
	}
	// FIX:
	deque<Operand> asdf;

	replace(reg, cop, match_instance, asdf, asdf);
//       } else {
// 	// if we get here, then the branch and bound didn't do its job
// 	// correctly.
// 	assert(0);
//       }
    }
  }

}



void bisect_block(Hash_map<Op*, int>& op_to_index, Vector<Op*>& index_to_op,
		  Vector<List<Vector<Op*> > >& matches, Bitvector& valid_matches,
		  Hash_set<Op*>& blk1, Hash_set<Op*>& blk2)
{
  if(index_to_op.size() < 10) {
    El_warn("Warning: trying to partition a very small set...\n");
  }

  // FIX: we got rid of the shmetis binary, and want to port this
  // function to use the metis library. Unfortunately this function
  // uses hyper-edges, and it doesn't look like the library has
  // hyper-edge partitioning. Don't know how to fix this exactly...

  assert(0);


//   ofstream of;
//   of.open("block_to_bisect.hgr");
//   if(!of.good()) {
//     El_punt("trouble opening block_to_bisect.hgr!\n");
//   }

//   // first trip just counts the number of edges.
//   int num_edges = valid_matches.ones_count();

//   of << num_edges << " " << op_to_index.size() << "\n";
  
//   unsigned cur_match = 0;
//   for(unsigned i = 0; i < matches.size(); i++) {
//     for(List_iterator<Vector<Op*> > li(matches[i]); li != 0; li++) {
//       if(valid_matches.bit(cur_match)) {
// 	Vector<Op*> match = *li;
// 	for(unsigned j = 0; j < match.size(); j++) {
// 	  of << op_to_index[match[j]] + 1 << " ";
// 	}
// 	of << " \n";
//       }
//       cur_match++;
//     }
//   }

//   of << flush;
//   of.close();

//   // That perl thingy just captures the number of edges cut by the
//   // partitioning and prints it to the screen.
//   system("shmetis block_to_bisect.hgr 2 8 | perl -e 'while(<>){if(/Cut:\\s+(\\d+)/){print $1 ; }}'");
  
//   ifstream in;

//   in.open("block_to_bisect.hgr.part.2");
//   if(!in.good()) {
//     El_punt("trouble opening partition file!\n");
//   }
  
//   int partition, ind = 0;
//   while(ind < (int)op_to_index.size()) {
//     in >> partition;
//     if(partition == 0) {
//       blk1 += index_to_op[ind];
//     } else {
//       blk2 += index_to_op[ind];
//     }
//     ind++;
//   }
//   in.close();

//   system("rm -f block_to_bisect.hgr block_to_bisect.hgr.part.2 bisection.output");
}


void bnb_covering(bool use_this_match, int match_num,
		  const Vector<Vector<Op*> >& flat_matches,
		  const Hash_map<Op*, int>& op_to_index,
		  const Vector<unsigned>& goodness,
		  const Vector<unsigned>& goodness_below,
		  Bitvector  cur_soln,  unsigned  cur_value,
		  Bitvector ops_used,
		  Bitvector& best_soln)
{
  bnb_calls++;
  if(bnb_calls != 0 && (bnb_calls % 10000000) == 0)
    cdbg << "  " << bnb_calls/10000000 << "e8  " << cur_value << " "
	 << best_value << endl;

  if(use_this_match) {
    for(unsigned i = 0; i < flat_matches[match_num].size(); i++) {
      if(ops_used.bit(op_to_index[flat_matches[match_num][i]])) {
	return;
      }
    }

    cur_value += goodness[match_num];
    if((cur_value + goodness_below[match_num]) < best_value)
      return;

    for(unsigned i = 0; i < flat_matches[match_num].size(); i++) {
      ops_used.set_bit(op_to_index[flat_matches[match_num][i]]);
    }
    cur_soln.set_bit(match_num);
    
    if(cur_value > best_value) {
      best_value = cur_value;
      best_soln  = cur_soln;
    }
  } else if((cur_value + goodness_below[match_num]) < best_value) {
    return;
  }

  if(match_num + 1 == (int)flat_matches.size()) {
    return;
  }

  // some heuristic pruning for large blocks.
//   if(flat_matches.size() > 100) {
//     if((cur_value + (goodness_below[match_num] / 2)) < best_value)
//       return;
//   }

  bnb_covering(true,  match_num + 1, flat_matches, op_to_index, goodness,
	       goodness_below, cur_soln, cur_value, ops_used, best_soln);
  bnb_covering(false, match_num + 1, flat_matches, op_to_index, goodness,
	       goodness_below, cur_soln, cur_value, ops_used, best_soln);

}
