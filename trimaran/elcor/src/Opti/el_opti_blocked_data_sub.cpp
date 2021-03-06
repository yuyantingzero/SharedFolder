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




///////////////////////////////////////////////////////////////////////////////
//
//      File:           el_opti_blocked_data_sub.cpp
//      Authors:        Sumedh Sathaye
//      Created:        August 1996
//      Description:    Blocked data substitution (data CPR)
//
//      Copyright (C) 1996 Hewlett-Packard Company 
//
//      Interface routine lies at the end of this file.
//
///////////////////////////////////////////////////////////////////////////////

/* See the comment in el_opti_litfold.h */

#include <iostream>
#include <fstream>
#include "el_main.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "hash_functions.h"
#include "el_opti_blocked_data_sub.h"
#include "dbg.h"

using namespace std;

#define MAX_BLOCKED_DATA_SUB 10000

Bds_op::Bds_op (void)
  : op (0), srcreg (0), srcreg2 (0), destreg (0), chain (0)
{
}

Bds_op::Bds_op (Op* o, Operand* sreg, Operand* sreg2, Operand* dreg)
{
  op = o;
  srcreg = sreg;
  srcreg2 = sreg2;
  destreg = dreg;
  chain = 0;
}

Bds_op::~Bds_op (void)
{
}

bool Bds_op::operator== (Bds_op op2)
{
  return (srcreg == op2.srcreg
	  && srcreg2 == op2.srcreg2
	  && destreg == op2.destreg); // no need to check Bds_chain m'ship
}
///////////////////////////////////////////////////////////////////////////////

Bds_chain::Bds_chain (void)
{
  oplist = new List<Bds_op*>;
}

Bds_chain::~Bds_chain (void)
{
  delete oplist;
}
///////////////////////////////////////////////////////////////////////////////

Bds_solver::Bds_solver (Region* r)
  : rgn (r), eligible_op_counter (0), total_bdss (0)
{
  Hash_map_iterator<El_ref, List<El_ref>*> ti;
  List_iterator<El_ref> li;

  // get the reaching defs and the maps:
  create_local_analysis_info_for_all_hbs_bbs ((Compound_region*)rgn);
  El_do_reaching_defs (rgn, ANALYZE_ALLREG);
  rdinfo = get_reaching_defs_info (rgn);
  rdmaps = new Reaching_defs_maps (rgn);

  // print ud/du chains as you got them:
  if (dbg (opti, 10)) {
    Reaching_defs_info* rdi = get_reaching_defs_info (rgn);

    cdbg << "*\n*\tPrinting the def-use chains:" << endl;
    for (ti (rdi->def_use); ti != 0; ti++) {
      cdbg << "*\tdef-" << (rdmaps->def_to_i_map.value ((*ti).first))
	   << " = " << (*ti).first << endl;
      for (li (*((*ti).second)); li != 0; li++)
	cdbg << "*\t\t<-> use-" << (rdmaps->use_to_i_map.value (*li))
	     << " " << *li << endl;
    }

    cdbg << "*\n*\tPrinting the use-def chains:" << endl;
    for (ti (rdi->use_def); ti != 0; ti++) {
      cdbg << "*\tuse-" << (rdmaps->use_to_i_map.value ((*ti).first))
	   << " = " << (*ti).first << endl;
      for (li (*((*ti).second)); li != 0; li++)
	cdbg << "*\t\t<-> def-" << (rdmaps->def_to_i_map.value (*li)) << " "
	     << *li << endl;
    }
    cdbg << "*" << endl;
  }

  // the list of bds chains:
  bds_chainlist = new List<Bds_chain*>;

  // bitvector later used in detecting bds chains:
  op_bdschain_member.resize (rgn->get_next_op_id_num());
}

Bds_solver::~Bds_solver (void)
{
  remove_reaching_defs_info (rgn);
  delete_local_analysis_info_for_all_hbs_bbs ((Compound_region*)rgn);
  delete rdmaps;

  // delete the Bds_op members in the eligible_op_map:
  for (Map_iterator<Op*, Bds_op*> miolo (eligible_op_map);
       miolo != 0; miolo++)
    delete ((*miolo).second);

  // delete list of bds_chains:
  for (List_iterator<Bds_chain*> lilc (*bds_chainlist);
       lilc != 0; lilc++)
    for (List_iterator<Bds_op*> lilo (*((*lilc)->oplist));
	 lilo != 0; lilo++)
      delete *lilo;
  delete bds_chainlist;
}

void Bds_solver::build_bds_chains (void)
{
  test_op_primary_eligibility ();
  gather_chains_from_eligible_ops ();
}

void Bds_solver::test_op_primary_eligibility (void)
{
  if (dbg (opti, 6))
    cdbg << "*\t\tTesting primary eligibilty" << endl;

  for (Region_ops_C0_order opi ((Compound_region*)rgn); opi != 0; opi++) {
    Op* op = (*opi);

    // ineligible is not int/float op (includes double):
    if (!is_ialu (op) && !is_falu (op))
      continue;

    if (is_ialu_simple_binary_op (op)) {
      if (dbg (opti, 6))
        cdbg << "*\t\t op-" << op->id() << " :simple binary integer op: "
	  //<< el_opcode_to_string_map.value (op->opcode()) << endl;
	  //tangw 06-03-02
	     << get_mdes_opcode_string(op) << endl;

      // check out the sources:
      Operand* srcreg = 0; Operand* srcreg2 = 0;
      bool found_srcreg = false; bool found_srcreg2 = false;
      bool src_eligible = true;
      for (Op_explicit_sources oesi (op); oesi != 0; oesi++) {
	Operand* oper = &(*oesi);

	// if a src reg:
	if (oper->is_reg() || oper->is_macro_reg()) {
	  if ((found_srcreg) && (found_srcreg2))
	    src_eligible = false;
	  else {
	    if (!found_srcreg) {
	      srcreg = oper;
	      found_srcreg = true;
	    }
	    else if (!found_srcreg2) {
	      srcreg2 = oper;
	      found_srcreg2 = true;
	    }
	    else
	      assert (0);	// should be un-reachable
	  }
	}
	else
	  src_eligible = false;
      }

      if (!src_eligible) {
	if (dbg (opti, 6))
	  cdbg << "*\t\t\t... src eligibility failure" << endl;

	continue;
      }

      // check out the dests:
      Operand* destreg = 0;
      bool found_destreg = false;
      bool dest_eligible = true;
      for (Op_explicit_dests oedi (op); oedi != 0; oedi++) {
	Operand* oper = &(*oedi);

	// if a dest reg:
	if (oper->is_reg() || oper->is_macro_reg()) {
	  if (found_destreg)
	    dest_eligible = false;
	  else {
	    destreg = oper;
	    found_destreg = true;
	  }
	}
	else
	  dest_eligible = false;
      }

      if (!dest_eligible) {
	if (dbg (opti, 6))
	  cdbg << "*\t\t\t... dest eligibilty failure" << endl;

	continue;
      }

      // add to the map of eligible ops:
      Bds_op* bdsop = new Bds_op (op, srcreg, srcreg2, destreg);
      eligible_op_map.bind (op, bdsop);
      eligible_op_counter++;
    }

    // similarly, but later:
    if (is_falu_simple_binary_op (op)) {
      if (dbg (opti, 6))
        cdbg << "*\t\t\t is simple binary float/double op: "
	  //<< (el_opcode_to_string_map.value (op->opcode())) << endl
	  //tangw 06-03-02
	     <<(get_mdes_opcode_string(op)) << endl 
	     << "*\t\t\t and is currently ignored." << endl;
    }

    // dont know how to handle the non-binary & complex operations!
    if (is_ialu_complex_binary_op (op)) {
    }
  }

  if (dbg (opti, 6)) {
    cdbg << "*\n*\t\tEligible ops are:" << endl;
    Map_iterator<Op*, Bds_op*> miolo;
    for (miolo (eligible_op_map); miolo != 0; miolo++) {
      cdbg << "*\top-" << ((*miolo).second->op->id()) << ","
	   << " srcreg=[" << (*((*miolo).second->srcreg)) << "],"
	   << " srcreg2=[" << (*((*miolo).second->srcreg2)) << "],"
	   << " destreg=[" << (*((*miolo).second->destreg)) << "]" << endl;
    }
  }
}

void Bds_solver::gather_chains_from_eligible_ops (void)
{
  if (dbg (opti, 6))
    cdbg << "*\n*\t\tGathering chains for eligible ops:" << endl;

  Map_iterator<Op*, Bds_op*> miolo;
  for (miolo (eligible_op_map); miolo != 0; miolo++) {
    Bds_chain* curchain = 0;

    if (dbg (opti, 6))
      cdbg << "*\t\top-" << (*miolo).second->op->id() << endl;

    // if not already a member of some chain:
    if ((curchain = (*miolo).second->chain) == 0) {
      // create a new chain:
      curchain = new Bds_chain ();
      bds_chainlist->add_tail (curchain);

      // chain this one:
      (*miolo).second->chain = curchain;
      curchain->oplist->add_tail ((*miolo).second);

      // down-stream search:
      def_dfs_to_gather_chain ((*miolo).second, curchain);

      // up-stream search:
      #if THIS_IS_NOT_NEEDED_SINCE_WE_DO_C0_ORDER_SEARCH
      use_dfs_to_gather_chain ((*miolo).second, curchain);
      #endif

      // mark it now:
      op_bdschain_member.set_bit ((*miolo).second->op->id());
    }
  }

  if (dbg (opti, 6)) {
    cdbg << "*\n*\t\tBds chains are:" << endl;
    List_iterator<Bds_chain*> lilc (*bds_chainlist);
    for (; lilc != 0; lilc++) {
      cdbg << "*\t\t\tChain ops: [";
      for (List_iterator<Bds_op*> lilo (*((*lilc)->oplist));
	   lilo != 0; lilo++)
	cdbg << " " << (*lilo)->op->id();
      cdbg << "]" << endl;
    }
  }

  if (dbg (opti, 6))
    cdbg << "*\t\tDone gathering chains for eligible ops:" << endl;
}

void
Bds_solver::use_dfs_to_gather_chain (Bds_op* bdsin, Bds_chain* chain)
{
  // if already a member of some chain, ignore: (current heuristic)
  if (op_bdschain_member.bit (bdsin->op->id()))
    return;

  El_ref* useref = new El_ref (bdsin->op, bdsin->srcreg, EXP_SRC);
  List<El_ref>* udchain = rdinfo->get_ud_chain (*useref);

  if (!udchain)
    return;

  if (udchain->size() != 1)
    return;

  List_iterator<El_ref> lier (*udchain);
  if ((*lier).get_op() == bdsin->op) {
    delete useref;
    return;
  }

  // if already a member of some chain, ignore: (current heuristic)
  if (op_bdschain_member.bit ((*lier).get_op()->id())) {
    delete useref;
    return;
  }

  if (eligible_op_map.is_bound ((*lier).get_op())
      && (*lier).get_op()->opcode() == bdsin->op->opcode()) {
    // mark as member of a chain:
    op_bdschain_member.set_bit ((*lier).get_op()->id());

    // search the drain:
    use_dfs_to_gather_chain (eligible_op_map.value ((*lier).get_op()), chain);

    // declare:
    if (dbg (opti, 6))
      cdbg << "*\t\t\top-" << ((*lier).get_op()->id())
	   << " is on it (use_dfs)" << endl;

    // assign membership of this chain:
    eligible_op_map.value ((*lier).get_op())->chain = chain;
    chain->oplist->add_tail (eligible_op_map.value ((*lier).get_op()));

    delete useref;
  }
}

void Bds_solver::def_dfs_to_gather_chain (Bds_op* bdsin, Bds_chain* chain)
{
  // if already a member of some chain, ignore: (current heuristic)
  if (op_bdschain_member.bit (bdsin->op->id()))
    return;

  El_ref* defref = new El_ref (bdsin->op, bdsin->destreg, EXP_DEST);
  List<El_ref>* duchain = rdinfo->get_du_chain (*defref);

  // action: (there maybe more than one uses of this def)
  if (duchain) {
    List_iterator<El_ref> lier;
    for (lier (*duchain); lier != 0; lier++) {
      if ((*lier).get_op() == bdsin->op)
	continue;

      // if already a member of some chain, ignore: (current heuristic)
      if (op_bdschain_member.bit ((*lier).get_op()->id()))
	continue;

      if (eligible_op_map.is_bound ((*lier).get_op())
	  && (*lier).get_op()->opcode() == bdsin->op->opcode()) {
	// mark as member of a chain:
	op_bdschain_member.set_bit ((*lier).get_op()->id());

	// search down the drain:
	def_dfs_to_gather_chain (eligible_op_map.value ((*lier).get_op()),
				 chain);
	// declare:
	if (dbg (opti, 6))
	  cdbg << "*\t\t\top-" << ((*lier).get_op()->id())
	       << " is on it (def_dfs)" << endl;

	// assign membership to this chain:
	eligible_op_map.value ((*lier).get_op())->chain = chain;
	chain->oplist->add_tail (eligible_op_map.value ((*lier).get_op()));
      }
      else
	continue;
    }
  }

  // cleanup:
  if (defref)
    delete defref;
}

void Bds_solver::data_substi_bds_chains (void)
{
  /* TODO:
   * This implemetation is not complete yet. Here is what is missing:
   *
   * 0. The check for liveness: see the comment and the invocation in
   *    the code for literal folding (method: "backsubsti_litfold_chains").
   *
   * 1. Dont have the way to know which op in the BDS chain to subject to
   *    this optimization. This is to be determined by the "blocking"
   *    heuristic.
   *
   * 2. Dont know how to combine the computation with the registers at
   *    hand. (Literals were easy: I could just perform the operation
   *    and replace the current literal with the result of the operation).
   *
   * 3. (2) above requires reorganization of the expression that is
   *    generated when actual blocked data substitution is done. This has
   *    to be matched with the resource availability, so as to derive some
   *    benefit.
   *    This transformation does not necessarily result into better code:
   *    the resource availability will constrain the usefuleness of this
   *    transformation.
   */
  #if OLD_LIFOLDING_CODE_TO_BE_REPLACED_BY_BLOCKED_DATA_SUBSTI_CODE
  for (List_iterator<Bds_chain*> lilc (*bds_chainlist);
       lilc != 0; lilc++) {

    // starting the second op in the chain:
    //   get its literal operand,
    //   perform: [(first literal operand) operation (this literal operand)]
    //   replace the srcreg for this op with destreg for the first op;
    if (dbg (opti, 6))
      cdbg << "*\t\top-" << ((*lilc)->oplist->head()->op->id()) << endl;

    Operand first_destreg (*((*lilc)->oplist->head()->destreg));
    Operand prev_lit (*((*lilc)->oplist->head()->srclit));

    List_iterator<Bds_op*> lo (*((*lilc)->oplist));
    bool first=true;
    for (; lo != 0; lo++) {
      if (first) {
	first = false;
	continue;
      }

      if (is_ialu_simple_binary_op ((*lo)->op)) {
	IR_ROOT_OPCODE t = get_root ((*lo)->op->opcode());

	switch (t) {
	case ROOT_ADD:
	case ROOT_ADDL:
	  // change the literal:
	  if (dbg (opti, 6))
	    cdbg << "*\t\t\top-" << (*lo)->op->id();

	  if (dbg (opti, 6))
	    cdbg << ", old lit=" << ((*lo)->srclit->int_value());
	  (*lo)->srclit->set_int_value ((*lo)->srclit->int_value()
					  + prev_lit.int_value());
	  if (dbg (opti, 6))
	    cdbg << ", new lit=" << (*lo)->srclit->int_value() << endl;

	  // the srcreg:
	  if (dbg (opti, 6))
	    cdbg << "*\t\t\told srcreg=" << (*lo)->srcreg->vr_num();
	  (*lo)->srcreg->rename ((*lilc)->oplist->head()->srcreg->vr_num());
	  if (dbg (opti, 6))
	    cdbg << ", new srcreg=" << (*lo)->srcreg->vr_num() << endl;
	  break;

	case ROOT_AND:
	  break;

	case ROOT_OR:
	  break;

	case ROOT_DIV:
	case ROOT_DIVL:
	  break;

	case ROOT_MPY:
	case ROOT_MPYL:
	  break;

	case ROOT_REM:
	case ROOT_REML:
	  break;

	case ROOT_SHL:
	  break;

	case ROOT_SHR:
	  break;

	case ROOT_SHLA:
	  break;

	case ROOT_SHRA:
	  break;

	case ROOT_SUB:
	  break;

	case ROOT_SUBL:
	  break;

	case ROOT_XOR:
	  break;

	default:
	  break;
	}

	// reset the prev literal value:
	prev_lit.set_int_value ((*lo)->srclit->int_value());
      }

      // did not get around to handle these (yet)
      if (is_falu_simple_binary_op ((*lo)->op)) {
      }

      // did not get around to handle these (yet)
      if (is_ialu_complex_binary_op ((*lo)->op)) {
      }
    }
  }
  #endif
}
///////////////////////////////////////////////////////////////////////////////

void El_do_blocked_data_sub (Region* rgn)
{
  if (dbg (opti, 1)) {
    cdbg << "*\tInvoking BDSer, ";
    if (rgn->is_procedure())
      cdbg << "procedure=" << (((Procedure*)rgn)->get_name()) << endl;
    else if (rgn->is_hb())
      cdbg << "hb-" << rgn->id() << endl;
    else if (rgn->is_bb())
      cdbg << "bb-" << rgn->id() << endl;
    else
      assert (0);
  }

  if (dbg (opti, 1)) cdbg << "*\tInvoking Blocked-data substi solver:" << endl;
  Bds_solver lfold_solver_object (rgn);

  if (dbg (opti, 1)) cdbg << "*\tBuilding Bds chains:" << endl;
  lfold_solver_object.build_bds_chains ();
  if (dbg (opti, 1)) cdbg << "*\tDone detecting Bds chains:" << endl;

  if (dbg (opti, 1)) cdbg << "*\tBds chains: doing data substi:" << endl;
  lfold_solver_object.data_substi_bds_chains ();
  if (dbg (opti, 1)) cdbg << "*\tBds chains: done data substi" << endl;

  if (dbg (opti, 1)) {
    cdbg << "*\t\tTotal bdss=" << lfold_solver_object.total_bdss
	 << endl;
    cdbg << "*\tBlocked-data substi done, ";
    if (rgn->is_procedure())
      cdbg << "procedure=" << (((Procedure*)rgn)->get_name()) << endl;
    else if (rgn->is_hb())
      cdbg << "hb-" << rgn->id() << endl;
    else if (rgn->is_bb())
      cdbg << "bb-" << rgn->id() << endl;
    else
      assert (0);
  }
}
///////////////////////////////////////////////////////////////////////////////
