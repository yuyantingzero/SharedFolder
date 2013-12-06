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
//      File:           hash_map.h
//      Authors:        Sadun Anik, Shail Aditya
//      Created:        December 1994
//      Description:    Hash map and its iterators
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _HASH_MAP_H
#define _HASH_MAP_H

#include <iostream>
#include "defs.h"
#include "filter.h"
#include "map.h"
#include "hash_table_ofp.h"

using namespace std;

template<class N, class V> class Hash_map ;
template<class N, class V> class Hash_map_iterator ;

/* ----------------------------------------------------------------------------
   Hash_map class: Hash_maps require copy constructor and == on N, and null and 
              copy constructor on V  
---------------------------------------------------------------------------- */

template<class N, class V>
class Hash_map{
   friend class Hash_map_iterator<N,V>;
private:
   Hash_map();
public:
   Hash_map(const Hash_map<N, V>&);
   Hash_map(unsigned (*hf)(N&), const int size = 1);   
   ~Hash_map();

   Hash_map<N, V>& operator=(const Hash_map<N, V>&);
   bool operator==(const Hash_map<N, V>&) const;
   bool operator!=(const Hash_map<N, V>&) const;
  
   bool is_empty() const;	
   unsigned size() const ;
   Hash_map<N,V>& clear();

   Hash_map<N,V>& bind(const N&,const V&);
   Hash_map<N,V>& unbind(const N&);
   V& value(const N&) const;    // changed return value to V& from V  (S.A.)
   inline V& operator[] (const N& index) const {
     return value(index);
   }
   bool is_bound(const N&) const;
   N& key(const N&) const;
   Pair<N,V>& entry(const N&) const;
   Pair<N,V>* find_entry(const N&) ;

private:
   void copy(const Hash_map<N, V>&);
   Hash_table<void* > tuple_table;
   unsigned (*hash_func)(N&) ;

  // automatic resizing of the hash tables -- Shail Aditya 02/98
   void expand();
   void contract();
   void rehash(int new_size);
};

/* ----------------- << for Hash_maps ----------------------------------------- */
template <class N, class V> ostream& operator<<(ostream&, Hash_map<N, V>&) ;


/* ----------------------------------------------------------------------------
   Map_iterator class
---------------------------------------------------------------------------- */

template<class N, class V>
class Hash_map_iterator{
public:
   Hash_map_iterator();
   Hash_map_iterator(const Hash_map<N,V>& );
   void operator()(const Hash_map<N,V>& );
   ~Hash_map_iterator();

   bool operator==(const int) const ;
   bool operator!=(const int) const ;

   void operator++();
   void operator++(int); // int parameter signals postfix
   Pair<N, V>& operator*() ; 

// ----- To let the user know that these are not supported. They do assert(0).
   Hash_map_iterator(const Hash_map_iterator<N, V>&);
   Hash_map_iterator<N, V>& operator=(const Hash_map_iterator<N, V>&);

private:
   Hash_table_iterator<void* > iter;
};


/* ---------------------------------------------------------------------------
   Map_filterator: Map iteration with filtering
   Methods not defined here are inherited from Map_iterator
---------------------------------------------------------------------------- */

template<class N, class V>
class Hash_map_filterator : public Hash_map_iterator<N, V> {
  public:
   Hash_map_filterator();
   Hash_map_filterator(const Hash_map<N, V>& s, const Filter<Pair<N, V> >* filter);
   void operator()(const Hash_map<N, V>& s, const Filter<Pair<N, V> >* filter);
   ~Hash_map_filterator();

   void operator++() ;
   void operator++(int) ;

// ----- To let the user know that these are not supported. They do assert(0).
   Hash_map_filterator(const Hash_map_filterator<N, V>&);
   Hash_map_filterator<N, V>& operator=(const Hash_map_filterator<N, V>&);

  private:
   const Filter<Pair<N, V> > *rep;
} ;



/* ----------------------------------------------------------------------------
   Map_domain_iterator class
---------------------------------------------------------------------------- */

template<class N, class V>
class Hash_map_domain_iterator: public Hash_map_iterator<N, V>{
public:
  Hash_map_domain_iterator();
  Hash_map_domain_iterator(const Hash_map<N,V>& );
  void operator()(const Hash_map<N,V>& );
  ~Hash_map_domain_iterator();

  N& operator*() ; 

// ----- To let the user know that these are not supported. They do assert(0).
   Hash_map_domain_iterator(const Hash_map_domain_iterator<N, V>&);
   Hash_map_domain_iterator<N, V>& operator=(const Hash_map_domain_iterator<N, V>&);
};

#endif

