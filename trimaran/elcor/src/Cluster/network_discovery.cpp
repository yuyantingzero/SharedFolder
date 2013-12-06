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
//      File:           network_discovery.cpp
//      Author:         Mike Chu
//      Created:        Oct 2004
//      Description:    Discover machine interconnection network
//                      configuration for clustering
//
/////////////////////////////////////////////////////////////////////////////

#include "cluster.h"
#include "el_init.h"

// find all-pairs shortest paths
void Cluster_mgr::find_min_latencies(Matrix<int> &network, Hash_map<int, int> &regf_id_to_cluster_map) {
  int N = network.dim1();

  Matrix<int> path(N, N, -1);

  int i,j,k;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      min_latency[i][j] = network[i][j];
    }
    min_latency[i][i] = 0;
  }
  for (k = 0; k < N; k++) {
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
	if (min_latency[i][k] == ELCOR_MAX_INT) continue;
	if (min_latency[k][j] == ELCOR_MAX_INT) continue;

	if (min_latency[i][k] + min_latency[k][j] < min_latency[i][j]) {
              min_latency[i][j] = min_latency[i][k] + min_latency[k][j];
	      path[i][j] = k;
    	} 
      } 
    } 
  }

  if (dbg(clst, 2)) {
    cdbg << "NETWORK: " << endl;
    cdbg << "\t";
    for (int j=0; j<min_latency.dim2(); j++)
      cdbg << "rf "<<j<<"\t";
    cdbg << endl;
    
    for (i=0; i<min_latency.dim1(); i++) {
      cdbg << "rf "<<i<<":\t";
      for (int j=0; j<min_latency.dim2(); j++) {
	if(min_latency[i][j] < ELCOR_MAX_INT)
	  cdbg << min_latency[i][j] << "\t";
	else
	  cdbg << "-\t";
      }
      cdbg << endl;
    }
  }

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      if(min_latency[i][j] == ELCOR_MAX_INT) continue;
      if(min_latency[i][j] == 0) continue;
      List<int> *list = new List<int>;
      find_min_paths(i, j, path, list, regf_id_to_cluster_map);
      // Fank wants me to output last cluster I visit for now
      list->add_tail(regf_id_to_cluster_map.value(j));
      min_path[i][j] = list;
    }
  }

  if (dbg(clst, 2)) {
    for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
	List<int> *list = min_path[i][j];
	if(list == NULL) continue;
	
	cdbg << "shortest path from " << i << " to " << j << ": ";
	
	for(List_iterator<int> lsi(*list); lsi!=0; lsi++) {
	  int node = *lsi;
	  cdbg << node << " ";
	}
	cdbg << endl;
      }
    }
  }

}

void Cluster_mgr::find_min_paths(int u, int v, Matrix<int> &paths, 
				 List<int> *list, Hash_map<int, int> &regf_id_to_cluster_map) {
  int k;

  k = paths[u][v];
  if (k == -1) return;
  find_min_paths(u, k, paths, list, regf_id_to_cluster_map);
  list->add_tail(regf_id_to_cluster_map.value(k));
  find_min_paths(k, v, paths, list, regf_id_to_cluster_map);
}

void Cluster_mgr::discover_network()
{
   int i;
   Matrix<int>            network;
   Hash_map<int, int>     regf_id_to_cluster_map(hash_int, HASH_SIZE_1);

   if (network_discovered)
     return;

   // figure out how many register files there are
   int num_files = 0;
   for (unsigned index = 0; index < hw_cluster_vec.size(); index++) {
     HW_cluster* hwc = hw_cluster_vec[index];
     num_files += hwc->get_num_files();

     Hash_set<Reg_descr*> descr_set = hwc->get_reg_descr_set();
     for(Hash_set_iterator<Reg_descr*> regf_i(descr_set); regf_i != 0; regf_i++) {
       Reg_descr* regf = *regf_i;
       if (dbg(clst, 2)) 
	 cdbg << "binding " << regf->get_io_id() << " to file: " << index << endl;      
       regf_id_to_cluster_map.bind(regf->get_io_id(), index);
     }
   }

   // resize the network matrix
   network.resize(num_files,num_files,ELCOR_MAX_INT);
   min_latency.resize(num_files,num_files,-1);
   min_path.resize(num_files,num_files, NULL);

   // figure out the network configuration
   for (Hash_set_iterator<eString> hsi(ic_move_opcodes);
	hsi != 0; hsi++) {
     eString es = (*hsi);

     if (dbg(clst, 2)) 
       cdbg << "Opcode is " << es << endl;

     List_set<Io_descr*> io_desc_set;
     MDES_collect_io_descrs(es, io_desc_set);

     for (List_set_iterator<Io_descr*> lsi(io_desc_set); lsi != 0; lsi++) {
       Io_descr* iod = *lsi;

       List_set<Reg_descr*> src_reg_set;
       List_set<Reg_descr*> dest_reg_set;
       List_set_iterator<Reg_descr*> regi;

       for (i = 1; i <= iod->get_num_in(); i++)
	 iod->collect_reg_descrs(DATA_IN, i, src_reg_set);
       for (i = 1; i <= iod->get_num_out(); i++)
	 iod->collect_reg_descrs(DATA_OUT, i, dest_reg_set);

       for (regi(src_reg_set); regi != 0; regi++) {	 
	 if ((*regi)->get_regkind() == REG_PHYSICAL) {
	   Reg_descr* regf_src = *regi;

	   if (dbg(clst, 2))
	     cdbg << "s io id: " << regf_src->get_io_id() << endl;

	   for (regi(dest_reg_set); regi != 0; regi++) {
	     if ((*regi)->get_regkind() == REG_PHYSICAL) {
	       Reg_descr* regf_dest = *regi;
	       if (dbg(clst, 2))
		 cdbg << "d io id: " << regf_dest->get_io_id() << endl;
	      
	       List_set<Alt_descr*> alt_desc_set;
	       MDES_collect_alt_descrs(es, alt_desc_set);
	       int latency = alt_desc_set.head()->get_lat()->get_flow_lat(DATA_OUT, DEST1);

	       if (dbg(clst, 2))
		 cdbg << "latency: " << latency << endl;

	       network[regf_src->get_io_id()][regf_dest->get_io_id()] = latency;
	     }
	   }

	 }
       }

     }
   }

   if (dbg(clst, 2)) {
     cdbg << "\t";
     for (int j=0; j<network.dim2(); j++)
       cdbg << "rf "<<j<<"\t";
     cdbg << endl;
     
     for (i=0; i<network.dim1(); i++) {
       cdbg << "rf "<<i<<":\t";
       for (int j=0; j<network.dim2(); j++) {   
	 if(network[i][j] < ELCOR_MAX_INT)
	   cdbg << network[i][j] << "\t";
	 else
	   cdbg << "-\t";
       }
       cdbg << endl;
     }
   }
   
   find_min_latencies(network, regf_id_to_cluster_map);

   network_discovered = true;
}
