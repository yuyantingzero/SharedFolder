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
//      File:           op.h
//      Authors:        Vinod Kathail, Sadun Anik, Richard Johnson,
//                      Santosh Abraham, Scott Mahlke, Dave August,
//                      Brian Deitrich, Matthai Philipose
//      Created:        January 1994
//      Description:    Op class
//
/////////////////////////////////////////////////////////////////////////////

/* ---------------------------------------------------------------------------
 Operation and derived classes.
 At present, Op class is used for all operation.
   Later, it will become the pure base for all derived classes.
 Source operands are numbered 0 .. n.
 Source 0 is the guarding predicate, if any
 Destination operands are numbered 1 .. m; there is no destination 0.
 For these, use corresponding port synonyms defined in el_port.h
---------------------------------------------------------------------------- */

#ifndef _OPERATION_
#define _OPERATION_

#include <iostream>
#include "defs.h"
#include "opcode.h"
#include "hash_set.h"
#include "el_port.h"
#include "vector.h"
#include "region.h"
#include "operand.h"
#include "spatial_ref.h"

using namespace std;

class Edge; // Forward declaration
class Op_inedges;
class Op_outedges;
class Topo_sort;
class IR_outstream;

//extern int op_id_num;


class Op : public Region {
   friend ostream& operator<<(ostream& os, const Op& op);
   friend IR_outstream &operator<<(IR_outstream &out, const Op &op);
   friend void name_edges(IR_outstream &, Op *op);
   friend void print_edges(IR_outstream &, Op *op);
   friend void name_attributes(IR_outstream &, Op *op);
   friend void print_attributes(IR_outstream &, Op *op);
   friend class Op_inedges;
   friend class Op_inedges_rel;
   friend class Op_outedges;
   friend class Op_outedges_rel;
   friend class Topo_sort;
public:
   Op(Procedure *scope = NULL);
   Op(const Opcode&, Procedure *scope = NULL);
   Op(const Opcode&, int id, Procedure *scope = NULL);
   Op(int id, Procedure *scope = NULL);
   Op(const Opcode& opcode, int sources, int destinations, 
      bool predicated = true, Procedure *scope = NULL); 
   Op(const Op&);
   virtual Op* clone() const;
   virtual ~Op();

   Op& operator=(const Op&);
   virtual bool operator==(const Op&) const;
   bool operator!=(const Op&) const;

   bool is_op() const;
   bool is_pseudo_op();

   int id() const;
   int num_srcs() const;
   int num_dests() const;
   int num_mem_srcs() const;
   int num_mem_dests() const;
   int num_mem_kills() const;
   bool predicated() const;

   Operand& dest(Port_num) const;
   Operand& src(Port_num) const;
   Hash_set<Operand>& mem_srcs();
   Hash_set<Operand>& mem_dests();
   Hash_set<Operand>& mem_kills();
 
   Port_num first_input() const;
   Port_num last_input() const;
   Port_num first_output() const;
   Port_num last_output() const ;
   Port_num pred() const;
   Port_num first_src() const;
   Port_num last_src() const ;
   Port_num first_dest() const;
   Port_num last_dest() const ;
   Opcode opcode() const;
   void or_opcode(Opcode);
   void set_opcode(Opcode);
   void update_num_operands();
  
   char* custom_opcode() const;

   char* sched_opcode() const;
   int sched_time() const;
   bool is_speculative() const;

   int relative_dest_latency(int);

   void set_id(int) ;
   void renumber(Procedure *scope = NULL);

   Op& set_dest(Port_num, const Operand&);
   Op& set_src(Port_num, const Operand&);

   void set_src_spatial_ref(Port_num pn, Spatial_ref sr);
   void set_dest_spatial_ref(Port_num pn, Spatial_ref sr);

   Op& add_mem_src(Operand&);
   Op& add_mem_src_unsafe(Operand&);      
   Op& remove_mem_src(Operand&);
   Op& add_mem_dest(Operand&);
   Op& remove_mem_dest(Operand&);   
   Op& add_mem_kill(Operand&);
   Op& remove_mem_kill(Operand&);   

   void set_custom_opcode(char *opc);

   void set_sched_opcode(char* opc);
   void set_sched_time(int scheduled_time);
   void set_speculative(bool val = true);

   Op& add_relops(Op*);
   Op& remove_relops(Op*);
   Op& remove_all_relops();
   List<Op*>& get_relops();

   Op& add_inedge(Edge* edge, Port_type type, Port_num num, Alt_num alt = 0);
   Op& add_outedge(Edge* edge, Port_type type, Port_num num, Alt_num alt = 0);
   Op& remove_inedge(Edge* edge, Port_type type, Port_num num, Alt_num alt = 0);
   Op& remove_outedge(Edge* edge, Port_type type, Port_num num, 
		      Alt_num alt = 0);
   
   Op& add_inop_recursively(Edge* edge);
   Op& add_outop_recursively(Edge* edge);
   Op& remove_inop_recursively(Edge* edge);
   Op& remove_outop_recursively(Edge* edge);
  
   Op& add_inedge_recursively_restricted(Edge* edge);
   Op& add_outedge_recursively_restricted(Edge* edge);
   Op& add_inedge_recursively(Edge* edge);
   Op& add_outedge_recursively(Edge* edge);
   Op& remove_inedge_recursively(Edge* edge);
   Op& remove_outedge_recursively(Edge* edge);

   Op* prev() ;
   Op* next() ;

   int flow_time(Port_type type, Port_num num, Port_dir dir = IN); 
   int anti_time(Port_type type, Port_num num, Port_dir dir = IN);
   void build_io_pat(char *iopat);



protected:
   Dlist<Region*>& subregions();
   Hash_set<Op*>& inops();
   Hash_set<Op*>& outops();
   List<Edge*>& inedges();
   List<Edge*>& outedges();
   Region* copy_with_edges(Region_map& region_map, Edge_map& edge_map);
   void print(ostream&) const ;
private:
   bool check_port(Port_type, Port_num);

   int id_rep;

   Opcode opcode_rep;
   // src_num is number of actual sources excluding guarding predicate
   int src_num;
   int dest_num;
   bool is_predicated;

   int sched_time_rep;
   char* sched_opcode_rep;

   char *custom_opcode_rep;

   Vector<Operand>* srcs;
   Vector<Operand>* dests;
   Hash_set<Operand>* mem_out;
   Hash_set<Operand>* mem_in;
   Hash_set<Operand>* mem_kill;


   List<Op*>* related_ops;

   List<Edge*> edges_in;
   List<Edge*> edges_out;
};

#endif



