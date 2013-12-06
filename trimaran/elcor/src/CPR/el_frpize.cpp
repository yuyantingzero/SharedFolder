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
 *      File:           el_frpize.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        February 1997
 *      Description:    FRPize superblocks
 *
 *===========================================================================*/
#include "dbg.h"
#include "region.h"
#include "iterators.h"
#include "el_cpr_init.h"
#include "el_control_init.h"
#include "iterators.h"
#include "hash_set.h"
#include "el_control.h"
#include "el_tail_duplication.h"
#include "el_if_converter.h"
#include "attributes.h"
#include "el_frpize.h"

/*
 *	Currently only SBs are frpizable
 */
bool El_is_frpizable(Hyperblock *hb)
{
    Op *op;
    Operand pred;

    /* See if there are any ops guarded by a predicate */
    /* Also, no table jumps allowed */
    for (Region_ops_C0_order op_i(hb); op_i!=0; op_i++) {
	op = *op_i;
	if (op->predicated()) {
	    pred = op->src(PRED1);
	    if (! pred.is_predicate_true())
	        return (false);
	}
	if (op->flag(EL_OPER_TABLE_JUMP))
	    return (false);
    }

    return (true);
}

void El_find_bbs_in_hb(Hyperblock *hb, Basicblock **entry_bb, List_set<Basicblock*>& hb_bbs)
{
    Region *bb, *e_bb;
    Op *e_op;
    int count;

    // Find entry_bb
    count = 0;
    for (Region_entry_ops e_iter(hb); e_iter!=0; e_iter++) {
	e_op = *e_iter;
	e_bb = ((Region *)e_op)->parent();
	if (! e_bb->is_bb())
	    El_punt("El_find_bbs_in_hb: no BB tiling!!");
	*entry_bb = (Basicblock *)e_bb;
	count++;
    }
    if (count!=1)
	El_punt("El_find_bbs_in_hb: %d entry ops found in Hyperblock %d",
			count, hb->id());

    // Fill in hb_bbs
    for (Region_subregions r_iter(hb); r_iter!=0; r_iter++) {
	bb = *r_iter;
	if (! bb->is_bb())
	    El_punt("El_find_bbs_in_hb: Basicblock tiling not present");
	hb_bbs += ((Basicblock *) bb);
    }
}

/*
 *	SB identified by the entry_bb and the set of BBs (hb_bbs)
 */
void El_fprize_this_sb(Procedure *f, Basicblock *entry_bb, List_set<Basicblock*>& hb_bbs)
{
    Hash_set<Edge*> top_entry_edges(hash_edge_ptr);
    Edge *edge;
    Compound_region *cr;

    if (dbg(cpr, 2)) {
	cdbg << "\tFRPizing Hyperblock with entry " << entry_bb->id() << endl;
	cdbg << "\tBBS: ";
	for (List_set_iterator<Basicblock*> hs_iter(hb_bbs); hs_iter!=0; hs_iter++) {
	    cdbg << (*hs_iter)->id() << " ";
	}
	cdbg << endl;
    }
    for(Region_entry_edges e_iter(entry_bb);e_iter!=0;e_iter++) {
	edge = *e_iter;
	top_entry_edges += edge;
    }

    // Create a temporary cr for if-conversion
    cr=el_mark_cr_on_cfg(f, hb_bbs, top_entry_edges);

    // If convert
    El_if_convert(cr, El_frp_model);

    // Nuke the temporary cr, its no longer needed
    El_remove_region(cr, true);
}

/*
 *	FRPize a superblock using the if-converter
 */
void El_frpize_sbs(Procedure *f)
{
    Basicblock *entry_bb;
    Hyperblock *hb;
    List_set<Basicblock*> hb_bbs;
    Hash_set<Hyperblock*> all_hbs;
    bool push_flag, applied;
    Dlist<Region*> rstack;
    Region *rtmp;
    Compound_region *new_hb;

    if (dbg(cpr, 2))
        cdbg << "Enter frpize sbs" << endl;

    // First find all the HBs in f and add them to set all_hbs
    rstack.push(f);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
	    all_hbs += ((Hyperblock *) rtmp);
            push_flag = false;  // Only ops below, so don't waste time!
        }
        else if (rtmp->is_bb()) {
            push_flag = false;  // Only ops below, so don't waste time!
        }
        if (push_flag==true) {
            for (Region_subregions subri(rtmp); subri!=0; subri++) {
                if ((*subri)->is_compound())
                    rstack.push(*subri);
            }
        }
    }

    // Now walk thru hash_set and frpize it
    applied = false;
    for (Hash_set_iterator<Hyperblock*> hseti(all_hbs); hseti!=0; hseti++) {
	hb = *hseti;
	if (dbg(cpr, 2))
	    cdbg << "Processing HB " << hb->id() << endl;
	if (! El_is_frpizable(hb)) {
	    if (dbg(cpr, 2))
		cdbg << "\tHB " << hb->id() << " not frpizable" << endl;
	    continue;
	}
	hb_bbs.clear();
	El_find_bbs_in_hb(hb, &entry_bb, hb_bbs);
	El_remove_region(hb, false);
	El_fprize_this_sb(f, entry_bb, hb_bbs);
	new_hb = entry_bb->parent();
	if (! new_hb->is_hb())
	   El_punt("El_frpize_sbs: HB not correctly inserted");
	// Copy over id/flags/lcode_attrs from hb to new_hb
	new_hb->set_id(hb->id());
	delete new_hb->attributes;
	new_hb->attributes = new Graph_attribute(*(hb->attributes)) ;
	((Region *)new_hb)->copy_flags(hb);
	// 1 of the flags will be the SB flag, so reset that one
	new_hb->reset_flag(EL_REGION_SUPERBLOCK);
	new_hb->set_flag(EL_REGION_HYPERBLOCK_FRP);
	applied = true;
	delete hb;
    }

    if (applied == true)
	f->set_flag(EL_PROC_HYPERBLOCK);
}
