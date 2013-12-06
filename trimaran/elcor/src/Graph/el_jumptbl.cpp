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
//      File:           el_jumptbl.cpp
//      Authors:        Scott A. Mahlke
//      Created:        June 1996
//      Description:    Data structures jump tables (ie case stmts)
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "attributes.h"
#include "el_jumptbl.h"
#include "op.h"
#include "region_utilities.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "edge.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "hash_set.h"

/*==========================================================================*/
/*
 *	Member functions for El_jumptbl
 */
/*==========================================================================*/

// Default constructor
El_jumptbl::El_jumptbl()
   : name_rep(0), reserve_rep(0), align_rep(0), n_entries_rep(0)
{
}

// Preferred constructor
El_jumptbl::El_jumptbl(eString name, int reserve, int align)
{
    if ((reserve % align) !=0)
	El_punt("El_jumptbl::El_jumptbl - corrupt jump table params");

    name_rep = name;
    id_rep = El_extract_jump_table_id(name);

    reserve_rep = reserve;
    align_rep = align;
    n_entries_rep = reserve/align;

    cc_vals.resize(n_entries_rep);
    cbid_targets.resize(n_entries_rep);
    op_targets.resize(n_entries_rep);
}

El_jumptbl::~El_jumptbl()
{
}

eString& El_jumptbl::get_name()
{
    return (name_rep);
}

void El_jumptbl::set_name(eString& name)
{
    name_rep = name;
}

int El_jumptbl::get_id()
{
    return (id_rep);
}

Op* El_jumptbl::get_jump_op()
{
    return (jump_op_rep);
}

void El_jumptbl::set_jump_op(Op *op)
{
    jump_op_rep = op;
}

int El_jumptbl::get_reserve()
{
    return (reserve_rep);
}

int El_jumptbl::get_align()
{
    return (align_rep);
}

int El_jumptbl::get_n_entries()
{
    return (n_entries_rep);
}

int El_jumptbl::get_cbid_target(int i)
{
    return (cbid_targets[i]);
}

Op *El_jumptbl::get_op_target(int i)
{
    return (op_targets[i]);
}

int El_jumptbl::get_cc_val(int i)
{
    return (cc_vals[i]);
}

int El_jumptbl::get_cc_index(int val)
{
    int i;

    for (i=0; i<n_entries_rep; i++) {
	if (cc_vals[i] == val)
	    return (i);
    }
    return (-1);
}

void El_jumptbl::init_cbid_targets(Vector<int>& table_data)
{
    int i;

    for (i=0; i<n_entries_rep; i++) {
	cbid_targets[i] = table_data[i];
    }
}

void El_jumptbl::init_op_targets(Vector<Op *>& table_data)
{
    int i;

    for (i=0; i<n_entries_rep; i++) {
        op_targets[i] = table_data[i];
    }
}

void El_jumptbl::init_cc_vals(Vector<int>& table_data)
{
    int i, min_cc;

    min_cc = EL_JUMPTBL_DEFAULT_CC;

    for (i=0; i<n_entries_rep; i++) {
        cc_vals[i] = table_data[i];
	if (table_data[i] < min_cc)
	    min_cc = table_data[i];
    }

    base_cc_rep = min_cc;
}

//
//	Assumes El_setup_hb_bb_id_map has been called!!
//
void El_jumptbl::construct_op_targets_from_cbid_targets()
{
    int i, cb_id;
    Region *r;
    Op *op;

    if (El_hb_bb_id_map.is_empty())
	El_punt("El_jumptbl::construct_op_targets_from_cbid_targets called \
		 without initializing El_setup_hb_bb_id_map");

    for (i=0; i<n_entries_rep; i++) {
	cb_id = cbid_targets[i];
	r = El_hb_bb_id_map.value(cb_id);
	op = get_first_region_op_from_inops(r);
	op_targets[i] = op;
    }
} 

//
//	update cb id values based on current settings of op_targets
//
void El_jumptbl::construct_cbid_targets_from_op_targets()
{
    int i;
    bool found;
    Region *reg;

    for (i=0; i<n_entries_rep; i++) {
	reg = op_targets[i];
	found = false;
	while (reg->parent()) {
	    reg = reg->parent();
	    if (reg->is_bb() || reg->is_hb()) {
		found = true;
		break;
	    }
	}
	assert(found);

	// Now look for a larger hb surrounding the reg which is what cb we want
	if (reg->is_bb() && reg->parent()->is_hb())
	    reg = reg->parent();

	cbid_targets[i] = reg->id();
    }
}

//
//	Use jump_op edges to init cc vals
//
void El_jumptbl::construct_cc_vals_from_op()
{
    Edge *edge;
    Control_flow_freq *cfreq;
    int cc, index, min_cc;
    Region_exit_edges exit_i;


    // Find the smallest cc, this corresponds to the CC for the first
    // entry in the jump table
    min_cc = EL_JUMPTBL_DEFAULT_CC;
    for (exit_i(jump_op_rep); exit_i!=0; exit_i++) {
	edge = *exit_i;
	cfreq = get_control_flow_freq(edge);
	cc = cfreq->cc;
	if (cc < min_cc)
	    min_cc = cc;
    }
    base_cc_rep = min_cc;

    // Initialize the CC's for each entry in the jump table
    cc_vals.assign(cc_vals.size(), EL_JUMPTBL_DEFAULT_CC);
    for (exit_i(jump_op_rep); exit_i!=0; exit_i++) {
	edge = *exit_i;
	cfreq = get_control_flow_freq(edge);
	cc = cfreq->cc;
	if (cc != EL_JUMPTBL_DEFAULT_CC) {
	    index = cc - base_cc_rep;
	    cc_vals[index] = cc;
	}
    }
}


/*==========================================================================*/
/*
 *	Member functions for El_jumptbl_info
 */
/*==========================================================================*/

El_jumptbl_info::El_jumptbl_info()
{
    num_tables_rep = 0;
    num_jump_op_rep = 0;
    max_id_rep = -1;
}

El_jumptbl_info::~El_jumptbl_info()
{
    for (List_iterator<El_jumptbl* > li(tbl_list); li!=0; li++) {
	delete *li;
    }
}

void El_jumptbl_info::set_proc_name(Procedure *f)
{
    proc_name_rep = f->get_name();
}

eString& El_jumptbl_info::get_proc_name()
{
    return (proc_name_rep);
}

void El_jumptbl_info::set_num_tables(int count)
{
    num_tables_rep = count;
}

void El_jumptbl_info::increment_num_tables(int inc_val)
{
    num_tables_rep += inc_val;
}

int El_jumptbl_info::get_num_tables()
{
    return (num_tables_rep);
}

void El_jumptbl_info::set_num_jump_op(int count)
{
    num_jump_op_rep = count;
}

int El_jumptbl_info::get_num_jump_op()
{
    return (num_jump_op_rep);
}

void El_jumptbl_info::set_max_id(int count)
{
    max_id_rep = count;
}

void El_jumptbl_info::update_max_id(int new_id)
{
    if (new_id > max_id_rep)
	max_id_rep = new_id;
}

int El_jumptbl_info::get_max_id()
{
    return (max_id_rep);
}

List<El_jumptbl *>& El_jumptbl_info::get_tbl_list()
{
    return (tbl_list);
}

void El_jumptbl_info::add_tbl(El_jumptbl *tbl)
{
    int tbl_id;

    num_tables_rep++;

    tbl_id = tbl->get_id();
    if (tbl_id > max_id_rep)
	max_id_rep = tbl_id;

    tbl_list.add_tail(tbl);
}

El_jumptbl* El_jumptbl_info::find_jump_table(eString name)
{
    El_jumptbl *tbl;

    for (List_iterator<El_jumptbl *> li(tbl_list); li!=0; li++) {
	tbl = *li;
	if (tbl->get_name()==name)
	    return (tbl);
    }

    return (0);
}

El_jumptbl* El_jumptbl_info::find_jump_table(Op *jump_op)
{
    El_jumptbl *tbl;

    for (List_iterator<El_jumptbl *> li(tbl_list); li!=0; li++) {
	tbl = *li;
	if (tbl->get_jump_op()==jump_op)
	    return (tbl);
    }

    return (0);
}

void El_jumptbl_info::init_all_jump_op(Procedure *f)
{
    Op *op;
    Hash_set<int> tbl_set(hash_int, 7);
    eString *tbl_name;
    int tbl_id;
    El_jumptbl *tbl;
    int num_jump_op;

    num_jump_op = 0;
    for (Region_all_ops r_iter(f); r_iter!=0; r_iter++) {
	op = *r_iter;
	if (! is_table_jump(op))
	    continue;
	num_jump_op++;
	tbl_name = El_jump_table_name(op);
	tbl_id = El_extract_jump_table_id(*tbl_name);
	tbl = this->find_jump_table(*tbl_name);
	if (tbl_set.is_member(tbl_id))
	    El_warn("El_jumptbl_info::find_jump_ops: multiple jrgs share tbl %s",
			(char *) *tbl_name);
	if (tbl == NULL)
	    El_punt("El_jumptbl_info::find_jump_ops: no tbl with name %s found",
			(char *) *tbl_name);

	tbl_set += tbl_id;
	tbl->set_jump_op(op);
    }

    num_jump_op_rep = num_jump_op;
}

void El_jumptbl_info::construct_all_cc_vals_from_op()
{
    El_jumptbl *tbl;

    for (List_iterator<El_jumptbl *> li(tbl_list); li!=0; li++) {
	tbl = *li;
	tbl->construct_cc_vals_from_op();
    }
}

/*==========================================================================*/
/*
 *	Utility functions
 */
/*==========================================================================*/

/*
 *	Name format is _$%s%s%d,
 *		where the first %s is the procedure name
 *		the second %s is EL_JUMPTBL_BASE_NAME
 *		%d is the table id
 */
int El_extract_jump_table_id(eString name)
{
    char *ptr, *ptr2;
    int len, val;

    len = strlen(EL_JUMPTBL_BASE_NAME);

    ptr = (char *)name;
    ptr2 = strstr(ptr, EL_JUMPTBL_BASE_NAME);
    if (!ptr2)
        El_punt("El_extract_jump_table_id: illegal string %s", (char *) name);
    ptr = ptr2 ;
    while (ptr2) {
        ptr2 = strstr((ptr+1), EL_JUMPTBL_BASE_NAME);
        if (ptr2)
            ptr = ptr2 ;
    }

    ptr += len;
    if (ptr[0] == '\0')
        El_punt("El_extract_jump_table_id: illegal string %s", (char *) name);

    val = ::atoi(ptr);

    return (val);
}

bool El_valid_jump_table_name(eString name, eString proc_name)
{
    int tbl_id, len;
    char *ptr, *proc_name_ptr;

    ptr = (char *) name;

    /* name should start with EL_JUMPTBL_NAME_PREFIX */
    len = strlen(EL_JUMPTBL_NAME_PREFIX);
    if (strncmp(ptr, EL_JUMPTBL_NAME_PREFIX, len))
	return (false);
    ptr += len;

    /* Next should be proc_name */
    proc_name_ptr = (char *) proc_name;
    len = strlen(proc_name_ptr);
    if (strncmp(ptr, proc_name_ptr, len))
	return (false);
    ptr += len;

    /* Next should be EL_JUMPTBL_BASE_NAME */
    len = strlen(EL_JUMPTBL_BASE_NAME);
    if (strncmp(ptr, EL_JUMPTBL_BASE_NAME, len))
	return (false);

    /* tbl_id should be non-negative and not larger than EL_JUMPTBL_MAX_ID */
    tbl_id = El_extract_jump_table_id(name);

    if (tbl_id < 0)
	return (false);
    else if (tbl_id > EL_JUMPTBL_MAX_ID)
	return (false);

    return (true);
}

eString& El_create_jump_table_name(int id, eString proc_name)
{
    char tmp[128];
    eString *new_str;

    sprintf(tmp, "%d", id);

    new_str = new eString(EL_JUMPTBL_NAME_PREFIX);
    new_str->cat(proc_name);
    new_str->cat(EL_JUMPTBL_BASE_NAME);
    new_str->cat(tmp);

    return (*new_str);
}

int El_num_jump_tables(Procedure *f)
{
    El_jumptbl_info *tbl_info;

    tbl_info = f->get_jumptbl_info();

    if (tbl_info == NULL)
	return (0);

    else
	return (tbl_info->get_num_tables());
}

bool El_proc_has_jump_tables(Procedure *f)
{
    if (El_num_jump_tables(f) > 0)
	return (true);
    else
	return (false);
}

int El_new_jump_table_id(Procedure *f)
{
    El_jumptbl_info *tbl_info;

    tbl_info = f->get_jumptbl_info();

    if (tbl_info == NULL)
	El_punt("El_new_jump_table_id: Proc has no jump table field created");

    return (tbl_info->get_max_id() + 1);
}

eString *El_jump_table_name(Op *op)
{
    Jump_table_name *tbl_attr;

    tbl_attr = get_jump_table_name(op);

    if (tbl_attr == NULL)
	return (NULL);

    else
	return (&(tbl_attr->tbl_name));
}

bool is_table_jump(Op *op)
{
    if (op==NULL)
	return (false);

    if ((is_bru(op)) && op->flag(EL_OPER_TABLE_JUMP))
	return (true);
    else
	return (false);
}

/*==========================================================================*/
/*
 *	Jump table name attribute attaching functions
 */
/*==========================================================================*/

Operand *El_extract_jump_table_name_operand(Op *op, eString proc_name)
{
    Operand *src, *tbl_name;

    if (op == NULL)
	return (NULL);

    tbl_name = NULL;
    for (Op_explicit_sources src_i(op); src_i!=0; src_i++) {
	src = &(*src_i);
	if (src->is_label() &&
              El_valid_jump_table_name(src->label_value(), proc_name)) {
	    tbl_name = src;
	    break;
	}
    }

    return (tbl_name);
}

Op *El_find_local_prev_def(Operand *operand, Op *op)
{
    Compound_region *reg, *parent;
    Op *cur_op;
    Operand *dest;

    reg = op->parent();
    parent = reg->parent();
    if (parent && parent->is_hb())
	reg = parent;

    for (Region_ops_reverse_C0_order op_iter(reg, op); op_iter != 0; op_iter++) {
	cur_op = *op_iter;
	for (Op_explicit_dests dest_i(cur_op); dest_i!=0; dest_i++) {
	    dest = &(*dest_i);
	    if (*dest == *operand)
		return (cur_op);
	}
    }

    return (NULL);
}

//  SAM: this routine has to be fixed!! to be more general and use Sumedh's
//  reaching defs analysis!!
Hash_set<Op *> *El_find_jump_table_address_ops(Procedure *f, Op *jrg_op,
                                               int *df_done)
{
    Operand *src, *name_operand;
    Op *pbr_op, *ld_op, *addr_op;
    Hash_set<Op *> *address_ops;

    address_ops = new Hash_set<Op *> (hash_op_ptr, 7);

    // Find the pbr
    src = &(jrg_op->src(SRC1));
    pbr_op = El_find_local_prev_def(src, jrg_op);
    if (pbr_op == NULL)		// Def is in another block, not handled yet!!
	El_punt("El_find_jump_table_address_ops: no local def of jrg src1 found");
    if (! is_pbr(pbr_op))
	El_punt("El_find_jump_table_address_ops: unexpected def of jrg src1");
    *address_ops += pbr_op;

    // Find the ld
    src = &(pbr_op->src(SRC1));
    ld_op = El_find_local_prev_def(src, pbr_op);
    if (ld_op == NULL)		// Def is in another block, not handled yet!!
	El_punt("El_find_jump_table_address_ops: no local def of pbr src1 found");
    if (! is_load(ld_op))
	El_punt("El_find_jump_table_address_ops: unexpected def of pbr src1");
    *address_ops += ld_op;

    // Find the addr calc
    src = &(ld_op->src(SRC1));
    addr_op = El_find_local_prev_def(src, ld_op);
    if (addr_op == NULL)	// Def is in another block, not handled yet!!
	El_punt("El_find_jump_table_address_ops: no local def of ld src1 found");
    *address_ops += addr_op;

    name_operand = El_extract_jump_table_name_operand(addr_op, f->get_name());
    if (name_operand == NULL)
	El_punt("El_find_jump_table_address_ops: no jump table name found");

    return (address_ops);
}

/*==========================================================================*/
/*
 *	Jump table construction routines
 */
/*==========================================================================*/

/* Given a jump_rg op, construct a jump table from the edges */
El_jumptbl *El_generate_jump_table_for_op(Op *jrg)
{
    Edge *edge;
    Region_exit_edges exit_i;
    int i, cc, min_cc, max_cc, align, num_bytes, num_entries;
    eString tbl_name;
    Op *default_op;
    Vector<Op *> edge_targets;
    Vector<int> edge_ccs;
    Control_flow_freq *cfreq;
    El_jumptbl *jumptbl;

    if (! is_table_jump(jrg))
	El_punt("El_generate_jump_table_for_op: op %d is not a jrg!", jrg->id());

    /* Find min, max cc and the default target */
    min_cc = EL_JUMPTBL_DEFAULT_CC;
    max_cc = -EL_JUMPTBL_DEFAULT_CC;
    default_op = NULL;
    for (exit_i(jrg); exit_i!=0; exit_i++) {
	edge = *exit_i;
	cfreq = get_control_flow_freq(edge);
	if (cfreq == NULL)
	    El_punt("El_generate_jump_table_for_op: edge has no cfreq attr");

	cc = cfreq->cc;
	if (cc == EL_JUMPTBL_DEFAULT_CC) {
	    default_op = edge->dest();
	    continue;
	}
	if (cc < min_cc)
	    min_cc = cc;
	if (cc > max_cc)
	    max_cc = cc;
    }

    if (default_op == NULL)
	El_punt("El_generate_jump_table_for_op: jrg %d is missing default edge",
		 jrg->id());

    /* Extract the jump table info */
    num_entries = max_cc - min_cc + 1;
    align = EL_JUMPTBL_DEFAULT_ALIGN;
    num_bytes = num_entries * align;
    tbl_name = "hash_0";	// SAM this need to be fixed!!!!!!!!!!!!!

    /* Create a Vector of target op's for each cc val between min/max */
    edge_ccs.resize(num_entries);
    edge_targets.resize(num_entries);
    
    for (i=0; i<num_entries; i++) {
	edge_targets[i] = default_op;
	edge_ccs[i] = EL_JUMPTBL_DEFAULT_CC;
    }
    for (exit_i(jrg); exit_i!=0; exit_i++) {
	edge = *exit_i;
	cfreq = get_control_flow_freq(edge);
	cc = cfreq->cc;
	if (cc == EL_JUMPTBL_DEFAULT_CC)
	    continue;
	edge_targets[cc-min_cc] = edge->dest();
	edge_ccs[cc-min_cc] = cc;
    }

    /* Create the jump table */
    jumptbl = new El_jumptbl(tbl_name, num_bytes, align);
    jumptbl->set_jump_op(jrg);
    jumptbl->init_op_targets(edge_targets);
    jumptbl->init_cc_vals(edge_ccs);
    jumptbl->construct_cbid_targets_from_op_targets();
   
    return (jumptbl); 
}

#if 0
void El_regenerate_all_jump_tables(Procedure *f)
{
    tbl_info = f->get_jumptbl_info();
    if (tbl_info != NULL)
	delete tbl_info;

    tbl_info = new El_jumptbl_info();
}
#endif
