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
//      File:           machine_discovery.cpp
//      Author:         Kevin Fan, Mike Chu
//      Created:        May 2003
//      Description:    Discover machine configuration for clustering
//
/////////////////////////////////////////////////////////////////////////////

#include "cluster.h"
#include "el_init.h"

// Rajiv: called when cluster_method=none
void Cluster_mgr::discover_machine_bogus()
{
   Hash_map<eString, eString> v_to_a_map(hash_estring, 8);

   if (machine_discovered)
      return;

   if (dbg(status, 1))
      cdbg << "Creating bogus cluster..." << endl;

   hw_cluster_vec.resize(1);
   HW_cluster* hwc = new HW_cluster;
   hw_cluster_vec[0] = hwc;

   int num_files = MDES_num_reg_types();
   for (int i = 0; i < num_files; i++) {
      Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
      char *vname = rd->get_vname();
      char *aname = rd->get_aname();

      if (rd->get_regkind() == REG_PHYSICAL) {
         hwc->add_reg_file(rd);

         // Make sure that when setting cluster_method=none, the mdes is really
         // a single cluster machine
         eString vn(vname);
         eString an(aname);
         if (v_to_a_map.is_bound(vn))
           El_punt("discover_machine_bogus: trying to add physical file: %s, physical file: %s exists for vname: %s..seems like a multi-clustered machine with cluster_method=none turned on\n", (char*)an, (char*)v_to_a_map.value(vn), (char*)vn);
         else
           v_to_a_map.bind(vn, an);
      }
   }

   // place_opcodes
   for (Hash_map_iterator<Opcode, eString> hmi(el_opcode_to_string_map);
        hmi != 0; hmi++) {

      Opcode opc = (*hmi).first;
      eString es = (*hmi).second;

      // check if it exists in Mdes
      if (current_MDES->get_MDES_hashtable()->find_op(es) == NULL)
         continue;

      // skip no-ops, pseudos, moves
      if (is_no_op(opc) || is_pseudo(opc))
         continue;

      hwc->add_supported_gen_opcode(opc);
   }

   // make physical_file to cluster map

   Hash_set<Reg_descr*>& rd_set = hw_cluster_vec[0]->get_reg_descr_set();
   for (Hash_set_iterator<Reg_descr*> rdi(rd_set); rdi != 0; rdi++) {
     Reg_descr* rd = *rdi;
     physical_file_to_cluster_map.bind(rd->get_aname(), 0);
   }

   place_macros();
   place_move_opcodes();
   
   List<HW_cluster*> hw_clusters;
   hw_clusters.push(hwc);
   place_us_opcodes(hw_clusters);

   machine_discovered = true;

   if (dbg(clst, 2))
      print_machine();
}

void Cluster_mgr::discover_machine()
{
   if (El_cluster_method == EL_CLSTR_NONE) {
     discover_machine_bogus();
     return;
   }
   
   if (machine_discovered)
     return;

   if (dbg(status, 1))
      cdbg << "Gathering machine info" << endl;

   List<HW_cluster*> hw_clusters;

   int num_files = MDES_num_reg_types();

   // make a bunch of clusters, one for each register file
   for (int i = 0; i < num_files; i++) {
      Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);

      if (rd->get_regkind() == REG_PHYSICAL) {
	 HW_cluster* hwc = new HW_cluster;
      	 hwc->add_reg_file(rd);
	 hw_clusters.add_tail(hwc);
      }
   }

   // place opcodes on appropriate clusters, merging as needed
   place_opcodes(hw_clusters);

   hw_cluster_vec.resize(hw_clusters.size());
   int cid = 0;
   for (List_iterator<HW_cluster*> ci(hw_clusters); ci != 0; ci++)
      hw_cluster_vec[cid++] = *ci;

   // make physical_file to cluster map
   for (int i = 0; i < (int)hw_cluster_vec.size(); i++) {
     Hash_set<Reg_descr*>& rd_set = hw_cluster_vec[i]->get_reg_descr_set();
     for (Hash_set_iterator<Reg_descr*> rdi(rd_set); rdi != 0; rdi++) {
       Reg_descr* rd = *rdi;
       physical_file_to_cluster_map.bind(rd->get_aname(), i);
     }
   }

   // now do macro regs and moves
   place_macros();
   place_move_opcodes();
   place_us_opcodes(hw_clusters);

   // hack to for brl and brf's to go on the same cluster as retaddr and LC/ESC, respectively
   remove_bad_brl_and_brf();

   machine_discovered = true;

   if (dbg(clst, 2))
      print_machine();
}

void Cluster_mgr::place_opcodes(List<HW_cluster*>& hw_clusters)
{
   // go through opcodes to find io patterns (ignoring moves)
   for (Hash_map_iterator<Opcode, eString> hmi(el_opcode_to_string_map);
	hmi != 0; hmi++) {

      Opcode opc = (*hmi).first;
      eString es = (*hmi).second;

      // check if it exists in Mdes
      if (current_MDES->get_MDES_hashtable()->find_op(es) == NULL)
	 continue;

      // skip no-ops, pseudos, moves
      if (is_no_op(opc) || is_pseudo(opc) || is_move(opc) || is_custom(opc))
	 continue;

      List_set<Io_descr*> io_desc_set;
      MDES_collect_io_descrs(es, io_desc_set);

      for (List_set_iterator<Io_descr*> lsi(io_desc_set); lsi != 0; lsi++) {
	 List_set<Reg_descr*> reg_desc_set;
	 Hash_set<Reg_descr*> cur_io_reg_set(hash_Reg_descr_ptr, HASH_SIZE_0);
	 Hash_set<HW_cluster*> to_merge(hash_hw_cluster_ptr, HASH_SIZE_0);
	 int i;
	 Io_descr* iod = *lsi;

	 if (iod->is_predicated())
	    iod->collect_reg_descrs(DATA_IN, 0, reg_desc_set);
	 for (i = 1; i <= iod->get_num_in(); i++)
	    iod->collect_reg_descrs(DATA_IN, i, reg_desc_set);
	 for (i = 1; i <= iod->get_num_out(); i++)
	    iod->collect_reg_descrs(DATA_OUT, i, reg_desc_set);

	 // convert to hash_set
	 for (List_set_iterator<Reg_descr*> regi(reg_desc_set);
	      regi != 0; regi++) {
	    if ((*regi)->get_regkind() == REG_PHYSICAL)
	       cur_io_reg_set += *regi;
	 }

	 // get a set of clusters that this io pattern references
	 for (List_iterator<HW_cluster*> hwci(hw_clusters);
	      hwci != 0; hwci++) {
	    HW_cluster* current_hwc = *hwci;
	    Hash_set<Reg_descr*> hwc_regs = 
	       current_hwc->get_reg_descr_set();
	    
	    // find intersect
	    hwc_regs *= cur_io_reg_set;
	    
	    if (!hwc_regs.is_empty())
	       to_merge += current_hwc;
	 }

	 Hash_set<HW_cluster*> to_delete = merge_or_share(to_merge, cur_io_reg_set);

	 // do actual deleting
         for (Hash_set_iterator<HW_cluster*> del_iter(to_delete); del_iter!=0; del_iter++) {
           hw_clusters.remove(*del_iter);
	   delete *del_iter;
         }

	 // find which of the new clusters supports this iopattern
	 for (List_iterator<HW_cluster*> new_hwci(hw_clusters);
	      new_hwci != 0; new_hwci++) {
	    HW_cluster* new_hwc = *new_hwci;
	    Hash_set<Reg_descr*> new_hwc_regs = new_hwc->get_reg_descr_set();
	    
	    if (new_hwc_regs.is_subset(cur_io_reg_set)) {
              new_hwc->add_supported_gen_opcode(opc);		  
	    }
	      
	 }
      }
   }
   //hack for custom op, added by hongtaoz
   for (List_iterator<HW_cluster*> new_hwci(hw_clusters);
		   new_hwci != 0; new_hwci++) {
	   HW_cluster* new_hwc = *new_hwci;
	   new_hwc->add_supported_gen_opcode(CUSTOM);		  

   }
}

void Cluster_mgr::place_us_opcodes(List<HW_cluster*>& hw_clusters) 
{
  Hash_set<eString> all_moves(hash_estring, HASH_SIZE_1);

  for (List_iterator<HW_cluster*> hwci(hw_clusters); hwci!=0; hwci++) {
    HW_cluster * hw_clust = *hwci;
  
    Hash_set<Reg_descr*> hwc_regs =  hw_clust->get_reg_descr_set();  
    Hash_set<Opcode>& hwc_gen_ops = hw_clust->get_supported_gen_opcodes();

    for (Hash_set_iterator<Opcode> hsi(hwc_gen_ops); hsi != 0; hsi++) {
      
      Opcode opc = (*hsi);

      // If this isn't here and you don't have custom ops in the mdes,
      // then this function will punt. ntclark 1/2/03
      if((opc == CUSTOM) && ! (El_do_prepass_custom_ops || 
			       El_do_postpass_custom_ops))
	continue;

      eString es = el_opcode_to_string_map.value(*hsi);


      List_set<Op_descr*> op_desc_set;
      MDES_collect_op_descrs(es, op_desc_set);

      for (List_set_iterator<Op_descr*> lsi(op_desc_set); lsi != 0; lsi++) {
	List_set<Reg_descr*> reg_desc_set;
	Hash_set<Reg_descr*> cur_opd_reg_set(hash_Reg_descr_ptr, HASH_SIZE_0);
	Op_descr* opd = *lsi;
	int i;
	char * aname = opd->get_aname_ptr();
       
	if (opd->is_predicated())
	  current_MDES->collect_reg_descrs(opd, DATA_IN, 0, reg_desc_set);
	for (i = 1; i <= opd->get_num_in(); i++)
	  current_MDES->collect_reg_descrs(opd, DATA_IN, i, reg_desc_set);
	for (i = 1; i <= opd->get_num_out(); i++)
	  current_MDES->collect_reg_descrs(opd, DATA_OUT, i, reg_desc_set);

	// convert to hash_set
	for (List_set_iterator<Reg_descr*> regi(reg_desc_set);
	     regi != 0; regi++) {
	  Reg_descr* cur_reg_desc = *regi;
	  if (cur_reg_desc->get_regkind() == REG_PHYSICAL)
	    cur_opd_reg_set += cur_reg_desc;
	}       

	if(hwc_regs.is_subset(cur_opd_reg_set)) {
	  eString temp(aname);
	  hw_clust->add_supported_us_opcode(temp);		  
	  if(is_move(opc))
	    all_moves += temp;
	}
        // do not collect ic_move_opcodes here, because any ic_move_opcodes that
        // do not also belong to a cluster will not be found. (for example, if a
        // machine has inter-cluster but no intra-cluster MOVEGG.) ic_move_opcode
        // discovery moved to place_move_opcodes(). -KF 8/2004
// 	else if(is_move(opc)) {
// 	  eString temp(aname);
// 	  ic_move_opcodes += temp;
// 	}
      }

    }

    // Now handle the custom ops
    if(El_do_prepass_custom_ops || El_do_postpass_custom_ops) {
      unsigned cop_size = current_MDES->get_num_custom_ops();
      for (unsigned j = 0; j < cop_size; j++) {
	Custom_op_descr* cop_descr = current_MDES->get_MDES_custom_op_descr(j);
	eString es(cop_descr->get_opcode());

	List_set<Op_descr*> op_desc_set;
	MDES_collect_op_descrs(es, op_desc_set);

	for (List_set_iterator<Op_descr*> lsi(op_desc_set); lsi != 0; lsi++) {
	  List_set<Reg_descr*> reg_desc_set;
	  Hash_set<Reg_descr*> cur_opd_reg_set(hash_Reg_descr_ptr, HASH_SIZE_0);
	  Op_descr* opd = *lsi;
	  int i;
	  char * aname = opd->get_aname_ptr();
       
	  if (opd->is_predicated())
	    current_MDES->collect_reg_descrs(opd, DATA_IN, 0, reg_desc_set);
	  for (i = 1; i <= opd->get_num_in(); i++)
	    current_MDES->collect_reg_descrs(opd, DATA_IN, i, reg_desc_set);
	  for (i = 1; i <= opd->get_num_out(); i++)
	    current_MDES->collect_reg_descrs(opd, DATA_OUT, i, reg_desc_set);

	  // convert to hash_set
	  for (List_set_iterator<Reg_descr*> regi(reg_desc_set);
	       regi != 0; regi++) {
	    Reg_descr* cur_reg_desc = *regi;
	    if (cur_reg_desc->get_regkind() == REG_PHYSICAL)
	      cur_opd_reg_set += cur_reg_desc;
	  }       

	  if(hwc_regs.is_subset(cur_opd_reg_set)) {
	    eString temp(aname);
	    hw_clust->add_supported_us_opcode(temp);
	  }

	}
      }
    }
  
  }
//  ic_move_opcodes -= all_moves;
}

Hash_set<HW_cluster*> Cluster_mgr::merge_or_share(Hash_set<HW_cluster*>& to_merge,
						  Hash_set<Reg_descr*>& rds)
{
   Hash_set<HW_cluster*> to_delete(hash_hw_cluster_ptr, HASH_SIZE_0); 

   // check if we need to do anything
   if (find_valid_hw_cluster(to_merge, rds) != NULL)
      return to_delete;

   for (Hash_set_iterator<HW_cluster*> hwci(to_merge); hwci!=0; hwci++) {
      HW_cluster* outer_hwc = *hwci;
      if (to_delete.is_member(outer_hwc)) continue; // skip deleted

      // return if we're done, don't need the other check below
      if (outer_hwc->get_reg_descr_set().is_subset(rds))
	 return to_delete;

      for (Hash_set_iterator<HW_cluster*> iter(to_merge); iter != 0; iter++) {
	 HW_cluster* inner_hwc = *iter;
	 if (inner_hwc == outer_hwc) continue; // skip this one
	 if (to_delete.is_member(inner_hwc)) continue; // skip deleted

	 Hash_set<Reg_descr*> interesting_outer_rds(outer_hwc->get_reg_descr_set());
	 Hash_set<Reg_descr*> interesting_inner_rds(inner_hwc->get_reg_descr_set());
         interesting_inner_rds *= rds;
         interesting_outer_rds *= rds;

	 // skip if outer won't get anything useful from inner
	 if (interesting_outer_rds.is_subset(interesting_inner_rds))
	    continue;
      
	 // see if merging is possible
	 if (outer_hwc->is_mergeable_with(*inner_hwc)) {
	    outer_hwc->merge_with(*inner_hwc);
	    to_delete += inner_hwc;
	 }
	 else {
	    Hash_set<Reg_descr*> regs = interesting_inner_rds;
	    regs -= interesting_outer_rds;
	    // regs = set of rf's on inner_hwc that outer_hwc needs copies of
	    for (Hash_set_iterator<Reg_descr*> riter(regs); riter!=0; riter++) {
	       Reg_descr* r = *riter;
	       bool can_copy = true;
	       char* vtypes = r->get_vname();
	       // make sure outer_hwc doesn't have any rf's that implement the
	       // virtual types of r
	       for (int i=0; i<(int)strlen(vtypes); i++) {
		  if (outer_hwc->get_physical_file(mdesname_to_vregfile(vtypes[i])) != NULL)
		     can_copy = false;
	       }
	       if (can_copy) {
		  if (dbg(clst, 2))
		     cdbg << "******** putting " << r->get_aname() << " with cluster " << *outer_hwc;
		  outer_hwc->add_reg_file(r);
		  interesting_outer_rds += r;
	       }
	       // else, do nothing. when inner_hwc becomes outer_hwc in
	       // a different outer iteration, the corresponding 
	       // complementary rf copy should be attempted.
	    }
	 }

	 // break if we're done
	 if (outer_hwc->get_reg_descr_set().is_subset(rds))
	    break;
      }
   }

   // check that we now have at least one cluster that has all regs in rds
   bool ok = false;
   for (Hash_set_iterator<HW_cluster*> ci(to_merge); ci!=0; ci++) {
      HW_cluster* c = *ci;
      if (to_delete.is_member(c)) continue;
      if (c->get_reg_descr_set().is_subset(rds))
	 ok = true;
   }
   if (!ok) {
      for (Hash_set_iterator<Reg_descr*> ri(rds); ri!=0; ri++)
	 cdbg << " " << (*ri)->get_aname();
      cdbg << endl;
      El_punt("machine discovery failed to find a valid cluster supporting all of the above reg files");
   }

   return to_delete;
}   

HW_cluster* Cluster_mgr::find_valid_hw_cluster(Hash_set<HW_cluster*>& clusters,
					       Hash_set<Reg_descr*>& rds)
{
   for (Hash_set_iterator<HW_cluster*> hwci(clusters); hwci != 0; hwci++) {
      Hash_set<Reg_descr*>& hwc_rds = (*hwci)->get_reg_descr_set();
      if (hwc_rds.is_subset(rds))
	 return *hwci;
   }
   return NULL;
}

// keep track of which moves are valid on which clusters.
// this code iterated over Io_descrs before. i changed it to iterate over
// Op_descrs so that we can add the (unit-specific) ic_move_opcodes here.
// (see comment in place_us_opcodes().) -KF 8/2004
void Cluster_mgr::place_move_opcodes()
{
  for (Hash_map_iterator<Opcode, eString> hmi(el_opcode_to_string_map);
       hmi != 0; hmi++) {

    Pair<Opcode, eString> opc = *hmi;
    if (is_move(opc.first) &&
        current_MDES->get_MDES_hashtable()->find_op(opc.second) != NULL) {

      List_set<Op_descr*> op_desc_set;
      MDES_collect_op_descrs(opc.second, op_desc_set);

      for (List_set_iterator<Op_descr*> lsi(op_desc_set); lsi != 0; lsi++) {
        Op_descr* opd = *lsi;
        List_set<Reg_descr*> reg_desc_set;
        Hash_set<Reg_descr*> cur_opd_reg_set(hash_Reg_descr_ptr, HASH_SIZE_0);
        int i;

        if (opd->is_predicated())
          current_MDES->collect_reg_descrs(opd, DATA_IN, 0, reg_desc_set);
        for (i = 1; i <= opd->get_num_in(); i++)
          current_MDES->collect_reg_descrs(opd, DATA_IN, i, reg_desc_set);
        for (i = 1; i <= opd->get_num_out(); i++)
          current_MDES->collect_reg_descrs(opd, DATA_OUT, i, reg_desc_set);
      
        // convert to hash_set, physical regs only
        // (so literals will be ok)
        for (List_set_iterator<Reg_descr*> regi(reg_desc_set);
             regi != 0; regi++) {
          if ((*regi)->get_regkind() == REG_PHYSICAL)
            cur_opd_reg_set += *regi;
        }
	 
        for (unsigned c = 0; c < hw_cluster_vec.size(); c++) {
          HW_cluster* current_hwc = hw_cluster_vec[c];
          Hash_set<Reg_descr*> hwc_regs =
            current_hwc->get_reg_descr_set();
	    
          // find intersect
          hwc_regs *= cur_opd_reg_set;
	    
          if (hwc_regs.is_empty())
            continue;
          else if (hwc_regs == cur_opd_reg_set) {
            // add current opcode to this cluster
            current_hwc->add_supported_gen_opcode(opc.first);
          } else {
            // intercluster move
            eString move_string(opd->get_aname_ptr());
            ic_move_opcodes += move_string;
          }
        }
      }
    }
  }
}

void Cluster_mgr::place_macros()
{
  // Fill in Macro_info Vector

  // for each cluster
  for(unsigned i = 0; i < hw_cluster_vec.size(); i++) {
    HW_cluster* clust = hw_cluster_vec[i];
    Hash_set<Reg_descr*>& rd_set = clust->get_reg_descr_set();

    // for each register file within a cluster
    for(Hash_set_iterator<Reg_descr *> hsi(rd_set); hsi != 0; hsi++) {
      Reg_descr* cur_rd = *hsi;
      Hash_set<eString> macros(hash_estring, 8);
      cur_rd->get_macros(macros);

      // for each macro in the register file
      for(Hash_set_iterator<eString> hsi2(macros); hsi2!= 0; hsi2++) {
	eString cur_macro = *hsi2;
	if (dbg(clst, 2))
	  cdbg << "Placing macro: " << cur_macro << " in cluster " << i << endl;
	int mn = el_string_to_macro_map.value(cur_macro);

	eString pf(cur_rd->get_aname());
	macro_vec[mn].add_physical_file(pf);	

	if(macro_vec[mn].get_potential_homes_size() >= 1) {
	  macro_vec[mn].reset_current_home();
	  macro_vec[mn].set_multiple_copy();
	}
	else
	  macro_vec[mn].set_current_home(i);

	// add this potential home
	if (dbg(clst, 2))
	  cdbg << "adding potential home for: " << mn << endl;
	macro_vec[mn].add_potential_home(i);

	if (dbg(clst, 2))
	  cdbg << endl;

      }
    }
  }
}

//
// temporary hack
// currently, clustering possibly puts BRLs on a different cluster than the RETURN_ADDR
// also, BRFs get put on different clusters from LC and ESC
// 
// the following function removes BRL and BRF's from clusters that can't support them
//
// added mchu -- 1.08.03
void Cluster_mgr::remove_bad_brl_and_brf()
{
  // for each cluster
  for(unsigned i = 0; i < hw_cluster_vec.size(); i++) {
    HW_cluster* clust = hw_cluster_vec[i];

    if(!macro_vec[RETURN_ADDR].is_potential_home(i))
      clust->remove_supported_gen_opcode(BRL);

    if(!macro_vec[LC_REG].is_potential_home(i) || !macro_vec[ESC_REG].is_potential_home(i))
      clust->remove_supported_gen_opcode(BRF_B_B_F);
  }
}

// checks to make sure each macro defined in the prgraom is also defined in the mdes
void Cluster_mgr::check_macros(Compound_region* block)
{
  for(Region_all_ops opi(block); opi!=0; opi++) {
    Op* op = *opi;
    if(is_pseudo(op))
      continue;
    for(Op_explicit_inputs in_i(op); in_i!=0; in_i++) {
      Operand oper = *in_i;
      if(oper.is_macro_reg()) {
	Macro_name mn = oper.name();
	
	if(macro_vec[mn].get_potential_homes_size() <= 0)
	  El_warn("check_macro: macro %s not defined in mdes", (char*) el_macro_to_string_map.value(mn));
      }	
    }
    for(Op_explicit_dests out_i(op); out_i!=0; out_i++) {
      Operand oper = *out_i;
      if(oper.is_macro_reg()) {
	Macro_name mn = oper.name();
	
	if(macro_vec[mn].get_potential_homes_size() <= 0)
	  El_warn("check_macro: macro %s not defined in mdes", (char*) el_macro_to_string_map.value(mn));
      }	
    }
    // add in Op_explicit_outputs
      
  }
}
