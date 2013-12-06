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
//      File:           dlist.h
//      Authors:        Mike Schlansker, Sadun Anik, Richard Johnson
//                      Vinod Kathail
//      Created:        December 1994
//      Description:    Doubly linked list
//
/////////////////////////////////////////////////////////////////////////////


#ifndef _DLIST_H
#define _DLIST_H 1

#include <iostream>
#include "defs.h"
#include "filter.h"

using namespace std;

template <class T> class Dlist; //Forward declarations
template <class T> class Dlist_iterator;

/*---------------------------------------------------------------------------
   Dnode class: Dlists are built of nodes
-----------------------------------------------------------------------------*/

template<class T>
class Dnode {
   friend class Dlist<T> ;
   friend class Dlist_iterator<T>;

public:
   Dnode();
   Dnode(const Dnode<T>&);
   Dnode(const T&);
   ~Dnode();

   T& value() ;

//private:
   T n1 ;
   Dnode<T>* next;
   Dnode<T>* prev;

};

/*---------------------------------------------------------------------------
   Dlist class: Doubly linked lists 
-----------------------------------------------------------------------------*/

template<class T>
class Dlist {
friend class Dlist_iterator<T> ;

public:
   Dlist();
   Dlist(const Dlist<T>&);
   ~Dlist();

   Dlist<T>& operator=(const Dlist<T>&);
   bool operator==(const Dlist<T>&) const;
   bool operator!=(const Dlist<T>&) const;

   void push(const T&) ; // push at front
   void push_tail(const T&); //push at back
   
   T pop() ; // pop from front
   T pop_tail(); // pop from back

   Dnode<T>* find(const T&) const;
   void insert_after(Dnode<T>*, const T&) ;
   void insert_before(Dnode<T>*, const T&) ;
   
   void remove(const T&) ;
   void remove(Dnode<T>*) ;

   bool is_member(const T&) const;
   unsigned size() const ;
   bool is_empty() const;
   void clear() ;
 
   T head() const; // value of front
   T tail() const; // value of back

   Dlist<T>& append(const Dlist<T>&);
	 
//   void drop_head();
//   void drop_tail();
  
//General nth methods -- Do we want these?
//  T& nth(int n) const;
//  Dlist<T>& remove(int n); //These modify the nth position 
//  Dlist<T>& insert(int n, T&);

// To be added later for full functionality of lists
//  Dlist<T>& cons(T&)
//  Dlist<T>& rest() const;
//  Dlist<T>& cat(T&) const;
//  Dlist<T>& butlast() const; 
//  Dlist<T>& nthcdr(int n);
//  Dlist<T>& nconc (Dlist<T>);
//  int member (T&, int n) const;

//private:
   Dnode<T>* head_node;
   Dnode<T>* tail_node;
};


/* ----------------- << for Dlists ----------------------------------------- */
template <class T> ostream& operator<<(ostream&, Dlist<T>&) ;


/*---------------------------------------------------------------------------
   Dlist_iterator: Bi-directional iterator for a doubly-linked list.
   The order of iteration is given by the list ordering. ++ moves in the 
   forward direction, -- in the reverse direction.
   The iteration can be started at:
   1. The beginning: This is the default if boolean input is not specified.
   2. At the end: Specify TRUE as the second argument.
   3. At any middle point: Specify the list element as the second argument
-----------------------------------------------------------------------------*/


template<class T>
class Dlist_iterator {
public:
   Dlist_iterator();
   Dlist_iterator(const Dlist<T>&, bool end = false) ;
   Dlist_iterator(const Dlist<T>&, const T&) ;  // start at T
   Dlist_iterator(Dnode<T> *) ; // start at Dnode<T> 
   void operator()(const Dlist<T>&, bool end = false) ;
   void operator()(const Dlist<T>&, const T&) ;
   void operator()(Dnode<T> *) ;
   virtual ~Dlist_iterator();

   bool operator==(const void*) const ;
   bool operator!=(const void*) const ;

   void operator++() ; 
   void operator++(int) ; // int parameter signals postfix

   void operator--() ; 
   void operator--(int) ; // int parameter signals postfix

   T& operator*() ;

// ----- To let the use know that these are not supported. They do assert(0)
   Dlist_iterator(const Dlist_iterator<T>&);
   Dlist_iterator<T>& operator=(const Dlist_iterator<T>&);

//private:
   Dnode<T>* iterator;
} ;



/*---------------------------------------------------------------------------
   Dlist_filterator: Forward iterator with filtering for a doubly-linked list.
   The iteration can be started only at the beginning. The order of iteration
   is given by the list ordering, and ++ moves in the forward direction. 
   Methods not defined here are inherited from the Dlist_iterator.
-----------------------------------------------------------------------------*/

template<class T>
class Dlist_filterator : public Dlist_iterator<T> {
public:
   Dlist_filterator();
   Dlist_filterator(const Dlist<T>& s, const Filter<T>* filter);
   void operator()(const Dlist<T>& s, const Filter<T>* filter);
   virtual ~Dlist_filterator();

   void operator++() ;
   void operator++(int) ;

// ----- To let the user know that these shouldn't be used. They do assert(0).
   Dlist_filterator(const Dlist_filterator<T>&);
   Dlist_filterator<T>& operator=(const Dlist_filterator<T>&);

private:
  const Filter<T> *rep;
} ;
 
#endif
