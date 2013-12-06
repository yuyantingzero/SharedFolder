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
//      File:           res_len.cpp
//      Authors:        Suren Talla
//      Created:        June 1997
//      Description:    definitions for DepLen class members
//
///////////////////////////////////////////////////////////////////////////

#include "res_len.h"
#include "attributes.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "intf.h"
#include "opcode.h"
#include "el_data_cpr_init.h"

//######################################################################
//       ResLen_OpInfo
//######################################################################

ResLen_OpInfo::ResLen_OpInfo()
{
    reslen = -1;
}

ResLen_OpInfo::~ResLen_OpInfo()
{
}


//######################################################################
//       ResLen
//######################################################################

ResLen::ResLen() 
{  
  
}

//----------------------------------------------------------------------
ResLen::~ResLen()
{
}

//----------------------------------------------------------------------
// Code cut-pasted from src/Cache/ls_cacheopt.cpp. Only modification
// being : 2 lines deleted (dead code in the source ??!!??)
//----------------------------------------------------------------------
double 
ResLen::max_rmsl_res_len(Compound_region* cr)
{
    // used to hold I/O pattern for operation;
    char iopat[MAXIOPATLEN];  
    
    RMSL_alloc();
    RMSL_init();
    
    double res_len = .0;
    double max_res_len = .0;

    Hash_map<Op*, int>  exitops_map(hash_op_ptr, 256);
    for(Region_exit_ops reops(cr); reops!=0; reops++){
	if (!exitops_map.is_bound(*reops))
	    exitops_map.bind(*reops, 1);
    }

    for (Region_ops_C0_order ops(cr); ops != 0; ops++) {
	Op* op = *ops;
	if (el_opcode_to_string_map.is_bound((op)->opcode())) {
	    (op)->build_io_pat(iopat);
	    //RMSL_nextop(el_opcode_to_string_map.value((op)->opcode()), iopat);
	    //tangw (06-03-02)
	    RMSL_nextop(get_mdes_opcode_string(op), iopat);


	}
	if (exitops_map.is_bound(op)){
	    //implies op is an exit op;
	    res_len = (double)RMSL_value();
	    max_res_len = (max_res_len<res_len)? res_len : max_res_len;
	}
    }  

    exitops_map.clear();
    return max_res_len;

    /*
    // used to hold I/O pattern for operation;
    char iopat[MAXIOPATLEN];  
    
    RMSL_alloc();
    RMSL_init();
    
    for (Region_ops_C0_order ops(cr); ops != 0; ops++) {
	Op* op = *ops;
	if (el_opcode_to_string_map.is_bound((op)->opcode())) {
	    (op)->build_io_pat(iopat);
	    //RMSL_nextop(el_opcode_to_string_map.value((op)->opcode()), iopat);
	    //tangw 06-03-02
	    RMSL_nextop(get_mdes_opcode_string(op), iopat);


	}
    }
    return (double)RMSL_value();
    */
}


//----------------------------------------------------------------------
// version above weighted with exit frequencies...
//----------------------------------------------------------------------
double 
ResLen::weighted_rmsl_res_len(Compound_region* cr)
{
    // used to hold I/O pattern for operation;
    char iopat[MAXIOPATLEN];  
    
    RMSL_alloc();
    RMSL_init();
    
    double cum_res_len = .0;
    double total_freq = 0;
    
    for (Region_ops_C0_order ops(cr); ops != 0; ops++) {
	Op* op = *ops;
	if (el_opcode_to_string_map.is_bound((op)->opcode())) {
	    (op)->build_io_pat(iopat);
	    //RMSL_nextop(el_opcode_to_string_map.value((op)->opcode()), iopat);
	    //tangw
	    RMSL_nextop(get_mdes_opcode_string(op), iopat);

	}
	for(Region_exit_edges eedges(cr); eedges != 0; eedges++){
	    Edge* e = *eedges;
	    Op* eop = e->src();
	    if (eop == op){
		Control_flow_freq *cfreq = get_control_flow_freq(e);
		total_freq += cfreq->freq;
		cum_res_len += (cfreq->freq * (double)RMSL_value());
		break;
	    }
	}
    }  
    double weighted_rmsl_reslen = 
	(total_freq>0) ? (cum_res_len/total_freq) : 0;

    return weighted_rmsl_reslen;
}

void
ResLen::rmsl_res_len(Compound_region *cr, Hash_map<Op*, ResLen_OpInfo*>& hOpInfo)
{
    // used to hold I/O pattern for operation;
    char iopat[MAXIOPATLEN];
    ResLen_OpInfo* rinfo;
    Op *op;

    //init the hash map structure;
    for(Region_ops_C0_order ops1(cr); ops1 != 0; ops1++){
        op = *ops1;
        if (!hOpInfo.is_bound(op)){
            rinfo = new ResLen_OpInfo();
            hOpInfo.bind(op, rinfo);
        }
    }

    RMSL_alloc();
    RMSL_init();

    for (Region_ops_C0_order ops(cr); ops != 0; ops++) {
        Op* op = *ops;
        if (el_opcode_to_string_map.is_bound((op)->opcode())) {
            (op)->build_io_pat(iopat);
            //RMSL_nextop(el_opcode_to_string_map.value((op)->opcode()), iopat);
	    //tangw (06-03-02)
	    RMSL_nextop(get_mdes_opcode_string(op), iopat);


        }
	rinfo = hOpInfo.value(op);
	rinfo->reslen = RMSL_value();
    } 
}


//----------------------------------------------------------------------
double 
ResLen::max_ifmb_res_len(Compound_region* cr)
{

    unsigned long optypes_vec[OP_TYPES_VECTOR_SZ];
    
    for(int i=0; i<OP_TYPES_VECTOR_SZ; i++){
	optypes_vec[i] = 0;
    }

    //compute the "total" resource restricted to I,F,M,B's usage Vector;    
    for (Region_ops_C0_order ops(cr); ops != 0; ops++) {
	Op* op = *ops;
	if (is_no_op(op)){
	    optypes_vec[NO_OP]++;
	} else if (is_pseudo(op)) {
	    //these do not consume any m/c cycles;
	    optypes_vec[PSEUDO_OP]++;
	} else if (is_memory(op)){
	    //load, store;
	    optypes_vec[MEMORY_OP]++;
	} else if (is_ialu(op)){
	    optypes_vec[IALU_OP]++;
	} else if (is_cmpp(op)){
	    optypes_vec[IALU_OP]++;
	} else if (is_falu(op)){
	    optypes_vec[FALU_OP]++;
	} else if (is_pbr(op)) {
	    // NOTE:: this may be different in future;
	    optypes_vec[FALU_OP]++;
	} else if (is_control_switch(op)){  
	    optypes_vec[BRANCH_OP]++;
	} else if (op->opcode() == PRED_CLEAR) {
	    optypes_vec[UNKNOWN_OP]++;
	} else {
	    //cerr << "unknown op = " << op->id() << " " << *op << endl << flush;
	    El_punt("ResLen::max_ifmb_res_len : unknown op - %d", op->id());
	    optypes_vec[UNKNOWN_OP]++;
	}
    }
  
    double max_ifmb_reslen = -1; 

    //max_IfmbResLen is the max of the ratio of number of ops of each type ;
    //to the number of alternatives (resources) for that op;
    int num_alts = 0;
    for(int op_type=0; op_type<OP_TYPES_VECTOR_SZ; op_type++){
	
	if (op_type==NO_OP || op_type==PSEUDO_OP || op_type==UNKNOWN_OP) 
	    continue;
	
	num_alts = ResLen::get_num_alternatives(op_type);
	double r = (num_alts>0) ? ((double)optypes_vec[op_type])/num_alts : ELCOR_MAX_INT;
	
	//cerr << "o = " << op_type << " n = " << num_alts << " r = " << r << endl << flush;
	max_ifmb_reslen = (max_ifmb_reslen < r) ? r : max_ifmb_reslen;
    }

    return max_ifmb_reslen;

}


//----------------------------------------------------------------------
double 
ResLen::weighted_ifmb_res_len(Compound_region* cr)
{
    unsigned long optypes_vec[OP_TYPES_VECTOR_SZ];
    double total_freq = cr->weight;
    double w_ifmb_reslen = .0;

    //if this compound region is not executed at all...;
    if (total_freq == 0)
	return w_ifmb_reslen;

    //need to compute the "total" resource usage Vector;
    for(int i=0; i<OP_TYPES_VECTOR_SZ; i++)
	optypes_vec[i] = 0;

    Region_exit_edges  exit_edges(cr);
    
    for (Region_ops_C0_order ops(cr); ops != 0; ops++) {
	Op* op = *ops;
	if (is_no_op(op)){
	    optypes_vec[NO_OP]++;
	} else if (is_pseudo(op)) {
	    //do not consume any m/c cycles;
	    optypes_vec[PSEUDO_OP]++;
	} else if (is_memory(op)){
	    //load, store;
	    optypes_vec[MEMORY_OP]++;
	} else if (is_ialu(op)){
	    optypes_vec[IALU_OP]++;
	} else if (is_cmpp(op)){
	    optypes_vec[IALU_OP]++;
	} else if (is_falu(op)){
	    optypes_vec[FALU_OP]++;
	} else if (is_pbr(op)) {
	    // to be changed to ...;
	    optypes_vec[FALU_OP]++;
	} else if (is_control_switch(op)){  
	    //cout << "branch = " << op->id() << " - " << *op << endl;
	    optypes_vec[BRANCH_OP]++;
	} else if (op->opcode() == PRED_CLEAR) {
	    optypes_vec[UNKNOWN_OP]++;
	} else {
	    //cerr << "unknown op = " << op->id() << " opcode = " << op->opcode() << endl << flush;
	    El_punt("ResLen::weighted_ifmb_res_len : unknown op - %d", op->id());
	    optypes_vec[UNKNOWN_OP]++;
	}
	if (is_branch(op)) {
	    
	    //there wont be any exit edges for the epilogue block ;
	    //the only exit op  is RTS whose "freq" = region weight;
	    if (!exit_edges.iterator) {
		w_ifmb_reslen = partial_resval(optypes_vec, total_freq, total_freq);
		return w_ifmb_reslen;
	    }
	    
	    //compute the res estimate for the portion upto this branch op;
	    //NOTE :: here we are assuming that the exit_edges come in the same; 
	    //order as the the branch ops. Need to change code to be sure of this;
	    Edge* e = *exit_edges;
	    Control_flow_freq *cfreq = get_control_flow_freq(e);
	    //cout << "\n    branch = " << op->id() << " e->src() = " << e->src()->id() ;      
	    double tmp_resval = partial_resval(optypes_vec, total_freq, cfreq->freq);
	    exit_edges++;
	    //cout << "          t = " << tmp_resval << " f = " << cfreq->freq << endl;
	    w_ifmb_reslen += tmp_resval;
	}
    }

    return w_ifmb_reslen;

}


//----------------------------------------------------------------------
double 
ResLen::partial_resval(unsigned long *vec, double total_freq, 
		       double this_freq)
{

    double tmp_resval = -1;
    //resval is the max of the ratio of number of ops of each type ;
    //to the number of alternatives (resources) for that op;
    int i, num_alts = 0;
    for(i=0; i<OP_TYPES_VECTOR_SZ; i++){
	
	if (i==NO_OP || i==PSEUDO_OP || i==UNKNOWN_OP) 
	    continue;
	
	num_alts = ResLen::get_num_alternatives(i);
	
	double r = (num_alts>0) ? ((double)vec[i])/num_alts : ELCOR_MAX_INT;
	tmp_resval = (tmp_resval < r) ? r : tmp_resval;
    }

    tmp_resval *= (this_freq/total_freq);
    return tmp_resval;

}

//----------------------------------------------------------------------
// temporary hack.. in actual mode, it should query the mdes interface
// to get the number of resources of each type - 
//----------------------------------------------------------------------
int 
ResLen::get_num_alternatives(int op_type)
{
    extern char* El_lmdes_file_name;
    int num_alts = 0;
    
    if (strcmp(El_lmdes_file_name, 
	       "/car/azrael/talla/benchmarks/machines/medium4221.lmdes2") == 0){
	switch (op_type) {
	  case NO_OP:
	    break;
	  case PSEUDO_OP:
	    break;
	  case UNKNOWN_OP:
	    break;
	  case IALU_OP:
	    num_alts = 4;
	    break;
	  case FALU_OP:
	    num_alts = 2;
	    break;
	  case MEMORY_OP:
	    num_alts = 2;
	    break;
	  case BRANCH_OP:
	    num_alts = 1;
	    break;
	  default:
	    break;
	}
    } else if (strcmp(El_lmdes_file_name, 
		    "/car/azrael/talla/benchmarks/machines/narrow1111.lmdes2") == 0){
	switch (op_type) {
	  case NO_OP:
	    break;
	  case PSEUDO_OP:
	    break;
	  case UNKNOWN_OP:
	    break;
	  case IALU_OP:
	    num_alts = 1;
	    break;
	  case FALU_OP:
	    num_alts = 1;
	    break;
	  case MEMORY_OP:
	    num_alts = 1;
	    break;
	  case BRANCH_OP:
	    num_alts = 1;
	    break;
	  default:
	    break;
	}
    } else if (strcmp(El_lmdes_file_name, 
		      "/car/azrael/talla/benchmarks/machines/reallybig8664.lmdes2") == 0){
	switch (op_type) {
	  case NO_OP:
	    break;
	  case PSEUDO_OP:
	    break;
	  case UNKNOWN_OP:
	    break;
	  case IALU_OP:
	    num_alts = 8;
	    break;
	  case FALU_OP:
	    num_alts = 6;
	    break;
	  case MEMORY_OP:
	    num_alts = 4;
	    break;
	  case BRANCH_OP:
	    num_alts = 4;
	    break;
	  default:
	    break;
	}
    } else if (strcmp(El_lmdes_file_name, 
		      "/car/azrael/talla/benchmarks/machines/wide4422.lmdes2") == 0){
	switch (op_type) {
	  case NO_OP:
	    break;
	  case PSEUDO_OP:
	    break;
	  case UNKNOWN_OP:
	    break;
	  case IALU_OP:
	    num_alts = 4;
	    break;
	  case FALU_OP:
	    num_alts = 4;
	    break;
	  case MEMORY_OP:
	    num_alts = 2;
	    break;
	  case BRANCH_OP:
	    num_alts = 2;
	    break;
	  default:
	    break;
	}
    } else if (strcmp(El_lmdes_file_name, 
		      "/car/azrael/talla/benchmarks/machines/wide6442.lmdes2") == 0){
	switch (op_type) {
	  case NO_OP:
	    break;
	  case PSEUDO_OP:
	    break;
	  case UNKNOWN_OP:
	    break;
	  case IALU_OP:
	    num_alts = 6;
	    break;
	  case FALU_OP:
	    num_alts = 4;
	    break;
	  case MEMORY_OP:
	    num_alts = 4;
	    break;
	  case BRANCH_OP:
	    num_alts = 2;
	    break;
	  default:
	    break;
	}
    } 
  
    return num_alts;

}

//---------------------------------------------------------------------
double 
ResLen::max_ancestors_res_len(Compound_region* cr)
{
    unsigned long optypes_vec[OP_TYPES_VECTOR_SZ];
    
    for(int i=0; i<OP_TYPES_VECTOR_SZ; i++){
	optypes_vec[i] = 0;
    }

    double max_ancestors_res_len = 0;

    for(Region_exit_ops ops(cr); ops != 0; ops++){
	Op* eop = *ops;

	//cout << "eop = " << eop->id() << "  -- " << flush;
	//iterator over ancestors of op;
	for(Op_ancestors op_ansc(eop, cr, NON_CONTROL0_INEDGES); op_ansc != 0; op_ansc++){
	    Op* op = *op_ansc;
	    
	    //cout << " " << op->id() ;
	    if (is_no_op(op)){
		optypes_vec[NO_OP]++;
	    } else if (is_pseudo(op)) {
		//these do not consume any m/c cycles;
		optypes_vec[PSEUDO_OP]++;
	    } else if (is_memory(op)){
		//load, store;
		optypes_vec[MEMORY_OP]++;
	    } else if (is_ialu(op)){
		optypes_vec[IALU_OP]++;
	    } else if (is_cmpp(op)){
		optypes_vec[IALU_OP]++;
	    } else if (is_falu(op)){
		optypes_vec[FALU_OP]++;
	    } else if (is_pbr(op)) {
		// NOTE:: this may be different in future;
		optypes_vec[FALU_OP]++;
	    } else if (is_control_switch(op)){  
		optypes_vec[BRANCH_OP]++;
	    } else if (op->opcode() == PRED_CLEAR) {
		optypes_vec[UNKNOWN_OP]++;
	    } else {
		//cerr << "unknown op = " << op->id() << " " << *op << endl << flush;
		El_punt("ResLen::max_ifmb_res_len : unknown op - %d", op->id());
		optypes_vec[UNKNOWN_OP]++;
	    }
	}
	//cout << endl << flush;
	double tmp_max_ancestors_res_len = -1;
	int num_alts = 0;

	for(int op_type=0; op_type<OP_TYPES_VECTOR_SZ; op_type++){
	    if (op_type==NO_OP || op_type==PSEUDO_OP || op_type==UNKNOWN_OP) 
		continue;
	    num_alts = ResLen::get_num_alternatives(op_type);
	    double r = (num_alts>0) ? ((double)optypes_vec[op_type])/num_alts : ELCOR_MAX_INT;
	    //cerr << "o = " << op_type << " n = " << num_alts << " r = " << r << endl << flush;
	    tmp_max_ancestors_res_len = (tmp_max_ancestors_res_len < r) ? r 
		                        : tmp_max_ancestors_res_len;
	}

	for(int i=0; i<OP_TYPES_VECTOR_SZ; i++){
	    optypes_vec[i] = 0;
	}
	max_ancestors_res_len = 
	     (max_ancestors_res_len<tmp_max_ancestors_res_len) 
             ? tmp_max_ancestors_res_len
             : max_ancestors_res_len;
    }
    
    return max_ancestors_res_len;
    
}

//----------------------------------------------------------------------
double 
ResLen::weighted_ancestors_res_len(Compound_region* cr)
{
    unsigned long optypes_vec[OP_TYPES_VECTOR_SZ];
    
    //should add weight() member to cr ..;
    double total_freq = cr->weight;
    
    double w_ancestors_reslen = .0;

    //if this compound region is not executed at all...;
    if (total_freq == 0){
	return w_ancestors_reslen;
    }
    
    //need to compute the "total" resource usage Vector;
    for(int i=0; i<OP_TYPES_VECTOR_SZ; i++){
	optypes_vec[i] = 0;
    }

    Region_exit_edges  exit_edges(cr);
    
    //there wont be any exit edges for the epilogue block ;
    //the only exit op  is RTS whose "freq" = region weight;
    if (!exit_edges.iterator) {
	for (Region_ops_C0_order ops(cr); ops != 0; ops++) {
	    Op* op = *ops;
	    if (is_no_op(op)){
		optypes_vec[NO_OP]++;
	    } else if (is_pseudo(op)) {
		//do not consume any m/c cycles;
		optypes_vec[PSEUDO_OP]++;
	    } else if (is_memory(op)){
		//load, store;
		optypes_vec[MEMORY_OP]++;
	    } else if (is_ialu(op)){
		optypes_vec[IALU_OP]++;
	    } else if (is_cmpp(op)){
		optypes_vec[IALU_OP]++;
	    } else if (is_falu(op)){
		optypes_vec[FALU_OP]++;
	    } else if (is_pbr(op)) {
		// to be changed to ...;
		optypes_vec[FALU_OP]++;
	    } else if (is_control_switch(op)){  //is_branch(op)){
		//cout << "branch = " << op->id() << " - " << *op << endl;
		optypes_vec[BRANCH_OP]++;
	    } else if (op->opcode() == PRED_CLEAR) {
		optypes_vec[UNKNOWN_OP]++;
	    } else {
		//cerr << "unknown op = " << op->id() << " opcode = " << op->opcode() << endl << flush;
		El_punt("ResLen::weighted_ifmb_res_len : unknown op - %d", op->id());
		optypes_vec[UNKNOWN_OP]++;
	    }
	}
	w_ancestors_reslen = partial_resval(optypes_vec, total_freq, total_freq);
	return w_ancestors_reslen;
    }


    for(; exit_edges != 0; exit_edges++){
	Edge *e = *exit_edges;
	Op* eop = e->src();
	//cout << "\n EOP = " << eop->id() << " " ;
	//iterator over ancestors of op;
	for(Op_ancestors op_ansc(eop, cr, NON_CONTROL0_INEDGES); op_ansc != 0; op_ansc++){
	    Op* op = *op_ansc;
	    if (is_no_op(op)){
		optypes_vec[NO_OP]++;
	    } else if (is_pseudo(op)) {
		//do not consume any m/c cycles;
		optypes_vec[PSEUDO_OP]++;
	    } else if (is_memory(op)){
		//load, store;
		optypes_vec[MEMORY_OP]++;
	    } else if (is_ialu(op)){
		optypes_vec[IALU_OP]++;
	    } else if (is_cmpp(op)){
		optypes_vec[IALU_OP]++;
	    } else if (is_falu(op)){
		optypes_vec[FALU_OP]++;
	    } else if (is_pbr(op)) {
		// to be changed to ...;
		optypes_vec[FALU_OP]++;
	    } else if (is_control_switch(op)){  //is_branch(op)){
		//cout << "branch = " << op->id() << " - " << *op << endl;
		optypes_vec[BRANCH_OP]++;
	    } else if (op->opcode() == PRED_CLEAR) {
		optypes_vec[UNKNOWN_OP]++;
	    } else {
		//cerr << "unknown op = " << op->id() << " opcode = " << op->opcode() << endl << flush;
		El_punt("ResLen::weighted_ancestors_res_len : unknown op - %d", op->id());
		optypes_vec[UNKNOWN_OP]++;
	    }
	}
	//NOTE :: here we are assuming that the exit_edges come in the same ;
	//order as the the branch ops. Need to change code to be sure of this;
	Control_flow_freq *cfreq = get_control_flow_freq(e);
	double tmp_resval = partial_resval(optypes_vec, total_freq, cfreq->freq);
	
	//cout << "tmp = " << tmp_resval << " tot = " 
	//     << total_freq << " freq = " << cfreq->freq << endl << flush;

	for(int i=0; i<OP_TYPES_VECTOR_SZ; i++){
	    optypes_vec[i] = 0;
	}
	w_ancestors_reslen += tmp_resval;
    }

    return w_ancestors_reslen;
}
//------------------------------------------------------------------
double 
ResLen::res_len_estimate(Compound_region* rgn)
{
    switch (El_reslen_estimate_type){
      case RMSL_RES_LEN:
	{
	    if (El_reslen_estimate_ops==C0_OPS_RES_LEN){
		switch(El_reslen_estimate_method){
		  case MAX_RES:
		    return max_rmsl_res_len(rgn);
		  case WEIGHTED_RES:
		    return weighted_rmsl_res_len(rgn);
		  default:
		    El_punt("ResLen::res_len_estimate: illegal estimate method");
		    return (0);
		}
	    }
	    else if (El_reslen_estimate_ops==ANSCESTOR_OPS_RES_LEN){
		El_punt("ResLen::res_len_estimate: illegal val for estimate ops");
	    }
	    else {
		El_punt("ResLen::res_len_estimate: illegal val for estimate ops");
	    }
	}
	break;
      default:
	El_punt("ResLen::res_len_estimate: illegal estimate type must use RMSL");
	break;
#if 0
      case IFMB_RES_LEN:
	{
	    if (El_reslen_estimate_ops==C0_OPS_RES_LEN){
		switch(El_reslen_estimate_method){
		  case MAX_RES:
		    return max_ifmb_res_len(rgn);
		  case WEIGHTED_RES:
		    return weighted_ifmb_res_len(rgn);
		  default:
		    return 0;
		}
	    } else if (El_reslen_estimate_ops==ANSCESTOR_OPS_RES_LEN){
		switch(El_reslen_estimate_method){
		  case MAX_RES:
		    return max_ancestors_res_len(rgn);
		  case WEIGHTED_RES:
		    return weighted_ancestors_res_len(rgn);
		  default:
		    return 0;
		}
	    }
	}
	break;
      default:
	break;
#endif
    }

    return 0;
}

//------------------------------------------------------------------
double 
ResLen::res_len_estimate(Compound_region* rgn, 
			 List<OpPtr_Double>& exitops_reslen)

{
    //at the moment the exit_ops i/p is not used. However,;
    //in future, the res-len fns can be updated to compute;
    //values w.r.t. the subset of exit ops listed in exit_ops;

    //so for now this is just a repeat of the above function;

    return res_len_estimate(rgn);

}


void ResLen::res_len_estimate(Compound_region *rgn,
				Hash_map<Op*, ResLen_OpInfo*>& hOpInfo)
{
    if (El_reslen_estimate_type == RMSL_RES_LEN) {
	if (El_reslen_estimate_ops == C0_OPS_RES_LEN) {
	    rmsl_res_len(rgn, hOpInfo);
	}
	else {
	    El_punt("ResLen::res_len_estimate: illegal val for estimate ops");
	}
    }
    else {
	El_punt("ResLen::res_len_estimate: illegal estimate type, use RMSL!");
    }
}
