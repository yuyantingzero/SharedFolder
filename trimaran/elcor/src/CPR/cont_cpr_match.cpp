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
 *      File:           cont_cpr_match.cpp
 *      Author:         Mike Schlansker
 *      Created:        March 1997
 *      Description:    control cpr pattern match for HB
 *
 *===========================================================================*/

#include <iostream>
#include "iterators.h"
#include "defs.h"
#include "opcode.h"
#include "opcode_properties.h"
#include "region_utilities.h"
#include "operand.h"
#include "el_port.h"
#include "vector.h"
#include "cont_cpr_descr.h"
#include "cont_cpr_match.h"
#include "tuples.h"
#include "dbg.h"
#include "print_cont_cpr_list.h"
#include "el_cpr_init.h"

using namespace std;


/*
 * Notes: There are now 3 versions of the pattern match
 *	1. original - fully functional original cpr pattern match
 *	2. maxreduce - suitability, max block length only enforced
 *	3. suitability - only suitability enforced
 *	
 */

/*===========================================================================*/
/*
 *	Original pattern match
 */
/*===========================================================================*/

Cont_cpr_match_result cpr_match_hb_original (Compound_region* blk){
  // pattern match applies to superblock or hyperblock
  // predicates use must be static single assignment
  // scan the hyper block and build up a hb-descr
  HB_cont_cpr_descr hb_descr(blk);
  Hash_set<Operand> dn_defs(hash_operand, 223);
  if(dbg(cpr,5))
    {
      cout << "Matching control block: " << blk->id() << endl;
      cout << *blk << endl;
    }
  int num_branches=0; // count the branches as we go
  for (Region_ops_C0_order opc(blk); opc != 0; ++opc) {
    // perform the match analysis
    Op* op =  (*opc);
    if(is_cmpp(op)) {// for a compare
      if(dbg(cpr,4)) {cout << "op" << op->id() << " is a compare"<< endl;}
      hb_descr.insert_compare(op);
    } else
      if(is_conditional_branch(op) || is_brl(op) || is_bru(op) ) { //
	if(dbg(cpr,4)){cout << "op" << op->id() << " is a branch"<< endl;}
	hb_descr.insert_branch(op);
	num_branches++;
      }     
  }
  //num_branches counts the number of branches in the cont_cpr_descr
  Cont_cpr_match_result result; //construct and initialize cpr_blk_list & cpr_blk_type
  Dlist< Pair<Op*, Op*> >  cpr_blk; // declare current cpr block
  Pair<Op* , Op*> curr_tuple; // tuple for potential inclusion into result
  int curr_br=0; //indicates last branch already in cpr block
  int cand_br; //indicates candidate for potentially joining block
  int prev_blk_last_br=0; //last branch in previous block
  bool blk_type=false;
  El_Cpr_Match_Info blk_info;
  while(true){
    cpr_blk.clear(); //begin building new CPR block 
    curr_br=prev_blk_last_br+1;
    if(curr_br>num_branches) break;
    curr_tuple=hb_descr.get_tuple(curr_br);
    cpr_blk.push_tail(curr_tuple); //always add first branch to cpr block
    if(dbg(cpr,4)) {
      cout << endl << "pushing tuple into new cpr block: " ;
      print_tuple(cout, curr_tuple);
      cout << endl;
    }
    hb_descr.suitability_init(curr_br); //initialize compare suitability test
    hb_descr.separability_init(curr_br);
    hb_descr.blocking_init(curr_br);
    bool predict_taken=false;
    bool predict_pausea=false;
    bool predict_break=false;
    blk_info = EL_CPR_MATCH_UNKNOWN;
    while(true){ //may append additonal branches to CPR block
      if(hb_descr.predict_taken(curr_br))  //complete tests which break after inclusion
	{
	  if (dbg(cpr,4)) { cout << "predict taken current\n";}
	  blk_type=true; //block of type taken block
	  blk_info = EL_CPR_MATCH_PRED_TAKEN;
	  break;
	}
      if( hb_descr.predict_break(curr_br) )
	{
	  if (dbg(cpr,4)) {cout<<"predicted break current"<<endl;}
	  blk_info = EL_CPR_MATCH_BREAK;
	  break;
	} 
      if(hb_descr.predict_pausea(curr_br))
	{
	  if (dbg(cpr,4)) { cout << "predict pausea current\n";}
	  blk_type=false;
	  blk_info = EL_CPR_MATCH_PAUSEA;
	  break;
	}
      cand_br=curr_br+1;
      if(cand_br > num_branches) 
        {
          blk_info = EL_CPR_MATCH_END;
          break;
	}
      if (dbg(cpr,4)) {cout<< "Considering candidate to be appended:" << cand_br << endl;}
      if(!hb_descr.suitability_test(cand_br)) {
        if (dbg(cpr,4))
	  {cout << "Suitability test failed -- candidiate not appended" << endl;} 
	blk_info = EL_CPR_MATCH_SUITABILITY;
        break; // preform suitability test and cut block if fails
      }
      if(!hb_descr.separability_test(cand_br)) {
        if (dbg(cpr,4))
          cout << "Separability test failed -- candidiate not appended" << endl; 
	blk_info = EL_CPR_MATCH_SEPARABILITY;
        break;
      }
      if(hb_descr.predict_taken(cand_br)&&hb_descr.can_move_back(prev_blk_last_br+1,cand_br))
      { // CPR to taken branch case
        if (dbg(cpr,4)) {cout << "Predict taken(cand_br)\n";}
	predict_taken=true;
      }

      if(!predict_taken && hb_descr.predict_break(cand_br)) {
	if (dbg(cpr,4)) {cout << "Predict break\n";} 
	predict_break=true; //preclude growth of next block
	blk_info = EL_CPR_MATCH_BREAK;
	break; 
      }

      //blocking test not applied in taken branch case
      if (!predict_taken && !predict_break && !hb_descr.sum_exit_freq_test(cand_br)) {
        if (dbg(cpr,4))  {cout << "Exit frequency exceeded" << endl;} 
	blk_info = EL_CPR_MATCH_EXIT_RATIO;
        break;
      }

      if(!predict_taken && hb_descr.predict_pauseb(cand_br)) {
        if (dbg(cpr,4)) {cout << "Predict pauseb\n";} 
	blk_info = EL_CPR_MATCH_PAUSEB;
	break; 
      }

      if(!predict_taken && hb_descr.predict_pausea(cand_br)) {
        if (dbg(cpr,4)) {cout << "Predict pausea\n";} 
	predict_pausea=true;
      }

      if (!hb_descr.max_block_length_test(cand_br)) {
        if (dbg(cpr,4)) {cout << "Max block length\n";} 
	blk_info = EL_CPR_MATCH_MAX_BR;
	break;
      }

      //passed all tests which break before inclusion
      curr_br=cand_br; 
      curr_tuple = hb_descr.get_tuple(curr_br);
      cpr_blk.push_tail(curr_tuple); //append candidate branch to cpr block
      if(dbg(cpr,4))
	{
	  cout << "Pushing tuple into previous cpr block. " ;
	  print_tuple(cout, curr_tuple);
	  cout << endl;
	} 
    }
    // add new cpr block to list if of appropriate size
    if((int) cpr_blk.size() >= El_cpr_min_block_length)
      {
     if(dbg(cpr,4))
	{
	  cout << "Adequate sized block pushed, type: " << blk_type << endl;
	}
	result.cpr_blk_list.push_tail(cpr_blk);
	result.cpr_blk_type.push_tail(blk_type);
	result.cpr_blk_info.push_tail(blk_info);
      }
    else
      {
	curr_br=prev_blk_last_br+1; // roll back to block of length one
	curr_tuple=cpr_blk.head();  //keep only the first tuple from the current block
	cpr_blk.clear(); cpr_blk.push_tail(curr_tuple);
	result.cpr_blk_list.push_tail(cpr_blk);
	result.cpr_blk_type.push_tail(false);
	result.cpr_blk_info.push_tail(blk_info);
        if (dbg(cpr,4)) 
	  {
	    cout << "Block too short, fabricating singleton cpr block. " ;
	    print_tuple(cout, curr_tuple);
	    cout << endl;
	  }
      }
    prev_blk_last_br=curr_br;
    if (dbg(cpr,4)) {
      cout << "prev_blk_last_br=" << prev_blk_last_br << endl;
    }
  }
  return result;
}

/*===========================================================================*/
/*
 *	The max reduce pattern match is a much weaker pattern match,
 *	only the suitability test is enforced.  Essentially the same code
 *	as above, minus all tests besides suitability.    SAM 2-98
 *	3-98: Added simple blocking test to the pattern match to make
 *	sure you don't get uncontrollably large cpr blocks.
 */
/*===========================================================================*/

Cont_cpr_match_result cpr_match_hb_maxreduce (Compound_region* blk)
{
  // pattern match applies to superblock or hyperblock
  // predicates use must be static single assignment
  // scan the hyper block and build up a hb-descr
  HB_cont_cpr_descr hb_descr(blk);
  Hash_set<Operand> dn_defs(hash_operand, 223);
  if(dbg(cpr,5))
    {
      cout << "Matching control block: " << blk->id() << endl;
      cout << *blk << endl;
    }
  int num_branches=0; // count the branches as we go
  for (Region_ops_C0_order opc(blk); opc != 0; ++opc) {
    // perform the match analysis
    Op* op =  (*opc);
    if(is_cmpp(op)) {// for a compare
      if(dbg(cpr,4)) {cout << "op" << op->id() << " is a compare"<< endl;}
      hb_descr.insert_compare(op);
    } else
      if(is_conditional_branch(op) || is_brl(op) || is_bru(op) ) { //
        if(dbg(cpr,4)){cout << "op" << op->id() << " is a branch"<< endl;}
        hb_descr.insert_branch(op);
        num_branches++;
      }
  }
  //num_branches counts the number of branches in the cont_cpr_descr
  Cont_cpr_match_result result; //construct and initialize cpr_blk_list & cpr_blk_type
  Dlist< Pair<Op*, Op*> >  cpr_blk; // declare current cpr block
  Pair<Op* , Op*> curr_tuple; // tuple for potential inclusion into result
  int curr_br=0; //indicates last branch already in cpr block
  int cand_br; //indicates candidate for potentially joining block
  int prev_blk_last_br=0; //last branch in previous block
  El_Cpr_Match_Info blk_info;

  while(true){ // build new CPR block
    cpr_blk.clear();
    curr_br=prev_blk_last_br+1;
    if(curr_br>num_branches) break;
    curr_tuple=hb_descr.get_tuple(curr_br);
    cpr_blk.push_tail(curr_tuple); //always add first branch to cpr block
    if(dbg(cpr,4)) {
      cout << "pushing tuple into new cpr block: " ;
      print_tuple(cout, curr_tuple);
      cout << endl;
    }
    hb_descr.suitability_init(curr_br); //initialize compare suitability test
    hb_descr.blocking_init(curr_br);
    bool type=false; // initialize to fall_through list
    blk_info = EL_CPR_MATCH_UNKNOWN;
    while(true){ //may append additonal branches to CPR block
      cand_br=curr_br+1;
      if(cand_br > num_branches) {
          blk_info = EL_CPR_MATCH_END;
          break;
      }
      if (dbg(cpr,4))
        cout << "+Considering candidate to be appended: " << cand_br << endl;
      if(!hb_descr.suitability_test(cand_br)) {
        if (dbg(cpr,4))
          cout << "\tSuitability test failed on candidiate - not appended" << endl;
	blk_info = EL_CPR_MATCH_SUITABILITY;
        break;
      }
      if (!hb_descr.max_block_length_test(cand_br)) {
        if (dbg(cpr,4))
          cout << "\tMax block length exceeded  - cand not appended" << endl; 
	blk_info = EL_CPR_MATCH_MAX_BR;
        break;
      }
      //passed all tests
      curr_br=cand_br;
      curr_tuple = hb_descr.get_tuple(curr_br);
      cpr_blk.push_tail(curr_tuple); //append candidate branch to cpr block
      if(dbg(cpr,4))
        {
          cout << "pushing tuple into previous cpr block: " ;
          print_tuple(cout, curr_tuple);
          cout << endl;
        }
    }
    result.cpr_blk_list.push_tail(cpr_blk);
    type = hb_descr.predict_taken(curr_br);
    result.cpr_blk_type.push_tail(type);
    result.cpr_blk_info.push_tail(blk_info);
    prev_blk_last_br=curr_br;
  }
  return result;
}

/*===========================================================================*/
/*
 *	Suitability pattern match - only suitability test enforced.
 */
/*===========================================================================*/

Cont_cpr_match_result cpr_match_hb_suitability (Compound_region* blk)
{
  // pattern match applies to superblock or hyperblock
  // predicates use must be static single assignment
  // scan the hyper block and build up a hb-descr
  HB_cont_cpr_descr hb_descr(blk);
  Hash_set<Operand> dn_defs(hash_operand, 223);
  int num_branches=0; // count the branches as we go
  for (Region_ops_C0_order opc(blk); opc != 0; ++opc) {
    // perform the match analysis
    Op* op =  (*opc);
    if(is_cmpp(op)) {
      hb_descr.insert_compare(op);
    }
    else if(is_conditional_branch(op) || is_brl(op) || is_bru(op) ) {
        hb_descr.insert_branch(op);
        num_branches++;
    }
  }
  //num_branches counts the number of branches in the cont_cpr_descr
  Cont_cpr_match_result result; //construct and initialize cpr_blk_list & cpr_blk_type
  Dlist< Pair<Op*, Op*> >  cpr_blk; // declare current cpr block
  Pair<Op* , Op*> curr_tuple; // tuple for potential inclusion into result
  int curr_br=0; //indicates last branch already in cpr block
  int cand_br; //indicates candidate for potentially joining block
  int prev_blk_last_br=0; //last branch in previous block
  El_Cpr_Match_Info blk_info;

  while(true){ // build new CPR block
    cpr_blk.clear();
    curr_br=prev_blk_last_br+1;
    if(curr_br>num_branches) break;
    curr_tuple=hb_descr.get_tuple(curr_br);
    cpr_blk.push_tail(curr_tuple); //always add first branch to cpr block
    hb_descr.suitability_init(curr_br); //initialize compare suitability test
    bool type=false; // initialize to fall_through list
    blk_info = EL_CPR_MATCH_UNKNOWN;
    while(true){ //may append additonal branches to CPR block
      cand_br=curr_br+1;
      if(cand_br > num_branches) {
	blk_info = EL_CPR_MATCH_END;
	break;
      }
      if(!hb_descr.suitability_test(cand_br)) {
        El_warn("cont_cpr_match_suitability - test failed for blk %d br %d",
			blk->id(), cand_br);
	blk_info = EL_CPR_MATCH_SUITABILITY;
        break;
      }
      //passed all tests
      curr_br=cand_br;
      curr_tuple = hb_descr.get_tuple(curr_br);
      cpr_blk.push_tail(curr_tuple); //append candidate branch to cpr block
    }
    result.cpr_blk_list.push_tail(cpr_blk);
    type = hb_descr.predict_taken(curr_br);
    result.cpr_blk_type.push_tail(type);
    result.cpr_blk_info.push_tail(blk_info);
    prev_blk_last_br=curr_br;
  }
  return result;
}
