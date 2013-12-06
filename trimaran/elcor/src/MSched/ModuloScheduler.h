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
//      File:           ModuloScheduler.cpp
//      Authors:        Vinod Kathail, Joel Jones, Shail Aditya
//      Created:        May 1994
//      Description:    Software Pipelining class definition
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MODULO_SCHED_
#define _MODULO_SCHED_
#include <iostream>
#include <fstream>
#include "defs.h"
#include "list.h"
#include "region.h"
#include "hash_map.h"
#include "iterators.h"
#include "slist.h"
#include "el_port.h"
#include "matrix.h"
#include "RU.h"
#include "tracware_base.h"
#include "mdes.h"

using namespace std;

//----------------------------SchedInfo---------------------------------------
// Scheduling information for each op

class SchedInfo {
public:
  SchedInfo(Op *op);
  ~SchedInfo();
  Op *op_rep;
  int stime;
  int estart;
  bool placed;
  bool fixed;
  int height;
  char *specific_op;
  int op_priority;
  int res_id;
  bool search_forward;
};

//----------------------------ModuloScheduler----------------------------------
// Main Modulo scheduler class

class Latency_map;
class RR_allocation;
class Tracware;

class ModuloScheduler {
  friend class Tracware;
public:
  // allocate the modulo-scheduling problem.
  // The ii_guess, if specified, should be a valid lower bound
  // ResMII calculation will not return a lower value
  // RecMII calculation may return a lower or higher value
  ModuloScheduler(LoopBody *loop_to_schedule, int ii_guess=0, Tracware* = NULL);
  virtual ~ModuloScheduler();
  virtual bool schedule(int potentialII, int budget_ratio, bool meld_allowed = false,
		bool stretch_Scheduling = false);
  void commit(bool do_sorting = true);
  int ResMII();
  int RecMII();
  int MII();
  int II();
  int ESC();
  int Numnodes();
  void calculate_esc();
  void attach_meld_info(RR_allocation& rr_alloc);
  
  // Useful if the loopbody changes after this is allocated.
  void reinit(int new_ii);

protected:
  List<Edge*>* draw_direct_edges_with_omega(LoopBody* loop);
  void draw_direct_edges_aux(Op* src, int lat, int omega,
			     Edge* initial, Alt_num cf_src_alt, 
			     Alt_num cf_dest_alt,
			     const Edge_filter* efilt,
			     List<Edge*>*  edge_list, bool omega_dep);
  int calculate_ResMII(int guess);
  int calculate_RecMII(int guess);
  bool failFloyd(int II);
  void print_failed_cycles(int II, ostream& os);
  void print_longest_cycles(int II, ostream& os);
  void print_failed_cycles_aux(ostream& os, int fromnode, int tonode);
  void print_cycles(ostream& os);
  void print_longest_paths(ostream& os);
  void print_path(int i, int k, ostream& os);
  bool check_if_schedule(int ii);
  void init_sched_state();
  void place_branch(Op* op, int time);
  void put_sched_info_in_ir();

  virtual void iterative_scheduler();
  virtual void swing_scheduler();
  void markAsScheduled(Op *node, int SchedSlot);
  void markAsUnScheduled(Op *node);
  bool put(Stuple<int, Op *> node, int SchedSlot);
  // Different put function for Swing algorithm, because it doesn't
  // need to be nearly as complex.
  bool swing_put(Stuple<int, Op *> node, int SchedSlot);
  int  CalcHeight(Op *op);
  void CalcASAP(Hash_map<Op*, int>&);
  void CalcALAP(Hash_map<Op*, int>&, int);
  void get_path_ops(int, int, List_set<Op*>&);
  void get_pred_set(Hash_map<Op*, int>& priorities, List_set<Op*>& answer,
		    Hash_map<Op*, int>&);
  void get_succ_set(Hash_map<Op*, int>& priorities, List_set<Op*>& answer,
		    Hash_map<Op*, int>&);
  int swing_estart(Op*, const Hash_map<Op*, int>&);
  int swing_lstart(Op*, const Hash_map<Op*, int>&);
  void CalcSwingPriorities(Hash_map<Op*, int>& answer);

  void sort_ops();
  void update_omegas();
  void connect_and_add_op(Op* current, Op* previous);
  void remove_incoming_c0_edge(Op* op);
  void fix_zero_cycle_problems(List<Op*>& mem_ops, int time);
  void copy_entry_meld_info(RR_allocation& rr_alloc, Latency_map &init_map, 
			     Latency_map *final_map);
  void copy_exit_meld_info(RR_allocation& rr_alloc, Latency_map &init_map, 
			   Latency_map *final_map);
  void delete_local_structures();
  void delete_added_edges_if_necessary();

protected:

// Data members
  int Budget;
  bool commited;
  int the_recmii;
  int the_resmii;
  int the_mii;
  int the_ii;
  int good_ii;
  int the_esc;
  int numnodes;
  bool meld;
  bool stretch;
  Compound_region *kernel;
  LoopBody *loopbody;
  Slist<Stuple<int, Op*> > unscheduledNodes;
  Hash_map<Op*, SchedInfo*> *sInfoMap;
  Filter_and<Edge *> *edge_filter;
  Hash_map<Op*, int> nodeMap;
  Hash_map<int, Op*> revnodeMap;
  Matrix<int> *minDist;
  Matrix<int> *nhbrs;
  RU *RUmap;
  List<Edge *> *added_edges;
  bool delete_added_edges;  // Some derived classes may not want the direct edges
                            // to be deleted.  (I don't think they can be easily
                            // redrawn after scheduling.)  -KF 10/2005

// Hook for Tracware 
    Tracware* Tracker;  
    bool _free_tracware;

// These are needed by the tracware module
public:
    void getResourceConflicts(Op *node, int time, Hash_set<Op*>& ResConfl);
    void getDependencyConflicts(Op *node, int time, Hash_set<Op*>& DepConfl);
    virtual void CalcEstart(Op *node, int *estart);
    virtual bool place(Stuple<int, Op *> node, int SchedSlot, bool tentatitve = false);
    virtual void unplace(Stuple<int, Op *> node, int SchedSlot, bool tentative = false);
    inline RU* rumap() { return RUmap; }
    inline Hash_map<Op*, SchedInfo*>* sinfomap() { return sInfoMap; }
    inline Filter_and<Edge *> *edgefilter() {return edge_filter;}
    void manually_adjust_edges(void);
};

#endif
