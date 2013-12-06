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



#include "codegen.h"

#if 0
static void prune_prf_set_based_on_window(El_ref ref, Hash_set<eString>* prf_set, eString base_prf,
                                          Hash_map<Op*, int>& window_map)
{
    assert(ref.get_operand().is_reg() || ref.get_operand().is_macro_reg());

    if (ref.get_operand().file_type() != GPR)
        return;

    if (!Codegen::base_prf_match(*prf_set, base_prf)) 
      return;

    Op *op = ref.get_op();

    if (op->flag(EL_OPER_IWMOVE)) return;

    assert(window_map.is_bound(op));
    int win = window_map.value(op);

    if (base_prf == "GPR") {
        char *buff = new char[16];
        memset(buff, 0, 16);
        sprintf(buff, "%s_%d", "GPR", win);      
        if (prf_set->is_member(buff)) {
          prf_set->clear();
          *(prf_set) += buff;
        }
        delete [] buff;
    } else if (base_prf == "AR") {
        char *buff = new char[16];
        memset(buff, 0, 16);
        sprintf(buff, "%s_%d", "AR", win);      
        if (prf_set->is_member(buff)) {
          prf_set->clear();
          *(prf_set) += buff;
        }
        delete [] buff;
    } else
        assert(0);
}
#endif


// Given a set of references, find the best prf to bind them
// called in bind_physical_files do determine the preferred prf
// when the particular vrnum/macro is not in the home set.
Pair<eString, int> Codegen::get_best_ref_prf(Hash_set<El_ref>& ref_set) 
{
    Pair<eString, int> home_prf;
    
    // Find the total # of occurances of each prf among all the prfs that are annotated
    // at all refs in ref_set

    Hash_map<eString, int> prf_count(hash_estring, 32);
    for (Hash_set_iterator<El_ref> h1(ref_set); h1!=0; h1++) {
        El_ref ref = *h1;
        Physfile_binding_attr *prf_battr = get_prf_binding_attr(ref.get_op());
        Hash_set<eString> *prf_set = NULL;

        if ((ref.get_ref_type() == PRED_SRC) || (ref.get_ref_type() == EXP_SRC)) {
            prf_set = &(prf_battr->src(ref.get_port_num()));
        } else if (ref.get_ref_type() == EXP_DEST) {
            prf_set = &(prf_battr->dest(ref.get_port_num()));
        }
#if 0
        // Hack for now, if there is only a single choice for a reference,
	// increase its preference by 10
        int inc = 0;
        if (prf_set->size() == 1)
          inc = 10;
        else
          inc = 1;
#endif
        int inc = 1;
        for (Hash_set_iterator<eString> hiter(*prf_set); hiter!=0; hiter++) {
            eString prf = *hiter;
            if (!prf_count.is_bound(prf))
                prf_count.bind(prf, inc);
            else
                (prf_count.value(prf)) += inc;
        }
    }
    if (dbg(genrio,5)) {
        cdbg << "In get best ref prf for vr/macro: " ;
        if (ref_set.head().get_operand().is_reg())
            cdbg << ref_set.head().get_operand().vr_num() << endl;
        else
            cdbg << ref_set.head().get_operand().name() << endl;
        for (Hash_map_iterator<eString, int> h2(prf_count); h2!=0; h2++)
            cdbg << "prf: " << (*h2).first << " count: " << (*h2).second << endl;
    }

    // find the prf with the max cost,and return that prf
    int max = 0;
    for (Hash_map_iterator<eString, int> h3(prf_count); h3!=0; h3++) {
        if ((*h3).second > max) {
            max = (*h3).second;
//          home_prf.first = (*h3).first;
        }
    }

    List<eString> tied_prf_list;
    for (Hash_map_iterator<eString, int> h4(prf_count); h4!=0; h4++) {
        if ((*h4).second == max)
            tied_prf_list.add_tail((*h4).first);
    }

    if (dbg(genrio,7)) {
        if (ref_set.head().get_operand().is_reg())
            cdbg << "Tied prf for vr: " << ref_set.head().get_operand().vr_num();
        else
            cdbg << "Tied prf for macro: " << ref_set.head().get_operand().name();
        for (List_iterator<eString> liter1(tied_prf_list); liter1!=0; liter1++)
            cdbg << " " << *liter1;
        cdbg << endl;
    }

    int file = (rand() % tied_prf_list.size()) + 1;
    List_iterator<eString> liter2(tied_prf_list);
    for (int i=1;i<file;i++,liter2++)
      ;
    home_prf.first = (*liter2);

    home_prf.second = max;
    if (dbg(genrio,7))
        cdbg << "returning random file: " << home_prf.first << " with count: " << max << endl;

    return home_prf;
}
