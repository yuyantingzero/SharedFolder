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




///////////////////////////////////////////////////////////////////////////
//
//      File:           ir_jumptbl.cpp
//      Authors:        Scott A. Mahlke
//      Created:        July 1996
//      Description:    Elcor Text IR reader-writer for class El_jumptbl
//
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "IR_symbol.h"
#include "el_jumptbl.h"
#include "ir_jumptbl.h"
#include "ir_op.h"
#include "list.h"
#include "op.h"


//-------------------------- Writer -------------------------------

IR_outstream &operator<<(IR_outstream &out, const Elcor_region_flag flag);
IR_outstream &operator<<(IR_outstream &out, const Hash_set<Edge*> &edges);

static int tbl_num = 0;

IR_outstream &operator<<(IR_outstream &out, El_jumptbl &tbl)
{
    int n_entries, align, reserve, i, cc, addr;
    Op *op;
    char *name;

    n_entries = tbl.get_n_entries();
    align = tbl.get_align();
    reserve = tbl.get_reserve();
    name = (char *) tbl.get_name();

    // should look like:
    // (Table name n_entries reserve align
    //     (addr cc op_id)
    //     ...
    // )
    out << tab << L_PAREN << JUMPTBL_STRING << SPACE << tbl_num << SPACE;
    out << JUMPTBL_NAME_STRING << L_PAREN << name << R_PAREN << SPACE;
    out << JUMPTBL_ENTRIES_STRING << L_PAREN << n_entries << R_PAREN << SPACE;
    out << JUMPTBL_RESERVE_STRING << L_PAREN << reserve << R_PAREN << SPACE;
    out << JUMPTBL_ALIGN_STRING << L_PAREN << align << R_PAREN;
    out << indent << indent << indent << endline;

    for (i=0; i<n_entries; i++) {
	addr = i*align;
	cc = tbl.get_cc_val(i);
	op = tbl.get_op_target(i);
	if (cc == EL_JUMPTBL_DEFAULT_CC)
	    out << tab << L_PAREN << addr << SPACE << JUMPTBL_DFLT_CC_STRING
		<< SPACE << op->id() << R_PAREN << endline;
	else
	    out << tab << L_PAREN << addr << SPACE << cc
		<< SPACE << op->id() << R_PAREN  << endline;
    }
    out << outdent << outdent << outdent << tab << R_PAREN << endline;

    return (out);    
}

void print_jump_tables(IR_outstream &out, Procedure *f)
{
    El_jumptbl_info *tbl_info;
    List<El_jumptbl *> tbl_list;
    El_jumptbl *tbl;

    tbl_info = f->get_jumptbl_info();

    if (tbl_info->get_num_tables() <= 0)
	El_punt("print_jump_tables: no tables to print out, shouldn't be called!");

    tbl_num = 1;
    tbl_list = tbl_info->get_tbl_list();
    for (List_iterator<El_jumptbl *> li(tbl_list); li != 0; li++) {
	tbl = *li;
	out << *tbl;
	tbl_num++;
    }
}

//--------------------------- Reader ------------------------------

El_jumptbl *el_jumptbl(IR_instream &in)
{
    IR_token token;
    char *name;
    eString str_name;
    int n_entries, reserve, align, addr, cc, op_id, i;
    Vector<Op *> tbl_ops;
    Vector<int> tbl_ccs;
    El_jumptbl *tbl;

    in >> token;

    if (token == L_PAREN) {
	in >> JUMPTBL_STRING >> token;	// throw away tbl number, not used
        // Get the name of the table 
	in >> JUMPTBL_NAME_STRING >> L_PAREN >> name >>  R_PAREN;
	str_name = name;
        // Get the number of entries
	in >> JUMPTBL_ENTRIES_STRING >> L_PAREN >> n_entries >>  R_PAREN;
        // Get the reserve
	in >> JUMPTBL_RESERVE_STRING >> L_PAREN >> reserve >>  R_PAREN;
        // Get the align
	in >> JUMPTBL_ALIGN_STRING >> L_PAREN >> align >>  R_PAREN;

	tbl_ops.resize(n_entries);
	tbl_ccs.resize(n_entries);
	for (i=0; i<n_entries; i++) {
	    in >> L_PAREN >> addr;
            in >> token;
	    if (token == JUMPTBL_DFLT_CC_STRING)
		cc = EL_JUMPTBL_DEFAULT_CC;
	    else
		cc = token;
	    in >> op_id >> R_PAREN;
	    tbl_ops[i] = (Op *) op_table[op_id];
	    tbl_ccs[i] = cc;
	}

	in >> R_PAREN;	// Last matching right paren

	/*** Now, create the jump table ***/
	tbl = new El_jumptbl(str_name, reserve, align);
	tbl->init_op_targets(tbl_ops);
	tbl->init_cc_vals(tbl_ccs);
	tbl->construct_cbid_targets_from_op_targets();

	return (tbl);
    }
    else {
	in.unget_token();
	return (NULL);
    }
}

void read_all_jump_tables(IR_instream &in, Procedure *f)
{
    El_jumptbl_info *tbl_info;
    El_jumptbl *tbl;

    tbl_info = new El_jumptbl_info();
    tbl_info->set_proc_name(f);
    f->set_jumptbl_info(tbl_info);

    while ( (tbl=el_jumptbl(in)) != NULL) {
	tbl_info->add_tbl(tbl);
    }

    tbl_info->init_all_jump_op(f);
}
