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
//      File:           opcode_load_store.h
//      Authors:        Sadun Anik, Scott Mahlke
//      Created:        May 1995
//      Description:    Playdoh's set of load store  operations
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OPCODE_LOAD_STORE_H
#define _OPCODE_LOAD_STORE_H
#include "opcode.h"

enum Opcode_load_store {

////////////////////////////////////////////////////////////////////////////////////
//
// Playdoh's mongo set of memory system opcodes
//
////////////////////////////////////////////////////////////////////////////////////

//
//	Standard GPR load ops    
//

   L_B_V1_V1 = ROOT_LD | IR_BYTE | IR_SRC_V1 | IR_DEST_V1,
   L_B_V1_C1 = ROOT_LD | IR_BYTE | IR_SRC_V1 | IR_DEST_C1,
   L_B_V1_C2 = ROOT_LD | IR_BYTE | IR_SRC_V1 | IR_DEST_C2,
   L_B_V1_C3 = ROOT_LD | IR_BYTE | IR_SRC_V1 | IR_DEST_C3,
   L_B_C1_V1 = ROOT_LD | IR_BYTE | IR_SRC_C1 | IR_DEST_V1,
   L_B_C1_C1 = ROOT_LD | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   L_B_C1_C2 = ROOT_LD | IR_BYTE | IR_SRC_C1 | IR_DEST_C2,
   L_B_C1_C3 = ROOT_LD | IR_BYTE | IR_SRC_C1 | IR_DEST_C3,
   L_B_C2_V1 = ROOT_LD | IR_BYTE | IR_SRC_C2 | IR_DEST_V1,
   L_B_C2_C1 = ROOT_LD | IR_BYTE | IR_SRC_C2 | IR_DEST_C1,
   L_B_C2_C2 = ROOT_LD | IR_BYTE | IR_SRC_C2 | IR_DEST_C2,
   L_B_C2_C3 = ROOT_LD | IR_BYTE | IR_SRC_C2 | IR_DEST_C3,
   L_B_C3_V1 = ROOT_LD | IR_BYTE | IR_SRC_C3 | IR_DEST_V1,
   L_B_C3_C1 = ROOT_LD | IR_BYTE | IR_SRC_C3 | IR_DEST_C1,
   L_B_C3_C2 = ROOT_LD | IR_BYTE | IR_SRC_C3 | IR_DEST_C2,
   L_B_C3_C3 = ROOT_LD | IR_BYTE | IR_SRC_C3 | IR_DEST_C3,

   L_H_V1_V1 = ROOT_LD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_V1,
   L_H_V1_C1 = ROOT_LD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C1,
   L_H_V1_C2 = ROOT_LD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C2,
   L_H_V1_C3 = ROOT_LD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C3,
   L_H_C1_V1 = ROOT_LD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_V1,
   L_H_C1_C1 = ROOT_LD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   L_H_C1_C2 = ROOT_LD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C2,
   L_H_C1_C3 = ROOT_LD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C3,
   L_H_C2_V1 = ROOT_LD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_V1,
   L_H_C2_C1 = ROOT_LD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C1,
   L_H_C2_C2 = ROOT_LD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C2,
   L_H_C2_C3 = ROOT_LD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C3,
   L_H_C3_V1 = ROOT_LD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_V1,
   L_H_C3_C1 = ROOT_LD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C1,
   L_H_C3_C2 = ROOT_LD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C2,
   L_H_C3_C3 = ROOT_LD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C3,

   L_W_V1_V1 = ROOT_LD | IR_SRC_V1 | IR_DEST_V1,
   L_W_V1_C1 = ROOT_LD | IR_SRC_V1 | IR_DEST_C1,
   L_W_V1_C2 = ROOT_LD | IR_SRC_V1 | IR_DEST_C2,
   L_W_V1_C3 = ROOT_LD | IR_SRC_V1 | IR_DEST_C3,
   L_W_C1_V1 = ROOT_LD | IR_SRC_C1 | IR_DEST_V1,
   L_W_C1_C1 = ROOT_LD | IR_SRC_C1 | IR_DEST_C1,
   L_W_C1_C2 = ROOT_LD | IR_SRC_C1 | IR_DEST_C2,
   L_W_C1_C3 = ROOT_LD | IR_SRC_C1 | IR_DEST_C3,
   L_W_C2_V1 = ROOT_LD | IR_SRC_C2 | IR_DEST_V1,
   L_W_C2_C1 = ROOT_LD | IR_SRC_C2 | IR_DEST_C1,
   L_W_C2_C2 = ROOT_LD | IR_SRC_C2 | IR_DEST_C2,
   L_W_C2_C3 = ROOT_LD | IR_SRC_C2 | IR_DEST_C3,
   L_W_C3_V1 = ROOT_LD | IR_SRC_C3 | IR_DEST_V1,
   L_W_C3_C1 = ROOT_LD | IR_SRC_C3 | IR_DEST_C1,
   L_W_C3_C2 = ROOT_LD | IR_SRC_C3 | IR_DEST_C2,
   L_W_C3_C3 = ROOT_LD | IR_SRC_C3 | IR_DEST_C3,

   L_Q_V1_V1 = ROOT_LD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_V1,
   L_Q_V1_C1 = ROOT_LD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C1,
   L_Q_V1_C2 = ROOT_LD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C2,
   L_Q_V1_C3 = ROOT_LD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C3,
   L_Q_C1_V1 = ROOT_LD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_V1,
   L_Q_C1_C1 = ROOT_LD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C1,
   L_Q_C1_C2 = ROOT_LD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C2,
   L_Q_C1_C3 = ROOT_LD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C3,
   L_Q_C2_V1 = ROOT_LD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_V1,
   L_Q_C2_C1 = ROOT_LD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C1,
   L_Q_C2_C2 = ROOT_LD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C2,
   L_Q_C2_C3 = ROOT_LD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C3,
   L_Q_C3_V1 = ROOT_LD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_V1,
   L_Q_C3_C1 = ROOT_LD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C1,
   L_Q_C3_C2 = ROOT_LD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C2,
   L_Q_C3_C3 = ROOT_LD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C3,

//
//	Standard GPR load ops with sign extension
//

   LX_B_V1_V1 = ROOT_LDX | IR_BYTE | IR_SRC_V1 | IR_DEST_V1,
   LX_B_V1_C1 = ROOT_LDX | IR_BYTE | IR_SRC_V1 | IR_DEST_C1,
   LX_B_V1_C2 = ROOT_LDX | IR_BYTE | IR_SRC_V1 | IR_DEST_C2,
   LX_B_V1_C3 = ROOT_LDX | IR_BYTE | IR_SRC_V1 | IR_DEST_C3,
   LX_B_C1_V1 = ROOT_LDX | IR_BYTE | IR_SRC_C1 | IR_DEST_V1,
   LX_B_C1_C1 = ROOT_LDX | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   LX_B_C1_C2 = ROOT_LDX | IR_BYTE | IR_SRC_C1 | IR_DEST_C2,
   LX_B_C1_C3 = ROOT_LDX | IR_BYTE | IR_SRC_C1 | IR_DEST_C3,
   LX_B_C2_V1 = ROOT_LDX | IR_BYTE | IR_SRC_C2 | IR_DEST_V1,
   LX_B_C2_C1 = ROOT_LDX | IR_BYTE | IR_SRC_C2 | IR_DEST_C1,
   LX_B_C2_C2 = ROOT_LDX | IR_BYTE | IR_SRC_C2 | IR_DEST_C2,
   LX_B_C2_C3 = ROOT_LDX | IR_BYTE | IR_SRC_C2 | IR_DEST_C3,
   LX_B_C3_V1 = ROOT_LDX | IR_BYTE | IR_SRC_C3 | IR_DEST_V1,
   LX_B_C3_C1 = ROOT_LDX | IR_BYTE | IR_SRC_C3 | IR_DEST_C1,
   LX_B_C3_C2 = ROOT_LDX | IR_BYTE | IR_SRC_C3 | IR_DEST_C2,
   LX_B_C3_C3 = ROOT_LDX | IR_BYTE | IR_SRC_C3 | IR_DEST_C3,

   LX_H_V1_V1 = ROOT_LDX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_V1,
   LX_H_V1_C1 = ROOT_LDX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C1,
   LX_H_V1_C2 = ROOT_LDX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C2,
   LX_H_V1_C3 = ROOT_LDX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C3,
   LX_H_C1_V1 = ROOT_LDX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_V1,
   LX_H_C1_C1 = ROOT_LDX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   LX_H_C1_C2 = ROOT_LDX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C2,
   LX_H_C1_C3 = ROOT_LDX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C3,
   LX_H_C2_V1 = ROOT_LDX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_V1,
   LX_H_C2_C1 = ROOT_LDX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C1,
   LX_H_C2_C2 = ROOT_LDX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C2,
   LX_H_C2_C3 = ROOT_LDX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C3,
   LX_H_C3_V1 = ROOT_LDX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_V1,
   LX_H_C3_C1 = ROOT_LDX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C1,
   LX_H_C3_C2 = ROOT_LDX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C2,
   LX_H_C3_C3 = ROOT_LDX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C3,

   LX_W_V1_V1 = ROOT_LDX | IR_SRC_V1 | IR_DEST_V1,
   LX_W_V1_C1 = ROOT_LDX | IR_SRC_V1 | IR_DEST_C1,
   LX_W_V1_C2 = ROOT_LDX | IR_SRC_V1 | IR_DEST_C2,
   LX_W_V1_C3 = ROOT_LDX | IR_SRC_V1 | IR_DEST_C3,
   LX_W_C1_V1 = ROOT_LDX | IR_SRC_C1 | IR_DEST_V1,
   LX_W_C1_C1 = ROOT_LDX | IR_SRC_C1 | IR_DEST_C1,
   LX_W_C1_C2 = ROOT_LDX | IR_SRC_C1 | IR_DEST_C2,
   LX_W_C1_C3 = ROOT_LDX | IR_SRC_C1 | IR_DEST_C3,
   LX_W_C2_V1 = ROOT_LDX | IR_SRC_C2 | IR_DEST_V1,
   LX_W_C2_C1 = ROOT_LDX | IR_SRC_C2 | IR_DEST_C1,
   LX_W_C2_C2 = ROOT_LDX | IR_SRC_C2 | IR_DEST_C2,
   LX_W_C2_C3 = ROOT_LDX | IR_SRC_C2 | IR_DEST_C3,
   LX_W_C3_V1 = ROOT_LDX | IR_SRC_C3 | IR_DEST_V1,
   LX_W_C3_C1 = ROOT_LDX | IR_SRC_C3 | IR_DEST_C1,
   LX_W_C3_C2 = ROOT_LDX | IR_SRC_C3 | IR_DEST_C2,
   LX_W_C3_C3 = ROOT_LDX | IR_SRC_C3 | IR_DEST_C3,

   LX_Q_V1_V1 = ROOT_LDX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_V1,
   LX_Q_V1_C1 = ROOT_LDX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C1,
   LX_Q_V1_C2 = ROOT_LDX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C2,
   LX_Q_V1_C3 = ROOT_LDX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C3,
   LX_Q_C1_V1 = ROOT_LDX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_V1,
   LX_Q_C1_C1 = ROOT_LDX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C1,
   LX_Q_C1_C2 = ROOT_LDX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C2,
   LX_Q_C1_C3 = ROOT_LDX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C3,
   LX_Q_C2_V1 = ROOT_LDX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_V1,
   LX_Q_C2_C1 = ROOT_LDX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C1,
   LX_Q_C2_C2 = ROOT_LDX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C2,
   LX_Q_C2_C3 = ROOT_LDX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C3,
   LX_Q_C3_V1 = ROOT_LDX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_V1,
   LX_Q_C3_C1 = ROOT_LDX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C1,
   LX_Q_C3_C2 = ROOT_LDX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C2,
   LX_Q_C3_C3 = ROOT_LDX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C3,

//
//	Standard GPR load ops with base + offset gpr or short literal
//

   LG_B_V1_V1 = ROOT_LG | IR_BYTE | IR_SRC_V1 | IR_DEST_V1,
   LG_B_V1_C1 = ROOT_LG | IR_BYTE | IR_SRC_V1 | IR_DEST_C1,
   LG_B_V1_C2 = ROOT_LG | IR_BYTE | IR_SRC_V1 | IR_DEST_C2,
   LG_B_V1_C3 = ROOT_LG | IR_BYTE | IR_SRC_V1 | IR_DEST_C3,
   LG_B_C1_V1 = ROOT_LG | IR_BYTE | IR_SRC_C1 | IR_DEST_V1,
   LG_B_C1_C1 = ROOT_LG | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   LG_B_C1_C2 = ROOT_LG | IR_BYTE | IR_SRC_C1 | IR_DEST_C2,
   LG_B_C1_C3 = ROOT_LG | IR_BYTE | IR_SRC_C1 | IR_DEST_C3,
   LG_B_C2_V1 = ROOT_LG | IR_BYTE | IR_SRC_C2 | IR_DEST_V1,
   LG_B_C2_C1 = ROOT_LG | IR_BYTE | IR_SRC_C2 | IR_DEST_C1,
   LG_B_C2_C2 = ROOT_LG | IR_BYTE | IR_SRC_C2 | IR_DEST_C2,
   LG_B_C2_C3 = ROOT_LG | IR_BYTE | IR_SRC_C2 | IR_DEST_C3,
   LG_B_C3_V1 = ROOT_LG | IR_BYTE | IR_SRC_C3 | IR_DEST_V1,
   LG_B_C3_C1 = ROOT_LG | IR_BYTE | IR_SRC_C3 | IR_DEST_C1,
   LG_B_C3_C2 = ROOT_LG | IR_BYTE | IR_SRC_C3 | IR_DEST_C2,
   LG_B_C3_C3 = ROOT_LG | IR_BYTE | IR_SRC_C3 | IR_DEST_C3,

   LG_H_V1_V1 = ROOT_LG | IR_HALFWORD | IR_SRC_V1 | IR_DEST_V1,
   LG_H_V1_C1 = ROOT_LG | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C1,
   LG_H_V1_C2 = ROOT_LG | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C2,
   LG_H_V1_C3 = ROOT_LG | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C3,
   LG_H_C1_V1 = ROOT_LG | IR_HALFWORD | IR_SRC_C1 | IR_DEST_V1,
   LG_H_C1_C1 = ROOT_LG | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   LG_H_C1_C2 = ROOT_LG | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C2,
   LG_H_C1_C3 = ROOT_LG | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C3,
   LG_H_C2_V1 = ROOT_LG | IR_HALFWORD | IR_SRC_C2 | IR_DEST_V1,
   LG_H_C2_C1 = ROOT_LG | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C1,
   LG_H_C2_C2 = ROOT_LG | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C2,
   LG_H_C2_C3 = ROOT_LG | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C3,
   LG_H_C3_V1 = ROOT_LG | IR_HALFWORD | IR_SRC_C3 | IR_DEST_V1,
   LG_H_C3_C1 = ROOT_LG | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C1,
   LG_H_C3_C2 = ROOT_LG | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C2,
   LG_H_C3_C3 = ROOT_LG | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C3,

   LG_W_V1_V1 = ROOT_LG | IR_SRC_V1 | IR_DEST_V1,
   LG_W_V1_C1 = ROOT_LG | IR_SRC_V1 | IR_DEST_C1,
   LG_W_V1_C2 = ROOT_LG | IR_SRC_V1 | IR_DEST_C2,
   LG_W_V1_C3 = ROOT_LG | IR_SRC_V1 | IR_DEST_C3,
   LG_W_C1_V1 = ROOT_LG | IR_SRC_C1 | IR_DEST_V1,
   LG_W_C1_C1 = ROOT_LG | IR_SRC_C1 | IR_DEST_C1,
   LG_W_C1_C2 = ROOT_LG | IR_SRC_C1 | IR_DEST_C2,
   LG_W_C1_C3 = ROOT_LG | IR_SRC_C1 | IR_DEST_C3,
   LG_W_C2_V1 = ROOT_LG | IR_SRC_C2 | IR_DEST_V1,
   LG_W_C2_C1 = ROOT_LG | IR_SRC_C2 | IR_DEST_C1,
   LG_W_C2_C2 = ROOT_LG | IR_SRC_C2 | IR_DEST_C2,
   LG_W_C2_C3 = ROOT_LG | IR_SRC_C2 | IR_DEST_C3,
   LG_W_C3_V1 = ROOT_LG | IR_SRC_C3 | IR_DEST_V1,
   LG_W_C3_C1 = ROOT_LG | IR_SRC_C3 | IR_DEST_C1,
   LG_W_C3_C2 = ROOT_LG | IR_SRC_C3 | IR_DEST_C2,
   LG_W_C3_C3 = ROOT_LG | IR_SRC_C3 | IR_DEST_C3,

   LG_Q_V1_V1 = ROOT_LG | IR_QUADWORD | IR_SRC_V1 | IR_DEST_V1,
   LG_Q_V1_C1 = ROOT_LG | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C1,
   LG_Q_V1_C2 = ROOT_LG | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C2,
   LG_Q_V1_C3 = ROOT_LG | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C3,
   LG_Q_C1_V1 = ROOT_LG | IR_QUADWORD | IR_SRC_C1 | IR_DEST_V1,
   LG_Q_C1_C1 = ROOT_LG | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C1,
   LG_Q_C1_C2 = ROOT_LG | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C2,
   LG_Q_C1_C3 = ROOT_LG | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C3,
   LG_Q_C2_V1 = ROOT_LG | IR_QUADWORD | IR_SRC_C2 | IR_DEST_V1,
   LG_Q_C2_C1 = ROOT_LG | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C1,
   LG_Q_C2_C2 = ROOT_LG | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C2,
   LG_Q_C2_C3 = ROOT_LG | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C3,
   LG_Q_C3_V1 = ROOT_LG | IR_QUADWORD | IR_SRC_C3 | IR_DEST_V1,
   LG_Q_C3_C1 = ROOT_LG | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C1,
   LG_Q_C3_C2 = ROOT_LG | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C2,
   LG_Q_C3_C3 = ROOT_LG | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C3,

//
//	Standard GPR load ops with base + offset gpr or short literal
//        with sign extension
//

   LGX_B_V1_V1 = ROOT_LGX | IR_BYTE | IR_SRC_V1 | IR_DEST_V1,
   LGX_B_V1_C1 = ROOT_LGX | IR_BYTE | IR_SRC_V1 | IR_DEST_C1,
   LGX_B_V1_C2 = ROOT_LGX | IR_BYTE | IR_SRC_V1 | IR_DEST_C2,
   LGX_B_V1_C3 = ROOT_LGX | IR_BYTE | IR_SRC_V1 | IR_DEST_C3,
   LGX_B_C1_V1 = ROOT_LGX | IR_BYTE | IR_SRC_C1 | IR_DEST_V1,
   LGX_B_C1_C1 = ROOT_LGX | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   LGX_B_C1_C2 = ROOT_LGX | IR_BYTE | IR_SRC_C1 | IR_DEST_C2,
   LGX_B_C1_C3 = ROOT_LGX | IR_BYTE | IR_SRC_C1 | IR_DEST_C3,
   LGX_B_C2_V1 = ROOT_LGX | IR_BYTE | IR_SRC_C2 | IR_DEST_V1,
   LGX_B_C2_C1 = ROOT_LGX | IR_BYTE | IR_SRC_C2 | IR_DEST_C1,
   LGX_B_C2_C2 = ROOT_LGX | IR_BYTE | IR_SRC_C2 | IR_DEST_C2,
   LGX_B_C2_C3 = ROOT_LGX | IR_BYTE | IR_SRC_C2 | IR_DEST_C3,
   LGX_B_C3_V1 = ROOT_LGX | IR_BYTE | IR_SRC_C3 | IR_DEST_V1,
   LGX_B_C3_C1 = ROOT_LGX | IR_BYTE | IR_SRC_C3 | IR_DEST_C1,
   LGX_B_C3_C2 = ROOT_LGX | IR_BYTE | IR_SRC_C3 | IR_DEST_C2,
   LGX_B_C3_C3 = ROOT_LGX | IR_BYTE | IR_SRC_C3 | IR_DEST_C3,

   LGX_H_V1_V1 = ROOT_LGX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_V1,
   LGX_H_V1_C1 = ROOT_LGX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C1,
   LGX_H_V1_C2 = ROOT_LGX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C2,
   LGX_H_V1_C3 = ROOT_LGX | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C3,
   LGX_H_C1_V1 = ROOT_LGX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_V1,
   LGX_H_C1_C1 = ROOT_LGX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   LGX_H_C1_C2 = ROOT_LGX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C2,
   LGX_H_C1_C3 = ROOT_LGX | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C3,
   LGX_H_C2_V1 = ROOT_LGX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_V1,
   LGX_H_C2_C1 = ROOT_LGX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C1,
   LGX_H_C2_C2 = ROOT_LGX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C2,
   LGX_H_C2_C3 = ROOT_LGX | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C3,
   LGX_H_C3_V1 = ROOT_LGX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_V1,
   LGX_H_C3_C1 = ROOT_LGX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C1,
   LGX_H_C3_C2 = ROOT_LGX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C2,
   LGX_H_C3_C3 = ROOT_LGX | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C3,

   LGX_W_V1_V1 = ROOT_LGX | IR_SRC_V1 | IR_DEST_V1,
   LGX_W_V1_C1 = ROOT_LGX | IR_SRC_V1 | IR_DEST_C1,
   LGX_W_V1_C2 = ROOT_LGX | IR_SRC_V1 | IR_DEST_C2,
   LGX_W_V1_C3 = ROOT_LGX | IR_SRC_V1 | IR_DEST_C3,
   LGX_W_C1_V1 = ROOT_LGX | IR_SRC_C1 | IR_DEST_V1,
   LGX_W_C1_C1 = ROOT_LGX | IR_SRC_C1 | IR_DEST_C1,
   LGX_W_C1_C2 = ROOT_LGX | IR_SRC_C1 | IR_DEST_C2,
   LGX_W_C1_C3 = ROOT_LGX | IR_SRC_C1 | IR_DEST_C3,
   LGX_W_C2_V1 = ROOT_LGX | IR_SRC_C2 | IR_DEST_V1,
   LGX_W_C2_C1 = ROOT_LGX | IR_SRC_C2 | IR_DEST_C1,
   LGX_W_C2_C2 = ROOT_LGX | IR_SRC_C2 | IR_DEST_C2,
   LGX_W_C2_C3 = ROOT_LGX | IR_SRC_C2 | IR_DEST_C3,
   LGX_W_C3_V1 = ROOT_LGX | IR_SRC_C3 | IR_DEST_V1,
   LGX_W_C3_C1 = ROOT_LGX | IR_SRC_C3 | IR_DEST_C1,
   LGX_W_C3_C2 = ROOT_LGX | IR_SRC_C3 | IR_DEST_C2,
   LGX_W_C3_C3 = ROOT_LGX | IR_SRC_C3 | IR_DEST_C3,

   LGX_Q_V1_V1 = ROOT_LGX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_V1,
   LGX_Q_V1_C1 = ROOT_LGX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C1,
   LGX_Q_V1_C2 = ROOT_LGX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C2,
   LGX_Q_V1_C3 = ROOT_LGX | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C3,
   LGX_Q_C1_V1 = ROOT_LGX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_V1,
   LGX_Q_C1_C1 = ROOT_LGX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C1,
   LGX_Q_C1_C2 = ROOT_LGX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C2,
   LGX_Q_C1_C3 = ROOT_LGX | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C3,
   LGX_Q_C2_V1 = ROOT_LGX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_V1,
   LGX_Q_C2_C1 = ROOT_LGX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C1,
   LGX_Q_C2_C2 = ROOT_LGX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C2,
   LGX_Q_C2_C3 = ROOT_LGX | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C3,
   LGX_Q_C3_V1 = ROOT_LGX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_V1,
   LGX_Q_C3_C1 = ROOT_LGX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C1,
   LGX_Q_C3_C2 = ROOT_LGX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C2,
   LGX_Q_C3_C3 = ROOT_LGX | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C3,

//
//	Standard GPR load ops with post increment
//

   LI_B_V1_V1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_V1,
   LI_B_V1_C1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_C1,
   LI_B_V1_C2 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_C2,
   LI_B_V1_C3 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_C3,
   LI_B_C1_V1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_V1,
   LI_B_C1_C1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   LI_B_C1_C2 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_C2,
   LI_B_C1_C3 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_C3,
   LI_B_C2_V1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_V1,
   LI_B_C2_C1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_C1,
   LI_B_C2_C2 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_C2,
   LI_B_C2_C3 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_C3,
   LI_B_C3_V1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_V1,
   LI_B_C3_C1 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_C1,
   LI_B_C3_C2 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_C2,
   LI_B_C3_C3 = ROOT_LD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_C3,

   LI_H_V1_V1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_V1,
   LI_H_V1_C1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C1,
   LI_H_V1_C2 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C2,
   LI_H_V1_C3 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C3,
   LI_H_C1_V1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_V1,
   LI_H_C1_C1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   LI_H_C1_C2 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C2,
   LI_H_C1_C3 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C3,
   LI_H_C2_V1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_V1,
   LI_H_C2_C1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C1,
   LI_H_C2_C2 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C2,
   LI_H_C2_C3 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C3,
   LI_H_C3_V1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_V1,
   LI_H_C3_C1 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C1,
   LI_H_C3_C2 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C2,
   LI_H_C3_C3 = ROOT_LD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C3,

   LI_W_V1_V1 = ROOT_LD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_V1,
   LI_W_V1_C1 = ROOT_LD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C1,
   LI_W_V1_C2 = ROOT_LD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C2,
   LI_W_V1_C3 = ROOT_LD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C3,
   LI_W_C1_V1 = ROOT_LD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_V1,
   LI_W_C1_C1 = ROOT_LD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C1,
   LI_W_C1_C2 = ROOT_LD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C2,
   LI_W_C1_C3 = ROOT_LD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C3,
   LI_W_C2_V1 = ROOT_LD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_V1,
   LI_W_C2_C1 = ROOT_LD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C1,
   LI_W_C2_C2 = ROOT_LD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C2,
   LI_W_C2_C3 = ROOT_LD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C3,
   LI_W_C3_V1 = ROOT_LD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_V1,
   LI_W_C3_C1 = ROOT_LD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C1,
   LI_W_C3_C2 = ROOT_LD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C2,
   LI_W_C3_C3 = ROOT_LD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C3,

   LI_Q_V1_V1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_V1,
   LI_Q_V1_C1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C1,
   LI_Q_V1_C2 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C2,
   LI_Q_V1_C3 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C3,
   LI_Q_C1_V1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_V1,
   LI_Q_C1_C1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C1,
   LI_Q_C1_C2 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C2,
   LI_Q_C1_C3 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C3,
   LI_Q_C2_V1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_V1,
   LI_Q_C2_C1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C1,
   LI_Q_C2_C2 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C2,
   LI_Q_C2_C3 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C3,
   LI_Q_C3_V1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_V1,
   LI_Q_C3_C1 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C1,
   LI_Q_C3_C2 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C2,
   LI_Q_C3_C3 = ROOT_LD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C3,

//
//	Standard FPR load ops    
//

   FL_S_V1_V1 = ROOT_FLD | IR_SRC_V1 | IR_DEST_V1,
   FL_S_V1_C1 = ROOT_FLD | IR_SRC_V1 | IR_DEST_C1,
   FL_S_V1_C2 = ROOT_FLD | IR_SRC_V1 | IR_DEST_C2,
   FL_S_V1_C3 = ROOT_FLD | IR_SRC_V1 | IR_DEST_C3,
   FL_S_C1_V1 = ROOT_FLD | IR_SRC_C1 | IR_DEST_V1,
   FL_S_C1_C1 = ROOT_FLD | IR_SRC_C1 | IR_DEST_C1,
   FL_S_C1_C2 = ROOT_FLD | IR_SRC_C1 | IR_DEST_C2,
   FL_S_C1_C3 = ROOT_FLD | IR_SRC_C1 | IR_DEST_C3,
   FL_S_C2_V1 = ROOT_FLD | IR_SRC_C2 | IR_DEST_V1,
   FL_S_C2_C1 = ROOT_FLD | IR_SRC_C2 | IR_DEST_C1,
   FL_S_C2_C2 = ROOT_FLD | IR_SRC_C2 | IR_DEST_C2,
   FL_S_C2_C3 = ROOT_FLD | IR_SRC_C2 | IR_DEST_C3,
   FL_S_C3_V1 = ROOT_FLD | IR_SRC_C3 | IR_DEST_V1,
   FL_S_C3_C1 = ROOT_FLD | IR_SRC_C3 | IR_DEST_C1,
   FL_S_C3_C2 = ROOT_FLD | IR_SRC_C3 | IR_DEST_C2,
   FL_S_C3_C3 = ROOT_FLD | IR_SRC_C3 | IR_DEST_C3,

   FL_D_V1_V1 = ROOT_FLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_V1,
   FL_D_V1_C1 = ROOT_FLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C1,
   FL_D_V1_C2 = ROOT_FLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C2,
   FL_D_V1_C3 = ROOT_FLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C3,
   FL_D_C1_V1 = ROOT_FLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_V1,
   FL_D_C1_C1 = ROOT_FLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,
   FL_D_C1_C2 = ROOT_FLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C2,
   FL_D_C1_C3 = ROOT_FLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C3,
   FL_D_C2_V1 = ROOT_FLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_V1,
   FL_D_C2_C1 = ROOT_FLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C1,
   FL_D_C2_C2 = ROOT_FLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C2,
   FL_D_C2_C3 = ROOT_FLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C3,
   FL_D_C3_V1 = ROOT_FLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_V1,
   FL_D_C3_C1 = ROOT_FLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C1,
   FL_D_C3_C2 = ROOT_FLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C2,
   FL_D_C3_C3 = ROOT_FLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C3,

//
//	FPR load ops with base + offset gpr or short literal
//

   FLG_S_V1_V1 = ROOT_FLG | IR_SRC_V1 | IR_DEST_V1,
   FLG_S_V1_C1 = ROOT_FLG | IR_SRC_V1 | IR_DEST_C1,
   FLG_S_V1_C2 = ROOT_FLG | IR_SRC_V1 | IR_DEST_C2,
   FLG_S_V1_C3 = ROOT_FLG | IR_SRC_V1 | IR_DEST_C3,
   FLG_S_C1_V1 = ROOT_FLG | IR_SRC_C1 | IR_DEST_V1,
   FLG_S_C1_C1 = ROOT_FLG | IR_SRC_C1 | IR_DEST_C1,
   FLG_S_C1_C2 = ROOT_FLG | IR_SRC_C1 | IR_DEST_C2,
   FLG_S_C1_C3 = ROOT_FLG | IR_SRC_C1 | IR_DEST_C3,
   FLG_S_C2_V1 = ROOT_FLG | IR_SRC_C2 | IR_DEST_V1,
   FLG_S_C2_C1 = ROOT_FLG | IR_SRC_C2 | IR_DEST_C1,
   FLG_S_C2_C2 = ROOT_FLG | IR_SRC_C2 | IR_DEST_C2,
   FLG_S_C2_C3 = ROOT_FLG | IR_SRC_C2 | IR_DEST_C3,
   FLG_S_C3_V1 = ROOT_FLG | IR_SRC_C3 | IR_DEST_V1,
   FLG_S_C3_C1 = ROOT_FLG | IR_SRC_C3 | IR_DEST_C1,
   FLG_S_C3_C2 = ROOT_FLG | IR_SRC_C3 | IR_DEST_C2,
   FLG_S_C3_C3 = ROOT_FLG | IR_SRC_C3 | IR_DEST_C3,

   FLG_D_V1_V1 = ROOT_FLG | IR_DOUBLE | IR_SRC_V1 | IR_DEST_V1,
   FLG_D_V1_C1 = ROOT_FLG | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C1,
   FLG_D_V1_C2 = ROOT_FLG | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C2,
   FLG_D_V1_C3 = ROOT_FLG | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C3,
   FLG_D_C1_V1 = ROOT_FLG | IR_DOUBLE | IR_SRC_C1 | IR_DEST_V1,
   FLG_D_C1_C1 = ROOT_FLG | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,
   FLG_D_C1_C2 = ROOT_FLG | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C2,
   FLG_D_C1_C3 = ROOT_FLG | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C3,
   FLG_D_C2_V1 = ROOT_FLG | IR_DOUBLE | IR_SRC_C2 | IR_DEST_V1,
   FLG_D_C2_C1 = ROOT_FLG | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C1,
   FLG_D_C2_C2 = ROOT_FLG | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C2,
   FLG_D_C2_C3 = ROOT_FLG | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C3,
   FLG_D_C3_V1 = ROOT_FLG | IR_DOUBLE | IR_SRC_C3 | IR_DEST_V1,
   FLG_D_C3_C1 = ROOT_FLG | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C1,
   FLG_D_C3_C2 = ROOT_FLG | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C2,
   FLG_D_C3_C3 = ROOT_FLG | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C3,

//
//	Standard FPR load ops with post increment
//

   FLI_S_V1_V1 = ROOT_FLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_V1,
   FLI_S_V1_C1 = ROOT_FLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C1,
   FLI_S_V1_C2 = ROOT_FLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C2,
   FLI_S_V1_C3 = ROOT_FLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C3,
   FLI_S_C1_V1 = ROOT_FLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_V1,
   FLI_S_C1_C1 = ROOT_FLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C1,
   FLI_S_C1_C2 = ROOT_FLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C2,
   FLI_S_C1_C3 = ROOT_FLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C3,
   FLI_S_C2_V1 = ROOT_FLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_V1,
   FLI_S_C2_C1 = ROOT_FLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C1,
   FLI_S_C2_C2 = ROOT_FLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C2,
   FLI_S_C2_C3 = ROOT_FLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C3,
   FLI_S_C3_V1 = ROOT_FLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_V1,
   FLI_S_C3_C1 = ROOT_FLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C1,
   FLI_S_C3_C2 = ROOT_FLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C2,
   FLI_S_C3_C3 = ROOT_FLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C3,

   FLI_D_V1_V1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_V1,
   FLI_D_V1_C1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C1,
   FLI_D_V1_C2 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C2,
   FLI_D_V1_C3 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C3,
   FLI_D_C1_V1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_V1,
   FLI_D_C1_C1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,
   FLI_D_C1_C2 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C2,
   FLI_D_C1_C3 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C3,
   FLI_D_C2_V1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_V1,
   FLI_D_C2_C1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C1,
   FLI_D_C2_C2 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C2,
   FLI_D_C2_C3 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C3,
   FLI_D_C3_V1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_V1,
   FLI_D_C3_C1 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C1,
   FLI_D_C3_C2 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C2,
   FLI_D_C3_C3 = ROOT_FLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C3,

//
//	Standard GPR store ops    
//

   S_B_V1    = ROOT_ST | IR_BYTE | IR_DEST_V1,
   S_B_C1    = ROOT_ST | IR_BYTE | IR_DEST_C1,
   S_B_C2    = ROOT_ST | IR_BYTE | IR_DEST_C2,
   S_B_C3    = ROOT_ST | IR_BYTE | IR_DEST_C3,

   S_H_V1    = ROOT_ST | IR_HALFWORD | IR_DEST_V1,
   S_H_C1    = ROOT_ST | IR_HALFWORD | IR_DEST_C1,
   S_H_C2    = ROOT_ST | IR_HALFWORD | IR_DEST_C2,
   S_H_C3    = ROOT_ST | IR_HALFWORD | IR_DEST_C3,

   S_W_V1    = ROOT_ST | IR_DEST_V1,
   S_W_C1    = ROOT_ST | IR_DEST_C1,
   S_W_C2    = ROOT_ST | IR_DEST_C2,
   S_W_C3    = ROOT_ST | IR_DEST_C3,

   S_Q_V1    = ROOT_ST | IR_QUADWORD | IR_DEST_V1,
   S_Q_C1    = ROOT_ST | IR_QUADWORD | IR_DEST_C1,
   S_Q_C2    = ROOT_ST | IR_QUADWORD | IR_DEST_C2,
   S_Q_C3    = ROOT_ST | IR_QUADWORD | IR_DEST_C3,

//
//	GPR store ops with base + offset gpr or short literal
//

   SG_B_V1    = ROOT_SG | IR_BYTE | IR_DEST_V1,
   SG_B_C1    = ROOT_SG | IR_BYTE | IR_DEST_C1,
   SG_B_C2    = ROOT_SG | IR_BYTE | IR_DEST_C2,
   SG_B_C3    = ROOT_SG | IR_BYTE | IR_DEST_C3,

   SG_H_V1    = ROOT_SG | IR_HALFWORD | IR_DEST_V1,
   SG_H_C1    = ROOT_SG | IR_HALFWORD | IR_DEST_C1,
   SG_H_C2    = ROOT_SG | IR_HALFWORD | IR_DEST_C2,
   SG_H_C3    = ROOT_SG | IR_HALFWORD | IR_DEST_C3,

   SG_W_V1    = ROOT_SG | IR_DEST_V1,
   SG_W_C1    = ROOT_SG | IR_DEST_C1,
   SG_W_C2    = ROOT_SG | IR_DEST_C2,
   SG_W_C3    = ROOT_SG | IR_DEST_C3,

   SG_Q_V1    = ROOT_SG | IR_QUADWORD | IR_DEST_V1,
   SG_Q_C1    = ROOT_SG | IR_QUADWORD | IR_DEST_C1,
   SG_Q_C2    = ROOT_SG | IR_QUADWORD | IR_DEST_C2,
   SG_Q_C3    = ROOT_SG | IR_QUADWORD | IR_DEST_C3,

//
//	Standard GPR store ops with post increment
//

   SI_B_V1    = ROOT_ST | IR_INCREMENT | IR_BYTE | IR_DEST_V1,
   SI_B_C1    = ROOT_ST | IR_INCREMENT | IR_BYTE | IR_DEST_C1,
   SI_B_C2    = ROOT_ST | IR_INCREMENT | IR_BYTE | IR_DEST_C2,
   SI_B_C3    = ROOT_ST | IR_INCREMENT | IR_BYTE | IR_DEST_C3,

   SI_H_V1    = ROOT_ST | IR_INCREMENT | IR_HALFWORD | IR_DEST_V1,
   SI_H_C1    = ROOT_ST | IR_INCREMENT | IR_HALFWORD | IR_DEST_C1,
   SI_H_C2    = ROOT_ST | IR_INCREMENT | IR_HALFWORD | IR_DEST_C2,
   SI_H_C3    = ROOT_ST | IR_INCREMENT | IR_HALFWORD | IR_DEST_C3,

   SI_W_V1    = ROOT_ST | IR_INCREMENT | IR_DEST_V1,
   SI_W_C1    = ROOT_ST | IR_INCREMENT | IR_DEST_C1,
   SI_W_C2    = ROOT_ST | IR_INCREMENT | IR_DEST_C2,
   SI_W_C3    = ROOT_ST | IR_INCREMENT | IR_DEST_C3,

   SI_Q_V1    = ROOT_ST | IR_INCREMENT | IR_QUADWORD | IR_DEST_V1,
   SI_Q_C1    = ROOT_ST | IR_INCREMENT | IR_QUADWORD | IR_DEST_C1,
   SI_Q_C2    = ROOT_ST | IR_INCREMENT | IR_QUADWORD | IR_DEST_C2,
   SI_Q_C3    = ROOT_ST | IR_INCREMENT | IR_QUADWORD | IR_DEST_C3,

//
//	Standard FPR store ops    
//

   FS_S_V1    = ROOT_FST | IR_DEST_V1,
   FS_S_C1    = ROOT_FST | IR_DEST_C1,
   FS_S_C2    = ROOT_FST | IR_DEST_C2,
   FS_S_C3    = ROOT_FST | IR_DEST_C3,

   FS_D_V1    = ROOT_FST | IR_DOUBLE | IR_DEST_V1,
   FS_D_C1    = ROOT_FST | IR_DOUBLE | IR_DEST_C1,
   FS_D_C2    = ROOT_FST | IR_DOUBLE | IR_DEST_C2,
   FS_D_C3    = ROOT_FST | IR_DOUBLE | IR_DEST_C3,

//
//	FPR store ops with base + offset gpr or short literal
//

   FSG_S_V1    = ROOT_FSG | IR_DEST_V1,
   FSG_S_C1    = ROOT_FSG | IR_DEST_C1,
   FSG_S_C2    = ROOT_FSG | IR_DEST_C2,
   FSG_S_C3    = ROOT_FSG | IR_DEST_C3,

   FSG_D_V1    = ROOT_FSG | IR_DOUBLE | IR_DEST_V1,
   FSG_D_C1    = ROOT_FSG | IR_DOUBLE | IR_DEST_C1,
   FSG_D_C2    = ROOT_FSG | IR_DOUBLE | IR_DEST_C2,
   FSG_D_C3    = ROOT_FSG | IR_DOUBLE | IR_DEST_C3,

//
//	Standard FPR store ops with post increment
//

   FSI_S_V1    = ROOT_FST | IR_INCREMENT | IR_DEST_V1,
   FSI_S_C1    = ROOT_FST | IR_INCREMENT | IR_DEST_C1,
   FSI_S_C2    = ROOT_FST | IR_INCREMENT | IR_DEST_C2,
   FSI_S_C3    = ROOT_FST | IR_INCREMENT | IR_DEST_C3,

   FSI_D_V1    = ROOT_FST | IR_INCREMENT | IR_DOUBLE | IR_DEST_V1,
   FSI_D_C1    = ROOT_FST | IR_INCREMENT | IR_DOUBLE | IR_DEST_C1,
   FSI_D_C2    = ROOT_FST | IR_INCREMENT | IR_DOUBLE | IR_DEST_C2,
   FSI_D_C3    = ROOT_FST | IR_INCREMENT | IR_DOUBLE | IR_DEST_C3,

//
//	Data speculation loads to GPR
//

   LDS_B_V1_V1 = ROOT_DSLD | IR_BYTE | IR_SRC_V1 | IR_DEST_V1,
   LDS_B_V1_C1 = ROOT_DSLD | IR_BYTE | IR_SRC_V1 | IR_DEST_C1,
   LDS_B_V1_C2 = ROOT_DSLD | IR_BYTE | IR_SRC_V1 | IR_DEST_C2,
   LDS_B_V1_C3 = ROOT_DSLD | IR_BYTE | IR_SRC_V1 | IR_DEST_C3,
   LDS_B_C1_V1 = ROOT_DSLD | IR_BYTE | IR_SRC_C1 | IR_DEST_V1,
   LDS_B_C1_C1 = ROOT_DSLD | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   LDS_B_C1_C2 = ROOT_DSLD | IR_BYTE | IR_SRC_C1 | IR_DEST_C2,
   LDS_B_C1_C3 = ROOT_DSLD | IR_BYTE | IR_SRC_C1 | IR_DEST_C3,
   LDS_B_C2_V1 = ROOT_DSLD | IR_BYTE | IR_SRC_C2 | IR_DEST_V1,
   LDS_B_C2_C1 = ROOT_DSLD | IR_BYTE | IR_SRC_C2 | IR_DEST_C1,
   LDS_B_C2_C2 = ROOT_DSLD | IR_BYTE | IR_SRC_C2 | IR_DEST_C2,
   LDS_B_C2_C3 = ROOT_DSLD | IR_BYTE | IR_SRC_C2 | IR_DEST_C3,
   LDS_B_C3_V1 = ROOT_DSLD | IR_BYTE | IR_SRC_C3 | IR_DEST_V1,
   LDS_B_C3_C1 = ROOT_DSLD | IR_BYTE | IR_SRC_C3 | IR_DEST_C1,
   LDS_B_C3_C2 = ROOT_DSLD | IR_BYTE | IR_SRC_C3 | IR_DEST_C2,
   LDS_B_C3_C3 = ROOT_DSLD | IR_BYTE | IR_SRC_C3 | IR_DEST_C3,

   LDS_H_V1_V1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_V1,
   LDS_H_V1_C1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C1,
   LDS_H_V1_C2 = ROOT_DSLD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C2,
   LDS_H_V1_C3 = ROOT_DSLD | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C3,
   LDS_H_C1_V1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_V1,
   LDS_H_C1_C1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   LDS_H_C1_C2 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C2,
   LDS_H_C1_C3 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C3,
   LDS_H_C2_V1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_V1,
   LDS_H_C2_C1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C1,
   LDS_H_C2_C2 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C2,
   LDS_H_C2_C3 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C3,
   LDS_H_C3_V1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_V1,
   LDS_H_C3_C1 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C1,
   LDS_H_C3_C2 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C2,
   LDS_H_C3_C3 = ROOT_DSLD | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C3,

   LDS_W_V1_V1 = ROOT_DSLD | IR_SRC_V1 | IR_DEST_V1,
   LDS_W_V1_C1 = ROOT_DSLD | IR_SRC_V1 | IR_DEST_C1,
   LDS_W_V1_C2 = ROOT_DSLD | IR_SRC_V1 | IR_DEST_C2,
   LDS_W_V1_C3 = ROOT_DSLD | IR_SRC_V1 | IR_DEST_C3,
   LDS_W_C1_V1 = ROOT_DSLD | IR_SRC_C1 | IR_DEST_V1,
   LDS_W_C1_C1 = ROOT_DSLD | IR_SRC_C1 | IR_DEST_C1,
   LDS_W_C1_C2 = ROOT_DSLD | IR_SRC_C1 | IR_DEST_C2,
   LDS_W_C1_C3 = ROOT_DSLD | IR_SRC_C1 | IR_DEST_C3,
   LDS_W_C2_V1 = ROOT_DSLD | IR_SRC_C2 | IR_DEST_V1,
   LDS_W_C2_C1 = ROOT_DSLD | IR_SRC_C2 | IR_DEST_C1,
   LDS_W_C2_C2 = ROOT_DSLD | IR_SRC_C2 | IR_DEST_C2,
   LDS_W_C2_C3 = ROOT_DSLD | IR_SRC_C2 | IR_DEST_C3,
   LDS_W_C3_V1 = ROOT_DSLD | IR_SRC_C3 | IR_DEST_V1,
   LDS_W_C3_C1 = ROOT_DSLD | IR_SRC_C3 | IR_DEST_C1,
   LDS_W_C3_C2 = ROOT_DSLD | IR_SRC_C3 | IR_DEST_C2,
   LDS_W_C3_C3 = ROOT_DSLD | IR_SRC_C3 | IR_DEST_C3,

   LDS_Q_V1_V1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_V1,
   LDS_Q_V1_C1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C1,
   LDS_Q_V1_C2 = ROOT_DSLD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C2,
   LDS_Q_V1_C3 = ROOT_DSLD | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C3,
   LDS_Q_C1_V1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_V1,
   LDS_Q_C1_C1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C1,
   LDS_Q_C1_C2 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C2,
   LDS_Q_C1_C3 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C3,
   LDS_Q_C2_V1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_V1,
   LDS_Q_C2_C1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C1,
   LDS_Q_C2_C2 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C2,
   LDS_Q_C2_C3 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C3,
   LDS_Q_C3_V1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_V1,
   LDS_Q_C3_C1 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C1,
   LDS_Q_C3_C2 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C2,
   LDS_Q_C3_C3 = ROOT_DSLD | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C3,

//
//	Data speculation loads to GPR with post increment
//

   LDSI_B_V1_V1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_V1,
   LDSI_B_V1_C1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_C1,
   LDSI_B_V1_C2 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_C2,
   LDSI_B_V1_C3 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_V1 | IR_DEST_C3,
   LDSI_B_C1_V1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_V1,
   LDSI_B_C1_C1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   LDSI_B_C1_C2 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_C2,
   LDSI_B_C1_C3 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C1 | IR_DEST_C3,
   LDSI_B_C2_V1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_V1,
   LDSI_B_C2_C1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_C1,
   LDSI_B_C2_C2 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_C2,
   LDSI_B_C2_C3 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C2 | IR_DEST_C3,
   LDSI_B_C3_V1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_V1,
   LDSI_B_C3_C1 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_C1,
   LDSI_B_C3_C2 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_C2,
   LDSI_B_C3_C3 = ROOT_DSLD | IR_INCREMENT | IR_BYTE | IR_SRC_C3 | IR_DEST_C3,

   LDSI_H_V1_V1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_V1,
   LDSI_H_V1_C1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C1,
   LDSI_H_V1_C2 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C2,
   LDSI_H_V1_C3 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_V1 | IR_DEST_C3,
   LDSI_H_C1_V1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_V1,
   LDSI_H_C1_C1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   LDSI_H_C1_C2 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C2,
   LDSI_H_C1_C3 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C3,
   LDSI_H_C2_V1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_V1,
   LDSI_H_C2_C1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C1,
   LDSI_H_C2_C2 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C2,
   LDSI_H_C2_C3 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C2 | IR_DEST_C3,
   LDSI_H_C3_V1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_V1,
   LDSI_H_C3_C1 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C1,
   LDSI_H_C3_C2 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C2,
   LDSI_H_C3_C3 = ROOT_DSLD | IR_INCREMENT | IR_HALFWORD | IR_SRC_C3 | IR_DEST_C3,

   LDSI_W_V1_V1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_V1,
   LDSI_W_V1_C1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C1,
   LDSI_W_V1_C2 = ROOT_DSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C2,
   LDSI_W_V1_C3 = ROOT_DSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C3,
   LDSI_W_C1_V1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_V1,
   LDSI_W_C1_C1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C1,
   LDSI_W_C1_C2 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C2,
   LDSI_W_C1_C3 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C3,
   LDSI_W_C2_V1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_V1,
   LDSI_W_C2_C1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C1,
   LDSI_W_C2_C2 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C2,
   LDSI_W_C2_C3 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C3,
   LDSI_W_C3_V1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_V1,
   LDSI_W_C3_C1 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C1,
   LDSI_W_C3_C2 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C2,
   LDSI_W_C3_C3 = ROOT_DSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C3,

   LDSI_Q_V1_V1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_V1,
   LDSI_Q_V1_C1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C1,
   LDSI_Q_V1_C2 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C2,
   LDSI_Q_V1_C3 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_V1 | IR_DEST_C3,
   LDSI_Q_C1_V1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_V1,
   LDSI_Q_C1_C1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C1,
   LDSI_Q_C1_C2 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C2,
   LDSI_Q_C1_C3 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C1 | IR_DEST_C3,
   LDSI_Q_C2_V1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_V1,
   LDSI_Q_C2_C1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C1,
   LDSI_Q_C2_C2 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C2,
   LDSI_Q_C2_C3 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C2 | IR_DEST_C3,
   LDSI_Q_C3_V1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_V1,
   LDSI_Q_C3_C1 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C1,
   LDSI_Q_C3_C2 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C2,
   LDSI_Q_C3_C3 = ROOT_DSLD | IR_INCREMENT | IR_QUADWORD | IR_SRC_C3 | IR_DEST_C3,

//
//	Data speculation loads to FPR
//

   FLDS_S_V1_V1 = ROOT_FDSLD | IR_SRC_V1 | IR_DEST_V1,
   FLDS_S_V1_C1 = ROOT_FDSLD | IR_SRC_V1 | IR_DEST_C1,
   FLDS_S_V1_C2 = ROOT_FDSLD | IR_SRC_V1 | IR_DEST_C2,
   FLDS_S_V1_C3 = ROOT_FDSLD | IR_SRC_V1 | IR_DEST_C3,
   FLDS_S_C1_V1 = ROOT_FDSLD | IR_SRC_C1 | IR_DEST_V1,
   FLDS_S_C1_C1 = ROOT_FDSLD | IR_SRC_C1 | IR_DEST_C1,
   FLDS_S_C1_C2 = ROOT_FDSLD | IR_SRC_C1 | IR_DEST_C2,
   FLDS_S_C1_C3 = ROOT_FDSLD | IR_SRC_C1 | IR_DEST_C3,
   FLDS_S_C2_V1 = ROOT_FDSLD | IR_SRC_C2 | IR_DEST_V1,
   FLDS_S_C2_C1 = ROOT_FDSLD | IR_SRC_C2 | IR_DEST_C1,
   FLDS_S_C2_C2 = ROOT_FDSLD | IR_SRC_C2 | IR_DEST_C2,
   FLDS_S_C2_C3 = ROOT_FDSLD | IR_SRC_C2 | IR_DEST_C3,
   FLDS_S_C3_V1 = ROOT_FDSLD | IR_SRC_C3 | IR_DEST_V1,
   FLDS_S_C3_C1 = ROOT_FDSLD | IR_SRC_C3 | IR_DEST_C1,
   FLDS_S_C3_C2 = ROOT_FDSLD | IR_SRC_C3 | IR_DEST_C2,
   FLDS_S_C3_C3 = ROOT_FDSLD | IR_SRC_C3 | IR_DEST_C3,

   FLDS_D_V1_V1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_V1,
   FLDS_D_V1_C1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C1,
   FLDS_D_V1_C2 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C2,
   FLDS_D_V1_C3 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C3,
   FLDS_D_C1_V1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_V1,
   FLDS_D_C1_C1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,
   FLDS_D_C1_C2 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C2,
   FLDS_D_C1_C3 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C3,
   FLDS_D_C2_V1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_V1,
   FLDS_D_C2_C1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C1,
   FLDS_D_C2_C2 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C2,
   FLDS_D_C2_C3 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C3,
   FLDS_D_C3_V1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_V1,
   FLDS_D_C3_C1 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C1,
   FLDS_D_C3_C2 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C2,
   FLDS_D_C3_C3 = ROOT_FDSLD | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C3,

//
//	Data speculation loads to FPR with post increment
//

   FLDSI_S_V1_V1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_V1,
   FLDSI_S_V1_C1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C1,
   FLDSI_S_V1_C2 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C2,
   FLDSI_S_V1_C3 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_V1 | IR_DEST_C3,
   FLDSI_S_C1_V1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_V1,
   FLDSI_S_C1_C1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C1,
   FLDSI_S_C1_C2 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C2,
   FLDSI_S_C1_C3 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C1 | IR_DEST_C3,
   FLDSI_S_C2_V1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_V1,
   FLDSI_S_C2_C1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C1,
   FLDSI_S_C2_C2 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C2,
   FLDSI_S_C2_C3 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C2 | IR_DEST_C3,
   FLDSI_S_C3_V1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_V1,
   FLDSI_S_C3_C1 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C1,
   FLDSI_S_C3_C2 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C2,
   FLDSI_S_C3_C3 = ROOT_FDSLD | IR_INCREMENT | IR_SRC_C3 | IR_DEST_C3,

   FLDSI_D_V1_V1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_V1,
   FLDSI_D_V1_C1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C1,
   FLDSI_D_V1_C2 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C2,
   FLDSI_D_V1_C3 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_V1 | IR_DEST_C3,
   FLDSI_D_C1_V1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_V1,
   FLDSI_D_C1_C1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,
   FLDSI_D_C1_C2 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C2,
   FLDSI_D_C1_C3 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C3,
   FLDSI_D_C2_V1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_V1,
   FLDSI_D_C2_C1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C1,
   FLDSI_D_C2_C2 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C2,
   FLDSI_D_C2_C3 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C2 | IR_DEST_C3,
   FLDSI_D_C3_V1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_V1,
   FLDSI_D_C3_C1 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C1,
   FLDSI_D_C3_C2 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C2,
   FLDSI_D_C3_C3 = ROOT_FDSLD | IR_INCREMENT | IR_DOUBLE | IR_SRC_C3 | IR_DEST_C3,

   // New load/store operations to save/restore and spill/unspill GPRs,
   // FPRs and BTRs. These operations ignore the speculative tag bit of the
   // register being loaded or stored.
   SAVE         = ROOT_SAVE,
   RESTORE      = ROOT_RESTORE,
   FSAVE_S      = ROOT_FSAVE,
   FRESTORE_S   = ROOT_FRESTORE,
   FSAVE_D      = ROOT_FSAVE | IR_DOUBLE,
   FRESTORE_D   = ROOT_FRESTORE | IR_DOUBLE,
   BSAVE        = ROOT_BSAVE,
   BRESTORE     = ROOT_BRESTORE,
   PSAVE        = ROOT_PSAVE,
   PRESTORE     = ROOT_PRESTORE,
   SAVEG        = ROOT_SAVEG,
   RESTOREG     = ROOT_RESTOREG,
   FSAVEG_S     = ROOT_FSAVEG,
   FRESTOREG_S  = ROOT_FRESTOREG,
   FSAVEG_D     = ROOT_FSAVEG | IR_DOUBLE,
   FRESTOREG_D  = ROOT_FRESTOREG | IR_DOUBLE,
   BSAVEG       = ROOT_BSAVEG,
   BRESTOREG    = ROOT_BRESTOREG,
   PSAVEG       = ROOT_PSAVEG,
   PRESTOREG    = ROOT_PRESTOREG,

   // parhkc 01/2007
   VSAVE        = ROOT_VSAVE,
   VRESTORE     = ROOT_VRESTORE,
   VFSAVE_S     = ROOT_VFSAVE,
   VFRESTORE_S  = ROOT_VFRESTORE,
   VFSAVE_D     = ROOT_VFSAVE | IR_DOUBLE,
   VFRESTORE_D  = ROOT_VFRESTORE | IR_DOUBLE,
   VSAVEG       = ROOT_VSAVEG,
   VRESTOREG    = ROOT_VRESTOREG,
   VFSAVEG_S    = ROOT_VFSAVEG,
   VFRESTOREG_S = ROOT_VFRESTOREG,
   VFSAVEG_D    = ROOT_VFSAVEG | IR_DOUBLE,
   VFRESTOREG_D = ROOT_VFRESTOREG | IR_DOUBLE,

   // SLARSEN: Vector memops
   VL_B_C1_C1   = ROOT_VLD | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   VL_H_C1_C1   = ROOT_VLD | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   VL_W_C1_C1   = ROOT_VLD | IR_SRC_C1 | IR_DEST_C1,
   VFL_S_C1_C1  = ROOT_VFLD | IR_SRC_C1 | IR_DEST_C1,
   VFL_D_C1_C1  = ROOT_VFLD | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,

   VS_B_C1      = ROOT_VST | IR_BYTE | IR_DEST_C1,
   VS_H_C1      = ROOT_VST | IR_HALFWORD | IR_DEST_C1,
   VS_W_C1      = ROOT_VST | IR_DEST_C1,
   VFS_S_C1     = ROOT_VFST | IR_DEST_C1,
   VFS_D_C1     = ROOT_VFST | IR_DOUBLE | IR_DEST_C1,

   VLG_B_C1_C1  = ROOT_VLG | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   VLG_H_C1_C1  = ROOT_VLG | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   VLG_W_C1_C1  = ROOT_VLG | IR_SRC_C1 | IR_DEST_C1,
   VFLG_S_C1_C1 = ROOT_VFLG | IR_SRC_C1 | IR_DEST_C1,
   VFLG_D_C1_C1 = ROOT_VFLG | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,

   VSG_B_C1     = ROOT_VSG | IR_BYTE | IR_DEST_C1,
   VSG_H_C1     = ROOT_VSG | IR_HALFWORD | IR_DEST_C1,
   VSG_W_C1     = ROOT_VSG | IR_DEST_C1,
   VFSG_S_C1    = ROOT_VFSG | IR_HALFWORD | IR_DEST_C1,
   VFSG_D_C1    = ROOT_VFSG | IR_DEST_C1,

   VLE_B_C1_C1  = ROOT_VLDE | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   VLE_H_C1_C1  = ROOT_VLDE | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   VLE_W_C1_C1  = ROOT_VLDE | IR_SRC_C1 | IR_DEST_C1,
   VFLE_S_C1_C1 = ROOT_VFLDE | IR_SRC_C1 | IR_DEST_C1,
   VFLE_D_C1_C1 = ROOT_VFLDE | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,

   VSE_B_C1     = ROOT_VSTE | IR_BYTE | IR_DEST_C1,
   VSE_H_C1     = ROOT_VSTE | IR_HALFWORD | IR_DEST_C1,
   VSE_W_C1     = ROOT_VSTE | IR_DEST_C1,
   VFSE_S_C1    = ROOT_VFSTE | IR_DEST_C1,
   VFSE_D_C1    = ROOT_VFSTE | IR_DOUBLE | IR_DEST_C1,

   VLGE_B_C1_C1  = ROOT_VLGE | IR_BYTE | IR_SRC_C1 | IR_DEST_C1,
   VLGE_H_C1_C1  = ROOT_VLGE | IR_HALFWORD | IR_SRC_C1 | IR_DEST_C1,
   VLGE_W_C1_C1  = ROOT_VLGE | IR_SRC_C1 | IR_DEST_C1,
   VFLGE_S_C1_C1 = ROOT_VFLGE | IR_SRC_C1 | IR_DEST_C1,
   VFLGE_D_C1_C1 = ROOT_VFLGE | IR_DOUBLE | IR_SRC_C1 | IR_DEST_C1,

   VSGE_B_C1     = ROOT_VSGE | IR_BYTE | IR_DEST_C1,
   VSGE_H_C1     = ROOT_VSGE | IR_HALFWORD | IR_DEST_C1,
   VSGE_W_C1     = ROOT_VSGE | IR_DEST_C1,
   VFSGE_S_C1    = ROOT_VFSGE | IR_HALFWORD | IR_DEST_C1,
   VFSGE_D_C1    = ROOT_VFSGE | IR_DEST_C1,
};
   
#endif
