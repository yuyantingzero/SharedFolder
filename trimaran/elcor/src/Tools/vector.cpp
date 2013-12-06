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
//      File:           vector.cpp
//      Authors:        nate clark
//      Created:        May 2003
//      Description:    Vector support functions
//
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "vector.h"

#if 0

template<class Item>
void shrink_vector(Vector<Item>& vec, Bitvector& mask)
{
  assert(mask.size() == vec.size()) ;
  int i = 0;
  for(typename Vector<Item>::iterator it = vec.begin(); 
      it != vec.end(); it++, i++) {
    if(mask.bit(i))
      it = vec.erase(it);
  }
}

/* -------------------- << for vectors ------------------------------------- */
template<class Item>
ostream& operator << (ostream& os, Vector<Item>& v)
{
  os << "- Vector = {" << endl;
  if (v.size() == 0) {
    os << "NULL VECTOR" << endl;
  }
  for (unsigned i=0; i< v.size(); i++) {
    os << i << ": " << v[i] << endl;
  }
  os << "}" << endl;
  return os;
}

#endif


template<class Item>
Vector<Item>::Vector()
{
  rep_dim = 0;
  rep_vect = NULL;
}

template<class Item>
Vector<Item>::Vector(int dim)
{
  rep_dim = dim;
  if (rep_dim > 0) {
    rep_vect = new Item[rep_dim] ;
    assert(rep_vect != NULL);
  } else {
    rep_vect = NULL;
  }
}

template<class Item>
Vector<Item>::Vector(int dim, Item initial_val)
{
  rep_dim = dim;
 if(rep_dim > 0)  {
    rep_vect = new Item[rep_dim] ;
    assert(rep_vect != NULL);
    for (int i=0; i<rep_dim; i++) {
      rep_vect[i] = initial_val;
    }
  } else {
    rep_vect = NULL;
  }
}

template<class Item>
Vector<Item>::Vector(const Vector<Item>& v ) // copy constructor
{
  int i;
  rep_dim=v.rep_dim;
  if(rep_dim > 0)  {
    rep_vect = new Item[rep_dim] ;
    assert(rep_vect!=NULL);
  } else {
    rep_vect = NULL;
  }
  for(i=0;i<rep_dim;i++) {
    rep_vect[i] = v.rep_vect[i];
  }
}


template<class Item>
Vector<Item>::~Vector(){
   delete [] rep_vect;
}

template<class Item> Vector<Item>&
Vector<Item>::clear() {
  delete [] rep_vect;
  rep_vect=NULL ;
  rep_dim = 0 ; 
  return *this;
}

// resize Vector, copy old elements where possible
template<class Item> Vector<Item>&
Vector<Item>::resize(int dim) {
  if(dim == rep_dim)
    return *this;

  Item * new_vect = new Item[dim];

  // copy existing elements into new Vector where possible
  for (int i=0; i<ELCOR_MIN(dim, rep_dim); i++) {
    new_vect[i] = rep_vect[i];
  }
  delete [] rep_vect;
  rep_vect = new_vect;
  rep_dim = dim;
  return *this;
}

// resize Vector, copy old elements where possible, initializing remaining
// elements.
template<class Item> Vector<Item>&
Vector<Item>::resize(int dim, const Item& init_val) {
  if(dim == rep_dim)
    return *this;

  Item * new_vect = new Item[dim];

  // copy existing elements into new Vector where possible
  int i ;
  for (i=0; i<ELCOR_MIN(dim, rep_dim); i++) {
    new_vect[i] = rep_vect[i];
  }

  // if new Vector is larger than old, initialize remaining elements
  for (i=rep_dim; i<dim; i++) {
    new_vect[i] = init_val;
  }
  delete [] rep_vect;
  rep_vect = new_vect;
  rep_dim = dim;
  return *this;
}

template<class Item>
void  Vector<Item>::reshape(int dim)
{
  if(dim != rep_dim) {
    delete [] rep_vect;
    rep_vect = NULL;
    rep_dim = dim;
    if (rep_dim > 0)  {
      rep_vect = new Item[rep_dim];
      assert(rep_vect != NULL);
    }
  }
}

template<class Item>
void  Vector<Item>::reshape(int dim, const Item& initial_value)
{
  int i;
  if(dim != rep_dim) {
    delete [] rep_vect;
    rep_vect = NULL;
    rep_dim = dim;
    if (rep_dim > 0)  {
      rep_vect = new Item[rep_dim];
      assert(rep_vect != NULL);
    }
  }
  if (rep_dim > 0)  {
    for (i=0; i<rep_dim; i++) {
      rep_vect[i] = initial_value;
    }
  }
}

template<class Item>
void  Vector<Item>::shrink(Bitvector& mask)
{
   assert((int)mask.size() == rep_dim) ;
   int dim = mask.ones_count() ;
   Item * new_vect = new Item[dim];

   // copy  elements under mask
   int count = 0 ;
   for (int i=0; i < rep_dim ; i++) {
      if (mask.bit(i)) {
         new_vect[count] = rep_vect[i];
         count++ ;
      }
   }
   delete [] rep_vect;
   rep_vect = new_vect;
   rep_dim = dim;
}


template<class Item>
void  Vector<Item>::append(const Vector<Item>& v)
{
  int old_dim = rep_dim;
  resize(rep_dim + v.rep_dim);
  for(int i = 0; i < v.rep_dim; i++) {
    rep_vect[i + old_dim] = v[i];
  }
}

template<class Item>
Vector<Item>& Vector<Item>::operator= (const Vector<Item>& v){ /* Assignment */
  int i;
  if (this == &v) return *this;
  if(rep_dim != v.rep_dim) {
    delete [] rep_vect;
    rep_dim=v.rep_dim;
    if (rep_dim>0)  {
      rep_vect = new Item[rep_dim];
      assert(rep_vect!=NULL); }
    else
      rep_vect=NULL;
  }
  for (i=0;i<rep_dim;i++) rep_vect[i]=v.rep_vect[i];
  return *this;
}

template <class Item>
bool Vector<Item>::operator==(const Vector<Item>& v) const {
        if (rep_dim != v.rep_dim) return false;
        for (int i = 0; i < rep_dim; i++)
          if (!(rep_vect[i] == v.rep_vect[i])) return false;
        return true;
}

template <class Item>
bool Vector<Item>::operator!=(const Vector<Item>& v) const {
  return (!operator==(v));
}

template<class Item>
Vector<Item>& Vector<Item>::set(const Item d){ /*sets all elements to value d*/
  int i;
  assert (rep_dim>=0);
  for(i=0;i<rep_dim;i++) {
    (*this)[i] = d;
  }
  return(*this);
}

template <class Item>
void Vector<Item>::assign(int n, const Item& x)
{
  reshape(n, x);
}


template <class Item>
int Vector<Item>::find(const Item& x) const
{
  for(int i = 0; i < rep_dim; i++) {
    if(rep_vect[i] == x)
      return i;
  }
  return -1;
}


template<class Item>
ostream& operator << (ostream& os, Vector<Item>& v)
{
  os << "- Vector = {" << endl;
  if (v.size() == 0) {
    os << "NULL VECTOR" << endl;
  }
  for (unsigned i=0; i< v.size(); i++) {
    os << i << ":" << v[i] << " ";
  }
  os << endl << "}" << endl;
  return os;
}

template <class Item>
void shrink_vector(Vector<Item>& vec, Bitvector& mask)
{
  vec.shrink(mask);
}

