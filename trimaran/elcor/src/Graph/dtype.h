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
//      File:           dtype.h
//      Authors:        Tim Sherwood, Scott Mahlke
//      Created:        July 1999
//      Description:    Dtype class declaration 
//
/////////////////////////////////////////////////////////////////////////////

/*
 * This is the basic type class to be applied to all operands
 * and operators in Elcor.
 *
 * Written by: Tim Sherwood
 * Date: Jul 8 1999
 * 
 */ 

#ifndef DTYPE_H
#define DTYPE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Yikes!  We should never be including assert.h. We have our own
// definitions (see defs.h) that seg fault gracefully in gdb, allowing
// you to go "up" to the error.  The definition in assert.h causes the
// gdb stack to be lost.  dcronqui, 11/9/99
//#include <assert.h>
#include <limits.h>

#define EL_DEFAULT_INT_WIDTH 32
#define EL_DEFAULT_LONG_WIDTH 32
#define EL_DEFAULT_LONGLONG_WIDTH 32
#define EL_DEFAULT_LONGLONGLONG_WIDTH 32
#define EL_DEFAULT_LABEL_WIDTH 32
#define EL_DEFAULT_BRANCH_WIDTH 32
#define EL_DEFAULT_STRING_WIDTH 32
#define EL_DEFAULT_CHAR_WIDTH 8
#define EL_DEFAULT_SHORT_WIDTH 16
#define EL_DEFAULT_LONG_WIDTH 32
#define EL_DEFAULT_SINGLE_WIDTH 32
#define EL_DEFAULT_DOUBLE_WIDTH 64
#define EL_DEFAULT_PREDICATE_WIDTH 1
#define EL_DEFAULT_POINTER_WIDTH 32

enum DType_info {
	EL_DT_INFO_INT,
	EL_DT_INFO_FLOAT,
	EL_DT_INFO_BRANCH,
	EL_DT_INFO_LOCAL_ABS,
	EL_DT_INFO_LOCAL_GP,
	EL_DT_INFO_GLOBAL_ABS,
	EL_DT_INFO_GLOBAL_GP,
	EL_DT_INFO_POINTER,
	EL_DT_INFO_PREDICATE,
        EL_DT_INFO_VEC_INT,
        EL_DT_INFO_VEC_FLOAT,
        EL_DT_INFO_VEC_MASK
};


class DType
{
  protected:
	unsigned int w;
	bool s;
	DType_info i;
	char* get_char_rep() const;
  public:
	//con and destruct
	DType( int width = EL_DEFAULT_INT_WIDTH, bool sign = true, DType_info dt = EL_DT_INFO_INT );
	//DType( DType_info dt );
	DType( const DType & );
	~DType() {}

	//printing and reading
	friend ostream& operator<< (ostream& o, const DType& t);
	void get_from_string(char* s);
	char* put_to_string( ) const;
	int print( FILE* fout=stdout );

	//width access functions
	unsigned int width() const {return w;}
	void set_width(unsigned int width) {w = width;}
	void calc_and_set_width(int lit, bool sign);
	int upconv_width();

	//sign access functions
	bool is_signed() const {return s;}
	void set_sign(bool sign) { s=sign; }

	//info access functions
	DType_info info() const {return i;}
	void set_info(DType_info inf) { i = inf; }

	bool is_float() const { return i==EL_DT_INFO_FLOAT; }
	bool is_int() const { return i==EL_DT_INFO_INT; }
	bool is_branch() const { return i==EL_DT_INFO_BRANCH; }
	bool is_predicate() const { return i==EL_DT_INFO_PREDICATE; }
	bool is_pointer() const { return i==EL_DT_INFO_POINTER; }

	bool is_single() const { return i==EL_DT_INFO_FLOAT && w==EL_DEFAULT_SINGLE_WIDTH; }
	bool is_double() const { return i==EL_DT_INFO_FLOAT && w==EL_DEFAULT_DOUBLE_WIDTH; }

	bool is_vec_int() const { return i==EL_DT_INFO_VEC_INT; }
	bool is_vec_float() const { return i==EL_DT_INFO_VEC_FLOAT; }
	bool is_vec_single() const { return i==EL_DT_INFO_VEC_FLOAT && w==EL_DEFAULT_SINGLE_WIDTH; }
	bool is_vec_double() const { return i==EL_DT_INFO_VEC_FLOAT && w==EL_DEFAULT_DOUBLE_WIDTH; }
	bool is_vec_mask() const { return i==EL_DT_INFO_VEC_MASK; }

	//comparison operators
	int operator ==( const DType & x ) const;
	int operator !=( const DType & x ) const;
};

extern const DType EL_STANDARD_INT;
extern const DType EL_STANDARD_UINT;
extern const DType EL_STANDARD_CHAR;
extern const DType EL_STANDARD_UCHAR;
extern const DType EL_STANDARD_SHORT;
extern const DType EL_STANDARD_USHORT;
extern const DType EL_STANDARD_FLOAT;
extern const DType EL_STANDARD_DOUBLE;
extern const DType EL_STANDARD_STRING;
extern const DType EL_STANDARD_LOCAL_ABS;
extern const DType EL_STANDARD_LOCAL_GP;
extern const DType EL_STANDARD_GLOBAL_ABS;
extern const DType EL_STANDARD_GLOBAL_GP;
extern const DType EL_STANDARD_LABEL;
extern const DType EL_STANDARD_BRANCH;
extern const DType EL_STANDARD_PREDICATE;
extern const DType EL_STANDARD_POINTER;
extern const DType EL_STANDARD_UNDEFINED;
extern const DType EL_STANDARD_VEC_INT;
extern const DType EL_STANDARD_VEC_FLOAT;
extern const DType EL_STANDARD_VEC_DOUBLE;
extern const DType EL_STANDARD_VEC_MASK;


#endif
