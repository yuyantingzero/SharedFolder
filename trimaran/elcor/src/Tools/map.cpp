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
//      File:           map.cpp
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson
//      Created:        December 1994
//      Description:    List based map 
//
/////////////////////////////////////////////////////////////////////////////
#include "stdlib.h"
#include "map.h"
#include "stdlib.h"

/* ---------------------------------------------------------------------------
   Mtuple class implementation
---------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------
   Map class implementation
---------------------------------------------------------------------------- */

template<class N, class V>
Map<N,V>::Map(){}

template<class N, class V>
Map<N, V>::Map(const Map<N, V>& map)
{
   // Do a direct copy; don't call copy
   for(Listofp_iterator<Pair<N,V>* > iter(map.tuple_list) ; iter!=0 ; iter++) {
      Pair<N,V>* new_pair = new Pair<N,V>(*(*iter)) ;
      tuple_list.add_tail(new_pair) ;
   }
}

template<class N, class V>
Map<N,V>::~Map()
{
   clear() ;
}

template<class N, class V>
Map<N, V>& Map<N, V>::operator=(const Map<N, V>& map){
   if (this != &map) {
      copy(map) ; // does clear the old list
   }
  return (*this);
}


template<class N, class V>
bool Map<N, V>::operator==(const Map<N, V>& map) const {
   assert(0) ;
   return (tuple_list == map.tuple_list);
}
  
template<class N, class V>
bool Map<N, V>::operator!=(const Map<N, V>& map) const {
   assert(0) ;
  return (!(*this == map));
}

template<class N, class V>
unsigned Map<N,V>::size() const
{ 
  return tuple_list.size();
}

template<class N, class V>
bool Map<N,V>::is_empty() const
{ 
  if(tuple_list.is_empty()) return(true);
  return(false);
}

template<class N, class V>
Map<N,V>& Map<N,V>::clear()
{
   for(Listofp_iterator<Pair<N,V>* > iter(tuple_list) ; iter!=0 ; iter++) {
      delete (*iter) ;
   }
   tuple_list.clear(); 
   return *this;
}

template<class N, class V>
void Map<N,V>::copy(const Map<N, V>& map)
{
   clear() ;
   for(Listofp_iterator<Pair<N,V>* > iter(map.tuple_list) ; iter!=0 ; iter++) {
      Pair<N,V>* new_pair = new Pair<N,V>(*(*iter)) ;
      tuple_list.add_tail(new_pair) ;
   }
}

template<class N, class V>
Map<N,V>& Map<N,V>::bind(const N& n, const V& v)
{
   for(Listofp_iterator<Pair<N,V>* > iter(tuple_list); iter!=0; ++iter){
      if(n==(*iter)->first) {
	 (*iter)->second = v;
	 return (*this) ;
      }
   }
   Pair<N,V>* new_pair = new Pair<N,V>(n,v) ;
   tuple_list.add_tail(new_pair) ;
   return(*this);
}

template<class N, class V>
Map<N,V>& Map<N,V>::unbind(const N& n)
{
   Pair<N,V>* tmp_node = 0 ;
   for(Listofp_iterator<Pair<N,V>* > iter(tuple_list); iter!=0; ++iter){
      if(n==(*iter)->first) {
	 tmp_node = (*iter) ;
	 break ;
      }
   }
   if (tmp_node) {
      tuple_list.remove(tmp_node) ;
      delete tmp_node ;
   }
   return (*this) ;
}

template<class N, class V>
V& Map<N,V>::value(const N& r) const
{
   for(Listofp_iterator<Pair<N,V>* > iter(tuple_list); iter!=0; ++iter){
      if(r==(*iter)->first) return((*iter)->second); // return reference to item;
   }
   assert(0) ;
//    // Just to get rid of the compiler warning.
//    Pair<N,V> junk;
//    return (junk.second);

   // Actually, the above now _causes_ a compiler warning.  Try the following:
   return *(new V);
}

template<class N, class V>
bool Map<N,V>::is_bound(const N& r) const
{
   for(Listofp_iterator<Pair<N,V>* > iter(tuple_list); iter!=0; ++iter){
      if(r==(*iter)->first) return(true); // return pointer to item;
   }
   return(false);
}

/* ----------------- << for maps ----------------------------------------- */
template <class N, class V> ostream& operator<<(ostream& os, Map<N, V>& map)
{
  os << "Map={ " ;
  for(Map_iterator<N, V> iter(map); iter != 0; ){
    os << (*iter);
    iter++;
    if (iter != 0) os << "," << endl;
  }
  os << " }" << endl ;
  return os;
}

/* ---------------------------------------------------------------------------
   Map_iterator class implementation
---------------------------------------------------------------------------- */

template<class N, class V>
Map_iterator<N,V>::Map_iterator(){}

template<class N, class V>
Map_iterator<N,V>::Map_iterator( const Map<N,V>& m ) : iter(m.tuple_list){}

template<class N, class V>
void Map_iterator<N,V>::operator()( const Map<N,V>& m )
{
  iter(m.tuple_list);
}

template<class N, class V>
Map_iterator<N,V>::~Map_iterator(){}

template<class N, class V>
bool Map_iterator<N,V>::operator==(const int i) const {
  if((iter==0) && (i==0)) return(true);
  return(false);
}

template<class N, class V>
bool Map_iterator<N,V>::operator!=(const int i) const {
  if((iter!=0) && (i==0) ) return(true);
  return(false);
}

template<class N, class V>
void Map_iterator<N,V>::operator++(){
  iter++;
}

template<class N, class V>
void Map_iterator<N,V>::operator++(int) {// int signals postfix
  iter++;
}

template<class N, class V>
Pair<N, V>& Map_iterator<N,V>::operator*() { 
  return(*(*iter)); 
}

// Unsupported methods

template<class N, class V>
Map_iterator<N, V>::Map_iterator(const Map_iterator<N, V>&)
{
    assert(0);
}

template<class N, class V>
Map_iterator<N, V>& Map_iterator<N, V>::operator=(const Map_iterator<N, V>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}

/*----------------------------------------------------------------------------
    Map_filterator Implementation
-----------------------------------------------------------------------------*/

template<class N, class V>
Map_filterator<N, V>::Map_filterator() {}

template<class N, class V>
Map_filterator<N, V>::Map_filterator(const Map<N, V>& s, 
	                              const Filter<Pair<N, V> >* filter)
   :Map_iterator<N, V>(s), rep(filter)
{
  while(!(Map_iterator<N, V>::operator==(0) ||
	rep->filter(Map_iterator<N, V>::operator*())))
    Map_iterator<N, V>::operator++();
}

template<class N, class V>
void Map_filterator<N, V>::operator()(const Map<N, V>& s, 
				       const Filter<Pair<N, V> >* filter)
{
  rep = filter;
  Map_iterator<N, V>::operator()(s);
  while(!(Map_iterator<N, V>::operator==(0) ||
	rep->filter(Map_iterator<N, V>::operator*())))
    Map_iterator<N, V>::operator++();
}

template<class N, class V>
Map_filterator<N, V>::~Map_filterator(){}

template<class N, class V>
void Map_filterator<N, V>::operator++() 
{
  do Map_iterator<N, V>::operator++();
  while(!(Map_iterator<N, V>::operator==(0) ||
	rep->filter(Map_iterator<N, V>::operator*())));
  return ;
}

template<class N, class V>
void Map_filterator<N, V>::operator++(int) 
{
   do Map_iterator<N, V>::operator++();
   while(!(Map_iterator<N, V>::operator==(0) ||
	 rep->filter(Map_iterator<N, V>::operator*())));
   return ;
}

// Unsupported methods

template<class N, class V>
Map_filterator<N, V>::Map_filterator(const Map_filterator<N, V>&)
{
    assert(0);
}

template<class N, class V>
Map_filterator<N, V>& 
     Map_filterator<N, V>::operator=(const Map_filterator<N, V>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}

/* ---------------------------------------------------------------------------
   Map_domain_iterator class implementation
---------------------------------------------------------------------------- */

template<class N, class V>
Map_domain_iterator<N,V>::Map_domain_iterator(){}

template<class N, class V>
Map_domain_iterator<N,V>::Map_domain_iterator(const Map<N,V>& m )
                          :Map_iterator<N, V>(m) {}

template<class N, class V>
void Map_domain_iterator<N,V>::operator()(const Map<N,V>& m )
{
  Map_iterator<N, V>::operator()(m);
}

template<class N, class V>
Map_domain_iterator<N,V>::~Map_domain_iterator(){}

template<class N, class V>
N& Map_domain_iterator<N,V>::operator*() { 
  Pair<N, V>& temp  = Map_iterator<N, V>::operator*();
  return(temp.first);
}

// Unsupported methods

template<class N, class V>
Map_domain_iterator<N, V>::Map_domain_iterator(const
					       Map_domain_iterator<N, V>&)
{
    assert(0);
}

template<class N, class V>
Map_domain_iterator<N, V>& 
     Map_domain_iterator<N, V>::operator=(const Map_domain_iterator<N, V>&) 
{ 
    assert(0); 
    return *this; //return to make CC happy
}
	






