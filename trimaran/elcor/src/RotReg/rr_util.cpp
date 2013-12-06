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
//      File:           rr_util.cpp
//      Authors:        Alexandre Eichenberger
//      Created:        June 1994
//      Description:    auxiliary classes for lifetime analysis 
//
/////////////////////////////////////////////////////////////////////////////

#include "map.h"
#include "el_error.h"
#include "attributes.h"
#include "region_attributes.h"
#include "el_port.h"
#include "iterators.h"
#include "dbg.h"

#include "rr_allocation.h"
#include "rr_file.h"
#include "rr_life.h"
#include "rr_util.h"


////////////////////////////////////////////////////
// class RR_order
//   used in the register allocation to order the 
//   lifetimes according to the ordering strategy
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// compute priority
////////////////////////////////////////////////////

// prio is a list of partition where a partition is a list of elements
// an element is an index to its corresponding lifetime

List<int> RR_order::Order(RR_file_data *reg_file, const Ordering_strategy& strategy)
{
  file = reg_file;
  Input_prio();
  if (dbg(ra, 7)) 
    cdbg << DEBUG_TITLE2("Ordering for ", reg_file->phys_file) << *this;
  for(List_iterator<Order_strategy> i(strategy); i!=0; i++){
    if (*i != no_order) Order_lifetime_by(*i);
    if (dbg(ra, 7)) cdbg << *this;
  }
  Output_prio();
  return result;
}

void  RR_order::Input_prio()
{
  List<int> partition;
  int i;

  // put all lifetime in one partition
  partition.clear();
  for(i=0; i<file->life_num; i++) {
    partition.add_tail(i);
  } 

  // init prio list with a single partition
  prio.clear();
  prio.add_tail(partition);
}

void RR_order::Output_prio()
{
  result.clear();

  for(List_iterator<List<int> > partition(prio); partition != 0; partition++) {
    for(List_iterator<int>  element(*partition); element != 0; element++) {
      result.add_tail(*element);
    }
  }
}

ostream& operator << (ostream& os, RR_order& ro)
{
  os << "Prio : (";
  for(List_iterator<List<int> > partition(ro.prio); partition != 0; partition++) {
    os << "(";
    for(List_iterator<int>  element(*partition); element != 0; element++) {
      os << " " << *element;
    }
    os << ")";
  } 
  os << ")" << endl;
  return os;
}

////////////////////////////////////////////////////
// Order heuristics
////////////////////////////////////////////////////

int RR_order::Start_time_order(const int element)
{
  return (file->life[element].start_time);
}

int RR_order::Adjacency_order(const int element, const int last_element)
{
  int diff, dist, min_dist(infinity);
 
  if (last_element == undefined) {
    // return first start (most adjacent to left border)
    return (file->life[element].start_time);
  } else {
    // adjacency order with previous element
    diff = file->life[element].start_time - file->life[last_element].stop_time;
    
    // is there are legal overlaps, of how much?
    for(Hash_set_iterator<int> s(file->overlap_no_interference_loc[last_element][element]);
        s!=0;s++){
      dist = diff + *s * file->II;
      min_dist = ELCOR_MIN(min_dist, dist);
    }
    dist = diff + file->dist[last_element][element] * file->II;
    min_dist = ELCOR_MIN(min_dist, dist);
    return (min_dist);
  }
}

int RR_order::Conflict_order(const int element, const List<int>& partition)
{
  int res;

  res = 0;
  for(List_iterator<int>  el(partition); el != 0; el++) {
    res += file->conf[element][*el];
  }
  return (res);
}

////////////////////////////////////////////////////
// Order inference engin
////////////////////////////////////////////////////

void RR_order::Order_lifetime_by(const Order_strategy strategy)
{
  List<List<int> > new_prio; 
  List<int> new_partition;
  int cur = -1, min, previous_min, min_lt, previous_min_lt;

  new_prio.clear();
  for(List_iterator<List<int> > partition(prio); partition != 0; partition++) {
    previous_min = infinity;
    previous_min_lt = undefined;
    while (!(*partition).is_empty()) {
      // search for minimum
      min = infinity;
      for(List_iterator<int>  element(*partition); element != 0; element++) {
        if (strategy == start_time) {
          cur = Start_time_order(*element);
        } else if (strategy == adjacency) {
          cur = Adjacency_order(*element, previous_min_lt);
        } else if (strategy == conflict) {
          cur = Conflict_order(*element, *partition);
        } else {
          El_punt("Unkown order strategy\n");
        }
        if (cur < min) {
          min = cur;
          min_lt = *element;
        }
      }
      // is it a new partition in the new partition ?
      if (min != previous_min && !new_partition.is_empty()) {
          new_prio.add_tail(new_partition);
          new_partition.clear();
      }
      // record min element in new_partition
      new_partition.add_tail(min_lt);
      (*partition).remove(min_lt);
      previous_min = min;
      previous_min_lt = min_lt;
    }
  }
  // store last new_partition
  if (!new_partition.is_empty()) {
    new_prio.add_tail(new_partition);
  }
  prio = new_prio;
}

////////////////////////////////////////////////////
// class RR_state
//   used in the register allocation to compute the
//   number of physical register needed for the 
//   current (partial) register allocation.
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Method for RR_state class
////////////////////////////////////////////////////

RR_state::RR_state()  
{
  reg_num = undefined;
  top_wand_num = top_trail_blade_num = top_loc_num = top = -infinity;
  bottom_wand_num = bottom_lead_blade_num = bottom_loc_num = bottom = infinity;
}

RR_state& RR_state::operator= (const RR_state&  r)	// copy
{
  reg_num = r.reg_num;
  top_wand_num = r.top_wand_num;
  top_trail_blade_num = r.top_trail_blade_num;
  top_loc_num = r.top_loc_num;
  top = r.top;  
  bottom_wand_num = r.bottom_wand_num;
  bottom_lead_blade_num = r.bottom_lead_blade_num;
  bottom_loc_num = r.bottom_loc_num;
  bottom = r.bottom;

  return (*this);
}

RR_state::~RR_state() {}

char pp(int i)
{
  if (i==undefined) return '?';
  return (char) ((int) 'a' + i);
}

ostream& operator<< (ostream& os, RR_state& r)
{
  os << endl;
  os << "Register shape: ('?' == undefined)" << endl;
  os << "  top: " << r.top << " wand_num(" << r.top_wand_num;
  os << ") trail_blade_num(" << r.top_trail_blade_num;
  os << ") loc_num(" << r.top_loc_num << ")" << endl;
  os << "  bottom: " << r.bottom << " wand_num(" << r.bottom_wand_num;
  os << ") lead_blade_num(" << (char) r.bottom_lead_blade_num;
  os << ") loc_num(" << r.bottom_loc_num << ")" << endl;
  return os;
}

////////////////////////////////////////////////////
// Compute register requirements
////////////////////////////////////////////////////

// project the tuple <time, reg> on <projected_time, reg0>

int Projected_time(const int time, const int reg, const int II)
{
  return (time + reg * II);
}

void RR_state::Record(RR_file_data* file, const int sel)
{
  RR_lifetime& l = file->life[sel];
  int II = file->II;
  int t;

  // top
  int stop = l.stop_time;
  int loc = l.location;
  if (l.trailing_blade) {
    loc = l.location + l.alpha -1;
    stop = ELCOR_MAX(stop, ((file->stage_count + 1) - file->ES) * II);
  }
  // stop_time + last * II > 0
  int last = file->PS - ceil(stop, II) +1;
  t = loc - last;
  top = ELCOR_MAX(t, top);

  // bottom
  int start = l.start_time;
  loc = l.location;
  if (l.leading_blade) {
    start = ELCOR_MIN(start, file->PS * II);
  } 
  int first = file->PS - floor(start, II);
  t = loc - first;
  bottom = ELCOR_MIN(bottom, t);

  // wand
  t = Projected_time(l.stop_time, l.location, II);
  top_wand_num = ELCOR_MAX(top_wand_num, t);
  t =  Projected_time(l.start_time, l.location, II);
  bottom_wand_num = ELCOR_MIN(bottom_wand_num, t);

  // leading blade
  if (l.leading_blade) {
    t = l.location + 1;
    bottom_lead_blade_num = ELCOR_MIN(bottom_lead_blade_num, t);
  }   

  // trailing blade
  if (l.trailing_blade) {
    t = l.location + l.alpha -1;
    top_trail_blade_num = ELCOR_MAX(top_trail_blade_num, t);
  }

  // location
  t = l.location + l.k_first;
  top_loc_num = ELCOR_MAX(top_loc_num, t);
  t = l.location + l.k_last;
  bottom_loc_num = ELCOR_MIN(bottom_loc_num, t);
}

int RR_state::Requirements(RR_file_data* file)
{ 
  int  wand(0), lead(0), trail(0), reg;

  if (file->life_num <= 0) return 0;

  if (file->open_loop) {
    reg = top - bottom +1;
  } else {
    wand = ceil(top_wand_num - bottom_wand_num, file->II);
    if (bottom_lead_blade_num != infinity) {
      lead = top_loc_num - bottom_lead_blade_num +1;
    } 
    if (top_trail_blade_num != -infinity) {
      trail = top_trail_blade_num  - bottom_loc_num +1;
    }
    reg = ELCOR_MAX(ELCOR_MAX(lead, trail), wand);
  }
  return reg;
}

int RR_state::Requirements(RR_file_data* file, const List<int>& allocated_lt)
{ 
  int reg, r, d;

  reg = -infinity;
  for(List_iterator<int> l1(allocated_lt); l1 != 0; l1++) {
    for(List_iterator<int> l2(allocated_lt); l2 != 0; l2++) {
      d = file->life[*l2].location - file->life[*l1].location;
      if ( d >= 0) {
        // need only to test half the case because of symmetry
        r = d + file->dist[*l2][*l1];
        reg = ELCOR_MAX(reg, r);
      }
    }
  }

  int reg_fast = Requirements(file);
  if (reg_fast != reg) {
    cerr << "RR ERROR: reg(" << reg << ") reg_fast(" << reg_fast << ")" << endl;
  }
  return reg;
}

////////////////////////////////////////////////////
// class RR_space_time
//   used to check the register allocation by 
//   constructing a space time figure. may require
//   a LOT of memory. Used also to print space time figures.
////////////////////////////////////////////////////

RR_space_time::RR_space_time()
{
  reg_num = iter_num = begin_time = end_time = overlap_num = 0;
  mat =  Sp_matrix();
  file = 0;
  valid = false;
}

RR_space_time::~RR_space_time() {}

void RR_space_time::Set_spacetime(const int x, const int y, 
                              const int sel, const bool first)
{
  int sel_loc, el_loc;

  // neglect the long lifetime at the end of the file: cut it at the end of the
  // matrix
  if (y < begin_time || y >= end_time) {
    return;
  }
  for(List_iterator<RR_sp_element> el(mat[x][y]); el != 0; el++) {
    if ((*el).id == undefined) return;
    sel_loc = file->life[sel].location;
    el_loc = file->life[(*el).id].location;
    if (!file->Overlap_no_interference(sel,(*el).id, sel_loc, el_loc)) {
      // error: add and undefined element
      // cerr << "RR ERROR: spacetime[" << x << "][" << y << "] ";
      // cerr << "has already been allocated for lifetime " << (*el).id << endl;
      mat[x][y].add_head(RR_sp_element()); 
      valid = false;
      return;
    }
  }
  mat[x][y].add_tail(RR_sp_element(sel, first));
  overlap_num = ELCOR_MAX(overlap_num, int(mat[x][y].size()));
}

void RR_space_time::Set_lifetime(const int sel)
{
  int i, t;

  RR_lifetime& curr_lifetime = file->life[sel];
  int init_loc = curr_lifetime.location;
  int II = file->II;

  if (!curr_lifetime.allocated) return;

  // draw wand
  for(i=curr_lifetime.first; i<iter_num + curr_lifetime.last; i++) {
    int reg_loc = mod(init_loc - i, reg_num);
    int t_start = curr_lifetime.start_time + i * II;
    int t_end = curr_lifetime.stop_time  +i * II;

    for(t=t_start; t<t_end; t++) {
      Set_spacetime(reg_loc, t, sel, (bool) (i==0 || i==iter_num-1));
    }
  }

  if (!file->wand_only) {
    // leading blade  
    for(i=-curr_lifetime.omega; i<0; i++) {
      int reg_loc = mod(init_loc - i, reg_num);
      int t_start = 0;
      int t_end = curr_lifetime.start_time + i * II;
  
      for(t=t_start; t<t_end; t++) {
        Set_spacetime(reg_loc, t, sel, false);
      }
    }

    // trailing blade
    for(i=iter_num-curr_lifetime.alpha; i<iter_num; i++) {
      int reg_loc = mod(init_loc - i, reg_num);
      int t_start = curr_lifetime.stop_time  +i * II;
      int t_end = dimension;

      for(t=t_start; t<t_end; t++) {
        Set_spacetime(reg_loc, t, sel, false);
      }
    } 
  }
}

RR_space_time::RR_space_time(RR_file_data* l, const int reg, const int iter)
{
  int i;

  // init
  file = l;
  reg_num = reg;
  iter_num = iter;
  dimension = (iter + file->stage_count-1) * file->II;
  begin_time = file->PS * file->II;
  end_time = dimension - file->ES * file->II;
  mat =  Sp_matrix(reg_num, dimension);
  valid = true;
  overlap_num = 0;
 
  // check for case where reg_num <= 0 (not valid)
  if (reg <= 0) {
    valid = false;
    return;
  }

  // fill matrix
  for(i=0; i<file->life_num; i++){
    Set_lifetime(i);
  }
}

const char sp_start_line[20] = "    |";
 
void RR_space_time::print_time_axis(ostream& os, bool first)
{
  int i;

  // print axis
  if (!first) {
    os << sp_start_line;
    for(i=0; i<dimension; i++) os << "-";
    os << "|" << endl;
  }

  // print numbers
  os << sp_start_line;
  for(i=0; i<dimension; i++) {
    os << floor(mod(i, 100), 10);
  }
  os << endl << sp_start_line;
  for(i=0; i<dimension; i++) {
    os << mod(i, 10);
  }
  os << endl;

  // print axis
  if (first) {
    os << sp_start_line;
    for(i=0; i<dimension; i++) os << "-";
    os << "|" << endl;
  }
}

void RR_space_time::print_reg_axis(ostream& os, int r, bool left)
{
  if (left) {
    os.width(3); 
    os << r << " |";
  } else {
    os << "| " << r << endl;
  }
}

char  RR_space_time::get_char_to_print(int r, int t)
{
  char c;
  if (mat[r][t].is_empty()) {
    c = ' ';
  } else {
    int i = 0;
    int desired_el_num = mod(r, mat[r][t].size());
    RR_sp_element el;
    
    // undefined element, if any; otherwise, take desired element
    for(List_iterator<RR_sp_element> i1(mat[r][t]); i1!=0; i1++) {
      RR_sp_element curr_el = *i1;
      if (i++ == desired_el_num) el = curr_el;
      if (curr_el.id == undefined) {
        el = curr_el;
        break;
      }
    }
    if (el.id == undefined) {
      c = '?';
    } else if (el.cap) {
      c = char(int('A')+el.id);
    } else {
      c = char(int('a')+el.id);
    }
  }
  mat[r][t].clear();
  return c;
}

ostream& operator<< (ostream& os, RR_space_time& st)
{
  int II = st.file->II;
  int reg_num = st.reg_num;

  if ((st.file->life_num == 0) || 
      (st.file->life_num == 1 && st.file->open_loop)) {
    // there were no lifetimes
    return os;
  }
  os << endl;

  st.print_time_axis(os, true);

  if (st.file->open_loop) {
    int window_high = II;
    int window_low = - (reg_num-1) * II;
    int r = reg_num-1;
    while(window_low<st.dimension) {
      st.print_reg_axis(os, r, true);
      int t ;
      for(t=0; t<window_low; t++) os << "=";
      for(t=ELCOR_MAX(0, window_low); t<ELCOR_MIN(window_high, st.dimension); t++) {
	os << st.get_char_to_print(r, t);
      }
      for(t=window_high; t<st.dimension; t++) os << "=";
      st.print_reg_axis(os, r, false);
      window_low += II;
      window_high += II;
      r = mod(r-1, reg_num);
    } 
  } else {
    // rotating lines
    for(int r=reg_num-1; r>=0; r--) {
      st.print_reg_axis(os, r, true);
      for(int t=0; t<st.dimension; t++) {
	os << st.get_char_to_print(r, t);
      }
      st.print_reg_axis(os, r, false);
    }
  }

  st.print_time_axis(os, false);
  os << endl << endl;
  os << (st.valid ? "VALID" : "##NOT A VALID##") << " register allocation"
     << " with " << st.reg_num << " registers";
  os << " II(" << st.file->II << ")";
  os << " SC(" << st.file->stage_count << ")";  
  os << " PS(" << st.file->PS << ")";  
  os << " ES(" << st.file->ES << ")";  
  os << " wand(" << st.file->wand_only << ")";  
  os << endl;
  return os;
}  
