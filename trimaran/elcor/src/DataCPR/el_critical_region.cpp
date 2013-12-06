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
//      File:           el_critical_region.cpp
//      Authors:        Suren Talla
//      Created:        July 1997
//      Description:    definitions for class Critical_region_info.
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include "el_critical_region.h"
#include "iterators.h"
#include "dbg.h"
#include "el_dot_tools.h"
#include "el_debug_init.h"
#include "opcode_properties.h"
#include "attributes.h"

using namespace std;

extern void el_dot_print_file_header(ostream& os, char* orntn, char* name) ;
extern void el_dot_print_file_tail(ostream& os) ;
extern void el_dot_process_dot_file(char* file_name, char* orntn, int bg) ;



// ------------------------------------------------------------------;
Critical_region_info::Critical_region_info()
 : critical_edges(hash_op_ptr, 2000), 
   critical_ops(hash_op_ptr, 2000), 
   dep_info(hash_op_ptr, INIT_HASH_MAP_SIZE),
   exit_ops_map(hash_op_ptr, INIT_HASH_MAP_SIZE),
   num_exit_ops(0)
{}

Critical_region_info::~Critical_region_info(){}


// ------------------------------------------------------------------;
List<Op*>*
Critical_region_info::get_critical_ops(Op* exitop, List<Op*>& lcrit)
{
    assert(exit_ops_map.is_bound(exitop));
    for(List_iterator<Op*> li(*critical_ops.value(exitop));
	li != 0; li++){
	Op* op = *li;
	if (!lcrit.is_member(op)) 
	    lcrit.add_tail(op);
    }
    return &lcrit;
}

List<Op*>*  
Critical_region_info::get_all_critical_ops(List<Op*>& lcrit)
{
    for(int k=0; k<num_exit_ops; k++){
	Op* exitop = rev_exit_ops[k];
	for(List_iterator<Op*> li(*critical_ops.value(exitop));
	    li != 0; li++){
	    Op* op = *li;
	    if (!lcrit.is_member(op)) 
		lcrit.add_tail(op);
	}
    }
    return &lcrit;
}

// ------------------------------------------------------------------;
List<Edge*>*
Critical_region_info::get_critical_edges(Op* exitop, List<Edge*>& lcrit)
{
    assert(exit_ops_map.is_bound(exitop));
    for(List_iterator<Edge*> li(*critical_edges.value(exitop));
	li != 0; li++){
	Edge* e = *li;
	if (!lcrit.is_member(e)) 
	    lcrit.add_tail(e);
    }
    return &lcrit;
}

List<Edge*>*  
Critical_region_info::get_all_critical_edges(List<Edge*>& lcrit)
{
    for(int k=0; k<num_exit_ops; k++){
	Op* exitop = rev_exit_ops[k];
	for(List_iterator<Edge*> li(*critical_edges.value(exitop));
	    li != 0; li++){
	    Edge* e = *li;
	    if (!lcrit.is_member(e))
		lcrit.add_tail(e);
	}
    }
    return &lcrit;
}

List<Edge*>*  
Critical_region_info::get_anti_critical_edges(Op* exitop, List<Edge*>& lanti)
{
    assert(exit_ops_map.is_bound(exitop));
    for(List_iterator<Edge*> li(*critical_edges.value(exitop));
	li != 0; li++){
	Edge* e = *li;
	if (e->is_reg_anti() && (!lanti.is_member(e))) 
	    lanti.add_tail(e);
    }
    return &lanti;
}


List<Edge*>*  
Critical_region_info::get_all_anti_critical_edges(List<Edge*>& lanti)
{
    for(int i=0; i<num_exit_ops; i++){
	Op* exitop = rev_exit_ops[i];
	get_anti_critical_edges(exitop, lanti);
    }
    return &lanti;
}


List<Edge*>*  
Critical_region_info::get_output_critical_edges(Op* exitop, List<Edge*>& loutput)
{
    assert(exit_ops_map.is_bound(exitop));
    for(List_iterator<Edge*> li(*critical_edges.value(exitop));
	li != 0; li++){
	Edge* e = *li;
	if (e->is_reg_out() && (!loutput.is_member(e)))
	    loutput.add_tail(e);
    }
    return &loutput;
}


List<Edge*>*  
Critical_region_info::get_all_output_critical_edges(List<Edge*>& loutput)
{
    for(int i=0; i<num_exit_ops; i++){
	Op* exitop = rev_exit_ops[i];
	get_output_critical_edges(exitop, loutput);
    }
    return &loutput;
}


List<Edge*>*  
Critical_region_info::get_flow_critical_edges(Op* exitop, List<Edge*>& lflow)
{
    assert(exit_ops_map.is_bound(exitop));
    for(List_iterator<Edge*> li(*critical_edges.value(exitop));
	li != 0; li++){
	Edge* e = *li;
	if (e->is_reg_flow() && (!lflow.is_member(e))) 
	    lflow.add_tail(e);
    }
    return &lflow;
}


List<Edge*>*  
Critical_region_info::get_all_flow_critical_edges(List<Edge*>& lflow)
{
    for(int i=0; i<num_exit_ops; i++){
	Op* exitop = rev_exit_ops[i];
	get_flow_critical_edges(exitop, lflow);
    }
    return &lflow;
}


// ------------------------------------------------------------------;
double
Critical_region_info::critical_path_length(Op* exitop)
{
    assert(exit_ops_map.is_bound(exitop));
    int idx = exit_ops_map.value(exitop);
    DepLen_OpInfo* dp = dep_info.value(exitop);
    return dp->m_Lstart[idx];
}

double
Critical_region_info::max_critical_path_length(void)
{
    double max =.0, cpl = .0;
    for(int i=0; i<num_exit_ops; i++){
	Op* exitop = rev_exit_ops[i];
	cpl = critical_path_length(exitop);
	max = (max < cpl) ? cpl : max;
    }
    return max;
}


// ------------------------------------------------------------------;
int  
Critical_region_info::compute_critical_region_info(Compound_region* rgn)
{

    //initialize the critical path edges and the exit_ops hashmaps;
    num_exit_ops = 0;
    for(Region_exit_ops eops(rgn); eops != 0; eops++){
	Op* op = *eops;
	critical_edges.bind(op, new List<Edge*>); 
	critical_ops.bind(op, new List<Op*>); 
	exit_ops_map.bind(op, num_exit_ops);
	num_exit_ops++;
    }

    //intialize the exitops Vector;
    rev_exit_ops.resize(num_exit_ops); 
    num_exit_ops = 0;
    for(Region_exit_ops eops1(rgn); eops1 != 0; eops1++){
	Op* op = *eops1;
	rev_exit_ops[num_exit_ops] = op;
	num_exit_ops++;
    }
    
    //get_estart is a static member of class DepLen;
    DepLen::get_estart(rgn, dep_info);

    //lstart is stored as a Vector of latest start times -one value w.r.t. ;
    //each exit op of the region;
    DepLen::get_lstart(rgn, dep_info);

    //now compute slack - another Vector of values for each op - one slack;
    //value for each lstart value (w.r.t. each exit op);
    DepLen::get_slack(rgn, dep_info);

    //determine the critical path edges and nodes (w.r.t. each exit op);
    for(int j=0; j<num_exit_ops; j++){
	Op* exitop = rev_exit_ops[j];
	
	//check in edges of critical exit node if any are critical;
	for(Op_inedges_rel ie(exitop, NON_CONTROL0_INEDGES); ie != 0; ie++){
	    Edge* e = *ie;
	    if (dbg(dcpr,1)){
		cdbg << "Op_" << e->src()->id() << " -> " ;
		cdbg << "Op_" << e->dest()->id() ;
	    }
	    if (is_edge_critical(e, exitop)){
		if (dbg(dcpr,1)){
		    cdbg << " is critical\n";
		}
		critical_edges.value(exitop)->add_tail(e);
	    } else if (dbg(dcpr,1)){
		cdbg << endl;
	    }
	}

	//add the list of edges on some critical path to exitop ;
	//to critical_edges(exitop);
	for(Op_ancestors op_ansc(exitop, rgn, NON_CONTROL0_INEDGES); 
	//for(Region_ops_reverse_C0_order op_ansc(rgn); 
	    op_ansc != 0; op_ansc++){
	    Op *op = *op_ansc;
	    
	    if (dbg(dcpr, 1))
		cdbg << "op <" << op->id() << "> ";
	    if (!is_node_critical(op, exitop)){
		if (dbg(dcpr, 1))
		    cdbg << endl;
		continue;
	    }
	    if (dbg(dcpr, 1))
		cdbg << " is critical\n";

	    critical_ops.value(exitop)->add_tail(op);

	    //check out edges of critical node if any are critical;
	    for(Op_outedges_rel oe(op, NON_CONTROL0_OUTEDGES); oe != 0; oe++){
		Edge* e = *oe;
		if (dbg(dcpr,1)){
		    cdbg << "Op_" << e->src()->id() << " -> " ;
		    cdbg << "Op_" << e->dest()->id() ;
		}
		if (is_edge_critical(e, exitop)){
		    if (dbg(dcpr,1)){
			cdbg << " is critical\n";
		    }
		    critical_edges.value(exitop)->add_tail(e);
		} else if (dbg(dcpr,1)){
		    cdbg << endl;
		}
	    }
	}
    }


    if (dbg(dcpr, 4)){
	for(Region_ops_C0_order ops(rgn); ops != 0; ops++){
	    Op *op = *ops;
	    if (dep_info.is_bound(op)){
		DepLen_OpInfo* cinfo = dep_info.value(op);
		cdbg << "op = " << op->id() << endl;
		cdbg << "deplen info = \n" << (*cinfo) << endl;
	    }
	}  
    }

    return num_exit_ops;

}

//---------------------------------------------------------------------;
void
Critical_region_info::delete_critical_region_info(Compound_region* rgn)
{ 
    //clear the critical path edges hash map;
    for(Region_exit_ops eops(rgn); eops != 0; eops++){
	Op* op = *eops;
	if (critical_edges.is_bound(op)){
	    List<Edge*>* l = critical_edges.value(op);
	    l->clear();
	    delete l;
	}
	if (critical_ops.is_bound(op)){
	    List<Op*>* cops = critical_ops.value(op);
	    cops->clear();
	    delete cops;
	}
    }
    //clear the dep_len map;
    DepLen::clear_opinfo_map(rgn, dep_info);
    dep_info.clear();
    
    critical_edges.clear();
    critical_ops.clear();
    exit_ops_map.clear();

    return;
}


//---------------------------------------------------------------------;
int
Critical_region_info::is_edge_critical(Edge* e, Op* exit_op)
{
    Op* sop = real_op(e->src());
    Op* dop = real_op(e->dest());

    if (is_node_critical(sop, exit_op) && is_node_critical(dop, exit_op))
	return true;

    assert(dep_info.is_bound(dop));
    assert(dep_info.is_bound(sop));

    DepLen_OpInfo* sdep = dep_info.value(sop);
    DepLen_OpInfo* ddep = dep_info.value(dop);

    if (dbg(dcpr,4)){
	cdbg << "is_edge_cr <" << sop->id() << "," << dop->id() << ">\n";
    }

    int i;
    if (exit_op!=(Op*)NULL){
	if (exit_ops_map.is_bound(exit_op)){
	    i = exit_ops_map.value(exit_op);
	} else {
	    if (dbg(dcpr,4)) 
		cdbg << "un-bound exitop : " << exit_op->id() << endl;
	    return false;
	}

	if (dbg(dcpr,4)) {
	    cdbg << "d.S, d.L, s.S, s.L, e.L = " 
		 << ddep->m_Slack[i] << " " << ddep->m_Lstart[i]
		 << " " <<  sdep->m_Slack[i] << " " << sdep->m_Lstart[i]
		 << " " << e->latency() << endl;
	}

	if (ddep->m_Slack[i]==0 && sdep->m_Slack[i]==0 && 
	    ((sdep->m_Lstart[i] + e->latency())==ddep->m_Lstart[i])){
	    return true;
	} 
    } else {
	for(i=0; i<num_exit_ops; i++){
	    if (ddep->m_Slack[i]==0 && sdep->m_Slack[i]==0 && 
		((sdep->m_Lstart[i] + e->latency())==ddep->m_Lstart[i])){
		return true;
	    } 
	}
    }

    return false;
}


//---------------------------------------------------------------------;
int
Critical_region_info::is_node_critical(Op* op, Op* exit_op)
{
    assert(dep_info.is_bound(op));
    DepLen_OpInfo* dep = dep_info.value(op);

    int i=0;
    if (exit_op!=(Op*)NULL){
	if (exit_ops_map.is_bound(exit_op))
	    i = exit_ops_map.value(exit_op);
	else 
	    return false;
	
	if (dep->m_Slack[i]==0)
	    return true;
    } else {
	for(i=0; i<num_exit_ops; i++){
	    if (dep->m_Slack[i]==0)
		return true;
	}
    }
    return false;

}

//---------------------------------------------------------------------;
Op* 
Critical_region_info::most_freq_exit_op(Region* r)
{
    Op* exit_op = (Op*)NULL;
    double max = .0;

    for(Region_exit_edges exit_edges(r); exit_edges != 0; exit_edges++){
	Edge* e = *exit_edges;
	Op* op = e->src();
	Control_flow_freq *cfreq = get_control_flow_freq(e);
	if (max<cfreq->freq){
	    max = cfreq->freq;
	    exit_op = op;
	}
    }
    return exit_op;
}

//---------------------------------------------------------------------;
Op* 
Critical_region_info::exitop_with_longest_cp(Region* r)
{
    Op* exit_op = (Op*)NULL;
    double max = .0;
    for(int k=0; k<num_exit_ops; k++){
	Op* op = rev_exit_ops[k];
	assert(exit_ops_map.is_bound(op));
	int idx = exit_ops_map.value(op);
	assert(dep_info.is_bound(op));
	DepLen_OpInfo* dp = dep_info.value(op);
	if (max<dp->m_Lstart[idx]){
	    max = dp->m_Lstart[idx];
	    exit_op = op;
	}
    }
    return exit_op;
}

/*--------------------------------------------------------------------------;
// this function cannibalized from Saduns version in el_dot_tools.cpp;
//
// cregion_type can be either of 
//    DISPLAY_ALL_CR_EDGES = display all edges on some critical path to
//                           atleast one exit node;
//    DISPLAY_MAX_FREQ_CR_EDGES = display all edges on some critical 
//                          path to the exit most freq. taken;
//    DISPLAY_MAX_CR_EDGES = display all edges on the longest critical  path;
// 
//    DISPLAY_ALL_EDGES = need to explain this?
//
// bg = true => spawn ghostview as background process so elcor doesnt block
//      
//---------------------------------------------------------------------------*/
void 
Critical_region_info::display_critical_region(Region* r, 
					      int cregion_type, 
					      char* proc_name,
					      int bg,
					      int file_index)
{
    if (!El_print_dot_graph) return ;

    char* orientation ;
    char name[256] ;
    Region_all_ops op_iter ;
    Op* exit_op;
    int k, exit_idx = 0;

    cdbg << "Critical_region_info::display_critical_region " << proc_name << endl << flush;

    assert(cregion_type==DISPLAY_MAX_FREQ_CR_EDGES 
	   || cregion_type==DISPLAY_ALL_CR_EDGES
	   || cregion_type==DISPLAY_MAX_CR_EDGES
	   || cregion_type==DISPLAY_ALL_EDGES);

    if (cregion_type==DISPLAY_MAX_FREQ_CR_EDGES){
	exit_op = most_freq_exit_op(r);
    } else if (cregion_type==DISPLAY_MAX_CR_EDGES) {
	exit_op = exitop_with_longest_cp(r);
    } else  if (cregion_type==DISPLAY_ALL_EDGES) {
	exit_op = exitop_with_longest_cp(r);
    } else {
	exit_op = (Op*)NULL;
	exit_idx = -1;
    }
    
    if (exit_idx>-1 && exit_ops_map.is_bound(exit_op)){
	exit_idx = exit_ops_map.value(exit_op);
    }

    char dot_file[256], underscore_file[256];
    for(k=0; k<256; k++) { 
	dot_file[k] = 0; 
	underscore_file[k] = 0;
    }

    if (proc_name!=0){
	sprintf(dot_file, "G.%s.%d.%d", proc_name, r->id(), file_index);
	sprintf(underscore_file, "G%s_%d_%d", proc_name, r->id(), file_index);
	if (cregion_type==DISPLAY_ALL_EDGES)
	    sprintf(underscore_file, "A%s_%d_%d", proc_name, r->id(), file_index);
	else
	    sprintf(underscore_file, "G%s_%d_%d", proc_name, r->id(), file_index);
    } else {
	sprintf(dot_file, "G.%d.%d", r->id(), file_index);
	sprintf(underscore_file, "G_%d_%d", r->id(), file_index);
    }
    ofstream os(dot_file);
    
    if (!strcmp(El_dot_graph_orientation, "portrait")) {
	orientation = (char*)"portrait" ;
    } else if (!strcmp(El_dot_graph_orientation, "landscape")) {
	orientation = (char*)"landscape" ;
    } else {  
	orientation = (char*)"landscape" ;
    }
    
    sprintf(name, "Region_%d",r->id()) ;
    el_dot_print_file_header(os,orientation,underscore_file);
    
    
    os << "node [shape = ellipse, fontname = \"Helvetica\" , fontsize = 24]\n";
    os << "edge [fontname = \"Helvetica\" , fontsize = 24]\n";
    
    for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
	Op* op = *op_iter ;
	
	//if (El_dot_schedop_labels && is_pseudo(op)) continue;
	//if ((cregion_type!=DISPLAY_ALL_EDGES) && (!is_node_critical(op, exit_op))) continue;
	
	os << "\"Op_" << op->id() << "\" [" ;
	if (is_switch(op) || is_merge(op)) {
	    if (is_switch(op)) {
		os << " label = \"";
		if (El_dot_verbose) {
		    Operand d1 = op->dest(DEST1);
		    Operand u1 = op->src(SRC1);
		    if (d1.is_reg() || d1.is_macro_reg())  os << d1;
		    os << "=" ;
		    if (u1.is_reg() || u1.is_macro_reg())  os << u1;
		} else {
		    os << "s" << op->id();
		}
		os << "\"" ;
	    } else {
		os << " label = \"";
		if (El_dot_verbose) {
		    if (is_remap(op)) {
		        cdbg << "is_remap :: 572\n" << flush;
			Operand& tmp_oprnd = op->src(SRC1) ;
			os << "REMAP(" ;
			os << vregfile_to_mdesname(data_type_to_file(tmp_oprnd.dtype())) ;
			os << " " << tmp_oprnd.vr_num() ;
			os << ")" ;
		    } else {
			Operand d1 = op->dest(DEST1);
			Operand u1 = op->src(SRC1);
			if (d1.is_reg() || d1.is_macro_reg())  os << d1;
			os << "=" ;
			if (u1.is_reg() || u1.is_macro_reg())  os << u1;
		    }
		} else {
		    os << "m" << op->id();
		}
		os << "\"" ;
	    }
	    os << ", width = 0.5" ;
	} else {
	    os << " label = \"";
	    if (El_dot_verbose) {
		const int buflen = 2048;
		char ss[buflen];
		char st[buflen];
		for(int i=0; i<buflen; i++){
		    ss[i] = st[i] = 0;
		}
		
		ostringstream oss;
		oss << *op;
		strcpy(ss, oss.str().c_str());

		// replace first newline with end-of-string, then print string;
		char* p = strchr(ss, '\n');
		if (p) *p = '\0';
		
		int sti=0, ssi=0;
		while(ss[ssi]!='\0'){
		    if (sti>=125) break;
		    if (ss[ssi]=='\"'){
			st[sti] = '\\';
			sti++;
		    }
		    st[sti] = ss[ssi];
		    sti++;
		    ssi++;
		}
		st[127] = '\0';
		os << st;
	    } else {
		os << op->id() << " ";
		if (El_dot_schedop_labels && op->flag(EL_REGION_SCHEDULED)) 
		    os << (char *) op->sched_opcode();
		else 
		  //os << (char *) el_opcode_to_string_map.value(op->opcode());
		  //tangw (06-03-02)
		  os << get_mdes_opcode_string(op);



		char times[128];
		for(int i=0; i<128; i++) times[i] = 0;

		if (dep_info.is_bound(op)){
		    int es = dep_info.value(op)->m_Estart;
		    int ls;
		    if (exit_idx>=0 || (cregion_type==DISPLAY_ALL_EDGES)){
			ls = dep_info.value(op)->m_Lstart[exit_idx];
			sprintf(times, "<%2d,%2d>", es, ls);
		    } else {
			sprintf(times, "<%2d>", es);
		    }
		    os << times;
		}
		
	    }
	    os << "\"" ;
	}
	os << "] ; \n" ;
    }
    
    //print edges;
    List<Edge*> cedges;
    List_iterator<Edge*> eiter;
    Region_all_internal_edges riedges;

    if (cregion_type==DISPLAY_ALL_CR_EDGES){
	get_all_critical_edges(cedges);
	eiter(cedges); 
    } else if (cregion_type==DISPLAY_ALL_EDGES){
	riedges(r); //, NON_CONTROL0_OUTEDGES);
    } else {
	assert((exit_op!=(Op*)NULL) && critical_edges.is_bound(exit_op));
        get_critical_edges(exit_op, cedges);
	eiter(cedges);
    }


//for(List_iterator<Edge*> eiter(cedges); eiter!=0; eiter++){   
    
								    
    while(1){
	
	Edge* e ;
	if (cregion_type==DISPLAY_ALL_EDGES){
	    if (riedges==0) break;
	    e = *riedges;
	    riedges++;
	} else {
	    if (eiter==0) break;
	    e = *eiter;
	    eiter++;
	}


	if (El_dot_c0 == 0 && e->is_control()) continue ;
	if (El_dot_c0 == 0 && e->is_seq()) continue ;
	if (El_dot_mem == 0 && e->is_mem()) continue ;
	if (El_dot_flow == 0 && e->is_reg_flow()) continue ;
	if (El_dot_anti == 0 && e->is_reg_anti()) continue ;
	if (El_dot_out == 0 && e->is_reg_out()) continue ;
	//if (is_pseudo(e->src()) || is_pseudo(e->dest())) continue;
	
	char r[20];
	for(int k=0; k<20; k++) r[k] = 0;
	
	Operand o1, o2;
	char *style_bold = (char*)"bold";
	char *style;
	
	style = style_bold;
	
	/*
	if (is_switch(e->dest())){
	    //output an edge between the e->src() to the corresp exitop ;
	    Op* eop = real_op(e->dest());
	    os << "\"Op_" << e->src()->id() << "\"" ;
	    os << " -> " ;
	    os << "\"Op_" << eop->id() << "\" " ;
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = " << style << "];\n";
	}
	*/

	os << "\"Op_" << e->src()->id() << "\"" ;
	os << " -> " ;
	os << "\"Op_" << e->dest()->id() << "\" " ;
	
	
	if (e->is_seq()) {
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = dotted]";
	} else if (e->is_control()) {
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = dotted, color = violet]";
	} else if (e->is_mem()) {
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = " << style << ", color = yellow]" ;
	} else if (e->is_reg_flow()) {
	    //cdbg << "is_reg_flow :: 710\n" << flush;
	    o1 = e->src()->dest(e->src_port());
	    //cdbg << o1 << endl << flush;
	    if (o1.is_reg()) sprintf(r, ", r%d" , o1.vr_num());
	    os << "[label = \"" << e->latency()  << r << "\", ";
	    os << "style = " << style << ", color = red]" ;
	} else if (e->is_reg_anti()) {
	    cdbg << "is_reg_anti :: 713\n" << flush;
	    //check if the anti dependence is between explicit reg operands;
	    o1 = e->src()->src(e->src_port());
	    o2 = e->dest()->dest(e->dest_port());
	    //cdbg << o1 << endl << flush;
	    //cdbg << o2 << endl << flush;
	    if (o1==o2 && o1.is_reg() && o2.is_reg()){
		sprintf(r, ", r%d" , o1.vr_num());
	    }
	    os << "[label = \"" << e->latency() << r << "\", ";
	    os << "style = " << style << ", color = blue]" ;
	} else if (e->is_reg_out()) {
	    cdbg << "is_reg_out :: 724\n" << flush;
	    //check if the output dependence is between explicit reg operands;
	    o1 = e->src()->dest(e->src_port());
	    o2 = e->dest()->dest(e->dest_port());
	    //cdbg << o1 << endl << flush;
	    //cdbg << o2 << endl << flush;
	    if (o1==o2 && o1.is_reg() && o2.is_reg()){
		sprintf(r, ", r%d" , o1.vr_num());
	    }
	    os << "[label = \"" << e->latency() << r << "\", ";
	    os << "style = " << style << ", color = green]" ;
	} else {
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = " << style << ", color = purple]" ;
	}
	os << " ;\n" ;
    }

    el_dot_print_file_tail(os) ;
    os.close();
    el_dot_process_dot_file(dot_file,orientation, bg) ;
    
}

//#####################################################################
//#####################################################################
//#####################################################################
/*
void junk_display_critical_region(Region* r, 
				  int cregion_type, 
				  char* proc_name,
				  int bg,
				  int file_index) 
{
    if (!El_print_dot_graph) return ;

    char* orientation ;
    char name[256] ;
    Region_all_ops op_iter ;
    Op* exit_op;
    double max = -1;
    int k, exit_idx = 0;

    assert(cregion_type==DISPLAY_MAX_FREQ_CR_EDGES 
	   || cregion_type==DISPLAY_ALL_CR_EDGES
	   || cregion_type==DISPLAY_MAX_CR_EDGES);

    if (cregion_type==DISPLAY_MAX_FREQ_CR_EDGES){
	for(Region_exit_edges exit_edges(r); exit_edges != 0; exit_edges++){
	    Edge* e = *exit_edges;
	    Op* op = e->src();
	    Control_flow_freq *cfreq = get_control_flow_freq(e);
	    if (max<cfreq->freq){
		max = cfreq->freq;
		exit_op = op;
	    }
	}
	if (exit_ops_map.is_bound(exit_op))
	    exit_idx = exit_ops_map.value(exit_op);
    } else if (cregion_type==DISPLAY_MAX_CR_EDGES) {
	for(k=0; k<num_exit_ops; k++){
	    Op* op = rev_exit_ops[k];
	    assert(exit_ops_map.is_bound(op));
	    int idx = exit_ops_map.value(op);
	    assert(dep_info.is_bound(op));
	    DepLen_OpInfo* dp = dep_info.value(op);
	    if (max<dp->m_Lstart[idx]){
		max = dp->m_Lstart[idx];
		exit_op = op;
		exit_idx = idx;
	    }
	}
    } else {
	exit_op = (Op*)NULL;
	exit_idx = -1;
    }

    //ofstream os(El_dot_graph_filename) ;

    char dot_file[256];
    for(k=0; k<256; k++) dot_file[k] = 0;
    char underscore_file[256];
    for(k=0; k<256; k++) underscore_file[k] = 0;
    if (proc_name!=0){
	sprintf(dot_file, "%s.%s.%d.%d", El_dot_graph_filename, 
		proc_name, r->id(), file_index);
	sprintf(underscore_file, "%s_%s_%d_%d", El_dot_graph_filename, 
		proc_name, r->id(), file_index);
    } else {
	sprintf(dot_file, "%s.%d.%d", El_dot_graph_filename, r->id(), file_index);
	sprintf(underscore_file, "%s_%d_%d", El_dot_graph_filename, r->id(), file_index);
    }
    ofstream os(dot_file);

    if (!strcmp(El_dot_graph_orientation, "portrait")) {
	orientation = "portrait" ;
    } else if (!strcmp(El_dot_graph_orientation, "landscape")) {
	orientation = "landscape" ;
    } else {  
	// Default for DFGs;
	orientation = "landscape" ;
    }

    sprintf(name,"Region_%d",r->id()) ;
    el_dot_print_file_header(os,orientation,underscore_file) ;


    os << "node [shape = ellipse, fontname = \"Helvetica\" , fontsize = 24]\n";
    os << "edge [fontname = \"Helvetica\" , fontsize = 24]\n";

    // If the op has related ops, put them at the same rank     ;
    // If El_dot_schedop_labels is set then a schedule time line is generated ;

    int maxtime = minus_infinity;
    int mintime = infinity;
    if (El_dot_schedop_labels) {
	// initialize max/min sched times;
	for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
	    Op* op = *op_iter;
	    maxtime = ELCOR_MAX(maxtime, op->sched_time());
	    mintime = ELCOR_MIN(mintime, op->sched_time());
	}
	// output time line;
	os << "node [ shape=plaintext, fontsize=24 ];\n";
	for(int i=mintime; i<maxtime; i++) os << i << " -> ";
	os << maxtime << " [ style = invis ];\n";
    }

    for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
	Op* op = *op_iter ;

	if (El_dot_schedop_labels && is_pseudo(op)) continue;
	if (!is_node_critical(op, exit_op)) continue;

	os << "\"Op_" << op->id() << "\" " ;
	os << "[" ; // fontname = \"Helvetica\" shape = box" ;

	if (is_switch(op) || is_merge(op)) {
	    if (is_switch(op)) {
		os << " label = \"";
		if (El_dot_verbose) {
		    Operand d1 = op->dest(DEST1);
		    Operand u1 = op->src(SRC1);
		    if (d1.is_reg() || d1.is_macro_reg())  os << d1;
		    os << "=" ;
		    if (u1.is_reg() || u1.is_macro_reg())  os << u1;
		} else {
		    os << "s" << op->id();
		}
		os << "\"" ;
	    } else {
		os << " label = \"";
		if (El_dot_verbose) {
		    if (is_remap(op)) {
			Operand& tmp_oprnd = op->src(SRC1) ;
			os << "REMAP(" ;
			os << vregfile_to_mdesname(data_type_to_file
						 (tmp_oprnd.data_type())) ;
			os << " " << tmp_oprnd.vr_num() ;
			os << ")" ;
		    } else {
			Operand d1 = op->dest(DEST1);
			Operand u1 = op->src(SRC1);
			if (d1.is_reg() || d1.is_macro_reg())  os << d1;
			os << "=" ;
			if (u1.is_reg() || u1.is_macro_reg())  os << u1;
		    }
		} else {
		    os << "m" << op->id();
		}
		os << "\"" ;
	    }
	    os << ", width = 0.5" ;
	} else {
	    os << " label = \"";
	    if (El_dot_verbose) {
		const buflen = 2048;
		char ss[buflen];
		char st[buflen];
		for(int i=0; i<buflen; i++){
		    ss[i] = st[i] = 0;
		}

		ostringstream oss;
		oss << *op;
                strcpy(ss, oss.str().c_str());
		
		// replace first newline with end-of-string, then print string;
		char* p = strchr(ss, '\n');
		if (p) *p = '\0';

		int sti=0, ssi=0;
		while(ss[ssi]!='\0'){
		    if (sti>=125) break;
		    if (ss[ssi]=='\"'){
			st[sti] = '\\';
			sti++;
		    }
		    st[sti] = ss[ssi];
		    sti++;
		    ssi++;
		}
		st[127] = '\0';
		os << st;
	    } else {
		os << op->id() << " ";
		if (El_dot_schedop_labels && op->flag(EL_REGION_SCHEDULED)) 
		    os << (char *) op->sched_opcode();
		else 
		//os << (char *) el_opcode_to_string_map.value(op->opcode());
		//tangw
		os << get_mdes_opcode_string(op);

		char times[128];
		for(int i=0; i<128; i++) times[i] = 0;

		if (dep_info.is_bound(op)){
		    int es = dep_info.value(op)->m_Estart;
		    int ls;
		    if (exit_idx>=0){
			ls = dep_info.value(op)->m_Lstart[exit_idx];
			sprintf(times, "<%2d,%2d>", es, ls);
		    } else {
			sprintf(times, "<%2d>", es);
		    }
		    os << times;
		}

	    }
	    os << "\"" ;
	}
	os << "] ; \n" ;
    }

    //
    if (El_dot_schedop_labels) {
	// rank all ops according to schedule;
	Vector<List<Op*> > ops(maxtime-mintime+1);
	for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
	    Op* op = *op_iter;
	    if (is_pseudo(op)) continue;
	    if (!is_node_critical(op, exit_op)) continue;
	    ops[op->sched_time()-mintime].add_tail(op);
	}
	for(int i=mintime; i<=maxtime; i++) {
	    os << "{ rank = same ; " << i << "; ";
	    for(List_iterator<Op*> iter(ops[i-mintime]); iter!=0; iter++){
		Op* op = *iter;
		if (!is_node_critical(op, exit_op)) continue;
		os << "\"Op_" << op->id() << "\"; "; 
	    }
	    os << "} \n";
	}
    } else {
	// rank related ops together;
	for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
	    Op* op = *op_iter ;
	    if (is_switch(op) || is_merge(op)) continue ;
	    if (!is_node_critical(op, exit_op)) continue;
	    if (!op->get_relops().is_empty()) {
		// os << "subgraph \"cluster_" << op->id() <<"\"\n" ;
		os << "{ rank = same ; \n" ;
		// os << "set = cluster ;\n" ;
		os << "\"Op_" << op->id() <<"\"; " ;
		for (List_iterator<Op*> relopi(op->get_relops()) ;
		     relopi != 0 ; relopi++) {
		    os << "\"Op_" << (*relopi)->id() << "\"; \n" ;
		}
		os << "} \n" ;
	    }
	}
    }
    
    for (Region_all_internal_edges edgei(r) ; edgei != 0 ; edgei++) {
	Edge* e = * edgei ;

	if (El_dot_c0 == 0 && e->is_control()) continue ;
	if (El_dot_mem == 0 && e->is_mem()) continue ;
	if (El_dot_flow == 0 && e->is_reg_flow()) continue ;
	if (El_dot_anti == 0 && e->is_reg_anti()) continue ;
	if (El_dot_out == 0 && e->is_reg_out()) continue ;
	
	if (El_dot_schedop_labels && 
	    (is_pseudo(e->src()) || is_pseudo(e->dest()))) continue;

	if (!is_edge_critical(e, exit_op)) continue;
    
    //
    
    if ((exit_op!=(Op*)NULL) && critical_edges.is_bound(exit_op)){

        List<Edge*>* cedges = critical_edges.value(exit_op);

	Edge* e = * edgei ;

	if (El_dot_c0 == 0 && e->is_control()) continue ;
	if (El_dot_mem == 0 && e->is_mem()) continue ;
	if (El_dot_flow == 0 && e->is_reg_flow()) continue ;
	if (El_dot_anti == 0 && e->is_reg_anti()) continue ;
	if (El_dot_out == 0 && e->is_reg_out()) continue ;
	
	if (El_dot_schedop_labels && 
	    (is_pseudo(e->src()) || is_pseudo(e->dest()))) continue;

	if (!is_edge_critical(e, exit_op)) continue;


	os << "\"Op_" << e->src()->id() << "\"" ;
	os << " -> " ;
	os << "\"Op_" << e->dest()->id() << "\" " ;
	

	char r[20];
	for(int k=0; k<20; k++) r[k] = 0;

	Operand o1, o2;

	char *style_bold = "bold";
	char *style_dotted = "dotted";
	char *style;
	
        style = style_bold;

	if (e->is_control()) {
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = " << style << "]";
	} else if (e->is_mem()) {
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = " << style << ", color = yellow]" ;
	} else if (e->is_reg_flow()) {
	    os << "[label = \"" << e->latency() << "\", ";
	    os << "style = " << style << ", color = red]" ;
	} else if (e->is_reg_anti()) {
	    //check if the anti dependence is between explicit reg operands;
	    o1 = e->src()->src(e->src_port());
	    o2 = e->dest()->dest(e->dest_port());
	    if (o1==o2){
		sprintf(r, ", r%d" , o1.vr_num());
	    }
	    os << "[label = \"" << e->latency() << r << "\", ";
	    os << "style = " << style << ", color = blue]" ;
	} else if (e->is_reg_out()) {
	    //check if the output dependence is between explicit reg operands;
	    o1 = e->src()->dest(e->src_port());
	    o2 = e->dest()->dest(e->dest_port());
	    if (o1==o2){
		sprintf(r, ", r%d" , o1.vr_num());
	    }
	    os << "[label = \"" << e->latency() << r << "\", ";
	    os << "style = " << style << ", color = green]" ;
	} else {
            cdbg << "<" << e->src()->id() << "," 
                 << e->dest()->id() << "> missed and edge!!" << endl;
        }
	os << " ;\n" ;
    }
    el_dot_print_file_tail(os) ;
    os.close();
    //el_dot_process_dot_file(El_dot_graph_filename,orientation, bg) ;
    el_dot_process_dot_file(dot_file,orientation, bg) ;

}
*/



//##############################################################;
//  critical edges filterator classes definitions;
//##############################################################;
/*
Region_critical_edges::Region_critical_edges()
 : all_edges(hash_edge_ptr, 1028)
{
}

Region_critical_edges::Region_critical_edges(Region* region, 
					     const Edge_filter* efilter)
 : all_edges(hash_edge_ptr, 1028)
{ 
  init(region, efilter);
}


void Region_critical_edges::operator()(Region* region, 
				       const Edge_filter* efilter)
{
  init(region, efilter);
}


Region_critical_edges::~Region_critical_edges() 
{
}


void 
Region_critical_edges::init(Region* region, const Edge_filter* efilter)
{
  Hash_set_filterator<Edge*>::operator()(all_edges, efilter);
}

*/

//#######################################################################
ostream& operator<<(ostream& os, Critical_region_info& cinfo)
{
    Op  *exitop, *op;
    List<Op*> cops;
    
    os << "all critical nodes :: " << endl;
    cinfo.get_all_critical_ops(cops);
    for(List_iterator<Op*> liop(*(cinfo.critical_ops.value(exitop)));
	liop!=0; liop++){
	op = *liop;
	os << op->id() << endl;
    }
    cops.clear();
    os << endl;

    List<Edge*> cedges;
    List_iterator<Edge*> liedges;
    Edge* e;

    os << "all critical edges :: " << endl;
    cinfo.get_all_critical_edges(cedges);
    for(liedges(cedges); liedges!=0; liedges++){
	e = *liedges;
	os << "<" << e->src()->id() << ", " 
	   << e->dest()->id() << ">\n";
    }
    cedges.clear();
    os << endl;

    os << "all anti critical edges = " << endl;
    cinfo.get_all_anti_critical_edges(cedges);
    for(liedges(cedges); liedges!=0; liedges++){
	e = *liedges;
	os << "<" << e->src()->id() << ", " 
	   << e->dest()->id() << ">\n";
    }
    cedges.clear();
    os << endl;

    os << "all output critical edges = " << endl;
    cinfo.get_all_output_critical_edges(cedges);
    for(liedges(cedges); liedges!=0; liedges++){
	e = *liedges;
	os << "<" << e->src()->id() << ", " 
	   << e->dest()->id() << ">\n";
    }
    cedges.clear();
    os << endl;

    os << "all flow critical edges = " << endl;
    cinfo.get_all_flow_critical_edges(cedges);
    for(liedges(cedges); liedges!=0; liedges++){
	e = *liedges;
	os << "<" << e->src()->id() << ", " 
	   << e->dest()->id() << ">\n";
    }
    cedges.clear();
    os << endl;

    return os;
}
