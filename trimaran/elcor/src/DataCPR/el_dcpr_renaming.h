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
//      File:           el_dcpr_renaming.h
//      Authors:        Suren Talla
//                      (Complete rewrite of Sumedh's Code)
//      Created:        July 1997
//      Description:    Renaming interface (a data CPR opti)
//
//      Copyright (C) 1996 Hewlett-Packard Company 
//      
///////////////////////////////////////////////////////////////////////////////



#ifndef _EL_DCPR_RENAMING_
#define _EL_DCPR_RENAMING_

#include "ref.h"
#include "bit_vector.h"
#include "reaching_defs_solver.h"



#define INTERNAL_NET   0x0000
#define CROSSING_NET   0x0001
#define LIVE_IN_NET    0x0003
#define LIVE_OUT_NET   0x0005


//the connected component of defs-uses;
//note that this struct maintains the number of defs/uses
//whether they are livein/liveout or are internal - all this 
//info is useful when breaking multi-def/multi-use SCC's 
//or when SCC's cross the region boundaries

class Renaming_ops_net {

  public:
    Renaming_ops_net (void);
    ~Renaming_ops_net (void);
    
    List<Pair<int, int>*>* reflist;
    int net_type;
    int num_internal_defs;
    int num_internal_uses;
    int num_live_outs;
    int num_live_ins;

    friend ostream& operator<<(ostream& os, const Renaming_ops_net& net);

  private:

};




//-------------------------------------------------------------;
class Renaming_data {

  public:

    Renaming_data (Region* r);
    ~Renaming_data (void);
    
    int  do_renaming(Compound_region* rgn, int to_rename_with_copy=false); 
    int  do_renaming(Compound_region* rgn, List<Edge*>* edges_to_break, int to_rename_with_copy=false); 
    int  do_blind_renaming(Compound_region* rgn);


  private:

    //markers for nodes during dfs traversal while constructing CC's;
    Bitvector def_visited;
    Bitvector use_visited;
    
    int total_renamed;
    
    //def, and use maps + reverse maps;
    Hash_map<El_ref,int>   def_to_i_map;
    Vector<El_ref>         rev_def_map;
    
    Hash_map<El_ref,int>   use_to_i_map;
    Vector<El_ref>         rev_use_map;

    // operand_net is all the NETs for this operand:
    // the Vector is indexed by operand_int;
    int                               m_net_num_groups;
    Vector<List<Renaming_ops_net*>*>  m_net;
    Vector<int>                       m_net_vrnum;

    Renaming_data (void);
    void get_net(int def_num, Reaching_defs_info* rdiin, Renaming_ops_net* net);
    int  rename_net(Compound_region* hb, Renaming_ops_net* net, 
		    int is_proc, int to_rename_with_copy);
    int  init_def_use_maps(Reaching_defs_info* rdi);

};



//###################################################################

extern int El_rename_region(Compound_region* rgn);
extern int El_rename_with_copy_region(Compound_region* rgn);
extern int El_rename_with_copy_region(Compound_region* rgn, List<Edge*>* anti_edges_to_break);
extern int El_rename_blindly_region(Procedure* f);



#endif  // _EL_DCPR_RENAMING_
