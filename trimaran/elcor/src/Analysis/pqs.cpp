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
//      File:           pqs.cpp
//      Authors:        Richard Johnson
//      Created:        March 1995
//      Description:    Predicate query system interface for hyperblocks.
//
///////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <stdlib.h>
#include "region.h"
#include "iterators.h"
#include "tuples.h"
#include "attributes.h"
#include "opcode_properties.h"
#include "string_class.h"
#include "hash_map.h"
#include "hash_functions.h"
#include "pqs.h"
#include "dfa.h"
#include "dbg.h"

using namespace std;

//-----------------------------------------------------------------------------
Pred::Pred (void)
{
  val = -1;
}

Pred::Pred (int i)
{
  val = i;
}

Pred::~Pred (void)
{
}

Pred&
Pred::operator=(const Pred& p)
{
  val = p.val;
  return *this;
}

bool
Pred::operator==(const Pred& p) const
{
  return (val == p.val);
}

bool
Pred::operator!=(const Pred& p) const
{
  return (val != p.val);
}

ostream&
operator << (ostream& os, const Pred& p)
{
  os << p.val << flush; 
  return os;
}

//-----------------------------------------------------------------------------
Pred_expr::Pred_expr (void)
{
  base = NULL;
}

Pred_expr::Pred_expr (const Pred_expr& qx)
{
  if (qx.base == NULL) {
    if (base != NULL) {
      delete base;
    }
    base = NULL;
  } else {
    base = qx.base->clone();
  }
}

Pred_expr::~Pred_expr (void)
{
  if (base != NULL) {
    delete base;
  }
}

Pred_expr&
Pred_expr::operator=(const Pred_expr& px)
{
  if (base == NULL) {
    base = px.base->clone();
  } else {
    *base = *px.base;
  }
  return *this;
}

bool
Pred_expr::operator==(const Pred_expr& px) const
{
  return (*base == *px.base);
}

bool
Pred_expr::operator!=(const Pred_expr& px) const
{
  return (!operator==(px));
}

ostream&
operator << (ostream& os, const Pred_expr& p)
{
  (p.base)->print(os);
  return os;
}

//-----------------------------------------------------------------------------
Pred_expr_1bit::Pred_expr_1bit (void)
{
  bt = 0;
}

Pred_expr_1bit::Pred_expr_1bit (const Pred_expr_1bit& pxb)
{
  bt = pxb.bt;
}

Pred_expr_1bit::~Pred_expr_1bit (void)
{
}

Pred_expr_base*
Pred_expr_1bit::clone(void)
{
  return new Pred_expr_1bit(*this);
}

Pred_expr_base&
Pred_expr_1bit::operator=(const Pred_expr_base& pxb)
{
  bt = ((Pred_expr_1bit&)pxb).bt;
  return *this;
}

bool
Pred_expr_1bit::operator==(const Pred_expr_base& pxb) const
{
  return (bt == ((Pred_expr_1bit&)pxb).bt);
}

bool
Pred_expr_1bit::operator!=(const Pred_expr_base& pxb) const
{
  return (!operator==(pxb));
}

void
Pred_expr_1bit::print(ostream& os) const
{
  os << bt << flush;
}

ostream&
operator << (ostream& os, const Pred_expr_1bit& pxb)
{
  os << pxb.bt << flush; 
  return os;
}

//-----------------------------------------------------------------------------
Pred_expr_1sym::Pred_expr_1sym (void) : pp()
{
}

Pred_expr_1sym::Pred_expr_1sym (const Pred_expr_1sym& pxb)
{
  pp = pxb.pp;
}

Pred_expr_1sym::~Pred_expr_1sym (void)
{
}

Pred_expr_base*
Pred_expr_1sym::clone(void)
{
  return new Pred_expr_1sym(*this);
}

Pred_expr_base&
Pred_expr_1sym::operator=(const Pred_expr_base& pxb)
{
  pp = ((Pred_expr_1sym&)pxb).pp;
  return *this;
}

bool
Pred_expr_1sym::operator==(const Pred_expr_base& pxb) const
{
  return (pp == ((Pred_expr_1sym&)pxb).pp);
}

bool
Pred_expr_1sym::operator!=(const Pred_expr_base& pxb) const
{
  return (!operator==(pxb));
}

void
Pred_expr_1sym::print(ostream& os) const
{
  os << pp << flush;
}

ostream&
operator << (ostream& os, const Pred_expr_1sym& pxb)
{
  os << pxb.pp << flush; 
  return os;
}

//-----------------------------------------------------------------------------
Pred_expr_1dnf::Pred_expr_1dnf (void)
{
}

Pred_expr_1dnf::Pred_expr_1dnf (const Pred_expr_1dnf& pxb)
{
  bv = pxb.bv;
}

Pred_expr_1dnf::~Pred_expr_1dnf (void)
{
}

Pred_expr_base*
Pred_expr_1dnf::clone(void)
{
  return new Pred_expr_1dnf(*this);
}

Pred_expr_base&
Pred_expr_1dnf::operator=(const Pred_expr_base& pxb)
{
  bv = ((Pred_expr_1dnf&)pxb).bv;
  return *this;
}

bool
Pred_expr_1dnf::operator==(const Pred_expr_base& pxb) const
{
  return (bv == ((Pred_expr_1dnf&)pxb).bv);
}

bool
Pred_expr_1dnf::operator!=(const Pred_expr_base& pxb) const
{
  return (!operator==(pxb));
}

void
Pred_expr_1dnf::print(ostream& os) const
{
  os << bv << flush;
#if 0
  os << "bits set: ";
  for (Bit_iterator bi(bv); bi != 0; bi++) {
      int p = *bi;
      os << p << " ";
  }
  os << flush;
#endif
}

ostream&
operator << (ostream& os, const Pred_expr_1dnf& pxb)
{
  os << pxb.bv << flush; 
  return os;
}

//-----------------------------------------------------------------------------
ostream&
operator << (ostream& os, const PQS& pqs)
{
  pqs.print(os);
  return os;
}

//
// Attribute implementation 
//
PQS_attrib::PQS_attrib()
   : pqs_ptr(NULL) 
{}

PQS_attrib::~PQS_attrib()
{
   delete pqs_ptr ;
}

Base_attribute* PQS_attrib::clone_attribute()
{
   El_punt("Cannot clone PQS.");
   return NULL;
}

//
// Attribute access functions 
//

PQS* get_pqs(Compound_region* r) {
   PQS_attrib* tmp_ptr = (PQS_attrib*) (r->attributes->get_attribute(LOCAL_PQS));
   if (tmp_ptr == NULL) {
      return (NULL) ;
   }
   return (tmp_ptr->pqs_ptr) ;
}

void set_pqs(Compound_region* r, PQS* pqs) {
   PQS_attrib* tmp_ptr = (PQS_attrib*) (r->attributes->get_attribute(LOCAL_PQS));
   if (tmp_ptr == NULL) {
      tmp_ptr = new PQS_attrib() ;
      tmp_ptr->pqs_ptr = NULL ;
      r->attributes->insert_attribute(LOCAL_PQS, tmp_ptr);
   }
   tmp_ptr->pqs_ptr = pqs ;
}

void remove_pqs(Compound_region* r) {
   r->attributes->remove_attribute(LOCAL_PQS) ;
}


// end of pqs.cpp
