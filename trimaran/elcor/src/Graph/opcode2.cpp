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




///////////////////////////////////////////////////////////////////////////
//
//      File:           opcode2.cpp
//      Authors:        Sadun Anik, Scott Mahlke, Richard Johnson
//      Created:        December 1994
//      Description:    Enums for the opcodes 
//
///////////////////////////////////////////////////////////////////////////

#include "opcode.h"
#include "opcode_cmpp.h"
#include "opcode_load_store.h"
#include "hash_functions.h"
#include <string.h>

extern Hash_map<IR_BASE_OPCODE, eString> el_base_opcode_to_string_map ;

static void el_init_elcor_opcode_maps_fcmpp1_1()
{
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_UN", (Opcode) FCMPP_S_FALSE_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_UN, "FCMPP_S_FALSE_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_UC", (Opcode) FCMPP_S_FALSE_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_UC, "FCMPP_S_FALSE_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_CN", (Opcode) FCMPP_S_FALSE_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_CN, "FCMPP_S_FALSE_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_CC", (Opcode) FCMPP_S_FALSE_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_CC, "FCMPP_S_FALSE_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_ON", (Opcode) FCMPP_S_FALSE_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_ON, "FCMPP_S_FALSE_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_OC", (Opcode) FCMPP_S_FALSE_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_OC, "FCMPP_S_FALSE_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_AN", (Opcode) FCMPP_S_FALSE_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_AN, "FCMPP_S_FALSE_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UN_AC", (Opcode) FCMPP_S_FALSE_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UN_AC, "FCMPP_S_FALSE_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_UN", (Opcode) FCMPP_S_FALSE_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_UN, "FCMPP_S_FALSE_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_UC", (Opcode) FCMPP_S_FALSE_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_UC, "FCMPP_S_FALSE_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_CN", (Opcode) FCMPP_S_FALSE_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_CN, "FCMPP_S_FALSE_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_CC", (Opcode) FCMPP_S_FALSE_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_CC, "FCMPP_S_FALSE_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_ON", (Opcode) FCMPP_S_FALSE_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_ON, "FCMPP_S_FALSE_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_OC", (Opcode) FCMPP_S_FALSE_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_OC, "FCMPP_S_FALSE_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_AN", (Opcode) FCMPP_S_FALSE_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_AN, "FCMPP_S_FALSE_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_UC_AC", (Opcode) FCMPP_S_FALSE_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_UC_AC, "FCMPP_S_FALSE_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_UN", (Opcode) FCMPP_S_FALSE_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_UN, "FCMPP_S_FALSE_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_UC", (Opcode) FCMPP_S_FALSE_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_UC, "FCMPP_S_FALSE_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_CN", (Opcode) FCMPP_S_FALSE_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_CN, "FCMPP_S_FALSE_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_CC", (Opcode) FCMPP_S_FALSE_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_CC, "FCMPP_S_FALSE_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_ON", (Opcode) FCMPP_S_FALSE_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_ON, "FCMPP_S_FALSE_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_OC", (Opcode) FCMPP_S_FALSE_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_OC, "FCMPP_S_FALSE_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_AN", (Opcode) FCMPP_S_FALSE_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_AN, "FCMPP_S_FALSE_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CN_AC", (Opcode) FCMPP_S_FALSE_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CN_AC, "FCMPP_S_FALSE_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_UN", (Opcode) FCMPP_S_FALSE_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_UN, "FCMPP_S_FALSE_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_UC", (Opcode) FCMPP_S_FALSE_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_UC, "FCMPP_S_FALSE_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_CN", (Opcode) FCMPP_S_FALSE_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_CN, "FCMPP_S_FALSE_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_CC", (Opcode) FCMPP_S_FALSE_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_CC, "FCMPP_S_FALSE_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_ON", (Opcode) FCMPP_S_FALSE_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_ON, "FCMPP_S_FALSE_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_OC", (Opcode) FCMPP_S_FALSE_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_OC, "FCMPP_S_FALSE_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_AN", (Opcode) FCMPP_S_FALSE_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_AN, "FCMPP_S_FALSE_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_CC_AC", (Opcode) FCMPP_S_FALSE_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_CC_AC, "FCMPP_S_FALSE_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_UN", (Opcode) FCMPP_S_FALSE_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_UN, "FCMPP_S_FALSE_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_UC", (Opcode) FCMPP_S_FALSE_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_UC, "FCMPP_S_FALSE_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_CN", (Opcode) FCMPP_S_FALSE_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_CN, "FCMPP_S_FALSE_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_CC", (Opcode) FCMPP_S_FALSE_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_CC, "FCMPP_S_FALSE_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_ON", (Opcode) FCMPP_S_FALSE_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_ON, "FCMPP_S_FALSE_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_OC", (Opcode) FCMPP_S_FALSE_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_OC, "FCMPP_S_FALSE_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_AN", (Opcode) FCMPP_S_FALSE_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_AN, "FCMPP_S_FALSE_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_ON_AC", (Opcode) FCMPP_S_FALSE_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_ON_AC, "FCMPP_S_FALSE_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_UN", (Opcode) FCMPP_S_FALSE_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_UN, "FCMPP_S_FALSE_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_UC", (Opcode) FCMPP_S_FALSE_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_UC, "FCMPP_S_FALSE_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_CN", (Opcode) FCMPP_S_FALSE_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_CN, "FCMPP_S_FALSE_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_CC", (Opcode) FCMPP_S_FALSE_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_CC, "FCMPP_S_FALSE_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_ON", (Opcode) FCMPP_S_FALSE_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_ON, "FCMPP_S_FALSE_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_OC", (Opcode) FCMPP_S_FALSE_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_OC, "FCMPP_S_FALSE_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_AN", (Opcode) FCMPP_S_FALSE_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_AN, "FCMPP_S_FALSE_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_OC_AC", (Opcode) FCMPP_S_FALSE_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_OC_AC, "FCMPP_S_FALSE_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_UN", (Opcode) FCMPP_S_FALSE_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_UN, "FCMPP_S_FALSE_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_UC", (Opcode) FCMPP_S_FALSE_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_UC, "FCMPP_S_FALSE_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_CN", (Opcode) FCMPP_S_FALSE_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_CN, "FCMPP_S_FALSE_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_CC", (Opcode) FCMPP_S_FALSE_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_CC, "FCMPP_S_FALSE_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_ON", (Opcode) FCMPP_S_FALSE_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_ON, "FCMPP_S_FALSE_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_OC", (Opcode) FCMPP_S_FALSE_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_OC, "FCMPP_S_FALSE_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_AN", (Opcode) FCMPP_S_FALSE_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_AN, "FCMPP_S_FALSE_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AN_AC", (Opcode) FCMPP_S_FALSE_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AN_AC, "FCMPP_S_FALSE_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_UN", (Opcode) FCMPP_S_FALSE_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_UN, "FCMPP_S_FALSE_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_UC", (Opcode) FCMPP_S_FALSE_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_UC, "FCMPP_S_FALSE_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_CN", (Opcode) FCMPP_S_FALSE_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_CN, "FCMPP_S_FALSE_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_CC", (Opcode) FCMPP_S_FALSE_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_CC, "FCMPP_S_FALSE_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_ON", (Opcode) FCMPP_S_FALSE_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_ON, "FCMPP_S_FALSE_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_OC", (Opcode) FCMPP_S_FALSE_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_OC, "FCMPP_S_FALSE_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_AN", (Opcode) FCMPP_S_FALSE_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_AN, "FCMPP_S_FALSE_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_FALSE_AC_AC", (Opcode) FCMPP_S_FALSE_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_FALSE_AC_AC, "FCMPP_S_FALSE_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp1_2()
{
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_UN", (Opcode) FCMPP_S_EQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_UN, "FCMPP_S_EQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_UC", (Opcode) FCMPP_S_EQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_UC, "FCMPP_S_EQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_CN", (Opcode) FCMPP_S_EQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_CN, "FCMPP_S_EQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_CC", (Opcode) FCMPP_S_EQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_CC, "FCMPP_S_EQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_ON", (Opcode) FCMPP_S_EQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_ON, "FCMPP_S_EQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_OC", (Opcode) FCMPP_S_EQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_OC, "FCMPP_S_EQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_AN", (Opcode) FCMPP_S_EQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_AN, "FCMPP_S_EQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UN_AC", (Opcode) FCMPP_S_EQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UN_AC, "FCMPP_S_EQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_UN", (Opcode) FCMPP_S_EQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_UN, "FCMPP_S_EQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_UC", (Opcode) FCMPP_S_EQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_UC, "FCMPP_S_EQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_CN", (Opcode) FCMPP_S_EQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_CN, "FCMPP_S_EQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_CC", (Opcode) FCMPP_S_EQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_CC, "FCMPP_S_EQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_ON", (Opcode) FCMPP_S_EQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_ON, "FCMPP_S_EQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_OC", (Opcode) FCMPP_S_EQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_OC, "FCMPP_S_EQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_AN", (Opcode) FCMPP_S_EQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_AN, "FCMPP_S_EQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_UC_AC", (Opcode) FCMPP_S_EQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_UC_AC, "FCMPP_S_EQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_UN", (Opcode) FCMPP_S_EQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_UN, "FCMPP_S_EQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_UC", (Opcode) FCMPP_S_EQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_UC, "FCMPP_S_EQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_CN", (Opcode) FCMPP_S_EQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_CN, "FCMPP_S_EQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_CC", (Opcode) FCMPP_S_EQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_CC, "FCMPP_S_EQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_ON", (Opcode) FCMPP_S_EQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_ON, "FCMPP_S_EQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_OC", (Opcode) FCMPP_S_EQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_OC, "FCMPP_S_EQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_AN", (Opcode) FCMPP_S_EQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_AN, "FCMPP_S_EQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CN_AC", (Opcode) FCMPP_S_EQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CN_AC, "FCMPP_S_EQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_UN", (Opcode) FCMPP_S_EQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_UN, "FCMPP_S_EQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_UC", (Opcode) FCMPP_S_EQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_UC, "FCMPP_S_EQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_CN", (Opcode) FCMPP_S_EQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_CN, "FCMPP_S_EQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_CC", (Opcode) FCMPP_S_EQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_CC, "FCMPP_S_EQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_ON", (Opcode) FCMPP_S_EQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_ON, "FCMPP_S_EQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_OC", (Opcode) FCMPP_S_EQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_OC, "FCMPP_S_EQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_AN", (Opcode) FCMPP_S_EQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_AN, "FCMPP_S_EQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_CC_AC", (Opcode) FCMPP_S_EQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_CC_AC, "FCMPP_S_EQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_UN", (Opcode) FCMPP_S_EQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_UN, "FCMPP_S_EQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_UC", (Opcode) FCMPP_S_EQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_UC, "FCMPP_S_EQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_CN", (Opcode) FCMPP_S_EQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_CN, "FCMPP_S_EQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_CC", (Opcode) FCMPP_S_EQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_CC, "FCMPP_S_EQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_ON", (Opcode) FCMPP_S_EQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_ON, "FCMPP_S_EQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_OC", (Opcode) FCMPP_S_EQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_OC, "FCMPP_S_EQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_AN", (Opcode) FCMPP_S_EQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_AN, "FCMPP_S_EQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_ON_AC", (Opcode) FCMPP_S_EQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_ON_AC, "FCMPP_S_EQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_UN", (Opcode) FCMPP_S_EQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_UN, "FCMPP_S_EQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_UC", (Opcode) FCMPP_S_EQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_UC, "FCMPP_S_EQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_CN", (Opcode) FCMPP_S_EQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_CN, "FCMPP_S_EQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_CC", (Opcode) FCMPP_S_EQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_CC, "FCMPP_S_EQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_ON", (Opcode) FCMPP_S_EQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_ON, "FCMPP_S_EQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_OC", (Opcode) FCMPP_S_EQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_OC, "FCMPP_S_EQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_AN", (Opcode) FCMPP_S_EQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_AN, "FCMPP_S_EQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_OC_AC", (Opcode) FCMPP_S_EQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_OC_AC, "FCMPP_S_EQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_UN", (Opcode) FCMPP_S_EQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_UN, "FCMPP_S_EQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_UC", (Opcode) FCMPP_S_EQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_UC, "FCMPP_S_EQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_CN", (Opcode) FCMPP_S_EQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_CN, "FCMPP_S_EQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_CC", (Opcode) FCMPP_S_EQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_CC, "FCMPP_S_EQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_ON", (Opcode) FCMPP_S_EQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_ON, "FCMPP_S_EQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_OC", (Opcode) FCMPP_S_EQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_OC, "FCMPP_S_EQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_AN", (Opcode) FCMPP_S_EQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_AN, "FCMPP_S_EQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AN_AC", (Opcode) FCMPP_S_EQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AN_AC, "FCMPP_S_EQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_UN", (Opcode) FCMPP_S_EQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_UN, "FCMPP_S_EQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_UC", (Opcode) FCMPP_S_EQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_UC, "FCMPP_S_EQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_CN", (Opcode) FCMPP_S_EQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_CN, "FCMPP_S_EQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_CC", (Opcode) FCMPP_S_EQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_CC, "FCMPP_S_EQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_ON", (Opcode) FCMPP_S_EQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_ON, "FCMPP_S_EQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_OC", (Opcode) FCMPP_S_EQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_OC, "FCMPP_S_EQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_AN", (Opcode) FCMPP_S_EQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_AN, "FCMPP_S_EQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_EQ_AC_AC", (Opcode) FCMPP_S_EQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_EQ_AC_AC, "FCMPP_S_EQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp1_3()
{
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_UN", (Opcode) FCMPP_S_LT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_UN, "FCMPP_S_LT_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_UC", (Opcode) FCMPP_S_LT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_UC, "FCMPP_S_LT_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_CN", (Opcode) FCMPP_S_LT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_CN, "FCMPP_S_LT_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_CC", (Opcode) FCMPP_S_LT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_CC, "FCMPP_S_LT_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_ON", (Opcode) FCMPP_S_LT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_ON, "FCMPP_S_LT_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_OC", (Opcode) FCMPP_S_LT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_OC, "FCMPP_S_LT_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_AN", (Opcode) FCMPP_S_LT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_AN, "FCMPP_S_LT_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UN_AC", (Opcode) FCMPP_S_LT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UN_AC, "FCMPP_S_LT_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_UN", (Opcode) FCMPP_S_LT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_UN, "FCMPP_S_LT_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_UC", (Opcode) FCMPP_S_LT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_UC, "FCMPP_S_LT_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_CN", (Opcode) FCMPP_S_LT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_CN, "FCMPP_S_LT_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_CC", (Opcode) FCMPP_S_LT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_CC, "FCMPP_S_LT_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_ON", (Opcode) FCMPP_S_LT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_ON, "FCMPP_S_LT_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_OC", (Opcode) FCMPP_S_LT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_OC, "FCMPP_S_LT_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_AN", (Opcode) FCMPP_S_LT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_AN, "FCMPP_S_LT_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_UC_AC", (Opcode) FCMPP_S_LT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_UC_AC, "FCMPP_S_LT_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_UN", (Opcode) FCMPP_S_LT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_UN, "FCMPP_S_LT_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_UC", (Opcode) FCMPP_S_LT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_UC, "FCMPP_S_LT_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_CN", (Opcode) FCMPP_S_LT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_CN, "FCMPP_S_LT_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_CC", (Opcode) FCMPP_S_LT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_CC, "FCMPP_S_LT_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_ON", (Opcode) FCMPP_S_LT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_ON, "FCMPP_S_LT_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_OC", (Opcode) FCMPP_S_LT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_OC, "FCMPP_S_LT_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_AN", (Opcode) FCMPP_S_LT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_AN, "FCMPP_S_LT_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CN_AC", (Opcode) FCMPP_S_LT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CN_AC, "FCMPP_S_LT_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_UN", (Opcode) FCMPP_S_LT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_UN, "FCMPP_S_LT_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_UC", (Opcode) FCMPP_S_LT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_UC, "FCMPP_S_LT_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_CN", (Opcode) FCMPP_S_LT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_CN, "FCMPP_S_LT_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_CC", (Opcode) FCMPP_S_LT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_CC, "FCMPP_S_LT_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_ON", (Opcode) FCMPP_S_LT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_ON, "FCMPP_S_LT_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_OC", (Opcode) FCMPP_S_LT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_OC, "FCMPP_S_LT_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_AN", (Opcode) FCMPP_S_LT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_AN, "FCMPP_S_LT_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_CC_AC", (Opcode) FCMPP_S_LT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_CC_AC, "FCMPP_S_LT_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_UN", (Opcode) FCMPP_S_LT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_UN, "FCMPP_S_LT_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_UC", (Opcode) FCMPP_S_LT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_UC, "FCMPP_S_LT_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_CN", (Opcode) FCMPP_S_LT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_CN, "FCMPP_S_LT_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_CC", (Opcode) FCMPP_S_LT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_CC, "FCMPP_S_LT_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_ON", (Opcode) FCMPP_S_LT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_ON, "FCMPP_S_LT_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_OC", (Opcode) FCMPP_S_LT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_OC, "FCMPP_S_LT_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_AN", (Opcode) FCMPP_S_LT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_AN, "FCMPP_S_LT_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_ON_AC", (Opcode) FCMPP_S_LT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_ON_AC, "FCMPP_S_LT_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_UN", (Opcode) FCMPP_S_LT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_UN, "FCMPP_S_LT_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_UC", (Opcode) FCMPP_S_LT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_UC, "FCMPP_S_LT_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_CN", (Opcode) FCMPP_S_LT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_CN, "FCMPP_S_LT_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_CC", (Opcode) FCMPP_S_LT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_CC, "FCMPP_S_LT_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_ON", (Opcode) FCMPP_S_LT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_ON, "FCMPP_S_LT_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_OC", (Opcode) FCMPP_S_LT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_OC, "FCMPP_S_LT_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_AN", (Opcode) FCMPP_S_LT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_AN, "FCMPP_S_LT_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_OC_AC", (Opcode) FCMPP_S_LT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_OC_AC, "FCMPP_S_LT_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_UN", (Opcode) FCMPP_S_LT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_UN, "FCMPP_S_LT_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_UC", (Opcode) FCMPP_S_LT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_UC, "FCMPP_S_LT_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_CN", (Opcode) FCMPP_S_LT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_CN, "FCMPP_S_LT_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_CC", (Opcode) FCMPP_S_LT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_CC, "FCMPP_S_LT_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_ON", (Opcode) FCMPP_S_LT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_ON, "FCMPP_S_LT_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_OC", (Opcode) FCMPP_S_LT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_OC, "FCMPP_S_LT_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_AN", (Opcode) FCMPP_S_LT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_AN, "FCMPP_S_LT_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AN_AC", (Opcode) FCMPP_S_LT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AN_AC, "FCMPP_S_LT_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_UN", (Opcode) FCMPP_S_LT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_UN, "FCMPP_S_LT_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_UC", (Opcode) FCMPP_S_LT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_UC, "FCMPP_S_LT_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_CN", (Opcode) FCMPP_S_LT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_CN, "FCMPP_S_LT_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_CC", (Opcode) FCMPP_S_LT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_CC, "FCMPP_S_LT_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_ON", (Opcode) FCMPP_S_LT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_ON, "FCMPP_S_LT_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_OC", (Opcode) FCMPP_S_LT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_OC, "FCMPP_S_LT_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_AN", (Opcode) FCMPP_S_LT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_AN, "FCMPP_S_LT_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LT_AC_AC", (Opcode) FCMPP_S_LT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LT_AC_AC, "FCMPP_S_LT_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp1_4()
{
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_UN", (Opcode) FCMPP_S_LEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_UN, "FCMPP_S_LEQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_UC", (Opcode) FCMPP_S_LEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_UC, "FCMPP_S_LEQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_CN", (Opcode) FCMPP_S_LEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_CN, "FCMPP_S_LEQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_CC", (Opcode) FCMPP_S_LEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_CC, "FCMPP_S_LEQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_ON", (Opcode) FCMPP_S_LEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_ON, "FCMPP_S_LEQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_OC", (Opcode) FCMPP_S_LEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_OC, "FCMPP_S_LEQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_AN", (Opcode) FCMPP_S_LEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_AN, "FCMPP_S_LEQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UN_AC", (Opcode) FCMPP_S_LEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UN_AC, "FCMPP_S_LEQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_UN", (Opcode) FCMPP_S_LEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_UN, "FCMPP_S_LEQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_UC", (Opcode) FCMPP_S_LEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_UC, "FCMPP_S_LEQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_CN", (Opcode) FCMPP_S_LEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_CN, "FCMPP_S_LEQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_CC", (Opcode) FCMPP_S_LEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_CC, "FCMPP_S_LEQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_ON", (Opcode) FCMPP_S_LEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_ON, "FCMPP_S_LEQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_OC", (Opcode) FCMPP_S_LEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_OC, "FCMPP_S_LEQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_AN", (Opcode) FCMPP_S_LEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_AN, "FCMPP_S_LEQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_UC_AC", (Opcode) FCMPP_S_LEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_UC_AC, "FCMPP_S_LEQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_UN", (Opcode) FCMPP_S_LEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_UN, "FCMPP_S_LEQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_UC", (Opcode) FCMPP_S_LEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_UC, "FCMPP_S_LEQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_CN", (Opcode) FCMPP_S_LEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_CN, "FCMPP_S_LEQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_CC", (Opcode) FCMPP_S_LEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_CC, "FCMPP_S_LEQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_ON", (Opcode) FCMPP_S_LEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_ON, "FCMPP_S_LEQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_OC", (Opcode) FCMPP_S_LEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_OC, "FCMPP_S_LEQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_AN", (Opcode) FCMPP_S_LEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_AN, "FCMPP_S_LEQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CN_AC", (Opcode) FCMPP_S_LEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CN_AC, "FCMPP_S_LEQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_UN", (Opcode) FCMPP_S_LEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_UN, "FCMPP_S_LEQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_UC", (Opcode) FCMPP_S_LEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_UC, "FCMPP_S_LEQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_CN", (Opcode) FCMPP_S_LEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_CN, "FCMPP_S_LEQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_CC", (Opcode) FCMPP_S_LEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_CC, "FCMPP_S_LEQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_ON", (Opcode) FCMPP_S_LEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_ON, "FCMPP_S_LEQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_OC", (Opcode) FCMPP_S_LEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_OC, "FCMPP_S_LEQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_AN", (Opcode) FCMPP_S_LEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_AN, "FCMPP_S_LEQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_CC_AC", (Opcode) FCMPP_S_LEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_CC_AC, "FCMPP_S_LEQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_UN", (Opcode) FCMPP_S_LEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_UN, "FCMPP_S_LEQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_UC", (Opcode) FCMPP_S_LEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_UC, "FCMPP_S_LEQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_CN", (Opcode) FCMPP_S_LEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_CN, "FCMPP_S_LEQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_CC", (Opcode) FCMPP_S_LEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_CC, "FCMPP_S_LEQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_ON", (Opcode) FCMPP_S_LEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_ON, "FCMPP_S_LEQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_OC", (Opcode) FCMPP_S_LEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_OC, "FCMPP_S_LEQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_AN", (Opcode) FCMPP_S_LEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_AN, "FCMPP_S_LEQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_ON_AC", (Opcode) FCMPP_S_LEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_ON_AC, "FCMPP_S_LEQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_UN", (Opcode) FCMPP_S_LEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_UN, "FCMPP_S_LEQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_UC", (Opcode) FCMPP_S_LEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_UC, "FCMPP_S_LEQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_CN", (Opcode) FCMPP_S_LEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_CN, "FCMPP_S_LEQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_CC", (Opcode) FCMPP_S_LEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_CC, "FCMPP_S_LEQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_ON", (Opcode) FCMPP_S_LEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_ON, "FCMPP_S_LEQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_OC", (Opcode) FCMPP_S_LEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_OC, "FCMPP_S_LEQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_AN", (Opcode) FCMPP_S_LEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_AN, "FCMPP_S_LEQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_OC_AC", (Opcode) FCMPP_S_LEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_OC_AC, "FCMPP_S_LEQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_UN", (Opcode) FCMPP_S_LEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_UN, "FCMPP_S_LEQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_UC", (Opcode) FCMPP_S_LEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_UC, "FCMPP_S_LEQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_CN", (Opcode) FCMPP_S_LEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_CN, "FCMPP_S_LEQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_CC", (Opcode) FCMPP_S_LEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_CC, "FCMPP_S_LEQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_ON", (Opcode) FCMPP_S_LEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_ON, "FCMPP_S_LEQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_OC", (Opcode) FCMPP_S_LEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_OC, "FCMPP_S_LEQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_AN", (Opcode) FCMPP_S_LEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_AN, "FCMPP_S_LEQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AN_AC", (Opcode) FCMPP_S_LEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AN_AC, "FCMPP_S_LEQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_UN", (Opcode) FCMPP_S_LEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_UN, "FCMPP_S_LEQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_UC", (Opcode) FCMPP_S_LEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_UC, "FCMPP_S_LEQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_CN", (Opcode) FCMPP_S_LEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_CN, "FCMPP_S_LEQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_CC", (Opcode) FCMPP_S_LEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_CC, "FCMPP_S_LEQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_ON", (Opcode) FCMPP_S_LEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_ON, "FCMPP_S_LEQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_OC", (Opcode) FCMPP_S_LEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_OC, "FCMPP_S_LEQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_AN", (Opcode) FCMPP_S_LEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_AN, "FCMPP_S_LEQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_LEQ_AC_AC", (Opcode) FCMPP_S_LEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_LEQ_AC_AC, "FCMPP_S_LEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp1_5()
{
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_UN", (Opcode) FCMPP_S_GT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_UN, "FCMPP_S_GT_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_UC", (Opcode) FCMPP_S_GT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_UC, "FCMPP_S_GT_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_CN", (Opcode) FCMPP_S_GT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_CN, "FCMPP_S_GT_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_CC", (Opcode) FCMPP_S_GT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_CC, "FCMPP_S_GT_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_ON", (Opcode) FCMPP_S_GT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_ON, "FCMPP_S_GT_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_OC", (Opcode) FCMPP_S_GT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_OC, "FCMPP_S_GT_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_AN", (Opcode) FCMPP_S_GT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_AN, "FCMPP_S_GT_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UN_AC", (Opcode) FCMPP_S_GT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UN_AC, "FCMPP_S_GT_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_UN", (Opcode) FCMPP_S_GT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_UN, "FCMPP_S_GT_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_UC", (Opcode) FCMPP_S_GT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_UC, "FCMPP_S_GT_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_CN", (Opcode) FCMPP_S_GT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_CN, "FCMPP_S_GT_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_CC", (Opcode) FCMPP_S_GT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_CC, "FCMPP_S_GT_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_ON", (Opcode) FCMPP_S_GT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_ON, "FCMPP_S_GT_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_OC", (Opcode) FCMPP_S_GT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_OC, "FCMPP_S_GT_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_AN", (Opcode) FCMPP_S_GT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_AN, "FCMPP_S_GT_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_UC_AC", (Opcode) FCMPP_S_GT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_UC_AC, "FCMPP_S_GT_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_UN", (Opcode) FCMPP_S_GT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_UN, "FCMPP_S_GT_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_UC", (Opcode) FCMPP_S_GT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_UC, "FCMPP_S_GT_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_CN", (Opcode) FCMPP_S_GT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_CN, "FCMPP_S_GT_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_CC", (Opcode) FCMPP_S_GT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_CC, "FCMPP_S_GT_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_ON", (Opcode) FCMPP_S_GT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_ON, "FCMPP_S_GT_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_OC", (Opcode) FCMPP_S_GT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_OC, "FCMPP_S_GT_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_AN", (Opcode) FCMPP_S_GT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_AN, "FCMPP_S_GT_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CN_AC", (Opcode) FCMPP_S_GT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CN_AC, "FCMPP_S_GT_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_UN", (Opcode) FCMPP_S_GT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_UN, "FCMPP_S_GT_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_UC", (Opcode) FCMPP_S_GT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_UC, "FCMPP_S_GT_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_CN", (Opcode) FCMPP_S_GT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_CN, "FCMPP_S_GT_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_CC", (Opcode) FCMPP_S_GT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_CC, "FCMPP_S_GT_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_ON", (Opcode) FCMPP_S_GT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_ON, "FCMPP_S_GT_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_OC", (Opcode) FCMPP_S_GT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_OC, "FCMPP_S_GT_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_AN", (Opcode) FCMPP_S_GT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_AN, "FCMPP_S_GT_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_CC_AC", (Opcode) FCMPP_S_GT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_CC_AC, "FCMPP_S_GT_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_UN", (Opcode) FCMPP_S_GT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_UN, "FCMPP_S_GT_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_UC", (Opcode) FCMPP_S_GT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_UC, "FCMPP_S_GT_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_CN", (Opcode) FCMPP_S_GT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_CN, "FCMPP_S_GT_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_CC", (Opcode) FCMPP_S_GT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_CC, "FCMPP_S_GT_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_ON", (Opcode) FCMPP_S_GT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_ON, "FCMPP_S_GT_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_OC", (Opcode) FCMPP_S_GT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_OC, "FCMPP_S_GT_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_AN", (Opcode) FCMPP_S_GT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_AN, "FCMPP_S_GT_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_ON_AC", (Opcode) FCMPP_S_GT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_ON_AC, "FCMPP_S_GT_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_UN", (Opcode) FCMPP_S_GT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_UN, "FCMPP_S_GT_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_UC", (Opcode) FCMPP_S_GT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_UC, "FCMPP_S_GT_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_CN", (Opcode) FCMPP_S_GT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_CN, "FCMPP_S_GT_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_CC", (Opcode) FCMPP_S_GT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_CC, "FCMPP_S_GT_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_ON", (Opcode) FCMPP_S_GT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_ON, "FCMPP_S_GT_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_OC", (Opcode) FCMPP_S_GT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_OC, "FCMPP_S_GT_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_AN", (Opcode) FCMPP_S_GT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_AN, "FCMPP_S_GT_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_OC_AC", (Opcode) FCMPP_S_GT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_OC_AC, "FCMPP_S_GT_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_UN", (Opcode) FCMPP_S_GT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_UN, "FCMPP_S_GT_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_UC", (Opcode) FCMPP_S_GT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_UC, "FCMPP_S_GT_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_CN", (Opcode) FCMPP_S_GT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_CN, "FCMPP_S_GT_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_CC", (Opcode) FCMPP_S_GT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_CC, "FCMPP_S_GT_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_ON", (Opcode) FCMPP_S_GT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_ON, "FCMPP_S_GT_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_OC", (Opcode) FCMPP_S_GT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_OC, "FCMPP_S_GT_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_AN", (Opcode) FCMPP_S_GT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_AN, "FCMPP_S_GT_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AN_AC", (Opcode) FCMPP_S_GT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AN_AC, "FCMPP_S_GT_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_UN", (Opcode) FCMPP_S_GT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_UN, "FCMPP_S_GT_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_UC", (Opcode) FCMPP_S_GT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_UC, "FCMPP_S_GT_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_CN", (Opcode) FCMPP_S_GT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_CN, "FCMPP_S_GT_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_CC", (Opcode) FCMPP_S_GT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_CC, "FCMPP_S_GT_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_ON", (Opcode) FCMPP_S_GT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_ON, "FCMPP_S_GT_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_OC", (Opcode) FCMPP_S_GT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_OC, "FCMPP_S_GT_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_AN", (Opcode) FCMPP_S_GT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_AN, "FCMPP_S_GT_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GT_AC_AC", (Opcode) FCMPP_S_GT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GT_AC_AC, "FCMPP_S_GT_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp1_6()
{
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_UN", (Opcode) FCMPP_S_GEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_UN, "FCMPP_S_GEQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_UC", (Opcode) FCMPP_S_GEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_UC, "FCMPP_S_GEQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_CN", (Opcode) FCMPP_S_GEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_CN, "FCMPP_S_GEQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_CC", (Opcode) FCMPP_S_GEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_CC, "FCMPP_S_GEQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_ON", (Opcode) FCMPP_S_GEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_ON, "FCMPP_S_GEQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_OC", (Opcode) FCMPP_S_GEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_OC, "FCMPP_S_GEQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_AN", (Opcode) FCMPP_S_GEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_AN, "FCMPP_S_GEQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UN_AC", (Opcode) FCMPP_S_GEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UN_AC, "FCMPP_S_GEQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_UN", (Opcode) FCMPP_S_GEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_UN, "FCMPP_S_GEQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_UC", (Opcode) FCMPP_S_GEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_UC, "FCMPP_S_GEQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_CN", (Opcode) FCMPP_S_GEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_CN, "FCMPP_S_GEQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_CC", (Opcode) FCMPP_S_GEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_CC, "FCMPP_S_GEQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_ON", (Opcode) FCMPP_S_GEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_ON, "FCMPP_S_GEQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_OC", (Opcode) FCMPP_S_GEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_OC, "FCMPP_S_GEQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_AN", (Opcode) FCMPP_S_GEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_AN, "FCMPP_S_GEQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_UC_AC", (Opcode) FCMPP_S_GEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_UC_AC, "FCMPP_S_GEQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_UN", (Opcode) FCMPP_S_GEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_UN, "FCMPP_S_GEQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_UC", (Opcode) FCMPP_S_GEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_UC, "FCMPP_S_GEQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_CN", (Opcode) FCMPP_S_GEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_CN, "FCMPP_S_GEQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_CC", (Opcode) FCMPP_S_GEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_CC, "FCMPP_S_GEQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_ON", (Opcode) FCMPP_S_GEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_ON, "FCMPP_S_GEQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_OC", (Opcode) FCMPP_S_GEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_OC, "FCMPP_S_GEQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_AN", (Opcode) FCMPP_S_GEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_AN, "FCMPP_S_GEQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CN_AC", (Opcode) FCMPP_S_GEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CN_AC, "FCMPP_S_GEQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_UN", (Opcode) FCMPP_S_GEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_UN, "FCMPP_S_GEQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_UC", (Opcode) FCMPP_S_GEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_UC, "FCMPP_S_GEQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_CN", (Opcode) FCMPP_S_GEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_CN, "FCMPP_S_GEQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_CC", (Opcode) FCMPP_S_GEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_CC, "FCMPP_S_GEQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_ON", (Opcode) FCMPP_S_GEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_ON, "FCMPP_S_GEQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_OC", (Opcode) FCMPP_S_GEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_OC, "FCMPP_S_GEQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_AN", (Opcode) FCMPP_S_GEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_AN, "FCMPP_S_GEQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_CC_AC", (Opcode) FCMPP_S_GEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_CC_AC, "FCMPP_S_GEQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_UN", (Opcode) FCMPP_S_GEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_UN, "FCMPP_S_GEQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_UC", (Opcode) FCMPP_S_GEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_UC, "FCMPP_S_GEQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_CN", (Opcode) FCMPP_S_GEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_CN, "FCMPP_S_GEQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_CC", (Opcode) FCMPP_S_GEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_CC, "FCMPP_S_GEQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_ON", (Opcode) FCMPP_S_GEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_ON, "FCMPP_S_GEQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_OC", (Opcode) FCMPP_S_GEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_OC, "FCMPP_S_GEQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_AN", (Opcode) FCMPP_S_GEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_AN, "FCMPP_S_GEQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_ON_AC", (Opcode) FCMPP_S_GEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_ON_AC, "FCMPP_S_GEQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_UN", (Opcode) FCMPP_S_GEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_UN, "FCMPP_S_GEQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_UC", (Opcode) FCMPP_S_GEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_UC, "FCMPP_S_GEQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_CN", (Opcode) FCMPP_S_GEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_CN, "FCMPP_S_GEQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_CC", (Opcode) FCMPP_S_GEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_CC, "FCMPP_S_GEQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_ON", (Opcode) FCMPP_S_GEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_ON, "FCMPP_S_GEQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_OC", (Opcode) FCMPP_S_GEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_OC, "FCMPP_S_GEQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_AN", (Opcode) FCMPP_S_GEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_AN, "FCMPP_S_GEQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_OC_AC", (Opcode) FCMPP_S_GEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_OC_AC, "FCMPP_S_GEQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_UN", (Opcode) FCMPP_S_GEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_UN, "FCMPP_S_GEQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_UC", (Opcode) FCMPP_S_GEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_UC, "FCMPP_S_GEQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_CN", (Opcode) FCMPP_S_GEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_CN, "FCMPP_S_GEQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_CC", (Opcode) FCMPP_S_GEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_CC, "FCMPP_S_GEQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_ON", (Opcode) FCMPP_S_GEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_ON, "FCMPP_S_GEQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_OC", (Opcode) FCMPP_S_GEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_OC, "FCMPP_S_GEQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_AN", (Opcode) FCMPP_S_GEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_AN, "FCMPP_S_GEQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AN_AC", (Opcode) FCMPP_S_GEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AN_AC, "FCMPP_S_GEQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_UN", (Opcode) FCMPP_S_GEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_UN, "FCMPP_S_GEQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_UC", (Opcode) FCMPP_S_GEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_UC, "FCMPP_S_GEQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_CN", (Opcode) FCMPP_S_GEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_CN, "FCMPP_S_GEQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_CC", (Opcode) FCMPP_S_GEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_CC, "FCMPP_S_GEQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_ON", (Opcode) FCMPP_S_GEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_ON, "FCMPP_S_GEQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_OC", (Opcode) FCMPP_S_GEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_OC, "FCMPP_S_GEQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_AN", (Opcode) FCMPP_S_GEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_AN, "FCMPP_S_GEQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_GEQ_AC_AC", (Opcode) FCMPP_S_GEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_GEQ_AC_AC, "FCMPP_S_GEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp1_7()
{
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_UN", (Opcode) FCMPP_S_NEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_UN, "FCMPP_S_NEQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_UC", (Opcode) FCMPP_S_NEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_UC, "FCMPP_S_NEQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_CN", (Opcode) FCMPP_S_NEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_CN, "FCMPP_S_NEQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_CC", (Opcode) FCMPP_S_NEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_CC, "FCMPP_S_NEQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_ON", (Opcode) FCMPP_S_NEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_ON, "FCMPP_S_NEQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_OC", (Opcode) FCMPP_S_NEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_OC, "FCMPP_S_NEQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_AN", (Opcode) FCMPP_S_NEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_AN, "FCMPP_S_NEQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UN_AC", (Opcode) FCMPP_S_NEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UN_AC, "FCMPP_S_NEQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_UN", (Opcode) FCMPP_S_NEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_UN, "FCMPP_S_NEQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_UC", (Opcode) FCMPP_S_NEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_UC, "FCMPP_S_NEQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_CN", (Opcode) FCMPP_S_NEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_CN, "FCMPP_S_NEQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_CC", (Opcode) FCMPP_S_NEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_CC, "FCMPP_S_NEQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_ON", (Opcode) FCMPP_S_NEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_ON, "FCMPP_S_NEQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_OC", (Opcode) FCMPP_S_NEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_OC, "FCMPP_S_NEQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_AN", (Opcode) FCMPP_S_NEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_AN, "FCMPP_S_NEQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_UC_AC", (Opcode) FCMPP_S_NEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_UC_AC, "FCMPP_S_NEQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_UN", (Opcode) FCMPP_S_NEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_UN, "FCMPP_S_NEQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_UC", (Opcode) FCMPP_S_NEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_UC, "FCMPP_S_NEQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_CN", (Opcode) FCMPP_S_NEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_CN, "FCMPP_S_NEQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_CC", (Opcode) FCMPP_S_NEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_CC, "FCMPP_S_NEQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_ON", (Opcode) FCMPP_S_NEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_ON, "FCMPP_S_NEQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_OC", (Opcode) FCMPP_S_NEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_OC, "FCMPP_S_NEQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_AN", (Opcode) FCMPP_S_NEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_AN, "FCMPP_S_NEQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CN_AC", (Opcode) FCMPP_S_NEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CN_AC, "FCMPP_S_NEQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_UN", (Opcode) FCMPP_S_NEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_UN, "FCMPP_S_NEQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_UC", (Opcode) FCMPP_S_NEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_UC, "FCMPP_S_NEQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_CN", (Opcode) FCMPP_S_NEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_CN, "FCMPP_S_NEQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_CC", (Opcode) FCMPP_S_NEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_CC, "FCMPP_S_NEQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_ON", (Opcode) FCMPP_S_NEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_ON, "FCMPP_S_NEQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_OC", (Opcode) FCMPP_S_NEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_OC, "FCMPP_S_NEQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_AN", (Opcode) FCMPP_S_NEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_AN, "FCMPP_S_NEQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_CC_AC", (Opcode) FCMPP_S_NEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_CC_AC, "FCMPP_S_NEQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_UN", (Opcode) FCMPP_S_NEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_UN, "FCMPP_S_NEQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_UC", (Opcode) FCMPP_S_NEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_UC, "FCMPP_S_NEQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_CN", (Opcode) FCMPP_S_NEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_CN, "FCMPP_S_NEQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_CC", (Opcode) FCMPP_S_NEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_CC, "FCMPP_S_NEQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_ON", (Opcode) FCMPP_S_NEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_ON, "FCMPP_S_NEQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_OC", (Opcode) FCMPP_S_NEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_OC, "FCMPP_S_NEQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_AN", (Opcode) FCMPP_S_NEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_AN, "FCMPP_S_NEQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_ON_AC", (Opcode) FCMPP_S_NEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_ON_AC, "FCMPP_S_NEQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_UN", (Opcode) FCMPP_S_NEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_UN, "FCMPP_S_NEQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_UC", (Opcode) FCMPP_S_NEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_UC, "FCMPP_S_NEQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_CN", (Opcode) FCMPP_S_NEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_CN, "FCMPP_S_NEQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_CC", (Opcode) FCMPP_S_NEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_CC, "FCMPP_S_NEQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_ON", (Opcode) FCMPP_S_NEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_ON, "FCMPP_S_NEQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_OC", (Opcode) FCMPP_S_NEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_OC, "FCMPP_S_NEQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_AN", (Opcode) FCMPP_S_NEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_AN, "FCMPP_S_NEQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_OC_AC", (Opcode) FCMPP_S_NEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_OC_AC, "FCMPP_S_NEQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_UN", (Opcode) FCMPP_S_NEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_UN, "FCMPP_S_NEQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_UC", (Opcode) FCMPP_S_NEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_UC, "FCMPP_S_NEQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_CN", (Opcode) FCMPP_S_NEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_CN, "FCMPP_S_NEQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_CC", (Opcode) FCMPP_S_NEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_CC, "FCMPP_S_NEQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_ON", (Opcode) FCMPP_S_NEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_ON, "FCMPP_S_NEQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_OC", (Opcode) FCMPP_S_NEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_OC, "FCMPP_S_NEQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_AN", (Opcode) FCMPP_S_NEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_AN, "FCMPP_S_NEQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AN_AC", (Opcode) FCMPP_S_NEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AN_AC, "FCMPP_S_NEQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_UN", (Opcode) FCMPP_S_NEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_UN, "FCMPP_S_NEQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_UC", (Opcode) FCMPP_S_NEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_UC, "FCMPP_S_NEQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_CN", (Opcode) FCMPP_S_NEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_CN, "FCMPP_S_NEQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_CC", (Opcode) FCMPP_S_NEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_CC, "FCMPP_S_NEQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_ON", (Opcode) FCMPP_S_NEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_ON, "FCMPP_S_NEQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_OC", (Opcode) FCMPP_S_NEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_OC, "FCMPP_S_NEQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_AN", (Opcode) FCMPP_S_NEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_AN, "FCMPP_S_NEQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_NEQ_AC_AC", (Opcode) FCMPP_S_NEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_NEQ_AC_AC, "FCMPP_S_NEQ_AC_AC") ;
}



void el_init_elcor_opcode_maps_fcmpp1()
{
   el_init_elcor_opcode_maps_fcmpp1_1() ;
   el_init_elcor_opcode_maps_fcmpp1_2() ;
   el_init_elcor_opcode_maps_fcmpp1_3() ;
   el_init_elcor_opcode_maps_fcmpp1_4() ;
   el_init_elcor_opcode_maps_fcmpp1_5() ;
   el_init_elcor_opcode_maps_fcmpp1_6() ;
   el_init_elcor_opcode_maps_fcmpp1_7() ;
}

static void el_init_elcor_opcode_maps_fcmpp2_1()
{
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_UN", (Opcode) FCMPP_S_TRUE_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_UN, "FCMPP_S_TRUE_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_UC", (Opcode) FCMPP_S_TRUE_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_UC, "FCMPP_S_TRUE_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_CN", (Opcode) FCMPP_S_TRUE_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_CN, "FCMPP_S_TRUE_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_CC", (Opcode) FCMPP_S_TRUE_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_CC, "FCMPP_S_TRUE_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_ON", (Opcode) FCMPP_S_TRUE_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_ON, "FCMPP_S_TRUE_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_OC", (Opcode) FCMPP_S_TRUE_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_OC, "FCMPP_S_TRUE_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_AN", (Opcode) FCMPP_S_TRUE_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_AN, "FCMPP_S_TRUE_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UN_AC", (Opcode) FCMPP_S_TRUE_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UN_AC, "FCMPP_S_TRUE_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_UN", (Opcode) FCMPP_S_TRUE_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_UN, "FCMPP_S_TRUE_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_UC", (Opcode) FCMPP_S_TRUE_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_UC, "FCMPP_S_TRUE_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_CN", (Opcode) FCMPP_S_TRUE_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_CN, "FCMPP_S_TRUE_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_CC", (Opcode) FCMPP_S_TRUE_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_CC, "FCMPP_S_TRUE_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_ON", (Opcode) FCMPP_S_TRUE_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_ON, "FCMPP_S_TRUE_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_OC", (Opcode) FCMPP_S_TRUE_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_OC, "FCMPP_S_TRUE_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_AN", (Opcode) FCMPP_S_TRUE_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_AN, "FCMPP_S_TRUE_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_UC_AC", (Opcode) FCMPP_S_TRUE_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_UC_AC, "FCMPP_S_TRUE_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_UN", (Opcode) FCMPP_S_TRUE_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_UN, "FCMPP_S_TRUE_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_UC", (Opcode) FCMPP_S_TRUE_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_UC, "FCMPP_S_TRUE_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_CN", (Opcode) FCMPP_S_TRUE_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_CN, "FCMPP_S_TRUE_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_CC", (Opcode) FCMPP_S_TRUE_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_CC, "FCMPP_S_TRUE_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_ON", (Opcode) FCMPP_S_TRUE_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_ON, "FCMPP_S_TRUE_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_OC", (Opcode) FCMPP_S_TRUE_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_OC, "FCMPP_S_TRUE_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_AN", (Opcode) FCMPP_S_TRUE_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_AN, "FCMPP_S_TRUE_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CN_AC", (Opcode) FCMPP_S_TRUE_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CN_AC, "FCMPP_S_TRUE_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_UN", (Opcode) FCMPP_S_TRUE_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_UN, "FCMPP_S_TRUE_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_UC", (Opcode) FCMPP_S_TRUE_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_UC, "FCMPP_S_TRUE_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_CN", (Opcode) FCMPP_S_TRUE_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_CN, "FCMPP_S_TRUE_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_CC", (Opcode) FCMPP_S_TRUE_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_CC, "FCMPP_S_TRUE_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_ON", (Opcode) FCMPP_S_TRUE_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_ON, "FCMPP_S_TRUE_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_OC", (Opcode) FCMPP_S_TRUE_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_OC, "FCMPP_S_TRUE_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_AN", (Opcode) FCMPP_S_TRUE_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_AN, "FCMPP_S_TRUE_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_CC_AC", (Opcode) FCMPP_S_TRUE_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_CC_AC, "FCMPP_S_TRUE_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_UN", (Opcode) FCMPP_S_TRUE_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_UN, "FCMPP_S_TRUE_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_UC", (Opcode) FCMPP_S_TRUE_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_UC, "FCMPP_S_TRUE_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_CN", (Opcode) FCMPP_S_TRUE_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_CN, "FCMPP_S_TRUE_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_CC", (Opcode) FCMPP_S_TRUE_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_CC, "FCMPP_S_TRUE_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_ON", (Opcode) FCMPP_S_TRUE_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_ON, "FCMPP_S_TRUE_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_OC", (Opcode) FCMPP_S_TRUE_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_OC, "FCMPP_S_TRUE_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_AN", (Opcode) FCMPP_S_TRUE_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_AN, "FCMPP_S_TRUE_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_ON_AC", (Opcode) FCMPP_S_TRUE_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_ON_AC, "FCMPP_S_TRUE_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_UN", (Opcode) FCMPP_S_TRUE_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_UN, "FCMPP_S_TRUE_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_UC", (Opcode) FCMPP_S_TRUE_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_UC, "FCMPP_S_TRUE_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_CN", (Opcode) FCMPP_S_TRUE_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_CN, "FCMPP_S_TRUE_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_CC", (Opcode) FCMPP_S_TRUE_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_CC, "FCMPP_S_TRUE_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_ON", (Opcode) FCMPP_S_TRUE_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_ON, "FCMPP_S_TRUE_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_OC", (Opcode) FCMPP_S_TRUE_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_OC, "FCMPP_S_TRUE_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_AN", (Opcode) FCMPP_S_TRUE_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_AN, "FCMPP_S_TRUE_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_OC_AC", (Opcode) FCMPP_S_TRUE_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_OC_AC, "FCMPP_S_TRUE_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_UN", (Opcode) FCMPP_S_TRUE_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_UN, "FCMPP_S_TRUE_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_UC", (Opcode) FCMPP_S_TRUE_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_UC, "FCMPP_S_TRUE_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_CN", (Opcode) FCMPP_S_TRUE_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_CN, "FCMPP_S_TRUE_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_CC", (Opcode) FCMPP_S_TRUE_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_CC, "FCMPP_S_TRUE_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_ON", (Opcode) FCMPP_S_TRUE_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_ON, "FCMPP_S_TRUE_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_OC", (Opcode) FCMPP_S_TRUE_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_OC, "FCMPP_S_TRUE_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_AN", (Opcode) FCMPP_S_TRUE_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_AN, "FCMPP_S_TRUE_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AN_AC", (Opcode) FCMPP_S_TRUE_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AN_AC, "FCMPP_S_TRUE_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_UN", (Opcode) FCMPP_S_TRUE_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_UN, "FCMPP_S_TRUE_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_UC", (Opcode) FCMPP_S_TRUE_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_UC, "FCMPP_S_TRUE_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_CN", (Opcode) FCMPP_S_TRUE_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_CN, "FCMPP_S_TRUE_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_CC", (Opcode) FCMPP_S_TRUE_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_CC, "FCMPP_S_TRUE_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_ON", (Opcode) FCMPP_S_TRUE_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_ON, "FCMPP_S_TRUE_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_OC", (Opcode) FCMPP_S_TRUE_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_OC, "FCMPP_S_TRUE_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_AN", (Opcode) FCMPP_S_TRUE_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_AN, "FCMPP_S_TRUE_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_S_TRUE_AC_AC", (Opcode) FCMPP_S_TRUE_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_S_TRUE_AC_AC, "FCMPP_S_TRUE_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_2()
{
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_UN", (Opcode) FCMPP_D_FALSE_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_UN, "FCMPP_D_FALSE_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_UC", (Opcode) FCMPP_D_FALSE_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_UC, "FCMPP_D_FALSE_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_CN", (Opcode) FCMPP_D_FALSE_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_CN, "FCMPP_D_FALSE_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_CC", (Opcode) FCMPP_D_FALSE_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_CC, "FCMPP_D_FALSE_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_ON", (Opcode) FCMPP_D_FALSE_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_ON, "FCMPP_D_FALSE_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_OC", (Opcode) FCMPP_D_FALSE_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_OC, "FCMPP_D_FALSE_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_AN", (Opcode) FCMPP_D_FALSE_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_AN, "FCMPP_D_FALSE_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UN_AC", (Opcode) FCMPP_D_FALSE_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UN_AC, "FCMPP_D_FALSE_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_UN", (Opcode) FCMPP_D_FALSE_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_UN, "FCMPP_D_FALSE_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_UC", (Opcode) FCMPP_D_FALSE_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_UC, "FCMPP_D_FALSE_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_CN", (Opcode) FCMPP_D_FALSE_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_CN, "FCMPP_D_FALSE_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_CC", (Opcode) FCMPP_D_FALSE_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_CC, "FCMPP_D_FALSE_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_ON", (Opcode) FCMPP_D_FALSE_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_ON, "FCMPP_D_FALSE_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_OC", (Opcode) FCMPP_D_FALSE_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_OC, "FCMPP_D_FALSE_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_AN", (Opcode) FCMPP_D_FALSE_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_AN, "FCMPP_D_FALSE_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_UC_AC", (Opcode) FCMPP_D_FALSE_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_UC_AC, "FCMPP_D_FALSE_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_UN", (Opcode) FCMPP_D_FALSE_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_UN, "FCMPP_D_FALSE_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_UC", (Opcode) FCMPP_D_FALSE_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_UC, "FCMPP_D_FALSE_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_CN", (Opcode) FCMPP_D_FALSE_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_CN, "FCMPP_D_FALSE_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_CC", (Opcode) FCMPP_D_FALSE_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_CC, "FCMPP_D_FALSE_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_ON", (Opcode) FCMPP_D_FALSE_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_ON, "FCMPP_D_FALSE_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_OC", (Opcode) FCMPP_D_FALSE_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_OC, "FCMPP_D_FALSE_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_AN", (Opcode) FCMPP_D_FALSE_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_AN, "FCMPP_D_FALSE_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CN_AC", (Opcode) FCMPP_D_FALSE_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CN_AC, "FCMPP_D_FALSE_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_UN", (Opcode) FCMPP_D_FALSE_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_UN, "FCMPP_D_FALSE_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_UC", (Opcode) FCMPP_D_FALSE_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_UC, "FCMPP_D_FALSE_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_CN", (Opcode) FCMPP_D_FALSE_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_CN, "FCMPP_D_FALSE_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_CC", (Opcode) FCMPP_D_FALSE_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_CC, "FCMPP_D_FALSE_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_ON", (Opcode) FCMPP_D_FALSE_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_ON, "FCMPP_D_FALSE_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_OC", (Opcode) FCMPP_D_FALSE_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_OC, "FCMPP_D_FALSE_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_AN", (Opcode) FCMPP_D_FALSE_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_AN, "FCMPP_D_FALSE_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_CC_AC", (Opcode) FCMPP_D_FALSE_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_CC_AC, "FCMPP_D_FALSE_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_UN", (Opcode) FCMPP_D_FALSE_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_UN, "FCMPP_D_FALSE_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_UC", (Opcode) FCMPP_D_FALSE_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_UC, "FCMPP_D_FALSE_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_CN", (Opcode) FCMPP_D_FALSE_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_CN, "FCMPP_D_FALSE_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_CC", (Opcode) FCMPP_D_FALSE_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_CC, "FCMPP_D_FALSE_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_ON", (Opcode) FCMPP_D_FALSE_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_ON, "FCMPP_D_FALSE_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_OC", (Opcode) FCMPP_D_FALSE_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_OC, "FCMPP_D_FALSE_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_AN", (Opcode) FCMPP_D_FALSE_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_AN, "FCMPP_D_FALSE_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_ON_AC", (Opcode) FCMPP_D_FALSE_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_ON_AC, "FCMPP_D_FALSE_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_UN", (Opcode) FCMPP_D_FALSE_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_UN, "FCMPP_D_FALSE_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_UC", (Opcode) FCMPP_D_FALSE_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_UC, "FCMPP_D_FALSE_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_CN", (Opcode) FCMPP_D_FALSE_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_CN, "FCMPP_D_FALSE_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_CC", (Opcode) FCMPP_D_FALSE_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_CC, "FCMPP_D_FALSE_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_ON", (Opcode) FCMPP_D_FALSE_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_ON, "FCMPP_D_FALSE_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_OC", (Opcode) FCMPP_D_FALSE_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_OC, "FCMPP_D_FALSE_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_AN", (Opcode) FCMPP_D_FALSE_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_AN, "FCMPP_D_FALSE_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_OC_AC", (Opcode) FCMPP_D_FALSE_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_OC_AC, "FCMPP_D_FALSE_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_UN", (Opcode) FCMPP_D_FALSE_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_UN, "FCMPP_D_FALSE_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_UC", (Opcode) FCMPP_D_FALSE_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_UC, "FCMPP_D_FALSE_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_CN", (Opcode) FCMPP_D_FALSE_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_CN, "FCMPP_D_FALSE_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_CC", (Opcode) FCMPP_D_FALSE_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_CC, "FCMPP_D_FALSE_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_ON", (Opcode) FCMPP_D_FALSE_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_ON, "FCMPP_D_FALSE_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_OC", (Opcode) FCMPP_D_FALSE_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_OC, "FCMPP_D_FALSE_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_AN", (Opcode) FCMPP_D_FALSE_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_AN, "FCMPP_D_FALSE_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AN_AC", (Opcode) FCMPP_D_FALSE_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AN_AC, "FCMPP_D_FALSE_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_UN", (Opcode) FCMPP_D_FALSE_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_UN, "FCMPP_D_FALSE_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_UC", (Opcode) FCMPP_D_FALSE_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_UC, "FCMPP_D_FALSE_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_CN", (Opcode) FCMPP_D_FALSE_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_CN, "FCMPP_D_FALSE_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_CC", (Opcode) FCMPP_D_FALSE_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_CC, "FCMPP_D_FALSE_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_ON", (Opcode) FCMPP_D_FALSE_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_ON, "FCMPP_D_FALSE_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_OC", (Opcode) FCMPP_D_FALSE_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_OC, "FCMPP_D_FALSE_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_AN", (Opcode) FCMPP_D_FALSE_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_AN, "FCMPP_D_FALSE_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_FALSE_AC_AC", (Opcode) FCMPP_D_FALSE_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_FALSE_AC_AC, "FCMPP_D_FALSE_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_3()
{
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_UN", (Opcode) FCMPP_D_EQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_UN, "FCMPP_D_EQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_UC", (Opcode) FCMPP_D_EQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_UC, "FCMPP_D_EQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_CN", (Opcode) FCMPP_D_EQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_CN, "FCMPP_D_EQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_CC", (Opcode) FCMPP_D_EQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_CC, "FCMPP_D_EQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_ON", (Opcode) FCMPP_D_EQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_ON, "FCMPP_D_EQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_OC", (Opcode) FCMPP_D_EQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_OC, "FCMPP_D_EQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_AN", (Opcode) FCMPP_D_EQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_AN, "FCMPP_D_EQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UN_AC", (Opcode) FCMPP_D_EQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UN_AC, "FCMPP_D_EQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_UN", (Opcode) FCMPP_D_EQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_UN, "FCMPP_D_EQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_UC", (Opcode) FCMPP_D_EQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_UC, "FCMPP_D_EQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_CN", (Opcode) FCMPP_D_EQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_CN, "FCMPP_D_EQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_CC", (Opcode) FCMPP_D_EQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_CC, "FCMPP_D_EQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_ON", (Opcode) FCMPP_D_EQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_ON, "FCMPP_D_EQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_OC", (Opcode) FCMPP_D_EQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_OC, "FCMPP_D_EQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_AN", (Opcode) FCMPP_D_EQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_AN, "FCMPP_D_EQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_UC_AC", (Opcode) FCMPP_D_EQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_UC_AC, "FCMPP_D_EQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_UN", (Opcode) FCMPP_D_EQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_UN, "FCMPP_D_EQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_UC", (Opcode) FCMPP_D_EQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_UC, "FCMPP_D_EQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_CN", (Opcode) FCMPP_D_EQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_CN, "FCMPP_D_EQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_CC", (Opcode) FCMPP_D_EQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_CC, "FCMPP_D_EQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_ON", (Opcode) FCMPP_D_EQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_ON, "FCMPP_D_EQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_OC", (Opcode) FCMPP_D_EQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_OC, "FCMPP_D_EQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_AN", (Opcode) FCMPP_D_EQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_AN, "FCMPP_D_EQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CN_AC", (Opcode) FCMPP_D_EQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CN_AC, "FCMPP_D_EQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_UN", (Opcode) FCMPP_D_EQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_UN, "FCMPP_D_EQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_UC", (Opcode) FCMPP_D_EQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_UC, "FCMPP_D_EQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_CN", (Opcode) FCMPP_D_EQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_CN, "FCMPP_D_EQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_CC", (Opcode) FCMPP_D_EQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_CC, "FCMPP_D_EQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_ON", (Opcode) FCMPP_D_EQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_ON, "FCMPP_D_EQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_OC", (Opcode) FCMPP_D_EQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_OC, "FCMPP_D_EQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_AN", (Opcode) FCMPP_D_EQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_AN, "FCMPP_D_EQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_CC_AC", (Opcode) FCMPP_D_EQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_CC_AC, "FCMPP_D_EQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_UN", (Opcode) FCMPP_D_EQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_UN, "FCMPP_D_EQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_UC", (Opcode) FCMPP_D_EQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_UC, "FCMPP_D_EQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_CN", (Opcode) FCMPP_D_EQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_CN, "FCMPP_D_EQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_CC", (Opcode) FCMPP_D_EQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_CC, "FCMPP_D_EQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_ON", (Opcode) FCMPP_D_EQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_ON, "FCMPP_D_EQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_OC", (Opcode) FCMPP_D_EQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_OC, "FCMPP_D_EQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_AN", (Opcode) FCMPP_D_EQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_AN, "FCMPP_D_EQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_ON_AC", (Opcode) FCMPP_D_EQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_ON_AC, "FCMPP_D_EQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_UN", (Opcode) FCMPP_D_EQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_UN, "FCMPP_D_EQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_UC", (Opcode) FCMPP_D_EQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_UC, "FCMPP_D_EQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_CN", (Opcode) FCMPP_D_EQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_CN, "FCMPP_D_EQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_CC", (Opcode) FCMPP_D_EQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_CC, "FCMPP_D_EQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_ON", (Opcode) FCMPP_D_EQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_ON, "FCMPP_D_EQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_OC", (Opcode) FCMPP_D_EQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_OC, "FCMPP_D_EQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_AN", (Opcode) FCMPP_D_EQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_AN, "FCMPP_D_EQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_OC_AC", (Opcode) FCMPP_D_EQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_OC_AC, "FCMPP_D_EQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_UN", (Opcode) FCMPP_D_EQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_UN, "FCMPP_D_EQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_UC", (Opcode) FCMPP_D_EQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_UC, "FCMPP_D_EQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_CN", (Opcode) FCMPP_D_EQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_CN, "FCMPP_D_EQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_CC", (Opcode) FCMPP_D_EQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_CC, "FCMPP_D_EQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_ON", (Opcode) FCMPP_D_EQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_ON, "FCMPP_D_EQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_OC", (Opcode) FCMPP_D_EQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_OC, "FCMPP_D_EQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_AN", (Opcode) FCMPP_D_EQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_AN, "FCMPP_D_EQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AN_AC", (Opcode) FCMPP_D_EQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AN_AC, "FCMPP_D_EQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_UN", (Opcode) FCMPP_D_EQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_UN, "FCMPP_D_EQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_UC", (Opcode) FCMPP_D_EQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_UC, "FCMPP_D_EQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_CN", (Opcode) FCMPP_D_EQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_CN, "FCMPP_D_EQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_CC", (Opcode) FCMPP_D_EQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_CC, "FCMPP_D_EQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_ON", (Opcode) FCMPP_D_EQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_ON, "FCMPP_D_EQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_OC", (Opcode) FCMPP_D_EQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_OC, "FCMPP_D_EQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_AN", (Opcode) FCMPP_D_EQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_AN, "FCMPP_D_EQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_EQ_AC_AC", (Opcode) FCMPP_D_EQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_EQ_AC_AC, "FCMPP_D_EQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_4()
{
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_UN", (Opcode) FCMPP_D_LT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_UN, "FCMPP_D_LT_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_UC", (Opcode) FCMPP_D_LT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_UC, "FCMPP_D_LT_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_CN", (Opcode) FCMPP_D_LT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_CN, "FCMPP_D_LT_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_CC", (Opcode) FCMPP_D_LT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_CC, "FCMPP_D_LT_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_ON", (Opcode) FCMPP_D_LT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_ON, "FCMPP_D_LT_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_OC", (Opcode) FCMPP_D_LT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_OC, "FCMPP_D_LT_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_AN", (Opcode) FCMPP_D_LT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_AN, "FCMPP_D_LT_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UN_AC", (Opcode) FCMPP_D_LT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UN_AC, "FCMPP_D_LT_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_UN", (Opcode) FCMPP_D_LT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_UN, "FCMPP_D_LT_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_UC", (Opcode) FCMPP_D_LT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_UC, "FCMPP_D_LT_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_CN", (Opcode) FCMPP_D_LT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_CN, "FCMPP_D_LT_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_CC", (Opcode) FCMPP_D_LT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_CC, "FCMPP_D_LT_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_ON", (Opcode) FCMPP_D_LT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_ON, "FCMPP_D_LT_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_OC", (Opcode) FCMPP_D_LT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_OC, "FCMPP_D_LT_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_AN", (Opcode) FCMPP_D_LT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_AN, "FCMPP_D_LT_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_UC_AC", (Opcode) FCMPP_D_LT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_UC_AC, "FCMPP_D_LT_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_UN", (Opcode) FCMPP_D_LT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_UN, "FCMPP_D_LT_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_UC", (Opcode) FCMPP_D_LT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_UC, "FCMPP_D_LT_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_CN", (Opcode) FCMPP_D_LT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_CN, "FCMPP_D_LT_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_CC", (Opcode) FCMPP_D_LT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_CC, "FCMPP_D_LT_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_ON", (Opcode) FCMPP_D_LT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_ON, "FCMPP_D_LT_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_OC", (Opcode) FCMPP_D_LT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_OC, "FCMPP_D_LT_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_AN", (Opcode) FCMPP_D_LT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_AN, "FCMPP_D_LT_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CN_AC", (Opcode) FCMPP_D_LT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CN_AC, "FCMPP_D_LT_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_UN", (Opcode) FCMPP_D_LT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_UN, "FCMPP_D_LT_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_UC", (Opcode) FCMPP_D_LT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_UC, "FCMPP_D_LT_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_CN", (Opcode) FCMPP_D_LT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_CN, "FCMPP_D_LT_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_CC", (Opcode) FCMPP_D_LT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_CC, "FCMPP_D_LT_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_ON", (Opcode) FCMPP_D_LT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_ON, "FCMPP_D_LT_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_OC", (Opcode) FCMPP_D_LT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_OC, "FCMPP_D_LT_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_AN", (Opcode) FCMPP_D_LT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_AN, "FCMPP_D_LT_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_CC_AC", (Opcode) FCMPP_D_LT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_CC_AC, "FCMPP_D_LT_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_UN", (Opcode) FCMPP_D_LT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_UN, "FCMPP_D_LT_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_UC", (Opcode) FCMPP_D_LT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_UC, "FCMPP_D_LT_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_CN", (Opcode) FCMPP_D_LT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_CN, "FCMPP_D_LT_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_CC", (Opcode) FCMPP_D_LT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_CC, "FCMPP_D_LT_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_ON", (Opcode) FCMPP_D_LT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_ON, "FCMPP_D_LT_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_OC", (Opcode) FCMPP_D_LT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_OC, "FCMPP_D_LT_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_AN", (Opcode) FCMPP_D_LT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_AN, "FCMPP_D_LT_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_ON_AC", (Opcode) FCMPP_D_LT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_ON_AC, "FCMPP_D_LT_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_UN", (Opcode) FCMPP_D_LT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_UN, "FCMPP_D_LT_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_UC", (Opcode) FCMPP_D_LT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_UC, "FCMPP_D_LT_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_CN", (Opcode) FCMPP_D_LT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_CN, "FCMPP_D_LT_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_CC", (Opcode) FCMPP_D_LT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_CC, "FCMPP_D_LT_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_ON", (Opcode) FCMPP_D_LT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_ON, "FCMPP_D_LT_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_OC", (Opcode) FCMPP_D_LT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_OC, "FCMPP_D_LT_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_AN", (Opcode) FCMPP_D_LT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_AN, "FCMPP_D_LT_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_OC_AC", (Opcode) FCMPP_D_LT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_OC_AC, "FCMPP_D_LT_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_UN", (Opcode) FCMPP_D_LT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_UN, "FCMPP_D_LT_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_UC", (Opcode) FCMPP_D_LT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_UC, "FCMPP_D_LT_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_CN", (Opcode) FCMPP_D_LT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_CN, "FCMPP_D_LT_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_CC", (Opcode) FCMPP_D_LT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_CC, "FCMPP_D_LT_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_ON", (Opcode) FCMPP_D_LT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_ON, "FCMPP_D_LT_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_OC", (Opcode) FCMPP_D_LT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_OC, "FCMPP_D_LT_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_AN", (Opcode) FCMPP_D_LT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_AN, "FCMPP_D_LT_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AN_AC", (Opcode) FCMPP_D_LT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AN_AC, "FCMPP_D_LT_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_UN", (Opcode) FCMPP_D_LT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_UN, "FCMPP_D_LT_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_UC", (Opcode) FCMPP_D_LT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_UC, "FCMPP_D_LT_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_CN", (Opcode) FCMPP_D_LT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_CN, "FCMPP_D_LT_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_CC", (Opcode) FCMPP_D_LT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_CC, "FCMPP_D_LT_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_ON", (Opcode) FCMPP_D_LT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_ON, "FCMPP_D_LT_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_OC", (Opcode) FCMPP_D_LT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_OC, "FCMPP_D_LT_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_AN", (Opcode) FCMPP_D_LT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_AN, "FCMPP_D_LT_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LT_AC_AC", (Opcode) FCMPP_D_LT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LT_AC_AC, "FCMPP_D_LT_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_5()
{
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_UN", (Opcode) FCMPP_D_LEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_UN, "FCMPP_D_LEQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_UC", (Opcode) FCMPP_D_LEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_UC, "FCMPP_D_LEQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_CN", (Opcode) FCMPP_D_LEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_CN, "FCMPP_D_LEQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_CC", (Opcode) FCMPP_D_LEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_CC, "FCMPP_D_LEQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_ON", (Opcode) FCMPP_D_LEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_ON, "FCMPP_D_LEQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_OC", (Opcode) FCMPP_D_LEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_OC, "FCMPP_D_LEQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_AN", (Opcode) FCMPP_D_LEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_AN, "FCMPP_D_LEQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UN_AC", (Opcode) FCMPP_D_LEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UN_AC, "FCMPP_D_LEQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_UN", (Opcode) FCMPP_D_LEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_UN, "FCMPP_D_LEQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_UC", (Opcode) FCMPP_D_LEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_UC, "FCMPP_D_LEQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_CN", (Opcode) FCMPP_D_LEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_CN, "FCMPP_D_LEQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_CC", (Opcode) FCMPP_D_LEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_CC, "FCMPP_D_LEQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_ON", (Opcode) FCMPP_D_LEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_ON, "FCMPP_D_LEQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_OC", (Opcode) FCMPP_D_LEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_OC, "FCMPP_D_LEQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_AN", (Opcode) FCMPP_D_LEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_AN, "FCMPP_D_LEQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_UC_AC", (Opcode) FCMPP_D_LEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_UC_AC, "FCMPP_D_LEQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_UN", (Opcode) FCMPP_D_LEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_UN, "FCMPP_D_LEQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_UC", (Opcode) FCMPP_D_LEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_UC, "FCMPP_D_LEQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_CN", (Opcode) FCMPP_D_LEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_CN, "FCMPP_D_LEQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_CC", (Opcode) FCMPP_D_LEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_CC, "FCMPP_D_LEQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_ON", (Opcode) FCMPP_D_LEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_ON, "FCMPP_D_LEQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_OC", (Opcode) FCMPP_D_LEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_OC, "FCMPP_D_LEQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_AN", (Opcode) FCMPP_D_LEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_AN, "FCMPP_D_LEQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CN_AC", (Opcode) FCMPP_D_LEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CN_AC, "FCMPP_D_LEQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_UN", (Opcode) FCMPP_D_LEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_UN, "FCMPP_D_LEQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_UC", (Opcode) FCMPP_D_LEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_UC, "FCMPP_D_LEQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_CN", (Opcode) FCMPP_D_LEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_CN, "FCMPP_D_LEQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_CC", (Opcode) FCMPP_D_LEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_CC, "FCMPP_D_LEQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_ON", (Opcode) FCMPP_D_LEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_ON, "FCMPP_D_LEQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_OC", (Opcode) FCMPP_D_LEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_OC, "FCMPP_D_LEQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_AN", (Opcode) FCMPP_D_LEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_AN, "FCMPP_D_LEQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_CC_AC", (Opcode) FCMPP_D_LEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_CC_AC, "FCMPP_D_LEQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_UN", (Opcode) FCMPP_D_LEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_UN, "FCMPP_D_LEQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_UC", (Opcode) FCMPP_D_LEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_UC, "FCMPP_D_LEQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_CN", (Opcode) FCMPP_D_LEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_CN, "FCMPP_D_LEQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_CC", (Opcode) FCMPP_D_LEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_CC, "FCMPP_D_LEQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_ON", (Opcode) FCMPP_D_LEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_ON, "FCMPP_D_LEQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_OC", (Opcode) FCMPP_D_LEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_OC, "FCMPP_D_LEQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_AN", (Opcode) FCMPP_D_LEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_AN, "FCMPP_D_LEQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_ON_AC", (Opcode) FCMPP_D_LEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_ON_AC, "FCMPP_D_LEQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_UN", (Opcode) FCMPP_D_LEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_UN, "FCMPP_D_LEQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_UC", (Opcode) FCMPP_D_LEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_UC, "FCMPP_D_LEQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_CN", (Opcode) FCMPP_D_LEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_CN, "FCMPP_D_LEQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_CC", (Opcode) FCMPP_D_LEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_CC, "FCMPP_D_LEQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_ON", (Opcode) FCMPP_D_LEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_ON, "FCMPP_D_LEQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_OC", (Opcode) FCMPP_D_LEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_OC, "FCMPP_D_LEQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_AN", (Opcode) FCMPP_D_LEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_AN, "FCMPP_D_LEQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_OC_AC", (Opcode) FCMPP_D_LEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_OC_AC, "FCMPP_D_LEQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_UN", (Opcode) FCMPP_D_LEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_UN, "FCMPP_D_LEQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_UC", (Opcode) FCMPP_D_LEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_UC, "FCMPP_D_LEQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_CN", (Opcode) FCMPP_D_LEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_CN, "FCMPP_D_LEQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_CC", (Opcode) FCMPP_D_LEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_CC, "FCMPP_D_LEQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_ON", (Opcode) FCMPP_D_LEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_ON, "FCMPP_D_LEQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_OC", (Opcode) FCMPP_D_LEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_OC, "FCMPP_D_LEQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_AN", (Opcode) FCMPP_D_LEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_AN, "FCMPP_D_LEQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AN_AC", (Opcode) FCMPP_D_LEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AN_AC, "FCMPP_D_LEQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_UN", (Opcode) FCMPP_D_LEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_UN, "FCMPP_D_LEQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_UC", (Opcode) FCMPP_D_LEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_UC, "FCMPP_D_LEQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_CN", (Opcode) FCMPP_D_LEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_CN, "FCMPP_D_LEQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_CC", (Opcode) FCMPP_D_LEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_CC, "FCMPP_D_LEQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_ON", (Opcode) FCMPP_D_LEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_ON, "FCMPP_D_LEQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_OC", (Opcode) FCMPP_D_LEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_OC, "FCMPP_D_LEQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_AN", (Opcode) FCMPP_D_LEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_AN, "FCMPP_D_LEQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_LEQ_AC_AC", (Opcode) FCMPP_D_LEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_LEQ_AC_AC, "FCMPP_D_LEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_6()
{
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_UN", (Opcode) FCMPP_D_GT_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_UN, "FCMPP_D_GT_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_UC", (Opcode) FCMPP_D_GT_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_UC, "FCMPP_D_GT_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_CN", (Opcode) FCMPP_D_GT_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_CN, "FCMPP_D_GT_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_CC", (Opcode) FCMPP_D_GT_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_CC, "FCMPP_D_GT_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_ON", (Opcode) FCMPP_D_GT_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_ON, "FCMPP_D_GT_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_OC", (Opcode) FCMPP_D_GT_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_OC, "FCMPP_D_GT_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_AN", (Opcode) FCMPP_D_GT_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_AN, "FCMPP_D_GT_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UN_AC", (Opcode) FCMPP_D_GT_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UN_AC, "FCMPP_D_GT_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_UN", (Opcode) FCMPP_D_GT_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_UN, "FCMPP_D_GT_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_UC", (Opcode) FCMPP_D_GT_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_UC, "FCMPP_D_GT_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_CN", (Opcode) FCMPP_D_GT_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_CN, "FCMPP_D_GT_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_CC", (Opcode) FCMPP_D_GT_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_CC, "FCMPP_D_GT_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_ON", (Opcode) FCMPP_D_GT_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_ON, "FCMPP_D_GT_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_OC", (Opcode) FCMPP_D_GT_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_OC, "FCMPP_D_GT_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_AN", (Opcode) FCMPP_D_GT_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_AN, "FCMPP_D_GT_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_UC_AC", (Opcode) FCMPP_D_GT_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_UC_AC, "FCMPP_D_GT_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_UN", (Opcode) FCMPP_D_GT_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_UN, "FCMPP_D_GT_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_UC", (Opcode) FCMPP_D_GT_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_UC, "FCMPP_D_GT_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_CN", (Opcode) FCMPP_D_GT_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_CN, "FCMPP_D_GT_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_CC", (Opcode) FCMPP_D_GT_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_CC, "FCMPP_D_GT_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_ON", (Opcode) FCMPP_D_GT_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_ON, "FCMPP_D_GT_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_OC", (Opcode) FCMPP_D_GT_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_OC, "FCMPP_D_GT_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_AN", (Opcode) FCMPP_D_GT_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_AN, "FCMPP_D_GT_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CN_AC", (Opcode) FCMPP_D_GT_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CN_AC, "FCMPP_D_GT_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_UN", (Opcode) FCMPP_D_GT_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_UN, "FCMPP_D_GT_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_UC", (Opcode) FCMPP_D_GT_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_UC, "FCMPP_D_GT_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_CN", (Opcode) FCMPP_D_GT_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_CN, "FCMPP_D_GT_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_CC", (Opcode) FCMPP_D_GT_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_CC, "FCMPP_D_GT_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_ON", (Opcode) FCMPP_D_GT_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_ON, "FCMPP_D_GT_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_OC", (Opcode) FCMPP_D_GT_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_OC, "FCMPP_D_GT_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_AN", (Opcode) FCMPP_D_GT_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_AN, "FCMPP_D_GT_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_CC_AC", (Opcode) FCMPP_D_GT_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_CC_AC, "FCMPP_D_GT_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_UN", (Opcode) FCMPP_D_GT_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_UN, "FCMPP_D_GT_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_UC", (Opcode) FCMPP_D_GT_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_UC, "FCMPP_D_GT_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_CN", (Opcode) FCMPP_D_GT_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_CN, "FCMPP_D_GT_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_CC", (Opcode) FCMPP_D_GT_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_CC, "FCMPP_D_GT_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_ON", (Opcode) FCMPP_D_GT_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_ON, "FCMPP_D_GT_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_OC", (Opcode) FCMPP_D_GT_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_OC, "FCMPP_D_GT_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_AN", (Opcode) FCMPP_D_GT_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_AN, "FCMPP_D_GT_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_ON_AC", (Opcode) FCMPP_D_GT_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_ON_AC, "FCMPP_D_GT_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_UN", (Opcode) FCMPP_D_GT_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_UN, "FCMPP_D_GT_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_UC", (Opcode) FCMPP_D_GT_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_UC, "FCMPP_D_GT_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_CN", (Opcode) FCMPP_D_GT_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_CN, "FCMPP_D_GT_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_CC", (Opcode) FCMPP_D_GT_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_CC, "FCMPP_D_GT_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_ON", (Opcode) FCMPP_D_GT_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_ON, "FCMPP_D_GT_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_OC", (Opcode) FCMPP_D_GT_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_OC, "FCMPP_D_GT_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_AN", (Opcode) FCMPP_D_GT_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_AN, "FCMPP_D_GT_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_OC_AC", (Opcode) FCMPP_D_GT_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_OC_AC, "FCMPP_D_GT_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_UN", (Opcode) FCMPP_D_GT_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_UN, "FCMPP_D_GT_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_UC", (Opcode) FCMPP_D_GT_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_UC, "FCMPP_D_GT_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_CN", (Opcode) FCMPP_D_GT_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_CN, "FCMPP_D_GT_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_CC", (Opcode) FCMPP_D_GT_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_CC, "FCMPP_D_GT_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_ON", (Opcode) FCMPP_D_GT_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_ON, "FCMPP_D_GT_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_OC", (Opcode) FCMPP_D_GT_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_OC, "FCMPP_D_GT_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_AN", (Opcode) FCMPP_D_GT_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_AN, "FCMPP_D_GT_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AN_AC", (Opcode) FCMPP_D_GT_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AN_AC, "FCMPP_D_GT_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_UN", (Opcode) FCMPP_D_GT_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_UN, "FCMPP_D_GT_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_UC", (Opcode) FCMPP_D_GT_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_UC, "FCMPP_D_GT_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_CN", (Opcode) FCMPP_D_GT_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_CN, "FCMPP_D_GT_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_CC", (Opcode) FCMPP_D_GT_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_CC, "FCMPP_D_GT_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_ON", (Opcode) FCMPP_D_GT_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_ON, "FCMPP_D_GT_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_OC", (Opcode) FCMPP_D_GT_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_OC, "FCMPP_D_GT_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_AN", (Opcode) FCMPP_D_GT_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_AN, "FCMPP_D_GT_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GT_AC_AC", (Opcode) FCMPP_D_GT_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GT_AC_AC, "FCMPP_D_GT_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_7()
{
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_UN", (Opcode) FCMPP_D_GEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_UN, "FCMPP_D_GEQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_UC", (Opcode) FCMPP_D_GEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_UC, "FCMPP_D_GEQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_CN", (Opcode) FCMPP_D_GEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_CN, "FCMPP_D_GEQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_CC", (Opcode) FCMPP_D_GEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_CC, "FCMPP_D_GEQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_ON", (Opcode) FCMPP_D_GEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_ON, "FCMPP_D_GEQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_OC", (Opcode) FCMPP_D_GEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_OC, "FCMPP_D_GEQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_AN", (Opcode) FCMPP_D_GEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_AN, "FCMPP_D_GEQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UN_AC", (Opcode) FCMPP_D_GEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UN_AC, "FCMPP_D_GEQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_UN", (Opcode) FCMPP_D_GEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_UN, "FCMPP_D_GEQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_UC", (Opcode) FCMPP_D_GEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_UC, "FCMPP_D_GEQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_CN", (Opcode) FCMPP_D_GEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_CN, "FCMPP_D_GEQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_CC", (Opcode) FCMPP_D_GEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_CC, "FCMPP_D_GEQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_ON", (Opcode) FCMPP_D_GEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_ON, "FCMPP_D_GEQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_OC", (Opcode) FCMPP_D_GEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_OC, "FCMPP_D_GEQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_AN", (Opcode) FCMPP_D_GEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_AN, "FCMPP_D_GEQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_UC_AC", (Opcode) FCMPP_D_GEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_UC_AC, "FCMPP_D_GEQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_UN", (Opcode) FCMPP_D_GEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_UN, "FCMPP_D_GEQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_UC", (Opcode) FCMPP_D_GEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_UC, "FCMPP_D_GEQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_CN", (Opcode) FCMPP_D_GEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_CN, "FCMPP_D_GEQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_CC", (Opcode) FCMPP_D_GEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_CC, "FCMPP_D_GEQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_ON", (Opcode) FCMPP_D_GEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_ON, "FCMPP_D_GEQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_OC", (Opcode) FCMPP_D_GEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_OC, "FCMPP_D_GEQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_AN", (Opcode) FCMPP_D_GEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_AN, "FCMPP_D_GEQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CN_AC", (Opcode) FCMPP_D_GEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CN_AC, "FCMPP_D_GEQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_UN", (Opcode) FCMPP_D_GEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_UN, "FCMPP_D_GEQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_UC", (Opcode) FCMPP_D_GEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_UC, "FCMPP_D_GEQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_CN", (Opcode) FCMPP_D_GEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_CN, "FCMPP_D_GEQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_CC", (Opcode) FCMPP_D_GEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_CC, "FCMPP_D_GEQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_ON", (Opcode) FCMPP_D_GEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_ON, "FCMPP_D_GEQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_OC", (Opcode) FCMPP_D_GEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_OC, "FCMPP_D_GEQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_AN", (Opcode) FCMPP_D_GEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_AN, "FCMPP_D_GEQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_CC_AC", (Opcode) FCMPP_D_GEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_CC_AC, "FCMPP_D_GEQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_UN", (Opcode) FCMPP_D_GEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_UN, "FCMPP_D_GEQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_UC", (Opcode) FCMPP_D_GEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_UC, "FCMPP_D_GEQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_CN", (Opcode) FCMPP_D_GEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_CN, "FCMPP_D_GEQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_CC", (Opcode) FCMPP_D_GEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_CC, "FCMPP_D_GEQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_ON", (Opcode) FCMPP_D_GEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_ON, "FCMPP_D_GEQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_OC", (Opcode) FCMPP_D_GEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_OC, "FCMPP_D_GEQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_AN", (Opcode) FCMPP_D_GEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_AN, "FCMPP_D_GEQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_ON_AC", (Opcode) FCMPP_D_GEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_ON_AC, "FCMPP_D_GEQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_UN", (Opcode) FCMPP_D_GEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_UN, "FCMPP_D_GEQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_UC", (Opcode) FCMPP_D_GEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_UC, "FCMPP_D_GEQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_CN", (Opcode) FCMPP_D_GEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_CN, "FCMPP_D_GEQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_CC", (Opcode) FCMPP_D_GEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_CC, "FCMPP_D_GEQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_ON", (Opcode) FCMPP_D_GEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_ON, "FCMPP_D_GEQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_OC", (Opcode) FCMPP_D_GEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_OC, "FCMPP_D_GEQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_AN", (Opcode) FCMPP_D_GEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_AN, "FCMPP_D_GEQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_OC_AC", (Opcode) FCMPP_D_GEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_OC_AC, "FCMPP_D_GEQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_UN", (Opcode) FCMPP_D_GEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_UN, "FCMPP_D_GEQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_UC", (Opcode) FCMPP_D_GEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_UC, "FCMPP_D_GEQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_CN", (Opcode) FCMPP_D_GEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_CN, "FCMPP_D_GEQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_CC", (Opcode) FCMPP_D_GEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_CC, "FCMPP_D_GEQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_ON", (Opcode) FCMPP_D_GEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_ON, "FCMPP_D_GEQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_OC", (Opcode) FCMPP_D_GEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_OC, "FCMPP_D_GEQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_AN", (Opcode) FCMPP_D_GEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_AN, "FCMPP_D_GEQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AN_AC", (Opcode) FCMPP_D_GEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AN_AC, "FCMPP_D_GEQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_UN", (Opcode) FCMPP_D_GEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_UN, "FCMPP_D_GEQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_UC", (Opcode) FCMPP_D_GEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_UC, "FCMPP_D_GEQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_CN", (Opcode) FCMPP_D_GEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_CN, "FCMPP_D_GEQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_CC", (Opcode) FCMPP_D_GEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_CC, "FCMPP_D_GEQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_ON", (Opcode) FCMPP_D_GEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_ON, "FCMPP_D_GEQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_OC", (Opcode) FCMPP_D_GEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_OC, "FCMPP_D_GEQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_AN", (Opcode) FCMPP_D_GEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_AN, "FCMPP_D_GEQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_GEQ_AC_AC", (Opcode) FCMPP_D_GEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_GEQ_AC_AC, "FCMPP_D_GEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_8()
{
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_UN", (Opcode) FCMPP_D_NEQ_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_UN, "FCMPP_D_NEQ_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_UC", (Opcode) FCMPP_D_NEQ_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_UC, "FCMPP_D_NEQ_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_CN", (Opcode) FCMPP_D_NEQ_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_CN, "FCMPP_D_NEQ_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_CC", (Opcode) FCMPP_D_NEQ_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_CC, "FCMPP_D_NEQ_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_ON", (Opcode) FCMPP_D_NEQ_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_ON, "FCMPP_D_NEQ_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_OC", (Opcode) FCMPP_D_NEQ_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_OC, "FCMPP_D_NEQ_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_AN", (Opcode) FCMPP_D_NEQ_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_AN, "FCMPP_D_NEQ_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UN_AC", (Opcode) FCMPP_D_NEQ_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UN_AC, "FCMPP_D_NEQ_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_UN", (Opcode) FCMPP_D_NEQ_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_UN, "FCMPP_D_NEQ_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_UC", (Opcode) FCMPP_D_NEQ_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_UC, "FCMPP_D_NEQ_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_CN", (Opcode) FCMPP_D_NEQ_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_CN, "FCMPP_D_NEQ_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_CC", (Opcode) FCMPP_D_NEQ_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_CC, "FCMPP_D_NEQ_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_ON", (Opcode) FCMPP_D_NEQ_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_ON, "FCMPP_D_NEQ_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_OC", (Opcode) FCMPP_D_NEQ_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_OC, "FCMPP_D_NEQ_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_AN", (Opcode) FCMPP_D_NEQ_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_AN, "FCMPP_D_NEQ_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_UC_AC", (Opcode) FCMPP_D_NEQ_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_UC_AC, "FCMPP_D_NEQ_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_UN", (Opcode) FCMPP_D_NEQ_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_UN, "FCMPP_D_NEQ_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_UC", (Opcode) FCMPP_D_NEQ_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_UC, "FCMPP_D_NEQ_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_CN", (Opcode) FCMPP_D_NEQ_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_CN, "FCMPP_D_NEQ_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_CC", (Opcode) FCMPP_D_NEQ_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_CC, "FCMPP_D_NEQ_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_ON", (Opcode) FCMPP_D_NEQ_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_ON, "FCMPP_D_NEQ_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_OC", (Opcode) FCMPP_D_NEQ_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_OC, "FCMPP_D_NEQ_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_AN", (Opcode) FCMPP_D_NEQ_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_AN, "FCMPP_D_NEQ_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CN_AC", (Opcode) FCMPP_D_NEQ_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CN_AC, "FCMPP_D_NEQ_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_UN", (Opcode) FCMPP_D_NEQ_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_UN, "FCMPP_D_NEQ_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_UC", (Opcode) FCMPP_D_NEQ_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_UC, "FCMPP_D_NEQ_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_CN", (Opcode) FCMPP_D_NEQ_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_CN, "FCMPP_D_NEQ_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_CC", (Opcode) FCMPP_D_NEQ_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_CC, "FCMPP_D_NEQ_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_ON", (Opcode) FCMPP_D_NEQ_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_ON, "FCMPP_D_NEQ_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_OC", (Opcode) FCMPP_D_NEQ_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_OC, "FCMPP_D_NEQ_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_AN", (Opcode) FCMPP_D_NEQ_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_AN, "FCMPP_D_NEQ_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_CC_AC", (Opcode) FCMPP_D_NEQ_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_CC_AC, "FCMPP_D_NEQ_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_UN", (Opcode) FCMPP_D_NEQ_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_UN, "FCMPP_D_NEQ_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_UC", (Opcode) FCMPP_D_NEQ_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_UC, "FCMPP_D_NEQ_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_CN", (Opcode) FCMPP_D_NEQ_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_CN, "FCMPP_D_NEQ_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_CC", (Opcode) FCMPP_D_NEQ_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_CC, "FCMPP_D_NEQ_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_ON", (Opcode) FCMPP_D_NEQ_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_ON, "FCMPP_D_NEQ_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_OC", (Opcode) FCMPP_D_NEQ_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_OC, "FCMPP_D_NEQ_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_AN", (Opcode) FCMPP_D_NEQ_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_AN, "FCMPP_D_NEQ_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_ON_AC", (Opcode) FCMPP_D_NEQ_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_ON_AC, "FCMPP_D_NEQ_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_UN", (Opcode) FCMPP_D_NEQ_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_UN, "FCMPP_D_NEQ_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_UC", (Opcode) FCMPP_D_NEQ_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_UC, "FCMPP_D_NEQ_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_CN", (Opcode) FCMPP_D_NEQ_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_CN, "FCMPP_D_NEQ_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_CC", (Opcode) FCMPP_D_NEQ_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_CC, "FCMPP_D_NEQ_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_ON", (Opcode) FCMPP_D_NEQ_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_ON, "FCMPP_D_NEQ_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_OC", (Opcode) FCMPP_D_NEQ_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_OC, "FCMPP_D_NEQ_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_AN", (Opcode) FCMPP_D_NEQ_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_AN, "FCMPP_D_NEQ_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_OC_AC", (Opcode) FCMPP_D_NEQ_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_OC_AC, "FCMPP_D_NEQ_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_UN", (Opcode) FCMPP_D_NEQ_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_UN, "FCMPP_D_NEQ_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_UC", (Opcode) FCMPP_D_NEQ_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_UC, "FCMPP_D_NEQ_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_CN", (Opcode) FCMPP_D_NEQ_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_CN, "FCMPP_D_NEQ_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_CC", (Opcode) FCMPP_D_NEQ_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_CC, "FCMPP_D_NEQ_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_ON", (Opcode) FCMPP_D_NEQ_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_ON, "FCMPP_D_NEQ_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_OC", (Opcode) FCMPP_D_NEQ_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_OC, "FCMPP_D_NEQ_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_AN", (Opcode) FCMPP_D_NEQ_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_AN, "FCMPP_D_NEQ_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AN_AC", (Opcode) FCMPP_D_NEQ_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AN_AC, "FCMPP_D_NEQ_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_UN", (Opcode) FCMPP_D_NEQ_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_UN, "FCMPP_D_NEQ_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_UC", (Opcode) FCMPP_D_NEQ_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_UC, "FCMPP_D_NEQ_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_CN", (Opcode) FCMPP_D_NEQ_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_CN, "FCMPP_D_NEQ_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_CC", (Opcode) FCMPP_D_NEQ_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_CC, "FCMPP_D_NEQ_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_ON", (Opcode) FCMPP_D_NEQ_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_ON, "FCMPP_D_NEQ_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_OC", (Opcode) FCMPP_D_NEQ_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_OC, "FCMPP_D_NEQ_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_AN", (Opcode) FCMPP_D_NEQ_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_AN, "FCMPP_D_NEQ_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_NEQ_AC_AC", (Opcode) FCMPP_D_NEQ_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_NEQ_AC_AC, "FCMPP_D_NEQ_AC_AC") ;
}

static void el_init_elcor_opcode_maps_fcmpp2_9()
{
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_UN", (Opcode) FCMPP_D_TRUE_UN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_UN, "FCMPP_D_TRUE_UN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_UC", (Opcode) FCMPP_D_TRUE_UN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_UC, "FCMPP_D_TRUE_UN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_CN", (Opcode) FCMPP_D_TRUE_UN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_CN, "FCMPP_D_TRUE_UN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_CC", (Opcode) FCMPP_D_TRUE_UN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_CC, "FCMPP_D_TRUE_UN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_ON", (Opcode) FCMPP_D_TRUE_UN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_ON, "FCMPP_D_TRUE_UN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_OC", (Opcode) FCMPP_D_TRUE_UN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_OC, "FCMPP_D_TRUE_UN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_AN", (Opcode) FCMPP_D_TRUE_UN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_AN, "FCMPP_D_TRUE_UN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UN_AC", (Opcode) FCMPP_D_TRUE_UN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UN_AC, "FCMPP_D_TRUE_UN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_UN", (Opcode) FCMPP_D_TRUE_UC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_UN, "FCMPP_D_TRUE_UC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_UC", (Opcode) FCMPP_D_TRUE_UC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_UC, "FCMPP_D_TRUE_UC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_CN", (Opcode) FCMPP_D_TRUE_UC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_CN, "FCMPP_D_TRUE_UC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_CC", (Opcode) FCMPP_D_TRUE_UC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_CC, "FCMPP_D_TRUE_UC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_ON", (Opcode) FCMPP_D_TRUE_UC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_ON, "FCMPP_D_TRUE_UC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_OC", (Opcode) FCMPP_D_TRUE_UC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_OC, "FCMPP_D_TRUE_UC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_AN", (Opcode) FCMPP_D_TRUE_UC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_AN, "FCMPP_D_TRUE_UC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_UC_AC", (Opcode) FCMPP_D_TRUE_UC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_UC_AC, "FCMPP_D_TRUE_UC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_UN", (Opcode) FCMPP_D_TRUE_CN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_UN, "FCMPP_D_TRUE_CN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_UC", (Opcode) FCMPP_D_TRUE_CN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_UC, "FCMPP_D_TRUE_CN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_CN", (Opcode) FCMPP_D_TRUE_CN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_CN, "FCMPP_D_TRUE_CN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_CC", (Opcode) FCMPP_D_TRUE_CN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_CC, "FCMPP_D_TRUE_CN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_ON", (Opcode) FCMPP_D_TRUE_CN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_ON, "FCMPP_D_TRUE_CN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_OC", (Opcode) FCMPP_D_TRUE_CN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_OC, "FCMPP_D_TRUE_CN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_AN", (Opcode) FCMPP_D_TRUE_CN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_AN, "FCMPP_D_TRUE_CN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CN_AC", (Opcode) FCMPP_D_TRUE_CN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CN_AC, "FCMPP_D_TRUE_CN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_UN", (Opcode) FCMPP_D_TRUE_CC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_UN, "FCMPP_D_TRUE_CC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_UC", (Opcode) FCMPP_D_TRUE_CC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_UC, "FCMPP_D_TRUE_CC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_CN", (Opcode) FCMPP_D_TRUE_CC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_CN, "FCMPP_D_TRUE_CC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_CC", (Opcode) FCMPP_D_TRUE_CC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_CC, "FCMPP_D_TRUE_CC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_ON", (Opcode) FCMPP_D_TRUE_CC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_ON, "FCMPP_D_TRUE_CC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_OC", (Opcode) FCMPP_D_TRUE_CC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_OC, "FCMPP_D_TRUE_CC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_AN", (Opcode) FCMPP_D_TRUE_CC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_AN, "FCMPP_D_TRUE_CC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_CC_AC", (Opcode) FCMPP_D_TRUE_CC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_CC_AC, "FCMPP_D_TRUE_CC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_UN", (Opcode) FCMPP_D_TRUE_ON_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_UN, "FCMPP_D_TRUE_ON_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_UC", (Opcode) FCMPP_D_TRUE_ON_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_UC, "FCMPP_D_TRUE_ON_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_CN", (Opcode) FCMPP_D_TRUE_ON_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_CN, "FCMPP_D_TRUE_ON_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_CC", (Opcode) FCMPP_D_TRUE_ON_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_CC, "FCMPP_D_TRUE_ON_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_ON", (Opcode) FCMPP_D_TRUE_ON_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_ON, "FCMPP_D_TRUE_ON_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_OC", (Opcode) FCMPP_D_TRUE_ON_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_OC, "FCMPP_D_TRUE_ON_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_AN", (Opcode) FCMPP_D_TRUE_ON_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_AN, "FCMPP_D_TRUE_ON_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_ON_AC", (Opcode) FCMPP_D_TRUE_ON_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_ON_AC, "FCMPP_D_TRUE_ON_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_UN", (Opcode) FCMPP_D_TRUE_OC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_UN, "FCMPP_D_TRUE_OC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_UC", (Opcode) FCMPP_D_TRUE_OC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_UC, "FCMPP_D_TRUE_OC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_CN", (Opcode) FCMPP_D_TRUE_OC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_CN, "FCMPP_D_TRUE_OC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_CC", (Opcode) FCMPP_D_TRUE_OC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_CC, "FCMPP_D_TRUE_OC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_ON", (Opcode) FCMPP_D_TRUE_OC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_ON, "FCMPP_D_TRUE_OC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_OC", (Opcode) FCMPP_D_TRUE_OC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_OC, "FCMPP_D_TRUE_OC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_AN", (Opcode) FCMPP_D_TRUE_OC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_AN, "FCMPP_D_TRUE_OC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_OC_AC", (Opcode) FCMPP_D_TRUE_OC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_OC_AC, "FCMPP_D_TRUE_OC_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_UN", (Opcode) FCMPP_D_TRUE_AN_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_UN, "FCMPP_D_TRUE_AN_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_UC", (Opcode) FCMPP_D_TRUE_AN_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_UC, "FCMPP_D_TRUE_AN_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_CN", (Opcode) FCMPP_D_TRUE_AN_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_CN, "FCMPP_D_TRUE_AN_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_CC", (Opcode) FCMPP_D_TRUE_AN_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_CC, "FCMPP_D_TRUE_AN_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_ON", (Opcode) FCMPP_D_TRUE_AN_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_ON, "FCMPP_D_TRUE_AN_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_OC", (Opcode) FCMPP_D_TRUE_AN_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_OC, "FCMPP_D_TRUE_AN_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_AN", (Opcode) FCMPP_D_TRUE_AN_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_AN, "FCMPP_D_TRUE_AN_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AN_AC", (Opcode) FCMPP_D_TRUE_AN_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AN_AC, "FCMPP_D_TRUE_AN_AC") ;

   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_UN", (Opcode) FCMPP_D_TRUE_AC_UN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_UN, "FCMPP_D_TRUE_AC_UN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_UC", (Opcode) FCMPP_D_TRUE_AC_UC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_UC, "FCMPP_D_TRUE_AC_UC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_CN", (Opcode) FCMPP_D_TRUE_AC_CN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_CN, "FCMPP_D_TRUE_AC_CN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_CC", (Opcode) FCMPP_D_TRUE_AC_CC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_CC, "FCMPP_D_TRUE_AC_CC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_ON", (Opcode) FCMPP_D_TRUE_AC_ON) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_ON, "FCMPP_D_TRUE_AC_ON") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_OC", (Opcode) FCMPP_D_TRUE_AC_OC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_OC, "FCMPP_D_TRUE_AC_OC") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_AN", (Opcode) FCMPP_D_TRUE_AC_AN) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_AN, "FCMPP_D_TRUE_AC_AN") ;
   el_string_to_opcode_map.bind("FCMPP_D_TRUE_AC_AC", (Opcode) FCMPP_D_TRUE_AC_AC) ;
   el_opcode_to_string_map.bind((Opcode) FCMPP_D_TRUE_AC_AC, "FCMPP_D_TRUE_AC_AC") ;
}

void el_init_elcor_opcode_maps_fcmpp2()
{
   el_init_elcor_opcode_maps_fcmpp2_1() ;
   el_init_elcor_opcode_maps_fcmpp2_2() ;
   el_init_elcor_opcode_maps_fcmpp2_3() ;
   el_init_elcor_opcode_maps_fcmpp2_4() ;
   el_init_elcor_opcode_maps_fcmpp2_5() ;
   el_init_elcor_opcode_maps_fcmpp2_6() ;
   el_init_elcor_opcode_maps_fcmpp2_7() ;
   el_init_elcor_opcode_maps_fcmpp2_8() ;
   el_init_elcor_opcode_maps_fcmpp2_9() ;
}

