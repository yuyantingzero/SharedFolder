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
//      File:           pred_interface.h
//      Authors:        Sadun Anik
//      Created:        December 1995
//      Description:    Defines Pred_jar and Pred_cookie for the universal
//                      interface between predicated work and pqs
//
/////////////////////////////////////////////////////////////////////////////
#ifndef _PRED_INTERFACE
#define _PRED_INTERFACE

#include <iostream>
#include "bit_vector.h"
#include "op.h"

using namespace std;

enum Pred_cookie_type { PQ_NULL, PQ_FALSE, PQ_TRUE, PQ_REF, PQ_EXPR } ;

class Compound_region ;
class PQS ;
class Pred_jar ;
class El_ref ;
class Pred;
class Pred_expr ;

class Pred_cookie {
   friend ostream& operator << (ostream&, const Pred_cookie&);
   friend class Pred_jar ;
public:
   Pred_cookie() ;
   Pred_cookie(bool val) ;
   Pred_cookie(const Pred_cookie& pq) ;

   ~Pred_cookie() ;

   Pred_cookie& operator=(const Pred_cookie&) ;
   void set_to_true() ;
   void set_to_false() ;
   void reset_to_true() ;
   void reset_to_false() ;


   bool operator==(const Pred_cookie&) const ;
   bool operator!=(const Pred_cookie&) const ;
  
   bool is_subset(const Pred_cookie&) const ;
   bool is_disjoint(const Pred_cookie&) const ;

   Pred_cookie& lub_sum(const Pred_cookie&) ;
   Pred_cookie& lub_diff(const Pred_cookie&) ;

   Pred_cookie& glb_sum(const Pred_cookie&) ;
   Pred_cookie& glb_diff(const Pred_cookie&) ;

   void print();
   
private:
   Pred_cookie(const Pred_jar& pj, Pred p) ;

   void promote_to_true_expression(PQS* pqs_ptr) ;
   void promote_to_false_expression(PQS* pqs_ptr) ;
   void promote_to_expression() ;
   void promote_to_pred(PQS* pqs_ptr) ;

   void simplify() ;

   bool is_true() const;
   bool is_false() const;
   
   PQS* pqs ;
   Pred_cookie_type type ;
   Pred* pred ;
   Pred_expr* pexpr ;
} ;

class Pred_jar {
   friend class Pred_cookie ;
public:
   Pred_jar(Compound_region* blk) ;
   ~Pred_jar() ;

   static Pred_cookie get_false() ;
   static Pred_cookie get_true() ;
      
   Pred_cookie get_lub_guard(El_ref& ref) ;
   Pred_cookie get_glb_guard(El_ref& ref) ;
      
   Pred_cookie get_lub_src_expression_guard(Op* op) ;
   Pred_cookie get_glb_src_expression_guard(Op* op) ;

   Pred_cookie get_lub_path_guard(Op* first_op, Op* last_op) ;
   Pred_cookie get_glb_path_guard(Op* first_op, Op* last_op) ;

   Pred_cookie get_lub_taken_branch_guard(Op* op) ;
   Pred_cookie get_glb_taken_branch_guard(Op* op) ;
      
   Pred_cookie get_lub_fall_through_branch_guard(Op* op) ;
   Pred_cookie get_glb_fall_through_branch_guard(Op* op) ;

private:
   Pred_cookie get_lub_swp_branch_guard(Op* swp_branch) ;
   Pred_cookie get_glb_swp_branch_guard(Op* swp_branch) ;
      
   Compound_region* block ;
   PQS* pqs ;
} ;

extern Pred_cookie El_true_cookie ;
extern Pred_cookie El_false_cookie ;
#endif
