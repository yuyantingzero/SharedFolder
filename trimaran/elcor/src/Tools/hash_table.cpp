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
//      File:           hash_table.cpp
//      Authors:        Sadun Anik, Shail Aditya
//      Created:        December 1994
//      Description:    Hash table implementation
//
/////////////////////////////////////////////////////////////////////////////


#include "hash_table.h"
#include "hash_functions.h"

#define MAX_ALLOWED_BUCKET_LENGTH 200
#define AVG_ALLOWED_BUCKET_LOAD   3

////////////////////////////////////////////////////////////////
//    Hash_table utility functions
////////////////////////////////////////////////////////////////

static bool size_is_power_of_2(int size)
{ 
   if (size > 0 && (size & (size - 1)) == 0)
      return (true);
   else
      return (false);
}

////////////////////////////////////////////////////////////////
//    Hash_table Implementation
////////////////////////////////////////////////////////////////

template <class T>
Hash_table<T>::Hash_table()
{
   table_size = 1 ;
   power_2_table_size = true;
   hash_func = NULL ;
   max_bucket_length = 0 ;
   entry_count = 0 ;
   table_ptr = new Vector<List<T> > (table_size) ;
}

template<class T>
Hash_table<T>::Hash_table(const Hash_table<T>& h) 
{
   table_size = h.table_size ;
   power_2_table_size = h.power_2_table_size;
   hash_func = h.hash_func ;
   max_bucket_length = h.max_bucket_length ;
   entry_count = h.entry_count ;
   table_ptr = new Vector<List<T> >(*(h.table_ptr));
}

template <class T>
Hash_table<T>::Hash_table(unsigned (*hf)(T&) , const int size)
{
   table_size = size ;
   power_2_table_size = size_is_power_of_2(size);
   hash_func = hf ;
   max_bucket_length = 0 ;
   entry_count = 0 ;
   table_ptr = new Vector<List<T> > (table_size) ;
}


template <class T>
Hash_table<T>::~Hash_table()
{
  if ((max_bucket_length > MAX_ALLOWED_BUCKET_LENGTH)) {
    El_warn("This hashed structure had a table size %d for %d entries \n maximum bucket length was %d", 
	    table_size, entry_count, max_bucket_length) ;
  }
  delete table_ptr ;
}

template <class T>
Hash_table<T>& Hash_table<T>::operator=(const Hash_table<T>& h)
{
   if (table_ptr != h.table_ptr) {
      if (table_size != h.table_size) {
	 table_size = h.table_size ;
	 table_ptr->resize(table_size) ;
      }
      hash_func = h.hash_func ;
      clear() ;
      max_bucket_length = h.max_bucket_length ;
      entry_count = h.entry_count ;
      table_ptr->resize(table_size) ;
      power_2_table_size = size_is_power_of_2(table_size);
      for (int i=0 ; i<table_size ; ++i) {
         (*table_ptr)[i] = (*h.table_ptr)[i] ;
      }
   }
   return (*this) ;
}

template<class T>
bool Hash_table<T>::operator==(const Hash_table<T>& h) const
{
   if ((table_size == h.table_size) &&
       (hash_func == h.hash_func)) {

      for (int i=0 ; i<table_size ; ++i) {
	 if ((*table_ptr)[i] != (*h.table_ptr)[i]) return(false) ;
      }
      return(true) ;
   }
   return(false) ;
}

template<class T>
bool Hash_table<T>::operator!=(const Hash_table<T>& h) const
{
   if (*this == h) return(false) ;
   else return(true) ;
}

template<class T>
bool Hash_table<T>::is_member(const T& item) const
{
   if(table_size == 0)
      return false;
   if (hash_func) {
      int bucket;
      if (power_2_table_size)
        bucket = hash_func((T&) item) & (table_size-1) ;
      else
        bucket = hash_func((T&) item) % table_size ;
      return((*table_ptr)[bucket].is_member(item)) ;
   }
   else {
      return((*table_ptr)[0].is_member(item)) ;
   }
}

template<class T>
void Hash_table<T>::add_head(const T& item) 
{
   if (hash_func) {
     // expand the table if load factor exceeded -- Shail Aditya 02/98
      if (entry_count >= AVG_ALLOWED_BUCKET_LOAD*table_size) expand();
      int bucket;
      if (power_2_table_size)
        bucket = hash_func((T&) item) & (table_size-1) ;
      else
        bucket = hash_func((T&) item) % table_size ;
      List<T>& the_list = (*table_ptr)[bucket] ;
      the_list.add_head(item) ;
      if (the_list.size() > max_bucket_length) max_bucket_length = the_list.size() ;
      entry_count++ ;
   }
   else {
      El_warn("This hashed structure has no hash function!  All objects hash to 0.");
      List<T>& the_list = (*table_ptr)[0] ;
      the_list.add_head(item) ;
      if (the_list.size() > max_bucket_length) max_bucket_length = the_list.size() ;
      entry_count++ ;
   }
}

template<class T>
void Hash_table<T>::add_tail(const T& item) 
{
   if (hash_func) {
     // expand the table if load factor exceeded -- Shail Aditya 02/98
      if (entry_count >= AVG_ALLOWED_BUCKET_LOAD*table_size) expand();
      int bucket;
      if (power_2_table_size)
        bucket = hash_func((T&) item) & (table_size-1) ;
      else
        bucket = hash_func((T&) item) % table_size ;
      List<T>& the_list = (*table_ptr)[bucket] ;
      the_list.add_tail(item) ;
      if (the_list.size() > max_bucket_length) max_bucket_length = the_list.size() ;
      entry_count++ ;
   }
   else {
      El_warn("This hashed structure has no hash function!  All objects hash to 0.");
      List<T>& the_list = (*table_ptr)[0] ;
      the_list.add_tail(item) ;
      if (the_list.size() > max_bucket_length) max_bucket_length = the_list.size() ;
      entry_count++ ;
   }
}

template<class T>
void Hash_table<T>::remove(const T& item) 
{
   if (hash_func) {
     // contract the table if load factor < 1 -- Shail Aditya 02/98
      if (entry_count <= table_size) contract();
      int bucket;
      if (power_2_table_size)
        bucket = hash_func((T&) item) & (table_size-1) ;
      else
        bucket = hash_func((T&) item) % table_size ;
      (*table_ptr)[bucket].remove(item) ;
      entry_count-- ;
   }
   else {
      (*table_ptr)[0].remove(item) ;
      entry_count-- ;
   }
}

template<class T>
unsigned Hash_table<T>::size() const 
{
  // ntclark
  return entry_count;

//    unsigned l=0 ;
//    for (int i=0 ; i<table_size ; ++i) {
//       l += (*table_ptr)[i].size() ;
//    }
//    assert(l == (unsigned) entry_count) ; // sanity check
//    return(l) ;
}

template<class T>
bool Hash_table<T>::is_empty() const
{
  // ntclark
  return (entry_count == 0);

//    for (int i=0 ; i<table_size ; ++i) {
//       if(!((*table_ptr)[i].is_empty())) return(false) ;
//    }
//    assert (entry_count == 0) ;
//    return(true) ;
}

template<class T>
void Hash_table<T>::clear()
{
   for (int i=0 ; i<table_size ; ++i) {
      (*table_ptr)[i].clear() ;
   }
   if ((max_bucket_length > MAX_ALLOWED_BUCKET_LENGTH)) {
      El_warn("This hashed structure had a table size %d for %d entries \n maximum bucket length was %d", 
         table_size, entry_count, max_bucket_length) ;
   }
   entry_count = 0 ;
   max_bucket_length = 0 ;
}

template<class T>
void Hash_table<T>::expand()
{
  int i;
  // search for next larger size
  for (i=0; i<NUMPRIMES; i++) {
    if (primes[i] > entry_count) break;
  }
  if (entry_count >= primes[NUMPRIMES-1]) return;

  rehash(primes[i]);
}

template<class T>
void Hash_table<T>::contract()
{
  int i;
  // search for next smaller size
  for (i=NUMPRIMES-1; i>=0; i--) {
    if (primes[i] < table_size) break;
  }
  if (table_size <= primes[0]) return;

  rehash(primes[i]);
}

template<class T>
void Hash_table<T>::rehash(int new_size)
{
  int old_entry_count = entry_count;
  Vector<List<T> >* old_table_ptr = table_ptr;

  entry_count = 0;
  max_bucket_length = 0;
  table_size = new_size;
  power_2_table_size = size_is_power_of_2(new_size);
  table_ptr = new Vector<List<T> > (new_size) ;
  for (unsigned i=0; i<old_table_ptr->size(); i++) {
    for (List_iterator<T> li((*old_table_ptr)[i]); li!=0; li++) {
      add_tail(*li);
    }
    (*old_table_ptr)[i].clear();
  }
  assert (old_entry_count == entry_count); // sanity check
  delete old_table_ptr;
}

////////////////////////////////////////////////////////////////////
//  Implementation of << for Hash_table class
////////////////////////////////////////////////////////////////////   

template<class T>
ostream& operator <<(ostream& os, Hash_table<T>& s)
{
  os << "Hash_table=[ ";
   for (Hash_table_iterator<T> sit(s) ; sit != 0 ; ) {
      os << *sit ;
      sit++;
      if (sit != 0) os << ", ";
   }
   os << " ]" << endl ;
   return os ;
}

////////////////////////////////////////////////////////////////
//    Hash_table_iterator Implementation
////////////////////////////////////////////////////////////////

template<class T>
Hash_table_iterator<T>::Hash_table_iterator() 
{
}

template<class T>
Hash_table_iterator<T>::Hash_table_iterator(const Hash_table<T>& table)
   : table_ptr(table.table_ptr), ts(table.table_size)
{
   vit = 0 ;
   while (( vit < ts) && ((*table_ptr)[vit].is_empty())) vit++ ;
   if ( vit < ts ) {
      lit((*table_ptr)[vit]) ;
   }
}

template<class T>
void Hash_table_iterator<T>::operator()(const Hash_table<T>& table)
{
   table_ptr = table.table_ptr ;
   ts = table.table_size ;
   vit = 0 ;
   while (( vit < ts) && ((*table_ptr)[vit].is_empty())) vit++ ;
   if ( vit < ts) {
      lit((*table_ptr)[vit]) ;
   }
}


template<class T>
Hash_table_iterator<T>::~Hash_table_iterator()
{
}

template<class T>
bool Hash_table_iterator<T>::operator==(const void* x) const
{
   return(lit == x) ;
}


template<class T>
bool Hash_table_iterator<T>::operator!=(const void* x) const
{
   return(lit != x) ;
}

template<class T>
void Hash_table_iterator<T>::operator++() 
{
   if (lit != 0) lit++;
   if (lit == 0) {
      if (vit < ts ) {
	 vit++ ;
	 while (( vit < ts) && ((*table_ptr)[vit].is_empty())) vit++ ;
	 if (vit < ts){
	    lit((*table_ptr)[vit]) ;
	 }
      }
      return ;
   }
}

template<class T>
void Hash_table_iterator<T>::operator++(int) 
{
   operator++() ;
   return ;
}

template<class T>
T& Hash_table_iterator<T>::operator*() 
{
   if(lit == 0) {
      assert(0) ;
   }
   return(*lit) ;
}

// Unsupported methods

template<class T>
Hash_table_iterator<T>::Hash_table_iterator(const Hash_table_iterator<T>&) 
{
    assert(0);
}

template<class T>
Hash_table_iterator<T>&
       Hash_table_iterator<T>::operator=(const Hash_table_iterator<T>&)
{
    assert(0); 
    return *this; //return to make CC happy
}


/*----------------------------------------------------------------------------
    Hash_table_filterator Implementation
-----------------------------------------------------------------------------*/

template<class T>
Hash_table_filterator<T>::Hash_table_filterator() {}

template<class T>
Hash_table_filterator<T>::Hash_table_filterator(const Hash_table<T>& s, 
						 const Filter<T>* filter)
	:Hash_table_iterator<T>(s), rep(filter) {
  while(!(Hash_table_iterator<T>::operator==(0) ||
	rep->filter(Hash_table_iterator<T>::operator*())))
    Hash_table_iterator<T>::operator++();
}

template<class T>
void Hash_table_filterator<T>::operator()(const Hash_table<T>& s, 
					   const Filter<T>* filter)
{
  rep = filter;
  Hash_table_iterator<T>::operator()(s);
  while(!(Hash_table_iterator<T>::operator==(0) ||
	rep->filter(Hash_table_iterator<T>::operator*())))
    Hash_table_iterator<T>::operator++();
}

template<class T>
Hash_table_filterator<T>::~Hash_table_filterator(){}

template<class T>
void Hash_table_filterator<T>::operator++() 
{
  do Hash_table_iterator<T>::operator++();
  while(!(Hash_table_iterator<T>::operator==(0) ||
	rep->filter(Hash_table_iterator<T>::operator*())));
  return ;
}

template<class T>
void Hash_table_filterator<T>::operator++(int) 
{
   do Hash_table_iterator<T>::operator++();
   while(!(Hash_table_iterator<T>::operator==(0) ||
	 rep->filter(Hash_table_iterator<T>::operator*())));
   return ;
}

// Unsupported methods

template<class T>
Hash_table_filterator<T>::Hash_table_filterator(const 
						Hash_table_filterator<T>&) 
{
    assert(0);
}

template<class T>
Hash_table_filterator<T>& 
    Hash_table_filterator<T>::operator=(const Hash_table_filterator<T>&)
{
    assert(0); 
    return *this; //return to make CC happy
}
