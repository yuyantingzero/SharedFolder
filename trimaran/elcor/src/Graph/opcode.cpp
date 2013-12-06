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
//      File:           opcode.cpp
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
#include "op.h"

Hash_map<eString, Opcode> el_string_to_opcode_map(hash_estring, 1000);
Hash_map<Opcode, eString> el_opcode_to_string_map(hash_opcode, 1000) ;
Hash_map<IR_BASE_OPCODE, eString> el_base_opcode_to_string_map(hash_IR_BASE_OPCODE, 200) ;

// SLARSEN: maps scalar opcodes to vector opcodes
Hash_map<int,int> el_opcode_to_vector_opcode_map(hash_int, 50);


char *get_mdes_opcode_string(Op *op)
{
   Opcode opc;
   opc = op->opcode();
   if (opc == CUSTOM)
	return (op->custom_opcode());
   else
	return ((char *)el_opcode_to_string_map.value(opc));
}




//
//	Note the map initialization function is split up into several
//	components for compilation speed reasons, not for correctness,
//	so additions/changes can be made freely.  SAM 5-95
//

static void el_init_elcor_opcode_maps_arithmetic()
{
   el_string_to_opcode_map.bind("NO_OP", NO_OP) ;
   el_opcode_to_string_map.bind(NO_OP, "NO_OP") ;
   el_string_to_opcode_map.bind("M_NO_OP", M_NO_OP) ;
   el_opcode_to_string_map.bind(M_NO_OP, "M_NO_OP") ;
   el_string_to_opcode_map.bind("ABS_W", ABS_W) ;
   el_opcode_to_string_map.bind(ABS_W, "ABS_W") ;
   el_string_to_opcode_map.bind("ADD_W", ADD_W) ;
   el_opcode_to_string_map.bind(ADD_W, "ADD_W") ;
   el_string_to_opcode_map.bind("ADD_CARRY_W", ADD_CARRY_W) ;
   el_opcode_to_string_map.bind(ADD_CARRY_W, "ADD_CARRY_W") ;
   el_string_to_opcode_map.bind("ADDL_CARRY_W", ADDL_CARRY_W) ;
   el_opcode_to_string_map.bind(ADDL_CARRY_W, "ADDL_CARRY_W") ;
   el_string_to_opcode_map.bind("SUB_CARRY_W", SUB_CARRY_W) ;
   el_opcode_to_string_map.bind(SUB_CARRY_W, "SUB_CARRY_W") ;
   el_string_to_opcode_map.bind("SUBL_CARRY_W", SUBL_CARRY_W) ;
   el_opcode_to_string_map.bind(SUBL_CARRY_W, "SUBL_CARRY_W") ;
   el_string_to_opcode_map.bind("ADDL_W", ADDL_W) ;
   el_opcode_to_string_map.bind(ADDL_W, "ADDL_W") ;
   el_string_to_opcode_map.bind("AND_W", AND_W) ;
   el_opcode_to_string_map.bind(AND_W, "AND_W") ;
   el_string_to_opcode_map.bind("ANDCM_W", ANDCM_W) ;
   el_opcode_to_string_map.bind(ANDCM_W, "ANDCM_W") ;
   el_string_to_opcode_map.bind("DIV_W", DIV_W) ;
   el_opcode_to_string_map.bind(DIV_W, "DIV_W") ;
   el_string_to_opcode_map.bind("DIVL_W", DIVL_W) ;
   el_opcode_to_string_map.bind(DIVL_W, "DIVL_W") ;
   el_string_to_opcode_map.bind("MAX_W", MAX_W) ;
   el_opcode_to_string_map.bind(MAX_W, "MAX_W") ;
   el_string_to_opcode_map.bind("MAXL_W", MAXL_W) ;
   el_opcode_to_string_map.bind(MAXL_W, "MAXL_W") ;
   el_string_to_opcode_map.bind("MIN_W", MIN_W) ;
   el_opcode_to_string_map.bind(MIN_W, "MIN_W") ;
   el_string_to_opcode_map.bind("MINL_W", MINL_W) ;
   el_opcode_to_string_map.bind(MINL_W, "MINL_W") ;
   el_string_to_opcode_map.bind("MPY_W", MPY_W) ;
   el_opcode_to_string_map.bind(MPY_W, "MPY_W") ;
   el_string_to_opcode_map.bind("MPYL_W", MPYL_W) ;
   el_opcode_to_string_map.bind(MPYL_W, "MPYL_W") ;
   el_string_to_opcode_map.bind("MPY_WIDE_W", MPY_WIDE_W) ;
   el_opcode_to_string_map.bind(MPY_WIDE_W, "MPY_WIDE_W") ;
   el_string_to_opcode_map.bind("MPYL_WIDE_W", MPYL_WIDE_W) ;
   el_opcode_to_string_map.bind(MPYL_WIDE_W, "MPYL_WIDE_W") ;
   el_string_to_opcode_map.bind("MPYADD_W", MPYADD_W) ;
   el_opcode_to_string_map.bind(MPYADD_W, "MPYADD_W") ;
   el_string_to_opcode_map.bind("MPYADDN_W", MPYADDN_W) ;
   el_opcode_to_string_map.bind(MPYADDN_W, "MPYADDN_W") ;
   el_string_to_opcode_map.bind("MPYRSUB_W", MPYRSUB_W) ;
   el_opcode_to_string_map.bind(MPYRSUB_W, "MPYRSUB_W") ;
   el_string_to_opcode_map.bind("MPYSUB_W", MPYSUB_W) ;
   el_opcode_to_string_map.bind(MPYSUB_W, "MPYSUB_W") ;
   el_string_to_opcode_map.bind("NAND_W", NAND_W) ;
   el_opcode_to_string_map.bind(NAND_W, "NAND_W") ;
   el_string_to_opcode_map.bind("NOR_W", NOR_W) ;
   el_opcode_to_string_map.bind(NOR_W, "NOR_W") ;
   el_string_to_opcode_map.bind("OR_W", OR_W) ;
   el_opcode_to_string_map.bind(OR_W, "OR_W") ;
   el_string_to_opcode_map.bind("ORCM_W", ORCM_W) ;
   el_opcode_to_string_map.bind(ORCM_W, "ORCM_W") ;
   el_string_to_opcode_map.bind("REM_W", REM_W) ;
   el_opcode_to_string_map.bind(REM_W, "REM_W") ;
   el_string_to_opcode_map.bind("REML_W", REML_W) ;
   el_opcode_to_string_map.bind(REML_W, "REML_W") ;
   el_string_to_opcode_map.bind("SH1ADDL_W", SH1ADDL_W) ;
   el_opcode_to_string_map.bind(SH1ADDL_W, "SH1ADDL_W") ;
   el_string_to_opcode_map.bind("SH2ADDL_W", SH2ADDL_W) ;
   el_opcode_to_string_map.bind(SH2ADDL_W, "SH2ADDL_W") ;
   el_string_to_opcode_map.bind("SH3ADDL_W", SH3ADDL_W) ;
   el_opcode_to_string_map.bind(SH3ADDL_W, "SH3ADDL_W") ;
   el_string_to_opcode_map.bind("SHL_W", SHL_W) ;
   el_opcode_to_string_map.bind(SHL_W, "SHL_W") ;
   el_string_to_opcode_map.bind("SHR_W", SHR_W) ;
   el_opcode_to_string_map.bind(SHR_W, "SHR_W") ;
   el_string_to_opcode_map.bind("SHLA_W", SHLA_W) ;
   el_opcode_to_string_map.bind(SHLA_W, "SHLA_W") ;
   el_string_to_opcode_map.bind("SHRA_W", SHRA_W) ;
   el_opcode_to_string_map.bind(SHRA_W, "SHRA_W") ;
   el_string_to_opcode_map.bind("SUB_W", SUB_W) ;
   el_opcode_to_string_map.bind(SUB_W, "SUB_W") ;
   el_string_to_opcode_map.bind("SUBL_W", SUBL_W) ;
   el_opcode_to_string_map.bind(SUBL_W, "SUBL_W") ;
   el_string_to_opcode_map.bind("XOR_W", XOR_W) ;
   el_opcode_to_string_map.bind(XOR_W, "XOR_W") ;
   el_string_to_opcode_map.bind("XORCM_W", XORCM_W) ;
   el_opcode_to_string_map.bind(XORCM_W, "XORCM_W") ;

   // Saturation Operations
   el_string_to_opcode_map.bind("SADD1_W", SADD1_W) ;
   el_opcode_to_string_map.bind(SADD1_W, "SADD1_W") ;
   el_string_to_opcode_map.bind("SADD2_W", SADD2_W) ;
   el_opcode_to_string_map.bind(SADD2_W, "SADD2_W") ;
   el_string_to_opcode_map.bind("SADD3_W", SADD3_W) ;
   el_opcode_to_string_map.bind(SADD3_W, "SADD3_W") ;
   el_string_to_opcode_map.bind("SADD4_W", SADD4_W) ;
   el_opcode_to_string_map.bind(SADD4_W, "SADD4_W") ;
   el_string_to_opcode_map.bind("SADD5_W", SADD5_W) ;
   el_opcode_to_string_map.bind(SADD5_W, "SADD5_W") ;
   el_string_to_opcode_map.bind("SADD6_W", SADD6_W) ;
   el_opcode_to_string_map.bind(SADD6_W, "SADD6_W") ;
   el_string_to_opcode_map.bind("SADD7_W", SADD7_W) ;
   el_opcode_to_string_map.bind(SADD7_W, "SADD7_W") ;
   el_string_to_opcode_map.bind("SADD8_W", SADD8_W) ;
   el_opcode_to_string_map.bind(SADD8_W, "SADD8_W") ;
   el_string_to_opcode_map.bind("SADD9_W", SADD9_W) ;
   el_opcode_to_string_map.bind(SADD9_W, "SADD9_W") ;
   el_string_to_opcode_map.bind("SADD10_W", SADD10_W) ;
   el_opcode_to_string_map.bind(SADD10_W, "SADD10_W") ;
   el_string_to_opcode_map.bind("SADD11_W", SADD11_W) ;
   el_opcode_to_string_map.bind(SADD11_W, "SADD11_W") ;
   el_string_to_opcode_map.bind("SADD12_W", SADD12_W) ;
   el_opcode_to_string_map.bind(SADD12_W, "SADD12_W") ;
   el_string_to_opcode_map.bind("SADD13_W", SADD13_W) ;
   el_opcode_to_string_map.bind(SADD13_W, "SADD13_W") ;
   el_string_to_opcode_map.bind("SADD14_W", SADD14_W) ;
   el_opcode_to_string_map.bind(SADD14_W, "SADD14_W") ;
   el_string_to_opcode_map.bind("SADD15_W", SADD15_W) ;
   el_opcode_to_string_map.bind(SADD15_W, "SADD15_W") ;
   el_string_to_opcode_map.bind("SADD16_W", SADD16_W) ;
   el_opcode_to_string_map.bind(SADD16_W, "SADD16_W") ;
   el_string_to_opcode_map.bind("SADD17_W", SADD17_W) ;
   el_opcode_to_string_map.bind(SADD17_W, "SADD17_W") ;
   el_string_to_opcode_map.bind("SADD18_W", SADD18_W) ;
   el_opcode_to_string_map.bind(SADD18_W, "SADD18_W") ;
   el_string_to_opcode_map.bind("SADD19_W", SADD19_W) ;
   el_opcode_to_string_map.bind(SADD19_W, "SADD19_W") ;
   el_string_to_opcode_map.bind("SADD20_W", SADD20_W) ;
   el_opcode_to_string_map.bind(SADD20_W, "SADD20_W") ;
   el_string_to_opcode_map.bind("SADD21_W", SADD21_W) ;
   el_opcode_to_string_map.bind(SADD21_W, "SADD21_W") ;
   el_string_to_opcode_map.bind("SADD22_W", SADD22_W) ;
   el_opcode_to_string_map.bind(SADD22_W, "SADD22_W") ;
   el_string_to_opcode_map.bind("SADD23_W", SADD23_W) ;
   el_opcode_to_string_map.bind(SADD23_W, "SADD23_W") ;
   el_string_to_opcode_map.bind("SADD24_W", SADD24_W) ;
   el_opcode_to_string_map.bind(SADD24_W, "SADD24_W") ;
   el_string_to_opcode_map.bind("SADD25_W", SADD25_W) ;
   el_opcode_to_string_map.bind(SADD25_W, "SADD25_W") ;
   el_string_to_opcode_map.bind("SADD26_W", SADD26_W) ;
   el_opcode_to_string_map.bind(SADD26_W, "SADD26_W") ;
   el_string_to_opcode_map.bind("SADD27_W", SADD27_W) ;
   el_opcode_to_string_map.bind(SADD27_W, "SADD27_W") ;
   el_string_to_opcode_map.bind("SADD28_W", SADD28_W) ;
   el_opcode_to_string_map.bind(SADD28_W, "SADD28_W") ;
   el_string_to_opcode_map.bind("SADD29_W", SADD29_W) ;
   el_opcode_to_string_map.bind(SADD29_W, "SADD29_W") ;
   el_string_to_opcode_map.bind("SADD30_W", SADD30_W) ;
   el_opcode_to_string_map.bind(SADD30_W, "SADD30_W") ;
   el_string_to_opcode_map.bind("SADD31_W", SADD31_W) ;
   el_opcode_to_string_map.bind(SADD31_W, "SADD31_W") ;
   el_string_to_opcode_map.bind("SADD32_W", SADD32_W) ;
   el_opcode_to_string_map.bind(SADD32_W, "SADD32_W") ;

   el_string_to_opcode_map.bind("SADDL1_W", SADDL1_W) ;
   el_opcode_to_string_map.bind(SADDL1_W, "SADDL1_W") ;
   el_string_to_opcode_map.bind("SADDL2_W", SADDL2_W) ;
   el_opcode_to_string_map.bind(SADDL2_W, "SADDL2_W") ;
   el_string_to_opcode_map.bind("SADDL3_W", SADDL3_W) ;
   el_opcode_to_string_map.bind(SADDL3_W, "SADDL3_W") ;
   el_string_to_opcode_map.bind("SADDL4_W", SADDL4_W) ;
   el_opcode_to_string_map.bind(SADDL4_W, "SADDL4_W") ;
   el_string_to_opcode_map.bind("SADDL5_W", SADDL5_W) ;
   el_opcode_to_string_map.bind(SADDL5_W, "SADDL5_W") ;
   el_string_to_opcode_map.bind("SADDL6_W", SADDL6_W) ;
   el_opcode_to_string_map.bind(SADDL6_W, "SADDL6_W") ;
   el_string_to_opcode_map.bind("SADDL7_W", SADDL7_W) ;
   el_opcode_to_string_map.bind(SADDL7_W, "SADDL7_W") ;
   el_string_to_opcode_map.bind("SADDL8_W", SADDL8_W) ;
   el_opcode_to_string_map.bind(SADDL8_W, "SADDL8_W") ;
   el_string_to_opcode_map.bind("SADDL9_W", SADDL9_W) ;
   el_opcode_to_string_map.bind(SADDL9_W, "SADDL9_W") ;
   el_string_to_opcode_map.bind("SADDL10_W", SADDL10_W) ;
   el_opcode_to_string_map.bind(SADDL10_W, "SADDL10_W") ;
   el_string_to_opcode_map.bind("SADDL11_W", SADDL11_W) ;
   el_opcode_to_string_map.bind(SADDL11_W, "SADDL11_W") ;
   el_string_to_opcode_map.bind("SADDL12_W", SADDL12_W) ;
   el_opcode_to_string_map.bind(SADDL12_W, "SADDL12_W") ;
   el_string_to_opcode_map.bind("SADDL13_W", SADDL13_W) ;
   el_opcode_to_string_map.bind(SADDL13_W, "SADDL13_W") ;
   el_string_to_opcode_map.bind("SADDL14_W", SADDL14_W) ;
   el_opcode_to_string_map.bind(SADDL14_W, "SADDL14_W") ;
   el_string_to_opcode_map.bind("SADDL15_W", SADDL15_W) ;
   el_opcode_to_string_map.bind(SADDL15_W, "SADDL15_W") ;
   el_string_to_opcode_map.bind("SADDL16_W", SADDL16_W) ;
   el_opcode_to_string_map.bind(SADDL16_W, "SADDL16_W") ;
   el_string_to_opcode_map.bind("SADDL17_W", SADDL17_W) ;
   el_opcode_to_string_map.bind(SADDL17_W, "SADDL17_W") ;
   el_string_to_opcode_map.bind("SADDL18_W", SADDL18_W) ;
   el_opcode_to_string_map.bind(SADDL18_W, "SADDL18_W") ;
   el_string_to_opcode_map.bind("SADDL19_W", SADDL19_W) ;
   el_opcode_to_string_map.bind(SADDL19_W, "SADDL19_W") ;
   el_string_to_opcode_map.bind("SADDL20_W", SADDL20_W) ;
   el_opcode_to_string_map.bind(SADDL20_W, "SADDL20_W") ;
   el_string_to_opcode_map.bind("SADDL21_W", SADDL21_W) ;
   el_opcode_to_string_map.bind(SADDL21_W, "SADDL21_W") ;
   el_string_to_opcode_map.bind("SADDL22_W", SADDL22_W) ;
   el_opcode_to_string_map.bind(SADDL22_W, "SADDL22_W") ;
   el_string_to_opcode_map.bind("SADDL23_W", SADDL23_W) ;
   el_opcode_to_string_map.bind(SADDL23_W, "SADDL23_W") ;
   el_string_to_opcode_map.bind("SADDL24_W", SADDL24_W) ;
   el_opcode_to_string_map.bind(SADDL24_W, "SADDL24_W") ;
   el_string_to_opcode_map.bind("SADDL25_W", SADDL25_W) ;
   el_opcode_to_string_map.bind(SADDL25_W, "SADDL25_W") ;
   el_string_to_opcode_map.bind("SADDL26_W", SADDL26_W) ;
   el_opcode_to_string_map.bind(SADDL26_W, "SADDL26_W") ;
   el_string_to_opcode_map.bind("SADDL27_W", SADDL27_W) ;
   el_opcode_to_string_map.bind(SADDL27_W, "SADDL27_W") ;
   el_string_to_opcode_map.bind("SADDL28_W", SADDL28_W) ;
   el_opcode_to_string_map.bind(SADDL28_W, "SADDL28_W") ;
   el_string_to_opcode_map.bind("SADDL29_W", SADDL29_W) ;
   el_opcode_to_string_map.bind(SADDL29_W, "SADDL29_W") ;
   el_string_to_opcode_map.bind("SADDL30_W", SADDL30_W) ;
   el_opcode_to_string_map.bind(SADDL30_W, "SADDL30_W") ;
   el_string_to_opcode_map.bind("SADDL31_W", SADDL31_W) ;
   el_opcode_to_string_map.bind(SADDL31_W, "SADDL31_W") ;
   el_string_to_opcode_map.bind("SADDL32_W", SADDL32_W) ;
   el_opcode_to_string_map.bind(SADDL32_W, "SADDL32_W") ;

   el_string_to_opcode_map.bind("SSUB1_W", SSUB1_W) ;
   el_opcode_to_string_map.bind(SSUB1_W, "SSUB1_W") ;
   el_string_to_opcode_map.bind("SSUB2_W", SSUB2_W) ;
   el_opcode_to_string_map.bind(SSUB2_W, "SSUB2_W") ;
   el_string_to_opcode_map.bind("SSUB3_W", SSUB3_W) ;
   el_opcode_to_string_map.bind(SSUB3_W, "SSUB3_W") ;
   el_string_to_opcode_map.bind("SSUB4_W", SSUB4_W) ;
   el_opcode_to_string_map.bind(SSUB4_W, "SSUB4_W") ;
   el_string_to_opcode_map.bind("SSUB5_W", SSUB5_W) ;
   el_opcode_to_string_map.bind(SSUB5_W, "SSUB5_W") ;
   el_string_to_opcode_map.bind("SSUB6_W", SSUB6_W) ;
   el_opcode_to_string_map.bind(SSUB6_W, "SSUB6_W") ;
   el_string_to_opcode_map.bind("SSUB7_W", SSUB7_W) ;
   el_opcode_to_string_map.bind(SSUB7_W, "SSUB7_W") ;
   el_string_to_opcode_map.bind("SSUB8_W", SSUB8_W) ;
   el_opcode_to_string_map.bind(SSUB8_W, "SSUB8_W") ;
   el_string_to_opcode_map.bind("SSUB9_W", SSUB9_W) ;
   el_opcode_to_string_map.bind(SSUB9_W, "SSUB9_W") ;
   el_string_to_opcode_map.bind("SSUB10_W", SSUB10_W) ;
   el_opcode_to_string_map.bind(SSUB10_W, "SSUB10_W") ;
   el_string_to_opcode_map.bind("SSUB11_W", SSUB11_W) ;
   el_opcode_to_string_map.bind(SSUB11_W, "SSUB11_W") ;
   el_string_to_opcode_map.bind("SSUB12_W", SSUB12_W) ;
   el_opcode_to_string_map.bind(SSUB12_W, "SSUB12_W") ;
   el_string_to_opcode_map.bind("SSUB13_W", SSUB13_W) ;
   el_opcode_to_string_map.bind(SSUB13_W, "SSUB13_W") ;
   el_string_to_opcode_map.bind("SSUB14_W", SSUB14_W) ;
   el_opcode_to_string_map.bind(SSUB14_W, "SSUB14_W") ;
   el_string_to_opcode_map.bind("SSUB15_W", SSUB15_W) ;
   el_opcode_to_string_map.bind(SSUB15_W, "SSUB15_W") ;
   el_string_to_opcode_map.bind("SSUB16_W", SSUB16_W) ;
   el_opcode_to_string_map.bind(SSUB16_W, "SSUB16_W") ;
   el_string_to_opcode_map.bind("SSUB17_W", SSUB17_W) ;
   el_opcode_to_string_map.bind(SSUB17_W, "SSUB17_W") ;
   el_string_to_opcode_map.bind("SSUB18_W", SSUB18_W) ;
   el_opcode_to_string_map.bind(SSUB18_W, "SSUB18_W") ;
   el_string_to_opcode_map.bind("SSUB19_W", SSUB19_W) ;
   el_opcode_to_string_map.bind(SSUB19_W, "SSUB19_W") ;
   el_string_to_opcode_map.bind("SSUB20_W", SSUB20_W) ;
   el_opcode_to_string_map.bind(SSUB20_W, "SSUB20_W") ;
   el_string_to_opcode_map.bind("SSUB21_W", SSUB21_W) ;
   el_opcode_to_string_map.bind(SSUB21_W, "SSUB21_W") ;
   el_string_to_opcode_map.bind("SSUB22_W", SSUB22_W) ;
   el_opcode_to_string_map.bind(SSUB22_W, "SSUB22_W") ;
   el_string_to_opcode_map.bind("SSUB23_W", SSUB23_W) ;
   el_opcode_to_string_map.bind(SSUB23_W, "SSUB23_W") ;
   el_string_to_opcode_map.bind("SSUB24_W", SSUB24_W) ;
   el_opcode_to_string_map.bind(SSUB24_W, "SSUB24_W") ;
   el_string_to_opcode_map.bind("SSUB25_W", SSUB25_W) ;
   el_opcode_to_string_map.bind(SSUB25_W, "SSUB25_W") ;
   el_string_to_opcode_map.bind("SSUB26_W", SSUB26_W) ;
   el_opcode_to_string_map.bind(SSUB26_W, "SSUB26_W") ;
   el_string_to_opcode_map.bind("SSUB27_W", SSUB27_W) ;
   el_opcode_to_string_map.bind(SSUB27_W, "SSUB27_W") ;
   el_string_to_opcode_map.bind("SSUB28_W", SSUB28_W) ;
   el_opcode_to_string_map.bind(SSUB28_W, "SSUB28_W") ;
   el_string_to_opcode_map.bind("SSUB29_W", SSUB29_W) ;
   el_opcode_to_string_map.bind(SSUB29_W, "SSUB29_W") ;
   el_string_to_opcode_map.bind("SSUB30_W", SSUB30_W) ;
   el_opcode_to_string_map.bind(SSUB30_W, "SSUB30_W") ;
   el_string_to_opcode_map.bind("SSUB31_W", SSUB31_W) ;
   el_opcode_to_string_map.bind(SSUB31_W, "SSUB31_W") ;
   el_string_to_opcode_map.bind("SSUB32_W", SSUB32_W) ;
   el_opcode_to_string_map.bind(SSUB32_W, "SSUB32_W") ;

   el_string_to_opcode_map.bind("SSUBL1_W", SSUBL1_W) ;
   el_opcode_to_string_map.bind(SSUBL1_W, "SSUBL1_W") ;
   el_string_to_opcode_map.bind("SSUBL2_W", SSUBL2_W) ;
   el_opcode_to_string_map.bind(SSUBL2_W, "SSUBL2_W") ;
   el_string_to_opcode_map.bind("SSUBL3_W", SSUBL3_W) ;
   el_opcode_to_string_map.bind(SSUBL3_W, "SSUBL3_W") ;
   el_string_to_opcode_map.bind("SSUBL4_W", SSUBL4_W) ;
   el_opcode_to_string_map.bind(SSUBL4_W, "SSUBL4_W") ;
   el_string_to_opcode_map.bind("SSUBL5_W", SSUBL5_W) ;
   el_opcode_to_string_map.bind(SSUBL5_W, "SSUBL5_W") ;
   el_string_to_opcode_map.bind("SSUBL6_W", SSUBL6_W) ;
   el_opcode_to_string_map.bind(SSUBL6_W, "SSUBL6_W") ;
   el_string_to_opcode_map.bind("SSUBL7_W", SSUBL7_W) ;
   el_opcode_to_string_map.bind(SSUBL7_W, "SSUBL7_W") ;
   el_string_to_opcode_map.bind("SSUBL8_W", SSUBL8_W) ;
   el_opcode_to_string_map.bind(SSUBL8_W, "SSUBL8_W") ;
   el_string_to_opcode_map.bind("SSUBL9_W", SSUBL9_W) ;
   el_opcode_to_string_map.bind(SSUBL9_W, "SSUBL9_W") ;
   el_string_to_opcode_map.bind("SSUBL10_W", SSUBL10_W) ;
   el_opcode_to_string_map.bind(SSUBL10_W, "SSUBL10_W") ;
   el_string_to_opcode_map.bind("SSUBL11_W", SSUBL11_W) ;
   el_opcode_to_string_map.bind(SSUBL11_W, "SSUBL11_W") ;
   el_string_to_opcode_map.bind("SSUBL12_W", SSUBL12_W) ;
   el_opcode_to_string_map.bind(SSUBL12_W, "SSUBL12_W") ;
   el_string_to_opcode_map.bind("SSUBL13_W", SSUBL13_W) ;
   el_opcode_to_string_map.bind(SSUBL13_W, "SSUBL13_W") ;
   el_string_to_opcode_map.bind("SSUBL14_W", SSUBL14_W) ;
   el_opcode_to_string_map.bind(SSUBL14_W, "SSUBL14_W") ;
   el_string_to_opcode_map.bind("SSUBL15_W", SSUBL15_W) ;
   el_opcode_to_string_map.bind(SSUBL15_W, "SSUBL15_W") ;
   el_string_to_opcode_map.bind("SSUBL16_W", SSUBL16_W) ;
   el_opcode_to_string_map.bind(SSUBL16_W, "SSUBL16_W") ;
   el_string_to_opcode_map.bind("SSUBL17_W", SSUBL17_W) ;
   el_opcode_to_string_map.bind(SSUBL17_W, "SSUBL17_W") ;
   el_string_to_opcode_map.bind("SSUBL18_W", SSUBL18_W) ;
   el_opcode_to_string_map.bind(SSUBL18_W, "SSUBL18_W") ;
   el_string_to_opcode_map.bind("SSUBL19_W", SSUBL19_W) ;
   el_opcode_to_string_map.bind(SSUBL19_W, "SSUBL19_W") ;
   el_string_to_opcode_map.bind("SSUBL20_W", SSUBL20_W) ;
   el_opcode_to_string_map.bind(SSUBL20_W, "SSUBL20_W") ;
   el_string_to_opcode_map.bind("SSUBL21_W", SSUBL21_W) ;
   el_opcode_to_string_map.bind(SSUBL21_W, "SSUBL21_W") ;
   el_string_to_opcode_map.bind("SSUBL22_W", SSUBL22_W) ;
   el_opcode_to_string_map.bind(SSUBL22_W, "SSUBL22_W") ;
   el_string_to_opcode_map.bind("SSUBL23_W", SSUBL23_W) ;
   el_opcode_to_string_map.bind(SSUBL23_W, "SSUBL23_W") ;
   el_string_to_opcode_map.bind("SSUBL24_W", SSUBL24_W) ;
   el_opcode_to_string_map.bind(SSUBL24_W, "SSUBL24_W") ;
   el_string_to_opcode_map.bind("SSUBL25_W", SSUBL25_W) ;
   el_opcode_to_string_map.bind(SSUBL25_W, "SSUBL25_W") ;
   el_string_to_opcode_map.bind("SSUBL26_W", SSUBL26_W) ;
   el_opcode_to_string_map.bind(SSUBL26_W, "SSUBL26_W") ;
   el_string_to_opcode_map.bind("SSUBL27_W", SSUBL27_W) ;
   el_opcode_to_string_map.bind(SSUBL27_W, "SSUBL27_W") ;
   el_string_to_opcode_map.bind("SSUBL28_W", SSUBL28_W) ;
   el_opcode_to_string_map.bind(SSUBL28_W, "SSUBL28_W") ;
   el_string_to_opcode_map.bind("SSUBL29_W", SSUBL29_W) ;
   el_opcode_to_string_map.bind(SSUBL29_W, "SSUBL29_W") ;
   el_string_to_opcode_map.bind("SSUBL30_W", SSUBL30_W) ;
   el_opcode_to_string_map.bind(SSUBL30_W, "SSUBL30_W") ;
   el_string_to_opcode_map.bind("SSUBL31_W", SSUBL31_W) ;
   el_opcode_to_string_map.bind(SSUBL31_W, "SSUBL31_W") ;
   el_string_to_opcode_map.bind("SSUBL32_W", SSUBL32_W) ;
   el_opcode_to_string_map.bind(SSUBL32_W, "SSUBL32_W") ;

   el_string_to_opcode_map.bind("SMPY1_W", SMPY1_W) ;
   el_opcode_to_string_map.bind(SMPY1_W, "SMPY1_W") ;
   el_string_to_opcode_map.bind("SMPY2_W", SMPY2_W) ;
   el_opcode_to_string_map.bind(SMPY2_W, "SMPY2_W") ;
   el_string_to_opcode_map.bind("SMPY3_W", SMPY3_W) ;
   el_opcode_to_string_map.bind(SMPY3_W, "SMPY3_W") ;
   el_string_to_opcode_map.bind("SMPY4_W", SMPY4_W) ;
   el_opcode_to_string_map.bind(SMPY4_W, "SMPY4_W") ;
   el_string_to_opcode_map.bind("SMPY5_W", SMPY5_W) ;
   el_opcode_to_string_map.bind(SMPY5_W, "SMPY5_W") ;
   el_string_to_opcode_map.bind("SMPY6_W", SMPY6_W) ;
   el_opcode_to_string_map.bind(SMPY6_W, "SMPY6_W") ;
   el_string_to_opcode_map.bind("SMPY7_W", SMPY7_W) ;
   el_opcode_to_string_map.bind(SMPY7_W, "SMPY7_W") ;
   el_string_to_opcode_map.bind("SMPY8_W", SMPY8_W) ;
   el_opcode_to_string_map.bind(SMPY8_W, "SMPY8_W") ;
   el_string_to_opcode_map.bind("SMPY9_W", SMPY9_W) ;
   el_opcode_to_string_map.bind(SMPY9_W, "SMPY9_W") ;
   el_string_to_opcode_map.bind("SMPY10_W", SMPY10_W) ;
   el_opcode_to_string_map.bind(SMPY10_W, "SMPY10_W") ;
   el_string_to_opcode_map.bind("SMPY11_W", SMPY11_W) ;
   el_opcode_to_string_map.bind(SMPY11_W, "SMPY11_W") ;
   el_string_to_opcode_map.bind("SMPY12_W", SMPY12_W) ;
   el_opcode_to_string_map.bind(SMPY12_W, "SMPY12_W") ;
   el_string_to_opcode_map.bind("SMPY13_W", SMPY13_W) ;
   el_opcode_to_string_map.bind(SMPY13_W, "SMPY13_W") ;
   el_string_to_opcode_map.bind("SMPY14_W", SMPY14_W) ;
   el_opcode_to_string_map.bind(SMPY14_W, "SMPY14_W") ;
   el_string_to_opcode_map.bind("SMPY15_W", SMPY15_W) ;
   el_opcode_to_string_map.bind(SMPY15_W, "SMPY15_W") ;
   el_string_to_opcode_map.bind("SMPY16_W", SMPY16_W) ;
   el_opcode_to_string_map.bind(SMPY16_W, "SMPY16_W") ;
   el_string_to_opcode_map.bind("SMPY17_W", SMPY17_W) ;
   el_opcode_to_string_map.bind(SMPY17_W, "SMPY17_W") ;
   el_string_to_opcode_map.bind("SMPY18_W", SMPY18_W) ;
   el_opcode_to_string_map.bind(SMPY18_W, "SMPY18_W") ;
   el_string_to_opcode_map.bind("SMPY19_W", SMPY19_W) ;
   el_opcode_to_string_map.bind(SMPY19_W, "SMPY19_W") ;
   el_string_to_opcode_map.bind("SMPY20_W", SMPY20_W) ;
   el_opcode_to_string_map.bind(SMPY20_W, "SMPY20_W") ;
   el_string_to_opcode_map.bind("SMPY21_W", SMPY21_W) ;
   el_opcode_to_string_map.bind(SMPY21_W, "SMPY21_W") ;
   el_string_to_opcode_map.bind("SMPY22_W", SMPY22_W) ;
   el_opcode_to_string_map.bind(SMPY22_W, "SMPY22_W") ;
   el_string_to_opcode_map.bind("SMPY23_W", SMPY23_W) ;
   el_opcode_to_string_map.bind(SMPY23_W, "SMPY23_W") ;
   el_string_to_opcode_map.bind("SMPY24_W", SMPY24_W) ;
   el_opcode_to_string_map.bind(SMPY24_W, "SMPY24_W") ;
   el_string_to_opcode_map.bind("SMPY25_W", SMPY25_W) ;
   el_opcode_to_string_map.bind(SMPY25_W, "SMPY25_W") ;
   el_string_to_opcode_map.bind("SMPY26_W", SMPY26_W) ;
   el_opcode_to_string_map.bind(SMPY26_W, "SMPY26_W") ;
   el_string_to_opcode_map.bind("SMPY27_W", SMPY27_W) ;
   el_opcode_to_string_map.bind(SMPY27_W, "SMPY27_W") ;
   el_string_to_opcode_map.bind("SMPY28_W", SMPY28_W) ;
   el_opcode_to_string_map.bind(SMPY28_W, "SMPY28_W") ;
   el_string_to_opcode_map.bind("SMPY29_W", SMPY29_W) ;
   el_opcode_to_string_map.bind(SMPY29_W, "SMPY29_W") ;
   el_string_to_opcode_map.bind("SMPY30_W", SMPY30_W) ;
   el_opcode_to_string_map.bind(SMPY30_W, "SMPY30_W") ;
   el_string_to_opcode_map.bind("SMPY31_W", SMPY31_W) ;
   el_opcode_to_string_map.bind(SMPY31_W, "SMPY31_W") ;
   el_string_to_opcode_map.bind("SMPY32_W", SMPY32_W) ;
   el_opcode_to_string_map.bind(SMPY32_W, "SMPY32_W") ;

   el_string_to_opcode_map.bind("SMPYL1_W", SMPYL1_W) ;
   el_opcode_to_string_map.bind(SMPYL1_W, "SMPYL1_W") ;
   el_string_to_opcode_map.bind("SMPYL2_W", SMPYL2_W) ;
   el_opcode_to_string_map.bind(SMPYL2_W, "SMPYL2_W") ;
   el_string_to_opcode_map.bind("SMPYL3_W", SMPYL3_W) ;
   el_opcode_to_string_map.bind(SMPYL3_W, "SMPYL3_W") ;
   el_string_to_opcode_map.bind("SMPYL4_W", SMPYL4_W) ;
   el_opcode_to_string_map.bind(SMPYL4_W, "SMPYL4_W") ;
   el_string_to_opcode_map.bind("SMPYL5_W", SMPYL5_W) ;
   el_opcode_to_string_map.bind(SMPYL5_W, "SMPYL5_W") ;
   el_string_to_opcode_map.bind("SMPYL6_W", SMPYL6_W) ;
   el_opcode_to_string_map.bind(SMPYL6_W, "SMPYL6_W") ;
   el_string_to_opcode_map.bind("SMPYL7_W", SMPYL7_W) ;
   el_opcode_to_string_map.bind(SMPYL7_W, "SMPYL7_W") ;
   el_string_to_opcode_map.bind("SMPYL8_W", SMPYL8_W) ;
   el_opcode_to_string_map.bind(SMPYL8_W, "SMPYL8_W") ;
   el_string_to_opcode_map.bind("SMPYL9_W", SMPYL9_W) ;
   el_opcode_to_string_map.bind(SMPYL9_W, "SMPYL9_W") ;
   el_string_to_opcode_map.bind("SMPYL10_W", SMPYL10_W) ;
   el_opcode_to_string_map.bind(SMPYL10_W, "SMPYL10_W") ;
   el_string_to_opcode_map.bind("SMPYL11_W", SMPYL11_W) ;
   el_opcode_to_string_map.bind(SMPYL11_W, "SMPYL11_W") ;
   el_string_to_opcode_map.bind("SMPYL12_W", SMPYL12_W) ;
   el_opcode_to_string_map.bind(SMPYL12_W, "SMPYL12_W") ;
   el_string_to_opcode_map.bind("SMPYL13_W", SMPYL13_W) ;
   el_opcode_to_string_map.bind(SMPYL13_W, "SMPYL13_W") ;
   el_string_to_opcode_map.bind("SMPYL14_W", SMPYL14_W) ;
   el_opcode_to_string_map.bind(SMPYL14_W, "SMPYL14_W") ;
   el_string_to_opcode_map.bind("SMPYL15_W", SMPYL15_W) ;
   el_opcode_to_string_map.bind(SMPYL15_W, "SMPYL15_W") ;
   el_string_to_opcode_map.bind("SMPYL16_W", SMPYL16_W) ;
   el_opcode_to_string_map.bind(SMPYL16_W, "SMPYL16_W") ;
   el_string_to_opcode_map.bind("SMPYL17_W", SMPYL17_W) ;
   el_opcode_to_string_map.bind(SMPYL17_W, "SMPYL17_W") ;
   el_string_to_opcode_map.bind("SMPYL18_W", SMPYL18_W) ;
   el_opcode_to_string_map.bind(SMPYL18_W, "SMPYL18_W") ;
   el_string_to_opcode_map.bind("SMPYL19_W", SMPYL19_W) ;
   el_opcode_to_string_map.bind(SMPYL19_W, "SMPYL19_W") ;
   el_string_to_opcode_map.bind("SMPYL20_W", SMPYL20_W) ;
   el_opcode_to_string_map.bind(SMPYL20_W, "SMPYL20_W") ;
   el_string_to_opcode_map.bind("SMPYL21_W", SMPYL21_W) ;
   el_opcode_to_string_map.bind(SMPYL21_W, "SMPYL21_W") ;
   el_string_to_opcode_map.bind("SMPYL22_W", SMPYL22_W) ;
   el_opcode_to_string_map.bind(SMPYL22_W, "SMPYL22_W") ;
   el_string_to_opcode_map.bind("SMPYL23_W", SMPYL23_W) ;
   el_opcode_to_string_map.bind(SMPYL23_W, "SMPYL23_W") ;
   el_string_to_opcode_map.bind("SMPYL24_W", SMPYL24_W) ;
   el_opcode_to_string_map.bind(SMPYL24_W, "SMPYL24_W") ;
   el_string_to_opcode_map.bind("SMPYL25_W", SMPYL25_W) ;
   el_opcode_to_string_map.bind(SMPYL25_W, "SMPYL25_W") ;
   el_string_to_opcode_map.bind("SMPYL26_W", SMPYL26_W) ;
   el_opcode_to_string_map.bind(SMPYL26_W, "SMPYL26_W") ;
   el_string_to_opcode_map.bind("SMPYL27_W", SMPYL27_W) ;
   el_opcode_to_string_map.bind(SMPYL27_W, "SMPYL27_W") ;
   el_string_to_opcode_map.bind("SMPYL28_W", SMPYL28_W) ;
   el_opcode_to_string_map.bind(SMPYL28_W, "SMPYL28_W") ;
   el_string_to_opcode_map.bind("SMPYL29_W", SMPYL29_W) ;
   el_opcode_to_string_map.bind(SMPYL29_W, "SMPYL29_W") ;
   el_string_to_opcode_map.bind("SMPYL30_W", SMPYL30_W) ;
   el_opcode_to_string_map.bind(SMPYL30_W, "SMPYL30_W") ;
   el_string_to_opcode_map.bind("SMPYL31_W", SMPYL31_W) ;
   el_opcode_to_string_map.bind(SMPYL31_W, "SMPYL31_W") ;
   el_string_to_opcode_map.bind("SMPYL32_W", SMPYL32_W) ;
   el_opcode_to_string_map.bind(SMPYL32_W, "SMPYL32_W") ;

   el_string_to_opcode_map.bind("SMPYADD1_W", SMPYADD1_W) ;
   el_opcode_to_string_map.bind(SMPYADD1_W, "SMPYADD1_W") ;
   el_string_to_opcode_map.bind("SMPYADD2_W", SMPYADD2_W) ;
   el_opcode_to_string_map.bind(SMPYADD2_W, "SMPYADD2_W") ;
   el_string_to_opcode_map.bind("SMPYADD3_W", SMPYADD3_W) ;
   el_opcode_to_string_map.bind(SMPYADD3_W, "SMPYADD3_W") ;
   el_string_to_opcode_map.bind("SMPYADD4_W", SMPYADD4_W) ;
   el_opcode_to_string_map.bind(SMPYADD4_W, "SMPYADD4_W") ;
   el_string_to_opcode_map.bind("SMPYADD5_W", SMPYADD5_W) ;
   el_opcode_to_string_map.bind(SMPYADD5_W, "SMPYADD5_W") ;
   el_string_to_opcode_map.bind("SMPYADD6_W", SMPYADD6_W) ;
   el_opcode_to_string_map.bind(SMPYADD6_W, "SMPYADD6_W") ;
   el_string_to_opcode_map.bind("SMPYADD7_W", SMPYADD7_W) ;
   el_opcode_to_string_map.bind(SMPYADD7_W, "SMPYADD7_W") ;
   el_string_to_opcode_map.bind("SMPYADD8_W", SMPYADD8_W) ;
   el_opcode_to_string_map.bind(SMPYADD8_W, "SMPYADD8_W") ;
   el_string_to_opcode_map.bind("SMPYADD9_W", SMPYADD9_W) ;
   el_opcode_to_string_map.bind(SMPYADD9_W, "SMPYADD9_W") ;
   el_string_to_opcode_map.bind("SMPYADD10_W", SMPYADD10_W) ;
   el_opcode_to_string_map.bind(SMPYADD10_W, "SMPYADD10_W") ;
   el_string_to_opcode_map.bind("SMPYADD11_W", SMPYADD11_W) ;
   el_opcode_to_string_map.bind(SMPYADD11_W, "SMPYADD11_W") ;
   el_string_to_opcode_map.bind("SMPYADD12_W", SMPYADD12_W) ;
   el_opcode_to_string_map.bind(SMPYADD12_W, "SMPYADD12_W") ;
   el_string_to_opcode_map.bind("SMPYADD13_W", SMPYADD13_W) ;
   el_opcode_to_string_map.bind(SMPYADD13_W, "SMPYADD13_W") ;
   el_string_to_opcode_map.bind("SMPYADD14_W", SMPYADD14_W) ;
   el_opcode_to_string_map.bind(SMPYADD14_W, "SMPYADD14_W") ;
   el_string_to_opcode_map.bind("SMPYADD15_W", SMPYADD15_W) ;
   el_opcode_to_string_map.bind(SMPYADD15_W, "SMPYADD15_W") ;
   el_string_to_opcode_map.bind("SMPYADD16_W", SMPYADD16_W) ;
   el_opcode_to_string_map.bind(SMPYADD16_W, "SMPYADD16_W") ;
   el_string_to_opcode_map.bind("SMPYADD17_W", SMPYADD17_W) ;
   el_opcode_to_string_map.bind(SMPYADD17_W, "SMPYADD17_W") ;
   el_string_to_opcode_map.bind("SMPYADD18_W", SMPYADD18_W) ;
   el_opcode_to_string_map.bind(SMPYADD18_W, "SMPYADD18_W") ;
   el_string_to_opcode_map.bind("SMPYADD19_W", SMPYADD19_W) ;
   el_opcode_to_string_map.bind(SMPYADD19_W, "SMPYADD19_W") ;
   el_string_to_opcode_map.bind("SMPYADD20_W", SMPYADD20_W) ;
   el_opcode_to_string_map.bind(SMPYADD20_W, "SMPYADD20_W") ;
   el_string_to_opcode_map.bind("SMPYADD21_W", SMPYADD21_W) ;
   el_opcode_to_string_map.bind(SMPYADD21_W, "SMPYADD21_W") ;
   el_string_to_opcode_map.bind("SMPYADD22_W", SMPYADD22_W) ;
   el_opcode_to_string_map.bind(SMPYADD22_W, "SMPYADD22_W") ;
   el_string_to_opcode_map.bind("SMPYADD23_W", SMPYADD23_W) ;
   el_opcode_to_string_map.bind(SMPYADD23_W, "SMPYADD23_W") ;
   el_string_to_opcode_map.bind("SMPYADD24_W", SMPYADD24_W) ;
   el_opcode_to_string_map.bind(SMPYADD24_W, "SMPYADD24_W") ;
   el_string_to_opcode_map.bind("SMPYADD25_W", SMPYADD25_W) ;
   el_opcode_to_string_map.bind(SMPYADD25_W, "SMPYADD25_W") ;
   el_string_to_opcode_map.bind("SMPYADD26_W", SMPYADD26_W) ;
   el_opcode_to_string_map.bind(SMPYADD26_W, "SMPYADD26_W") ;
   el_string_to_opcode_map.bind("SMPYADD27_W", SMPYADD27_W) ;
   el_opcode_to_string_map.bind(SMPYADD27_W, "SMPYADD27_W") ;
   el_string_to_opcode_map.bind("SMPYADD28_W", SMPYADD28_W) ;
   el_opcode_to_string_map.bind(SMPYADD28_W, "SMPYADD28_W") ;
   el_string_to_opcode_map.bind("SMPYADD29_W", SMPYADD29_W) ;
   el_opcode_to_string_map.bind(SMPYADD29_W, "SMPYADD29_W") ;
   el_string_to_opcode_map.bind("SMPYADD30_W", SMPYADD30_W) ;
   el_opcode_to_string_map.bind(SMPYADD30_W, "SMPYADD30_W") ;
   el_string_to_opcode_map.bind("SMPYADD31_W", SMPYADD31_W) ;
   el_opcode_to_string_map.bind(SMPYADD31_W, "SMPYADD31_W") ;
   el_string_to_opcode_map.bind("SMPYADD32_W", SMPYADD32_W) ;
   el_opcode_to_string_map.bind(SMPYADD32_W, "SMPYADD32_W") ;

   el_string_to_opcode_map.bind("SMPYADDN1_W", SMPYADDN1_W) ;
   el_opcode_to_string_map.bind(SMPYADDN1_W, "SMPYADDN1_W") ;
   el_string_to_opcode_map.bind("SMPYADDN2_W", SMPYADDN2_W) ;
   el_opcode_to_string_map.bind(SMPYADDN2_W, "SMPYADDN2_W") ;
   el_string_to_opcode_map.bind("SMPYADDN3_W", SMPYADDN3_W) ;
   el_opcode_to_string_map.bind(SMPYADDN3_W, "SMPYADDN3_W") ;
   el_string_to_opcode_map.bind("SMPYADDN4_W", SMPYADDN4_W) ;
   el_opcode_to_string_map.bind(SMPYADDN4_W, "SMPYADDN4_W") ;
   el_string_to_opcode_map.bind("SMPYADDN5_W", SMPYADDN5_W) ;
   el_opcode_to_string_map.bind(SMPYADDN5_W, "SMPYADDN5_W") ;
   el_string_to_opcode_map.bind("SMPYADDN6_W", SMPYADDN6_W) ;
   el_opcode_to_string_map.bind(SMPYADDN6_W, "SMPYADDN6_W") ;
   el_string_to_opcode_map.bind("SMPYADDN7_W", SMPYADDN7_W) ;
   el_opcode_to_string_map.bind(SMPYADDN7_W, "SMPYADDN7_W") ;
   el_string_to_opcode_map.bind("SMPYADDN8_W", SMPYADDN8_W) ;
   el_opcode_to_string_map.bind(SMPYADDN8_W, "SMPYADDN8_W") ;
   el_string_to_opcode_map.bind("SMPYADDN9_W", SMPYADDN9_W) ;
   el_opcode_to_string_map.bind(SMPYADDN9_W, "SMPYADDN9_W") ;
   el_string_to_opcode_map.bind("SMPYADDN10_W", SMPYADDN10_W) ;
   el_opcode_to_string_map.bind(SMPYADDN10_W, "SMPYADDN10_W") ;
   el_string_to_opcode_map.bind("SMPYADDN11_W", SMPYADDN11_W) ;
   el_opcode_to_string_map.bind(SMPYADDN11_W, "SMPYADDN11_W") ;
   el_string_to_opcode_map.bind("SMPYADDN12_W", SMPYADDN12_W) ;
   el_opcode_to_string_map.bind(SMPYADDN12_W, "SMPYADDN12_W") ;
   el_string_to_opcode_map.bind("SMPYADDN13_W", SMPYADDN13_W) ;
   el_opcode_to_string_map.bind(SMPYADDN13_W, "SMPYADDN13_W") ;
   el_string_to_opcode_map.bind("SMPYADDN14_W", SMPYADDN14_W) ;
   el_opcode_to_string_map.bind(SMPYADDN14_W, "SMPYADDN14_W") ;
   el_string_to_opcode_map.bind("SMPYADDN15_W", SMPYADDN15_W) ;
   el_opcode_to_string_map.bind(SMPYADDN15_W, "SMPYADDN15_W") ;
   el_string_to_opcode_map.bind("SMPYADDN16_W", SMPYADDN16_W) ;
   el_opcode_to_string_map.bind(SMPYADDN16_W, "SMPYADDN16_W") ;
   el_string_to_opcode_map.bind("SMPYADDN17_W", SMPYADDN17_W) ;
   el_opcode_to_string_map.bind(SMPYADDN17_W, "SMPYADDN17_W") ;
   el_string_to_opcode_map.bind("SMPYADDN18_W", SMPYADDN18_W) ;
   el_opcode_to_string_map.bind(SMPYADDN18_W, "SMPYADDN18_W") ;
   el_string_to_opcode_map.bind("SMPYADDN19_W", SMPYADDN19_W) ;
   el_opcode_to_string_map.bind(SMPYADDN19_W, "SMPYADDN19_W") ;
   el_string_to_opcode_map.bind("SMPYADDN20_W", SMPYADDN20_W) ;
   el_opcode_to_string_map.bind(SMPYADDN20_W, "SMPYADDN20_W") ;
   el_string_to_opcode_map.bind("SMPYADDN21_W", SMPYADDN21_W) ;
   el_opcode_to_string_map.bind(SMPYADDN21_W, "SMPYADDN21_W") ;
   el_string_to_opcode_map.bind("SMPYADDN22_W", SMPYADDN22_W) ;
   el_opcode_to_string_map.bind(SMPYADDN22_W, "SMPYADDN22_W") ;
   el_string_to_opcode_map.bind("SMPYADDN23_W", SMPYADDN23_W) ;
   el_opcode_to_string_map.bind(SMPYADDN23_W, "SMPYADDN23_W") ;
   el_string_to_opcode_map.bind("SMPYADDN24_W", SMPYADDN24_W) ;
   el_opcode_to_string_map.bind(SMPYADDN24_W, "SMPYADDN24_W") ;
   el_string_to_opcode_map.bind("SMPYADDN25_W", SMPYADDN25_W) ;
   el_opcode_to_string_map.bind(SMPYADDN25_W, "SMPYADDN25_W") ;
   el_string_to_opcode_map.bind("SMPYADDN26_W", SMPYADDN26_W) ;
   el_opcode_to_string_map.bind(SMPYADDN26_W, "SMPYADDN26_W") ;
   el_string_to_opcode_map.bind("SMPYADDN27_W", SMPYADDN27_W) ;
   el_opcode_to_string_map.bind(SMPYADDN27_W, "SMPYADDN27_W") ;
   el_string_to_opcode_map.bind("SMPYADDN28_W", SMPYADDN28_W) ;
   el_opcode_to_string_map.bind(SMPYADDN28_W, "SMPYADDN28_W") ;
   el_string_to_opcode_map.bind("SMPYADDN29_W", SMPYADDN29_W) ;
   el_opcode_to_string_map.bind(SMPYADDN29_W, "SMPYADDN29_W") ;
   el_string_to_opcode_map.bind("SMPYADDN30_W", SMPYADDN30_W) ;
   el_opcode_to_string_map.bind(SMPYADDN30_W, "SMPYADDN30_W") ;
   el_string_to_opcode_map.bind("SMPYADDN31_W", SMPYADDN31_W) ;
   el_opcode_to_string_map.bind(SMPYADDN31_W, "SMPYADDN31_W") ;
   el_string_to_opcode_map.bind("SMPYADDN32_W", SMPYADDN32_W) ;
   el_opcode_to_string_map.bind(SMPYADDN32_W, "SMPYADDN32_W") ;

   el_string_to_opcode_map.bind("SMPYSUB1_W", SMPYSUB1_W) ;
   el_opcode_to_string_map.bind(SMPYSUB1_W, "SMPYSUB1_W") ;
   el_string_to_opcode_map.bind("SMPYSUB2_W", SMPYSUB2_W) ;
   el_opcode_to_string_map.bind(SMPYSUB2_W, "SMPYSUB2_W") ;
   el_string_to_opcode_map.bind("SMPYSUB3_W", SMPYSUB3_W) ;
   el_opcode_to_string_map.bind(SMPYSUB3_W, "SMPYSUB3_W") ;
   el_string_to_opcode_map.bind("SMPYSUB4_W", SMPYSUB4_W) ;
   el_opcode_to_string_map.bind(SMPYSUB4_W, "SMPYSUB4_W") ;
   el_string_to_opcode_map.bind("SMPYSUB5_W", SMPYSUB5_W) ;
   el_opcode_to_string_map.bind(SMPYSUB5_W, "SMPYSUB5_W") ;
   el_string_to_opcode_map.bind("SMPYSUB6_W", SMPYSUB6_W) ;
   el_opcode_to_string_map.bind(SMPYSUB6_W, "SMPYSUB6_W") ;
   el_string_to_opcode_map.bind("SMPYSUB7_W", SMPYSUB7_W) ;
   el_opcode_to_string_map.bind(SMPYSUB7_W, "SMPYSUB7_W") ;
   el_string_to_opcode_map.bind("SMPYSUB8_W", SMPYSUB8_W) ;
   el_opcode_to_string_map.bind(SMPYSUB8_W, "SMPYSUB8_W") ;
   el_string_to_opcode_map.bind("SMPYSUB9_W", SMPYSUB9_W) ;
   el_opcode_to_string_map.bind(SMPYSUB9_W, "SMPYSUB9_W") ;
   el_string_to_opcode_map.bind("SMPYSUB10_W", SMPYSUB10_W) ;
   el_opcode_to_string_map.bind(SMPYSUB10_W, "SMPYSUB10_W") ;
   el_string_to_opcode_map.bind("SMPYSUB11_W", SMPYSUB11_W) ;
   el_opcode_to_string_map.bind(SMPYSUB11_W, "SMPYSUB11_W") ;
   el_string_to_opcode_map.bind("SMPYSUB12_W", SMPYSUB12_W) ;
   el_opcode_to_string_map.bind(SMPYSUB12_W, "SMPYSUB12_W") ;
   el_string_to_opcode_map.bind("SMPYSUB13_W", SMPYSUB13_W) ;
   el_opcode_to_string_map.bind(SMPYSUB13_W, "SMPYSUB13_W") ;
   el_string_to_opcode_map.bind("SMPYSUB14_W", SMPYSUB14_W) ;
   el_opcode_to_string_map.bind(SMPYSUB14_W, "SMPYSUB14_W") ;
   el_string_to_opcode_map.bind("SMPYSUB15_W", SMPYSUB15_W) ;
   el_opcode_to_string_map.bind(SMPYSUB15_W, "SMPYSUB15_W") ;
   el_string_to_opcode_map.bind("SMPYSUB16_W", SMPYSUB16_W) ;
   el_opcode_to_string_map.bind(SMPYSUB16_W, "SMPYSUB16_W") ;
   el_string_to_opcode_map.bind("SMPYSUB17_W", SMPYSUB17_W) ;
   el_opcode_to_string_map.bind(SMPYSUB17_W, "SMPYSUB17_W") ;
   el_string_to_opcode_map.bind("SMPYSUB18_W", SMPYSUB18_W) ;
   el_opcode_to_string_map.bind(SMPYSUB18_W, "SMPYSUB18_W") ;
   el_string_to_opcode_map.bind("SMPYSUB19_W", SMPYSUB19_W) ;
   el_opcode_to_string_map.bind(SMPYSUB19_W, "SMPYSUB19_W") ;
   el_string_to_opcode_map.bind("SMPYSUB20_W", SMPYSUB20_W) ;
   el_opcode_to_string_map.bind(SMPYSUB20_W, "SMPYSUB20_W") ;
   el_string_to_opcode_map.bind("SMPYSUB21_W", SMPYSUB21_W) ;
   el_opcode_to_string_map.bind(SMPYSUB21_W, "SMPYSUB21_W") ;
   el_string_to_opcode_map.bind("SMPYSUB22_W", SMPYSUB22_W) ;
   el_opcode_to_string_map.bind(SMPYSUB22_W, "SMPYSUB22_W") ;
   el_string_to_opcode_map.bind("SMPYSUB23_W", SMPYSUB23_W) ;
   el_opcode_to_string_map.bind(SMPYSUB23_W, "SMPYSUB23_W") ;
   el_string_to_opcode_map.bind("SMPYSUB24_W", SMPYSUB24_W) ;
   el_opcode_to_string_map.bind(SMPYSUB24_W, "SMPYSUB24_W") ;
   el_string_to_opcode_map.bind("SMPYSUB25_W", SMPYSUB25_W) ;
   el_opcode_to_string_map.bind(SMPYSUB25_W, "SMPYSUB25_W") ;
   el_string_to_opcode_map.bind("SMPYSUB26_W", SMPYSUB26_W) ;
   el_opcode_to_string_map.bind(SMPYSUB26_W, "SMPYSUB26_W") ;
   el_string_to_opcode_map.bind("SMPYSUB27_W", SMPYSUB27_W) ;
   el_opcode_to_string_map.bind(SMPYSUB27_W, "SMPYSUB27_W") ;
   el_string_to_opcode_map.bind("SMPYSUB28_W", SMPYSUB28_W) ;
   el_opcode_to_string_map.bind(SMPYSUB28_W, "SMPYSUB28_W") ;
   el_string_to_opcode_map.bind("SMPYSUB29_W", SMPYSUB29_W) ;
   el_opcode_to_string_map.bind(SMPYSUB29_W, "SMPYSUB29_W") ;
   el_string_to_opcode_map.bind("SMPYSUB30_W", SMPYSUB30_W) ;
   el_opcode_to_string_map.bind(SMPYSUB30_W, "SMPYSUB30_W") ;
   el_string_to_opcode_map.bind("SMPYSUB31_W", SMPYSUB31_W) ;
   el_opcode_to_string_map.bind(SMPYSUB31_W, "SMPYSUB31_W") ;
   el_string_to_opcode_map.bind("SMPYSUB32_W", SMPYSUB32_W) ;
   el_opcode_to_string_map.bind(SMPYSUB32_W, "SMPYSUB32_W") ;

   el_string_to_opcode_map.bind("SMPYRSUB1_W", SMPYRSUB1_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB1_W, "SMPYRSUB1_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB2_W", SMPYRSUB2_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB2_W, "SMPYRSUB2_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB3_W", SMPYRSUB3_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB3_W, "SMPYRSUB3_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB4_W", SMPYRSUB4_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB4_W, "SMPYRSUB4_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB5_W", SMPYRSUB5_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB5_W, "SMPYRSUB5_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB6_W", SMPYRSUB6_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB6_W, "SMPYRSUB6_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB7_W", SMPYRSUB7_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB7_W, "SMPYRSUB7_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB8_W", SMPYRSUB8_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB8_W, "SMPYRSUB8_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB9_W", SMPYRSUB9_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB9_W, "SMPYRSUB9_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB10_W", SMPYRSUB10_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB10_W, "SMPYRSUB10_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB11_W", SMPYRSUB11_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB11_W, "SMPYRSUB11_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB12_W", SMPYRSUB12_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB12_W, "SMPYRSUB12_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB13_W", SMPYRSUB13_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB13_W, "SMPYRSUB13_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB14_W", SMPYRSUB14_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB14_W, "SMPYRSUB14_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB15_W", SMPYRSUB15_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB15_W, "SMPYRSUB15_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB16_W", SMPYRSUB16_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB16_W, "SMPYRSUB16_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB17_W", SMPYRSUB17_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB17_W, "SMPYRSUB17_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB18_W", SMPYRSUB18_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB18_W, "SMPYRSUB18_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB19_W", SMPYRSUB19_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB19_W, "SMPYRSUB19_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB20_W", SMPYRSUB20_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB20_W, "SMPYRSUB20_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB21_W", SMPYRSUB21_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB21_W, "SMPYRSUB21_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB22_W", SMPYRSUB22_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB22_W, "SMPYRSUB22_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB23_W", SMPYRSUB23_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB23_W, "SMPYRSUB23_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB24_W", SMPYRSUB24_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB24_W, "SMPYRSUB24_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB25_W", SMPYRSUB25_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB25_W, "SMPYRSUB25_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB26_W", SMPYRSUB26_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB26_W, "SMPYRSUB26_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB27_W", SMPYRSUB27_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB27_W, "SMPYRSUB27_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB28_W", SMPYRSUB28_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB28_W, "SMPYRSUB28_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB29_W", SMPYRSUB29_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB29_W, "SMPYRSUB29_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB30_W", SMPYRSUB30_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB30_W, "SMPYRSUB30_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB31_W", SMPYRSUB31_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB31_W, "SMPYRSUB31_W") ;
   el_string_to_opcode_map.bind("SMPYRSUB32_W", SMPYRSUB32_W) ;
   el_opcode_to_string_map.bind(SMPYRSUB32_W, "SMPYRSUB32_W") ;

   el_string_to_opcode_map.bind("FADD_S", FADD_S) ;
   el_opcode_to_string_map.bind(FADD_S, "FADD_S") ;
   el_string_to_opcode_map.bind("FADD_D", FADD_D) ;
   el_opcode_to_string_map.bind(FADD_D, "FADD_D") ;
   el_string_to_opcode_map.bind("FABS_S", FABS_S) ;
   el_opcode_to_string_map.bind(FABS_S, "FABS_S") ;
   el_string_to_opcode_map.bind("FABS_D", FABS_D) ;
   el_opcode_to_string_map.bind(FABS_D, "FABS_D") ;
   el_string_to_opcode_map.bind("FDIV_S", FDIV_S) ;
   el_opcode_to_string_map.bind(FDIV_S, "FDIV_S") ;
   el_string_to_opcode_map.bind("FDIV_D", FDIV_D) ;
   el_opcode_to_string_map.bind(FDIV_D, "FDIV_D") ;
   el_string_to_opcode_map.bind("FMAX_S", FMAX_S) ;
   el_opcode_to_string_map.bind(FMAX_S, "FMAX_S") ;
   el_string_to_opcode_map.bind("FMAX_D", FMAX_D) ;
   el_opcode_to_string_map.bind(FMAX_D, "FMAX_D") ;
   el_string_to_opcode_map.bind("FMIN_S", FMIN_S) ;
   el_opcode_to_string_map.bind(FMIN_S, "FMIN_S") ;
   el_string_to_opcode_map.bind("FMIN_D", FMIN_D) ;
   el_opcode_to_string_map.bind(FMIN_D, "FMIN_D") ;
   el_string_to_opcode_map.bind("FMPY_S", FMPY_S) ;
   el_opcode_to_string_map.bind(FMPY_S, "FMPY_S") ;
   el_string_to_opcode_map.bind("FMPY_D", FMPY_D) ;
   el_opcode_to_string_map.bind(FMPY_D, "FMPY_D") ;
   el_string_to_opcode_map.bind("FMPYADD_S", FMPYADD_S) ;
   el_opcode_to_string_map.bind(FMPYADD_S, "FMPYADD_S") ;
   el_string_to_opcode_map.bind("FMPYADD_D", FMPYADD_D) ;
   el_opcode_to_string_map.bind(FMPYADD_D, "FMPYADD_D") ;
   el_string_to_opcode_map.bind("FMPYADDN_S", FMPYADDN_S) ;
   el_opcode_to_string_map.bind(FMPYADDN_S, "FMPYADDN_S") ;
   el_string_to_opcode_map.bind("FMPYADDN_D", FMPYADDN_D) ;
   el_opcode_to_string_map.bind(FMPYADDN_D, "FMPYADDN_D") ;
   el_string_to_opcode_map.bind("FMPYRSUB_S", FMPYRSUB_S) ;
   el_opcode_to_string_map.bind(FMPYRSUB_S, "FMPYRSUB_S") ;
   el_string_to_opcode_map.bind("FMPYRSUB_D", FMPYRSUB_D) ;
   el_opcode_to_string_map.bind(FMPYRSUB_D, "FMPYRSUB_D") ;
   el_string_to_opcode_map.bind("FMPYSUB_S", FMPYSUB_S) ;
   el_opcode_to_string_map.bind(FMPYSUB_S, "FMPYSUB_S") ;
   el_string_to_opcode_map.bind("FMPYSUB_D", FMPYSUB_D) ;
   el_opcode_to_string_map.bind(FMPYSUB_D, "FMPYSUB_D") ;
   el_string_to_opcode_map.bind("FRCP_S", FRCP_S) ;
   el_opcode_to_string_map.bind(FRCP_S, "FRCP_S") ;
   el_string_to_opcode_map.bind("FRCP_D", FRCP_D) ;
   el_opcode_to_string_map.bind(FRCP_D, "FRCP_D") ;
   el_string_to_opcode_map.bind("FSQRT_S", FSQRT_S) ;
   el_opcode_to_string_map.bind(FSQRT_S, "FSQRT_S") ;
   el_string_to_opcode_map.bind("FSQRT_D", FSQRT_D) ;
   el_opcode_to_string_map.bind(FSQRT_D, "FSQRT_D") ;
   el_string_to_opcode_map.bind("FSUB_S", FSUB_S) ;
   el_opcode_to_string_map.bind(FSUB_S, "FSUB_S") ;
   el_string_to_opcode_map.bind("FSUB_D", FSUB_D) ;
   el_opcode_to_string_map.bind(FSUB_D, "FSUB_D") ;

   el_string_to_opcode_map.bind("CONVWS", CONVWS) ;
   el_opcode_to_string_map.bind(CONVWS, "CONVWS") ;
   el_string_to_opcode_map.bind("CONVWD", CONVWD) ;
   el_opcode_to_string_map.bind(CONVWD, "CONVWD") ;
   el_string_to_opcode_map.bind("CONVSW", CONVSW) ;
   el_opcode_to_string_map.bind(CONVSW, "CONVSW") ;
   el_string_to_opcode_map.bind("CONVDW", CONVDW) ;
   el_opcode_to_string_map.bind(CONVDW, "CONVDW") ;
   el_string_to_opcode_map.bind("CONVSD", CONVSD) ;
   el_opcode_to_string_map.bind(CONVSD, "CONVSD") ;
   el_string_to_opcode_map.bind("CONVDS", CONVDS) ;
   el_opcode_to_string_map.bind(CONVDS, "CONVDS") ;
   el_string_to_opcode_map.bind("CONVLWS", CONVLWS) ;
   el_opcode_to_string_map.bind(CONVLWS, "CONVLWS") ;
   el_string_to_opcode_map.bind("CONVLWD", CONVLWD) ;
   el_opcode_to_string_map.bind(CONVLWD, "CONVLWD") ;
   el_string_to_opcode_map.bind("CONVLSW", CONVLSW) ;
   el_opcode_to_string_map.bind(CONVLSW, "CONVLSW") ;
   el_string_to_opcode_map.bind("CONVLDW", CONVLDW) ;
   el_opcode_to_string_map.bind(CONVLDW, "CONVLDW") ;

   el_string_to_opcode_map.bind("EXTS_B", EXTS_B) ;
   el_opcode_to_string_map.bind(EXTS_B, "EXTS_B") ;
   el_string_to_opcode_map.bind("EXTS_H", EXTS_H) ;
   el_opcode_to_string_map.bind(EXTS_H, "EXTS_H") ;
   el_string_to_opcode_map.bind("EXTS_W", EXTS_W) ;
   el_opcode_to_string_map.bind(EXTS_W, "EXTS_W") ;

   el_string_to_opcode_map.bind("EXTZ_B", EXTZ_B) ;
   el_opcode_to_string_map.bind(EXTZ_B, "EXTZ_B") ;
   el_string_to_opcode_map.bind("EXTZ_H", EXTZ_H) ;
   el_opcode_to_string_map.bind(EXTZ_H, "EXTZ_H") ;
   el_string_to_opcode_map.bind("EXTZ_W", EXTZ_W) ;
   el_opcode_to_string_map.bind(EXTZ_W, "EXTZ_W") ;

   el_string_to_opcode_map.bind("EXTRACT_W", EXTRACT_W) ;
   el_opcode_to_string_map.bind(EXTRACT_W, "EXTRACT_W") ;
   el_string_to_opcode_map.bind("EXTRACTU_W", EXTRACTU_W) ;
   el_opcode_to_string_map.bind(EXTRACTU_W, "EXTRACTU_W") ;
   el_string_to_opcode_map.bind("DEPOSIT_W", DEPOSIT_W) ;
   el_opcode_to_string_map.bind(DEPOSIT_W, "DEPOSIT_W") ;

   el_string_to_opcode_map.bind("MOVE", MOVE) ;
   el_opcode_to_string_map.bind(MOVE, "MOVE") ;
   el_string_to_opcode_map.bind("MOVEGF_L", MOVEGF_L) ;
   el_opcode_to_string_map.bind(MOVEGF_L, "MOVEGF_L") ;
   el_string_to_opcode_map.bind("MOVEGF_U", MOVEGF_U) ;
   el_opcode_to_string_map.bind(MOVEGF_U, "MOVEGF_U") ;
   el_string_to_opcode_map.bind("MOVEF_S", MOVEF_S) ;
   el_opcode_to_string_map.bind(MOVEF_S, "MOVEF_S") ;
   el_string_to_opcode_map.bind("MOVEF_D", MOVEF_D) ;
   el_opcode_to_string_map.bind(MOVEF_D, "MOVEF_D") ;
   el_string_to_opcode_map.bind("MOVEFG_L", MOVEFG_L) ;
   el_opcode_to_string_map.bind(MOVEFG_L, "MOVEFG_L") ;
   el_string_to_opcode_map.bind("MOVEFG_U", MOVEFG_U) ;
   el_opcode_to_string_map.bind(MOVEFG_U, "MOVEFG_U") ;
   el_string_to_opcode_map.bind("MOVEPG", MOVEPG) ;
   el_opcode_to_string_map.bind(MOVEPG, "MOVEPG") ;
   el_string_to_opcode_map.bind("MOVEGP", MOVEGP) ;
   el_opcode_to_string_map.bind(MOVEGP, "MOVEGP") ;
   el_string_to_opcode_map.bind("LDCM", LDCM) ;
   el_opcode_to_string_map.bind(LDCM, "LDCM") ;

   el_string_to_opcode_map.bind("MOVEGG", MOVEGG) ;
   el_opcode_to_string_map.bind(MOVEGG, "MOVEGG") ;
   el_string_to_opcode_map.bind("MOVEGC", MOVEGC) ;
   el_opcode_to_string_map.bind(MOVEGC, "MOVEGC") ;
   el_string_to_opcode_map.bind("MOVECG", MOVECG) ;
   el_opcode_to_string_map.bind(MOVECG, "MOVECG") ;
   el_string_to_opcode_map.bind("MOVEBB", MOVEBB) ;
   el_opcode_to_string_map.bind(MOVEBB, "MOVEBB") ;
   el_string_to_opcode_map.bind("MOVEPP", MOVEPP) ;
   el_opcode_to_string_map.bind(MOVEPP, "MOVEPP") ;

   // New move operations used to save/restore and spill/unspill predicate 
   // registers -- VK/5/22/98
   el_string_to_opcode_map.bind("MOVEGBP", MOVEGBP) ;
   el_opcode_to_string_map.bind(MOVEGBP, "MOVEGBP") ;
   el_string_to_opcode_map.bind("MOVEGCM", MOVEGCM) ;
   el_opcode_to_string_map.bind(MOVEGCM, "MOVEGCM") ;

   // New move operations for speculative tags, SAM 7-01.
   el_string_to_opcode_map.bind("MOVEGBGT", MOVEGBGT) ;
   el_opcode_to_string_map.bind(MOVEGBGT, "MOVEGBGT") ;
   el_string_to_opcode_map.bind("MOVEGBFT", MOVEGBFT) ;
   el_opcode_to_string_map.bind(MOVEGBFT, "MOVEGBFT") ;
   el_string_to_opcode_map.bind("MOVEGBPT", MOVEGBPT) ;
   el_opcode_to_string_map.bind(MOVEGBPT, "MOVEGBPT") ;
   el_string_to_opcode_map.bind("MOVEGBBT", MOVEGBBT) ;
   el_opcode_to_string_map.bind(MOVEGBBT, "MOVEGBBT") ;

   el_string_to_opcode_map.bind("CMPR_W_FALSE", CMPR_W_FALSE) ;
   el_opcode_to_string_map.bind(CMPR_W_FALSE, "CMPR_W_FALSE") ;
   el_string_to_opcode_map.bind("CMPR_W_EQ", CMPR_W_EQ) ;
   el_opcode_to_string_map.bind(CMPR_W_EQ, "CMPR_W_EQ") ;
   el_string_to_opcode_map.bind("CMPR_W_LT", CMPR_W_LT) ;
   el_opcode_to_string_map.bind(CMPR_W_LT, "CMPR_W_LT") ;
   el_string_to_opcode_map.bind("CMPR_W_LEQ", CMPR_W_LEQ) ;
   el_opcode_to_string_map.bind(CMPR_W_LEQ, "CMPR_W_LEQ") ;
   el_string_to_opcode_map.bind("CMPR_W_GT", CMPR_W_GT) ;
   el_opcode_to_string_map.bind(CMPR_W_GT, "CMPR_W_GT") ;
   el_string_to_opcode_map.bind("CMPR_W_GEQ", CMPR_W_GEQ) ;
   el_opcode_to_string_map.bind(CMPR_W_GEQ, "CMPR_W_GEQ") ;
   el_string_to_opcode_map.bind("CMPR_W_SV", CMPR_W_SV) ;
   el_opcode_to_string_map.bind(CMPR_W_SV, "CMPR_W_SV") ;
   el_string_to_opcode_map.bind("CMPR_W_OD", CMPR_W_OD) ;
   el_opcode_to_string_map.bind(CMPR_W_OD, "CMPR_W_OD") ;
   el_string_to_opcode_map.bind("CMPR_W_TRUE", CMPR_W_TRUE) ;
   el_opcode_to_string_map.bind(CMPR_W_TRUE, "CMPR_W_TRUE") ;
   el_string_to_opcode_map.bind("CMPR_W_NEQ", CMPR_W_NEQ) ;
   el_opcode_to_string_map.bind(CMPR_W_NEQ, "CMPR_W_NEQ") ;
   el_string_to_opcode_map.bind("CMPR_W_LLT", CMPR_W_LLT) ;
   el_opcode_to_string_map.bind(CMPR_W_LLT, "CMPR_W_LLT") ;
   el_string_to_opcode_map.bind("CMPR_W_LLEQ", CMPR_W_LLEQ) ;
   el_opcode_to_string_map.bind(CMPR_W_LLEQ, "CMPR_W_LLEQ") ;
   el_string_to_opcode_map.bind("CMPR_W_LGT", CMPR_W_LGT) ;
   el_opcode_to_string_map.bind(CMPR_W_LGT, "CMPR_W_LGT") ;
   el_string_to_opcode_map.bind("CMPR_W_LGEQ", CMPR_W_LGEQ) ;
   el_opcode_to_string_map.bind(CMPR_W_LGEQ, "CMPR_W_LGEQ") ;
   el_string_to_opcode_map.bind("CMPR_W_NSV", CMPR_W_NSV) ;
   el_opcode_to_string_map.bind(CMPR_W_NSV, "CMPR_W_NSV") ;
   el_string_to_opcode_map.bind("CMPR_W_EV", CMPR_W_EV) ;
   el_opcode_to_string_map.bind(CMPR_W_EV, "CMPR_W_EV") ;

   el_string_to_opcode_map.bind("FCMPR_S_FALSE", FCMPR_S_FALSE) ;
   el_opcode_to_string_map.bind(FCMPR_S_FALSE, "FCMPR_S_FALSE") ;
   el_string_to_opcode_map.bind("FCMPR_S_EQ", FCMPR_S_EQ) ;
   el_opcode_to_string_map.bind(FCMPR_S_EQ, "FCMPR_S_EQ") ;
   el_string_to_opcode_map.bind("FCMPR_S_LT", FCMPR_S_LT) ;
   el_opcode_to_string_map.bind(FCMPR_S_LT, "FCMPR_S_LT") ;
   el_string_to_opcode_map.bind("FCMPR_S_LEQ", FCMPR_S_LEQ) ;
   el_opcode_to_string_map.bind(FCMPR_S_LEQ, "FCMPR_S_LEQ") ;
   el_string_to_opcode_map.bind("FCMPR_S_GT", FCMPR_S_GT) ;
   el_opcode_to_string_map.bind(FCMPR_S_GT, "FCMPR_S_GT") ;
   el_string_to_opcode_map.bind("FCMPR_S_GEQ", FCMPR_S_GEQ) ;
   el_opcode_to_string_map.bind(FCMPR_S_GEQ, "FCMPR_S_GEQ") ;
   el_string_to_opcode_map.bind("FCMPR_S_NEQ", FCMPR_S_NEQ) ;
   el_opcode_to_string_map.bind(FCMPR_S_NEQ, "FCMPR_S_NEQ") ;
   el_string_to_opcode_map.bind("FCMPR_S_TRUE", FCMPR_S_TRUE) ;
   el_opcode_to_string_map.bind(FCMPR_S_TRUE, "FCMPR_S_TRUE") ;

   el_string_to_opcode_map.bind("FCMPR_D_FALSE", FCMPR_D_FALSE) ;
   el_opcode_to_string_map.bind(FCMPR_D_FALSE, "FCMPR_D_FALSE") ;
   el_string_to_opcode_map.bind("FCMPR_D_EQ", FCMPR_D_EQ) ;
   el_opcode_to_string_map.bind(FCMPR_D_EQ, "FCMPR_D_EQ") ;
   el_string_to_opcode_map.bind("FCMPR_D_LT", FCMPR_D_LT) ;
   el_opcode_to_string_map.bind(FCMPR_D_LT, "FCMPR_D_LT") ;
   el_string_to_opcode_map.bind("FCMPR_D_LEQ", FCMPR_D_LEQ) ;
   el_opcode_to_string_map.bind(FCMPR_D_LEQ, "FCMPR_D_LEQ") ;
   el_string_to_opcode_map.bind("FCMPR_D_GT", FCMPR_D_GT) ;
   el_opcode_to_string_map.bind(FCMPR_D_GT, "FCMPR_D_GT") ;
   el_string_to_opcode_map.bind("FCMPR_D_GEQ", FCMPR_D_GEQ) ;
   el_opcode_to_string_map.bind(FCMPR_D_GEQ, "FCMPR_D_GEQ") ;
   el_string_to_opcode_map.bind("FCMPR_D_NEQ", FCMPR_D_NEQ) ;
   el_opcode_to_string_map.bind(FCMPR_D_NEQ, "FCMPR_D_NEQ") ;
   el_string_to_opcode_map.bind("FCMPR_D_TRUE", FCMPR_D_TRUE) ;
   el_opcode_to_string_map.bind(FCMPR_D_TRUE, "FCMPR_D_TRUE") ;
}

static void el_init_elcor_opcode_maps_cmpp1_1()
{
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_UN", (Opcode) CMPP_W_FALSE_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_UN, "CMPP_W_FALSE_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_UC", (Opcode) CMPP_W_FALSE_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_UC, "CMPP_W_FALSE_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_CN", (Opcode) CMPP_W_FALSE_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_CN, "CMPP_W_FALSE_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_CC", (Opcode) CMPP_W_FALSE_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_CC, "CMPP_W_FALSE_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_ON", (Opcode) CMPP_W_FALSE_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_ON, "CMPP_W_FALSE_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_OC", (Opcode) CMPP_W_FALSE_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_OC, "CMPP_W_FALSE_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_AN", (Opcode) CMPP_W_FALSE_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_AN, "CMPP_W_FALSE_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UN_AC", (Opcode) CMPP_W_FALSE_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UN_AC, "CMPP_W_FALSE_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_UN", (Opcode) CMPP_W_FALSE_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_UN, "CMPP_W_FALSE_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_UC", (Opcode) CMPP_W_FALSE_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_UC, "CMPP_W_FALSE_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_CN", (Opcode) CMPP_W_FALSE_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_CN, "CMPP_W_FALSE_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_CC", (Opcode) CMPP_W_FALSE_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_CC, "CMPP_W_FALSE_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_ON", (Opcode) CMPP_W_FALSE_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_ON, "CMPP_W_FALSE_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_OC", (Opcode) CMPP_W_FALSE_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_OC, "CMPP_W_FALSE_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_AN", (Opcode) CMPP_W_FALSE_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_AN, "CMPP_W_FALSE_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_UC_AC", (Opcode) CMPP_W_FALSE_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_UC_AC, "CMPP_W_FALSE_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_UN", (Opcode) CMPP_W_FALSE_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_UN, "CMPP_W_FALSE_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_UC", (Opcode) CMPP_W_FALSE_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_UC, "CMPP_W_FALSE_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_CN", (Opcode) CMPP_W_FALSE_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_CN, "CMPP_W_FALSE_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_CC", (Opcode) CMPP_W_FALSE_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_CC, "CMPP_W_FALSE_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_ON", (Opcode) CMPP_W_FALSE_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_ON, "CMPP_W_FALSE_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_OC", (Opcode) CMPP_W_FALSE_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_OC, "CMPP_W_FALSE_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_AN", (Opcode) CMPP_W_FALSE_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_AN, "CMPP_W_FALSE_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CN_AC", (Opcode) CMPP_W_FALSE_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CN_AC, "CMPP_W_FALSE_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_UN", (Opcode) CMPP_W_FALSE_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_UN, "CMPP_W_FALSE_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_UC", (Opcode) CMPP_W_FALSE_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_UC, "CMPP_W_FALSE_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_CN", (Opcode) CMPP_W_FALSE_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_CN, "CMPP_W_FALSE_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_CC", (Opcode) CMPP_W_FALSE_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_CC, "CMPP_W_FALSE_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_ON", (Opcode) CMPP_W_FALSE_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_ON, "CMPP_W_FALSE_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_OC", (Opcode) CMPP_W_FALSE_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_OC, "CMPP_W_FALSE_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_AN", (Opcode) CMPP_W_FALSE_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_AN, "CMPP_W_FALSE_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_CC_AC", (Opcode) CMPP_W_FALSE_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_CC_AC, "CMPP_W_FALSE_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_UN", (Opcode) CMPP_W_FALSE_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_UN, "CMPP_W_FALSE_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_UC", (Opcode) CMPP_W_FALSE_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_UC, "CMPP_W_FALSE_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_CN", (Opcode) CMPP_W_FALSE_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_CN, "CMPP_W_FALSE_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_CC", (Opcode) CMPP_W_FALSE_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_CC, "CMPP_W_FALSE_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_ON", (Opcode) CMPP_W_FALSE_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_ON, "CMPP_W_FALSE_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_OC", (Opcode) CMPP_W_FALSE_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_OC, "CMPP_W_FALSE_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_AN", (Opcode) CMPP_W_FALSE_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_AN, "CMPP_W_FALSE_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_ON_AC", (Opcode) CMPP_W_FALSE_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_ON_AC, "CMPP_W_FALSE_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_UN", (Opcode) CMPP_W_FALSE_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_UN, "CMPP_W_FALSE_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_UC", (Opcode) CMPP_W_FALSE_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_UC, "CMPP_W_FALSE_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_CN", (Opcode) CMPP_W_FALSE_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_CN, "CMPP_W_FALSE_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_CC", (Opcode) CMPP_W_FALSE_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_CC, "CMPP_W_FALSE_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_ON", (Opcode) CMPP_W_FALSE_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_ON, "CMPP_W_FALSE_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_OC", (Opcode) CMPP_W_FALSE_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_OC, "CMPP_W_FALSE_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_AN", (Opcode) CMPP_W_FALSE_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_AN, "CMPP_W_FALSE_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_OC_AC", (Opcode) CMPP_W_FALSE_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_OC_AC, "CMPP_W_FALSE_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_UN", (Opcode) CMPP_W_FALSE_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_UN, "CMPP_W_FALSE_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_UC", (Opcode) CMPP_W_FALSE_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_UC, "CMPP_W_FALSE_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_CN", (Opcode) CMPP_W_FALSE_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_CN, "CMPP_W_FALSE_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_CC", (Opcode) CMPP_W_FALSE_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_CC, "CMPP_W_FALSE_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_ON", (Opcode) CMPP_W_FALSE_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_ON, "CMPP_W_FALSE_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_OC", (Opcode) CMPP_W_FALSE_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_OC, "CMPP_W_FALSE_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_AN", (Opcode) CMPP_W_FALSE_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_AN, "CMPP_W_FALSE_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AN_AC", (Opcode) CMPP_W_FALSE_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AN_AC, "CMPP_W_FALSE_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_UN", (Opcode) CMPP_W_FALSE_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_UN, "CMPP_W_FALSE_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_UC", (Opcode) CMPP_W_FALSE_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_UC, "CMPP_W_FALSE_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_CN", (Opcode) CMPP_W_FALSE_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_CN, "CMPP_W_FALSE_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_CC", (Opcode) CMPP_W_FALSE_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_CC, "CMPP_W_FALSE_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_ON", (Opcode) CMPP_W_FALSE_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_ON, "CMPP_W_FALSE_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_OC", (Opcode) CMPP_W_FALSE_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_OC, "CMPP_W_FALSE_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_AN", (Opcode) CMPP_W_FALSE_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_AN, "CMPP_W_FALSE_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_FALSE_AC_AC", (Opcode) CMPP_W_FALSE_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_FALSE_AC_AC, "CMPP_W_FALSE_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1_2()
{
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_UN", (Opcode) CMPP_W_EQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_UN, "CMPP_W_EQ_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_UC", (Opcode) CMPP_W_EQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_UC, "CMPP_W_EQ_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_CN", (Opcode) CMPP_W_EQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_CN, "CMPP_W_EQ_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_CC", (Opcode) CMPP_W_EQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_CC, "CMPP_W_EQ_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_ON", (Opcode) CMPP_W_EQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_ON, "CMPP_W_EQ_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_OC", (Opcode) CMPP_W_EQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_OC, "CMPP_W_EQ_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_AN", (Opcode) CMPP_W_EQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_AN, "CMPP_W_EQ_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UN_AC", (Opcode) CMPP_W_EQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UN_AC, "CMPP_W_EQ_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_UN", (Opcode) CMPP_W_EQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_UN, "CMPP_W_EQ_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_UC", (Opcode) CMPP_W_EQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_UC, "CMPP_W_EQ_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_CN", (Opcode) CMPP_W_EQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_CN, "CMPP_W_EQ_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_CC", (Opcode) CMPP_W_EQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_CC, "CMPP_W_EQ_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_ON", (Opcode) CMPP_W_EQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_ON, "CMPP_W_EQ_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_OC", (Opcode) CMPP_W_EQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_OC, "CMPP_W_EQ_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_AN", (Opcode) CMPP_W_EQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_AN, "CMPP_W_EQ_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_UC_AC", (Opcode) CMPP_W_EQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_UC_AC, "CMPP_W_EQ_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_UN", (Opcode) CMPP_W_EQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_UN, "CMPP_W_EQ_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_UC", (Opcode) CMPP_W_EQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_UC, "CMPP_W_EQ_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_CN", (Opcode) CMPP_W_EQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_CN, "CMPP_W_EQ_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_CC", (Opcode) CMPP_W_EQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_CC, "CMPP_W_EQ_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_ON", (Opcode) CMPP_W_EQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_ON, "CMPP_W_EQ_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_OC", (Opcode) CMPP_W_EQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_OC, "CMPP_W_EQ_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_AN", (Opcode) CMPP_W_EQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_AN, "CMPP_W_EQ_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CN_AC", (Opcode) CMPP_W_EQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CN_AC, "CMPP_W_EQ_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_UN", (Opcode) CMPP_W_EQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_UN, "CMPP_W_EQ_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_UC", (Opcode) CMPP_W_EQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_UC, "CMPP_W_EQ_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_CN", (Opcode) CMPP_W_EQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_CN, "CMPP_W_EQ_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_CC", (Opcode) CMPP_W_EQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_CC, "CMPP_W_EQ_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_ON", (Opcode) CMPP_W_EQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_ON, "CMPP_W_EQ_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_OC", (Opcode) CMPP_W_EQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_OC, "CMPP_W_EQ_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_AN", (Opcode) CMPP_W_EQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_AN, "CMPP_W_EQ_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_CC_AC", (Opcode) CMPP_W_EQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_CC_AC, "CMPP_W_EQ_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_UN", (Opcode) CMPP_W_EQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_UN, "CMPP_W_EQ_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_UC", (Opcode) CMPP_W_EQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_UC, "CMPP_W_EQ_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_CN", (Opcode) CMPP_W_EQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_CN, "CMPP_W_EQ_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_CC", (Opcode) CMPP_W_EQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_CC, "CMPP_W_EQ_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_ON", (Opcode) CMPP_W_EQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_ON, "CMPP_W_EQ_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_OC", (Opcode) CMPP_W_EQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_OC, "CMPP_W_EQ_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_AN", (Opcode) CMPP_W_EQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_AN, "CMPP_W_EQ_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_ON_AC", (Opcode) CMPP_W_EQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_ON_AC, "CMPP_W_EQ_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_UN", (Opcode) CMPP_W_EQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_UN, "CMPP_W_EQ_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_UC", (Opcode) CMPP_W_EQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_UC, "CMPP_W_EQ_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_CN", (Opcode) CMPP_W_EQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_CN, "CMPP_W_EQ_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_CC", (Opcode) CMPP_W_EQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_CC, "CMPP_W_EQ_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_ON", (Opcode) CMPP_W_EQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_ON, "CMPP_W_EQ_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_OC", (Opcode) CMPP_W_EQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_OC, "CMPP_W_EQ_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_AN", (Opcode) CMPP_W_EQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_AN, "CMPP_W_EQ_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_OC_AC", (Opcode) CMPP_W_EQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_OC_AC, "CMPP_W_EQ_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_UN", (Opcode) CMPP_W_EQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_UN, "CMPP_W_EQ_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_UC", (Opcode) CMPP_W_EQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_UC, "CMPP_W_EQ_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_CN", (Opcode) CMPP_W_EQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_CN, "CMPP_W_EQ_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_CC", (Opcode) CMPP_W_EQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_CC, "CMPP_W_EQ_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_ON", (Opcode) CMPP_W_EQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_ON, "CMPP_W_EQ_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_OC", (Opcode) CMPP_W_EQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_OC, "CMPP_W_EQ_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_AN", (Opcode) CMPP_W_EQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_AN, "CMPP_W_EQ_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AN_AC", (Opcode) CMPP_W_EQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AN_AC, "CMPP_W_EQ_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_UN", (Opcode) CMPP_W_EQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_UN, "CMPP_W_EQ_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_UC", (Opcode) CMPP_W_EQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_UC, "CMPP_W_EQ_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_CN", (Opcode) CMPP_W_EQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_CN, "CMPP_W_EQ_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_CC", (Opcode) CMPP_W_EQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_CC, "CMPP_W_EQ_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_ON", (Opcode) CMPP_W_EQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_ON, "CMPP_W_EQ_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_OC", (Opcode) CMPP_W_EQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_OC, "CMPP_W_EQ_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_AN", (Opcode) CMPP_W_EQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_AN, "CMPP_W_EQ_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_EQ_AC_AC", (Opcode) CMPP_W_EQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_EQ_AC_AC, "CMPP_W_EQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1_3()
{
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_UN", (Opcode) CMPP_W_LT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_UN, "CMPP_W_LT_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_UC", (Opcode) CMPP_W_LT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_UC, "CMPP_W_LT_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_CN", (Opcode) CMPP_W_LT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_CN, "CMPP_W_LT_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_CC", (Opcode) CMPP_W_LT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_CC, "CMPP_W_LT_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_ON", (Opcode) CMPP_W_LT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_ON, "CMPP_W_LT_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_OC", (Opcode) CMPP_W_LT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_OC, "CMPP_W_LT_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_AN", (Opcode) CMPP_W_LT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_AN, "CMPP_W_LT_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UN_AC", (Opcode) CMPP_W_LT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UN_AC, "CMPP_W_LT_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LT_UC_UN", (Opcode) CMPP_W_LT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_UN, "CMPP_W_LT_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UC_UC", (Opcode) CMPP_W_LT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_UC, "CMPP_W_LT_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UC_CN", (Opcode) CMPP_W_LT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_CN, "CMPP_W_LT_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UC_CC", (Opcode) CMPP_W_LT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_CC, "CMPP_W_LT_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UC_ON", (Opcode) CMPP_W_LT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_ON, "CMPP_W_LT_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UC_OC", (Opcode) CMPP_W_LT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_OC, "CMPP_W_LT_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UC_AN", (Opcode) CMPP_W_LT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_AN, "CMPP_W_LT_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_UC_AC", (Opcode) CMPP_W_LT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_UC_AC, "CMPP_W_LT_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LT_CN_UN", (Opcode) CMPP_W_LT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_UN, "CMPP_W_LT_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CN_UC", (Opcode) CMPP_W_LT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_UC, "CMPP_W_LT_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CN_CN", (Opcode) CMPP_W_LT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_CN, "CMPP_W_LT_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CN_CC", (Opcode) CMPP_W_LT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_CC, "CMPP_W_LT_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CN_ON", (Opcode) CMPP_W_LT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_ON, "CMPP_W_LT_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CN_OC", (Opcode) CMPP_W_LT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_OC, "CMPP_W_LT_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CN_AN", (Opcode) CMPP_W_LT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_AN, "CMPP_W_LT_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CN_AC", (Opcode) CMPP_W_LT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CN_AC, "CMPP_W_LT_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LT_CC_UN", (Opcode) CMPP_W_LT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_UN, "CMPP_W_LT_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CC_UC", (Opcode) CMPP_W_LT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_UC, "CMPP_W_LT_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CC_CN", (Opcode) CMPP_W_LT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_CN, "CMPP_W_LT_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CC_CC", (Opcode) CMPP_W_LT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_CC, "CMPP_W_LT_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CC_ON", (Opcode) CMPP_W_LT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_ON, "CMPP_W_LT_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CC_OC", (Opcode) CMPP_W_LT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_OC, "CMPP_W_LT_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CC_AN", (Opcode) CMPP_W_LT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_AN, "CMPP_W_LT_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_CC_AC", (Opcode) CMPP_W_LT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_CC_AC, "CMPP_W_LT_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LT_ON_UN", (Opcode) CMPP_W_LT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_UN, "CMPP_W_LT_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_ON_UC", (Opcode) CMPP_W_LT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_UC, "CMPP_W_LT_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_ON_CN", (Opcode) CMPP_W_LT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_CN, "CMPP_W_LT_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_ON_CC", (Opcode) CMPP_W_LT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_CC, "CMPP_W_LT_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_ON_ON", (Opcode) CMPP_W_LT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_ON, "CMPP_W_LT_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_ON_OC", (Opcode) CMPP_W_LT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_OC, "CMPP_W_LT_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_ON_AN", (Opcode) CMPP_W_LT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_AN, "CMPP_W_LT_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_ON_AC", (Opcode) CMPP_W_LT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_ON_AC, "CMPP_W_LT_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LT_OC_UN", (Opcode) CMPP_W_LT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_UN, "CMPP_W_LT_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_OC_UC", (Opcode) CMPP_W_LT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_UC, "CMPP_W_LT_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_OC_CN", (Opcode) CMPP_W_LT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_CN, "CMPP_W_LT_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_OC_CC", (Opcode) CMPP_W_LT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_CC, "CMPP_W_LT_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_OC_ON", (Opcode) CMPP_W_LT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_ON, "CMPP_W_LT_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_OC_OC", (Opcode) CMPP_W_LT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_OC, "CMPP_W_LT_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_OC_AN", (Opcode) CMPP_W_LT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_AN, "CMPP_W_LT_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_OC_AC", (Opcode) CMPP_W_LT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_OC_AC, "CMPP_W_LT_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LT_AN_UN", (Opcode) CMPP_W_LT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_UN, "CMPP_W_LT_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AN_UC", (Opcode) CMPP_W_LT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_UC, "CMPP_W_LT_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AN_CN", (Opcode) CMPP_W_LT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_CN, "CMPP_W_LT_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AN_CC", (Opcode) CMPP_W_LT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_CC, "CMPP_W_LT_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AN_ON", (Opcode) CMPP_W_LT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_ON, "CMPP_W_LT_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AN_OC", (Opcode) CMPP_W_LT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_OC, "CMPP_W_LT_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AN_AN", (Opcode) CMPP_W_LT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_AN, "CMPP_W_LT_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AN_AC", (Opcode) CMPP_W_LT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AN_AC, "CMPP_W_LT_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LT_AC_UN", (Opcode) CMPP_W_LT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_UN, "CMPP_W_LT_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AC_UC", (Opcode) CMPP_W_LT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_UC, "CMPP_W_LT_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AC_CN", (Opcode) CMPP_W_LT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_CN, "CMPP_W_LT_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AC_CC", (Opcode) CMPP_W_LT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_CC, "CMPP_W_LT_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AC_ON", (Opcode) CMPP_W_LT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_ON, "CMPP_W_LT_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AC_OC", (Opcode) CMPP_W_LT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_OC, "CMPP_W_LT_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AC_AN", (Opcode) CMPP_W_LT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_AN, "CMPP_W_LT_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LT_AC_AC", (Opcode) CMPP_W_LT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LT_AC_AC, "CMPP_W_LT_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1_4()
{
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_UN", (Opcode) CMPP_W_LEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_UN, "CMPP_W_LEQ_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_UC", (Opcode) CMPP_W_LEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_UC, "CMPP_W_LEQ_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_CN", (Opcode) CMPP_W_LEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_CN, "CMPP_W_LEQ_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_CC", (Opcode) CMPP_W_LEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_CC, "CMPP_W_LEQ_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_ON", (Opcode) CMPP_W_LEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_ON, "CMPP_W_LEQ_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_OC", (Opcode) CMPP_W_LEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_OC, "CMPP_W_LEQ_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_AN", (Opcode) CMPP_W_LEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_AN, "CMPP_W_LEQ_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UN_AC", (Opcode) CMPP_W_LEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UN_AC, "CMPP_W_LEQ_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_UN", (Opcode) CMPP_W_LEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_UN, "CMPP_W_LEQ_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_UC", (Opcode) CMPP_W_LEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_UC, "CMPP_W_LEQ_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_CN", (Opcode) CMPP_W_LEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_CN, "CMPP_W_LEQ_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_CC", (Opcode) CMPP_W_LEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_CC, "CMPP_W_LEQ_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_ON", (Opcode) CMPP_W_LEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_ON, "CMPP_W_LEQ_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_OC", (Opcode) CMPP_W_LEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_OC, "CMPP_W_LEQ_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_AN", (Opcode) CMPP_W_LEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_AN, "CMPP_W_LEQ_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_UC_AC", (Opcode) CMPP_W_LEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_UC_AC, "CMPP_W_LEQ_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_UN", (Opcode) CMPP_W_LEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_UN, "CMPP_W_LEQ_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_UC", (Opcode) CMPP_W_LEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_UC, "CMPP_W_LEQ_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_CN", (Opcode) CMPP_W_LEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_CN, "CMPP_W_LEQ_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_CC", (Opcode) CMPP_W_LEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_CC, "CMPP_W_LEQ_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_ON", (Opcode) CMPP_W_LEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_ON, "CMPP_W_LEQ_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_OC", (Opcode) CMPP_W_LEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_OC, "CMPP_W_LEQ_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_AN", (Opcode) CMPP_W_LEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_AN, "CMPP_W_LEQ_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CN_AC", (Opcode) CMPP_W_LEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CN_AC, "CMPP_W_LEQ_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_UN", (Opcode) CMPP_W_LEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_UN, "CMPP_W_LEQ_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_UC", (Opcode) CMPP_W_LEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_UC, "CMPP_W_LEQ_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_CN", (Opcode) CMPP_W_LEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_CN, "CMPP_W_LEQ_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_CC", (Opcode) CMPP_W_LEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_CC, "CMPP_W_LEQ_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_ON", (Opcode) CMPP_W_LEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_ON, "CMPP_W_LEQ_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_OC", (Opcode) CMPP_W_LEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_OC, "CMPP_W_LEQ_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_AN", (Opcode) CMPP_W_LEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_AN, "CMPP_W_LEQ_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_CC_AC", (Opcode) CMPP_W_LEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_CC_AC, "CMPP_W_LEQ_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_UN", (Opcode) CMPP_W_LEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_UN, "CMPP_W_LEQ_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_UC", (Opcode) CMPP_W_LEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_UC, "CMPP_W_LEQ_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_CN", (Opcode) CMPP_W_LEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_CN, "CMPP_W_LEQ_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_CC", (Opcode) CMPP_W_LEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_CC, "CMPP_W_LEQ_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_ON", (Opcode) CMPP_W_LEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_ON, "CMPP_W_LEQ_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_OC", (Opcode) CMPP_W_LEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_OC, "CMPP_W_LEQ_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_AN", (Opcode) CMPP_W_LEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_AN, "CMPP_W_LEQ_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_ON_AC", (Opcode) CMPP_W_LEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_ON_AC, "CMPP_W_LEQ_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_UN", (Opcode) CMPP_W_LEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_UN, "CMPP_W_LEQ_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_UC", (Opcode) CMPP_W_LEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_UC, "CMPP_W_LEQ_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_CN", (Opcode) CMPP_W_LEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_CN, "CMPP_W_LEQ_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_CC", (Opcode) CMPP_W_LEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_CC, "CMPP_W_LEQ_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_ON", (Opcode) CMPP_W_LEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_ON, "CMPP_W_LEQ_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_OC", (Opcode) CMPP_W_LEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_OC, "CMPP_W_LEQ_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_AN", (Opcode) CMPP_W_LEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_AN, "CMPP_W_LEQ_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_OC_AC", (Opcode) CMPP_W_LEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_OC_AC, "CMPP_W_LEQ_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_UN", (Opcode) CMPP_W_LEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_UN, "CMPP_W_LEQ_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_UC", (Opcode) CMPP_W_LEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_UC, "CMPP_W_LEQ_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_CN", (Opcode) CMPP_W_LEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_CN, "CMPP_W_LEQ_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_CC", (Opcode) CMPP_W_LEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_CC, "CMPP_W_LEQ_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_ON", (Opcode) CMPP_W_LEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_ON, "CMPP_W_LEQ_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_OC", (Opcode) CMPP_W_LEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_OC, "CMPP_W_LEQ_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_AN", (Opcode) CMPP_W_LEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_AN, "CMPP_W_LEQ_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AN_AC", (Opcode) CMPP_W_LEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AN_AC, "CMPP_W_LEQ_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_UN", (Opcode) CMPP_W_LEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_UN, "CMPP_W_LEQ_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_UC", (Opcode) CMPP_W_LEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_UC, "CMPP_W_LEQ_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_CN", (Opcode) CMPP_W_LEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_CN, "CMPP_W_LEQ_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_CC", (Opcode) CMPP_W_LEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_CC, "CMPP_W_LEQ_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_ON", (Opcode) CMPP_W_LEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_ON, "CMPP_W_LEQ_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_OC", (Opcode) CMPP_W_LEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_OC, "CMPP_W_LEQ_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_AN", (Opcode) CMPP_W_LEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_AN, "CMPP_W_LEQ_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LEQ_AC_AC", (Opcode) CMPP_W_LEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LEQ_AC_AC, "CMPP_W_LEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1_5()
{
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_UN", (Opcode) CMPP_W_GT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_UN, "CMPP_W_GT_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_UC", (Opcode) CMPP_W_GT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_UC, "CMPP_W_GT_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_CN", (Opcode) CMPP_W_GT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_CN, "CMPP_W_GT_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_CC", (Opcode) CMPP_W_GT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_CC, "CMPP_W_GT_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_ON", (Opcode) CMPP_W_GT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_ON, "CMPP_W_GT_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_OC", (Opcode) CMPP_W_GT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_OC, "CMPP_W_GT_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_AN", (Opcode) CMPP_W_GT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_AN, "CMPP_W_GT_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UN_AC", (Opcode) CMPP_W_GT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UN_AC, "CMPP_W_GT_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GT_UC_UN", (Opcode) CMPP_W_GT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_UN, "CMPP_W_GT_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UC_UC", (Opcode) CMPP_W_GT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_UC, "CMPP_W_GT_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UC_CN", (Opcode) CMPP_W_GT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_CN, "CMPP_W_GT_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UC_CC", (Opcode) CMPP_W_GT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_CC, "CMPP_W_GT_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UC_ON", (Opcode) CMPP_W_GT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_ON, "CMPP_W_GT_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UC_OC", (Opcode) CMPP_W_GT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_OC, "CMPP_W_GT_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UC_AN", (Opcode) CMPP_W_GT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_AN, "CMPP_W_GT_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_UC_AC", (Opcode) CMPP_W_GT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_UC_AC, "CMPP_W_GT_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GT_CN_UN", (Opcode) CMPP_W_GT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_UN, "CMPP_W_GT_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CN_UC", (Opcode) CMPP_W_GT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_UC, "CMPP_W_GT_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CN_CN", (Opcode) CMPP_W_GT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_CN, "CMPP_W_GT_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CN_CC", (Opcode) CMPP_W_GT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_CC, "CMPP_W_GT_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CN_ON", (Opcode) CMPP_W_GT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_ON, "CMPP_W_GT_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CN_OC", (Opcode) CMPP_W_GT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_OC, "CMPP_W_GT_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CN_AN", (Opcode) CMPP_W_GT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_AN, "CMPP_W_GT_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CN_AC", (Opcode) CMPP_W_GT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CN_AC, "CMPP_W_GT_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GT_CC_UN", (Opcode) CMPP_W_GT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_UN, "CMPP_W_GT_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CC_UC", (Opcode) CMPP_W_GT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_UC, "CMPP_W_GT_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CC_CN", (Opcode) CMPP_W_GT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_CN, "CMPP_W_GT_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CC_CC", (Opcode) CMPP_W_GT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_CC, "CMPP_W_GT_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CC_ON", (Opcode) CMPP_W_GT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_ON, "CMPP_W_GT_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CC_OC", (Opcode) CMPP_W_GT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_OC, "CMPP_W_GT_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CC_AN", (Opcode) CMPP_W_GT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_AN, "CMPP_W_GT_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_CC_AC", (Opcode) CMPP_W_GT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_CC_AC, "CMPP_W_GT_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GT_ON_UN", (Opcode) CMPP_W_GT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_UN, "CMPP_W_GT_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_ON_UC", (Opcode) CMPP_W_GT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_UC, "CMPP_W_GT_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_ON_CN", (Opcode) CMPP_W_GT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_CN, "CMPP_W_GT_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_ON_CC", (Opcode) CMPP_W_GT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_CC, "CMPP_W_GT_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_ON_ON", (Opcode) CMPP_W_GT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_ON, "CMPP_W_GT_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_ON_OC", (Opcode) CMPP_W_GT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_OC, "CMPP_W_GT_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_ON_AN", (Opcode) CMPP_W_GT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_AN, "CMPP_W_GT_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_ON_AC", (Opcode) CMPP_W_GT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_ON_AC, "CMPP_W_GT_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GT_OC_UN", (Opcode) CMPP_W_GT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_UN, "CMPP_W_GT_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_OC_UC", (Opcode) CMPP_W_GT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_UC, "CMPP_W_GT_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_OC_CN", (Opcode) CMPP_W_GT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_CN, "CMPP_W_GT_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_OC_CC", (Opcode) CMPP_W_GT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_CC, "CMPP_W_GT_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_OC_ON", (Opcode) CMPP_W_GT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_ON, "CMPP_W_GT_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_OC_OC", (Opcode) CMPP_W_GT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_OC, "CMPP_W_GT_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_OC_AN", (Opcode) CMPP_W_GT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_AN, "CMPP_W_GT_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_OC_AC", (Opcode) CMPP_W_GT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_OC_AC, "CMPP_W_GT_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GT_AN_UN", (Opcode) CMPP_W_GT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_UN, "CMPP_W_GT_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AN_UC", (Opcode) CMPP_W_GT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_UC, "CMPP_W_GT_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AN_CN", (Opcode) CMPP_W_GT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_CN, "CMPP_W_GT_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AN_CC", (Opcode) CMPP_W_GT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_CC, "CMPP_W_GT_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AN_ON", (Opcode) CMPP_W_GT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_ON, "CMPP_W_GT_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AN_OC", (Opcode) CMPP_W_GT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_OC, "CMPP_W_GT_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AN_AN", (Opcode) CMPP_W_GT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_AN, "CMPP_W_GT_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AN_AC", (Opcode) CMPP_W_GT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AN_AC, "CMPP_W_GT_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GT_AC_UN", (Opcode) CMPP_W_GT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_UN, "CMPP_W_GT_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AC_UC", (Opcode) CMPP_W_GT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_UC, "CMPP_W_GT_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AC_CN", (Opcode) CMPP_W_GT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_CN, "CMPP_W_GT_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AC_CC", (Opcode) CMPP_W_GT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_CC, "CMPP_W_GT_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AC_ON", (Opcode) CMPP_W_GT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_ON, "CMPP_W_GT_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AC_OC", (Opcode) CMPP_W_GT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_OC, "CMPP_W_GT_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AC_AN", (Opcode) CMPP_W_GT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_AN, "CMPP_W_GT_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GT_AC_AC", (Opcode) CMPP_W_GT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GT_AC_AC, "CMPP_W_GT_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1_6()
{
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_UN", (Opcode) CMPP_W_GEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_UN, "CMPP_W_GEQ_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_UC", (Opcode) CMPP_W_GEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_UC, "CMPP_W_GEQ_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_CN", (Opcode) CMPP_W_GEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_CN, "CMPP_W_GEQ_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_CC", (Opcode) CMPP_W_GEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_CC, "CMPP_W_GEQ_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_ON", (Opcode) CMPP_W_GEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_ON, "CMPP_W_GEQ_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_OC", (Opcode) CMPP_W_GEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_OC, "CMPP_W_GEQ_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_AN", (Opcode) CMPP_W_GEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_AN, "CMPP_W_GEQ_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UN_AC", (Opcode) CMPP_W_GEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UN_AC, "CMPP_W_GEQ_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_UN", (Opcode) CMPP_W_GEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_UN, "CMPP_W_GEQ_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_UC", (Opcode) CMPP_W_GEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_UC, "CMPP_W_GEQ_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_CN", (Opcode) CMPP_W_GEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_CN, "CMPP_W_GEQ_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_CC", (Opcode) CMPP_W_GEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_CC, "CMPP_W_GEQ_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_ON", (Opcode) CMPP_W_GEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_ON, "CMPP_W_GEQ_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_OC", (Opcode) CMPP_W_GEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_OC, "CMPP_W_GEQ_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_AN", (Opcode) CMPP_W_GEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_AN, "CMPP_W_GEQ_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_UC_AC", (Opcode) CMPP_W_GEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_UC_AC, "CMPP_W_GEQ_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_UN", (Opcode) CMPP_W_GEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_UN, "CMPP_W_GEQ_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_UC", (Opcode) CMPP_W_GEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_UC, "CMPP_W_GEQ_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_CN", (Opcode) CMPP_W_GEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_CN, "CMPP_W_GEQ_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_CC", (Opcode) CMPP_W_GEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_CC, "CMPP_W_GEQ_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_ON", (Opcode) CMPP_W_GEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_ON, "CMPP_W_GEQ_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_OC", (Opcode) CMPP_W_GEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_OC, "CMPP_W_GEQ_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_AN", (Opcode) CMPP_W_GEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_AN, "CMPP_W_GEQ_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CN_AC", (Opcode) CMPP_W_GEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CN_AC, "CMPP_W_GEQ_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_UN", (Opcode) CMPP_W_GEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_UN, "CMPP_W_GEQ_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_UC", (Opcode) CMPP_W_GEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_UC, "CMPP_W_GEQ_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_CN", (Opcode) CMPP_W_GEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_CN, "CMPP_W_GEQ_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_CC", (Opcode) CMPP_W_GEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_CC, "CMPP_W_GEQ_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_ON", (Opcode) CMPP_W_GEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_ON, "CMPP_W_GEQ_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_OC", (Opcode) CMPP_W_GEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_OC, "CMPP_W_GEQ_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_AN", (Opcode) CMPP_W_GEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_AN, "CMPP_W_GEQ_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_CC_AC", (Opcode) CMPP_W_GEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_CC_AC, "CMPP_W_GEQ_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_UN", (Opcode) CMPP_W_GEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_UN, "CMPP_W_GEQ_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_UC", (Opcode) CMPP_W_GEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_UC, "CMPP_W_GEQ_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_CN", (Opcode) CMPP_W_GEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_CN, "CMPP_W_GEQ_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_CC", (Opcode) CMPP_W_GEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_CC, "CMPP_W_GEQ_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_ON", (Opcode) CMPP_W_GEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_ON, "CMPP_W_GEQ_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_OC", (Opcode) CMPP_W_GEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_OC, "CMPP_W_GEQ_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_AN", (Opcode) CMPP_W_GEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_AN, "CMPP_W_GEQ_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_ON_AC", (Opcode) CMPP_W_GEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_ON_AC, "CMPP_W_GEQ_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_UN", (Opcode) CMPP_W_GEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_UN, "CMPP_W_GEQ_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_UC", (Opcode) CMPP_W_GEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_UC, "CMPP_W_GEQ_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_CN", (Opcode) CMPP_W_GEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_CN, "CMPP_W_GEQ_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_CC", (Opcode) CMPP_W_GEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_CC, "CMPP_W_GEQ_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_ON", (Opcode) CMPP_W_GEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_ON, "CMPP_W_GEQ_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_OC", (Opcode) CMPP_W_GEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_OC, "CMPP_W_GEQ_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_AN", (Opcode) CMPP_W_GEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_AN, "CMPP_W_GEQ_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_OC_AC", (Opcode) CMPP_W_GEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_OC_AC, "CMPP_W_GEQ_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_UN", (Opcode) CMPP_W_GEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_UN, "CMPP_W_GEQ_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_UC", (Opcode) CMPP_W_GEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_UC, "CMPP_W_GEQ_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_CN", (Opcode) CMPP_W_GEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_CN, "CMPP_W_GEQ_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_CC", (Opcode) CMPP_W_GEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_CC, "CMPP_W_GEQ_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_ON", (Opcode) CMPP_W_GEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_ON, "CMPP_W_GEQ_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_OC", (Opcode) CMPP_W_GEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_OC, "CMPP_W_GEQ_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_AN", (Opcode) CMPP_W_GEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_AN, "CMPP_W_GEQ_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AN_AC", (Opcode) CMPP_W_GEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AN_AC, "CMPP_W_GEQ_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_UN", (Opcode) CMPP_W_GEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_UN, "CMPP_W_GEQ_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_UC", (Opcode) CMPP_W_GEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_UC, "CMPP_W_GEQ_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_CN", (Opcode) CMPP_W_GEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_CN, "CMPP_W_GEQ_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_CC", (Opcode) CMPP_W_GEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_CC, "CMPP_W_GEQ_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_ON", (Opcode) CMPP_W_GEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_ON, "CMPP_W_GEQ_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_OC", (Opcode) CMPP_W_GEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_OC, "CMPP_W_GEQ_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_AN", (Opcode) CMPP_W_GEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_AN, "CMPP_W_GEQ_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_GEQ_AC_AC", (Opcode) CMPP_W_GEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_GEQ_AC_AC, "CMPP_W_GEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1_7()
{
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_UN", (Opcode) CMPP_W_SV_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_UN, "CMPP_W_SV_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_UC", (Opcode) CMPP_W_SV_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_UC, "CMPP_W_SV_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_CN", (Opcode) CMPP_W_SV_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_CN, "CMPP_W_SV_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_CC", (Opcode) CMPP_W_SV_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_CC, "CMPP_W_SV_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_ON", (Opcode) CMPP_W_SV_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_ON, "CMPP_W_SV_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_OC", (Opcode) CMPP_W_SV_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_OC, "CMPP_W_SV_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_AN", (Opcode) CMPP_W_SV_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_AN, "CMPP_W_SV_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UN_AC", (Opcode) CMPP_W_SV_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UN_AC, "CMPP_W_SV_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_SV_UC_UN", (Opcode) CMPP_W_SV_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_UN, "CMPP_W_SV_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UC_UC", (Opcode) CMPP_W_SV_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_UC, "CMPP_W_SV_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UC_CN", (Opcode) CMPP_W_SV_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_CN, "CMPP_W_SV_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UC_CC", (Opcode) CMPP_W_SV_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_CC, "CMPP_W_SV_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UC_ON", (Opcode) CMPP_W_SV_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_ON, "CMPP_W_SV_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UC_OC", (Opcode) CMPP_W_SV_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_OC, "CMPP_W_SV_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UC_AN", (Opcode) CMPP_W_SV_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_AN, "CMPP_W_SV_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_UC_AC", (Opcode) CMPP_W_SV_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_UC_AC, "CMPP_W_SV_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_SV_CN_UN", (Opcode) CMPP_W_SV_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_UN, "CMPP_W_SV_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CN_UC", (Opcode) CMPP_W_SV_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_UC, "CMPP_W_SV_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CN_CN", (Opcode) CMPP_W_SV_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_CN, "CMPP_W_SV_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CN_CC", (Opcode) CMPP_W_SV_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_CC, "CMPP_W_SV_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CN_ON", (Opcode) CMPP_W_SV_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_ON, "CMPP_W_SV_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CN_OC", (Opcode) CMPP_W_SV_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_OC, "CMPP_W_SV_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CN_AN", (Opcode) CMPP_W_SV_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_AN, "CMPP_W_SV_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CN_AC", (Opcode) CMPP_W_SV_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CN_AC, "CMPP_W_SV_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_SV_CC_UN", (Opcode) CMPP_W_SV_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_UN, "CMPP_W_SV_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CC_UC", (Opcode) CMPP_W_SV_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_UC, "CMPP_W_SV_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CC_CN", (Opcode) CMPP_W_SV_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_CN, "CMPP_W_SV_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CC_CC", (Opcode) CMPP_W_SV_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_CC, "CMPP_W_SV_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CC_ON", (Opcode) CMPP_W_SV_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_ON, "CMPP_W_SV_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CC_OC", (Opcode) CMPP_W_SV_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_OC, "CMPP_W_SV_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CC_AN", (Opcode) CMPP_W_SV_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_AN, "CMPP_W_SV_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_CC_AC", (Opcode) CMPP_W_SV_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_CC_AC, "CMPP_W_SV_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_SV_ON_UN", (Opcode) CMPP_W_SV_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_UN, "CMPP_W_SV_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_ON_UC", (Opcode) CMPP_W_SV_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_UC, "CMPP_W_SV_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_ON_CN", (Opcode) CMPP_W_SV_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_CN, "CMPP_W_SV_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_ON_CC", (Opcode) CMPP_W_SV_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_CC, "CMPP_W_SV_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_ON_ON", (Opcode) CMPP_W_SV_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_ON, "CMPP_W_SV_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_ON_OC", (Opcode) CMPP_W_SV_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_OC, "CMPP_W_SV_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_ON_AN", (Opcode) CMPP_W_SV_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_AN, "CMPP_W_SV_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_ON_AC", (Opcode) CMPP_W_SV_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_ON_AC, "CMPP_W_SV_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_SV_OC_UN", (Opcode) CMPP_W_SV_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_UN, "CMPP_W_SV_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_OC_UC", (Opcode) CMPP_W_SV_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_UC, "CMPP_W_SV_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_W_SV_OC_CN", (Opcode) CMPP_W_SV_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_CN, "CMPP_W_SV_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_OC_CC", (Opcode) CMPP_W_SV_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_CC, "CMPP_W_SV_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_OC_ON", (Opcode) CMPP_W_SV_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_ON, "CMPP_W_SV_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_OC_OC", (Opcode) CMPP_W_SV_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_OC, "CMPP_W_SV_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_OC_AN", (Opcode) CMPP_W_SV_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_AN, "CMPP_W_SV_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_OC_AC", (Opcode) CMPP_W_SV_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_OC_AC, "CMPP_W_SV_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_SV_AN_UN", (Opcode) CMPP_W_SV_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_UN, "CMPP_W_SV_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AN_UC", (Opcode) CMPP_W_SV_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_UC, "CMPP_W_SV_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AN_CN", (Opcode) CMPP_W_SV_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_CN, "CMPP_W_SV_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AN_CC", (Opcode) CMPP_W_SV_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_CC, "CMPP_W_SV_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AN_ON", (Opcode) CMPP_W_SV_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_ON, "CMPP_W_SV_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AN_OC", (Opcode) CMPP_W_SV_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_OC, "CMPP_W_SV_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AN_AN", (Opcode) CMPP_W_SV_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_AN, "CMPP_W_SV_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AN_AC", (Opcode) CMPP_W_SV_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AN_AC, "CMPP_W_SV_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_SV_AC_UN", (Opcode) CMPP_W_SV_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_UN, "CMPP_W_SV_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AC_UC", (Opcode) CMPP_W_SV_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_UC, "CMPP_W_SV_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AC_CN", (Opcode) CMPP_W_SV_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_CN, "CMPP_W_SV_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AC_CC", (Opcode) CMPP_W_SV_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_CC, "CMPP_W_SV_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AC_ON", (Opcode) CMPP_W_SV_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_ON, "CMPP_W_SV_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AC_OC", (Opcode) CMPP_W_SV_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_OC, "CMPP_W_SV_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AC_AN", (Opcode) CMPP_W_SV_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_AN, "CMPP_W_SV_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_SV_AC_AC", (Opcode) CMPP_W_SV_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_SV_AC_AC, "CMPP_W_SV_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1_8()
{
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_UN", (Opcode) CMPP_W_OD_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_UN, "CMPP_W_OD_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_UC", (Opcode) CMPP_W_OD_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_UC, "CMPP_W_OD_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_CN", (Opcode) CMPP_W_OD_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_CN, "CMPP_W_OD_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_CC", (Opcode) CMPP_W_OD_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_CC, "CMPP_W_OD_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_ON", (Opcode) CMPP_W_OD_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_ON, "CMPP_W_OD_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_OC", (Opcode) CMPP_W_OD_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_OC, "CMPP_W_OD_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_AN", (Opcode) CMPP_W_OD_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_AN, "CMPP_W_OD_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UN_AC", (Opcode) CMPP_W_OD_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UN_AC, "CMPP_W_OD_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_OD_UC_UN", (Opcode) CMPP_W_OD_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_UN, "CMPP_W_OD_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UC_UC", (Opcode) CMPP_W_OD_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_UC, "CMPP_W_OD_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UC_CN", (Opcode) CMPP_W_OD_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_CN, "CMPP_W_OD_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UC_CC", (Opcode) CMPP_W_OD_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_CC, "CMPP_W_OD_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UC_ON", (Opcode) CMPP_W_OD_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_ON, "CMPP_W_OD_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UC_OC", (Opcode) CMPP_W_OD_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_OC, "CMPP_W_OD_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UC_AN", (Opcode) CMPP_W_OD_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_AN, "CMPP_W_OD_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_UC_AC", (Opcode) CMPP_W_OD_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_UC_AC, "CMPP_W_OD_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_OD_CN_UN", (Opcode) CMPP_W_OD_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_UN, "CMPP_W_OD_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CN_UC", (Opcode) CMPP_W_OD_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_UC, "CMPP_W_OD_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CN_CN", (Opcode) CMPP_W_OD_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_CN, "CMPP_W_OD_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CN_CC", (Opcode) CMPP_W_OD_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_CC, "CMPP_W_OD_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CN_ON", (Opcode) CMPP_W_OD_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_ON, "CMPP_W_OD_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CN_OC", (Opcode) CMPP_W_OD_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_OC, "CMPP_W_OD_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CN_AN", (Opcode) CMPP_W_OD_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_AN, "CMPP_W_OD_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CN_AC", (Opcode) CMPP_W_OD_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CN_AC, "CMPP_W_OD_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_OD_CC_UN", (Opcode) CMPP_W_OD_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_UN, "CMPP_W_OD_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CC_UC", (Opcode) CMPP_W_OD_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_UC, "CMPP_W_OD_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CC_CN", (Opcode) CMPP_W_OD_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_CN, "CMPP_W_OD_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CC_CC", (Opcode) CMPP_W_OD_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_CC, "CMPP_W_OD_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CC_ON", (Opcode) CMPP_W_OD_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_ON, "CMPP_W_OD_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CC_OC", (Opcode) CMPP_W_OD_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_OC, "CMPP_W_OD_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CC_AN", (Opcode) CMPP_W_OD_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_AN, "CMPP_W_OD_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_CC_AC", (Opcode) CMPP_W_OD_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_CC_AC, "CMPP_W_OD_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_OD_ON_UN", (Opcode) CMPP_W_OD_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_UN, "CMPP_W_OD_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_ON_UC", (Opcode) CMPP_W_OD_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_UC, "CMPP_W_OD_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_ON_CN", (Opcode) CMPP_W_OD_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_CN, "CMPP_W_OD_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_ON_CC", (Opcode) CMPP_W_OD_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_CC, "CMPP_W_OD_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_ON_ON", (Opcode) CMPP_W_OD_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_ON, "CMPP_W_OD_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_ON_OC", (Opcode) CMPP_W_OD_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_OC, "CMPP_W_OD_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_ON_AN", (Opcode) CMPP_W_OD_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_AN, "CMPP_W_OD_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_ON_AC", (Opcode) CMPP_W_OD_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_ON_AC, "CMPP_W_OD_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_OD_OC_UN", (Opcode) CMPP_W_OD_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_UN, "CMPP_W_OD_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_OC_UC", (Opcode) CMPP_W_OD_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_UC, "CMPP_W_OD_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_OC_CN", (Opcode) CMPP_W_OD_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_CN, "CMPP_W_OD_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_OC_CC", (Opcode) CMPP_W_OD_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_CC, "CMPP_W_OD_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_OC_ON", (Opcode) CMPP_W_OD_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_ON, "CMPP_W_OD_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_OC_OC", (Opcode) CMPP_W_OD_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_OC, "CMPP_W_OD_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_OC_AN", (Opcode) CMPP_W_OD_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_AN, "CMPP_W_OD_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_OC_AC", (Opcode) CMPP_W_OD_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_OC_AC, "CMPP_W_OD_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_OD_AN_UN", (Opcode) CMPP_W_OD_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_UN, "CMPP_W_OD_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AN_UC", (Opcode) CMPP_W_OD_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_UC, "CMPP_W_OD_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AN_CN", (Opcode) CMPP_W_OD_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_CN, "CMPP_W_OD_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AN_CC", (Opcode) CMPP_W_OD_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_CC, "CMPP_W_OD_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AN_ON", (Opcode) CMPP_W_OD_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_ON, "CMPP_W_OD_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AN_OC", (Opcode) CMPP_W_OD_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_OC, "CMPP_W_OD_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AN_AN", (Opcode) CMPP_W_OD_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_AN, "CMPP_W_OD_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AN_AC", (Opcode) CMPP_W_OD_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AN_AC, "CMPP_W_OD_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_OD_AC_UN", (Opcode) CMPP_W_OD_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_UN, "CMPP_W_OD_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AC_UC", (Opcode) CMPP_W_OD_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_UC, "CMPP_W_OD_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AC_CN", (Opcode) CMPP_W_OD_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_CN, "CMPP_W_OD_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AC_CC", (Opcode) CMPP_W_OD_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_CC, "CMPP_W_OD_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AC_ON", (Opcode) CMPP_W_OD_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_ON, "CMPP_W_OD_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AC_OC", (Opcode) CMPP_W_OD_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_OC, "CMPP_W_OD_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AC_AN", (Opcode) CMPP_W_OD_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_AN, "CMPP_W_OD_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_OD_AC_AC", (Opcode) CMPP_W_OD_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_OD_AC_AC, "CMPP_W_OD_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp1()
{
   el_init_elcor_opcode_maps_cmpp1_1() ;
   el_init_elcor_opcode_maps_cmpp1_2() ;
   el_init_elcor_opcode_maps_cmpp1_3() ;
   el_init_elcor_opcode_maps_cmpp1_4() ;
   el_init_elcor_opcode_maps_cmpp1_5() ;
   el_init_elcor_opcode_maps_cmpp1_6() ;
   el_init_elcor_opcode_maps_cmpp1_7() ;
   el_init_elcor_opcode_maps_cmpp1_8() ;
}

static void el_init_elcor_opcode_maps_cmpp2_1()
{
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_UN", (Opcode) CMPP_W_TRUE_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_UN, "CMPP_W_TRUE_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_UC", (Opcode) CMPP_W_TRUE_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_UC, "CMPP_W_TRUE_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_CN", (Opcode) CMPP_W_TRUE_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_CN, "CMPP_W_TRUE_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_CC", (Opcode) CMPP_W_TRUE_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_CC, "CMPP_W_TRUE_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_ON", (Opcode) CMPP_W_TRUE_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_ON, "CMPP_W_TRUE_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_OC", (Opcode) CMPP_W_TRUE_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_OC, "CMPP_W_TRUE_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_AN", (Opcode) CMPP_W_TRUE_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_AN, "CMPP_W_TRUE_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UN_AC", (Opcode) CMPP_W_TRUE_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UN_AC, "CMPP_W_TRUE_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_UN", (Opcode) CMPP_W_TRUE_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_UN, "CMPP_W_TRUE_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_UC", (Opcode) CMPP_W_TRUE_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_UC, "CMPP_W_TRUE_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_CN", (Opcode) CMPP_W_TRUE_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_CN, "CMPP_W_TRUE_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_CC", (Opcode) CMPP_W_TRUE_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_CC, "CMPP_W_TRUE_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_ON", (Opcode) CMPP_W_TRUE_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_ON, "CMPP_W_TRUE_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_OC", (Opcode) CMPP_W_TRUE_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_OC, "CMPP_W_TRUE_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_AN", (Opcode) CMPP_W_TRUE_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_AN, "CMPP_W_TRUE_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_UC_AC", (Opcode) CMPP_W_TRUE_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_UC_AC, "CMPP_W_TRUE_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_UN", (Opcode) CMPP_W_TRUE_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_UN, "CMPP_W_TRUE_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_UC", (Opcode) CMPP_W_TRUE_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_UC, "CMPP_W_TRUE_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_CN", (Opcode) CMPP_W_TRUE_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_CN, "CMPP_W_TRUE_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_CC", (Opcode) CMPP_W_TRUE_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_CC, "CMPP_W_TRUE_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_ON", (Opcode) CMPP_W_TRUE_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_ON, "CMPP_W_TRUE_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_OC", (Opcode) CMPP_W_TRUE_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_OC, "CMPP_W_TRUE_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_AN", (Opcode) CMPP_W_TRUE_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_AN, "CMPP_W_TRUE_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CN_AC", (Opcode) CMPP_W_TRUE_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CN_AC, "CMPP_W_TRUE_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_UN", (Opcode) CMPP_W_TRUE_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_UN, "CMPP_W_TRUE_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_UC", (Opcode) CMPP_W_TRUE_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_UC, "CMPP_W_TRUE_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_CN", (Opcode) CMPP_W_TRUE_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_CN, "CMPP_W_TRUE_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_CC", (Opcode) CMPP_W_TRUE_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_CC, "CMPP_W_TRUE_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_ON", (Opcode) CMPP_W_TRUE_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_ON, "CMPP_W_TRUE_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_OC", (Opcode) CMPP_W_TRUE_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_OC, "CMPP_W_TRUE_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_AN", (Opcode) CMPP_W_TRUE_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_AN, "CMPP_W_TRUE_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_CC_AC", (Opcode) CMPP_W_TRUE_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_CC_AC, "CMPP_W_TRUE_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_UN", (Opcode) CMPP_W_TRUE_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_UN, "CMPP_W_TRUE_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_UC", (Opcode) CMPP_W_TRUE_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_UC, "CMPP_W_TRUE_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_CN", (Opcode) CMPP_W_TRUE_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_CN, "CMPP_W_TRUE_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_CC", (Opcode) CMPP_W_TRUE_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_CC, "CMPP_W_TRUE_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_ON", (Opcode) CMPP_W_TRUE_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_ON, "CMPP_W_TRUE_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_OC", (Opcode) CMPP_W_TRUE_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_OC, "CMPP_W_TRUE_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_AN", (Opcode) CMPP_W_TRUE_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_AN, "CMPP_W_TRUE_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_ON_AC", (Opcode) CMPP_W_TRUE_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_ON_AC, "CMPP_W_TRUE_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_UN", (Opcode) CMPP_W_TRUE_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_UN, "CMPP_W_TRUE_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_UC", (Opcode) CMPP_W_TRUE_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_UC, "CMPP_W_TRUE_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_CN", (Opcode) CMPP_W_TRUE_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_CN, "CMPP_W_TRUE_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_CC", (Opcode) CMPP_W_TRUE_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_CC, "CMPP_W_TRUE_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_ON", (Opcode) CMPP_W_TRUE_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_ON, "CMPP_W_TRUE_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_OC", (Opcode) CMPP_W_TRUE_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_OC, "CMPP_W_TRUE_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_AN", (Opcode) CMPP_W_TRUE_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_AN, "CMPP_W_TRUE_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_OC_AC", (Opcode) CMPP_W_TRUE_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_OC_AC, "CMPP_W_TRUE_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_UN", (Opcode) CMPP_W_TRUE_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_UN, "CMPP_W_TRUE_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_UC", (Opcode) CMPP_W_TRUE_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_UC, "CMPP_W_TRUE_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_CN", (Opcode) CMPP_W_TRUE_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_CN, "CMPP_W_TRUE_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_CC", (Opcode) CMPP_W_TRUE_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_CC, "CMPP_W_TRUE_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_ON", (Opcode) CMPP_W_TRUE_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_ON, "CMPP_W_TRUE_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_OC", (Opcode) CMPP_W_TRUE_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_OC, "CMPP_W_TRUE_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_AN", (Opcode) CMPP_W_TRUE_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_AN, "CMPP_W_TRUE_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AN_AC", (Opcode) CMPP_W_TRUE_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AN_AC, "CMPP_W_TRUE_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_UN", (Opcode) CMPP_W_TRUE_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_UN, "CMPP_W_TRUE_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_UC", (Opcode) CMPP_W_TRUE_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_UC, "CMPP_W_TRUE_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_CN", (Opcode) CMPP_W_TRUE_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_CN, "CMPP_W_TRUE_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_CC", (Opcode) CMPP_W_TRUE_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_CC, "CMPP_W_TRUE_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_ON", (Opcode) CMPP_W_TRUE_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_ON, "CMPP_W_TRUE_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_OC", (Opcode) CMPP_W_TRUE_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_OC, "CMPP_W_TRUE_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_AN", (Opcode) CMPP_W_TRUE_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_AN, "CMPP_W_TRUE_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_TRUE_AC_AC", (Opcode) CMPP_W_TRUE_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_TRUE_AC_AC, "CMPP_W_TRUE_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp2_2()
{
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_UN", (Opcode) CMPP_W_NEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_UN, "CMPP_W_NEQ_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_UC", (Opcode) CMPP_W_NEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_UC, "CMPP_W_NEQ_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_CN", (Opcode) CMPP_W_NEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_CN, "CMPP_W_NEQ_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_CC", (Opcode) CMPP_W_NEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_CC, "CMPP_W_NEQ_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_ON", (Opcode) CMPP_W_NEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_ON, "CMPP_W_NEQ_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_OC", (Opcode) CMPP_W_NEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_OC, "CMPP_W_NEQ_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_AN", (Opcode) CMPP_W_NEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_AN, "CMPP_W_NEQ_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UN_AC", (Opcode) CMPP_W_NEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UN_AC, "CMPP_W_NEQ_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_UN", (Opcode) CMPP_W_NEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_UN, "CMPP_W_NEQ_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_UC", (Opcode) CMPP_W_NEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_UC, "CMPP_W_NEQ_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_CN", (Opcode) CMPP_W_NEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_CN, "CMPP_W_NEQ_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_CC", (Opcode) CMPP_W_NEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_CC, "CMPP_W_NEQ_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_ON", (Opcode) CMPP_W_NEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_ON, "CMPP_W_NEQ_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_OC", (Opcode) CMPP_W_NEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_OC, "CMPP_W_NEQ_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_AN", (Opcode) CMPP_W_NEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_AN, "CMPP_W_NEQ_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_UC_AC", (Opcode) CMPP_W_NEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_UC_AC, "CMPP_W_NEQ_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_UN", (Opcode) CMPP_W_NEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_UN, "CMPP_W_NEQ_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_UC", (Opcode) CMPP_W_NEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_UC, "CMPP_W_NEQ_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_CN", (Opcode) CMPP_W_NEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_CN, "CMPP_W_NEQ_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_CC", (Opcode) CMPP_W_NEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_CC, "CMPP_W_NEQ_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_ON", (Opcode) CMPP_W_NEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_ON, "CMPP_W_NEQ_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_OC", (Opcode) CMPP_W_NEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_OC, "CMPP_W_NEQ_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_AN", (Opcode) CMPP_W_NEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_AN, "CMPP_W_NEQ_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CN_AC", (Opcode) CMPP_W_NEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CN_AC, "CMPP_W_NEQ_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_UN", (Opcode) CMPP_W_NEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_UN, "CMPP_W_NEQ_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_UC", (Opcode) CMPP_W_NEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_UC, "CMPP_W_NEQ_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_CN", (Opcode) CMPP_W_NEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_CN, "CMPP_W_NEQ_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_CC", (Opcode) CMPP_W_NEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_CC, "CMPP_W_NEQ_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_ON", (Opcode) CMPP_W_NEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_ON, "CMPP_W_NEQ_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_OC", (Opcode) CMPP_W_NEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_OC, "CMPP_W_NEQ_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_AN", (Opcode) CMPP_W_NEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_AN, "CMPP_W_NEQ_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_CC_AC", (Opcode) CMPP_W_NEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_CC_AC, "CMPP_W_NEQ_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_UN", (Opcode) CMPP_W_NEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_UN, "CMPP_W_NEQ_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_UC", (Opcode) CMPP_W_NEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_UC, "CMPP_W_NEQ_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_CN", (Opcode) CMPP_W_NEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_CN, "CMPP_W_NEQ_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_CC", (Opcode) CMPP_W_NEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_CC, "CMPP_W_NEQ_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_ON", (Opcode) CMPP_W_NEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_ON, "CMPP_W_NEQ_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_OC", (Opcode) CMPP_W_NEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_OC, "CMPP_W_NEQ_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_AN", (Opcode) CMPP_W_NEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_AN, "CMPP_W_NEQ_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_ON_AC", (Opcode) CMPP_W_NEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_ON_AC, "CMPP_W_NEQ_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_UN", (Opcode) CMPP_W_NEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_UN, "CMPP_W_NEQ_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_UC", (Opcode) CMPP_W_NEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_UC, "CMPP_W_NEQ_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_CN", (Opcode) CMPP_W_NEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_CN, "CMPP_W_NEQ_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_CC", (Opcode) CMPP_W_NEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_CC, "CMPP_W_NEQ_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_ON", (Opcode) CMPP_W_NEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_ON, "CMPP_W_NEQ_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_OC", (Opcode) CMPP_W_NEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_OC, "CMPP_W_NEQ_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_AN", (Opcode) CMPP_W_NEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_AN, "CMPP_W_NEQ_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_OC_AC", (Opcode) CMPP_W_NEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_OC_AC, "CMPP_W_NEQ_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_UN", (Opcode) CMPP_W_NEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_UN, "CMPP_W_NEQ_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_UC", (Opcode) CMPP_W_NEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_UC, "CMPP_W_NEQ_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_CN", (Opcode) CMPP_W_NEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_CN, "CMPP_W_NEQ_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_CC", (Opcode) CMPP_W_NEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_CC, "CMPP_W_NEQ_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_ON", (Opcode) CMPP_W_NEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_ON, "CMPP_W_NEQ_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_OC", (Opcode) CMPP_W_NEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_OC, "CMPP_W_NEQ_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_AN", (Opcode) CMPP_W_NEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_AN, "CMPP_W_NEQ_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AN_AC", (Opcode) CMPP_W_NEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AN_AC, "CMPP_W_NEQ_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_UN", (Opcode) CMPP_W_NEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_UN, "CMPP_W_NEQ_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_UC", (Opcode) CMPP_W_NEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_UC, "CMPP_W_NEQ_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_CN", (Opcode) CMPP_W_NEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_CN, "CMPP_W_NEQ_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_CC", (Opcode) CMPP_W_NEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_CC, "CMPP_W_NEQ_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_ON", (Opcode) CMPP_W_NEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_ON, "CMPP_W_NEQ_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_OC", (Opcode) CMPP_W_NEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_OC, "CMPP_W_NEQ_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_AN", (Opcode) CMPP_W_NEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_AN, "CMPP_W_NEQ_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_NEQ_AC_AC", (Opcode) CMPP_W_NEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_NEQ_AC_AC, "CMPP_W_NEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp2_3()
{
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_UN", (Opcode) CMPP_W_LLT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_UN, "CMPP_W_LLT_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_UC", (Opcode) CMPP_W_LLT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_UC, "CMPP_W_LLT_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_CN", (Opcode) CMPP_W_LLT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_CN, "CMPP_W_LLT_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_CC", (Opcode) CMPP_W_LLT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_CC, "CMPP_W_LLT_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_ON", (Opcode) CMPP_W_LLT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_ON, "CMPP_W_LLT_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_OC", (Opcode) CMPP_W_LLT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_OC, "CMPP_W_LLT_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_AN", (Opcode) CMPP_W_LLT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_AN, "CMPP_W_LLT_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UN_AC", (Opcode) CMPP_W_LLT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UN_AC, "CMPP_W_LLT_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_UN", (Opcode) CMPP_W_LLT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_UN, "CMPP_W_LLT_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_UC", (Opcode) CMPP_W_LLT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_UC, "CMPP_W_LLT_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_CN", (Opcode) CMPP_W_LLT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_CN, "CMPP_W_LLT_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_CC", (Opcode) CMPP_W_LLT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_CC, "CMPP_W_LLT_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_ON", (Opcode) CMPP_W_LLT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_ON, "CMPP_W_LLT_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_OC", (Opcode) CMPP_W_LLT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_OC, "CMPP_W_LLT_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_AN", (Opcode) CMPP_W_LLT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_AN, "CMPP_W_LLT_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_UC_AC", (Opcode) CMPP_W_LLT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_UC_AC, "CMPP_W_LLT_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_UN", (Opcode) CMPP_W_LLT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_UN, "CMPP_W_LLT_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_UC", (Opcode) CMPP_W_LLT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_UC, "CMPP_W_LLT_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_CN", (Opcode) CMPP_W_LLT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_CN, "CMPP_W_LLT_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_CC", (Opcode) CMPP_W_LLT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_CC, "CMPP_W_LLT_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_ON", (Opcode) CMPP_W_LLT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_ON, "CMPP_W_LLT_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_OC", (Opcode) CMPP_W_LLT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_OC, "CMPP_W_LLT_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_AN", (Opcode) CMPP_W_LLT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_AN, "CMPP_W_LLT_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CN_AC", (Opcode) CMPP_W_LLT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CN_AC, "CMPP_W_LLT_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_UN", (Opcode) CMPP_W_LLT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_UN, "CMPP_W_LLT_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_UC", (Opcode) CMPP_W_LLT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_UC, "CMPP_W_LLT_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_CN", (Opcode) CMPP_W_LLT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_CN, "CMPP_W_LLT_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_CC", (Opcode) CMPP_W_LLT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_CC, "CMPP_W_LLT_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_ON", (Opcode) CMPP_W_LLT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_ON, "CMPP_W_LLT_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_OC", (Opcode) CMPP_W_LLT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_OC, "CMPP_W_LLT_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_AN", (Opcode) CMPP_W_LLT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_AN, "CMPP_W_LLT_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_CC_AC", (Opcode) CMPP_W_LLT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_CC_AC, "CMPP_W_LLT_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_UN", (Opcode) CMPP_W_LLT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_UN, "CMPP_W_LLT_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_UC", (Opcode) CMPP_W_LLT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_UC, "CMPP_W_LLT_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_CN", (Opcode) CMPP_W_LLT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_CN, "CMPP_W_LLT_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_CC", (Opcode) CMPP_W_LLT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_CC, "CMPP_W_LLT_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_ON", (Opcode) CMPP_W_LLT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_ON, "CMPP_W_LLT_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_OC", (Opcode) CMPP_W_LLT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_OC, "CMPP_W_LLT_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_AN", (Opcode) CMPP_W_LLT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_AN, "CMPP_W_LLT_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_ON_AC", (Opcode) CMPP_W_LLT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_ON_AC, "CMPP_W_LLT_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_UN", (Opcode) CMPP_W_LLT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_UN, "CMPP_W_LLT_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_UC", (Opcode) CMPP_W_LLT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_UC, "CMPP_W_LLT_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_CN", (Opcode) CMPP_W_LLT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_CN, "CMPP_W_LLT_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_CC", (Opcode) CMPP_W_LLT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_CC, "CMPP_W_LLT_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_ON", (Opcode) CMPP_W_LLT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_ON, "CMPP_W_LLT_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_OC", (Opcode) CMPP_W_LLT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_OC, "CMPP_W_LLT_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_AN", (Opcode) CMPP_W_LLT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_AN, "CMPP_W_LLT_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_OC_AC", (Opcode) CMPP_W_LLT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_OC_AC, "CMPP_W_LLT_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_UN", (Opcode) CMPP_W_LLT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_UN, "CMPP_W_LLT_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_UC", (Opcode) CMPP_W_LLT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_UC, "CMPP_W_LLT_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_CN", (Opcode) CMPP_W_LLT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_CN, "CMPP_W_LLT_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_CC", (Opcode) CMPP_W_LLT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_CC, "CMPP_W_LLT_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_ON", (Opcode) CMPP_W_LLT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_ON, "CMPP_W_LLT_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_OC", (Opcode) CMPP_W_LLT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_OC, "CMPP_W_LLT_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_AN", (Opcode) CMPP_W_LLT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_AN, "CMPP_W_LLT_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AN_AC", (Opcode) CMPP_W_LLT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AN_AC, "CMPP_W_LLT_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_UN", (Opcode) CMPP_W_LLT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_UN, "CMPP_W_LLT_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_UC", (Opcode) CMPP_W_LLT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_UC, "CMPP_W_LLT_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_CN", (Opcode) CMPP_W_LLT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_CN, "CMPP_W_LLT_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_CC", (Opcode) CMPP_W_LLT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_CC, "CMPP_W_LLT_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_ON", (Opcode) CMPP_W_LLT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_ON, "CMPP_W_LLT_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_OC", (Opcode) CMPP_W_LLT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_OC, "CMPP_W_LLT_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_AN", (Opcode) CMPP_W_LLT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_AN, "CMPP_W_LLT_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLT_AC_AC", (Opcode) CMPP_W_LLT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLT_AC_AC, "CMPP_W_LLT_AC_AC") ;
}

static void el_init_elcor_opcode_maps_cmpp2_4()
{
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_UN", (Opcode) CMPP_W_LLEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_UN, "CMPP_W_LLEQ_UN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_UC", (Opcode) CMPP_W_LLEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_UC, "CMPP_W_LLEQ_UN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_CN", (Opcode) CMPP_W_LLEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_CN, "CMPP_W_LLEQ_UN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_CC", (Opcode) CMPP_W_LLEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_CC, "CMPP_W_LLEQ_UN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_ON", (Opcode) CMPP_W_LLEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_ON, "CMPP_W_LLEQ_UN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_OC", (Opcode) CMPP_W_LLEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_OC, "CMPP_W_LLEQ_UN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_AN", (Opcode) CMPP_W_LLEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_AN, "CMPP_W_LLEQ_UN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UN_AC", (Opcode) CMPP_W_LLEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UN_AC, "CMPP_W_LLEQ_UN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_UN", (Opcode) CMPP_W_LLEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_UN, "CMPP_W_LLEQ_UC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_UC", (Opcode) CMPP_W_LLEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_UC, "CMPP_W_LLEQ_UC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_CN", (Opcode) CMPP_W_LLEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_CN, "CMPP_W_LLEQ_UC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_CC", (Opcode) CMPP_W_LLEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_CC, "CMPP_W_LLEQ_UC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_ON", (Opcode) CMPP_W_LLEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_ON, "CMPP_W_LLEQ_UC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_OC", (Opcode) CMPP_W_LLEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_OC, "CMPP_W_LLEQ_UC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_AN", (Opcode) CMPP_W_LLEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_AN, "CMPP_W_LLEQ_UC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_UC_AC", (Opcode) CMPP_W_LLEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_UC_AC, "CMPP_W_LLEQ_UC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_UN", (Opcode) CMPP_W_LLEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_UN, "CMPP_W_LLEQ_CN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_UC", (Opcode) CMPP_W_LLEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_UC, "CMPP_W_LLEQ_CN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_CN", (Opcode) CMPP_W_LLEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_CN, "CMPP_W_LLEQ_CN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_CC", (Opcode) CMPP_W_LLEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_CC, "CMPP_W_LLEQ_CN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_ON", (Opcode) CMPP_W_LLEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_ON, "CMPP_W_LLEQ_CN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_OC", (Opcode) CMPP_W_LLEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_OC, "CMPP_W_LLEQ_CN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_AN", (Opcode) CMPP_W_LLEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_AN, "CMPP_W_LLEQ_CN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CN_AC", (Opcode) CMPP_W_LLEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CN_AC, "CMPP_W_LLEQ_CN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_UN", (Opcode) CMPP_W_LLEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_UN, "CMPP_W_LLEQ_CC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_UC", (Opcode) CMPP_W_LLEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_UC, "CMPP_W_LLEQ_CC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_CN", (Opcode) CMPP_W_LLEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_CN, "CMPP_W_LLEQ_CC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_CC", (Opcode) CMPP_W_LLEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_CC, "CMPP_W_LLEQ_CC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_ON", (Opcode) CMPP_W_LLEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_ON, "CMPP_W_LLEQ_CC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_OC", (Opcode) CMPP_W_LLEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_OC, "CMPP_W_LLEQ_CC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_AN", (Opcode) CMPP_W_LLEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_AN, "CMPP_W_LLEQ_CC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_CC_AC", (Opcode) CMPP_W_LLEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_CC_AC, "CMPP_W_LLEQ_CC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_UN", (Opcode) CMPP_W_LLEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_UN, "CMPP_W_LLEQ_ON_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_UC", (Opcode) CMPP_W_LLEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_UC, "CMPP_W_LLEQ_ON_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_CN", (Opcode) CMPP_W_LLEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_CN, "CMPP_W_LLEQ_ON_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_CC", (Opcode) CMPP_W_LLEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_CC, "CMPP_W_LLEQ_ON_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_ON", (Opcode) CMPP_W_LLEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_ON, "CMPP_W_LLEQ_ON_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_OC", (Opcode) CMPP_W_LLEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_OC, "CMPP_W_LLEQ_ON_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_AN", (Opcode) CMPP_W_LLEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_AN, "CMPP_W_LLEQ_ON_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_ON_AC", (Opcode) CMPP_W_LLEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_ON_AC, "CMPP_W_LLEQ_ON_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_UN", (Opcode) CMPP_W_LLEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_UN, "CMPP_W_LLEQ_OC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_UC", (Opcode) CMPP_W_LLEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_UC, "CMPP_W_LLEQ_OC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_CN", (Opcode) CMPP_W_LLEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_CN, "CMPP_W_LLEQ_OC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_CC", (Opcode) CMPP_W_LLEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_CC, "CMPP_W_LLEQ_OC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_ON", (Opcode) CMPP_W_LLEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_ON, "CMPP_W_LLEQ_OC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_OC", (Opcode) CMPP_W_LLEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_OC, "CMPP_W_LLEQ_OC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_AN", (Opcode) CMPP_W_LLEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_AN, "CMPP_W_LLEQ_OC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_OC_AC", (Opcode) CMPP_W_LLEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_OC_AC, "CMPP_W_LLEQ_OC_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_UN", (Opcode) CMPP_W_LLEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_UN, "CMPP_W_LLEQ_AN_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_UC", (Opcode) CMPP_W_LLEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_UC, "CMPP_W_LLEQ_AN_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_CN", (Opcode) CMPP_W_LLEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_CN, "CMPP_W_LLEQ_AN_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_CC", (Opcode) CMPP_W_LLEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_CC, "CMPP_W_LLEQ_AN_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_ON", (Opcode) CMPP_W_LLEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_ON, "CMPP_W_LLEQ_AN_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_OC", (Opcode) CMPP_W_LLEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_OC, "CMPP_W_LLEQ_AN_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_AN", (Opcode) CMPP_W_LLEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_AN, "CMPP_W_LLEQ_AN_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AN_AC", (Opcode) CMPP_W_LLEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AN_AC, "CMPP_W_LLEQ_AN_AC") ;

   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_UN", (Opcode) CMPP_W_LLEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_UN, "CMPP_W_LLEQ_AC_UN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_UC", (Opcode) CMPP_W_LLEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_UC, "CMPP_W_LLEQ_AC_UC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_CN", (Opcode) CMPP_W_LLEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_CN, "CMPP_W_LLEQ_AC_CN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_CC", (Opcode) CMPP_W_LLEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_CC, "CMPP_W_LLEQ_AC_CC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_ON", (Opcode) CMPP_W_LLEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_ON, "CMPP_W_LLEQ_AC_ON") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_OC", (Opcode) CMPP_W_LLEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_OC, "CMPP_W_LLEQ_AC_OC") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_AN", (Opcode) CMPP_W_LLEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_AN, "CMPP_W_LLEQ_AC_AN") ;
   el_string_to_opcode_map.bind("CMPP_W_LLEQ_AC_AC", (Opcode) CMPP_W_LLEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) CMPP_W_LLEQ_AC_AC, "CMPP_W_LLEQ_AC_AC") ;
}

// SLARSEN: Vector instructions
void el_init_elcor_opcode_maps_vector()
{
   el_string_to_opcode_map.bind("VADD_W", VADD_W) ;
   el_opcode_to_string_map.bind(VADD_W, "VADD_W") ;
   el_string_to_opcode_map.bind("VADDL_W", VADDL_W) ;
   el_opcode_to_string_map.bind(VADDL_W, "VADDL_W") ;
   el_string_to_opcode_map.bind("VSUB_W", VSUB_W) ;
   el_opcode_to_string_map.bind(VSUB_W, "VSUB_W") ;
   el_string_to_opcode_map.bind("VSUBL_W", VSUBL_W) ;
   el_opcode_to_string_map.bind(VSUBL_W, "VSUBL_W") ;
   el_string_to_opcode_map.bind("VMPY_W", VMPY_W) ;
   el_opcode_to_string_map.bind(VMPY_W, "VMPY_W") ;
   el_string_to_opcode_map.bind("VMPYL_W", VMPYL_W) ;
   el_opcode_to_string_map.bind(VMPYL_W, "VMPYL_W") ;
   el_string_to_opcode_map.bind("VDIV_W", VDIV_W) ;
   el_opcode_to_string_map.bind(VDIV_W, "VDIV_W") ;
   el_string_to_opcode_map.bind("VDIVL_W", VDIVL_W) ;
   el_opcode_to_string_map.bind(VDIVL_W, "VDIVL_W") ;
   el_string_to_opcode_map.bind("VREM_W", VREM_W) ;
   el_opcode_to_string_map.bind(VREM_W, "VREM_W") ;
   el_string_to_opcode_map.bind("VREML_W", VREML_W) ;
   el_opcode_to_string_map.bind(VREML_W, "VREML_W") ;
   el_string_to_opcode_map.bind("VMIN_W", VMIN_W) ;
   el_opcode_to_string_map.bind(VMIN_W, "VMIN_W") ;
   el_string_to_opcode_map.bind("VMAX_W", VMAX_W) ;
   el_opcode_to_string_map.bind(VMAX_W, "VMAX_W") ;
   el_string_to_opcode_map.bind("VOR_W", VOR_W) ;
   el_opcode_to_string_map.bind(VOR_W, "VOR_W") ;
   el_string_to_opcode_map.bind("VAND_W", VAND_W) ;
   el_opcode_to_string_map.bind(VAND_W, "VAND_W") ;

   el_string_to_opcode_map.bind("VFADD_S", VFADD_S) ;
   el_opcode_to_string_map.bind(VFADD_S, "VFADD_S") ;
   el_string_to_opcode_map.bind("VFSUB_S", VFSUB_S) ;
   el_opcode_to_string_map.bind(VFSUB_S, "VFSUB_S") ;
   el_string_to_opcode_map.bind("VFMPY_S", VFMPY_S) ;
   el_opcode_to_string_map.bind(VFMPY_S, "VFMPY_S") ;
   el_string_to_opcode_map.bind("VFDIV_S", VFDIV_S) ;
   el_opcode_to_string_map.bind(VFDIV_S, "VFDIV_S") ;
   el_string_to_opcode_map.bind("VFMAX_S", VFMAX_S) ;
   el_opcode_to_string_map.bind(VFMAX_S, "VFMAX_S") ;
   el_string_to_opcode_map.bind("VFMIN_S", VFMIN_S) ;
   el_opcode_to_string_map.bind(VFMIN_S, "VFMIN_S") ;
   el_string_to_opcode_map.bind("VFABS_S", VFABS_S) ;
   el_opcode_to_string_map.bind(VFABS_S, "VFABS_S") ;
   el_string_to_opcode_map.bind("VFSQRT_S", VFSQRT_S) ;
   el_opcode_to_string_map.bind(VFSQRT_S, "VFSQRT_S") ;

   el_string_to_opcode_map.bind("VFADD_D", VFADD_D) ;
   el_opcode_to_string_map.bind(VFADD_D, "VFADD_D") ;
   el_string_to_opcode_map.bind("VFSUB_D", VFSUB_D) ;
   el_opcode_to_string_map.bind(VFSUB_D, "VFSUB_D") ;
   el_string_to_opcode_map.bind("VFMPY_D", VFMPY_D) ;
   el_opcode_to_string_map.bind(VFMPY_D, "VFMPY_D") ;
   el_string_to_opcode_map.bind("VFDIV_D", VFDIV_D) ;
   el_opcode_to_string_map.bind(VFDIV_D, "VFDIV_D") ;
   el_string_to_opcode_map.bind("VFMAX_D", VFMAX_D) ;
   el_opcode_to_string_map.bind(VFMAX_D, "VFMAX_D") ;
   el_string_to_opcode_map.bind("VFMIN_D", VFMIN_D) ;
   el_opcode_to_string_map.bind(VFMIN_D, "VFMIN_D") ;
   el_string_to_opcode_map.bind("VFABS_D", VFABS_D) ;
   el_opcode_to_string_map.bind(VFABS_D, "VFABS_D") ;
   el_string_to_opcode_map.bind("VFSQRT_D", VFSQRT_D) ;
   el_opcode_to_string_map.bind(VFSQRT_D, "VFSQRT_D") ;

   el_string_to_opcode_map.bind("VCONVWS", VCONVWS) ;
   el_opcode_to_string_map.bind(VCONVWS, "VCONVWS") ;
   el_string_to_opcode_map.bind("VCONVWD", VCONVWD) ;
   el_opcode_to_string_map.bind(VCONVWD, "VCONVWD") ;
   el_string_to_opcode_map.bind("VCONVSW", VCONVSW) ;
   el_opcode_to_string_map.bind(VCONVSW, "VCONVSW") ;
   el_string_to_opcode_map.bind("VCONVDW", VCONVDW) ;
   el_opcode_to_string_map.bind(VCONVDW, "VCONVDW") ;
   el_string_to_opcode_map.bind("VCONVSD", VCONVSD) ;
   el_opcode_to_string_map.bind(VCONVSD, "VCONVSD") ;
   el_string_to_opcode_map.bind("VCONVDS", VCONVDS) ;
   el_opcode_to_string_map.bind(VCONVDS, "VCONVDS") ;
   el_string_to_opcode_map.bind("VEXTS_B", VEXTS_B) ;
   el_opcode_to_string_map.bind(VEXTS_B, "VEXTS_B") ;
   el_string_to_opcode_map.bind("VEXTS_H", VEXTS_H) ;
   el_opcode_to_string_map.bind(VEXTS_H, "VEXTS_H") ;

   el_string_to_opcode_map.bind("VSMOVE", VSMOVE);
   el_opcode_to_string_map.bind(VSMOVE, "VSMOVE");
   el_string_to_opcode_map.bind("SVMOVE", SVMOVE);
   el_opcode_to_string_map.bind(SVMOVE, "SVMOVE");
   el_string_to_opcode_map.bind("VSFMOVE_S", VSFMOVE_S);
   el_opcode_to_string_map.bind(VSFMOVE_S, "VSFMOVE_S");
   el_string_to_opcode_map.bind("SVFMOVE_S", SVFMOVE_S);
   el_opcode_to_string_map.bind(SVFMOVE_S, "SVFMOVE_S");
   el_string_to_opcode_map.bind("VSFMOVE_D", VSFMOVE_D);
   el_opcode_to_string_map.bind(VSFMOVE_D, "VSFMOVE_D");
   el_string_to_opcode_map.bind("SVFMOVE_D", SVFMOVE_D);
   el_opcode_to_string_map.bind(SVFMOVE_D, "SVFMOVE_D");

   el_string_to_opcode_map.bind("VMOVE", VMOVE) ;
   el_opcode_to_string_map.bind(VMOVE, "VMOVE") ;
   el_string_to_opcode_map.bind("VMOVEF_S", VMOVEF_S) ;
   el_opcode_to_string_map.bind(VMOVEF_S, "VMOVEF_S") ;
   el_string_to_opcode_map.bind("VMOVEF_D", VMOVEF_D) ;
   el_opcode_to_string_map.bind(VMOVEF_D, "VMOVEF_D") ;
   el_string_to_opcode_map.bind("VMOVEGG", VMOVEGG) ;
   el_opcode_to_string_map.bind(VMOVEGG, "VMOVEGG") ;
   el_string_to_opcode_map.bind("VMOVEFF_S", VMOVEFF_S) ;
   el_opcode_to_string_map.bind(VMOVEFF_S, "VMOVEFF_S") ;
   el_string_to_opcode_map.bind("VMOVEFF_D", VMOVEFF_D) ;
   el_opcode_to_string_map.bind(VMOVEFF_D, "VMOVEFF_D") ;
   el_string_to_opcode_map.bind("VMOVEMM", VMOVEMM) ;
   el_opcode_to_string_map.bind(VMOVEMM, "VMOVEMM") ;

   el_string_to_opcode_map.bind("VL_B_C1_C1", (Opcode) VL_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VL_B_C1_C1, "VL_B_C1_C1") ;
   el_string_to_opcode_map.bind("VL_H_C1_C1", (Opcode) VL_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VL_H_C1_C1, "VL_H_C1_C1") ;
   el_string_to_opcode_map.bind("VL_W_C1_C1", (Opcode) VL_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VL_W_C1_C1, "VL_W_C1_C1") ;
   el_string_to_opcode_map.bind("VFL_S_C1_C1", (Opcode) VFL_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFL_S_C1_C1, "VFL_S_C1_C1") ;
   el_string_to_opcode_map.bind("VFL_D_C1_C1", (Opcode) VFL_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFL_D_C1_C1, "VFL_D_C1_C1") ;

   el_string_to_opcode_map.bind("VS_B_C1", (Opcode) VS_B_C1) ;
   el_opcode_to_string_map.bind((Opcode) VS_B_C1, "VS_B_C1") ;
   el_string_to_opcode_map.bind("VS_H_C1", (Opcode) VS_H_C1) ;
   el_opcode_to_string_map.bind((Opcode) VS_H_C1, "VS_H_C1") ;
   el_string_to_opcode_map.bind("VS_W_C1", (Opcode) VS_W_C1) ;
   el_opcode_to_string_map.bind((Opcode) VS_W_C1, "VS_W_C1") ;
   el_string_to_opcode_map.bind("VFS_S_C1", (Opcode) VFS_S_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFS_S_C1, "VFS_S_C1") ;
   el_string_to_opcode_map.bind("VFS_D_C1", (Opcode) VFS_D_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFS_D_C1, "VFS_D_C1") ;

   el_string_to_opcode_map.bind("VLG_B_C1_C1", (Opcode) VLG_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLG_B_C1_C1, "VLG_B_C1_C1") ;
   el_string_to_opcode_map.bind("VLG_H_C1_C1", (Opcode) VLG_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLG_H_C1_C1, "VLG_H_C1_C1") ;
   el_string_to_opcode_map.bind("VLG_W_C1_C1", (Opcode) VLG_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLG_W_C1_C1, "VLG_W_C1_C1") ;
   el_string_to_opcode_map.bind("VFLG_S_C1_C1", (Opcode) VFLG_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFLG_S_C1_C1, "VFLG_S_C1_C1") ;
   el_string_to_opcode_map.bind("VFLG_D_C1_C1", (Opcode) VFLG_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFLG_D_C1_C1, "VFLG_D_C1_C1") ;
   
   el_string_to_opcode_map.bind("VSG_B_C1", (Opcode) VSG_B_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSG_B_C1, "VSG_B_C1") ;
   el_string_to_opcode_map.bind("VSG_H_C1", (Opcode) VSG_H_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSG_H_C1, "VSG_H_C1") ;
   el_string_to_opcode_map.bind("VSG_W_C1", (Opcode) VSG_W_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSG_W_C1, "VSG_W_C1") ;
   el_string_to_opcode_map.bind("VFSG_S_C1", (Opcode) VFSG_S_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFSG_S_C1, "VFSG_S_C1") ;
   el_string_to_opcode_map.bind("VFSG_D_C1", (Opcode) VFSG_D_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFSG_D_C1, "VFSG_D_C1") ;

   el_string_to_opcode_map.bind("VLE_B_C1_C1", (Opcode) VLE_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLE_B_C1_C1, "VLE_B_C1_C1") ;
   el_string_to_opcode_map.bind("VLE_H_C1_C1", (Opcode) VLE_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLE_H_C1_C1, "VLE_H_C1_C1") ;
   el_string_to_opcode_map.bind("VLE_W_C1_C1", (Opcode) VLE_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLE_W_C1_C1, "VLE_W_C1_C1") ;
   el_string_to_opcode_map.bind("VFLE_S_C1_C1", (Opcode) VFLE_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFLE_S_C1_C1, "VFLE_S_C1_C1") ;
   el_string_to_opcode_map.bind("VFLE_D_C1_C1", (Opcode) VFLE_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFLE_D_C1_C1, "VFLE_D_C1_C1") ;

   el_string_to_opcode_map.bind("VSE_B_C1", (Opcode) VSE_B_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSE_B_C1, "VSE_B_C1") ;
   el_string_to_opcode_map.bind("VSE_H_C1", (Opcode) VSE_H_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSE_H_C1, "VSE_H_C1") ;
   el_string_to_opcode_map.bind("VSE_W_C1", (Opcode) VSE_W_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSE_W_C1, "VSE_W_C1") ;
   el_string_to_opcode_map.bind("VFSE_S_C1", (Opcode) VFSE_S_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFSE_S_C1, "VFSE_S_C1") ;
   el_string_to_opcode_map.bind("VFSE_D_C1", (Opcode) VFSE_D_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFSE_D_C1, "VFSE_D_C1") ;

   el_string_to_opcode_map.bind("VLGE_B_C1_C1", (Opcode) VLGE_B_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLGE_B_C1_C1, "VLGE_B_C1_C1") ;
   el_string_to_opcode_map.bind("VLGE_H_C1_C1", (Opcode) VLGE_H_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLGE_H_C1_C1, "VLGE_H_C1_C1") ;
   el_string_to_opcode_map.bind("VLGE_W_C1_C1", (Opcode) VLGE_W_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VLGE_W_C1_C1, "VLGE_W_C1_C1") ;
   el_string_to_opcode_map.bind("VFLGE_S_C1_C1", (Opcode) VFLGE_S_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFLGE_S_C1_C1, "VFLGE_S_C1_C1") ;
   el_string_to_opcode_map.bind("VFLGE_D_C1_C1", (Opcode) VFLGE_D_C1_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFLGE_D_C1_C1, "VFLGE_D_C1_C1") ;
   
   el_string_to_opcode_map.bind("VSGE_B_C1", (Opcode) VSGE_B_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSGE_B_C1, "VSGE_B_C1") ;
   el_string_to_opcode_map.bind("VSGE_H_C1", (Opcode) VSGE_H_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSGE_H_C1, "VSGE_H_C1") ;
   el_string_to_opcode_map.bind("VSGE_W_C1", (Opcode) VSGE_W_C1) ;
   el_opcode_to_string_map.bind((Opcode) VSGE_W_C1, "VSGE_W_C1") ;
   el_string_to_opcode_map.bind("VFSGE_S_C1", (Opcode) VFSGE_S_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFSGE_S_C1, "VFSGE_S_C1") ;
   el_string_to_opcode_map.bind("VFSGE_D_C1", (Opcode) VFSGE_D_C1) ;
   el_opcode_to_string_map.bind((Opcode) VFSGE_D_C1, "VFSGE_D_C1") ;

   el_string_to_opcode_map.bind("VPERM", VPERM) ;
   el_opcode_to_string_map.bind(VPERM, "VPERM") ;
   el_string_to_opcode_map.bind("VFPERM_S", VFPERM_S) ;
   el_opcode_to_string_map.bind(VFPERM_S, "VFPERM_S") ;
   el_string_to_opcode_map.bind("VFPERM_D", VFPERM_D) ;
   el_opcode_to_string_map.bind(VFPERM_D, "VFPERM_D") ;
   
   el_string_to_opcode_map.bind("VSPLAT", VSPLAT) ;
   el_opcode_to_string_map.bind(VSPLAT, "VSPLAT") ;
   el_string_to_opcode_map.bind("VFSPLAT_S", VFSPLAT_S) ;
   el_opcode_to_string_map.bind(VFSPLAT_S, "VFSPLAT_S") ;
   el_string_to_opcode_map.bind("VFSPLAT_D", VFSPLAT_D) ;
   el_opcode_to_string_map.bind(VFSPLAT_D, "VFSPLAT_D") ;
   
   el_string_to_opcode_map.bind("VCMPP_W_EQ_UN_UN", (Opcode) VCMPP_W_EQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VCMPP_W_EQ_UN_UN, "VCMPP_W_EQ_UN_UN") ;
   el_string_to_opcode_map.bind("VCMPP_W_NEQ_UN_UN", (Opcode) VCMPP_W_NEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VCMPP_W_NEQ_UN_UN, "VCMPP_W_NEQ_UN_UN") ;
   el_string_to_opcode_map.bind("VCMPP_W_GT_UN_UN", (Opcode) VCMPP_W_GT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VCMPP_W_GT_UN_UN, "VCMPP_W_GT_UN_UN") ;
   el_string_to_opcode_map.bind("VCMPP_W_GEQ_UN_UN", (Opcode) VCMPP_W_GEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VCMPP_W_GEQ_UN_UN, "VCMPP_W_GEQ_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_S_EQ_UN_UN", (Opcode) VFCMPP_S_EQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_S_EQ_UN_UN, "VFCMPP_S_EQ_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_S_NEQ_UN_UN", (Opcode) VFCMPP_S_NEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_S_NEQ_UN_UN, "VFCMPP_S_NEQ_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_S_GT_UN_UN", (Opcode) VFCMPP_S_GT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_S_GT_UN_UN, "VFCMPP_S_GT_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_S_GEQ_UN_UN", (Opcode) VFCMPP_S_GEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_S_GEQ_UN_UN, "VFCMPP_S_GEQ_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_D_EQ_UN_UN", (Opcode) VFCMPP_D_EQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_D_EQ_UN_UN, "VFCMPP_D_EQ_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_D_NEQ_UN_UN", (Opcode) VFCMPP_D_NEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_D_NEQ_UN_UN, "VFCMPP_D_NEQ_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_D_GT_UN_UN", (Opcode) VFCMPP_D_GT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_D_GT_UN_UN, "VFCMPP_D_GT_UN_UN") ;
   el_string_to_opcode_map.bind("VFCMPP_D_GEQ_UN_UN", (Opcode) VFCMPP_D_GEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) VFCMPP_D_GEQ_UN_UN, "VFCMPP_D_GEQ_UN_UN") ;

   // map scalar opcodes to vector opcodes
   el_opcode_to_vector_opcode_map.bind(ADD_W, VADD_W) ;
   el_opcode_to_vector_opcode_map.bind(ADDL_W, VADDL_W) ;
   el_opcode_to_vector_opcode_map.bind(SUB_W, VSUB_W) ;
   el_opcode_to_vector_opcode_map.bind(SUBL_W, VSUBL_W) ;
   el_opcode_to_vector_opcode_map.bind(MPY_W, VMPY_W) ;
   el_opcode_to_vector_opcode_map.bind(MPYL_W, VMPYL_W) ;
   el_opcode_to_vector_opcode_map.bind(DIV_W, VDIV_W) ;
   el_opcode_to_vector_opcode_map.bind(DIVL_W, VDIVL_W) ;
   el_opcode_to_vector_opcode_map.bind(REM_W, VREM_W) ;
   el_opcode_to_vector_opcode_map.bind(REML_W, VREML_W) ;
   el_opcode_to_vector_opcode_map.bind(MIN_W, VMIN_W) ;
   el_opcode_to_vector_opcode_map.bind(MAX_W, VMAX_W) ;
   el_opcode_to_vector_opcode_map.bind(OR_W, VOR_W) ;
   el_opcode_to_vector_opcode_map.bind(AND_W, VAND_W) ;

   el_opcode_to_vector_opcode_map.bind(FADD_S, VFADD_S) ;
   el_opcode_to_vector_opcode_map.bind(FSUB_S, VFSUB_S) ;
   el_opcode_to_vector_opcode_map.bind(FMPY_S, VFMPY_S) ;
   el_opcode_to_vector_opcode_map.bind(FDIV_S, VFDIV_S) ;
   el_opcode_to_vector_opcode_map.bind(FMAX_S, VFMAX_S) ;
   el_opcode_to_vector_opcode_map.bind(FMIN_S, VFMIN_S) ;
   el_opcode_to_vector_opcode_map.bind(FABS_S, VFABS_S) ;
   el_opcode_to_vector_opcode_map.bind(FSQRT_S, VFSQRT_S) ;
   
   el_opcode_to_vector_opcode_map.bind(FADD_D, VFADD_D) ;
   el_opcode_to_vector_opcode_map.bind(FSUB_D, VFSUB_D) ;
   el_opcode_to_vector_opcode_map.bind(FMPY_D, VFMPY_D) ;
   el_opcode_to_vector_opcode_map.bind(FDIV_D, VFDIV_D) ;
   el_opcode_to_vector_opcode_map.bind(FMAX_D, VFMAX_D) ;
   el_opcode_to_vector_opcode_map.bind(FMIN_D, VFMIN_D) ;
   el_opcode_to_vector_opcode_map.bind(FABS_D, VFABS_D) ;
   el_opcode_to_vector_opcode_map.bind(FSQRT_D, VFSQRT_D) ;
   
   el_opcode_to_vector_opcode_map.bind(CONVWS, VCONVWS) ;
   el_opcode_to_vector_opcode_map.bind(CONVWD, VCONVWD) ;
   el_opcode_to_vector_opcode_map.bind(CONVSW, VCONVSW) ;
   el_opcode_to_vector_opcode_map.bind(CONVDW, VCONVDW) ;
   el_opcode_to_vector_opcode_map.bind(CONVSD, VCONVSD) ;
   el_opcode_to_vector_opcode_map.bind(CONVDS, VCONVDS) ;
   el_opcode_to_vector_opcode_map.bind(EXTS_B, VEXTS_B) ;
   el_opcode_to_vector_opcode_map.bind(EXTS_H, VEXTS_H) ;
   
   el_opcode_to_vector_opcode_map.bind(MOVE, VMOVE) ;
   el_opcode_to_vector_opcode_map.bind(MOVEF_S, VMOVEF_S) ;
   el_opcode_to_vector_opcode_map.bind(MOVEF_D, VMOVEF_D) ;

   el_opcode_to_vector_opcode_map.bind(L_B_C1_C1, VL_B_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(L_H_C1_C1, VL_H_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(L_W_C1_C1, VL_W_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(FL_S_C1_C1, VFL_S_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(FL_D_C1_C1, VFL_D_C1_C1) ;
   
   el_opcode_to_vector_opcode_map.bind(S_B_C1, VS_B_C1) ;
   el_opcode_to_vector_opcode_map.bind(S_H_C1, VS_H_C1) ;
   el_opcode_to_vector_opcode_map.bind(S_W_C1, VS_W_C1) ;
   el_opcode_to_vector_opcode_map.bind(FS_S_C1, VFS_S_C1) ;
   el_opcode_to_vector_opcode_map.bind(FS_D_C1, VFS_D_C1) ;

   el_opcode_to_vector_opcode_map.bind(LG_B_C1_C1, VLG_B_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(LG_H_C1_C1, VLG_H_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(LG_W_C1_C1, VLG_W_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(FLG_S_C1_C1, VFLG_S_C1_C1) ;
   el_opcode_to_vector_opcode_map.bind(FLG_D_C1_C1, VFLG_D_C1_C1) ;
   
   el_opcode_to_vector_opcode_map.bind(SG_B_C1, VSG_B_C1) ;
   el_opcode_to_vector_opcode_map.bind(SG_H_C1, VSG_H_C1) ;
   el_opcode_to_vector_opcode_map.bind(SG_W_C1, VSG_W_C1) ;
   el_opcode_to_vector_opcode_map.bind(FSG_S_C1, VFSG_S_C1) ;
   el_opcode_to_vector_opcode_map.bind(FSG_D_C1, VFSG_D_C1) ;

   el_opcode_to_vector_opcode_map.bind(CMPP_W_EQ_UN_UN, VCMPP_W_EQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(CMPP_W_NEQ_UN_UN, VCMPP_W_NEQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(CMPP_W_GT_UN_UN, VCMPP_W_GT_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(CMPP_W_GEQ_UN_UN, VCMPP_W_GEQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_S_EQ_UN_UN, VFCMPP_S_EQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_S_NEQ_UN_UN, VFCMPP_S_NEQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_S_GT_UN_UN, VFCMPP_S_GT_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_S_GEQ_UN_UN, VFCMPP_S_GEQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_D_EQ_UN_UN, VFCMPP_D_EQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_D_NEQ_UN_UN, VFCMPP_D_NEQ_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_D_GT_UN_UN, VFCMPP_D_GT_UN_UN) ;
   el_opcode_to_vector_opcode_map.bind(FCMPP_D_GEQ_UN_UN, VFCMPP_D_GEQ_UN_UN) ;
}

extern void    el_init_elcor_opcode_maps_cmpp2_5() ;
extern void    el_init_elcor_opcode_maps_cmpp2_6() ;
extern void    el_init_elcor_opcode_maps_cmpp2_7() ;
extern void    el_init_elcor_opcode_maps_cmpp2_8() ;

static void el_init_elcor_opcode_maps_cmpp2()
{
   el_init_elcor_opcode_maps_cmpp2_1() ;
   el_init_elcor_opcode_maps_cmpp2_2() ;
   el_init_elcor_opcode_maps_cmpp2_3() ;
   el_init_elcor_opcode_maps_cmpp2_4() ;
   el_init_elcor_opcode_maps_cmpp2_5() ;
   el_init_elcor_opcode_maps_cmpp2_6() ;
   el_init_elcor_opcode_maps_cmpp2_7() ;
   el_init_elcor_opcode_maps_cmpp2_8() ;
}

extern void el_init_elcor_opcode_maps_fcmpp1();
extern void el_init_elcor_opcode_maps_fcmpp2();
extern void el_init_elcor_opcode_maps_memory();
extern void el_init_elcor_opcode_maps_misc();
extern void el_init_elcor_opcode_maps_lm();
extern void el_init_elcor_base_opcode_maps();


// <fbouchez>
// streams

static void el_init_elcor_opcode_maps_streams()
{
  el_string_to_opcode_map.bind("S_PUSH", S_PUSH) ;
  el_opcode_to_string_map.bind(S_PUSH, "S_PUSH") ;
  el_string_to_opcode_map.bind("S_POP", S_POP) ;
  el_opcode_to_string_map.bind(S_POP, "S_POP") ;
  el_string_to_opcode_map.bind("S_PEEK", S_PEEK) ;
  el_opcode_to_string_map.bind(S_PEEK, "S_PEEK") ;
  el_string_to_opcode_map.bind("S_NEW", S_NEW) ;
  el_opcode_to_string_map.bind(S_NEW, "S_NEW") ;
  el_string_to_opcode_map.bind("S_SYNONYM", S_SYNONYM) ;
  el_opcode_to_string_map.bind(S_SYNONYM, "S_SYNONYM") ;
}

// </fbouchez>

void el_init_elcor_opcode_maps()
{
    el_init_elcor_opcode_maps_arithmetic();
    el_init_elcor_opcode_maps_cmpp1();
    el_init_elcor_opcode_maps_cmpp2();
    el_init_elcor_opcode_maps_fcmpp1();
    el_init_elcor_opcode_maps_fcmpp2();
    el_init_elcor_opcode_maps_memory();
    el_init_elcor_opcode_maps_misc();
    el_init_elcor_opcode_maps_lm();

    el_init_elcor_opcode_maps_streams();

    // SLARSEN: Add vector instructions to the opcode maps
    el_init_elcor_opcode_maps_vector();

    el_init_elcor_base_opcode_maps();
}

void el_deinit_elcor_opcode_maps()
{
   el_opcode_to_string_map.clear();
   el_string_to_opcode_map.clear();
}

//////////////////////////////////////////////////////////////////

unsigned hash_opcode(Opcode& opc)
{
   return(ELCOR_INT_HASH(opc)) ;
}

//////////////////////////////////////////////////////////////////

unsigned hash_IR_BASE_OPCODE(IR_BASE_OPCODE& opc)
{
   return(ELCOR_INT_HASH(opc)) ;
}

//////////////////////////////////////////////////////////////////

istream& operator>>(istream& is, Opcode& opc) 
{
    char name[80] ;

    is >> name ;
    opc = el_string_to_opcode_map.value(name) ;

    return (is) ;
}

ostream& operator<<(ostream& os, const Opcode opcode) 
{
    eString name;

    name = el_opcode_to_string_map.value(opcode);
    os << name;

    return (os);
}
  
ostream& operator<<(ostream& os, const IR_BASE_OPCODE opcode) 
{
    eString name;

    name = el_base_opcode_to_string_map.value(opcode);
    os << name;

    return (os);
}

Opcode opcode_or(Opcode& opc, Opcode& modifier)
{
  Opcode new_opc = (Opcode)(opc | modifier);
  return new_opc;
}
  
// end of opcode.cpp
