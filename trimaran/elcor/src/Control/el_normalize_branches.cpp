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
//      File:           el_normalize_branches.cpp
//      Author:         Matthai Philipose
//      Created:        June 1996
//      Description:    The "normalize" routines convert to a pure cfg 
//                      representation by removing layout-sensitive branch 
//                      ops from CFG branches (e.g. get rid of the 
//                      DUMMY_BR/BRCT[F]/BRU ops). The "denormalize" routines
//                      convert back into a layout-specific format; 
//                      currently, we use a simple layout algorithm
//
/////////////////////////////////////////////////////////////////////////////
#include "connect.h"
#include "dbg.h"
#include "edge_utilities.h"
#include "el_bb_tools.h"
#include "el_control.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "op.h"
#include "region.h"
#include "edge.h"
#include "attributes.h"
#include "edge_attributes.h"
#include "op_attributes.h"
#include "pred_analysis.h"
#include "operand.h"
#include "el_normalize_branches.h"
#include "reaching_defs_solver.h"
#include "region_utilities.h"
#include "el_control_init.h"


/*************************************************************
            NORMALIZATION
*************************************************************/

void el_normalize_branches(Procedure *proc)
{
  List_set<Basicblock*> proc_bb_set;

  //printf("normalizing branches...\n");  

  /*Extract all the bblocks in the cfg for the procedure*/
  El_form_basic_blocks(proc);
  El_confirm_BB_tiling(proc,proc_bb_set);
  
  //Find use-def information
  //These always go together
  create_local_analysis_info_for_all_hbs_bbs (proc);
  El_do_reaching_defs ((Region*)proc, ANALYZE_ALLREG);
  Reaching_defs_info* rdi = get_reaching_defs_info ((Region*)proc);
  
  
  /*Normalize the branch at the end of each bblock*/
  el_normalize_bblocks(proc_bb_set,rdi);

  //get rid of all the analysis info
  remove_reaching_defs_info((Region*)proc);
  // delete vr_maps and PQS  
  delete_local_analysis_info_for_all_hbs_bbs(proc);
  
}


//return the normalized branch predicate condition
//
int el_normalize_br_opcode(IR_BASE_OPCODE pred)
{
  
  switch(pred){
  case BASE_CMPP_W_FALSE: return (int)COND_W_FALSE;
  case BASE_CMPP_W_EQ: return (int)COND_W_EQ;
  case BASE_CMPP_W_LT: return (int)COND_W_LT;
  case BASE_CMPP_W_LEQ: return (int)COND_W_LEQ;
  case BASE_CMPP_W_GT: return (int)COND_W_GT;
  case BASE_CMPP_W_GEQ: return (int)COND_W_GEQ;
  case BASE_CMPP_W_SV: return (int)COND_W_SV;
  case BASE_CMPP_W_OD: return (int)COND_W_OD;
  case BASE_CMPP_W_TRUE: return (int)COND_W_TRUE;
  case BASE_CMPP_W_NEQ: return (int)COND_W_NEQ;
  case BASE_CMPP_W_LLT: return (int)COND_W_LLT;
  case BASE_CMPP_W_LLEQ: return (int)COND_W_LLEQ;
  case BASE_CMPP_W_LGT: return (int)COND_W_LGT;
  case BASE_CMPP_W_LGEQ: return (int)COND_W_LGEQ;
  case BASE_CMPP_W_NSV: return (int)COND_W_NSV;
  case BASE_CMPP_W_EV: return (int)COND_W_EV;
    
  case BASE_FCMPP_S_FALSE: return (int)COND_S_FALSE;
  case BASE_FCMPP_S_EQ: return (int)COND_S_EQ;
  case BASE_FCMPP_S_LT: return (int)COND_S_LT;
  case BASE_FCMPP_S_LEQ: return (int)COND_S_LEQ;
  case BASE_FCMPP_S_GT: return (int)COND_S_GT;
  case BASE_FCMPP_S_GEQ: return (int)COND_S_GEQ;
  case BASE_FCMPP_S_NEQ: return (int)COND_S_NEQ;
  case BASE_FCMPP_S_TRUE: return (int)COND_S_TRUE;

  case BASE_FCMPP_D_FALSE: return (int)COND_D_FALSE;
  case BASE_FCMPP_D_EQ: return (int)COND_D_EQ;
  case BASE_FCMPP_D_LT: return (int)COND_D_LT;
  case BASE_FCMPP_D_LEQ: return (int)COND_D_LEQ;
  case BASE_FCMPP_D_GT:  return (int)COND_D_GT;
  case BASE_FCMPP_D_GEQ: return (int)COND_D_GEQ;
  case BASE_FCMPP_D_NEQ: return (int)COND_D_NEQ;
  case BASE_FCMPP_D_TRUE: return (int)COND_D_TRUE;

  default:
      El_punt("el_normalize_branches.cpp:predicate opcode not recognized\n");
      return -1;
  }
}

//Given a cond op, return the normalized version (NV) of the 
//associated predicate (compare condition). If the normalized
//version evaluates to true, we take the true edge out of the 
//bblock, otherwise we take the false edge. The NV predicate is
//therefore the same as in the cmpp before the branch out of
//the basic block except:
//(i)When the cmpp is of the 'C' variety (i.e. its output
//   is complemented)
//(ii)When the branch is a BRCF
//In these two cases, the NV predicate is the _complement_
//of the original cmpp predicate
int el_normalize_br_op(Op* cond_op_ptr, Op *branch_op_ptr)
{
  Opcode opc;
  IR_BASE_OPCODE pred; 
  int result_pred;

  //Assert that the cmpp is a U predicate
  assert(is_U_pred(cond_op_ptr,DEST1));

   //Extract the compare condition from the cmpp
  opc=cond_op_ptr->opcode();
  
  //If the cmpp dest is complemented, return the 
  //complement of the pred
  if(is_UC_pred(cond_op_ptr,DEST1))
    opc=get_opposite_cond(opc);

  //BRCFs work when the complement of the predicate is true
  //use
  if(branch_op_ptr->opcode() == BRCF)
    opc=get_opposite_cond(opc);

  //Convert the branch predicate opcode to a 
  //General_Branch_Cond (see operand.h) opcode
  pred=get_base(opc);
  result_pred = el_normalize_br_opcode(pred);
  
  return result_pred;
}

void el_normalize_bblocks(List_set<Basicblock*>& bb_set,Reaching_defs_info* rdi)
{
  Basicblock *cur_bb;
  Op *normalized_br_op_ptr;
  

  //Process each basic block
  for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
    {
      cur_bb=*bb_iter;
      
      Op* cur_op_ptr= el_get_bb_exit_op(cur_bb);
      Opcode br_opcode;

      //Create the appropriate normalized branch op and 
      //clobber the old version
      assert(is_control_switch(cur_op_ptr));
      if (cur_op_ptr->flag(EL_OPER_TABLE_JUMP))
          normalized_br_op_ptr= new Op(BRIND,3,0,true);
      else
          normalized_br_op_ptr= new Op(BRDIR,3,0,true);

      //Set the id and PRED1 of the normalized branch to that of the old
      //one, so it can be restored at denormalization
      normalized_br_op_ptr->set_id(cur_op_ptr->id());
      if(!is_dummy_branch(cur_op_ptr))	
	normalized_br_op_ptr->set_src(PRED1,cur_op_ptr->src(PRED1));
      else
	{
	  Pred_lit* new_pred_lit = new Pred_lit(true);
	  Operand pred_true(new_pred_lit);
	  normalized_br_op_ptr->set_src(PRED1, pred_true);
	}

      
      //Record the attributes of the branch that will be replaced
      el_set_attrs_in_br_info(normalized_br_op_ptr,
			      get_lcode_attributes(cur_op_ptr),
			      BR_INFO_BR);
      
      //For conditional and register branches, need to fill in 
      //source operands 
      br_opcode=cur_op_ptr->opcode();

      // Now handle normalization of the various types of branches case by case

      // unconditional register branches (many targets)
      if ((br_opcode == BRU) && (cur_op_ptr->flag(EL_OPER_TABLE_JUMP)))
        {
	  /* SAM 10-97, do nothing for now, need to fix in the future!!!! */  ;
        }

      //unconditional branches
      else if (br_opcode == BRU)
        {

	  //Delete the pbr op (remember the id, attrs and target operand though) if
          // it has just one use, otherwise keep around a ptr to it
	  //Op* pbr_op_ptr=El_new_find_pbr_for_branch(cur_op_ptr,rdi);
          // 1 branch could have multiple pbrs. -KF 12/2004
          List<Op*> pbr_list;
          El_new_find_pbrs_for_branch(cur_op_ptr,rdi, pbr_list);
          assert(pbr_list.size() == 1);
          Op* pbr_op_ptr = pbr_list.head();
	  if (el_op_has_exactly_one_use(pbr_op_ptr,rdi))
	    {
	      //record the id of the pbr
	      el_set_id_in_br_info(normalized_br_op_ptr,pbr_op_ptr->id(),BR_INFO_PBR);
	      //record the attributes of the pbr
	      el_set_attrs_in_br_info(normalized_br_op_ptr,
				      get_lcode_attributes(pbr_op_ptr),
				      BR_INFO_PBR);
	      //record the dest_op of the pbr
	      el_set_oper_in_br_info(normalized_br_op_ptr,
				     &(pbr_op_ptr->dest(DEST1)),
				     BR_INFO_PBR);

	      El_remove_op(pbr_op_ptr);
	    }
	  else
	    {
	      el_set_op_in_br_info(normalized_br_op_ptr,pbr_op_ptr,BR_INFO_PBR);
	    }
	  //replace the BRU with a BRDIR
	  El_replace_op(cur_bb,cur_op_ptr,normalized_br_op_ptr);
	}

      // dummy branches
      else if (br_opcode == DUMMY_BR)
        {
	  //replace the DUMMY_BR with a BRDIR
	  El_replace_op(cur_bb,cur_op_ptr,normalized_br_op_ptr);

	  //in the normalized form, the conventionis for the non-
	  //predicate-fallthrugh sides of unconditional brdirs have
	  //alt number 1
	  int db_count=0;
	  for(Region_exit_edges dbe_iter(normalized_br_op_ptr);dbe_iter!=0;dbe_iter++,db_count++)
	    {
	      Edge *edge=*dbe_iter;
	      edge->set_src_alt((Alt_num)1);
	    }
	  assert(db_count==1);
	}

      //conditional branches
      else if ((br_opcode == BRCT) || (br_opcode == BRCF)) 
	{
	  Op* cond_op_ptr = El_new_get_cmpp_for_branch(cur_op_ptr,rdi);
	  assert(is_cmpp(cond_op_ptr));
	      
	  //extract the opcode and source operands from the cmpp
	  Operand cond_opcode_lit = new Int_lit(el_normalize_br_op(cond_op_ptr, cur_op_ptr));
	  //This is the dest actually used by the branch following the cmpp
	  Port_num br_pred_port = el_find_br_pred_port(cond_op_ptr,cur_op_ptr,rdi);
	  
	  //copy/set the various operands/attributes from the ops
	  //being normalized to the normalized op being constructed
	  el_set_oper_in_br_info(normalized_br_op_ptr,&(cond_op_ptr->dest(br_pred_port)),BR_INFO_CMPP);
	  el_set_attrs_in_br_info(normalized_br_op_ptr,get_lcode_attributes(cond_op_ptr),BR_INFO_CMPP);
	  normalized_br_op_ptr->set_src(SRC1,cond_opcode_lit);
	  normalized_br_op_ptr->set_src(SRC2,cond_op_ptr->src(SRC1));
	  normalized_br_op_ptr->set_src(SRC3,cond_op_ptr->src(SRC2));
	      
	  //delete the cmpp, if its second predicate register 
	  //is not being used...
	  if((cond_op_ptr->dest(DEST1).is_undefined())||
	     (cond_op_ptr->dest(DEST2).is_undefined()))
	    {
	      //remember the id of the cmpp so it can be restored
	      el_set_id_in_br_info(normalized_br_op_ptr,cond_op_ptr->id(),BR_INFO_CMPP);
	      El_remove_op(cond_op_ptr);
	    }
	  else
	    {
	      //otherwise, just clobber the dest originally used 
	      //in the branch, since it will not be used, and remember
	      //the cmpp 
	      el_set_op_in_br_info(normalized_br_op_ptr,cond_op_ptr,BR_INFO_CMPP);
	      //cdbg << "warning: need to clobber operand not used by new brdir: use ud\n";
	    }

	  //Op* pbr_op_ptr=El_new_find_pbr_for_branch(cur_op_ptr,rdi);
          // 1 branch could have multiple pbrs. -KF 12/2004
          List<Op*> pbr_list;
          El_new_find_pbrs_for_branch(cur_op_ptr,rdi, pbr_list);
          assert(pbr_list.size() == 1);
          Op* pbr_op_ptr = pbr_list.head();
	  if (el_op_has_exactly_one_use(pbr_op_ptr,rdi))
	    {
	      //delete the pbr operator after recording its id, dest operand and attributes
	      //for denormalization
	      el_set_oper_in_br_info(normalized_br_op_ptr,&(pbr_op_ptr->dest(DEST1)),BR_INFO_PBR);
	      el_set_attrs_in_br_info(normalized_br_op_ptr,get_lcode_attributes(pbr_op_ptr),BR_INFO_PBR);
	      el_set_id_in_br_info(normalized_br_op_ptr,pbr_op_ptr->id(),BR_INFO_PBR);
	      El_remove_op(pbr_op_ptr);
	    }
	  else
	    { 
	      //just store a ptr to the pbr
	      el_set_op_in_br_info(normalized_br_op_ptr,pbr_op_ptr,BR_INFO_PBR);
	    }

	  //replace the BRC[T|F] with BRDIR
	  El_replace_op(cur_bb,cur_op_ptr,normalized_br_op_ptr);
	      
	}
	    
      else if (br_opcode == RTS)
        {
	  /* do nothing, RTS's are not converted to normalized form */  ;
        }
      else
	{
	  assert(0);
        }
    }
} 

/*************************************************************
            DE-NORMALIZATION
*************************************************************/

//Given a cfg with BRDIRs and BRINDs, convert back to 
//a format with BRCT/BRCF/BRU/DUMMY_BR;
//In order to insert the DUMMY_BRes, we essentially 
//have to add a code-layout pass.
//A BRDIR can be converted into an associated PBR, CMPP
//and BRCT; the complications here are:
// (i)If a CMPP computing the branch condition already
//    exists (the normalization process may have left
//       it intact because its second target pred is used
//       elsewhere), we would like to re-use it.
//(ii)If the BRCT is a re-incarnation of an old one
void el_denormalize_branches(Procedure *proc)
{
  List_set<Basicblock*> proc_bb_set;
  //cdbg << "DE-normalizing branches...\n";
  
  /*Extract all the bblocks in the cfg for the procedure*/
  El_form_basic_blocks(proc);
  El_confirm_BB_tiling(proc,proc_bb_set);
  
  /*De-normalize the branch at the end of each bblock*/
  el_denormalize_bblocks(proc_bb_set);
}


//return the normalized branch predicate condition
IR_BASE_OPCODE el_denormalize_br_opcode(int pred)
{
  
  switch((General_Branch_Cond)pred){
  case COND_W_FALSE: return BASE_CMPP_W_FALSE;
  case COND_W_EQ: return BASE_CMPP_W_EQ;
  case COND_W_LT: return BASE_CMPP_W_LT;
  case COND_W_LEQ: return BASE_CMPP_W_LEQ;
  case COND_W_GT: return BASE_CMPP_W_GT;
  case COND_W_GEQ: return BASE_CMPP_W_GEQ;
  case COND_W_SV: return BASE_CMPP_W_SV;
  case COND_W_OD: return BASE_CMPP_W_OD;
  case COND_W_TRUE: return BASE_CMPP_W_TRUE;
  case COND_W_NEQ: return BASE_CMPP_W_NEQ;
  case COND_W_LLT: return BASE_CMPP_W_LLT;
  case COND_W_LLEQ: return BASE_CMPP_W_LLEQ;
  case COND_W_LGT: return BASE_CMPP_W_LGT;
  case COND_W_LGEQ: return BASE_CMPP_W_LGEQ;
  case COND_W_NSV: return BASE_CMPP_W_NSV;
  case COND_W_EV: return BASE_CMPP_W_EV;
    
  case COND_S_FALSE: return BASE_FCMPP_S_FALSE;
  case COND_S_EQ: return BASE_FCMPP_S_EQ;
  case COND_S_LT: return BASE_FCMPP_S_LT;
  case COND_S_LEQ: return BASE_FCMPP_S_LEQ;
  case COND_S_GT: return BASE_FCMPP_S_GT;
  case COND_S_GEQ: return BASE_FCMPP_S_GEQ;
  case COND_S_NEQ: return BASE_FCMPP_S_NEQ;
  case COND_S_TRUE: return BASE_FCMPP_S_TRUE;

  case COND_D_FALSE: return BASE_FCMPP_D_FALSE;
  case COND_D_EQ: return BASE_FCMPP_D_EQ;
  case COND_D_LT: return BASE_FCMPP_D_LT;
  case COND_D_LEQ: return BASE_FCMPP_D_LEQ;
  case COND_D_GT:  return BASE_FCMPP_D_GT;
  case COND_D_GEQ: return BASE_FCMPP_D_GEQ;
  case COND_D_NEQ: return BASE_FCMPP_D_NEQ;
  case COND_D_TRUE: return BASE_FCMPP_D_TRUE;

  default:
    El_punt("el_normalize_branches.cppa:predicate opcode not recognized\n");
    return BASE_FCMPP_D_TRUE; //keep gnu compiler happy
  }
}

//a simple code layout algo... it is not used right now,
//but may be a good base to start with, for a real layout algo
void el_layout_bblocks(Hash_set<Basicblock*> bb_set)
{
  Basicblock *cur_bb;
  Hash_set<Basicblock*> laid_out_bb_set(hash_bb_ptr, 128);

  //Process each basic block
  for(Hash_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
    {
      cur_bb=*bb_iter;
      //Need to ensure that current basic block is laid out 
      //at the end of the loop body 
     
      //cout << "bb is:\n" << *cur_bb;

      Op* cur_op_ptr= el_get_bb_exit_op(cur_bb);
      Opcode br_opcode=cur_op_ptr->opcode();
      
      laid_out_bb_set += cur_bb;

      switch(br_opcode){
	    
	//Convert direct branches to BRUs,BRCTs & BRCT/BRU combos
      case BRDIR:

	//case 1: An unconditional BRDIR
	if(is_unconditional_branch(cur_op_ptr))
	  {
	    //need to deal with 2-exit brus (see below)
	    assert(0);

	    //If the succeeding bblock is already laid out, 
	    //replace BRDIR with BRU
	    Basicblock *succ_bb=el_single_successor_bb(cur_bb);
	    if(laid_out_bb_set.is_member(succ_bb))
	      {
		el_denormalize_br_op(cur_op_ptr,UNCOND_BRU,succ_bb);
	      }
	    //...else replace BRDIR with, DUMMY_BR 
	    //and mark the successor laid out
	    else
	      {
		laid_out_bb_set += succ_bb;
		el_denormalize_br_op(cur_op_ptr,UNCOND_DUMMY,succ_bb);
	      }
	  }
	    
	//case 2: A conditional BRDIR
	else
	  {
	    Basicblock *true_succ=el_true_successor_bb(cur_bb);
	    Basicblock *false_succ=el_false_successor_bb(cur_bb);

	    //If the false successor is not laid out,
	    //   insert a BRCT, and mark it laid out...
	    if(!laid_out_bb_set.
	       is_member(false_succ))
	      {
		laid_out_bb_set += false_succ;
		el_denormalize_br_op(cur_op_ptr,COND_BRCT,true_succ);
	      }
	    //...else if the true successor is not laid out,
	    //    make it the false successor (by changing 
	    //    the br condition) insert a BRCT, and mark 
	    //    it laid out...
	    else 
	      if(!laid_out_bb_set.
		 is_member(true_succ))
		{
		  //cdbg << "Need to flip exists\n";
		  assert(0);
		  laid_out_bb_set += true_succ;		  
		  el_denormalize_br_op(cur_op_ptr,COND_BRCF,false_succ);		
		}
	    //...else (both successors laid out)
	    //insert a BRCT with a BRU leading to the false
	    //side of the BRDIR
	      else
		el_denormalize_br_op(cur_op_ptr,COND_BRCT_BRU,true_succ);
	  }
	break;
	    
      case BRIND:
	assert(0);//no indirect calls for now
	break;

	//subroutine calls and return statements are not changed
      case BRL:
      case RTS:
	break;
	    
      default:
	assert(0);
      }	  
    }
} 

//a simpler code layout algo, based on the order that already
//exists in the subregions dlist
void el_denormalize_bblocks(List_set<Basicblock*>& bb_set)
{
  Basicblock *cur_bb;
  Hash_set<Basicblock*> laid_out_bb_set(hash_bb_ptr, 128);


  //Process each basic block
  for(List_set_iterator<Basicblock*> bb_iter(bb_set); bb_iter != 0; bb_iter++)
    {
      cur_bb=*bb_iter;
      //Need to ensure that current basic block is laid out 
      //at the end of the loop body 
     
      //cout << "[BEFORE DENORM:] bb is:\n" << *cur_bb;

      Op* cur_op_ptr= el_get_bb_exit_op(cur_bb);
      Opcode br_opcode=cur_op_ptr->opcode();
      
      laid_out_bb_set += cur_bb;

      switch(br_opcode){
	    
	//Convert direct branches to BRUs,BRCTs & BRCT/BRU combos
      case BRDIR:

	//case 1: An unconditional BRDIR
	if(is_unconditional_branch(cur_op_ptr))
	  {
	    //unconditional brdirs can have two exit edges,
	    //if they are predicated...
	    if(el_num_region_successors(cur_op_ptr)== 2)
	      {
#if 1
		Basicblock *pred_false_succ=el_predicate_false_successor_bb(cur_bb);
#else
		Op *exit_op = el_get_bb_exit_op(cur_bb);
		Basicblock *pred_false_succ = (Basicblock *) get_outgoing_CONTROL0_fall_through_edge(exit_op)->dest()->parent();
#endif
		//we're guaranteed (during normalization) that the non-predicate
		//fallthrough edge is labelled alt 1
		Basicblock *true_succ=el_true_successor_bb(cur_bb);
		
		//We assume that the false successor of a predicted BRU is
		//laid out right after the bru; right now the only module
		//that produces predicated brus is the if-converter; if your module
		//violates this assumption and barfs at this assert, talk to Matthai/
		//Scott
		assert(el_compound_region_succeeds_in_dlist(pred_false_succ,cur_bb));
		
		//Put in a two-exit bru with the edges pointing the same
		//way as the unconditional brdir it's replacing 
		laid_out_bb_set += pred_false_succ;
		el_denormalize_br_op(cur_op_ptr,COND_BRU,true_succ);
	      }
	    else
	      {
		assert(el_num_region_successors(cur_op_ptr)== 1);
		//If the succeeding bblock is already laid out, 
		//doesn't follow the current bblock in the dlist,
		//put in a BRU...
		Basicblock *succ_bb=el_single_successor_bb(cur_bb);
		if(!el_compound_region_succeeds_in_dlist(succ_bb,cur_bb))
		  {
		    el_denormalize_br_op(cur_op_ptr,UNCOND_BRU,succ_bb);
		  }
		//...else replace BRDIR with, DUMMY_BR 
		//and mark the successor laid out
		else
		  {
		    laid_out_bb_set += succ_bb;
		    el_denormalize_br_op(cur_op_ptr,UNCOND_DUMMY,succ_bb);
		  }
	      }
	  }
	
	//case 2: A conditional BRDIR
	else
	  {
	    if(el_num_region_successors(cur_op_ptr)== 3)
	      {
		//if a conditional branch has a three successors,
		//they are the true and false successors, and the
		//predicate-false successor (i.e. the basic block
		//to which control flows if the branch inst doesn't
		//execute; further the pred_false_succ bb is different
		//from the true and false bbs
		Basicblock* pred_false_succ=el_predicate_false_successor_bb(cur_bb);
		Basicblock *true_succ=el_true_successor_bb(cur_bb);

		//The pred_false bblock should already be the fall-through
		//(right now, only the if-converter produces pred_false bblocks,
		//and it always makes sure the fall-through is in the right place
		//in the dlist
		assert(el_compound_region_succeeds_in_dlist(pred_false_succ,cur_bb));

		//Insert the brct and the bru to point to the true and false succs
		//The brct/bru should be predicated to the same pred register as
		//cur_op_ptr
		el_denormalize_br_op(cur_op_ptr,COND_BRCT_BRU,true_succ);
	      }
	    else
	      {
		//a conditional branch better have at least two successors!
		assert(el_num_region_successors(cur_op_ptr)== 2);

		Basicblock *true_succ=el_true_successor_bb(cur_bb);
		Basicblock *false_succ=el_false_successor_bb(cur_bb);
		
		//If the false successor is laid out after cur_bb,
		//   insert a BRCT, and mark it laid out...
		Compound_region* hb_parent=cur_bb->parent();
		Op* hb_last_op = get_last_region_op_from_subregions(hb_parent);
		
		//The branch at the bottom of an hb gets special treatment...
		if((hb_parent->is_hb())&&
		   (cur_op_ptr==hb_last_op)&&
		   El_frp_model &&   //frp is on
		   El_enable_branch_reorder)
		  {
		    //if branch reordering is enabled, always gnerate a brct
		    //and a bru at the bottom of a hb, so that both outedges can be
		    //moved up
		    el_denormalize_br_op(cur_op_ptr,COND_BRCT_BRU,true_succ);
		  }
		else
		  {
		    if(el_compound_region_succeeds_in_dlist(false_succ,cur_bb))
		      {
			laid_out_bb_set += false_succ;
			el_denormalize_br_op(cur_op_ptr,COND_BRCT,true_succ);
		      }
		    //...else if the true successor is laid out after cur_bb,
		    //    make it the false successor (by changing 
		    //    the br condition) insert a BRCT, and mark 
		    //    it laid out...
		    else 
		      if(el_compound_region_succeeds_in_dlist(true_succ,cur_bb))
			{
			  //cdbg << "Need to flip exists\n";
			  laid_out_bb_set += true_succ;		  
			  el_denormalize_br_op(cur_op_ptr,COND_BRCF,false_succ);		
			}
		    //...else (both successors laid out)
		    //insert a BRCT with a BRU leading to the false
		    //side of the BRDIR
		      else
			el_denormalize_br_op(cur_op_ptr,COND_BRCT_BRU,true_succ);
		  }
	      }
	  }
	break;
	    
      case BRIND:
	assert(0);//no indirect calls for now
	break;

	//subroutine calls and return statements are not changed
      case BRL:
      case RTS:
      case BRU:	/* Temporary while jump_rgs are not normalized!! */
	break;
	    
      default:
	assert(0);
      }	 
 
      //cout << "[AFTER DENORM:] bb is:\n" << *cur_bb;
    }
} 

//Replace the normalized form of the branch with a denormalized
//form; the denormalized form required is indicated by the second
//arg
void el_denormalize_br_op(Op *op,BR_REPLACE_TYPE br, Basicblock *dest_bb)
{
  bool is_brcf=false;
  bool is_brct_bru=false;
  bool is_cond_bru=false;
  
  switch(br){
  case COND_BRU:
    //cout<<"COND_BRU\n";
    is_cond_bru=true;
  case UNCOND_BRU:
    {
      //cout<<"UNCOND_BRU\n";

      Basicblock *cur_bb= (Basicblock*)(op->parent());
      Op *pbr_op= el_get_op_from_br_info(op,BR_INFO_PBR);
      Op* bru_op;

      //Insert the pbr that was eliminated during normalization,
      //if necessary
      if(!el_op_exists_in_br_info(op,BR_INFO_PBR))
	{
	  int pred_bit = el_find_branch_prediction_bit(op,BRU);	  
	  pbr_op= el_insert_pbr(op,dest_bb,pred_bit);
	}

      //Put in the branch after the pbr (clobber the
      //brdir in the process)
      bru_op=el_insert_bru(op,pbr_op);
      
      if((el_num_region_successors(bru_op)==1)&&
	 (!((bru_op->src(PRED1)).is_predicate_true())))
	{
	  Hyperblock* parent_hb=(Hyperblock*)cur_bb->parent();
	  assert(parent_hb->is_hb());//a predicated bru better be in a hb
	  
	  Op* last_op = get_last_region_op_from_subregions(parent_hb);
	  if(bru_op==last_op)
	    parent_hb->set_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU);
	}

      //only fall throughs should have 0 alt numbers
      if(!is_cond_bru)
	el_set_single_exit_alt(bru_op,1);
      else
	el_set_pred_false_exit_alt(bru_op,0);

      //Insert the stop op for frp-ized hyperblocks if needed
      Compound_region* hb_parent=cur_bb->parent();
      Op* hb_last_op = get_last_region_op_from_subregions(hb_parent);
      if((hb_parent->is_hb())&&
	 (hb_parent->flag(EL_REGION_HYPERBLOCK_FRP))&&
	 (el_C0_hb_successor_op(hb_last_op) != NULL))
	{
	  el_append_hb_stop_op(cur_bb);
	}
      
      break;
    }
  case UNCOND_DUMMY:
    {
      //cout<<"UNCOND_DUMMY\n";
      Op *ud_op= new Op(DUMMY_BR);
      Basicblock *cur_bb= (Basicblock*)(op->parent());
      
      ud_op->set_id(op->id());
      //set_lcode_attributes(op,el_get_attrs_from_br_info(op,BR_INFO_BR));
      // Setting attributes for the op you are replacing?  This looks wrong.
      // Plus the attributes need to be cloned.  -KF 2/2006
      set_lcode_attributes(ud_op,
        new Lcode_attribute_map(*el_get_attrs_from_br_info(op,BR_INFO_BR)));
      El_replace_op(cur_bb,op,ud_op);
      //fall throughs should have 0 alt numbers
      el_set_single_exit_alt(ud_op,0);

      break;
    }
  case COND_BRCT_BRU:
    {
      //Put in the BRCT (handled by the case COND_BRCT below)
      //and then add a BRU on the false (fallthrough) side
      //cout<<"COND_BRCT_BRU\n";
      is_brct_bru=true;      
    }

  case COND_BRCF:
    {
      //Set the flag, so that the branch can be treated as a brct,
      //except that the predicate of its cmpp is reversed and the
      //alt_numbers of its edges flipped
      if(!is_brct_bru)is_brcf=true;
    }

  case COND_BRCT:
    {
      //cout<<"COND_BRCT\n";

      Basicblock* cur_bb=(Basicblock*)op->parent();
      Operand *cmpp_oper;

      //Get the location of pbrs and cmpp ops for the branch
      //(in case normalization didn't get rid of these)
      Op *pbr_op= el_get_op_from_br_info(op,BR_INFO_PBR);
      Op *cmpp_op= el_get_op_from_br_info(op,BR_INFO_CMPP);


      //Insert the pbr that was eliminated during normalization,
      //if necessary
      if(!el_op_exists_in_br_info(op,BR_INFO_PBR))
	{
	  assert(pbr_op==NULL);
	  int pred_bit = el_find_branch_prediction_bit(op,BRCT);	  
	  pbr_op= el_insert_pbr(op,dest_bb,pred_bit);
	}

      //Insert the cmpp that was eliminated during normalization,
      //if necessary
      if(!el_op_exists_in_br_info(op,BR_INFO_CMPP))
	{
	  int id;
	  assert(cmpp_op == NULL);
	  cmpp_op= el_insert_cmpp(op);
	  if((id=el_get_id_from_br_info(op,BR_INFO_CMPP))!=-1)
	    cmpp_op->set_id(id);
	  cmpp_oper= &(cmpp_op->dest(DEST1));
	}
      else 
	//we're using  cmpp left behind by normalization; 
	//fill in the UNDEFINED dest
	{
	  cmpp_oper= el_get_oper_from_br_info(op,BR_INFO_CMPP);
	  if (cmpp_op->dest(DEST1).is_undefined())
	    cmpp_op->set_dest(DEST1,*cmpp_oper);
	  else
	    {
	      //cdbg << "reminder: assert that dest2 is undefined when normalization clobbers arg2\n";
	      cmpp_op->set_dest(DEST2,*cmpp_oper);
	    }
	}

      //Replace brdir with brct
      Op* brct_op=el_insert_brct(op,pbr_op,*cmpp_oper);
      
      
      if(is_brcf)
	{
	  //Flip cmpp predicate and alt_numers
	  cmpp_op->set_opcode(get_opposite_cond(cmpp_op->opcode()));
	  el_flip_exit_alt_numbers(cur_bb);
	}

      Basicblock* new_bru_bb=NULL;
      if(is_brct_bru)
	{
	  //put in a BRU to the target of the false side
	  //the fall through of the BRU (if it is predicated
	  //and a fall through also needs to be set)
	  Edge* p_false_edge=(Edge*)NULL;
	  if(el_num_region_successors(brct_op)==3)
	    p_false_edge=el_predicate_false_successor_edge(cur_bb);
	  new_bru_bb=el_insert_bru_bb_on_false_exit(brct_op,p_false_edge);
	}
      
      //Insert the stop op for frp-ized hyperblocks if needed
      Compound_region* hb_parent=cur_bb->parent();
      Op* hb_last_op = get_last_region_op_from_subregions(hb_parent);
      
      if((hb_parent->is_hb())&&
	 (hb_parent->flag(EL_REGION_HYPERBLOCK_FRP))&&
	 (el_C0_hb_successor_op(hb_last_op) != NULL)) //stop op is inserted only if a fallthru op exists	
	{
	  el_append_hb_stop_op(is_brct_bru ? new_bru_bb : cur_bb);
	}
      break;
    }
  default:
    break;
  }
  
  
}

void  el_append_hb_stop_op(Basicblock* cur_bb)
{
  
  if(!El_enable_branch_reorder)
    return;

  //Stop ops are only for hyperblocks!
  Hyperblock* parent_hb=(Hyperblock*)cur_bb->parent();
  assert(parent_hb != NULL);

  Op* hb_last_op = get_last_region_op_from_subregions(parent_hb);
  Op* bb_last_op = get_last_region_op_from_subregions(cur_bb);

  //Stop ops go only in the last bb of the hb
  if(hb_last_op==bb_last_op)
    {
      //create the stop op (it's just a dummy branch)
      Op *stop_op= new Op(DUMMY_BR);
      stop_op->set_parent(cur_bb);//The stop op goes in the current bb
      cur_bb->add_region(stop_op);

      //connect the stop op to the current last op
      Edge* new_edge=C0_connect(hb_last_op,stop_op);
      new_edge->set_src_alt(0);

      Edge* fallthru_edge=el_false_exit_edge(cur_bb);
      if(fallthru_edge)
	{
	  //if there's a fallthru edge, just reconnect it 
	  //so its source is the new stop op 
	  hb_last_op->remove_outop_recursively(fallthru_edge);
	  hb_last_op->remove_outedge_recursively(fallthru_edge);
	  
	  fallthru_edge->set_src(stop_op,fallthru_edge->src_port(),
				 fallthru_edge->src_alt());
	  
	  stop_op->add_outop_recursively(fallthru_edge);
	  stop_op->add_outedge_recursively_restricted(fallthru_edge);
	}
      else
	//no fallthru currently exists; we need to create one and
	//attach it to the beginning of the block succeeding cur_bb
	//in the dlist
	{
	  Op *C0_succ=el_C0_hb_successor_op(hb_last_op);
	  fallthru_edge=C0_connect(stop_op,C0_succ);
	  fallthru_edge->set_src_alt(0);
	  Control_flow_freq *cfreq = new Control_flow_freq();
	  cfreq->freq =0.0;
	  set_control_flow_freq(fallthru_edge, cfreq);
	  
	  stop_op->add_outop_recursively(fallthru_edge);
	  stop_op->add_outedge_recursively_restricted(fallthru_edge);
	  C0_succ->add_inedge_recursively_restricted(fallthru_edge);
	  
	  
	  //the parent hb is no longer a no-fallthrough
	  //hb
	  parent_hb->reset_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU);
	}
    }
}

 
Op* el_C0_hb_successor_op(Op *op)
{
  Hyperblock* parent_hb=(Hyperblock*)op->parent()->parent();
  assert(parent_hb->is_hb());//this function works only for ops in hbs
  //Find the successor of the hb in the dlist
  Compound_region* next_reg=(Compound_region*)parent_hb->parent()->successor_in_subregion_list(parent_hb);
  if(next_reg)
    return get_first_region_op_from_subregions(next_reg);
  else      
    return NULL;  
}

void el_set_single_exit_alt(Op* bru_op,Alt_num val)
{
  int count=0;
  
  for (Region_exit_edges exit_iter(bru_op->parent());
       exit_iter!=0; 
       exit_iter++,count++)
    {
      Edge *edge=*exit_iter;
      edge->set_src_alt(val);
    }
  assert(count==1);
  return;
  
}

void el_set_pred_false_exit_alt(Op* bru_op,Alt_num val)
{
  for (Region_exit_edges exit_iter(bru_op->parent());
       exit_iter!=0; 
       exit_iter++)
    {
      Edge *edge=*exit_iter;
      if(edge->src_alt()==2)
	edge->set_src_alt(val);
    }

  return;
  
}

Basicblock* el_single_successor_bb(Basicblock* bb){
  Basicblock* succ_bb = NULL;
  int succ_count=0;

  for (Region_exit_edges exit_iter(bb);
       exit_iter!=0; 
       exit_iter++)
    {
      succ_count++;
      succ_bb = (Basicblock*) ((*exit_iter)->dest()->parent());
    }
  assert(succ_count==1);
  return succ_bb; 
}

//Applies to brdirs with three successors; this one
//returns the successor that's hit when the op predicate
//evaluates to false
Basicblock* el_predicate_false_successor_bb(Basicblock* bb)
{
  Basicblock* succ_bb = NULL;
  int succ_count=0;
  Op* op=el_get_bb_exit_op(bb);

  for (Region_exit_edges exit_iter(bb); 
       exit_iter!=0; 
       exit_iter++)
    {
      Edge* edge = (*exit_iter);
      if(edge->src_alt() == 2)
	{
	  op=edge->src();
	  succ_bb = (Basicblock*) (edge->dest()->parent());
	}
      succ_count++;
    }
  assert(!((op->src(PRED1)).is_predicate_true()));
  return succ_bb;
}

Edge* el_predicate_false_successor_edge(Basicblock* bb)
{
  Edge* pf_edge=(Edge*)NULL;
  int succ_count=0;
  Op* op=el_get_bb_exit_op(bb);

  for (Region_exit_edges exit_iter(bb); 
       exit_iter!=0; 
       exit_iter++)
    {
      Edge* edge = (*exit_iter);
      if(edge->src_alt() == 2)
	{
	  pf_edge=edge;
	}
      succ_count++;
    }
  assert(pf_edge&&(succ_count==3)&&(!((op->src(PRED1)).is_predicate_true())));
  return pf_edge;
}

Basicblock* el_true_successor_bb(Basicblock* bb){
  Basicblock* succ_bb=(Basicblock*)NULL;
  int succ_count=0;
  Op* op=el_get_bb_exit_op(bb);

  for (Region_exit_edges exit_iter(bb); 
       exit_iter!=0; 
       exit_iter++)
    {
      Edge* edge = (*exit_iter);
      if(edge->src_alt() == 1)
	{
	  op=edge->src();
	  succ_bb = (Basicblock*) (edge->dest()->parent());
	}
      succ_count++;
    }
  assert(succ_count==2||
	 ((succ_count==3)&&(!(op->src(PRED1)).is_predicate_true())));
  return succ_bb;
}

Edge* el_false_exit_edge(Basicblock* bb)
{

  for (  Region_exit_edges exit_iter(bb);
	exit_iter!=0; 
	exit_iter++)
    {
      Edge* edge = (*exit_iter);
      if(edge->src_alt() == 0)
	return edge;
    }

  //There's no alt0 edge
  return (Edge*)NULL;
}

Basicblock* el_false_successor_bb(Basicblock* bb){
  Basicblock* succ_bb = NULL;
  int succ_count=0;
  Op* op=el_get_bb_exit_op(bb);

  for (  Region_exit_edges exit_iter(bb);
	exit_iter!=0; 
	exit_iter++)
    {
      Edge* edge = (*exit_iter);
      if(edge->src_alt() == 0)
	{
	  op=edge->src();
	  succ_bb = (Basicblock*) (*exit_iter)->dest()->parent();
	}
      succ_count++;
    }

  assert(succ_count==2||
	 ((succ_count==3)&&(!(op->src(PRED1)).is_predicate_true())));
  
  return succ_bb;
}

Basicblock *el_insert_bru_bb_on_false_exit(Op* brct_op,Edge* pred_false_edge)
{
  Basicblock* bru_bb= new Basicblock;

  Basicblock* cur_bb=(Basicblock*)brct_op->parent();
  Edge* false_edge=el_false_exit_edge(cur_bb);
  
  Op* tgt_op = false_edge->dest();//Target of the bru jump

  Compound_region* bb_parent_region=cur_bb->parent();
  Op* merge_op;
  
 
  //create and add the new basic block for the bru
  bru_bb->set_parent(bb_parent_region);
  bb_parent_region->insert_after_region(bru_bb,cur_bb);
  
  //get the weight of the bru_bb right: its weight is the sum of the
  //weights of the false edge and the fallthru edge out of the brct
  //preceding it
  Control_flow_freq *cfreq_false = get_control_flow_freq(false_edge);
  Control_flow_freq *cfreq_pred_false = 
    pred_false_edge ? 
    get_control_flow_freq(false_edge): NULL;
  bru_bb->weight=cfreq_false->freq + 
    (pred_false_edge ? cfreq_pred_false->freq : 0);


  //create and add the merge op at the top of the new bb
  merge_op = new Op(C_MERGE) ;
  bru_bb->add_region(merge_op) ;
  merge_op->set_parent(bru_bb) ;
  bru_bb->add_entry(merge_op);

  //create, add and connect the pbr and bru op at the bottom of the new bb
  //both ops should be predicated to the pred register of the branch of
  //the brct_op above; the bru added should jump to tgt_op, the dest of
  //the false branch of the brct
  El_add_predicated_jump_to_region(bru_bb,tgt_op,brct_op->src(PRED1));
  Op* bru_op=el_get_bb_exit_op(bru_bb);
  if((el_num_region_successors(bru_op)==1)&&
     (!((bru_op->src(PRED1)).is_predicate_true())))
    {
      Hyperblock* parent_hb=(Hyperblock*)bru_bb->parent();
      assert(parent_hb->is_hb());//a predicated bru better be in a hb
      
      Op* last_op = get_last_region_op_from_subregions(parent_hb);
      if(bru_op==last_op)	
	parent_hb->set_flag(EL_REGION_HYPERBLOCK_NO_FALLTHRU);
    }

  //Remove the false edge between brct_op and tgt_op
  brct_op->remove_outop_recursively(false_edge);
  brct_op->remove_outedge_recursively(false_edge);

  tgt_op->remove_inop_recursively(false_edge);
  tgt_op->remove_inedge_recursively(false_edge);

  
  //If necessary, remove the pred_false edge between
  //brct_op and its pred_false fallthru op
  if(pred_false_edge)
    {
      Op* pf_tgt_op=  pred_false_edge->dest();
      Basicblock* pf_tgt_bb= (Basicblock*)pf_tgt_op->parent();
            
      brct_op->remove_outop_recursively(pred_false_edge);
      brct_op->remove_outedge_recursively(pred_false_edge);
      //Remove in ops (appropriate) and edges from tgt_op 
      pf_tgt_op->remove_inop_recursively(pred_false_edge);
      pf_tgt_op->remove_inedge_recursively(pred_false_edge);

      //Put in the fall thru edge for the bru_bb
      El_connect_region_fallthrough(bru_bb,pf_tgt_bb);
    }


  //Re-connect the old false edge to point to the bru_bb
  false_edge->set_dest(brct_op, false_edge->src_port(), false_edge->src_alt());
  false_edge->set_dest(merge_op, false_edge->dest_port(), false_edge->dest_alt());

  
  //Add this new edge between brct_op and merge_op to the right
  //in and out sets
  merge_op->add_inop_recursively(false_edge);
  merge_op->add_inedge_recursively_restricted(false_edge);
  //Add outops and edges to the brct op
  brct_op->add_outop_recursively(false_edge);
  brct_op->add_outedge_recursively_restricted(false_edge);


  return bru_bb;
}


Op* el_insert_pbr(Op* op, Compound_region* dest_cr, int pred_bit)
{
  Op* pbr_op = new Op(PBR);
  Base_operand* regist;
  Base_operand* base_oper;
  Operand temp_oper;
  //Hack to get around the fact that hyperblocks (if
  //they exist) determine the branch targets loaded into the pbr
  Region* dest_reg= 
    (dest_cr->parent() && dest_cr->parent()->is_hb()) ? (Region *)(dest_cr->parent()): (Region *)dest_cr;
  
  //Set dest operands,id attributes to the dest and
  //attrs stored in the brdir (these values were stored
  //during normalization specifically to be restored at
  //denormalization)
  Operand* temp_oper_ptr=el_get_oper_from_br_info(op,BR_INFO_PBR);
  if(temp_oper_ptr)
    {
      int id;
      temp_oper = *temp_oper_ptr;
      set_lcode_attributes(pbr_op,
        new Lcode_attribute_map(*el_get_attrs_from_br_info(op,BR_INFO_PBR)));
      if((id=el_get_id_from_br_info(op,BR_INFO_PBR))!=-1)
	pbr_op->set_id(id);
    }
  else
    {
      regist = new Reg(EL_STANDARD_BRANCH, BTR, virtual_register_number);
      temp_oper_ptr= new Operand(regist);
      temp_oper = *temp_oper_ptr;
    }

  pbr_op->set_dest(DEST1, temp_oper);
  base_oper = new Cb_operand(dest_reg->id());
  temp_oper = *(new Operand(base_oper));
  pbr_op->set_src(SRC1, temp_oper);

  base_oper = new Int_lit(pred_bit);
  temp_oper = *(new Operand(base_oper));
  pbr_op->set_src(SRC2, temp_oper);

  pbr_op->set_src(PRED1, op->src(PRED1));
		     
  El_insert_op_before(pbr_op,op);
  return pbr_op;
}

Op* el_insert_cmpp(Op* op)
{
  Op* cmpp_op;
  IR_BASE_OPCODE new_opc;
  int old_opc = (op->src(SRC1)).int_value();
  Operand cmpp_oper;

  new_opc=el_denormalize_br_opcode(old_opc);
  cmpp_op=new Op(make_UN_pred(make_UN_pred((Opcode)(new_opc),DEST1),
			      DEST2));
  assert(is_UN_pred(cmpp_op,DEST1));//Check I got it right
  
  Operand *cmpp_oper_ptr=el_get_oper_from_br_info(op,BR_INFO_CMPP);
  if(cmpp_oper_ptr)
    {
      int id;
      cmpp_oper=*cmpp_oper_ptr;
      if((id=el_get_id_from_br_info(op,BR_INFO_CMPP))!=-1)
	cmpp_op->set_id(id);
      set_lcode_attributes(cmpp_op,
        new Lcode_attribute_map(*el_get_attrs_from_br_info(op,BR_INFO_CMPP)));
    }
  else
    {
      Base_operand* regist = new Reg(EL_STANDARD_PREDICATE, PR, virtual_register_number);
      cmpp_oper_ptr= new Operand(regist);
      cmpp_oper = *cmpp_oper_ptr;
    }
  
  
  cmpp_op->set_dest(DEST1,cmpp_oper);
  cmpp_op->set_src(SRC1,op->src(SRC2));
  cmpp_op->set_src(SRC2,op->src(SRC3));

  cmpp_op->set_src(PRED1, op->src(PRED1));

  El_insert_op_before(cmpp_op,op);
  
  return cmpp_op;
}

Op* el_insert_brct(Op* op, Op* pbr_op, Operand cmpp_oper)
{
  Op *brct_op = new Op(BRCT);
  Basicblock *cur_bb = (Basicblock*)(op->parent());
  Operand pbr_dest = pbr_op->dest(DEST1);

  //base_oper = new Reg(BRANCH, BTR, ((Reg*)regist)->vr_num());
  //temp_oper = new Operand(base_oper);
  //brct_op->set_src(SRC1, *temp_oper); 
  brct_op->set_src(SRC1, pbr_dest); 

 
  brct_op->set_src(SRC2, cmpp_oper);

  brct_op->set_src(PRED1, op->src(PRED1));
  brct_op->set_id(op->id());
  set_lcode_attributes(brct_op,
    new Lcode_attribute_map(*el_get_attrs_from_br_info(op,BR_INFO_BR)));

  El_replace_op(cur_bb,op,brct_op);
  return brct_op;
}

Op* el_insert_bru(Op* op, Op* pbr_op)
{
  Op *bru_op = new Op(BRU);
  Basicblock *cur_bb = (Basicblock*)(op->parent());

  bru_op->set_src(SRC1, pbr_op->dest(DEST1));
  bru_op->set_src(PRED1, op->src(PRED1));
  bru_op->set_id(op->id());
  set_lcode_attributes(bru_op,
    new Lcode_attribute_map(*el_get_attrs_from_br_info(op,BR_INFO_BR)));

  El_replace_op(cur_bb,op,bru_op);
  
  return bru_op;
}

//Given a cmpp op (remember it has two dests), find which of the dests
//is used by the branch that follows the cmpp
Port_num el_find_br_pred_port(Op* cmpp,Op* br,Reaching_defs_info* rdi)
{
  
  //Iterate over the dest opers of the cmpp till we find the
  //(unique) one that is used by the br
  for (Op_complete_dests dest_oper(cmpp); dest_oper != 0; dest_oper++) 
    {
      Operand* cur_oprnd = &(*dest_oper);
      
      if(rdi->is_interesting(*cur_oprnd)) 
	{
	  El_ref new_def (cmpp,cur_oprnd,dest_oper.ref_type());
	  List<El_ref>* uses = rdi->get_du_chain(new_def);
	  
	  //check if the oper is used by the br; if so, return its
	  //port number
	  List_iterator<El_ref> uses_li;
	  for(uses_li(*uses);uses_li!=0;uses_li++)
	    {
	      El_ref def=*uses_li;
	      Op* use_op=def.get_op();
	      if(use_op == br)
		{
		  return dest_oper.get_port_num();
		}
	      
	    }
	}
    }
  //should definitely have found a port
  assert(0);
  return (Port_num)0; //keep compiler happy
}

bool el_op_has_exactly_one_use(Op *op,Reaching_defs_info* rdi)
{
  int num_uses=0;
  
  for (Op_complete_dests dest_oper(op); dest_oper != 0; dest_oper++) {
    Operand* cur_oprnd = &(*dest_oper);
    El_ref new_def (op,cur_oprnd,dest_oper.ref_type());
    num_uses+=el_num_uses_of_def(new_def,rdi);
  }
    
  return (num_uses==1);
}

int el_num_uses_of_def(El_ref& def,Reaching_defs_info* rdi)
{
  return rdi->get_du_chain(def)->size();
  
}

void el_set_id_in_br_info(Op *brdir_op,int id,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    binfo->pbr_id=id;
    break;
  case BR_INFO_CMPP:
    binfo->cmpp_id=id;
    break;
  case BR_INFO_LD:
    binfo->ld_id=id;
    break;
  case BR_INFO_CALC:
    binfo->addr_calc_id=id;
    break;
  default:
    assert(0);
  }
  
  set_branch_info_attr(brdir_op,binfo);
  return;
}

void el_set_attrs_in_br_info(Op *brdir_op,Lcode_attribute_map* attrs,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    binfo->pbr_attrs=attrs;break;
  case BR_INFO_CMPP:
    binfo->cmpp_attrs=attrs;break;
  case BR_INFO_LD:
    binfo->ld_attrs=attrs;break;
  case BR_INFO_CALC:
    binfo->addr_calc_attrs=attrs;break;
  case BR_INFO_BR:
    binfo->br_attrs=attrs;break;
  default:
    assert(0);
  }
  
  set_branch_info_attr(brdir_op,binfo);
  return;
}

void el_set_oper_in_br_info(Op *brdir_op,Operand *tgt,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    binfo->pbr_tgt=tgt;break;
  case BR_INFO_CMPP:
    binfo->cmpp_tgt=tgt;break;
  case BR_INFO_LD:
    binfo->ld_tgt=tgt;break;
  case BR_INFO_CALC:
    binfo->addr_calc_tgt=tgt;break;
  case BR_INFO_BR:
  default:
    assert(0);
  }
  
  set_branch_info_attr(brdir_op,binfo);
  return;
}


void el_set_op_in_br_info(Op *brdir_op,Op* op,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    binfo->pbr_op=op;break;
  case BR_INFO_CMPP:
    binfo->cmpp_op=op;break;
  case BR_INFO_LD:
    binfo->ld_op=op;break;
  case BR_INFO_CALC:
    binfo->addr_calc_op=op;break;
  default:
    assert(0);
  }
  
  set_branch_info_attr(brdir_op,binfo);
  return;
}

Operand* el_get_oper_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    return binfo->pbr_tgt;
  case BR_INFO_CMPP:
    return binfo->cmpp_tgt;
  case BR_INFO_LD:
    return binfo->ld_tgt;
  case BR_INFO_CALC:
    return binfo->addr_calc_tgt;
  default:
    {
      assert(0);
      return (Operand*)NULL;
    }
  }
}

Lcode_attribute_map* el_get_attrs_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    return binfo->pbr_attrs;
  case BR_INFO_CMPP:
    return binfo->cmpp_attrs;
  case BR_INFO_LD:
    return binfo->ld_attrs;
  case BR_INFO_CALC:
    return binfo->addr_calc_attrs;
  case BR_INFO_BR:
    return binfo->br_attrs;
  default:
    {
      assert(0);
      return (Lcode_attribute_map*)NULL;
    }
  }
}

int el_get_id_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    return binfo->pbr_id;
  case BR_INFO_CMPP:
    return binfo->cmpp_id;
  case BR_INFO_LD:
    return binfo->ld_id;
  case BR_INFO_CALC:
    return binfo->addr_calc_id;
  default:
    {
      assert(0);
      return (int)0;
    }
  }
}

Op* el_get_op_from_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype)
{
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    return binfo->pbr_op;
  case BR_INFO_CMPP:
    return binfo->cmpp_op;
  case BR_INFO_LD:
    return binfo->ld_op;
  case BR_INFO_CALC:
    return binfo->addr_calc_op;
  default:
    {
      assert(0);
      return (Op *)NULL;
    }
  }
}

bool el_op_exists_in_br_info(Op *brdir_op,BR_INFO_FIELD_TYPE ftype)
{ 
  Branch_info_attr* binfo = get_branch_info_attr(brdir_op);
  switch(ftype){
  case BR_INFO_PBR:
    return (binfo->pbr_op!=NULL);
  case BR_INFO_CMPP:
    return (binfo->cmpp_op!=NULL);
  case BR_INFO_LD:
    return (binfo->ld_op!=NULL);
  case BR_INFO_CALC:
    return (binfo->addr_calc_op!=NULL);
  default:
    {
      assert(0);
      return (bool)NULL;
    }
  }
}


//return  a 1 if the branch is predicted taken, 0 otherwise
int el_find_branch_prediction_bit(Op* op,Opcode opc)
{
  bool is_brcf=false;
  Basicblock* cur_bb=(Basicblock*)op->parent();
  double true_freq=0.0,false_freq=0.0;
  int count=0;

  switch(opc)
    {
    case BRU:
      {
	  return 1;
      }
    case BRCF:
      is_brcf=true;
    case BRCT:
      {
	bool is_predicated=false;

	for (Region_exit_edges exit_iter(cur_bb); exit_iter!=0; exit_iter++,count++)
	  {
	    Edge* out = (*exit_iter);
	    Control_flow_freq* freq = get_control_flow_freq(out);
	    
	    if(out->src_alt() == 2)
	      is_predicated=true;
	    else if(out->src_alt() == 1)
	      true_freq=freq->freq;
	    else
	      false_freq=freq->freq;	   
	  }
	assert(count==2||((count==3)&&is_predicated));
	//if the false frequency is less than the true frequency,
	//a brct is predicted taken else not (opposite for brcfs)
	if (false_freq<true_freq)
	  return (is_brcf ? 0:1); 
	else
	  return (is_brcf ? 1:0);
      }
    case BRIND:
      {
	assert(0);//no indirect branches for now
	return 1;
      }
    default:
      assert(0);
      return 1;//keep the compiler happy
    }
}


void el_flip_exit_alt_numbers(Basicblock* cur_bb)
{
  int edge_count=0;

  for (Region_exit_edges exit_iter(cur_bb);
       exit_iter!=0; 
       exit_iter++,edge_count++)
    {
      Edge* edge = (*exit_iter);
      
      if(edge->src_alt() == 0)
	edge->set_src(edge->src(),edge->src_port(),1);
      else
	edge->set_src(edge->src(),edge->src_port(),0);
    }

  assert(edge_count==2);

  return;
}


//a recursive scheme for finding out if a cr is the last cr on the dlist
//of an ancestor
bool is_last_subregion(Compound_region* region, Region* anc_region)
{
  Compound_region *reg=region ;
  Region *anc_reg=anc_region ;
  assert(reg != NULL);

  if(reg==anc_reg)
    return true;
  else if(anc_reg->is_op())
    return false;
  else
    return (is_last_subregion(reg,((Compound_region*)anc_reg)->last_subregion()));
}

bool is_first_subregion(Compound_region* region, Region* anc_region)
{
  Compound_region *reg=region ;
  Region* anc_reg=anc_region;

  if(reg==anc_reg)
    return true;
  else if (anc_reg->is_op())
    return false;
  else
    return (is_first_subregion(reg,((Compound_region*)anc_reg)->first_subregion()));
}


bool el_compound_region_succeeds_in_dlist(Compound_region* second,Compound_region* first)
{
  Compound_region* anc_second,*anc_first;
  Compound_region* ica= innermost_common_ancestor(second,first,anc_second,anc_first);
  if(ica->succeeds_in_subregion_list(anc_second,anc_first))
    return (is_first_subregion(second,anc_second)&&
	    is_last_subregion(first,anc_first));
  else
    return false;
}

int el_num_region_successors(Region* reg)
{
  return (reg->outedges()).size();
}


