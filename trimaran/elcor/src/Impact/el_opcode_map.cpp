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
//      File:           el_opcode_map.cpp                                     
// 	Author:         Scott A. Mahlke                                       
//      Created:        May 1995                                              
//      Description:    This file contains 2 maps used for translating        
//                      back and forth between Lcode and Elcor IRs.           
//                      a) map Elcor opcodes to corresponding opcodes in Lcode
//	                b) map Lcode opcodes to Elcor opcodes
//
/////////////////////////////////////////////////////////////////////////////

#include "el_main.h"
#include "opcode_load_store.h"
#include "opcode_local_mem.h"
#include "hash_functions.h"

Hash_map<int,Opcode> el_lcode_to_elcor_opcode_map(hash_int, 2048);
Hash_map<Opcode,int> el_elcor_to_lcode_opcode_map(hash_opcode, 2048);

//
//	Load / Store ops
//
static void el_init_lcode_opcode_maps_load_store()
{

//
//	Standard GPR load operations
//
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_V1_V1, (Opcode)L_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_V1_V1,PLAYDOHop_L_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_V1_C1, (Opcode)L_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_V1_C1,PLAYDOHop_L_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_V1_C2, (Opcode)L_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_V1_C2,PLAYDOHop_L_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_V1_C3, (Opcode)L_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_V1_C3,PLAYDOHop_L_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C1_V1, (Opcode)L_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C1_V1,PLAYDOHop_L_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C1_C1, (Opcode)L_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C1_C1,PLAYDOHop_L_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C1_C2, (Opcode)L_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C1_C2,PLAYDOHop_L_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C1_C3, (Opcode)L_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C1_C3,PLAYDOHop_L_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C2_V1, (Opcode)L_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C2_V1,PLAYDOHop_L_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C2_C1, (Opcode)L_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C2_C1,PLAYDOHop_L_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C2_C2, (Opcode)L_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C2_C2,PLAYDOHop_L_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C2_C3, (Opcode)L_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C2_C3,PLAYDOHop_L_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C3_V1, (Opcode)L_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C3_V1,PLAYDOHop_L_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C3_C1, (Opcode)L_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C3_C1,PLAYDOHop_L_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C3_C2, (Opcode)L_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C3_C2,PLAYDOHop_L_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_B_C3_C3, (Opcode)L_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_B_C3_C3,PLAYDOHop_L_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_V1_V1, (Opcode)L_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_V1_V1,PLAYDOHop_L_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_V1_C1, (Opcode)L_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_V1_C1,PLAYDOHop_L_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_V1_C2, (Opcode)L_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_V1_C2,PLAYDOHop_L_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_V1_C3, (Opcode)L_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_V1_C3,PLAYDOHop_L_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C1_V1, (Opcode)L_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C1_V1,PLAYDOHop_L_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C1_C1, (Opcode)L_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C1_C1,PLAYDOHop_L_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C1_C2, (Opcode)L_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C1_C2,PLAYDOHop_L_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C1_C3, (Opcode)L_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C1_C3,PLAYDOHop_L_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C2_V1, (Opcode)L_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C2_V1,PLAYDOHop_L_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C2_C1, (Opcode)L_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C2_C1,PLAYDOHop_L_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C2_C2, (Opcode)L_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C2_C2,PLAYDOHop_L_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C2_C3, (Opcode)L_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C2_C3,PLAYDOHop_L_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C3_V1, (Opcode)L_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C3_V1,PLAYDOHop_L_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C3_C1, (Opcode)L_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C3_C1,PLAYDOHop_L_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C3_C2, (Opcode)L_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C3_C2,PLAYDOHop_L_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_H_C3_C3, (Opcode)L_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_H_C3_C3,PLAYDOHop_L_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_V1_V1, (Opcode)L_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_V1_V1,PLAYDOHop_L_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_V1_C1, (Opcode)L_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_V1_C1,PLAYDOHop_L_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_V1_C2, (Opcode)L_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_V1_C2,PLAYDOHop_L_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_V1_C3, (Opcode)L_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_V1_C3,PLAYDOHop_L_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C1_V1, (Opcode)L_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C1_V1,PLAYDOHop_L_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C1_C1, (Opcode)L_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C1_C1,PLAYDOHop_L_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C1_C2, (Opcode)L_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C1_C2,PLAYDOHop_L_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C1_C3, (Opcode)L_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C1_C3,PLAYDOHop_L_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C2_V1, (Opcode)L_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C2_V1,PLAYDOHop_L_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C2_C1, (Opcode)L_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C2_C1,PLAYDOHop_L_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C2_C2, (Opcode)L_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C2_C2,PLAYDOHop_L_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C2_C3, (Opcode)L_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C2_C3,PLAYDOHop_L_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C3_V1, (Opcode)L_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C3_V1,PLAYDOHop_L_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C3_C1, (Opcode)L_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C3_C1,PLAYDOHop_L_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C3_C2, (Opcode)L_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C3_C2,PLAYDOHop_L_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_W_C3_C3, (Opcode)L_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_W_C3_C3,PLAYDOHop_L_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_V1_V1, (Opcode)L_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_V1_V1,PLAYDOHop_L_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_V1_C1, (Opcode)L_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_V1_C1,PLAYDOHop_L_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_V1_C2, (Opcode)L_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_V1_C2,PLAYDOHop_L_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_V1_C3, (Opcode)L_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_V1_C3,PLAYDOHop_L_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C1_V1, (Opcode)L_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C1_V1,PLAYDOHop_L_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C1_C1, (Opcode)L_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C1_C1,PLAYDOHop_L_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C1_C2, (Opcode)L_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C1_C2,PLAYDOHop_L_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C1_C3, (Opcode)L_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C1_C3,PLAYDOHop_L_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C2_V1, (Opcode)L_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C2_V1,PLAYDOHop_L_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C2_C1, (Opcode)L_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C2_C1,PLAYDOHop_L_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C2_C2, (Opcode)L_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C2_C2,PLAYDOHop_L_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C2_C3, (Opcode)L_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C2_C3,PLAYDOHop_L_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C3_V1, (Opcode)L_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C3_V1,PLAYDOHop_L_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C3_C1, (Opcode)L_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C3_C1,PLAYDOHop_L_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C3_C2, (Opcode)L_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C3_C2,PLAYDOHop_L_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_L_Q_C3_C3, (Opcode)L_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) L_Q_C3_C3,PLAYDOHop_L_Q_C3_C3) ;
#endif
   
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_V1_V1, (Opcode)LX_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_V1_V1,PLAYDOHop_LX_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_V1_C1, (Opcode)LX_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_V1_C1,PLAYDOHop_LX_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_V1_C2, (Opcode)LX_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_V1_C2,PLAYDOHop_LX_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_V1_C3, (Opcode)LX_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_V1_C3,PLAYDOHop_LX_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C1_V1, (Opcode)LX_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C1_V1,PLAYDOHop_LX_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C1_C1, (Opcode)LX_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C1_C1,PLAYDOHop_LX_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C1_C2, (Opcode)LX_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C1_C2,PLAYDOHop_LX_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C1_C3, (Opcode)LX_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C1_C3,PLAYDOHop_LX_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C2_V1, (Opcode)LX_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C2_V1,PLAYDOHop_LX_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C2_C1, (Opcode)LX_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C2_C1,PLAYDOHop_LX_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C2_C2, (Opcode)LX_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C2_C2,PLAYDOHop_LX_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C2_C3, (Opcode)LX_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C2_C3,PLAYDOHop_LX_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C3_V1, (Opcode)LX_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C3_V1,PLAYDOHop_LX_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C3_C1, (Opcode)LX_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C3_C1,PLAYDOHop_LX_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C3_C2, (Opcode)LX_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C3_C2,PLAYDOHop_LX_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_B_C3_C3, (Opcode)LX_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_B_C3_C3,PLAYDOHop_LX_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_V1_V1, (Opcode)LX_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_V1_V1,PLAYDOHop_LX_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_V1_C1, (Opcode)LX_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_V1_C1,PLAYDOHop_LX_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_V1_C2, (Opcode)LX_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_V1_C2,PLAYDOHop_LX_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_V1_C3, (Opcode)LX_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_V1_C3,PLAYDOHop_LX_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C1_V1, (Opcode)LX_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C1_V1,PLAYDOHop_LX_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C1_C1, (Opcode)LX_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C1_C1,PLAYDOHop_LX_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C1_C2, (Opcode)LX_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C1_C2,PLAYDOHop_LX_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C1_C3, (Opcode)LX_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C1_C3,PLAYDOHop_LX_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C2_V1, (Opcode)LX_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C2_V1,PLAYDOHop_LX_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C2_C1, (Opcode)LX_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C2_C1,PLAYDOHop_LX_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C2_C2, (Opcode)LX_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C2_C2,PLAYDOHop_LX_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C2_C3, (Opcode)LX_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C2_C3,PLAYDOHop_LX_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C3_V1, (Opcode)LX_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C3_V1,PLAYDOHop_LX_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C3_C1, (Opcode)LX_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C3_C1,PLAYDOHop_LX_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C3_C2, (Opcode)LX_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C3_C2,PLAYDOHop_LX_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_H_C3_C3, (Opcode)LX_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_H_C3_C3,PLAYDOHop_LX_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_V1_V1, (Opcode)LX_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_V1_V1,PLAYDOHop_LX_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_V1_C1, (Opcode)LX_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_V1_C1,PLAYDOHop_LX_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_V1_C2, (Opcode)LX_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_V1_C2,PLAYDOHop_LX_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_V1_C3, (Opcode)LX_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_V1_C3,PLAYDOHop_LX_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C1_V1, (Opcode)LX_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C1_V1,PLAYDOHop_LX_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C1_C1, (Opcode)LX_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C1_C1,PLAYDOHop_LX_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C1_C2, (Opcode)LX_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C1_C2,PLAYDOHop_LX_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C1_C3, (Opcode)LX_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C1_C3,PLAYDOHop_LX_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C2_V1, (Opcode)LX_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C2_V1,PLAYDOHop_LX_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C2_C1, (Opcode)LX_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C2_C1,PLAYDOHop_LX_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C2_C2, (Opcode)LX_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C2_C2,PLAYDOHop_LX_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C2_C3, (Opcode)LX_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C2_C3,PLAYDOHop_LX_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C3_V1, (Opcode)LX_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C3_V1,PLAYDOHop_LX_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C3_C1, (Opcode)LX_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C3_C1,PLAYDOHop_LX_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C3_C2, (Opcode)LX_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C3_C2,PLAYDOHop_LX_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_W_C3_C3, (Opcode)LX_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_W_C3_C3,PLAYDOHop_LX_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_V1_V1, (Opcode)LX_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_V1_V1,PLAYDOHop_LX_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_V1_C1, (Opcode)LX_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_V1_C1,PLAYDOHop_LX_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_V1_C2, (Opcode)LX_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_V1_C2,PLAYDOHop_LX_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_V1_C3, (Opcode)LX_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_V1_C3,PLAYDOHop_LX_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C1_V1, (Opcode)LX_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C1_V1,PLAYDOHop_LX_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C1_C1, (Opcode)LX_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C1_C1,PLAYDOHop_LX_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C1_C2, (Opcode)LX_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C1_C2,PLAYDOHop_LX_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C1_C3, (Opcode)LX_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C1_C3,PLAYDOHop_LX_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C2_V1, (Opcode)LX_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C2_V1,PLAYDOHop_LX_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C2_C1, (Opcode)LX_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C2_C1,PLAYDOHop_LX_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C2_C2, (Opcode)LX_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C2_C2,PLAYDOHop_LX_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C2_C3, (Opcode)LX_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C2_C3,PLAYDOHop_LX_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C3_V1, (Opcode)LX_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C3_V1,PLAYDOHop_LX_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C3_C1, (Opcode)LX_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C3_C1,PLAYDOHop_LX_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C3_C2, (Opcode)LX_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C3_C2,PLAYDOHop_LX_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LX_Q_C3_C3, (Opcode)LX_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LX_Q_C3_C3,PLAYDOHop_LX_Q_C3_C3) ;
#endif

   /*new load short operation: LG (tangw 05-14-02)*/
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_V1_V1, (Opcode)LG_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_V1,PLAYDOHop_LG_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_V1_C1, (Opcode)LG_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_C1,PLAYDOHop_LG_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_V1_C2, (Opcode)LG_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_C2,PLAYDOHop_LG_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_V1_C3, (Opcode)LG_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_C3,PLAYDOHop_LG_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C1_V1, (Opcode)LG_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_V1,PLAYDOHop_LG_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C1_C1, (Opcode)LG_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_C1,PLAYDOHop_LG_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C1_C2, (Opcode)LG_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_C2,PLAYDOHop_LG_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C1_C3, (Opcode)LG_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_C3,PLAYDOHop_LG_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C2_V1, (Opcode)LG_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_V1,PLAYDOHop_LG_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C2_C1, (Opcode)LG_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_C1,PLAYDOHop_LG_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C2_C2, (Opcode)LG_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_C2,PLAYDOHop_LG_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C2_C3, (Opcode)LG_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_C3,PLAYDOHop_LG_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C3_V1, (Opcode)LG_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_V1,PLAYDOHop_LG_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C3_C1, (Opcode)LG_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_C1,PLAYDOHop_LG_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C3_C2, (Opcode)LG_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_C2,PLAYDOHop_LG_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_B_C3_C3, (Opcode)LG_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_C3,PLAYDOHop_LG_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_V1_V1, (Opcode)LG_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_V1,PLAYDOHop_LG_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_V1_C1, (Opcode)LG_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_C1,PLAYDOHop_LG_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_V1_C2, (Opcode)LG_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_C2,PLAYDOHop_LG_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_V1_C3, (Opcode)LG_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_C3,PLAYDOHop_LG_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C1_V1, (Opcode)LG_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_V1,PLAYDOHop_LG_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C1_C1, (Opcode)LG_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_C1,PLAYDOHop_LG_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C1_C2, (Opcode)LG_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_C2,PLAYDOHop_LG_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C1_C3, (Opcode)LG_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_C3,PLAYDOHop_LG_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C2_V1, (Opcode)LG_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_V1,PLAYDOHop_LG_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C2_C1, (Opcode)LG_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_C1,PLAYDOHop_LG_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C2_C2, (Opcode)LG_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_C2,PLAYDOHop_LG_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C2_C3, (Opcode)LG_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_C3,PLAYDOHop_LG_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C3_V1, (Opcode)LG_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_V1,PLAYDOHop_LG_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C3_C1, (Opcode)LG_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_C1,PLAYDOHop_LG_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C3_C2, (Opcode)LG_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_C2,PLAYDOHop_LG_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_H_C3_C3, (Opcode)LG_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_C3,PLAYDOHop_LG_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_V1_V1, (Opcode)LG_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_V1,PLAYDOHop_LG_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_V1_C1, (Opcode)LG_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_C1,PLAYDOHop_LG_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_V1_C2, (Opcode)LG_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_C2,PLAYDOHop_LG_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_V1_C3, (Opcode)LG_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_C3,PLAYDOHop_LG_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C1_V1, (Opcode)LG_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_V1,PLAYDOHop_LG_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C1_C1, (Opcode)LG_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_C1,PLAYDOHop_LG_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C1_C2, (Opcode)LG_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_C2,PLAYDOHop_LG_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C1_C3, (Opcode)LG_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_C3,PLAYDOHop_LG_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C2_V1, (Opcode)LG_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_V1,PLAYDOHop_LG_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C2_C1, (Opcode)LG_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_C1,PLAYDOHop_LG_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C2_C2, (Opcode)LG_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_C2,PLAYDOHop_LG_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C2_C3, (Opcode)LG_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_C3,PLAYDOHop_LG_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C3_V1, (Opcode)LG_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_V1,PLAYDOHop_LG_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C3_C1, (Opcode)LG_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_C1,PLAYDOHop_LG_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C3_C2, (Opcode)LG_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_C2,PLAYDOHop_LG_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_W_C3_C3, (Opcode)LG_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_C3,PLAYDOHop_LG_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_V1_V1, (Opcode)LG_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_V1,PLAYDOHop_LG_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_V1_C1, (Opcode)LG_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_C1,PLAYDOHop_LG_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_V1_C2, (Opcode)LG_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_C2,PLAYDOHop_LG_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_V1_C3, (Opcode)LG_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_C3,PLAYDOHop_LG_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C1_V1, (Opcode)LG_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_V1,PLAYDOHop_LG_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C1_C1, (Opcode)LG_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_C1,PLAYDOHop_LG_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C1_C2, (Opcode)LG_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_C2,PLAYDOHop_LG_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C1_C3, (Opcode)LG_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_C3,PLAYDOHop_LG_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C2_V1, (Opcode)LG_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_V1,PLAYDOHop_LG_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C2_C1, (Opcode)LG_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_C1,PLAYDOHop_LG_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C2_C2, (Opcode)LG_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_C2,PLAYDOHop_LG_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C2_C3, (Opcode)LG_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_C3,PLAYDOHop_LG_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C3_V1, (Opcode)LG_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_V1,PLAYDOHop_LG_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C3_C1, (Opcode)LG_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_C1,PLAYDOHop_LG_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C3_C2, (Opcode)LG_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_C2,PLAYDOHop_LG_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LG_Q_C3_C3, (Opcode)LG_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_C3,PLAYDOHop_LG_Q_C3_C3) ;
#endif

   /*load short w/ sign extension: LGX (tangw 05-14-02)*/
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_V1_V1, (Opcode)LGX_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_V1,PLAYDOHop_LGX_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_V1_C1, (Opcode)LGX_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_C1,PLAYDOHop_LGX_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_V1_C2, (Opcode)LGX_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_C2,PLAYDOHop_LGX_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_V1_C3, (Opcode)LGX_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_C3,PLAYDOHop_LGX_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C1_V1, (Opcode)LGX_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_V1,PLAYDOHop_LGX_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C1_C1, (Opcode)LGX_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_C1,PLAYDOHop_LGX_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C1_C2, (Opcode)LGX_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_C2,PLAYDOHop_LGX_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C1_C3, (Opcode)LGX_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_C3,PLAYDOHop_LGX_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C2_V1, (Opcode)LGX_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_V1,PLAYDOHop_LGX_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C2_C1, (Opcode)LGX_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_C1,PLAYDOHop_LGX_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C2_C2, (Opcode)LGX_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_C2,PLAYDOHop_LGX_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C2_C3, (Opcode)LGX_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_C3,PLAYDOHop_LGX_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C3_V1, (Opcode)LGX_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_V1,PLAYDOHop_LGX_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C3_C1, (Opcode)LGX_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_C1,PLAYDOHop_LGX_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C3_C2, (Opcode)LGX_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_C2,PLAYDOHop_LGX_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_B_C3_C3, (Opcode)LGX_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_C3,PLAYDOHop_LGX_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_V1_V1, (Opcode)LGX_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_V1,PLAYDOHop_LGX_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_V1_C1, (Opcode)LGX_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_C1,PLAYDOHop_LGX_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_V1_C2, (Opcode)LGX_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_C2,PLAYDOHop_LGX_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_V1_C3, (Opcode)LGX_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_C3,PLAYDOHop_LGX_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C1_V1, (Opcode)LGX_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_V1,PLAYDOHop_LGX_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C1_C1, (Opcode)LGX_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_C1,PLAYDOHop_LGX_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C1_C2, (Opcode)LGX_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_C2,PLAYDOHop_LGX_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C1_C3, (Opcode)LGX_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_C3,PLAYDOHop_LGX_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C2_V1, (Opcode)LGX_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_V1,PLAYDOHop_LGX_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C2_C1, (Opcode)LGX_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_C1,PLAYDOHop_LGX_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C2_C2, (Opcode)LGX_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_C2,PLAYDOHop_LGX_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C2_C3, (Opcode)LGX_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_C3,PLAYDOHop_LGX_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C3_V1, (Opcode)LGX_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_V1,PLAYDOHop_LGX_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C3_C1, (Opcode)LGX_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_C1,PLAYDOHop_LGX_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C3_C2, (Opcode)LGX_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_C2,PLAYDOHop_LGX_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_H_C3_C3, (Opcode)LGX_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_C3,PLAYDOHop_LGX_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_V1_V1, (Opcode)LGX_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_V1,PLAYDOHop_LGX_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_V1_C1, (Opcode)LGX_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_C1,PLAYDOHop_LGX_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_V1_C2, (Opcode)LGX_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_C2,PLAYDOHop_LGX_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_V1_C3, (Opcode)LGX_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_C3,PLAYDOHop_LGX_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C1_V1, (Opcode)LGX_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_V1,PLAYDOHop_LGX_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C1_C1, (Opcode)LGX_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_C1,PLAYDOHop_LGX_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C1_C2, (Opcode)LGX_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_C2,PLAYDOHop_LGX_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C1_C3, (Opcode)LGX_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_C3,PLAYDOHop_LGX_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C2_V1, (Opcode)LGX_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_V1,PLAYDOHop_LGX_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C2_C1, (Opcode)LGX_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_C1,PLAYDOHop_LGX_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C2_C2, (Opcode)LGX_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_C2,PLAYDOHop_LGX_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C2_C3, (Opcode)LGX_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_C3,PLAYDOHop_LGX_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C3_V1, (Opcode)LGX_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_V1,PLAYDOHop_LGX_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C3_C1, (Opcode)LGX_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_C1,PLAYDOHop_LGX_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C3_C2, (Opcode)LGX_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_C2,PLAYDOHop_LGX_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_W_C3_C3, (Opcode)LGX_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_C3,PLAYDOHop_LGX_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_V1_V1, (Opcode)LGX_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_V1,PLAYDOHop_LGX_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_V1_C1, (Opcode)LGX_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_C1,PLAYDOHop_LGX_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_V1_C2, (Opcode)LGX_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_C2,PLAYDOHop_LGX_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_V1_C3, (Opcode)LGX_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_C3,PLAYDOHop_LGX_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C1_V1, (Opcode)LGX_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_V1,PLAYDOHop_LGX_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C1_C1, (Opcode)LGX_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_C1,PLAYDOHop_LGX_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C1_C2, (Opcode)LGX_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_C2,PLAYDOHop_LGX_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C1_C3, (Opcode)LGX_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_C3,PLAYDOHop_LGX_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C2_V1, (Opcode)LGX_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_V1,PLAYDOHop_LGX_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C2_C1, (Opcode)LGX_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_C1,PLAYDOHop_LGX_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C2_C2, (Opcode)LGX_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_C2,PLAYDOHop_LGX_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C2_C3, (Opcode)LGX_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_C3,PLAYDOHop_LGX_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C3_V1, (Opcode)LGX_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_V1,PLAYDOHop_LGX_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C3_C1, (Opcode)LGX_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_C1,PLAYDOHop_LGX_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C3_C2, (Opcode)LGX_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_C2,PLAYDOHop_LGX_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LGX_Q_C3_C3, (Opcode)LGX_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_C3,PLAYDOHop_LGX_Q_C3_C3) ;
#endif

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_V1_V1, (Opcode)LG_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_V1,PLAYDOHop_LM_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_V1_C1, (Opcode)LG_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_C1,PLAYDOHop_LM_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_V1_C2, (Opcode)LG_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_C2,PLAYDOHop_LM_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_V1_C3, (Opcode)LG_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_V1_C3,PLAYDOHop_LM_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C1_V1, (Opcode)LG_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_V1,PLAYDOHop_LM_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C1_C1, (Opcode)LG_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_C1,PLAYDOHop_LM_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C1_C2, (Opcode)LG_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_C2,PLAYDOHop_LM_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C1_C3, (Opcode)LG_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C1_C3,PLAYDOHop_LM_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C2_V1, (Opcode)LG_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_V1,PLAYDOHop_LM_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C2_C1, (Opcode)LG_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_C1,PLAYDOHop_LM_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C2_C2, (Opcode)LG_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_C2,PLAYDOHop_LM_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C2_C3, (Opcode)LG_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C2_C3,PLAYDOHop_LM_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C3_V1, (Opcode)LG_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_V1,PLAYDOHop_LM_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C3_C1, (Opcode)LG_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_C1,PLAYDOHop_LM_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C3_C2, (Opcode)LG_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_C2,PLAYDOHop_LM_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_B_C3_C3, (Opcode)LG_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_B_C3_C3,PLAYDOHop_LM_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_V1_V1, (Opcode)LG_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_V1,PLAYDOHop_LM_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_V1_C1, (Opcode)LG_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_C1,PLAYDOHop_LM_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_V1_C2, (Opcode)LG_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_C2,PLAYDOHop_LM_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_V1_C3, (Opcode)LG_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_V1_C3,PLAYDOHop_LM_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C1_V1, (Opcode)LG_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_V1,PLAYDOHop_LM_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C1_C1, (Opcode)LG_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_C1,PLAYDOHop_LM_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C1_C2, (Opcode)LG_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_C2,PLAYDOHop_LM_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C1_C3, (Opcode)LG_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C1_C3,PLAYDOHop_LM_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C2_V1, (Opcode)LG_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_V1,PLAYDOHop_LM_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C2_C1, (Opcode)LG_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_C1,PLAYDOHop_LM_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C2_C2, (Opcode)LG_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_C2,PLAYDOHop_LM_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C2_C3, (Opcode)LG_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C2_C3,PLAYDOHop_LM_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C3_V1, (Opcode)LG_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_V1,PLAYDOHop_LM_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C3_C1, (Opcode)LG_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_C1,PLAYDOHop_LM_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C3_C2, (Opcode)LG_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_C2,PLAYDOHop_LM_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_H_C3_C3, (Opcode)LG_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_H_C3_C3,PLAYDOHop_LM_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_V1_V1, (Opcode)LG_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_V1,PLAYDOHop_LM_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_V1_C1, (Opcode)LG_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_C1,PLAYDOHop_LM_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_V1_C2, (Opcode)LG_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_C2,PLAYDOHop_LM_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_V1_C3, (Opcode)LG_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_V1_C3,PLAYDOHop_LM_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C1_V1, (Opcode)LG_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_V1,PLAYDOHop_LM_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C1_C1, (Opcode)LG_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_C1,PLAYDOHop_LM_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C1_C2, (Opcode)LG_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_C2,PLAYDOHop_LM_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C1_C3, (Opcode)LG_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C1_C3,PLAYDOHop_LM_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C2_V1, (Opcode)LG_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_V1,PLAYDOHop_LM_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C2_C1, (Opcode)LG_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_C1,PLAYDOHop_LM_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C2_C2, (Opcode)LG_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_C2,PLAYDOHop_LM_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C2_C3, (Opcode)LG_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C2_C3,PLAYDOHop_LM_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C3_V1, (Opcode)LG_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_V1,PLAYDOHop_LM_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C3_C1, (Opcode)LG_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_C1,PLAYDOHop_LM_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C3_C2, (Opcode)LG_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_C2,PLAYDOHop_LM_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_W_C3_C3, (Opcode)LG_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_W_C3_C3,PLAYDOHop_LM_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_V1_V1, (Opcode)LG_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_V1,PLAYDOHop_LM_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_V1_C1, (Opcode)LG_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_C1,PLAYDOHop_LM_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_V1_C2, (Opcode)LG_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_C2,PLAYDOHop_LM_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_V1_C3, (Opcode)LG_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_V1_C3,PLAYDOHop_LM_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C1_V1, (Opcode)LG_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_V1,PLAYDOHop_LM_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C1_C1, (Opcode)LG_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_C1,PLAYDOHop_LM_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C1_C2, (Opcode)LG_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_C2,PLAYDOHop_LM_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C1_C3, (Opcode)LG_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C1_C3,PLAYDOHop_LM_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C2_V1, (Opcode)LG_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_V1,PLAYDOHop_LM_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C2_C1, (Opcode)LG_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_C1,PLAYDOHop_LM_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C2_C2, (Opcode)LG_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_C2,PLAYDOHop_LM_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C2_C3, (Opcode)LG_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C2_C3,PLAYDOHop_LM_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C3_V1, (Opcode)LG_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_V1,PLAYDOHop_LM_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C3_C1, (Opcode)LG_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_C1,PLAYDOHop_LM_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C3_C2, (Opcode)LG_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_C2,PLAYDOHop_LM_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LM_Q_C3_C3, (Opcode)LG_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LG_Q_C3_C3,PLAYDOHop_LM_Q_C3_C3) ;
#endif
  
    /*new load medium operation w/ sign extension: LMX (tangw 05-14-02)*/
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_V1_V1, (Opcode)LGX_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_V1,PLAYDOHop_LMX_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_V1_C1, (Opcode)LGX_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_C1,PLAYDOHop_LMX_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_V1_C2, (Opcode)LGX_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_C2,PLAYDOHop_LMX_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_V1_C3, (Opcode)LGX_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_V1_C3,PLAYDOHop_LMX_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C1_V1, (Opcode)LGX_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_V1,PLAYDOHop_LMX_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C1_C1, (Opcode)LGX_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_C1,PLAYDOHop_LMX_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C1_C2, (Opcode)LGX_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_C2,PLAYDOHop_LMX_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C1_C3, (Opcode)LGX_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C1_C3,PLAYDOHop_LMX_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C2_V1, (Opcode)LGX_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_V1,PLAYDOHop_LMX_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C2_C1, (Opcode)LGX_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_C1,PLAYDOHop_LMX_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C2_C2, (Opcode)LGX_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_C2,PLAYDOHop_LMX_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C2_C3, (Opcode)LGX_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C2_C3,PLAYDOHop_LMX_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C3_V1, (Opcode)LGX_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_V1,PLAYDOHop_LMX_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C3_C1, (Opcode)LGX_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_C1,PLAYDOHop_LMX_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C3_C2, (Opcode)LGX_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_C2,PLAYDOHop_LMX_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_B_C3_C3, (Opcode)LGX_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_B_C3_C3,PLAYDOHop_LMX_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_V1_V1, (Opcode)LGX_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_V1,PLAYDOHop_LMX_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_V1_C1, (Opcode)LGX_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_C1,PLAYDOHop_LMX_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_V1_C2, (Opcode)LGX_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_C2,PLAYDOHop_LMX_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_V1_C3, (Opcode)LGX_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_V1_C3,PLAYDOHop_LMX_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C1_V1, (Opcode)LGX_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_V1,PLAYDOHop_LMX_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C1_C1, (Opcode)LGX_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_C1,PLAYDOHop_LMX_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C1_C2, (Opcode)LGX_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_C2,PLAYDOHop_LMX_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C1_C3, (Opcode)LGX_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C1_C3,PLAYDOHop_LMX_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C2_V1, (Opcode)LGX_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_V1,PLAYDOHop_LMX_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C2_C1, (Opcode)LGX_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_C1,PLAYDOHop_LMX_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C2_C2, (Opcode)LGX_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_C2,PLAYDOHop_LMX_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C2_C3, (Opcode)LGX_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C2_C3,PLAYDOHop_LMX_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C3_V1, (Opcode)LGX_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_V1,PLAYDOHop_LMX_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C3_C1, (Opcode)LGX_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_C1,PLAYDOHop_LMX_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C3_C2, (Opcode)LGX_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_C2,PLAYDOHop_LMX_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_H_C3_C3, (Opcode)LGX_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_H_C3_C3,PLAYDOHop_LMX_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_V1_V1, (Opcode)LGX_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_V1,PLAYDOHop_LMX_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_V1_C1, (Opcode)LGX_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_C1,PLAYDOHop_LMX_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_V1_C2, (Opcode)LGX_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_C2,PLAYDOHop_LMX_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_V1_C3, (Opcode)LGX_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_V1_C3,PLAYDOHop_LMX_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C1_V1, (Opcode)LGX_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_V1,PLAYDOHop_LMX_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C1_C1, (Opcode)LGX_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_C1,PLAYDOHop_LMX_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C1_C2, (Opcode)LGX_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_C2,PLAYDOHop_LMX_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C1_C3, (Opcode)LGX_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C1_C3,PLAYDOHop_LMX_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C2_V1, (Opcode)LGX_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_V1,PLAYDOHop_LMX_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C2_C1, (Opcode)LGX_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_C1,PLAYDOHop_LMX_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C2_C2, (Opcode)LGX_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_C2,PLAYDOHop_LMX_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C2_C3, (Opcode)LGX_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C2_C3,PLAYDOHop_LMX_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C3_V1, (Opcode)LGX_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_V1,PLAYDOHop_LMX_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C3_C1, (Opcode)LGX_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_C1,PLAYDOHop_LMX_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C3_C2, (Opcode)LGX_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_C2,PLAYDOHop_LMX_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_W_C3_C3, (Opcode)LGX_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_W_C3_C3,PLAYDOHop_LMX_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_V1_V1, (Opcode)LGX_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_V1,PLAYDOHop_LMX_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_V1_C1, (Opcode)LGX_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_C1,PLAYDOHop_LMX_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_V1_C2, (Opcode)LGX_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_C2,PLAYDOHop_LMX_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_V1_C3, (Opcode)LGX_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_V1_C3,PLAYDOHop_LMX_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C1_V1, (Opcode)LGX_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_V1,PLAYDOHop_LMX_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C1_C1, (Opcode)LGX_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_C1,PLAYDOHop_LMX_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C1_C2, (Opcode)LGX_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_C2,PLAYDOHop_LMX_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C1_C3, (Opcode)LGX_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C1_C3,PLAYDOHop_LMX_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C2_V1, (Opcode)LGX_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_V1,PLAYDOHop_LMX_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C2_C1, (Opcode)LGX_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_C1,PLAYDOHop_LMX_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C2_C2, (Opcode)LGX_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_C2,PLAYDOHop_LMX_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C2_C3, (Opcode)LGX_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C2_C3,PLAYDOHop_LMX_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C3_V1, (Opcode)LGX_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_V1,PLAYDOHop_LMX_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C3_C1, (Opcode)LGX_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_C1,PLAYDOHop_LMX_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C3_C2, (Opcode)LGX_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_C2,PLAYDOHop_LMX_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LMX_Q_C3_C3, (Opcode)LGX_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LGX_Q_C3_C3,PLAYDOHop_LMX_Q_C3_C3) ;
#endif
   
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_V1_V1, (Opcode)LI_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_V1_V1,PLAYDOHop_LI_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_V1_C1, (Opcode)LI_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_V1_C1,PLAYDOHop_LI_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_V1_C2, (Opcode)LI_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_V1_C2,PLAYDOHop_LI_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_V1_C3, (Opcode)LI_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_V1_C3,PLAYDOHop_LI_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C1_V1, (Opcode)LI_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C1_V1,PLAYDOHop_LI_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C1_C1, (Opcode)LI_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C1_C1,PLAYDOHop_LI_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C1_C2, (Opcode)LI_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C1_C2,PLAYDOHop_LI_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C1_C3, (Opcode)LI_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C1_C3,PLAYDOHop_LI_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C2_V1, (Opcode)LI_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C2_V1,PLAYDOHop_LI_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C2_C1, (Opcode)LI_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C2_C1,PLAYDOHop_LI_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C2_C2, (Opcode)LI_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C2_C2,PLAYDOHop_LI_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C2_C3, (Opcode)LI_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C2_C3,PLAYDOHop_LI_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C3_V1, (Opcode)LI_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C3_V1,PLAYDOHop_LI_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C3_C1, (Opcode)LI_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C3_C1,PLAYDOHop_LI_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C3_C2, (Opcode)LI_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C3_C2,PLAYDOHop_LI_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_B_C3_C3, (Opcode)LI_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_B_C3_C3,PLAYDOHop_LI_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_V1_V1, (Opcode)LI_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_V1_V1,PLAYDOHop_LI_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_V1_C1, (Opcode)LI_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_V1_C1,PLAYDOHop_LI_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_V1_C2, (Opcode)LI_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_V1_C2,PLAYDOHop_LI_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_V1_C3, (Opcode)LI_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_V1_C3,PLAYDOHop_LI_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C1_V1, (Opcode)LI_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C1_V1,PLAYDOHop_LI_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C1_C1, (Opcode)LI_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C1_C1,PLAYDOHop_LI_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C1_C2, (Opcode)LI_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C1_C2,PLAYDOHop_LI_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C1_C3, (Opcode)LI_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C1_C3,PLAYDOHop_LI_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C2_V1, (Opcode)LI_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C2_V1,PLAYDOHop_LI_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C2_C1, (Opcode)LI_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C2_C1,PLAYDOHop_LI_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C2_C2, (Opcode)LI_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C2_C2,PLAYDOHop_LI_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C2_C3, (Opcode)LI_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C2_C3,PLAYDOHop_LI_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C3_V1, (Opcode)LI_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C3_V1,PLAYDOHop_LI_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C3_C1, (Opcode)LI_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C3_C1,PLAYDOHop_LI_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C3_C2, (Opcode)LI_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C3_C2,PLAYDOHop_LI_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_H_C3_C3, (Opcode)LI_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_H_C3_C3,PLAYDOHop_LI_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_V1_V1, (Opcode)LI_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_V1_V1,PLAYDOHop_LI_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_V1_C1, (Opcode)LI_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_V1_C1,PLAYDOHop_LI_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_V1_C2, (Opcode)LI_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_V1_C2,PLAYDOHop_LI_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_V1_C3, (Opcode)LI_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_V1_C3,PLAYDOHop_LI_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C1_V1, (Opcode)LI_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C1_V1,PLAYDOHop_LI_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C1_C1, (Opcode)LI_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C1_C1,PLAYDOHop_LI_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C1_C2, (Opcode)LI_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C1_C2,PLAYDOHop_LI_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C1_C3, (Opcode)LI_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C1_C3,PLAYDOHop_LI_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C2_V1, (Opcode)LI_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C2_V1,PLAYDOHop_LI_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C2_C1, (Opcode)LI_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C2_C1,PLAYDOHop_LI_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C2_C2, (Opcode)LI_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C2_C2,PLAYDOHop_LI_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C2_C3, (Opcode)LI_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C2_C3,PLAYDOHop_LI_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C3_V1, (Opcode)LI_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C3_V1,PLAYDOHop_LI_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C3_C1, (Opcode)LI_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C3_C1,PLAYDOHop_LI_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C3_C2, (Opcode)LI_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C3_C2,PLAYDOHop_LI_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_W_C3_C3, (Opcode)LI_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_W_C3_C3,PLAYDOHop_LI_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_V1_V1, (Opcode)LI_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_V1_V1,PLAYDOHop_LI_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_V1_C1, (Opcode)LI_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_V1_C1,PLAYDOHop_LI_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_V1_C2, (Opcode)LI_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_V1_C2,PLAYDOHop_LI_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_V1_C3, (Opcode)LI_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_V1_C3,PLAYDOHop_LI_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C1_V1, (Opcode)LI_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C1_V1,PLAYDOHop_LI_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C1_C1, (Opcode)LI_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C1_C1,PLAYDOHop_LI_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C1_C2, (Opcode)LI_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C1_C2,PLAYDOHop_LI_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C1_C3, (Opcode)LI_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C1_C3,PLAYDOHop_LI_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C2_V1, (Opcode)LI_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C2_V1,PLAYDOHop_LI_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C2_C1, (Opcode)LI_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C2_C1,PLAYDOHop_LI_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C2_C2, (Opcode)LI_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C2_C2,PLAYDOHop_LI_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C2_C3, (Opcode)LI_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C2_C3,PLAYDOHop_LI_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C3_V1, (Opcode)LI_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C3_V1,PLAYDOHop_LI_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C3_C1, (Opcode)LI_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C3_C1,PLAYDOHop_LI_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C3_C2, (Opcode)LI_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C3_C2,PLAYDOHop_LI_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LI_Q_C3_C3, (Opcode)LI_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LI_Q_C3_C3,PLAYDOHop_LI_Q_C3_C3) ;
#endif

//
//	Standard FPR load operations
//

   /*floating point load short operation: FLG (tangw 05-15-02)*/
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_V1_V1, (Opcode)FLG_S_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_V1,PLAYDOHop_FLG_S_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_V1_C1, (Opcode)FLG_S_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_C1,PLAYDOHop_FLG_S_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_V1_C2, (Opcode)FLG_S_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_C2,PLAYDOHop_FLG_S_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_V1_C3, (Opcode)FLG_S_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_C3,PLAYDOHop_FLG_S_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C1_V1, (Opcode)FLG_S_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_V1,PLAYDOHop_FLG_S_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C1_C1, (Opcode)FLG_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_C1,PLAYDOHop_FLG_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C1_C2, (Opcode)FLG_S_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_C2,PLAYDOHop_FLG_S_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C1_C3, (Opcode)FLG_S_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_C3,PLAYDOHop_FLG_S_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C2_V1, (Opcode)FLG_S_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_V1,PLAYDOHop_FLG_S_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C2_C1, (Opcode)FLG_S_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_C1,PLAYDOHop_FLG_S_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C2_C2, (Opcode)FLG_S_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_C2,PLAYDOHop_FLG_S_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C2_C3, (Opcode)FLG_S_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_C3,PLAYDOHop_FLG_S_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C3_V1, (Opcode)FLG_S_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_V1,PLAYDOHop_FLG_S_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C3_C1, (Opcode)FLG_S_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_C1,PLAYDOHop_FLG_S_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C3_C2, (Opcode)FLG_S_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_C2,PLAYDOHop_FLG_S_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_S_C3_C3, (Opcode)FLG_S_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_C3,PLAYDOHop_FLG_S_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_V1_V1, (Opcode)FLG_D_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_V1,PLAYDOHop_FLG_D_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_V1_C1, (Opcode)FLG_D_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_C1,PLAYDOHop_FLG_D_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_V1_C2, (Opcode)FLG_D_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_C2,PLAYDOHop_FLG_D_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_V1_C3, (Opcode)FLG_D_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_C3,PLAYDOHop_FLG_D_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C1_V1, (Opcode)FLG_D_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_V1,PLAYDOHop_FLG_D_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C1_C1, (Opcode)FLG_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_C1,PLAYDOHop_FLG_D_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C1_C2, (Opcode)FLG_D_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_C2,PLAYDOHop_FLG_D_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C1_C3, (Opcode)FLG_D_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_C3,PLAYDOHop_FLG_D_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C2_V1, (Opcode)FLG_D_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_V1,PLAYDOHop_FLG_D_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C2_C1, (Opcode)FLG_D_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_C1,PLAYDOHop_FLG_D_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C2_C2, (Opcode)FLG_D_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_C2,PLAYDOHop_FLG_D_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C2_C3, (Opcode)FLG_D_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_C3,PLAYDOHop_FLG_D_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C3_V1, (Opcode)FLG_D_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_V1,PLAYDOHop_FLG_D_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C3_C1, (Opcode)FLG_D_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_C1,PLAYDOHop_FLG_D_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C3_C2, (Opcode)FLG_D_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_C2,PLAYDOHop_FLG_D_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLG_D_C3_C3, (Opcode)FLG_D_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_C3,PLAYDOHop_FLG_D_C3_C3) ;

   /*floating point load medium operation: FLM (tangw 05-15-02)*/
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_V1_V1, (Opcode)FLG_S_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_V1,PLAYDOHop_FLM_S_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_V1_C1, (Opcode)FLG_S_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_C1,PLAYDOHop_FLM_S_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_V1_C2, (Opcode)FLG_S_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_C2,PLAYDOHop_FLM_S_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_V1_C3, (Opcode)FLG_S_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_V1_C3,PLAYDOHop_FLM_S_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C1_V1, (Opcode)FLG_S_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_V1,PLAYDOHop_FLM_S_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C1_C1, (Opcode)FLG_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_C1,PLAYDOHop_FLM_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C1_C2, (Opcode)FLG_S_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_C2,PLAYDOHop_FLM_S_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C1_C3, (Opcode)FLG_S_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C1_C3,PLAYDOHop_FLM_S_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C2_V1, (Opcode)FLG_S_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_V1,PLAYDOHop_FLM_S_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C2_C1, (Opcode)FLG_S_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_C1,PLAYDOHop_FLM_S_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C2_C2, (Opcode)FLG_S_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_C2,PLAYDOHop_FLM_S_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C2_C3, (Opcode)FLG_S_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C2_C3,PLAYDOHop_FLM_S_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C3_V1, (Opcode)FLG_S_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_V1,PLAYDOHop_FLM_S_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C3_C1, (Opcode)FLG_S_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_C1,PLAYDOHop_FLM_S_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C3_C2, (Opcode)FLG_S_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_C2,PLAYDOHop_FLM_S_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_S_C3_C3, (Opcode)FLG_S_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_S_C3_C3,PLAYDOHop_FLM_S_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_V1_V1, (Opcode)FLG_D_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_V1,PLAYDOHop_FLM_D_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_V1_C1, (Opcode)FLG_D_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_C1,PLAYDOHop_FLM_D_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_V1_C2, (Opcode)FLG_D_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_C2,PLAYDOHop_FLM_D_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_V1_C3, (Opcode)FLG_D_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_V1_C3,PLAYDOHop_FLM_D_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C1_V1, (Opcode)FLG_D_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_V1,PLAYDOHop_FLM_D_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C1_C1, (Opcode)FLG_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_C1,PLAYDOHop_FLM_D_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C1_C2, (Opcode)FLG_D_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_C2,PLAYDOHop_FLM_D_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C1_C3, (Opcode)FLG_D_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C1_C3,PLAYDOHop_FLM_D_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C2_V1, (Opcode)FLG_D_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_V1,PLAYDOHop_FLM_D_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C2_C1, (Opcode)FLG_D_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_C1,PLAYDOHop_FLM_D_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C2_C2, (Opcode)FLG_D_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_C2,PLAYDOHop_FLM_D_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C2_C3, (Opcode)FLG_D_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C2_C3,PLAYDOHop_FLM_D_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C3_V1, (Opcode)FLG_D_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_V1,PLAYDOHop_FLM_D_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C3_C1, (Opcode)FLG_D_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_C1,PLAYDOHop_FLM_D_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C3_C2, (Opcode)FLG_D_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_C2,PLAYDOHop_FLM_D_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLM_D_C3_C3, (Opcode)FLG_D_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLG_D_C3_C3,PLAYDOHop_FLM_D_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_V1_V1, (Opcode)FL_S_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_V1_V1,PLAYDOHop_FL_S_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_V1_C1, (Opcode)FL_S_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_V1_C1,PLAYDOHop_FL_S_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_V1_C2, (Opcode)FL_S_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_V1_C2,PLAYDOHop_FL_S_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_V1_C3, (Opcode)FL_S_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_V1_C3,PLAYDOHop_FL_S_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C1_V1, (Opcode)FL_S_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C1_V1,PLAYDOHop_FL_S_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C1_C1, (Opcode)FL_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C1_C1,PLAYDOHop_FL_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C1_C2, (Opcode)FL_S_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C1_C2,PLAYDOHop_FL_S_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C1_C3, (Opcode)FL_S_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C1_C3,PLAYDOHop_FL_S_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C2_V1, (Opcode)FL_S_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C2_V1,PLAYDOHop_FL_S_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C2_C1, (Opcode)FL_S_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C2_C1,PLAYDOHop_FL_S_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C2_C2, (Opcode)FL_S_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C2_C2,PLAYDOHop_FL_S_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C2_C3, (Opcode)FL_S_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C2_C3,PLAYDOHop_FL_S_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C3_V1, (Opcode)FL_S_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C3_V1,PLAYDOHop_FL_S_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C3_C1, (Opcode)FL_S_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C3_C1,PLAYDOHop_FL_S_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C3_C2, (Opcode)FL_S_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C3_C2,PLAYDOHop_FL_S_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_S_C3_C3, (Opcode)FL_S_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_S_C3_C3,PLAYDOHop_FL_S_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_V1_V1, (Opcode)FL_D_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_V1_V1,PLAYDOHop_FL_D_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_V1_C1, (Opcode)FL_D_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_V1_C1,PLAYDOHop_FL_D_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_V1_C2, (Opcode)FL_D_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_V1_C2,PLAYDOHop_FL_D_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_V1_C3, (Opcode)FL_D_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_V1_C3,PLAYDOHop_FL_D_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C1_V1, (Opcode)FL_D_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C1_V1,PLAYDOHop_FL_D_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C1_C1, (Opcode)FL_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C1_C1,PLAYDOHop_FL_D_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C1_C2, (Opcode)FL_D_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C1_C2,PLAYDOHop_FL_D_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C1_C3, (Opcode)FL_D_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C1_C3,PLAYDOHop_FL_D_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C2_V1, (Opcode)FL_D_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C2_V1,PLAYDOHop_FL_D_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C2_C1, (Opcode)FL_D_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C2_C1,PLAYDOHop_FL_D_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C2_C2, (Opcode)FL_D_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C2_C2,PLAYDOHop_FL_D_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C2_C3, (Opcode)FL_D_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C2_C3,PLAYDOHop_FL_D_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C3_V1, (Opcode)FL_D_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C3_V1,PLAYDOHop_FL_D_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C3_C1, (Opcode)FL_D_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C3_C1,PLAYDOHop_FL_D_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C3_C2, (Opcode)FL_D_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C3_C2,PLAYDOHop_FL_D_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FL_D_C3_C3, (Opcode)FL_D_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FL_D_C3_C3,PLAYDOHop_FL_D_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_V1_V1, (Opcode)FLI_S_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_V1_V1,PLAYDOHop_FLI_S_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_V1_C1, (Opcode)FLI_S_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_V1_C1,PLAYDOHop_FLI_S_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_V1_C2, (Opcode)FLI_S_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_V1_C2,PLAYDOHop_FLI_S_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_V1_C3, (Opcode)FLI_S_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_V1_C3,PLAYDOHop_FLI_S_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C1_V1, (Opcode)FLI_S_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C1_V1,PLAYDOHop_FLI_S_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C1_C1, (Opcode)FLI_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C1_C1,PLAYDOHop_FLI_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C1_C2, (Opcode)FLI_S_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C1_C2,PLAYDOHop_FLI_S_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C1_C3, (Opcode)FLI_S_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C1_C3,PLAYDOHop_FLI_S_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C2_V1, (Opcode)FLI_S_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C2_V1,PLAYDOHop_FLI_S_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C2_C1, (Opcode)FLI_S_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C2_C1,PLAYDOHop_FLI_S_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C2_C2, (Opcode)FLI_S_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C2_C2,PLAYDOHop_FLI_S_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C2_C3, (Opcode)FLI_S_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C2_C3,PLAYDOHop_FLI_S_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C3_V1, (Opcode)FLI_S_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C3_V1,PLAYDOHop_FLI_S_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C3_C1, (Opcode)FLI_S_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C3_C1,PLAYDOHop_FLI_S_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C3_C2, (Opcode)FLI_S_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C3_C2,PLAYDOHop_FLI_S_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_S_C3_C3, (Opcode)FLI_S_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_S_C3_C3,PLAYDOHop_FLI_S_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_V1_V1, (Opcode)FLI_D_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_V1_V1,PLAYDOHop_FLI_D_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_V1_C1, (Opcode)FLI_D_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_V1_C1,PLAYDOHop_FLI_D_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_V1_C2, (Opcode)FLI_D_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_V1_C2,PLAYDOHop_FLI_D_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_V1_C3, (Opcode)FLI_D_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_V1_C3,PLAYDOHop_FLI_D_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C1_V1, (Opcode)FLI_D_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C1_V1,PLAYDOHop_FLI_D_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C1_C1, (Opcode)FLI_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C1_C1,PLAYDOHop_FLI_D_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C1_C2, (Opcode)FLI_D_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C1_C2,PLAYDOHop_FLI_D_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C1_C3, (Opcode)FLI_D_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C1_C3,PLAYDOHop_FLI_D_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C2_V1, (Opcode)FLI_D_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C2_V1,PLAYDOHop_FLI_D_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C2_C1, (Opcode)FLI_D_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C2_C1,PLAYDOHop_FLI_D_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C2_C2, (Opcode)FLI_D_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C2_C2,PLAYDOHop_FLI_D_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C2_C3, (Opcode)FLI_D_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C2_C3,PLAYDOHop_FLI_D_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C3_V1, (Opcode)FLI_D_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C3_V1,PLAYDOHop_FLI_D_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C3_C1, (Opcode)FLI_D_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C3_C1,PLAYDOHop_FLI_D_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C3_C2, (Opcode)FLI_D_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C3_C2,PLAYDOHop_FLI_D_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLI_D_C3_C3, (Opcode)FLI_D_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLI_D_C3_C3,PLAYDOHop_FLI_D_C3_C3) ;

//
//	Standard GPR store operations
//

   /*store short operation: SG (tangw 05-15-02)*/
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_B_V1, (Opcode)SG_B_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_V1,PLAYDOHop_SG_B_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_B_C1, (Opcode)SG_B_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_C1,PLAYDOHop_SG_B_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_B_C2, (Opcode)SG_B_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_C2,PLAYDOHop_SG_B_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_B_C3, (Opcode)SG_B_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_C3,PLAYDOHop_SG_B_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_H_V1, (Opcode)SG_H_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_V1,PLAYDOHop_SG_H_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_H_C1, (Opcode)SG_H_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_C1,PLAYDOHop_SG_H_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_H_C2, (Opcode)SG_H_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_C2,PLAYDOHop_SG_H_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_H_C3, (Opcode)SG_H_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_C3,PLAYDOHop_SG_H_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_W_V1, (Opcode)SG_W_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_V1,PLAYDOHop_SG_W_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_W_C1, (Opcode)SG_W_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_C1,PLAYDOHop_SG_W_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_W_C2, (Opcode)SG_W_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_C2,PLAYDOHop_SG_W_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_W_C3, (Opcode)SG_W_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_C3,PLAYDOHop_SG_W_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_Q_V1, (Opcode)SG_Q_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_Q_V1,PLAYDOHop_SG_Q_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_Q_C1, (Opcode)SG_Q_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_Q_C1,PLAYDOHop_SG_Q_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_Q_C2, (Opcode)SG_Q_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_Q_C2,PLAYDOHop_SG_Q_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SG_Q_C3, (Opcode)SG_Q_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_Q_C3,PLAYDOHop_SG_Q_C3) ;
#endif

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_B_V1, (Opcode)SG_B_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_V1,PLAYDOHop_SM_B_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_B_C1, (Opcode)SG_B_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_C1,PLAYDOHop_SM_B_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_B_C2, (Opcode)SG_B_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_C2,PLAYDOHop_SM_B_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_B_C3, (Opcode)SG_B_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_B_C3,PLAYDOHop_SM_B_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_H_V1, (Opcode)SG_H_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_V1,PLAYDOHop_SM_H_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_H_C1, (Opcode)SG_H_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_C1,PLAYDOHop_SM_H_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_H_C2, (Opcode)SG_H_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_C2,PLAYDOHop_SM_H_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_H_C3, (Opcode)SG_H_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_H_C3,PLAYDOHop_SM_H_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_W_V1, (Opcode)SG_W_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_V1,PLAYDOHop_SM_W_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_W_C1, (Opcode)SG_W_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_C1,PLAYDOHop_SM_W_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_W_C2, (Opcode)SG_W_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_C2,PLAYDOHop_SM_W_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SM_W_C3, (Opcode)SG_W_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SG_W_C3,PLAYDOHop_SM_W_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_B_V1, (Opcode)S_B_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_B_V1,PLAYDOHop_S_B_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_B_C1, (Opcode)S_B_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_B_C1,PLAYDOHop_S_B_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_B_C2, (Opcode)S_B_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_B_C2,PLAYDOHop_S_B_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_B_C3, (Opcode)S_B_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_B_C3,PLAYDOHop_S_B_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_H_V1, (Opcode)S_H_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_H_V1,PLAYDOHop_S_H_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_H_C1, (Opcode)S_H_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_H_C1,PLAYDOHop_S_H_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_H_C2, (Opcode)S_H_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_H_C2,PLAYDOHop_S_H_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_H_C3, (Opcode)S_H_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_H_C3,PLAYDOHop_S_H_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_W_V1, (Opcode)S_W_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_W_V1,PLAYDOHop_S_W_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_W_C1, (Opcode)S_W_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_W_C1,PLAYDOHop_S_W_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_W_C2, (Opcode)S_W_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_W_C2,PLAYDOHop_S_W_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_W_C3, (Opcode)S_W_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_W_C3,PLAYDOHop_S_W_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_Q_V1, (Opcode)S_Q_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_Q_V1,PLAYDOHop_S_Q_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_Q_C1, (Opcode)S_Q_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_Q_C1,PLAYDOHop_S_Q_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_Q_C2, (Opcode)S_Q_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_Q_C2,PLAYDOHop_S_Q_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_S_Q_C3, (Opcode)S_Q_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) S_Q_C3,PLAYDOHop_S_Q_C3) ;
#endif

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_B_V1, (Opcode)SI_B_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_B_V1,PLAYDOHop_SI_B_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_B_C1, (Opcode)SI_B_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_B_C1,PLAYDOHop_SI_B_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_B_C2, (Opcode)SI_B_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_B_C2,PLAYDOHop_SI_B_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_B_C3, (Opcode)SI_B_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_B_C3,PLAYDOHop_SI_B_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_H_V1, (Opcode)SI_H_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_H_V1,PLAYDOHop_SI_H_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_H_C1, (Opcode)SI_H_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_H_C1,PLAYDOHop_SI_H_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_H_C2, (Opcode)SI_H_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_H_C2,PLAYDOHop_SI_H_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_H_C3, (Opcode)SI_H_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_H_C3,PLAYDOHop_SI_H_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_W_V1, (Opcode)SI_W_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_W_V1,PLAYDOHop_SI_W_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_W_C1, (Opcode)SI_W_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_W_C1,PLAYDOHop_SI_W_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_W_C2, (Opcode)SI_W_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_W_C2,PLAYDOHop_SI_W_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_W_C3, (Opcode)SI_W_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_W_C3,PLAYDOHop_SI_W_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_Q_V1, (Opcode)SI_Q_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_Q_V1,PLAYDOHop_SI_Q_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_Q_C1, (Opcode)SI_Q_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_Q_C1,PLAYDOHop_SI_Q_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_Q_C2, (Opcode)SI_Q_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_Q_C2,PLAYDOHop_SI_Q_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SI_Q_C3, (Opcode)SI_Q_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) SI_Q_C3,PLAYDOHop_SI_Q_C3) ;
#endif

//
//	Standard FPR store operations
//

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_S_V1, (Opcode)FS_S_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_S_V1,PLAYDOHop_FS_S_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_S_C1, (Opcode)FS_S_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_S_C1,PLAYDOHop_FS_S_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_S_C2, (Opcode)FS_S_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_S_C2,PLAYDOHop_FS_S_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_S_C3, (Opcode)FS_S_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_S_C3,PLAYDOHop_FS_S_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_D_V1, (Opcode)FS_D_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_D_V1,PLAYDOHop_FS_D_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_D_C1, (Opcode)FS_D_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_D_C1,PLAYDOHop_FS_D_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_D_C2, (Opcode)FS_D_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_D_C2,PLAYDOHop_FS_D_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FS_D_C3, (Opcode)FS_D_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FS_D_C3,PLAYDOHop_FS_D_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_S_V1, (Opcode)FSI_S_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_S_V1,PLAYDOHop_FSI_S_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_S_C1, (Opcode)FSI_S_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_S_C1,PLAYDOHop_FSI_S_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_S_C2, (Opcode)FSI_S_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_S_C2,PLAYDOHop_FSI_S_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_S_C3, (Opcode)FSI_S_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_S_C3,PLAYDOHop_FSI_S_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_D_V1, (Opcode)FSI_D_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_D_V1,PLAYDOHop_FSI_D_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_D_C1, (Opcode)FSI_D_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_D_C1,PLAYDOHop_FSI_D_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_D_C2, (Opcode)FSI_D_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_D_C2,PLAYDOHop_FSI_D_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSI_D_C3, (Opcode)FSI_D_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSI_D_C3,PLAYDOHop_FSI_D_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_S_V1, (Opcode)FSG_S_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_V1,PLAYDOHop_FSG_S_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_S_C1, (Opcode)FSG_S_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_C1,PLAYDOHop_FSG_S_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_S_C2, (Opcode)FSG_S_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_C2,PLAYDOHop_FSG_S_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_S_C3, (Opcode)FSG_S_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_C3,PLAYDOHop_FSG_S_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_D_V1, (Opcode)FSG_D_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_V1,PLAYDOHop_FSG_D_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_D_C1, (Opcode)FSG_D_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_C1,PLAYDOHop_FSG_D_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_D_C2, (Opcode)FSG_D_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_C2,PLAYDOHop_FSG_D_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSG_D_C3, (Opcode)FSG_D_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_C3,PLAYDOHop_FSG_D_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_S_V1, (Opcode)FSG_S_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_V1,PLAYDOHop_FSM_S_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_S_C1, (Opcode)FSG_S_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_C1,PLAYDOHop_FSM_S_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_S_C2, (Opcode)FSG_S_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_C2,PLAYDOHop_FSM_S_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_S_C3, (Opcode)FSG_S_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_S_C3,PLAYDOHop_FSM_S_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_D_V1, (Opcode)FSG_D_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_V1,PLAYDOHop_FSM_D_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_D_C1, (Opcode)FSG_D_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_C1,PLAYDOHop_FSM_D_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_D_C2, (Opcode)FSG_D_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_C2,PLAYDOHop_FSM_D_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSM_D_C3, (Opcode)FSG_D_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FSG_D_C3,PLAYDOHop_FSM_D_C3) ;
}

//
//	Data speculative loads and verify ops
//
static void el_init_lcode_opcode_maps_data_spec()
{
//
//	Data speculative GPR load operations
//

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_V1_V1, (Opcode)LDS_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_V1_V1,PLAYDOHop_LDS_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_V1_C1, (Opcode)LDS_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_V1_C1,PLAYDOHop_LDS_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_V1_C2, (Opcode)LDS_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_V1_C2,PLAYDOHop_LDS_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_V1_C3, (Opcode)LDS_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_V1_C3,PLAYDOHop_LDS_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C1_V1, (Opcode)LDS_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C1_V1,PLAYDOHop_LDS_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C1_C1, (Opcode)LDS_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C1_C1,PLAYDOHop_LDS_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C1_C2, (Opcode)LDS_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C1_C2,PLAYDOHop_LDS_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C1_C3, (Opcode)LDS_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C1_C3,PLAYDOHop_LDS_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C2_V1, (Opcode)LDS_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C2_V1,PLAYDOHop_LDS_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C2_C1, (Opcode)LDS_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C2_C1,PLAYDOHop_LDS_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C2_C2, (Opcode)LDS_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C2_C2,PLAYDOHop_LDS_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C2_C3, (Opcode)LDS_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C2_C3,PLAYDOHop_LDS_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C3_V1, (Opcode)LDS_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C3_V1,PLAYDOHop_LDS_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C3_C1, (Opcode)LDS_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C3_C1,PLAYDOHop_LDS_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C3_C2, (Opcode)LDS_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C3_C2,PLAYDOHop_LDS_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_B_C3_C3, (Opcode)LDS_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_B_C3_C3,PLAYDOHop_LDS_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_V1_V1, (Opcode)LDS_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_V1_V1,PLAYDOHop_LDS_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_V1_C1, (Opcode)LDS_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_V1_C1,PLAYDOHop_LDS_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_V1_C2, (Opcode)LDS_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_V1_C2,PLAYDOHop_LDS_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_V1_C3, (Opcode)LDS_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_V1_C3,PLAYDOHop_LDS_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C1_V1, (Opcode)LDS_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C1_V1,PLAYDOHop_LDS_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C1_C1, (Opcode)LDS_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C1_C1,PLAYDOHop_LDS_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C1_C2, (Opcode)LDS_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C1_C2,PLAYDOHop_LDS_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C1_C3, (Opcode)LDS_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C1_C3,PLAYDOHop_LDS_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C2_V1, (Opcode)LDS_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C2_V1,PLAYDOHop_LDS_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C2_C1, (Opcode)LDS_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C2_C1,PLAYDOHop_LDS_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C2_C2, (Opcode)LDS_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C2_C2,PLAYDOHop_LDS_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C2_C3, (Opcode)LDS_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C2_C3,PLAYDOHop_LDS_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C3_V1, (Opcode)LDS_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C3_V1,PLAYDOHop_LDS_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C3_C1, (Opcode)LDS_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C3_C1,PLAYDOHop_LDS_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C3_C2, (Opcode)LDS_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C3_C2,PLAYDOHop_LDS_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_H_C3_C3, (Opcode)LDS_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_H_C3_C3,PLAYDOHop_LDS_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_V1_V1, (Opcode)LDS_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_V1_V1,PLAYDOHop_LDS_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_V1_C1, (Opcode)LDS_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_V1_C1,PLAYDOHop_LDS_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_V1_C2, (Opcode)LDS_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_V1_C2,PLAYDOHop_LDS_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_V1_C3, (Opcode)LDS_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_V1_C3,PLAYDOHop_LDS_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C1_V1, (Opcode)LDS_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C1_V1,PLAYDOHop_LDS_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C1_C1, (Opcode)LDS_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C1_C1,PLAYDOHop_LDS_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C1_C2, (Opcode)LDS_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C1_C2,PLAYDOHop_LDS_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C1_C3, (Opcode)LDS_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C1_C3,PLAYDOHop_LDS_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C2_V1, (Opcode)LDS_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C2_V1,PLAYDOHop_LDS_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C2_C1, (Opcode)LDS_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C2_C1,PLAYDOHop_LDS_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C2_C2, (Opcode)LDS_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C2_C2,PLAYDOHop_LDS_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C2_C3, (Opcode)LDS_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C2_C3,PLAYDOHop_LDS_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C3_V1, (Opcode)LDS_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C3_V1,PLAYDOHop_LDS_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C3_C1, (Opcode)LDS_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C3_C1,PLAYDOHop_LDS_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C3_C2, (Opcode)LDS_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C3_C2,PLAYDOHop_LDS_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_W_C3_C3, (Opcode)LDS_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_W_C3_C3,PLAYDOHop_LDS_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_V1_V1, (Opcode)LDS_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_V1_V1,PLAYDOHop_LDS_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_V1_C1, (Opcode)LDS_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_V1_C1,PLAYDOHop_LDS_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_V1_C2, (Opcode)LDS_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_V1_C2,PLAYDOHop_LDS_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_V1_C3, (Opcode)LDS_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_V1_C3,PLAYDOHop_LDS_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C1_V1, (Opcode)LDS_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C1_V1,PLAYDOHop_LDS_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C1_C1, (Opcode)LDS_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C1_C1,PLAYDOHop_LDS_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C1_C2, (Opcode)LDS_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C1_C2,PLAYDOHop_LDS_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C1_C3, (Opcode)LDS_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C1_C3,PLAYDOHop_LDS_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C2_V1, (Opcode)LDS_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C2_V1,PLAYDOHop_LDS_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C2_C1, (Opcode)LDS_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C2_C1,PLAYDOHop_LDS_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C2_C2, (Opcode)LDS_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C2_C2,PLAYDOHop_LDS_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C2_C3, (Opcode)LDS_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C2_C3,PLAYDOHop_LDS_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C3_V1, (Opcode)LDS_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C3_V1,PLAYDOHop_LDS_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C3_C1, (Opcode)LDS_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C3_C1,PLAYDOHop_LDS_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C3_C2, (Opcode)LDS_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C3_C2,PLAYDOHop_LDS_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDS_Q_C3_C3, (Opcode)LDS_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDS_Q_C3_C3,PLAYDOHop_LDS_Q_C3_C3) ;
#endif

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_V1_V1, (Opcode)LDSI_B_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_V1_V1,PLAYDOHop_LDSI_B_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_V1_C1, (Opcode)LDSI_B_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_V1_C1,PLAYDOHop_LDSI_B_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_V1_C2, (Opcode)LDSI_B_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_V1_C2,PLAYDOHop_LDSI_B_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_V1_C3, (Opcode)LDSI_B_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_V1_C3,PLAYDOHop_LDSI_B_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C1_V1, (Opcode)LDSI_B_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C1_V1,PLAYDOHop_LDSI_B_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C1_C1, (Opcode)LDSI_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C1_C1,PLAYDOHop_LDSI_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C1_C2, (Opcode)LDSI_B_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C1_C2,PLAYDOHop_LDSI_B_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C1_C3, (Opcode)LDSI_B_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C1_C3,PLAYDOHop_LDSI_B_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C2_V1, (Opcode)LDSI_B_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C2_V1,PLAYDOHop_LDSI_B_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C2_C1, (Opcode)LDSI_B_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C2_C1,PLAYDOHop_LDSI_B_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C2_C2, (Opcode)LDSI_B_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C2_C2,PLAYDOHop_LDSI_B_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C2_C3, (Opcode)LDSI_B_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C2_C3,PLAYDOHop_LDSI_B_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C3_V1, (Opcode)LDSI_B_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C3_V1,PLAYDOHop_LDSI_B_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C3_C1, (Opcode)LDSI_B_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C3_C1,PLAYDOHop_LDSI_B_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C3_C2, (Opcode)LDSI_B_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C3_C2,PLAYDOHop_LDSI_B_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_B_C3_C3, (Opcode)LDSI_B_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_B_C3_C3,PLAYDOHop_LDSI_B_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_V1_V1, (Opcode)LDSI_H_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_V1_V1,PLAYDOHop_LDSI_H_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_V1_C1, (Opcode)LDSI_H_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_V1_C1,PLAYDOHop_LDSI_H_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_V1_C2, (Opcode)LDSI_H_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_V1_C2,PLAYDOHop_LDSI_H_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_V1_C3, (Opcode)LDSI_H_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_V1_C3,PLAYDOHop_LDSI_H_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C1_V1, (Opcode)LDSI_H_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C1_V1,PLAYDOHop_LDSI_H_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C1_C1, (Opcode)LDSI_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C1_C1,PLAYDOHop_LDSI_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C1_C2, (Opcode)LDSI_H_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C1_C2,PLAYDOHop_LDSI_H_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C1_C3, (Opcode)LDSI_H_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C1_C3,PLAYDOHop_LDSI_H_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C2_V1, (Opcode)LDSI_H_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C2_V1,PLAYDOHop_LDSI_H_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C2_C1, (Opcode)LDSI_H_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C2_C1,PLAYDOHop_LDSI_H_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C2_C2, (Opcode)LDSI_H_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C2_C2,PLAYDOHop_LDSI_H_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C2_C3, (Opcode)LDSI_H_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C2_C3,PLAYDOHop_LDSI_H_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C3_V1, (Opcode)LDSI_H_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C3_V1,PLAYDOHop_LDSI_H_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C3_C1, (Opcode)LDSI_H_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C3_C1,PLAYDOHop_LDSI_H_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C3_C2, (Opcode)LDSI_H_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C3_C2,PLAYDOHop_LDSI_H_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_H_C3_C3, (Opcode)LDSI_H_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_H_C3_C3,PLAYDOHop_LDSI_H_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_V1_V1, (Opcode)LDSI_W_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_V1_V1,PLAYDOHop_LDSI_W_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_V1_C1, (Opcode)LDSI_W_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_V1_C1,PLAYDOHop_LDSI_W_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_V1_C2, (Opcode)LDSI_W_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_V1_C2,PLAYDOHop_LDSI_W_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_V1_C3, (Opcode)LDSI_W_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_V1_C3,PLAYDOHop_LDSI_W_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C1_V1, (Opcode)LDSI_W_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C1_V1,PLAYDOHop_LDSI_W_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C1_C1, (Opcode)LDSI_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C1_C1,PLAYDOHop_LDSI_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C1_C2, (Opcode)LDSI_W_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C1_C2,PLAYDOHop_LDSI_W_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C1_C3, (Opcode)LDSI_W_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C1_C3,PLAYDOHop_LDSI_W_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C2_V1, (Opcode)LDSI_W_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C2_V1,PLAYDOHop_LDSI_W_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C2_C1, (Opcode)LDSI_W_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C2_C1,PLAYDOHop_LDSI_W_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C2_C2, (Opcode)LDSI_W_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C2_C2,PLAYDOHop_LDSI_W_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C2_C3, (Opcode)LDSI_W_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C2_C3,PLAYDOHop_LDSI_W_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C3_V1, (Opcode)LDSI_W_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C3_V1,PLAYDOHop_LDSI_W_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C3_C1, (Opcode)LDSI_W_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C3_C1,PLAYDOHop_LDSI_W_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C3_C2, (Opcode)LDSI_W_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C3_C2,PLAYDOHop_LDSI_W_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_W_C3_C3, (Opcode)LDSI_W_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_W_C3_C3,PLAYDOHop_LDSI_W_C3_C3) ;

#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_V1_V1, (Opcode)LDSI_Q_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_V1_V1,PLAYDOHop_LDSI_Q_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_V1_C1, (Opcode)LDSI_Q_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_V1_C1,PLAYDOHop_LDSI_Q_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_V1_C2, (Opcode)LDSI_Q_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_V1_C2,PLAYDOHop_LDSI_Q_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_V1_C3, (Opcode)LDSI_Q_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_V1_C3,PLAYDOHop_LDSI_Q_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C1_V1, (Opcode)LDSI_Q_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C1_V1,PLAYDOHop_LDSI_Q_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C1_C1, (Opcode)LDSI_Q_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C1_C1,PLAYDOHop_LDSI_Q_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C1_C2, (Opcode)LDSI_Q_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C1_C2,PLAYDOHop_LDSI_Q_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C1_C3, (Opcode)LDSI_Q_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C1_C3,PLAYDOHop_LDSI_Q_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C2_V1, (Opcode)LDSI_Q_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C2_V1,PLAYDOHop_LDSI_Q_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C2_C1, (Opcode)LDSI_Q_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C2_C1,PLAYDOHop_LDSI_Q_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C2_C2, (Opcode)LDSI_Q_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C2_C2,PLAYDOHop_LDSI_Q_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C2_C3, (Opcode)LDSI_Q_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C2_C3,PLAYDOHop_LDSI_Q_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C3_V1, (Opcode)LDSI_Q_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C3_V1,PLAYDOHop_LDSI_Q_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C3_C1, (Opcode)LDSI_Q_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C3_C1,PLAYDOHop_LDSI_Q_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C3_C2, (Opcode)LDSI_Q_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C3_C2,PLAYDOHop_LDSI_Q_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDSI_Q_C3_C3, (Opcode)LDSI_Q_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) LDSI_Q_C3_C3,PLAYDOHop_LDSI_Q_C3_C3) ;
#endif

//
//	Data speculative FPR load operations
//

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_V1_V1, (Opcode)FLDS_S_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_V1_V1,PLAYDOHop_FLDS_S_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_V1_C1, (Opcode)FLDS_S_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_V1_C1,PLAYDOHop_FLDS_S_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_V1_C2, (Opcode)FLDS_S_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_V1_C2,PLAYDOHop_FLDS_S_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_V1_C3, (Opcode)FLDS_S_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_V1_C3,PLAYDOHop_FLDS_S_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C1_V1, (Opcode)FLDS_S_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C1_V1,PLAYDOHop_FLDS_S_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C1_C1, (Opcode)FLDS_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C1_C1,PLAYDOHop_FLDS_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C1_C2, (Opcode)FLDS_S_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C1_C2,PLAYDOHop_FLDS_S_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C1_C3, (Opcode)FLDS_S_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C1_C3,PLAYDOHop_FLDS_S_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C2_V1, (Opcode)FLDS_S_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C2_V1,PLAYDOHop_FLDS_S_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C2_C1, (Opcode)FLDS_S_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C2_C1,PLAYDOHop_FLDS_S_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C2_C2, (Opcode)FLDS_S_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C2_C2,PLAYDOHop_FLDS_S_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C2_C3, (Opcode)FLDS_S_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C2_C3,PLAYDOHop_FLDS_S_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C3_V1, (Opcode)FLDS_S_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C3_V1,PLAYDOHop_FLDS_S_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C3_C1, (Opcode)FLDS_S_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C3_C1,PLAYDOHop_FLDS_S_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C3_C2, (Opcode)FLDS_S_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C3_C2,PLAYDOHop_FLDS_S_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_S_C3_C3, (Opcode)FLDS_S_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_S_C3_C3,PLAYDOHop_FLDS_S_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_V1_V1, (Opcode)FLDS_D_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_V1_V1,PLAYDOHop_FLDS_D_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_V1_C1, (Opcode)FLDS_D_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_V1_C1,PLAYDOHop_FLDS_D_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_V1_C2, (Opcode)FLDS_D_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_V1_C2,PLAYDOHop_FLDS_D_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_V1_C3, (Opcode)FLDS_D_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_V1_C3,PLAYDOHop_FLDS_D_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C1_V1, (Opcode)FLDS_D_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C1_V1,PLAYDOHop_FLDS_D_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C1_C1, (Opcode)FLDS_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C1_C1,PLAYDOHop_FLDS_D_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C1_C2, (Opcode)FLDS_D_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C1_C2,PLAYDOHop_FLDS_D_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C1_C3, (Opcode)FLDS_D_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C1_C3,PLAYDOHop_FLDS_D_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C2_V1, (Opcode)FLDS_D_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C2_V1,PLAYDOHop_FLDS_D_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C2_C1, (Opcode)FLDS_D_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C2_C1,PLAYDOHop_FLDS_D_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C2_C2, (Opcode)FLDS_D_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C2_C2,PLAYDOHop_FLDS_D_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C2_C3, (Opcode)FLDS_D_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C2_C3,PLAYDOHop_FLDS_D_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C3_V1, (Opcode)FLDS_D_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C3_V1,PLAYDOHop_FLDS_D_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C3_C1, (Opcode)FLDS_D_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C3_C1,PLAYDOHop_FLDS_D_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C3_C2, (Opcode)FLDS_D_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C3_C2,PLAYDOHop_FLDS_D_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDS_D_C3_C3, (Opcode)FLDS_D_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDS_D_C3_C3,PLAYDOHop_FLDS_D_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_V1_V1, (Opcode)FLDSI_S_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_V1_V1,PLAYDOHop_FLDSI_S_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_V1_C1, (Opcode)FLDSI_S_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_V1_C1,PLAYDOHop_FLDSI_S_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_V1_C2, (Opcode)FLDSI_S_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_V1_C2,PLAYDOHop_FLDSI_S_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_V1_C3, (Opcode)FLDSI_S_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_V1_C3,PLAYDOHop_FLDSI_S_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C1_V1, (Opcode)FLDSI_S_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C1_V1,PLAYDOHop_FLDSI_S_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C1_C1, (Opcode)FLDSI_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C1_C1,PLAYDOHop_FLDSI_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C1_C2, (Opcode)FLDSI_S_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C1_C2,PLAYDOHop_FLDSI_S_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C1_C3, (Opcode)FLDSI_S_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C1_C3,PLAYDOHop_FLDSI_S_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C2_V1, (Opcode)FLDSI_S_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C2_V1,PLAYDOHop_FLDSI_S_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C2_C1, (Opcode)FLDSI_S_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C2_C1,PLAYDOHop_FLDSI_S_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C2_C2, (Opcode)FLDSI_S_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C2_C2,PLAYDOHop_FLDSI_S_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C2_C3, (Opcode)FLDSI_S_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C2_C3,PLAYDOHop_FLDSI_S_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C3_V1, (Opcode)FLDSI_S_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C3_V1,PLAYDOHop_FLDSI_S_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C3_C1, (Opcode)FLDSI_S_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C3_C1,PLAYDOHop_FLDSI_S_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C3_C2, (Opcode)FLDSI_S_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C3_C2,PLAYDOHop_FLDSI_S_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_S_C3_C3, (Opcode)FLDSI_S_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_S_C3_C3,PLAYDOHop_FLDSI_S_C3_C3) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_V1_V1, (Opcode)FLDSI_D_V1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_V1_V1,PLAYDOHop_FLDSI_D_V1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_V1_C1, (Opcode)FLDSI_D_V1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_V1_C1,PLAYDOHop_FLDSI_D_V1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_V1_C2, (Opcode)FLDSI_D_V1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_V1_C2,PLAYDOHop_FLDSI_D_V1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_V1_C3, (Opcode)FLDSI_D_V1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_V1_C3,PLAYDOHop_FLDSI_D_V1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C1_V1, (Opcode)FLDSI_D_C1_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C1_V1,PLAYDOHop_FLDSI_D_C1_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C1_C1, (Opcode)FLDSI_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C1_C1,PLAYDOHop_FLDSI_D_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C1_C2, (Opcode)FLDSI_D_C1_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C1_C2,PLAYDOHop_FLDSI_D_C1_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C1_C3, (Opcode)FLDSI_D_C1_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C1_C3,PLAYDOHop_FLDSI_D_C1_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C2_V1, (Opcode)FLDSI_D_C2_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C2_V1,PLAYDOHop_FLDSI_D_C2_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C2_C1, (Opcode)FLDSI_D_C2_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C2_C1,PLAYDOHop_FLDSI_D_C2_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C2_C2, (Opcode)FLDSI_D_C2_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C2_C2,PLAYDOHop_FLDSI_D_C2_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C2_C3, (Opcode)FLDSI_D_C2_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C2_C3,PLAYDOHop_FLDSI_D_C2_C3) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C3_V1, (Opcode)FLDSI_D_C3_V1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C3_V1,PLAYDOHop_FLDSI_D_C3_V1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C3_C1, (Opcode)FLDSI_D_C3_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C3_C1,PLAYDOHop_FLDSI_D_C3_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C3_C2, (Opcode)FLDSI_D_C3_C2) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C3_C2,PLAYDOHop_FLDSI_D_C3_C2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDSI_D_C3_C3, (Opcode)FLDSI_D_C3_C3) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) FLDSI_D_C3_C3,PLAYDOHop_FLDSI_D_C3_C3) ;

//
//	Data verify GPR load operations
//

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDV_B,LDV_B) ;
   el_elcor_to_lcode_opcode_map.bind(LDV_B,PLAYDOHop_LDV_B) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDV_H,LDV_H) ;
   el_elcor_to_lcode_opcode_map.bind(LDV_H,PLAYDOHop_LDV_H) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDV_W,LDV_W) ;
   el_elcor_to_lcode_opcode_map.bind(LDV_W,PLAYDOHop_LDV_W) ;
#if 0
   // uncomment these when they are defined in Mspec
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LDV_Q,LDV_Q) ;
   el_elcor_to_lcode_opcode_map.bind(LDV_Q,PLAYDOHop_LDV_Q) ;
#endif

//
//	Data verify FPR load operations
//

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDV_S,FLDV_S) ;
   el_elcor_to_lcode_opcode_map.bind(FLDV_S,PLAYDOHop_FLDV_S) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLDV_D,FLDV_D) ;
   el_elcor_to_lcode_opcode_map.bind(FLDV_D,PLAYDOHop_FLDV_D) ;
}

//
//	Local memory loads and stores.
//
static void el_init_lcode_opcode_maps_lm()
{
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_B_L1, (Opcode) LL_B_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_B_L1, PLAYDOHop_LL_B_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_B_L2, (Opcode) LL_B_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_B_L2, PLAYDOHop_LL_B_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_B_L3, (Opcode) LL_B_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_B_L3, PLAYDOHop_LL_B_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_B_L4, (Opcode) LL_B_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_B_L4, PLAYDOHop_LL_B_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_H_L1, (Opcode) LL_H_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_H_L1, PLAYDOHop_LL_H_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_H_L2, (Opcode) LL_H_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_H_L2, PLAYDOHop_LL_H_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_H_L3, (Opcode) LL_H_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_H_L3, PLAYDOHop_LL_H_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_H_L4, (Opcode) LL_H_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_H_L4, PLAYDOHop_LL_H_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_W_L1, (Opcode) LL_W_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_W_L1, PLAYDOHop_LL_W_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_W_L2, (Opcode) LL_W_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_W_L2, PLAYDOHop_LL_W_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_W_L3, (Opcode) LL_W_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_W_L3, PLAYDOHop_LL_W_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_W_L4, (Opcode) LL_W_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_W_L4, PLAYDOHop_LL_W_L4);

#if 0
   // uncomment these when they are defined in Mspec
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_Q_L1, (Opcode) LL_Q_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_Q_L1, PLAYDOHop_LL_Q_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_Q_L2, (Opcode) LL_Q_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_Q_L2, PLAYDOHop_LL_Q_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_Q_L3, (Opcode) LL_Q_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_Q_L3, PLAYDOHop_LL_Q_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LL_Q_L4, (Opcode) LL_Q_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LL_Q_L4, PLAYDOHop_LL_Q_L4);
#endif

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_B_L1, (Opcode) LLX_B_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_B_L1, PLAYDOHop_LLX_B_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_B_L2, (Opcode) LLX_B_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_B_L2, PLAYDOHop_LLX_B_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_B_L3, (Opcode) LLX_B_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_B_L3, PLAYDOHop_LLX_B_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_B_L4, (Opcode) LLX_B_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_B_L4, PLAYDOHop_LLX_B_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_H_L1, (Opcode) LLX_H_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_H_L1, PLAYDOHop_LLX_H_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_H_L2, (Opcode) LLX_H_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_H_L2, PLAYDOHop_LLX_H_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_H_L3, (Opcode) LLX_H_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_H_L3, PLAYDOHop_LLX_H_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_H_L4, (Opcode) LLX_H_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_H_L4, PLAYDOHop_LLX_H_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_W_L1, (Opcode) LLX_W_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_W_L1, PLAYDOHop_LLX_W_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_W_L2, (Opcode) LLX_W_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_W_L2, PLAYDOHop_LLX_W_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_W_L3, (Opcode) LLX_W_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_W_L3, PLAYDOHop_LLX_W_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_W_L4, (Opcode) LLX_W_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_W_L4, PLAYDOHop_LLX_W_L4);

#if 0
   // uncomment these when they are defined in Mspec
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_Q_L1, (Opcode) LLX_Q_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_Q_L1, PLAYDOHop_LLX_Q_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_Q_L2, (Opcode) LLX_Q_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_Q_L2, PLAYDOHop_LLX_Q_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_Q_L3, (Opcode) LLX_Q_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_Q_L3, PLAYDOHop_LLX_Q_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_LLX_Q_L4, (Opcode) LLX_Q_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) LLX_Q_L4, PLAYDOHop_LLX_Q_L4);
#endif

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_S_L1, (Opcode) FLL_S_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_S_L1, PLAYDOHop_FLL_S_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_S_L2, (Opcode) FLL_S_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_S_L2, PLAYDOHop_FLL_S_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_S_L3, (Opcode) FLL_S_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_S_L3, PLAYDOHop_FLL_S_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_S_L4, (Opcode) FLL_S_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_S_L4, PLAYDOHop_FLL_S_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_D_L1, (Opcode) FLL_D_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_D_L1, PLAYDOHop_FLL_D_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_D_L2, (Opcode) FLL_D_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_D_L2, PLAYDOHop_FLL_D_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_D_L3, (Opcode) FLL_D_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_D_L3, PLAYDOHop_FLL_D_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FLL_D_L4, (Opcode) FLL_D_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FLL_D_L4, PLAYDOHop_FLL_D_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_B_L1, (Opcode) SL_B_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_B_L1, PLAYDOHop_SL_B_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_B_L2, (Opcode) SL_B_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_B_L2, PLAYDOHop_SL_B_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_B_L3, (Opcode) SL_B_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_B_L3, PLAYDOHop_SL_B_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_B_L4, (Opcode) SL_B_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_B_L4, PLAYDOHop_SL_B_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_H_L1, (Opcode) SL_H_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_H_L1, PLAYDOHop_SL_H_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_H_L2, (Opcode) SL_H_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_H_L2, PLAYDOHop_SL_H_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_H_L3, (Opcode) SL_H_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_H_L3, PLAYDOHop_SL_H_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_H_L4, (Opcode) SL_H_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_H_L4, PLAYDOHop_SL_H_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_W_L1, (Opcode) SL_W_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_W_L1, PLAYDOHop_SL_W_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_W_L2, (Opcode) SL_W_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_W_L2, PLAYDOHop_SL_W_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_W_L3, (Opcode) SL_W_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_W_L3, PLAYDOHop_SL_W_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_W_L4, (Opcode) SL_W_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_W_L4, PLAYDOHop_SL_W_L4);

#if 0
   // uncomment these when they are defined in Mspec
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_Q_L1, (Opcode) SL_Q_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_Q_L1, PLAYDOHop_SL_Q_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_Q_L2, (Opcode) SL_Q_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_Q_L2, PLAYDOHop_SL_Q_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_Q_L3, (Opcode) SL_Q_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_Q_L3, PLAYDOHop_SL_Q_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SL_Q_L4, (Opcode) SL_Q_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) SL_Q_L4, PLAYDOHop_SL_Q_L4);
#endif

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_S_L1, (Opcode) FSL_S_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_S_L1, PLAYDOHop_FSL_S_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_S_L2, (Opcode) FSL_S_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_S_L2, PLAYDOHop_FSL_S_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_S_L3, (Opcode) FSL_S_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_S_L3, PLAYDOHop_FSL_S_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_S_L4, (Opcode) FSL_S_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_S_L4, PLAYDOHop_FSL_S_L4);

    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_D_L1, (Opcode) FSL_D_L1);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_D_L1, PLAYDOHop_FSL_D_L1);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_D_L2, (Opcode) FSL_D_L2);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_D_L2, PLAYDOHop_FSL_D_L2);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_D_L3, (Opcode) FSL_D_L3);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_D_L3, PLAYDOHop_FSL_D_L3);
    el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSL_D_L4, (Opcode) FSL_D_L4);
    el_elcor_to_lcode_opcode_map.bind((Opcode) FSL_D_L4, PLAYDOHop_FSL_D_L4);
}

//
//	Compare to register operations
//    
static void el_init_lcode_opcode_cmpr()
{
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_CMPR_FALSE,CMPR_W_FALSE) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_FALSE,PLAYDOHop_CMPR_FALSE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_EQ,CMPR_W_EQ) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_EQ,Lop_EQ) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LT,CMPR_W_LT) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_LT,Lop_LT) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LE,CMPR_W_LEQ) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_LEQ,Lop_LE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GT,CMPR_W_GT) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_GT,Lop_GT) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GE,CMPR_W_GEQ) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_GEQ,Lop_GE) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_CMPR_SV,CMPR_W_SV) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_SV,PLAYDOHop_CMPR_SV) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_CMPR_OD,CMPR_W_OD) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_OD,PLAYDOHop_CMPR_OD) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_NE,CMPR_W_NEQ) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_NEQ,Lop_NE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LT_U,CMPR_W_LLT) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_LLT,Lop_LT_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LE_U,CMPR_W_LLEQ) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_LLEQ,Lop_LE_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GT_U,CMPR_W_LGT) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_LGT,Lop_GT_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GE_U,CMPR_W_LGEQ) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_LGEQ,Lop_GE_U) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_CMPR_NSV,CMPR_W_NSV) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_NSV,PLAYDOHop_CMPR_NSV) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_CMPR_EV,CMPR_W_EV) ;
   el_elcor_to_lcode_opcode_map.bind(CMPR_W_EV,PLAYDOHop_CMPR_EV) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FCMPR_S_FALSE,FCMPR_S_FALSE) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_FALSE,PLAYDOHop_FCMPR_S_FALSE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_EQ_F,FCMPR_S_EQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_EQ,Lop_EQ_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LT_F,FCMPR_S_LT) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_LT,Lop_LT_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LE_F,FCMPR_S_LEQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_LEQ,Lop_LE_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GT_F,FCMPR_S_GT) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_GT,Lop_GT_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GE_F,FCMPR_S_GEQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_GEQ,Lop_GE_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_NE_F,FCMPR_S_NEQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_NEQ,Lop_NE_F) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FCMPR_S_TRUE,FCMPR_S_TRUE) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_S_TRUE,PLAYDOHop_FCMPR_S_TRUE) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FCMPR_D_FALSE,FCMPR_D_FALSE) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_FALSE,PLAYDOHop_FCMPR_D_FALSE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_EQ_F2,FCMPR_D_EQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_EQ,Lop_EQ_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LT_F2,FCMPR_D_LT) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_LT,Lop_LT_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LE_F2,FCMPR_D_LEQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_LEQ,Lop_LE_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GT_F2,FCMPR_D_GT) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_GT,Lop_GT_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_GE_F2,FCMPR_D_GEQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_GEQ,Lop_GE_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_NE_F2,FCMPR_D_NEQ) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_NEQ,Lop_NE_F2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FCMPR_D_TRUE,FCMPR_D_TRUE) ;
   el_elcor_to_lcode_opcode_map.bind(FCMPR_D_TRUE,PLAYDOHop_FCMPR_D_TRUE) ;
}

//
//	Compare to predicate operations (Note here only map to base
//		CMPP, dest modifiers are added later).
//    
static void el_init_lcode_opcode_cmpp()
{
   // BASE_CMPP_FALSE has no current mapping to Lcode.
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_EQ,(Opcode)BASE_CMPP_W_EQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_EQ,Lop_PRED_EQ) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LT,(Opcode)BASE_CMPP_W_LT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_LT,Lop_PRED_LT) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LE,(Opcode)BASE_CMPP_W_LEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_LEQ,Lop_PRED_LE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GT,(Opcode)BASE_CMPP_W_GT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_GT,Lop_PRED_GT) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GE,(Opcode)BASE_CMPP_W_GEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_GEQ,Lop_PRED_GE) ;
   // BASE_CMPP_SV has no current mapping to Lcode.
   // BASE_CMPP_OD has no current mapping to Lcode.
   // BASE_CMPP_TRUE has no current mapping to Lcode.
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_NE,(Opcode)BASE_CMPP_W_NEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_NEQ,Lop_PRED_NE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LT_U,(Opcode)BASE_CMPP_W_LLT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_LLT,Lop_PRED_LT_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LE_U,(Opcode)BASE_CMPP_W_LLEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_LLEQ,Lop_PRED_LE_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GT_U,(Opcode)BASE_CMPP_W_LGT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_LGT,Lop_PRED_GT_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GE_U,(Opcode)BASE_CMPP_W_LGEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_CMPP_W_LGEQ,Lop_PRED_GE_U) ;
   // BASE_CMPP_NSV has no current mapping to Lcode.
   // BASE_CMPP_EV has no current mapping to Lcode.

   // BASE_FCMPP_S_FALSE has no current mapping to Lcode.
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_EQ_F,(Opcode)BASE_FCMPP_S_EQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_S_EQ,Lop_PRED_EQ_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LT_F,(Opcode)BASE_FCMPP_S_LT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_S_LT,Lop_PRED_LT_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LE_F,(Opcode)BASE_FCMPP_S_LEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_S_LEQ,Lop_PRED_LE_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GT_F,(Opcode)BASE_FCMPP_S_GT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_S_GT,Lop_PRED_GT_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GE_F,(Opcode)BASE_FCMPP_S_GEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_S_GEQ,Lop_PRED_GE_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_NE_F,(Opcode)BASE_FCMPP_S_NEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_S_NEQ,Lop_PRED_NE_F) ;
   // BASE_FCMPP_S_TRUE has no current mapping to Lcode.

   // BASE_FCMPP_D_FALSE has no current mapping to Lcode.
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_EQ_F2,(Opcode)BASE_FCMPP_D_EQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_D_EQ,Lop_PRED_EQ_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LT_F2,(Opcode)BASE_FCMPP_D_LT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_D_LT,Lop_PRED_LT_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LE_F2,(Opcode)BASE_FCMPP_D_LEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_D_LEQ,Lop_PRED_LE_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GT_F2,(Opcode)BASE_FCMPP_D_GT) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_D_GT,Lop_PRED_GT_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_GE_F2,(Opcode)BASE_FCMPP_D_GEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_D_GEQ,Lop_PRED_GE_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_NE_F2,(Opcode)BASE_FCMPP_D_NEQ) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BASE_FCMPP_D_NEQ,Lop_PRED_NE_F2) ;
   // BASE_FCMPP_D_TRUE has no current mapping to Lcode.

   el_lcode_to_elcor_opcode_map.bind(Lop_CMP,(Opcode)ROOT_CMPP) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)ROOT_CMPP,Lop_CMP) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_CMP_F,(Opcode)ROOT_FCMPP) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)ROOT_FCMPP,Lop_CMP_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_RCMP,(Opcode)ROOT_CMPR) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)ROOT_CMPR,Lop_RCMP) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_RCMP_F,(Opcode)ROOT_FCMPR) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)ROOT_FCMPR,Lop_RCMP_F) ;
}

static void el_init_lcode_opcode_arith()
{

//
//	Standard INT Arithmetic operations
//
   el_lcode_to_elcor_opcode_map.bind(Lop_NO_OP,NO_OP) ;
   el_elcor_to_lcode_opcode_map.bind(NO_OP,Lop_NO_OP) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_M_NO_OP,M_NO_OP) ;
   el_elcor_to_lcode_opcode_map.bind(M_NO_OP,PLAYDOHop_M_NO_OP) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ABS,ABS_W) ;
   el_elcor_to_lcode_opcode_map.bind(ABS_W,Lop_ABS) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ADD,ADD_W) ;
   el_elcor_to_lcode_opcode_map.bind(ADD_W,Lop_ADD) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ADD_U,ADDL_W) ;
   el_elcor_to_lcode_opcode_map.bind(ADDL_W,Lop_ADD_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ADD_CARRY,ADD_CARRY_W) ;
   el_elcor_to_lcode_opcode_map.bind(ADD_CARRY_W,Lop_ADD_CARRY) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ADD_CARRY_U,ADDL_CARRY_W) ;
   el_elcor_to_lcode_opcode_map.bind(ADDL_CARRY_W,Lop_ADD_CARRY_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SUB_CARRY_U,SUBL_CARRY_W) ;
   el_elcor_to_lcode_opcode_map.bind(SUBL_CARRY_W,Lop_SUB_CARRY_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SUB_CARRY,SUB_CARRY_W) ;
   el_elcor_to_lcode_opcode_map.bind(SUB_CARRY_W,Lop_SUB_CARRY) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_AND,AND_W) ;
   el_elcor_to_lcode_opcode_map.bind(AND_W,Lop_AND) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_AND_COMPL,ANDCM_W) ;
   el_elcor_to_lcode_opcode_map.bind(ANDCM_W,Lop_AND_COMPL) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_DIV,DIV_W) ;
   el_elcor_to_lcode_opcode_map.bind(DIV_W,Lop_DIV) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_DIV_U,DIVL_W) ;
   el_elcor_to_lcode_opcode_map.bind(DIVL_W,Lop_DIV_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL,MPY_W) ;
   el_elcor_to_lcode_opcode_map.bind(MPY_W,Lop_MUL) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_U,MPYL_W) ;
   el_elcor_to_lcode_opcode_map.bind(MPYL_W,Lop_MUL_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_WIDE,MPY_WIDE_W) ;
   el_elcor_to_lcode_opcode_map.bind(MPY_WIDE_W,Lop_MUL_WIDE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_WIDE_U,MPYL_WIDE_W) ;
   el_elcor_to_lcode_opcode_map.bind(MPYL_WIDE_W,Lop_MUL_WIDE_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_ADD,MPYADD_W) ;
   el_elcor_to_lcode_opcode_map.bind(MPYADD_W,Lop_MUL_ADD) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_SUB_REV,MPYRSUB_W) ;
   el_elcor_to_lcode_opcode_map.bind(MPYRSUB_W,Lop_MUL_SUB_REV) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_SUB,MPYSUB_W) ;
   el_elcor_to_lcode_opcode_map.bind(MPYSUB_W,Lop_MUL_SUB) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_NAND,NAND_W) ;
   el_elcor_to_lcode_opcode_map.bind(NAND_W,Lop_NAND) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_NOR,NOR_W) ;
   el_elcor_to_lcode_opcode_map.bind(NOR_W,Lop_NOR) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_OR,OR_W) ;
   el_elcor_to_lcode_opcode_map.bind(OR_W,Lop_OR) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_OR_COMPL,ORCM_W) ;
   el_elcor_to_lcode_opcode_map.bind(ORCM_W,Lop_OR_COMPL) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_REM,REM_W) ;
   el_elcor_to_lcode_opcode_map.bind(REM_W,Lop_REM) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_REM_U,REML_W) ;
   el_elcor_to_lcode_opcode_map.bind(REML_W,Lop_REM_U) ;
   // SLARSEN: integer min and max
   el_lcode_to_elcor_opcode_map.bind(Lop_MIN,MIN_W) ;
   el_elcor_to_lcode_opcode_map.bind(MIN_W,Lop_MIN) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MAX,MAX_W) ;
   el_elcor_to_lcode_opcode_map.bind(MAX_W,Lop_MAX) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SH1ADDL,SH1ADDL_W) ;
   el_elcor_to_lcode_opcode_map.bind(SH1ADDL_W,PLAYDOHop_SH1ADDL) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SH2ADDL,SH2ADDL_W) ;
   el_elcor_to_lcode_opcode_map.bind(SH2ADDL_W,PLAYDOHop_SH2ADDL) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SH3ADDL,SH3ADDL_W) ;
   el_elcor_to_lcode_opcode_map.bind(SH3ADDL_W,PLAYDOHop_SH3ADDL) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LSL,SHL_W) ;
   el_elcor_to_lcode_opcode_map.bind(SHL_W,Lop_LSL) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_LSR,SHR_W) ;
   el_elcor_to_lcode_opcode_map.bind(SHR_W,Lop_LSR) ;
   // SHLA has no equivalent in Lcode, all left shifts are logical there
   el_elcor_to_lcode_opcode_map.bind(SHLA_W,Lop_LSL);
   el_lcode_to_elcor_opcode_map.bind(Lop_ASR,SHRA_W) ;
   el_elcor_to_lcode_opcode_map.bind(SHRA_W,Lop_ASR) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SUB,SUB_W) ;
   el_elcor_to_lcode_opcode_map.bind(SUB_W,Lop_SUB) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SUB_U,SUBL_W) ;
   el_elcor_to_lcode_opcode_map.bind(SUBL_W,Lop_SUB_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_XOR,XOR_W) ;
   el_elcor_to_lcode_opcode_map.bind(XOR_W,Lop_XOR) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_NXOR,XORCM_W) ;	/* functionally equiv */
   el_elcor_to_lcode_opcode_map.bind(XORCM_W,Lop_NXOR) ;	/* functionally equiv */

//
//	Standard FP Arithmetic operations
//

   el_lcode_to_elcor_opcode_map.bind(Lop_ADD_F,FADD_S) ;
   el_elcor_to_lcode_opcode_map.bind(FADD_S,Lop_ADD_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ADD_F2,FADD_D) ;
   el_elcor_to_lcode_opcode_map.bind(FADD_D,Lop_ADD_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ABS_F,FABS_S) ;
   el_elcor_to_lcode_opcode_map.bind(FABS_S,Lop_ABS_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ABS_F2,FABS_D) ;
   el_elcor_to_lcode_opcode_map.bind(FABS_D,Lop_ABS_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_DIV_F,FDIV_S) ;
   el_elcor_to_lcode_opcode_map.bind(FDIV_S,Lop_DIV_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_DIV_F2,FDIV_D) ;
   el_elcor_to_lcode_opcode_map.bind(FDIV_D,Lop_DIV_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MAX_F,FMAX_S) ;
   el_elcor_to_lcode_opcode_map.bind(FMAX_S,Lop_MAX_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MAX_F2,FMAX_D) ;
   el_elcor_to_lcode_opcode_map.bind(FMAX_D,Lop_MAX_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MIN_F,FMIN_S) ;
   el_elcor_to_lcode_opcode_map.bind(FMIN_S,Lop_MIN_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MIN_F2,FMIN_D) ;
   el_elcor_to_lcode_opcode_map.bind(FMIN_D,Lop_MIN_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_F,FMPY_S) ;
   el_elcor_to_lcode_opcode_map.bind(FMPY_S,Lop_MUL_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_F2,FMPY_D) ;
   el_elcor_to_lcode_opcode_map.bind(FMPY_D,Lop_MUL_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_ADD_F,FMPYADD_S) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYADD_S,Lop_MUL_ADD_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_ADD_F2,FMPYADD_D) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYADD_D,Lop_MUL_ADD_F2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FMPYADDN_S,FMPYADDN_S) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYADDN_S,PLAYDOHop_FMPYADDN_S) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FMPYADDN_D,FMPYADDN_D) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYADDN_D,PLAYDOHop_FMPYADDN_D) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_SUB_REV_F,FMPYRSUB_S) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYRSUB_S,Lop_MUL_SUB_REV_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_SUB_REV_F2,FMPYRSUB_D) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYRSUB_D,Lop_MUL_SUB_REV_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_SUB_F,FMPYSUB_S) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYSUB_S,Lop_MUL_SUB_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MUL_SUB_F2,FMPYSUB_D) ;
   el_elcor_to_lcode_opcode_map.bind(FMPYSUB_D,Lop_MUL_SUB_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_RCP_F,FRCP_S) ;
   el_elcor_to_lcode_opcode_map.bind(FRCP_S,Lop_RCP_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_RCP_F2,FRCP_D) ;
   el_elcor_to_lcode_opcode_map.bind(FRCP_D,Lop_RCP_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SQRT_F,FSQRT_S) ;
   el_elcor_to_lcode_opcode_map.bind(FSQRT_S,Lop_SQRT_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SQRT_F2,FSQRT_D) ;
   el_elcor_to_lcode_opcode_map.bind(FSQRT_D,Lop_SQRT_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SUB_F,FSUB_S) ;
   el_elcor_to_lcode_opcode_map.bind(FSUB_S,Lop_SUB_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SUB_F2,FSUB_D) ;
   el_elcor_to_lcode_opcode_map.bind(FSUB_D,Lop_SUB_F2) ;

//
//	Conversion operations
//    

   el_lcode_to_elcor_opcode_map.bind(Lop_F2_I,CONVDW) ;
   el_elcor_to_lcode_opcode_map.bind(CONVDW,Lop_F2_I) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_I_F2,CONVWD) ;
   el_elcor_to_lcode_opcode_map.bind(CONVWD,Lop_I_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_F_I,CONVSW) ;
   el_elcor_to_lcode_opcode_map.bind(CONVSW,Lop_F_I) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_I_F,CONVWS) ;
   el_elcor_to_lcode_opcode_map.bind(CONVWS,Lop_I_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_F2_F,CONVDS) ;
   el_elcor_to_lcode_opcode_map.bind(CONVDS,Lop_F2_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_F_F2,CONVSD) ;
   el_elcor_to_lcode_opcode_map.bind(CONVSD,Lop_F_F2) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_SXT_C,EXTS_B) ;
   el_elcor_to_lcode_opcode_map.bind(EXTS_B,Lop_SXT_C) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SXT_C2,EXTS_H) ;
   el_elcor_to_lcode_opcode_map.bind(EXTS_H,Lop_SXT_C2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SXT_I,EXTS_W) ;
   el_elcor_to_lcode_opcode_map.bind(EXTS_W,Lop_SXT_I) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_ZXT_C,EXTZ_B) ;
   el_elcor_to_lcode_opcode_map.bind(EXTZ_B,Lop_ZXT_C) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ZXT_C2,EXTZ_H) ;
   el_elcor_to_lcode_opcode_map.bind(EXTZ_H,Lop_ZXT_C2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ZXT_I,EXTZ_W) ;
   el_elcor_to_lcode_opcode_map.bind(EXTZ_W,Lop_ZXT_I) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_EXTRACT,EXTRACT_W) ;
   el_elcor_to_lcode_opcode_map.bind(EXTRACT_W,Lop_EXTRACT) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_EXTRACT_U,EXTRACTU_W) ;
   el_elcor_to_lcode_opcode_map.bind(EXTRACTU_W,Lop_EXTRACT_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_DEPOSIT,DEPOSIT_W) ;
   el_elcor_to_lcode_opcode_map.bind(DEPOSIT_W,Lop_DEPOSIT) ;

//
//	Move operations
//    

   el_lcode_to_elcor_opcode_map.bind(Lop_MOV,MOVE) ;
   el_elcor_to_lcode_opcode_map.bind(MOVE,Lop_MOV) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEGF_L,MOVEGF_L) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEGF_L,PLAYDOHop_MOVEGF_L) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEGF_U,MOVEGF_U) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEGF_U,PLAYDOHop_MOVEGF_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MOV_F,MOVEF_S) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEF_S,Lop_MOV_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_MOV_F2,MOVEF_D) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEF_D,Lop_MOV_F2) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEFG_L,MOVEFG_L) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEFG_L,PLAYDOHop_MOVEFG_L) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEFG_U,MOVEFG_U) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEFG_U,PLAYDOHop_MOVEFG_U) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEPG,MOVEPG) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEPG,PLAYDOHop_MOVEPG) ;
   // LDCM has no current mapping to Lcode.

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEGC,MOVEGC) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEGC,PLAYDOHop_MOVEGC) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVECG,MOVECG) ;
   el_elcor_to_lcode_opcode_map.bind(MOVECG,PLAYDOHop_MOVECG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEGG,MOVEGG) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEGG,PLAYDOHop_MOVEGG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEBB,MOVEBB) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEBB,PLAYDOHop_MOVEBB) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEPP,MOVEPP) ;
   el_elcor_to_lcode_opcode_map.bind(MOVEPP,PLAYDOHop_MOVEPP) ;
}

//
//	Branch operations
//
static void el_init_lcode_opcode_branch()
{
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_PBRR,PBR) ;
   el_elcor_to_lcode_opcode_map.bind(PBR,PLAYDOHop_PBRR) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_PBRA,PBR) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRU,BRU) ;
   el_elcor_to_lcode_opcode_map.bind(BRU,PLAYDOHop_BRU) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRCT,BRCT) ;
   el_elcor_to_lcode_opcode_map.bind(BRCT,PLAYDOHop_BRCT) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRCF,BRCF) ;
   el_elcor_to_lcode_opcode_map.bind(BRCF,PLAYDOHop_BRCF) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRL,BRL) ;
   el_elcor_to_lcode_opcode_map.bind(BRL,PLAYDOHop_BRL) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRLC,BRLC) ;
   el_elcor_to_lcode_opcode_map.bind(BRLC,PLAYDOHop_BRLC) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_RTS,RTS) ;
   el_elcor_to_lcode_opcode_map.bind(RTS,Lop_RTS) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRF_B_B_F,BRF_B_B_F) ;
   el_elcor_to_lcode_opcode_map.bind(BRF_B_B_F,PLAYDOHop_BRF_B_B_F) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRF_B_F_F,BRF_B_F_F) ;
   el_elcor_to_lcode_opcode_map.bind(BRF_B_F_F,PLAYDOHop_BRF_B_F_F) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRF_F_B_B,BRF_F_B_B) ;
   el_elcor_to_lcode_opcode_map.bind(BRF_F_B_B,PLAYDOHop_BRF_F_B_B) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRF_F_F_B,BRF_F_F_B) ;
   el_elcor_to_lcode_opcode_map.bind(BRF_F_F_B,PLAYDOHop_BRF_F_F_B) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRF_F_F_F,BRF_F_F_F) ;
   el_elcor_to_lcode_opcode_map.bind(BRF_F_F_F,PLAYDOHop_BRF_F_F_F) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRW_B_B_F,BRW_B_B_F) ;
   el_elcor_to_lcode_opcode_map.bind(BRW_B_B_F,PLAYDOHop_BRW_B_B_F) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRW_B_F_F,BRW_B_F_F) ;
   el_elcor_to_lcode_opcode_map.bind(BRW_B_F_F,PLAYDOHop_BRW_B_F_F) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRW_F_B_B,BRW_F_B_B) ;
   el_elcor_to_lcode_opcode_map.bind(BRW_F_B_B,PLAYDOHop_BRW_F_B_B) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRW_F_F_B,BRW_F_F_B) ;
   el_elcor_to_lcode_opcode_map.bind(BRW_F_F_B,PLAYDOHop_BRW_F_F_B) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRW_F_F_F,BRW_F_F_F) ;
   el_elcor_to_lcode_opcode_map.bind(BRW_F_F_F,PLAYDOHop_BRW_F_F_F) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRDVI,BRDVI) ;
   el_elcor_to_lcode_opcode_map.bind(BRDVI,PLAYDOHop_BRDVI) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRDVF,BRDVF) ;
   el_elcor_to_lcode_opcode_map.bind(BRDVF,PLAYDOHop_BRDVF) ;
}

//
//	Literal forming operations
//
static void el_init_lcode_opcode_lit()
{
   // Literal moves to btr, pbrs with literals

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELB,MOVELB) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELB,PLAYDOHop_MOVELB) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELBS,MOVELBS) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELBS,PLAYDOHop_MOVELBS) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELBX,MOVELBX) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELBX,PLAYDOHop_MOVELBX) ;
  
   // Literal move operations
 
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELG,MOVELG) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELG,PLAYDOHop_MOVELG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELGS,MOVELGS) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELGS,PLAYDOHop_MOVELGS) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELGX,MOVELGX) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELGX,PLAYDOHop_MOVELGX) ;
 
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELF,MOVELF) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELF,PLAYDOHop_MOVELF) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVELFS,MOVELFS) ;
   el_elcor_to_lcode_opcode_map.bind(MOVELFS,PLAYDOHop_MOVELFS) ;
}

//
//	Regalloc ops
//
static void el_init_lcode_opcode_regalloc()
{
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SAVE,(Opcode)SAVE) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)SAVE,PLAYDOHop_SAVE) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_RESTORE,(Opcode)RESTORE) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)RESTORE,PLAYDOHop_RESTORE) ;
   // Explicitly may FSAVE and FRESTORE to the Playdoh double precision versions
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSAVE_S,(Opcode)FSAVE_S) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FSAVE_S,PLAYDOHop_FSAVE_S) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FRESTORE_S,(Opcode)FRESTORE_S) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FRESTORE_S,PLAYDOHop_FRESTORE_S) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSAVE_D,(Opcode)FSAVE_D) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FSAVE_D,PLAYDOHop_FSAVE_D) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FRESTORE_D,(Opcode)FRESTORE_D) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FRESTORE_D,PLAYDOHop_FRESTORE_D) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BSAVE,(Opcode)BSAVE) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BSAVE,PLAYDOHop_BSAVE) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRESTORE,(Opcode)BRESTORE) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BRESTORE,PLAYDOHop_BRESTORE) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_PSAVE,(Opcode)PSAVE) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)PSAVE,PLAYDOHop_PSAVE) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_PRESTORE,(Opcode)PRESTORE) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)PRESTORE,PLAYDOHop_PRESTORE) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_SAVEG,(Opcode)SAVEG) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)SAVEG,PLAYDOHop_SAVEG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_RESTOREG,(Opcode)RESTOREG) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)RESTOREG,PLAYDOHop_RESTOREG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSAVEG_S,(Opcode)FSAVEG_S) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FSAVEG_S,PLAYDOHop_FSAVEG_S) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FRESTOREG_S,(Opcode)FRESTOREG_S) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FRESTOREG_S,PLAYDOHop_FRESTOREG_S) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FSAVEG_D,(Opcode)FSAVEG_D) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FSAVEG_D,PLAYDOHop_FSAVEG_D) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_FRESTOREG_D,(Opcode)FRESTOREG_D) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)FRESTOREG_D,PLAYDOHop_FRESTOREG_D) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BSAVEG,(Opcode)BSAVEG) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BSAVEG,PLAYDOHop_BSAVEG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_BRESTOREG,(Opcode)BRESTOREG) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)BRESTOREG,PLAYDOHop_BRESTOREG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_PSAVEG,(Opcode)PSAVEG) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)PSAVEG,PLAYDOHop_PSAVEG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_PRESTOREG,(Opcode)PRESTOREG) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)PRESTOREG,PLAYDOHop_PRESTOREG) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEGBP,(Opcode)MOVEGBP) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)MOVEGBP,PLAYDOHop_MOVEGBP) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_MOVEGCM,(Opcode)MOVEGCM) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode)MOVEGCM,PLAYDOHop_MOVEGCM) ;
}

//
//	Lcode operations
// 
static void el_init_lcode_opcode_misc()
{
   el_lcode_to_elcor_opcode_map.bind(Lop_PROLOGUE,PROLOGUE) ;
   el_elcor_to_lcode_opcode_map.bind(PROLOGUE,Lop_PROLOGUE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_EPILOGUE,EPILOGUE) ;
   el_elcor_to_lcode_opcode_map.bind(EPILOGUE,Lop_EPILOGUE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_DEFINE,DEFINE) ;
   el_elcor_to_lcode_opcode_map.bind(DEFINE,Lop_DEFINE) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_ALLOC,ALLOC) ;
   el_elcor_to_lcode_opcode_map.bind(ALLOC,Lop_ALLOC) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_SIM_DIR,SIM_DIR) ;
   el_elcor_to_lcode_opcode_map.bind(SIM_DIR,Lop_SIM_DIR) ;
 
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_CLEAR,PRED_CLEAR) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_CLEAR,Lop_PRED_CLEAR) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_CLEAR_ALL,Lop_PRED_CLEAR) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_CLEAR_ALL_STATIC,Lop_PRED_CLEAR) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_CLEAR_ALL_ROTATING,Lop_PRED_CLEAR) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_SET,PRED_SET) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_SET,Lop_PRED_SET) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_AND,PRED_AND) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_AND,Lop_PRED_AND) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_COMPL,PRED_COMPL) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_COMPL,Lop_PRED_COMPL) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_LD_BLK,PRED_LOAD_ALL) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_LOAD_ALL,Lop_PRED_LD_BLK) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_PRED_ST_BLK,PRED_STORE_ALL) ;
   el_elcor_to_lcode_opcode_map.bind(PRED_STORE_ALL,Lop_PRED_ST_BLK) ;
}

// SLARSEN: Vector instructions
void el_init_lcode_opcode_vector()
{
   el_lcode_to_elcor_opcode_map.bind(Lop_VADD, VADD_W) ;
   el_elcor_to_lcode_opcode_map.bind(VADD_W, Lop_VADD) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VADD_U, VADDL_W) ;
   el_elcor_to_lcode_opcode_map.bind(VADDL_W, Lop_VADD_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSUB, VSUB_W) ;
   el_elcor_to_lcode_opcode_map.bind(VSUB_W, Lop_VSUB) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSUB_U, VSUBL_W) ;
   el_elcor_to_lcode_opcode_map.bind(VSUBL_W, Lop_VSUB_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMUL, VMPY_W) ;
   el_elcor_to_lcode_opcode_map.bind(VMPY_W, Lop_VMUL) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMUL_U, VMPYL_W) ;
   el_elcor_to_lcode_opcode_map.bind(VMPYL_W, Lop_VMUL_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VDIV, VDIV_W) ;
   el_elcor_to_lcode_opcode_map.bind(VDIV_W, Lop_VDIV) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VDIV_U, VDIVL_W) ;
   el_elcor_to_lcode_opcode_map.bind(VDIVL_W, Lop_VDIV_U) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VREM, VREM_W) ;
   el_elcor_to_lcode_opcode_map.bind(VREM_W, Lop_VREM) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VREM_U, VREML_W) ;
   el_elcor_to_lcode_opcode_map.bind(VREML_W, Lop_VREM_U) ; 
   el_lcode_to_elcor_opcode_map.bind(Lop_VMIN, VMIN_W) ;
   el_elcor_to_lcode_opcode_map.bind(VMIN_W, Lop_VMIN) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMAX, VMAX_W) ;
   el_elcor_to_lcode_opcode_map.bind(VMAX_W, Lop_VMAX) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VOR, VOR_W) ;
   el_elcor_to_lcode_opcode_map.bind(VOR_W, Lop_VOR) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VAND, VAND_W) ;
   el_elcor_to_lcode_opcode_map.bind(VAND_W, Lop_VAND) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_VADD_F, VFADD_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFADD_S, Lop_VADD_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSUB_F, VFSUB_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFSUB_S, Lop_VSUB_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMUL_F, VFMPY_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFMPY_S, Lop_VMUL_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VDIV_F, VFDIV_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFDIV_S, Lop_VDIV_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMAX_F, VFMAX_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFMAX_S, Lop_VMAX_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMIN_F, VFMIN_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFMIN_S, Lop_VMIN_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VABS_F, VFABS_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFABS_S, Lop_VABS_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSQRT_F, VFSQRT_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFSQRT_S, Lop_VSQRT_F) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_VADD_F2, VFADD_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFADD_D, Lop_VADD_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSUB_F2, VFSUB_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFSUB_D, Lop_VSUB_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMUL_F2, VFMPY_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFMPY_D, Lop_VMUL_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VDIV_F2, VFDIV_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFDIV_D, Lop_VDIV_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMAX_F2, VFMAX_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFMAX_D, Lop_VMAX_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMIN_F2, VFMIN_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFMIN_D, Lop_VMIN_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VABS_F2, VFABS_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFABS_D, Lop_VABS_F2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSQRT_F2, VFSQRT_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFSQRT_D, Lop_VSQRT_F2) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_VMOV, VMOVE) ;
   el_elcor_to_lcode_opcode_map.bind(VMOVE, Lop_VMOV) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMOV_F, VMOVEF_S) ;
   el_elcor_to_lcode_opcode_map.bind(VMOVEF_S, Lop_VMOV_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VMOV_F2, VMOVEF_D) ;
   el_elcor_to_lcode_opcode_map.bind(VMOVEF_D, Lop_VMOV_F2) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_VI_VF, VCONVWS) ;
   el_elcor_to_lcode_opcode_map.bind(VCONVWS, Lop_VI_VF) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VI_VF2, VCONVWD) ;
   el_elcor_to_lcode_opcode_map.bind(VCONVWD, Lop_VI_VF2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VF_VI, VCONVSW) ;
   el_elcor_to_lcode_opcode_map.bind(VCONVSW, Lop_VF_VI) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VF2_VI, VCONVDW) ;
   el_elcor_to_lcode_opcode_map.bind(VCONVDW, Lop_VF2_VI) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VF_VF2, VCONVSD) ;
   el_elcor_to_lcode_opcode_map.bind(VCONVSD, Lop_VF_VF2) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VF2_VF, VCONVDS) ;
   el_elcor_to_lcode_opcode_map.bind(VCONVDS, Lop_VF2_VF) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_VI_I, VSMOVE);
   el_elcor_to_lcode_opcode_map.bind(VSMOVE, Lop_VI_I);
   el_lcode_to_elcor_opcode_map.bind(Lop_I_VI, SVMOVE);
   el_elcor_to_lcode_opcode_map.bind(SVMOVE, Lop_I_VI);
   el_lcode_to_elcor_opcode_map.bind(Lop_VF_F, VSFMOVE_S);
   el_elcor_to_lcode_opcode_map.bind(VSFMOVE_S, Lop_VF_F);
   el_lcode_to_elcor_opcode_map.bind(Lop_F_VF, SVFMOVE_S);
   el_elcor_to_lcode_opcode_map.bind(SVFMOVE_S, Lop_F_VF);
   el_lcode_to_elcor_opcode_map.bind(Lop_VF2_F2, VSFMOVE_D);
   el_elcor_to_lcode_opcode_map.bind(VSFMOVE_D, Lop_VF2_F2);
   el_lcode_to_elcor_opcode_map.bind(Lop_F2_VF2, SVFMOVE_D);
   el_elcor_to_lcode_opcode_map.bind(SVFMOVE_D, Lop_F2_VF2);

   el_lcode_to_elcor_opcode_map.bind(Lop_VPERM, VPERM) ;
   el_elcor_to_lcode_opcode_map.bind(VPERM, Lop_VPERM) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VPERM_F, VFPERM_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFPERM_S, Lop_VPERM_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VPERM_F2, VFPERM_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFPERM_D, Lop_VPERM_F2) ;

   el_lcode_to_elcor_opcode_map.bind(Lop_VSPLAT, VSPLAT) ;
   el_elcor_to_lcode_opcode_map.bind(VSPLAT, Lop_VSPLAT) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSPLAT_F, VFSPLAT_S) ;
   el_elcor_to_lcode_opcode_map.bind(VFSPLAT_S, Lop_VSPLAT_F) ;
   el_lcode_to_elcor_opcode_map.bind(Lop_VSPLAT_F2, VFSPLAT_D) ;
   el_elcor_to_lcode_opcode_map.bind(VFSPLAT_D, Lop_VSPLAT_F2) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VEXTRSB,VEXTS_B) ;
   el_elcor_to_lcode_opcode_map.bind(VEXTS_B,PLAYDOHop_VEXTRSB) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VEXTRSH,VEXTS_H) ;
   el_elcor_to_lcode_opcode_map.bind(VEXTS_H,PLAYDOHop_VEXTRSH) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VL_B_C1_C1, (Opcode)VL_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VL_B_C1_C1,PLAYDOHop_VL_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VL_H_C1_C1, (Opcode)VL_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VL_H_C1_C1,PLAYDOHop_VL_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VL_W_C1_C1, (Opcode)VL_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VL_W_C1_C1,PLAYDOHop_VL_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFL_S_C1_C1, (Opcode)VFL_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFL_S_C1_C1,PLAYDOHop_VFL_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFL_D_C1_C1, (Opcode)VFL_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFL_D_C1_C1,PLAYDOHop_VFL_D_C1_C1) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VS_B_C1, (Opcode)VS_B_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VS_B_C1,PLAYDOHop_VS_B_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VS_H_C1, (Opcode)VS_H_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VS_H_C1,PLAYDOHop_VS_H_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VS_W_C1, (Opcode)VS_W_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VS_W_C1,PLAYDOHop_VS_W_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFS_S_C1, (Opcode)VFS_S_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFS_S_C1,PLAYDOHop_VFS_S_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFS_D_C1, (Opcode)VFS_D_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFS_D_C1,PLAYDOHop_VFS_D_C1) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VLE_B_C1_C1, (Opcode)VL_B_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VLE_B_C1_C1,PLAYDOHop_VL_B_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VLE_H_C1_C1, (Opcode)VL_H_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VLE_H_C1_C1,PLAYDOHop_VL_H_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VLE_W_C1_C1, (Opcode)VL_W_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VLE_W_C1_C1,PLAYDOHop_VL_W_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFLE_S_C1_C1, (Opcode)VFL_S_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFLE_S_C1_C1,PLAYDOHop_VFL_S_C1_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFLE_D_C1_C1, (Opcode)VFL_D_C1_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFLE_D_C1_C1,PLAYDOHop_VFL_D_C1_C1) ;

   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VSE_B_C1, (Opcode)VSE_B_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VSE_B_C1,PLAYDOHop_VSE_B_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VSE_H_C1, (Opcode)VSE_H_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VSE_H_C1,PLAYDOHop_VSE_H_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VSE_W_C1, (Opcode)VSE_W_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VSE_W_C1,PLAYDOHop_VSE_W_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFSE_S_C1, (Opcode)VFSE_S_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFSE_S_C1,PLAYDOHop_VFSE_S_C1) ;
   el_lcode_to_elcor_opcode_map.bind(PLAYDOHop_VFSE_D_C1, (Opcode)VFSE_D_C1) ;
   el_elcor_to_lcode_opcode_map.bind((Opcode) VFSE_D_C1,PLAYDOHop_VFSE_D_C1) ;
}
void el_init_lcode_opcode_maps()
{
    el_init_lcode_opcode_maps_load_store();
    el_init_lcode_opcode_maps_data_spec();
    el_init_lcode_opcode_maps_lm();
    el_init_lcode_opcode_cmpr();
    el_init_lcode_opcode_cmpp();
    el_init_lcode_opcode_arith();
    el_init_lcode_opcode_branch();
    el_init_lcode_opcode_lit();
    el_init_lcode_opcode_regalloc();
    el_init_lcode_opcode_misc();

    // SLARSEN: Add vector instructions to the opcode maps
    el_init_lcode_opcode_vector();
}
