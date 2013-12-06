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
 *      File:           el_bypass_branch.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        April 1997
 *      Description:    Bypass branch insertion
 *
 *===========================================================================*/
#ifndef EL_BYPASS_BRANCH_H
#define EL_BYPASS_BRANCH_H

#include "tuples.h"
#include "dlist.h"
#include "operand.h"

class Hyperblock;
class Op;
class Compound_region;
class Cont_cpr_match_result;

#define EL_CPR_LAST_BR_TAKEN	0x1
#define EL_CPR_HAS_COMP_BLOCK	0x2

class El_cpr_chain {
public:
    El_cpr_chain(Dlist< Pair<Op*, Op*> > &group);
    ~El_cpr_chain();

    void set_flag(unsigned int f);
    void reset_flag(unsigned int f);
    bool is_flag(unsigned int f);

    Op *get_first_cmpp_op();
    Op *get_last_cmpp_op();
    Op *get_first_branch_op();
    Op *get_last_branch_op();
    Op *get_corres_branch_op(Op *cmpp_op);
    Op *get_corres_cmpp_op(Op *branch_op);
    Dlist<Pair<Op*, Op* > > &get_cmpp_branch_op_list();
    bool is_chain_branch_op(Op *x);
    bool is_chain_cmpp_op(Op *x);

    void set_pred_incoming(Operand &val);
    void set_pred_on(Operand &val);
    void set_pred_off(Operand &val);
    Operand &get_pred_incoming();
    Operand &get_pred_on();
    Operand &get_pred_off();
    bool is_cpr_pred(Operand &val);

    void set_comp_block(Hyperblock *hb);
    Hyperblock *get_comp_block();

    void add_cpr_cmpp_op(Op *op);
    void set_cpr_branch_op(Op *op);
    Op *get_cpr_branch_op();

    void print_chain();

private:
    unsigned int flags;
    Dlist<Pair<Op*, Op* > > cmpp_branch_ops;   
    Operand pred_incoming;
    Operand pred_on;
    Operand pred_off;
    Hyperblock *comp_block;
    Dlist<Op*> cpr_cmpp_ops;   
    Op *cpr_branch_op;
};

class El_cpr_chain_list {
public:
    El_cpr_chain_list(Hyperblock *hb);
    ~El_cpr_chain_list();

    void add_chain(El_cpr_chain *chain);
    Dlist<El_cpr_chain*> &get_chain_list();

    int get_num_chains();

    void print_chain_list();

private:
   Dlist<El_cpr_chain*> chains;
   int num_chains;
   Hyperblock *block;
};



/*
 *	Utility routines
 */
extern bool El_branch_is_likely_taken(Op *branch_op);
extern void El_create_on_off_trace_predicates(El_cpr_chain *chain, Operand &on_pred,
						Operand &off_pred);


/*
 *	Conversion to normalized form
 */
extern void El_normalize_cmpp_chain(El_cpr_chain *chain);


/*
 *	CPR transform utilities
 */
extern void El_insert_initializer_for_on_trace_pred(Hyperblock *hb, Operand &on_pred,
                                        Operand &incoming_pred, Op *first_cmpp);
extern void El_insert_initializer_for_off_trace_pred(Hyperblock *hb, Operand &off_pred);
extern void El_insert_cpr_cmpp_op(El_cpr_chain *chain, Hyperblock *hb, Op *cmpp_op,
			Op *branch_op, Operand &on_pred, Operand &off_pred,
			Operand &incoming_pred, bool invert);
extern Hyperblock *El_insert_compensation_hb(Compound_region *cr);
extern void El_insert_off_trace_branch(El_cpr_chain *chain, Hyperblock *hb, Op *branch_op,
			Operand &off_pred, Compound_region *target_cr);
extern void El_reassociate_ops_with_new_on_trace_pred(Hyperblock *hb, Op *branch_op,
                        Operand &on_pred, Operand &outgoing_pred);
extern void El_reassociate_branch_with_new_on_trace_pred(Hyperblock *hb, Op *branch_op,
                                        Operand &on_pred, Operand &incoming_pred);
extern void El_compute_derived_preds(Hyperblock *hb, El_cpr_chain *last_chain,
		El_cpr_chain *chain, Hash_set<Operand> &derived_on_preds,
                Hash_set<Operand> &derived_off_preds);
extern void El_record_cpr_info_in_attr(Hyperblock *hb, El_cpr_chain_list *hb_chain_list);


/*
 *	Interface routines
 */
extern void El_insert_bypass_branches(Hyperblock *hb, Cont_cpr_match_result &match_result,
		El_cpr_chain_list *hb_chain_list);

#endif
