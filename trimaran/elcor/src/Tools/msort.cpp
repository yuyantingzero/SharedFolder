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
//      File:           msort.cpp
//      Authors:        Shail Aditya, Vinod Kathail
//      Created:        November 1995
//      Description:    Merge Sort
//
/////////////////////////////////////////////////////////////////////////////

#include "msort.h"

const int SORT_THRESHOLD = 8;

//Auxiliary functions

template<class T>
void insertion_sort_aux(Vector<T>& vec, int from, int to);

template<class T>
void merge(Vector<T>& vec1,Vector<T> vec2,Vector<T>& result);

template<class T>
Vector<T>* merge_sort_aux(Vector<T>& vec, int from, int to);

template<class T>
void insertion_sort_aux_pred(Vector<T>& vec, bool (*less)(T, T), int from, int 
to);

template<class T>
void merge_pred(Vector<T>& vec1,Vector<T> vec2,Vector<T>& result, 
		bool (*less)(T, T));

template<class T>
Vector<T>* merge_sort_aux_pred(Vector<T>& vec, bool (*less)(T, T), int from, int to);

// The real functions

template<class T>
void merge_sort(Vector<T>& vec){
    Vector<T>* new_vec = merge_sort_aux(vec, 0, vec.size());
    for(unsigned i = 0; i < vec.size(); i++){
	vec[i] = (*new_vec)[i];
    }
    delete new_vec;
    return ;
}

template<class T>
void insertion_sort(Vector<T>& vec) {
    insertion_sort_aux(vec, 0, vec.size());
    return ;
}

template<class T>
void merge_sort_pred(Vector<T>& vec, bool (*less)(T, T)){
    Vector<T>* new_vec = merge_sort_aux_pred(vec, less, 0, vec.size());
    for(unsigned i = 0; i < vec.size(); i++){
	vec[i] = (*new_vec)[i];
    }
    delete new_vec;
    return ;
}

template<class T>
void insertion_sort_pred(Vector<T>& vec, bool (*less)(T, T)) {
    insertion_sort_aux_pred(vec, less, 0, vec.size());
    return ;
}

// The auxiliary functions

template<class T>
void merge(Vector<T>& vec1,Vector<T> vec2,Vector<T>& result)
{ 
    int length1 = vec1.size();
    int length2 = vec2.size();
    int i = 0;
    int j = 0; 
    int k = 0;
    while ((i < length1) && (j < length2)) {
	if (vec1[i] < vec2[j]) {
	   result[k] = vec2[j]; 
	    j++; 
	    k++;
	} else {
	    result[k] = vec1[i];
	    i++; 
	    k++;
	}
    }
    while (i < length1) {
	result[k] = vec1[i];
	i++; 
	k++;
    }
    while (j < length2) {
	result[k] = vec2[j];
	j++; 
	k++;
    }
}

// The range is "from" inclusive to "to" exclusive
template<class T>
Vector<T>* merge_sort_aux(Vector<T>& vec, int from, int to) {
    int length = to - from;
    if (length < SORT_THRESHOLD) {           // use insertion sort
	Vector<T>* new_vec = new Vector<T>(length);
	for(int j = 0, i = from; j < length; j++, i++){
	    (*new_vec)[j] = vec[i];
	}
	insertion_sort_aux(*new_vec, 0, length);
	return new_vec;
    }
    else {
	int middle;
	middle = (to - from) / 2; // truncated towards zero
	Vector<T>* vec1 = merge_sort_aux(vec, from, from+middle);
	Vector<T>* vec2 = merge_sort_aux(vec, from+middle, to);
	Vector<T>* new_vec1 = new Vector<T>(to - from);
	merge(*vec1, *vec2, *new_vec1);
	delete vec1;
	delete vec2;
	return new_vec1;
    }
}

// The range is "from" inclusive to "to" exclusive
template<class T>
void insertion_sort_aux(Vector<T>& vec, int from, int to){
    T i_temp;
    T j_temp;
    for (int i = from; i < to; i++){
	for(int j = i+1; j < to; j++){
	    i_temp = vec[i];
	    j_temp = vec[j];
	    if (i_temp < j_temp) {
		vec[i] = j_temp;
		vec[j] = i_temp;
	    }
	}
    }
}

template<class T>
void merge_pred(Vector<T>& vec1,Vector<T> vec2,Vector<T>& result, bool (*less)(T, T))
{ 
    int length1 = vec1.size();
    int length2 = vec2.size();
    int i = 0;
    int j = 0; 
    int k = 0;
    while ((i < length1) && (j < length2)) {
	if ((*less)(vec1[i],vec2[j])) {
	   result[k] = vec2[j]; 
	    j++; 
	    k++;
	} else {
	    result[k] = vec1[i];
	    i++; 
	    k++;
	}
    }
    while (i < length1) {
	result[k] = vec1[i];
	i++; 
	k++;
    }
    while (j < length2) {
	result[k] = vec2[j];
	j++; 
	k++;
    }
}

// The range is "from" inclusive to "to" exclusive
template<class T>
Vector<T>* merge_sort_aux_pred(Vector<T>& vec, bool (*less)(T, T), int from, int to)
{
    int length = to - from;
    if (length < SORT_THRESHOLD) {           // use insertion sort
	Vector<T>* new_vec = new Vector<T>(length);
	for(int j = 0, i = from; j < length; j++, i++){
	    (*new_vec)[j] = vec[i];
	}
	insertion_sort_aux_pred(*new_vec, less, 0, length);
	return new_vec;
    }
    else {
	int middle;
	middle = (to - from) / 2; // truncates towards zero
	Vector<T>* vec1 = merge_sort_aux_pred(vec, less, from, from+middle);
	Vector<T>* vec2 = merge_sort_aux_pred(vec, less, from+middle, to);
	Vector<T>* new_vec1 = new Vector<T>(to - from);
	merge_pred(*vec1, *vec2, *new_vec1, less);
	delete vec1;
	delete vec2;
	return new_vec1;
    }
}

// The range is "from" inclusive to "to" exclusive
template<class T>
void insertion_sort_aux_pred(Vector<T>& vec, bool (*less)(T, T), int from, int to){
    T i_temp;
    T j_temp;
    for (int i = from; i < to; i++){
	for(int j = i+1; j < to; j++){
	    i_temp = vec[i];
	    j_temp = vec[j];
	    if ((*less)(i_temp, j_temp)) {
		vec[i] = j_temp;
		vec[j] = i_temp;
	    }
	}
    }
}
