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
//      File:           el_jumptbl.h
//      Authors:        Scott A. Mahlke
//      Created:        June 1996
//      Description:    Data structures jump tables (ie case stmts)
//
/////////////////////////////////////////////////////////////////////////////
#ifndef EL_JUMPTBL_H
#define EL_JUMPTBL_H

#include "string_class.h"
#include "vector.h"
#include "list.h"

#define EL_JUMPTBL_DEFAULT_CC		2147483647
#define EL_JUMPTBL_MAX_ID		99999		/* sanity check only */
#define EL_JUMPTBL_NAME_PREFIX		"_$"
#define EL_JUMPTBL_BASE_NAME		"_hash_"	/* Naming convention */
#define EL_JUMPTBL_FUNC_ATTR		"jump_tbls"
#define EL_JUMPTBL_OP_ATTR		"tbl_name"
#define EL_JUMPTBL_DEFAULT_ALIGN	4

/* Jump table status flags */
#define EL_JUMPTBL_MODIFIED_CONTROL	0x00000001
#define EL_JUMPTBL_NEW_TABLES		0x00000002

class Procedure;
class Op;


/*==========================================================================*/
/*
 *	Jump table class definitions
 */
/*==========================================================================*/

// change all entries which are label1  to label2
// Change a particular cc from label1 to label2
class El_jumptbl {
    friend class El_jumptbl_cbid_targets;
public:
    El_jumptbl();
    El_jumptbl(eString name, int reserve, int align);
    ~El_jumptbl();

    eString& get_name();
    void set_name(eString& name);
    int get_id();
    Op* get_jump_op();
    void set_jump_op(Op *op);
    int get_reserve();
    int get_align();
    int get_n_entries();

    int get_cbid_target(int i);		// get cbid_targets[i]
    Op* get_op_target(int i);		// get op_targets[i]
    int get_cc_val(int i);		// get cc_vals[i]
    int get_cc_index(int val);		// get i where cc_vals[i] = val

    void init_cbid_targets(Vector<int>& table_data);
    void init_op_targets(Vector<Op *>& table_data);
    void init_cc_vals(Vector<int>& table_data);
    void construct_op_targets_from_cbid_targets();
    void construct_cbid_targets_from_op_targets();
    void construct_cc_vals_from_op();

private:
    Op *jump_op_rep;
    eString name_rep;
    int id_rep;
    int reserve_rep;
    int align_rep;
    int n_entries_rep;
    int base_cc_rep;		// Smallest cc value
    Vector<int> cbid_targets;	// This is whats needed to go to Lcode eventually
    Vector<Op *> op_targets;	// This is whats used internally in Elcor
    Vector<int> cc_vals;	// Condition code values (case vals)
};

class El_jumptbl_info {
public:
    El_jumptbl_info();
    ~El_jumptbl_info();

    void set_proc_name(Procedure *f);
    eString& get_proc_name();

    void set_num_tables(int count);
    void increment_num_tables(int inc_val);
    int get_num_tables();

    void set_num_jump_op(int count);
    int get_num_jump_op();

    void set_max_id(int count);
    void update_max_id(int new_id);
    int get_max_id();

    List<El_jumptbl *>& get_tbl_list();
    void add_tbl(El_jumptbl *tbl);

    El_jumptbl* find_jump_table(eString name);
    El_jumptbl* find_jump_table(Op *jump_op);
    void init_all_jump_op(Procedure *f);
    void construct_all_cc_vals_from_op();

private:
    eString proc_name_rep;
    int num_tables_rep;
    int num_jump_op_rep;
    int max_id_rep;
    List<El_jumptbl *> tbl_list;
};

/*==========================================================================*/
/*
 *	Utility functions
 */
/*==========================================================================*/

extern int El_extract_jump_table_id(eString name);
extern bool El_valid_jump_table_name(eString name, eString proc_name);
extern eString& El_create_jump_table_name(int id, eString proc_name);
extern int El_num_jump_tables(Procedure *f);
extern bool El_proc_has_jump_tables(Procedure *f);
extern int El_new_jump_table_id(Procedure *f);
extern eString *El_jump_table_name(Op *op);
extern bool is_table_jump(Op *op);

/*==========================================================================*/
/*
 *	Jump table construction routines
 */
/*==========================================================================*/

extern El_jumptbl *El_generate_jump_table_for_op(Op *jrg);

#endif
