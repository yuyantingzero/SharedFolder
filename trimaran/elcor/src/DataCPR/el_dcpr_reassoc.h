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




///////////////////////////////////////////////////////////////////////////////
//
//      File:           el_dcpr_reassoc.h
//      Authors:        Suren Talla
//      Created:        August 1997
//      Description:    Reassoc interface (a data CPR opti)
//
//      Copyright (C) 1996 Hewlett-Packard Company 
//      
///////////////////////////////////////////////////////////////////////////////



#ifndef _EL_DCPR_REASSOC_
#define _EL_DCPR_REASSOC_

#include <iostream>

#include "ref.h"
#include "bit_vector.h"
#include "hash_map.h"
#include "list.h"
#include "op.h"
#include "edge.h"
#include "dep_len.h"
#include "region.h"
#include "reaching_defs_solver.h"

using namespace std;

#define MAX_CHAIN_LENGTH        64

#define SCALAR_CHAIN            0
#define VECTOR_CHAIN            1




//#####################################################################

typedef struct reassoc_term_t {
    El_ref   useref;
    El_ref   defref;
    int      estart;
} Reassoc_term;


//#####################################################################
class Flow_chain {

  public:
    Flow_chain(Opcode opcode_type);
    ~Flow_chain();

    int insert(Edge* e);
    int append(Edge* e);
    int will_split_chain(Edge*);
    int is_interesting(Edge* e);
    int sub_chain(Edge* e, Flow_chain* fc);
    int follows_edge(Edge*);
    int concat(Flow_chain*);
    int get_unique_ops(List<Op*>& ops_list);
    int is_scalar(Reaching_defs_info* rdefs_info);

    Edge*  tail_edge(){ return chain_edges.tail();};
    Edge*  head_edge(){ return chain_edges.head();};
    Opcode type(void){ return opcode;}
    void   type(Opcode& opc) { opcode = opc;}
    int    id(){ return chain_set_id; }
    void   id(int sid){ chain_set_id = sid;}
    int    is_scalar(void){ return (chain_type==SCALAR_CHAIN); }
    int    size(void){ return chain_edges.size(); }


    //---------------------------------------------------------------------;
    // related to forming expressions and gencode after reassoc;
    // deplen_info needed to pass E-start times;
    //---------------------------------------------------------------------;
    int gen_code(Region*,Reaching_defs_info*,Hash_map<Op*, DepLen_OpInfo*>&);
    int is_reassoc_profitable();   
    int terms_to_ops(Reassoc_term*,Op*,int,int, List<Op*>&);
    int insert_new_ops(Region*, List<Op*>& new_ops, Op* result_op); 
   
    int get_sub_chains(Reaching_defs_info* rdefs_info,List<Flow_chain*>& subchains);

    int delete_chain_ops();
    int mark_for_deletion(void){ to_delete = true; return to_delete; };
    int marked_for_deletion(void){ return to_delete; }

    friend ostream& operator<<(ostream&, Flow_chain&);


  private:

    /* currently opcode types of interest ::
       ADD_W, MPY_W, SUB_W, ADDL_W, MPYL_W, SUBL_W 
       FADD_S, FADD_D, FMPY_S, FMPY_D, FSUB_S, FSUB_D
       each in its own class (chain) */
    Opcode    opcode;

    /* also need to keep track of the predicate */
    int   pred_vr_num;
    int   is_predicated;

    /* if only the final value of the chain is used 
       then this is a scalar chain else a Vector chain */
    int   chain_type;

    /* for some reason the chain nodes cannot be deleted until the 
       region edges are deleted. So to remember which chains are to
       be deleted, they are marked */
    int   to_delete;

    int   chain_set_id;

    List<Edge*>             chain_edges;
    Hash_map<Op*, int>      chain_op_map;
    Vector<Op*>             rev_chain_op_map;


};

//#####################################################################

class Reassoc_flow_chains {

  public:

    Reassoc_flow_chains();
    ~Reassoc_flow_chains();

    int  compute_flow_chains(Region* rgn, int flag =0);
    int  compute_flow_chains(List<Edge*>& flow_edges, int flag =0);
    void gen_code(Region*, Reaching_defs_info*, Hash_map<Op*, DepLen_OpInfo*>&);


    int  delete_dead_flow_chains();
    void delete_flow_chains();
    void rename_flow_chains(int these_ids, int to_these_ids);
    void display_flow_chains(ostream& os);
    int  chain_set_size(int id);

    static int new_chain_set_id();


  private:

    List<Flow_chain*>   flow_chains;
 
    int  is_interesting(Edge*);
    int  merge_chains();

    //chains fall into distinct sets - those that have been split by 
    //replication have the same id;
    static int chain_set_id_counter;
};



/*#################################################
ALGO FOR REASSOC::

//form flow chains of desired opcode types 
 - compute critical region and e-start times
 - for flow edges e on some critical path
      - for each chain check if
            - e is eligible
            - e splits chain 
            - insert e and update chains
 - for each chain
     - compute profit due to reassoc
     - if profitable,
          - gen code for reassoc expn
          - insert generated code into region

##################################################*/

#endif  // _EL_DCPR_REASSOC_
