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
//                                                                       //
//      File:           codegen.h                                        //
//      Authors:        Rajiv Ravindran                                  //
//      Created:        April 2002                                       //
//      Description:    Codegen class                                    //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#ifndef _CODEGEN_H
#define _CODEGEN_H

#define INVALID_OPDES -999999
#define EMPTY_PRF_SET -999

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "region.h"
#include "op.h"
#include "operand.h"
#include "edge.h"
#include "edge_attributes.h"
#include "el_control.h"
#include "mdes.h"
#include "intf.h"
#include "op_attributes.h"
#include "opcode_properties.h"
#include "dtype.h"
#include "ref.h"
#include "pred_analysis.h"
#include "flow_analysis_solver.h"
#include "reaching_defs_solver.h"
#include "ir_writer.h"
#include "dbg.h"
#include "string_class.h"
#include "dlist.h"
#include "hash_set.h"
#include "list_set.h"
#include "slist.h"
#include "tuples.h"
#include "iterators.h"
#include "register_file.h"
#include "cluster.h"
#include "hw_cluster.h"
#include "el_codegen_init.h"
#include "el_opti_utilities.h"
#include "el_opti_dead_code.h"
#include "el_available_expression.h"
#include "connect.h"
#include "region_attributes.h"
#include "el_normalize_branches.h"
#include "opcode_load_store.h"
#include "regfile_bind.h"
#include "el_bb_tools.h"
// needed for the regalloc command line switch
#include "el_init.h"
#include "el_data.h"
#include "el_expression.h"
#include "live_range_construction.h"
#include "live_range.h"

typedef List<Op*> (*FuncImpl)(Op *);

using namespace std;

class Codegen {

public:

    static bool init;

    // codegen move op statistics
    static int move_ops;
    static int two_oprnd_move_ops;
    static int prf_move_ops;
    static int bind_prf_move_ops;

    // Initialized from mdes in method mdes_regfiles_init
    // Creates a width sorted list of literal register files
    static Slist<Stuple<int, char *> > Litfile_List;

    // Initialized from mdes in method mdes_regfiles_init
    // Creates a list of regfile name and its corresponding width
    static List<Pair<char *, int> > Prf_Width_List;

    static List<Pair<char *, int> > Prf_reg_size_list;

    static Hash_map<int, Hash_set<eString> > Vr_Home_Set;
    static Hash_map<Macro_name, Hash_set<eString> > Macro_Home_Set;
    static Hash_map<int, eString> Vr_Home_Prf;

    // Register file binding class
    static PRF_Bind_Map prf_bind_map;

    // Map containing literals to labels in the data section
    // where they are to be inserted
    static Hash_map<Operand, eString> operand_to_label_map;

    // Machine specific opcode conversion
    static Hash_map<Opcode, FuncImpl> *opcTable;

public:

    static void do_initial_codegen(Procedure *f);
    static void annotate_physical_files(Compound_region *r);
    static Pair<int, char *> get_lit_operand_prf(Operand& oprnd);
    static void annotate_physical_file_op(Op *op, List_set<Op_descr *>& uspc_ops);
    static void get_mdes_prf_set(IO_Portkind portkind, int port_num, char *uspc_op_name, 
                                 char *vrf, Hash_set<Reg_descr*>& reg_descr_set);
    static void get_mdes_prf_set_all(Op *op, Port_num port, IO_Portkind portkind,
                                     char* uspc_op, Hash_set<eString>& prf_set);
    static void get_macro_home_prf(Macro_name mn, Hash_set<eString>& prf_set);
    static void annotate_prf_port(IO_Portkind port_kind, Port_num port_num, Operand& oprnd, 
                                   Physfile_binding_attr *prf_battr, 
                                   List_set<Op_descr *>& uspc_ops, Op *op);
    static void post_cluster_prune_prf_bindings(Compound_region *r, Cluster_mgr& cmgr);
    static void get_uspc_ops_in_cluster(Op *op, int cid, List_set<Op_descr*>& clust_uspc_ops);
    static void do_physfile_move_codegen(Compound_region *r, bool post_cluster);
    inline static unsigned hash_macro_name(Macro_name& i) { return (ELCOR_INT_HASH(i)); }
    static void mdes_regfiles_init();
    static void check_files_bound(Compound_region *r);
    static void do_prepass_post_cluster_codegen(Compound_region *r, Cluster_mgr& cmgr);
    static void postpass_codegen_initialize(Procedure *f);
    static void post_regalloc_codegen_initialize(Procedure *f);
    static void update_prf_home(Compound_region *r);
    static void do_two_oprnd_convert(Op *op, List_set<Op_descr *>& uspc_ops, bool post_cluster);
    static void do_postpass_post_cluster_codegen(Compound_region *r, Cluster_mgr& cmgr);
    static void print_home_sets();

    static bool local_copy_propagate(Compound_region *r, Cluster_mgr& cmgr, bool post_cluster, bool postpass);
    static bool global_copy_propagate(Compound_region *f);
    static bool global_constant_cse_elimination(Compound_region *f);
    static bool global_constant_cse_elimination2(Compound_region *f);
    static void init_codegen_homeset(Operand& oprnd, eString prf);
    static bool prf_bind_replace(Op *op, Operand& oprnd, IO_Portkind port_kind, 
                                 Port_num port_num, List_set<Op_descr*>& uspc_ops);
    static bool check_replace_prf_bind_empty(Op *op, Operand& oprnd, IO_Portkind port_kind,
                                 Port_num port_num, List_set<Op_descr*>& uspc_ops);
    static void prune_invalid_prf_choice(Compound_region *r, Cluster_mgr& mgr);
    static void set_address_data_bit_width(Procedure *f);
    static void prune_prf_based_on_width(Operand& oprnd, Hash_set<eString>& prf_set, Op *op);
    static void check_for_valid_opdes(Compound_region *r, Cluster_mgr* cmgr, bool post_cluster);
    static Op_descr *check_for_valid_opdes_op(Op *op, List_set<Op_descr*>& uspc_ops, bool post_cluster);
    static int get_opdes_cost(Op *op, Op_descr *opdes, Hash_map<int, Pair<eString, int> >& vr_preferred_prf);
    static int get_prf_width(eString prf);
    static int get_reg_file_size(char *regfile);
    static void print_move_stats();
   
    static void do_move_codegen(Compound_region *r);
    static void do_post_cluster_move_codegen(Compound_region *r, Cluster_mgr& cmgr);
    static void insert_move_ops(Op *op, List_set<Op_descr*>& uspc_ops, bool post_cluster);
    static void do_two_operand(Compound_region *r);
    static void do_two_operand_op(Op *op, Reaching_defs_info *rdindo, List_set<Op_descr*>& unit_specific_opdes_set, bool post_cluster);
    static void bind_physical_files(Compound_region *r, Cluster_mgr& cmgr);
    static void insert_post_bind_prf_moves(Compound_region *r);
    static bool is_two_oprnd_compatible(Op *op, Operand& src_oprnd, 
                                        Hash_set<eString>& src_prf_set);
    static Pair<eString, int> get_best_ref_prf(Hash_set<El_ref>& ref_set); 
    static bool in_two_operand_format(Op *op);
    static void do_actual_move_insertion(Op *op, List_set<Op_descr*>& uspc_ops, bool post_cluster);
    static void get_ref_set_for_region(Compound_region *r, 
                                  Hash_map<int, Hash_set<El_ref> >& vr_ref_set,
                                  Hash_map<Macro_name, Hash_set<El_ref> >& macro_ref_set);
    static int compute_estimated_moves(Op *op, List_set<Op_descr*>& uspc_ops);
    static bool check_for_valid_bind(Op *op, Cluster_mgr& mgr);
    static bool all_oprnd_bound(Op *op);
    static Operand *get_new_operand(Operand& oprnd);
    static Op *clone_op(Op *);
    
    // Loop cache related stuff
    static void Split_regions_at_BRLs(Procedure *f); 
    static void El_form_basic_blocks_with_BRLs(Procedure *f);

    // Create new data section to hold the newly created labels
    // generated during move_codegen
    static void createDataSection(IR_outstream& outs);

   static void create_stack_frame(Procedure *f, Cluster_mgr& cmgr);  
   static void create_stack_frame_prepass(Procedure *f);

   // Machine specific opcode conversion
   static void MachineSpecificConversion(Procedure* f);
   static void OpConvert(Procedure *f);
   static void initialize_opc_table(Hash_map<Opcode, FuncImpl> **opcTable);

   static void fix_op_formats(Compound_region* f, bool prepass);
};

#define CGEN_HASH_SIZE_1	64
#define CGEN_HASH_SIZE_2	1024

#endif
