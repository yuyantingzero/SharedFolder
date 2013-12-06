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




///////////////////////////////////////////////////////////////////////////
//
//      File:          pqs-1bit-triv.cpp
//      Authors:       Richard Johnson 
//      Created:       March 1995 
//      Description:   1-bit trivial predicate query system for hyperblocks.
//
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// The 1bit-triv pqs implementation has the following goals/features:
//   o robustness - runs on *any* predicated code.
//   o simplicity - works with standard bitvector data flow analysis.
//   o efficiency - doesn't build any data structure or perform any analysis.
//       - understands that true is a superset of all predicates.
//       - predicate expressions are single bits (true or false).
//
/////////////////////////////////////////////////////////////////////////////

#include "pqs-1bit-triv.h"
#include "opcode_properties.h"
#include "dbg.h"

//-----------------------------------------------------------------------------
PQS_1bit_triv::PQS_1bit_triv (Compound_region* r)
{
  // allocate names for true, false
  _sym_false = 0;
  _sym_true = 1;

  _false_pred.val = _sym_false;
  _true_pred.val = _sym_true;

  _false_expr.base = new Pred_expr_1bit();
  ((Pred_expr_1bit*) _false_expr.base)->bt = false;

  _true_expr.base = new Pred_expr_1bit();
  ((Pred_expr_1bit*) _true_expr.base)->bt = true;
}

PQS_1bit_triv::~PQS_1bit_triv (void)
{
}

Pred
PQS_1bit_triv::false_pred (void) const
{
  return _false_pred;
}

Pred
PQS_1bit_triv::true_pred (void) const
{
  return _true_pred;
}

Pred_expr
PQS_1bit_triv::false_expr (void) const
{
  return _false_expr;
}

Pred_expr
PQS_1bit_triv::true_expr (void) const
{
  return _true_expr;
}

void
PQS_1bit_triv::set_expr_to_false (Pred_expr &px) const
{
  px = _false_expr;
}

void
PQS_1bit_triv::set_expr_to_true (Pred_expr &px) const
{
  px = _true_expr;
}

//-----------------------------------------------------------------------------
/*
 *  ALU       x <- op(y) if q          w(x) = Q       r(y) = Q   r(q) = T
 *  cmpp U    p <- c^q   if true       w(p) = T       r(c) = T   r(q) = T
 *  cmpp C    p <- c     if q          w(p) = Q       r(c) = Q   r(q) = T
 *  cmpp O    p <- 1     if c^q        w(p) = C^Q     r(c) = Q   r(q) = T
 *  cmpp A    p <- 0     if (!c)^q     w(p) = (!C)^Q  r(c) = Q   r(q) = T
 *  merge     x <- y                   w(x) = T       r(y) = T
 *  switch    x <- y
 *  branch    p <- br(r) if q          w(p) =         r(r) = Q   r(q) = T
 *
 *  lub: everything goes to true
 *  glb: trues remain true, everything else goes to false
 *
 *  special cases: glb can often return true when guard is true.
 */
Pred
PQS_1bit_triv::lub_src_guard (Op* op, Port_num pn) const
{
  return _true_pred;
}

Pred
PQS_1bit_triv::lub_dest_guard (Op* op, Port_num pn) const
{
  return _true_pred;
}

Pred
PQS_1bit_triv::lub_branch_pred (Op* op) const
{
  return _true_pred;
}

Pred
PQS_1bit_triv::glb_src_guard (Op* op, Port_num pn) const
{
   if (pn == PRED1) {
      assert (op->predicated());
      return _true_pred;
   }
   // handle special cases when guarding predicate is true
   if (!op->predicated() || (op->src(PRED1)).is_predicate_true()) {
      return _true_pred;
   }
   return _false_pred;
}

Pred
PQS_1bit_triv::glb_dest_guard (Op* op, Port_num pn) const
{
  // handle special cases when guarding predicate is true
  if (!op->predicated() || (op->src(PRED1)).is_predicate_true()) {
    if (is_O_pred(op, pn)) return false_pred();
    else if (is_A_pred(op, pn)) return false_pred();
    else return _true_pred;
  }
  return _false_pred;
}

Pred
PQS_1bit_triv::glb_branch_pred (Op* op) const
{
  return _false_pred;
}

// is zz a subset of pp ?
bool
PQS_1bit_triv::is_subset(const Pred& zz, const Pred& pp) const
{
  return is_subset(zz.val, pp.val);
}

// is zz a subset of union of sets in px ?
//  ( assumes px is in reduced form)
bool
PQS_1bit_triv::is_subset(const Pred& zz, const Pred_expr& px) const
{
  Pred_expr_1bit* pxb = (Pred_expr_1bit*) px.base;
  return is_subset(zz, (pxb->bt ? _true_pred : _false_pred));
}

// is px a subset of qx ?
bool
PQS_1bit_triv::is_subset(const Pred_expr& px, const Pred_expr& qx) const
{
  Pred_expr_1bit* pxb = (Pred_expr_1bit*) px.base;
  Pred_expr_1bit* qxb = (Pred_expr_1bit*) qx.base;
  return is_subset((pxb->bt ? _true_pred : _false_pred),
		     (qxb->bt ? _true_pred : _false_pred));
}

// are pp and qq disjoint ?
bool
PQS_1bit_triv::is_disjoint(const Pred& pp, const Pred& qq) const
{
  return is_disjoint(pp.val, qq.val);
}

// is zz disjoint from expression px ?
//   ( assumes px is in reduced form )
bool
PQS_1bit_triv::is_disjoint(const Pred& zz, const Pred_expr& px) const
{
  Pred_expr_1bit* pxb = (Pred_expr_1bit*) px.base;
  return is_disjoint(zz, (pxb->bt ? _true_pred : _false_pred));
}

// are px and qx disjoint expressions ?
bool
PQS_1bit_triv::is_disjoint(const Pred_expr& px, const Pred_expr& qx) const
{
  Pred_expr_1bit* pxb = (Pred_expr_1bit*) px.base;
  Pred_expr_1bit* qxb = (Pred_expr_1bit*) qx.base;
  return is_disjoint((pxb->bt ? _true_pred : _false_pred),
		     (qxb->bt ? _true_pred : _false_pred));
}

// px = lub (px + zz)
//   . px + false -> px
//   . px + true  -> true_expr
//   . else       -> error (no other possible values for zz)
void
PQS_1bit_triv::lub_sum (Pred_expr& px, const Pred& zz) const
{
  if (zz == _false_pred) {
    ; // do nothing
  } else if (zz == _true_pred) {
    px = _true_expr;
  } else {
    El_punt("sanity failure: preds are all true or false.");
  }
}

// px = lub (px - zz)
//   . px - false -> px
//   . px - true  -> false_expr
//   . else       -> error (no other possible values for zz)
void
PQS_1bit_triv::lub_diff (Pred_expr& px, const Pred& zz) const
{
  if (zz == _false_pred) {
    ; // do nothing
  } else if (zz == _true_pred) {
    px = _false_expr;
  } else {
    El_punt("sanity failure: preds are all true or false.");
  }
}

// px = glb (px + zz)
//   . px + false -> px
//   . px + true  -> true_expr
//   . else       -> error (no other possible values for zz)
void
PQS_1bit_triv::glb_sum (Pred_expr& px, const Pred& zz) const
{
  if (zz == _false_pred) {
    ; // do nothing
  } else if (zz == _true_pred) {
    px = _true_expr;
  } else {
    El_punt("sanity failure: preds are all true or false.");
  }
}

// px = glb (px - zz)
//   . px - false -> px
//   . px - true  -> false_expr
//   . else       -> error (no other possible values for zz)
void
PQS_1bit_triv::glb_diff (Pred_expr& px, const Pred& zz) const
{
  if (zz == _false_pred) {
    ; // do nothing
  } else if (zz == _true_pred) {
    px = _false_expr;
  } else {
    El_punt("sanity failure: preds are all true or false.");
  }
}

// p = lub (px)
Pred
PQS_1bit_triv::lub (const Pred_expr& px) const
{
  if (px == _false_expr) {
    return _false_pred;
  } else {
    return _true_pred;
  }
}

// p = glb (px)
Pred
PQS_1bit_triv::glb (const Pred_expr& px) const
{
  if (px == _true_expr) {
    return _true_pred;
  } else {
    return _false_pred;
  }
}

bool
PQS_1bit_triv::is_false (const Pred_expr& px) const
{
  return (px == _false_expr);
}

bool
PQS_1bit_triv::is_true (const Pred_expr& px) const
{
  return (px == _true_expr);
}

//-----------------------------------------------------------------------------
// is z a subset of p ?
//  - return true if z is false or p is true, else assume false.
bool
PQS_1bit_triv::is_subset(int z, int p) const
{
  if (z == _sym_false || p == _sym_true) {
    return true;
  } else {
    return false;
  }
}

// are p and q disjoint ?
//  - true if either p or q is false, else true
bool
PQS_1bit_triv::is_disjoint(int p, int q) const
{
  if (p == _sym_false || q == _sym_false) {
    return true;
  } else {
    return false;
  }
}

void
PQS_1bit_triv::print(ostream& os) const
{
  os << "- PQS_1bit_triv = <" << endl;
  os << " >" << endl;
}

// end of pqs-1bit-triv.cpp
