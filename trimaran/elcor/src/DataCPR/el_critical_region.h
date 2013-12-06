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
//      File:           el_critical_region.h
//      Authors:        Suren Talla
//      Created:        July 1997
//      Description:    Class holds info about the region edges which are
//                      on some critical path of the region. 
//
///////////////////////////////////////////////////////////////////////////

#ifndef _EL_CRITICAL_REGION_H_
#define _EL_CRITICAL_REGION_H_


#include "defs.h"
#include "op.h"
#include "edge.h"
#include "region.h"
#include "hash_map.h"
#include "hash_functions.h"
#include "dep_len.h"


#define DISPLAY_MAX_FREQ_CR_EDGES     0
#define DISPLAY_ALL_CR_EDGES          1
#define DISPLAY_MAX_CR_EDGES          2
#define DISPLAY_ALL_EDGES             3


//###############################################################

class Critical_region_info {

  public:
    Critical_region_info();
    ~Critical_region_info();

    double  critical_path_length(Op* exitop);
    double  max_critical_path_length(void);

    List<Op*>*    get_critical_ops(Op* exitop, List<Op*>& l);
    List<Op*>*    get_all_critical_ops(List<Op*>& l);

    List<Edge*>*  get_critical_edges(Op* exitop, List<Edge*>& l);
    List<Edge*>*  get_all_critical_edges(List<Edge*>& l);
    List<Edge*>*  get_anti_critical_edges(Op* exitop, List<Edge*>& l);
    List<Edge*>*  get_all_anti_critical_edges(List<Edge*>& l);
    List<Edge*>*  get_output_critical_edges(Op* exitop, List<Edge*>& l);
    List<Edge*>*  get_all_output_critical_edges(List<Edge*>& l);
    List<Edge*>*  get_flow_critical_edges(Op* exitop, List<Edge*>& l);
    List<Edge*>*  get_all_flow_critical_edges(List<Edge*>& l);
    
    int  compute_critical_region_info(Compound_region*);
    void delete_critical_region_info(Compound_region*);
    void display_critical_region(Region* rgn, int cregion_type, 
				 char* proc_name=0, int bg=false,
				 int file_index=0);

    friend ostream& operator<<(ostream& os, Critical_region_info& cinfo);

    
    //------------------------------------------------------------;
    //map from each exit op to all the edges on critical path;
    //to that exit op from the start op;
    //------------------------------------------------------------;
    Hash_map<Op*, List<Edge*>*>     critical_edges;
    Hash_map<Op*, List<Op*>*>       critical_ops;
    Hash_map<Op*, DepLen_OpInfo*>   dep_info; 
    Hash_map<Op*, int>              exit_ops_map;
    int                             num_exit_ops;
    Vector<Op*>                     rev_exit_ops;


  private:

    int is_edge_critical(Edge* e, Op* exit_op);
    int is_node_critical(Op* op, Op* exit_op);
    Op* most_freq_exit_op(Region* r);
    Op* exitop_with_longest_cp(Region* r);

};

//--------------------------------------------------------------;
//filterator for critical edges of a region;
//--------------------------------------------------------------;



#endif // _EL_CRITICAL_REGION_H_
