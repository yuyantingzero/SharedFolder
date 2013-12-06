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
//      File:           hash_map.cpp
//      Authors:        Sadun Anik, Shail Aditya
//      Created:        December 1994
//      Description:    Hash map and its iterators
//
/////////////////////////////////////////////////////////////////////////////

#include "hash_map.h"
#include "hash_functions.h"
#include "dbg.h"

#define MAX_ALLOWED_BUCKET_LENGTH 200
#define AVG_ALLOWED_BUCKET_LOAD   3

/* ---------------------------------------------------------------------------
   Hash_map class implementation
---------------------------------------------------------------------------- */

template<class N, class V>
Hash_map<N,V>::Hash_map()
   : hash_func(NULL)
{}

template<class N, class V>
Hash_map<N,V>::Hash_map(unsigned (*hf)(N&) , const int size)
   : tuple_table(NULL, size), hash_func(hf)
{
   if (hf == NULL)
      El_punt("Hash map must have a hash function.");
}

template<class N, class V>
Hash_map<N, V>::Hash_map(const Hash_map<N, V>& map)
  : tuple_table(NULL, map.tuple_table.table_size),
    hash_func(map.hash_func)
{
   copy(map) ;
}

template<class N, class V>
Hash_map<N,V>::~Hash_map()
{
   clear() ;
}

template<class N, class V>
Hash_map<N, V>& Hash_map<N, V>::operator=(const Hash_map<N, V>& map){
  if (this != &map) {
     copy(map) ; // clears the old hash table
  }
  return (*this);
}

template<class N, class V>
bool Hash_map<N, V>::operator==(const Hash_map<N, V>& map) const {
  return (tuple_table == map.tuple_table);
}
  
template<class N, class V>
bool Hash_map<N, V>::operator!=(const Hash_map<N, V>& map) const {
  return (!(*this == map));
}

template<class N, class V>
bool Hash_map<N,V>::is_empty() const {
  return tuple_table.is_empty();
}

template<class N, class V>
unsigned Hash_map<N,V>::size() const {
  return tuple_table.size();
}

template<class N, class V>
Hash_map<N,V>& Hash_map<N,V>::clear()
{
   for (Hash_table_iterator<void* > iter(tuple_table) ; iter!=0 ;) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      iter++ ;
      delete(tmp_ptr) ;
   }
   tuple_table.clear(); 
   return *this;
}

template<class N, class V>
void Hash_map<N,V>::copy(const Hash_map<N, V>& hm)
{
   clear();
   for (Hash_table_iterator<void* > iter(hm.tuple_table) ; iter!=0 ; iter++) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      Pair<N,V>* new_pair = new Pair<N,V>(*tmp_ptr) ;
      int bucket = hash_func(new_pair->first) % tuple_table.table_size ;
      (*tuple_table.table_ptr)[bucket].add_tail(new_pair) ;
      tuple_table.entry_count += 1;
   }
}

template<class N, class V>
Hash_map<N,V>& Hash_map<N,V>::bind(const N& n, const V& v){
   int bucket = hash_func((N&)n) % tuple_table.table_size ;
   for (List_iterator<void* > iter((*tuple_table.table_ptr)[bucket]) ;
	iter != 0 ; ++iter) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      if (tmp_ptr->first == n) {
	 tmp_ptr->second = v ;
	 return(*this);
      }
   }
   // expand the table if load factor exceeded -- Shail Aditya 02/98
   if (tuple_table.entry_count >= AVG_ALLOWED_BUCKET_LOAD*tuple_table.table_size) {
     expand();
     bucket = hash_func((N&)n) % tuple_table.table_size ;
   }
   Pair<N,V>* new_pair = new Pair<N,V>(n,v);
   (*tuple_table.table_ptr)[bucket].add_tail(new_pair) ;
   tuple_table.entry_count += 1;
   return(*this);
}

template<class N, class V>
Hash_map<N,V>& Hash_map<N,V>::unbind(const N& n){
   Pair<N,V>* prptr = NULL ;
   int bucket = hash_func((N&)n) % tuple_table.table_size ;
   for (List_iterator<void* > iter((*tuple_table.table_ptr)[bucket]) ;
	iter != 0 ; ++iter) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      if (tmp_ptr->first == n) {
	 prptr = tmp_ptr ;
	 break ;
      }
   }
   if (prptr != NULL) {                      // check if *prptr is safe to remove
     // contract the table if load factor < 1 -- Shail Aditya 02/98
      if (tuple_table.entry_count <= tuple_table.table_size) {
	contract();
	bucket = hash_func((N&)n) % tuple_table.table_size ;
      }
      (*tuple_table.table_ptr)[bucket].remove(prptr) ;
      tuple_table.entry_count -= 1;
      delete prptr ;
   }
   return(*this); // not currently bound - take no action
}

template<class N, class V>
V& Hash_map<N,V>::value(const N& n) const
{
   int bucket = hash_func((N&)n) % tuple_table.table_size ;
   for (List_iterator<void* > iter((*tuple_table.table_ptr)[bucket]) ;
	iter != 0 ; ++iter) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      if (tmp_ptr->first == n) {
	 return(tmp_ptr->second) ;
      }
   }
   El_punt("ERROR: could not find the value for your key in a Hash_map!\n");
   assert(0); // could not find the item
   V* dummy = 0 ;
   return (*dummy); // return the value component *** BOGUS ***
}

template<class N, class V>
bool Hash_map<N,V>::is_bound(const N& n) const {
   int bucket = hash_func((N&)n) % tuple_table.table_size ;
   for (List_iterator<void* > iter((*tuple_table.table_ptr)[bucket]) ;
	iter != 0 ; ++iter) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      if (tmp_ptr->first == n) {
	 return(true) ;
      }
   }
   return(false);
}

template<class N, class V>
N& Hash_map<N,V>::key(const N& n) const
{
   int bucket = hash_func((N&)n) % tuple_table.table_size ;
   for (List_iterator<void* > iter((*tuple_table.table_ptr)[bucket]) ;
	iter != 0 ; ++iter) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      if (tmp_ptr->first == n) {
	 return(tmp_ptr->first) ;
      }
   }
   assert(0); // could not find the item
   N* dummy = 0 ;
   return (*dummy); // return the value component *** BOGUS ***
}

template<class N, class V>
Pair<N,V>& Hash_map<N,V>::entry(const N& n) const
{
   int bucket = hash_func((N&)n) % tuple_table.table_size ;
   for (List_iterator<void* > iter((*tuple_table.table_ptr)[bucket]) ;
	iter != 0 ; ++iter) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      if (tmp_ptr->first == n) {
	 return(*tmp_ptr) ;
      }
   }
   assert(0); // could not find the item
   Pair<N,V>* dummy = 0 ;
   return (*dummy); // return the value component *** BOGUS ***
}

template<class N, class V>
Pair<N,V>* Hash_map<N,V>::find_entry(const N& n)
{
   int bucket = hash_func((N&)n) % tuple_table.table_size ;
   for (List_iterator<void* > iter((*tuple_table.table_ptr)[bucket]) ;
	iter != 0 ; ++iter) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
      if (tmp_ptr->first == n) {
	 return(tmp_ptr) ;
      }
   }
   return (NULL);
}

template<class N, class V>
void Hash_map<N,V>::expand()
{
  int i;
  // search for next larger size
  for (i=0; i<NUMPRIMES; i++) {
    if (primes[i] > tuple_table.entry_count) break;
  }
  if (tuple_table.entry_count >= primes[NUMPRIMES-1]) return;
  
  rehash(primes[i]);
}

template<class N, class V>
void Hash_map<N,V>::contract()
{
  int i;
  // search for next smaller size
  for (i=NUMPRIMES-1; i>=0; i--) {
    if (primes[i] < tuple_table.table_size) break;
  }
  if (tuple_table.table_size <= primes[0]) return;

  rehash(primes[i]);
}

template<class N, class V>
void Hash_map<N,V>::rehash(int new_size)
{
  int old_entry_count = tuple_table.entry_count;
  Vector<List<void*> >* old_table_ptr = tuple_table.table_ptr;

  tuple_table.entry_count = 0;
  tuple_table.max_bucket_length = 0;
  tuple_table.table_size = new_size;
  tuple_table.table_ptr = new Vector<List<void*> > (new_size) ;
  for (unsigned i=0; i<old_table_ptr->size(); i++) {
    for (List_iterator<void*> li((*old_table_ptr)[i]); li!=0; li++) {
      Pair<N,V>* tmp_ptr = (Pair<N,V>*) *li ;
      int bucket = hash_func(tmp_ptr->first) % tuple_table.table_size ;
      (*tuple_table.table_ptr)[bucket].add_tail(tmp_ptr) ;
      tuple_table.entry_count += 1;
    }
    (*old_table_ptr)[i].clear();
  }
  assert (old_entry_count == tuple_table.entry_count); // sanity check
  delete old_table_ptr;
}

/* ----------------- << for Hash_maps ----------------------------------------- */
template <class N, class V> ostream& operator<<(ostream& os, Hash_map<N, V>& map)
{
  os << "Hash_map={ " ;
  for(Hash_map_iterator<N, V> iter(map); iter != 0; ){
    os << *iter;
    iter++;
    if (iter != 0) os << ", ";
  }
  os << " }" << endl ;
  return os;
}

/* ---------------------------------------------------------------------------
   Hash_map_iterator class implementation
---------------------------------------------------------------------------- */

template<class N, class V>
Hash_map_iterator<N,V>::Hash_map_iterator(){}

template<class N, class V>
Hash_map_iterator<N,V>::Hash_map_iterator( const Hash_map<N,V>& m ) : iter(m.tuple_table){}

template<class N, class V>
void Hash_map_iterator<N,V>::operator()( const Hash_map<N,V>& m )
{
  iter(m.tuple_table);
}

template<class N, class V>
Hash_map_iterator<N,V>::~Hash_map_iterator(){}

template<class N, class V>
bool Hash_map_iterator<N,V>::operator==(const int i) const {
  if((iter==0) && (i==0)) return(true);
  return(false);
}

template<class N, class V>
bool Hash_map_iterator<N,V>::operator!=(const int i) const {
  if((iter!=0) && (i==0) ) return(true);
  return(false);
}

template<class N, class V>
void Hash_map_iterator<N,V>::operator++(){
  iter++;
}

template<class N, class V>
void Hash_map_iterator<N,V>::operator++(int) {// int signals postfix
  iter++;
}

template<class N, class V>
Pair<N, V>& Hash_map_iterator<N,V>::operator*() {
   Pair<N,V>* tmp_ptr = (Pair<N,V>*) *iter ;
   return(*tmp_ptr); 
}

// Unsupported methods

template<class N, class V>
Hash_map_iterator<N, V>::Hash_map_iterator(const Hash_map_iterator<N, V>&)
{
    assert(0);
}

template<class N, class V>
Hash_map_iterator<N, V>& Hash_map_iterator<N, V>::operator=(const Hash_map_iterator<N, V>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}

/*----------------------------------------------------------------------------
    Hash_map_filterator Implementation
-----------------------------------------------------------------------------*/

template<class N, class V>
Hash_map_filterator<N, V>::Hash_map_filterator() {}

template<class N, class V>
Hash_map_filterator<N, V>::Hash_map_filterator(const Hash_map<N, V>& s, 
	                              const Filter<Pair<N, V> >* filter)
   :Hash_map_iterator<N, V>(s), rep(filter)
{
  while(!(Hash_map_iterator<N, V>::operator==(0) ||
	rep->filter(Hash_map_iterator<N, V>::operator*())))
    Hash_map_iterator<N, V>::operator++();
}

template<class N, class V>
void Hash_map_filterator<N, V>::operator()(const Hash_map<N, V>& s, 
				       const Filter<Pair<N, V> >* filter)
{
  rep = filter;
  Hash_map_iterator<N, V>::operator()(s);
  while(!(Hash_map_iterator<N, V>::operator==(0) ||
	rep->filter(Hash_map_iterator<N, V>::operator*())))
    Hash_map_iterator<N, V>::operator++();
}

template<class N, class V>
Hash_map_filterator<N, V>::~Hash_map_filterator(){}

template<class N, class V>
void Hash_map_filterator<N, V>::operator++() 
{
  do Hash_map_iterator<N, V>::operator++();
  while(!(Hash_map_iterator<N, V>::operator==(0) ||
	rep->filter(Hash_map_iterator<N, V>::operator*())));
  return ;
}

template<class N, class V>
void Hash_map_filterator<N, V>::operator++(int) 
{
   do Hash_map_iterator<N, V>::operator++();
   while(!(Hash_map_iterator<N, V>::operator==(0) ||
	 rep->filter(Hash_map_iterator<N, V>::operator*())));
   return ;
}

// Unsupported methods

template<class N, class V>
Hash_map_filterator<N, V>::Hash_map_filterator(const Hash_map_filterator<N, V>&)
{
    assert(0);
}

template<class N, class V>
Hash_map_filterator<N, V>& 
     Hash_map_filterator<N, V>::operator=(const Hash_map_filterator<N, V>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}

/* ---------------------------------------------------------------------------
   Hash_map_domain_iterator class implementation
---------------------------------------------------------------------------- */

template<class N, class V>
Hash_map_domain_iterator<N,V>::Hash_map_domain_iterator(){}

template<class N, class V>
Hash_map_domain_iterator<N,V>::Hash_map_domain_iterator(const Hash_map<N,V>& m )
                          :Hash_map_iterator<N, V>(m) {}

template<class N, class V>
void Hash_map_domain_iterator<N,V>::operator()(const Hash_map<N,V>& m )
{
  Hash_map_iterator<N, V>::operator()(m);
}

template<class N, class V>
Hash_map_domain_iterator<N,V>::~Hash_map_domain_iterator(){}

template<class N, class V>
N& Hash_map_domain_iterator<N,V>::operator*() { 
  Pair<N, V>& temp  = Hash_map_iterator<N, V>::operator*();
  return(temp.first);
}

// Unsupported methods

template<class N, class V>
Hash_map_domain_iterator<N, V>::Hash_map_domain_iterator(const
					       Hash_map_domain_iterator<N, V>&)
{
    assert(0);
}

template<class N, class V>
Hash_map_domain_iterator<N, V>& 
     Hash_map_domain_iterator<N, V>::operator=(const Hash_map_domain_iterator<N, V>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}
	







