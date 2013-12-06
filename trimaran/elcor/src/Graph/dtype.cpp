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
//      File:           dtype.cpp
//      Authors:        Tim Sherwood, Scott Mahlke
//      Created:        July 1999
//      Description:    Dtype class declaration
//
/////////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include "defs.h"
#include "dtype.h"
#include "el_error.h"
#include "el_opti_init.h"
#include "width_utilities.h"

const DType EL_STANDARD_INT		(EL_DEFAULT_INT_WIDTH,true,EL_DT_INFO_INT);
const DType EL_STANDARD_UINT		(EL_DEFAULT_INT_WIDTH,false,EL_DT_INFO_INT);
const DType EL_STANDARD_CHAR		(8,true,EL_DT_INFO_INT);
const DType EL_STANDARD_UCHAR		(8,false,EL_DT_INFO_INT);
const DType EL_STANDARD_SHORT		(16,true,EL_DT_INFO_INT);
const DType EL_STANDARD_USHORT		(16,false,EL_DT_INFO_INT);

const DType EL_STANDARD_SINGLE		(EL_DEFAULT_SINGLE_WIDTH,true,EL_DT_INFO_FLOAT);
const DType EL_STANDARD_FLOAT		(EL_STANDARD_SINGLE);
const DType EL_STANDARD_DOUBLE		(EL_DEFAULT_DOUBLE_WIDTH,true,EL_DT_INFO_FLOAT);

const DType EL_STANDARD_GLOBAL_ABS	(EL_DEFAULT_STRING_WIDTH,true,EL_DT_INFO_GLOBAL_ABS);
const DType EL_STANDARD_LOCAL_ABS	(EL_DEFAULT_STRING_WIDTH,true,EL_DT_INFO_LOCAL_ABS);
const DType EL_STANDARD_GLOBAL_GP	(EL_DEFAULT_STRING_WIDTH,true,EL_DT_INFO_GLOBAL_GP);
const DType EL_STANDARD_LOCAL_GP	(EL_DEFAULT_STRING_WIDTH,true,EL_DT_INFO_LOCAL_GP);
const DType EL_STANDARD_STRING		(EL_STANDARD_GLOBAL_ABS);

const DType EL_STANDARD_LABEL		(EL_DEFAULT_LABEL_WIDTH,true,EL_DT_INFO_GLOBAL_ABS);  
const DType EL_STANDARD_BRANCH		(EL_DEFAULT_BRANCH_WIDTH,true,EL_DT_INFO_BRANCH);  
const DType EL_STANDARD_PREDICATE	(EL_DEFAULT_PREDICATE_WIDTH,false,EL_DT_INFO_PREDICATE);
const DType EL_STANDARD_POINTER		(EL_DEFAULT_POINTER_WIDTH,false,EL_DT_INFO_POINTER);

/* RMR { add vector data types */
const DType EL_STANDARD_VEC_INT         (EL_DEFAULT_INT_WIDTH, true, EL_DT_INFO_VEC_INT); 
const DType EL_STANDARD_VEC_FLOAT       (EL_DEFAULT_SINGLE_WIDTH, true, EL_DT_INFO_VEC_FLOAT);
const DType EL_STANDARD_VEC_DOUBLE      (EL_DEFAULT_DOUBLE_WIDTH, true, EL_DT_INFO_VEC_FLOAT);
const DType EL_STANDARD_VEC_MASK        (EL_DEFAULT_PREDICATE_WIDTH, false, EL_DT_INFO_VEC_MASK);
/* } RMR */
  
//note: this code is backward compatible not forward thinking -Tim Sherwood 7/99
const DType EL_STANDARD_UNDEFINED	(EL_DEFAULT_INT_WIDTH,false,EL_DT_INFO_INT);



DType::DType( int width, bool sign, DType_info dti )
{
	w = width;
	assert( w >= 0 );
	s = sign;
	i = dti;
}

//DType::DType( DType_info dt )
//{
//}

DType::DType( const DType & t )
{
	w = t.w;
	i = t.i;
	s = t.s;
}

char* DType::get_char_rep() const
{
	static char c[150];
	char ts[50];

	// signed or unsinged
	char sc = s?'s':'u';

	switch ( i ) {
        case(EL_DT_INFO_INT): sprintf(ts,"int"); break;
        case(EL_DT_INFO_FLOAT):  sprintf(ts,"float"); break;
        case(EL_DT_INFO_BRANCH):  sprintf(ts,"branch"); break;
        case(EL_DT_INFO_LOCAL_ABS): 
        case(EL_DT_INFO_LOCAL_GP):
        case(EL_DT_INFO_GLOBAL_ABS):
        case(EL_DT_INFO_GLOBAL_GP):  sprintf(ts,"string lit"); break;
        case(EL_DT_INFO_POINTER):  sprintf(ts,"pointer"); break;
        case(EL_DT_INFO_PREDICATE):  sprintf(ts,"predicate"); break;
        case(EL_DT_INFO_VEC_INT): sprintf(ts,"vint"); break;
        case(EL_DT_INFO_VEC_FLOAT): sprintf(ts,"vfloat"); break;
        case(EL_DT_INFO_VEC_MASK): sprintf(ts,"vmask"); break;
        default:  sprintf(ts,"unknown"); break;
	}

	sprintf( c, "{%d,%c,%s}", w, sc, ts );
	
	return c;
}


void DType::get_from_string(char* c)
{
	char next_char = 0, ctemp;
	char * numstart;
	assert( isalpha( c[0] ) );
	char cstart = c[next_char++];
	switch( cstart )
	{
	    case('u'): 
		i=EL_DT_INFO_INT;
		s=false;
		assert( c[next_char++] == 'i' );
		break;
	    case('i'): 
		i=EL_DT_INFO_INT;
		s=true;
		break;
	    case('f'): 
		i=EL_DT_INFO_FLOAT;
		s=EL_STANDARD_FLOAT.is_signed();
		break;
	    case('b'): 
		i=EL_DT_INFO_BRANCH;
		s=EL_STANDARD_BRANCH.is_signed();
		break;
	    case('p'):
		ctemp = c[next_char];
		if ( ctemp == 't' ) {
		   assert( c[next_char++] == 't' );
		   assert( c[next_char++] == 'r' );
		   i=EL_DT_INFO_POINTER;	
		   s=EL_STANDARD_POINTER.is_signed();
		} else {
		   assert( isdigit(c[next_char]) );
		   i=EL_DT_INFO_PREDICATE;
		   s=EL_STANDARD_PREDICATE.is_signed();
		}
		break;
	    case('l'):
		assert( c[next_char++] == '_' );
		ctemp = c[next_char++];
		if ( ctemp == 'g' ) {
		   assert( c[next_char++] == 'p' );
		   s=EL_STANDARD_LOCAL_GP.is_signed();
		   i=EL_DT_INFO_LOCAL_GP;		   
		} else {
		   assert(          ctemp == 'a' );
		   assert( c[next_char++] == 'b' );
		   assert( c[next_char++] == 's' );
		   s=EL_STANDARD_LOCAL_ABS.is_signed();
		   i=EL_DT_INFO_LOCAL_ABS;		   
		} 
		break;
	    case('g'):
		assert( c[next_char++] == '_' );
		ctemp = c[next_char++];
		if ( ctemp == 'g' ) {
		   assert( c[next_char++] == 'p' );
		   s=EL_STANDARD_GLOBAL_GP.is_signed();		   
		   i=EL_DT_INFO_GLOBAL_GP;		   
		} else {
		   assert(          ctemp == 'a' );
		   assert( c[next_char++] == 'b' );
		   assert( c[next_char++] == 's' );
		   s=EL_STANDARD_GLOBAL_ABS.is_signed();		   
		   i=EL_DT_INFO_GLOBAL_ABS;		   
		} 
		break;
            case('v'):
                ctemp = c[next_char++];
                if (ctemp == 'i') {
                    i = EL_DT_INFO_VEC_INT;
                    s = EL_STANDARD_VEC_INT.is_signed();
                } else if (ctemp == 'f') {
                    i = EL_DT_INFO_VEC_FLOAT;
                    s = EL_STANDARD_VEC_FLOAT.is_signed();
                } else {
                    assert(ctemp == 'm');
                    i = EL_DT_INFO_VEC_MASK;
                    s = EL_STANDARD_VEC_MASK.is_signed();
                }
                break;
	     default:
		El_punt("Unknown character at start of DType: %c", cstart );
		break;
	}
	numstart = &(c[next_char]);
	w = atoi( numstart );

	//assert that the input is valid
	switch( i ) {
	case(EL_DT_INFO_INT):	     break;
        case(EL_DT_INFO_FLOAT):      assert( (*this)==EL_STANDARD_SINGLE 
					||   (*this)==EL_STANDARD_DOUBLE ); break;
        case(EL_DT_INFO_BRANCH):     assert( (*this)==EL_STANDARD_BRANCH ); break;
        case(EL_DT_INFO_LOCAL_ABS):  assert( (*this)==EL_STANDARD_LOCAL_ABS ); break;
        case(EL_DT_INFO_LOCAL_GP):   assert( (*this)==EL_STANDARD_LOCAL_GP ); break;
        case(EL_DT_INFO_GLOBAL_ABS): assert( (*this)==EL_STANDARD_GLOBAL_ABS ); break;
        case(EL_DT_INFO_GLOBAL_GP):  assert( (*this)==EL_STANDARD_GLOBAL_ABS ); break;
        case(EL_DT_INFO_POINTER):    break;
        case(EL_DT_INFO_PREDICATE):  assert( (*this)==EL_STANDARD_PREDICATE ); break;
        case(EL_DT_INFO_VEC_INT):    assert( (*this)==EL_STANDARD_VEC_INT ); break;
        case(EL_DT_INFO_VEC_FLOAT):  assert( (*this)==EL_STANDARD_VEC_FLOAT 
                                        ||   (*this)==EL_STANDARD_VEC_DOUBLE ); break;
        case(EL_DT_INFO_VEC_MASK):   assert( (*this)==EL_STANDARD_VEC_MASK ); break;
	default: El_punt("What the.. Unknown info type field");
	}
}

char* DType::put_to_string() const
{
	char ts[8];
	char* retstr = new char[15];
	switch( i ) {
	case(EL_DT_INFO_INT):        (s)?sprintf(ts,"i"):sprintf(ts,"ui"); break;
        case(EL_DT_INFO_FLOAT):      sprintf(ts,"f"); break;
        case(EL_DT_INFO_BRANCH):     sprintf(ts,"b"); break;
        case(EL_DT_INFO_LOCAL_ABS):  sprintf(ts,"l_abs"); break;
        case(EL_DT_INFO_LOCAL_GP):   sprintf(ts,"l_gp"); break;
        case(EL_DT_INFO_GLOBAL_ABS): sprintf(ts,"g_abs"); break;
        case(EL_DT_INFO_GLOBAL_GP):  sprintf(ts,"g_gp"); break;
        case(EL_DT_INFO_POINTER):    sprintf(ts,"ptr"); break;
        case(EL_DT_INFO_PREDICATE):  sprintf(ts,"p"); break;
        case(EL_DT_INFO_VEC_INT):    sprintf(ts,"vi"); break;
        case(EL_DT_INFO_VEC_FLOAT):  sprintf(ts,"vf"); break;
        case(EL_DT_INFO_VEC_MASK):   sprintf(ts,"vm"); break;
        default:  sprintf(ts,"unknown"); break;
	}
	sprintf(retstr,"%s%d", ts, w );
	return retstr;
}

ostream& operator<< (ostream& o, const DType& t)
{
	char * c = t.get_char_rep();
	return o << c;
}


int DType::print( FILE* fout )
{
	char * c = get_char_rep();
	int r = fprintf( fout, "%s", c );
	return r;
}

void DType::calc_and_set_width(int lit, bool sign)
{
    w = get_int_lit_width(lit,sign);
}

int DType::operator==( const DType & x ) const
{
	if( w==x.w && s==x.s ) return true;
	else return false;
}

int DType::operator!=( const DType & x ) const
{
	if( w==x.w && s==x.s ) return false;
	else return true;
}


int DType::upconv_width()
{
	if ( i == EL_DT_INFO_INT || i == EL_DT_INFO_POINTER) {
	   if ( w == EL_DEFAULT_INT_WIDTH ) return 32;
	   else if ( w <= EL_DEFAULT_CHAR_WIDTH )  return EL_DEFAULT_CHAR_WIDTH;
	   else if ( w <= EL_DEFAULT_SHORT_WIDTH ) return EL_DEFAULT_SHORT_WIDTH;
	   else if ( w <= EL_DEFAULT_INT_WIDTH )   return EL_DEFAULT_INT_WIDTH;
	   else if ( w <= EL_DEFAULT_LONG_WIDTH )  return EL_DEFAULT_LONG_WIDTH;
	   else El_punt("Dtype::upconv_width: cannot up convert int of size %d",w);
	} else if (i == EL_DT_INFO_FLOAT) {
	   if ( w <= EL_DEFAULT_SINGLE_WIDTH ) return EL_DEFAULT_SINGLE_WIDTH;
	   else if ( w <= EL_DEFAULT_DOUBLE_WIDTH ) return EL_DEFAULT_DOUBLE_WIDTH;
	   else El_punt("Dtype::upconv_width: cannot up convert float of size %d",w);
	}
	else {
	   // For the other operand types, just leave the width alone
	   return (w);
	}
	assert(0);
	return (0);
}
