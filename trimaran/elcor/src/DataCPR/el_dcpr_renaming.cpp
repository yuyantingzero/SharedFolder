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
//      File:           el_dcpr_renaming.cpp
//      Authors:        Suren Talla
//                      (parts of it from earlier version by Sumedh Sathaye)
//      Created:        July 1997
//      Description:    Renaming interface code
//
//      Copyright (C) 1996 Hewlett-Packard Company 
//
///////////////////////////////////////////////////////////////////////////



#include <iostream>
#include <fstream>
#include "el_main.h"
#include "pred_analysis.h"
#include "iterators.h"
#include "reaching_defs_solver.h"
#include "el_dcpr_renaming.h"
#include "dbg.h"
#include "el_control.h"
#include "el_dcpr_driver.h"


extern int is_eligible_for_renaming(Operand &opr);


Renaming_ops_net::Renaming_ops_net (void)
{
    reflist = new List<Pair<int, int>*>; 
    net_type = INTERNAL_NET;
    num_internal_defs = 0;
    num_internal_uses = 0;
    num_live_outs = 0;
    num_live_ins = 0;
}


Renaming_ops_net::~Renaming_ops_net (void)
{
    if (reflist->size() > 0){
	for(List_iterator<Pair<int,int>*> ttt; ttt!=0; ttt++){
	    Pair<int,int>* p = *ttt;
	    delete p;
	}
    }
    delete reflist;
    reflist = NULL;
    net_type = INTERNAL_NET;
    num_internal_defs = 0;
    num_internal_uses = 0;
    num_live_outs = 0;
    num_live_ins = 0;
}

//----------------------------------------------------------------------;
//given a net of def-uses which all have to be renamed atomically, this ;
//procedure does the actual renaming;
//----------------------------------------------------------------------;
int
Renaming_data::rename_net(Compound_region* rgn, 
			  Renaming_ops_net* net, 
			  int is_proc, 
			  int to_rename_with_copy)
{

    // if there is just one ref in the NET; do not rename:
    // should later be cleaned by the dead-code elim;
    if (net->reflist->size() <= 1){
	return 0;
    }

    int f, s, ref_type, old_nvn=0, nvn = 0;

    //if the NET crosses the HB and the rename with copy bit is off,
    f = net->net_type & CROSSING_NET;
    if (is_proc!=true && (f==CROSSING_NET) && (!to_rename_with_copy)){
	return 0;
    }

    f = net->net_type & LIVE_IN_NET;
    if (f==LIVE_IN_NET){
	return 0;
    }

    List_iterator<Pair<int,int>*> ttt (*(net->reflist));
    
    // go over first to find the new vr number;
    while(1){
	Pair<int, int>  *p = *ttt;
	f = p->first;
	s = p->second;
	if (f==DEF_REF){
	    ref_type = rev_def_map[s].get_ref_type();
	    if (ref_type!=EXP_DEST)
		continue;
	    if (dbg (dcpr, 4))
		cdbg << "*\trenaming(d)-" << rev_def_map[s] << endl;
	    old_nvn = rev_def_map[s].get_operand().vr_num();
	    rev_def_map[s].get_operand().rename();
	    nvn = rev_def_map[s].get_operand().vr_num();
	    break;
	} else {
	    ref_type = rev_use_map[s].get_ref_type();
	    if (ref_type!=EXP_SRC && ref_type!=PRED_SRC)
		continue;
	    if (dbg (dcpr, 4))
		cdbg << "*\trenaming(u)-" << rev_use_map[s] << endl;
	    old_nvn = rev_use_map[s].get_operand().vr_num();
	    rev_use_map[s].get_operand().rename();
	    nvn = rev_use_map[s].get_operand().vr_num();
	    break;
	}
	ttt++;
	if (ttt==0){
	    break;
	}
    }

    //if (dbg (dcpr, 2)) {
	cdbg << "new/old vrnum = [" << nvn 
	     << ", " << old_nvn << "]" << endl;
    //}

    // now actually rename - only those uses and defs which are:
    // internal to the region;
    List_iterator<Pair<int,int>*> nt (*(net->reflist));
    for (; nt != 0; nt++) {
	f = (*nt)->first;
	s = (*nt)->second;
	if (f==DEF_REF){
	    ref_type = rev_def_map[s].get_ref_type();
	    if (ref_type!=EXP_DEST)
		continue;

	    if (net->net_type & CROSSING_NET){
		//when doing renaming with copy, need to insert a copy;
		//instr for the def;
		Op *new_op;
		Op *old_op = rev_def_map[s].get_op();
		Operand cur_dest = old_op->dest(DEST1);
		Operand new_reg = new Reg(old_op->dest(DEST1));

		new_reg.rename(nvn);
		cur_dest.rename(old_nvn);

		new_op = new Op(get_move_opcode_for_operand(cur_dest));
		new_op->set_dest(DEST1, cur_dest);
		new_op->set_src(SRC1, new_reg);
		new_op->set_src(PRED1, old_op->src(PRED1));
		
		El_insert_op_after(new_op, old_op);
		if (dbg (dcpr, 2)){
		    cdbg << "new_op = " << new_op->id()  << endl;
		}

	    }

	    if (dbg (dcpr, 4))
		cdbg << "*\t\trenaming(d)-"<< rev_def_map[s] << "  -->  ";
	    rev_def_map[s].get_operand().rename(nvn);
	    if (dbg (dcpr, 4))
		cdbg << rev_def_map[s] << endl;
	} else {
	    ref_type = rev_use_map[s].get_ref_type();
	    if (ref_type!=EXP_SRC && ref_type!=PRED_SRC)
		continue;
	    if (dbg (dcpr, 4))
		cdbg << "*\t\trenaming(u)-" << rev_use_map[s] << " -->  ";
	    rev_use_map[s].get_operand().rename(nvn);
	    if (dbg (dcpr, 4))
		cdbg << rev_use_map[s] << endl;
	}
    }
    return 1;

}

//------------------------------------------------------------------------;
Renaming_data::Renaming_data (Region* r) :
    total_renamed (0), 
    def_to_i_map (hash_El_ref, 1000), 
    use_to_i_map (hash_El_ref, 1000),
    m_net_num_groups(0)
{
}

Renaming_data::~Renaming_data (void)
{
    m_net_num_groups = 0;
}

//-------------------------------------------------------------------------;
//
//-------------------------------------------------------------------------;
void 
Renaming_data::get_net (int defnum, Reaching_defs_info* rdiin, 
			Renaming_ops_net* net)
{
    if (def_visited.bit (defnum))
	return;

    // visit;
    def_visited.set_bit (defnum);

    // add the def to current NET;
    net->reflist->add_tail (new Pair<int, int> (DEF_REF, defnum));

    if (dbg (dcpr, 4))
	cdbg << "*\t\t\tdef-" << defnum << " " << rev_def_map[defnum] << endl;

    El_ref defref = rev_def_map[defnum];
    int ref_type = defref.get_ref_type();

    //check if the def is internal to the hyperblock or external reaching def;
    if (ref_type!=EXP_DEST){
	if (ref_type==LIVEIN_DEST){
	    net->net_type = net->net_type | LIVE_IN_NET;
	    net->num_live_ins++;
	} else {
	    net->net_type = net->net_type | CROSSING_NET;
	}
    } else {
	net->num_internal_defs++;
    }

    List_iterator<El_ref> duli (*(rdiin->get_du_chain (defref)));
    for (; duli != 0; duli++) {
	El_ref use_ref = *duli;
	ref_type = use_ref.get_ref_type();

	int u_int = use_to_i_map.value(use_ref); 
	if (use_visited.bit (u_int))
	    continue;

	//check if the use is internal to the hyperblock or external live out;
	if (ref_type!=EXP_SRC && ref_type!=PRED_SRC){
	    if (ref_type==LIVEOUT_FALLTHROUGH_SRC || ref_type==LIVEOUT_TAKEN_SRC){
		net->net_type = net->net_type | LIVE_OUT_NET;
		net->num_live_outs++;
	    } else {
		net->net_type = net->net_type | CROSSING_NET ;
	    }
	} else {
	    net->num_internal_uses++;
	}
	   
	// visit the use;
	use_visited.set_bit (u_int);

	// add the use to the current NET;
	net->reflist->add_tail (new Pair<int, int> (USE_REF, u_int));

	if (dbg (dcpr, 4))
	    cdbg << "*\t\t\tuse-" << u_int << " " << rev_use_map[u_int] << endl;


	List_iterator<El_ref> udli (*(rdiin->get_ud_chain (use_ref)));
	for (; udli != 0; udli++) {
            El_ref def_ref = *udli;
	    int d_int = def_to_i_map.value (def_ref);
	    get_net (d_int, rdiin, net); 
	}
    }

}

//---------------------------------------------------------------------
int
Renaming_data::init_def_use_maps(Reaching_defs_info* rdi)
{
    int def_counter = 1;
    
    for (Hash_map_iterator<El_ref, List<El_ref>*> dui(rdi->def_use); 
	 dui != 0 ; dui ++) {
	El_ref& cur_def = (*dui).first ;
	//for now we are doing only vr renaming so restrict maps to those;
	if (cur_def.get_operand().is_reg()){
	    if (!def_to_i_map.is_bound(cur_def)){
		def_to_i_map.bind(cur_def, def_counter);
		def_counter++;
	    }
	}
    }

    // create the reverse definition map:
    rev_def_map.resize (def_counter+1);
    Hash_map_iterator<El_ref,int> edi;
    for (edi(def_to_i_map); edi != 0; edi++){
	if ((*edi).second > 0)
	    rev_def_map[(*edi).second] = (*edi).first;
    }

    def_visited.resize(def_counter+1);
    m_net_vrnum.resize(def_counter+1);
    m_net.resize(def_counter+1);
    for(int i=1; i<=def_counter; i++){
	m_net_vrnum[i] = -1;
    }

    int use_counter = 1;
    for (Hash_map_iterator<El_ref, List<El_ref>*> udi(rdi->use_def); udi != 0 ; udi ++) {
	El_ref& cur_def = (*udi).first ;
	//for now we are doing only vr renaming so restrict maps to those;
	if (cur_def.get_operand().is_reg()){
	    if (!use_to_i_map.is_bound(cur_def)){
		use_to_i_map.bind(cur_def, use_counter);
		use_counter++;
	    }
	}
    }

    // create the reverse use map:
    rev_use_map.resize (use_counter + 1);
    for (Hash_map_iterator<El_ref,int> eui(use_to_i_map); eui != 0; eui++){
	if ((*eui).second > 0)
	    rev_use_map[(*eui).second] = (*eui).first;
    }

    use_visited.resize(use_counter+1);

    // Print all if asked to:
    if (dbg (dcpr, 1)) {
	cdbg << "*\t\t\tPrinting the def map:" << endl;
	for (edi (def_to_i_map); edi != 0; edi++)
	    if ((*edi).second > 0)
		cdbg << "*\t\t\t{" << (*edi).second << ", "
		    << (*edi).first << "}" << endl;
	cdbg << "*" << endl;
	
	cdbg << "*\t\t\tPrinting the reverse def map:" << endl;
	for (int i=1; i < (int)rev_def_map.size()-1; i++)
	    cdbg << "*\t\t\t{" << i << ", " << rev_def_map[i] << "}" << endl;
	cdbg << "*" << endl;
	
	cdbg << "*\t\t\tPrinting the use map:" << endl;
	for (edi (use_to_i_map); edi != 0; edi++)
	    if ((*edi).second > 0)
		cdbg << "*\t\t\t{" << (*edi).second << ", "
		    << (*edi).first << "}" << endl;
	cdbg << "*" << endl;
	
	cdbg << "*\t\t\tPrinting the reverse use map:" << endl;
	for (int i=1; i < (int)rev_use_map.size()-1; i++)
	    cdbg << "*\t\t\t{" << i << ", " << rev_use_map[i] << "}" << endl;
	cdbg << "*" << endl;
    }	
	    
    return def_counter;
}

//---------------------------------------------------------------------;
int 
Renaming_data::do_renaming(Compound_region* rgn, 
			   int to_rename_with_copy)
{

    int i, is_proc = false;
    
    if (rgn->is_procedure()){ 	
	//procedure level renaming;
	is_proc = true;
    } else if (rgn->is_hb() || rgn->is_bb()) { 	
	//hyperblock renaming;
	is_proc = false;
    } else {
	assert(rgn->is_hb() || rgn->is_bb() || rgn->is_procedure());
    }
    
    //compute reaching definitions;
    El_do_reaching_defs ((Region*)rgn);
    Reaching_defs_info* rdi = get_reaching_defs_info ((Region*)rgn);
    
    //initialize the int-def, int-use, rev-def, rev-use maps;
    int def_counter = init_def_use_maps(rdi);
    
    m_net_num_groups = 0;
    int net_added = 0;
    
    for(i=1; i<def_counter; i++){
	int vr_num = rev_def_map[i].get_operand().vr_num();
	int net_idx = m_net_num_groups;
	
	for(int j=0; j<m_net_num_groups; j++){
	    if (m_net_vrnum[j]==vr_num){
		net_idx = j;
		break;
	    }
	}
	
	if (net_idx==m_net_num_groups){
	    // instantiate the NET list;
	    m_net[m_net_num_groups] = new List<Renaming_ops_net*>;
	    m_net_vrnum[m_net_num_groups] = vr_num;
	    m_net_num_groups++;
	}
	
	// form the strongly connected componants;
	Renaming_ops_net *net = new Renaming_ops_net;
	m_net[net_idx]->add_tail(net);
        net_added++;
	net->net_type = INTERNAL_NET;
	get_net(i, rdi, net);
    }
    
    
    //for all operands, rename where applicable;
    for (i=0; i<m_net_num_groups; i++) {
        Renaming_ops_net* net;
	
	//if #NET's of this operand <=1, donot rename;
	if (m_net[i]->size() <= 1){
            if (m_net[i]->size() ==1){
               List_iterator<Renaming_ops_net*>  sli (*m_net[i]);  
               delete *sli;
            }
            delete m_net[i]; 
	    continue;
        }

	List_iterator<Renaming_ops_net*> sli (*m_net[i]);
        net = *sli;

	// do not rename the first NET;
	sli++;
        delete net;
 
	for (; sli != 0; sli++) {
            net = *sli;
	    total_renamed += rename_net(rgn, net, is_proc, to_rename_with_copy);
            delete net;
        } 
        delete m_net[i];
    }

    
    remove_reaching_defs_info ((Region*)rgn);
    return total_renamed;    
}

//---------------------------------------------------------------------;
int 
Renaming_data::do_renaming(Compound_region* rgn, 
			   List<Edge*>* edges_to_break,
			   int to_rename_with_copy)
{

    int i, is_proc = false;
    
    if (rgn->is_procedure()){ 	
	//procedure level renaming;
	is_proc = true;
    } else if (rgn->is_hb() || rgn->is_bb()) { 	
	//hyperblock renaming;
	is_proc = false;
    } else {
	assert(rgn->is_hb() || rgn->is_bb() || rgn->is_procedure());
    }

    if (dbg(dcpr, 1)) 
	cdbg << "is-proc = " << is_proc << endl;

    //compute reaching definitions;
    El_do_reaching_defs ((Region*)rgn);
    Reaching_defs_info* rdi = get_reaching_defs_info ((Region*)rgn);
    
    //initialize the int-def, int-use, rev-def, rev-use maps;
    int def_counter = init_def_use_maps(rdi);
    
    m_net_num_groups = 0;
    int net_added = 0;
    
    if (edges_to_break->size() <= 0) {
	cdbg << "Renaming_data::do_renaming - no anti edges to break?\n";
	return 0;
    }

    cdbg << "edges to break; size = " << edges_to_break->size() << endl;

    Operand o1, o2;
    int old_vrnum;

    //to break an anti-dependence edge with renaming, the dest operand;
    //of the destination op of the edge has to be renamable - for now;
    //it is restricted to explicit vr dests;
    for(List_iterator<Edge*> li(*edges_to_break); li!=0; li++){
	Edge* e = *li;

	if (!(e->is_reg_anti() || e->is_reg_flow() || e->is_reg_out())){
	    //cdbg << "do_renaming:: wrong edge type to break!\n";
	    continue;
	}
	
	if (e->is_reg_out()){
	    o1 = e->src()->dest(e->src_port());
	    o2 = e->dest()->dest(e->dest_port());
	    if (!(o1==o2 && o1.is_reg() && o2.is_reg())){
		if (dbg(dcpr, 1)) 
		    cdbg << "do_renaming:: wrong out edge type to break!\n";
		continue;
	    }
	} else if (e->is_reg_anti()){
	    o1 = e->src()->src(e->src_port());
	    o2 = e->dest()->dest(e->dest_port());
	    if (!(o1==o2 && o1.is_reg() && o2.is_reg())) {
		if (dbg(dcpr, 1)) 
		    cdbg << "do_renaming:: wrong anti edge type to break!\n";
		continue;
	    }
	} else {
	    o1 = e->src()->dest(e->src_port());
	    o2 = e->dest()->src(e->dest_port());
	    if (!(o1==o2 && o1.is_reg() && o2.is_reg())){
		if (dbg(dcpr, 1)) 
		    cdbg << "do_renaming:: wrong flow edge type to break!\n";
		continue;
	    }
	}

	old_vrnum = o1.vr_num();

	if (dbg(dcpr, 1)) 
	    cdbg << e->src()->id() << " -> " 
		<< e->dest()->id() << "  old_vrnum = " << old_vrnum << endl;

	//if the dest op operand is not explicit, skip;
	for(int k=1; k<def_counter; k++){
	    Op* op = rev_def_map[k].get_op();
	    int reftype = rev_def_map[k].get_ref_type();
	    int vrnum = rev_def_map[k].get_operand().vr_num();

	    if (op==(e->dest()) && (reftype==EXP_DEST) && (vrnum==old_vrnum)){

		//form a net on the def "k" and rename that net;
		int net_idx = m_net_num_groups;
	
		for(int j=0; j<m_net_num_groups; j++){
		    if (m_net_vrnum[j]==vrnum){
			net_idx = j;
			break;
		    }
		}
		if (net_idx==m_net_num_groups){
		    // instantiate the NET list;
		    m_net[m_net_num_groups] = new List<Renaming_ops_net*>;
		    m_net_vrnum[m_net_num_groups] = vrnum;
		    m_net_num_groups++;
		}

		// now form the strongly connected componant for this;
		// anti dependence edge break from for loop;

		Renaming_ops_net *net = new Renaming_ops_net;
		m_net[net_idx]->add_tail(net);
		net_added++;
		net->net_type = INTERNAL_NET;
		get_net(k, rdi, net);
		break;
	    }
	}
    }
    
    
    //for all operands, rename where applicable;
    for (i=0; i<m_net_num_groups; i++) {
  
	Renaming_ops_net* net;
	List_iterator<Renaming_ops_net*> sli (*m_net[i]);
        net = *sli;
	
	/*
	//if #NET's of this operand <=1, donot rename;
	if (m_net[i]->size() <= 1){
            if (m_net[i]->size() ==1){
               delete *sli;
            }
            delete m_net[i]; 
	    continue;
        }
	*/

	// do not rename the first NET;
	//sli++;
        //delete net;
 
	for (; sli != 0; sli++) {
            net = *sli;
	    total_renamed += rename_net(rgn, net, is_proc, to_rename_with_copy);
            delete net;
        } 
        delete m_net[i];
    }

    remove_reaching_defs_info ((Region*)rgn);
    return total_renamed;    
}


//---------------------------------------------------------------------------;
// is horrible when it comes to inserting copies; for each use, insert a copy;
// at the def which reaches this use;
//---------------------------------------------------------------------------;
int 
Renaming_data::do_blind_renaming(Compound_region* rgn)
{

    //compute reaching definitions;
    El_do_reaching_defs ((Region*)rgn);
    Reaching_defs_info* rdi = get_reaching_defs_info ((Region*)rgn);
    
    //initialize the int-def, int-use, rev-def, rev-use maps;
    int def_counter = init_def_use_maps(rdi);

    for(int i=1; i<def_counter; i++){
	El_ref def_ref = rev_def_map[i];
	
	if (def_ref.get_ref_type() != EXP_DEST)
	    continue;

	Operand opr = def_ref.get_operand();

	if (!is_eligible_for_renaming(opr))
	    continue;

	//get all the uses of this def and rename them ;
	//if atleast one use has some other def reaching it;
	//then insert a copy;
 
	List<El_ref>* dul = rdi->get_du_chain(def_ref);
	if (dul->size()<=1){
	    //dont rename;
	    continue;
	}

	int old_nvn = def_ref.get_operand().vr_num();
	def_ref.get_operand().rename();
	int nvn = def_ref.get_operand().vr_num(); 
	int copy_required = false;
	total_renamed++;

	List_iterator<El_ref>  duli(*dul);
	
	for(; duli!=0; duli++){
	    El_ref use_ref = *duli;

	    List<El_ref>* udl = rdi->get_ud_chain(use_ref);
	    if (udl->size() >1){
		//other defs reach this use;
		//and donot rename this use;
		copy_required = true;
	    } else {
		//rename;
		use_ref.get_operand().rename(nvn);
	    }
	}
	if (copy_required==true){
	    Op *new_op;
	    Op *old_op = def_ref.get_op();
	    Operand cur_dest = old_op->dest(DEST1);
	    Operand new_reg = new Reg(old_op->dest(DEST1));
	    
	    new_reg.rename(nvn);
	    cur_dest.rename(old_nvn);
	    
	    if (dbg(dcpr,3)){
		cdbg << "old op = " << old_op->id() << endl;
		cdbg << "old op = " << *(def_ref.get_op()) << endl;
		cdbg << "def ref type = " << (int)def_ref.get_ref_type() << endl;
		cdbg << "old dest     = " << cur_dest << endl;
		cdbg << "new dest     = " << new_reg << endl;
		cdbg << "data type    = " << cur_dest.dtype() << endl;
	    }

	    new_op = new Op(get_move_opcode_for_operand(cur_dest));
	    new_op->set_dest(DEST1, cur_dest);
	    new_op->set_src(SRC1, new_reg);
	    new_op->set_src(PRED1, old_op->src(PRED1));

	    if (dbg(dcpr,2)){
		cdbg << "new op = " << new_op->id() << endl;
	    }
	    
	    El_insert_op_after(new_op, old_op);
	}
    }

    remove_reaching_defs_info ((Region*)rgn);
    return total_renamed;    
    
}

//---------------------------------------------------------------------------
ostream& 
operator<<(ostream& os, const Renaming_ops_net& net)
{
    os << "[" << net.net_type  << "] ";
    os << "<" << net.num_internal_defs << ", " 
       << net.num_internal_uses << ", "
       << net.num_live_outs << ", "
       << net.num_live_ins << "> \n";
    for(List_iterator<Pair<int, int>*> iter(*(net.reflist)); iter != 0; ++iter){
	int f = (*iter)->first;
	int s = (*iter)->second;
	os << "<" << f << "," << s << "> ";
    }
    os << endl;
    return os;
}

int is_eligible_for_renaming(Operand &opr)
{
    if (!opr.is_reg()) return 0;
    enum Reg_file ftype;
    ftype = opr.file_type();
    /* RMR { make VIR, VFR, and VMR interesting } */
    if (ftype==GPR || ftype==FPR || ftype==BTR || ftype==VIR || ftype==VFR || ftype==VMR)
	return 1;
    else 
	return 0;
}

//###############################################################################
int El_rename_region(Compound_region* rgn){

    Dlist<Region*> rstack;
    bool push_flag;

    int total_renamed =0;
    if (El_rename_method==DO_PROCEDURE_RENAMING){
	if (dbg(dcpr, 1)) 
	    cdbg << "procedure renaming (no copy)\n";
	Renaming_data rdat((Compound_region *)rgn);
	total_renamed = rdat.do_renaming((Compound_region*)rgn);
    } else if (El_rename_method==DO_HB_RENAMING){
	rstack.push(rgn);
	while (! rstack.is_empty()) {
	    Region* tmp_rgn = rstack.pop();
	    push_flag = true;
	    if (tmp_rgn->is_hb() || tmp_rgn->is_bb()) {
		Renaming_data rdat((Compound_region *)tmp_rgn);
		total_renamed += rdat.do_renaming((Compound_region *)tmp_rgn);
		if (dbg(dcpr, 1)) {
		    cdbg << "HB <" << ((Compound_region *)tmp_rgn)->id() 
			 << "> renaming (no copy). No. renamed = "
			 << total_renamed << "\n";
		}
		push_flag = false; 
	    }
	    if (push_flag==true) {
		for (Region_subregions subri(tmp_rgn); subri!=0; subri++) {
		    if ((*subri)->is_compound())
			rstack.push(*subri);
		}
	    }
	}
    }
    return total_renamed;
}


//###############################################################################
int El_rename_with_copy_region(Compound_region* rgn){
    Renaming_data rdat((Compound_region *)rgn);
    return rdat.do_renaming((Compound_region *)rgn, true);
}

//###############################################################################
// the edges_to_break could be a subset of the critical path anti dependences
// or just some particular edge (in which case, the list consists of one 
// element. How we pick these is a diff. matter.
//###############################################################################
int El_rename_with_copy_region(Compound_region* rgn, List<Edge*>* edges_to_break){
    Renaming_data rdat((Compound_region *)rgn);
    return rdat.do_renaming((Compound_region *)rgn, edges_to_break, true);
}


//###############################################################################;
int El_rename_blindly_region(Procedure* f)
{

    Dlist<Region*> rstack;
    bool push_flag;
    int total_renamed =0;

    rstack.push(f);
    while (! rstack.is_empty()) {
	Region* rgn = rstack.pop();
	push_flag = true;
	if (rgn->is_hb() || rgn->is_bb()) {
	    Renaming_data rdat((Compound_region *)rgn);
	    total_renamed += rdat.do_blind_renaming((Compound_region *)rgn);
	    if (dbg(dcpr, 1)) {
		cdbg << "HB <" << ((Compound_region *)rgn)->id() 
		    << "> renaming (blind). No. renamed = "
		    << total_renamed << "\n";
	    }
	    push_flag = false; 
	}
	if (push_flag==true) {
	    for (Region_subregions subri(rgn); subri!=0; subri++) {
		if ((*subri)->is_compound())
		    rstack.push(*subri);
	    }
	}
    }

    return total_renamed;
}



