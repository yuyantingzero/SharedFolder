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
//      File:           edge_attributes.h
//      Authors:        Sadun Anik, Scott Mahlke
//      Created:        December 1994
//      Description:    Edge attributes 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _EDGE_ATTRIBUTES_H
#define _EDGE_ATTRIBUTES_H

#include "attribute_types.h"
#include "hash_set.h"
#include "operand.h"

/////////////////////////////////////////////////////////////////////////////
//
// Liveness info
//

typedef List<Operand> Liveness_info ;
typedef List_iterator<Operand> Liveness_info_iterator ;

class Liveness_info_attrib : public Base_attribute {
public:
  Liveness_info_attrib() ;
  ~Liveness_info_attrib() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return LIVENESS_SET; }
  bool is_empty() { return (!info || info->is_empty()); }
  bool is_edge_attribute() { return true; }
      
  Liveness_info* info ;
} ;

Liveness_info* get_liveness_info(Edge*) ;
void           set_liveness_info(Edge*, Liveness_info*) ;
void           remove_liveness_info(Edge*) ;

/////////////////////////////////////////////////////////////////////////////
//
// Down exposed uses info
//

class Downward_use_attrib : public Base_attribute {
public:
  Downward_use_attrib() ;
  ~Downward_use_attrib() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return DOWN_EXPOSED_USES_SET; }
  bool is_empty() { return (!info || info->is_empty()); }
  bool is_edge_attribute() { return true; }
      
  Liveness_info* info ;
} ;

Liveness_info* get_down_exposed_uses_info(Edge*) ;
void           set_down_exposed_uses_info(Edge*, Liveness_info*) ;
void           remove_down_exposed_uses_info(Edge*) ;

/////////////////////////////////////////////////////////////////////////////
//
// Up exposed defs info
//

class Upward_def_attrib : public Base_attribute {
public:
  Upward_def_attrib() ;
  ~Upward_def_attrib() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return UP_EXPOSED_DEFS_SET; }
  bool is_empty() { return (!info || info->is_empty()); }
  bool is_edge_attribute() { return true; }
      
  Liveness_info* info ;
} ;

Liveness_info* get_up_exposed_defs_info(Edge*) ;
void           set_up_exposed_defs_info(Edge*, Liveness_info*) ;
void           remove_up_exposed_defs_info(Edge*) ;

/////////////////////////////////////////////////////////////////////////////
//
// Down exposed defs info
//

class Downward_def_attrib : public Base_attribute {
public:
  Downward_def_attrib() ;
  ~Downward_def_attrib() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return DOWN_EXPOSED_DEFS_SET; }
  bool is_empty() { return (!info || info->is_empty()); }
  bool is_edge_attribute() { return true; }
      
  Liveness_info* info ;
} ;

Liveness_info* get_down_exposed_defs_info(Edge*) ;
void           set_down_exposed_defs_info(Edge*, Liveness_info*) ;
void           remove_down_exposed_defs_info(Edge*) ;

/////////////////////////////////////////////////////////////////////////////

#define EL_UNKNOWN_CC_VAL       -9999999

class Control_flow_freq : public Base_attribute {
public:
  Control_flow_freq() ;
  ~Control_flow_freq() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return true; }
  Attribute_type attribute_type() { return CONTROL_FLOW_FREQ; }
  bool is_edge_attribute() { return true; }
  bool is_inline() { return true; }

  double freq ;
  int cc;
} ;

Control_flow_freq* get_control_flow_freq(Edge*) ;
void               set_control_flow_freq(Edge*, Control_flow_freq*) ;
void               remove_control_flow_freq(Edge*) ;


#endif

