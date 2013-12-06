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
//      File:           tuples.cpp
//      Authors:        Vinod Kathail, Richard Johnson, Sadun Anik
//      Created:        December 1994
//      Description:    Pair, Triple and Quad definitions
//
/////////////////////////////////////////////////////////////////////////////

#include "tuples.h"

/* ---------------------------------------------------------------------------
   Pair class implementation
---------------------------------------------------------------------------- */
/*
template<class T1, class T2>
Pair<T1, T2>::Pair(){}

template<class T1, class T2>
Pair<T1, T2>::Pair(const T1& t1, const T2& t2)
      :first(t1), second(t2) {}

template<class T1, class T2>
Pair<T1, T2>::Pair(const Pair<T1, T2>& x)
      :first(x.first), second(x.second) {}

template<class T1, class T2>
Pair<T1, T2>::~Pair(){}

template<class T1, class T2>
Pair<T1, T2>& Pair<T1, T2>::operator=(const Pair<T1, T2>& x) {
  if (this != &x) {
    first = x.first;
    second = x.second;
  }
  return *this;
}

template<class T1, class T2>
bool Pair<T1, T2>::operator==(const Pair<T1, T2>& x) const {
  return ((first == x.first) && (second == x.second));
}

    
template<class T1, class T2>
bool Pair<T1, T2>::operator!=(const Pair<T1, T2>& x) const {
  return (!(x == *this));
}

*/
template <class T1, class T2> 
Pair<T1, T2> make_pair(const T1& t1, const T2& t2) {
  return Pair<T1, T2>(t1, t2);
}
  
template <class T1, class T2> 
ostream& operator<<(ostream& os, const Pair<T1, T2>& x) {
  os << "<" << x.first << ", " << x.second << ">" ;
  return os;
}

/* ---------------------------------------------------------------------------
   Triple class implementation
---------------------------------------------------------------------------- */

template<class T1, class T2, class T3>
Triple<T1, T2, T3>::Triple(){}

template<class T1, class T2, class T3>
Triple<T1, T2, T3>::Triple(const T1& t1, const T2& t2, const T3& t3)
      :first(t1), second(t2), third(t3) {}

template<class T1, class T2, class T3>
Triple<T1, T2, T3>::Triple(const Triple<T1, T2, T3>& x)
      :first(x.first), second(x.second), third(x.third) {}

template<class T1, class T2, class T3>
Triple<T1, T2, T3>::~Triple(){}

template<class T1, class T2, class T3>
Triple<T1, T2, T3>& Triple<T1, T2, T3>::operator=(const Triple<T1, T2, T3>& x)
{
  if (this != &x) {
    first = x.first;
    second = x.second;
    third = x.third;
  }
  return *this;
}

template<class T1, class T2, class T3>
bool Triple<T1, T2, T3>::operator==(const Triple<T1, T2, T3>& x) const {
  return ((first == x.first) && (second == x.second) 
	  && (third == x.third));
}

    
template<class T1, class T2, class T3>
bool Triple<T1, T2, T3>::operator!=(const Triple<T1, T2, T3>& x) const {
  return (!(x == *this));
}


template <class T1, class T2, class T3> 
Triple<T1, T2, T3> triple(const T1& t1, const T2& t2, const T3& t3) {
  return Triple<T1, T2, T3>(t1, t2, t3);
}
  
template <class T1, class T2, class T3> 
ostream& operator<<(ostream& os, Triple<T1, T2, T3>& x) {
  os << "<" << x.first << ", " << x.second << ", " << x.third << ">" ;
  return os;
}



/* ---------------------------------------------------------------------------
   Quad class implementation
---------------------------------------------------------------------------- */

template<class T1, class T2, class T3, class T4>
Quad<T1, T2, T3, T4>::Quad(){}

template<class T1, class T2, class T3, class T4>
Quad<T1, T2, T3, T4>::Quad(const T1& t1, const T2& t2,
			   const T3& t3, const T4& t4)
      :first(t1), second(t2), third(t3), fourth(t4) {}

template<class T1, class T2, class T3, class T4>
Quad<T1, T2, T3, T4>::Quad(const Quad<T1, T2, T3, T4>& x)
      :first(x.first), second(x.second), third(x.third), fourth(x.fourth) {}

template<class T1, class T2, class T3, class T4>
Quad<T1, T2, T3, T4>::~Quad(){}

template<class T1, class T2, class T3, class T4>
Quad<T1, T2, T3, T4>& Quad<T1, T2, T3, T4>::operator=(const Quad<T1, T2, T3, T4>& x)
{
  if (this != &x) {
    first = x.first;
    second = x.second;
    third = x.third;
    fourth = x.fourth;
  }
  return *this;
}

template<class T1, class T2, class T3, class T4>
bool Quad<T1, T2, T3, T4>::operator==(const Quad<T1, T2, T3, T4>& x) const {
  return ((first == x.first) && (second == x.second) 
	  && (third == x.third) && (fourth == x.fourth));
}

    
template<class T1, class T2, class T3, class T4>
bool Quad<T1, T2, T3, T4>::operator!=(const Quad<T1, T2, T3, T4>& x) const {
  return (!(x == *this));
}


template <class T1, class T2, class T3, class T4> 
Quad<T1, T2, T3, T4> quadr(const T1& t1, const T2& t2, const T3& t3, const T4& t4) {
  return Quad<T1, T2, T3, T4>(t1, t2, t3, t4);
}
  
template <class T1, class T2, class T3, class T4> 
ostream& operator<<(ostream& os, Quad<T1, T2, T3, T4>& x) {
  os << "<" << x.first << ", " << x.second
     << ", " << x.third << ", " << x.fourth << ">" ;
  return os;
}

// end of tuples.cpp
