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
//      File:           opcode3.cpp
//      Authors:        Sadun Anik, Scott Mahlke, Richard Johnson
//      Created:        December 1994
//      Description:    Enums for the opcodes 
//
/////////////////////////////////////////////////////////////////////////////


#include "opcode.h"
#include "opcode_cmpp.h"
#include "opcode_load_store.h"
#include "hash_functions.h"
#include <string.h>

extern Hash_map<IR_BASE_OPCODE, eString> el_base_opcode_to_string_map ;

static void el_init_elcor_opcode_maps_memory_1()
{
   el_string_to_opcode_map.bind("L_B_V1_V1", (Opcode) L_B_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_V1_V1, "L_B_V1_V1") ;
   el_string_to_opcode_map.bind("L_B_V1_C1", (Opcode) L_B_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_V1_C1, "L_B_V1_C1") ;
   el_string_to_opcode_map.bind("L_B_V1_C2", (Opcode) L_B_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_B_V1_C2, "L_B_V1_C2") ;
   el_string_to_opcode_map.bind("L_B_V1_C3", (Opcode) L_B_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_B_V1_C3, "L_B_V1_C3") ;
   el_string_to_opcode_map.bind("L_B_C1_V1", (Opcode) L_B_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C1_V1, "L_B_C1_V1") ;
   el_string_to_opcode_map.bind("L_B_C1_C1", (Opcode) L_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C1_C1, "L_B_C1_C1") ;
   el_string_to_opcode_map.bind("L_B_C1_C2", (Opcode) L_B_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C1_C2, "L_B_C1_C2") ;
   el_string_to_opcode_map.bind("L_B_C1_C3", (Opcode) L_B_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C1_C3, "L_B_C1_C3") ;
   el_string_to_opcode_map.bind("L_B_C2_V1", (Opcode) L_B_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C2_V1, "L_B_C2_V1") ;
   el_string_to_opcode_map.bind("L_B_C2_C1", (Opcode) L_B_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C2_C1, "L_B_C2_C1") ;
   el_string_to_opcode_map.bind("L_B_C2_C2", (Opcode) L_B_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C2_C2, "L_B_C2_C2") ;
   el_string_to_opcode_map.bind("L_B_C2_C3", (Opcode) L_B_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C2_C3, "L_B_C2_C3") ;
   el_string_to_opcode_map.bind("L_B_C3_V1", (Opcode) L_B_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C3_V1, "L_B_C3_V1") ;
   el_string_to_opcode_map.bind("L_B_C3_C1", (Opcode) L_B_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C3_C1, "L_B_C3_C1") ;
   el_string_to_opcode_map.bind("L_B_C3_C2", (Opcode) L_B_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C3_C2, "L_B_C3_C2") ;
   el_string_to_opcode_map.bind("L_B_C3_C3", (Opcode) L_B_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_B_C3_C3, "L_B_C3_C3") ;

   el_string_to_opcode_map.bind("L_H_V1_V1", (Opcode) L_H_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_V1_V1, "L_H_V1_V1") ;
   el_string_to_opcode_map.bind("L_H_V1_C1", (Opcode) L_H_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_V1_C1, "L_H_V1_C1") ;
   el_string_to_opcode_map.bind("L_H_V1_C2", (Opcode) L_H_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_H_V1_C2, "L_H_V1_C2") ;
   el_string_to_opcode_map.bind("L_H_V1_C3", (Opcode) L_H_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_H_V1_C3, "L_H_V1_C3") ;
   el_string_to_opcode_map.bind("L_H_C1_V1", (Opcode) L_H_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C1_V1, "L_H_C1_V1") ;
   el_string_to_opcode_map.bind("L_H_C1_C1", (Opcode) L_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C1_C1, "L_H_C1_C1") ;
   el_string_to_opcode_map.bind("L_H_C1_C2", (Opcode) L_H_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C1_C2, "L_H_C1_C2") ;
   el_string_to_opcode_map.bind("L_H_C1_C3", (Opcode) L_H_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C1_C3, "L_H_C1_C3") ;
   el_string_to_opcode_map.bind("L_H_C2_V1", (Opcode) L_H_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C2_V1, "L_H_C2_V1") ;
   el_string_to_opcode_map.bind("L_H_C2_C1", (Opcode) L_H_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C2_C1, "L_H_C2_C1") ;
   el_string_to_opcode_map.bind("L_H_C2_C2", (Opcode) L_H_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C2_C2, "L_H_C2_C2") ;
   el_string_to_opcode_map.bind("L_H_C2_C3", (Opcode) L_H_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C2_C3, "L_H_C2_C3") ;
   el_string_to_opcode_map.bind("L_H_C3_V1", (Opcode) L_H_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C3_V1, "L_H_C3_V1") ;
   el_string_to_opcode_map.bind("L_H_C3_C1", (Opcode) L_H_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C3_C1, "L_H_C3_C1") ;
   el_string_to_opcode_map.bind("L_H_C3_C2", (Opcode) L_H_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C3_C2, "L_H_C3_C2") ;
   el_string_to_opcode_map.bind("L_H_C3_C3", (Opcode) L_H_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_H_C3_C3, "L_H_C3_C3") ;

   el_string_to_opcode_map.bind("L_W_V1_V1", (Opcode) L_W_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_V1_V1, "L_W_V1_V1") ;
   el_string_to_opcode_map.bind("L_W_V1_C1", (Opcode) L_W_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_V1_C1, "L_W_V1_C1") ;
   el_string_to_opcode_map.bind("L_W_V1_C2", (Opcode) L_W_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_W_V1_C2, "L_W_V1_C2") ;
   el_string_to_opcode_map.bind("L_W_V1_C3", (Opcode) L_W_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_W_V1_C3, "L_W_V1_C3") ;
   el_string_to_opcode_map.bind("L_W_C1_V1", (Opcode) L_W_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C1_V1, "L_W_C1_V1") ;
   el_string_to_opcode_map.bind("L_W_C1_C1", (Opcode) L_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C1_C1, "L_W_C1_C1") ;
   el_string_to_opcode_map.bind("L_W_C1_C2", (Opcode) L_W_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C1_C2, "L_W_C1_C2") ;
   el_string_to_opcode_map.bind("L_W_C1_C3", (Opcode) L_W_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C1_C3, "L_W_C1_C3") ;
   el_string_to_opcode_map.bind("L_W_C2_V1", (Opcode) L_W_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C2_V1, "L_W_C2_V1") ;
   el_string_to_opcode_map.bind("L_W_C2_C1", (Opcode) L_W_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C2_C1, "L_W_C2_C1") ;
   el_string_to_opcode_map.bind("L_W_C2_C2", (Opcode) L_W_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C2_C2, "L_W_C2_C2") ;
   el_string_to_opcode_map.bind("L_W_C2_C3", (Opcode) L_W_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C2_C3, "L_W_C2_C3") ;
   el_string_to_opcode_map.bind("L_W_C3_V1", (Opcode) L_W_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C3_V1, "L_W_C3_V1") ;
   el_string_to_opcode_map.bind("L_W_C3_C1", (Opcode) L_W_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C3_C1, "L_W_C3_C1") ;
   el_string_to_opcode_map.bind("L_W_C3_C2", (Opcode) L_W_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C3_C2, "L_W_C3_C2") ;
   el_string_to_opcode_map.bind("L_W_C3_C3", (Opcode) L_W_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_W_C3_C3, "L_W_C3_C3") ;

   el_string_to_opcode_map.bind("L_Q_V1_V1", (Opcode) L_Q_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_V1_V1, "L_Q_V1_V1") ;
   el_string_to_opcode_map.bind("L_Q_V1_C1", (Opcode) L_Q_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_V1_C1, "L_Q_V1_C1") ;
   el_string_to_opcode_map.bind("L_Q_V1_C2", (Opcode) L_Q_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_V1_C2, "L_Q_V1_C2") ;
   el_string_to_opcode_map.bind("L_Q_V1_C3", (Opcode) L_Q_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_V1_C3, "L_Q_V1_C3") ;
   el_string_to_opcode_map.bind("L_Q_C1_V1", (Opcode) L_Q_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C1_V1, "L_Q_C1_V1") ;
   el_string_to_opcode_map.bind("L_Q_C1_C1", (Opcode) L_Q_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C1_C1, "L_Q_C1_C1") ;
   el_string_to_opcode_map.bind("L_Q_C1_C2", (Opcode) L_Q_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C1_C2, "L_Q_C1_C2") ;
   el_string_to_opcode_map.bind("L_Q_C1_C3", (Opcode) L_Q_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C1_C3, "L_Q_C1_C3") ;
   el_string_to_opcode_map.bind("L_Q_C2_V1", (Opcode) L_Q_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C2_V1, "L_Q_C2_V1") ;
   el_string_to_opcode_map.bind("L_Q_C2_C1", (Opcode) L_Q_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C2_C1, "L_Q_C2_C1") ;
   el_string_to_opcode_map.bind("L_Q_C2_C2", (Opcode) L_Q_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C2_C2, "L_Q_C2_C2") ;
   el_string_to_opcode_map.bind("L_Q_C2_C3", (Opcode) L_Q_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C2_C3, "L_Q_C2_C3") ;
   el_string_to_opcode_map.bind("L_Q_C3_V1", (Opcode) L_Q_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C3_V1, "L_Q_C3_V1") ;
   el_string_to_opcode_map.bind("L_Q_C3_C1", (Opcode) L_Q_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C3_C1, "L_Q_C3_C1") ;
   el_string_to_opcode_map.bind("L_Q_C3_C2", (Opcode) L_Q_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C3_C2, "L_Q_C3_C2") ;
   el_string_to_opcode_map.bind("L_Q_C3_C3", (Opcode) L_Q_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) L_Q_C3_C3, "L_Q_C3_C3") ;
}

static void el_init_elcor_opcode_maps_memory_2()
{
   el_string_to_opcode_map.bind("LI_B_V1_V1", (Opcode) LI_B_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_V1_V1, "LI_B_V1_V1") ;
   el_string_to_opcode_map.bind("LI_B_V1_C1", (Opcode) LI_B_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_V1_C1, "LI_B_V1_C1") ;
   el_string_to_opcode_map.bind("LI_B_V1_C2", (Opcode) LI_B_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_V1_C2, "LI_B_V1_C2") ;
   el_string_to_opcode_map.bind("LI_B_V1_C3", (Opcode) LI_B_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_V1_C3, "LI_B_V1_C3") ;
   el_string_to_opcode_map.bind("LI_B_C1_V1", (Opcode) LI_B_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C1_V1, "LI_B_C1_V1") ;
   el_string_to_opcode_map.bind("LI_B_C1_C1", (Opcode) LI_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C1_C1, "LI_B_C1_C1") ;
   el_string_to_opcode_map.bind("LI_B_C1_C2", (Opcode) LI_B_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C1_C2, "LI_B_C1_C2") ;
   el_string_to_opcode_map.bind("LI_B_C1_C3", (Opcode) LI_B_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C1_C3, "LI_B_C1_C3") ;
   el_string_to_opcode_map.bind("LI_B_C2_V1", (Opcode) LI_B_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C2_V1, "LI_B_C2_V1") ;
   el_string_to_opcode_map.bind("LI_B_C2_C1", (Opcode) LI_B_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C2_C1, "LI_B_C2_C1") ;
   el_string_to_opcode_map.bind("LI_B_C2_C2", (Opcode) LI_B_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C2_C2, "LI_B_C2_C2") ;
   el_string_to_opcode_map.bind("LI_B_C2_C3", (Opcode) LI_B_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C2_C3, "LI_B_C2_C3") ;
   el_string_to_opcode_map.bind("LI_B_C3_V1", (Opcode) LI_B_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C3_V1, "LI_B_C3_V1") ;
   el_string_to_opcode_map.bind("LI_B_C3_C1", (Opcode) LI_B_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C3_C1, "LI_B_C3_C1") ;
   el_string_to_opcode_map.bind("LI_B_C3_C2", (Opcode) LI_B_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C3_C2, "LI_B_C3_C2") ;
   el_string_to_opcode_map.bind("LI_B_C3_C3", (Opcode) LI_B_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_B_C3_C3, "LI_B_C3_C3") ;

   el_string_to_opcode_map.bind("LI_H_V1_V1", (Opcode) LI_H_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_V1_V1, "LI_H_V1_V1") ;
   el_string_to_opcode_map.bind("LI_H_V1_C1", (Opcode) LI_H_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_V1_C1, "LI_H_V1_C1") ;
   el_string_to_opcode_map.bind("LI_H_V1_C2", (Opcode) LI_H_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_V1_C2, "LI_H_V1_C2") ;
   el_string_to_opcode_map.bind("LI_H_V1_C3", (Opcode) LI_H_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_V1_C3, "LI_H_V1_C3") ;
   el_string_to_opcode_map.bind("LI_H_C1_V1", (Opcode) LI_H_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C1_V1, "LI_H_C1_V1") ;
   el_string_to_opcode_map.bind("LI_H_C1_C1", (Opcode) LI_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C1_C1, "LI_H_C1_C1") ;
   el_string_to_opcode_map.bind("LI_H_C1_C2", (Opcode) LI_H_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C1_C2, "LI_H_C1_C2") ;
   el_string_to_opcode_map.bind("LI_H_C1_C3", (Opcode) LI_H_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C1_C3, "LI_H_C1_C3") ;
   el_string_to_opcode_map.bind("LI_H_C2_V1", (Opcode) LI_H_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C2_V1, "LI_H_C2_V1") ;
   el_string_to_opcode_map.bind("LI_H_C2_C1", (Opcode) LI_H_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C2_C1, "LI_H_C2_C1") ;
   el_string_to_opcode_map.bind("LI_H_C2_C2", (Opcode) LI_H_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C2_C2, "LI_H_C2_C2") ;
   el_string_to_opcode_map.bind("LI_H_C2_C3", (Opcode) LI_H_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C2_C3, "LI_H_C2_C3") ;
   el_string_to_opcode_map.bind("LI_H_C3_V1", (Opcode) LI_H_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C3_V1, "LI_H_C3_V1") ;
   el_string_to_opcode_map.bind("LI_H_C3_C1", (Opcode) LI_H_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C3_C1, "LI_H_C3_C1") ;
   el_string_to_opcode_map.bind("LI_H_C3_C2", (Opcode) LI_H_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C3_C2, "LI_H_C3_C2") ;
   el_string_to_opcode_map.bind("LI_H_C3_C3", (Opcode) LI_H_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_H_C3_C3, "LI_H_C3_C3") ;

   el_string_to_opcode_map.bind("LI_W_V1_V1", (Opcode) LI_W_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_V1_V1, "LI_W_V1_V1") ;
   el_string_to_opcode_map.bind("LI_W_V1_C1", (Opcode) LI_W_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_V1_C1, "LI_W_V1_C1") ;
   el_string_to_opcode_map.bind("LI_W_V1_C2", (Opcode) LI_W_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_V1_C2, "LI_W_V1_C2") ;
   el_string_to_opcode_map.bind("LI_W_V1_C3", (Opcode) LI_W_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_V1_C3, "LI_W_V1_C3") ;
   el_string_to_opcode_map.bind("LI_W_C1_V1", (Opcode) LI_W_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C1_V1, "LI_W_C1_V1") ;
   el_string_to_opcode_map.bind("LI_W_C1_C1", (Opcode) LI_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C1_C1, "LI_W_C1_C1") ;
   el_string_to_opcode_map.bind("LI_W_C1_C2", (Opcode) LI_W_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C1_C2, "LI_W_C1_C2") ;
   el_string_to_opcode_map.bind("LI_W_C1_C3", (Opcode) LI_W_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C1_C3, "LI_W_C1_C3") ;
   el_string_to_opcode_map.bind("LI_W_C2_V1", (Opcode) LI_W_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C2_V1, "LI_W_C2_V1") ;
   el_string_to_opcode_map.bind("LI_W_C2_C1", (Opcode) LI_W_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C2_C1, "LI_W_C2_C1") ;
   el_string_to_opcode_map.bind("LI_W_C2_C2", (Opcode) LI_W_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C2_C2, "LI_W_C2_C2") ;
   el_string_to_opcode_map.bind("LI_W_C2_C3", (Opcode) LI_W_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C2_C3, "LI_W_C2_C3") ;
   el_string_to_opcode_map.bind("LI_W_C3_V1", (Opcode) LI_W_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C3_V1, "LI_W_C3_V1") ;
   el_string_to_opcode_map.bind("LI_W_C3_C1", (Opcode) LI_W_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C3_C1, "LI_W_C3_C1") ;
   el_string_to_opcode_map.bind("LI_W_C3_C2", (Opcode) LI_W_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C3_C2, "LI_W_C3_C2") ;
   el_string_to_opcode_map.bind("LI_W_C3_C3", (Opcode) LI_W_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_W_C3_C3, "LI_W_C3_C3") ;

   el_string_to_opcode_map.bind("LI_Q_V1_V1", (Opcode) LI_Q_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_V1_V1, "LI_Q_V1_V1") ;
   el_string_to_opcode_map.bind("LI_Q_V1_C1", (Opcode) LI_Q_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_V1_C1, "LI_Q_V1_C1") ;
   el_string_to_opcode_map.bind("LI_Q_V1_C2", (Opcode) LI_Q_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_V1_C2, "LI_Q_V1_C2") ;
   el_string_to_opcode_map.bind("LI_Q_V1_C3", (Opcode) LI_Q_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_V1_C3, "LI_Q_V1_C3") ;
   el_string_to_opcode_map.bind("LI_Q_C1_V1", (Opcode) LI_Q_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C1_V1, "LI_Q_C1_V1") ;
   el_string_to_opcode_map.bind("LI_Q_C1_C1", (Opcode) LI_Q_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C1_C1, "LI_Q_C1_C1") ;
   el_string_to_opcode_map.bind("LI_Q_C1_C2", (Opcode) LI_Q_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C1_C2, "LI_Q_C1_C2") ;
   el_string_to_opcode_map.bind("LI_Q_C1_C3", (Opcode) LI_Q_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C1_C3, "LI_Q_C1_C3") ;
   el_string_to_opcode_map.bind("LI_Q_C2_V1", (Opcode) LI_Q_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C2_V1, "LI_Q_C2_V1") ;
   el_string_to_opcode_map.bind("LI_Q_C2_C1", (Opcode) LI_Q_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C2_C1, "LI_Q_C2_C1") ;
   el_string_to_opcode_map.bind("LI_Q_C2_C2", (Opcode) LI_Q_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C2_C2, "LI_Q_C2_C2") ;
   el_string_to_opcode_map.bind("LI_Q_C2_C3", (Opcode) LI_Q_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C2_C3, "LI_Q_C2_C3") ;
   el_string_to_opcode_map.bind("LI_Q_C3_V1", (Opcode) LI_Q_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C3_V1, "LI_Q_C3_V1") ;
   el_string_to_opcode_map.bind("LI_Q_C3_C1", (Opcode) LI_Q_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C3_C1, "LI_Q_C3_C1") ;
   el_string_to_opcode_map.bind("LI_Q_C3_C2", (Opcode) LI_Q_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C3_C2, "LI_Q_C3_C2") ;
   el_string_to_opcode_map.bind("LI_Q_C3_C3", (Opcode) LI_Q_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LI_Q_C3_C3, "LI_Q_C3_C3") ;
}

static void el_init_elcor_opcode_maps_memory_3()
{
   el_string_to_opcode_map.bind("FL_S_V1_V1", (Opcode) FL_S_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_V1_V1, "FL_S_V1_V1") ;
   el_string_to_opcode_map.bind("FL_S_V1_C1", (Opcode) FL_S_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_V1_C1, "FL_S_V1_C1") ;
   el_string_to_opcode_map.bind("FL_S_V1_C2", (Opcode) FL_S_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_V1_C2, "FL_S_V1_C2") ;
   el_string_to_opcode_map.bind("FL_S_V1_C3", (Opcode) FL_S_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_V1_C3, "FL_S_V1_C3") ;
   el_string_to_opcode_map.bind("FL_S_C1_V1", (Opcode) FL_S_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C1_V1, "FL_S_C1_V1") ;
   el_string_to_opcode_map.bind("FL_S_C1_C1", (Opcode) FL_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C1_C1, "FL_S_C1_C1") ;
   el_string_to_opcode_map.bind("FL_S_C1_C2", (Opcode) FL_S_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C1_C2, "FL_S_C1_C2") ;
   el_string_to_opcode_map.bind("FL_S_C1_C3", (Opcode) FL_S_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C1_C3, "FL_S_C1_C3") ;
   el_string_to_opcode_map.bind("FL_S_C2_V1", (Opcode) FL_S_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C2_V1, "FL_S_C2_V1") ;
   el_string_to_opcode_map.bind("FL_S_C2_C1", (Opcode) FL_S_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C2_C1, "FL_S_C2_C1") ;
   el_string_to_opcode_map.bind("FL_S_C2_C2", (Opcode) FL_S_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C2_C2, "FL_S_C2_C2") ;
   el_string_to_opcode_map.bind("FL_S_C2_C3", (Opcode) FL_S_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C2_C3, "FL_S_C2_C3") ;
   el_string_to_opcode_map.bind("FL_S_C3_V1", (Opcode) FL_S_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C3_V1, "FL_S_C3_V1") ;
   el_string_to_opcode_map.bind("FL_S_C3_C1", (Opcode) FL_S_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C3_C1, "FL_S_C3_C1") ;
   el_string_to_opcode_map.bind("FL_S_C3_C2", (Opcode) FL_S_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C3_C2, "FL_S_C3_C2") ;
   el_string_to_opcode_map.bind("FL_S_C3_C3", (Opcode) FL_S_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_S_C3_C3, "FL_S_C3_C3") ;

   el_string_to_opcode_map.bind("FL_D_V1_V1", (Opcode) FL_D_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_V1_V1, "FL_D_V1_V1") ;
   el_string_to_opcode_map.bind("FL_D_V1_C1", (Opcode) FL_D_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_V1_C1, "FL_D_V1_C1") ;
   el_string_to_opcode_map.bind("FL_D_V1_C2", (Opcode) FL_D_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_V1_C2, "FL_D_V1_C2") ;
   el_string_to_opcode_map.bind("FL_D_V1_C3", (Opcode) FL_D_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_V1_C3, "FL_D_V1_C3") ;
   el_string_to_opcode_map.bind("FL_D_C1_V1", (Opcode) FL_D_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C1_V1, "FL_D_C1_V1") ;
   el_string_to_opcode_map.bind("FL_D_C1_C1", (Opcode) FL_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C1_C1, "FL_D_C1_C1") ;
   el_string_to_opcode_map.bind("FL_D_C1_C2", (Opcode) FL_D_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C1_C2, "FL_D_C1_C2") ;
   el_string_to_opcode_map.bind("FL_D_C1_C3", (Opcode) FL_D_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C1_C3, "FL_D_C1_C3") ;
   el_string_to_opcode_map.bind("FL_D_C2_V1", (Opcode) FL_D_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C2_V1, "FL_D_C2_V1") ;
   el_string_to_opcode_map.bind("FL_D_C2_C1", (Opcode) FL_D_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C2_C1, "FL_D_C2_C1") ;
   el_string_to_opcode_map.bind("FL_D_C2_C2", (Opcode) FL_D_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C2_C2, "FL_D_C2_C2") ;
   el_string_to_opcode_map.bind("FL_D_C2_C3", (Opcode) FL_D_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C2_C3, "FL_D_C2_C3") ;
   el_string_to_opcode_map.bind("FL_D_C3_V1", (Opcode) FL_D_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C3_V1, "FL_D_C3_V1") ;
   el_string_to_opcode_map.bind("FL_D_C3_C1", (Opcode) FL_D_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C3_C1, "FL_D_C3_C1") ;
   el_string_to_opcode_map.bind("FL_D_C3_C2", (Opcode) FL_D_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C3_C2, "FL_D_C3_C2") ;
   el_string_to_opcode_map.bind("FL_D_C3_C3", (Opcode) FL_D_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FL_D_C3_C3, "FL_D_C3_C3") ;

   el_string_to_opcode_map.bind("FLI_S_V1_V1", (Opcode) FLI_S_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_V1_V1, "FLI_S_V1_V1") ;
   el_string_to_opcode_map.bind("FLI_S_V1_C1", (Opcode) FLI_S_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_V1_C1, "FLI_S_V1_C1") ;
   el_string_to_opcode_map.bind("FLI_S_V1_C2", (Opcode) FLI_S_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_V1_C2, "FLI_S_V1_C2") ;
   el_string_to_opcode_map.bind("FLI_S_V1_C3", (Opcode) FLI_S_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_V1_C3, "FLI_S_V1_C3") ;
   el_string_to_opcode_map.bind("FLI_S_C1_V1", (Opcode) FLI_S_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C1_V1, "FLI_S_C1_V1") ;
   el_string_to_opcode_map.bind("FLI_S_C1_C1", (Opcode) FLI_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C1_C1, "FLI_S_C1_C1") ;
   el_string_to_opcode_map.bind("FLI_S_C1_C2", (Opcode) FLI_S_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C1_C2, "FLI_S_C1_C2") ;
   el_string_to_opcode_map.bind("FLI_S_C1_C3", (Opcode) FLI_S_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C1_C3, "FLI_S_C1_C3") ;
   el_string_to_opcode_map.bind("FLI_S_C2_V1", (Opcode) FLI_S_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C2_V1, "FLI_S_C2_V1") ;
   el_string_to_opcode_map.bind("FLI_S_C2_C1", (Opcode) FLI_S_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C2_C1, "FLI_S_C2_C1") ;
   el_string_to_opcode_map.bind("FLI_S_C2_C2", (Opcode) FLI_S_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C2_C2, "FLI_S_C2_C2") ;
   el_string_to_opcode_map.bind("FLI_S_C2_C3", (Opcode) FLI_S_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C2_C3, "FLI_S_C2_C3") ;
   el_string_to_opcode_map.bind("FLI_S_C3_V1", (Opcode) FLI_S_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C3_V1, "FLI_S_C3_V1") ;
   el_string_to_opcode_map.bind("FLI_S_C3_C1", (Opcode) FLI_S_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C3_C1, "FLI_S_C3_C1") ;
   el_string_to_opcode_map.bind("FLI_S_C3_C2", (Opcode) FLI_S_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C3_C2, "FLI_S_C3_C2") ;
   el_string_to_opcode_map.bind("FLI_S_C3_C3", (Opcode) FLI_S_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_S_C3_C3, "FLI_S_C3_C3") ;

   el_string_to_opcode_map.bind("FLI_D_V1_V1", (Opcode) FLI_D_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_V1_V1, "FLI_D_V1_V1") ;
   el_string_to_opcode_map.bind("FLI_D_V1_C1", (Opcode) FLI_D_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_V1_C1, "FLI_D_V1_C1") ;
   el_string_to_opcode_map.bind("FLI_D_V1_C2", (Opcode) FLI_D_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_V1_C2, "FLI_D_V1_C2") ;
   el_string_to_opcode_map.bind("FLI_D_V1_C3", (Opcode) FLI_D_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_V1_C3, "FLI_D_V1_C3") ;
   el_string_to_opcode_map.bind("FLI_D_C1_V1", (Opcode) FLI_D_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C1_V1, "FLI_D_C1_V1") ;
   el_string_to_opcode_map.bind("FLI_D_C1_C1", (Opcode) FLI_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C1_C1, "FLI_D_C1_C1") ;
   el_string_to_opcode_map.bind("FLI_D_C1_C2", (Opcode) FLI_D_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C1_C2, "FLI_D_C1_C2") ;
   el_string_to_opcode_map.bind("FLI_D_C1_C3", (Opcode) FLI_D_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C1_C3, "FLI_D_C1_C3") ;
   el_string_to_opcode_map.bind("FLI_D_C2_V1", (Opcode) FLI_D_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C2_V1, "FLI_D_C2_V1") ;
   el_string_to_opcode_map.bind("FLI_D_C2_C1", (Opcode) FLI_D_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C2_C1, "FLI_D_C2_C1") ;
   el_string_to_opcode_map.bind("FLI_D_C2_C2", (Opcode) FLI_D_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C2_C2, "FLI_D_C2_C2") ;
   el_string_to_opcode_map.bind("FLI_D_C2_C3", (Opcode) FLI_D_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C2_C3, "FLI_D_C2_C3") ;
   el_string_to_opcode_map.bind("FLI_D_C3_V1", (Opcode) FLI_D_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C3_V1, "FLI_D_C3_V1") ;
   el_string_to_opcode_map.bind("FLI_D_C3_C1", (Opcode) FLI_D_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C3_C1, "FLI_D_C3_C1") ;
   el_string_to_opcode_map.bind("FLI_D_C3_C2", (Opcode) FLI_D_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C3_C2, "FLI_D_C3_C2") ;
   el_string_to_opcode_map.bind("FLI_D_C3_C3", (Opcode) FLI_D_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLI_D_C3_C3, "FLI_D_C3_C3") ;
}

static void el_init_elcor_opcode_maps_memory_4()
{
   el_string_to_opcode_map.bind("S_B_V1", (Opcode) S_B_V1) ;
   el_opcode_to_string_map.bind((Opcode) S_B_V1, "S_B_V1") ;
   el_string_to_opcode_map.bind("S_B_C1", (Opcode) S_B_C1) ;
   el_opcode_to_string_map.bind((Opcode) S_B_C1, "S_B_C1") ;
   el_string_to_opcode_map.bind("S_B_C2", (Opcode) S_B_C2) ;
   el_opcode_to_string_map.bind((Opcode) S_B_C2, "S_B_C2") ;
   el_string_to_opcode_map.bind("S_B_C3", (Opcode) S_B_C3) ;
   el_opcode_to_string_map.bind((Opcode) S_B_C3, "S_B_C3") ;

   el_string_to_opcode_map.bind("S_H_V1", (Opcode) S_H_V1) ;
   el_opcode_to_string_map.bind((Opcode) S_H_V1, "S_H_V1") ;
   el_string_to_opcode_map.bind("S_H_C1", (Opcode) S_H_C1) ;
   el_opcode_to_string_map.bind((Opcode) S_H_C1, "S_H_C1") ;
   el_string_to_opcode_map.bind("S_H_C2", (Opcode) S_H_C2) ;
   el_opcode_to_string_map.bind((Opcode) S_H_C2, "S_H_C2") ;
   el_string_to_opcode_map.bind("S_H_C3", (Opcode) S_H_C3) ;
   el_opcode_to_string_map.bind((Opcode) S_H_C3, "S_H_C3") ;

   el_string_to_opcode_map.bind("S_W_V1", (Opcode) S_W_V1) ;
   el_opcode_to_string_map.bind((Opcode) S_W_V1, "S_W_V1") ;
   el_string_to_opcode_map.bind("S_W_C1", (Opcode) S_W_C1) ;
   el_opcode_to_string_map.bind((Opcode) S_W_C1, "S_W_C1") ;
   el_string_to_opcode_map.bind("S_W_C2", (Opcode) S_W_C2) ;
   el_opcode_to_string_map.bind((Opcode) S_W_C2, "S_W_C2") ;
   el_string_to_opcode_map.bind("S_W_C3", (Opcode) S_W_C3) ;
   el_opcode_to_string_map.bind((Opcode) S_W_C3, "S_W_C3") ;

   el_string_to_opcode_map.bind("S_Q_V1", (Opcode) S_Q_V1) ;
   el_opcode_to_string_map.bind((Opcode) S_Q_V1, "S_Q_V1") ;
   el_string_to_opcode_map.bind("S_Q_C1", (Opcode) S_Q_C1) ;
   el_opcode_to_string_map.bind((Opcode) S_Q_C1, "S_Q_C1") ;
   el_string_to_opcode_map.bind("S_Q_C2", (Opcode) S_Q_C2) ;
   el_opcode_to_string_map.bind((Opcode) S_Q_C2, "S_Q_C2") ;
   el_string_to_opcode_map.bind("S_Q_C3", (Opcode) S_Q_C3) ;
   el_opcode_to_string_map.bind((Opcode) S_Q_C3, "S_Q_C3") ;

   el_string_to_opcode_map.bind("SI_B_V1", (Opcode) SI_B_V1) ;
   el_opcode_to_string_map.bind((Opcode) SI_B_V1, "SI_B_V1") ;
   el_string_to_opcode_map.bind("SI_B_C1", (Opcode) SI_B_C1) ;
   el_opcode_to_string_map.bind((Opcode) SI_B_C1, "SI_B_C1") ;
   el_string_to_opcode_map.bind("SI_B_C2", (Opcode) SI_B_C2) ;
   el_opcode_to_string_map.bind((Opcode) SI_B_C2, "SI_B_C2") ;
   el_string_to_opcode_map.bind("SI_B_C3", (Opcode) SI_B_C3) ;
   el_opcode_to_string_map.bind((Opcode) SI_B_C3, "SI_B_C3") ;

   el_string_to_opcode_map.bind("SI_H_V1", (Opcode) SI_H_V1) ;
   el_opcode_to_string_map.bind((Opcode) SI_H_V1, "SI_H_V1") ;
   el_string_to_opcode_map.bind("SI_H_C1", (Opcode) SI_H_C1) ;
   el_opcode_to_string_map.bind((Opcode) SI_H_C1, "SI_H_C1") ;
   el_string_to_opcode_map.bind("SI_H_C2", (Opcode) SI_H_C2) ;
   el_opcode_to_string_map.bind((Opcode) SI_H_C2, "SI_H_C2") ;
   el_string_to_opcode_map.bind("SI_H_C3", (Opcode) SI_H_C3) ;
   el_opcode_to_string_map.bind((Opcode) SI_H_C3, "SI_H_C3") ;

   el_string_to_opcode_map.bind("SI_W_V1", (Opcode) SI_W_V1) ;
   el_opcode_to_string_map.bind((Opcode) SI_W_V1, "SI_W_V1") ;
   el_string_to_opcode_map.bind("SI_W_C1", (Opcode) SI_W_C1) ;
   el_opcode_to_string_map.bind((Opcode) SI_W_C1, "SI_W_C1") ;
   el_string_to_opcode_map.bind("SI_W_C2", (Opcode) SI_W_C2) ;
   el_opcode_to_string_map.bind((Opcode) SI_W_C2, "SI_W_C2") ;
   el_string_to_opcode_map.bind("SI_W_C3", (Opcode) SI_W_C3) ;
   el_opcode_to_string_map.bind((Opcode) SI_W_C3, "SI_W_C3") ;

   el_string_to_opcode_map.bind("SI_Q_V1", (Opcode) SI_Q_V1) ;
   el_opcode_to_string_map.bind((Opcode) SI_Q_V1, "SI_Q_V1") ;
   el_string_to_opcode_map.bind("SI_Q_C1", (Opcode) SI_Q_C1) ;
   el_opcode_to_string_map.bind((Opcode) SI_Q_C1, "SI_Q_C1") ;
   el_string_to_opcode_map.bind("SI_Q_C2", (Opcode) SI_Q_C2) ;
   el_opcode_to_string_map.bind((Opcode) SI_Q_C2, "SI_Q_C2") ;
   el_string_to_opcode_map.bind("SI_Q_C3", (Opcode) SI_Q_C3) ;
   el_opcode_to_string_map.bind((Opcode) SI_Q_C3, "SI_Q_C3") ;

   el_string_to_opcode_map.bind("FS_S_V1", (Opcode) FS_S_V1) ;
   el_opcode_to_string_map.bind((Opcode) FS_S_V1, "FS_S_V1") ;
   el_string_to_opcode_map.bind("FS_S_C1", (Opcode) FS_S_C1) ;
   el_opcode_to_string_map.bind((Opcode) FS_S_C1, "FS_S_C1") ;
   el_string_to_opcode_map.bind("FS_S_C2", (Opcode) FS_S_C2) ;
   el_opcode_to_string_map.bind((Opcode) FS_S_C2, "FS_S_C2") ;
   el_string_to_opcode_map.bind("FS_S_C3", (Opcode) FS_S_C3) ;
   el_opcode_to_string_map.bind((Opcode) FS_S_C3, "FS_S_C3") ;

   el_string_to_opcode_map.bind("FS_D_V1", (Opcode) FS_D_V1) ;
   el_opcode_to_string_map.bind((Opcode) FS_D_V1, "FS_D_V1") ;
   el_string_to_opcode_map.bind("FS_D_C1", (Opcode) FS_D_C1) ;
   el_opcode_to_string_map.bind((Opcode) FS_D_C1, "FS_D_C1") ;
   el_string_to_opcode_map.bind("FS_D_C2", (Opcode) FS_D_C2) ;
   el_opcode_to_string_map.bind((Opcode) FS_D_C2, "FS_D_C2") ;
   el_string_to_opcode_map.bind("FS_D_C3", (Opcode) FS_D_C3) ;
   el_opcode_to_string_map.bind((Opcode) FS_D_C3, "FS_D_C3") ;

   el_string_to_opcode_map.bind("FSI_S_V1", (Opcode) FSI_S_V1) ;
   el_opcode_to_string_map.bind((Opcode) FSI_S_V1, "FSI_S_V1") ;
   el_string_to_opcode_map.bind("FSI_S_C1", (Opcode) FSI_S_C1) ;
   el_opcode_to_string_map.bind((Opcode) FSI_S_C1, "FSI_S_C1") ;
   el_string_to_opcode_map.bind("FSI_S_C2", (Opcode) FSI_S_C2) ;
   el_opcode_to_string_map.bind((Opcode) FSI_S_C2, "FSI_S_C2") ;
   el_string_to_opcode_map.bind("FSI_S_C3", (Opcode) FSI_S_C3) ;
   el_opcode_to_string_map.bind((Opcode) FSI_S_C3, "FSI_S_C3") ;

   el_string_to_opcode_map.bind("FSI_D_V1", (Opcode) FSI_D_V1) ;
   el_opcode_to_string_map.bind((Opcode) FSI_D_V1, "FSI_D_V1") ;
   el_string_to_opcode_map.bind("FSI_D_C1", (Opcode) FSI_D_C1) ;
   el_opcode_to_string_map.bind((Opcode) FSI_D_C1, "FSI_D_C1") ;
   el_string_to_opcode_map.bind("FSI_D_C2", (Opcode) FSI_D_C2) ;
   el_opcode_to_string_map.bind((Opcode) FSI_D_C2, "FSI_D_C2") ;
   el_string_to_opcode_map.bind("FSI_D_C3", (Opcode) FSI_D_C3) ;
   el_opcode_to_string_map.bind((Opcode) FSI_D_C3, "FSI_D_C3") ;
}

static void el_init_elcor_opcode_maps_memory_5()
{
   el_string_to_opcode_map.bind("LDS_B_V1_V1", (Opcode) LDS_B_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_V1_V1, "LDS_B_V1_V1") ;
   el_string_to_opcode_map.bind("LDS_B_V1_C1", (Opcode) LDS_B_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_V1_C1, "LDS_B_V1_C1") ;
   el_string_to_opcode_map.bind("LDS_B_V1_C2", (Opcode) LDS_B_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_V1_C2, "LDS_B_V1_C2") ;
   el_string_to_opcode_map.bind("LDS_B_V1_C3", (Opcode) LDS_B_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_V1_C3, "LDS_B_V1_C3") ;
   el_string_to_opcode_map.bind("LDS_B_C1_V1", (Opcode) LDS_B_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C1_V1, "LDS_B_C1_V1") ;
   el_string_to_opcode_map.bind("LDS_B_C1_C1", (Opcode) LDS_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C1_C1, "LDS_B_C1_C1") ;
   el_string_to_opcode_map.bind("LDS_B_C1_C2", (Opcode) LDS_B_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C1_C2, "LDS_B_C1_C2") ;
   el_string_to_opcode_map.bind("LDS_B_C1_C3", (Opcode) LDS_B_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C1_C3, "LDS_B_C1_C3") ;
   el_string_to_opcode_map.bind("LDS_B_C2_V1", (Opcode) LDS_B_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C2_V1, "LDS_B_C2_V1") ;
   el_string_to_opcode_map.bind("LDS_B_C2_C1", (Opcode) LDS_B_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C2_C1, "LDS_B_C2_C1") ;
   el_string_to_opcode_map.bind("LDS_B_C2_C2", (Opcode) LDS_B_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C2_C2, "LDS_B_C2_C2") ;
   el_string_to_opcode_map.bind("LDS_B_C2_C3", (Opcode) LDS_B_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C2_C3, "LDS_B_C2_C3") ;
   el_string_to_opcode_map.bind("LDS_B_C3_V1", (Opcode) LDS_B_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C3_V1, "LDS_B_C3_V1") ;
   el_string_to_opcode_map.bind("LDS_B_C3_C1", (Opcode) LDS_B_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C3_C1, "LDS_B_C3_C1") ;
   el_string_to_opcode_map.bind("LDS_B_C3_C2", (Opcode) LDS_B_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C3_C2, "LDS_B_C3_C2") ;
   el_string_to_opcode_map.bind("LDS_B_C3_C3", (Opcode) LDS_B_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_B_C3_C3, "LDS_B_C3_C3") ;

   el_string_to_opcode_map.bind("LDS_H_V1_V1", (Opcode) LDS_H_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_V1_V1, "LDS_H_V1_V1") ;
   el_string_to_opcode_map.bind("LDS_H_V1_C1", (Opcode) LDS_H_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_V1_C1, "LDS_H_V1_C1") ;
   el_string_to_opcode_map.bind("LDS_H_V1_C2", (Opcode) LDS_H_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_V1_C2, "LDS_H_V1_C2") ;
   el_string_to_opcode_map.bind("LDS_H_V1_C3", (Opcode) LDS_H_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_V1_C3, "LDS_H_V1_C3") ;
   el_string_to_opcode_map.bind("LDS_H_C1_V1", (Opcode) LDS_H_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C1_V1, "LDS_H_C1_V1") ;
   el_string_to_opcode_map.bind("LDS_H_C1_C1", (Opcode) LDS_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C1_C1, "LDS_H_C1_C1") ;
   el_string_to_opcode_map.bind("LDS_H_C1_C2", (Opcode) LDS_H_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C1_C2, "LDS_H_C1_C2") ;
   el_string_to_opcode_map.bind("LDS_H_C1_C3", (Opcode) LDS_H_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C1_C3, "LDS_H_C1_C3") ;
   el_string_to_opcode_map.bind("LDS_H_C2_V1", (Opcode) LDS_H_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C2_V1, "LDS_H_C2_V1") ;
   el_string_to_opcode_map.bind("LDS_H_C2_C1", (Opcode) LDS_H_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C2_C1, "LDS_H_C2_C1") ;
   el_string_to_opcode_map.bind("LDS_H_C2_C2", (Opcode) LDS_H_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C2_C2, "LDS_H_C2_C2") ;
   el_string_to_opcode_map.bind("LDS_H_C2_C3", (Opcode) LDS_H_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C2_C3, "LDS_H_C2_C3") ;
   el_string_to_opcode_map.bind("LDS_H_C3_V1", (Opcode) LDS_H_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C3_V1, "LDS_H_C3_V1") ;
   el_string_to_opcode_map.bind("LDS_H_C3_C1", (Opcode) LDS_H_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C3_C1, "LDS_H_C3_C1") ;
   el_string_to_opcode_map.bind("LDS_H_C3_C2", (Opcode) LDS_H_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C3_C2, "LDS_H_C3_C2") ;
   el_string_to_opcode_map.bind("LDS_H_C3_C3", (Opcode) LDS_H_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_H_C3_C3, "LDS_H_C3_C3") ;

   el_string_to_opcode_map.bind("LDS_W_V1_V1", (Opcode) LDS_W_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_V1_V1, "LDS_W_V1_V1") ;
   el_string_to_opcode_map.bind("LDS_W_V1_C1", (Opcode) LDS_W_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_V1_C1, "LDS_W_V1_C1") ;
   el_string_to_opcode_map.bind("LDS_W_V1_C2", (Opcode) LDS_W_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_V1_C2, "LDS_W_V1_C2") ;
   el_string_to_opcode_map.bind("LDS_W_V1_C3", (Opcode) LDS_W_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_V1_C3, "LDS_W_V1_C3") ;
   el_string_to_opcode_map.bind("LDS_W_C1_V1", (Opcode) LDS_W_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C1_V1, "LDS_W_C1_V1") ;
   el_string_to_opcode_map.bind("LDS_W_C1_C1", (Opcode) LDS_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C1_C1, "LDS_W_C1_C1") ;
   el_string_to_opcode_map.bind("LDS_W_C1_C2", (Opcode) LDS_W_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C1_C2, "LDS_W_C1_C2") ;
   el_string_to_opcode_map.bind("LDS_W_C1_C3", (Opcode) LDS_W_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C1_C3, "LDS_W_C1_C3") ;
   el_string_to_opcode_map.bind("LDS_W_C2_V1", (Opcode) LDS_W_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C2_V1, "LDS_W_C2_V1") ;
   el_string_to_opcode_map.bind("LDS_W_C2_C1", (Opcode) LDS_W_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C2_C1, "LDS_W_C2_C1") ;
   el_string_to_opcode_map.bind("LDS_W_C2_C2", (Opcode) LDS_W_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C2_C2, "LDS_W_C2_C2") ;
   el_string_to_opcode_map.bind("LDS_W_C2_C3", (Opcode) LDS_W_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C2_C3, "LDS_W_C2_C3") ;
   el_string_to_opcode_map.bind("LDS_W_C3_V1", (Opcode) LDS_W_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C3_V1, "LDS_W_C3_V1") ;
   el_string_to_opcode_map.bind("LDS_W_C3_C1", (Opcode) LDS_W_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C3_C1, "LDS_W_C3_C1") ;
   el_string_to_opcode_map.bind("LDS_W_C3_C2", (Opcode) LDS_W_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C3_C2, "LDS_W_C3_C2") ;
   el_string_to_opcode_map.bind("LDS_W_C3_C3", (Opcode) LDS_W_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_W_C3_C3, "LDS_W_C3_C3") ;

   el_string_to_opcode_map.bind("LDS_Q_V1_V1", (Opcode) LDS_Q_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_V1_V1, "LDS_Q_V1_V1") ;
   el_string_to_opcode_map.bind("LDS_Q_V1_C1", (Opcode) LDS_Q_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_V1_C1, "LDS_Q_V1_C1") ;
   el_string_to_opcode_map.bind("LDS_Q_V1_C2", (Opcode) LDS_Q_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_V1_C2, "LDS_Q_V1_C2") ;
   el_string_to_opcode_map.bind("LDS_Q_V1_C3", (Opcode) LDS_Q_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_V1_C3, "LDS_Q_V1_C3") ;
   el_string_to_opcode_map.bind("LDS_Q_C1_V1", (Opcode) LDS_Q_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C1_V1, "LDS_Q_C1_V1") ;
   el_string_to_opcode_map.bind("LDS_Q_C1_C1", (Opcode) LDS_Q_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C1_C1, "LDS_Q_C1_C1") ;
   el_string_to_opcode_map.bind("LDS_Q_C1_C2", (Opcode) LDS_Q_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C1_C2, "LDS_Q_C1_C2") ;
   el_string_to_opcode_map.bind("LDS_Q_C1_C3", (Opcode) LDS_Q_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C1_C3, "LDS_Q_C1_C3") ;
   el_string_to_opcode_map.bind("LDS_Q_C2_V1", (Opcode) LDS_Q_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C2_V1, "LDS_Q_C2_V1") ;
   el_string_to_opcode_map.bind("LDS_Q_C2_C1", (Opcode) LDS_Q_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C2_C1, "LDS_Q_C2_C1") ;
   el_string_to_opcode_map.bind("LDS_Q_C2_C2", (Opcode) LDS_Q_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C2_C2, "LDS_Q_C2_C2") ;
   el_string_to_opcode_map.bind("LDS_Q_C2_C3", (Opcode) LDS_Q_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C2_C3, "LDS_Q_C2_C3") ;
   el_string_to_opcode_map.bind("LDS_Q_C3_V1", (Opcode) LDS_Q_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C3_V1, "LDS_Q_C3_V1") ;
   el_string_to_opcode_map.bind("LDS_Q_C3_C1", (Opcode) LDS_Q_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C3_C1, "LDS_Q_C3_C1") ;
   el_string_to_opcode_map.bind("LDS_Q_C3_C2", (Opcode) LDS_Q_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C3_C2, "LDS_Q_C3_C2") ;
   el_string_to_opcode_map.bind("LDS_Q_C3_C3", (Opcode) LDS_Q_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDS_Q_C3_C3, "LDS_Q_C3_C3") ;

   el_string_to_opcode_map.bind("LDSI_B_V1_V1", (Opcode) LDSI_B_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_V1_V1, "LDSI_B_V1_V1") ;
   el_string_to_opcode_map.bind("LDSI_B_V1_C1", (Opcode) LDSI_B_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_V1_C1, "LDSI_B_V1_C1") ;
   el_string_to_opcode_map.bind("LDSI_B_V1_C2", (Opcode) LDSI_B_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_V1_C2, "LDSI_B_V1_C2") ;
   el_string_to_opcode_map.bind("LDSI_B_V1_C3", (Opcode) LDSI_B_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_V1_C3, "LDSI_B_V1_C3") ;
   el_string_to_opcode_map.bind("LDSI_B_C1_V1", (Opcode) LDSI_B_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C1_V1, "LDSI_B_C1_V1") ;
   el_string_to_opcode_map.bind("LDSI_B_C1_C1", (Opcode) LDSI_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C1_C1, "LDSI_B_C1_C1") ;
   el_string_to_opcode_map.bind("LDSI_B_C1_C2", (Opcode) LDSI_B_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C1_C2, "LDSI_B_C1_C2") ;
   el_string_to_opcode_map.bind("LDSI_B_C1_C3", (Opcode) LDSI_B_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C1_C3, "LDSI_B_C1_C3") ;
   el_string_to_opcode_map.bind("DS_I_B_C2_V1", (Opcode) LDSI_B_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C2_V1, "LDSI_B_C2_V1") ;
   el_string_to_opcode_map.bind("LDSI_B_C2_C1", (Opcode) LDSI_B_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C2_C1, "LDSI_B_C2_C1") ;
   el_string_to_opcode_map.bind("LDSI_B_C2_C2", (Opcode) LDSI_B_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C2_C2, "LDSI_B_C2_C2") ;
   el_string_to_opcode_map.bind("LDSI_B_C2_C3", (Opcode) LDSI_B_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C2_C3, "LDSI_B_C2_C3") ;
   el_string_to_opcode_map.bind("LDSI_B_C3_V1", (Opcode) LDSI_B_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C3_V1, "LDSI_B_C3_V1") ;
   el_string_to_opcode_map.bind("LDSI_B_C3_C1", (Opcode) LDSI_B_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C3_C1, "LDSI_B_C3_C1") ;
   el_string_to_opcode_map.bind("LDSI_B_C3_C2", (Opcode) LDSI_B_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C3_C2, "LDSI_B_C3_C2") ;
   el_string_to_opcode_map.bind("LDSI_B_C3_C3", (Opcode) LDSI_B_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_B_C3_C3, "LDSI_B_C3_C3") ;

   el_string_to_opcode_map.bind("LDSI_H_V1_V1", (Opcode) LDSI_H_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_V1_V1, "LDSI_H_V1_V1") ;
   el_string_to_opcode_map.bind("LDSI_H_V1_C1", (Opcode) LDSI_H_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_V1_C1, "LDSI_H_V1_C1") ;
   el_string_to_opcode_map.bind("LDSI_H_V1_C2", (Opcode) LDSI_H_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_V1_C2, "LDSI_H_V1_C2") ;
   el_string_to_opcode_map.bind("LDSI_H_V1_C3", (Opcode) LDSI_H_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_V1_C3, "LDSI_H_V1_C3") ;
   el_string_to_opcode_map.bind("LDSI_H_C1_V1", (Opcode) LDSI_H_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C1_V1, "LDSI_H_C1_V1") ;
   el_string_to_opcode_map.bind("LDSI_H_C1_C1", (Opcode) LDSI_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C1_C1, "LDSI_H_C1_C1") ;
   el_string_to_opcode_map.bind("LDSI_H_C1_C2", (Opcode) LDSI_H_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C1_C2, "LDSI_H_C1_C2") ;
   el_string_to_opcode_map.bind("LDSI_H_C1_C3", (Opcode) LDSI_H_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C1_C3, "LDSI_H_C1_C3") ;
   el_string_to_opcode_map.bind("LDSI_H_C2_V1", (Opcode) LDSI_H_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C2_V1, "LDSI_H_C2_V1") ;
   el_string_to_opcode_map.bind("LDSI_H_C2_C1", (Opcode) LDSI_H_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C2_C1, "LDSI_H_C2_C1") ;
   el_string_to_opcode_map.bind("LDSI_H_C2_C2", (Opcode) LDSI_H_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C2_C2, "LDSI_H_C2_C2") ;
   el_string_to_opcode_map.bind("LDSI_H_C2_C3", (Opcode) LDSI_H_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C2_C3, "LDSI_H_C2_C3") ;
   el_string_to_opcode_map.bind("LDSI_H_C3_V1", (Opcode) LDSI_H_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C3_V1, "LDSI_H_C3_V1") ;
   el_string_to_opcode_map.bind("LDSI_H_C3_C1", (Opcode) LDSI_H_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C3_C1, "LDSI_H_C3_C1") ;
   el_string_to_opcode_map.bind("LDSI_H_C3_C2", (Opcode) LDSI_H_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C3_C2, "LDSI_H_C3_C2") ;
   el_string_to_opcode_map.bind("LDSI_H_C3_C3", (Opcode) LDSI_H_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_H_C3_C3, "LDSI_H_C3_C3") ;

   el_string_to_opcode_map.bind("LDSI_W_V1_V1", (Opcode) LDSI_W_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_V1_V1, "LDSI_W_V1_V1") ;
   el_string_to_opcode_map.bind("LDSI_W_V1_C1", (Opcode) LDSI_W_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_V1_C1, "LDSI_W_V1_C1") ;
   el_string_to_opcode_map.bind("LDSI_W_V1_C2", (Opcode) LDSI_W_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_V1_C2, "LDSI_W_V1_C2") ;
   el_string_to_opcode_map.bind("LDSI_W_V1_C3", (Opcode) LDSI_W_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_V1_C3, "LDSI_W_V1_C3") ;
   el_string_to_opcode_map.bind("LDSI_W_C1_V1", (Opcode) LDSI_W_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C1_V1, "LDSI_W_C1_V1") ;
   el_string_to_opcode_map.bind("LDSI_W_C1_C1", (Opcode) LDSI_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C1_C1, "LDSI_W_C1_C1") ;
   el_string_to_opcode_map.bind("LDSI_W_C1_C2", (Opcode) LDSI_W_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C1_C2, "LDSI_W_C1_C2") ;
   el_string_to_opcode_map.bind("LDSI_W_C1_C3", (Opcode) LDSI_W_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C1_C3, "LDSI_W_C1_C3") ;
   el_string_to_opcode_map.bind("LDSI_W_C2_V1", (Opcode) LDSI_W_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C2_V1, "LDSI_W_C2_V1") ;
   el_string_to_opcode_map.bind("LDSI_W_C2_C1", (Opcode) LDSI_W_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C2_C1, "LDSI_W_C2_C1") ;
   el_string_to_opcode_map.bind("LDSI_W_C2_C2", (Opcode) LDSI_W_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C2_C2, "LDSI_W_C2_C2") ;
   el_string_to_opcode_map.bind("LDSI_W_C2_C3", (Opcode) LDSI_W_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C2_C3, "LDSI_W_C2_C3") ;
   el_string_to_opcode_map.bind("LDSI_W_C3_V1", (Opcode) LDSI_W_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C3_V1, "LDSI_W_C3_V1") ;
   el_string_to_opcode_map.bind("LDSI_W_C3_C1", (Opcode) LDSI_W_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C3_C1, "LDSI_W_C3_C1") ;
   el_string_to_opcode_map.bind("LDSI_W_C3_C2", (Opcode) LDSI_W_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C3_C2, "LDSI_W_C3_C2") ;
   el_string_to_opcode_map.bind("LDSI_W_C3_C3", (Opcode) LDSI_W_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_W_C3_C3, "LDSI_W_C3_C3") ;

   el_string_to_opcode_map.bind("LDSI_Q_V1_V1", (Opcode) LDSI_Q_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_V1_V1, "LDSI_Q_V1_V1") ;
   el_string_to_opcode_map.bind("LDSI_Q_V1_C1", (Opcode) LDSI_Q_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_V1_C1, "LDSI_Q_V1_C1") ;
   el_string_to_opcode_map.bind("LDSI_Q_V1_C2", (Opcode) LDSI_Q_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_V1_C2, "LDSI_Q_V1_C2") ;
   el_string_to_opcode_map.bind("LDSI_Q_V1_C3", (Opcode) LDSI_Q_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_V1_C3, "LDSI_Q_V1_C3") ;
   el_string_to_opcode_map.bind("LDSI_Q_C1_V1", (Opcode) LDSI_Q_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C1_V1, "LDSI_Q_C1_V1") ;
   el_string_to_opcode_map.bind("LDSI_Q_C1_C1", (Opcode) LDSI_Q_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C1_C1, "LDSI_Q_C1_C1") ;
   el_string_to_opcode_map.bind("LDSI_Q_C1_C2", (Opcode) LDSI_Q_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C1_C2, "LDSI_Q_C1_C2") ;
   el_string_to_opcode_map.bind("LDSI_Q_C1_C3", (Opcode) LDSI_Q_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C1_C3, "LDSI_Q_C1_C3") ;
   el_string_to_opcode_map.bind("LDSI_Q_C2_V1", (Opcode) LDSI_Q_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C2_V1, "LDSI_Q_C2_V1") ;
   el_string_to_opcode_map.bind("LDSI_Q_C2_C1", (Opcode) LDSI_Q_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C2_C1, "LDSI_Q_C2_C1") ;
   el_string_to_opcode_map.bind("LDSI_Q_C2_C2", (Opcode) LDSI_Q_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C2_C2, "LDSI_Q_C2_C2") ;
   el_string_to_opcode_map.bind("LDSI_Q_C2_C3", (Opcode) LDSI_Q_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C2_C3, "LDSI_Q_C2_C3") ;
   el_string_to_opcode_map.bind("LDSI_Q_C3_V1", (Opcode) LDSI_Q_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C3_V1, "LDSI_Q_C3_V1") ;
   el_string_to_opcode_map.bind("LDSI_Q_C3_C1", (Opcode) LDSI_Q_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C3_C1, "LDSI_Q_C3_C1") ;
   el_string_to_opcode_map.bind("LDSI_Q_C3_C2", (Opcode) LDSI_Q_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C3_C2, "LDSI_Q_C3_C2") ;
   el_string_to_opcode_map.bind("LDSI_Q_C3_C3", (Opcode) LDSI_Q_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LDSI_Q_C3_C3, "LDSI_Q_C3_C3") ;
}

static void el_init_elcor_opcode_maps_memory_6()
{
   el_string_to_opcode_map.bind("FLDS_S_V1_V1", (Opcode) FLDS_S_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_V1_V1, "FLDS_S_V1_V1") ;
   el_string_to_opcode_map.bind("FLDS_S_V1_C1", (Opcode) FLDS_S_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_V1_C1, "FLDS_S_V1_C1") ;
   el_string_to_opcode_map.bind("FLDS_S_V1_C2", (Opcode) FLDS_S_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_V1_C2, "FLDS_S_V1_C2") ;
   el_string_to_opcode_map.bind("FLDS_S_V1_C3", (Opcode) FLDS_S_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_V1_C3, "FLDS_S_V1_C3") ;
   el_string_to_opcode_map.bind("FLDS_S_C1_V1", (Opcode) FLDS_S_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C1_V1, "FLDS_S_C1_V1") ;
   el_string_to_opcode_map.bind("FLDS_S_C1_C1", (Opcode) FLDS_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C1_C1, "FLDS_S_C1_C1") ;
   el_string_to_opcode_map.bind("FLDS_S_C1_C2", (Opcode) FLDS_S_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C1_C2, "FLDS_S_C1_C2") ;
   el_string_to_opcode_map.bind("FLDS_S_C1_C3", (Opcode) FLDS_S_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C1_C3, "FLDS_S_C1_C3") ;
   el_string_to_opcode_map.bind("FLDS_S_C2_V1", (Opcode) FLDS_S_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C2_V1, "FLDS_S_C2_V1") ;
   el_string_to_opcode_map.bind("FLDS_S_C2_C1", (Opcode) FLDS_S_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C2_C1, "FLDS_S_C2_C1") ;
   el_string_to_opcode_map.bind("FLDS_S_C2_C2", (Opcode) FLDS_S_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C2_C2, "FLDS_S_C2_C2") ;
   el_string_to_opcode_map.bind("FLDS_S_C2_C3", (Opcode) FLDS_S_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C2_C3, "FLDS_S_C2_C3") ;
   el_string_to_opcode_map.bind("FLDS_S_C3_V1", (Opcode) FLDS_S_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C3_V1, "FLDS_S_C3_V1") ;
   el_string_to_opcode_map.bind("FLDS_S_C3_C1", (Opcode) FLDS_S_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C3_C1, "FLDS_S_C3_C1") ;
   el_string_to_opcode_map.bind("FLDS_S_C3_C2", (Opcode) FLDS_S_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C3_C2, "FLDS_S_C3_C2") ;
   el_string_to_opcode_map.bind("FLDS_S_C3_C3", (Opcode) FLDS_S_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_S_C3_C3, "FLDS_S_C3_C3") ;

   el_string_to_opcode_map.bind("FLDS_D_V1_V1", (Opcode) FLDS_D_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_V1_V1, "FLDS_D_V1_V1") ;
   el_string_to_opcode_map.bind("FLDS_D_V1_C1", (Opcode) FLDS_D_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_V1_C1, "FLDS_D_V1_C1") ;
   el_string_to_opcode_map.bind("FLDS_D_V1_C2", (Opcode) FLDS_D_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_V1_C2, "FLDS_D_V1_C2") ;
   el_string_to_opcode_map.bind("FLDS_D_V1_C3", (Opcode) FLDS_D_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_V1_C3, "FLDS_D_V1_C3") ;
   el_string_to_opcode_map.bind("FLDS_D_C1_V1", (Opcode) FLDS_D_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C1_V1, "FLDS_D_C1_V1") ;
   el_string_to_opcode_map.bind("FLDS_D_C1_C1", (Opcode) FLDS_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C1_C1, "FLDS_D_C1_C1") ;
   el_string_to_opcode_map.bind("FLDS_D_C1_C2", (Opcode) FLDS_D_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C1_C2, "FLDS_D_C1_C2") ;
   el_string_to_opcode_map.bind("FLDS_D_C1_C3", (Opcode) FLDS_D_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C1_C3, "FLDS_D_C1_C3") ;
   el_string_to_opcode_map.bind("FLDS_D_C2_V1", (Opcode) FLDS_D_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C2_V1, "FLDS_D_C2_V1") ;
   el_string_to_opcode_map.bind("FLDS_D_C2_C1", (Opcode) FLDS_D_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C2_C1, "FLDS_D_C2_C1") ;
   el_string_to_opcode_map.bind("FLDS_D_C2_C2", (Opcode) FLDS_D_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C2_C2, "FLDS_D_C2_C2") ;
   el_string_to_opcode_map.bind("FLDS_D_C2_C3", (Opcode) FLDS_D_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C2_C3, "FLDS_D_C2_C3") ;
   el_string_to_opcode_map.bind("FLDS_D_C3_V1", (Opcode) FLDS_D_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C3_V1, "FLDS_D_C3_V1") ;
   el_string_to_opcode_map.bind("FLDS_D_C3_C1", (Opcode) FLDS_D_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C3_C1, "FLDS_D_C3_C1") ;
   el_string_to_opcode_map.bind("FLDS_D_C3_C2", (Opcode) FLDS_D_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C3_C2, "FLDS_D_C3_C2") ;
   el_string_to_opcode_map.bind("FLDS_D_C3_C3", (Opcode) FLDS_D_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDS_D_C3_C3, "FLDS_D_C3_C3") ;

   el_string_to_opcode_map.bind("FLDSI_S_V1_V1", (Opcode) FLDSI_S_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_V1_V1, "FLDSI_S_V1_V1") ;
   el_string_to_opcode_map.bind("FLDSI_S_V1_C1", (Opcode) FLDSI_S_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_V1_C1, "FLDSI_S_V1_C1") ;
   el_string_to_opcode_map.bind("FLDSI_S_V1_C2", (Opcode) FLDSI_S_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_V1_C2, "FLDSI_S_V1_C2") ;
   el_string_to_opcode_map.bind("FLDSI_S_V1_C3", (Opcode) FLDSI_S_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_V1_C3, "FLDSI_S_V1_C3") ;
   el_string_to_opcode_map.bind("FLDSI_S_C1_V1", (Opcode) FLDSI_S_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C1_V1, "FLDSI_S_C1_V1") ;
   el_string_to_opcode_map.bind("FLDSI_S_C1_C1", (Opcode) FLDSI_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C1_C1, "FLDSI_S_C1_C1") ;
   el_string_to_opcode_map.bind("FLDSI_S_C1_C2", (Opcode) FLDSI_S_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C1_C2, "FLDSI_S_C1_C2") ;
   el_string_to_opcode_map.bind("FLDSI_S_C1_C3", (Opcode) FLDSI_S_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C1_C3, "FLDSI_S_C1_C3") ;
   el_string_to_opcode_map.bind("FLDSI_S_C2_V1", (Opcode) FLDSI_S_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C2_V1, "FLDSI_S_C2_V1") ;
   el_string_to_opcode_map.bind("FLDSI_S_C2_C1", (Opcode) FLDSI_S_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C2_C1, "FLDSI_S_C2_C1") ;
   el_string_to_opcode_map.bind("FLDSI_S_C2_C2", (Opcode) FLDSI_S_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C2_C2, "FLDSI_S_C2_C2") ;
   el_string_to_opcode_map.bind("FLDSI_S_C2_C3", (Opcode) FLDSI_S_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C2_C3, "FLDSI_S_C2_C3") ;
   el_string_to_opcode_map.bind("FLDSI_S_C3_V1", (Opcode) FLDSI_S_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C3_V1, "FLDSI_S_C3_V1") ;
   el_string_to_opcode_map.bind("FLDSI_S_C3_C1", (Opcode) FLDSI_S_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C3_C1, "FLDSI_S_C3_C1") ;
   el_string_to_opcode_map.bind("FLDSI_S_C3_C2", (Opcode) FLDSI_S_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C3_C2, "FLDSI_S_C3_C2") ;
   el_string_to_opcode_map.bind("FLDSI_S_C3_C3", (Opcode) FLDSI_S_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_S_C3_C3, "FLDSI_S_C3_C3") ;

   el_string_to_opcode_map.bind("FLDSI_D_V1_V1", (Opcode) FLDSI_D_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_V1_V1, "FLDSI_D_V1_V1") ;
   el_string_to_opcode_map.bind("FLDSI_D_V1_C1", (Opcode) FLDSI_D_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_V1_C1, "FLDSI_D_V1_C1") ;
   el_string_to_opcode_map.bind("FLDSI_D_V1_C2", (Opcode) FLDSI_D_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_V1_C2, "FLDSI_D_V1_C2") ;
   el_string_to_opcode_map.bind("FLDSI_D_V1_C3", (Opcode) FLDSI_D_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_V1_C3, "FLDSI_D_V1_C3") ;
   el_string_to_opcode_map.bind("FLDSI_D_C1_V1", (Opcode) FLDSI_D_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C1_V1, "FLDSI_D_C1_V1") ;
   el_string_to_opcode_map.bind("FLDSI_D_C1_C1", (Opcode) FLDSI_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C1_C1, "FLDSI_D_C1_C1") ;
   el_string_to_opcode_map.bind("FLDSI_D_C1_C2", (Opcode) FLDSI_D_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C1_C2, "FLDSI_D_C1_C2") ;
   el_string_to_opcode_map.bind("FLDSI_D_C1_C3", (Opcode) FLDSI_D_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C1_C3, "FLDSI_D_C1_C3") ;
   el_string_to_opcode_map.bind("FLDSI_D_C2_V1", (Opcode) FLDSI_D_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C2_V1, "FLDSI_D_C2_V1") ;
   el_string_to_opcode_map.bind("FLDSI_D_C2_C1", (Opcode) FLDSI_D_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C2_C1, "FLDSI_D_C2_C1") ;
   el_string_to_opcode_map.bind("FLDSI_D_C2_C2", (Opcode) FLDSI_D_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C2_C2, "FLDSI_D_C2_C2") ;
   el_string_to_opcode_map.bind("FLDSI_D_C2_C3", (Opcode) FLDSI_D_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C2_C3, "FLDSI_D_C2_C3") ;
   el_string_to_opcode_map.bind("FLDSI_D_C3_V1", (Opcode) FLDSI_D_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C3_V1, "FLDSI_D_C3_V1") ;
   el_string_to_opcode_map.bind("FLDSI_D_C3_C1", (Opcode) FLDSI_D_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C3_C1, "FLDSI_D_C3_C1") ;
   el_string_to_opcode_map.bind("FLDSI_D_C3_C2", (Opcode) FLDSI_D_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C3_C2, "FLDSI_D_C3_C2") ;
   el_string_to_opcode_map.bind("FLDSI_D_C3_C3", (Opcode) FLDSI_D_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLDSI_D_C3_C3, "FLDSI_D_C3_C3") ;

   el_string_to_opcode_map.bind("LDV_B", LDV_B) ;
   el_opcode_to_string_map.bind(LDV_B, "LDV_B") ;
   el_string_to_opcode_map.bind("LDV_H", LDV_H) ;
   el_opcode_to_string_map.bind(LDV_H, "LDV_H") ;
   el_string_to_opcode_map.bind("LDV_W", LDV_W) ;
   el_opcode_to_string_map.bind(LDV_W, "LDV_W") ;
   el_string_to_opcode_map.bind("LDV_Q", LDV_Q) ;
   el_opcode_to_string_map.bind(LDV_Q, "LDV_Q") ;

   el_string_to_opcode_map.bind("FLDV_S", FLDV_S) ;
   el_opcode_to_string_map.bind(FLDV_S, "FLDV_S") ;
   el_string_to_opcode_map.bind("FLDV_D", FLDV_D) ;
   el_opcode_to_string_map.bind(FLDV_D, "FLDV_D") ;
}

static void el_init_elcor_opcode_maps_memory_7()
{
   el_string_to_opcode_map.bind("LX_B_V1_V1", (Opcode) LX_B_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_V1_V1, "LX_B_V1_V1") ;
   el_string_to_opcode_map.bind("LX_B_V1_C1", (Opcode) LX_B_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_V1_C1, "LX_B_V1_C1") ;
   el_string_to_opcode_map.bind("LX_B_V1_C2", (Opcode) LX_B_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_V1_C2, "LX_B_V1_C2") ;
   el_string_to_opcode_map.bind("LX_B_V1_C3", (Opcode) LX_B_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_V1_C3, "LX_B_V1_C3") ;
   el_string_to_opcode_map.bind("LX_B_C1_V1", (Opcode) LX_B_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C1_V1, "LX_B_C1_V1") ;
   el_string_to_opcode_map.bind("LX_B_C1_C1", (Opcode) LX_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C1_C1, "LX_B_C1_C1") ;
   el_string_to_opcode_map.bind("LX_B_C1_C2", (Opcode) LX_B_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C1_C2, "LX_B_C1_C2") ;
   el_string_to_opcode_map.bind("LX_B_C1_C3", (Opcode) LX_B_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C1_C3, "LX_B_C1_C3") ;
   el_string_to_opcode_map.bind("LX_B_C2_V1", (Opcode) LX_B_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C2_V1, "LX_B_C2_V1") ;
   el_string_to_opcode_map.bind("LX_B_C2_C1", (Opcode) LX_B_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C2_C1, "LX_B_C2_C1") ;
   el_string_to_opcode_map.bind("LX_B_C2_C2", (Opcode) LX_B_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C2_C2, "LX_B_C2_C2") ;
   el_string_to_opcode_map.bind("LX_B_C2_C3", (Opcode) LX_B_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C2_C3, "LX_B_C2_C3") ;
   el_string_to_opcode_map.bind("LX_B_C3_V1", (Opcode) LX_B_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C3_V1, "LX_B_C3_V1") ;
   el_string_to_opcode_map.bind("LX_B_C3_C1", (Opcode) LX_B_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C3_C1, "LX_B_C3_C1") ;
   el_string_to_opcode_map.bind("LX_B_C3_C2", (Opcode) LX_B_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C3_C2, "LX_B_C3_C2") ;
   el_string_to_opcode_map.bind("LX_B_C3_C3", (Opcode) LX_B_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_B_C3_C3, "LX_B_C3_C3") ;

   el_string_to_opcode_map.bind("LX_H_V1_V1", (Opcode) LX_H_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_V1_V1, "LX_H_V1_V1") ;
   el_string_to_opcode_map.bind("LX_H_V1_C1", (Opcode) LX_H_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_V1_C1, "LX_H_V1_C1") ;
   el_string_to_opcode_map.bind("LX_H_V1_C2", (Opcode) LX_H_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_V1_C2, "LX_H_V1_C2") ;
   el_string_to_opcode_map.bind("LX_H_V1_C3", (Opcode) LX_H_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_V1_C3, "LX_H_V1_C3") ;
   el_string_to_opcode_map.bind("LX_H_C1_V1", (Opcode) LX_H_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C1_V1, "LX_H_C1_V1") ;
   el_string_to_opcode_map.bind("LX_H_C1_C1", (Opcode) LX_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C1_C1, "LX_H_C1_C1") ;
   el_string_to_opcode_map.bind("LX_H_C1_C2", (Opcode) LX_H_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C1_C2, "LX_H_C1_C2") ;
   el_string_to_opcode_map.bind("LX_H_C1_C3", (Opcode) LX_H_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C1_C3, "LX_H_C1_C3") ;
   el_string_to_opcode_map.bind("LX_H_C2_V1", (Opcode) LX_H_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C2_V1, "LX_H_C2_V1") ;
   el_string_to_opcode_map.bind("LX_H_C2_C1", (Opcode) LX_H_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C2_C1, "LX_H_C2_C1") ;
   el_string_to_opcode_map.bind("LX_H_C2_C2", (Opcode) LX_H_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C2_C2, "LX_H_C2_C2") ;
   el_string_to_opcode_map.bind("LX_H_C2_C3", (Opcode) LX_H_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C2_C3, "LX_H_C2_C3") ;
   el_string_to_opcode_map.bind("LX_H_C3_V1", (Opcode) LX_H_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C3_V1, "LX_H_C3_V1") ;
   el_string_to_opcode_map.bind("LX_H_C3_C1", (Opcode) LX_H_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C3_C1, "LX_H_C3_C1") ;
   el_string_to_opcode_map.bind("LX_H_C3_C2", (Opcode) LX_H_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C3_C2, "LX_H_C3_C2") ;
   el_string_to_opcode_map.bind("LX_H_C3_C3", (Opcode) LX_H_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_H_C3_C3, "LX_H_C3_C3") ;

   el_string_to_opcode_map.bind("LX_W_V1_V1", (Opcode) LX_W_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_V1_V1, "LX_W_V1_V1") ;
   el_string_to_opcode_map.bind("LX_W_V1_C1", (Opcode) LX_W_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_V1_C1, "LX_W_V1_C1") ;
   el_string_to_opcode_map.bind("LX_W_V1_C2", (Opcode) LX_W_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_V1_C2, "LX_W_V1_C2") ;
   el_string_to_opcode_map.bind("LX_W_V1_C3", (Opcode) LX_W_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_V1_C3, "LX_W_V1_C3") ;
   el_string_to_opcode_map.bind("LX_W_C1_V1", (Opcode) LX_W_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C1_V1, "LX_W_C1_V1") ;
   el_string_to_opcode_map.bind("LX_W_C1_C1", (Opcode) LX_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C1_C1, "LX_W_C1_C1") ;
   el_string_to_opcode_map.bind("LX_W_C1_C2", (Opcode) LX_W_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C1_C2, "LX_W_C1_C2") ;
   el_string_to_opcode_map.bind("LX_W_C1_C3", (Opcode) LX_W_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C1_C3, "LX_W_C1_C3") ;
   el_string_to_opcode_map.bind("LX_W_C2_V1", (Opcode) LX_W_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C2_V1, "LX_W_C2_V1") ;
   el_string_to_opcode_map.bind("LX_W_C2_C1", (Opcode) LX_W_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C2_C1, "LX_W_C2_C1") ;
   el_string_to_opcode_map.bind("LX_W_C2_C2", (Opcode) LX_W_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C2_C2, "LX_W_C2_C2") ;
   el_string_to_opcode_map.bind("LX_W_C2_C3", (Opcode) LX_W_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C2_C3, "LX_W_C2_C3") ;
   el_string_to_opcode_map.bind("LX_W_C3_V1", (Opcode) LX_W_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C3_V1, "LX_W_C3_V1") ;
   el_string_to_opcode_map.bind("LX_W_C3_C1", (Opcode) LX_W_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C3_C1, "LX_W_C3_C1") ;
   el_string_to_opcode_map.bind("LX_W_C3_C2", (Opcode) LX_W_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C3_C2, "LX_W_C3_C2") ;
   el_string_to_opcode_map.bind("LX_W_C3_C3", (Opcode) LX_W_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_W_C3_C3, "LX_W_C3_C3") ;

   el_string_to_opcode_map.bind("LX_Q_V1_V1", (Opcode) LX_Q_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_V1_V1, "LX_Q_V1_V1") ;
   el_string_to_opcode_map.bind("LX_Q_V1_C1", (Opcode) LX_Q_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_V1_C1, "LX_Q_V1_C1") ;
   el_string_to_opcode_map.bind("LX_Q_V1_C2", (Opcode) LX_Q_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_V1_C2, "LX_Q_V1_C2") ;
   el_string_to_opcode_map.bind("LX_Q_V1_C3", (Opcode) LX_Q_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_V1_C3, "LX_Q_V1_C3") ;
   el_string_to_opcode_map.bind("LX_Q_C1_V1", (Opcode) LX_Q_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C1_V1, "LX_Q_C1_V1") ;
   el_string_to_opcode_map.bind("LX_Q_C1_C1", (Opcode) LX_Q_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C1_C1, "LX_Q_C1_C1") ;
   el_string_to_opcode_map.bind("LX_Q_C1_C2", (Opcode) LX_Q_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C1_C2, "LX_Q_C1_C2") ;
   el_string_to_opcode_map.bind("LX_Q_C1_C3", (Opcode) LX_Q_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C1_C3, "LX_Q_C1_C3") ;
   el_string_to_opcode_map.bind("LX_Q_C2_V1", (Opcode) LX_Q_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C2_V1, "LX_Q_C2_V1") ;
   el_string_to_opcode_map.bind("LX_Q_C2_C1", (Opcode) LX_Q_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C2_C1, "LX_Q_C2_C1") ;
   el_string_to_opcode_map.bind("LX_Q_C2_C2", (Opcode) LX_Q_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C2_C2, "LX_Q_C2_C2") ;
   el_string_to_opcode_map.bind("LX_Q_C2_C3", (Opcode) LX_Q_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C2_C3, "LX_Q_C2_C3") ;
   el_string_to_opcode_map.bind("LX_Q_C3_V1", (Opcode) LX_Q_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C3_V1, "LX_Q_C3_V1") ;
   el_string_to_opcode_map.bind("LX_Q_C3_C1", (Opcode) LX_Q_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C3_C1, "LX_Q_C3_C1") ;
   el_string_to_opcode_map.bind("LX_Q_C3_C2", (Opcode) LX_Q_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C3_C2, "LX_Q_C3_C2") ;
   el_string_to_opcode_map.bind("LX_Q_C3_C3", (Opcode) LX_Q_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LX_Q_C3_C3, "LX_Q_C3_C3") ;
}

static void el_init_elcor_opcode_maps_memory_8()
{
   el_string_to_opcode_map.bind("LG_B_V1_V1", (Opcode) LG_B_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_V1_V1, "LG_B_V1_V1") ;
   el_string_to_opcode_map.bind("LG_B_V1_C1", (Opcode) LG_B_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_V1_C1, "LG_B_V1_C1") ;
   el_string_to_opcode_map.bind("LG_B_V1_C2", (Opcode) LG_B_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_V1_C2, "LG_B_V1_C2") ;
   el_string_to_opcode_map.bind("LG_B_V1_C3", (Opcode) LG_B_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_V1_C3, "LG_B_V1_C3") ;
   el_string_to_opcode_map.bind("LG_B_C1_V1", (Opcode) LG_B_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C1_V1, "LG_B_C1_V1") ;
   el_string_to_opcode_map.bind("LG_B_C1_C1", (Opcode) LG_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C1_C1, "LG_B_C1_C1") ;
   el_string_to_opcode_map.bind("LG_B_C1_C2", (Opcode) LG_B_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C1_C2, "LG_B_C1_C2") ;
   el_string_to_opcode_map.bind("LG_B_C1_C3", (Opcode) LG_B_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C1_C3, "LG_B_C1_C3") ;
   el_string_to_opcode_map.bind("LG_B_C2_V1", (Opcode) LG_B_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C2_V1, "LG_B_C2_V1") ;
   el_string_to_opcode_map.bind("LG_B_C2_C1", (Opcode) LG_B_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C2_C1, "LG_B_C2_C1") ;
   el_string_to_opcode_map.bind("LG_B_C2_C2", (Opcode) LG_B_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C2_C2, "LG_B_C2_C2") ;
   el_string_to_opcode_map.bind("LG_B_C2_C3", (Opcode) LG_B_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C2_C3, "LG_B_C2_C3") ;
   el_string_to_opcode_map.bind("LG_B_C3_V1", (Opcode) LG_B_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C3_V1, "LG_B_C3_V1") ;
   el_string_to_opcode_map.bind("LG_B_C3_C1", (Opcode) LG_B_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C3_C1, "LG_B_C3_C1") ;
   el_string_to_opcode_map.bind("LG_B_C3_C2", (Opcode) LG_B_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C3_C2, "LG_B_C3_C2") ;
   el_string_to_opcode_map.bind("LG_B_C3_C3", (Opcode) LG_B_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_B_C3_C3, "LG_B_C3_C3") ;

   el_string_to_opcode_map.bind("LG_H_V1_V1", (Opcode) LG_H_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_V1_V1, "LG_H_V1_V1") ;
   el_string_to_opcode_map.bind("LG_H_V1_C1", (Opcode) LG_H_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_V1_C1, "LG_H_V1_C1") ;
   el_string_to_opcode_map.bind("LG_H_V1_C2", (Opcode) LG_H_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_V1_C2, "LG_H_V1_C2") ;
   el_string_to_opcode_map.bind("LG_H_V1_C3", (Opcode) LG_H_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_V1_C3, "LG_H_V1_C3") ;
   el_string_to_opcode_map.bind("LG_H_C1_V1", (Opcode) LG_H_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C1_V1, "LG_H_C1_V1") ;
   el_string_to_opcode_map.bind("LG_H_C1_C1", (Opcode) LG_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C1_C1, "LG_H_C1_C1") ;
   el_string_to_opcode_map.bind("LG_H_C1_C2", (Opcode) LG_H_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C1_C2, "LG_H_C1_C2") ;
   el_string_to_opcode_map.bind("LG_H_C1_C3", (Opcode) LG_H_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C1_C3, "LG_H_C1_C3") ;
   el_string_to_opcode_map.bind("LG_H_C2_V1", (Opcode) LG_H_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C2_V1, "LG_H_C2_V1") ;
   el_string_to_opcode_map.bind("LG_H_C2_C1", (Opcode) LG_H_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C2_C1, "LG_H_C2_C1") ;
   el_string_to_opcode_map.bind("LG_H_C2_C2", (Opcode) LG_H_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C2_C2, "LG_H_C2_C2") ;
   el_string_to_opcode_map.bind("LG_H_C2_C3", (Opcode) LG_H_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C2_C3, "LG_H_C2_C3") ;
   el_string_to_opcode_map.bind("LG_H_C3_V1", (Opcode) LG_H_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C3_V1, "LG_H_C3_V1") ;
   el_string_to_opcode_map.bind("LG_H_C3_C1", (Opcode) LG_H_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C3_C1, "LG_H_C3_C1") ;
   el_string_to_opcode_map.bind("LG_H_C3_C2", (Opcode) LG_H_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C3_C2, "LG_H_C3_C2") ;
   el_string_to_opcode_map.bind("LG_H_C3_C3", (Opcode) LG_H_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_H_C3_C3, "LG_H_C3_C3") ;

   el_string_to_opcode_map.bind("LG_W_V1_V1", (Opcode) LG_W_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_V1_V1, "LG_W_V1_V1") ;
   el_string_to_opcode_map.bind("LG_W_V1_C1", (Opcode) LG_W_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_V1_C1, "LG_W_V1_C1") ;
   el_string_to_opcode_map.bind("LG_W_V1_C2", (Opcode) LG_W_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_V1_C2, "LG_W_V1_C2") ;
   el_string_to_opcode_map.bind("LG_W_V1_C3", (Opcode) LG_W_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_V1_C3, "LG_W_V1_C3") ;
   el_string_to_opcode_map.bind("LG_W_C1_V1", (Opcode) LG_W_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C1_V1, "LG_W_C1_V1") ;
   el_string_to_opcode_map.bind("LG_W_C1_C1", (Opcode) LG_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C1_C1, "LG_W_C1_C1") ;
   el_string_to_opcode_map.bind("LG_W_C1_C2", (Opcode) LG_W_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C1_C2, "LG_W_C1_C2") ;
   el_string_to_opcode_map.bind("LG_W_C1_C3", (Opcode) LG_W_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C1_C3, "LG_W_C1_C3") ;
   el_string_to_opcode_map.bind("LG_W_C2_V1", (Opcode) LG_W_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C2_V1, "LG_W_C2_V1") ;
   el_string_to_opcode_map.bind("LG_W_C2_C1", (Opcode) LG_W_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C2_C1, "LG_W_C2_C1") ;
   el_string_to_opcode_map.bind("LG_W_C2_C2", (Opcode) LG_W_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C2_C2, "LG_W_C2_C2") ;
   el_string_to_opcode_map.bind("LG_W_C2_C3", (Opcode) LG_W_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C2_C3, "LG_W_C2_C3") ;
   el_string_to_opcode_map.bind("LG_W_C3_V1", (Opcode) LG_W_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C3_V1, "LG_W_C3_V1") ;
   el_string_to_opcode_map.bind("LG_W_C3_C1", (Opcode) LG_W_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C3_C1, "LG_W_C3_C1") ;
   el_string_to_opcode_map.bind("LG_W_C3_C2", (Opcode) LG_W_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C3_C2, "LG_W_C3_C2") ;
   el_string_to_opcode_map.bind("LG_W_C3_C3", (Opcode) LG_W_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_W_C3_C3, "LG_W_C3_C3") ;

   el_string_to_opcode_map.bind("LG_Q_V1_V1", (Opcode) LG_Q_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_V1_V1, "LG_Q_V1_V1") ;
   el_string_to_opcode_map.bind("LG_Q_V1_C1", (Opcode) LG_Q_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_V1_C1, "LG_Q_V1_C1") ;
   el_string_to_opcode_map.bind("LG_Q_V1_C2", (Opcode) LG_Q_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_V1_C2, "LG_Q_V1_C2") ;
   el_string_to_opcode_map.bind("LG_Q_V1_C3", (Opcode) LG_Q_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_V1_C3, "LG_Q_V1_C3") ;
   el_string_to_opcode_map.bind("LG_Q_C1_V1", (Opcode) LG_Q_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C1_V1, "LG_Q_C1_V1") ;
   el_string_to_opcode_map.bind("LG_Q_C1_C1", (Opcode) LG_Q_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C1_C1, "LG_Q_C1_C1") ;
   el_string_to_opcode_map.bind("LG_Q_C1_C2", (Opcode) LG_Q_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C1_C2, "LG_Q_C1_C2") ;
   el_string_to_opcode_map.bind("LG_Q_C1_C3", (Opcode) LG_Q_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C1_C3, "LG_Q_C1_C3") ;
   el_string_to_opcode_map.bind("LG_Q_C2_V1", (Opcode) LG_Q_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C2_V1, "LG_Q_C2_V1") ;
   el_string_to_opcode_map.bind("LG_Q_C2_C1", (Opcode) LG_Q_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C2_C1, "LG_Q_C2_C1") ;
   el_string_to_opcode_map.bind("LG_Q_C2_C2", (Opcode) LG_Q_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C2_C2, "LG_Q_C2_C2") ;
   el_string_to_opcode_map.bind("LG_Q_C2_C3", (Opcode) LG_Q_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C2_C3, "LG_Q_C2_C3") ;
   el_string_to_opcode_map.bind("LG_Q_C3_V1", (Opcode) LG_Q_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C3_V1, "LG_Q_C3_V1") ;
   el_string_to_opcode_map.bind("LG_Q_C3_C1", (Opcode) LG_Q_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C3_C1, "LG_Q_C3_C1") ;
   el_string_to_opcode_map.bind("LG_Q_C3_C2", (Opcode) LG_Q_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C3_C2, "LG_Q_C3_C2") ;
   el_string_to_opcode_map.bind("LG_Q_C3_C3", (Opcode) LG_Q_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LG_Q_C3_C3, "LG_Q_C3_C3") ;
}

static void el_init_elcor_opcode_maps_memory_9()
{
   el_string_to_opcode_map.bind("LGX_B_V1_V1", (Opcode) LGX_B_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_V1_V1, "LGX_B_V1_V1") ;
   el_string_to_opcode_map.bind("LGX_B_V1_C1", (Opcode) LGX_B_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_V1_C1, "LGX_B_V1_C1") ;
   el_string_to_opcode_map.bind("LGX_B_V1_C2", (Opcode) LGX_B_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_V1_C2, "LGX_B_V1_C2") ;
   el_string_to_opcode_map.bind("LGX_B_V1_C3", (Opcode) LGX_B_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_V1_C3, "LGX_B_V1_C3") ;
   el_string_to_opcode_map.bind("LGX_B_C1_V1", (Opcode) LGX_B_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C1_V1, "LGX_B_C1_V1") ;
   el_string_to_opcode_map.bind("LGX_B_C1_C1", (Opcode) LGX_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C1_C1, "LGX_B_C1_C1") ;
   el_string_to_opcode_map.bind("LGX_B_C1_C2", (Opcode) LGX_B_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C1_C2, "LGX_B_C1_C2") ;
   el_string_to_opcode_map.bind("LGX_B_C1_C3", (Opcode) LGX_B_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C1_C3, "LGX_B_C1_C3") ;
   el_string_to_opcode_map.bind("LGX_B_C2_V1", (Opcode) LGX_B_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C2_V1, "LGX_B_C2_V1") ;
   el_string_to_opcode_map.bind("LGX_B_C2_C1", (Opcode) LGX_B_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C2_C1, "LGX_B_C2_C1") ;
   el_string_to_opcode_map.bind("LGX_B_C2_C2", (Opcode) LGX_B_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C2_C2, "LGX_B_C2_C2") ;
   el_string_to_opcode_map.bind("LGX_B_C2_C3", (Opcode) LGX_B_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C2_C3, "LGX_B_C2_C3") ;
   el_string_to_opcode_map.bind("LGX_B_C3_V1", (Opcode) LGX_B_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C3_V1, "LGX_B_C3_V1") ;
   el_string_to_opcode_map.bind("LGX_B_C3_C1", (Opcode) LGX_B_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C3_C1, "LGX_B_C3_C1") ;
   el_string_to_opcode_map.bind("LGX_B_C3_C2", (Opcode) LGX_B_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C3_C2, "LGX_B_C3_C2") ;
   el_string_to_opcode_map.bind("LGX_B_C3_C3", (Opcode) LGX_B_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_B_C3_C3, "LGX_B_C3_C3") ;

   el_string_to_opcode_map.bind("LGX_H_V1_V1", (Opcode) LGX_H_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_V1_V1, "LGX_H_V1_V1") ;
   el_string_to_opcode_map.bind("LGX_H_V1_C1", (Opcode) LGX_H_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_V1_C1, "LGX_H_V1_C1") ;
   el_string_to_opcode_map.bind("LGX_H_V1_C2", (Opcode) LGX_H_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_V1_C2, "LGX_H_V1_C2") ;
   el_string_to_opcode_map.bind("LGX_H_V1_C3", (Opcode) LGX_H_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_V1_C3, "LGX_H_V1_C3") ;
   el_string_to_opcode_map.bind("LGX_H_C1_V1", (Opcode) LGX_H_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C1_V1, "LGX_H_C1_V1") ;
   el_string_to_opcode_map.bind("LGX_H_C1_C1", (Opcode) LGX_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C1_C1, "LGX_H_C1_C1") ;
   el_string_to_opcode_map.bind("LGX_H_C1_C2", (Opcode) LGX_H_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C1_C2, "LGX_H_C1_C2") ;
   el_string_to_opcode_map.bind("LGX_H_C1_C3", (Opcode) LGX_H_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C1_C3, "LGX_H_C1_C3") ;
   el_string_to_opcode_map.bind("LGX_H_C2_V1", (Opcode) LGX_H_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C2_V1, "LGX_H_C2_V1") ;
   el_string_to_opcode_map.bind("LGX_H_C2_C1", (Opcode) LGX_H_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C2_C1, "LGX_H_C2_C1") ;
   el_string_to_opcode_map.bind("LGX_H_C2_C2", (Opcode) LGX_H_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C2_C2, "LGX_H_C2_C2") ;
   el_string_to_opcode_map.bind("LGX_H_C2_C3", (Opcode) LGX_H_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C2_C3, "LGX_H_C2_C3") ;
   el_string_to_opcode_map.bind("LGX_H_C3_V1", (Opcode) LGX_H_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C3_V1, "LGX_H_C3_V1") ;
   el_string_to_opcode_map.bind("LGX_H_C3_C1", (Opcode) LGX_H_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C3_C1, "LGX_H_C3_C1") ;
   el_string_to_opcode_map.bind("LGX_H_C3_C2", (Opcode) LGX_H_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C3_C2, "LGX_H_C3_C2") ;
   el_string_to_opcode_map.bind("LGX_H_C3_C3", (Opcode) LGX_H_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_H_C3_C3, "LGX_H_C3_C3") ;

   el_string_to_opcode_map.bind("LGX_W_V1_V1", (Opcode) LGX_W_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_V1_V1, "LGX_W_V1_V1") ;
   el_string_to_opcode_map.bind("LGX_W_V1_C1", (Opcode) LGX_W_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_V1_C1, "LGX_W_V1_C1") ;
   el_string_to_opcode_map.bind("LGX_W_V1_C2", (Opcode) LGX_W_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_V1_C2, "LGX_W_V1_C2") ;
   el_string_to_opcode_map.bind("LGX_W_V1_C3", (Opcode) LGX_W_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_V1_C3, "LGX_W_V1_C3") ;
   el_string_to_opcode_map.bind("LGX_W_C1_V1", (Opcode) LGX_W_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C1_V1, "LGX_W_C1_V1") ;
   el_string_to_opcode_map.bind("LGX_W_C1_C1", (Opcode) LGX_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C1_C1, "LGX_W_C1_C1") ;
   el_string_to_opcode_map.bind("LGX_W_C1_C2", (Opcode) LGX_W_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C1_C2, "LGX_W_C1_C2") ;
   el_string_to_opcode_map.bind("LGX_W_C1_C3", (Opcode) LGX_W_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C1_C3, "LGX_W_C1_C3") ;
   el_string_to_opcode_map.bind("LGX_W_C2_V1", (Opcode) LGX_W_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C2_V1, "LGX_W_C2_V1") ;
   el_string_to_opcode_map.bind("LGX_W_C2_C1", (Opcode) LGX_W_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C2_C1, "LGX_W_C2_C1") ;
   el_string_to_opcode_map.bind("LGX_W_C2_C2", (Opcode) LGX_W_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C2_C2, "LGX_W_C2_C2") ;
   el_string_to_opcode_map.bind("LGX_W_C2_C3", (Opcode) LGX_W_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C2_C3, "LGX_W_C2_C3") ;
   el_string_to_opcode_map.bind("LGX_W_C3_V1", (Opcode) LGX_W_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C3_V1, "LGX_W_C3_V1") ;
   el_string_to_opcode_map.bind("LGX_W_C3_C1", (Opcode) LGX_W_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C3_C1, "LGX_W_C3_C1") ;
   el_string_to_opcode_map.bind("LGX_W_C3_C2", (Opcode) LGX_W_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C3_C2, "LGX_W_C3_C2") ;
   el_string_to_opcode_map.bind("LGX_W_C3_C3", (Opcode) LGX_W_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_W_C3_C3, "LGX_W_C3_C3") ;

   el_string_to_opcode_map.bind("LGX_Q_V1_V1", (Opcode) LGX_Q_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_V1_V1, "LGX_Q_V1_V1") ;
   el_string_to_opcode_map.bind("LGX_Q_V1_C1", (Opcode) LGX_Q_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_V1_C1, "LGX_Q_V1_C1") ;
   el_string_to_opcode_map.bind("LGX_Q_V1_C2", (Opcode) LGX_Q_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_V1_C2, "LGX_Q_V1_C2") ;
   el_string_to_opcode_map.bind("LGX_Q_V1_C3", (Opcode) LGX_Q_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_V1_C3, "LGX_Q_V1_C3") ;
   el_string_to_opcode_map.bind("LGX_Q_C1_V1", (Opcode) LGX_Q_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C1_V1, "LGX_Q_C1_V1") ;
   el_string_to_opcode_map.bind("LGX_Q_C1_C1", (Opcode) LGX_Q_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C1_C1, "LGX_Q_C1_C1") ;
   el_string_to_opcode_map.bind("LGX_Q_C1_C2", (Opcode) LGX_Q_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C1_C2, "LGX_Q_C1_C2") ;
   el_string_to_opcode_map.bind("LGX_Q_C1_C3", (Opcode) LGX_Q_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C1_C3, "LGX_Q_C1_C3") ;
   el_string_to_opcode_map.bind("LGX_Q_C2_V1", (Opcode) LGX_Q_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C2_V1, "LGX_Q_C2_V1") ;
   el_string_to_opcode_map.bind("LGX_Q_C2_C1", (Opcode) LGX_Q_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C2_C1, "LGX_Q_C2_C1") ;
   el_string_to_opcode_map.bind("LGX_Q_C2_C2", (Opcode) LGX_Q_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C2_C2, "LGX_Q_C2_C2") ;
   el_string_to_opcode_map.bind("LGX_Q_C2_C3", (Opcode) LGX_Q_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C2_C3, "LGX_Q_C2_C3") ;
   el_string_to_opcode_map.bind("LGX_Q_C3_V1", (Opcode) LGX_Q_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C3_V1, "LGX_Q_C3_V1") ;
   el_string_to_opcode_map.bind("LGX_Q_C3_C1", (Opcode) LGX_Q_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C3_C1, "LGX_Q_C3_C1") ;
   el_string_to_opcode_map.bind("LGX_Q_C3_C2", (Opcode) LGX_Q_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C3_C2, "LGX_Q_C3_C2") ;
   el_string_to_opcode_map.bind("LGX_Q_C3_C3", (Opcode) LGX_Q_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) LGX_Q_C3_C3, "LGX_Q_C3_C3") ;
}

// 11 removed because of obsolete opcode.

static void el_init_elcor_opcode_maps_memory_12()
{
   el_string_to_opcode_map.bind("FLG_S_V1_V1", (Opcode) FLG_S_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_V1_V1, "FLG_S_V1_V1") ;
   el_string_to_opcode_map.bind("FLG_S_V1_C1", (Opcode) FLG_S_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_V1_C1, "FLG_S_V1_C1") ;
   el_string_to_opcode_map.bind("FLG_S_V1_C2", (Opcode) FLG_S_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_V1_C2, "FLG_S_V1_C2") ;
   el_string_to_opcode_map.bind("FLG_S_V1_C3", (Opcode) FLG_S_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_V1_C3, "FLG_S_V1_C3") ;
   el_string_to_opcode_map.bind("FLG_S_C1_V1", (Opcode) FLG_S_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C1_V1, "FLG_S_C1_V1") ;
   el_string_to_opcode_map.bind("FLG_S_C1_C1", (Opcode) FLG_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C1_C1, "FLG_S_C1_C1") ;
   el_string_to_opcode_map.bind("FLG_S_C1_C2", (Opcode) FLG_S_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C1_C2, "FLG_S_C1_C2") ;
   el_string_to_opcode_map.bind("FLG_S_C1_C3", (Opcode) FLG_S_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C1_C3, "FLG_S_C1_C3") ;
   el_string_to_opcode_map.bind("FLG_S_C2_V1", (Opcode) FLG_S_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C2_V1, "FLG_S_C2_V1") ;
   el_string_to_opcode_map.bind("FLG_S_C2_C1", (Opcode) FLG_S_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C2_C1, "FLG_S_C2_C1") ;
   el_string_to_opcode_map.bind("FLG_S_C2_C2", (Opcode) FLG_S_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C2_C2, "FLG_S_C2_C2") ;
   el_string_to_opcode_map.bind("FLG_S_C2_C3", (Opcode) FLG_S_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C2_C3, "FLG_S_C2_C3") ;
   el_string_to_opcode_map.bind("FLG_S_C3_V1", (Opcode) FLG_S_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C3_V1, "FLG_S_C3_V1") ;
   el_string_to_opcode_map.bind("FLG_S_C3_C1", (Opcode) FLG_S_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C3_C1, "FLG_S_C3_C1") ;
   el_string_to_opcode_map.bind("FLG_S_C3_C2", (Opcode) FLG_S_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C3_C2, "FLG_S_C3_C2") ;
   el_string_to_opcode_map.bind("FLG_S_C3_C3", (Opcode) FLG_S_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_S_C3_C3, "FLG_S_C3_C3") ;

   el_string_to_opcode_map.bind("FLG_D_V1_V1", (Opcode) FLG_D_V1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_V1_V1, "FLG_D_V1_V1") ;
   el_string_to_opcode_map.bind("FLG_D_V1_C1", (Opcode) FLG_D_V1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_V1_C1, "FLG_D_V1_C1") ;
   el_string_to_opcode_map.bind("FLG_D_V1_C2", (Opcode) FLG_D_V1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_V1_C2, "FLG_D_V1_C2") ;
   el_string_to_opcode_map.bind("FLG_D_V1_C3", (Opcode) FLG_D_V1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_V1_C3, "FLG_D_V1_C3") ;
   el_string_to_opcode_map.bind("FLG_D_C1_V1", (Opcode) FLG_D_C1_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C1_V1, "FLG_D_C1_V1") ;
   el_string_to_opcode_map.bind("FLG_D_C1_C1", (Opcode) FLG_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C1_C1, "FLG_D_C1_C1") ;
   el_string_to_opcode_map.bind("FLG_D_C1_C2", (Opcode) FLG_D_C1_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C1_C2, "FLG_D_C1_C2") ;
   el_string_to_opcode_map.bind("FLG_D_C1_C3", (Opcode) FLG_D_C1_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C1_C3, "FLG_D_C1_C3") ;
   el_string_to_opcode_map.bind("FLG_D_C2_V1", (Opcode) FLG_D_C2_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C2_V1, "FLG_D_C2_V1") ;
   el_string_to_opcode_map.bind("FLG_D_C2_C1", (Opcode) FLG_D_C2_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C2_C1, "FLG_D_C2_C1") ;
   el_string_to_opcode_map.bind("FLG_D_C2_C2", (Opcode) FLG_D_C2_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C2_C2, "FLG_D_C2_C2") ;
   el_string_to_opcode_map.bind("FLG_D_C2_C3", (Opcode) FLG_D_C2_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C2_C3, "FLG_D_C2_C3") ;
   el_string_to_opcode_map.bind("FLG_D_C3_V1", (Opcode) FLG_D_C3_V1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C3_V1, "FLG_D_C3_V1") ;
   el_string_to_opcode_map.bind("FLG_D_C3_C1", (Opcode) FLG_D_C3_C1) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C3_C1, "FLG_D_C3_C1") ;
   el_string_to_opcode_map.bind("FLG_D_C3_C2", (Opcode) FLG_D_C3_C2) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C3_C2, "FLG_D_C3_C2") ;
   el_string_to_opcode_map.bind("FLG_D_C3_C3", (Opcode) FLG_D_C3_C3) ;
   el_opcode_to_string_map.bind((Opcode) FLG_D_C3_C3, "FLG_D_C3_C3") ;
}

static void el_init_elcor_opcode_maps_memory_14()
{
   el_string_to_opcode_map.bind("SG_B_V1", (Opcode) SG_B_V1) ;
   el_opcode_to_string_map.bind((Opcode) SG_B_V1, "SG_B_V1") ;
   el_string_to_opcode_map.bind("SG_B_C1", (Opcode) SG_B_C1) ;
   el_opcode_to_string_map.bind((Opcode) SG_B_C1, "SG_B_C1") ;
   el_string_to_opcode_map.bind("SG_B_C2", (Opcode) SG_B_C2) ;
   el_opcode_to_string_map.bind((Opcode) SG_B_C2, "SG_B_C2") ;
   el_string_to_opcode_map.bind("SG_B_C3", (Opcode) SG_B_C3) ;
   el_opcode_to_string_map.bind((Opcode) SG_B_C3, "SG_B_C3") ;

   el_string_to_opcode_map.bind("SG_H_V1", (Opcode) SG_H_V1) ;
   el_opcode_to_string_map.bind((Opcode) SG_H_V1, "SG_H_V1") ;
   el_string_to_opcode_map.bind("SG_H_C1", (Opcode) SG_H_C1) ;
   el_opcode_to_string_map.bind((Opcode) SG_H_C1, "SG_H_C1") ;
   el_string_to_opcode_map.bind("SG_H_C2", (Opcode) SG_H_C2) ;
   el_opcode_to_string_map.bind((Opcode) SG_H_C2, "SG_H_C2") ;
   el_string_to_opcode_map.bind("SG_H_C3", (Opcode) SG_H_C3) ;
   el_opcode_to_string_map.bind((Opcode) SG_H_C3, "SG_H_C3") ;

   el_string_to_opcode_map.bind("SG_W_V1", (Opcode) SG_W_V1) ;
   el_opcode_to_string_map.bind((Opcode) SG_W_V1, "SG_W_V1") ;
   el_string_to_opcode_map.bind("SG_W_C1", (Opcode) SG_W_C1) ;
   el_opcode_to_string_map.bind((Opcode) SG_W_C1, "SG_W_C1") ;
   el_string_to_opcode_map.bind("SG_W_C2", (Opcode) SG_W_C2) ;
   el_opcode_to_string_map.bind((Opcode) SG_W_C2, "SG_W_C2") ;
   el_string_to_opcode_map.bind("SG_W_C3", (Opcode) SG_W_C3) ;
   el_opcode_to_string_map.bind((Opcode) SG_W_C3, "SG_W_C3") ;

   el_string_to_opcode_map.bind("SG_Q_V1", (Opcode) SG_Q_V1) ;
   el_opcode_to_string_map.bind((Opcode) SG_Q_V1, "SG_Q_V1") ;
   el_string_to_opcode_map.bind("SG_Q_C1", (Opcode) SG_Q_C1) ;
   el_opcode_to_string_map.bind((Opcode) SG_Q_C1, "SG_Q_C1") ;
   el_string_to_opcode_map.bind("SG_Q_C2", (Opcode) SG_Q_C2) ;
   el_opcode_to_string_map.bind((Opcode) SG_Q_C2, "SG_Q_C2") ;
   el_string_to_opcode_map.bind("SG_Q_C3", (Opcode) SG_Q_C3) ;
   el_opcode_to_string_map.bind((Opcode) SG_Q_C3, "SG_Q_C3") ;
}

static void el_init_elcor_opcode_maps_memory_16()
{
   el_string_to_opcode_map.bind("FSG_S_V1", (Opcode) FSG_S_V1) ;
   el_opcode_to_string_map.bind((Opcode) FSG_S_V1, "FSG_S_V1") ;
   el_string_to_opcode_map.bind("FSG_S_C1", (Opcode) FSG_S_C1) ;
   el_opcode_to_string_map.bind((Opcode) FSG_S_C1, "FSG_S_C1") ;
   el_string_to_opcode_map.bind("FSG_S_C2", (Opcode) FSG_S_C2) ;
   el_opcode_to_string_map.bind((Opcode) FSG_S_C2, "FSG_S_C2") ;
   el_string_to_opcode_map.bind("FSG_S_C3", (Opcode) FSG_S_C3) ;
   el_opcode_to_string_map.bind((Opcode) FSG_S_C3, "FSG_S_C3") ;

   el_string_to_opcode_map.bind("FSG_D_V1", (Opcode) FSG_D_V1) ;
   el_opcode_to_string_map.bind((Opcode) FSG_D_V1, "FSG_D_V1") ;
   el_string_to_opcode_map.bind("FSG_D_C1", (Opcode) FSG_D_C1) ;
   el_opcode_to_string_map.bind((Opcode) FSG_D_C1, "FSG_D_C1") ;
   el_string_to_opcode_map.bind("FSG_D_C2", (Opcode) FSG_D_C2) ;
   el_opcode_to_string_map.bind((Opcode) FSG_D_C2, "FSG_D_C2") ;
   el_string_to_opcode_map.bind("FSG_D_C3", (Opcode) FSG_D_C3) ;
   el_opcode_to_string_map.bind((Opcode) FSG_D_C3, "FSG_D_C3") ;
}


// New load/store operations to save/restore and spill/unspill GPRs and FPRs. 
// These operations ignore the speculative tag bit of the register being 
// loaded or stored. -- VK/5/22/98

static void el_init_elcor_opcode_maps_memory_19() {
   el_string_to_opcode_map.bind("SAVE", (Opcode) SAVE) ;
   el_opcode_to_string_map.bind((Opcode) SAVE, "SAVE") ;
   el_string_to_opcode_map.bind("RESTORE", (Opcode) RESTORE) ;
   el_opcode_to_string_map.bind((Opcode) RESTORE, "RESTORE") ;
   el_string_to_opcode_map.bind("FSAVE_S", (Opcode) FSAVE_S) ;
   el_opcode_to_string_map.bind((Opcode) FSAVE_S, "FSAVE_S") ;
   el_string_to_opcode_map.bind("FRESTORE_S", (Opcode) FRESTORE_S) ;
   el_opcode_to_string_map.bind((Opcode) FRESTORE_S, "FRESTORE_S") ;
   el_string_to_opcode_map.bind("FSAVE_D", (Opcode) FSAVE_D) ;
   el_opcode_to_string_map.bind((Opcode) FSAVE_D, "FSAVE_D") ;
   el_string_to_opcode_map.bind("FRESTORE_D", (Opcode) FRESTORE_D) ;
   el_opcode_to_string_map.bind((Opcode) FRESTORE_D, "FRESTORE_D") ;
   el_string_to_opcode_map.bind("BSAVE", (Opcode) BSAVE) ;
   el_opcode_to_string_map.bind((Opcode) BSAVE, "BSAVE") ;
   el_string_to_opcode_map.bind("BRESTORE", (Opcode) BRESTORE) ;
   el_opcode_to_string_map.bind((Opcode) BRESTORE, "BRESTORE") ;
   el_string_to_opcode_map.bind("PSAVE", (Opcode) PSAVE) ;
   el_opcode_to_string_map.bind((Opcode) PSAVE, "PSAVE") ;
   el_string_to_opcode_map.bind("PRESTORE", (Opcode) PRESTORE) ;
   el_opcode_to_string_map.bind((Opcode) PRESTORE, "PRESTORE") ;

   el_string_to_opcode_map.bind("SAVEG", (Opcode) SAVEG) ;
   el_opcode_to_string_map.bind((Opcode) SAVEG, "SAVEG") ;
   el_string_to_opcode_map.bind("RESTOREG", (Opcode) RESTOREG) ;
   el_opcode_to_string_map.bind((Opcode) RESTOREG, "RESTOREG") ;
   el_string_to_opcode_map.bind("FSAVEG_S", (Opcode) FSAVEG_S) ;
   el_opcode_to_string_map.bind((Opcode) FSAVEG_S, "FSAVEG_S") ;
   el_string_to_opcode_map.bind("FRESTOREG_S", (Opcode) FRESTOREG_S) ;
   el_opcode_to_string_map.bind((Opcode) FRESTOREG_S, "FRESTOREG_S") ;
   el_string_to_opcode_map.bind("FSAVEG_D", (Opcode) FSAVEG_D) ;
   el_opcode_to_string_map.bind((Opcode) FSAVEG_D, "FSAVEG_D") ;
   el_string_to_opcode_map.bind("FRESTOREG_D", (Opcode) FRESTOREG_D) ;
   el_opcode_to_string_map.bind((Opcode) FRESTOREG_D, "FRESTOREG_D") ;
   el_string_to_opcode_map.bind("BSAVEG", (Opcode) BSAVEG) ;
   el_opcode_to_string_map.bind((Opcode) BSAVEG, "BSAVEG") ;
   el_string_to_opcode_map.bind("BRESTOREG", (Opcode) BRESTOREG) ;
   el_opcode_to_string_map.bind((Opcode) BRESTOREG, "BRESTOREG") ;
   el_string_to_opcode_map.bind("PSAVEG", (Opcode) PSAVEG) ;
   el_opcode_to_string_map.bind((Opcode) PSAVEG, "PSAVEG") ;
   el_string_to_opcode_map.bind("PRESTOREG", (Opcode) PRESTOREG) ;
   el_opcode_to_string_map.bind((Opcode) PRESTOREG, "PRESTOREG") ;
}

void el_init_elcor_opcode_maps_memory()
{
  el_init_elcor_opcode_maps_memory_1() ;  // l
  el_init_elcor_opcode_maps_memory_2() ;  // li
  el_init_elcor_opcode_maps_memory_3() ;  // fl/fli
  el_init_elcor_opcode_maps_memory_4() ;  // s/fs
  el_init_elcor_opcode_maps_memory_5() ;  // lds/ldsi
  el_init_elcor_opcode_maps_memory_6() ;  // flds/fldsi/ldv/fldv
  el_init_elcor_opcode_maps_memory_7() ;  // lx
  el_init_elcor_opcode_maps_memory_8() ;  // lg
  el_init_elcor_opcode_maps_memory_9() ;  // lm
  el_init_elcor_opcode_maps_memory_12() ; // flg
  el_init_elcor_opcode_maps_memory_14() ; // sg
  el_init_elcor_opcode_maps_memory_16() ; // fsg
  el_init_elcor_opcode_maps_memory_19() ; // Spill/unspill gpr and fpr
}

void el_init_elcor_opcode_maps_misc()
{
   el_string_to_opcode_map.bind("BRDVI", BRDVI) ;
   el_opcode_to_string_map.bind(BRDVI, "BRDVI") ;
   el_string_to_opcode_map.bind("BRDVF", BRDVF) ;
   el_opcode_to_string_map.bind(BRDVF, "BRDVF") ;
   // long literal generation
   el_string_to_opcode_map.bind("MOVELG", MOVELG) ;
   el_opcode_to_string_map.bind(MOVELG, "MOVELG") ;
   el_string_to_opcode_map.bind("MOVELGS", MOVELGS) ;
   el_opcode_to_string_map.bind(MOVELGS, "MOVELGS") ;
   el_string_to_opcode_map.bind("MOVELGX", MOVELGX) ;
   el_opcode_to_string_map.bind(MOVELGX, "MOVELGX") ;
   // float literal generation
   el_string_to_opcode_map.bind("MOVELF", MOVELF) ;
   el_opcode_to_string_map.bind(MOVELF, "MOVELF") ;
   el_string_to_opcode_map.bind("MOVELFS", MOVELFS) ;
   el_opcode_to_string_map.bind(MOVELFS, "MOVELFS") ;
   // branch literal generation
   el_string_to_opcode_map.bind("MOVELB", MOVELB) ;
   el_opcode_to_string_map.bind(MOVELB, "MOVELB") ;
   el_string_to_opcode_map.bind("MOVELBS", MOVELBS) ;
   el_opcode_to_string_map.bind(MOVELBS, "MOVELBS") ;
   el_string_to_opcode_map.bind("MOVELBX", MOVELBX) ;
   el_opcode_to_string_map.bind(MOVELBX, "MOVELBX") ;

   el_string_to_opcode_map.bind("PBR", PBR) ;
   el_opcode_to_string_map.bind(PBR, "PBR") ;
   el_string_to_opcode_map.bind("BRU", BRU) ;
   el_opcode_to_string_map.bind(BRU, "BRU") ;
   el_string_to_opcode_map.bind("BRCT", BRCT) ;
   el_opcode_to_string_map.bind(BRCT, "BRCT") ;
   el_string_to_opcode_map.bind("BRCF", BRCF) ;
   el_opcode_to_string_map.bind(BRCF, "BRCF") ;
   el_string_to_opcode_map.bind("BRL", BRL) ;
   el_opcode_to_string_map.bind(BRL, "BRL") ;
   el_string_to_opcode_map.bind("BRLC", BRLC) ;
   el_opcode_to_string_map.bind(BRLC, "BRLC") ;
   el_string_to_opcode_map.bind("BRLP", BRLP) ;
   el_opcode_to_string_map.bind(BRLP, "BRLP") ;
   el_string_to_opcode_map.bind("RTS", RTS) ;
   el_opcode_to_string_map.bind(RTS, "RTS") ;
   el_string_to_opcode_map.bind("BRDIR", BRDIR) ;
   el_opcode_to_string_map.bind(BRDIR, "BRDIR") ;
   el_string_to_opcode_map.bind("BRIND", BRIND) ;
   el_opcode_to_string_map.bind(BRIND, "BRIND") ;
   el_string_to_opcode_map.bind("BRF_B_B_F", BRF_B_B_F) ;
   el_opcode_to_string_map.bind(BRF_B_B_F, "BRF_B_B_F") ;
   el_string_to_opcode_map.bind("BRF_B_F_F", BRF_B_F_F) ;
   el_opcode_to_string_map.bind(BRF_B_F_F, "BRF_B_F_F") ;
   el_string_to_opcode_map.bind("BRF_F_B_B", BRF_F_B_B) ;
   el_opcode_to_string_map.bind(BRF_F_B_B, "BRF_F_B_B") ;
   el_string_to_opcode_map.bind("BRF_F_F_B", BRF_F_F_B) ;
   el_opcode_to_string_map.bind(BRF_F_F_B, "BRF_F_F_B") ;
   el_string_to_opcode_map.bind("BRF_F_F_F", BRF_F_F_F) ;
   el_opcode_to_string_map.bind(BRF_F_F_F, "BRF_F_F_F") ;
   el_string_to_opcode_map.bind("BRW_B_B_F", BRW_B_B_F) ;
   el_opcode_to_string_map.bind(BRW_B_B_F, "BRW_B_B_F") ;
   el_string_to_opcode_map.bind("BRW_B_F_F", BRW_B_F_F) ;
   el_opcode_to_string_map.bind(BRW_B_F_F, "BRW_B_F_F") ;
   el_string_to_opcode_map.bind("BRW_F_B_B", BRW_F_B_B) ;
   el_opcode_to_string_map.bind(BRW_F_B_B, "BRW_F_B_B") ;
   el_string_to_opcode_map.bind("BRW_F_F_B", BRW_F_F_B) ;
   el_opcode_to_string_map.bind(BRW_F_F_B, "BRW_F_F_B") ;
   el_string_to_opcode_map.bind("BRW_F_F_F", BRW_F_F_F) ;
   el_opcode_to_string_map.bind(BRW_F_F_F, "BRW_F_F_F") ;

   el_string_to_opcode_map.bind("PROLOGUE", PROLOGUE) ;
   el_opcode_to_string_map.bind(PROLOGUE, "PROLOGUE") ;
   el_string_to_opcode_map.bind("EPILOGUE", EPILOGUE) ;
   el_opcode_to_string_map.bind(EPILOGUE, "EPILOGUE") ;
   el_string_to_opcode_map.bind("DEFINE", DEFINE) ;
   el_opcode_to_string_map.bind(DEFINE, "DEFINE") ;
   el_string_to_opcode_map.bind("ALLOC", ALLOC) ;
   el_opcode_to_string_map.bind(ALLOC, "ALLOC") ;
   el_string_to_opcode_map.bind("SIM_DIR", SIM_DIR) ;
   el_opcode_to_string_map.bind(SIM_DIR, "SIM_DIR") ;

   el_string_to_opcode_map.bind("DUMMY_BR", DUMMY_BR) ;
   el_opcode_to_string_map.bind(DUMMY_BR, "DUMMY_BR") ;
   el_string_to_opcode_map.bind("C_MERGE", C_MERGE) ;
   el_opcode_to_string_map.bind(C_MERGE, "C_MERGE") ;
   el_string_to_opcode_map.bind("D_SWITCH", D_SWITCH) ;
   el_opcode_to_string_map.bind(D_SWITCH, "D_SWITCH") ;
   el_string_to_opcode_map.bind("D_MERGE", D_MERGE) ;
   el_opcode_to_string_map.bind(D_MERGE, "D_MERGE") ;

   el_string_to_opcode_map.bind("USE", USE) ;
   el_opcode_to_string_map.bind(USE, "USE") ;
   el_string_to_opcode_map.bind("DEF", DEF) ;
   el_opcode_to_string_map.bind(DEF, "DEF") ;

   el_string_to_opcode_map.bind("REMAP", REMAP) ;
   el_opcode_to_string_map.bind(REMAP, "REMAP") ;

   el_string_to_opcode_map.bind("PRED_CLEAR", PRED_CLEAR) ;
   el_opcode_to_string_map.bind(PRED_CLEAR, "PRED_CLEAR") ;
   el_string_to_opcode_map.bind("PRED_CLEAR_ALL", PRED_CLEAR_ALL) ;
   el_opcode_to_string_map.bind(PRED_CLEAR_ALL, "PRED_CLEAR_ALL") ;
   el_string_to_opcode_map.bind("PRED_CLEAR_ALL_STATIC", PRED_CLEAR_ALL_STATIC) ;
   el_opcode_to_string_map.bind(PRED_CLEAR_ALL_STATIC, "PRED_CLEAR_ALL_STATIC") ;
   el_string_to_opcode_map.bind("PRED_CLEAR_ALL_ROTATING", PRED_CLEAR_ALL_ROTATING) ;
   el_opcode_to_string_map.bind(PRED_CLEAR_ALL_ROTATING, "PRED_CLEAR_ALL_ROTATING") ;
   el_string_to_opcode_map.bind("PRED_SET", PRED_SET) ;
   el_opcode_to_string_map.bind(PRED_SET, "PRED_SET") ;
   el_string_to_opcode_map.bind("PRED_AND", PRED_AND) ;
   el_opcode_to_string_map.bind(PRED_AND, "PRED_AND") ;
   el_string_to_opcode_map.bind("PRED_COMPL", PRED_COMPL) ;
   el_opcode_to_string_map.bind(PRED_COMPL, "PRED_COMPL") ;
   el_string_to_opcode_map.bind("PRED_LOAD_ALL", PRED_LOAD_ALL) ;
   el_opcode_to_string_map.bind(PRED_LOAD_ALL, "PRED_LOAD_ALL") ;
   el_string_to_opcode_map.bind("PRED_STORE_ALL", PRED_STORE_ALL) ;
   el_opcode_to_string_map.bind(PRED_STORE_ALL, "PRED_STORE_ALL") ;

   /* Shivarama Rao. Added the code on 06-06-2001. The earlier code of
    * using only PSEUDO_LOAD is replaced with using explicit loads for
    * different types of PSEUDO_LOAD operations. ie Scalar load, EVR load
    * and constant value load.
    */
   el_string_to_opcode_map.bind("PSEUDO_LOAD_LITERAL", PSEUDO_LOAD_LITERAL) ;
   el_opcode_to_string_map.bind(PSEUDO_LOAD_LITERAL, "PSEUDO_LOAD_LITERAL") ;

   el_string_to_opcode_map.bind("PSEUDO_LOAD_SCALAR", PSEUDO_LOAD_SCALAR) ;
   el_opcode_to_string_map.bind(PSEUDO_LOAD_SCALAR, "PSEUDO_LOAD_SCALAR") ;

   el_string_to_opcode_map.bind("PSEUDO_LOAD_SCALAR_ARRAY", PSEUDO_LOAD_SCALAR_ARRAY) ;
   el_opcode_to_string_map.bind(PSEUDO_LOAD_SCALAR_ARRAY, "PSEUDO_LOAD_SCALAR_ARRAY") ;

   el_string_to_opcode_map.bind("PSEUDO_LOAD_EVR", PSEUDO_LOAD_EVR) ;
   el_opcode_to_string_map.bind(PSEUDO_LOAD_EVR, "PSEUDO_LOAD_EVR") ;

   /* Shivarama Rao. Added the code on 06-06-2001. The earlier code of
    * using only PSEUDO_STORE is replaced with using explicit stores for
    * different types of PSEUDO_STORE operations. i.e scalar store and
    * and evr store.
    */
   el_string_to_opcode_map.bind("PSEUDO_STORE_SCALAR", PSEUDO_STORE_SCALAR) ;
   el_opcode_to_string_map.bind(PSEUDO_STORE_SCALAR, "PSEUDO_STORE_SCALAR") ;

   el_string_to_opcode_map.bind("PSEUDO_STORE_SCALAR_ARRAY", PSEUDO_STORE_SCALAR_ARRAY) ;
   el_opcode_to_string_map.bind(PSEUDO_STORE_SCALAR_ARRAY, "PSEUDO_STORE_SCALAR_ARRAY") ;

   el_string_to_opcode_map.bind("PSEUDO_STORE_EVR", PSEUDO_STORE_EVR) ;
   el_opcode_to_string_map.bind(PSEUDO_STORE_EVR, "PSEUDO_STORE_EVR") ;

   el_string_to_opcode_map.bind("PSEUDO_PRINT", PSEUDO_PRINT) ;
   el_opcode_to_string_map.bind(PSEUDO_PRINT, "PSEUDO_PRINT") ;

   el_string_to_opcode_map.bind("PSEUDO_INIT_MEM", PSEUDO_INIT_MEM) ;
   el_opcode_to_string_map.bind(PSEUDO_INIT_MEM, "PSEUDO_INIT_MEM") ;

   /*modification for custom opcode (scott/wilkin 05-31-02)*/
   el_string_to_opcode_map.bind("CUSTOM", CUSTOM) ;
   el_opcode_to_string_map.bind(CUSTOM, "CUSTOM") ;

   //distribtued VLIW, hongtaoz 11-17-03
   el_string_to_opcode_map.bind("EBR", EBR) ;
   el_opcode_to_string_map.bind(EBR, "EBR") ;
   el_string_to_opcode_map.bind("EBRL", EBRL) ;
   el_opcode_to_string_map.bind(EBRL, "EBRL") ;
   el_string_to_opcode_map.bind("EBRF_B_B_F", EBRF_B_B_F) ;
   el_opcode_to_string_map.bind(EBRF_B_B_F, "EBRF_B_B_F") ;
   el_string_to_opcode_map.bind("EBRF_F_F_F", EBRF_F_F_F) ;
   el_opcode_to_string_map.bind(EBRF_F_F_F, "EBRF_F_F_F") ;
   el_string_to_opcode_map.bind("ERTS", ERTS) ;
   el_opcode_to_string_map.bind(ERTS, "ERTS") ;
   el_string_to_opcode_map.bind("BCAST", BCAST) ;
   el_opcode_to_string_map.bind(BCAST, "BCAST") ;
   el_string_to_opcode_map.bind("BCASTF", BCASTF) ;
   el_opcode_to_string_map.bind(BCASTF, "BCASTF") ;

   el_string_to_opcode_map.bind("SLEEP", SLEEP) ;
   el_opcode_to_string_map.bind(SLEEP, "SLEEP") ;
   el_string_to_opcode_map.bind("SLEEP_FT", SLEEP_FT) ;
   el_opcode_to_string_map.bind(SLEEP_FT, "SLEEP_FT") ;
   el_string_to_opcode_map.bind("SLEEP_UC", SLEEP_UC) ;
   el_opcode_to_string_map.bind(SLEEP_UC, "SLEEP_UC") ;
   el_string_to_opcode_map.bind("WAKE", WAKE) ;
   el_opcode_to_string_map.bind(WAKE, "WAKE") ;
   el_string_to_opcode_map.bind("WAKE_FT", WAKE_FT) ;
   el_opcode_to_string_map.bind(WAKE_FT, "WAKE_FT") ;
   el_string_to_opcode_map.bind("WAKE_UC", WAKE_UC) ;
   el_opcode_to_string_map.bind(WAKE_UC, "WAKE_UC") ;

   el_string_to_opcode_map.bind("SEND_I", SEND_I) ;
   el_opcode_to_string_map.bind(SEND_I, "SEND_I") ;
   el_string_to_opcode_map.bind("SEND_F", SEND_F) ;
   el_opcode_to_string_map.bind(SEND_F, "SEND_F") ;
   el_string_to_opcode_map.bind("SEND_P", SEND_P) ;
   el_opcode_to_string_map.bind(SEND_P, "SEND_P") ;
   el_string_to_opcode_map.bind("SEND_B", SEND_B) ;
   el_opcode_to_string_map.bind(SEND_B, "SEND_B") ;
   el_string_to_opcode_map.bind("RECV_I", RECV_I) ;
   el_opcode_to_string_map.bind(RECV_I, "RECV_I") ;
   el_string_to_opcode_map.bind("RECV_F", RECV_F) ;
   el_opcode_to_string_map.bind(RECV_F, "RECV_F") ;
   el_string_to_opcode_map.bind("RECV_P", RECV_P) ;
   el_opcode_to_string_map.bind(RECV_P, "RECV_P") ;
   el_string_to_opcode_map.bind("RECV_B", RECV_B) ;
   el_opcode_to_string_map.bind(RECV_B, "RECV_B") ;
}

void el_init_elcor_base_opcode_maps()
{
   // add opcode-to-string mapping for base opcodes

   el_base_opcode_to_string_map.bind(BASE_CMPP_W_FALSE, "BASE_CMPP_W_FALSE");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_EQ, "BASE_CMPP_W_EQ");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_LT, "BASE_CMPP_W_LT");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_LEQ, "BASE_CMPP_W_LEQ");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_GT, "BASE_CMPP_W_GT");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_GEQ, "BASE_CMPP_W_GEQ");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_SV, "BASE_CMPP_W_SV");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_OD, "BASE_CMPP_W_OD");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_TRUE , "BASE_CMPP_W_TRUE");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_NEQ, "BASE_CMPP_W_NEQ");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_LLT, "BASE_CMPP_W_LLT");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_LLEQ , "BASE_CMPP_W_LLEQ");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_LGT, "BASE_CMPP_W_LGT");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_LGEQ , "BASE_CMPP_W_LGEQ");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_NSV, "BASE_CMPP_W_NSV");
   el_base_opcode_to_string_map.bind(BASE_CMPP_W_EV, "BASE_CMPP_W_EV");

   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_FALSE,"BASE_FCMPP_S_FALSE");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_EQ, "BASE_FCMPP_S_EQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_LT, "BASE_FCMPP_S_LT");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_LEQ, "BASE_FCMPP_S_LEQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_GT, "BASE_FCMPP_S_GT");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_GEQ, "BASE_FCMPP_S_GEQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_NEQ, "BASE_FCMPP_S_NEQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_S_TRUE, "BASE_FCMPP_S_TRUE");

   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_FALSE,"BASE_FCMPP_D_FALSE");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_EQ, "BASE_FCMPP_D_EQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_LT, "BASE_FCMPP_D_LT");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_LEQ, "BASE_FCMPP_D_LEQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_GT, "BASE_FCMPP_D_GT");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_GEQ, "BASE_FCMPP_D_GEQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_NEQ, "BASE_FCMPP_D_NEQ");
   el_base_opcode_to_string_map.bind(BASE_FCMPP_D_TRUE, "BASE_FCMPP_D_TRUE");
}

//////////////////////////////////////////////////////////////////////////

void el_init_elcor_opcode_maps_cmpp2_5()
{
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_UN", (Opcode) CMPP_W_LGT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_UN, "CMPP_W_LGT_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_UC", (Opcode) CMPP_W_LGT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_UC, "CMPP_W_LGT_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_CN", (Opcode) CMPP_W_LGT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_CN, "CMPP_W_LGT_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_CC", (Opcode) CMPP_W_LGT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_CC, "CMPP_W_LGT_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_ON", (Opcode) CMPP_W_LGT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_ON, "CMPP_W_LGT_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_OC", (Opcode) CMPP_W_LGT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_OC, "CMPP_W_LGT_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_AN", (Opcode) CMPP_W_LGT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_AN, "CMPP_W_LGT_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UN_AC", (Opcode) CMPP_W_LGT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UN_AC, "CMPP_W_LGT_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_UN", (Opcode) CMPP_W_LGT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_UN, "CMPP_W_LGT_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_UC", (Opcode) CMPP_W_LGT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_UC, "CMPP_W_LGT_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_CN", (Opcode) CMPP_W_LGT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_CN, "CMPP_W_LGT_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_CC", (Opcode) CMPP_W_LGT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_CC, "CMPP_W_LGT_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_ON", (Opcode) CMPP_W_LGT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_ON, "CMPP_W_LGT_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_OC", (Opcode) CMPP_W_LGT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_OC, "CMPP_W_LGT_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_AN", (Opcode) CMPP_W_LGT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_AN, "CMPP_W_LGT_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_UC_AC", (Opcode) CMPP_W_LGT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_UC_AC, "CMPP_W_LGT_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_UN", (Opcode) CMPP_W_LGT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_UN, "CMPP_W_LGT_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_UC", (Opcode) CMPP_W_LGT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_UC, "CMPP_W_LGT_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_CN", (Opcode) CMPP_W_LGT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_CN, "CMPP_W_LGT_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_CC", (Opcode) CMPP_W_LGT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_CC, "CMPP_W_LGT_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_ON", (Opcode) CMPP_W_LGT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_ON, "CMPP_W_LGT_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_OC", (Opcode) CMPP_W_LGT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_OC, "CMPP_W_LGT_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_AN", (Opcode) CMPP_W_LGT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_AN, "CMPP_W_LGT_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CN_AC", (Opcode) CMPP_W_LGT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CN_AC, "CMPP_W_LGT_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_UN", (Opcode) CMPP_W_LGT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_UN, "CMPP_W_LGT_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_UC", (Opcode) CMPP_W_LGT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_UC, "CMPP_W_LGT_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_CN", (Opcode) CMPP_W_LGT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_CN, "CMPP_W_LGT_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_CC", (Opcode) CMPP_W_LGT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_CC, "CMPP_W_LGT_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_ON", (Opcode) CMPP_W_LGT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_ON, "CMPP_W_LGT_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_OC", (Opcode) CMPP_W_LGT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_OC, "CMPP_W_LGT_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_AN", (Opcode) CMPP_W_LGT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_AN, "CMPP_W_LGT_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_CC_AC", (Opcode) CMPP_W_LGT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_CC_AC, "CMPP_W_LGT_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_UN", (Opcode) CMPP_W_LGT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_UN, "CMPP_W_LGT_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_UC", (Opcode) CMPP_W_LGT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_UC, "CMPP_W_LGT_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_CN", (Opcode) CMPP_W_LGT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_CN, "CMPP_W_LGT_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_CC", (Opcode) CMPP_W_LGT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_CC, "CMPP_W_LGT_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_ON", (Opcode) CMPP_W_LGT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_ON, "CMPP_W_LGT_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_OC", (Opcode) CMPP_W_LGT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_OC, "CMPP_W_LGT_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_AN", (Opcode) CMPP_W_LGT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_AN, "CMPP_W_LGT_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_ON_AC", (Opcode) CMPP_W_LGT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_ON_AC, "CMPP_W_LGT_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_UN", (Opcode) CMPP_W_LGT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_UN, "CMPP_W_LGT_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_UC", (Opcode) CMPP_W_LGT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_UC, "CMPP_W_LGT_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_CN", (Opcode) CMPP_W_LGT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_CN, "CMPP_W_LGT_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_CC", (Opcode) CMPP_W_LGT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_CC, "CMPP_W_LGT_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_ON", (Opcode) CMPP_W_LGT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_ON, "CMPP_W_LGT_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_OC", (Opcode) CMPP_W_LGT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_OC, "CMPP_W_LGT_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_AN", (Opcode) CMPP_W_LGT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_AN, "CMPP_W_LGT_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_OC_AC", (Opcode) CMPP_W_LGT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_OC_AC, "CMPP_W_LGT_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_UN", (Opcode) CMPP_W_LGT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_UN, "CMPP_W_LGT_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_UC", (Opcode) CMPP_W_LGT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_UC, "CMPP_W_LGT_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_CN", (Opcode) CMPP_W_LGT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_CN, "CMPP_W_LGT_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_CC", (Opcode) CMPP_W_LGT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_CC, "CMPP_W_LGT_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_ON", (Opcode) CMPP_W_LGT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_ON, "CMPP_W_LGT_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_OC", (Opcode) CMPP_W_LGT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_OC, "CMPP_W_LGT_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_AN", (Opcode) CMPP_W_LGT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_AN, "CMPP_W_LGT_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AN_AC", (Opcode) CMPP_W_LGT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AN_AC, "CMPP_W_LGT_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_UN", (Opcode) CMPP_W_LGT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_UN, "CMPP_W_LGT_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_UC", (Opcode) CMPP_W_LGT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_UC, "CMPP_W_LGT_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_CN", (Opcode) CMPP_W_LGT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_CN, "CMPP_W_LGT_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_CC", (Opcode) CMPP_W_LGT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_CC, "CMPP_W_LGT_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_ON", (Opcode) CMPP_W_LGT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_ON, "CMPP_W_LGT_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_OC", (Opcode) CMPP_W_LGT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_OC, "CMPP_W_LGT_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_AN", (Opcode) CMPP_W_LGT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_AN, "CMPP_W_LGT_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGT_AC_AC", (Opcode) CMPP_W_LGT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGT_AC_AC, "CMPP_W_LGT_AC_AC") ;
}

void el_init_elcor_opcode_maps_cmpp2_6()
{
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_UN", (Opcode) CMPP_W_LGEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_UN, "CMPP_W_LGEQ_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_UC", (Opcode) CMPP_W_LGEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_UC, "CMPP_W_LGEQ_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_CN", (Opcode) CMPP_W_LGEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_CN, "CMPP_W_LGEQ_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_CC", (Opcode) CMPP_W_LGEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_CC, "CMPP_W_LGEQ_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_ON", (Opcode) CMPP_W_LGEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_ON, "CMPP_W_LGEQ_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_OC", (Opcode) CMPP_W_LGEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_OC, "CMPP_W_LGEQ_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_AN", (Opcode) CMPP_W_LGEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_AN, "CMPP_W_LGEQ_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UN_AC", (Opcode) CMPP_W_LGEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UN_AC, "CMPP_W_LGEQ_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_UN", (Opcode) CMPP_W_LGEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_UN, "CMPP_W_LGEQ_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_UC", (Opcode) CMPP_W_LGEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_UC, "CMPP_W_LGEQ_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_CN", (Opcode) CMPP_W_LGEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_CN, "CMPP_W_LGEQ_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_CC", (Opcode) CMPP_W_LGEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_CC, "CMPP_W_LGEQ_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_ON", (Opcode) CMPP_W_LGEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_ON, "CMPP_W_LGEQ_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_OC", (Opcode) CMPP_W_LGEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_OC, "CMPP_W_LGEQ_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_AN", (Opcode) CMPP_W_LGEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_AN, "CMPP_W_LGEQ_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_UC_AC", (Opcode) CMPP_W_LGEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_UC_AC, "CMPP_W_LGEQ_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_UN", (Opcode) CMPP_W_LGEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_UN, "CMPP_W_LGEQ_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_UC", (Opcode) CMPP_W_LGEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_UC, "CMPP_W_LGEQ_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_CN", (Opcode) CMPP_W_LGEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_CN, "CMPP_W_LGEQ_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_CC", (Opcode) CMPP_W_LGEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_CC, "CMPP_W_LGEQ_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_ON", (Opcode) CMPP_W_LGEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_ON, "CMPP_W_LGEQ_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_OC", (Opcode) CMPP_W_LGEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_OC, "CMPP_W_LGEQ_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_AN", (Opcode) CMPP_W_LGEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_AN, "CMPP_W_LGEQ_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CN_AC", (Opcode) CMPP_W_LGEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CN_AC, "CMPP_W_LGEQ_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_UN", (Opcode) CMPP_W_LGEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_UN, "CMPP_W_LGEQ_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_UC", (Opcode) CMPP_W_LGEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_UC, "CMPP_W_LGEQ_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_CN", (Opcode) CMPP_W_LGEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_CN, "CMPP_W_LGEQ_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_CC", (Opcode) CMPP_W_LGEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_CC, "CMPP_W_LGEQ_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_ON", (Opcode) CMPP_W_LGEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_ON, "CMPP_W_LGEQ_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_OC", (Opcode) CMPP_W_LGEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_OC, "CMPP_W_LGEQ_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_AN", (Opcode) CMPP_W_LGEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_AN, "CMPP_W_LGEQ_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_CC_AC", (Opcode) CMPP_W_LGEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_CC_AC, "CMPP_W_LGEQ_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_UN", (Opcode) CMPP_W_LGEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_UN, "CMPP_W_LGEQ_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_UC", (Opcode) CMPP_W_LGEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_UC, "CMPP_W_LGEQ_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_CN", (Opcode) CMPP_W_LGEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_CN, "CMPP_W_LGEQ_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_CC", (Opcode) CMPP_W_LGEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_CC, "CMPP_W_LGEQ_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_ON", (Opcode) CMPP_W_LGEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_ON, "CMPP_W_LGEQ_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_OC", (Opcode) CMPP_W_LGEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_OC, "CMPP_W_LGEQ_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_AN", (Opcode) CMPP_W_LGEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_AN, "CMPP_W_LGEQ_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_ON_AC", (Opcode) CMPP_W_LGEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_ON_AC, "CMPP_W_LGEQ_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_UN", (Opcode) CMPP_W_LGEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_UN, "CMPP_W_LGEQ_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_UC", (Opcode) CMPP_W_LGEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_UC, "CMPP_W_LGEQ_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_CN", (Opcode) CMPP_W_LGEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_CN, "CMPP_W_LGEQ_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_CC", (Opcode) CMPP_W_LGEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_CC, "CMPP_W_LGEQ_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_ON", (Opcode) CMPP_W_LGEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_ON, "CMPP_W_LGEQ_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_OC", (Opcode) CMPP_W_LGEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_OC, "CMPP_W_LGEQ_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_AN", (Opcode) CMPP_W_LGEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_AN, "CMPP_W_LGEQ_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_OC_AC", (Opcode) CMPP_W_LGEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_OC_AC, "CMPP_W_LGEQ_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_UN", (Opcode) CMPP_W_LGEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_UN, "CMPP_W_LGEQ_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_UC", (Opcode) CMPP_W_LGEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_UC, "CMPP_W_LGEQ_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_CN", (Opcode) CMPP_W_LGEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_CN, "CMPP_W_LGEQ_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_CC", (Opcode) CMPP_W_LGEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_CC, "CMPP_W_LGEQ_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_ON", (Opcode) CMPP_W_LGEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_ON, "CMPP_W_LGEQ_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_OC", (Opcode) CMPP_W_LGEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_OC, "CMPP_W_LGEQ_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_AN", (Opcode) CMPP_W_LGEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_AN, "CMPP_W_LGEQ_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AN_AC", (Opcode) CMPP_W_LGEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AN_AC, "CMPP_W_LGEQ_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_UN", (Opcode) CMPP_W_LGEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_UN, "CMPP_W_LGEQ_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_UC", (Opcode) CMPP_W_LGEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_UC, "CMPP_W_LGEQ_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_CN", (Opcode) CMPP_W_LGEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_CN, "CMPP_W_LGEQ_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_CC", (Opcode) CMPP_W_LGEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_CC, "CMPP_W_LGEQ_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_ON", (Opcode) CMPP_W_LGEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_ON, "CMPP_W_LGEQ_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_OC", (Opcode) CMPP_W_LGEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_OC, "CMPP_W_LGEQ_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_AN", (Opcode) CMPP_W_LGEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_AN, "CMPP_W_LGEQ_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LGEQ_AC_AC", (Opcode) CMPP_W_LGEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LGEQ_AC_AC, "CMPP_W_LGEQ_AC_AC") ;
}

void el_init_elcor_opcode_maps_cmpp2_7()
{
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_UN", (Opcode) CMPP_W_NSV_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_UN, "CMPP_W_NSV_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_UC", (Opcode) CMPP_W_NSV_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_UC, "CMPP_W_NSV_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_CN", (Opcode) CMPP_W_NSV_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_CN, "CMPP_W_NSV_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_CC", (Opcode) CMPP_W_NSV_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_CC, "CMPP_W_NSV_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_ON", (Opcode) CMPP_W_NSV_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_ON, "CMPP_W_NSV_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_OC", (Opcode) CMPP_W_NSV_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_OC, "CMPP_W_NSV_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_AN", (Opcode) CMPP_W_NSV_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_AN, "CMPP_W_NSV_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UN_AC", (Opcode) CMPP_W_NSV_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UN_AC, "CMPP_W_NSV_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_UN", (Opcode) CMPP_W_NSV_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_UN, "CMPP_W_NSV_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_UC", (Opcode) CMPP_W_NSV_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_UC, "CMPP_W_NSV_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_CN", (Opcode) CMPP_W_NSV_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_CN, "CMPP_W_NSV_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_CC", (Opcode) CMPP_W_NSV_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_CC, "CMPP_W_NSV_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_ON", (Opcode) CMPP_W_NSV_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_ON, "CMPP_W_NSV_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_OC", (Opcode) CMPP_W_NSV_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_OC, "CMPP_W_NSV_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_AN", (Opcode) CMPP_W_NSV_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_AN, "CMPP_W_NSV_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_UC_AC", (Opcode) CMPP_W_NSV_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_UC_AC, "CMPP_W_NSV_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_UN", (Opcode) CMPP_W_NSV_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_UN, "CMPP_W_NSV_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_UC", (Opcode) CMPP_W_NSV_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_UC, "CMPP_W_NSV_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_CN", (Opcode) CMPP_W_NSV_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_CN, "CMPP_W_NSV_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_CC", (Opcode) CMPP_W_NSV_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_CC, "CMPP_W_NSV_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_ON", (Opcode) CMPP_W_NSV_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_ON, "CMPP_W_NSV_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_OC", (Opcode) CMPP_W_NSV_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_OC, "CMPP_W_NSV_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_AN", (Opcode) CMPP_W_NSV_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_AN, "CMPP_W_NSV_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CN_AC", (Opcode) CMPP_W_NSV_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CN_AC, "CMPP_W_NSV_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_UN", (Opcode) CMPP_W_NSV_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_UN, "CMPP_W_NSV_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_UC", (Opcode) CMPP_W_NSV_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_UC, "CMPP_W_NSV_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_CN", (Opcode) CMPP_W_NSV_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_CN, "CMPP_W_NSV_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_CC", (Opcode) CMPP_W_NSV_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_CC, "CMPP_W_NSV_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_ON", (Opcode) CMPP_W_NSV_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_ON, "CMPP_W_NSV_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_OC", (Opcode) CMPP_W_NSV_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_OC, "CMPP_W_NSV_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_AN", (Opcode) CMPP_W_NSV_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_AN, "CMPP_W_NSV_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_CC_AC", (Opcode) CMPP_W_NSV_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_CC_AC, "CMPP_W_NSV_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_UN", (Opcode) CMPP_W_NSV_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_UN, "CMPP_W_NSV_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_UC", (Opcode) CMPP_W_NSV_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_UC, "CMPP_W_NSV_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_CN", (Opcode) CMPP_W_NSV_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_CN, "CMPP_W_NSV_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_CC", (Opcode) CMPP_W_NSV_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_CC, "CMPP_W_NSV_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_ON", (Opcode) CMPP_W_NSV_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_ON, "CMPP_W_NSV_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_OC", (Opcode) CMPP_W_NSV_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_OC, "CMPP_W_NSV_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_AN", (Opcode) CMPP_W_NSV_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_AN, "CMPP_W_NSV_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_ON_AC", (Opcode) CMPP_W_NSV_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_ON_AC, "CMPP_W_NSV_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_UN", (Opcode) CMPP_W_NSV_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_UN, "CMPP_W_NSV_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_UC", (Opcode) CMPP_W_NSV_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_UC, "CMPP_W_NSV_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_CN", (Opcode) CMPP_W_NSV_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_CN, "CMPP_W_NSV_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_CC", (Opcode) CMPP_W_NSV_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_CC, "CMPP_W_NSV_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_ON", (Opcode) CMPP_W_NSV_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_ON, "CMPP_W_NSV_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_OC", (Opcode) CMPP_W_NSV_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_OC, "CMPP_W_NSV_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_AN", (Opcode) CMPP_W_NSV_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_AN, "CMPP_W_NSV_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_OC_AC", (Opcode) CMPP_W_NSV_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_OC_AC, "CMPP_W_NSV_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_UN", (Opcode) CMPP_W_NSV_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_UN, "CMPP_W_NSV_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_UC", (Opcode) CMPP_W_NSV_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_UC, "CMPP_W_NSV_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_CN", (Opcode) CMPP_W_NSV_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_CN, "CMPP_W_NSV_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_CC", (Opcode) CMPP_W_NSV_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_CC, "CMPP_W_NSV_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_ON", (Opcode) CMPP_W_NSV_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_ON, "CMPP_W_NSV_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_OC", (Opcode) CMPP_W_NSV_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_OC, "CMPP_W_NSV_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_AN", (Opcode) CMPP_W_NSV_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_AN, "CMPP_W_NSV_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AN_AC", (Opcode) CMPP_W_NSV_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AN_AC, "CMPP_W_NSV_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_UN", (Opcode) CMPP_W_NSV_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_UN, "CMPP_W_NSV_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_UC", (Opcode) CMPP_W_NSV_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_UC, "CMPP_W_NSV_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_CN", (Opcode) CMPP_W_NSV_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_CN, "CMPP_W_NSV_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_CC", (Opcode) CMPP_W_NSV_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_CC, "CMPP_W_NSV_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_ON", (Opcode) CMPP_W_NSV_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_ON, "CMPP_W_NSV_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_OC", (Opcode) CMPP_W_NSV_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_OC, "CMPP_W_NSV_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_AN", (Opcode) CMPP_W_NSV_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_AN, "CMPP_W_NSV_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NSV_AC_AC", (Opcode) CMPP_W_NSV_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NSV_AC_AC, "CMPP_W_NSV_AC_AC") ;
}

void el_init_elcor_opcode_maps_cmpp2_8()
{
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_UN", (Opcode) CMPP_W_EV_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_UN, "CMPP_W_EV_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_UC", (Opcode) CMPP_W_EV_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_UC, "CMPP_W_EV_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_CN", (Opcode) CMPP_W_EV_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_CN, "CMPP_W_EV_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_CC", (Opcode) CMPP_W_EV_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_CC, "CMPP_W_EV_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_ON", (Opcode) CMPP_W_EV_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_ON, "CMPP_W_EV_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_OC", (Opcode) CMPP_W_EV_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_OC, "CMPP_W_EV_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_AN", (Opcode) CMPP_W_EV_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_AN, "CMPP_W_EV_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UN_AC", (Opcode) CMPP_W_EV_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UN_AC, "CMPP_W_EV_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EV_UC_UN", (Opcode) CMPP_W_EV_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_UN, "CMPP_W_EV_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UC_UC", (Opcode) CMPP_W_EV_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_UC, "CMPP_W_EV_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UC_CN", (Opcode) CMPP_W_EV_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_CN, "CMPP_W_EV_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UC_CC", (Opcode) CMPP_W_EV_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_CC, "CMPP_W_EV_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UC_ON", (Opcode) CMPP_W_EV_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_ON, "CMPP_W_EV_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UC_OC", (Opcode) CMPP_W_EV_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_OC, "CMPP_W_EV_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UC_AN", (Opcode) CMPP_W_EV_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_AN, "CMPP_W_EV_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_UC_AC", (Opcode) CMPP_W_EV_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_UC_AC, "CMPP_W_EV_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EV_CN_UN", (Opcode) CMPP_W_EV_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_UN, "CMPP_W_EV_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CN_UC", (Opcode) CMPP_W_EV_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_UC, "CMPP_W_EV_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CN_CN", (Opcode) CMPP_W_EV_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_CN, "CMPP_W_EV_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CN_CC", (Opcode) CMPP_W_EV_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_CC, "CMPP_W_EV_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CN_ON", (Opcode) CMPP_W_EV_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_ON, "CMPP_W_EV_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CN_OC", (Opcode) CMPP_W_EV_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_OC, "CMPP_W_EV_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CN_AN", (Opcode) CMPP_W_EV_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_AN, "CMPP_W_EV_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CN_AC", (Opcode) CMPP_W_EV_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CN_AC, "CMPP_W_EV_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EV_CC_UN", (Opcode) CMPP_W_EV_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_UN, "CMPP_W_EV_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CC_UC", (Opcode) CMPP_W_EV_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_UC, "CMPP_W_EV_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CC_CN", (Opcode) CMPP_W_EV_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_CN, "CMPP_W_EV_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CC_CC", (Opcode) CMPP_W_EV_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_CC, "CMPP_W_EV_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CC_ON", (Opcode) CMPP_W_EV_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_ON, "CMPP_W_EV_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CC_OC", (Opcode) CMPP_W_EV_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_OC, "CMPP_W_EV_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CC_AN", (Opcode) CMPP_W_EV_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_AN, "CMPP_W_EV_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_CC_AC", (Opcode) CMPP_W_EV_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_CC_AC, "CMPP_W_EV_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EV_ON_UN", (Opcode) CMPP_W_EV_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_UN, "CMPP_W_EV_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_ON_UC", (Opcode) CMPP_W_EV_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_UC, "CMPP_W_EV_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_ON_CN", (Opcode) CMPP_W_EV_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_CN, "CMPP_W_EV_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_ON_CC", (Opcode) CMPP_W_EV_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_CC, "CMPP_W_EV_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_ON_ON", (Opcode) CMPP_W_EV_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_ON, "CMPP_W_EV_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_ON_OC", (Opcode) CMPP_W_EV_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_OC, "CMPP_W_EV_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_ON_AN", (Opcode) CMPP_W_EV_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_AN, "CMPP_W_EV_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_ON_AC", (Opcode) CMPP_W_EV_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_ON_AC, "CMPP_W_EV_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EV_OC_UN", (Opcode) CMPP_W_EV_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_UN, "CMPP_W_EV_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_OC_UC", (Opcode) CMPP_W_EV_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_UC, "CMPP_W_EV_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_OC_CN", (Opcode) CMPP_W_EV_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_CN, "CMPP_W_EV_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_OC_CC", (Opcode) CMPP_W_EV_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_CC, "CMPP_W_EV_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_OC_ON", (Opcode) CMPP_W_EV_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_ON, "CMPP_W_EV_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_OC_OC", (Opcode) CMPP_W_EV_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_OC, "CMPP_W_EV_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_OC_AN", (Opcode) CMPP_W_EV_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_AN, "CMPP_W_EV_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_OC_AC", (Opcode) CMPP_W_EV_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_OC_AC, "CMPP_W_EV_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EV_AN_UN", (Opcode) CMPP_W_EV_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_UN, "CMPP_W_EV_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AN_UC", (Opcode) CMPP_W_EV_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_UC, "CMPP_W_EV_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AN_CN", (Opcode) CMPP_W_EV_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_CN, "CMPP_W_EV_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AN_CC", (Opcode) CMPP_W_EV_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_CC, "CMPP_W_EV_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AN_ON", (Opcode) CMPP_W_EV_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_ON, "CMPP_W_EV_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AN_OC", (Opcode) CMPP_W_EV_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_OC, "CMPP_W_EV_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AN_AN", (Opcode) CMPP_W_EV_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_AN, "CMPP_W_EV_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AN_AC", (Opcode) CMPP_W_EV_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AN_AC, "CMPP_W_EV_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EV_AC_UN", (Opcode) CMPP_W_EV_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_UN, "CMPP_W_EV_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AC_UC", (Opcode) CMPP_W_EV_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_UC, "CMPP_W_EV_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AC_CN", (Opcode) CMPP_W_EV_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_CN, "CMPP_W_EV_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AC_CC", (Opcode) CMPP_W_EV_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_CC, "CMPP_W_EV_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AC_ON", (Opcode) CMPP_W_EV_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_ON, "CMPP_W_EV_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AC_OC", (Opcode) CMPP_W_EV_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_OC, "CMPP_W_EV_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AC_AN", (Opcode) CMPP_W_EV_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_AN, "CMPP_W_EV_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EV_AC_AC", (Opcode) CMPP_W_EV_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EV_AC_AC, "CMPP_W_EV_AC_AC") ;
}

