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
//      File:           ls_adv_load.cpp
//      Authors:        Teresa L. Johnson, Santosh Abraham
//      Created:        June 1996
//      Description:    Convert loads to advanced loads and load verifies
//
/////////////////////////////////////////////////////////////////////////////

#define USE_BRDV 0

#include "ls_adv_load.h"
#include "load_info.h"
#include "load_par.h"
#include "ls_get_profile.h"
#include "ls_efilt.h"
#include "ls_prof_mem_edge.h"
#include "el_cache_init.h"

#include "iterators.h"
#include "op_info.h"
#include "opcode_properties.h"
#include "hash_functions.h"
#include "sched_functions.h"
#include "core_edge_drawing.h"
#include "edge_utilities.h"
#include "dbg.h"
#include "intf.h"
#include "slist.h"
#include "attributes.h"
#include "region_attributes.h"

void convert_load_to_adv_load (Compound_region* region, Op* op) {
  // Change the LD to an LDS
  Opcode opc = op->opcode();
  if (opc & IR_INCREMENT)
  {
    cerr << "Ignoring incrementing load!" << endl;
    return;
  }
  //cerr << *op << endl;
  cerr << op->id() << " " << "load opcode: " << opc << " ... ";
  if (((opc) & ((ROOT_MASK))) == ROOT_FLD)
    opc = (Opcode) (((opc) & (~(ROOT_MASK))) | ROOT_FDSLD);
  else
    opc = (Opcode) (((opc) & (~(ROOT_MASK))) | ROOT_DSLD);
  cerr << "advanced load opcode: " << opc << endl;
  op->set_opcode(opc);

  // Create the LDV op
#if USE_BRDV
  if (((opc) & ((ROOT_MASK))) == ROOT_FDSLD)
    opc = (Opcode) ROOT_BRDVF;
  else
    opc = (Opcode) ROOT_BRDVI;
  Op *ldv = new Op(opc, 2, 0, op->predicated());
  ldv->set_src((Port_num)(op->first_src()+1),op->dest(op->first_output()));
  Reg *reg1 = new Reg(EL_RDT_BRANCH);
  Operand operand1 = reg1;
  ldv->set_src(op->first_src(),operand1);
  cerr << "\tBRDV " << ldv->id() << endl;

  // Need a pbr op as well
  opc = (Opcode) PBR;
  Op *pbr = new Op(opc, 2, 1, op->predicated());
  // first dest is SRC1 reg of brdv
  pbr->set_dest(op->first_output(),operand1);
  // first source is label of compensation code
  char label[25];
  sprintf(label,"_compensation%d",op->id());
  eString str1(label);
  Label_lit *lit0 = new Label_lit(str1);
  operand1 = lit0;
  pbr->set_src(op->first_src(),operand1);
  // second source is 0 indicating predict not taken
  Int_lit *lit1 = new Int_lit(0);
  operand1 = lit1;
  pbr->set_src((Port_num)(op->first_src()+1),operand1);

#else
  if (((opc) & ((ROOT_MASK))) == ROOT_FDSLD)
    opc = (Opcode) (((opc) & (~(ROOT_MASK)) & (~(IR_SRC_MASK)) &
		(~(IR_DEST_MASK))) | ROOT_FDVLD);
  else
    opc = (Opcode) (((opc) & (~(ROOT_MASK)) & (~(IR_SRC_MASK)) &
		(~(IR_DEST_MASK))) | ROOT_DVLD);
  Op *ldv = new Op(opc, op->num_srcs(), op->num_dests(), op->predicated());
  for (int i = op->first_input(); i <= op->last_input(); i++) {
    ldv->set_src((Port_num)(i),op->src((Port_num)(i)));
  }
  for (int j = op->first_output(); j <= op->last_output(); j++) {
    ldv->set_dest((Port_num)(j),op->dest((Port_num)(j)));
  }
#endif

  // Copy input edges of SRC1, MEM and C1 to ldv
  for (Op_inedges iter(op); iter != 0; iter++) {
    Edge* edge = (*iter);
    if ((edge->is_control() && 
	edge->dest_port_type() == CONTROL && edge->dest_port() == CONTROL1) || 
        (edge->is_mem()) ||
    	(edge->is_reg_dep() && 
	edge->dest_port_type() == SRC && edge->dest_port() == SRC1))
    {
#if USE_BRDV
	// If we can use brdv's, don't need to copy SRC1 edges to brdv
	if (edge->dest_port_type() == SRC) continue;
#endif
	Edge *new_edge = edge->clone();
	new_edge->set_src(edge->src(),edge->src_port(),edge->src_alt());
	new_edge->set_dest(ldv,new_edge->dest_port(),new_edge->dest_alt());
	ldv->add_inedge(new_edge,new_edge->dest_port_type(),
			new_edge->dest_port(),new_edge->dest_alt());
	edge->src()->add_outedge(new_edge,new_edge->src_port_type(),
				new_edge->src_port(),new_edge->src_alt());
    }
  }

  // Copy output edges of DEST1, SRC1, MEM, C0 and C1 to ldv
  for (Op_outedges iter2(op); iter2 != 0; iter2++) {
    Edge* edge = (*iter2);
    if ((edge->is_control()) ||
        (edge->is_mem()) ||
    	(edge->is_reg_dep() && 
	(edge->src_port_type() == SRC && edge->src_port() == SRC1) ||
	(edge->src_port_type() == DEST && edge->src_port() == DEST1)))
    {
#if USE_BRDV
	// If we can use brdv's, don't need to copy SRC1 or DEST1 edges to brdv
	if (edge->src_port_type() == DEST || edge->src_port_type() == SRC)
	    continue;
#endif
	Edge *new_edge = edge->clone();
	new_edge->set_src(ldv,new_edge->src_port(),new_edge->src_alt());
	new_edge->set_dest(edge->dest(),new_edge->dest_port(),
						new_edge->dest_alt());
	ldv->add_outedge(new_edge,new_edge->src_port_type(),
			new_edge->src_port(),new_edge->src_alt());
	edge->dest()->add_inedge(new_edge,new_edge->dest_port_type(),
				new_edge->dest_port(),new_edge->dest_alt());
#if USE_BRDV
	// If this is the control 0 edge, save it for later use
	if (edge->is_control() && edge->src_port() == CONTROL0)
	    not_taken = new_edge;
#endif
    }
  }

  // Remove input edges to MEM of lds
  for (Op_inedges iter3(op); iter3 != 0; iter3++) {
    Edge* edge = (*iter3);
    if (edge->is_mem())
    {
	if (is_store(edge->src())) {
	  // Get the lds' mdp attributes and see if there is one for st
	  MemDepProf_attribute_map* map = 
			get_mdp_attributes((Region *) op);
	  if (map->is_bound(edge->src()->id()) &&
	      	(double)map->value(edge->src()->id())/
		(double)get_load_exec_freq(op) > El_cache_alias_thresh) {
	      cerr << "Leaving MEM edge from store to lds" << endl;
	      continue;
	  }
	}
	op->remove_inedge(edge,edge->dest_port_type(),edge->dest_port(),
		edge->dest_alt());
	edge->src()->remove_outedge(edge,edge->src_port_type(),
		edge->src_port(),edge->src_alt());
    }
  }

  // Remove output edges from SRC1 of lds
  // Remove output edges from MEM? of lds
  // Remove output edges from CONTROL0 of lds
  for (Op_outedges iter4(op); iter4 != 0; iter4++) {
    Edge* edge = (*iter4);
    if (is_CONTROL0(edge) || edge->is_mem() || (edge->is_reg_dep() && 
	edge->src_port_type() == SRC && edge->src_port() == SRC1))
    {
	op->remove_outedge(edge,edge->src_port_type(),edge->src_port(),
		edge->src_alt());
	edge->dest()->remove_inedge(edge,edge->dest_port_type(),
		edge->dest_port(),edge->dest_alt());
    }
  }

  // Copy src memvrs from lds to ldv
  // Remove src memvrs from lds
     for (Hash_set_iterator<Operand> hiter(op->mem_srcs()); hiter != 0; hiter++) {
        Operand operand(*hiter);
        ldv->add_mem_src(operand);
        op->remove_mem_src(*hiter);
     }
  // Copy dest memvrs from lds to ldv
  // Remove dest memvrs from lds
     for (Hash_set_iterator<Operand> hiter2(op->mem_dests()); 
     hiter2 != 0; hiter2++) {
        Operand operand(*hiter2);
        ldv->add_mem_dest(operand);
        op->remove_mem_dest(*hiter2);
  }
  
  // Copy the rel ops from lds to ldv
  for (List_iterator<Op* > oiter(op->get_relops()); oiter != 0; oiter++) {
    ldv->add_relops(*oiter);
  }

#if USE_BRDV
  // Attach a dest memvr to lds
  Mem_vr *memvr = new Mem_vr;
  Operand operand = memvr;
  op->add_mem_dest(operand);

  // Attach a src memvr to brdv
  ldv->add_mem_src(operand);

  // Make an new MEM dep edge between lds and brdv
  Edge *edge = new Mem;
  edge->set_src(op,MEM0);
  edge->set_dest(ldv,MEM0);
  edge->set_latency();
  op->add_outedge(edge,MEM,MEM0,0);
  ldv->add_inedge(edge,MEM,MEM0,0);

  // Make a flow dep between pbr and brdv (DEST1 to SRC1)
  edge = new Reg_flow;
  edge->set_src(pbr,DEST1);
  edge->set_dest(ldv,SRC1);
  pbr->add_outedge(edge,DEST,DEST1,0);
  ldv->add_inedge(edge,SRC,SRC1,0);

 // Don't need this since the brdv does not read the reg value?!
  // Make anti deps to dests of output deps of lds
  //for (Op_outedges iter5(op); iter5 != 0; iter5++) {
  //  edge = (*iter5);
  //  if (edge->is_reg_out() && 
  //		edge->src_port_type() == DEST && edge->src_port() == DEST1)
  //  {
  //	Edge *edge1 = new Reg_anti;
  //	edge1->set_src(ldv,SRC2);
  //	edge1->set_dest(edge->dest(),DEST1);
  //	ldv->add_outedge(edge1,SRC,SRC2,0);
  //	edge->dest()->add_inedge(edge1,DEST,DEST1,0);
  //  }
  //}

  // Make an new C0 edge between lds and pbr
  edge = new Control;
  edge->set_src(op,CONTROL0);
  edge->set_dest(pbr,CONTROL0);
  edge->set_latency();
  op->add_outedge(edge,CONTROL,CONTROL0,0);
  pbr->add_inedge(edge,CONTROL,CONTROL0,0);

  // Make an new C0 edge between pbr and brdv
  edge = new Control;
  edge->set_src(pbr,CONTROL0);
  edge->set_dest(ldv,CONTROL0);
  edge->set_latency();
  pbr->add_outedge(edge,CONTROL,CONTROL0,0);
  ldv->add_inedge(edge,CONTROL,CONTROL0,0);

#else
  // Make an output dep between lds and ldv (DEST1 to DEST1)
  Edge *edge = new Reg_out;
  edge->set_src(op,DEST1);
  edge->set_dest(ldv,DEST1);
  op->add_outedge(edge,DEST,DEST1,0);
  ldv->add_inedge(edge,DEST,DEST1,0);

  // Make an new C0 edge between lds and ldv
  edge = new Control;
  edge->set_src(op,CONTROL0);
  edge->set_dest(ldv,CONTROL0);
  edge->set_latency();
  op->add_outedge(edge,CONTROL,CONTROL0,0);
  ldv->add_inedge(edge,CONTROL,CONTROL0,0);
#endif

  // reset latencies on lds edges
  Edge_style es = ES_SCALAR;
  for (Op_inedges iter6(op); iter6 != 0; iter6++) {
    Edge* edge = (*iter6);
    if (is_CONTROL0(edge)) continue;
    set_edge_latency (edge, es);
  }
  for (Op_outedges eiter(op); eiter != 0; eiter++) {
    Edge* edge = (*eiter);
    if (is_CONTROL0(edge)) continue;
    set_edge_latency(edge, es);
  }

  // reset latencies on ldv edges
  for (Op_inedges iter7(ldv); iter7 != 0; iter7++) {
    Edge* edge = (*iter7);
    if (is_CONTROL0(edge)) continue;
    set_edge_latency(edge, es);
  }
  for (Op_outedges eiter2(ldv); eiter2 != 0; eiter2++) {
    Edge* edge = (*eiter2);
    if (is_CONTROL0(edge)) continue;
    set_edge_latency(edge, es);
  }

  // If there is a preceeding jsr in C0 order, draw a mem edge from it to lds
  assert(loadInfoMap->is_bound(op));
  LoadInfo* ldinfo = loadInfoMap->value(op);
  Op *last_jsr = ldinfo->get_last_jsr();
  if (last_jsr) {
    edge = new Mem;
    edge->set_src(last_jsr,MEM0);
    edge->set_dest(op,MEM0);
    edge->set_latency(1);
    last_jsr->add_outedge(edge,MEM,MEM0,0);
    op->add_inedge(edge,MEM,MEM0,0);
  }

  // Attach ldv to lds's loadInfo
  ldinfo->set_ldv(ldv);

  // add ldv to region
  region->add_region(ldv);
  ldv->set_parent(region);

#if USE_BRDV
  // add pbr to region
  region->add_region(pbr);
  pbr->set_parent(region);
#endif

  // Recompute etimes and ltimes. Is there a better way to
  // do this?
  calculate_earliest_and_latest_times(region, CONTROL_SPEC_ALWAYS,
  	NON_PSEUDO_OPS, NON_CONTROL0_INEDGES, NON_CONTROL0_OUTEDGES);
}

double calculate_alias_probability(Compound_region *region, Op *lds)
{
  assert(loadInfoMap->is_bound(lds));
  LoadInfo* ldinfo = loadInfoMap->value(lds);
  Op *ldv = ldinfo->get_ldv();
  int alias_cnt = 0;
  for (Region_ops_C0_order oiter (region, lds); oiter != 0; oiter++) {
    Op* op = *oiter;

    // Check if this is the lds' ldv
    if (op == ldv) break;

    // Check if this is a store
    if (!is_store(op)) continue;

    // Get the lds' mdp attributes and see if there is one for store
    MemDepProf_attribute_map* map = get_mdp_attributes((Region *) lds);
    if (!map->is_bound(op->id())) continue;
    alias_cnt += map->value(op->id());
  }
  cerr << "lds aliases with freq " << alias_cnt << endl;
  int freq = get_load_exec_freq(lds);
  double p = 0.0;
  if (freq)
	p = (double)(alias_cnt)/(double)(freq);
  return p;
}
