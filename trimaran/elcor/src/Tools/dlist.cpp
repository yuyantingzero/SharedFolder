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
//      File:           dlist.cpp
//      Authors:        Mike Schlansker, Sadun Anik, Richard Johnson
//                      Vinod Kathail
//      Created:        December 1994
//      Description:    Doubly linked list implementation
//
/////////////////////////////////////////////////////////////////////////////

#include "dlist.h"

/*---------------------------------------------------------------------------
   Dnode class implementation
-----------------------------------------------------------------------------*/
template<class T>
Dnode<T>::Dnode()
{
//   n1 ; //dia
   next = 0 ;
}

template<class T>
Dnode<T>::Dnode(const Dnode<T>& Dnode) 
   : n1(Dnode.n1)
{
//   n1=new T(*Dnode.n1); //mss
   next = Dnode.next ;
   
}

template<class T>
Dnode<T>::Dnode(const T& item) 
   : n1(item)
{
//   n1=new T(item); //mss
   next = 0 ;
}

template<class T>
Dnode<T>::~Dnode() 
{
//delete n1;
}

template<class T>
T& Dnode<T>::value() 
{
   return (n1) ;
}

/*---------------------------------------------------------------------------
   Dlist class implementation
-----------------------------------------------------------------------------*/

template<class T>
Dlist<T>::Dlist()
{
   head_node = 0;
   tail_node = 0;
}

template<class T>
Dlist<T>::Dlist(const Dlist<T>& l) {
   head_node = 0;
   tail_node = 0;
   for (Dlist_iterator<T> lit(l) ; lit != 0 ; lit++) {
	   push_tail(*lit) ;
   }
}

template<class T>
Dlist<T>::~Dlist()
{
   clear() ;
}

template<class T>
Dlist<T>& Dlist<T>::operator=(const Dlist<T>& l) 
{
   if (this != &l)
   {
      clear() ;
      for (Dlist_iterator<T> lit(l) ; lit != 0 ; ++lit) {
	 push_tail(*lit) ;
      }
   }
   return (*this) ;
}

template<class T>
bool Dlist<T>::operator==(const Dlist<T>& l) const 
{
   Dlist_iterator<T> lit(l) ;
   Dlist_iterator<T> tit(*this) ;
   while((lit!=0) && (tit!=0)) {
      if (*lit == *tit) {
	      lit++ ;
	      tit++ ;
      } else {
	      return(0) ;
      }
   }
   if ((lit == 0) && (tit == 0)) {
      return(1) ;
   } else {
      return(0) ;
   }
}

template<class T>
bool Dlist<T>::operator!=(const Dlist<T>& lhs) const 
{
   if(*this==lhs) return false;
   return true;
}

template<class T>
void Dlist<T>::push(const T& item)
{
   Dnode<T> *newnode = new Dnode<T> (item) ;
   if (head_node == 0) {
      head_node = newnode ;
      tail_node = newnode;
      head_node->next = 0 ;
      head_node->prev = 0;
   } else {
      newnode->next = head_node ;
      newnode->prev=0;
      head_node->prev = newnode;
      head_node = newnode ;
   }
}

template<class T>
void Dlist<T>::push_tail(const T& item)
{
   Dnode<T> *newnode = new Dnode<T> (item) ;
   if (head_node == 0) {
      tail_node = newnode ;
      head_node = newnode;
      head_node->next = 0 ;
      head_node->prev = 0;
   } else {
      newnode->prev = tail_node ;
      newnode->next=0;
      tail_node->next = newnode;
      tail_node = newnode ;
   }
}

template<class T>
T Dlist<T>::pop()
{
   if (head_node == 0) assert(0) ; // Dlist is empty 
   Dnode<T>* tmp=head_node;
   T val(head_node->n1);
   head_node = head_node->next;
   if (head_node==0) tail_node=0;
   else head_node->prev=0;
   delete tmp ;
   return(val);
}

template<class T>
T Dlist<T>::pop_tail()
{
   if (tail_node == 0) assert(0) ; // Dlist is empty 
   Dnode<T>* tmp=tail_node;
//   T val=*(tail_node->n1);
   T val(tail_node->n1);   
   tail_node=tail_node->prev;
   if(tail_node==0) head_node=0;
   else tail_node->next=0;
   delete tmp ;
   return(val);
}

// return node containing item, else return NULL;
template<class T>
Dnode<T>* Dlist<T>::find(const T& item) const
{
  Dnode<T>* nd = head_node;
  while (nd != NULL && nd->value() != item) {
    nd = nd->next;
  }
  return nd;
}

template<class T>
void Dlist<T>::insert_after(Dnode<T>* nptr, const T& item)
{
  assert (nptr != NULL);

  Dnode<T>* new_node = new Dnode<T>(item) ;
  new_node->next = nptr->next ;
  if (nptr->next) {
    nptr->next->prev = new_node;
  }

  new_node->prev = nptr ;
  nptr->next = new_node ;

  if (nptr == tail_node) {
    tail_node = new_node ;
  }
}

template<class T>
void Dlist<T>::insert_before(Dnode<T>* nptr, const T& item)
{
  assert (nptr != NULL);

  Dnode<T>* new_node = new Dnode<T>(item) ;
  new_node->prev = nptr->prev ;
  if (nptr->prev) {
    nptr->prev->next = new_node;
  }

  new_node->next = nptr ;
  nptr->prev = new_node ;

  if (nptr == head_node) {
    head_node = new_node ;
  }
}

template<class T>
void Dlist<T>::remove(const T& item)
{
  Dnode<T>* nd = head_node;
  while (nd != NULL && nd->value() != item) {
    nd = nd->next;
  }
  remove(nd) ;
}

template<class T>
void Dlist<T>::remove(Dnode<T>* nd)
{
  if(nd == NULL)
    return;

  if (nd == head_node) {
    head_node = nd->next;
  } else {
    nd->prev->next = nd->next;
  }
  if (nd == tail_node) {
    tail_node = nd->prev;
  } else {
    nd->next->prev = nd->prev;
  }
  delete nd;
}

template<class T>
bool Dlist<T>::is_member(const T& item) const
{
   for (Dlist_iterator<T> lit(*this) ; lit != 0 ; lit++) {
      if (item == (*lit)) return(true) ;
   }
   return(false) ;
}

template<class T>
unsigned Dlist<T>::size() const
{
   int i =0;
   Dnode<T>* tmp = head_node;
   while(tmp != 0) {
      i=i+1;
      tmp=(*tmp).next;
   }
   return(i);
}
 
template<class T>
bool Dlist<T>::is_empty() const
{
   if(head_node==0) return(1);
   return(0);
}

template<class T>
void Dlist<T>::clear()
{
   Dnode<T>* tmp ;
   while(head_node != 0) {
      tmp = head_node ;
      head_node = head_node->next ;
      delete tmp ;
   }
   tail_node=0;
}

template<class T>
T Dlist<T>::head() const
{
   assert(head_node != 0);
//   return *(head_node->n1);
   return (head_node->n1);
}

template<class T>
T Dlist<T>::tail() const
{
   assert(tail_node != 0);
//   return *(tail_node->n1);
   return (tail_node->n1);
}

// SAM 9-2000, append all elements of l to the current list
template<class T>
Dlist<T>& Dlist<T>::append(const Dlist<T>& l)
{
   for (Dlist_iterator<T> lit(l) ; lit != 0 ; ++lit) {
      push_tail(*lit) ;
   }

   return (*this);
}

//template<class T>
//void Dlist<T>::drop_head()
//{
//	assert(head!=0);
//	Dnode<T>* tmp=head;
//	head=head->next;
//	head->prev=0;
//	delete tmp;
//}
   
//template<class T>
//void Dlist<T>::drop_tail()
//{
//	assert(tail!=0);
//	Dnode<T>* tmp=tail;
//	tail=tail->prev;
//	tail->next=0;
//	delete tmp;
//}

/* ----------------- << for Dlists ----------------------------------------- */
template <class T> ostream& operator<<(ostream& os, Dlist<T>& dlist)
{
  os << "Dlist=[ " ;
  for(Dlist_iterator<T> iter(dlist); iter != 0; ){
    os << *iter;
    iter++;
    if (iter != 0) os << ", ";
  }
  os << " ]" << endl;
  return os;
}

/*---------------------------------------------------------------------------
   Dlist_iterator class implementation
-----------------------------------------------------------------------------*/

template<class T>
Dlist_iterator<T>::Dlist_iterator() 
{
   iterator = 0 ;
}

template<class T>
Dlist_iterator<T>::Dlist_iterator(const Dlist<T>& dlist, bool end)
{
  if (end == false) {
    iterator = (Dnode<T>*)(dlist.head_node) ;
  } else {
    iterator = (Dnode<T>*)(dlist.tail_node) ;
  }
}

template<class T>
Dlist_iterator<T>::Dlist_iterator(const Dlist<T>& dlist, const T& val)
{
  iterator = dlist.find(val);
}

template<class T>
Dlist_iterator<T>::Dlist_iterator(Dnode<T> *node)
{
  iterator = node;
}

template<class T>
void Dlist_iterator<T>::operator()(const Dlist<T>& dlist, bool end)
{
  if (end == false) {
    iterator = (Dnode<T>*)(dlist.head_node) ;
  } else {
    iterator = (Dnode<T>*)(dlist.tail_node) ;
  }
}

template<class T>
void Dlist_iterator<T>::operator()(const Dlist<T>& dlist, const T& val)
{
  iterator = dlist.find(val);
}

template<class T>
void Dlist_iterator<T>::operator()(Dnode<T> *node)
{
  iterator = node;
}

template<class T>
Dlist_iterator<T>::~Dlist_iterator()
{
}

template<class T>
bool Dlist_iterator<T>::operator==(const void* x) const
{
   return (x == (void*)(iterator)) ;
}


template<class T>
bool Dlist_iterator<T>::operator!=(const void* x) const
{
   return (x != (void*)(iterator)) ;
}

template<class T>
void Dlist_iterator<T>::operator++()
{
   if(iterator != 0) iterator = iterator->next ;
   return ;
}

template<class T>
void Dlist_iterator<T>::operator++(int)
	                                   // int parameter signals postfix
{
   if(iterator != 0) iterator = iterator->next ;
   return ;
}

template<class T>
void Dlist_iterator<T>::operator--()
{
   if(iterator != 0) iterator = iterator->prev ;
   return ;
}

template<class T>
void Dlist_iterator<T>::operator--(int)
	                                   // int parameter signals postfix
{
   if(iterator != 0) iterator = iterator->prev ;
   return ;
}

template<class T>
T& Dlist_iterator<T>::operator*() 
{
   if(iterator == 0) {
      assert(0) ;
   }
   return((*iterator).n1) ;
//   return(*((*iterator).n1)) ; //mss
}

// Unsupported functions 
template<class T>
Dlist_iterator<T>::Dlist_iterator(const Dlist_iterator<T>&)
{
    assert(0); 
}

template<class T>
Dlist_iterator<T>& Dlist_iterator<T>::operator=(const Dlist_iterator<T>&)
{
    assert(0); 
    return *this; //return to make CC happy
} 


/*----------------------------------------------------------------------------
    Dlist_filterator Implementation
-----------------------------------------------------------------------------*/

template<class T>
Dlist_filterator<T>::Dlist_filterator() {}

template<class T>
Dlist_filterator<T>::Dlist_filterator(const Dlist<T>& s, 
	                               const Filter<T>* filter)
	:Dlist_iterator<T>(s), rep(filter) {

  while(!(Dlist_iterator<T>::operator==(0) ||
	rep->filter(Dlist_iterator<T>::operator*())))
    Dlist_iterator<T>::operator++();
}

template<class T>
void Dlist_filterator<T>::operator()(const Dlist<T>& s,
				     const Filter<T>* filter)
{
  rep = filter;
  Dlist_iterator<T>::operator()(s);
  while(!(Dlist_iterator<T>::operator==(0) ||
	rep->filter(Dlist_iterator<T>::operator*())))
    Dlist_iterator<T>::operator++();
}

template<class T>
Dlist_filterator<T>::~Dlist_filterator(){}

template<class T>
void Dlist_filterator<T>::operator++() 
{
  do Dlist_iterator<T>::operator++();
  while(!(Dlist_iterator<T>::operator==(0) ||
	rep->filter(Dlist_iterator<T>::operator*())));
  return ;
}

template<class T>
void Dlist_filterator<T>::operator++(int) 
{
   do Dlist_iterator<T>::operator++();
   while(!(Dlist_iterator<T>::operator==(0) ||
	 rep->filter(Dlist_iterator<T>::operator*())));
   return ;
}

// Unsupported methods 
template<class T>
Dlist_filterator<T>::Dlist_filterator(const Dlist_filterator<T>&)
{
    assert(0); 
}

template<class T>
Dlist_filterator<T>& Dlist_filterator<T>::operator=(const Dlist_filterator<T>&)
{
    assert(0); 
    return *this; //return to make CC happy
} 
