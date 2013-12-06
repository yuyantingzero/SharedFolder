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



/*===========================================================================
 *
 *      File:           wconstraint.cpp
 *      Author:         Tim Sherwood
 *      Created:        Aug 1999
 *      Description:    Width constraint class, used for width analysis
 *
 *===========================================================================*/

#include "stdlib.h"
#include "wconstraint.h"
#include "operand.h"


WConstraint::WConstraint( Operand& o )
{
	set_from_operand( o );
}

WConstraint::WConstraint()
{
        dt = EL_STANDARD_INT;
        is_lit = false;  
        val = 0;
}

WConstraint::~WConstraint()
{
}

void WConstraint::set_from_operand( Operand& o )
{
	dt = o.dtype();
	if ( o.is_lit() && o.is_int() ) {
		is_lit = true;
		val = ((Int_lit*) &o)->value();
	} else {
		is_lit = false;
		val = 0;
	}	
}

void WConstraint::set_unconstrained()
{
	dt = EL_STANDARD_INT;
	is_lit = false;
	val = 0;
}

void WConstraint::set_to_max( WConstraint wc  )
{
  if ( wc.dt.is_signed() != dt.is_signed() ) {
    // El_punt( "WConstraint::set_to_max: cannot handle two defs to with different signs");
    El_warn("WConstraint::set_to_max: Two defs with different signs! Setting unconstrained.");
    set_unconstrained();
    return;
  }
  dt.set_width( WIDTH_MAX( wc.dt.width(), dt.width() ) );
  is_lit = false;
}

//return true if they are the differ
bool WConstraint::set_relax( WConstraint wc )
{
	if ( wc.dt.is_signed() != dt.is_signed() )
		El_punt( "WConstraint::set_to_max: cannot handle two defs to with different signs");
	
	unsigned old_width = dt.width();
	// the min of the two, but not less than zero
	dt.set_width( WIDTH_RELAX(wc.dt.width(),dt.width() ) );

	if ( old_width == dt.width() ) return false;
	else return true;
}


//return true if they are the same
bool WConstraint::set_relax( int s )
{
	assert( s > 0 );
	if ( (unsigned) s ==  dt.width() ) {
		return true; 
	} else { 
		// the min of the two, but not less than zero
		dt.set_width( WIDTH_RELAX( (unsigned)s, dt.width() ) );
		return false;
	}
}

bool WConstraint::operator!= ( const WConstraint & wc1 ) const
{ 
	return( ! ( operator==(wc1) ) );
}

bool WConstraint::operator== ( const WConstraint & wc1 ) const
{
	//return true if there is they have the same dtype and 
 	//they are both not lits, or if they have the same lit val
	return  (    ( (dt==wc1.dt)&&(!is_lit) )
	          || ( (dt==wc1.dt)&&(is_lit)&&(val==wc1.val) ) 
	  	);
}


ostream& operator<< (ostream& o, const WConstraint& wc1)
{
	o << wc1.dt;
	if ( wc1.is_lit ) o << "<litval=" << wc1.val << ">";
	return o;
}

bool WConstraint::ratchet_up( WConstraint wc1, WConstraint maxout )
{
	return ratchet_up(wc1.dt.width(),maxout);
}

/* returns true if a change was made, otherwise it returns false */
bool WConstraint::ratchet_up( int s, WConstraint maxout )
{
	unsigned m = maxout.dt.width();
	if( s<0 ) s = 0;
	unsigned orig = dt.width();
	if ( (unsigned) s ==  dt.width() ) {
		assert( (unsigned)s <= m );
	} else { 
		// the max of the two, but not more than maxout
		dt.set_width( WIDTH_MIN( WIDTH_MAX( (unsigned)s, dt.width() ) , m ));
	}
	//return true if they are different than the original
	return ( orig != dt.width() );
}

