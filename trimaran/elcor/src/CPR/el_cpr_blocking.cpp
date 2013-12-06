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
 *      File:           el_cpr_blocking.cpp
 *      Author:         Scott A. Mahlke, Mike Schlansker
 *      Created:        April 1997
 *      Description:    Identifying the set of brs to perform cpr across
 *
 *===========================================================================*/

#include "defs.h"
#include "dbg.h"
#include "vector.h"
#include "region.h"
#include "iterators.h"
#include "el_cpr_init.h"
#include "hash_set.h"
#include "attributes.h"
#include "dlist.h"
#include "tuples.h"
#include "el_cpr_blocking.h"
#include "el_cpr_init.h"
#include "cont_cpr_match.h"
#include "print_cont_cpr_list.h"
#include "opcode_properties.h"
#include "edge_utilities.h"

#undef DEBUG

#define EL_CPR_MIN_BLOCK_SIZE	El_cpr_min_block_length


/*==============================================================================*/
/*
 *	External routines
 */
/*==============================================================================*/

/*
 *	For now we just prune away the tuples that contain 1 pair, applying
 *	control cpr to 1 branch doesn't make much sense.
 *	Eventually may prune away more stuff.
 */
void El_prune_cpr_blocks(Hyperblock* hb, Cont_cpr_match_result &match_result)
{

    Dlist<Dlist< Pair<Op*, Op*> > > &hb_cpr_groups = match_result.cpr_blk_list;
    Dlist<bool> &hb_cpr_types = match_result.cpr_blk_type;
    Dlist<El_Cpr_Match_Info> & hb_cpr_infos = match_result.cpr_blk_info;
    Dlist<bool> new_hb_cpr_types;
    Dlist<El_Cpr_Match_Info> new_hb_cpr_infos;
    Dlist< Pair<Op*, Op*> > cur_group;
    Pair<Op* , Op*> cur_tuple;
    Dlist<Dlist< Pair<Op*, Op*> > > to_be_deleted;
    int num_tuples;
    Op *cmpp_op, *branch_op;
    Vector<bool> keep_flags;
    bool cur_val;
    El_Cpr_Match_Info cur_info;
    int index;

#ifdef DEBUG
        cout << "**Enter El_prune_cpr_blocks for HB " << hb->id() << endl;
#endif

    keep_flags.resize(hb_cpr_groups.size());

    index = 0;
    for (Dlist_iterator<Dlist< Pair<Op*, Op*> > > d_iter(hb_cpr_groups);
	 d_iter!=0; d_iter++) {
	cur_group = *d_iter;
	num_tuples = 0;
#ifdef DEBUG
        cout << "Considering tuple for deletion: " << endl;
#endif
	for (Dlist_iterator< Pair<Op*, Op*> > g_iter(cur_group);
	     g_iter!=0; g_iter++) {
	    cur_tuple = *g_iter;
	    cmpp_op = cur_tuple.second;
            branch_op = cur_tuple.first;
#ifdef DEBUG
            cout << "    " << "compare: " << cmpp_op->id() << "  branch: "
		 << branch_op->id() << endl;
#endif

	    num_tuples++;
	}
	if (num_tuples < EL_CPR_MIN_BLOCK_SIZE) {
#ifdef DEBUG
            cout << "Tuple will be nuked" << endl;
#endif
	    to_be_deleted.push(cur_group);
	    keep_flags[index] = false;
	}
	else {
#ifdef DEBUG
            cout << "Tuple will be kept" << endl;
#endif
	    keep_flags[index] = true;
	}
	index++;
    }

    /* Do the actual deletion from hb_cpr_groups */
    for (Dlist_iterator<Dlist< Pair<Op*, Op*> > > d2_iter(to_be_deleted);
		d2_iter!=0; d2_iter++) {
	cur_group = *d2_iter;
	hb_cpr_groups.remove(cur_group);
    }

    /* Do the actual deletion from hb_cpr_types, this is done by constructing
	a new list and replacing the old with the new */
    index = 0;
    for (Dlist_iterator<bool> d3_iter(hb_cpr_types); d3_iter!=0; d3_iter++) {
	cur_val = *d3_iter;
	if (keep_flags[index] == true)
	    new_hb_cpr_types.push_tail(cur_val);
	index++;
    }
    match_result.cpr_blk_type = new_hb_cpr_types;

    index = 0;
    for (Dlist_iterator<El_Cpr_Match_Info> d4_iter(hb_cpr_infos); d4_iter!=0; d4_iter++) {
        cur_info = *d4_iter;
        if (keep_flags[index] == true) 
            new_hb_cpr_infos.push_tail(cur_info);
        index++;
    }
    match_result.cpr_blk_info = new_hb_cpr_infos;
}
