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
//      File:           slist.cpp
//      Authors:        Santosh Abraham
//      Created:        April 1995
//      Description:    Sorted list class and its iterators
//
/////////////////////////////////////////////////////////////////////////////

#include "slist.h"

/* ---------------------------------------------------------------------------
   Stuple class implementation
---------------------------------------------------------------------------- */

template<class N, class V>
Stuple<N,V>::Stuple(){}

template<class N, class V>
Stuple<N,V>::Stuple(const N& r){
  first = r;
}

template<class N, class V>
Stuple<N,V>::Stuple(const N& r, const V& v){
  first = r;
  second = v;
}

template<class N, class V>
Stuple<N,V>::Stuple(const Stuple<N,V>& t) {
  first = t.first;
  second = t.second;
}

template<class N, class V>
Stuple<N,V>::~Stuple(){}

template<class N, class V>      // defines name only equivalence
Stuple<N, V>& Stuple<N,V>::operator= (const Stuple<N,V>& t)
{
  first = t.first;
  second = t.second;
  return *this;
}

template<class N, class V>      // defines name & value equivalence
bool Stuple<N,V>::operator== (const Stuple<N,V>& lhs) const
{
  if ((first == lhs.first) && (second == lhs.second)) return(true);
  return(false);
}

template<class N, class V>      // defines name & value equivalence
bool Stuple<N,V>::operator!= (const Stuple<N,V>& lhs) const
{
  return (!(*this == lhs));
}

template<class N, class V>      // less than operator
bool Stuple<N,V>::operator< (const Stuple<N,V>& rhs) const
{
  if (first < rhs.first) return(true);
  return(false);
}



/* ----------------- << for Stuple ----------------------------------------- */
template <class N, class V> ostream& operator<<(ostream& os, Stuple<N, V>& stuple)
{
  os << "Stuple={ " ;
  os << stuple.first <<", " << stuple.second << " }" << endl;
  return os;
}


/*---------------------------------------------------------------------------
Slist class implementation
-----------------------------------------------------------------------------*/
template<class T>
Slist<T>::Slist()
{
   head_node = 0 ;
   tail_node = 0 ;
}

template<class T>
Slist<T>::Slist(const Slist<T>& l) 
{
   head_node = 0 ;
   tail_node = 0 ;
   for (Slist_iterator<T> lit(l) ; lit != 0 ; ++lit) {
	add_tail(*lit) ;
   }
}

template<class T>
Slist<T>& Slist<T>::operator=(const Slist<T>& l) 
{
   if (head_node != l.head_node)
   {
      clear() ;
      for (Slist_iterator<T> lit(l) ; lit != 0 ; ++lit) {
	 add_tail(*lit) ;
      }
   }
   return (*this) ;
}

template<class T>
Slist<T>& Slist<T>::operator=(const List<T>& l) 
{
   clear() ;
   for (List_iterator<T> lit(l) ; lit != 0 ; ++lit) {
	 add(*lit) ;
      }
   return (*this) ;
}


template<class T>
Slist<T>::~Slist()
{
   clear() ;
}

template<class T>
bool Slist<T>::operator==(const Slist<T>& l) const 
{
   Slist_iterator<T> lit(l) ;
   Slist_iterator<T> tit(*this) ;
   while((lit!=0) && (tit!=0)) {
      if (*lit == *tit) {
	 ++lit ;
	 ++tit ;
      } else {
	 return(false) ;
      }
   }
   if ((lit == 0) && (tit == 0)) {
      return(true) ;
   } else {
      return(false) ;
   }
}

template<class T>
bool Slist<T>::operator!=(const Slist<T>& lhs) const
{
   if(*this==lhs) return false;
   return true;
}


template<class T>
bool Slist<T>::is_member(const T& item) const
{
   for (Slist_iterator<T> lit(*this) ; lit != 0 ; ++lit) {
      if (item == (*lit)) return(true) ;
   }
   return(false) ;
}
	  
template<class T>
void Slist<T>::add(const T& item)
{
   Node<T> *newnode = new Node<T> (item) ;
   if (head_node == 0) {
      head_node = newnode ;
      head_node->set_next(0) ;
      tail_node = newnode ;
      return;
   }
   if (item < head()) {
      newnode->set_next(head_node);
      head_node = newnode;
      return;
   }
   Node<T>* tmp;
   for (tmp = head_node; tmp->next() != 0; tmp = tmp->next()) {
      if (item < (const T) (tmp->next()->value())) {
         newnode->set_next(tmp->next());
	 tmp->set_next(newnode);
	 return;
      }
   }
   assert (tail() == tmp->value());
   tmp->set_next(newnode);
   tail_node = newnode;
   tail_node->set_next(0);
   return;
}

template<class T>
void Slist<T>::remove(const T& item)
{
   if(head_node == 0) assert(0) ; // Slist is empty 
   if((const T)(head_node->value()) == item) {
      Node<T>* tmp = head_node ;
      if (tmp == tail_node) {
	 tail_node = 0 ;
      }
      head_node = head_node->next() ;
      delete tmp ;
      return ;
   } else {
      for(Node<T>* tmp = head_node ; tmp->next() != 0 ; tmp = tmp->next()) {
	 if((const T)(tmp->next()->value()) == item) {
	    Node<T>* tmp2 = tmp->next() ;
	    if (tmp2 == tail_node){
	       tail_node = tmp ;
	    }
	    tmp->set_next(tmp2->next()) ;
	    delete tmp2 ;
	    return ;
	 }
      }
      assert(0); // tried to remove an iterm not in Slist
   }
}

template<class T>
T Slist<T>::pop()
{
  T headval;
  Node<T>* tmp = head_node;
  assert (head_node != 0);
  headval = head_node->value();
  if (tmp == tail_node) {
     tail_node = 0;
     }
  head_node = head_node->next();
  delete tmp;
  return (headval);
}

template<class T>
void Slist<T>::add_tail(const T& item)
{
   Node<T> *newnode = new Node<T> (item) ;
   if (head_node == 0) {
      head_node = newnode ;
      head_node->set_next(0);
      tail_node = newnode ;
   } else {
      assert (!(item < tail()));
      tail_node->set_next(newnode) ;
      newnode->set_next(0) ;
      tail_node = newnode ;
   }
}

template<class T>
unsigned Slist<T>::size() const {
   int i =0;
   Node<T>* tmp = head_node;
   while(tmp != 0) {
      i=i+1;
      tmp=(*tmp).next();
   }
   return(i);
}

template<class T>
bool Slist<T>::is_empty() const
{
   if(head_node == 0) return(true);
   return(false);
}

template<class T>
void Slist<T>::clear()
{
   Node<T>* tmp ;
   while(head_node != 0) {
      tmp = head_node ;
      head_node = head_node->next() ;
      delete tmp ;
   }
   tail_node = 0 ;
}

template<class T>
T Slist<T>::head() const
{
   return(head_node->value()) ;
}

template<class T>
T Slist<T>::tail() const
{
   return(tail_node->value()) ;
}

template<class T>
void Slist<T>::check()
{
   T item = head();
   for (Slist_iterator<T> lit(*this) ; lit != 0 ; ++lit) {
        if ((*lit) < item)
		assert(0);
	item = (*lit);
	}
}      
	 
   
   
   
/* ----------------- << for Slists ----------------------------------------- */
template <class T> ostream& operator<<(ostream& os, Slist<T>& slist)
{
  os << "Slist=[ " ;
  for(Slist_iterator<T> iter(slist); iter != 0; ){
      os << *iter;
    iter++;
    if (iter != 0) os << ", ";
  }
  os << " ]" << endl;
  return os;
}


/*---------------------------------------------------------------------------
Slist iterator class implementation
-----------------------------------------------------------------------------*/

template<class T>
Slist_iterator<T>::Slist_iterator() 
{
   iterator = 0 ;
}

template<class T>
Slist_iterator<T>::Slist_iterator(const Slist<T>& slist)
{
   iterator = slist.head_node ;
}

template<class T>
void Slist_iterator<T>::operator()(const Slist<T>& slist)
{
   iterator = slist.head_node;
}

template<class T>
Slist_iterator<T>::~Slist_iterator()
{
}

template<class T>
bool Slist_iterator<T>::operator==(const void* x) const
{
   return (x == (void*)(iterator)) ;
}

template<class T>
bool Slist_iterator<T>::operator!=(const void* x) const
{
   return (x != (void*)(iterator)) ;
}


template<class T>
void Slist_iterator<T>::operator++() 
{
   if(iterator != 0) iterator = iterator->next() ;
   return ;
}

template<class T>
void Slist_iterator<T>::operator++(int) 
{
   if(iterator != 0) iterator = iterator->next() ;
   return ;
}

template<class T>
T& Slist_iterator<T>::operator*() 
{
   if(iterator == 0) {
      assert(0) ;
   }
   return(iterator->value()) ;
}


// Unsupported methods 
template<class T>
Slist_iterator<T>::Slist_iterator(const Slist_iterator<T>&)
{
    assert(0); 
}

template<class T>
Slist_iterator<T>& Slist_iterator<T>::operator=(const Slist_iterator<T>&)
{
    assert(0); 
    return *this; //return to make CC happy
} 


/*----------------------------------------------------------------------------
Slist_filterator Implementation
-----------------------------------------------------------------------------*/

template<class T>
Slist_filterator<T>::Slist_filterator() {}

template<class T>
Slist_filterator<T>::Slist_filterator(const Slist<T>& s,
	const Filter<T>* filter) :Slist_iterator<T>(s), rep(filter) {

  while(!(Slist_iterator<T>::operator==(0) ||
	rep->filter(Slist_iterator<T>::operator*())))
	Slist_iterator<T>::operator++();
}

template<class T>
void Slist_filterator<T>::operator()(const Slist<T>& s, const Filter<T>* filter)

{
  rep = filter;
  Slist_iterator<T>::operator()(s);
  while(!(Slist_iterator<T>::operator==(0) ||
	rep->filter(Slist_iterator<T>::operator*())))
	Slist_iterator<T>::operator++();
}

template<class T>
Slist_filterator<T>::~Slist_filterator(){}

template<class T>
void Slist_filterator<T>::operator++() 
{
  do Slist_iterator<T>::operator++();
  while(!(Slist_iterator<T>::operator==(0) ||
	rep->filter(Slist_iterator<T>::operator*())));
  return ;
}

template<class T>
void Slist_filterator<T>::operator++(int) 
{
   do Slist_iterator<T>::operator++();
   while(!(Slist_iterator<T>::operator==(0) ||
	 rep->filter(Slist_iterator<T>::operator*())));
   return ;
}


// Unsupported methods 
template<class T>
Slist_filterator<T>::Slist_filterator(const Slist_filterator<T>&)
{
    assert(0); 
}

template<class T>
Slist_filterator<T>& Slist_filterator<T>::operator=(const Slist_filterator<T>&)
{
    assert(0); 
    return *this; //return to make CC happy
} 


