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
//      File:           el_loop.h
//      Author:         David I. August
//      Created:        May 1995
//      Description:    Loop Class Definition
//
/////////////////////////////////////////////////////////////////////////////

#ifndef EL_LOOP_H
#define EL_LOOP_H

#include "hash_set.h"
#include "list.h"
#include "list_set.h"
#include "map.h"
#include "vector.h"
#include "operand.h"
#include "el_loop_filters.h"

class Dominator_CFA_solver ;
class Basicblock ;
class Op ;
class Compound_region ;
class Region ;

enum Loop_category {
  UNCLASSIFIED,
  MARKED_FOR_DELETION,
  DO_LOOP,
  WHILE_LOOP
} ;

enum Loop_swp_status {
  UNKNOWN,
  NOT_SWP_ABLE,
  SWP_ABLE
} ;

class El_Loop {
  friend ostream& operator<<(ostream& os, const El_Loop& loop);
  
public:
  El_Loop();
  El_Loop(int, Basicblock*);
  ~El_Loop();
  
  int id;
  int nesting_level;
  Loop_category category;
  Loop_swp_status swp_status;
  Basicblock *preloop_block;
  Basicblock *header_block;
  Basicblock *remainder_block;
  Basicblock *preremainder_block;
  double num_invocation;
  double num_iteration;
  int loopcount; // iterations/invocation, if compile-time constant. -1 if unknown.
  List_set<Basicblock*> loop_blocks;
  Hash_set<Basicblock*> back_edge_blocks;
  Hash_set<Basicblock*> exit_blocks;
  Hash_set<Basicblock*> postloop_blocks;

  Hash_set<Operand> basic_ind_var;
  Hash_set<Op*> basic_ind_var_ops;
  Hash_map<Operand,Operand> *basic_ind_var_init_vals;
  Hash_map<Operand,Op*> *basic_ind_var_init_ops;
  Operand primary_ind_var;
  Hash_set<Op*> primary_ind_var_ops;

  /* Making reference to source code easily accessible (when available). */
  eString source_filename;
  int source_line_num;

  /*
  ** The following functions query the IR for information
  */
  void find_induction_variables();
  void find_induction_ops();
  void find_induction_init_vals();
  void find_primary_ind_var(Dominator_CFA_solver*);
  void find_primary_ind_var_ops();

  bool region_is_subset(Compound_region*);
  bool region_is_subset(Op*);
  
  void mark_swp_able();
  bool is_contiguous();

  int calculate_lc(Operand&,Operand&,Operand&,bool,bool);
  /*
  ** The following functions MODIFY the IR
  */
  void insert_brlc();
  void insert_loop_region();

  // This function inserts a generic compund_region
  // which contains all the loop basic blocks but not
  // the loop pre and postheaders
  Compound_region* insert_compound_region() ; 

protected:
  void print(ostream& os) const;

private:
  bool is_invariant(Operand*);
  bool is_unique_def(Op*);
  int potential_primary_ind_op_count(Basicblock*);
  
} ;


class El_Loop_Graph {
  friend ostream& operator<<(ostream& os, const El_Loop_Graph& loop_graph);
  
public:
  El_Loop_Graph();
  El_Loop_Graph(Compound_region*);
  ~El_Loop_Graph();

  /*
  ** These member functions QUERY the IR to obtain loop information.
  */
  void find_induction_variable_info(const Loop_filter*);
  void find_swp_able_loops(const Loop_filter*);

  /*
  ** These member functions MODIFY the IR in various ways.
  */
  void create_preloop_blocks(const Loop_filter*);
  void create_postloop_blocks(const Loop_filter*);
  void create_remainder_blocks(const Loop_filter*);
  void create_preremainder_blocks(const Loop_filter*);
  void insert_loop_regions(const Loop_filter*);
  void insert_brlc(const Loop_filter*);


  /*
  ** Loop graph information
  */
  Map<int, El_Loop *>id;
  Map<El_Loop*, El_Loop *>child;
  Map<El_Loop*, El_Loop *>parent;
  Map<El_Loop*, El_Loop *>sibling;
  El_Loop* first_loop;
  int num_loops;
  
protected:
  void print(ostream& os) const;

private:
  Compound_region *region;
  List_set<Basicblock*> bb_set;
  int cur_id;
  Dominator_CFA_solver *control;
  
  void Find_Blocks_In_Loop(El_Loop*);
  void Delete_Marked_Loops();
  void Find_Num_Invocation(El_Loop*);
  void Loop_Detection();
  Basicblock* Create_Loop_Postheader(El_Loop*, Basicblock*, Basicblock*);
  Basicblock* Create_Loop_Preheader(El_Loop*);
  void Find_Nesting_Levels();
  void Build_Loop_Hierarchy();
  void Find_Exit_Blocks_In_Loop(El_Loop*);
  void Mark_Loop_Headers();
  void Connect_Loop_Hierarchy();
  void Merge_Loops();
  bool filter_match(const Loop_filter*, El_Loop*);
} ;

El_Loop*  el_find_innermost_loop_containing_bb(El_Loop_Graph& loops,Basicblock* bb);
void el_find_loop_bbs_not_in_nested_loops(El_Loop* loop,El_Loop_Graph& loops,
				       List_set<Basicblock*>& todo_bbs);
#endif


