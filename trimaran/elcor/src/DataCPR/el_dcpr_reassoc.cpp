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
//      File:           el_dcpr_reassoc.cpp
//      Authors:        Suren Talla
//      Created:        August 1997
//      Description:    Reassoc interface defns (a data CPR opti)
//
//      Copyright (C) 1996 Hewlett-Packard Company 
//      
///////////////////////////////////////////////////////////////////////////

#include "dbg.h"
#include "el_dcpr_reassoc.h"
#include "reaching_defs_solver.h"
#include "el_control.h"
#include "el_bb_tools.h"
#include "dcpr_utils.h"
#include <stdlib.h>
#include "operand.h"
#include "opcode.h"
#include "edge.h"
#include "op.h"
#include "opcode_properties.h"
#include "el_data_cpr_init.h"
#include "iterators.h"

int Reassoc_flow_chains::chain_set_id_counter = 0;

extern "C" {
void qsort(void* base, size_t nel, size_t size, int (*compare)(const void*, const void*));
}

//##################################################################
int compare_reassoc_terms(const void* x, const void* y)
{
    if (((Reassoc_term*)x)->estart < ((Reassoc_term*)y)->estart) {
	return -1;
    } else if  (((Reassoc_term*)x)->estart == ((Reassoc_term*)y)->estart) {
	// yeah, we want 'em sorted by estart times, but also would;
	// like the literals up ahead of other operands with same estart;
	if ((((Reassoc_term*)y)->useref.get_operand()).is_lit())
	    return 1;
	return 0;
    }
    return 1;
}

//##################################################################
Flow_chain::Flow_chain(Opcode opc) : 
  opcode(opc),
  pred_vr_num(-1), 
  is_predicated(false),
  chain_type(SCALAR_CHAIN),
  to_delete(false),
  chain_set_id(-1),
  chain_op_map (hash_op_ptr, MAX_CHAIN_LENGTH)
{
    rev_chain_op_map.resize(MAX_CHAIN_LENGTH);
}

Flow_chain::~Flow_chain(void)
{
    chain_edges.clear();
    chain_op_map.clear();
    rev_chain_op_map.clear();
}

//##################################################################
int
Flow_chain::will_split_chain(Edge* e)
{
    int len, s;

    len = chain_edges.size();
    if (len<1)
	return false;
    
    /*if the source or the dest of the edge is part of the
      chain and is not a start/end point, then the chain splits */
    
    if (chain_op_map.is_bound(e->src())){
	int s = chain_op_map.value(e->src());
	if (s==len) return false;
	return true;
    }

    if (chain_op_map.is_bound(e->dest())){
	s = chain_op_map.value(e->dest());
	if (s==0) return false;
	return true;
    }

    return false;
    
}

//##################################################################
int 
Flow_chain::follows_edge(Edge* e)
{
    Op* sop = e->src();
    if (!chain_op_map.is_bound(sop)) return false;

    Edge* head_edge = chain_edges.tail();

    if (head_edge->dest()==sop) return true;
    return false;

}

//##################################################################
int 
Flow_chain::concat(Flow_chain* this_fc)
{
    for(List_iterator<Edge*> li(this_fc->chain_edges); li!=0; li++){
	Edge *e = *li;
	append(e);
    }
    return 1;
}

//##################################################################
// remove the ops of the chain from the IR
//##################################################################
int 
Flow_chain::delete_chain_ops()
{
    if (!to_delete)
	return 0;

    int len = chain_edges.size() + 1;
    for(int i=0; i<len; i++){
	El_remove_op(rev_chain_op_map[i]);
    }
    return len;
}
//##################################################################
int 
Flow_chain::is_interesting(Edge* e)
{
    /*if the source or the dest op of the edge is part of the
      chain, then the edge can extend the chain */
    
    Opcode opc ;
    
    if (chain_op_map.is_bound(e->src())){
	opc = e->src()->opcode();
    } else if (chain_op_map.is_bound(e->dest())) {
	opc = e->dest()->opcode();
    } else {
	return false;
    }

    //if edge type doesnt match with that of the chain, not interesting;
    if (opc!=opcode) return false;

    //this check is really not necessary since checking with chain;
    //intersection is sufficient to guarantee this;
    if (opc==ADD_W || opc==ADDL_W || opc==SUB_W || opc==SUBL_W ||
	opc==MPY_W || opc==MPYL_W || opc==FMPY_S || opc==FMPY_D ||
 	opc==FADD_S || opc==FADD_D || opc==FSUB_S || opc==FSUB_D)
	return true;

    return false;

}

/*##################################################################
// when the chain is split, due to another edge merging into/away
// from it, a new chain is created making a copy of the
// common portion of the overlap if it is splitting and is truncated
// if the new edge is merging.
//#################################################################*/
int
Flow_chain::sub_chain(Edge* e, Flow_chain* fc)
{
    int len = chain_edges.size();

    if (len<1) 	return 0;

    Op *sop = e->src();
    Op* dop = e->dest();

    assert (chain_op_map.is_bound(sop) || chain_op_map.is_bound(dop));

    //NOTE :: it is possible for both the sop and dop to be already ;
    //be present on this chain. But if we consider flow edges from  ;
    //the critical region only, then this case wont occur;
    //For now punt on it? But this needs to be fixed!!

    //assert (!(chain_op_map.is_bound(sop) && chain_op_map.is_bound(dop)));

    List_iterator<Edge*> ci;
    int i, n, num;
    Edge *ce;

    if (chain_op_map.is_bound(sop)) {
	//new edge splits away from chain;
	n = chain_op_map.value(sop);
	num = 0;
	ci(chain_edges);
	//append edges till the split point;
	for(i=0; i<n; i++){
	    ce = *ci;
	    fc->append(ce);
	    ci++;
	    num++;
	}
	//append the new edge;
	fc->append(e);

	//both , this chain and the new chain are no longer scalar-chains;
	chain_type = VECTOR_CHAIN;
	fc->chain_type = VECTOR_CHAIN;

	//both the new subchain and the current chain have the same ;
	//chain set id (replicated cases of reduction to Vector);
	fc->id(chain_set_id);

	return num + 1;

    } else if (chain_op_map.is_bound(dop)) {
	/* now check if the merge point is on the operand 
	// which is part of the chain (case 1) or on a different
	// operand (case 2). if same operand, then the chain splits
	// from merge point downwards else the chain splits two 
        // way - check notes for details
        // 
        // case 1)
        //
	//        \ /     
        //      r= x + y       
        //       \
        //    r1= r + z
        // 
        // case 2)
        //
	//        \     /     
        //      r= x + y       
        //       \
        //    r1= r + z
        //
        */
	
	int split_chain = true;
	for(List_iterator<Edge*> li(chain_edges); li!=0; li++){
	    ce = *li;
	    if (ce->dest() == dop){
		if (ce->dest_port() == e->dest_port()){
		    //case 2 - chain loses all edges prior to merge;
		    split_chain = false;
		} 
		break;
	    }
	}


	n = chain_op_map.value(dop);
	ci(chain_edges);

	if (split_chain){
	    //first add the new edge to the new chain;
	    fc->append(e);
	    
	    if (n==len) return 1;

	    //skip the edges till the merge point;
	    for(i=0; i<n; i++)
		ci++;
	    
	    num =1;
	    //append edges from merge point till end of chain into the new chain;
	    for(; ci!=0; ci++){
		ce = *ci;
		fc->append(ce);
		ci++;
		num++;
	    }

	    //fc is a distinct new chain so it gets a new chain_set_id;
	    fc->id(Reassoc_flow_chains::new_chain_set_id());

	} else {
	    //delete the edges till the merge point;
	    for(i=0; i<n; i++){
		ce = *ci;
		chain_edges.remove(ce);
		ci++;
	    }
	    
	    //need to recreate the maps;
	    chain_op_map.clear();
	    rev_chain_op_map.clear();
	    rev_chain_op_map.resize(MAX_CHAIN_LENGTH); 

	    if (chain_edges.size()==0) {
		//this chain itself is deleted;
		return -1;
	    }

	    n = 0;
	    List_iterator<Edge*> lii(chain_edges);
	    ce = *lii; 
	    chain_op_map.bind(ce->src(), n); 
	    rev_chain_op_map[n] = ce->src();
	    
	    for(; lii!=0; lii++){
		ce = *lii;
		n++;
		dop = ce->dest();
		chain_op_map.bind(dop, n);
		rev_chain_op_map[n] = dop;
	    }
	    //since no new chain is created;
	    num =0;
 	    
	}

	return num;
	    
    } else {
	El_punt("Flow_chain::sub_chain called unnecessarily!");
    }

    return 0;

}

//##################################################################
int 
Flow_chain::is_scalar(Reaching_defs_info* rdefs_info)
{
    //if any of the definitions in the flow chain;
    //have uses ouside of the chain, then chain;
    //is NOT scalar;

    List<El_ref>* use_list;
    for(List_iterator<Edge*> li(chain_edges); li!=0; li++){
	Edge* e = *li;
	Op* defop = e->src();
	Operand* defopnd = &(e->src()->dest(e->src_port()));
	El_ref defref(defop, defopnd, EXP_DEST);
	if (rdefs_info->def_use.is_bound(defref)){
	    use_list = rdefs_info->get_du_chain(defref);
	    if (use_list->size() > 1)
		return false;
	} else {
	    El_punt("unbound def part of flow chain?");
	}
    }
    return true;
}

//##################################################################
int 
Flow_chain::get_sub_chains(Reaching_defs_info* rdefs_info,
			   List<Flow_chain*>& subchains)
{
    List<El_ref>* use_list;
    for(List_iterator<Edge*> li(chain_edges); li!=0; li++){
	Edge* e = *li;
	Op* defop = e->src();
	Operand* defopnd = &(e->src()->dest(e->src_port()));
	El_ref defref(defop, defopnd, EXP_DEST);
	if (rdefs_info->def_use.is_bound(defref)){
	    use_list = rdefs_info->get_du_chain(defref);
	    if (use_list->size() > 1){
		//need the chain upto this point - this sub expression;
		//is used elsewhere (in another use);
		Flow_chain *new_fc = new Flow_chain(type());
		for(List_iterator<Edge*> lie(chain_edges); lie!=0; lie++){
		    Edge *ce = *lie;
		    if (ce==e){
			break;
		    } else {
			new_fc->append(ce);
		    }
		}
		if (new_fc->size() > 0){
		    //atleast one edge;
		    subchains.add_tail(new_fc);
		} else {
		    delete new_fc;
		}
	    }
	} else {
	    El_punt("unbound def part of flow chain?");
	}
    }
    return 1;
}

//##################################################################
int 
Flow_chain::append(Edge* e)
{
    //dont need to store the edges going to switches;
    if (is_switch(e->dest())) return 0;

    int len = chain_edges.size();
    Op* op;

    chain_edges.add_tail(e);
    if (len==0) {
	//first edge added;
	op = e->src();
	chain_op_map.bind(op, 0);
	rev_chain_op_map[0] = op;
    }

    op = e->dest();
    chain_op_map.bind(op, len + 1);
    rev_chain_op_map[len+1] = op;

    return 0;
}

//##################################################################
// to be called only if the edge is interesting but will not split
// the chain - in other words the new edge will be added either at
// the front or at the tail of this chain.
//##################################################################
int 
Flow_chain::insert(Edge* e)
{
    Op* sop = e->src();
    Op* dop = e->dest();

    assert (chain_op_map.is_bound(sop) || chain_op_map.is_bound(dop));

    int n, len = chain_edges.size();

    if (chain_op_map.is_bound(sop)){
	//the new edge should go to the end of the chain;
	n = chain_op_map.value(sop);
	assert(n == len);
	append(e);

    } else {

	//new edge will go to the front of the list;
	n = chain_op_map.value(dop);
	assert(n == 0);

	//need to renumber the whole chain.. gosh..;
	chain_edges.add_head(e);
	
	chain_op_map.clear();
	rev_chain_op_map.clear();
	rev_chain_op_map.resize(MAX_CHAIN_LENGTH); 

        List_iterator<Edge*> li(chain_edges);
        Edge *e = *li; 
	//n == 0 here always;
        chain_op_map.bind(e->src(), n);   
        rev_chain_op_map[n] = e->src();
       
        for(; li!=0; li++){
	    e = *li;
	    n++;
	    dop = e->dest();
	    chain_op_map.bind(dop, n);
	    rev_chain_op_map[n] = dop;
	}
	
    }

    return chain_edges.size();

}

//##################################################################
int
Flow_chain::is_reassoc_profitable(void)
{
    //reassoc is profitable if the critical height is larger;
    //than the subtree height of the new expression tree;
    //for now we use a very simple heuristic;
    unsigned len = chain_edges.size();
    if ((int)len>(int)El_reassoc_chain_length) // && chain_type==SCALAR_CHAIN)
	return true;

    return false;
}

//##################################################################
// function given a sequence of terms ("terms" == operands) of the
// arithmetic expression to be reassociated, will reompute an better
// evaluation tree and generate code for it (stored in new_ops)
//##################################################################
int
Flow_chain::terms_to_ops(Reassoc_term* terms, Op* result_op,
			 int num_terms, int latency, List<Op*>& new_ops)
{
    //used for literal in place computation;
    int j, i = 0; 
    float f = .0; 
    double d = .0;

    Reassoc_term  new_terms[MAX_CHAIN_LENGTH];
    El_ref null_uref((Op*)NULL, (Operand*)NULL, EXP_SRC);
    El_ref null_dref((Op*)NULL, (Operand*)NULL, EXP_DEST);

    cdbg << "num terms = " << num_terms << endl;
    cdbg << "before sort :: " ;
    for(j=0; j<num_terms; j++){
	cdbg << "<" << (terms[j].useref.get_op())->id()  << ", "
	     << terms[j].estart << "> ";
    }
    cdbg << endl;


    // sort terms according to their e-start times;
    // NOTE :: since the constants (literals) have 0 estart times, ;
    // they will be in the front of the list after sort - check the;
    // compare_reassoc_terms fn for  slight subtlety!;

    qsort((void*)terms, (size_t)num_terms, (size_t)(sizeof(Reassoc_term)), 
          compare_reassoc_terms);

    cdbg << "after sort :: " ;
    for(j=0; j<num_terms; j++){
	cdbg << "<" << (terms[j].useref.get_op())->id() << ", "
	     << terms[j].estart << "> ";
    }
    cdbg << endl;

    
    Operand* o = &(terms[0].useref.get_operand());
    Operand new_operand(*o);

    //number of terms in the new long expression - could be;
    //less than the original due to literal combining;
    int numt = 0;
    int oldt = 0;

    //first merge all the literal expressions into one precomputed value;
    //sort puts all literals in front;
    if (o->is_lit()){

	//process literals - allocate new literal operand;
	//all literals better be of the same type;
	if (o->is_int()) {
	    while (o->is_lit()){
		i += o->int_value();
		oldt++;
		if (oldt==num_terms) break;
		o = &(terms[oldt].useref.get_operand());
	    }
	    new_operand.set_int_value(i);
	    // Reset the Datatype of new_operand (the width may have changed)
            DType dt(new_operand.dtype());
	    dt.calc_and_set_width(i, dt.is_signed());
	    new_operand.set_dtype(dt);
	    cdbg << "int = " << i << endl;
        }
        else if (o->is_float()) {
	    while (o->is_lit()){
	        f += o->float_value();
	        oldt++;
	        if (oldt==num_terms) break;
	        o = &(terms[oldt].useref.get_operand());
	    }
	    new_operand.set_float_value(f);
	    cdbg << "float = " << f << endl;
        }
        else if (o->is_double()) {
	    while (o->is_lit()){
		d += o->double_value();
		oldt++;
		if (oldt==num_terms) break;
		o = &(terms[oldt].useref.get_operand());
	    }
	    new_operand.set_double_value(d);
	    cdbg << "double = " << d << endl;
        }
        El_ref tref1(terms[0].useref.get_op(), &(new_operand), EXP_SRC);
	new_terms[numt].useref = tref1;
	new_terms[numt].defref = null_dref;   //reaching defs do not apply to literals;
	numt++;
    }

    //copy the rest of the terms;
    while (oldt < num_terms){
	new_terms[numt] = terms[oldt];
	numt++;
	oldt++;
    }
    
    //cdbg << "oldt, newt = <" << oldt << ", " << numt << ">\n";


    //------------------------------------------------------------;
    //while there are terms to be merged;
    //   if the first term > timer, advance timer to first term;
    //   if there are two min terms with the same estart as timer ;
    //       gen new op for the first two terms;
    //       insert new op into the new ops list;
    //       note :: the point of new op insertion is complicated - ;
    //       explained in the document - at the moment, we take;
    //       a simplified approach : all new ops go after the final op;
    //       of the original chain;
    //------------------------------------------------------------;

    int current_term=0;
    Op *new_op;
    Operand newop_dest, newop_src1, newop_src2, newop_pred;
    int num_new_ops = 0;

    assert(numt>0);
    
    Operand result_dest = result_op->dest(DEST1);
    Operand result_pred = result_op->src(PRED1);

    while (current_term<numt){
	Operand* o1 = &(new_terms[current_term].useref.get_operand());

	if (current_term==(numt-1)){
	    newop_dest = result_dest; 
	    newop_src1 = *o1;
	    newop_pred = result_pred; 
	    new_op = El_generate_move_op(newop_dest, newop_src1, newop_pred);
	    //cdbg << "1. new_op = " << new_op->id() << endl;
	    current_term++;
	} else {
	    //there are atleast two terms;
	    if (new_terms[current_term].estart < new_terms[current_term+1].estart) {
		new_terms[current_term].estart = new_terms[current_term+1].estart;
	    }
	    if (current_term==(numt-2)){
		//only the last two terms remain - so the result should go ;
		//into the result_op;
		newop_dest = result_dest; 
		newop_pred = result_pred; 
	    } else {
		//otherwise, generate a new temporary to hold result;
		newop_dest = new Reg(result_dest); 
		newop_pred = result_pred; 
	    }
	    newop_src1 = *o1;
	    newop_src2 = new_terms[current_term+1].useref.get_operand();
	    /*
	    cdbg << newop_dest << endl; 
	    cdbg << "-----------------------------------------\n";
	    cdbg << newop_src1 << endl;
	    cdbg << "-----------------------------------------\n";
	    cdbg << newop_src2 << endl;
	    cdbg << "-----------------------------------------\n";
	    cdbg << newop_pred << endl;
	    cdbg << "-----------------------------------------\n";
	    */
	    switch (opcode){
	      case ADD_W:
	      case ADDL_W:
	      case FADD_S:
	      case FADD_D:
		new_op = El_generate_add_op(newop_dest, newop_src1, newop_src2, newop_pred);
		break;
	      case SUB_W:
	      case SUBL_W:
	      case FSUB_S:
	      case FSUB_D:
		new_op = El_generate_sub_op(newop_dest, newop_src1, newop_src2, newop_pred);
		break;
	      case MPY_W:
	      case MPYL_W:
	      case FMPY_S:
	      case FMPY_D:
		new_op = El_generate_mpy_op(newop_dest, newop_src1, newop_src2, newop_pred);
		break;
	      default:
		El_punt("unknown opcode for reassoc!");
	    }
	    cdbg << "new_op = " << new_op->id() << " op = " << (*new_op) << endl;
	    
	    //if these were the last two ops, then no need to insert the;
	    //created temp destination into the array to terms to be combined;
	    if (current_term<(numt-2)){
		//need to put in the temporary into the terms array. we reuse;
		//one of the slots used when the two terms were combined;
		El_ref tref2(new_op, &newop_dest, EXP_DEST);
		new_terms[current_term+1].defref = tref2;

		El_ref tref3(new_op, &newop_dest, EXP_SRC);
		new_terms[current_term+1].useref = tref3;
		//NOTE:: in the above stmt, the first parameter should not be;
		//new_op but NULL. Since it is not used, it doesnt matter. ;
		//neeed to remember that for later;

		new_terms[current_term+1].estart += latency;
		current_term++;
	    } else {
		current_term += 2;
	    }
	}
	
	//Now sort the terms again based on their e_start times for the;
	//next merge. Presumably there is a smarter way to do this instead;
	//of sorting everytime, but my guess is it might be tough to code;
	//in an hour;
	
	if (current_term<(numt-2)){

	    qsort((void*)(&new_terms[current_term]), 
		  (size_t)(numt - current_term), 
		  (size_t)(sizeof(Reassoc_term)), 
		  compare_reassoc_terms);
	}
		      
	new_ops.add_tail(new_op);
	num_new_ops++;

    }

    return num_new_ops;
}

//##################################################################
int
Flow_chain::get_unique_ops(List<Op*>& ops_list)
{
    int len = chain_edges.size() + 1;
    for(int i=0; i<len; i++){
	Op* op = rev_chain_op_map[i];
	if (!ops_list.is_member(op))
	    ops_list.add_tail(op);
    }
    return len;
}

//##################################################################
int
Flow_chain::insert_new_ops(Region* rgn, List<Op*>& ops,Op* after_op) 
{
    for(List_iterator<Op*> li(ops); li!=0; li++){
	Op* new_op = *li;
	El_insert_op_after(new_op, after_op);
    }
    return ops.size();

}

//##################################################################
int
Flow_chain::gen_code(Region* rgn, Reaching_defs_info* rdefs_info,
		     Hash_map<Op*, DepLen_OpInfo*>& deplen_info)
{
    //the deplen_info map is used for the e-start times;
    //get the terms in the big expression to be reassoc;
    //get e-start values for the defs of those uses;
    
    Reassoc_term  terms[MAX_CHAIN_LENGTH];


    //NOTE:: for the estart times, we need to get the estart time;
    //of the operation which defined the operand being used;

    List_iterator<Edge*> li(chain_edges);
    Edge *e = *li;
    List<El_ref>* def_list;
    El_ref x_defref;
    El_ref null_ref((Op*)NULL, (Operand*)NULL, EXP_DEST);
    Op* op;
    List_iterator<El_ref> liref;
    Operand* operand;
    int num_terms = 0;

    //we need both the terms of the first instruction of the chain;

    //term 1 = first source of the first op in the chain;
    operand =  &(e->src()->src(SRC1));
    El_ref x_useref1(e->src(), operand, EXP_SRC); 
    terms[0].useref = x_useref1;

    if (operand->is_lit()){
	terms[0].defref = null_ref;  //reaching defs do not apply to literals;
	terms[0].estart = 0;         //all literals have 0 estart times;
    } else if (rdefs_info->use_def.is_bound(x_useref1)){
	def_list = rdefs_info->get_ud_chain(x_useref1);
	assert(def_list->size() > 0);
	terms[0].estart = ELCOR_MIN_INT;
	for(liref(*def_list); liref!=0; liref++){
	    x_defref = *liref;
	    op = x_defref.get_op();
	    assert(deplen_info.is_bound(op));
	    if (terms[0].estart < (deplen_info.value(op))->m_Estart) {
		terms[0].estart = (deplen_info.value(op))->m_Estart;
		terms[0].defref =  x_defref;
	    }
	} 
    } else {
	El_punt("use is not a literal and has no reaching defs??");
    }


    //term 2 = second source of the first op in the chain;
    operand =  &(e->src()->src(SRC2));
    El_ref x_useref2(e->src(), operand, EXP_SRC);
    terms[1].useref = x_useref2;
    if (operand->is_lit()){
	terms[1].defref = null_ref;    //reaching defs do not apply to literals;
	terms[1].estart = 0;           //all literals have 0 estart times;
    } else if (rdefs_info->use_def.is_bound(x_useref2)){
	def_list = rdefs_info->get_ud_chain(x_useref2);
	assert(def_list->size() > 0);
	terms[1].estart = ELCOR_MIN_INT;
	for(liref(*def_list); liref!=0; liref++){
	    x_defref = *liref;
	    op = x_defref.get_op();
	    assert(deplen_info.is_bound(op));
	    if (terms[1].estart < (deplen_info.value(op))->m_Estart) {
		terms[1].estart = (deplen_info.value(op))->m_Estart;
		terms[1].defref =  x_defref;
	    }
	} 
    } else {
	El_punt("use is not a literal and has no reaching defs??");
    }
    
    num_terms =2;  // so far we got two terms;
    
    //collect the terms from the rest of the chain;
    for(; li!=0; li++){
	e = *li;
	//cdbg << (*e) << endl;
	//cdbg << (*(e->src())) << endl;
	//cdbg << (*(e->dest())) << endl;
	if (e->dest_port() == SRC1){
	    operand =  &(e->src()->src(SRC2));
	} else if (e->dest_port() == SRC2) {
	    operand =  &(e->src()->src(SRC1));
	}
	El_ref x_useref3(e->dest(), operand, EXP_SRC);
	terms[num_terms].useref = x_useref3;
	if (operand->is_lit()){
	    terms[num_terms].defref = null_ref;  //reaching defs do not apply ;
	    terms[num_terms].estart = 0;         //all literals have 0 estart times;
	} else if (rdefs_info->use_def.is_bound(x_useref3)){
	    def_list = rdefs_info->get_ud_chain(x_useref3);
	    assert(def_list->size() > 0);
	    terms[num_terms].estart = ELCOR_MIN_INT;
	    //for estart time of the def which reaches this use, use the;
	    //def which has the max estart time;
	    for(liref(*def_list); liref!=0; liref++){
		x_defref = *liref;
		op = x_defref.get_op();
		op = terms[num_terms].defref.get_op();
		assert(deplen_info.is_bound(op));
		if (terms[num_terms].estart < (deplen_info.value(op))->m_Estart) {
		    terms[num_terms].estart = (deplen_info.value(op))->m_Estart;
		    terms[num_terms].defref =  x_defref;
		}
	    } 
	} else {
	    terms[num_terms].defref = null_ref;  
	    terms[num_terms].estart = 0;         
	    El_punt("no reaching defs for the use??");
	}

	num_terms++;
    }
	
    //at this point, "e" is the last edge in the chain;
    Op* result_op = e->dest();
    int latency = e->latency();  //all ops in chain have same latency;
    List<Op*>  new_ops;
    
    if (is_switch(result_op))
	cdbg << "WHACCCCCCKKK !!! switches...\n";

    //merge terms in the sorted list and generate code for the pairs;
    int num_new_ops = terms_to_ops(terms, result_op, num_terms, latency, new_ops);
    
    cdbg << "num new ops = " << num_new_ops << endl;
    for(List_iterator<Op*>  lops(new_ops); lops!=0; lops++){
	Op* op = *lops;
	cdbg << (*op) << endl;
    }

    //insert new code;
    insert_new_ops(rgn, new_ops, result_op);

    //delete old code (the delete_chain_ops function needs to be updated);
    mark_for_deletion();

    return 0;
}

//##################################################################
ostream&
operator<<(ostream& os, Flow_chain& fc)
{
    int i;
    os << "id = " << fc.chain_set_id << "  opcode = " << fc.opcode ;
    if (fc.chain_type==SCALAR_CHAIN) 
	os << " SCALAR \n";
    else 
	os << " VECTOR \n";

    /*
    os << "ops    = \n" ;
    int dim = fc.chain_edges.size() + 1;
    for(i=0; i<dim; i++){
	os << "  [" << i << "]   <" << fc.rev_chain_op_map[i]->id() 
	   << ", " << fc.rev_chain_op_map[i]->opcode() << ">\n";
    }
    os << "edges  = " << endl;
    */
    i = 0;
    for(List_iterator<Edge*> li(fc.chain_edges); li!=0; li++){
	Edge* e = *li;
	//os << (*e) << endl;
	//os << (*(e->src())) << endl;
	//os << (*(e->dest())) << endl;
	if (is_switch(e->src())){
	    os << "  [" << i << "]   " << e->src()->id() << " switch\n";
	} else if (is_switch(e->dest())){
	    os << "  [" << i << "]   " << e->dest()->id() << " switch\n";
	} else {
	    Operand& o1 = e->src()->dest(e->src_port());
	    assert(o1.is_reg());
	    os << "  [" << i << "]   " << e->src()->id() << " -> " 
		<< e->dest()->id() << "   r" << (o1.vr_num()) << "\n";
	}
	i++;
    }
    os << endl;
    return os;
}

/*##################################################################

class Reassoc_flow_chains {

  public:

    Reassoc_flow_chains();
    ~Reassoc_flow_chains();

    int  compute_flow_chains(Region*, int flag);
    void delete_flow_chains();

  private:
    List<Flow_chain*>   flow_chains;
    
};

 #################################################################*/

Reassoc_flow_chains::Reassoc_flow_chains()
{
}

Reassoc_flow_chains::~Reassoc_flow_chains()
{
}

//##################################################################
int 
Reassoc_flow_chains::is_interesting(Edge* e)
{
    //check 1;
    if (!e->is_reg_flow()) return false;

    //check 2 - get rid of the memvrs;
    Operand& oper = e->src()->dest(e->src_port());
    if (!oper.is_reg()) return false;
    
    //check 3: atleast opcode should be the right one;
    Opcode opc = e->src()->opcode();
    if (opc==ADD_W || opc==ADDL_W || opc==SUB_W || opc==SUBL_W ||
	opc==MPY_W || opc==MPYL_W || opc==FMPY_S || opc==FMPY_D ||
 	opc==FADD_S || opc==FADD_D || opc==FSUB_S || opc==FSUB_D){
	if (!is_switch(e->dest())){
	    opc = e->dest()->opcode();
	    if (opc==ADD_W || opc==ADDL_W || opc==SUB_W || opc==SUBL_W ||
		opc==MPY_W || opc==MPYL_W || opc==FMPY_S || opc==FMPY_D ||
		opc==FADD_S || opc==FADD_D || opc==FSUB_S || opc==FSUB_D)
		return true;
	    else 
		return false;
	}
	return true;
    }
    return false;

}

//##################################################################
int
Reassoc_flow_chains::merge_chains()
{
    if (flow_chains.size()<=1) return 0;

    Edge *e;
    int id_inner, id_outer;

    //cdbg << "merge chains (" << flow_chains.size() << ")\n";
    //display_flow_chains(cdbg);

    for(List_iterator<Flow_chain*> lf_outer(flow_chains); lf_outer!=0; lf_outer++){
	Flow_chain* fc_outer = *lf_outer;
	for(List_iterator<Flow_chain*> lf_inner(flow_chains); lf_inner!=0; lf_inner++){
	    Flow_chain* fc_inner = *lf_inner;
	    if (fc_inner==fc_outer)
		continue;

	    if (fc_inner->size()==0){
		flow_chains.remove(fc_inner);
		return true;
	    }
	    if (fc_outer->size()==0){
		flow_chains.remove(fc_outer);
		return true;
	    }

	    if (fc_inner->type() == fc_outer->type()){
		e = fc_inner->head_edge();
		if (fc_outer->follows_edge(e)){
		    //cdbg << "1. " << e->src()->id() << "->" << e->dest()->id() << endl;
		    fc_outer->concat(fc_inner);
		    id_inner = fc_inner->id();
		    id_outer = fc_outer->id();
		    delete fc_inner;
		    flow_chains.remove(fc_inner);
		    rename_flow_chains(id_inner, id_outer);
		    return true;
		}
		e = fc_outer->head_edge();
		if (fc_inner->follows_edge(e)){
		    //cdbg << "2. " << e->src()->id() << "->" << e->dest()->id() << endl;
		    fc_inner->concat(fc_outer);
		    id_inner = fc_inner->id();
		    id_outer = fc_outer->id();
		    delete fc_outer;
		    flow_chains.remove(fc_outer);
		    rename_flow_chains(id_outer, id_inner);
		    return true;
		}
	    }
	}
    }

    return false;
}

//##################################################################
void
Reassoc_flow_chains::rename_flow_chains(int these_ids, int to_these_ids)
{
    for(List_iterator<Flow_chain*> lf(flow_chains); lf!=0; lf++){
	Flow_chain *fc = *lf;
	if (fc->id() == these_ids){
	    fc->id(to_these_ids);
	}
    }
}

//##################################################################
int
Reassoc_flow_chains::delete_dead_flow_chains()
{
    List<Op*> ops_to_delete;
    int num_deleted=0;
    for(List_iterator<Flow_chain*> lf(flow_chains); lf!=0; lf++){
	Flow_chain *fc = *lf;
	if (fc->marked_for_deletion()){
	    //fc->delete_chain_ops();
	    fc->get_unique_ops(ops_to_delete);
	    //num_deleted++;
	    delete fc;
	    flow_chains.remove(fc);
	}
    }
    for(List_iterator<Op*> li(ops_to_delete); li!=0; li++){
	Op* op = *li;
	cdbg << "op removed = " << op->id() << endl;
	El_remove_op(op);
    }

    return num_deleted;
}
//##################################################################
int
Reassoc_flow_chains::chain_set_size(int ch_id)
{
    int size =0;
    for(List_iterator<Flow_chain*> lf(flow_chains); lf!=0; lf++){
	Flow_chain *fc = *lf;
	if (fc->id() == ch_id)
	    size++;
    }
    return size;
}
//##################################################################
void
Reassoc_flow_chains::gen_code(Region* rgn, Reaching_defs_info* rdefs_info,
			      Hash_map<Op*, DepLen_OpInfo*>& deplen_info)
{
    List<int> sets_to_rename;
    Flow_chain *fc;
    List_iterator<Flow_chain*> lf;

/*
    int num_chains = 0;
    for(lf(flow_chains); lf!=0; lf++){
	fc = *lf;
	if (fc->is_reassoc_profitable()){
	    num_chains = chain_set_size(fc->id());
	    if (El_do_scalar_reassoc_only && num_chains>1)
		continue;
	    fc->gen_code(rgn, rdefs_info, deplen_info);
	    if (!sets_to_rename.is_member(fc->id()))
		sets_to_rename.add_tail(fc->id());
	}
    }
    if (El_do_scalar_reassoc_only) 
	return;

    for(lf(flow_chains); lf!=0; lf++){
	fc = *lf;
	if (sets_to_rename.is_member(fc->id())){
	    fc->gen_code(rgn, rdefs_info, deplen_info);
	}
    }
*/
    
    if (El_do_scalar_reassoc_only){
	for(lf(flow_chains); lf!=0; lf++){
	    fc = *lf;
	    if (fc->is_reassoc_profitable()){
		if (fc->is_scalar(rdefs_info)){
		    fc->gen_code(rgn, rdefs_info, deplen_info);
		}
	    }
	}
    } else {
	/*
	for(lf(flow_chains); lf!=0; lf++){
	    fc = *lf;
	    if (fc->is_reassoc_profitable()){
		List<Flow_chain*> sub_chains;
		fc->get_sub_chains(rdefs_info, sub_chains);
		for(List_iterator<Flow_chain*> lfc(sub_chains); lfc!=0; lfc++){
		    Flow_chain* sfc = *lfc;
		    sfc->gen_code(rgn, rdefs_info, deplen_info);
		}
		//NEED TO GET RID OF THESE SFC's;
	    }
	}
	*/
	//find the longest flow chain;
	Flow_chain *fc_max = NULL;
	int max = 0;
	for(lf(flow_chains); lf!=0; lf++){
	    fc = *lf;
	    if (fc->size() > max){
		fc_max = fc;
		max = fc->size();
	    }
	}
	if (fc_max->is_reassoc_profitable()){
	    List<Flow_chain*> sub_chains;
	    fc_max->get_sub_chains(rdefs_info, sub_chains);
	    for(List_iterator<Flow_chain*> lfc(sub_chains); lfc!=0; lfc++){
		Flow_chain* sfc = *lfc;
		sfc->gen_code(rgn, rdefs_info, deplen_info);
	    }
	    fc_max->mark_for_deletion();
	}
    }

}

//##################################################################
// this function assumes the edge drawing is available..
//##################################################################
int
Reassoc_flow_chains::compute_flow_chains(List<Edge*>& flow_edges, int flag)
{
    int edge_added;
    Flow_chain *new_fc;

    for(List_iterator<Edge*> li(flow_edges); li!=0; li++){
	Edge* e = *li;

	//consider only register flow edges;
	if (!is_interesting(e)) 
	    continue;

	//cdbg << "e:: " << e->src()->id() << " -> " << e->dest()->id();

	edge_added = false;
	for(List_iterator<Flow_chain*> lf(flow_chains); lf!=0; lf++){
	    Flow_chain* fc = *lf;
	    
	    //consider those chains which intersect with this flow edge;
	    if (fc->is_interesting(e)){

		//the chain is split due to this edge which happens only;
		//when the edge intersects at an internal node of the chain;
		if (fc->will_split_chain(e)){
		    new_fc = new Flow_chain(fc->type());
		    int r = fc->sub_chain(e, new_fc);
		    if (r==-1){
			delete fc; //the old chain lost all edges due to a merge;
			flow_chains.remove(fc);
		    }
		    if (r>0) {
			//new subchain has atleast one edge;
			flow_chains.add_tail(new_fc);
		    } else {
			delete new_fc;
		    }
		} else {
		    fc->insert(e);
		}
		edge_added = true;
		break;
	    }
	    //check all chains w.r.t. this edge;
	}

	if ((!edge_added) && (!is_switch(e->dest()))) {
	    new_fc = new Flow_chain(e->src()->opcode());
	    new_fc->id(Reassoc_flow_chains::new_chain_set_id());
	    new_fc->append(e);
	    flow_chains.add_tail(new_fc);
	}
	/*
	if (!edge_added){
	    if (is_switch(e->dest())){
		new_fc = new Flow_chain(e->src()->opcode());
	    } else if (is_switch(e->src())) {
		new_fc = new Flow_chain(e->dest()->opcode());
	    } else {
		new_fc = new Flow_chain(e->src()->opcode());
	    }		
	    new_fc->id(Reassoc_flow_chains::new_chain_set_id());
	    new_fc->append(e);
	    flow_chains.add_tail(new_fc);
	} */

	while(merge_chains());
    }
    return 0;
}

//##################################################################
// this function assumes the edge drawing is available..
//##################################################################
int
Reassoc_flow_chains::compute_flow_chains(Region* rgn, int flag)
{
    int edge_added;
    Flow_chain *new_fc;

    for(Region_all_internal_edges li(rgn); li!=0; li++){
	Edge* e = *li;

	//consider only register flow edges;
	if (!is_interesting(e)) 
	    continue;

	//cdbg << "e:: " << e->src()->id() << " -> " << e->dest()->id();

	edge_added = false;
	for(List_iterator<Flow_chain*> lf(flow_chains); lf!=0; lf++){
	    Flow_chain* fc = *lf;
	    
	    //consider those chains which intersect with this flow edge;
	    if (fc->is_interesting(e)){

		//the chain is split due to this edge which happens only;
		//when the edge intersects at an internal node of the chain;
		if (fc->will_split_chain(e)){
		    new_fc = new Flow_chain(fc->type());
		    int r = fc->sub_chain(e, new_fc);
		    if (r==-1){
			delete fc; //the old chain lost all edges due to a merge;
			flow_chains.remove(fc);
		    }
		    if (r>0) {
			//new subchain has atleast one edge;
			flow_chains.add_tail(new_fc);
		    } else {
			delete new_fc;
		    }
		} else {
		    fc->insert(e);
		}
		edge_added = true;
		break;
	    }
	    //check all chains w.r.t. this edge;
	}

	if ((!edge_added) && (!is_switch(e->dest()))) {
	    new_fc = new Flow_chain(e->src()->opcode());
	    new_fc->id(Reassoc_flow_chains::new_chain_set_id());
	    new_fc->append(e);
	    flow_chains.add_tail(new_fc);
	}

	while(merge_chains());
    }
    return 0;
}


//##################################################################
/*
int
Reassoc_flow_chains::compute_flow_chains(Region* rgn, int flag)
{
    int edge_added;

    for(Region_all_internal_edges li(rgn); li!=0; li++){
	Edge* e = *li;

	if (!is_interesting(e)) 
	    continue;

	edge_added = false;
	for(List_iterator<Flow_chain*> lf(flow_chains); lf!=0; lf++){
	    Flow_chain* fc = *lf;
	    
	    if (fc->is_interesting(e)){
		if (fc->will_split_chain(e)){
		    Flow_chain *new_fc = new Flow_chain(fc->type());
		    int r = fc->sub_chain(e, new_fc);
		    if (r==-1){
			//the old chain lost all edges due to a merge;
			delete fc;
			flow_chains.remove(fc);
		    }
		    if (r>0) {
			//new subchain has atleast one edge;
			flow_chains.add_tail(new_fc);
		    }
		} else {
		    fc->insert(e);
		}
		edge_added = true;
		break;
	    }
	}

	if (!edge_added){
	    Flow_chain* new_fc = new Flow_chain(e->src()->opcode());
	    new_fc->append(e);
	    flow_chains.add_tail(new_fc);
	}

	while(merge_chains());

    }
    return 0;
}
*/
//##################################################################
void 
Reassoc_flow_chains::delete_flow_chains()
{
    for(List_iterator<Flow_chain*> lf(flow_chains); lf!=0; lf++){
	Flow_chain* fc = *lf;
	delete fc;
    }
    flow_chains.clear();

}
//##################################################################;
void 
Reassoc_flow_chains::display_flow_chains(ostream& os)
{
    for(List_iterator<Flow_chain*> li(flow_chains); li!=0; li++){
	Flow_chain* fc = *li;
	os << (*fc) << endl;
    }
}

//##################################################################;
int
Reassoc_flow_chains::new_chain_set_id(void)
{
    chain_set_id_counter++;
    return chain_set_id_counter;
}
//##################################################################;


