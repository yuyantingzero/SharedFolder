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
//      File:           el_available_expression.h
//      Authors:        Sadun Anik
//      Created:        April 1997
//      Description:    Available expressions analysis interface
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _EL_AVAILABLE_EXPRESSION_H
#define _EL_AVAILABLE_EXPRESSION_H

#include <iostream>
#include "el_expr.h"
#include "bit_vector.h"
#include "adjlist_graph.h"
#include "region.h"
#include "op.h"
#include "hash_set.h"
#include "hash_map.h"
#include "pred_interface.h"
#include "iterators.h"

using namespace std;

enum AV_EXPR_FLAG {
   AV_EXPR_UNARY, // Unary expressions only 
   AV_EXPR_BINARY, // Binary expressions only
   AV_EXPR_UNARY_OR_BINARY // Unary or binary expressions
} ;

enum AV_EXPR_LOCAL_WALK_FLAG {
   AV_EXPR_TRANSFER_FUNCTION, 
   AV_EXPR_LOCAL_QUERY 
} ;


class Available_expression_data {
public:
   Available_expression_data() ;
   Available_expression_data(int) ;

   bool operator==(const Available_expression_data& ad) const;

   Bitvector av_in ;
   Bitvector av_out ;

   Bitvector av_in_initial ;

   Bitvector gen ;
   Bitvector kill ;
} ;

class Available_expression_map {
public:
   Available_expression_map(Region* r, AV_EXPR_FLAG fl=AV_EXPR_UNARY_OR_BINARY,
       bool use_normalized_expr = true) ;

   bool is_interesting(Op*) ;

   
   int symbol_count ;

   Hash_map<Operand,int> operand_map ;             // Operand to int
   Vector<Operand> rev_operand_map ;               // int to Operand

   Hash_map<El_expr,int> expression_map ;          // Expression to int
   Vector<El_expr> rev_expression_map ;            // int to Expression

   Hash_map<Op*, int> op_to_expression_map ;       // Op to Expression_int
   Vector<List<Op*> > expression_to_op_map ;       // Expression_int to Op

   Vector<Hash_set<int> > operand_to_killed_expr_map ;  // Opernd_int to Expr_int
private:
   El_expr* get_normalized_expression(Op*) ;
   AV_EXPR_FLAG filter_flag ;

   // parkhc 1/18/2005
   bool is_normalized;

} ;

class Available_expression_solver {
   public:
      Available_expression_solver(Region* r, AV_EXPR_FLAG fl=AV_EXPR_UNARY_OR_BINARY, bool use_normalized_expr = true) ;
      ~Available_expression_solver() ;

      void solve() ;
      Pred_cookie get_available_expression_cookie_before_op
      (Compound_region* scope, Op* src_expr_op, Op* before_this_op) ;

      bool is_op_fully_redundant(Op* expr_op) ;
      List<int> get_expression_list() ;
      List<Op*>& get_ops_for_expression(int expr_id) ;

      Alist_region_tiling_graph node_graph ;
      Available_expression_map emap ;
      Vector<Available_expression_data> node_data ;  

   private:
      bool is_normalized;

      void compute_local_transfer_function(int) ;
      void compute_local_remap_function(int) ;


      void meet_function(int n, bool& tag) ;
      void remap_function(int n, bool& tag) ;
      void transfer_function(int n, bool& tag);

      void forward_local_block_walk(Compound_region* r, Op* entry_op, 
          Op* exit_op, 
          Vector<Pred_cookie>* p_gen_ptr, Vector<Pred_cookie>* p_kill_ptr, 
          Pred_jar& pj, AV_EXPR_LOCAL_WALK_FLAG flg) ;

      void reset_pred_cookie_vector(Vector<Pred_cookie>*& pcptr, Pred_cookie& value) ;

      Vector<Pred_cookie>* tmp_gen ;
      Vector<Pred_cookie>* tmp_kill ;
} ;



#endif
