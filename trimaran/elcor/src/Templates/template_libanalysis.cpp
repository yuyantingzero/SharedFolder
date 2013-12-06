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
//      File:           template_libanalysis.cpp
//      Author:         Scott A. Mahlke
//      Created:        August 1996
//      Description:    manual template instantiations for gcc
//
/////////////////////////////////////////////////////////////////////////////

#include "control_analysis_solver.h"
#include "el_available_expression.h"
// #include "flow_analysis_solver.h"
#include "partition.h"
#include "pqs.h"
// #include "pred_interface.h"
#include "reaching_defs_solver.h"
// #include "ref.h"
#include "wconstraint.h"
#include "dep_len.h"


class DdNode;
class BB;
class LoopBody;
class El_Loop;
class Base_operand;
class DepLen_OpInfo;
class ResLen_OpInfo;

#include "filter.cpp"

template class Filter<El_Loop *>;

#include "hash_set.cpp"

template class Hash_set<LoopBody *>;
template class Hash_set<Base_operand *>;

template class Hash_set_iterator<LoopBody *>;
template class Hash_set_iterator<Base_operand *>;

#include "hash_table.cpp"

// template class Hash_table<LoopBody *>;

// template class Hash_table_iterator<LoopBody *>;
// template class Hash_table_iterator<Operand>;

#include "hash_map.cpp"


// template class Hash_map<El_ref, DdNode * > ;
template class Hash_map<El_ref, WConstraint * > ;
template class Hash_map<Base_operand*, int > ;
template class Hash_map_iterator<Base_operand*, int> ;
template class Hash_map_iterator<El_ref, WConstraint * > ;
template class Hash_map<Operand, Operand >;
template class Hash_map<Op*, Hash_set<Op*> >;
template class Hash_map_iterator<Op*, Hash_set<Op*> >;
template class Hash_map<Op*, DepLen_OpInfo*>;
template class Hash_map<Op*, ResLen_OpInfo*>;
template class Hash_map_iterator<Operand*, int>;

#include "list.cpp"

template class List<Pair<Op *, Pred_expr *> >;
template class List<Pair<Operand, Pred_cookie> >;
template class List<Partition * >;
template class List<Partition>;
template class List<Quad<Operand, Op *, int, Pred_expr *> >;
// template class List<WConstraint>;
// template ostream& operator<<(ostream &, List<WConstraint> &);
template class List<OpPtr_Double>;

template class List_iterator<El_ref>;
template class List_iterator<Pair<Op *, Pred_expr *> >;
template class List_iterator<Pair<Operand, Pred_cookie> >;
template class List_iterator<Partition * >;
template class List_iterator<Partition>;
template class List_iterator<Quad<Operand, Op *, int, Pred_expr *> >;

#include "list_set.cpp"

template class List_set<Hash_set<Edge*> >;
template class List_set_iterator<Hash_set<Edge*> >;

#include "map.cpp"

template class Map<Basicblock*, Control_dep_CFA_solver_data>;
template class Map<El_Loop *, El_Loop *>;
template class Map<int, El_Loop *>;

template class Map_iterator<int, El_Loop *>;

#include "tuples.cpp"

// template class Pair<Op *, Pred_expr *>;
template class Quad<Operand, Op *, int, Pred_expr *>;

#include "vector.cpp"

template class Vector<Available_expression_data >;
template class Vector<Dominator_CFA_solver_data>;
template class Vector<El_Loop *>;

template class Vector<List<Partition * > >;
template class Vector<Liveness_solver_data >;
template class Vector<Pred_cookie >;
template class Vector<Pred_expr >;
template class Vector<Reaching_defs_solver_data>;

template void shrink_vector<int>(Vector<int>&, Bitvector&);
template void shrink_vector<Edge*>(Vector<Edge*>&, Bitvector&);

