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
//      File:           hash_set.cpp
//      Authors:        Sadun Anik
//      Created:        December 1994
//      Description:    Hash map and its iterators
//
/////////////////////////////////////////////////////////////////////////////

#include "hash_set.h"
#include "hash_functions.h"

////////////////////////////////////////////////////////////////
//    Hash_set Implementation
////////////////////////////////////////////////////////////////


template<class T>
Hash_set<T>::Hash_set() 
{
   body = new Hash_table<T>();
   assert(body && "you're out of memory");
} 

template<class T>
Hash_set<T>::Hash_set(const Hash_set<T>& s) 
{
   body = new Hash_table<T>(*(s.body));
   assert(body && "you're out of memory");
}

template<class T>
Hash_set<T>::Hash_set(unsigned (*hf)(T&), const int size) 
{
   if (hf == NULL)
      El_warn("Creating Hash_set with no hash func! Everything will hash to 0.");
   body = new Hash_table<T>(hf,size) ;
   assert(body && "you're out of memory");
}
   
template<class T>
Hash_set<T>::~Hash_set() 
{
   clear() ;
   delete(body) ;
}
      
template<class T>
Hash_set<T>& Hash_set<T>::operator=(const Hash_set<T>& s)
{
   if (body != s.body)
   {
      clear();
      *body = *(s.body) ;
   }
   return *this ;
}

template<class T>
bool Hash_set<T>::operator==(const Hash_set<T>& s) const
{
   if (this->size() != s.size()) return false ; 

   for(Hash_set_iterator<T> hmi(s); hmi != 0; hmi++)
     if(!this->is_member(*hmi))
       return false;
   return true;
}

template<class T>
bool Hash_set<T>::operator!=(const Hash_set<T>& s) const
{
   return !(*this==s) ;
}

template<class T>
Hash_set<T>& Hash_set<T>::clear()
{
   body->clear() ;
   return *this ;
}

template<class T>
void Hash_set<T>::resize(int n)
{
  body->rehash(n);
}

template<class T>
Hash_set<T>& Hash_set<T>::operator+=(const T& item)
{
   if (!is_member(item)) 
   {
      body->add_tail(item) ;  
   }
   return *this ;
}
   
template<class T>
Hash_set<T>& Hash_set<T>::operator-=(const T& item)
{
   if (is_member(item)) {
      body->remove(item) ;  
   }
   return *this ;
}
   
template<class T>
Hash_set<T>& Hash_set<T>::operator+=(const Hash_set<T>& s)
{
   for (Hash_set_iterator<T> sit(s) ; sit != 0 ; ++sit) {
      operator+= (*sit) ;
   }
   return *this ;
}

template<class T>
Hash_set<T>& Hash_set<T>::operator-=(const Hash_set<T>& s)
{
   for (Hash_set_iterator<T> sit(s) ; sit != 0 ; ++sit) {
      if (is_member(*sit)) {
      	operator-=(*sit) ;
      }
   }
   return *this ;
}

template<class T>
Hash_set<T>& Hash_set<T>::operator*=(const Hash_set<T>& s)
{
   Hash_set<T> tmpset(*this) ;
   for (Hash_set_iterator<T> sit(tmpset) ; sit != 0 ; ++sit) {
      if (!s.is_member(*sit)) {
      	operator-=(*sit) ;
      }
   }
   return *this ;
}

template<class T>
unsigned Hash_set<T>::size() const
{
   return(body->size()) ;
}

template<class T>
bool Hash_set<T>::is_empty() const
{
   return(body->is_empty()) ;
}

template<class T>
bool Hash_set<T>::is_member(const T& item) const
{
   return(body->is_member(item)) ;
}

template<class T>
bool Hash_set<T>::intersects(const Hash_set<T>& h2) const
{
  if(h2.size() < size()) {
    for(Hash_set_iterator<T> hsi(h2); hsi != 0; hsi++) {
      if(is_member(*hsi))
	return true;
    }
  } else {
    for(Hash_set_iterator<T> hsi(*this); hsi != 0; hsi++) {
      if(h2.is_member(*hsi))
	return true;
    }
  }
  return false;
}

template<class T>
bool Hash_set<T>::is_subset(const Hash_set<T>& s) const 
{
  if(s.size() > size())
    return false;

  for(Hash_set_iterator<T> hmi(s); hmi != 0; hmi++)
    if(!this->is_member(*hmi))
      return false;
  return true;  

//    Hash_set<T> tmps(*this) ;
//    tmps += s ;
//    return(tmps == *this) ;
}

template<class T>
bool Hash_set<T>::is_proper_subset(const Hash_set<T>& s) const
{
   if(size() == s.size()) return(0) ;
   return(is_subset(s)) ;
}

template<class T>
T  Hash_set<T>::pop()        // gets an element from the set and removes it
{ 
   Hash_set_iterator<T> iter(*this);
   assert(iter!=0); // empty set;
   T  item = * iter;
   operator-=(item);
   return(item);
}

template<class T>
T  Hash_set<T>::head() const     // gets an element from the set and removes it
{ 
   Hash_set_iterator<T> iter(*this);
   assert(iter!=0); // empty set;
   T  item = * iter;
   return(item);
}

template<class T>
T* Hash_set<T>::find(const T& item)
{
   for(List_iterator<T> iter((*(body->table_ptr))
			     [body->hash_func((T&)item) % body->table_size]);
       iter!=0; ++iter){
      if(item==*iter) return(&*iter); // return pointer to item;
   }
   return(0);
}

template<class T>
T* Hash_set<T>::find_and_add(const T& item){
   T* tmp_item ;
   tmp_item = find(item) ;
   if (tmp_item == 0) {
      operator+= (item) ;
      return(find(item));  // this locate() should be very fast 
   } else {
      return(tmp_item) ;
   }
}

template<class T>
Hash_set<T>& Hash_set<T>::add_unsafe(const T& item){
   body->add_tail(item) ;
   return (*this) ;
}
////////////////////////////////////////////////////////////////////
//  Implementation of << for Hash_set class
////////////////////////////////////////////////////////////////////   

template <class T> ostream& operator<<(ostream& os, const Hash_set<T>& s)
{
  os << "Hash_set={ ";
   for (Hash_set_iterator<T> sit(s) ; sit != 0 ; ) {
      os << *sit ;
      sit++;
      if (sit != 0) os << ", ";
   }
   os << " }" << endl ;
   return os ;
}

/* ----------------------------------------------------------------------------
   Hash_set_iterator class implementation
---------------------------------------------------------------------------- */

template<class T>
Hash_set_iterator<T>::Hash_set_iterator() 
{
}

template<class T>
Hash_set_iterator<T>::Hash_set_iterator(const Hash_set<T>& s)
   : iterator(*(s.body))
{
}

template<class T>
void Hash_set_iterator<T>::operator()(const Hash_set<T>& s)
{
  iterator(*(s.body)) ;
}


template<class T>
Hash_set_iterator<T>::~Hash_set_iterator()
{
}

template<class T>
bool Hash_set_iterator<T>::operator==(const void* x) const
{
   return (iterator == x) ;
}

template<class T>
bool Hash_set_iterator<T>::operator!=(const void* x) const
{
   return (iterator != x) ;
}

template<class T>
void Hash_set_iterator<T>::operator++() 
{
   if(iterator != 0) iterator++ ;
   return ;
}

template<class T>
void Hash_set_iterator<T>::operator++(int) 
{
   if(iterator != 0) iterator++ ;
   return ;
}


template<class T>
T& Hash_set_iterator<T>::operator*() 
{
   if(iterator == 0) {
      assert(0) ;
   }
   return(*iterator) ;
}

// Unsupported methods

template<class T>
Hash_set_iterator<T>::Hash_set_iterator(const Hash_set_iterator<T>&)
{
    assert(0);
}

template<class T>
Hash_set_iterator<T>& Hash_set_iterator<T>::operator=(const 
						      Hash_set_iterator<T>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}

/*----------------------------------------------------------------------------
    Hash_set_filterator Implementation
-----------------------------------------------------------------------------*/

template<class T>
Hash_set_filterator<T>::Hash_set_filterator()
   : flag(false)
{}

template<class T>
Hash_set_filterator<T>::Hash_set_filterator(const Hash_set<T>& s, 
					     const Filter<T>* filter,
					     bool delete_flag)
	:Hash_set_iterator<T>(s), rep(filter), flag(delete_flag) {

  while(!(Hash_set_iterator<T>::operator==(0) ||
	rep->filter(Hash_set_iterator<T>::operator*())))
    Hash_set_iterator<T>::operator++();
}

template<class T>
void Hash_set_filterator<T>::operator()(const Hash_set<T>& s, 
					 const Filter<T>* filter,
					 bool delete_flag)
{
  rep = filter;
  flag = delete_flag;
  Hash_set_iterator<T>::operator()(s);
  while(!(Hash_set_iterator<T>::operator==(0) ||
	rep->filter(Hash_set_iterator<T>::operator*())))
    Hash_set_iterator<T>::operator++();
}

template<class T>
Hash_set_filterator<T>::~Hash_set_filterator()
{
  if (flag) delete ((Filter<T>*) rep);
}

template<class T>
void Hash_set_filterator<T>::operator++() 
{
  do Hash_set_iterator<T>::operator++();
  while(!(Hash_set_iterator<T>::operator==(0) ||
	rep->filter(Hash_set_iterator<T>::operator*())));
  return ;
}

template<class T>
void Hash_set_filterator<T>::operator++(int) 
{
   do Hash_set_iterator<T>::operator++();
   while(!(Hash_set_iterator<T>::operator==(0) ||
	 rep->filter(Hash_set_iterator<T>::operator*())));
   return ;
}


// Unsupported methods

template<class T>
Hash_set_filterator<T>::Hash_set_filterator(const Hash_set_filterator<T>&)
{
    assert(0);
}

template<class T>
Hash_set_filterator<T>& Hash_set_filterator<T>::operator=(const 
						      Hash_set_filterator<T>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}
