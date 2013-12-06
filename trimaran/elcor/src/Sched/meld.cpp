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
//      File:           meld.cpp
//      Authors:        Vinod Kathail, Santosh G. Abraham
//      Created:        April 1995
//      Description:    MS_constraints class for holding meld info on region
//
/////////////////////////////////////////////////////////////////////////////

#include "meld.h"
#include "iterators.h"
#include "intf.h"	// defines MDES_max_op_lat
#include "attributes.h"	

Latency_map::Latency_map(int default_val) {
    rep_default = default_val;
}

Latency_map::~Latency_map(){}

Latency_map::Latency_map(const Latency_map& map)
    :Map<Operand, int>(map), rep_default(map.rep_default) {}

void Latency_map::set_default_value(int default_value){
    rep_default = default_value;
}

int Latency_map::get_default_value() {
    return (rep_default);
}

Latency_map& Latency_map::operator=(const Latency_map& map){
    Map<Operand, int>::operator=(map);
    rep_default = map.rep_default;
    return *this;
}

int& Latency_map::value(const Operand& operand){
    if (this->is_bound(operand)) 
        return (Map<Operand, int>::value(operand));
    else 
        return (rep_default);
}

void Latency_map::print(ostream& os, const Latency_map& lat_map) const {
    for (Latency_map_iterator lmi(lat_map); lmi != 0; lmi++) {
        Operand curr_operand = (*lmi).first;
        int latency = (*lmi).second;
        os << "       - operand = " << curr_operand;
        os << "    with latency = " << latency << endl;
    }
}

ostream& operator<<(ostream& os, const Latency_map& lat_map){
    lat_map.print(os, lat_map);
    return os;
}




MS_constraints::MS_constraints(){}

MS_constraints::MS_constraints(Compound_region *region)
{
    List<Op*>  entry_ops, exit_ops;

    for (Region_entry_ops entry(region); entry != 0; entry++) {
        Latency_map* new_def_map = new Latency_map(MDES_max_op_lat);
        this->def_entry_map.bind (*entry, new_def_map);
        Latency_map* new_use_map = new Latency_map(MDES_max_op_lat);
        this->use_entry_map.bind (*entry, new_use_map);

        Latency_map* new_import_def_map = new Latency_map(0);
        this->import_def_entry_map.bind (*entry, new_import_def_map);
        Latency_map* new_import_use_map = new Latency_map(0);
        this->import_use_entry_map.bind (*entry, new_import_use_map);

	List<Operand>* new_dm_list = new List<Operand>();
	this->def_merge_map.bind(*entry, new_dm_list);
	List<Operand>* new_um_list = new List<Operand>();
	this->use_merge_map.bind(*entry, new_um_list);

        entry_ops.push (*entry);
    }

    for (Region_exit_ops exit(region); exit != 0; exit++) {
        Latency_map* new_def_map = new Latency_map(0);
        this->def_exit_map.bind (*exit, new_def_map);
        Latency_map* new_use_map = new Latency_map(0);
        this->use_exit_map.bind (*exit, new_use_map);

        Latency_map* new_import_def_map = new Latency_map(MDES_max_op_lat);
        this->import_def_exit_map.bind (*exit, new_import_def_map);
        Latency_map* new_import_use_map = new Latency_map(MDES_max_op_lat);
        this->import_use_exit_map.bind (*exit, new_import_use_map);

	List<Operand>* new_ds_list = new List<Operand>();
	this->def_switch_map.bind(*exit, new_ds_list);

        exit_ops.push (*exit);
    }

    for (List_iterator<Op*> en(entry_ops); en != 0; en++)
        for (List_iterator<Op*> ex(exit_ops); ex != 0; ex++) {
            Pair<Op*, Op*>* new_pair = new Pair<Op*, Op*> (*en, *ex);
            this->min_distance.bind (*new_pair, -1);
            delete new_pair;
        }
} 

MS_constraints::MS_constraints(const MS_constraints& constr) {}
#if 0
// not used and not up to date (at least need to add import*map, *merge_map
//	 and def_switch_map)
{
    for (Map_iterator<Pair<Op*, Op*>, int> mi(constr.min_distance); mi != 0;
                                            mi++)
        this->min_distance.bind ((*mi).first, (*mi).second);

    for (Map_iterator<Op*, Latency_map*> mi_def_entry(constr.def_entry_map);
                                         mi_def_entry != 0; mi_def_entry++) {
        Latency_map* new_lat_map = (*mi_def_entry).second;
        this->def_entry_map.bind ((*mi_def_entry).first, new_lat_map);
    }

    for (Map_iterator<Op*, Latency_map*> mi_use_entry(constr.use_entry_map);
                                         mi_use_entry != 0; mi_use_entry++) {
        Latency_map* new_lat_map = (*mi_use_entry).second;
        this->use_entry_map.bind ((*mi_use_entry).first, new_lat_map);
    }

    for (Map_iterator<Op*, Latency_map*> mi_def_exit(constr.def_exit_map);
                                         mi_def_exit != 0; mi_def_exit++) {
        Latency_map* new_lat_map = (*mi_def_exit).second;
        this->def_exit_map.bind ((*mi_def_exit).first, new_lat_map);
    }

    for (Map_iterator<Op*, Latency_map*> mi_use_exit(constr.use_exit_map);
                                         mi_use_exit != 0; mi_use_exit++) {
        Latency_map* new_lat_map = (*mi_use_exit).second;
        this->use_entry_map.bind ((*mi_use_exit).first, new_lat_map);
    }
}
#endif

MS_constraints::~MS_constraints()
{
    for (Map_iterator<Op*, Latency_map*> mi_def_entry(this->def_entry_map);
                                         mi_def_entry != 0; mi_def_entry++) {
        Latency_map* old_lat_map = (*mi_def_entry).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, Latency_map*> mi_use_entry(this->use_entry_map);
                                         mi_use_entry != 0; mi_use_entry++) {
        Latency_map* old_lat_map = (*mi_use_entry).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, Latency_map*> mi_def_exit(this->def_exit_map);
                                         mi_def_exit != 0; mi_def_exit++) {
        Latency_map* old_lat_map = (*mi_def_exit).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, Latency_map*> mi_use_exit(this->use_exit_map);
                                         mi_use_exit != 0; mi_use_exit++) {
        Latency_map* old_lat_map = (*mi_use_exit).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, Latency_map*> ide (this->import_def_entry_map);
                                         ide != 0; ide++) {
        Latency_map* old_lat_map = (*ide).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, Latency_map*> iue (this->import_use_entry_map);
                                         iue != 0; iue++) {
        Latency_map* old_lat_map = (*iue).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, Latency_map*> idx(this->import_def_exit_map);
                                         idx != 0; idx++) {
        Latency_map* old_lat_map = (*idx).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, Latency_map*> iux(this->import_use_exit_map);
                                         iux != 0; iux++) {
        Latency_map* old_lat_map = (*iux).second;
        delete old_lat_map;
    }

    for (Map_iterator<Op*, List<Operand>*> mi_dm_entry(this->def_merge_map);
                                         mi_dm_entry != 0; mi_dm_entry++) {
        List<Operand>* operlist = (*mi_dm_entry).second;
        delete operlist;
    }

    for (Map_iterator<Op*, List<Operand>*> mi_um_entry(this->use_merge_map);
                                         mi_um_entry != 0; mi_um_entry++) {
        List<Operand>* operlist = (*mi_um_entry).second;
        delete operlist;
    }

    for (Map_iterator<Op*, List<Operand>*> mi_ds_exit(this->def_switch_map);
                                         mi_ds_exit != 0; mi_ds_exit++) {
        List<Operand>* operlist = (*mi_ds_exit).second;
        delete operlist;
    }

}

MS_constraints& MS_constraints::set_min_distance(Op* entry, Op* exit, 
						 int distance){
    Pair<Op*, Op*> x(entry, exit);
    min_distance.bind(x, distance);
    return *this;
    
}

MS_constraints& MS_constraints::set_remap_number(Op* entry, Op* exit, 
						 int number_of_remaps){
    Pair<Op*, Op*> x(entry, exit);
    remaps.bind(x, number_of_remaps);
    return *this;
    
}

MS_constraints& MS_constraints::set_def_at_entry(Op* entry, Latency_map* map)
{
    if(!(def_entry_map.is_bound(entry)))
	def_entry_map.bind(entry, map);
    else cerr << "Error: MS_constraints::set_def_at_entry-- Already bound";
    return *this;
}

MS_constraints& MS_constraints::set_use_at_entry(Op* entry, Latency_map* map)
{
    if(!(use_entry_map.is_bound(entry)))
	use_entry_map.bind(entry, map);
    else cerr << "Error: MS_constraints::set_use_at_entry-- Already bound";
    return *this;
}

MS_constraints& MS_constraints::set_def_at_exit(Op* exit, Latency_map* map)
{
    if(!(def_exit_map.is_bound(exit)))
	def_exit_map.bind(exit, map);
    else cerr << "Error: MS_constraints::set_def_at_exit-- Already bound";
    return *this;
}

MS_constraints& MS_constraints::set_use_at_exit(Op* exit, Latency_map* map)
{
    if(!(use_exit_map.is_bound(exit)))
	use_exit_map.bind(exit, map);
    else cerr << "Error: MS_constraints::set_use_at_exit-- Already bound";
    return *this;
}

MS_constraints& MS_constraints::set_import_def_at_entry
					(Op* entry, Latency_map* map)
{
 if(!(import_def_entry_map.is_bound(entry)))
   import_def_entry_map.bind(entry, map);
 else cerr << "Error: MS_constraints::set_import_def_at_entry-- Already bound";
  return *this;
}

MS_constraints& MS_constraints::set_import_use_at_entry
					(Op* entry, Latency_map* map)
{
 if(!(import_use_entry_map.is_bound(entry)))
   import_use_entry_map.bind(entry, map);
 else cerr << "Error: MS_constraints::set_import_use_at_entry-- Already bound";
   return *this;
}

MS_constraints& MS_constraints::set_import_def_at_exit
					(Op* exit, Latency_map* map)
{
 if(!(import_def_exit_map.is_bound(exit)))
   import_def_exit_map.bind(exit, map);
 else cerr << "Error: MS_constraints::set_import_def_at_exit-- Already bound";
   return *this;
}

MS_constraints& MS_constraints::set_import_use_at_exit
					(Op* exit, Latency_map* map)
{
 if(!(import_use_exit_map.is_bound(exit)))
   import_use_exit_map.bind(exit, map);
 else cerr << "Error: MS_constraints::set_import_use_at_exit-- Already bound";
   return *this;
}

MS_constraints& MS_constraints::set_def_merges_at_entry
					 (Op* entry, List<Operand>* opers)
{
    if (!(def_merge_map.is_bound(entry)))
          def_merge_map.bind(entry, opers);
    else cerr << "Error: MS_constraints::set_def_merges_at_entry-- \
		Already bound";
    return *this;
}

MS_constraints& MS_constraints::set_use_merges_at_entry
					(Op* entry, List<Operand>* opers)
{
    if (!(use_merge_map.is_bound(entry)))
          use_merge_map.bind(entry, opers);
    else cerr << "Error: MS_constraints::set_use_merges_at_entry-- \
		Already bound";
    return *this;
}

MS_constraints& MS_constraints::set_def_switches_at_exit
					 (Op* exit, List<Operand>* opers)
{
    if (!(def_switch_map.is_bound(exit)))
          def_switch_map.bind(exit, opers);
    else cerr << "Error: MS_constraints::set_def_switches_at_exit-- \
		Already bound";
    return *this;
}

MS_constraints& MS_constraints::unbind_def_at_entry(Op* entry)
{
    def_entry_map.unbind(entry);
    return *this;
}

MS_constraints& MS_constraints::unbind_use_at_entry(Op* entry)
{
    use_entry_map.unbind(entry);
    return *this;
}

MS_constraints& MS_constraints::unbind_def_at_exit(Op* exit)
{
    def_exit_map.unbind(exit);
    return *this;
}

MS_constraints& MS_constraints::unbind_use_at_exit(Op* exit)
{
    use_exit_map.unbind(exit);
    return *this;
}

MS_constraints& MS_constraints::unbind_import_def_at_entry(Op* entry)
{
    import_def_entry_map.unbind(entry);
    return *this;
}

MS_constraints& MS_constraints::unbind_import_use_at_entry(Op* entry)
{
    import_use_entry_map.unbind(entry);
    return *this;
}

MS_constraints& MS_constraints::unbind_import_def_at_exit(Op* exit)
{
    import_def_exit_map.unbind(exit);
    return *this;
}

MS_constraints& MS_constraints::unbind_import_use_at_exit(Op* exit)
{
    import_use_exit_map.unbind(exit);
    return *this;
}

int MS_constraints::get_min_distance(Op* entry, Op* exit){
    Pair<Op*, Op*> x(entry, exit);
    if (min_distance.is_bound(x)) {
      int dist = min_distance.value(x);
      assert (dist >= 0);
      return (dist);
    }
    assert (0);
    return 0;
}

int MS_constraints::remap_number(Op* entry, Op* exit){
    Pair<Op*, Op*> x(entry, exit);
#if 0
    if (remaps.is_bound(x)) return (remaps.value(x));
    else return (0);
#endif
    return 0;
}

Latency_map* MS_constraints::def_at_entry (Op* entry){
    if (def_entry_map.is_bound(entry)) return (def_entry_map.value(entry));
    else {
	  cerr << "MS_constraints::def_at_entry -- No latency map";
	  return ((Latency_map*)NULL);
    }
}

Latency_map* MS_constraints::use_at_entry(Op* entry){
    if (use_entry_map.is_bound(entry)) return (use_entry_map.value(entry));
    else {
	  cerr << "MS_constraints::use_at_entry -- No latency map";
	  return ((Latency_map*)NULL);
    }
}

Latency_map* MS_constraints::def_at_exit(Op* exit){
    if (def_exit_map.is_bound(exit)) return (def_exit_map.value(exit));
    else {
	  cerr << "MS_constraints::def_at_exit -- No latency map";
          return ((Latency_map*)NULL);
    }
}

Latency_map* MS_constraints::use_at_exit(Op* exit){
    if (use_exit_map.is_bound(exit)) return (use_exit_map.value(exit));
    else {
	  cerr << "MS_constraints::use_at_exit -- No latency map";
          return ((Latency_map*)NULL);
    }
}

Latency_map* MS_constraints::import_def_at_entry (Op* entry){
    if (import_def_entry_map.is_bound(entry))
	 return (import_def_entry_map.value(entry));
    else {
	  cerr << "MS_constraints::import_def_at_entry -- No latency map";
	  return ((Latency_map*)NULL);
    }
}

Latency_map* MS_constraints::import_use_at_entry(Op* entry){
    if (import_use_entry_map.is_bound(entry))
	 return (import_use_entry_map.value(entry));
    else {
	  cerr << "MS_constraints::import_use_at_entry -- No latency map";
	  return ((Latency_map*)NULL);
    }
}

Latency_map* MS_constraints::import_def_at_exit(Op* exit){
    if (import_def_exit_map.is_bound(exit))
	 return (import_def_exit_map.value(exit));
    else {
	  cerr << "MS_constraints::import_def_at_exit -- No latency map";
          return ((Latency_map*)NULL);
    }
}

Latency_map* MS_constraints::import_use_at_exit(Op* exit){
    if (import_use_exit_map.is_bound(exit))
	 return (import_use_exit_map.value(exit));
    else {
	  cerr << "MS_constraints::import_use_at_exit -- No latency map";
          return ((Latency_map*)NULL);
    }
}

List<Operand>* MS_constraints::def_merges_at_entry(Op* entry){
    if (def_merge_map.is_bound(entry)) return (def_merge_map.value(entry));
    else {
	  cerr << "MS_constraints::def_merges_at_entry -- No operand list";
          return ((List<Operand>*)NULL);
    }
}

List<Operand>* MS_constraints::use_merges_at_entry(Op* entry){
    if (use_merge_map.is_bound(entry)) return (use_merge_map.value(entry));
    else {
	  cerr << "MS_constraints::use_merges_at_entry -- No operand list";
          return ((List<Operand>*)NULL);
    }
}

List<Operand>* MS_constraints::def_switches_at_exit(Op* exit){
    if (def_switch_map.is_bound(exit)) return (def_switch_map.value(exit));
    else {
	  cerr << "MS_constraints::def_switches_at_exit -- No operand list";
          return ((List<Operand>*)NULL);
    }
}

void MS_constraints::print(ostream& os) const {
    os << "MS_constraints" << endl;

    os << "  Entry-to-Exit Distances" << endl;
    for (Map_iterator<Pair<Op*, Op*>, int> mi(min_distance); mi != 0;
                                                 mi++) {
        Pair<Op*, Op*> entry_and_exit = (*mi).first;
        os << "    from " << (entry_and_exit.first)->id() << " to ";
        os << (entry_and_exit.second)->id() << ":  " << (*mi).second << endl;
    }

    os << "  Entry Def Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> mi_def_entry(def_entry_map);
                             mi_def_entry != 0; mi_def_entry++) {
        Op* entry_op = (*mi_def_entry).first;
        Latency_map* lat_map = (*mi_def_entry).second;
        os << "    for entry op " << entry_op->id() << endl;
        os << *lat_map;
    }

    os << "  Entry Use Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> mi_use_entry(use_entry_map);
                             mi_use_entry != 0; mi_use_entry++) {
        Op* entry_op = (*mi_use_entry).first;
        Latency_map* lat_map = (*mi_use_entry).second;
        os << "    for entry op " << entry_op->id() << endl;
        os << *lat_map;
    }

    os << "  Exit Def Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> mi_def_exit(def_exit_map);
                             mi_def_exit != 0; mi_def_exit++) {
        Op* exit_op = (*mi_def_exit).first;
        Latency_map* lat_map = (*mi_def_exit).second;
        os << "    for exit op " << exit_op->id() << endl;
        os << *lat_map;
    }

    os << "  Exit Use Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> mi_use_exit(use_exit_map);
                             mi_use_exit != 0; mi_use_exit++) {
        Op* exit_op = (*mi_use_exit).first;
        Latency_map* lat_map = (*mi_use_exit).second;
        os << "    for exit op " << exit_op->id() << endl;
        os << *lat_map;
    }

    os << "  Import Entry Def Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> ide(import_def_entry_map);
                             ide != 0; ide++) {
        Op* entry_op = (*ide).first;
        Latency_map* lat_map = (*ide).second;
        os << "    for entry op " << entry_op->id() << endl;
        os << *lat_map;
    }

    os << "  Import Entry Use Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> iue(import_use_entry_map);
                             iue != 0; iue++) {
        Op* entry_op = (*iue).first;
        Latency_map* lat_map = (*iue).second;
        os << "    for entry op " << entry_op->id() << endl;
        os << *lat_map;
    }

    os << "  Import Exit Def Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> idx(import_def_exit_map);
                             idx != 0; idx++) {
        Op* exit_op = (*idx).first;
        Latency_map* lat_map = (*idx).second;
        os << "    for exit op " << exit_op->id() << endl;
        os << *lat_map;
    }

    os << "  Import Exit Use Latency Maps" << endl;
    for (Map_iterator<Op*, Latency_map*> iux(import_use_exit_map);
                             iux != 0; iux++) {
        Op* exit_op = (*iux).first;
        Latency_map* lat_map = (*iux).second;
        os << "    for exit op " << exit_op->id() << endl;
        os << *lat_map;
    }

    os << "  Def Merge Maps" << endl;
    for (Map_iterator<Op*, List<Operand>*> mi_dm_entry(def_merge_map);
                             mi_dm_entry != 0; mi_dm_entry++) {
        Op* entry_op = (*mi_dm_entry).first;
        List<Operand>* operlist = (*mi_dm_entry).second;
        os << "    for entry op " << entry_op->id() << endl;
        os << *operlist;
    }

    os << "  Use Merge Maps" << endl;
    for (Map_iterator<Op*, List<Operand>*> mi_um_entry(use_merge_map);
                             mi_um_entry != 0; mi_um_entry++) {
        Op* entry_op = (*mi_um_entry).first;
        List<Operand>* operlist = (*mi_um_entry).second;
        os << "    for entry op " << entry_op->id() << endl;
        os << *operlist;
    }

    os << "  Def Switch Maps" << endl;
    for (Map_iterator<Op*, List<Operand>*> mi_ds_entry(def_switch_map);
                             mi_ds_entry != 0; mi_ds_entry++) {
        Op* exit_op = (*mi_ds_entry).first;
        List<Operand>* operlist = (*mi_ds_entry).second;
        os << "    for exit op " << exit_op->id() << endl;
        os << *operlist;
    }
}


ostream& operator<<(ostream& os, const MS_constraints& msc){
    msc.print(os);
    return os;
}


//
// Attribute class and access functions for meld scheduling
//

MS_constraints_attrib::MS_constraints_attrib()
   : msc(NULL) 
{}

MS_constraints_attrib::~MS_constraints_attrib()
{
   delete msc ;
}

Base_attribute* MS_constraints_attrib::clone_attribute()
{
   MS_constraints_attrib* tmp_ptr = new MS_constraints_attrib() ;
   tmp_ptr->msc = new MS_constraints();
   return (tmp_ptr) ;
}

// void MS_constraints_attrib::print_ptr(IR_outstream &out)
// {
//     // Print out a pointer to this attribute;
//     out << MELD_ATTR_STRING << (void*)this;
// } 


// void MS_constraints_attrib::print(IR_outstream &out)
// {
//   // Print out an MS_constraints attribute;
//   // NOTE: not implemented yet.
//   if (!out.has_been_printed((void*) this)) {
//     out << this << SPACE << L_PAREN << "not implemented yet" << R_PAREN;
//     out.mark_printed((void*) this);
//   }
//   return out;
// } 

// MS_constraints *ms(IR_instream &in)
// {
//     // Extracts an ms_constraints attribute, if any, from the input stream
//     // and fills in its information.  Returns true if an attribute is found;
//     // NOTE: not really implemented yet--just strips it out;

//     IR_token token;
//     in >> token;
//     if (token == MELD_ATTR_STRING) {
// 	void *ptr;
// 	in >> ptr >> L_PAREN;
// 	if (ptr == NULL) {
// 	    cerr << "orphaned attribute" << endl;
// 	    exit(1);
// 	}
// 	MS_constraints *ms = (MS_constraints*) ptr;

// 	// Strip it out for now...;
// 	for (in >> token;  token != R_PAREN;  in >> token)
// 	    ;
// 	return ms;
//     } else {
// 	in.unget_token();
// 	return NULL;
//     }
// }

MS_constraints* get_ms_constraints(Compound_region* region) {
   MS_constraints_attrib* tmp_ptr = (MS_constraints_attrib*) 
       (region->attributes->get_attribute(MS_CONSTRAINTS));
   if (tmp_ptr == 0) {
      tmp_ptr = new MS_constraints_attrib() ;
      tmp_ptr->msc = new MS_constraints(region) ;
      region->attributes->insert_attribute(MS_CONSTRAINTS, tmp_ptr) ;
   }
   return (tmp_ptr->msc) ;
}

bool has_ms_constraints(Compound_region* region) {
   MS_constraints_attrib* tmp_ptr = (MS_constraints_attrib*) 
       (region->attributes->get_attribute(MS_CONSTRAINTS));
   if (tmp_ptr == 0) return false;
   return true;
}
   
void set_ms_constraints(Compound_region* region, MS_constraints* constr) {
   MS_constraints_attrib* tmp_ptr = (MS_constraints_attrib*) 
       (region->attributes->get_attribute(MS_CONSTRAINTS));
   if (tmp_ptr == NULL) {
      tmp_ptr = new MS_constraints_attrib() ;
   }
   tmp_ptr->msc = constr ;
   region->attributes->insert_attribute(MS_CONSTRAINTS, tmp_ptr) ;
}

void remove_ms_constraints(Compound_region* region) {
   region->attributes->remove_attribute(MS_CONSTRAINTS) ;
}

