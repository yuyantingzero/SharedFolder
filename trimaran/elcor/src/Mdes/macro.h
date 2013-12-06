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
//      File:           macro.h
//      Authors:        Mike Chu
//      Created:        November 2002
//      Description:    Macro class declaration 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MACRO_H
#define _MACRO_H

#include <iostream>
#include "defs.h"
#include "string_class.h"
#include "hash_map.h"
#include "hash_set.h"

using namespace std;

enum Macro_name {
  UNDEFINED=0,
  FIRST_MACRO=1,
  LOCAL=1,
  PARAM,
  SWAP,
  INT_RETURN_TYPE,
  FLT_RETURN_TYPE,
  DBL_RETURN_TYPE,
  INT_RETURN,
  INT_PARAM_1,
  INT_PARAM_2,
  INT_PARAM_3,
  INT_PARAM_4,
  FLT_RETURN,
  FLT_PARAM_1,
  FLT_PARAM_2,
  FLT_PARAM_3,
  FLT_PARAM_4,
  DBL_RETURN,
  DBL_PARAM_1,
  DBL_PARAM_2,
  DBL_PARAM_3,
  DBL_PARAM_4,
  INT_TM_TYPE,		/* INT Thru memory parameter */
  FLT_TM_TYPE,		/* INT Thru memory parameter */
  DBL_TM_TYPE,		/* INT Thru memory parameter */
  SP_REG,		/* Stack pointer */
  FP_REG,		/* Frame pointer */
  IP_REG,		/* pointer to incoming parameter space */
  OP_REG,		/* pointer to outgoing parameter space */
  LV_REG,		/* pointer to local variable space     */
  RGS_REG,		/* pointer to register swap space      */
  LC_REG,		/* Loop counter */
  ESC_REG,		/* Epilogue stage counter */
  ALL_PRED,  		/* used to refer to the entire predicate register file */
  ALL_ROT_PRED,		/* all the rotating predicate regs */
  ALL_STATIC_PRED,	/* all the non-rotating predicate regs */
  RRB,
  RETURN_ADDR,
  FLT_ZERO,
  FLT_ONE,
  DBL_ZERO,
  DBL_ONE,
  INT_ZERO,
  PRED_FALSE,
  PRED_TRUE,
  /* RMR { */
  VEC_MASK_TRUE,      /* default all true vector mask */
  VEC_SCRATCH_PAD,    /* tied to temp space in memory */
  /* } RMR */
  SPILL_TEMPREG,
  ICMOVE_REG,
  PV_0,                 // PV_0, ..., PV_7 are control register aliases for
  PV_1,                 // 32-bit wide access to predicate registers.
  PV_2,
  PV_3,
  PV_4,
  PV_5,
  PV_6,
  PV_7,
  EBR_BUS,		//support distributed instruction fetch
  RESERVE,
  LAST_MACRO		/* Insert all macros before this one! */
};

class Macro_descr {
   friend ostream& operator<<(ostream& os, const Macro_descr& macro);
public:
   Macro_descr();
   Macro_descr(eString&);

   eString get_name();
   eString get_macro_aliased();
   eString get_pv_file();
   
   bool is_pseudo();
   bool is_alias();   
   bool is_read_only();   
   bool is_pv();
   bool is_in_pf(eString pf);
   bool aliases_macro(eString macro);

   void set_pseudo(bool pseudo);
   void set_alias(bool alias);
   void set_multialias(bool multialias);
   void set_coherent(bool cohernet);
   void set_read_only(bool read_only);
   void set_pv(bool pv);
   void set_macro_aliased(eString macro);
   void add_physical_file(eString pf);
  
   void set_pv_file(eString file) {pv_file = file;};

protected:
   void print(ostream&) const;

private:   
   eString name;
   eString macro_aliased; // which macro this one aliases (i.e. IP -> SP)
   bool pseudo;
   bool alias;               // is an alias
   bool multialias;
   bool coherent;
   bool read_only;
   bool pv;

   eString pv_file;
   Hash_set<eString> potential_homes; // regfiles this macro can belong in
};

#endif
