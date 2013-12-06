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
//      File:           operand_concat.cpp
//      Authors:        Sadun Anik 
//      Created:        December 1995
//      Description:    Templates for concatanating operand iterators
//
/////////////////////////////////////////////////////////////////////////////

#include "operand_concat.h"
#include "el_port.h"

//////////////////////////////////////////////////////////////////////////
// Operand_2_iter class 
//////////////////////////////////////////////////////////////////////////


template<class T, class U>
Operand_2_iter<T, U>::Operand_2_iter()
   : iterator_count(0) 
{}

template<class T, class U>
Operand_2_iter<T, U>::Operand_2_iter(const Op* obj)
   : it1(obj), it2(obj)
{
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
   }
}

template<class T, class U>
Operand_2_iter<T, U>::Operand_2_iter
       (const Operand_2_iter<T,U>& pc)
	  : iterator_count(pc.iterator_count), it1(pc.it1), it2(pc.it2)
{}

template<class T, class U>
void Operand_2_iter<T, U>::operator() (const Op* obj)
{
   it1(obj) ;
   it2(obj) ;
   iterator_count = 0 ;
   if (it1 == 0) {
      iterator_count++ ;
   }
}

template<class T, class U>
Operand_2_iter<T, U>& Operand_2_iter<T, U>::operator=
       (const Operand_2_iter<T,U>& pc)
{
  iterator_count = pc.iterator_count ;
  it1 = pc.it1 ;
  it2 = pc.it2 ;
  return *this ;
}

template<class T, class U>
Operand_2_iter<T, U>::~Operand_2_iter()
{}

template<class T, class U>
bool Operand_2_iter<T, U>::operator==(const void* i) const
{
   assert (i == 0) ;
   if (iterator_count == 1) return (it2 == i) ;
   else return(false) ;
}

template<class T, class U>
bool Operand_2_iter<T, U>::operator!=(const void* i) const
{
   assert (i == 0) ;
   if (iterator_count == 1) return (it2 != i) ;
   else return(true) ;
}

template<class T, class U>
void  Operand_2_iter<T, U>::operator++()
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

template<class T, class U>
void  Operand_2_iter<T, U>::operator++(int)
{
   operator++() ;
}

template<class T, class U>
Operand& Operand_2_iter<T, U>::operator*()
{
   switch (iterator_count) {
    case 0:
      return(*it1) ;
    default:
      return(*it2) ;
   }
}

template<class T, class U>
El_ref_type Operand_2_iter<T, U>::ref_type() 
{
   switch (iterator_count) {
    case 0:
      return(it1.ref_type()) ;
    default:
      return(it2.ref_type()) ;
   }
}

template<class T, class U>
Port_num Operand_2_iter<T, U>::get_port_num() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_port_num()) ;
    default:
      return(it2.get_port_num()) ;
   }
}

template<class T, class U>
Op* Operand_2_iter<T, U>::get_op() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_op()) ;
    default:
      return(it2.get_op()) ;
   }
}

template<class T, class U>
El_ref Operand_2_iter<T, U>::get_ref() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_ref()) ;
    default:
      return(it2.get_ref()) ;
   }
}


//////////////////////////////////////////////////////////////////////////
// Operand_3_iter class 
//////////////////////////////////////////////////////////////////////////


template<class T, class U, class V>
Operand_3_iter<T,U,V>::Operand_3_iter()
   : iterator_count(0) 
{}

template<class T, class U, class V>
Operand_3_iter<T,U,V>::Operand_3_iter(const Op* obj)
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

template<class T, class U, class V>
Operand_3_iter<T,U,V>::Operand_3_iter
       (const Operand_3_iter<T,U,V>& pc)
	  : iterator_count(pc.iterator_count), it1(pc.it1),
	    it2(pc.it2), it3(pc.it3)
{}

template<class T, class U, class V>
void Operand_3_iter<T,U,V>::operator() (const Op* obj)
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

template<class T, class U, class V>
Operand_3_iter<T,U,V>& Operand_3_iter<T,U,V>::operator=
       (const Operand_3_iter<T,U,V>& pc)
{
  iterator_count = pc.iterator_count ;
  it1 = pc.it1 ;
  it2 = pc.it2 ;
  it3 = pc.it3 ;
  return *this ;
}

template<class T, class U, class V>
Operand_3_iter<T,U,V>::~Operand_3_iter()
{}

template<class T, class U, class V>
bool Operand_3_iter<T,U,V>::operator==(const void* i) const
{
   assert (i == 0) ;
   if (iterator_count == 2) return (it3 == i) ;
   else return(false) ;
}

template<class T, class U, class V>
bool Operand_3_iter<T,U,V>::operator!=(const void* i) const
{
   assert (i == 0) ;
   if (iterator_count == 2) return (it3 != i) ;
   else return(true) ;
}

template<class T, class U, class V>
void  Operand_3_iter<T,U,V>::operator++()
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

template<class T, class U, class V>
void  Operand_3_iter<T,U,V>::operator++(int)
{
   operator++() ;
}

template<class T, class U, class V>
Operand& Operand_3_iter<T,U,V>::operator*()
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

template<class T, class U, class V>
El_ref_type Operand_3_iter<T,U,V>::ref_type() 
{
   switch (iterator_count) {
    case 0:
      return(it1.ref_type()) ;
    case 1:
      return(it2.ref_type()) ;
    default:
      return(it3.ref_type()) ;
   }
}

template<class T, class U, class V>
Port_num Operand_3_iter<T,U,V>::get_port_num() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_port_num()) ;
    case 1:
      return(it2.get_port_num()) ;
    default:
      return(it3.get_port_num()) ;
   }
}

template<class T, class U, class V>
Op* Operand_3_iter<T,U,V>::get_op() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_op()) ;
    case 1:
      return(it2.get_op()) ;
    default:
      return(it3.get_op()) ;
   }
}

template<class T, class U, class V>
El_ref Operand_3_iter<T,U,V>::get_ref() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_ref()) ;
    case 1:
      return(it2.get_ref()) ;
    default:
      return(it3.get_ref()) ;
   }
}

//////////////////////////////////////////////////////////////////////////
// Operand_4_iter class 
//////////////////////////////////////////////////////////////////////////

template<class T, class U, class V, class W>
Operand_4_iter<T,U,V,W>::Operand_4_iter()
   : iterator_count(0) 
{}

template<class T, class U, class V, class W>
Operand_4_iter<T,U,V,W>::Operand_4_iter(const Op* obj)
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

template<class T, class U, class V, class W>
Operand_4_iter<T,U,V,W>::Operand_4_iter
       (const Operand_4_iter<T,U,V,W>& pc)
	  : iterator_count(pc.iterator_count), it1(pc.it1),
	    it2(pc.it2), it3(pc.it3), it4(pc.it4)
{}

template<class T, class U, class V, class W>
void Operand_4_iter<T,U,V,W>::operator() (const Op* obj)
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

template<class T, class U, class V, class W>
Operand_4_iter<T,U,V,W>& Operand_4_iter<T,U,V,W>::operator=
       (const Operand_4_iter<T,U,V,W>& pc)
{
  iterator_count = pc.iterator_count ;
  it1 = pc.it1 ;
  it2 = pc.it2 ;
  it3 = pc.it3 ;
  it4 = pc.it4 ;
  return *this ;
}

template<class T, class U, class V, class W>
Operand_4_iter<T,U,V,W>::~Operand_4_iter()
{}

template<class T, class U, class V, class W>
bool Operand_4_iter<T,U,V,W>::operator==(const void* i) const
{
   assert (i == 0) ;
   if (iterator_count == 3) return (it4 == i) ;
   else return(false) ;
}

template<class T, class U, class V, class W>
bool Operand_4_iter<T,U,V,W>::operator!=(const void* i) const
{
   assert (i == 0) ;
   if (iterator_count == 3) return (it4 != i) ;
   else return(true) ;
}

template<class T, class U, class V, class W>
void  Operand_4_iter<T,U,V,W>::operator++()
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

template<class T, class U, class V, class W>
void  Operand_4_iter<T,U,V,W>::operator++(int)
{
   operator++() ;
}

template<class T, class U, class V, class W>
Operand& Operand_4_iter<T,U,V,W>::operator*()
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

template<class T, class U, class V, class W>
El_ref_type Operand_4_iter<T,U,V,W>::ref_type() 
{
   switch (iterator_count) {
    case 0:
      return(it1.ref_type()) ;
    case 1:
      return(it2.ref_type()) ;
    case 2:
      return(it3.ref_type()) ;
    default:
      return(it4.ref_type()) ;
   }
}

template<class T, class U, class V, class W>
Port_num Operand_4_iter<T,U,V,W>::get_port_num() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_port_num()) ;
    case 1:
      return(it2.get_port_num()) ;
    case 2:
      return(it3.get_port_num()) ;
    default:
      return(it4.get_port_num()) ;
   }
}

template<class T, class U, class V, class W>
Op* Operand_4_iter<T,U,V,W>::get_op() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_op()) ;
    case 1:
      return(it2.get_op()) ;
    case 2:
      return(it3.get_op()) ;
    default:
      return(it4.get_op()) ;
   }
}

template<class T, class U, class V, class W>
El_ref Operand_4_iter<T,U,V,W>::get_ref() 
{
   switch (iterator_count) {
    case 0:
      return(it1.get_ref()) ;
    case 1:
      return(it2.get_ref()) ;
    case 2:
      return(it3.get_ref()) ;
    default:
      return(it4.get_ref()) ;
   }
}

