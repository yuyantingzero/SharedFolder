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
//      File:           width_utilities.cpp
//      Authors:        Scott Mahlke
//      Created:        January 2001
//      Description:    Utilities for width manipulation
//
/////////////////////////////////////////////////////////////////////////////

#include "width_utilities.h"
#include "iterators.h"
#include "op.h"
#include "opcode_properties.h"
#include "el_error.h"
#include "hash_functions.h"
#include "attributes.h"

/*========================================================================*/
/*
 *	Misc functions for determining the width of literals
 */
/*========================================================================*/

int numbits_for_value(int val, bool sign)
{
    int x = val;
    if ( sign ) {
        if (x == -1 || x == 0) return 1;

        int seq = 0;
        unsigned int mask = 1 << (EL_DEFAULT_INT_WIDTH - 1);
        unsigned int ux = x;


        /* is msb the same as next bit ? */
        while ( (ux&mask) == ((ux << 1) & mask) ) {
           seq++;
           ux = ux << 1;
        }
        assert (0 <= seq && seq <= EL_DEFAULT_INT_WIDTH-2);
        return EL_DEFAULT_INT_WIDTH-seq;

    } else { //unsigned
	if (x == 0) return 1;

        if (x == 0) return 1;
        unsigned int y = x;

        int result = 0;
        while (y > 0) {
           result++;
           y = y >> 1;
        }
        return result;

    }
}

int numbits_for_range(int minval, int maxval, bool sign)
{
    int numbits1, numbits2;

    numbits1 = numbits_for_value(minval, sign);
    numbits2 = numbits_for_value(maxval, sign);

    return (ELCOR_MAX(numbits1, numbits2));
}

int get_int_lit_width( int lit, bool sign )
{
    return (numbits_for_value(lit, sign));
}

/*
 *      If signed, max = 2^(n-1) - 1
 *      if unsigned, max = 2^(n) - 1
 */
int get_positive_max_value(int width, bool sign)
{
    double dval;
    if (sign)
        dval = pow(2.0, double(width-1) - 1.0);
    else
        dval = pow(2.0, double(width) - 1.0);
    return (int) dval;
}

int update_value_for_width(int val, int width, bool sign)
{
    int i;

    // Do nothing if value can be stored in specified bits
    if (width >= numbits_for_value(val, sign))
	return (val);

    // Otherwise, we need to truncate val
    unsigned int mask = 0;
    for (i=0; i<width; i++) {
	mask = mask | (1 << i);
    }

    // For signed, truncate, then sign extend
    if (sign) {
	int new_val = ((unsigned) val) & mask;
	new_val = new_val << (EL_DEFAULT_INT_WIDTH-width);
	new_val = new_val >> (EL_DEFAULT_INT_WIDTH-width);
	return (new_val);
    } 
    // For unsigned, just plain truncate
    else {
	unsigned int new_val = ((unsigned) val) & mask;
        return (new_val);
    } 

}


/*========================================================================*/
/*
 *	Operand width (only!) access functions
 */
/*========================================================================*/

DType El_get_operand_dtype(Op *op, Port_type pt, Port_num pn)
{
    if (pt == SRC)
	return (op->src(pn).dtype());
    else if (pt == DEST)
	return (op->dest(pn).dtype());
    else {
        El_punt("El_get_dtype: illegal port type %s",
				(char *)port_type_to_text(pt));
	return (EL_STANDARD_INT);
    }
}

int El_get_operand_width(Op *op, Port_type pt, Port_num pn)
{
    DType dt = El_get_operand_dtype(op, pt, pn);
    return (dt.width());
}

bool El_get_operand_sign(Op *op, Port_type pt, Port_num pn)
{
    DType dt = El_get_operand_dtype(op, pt, pn);
    return (dt.is_signed());
}

/*========================================================================*/
/*
 *	Reference width (only!) access functions
 */
/*========================================================================*/

DType El_get_ref_dtype(Op *op, Port_type pt, Port_num pn)
{
    Reference_dtype_attr *attr = get_reference_dtype_attr(op);
    if (attr == NULL)
	El_punt("El_get_ref_dtype: op %d has no reference dtype attr", op->id());

    if (pt == SRC)
        return (attr->src(pn));
    else if (pt == DEST)
        return (attr->dest(pn));
    else {
	El_punt("El_get_ref_dtype: illegal port type %s",
				(char *)port_type_to_text(pt));
	return (EL_STANDARD_INT);
    }

}

int El_get_ref_width(Op *op, Port_type pt, Port_num pn)
{
    DType dt = El_get_ref_dtype(op, pt, pn);
    return (dt.width());
}

bool El_get_ref_sign(Op *op, Port_type pt, Port_num pn)
{
    DType dt = El_get_ref_dtype(op, pt, pn);
    return (dt.is_signed());
}

// This function kept for compatibility reasons only!!!!
//returns width of an Op port or -1 if there is no annote
//or if there is no port of that number
int operand_reference_dtype(DType& dt, Op* opptr,Port_type pt,Port_num pn)
{
    Reference_dtype_attr* wannote = get_reference_dtype_attr(opptr);
    if ( wannote == NULL ) return -1;

    if ( pt == DEST ) {
        dt =  wannote->dest(pn);
        return(0);
    } else if ( pt == SRC ) {
        dt = wannote->src(pn);
        return(0);
    } else {
        El_punt("operand_reference_dtype: cannot handle non-src/dst ports");
        return 0;
    }
}

/*========================================================================*/
/*
 *	General width access functions, these return the reference
 *	width info if it is available, otherwise it returns the operand
 *	width.
 */
/*========================================================================*/

DType El_get_dtype(Op *op, Port_type pt, Port_num pn)
{
    Reference_dtype_attr *attr = get_reference_dtype_attr(op);

    /* If no attr, use the operand widths */
    if (attr == NULL)
	return (El_get_operand_dtype(op, pt, pn));

    /* If attr, use the reference widths */
    else
	return (El_get_ref_dtype(op, pt, pn));
}

int El_get_width(Op *op, Port_type pt, Port_num pn)
{
   DType dt = El_get_dtype(op, pt, pn);
   return (dt.width());
}

bool El_get_sign(Op *op, Port_type pt, Port_num pn)
{
   DType dt = El_get_dtype(op, pt, pn);
   return (dt.is_signed());
}

