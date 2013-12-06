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

Copyright 2007 University of Michigan

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
//      File:           pattern_match_node.cpp
//      Author:         Nate Clark
//      Created:        October 2005
//      Description:    Class used as a node in a graph to perform
//                      pattern matching on.
//
/////////////////////////////////////////////////////////////////////////////


#include "pattern_match_node.h"
#include "intf.h"


inline bool operator==(const Pattern_match_node& p1, const Pattern_match_node& p2)
{
  switch(p1.type) {
  case PM_OPERATION_INPUT:

    switch(p2.type) {
    case PM_OPERATION_INPUT:
      // FIX: I should check if the operation is commutative, and then
      // return true if the operations match, however, if we allow
      // that determining what the inputs and outputs are for the
      // final custom instruction is a bit trickier, as the pattern
      // discovered would not exactly match the pattern described in
      // the MDES. For the time being, if you want to support
      // commutative operations, both forms must be specified in the
      // MDES. E.g. both of the following patterns must be specified
      // separately, even though & is commutative:
      //
      // r1 = r2 << 16
      // r3 = r1 & r4
      //
      // r1 = r2 << 16
      // r3 = r4 & r1

//       if(MDES_commutative(el_opcode_to_string_map[p1.opc]))
// 	return (p1.opc == p2.opc);
//       else
      return ((p1.opcs.intersects(p2.opcs)) && (p1.oper_num == p2.oper_num));
    case PM_ANY_INPUT:
      return true;
    case PM_OPERATION_OUTPUT:
    case PM_ANY_OUTPUT:
    case PM_CONST:
    case PM_NONE:
    default:
      return false;
    };

  case PM_OPERATION_OUTPUT:
    switch(p2.type) {
    case PM_OPERATION_OUTPUT:
      return ((p1.opcs.intersects(p2.opcs)) && (p1.oper_num == p2.oper_num));
    case PM_ANY_OUTPUT:
      return true;
    case PM_OPERATION_INPUT:
    case PM_ANY_INPUT:
    case PM_CONST:
    case PM_NONE:
    default:
      return false;
    };

  case PM_ANY_INPUT:
    switch(p2.type) {
    case PM_OPERATION_INPUT:
    case PM_ANY_INPUT:
      return true;
    case PM_OPERATION_OUTPUT:
    case PM_ANY_OUTPUT:
    case PM_CONST:
    case PM_NONE:
    default:
      return false;
    };

  case PM_ANY_OUTPUT:
    switch(p2.type) {
    case PM_OPERATION_OUTPUT:
    case PM_ANY_OUTPUT:
      return true;
    case PM_OPERATION_INPUT:
    case PM_ANY_INPUT:
    case PM_CONST:
    case PM_NONE:
    default:
      return false;
    };

  case PM_CONST:
    switch(p2.type) {
    case PM_CONST:
      return p1.cset->overlaps(p2.cset);
    case PM_OPERATION_INPUT:
    case PM_OPERATION_OUTPUT:
    case PM_ANY_INPUT:
    case PM_ANY_OUTPUT:
    case PM_NONE:
    default:
      return false;
    };

  case PM_NONE:
  default:
    return false;
  };

  // To make gcc happy
  return false;
}


ostream& operator<<(ostream& os, const Pattern_match_node& n)
{
  switch(n.type) {
  case PM_CONST:
    os << "Const: " << *(n.cset);
    break;
  case PM_OPERATION_INPUT:
    os << "Operation input: " << n.opcs << '\t' << n.oper_num << endl;
    break;
  case PM_OPERATION_OUTPUT:
    os << "Operation output: " << n.opcs << '\t' << n.oper_num << endl;
    break;
  case PM_ANY_INPUT:
    os << "Any input" << endl;
    break;
  case PM_ANY_OUTPUT:
    os << "Any output" << endl;
    break;
  case PM_NONE:
    os << "None" << endl;
    break;
  default:
    break;
  };

  return os;
}
