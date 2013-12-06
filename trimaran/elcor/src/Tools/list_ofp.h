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
//      File:           list_ofp.h
//      Authors:        Sadun Anik
//      Created:        June 1995
//      Description:    List of pointers to structures
//                      (declaration and definition)
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _LIST_OFP_H
#define _LIST_OFP_H
#include <iostream>
#include "defs.h"
#include "list.h"
#include "filter.h"

using namespace std;

template <class T> class Listofp_iterator; //Forward declaration
template <class T> class List_set_p; //Forward declaration


/*---------------------------------------------------------------------------
   List class
-----------------------------------------------------------------------------*/

template<class T>
class Listofp {
   friend class Listofp_iterator<T> ;
   friend class List_set_p<T> ;
public:
   inline Listofp() {};
   inline Listofp(const Listofp<T>& l)
      : body(l.body) {} ;
   inline ~Listofp() {} ;
   
   inline Listofp<T>& operator=(const Listofp<T>& l) {
      body = l.body ;
      return (*this) ;
   }
   inline bool operator==(const Listofp<T>& l) const {
      return (body == l.body) ;
   }
   inline bool operator!=(const Listofp<T>& l) const {
      return (body != l.body) ;
   }

//   bool isin(const T&) const;
   inline bool is_member(const T& t) const {
      return (body.is_member((void*)t)) ;
   }
	      

   inline void add_head(const T& t) { body.add_head((void*) t) ; }
   inline void add_tail(const T& t) { body.add_tail((void*) t) ; }
   
//   inline Node<T>* find(const T& t);
//   inline Node<T>* find_prev(const T& t);
//   inline void insert_after(Node<T>*, const T&) ;
   
   inline void remove(const T& t) { body.remove((void*) t); }

   inline void push(const T& t) { body.push((void*) t); }
   
   inline T pop() {
      return ((T) body.pop()) ;
   }
   
   inline unsigned size() const { return body.size() ;}
   inline bool is_empty() const { return body.is_empty() ;}
   inline void clear() { body.clear(); }
   
   inline T head() const { return ((T) body.head()) ; }
   inline T tail() const { return ((T) body.tail()) ; }

private:
   List<void*> body ;
};

// operator<< for Lists
// template <class T> ostream& operator<<(ostream&, List<T>&) ;


//
//   Listofp iterator class
//

template<class T>
class Listofp_iterator {
public:
   inline Listofp_iterator() {} 
   inline Listofp_iterator(const Listofp<T>& lofp ) : real_iter(lofp.body) {}
   inline Listofp_iterator(const Listofp_iterator<T>& lofp_i)
      : real_iter(lofp_i.real_iter) {} 
   inline Listofp_iterator<T>& operator=(const Listofp_iterator<T>& lofp_i) {
      real_iter = lofp_i.real_iter ;
      return (*this) ;
   }
   inline void operator()(const Listofp<T>& lofp) { real_iter(lofp.body) ; } 

   inline virtual ~Listofp_iterator() {} 

   inline bool operator==(const void* i) const { return (real_iter == i) ; }
   inline bool operator!=(const void* i) const { return (real_iter != i) ; }


   inline virtual void operator++() { ++real_iter ; }
   inline virtual void operator++(int) { real_iter++ ; }
   inline T& operator*() { return ((T&) (*real_iter)) ; }

private:
   List_iterator<void*> real_iter;
} ;


//
//   List_filterator: List iteration with filtering
//   Methods not defined here are inherited from List_iterator
//

template<class T>
class Listofp_filterator : public Listofp_iterator<T> {
public:
   Listofp_filterator();
   Listofp_filterator(const Listofp<T>& s, const Filter<T>* filter);
   void operator()(const Listofp<T>& s, const Filter<T>* filter);
   virtual ~Listofp_filterator();

   void operator++() ;
   void operator++(int) ;

// ----- To let the user know that these shouldn't be used. They do assert(0).
   Listofp_filterator(const Listofp_filterator<T>&);
   Listofp_filterator<T>& operator=(const Listofp_filterator<T>&);

private:
  const Filter<T> *rep;
} ;
 
#endif
