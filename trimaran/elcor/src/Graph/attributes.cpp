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
//      File:           attributes.cpp
//      Authors:        Sadun Anik, Richard Johnson, Santosh Abraham,
//                      Dave Agust, Vinod Kathail, Brian Deitrich
//      Created:        December 1994
//      Description:    Attributes for IR graph components.
//
/////////////////////////////////////////////////////////////////////////////

#include "attributes.h"
#include "edge.h"
#include "op.h"
#include "region.h"
#include "el_error.h"

Graph_attribute::Graph_attribute() {}

Graph_attribute::Graph_attribute(Graph_attribute& attr) 
{
   attribute_map = attr.attribute_map ;
   for (Map_domain_iterator<Attribute_type, Base_attribute*> atr_mapi(attribute_map) ; 
	atr_mapi != 0 ; atr_mapi++){
      Attribute_type a_type = *atr_mapi ;
      Base_attribute* tmp_ptr = get_attribute(a_type) ;
      Base_attribute* new_ptr = tmp_ptr->clone_attribute() ;
      if (new_ptr != NULL) {
	 attribute_map.bind(*atr_mapi,new_ptr) ;
      }
   }
}

Graph_attribute::~Graph_attribute() 
{
   for (Map_domain_iterator<Attribute_type, Base_attribute*> atr_mapi(attribute_map) ; 
	atr_mapi != 0 ; ) {
      Attribute_type a_type = *atr_mapi ;
      atr_mapi++;
      Base_attribute* tmp_ptr = get_attribute(a_type) ;
      attribute_map.unbind(a_type) ;
      delete tmp_ptr ;
   }
}

Graph_attribute& Graph_attribute::operator=(Graph_attribute& attr) 
{
   if (this != &attr) {
      attribute_map = attr.attribute_map ;
      for (Map_domain_iterator<Attribute_type, Base_attribute*> atr_mapi(attribute_map);
	   atr_mapi != 0 ; atr_mapi++){
	 Attribute_type a_type = *atr_mapi ;
	 Base_attribute* tmp_ptr = get_attribute(a_type) ;
	 Base_attribute* new_ptr = tmp_ptr->clone_attribute() ;
	 if (new_ptr != NULL) {
	    attribute_map.bind(*atr_mapi,new_ptr) ;
	 }
      }
   }
   return (*this) ;
}

void Graph_attribute::insert_attribute(Attribute_type a_type, Base_attribute* attrib)
{
   attribute_map.bind(a_type,attrib) ;
}

Base_attribute* Graph_attribute::get_attribute(Attribute_type a_type)
{
   if (attribute_map.is_bound(a_type)) {
      return(attribute_map.value(a_type)) ;
   } else {
      return NULL ;
   }
}

void Graph_attribute::remove_attribute(Attribute_type a_type)
{
   Base_attribute* tmp_ptr = get_attribute(a_type) ;
   if (tmp_ptr) {
      delete tmp_ptr ;
      attribute_map.unbind(a_type) ;
   }
}

void Graph_attribute::IR_print_inline(IR_outstream &out)
{}

void Graph_attribute::IR_print_offline(IR_outstream &out)
{}

void Graph_attribute::IR_parse_inline(IR_instream &in)
{}

void Graph_attribute::IR_parse_offline(IR_instream &in)
{}

void Graph_attribute::IR_name_attributes(IR_outstream &out)
{}

///////////////////////////////////////////////////////////////////////////
//
// Generic attributes
//

Generic_attribute_map_attrib::Generic_attribute_map_attrib() : map(0) {}

Generic_attribute_map_attrib::~Generic_attribute_map_attrib()
{
   delete map ;
}   

Base_attribute* Generic_attribute_map_attrib::clone_attribute()
{
   Generic_attribute_map_attrib* tmp_ptr = new  Generic_attribute_map_attrib ;
   tmp_ptr->map = new Generic_attribute_map(*map) ;
   return(tmp_ptr) ;
}

/////////////////////////////////////////////////////////////////////
// 
//    Generic_attribute_map for edges
//
/////////////////////////////////////////////////////////////////////

Generic_attribute_map* get_generic_attribute_map(Edge* e)
{
   Generic_attribute_map_attrib* tmp_ptr ;

   tmp_ptr = (Generic_attribute_map_attrib*)
      (e->attributes->get_attribute(GENERIC_ATTRIBUTES));
   if (tmp_ptr == 0) {
      tmp_ptr = new Generic_attribute_map_attrib() ;
      tmp_ptr->map = new Generic_attribute_map() ;
      e->attributes->insert_attribute(GENERIC_ATTRIBUTES,tmp_ptr) ;
   }
   return (tmp_ptr->map) ;
}

void set_generic_attribute_map(Edge* e, Generic_attribute_map* lat)
{
   Generic_attribute_map_attrib* tmp_ptr = (Generic_attribute_map_attrib*)
      e->attributes->get_attribute(GENERIC_ATTRIBUTES);
   if (tmp_ptr == 0) {
      tmp_ptr = new Generic_attribute_map_attrib() ;
      tmp_ptr->map = 0 ;
   }
   delete tmp_ptr->map ;
   tmp_ptr->map = lat ;
}

void remove_generic_attribute_map(Edge* e)
{
   e->attributes->remove_attribute(GENERIC_ATTRIBUTES) ;
}


void* get_generic_attribute(Edge* e, const eString& str)
{
   Generic_attribute_map* gmap = get_generic_attribute_map(e) ;
   if (gmap->is_bound(str)) {
      return (gmap->value(str)) ;
   } else {
      return 0 ;
   }
}

void set_generic_attribute(Edge* e, const eString& str, void* gnrc_ptr)
{
   Generic_attribute_map* gmap = get_generic_attribute_map(e) ;
   gmap->bind(str,gnrc_ptr) ;
}

void remove_generic_attribute(Edge* e, const eString& str)
{
   Generic_attribute_map* gmap = get_generic_attribute_map(e) ;
   gmap->unbind(str) ;
   if (gmap->is_empty()) {
      remove_generic_attribute_map(e) ;
   }
}


///////////////////////////////////////////////////////////////////
//
//  Region attributes
//
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// 
//    Generic_attribute_map for regions
//
/////////////////////////////////////////////////////////////////////

Generic_attribute_map* get_generic_attribute_map(Region* r)
{
   Generic_attribute_map_attrib* tmp_ptr ;

   tmp_ptr = (Generic_attribute_map_attrib*)
      (r->attributes->get_attribute(GENERIC_ATTRIBUTES));
   if (tmp_ptr == 0) {
      tmp_ptr = new Generic_attribute_map_attrib ;
      tmp_ptr->map = new Generic_attribute_map ;      
      r->attributes->insert_attribute(GENERIC_ATTRIBUTES,tmp_ptr) ;
   }
   return (tmp_ptr->map) ;
   
}

void set_generic_attribute_map(Region* r, Generic_attribute_map* lat)
{
   Generic_attribute_map_attrib* tmp_ptr = (Generic_attribute_map_attrib*)
      r->attributes->get_attribute(GENERIC_ATTRIBUTES);
   if (tmp_ptr->map) delete tmp_ptr->map ;
}

void remove_generic_attribute_map(Region* r)
{
   r->attributes->remove_attribute(GENERIC_ATTRIBUTES) ;
}


void* get_generic_attribute(Region* r, const eString& str)
{
   Generic_attribute_map* gmap = get_generic_attribute_map(r) ;
   if (gmap->is_bound(str)) {
      return (gmap->value(str)) ;
   } else {
      return 0 ;
   }
}

void set_generic_attribute(Region* r, const eString& str, void* gnrc_ptr)
{
   Generic_attribute_map* gmap = get_generic_attribute_map(r) ;
   gmap->bind(str,gnrc_ptr) ;
}

void remove_generic_attribute(Region* r, const eString& str)
{
   Generic_attribute_map* gmap = get_generic_attribute_map(r) ;
   gmap->unbind(str) ;
   if (gmap->is_empty()) {
      remove_generic_attribute_map(r) ;
   }
}

