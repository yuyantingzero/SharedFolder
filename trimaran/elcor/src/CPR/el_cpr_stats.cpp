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
 *      File:           el_cpr_stats.cpp
 *      Author:         Scott A. Mahlke
 *      Created:        August 1998
 *      Description:    CPR application statistics
 *
 *===========================================================================*/

#include "defs.h"
#include "dbg.h"
#include "el_io.h"
#include "el_stats_init.h"
#include "hash_set.h"
#include "attributes.h"
#include "dlist.h"
#include "tuples.h"
#include "el_cpr_init.h"
#include "cont_cpr_match.h"
#include "print_cont_cpr_list.h"
#include "edge_utilities.h"

char *El_cpr_match_info_to_string(El_Cpr_Match_Info val)
{
    switch (val) {
	case EL_CPR_MATCH_UNKNOWN:		return ("UKN");
	case EL_CPR_MATCH_END:			return ("END");
	case EL_CPR_MATCH_MIN_BR:		return ("MIN");
	case EL_CPR_MATCH_MAX_BR:		return ("MAX");
	case EL_CPR_MATCH_SUITABILITY:		return ("SUI");
	case EL_CPR_MATCH_SEPARABILITY:		return ("SEP");
	case EL_CPR_MATCH_PRED_TAKEN:		return ("TKN");
	case EL_CPR_MATCH_EXIT_RATIO:		return ("EXR");
	case EL_CPR_MATCH_PAUSEA:		return ("PSA");
	case EL_CPR_MATCH_PAUSEB:		return ("PSB");
	case EL_CPR_MATCH_BREAK:		return ("BRK");
	default:
	    El_punt("El_cpr_match_info_to_char: uknown info value");
	    return ("???");
    }
}

/*============================================================================*/
/*
 *	External functions
 */
/*============================================================================*/

void El_compute_cpr_block_stats(Hyperblock *hb, Cont_cpr_match_result &match_result)
{
    Dlist<Dlist< Pair<Op*, Op*> > > hb_cpr_blks;
    Dlist<bool> cpr_blk_type;
    Dlist<El_Cpr_Match_Info> cpr_blk_info;
    Dlist< Pair<Op*, Op*> > cpr_blk;
    Pair<Op*,Op*> cur_tuple;
    int num_cpr_blk, blk_len, count;
    bool blk_type;
    El_Cpr_Match_Info blk_info;
    Edge *tk_edge, *ft_edge;
    Control_flow_freq *tk_cfreq, *ft_cfreq;
    double on_trace_w, off_trace_w, ft_w, tk_w;
    Op *branch_op;

    hb_cpr_blks = match_result.cpr_blk_list;
    cpr_blk_type = match_result.cpr_blk_type;
    cpr_blk_info = match_result.cpr_blk_info;
    num_cpr_blk = hb_cpr_blks.size();

    if (num_cpr_blk == 0)
	return;

    (*EL_STAT_STREAM) << "    cpr_blks\t"
  	           << "hb "
	           << hb->id() << '\t'
		   << "nblk "
                   << num_cpr_blk << "  "
		   << "blks ";
    for (Dlist_iterator<Dlist< Pair<Op*, Op*> > > d_iter(hb_cpr_blks);
						  d_iter!=0; d_iter++) {
	cpr_blk = *d_iter;
	blk_len = cpr_blk.size();
	blk_type = cpr_blk_type.pop();
	blk_info = cpr_blk_info.pop();

	count = 0;
	off_trace_w = 0.0;
	on_trace_w = 0.0;
	for (Dlist_iterator<Pair<Op*,Op*> >d_iter2(cpr_blk); d_iter2!=0; d_iter2++) {
	    cur_tuple = *d_iter2;
	    count++;
	    branch_op = cur_tuple.first;
	    tk_edge = get_outgoing_CONTROL0_non_fall_through_edge(branch_op);
	    if (tk_edge != NULL) {
		tk_cfreq = get_control_flow_freq(tk_edge);
		tk_w = tk_cfreq->freq;
	    }
	    else {
		tk_w = 0.0;
	    }
	    ft_edge = get_outgoing_CONTROL0_fall_through_edge(branch_op);
	    if (ft_edge != NULL) {
	        ft_cfreq = get_control_flow_freq(ft_edge);
		ft_w = ft_cfreq->freq;
	    }
	    else {
		ft_w = 0.0;
	    }
	    if (count == blk_len) {
		if (blk_type == true) { /* likely taken */
		    on_trace_w = tk_w;
		    off_trace_w += ft_w;
		}
		else {	/* likely fallthru */
		    off_trace_w += tk_w;
		    on_trace_w = ft_w;
		}
	    }
	    else {
		off_trace_w += tk_w;
	    }
	}

	(*EL_STAT_STREAM) << "(" << blk_len << " "
		       << El_cpr_match_info_to_string(blk_info) << " "
		       << on_trace_w << " " << off_trace_w << ")";
    }

    (*EL_STAT_STREAM) << endl;
}
