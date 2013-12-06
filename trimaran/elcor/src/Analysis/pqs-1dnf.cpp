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
//      File:          pqs-1dnf.cpp
//      Authors:       Richard Johnson 
//      Created:       March 1995
//      Description:   1-dnf predicate query system for hyperblocks.
//
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// The 1-dnf pqs implementation has the following goals/features:
//   o robustness - runs on *any* predicated code.
//   o accuracy - produces good results on if-converted code.
//       - understands unconditional and OR-style cmpp ops produced
//         by if-conversion.
//       - yields conservative results for conditional and AND-style
//         cmpp code.
//
/////////////////////////////////////////////////////////////////////////////

#include "partition_graph_default.h"
#include "partition_graph_bv.h"
#include "pqs-1dnf.h"
#include "dbg.h"
#include "el_anal_init.h"

//-----------------------------------------------------------------------------
PQS_1dnf::PQS_1dnf (Compound_region* r) :
  pmap(r)
{
  if (dbg(pqs)) cdbg << " PQS_1dnf: region " << r->id() << endl;

  if (strcmp(El_anal_pg_type, "default") == 0) {
    pg = new PartitionGraph_default(&pmap);
  } else if (strcmp(El_anal_pg_type, "bv") == 0) {
    pg = new PartitionGraph_bv(&pmap);
  } else {
    El_punt("unknown partition graph type.");
  }

  _false_pred.val = pg->false_sym();
  _true_pred.val = pg->true_sym();

  _false_expr.base = new Pred_expr_1dnf();
  ((Pred_expr_1dnf*) _false_expr.base)->bv.resize(pg->sym_count());

  _true_expr.base = new Pred_expr_1dnf();
  ((Pred_expr_1dnf*)
   _true_expr.base)->bv.resize(pg->sym_count()).set_bit(_true_pred.val);
}

PQS_1dnf::~PQS_1dnf (void)
{
   delete(pg) ;
}

Pred
PQS_1dnf::false_pred (void) const
{
  return _false_pred;
}

Pred
PQS_1dnf::true_pred (void) const
{
  return _true_pred;
}

Pred_expr
PQS_1dnf::false_expr (void) const
{
  return _false_expr;
}

Pred_expr
PQS_1dnf::true_expr (void) const
{
  return _true_expr;
}

void
PQS_1dnf::set_expr_to_false (Pred_expr &px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  pxb->bv.clear();
}

void
PQS_1dnf::set_expr_to_true (Pred_expr &px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  pxb->bv.clear();
  pxb->bv.set_bit(_true_pred.val);
}

Pred
PQS_1dnf::lub_src_guard (Op* op, Port_num pn) const
{
  return Pred(pmap.src_guard_sym(op, pn));
}

Pred
PQS_1dnf::lub_dest_guard (Op* op, Port_num pn) const
{
  return Pred(pmap.dest_guard_sym(op, pn));
}

Pred
PQS_1dnf::lub_branch_pred (Op* op) const
{
  return Pred(pmap.branch_sym(op));
}

Pred
PQS_1dnf::glb_src_guard (Op* op, Port_num pn) const
{
  return Pred(pmap.src_guard_sym(op, pn));
}

Pred
PQS_1dnf::glb_dest_guard (Op* op, Port_num pn) const
{
  return Pred(pmap.dest_guard_sym(op, pn));
}

Pred
PQS_1dnf::glb_branch_pred (Op* op) const
{
  return Pred(pmap.branch_sym(op));
}

// is zz a subset of pp ?
bool
PQS_1dnf::is_subset(const Pred& zz, const Pred& pp) const
{
  return pg->is_subset(zz.val, pp.val);
}

// is zz a subset of union of sets in px ?
//  ( assumes px is in reduced form)
bool
PQS_1dnf::is_subset(const Pred& zz, const Pred_expr& px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  const Bitvector& bv = pxb->bv;

  for (Bit_iterator bi(bv); bi != 0; bi++) {
    int p = *(bi);

    if (pg->is_subset(zz.val, p)) {
      return true;
    }
  }
  return false;
}

// is px a subset of qx ?
bool
PQS_1dnf::is_subset(const Pred_expr& px, const Pred_expr& qx) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  const Bitvector& bv = pxb->bv;

  for (Bit_iterator bi(bv); bi != 0; bi++) {
    int p = *(bi);

    if (!is_subset(p, qx)) return false;
  }
  return true;
}

// are pp and qq disjoint ?
bool
PQS_1dnf::is_disjoint(const Pred& pp, const Pred& qq) const
{
  return pg->is_disjoint(pp.val, qq.val);
}

// is zz disjoint from expression px ?
bool
PQS_1dnf::is_disjoint(const Pred& zz, const Pred_expr& px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  const Bitvector& bv = pxb->bv;

  for (Bit_iterator bi(bv); bi != 0; bi++) {
    int p = *(bi);
    if (!pg->is_disjoint(zz.val, p)) {
      return false;
    }
  }
  return true;
}

// are px and qx disjoint expressions ?
bool
PQS_1dnf::is_disjoint(const Pred_expr& px, const Pred_expr& qx) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  Pred_expr_1dnf* qxb = (Pred_expr_1dnf*) qx.base;
  const Bitvector& bv1 = pxb->bv;
  const Bitvector& bv2 = qxb->bv;

  for (Bit_iterator bi1(bv1); bi1 != 0; bi1++) {
    int p = *(bi1);

    for (Bit_iterator bi2(bv2); bi2 != 0; bi2++) {
      int q = *(bi2);

      if (!pg->is_disjoint(p, q)) {
	return false;
      }
    }
  }
  return true;
}

// px = lub (px + zz)
void
PQS_1dnf::lub_sum (Pred_expr& px, const Pred& zz) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  Bitvector& bv = pxb->bv;

  pg->lub_sum (bv, zz.val); }

// px = lub (px - zz)
void
PQS_1dnf::lub_diff (Pred_expr& px, const Pred& zz) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  Bitvector& bv = pxb->bv;

  pg->lub_diff (bv, zz.val);
}

void
PQS_1dnf::glb_sum (Pred_expr& px, const Pred& zz) const
{
  // temporarily use lub sum as the glb sum -- Shail Aditya 09/04/97
  lub_sum(px, zz);
  //  El_punt("glb_sum not implemented.");
}

// px = glb (px - zz)
void
PQS_1dnf::glb_diff (Pred_expr& px, const Pred& zz) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  Bitvector& bv = pxb->bv;

  pg->glb_diff (bv, zz.val);
}

// p = lub (px)
Pred
PQS_1dnf::lub (const Pred_expr& px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  const Bitvector& bv = pxb->bv;

  if (bv.ones_count() == 1) {
    for (Bit_iterator bi(bv); bi != 0; bi++) {
      int p = *(bi);
      return p;
    }
  }
  return pg->lca(bv);
}

// p = glb (px)
Pred
PQS_1dnf::glb (const Pred_expr& px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  const Bitvector& bv = pxb->bv;

  if (bv.ones_count() == 1) {
    for (Bit_iterator bi(bv); bi != 0; bi++) {
      int p = *(bi);
      return p;
    }
  }
  return _false_pred; // conservative approx.
}

bool
PQS_1dnf::is_false (const Pred_expr& px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  if (pxb->bv.is_allclear() == true) return true;
  else return false ;
}

bool
PQS_1dnf::is_true (const Pred_expr& px) const
{
  Pred_expr_1dnf* pxb = (Pred_expr_1dnf*) px.base;
  if (pxb->bv.bit(_true_pred.val) == true) return true;
  else return false;
}

void
PQS_1dnf::print(ostream& os) const
{
  os << "- PQS-1dnf = <";
  os << " >" << endl;
}

// end of pqs-1dnf.cpp
