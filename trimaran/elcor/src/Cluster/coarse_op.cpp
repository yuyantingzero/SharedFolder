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



#include "coarse_op.h"

Coarse_op::Coarse_op()  
  : members(hash_op_ptr, 1000),
    in_edges(hash_edge_ptr, 1000),
    out_edges(hash_edge_ptr, 1000),
    valid_clusters(hash_int, 16)
{
  locked = false;
  boundary_weight = 0;
}

void Coarse_op::add_member(Op* op)
{
  members += op;
}

bool Coarse_op::is_member(Op* op)
{
  return(members.is_member(op));
}

void Coarse_op::add_in_edge(float weight, int cop_id, Edge* edge)
{
  in_edges += edge;
  boundary_weight++;
  
  Stuple<float, int> tuple(-weight, cop_id);
  edge_list.add(tuple);    
}

void Coarse_op::add_out_edge(float weight, int cop_id, Edge* edge)
{
  out_edges += edge;
  boundary_weight++;
  
  Stuple<float, int> tuple(-weight, cop_id);
  edge_list.add(tuple);    
}


int Coarse_op::get_highest_weight_edge(Hash_set<int> coarsened, Vector<Coarse_op>& ops)
{
  for (Slist_iterator<Stuple<float, int> > sli(edge_list);
       sli != 0; sli++) {

    Stuple<float, int> head = *sli;
    int cop_id = head.second;

    if(!coarsened.is_member(cop_id)) {
      Hash_set<int> valid_clusts = ops[cop_id].valid_clusters;
      valid_clusts *= valid_clusters;

      if(valid_clusts.size() != 0) 
	return cop_id;
    }
  }
  
  return -1;
}

void Coarse_op::fix_coarse_edges(int cop_id, Hash_map<int, int> *id_map)
{
  Slist< Stuple<float, int> > new_edge_list;

  for (Slist_iterator<Stuple<float, int> > sli(edge_list);
       sli != 0; sli++) {
    Stuple<float, int> tuple = *sli;
    int old_value = tuple.second;
    int new_value = id_map->value(old_value);

    if(new_value == cop_id) 
      continue;

    tuple.second = new_value;
    new_edge_list.add(tuple);
    
  }
  edge_list = new_edge_list;
}

void Coarse_op::fix_boundary_weight(Coarse_op& op1, Coarse_op& op2)
{
  for(Hash_set_iterator<Edge *> hsi(in_edges); hsi!=0; hsi++) {
    Edge * edge = *hsi;
    if(op1.is_member(edge->src()) && op2.is_member(edge->dest()) ||
       op2.is_member(edge->src()) && op1.is_member(edge->dest()) ) {
      //      boundary_weight -= 2*(edge_weights.value(edge));
      boundary_weight -= 2;
    }
  }
}

Coarse_op& Coarse_op::operator+= (const Coarse_op & lhs)
{  
  if(members.size() == 0)
    this->valid_clusters += lhs.valid_clusters;
  else
    this->valid_clusters *= lhs.valid_clusters;

  this->members += lhs.members;
  this->in_edges += lhs.in_edges;
  this->out_edges += lhs.out_edges;
  this->boundary_weight += lhs.boundary_weight;

  for (Slist_iterator<Stuple<float, int> > sli(lhs.edge_list);
       sli != 0; sli++) {
    Stuple<float, int> tuple = *sli;
    this->edge_list.add(tuple);      
  }

  return *this;
}

bool Coarse_op::operator== (const Coarse_op & lhs) const
{
  return(false);
}

bool Coarse_op::operator!= (const Coarse_op & lhs) const
{
  return(true);
}

bool Coarse_op::operator< (const Coarse_op & lhs) const
{
  return(false);
}

void Coarse_op::print_members()
{
  for(Hash_set_iterator<Op*> hmi(members); hmi != 0; hmi++) {
    Op* op = *hmi;
    cdbg << op->id() << " ";      
  }
  cdbg << endl;
}

unsigned hash_coarse_op_ptr(Coarse_op*& i)
{
   return(ELCOR_INT_HASH(i)) ;
}

void Coarse_op::print(ostream& os) const {
  os << "( ";
  for(Hash_set_iterator<Op*> opi(this->members); opi!=0; opi++) {
    Op* op = *opi;
    os << op->id() << " ";
  }
  os << ")";

  if(this->locked)
    os << " [locked]";
  os << endl;
}

ostream& operator<<(ostream& os, const Coarse_op& cop){
  cop.print(os);
  return os;
}

