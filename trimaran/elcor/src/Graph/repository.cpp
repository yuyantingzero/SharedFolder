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
//      File:           repository.cpp
//      Authors:        Vinod Kathail
//      Created:        May 1994
//      Description:    Mechanisms for saving a region
//
/////////////////////////////////////////////////////////////////////////////

#include "repository.h"
#include "region_utilities.h"
#include "iterators.h"
#include "region_purge.h"
#include "op.h"
#include "edge.h"


//--------------------  Declare a repository ---------------------------------
static Repository repository;

// ------------------- External functions ------------------------------------
int save_in_repository(Region* region) {
  return (repository.save(region));
}

void restore_from_repository(int tag){
  repository.restore(tag);
  return;
}

void remove_from_repository(int tag){
  repository.remove(tag);
  return;
}

//-------------------------- Repository class --------------------------------

Repository::Repository()
	:cookie(0) {} ;

Repository::~Repository(){};

int Repository::save(Region* region)
{
  Region* copy_of_region = copy_for_repository(region);
  cookie++;
  rep.bind(cookie, copy_of_region);
  return(cookie);
}

void Repository::restore(int cookie)
{
  Region* region = rep.value(cookie);
  rep.unbind(cookie);
  restore_for_repository(region);
  return;
}

void Repository::remove(int)
{
  Region* region = rep.value(cookie);
  rep.unbind(cookie);
  ::region_purge(region);
  return;
}

Region* Repository::copy_for_repository(Region* region) {
  Edge* edge;
  Edge* new_edge;
  Region* temp_new;
  Region* temp_old;
  Region_map map_for_regions;
  Edge_map map_for_edges;
  Region* copy_of_region = 
    region->copy_with_specified_edges(map_for_regions, map_for_edges);
  // Set src or dest ops for interface edges
  for (Region_all_inedges in(region); in != 0; in++) {
    edge = *in;
    new_edge = map_for_edges.value(edge);
    new_edge->set_src(edge->src(), edge->src_port(), edge->src_alt());
  }

  for (Region_all_outedges out(region); out != 0; out++) {
    edge = *out;
    new_edge = map_for_edges.value(edge);
    new_edge->set_dest(edge->dest(), edge->dest_port(), edge->dest_alt());
  }

  // Preserve unique id's for ops and subregions
  // Assume Region_map is correctly set_up
  for (Map_iterator<Region*, Region*>  miter(map_for_regions);  
          miter != 0; miter++){
    temp_old = (*miter).first;
    temp_new = (*miter).second;
    temp_new->set_id(temp_old->id());
  }
  
  // Preserve parent information
  copy_of_region->set_parent(region->parent());
  return copy_of_region;
}

void Repository::restore_for_repository(Region* region) {
  Edge* edge;
  Op* op;
  Region* parent;

 // Connect edges to ops
  for(Region_all_inedges in(region); in != 0; in++) {
    edge = *in;
    op = edge->src();
    op->add_outedge(edge, edge->dest_port_type(),
		    edge->dest_port(), edge->dest_alt());
  }
  for(Region_all_outedges out(region); out != 0; out++) {
    edge = *out;
    op = edge->dest();
    op->add_inedge(edge, edge->dest_port_type(),
		   edge->dest_port(), edge->dest_alt());
  }
  
  // Update entry and exit edges of appropriate surrounding regions and
  // region's ancestors in the hierarchy 
  for (Region_entry_edges entry(region); entry != 0; entry++){
    edge = *entry;
    op = edge->src();
    parent = ::region_find_common_ancestor(op, region);
    ::set_ancestors_exit_edges(op, parent, edge);
    ::set_ancestors_entry_edges(region, parent, edge);
    }
  for (Region_exit_edges exit(region); exit != 0; exit++){
    edge = *exit;
    op = edge->dest();
    parent = ::region_find_common_ancestor(op, region);
    ::set_ancestors_entry_edges(op, parent, edge);
    ::set_ancestors_exit_edges(region, parent, edge);
    }

  //Add to the parent 
  ((Compound_region*)(region->parent()))->add_region(region);

  return;
}

// Auxiliary functions 
void set_ancestors_entry_edges(Region* from, Region* to, Edge* edge){
  Region* temp = from->parent();
  while(temp != to) {
    ((Compound_region*)temp)->add_entry_safely(edge);
    temp = temp->parent();
  }
}

void set_ancestors_exit_edges(Region* from, Region* to, Edge* edge){
  Region* temp = from->parent();
  while(temp != to) {
    ((Compound_region*)temp)->add_exit_safely(edge);
    temp = temp->parent();
  }
}
