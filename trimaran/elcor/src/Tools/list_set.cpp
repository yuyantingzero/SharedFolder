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
//      File:           list_set.cpp, Vinod Kathail
//      Authors:        Sadun Anik
//      Created:        December 1994
//      Description:    List based set declaration
//
/////////////////////////////////////////////////////////////////////////////

#include "list_set.h"

/*----------------------------------------------------------------------------
    List_set Implementation
-----------------------------------------------------------------------------*/


template<class T>
List_set<T>::List_set() 
{
   body = new List<T>();
   max_size = body->size();
} 

template<class T>
List_set<T>::List_set(const List_set<T>& s) 
{
   body = new List<T>(*(s.body));
   max_size = body->size();
}

template<class T>
List_set<T>::List_set(const List<T>& l) 
{
   body = new List<T>(l);
   max_size = body->size();
}

template<class T>
List_set<T>::List_set(const T& item)
{
   body = new List<T>() ;
   body->add_tail(item) ;
   max_size = body->size();
}
   
template<class T>
List_set<T>::~List_set() 
{
   if (max_size > 400) {
      El_warn("Yo dood, you made a list_set with %d elements!",max_size) ;
   }
   clear() ;
   delete(body) ;
}
      
template<class T>
List_set<T>& List_set<T>::operator=(const List_set<T>& s)
{
   if (body != s.body)
   {
      clear();
      *body = *(s.body) ;
      if (body->size() > max_size) max_size = body->size();
   }
   return *this ;
}

template<class T>
bool List_set<T>::operator==(const List_set<T>& s) const
{
  if (size() != s.size()) return false;

  for (List_set_iterator<T> iter(s); iter != 0; iter++) {
    if (!is_member(*iter))
      return false;
  }
  return true;
  
//   List_set<T> tmps = *this ;
//   if(tmps.size() != s.size()) return false; // mss 5-94
//   tmps -= s ;
//   if(tmps.is_empty()) {
//      return true ;
//   } else {
//      return false;
//   }
}

template<class T>
bool List_set<T>::operator!=(const List_set<T>& s) const
{
   return !(*this==s) ;
}

template<class T>
List_set<T>& List_set<T>::clear()
{
   body->clear() ;
   return *this ;
}

template<class T>
List_set<T>& List_set<T>::operator+=(const T& item)
{
   if (!is_member(item)) {
      body->add_tail(item) ;  
      if (body->size() > max_size) max_size = body->size();
   }
   return *this ;
}
   
template<class T>
List_set<T>& List_set<T>::operator-=(const T& item)
{
   body->remove(item) ;  
   return *this ;
}
   

template<class T>
List_set<T>& List_set<T>::operator+=(const List_set<T>& s)
{
   for (List_set_iterator<T> sit(s) ; sit != 0 ; ++sit) {
      operator+= (*sit) ;
   }
   return *this ;
}

template<class T>
List_set<T>& List_set<T>::operator-=(const List_set<T>& s)
{
   for (List_set_iterator<T> sit(s) ; sit != 0 ; ++sit) {
      if (is_member(*sit)) {
      	operator-=(*sit) ;
      }
   }
   return *this ;
}

template<class T>
List_set<T>& List_set<T>::operator*=(const List_set<T>& s)
{
   List_set<T> tmpset(*this) ;
   for (List_set_iterator<T> sit(tmpset) ; sit != 0 ; ++sit) {
      if (!s.is_member(*sit)) {
      	operator-=(*sit) ;
      }
   }
   return *this ;
}


template<class T>
unsigned List_set<T>::size() const
{
   return(body->size()) ;
}

template<class T>
bool List_set<T>::is_empty() const
{
   return(body->is_empty()) ;
}

template<class T>
bool List_set<T>::is_member(const T& item) const
{
   return(body->is_member(item)) ;
}

template<class T>
bool List_set<T>::is_subset(const List_set<T>& s) const 
{
  for (List_set_iterator<T> iter(s); iter != 0; iter++) {
    if (!is_member(*iter))
      return false;
  }
  return true;
  
//   List_set<T> tmps(*this) ;
//   tmps += s ;
//   return(tmps == *this) ;
}

template<class T>
bool List_set<T>::is_proper_subset(const List_set<T>& s) const
{
   if(size() == s.size()) return(0) ;
   return(is_subset(s)) ;
}


template<class T>
bool List_set<T>::intersects(const List_set<T>& s) const
{
  for(List_set_iterator<T> iter(s); iter != 0; iter++) {
    if(is_member(*iter))
      return true;
  }
  return false;
}


template<class T>
T  List_set<T>::pop(){ // gets an element from the set and removes it
   return(body->pop());
}

template<class T>
T  List_set<T>::head() const { // gets an element from the set 
   return(body->head());
}

template<class T>
T* List_set<T>::find(const T& item){
   for(List_set_iterator<T> iter(*this); iter!=0; ++iter){
      if(item==*iter) return(&*iter); // return pointer to item;
   }
   return(0);
}

template<class T>
T* List_set<T>::find_and_add(const T& item){
   T* tmp_item ;
   tmp_item = find(item) ;
   if (tmp_item == 0) {
      operator+= (item) ;
      return(find(item));  // this locate() should be very fast 
   } else {
      return(tmp_item) ;
   }
}


////////////////////////////////////////////////////////////////////
//  Implementation of << for List_set class
////////////////////////////////////////////////////////////////////   

template<class T>
ostream& operator <<(ostream& os, const List_set<T>& s)
{
   os << "List_set={ " ;
   for (List_set_iterator<T> sit(s) ; sit != 0 ; ) {
     os << *sit;
     sit++;
     if (sit != 0) os << ", ";
   }
   os << " }" << endl;
   return os ;
}



/*----------------------------------------------------------------------------
    List_set_iterator Implementation
-----------------------------------------------------------------------------*/

template<class T>
List_set_iterator<T>::List_set_iterator() 
{
}

template<class T>
List_set_iterator<T>::List_set_iterator(const List_set<T>& s)
   : iterator(*(s.body))
{
}

template<class T>
void List_set_iterator<T>::operator()(const List_set<T>& s)
{	
  iterator(*(s.body)) ; // Explicitly initialize the list iterator
}


template<class T>
List_set_iterator<T>::~List_set_iterator()
{
}

template<class T>
bool List_set_iterator<T>::operator==(const void* x) const
{
   return (iterator == x) ;
}

template<class T>
bool List_set_iterator<T>::operator!=(const void* x) const
{
   return (iterator != x) ;
}

template<class T>
bool List_set_iterator<T>::operator==(const List_set_iterator<T>& x) const
{
  // who names these variables?
   return (iterator.iterator == x.iterator.iterator) ;
}

template<class T>
bool List_set_iterator<T>::operator!=(const List_set_iterator<T>& x) const
{
   return !((*this) == x) ;
}

template<class T>
void List_set_iterator<T>::operator++() 
{
   ++iterator;
   return ;
}

template<class T>
void List_set_iterator<T>::operator++(int) 
{
   iterator++ ;
   return ;
}


template<class T>
T& List_set_iterator<T>::operator*() 
{
   return(*iterator) ;
}

// Unsupported methods

template<class T>
List_set_iterator<T>::List_set_iterator(const List_set_iterator<T>&)
{
    assert(0); 
}

template<class T>
List_set_iterator<T>& List_set_iterator<T>::operator=(const List_set_iterator<T>& x) 
{ 
  iterator.iterator = x.iterator.iterator;
  return *this;

  //    assert(0); 
  //  return *this; //return to make CC happy
}

/*----------------------------------------------------------------------------
    List_set_filterator Implementation
-----------------------------------------------------------------------------*/

template<class T>
List_set_filterator<T>::List_set_filterator()
   : flag(false)
{}

template<class T>
List_set_filterator<T>::List_set_filterator(const List_set<T>& s, const Filter<T>* filter,
				  bool delete_flag)
	:List_set_iterator<T>(s), rep(filter), flag(delete_flag){

  while(!(List_set_iterator<T>::operator==(0) ||
	rep->filter(List_set_iterator<T>::operator*())))
    List_set_iterator<T>::operator++();
}

template<class T>
void List_set_filterator<T>::operator()(const List_set<T>& s, const Filter<T>* filter,
				   bool delete_flag)
{
  rep = filter;
  flag = delete_flag;
  List_set_iterator<T>::operator()(s);
  while(!(List_set_iterator<T>::operator==(0) ||
	rep->filter(List_set_iterator<T>::operator*())))
    List_set_iterator<T>::operator++();
}

template<class T>
List_set_filterator<T>::~List_set_filterator()
{
  if (flag) delete ((Filter<T>*) rep); // mss 12-16-94
}

template<class T>
void List_set_filterator<T>::operator++() 
{
  do List_set_iterator<T>::operator++();
  while(!(List_set_iterator<T>::operator==(0) ||
	rep->filter(List_set_iterator<T>::operator*())));
  return ;
}

template<class T>
void List_set_filterator<T>::operator++(int) 
{
   do List_set_iterator<T>::operator++();
   while(!(List_set_iterator<T>::operator==(0) ||
	 rep->filter(List_set_iterator<T>::operator*())));
   return ;
}

// Unsupported methods

template<class T>
List_set_filterator<T>::List_set_filterator(const List_set_filterator<T>&)
{
    assert(0); 
}

template<class T>
List_set_filterator<T>& List_set_filterator<T>::operator=(const List_set_filterator<T>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}
