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
//      File:           reaching_defs_solver.h
//      Authors:        Sadun Anik, Sumedh Sathaye, Richard Johnson
//      Created:        August 1994
//      Description:    Reaching definitons solver interface
//                      Also contains the definition of Reaching_defs_info
//                      Also contains the definition of
//                                   Reaching_defs_info attribute
//
//      Notes:          See the notes in the corresponding .cpp file
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef _REACHING_DEFS_SOLVER
#define _REACHING_DEFS_SOLVER

#include <iostream>
#include "defs.h"
#include "bit_vector.h"
#include "adjlist_graph.h"
#include "hash_map.h"
#include "op.h"
#include "attribute_types.h"
#include "ref.h"
#include "flow_analysis_solver.h"

using namespace std;

class Reaching_defs_info;

// Interface routines:
void El_do_reaching_defs (Region*);
void El_do_reaching_defs (Region*, unsigned int flag);
Reaching_defs_info* get_reaching_defs_info (Region*);
void set_reaching_defs_info (Region*, Reaching_defs_info*);
void remove_reaching_defs_info (Region*);

enum DEFUSE_TYPE {
   DEF_REF,
   USE_REF
};

class Reaching_defs_solver_data {
public:
   Reaching_defs_solver_data (void);
   Reaching_defs_solver_data (int n);
   ~Reaching_defs_solver_data (void);
   
   bool operator== (const Reaching_defs_solver_data&) const;
   
   Bitvector rd_in;
   Bitvector rd_out;
   Bitvector rd_in_initial;
   Bitvector rd_gen;
   Bitvector rd_kill;
   
private:
   
protected:
};


class Reaching_defs_solver ;

/////////////////////////////////////////////////////////////////////////////

class Reaching_defs_maps {
public:
   Reaching_defs_maps(Region*, unsigned int filter_flag = ANALYZE_ALL);
   ~Reaching_defs_maps();
   
   bool is_interesting(Operand&) ;
      
   int operand_counter;		// number of operands in the region
   int def_counter;		// number of definitions in the region
   int use_counter;		// number of uses in the region
   
   Hash_map<El_ref,int>   def_to_i_map;
   Vector<El_ref>         rev_def_map;
   
   Hash_map<El_ref,int>   use_to_i_map;
   Vector<El_ref>         rev_use_map;
   
   Hash_map<Operand,int>  operand_map;
   Vector<Operand>        rev_operand_map;
   
   Hash_map<int,int>      def_to_operand_map; // Def_int to Operand_int
   Vector<List<int> >     operand_to_def_map; // Operand_int to Def_int (list)
   
   Hash_map<int,int>      use_to_operand_map; // Use_int to Operand_int
   Vector<List<int> >     operand_to_use_map; // Operand_int to Use_int (list)
   
protected:
private:
   unsigned int filter_mask ;
};

/////////////////////////////////////////////////////////////////////////////

class Reaching_defs_solver {
public:
   Reaching_defs_solver(Region*, unsigned int filter_flag = ANALYZE_ALL);
   ~Reaching_defs_solver();
   
   void solve_reaching_defs();
   void construct_uddu_chains();
   void clone_uddu_chains(Reaching_defs_info*);

   bool is_interesting(Operand&) ;
   
   Alist_region_tiling_graph graph;
   Vector<Reaching_defs_solver_data> rd_info;
   Reaching_defs_maps* rdm;
   
   Hash_map<El_ref, List<El_ref>* > def_use; // list of du chains
   Hash_map<El_ref, List<El_ref>* > use_def; // list of ud chains
   
private:
   void initialize_live_ins(Region *r);
   void compute_gen_kill_info(int);
   void meet_function(int n, bool& tag);
   void transfer_function(int n, bool& tag);
   void do_chains(Compound_region* blk, int rd_idx);
   void do_remap_transitive_chain_fixup(Compound_region* blk, int rd_idx);

   unsigned int filter_mask ;
};

/////////////////////////////////////////////////////////////////////////////

class Reaching_defs_info {
public:
   Reaching_defs_info(unsigned int filter_flag = ANALYZE_ALL);
   ~Reaching_defs_info();
   
   List<El_ref>* get_du_chain(El_ref& def);
   List<El_ref>* get_ud_chain(El_ref& use);
   
   void remove_use(El_ref& use) ;
   void remove_def(El_ref& def) ;
   
   // These two  mehtods are for updating du or ud chains after
   // instruction duplication. They do complete cloning
   void clone_ud_for_new_use(El_ref& new_use, El_ref& old_use) ;
   void clone_du_for_new_def(El_ref& new_def, El_ref& old_def) ;
   
   // This method is for updating du ud chains after renaming
   // style operations which remove links from chains
   void remove_du_ud_links(El_ref& def, El_ref& use) ;
   void add_du_ud_links(El_ref& def, El_ref& use) ;      
   
   Hash_map<El_ref, List<El_ref>* > def_use; // def <--> du chain
   Hash_map<El_ref, List<El_ref>* > use_def; // use <--> ud chain
      
   bool is_interesting(Operand&) ;
   
private:
   bool is_interesting_ref(El_ref& ref) ;
protected:
   unsigned int filter_mask ;
};

/////////////////////////////////////////////////////////////////////////////

class Reaching_defs_info_attrib : public Base_attribute {
public:
  Reaching_defs_info_attrib();
  ~Reaching_defs_info_attrib();
   
  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return REACHING_DEFS; }

  Reaching_defs_info* info;
};

/////////////////////////////////////////////////////////////////////////////

#endif
