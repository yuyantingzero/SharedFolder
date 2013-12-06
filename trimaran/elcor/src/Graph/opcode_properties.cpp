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
//      File:           opcode_properties.cpp
//      Authors:        Sadun Anik, Richard Johnson, Scott Mahlke,
//                      Dave August, Sumedh Sathaye
//      Created:        December 1994
//      Description:    Functions specific to opcode values 
//
/////////////////////////////////////////////////////////////////////////////

#include "iterators.h"
#include "opcode_properties.h"
#include "op.h"
#include "dbg.h"
#include "operand.h"
#include "opcode_load_store.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Macros
//
//////////////////////////////////////////////////////////////////////////////

IR_ROOT_OPCODE get_root(Opcode opc) 
{
   return ((IR_ROOT_OPCODE) (opc & ROOT_MASK)) ;
}

IR_ROOT_OPCODE get_root(IR_BASE_OPCODE opc) 
{
   return ((IR_ROOT_OPCODE) (opc & ROOT_MASK)) ;
}


IR_BASE_OPCODE get_base(Opcode opc) 
{
  assert (is_cmpp(opc));
  return ((IR_BASE_OPCODE) (opc & IR_CMPP_BASE_MASK)) ;
}

IR_MODIFIERS get_cond_modifier(Opcode opc)
{
    return ((IR_MODIFIERS) (opc & IR_COND_MASK));
}

///////////////////////////////////////////////////////////////////////////////
//
//	Branch queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_control_switch(Op* op_ptr)
{
   return is_control_switch(op_ptr->opcode()) ;
}

bool is_control_switch(Opcode opc)
{
   if (opc == BRDVI ||
	opc == BRDVF ||
	opc == BRU ||
	opc == BRCT ||
	opc == BRCF ||
        opc == BRL ||
	opc == BRLC ||
	opc == BRLP ||
	opc == BRDIR ||
	opc == BRIND ||
	opc == BRF_B_B_F ||
	opc == BRF_B_F_F ||
	opc == BRF_F_B_B ||
	opc == BRF_F_F_B ||
	opc == BRF_F_F_F ||
	opc == BRW_B_B_F ||
	opc == BRW_B_F_F ||
	opc == BRW_F_B_B ||
	opc == BRW_F_F_B ||
	opc == BRW_F_F_F ||
	opc == RTS ||
        opc == DUMMY_BR ) {
      return true ;
   } else {
      return false ;
   }
}

bool is_branch(Op* op_ptr)
{
   return is_branch(op_ptr->opcode()) ;
}

bool is_branch(Opcode opc)
{
   if (opc == BRDVI ||
	opc == BRDVF ||
	opc == BRU ||
	opc == BRCT ||
	opc == BRCF ||
        opc == BRLC ||
        opc == BRLP ||
        opc == BRDIR ||
        opc == BRIND ||
	opc == BRF_B_B_F ||
	opc == BRF_B_F_F ||
	opc == BRF_F_B_B ||
	opc == BRF_F_F_B ||
	opc == BRF_F_F_F ||
	opc == BRW_B_B_F ||
	opc == BRW_B_F_F ||
	opc == BRW_F_B_B ||
	opc == BRW_F_F_B ||
	opc == BRW_F_F_F ||
	opc == RTS ||
	opc == DUMMY_BR ) {
      return true ;
   } else {
      return false ;
   }
}

bool is_proper_branch(Op* op_ptr)
{
   return is_proper_branch(op_ptr->opcode()) ;
}

bool is_proper_branch(Opcode opc)
{
   if (opc == BRDVI ||
	opc == BRDVF ||
	opc == BRU ||
	opc == BRCT ||
	opc == BRCF ||
        opc == BRLC ||
        opc == BRLP ||
        opc == BRDIR ||
        opc == BRIND ||
	opc == BRF_B_B_F ||
	opc == BRF_B_F_F ||
	opc == BRF_F_B_B ||
	opc == BRF_F_F_B ||
	opc == BRF_F_F_F ||
	opc == BRW_B_B_F ||
	opc == BRW_B_F_F ||
	opc == BRW_F_B_B ||
	opc == BRW_F_F_B ||
	opc == BRW_F_F_F ) {
      return true ;
   } else {
      return false ;
   }
}

bool is_dummy_branch(Op* op_ptr)
{
   return is_dummy_branch(op_ptr->opcode()) ;
}

bool is_dummy_branch(Opcode opc)
{
  if (opc == DUMMY_BR) {
    return true ;
  } else {
    return false ;
  }
}

bool is_normalized_branch(Op* op_ptr)
{
   return is_normalized_branch(op_ptr->opcode()) ;
}

bool is_normalized_branch(Opcode opc)
{
  if (opc == BRDIR||
      opc == BRIND) 
    return true;
  else
    return false;	 
}

bool is_traditional_branch(Op* op_ptr)
{
   return is_traditional_branch(op_ptr->opcode()) ;
}

bool is_traditional_branch(Opcode opc)
{
   if (	opc == BRU ||
	opc == BRCT ||
	opc == BRCF ||
	opc == DUMMY_BR||
	opc == BRDIR||
	opc == BRIND) {
	return true ;
   } else {
      return false ;
   }
}

bool is_conditional_branch(Op* op_ptr)
{
  if(is_brdir(op_ptr))
    return !op_ptr->src(SRC1).is_undefined();
  else
    return is_conditional_branch(op_ptr->opcode());
}

bool is_conditional_branch(Opcode opc)
{
   if (	opc == BRCT ||
	opc == BRCF ) {
      return true ;
   } else {
      return false ;
   }
}

bool is_unconditional_branch(Op* op_ptr)
{
  if(is_brdir(op_ptr))
    return op_ptr->src(SRC1).is_undefined();
  else
    return is_unconditional_branch(op_ptr->opcode()) ;
}

bool is_unconditional_branch(Opcode opc)
{
   if (	opc == BRU ||
        opc == DUMMY_BR||
	opc == BRIND) {
      return true ;
   } else {
      return false ;
   }
}

bool is_brl(Op* op_ptr)
{
  return is_brl(op_ptr->opcode());
}

bool is_brl(Opcode opc)
{
  if (opc == BRL) {
    return true;
  } else {
    return false;
  }
}

bool is_brlp(Op* op_ptr)
{
  return is_brlp(op_ptr->opcode());
}

bool is_brlp(Opcode opc)
{
  if (opc == BRLP) {
    return true;
  } else {
    return false;
  }
}

bool is_rts(Op* op_ptr)
{
  return is_rts(op_ptr->opcode());
}

bool is_rts(Opcode opc)
{
  if (opc == RTS) {
    return true;
  } else {
    return false;
  }
}

bool is_brlc(Op* op_ptr)
{
  return is_brlc(op_ptr->opcode());
}

bool is_brlc(Opcode opc)
{
  if (opc == BRLC) {
    return true;
  } else {
    return false;
  }
}

bool is_brind(Op* op_ptr) 
{
  return is_brind(op_ptr->opcode());
}

bool is_brind(Opcode opc) 
{
  return (opc == BRIND);
}


bool is_brdir(Op* op_ptr)
{
  return is_brdir(op_ptr->opcode());
}

bool is_brdir(Opcode opc)
{
  return (opc == BRDIR);
}

bool is_brct(Op* op_ptr)
{
  return is_brct(op_ptr->opcode());
}

bool is_brct(Opcode opc)
{
  if (opc == BRCT) {
    return true;
  } else {
    return false;
  }
}


bool is_brcf(Op* op_ptr)
{
  return is_brcf(op_ptr->opcode());
}

bool is_brcf(Opcode opc)
{
  if (opc == BRCF) {
    return true;
  } else {
    return false;
  }
}

bool is_bru(Op* op_ptr)
{
  return is_bru(op_ptr->opcode());
}

bool is_bru(Opcode opc)
{
  if (opc == BRU) {
    return true;
  } else {
    return false;
  }
}

bool is_brdv(Op* op_ptr)
{
  return is_brdv(op_ptr->opcode());
}

bool is_brdv(Opcode opc)
{
  if (opc == BRDVI || opc == BRDVF) {
    return true;
  } else {
    return false;
  }
}

bool is_brf(Opcode opc)
{
    if (opc==BRF_B_B_F || opc==BRF_B_F_F || opc==BRF_F_B_B ||
	opc==BRF_F_F_B || opc==BRF_F_F_F)
	return (true);
    else
	return (false);
}

bool is_brf(Op* op_ptr)
{
  return is_brf(op_ptr->opcode());
}

bool is_brw(Opcode opc)
{
    if (opc==BRW_B_B_F || opc==BRW_B_F_F || opc==BRW_F_B_B ||
	opc==BRW_F_F_B || opc==BRW_F_F_F)
	return (true);
    else
	return (false);
}

bool is_brw(Op* op_ptr)
{
  return is_brw(op_ptr->opcode());
}

bool is_swp_branch(Opcode opc)
{
    return (is_brf(opc)||is_brw(opc));
}

bool is_swp_branch(Op* op_ptr)
{
  return is_swp_branch(op_ptr->opcode());
}

bool is_call(Op* op_ptr) 
{
  return (op_ptr->opcode() == BRL);
}

bool is_return(Op* op_ptr) 
{
  return (op_ptr->opcode() == RTS);
}

bool is_movelb(Op* op_ptr)
{
  return (op_ptr->opcode() == MOVELB);
}

bool is_movelb(Opcode opc)
{
  return (opc == MOVELB);
}

bool is_movelbx(Op* op_ptr)
{
  return (op_ptr->opcode() == MOVELBX);
}

bool is_movelbx(Opcode opc)
{
  return (opc == MOVELBX);
}

bool is_movelbs(Op* op_ptr)
{
  return (op_ptr->opcode() == MOVELBS);
}

bool is_movelbs(Opcode opc)
{
  return (opc == MOVELBS);
}

bool is_pbr(Op* op_ptr)
{
  return is_pbr(op_ptr->opcode());
}

bool is_pbr(Opcode opc)
{
  if (opc == PBR) {
    return true;
  } else {
    return false;
   }
}

bool requires_pbr(Op* op_ptr)
{
   return requires_pbr(op_ptr->opcode()) ;
}

bool requires_pbr(Opcode opc)
{
   if (opc == BRDVI ||
        opc == BRDVF ||
        opc == BRU ||
        opc == BRCT ||
        opc == BRCF ||
        opc == BRL ||
        opc == BRLC ||
        opc == BRLP ||
        opc == BRDIR ||
        opc == BRIND ||
        opc == BRF_B_B_F ||
        opc == BRF_B_F_F ||
        opc == BRF_F_B_B ||
        opc == BRF_F_F_B ||
        opc == BRF_F_F_F ||
        opc == BRW_B_B_F ||
        opc == BRW_B_F_F ||
        opc == BRW_F_B_B ||
        opc == BRW_F_F_B ||
        opc == BRW_F_F_F ||
        opc == RTS) {
      return true ;
   } else {
      return false ;
   }
}

///////////////////////////////////////////////////////////////////////////////
//
//	Comparison queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_cmpr(Op* op_ptr) {
   return is_cmpr(op_ptr->opcode()) ;
}

bool is_cmpr(Opcode opc) {
   IR_ROOT_OPCODE t = get_root(opc) ;

   if ( (t == ROOT_CMPR) ||
        (t == ROOT_FCMPR))  {
      return true ;
   } else {
      return false ;
   }
}

bool is_icmpr(Op* op_ptr) {
   return is_icmpr(op_ptr->opcode()) ;
}

bool is_icmpr(Opcode opc) {
   IR_ROOT_OPCODE t = get_root(opc) ;

   if (t == ROOT_CMPR) {
      return true ;
   } else {
      return false ;
   }
}

bool is_fcmpr(Op* op_ptr) {
   return is_fcmpr(op_ptr->opcode()) ;
}

bool is_fcmpr(Opcode opc) {
   IR_ROOT_OPCODE t = get_root(opc) ;

   if (t == ROOT_FCMPR) {
      return true ;
   } else {
      return false ;
   }
}

bool is_int_eq_cmpr(Op* op_ptr)
{
  return is_int_eq_cmpr(op_ptr->opcode());
}

bool is_int_eq_cmpr(Opcode opc)
{
  if (opc == CMPR_W_EQ) {
    return true;
  } else {
    return false;
  }
}

bool is_int_neq_cmpr(Op* op_ptr)
{
  return is_int_neq_cmpr(op_ptr->opcode());
}

bool is_int_neq_cmpr(Opcode opc)
{
  if (opc == CMPR_W_NEQ) {
    return true;
  } else {
    return false;
  }
}

bool is_int_leq_cmpr(Op* op_ptr)
{
  return is_int_leq_cmpr(op_ptr->opcode());
}


bool is_int_leq_cmpr(Opcode opc)
{
  if (opc == CMPR_W_LLEQ ||
      opc == CMPR_W_LEQ) {
    return true;
  } else {
    return false;
  }
}

bool is_int_geq_cmpr(Op* op_ptr)
{
  return is_int_geq_cmpr(op_ptr->opcode());
}

bool is_int_geq_cmpr(Opcode opc)
{
  if (opc == CMPR_W_LGEQ ||
      opc == CMPR_W_GEQ) {
    return true;
  } else {
     return false;
  }
}

bool is_int_lt_cmpr(Op* op_ptr)
{
  return is_int_lt_cmpr(op_ptr->opcode());
}

bool is_int_lt_cmpr(Opcode opc)
{
  if (opc == CMPR_W_LLT ||
      opc == CMPR_W_LT) {
    return true;
  } else {
    return false;
  }
}

bool is_int_gt_cmpr(Op* op_ptr)
{
  return is_int_gt_cmpr(op_ptr->opcode());
}

bool is_int_gt_cmpr(Opcode opc)
{
  if (opc == CMPR_W_LGT ||
      opc == CMPR_W_GT) {
    return true;
  } else {
    return false;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//	Predicate comparison/setting queries
//
///////////////////////////////////////////////////////////////////////////////

Opcode get_cmpp_dest1_modifier(Opcode opc)
{
    return (Opcode) (opc & IR_DEST1_MASK);
}

Opcode get_cmpp_dest2_modifier(Opcode opc)
{
    return (Opcode) (opc & IR_DEST2_MASK);
}

bool is_cmpp(Op* op_ptr) {
   return is_cmpp(op_ptr->opcode()) ;
}

bool is_cmpp(Opcode opc) {
   IR_ROOT_OPCODE t = get_root(opc) ;

   if ( (t == ROOT_CMPP) ||
        (t == ROOT_FCMPP))  {
      return true ;
   } else {
      return false ;
   }
}

bool is_icmpp(Op* op_ptr) {
   return is_icmpp(op_ptr->opcode()) ;
}

bool is_icmpp(Opcode opc) {
   IR_ROOT_OPCODE t = get_root(opc) ;

   if (t == ROOT_CMPP) {
      return true ;
   } else {
      return false ;
   }
}

bool is_fcmpp(Op* op_ptr) {
   return is_fcmpp(op_ptr->opcode()) ;
}

bool is_fcmpp(Opcode opc) {
   IR_ROOT_OPCODE t = get_root(opc) ;

   if (t == ROOT_FCMPP) {
      return true ;
   } else {
      return false ;
   }
}

bool is_predicate_clear(Op* op_ptr) 
{
  return (is_predicate_clear(op_ptr->opcode()));
}

bool is_predicate_clear(Opcode opc)
{
    if (opc == PRED_CLEAR)
        return true;
    else
	return false;
}

bool is_predicate_clear_all_rotating(Op* op_ptr) 
{
  return (is_predicate_clear_all_rotating(op_ptr->opcode()));
}

bool is_predicate_clear_all_rotating(Opcode opc)
{
    if (opc == PRED_CLEAR_ALL_ROTATING)
        return true;
    else
	return false;
}

bool is_predicate_set(Op* op_ptr) 
{
  return (is_predicate_set(op_ptr->opcode()));
} 

bool is_predicate_set(Opcode opc)
{
    if (opc == PRED_SET)
        return true;
    else
	return false;
}

bool is_clear_all_rotating(Op* op_ptr) 
{
  return (is_clear_all_rotating(op_ptr->opcode()));
}

bool is_clear_all_rotating(Opcode opc)
{
    if (opc == PRED_CLEAR_ALL_ROTATING)
        return true;
    else
	return false;
}

bool is_U_pred(Op* op, Port_num dest)
{
  return (is_U_pred(op->opcode(), dest));
}

bool is_U_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_UN || x == IR_DEST1_UC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_UN || x == IR_DEST2_UC);
  }
  else {
    assert (0);
    return false;  // make CC happy
  }
}

bool is_UN_pred(Op* op, Port_num dest)
{
  return (is_UN_pred(op->opcode(), dest));
}

bool is_UN_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_UN);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_UN);
  }
  else {
    assert (0);
    return false;  // make CC happy
  }
}

bool is_UC_pred(Op* op, Port_num dest)
{
  return (is_UC_pred(op->opcode(), dest));
}

bool is_UC_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_UC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_UC);
  }
  else {
    assert (0);
    return false;  // make CC happy
  }
}

bool is_C_pred(Op* op, Port_num dest)
{
  return (is_C_pred(op->opcode(), dest));
}

bool is_C_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_CN || x == IR_DEST1_CC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_CN || x == IR_DEST2_CC);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_CN_pred(Op* op, Port_num dest)
{
  return (is_CN_pred(op->opcode(), dest));
}

bool is_CN_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_CN);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_CN);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_CC_pred(Op* op, Port_num dest)
{
  return (is_CC_pred(op->opcode(), dest));
}

bool is_CC_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_CC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_CC);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_O_pred(Op* op, Port_num dest)
{
  return (is_O_pred(op->opcode(), dest));
}

bool is_O_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_ON || x == IR_DEST1_OC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_ON || x == IR_DEST2_OC);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_ON_pred(Op* op, Port_num dest)
{
  return is_ON_pred(op->opcode(), dest);
}

bool is_ON_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_ON);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_ON);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_OC_pred(Op* op, Port_num dest)
{
  return is_OC_pred(op->opcode(), dest);
}

bool is_OC_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_OC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_OC);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_A_pred(Op* op, Port_num dest)
{
  return is_A_pred(op->opcode(), dest);
}

bool is_A_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_AN || x == IR_DEST1_AC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_AN || x == IR_DEST2_AC);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_AN_pred(Op* op, Port_num dest)
{
  return is_AN_pred(op->opcode(), dest);
}

bool is_AN_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_AN);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_AN);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_AC_pred(Op* op, Port_num dest)
{
  return is_AC_pred(op->opcode(), dest);
}

bool is_AC_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    int x = opc & IR_DEST1_MASK;
    return (x == IR_DEST1_AC);
  }
  else if (dest == DEST2) {
    int x = opc & IR_DEST2_MASK;
    return (x == IR_DEST2_AC);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

bool is_normal_pred(Op* op, Port_num dest)
{
  return is_normal_pred(op->opcode(), dest);
}

bool is_normal_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  return (!is_complement_pred(opc, dest));
}

bool is_complement_pred(Op* op, Port_num dest)
{
  return is_complement_pred(op->opcode(), dest);
}

bool is_complement_pred(Opcode opc, Port_num dest)
{
  if (!is_cmpp(opc)) return false;

  if (dest == DEST1) {
    return (opc & IR_DEST1_CBIT_MASK);
  }
  else if (dest == DEST2) {
    return (opc & IR_DEST2_CBIT_MASK);
  }
  else {
    assert (0);
    return false; // make CC happy
  }
}

Opcode cmpp_dest1_to_dest2(Opcode opc)
{ 
  assert(is_cmpp(opc));
  Opcode new_opc = (Opcode)(opc & IR_DEST1_MASK);
  return (Opcode)(new_opc << 4);
}

Opcode cmpp_dest2_to_dest1(Opcode opc)
{
  assert(is_cmpp(opc));
  Opcode new_opc = (Opcode)(opc & IR_DEST1_MASK);
  return (Opcode)(new_opc >> 4);
}

Opcode make_complement_pred(Op* op, Port_num dest)
{
  return make_complement_pred(op->opcode(), dest);
}

Opcode make_complement_pred(Opcode opc, Port_num dest)
{
  assert (is_cmpp (opc));
  if (dest == DEST1) {
    if(opc & IR_DEST1_CBIT_MASK)
      {
	return (Opcode)(opc & ~IR_DEST1_CBIT_MASK);
      }
    else
      {
	return (Opcode)(opc | IR_DEST1_CBIT_MASK);
      }
  }
  else if (dest == DEST2) {
    if(opc & IR_DEST2_CBIT_MASK)
      {
	return (Opcode)(opc & ~IR_DEST2_CBIT_MASK);
      }
    else
      {
	return (Opcode)(opc | IR_DEST2_CBIT_MASK);
      }
  }
  El_punt("make_complement_pred: illegal value for dest");
  return ((Opcode) 0);
}

/*
 *  This function returns the opcode to be used when we want to make the
 *  destination predicate UN type.
 */

Opcode make_UN_pred(Opcode opc, Port_num dest)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      if (dest == DEST1)
	{
	  new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
	  new_opc = (Opcode) (new_opc | IR_DEST1_UN);
	}
      else if (dest == DEST2)
	{
	  new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
	  new_opc = (Opcode) (new_opc | IR_DEST2_UN);
	}
      return new_opc;
    }

    default:
      return (Opcode)0;
    }

  El_punt("make_UN_pred: unknown opc");
  return (Opcode)0;  
  
}

/*
 *  This function returns the opcode to be used when we want to make the
 *  destination predicate UC type.
 */

Opcode make_UC_pred(Opcode opc, Port_num dest)
{
  Opcode new_opc = opc;

  IR_ROOT_OPCODE root = get_root(opc);

  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      if (dest == DEST1)
        {
          new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
          new_opc = (Opcode) (new_opc | IR_DEST1_UC);
        }
      else if (dest == DEST2)
        {
          new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
          new_opc = (Opcode) (new_opc | IR_DEST2_UC);
        }
      return new_opc;
    }

    default:
      return (Opcode)0;
    }

  El_punt("make_UC_pred: unknown opc");
  return (Opcode)0;

}


/*
 *  This function returns the opcode to be used when we want to make the
 *  destination predicate ON type.
 */

Opcode make_ON_pred(Opcode opc, Port_num dest)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      if (dest == DEST1)
	{
	  new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
	  new_opc = (Opcode) (new_opc | IR_DEST1_ON);
	}
      else if (dest == DEST2)
	{
	  new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
	  new_opc = (Opcode) (new_opc | IR_DEST2_ON);
	}
      return new_opc;
    }

    default:
      return (Opcode)0;
    }

  El_punt("make_ON_pred: unknown opc");
  return (Opcode)0;  
  
}

/*
 *  This function returns the opcode to be used when we want to make the
 *  destination predicate OC type.
 */

Opcode make_OC_pred(Opcode opc, Port_num dest)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      if (dest == DEST1)
	{
	  new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
	  new_opc = (Opcode) (new_opc | IR_DEST1_OC);
	}
      else if (dest == DEST2)
	{
	  new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
	  new_opc = (Opcode) (new_opc | IR_DEST2_OC);
	}
      return new_opc;
    }

    default:
      return (Opcode)0;
    }

  El_punt("make_OC_pred: unknown opc");
  return (Opcode)0;  
  
}

/*
 *  This function returns the opcode to be used when we want to make the
 *  destination predicate OC type.
 */

Opcode make_AN_pred(Opcode opc, Port_num dest)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      if (dest == DEST1)
        {
          new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
          new_opc = (Opcode) (new_opc | IR_DEST1_AN);
        }
      else if (dest == DEST2)
        {
          new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
          new_opc = (Opcode) (new_opc | IR_DEST2_AN);
        }
      return new_opc;
    }

    default:
      return (Opcode)0;
    }

  El_punt("make_AN_pred: unknown opc");
  return (Opcode)0;

}

/*
 *  This function returns the opcode to be used when we want to make the
 *  destination predicate OC type.
 */

Opcode make_AC_pred(Opcode opc, Port_num dest)
{
  Opcode new_opc = opc;

  IR_ROOT_OPCODE root = get_root(opc);

  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      if (dest == DEST1)
        {
          new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
          new_opc = (Opcode) (new_opc | IR_DEST1_AC);
        }
      else if (dest == DEST2)
        {
          new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
          new_opc = (Opcode) (new_opc | IR_DEST2_AC);
        }
      return new_opc;
    }

    default:
      return (Opcode)0;
    }

  El_punt("make_AC_pred: unknown opc");
  return (Opcode)0;

}


/* 
** Changes an or type predicate to an unconditional type predicate
*/

Opcode make_U_pred(Opcode opc, Port_num dest)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      if (dest == DEST1)
	{
	  if(is_ON_pred(opc, DEST1))
	    {
	      new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
	      new_opc = (Opcode) (new_opc | IR_DEST1_UN);
	    }
	  if(is_OC_pred(opc, DEST1))
	    {
	      new_opc = (Opcode) (opc & ~IR_DEST1_MASK);
	      new_opc = (Opcode) (new_opc | IR_DEST1_UC);
	    }
	}
      else if (dest == DEST2)
	{	
	  if(is_ON_pred(opc, DEST2))
	    {
	      new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
	      new_opc = (Opcode) (new_opc | IR_DEST2_UN);
	    }
	  if(is_OC_pred(opc, DEST2))
	    {
	      new_opc = (Opcode) (opc & ~IR_DEST2_MASK);
	      new_opc = (Opcode) (new_opc | IR_DEST2_UC);
	    }
	}
      return new_opc;
    }

    default:
      return (Opcode)0;
    }

  El_punt("Something is SERIOUSLY wrong.  The world is coming to an end.");
  return (Opcode)0;  
  
}


/*
** This function makes any compare instruction into an OR type CMPP.
** The first dest in ON and the second dest is OC.
*/

Opcode make_O_cmpp(Opcode opc)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_CMPP:
    case ROOT_FCMPP:
    {
      new_opc = make_ON_pred(opc, DEST1);
      new_opc = make_OC_pred(new_opc, DEST2); 
    }

    case ROOT_CMPR:
    {
      if(opc == CMPR_W_EQ) new_opc = (Opcode) BASE_CMPP_W_EQ;
      if(opc == CMPR_W_NEQ) new_opc = (Opcode) BASE_CMPP_W_NEQ;
      
      if(opc == CMPR_W_LT) new_opc = (Opcode) BASE_CMPP_W_LT;
      if(opc == CMPR_W_GT) new_opc = (Opcode) BASE_CMPP_W_GT;
      
      if(opc == CMPR_W_LEQ) new_opc = (Opcode) BASE_CMPP_W_LEQ;
      if(opc == CMPR_W_GEQ) new_opc = (Opcode) BASE_CMPP_W_GEQ;
      
      new_opc = make_ON_pred(opc, DEST1);
      new_opc = make_OC_pred(new_opc, DEST2);
    }

    case ROOT_FCMPR:
    {
      if(opc == FCMPR_S_EQ) new_opc = (Opcode) BASE_FCMPP_S_EQ;
      if(opc == FCMPR_S_NEQ) new_opc = (Opcode) BASE_FCMPP_S_NEQ;
      
      if(opc == FCMPR_S_LT) new_opc = (Opcode) BASE_FCMPP_S_LT;
      if(opc == FCMPR_S_GT) new_opc = (Opcode) BASE_FCMPP_S_GT;
      
      if(opc == FCMPR_S_LEQ) new_opc = (Opcode) BASE_FCMPP_S_LEQ;
      if(opc == FCMPR_S_GEQ) new_opc = (Opcode) BASE_FCMPP_S_GEQ;

      if(opc == FCMPR_D_EQ) new_opc = (Opcode) BASE_FCMPP_D_EQ;
      if(opc == FCMPR_D_NEQ) new_opc = (Opcode) BASE_FCMPP_D_NEQ;
      
      if(opc == FCMPR_D_LT) new_opc = (Opcode) BASE_FCMPP_D_LT;
      if(opc == FCMPR_D_GT) new_opc = (Opcode) BASE_FCMPP_D_GT;
      
      if(opc == FCMPR_D_LEQ) new_opc = (Opcode) BASE_FCMPP_D_LEQ; 
      if(opc == FCMPR_D_GEQ) new_opc = (Opcode) BASE_FCMPP_D_GEQ;

      new_opc = make_ON_pred(opc, DEST1);
      new_opc = make_OC_pred(new_opc, DEST2);
      
    }

    default:
      return (Opcode)0;
    }

  El_punt("Something is SERIOUSLY wrong.  The world is coming to an end.");
  return (Opcode)0;  
  
}

bool is_int_leq_cmpp(Op* op_ptr)
{
  return is_int_leq_cmpp(op_ptr->opcode());
}

bool is_int_leq_cmpp(Opcode opc)
{
  IR_BASE_OPCODE base_opc = get_base(opc);
  if (base_opc == BASE_CMPP_W_LLEQ ||
      base_opc == BASE_CMPP_W_LEQ) {
    return true;
  } else {
    return false;
  }
}

bool is_int_geq_cmpp(Op* op_ptr)
{
  return is_int_geq_cmpp(op_ptr->opcode());
}

bool is_int_geq_cmpp(Opcode opc)
{
  IR_BASE_OPCODE base_opc = get_base(opc);
  if (base_opc == BASE_CMPP_W_LGEQ ||
      base_opc == BASE_CMPP_W_GEQ) {
    return true;
  } else {
    return false;
  }
}

bool is_int_lt_cmpp(Op* op_ptr)
{
  return is_int_lt_cmpp(op_ptr->opcode());
}

bool is_int_lt_cmpp(Opcode opc)
{
  IR_BASE_OPCODE base_opc = get_base(opc);
  if (base_opc == BASE_CMPP_W_LLT ||
      base_opc == BASE_CMPP_W_LT) {
    return true;
  } else {
    return false;
  }
}

bool is_int_gt_cmpp(Op* op_ptr)
{
  return is_int_gt_cmpp(op_ptr->opcode());
}

bool is_int_gt_cmpp(Opcode opc)
{
  IR_BASE_OPCODE base_opc = get_base(opc);
  if (base_opc == BASE_CMPP_W_LGT ||
      base_opc == BASE_CMPP_W_GT) {
    return true;
  } else {
    return false;
  }
}

bool is_int_neq_cmpp(Op* op_ptr)
{
  return is_int_neq_cmpp(op_ptr->opcode());
}

bool is_int_neq_cmpp(Opcode opc)
{
  IR_BASE_OPCODE base_opc = get_base(opc);
  if (base_opc == BASE_CMPP_W_NEQ) {
    return true;
  } else {
    return false;
  }
}

bool is_int_eq_cmpp(Op* op_ptr)
{
  return is_int_eq_cmpp(op_ptr->opcode());
}

bool is_int_eq_cmpp(Opcode opc)
{
  IR_BASE_OPCODE base_opc = get_base(opc);
  if (base_opc == BASE_CMPP_W_EQ) {
    return true;
  } else {
    return false;
  }
}

bool is_conditionally_executed(Op* op_ptr)
{
   if (!op_ptr->predicated()) return false ;
   for (Op_predicate_sources ii(op_ptr) ; ii != 0 ; ii++) {
      Operand& oprnd = *ii ;
      if (!oprnd.is_predicate_true()) return true ;
   }
   return false ;
}

Opcode exchange_cmpp_dest_modifiers(Opcode opc)
{
    Opcode mod1, mod2, new_mod1 = (Opcode)0, new_mod2 = (Opcode)0, new_opc;

    mod1 = get_cmpp_dest1_modifier(opc);
    mod2 = get_cmpp_dest2_modifier(opc);

    /* Figure out new modifier for dest2 */
    switch ((IR_MODIFIERS) mod1) {
	case IR_DEST1_UN:
	    new_mod2 = (Opcode) IR_DEST2_UN;
	    break;
	case IR_DEST1_UC:
	    new_mod2 = (Opcode) IR_DEST2_UC;
	    break;
	case IR_DEST1_CN:
	    new_mod2 = (Opcode) IR_DEST2_CN;
	    break;
	case IR_DEST1_CC:
	    new_mod2 = (Opcode) IR_DEST2_CC;
	    break;
	case IR_DEST1_ON:
	    new_mod2 = (Opcode) IR_DEST2_ON;
	    break;
	case IR_DEST1_OC:
	    new_mod2 = (Opcode) IR_DEST2_OC;
	    break;
	case IR_DEST1_AN:
	    new_mod2 = (Opcode) IR_DEST2_AN;
	    break;
	case IR_DEST1_AC:
	    new_mod2 = (Opcode) IR_DEST2_AC;
	    break;
	default:
	    El_punt("exchange_cmpp_dest_modifiers: unknown dest1 modifier!");
    }

    /* Figure out new modifier for dest1 */
    switch ((IR_MODIFIERS) mod2) {
        case IR_DEST2_UN:
            new_mod1 = (Opcode) IR_DEST1_UN;
            break;
        case IR_DEST2_UC:
            new_mod1 = (Opcode) IR_DEST1_UC;
            break;
        case IR_DEST2_CN:
            new_mod1 = (Opcode) IR_DEST1_CN;
            break;
        case IR_DEST2_CC:
            new_mod1 = (Opcode) IR_DEST1_CC;
            break;
        case IR_DEST2_ON:
            new_mod1 = (Opcode) IR_DEST1_ON;
            break;
        case IR_DEST2_OC:
            new_mod1 = (Opcode) IR_DEST1_OC;
            break;
        case IR_DEST2_AN:
            new_mod1 = (Opcode) IR_DEST1_AN;
            break;
        case IR_DEST2_AC:
            new_mod1 = (Opcode) IR_DEST1_AC;
            break;
        default:
            El_punt("exchange_cmpp_dest_modifiers: unknown dest2 modifier!");
    }

    /* Construct the new opcode */
    new_opc = opc;
    new_opc = (Opcode) (new_opc & (~IR_DEST1_MASK));
    new_opc = (Opcode) (new_opc & (~IR_DEST2_MASK));
    new_opc = (Opcode) (new_opc | new_mod1);
    new_opc = (Opcode) (new_opc | new_mod2);

    return (new_opc);
}


///////////////////////////////////////////////////////////////////////////////
//
//	Load/Store queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_postincrement(Opcode opc)
{
    if (opc & IR_INCREMENT)
	return (true);
    else
	return (false);
}

bool is_postincrement(Op* op_ptr)
{
   return is_postincrement(op_ptr->opcode());
}

Opcode get_mem_src_modifier(Opcode opc)
{
    return (Opcode) (opc & (IR_SRC_C1|IR_SRC_C2|IR_SRC_C3|IR_SRC_V1));
}

Opcode get_mem_dest_modifier(Opcode opc)
{
    return (Opcode) (opc & (IR_DEST_C1|IR_DEST_C2|IR_DEST_C3|IR_DEST_V1));
}

Opcode get_local_mem_modifier(Opcode opc)
{
    return (Opcode) (opc & IR_LM_MASK);
}


// Load ops that differ only in DEST field share same MDES entry
// Load ops are therefore translated to a base load with DEST set to C1
Opcode get_C1_dest_load(Opcode opc)
{
  assert (is_load(opc));
  IR_ROOT_OPCODE t = get_root(opc) ;
  if (t == ROOT_DVLD || t == ROOT_FDVLD) return opc;

  if (t == ROOT_LD || t == ROOT_FLD || 
      t == ROOT_DSLD || t == ROOT_FDSLD ||
      // SLARSEN: Vector loads
      t == ROOT_VLD || t == ROOT_VFLD ||
      t == ROOT_VLDE || t == ROOT_VFLDE)
    // opc_XX_C1
    return ((Opcode) ((opc & (~IR_DEST_MASK)) | IR_DEST_C1));
  
  // tmp. hack: new loads only with opc_C1_C1  -- Marnix.
  return ((Opcode) ((opc & (~IR_SRC_MASK | ~IR_DEST_MASK)) |
		     (IR_SRC_C1 | IR_DEST_C1)) );
}

bool is_load(Op* op_ptr)
{
  return is_load(op_ptr->opcode()) ;
}

bool is_load(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_LD || 
	  t == ROOT_FLD || 
	  t == ROOT_DSLD || 
	  t == ROOT_FDSLD ||
	  t == ROOT_DVLD || 
	  t == ROOT_FDVLD ||
	  t == ROOT_LDX ||
	  t == ROOT_LG ||
	  t == ROOT_LGX ||
	  t == ROOT_FLG ||
	  t == ROOT_RESTORE ||      // Restore/unspill ops for GPR & FPR
	  t == ROOT_FRESTORE ||
	  t == ROOT_BRESTORE ||
	  t == ROOT_PRESTORE ||
	  t == ROOT_RESTOREG ||      // Restore/unspill ops for GPR & FPR
	  t == ROOT_FRESTOREG ||
	  t == ROOT_BRESTOREG ||
	  t == ROOT_PRESTOREG ||
	  t == ROOT_LL ||	// SAM 10-98, add local mem opcodes
	  t == ROOT_LLX ||
	  t == ROOT_FLL ||
	  t == ROOT_LLG ||
	  t == ROOT_LLGX ||
	  t == ROOT_FLLG ||
	  // SLARSEN: Vector loads
	  t == ROOT_VLD ||
	  t == ROOT_VFLD ||
	  t == ROOT_VLG ||
	  t == ROOT_VFLG ||
	  t == ROOT_VLDE ||
	  t == ROOT_VFLDE ||
	  t == ROOT_VLGE ||
	  t == ROOT_VFLGE);
}

bool is_sign_extend_load(Op *op_ptr)
{
  return is_sign_extend_load(op_ptr->opcode()) ;
}

bool is_sign_extend_load(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_LDX ||
	  t == ROOT_LGX ||
	  t == ROOT_LLX ||	// SAM 10-98, add local mem opcodes
	  t == ROOT_LLGX);
}

bool is_base_plus_offset_load(Op *op_ptr)
{
    return is_base_plus_offset_load(op_ptr->opcode()) ;
}

bool is_base_plus_offset_load(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_LG ||
	  t == ROOT_LGX ||
	  t == ROOT_FLG ||
	  t == ROOT_LLG ||
	  t == ROOT_RESTOREG ||      // Restore/unspill ops for GPR & FPR
	  t == ROOT_FRESTOREG ||
	  t == ROOT_BRESTOREG ||
	  t == ROOT_PRESTOREG ||
	  t == ROOT_LLGX ||
	  t == ROOT_FLLG);
}

bool is_load_from_C1(Op* op_ptr)
{
  return is_load_from_C1(op_ptr->opcode()) ;
}

bool is_load_from_C1(Opcode opc)
{
  if (!is_load(opc)) return (false);
  return ((opc & IR_SRC_MASK) == IR_SRC_C1);
}

bool is_store(Op* op_ptr)
{
  return is_store(op_ptr->opcode()) ;
}

bool is_store(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_ST || 
	  t == ROOT_FST ||
	  t == ROOT_SG ||
	  t == ROOT_FSG ||
	  t == ROOT_SAVE ||       // To save/spill GPR, FPR and BTR
	  t == ROOT_FSAVE ||
	  t == ROOT_BSAVE ||
	  t == ROOT_PSAVE ||
	  t == ROOT_SAVEG ||       // To save/spill GPR, FPR and BTR
	  t == ROOT_FSAVEG ||
	  t == ROOT_BSAVEG ||
	  t == ROOT_PSAVEG ||
	  t == ROOT_SL ||	// SAM 10-98, add local mem opcodes
	  t == ROOT_FSL ||
	  t == ROOT_SLG ||
	  t == ROOT_FSLG ||
	  // SLARSEN: Vector stores
	  t == ROOT_VST ||
	  t == ROOT_VFST ||
	  t == ROOT_VSG ||
	  t == ROOT_VFSG ||
	  t == ROOT_VSTE ||
	  t == ROOT_VFSTE ||
	  t == ROOT_VSGE ||
	  t == ROOT_VFSGE);
}

bool is_base_plus_offset_store(Op *op_ptr)
{
    return is_base_plus_offset_store(op_ptr->opcode()) ;
}

bool is_base_plus_offset_store(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_SG ||
	  t == ROOT_FSG ||
	  t == ROOT_SLG ||
	  t == ROOT_FSLG ||
	  t == ROOT_SAVEG ||       // To save/spill GPR, FPR and BTR
	  t == ROOT_FSAVEG ||
	  t == ROOT_BSAVEG ||
	  t == ROOT_PSAVEG);
}

bool is_memory(Op* op_ptr)
{
   return is_memory(op_ptr->opcode()) ;
}

bool is_memory(Opcode opc)
{
  // <fbouchez>
  if (opc == S_PUSH ||
      opc == S_POP ||
      opc == S_PEEK) {
    return true;
  } else {
  // </fbouchez>
   return(is_load(opc) || is_store(opc)) ;
  }
}

bool is_mem_base_plus_offset(Op *op_ptr)
{
  return is_mem_base_plus_offset(op_ptr->opcode());
}

bool is_mem_base_plus_offset(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_LG  ||
	  t == ROOT_LGX ||
	  t == ROOT_FLG ||
	  t == ROOT_RESTOREG ||      // Restore/unspill ops for GPR & FPR
	  t == ROOT_FRESTOREG ||
	  t == ROOT_BRESTOREG ||
	  t == ROOT_PRESTOREG ||
	  t == ROOT_SAVEG ||      // Restore/unspill ops for GPR & FPR
	  t == ROOT_FSAVEG ||
	  t == ROOT_BSAVEG ||
	  t == ROOT_PSAVEG ||
	  t == ROOT_SG  ||
	  t == ROOT_FSG ||
	  t == ROOT_LLG ||	// SAM 10-98, add local mem opcodes
	  t == ROOT_LLGX ||
	  t == ROOT_FLLG ||
	  t == ROOT_SLG ||
	  t == ROOT_FSLG);
}

bool is_memref(Op* op_ptr)
{
   return is_memref(op_ptr->opcode()) ;
}

bool is_memref(Opcode opc)
{
   return(is_load(opc) || is_store(opc) || is_brl(opc) || is_rts(opc)) ;
}

bool is_save(Opcode opc)
{
    return (opc == (Opcode) SAVE     || opc == (Opcode) SAVEG ||
            opc == (Opcode) FSAVE_S  || opc == (Opcode) FSAVE_D ||
            opc == (Opcode) FSAVEG_S || opc == (Opcode) FSAVEG_D ||
            opc == (Opcode) BSAVE    || opc == (Opcode) BSAVEG || 
            opc == (Opcode) PSAVE    || opc == (Opcode) PSAVEG) ;
}

bool is_save(Op *op_ptr)
{
    return (is_save(op_ptr->opcode()));
}

bool is_restore(Opcode opc)
{
    return (opc == (Opcode) RESTORE     || opc == (Opcode) RESTOREG ||
            opc == (Opcode) FRESTORE_S  || opc == (Opcode) FRESTORE_D ||
            opc == (Opcode) FRESTOREG_S || opc == (Opcode) FRESTOREG_D ||
            opc == (Opcode) BRESTORE    || opc == (Opcode) BRESTOREG || 
            opc == (Opcode) PRESTORE    || opc == (Opcode) PRESTOREG) ;
}

bool is_restore(Op *op_ptr)
{
    return (is_restore(op_ptr->opcode()));
}

bool is_gpr_save_restore(Opcode opc)
{
    return (opc == (Opcode) SAVE || opc == (Opcode) RESTORE ||
	    opc == (Opcode) SAVEG || opc == (Opcode) RESTOREG);
}

bool is_gpr_save_restore(Op *op_ptr)
{
    return (is_gpr_save_restore(op_ptr->opcode()));
}

bool is_fpr_save_restore(Opcode opc)
{
    return (opc == (Opcode) FSAVE_S || opc == (Opcode) FRESTORE_S ||
	    opc == (Opcode) FSAVE_D || opc == (Opcode) FRESTORE_D ||
	    opc == (Opcode) FSAVEG_S || opc == (Opcode) FRESTOREG_S ||
	    opc == (Opcode) FSAVEG_D || opc == (Opcode) FRESTOREG_D);
}

bool is_fpr_save_restore(Op *op_ptr)
{
    return (is_fpr_save_restore(op_ptr->opcode()));
}

bool is_btr_save_restore(Opcode opc)
{
    return (opc == (Opcode) BSAVE || opc == (Opcode) BRESTORE ||
	    opc == (Opcode) BSAVEG || opc == (Opcode) BRESTOREG);
}

bool is_btr_save_restore(Op *op_ptr)
{
    return (is_btr_save_restore(op_ptr->opcode()));
}

bool is_pr_save_restore(Opcode opc)
{
    return (opc == (Opcode) PSAVE || opc == (Opcode) PRESTORE ||
	    opc == (Opcode) PSAVEG || opc == (Opcode) PRESTOREG);
}

bool is_pr_save_restore(Op *op_ptr)
{
    return (is_pr_save_restore(op_ptr->opcode()));
}

bool is_single_fpr_save_restore(Op *op_ptr)
{
    Opcode opc;
    Operand operand;

    opc = op_ptr->opcode();
    if (! is_fpr_save_restore(opc))
       return (false);

    if (opc == (Opcode) FSAVE_S) {
	operand = op_ptr->src(SRC2);
    }
    else {
	operand = op_ptr->dest(DEST1);
    }

    if (operand.dtype().is_float())
        return (true);
    else
	return (false);
}

bool is_save_restore(Opcode opc)
{
    return (is_gpr_save_restore(opc) ||
            is_fpr_save_restore(opc) ||
            is_pr_save_restore(opc) ||
            is_btr_save_restore(opc));
}

bool is_save_restore(Op *op_ptr)
{
    return (is_gpr_save_restore(op_ptr) ||
            is_fpr_save_restore(op_ptr) ||
            is_pr_save_restore(op_ptr) ||
	    is_btr_save_restore(op_ptr));
}

Opcode remove_post_increment(Opcode opc)
{
    Opcode new_opc;

    if (! is_postincrement(opc))
	El_punt("remove_post_increment: opc is not a post increment");

    new_opc = opc;
    new_opc = (Opcode) (new_opc & (~IR_INCREMENT));

    return (new_opc);
}

Opcode extract_increment_from_post_increment(Opcode opc)
{
    if (! is_postincrement(opc))
	El_punt("extract_increment_from_post_increment: opc is not a post inc");

    return (ADD_W);
}

bool is_pred_load_all(Op* op_ptr)
{
  return is_pred_load_all(op_ptr->opcode()) ;
}

bool is_pred_load_all(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_PRED_LOAD_ALL);
}

bool is_pred_store_all(Op* op_ptr)
{
  return is_pred_store_all(op_ptr->opcode()) ;
}

bool is_pred_store_all(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_PRED_STORE_ALL);
}

bool is_local_mem_load(Op *op_ptr)
{
  return is_local_mem_load(op_ptr->opcode());
}

bool is_local_mem_load(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_LL ||
	  t == ROOT_LLX ||
	  t == ROOT_FLL ||
	  t == ROOT_LLG ||
	  t == ROOT_LLGX ||
	  t == ROOT_FLLG);
}

bool is_local_mem_store(Op *op_ptr)
{
  return is_local_mem_store(op_ptr->opcode());
}

bool is_local_mem_store(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_SL ||
	  t == ROOT_FSL ||
	  t == ROOT_SLG ||
	  t == ROOT_FSLG);
}

bool is_local_mem(Op *op_ptr)
{
    return is_local_mem(op_ptr->opcode());
}

bool is_local_mem(Opcode opc)
{
    return (is_local_mem_load(opc) || is_local_mem_store(opc));
}


///////////////////////////////////////////////////////////////////////////////
//
//	Arithmetic queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_no_op(Op* op_ptr)
{
  return is_no_op(op_ptr->opcode());
}

bool is_no_op(Opcode opc)
{
  return (opc == NO_OP || opc == M_NO_OP);
}

bool is_or(Op* op_ptr)
{
   return is_or(op_ptr->opcode()) ;
}

bool is_or(Opcode opc)
{
   return(opc == OR_W) ;
}

bool is_and(Op* op_ptr)
{
   return is_and(op_ptr->opcode()) ;
}

bool is_and(Opcode opc)
{
   return(opc == AND_W) ;
}

bool is_xor(Op* op_ptr)
{
   return is_xor(op_ptr->opcode()) ;
}

bool is_xor(Opcode opc)
{
   return(opc == XOR_W) ;
}

bool is_abs(Op* op_ptr)
{
   return is_abs(op_ptr->opcode()) ;
}

bool is_abs(Opcode opc)
{
   return(opc == ABS_W) ;
}

bool is_move(Op* op_ptr)
{
   return is_move(op_ptr->opcode()) ;
}

bool is_move(Opcode opc)
{
  if ((opc == MOVE ||
       opc == MOVEGF_L ||
       opc == MOVEGF_U ||
       opc == MOVEF_S ||
       opc == MOVEF_D ||
       opc == MOVEFG_L ||
       opc == MOVEFG_U ||
       opc == MOVEPG ||
       opc == MOVEGP ||
       opc == MOVEGG ||
       opc == MOVEGC ||
       opc == MOVECG ||
       opc == MOVEBB ||
       opc == MOVEPP ||
       opc == MOVELG ||
       opc == MOVELGS ||
       opc == MOVELGX ||
       opc == MOVELF ||
       opc == MOVELFS ||
       opc == MOVELB ||
       opc == MOVELBS ||
       opc == MOVELBX ||
       opc == MOVEGBP ||    // To restore predicate and speculative tag bits 
       opc == MOVEGCM ||
       opc == MOVEGBGT ||
       opc == MOVEGBFT ||
       opc == MOVEGBPT ||
       opc == MOVEGBBT ||
       opc == VMOVE ||
       opc == VMOVEF_S ||
       opc == VMOVEF_D ||
       opc == VMOVEGG ||
       opc == VMOVEFF_S ||
       opc == VMOVEFF_D ||
       opc == VMOVEMM
       )
      || is_pbr(opc)
      || is_svmove(opc)
      || is_vsmove(opc)) {
    return true ;
  }
  else {
    return false ;
  }
}

bool is_movepp(Op* op_ptr)
{
   return is_movepp(op_ptr->opcode()) ;
}

bool is_movepp(Opcode opc)
{
  if (opc == MOVEPP)
    return (true);
  else
    return (false);
}

/* RMR { */
bool is_vector(Op* op_ptr)
{
  return is_vector(op_ptr->opcode());
}

bool is_vector(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return ((t <= ROOT_VFSPLAT) && (t >= ROOT_VADD));
}

bool is_vector_ialu(Op* op_ptr)
{
  return is_vector_ialu(op_ptr->opcode());
}

bool is_vector_ialu(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_VADD ||
	  t == ROOT_VADDL ||
	  t == ROOT_VSUB ||
	  t == ROOT_VSUBL ||
	  t == ROOT_VMPY ||
	  t == ROOT_VMPYL ||
	  t == ROOT_VDIV ||
	  t == ROOT_VDIVL ||
	  t == ROOT_VREM ||
	  t == ROOT_VREML ||
	  t == ROOT_VMIN ||
	  t == ROOT_VMAX ||
	  t == ROOT_VOR ||
	  t == ROOT_VAND ||
	  t == ROOT_VMOVE ||
	  t == ROOT_VEXTS ||
	  t == ROOT_VCMPP ||
	  t == ROOT_VSPLAT);
}

bool is_vector_falu(Op* op_ptr)
{
  return is_vector_falu(op_ptr->opcode());
}

bool is_vector_falu(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_VFADD ||
	  t == ROOT_VFSUB ||
	  t == ROOT_VFMPY ||
	  t == ROOT_VFDIV ||
	  t == ROOT_VFMAX ||
	  t == ROOT_VFMIN ||
	  t == ROOT_VFABS ||
	  t == ROOT_VFSQRT ||
	  t == ROOT_VCONVWS ||
	  t == ROOT_VCONVWD ||
	  t == ROOT_VCONVSW ||
	  t == ROOT_VCONVDW ||
	  t == ROOT_VCONVSD ||
	  t == ROOT_VCONVDS ||
	  t == ROOT_VMOVEF ||
	  t == ROOT_VFCMPP ||
	  t == ROOT_VFSPLAT);
}

bool is_vector_load(Op* op_ptr)
{
  return is_vector_load(op_ptr->opcode());
}

bool is_vector_load(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_VLD ||
	  t == ROOT_VFLD ||
	  t == ROOT_VLG ||
	  t == ROOT_VFLG ||
	  t == ROOT_VLDE ||
	  t == ROOT_VFLDE ||
	  t == ROOT_VLGE ||
	  t == ROOT_VFLGE);
}

bool is_vector_store(Op* op_ptr)
{
  return is_vector_store(op_ptr->opcode());
}

bool is_vector_store(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_VST ||
	  t == ROOT_VFST ||
	  t == ROOT_VSG ||
	  t == ROOT_VFSG ||
	  t == ROOT_VSTE ||
	  t == ROOT_VFSTE ||
	  t == ROOT_VSGE ||
	  t == ROOT_VFSGE);
}

bool is_vector_perm(Op* op_ptr)
{
  return is_vector_perm(op_ptr->opcode());
}

bool is_vector_perm(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc);
  return (t == ROOT_VPERM || t == ROOT_VFPERM);
}

bool is_svmove(Op* op_ptr)
{
  return is_svmove(op_ptr->opcode()) ;
}

bool is_svmove(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return ((t == ROOT_SVMOVE) || (t == ROOT_SVFMOVE));
}

bool is_vsmove(Op* op_ptr)
{
  return is_vsmove(op_ptr->opcode()) ;
}

bool is_vsmove(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return ((t == ROOT_VSMOVE) || (t == ROOT_VSFMOVE));
}
/* } RMR */

bool is_int_add(Op* op_ptr)
{
   return is_int_add(op_ptr->opcode()) ;
}

bool is_int_add(Opcode opc)
{
   return (opc == ADD_W ||
           opc == ADDL_W);
}

bool is_int_sub(Op* op_ptr)
{
   return is_int_sub(op_ptr->opcode()) ;
}

bool is_int_sub(Opcode opc)
{
   return (opc == SUB_W ||
           opc == SUBL_W);
}

bool is_int_mul(Op* op_ptr)
{
   return is_int_mul(op_ptr->opcode()) ;
}

bool is_int_mul(Opcode opc)
{
   return (opc == MPY_W ||
           opc == MPYL_W);
}

bool is_int_muladd(Op* op_ptr)
{
   return is_int_muladd(op_ptr->opcode()) ;
}

bool is_int_muladd(Opcode opc)
{
   return (opc == MPYADD_W ||
           opc == MPYADDN_W);
}

bool is_int_mulsub(Op* op_ptr)
{
   return is_int_mulsub(op_ptr->opcode()) ;
}

bool is_int_mulsub(Opcode opc)
{
   return (opc == MPYSUB_W ||
           opc == MPYRSUB_W);
}

bool is_int_div(Op* op_ptr)
{
   return is_int_div(op_ptr->opcode()) ;
}

bool is_int_div(Opcode opc)
{
   return (opc == DIV_W ||
           opc == DIVL_W);
}

bool is_int_rem(Op* op_ptr)
{
   return is_int_rem(op_ptr->opcode()) ;
}

bool is_int_rem(Opcode opc)
{
   return (opc == REM_W ||
           opc == REML_W);
}

bool is_int_lshift(Op* op_ptr)
{
   return is_int_lshift(op_ptr->opcode()) ;
}

bool is_int_lshift(Opcode opc)
{
   return (opc == SHL_W ||
           opc == SHLA_W);
}

bool is_int_rshift(Op* op_ptr)
{
   return is_int_rshift(op_ptr->opcode()) ;
}

bool is_int_rshift(Opcode opc)
{
   return (opc == SHR_W ||
           opc == SHRA_W);
}

bool is_signed_extract(Op* op_ptr)
{
    return is_signed_extract(op_ptr->opcode());
}

bool is_signed_extract(Opcode opc)
{
//    return (opc == EXTS_B ||
//            opc == EXTS_H);
// KF 12/2004
   return ((opc & ROOT_MASK) == ROOT_EXTS);
}

bool is_unsigned_extract(Op* op_ptr)
{
    return is_unsigned_extract(op_ptr->opcode());
}

bool is_unsigned_extract(Opcode opc)
{
//    return (opc == EXTZ_B ||
// 	   opc == EXTZ_H);
// KF 12/2004
  return ((opc & ROOT_MASK) == ROOT_EXTZ);
}

bool is_extract(Op* op_ptr)
{
    return is_extract(op_ptr->opcode());
}

bool is_extract(Opcode opc)
{
   return (is_signed_extract(opc) || is_unsigned_extract(opc));
}

Opcode get_saturate_modifier(Opcode opc)
{
    return (Opcode) (opc & IR_SAT_MASK);
}

int extract_saturate_point(Opcode opc)
{
    IR_MODIFIERS mod = (IR_MODIFIERS) get_saturate_modifier(opc);
    return (extract_saturate_point(mod));
}

int extract_saturate_point(IR_MODIFIERS mod)
{
    switch (mod) {
	case IR_SAT_1:		return (1);
	case IR_SAT_2:		return (2);
	case IR_SAT_3:		return (3);
	case IR_SAT_4:		return (4);
	case IR_SAT_5:		return (5);
	case IR_SAT_6:		return (6);
	case IR_SAT_7:		return (7);
	case IR_SAT_8:		return (8);
	case IR_SAT_9:		return (9);
	case IR_SAT_10:		return (10);
	case IR_SAT_11:		return (11);
	case IR_SAT_12:		return (12);
	case IR_SAT_13:		return (13);
	case IR_SAT_14:		return (14);
	case IR_SAT_15:		return (15);
	case IR_SAT_16:		return (16);
	case IR_SAT_17:		return (17);
	case IR_SAT_18:		return (18);
	case IR_SAT_19:		return (19);
	case IR_SAT_20:		return (20);
	case IR_SAT_21:		return (21);
	case IR_SAT_22:		return (22);
	case IR_SAT_23:		return (23);
	case IR_SAT_24:		return (24);
	case IR_SAT_25:		return (25);
	case IR_SAT_26:		return (26);
	case IR_SAT_27:		return (27);
	case IR_SAT_28:		return (28);
	case IR_SAT_29:		return (29);
	case IR_SAT_30:		return (30);
	case IR_SAT_31:		return (31);
	case IR_SAT_32:		return (32);
	default:
	    El_punt("extract_saturate_point: illegal modifier");
	    return (-1);
    }
}

bool is_int_satadd(Op* op_ptr)
{
   return is_int_satadd(op_ptr->opcode()) ;
}

bool is_int_satadd(Opcode opc)
{
   return (opc == SADD1_W || opc == SADD2_W || opc == SADD3_W || opc == SADD4_W ||
           opc == SADD5_W || opc == SADD6_W || opc == SADD7_W || opc == SADD8_W ||
           opc == SADD9_W || opc == SADD10_W || opc == SADD11_W || opc == SADD12_W ||
           opc == SADD13_W || opc == SADD14_W || opc == SADD15_W || opc == SADD16_W ||
           opc == SADD17_W || opc == SADD18_W || opc == SADD19_W || opc == SADD20_W ||
           opc == SADD21_W || opc == SADD22_W || opc == SADD23_W || opc == SADD24_W ||
           opc == SADD25_W || opc == SADD26_W || opc == SADD27_W || opc == SADD28_W ||
           opc == SADD29_W || opc == SADD30_W || opc == SADD31_W || opc == SADD32_W ||

   	   opc == SADDL1_W || opc == SADDL2_W || opc == SADDL3_W || opc == SADDL4_W ||
           opc == SADDL5_W || opc == SADDL6_W || opc == SADDL7_W || opc == SADDL8_W ||
           opc == SADDL9_W || opc == SADDL10_W || opc == SADDL11_W || opc == SADDL12_W ||
           opc == SADDL13_W || opc == SADDL14_W || opc == SADDL15_W || opc == SADDL16_W ||
           opc == SADDL17_W || opc == SADDL18_W || opc == SADDL19_W || opc == SADDL20_W ||
           opc == SADDL21_W || opc == SADDL22_W || opc == SADDL23_W || opc == SADDL24_W ||
           opc == SADDL25_W || opc == SADDL26_W || opc == SADDL27_W || opc == SADDL28_W ||
           opc == SADDL29_W || opc == SADDL30_W || opc == SADDL31_W || opc == SADDL32_W );
}
bool is_int_satsub(Op* op_ptr)
{
   return is_int_satsub(op_ptr->opcode()) ;
}

bool is_int_satsub(Opcode opc)
{
   return (opc == SSUB1_W || opc == SSUB2_W || opc == SSUB3_W || opc == SSUB4_W ||
           opc == SSUB5_W || opc == SSUB6_W || opc == SSUB7_W || opc == SSUB8_W ||
           opc == SSUB9_W || opc == SSUB10_W || opc == SSUB11_W || opc == SSUB12_W ||
           opc == SSUB13_W || opc == SSUB14_W || opc == SSUB15_W || opc == SSUB16_W ||
           opc == SSUB17_W || opc == SSUB18_W || opc == SSUB19_W || opc == SSUB20_W ||
           opc == SSUB21_W || opc == SSUB22_W || opc == SSUB23_W || opc == SSUB24_W ||
           opc == SSUB25_W || opc == SSUB26_W || opc == SSUB27_W || opc == SSUB28_W ||
           opc == SSUB29_W || opc == SSUB30_W || opc == SSUB31_W || opc == SSUB32_W ||

   	   opc == SSUBL1_W || opc == SSUBL2_W || opc == SSUBL3_W || opc == SSUBL4_W ||
           opc == SSUBL5_W || opc == SSUBL6_W || opc == SSUBL7_W || opc == SSUBL8_W ||
           opc == SSUBL9_W || opc == SSUBL10_W || opc == SSUBL11_W || opc == SSUBL12_W ||
           opc == SSUBL13_W || opc == SSUBL14_W || opc == SSUBL15_W || opc == SSUBL16_W ||
           opc == SSUBL17_W || opc == SSUBL18_W || opc == SSUBL19_W || opc == SSUBL20_W ||
           opc == SSUBL21_W || opc == SSUBL22_W || opc == SSUBL23_W || opc == SSUBL24_W ||
           opc == SSUBL25_W || opc == SSUBL26_W || opc == SSUBL27_W || opc == SSUBL28_W ||
           opc == SSUBL29_W || opc == SSUBL30_W || opc == SSUBL31_W || opc == SSUBL32_W );
}


bool is_int_satmul(Op* op_ptr)
{
   return is_int_satmul(op_ptr->opcode()) ;
}

bool is_int_satmul(Opcode opc)
{
   return (opc == SMPY1_W || opc == SMPY2_W || opc == SMPY3_W || opc == SMPY4_W ||
           opc == SMPY5_W || opc == SMPY6_W || opc == SMPY7_W || opc == SMPY8_W ||
           opc == SMPY9_W || opc == SMPY10_W || opc == SMPY11_W || opc == SMPY12_W ||
           opc == SMPY13_W || opc == SMPY14_W || opc == SMPY15_W || opc == SMPY16_W ||
           opc == SMPY17_W || opc == SMPY18_W || opc == SMPY19_W || opc == SMPY20_W ||
           opc == SMPY21_W || opc == SMPY22_W || opc == SMPY23_W || opc == SMPY24_W ||
           opc == SMPY25_W || opc == SMPY26_W || opc == SMPY27_W || opc == SMPY28_W ||
           opc == SMPY29_W || opc == SMPY30_W || opc == SMPY31_W || opc == SMPY32_W ||

   	   opc == SMPYL1_W || opc == SMPYL2_W || opc == SMPYL3_W || opc == SMPYL4_W ||
           opc == SMPYL5_W || opc == SMPYL6_W || opc == SMPYL7_W || opc == SMPYL8_W ||
           opc == SMPYL9_W || opc == SMPYL10_W || opc == SMPYL11_W || opc == SMPYL12_W ||
           opc == SMPYL13_W || opc == SMPYL14_W || opc == SMPYL15_W || opc == SMPYL16_W ||
           opc == SMPYL17_W || opc == SMPYL18_W || opc == SMPYL19_W || opc == SMPYL20_W ||
           opc == SMPYL21_W || opc == SMPYL22_W || opc == SMPYL23_W || opc == SMPYL24_W ||
           opc == SMPYL25_W || opc == SMPYL26_W || opc == SMPYL27_W || opc == SMPYL28_W ||
           opc == SMPYL29_W || opc == SMPYL30_W || opc == SMPYL31_W || opc == SMPYL32_W );
}

bool is_int_satmuladd(Op* op_ptr)
{
   return is_int_satmuladd(op_ptr->opcode()) ;
}

bool is_int_satmuladd(Opcode opc)
{
   return (opc == SMPYADD1_W || opc == SMPYADD2_W || opc == SMPYADD3_W || opc == SMPYADD4_W ||
           opc == SMPYADD5_W || opc == SMPYADD6_W || opc == SMPYADD7_W || opc == SMPYADD8_W ||
           opc == SMPYADD9_W || opc == SMPYADD10_W || opc == SMPYADD11_W || opc == SMPYADD12_W ||
           opc == SMPYADD13_W || opc == SMPYADD14_W || opc == SMPYADD15_W || opc == SMPYADD16_W ||
           opc == SMPYADD17_W || opc == SMPYADD18_W || opc == SMPYADD19_W || opc == SMPYADD20_W ||
           opc == SMPYADD21_W || opc == SMPYADD22_W || opc == SMPYADD23_W || opc == SMPYADD24_W ||
           opc == SMPYADD25_W || opc == SMPYADD26_W || opc == SMPYADD27_W || opc == SMPYADD28_W ||
           opc == SMPYADD29_W || opc == SMPYADD30_W || opc == SMPYADD31_W || opc == SMPYADD32_W ||

   	   opc == SMPYADDN1_W || opc == SMPYADDN2_W || opc == SMPYADDN3_W || opc == SMPYADDN4_W ||
           opc == SMPYADDN5_W || opc == SMPYADDN6_W || opc == SMPYADDN7_W || opc == SMPYADDN8_W ||
           opc == SMPYADDN9_W || opc == SMPYADDN10_W || opc == SMPYADDN11_W || opc == SMPYADDN12_W ||
           opc == SMPYADDN13_W || opc == SMPYADDN14_W || opc == SMPYADDN15_W || opc == SMPYADDN16_W ||
           opc == SMPYADDN17_W || opc == SMPYADDN18_W || opc == SMPYADDN19_W || opc == SMPYADDN20_W ||
           opc == SMPYADDN21_W || opc == SMPYADDN22_W || opc == SMPYADDN23_W || opc == SMPYADDN24_W ||
           opc == SMPYADDN25_W || opc == SMPYADDN26_W || opc == SMPYADDN27_W || opc == SMPYADDN28_W ||
           opc == SMPYADDN29_W || opc == SMPYADDN30_W || opc == SMPYADDN31_W || opc == SMPYADDN32_W );
}

bool is_int_satmulsub(Op* op_ptr)
{
   return is_int_satmulsub(op_ptr->opcode()) ;
}

bool is_int_satmulsub(Opcode opc)
{
   return (opc == SMPYSUB1_W || opc == SMPYSUB2_W || opc == SMPYSUB3_W || opc == SMPYSUB4_W ||
           opc == SMPYSUB5_W || opc == SMPYSUB6_W || opc == SMPYSUB7_W || opc == SMPYSUB8_W ||
           opc == SMPYSUB9_W || opc == SMPYSUB10_W || opc == SMPYSUB11_W || opc == SMPYSUB12_W ||
           opc == SMPYSUB13_W || opc == SMPYSUB14_W || opc == SMPYSUB15_W || opc == SMPYSUB16_W ||
           opc == SMPYSUB17_W || opc == SMPYSUB18_W || opc == SMPYSUB19_W || opc == SMPYSUB20_W ||
           opc == SMPYSUB21_W || opc == SMPYSUB22_W || opc == SMPYSUB23_W || opc == SMPYSUB24_W ||
           opc == SMPYSUB25_W || opc == SMPYSUB26_W || opc == SMPYSUB27_W || opc == SMPYSUB28_W ||
           opc == SMPYSUB29_W || opc == SMPYSUB30_W || opc == SMPYSUB31_W || opc == SMPYSUB32_W ||

   	   opc == SMPYRSUB1_W || opc == SMPYRSUB2_W || opc == SMPYRSUB3_W || opc == SMPYRSUB4_W ||
           opc == SMPYRSUB5_W || opc == SMPYRSUB6_W || opc == SMPYRSUB7_W || opc == SMPYRSUB8_W ||
           opc == SMPYRSUB9_W || opc == SMPYRSUB10_W || opc == SMPYRSUB11_W || opc == SMPYRSUB12_W ||
           opc == SMPYRSUB13_W || opc == SMPYRSUB14_W || opc == SMPYRSUB15_W || opc == SMPYRSUB16_W ||
           opc == SMPYRSUB17_W || opc == SMPYRSUB18_W || opc == SMPYRSUB19_W || opc == SMPYRSUB20_W ||
           opc == SMPYRSUB21_W || opc == SMPYRSUB22_W || opc == SMPYRSUB23_W || opc == SMPYRSUB24_W ||
           opc == SMPYRSUB25_W || opc == SMPYRSUB26_W || opc == SMPYRSUB27_W || opc == SMPYRSUB28_W ||
           opc == SMPYRSUB29_W || opc == SMPYRSUB30_W || opc == SMPYRSUB31_W || opc == SMPYRSUB32_W );
}

bool is_flt_add(Op* op_ptr)
{
   return is_flt_add(op_ptr->opcode()) ;
}

bool is_flt_add(Opcode opc)
{
   return (opc == FADD_S ||
           opc == FADD_D);
}

bool is_flt_sub(Op* op_ptr)
{
   return is_flt_sub(op_ptr->opcode()) ;
}

bool is_flt_sub(Opcode opc)
{
   return (opc == FSUB_S ||
           opc == FSUB_D);
}

bool is_flt_mul(Op* op_ptr)
{
   return is_flt_mul(op_ptr->opcode()) ;
}

bool is_flt_mul(Opcode opc)
{
   return (opc == FMPY_S ||
           opc == FMPY_D);
}

bool is_flt_muladd(Op* op_ptr)
{
   return is_flt_muladd(op_ptr->opcode()) ;
}

bool is_flt_muladd(Opcode opc)
{
   return (opc == FMPYADD_S ||
           opc == FMPYADDN_S ||
           opc == FMPYADD_D ||
           opc == FMPYADDN_D);
}

bool is_flt_mulsub(Op* op_ptr)
{
   return is_flt_mulsub(op_ptr->opcode()) ;
}

bool is_flt_mulsub(Opcode opc)
{
   return (opc == FMPYSUB_S ||
           opc == FMPYRSUB_S ||
           opc == FMPYSUB_D ||
           opc == FMPYRSUB_D);
}

bool is_flt_div(Op* op_ptr)
{
   return is_flt_div(op_ptr->opcode()) ;
}

bool is_flt_div(Opcode opc)
{
   return (opc == FDIV_S ||
           opc == FDIV_D);
}

bool is_add(Op* op_ptr)
{
   return (is_int_add(op_ptr) || is_int_satadd(op_ptr) || is_flt_add(op_ptr));
}

bool is_add(Opcode opc)
{
   return (is_int_add(opc) || is_int_satadd(opc) || is_flt_add(opc));
}

bool is_add_carry(Op* op_ptr)
{
  return is_add_carry(op_ptr->opcode());
}

bool is_add_carry(Opcode opc)
{
  return (opc == ADD_CARRY_W || opc == ADDL_CARRY_W);
}

bool is_sub_carry(Op* op_ptr)
{
  return is_sub_carry(op_ptr->opcode());
}

bool is_sub_carry(Opcode opc)
{
  return (opc == SUB_CARRY_W || opc == SUBL_CARRY_W);
}

bool is_mul_wide(Op* op_ptr)
{
  return is_mul_wide(op_ptr->opcode());
}

bool is_mul_wide(Opcode opc)
{
  return (opc == MPY_WIDE_W || opc == MPYL_WIDE_W);
}

bool is_sub(Op* op_ptr)
{
   return (is_int_sub(op_ptr) || is_int_satsub(op_ptr) || is_flt_sub(op_ptr));
}

bool is_sub(Opcode opc)
{
   return (is_int_sub(opc) || is_int_satsub(opc) || is_flt_sub(opc));
}

bool is_mul(Op* op_ptr)
{
   return (is_int_mul(op_ptr) || is_int_satmul(op_ptr) || is_flt_mul(op_ptr));
}

bool is_mul(Opcode opc)
{
   return (is_int_mul(opc) || is_int_satmul(opc)  || is_flt_mul(opc));
}

bool is_muladd(Op* op_ptr)
{
   return (is_int_muladd(op_ptr) || is_int_satmuladd(op_ptr) || is_flt_muladd(op_ptr));
}

bool is_muladd(Opcode opc)
{
   return (is_int_muladd(opc) || is_int_satmuladd(opc)  || is_flt_muladd(opc));
}

bool is_mulsub(Op* op_ptr)
{
   return (is_int_mulsub(op_ptr) || is_int_satmulsub(op_ptr) || is_flt_mulsub(op_ptr));
}

bool is_mulsub(Opcode opc)
{
   return (is_int_mulsub(opc) || is_int_satmulsub(opc)  || is_flt_mulsub(opc));
}

bool is_div(Op *op_ptr)
{
   return (is_int_div(op_ptr) || is_flt_div(op_ptr));
}

bool is_div(Opcode opc)
{
   return (is_int_div(opc) || is_flt_div(opc));
}

bool is_ialu(Op* op_ptr)
{
  return is_ialu(op_ptr->opcode());
}

bool is_ialu(Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_ABS ||
          t == ROOT_ADD ||
          t == ROOT_ADDL ||
          t == ROOT_ADD_CARRY ||
          t == ROOT_ADDL_CARRY ||
          t == ROOT_SUB_CARRY ||
          t == ROOT_SUBL_CARRY ||
          t == ROOT_AND ||
          t == ROOT_ANDCM ||
          t == ROOT_DIV ||
          t == ROOT_DIVL ||
          t == ROOT_MAX ||
          t == ROOT_MAXL ||
          t == ROOT_MIN ||
          t == ROOT_MINL ||
          t == ROOT_MPY ||
          t == ROOT_MPYL ||
          t == ROOT_MPY_WIDE ||
          t == ROOT_MPYL_WIDE ||
          t == ROOT_NAND ||
          t == ROOT_NOR ||
          t == ROOT_OR ||
          t == ROOT_ORCM ||
          t == ROOT_REM ||
          t == ROOT_REML ||
          t == ROOT_SH1ADDL ||
          t == ROOT_SH2ADDL ||
          t == ROOT_SH3ADDL ||
          t == ROOT_SHL ||
          t == ROOT_SHR ||
          t == ROOT_SHLA ||
          t == ROOT_SHRA ||
          t == ROOT_SUB ||
          t == ROOT_SUBL ||
          t == ROOT_XOR ||
          t == ROOT_XORCM ||
          t == ROOT_EXTS ||
          t == ROOT_EXTZ ||
          t == ROOT_MOVE ||
          t == ROOT_MOVEGF_L ||
          t == ROOT_MOVEGF_U ||
          t == ROOT_MOVEPG ||
          t == ROOT_MOVEGP ||
	  t == ROOT_MOVELG ||
	  t == ROOT_MOVELGS ||
	  t == ROOT_MOVELGX ||
          t == ROOT_MOVEGG ||
          t == ROOT_MOVEGC ||
          t == ROOT_MOVECG ||
          t == ROOT_MOVEBB ||
          t == ROOT_MOVEPP ||
          t == ROOT_CMPR   ||
	  t == ROOT_MOVEGBP ||  // To restore predicates and tag bits
	  t == ROOT_MOVEGCM ||
	  t == ROOT_MOVEGBGT ||
	  t == ROOT_MOVEGBFT ||
	  t == ROOT_MOVEGBPT ||
	  t == ROOT_MOVEGBBT ||
	  t == ROOT_MOVELB ||
	  t == ROOT_MOVELBS ||
	  t == ROOT_MOVELBX ||
	  t == ROOT_EXTRACT ||
	  t == ROOT_EXTRACT_U ||
	  t == ROOT_DEPOSIT ||

// Shivaram: 16:1:2001 : Added the MPYADD operation

	  t == ROOT_MPYADD  ||
	  t == ROOT_MPYADDN ||
	  t == ROOT_MPYSUB  ||
	  t == ROOT_MPYRSUB  ||

// Shivaram: 16:1:2001 : Added the saturating operations

	  t == ROOT_SADD    ||
	  t == ROOT_SADDL   ||
	  t == ROOT_SSUB    ||
	  t == ROOT_SSUBL   ||
	  t == ROOT_SMPY    ||
	  t == ROOT_SMPYL   ||
	  t == ROOT_SMPYADD    ||
	  t == ROOT_SMPYADDN   ||
	  t == ROOT_SMPYSUB    ||
	  t == ROOT_SMPYRSUB   ||
	  // SLARSEN: min and max operations
	  t == ROOT_MIN ||
	  t == ROOT_MAX
	  );
}

// note: Sumedh
// all ops with a single binary operation are simple binary;
//          e.g. add, addl, and, or etc
//
// all ops with more than one binary operations bundled into one are complex
// binary;
//          e.g. andcm (AND and complement), sh1addl (shift left by 1 and add)
//
// simple binary ops have exactly two operands
// complex ops have more than two operands, some of which may be implicit.
//

bool is_ialu_simple_binary_op (Op* op_ptr)
{
  return is_ialu_simple_binary_op (op_ptr->opcode());
}

bool is_ialu_simple_binary_op (Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_ADD ||
          t == ROOT_ADDL ||
          t == ROOT_AND ||
          t == ROOT_OR ||
          t == ROOT_DIV ||
          t == ROOT_DIVL ||
          t == ROOT_MPY ||
          t == ROOT_MPYL ||
          t == ROOT_REM ||
          t == ROOT_REML ||
          t == ROOT_SHL ||
          t == ROOT_SHR ||
          t == ROOT_SHLA ||
          t == ROOT_SHRA ||
          t == ROOT_SUB ||
          t == ROOT_SUBL ||
          t == ROOT_XOR   ||
	  t == ROOT_SADD    ||
	  t == ROOT_SADDL   ||
	  t == ROOT_SSUB    ||
	  t == ROOT_SSUBL   ||
	  t == ROOT_SMPY    ||
	  t == ROOT_SMPYL   
	  );
}

bool is_ialu_complex_binary_op (Op* op_ptr)
{
  return is_ialu_complex_binary_op (op_ptr->opcode());
}

bool is_ialu_complex_binary_op (Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_ANDCM ||
          t == ROOT_NAND ||
          t == ROOT_NOR ||
          t == ROOT_ORCM ||
          t == ROOT_SH1ADDL ||
          t == ROOT_SH2ADDL ||
          t == ROOT_SH3ADDL ||
          t == ROOT_XORCM);
}

bool is_falu (Op* op_ptr)
{
  return is_falu(op_ptr->opcode());
}

bool is_falu (Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_FADD ||
          t == ROOT_FABS ||
          t == ROOT_FDIV ||
          t == ROOT_FMAX ||
          t == ROOT_FMIN ||
          t == ROOT_FMPY ||
          t == ROOT_FMPYADD ||
          t == ROOT_FMPYADDN ||
          t == ROOT_FMPYRSUB ||
          t == ROOT_FMPYSUB ||
          t == ROOT_FRCP ||
          t == ROOT_FSQRT ||
          t == ROOT_FSUB ||
          t == ROOT_CONVWS ||
          t == ROOT_CONVWD ||
          t == ROOT_CONVSW ||
          t == ROOT_CONVDW ||
          t == ROOT_CONVSD ||
          t == ROOT_CONVDS ||
          t == ROOT_CONVLWS ||
          t == ROOT_CONVLWD ||
          t == ROOT_CONVLSW ||
          t == ROOT_CONVLDW ||
          t == ROOT_MOVEF ||
          t == ROOT_MOVEFG_L ||
          t == ROOT_MOVEFG_U ||
          t == ROOT_FCMPR ||
	  t == ROOT_MOVELF ||
	  t == ROOT_MOVELFS
	  );
}

// note:
// convention similar to the ialus...
//
bool is_falu_simple_binary_op (Op* op_ptr)
{
  return is_falu_simple_binary_op (op_ptr->opcode());
}

bool is_falu_simple_binary_op (Opcode opc)
{
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_FADD ||
          t == ROOT_FSUB ||
          t == ROOT_FDIV ||
          t == ROOT_FMAX ||
          t == ROOT_FMIN ||
          t == ROOT_FMPY );
  // Shivaram: 16:01:2001: FRCP is not a binary op
  //        t == ROOT_FRCP ||
}

bool is_falu_complex_binary_op (Op* op_ptr)
{
  return is_falu_complex_binary_op (op_ptr->opcode());
}

bool is_falu_complex_binary_op (Opcode opc)
{
  return false;
  // Shivaram: 16:01:2001: These are not binary ops
  // 
  // return (t == ROOT_FMPYADD ||
  //         t == ROOT_FMPYADDN ||
  //         t == ROOT_FMPYRSUB ||
  //         t == ROOT_FMPYSUB);
}

bool is_literal_move(Op* op_ptr) {
  return is_literal_move(op_ptr->opcode());
}

bool is_literal_move(Opcode opc) {
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_MOVELG  ||
	  t == ROOT_MOVELGS ||
	  t == ROOT_MOVELGX ||
	  t == ROOT_MOVELF  ||
	  t == ROOT_MOVELFS ||
	  t == ROOT_MOVELB  ||
	  t == ROOT_MOVELBS ||
	  t == ROOT_MOVELBX
	  );
}

bool is_literal_move_with_shift(Op* op_ptr) {
  return is_literal_move_with_shift(op_ptr->opcode());
}

bool is_literal_move_with_shift(Opcode opc) {
  IR_ROOT_OPCODE t = get_root(opc) ;
  return (t == ROOT_MOVELGS ||
	  t == ROOT_MOVELGX ||
	  t == ROOT_MOVELFS ||
	  t == ROOT_MOVELBS ||
	  t == ROOT_MOVELBX
	  );
}

// Playdoh manual classifications

// Table 2, integer computation ops
bool is_int_computation_op(Opcode opc)
{
  Opcode t = opc;
  return (t == ABS_W ||
          t == ADD_W ||
	  t == ADDL_W ||
	  t == AND_W ||
	  t == ANDCM_W ||
	  t == DIV_W ||
	  t == DIVL_W ||
	  t == MAX_W ||
	  t == MAXL_W ||
	  t == MIN_W ||
	  t == MINL_W ||
	  t == MPY_W ||
	  t == MPYL_W ||
	  t == NAND_W ||
	  t == NOR_W ||
	  t == OR_W ||
	  t == ORCM_W ||
	  t == REM_W ||
	  t == REML_W ||
	  t == SH1ADDL_W ||
	  t == SH2ADDL_W ||
	  t == SH3ADDL_W ||
	  t == SHL_W ||
	  t == SHR_W ||
	  t == SHLA_W ||
	  t == SHRA_W ||
	  t == SUB_W ||
	  t == SUBL_W ||
	  t == XOR_W ||
	  t == XORCM_W ||

// Shivaram: 16:1:2001 : Added the MPYADD and MPYSUB operations
          t == MPYADD_W ||
          t == MPYADDN_W ||
          t == MPYRSUB_W ||
          t == MPYSUB_W ||

// Shivaram: 16:1:2001 : Added the saturating operations
          is_int_satadd(t) || 
          is_int_satsub(t) || 
          is_int_satmul(t) ||
          is_int_satmuladd(t) || 
          is_int_satmulsub(t) );
}

bool is_int_computation_op(Op *op_ptr)
{
    return (is_int_computation_op(op_ptr->opcode()));
}

// Table 3, floating-point computation ops (single precision)
bool is_flt_computation_op(Opcode opc)
{
  Opcode t = opc;
  return (t == FADD_S ||
          t == FABS_S ||
          t == FDIV_S ||
          t == FMAX_S ||
          t == FMIN_S ||
          t == FMPY_S ||
          t == FMPYADD_S ||
          t == FMPYADDN_S ||
          t == FMPYRSUB_S ||
          t == FMPYSUB_S ||
          t == FRCP_S ||
          t == FSQRT_S ||
          t == FSUB_S);
}

bool is_flt_computation_op(Op *op_ptr)
{
    return (is_flt_computation_op(op_ptr->opcode()));
}

// Table 3, floating-point computation ops (double precision)
bool is_dbl_computation_op(Opcode opc)
{
  Opcode t = opc;
  return (t == FADD_D ||
          t == FABS_D ||
          t == FDIV_D ||
          t == FMAX_D ||
          t == FMIN_D ||
          t == FMPY_D ||
          t == FMPYADD_D ||
          t == FMPYADDN_D ||
          t == FMPYRSUB_D ||
          t == FMPYSUB_D ||
          t == FRCP_D ||
          t == FSQRT_D ||
          t == FSUB_D);
}

bool is_dbl_computation_op(Op *op_ptr)
{
    return (is_dbl_computation_op(op_ptr->opcode()));
}

// Table 4, conversion operations (return int value)
bool is_int_conversion_op(Opcode opc)
{
  Opcode t = opc;
  return (t == CONVSW ||
          t == CONVLSW ||
          t == CONVDW ||
          t == CONVLDW ||
          is_extract(opc));
}

bool is_int_conversion_op(Op *op_ptr)
{
    return (is_int_conversion_op(op_ptr->opcode()));
}

// Table 4, conversion operations (return flt value)
bool is_flt_conversion_op(Opcode opc)
{
  Opcode t = opc;
  return (t == CONVWS ||
          t == CONVLWS ||
          t == CONVDS);
}

bool is_flt_conversion_op(Op *op_ptr)
{
    return (is_flt_conversion_op(op_ptr->opcode()));
}

// Table 4, conversion operations (return dbl value)
bool is_dbl_conversion_op(Opcode opc)
{
  Opcode t = opc;
  return (t == CONVWD ||
          t == CONVLWD ||
          t == CONVSD);
}

bool is_dbl_conversion_op(Op *op_ptr)
{
    return (is_dbl_conversion_op(op_ptr->opcode()));
}

// Conversion between int -> flt/dbl or the reverse
bool is_int_fltdbl_conversion_op(Op *op_ptr)
{
    return (is_int_fltdbl_conversion_op(op_ptr->opcode()));
}

bool is_int_fltdbl_conversion_op(Opcode opc)
{
  Opcode t = opc;
  return (t == CONVSW ||
          t == CONVLSW ||
          t == CONVDW ||
          t == CONVLDW ||
	  t == CONVWS ||
          t == CONVLWS ||
          t == CONVDS ||
	  t == CONVWD ||
          t == CONVLWD ||
          t == CONVSD);
}

bool is_bit_extraction_op(Opcode opc)
{
  Opcode t = opc;
  return (t == EXTRACT_W || 
	  t == EXTRACTU_W ||
	  t == DEPOSIT_W);
}

bool is_bit_extraction_op(Op *op_ptr)
{
    return (is_bit_extraction_op(op_ptr->opcode()));
}

// Mem_word instruction gets converted to Mem_halfword for WIMS simulation
// Used in SIMU
// pnagarka (4/03)
Opcode convert_MEMW_to_MEMHW(Opcode opc)
{
  Opcode new_opc = opc;

  if (!(opc&IR_BYTE) && !(opc&IR_HALFWORD) && !(opc&IR_QUADWORD) && !(opc&IR_DOUBLE))
    new_opc = (Opcode) (new_opc | IR_HALFWORD);

  return (new_opc);
}

Opcode convert_ADD_to_MPYADD(Opcode opc, bool nflag)
{
    Opcode new_opc = (Opcode)0;

// Added the code to support conversion of saturating operations
// Shivarama Rao K : 17-01-2001

    switch (opc) {
	case ADD_W:
	case ADDL_W:
	    if (nflag)
		new_opc = MPYADDN_W;
	    else
		new_opc = MPYADD_W;
	    break;
	case FADD_S:
	    if (nflag)
		new_opc = FMPYADDN_S;
	    else
		new_opc = FMPYADD_S;
	    break;
	case FADD_D:
	    if (nflag)
		new_opc = FMPYADDN_D;
	    else
		new_opc = FMPYADD_D;
	    break;

	case SADD1_W:
	case SADDL1_W:
	    if (nflag)
		new_opc = SMPYADDN1_W;
	    else
		new_opc = SMPYADD1_W;
	    break;
	case SADD2_W:
	case SADDL2_W:
	    if (nflag)
		new_opc = SMPYADDN2_W;
	    else
		new_opc = SMPYADD2_W;
	    break;
	case SADD3_W:
	case SADDL3_W:
	    if (nflag)
		new_opc = SMPYADDN3_W;
	    else
		new_opc = SMPYADD3_W;
	    break;
	case SADD4_W:
	case SADDL4_W:
	    if (nflag)
		new_opc = SMPYADDN4_W;
	    else
		new_opc = SMPYADD4_W;
	    break;
	case SADD5_W:
	case SADDL5_W:
	    if (nflag)
		new_opc = SMPYADDN5_W;
	    else
		new_opc = SMPYADD5_W;
	    break;
	case SADD6_W:
	case SADDL6_W:
	    if (nflag)
		new_opc = SMPYADDN6_W;
	    else
		new_opc = SMPYADD6_W;
	    break;
	case SADD7_W:
	case SADDL7_W:
	    if (nflag)
		new_opc = SMPYADDN7_W;
	    else
		new_opc = SMPYADD7_W;
	    break;
	case SADD8_W:
	case SADDL8_W:
	    if (nflag)
		new_opc = SMPYADDN8_W;
	    else
		new_opc = SMPYADD8_W;
	    break;
	case SADD9_W:
	case SADDL9_W:
	    if (nflag)
		new_opc = SMPYADDN9_W;
	    else
		new_opc = SMPYADD9_W;
	    break;
	case SADD10_W:
	case SADDL10_W:
	    if (nflag)
		new_opc = SMPYADDN10_W;
	    else
		new_opc = SMPYADD10_W;
	    break;
	case SADD11_W:
	case SADDL11_W:
	    if (nflag)
		new_opc = SMPYADDN11_W;
	    else
		new_opc = SMPYADD11_W;
	    break;
	case SADD12_W:
	case SADDL12_W:
	    if (nflag)
		new_opc = SMPYADDN12_W;
	    else
		new_opc = SMPYADD12_W;
	    break;
	case SADD13_W:
	case SADDL13_W:
	    if (nflag)
		new_opc = SMPYADDN13_W;
	    else
		new_opc = SMPYADD13_W;
	    break;
	case SADD14_W:
	case SADDL14_W:
	    if (nflag)
		new_opc = SMPYADDN14_W;
	    else
		new_opc = SMPYADD14_W;
	    break;
	case SADD15_W:
	case SADDL15_W:
	    if (nflag)
		new_opc = SMPYADDN15_W;
	    else
		new_opc = SMPYADD15_W;
	    break;
	case SADD16_W:
	case SADDL16_W:
	    if (nflag)
		new_opc = SMPYADDN16_W;
	    else
		new_opc = SMPYADD16_W;
	    break;
	case SADD17_W:
	case SADDL17_W:
	    if (nflag)
		new_opc = SMPYADDN17_W;
	    else
		new_opc = SMPYADD17_W;
	    break;
	case SADD18_W:
	case SADDL18_W:
	    if (nflag)
		new_opc = SMPYADDN18_W;
	    else
		new_opc = SMPYADD18_W;
	    break;
	case SADD19_W:
	case SADDL19_W:
	    if (nflag)
		new_opc = SMPYADDN19_W;
	    else
		new_opc = SMPYADD19_W;
	    break;

	case SADD20_W:
	case SADDL20_W:
	    if (nflag)
		new_opc = SMPYADDN20_W;
	    else
		new_opc = SMPYADD20_W;
	    break;
	case SADD21_W:
	case SADDL21_W:
	    if (nflag)
		new_opc = SMPYADDN21_W;
	    else
		new_opc = SMPYADD21_W;
	    break;
	case SADD22_W:
	case SADDL22_W:
	    if (nflag)
		new_opc = SMPYADDN22_W;
	    else
		new_opc = SMPYADD22_W;
	    break;
	case SADD23_W:
	case SADDL23_W:
	    if (nflag)
		new_opc = SMPYADDN23_W;
	    else
		new_opc = SMPYADD23_W;
	    break;
	case SADD24_W:
	case SADDL24_W:
	    if (nflag)
		new_opc = SMPYADDN24_W;
	    else
		new_opc = SMPYADD24_W;
	    break;
	case SADD25_W:
	case SADDL25_W:
	    if (nflag)
		new_opc = SMPYADDN25_W;
	    else
		new_opc = SMPYADD25_W;
	    break;
	case SADD26_W:
	case SADDL26_W:
	    if (nflag)
		new_opc = SMPYADDN26_W;
	    else
		new_opc = SMPYADD26_W;
	    break;
	case SADD27_W:
	case SADDL27_W:
	    if (nflag)
		new_opc = SMPYADDN27_W;
	    else
		new_opc = SMPYADD27_W;
	    break;
	case SADD28_W:
	case SADDL28_W:
	    if (nflag)
		new_opc = SMPYADDN28_W;
	    else
		new_opc = SMPYADD28_W;
	    break;
	case SADD29_W:
	case SADDL29_W:
	    if (nflag)
		new_opc = SMPYADDN29_W;
	    else
		new_opc = SMPYADD29_W;
	    break;
	case SADD30_W:
	case SADDL30_W:
	    if (nflag)
		new_opc = SMPYADDN30_W;
	    else
		new_opc = SMPYADD30_W;
	    break;
	case SADD31_W:
	case SADDL31_W:
	    if (nflag)
		new_opc = SMPYADDN31_W;
	    else
		new_opc = SMPYADD31_W;
	    break;
	case SADD32_W:
	case SADDL32_W:
	    if (nflag)
		new_opc = SMPYADDN32_W;
	    else
		new_opc = SMPYADD32_W;
	    break;

	default:
	    cdbg << "Unknown opcode: " << opc << endl;
	    El_punt("convert_ADD_to_MPYADD: unknown opcode");
	    break;
    }

    return (new_opc);
}

Opcode convert_SUB_to_MPYSUB(Opcode opc, bool rflag)
{
    Opcode new_opc = (Opcode)0;

// Added the code to support conversion of saturating operations
// Shivarama Rao K : 17-01-2001

    switch (opc) {
	case SUB_W:
	case SUBL_W:
	    if (rflag)
		new_opc = MPYRSUB_W;
	    else
		new_opc = MPYSUB_W;
	    break;
	case FSUB_S:
	    if (rflag)
		new_opc = FMPYRSUB_S;
	    else
		new_opc = FMPYSUB_S;
	    break;
	case FSUB_D:
	    if (rflag)
		new_opc = FMPYRSUB_D;
	    else
		new_opc = FMPYSUB_D;
	    break;


	case SSUB1_W:
	case SSUBL1_W:
	    if (rflag)
		new_opc = SMPYRSUB1_W;
	    else
		new_opc = SMPYSUB1_W;
	    break;
	case SSUB2_W:
	case SSUBL2_W:
	    if (rflag)
		new_opc = SMPYRSUB2_W;
	    else
		new_opc = SMPYSUB2_W;
	    break;
	case SSUB3_W:
	case SSUBL3_W:
	    if (rflag)
		new_opc = SMPYRSUB3_W;
	    else
		new_opc = SMPYSUB3_W;
	    break;
	case SSUB4_W:
	case SSUBL4_W:
	    if (rflag)
		new_opc = SMPYRSUB4_W;
	    else
		new_opc = SMPYSUB4_W;
	    break;
	case SSUB5_W:
	case SSUBL5_W:
	    if (rflag)
		new_opc = SMPYRSUB5_W;
	    else
		new_opc = SMPYSUB5_W;
	    break;
	case SSUB6_W:
	case SSUBL6_W:
	    if (rflag)
		new_opc = SMPYRSUB6_W;
	    else
		new_opc = SMPYSUB6_W;
	    break;
	case SSUB7_W:
	case SSUBL7_W:
	    if (rflag)
		new_opc = SMPYRSUB7_W;
	    else
		new_opc = SMPYSUB7_W;
	    break;
	case SSUB8_W:
	case SSUBL8_W:
	    if (rflag)
		new_opc = SMPYRSUB8_W;
	    else
		new_opc = SMPYSUB8_W;
	    break;
	case SSUB9_W:
	case SSUBL9_W:
	    if (rflag)
		new_opc = SMPYRSUB9_W;
	    else
		new_opc = SMPYSUB9_W;
	    break;
	case SSUB10_W:
	case SSUBL10_W:
	    if (rflag)
		new_opc = SMPYRSUB10_W;
	    else
		new_opc = SMPYSUB10_W;
	    break;
	case SSUB11_W:
	case SSUBL11_W:
	    if (rflag)
		new_opc = SMPYRSUB11_W;
	    else
		new_opc = SMPYSUB11_W;
	    break;
	case SSUB12_W:
	case SSUBL12_W:
	    if (rflag)
		new_opc = SMPYRSUB12_W;
	    else
		new_opc = SMPYSUB12_W;
	    break;
	case SSUB13_W:
	case SSUBL13_W:
	    if (rflag)
		new_opc = SMPYRSUB13_W;
	    else
		new_opc = SMPYSUB13_W;
	    break;
	case SSUB14_W:
	case SSUBL14_W:
	    if (rflag)
		new_opc = SMPYRSUB14_W;
	    else
		new_opc = SMPYSUB14_W;
	    break;
	case SSUB15_W:
	case SSUBL15_W:
	    if (rflag)
		new_opc = SMPYRSUB15_W;
	    else
		new_opc = SMPYSUB15_W;
	    break;
	case SSUB16_W:
	case SSUBL16_W:
	    if (rflag)
		new_opc = SMPYRSUB16_W;
	    else
		new_opc = SMPYSUB16_W;
	    break;
	case SSUB17_W:
	case SSUBL17_W:
	    if (rflag)
		new_opc = SMPYRSUB17_W;
	    else
		new_opc = SMPYSUB17_W;
	    break;
	case SSUB18_W:
	case SSUBL18_W:
	    if (rflag)
		new_opc = SMPYRSUB18_W;
	    else
		new_opc = SMPYSUB18_W;
	    break;
	case SSUB19_W:
	case SSUBL19_W:
	    if (rflag)
		new_opc = SMPYRSUB19_W;
	    else
		new_opc = SMPYSUB19_W;
	    break;

	case SSUB20_W:
	case SSUBL20_W:
	    if (rflag)
		new_opc = SMPYRSUB20_W;
	    else
		new_opc = SMPYSUB20_W;
	    break;
	case SSUB21_W:
	case SSUBL21_W:
	    if (rflag)
		new_opc = SMPYRSUB21_W;
	    else
		new_opc = SMPYSUB21_W;
	    break;
	case SSUB22_W:
	case SSUBL22_W:
	    if (rflag)
		new_opc = SMPYRSUB22_W;
	    else
		new_opc = SMPYSUB22_W;
	    break;
	case SSUB23_W:
	case SSUBL23_W:
	    if (rflag)
		new_opc = SMPYRSUB23_W;
	    else
		new_opc = SMPYSUB23_W;
	    break;
	case SSUB24_W:
	case SSUBL24_W:
	    if (rflag)
		new_opc = SMPYRSUB24_W;
	    else
		new_opc = SMPYSUB24_W;
	    break;
	case SSUB25_W:
	case SSUBL25_W:
	    if (rflag)
		new_opc = SMPYRSUB25_W;
	    else
		new_opc = SMPYSUB25_W;
	    break;
	case SSUB26_W:
	case SSUBL26_W:
	    if (rflag)
		new_opc = SMPYRSUB26_W;
	    else
		new_opc = SMPYSUB26_W;
	    break;
	case SSUB27_W:
	case SSUBL27_W:
	    if (rflag)
		new_opc = SMPYRSUB27_W;
	    else
		new_opc = SMPYSUB27_W;
	    break;
	case SSUB28_W:
	case SSUBL28_W:
	    if (rflag)
		new_opc = SMPYRSUB28_W;
	    else
		new_opc = SMPYSUB28_W;
	    break;
	case SSUB29_W:
	case SSUBL29_W:
	    if (rflag)
		new_opc = SMPYRSUB29_W;
	    else
		new_opc = SMPYSUB29_W;
	    break;
	case SSUB30_W:
	case SSUBL30_W:
	    if (rflag)
		new_opc = SMPYRSUB30_W;
	    else
		new_opc = SMPYSUB30_W;
	    break;
	case SSUB31_W:
	case SSUBL31_W:
	    if (rflag)
		new_opc = SMPYRSUB31_W;
	    else
		new_opc = SMPYSUB31_W;
	    break;
	case SSUB32_W:
	case SSUBL32_W:
	    if (rflag)
		new_opc = SMPYRSUB32_W;
	    else
		new_opc = SMPYSUB32_W;
	    break;
	default:
	    cdbg << "Unknown opcode: " << opc << endl;
	    El_punt("convert_SUB_to_MPYSUB: unknown opcode");
	    break;
    }

    return (new_opc);
}


bool is_custom(Op* op_ptr)
{
  return is_custom(op_ptr->opcode());
}

bool is_custom(Opcode opc)
{
  return ((opc & ROOT_MASK) == ROOT_CUSTOM);
}


///////////////////////////////////////////////////////////////////////////////
//
//	Pseudo Op queries
//
//////////////////////////////////////////////////////////////////////////////

bool is_pseudo(Op* op)
{
  return (is_pseudo(op->opcode()));
}

bool is_pseudo(Opcode opc)
{
  return ((IR_PSEUDO & opc) != 0);
}

bool is_control_merge(Op* op_ptr) 
{
  return (is_control_merge(op_ptr->opcode()));
} 

bool is_control_merge(Opcode opc)
{
    if (opc==C_MERGE)
	return (true);
    else
	return (false);
}

bool is_merge(Op* op_ptr)
{
  return (op_ptr->opcode() == D_MERGE);
} 

bool is_switch(Op* op_ptr) 
{
  return (op_ptr->opcode() == D_SWITCH);
} 

bool is_use(Op* op_ptr) 
{
  return (op_ptr->opcode() == USE);
}

bool is_def(Op* op_ptr) 
{
  return is_def(op_ptr->opcode());
}

bool is_def(Opcode opc) 
{
  return (opc == DEF);
}

bool is_remap(Op* op_ptr) 
{
  return is_remap(op_ptr->opcode());
}

bool is_remap(Opcode opc) 
{
  return (opc == REMAP);
}

bool is_define(Op* op_ptr) 
{
  return is_define(op_ptr->opcode());
}

bool is_define(Opcode opc) 
{
  return (opc == DEFINE);
}

bool is_pseudo_load(Op* op_ptr) 
{
  return is_pseudo_load(op_ptr->opcode());
}

bool is_pseudo_load(Opcode opc) 
{
  return ( is_pseudo_load_literal(opc) ||
           is_pseudo_load_scalar(opc) ||
           is_pseudo_load_scalar_array(opc) ||
           is_pseudo_load_evr(opc) );
}

/* Shivarama Rao; Added the code on 06-06-2001
 * This code is defined as new pseudo_load operations are 
 * added, which loads explicit contents instead of earlier
 * overloaded PSEUDO_LOAD operation
 */
bool is_pseudo_load_literal(Op* op_ptr) 
{
  return is_pseudo_load_literal(op_ptr->opcode());
}

bool is_pseudo_load_literal(Opcode opc) 
{
  return (opc == PSEUDO_LOAD_LITERAL);
}

bool is_pseudo_load_scalar(Op* op_ptr) 
{
  return is_pseudo_load_scalar(op_ptr->opcode());
}

bool is_pseudo_load_scalar(Opcode opc) 
{
  return (opc == PSEUDO_LOAD_SCALAR);
}

bool is_pseudo_load_scalar_array(Op* op_ptr) 
{
  return is_pseudo_load_scalar_array(op_ptr->opcode());
}

bool is_pseudo_load_scalar_array(Opcode opc) 
{
  return (opc == PSEUDO_LOAD_SCALAR_ARRAY);
}

bool is_pseudo_load_evr(Op* op_ptr) 
{
  return is_pseudo_load_evr(op_ptr->opcode());
}

bool is_pseudo_load_evr(Opcode opc) 
{
  return (opc == PSEUDO_LOAD_EVR);
}

bool is_pseudo_store(Op* op_ptr) 
{
/*  return (opc == PSEUDO_STORE); */
  return is_pseudo_store(op_ptr->opcode());
}

bool is_pseudo_store(Opcode opc) 
{
  return (is_pseudo_store_scalar(opc) ||
          is_pseudo_store_scalar_array(opc) ||
          is_pseudo_store_evr(opc) );
}


/* Shivarama Rao; Added the code on 06-06-2001
 * This code is defined as new pseudo_store operations are
 * added, which stores to explicit contents instead of earlier
 * overloaded PSEUDO_STORE operation
 */


bool is_pseudo_store_scalar(Op* op_ptr) 
{
  return is_pseudo_store_scalar(op_ptr->opcode());
}

bool is_pseudo_store_scalar(Opcode opc) 
{
  return (opc == PSEUDO_STORE_SCALAR);
}

bool is_pseudo_store_scalar_array(Op* op_ptr) 
{
  return is_pseudo_store_scalar_array(op_ptr->opcode());
}

bool is_pseudo_store_scalar_array(Opcode opc) 
{
  return (opc == PSEUDO_STORE_SCALAR_ARRAY);
}

bool is_pseudo_store_evr(Op* op_ptr) 
{
  return is_pseudo_store_evr(op_ptr->opcode());
}

bool is_pseudo_store_evr(Opcode opc) 
{
  return (opc == PSEUDO_STORE_EVR);
}

bool is_pseudo_print(Op* op_ptr) 
{
  return is_pseudo_print(op_ptr->opcode());
}

bool is_pseudo_print(Opcode opc) 
{
  return (opc == PSEUDO_PRINT);
}

bool is_pseudo_init_mem(Op* op_ptr) 
{
  return is_pseudo_init_mem(op_ptr->opcode());
}

bool is_pseudo_init_mem(Opcode opc) 
{
  return (opc == PSEUDO_INIT_MEM);
}


///////////////////////////////////////////////////////////////////////////////
//
//	Miscellaneous queries
//
///////////////////////////////////////////////////////////////////////////////

Op* real_op (Op* op)
{
  if (is_merge(op) || is_switch(op)) {
    List<Op*>& relops = op->get_relops();
    return relops.head();
  } else {
    return op;
  }
}

bool is_double(Op* op)
{
  return (is_double(op->opcode()));
}

bool is_double(Opcode opc)
{ 
  return ((IR_DOUBLE & opc) != 0);
}

bool has_side_effect(Op* op)
{
  return has_side_effect(op->opcode());
}

bool has_side_effect(Opcode opc)
{
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_LD:      
    case ROOT_FLD:  
    case ROOT_ST:   
    case ROOT_FST:   
    case ROOT_DSLD:
    case ROOT_FDSLD:  
    case ROOT_DVLD:   
    case ROOT_FDVLD: 
    case ROOT_BRU:    
    case ROOT_RTS:    
    case ROOT_BRCT:   
    case ROOT_BRCF:   
    case ROOT_BRL:    
    case ROOT_BRLC:  
    case ROOT_BRLP:  
    case ROOT_BRF:    
    case ROOT_BRW:
    // SLARSEN: Vector memops
    case ROOT_VLD:
    case ROOT_VFLD:
    case ROOT_VST:
    case ROOT_VFST:
    case ROOT_VLG:
    case ROOT_VFLG:
    case ROOT_VSG:
    case ROOT_VFSG:
    case ROOT_VLDE:
    case ROOT_VFLDE:
    case ROOT_VSTE:
    case ROOT_VFSTE:
    case ROOT_VLGE:
    case ROOT_VFLGE:
    case ROOT_VSGE:
    case ROOT_VFSGE:
      return true;
      break;
    default:
      return false;
    }
  return false;  
}

/*
 *  Reverse operation support function
 *
 *  This function returns the opcode to be used when the order of the operands
 *  are switched.  This is only defined for 2 operand instructions.  Returns 0
 *  if no reverse op exists or the number of operands is not 2.  (Even though
 *  0 is NO_OP, this is OK since NO_OP has no reverse op.)
 */

Opcode get_reverse_op(Opcode opc)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_ADD:
    case ROOT_ADDL:
    case ROOT_AND:
    case ROOT_MPY:
    case ROOT_MPYL:
    case ROOT_NAND: 
    case ROOT_NOR:
    case ROOT_OR: 
    case ROOT_XOR:
    case ROOT_FADD:
    case ROOT_FMAX:
    case ROOT_FMIN:
    case ROOT_FMPY:

// Shivaram: 16:1:2001 : Added the saturating operations
    case ROOT_SADD:
    case ROOT_SADDL:
    case ROOT_SSUB:
    case ROOT_SSUBL:
    case ROOT_SMPY:
    case ROOT_SMPYL:
      return new_opc;
      
    case ROOT_CMPP:
    {
      IR_BASE_OPCODE base = get_base(opc);
      new_opc = (Opcode)(new_opc & ~base);
      
      if(base == BASE_CMPP_W_EQ) return (Opcode)(new_opc | BASE_CMPP_W_EQ);
      if(base == BASE_CMPP_W_NEQ) return (Opcode)(new_opc | BASE_CMPP_W_NEQ);
      
      if(base == BASE_CMPP_W_LT) return (Opcode)(new_opc | BASE_CMPP_W_GT);
      if(base == BASE_CMPP_W_GT) return (Opcode)(new_opc | BASE_CMPP_W_LT);
      
      if(base == BASE_CMPP_W_LEQ) return (Opcode)(new_opc | BASE_CMPP_W_GEQ);
      if(base == BASE_CMPP_W_GEQ) return (Opcode)(new_opc | BASE_CMPP_W_LEQ);
      
      if(base == BASE_CMPP_W_LLT) return (Opcode)(new_opc | BASE_CMPP_W_LGT);
      if(base == BASE_CMPP_W_LGT) return (Opcode)(new_opc | BASE_CMPP_W_LLT);
      
      if(base == BASE_CMPP_W_LLEQ) return (Opcode)(new_opc | BASE_CMPP_W_LGEQ);
      if(base == BASE_CMPP_W_LGEQ) return (Opcode)(new_opc | BASE_CMPP_W_LLEQ);
    }

    case ROOT_FCMPP:
    {
      IR_BASE_OPCODE base = get_base(opc);
      new_opc = (Opcode)(new_opc & ~base);

      if(base == BASE_FCMPP_S_EQ) return (Opcode)(new_opc | BASE_FCMPP_S_EQ);
      if(base == BASE_FCMPP_S_NEQ) return (Opcode)(new_opc | BASE_FCMPP_S_NEQ);
      
      if(base == BASE_FCMPP_S_LT) return (Opcode)(new_opc | BASE_FCMPP_S_GT);
      if(base == BASE_FCMPP_S_GT) return (Opcode)(new_opc | BASE_FCMPP_S_LT);
      
      if(base == BASE_FCMPP_S_LEQ) return (Opcode)(new_opc | BASE_FCMPP_S_GEQ);
      if(base == BASE_FCMPP_S_GEQ) return (Opcode)(new_opc | BASE_FCMPP_S_LEQ);
      
      if(base == BASE_FCMPP_D_EQ) return (Opcode)(new_opc | BASE_FCMPP_D_EQ);
      if(base == BASE_FCMPP_D_NEQ) return (Opcode)(new_opc | BASE_FCMPP_D_NEQ);
      
      if(base == BASE_FCMPP_D_LT) return (Opcode)(new_opc | BASE_FCMPP_D_GT);
      if(base == BASE_FCMPP_D_GT) return (Opcode)(new_opc | BASE_FCMPP_D_LT);
      
      if(base == BASE_FCMPP_D_LEQ) return (Opcode)(new_opc | BASE_FCMPP_D_GEQ);
      if(base == BASE_FCMPP_D_GEQ) return (Opcode)(new_opc | BASE_FCMPP_D_LEQ);
    }

    case ROOT_CMPR:
    {
      if(opc == CMPR_W_EQ) return (Opcode)(CMPR_W_EQ);
      if(opc == CMPR_W_NEQ) return (Opcode)(CMPR_W_NEQ);
      
      if(opc == CMPR_W_LT) return (Opcode)(CMPR_W_GT);
      if(opc == CMPR_W_GT) return (Opcode)(CMPR_W_LT);
      
      if(opc == CMPR_W_LEQ) return (Opcode)(CMPR_W_GEQ);
      if(opc == CMPR_W_GEQ) return (Opcode)(CMPR_W_LEQ);
    }

    case ROOT_FCMPR:
    {
      if(opc == FCMPR_S_EQ) return (Opcode)(FCMPR_S_EQ);
      if(opc == FCMPR_S_NEQ) return (Opcode)(FCMPR_S_NEQ);
      
      if(opc == FCMPR_S_LT) return (Opcode)(FCMPR_S_GT);
      if(opc == FCMPR_S_GT) return (Opcode)(FCMPR_S_LT);
      
      if(opc == FCMPR_S_LEQ) return (Opcode)(FCMPR_S_GEQ);
      if(opc == FCMPR_S_GEQ) return (Opcode)(FCMPR_S_LEQ);

      if(opc == FCMPR_D_EQ) return (Opcode)(FCMPR_D_EQ);
      if(opc == FCMPR_D_NEQ) return (Opcode)(FCMPR_D_NEQ);
      
      if(opc == FCMPR_D_LT) return (Opcode)(FCMPR_D_GT);
      if(opc == FCMPR_D_GT) return (Opcode)(FCMPR_D_LT);
      
      if(opc == FCMPR_D_LEQ) return (Opcode)(FCMPR_D_GEQ);
      if(opc == FCMPR_D_GEQ) return (Opcode)(FCMPR_D_LEQ);
    }

    default:
      return (Opcode)0;
    }

  El_punt("Something is SERIOUSLY wrong.  The world is coming to an end.");
  return (Opcode)0;  
  
}

/*
 *  Reverse condition support function
 *
 *  This function returns the opcode to be used when we want to compute
 *  the negation of the current opcode.
 */

Opcode get_opposite_cond(Opcode opc)
{
  Opcode new_opc = opc;
  
  IR_ROOT_OPCODE root = get_root(opc);
  
  switch(root)
    {
    case ROOT_CMPP:
    {
      IR_BASE_OPCODE base = get_base(opc);
      new_opc = (Opcode)(new_opc & ~base);
      
      if(base == BASE_CMPP_W_EQ) return (Opcode)(new_opc | BASE_CMPP_W_NEQ);
      if(base == BASE_CMPP_W_NEQ) return (Opcode)(new_opc | BASE_CMPP_W_EQ);
      
      if(base == BASE_CMPP_W_LT) return (Opcode)(new_opc | BASE_CMPP_W_GEQ);
      if(base == BASE_CMPP_W_GT) return (Opcode)(new_opc | BASE_CMPP_W_LEQ);
      
      if(base == BASE_CMPP_W_LEQ) return (Opcode)(new_opc | BASE_CMPP_W_GT);
      if(base == BASE_CMPP_W_GEQ) return (Opcode)(new_opc | BASE_CMPP_W_LT);
      
      if(base == BASE_CMPP_W_LLT) return (Opcode)(new_opc | BASE_CMPP_W_LGEQ);
      if(base == BASE_CMPP_W_LGT) return (Opcode)(new_opc | BASE_CMPP_W_LLEQ);
      
      if(base == BASE_CMPP_W_LLEQ) return (Opcode)(new_opc | BASE_CMPP_W_LGT);
      if(base == BASE_CMPP_W_LGEQ) return (Opcode)(new_opc | BASE_CMPP_W_LLT);
    }

    case ROOT_FCMPP:
    {
      IR_BASE_OPCODE base = get_base(opc);
      new_opc = (Opcode)(new_opc & ~base);

      if(base == BASE_FCMPP_S_EQ) return (Opcode)(new_opc | BASE_FCMPP_S_NEQ);
      if(base == BASE_FCMPP_S_NEQ) return (Opcode)(new_opc | BASE_FCMPP_S_EQ);
      
      if(base == BASE_FCMPP_S_LT) return (Opcode)(new_opc | BASE_FCMPP_S_GEQ);
      if(base == BASE_FCMPP_S_GT) return (Opcode)(new_opc | BASE_FCMPP_S_LEQ);
      
      if(base == BASE_FCMPP_S_LEQ) return (Opcode)(new_opc | BASE_FCMPP_S_GT);
      if(base == BASE_FCMPP_S_GEQ) return (Opcode)(new_opc | BASE_FCMPP_S_LT);
      
      if(base == BASE_FCMPP_D_EQ) return (Opcode)(new_opc | BASE_FCMPP_D_NEQ);
      if(base == BASE_FCMPP_D_NEQ) return (Opcode)(new_opc | BASE_FCMPP_D_EQ);
      
      if(base == BASE_FCMPP_D_LT) return (Opcode)(new_opc | BASE_FCMPP_D_GEQ);
      if(base == BASE_FCMPP_D_GT) return (Opcode)(new_opc | BASE_FCMPP_D_LEQ);
      
      if(base == BASE_FCMPP_D_LEQ) return (Opcode)(new_opc | BASE_FCMPP_D_GT);
      if(base == BASE_FCMPP_D_GEQ) return (Opcode)(new_opc | BASE_FCMPP_D_LT);
    }

    case ROOT_CMPR:
    {
      if(opc == CMPR_W_EQ) return (Opcode)(CMPR_W_NEQ);
      if(opc == CMPR_W_NEQ) return (Opcode)(CMPR_W_EQ);
      
      if(opc == CMPR_W_LT) return (Opcode)(CMPR_W_GEQ);
      if(opc == CMPR_W_GT) return (Opcode)(CMPR_W_LEQ);
      
      if(opc == CMPR_W_LEQ) return (Opcode)(CMPR_W_GT);
      if(opc == CMPR_W_GEQ) return (Opcode)(CMPR_W_LT);
    }

    case ROOT_FCMPR:
    {
      if(opc == FCMPR_S_EQ) return (Opcode)(FCMPR_S_NEQ);
      if(opc == FCMPR_S_NEQ) return (Opcode)(FCMPR_S_EQ);
      
      if(opc == FCMPR_S_LT) return (Opcode)(FCMPR_S_GEQ);
      if(opc == FCMPR_S_GT) return (Opcode)(FCMPR_S_LEQ);
      
      if(opc == FCMPR_S_LEQ) return (Opcode)(FCMPR_S_GT);
      if(opc == FCMPR_S_GEQ) return (Opcode)(FCMPR_S_LT);

      if(opc == FCMPR_D_EQ) return (Opcode)(FCMPR_D_NEQ);
      if(opc == FCMPR_D_NEQ) return (Opcode)(FCMPR_D_EQ);
      
      if(opc == FCMPR_D_LT) return (Opcode)(FCMPR_D_GEQ);
      if(opc == FCMPR_D_GT) return (Opcode)(FCMPR_D_LEQ);
      
      if(opc == FCMPR_D_LEQ) return (Opcode)(FCMPR_D_GT);
      if(opc == FCMPR_D_GEQ) return (Opcode)(FCMPR_D_LT);
    }

    default:
      return (Opcode)0;
    }

  El_punt("Something is SERIOUSLY wrong.  The world is coming to an end.");
  return (Opcode)0;  
  
}

Opcode convert_CMPP_to_CMPR(Opcode opc)
{
   Opcode new_opc = opc;
  
   IR_ROOT_OPCODE root = get_root(opc);
   if ((root == ROOT_CMPP) || (root == ROOT_FCMPP))
   {
      new_opc = (Opcode)(IR_CMPP_BASE_MASK & opc) ;
      if (root == ROOT_CMPP) return (Opcode)((new_opc^ROOT_CMPP) | ROOT_CMPR) ;
      if (root == ROOT_FCMPP) return (Opcode)((new_opc^ROOT_FCMPP) | ROOT_FCMPR) ;
   }
   return(new_opc) ;
}

Opcode convert_CMPR_to_base_CMPP(Opcode opc)
{
   Opcode new_opc = opc;
  
   IR_ROOT_OPCODE root = get_root(opc);
   if ((root == ROOT_CMPR) || (root == ROOT_FCMPR))
   {
      new_opc = (Opcode)(IR_CMPP_BASE_MASK & opc) ;
      if (root == ROOT_CMPR) return (Opcode)((new_opc^ROOT_CMPR) | ROOT_CMPP) ;
      if (root == ROOT_FCMPR) return (Opcode)((new_opc^ROOT_FCMPR) | ROOT_FCMPP) ;
   }
   return(new_opc) ;
}

Opcode change_base_opcode(Opcode opc, Opcode new_base)
{
    Opcode new_opc = opc;

    IR_BASE_OPCODE base = get_base(opc);
    new_opc = (Opcode) (opc & ~base);
    new_opc = (Opcode) (new_opc | new_base);
    return (new_opc);
}

bool is_excepting_op(Op *op_ptr)
{
    return is_excepting_op(op_ptr->opcode());
}

bool is_excepting_op(Opcode opc)
{
    return (is_load(opc) || is_store(opc) || is_falu(opc) || is_int_div(opc) ||
	    is_int_rem(opc));
}


int extract_local_memory_id(Opcode opc)
{
    IR_MODIFIERS mod = (IR_MODIFIERS) get_local_mem_modifier(opc);
    return (extract_local_memory_id(mod));
}

int extract_local_memory_id(IR_MODIFIERS mod)
{
    switch (mod) {
	case IR_LM_L1:		return (1);
	case IR_LM_L2:		return (2);
	case IR_LM_L3:		return (3);
	case IR_LM_L4:		return (4);
	case IR_LM_L5:		return (5);
	case IR_LM_L6:		return (6);
	case IR_LM_L7:		return (7);
	case IR_LM_L8:		return (8);
	case IR_LM_L9:		return (9);
	case IR_LM_L10:		return (10);
	case IR_LM_L11:		return (11);
	case IR_LM_L12:		return (12);
	case IR_LM_L13:		return (13);
	case IR_LM_L14:		return (14);
	case IR_LM_L15:		return (15);
	case IR_LM_L16:		return (16);
	case IR_LM_L17:		return (17);
	case IR_LM_L18:		return (18);
	case IR_LM_L19:		return (19);
	case IR_LM_L20:		return (20);
	case IR_LM_L21:		return (21);
	case IR_LM_L22:		return (22);
	case IR_LM_L23:		return (23);
	case IR_LM_L24:		return (24);
	case IR_LM_L25:		return (25);
	case IR_LM_L26:		return (26);
	case IR_LM_L27:		return (27);
	case IR_LM_L28:		return (28);
	case IR_LM_L29:		return (29);
	case IR_LM_L30:		return (30);
	case IR_LM_L31:		return (31);
	case IR_LM_L32:		return (32);
	default:
	    El_punt("extract_local_memory_id: illegal modifier");
	    return (-1);
    }
}

Opcode convert_to_local_memory(Opcode opc, IR_MODIFIERS lm_spec)
{
    IR_ROOT_OPCODE root, new_root = IR_ROOT_OPCODE(0);
    Opcode new_opc;

    root = get_root(opc);
    switch (root) {
        case ROOT_LD:
	    new_root = ROOT_LL;
	    break;
	case ROOT_LG:
	    new_root = ROOT_LLG;
	    break;
	case ROOT_LDX:
	    new_root = ROOT_LLX;
	    break;
	case ROOT_LGX:
	    new_root = ROOT_LLGX;
	    break;

	case ROOT_FLD:
	    new_root = ROOT_FLL;
	    break;
	case ROOT_FLG:
	    new_root = ROOT_FLLG;
	    break;

	case ROOT_ST:
	    new_root = ROOT_SL;
	    break;
	case ROOT_SG:
	    new_root = ROOT_SLG;
	    break;

	case ROOT_FST:
	    new_root = ROOT_FSL;
	    break;
	case ROOT_FSG:
	    new_root = ROOT_FSLG;
	    break;

	default:
	    El_punt("convert_to_local_memory: unknown opc %s",
			(char *)el_opcode_to_string_map.value(opc));
    }
    new_opc = (Opcode) ((opc^root) | new_root);
    new_opc = (Opcode) (new_opc & (~IR_SRC_MASK));
    new_opc = (Opcode) (new_opc & (~IR_DEST_MASK));
    new_opc = (Opcode) (new_opc | lm_spec);
    return (new_opc);
}

Opcode convert_to_local_memory(Opcode opc, int lm_id)
{
    if (lm_id <= 0)
	El_punt("convert_to_local_memory: illegal lm_id %d", lm_id);
    if (lm_id > 32)
	El_punt("convert_to_local_memory: max lm_id exceeded %d", lm_id);

    switch (lm_id) {
	case 1:
	    return (convert_to_local_memory(opc, IR_LM_L1));
	case 2:
	    return (convert_to_local_memory(opc, IR_LM_L2));
	case 3:
	    return (convert_to_local_memory(opc, IR_LM_L3));
	case 4:
	    return (convert_to_local_memory(opc, IR_LM_L4));
	case 5:
	    return (convert_to_local_memory(opc, IR_LM_L5));
	case 6:
	    return (convert_to_local_memory(opc, IR_LM_L6));
	case 7:
	    return (convert_to_local_memory(opc, IR_LM_L7));
	case 8:
	    return (convert_to_local_memory(opc, IR_LM_L8));
	case 9:
	    return (convert_to_local_memory(opc, IR_LM_L9));
	case 10:
	    return (convert_to_local_memory(opc, IR_LM_L10));
	case 11:
	    return (convert_to_local_memory(opc, IR_LM_L11));
	case 12:
	    return (convert_to_local_memory(opc, IR_LM_L12));
	case 13:
	    return (convert_to_local_memory(opc, IR_LM_L13));
	case 14:
	    return (convert_to_local_memory(opc, IR_LM_L14));
	case 15:
	    return (convert_to_local_memory(opc, IR_LM_L15));
	case 16:
	    return (convert_to_local_memory(opc, IR_LM_L16));
	case 17:
	    return (convert_to_local_memory(opc, IR_LM_L17));
	case 18:
	    return (convert_to_local_memory(opc, IR_LM_L18));
	case 19:
	    return (convert_to_local_memory(opc, IR_LM_L19));
	case 20:
	    return (convert_to_local_memory(opc, IR_LM_L20));
	case 21:
	    return (convert_to_local_memory(opc, IR_LM_L21));
	case 22:
	    return (convert_to_local_memory(opc, IR_LM_L22));
	case 23:
	    return (convert_to_local_memory(opc, IR_LM_L23));
	case 24:
	    return (convert_to_local_memory(opc, IR_LM_L24));
	case 25:
	    return (convert_to_local_memory(opc, IR_LM_L25));
	case 26:
	    return (convert_to_local_memory(opc, IR_LM_L26));
	case 27:
	    return (convert_to_local_memory(opc, IR_LM_L27));
	case 28:
	    return (convert_to_local_memory(opc, IR_LM_L28));
	case 29:
	    return (convert_to_local_memory(opc, IR_LM_L29));
	case 30:
	    return (convert_to_local_memory(opc, IR_LM_L30));
	case 31:
	    return (convert_to_local_memory(opc, IR_LM_L31));
	case 32:
	    return (convert_to_local_memory(opc, IR_LM_L32));
	default:
	    El_punt("convert_to_local_memory: should never get here");
	    return ((Opcode) 0);
    }
}

bool is_logical_int_cmp(Op* op_ptr) {
   return is_logical_int_cmp(op_ptr->opcode()) ;
}

bool is_logical_int_cmp(Opcode opc) {
  IR_ROOT_OPCODE root = get_root(opc) ;
  if ((root == ROOT_CMPR) || 
      (root == ROOT_CMPP)) {
    IR_MODIFIERS t = (IR_MODIFIERS)(opc & IR_COND_MASK) ;
    if ( (t == IR_LLT) ||
	 (t == IR_LLEQ) ||
	 (t == IR_LGT) ||
	 (t == IR_LGEQ))
      return true ;
    else return false;
  } else return false;
}

///////////////////////////////////////////////////////////////////////////////
//
//	Op generating queries/functions
//
///////////////////////////////////////////////////////////////////////////////

// This function assumes src and dest operands are the same type!!!!, ie GPR to GPR, PR to
// PR, etc.  Thsi function does not handle moving PR to GPR, etc.
Opcode get_move_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	switch (dti) {
	    case EL_DT_INFO_POINTER:
	    case EL_DT_INFO_INT:
		return (MOVEGG);
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return (MOVEF_S);
		else if ( operand.dtype().is_double() ) return (MOVEF_D);
		else El_punt("get_move_opcode_for_operand: wacky float sizes not supported yet");
	    case EL_DT_INFO_BRANCH:
		return (MOVEBB);
            case EL_DT_INFO_PREDICATE:
		return (MOVEPP);
	    default:
		El_punt("get_move_opcode_for_operand: uknown data type");
	}
    }
    else if (operand.is_int() || operand.is_string() || operand.is_label()) {
	return (MOVE);
    }
    else if (operand.is_float())
	return (MOVEF_S);
    else if (operand.is_double())
	return (MOVEF_D);
    else 
	El_punt("get_move_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}

Opcode get_move_opcode_for_operands(const Operand& dest, const Operand& src)
{
   Reg_file dest_vrf = dest.file_type();
   Reg_file src_vrf;

   if (src.is_reg() || src.is_macro_reg() || src.is_vr_name())
      src_vrf = src.file_type();
   else
      src_vrf = UNDEF_VRF;

   switch (dest_vrf) {
   case GPR:

      if (src_vrf == GPR)
	 return (MOVEGG);
      else if (src_vrf == CR)
	 return (MOVECG);
      else if (src_vrf == PR)
	 return (MOVEPG);
      else if (src_vrf == BTR)
	 return (MOVE);
      else if (src.is_lit())
	 return (MOVE);
      else
	 El_punt("get_move_opcode_for_operands: invalid move");

   case FPR:

      if (src_vrf == FPR) {
	 if (src.dtype().is_single())
	    return (MOVEF_S);
	 else if (src.dtype().is_double())
	    return (MOVEF_D);
      }
      else if (src.is_float())
	 return (MOVEF_S);
      else if (src.is_double())
	 return (MOVEF_D);
      else
	 El_punt("get_move_opcode_for_operands: invalid move");

   case PR:

      if (src_vrf == PR)
	 return (MOVEPP);
      else
	 El_punt("get_move_opcode_for_operands: invalid move");

   case BTR:

      if (src_vrf == BTR)
	 return (MOVEBB);
      else if (src_vrf == CR || src_vrf == GPR)
	 return (MOVE);
      else if (src.is_lit())
	 return (MOVE);
      else
	 El_punt("get_move_opcode_for_operands: invalid move");

   case CR:

      if (src_vrf == GPR)
	 return (MOVEGC);
      else if (src_vrf == CR || src_vrf == BTR)
	 return (MOVE);
      else if (src.is_lit())
	 return (MOVE);
      else
	 El_punt("get_move_opcode_for_operands: invalid move");

   default:
      
      El_punt("get_move_opcode_for_operands; invalid move");
   }

   return (Opcode)0;
}

Opcode get_add_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	switch (dti) {
	    case EL_DT_INFO_POINTER:
	    case EL_DT_INFO_INT:
		if ( operand.dtype().is_signed() ) return (ADD_W);
		else return (ADDL_W);
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return (FADD_S);
		else if ( operand.dtype().is_double() ) return (FADD_D);
		else El_punt("get_move_opcode_for_operand: wacky float sizes not supported yet");
	    case EL_DT_INFO_BRANCH:
	        El_punt("get_add_opcode_for_operand: no add for branch tgt");
	        break;
            case EL_DT_INFO_PREDICATE:
	        El_punt("get_add_opcode_for_operand: no add for predicate");
	        break;
	    default:
	        El_punt("get_add_opcode_for_operand: uknown data type!");
	}
    }
    else if (operand.is_int() || operand.is_string() || operand.is_label())
	return (ADD_W);
    else if (operand.is_float())
	return (FADD_S);
    else if (operand.is_double())
	return (FADD_D);
    else 
	El_punt("get_add_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}

Opcode get_sub_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	switch (dti) {
	    case EL_DT_INFO_INT:
	    case EL_DT_INFO_POINTER:
		if ( operand.dtype().is_signed() ) return (SUB_W);
		else return (SUBL_W);
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return (FSUB_S);
		else if ( operand.dtype().is_double() ) return (FSUB_D);
		else El_punt("get_move_opcode_for_operand: wacky float sizes not supported yet");
	    case EL_DT_INFO_BRANCH:
	        El_punt("get_sub_opcode_for_operand: no sub for branch tgt");
	        break;
            case EL_DT_INFO_PREDICATE:
	        El_punt("get_sub_opcode_for_operand: no sub for predicate");
	        break;
	    default:
	        El_punt("get_sub_opcode_for_operand: uknown data type!");
	}
    }
    else if (operand.is_int() || operand.is_string() || operand.is_label())
	return (SUB_W);
    else if (operand.is_float())
	return (FSUB_S);
    else if (operand.is_double())
	return (FSUB_D);
    else 
	El_punt("get_sub_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}

Opcode get_mpy_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	switch (dti) {
	    case EL_DT_INFO_INT:
	    case EL_DT_INFO_POINTER:
		if ( operand.dtype().is_signed() ) return (MPY_W);
		else return (MPYL_W);
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return (FMPY_S);
		else if ( operand.dtype().is_double() ) return (FMPY_D);
		else El_punt("get_move_opcode_for_operand: wacky float sizes not supported yet");
	    case EL_DT_INFO_BRANCH:
	        El_punt("get_mpy_opcode_for_operand: no mpy for branch tgt");
	        break;
            case EL_DT_INFO_PREDICATE:
	        El_punt("get_mpy_opcode_for_operand: no mpy for predicate");
	        break;
	    default:
	        El_punt("get_mpy_opcode_for_operand: uknown data type!");
	}
    }
    else if (operand.is_int() || operand.is_string() || operand.is_label())
	return (MPY_W);
    else if (operand.is_float())
	return (FMPY_S);
    else if (operand.is_double())
	return (FMPY_D);
    else 
	El_punt("get_mpy_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}

// * Assume C1/C1 cache specifiers for this function
// * At this time do not account for signed/unsigned, needs to be fixed
//   for the future.
Opcode get_load_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	int upwidth = operand.dtype().upconv_width();
	switch (dti) {
	    case EL_DT_INFO_INT:
	    case EL_DT_INFO_POINTER:
		switch( upwidth ) {
		     case EL_DEFAULT_INT_WIDTH: 
			return ((Opcode) L_W_C1_C1);
		     case EL_DEFAULT_CHAR_WIDTH: 
			return ((Opcode) L_B_C1_C1);
		     case EL_DEFAULT_SHORT_WIDTH:  
			return ((Opcode) L_H_C1_C1);
		     default:
			El_punt("get_load_opcode_for_operand: no load for specified width"); 
		}
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return ((Opcode) FL_S_C1_C1);
		else if ( operand.dtype().is_double() ) return ((Opcode) FL_D_C1_C1);
		else El_punt("get_load_opcode_for_operand: wacky float sizes not supported yet");
	    case EL_DT_INFO_BRANCH:
	        El_punt("get_load_opcode_for_operand: no load for branch tgt");
	        break;
            case EL_DT_INFO_PREDICATE:
	        El_punt("get_load_opcode_for_operand: no load for predicate");
	        break;
	    default:
	        El_punt("get_load_opcode_for_operand: uknown data type!");
	}
    }
    else 
	El_punt("get_load_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}


// * Assume C2/C1 cache specifiers for this function
// * At this time do not account for signed/unsigned, needs to be fixed
//   for the future.
Opcode get_C2_load_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	int upwidth = operand.dtype().upconv_width();
	switch (dti) {
	    case EL_DT_INFO_INT:
	    case EL_DT_INFO_POINTER:
		switch( upwidth ) {
		     case EL_DEFAULT_INT_WIDTH: 
			return ((Opcode) L_W_C2_C1);
		     case EL_DEFAULT_CHAR_WIDTH: 
			return ((Opcode) L_B_C2_C1);
		     case EL_DEFAULT_SHORT_WIDTH:  
			return ((Opcode) L_H_C2_C1);
		     default:
			El_punt("get_load_opcode_for_operand: no load for specified width"); 
                        break;
                }
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return ((Opcode) FL_S_C2_C1);
		else if ( operand.dtype().is_double() ) return ((Opcode) FL_D_C2_C1);
		else El_punt("get_load_opcode_for_operand: wacky float sizes not supported yet");
	    case EL_DT_INFO_BRANCH:
	        El_punt("get_load_opcode_for_operand: no load for branch tgt");
	        break;
            case EL_DT_INFO_PREDICATE:
	        El_punt("get_load_opcode_for_operand: no load for predicate");
	        break;
	    default:
	        El_punt("get_load_opcode_for_operand: uknown data type!");
	}
    }
    else 
	El_punt("get_load_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}

// Again, create C1 versions only
Opcode get_store_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	int upwidth = operand.dtype().upconv_width();
	switch (dti) {
	    case EL_DT_INFO_INT:
	    case EL_DT_INFO_POINTER:
		switch( upwidth ) {
		     case EL_DEFAULT_INT_WIDTH: 
			return ((Opcode) S_W_C1);
		     case EL_DEFAULT_CHAR_WIDTH: 
			return ((Opcode) S_B_C1);
		     case EL_DEFAULT_SHORT_WIDTH:  
			return ((Opcode) S_H_C1);
		     default:
			El_punt("get_load_opcode_for_operand: no load for specified width"); 
		}
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return ((Opcode) FS_S_C1);
		else if ( operand.dtype().is_double() ) return ((Opcode) FS_D_C1);
		else El_punt("get_store_opcode_for_operand: wacky float sizes not supported yet");
            case EL_DT_INFO_BRANCH:
                El_punt("get_store_opcode_for_operand: no store for branch tgt");
                break;
            case EL_DT_INFO_PREDICATE:
                El_punt("get_store_opcode_for_operand: no store for predicate");
                break;
            default:
                El_punt("get_store_opcode_for_operand: uknown data type!");
        }
    }
    else
        El_punt("get_store_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}
// Again, create C2 versions only
Opcode get_C2_store_opcode_for_operand(Operand &operand)
{
    if (operand.is_reg() || operand.is_macro_reg()) {
	DType_info dti;
        dti = operand.dtype().info();
	int upwidth = operand.dtype().upconv_width();
	switch (dti) {
	    case EL_DT_INFO_INT:
	    case EL_DT_INFO_POINTER:
		switch( upwidth ) {
		     case EL_DEFAULT_INT_WIDTH: 
			return ((Opcode) S_W_C2);
		     case EL_DEFAULT_CHAR_WIDTH: 
			return ((Opcode) S_B_C2);
		     case EL_DEFAULT_SHORT_WIDTH:  
			return ((Opcode) S_H_C2);
		     default:
			El_punt("get_load_opcode_for_operand: no load for specified width"); 
		}
	    case EL_DT_INFO_FLOAT:
		if ( operand.dtype().is_single() ) return ((Opcode) FS_S_C2);
		else if ( operand.dtype().is_double() ) return ((Opcode) FS_D_C2);
		else El_punt("get_store_opcode_for_operand: wacky float sizes not supported yet");
            case EL_DT_INFO_BRANCH:
                El_punt("get_store_opcode_for_operand: no store for branch tgt");
                break;
            case EL_DT_INFO_PREDICATE:
                El_punt("get_store_opcode_for_operand: no store for predicate");
                break;
            default:
                El_punt("get_store_opcode_for_operand: uknown data type!");
        }
    }
    else
        El_punt("get_store_opcode_for_operand: unknown operand type");

    return ((Opcode)0);
}

Op *El_generate_move_op(Operand &d, Operand &s, Operand &p)
{
    Opcode opc;
    Op *new_op;

    if (! (d.is_reg() || d.is_macro_reg()))
	El_punt("El_generate_move_op: cannot create move with specifed dest");

    opc = get_move_opcode_for_operand(d);
    new_op = new Op(opc);
    new_op->set_dest(DEST1, d);
    new_op->set_src(SRC1, s);
    new_op->set_src(PRED1, p);

    return (new_op);
}

Op *El_generate_add_op(Operand &d, Operand &s1, Operand &s2, Operand &p)
{
    Opcode opc;
    Op *new_op;

    if (! (d.is_reg() || d.is_macro_reg()))
	El_punt("El_generate_add_op: cannot create add with specifed dest");

    opc = get_add_opcode_for_operand(d);
    new_op = new Op(opc);
    new_op->set_dest(DEST1, d);
    new_op->set_src(SRC1, s1);
    new_op->set_src(SRC2, s2);
    new_op->set_src(PRED1, p);

    return (new_op);
}

Op *El_generate_sub_op(Operand &d, Operand &s1, Operand &s2, Operand &p)
{
    Opcode opc;
    Op *new_op;

    if (! (d.is_reg() || d.is_macro_reg()))
	El_punt("El_generate_sub_op: cannot create sub with specifed dest");

    opc = get_sub_opcode_for_operand(d);
    new_op = new Op(opc);
    new_op->set_dest(DEST1, d);
    new_op->set_src(SRC1, s1);
    new_op->set_src(SRC2, s2);
    new_op->set_src(PRED1, p);

    return (new_op);
}

Op *El_generate_mpy_op(Operand &d, Operand &s1, Operand &s2, Operand &p)
{
    Opcode opc;
    Op *new_op;

    if (! (d.is_reg() || d.is_macro_reg()))
	El_punt("El_generate_mpy_op: cannot create mpy with specifed dest");

    opc = get_mpy_opcode_for_operand(d);
    new_op = new Op(opc);
    new_op->set_dest(DEST1, d);
    new_op->set_src(SRC1, s1);
    new_op->set_src(SRC2, s2);
    new_op->set_src(PRED1, p);

    return (new_op);
}


void El_convert_to_move(Op *op, Operand &d, Operand &s1, Operand &p)
{
    Opcode opc;

    if (! (d.is_reg() || d.is_macro_reg()))
	El_punt("El_convert_to_move: cannot create move with specifed dest");

    opc = get_move_opcode_for_operands(d, s1);
    op->set_opcode(opc);
    op->update_num_operands();
    op->set_dest(DEST1, d);
    op->set_src(SRC1, s1);
    op->set_src(PRED1, p);

    /* update the rdt_attr needed because its src operands are changed */
    Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
    if (rdt_attr != NULL) {
        rdt_attr->set_srcs_conservatively(op);
        rdt_attr->set_dests_conservatively(op);
    }
}

void El_convert_to_bru(Op *op, Operand &s1, Operand &p)
{
    op->set_opcode(BRU);
    op->update_num_operands();
    op->set_src(SRC1, s1);
    op->set_src(PRED1, p);

    /* update the rdt_attr needed because its src operands are changed */
    Reference_dtype_attr *rdt_attr = get_reference_dtype_attr(op);
    if (rdt_attr != NULL) {
        rdt_attr->set_srcs_conservatively(op);
        rdt_attr->set_dests_conservatively(op);
    }
}

bool is_logical_op(Op *op)
{
   return (is_logical_op(op->opcode()));
}

bool is_logical_op(Opcode opc)
{
   if (opc == AND_W ||
           opc == ANDCM_W ||
           opc == NAND_W ||
           opc == NOR_W ||
           opc == OR_W ||
           opc == ORCM_W ||
           opc == XOR_W ||
           opc == XORCM_W ) {
      return true;
   } else {
     return false;
   }
}


// <fbouchez>

bool is_stream_access(Op *op)
{
  return (is_stream_access(op->opcode())) ;
}

bool is_stream_access(Opcode opc)
{
  if (opc == S_PUSH ||
      opc == S_POP ||
      opc == S_PEEK) {
    return true;
  } else {
    return false;
  }
}

// parkhc 02/2007
int opcode_data_width(Op *op)
{
  return (opcode_data_width(op->opcode()));
}

int opcode_data_width(Opcode opc)
{
  if (opc & IR_BYTE)
    return 8;
  else if (opc & IR_HALFWORD)
    return 16;
  else if (opc & IR_QUADWORD)
    return 64;
  else if (opc & IR_DOUBLE)
    return 64;
  else
    return 32;
  
}


// </fbouchez>
