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
//      File:           list.cpp
//      Authors:        Sadun Anik, Vinod Kathail Richard Johnson,
//                      Santosh Abraham
//      Created:        May 1994
//      Description:    List class and its iterators
//
/////////////////////////////////////////////////////////////////////////////

#include "list.h"

/*---------------------------------------------------------------------------
   Node class implementation
-----------------------------------------------------------------------------*/
/*
template<class T>
Node<T>::Node()
{
   next_node = 0 ;
}

template<class T>
Node<T>::Node(const Node<T>& node)
   : n1(node.n1)
{
   next_node = node.next_node ;
//   n1 = new T(*(node.n1)) ;   
}

template<class T>
Node<T>::Node(const T& item)
   : n1(item)
{
   next_node = 0 ;
//   n1 = new T(item) ;
}

template<class T>
Node<T>::~Node() 
{
//   delete n1 ;
}

template<class T>
Node<T>* Node<T>::next() 
{
   return(next_node) ;
}

template<class T>
T& Node<T>::value() 
{
   return(n1) ;
}

template<class T>
void Node<T>::set_next(Node<T>* nptr) 
{
   next_node = nptr ;
}
*/
/*---------------------------------------------------------------------------
   List class implementation
-----------------------------------------------------------------------------*/
template<class T>
List<T>::List()
{
   head_node = 0 ;
   tail_node = 0 ;
   num_items = 0 ;
}

template<class T>
List<T>::List(const List<T>& l) 
{
   head_node = 0 ;
   tail_node = 0 ;
   num_items = 0 ;
   for (Node<T>* lit = l.head_node ; lit != 0 ; lit = lit->next_node) {
     add_tail(lit->n1) ;
   }
}

template<class T>
List<T>::List(const T& item) 
{
   head_node = 0 ;
   tail_node = 0 ;
   num_items = 0 ;
   add_tail(item) ;
}

template<class T>
List<T>& List<T>::operator=(const List<T>& l) 
{
   if (head_node != l.head_node)
   {
      clear() ;
      for (List_iterator<T> lit(l) ; lit != 0 ; ++lit) {
	 add_tail(*lit) ;
      }
   }
   return (*this) ;
}

template<class T>
List<T>::~List()
{
   clear() ;
}

template<class T>
bool List<T>::operator==(const List<T>& l) const 
{
   if (num_items != l.size()) return (false);

   List_iterator<T> lit(l) ;
   List_iterator<T> tit(*this) ;
   while(lit!=0) {
      if (*lit != *tit) {
	 return false;
      }
      ++lit ;
      ++tit ;
   }
   return(true) ;
}

template<class T>
bool List<T>::operator!=(const List<T>& lhs) const
{
   if (*this==lhs) return false;
   return true;
}

template<class T>
bool List<T>::is_member(const T& item) const
{
  Node<T>* nd = head_node;
  while (nd != NULL && nd->value() != item) {
    nd = nd->next();
  }
  if (nd) return(true) ;
  else return(false) ;
}
	  
template<class T>
T& List<T>::operator[](unsigned idx) const
{
  Node<T>* nd = head_node;
  
  if (idx < 0 || idx >= num_items)
    El_punt("list index out of bounds: %u (num_items=%u)", idx, num_items);

  for (unsigned j = 1; j <=  idx; j++) {
    nd = nd->next();
    assert(nd); // if this fails, num_items is inconsistent
  }

  return nd->value();
}

template<class T>
void List<T>::add_head(const T& item)
{
   Node<T> *newnode = new Node<T> (item) ;
   num_items += 1;
   if (head_node == 0) {
      head_node = newnode ;
      head_node->set_next(0) ;
      tail_node = newnode ;
   } else {
      newnode->set_next(head_node) ;
      head_node = newnode ;
   }
}

template<class T>
void List<T>::add_tail(const T& item)
{
   Node<T> *newnode = new Node<T> (item) ;
   num_items += 1;
   if (head_node == 0) {
      head_node = newnode ;
      head_node->set_next(0);
      tail_node = newnode ;
   } else {
      tail_node->set_next(newnode) ;
      newnode->set_next(0) ;
      tail_node = newnode ;
   }
}

// return node containing item, else return NULL;
template<class T>
Node<T>* List<T>::find(const T& item)
{
  Node<T>* nd = head_node;
  while (nd != NULL && nd->value() != item) {
    nd = nd->next();
  }
  return nd;
}

// return node previous to node containing item, else
// return NULL if item not found or item is first in list.
template<class T>
Node<T>* List<T>::find_prev(const T& item)
{
  Node<T>* prev_nd = NULL;
  Node<T>* nd = head_node;
  while (nd != NULL && nd->value() != item) {
    prev_nd = nd;
    nd = nd->next();
  }
  return prev_nd;
}

template<class T>
void List<T>::insert_after(Node<T>* nptr, const T& item)
{
  assert (nptr != NULL);

  Node<T>* new_node = new Node<T>(item) ;
  num_items += 1;
  new_node->next_node = nptr->next_node ;
  nptr->next_node = new_node ;
  if (nptr == tail_node) {
    tail_node = new_node ;
  }
}

// remove item if it exists in list
template<class T>
void List<T>::remove(const T& item)
{
   if (head_node == 0) return;

   if ((const T&)(head_node->value()) == item) {
      Node<T>* tmp = head_node ;
      if (tmp == tail_node) {
	 tail_node = 0 ;
      }
      head_node = head_node->next() ;
      num_items -= 1;
      delete tmp ;
      return ;
   } else {
      for (Node<T>* tmp = head_node ; tmp->next() != 0 ; tmp = tmp->next()) {
	 Node<T>* tmp_next = tmp->next() ;
	 if ((const T&)(tmp_next->value()) == item) {
	    if (tmp_next == tail_node){
	       tail_node = tmp;
	    }
	    tmp->set_next(tmp_next->next()) ;
            num_items -= 1;
	    delete tmp_next ;
	    return ;
	 }
      }
   }
}



template<class T>
void List<T>::push(const T& item)
{
  add_head(item);
}

template<class T>
T List<T>::pop()
{
  assert(head_node != NULL) ;

  Node<T>* tmp = head_node ;
  T item = tmp->value();
  if (tmp == tail_node) {
    tail_node = NULL ;
  }
  head_node = head_node->next() ;
  num_items -= 1;
  delete tmp ;
  return item;
}

template<class T>
unsigned List<T>::size() const {
  return num_items;
}

template<class T>
bool List<T>::is_empty() const
{
   if(head_node == 0) return(true);
   return(false);
}

template<class T>
void List<T>::clear()
{
   Node<T>* tmp ;
   while(head_node != 0) {
      tmp = head_node ;
      head_node = head_node->next() ;
      num_items -= 1;
      delete tmp ;
   }
   tail_node = 0 ;
   assert (num_items == 0); // sanity check
}

template<class T>
T List<T>::head() const
{
   return(head_node->value()) ;
}

template<class T>
T List<T>::tail() const
{
   return(tail_node->value()) ;
}

// SAM 9-2000, append all elements of l to the current list
template<class T>
List<T>& List<T>::append(const List<T>& l)
{
   for (List_iterator<T> lit(l) ; lit != 0 ; ++lit) {
      add_tail(*lit) ;
   }

   return (*this);
}

/* ----------------- << for Lists ----------------------------------------- */
template <class T> ostream& operator<<(ostream& os, List<T>& list)
{
  os << "List=[ " ;
  for(List_iterator<T> iter(list); iter != 0; ){
    os << *iter;
    iter++;
    if (iter != 0) os << ", ";
  }
  os << " ]" << endl;
  return os;
}


/*---------------------------------------------------------------------------
   List iterator class implementation
-----------------------------------------------------------------------------*/

template<class T>
List_iterator<T>::List_iterator() 
{
   iterator = 0 ;
}

template<class T>
List_iterator<T>::List_iterator(const List<T>& list)
{
   iterator = list.head_node ;
}

template<class T>
List_iterator<T>::List_iterator(const List_iterator<T>& tli)
   : iterator(tli.iterator)
{
}

template<class T>
List_iterator<T>& List_iterator<T>::operator=(const List_iterator<T>& tli)
{
   iterator = tli.iterator ;
    return *this; //return to make CC happy
} 

template<class T>
void List_iterator<T>::operator()(const List<T>& list)
{
   iterator = list.head_node;
}

template<class T>
List_iterator<T>::~List_iterator()
{
}

template<class T>
bool List_iterator<T>::operator==(const void* x) const
{
   return (x == (void*)(iterator)) ;
}

template<class T>
bool List_iterator<T>::operator!=(const void* x) const
{
   return (x != (void*)(iterator)) ;
}


template<class T>
void List_iterator<T>::operator++() 
{
   if(iterator != 0) iterator = iterator->next() ;
   return ;
}

template<class T>
void List_iterator<T>::operator++(int) 
{
   if(iterator != 0) iterator = iterator->next() ;
   return ;
}

template<class T>
T& List_iterator<T>::operator*() 
{
   if(iterator == 0) {
      assert(0) ;
   }
   return(iterator->value()) ;
}

template<class T>
Node<T>* List_iterator<T>::get_internal_node() 
{
   if(iterator == 0) {
      assert(0) ;
   }
   return(iterator) ;
}


/*----------------------------------------------------------------------------
    List_filterator Implementation
-----------------------------------------------------------------------------*/

template<class T>
List_filterator<T>::List_filterator()
   : flag(false)
{}

template<class T>
List_filterator<T>::List_filterator(const List<T>& s, const Filter<T>* filter,
				    bool delete_flag)
	:List_iterator<T>(s), rep(filter), flag(delete_flag) {

  while(!(List_iterator<T>::operator==(0) ||
	rep->filter(List_iterator<T>::operator*())))
    List_iterator<T>::operator++();
}

template<class T>
void List_filterator<T>::operator()(const List<T>& s, const Filter<T>* filter,
				    bool delete_flag)

{
  rep = filter;
  flag = delete_flag;
  List_iterator<T>::operator()(s);
  while(!(List_iterator<T>::operator==(0) ||
	rep->filter(List_iterator<T>::operator*())))
    List_iterator<T>::operator++();
}

template<class T>
List_filterator<T>::~List_filterator()
{
  if (flag) delete ((Filter<T>*) rep);
}

template<class T>
void List_filterator<T>::operator++() 
{
  do List_iterator<T>::operator++();
  while(!(List_iterator<T>::operator==(0) ||
	rep->filter(List_iterator<T>::operator*())));
  return ;
}

template<class T>
void List_filterator<T>::operator++(int) 
{
   do List_iterator<T>::operator++();
   while(!(List_iterator<T>::operator==(0) ||
	 rep->filter(List_iterator<T>::operator*())));
   return ;
}


// Unsupported methods 
template<class T>
List_filterator<T>::List_filterator(const List_filterator<T>&)
{
    assert(0); 
}

template<class T>
List_filterator<T>& List_filterator<T>::operator=(const List_filterator<T>&)
{
    assert(0); 
    return *this; //return to make CC happy
} 

