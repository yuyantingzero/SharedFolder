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
//      File:           opcode4.cpp
//      Authors:        Scott Mahlke
//      Created:        October 1998
//      Description:    Symbol table setup for local memory opcodes
//
/////////////////////////////////////////////////////////////////////////////


#include "opcode.h"
#include "opcode_local_mem.h"
#include "hash_functions.h"
#include <string.h>

extern Hash_map<IR_BASE_OPCODE, eString> el_base_opcode_to_string_map ;

static void el_init_elcor_opcode_maps_lm_1()
{
   el_string_to_opcode_map.bind("LL_B_L1", (Opcode) LL_B_L1) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L1, "LL_B_L1") ;
   el_string_to_opcode_map.bind("LL_B_L2", (Opcode) LL_B_L2) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L2, "LL_B_L2") ;
   el_string_to_opcode_map.bind("LL_B_L3", (Opcode) LL_B_L3) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L3, "LL_B_L3") ;
   el_string_to_opcode_map.bind("LL_B_L4", (Opcode) LL_B_L4) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L4, "LL_B_L4") ;
   el_string_to_opcode_map.bind("LL_B_L5", (Opcode) LL_B_L5) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L5, "LL_B_L5") ;
   el_string_to_opcode_map.bind("LL_B_L6", (Opcode) LL_B_L6) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L6, "LL_B_L6") ;
   el_string_to_opcode_map.bind("LL_B_L7", (Opcode) LL_B_L7) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L7, "LL_B_L7") ;
   el_string_to_opcode_map.bind("LL_B_L8", (Opcode) LL_B_L8) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L8, "LL_B_L8") ;
   el_string_to_opcode_map.bind("LL_B_L9", (Opcode) LL_B_L9) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L9, "LL_B_L9") ;
   el_string_to_opcode_map.bind("LL_B_L10", (Opcode) LL_B_L10) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L10, "LL_B_L10") ;
   el_string_to_opcode_map.bind("LL_B_L11", (Opcode) LL_B_L11) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L11, "LL_B_L11") ;
   el_string_to_opcode_map.bind("LL_B_L12", (Opcode) LL_B_L12) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L12, "LL_B_L12") ;
   el_string_to_opcode_map.bind("LL_B_L13", (Opcode) LL_B_L13) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L13, "LL_B_L13") ;
   el_string_to_opcode_map.bind("LL_B_L14", (Opcode) LL_B_L14) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L14, "LL_B_L14") ;
   el_string_to_opcode_map.bind("LL_B_L15", (Opcode) LL_B_L15) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L15, "LL_B_L15") ;
   el_string_to_opcode_map.bind("LL_B_L16", (Opcode) LL_B_L16) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L16, "LL_B_L16") ;
   el_string_to_opcode_map.bind("LL_B_L17", (Opcode) LL_B_L17) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L17, "LL_B_L17") ;
   el_string_to_opcode_map.bind("LL_B_L18", (Opcode) LL_B_L18) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L18, "LL_B_L18") ;
   el_string_to_opcode_map.bind("LL_B_L19", (Opcode) LL_B_L19) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L19, "LL_B_L19") ;
   el_string_to_opcode_map.bind("LL_B_L20", (Opcode) LL_B_L20) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L20, "LL_B_L20") ;
   el_string_to_opcode_map.bind("LL_B_L21", (Opcode) LL_B_L21) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L21, "LL_B_L21") ;
   el_string_to_opcode_map.bind("LL_B_L22", (Opcode) LL_B_L22) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L22, "LL_B_L22") ;
   el_string_to_opcode_map.bind("LL_B_L23", (Opcode) LL_B_L23) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L23, "LL_B_L23") ;
   el_string_to_opcode_map.bind("LL_B_L24", (Opcode) LL_B_L24) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L24, "LL_B_L24") ;
   el_string_to_opcode_map.bind("LL_B_L25", (Opcode) LL_B_L25) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L25, "LL_B_L25") ;
   el_string_to_opcode_map.bind("LL_B_L26", (Opcode) LL_B_L26) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L26, "LL_B_L26") ;
   el_string_to_opcode_map.bind("LL_B_L27", (Opcode) LL_B_L27) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L27, "LL_B_L27") ;
   el_string_to_opcode_map.bind("LL_B_L28", (Opcode) LL_B_L28) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L28, "LL_B_L28") ;
   el_string_to_opcode_map.bind("LL_B_L29", (Opcode) LL_B_L29) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L29, "LL_B_L29") ;
   el_string_to_opcode_map.bind("LL_B_L30", (Opcode) LL_B_L30) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L30, "LL_B_L30") ;
   el_string_to_opcode_map.bind("LL_B_L31", (Opcode) LL_B_L31) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L31, "LL_B_L31") ;
   el_string_to_opcode_map.bind("LL_B_L32", (Opcode) LL_B_L32) ;
   el_opcode_to_string_map.bind((Opcode) LL_B_L32, "LL_B_L32") ;

   el_string_to_opcode_map.bind("LL_H_L1", (Opcode) LL_H_L1) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L1, "LL_H_L1") ;
   el_string_to_opcode_map.bind("LL_H_L2", (Opcode) LL_H_L2) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L2, "LL_H_L2") ;
   el_string_to_opcode_map.bind("LL_H_L3", (Opcode) LL_H_L3) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L3, "LL_H_L3") ;
   el_string_to_opcode_map.bind("LL_H_L4", (Opcode) LL_H_L4) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L4, "LL_H_L4") ;
   el_string_to_opcode_map.bind("LL_H_L5", (Opcode) LL_H_L5) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L5, "LL_H_L5") ;
   el_string_to_opcode_map.bind("LL_H_L6", (Opcode) LL_H_L6) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L6, "LL_H_L6") ;
   el_string_to_opcode_map.bind("LL_H_L7", (Opcode) LL_H_L7) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L7, "LL_H_L7") ;
   el_string_to_opcode_map.bind("LL_H_L8", (Opcode) LL_H_L8) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L8, "LL_H_L8") ;
   el_string_to_opcode_map.bind("LL_H_L9", (Opcode) LL_H_L9) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L9, "LL_H_L9") ;
   el_string_to_opcode_map.bind("LL_H_L10", (Opcode) LL_H_L10) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L10, "LL_H_L10") ;
   el_string_to_opcode_map.bind("LL_H_L11", (Opcode) LL_H_L11) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L11, "LL_H_L11") ;
   el_string_to_opcode_map.bind("LL_H_L12", (Opcode) LL_H_L12) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L12, "LL_H_L12") ;
   el_string_to_opcode_map.bind("LL_H_L13", (Opcode) LL_H_L13) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L13, "LL_H_L13") ;
   el_string_to_opcode_map.bind("LL_H_L14", (Opcode) LL_H_L14) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L14, "LL_H_L14") ;
   el_string_to_opcode_map.bind("LL_H_L15", (Opcode) LL_H_L15) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L15, "LL_H_L15") ;
   el_string_to_opcode_map.bind("LL_H_L16", (Opcode) LL_H_L16) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L16, "LL_H_L16") ;
   el_string_to_opcode_map.bind("LL_H_L17", (Opcode) LL_H_L17) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L17, "LL_H_L17") ;
   el_string_to_opcode_map.bind("LL_H_L18", (Opcode) LL_H_L18) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L18, "LL_H_L18") ;
   el_string_to_opcode_map.bind("LL_H_L19", (Opcode) LL_H_L19) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L19, "LL_H_L19") ;
   el_string_to_opcode_map.bind("LL_H_L20", (Opcode) LL_H_L20) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L20, "LL_H_L20") ;
   el_string_to_opcode_map.bind("LL_H_L21", (Opcode) LL_H_L21) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L21, "LL_H_L21") ;
   el_string_to_opcode_map.bind("LL_H_L22", (Opcode) LL_H_L22) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L22, "LL_H_L22") ;
   el_string_to_opcode_map.bind("LL_H_L23", (Opcode) LL_H_L23) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L23, "LL_H_L23") ;
   el_string_to_opcode_map.bind("LL_H_L24", (Opcode) LL_H_L24) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L24, "LL_H_L24") ;
   el_string_to_opcode_map.bind("LL_H_L25", (Opcode) LL_H_L25) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L25, "LL_H_L25") ;
   el_string_to_opcode_map.bind("LL_H_L26", (Opcode) LL_H_L26) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L26, "LL_H_L26") ;
   el_string_to_opcode_map.bind("LL_H_L27", (Opcode) LL_H_L27) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L27, "LL_H_L27") ;
   el_string_to_opcode_map.bind("LL_H_L28", (Opcode) LL_H_L28) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L28, "LL_H_L28") ;
   el_string_to_opcode_map.bind("LL_H_L29", (Opcode) LL_H_L29) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L29, "LL_H_L29") ;
   el_string_to_opcode_map.bind("LL_H_L30", (Opcode) LL_H_L30) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L30, "LL_H_L30") ;
   el_string_to_opcode_map.bind("LL_H_L31", (Opcode) LL_H_L31) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L31, "LL_H_L31") ;
   el_string_to_opcode_map.bind("LL_H_L32", (Opcode) LL_H_L32) ;
   el_opcode_to_string_map.bind((Opcode) LL_H_L32, "LL_H_L32") ;

   el_string_to_opcode_map.bind("LL_W_L1", (Opcode) LL_W_L1) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L1, "LL_W_L1") ;
   el_string_to_opcode_map.bind("LL_W_L2", (Opcode) LL_W_L2) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L2, "LL_W_L2") ;
   el_string_to_opcode_map.bind("LL_W_L3", (Opcode) LL_W_L3) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L3, "LL_W_L3") ;
   el_string_to_opcode_map.bind("LL_W_L4", (Opcode) LL_W_L4) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L4, "LL_W_L4") ;
   el_string_to_opcode_map.bind("LL_W_L5", (Opcode) LL_W_L5) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L5, "LL_W_L5") ;
   el_string_to_opcode_map.bind("LL_W_L6", (Opcode) LL_W_L6) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L6, "LL_W_L6") ;
   el_string_to_opcode_map.bind("LL_W_L7", (Opcode) LL_W_L7) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L7, "LL_W_L7") ;
   el_string_to_opcode_map.bind("LL_W_L8", (Opcode) LL_W_L8) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L8, "LL_W_L8") ;
   el_string_to_opcode_map.bind("LL_W_L9", (Opcode) LL_W_L9) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L9, "LL_W_L9") ;
   el_string_to_opcode_map.bind("LL_W_L10", (Opcode) LL_W_L10) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L10, "LL_W_L10") ;
   el_string_to_opcode_map.bind("LL_W_L11", (Opcode) LL_W_L11) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L11, "LL_W_L11") ;
   el_string_to_opcode_map.bind("LL_W_L12", (Opcode) LL_W_L12) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L12, "LL_W_L12") ;
   el_string_to_opcode_map.bind("LL_W_L13", (Opcode) LL_W_L13) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L13, "LL_W_L13") ;
   el_string_to_opcode_map.bind("LL_W_L14", (Opcode) LL_W_L14) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L14, "LL_W_L14") ;
   el_string_to_opcode_map.bind("LL_W_L15", (Opcode) LL_W_L15) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L15, "LL_W_L15") ;
   el_string_to_opcode_map.bind("LL_W_L16", (Opcode) LL_W_L16) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L16, "LL_W_L16") ;
   el_string_to_opcode_map.bind("LL_W_L17", (Opcode) LL_W_L17) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L17, "LL_W_L17") ;
   el_string_to_opcode_map.bind("LL_W_L18", (Opcode) LL_W_L18) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L18, "LL_W_L18") ;
   el_string_to_opcode_map.bind("LL_W_L19", (Opcode) LL_W_L19) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L19, "LL_W_L19") ;
   el_string_to_opcode_map.bind("LL_W_L20", (Opcode) LL_W_L20) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L20, "LL_W_L20") ;
   el_string_to_opcode_map.bind("LL_W_L21", (Opcode) LL_W_L21) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L21, "LL_W_L21") ;
   el_string_to_opcode_map.bind("LL_W_L22", (Opcode) LL_W_L22) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L22, "LL_W_L22") ;
   el_string_to_opcode_map.bind("LL_W_L23", (Opcode) LL_W_L23) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L23, "LL_W_L23") ;
   el_string_to_opcode_map.bind("LL_W_L24", (Opcode) LL_W_L24) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L24, "LL_W_L24") ;
   el_string_to_opcode_map.bind("LL_W_L25", (Opcode) LL_W_L25) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L25, "LL_W_L25") ;
   el_string_to_opcode_map.bind("LL_W_L26", (Opcode) LL_W_L26) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L26, "LL_W_L26") ;
   el_string_to_opcode_map.bind("LL_W_L27", (Opcode) LL_W_L27) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L27, "LL_W_L27") ;
   el_string_to_opcode_map.bind("LL_W_L28", (Opcode) LL_W_L28) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L28, "LL_W_L28") ;
   el_string_to_opcode_map.bind("LL_W_L29", (Opcode) LL_W_L29) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L29, "LL_W_L29") ;
   el_string_to_opcode_map.bind("LL_W_L30", (Opcode) LL_W_L30) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L30, "LL_W_L30") ;
   el_string_to_opcode_map.bind("LL_W_L31", (Opcode) LL_W_L31) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L31, "LL_W_L31") ;
   el_string_to_opcode_map.bind("LL_W_L32", (Opcode) LL_W_L32) ;
   el_opcode_to_string_map.bind((Opcode) LL_W_L32, "LL_W_L32") ;

   el_string_to_opcode_map.bind("LL_Q_L1", (Opcode) LL_Q_L1) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L1, "LL_Q_L1") ;
   el_string_to_opcode_map.bind("LL_Q_L2", (Opcode) LL_Q_L2) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L2, "LL_Q_L2") ;
   el_string_to_opcode_map.bind("LL_Q_L3", (Opcode) LL_Q_L3) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L3, "LL_Q_L3") ;
   el_string_to_opcode_map.bind("LL_Q_L4", (Opcode) LL_Q_L4) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L4, "LL_Q_L4") ;
   el_string_to_opcode_map.bind("LL_Q_L5", (Opcode) LL_Q_L5) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L5, "LL_Q_L5") ;
   el_string_to_opcode_map.bind("LL_Q_L6", (Opcode) LL_Q_L6) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L6, "LL_Q_L6") ;
   el_string_to_opcode_map.bind("LL_Q_L7", (Opcode) LL_Q_L7) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L7, "LL_Q_L7") ;
   el_string_to_opcode_map.bind("LL_Q_L8", (Opcode) LL_Q_L8) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L8, "LL_Q_L8") ;
   el_string_to_opcode_map.bind("LL_Q_L9", (Opcode) LL_Q_L9) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L9, "LL_Q_L9") ;
   el_string_to_opcode_map.bind("LL_Q_L10", (Opcode) LL_Q_L10) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L10, "LL_Q_L10") ;
   el_string_to_opcode_map.bind("LL_Q_L11", (Opcode) LL_Q_L11) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L11, "LL_Q_L11") ;
   el_string_to_opcode_map.bind("LL_Q_L12", (Opcode) LL_Q_L12) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L12, "LL_Q_L12") ;
   el_string_to_opcode_map.bind("LL_Q_L13", (Opcode) LL_Q_L13) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L13, "LL_Q_L13") ;
   el_string_to_opcode_map.bind("LL_Q_L14", (Opcode) LL_Q_L14) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L14, "LL_Q_L14") ;
   el_string_to_opcode_map.bind("LL_Q_L15", (Opcode) LL_Q_L15) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L15, "LL_Q_L15") ;
   el_string_to_opcode_map.bind("LL_Q_L16", (Opcode) LL_Q_L16) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L16, "LL_Q_L16") ;
   el_string_to_opcode_map.bind("LL_Q_L17", (Opcode) LL_Q_L17) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L17, "LL_Q_L17") ;
   el_string_to_opcode_map.bind("LL_Q_L18", (Opcode) LL_Q_L18) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L18, "LL_Q_L18") ;
   el_string_to_opcode_map.bind("LL_Q_L19", (Opcode) LL_Q_L19) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L19, "LL_Q_L19") ;
   el_string_to_opcode_map.bind("LL_Q_L20", (Opcode) LL_Q_L20) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L20, "LL_Q_L20") ;
   el_string_to_opcode_map.bind("LL_Q_L21", (Opcode) LL_Q_L21) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L21, "LL_Q_L21") ;
   el_string_to_opcode_map.bind("LL_Q_L22", (Opcode) LL_Q_L22) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L22, "LL_Q_L22") ;
   el_string_to_opcode_map.bind("LL_Q_L23", (Opcode) LL_Q_L23) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L23, "LL_Q_L23") ;
   el_string_to_opcode_map.bind("LL_Q_L24", (Opcode) LL_Q_L24) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L24, "LL_Q_L24") ;
   el_string_to_opcode_map.bind("LL_Q_L25", (Opcode) LL_Q_L25) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L25, "LL_Q_L25") ;
   el_string_to_opcode_map.bind("LL_Q_L26", (Opcode) LL_Q_L26) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L26, "LL_Q_L26") ;
   el_string_to_opcode_map.bind("LL_Q_L27", (Opcode) LL_Q_L27) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L27, "LL_Q_L27") ;
   el_string_to_opcode_map.bind("LL_Q_L28", (Opcode) LL_Q_L28) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L28, "LL_Q_L28") ;
   el_string_to_opcode_map.bind("LL_Q_L29", (Opcode) LL_Q_L29) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L29, "LL_Q_L29") ;
   el_string_to_opcode_map.bind("LL_Q_L30", (Opcode) LL_Q_L30) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L30, "LL_Q_L30") ;
   el_string_to_opcode_map.bind("LL_Q_L31", (Opcode) LL_Q_L31) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L31, "LL_Q_L31") ;
   el_string_to_opcode_map.bind("LL_Q_L32", (Opcode) LL_Q_L32) ;
   el_opcode_to_string_map.bind((Opcode) LL_Q_L32, "LL_Q_L32") ;
}

static void el_init_elcor_opcode_maps_lm_2()
{
   el_string_to_opcode_map.bind("LLX_B_L1", (Opcode) LLX_B_L1) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L1, "LLX_B_L1") ;
   el_string_to_opcode_map.bind("LLX_B_L2", (Opcode) LLX_B_L2) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L2, "LLX_B_L2") ;
   el_string_to_opcode_map.bind("LLX_B_L3", (Opcode) LLX_B_L3) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L3, "LLX_B_L3") ;
   el_string_to_opcode_map.bind("LLX_B_L4", (Opcode) LLX_B_L4) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L4, "LLX_B_L4") ;
   el_string_to_opcode_map.bind("LLX_B_L5", (Opcode) LLX_B_L5) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L5, "LLX_B_L5") ;
   el_string_to_opcode_map.bind("LLX_B_L6", (Opcode) LLX_B_L6) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L6, "LLX_B_L6") ;
   el_string_to_opcode_map.bind("LLX_B_L7", (Opcode) LLX_B_L7) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L7, "LLX_B_L7") ;
   el_string_to_opcode_map.bind("LLX_B_L8", (Opcode) LLX_B_L8) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L8, "LLX_B_L8") ;
   el_string_to_opcode_map.bind("LLX_B_L9", (Opcode) LLX_B_L9) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L9, "LLX_B_L9") ;
   el_string_to_opcode_map.bind("LLX_B_L10", (Opcode) LLX_B_L10) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L10, "LLX_B_L10") ;
   el_string_to_opcode_map.bind("LLX_B_L11", (Opcode) LLX_B_L11) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L11, "LLX_B_L11") ;
   el_string_to_opcode_map.bind("LLX_B_L12", (Opcode) LLX_B_L12) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L12, "LLX_B_L12") ;
   el_string_to_opcode_map.bind("LLX_B_L13", (Opcode) LLX_B_L13) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L13, "LLX_B_L13") ;
   el_string_to_opcode_map.bind("LLX_B_L14", (Opcode) LLX_B_L14) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L14, "LLX_B_L14") ;
   el_string_to_opcode_map.bind("LLX_B_L15", (Opcode) LLX_B_L15) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L15, "LLX_B_L15") ;
   el_string_to_opcode_map.bind("LLX_B_L16", (Opcode) LLX_B_L16) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L16, "LLX_B_L16") ;
   el_string_to_opcode_map.bind("LLX_B_L17", (Opcode) LLX_B_L17) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L17, "LLX_B_L17") ;
   el_string_to_opcode_map.bind("LLX_B_L18", (Opcode) LLX_B_L18) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L18, "LLX_B_L18") ;
   el_string_to_opcode_map.bind("LLX_B_L19", (Opcode) LLX_B_L19) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L19, "LLX_B_L19") ;
   el_string_to_opcode_map.bind("LLX_B_L20", (Opcode) LLX_B_L20) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L20, "LLX_B_L20") ;
   el_string_to_opcode_map.bind("LLX_B_L21", (Opcode) LLX_B_L21) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L21, "LLX_B_L21") ;
   el_string_to_opcode_map.bind("LLX_B_L22", (Opcode) LLX_B_L22) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L22, "LLX_B_L22") ;
   el_string_to_opcode_map.bind("LLX_B_L23", (Opcode) LLX_B_L23) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L23, "LLX_B_L23") ;
   el_string_to_opcode_map.bind("LLX_B_L24", (Opcode) LLX_B_L24) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L24, "LLX_B_L24") ;
   el_string_to_opcode_map.bind("LLX_B_L25", (Opcode) LLX_B_L25) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L25, "LLX_B_L25") ;
   el_string_to_opcode_map.bind("LLX_B_L26", (Opcode) LLX_B_L26) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L26, "LLX_B_L26") ;
   el_string_to_opcode_map.bind("LLX_B_L27", (Opcode) LLX_B_L27) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L27, "LLX_B_L27") ;
   el_string_to_opcode_map.bind("LLX_B_L28", (Opcode) LLX_B_L28) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L28, "LLX_B_L28") ;
   el_string_to_opcode_map.bind("LLX_B_L29", (Opcode) LLX_B_L29) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L29, "LLX_B_L29") ;
   el_string_to_opcode_map.bind("LLX_B_L30", (Opcode) LLX_B_L30) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L30, "LLX_B_L30") ;
   el_string_to_opcode_map.bind("LLX_B_L31", (Opcode) LLX_B_L31) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L31, "LLX_B_L31") ;
   el_string_to_opcode_map.bind("LLX_B_L32", (Opcode) LLX_B_L32) ;
   el_opcode_to_string_map.bind((Opcode) LLX_B_L32, "LLX_B_L32") ;

   el_string_to_opcode_map.bind("LLX_H_L1", (Opcode) LLX_H_L1) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L1, "LLX_H_L1") ;
   el_string_to_opcode_map.bind("LLX_H_L2", (Opcode) LLX_H_L2) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L2, "LLX_H_L2") ;
   el_string_to_opcode_map.bind("LLX_H_L3", (Opcode) LLX_H_L3) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L3, "LLX_H_L3") ;
   el_string_to_opcode_map.bind("LLX_H_L4", (Opcode) LLX_H_L4) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L4, "LLX_H_L4") ;
   el_string_to_opcode_map.bind("LLX_H_L5", (Opcode) LLX_H_L5) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L5, "LLX_H_L5") ;
   el_string_to_opcode_map.bind("LLX_H_L6", (Opcode) LLX_H_L6) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L6, "LLX_H_L6") ;
   el_string_to_opcode_map.bind("LLX_H_L7", (Opcode) LLX_H_L7) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L7, "LLX_H_L7") ;
   el_string_to_opcode_map.bind("LLX_H_L8", (Opcode) LLX_H_L8) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L8, "LLX_H_L8") ;
   el_string_to_opcode_map.bind("LLX_H_L9", (Opcode) LLX_H_L9) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L9, "LLX_H_L9") ;
   el_string_to_opcode_map.bind("LLX_H_L10", (Opcode) LLX_H_L10) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L10, "LLX_H_L10") ;
   el_string_to_opcode_map.bind("LLX_H_L11", (Opcode) LLX_H_L11) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L11, "LLX_H_L11") ;
   el_string_to_opcode_map.bind("LLX_H_L12", (Opcode) LLX_H_L12) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L12, "LLX_H_L12") ;
   el_string_to_opcode_map.bind("LLX_H_L13", (Opcode) LLX_H_L13) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L13, "LLX_H_L13") ;
   el_string_to_opcode_map.bind("LLX_H_L14", (Opcode) LLX_H_L14) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L14, "LLX_H_L14") ;
   el_string_to_opcode_map.bind("LLX_H_L15", (Opcode) LLX_H_L15) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L15, "LLX_H_L15") ;
   el_string_to_opcode_map.bind("LLX_H_L16", (Opcode) LLX_H_L16) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L16, "LLX_H_L16") ;
   el_string_to_opcode_map.bind("LLX_H_L17", (Opcode) LLX_H_L17) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L17, "LLX_H_L17") ;
   el_string_to_opcode_map.bind("LLX_H_L18", (Opcode) LLX_H_L18) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L18, "LLX_H_L18") ;
   el_string_to_opcode_map.bind("LLX_H_L19", (Opcode) LLX_H_L19) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L19, "LLX_H_L19") ;
   el_string_to_opcode_map.bind("LLX_H_L20", (Opcode) LLX_H_L20) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L20, "LLX_H_L20") ;
   el_string_to_opcode_map.bind("LLX_H_L21", (Opcode) LLX_H_L21) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L21, "LLX_H_L21") ;
   el_string_to_opcode_map.bind("LLX_H_L22", (Opcode) LLX_H_L22) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L22, "LLX_H_L22") ;
   el_string_to_opcode_map.bind("LLX_H_L23", (Opcode) LLX_H_L23) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L23, "LLX_H_L23") ;
   el_string_to_opcode_map.bind("LLX_H_L24", (Opcode) LLX_H_L24) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L24, "LLX_H_L24") ;
   el_string_to_opcode_map.bind("LLX_H_L25", (Opcode) LLX_H_L25) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L25, "LLX_H_L25") ;
   el_string_to_opcode_map.bind("LLX_H_L26", (Opcode) LLX_H_L26) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L26, "LLX_H_L26") ;
   el_string_to_opcode_map.bind("LLX_H_L27", (Opcode) LLX_H_L27) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L27, "LLX_H_L27") ;
   el_string_to_opcode_map.bind("LLX_H_L28", (Opcode) LLX_H_L28) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L28, "LLX_H_L28") ;
   el_string_to_opcode_map.bind("LLX_H_L29", (Opcode) LLX_H_L29) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L29, "LLX_H_L29") ;
   el_string_to_opcode_map.bind("LLX_H_L30", (Opcode) LLX_H_L30) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L30, "LLX_H_L30") ;
   el_string_to_opcode_map.bind("LLX_H_L31", (Opcode) LLX_H_L31) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L31, "LLX_H_L31") ;
   el_string_to_opcode_map.bind("LLX_H_L32", (Opcode) LLX_H_L32) ;
   el_opcode_to_string_map.bind((Opcode) LLX_H_L32, "LLX_H_L32") ;

   el_string_to_opcode_map.bind("LLX_W_L1", (Opcode) LLX_W_L1) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L1, "LLX_W_L1") ;
   el_string_to_opcode_map.bind("LLX_W_L2", (Opcode) LLX_W_L2) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L2, "LLX_W_L2") ;
   el_string_to_opcode_map.bind("LLX_W_L3", (Opcode) LLX_W_L3) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L3, "LLX_W_L3") ;
   el_string_to_opcode_map.bind("LLX_W_L4", (Opcode) LLX_W_L4) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L4, "LLX_W_L4") ;
   el_string_to_opcode_map.bind("LLX_W_L5", (Opcode) LLX_W_L5) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L5, "LLX_W_L5") ;
   el_string_to_opcode_map.bind("LLX_W_L6", (Opcode) LLX_W_L6) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L6, "LLX_W_L6") ;
   el_string_to_opcode_map.bind("LLX_W_L7", (Opcode) LLX_W_L7) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L7, "LLX_W_L7") ;
   el_string_to_opcode_map.bind("LLX_W_L8", (Opcode) LLX_W_L8) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L8, "LLX_W_L8") ;
   el_string_to_opcode_map.bind("LLX_W_L9", (Opcode) LLX_W_L9) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L9, "LLX_W_L9") ;
   el_string_to_opcode_map.bind("LLX_W_L10", (Opcode) LLX_W_L10) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L10, "LLX_W_L10") ;
   el_string_to_opcode_map.bind("LLX_W_L11", (Opcode) LLX_W_L11) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L11, "LLX_W_L11") ;
   el_string_to_opcode_map.bind("LLX_W_L12", (Opcode) LLX_W_L12) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L12, "LLX_W_L12") ;
   el_string_to_opcode_map.bind("LLX_W_L13", (Opcode) LLX_W_L13) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L13, "LLX_W_L13") ;
   el_string_to_opcode_map.bind("LLX_W_L14", (Opcode) LLX_W_L14) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L14, "LLX_W_L14") ;
   el_string_to_opcode_map.bind("LLX_W_L15", (Opcode) LLX_W_L15) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L15, "LLX_W_L15") ;
   el_string_to_opcode_map.bind("LLX_W_L16", (Opcode) LLX_W_L16) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L16, "LLX_W_L16") ;
   el_string_to_opcode_map.bind("LLX_W_L17", (Opcode) LLX_W_L17) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L17, "LLX_W_L17") ;
   el_string_to_opcode_map.bind("LLX_W_L18", (Opcode) LLX_W_L18) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L18, "LLX_W_L18") ;
   el_string_to_opcode_map.bind("LLX_W_L19", (Opcode) LLX_W_L19) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L19, "LLX_W_L19") ;
   el_string_to_opcode_map.bind("LLX_W_L20", (Opcode) LLX_W_L20) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L20, "LLX_W_L20") ;
   el_string_to_opcode_map.bind("LLX_W_L21", (Opcode) LLX_W_L21) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L21, "LLX_W_L21") ;
   el_string_to_opcode_map.bind("LLX_W_L22", (Opcode) LLX_W_L22) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L22, "LLX_W_L22") ;
   el_string_to_opcode_map.bind("LLX_W_L23", (Opcode) LLX_W_L23) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L23, "LLX_W_L23") ;
   el_string_to_opcode_map.bind("LLX_W_L24", (Opcode) LLX_W_L24) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L24, "LLX_W_L24") ;
   el_string_to_opcode_map.bind("LLX_W_L25", (Opcode) LLX_W_L25) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L25, "LLX_W_L25") ;
   el_string_to_opcode_map.bind("LLX_W_L26", (Opcode) LLX_W_L26) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L26, "LLX_W_L26") ;
   el_string_to_opcode_map.bind("LLX_W_L27", (Opcode) LLX_W_L27) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L27, "LLX_W_L27") ;
   el_string_to_opcode_map.bind("LLX_W_L28", (Opcode) LLX_W_L28) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L28, "LLX_W_L28") ;
   el_string_to_opcode_map.bind("LLX_W_L29", (Opcode) LLX_W_L29) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L29, "LLX_W_L29") ;
   el_string_to_opcode_map.bind("LLX_W_L30", (Opcode) LLX_W_L30) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L30, "LLX_W_L30") ;
   el_string_to_opcode_map.bind("LLX_W_L31", (Opcode) LLX_W_L31) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L31, "LLX_W_L31") ;
   el_string_to_opcode_map.bind("LLX_W_L32", (Opcode) LLX_W_L32) ;
   el_opcode_to_string_map.bind((Opcode) LLX_W_L32, "LLX_W_L32") ;

   el_string_to_opcode_map.bind("LLX_Q_L1", (Opcode) LLX_Q_L1) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L1, "LLX_Q_L1") ;
   el_string_to_opcode_map.bind("LLX_Q_L2", (Opcode) LLX_Q_L2) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L2, "LLX_Q_L2") ;
   el_string_to_opcode_map.bind("LLX_Q_L3", (Opcode) LLX_Q_L3) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L3, "LLX_Q_L3") ;
   el_string_to_opcode_map.bind("LLX_Q_L4", (Opcode) LLX_Q_L4) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L4, "LLX_Q_L4") ;
   el_string_to_opcode_map.bind("LLX_Q_L5", (Opcode) LLX_Q_L5) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L5, "LLX_Q_L5") ;
   el_string_to_opcode_map.bind("LLX_Q_L6", (Opcode) LLX_Q_L6) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L6, "LLX_Q_L6") ;
   el_string_to_opcode_map.bind("LLX_Q_L7", (Opcode) LLX_Q_L7) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L7, "LLX_Q_L7") ;
   el_string_to_opcode_map.bind("LLX_Q_L8", (Opcode) LLX_Q_L8) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L8, "LLX_Q_L8") ;
   el_string_to_opcode_map.bind("LLX_Q_L9", (Opcode) LLX_Q_L9) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L9, "LLX_Q_L9") ;
   el_string_to_opcode_map.bind("LLX_Q_L10", (Opcode) LLX_Q_L10) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L10, "LLX_Q_L10") ;
   el_string_to_opcode_map.bind("LLX_Q_L11", (Opcode) LLX_Q_L11) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L11, "LLX_Q_L11") ;
   el_string_to_opcode_map.bind("LLX_Q_L12", (Opcode) LLX_Q_L12) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L12, "LLX_Q_L12") ;
   el_string_to_opcode_map.bind("LLX_Q_L13", (Opcode) LLX_Q_L13) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L13, "LLX_Q_L13") ;
   el_string_to_opcode_map.bind("LLX_Q_L14", (Opcode) LLX_Q_L14) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L14, "LLX_Q_L14") ;
   el_string_to_opcode_map.bind("LLX_Q_L15", (Opcode) LLX_Q_L15) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L15, "LLX_Q_L15") ;
   el_string_to_opcode_map.bind("LLX_Q_L16", (Opcode) LLX_Q_L16) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L16, "LLX_Q_L16") ;
   el_string_to_opcode_map.bind("LLX_Q_L17", (Opcode) LLX_Q_L17) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L17, "LLX_Q_L17") ;
   el_string_to_opcode_map.bind("LLX_Q_L18", (Opcode) LLX_Q_L18) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L18, "LLX_Q_L18") ;
   el_string_to_opcode_map.bind("LLX_Q_L19", (Opcode) LLX_Q_L19) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L19, "LLX_Q_L19") ;
   el_string_to_opcode_map.bind("LLX_Q_L20", (Opcode) LLX_Q_L20) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L20, "LLX_Q_L20") ;
   el_string_to_opcode_map.bind("LLX_Q_L21", (Opcode) LLX_Q_L21) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L21, "LLX_Q_L21") ;
   el_string_to_opcode_map.bind("LLX_Q_L22", (Opcode) LLX_Q_L22) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L22, "LLX_Q_L22") ;
   el_string_to_opcode_map.bind("LLX_Q_L23", (Opcode) LLX_Q_L23) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L23, "LLX_Q_L23") ;
   el_string_to_opcode_map.bind("LLX_Q_L24", (Opcode) LLX_Q_L24) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L24, "LLX_Q_L24") ;
   el_string_to_opcode_map.bind("LLX_Q_L25", (Opcode) LLX_Q_L25) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L25, "LLX_Q_L25") ;
   el_string_to_opcode_map.bind("LLX_Q_L26", (Opcode) LLX_Q_L26) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L26, "LLX_Q_L26") ;
   el_string_to_opcode_map.bind("LLX_Q_L27", (Opcode) LLX_Q_L27) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L27, "LLX_Q_L27") ;
   el_string_to_opcode_map.bind("LLX_Q_L28", (Opcode) LLX_Q_L28) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L28, "LLX_Q_L28") ;
   el_string_to_opcode_map.bind("LLX_Q_L29", (Opcode) LLX_Q_L29) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L29, "LLX_Q_L29") ;
   el_string_to_opcode_map.bind("LLX_Q_L30", (Opcode) LLX_Q_L30) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L30, "LLX_Q_L30") ;
   el_string_to_opcode_map.bind("LLX_Q_L31", (Opcode) LLX_Q_L31) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L31, "LLX_Q_L31") ;
   el_string_to_opcode_map.bind("LLX_Q_L32", (Opcode) LLX_Q_L32) ;
   el_opcode_to_string_map.bind((Opcode) LLX_Q_L32, "LLX_Q_L32") ;
}

static void el_init_elcor_opcode_maps_lm_3()
{
   el_string_to_opcode_map.bind("FLL_S_L1", (Opcode) FLL_S_L1) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L1, "FLL_S_L1") ;
   el_string_to_opcode_map.bind("FLL_S_L2", (Opcode) FLL_S_L2) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L2, "FLL_S_L2") ;
   el_string_to_opcode_map.bind("FLL_S_L3", (Opcode) FLL_S_L3) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L3, "FLL_S_L3") ;
   el_string_to_opcode_map.bind("FLL_S_L4", (Opcode) FLL_S_L4) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L4, "FLL_S_L4") ;
   el_string_to_opcode_map.bind("FLL_S_L5", (Opcode) FLL_S_L5) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L5, "FLL_S_L5") ;
   el_string_to_opcode_map.bind("FLL_S_L6", (Opcode) FLL_S_L6) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L6, "FLL_S_L6") ;
   el_string_to_opcode_map.bind("FLL_S_L7", (Opcode) FLL_S_L7) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L7, "FLL_S_L7") ;
   el_string_to_opcode_map.bind("FLL_S_L8", (Opcode) FLL_S_L8) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L8, "FLL_S_L8") ;
   el_string_to_opcode_map.bind("FLL_S_L9", (Opcode) FLL_S_L9) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L9, "FLL_S_L9") ;
   el_string_to_opcode_map.bind("FLL_S_L10", (Opcode) FLL_S_L10) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L10, "FLL_S_L10") ;
   el_string_to_opcode_map.bind("FLL_S_L11", (Opcode) FLL_S_L11) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L11, "FLL_S_L11") ;
   el_string_to_opcode_map.bind("FLL_S_L12", (Opcode) FLL_S_L12) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L12, "FLL_S_L12") ;
   el_string_to_opcode_map.bind("FLL_S_L13", (Opcode) FLL_S_L13) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L13, "FLL_S_L13") ;
   el_string_to_opcode_map.bind("FLL_S_L14", (Opcode) FLL_S_L14) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L14, "FLL_S_L14") ;
   el_string_to_opcode_map.bind("FLL_S_L15", (Opcode) FLL_S_L15) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L15, "FLL_S_L15") ;
   el_string_to_opcode_map.bind("FLL_S_L16", (Opcode) FLL_S_L16) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L16, "FLL_S_L16") ;
   el_string_to_opcode_map.bind("FLL_S_L17", (Opcode) FLL_S_L17) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L17, "FLL_S_L17") ;
   el_string_to_opcode_map.bind("FLL_S_L18", (Opcode) FLL_S_L18) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L18, "FLL_S_L18") ;
   el_string_to_opcode_map.bind("FLL_S_L19", (Opcode) FLL_S_L19) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L19, "FLL_S_L19") ;
   el_string_to_opcode_map.bind("FLL_S_L20", (Opcode) FLL_S_L20) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L20, "FLL_S_L20") ;
   el_string_to_opcode_map.bind("FLL_S_L21", (Opcode) FLL_S_L21) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L21, "FLL_S_L21") ;
   el_string_to_opcode_map.bind("FLL_S_L22", (Opcode) FLL_S_L22) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L22, "FLL_S_L22") ;
   el_string_to_opcode_map.bind("FLL_S_L23", (Opcode) FLL_S_L23) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L23, "FLL_S_L23") ;
   el_string_to_opcode_map.bind("FLL_S_L24", (Opcode) FLL_S_L24) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L24, "FLL_S_L24") ;
   el_string_to_opcode_map.bind("FLL_S_L25", (Opcode) FLL_S_L25) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L25, "FLL_S_L25") ;
   el_string_to_opcode_map.bind("FLL_S_L26", (Opcode) FLL_S_L26) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L26, "FLL_S_L26") ;
   el_string_to_opcode_map.bind("FLL_S_L27", (Opcode) FLL_S_L27) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L27, "FLL_S_L27") ;
   el_string_to_opcode_map.bind("FLL_S_L28", (Opcode) FLL_S_L28) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L28, "FLL_S_L28") ;
   el_string_to_opcode_map.bind("FLL_S_L29", (Opcode) FLL_S_L29) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L29, "FLL_S_L29") ;
   el_string_to_opcode_map.bind("FLL_S_L30", (Opcode) FLL_S_L30) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L30, "FLL_S_L30") ;
   el_string_to_opcode_map.bind("FLL_S_L31", (Opcode) FLL_S_L31) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L31, "FLL_S_L31") ;
   el_string_to_opcode_map.bind("FLL_S_L32", (Opcode) FLL_S_L32) ;
   el_opcode_to_string_map.bind((Opcode) FLL_S_L32, "FLL_S_L32") ;

   el_string_to_opcode_map.bind("FLL_D_L1", (Opcode) FLL_D_L1) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L1, "FLL_D_L1") ;
   el_string_to_opcode_map.bind("FLL_D_L2", (Opcode) FLL_D_L2) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L2, "FLL_D_L2") ;
   el_string_to_opcode_map.bind("FLL_D_L3", (Opcode) FLL_D_L3) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L3, "FLL_D_L3") ;
   el_string_to_opcode_map.bind("FLL_D_L4", (Opcode) FLL_D_L4) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L4, "FLL_D_L4") ;
   el_string_to_opcode_map.bind("FLL_D_L5", (Opcode) FLL_D_L5) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L5, "FLL_D_L5") ;
   el_string_to_opcode_map.bind("FLL_D_L6", (Opcode) FLL_D_L6) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L6, "FLL_D_L6") ;
   el_string_to_opcode_map.bind("FLL_D_L7", (Opcode) FLL_D_L7) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L7, "FLL_D_L7") ;
   el_string_to_opcode_map.bind("FLL_D_L8", (Opcode) FLL_D_L8) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L8, "FLL_D_L8") ;
   el_string_to_opcode_map.bind("FLL_D_L9", (Opcode) FLL_D_L9) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L9, "FLL_D_L9") ;
   el_string_to_opcode_map.bind("FLL_D_L10", (Opcode) FLL_D_L10) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L10, "FLL_D_L10") ;
   el_string_to_opcode_map.bind("FLL_D_L11", (Opcode) FLL_D_L11) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L11, "FLL_D_L11") ;
   el_string_to_opcode_map.bind("FLL_D_L12", (Opcode) FLL_D_L12) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L12, "FLL_D_L12") ;
   el_string_to_opcode_map.bind("FLL_D_L13", (Opcode) FLL_D_L13) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L13, "FLL_D_L13") ;
   el_string_to_opcode_map.bind("FLL_D_L14", (Opcode) FLL_D_L14) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L14, "FLL_D_L14") ;
   el_string_to_opcode_map.bind("FLL_D_L15", (Opcode) FLL_D_L15) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L15, "FLL_D_L15") ;
   el_string_to_opcode_map.bind("FLL_D_L16", (Opcode) FLL_D_L16) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L16, "FLL_D_L16") ;
   el_string_to_opcode_map.bind("FLL_D_L17", (Opcode) FLL_D_L17) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L17, "FLL_D_L17") ;
   el_string_to_opcode_map.bind("FLL_D_L18", (Opcode) FLL_D_L18) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L18, "FLL_D_L18") ;
   el_string_to_opcode_map.bind("FLL_D_L19", (Opcode) FLL_D_L19) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L19, "FLL_D_L19") ;
   el_string_to_opcode_map.bind("FLL_D_L20", (Opcode) FLL_D_L20) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L20, "FLL_D_L20") ;
   el_string_to_opcode_map.bind("FLL_D_L21", (Opcode) FLL_D_L21) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L21, "FLL_D_L21") ;
   el_string_to_opcode_map.bind("FLL_D_L22", (Opcode) FLL_D_L22) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L22, "FLL_D_L22") ;
   el_string_to_opcode_map.bind("FLL_D_L23", (Opcode) FLL_D_L23) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L23, "FLL_D_L23") ;
   el_string_to_opcode_map.bind("FLL_D_L24", (Opcode) FLL_D_L24) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L24, "FLL_D_L24") ;
   el_string_to_opcode_map.bind("FLL_D_L25", (Opcode) FLL_D_L25) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L25, "FLL_D_L25") ;
   el_string_to_opcode_map.bind("FLL_D_L26", (Opcode) FLL_D_L26) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L26, "FLL_D_L26") ;
   el_string_to_opcode_map.bind("FLL_D_L27", (Opcode) FLL_D_L27) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L27, "FLL_D_L27") ;
   el_string_to_opcode_map.bind("FLL_D_L28", (Opcode) FLL_D_L28) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L28, "FLL_D_L28") ;
   el_string_to_opcode_map.bind("FLL_D_L29", (Opcode) FLL_D_L29) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L29, "FLL_D_L29") ;
   el_string_to_opcode_map.bind("FLL_D_L30", (Opcode) FLL_D_L30) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L30, "FLL_D_L30") ;
   el_string_to_opcode_map.bind("FLL_D_L31", (Opcode) FLL_D_L31) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L31, "FLL_D_L31") ;
   el_string_to_opcode_map.bind("FLL_D_L32", (Opcode) FLL_D_L32) ;
   el_opcode_to_string_map.bind((Opcode) FLL_D_L32, "FLL_D_L32") ;
}

static void el_init_elcor_opcode_maps_lm_4()
{
   el_string_to_opcode_map.bind("SL_B_L1", (Opcode) SL_B_L1) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L1, "SL_B_L1") ;
   el_string_to_opcode_map.bind("SL_B_L2", (Opcode) SL_B_L2) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L2, "SL_B_L2") ;
   el_string_to_opcode_map.bind("SL_B_L3", (Opcode) SL_B_L3) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L3, "SL_B_L3") ;
   el_string_to_opcode_map.bind("SL_B_L4", (Opcode) SL_B_L4) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L4, "SL_B_L4") ;
   el_string_to_opcode_map.bind("SL_B_L5", (Opcode) SL_B_L5) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L5, "SL_B_L5") ;
   el_string_to_opcode_map.bind("SL_B_L6", (Opcode) SL_B_L6) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L6, "SL_B_L6") ;
   el_string_to_opcode_map.bind("SL_B_L7", (Opcode) SL_B_L7) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L7, "SL_B_L7") ;
   el_string_to_opcode_map.bind("SL_B_L8", (Opcode) SL_B_L8) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L8, "SL_B_L8") ;
   el_string_to_opcode_map.bind("SL_B_L9", (Opcode) SL_B_L9) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L9, "SL_B_L9") ;
   el_string_to_opcode_map.bind("SL_B_L10", (Opcode) SL_B_L10) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L10, "SL_B_L10") ;
   el_string_to_opcode_map.bind("SL_B_L11", (Opcode) SL_B_L11) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L11, "SL_B_L11") ;
   el_string_to_opcode_map.bind("SL_B_L12", (Opcode) SL_B_L12) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L12, "SL_B_L12") ;
   el_string_to_opcode_map.bind("SL_B_L13", (Opcode) SL_B_L13) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L13, "SL_B_L13") ;
   el_string_to_opcode_map.bind("SL_B_L14", (Opcode) SL_B_L14) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L14, "SL_B_L14") ;
   el_string_to_opcode_map.bind("SL_B_L15", (Opcode) SL_B_L15) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L15, "SL_B_L15") ;
   el_string_to_opcode_map.bind("SL_B_L16", (Opcode) SL_B_L16) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L16, "SL_B_L16") ;
   el_string_to_opcode_map.bind("SL_B_L17", (Opcode) SL_B_L17) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L17, "SL_B_L17") ;
   el_string_to_opcode_map.bind("SL_B_L18", (Opcode) SL_B_L18) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L18, "SL_B_L18") ;
   el_string_to_opcode_map.bind("SL_B_L19", (Opcode) SL_B_L19) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L19, "SL_B_L19") ;
   el_string_to_opcode_map.bind("SL_B_L20", (Opcode) SL_B_L20) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L20, "SL_B_L20") ;
   el_string_to_opcode_map.bind("SL_B_L21", (Opcode) SL_B_L21) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L21, "SL_B_L21") ;
   el_string_to_opcode_map.bind("SL_B_L22", (Opcode) SL_B_L22) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L22, "SL_B_L22") ;
   el_string_to_opcode_map.bind("SL_B_L23", (Opcode) SL_B_L23) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L23, "SL_B_L23") ;
   el_string_to_opcode_map.bind("SL_B_L24", (Opcode) SL_B_L24) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L24, "SL_B_L24") ;
   el_string_to_opcode_map.bind("SL_B_L25", (Opcode) SL_B_L25) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L25, "SL_B_L25") ;
   el_string_to_opcode_map.bind("SL_B_L26", (Opcode) SL_B_L26) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L26, "SL_B_L26") ;
   el_string_to_opcode_map.bind("SL_B_L27", (Opcode) SL_B_L27) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L27, "SL_B_L27") ;
   el_string_to_opcode_map.bind("SL_B_L28", (Opcode) SL_B_L28) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L28, "SL_B_L28") ;
   el_string_to_opcode_map.bind("SL_B_L29", (Opcode) SL_B_L29) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L29, "SL_B_L29") ;
   el_string_to_opcode_map.bind("SL_B_L30", (Opcode) SL_B_L30) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L30, "SL_B_L30") ;
   el_string_to_opcode_map.bind("SL_B_L31", (Opcode) SL_B_L31) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L31, "SL_B_L31") ;
   el_string_to_opcode_map.bind("SL_B_L32", (Opcode) SL_B_L32) ;
   el_opcode_to_string_map.bind((Opcode) SL_B_L32, "SL_B_L32") ;

   el_string_to_opcode_map.bind("SL_H_L1", (Opcode) SL_H_L1) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L1, "SL_H_L1") ;
   el_string_to_opcode_map.bind("SL_H_L2", (Opcode) SL_H_L2) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L2, "SL_H_L2") ;
   el_string_to_opcode_map.bind("SL_H_L3", (Opcode) SL_H_L3) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L3, "SL_H_L3") ;
   el_string_to_opcode_map.bind("SL_H_L4", (Opcode) SL_H_L4) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L4, "SL_H_L4") ;
   el_string_to_opcode_map.bind("SL_H_L5", (Opcode) SL_H_L5) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L5, "SL_H_L5") ;
   el_string_to_opcode_map.bind("SL_H_L6", (Opcode) SL_H_L6) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L6, "SL_H_L6") ;
   el_string_to_opcode_map.bind("SL_H_L7", (Opcode) SL_H_L7) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L7, "SL_H_L7") ;
   el_string_to_opcode_map.bind("SL_H_L8", (Opcode) SL_H_L8) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L8, "SL_H_L8") ;
   el_string_to_opcode_map.bind("SL_H_L9", (Opcode) SL_H_L9) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L9, "SL_H_L9") ;
   el_string_to_opcode_map.bind("SL_H_L10", (Opcode) SL_H_L10) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L10, "SL_H_L10") ;
   el_string_to_opcode_map.bind("SL_H_L11", (Opcode) SL_H_L11) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L11, "SL_H_L11") ;
   el_string_to_opcode_map.bind("SL_H_L12", (Opcode) SL_H_L12) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L12, "SL_H_L12") ;
   el_string_to_opcode_map.bind("SL_H_L13", (Opcode) SL_H_L13) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L13, "SL_H_L13") ;
   el_string_to_opcode_map.bind("SL_H_L14", (Opcode) SL_H_L14) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L14, "SL_H_L14") ;
   el_string_to_opcode_map.bind("SL_H_L15", (Opcode) SL_H_L15) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L15, "SL_H_L15") ;
   el_string_to_opcode_map.bind("SL_H_L16", (Opcode) SL_H_L16) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L16, "SL_H_L16") ;
   el_string_to_opcode_map.bind("SL_H_L17", (Opcode) SL_H_L17) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L17, "SL_H_L17") ;
   el_string_to_opcode_map.bind("SL_H_L18", (Opcode) SL_H_L18) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L18, "SL_H_L18") ;
   el_string_to_opcode_map.bind("SL_H_L19", (Opcode) SL_H_L19) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L19, "SL_H_L19") ;
   el_string_to_opcode_map.bind("SL_H_L20", (Opcode) SL_H_L20) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L20, "SL_H_L20") ;
   el_string_to_opcode_map.bind("SL_H_L21", (Opcode) SL_H_L21) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L21, "SL_H_L21") ;
   el_string_to_opcode_map.bind("SL_H_L22", (Opcode) SL_H_L22) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L22, "SL_H_L22") ;
   el_string_to_opcode_map.bind("SL_H_L23", (Opcode) SL_H_L23) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L23, "SL_H_L23") ;
   el_string_to_opcode_map.bind("SL_H_L24", (Opcode) SL_H_L24) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L24, "SL_H_L24") ;
   el_string_to_opcode_map.bind("SL_H_L25", (Opcode) SL_H_L25) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L25, "SL_H_L25") ;
   el_string_to_opcode_map.bind("SL_H_L26", (Opcode) SL_H_L26) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L26, "SL_H_L26") ;
   el_string_to_opcode_map.bind("SL_H_L27", (Opcode) SL_H_L27) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L27, "SL_H_L27") ;
   el_string_to_opcode_map.bind("SL_H_L28", (Opcode) SL_H_L28) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L28, "SL_H_L28") ;
   el_string_to_opcode_map.bind("SL_H_L29", (Opcode) SL_H_L29) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L29, "SL_H_L29") ;
   el_string_to_opcode_map.bind("SL_H_L30", (Opcode) SL_H_L30) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L30, "SL_H_L30") ;
   el_string_to_opcode_map.bind("SL_H_L31", (Opcode) SL_H_L31) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L31, "SL_H_L31") ;
   el_string_to_opcode_map.bind("SL_H_L32", (Opcode) SL_H_L32) ;
   el_opcode_to_string_map.bind((Opcode) SL_H_L32, "SL_H_L32") ;

   el_string_to_opcode_map.bind("SL_W_L1", (Opcode) SL_W_L1) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L1, "SL_W_L1") ;
   el_string_to_opcode_map.bind("SL_W_L2", (Opcode) SL_W_L2) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L2, "SL_W_L2") ;
   el_string_to_opcode_map.bind("SL_W_L3", (Opcode) SL_W_L3) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L3, "SL_W_L3") ;
   el_string_to_opcode_map.bind("SL_W_L4", (Opcode) SL_W_L4) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L4, "SL_W_L4") ;
   el_string_to_opcode_map.bind("SL_W_L5", (Opcode) SL_W_L5) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L5, "SL_W_L5") ;
   el_string_to_opcode_map.bind("SL_W_L6", (Opcode) SL_W_L6) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L6, "SL_W_L6") ;
   el_string_to_opcode_map.bind("SL_W_L7", (Opcode) SL_W_L7) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L7, "SL_W_L7") ;
   el_string_to_opcode_map.bind("SL_W_L8", (Opcode) SL_W_L8) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L8, "SL_W_L8") ;
   el_string_to_opcode_map.bind("SL_W_L9", (Opcode) SL_W_L9) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L9, "SL_W_L9") ;
   el_string_to_opcode_map.bind("SL_W_L10", (Opcode) SL_W_L10) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L10, "SL_W_L10") ;
   el_string_to_opcode_map.bind("SL_W_L11", (Opcode) SL_W_L11) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L11, "SL_W_L11") ;
   el_string_to_opcode_map.bind("SL_W_L12", (Opcode) SL_W_L12) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L12, "SL_W_L12") ;
   el_string_to_opcode_map.bind("SL_W_L13", (Opcode) SL_W_L13) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L13, "SL_W_L13") ;
   el_string_to_opcode_map.bind("SL_W_L14", (Opcode) SL_W_L14) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L14, "SL_W_L14") ;
   el_string_to_opcode_map.bind("SL_W_L15", (Opcode) SL_W_L15) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L15, "SL_W_L15") ;
   el_string_to_opcode_map.bind("SL_W_L16", (Opcode) SL_W_L16) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L16, "SL_W_L16") ;
   el_string_to_opcode_map.bind("SL_W_L17", (Opcode) SL_W_L17) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L17, "SL_W_L17") ;
   el_string_to_opcode_map.bind("SL_W_L18", (Opcode) SL_W_L18) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L18, "SL_W_L18") ;
   el_string_to_opcode_map.bind("SL_W_L19", (Opcode) SL_W_L19) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L19, "SL_W_L19") ;
   el_string_to_opcode_map.bind("SL_W_L20", (Opcode) SL_W_L20) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L20, "SL_W_L20") ;
   el_string_to_opcode_map.bind("SL_W_L21", (Opcode) SL_W_L21) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L21, "SL_W_L21") ;
   el_string_to_opcode_map.bind("SL_W_L22", (Opcode) SL_W_L22) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L22, "SL_W_L22") ;
   el_string_to_opcode_map.bind("SL_W_L23", (Opcode) SL_W_L23) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L23, "SL_W_L23") ;
   el_string_to_opcode_map.bind("SL_W_L24", (Opcode) SL_W_L24) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L24, "SL_W_L24") ;
   el_string_to_opcode_map.bind("SL_W_L25", (Opcode) SL_W_L25) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L25, "SL_W_L25") ;
   el_string_to_opcode_map.bind("SL_W_L26", (Opcode) SL_W_L26) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L26, "SL_W_L26") ;
   el_string_to_opcode_map.bind("SL_W_L27", (Opcode) SL_W_L27) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L27, "SL_W_L27") ;
   el_string_to_opcode_map.bind("SL_W_L28", (Opcode) SL_W_L28) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L28, "SL_W_L28") ;
   el_string_to_opcode_map.bind("SL_W_L29", (Opcode) SL_W_L29) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L29, "SL_W_L29") ;
   el_string_to_opcode_map.bind("SL_W_L30", (Opcode) SL_W_L30) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L30, "SL_W_L30") ;
   el_string_to_opcode_map.bind("SL_W_L31", (Opcode) SL_W_L31) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L31, "SL_W_L31") ;
   el_string_to_opcode_map.bind("SL_W_L32", (Opcode) SL_W_L32) ;
   el_opcode_to_string_map.bind((Opcode) SL_W_L32, "SL_W_L32") ;

   el_string_to_opcode_map.bind("SL_Q_L1", (Opcode) SL_Q_L1) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L1, "SL_Q_L1") ;
   el_string_to_opcode_map.bind("SL_Q_L2", (Opcode) SL_Q_L2) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L2, "SL_Q_L2") ;
   el_string_to_opcode_map.bind("SL_Q_L3", (Opcode) SL_Q_L3) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L3, "SL_Q_L3") ;
   el_string_to_opcode_map.bind("SL_Q_L4", (Opcode) SL_Q_L4) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L4, "SL_Q_L4") ;
   el_string_to_opcode_map.bind("SL_Q_L5", (Opcode) SL_Q_L5) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L5, "SL_Q_L5") ;
   el_string_to_opcode_map.bind("SL_Q_L6", (Opcode) SL_Q_L6) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L6, "SL_Q_L6") ;
   el_string_to_opcode_map.bind("SL_Q_L7", (Opcode) SL_Q_L7) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L7, "SL_Q_L7") ;
   el_string_to_opcode_map.bind("SL_Q_L8", (Opcode) SL_Q_L8) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L8, "SL_Q_L8") ;
   el_string_to_opcode_map.bind("SL_Q_L9", (Opcode) SL_Q_L9) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L9, "SL_Q_L9") ;
   el_string_to_opcode_map.bind("SL_Q_L10", (Opcode) SL_Q_L10) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L10, "SL_Q_L10") ;
   el_string_to_opcode_map.bind("SL_Q_L11", (Opcode) SL_Q_L11) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L11, "SL_Q_L11") ;
   el_string_to_opcode_map.bind("SL_Q_L12", (Opcode) SL_Q_L12) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L12, "SL_Q_L12") ;
   el_string_to_opcode_map.bind("SL_Q_L13", (Opcode) SL_Q_L13) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L13, "SL_Q_L13") ;
   el_string_to_opcode_map.bind("SL_Q_L14", (Opcode) SL_Q_L14) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L14, "SL_Q_L14") ;
   el_string_to_opcode_map.bind("SL_Q_L15", (Opcode) SL_Q_L15) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L15, "SL_Q_L15") ;
   el_string_to_opcode_map.bind("SL_Q_L16", (Opcode) SL_Q_L16) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L16, "SL_Q_L16") ;
   el_string_to_opcode_map.bind("SL_Q_L17", (Opcode) SL_Q_L17) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L17, "SL_Q_L17") ;
   el_string_to_opcode_map.bind("SL_Q_L18", (Opcode) SL_Q_L18) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L18, "SL_Q_L18") ;
   el_string_to_opcode_map.bind("SL_Q_L19", (Opcode) SL_Q_L19) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L19, "SL_Q_L19") ;
   el_string_to_opcode_map.bind("SL_Q_L20", (Opcode) SL_Q_L20) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L20, "SL_Q_L20") ;
   el_string_to_opcode_map.bind("SL_Q_L21", (Opcode) SL_Q_L21) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L21, "SL_Q_L21") ;
   el_string_to_opcode_map.bind("SL_Q_L22", (Opcode) SL_Q_L22) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L22, "SL_Q_L22") ;
   el_string_to_opcode_map.bind("SL_Q_L23", (Opcode) SL_Q_L23) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L23, "SL_Q_L23") ;
   el_string_to_opcode_map.bind("SL_Q_L24", (Opcode) SL_Q_L24) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L24, "SL_Q_L24") ;
   el_string_to_opcode_map.bind("SL_Q_L25", (Opcode) SL_Q_L25) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L25, "SL_Q_L25") ;
   el_string_to_opcode_map.bind("SL_Q_L26", (Opcode) SL_Q_L26) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L26, "SL_Q_L26") ;
   el_string_to_opcode_map.bind("SL_Q_L27", (Opcode) SL_Q_L27) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L27, "SL_Q_L27") ;
   el_string_to_opcode_map.bind("SL_Q_L28", (Opcode) SL_Q_L28) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L28, "SL_Q_L28") ;
   el_string_to_opcode_map.bind("SL_Q_L29", (Opcode) SL_Q_L29) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L29, "SL_Q_L29") ;
   el_string_to_opcode_map.bind("SL_Q_L30", (Opcode) SL_Q_L30) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L30, "SL_Q_L30") ;
   el_string_to_opcode_map.bind("SL_Q_L31", (Opcode) SL_Q_L31) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L31, "SL_Q_L31") ;
   el_string_to_opcode_map.bind("SL_Q_L32", (Opcode) SL_Q_L32) ;
   el_opcode_to_string_map.bind((Opcode) SL_Q_L32, "SL_Q_L32") ;
}

static void el_init_elcor_opcode_maps_lm_5()
{
   el_string_to_opcode_map.bind("FSL_S_L1", (Opcode) FSL_S_L1) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L1, "FSL_S_L1") ;
   el_string_to_opcode_map.bind("FSL_S_L2", (Opcode) FSL_S_L2) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L2, "FSL_S_L2") ;
   el_string_to_opcode_map.bind("FSL_S_L3", (Opcode) FSL_S_L3) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L3, "FSL_S_L3") ;
   el_string_to_opcode_map.bind("FSL_S_L4", (Opcode) FSL_S_L4) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L4, "FSL_S_L4") ;
   el_string_to_opcode_map.bind("FSL_S_L5", (Opcode) FSL_S_L5) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L5, "FSL_S_L5") ;
   el_string_to_opcode_map.bind("FSL_S_L6", (Opcode) FSL_S_L6) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L6, "FSL_S_L6") ;
   el_string_to_opcode_map.bind("FSL_S_L7", (Opcode) FSL_S_L7) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L7, "FSL_S_L7") ;
   el_string_to_opcode_map.bind("FSL_S_L8", (Opcode) FSL_S_L8) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L8, "FSL_S_L8") ;
   el_string_to_opcode_map.bind("FSL_S_L9", (Opcode) FSL_S_L9) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L9, "FSL_S_L9") ;
   el_string_to_opcode_map.bind("FSL_S_L10", (Opcode) FSL_S_L10) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L10, "FSL_S_L10") ;
   el_string_to_opcode_map.bind("FSL_S_L11", (Opcode) FSL_S_L11) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L11, "FSL_S_L11") ;
   el_string_to_opcode_map.bind("FSL_S_L12", (Opcode) FSL_S_L12) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L12, "FSL_S_L12") ;
   el_string_to_opcode_map.bind("FSL_S_L13", (Opcode) FSL_S_L13) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L13, "FSL_S_L13") ;
   el_string_to_opcode_map.bind("FSL_S_L14", (Opcode) FSL_S_L14) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L14, "FSL_S_L14") ;
   el_string_to_opcode_map.bind("FSL_S_L15", (Opcode) FSL_S_L15) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L15, "FSL_S_L15") ;
   el_string_to_opcode_map.bind("FSL_S_L16", (Opcode) FSL_S_L16) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L16, "FSL_S_L16") ;
   el_string_to_opcode_map.bind("FSL_S_L17", (Opcode) FSL_S_L17) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L17, "FSL_S_L17") ;
   el_string_to_opcode_map.bind("FSL_S_L18", (Opcode) FSL_S_L18) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L18, "FSL_S_L18") ;
   el_string_to_opcode_map.bind("FSL_S_L19", (Opcode) FSL_S_L19) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L19, "FSL_S_L19") ;
   el_string_to_opcode_map.bind("FSL_S_L20", (Opcode) FSL_S_L20) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L20, "FSL_S_L20") ;
   el_string_to_opcode_map.bind("FSL_S_L21", (Opcode) FSL_S_L21) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L21, "FSL_S_L21") ;
   el_string_to_opcode_map.bind("FSL_S_L22", (Opcode) FSL_S_L22) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L22, "FSL_S_L22") ;
   el_string_to_opcode_map.bind("FSL_S_L23", (Opcode) FSL_S_L23) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L23, "FSL_S_L23") ;
   el_string_to_opcode_map.bind("FSL_S_L24", (Opcode) FSL_S_L24) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L24, "FSL_S_L24") ;
   el_string_to_opcode_map.bind("FSL_S_L25", (Opcode) FSL_S_L25) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L25, "FSL_S_L25") ;
   el_string_to_opcode_map.bind("FSL_S_L26", (Opcode) FSL_S_L26) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L26, "FSL_S_L26") ;
   el_string_to_opcode_map.bind("FSL_S_L27", (Opcode) FSL_S_L27) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L27, "FSL_S_L27") ;
   el_string_to_opcode_map.bind("FSL_S_L28", (Opcode) FSL_S_L28) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L28, "FSL_S_L28") ;
   el_string_to_opcode_map.bind("FSL_S_L29", (Opcode) FSL_S_L29) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L29, "FSL_S_L29") ;
   el_string_to_opcode_map.bind("FSL_S_L30", (Opcode) FSL_S_L30) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L30, "FSL_S_L30") ;
   el_string_to_opcode_map.bind("FSL_S_L31", (Opcode) FSL_S_L31) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L31, "FSL_S_L31") ;
   el_string_to_opcode_map.bind("FSL_S_L32", (Opcode) FSL_S_L32) ;
   el_opcode_to_string_map.bind((Opcode) FSL_S_L32, "FSL_S_L32") ;

   el_string_to_opcode_map.bind("FSL_D_L1", (Opcode) FSL_D_L1) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L1, "FSL_D_L1") ;
   el_string_to_opcode_map.bind("FSL_D_L2", (Opcode) FSL_D_L2) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L2, "FSL_D_L2") ;
   el_string_to_opcode_map.bind("FSL_D_L3", (Opcode) FSL_D_L3) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L3, "FSL_D_L3") ;
   el_string_to_opcode_map.bind("FSL_D_L4", (Opcode) FSL_D_L4) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L4, "FSL_D_L4") ;
   el_string_to_opcode_map.bind("FSL_D_L5", (Opcode) FSL_D_L5) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L5, "FSL_D_L5") ;
   el_string_to_opcode_map.bind("FSL_D_L6", (Opcode) FSL_D_L6) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L6, "FSL_D_L6") ;
   el_string_to_opcode_map.bind("FSL_D_L7", (Opcode) FSL_D_L7) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L7, "FSL_D_L7") ;
   el_string_to_opcode_map.bind("FSL_D_L8", (Opcode) FSL_D_L8) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L8, "FSL_D_L8") ;
   el_string_to_opcode_map.bind("FSL_D_L9", (Opcode) FSL_D_L9) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L9, "FSL_D_L9") ;
   el_string_to_opcode_map.bind("FSL_D_L10", (Opcode) FSL_D_L10) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L10, "FSL_D_L10") ;
   el_string_to_opcode_map.bind("FSL_D_L11", (Opcode) FSL_D_L11) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L11, "FSL_D_L11") ;
   el_string_to_opcode_map.bind("FSL_D_L12", (Opcode) FSL_D_L12) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L12, "FSL_D_L12") ;
   el_string_to_opcode_map.bind("FSL_D_L13", (Opcode) FSL_D_L13) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L13, "FSL_D_L13") ;
   el_string_to_opcode_map.bind("FSL_D_L14", (Opcode) FSL_D_L14) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L14, "FSL_D_L14") ;
   el_string_to_opcode_map.bind("FSL_D_L15", (Opcode) FSL_D_L15) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L15, "FSL_D_L15") ;
   el_string_to_opcode_map.bind("FSL_D_L16", (Opcode) FSL_D_L16) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L16, "FSL_D_L16") ;
   el_string_to_opcode_map.bind("FSL_D_L17", (Opcode) FSL_D_L17) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L17, "FSL_D_L17") ;
   el_string_to_opcode_map.bind("FSL_D_L18", (Opcode) FSL_D_L18) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L18, "FSL_D_L18") ;
   el_string_to_opcode_map.bind("FSL_D_L19", (Opcode) FSL_D_L19) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L19, "FSL_D_L19") ;
   el_string_to_opcode_map.bind("FSL_D_L20", (Opcode) FSL_D_L20) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L20, "FSL_D_L20") ;
   el_string_to_opcode_map.bind("FSL_D_L21", (Opcode) FSL_D_L21) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L21, "FSL_D_L21") ;
   el_string_to_opcode_map.bind("FSL_D_L22", (Opcode) FSL_D_L22) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L22, "FSL_D_L22") ;
   el_string_to_opcode_map.bind("FSL_D_L23", (Opcode) FSL_D_L23) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L23, "FSL_D_L23") ;
   el_string_to_opcode_map.bind("FSL_D_L24", (Opcode) FSL_D_L24) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L24, "FSL_D_L24") ;
   el_string_to_opcode_map.bind("FSL_D_L25", (Opcode) FSL_D_L25) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L25, "FSL_D_L25") ;
   el_string_to_opcode_map.bind("FSL_D_L26", (Opcode) FSL_D_L26) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L26, "FSL_D_L26") ;
   el_string_to_opcode_map.bind("FSL_D_L27", (Opcode) FSL_D_L27) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L27, "FSL_D_L27") ;
   el_string_to_opcode_map.bind("FSL_D_L28", (Opcode) FSL_D_L28) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L28, "FSL_D_L28") ;
   el_string_to_opcode_map.bind("FSL_D_L29", (Opcode) FSL_D_L29) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L29, "FSL_D_L29") ;
   el_string_to_opcode_map.bind("FSL_D_L30", (Opcode) FSL_D_L30) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L30, "FSL_D_L30") ;
   el_string_to_opcode_map.bind("FSL_D_L31", (Opcode) FSL_D_L31) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L31, "FSL_D_L31") ;
   el_string_to_opcode_map.bind("FSL_D_L32", (Opcode) FSL_D_L32) ;
   el_opcode_to_string_map.bind((Opcode) FSL_D_L32, "FSL_D_L32") ;
}

void el_init_elcor_opcode_maps_lm()
{
    el_init_elcor_opcode_maps_lm_1();
    el_init_elcor_opcode_maps_lm_2();
    el_init_elcor_opcode_maps_lm_3();
    el_init_elcor_opcode_maps_lm_4();
    el_init_elcor_opcode_maps_lm_5();
}
