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
//      File:           el_normalize_branches.h
//      Author:         Matthai Philipose
//      Created:        June 1996
//      Description:    Transform the CFG to get rid of code-layout info
//
/////////////////////////////////////////////////////////////////////////////

#ifndef EL_NORMALIZE_BRANCHES_H
#define EL_NORMALIZE_BRANCHES_H
#include "list_set.h"
#include "defs.h"
#include "reaching_defs_solver.h"
#include "op_attributes.h"

enum BR_REPLACE_TYPE
{
UNCOND_BRU=1,
UNCOND_DUMMY,
COND_BRU, //predicted brus
COND_BRCT,
COND_BRCF,
COND_BRCT_BRU,
COND_UNUSED
};

enum BR_INFO_FIELD_TYPE
{
  BR_INFO_PBR=1,
  BR_INFO_CMPP,
  BR_INFO_LD,
  BR_INFO_CALC,
  BR_INFO_BR,
  BR_INFO_UNUSED  
};
 
void el_normalize_branches(Procedure *reg);
void el_normalize_bblocks(List_set<Basicblock*>& bb_set,Reaching_defs_info* rdi);
int el_normalize_br_opcode(Opcode pred);
int el_normalize_br_op(Op* cond_op_ptr, Op* branch_op_ptr);

void el_denormalize_branches(Procedure *reg);
void el_denormalize_bblocks(List_set<Basicblock*>& bb_set);
void el_denormalize_br_op(Op *op,BR_REPLACE_TYPE br,Basicblock* dest_bb);
IR_BASE_OPCODE el_denormalize_br_opcode(int pred);

Basicblock* el_single_successor_bb(Basicblock* cur_bb);
Basicblock* el_true_successor_bb(Basicblock* cur_bb);
Basicblock* el_false_successor_bb(Basicblock* cur_bb);
Basicblock* el_predicate_false_successor_bb(Basicblock* cur_bb);
Edge* el_predicate_false_successor_edge(Basicblock* cur_bb);

Edge* el_false_exit_edge(Basicblock* cur_bb);
Op *el_bb_exit_br_op(Basicblock* cur_bb);
void el_set_single_exit_alt(Op* bru_op,int val);
void el_set_pred_false_exit_alt(Op* bru_op,Alt_num val);
int el_num_region_successors(Region* reg);

Op* el_insert_pbr(Op* op,Compound_region* dest_bb,int pred_bit);
Op* el_insert_cmpp(Op* op);
Op* el_insert_brct(Op* op,Op *cmpp_0p,Operand cmpp_oper);
Op* el_insert_bru(Op *op,Op *pbr_op);
Port_num el_find_br_pred_port(Op* cmpp,Op* br,Reaching_defs_info* rdi);

void el_set_id_in_br_info(Op *brdir_op,int id,BR_INFO_FIELD_TYPE ftype);
void el_set_op_in_br_info(Op *brdir_op,Op* op,BR_INFO_FIELD_TYPE ftype);
void el_set_attrs_in_br_info(Op *brdir_op,Lcode_attribute_map* attr,BR_INFO_FIELD_TYPE ftype);
void el_set_oper_in_br_info(Op *brdir_op,Operand* operand,BR_INFO_FIELD_TYPE ftype);

int el_get_id_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype);
Op* el_get_op_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype);
Lcode_attribute_map* el_get_attrs_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype);
Operand* el_get_oper_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype);
bool el_op_exists_in_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype);

bool el_op_has_exactly_one_use(Op *pbr_op_ptr,Reaching_defs_info* rdi);
int el_num_uses_of_def(El_ref& def,Reaching_defs_info* rdi);
int el_find_branch_prediction_bit(Op* br,Opcode opc);

void el_flip_exit_alt_numbers(Basicblock* cur_bb);
Basicblock* el_insert_bru_bb_on_false_exit(Op* brct_op,Edge* pred_false_edge);
int tail_dup_hash_bb(Basicblock*& bb);

bool is_last_subregion(Compound_region* region, Region* anc_region);
bool is_first_subregion(Compound_region* region, Region* anc_region);
bool el_compound_region_succeeds_in_dlist(Compound_region* second,Compound_region* first);

Op* el_C0_hb_successor_op(Op *op);
void  el_append_hb_stop_op(Basicblock* cur_bb);
#endif
