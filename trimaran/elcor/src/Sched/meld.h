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
//      File:           meld.h
//      Authors:        Vinod Kathail, Santosh G. Abraham
//      Created:        April 1995
//      Description:    MS_constraints class for holding meld info on region
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MELD_
#define _MELD_

#include "map.h"
#include "op.h"
#include "region.h"
#include "attribute_types.h"
#include "IR_outstream.h"
#include "IR_instream.h"

typedef Map_iterator<Operand, int> Latency_map_iterator;

class Latency_map: public Map<Operand, int> {
  friend ostream& operator<<(ostream& os, const Latency_map& lat_map);
 public:
  Latency_map(int default_val = 0);
  Latency_map(const Latency_map& map);
  ~Latency_map();
  void set_default_value(int); 
  int  get_default_value();

  Latency_map& operator=(const Latency_map& map);

  int& value(const Operand&); 

 protected:
    void print(ostream&, const Latency_map&) const ;
    
 private:
    int rep_default;
};

class MS_constraints{
    friend ostream& operator<<(ostream& os, const MS_constraints& msc);
 public:
    MS_constraints();
    MS_constraints(Compound_region *region);
    MS_constraints(const MS_constraints& constr);
    ~MS_constraints();
    
    MS_constraints& set_min_distance(Op* entry, Op* exit, int distance);
    int get_min_distance(Op* entry, Op* exit);

    MS_constraints& set_remap_number(Op* entry, Op* exit, 
				     int number_of_remaps);
    int remap_number(Op* entry, Op* exit);

    MS_constraints& set_def_at_entry (Op* entry, Latency_map* map);
    MS_constraints& set_use_at_entry (Op* entry, Latency_map* map);
    MS_constraints& set_def_at_exit (Op* exit, Latency_map* map);
    MS_constraints& set_use_at_exit (Op* exit, Latency_map* map);
    Latency_map* def_at_entry (Op* entry);
    Latency_map* use_at_entry (Op* entry);
    Latency_map* def_at_exit (Op* exit);
    Latency_map* use_at_exit (Op* exit);

    MS_constraints& set_import_def_at_entry (Op* entry, Latency_map* map);
    MS_constraints& set_import_use_at_entry (Op* entry, Latency_map* map);
    MS_constraints& set_import_def_at_exit (Op* exit, Latency_map* map);
    MS_constraints& set_import_use_at_exit (Op* exit, Latency_map* map);
    Latency_map* import_def_at_entry (Op* entry);
    Latency_map* import_use_at_entry (Op* entry);
    Latency_map* import_def_at_exit (Op* exit);
    Latency_map* import_use_at_exit (Op* exit);

    MS_constraints& set_def_merges_at_entry (Op* entry, List<Operand>* opers);
    MS_constraints& set_use_merges_at_entry (Op* entry, List<Operand>* opers);
    MS_constraints& set_def_switches_at_exit (Op* exit, List<Operand>* opers);
    List<Operand>* def_merges_at_entry (Op* entry);
    List<Operand>* use_merges_at_entry (Op* entry);
    List<Operand>* def_switches_at_exit (Op* exit);

    MS_constraints& unbind_def_at_entry (Op* entry);
    MS_constraints& unbind_use_at_entry (Op* entry);
    MS_constraints& unbind_def_at_exit (Op* exit);
    MS_constraints& unbind_use_at_exit (Op* exit);

    MS_constraints& unbind_import_def_at_entry (Op* entry);
    MS_constraints& unbind_import_use_at_entry (Op* entry);
    MS_constraints& unbind_import_def_at_exit  (Op* exit);
    MS_constraints& unbind_import_use_at_exit  (Op* exit);

 protected:
    void print(ostream&) const ;

 private:
    // Latency constraints
    // Minimum distance between entry and exit
    Map<Pair<Op*, Op*>, int> min_distance;
    // Number of remaps between entry and exit
    Map<Pair<Op*, Op*>, int> remaps;

    // def and use constraints to export to predecessors -- Per entry basis
    Map<Op*, Latency_map*> def_entry_map;
    Map<Op*, Latency_map*> use_entry_map;
    // def and use constraints to export to successors -- Per exit basis
    Map<Op*, Latency_map*> def_exit_map;
    Map<Op*, Latency_map*> use_exit_map;

    // def and use constraints to import from predecessors -- Per entry basis
    Map<Op*, Latency_map*> import_def_entry_map;
    Map<Op*, Latency_map*> import_use_entry_map;
    // def and use constraints to import from successors -- Per exit basis
    Map<Op*, Latency_map*> import_def_exit_map;
    Map<Op*, Latency_map*> import_use_exit_map;

    // additional merge and switch ops at entry and exit respy
    Map<Op*, List<Operand>* > def_merge_map;
    Map<Op*, List<Operand>* > use_merge_map;
    Map<Op*, List<Operand>* > def_switch_map;
};

// Meld Scheduling attributes

class MS_constraints_attrib : public Base_attribute {
public:
  MS_constraints_attrib() ;
  ~MS_constraints_attrib() ;

  Base_attribute* clone_attribute() ;
  bool has_rebel_io() { return false; }
  Attribute_type attribute_type() { return MS_CONSTRAINTS; }

  MS_constraints* msc ;
} ;

MS_constraints* get_ms_constraints(Compound_region*) ;
bool 		has_ms_constraints(Compound_region*) ;
void           set_ms_constraints(Compound_region*, MS_constraints*) ;
void           remove_ms_constraints(Compound_region*) ;

#endif
