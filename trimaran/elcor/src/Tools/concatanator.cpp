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
//      File:           concatanator.cpp
//      Authors:        Sadun Anik 
//      Created:        August 1995
//      Description:    Templates for concatanating iterators
//
/////////////////////////////////////////////////////////////////////////////

#include "concatanator.h"


//////////////////////////////////////////////////////////////////////////
// Pair_concatanator class 
//////////////////////////////////////////////////////////////////////////


template<class A, class T, class U, class B>
Pair_concatanator<A, T, U, B>::Pair_concatanator()
   : iterator_count(0) 
{}

template<class A, class T, class U, class B>
Pair_concatanator<A, T, U, B>::Pair_concatanator(const B& obj)
   : it1(obj), it2(obj)
{
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
   }
}

template<class A, class T, class U, class B>
Pair_concatanator<A, T, U, B>::Pair_concatanator
       (const Pair_concatanator<A,T,U, B>& pc)
	  : iterator_count(pc.iterator_count), it1(pc.it1), it2(pc.it2)
{}

template<class A, class T, class U, class B>
void Pair_concatanator<A, T, U, B>::operator() (const B& obj)
{
   it1(obj) ;
   it2(obj) ;
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
   }
}

template<class A, class T, class U, class B>
Pair_concatanator<A, T, U, B>& Pair_concatanator<A, T, U, B>::operator=
       (const Pair_concatanator<A,T,U, B>& pc)
{
  iterator_count = pc.iterator_count ;
  it1 = pc.it1 ;
  it2 = pc.it2 ;
  return *this ;
}

template<class A, class T, class U, class B>
Pair_concatanator<A, T, U, B>::~Pair_concatanator()
{}

template<class A, class T, class U, class B>
bool Pair_concatanator<A, T, U, B>::operator==(const int i) const
{
   assert (i == 0) ;
   if (iterator_count == 1) return (it2 == i) ;
   else return(false) ;
}

template<class A, class T, class U, class B>
bool Pair_concatanator<A, T, U, B>::operator!=(const int i) const
{
   assert (i == 0) ;
   if (iterator_count == 1) return (it2 != i) ;
   else return(true) ;
}

template<class A, class T, class U, class B>
void  Pair_concatanator<A, T, U, B>::operator++()
{
   switch (iterator_count) {
    case 0:
      it1++ ;
      if(it1 == 0) {
	 iterator_count++ ;
      }
      break ;
    case 1:
      it2++ ;
      break ;
    default:
      assert(0) ;
   }
}

template<class A, class T, class U, class B>
void  Pair_concatanator<A, T, U, B>::operator++(int)
{
   operator++() ;
}

template<class A, class T, class U, class B>
A& Pair_concatanator<A, T, U, B>::operator*()
{
   switch (iterator_count) {
    case 0:
      return(*it1) ;
    default:
      return(*it2) ;
   }
}

//////////////////////////////////////////////////////////////////////////
// Triple_concatanator class 
//////////////////////////////////////////////////////////////////////////


template<class A, class T, class U, class V, class B>
Triple_concatanator<A,T,U,V,B>::Triple_concatanator()
   : iterator_count(0) 
{}

template<class A, class T, class U, class V, class B>
Triple_concatanator<A,T,U,V,B>::Triple_concatanator(const B& obj)
   : it1(obj), it2(obj), it3(obj)
{
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
      if (it2 == 0) {
	 iterator_count++ ;
      }
   }
}

template<class A, class T, class U, class V, class B>
Triple_concatanator<A,T,U,V,B>::Triple_concatanator
       (const Triple_concatanator<A,T,U,V,B>& pc)
	  : iterator_count(pc.iterator_count), it1(pc.it1),
	    it2(pc.it2), it3(pc.it3)
{}

template<class A, class T, class U, class V, class B>
void Triple_concatanator<A,T,U,V,B>::operator() (const B& obj)
{
   it1(obj) ;
   it2(obj) ;
   it3(obj) ;
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
      if (it2 == 0) {
	 iterator_count++ ;
      }
   }
}

template<class A, class T, class U, class V, class B>
Triple_concatanator<A,T,U,V,B>& Triple_concatanator<A,T,U,V,B>::operator=
       (const Triple_concatanator<A,T,U,V,B>& pc)
{
  iterator_count = pc.iterator_count ;
  it1 = pc.it1 ;
  it2 = pc.it2 ;
  it3 = pc.it3 ;
  return *this ;
}

template<class A, class T, class U, class V, class B>
Triple_concatanator<A,T,U,V,B>::~Triple_concatanator()
{}

template<class A, class T, class U, class V, class B>
bool Triple_concatanator<A,T,U,V,B>::operator==(const int i) const
{
   assert (i == 0) ;
   if (iterator_count == 2) return (it3 == i) ;
   else return(false) ;
}

template<class A, class T, class U, class V, class B>
bool Triple_concatanator<A,T,U,V,B>::operator!=(const int i) const
{
   assert (i == 0) ;
   if (iterator_count == 2) return (it3 != i) ;
   else return(true) ;
}

template<class A, class T, class U, class V, class B>
void  Triple_concatanator<A,T,U,V,B>::operator++()
{
   switch (iterator_count) {
    case 0:
      it1++ ;
      if(it1 == 0) {
	 iterator_count++ ;
	 if(it2 == 0) {
	    iterator_count++ ;
	 }
      }
      break ;
    case 1:
      it2++ ;
      if(it2 == 0) {
	 iterator_count++ ;
      }
      break ;
    case 2:
      it3++ ;
      break ;
    default:
      assert(0) ;
   }
}

template<class A, class T, class U, class V, class B>
void  Triple_concatanator<A,T,U,V,B>::operator++(int)
{
   operator++() ;
}

template<class A, class T, class U, class V, class B>
A& Triple_concatanator<A,T,U,V,B>::operator*()
{
   switch (iterator_count) {
    case 0:
      return(*it1) ;
    case 1:
      return(*it2) ;
    default:
      return(*it3) ;
   }
}

//////////////////////////////////////////////////////////////////////////
// Quadruple_concatanator class 
//////////////////////////////////////////////////////////////////////////

template<class A, class T, class U, class V, class W, class B>
Quadruple_concatanator<A,T,U,V,W,B>::Quadruple_concatanator()
   : iterator_count(0) 
{}

template<class A, class T, class U, class V, class W, class B>
Quadruple_concatanator<A,T,U,V,W,B>::Quadruple_concatanator(const B& obj)
   : it1(obj), it2(obj), it3(obj), it4(obj)
{
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
      if (it2 == 0) {
	 iterator_count++ ;
	 if (it3 == 0) {
	    iterator_count++ ;
	 }
      }
   }
}

template<class A, class T, class U, class V, class W, class B>
Quadruple_concatanator<A,T,U,V,W,B>::Quadruple_concatanator
       (const Quadruple_concatanator<A,T,U,V,W,B>& pc)
	  : iterator_count(pc.iterator_count), it1(pc.it1),
	    it2(pc.it2), it3(pc.it3), it4(pc.it4)
{}

template<class A, class T, class U, class V, class W, class B>
void Quadruple_concatanator<A,T,U,V,W,B>::operator() (const B& obj)
{
   it1(obj) ;
   it2(obj) ;
   it3(obj) ;
   it4(obj) ;
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
      if (it2 == 0) {
	 iterator_count++ ;
	 if (it3 == 0) {
	    iterator_count++ ;
	 }
      }
   }
}

template<class A, class T, class U, class V, class W, class B>
Quadruple_concatanator<A,T,U,V,W,B>& Quadruple_concatanator<A,T,U,V,W,B>::operator=
       (const Quadruple_concatanator<A,T,U,V,W,B>& pc)
{
  iterator_count = pc.iterator_count ;
  it1 = pc.it1 ;
  it2 = pc.it2 ;
  it3 = pc.it3 ;
  it4 = pc.it4 ;
  return *this ;
}

template<class A, class T, class U, class V, class W, class B>
Quadruple_concatanator<A,T,U,V,W,B>::~Quadruple_concatanator()
{}

template<class A, class T, class U, class V, class W, class B>
bool Quadruple_concatanator<A,T,U,V,W,B>::operator==(const int i) const
{
   assert (i == 0) ;
   if (iterator_count == 3) return (it4 == i) ;
   else return(false) ;
}

template<class A, class T, class U, class V, class W, class B>
bool Quadruple_concatanator<A,T,U,V,W,B>::operator!=(const int i) const
{
   assert (i == 0) ;
   if (iterator_count == 3) return (it4 != i) ;
   else return(true) ;
}

template<class A, class T, class U, class V, class W, class B>
void  Quadruple_concatanator<A,T,U,V,W,B>::operator++()
{
   switch (iterator_count) {
    case 0:
      it1++ ;
      if(it1 == 0) {
	 iterator_count++ ;
	 if(it2 == 0) {
	    iterator_count++ ;
	    if(it3 == 0) {
	       iterator_count++ ;
	    }
	 }
      }
      break ;
    case 1:
      it2++ ;
      if(it2 == 0) {
	 iterator_count++ ;
	 if(it3 == 0) {
	    iterator_count++ ;
	 }
      }
      break ;
    case 2:
      it3++ ;
      if(it3 == 0) {
	 iterator_count++ ;
      }
      break ;
    case 3:
      it4++ ;
      break ;
    default:
      assert(0) ;
   }
}

template<class A, class T, class U, class V, class W, class B>
void  Quadruple_concatanator<A,T,U,V,W,B>::operator++(int)
{
   operator++() ;
}

template<class A, class T, class U, class V, class W, class B>
A& Quadruple_concatanator<A,T,U,V,W,B>::operator*()
{
   switch (iterator_count) {
    case 0:
      return(*it1) ;
    case 1:
      return(*it2) ;
    case 2:
      return(*it3) ;
    default:
      return(*it4) ;
   }
}



