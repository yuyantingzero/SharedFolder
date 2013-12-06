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
//      File:           load_info.h
//      Authors:        Santosh G. Abraham
//      Created:        October 1995
//      Description:    LoadInfo class for managing info on each load op
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _LOAD_INFO_
#define _LOAD_INFO_

#include <stdio.h>

// #include "defs.h"
#include "op.h"
#include "vector.h"

const float MR_GATHER_STATS = 0.0f;
const int NUM_MR_BUCKETS = 6;   // number of buckets used to classify loads
// array bkts_lim contains upper limits for each bucket 0..(NUM_MR_BUCKETS-1)
extern const float bkts_lim[NUM_MR_BUCKETS];

enum DepModels {ALL_DEP=0,      // All dependencies on missing loads honored
		NO_MEM_DEP,   // Ignore mem deps
		NO_BR_MEM_DEP,      // Additionally ignore anti and output
		                // deps from br to other ops
		FLOW_DEP,     // Only honor reg flow deps
		// Add new models above this line
		MAX_MODELS      // Used only to initialize array sizes 
                };

enum ResTypes  {ISSUE_RES=0,    // ops requiring issue resource (real ops)
		INT_RES,	// Ops requiring integer resource unit
		FLOAT_RES,	// Float unit ops
		LOAD_RES,	// Load unit ops
		STORE_RES,	// Store unit ops
		PBR_RES,	// Pbr unit ops
		CMPP_RES,	// Predicate compare ops
		BRANCH_RES,	// Branch ops
		// Add new res types above this line
		MAX_RES		// Used only to initialize array sizes 
                };

class LoadInfo {
  friend ostream& operator << (ostream&, const LoadInfo&);
public:
  LoadInfo(Op* op);
  ~LoadInfo();
  
  Op* get_load_op() const;      // get ptr to load op

  float get_miss_ratio() const; // get cache miss ratio
  int get_exec_freq() const;    // get number of times load executed
  int get_data_miss_count() const;        // get number of cache misses of load

  int get_dep_ops_above(int model) const; // get dep ops above for model 'model'
  int get_dep_ops_below(int model) const; // get dep ops below for model 'model'
  int get_pos_in_block() const;           // get pos of load in block
  int get_num_ops_in_block() const;       // get number of ops in sched region

  int get_etime() const; /* gets dag earliest time */
  int get_nomem_etime() const; /* gets dag earliest time */
  int get_ltime() const; /* gets dag latest time */
  int get_slack() const; /* gets the slack value */
  int get_block_path_len() const; // critical path length of sched region
  int get_entry_res_dist() const;  // resource cycles of ops above
  int get_exit_res_dist() const;        // exit_res_dist
  int get_res_dist_slack() const;        // slack due to resource constraints

  Op* get_last_jsr() const;
  Op* get_prefetch() const;
  Op* get_ldv() const;

  int get_entry_load_dist(int model) const; // for 'model' get entry-load dist
  int get_load_exit_dist (int model) const; // for 'model' get load-exit dist

  int get_presched_lat() const;
  int get_updated_ltime() const;
  int get_final_lat() const;
  int get_sched_lat() const;

  void set_load_op(Op* load_op);

  void set_miss_ratio(float miss_ratio);
  void set_exec_freq(int exec_freq);
  void set_data_miss_count(int data_miss_count);

  void init_dep_ops_above(int num_models = MAX_MODELS, int value = 0);
  void set_dep_ops_above (int model, int value);
  void init_dep_ops_below(int num_models = MAX_MODELS, int value = 0);
  void set_dep_ops_below (int model, int value);
  void set_pos_in_block(int pos_in_block);
  void set_num_ops_in_block(int num_ops_in_block);

  void set_etime(int time);
  void set_nomem_etime(int time);
  void set_ltime(int time);
  void set_slack(int value);
  void set_block_path_len(int crit_path_len);
  void set_entry_res_dist(int dist);
  void set_exit_res_dist(int dist);
  void set_res_dist_slack(int value);

  void set_last_jsr(Op *jsr);
  void set_prefetch(Op *pre);
  void set_ldv(Op *ld);
 
  void init_entry_load_dist(int num_models = MAX_MODELS, int value = 0);
  void set_entry_load_dist (int model, int value);
  void init_load_exit_dist(int num_models = MAX_MODELS, int value = 0);
  void set_load_exit_dist (int model, int value);

  void set_presched_lat(int lat);
  void set_updated_ltime(int ltime);
  void set_final_lat(int lat);
  void set_sched_lat(int lat);

private:
  Op* op_rep;         // operation id

  float miss_ratio;   // miss ratio of load op
  int exec_freq;      // number of times this load executed
  int data_miss_count;        // number of times this load missed

  Vector<int> dep_ops_above;  // for each model, number of dep ops above load
  Vector<int> dep_ops_below;  // for each model, number of dep ops below load
  int pos_in_block;   // original C0 position in block
  int num_ops_in_block;       // number of real ops in region

  int etime;          // earliest time the op can be scheduled (all dependences)
  int nomem_etime;    // earliest time the op can be scheduled (no mem deps)
  int ltime;          // latest time the op can be scheduled (all dependences)
  int slack;          // ltime - slack (ltime wrt last exit)
  int block_path_len; // critical path length through block to last exit
  int entry_res_dist;   // minimum resource cycles for ops trapped above this op
  int exit_res_dist; // minimum resource cycles for ops trapped below
  int res_dist_slack; // slack due to ops trapped in between op and uses

  Op *last_jsr;		// the immediately preceeding jsr in C0 order
  Op *prefetch;		// prefetch associated with this load op
  Op *ldv;		// ldv associated with this lds op
  
  Vector<int> entry_load_dist; // for each model, dist from entry to load
  Vector<int> load_exit_dist;  // for each model, dist from load to exit

  int presched_lat;	// latency set in presched_cacheopt
  int updated_ltime;	// ltime updated just before scheduling load
  int final_lat;	// final latency on load-use edges
  int sched_lat;	// load-use distance in scheduled code
};

ostream& operator << (ostream&, const LoadInfo&);

class LSExitInfo {
  friend ostream& operator << (ostream&, const LSExitInfo&);
public:
  LSExitInfo();
  ~LSExitInfo();

  Op* exit_op;     // exit ops for which info is maintained
  Compound_region* parent_region;	// parent block
  Compound_region* target_region;	// target block

  double exit_freq;          // execution frequecy
  int crit_path_len;           // critical path length to this exit
  Vector<int> res_use;	// number of ops of each type
};

ostream& operator << (ostream&, const LSExitInfo&);

class LSBlockInfo {
  friend ostream& operator << (ostream&, const LSBlockInfo&);
public:
  LSBlockInfo();
  ~LSBlockInfo();

  Compound_region* region;     // region on which info is maintained
  double cb_exec_freq;          // execution frequecy
  int num_ops;                 // number of ops in region
  int num_exits;               // number of exits in region
  int crit_path_len;           // critical path length; last exits etime
  int res_length;             // length of schedule using resource constraints
  int comp_cycles;             // stall-free execution time of this region
  float norm_stall_cycles;	// stall cycles over cache miss penalty
  Vector<int> num_loads_mr;	// number of loads with each miss ratio
  Vector<int> res_use;	// number of ops of each type

  Vector<LSExitInfo*> exitInfoVec;	// Vector of pointers to exitInfo
};

ostream& operator << (ostream&, const LSBlockInfo&);

extern Hash_map<Op*, LoadInfo*>* loadInfoMap;
extern LSBlockInfo* lsBlockInfo;

#endif
