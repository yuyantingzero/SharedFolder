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
//      File:           ref.h
//      Authors:        Sadun Anik
//      Created:        July 1996
//      Description:    El_ref class definition
//
/////////////////////////////////////////////////////////////////////////////

#include "ref.h"
#include "iterators.h"
#include "op.h"
#include "hash_functions.h"

El_ref::El_ref ()
{
  op_ptr = (Op*)-1;
  oprnd_ptr = NULL;
  type = UNKNOWN_REF_TYPE;
}

El_ref::El_ref (Op* op, Operand* opnd_ptr, El_ref_type t)
{
  op_ptr = op;
  oprnd_ptr = opnd_ptr;
  type = t;
}

El_ref::~El_ref ()
{
}

El_ref& El_ref::operator= (const El_ref& d1)
{
  op_ptr = d1.op_ptr;
  oprnd_ptr = d1.oprnd_ptr;
  type = d1.type;
  return (*this);
}

bool El_ref::operator== (const El_ref& d1) const
{
  if ((op_ptr == d1.op_ptr) && (oprnd_ptr == d1.oprnd_ptr))
    return true;
  else
    return false;
}

bool El_ref::operator!= (const El_ref& d1) const
{
  if (*this == d1)
    return false;
  else
    return true;
}

Operand& El_ref::get_operand ()
{
  return (*oprnd_ptr);
}

Op* El_ref::get_op ()
{
  return (op_ptr);
}

Port_num El_ref::get_port_num ()
{
  switch (type) {
  case MEM_SRC:
  case MEM_DEST:
    return (MEM0);
    break;
  case PRED_SRC:
    return (PRED1);
    break;
  case IMP_SRC:
    return (SRC1);
    break;
  case IMP_DEST:
    return (DEST1);
    break;
  case EXP_SRC: {
    Op_explicit_sources os;
    for (os (op_ptr); os != 0; os++)
      if (&(*os) == oprnd_ptr)
	return (os.get_port_num());
    El_punt ("Can't find source operand");
    break;
  }
  case EXP_DEST: {
    Op_explicit_dests od;
    for (od (op_ptr); od != 0; od++)
      if (&(*od) == oprnd_ptr)
	return (od.get_port_num());
    El_punt ("Can't find destination operand");
    break;
  }
  default:
    El_punt("Unknown reference type");
    break;
  }

  return (SRC1); // Should never execute
}


/*
 *	SAM: 2-29-00
 *	Return the reference dtype if one exists, else return the operand dtype
 *	Note, only explicit source and dest have reference dtypes, preds do not.
 *	If this is called for a reference that doesn't have a dtype, ie a memvr,
 *	you should get a failed virtual function call.
 */
#if 0
DType El_ref::dtype()
{
    Port_num port_num;
    Port_type port_type;

    if (type == EXP_SRC)
	port_type = SRC;
    else if (type == EXP_DEST)
	port_type = DEST;
    else
	return (oprnd_ptr->dtype());

    port_num = this->get_port_num();
    return (El_get_dtype(op_ptr, port_type, port_num));
}
#endif

DType El_ref::dtype()
{
    Reference_dtype_attr *itype;
    DType dt;
    Port_num port_num;

    itype = get_reference_dtype_attr(op_ptr);
    if (itype == NULL)
        return (oprnd_ptr->dtype());
    if (! ((type == EXP_SRC) || (type == EXP_DEST)))
        return (oprnd_ptr->dtype());

    port_num = this->get_port_num();
    if (type == EXP_SRC) {
        return (itype->src(port_num));
    }
    else {
        return (itype->dest(port_num));
    }
}

Spatial_ref El_ref::spatial_ref()
{
    Spatial_ref_attr *itype;
    Port_num port_num;

    itype = get_spatial_ref_attr(op_ptr);
    if (itype == NULL)
        return Spatial_ref();
    if (! ((type == EXP_SRC) || (type == EXP_DEST)))
        return Spatial_ref();

    port_num = this->get_port_num();
    if (type == EXP_SRC) {
        return (itype->src(port_num));
    }
    else {
        return (itype->dest(port_num));
    }
}



int El_ref::width()
{
   DType dt = this->dtype();
   return (dt.width());
}

void El_ref::print (ostream& os) const
{
  os << "[";

  if (op_ptr == 0)
    os << "undefined?" << ", ";
  else
    os << "opid-" << op_ptr->id() << ", ";

  if (oprnd_ptr == 0)
    os << "undefined?" << ", ";
  else
    os << (*oprnd_ptr) << ", ";

  switch (type) {
  case UNKNOWN_REF_TYPE:
    os << "UNKNOWN_REF";
    break;
  case PRED_SRC:
    os << "PRED_SRC";
    break;
  case EXP_SRC:
    os << "EXP_SRC";
    break;
  case IMP_SRC:
    os << "IMP_SRC";
    break;
  case MEM_SRC:
    os << "MEM_SRC";
    break;
  case LIVEOUT_TAKEN_SRC:
    os << "LIVEOUT_TAKEN_SRC";
    break;
  case LIVEOUT_FALLTHROUGH_SRC:
    os << "LIVEOUT_FALLTHROUGH_SRC";
    break;
  case EXP_DEST:
    os << "EXP_DEST";
    break;
  case IMP_DEST:
    os << "IMP_DEST";
    break;
  case MEM_DEST:
    os << "MEM_DEST";
    break;
  case LIVEIN_DEST:
    os << "LIVEIN_DEST";
    break;
  default:
    El_punt ("This type of ref should not exist!");
    break;
  }
  os << "]";
}

ostream& operator<< (ostream& os, const El_ref& ref)
{
  ref.print (os);
  return os;
}

unsigned hash_El_ref (El_ref& ref)
{
  long int sum;

  sum = (long)ref.get_op () + (long)(&(ref.get_operand ()));

  return (ELCOR_INT_HASH(sum));
}
//////////////////////////////////////////////////////////////////////////////
