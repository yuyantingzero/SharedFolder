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
//      File:           template_libgraph.cpp
//      Authors:        Sadun Anik
//      Created:        March 1998
//      Description:    GCC templates for libgraph.a
//
//
/////////////////////////////////////////////////////////////////////////////

#include "attributes.h"
#include "el_expr.h"
#include "iterators.h"
#include "slist.h"

class El_expression;
class El_jumptbl;
class El_data;
class El_data_access_declarator;
class DType;

#include "dlist.cpp"

template class Dnode<Region *>;

template class Dlist<Basicblock *>;
template class Dlist<Op *>;
template class Dlist<Operand>;
template class Dlist<Region *>;

template class Dlist_iterator<Basicblock *>;
template class Dlist_iterator<Op *>;
// template class Dlist_iterator<Operand>;
template class Dlist_iterator<Region *>;

template class Dlist_filterator<Region *>;

#include "list.cpp"

// template class List<bool>;
template class List<Procedure*>;
template class List<Dlist_iterator<Region *> *>;
template class List<Edge *>;
template class List_iterator<Edge *>;
template class List<El_data *>;
template class List<El_jumptbl *>;
template class List_iterator<El_jumptbl *>;
template class List<El_data_access_declarator *>;
// template class List<El_ref > ;
template class List<Op *>;
template class List<Operand>;
template class List<Pair<El_ref, El_ref> *>;
template class List<Region *>;
template class List<Opcode>;
template class List<List <Op*> >;
template class List<El_datalist*>;
template class List_iterator<El_datalist*>;

template class List_iterator<Procedure*>;
// template class List_iterator<Edge *>;
template class List_iterator<El_data *>;
// template class List_iterator<El_jumptbl *>;
template class List_iterator<El_data_access_declarator *>;
// template class List_iterator<El_ref > ;
template class List_iterator<Op *>;
template class List_iterator<Operand>;
// template class List_iterator<Pair<El_ref, El_ref> *>;
template class List_iterator<Region *>;


template class List_filterator<Edge *>;
template class List_filterator<Op *>;
/* Shivarama Rao K. Added on 22nd February */
// template class List_iterator<Opcode>;
template class List_iterator<List<Op*> >;

#include "list_set.cpp"

template class List_set<El_ref > ;
template class List_set<Op*>;
template class List_set<Operand>;
template class List_set_iterator<Operand>;
// template class List_set<Op const *>;
template class List_set<Basicblock *>;
template class List_set<Compound_region *>;
// template class List_set_iterator<El_ref > ;
template class List_set_iterator<Op*>;
// template class List_set_iterator<Op const *>;
template class List_set_iterator<Basicblock *>;
template class List_set_iterator<Compound_region *>;
template class List_set<Stuple<int, Op*> >;
template class List_set_iterator<Stuple<int, Op*> >;

#include "filter.cpp"

template class Filter<Region *>;
template class Filter<Op *>;
template class Filter<Edge *>;

template class Filter_and<Edge *>;

template class Filter_not<Edge *>;

template class Filter_or<Region *>;

#include "hash_set.cpp"

template class Hash_set<Basicblock *>;
template class Hash_set<Compound_region *>;
template class Hash_set<Edge *>;
template class Hash_set<Hyperblock *>;
// template class Hash_set<Hash_set<Edge *> >;
template class Hash_set<Op *>;
template class Hash_set<Operand>;
template class Hash_set<Opcode>;
template class Hash_set<Region *>;
template class Hash_set<Macro_name>;

template class Hash_set_iterator<Basicblock *>;
template class Hash_set_iterator<Compound_region *>;
template class Hash_set_iterator<Edge *>;
// template class Hash_set_iterator<Hyperblock *>;
// template class Hash_set_iterator<Hash_set<Edge *> >;
template class Hash_set_iterator<Op *>;
template class Hash_set_iterator<Operand>;
// template class Hash_set_iterator<Opcode>;
template class Hash_set_iterator<Region *>;
// template class Hash_set_iterator<Macro_name>;

template class Hash_set_filterator<Edge *>;
template class Hash_set_filterator<Op *>;
template class Hash_set_filterator<Region *>;

#include "hash_map.cpp"

template class Hash_map<Basicblock *, Basicblock *>;
template class Hash_map<Basicblock *, double>;
template class Hash_map<Basicblock *, int>;
template class Hash_map<Edge *, int>;
template class Hash_map<El_expr, int>;
template class Hash_map<El_ref, List<El_ref> *>;
// template class Hash_map<El_ref, List<El_ref> >;
// template class Hash_map<El_ref,Vector<int> >;
template class Hash_map<El_ref,int >;
template class Hash_map<IR_BASE_OPCODE, eString>;
template class Hash_map<Op *, List<int> *>;
template class Hash_map<Op *, List<Edge *> *>;
template class Hash_map<Op *, List<Op *> *>;
template class Hash_map<Op *, bool>;
template class Hash_map<Op *, int>;
template class Hash_map<Op *, double>;
template class Hash_map<Op *, Operand>;
// template class Hash_map<Opcode, char *>;
template class Hash_map<Opcode, eString>;
template class Hash_map<Opcode, int>;
template class Hash_map<Opcode, Op *>;
template class Hash_map<Operand *, int>;
template class Hash_map<Operand, Op *>;
template class Hash_map<Operand, int>;
template class Hash_map<Region *, int>;
// template class Hash_map<Reg_file, char *>;
template class Hash_map<eString, Opcode>;
template class Hash_map<eString, Procedure*>;
template class Hash_map<int, Op *>;
template class Hash_map<int, Opcode>;
template class Hash_map<int, Operand>;
template class Hash_map<int, Region *>;
template class Hash_map<int, Macro_reg>;
template class Hash_map<Symboltable_entry, Symbol_table_info>;
template class Hash_map<int, Spatial_ref>;
template class Hash_map<int, Exsym_info*>;
template class Hash_map<eString, Macro_name>;
template class Hash_map<Macro_name, eString>;


template class Hash_map_iterator<Basicblock *, Basicblock *>;
template class Hash_map_iterator<Edge *, int>;
template class Hash_map_iterator<El_expr, int>;
template class Hash_map_iterator<El_ref, List<El_ref> *>;
// template class Hash_map_iterator<El_ref,Vector<int> >;
template class Hash_map_iterator<El_ref,int >;
template class Hash_map_iterator<Op *, int>;
template class Hash_map_iterator<Op *, List<int> *>;
template class Hash_map_iterator<Opcode,int >;
template class Hash_map_iterator<Opcode, eString>;
// template class Hash_map_iterator<Operand *, int>;
template class Hash_map_iterator<Operand, Op *>;
template class Hash_map_iterator<Operand, int>;
// template class Hash_map_iterator<Reg_file, char *>;
template class Hash_map_iterator<Symboltable_entry, Symbol_table_info>;
template class Hash_map_iterator<int, int>;
template class Hash_map_iterator<int, Spatial_ref>;
template class Hash_map_iterator<int, Exsym_info*>;
// template class Hash_map_iterator<eString, Macro_name>;
// template class Hash_map_iterator<Macro_name, eString>;

#include "hash_table.cpp"

// template class Hash_table<Opcode>;
template class Hash_table<eString>;
template class Hash_table<El_ref>;
// template class Hash_table_iterator<Edge *>;
// template class Hash_table_iterator<Op *>;
// template class Hash_table_iterator<Opcode>;
// template class Hash_table_iterator<Region *>;

#include "map.cpp"

template class Map<int, Pair<int,int> >;
template class Map_iterator<int, Pair<int,int> >;

#include "tuples.cpp"
// template class Triple<int, int, double>;
template class Quad<char*, int, int, double>;
// template class List<Triple<int, int, double> >;
template class List<Quad<char*, int, int, double> >;
// template class List_iterator<Triple<int, int, double> >;
template class List_iterator<Quad<char*, int, int, double> >;
template class Hash_map<Operand, Quad<char*, int, int, double> >;

template class Map<Attribute_type, Base_attribute *>;
template class Map<Basicblock *, Operand>;
template class Map<Edge *, Edge *>;
template class Map<Edge *, Hash_set<Operand> >;
template class Map<Hash_set<Edge *>, Operand>;
template class Map<Op *, List<Operand> *>;
template class Map<Op *, Op *>;
template class Map<Op *, int>;
template class Map<Operand, Op *>;
template class Map<Operand, Operand>;
template class Map<Operand, int>;
template class Map<Pair<Compound_region *, Op *>, Pair<int, bool> >;
template class Map<Pair<Compound_region *, Op *>, int>;
template class Map<Pair<Op *, Op *>, int>;
template class Map<Region *, Region *>;
template class Map<eString, List<Operand> >;
template class Map<eString, void *>;
template class Map<int, Op *>;
template class Map<int, Region *>;

template class Map_domain_iterator<Attribute_type, Base_attribute *>;

template class Map_iterator<Attribute_type, Base_attribute *>;
// template class Map_iterator<Op *, List<Operand> *>;
template class Map_iterator<Op *, Op *>;
template class Map_iterator<Op *, int>;
template class Map_iterator<Operand, Op *>;
template class Map_iterator<Operand, int>;
template class Map_iterator<Pair<Compound_region *, Op *>, Pair<int, bool> >;
template class Map_iterator<Pair<Compound_region *, Op *>, int>;
template class Map_iterator<Pair<Op *, Op *>, int>;
template class Map_iterator<Region *, Region *>;
template class Map_iterator<eString, List<Operand> >;

#include "operand_concat.cpp"

template class Operand_2_iter<Op_explicit_sources, Op_implicit_sources>;
// template class Operand_2_iter<Op_explicit_dests, Op_implicit_dests>;
// template class Operand_2_iter<Op_predicate_sources, Op_explicit_sources>;
template class Operand_2_iter<Operand_4_iter<Op_predicate_sources, Op_explicit_sources, Op_implicit_sources, Op_mem_sources>, Operand_2_iter<Op_taken_liveout_sources, Op_fallthrough_liveout_sources> >;

template class Operand_2_iter<Operand_2_iter<Op_predicate_sources, Op_explicit_sources>, Op_explicit_dests>;
// template class Operand_2_iter<Op_implicit_sources, Op_implicit_dests>;
// template class Operand_2_iter<Operand_3_iter<Op_predicate_sources, Op_explicit_sources, Op_implicit_sources>, Operand_2_iter<Op_explicit_dests, Op_implicit_dests> >;
// template class Operand_2_iter<Op_mem_sources, Op_mem_dests>;
template class Operand_2_iter<Operand_2_iter<Operand_3_iter<Op_predicate_sources, Op_explicit_sources, Op_implicit_sources>, Operand_2_iter<Op_explicit_dests, Op_implicit_dests> >, Operand_2_iter<Op_mem_sources, Op_mem_dests> >;

// template class Operand_3_iter<Op_explicit_dests, Op_implicit_dests, Op_livein_dests>;
template class Operand_3_iter<Op_explicit_dests, Op_implicit_dests,Op_mem_dests>;
template class Operand_3_iter<Op_explicit_sources, Op_implicit_sources, Op_mem_sources>;
// template class Operand_3_iter<Op_predicate_sources, Op_explicit_sources, Op_implicit_sources>;

template class Operand_4_iter<Op_explicit_dests, Op_implicit_dests, Op_mem_dests, Op_livein_dests>;
// template class Operand_4_iter<Op_predicate_sources, Op_explicit_sources, Op_implicit_sources, Op_mem_sources>;

#include "slist.cpp"

// template class Slist<Stuple<double, Op *> >;
// template class Slist<Stuple<int, Op *> >;
// template class Slist<Stuple<double, Compound_region *> >;
// template class Slist_iterator<Stuple<double, Compound_region *> > ;

// template class Stuple<double, Op *>;
// template class Stuple<int, Op *>;
// template class Stuple<double, Compound_region *>;

#include "vector.cpp"

template class Vector<Edge *>;
template class Vector<El_expr>;
template class Vector<El_expression *>;
template class Vector<El_ref>;
// template class Vector<Hash_map<El_ref, List<El_ref> >* >;
template class Vector<Hash_set<Operand> >;
template class Vector<List<Op *> >;
// template class Vector<List<Opcode> >;
template class Vector<Op *>;
template class Vector<Opcode>;
template class Vector<Operand>;
template class Vector<Region *>;
template class Vector<DType>;

//support for distributed instruction fetch, hongtaoz.
// template class Vector<Compound_region*>;
// template class Vector<IR_outstream*>;
template class Vector<unsigned>;

enum Spatial_ref_type;
template class Vector<Pair<Spatial_ref_type,int> >;

class Spatial_ref;
// template class Vector<Spatial_ref>;

// Physical file binding attribute, Rajiv
// template class Hash_set<eString>;
template class Hash_map<int, Hash_set<eString> >;
template class Hash_map_iterator<int, Hash_set<eString> >;

// AccSpec
template class List<AccSpec *>;
template class List_iterator<AccSpec *>;

// Function instantiations

template ostream& operator<<(ostream &, Hash_map<Operand *, int>&);
template ostream& operator<<(ostream &, const Hash_set<Operand> &);
// template ostream& operator<<(ostream &, List<El_ref >&);
template ostream& operator<<(ostream &, List<Operand >&);
template ostream& operator<<(ostream &, Map<Operand, int> &);
// template ostream& operator<<(ostream &, const Pair<Operand *, int>&);
