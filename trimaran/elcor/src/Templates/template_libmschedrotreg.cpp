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
//      File:           template_libmschedrotreg.cpp
//      Author:         Scott A. Mahlke, Shail Aditya
//      Created:        August 1996
//      Description:    manual template instantiations for gcc
//      Modified:       cleaned and combined into a mschedrotreg library 
//
/////////////////////////////////////////////////////////////////////////////

// #include "RU.h"
#include "ModuloScheduler.h"
#include "stage_sched.h"

// #include "operand.h"
// #include "rr_allocation.h"
// #include "rr_file.h"
// #include "rr_life.h"
#include "rr_util.h"

// #include "tuples.cpp"
// template class Quad<Edge*, int, int, int>;

#include "filter.cpp"

template class Filter<SS_edge*>;

#include "list.cpp"

template class List<pattern>;
// template class List<Pair<Op*, SchedInfo* > >;
template class List<SS_edge*>;
template class List<SS_vertex*>;
template class List<Order_strategy >;
template class List<RR_sp_element >;
// template class List<Pair<Edge*, int> >;
// template class List<Quad<Edge*, int, int, int> >;
// template class List<FU_group*>;

// template class List_iterator<pattern>;
template class List_iterator<SS_edge*>;
template class List_iterator<SS_vertex*>;
template class List_filterator<SS_edge*>;
template class List_iterator<Order_strategy >;
template class List_iterator<RR_sp_element >;
template class List_iterator<char*>;
template class List_iterator<List<int> >;

#include "hash_table.cpp"

// template class Hash_table<RR_ref_data >;
// template class Hash_table<RR_ref_data* >;

// template class Hash_table_iterator<RR_ref_data >;
// template class Hash_table_iterator<RR_ref_data* >;

#include "hash_set.cpp"

template class Hash_set<RR_ref_data >;
template class Hash_set<RR_ref_data* >;
// template class Hash_set<Storage* >;
template class Hash_set_iterator<RR_ref_data >;
template class Hash_set_iterator<RR_ref_data* >;
// template class Hash_set<Matrix<int> >;
// template class Hash_set<FU_group*>;

#include "map.cpp"

template class Map<Op *, SS_vertex *>;
template class Map<int,Hash_set<RR_ref_data >* >;

#include "hash_map.cpp"

template class Hash_map<int, unsigned>;
template class Hash_map<Op*, SchedInfo *>;
// template class Hash_map <Op*, SchedInfo>;
// template class Hash_map <Pair<int, int>, int>;
//template class Hash_map <Pair<int, int>, Storage*>;
//template class Hash_map <Op*, List<Edge*> >;
//template class Hash_map <Op*, FU_group*>;

// template class Hash_map_domain_iterator<Op*, SchedInfo* >;
template class Hash_map_iterator<Op*,SchedInfo* >;
// template class Hash_map_iterator<Op*, SchedInfo>;
//template class Hash_map_iterator<Pair<int, int>, Storage*>;

#include "vector.cpp"

template class Vector<Vector<Op*> >;
template class Vector<List<El_ref> >;
template class Vector<RR_file_data *>;
template class Vector<RR_lifetime >;
template class Vector<RR_ref_data >;
// template class Vector<Hash_set<eString> >;

#include "matrix.cpp"

template class Matrix<List<RR_sp_element > >;
// template class Matrix<List<Pair<Edge*, int> > >;
// template class Matrix<Hash_set<Edge*> >;

#include "slist.cpp"

// template class Stuple<int, Pair<Op*, int> >;
//template class Stuple<int, Storage*>;
// template class Slist<Stuple<int, Pair<Op*, int> > >;
//template class Slist<Stuple<int, Storage*> >;
// template class Slist<Stuple<int, Op*> >;
// template class Slist<int>;
//template class Slist<similar_node>;
//template class Slist_iterator<similar_node>;
//template class Stuple<double, List<Op*> >;
//template class Slist<Stuple<double, List<Op*> > >;
template class Stuple<int, List_set<Op*> >;
template class Slist<Stuple<int, List_set<Op*> > >;
template class Slist_iterator<Stuple<int, List_set<Op*> > >;

// template ostream& operator<<(ostream&, Slist<int>&);
template ostream& operator<<(ostream&, Vector<RR_ref_data>&);
template ostream& operator<<(ostream&, Vector<RR_lifetime>&);

// Tracware base class template instantiations
// template class Hash_map<char*, Hash_set<Op*>*>;
// template class Hash_map_iterator<char*, Hash_set<Op*>*>;

// template class Hash_map<Op*, char*>;
// template class Hash_map_iterator<Op*, char*>;
