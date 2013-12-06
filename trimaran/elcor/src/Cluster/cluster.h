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
//      File:           cluster.h
//      Author:         Kevin Fan
//      Created:        June 2001
//      Description:    Perform cluster assignment and move insertion
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CLUSTER_H
#define _CLUSTER_H

#define HASH_SIZE_0  16
#define HASH_SIZE_1  512
#define HASH_SIZE_2  1024
#define HASH_SIZE_3  4096
#define HASH_SIZE_4  8192

#include "el_cluster_init.h"
#include "el_port.h"
#include "operand.h"
#include "hash_set.h"
#include "hash_map.h"
#include "list.h"
#include "matrix.h"
#include "opcode.h"
#include "macro_info.h"

class El_ref;
class Compound_region;
class Procedure;
class Op;
class Operand;
class Reg_descr;
class HW_cluster;
class Reaching_defs_info;
class Cluster_mgr;

//////////////////////////////////////////////////////////////////////
//
// Cluster_algorithm: This is a base class for clustering algorithms.

class Cluster_algorithm {

private:

   // disabled constructors
   Cluster_algorithm();
   Cluster_algorithm(const Cluster_algorithm&);
   void operator= (const Cluster_algorithm&);

public:

   // constructor / destructor
   Cluster_algorithm(Cluster_mgr*);
   virtual ~Cluster_algorithm();

   virtual void static_init();

   // main driver
   virtual void do_block_setup_and_clustering(Compound_region*) = 0;

   // utility
   virtual void append_io(Operand& opr, char* iopat, bool is_dest, int clstr, Op* op);
   virtual void build_io(char* iopat, Op* op, int clstr);
   
protected:

   Cluster_mgr* cmgr;
   static bool is_init;
};

//////////////////////////////////////////////////////////////////////
// Cluster manager
//
// Cluster_mgr: This class handles assignment of operations
//   to clusters and operands to clusters, insertion and optimization
//   of inter-cluster move operations, and binding of operands to
//   physical files.

class Cluster_mgr {

friend class Single_cluster;
friend class BUG_cluster;
friend class MultiLevel;
friend class RHOP;
friend class Random_cluster;

public:
   // constructor / destructor
   Cluster_mgr(Procedure*);
   ~Cluster_mgr();

   // prepass clustering
   void do_block_clustering(Compound_region*, Cluster_method cm);

   // A function to reset the state of the region, so the clustering
   // can be redone.
   void undo_block_clustering(Compound_region*);

   // common to both prepass and postpass clustering
   void install_cluster_id_attr(Compound_region *r);

   // postpass clustering - see postpass.cpp
   void do_proc_postpass_clustering(Cluster_method cm);
   void do_block_postpass_clustering(Compound_region* r, Cluster_method cm);
   void do_block_postpass_move_insertion(Compound_region* r, Cluster_method cm);

   // utility
   int    get_cluster(Op*);
   int    get_cluster(Operand&);
   int    get_cluster_for_physical_file(eString);
   char*  get_physical_file_for_operand(Operand);
   Opcode get_icmove_opcode(Operand& oprnd, bool is_source);
   Hash_set<eString>& get_ic_move_opcodes();
   Hash_set<eString>& get_physical_files_for_macro(Macro_name);

   int    get_staging_pred_cid(Compound_region *kernel);

   // The following should ideally not be public, but are because of
   // limitations of the system. (i.e. they are a big hack.)

   // no longer used. -KF 12/2004
   // void allocated_register_fixup(Compound_region*, Cluster_method cm);

   static Vector<HW_cluster*>    hw_cluster_vec;
   static Hash_map<Op*, int>     op_map; // static for msched
   static Hash_map<int, int>     operand_map;
   static Vector<Macro_info>     macro_vec;

   void insert_block_moves(Compound_region*);
private:   
   // disabled constructors
   Cluster_mgr();
   Cluster_mgr(const Cluster_mgr&);
   void operator= (const Cluster_mgr&);

   // machine discovery - see machine_discovery.cpp
   void discover_machine();
   void discover_machine_bogus();
   void place_opcodes(List<HW_cluster*>&);
   void place_us_opcodes(List<HW_cluster*>&);
   void place_macros();
   void check_macros(Compound_region* block);
   void place_move_opcodes();
   void remove_bad_brl_and_brf();
   Hash_set<HW_cluster*> merge_or_share(Hash_set<HW_cluster*>&, Hash_set<Reg_descr*>&);
   HW_cluster* find_valid_hw_cluster(Hash_set<HW_cluster*>&, Hash_set<Reg_descr*>&);

   // interconnection network discovery - see network_discovery.cpp
   void find_min_latencies(Matrix<int> &, Hash_map<int, int> &);
   void find_min_paths(int, int, Matrix<int> &, List<int> *, Hash_map<int, int> &);
   void discover_network();

   // clustering
   void do_block_operand_assignment(Compound_region*);
   void do_block_smarter_operand_assignment(Compound_region*);

   // move insertion
   //hongtaoz make insert_block_move public
   //void insert_block_moves(Compound_region*);
   void get_src_moves_for_operand(Op*, Port_num, int, int, List<Op*>&);
   void get_dest_moves_for_operand(Op*, Port_num, int, int, List<Op*>&);
   Operand get_staging_predicate(Compound_region *kernel);
   Op*  get_pred_clear_op(Operand);
   Op*  get_pred_set_op(Operand);

   // move optimization - see move_opti.cpp
   void do_proc_move_opti();
   int  do_block_move_opti(Compound_region*);
   int  do_block_move_opti_1(Compound_region*, Reaching_defs_info*);
   int  do_block_move_opti_2(Compound_region*, Reaching_defs_info*);
   int  do_block_move_opti_3(Compound_region*, Reaching_defs_info*);
   void check_moves(Compound_region*);
   bool same_contents(List<El_ref>&, List<El_ref>&);

   // post-scalarreg clustering - see postpass.cpp
   void do_block_bogus_postpass_clustering(Compound_region *r);
   void do_block_stack_mod_postpass_clustering(Compound_region* r, Cluster_method cm);   
   void do_block_bd_postpass_clustering(Compound_region* r, Cluster_method cm);   
   Op*  get_postpass_move_op(Operand& dest, Operand& src, Operand& pred);
   Op* insert_postpass_pv_moves(Op*, Port_type, Port_num, int, int, List<Op*>&);
   void get_postpass_moves(Op*, Port_type, Port_num, int, int, List<Op*>&);
   int add_predicate_moves(Compound_region *, Op*, int);

   // utility
   void collect_valid_clusters_op(Op*, List<int>*&);
   void collect_valid_clusters_operand(Reg_file, int, List<int>&);

   // debug output
   void print_machine();
   void print_clusters();
   void print_op_clusters();
   void print_operand_clusters();
   void print_operand_freq();
   void print_macro_vec();

   Procedure*             m_proc;
   
   Cluster_algorithm*     cluster_alg;

   Hash_map<Op*, double>  weight_map;
   Matrix<double>         operand_freq;  
   bool                   freqs_found;

   Hash_map<Op*, List<int>*> op_valid_cluster_map;

   static Hash_map<eString, int> physical_file_to_cluster_map;
   static Hash_set<eString>      ic_move_opcodes;
   
   static int moves_inserted;

   static Matrix<int>            min_latency;
   static Matrix< List<int>* >   min_path;

   // Macro to be used during postpass clustering
   // Could be RGS_REG or SP_REG depending on El_create_stack_frame
   static Macro_name spill_macro;
   
   // This is used for certain op types (CMPPs, SVMOVEs) where we force all
   // operations that write to the same dest register to be assigned to the same
   // cluster.
   Hash_map<Operand, int> pre_assign_map;
};

#endif
