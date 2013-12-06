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
//      File:           pred_interface.cpp
//      Authors:        Sadun Anik
//      Created:        December 1995
//      Description:    Defines Pred_jar and Pred_cookie for the universal
//                      interface between predicated work and pqs
//
/////////////////////////////////////////////////////////////////////////////
#include "pred_interface.h"
#include "pqs.h"
#include "attributes.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "reaching_defs_solver.h"
#include "ir_writer.h"

Pred_cookie El_true_cookie(true) ;
Pred_cookie El_false_cookie(false) ;

////////////////////////////////////////////////////////////////////////////////

#define SIMPLIFY_AND_RETURN(PQ) (if (PQ.is_true()) { return (El_true_cookie) ; } \
                            else if (PQ.is_false()) {return (El_false_cookie) ; } \
                            else {return (PQ) ; })


Pred_jar::Pred_jar(Compound_region* blk) 
{
   block = blk ;
   pqs = get_pqs(blk) ;
}

Pred_jar::~Pred_jar()
{
}

Pred_cookie Pred_jar::get_false()
{
   return (El_false_cookie) ;
}

Pred_cookie Pred_jar::get_true() 
{
   return (El_true_cookie) ;
}

Pred_cookie Pred_jar::get_lub_guard(El_ref& ref)
{
   if (is_swp_branch(ref.get_op())) {
      return (get_lub_swp_branch_guard(ref.get_op())) ;
   }
   else {
      switch (ref.get_ref_type()) {
	 case PRED_SRC:
         return (El_true_cookie) ;
         break ;
	 case EXP_SRC:
	 case IMP_SRC:
	 case MEM_SRC:
         {
            Op* op = ref.get_op() ;
            if (op->predicated()) {
               Operand p = op->src(PRED1);
               if (p.is_reg()) {
                  Pred_cookie tmp_cq(*this, pqs->lub_src_guard(op , SRC1));
                  return (tmp_cq) ;
               }
               else {
                  if (p.is_predicate_true())
                   return (El_true_cookie) ;
                  else
                    return (El_false_cookie);
               }
            }
            else {
               if (is_switch(op)) {
                  Pred_cookie tmp_cq(*this, pqs->lub_src_guard(op , SRC1));
                  return (tmp_cq) ;
               }
               else {
                  return (El_true_cookie) ;
               }
            }
         }
         break ;
	 case EXP_DEST:
	 case IMP_DEST:
	 case MEM_DEST:
         {
            Op* op = ref.get_op() ;
            if (is_cmpp(op)) {
               Pred_cookie tmp_cq(*this, pqs->lub_dest_guard(op, ref.get_port_num()));
               return (tmp_cq) ;
            }
            else {
               if (op->predicated()) {
                  Operand p = op->src(PRED1);
                  if (p.is_reg()) {
                     Pred_cookie tmp_cq(*this, pqs->lub_dest_guard(op , DEST1));
                     return (tmp_cq) ;
                  }
                  else {
                    if (p.is_predicate_true())
                     return (El_true_cookie) ;
                    else
                      return (El_false_cookie);
                  }
               }
               else {
                  if (is_switch(op)) {
                     Pred_cookie tmp_cq(*this, pqs->lub_dest_guard(op , DEST));
                     return (tmp_cq) ;
                  }
                  else {
                     return (El_true_cookie) ;
                  }
               }
            }
         }
         break;
	 case LIVEOUT_TAKEN_SRC:
	 {
	    Op* op = ref.get_op() ;
	    return (get_lub_taken_branch_guard(op)) ;

	 }
	 break ;
	 case LIVEOUT_FALLTHROUGH_SRC:
	 {
	    Op* op = ref.get_op() ;
	    return (get_lub_fall_through_branch_guard(op)) ;
	 }
	 break ;
	 case LIVEIN_DEST:
	 {
	    return (El_true_cookie) ;
	 }
	 break ;
	 default:
         El_punt("Unknown reference type") ;
         return (El_false_cookie) ;
      }
   }
}

Pred_cookie Pred_jar::get_glb_guard(El_ref& ref)
{
   if (is_swp_branch(ref.get_op())) {
      return (get_glb_swp_branch_guard(ref.get_op())) ;
   }
   else {
      switch (ref.get_ref_type()) {
	 case PRED_SRC:
	 return (El_true_cookie) ;
	 break ;
	 case EXP_SRC:
	 case IMP_SRC:
	 case MEM_SRC:
	 {
	    Op* op = ref.get_op() ;
	    if (op->predicated()) {
	       Operand p = op->src(PRED1);
	       if (p.is_reg()) {
		  Pred_cookie tmp_cq(*this, pqs->glb_src_guard(op , SRC1));
		  return (tmp_cq) ;
	       }
	       else {
                  if (p.is_predicate_true())
                   return (El_true_cookie) ;
                  else
                    return (El_false_cookie);
	       }
	    }
	    else {
	       if (is_switch(op)) {
		  Pred_cookie tmp_cq(*this, pqs->glb_src_guard(op , SRC1));
		  return (tmp_cq) ;
                  
	       }
	       else {
		  return (El_true_cookie) ;
	       }
	    }
	 }
	 break ;
	 case EXP_DEST:
	 case IMP_DEST:
	 case MEM_DEST:
	 {
	    Op* op = ref.get_op() ;
	    if (is_cmpp(op)) {
	       Pred_cookie tmp_cq(*this, pqs->glb_dest_guard(op, ref.get_port_num()));
	       return (tmp_cq) ;
	    }
	    else {
	       if (op->predicated()) {
		  Operand p = op->src(PRED1);
		  if (p.is_reg()) {
		     Pred_cookie tmp_cq(*this, pqs->glb_dest_guard(op , DEST1));
		     return (tmp_cq) ;
		  }
		  else {
                    if (p.is_predicate_true())
                     return (El_true_cookie) ;
                    else
                      return (El_false_cookie);
		  }
	       }
	       else {
		  if (is_switch(op)) {
		     Pred_cookie tmp_cq(*this, pqs->glb_dest_guard(op , DEST));
		     return (tmp_cq) ;
		  }
		  else {
		     return (El_true_cookie) ;
		  }
	       }
	    }
	 }
	 break;
	 case LIVEOUT_TAKEN_SRC:
	 {
	    Op* op = ref.get_op() ;
	    return (get_glb_taken_branch_guard(op)) ;

	 }
	 break ;
	 case LIVEOUT_FALLTHROUGH_SRC:
	 {
	    Op* op = ref.get_op() ;
	    return (get_glb_fall_through_branch_guard(op)) ;
	 }
	 break ;
	 case LIVEIN_DEST:
	 {
	    return (El_false_cookie) ;
	 }
	 break ;
	 default:
	 El_punt("Unknown reference type") ;
	 return (El_false_cookie) ;
      }
   }
}

Pred_cookie Pred_jar::get_lub_src_expression_guard(Op* op) 
{
   El_ref tmp_ref(op, &(op->src(SRC1)), EXP_SRC) ;
   Pred_cookie tmp_cq = get_lub_guard(tmp_ref) ;
   return (tmp_cq) ;
}

Pred_cookie Pred_jar::get_glb_src_expression_guard(Op* op) 
{
   El_ref tmp_ref(op, &(op->src(SRC1)), EXP_SRC) ;
   Pred_cookie tmp_cq = get_glb_guard(tmp_ref) ;
   return (tmp_cq) ;
}

Pred_cookie Pred_jar::get_lub_taken_branch_guard(Op* op)
{
   Pred_cookie tmp_cq(*this,pqs->lub_branch_pred(op)) ;
   return (tmp_cq) ;
}

Pred_cookie Pred_jar::get_glb_taken_branch_guard(Op* op)
{
   Pred_cookie tmp_cq(*this,pqs->glb_branch_pred(op)) ;
   return (tmp_cq) ;
}

Pred_cookie Pred_jar::get_lub_fall_through_branch_guard(Op* op)
{
   Pred_cookie tmp_cq = El_true_cookie ;
   tmp_cq.lub_diff(get_glb_taken_branch_guard(op)) ;
   return (tmp_cq) ;
}

Pred_cookie Pred_jar::get_glb_fall_through_branch_guard(Op* op)
{
   Pred_cookie tmp_cq = El_true_cookie ;
   tmp_cq.glb_diff(get_lub_taken_branch_guard(op)) ;
   return (tmp_cq) ;
}

Pred_cookie Pred_jar::get_lub_path_guard(Op* first_op, Op* last_op)
{
   Op *entry_op, *exit_op ;
   if (is_control_merge(first_op)) {
      entry_op = first_op ;
      exit_op = last_op ;
   }
   else {
      exit_op = first_op ;
      entry_op = last_op ;
   }
   if (is_swp_branch(exit_op)) {
      return (get_lub_swp_branch_guard(exit_op)) ;
   } 
   else {
      if (exit_op != get_last_region_op_from_subregions (block)) {
         Pred_cookie tmp_cq(*this,pqs->lub_branch_pred(exit_op)) ;
         return (tmp_cq) ;
      }
      else {
	//
	// The following is a hack for blocks with FRP's
	// Instead of returning true, we return the predicate under
	// which the branch executes, i.e. its PRED1. The SRC1
	// lub_gard query should return PRED1
	//
	if (block->flag(EL_REGION_HYPERBLOCK_FRP) == true) {
	  Pred_cookie tmp_cq(*this,pqs->lub_src_guard(exit_op,SRC1)) ;
	  return (tmp_cq) ;
	}
	else {
	  return (El_true_cookie) ;
	}
      }
   }
}

Pred_cookie Pred_jar::get_glb_path_guard(Op* first_op, Op* last_op)
{
   Op *entry_op, *exit_op ;
   if (is_control_merge(first_op)) {
      entry_op = first_op ;
      exit_op = last_op ;
   }
   else {
      exit_op = first_op ;
      entry_op = last_op ;
   }
   if (is_swp_branch(exit_op)) {
      return (get_glb_swp_branch_guard(exit_op)) ;
   } 
   else {
      if (exit_op != get_last_region_op_from_subregions (block)) {
         Pred_cookie tmp_cq(*this,pqs->glb_branch_pred(exit_op)) ;
         return (tmp_cq) ;
      }
      else {
         return (El_true_cookie) ;
      }
   }
}

Pred_cookie Pred_jar::get_lub_swp_branch_guard(Op* swp_branch) 
{
   Region_ops_C0_order opi (swp_branch->parent());
   assert(is_control_merge (*opi));
   opi++;

   // instead of looking for a first real op, find the first op that is
   // guarded by staging predicate : parkhc 02/2007
   Operand staging_pred = swp_branch->dest(DEST1);
   // also keep track of all the staging predicates in other clusters
   Hash_set<int> staging_preds(hash_int, 7);
   staging_preds += staging_pred.vr_num();

   while (opi != NULL) {
      Op* cur_op = *opi;
      if (is_movepp(cur_op) && cur_op->src(SRC1).is_reg())
        if (staging_preds.is_member(cur_op->src(SRC1).vr_num()))
          staging_preds += cur_op->dest(DEST1).vr_num();

      if (cur_op->predicated() && cur_op->src(PRED1).is_reg())
        if (staging_preds.is_member(cur_op->src(PRED1).vr_num()))
           break;
      
      opi++;
   }
   
   Op* first_real_op = *opi;
   assert (!is_switch (first_real_op) && !is_merge (first_real_op));
   Pred_cookie tmp_cq(*this, pqs->lub_src_guard(first_real_op, SRC1));
   return (tmp_cq) ;
}


Pred_cookie Pred_jar::get_glb_swp_branch_guard(Op* swp_branch) 
{
   Region_ops_C0_order opi (swp_branch->parent());
   assert(is_control_merge (*opi));
   opi++;
   
   // instead of looking for a first real op, find the first op that is
   // guarded by staging predicate : parkhc 02/2007
   Operand staging_pred = swp_branch->dest(DEST1);
   // also keep track of all the staging predicates in other clusters
   Hash_set<int> staging_preds(hash_int, 7);
   staging_preds += staging_pred.vr_num();

   while (opi != NULL) {
      Op* cur_op = *opi;
      if (is_movepp(cur_op) && cur_op->src(SRC1).is_reg())
        if (staging_preds.is_member(cur_op->src(SRC1).vr_num()))
          staging_preds += cur_op->dest(DEST1).vr_num();

      if (cur_op->predicated() && cur_op->src(PRED1).is_reg())
        if (staging_preds.is_member(cur_op->src(PRED1).vr_num()))
           break;
      
      opi++;
   }
   
   Op* first_real_op = *opi;
   assert (!is_switch (first_real_op) && !is_merge (first_real_op));
   Pred_cookie tmp_cq(*this, pqs->glb_src_guard(first_real_op, SRC1));
   return (tmp_cq) ;
}


/////////////////////////////////////////////////////////////////////////////


Pred_cookie::Pred_cookie()
: pqs(NULL), type(PQ_NULL), pred(NULL), pexpr(NULL)
{}


Pred_cookie::Pred_cookie(bool val)
: pqs(NULL), pred(NULL), pexpr(NULL)
{
   if (val == false) {
      type = PQ_FALSE ;
   } else {
      type = PQ_TRUE ;
   }
}

Pred_cookie::Pred_cookie(const Pred_jar& pj, Pred p)
: pqs(NULL),  pred(NULL), pexpr(NULL)
{
   if(p == pj.pqs->true_pred()) {
      type = PQ_TRUE ;
   }
   else {
      pqs = pj.pqs ;
      type = PQ_REF ;
      pred = new Pred(p) ;
      simplify() ;
   }
}

Pred_cookie::Pred_cookie(const Pred_cookie& pq)
:pqs(pq.pqs), type(pq.type), pred(NULL), pexpr(NULL)
{
   if (pq.pred) {
      pred = new Pred(*(pq.pred)) ;
   }
   if (pq.pexpr) {
      pexpr = new Pred_expr(*(pq.pexpr)) ;
   }
}

Pred_cookie::~Pred_cookie()
{
   if (pred) delete(pred) ;
   if (pexpr) delete(pexpr) ;   
}

Pred_cookie& Pred_cookie::operator=(const Pred_cookie& pq)
{
   // If PQS is changing, delete pexpr because none will
   // know which PQS it came from after copying
   if ((pexpr) && (pqs != pq.pqs)) {
      delete (pexpr) ;
      pexpr = NULL ;
   }
   // copy fields without deleteing anything
   switch (pq.type) {
      case PQ_NULL:
      case PQ_TRUE:
      case PQ_FALSE:
        break ;
      case PQ_REF:
        if (pred) {
	   *pred = *(pq.pred) ;
	}
	else {
	   pred = new Pred(*(pq.pred)) ;
	}
      break ;
      case PQ_EXPR:
        if (pexpr) {
	   if (pqs == pq.pqs) {
	      *pexpr = *(pq.pexpr) ;
	   }
	   else {
	      // control should never reach here. pexpr must have been deleted
	      assert(0) ; 
	   }
	}
	else {
	   pexpr = new Pred_expr(*(pq.pexpr)) ;
	}
      break ;
      default:
        assert(0) ;
   }
   type = pq.type ;
   pqs = pq.pqs ;
   return(*this) ;
}

bool Pred_cookie::operator==(const Pred_cookie& pq) const
{
   if ((type == pq.type) && ((type == PQ_TRUE) || (type == PQ_FALSE))) return true ;
   if ((pqs == pq.pqs) && (type == pq.type)) {
      if ((type == PQ_REF) && (*pred == *(pq.pred))) return true ;
      if ((type == PQ_EXPR) && (*pexpr == *(pq.pexpr))) return true ;
   }
   return false ;
}

bool Pred_cookie::operator!=(const Pred_cookie& pq) const
{
   if (*this == pq) return false ;
   else return true ;
}

//
// Is *this a subset of pq
//
bool Pred_cookie::is_subset(const Pred_cookie& pq) const
{
   switch (type) {
   case PQ_FALSE: 
      {
         return true ;
      }
      break;
   case PQ_TRUE:
      {
         if (pq.type == PQ_TRUE) return true;
         else return false;
      }
      break ;
   case PQ_REF: 
      {
         switch(pq.type) {
         case PQ_FALSE:
            {
               return is_false();
            }
            break ;
         case PQ_TRUE:
            {
               return true;
            }
            break ;
         case PQ_REF:
            {
               assert(pqs == pq.pqs) ;
               return (pqs->is_subset(*pred, *(pq.pred))) ;
            }
            break ;
         case PQ_EXPR:
            {
               assert(pqs == pq.pqs) ;
               return (pqs->is_subset(*pred, *(pq.pexpr))) ;
            }
            break ;
         default:
            El_punt("Second operand of is_subset() is NULL");
            return false ;
         }
      }
      break ;
   case PQ_EXPR: 
      {
         switch(pq.type) {
         case PQ_FALSE:
            {
               return is_false();
            }
            break ;
         case PQ_TRUE:
            {
               return true;
            }
            break ;
         case PQ_REF:
            {
	      Pred_cookie px(pq);
	      px.promote_to_expression();

	      assert(pqs == pq.pqs) ;
	      return (pqs->is_subset(*pexpr, *(px.pexpr)));
	    }
            break ;
         case PQ_EXPR:
            {
               assert(pqs == pq.pqs) ;
               return (pqs->is_disjoint(*pexpr, *(pq.pexpr))) ;
            }
            break ;
         default:
            El_punt("Second operand of is_subset() is NULL");
            return false ;
	    }
      }
      break ;
   default :
      El_punt("First operand of is_subset() is NULL");
      return false ;
   }
}
//
// Are *this and pq disjoint?
//

bool Pred_cookie::is_disjoint(const Pred_cookie& pq) const
{
   switch (type) {
   case PQ_FALSE: 
      {
         return true ;
      }
      break;
   case PQ_TRUE:
      {
         if (pq.type == PQ_FALSE) return true;
         else return false;
      }
      break ;
   case PQ_REF:
      {
         switch (pq.type) {
         case PQ_FALSE:
            {
               return true ;
            }
            break ;
         case PQ_TRUE:
            {
               return false ;
            }
            break ;
         case PQ_REF:
            {
               assert(pqs == pq.pqs) ;
               return (pqs->is_disjoint(*pred, *(pq.pred))) ;
            }
            break ;
         case PQ_EXPR:
            {
               assert(pqs == pq.pqs) ;
               return (pqs->is_disjoint(*pred,*(pq.pexpr))) ;
            }
            break ;
         default:
            El_punt("Second operand of is_disjoint() is NULL");
            return false ;
         }
      }
   case PQ_EXPR:
      {
         switch (pq.type) {
         case PQ_FALSE:
            {
               return true ;
            }
            break ;
         case PQ_TRUE:
            {
               return false ;
            }
            break ;
         case PQ_REF:
            {
               assert(pqs == pq.pqs) ;
               return (pqs->is_disjoint(*(pq.pred), *pexpr)) ;
            }
            break ;
         case PQ_EXPR:
            {
               assert(pqs == pq.pqs) ;
               return (pqs->is_disjoint(*pexpr, *(pq.pexpr))) ;
            }
            break ;
         default:
            El_punt("Second operand of is_disjoint() is NULL");
            return false ;
         }
      }
      break;
   default:
      El_punt("First operand of is_disjoint() is NULL");
      return false ;
   }
}

Pred_cookie& Pred_cookie::lub_sum(const Pred_cookie& pq)
{
   switch (type) {
   case PQ_NULL:
      El_punt("First operand of lub_sum() is NULL");
      break ;
   case PQ_FALSE:
      operator=(pq) ;
      break ;
   case PQ_TRUE:
      break;   
   case PQ_REF:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of lub_sum() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_true() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            promote_to_expression() ;
            pqs->lub_sum(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a lub_sum() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
         break ;
      }
   case PQ_EXPR:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of lub_sum() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_true() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            pqs->lub_sum(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a lub_sum() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
      }
      break;
      
   default:
      assert(0) ;
   }
   simplify() ;
   return (*this) ;
}

Pred_cookie& Pred_cookie::glb_sum(const Pred_cookie& pq)
{
   switch (type) {
   case PQ_NULL:
      El_punt("First operand of glb_sum() is NULL");
      break ;
   case PQ_FALSE:
      operator=(pq) ;
      break ;
   case PQ_TRUE:
      break;   
   case PQ_REF:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of glb_sum() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_true() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            promote_to_expression() ;
            pqs->glb_sum(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a glb_sum() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
         break ;
      }
   case PQ_EXPR:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of glb_sum() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_true() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            pqs->glb_sum(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a glb_sum() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
      }
      break;
      
   default:
      assert(0) ;
   }
   simplify() ;
   return (*this) ;
}

Pred_cookie& Pred_cookie::lub_diff(const Pred_cookie& pq)
{
   switch (type) {
   case PQ_NULL:
      El_punt("First operand of lub_diff() is NULL");
      break ;
   case PQ_FALSE:
      break ;
   case PQ_TRUE:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of lub_diff() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_false() ;
            break ;
         case PQ_REF:
            promote_to_true_expression(pq.pqs) ;
            pqs->lub_diff(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a lub_diff() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
      }
      break;   
   case PQ_REF:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of lub_diff() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_false() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            promote_to_expression() ;
            pqs->lub_diff(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a lub_diff() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
         break ;
      }
   case PQ_EXPR:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of lub_diff() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_false() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            pqs->lub_diff(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a lub_diff() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
         break ;
      }
      break;
      
   default:
      assert(0) ;
   }
   simplify() ;
   return (*this) ;
}

Pred_cookie& Pred_cookie::glb_diff(const Pred_cookie& pq)
{
   switch (type) {
   case PQ_NULL:
      El_punt("First operand of glb_diff() is NULL");
      break ;
   case PQ_FALSE:
      break ;
   case PQ_TRUE:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of glb_diff() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_false() ;
            break ;
         case PQ_REF:
            promote_to_true_expression(pq.pqs) ;
            pqs->glb_diff(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a glb_diff() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
      }
      break;   
   case PQ_REF:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of glb_diff() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_false() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            promote_to_expression() ;
            pqs->glb_diff(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a glb_diff() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
         break ;
      }
   case PQ_EXPR:
      {
         switch (pq.type) {
         case PQ_NULL:
            El_punt("Second operand of glb_diff() is NULL");
            break ;
         case PQ_FALSE:
            break ;
         case PQ_TRUE:
            set_to_false() ;
            break ;
         case PQ_REF:
            assert(pqs == pq.pqs) ;
            pqs->glb_diff(*pexpr, *(pq.pred)) ;
            break ;
         case PQ_EXPR:
            El_punt("PQS cannot handle a glb_diff() where second operand is a predicate expression") ;
            break ;
         default:
            assert(0) ;
         }
         break ;
      }
      break;
      
   default:
      assert(0) ;
   }
   simplify() ;
   return (*this) ;
}

void Pred_cookie::promote_to_true_expression(PQS* pqs_ptr)
{
   if (type == PQ_TRUE) {
      if (pexpr) {
	 if (pqs == pqs_ptr) {
	    pqs->set_expr_to_true(*pexpr) ;
	 }
	 else {
	    delete pexpr ;
	    pexpr = new Pred_expr(pqs_ptr->true_expr()) ;
	 }
      }
      else {
	 pexpr = new Pred_expr(pqs_ptr->true_expr()) ;
      }
      pqs = pqs_ptr ;
      type = PQ_EXPR ;
   }
   else {
      assert(0) ;
   }
}

void Pred_cookie::promote_to_false_expression(PQS* pqs_ptr)
{
   if (type == PQ_FALSE) {
      if (pexpr) {
	 if (pqs == pqs_ptr) {
	    pqs->set_expr_to_false(*pexpr) ;
	 }
	 else {
	    delete pexpr ;
	    pexpr = new Pred_expr(pqs_ptr->false_expr()) ;
	 }
      }
      else {
	 pexpr = new Pred_expr(pqs_ptr->false_expr()) ;
      }
      pqs = pqs_ptr ;
      type = PQ_EXPR ;
   }
   else {
      assert(0) ;
   }
}
void Pred_cookie::promote_to_expression()
{
   if (type == PQ_REF) {
      if (pexpr) {
	 pqs->set_expr_to_false(*pexpr) ;
      }
      else {
	 pexpr = new Pred_expr(pqs->false_expr()) ;
      }
      pqs->lub_sum(*pexpr, *pred) ;
      type = PQ_EXPR ;
   }
   else {
      assert(0) ;
   }
}

void Pred_cookie::promote_to_pred(PQS* pqs_ptr)
{
   if ((pexpr) && (pqs != pqs_ptr)) {
      delete (pexpr) ;
      pexpr = NULL ;
   }
   pqs = pqs_ptr ;
   if (type == PQ_TRUE) {
      if (pred) {
	 *pred = pqs->true_pred() ;
      }
      else {
	 pred = new Pred(pqs->true_pred()) ;
      }
      type = PQ_REF ;
   }
   else if (type == PQ_FALSE) {
      if (pred) {
	 *pred = pqs->true_pred() ;
      }
      else {
	 pred = new Pred(pqs->false_pred()) ;
      }
      type = PQ_REF ;
   }
   else {
      assert (0) ;
   }
}


void Pred_cookie::simplify() 
{
   if ((type == PQ_REF) || (type == PQ_EXPR)) {
      if (is_true()) {
	 set_to_true() ;
      }
      else if (is_false()) {
	 set_to_false() ;
      }
      else if (type == PQ_EXPR) {
	 Pred lub_pred = pqs->lub(*pexpr) ;
	 Pred glb_pred = pqs->glb(*pexpr) ;
	 if (lub_pred == glb_pred) {
	    type = PQ_REF ;
	    if (pred) {
	       *pred = lub_pred ;
	    }
	    else {
	       pred = new Pred(lub_pred) ;
	    }
	 }
      }
   }
}

void Pred_cookie::set_to_true()
{
   switch (type) {
   case PQ_NULL:
   case PQ_TRUE:
   case PQ_FALSE:
   case PQ_REF:
   case PQ_EXPR:
      type = PQ_TRUE ;
      break ;
   default:
      assert(0) ;
   }
}

void Pred_cookie::set_to_false()
{
   switch (type) {
   case PQ_NULL:
   case PQ_TRUE:
   case PQ_FALSE:
   case PQ_REF:
   case PQ_EXPR:
      type = PQ_FALSE ;
      break ;
   default:
      assert(0) ;
   }
}

void Pred_cookie::reset_to_true()
{
   switch (type) {
   case PQ_NULL:
   case PQ_TRUE:
   case PQ_FALSE:
   case PQ_REF:
   case PQ_EXPR:
      type = PQ_TRUE ;
      pqs = NULL ;
      if (pred) delete pred ;
      pred = NULL ;
      if (pexpr) delete pexpr ;
      pexpr = NULL ;
      break ;
   default:
      assert(0) ;
   }
}

void Pred_cookie::reset_to_false()
{
   switch (type) {
   case PQ_NULL:
   case PQ_TRUE:
   case PQ_FALSE:
   case PQ_REF:
   case PQ_EXPR:
      type = PQ_FALSE ;
      pqs = NULL ;
      if (pred) delete pred ;
      pred = NULL ;
      if (pexpr) delete pexpr ;
      pexpr = NULL ;
      break ;
   default:
      assert(0) ;
   }
}

bool Pred_cookie::is_true() const
{
   switch (type) {
   case PQ_NULL:
      assert(0) ;
      break ;
   case PQ_FALSE:
      return false ;
   case PQ_TRUE:
      return true ;
   case PQ_REF:
      return (*pred == pqs->true_pred()) ;
   case PQ_EXPR:
      // this can also be pred == pqs->true_expr()
      return (pqs->is_true(*pexpr)) ; 
   default:
      assert(0) ;
   }
   return false ;
}

bool Pred_cookie::is_false() const
{
   switch (type) {
   case PQ_NULL:
      assert(0) ;
      break ;
   case PQ_FALSE:
      return true ;
   case PQ_TRUE:
      return false ;
   case PQ_REF:
      return (*pred == pqs->false_pred()) ;
   case PQ_EXPR:
      // this can also be pred == pqs->false_expr()
      return (pqs->is_false(*pexpr)) ;
   default:
      assert(0) ;
   }
   return false ;
}

void Pred_cookie::print() {
  cout << *this << endl;
}

ostream&
operator << (ostream& os, const Pred_cookie& p)
{
  switch (p.type) {
  case PQ_NULL:
    os << "NULL" << flush; break;
  case PQ_FALSE:
    os << "false" << flush; break;
  case PQ_TRUE:
    os << "true" << flush; break;
  case PQ_REF:
    os << "Pref<" << *(p.pred) << ">" << flush; break;
  case PQ_EXPR:
    os << "Pexpr<" << *(p.pexpr) << ">" << flush; break;
  default:
    assert(0);
  }
  return os;
}

