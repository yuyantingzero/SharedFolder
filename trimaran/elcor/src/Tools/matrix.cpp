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
//      File:           matrix.cpp
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    Matrix class
//
/////////////////////////////////////////////////////////////////////////////

// Matrices are defined as an array of vectors of items. 

// The follwing relation holds: 
//	typeof(m) == (pointer to) matrix
//	typeof(m[i]) == (pointer to) Vector
//	typeof(m[i][j]) == item (i: row index, j: column index)
// both array dimention are checked;

// assumed from the item class:
//	=, ==, << (print)

#include <stdlib.h>
#include "defs.h"
#include "vector.h"
#include "matrix.h"

template<class Item>
ostream& operator << (ostream& os, Matrix<Item>& m){
	int i, j;
	// char string[100];
	os << "- matrix={" << endl; /*indicate the type*/
	if ((m.rep_dim1==0 || m.rep_dim2==0) && m.rep_mat==NULL) {
		os << "NULL MATRIX" << endl;
	} else { 
		for(i=0; i< m.rep_dim1; i++) {
		  	for(j=0; j<m.rep_dim2; j++) os << m[i][j] << " "; 
			os << endl;

		}
	}
	os << "}" << endl;
return os;
}

template<class Item>
Matrix<Item>::Matrix() {   /*null constructor*/
	rep_dim1=0;
        rep_dim2=0;
	rep_mat=NULL;
}

template<class Item>
Matrix<Item>::Matrix(int dim1, int dim2)
{
	int i;

	rep_dim1=dim1;
	rep_dim2=dim2;
	if(rep_dim1 > 0)  {
		rep_mat = new Vector<Item>*[rep_dim1];
		assert(rep_mat!=NULL);
		for(i=0; i<dim1; i++) {
			rep_mat[i] = new Vector<Item>(dim2);
		}
	}
	else { rep_mat=NULL;}
}

template<class Item>
Matrix<Item>::Matrix(int dim1, int dim2, Item initial_val) {
	int i;
	rep_dim1=dim1;
        rep_dim2=dim2;
	if(rep_dim1 > 0)  {
                rep_mat = new Vector<Item>*[rep_dim1];
                assert(rep_mat!=NULL);
                for(i=0; i<dim1; i++) {
                        rep_mat[i] = new Vector<Item> (dim2, initial_val);
                }
	} else { rep_mat=NULL;}
}

template<class Item>
Matrix<Item>::Matrix(const Matrix<Item>& m ){ /* copy constructor*/
	int i;
	rep_dim1=m.rep_dim1;
	rep_dim2=m.rep_dim2;
	if(rep_dim1 > 0)  {
                rep_mat = new Vector<Item>*[rep_dim1];	
		assert(rep_mat!=NULL); }
	else { rep_mat=NULL;}
	for(i=0;i<rep_dim1;i++)
          rep_mat[i] = new Vector<Item>(*m.rep_mat[i]);
}

template<class Item>
Matrix<Item>::~Matrix(){
	int i;
        
        for(i=0;i<rep_dim1;i++) delete rep_mat[i];
	delete [] rep_mat;
}


template<class Item> Matrix<Item>&
Matrix<Item>::clear() {
  for (int i=0;i<rep_dim1;i++) delete rep_mat[i];
  delete [] rep_mat;
  rep_mat=NULL ;
  rep_dim1 = 0 ;
  rep_dim2 = 0 ;
  return *this;
}

// resize Matrix, copy old elements where possible
template<class Item> Matrix<Item>&
Matrix<Item>::resize(int dim1, int dim2) {
  Vector<Item>** new_mat = new Vector<Item>*[dim1];
  assert(new_mat!=NULL);

  // copy existing elements into new Matrix where possible
  int i;
  for (i=0; i<ELCOR_MIN(dim1, rep_dim1); i++) {
    rep_mat[i]->resize(dim2);
    new_mat[i] = rep_mat[i];
  }
  // if new matrix is larger allocate more elements
  for (i=rep_dim1; i<dim1; i++) {
    new_mat[i] = new Vector<Item>(dim2);
  }
  // if new matrix is smaller delete remaining elements
  for (i=dim1; i<rep_dim1; i++) {
    delete rep_mat[i];
  }

  delete [] rep_mat;
  rep_mat = new_mat;
  rep_dim1 = dim1;
  rep_dim2 = dim2;
  return *this;
}

// resize Matrix, copy old elements where possible, initializing remaining
// elements.
template<class Item> Matrix<Item>&
Matrix<Item>::resize(int dim1, int dim2, Item init_val) {
  Vector<Item>** new_mat = new Vector<Item>*[dim1];

  // copy existing elements into new Matrix where possible
  int i ;
  for (i=0; i<ELCOR_MIN(dim1, rep_dim1); i++) {
    rep_mat[i]->resize(dim2, init_val);
    new_mat[i] = rep_mat[i];
  }
  // if new Matrix is larger than old, initialize remaining elements
  for (i=rep_dim1; i<dim1; i++) {
    new_mat[i] = new Vector<Item>(dim2, init_val);
  }
  // if new matrix is smaller delete remaining elements
  for (i=dim1; i<rep_dim1; i++) {
    delete rep_mat[i];
  }

  delete [] rep_mat;
  rep_mat = new_mat;
  rep_dim1 = dim1;
  rep_dim2 = dim2;
  return *this;
}

template<class Item>
void Matrix<Item>::reshape(int dim1, int dim2)
{
  clear();
  resize(dim1, dim2);
}

template<class Item>
void  Matrix<Item>::reshape(int dim1, int dim2, Item initial_value)
{
  clear();
  resize(dim1, dim2, initial_value);
}

template<class Item>
Matrix<Item>& Matrix<Item>::operator= (const Matrix<Item>& m){ /* Assignment */
	int i;
	if (this == &m) return *this;
	for(i=0;i<rep_dim1;i++) delete rep_mat[i];
	delete [] rep_mat;
	rep_dim1=m.rep_dim1;
	rep_dim2=m.rep_dim2;
	if (rep_dim1>0)  {
                rep_mat = new Vector<Item>* [rep_dim1];	
		assert(rep_mat!=NULL); 
	} else
		rep_mat=NULL;
	for (i=0;i<rep_dim1;i++) rep_mat[i]= new Vector<Item> (*m.rep_mat[i]);
	return *this;
}

template <class Item>
bool Matrix<Item>::operator==(const Matrix<Item>& m) const {
	if (rep_dim1 != m.rep_dim1) return false;
	if (rep_dim2 != m.rep_dim2) return false;
	for (int i = 0; i < rep_dim1; i++)
	  if (!(*rep_mat[i] == *m.rep_mat[i])) return false;
	return true;
}

template <class Item>
bool Matrix<Item>::operator!=(const Matrix<Item>& m) const {
        return (!operator==(m));
}

template<class Item>
Vector<Item>& Matrix<Item>::operator[] (int inx) const {
	assert(0<=inx);
	assert(inx<rep_dim1);
	assert(rep_mat!=NULL);
	return(*rep_mat[inx]);
}

template<class Item>
int Matrix<Item>::dim1() const {
	return rep_dim1;
}

template<class Item>
int Matrix<Item>::dim2() const {
	return rep_dim2;
}

template<class Item>
Matrix<Item>& Matrix<Item>::set(const Item d){ /*sets all elements to value d*/
int i;
	assert (rep_dim1>=0);
	for(i=0;i<rep_dim1;i++) {
		(*this)[i].assign((*this)[i].size(), d);
	}
	return(*this);
}


