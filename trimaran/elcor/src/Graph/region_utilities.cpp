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
//      File:           region_utilities.cpp
//      Authors:        Sadun Anik, Vinod Kathail, Richard Johnson,
//                      Dave August
//      Created:        December 1994
//      Description:    Utilities for regions
//
/////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "region_utilities.h"
#include "iterators.h"
#include "op.h"
#include "opcode_properties.h"
#include "edge_utilities.h"
#include "el_error.h"
#include "edge.h"
#include "hash_functions.h"
#include "attributes.h"

Op* get_first_region_op_from_inops(Region* R) {
   Hash_set<Op*> tmplist = R->inops() ;
   if (tmplist.size() == 1) {
      return(tmplist.head()) ;
   } else {
      El_punt("get_first_region_op: trying to get the first op from a multiple entry region") ;
   }
   return(0) ;
}

Op* get_last_region_op_from_outops(Region* R) {
   Hash_set<Op*> tmplist = R->outops() ;
   if (tmplist.size() == 1) {
      return(tmplist.head()) ;
   } else {
      El_punt("get_last_region_op: trying to get the last op from a multiple exit region") ;
   }
   return(0) ;
}

Op* get_first_region_op_from_subregions(Region* R) {
   Dlist<Region*>& tmplist = R->subregions() ;
   Region* tmphead = tmplist.head() ;
   if (tmphead->is_op()) return ((Op*)tmphead) ;
   else return (get_first_region_op_from_subregions(tmphead)) ;
}

Op* get_last_region_op_from_subregions(Region* R) {
   Dlist<Region*>& tmplist = R->subregions() ;
   Region* tmptail = tmplist.tail() ;
   if (tmptail->is_op()) return ((Op*)tmptail) ;
   else return (get_last_region_op_from_subregions(tmptail)) ;
}

bool region_contains_edge(Region* r, Edge* e)
{
  Op* e_src = e->src() ;
  Op* e_dest = e->dest() ;   
  if (region_contains_op(r, e_src) && region_contains_op(r, e_dest)) {
    if (is_switch(e_dest)) return true;  // BRTOP edges have this property
    if (is_control_switch(e_src) || is_switch(e_src)) {
      Edge* C0_edge = get_corresponding_CONTROL0_edge(e) ;
      if ((r->outedges()).is_member(C0_edge)) {
	return false ;
	 } else {
	    return(true) ;
	 }
      } else {
	 return true;
      }
   } else {
      return false ;
   }
}

bool region_contains_op(Region* r, Op* op)
{
   if (r->is_op()) {
      if (r == (Region*) op) return true ;
      else return false ;
   }
   Region* tmp_r = op->parent() ;
   while(tmp_r != NULL) {
      if (r == tmp_r) return true ;
      else tmp_r = tmp_r->parent() ;
   }
   return false ;
}

bool region_contains_op(Region* r, int op_id)
{
   if (r->is_op()) {
     if (r->id() == op_id) return true ;
      else return false ;
   }
   for(Region_all_ops rops (r); rops != 0; rops++) {
     if((*rops)->id() == op_id)
       return true;
   }
   return false ;
}

bool region_edge_crosses_boundary(Region* r, Edge* e)
{
   if (region_contains_edge(r, e)) {
      return false ;
   } else {
      if ((region_contains_op(r, e->src())) ||
	  (region_contains_op(r, e->dest()))) {
	 return true ;
      } else {
	 return false ;
      }
   }
}

bool region_op_at_boundary(Region* r, Op* op)
{
   if (region_op_at_entry(r,op) || region_op_at_exit(r,op)) return true;
   else return false ;
}

bool region_op_at_entry(Region* r, Op* op)
{
   if ((r->inops()).is_member(op)) return true ;
   else return false ;
}

bool region_op_at_exit(Region* r, Op* op)
{
   if ((r->outops()).is_member(op)) return true ;
   else return false ;
}

bool region_has_predicated_ops(Region* r)
{
  if (r->is_op()) {
    return is_conditionally_executed((Op*)r) ;
  } else if (r->is_bb() || r->is_hb()) {
    // traverse ops in block in forward linear order
    for (Region_ops_C0_order opi((Compound_region*)r); opi != 0; ++opi) {
      Op* op = (Op*) (*opi);
      if (is_conditionally_executed(op)) return true;
    }
  } else {
    for (Region_subregions sub(r); sub != 0; sub++) {
      if (region_has_predicated_ops(*sub)) {
	return true;
      }
    }
  }
  return false;
}

Region* ancestor_with_same_parent(Region* region, Region* other_region)
{
  if (other_region == 0) assert(0);
  Region* temp = region;
  Region* parent = other_region->parent();
  while((temp != 0) && (temp->parent() != parent))
    temp = temp->parent();
  return temp;
}

//  --------------------region_find_common_ancestor---------------------------
// Find the closest region that is ancestor of the two regions

Region* region_find_common_ancestor(Region* region1, Region* region2){
   Region* temp1 = region1->parent();
   Region* temp2 ;
   while(temp1 != NULL) {
      temp2 = region2->parent() ;
      while (temp2 != NULL) {
	 if (temp1 == temp2) return temp2;
	 temp2 = temp2->parent();
      }
      temp1 = temp1->parent();
   }
   cerr << "No common ancestor -- region hierarchy is all mangled up \n";
   assert(0);
   return((Region*)NULL);
}

  
Alt_num get_new_in_alt_num(Op* op)
{
   int max_alt = 0 ;
   for (Op_inedges inedge_i(op, CONTROL, CONTROL0) ; inedge_i!= 0; inedge_i++) {
      int cur_alt = (*inedge_i)->dest_alt() ;
      max_alt = ELCOR_MAX(max_alt,cur_alt) ;
   }
   return(max_alt+1) ;
}

Alt_num get_new_out_alt_num(Op* op)
{
   int max_alt = 0 ;
   for (Op_outedges outedge_i(op, CONTROL, CONTROL0) ; outedge_i!= 0; outedge_i++) {
      int cur_alt = (*outedge_i)->src_alt() ;
      max_alt = ELCOR_MAX(max_alt,cur_alt) ;
   }
   return(max_alt+1) ;
}

bool is_dataflow_merge_switch(Op* op)
{
   Op_inedges e_ii(op) ;
   if (e_ii != 0) {
      if ((*e_ii)->is_reg_flow()) {
	 return true ;
      } else {
	 return false ;
      }
   }
   Op_outedges e_oi(op) ;
   if (e_oi != 0) {
      if ((*e_oi)->is_reg_flow()) {
	 return true ;
      } else {
	 return false ;
      }
   }
   return true ; // If the node is not connected, then assume true
}

Op* get_relop_of_merge_switch(Op* op)
{
   List_iterator<Op*> relops_i(op->get_relops()) ;
   return (*relops_i) ;
}

// Compare region frequencies
bool region_less_than (Region* region1, Region* region2) {
    if (region1->is_compound() && region2->is_compound())
	return (((Compound_region*)region1)->weight < 
		((Compound_region*)region2)->weight);
    else return(false);
}

Hash_set<Region*> El_hb_bb_set(hash_region_ptr, 311);
Hash_map<int, Region*> El_hb_bb_id_map(hash_int, 311);

//
//      Set up map of hb/bb ids to their corresponding
//      objects (the id space of hb/bb is assumed to be unique)
void El_setup_hb_bb_id_map(Region *reg)
{
    bool push_flag;
    Dlist<Region*> rstack;
    Region *rtmp;

    El_hb_bb_set.clear();
    El_hb_bb_id_map.clear();

    rstack.push(reg);
    while (! rstack.is_empty()) {
        rtmp = rstack.pop();
        push_flag = true;
        if (rtmp->is_hb()) {
            El_hb_bb_set += rtmp;
            El_hb_bb_id_map.bind(rtmp->id(), rtmp);
        }
        else if (rtmp->is_bb()) {
            El_hb_bb_set += rtmp;
            El_hb_bb_id_map.bind(rtmp->id(), rtmp);
            push_flag = false;  // Only ops below, so don't waste time!
        }
        if (push_flag==true) {
            for (Region_subregions subri(rtmp); subri!=0; subri++) {
                if ((*subri)->is_compound())
                    rstack.push(*subri);
            }
        }
    }
}

Hash_map<int, Op*> El_op_id_map(hash_int, 311);

//
//      Set up map of op ids to their corresponding
//      objects (the id space of op is assumed to be unique)
void El_setup_op_id_map(Region *reg)
{
    Compound_region *rtmp;

    El_op_id_map.clear();

   for (Region_subregions sri(reg) ; sri != 0 ; sri++) {
      Region* cur_subregion = *sri ;

      if (!(cur_subregion->is_compound()))
	continue;

       rtmp = (Compound_region*) cur_subregion;

       // Bind all of the ops
       for (Region_ops_C0_order oi1(rtmp); oi1 != 0; oi1++) {
         Op* op1 = (*oi1);
         El_op_id_map.bind(op1->id(), op1);
       }
    }
}

Procedure* el_get_enclosing_procedure(Region* reg)
{
  Compound_region* parent=(Compound_region *)reg;
  
  while(!parent->is_procedure())
	{
	  parent=parent->parent();
	}
  
  return (Procedure*)parent;
}


void make_macros_canonical(Region* reg)
{
  const Hash_set<eString>& aliases = current_MDES->get_MDES_alias_macros();

  for(Region_all_ops rops(reg); rops != 0; rops++) {
    Op* op = *rops;
    List_set<Operand> to_remove;
    for (Op_all_inputs ins(op); ins != 0; ++ins) {
      Operand opr = *ins;
      if(opr.is_macro_reg() && ! opr.allocated()) {
	const eString& my_name = el_macro_to_string_map[opr.name()];
	if(aliases.is_member(my_name)) {
	  const eString& real_name = current_MDES->get_macro_aliased(my_name);
	  // Can't change macros that alias the stack pointer, because
	  // this might be called prior to doing the stack
	  // layout. This would essentially destroy information needed
	  // to do the layout correctly.
	  if(real_name == "SP_REG" || real_name == "RGS_REG")
	    continue;
	  Operand new_opr(new Macro_reg(el_string_to_macro_map[real_name]));
	  if(opr.assigned_to_physical_file())
	    new_opr.bind_physical_file(opr.physical_file_type());
	  if (ins.ref_type() != IMP_SRC) {
	    op->set_src(ins.get_port_num(), new_opr);
	  } else {
	    List<Operand>* imp_srcs = get_implicit_use_list(op);
	    Node<Operand>* old_imp_src = imp_srcs->find(opr);
	    imp_srcs->insert_after(old_imp_src, new_opr);
	    // Can't remove these nodes in the middle of the loop,
	    // because the iterator gets screwed up, and may
	    // segfault. Instead, add them to a list, and remove them
	    // outside the loop.
	    //  imp_srcs->remove(opr);
	    to_remove += opr;
	  }	  
	} // if it's an alias macro
      } // if it's a macro and not allocated
    } // for all inputs

    for(List_set_iterator<Operand> lsi(to_remove); lsi != 0; lsi++) {
      get_implicit_use_list(op)->remove(*lsi);
    }

    to_remove.clear();
    for (Op_all_dests dests(op); dests != 0; ++dests) {
      Operand opr = *dests;
      if(opr.is_macro_reg() && ! opr.allocated()) {
	const eString& my_name = el_macro_to_string_map[opr.name()];
	if(aliases.is_member(my_name)) {
	  const eString& real_name = current_MDES->get_macro_aliased(my_name);
	  // Can't change macros that alias the stack pointer, because
	  // this might be called prior to doing the stack
	  // layout. This would essentially destroy information needed
	  // to do the layout correctly.
	  if(real_name == "SP_REG" || real_name == "RGS_REG")
	    continue;
	  Operand new_opr(new Macro_reg(el_string_to_macro_map[real_name]));
	  if(opr.assigned_to_physical_file())
	    new_opr.bind_physical_file(opr.physical_file_type());
	  if (dests.ref_type() != IMP_DEST) {
	    op->set_dest(dests.get_port_num(), new_opr);
	  } else {
	    List<Operand>* imp_dsts = get_implicit_def_list(op);
	    Node<Operand>* old_imp_dst = imp_dsts->find(opr);
	    imp_dsts->insert_after(old_imp_dst, new_opr);
	    // See comment above about screwing up the iterator.
	    // imp_dsts->remove(opr);
	    to_remove += opr;
	  }
	} // if it's an alias macro
      } // if it's a macro and not allocated
    } // for all dests

    for(List_set_iterator<Operand> lsi(to_remove); lsi != 0; lsi++) {
      get_implicit_def_list(op)->remove(*lsi);
    }

  } // for all ops
}


//Function for finding the closest common ancestor of two compound
//regions second and first; anc_second and anc_first are the ancestors 
//of the second and first that are the direct children of the common ancestor
Compound_region* innermost_common_ancestor(Region* second,
					   Region* first,
					   Compound_region*& anc_second,
					   Compound_region*& anc_first)
{
  Region *as,*as_old=NULL;
  Region *af,*af_old=NULL;
  
  for(as=second;as!=NULL;as=as->parent())
    {
      for(af=first;af!=NULL;af=af->parent())
	{
	  if(as==af)
	    {
	      anc_second=(Compound_region*)as_old;
	      anc_first=(Compound_region*)af_old;
	      return (Compound_region*)as;
	  }
	  else
	    {
	      af_old = af;
	    }
	}
      as_old = as;
    }
  assert(0);  //Shouldn't get here: at the very least, the proc should be the common ancestor
  return (Compound_region*)as; //keep compiler happy
}


// Calculate the execution frequency of an op from the parent's weight and
// the branch exit frequencies.
double El_calc_op_weight(Op *op)
{
    Compound_region *parent;
    double cur_weight, taken_weight;
    Edge *edge;
    Op *cur_op;
    Control_flow_freq *cfreq;

    parent = op->parent();
    if (parent == NULL)
	El_punt("El_calc_op_weight: op %d has no parent", op->id());
    if (parent->is_bb())
	return (parent->weight);
    else if (! parent->is_hb()) {
	El_warn("El_calc_op_weight: unable to determine, unknown parent reg type");
	return (0.0);
    }

    cur_weight = parent->weight;
    for (Region_ops_C0_order op_i(parent); op_i!=0; op_i++) {
	cur_op = *op_i;
	if (cur_op == op) {
	    if (cur_weight < 0.0)
		return (0.0);
	    else
	        return (cur_weight);    
	}
	if (is_branch(cur_op)) {
	    /* Subtract off all weights of non-ft edges */
	    taken_weight = 0.0;
	    for (Op_outedges edge_i(cur_op, CONTROL0_OUTEDGES);
					edge_i!=0; edge_i++) {
		edge = *edge_i;
		if (is_fall_through(edge))
		    continue;
		cfreq = get_control_flow_freq(edge);
		if (cfreq == NULL)
		    El_warn("El_calc_op_weight: branch %d missing taken freq",
				cur_op->id());
		else
		    taken_weight += cfreq->freq;
	    }
	    cur_weight -= taken_weight;
	}
    }

    El_punt("El_calc_op_weight: op %d not found by walking its parent region",
		op->id());
    return (0.0);
}

//
// This function installs a cfreq attribute on the FT edge of all branches
// if they are missing.  It assumes the taken edge attrs are there, otherwise
// it is not possible to compute the correct frequency.
void El_install_missing_cfreq_attrs(Compound_region *cr)
{
    Edge *ft_edge, *edge;
    Op *op;
    double taken_weight, ft_weight, cur_weight;
    bool ft_cfreq_needed;
    Control_flow_freq *cfreq;

    if (! (cr->is_bb() || cr->is_hb())) {
	El_warn("El_install_missing_cfreq_attrs: cannot handle non-BB/HB regions");
	return;
    }

    cur_weight = cr->weight;
    for (Region_ops_C0_order o_iter(cr); o_iter!=0; o_iter++) {
        op = *o_iter;
        if (! is_branch(op))
            continue;
	taken_weight = 0.0;
	ft_edge = NULL;
	ft_cfreq_needed = false;
        for (Op_outedges e_iter(op, CONTROL0_OUTEDGES); e_iter!=0; e_iter++) {
            edge = *e_iter;
	    cfreq = get_control_flow_freq(edge);
            if (is_fall_through(edge)) {
                ft_edge = edge;
	        if (cfreq == NULL)
		    ft_cfreq_needed = true;
		else
		    ft_cfreq_needed = false;
	    }
	    else {
	        if (cfreq == NULL)
		    El_warn("El_install_missing_cfreq_attrs: missing cfreq for taken edge on branch %d", op->id());
		else
		    taken_weight += cfreq->freq;
	    }
        }
	ft_weight = cur_weight - taken_weight;
	if (ft_weight < 0.0)
	    ft_weight = 0.0;
	if (ft_cfreq_needed) {
	    cfreq = new Control_flow_freq();
	    cfreq->freq = ft_weight;
	    set_control_flow_freq(ft_edge, cfreq);
	}
	else if (ft_edge != NULL) {
	    /* Check the weight */
	    cfreq = get_control_flow_freq(ft_edge);
	    if (fabs(cfreq->freq - ft_weight) > .01) {
		El_warn("El_install_missing_cfreq_attrs: inconsistent ft weight for branch %d", op->id());
		cfreq->freq = ft_weight;
	    }
	}
	cur_weight -= taken_weight;
    }
}

//////////////////////////////////////////////////////////////////

// Fixing Dlist order
//
// This function sorts the linear subsequences of a region.
// Each subsequence starts from a node which does not have
// a fall through entry from a node in this region. The exceptions
// are subsequences which start from a region entry and the incoming
// edge is a fallthrough region entry edge.
//
// There is a two level priority scheme. The high priority subsequences
// are those which start with a region entry with a fall-through or not)
//

void region_sort_subregions_wrt_layout(Compound_region* r)
{
   region_sort_immediate_subregions_wrt_layout(r) ;
   for (Region_subregions sri(r) ; sri != 0 ; sri++) {
      Region* cur_subregion = *sri ;
      if (cur_subregion->is_compound()) {
	 region_sort_subregions_wrt_layout((Compound_region*)cur_subregion) ;
      }
   }
}

void region_sort_immediate_subregions_wrt_layout(Compound_region* r)
{
   // The folling is the list of regions one can pick from to
   // start a linear subsequence. When a linear sequence ends,
   // we can pick any region from this list to start a new
   // linear subsequence. 
   List<Region*> no_in_falltru_regions ;
   
   // The following list is the entry points to a region where
   // the region entry edge is a fallthrough edge. These
   // are the high priority entry points to a region in the layout
   List<Region*> falltru_region_entries ;
   Region_entry_ops ent_opi ;

   // The following code will construct a list of  no_in_falltru_regions
   // A region may have more than one fall-through entry points,
   for (Region_subregions sri(r) ; sri != 0 ; sri++) {
      Region* cur_subreg = *sri ;
      if (cur_subreg->is_op()) {
	 Op* cur_op = (Op*) cur_subreg ;
	 Edge* ft_edge = get_incoming_CONTROL0_fall_through_edge(cur_op) ;
	 if (ft_edge == NULL) {
	    no_in_falltru_regions.add_tail(cur_subreg) ;
	 }
      }
      else {
	 int ft_ent_count = 0 ;
	 for (ent_opi(cur_subreg) ; ent_opi != 0 ; ent_opi++) {
	    Op* cur_op = *ent_opi ;
	    Edge* ft_edge = get_incoming_CONTROL0_fall_through_edge(cur_op) ;
	    if (ft_edge == NULL) {
	       ft_ent_count++ ;
	       // if region has multiple entries with no fall through edges
	       // then use the first entry to sort this subregion
	       if(ft_ent_count == 1) {
		  no_in_falltru_regions.add_tail(cur_subreg) ;
	       }
	    }
	 }
	 //
	 // I don't know if this warning is warranted (Sadun Anik 6/26/97)
	 // If this warning goes off, the code will use only the first of the
	 // entry points in layout 
	 //
	 if (ft_ent_count > 1) {
	    El_warn("Region %i has more than one fall-through entry. \n Disable this warning message if this is OK", cur_subreg->id()) ;
	 }
      }
   }

   //
   // When laying out the region, we will give priority to the region
   // entries, that have a fall through edge coming in from outside
   // the region. If they have a fall through edge coming in from
   // inside the region, e.g. a trace side entry, they are not treated
   // specially.
   //

   for (ent_opi(r) ; ent_opi!= 0 ; ent_opi++) {
      Op* cur_op = *ent_opi ;
      Region* cur_entry_region ;
      cur_entry_region = cur_op ;
      while (cur_entry_region->parent() != r) {
	 cur_entry_region = cur_entry_region->parent() ;
	 if (!cur_entry_region) {
	    El_punt("Region hierarchy error: Region %i's entry op %i is not in region", r->id(), cur_op->id()) ;
	 }
      }
      Edge* ft_edge = get_incoming_CONTROL0_fall_through_edge(cur_op) ;
      if (ft_edge != NULL) {
         // Find out if this is a region entry edge if so cur_entry_region
	 // is a high priority starting point. If not then this entry is
	 // not important, e.g. trace side entry
	 if (r->is_entry_edge(ft_edge)) {
	    falltru_region_entries.add_tail(cur_entry_region) ;
	 }
      }
      else {
	 // cur_entry_region must be in the no_in_falltru_regions. Give
	 // priority to this region over the others by moving it to
	 // the head of the list. (This can be procedure or other region
	 // entry node which should be the first node in the Dlist order
	 // due to Impact constraints)
	 assert(no_in_falltru_regions.is_member(cur_entry_region)) ;
	 no_in_falltru_regions.remove(cur_entry_region) ;
	 no_in_falltru_regions.add_head(cur_entry_region) ;
      }
   }

   List<Region*>& linear_sequence_starts =  falltru_region_entries ;
   for (List_iterator<Region*> li( no_in_falltru_regions) ; li!=0 ; li++) {
       linear_sequence_starts.add_tail(*li) ;
   }

   // Now sort the Dlist of region by following fallthrough edges and using
   // the entries in  linear_sequence_starts as starting points
   bool first_subregion_done = false ;
   Region* last_subregion = NULL ;
   while (!linear_sequence_starts.is_empty()) {
     Region* cur_node = linear_sequence_starts.pop() ;

     while (cur_node != NULL) {
       bool skip = (cur_node->is_op() &&
		    (is_switch((Op*)cur_node) || is_merge((Op*)cur_node)));
       if( ! skip) {
	 if (!first_subregion_done) {
	   if (cur_node != r->first_subregion()) {

	     // Relops don't occur in the successor lists, so if we're
	     // moving an op that has relops they all need to be moved,
	     // too.
	     List<Region*> to_move;
	     if(cur_node->is_op()) {
	       for(List_iterator<Op*> li(((Op*)cur_node)->get_relops()); li != 0; li++) {
		 Op* rel_op = *li;
		 to_move.add_tail( (Region*) rel_op);
	       }
	     }

	     r->remove_region(cur_node) ;
	     r->insert_before_region(cur_node,r->first_subregion()) ;

	     for(List_iterator<Region*> li(to_move); li != 0; li++) {
	       Region* move_me = *li;
	       r->remove_region(move_me) ;
	       if( is_control_switch((Op*)cur_node) ) {
		 r->insert_before_region(move_me,cur_node) ;
	       } else {
		 r->insert_after_region(move_me,cur_node) ;
	       }
	     }
	   }
	   first_subregion_done = true ;
	 }
	 else {
	   if (!(r->succeeds_in_subregion_list(last_subregion, cur_node))) {

	     // Relops don't occur in the successor lists, so if we're
	     // moving an op that has relops they all need to be moved,
	     // too.
	     List<Region*> to_move;
	     if(cur_node->is_op()) {
	       for(List_iterator<Op*> li(((Op*)cur_node)->get_relops()); li != 0; li++) {
		 Op* rel_op = *li;
		 to_move.add_tail( (Region*) rel_op);
	       }
	     }

	     r->remove_region(cur_node) ;
	     r->insert_after_region(cur_node,last_subregion) ;

	     for(List_iterator<Region*> li(to_move); li != 0; li++) {
	       Region* move_me = *li;
	       r->remove_region(move_me) ;
	       if( is_control_switch((Op*)cur_node) ) {
		 r->insert_before_region(move_me,cur_node) ;
	       } else {
		 r->insert_after_region(move_me,cur_node) ;
	       }
	     }
	   }
	 }
       }
       last_subregion = cur_node ;
       cur_node = get_region_fallthrough_succ_with_same_parent(last_subregion) ;
     }
   }
}

Region* get_region_fallthrough_succ_with_same_parent(Region* r)
{
   Region* succ = NULL ;
   Compound_region* parent_r = r->parent() ;
   if (r->is_op()) {
      Edge* succ_edge = get_outgoing_CONTROL0_fall_through_edge((Op*)r);
      if (succ_edge) {
	 Region* succ_reg = succ_edge->dest() ;
	 while (succ_reg && (succ_reg->parent() != parent_r)) {
	    succ_reg = succ_reg->parent() ;
	 }
	 if (succ_reg) {
	    succ = succ_reg ;
	 }
      }
   }
   else {
      // If region has multiple fall-through exits we return the first
      // successor whose parent is the same as that of r
      for (Region_exit_ops rexi(r) ; rexi != 0 ; rexi++) {
	 Op* cur_exit_op = *rexi ;
	 Edge* succ_edge = get_outgoing_CONTROL0_fall_through_edge(cur_exit_op);
	 // skip this op has no fallthrough successors
	 if (!succ_edge) continue ;
	 
	 Region* succ_reg = succ_edge->dest() ;
	 while (succ_reg && (succ_reg->parent() != parent_r)) {
	    succ_reg = succ_reg->parent() ;
	 }
	 // if the fallthrough successor is still in the region, skip
	 if (succ_reg == r) continue ;
	 if (succ_reg) {
	    succ = succ_reg ;
	    break ; // no need to iterate over rest of region exit ops
	 }
      }
   }
   return (succ ) ;
}
