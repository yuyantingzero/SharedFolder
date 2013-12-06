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
//      File:           region_utilities.h
//      Authors:        Sadun Anik, Vinod Kathail
//      Created:        December 1994
//      Description:    Utilities for regions
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _REGION_UTILITIES_H
#define _REGION_UTILITIES_H

#include "defs.h"
#include "el_port.h"
#include "hash_set.h"
#include "hash_map.h"
class Region ;
class Op ;
class Edge ;
class Compound_region ;
class Procedure;

Op* get_first_region_op_from_inops(Region* R) ;

Op* get_last_region_op_from_outops(Region* R) ;

Op* get_first_region_op_from_subregions(Region* R) ;

Op* get_last_region_op_from_subregions(Region* R) ;

bool region_contains_edge(Region*, Edge*) ;

bool region_contains_op(Region*, Op*) ;

bool region_contains_op(Region*, int op_id) ;

bool region_edge_crosses_boundary(Region*, Edge*) ;

bool region_op_at_boundary(Region*, Op*) ;

bool region_op_at_entry(Region*, Op*) ;

bool region_op_at_exit(Region*, Op*) ;

bool region_has_predicated_ops(Region*) ;

Region* ancestor_with_same_parent(Region* region, Region* parent);

Procedure* el_get_enclosing_procedure(Region* reg);

// Change all the macros that alias to a single name.
// This is needed because not all parts of the compiler understand when two
// macros alias that they are actually the same register.
void make_macros_canonical(Region*);

//  --------------------region_find_common_ancestor---------------------------
// Find the closest region that is a common ancestor of the two regions
Region* region_find_common_ancestor(Region* region1, Region* region2);

//Function for finding the closest common ancestor of two compound
//regions second and first; anc_second and anc_first are the ancestors 
//of the second and first that are the direct children of the common ancestor
Compound_region* innermost_common_ancestor(Region* second,
					   Region* first,
					   Compound_region*& anc_second,
					   Compound_region*& anc_first);


Alt_num get_new_in_alt_num(Op*) ;

Alt_num get_new_out_alt_num(Op*) ;

bool is_dataflow_merge_switch(Op*) ;

Op* get_relop_of_merge_switch(Op*) ;

// Compare region frequencies
bool region_less_than(Region* region1, Region* region2);

extern Hash_set<Region*> El_hb_bb_set;
extern Hash_map<int, Region*> El_hb_bb_id_map;
extern void El_setup_hb_bb_id_map(Region *reg);

extern Hash_map<int, Op*> El_op_id_map;
extern void El_setup_op_id_map(Region *reg);

extern double El_calc_op_weight(Op *op);
extern void El_install_missing_cfreq_attrs(Compound_region *cr);

// Fixing Dlist order
void region_sort_subregions_wrt_layout(Compound_region* r) ;

void region_sort_immediate_subregions_wrt_layout(Compound_region* r) ;

Region*  get_region_fallthrough_succ_with_same_parent(Region* r) ;

#endif


