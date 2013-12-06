/***********************************************************************

Copyright (c) 2007 Massachusetts Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

***********************************************************************/



//===========================================================================
//
//   FILE: rmsl_ext.cpp:
//   
//   Author: Sam Larsen
//   Date: Wed Apr 20 21:41:02 2005
//
//   Function:  Copied from rmsl.cpp and extended to allow for removal of
//		operations from bins.
//
//===========================================================================

#include "rmsl_ext.h"

static double MOMENT = 2.0;


static inline int ceiling(int x, int y) {
  int result = x / y;
  if (result*y<x) result++;
  return result;
}

static inline double calc_fval(int cycles, int count)
{
  int most = (cycles + count-1) / count;
  int numH = cycles - count * (most-1);
  int numL = count - numH;
  return (numH * pow((double)most, MOMENT) + 
	  numL * pow((double)(most-1), MOMENT));
}

Rmsl_ext::Rmsl_ext(MDES* md_in)
  : md(md_in), cum_vect(md->get_MDES_num_resources(), 0)
{}
  
Rmsl_ext::~Rmsl_ext() {}

void Rmsl_ext::reset()
{
  cum_vect = Vector<int>(md->get_MDES_num_resources(), 0);
}

// Copied from mdes.cpp and modified
void Rmsl_ext::nextop_io(char* opcode, char* io){
  Res_use_descr *res_use, *descr_head, *min_res_use;
  Io_descr request_iod(md);
  Io_list *iol_elem;
  Io_descr* iod_elem;
  Alt_descr* altd_elem;
  char* min_aname;
  int ires, i;
  int ival, min_ival;
  double fval, min_fval;

  // We tally the opcode and its resource usage into the cumulative
  // resource requirement vector. The strategy is to select that
  // alternative that minimizes number of cycles required. However, if
  // the lower_bound is non-zero, we consume at least that many
  // resource-cycles before moving to the next alternative in priority.

  // parse operation and its io descr
  int num_resources = md->get_MDES_num_resources();
  Op_descr *operation = (md->get_MDES_hashtable())->find_op(opcode);
  if(!operation) El_punt("Rmsl_ext::nextop: couldn't find operation %s", opcode);
  request_iod.parse_iospec(io); 

  min_ival=infinity;                      // initialize minimum resource count
  min_fval=(double)infinity;
  min_res_use=(Res_use_descr*) NULL;      // initial best resource
  Vector<int> total_vect(num_resources);  // scratch resource usage totals

  while((iol_elem=operation->get_next_io())!=0) { 
    iod_elem=iol_elem->get_iod();         // iterate over io_desc of operation
    if(iod_elem->test_io(&request_iod)){  // match the requested iod

      // First suitable io descriptor is used to select sched alternative
      // We pick a "best" alternative (that minimally increases ResMII)
      iol_elem->init_alt(); 
      while((altd_elem=iol_elem->get_next_alt())!=0){
 	   
	// Calculate resource accumulation from this alternative
	descr_head=altd_elem->get_res_use(); 
	for(i=0;i<num_resources; i++) total_vect[i]=cum_vect[i];
	res_use=descr_head;
	while(res_use) { 
	  ires = res_use->get_res();
	  // increment count irrespective of time
	  total_vect[ires] += res_use->get_count();
	  res_use = res_use->get_next();
	} 

	// Calculate ResMII induced from this alt and save if minimum.
	// For counted resources, we take the ceiling of used_res/avail_res
	for (ival=0, fval=0.0, i=0; i<num_resources; i++) {
	  int count = md->get_MDES_res_descr(i)->get_count();
	  int cycles = ceiling(total_vect[i], count);
	  if (cycles>ival) ival = cycles;
	  fval += calc_fval(total_vect[i], count);
	}

	if (ival < min_ival || (ival == min_ival && fval < min_fval)) {
	  min_ival=ival;
	  min_fval=fval;
	  min_res_use=descr_head;
	  min_aname = altd_elem->get_op()->get_aname_ptr();
	}
      }
//      if (dbg(mdes,2)) 
//	printf("RMSL: op '%s' : Choosing alt '%s' with value=%.2f\n", 
//	       opcode, min_aname, min_ival);

      // Accumulate the best case pattern
      res_use=min_res_use; 
      while(res_use){ 
	ires = res_use->get_res();
	cum_vect[ires] += res_use->get_count();
	res_use = res_use->get_next();
      }

      return;
    }
	
  }
  El_punt("MDES (Rmsl_ext::nextop): No alternative of '%s' matches io format '%s'.",
	  opcode, io);
}

void Rmsl_ext::remove_io(char* opcode, char* io){
  Res_use_descr *res_use, *descr_head, *min_res_use;
  Io_descr request_iod(md);
  Io_list *iol_elem;
  Io_descr* iod_elem;
  Alt_descr* altd_elem;
  char* min_aname;
  int ires, i;
  int ival, min_ival;
  double fval, min_fval;

  // parse operation and its io descr
  int num_resources = md->get_MDES_num_resources();
  Op_descr *operation = (md->get_MDES_hashtable())->find_op(opcode);
  if(!operation) El_punt("Rmsl_ext::remove: couldn't find operation %s", opcode);
  request_iod.parse_iospec(io); 

  min_ival=infinity;                      // initialize minimum resource count
  min_fval=(double)infinity;
  min_res_use=(Res_use_descr*) NULL;      // initial best resource
  Vector<int> total_vect(num_resources);  // scratch resource usage totals

  while((iol_elem=operation->get_next_io())!=0) { 
    iod_elem=iol_elem->get_iod();         // iterate over io_desc of operation
    if(iod_elem->test_io(&request_iod)){  // match the requested iod

      // First suitable io descriptor is used to select sched alternative
      // We pick a "best" alternative (that maximally decreases ResMII)
      iol_elem->init_alt(); 
      while((altd_elem=iol_elem->get_next_alt())!=0){
 	   
	// Calculate resource accumulation from this alternative
	descr_head=altd_elem->get_res_use(); 
	for(i=0;i<num_resources; i++) total_vect[i]=cum_vect[i];
	res_use=descr_head;
	while(res_use) { 
	  ires = res_use->get_res();
	  // decrement count irrespective of time
	  total_vect[ires] -= res_use->get_count();
	  if (total_vect[ires] < 0) break;
	  res_use = res_use->get_next();
	}
	// If resource usage has been decremented past 0, go on to
	// the next alternative.
	if (res_use) continue;

	// Calculate ResMII induced from this alt and save if minimum.
	// For counted resources, we take the ceiling of used_res/avail_res
	for (ival=0, fval=0.0, i=0; i<num_resources; i++) {
	  int count = md->get_MDES_res_descr(i)->get_count();
	  int cycles = ceiling(total_vect[i], count);
	  if (cycles>ival) ival = cycles;
	  fval += calc_fval(total_vect[i], count);
	}

	if (ival < min_ival || (ival == min_ival && fval < min_fval)) {
	  min_ival=ival; 
	  min_fval=fval;
	  min_res_use=descr_head;
	  min_aname = altd_elem->get_op()->get_aname_ptr();
	}
      }
//      if (dbg(mdes,2)) 
//	printf("RMSL: op '%s' : Removing alt '%s' with value=%.2f\n", 
//	       opcode, min_aname, min_ival);

      if (min_ival == infinity)
	El_punt("MDES (Rmsl_ext::remove): something bad has happened\n");

      // Remove the best case pattern
      res_use=min_res_use; 
      while(res_use){ 
	ires = res_use->get_res();
	cum_vect[ires] -= res_use->get_count();
	res_use = res_use->get_next();
      }
      return;
    }
	
  }
  El_punt("MDES (Rmsl_ext::remove): No alternative of '%s' matches io format '%s'.",
	  opcode, io);
}

int Rmsl_ext::value(double& fval_out) {
  int i;
  int ival = 0;
  double fval = 0.0;
  for(i=0; i<(md->get_MDES_num_resources()); i++) {
    int count = md->get_MDES_res_descr(i)->get_count();
    int cycles = ceiling(cum_vect[i], count);
    if (cycles > ival) ival = cycles;
    fval += calc_fval(cum_vect[i], count);
  }

  fval_out = fval;
  return ival;
}

void Rmsl_ext::print() {
  for (int j=0; j<cum_vect.dim(); j++) {
    cerr << md->get_MDES_res_descr(j)->get_name() << "\t";
  }
  cerr << "\n";
  for (int i=0; i<cum_vect.dim(); i++) {
    int count = md->get_MDES_res_descr(i)->get_count();
    int cycles = ceiling(cum_vect[i], count);
    cerr << cycles;
    if (count > 1) cerr << " (" << cum_vect[i] << ")";
    cerr << "\t";
  }
  cerr << "\n";
}

char* Rmsl_ext::most_used() {
  int max = 0;
  int id = 0;
  for (int i=0; i<cum_vect.dim(); i++) {
    int cycles = ceiling(cum_vect[i], md->get_MDES_res_descr(i)->get_count());
    if (cycles > max) {
      max = cycles;
      id = i;
    }
  }

  return md->get_MDES_res_descr(id)->get_name();
}
