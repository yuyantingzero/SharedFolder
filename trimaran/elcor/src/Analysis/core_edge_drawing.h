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
//      File:         core_edge_drawing.h
//      Authors:      Richard Johnson  
//      Created:      March 1995
//      Description:  Predicate-cognizant data dependence edge drawing 
//                    for regions.
//
///////////////////////////////////////////////////////////////////////////

#ifndef _CORE_EDGE_DRAWING_H
#define _CORE_EDGE_DRAWING_H

#include "op.h"
#include "hash_set.h"

// Forwarde declarations
class Control_cpr_info ;

/*
 *	Scalar scheduling modes (SAM 10-96)
 */
enum El_Scalar_Sched_Phase {
    EL_SCALAR_SCHED_UNKNOWN=0,
    EL_SCALAR_SCHED_PREPASS,
    EL_SCALAR_SCHED_POSTPASS
};


enum Edge_style {
  NO_EDGES                  = 0x0000,

  // basic edge drawing / latency flags
  ES_SCALAR                 = 0x0001,
  ES_MELD                   = 0x0002,
  ES_MODULO                 = 0x0004,
  ES_CPR                    = 0x0008,
  ES_PREPASS                = 0x0010,
  ES_POSTPASS               = 0x0020,
  ES_GLOBAL                 = 0x0040,

  // the following are depricated flags
  SCALAR_PREPASS_EDGES      = ES_SCALAR | ES_PREPASS,
  SCALAR_POSTPASS_EDGES     = ES_SCALAR | ES_POSTPASS,
  MELD_EDGES                = ES_MELD,
  MODULO_EDGES              = ES_MODULO,
  GLOBAL_ANALYSIS_EDGES     = ES_GLOBAL
};

// flag to indicate which edges get drawn
// Uses:
//   - SCALAR and MELD edge drawing supresses ALL intra-op edges,
//     including edges from a branch to its related switch ops.
//
//   - MODULO edge drawing threads branches but omits full C1 edges.
//
extern Edge_style edge_style ;

// add data merge to agregate; merge has operand of associated VR
void add_merge_to_control_merge(Op* ctl_mg, Op* new_mg) ;

void add_switch_to_control_switch(Op* ctl_sw, Op* new_sw) ;

void insert_block_jsr_sw_nodes (Compound_region* blk) ;

void insert_block_sw_mg_nodes_scalar (Compound_region* blk) ;

void insert_block_sw_mg_nodes_modulo (Compound_region* blk, Hash_set<Operand>* s) ;

void insert_block_sw_mg_nodes_meld (Compound_region* blk) ;

void insert_block_sw_mg_nodes_global (Compound_region* blk) ;

void insert_region_sw_mg_nodes (Compound_region* r, Hash_set<Operand>* s = NULL) ;

void delete_region_sw_mg_nodes (Compound_region* r) ;

bool is_intra_op_edge(Op* src_op, Op* dst_op) ;

void set_edge_latency(Edge* ed, Edge_style es) ;

void control_dep_edge(Op *src_op, Op *dst_op) ;

void mem_edge(Op *src_op, Op *dst_op, Alt_num src_alt = 0, Alt_num dest_alt = 0) ;

void reg_flow_edge(Op* src_op, Port_num src_port, Op* dst_op, Port_num dst_port, Alt_num src_alt = 0, Alt_num dest_alt = 0) ;

void reg_anti_edge(Op* src_op, Port_num src_port, Op* dst_op, Port_num dst_port, Alt_num src_alt = 0, Alt_num dest_alt = 0) ;

void reg_out_edge(Op* src_op, Port_num src_port, Op* dst_op, Port_num dst_port,Alt_num src_alt = 0, Alt_num dest_alt = 0) ;

void insert_intrablock_control_edges(Compound_region* blk) ;

bool is_disjoint_cpr_hack(Op *op1, Op *op2, Control_cpr_info *info) ;

void insert_intrablock_control_edges_scott(Compound_region* blk) ;

void insert_intrablock_edges (Compound_region* blk) ;

void insert_intrablock_edges_gen2 (Compound_region* blk) ;

void insert_region_intrablock_edges (Compound_region* r) ;

void insert_region_intrablock_edges_gen2 (Compound_region* r) ;

void insert_interblock_edges (Edge* e) ;

void insert_region_interblock_edges (Compound_region* r, Compound_region* root) ;

void get_region_vr_set (Compound_region* r, Hash_set<Operand>* s) ;

void insert_region_scalar_edges (Compound_region* r, El_Scalar_Sched_Phase phase);

void insert_region_meld_edges (Compound_region* r) ;

void insert_region_modulo_edges (Compound_region* r) ;

void insert_region_global_edges (Compound_region* r) ;

void delete_region_edges (Compound_region* r) ;

void delete_op_edges (Op *op) ;

void add_additional_edges(Compound_region* r) ;

int adjust_edge_latency_scalar(Edge* curr_edge) ;
int adjust_edge_latency_modulo(Edge* curr_edge) ;

void update_edges_for_op_chaining(Compound_region *r);
void incr_edge_latencies_along_rejected_opchains(Compound_region *r);

#endif
