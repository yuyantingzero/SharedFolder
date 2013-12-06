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
//      File:           el_lit_histogram.cpp
//      Authors:        Marnix Arnold, Richard Johnson
//      Created:        June 1997
//      Description:    literal histogram
//
/////////////////////////////////////////////////////////////////////////////

#include "el_lit_histogram.h"
#include "iterators.h"
#include "opcode_properties.h"
#include "el_bb_tools.h"
#include "dbg.h"

// for the time being, assume all branch literals are 32 bits
#define BRLITS_VALUE_UNKNOWN
#define BOGUS_VALUE 0

// some 'local globals' for the histogram
Int_literal_histogram mem_hist;
Int_literal_histogram br_hist;
Int_literal_histogram int_hist;

FILE *El_literals_file;

//---------------------------------------------------------------------------
// Some member methods for integer histograms
//---------------------------------------------------------------------------
Int_literal_histogram::Int_literal_histogram()
  : stat_lit_histogram(hash_int, 1009), dyn_lit_histogram(hash_int, 1009)
{}

void 
Int_literal_histogram::add_value(int ival, double weight) {

  if (stat_lit_histogram.is_bound(ival)) {
    int stat = stat_lit_histogram.value(ival);
    stat_lit_histogram.bind(ival, stat+1);
  } else {
    stat_lit_histogram.bind(ival, 1);
  }

  if (dyn_lit_histogram.is_bound(ival)) {
    double dyn = dyn_lit_histogram.value(ival);
    dyn_lit_histogram.bind(ival, dyn+weight);
  } else {
    dyn_lit_histogram.bind(ival, weight);
  }
}

void Int_literal_histogram::print() {
  int i=0, j=0;

  if (dbg(genlit, 1)) cdbg << "Starting histogram sort." << endl;

  //
  // Move keys into an array, sort simultaneously
  //
  int *thekeys = new int[stat_lit_histogram.size()];

  for (Hash_map_iterator<int,int> mi(stat_lit_histogram); mi!=0; mi++,i++) {
    thekeys[i] = (*mi).first;
    for(j=0; j<i; j++) {
      int a = thekeys[j];
      int b = thekeys[i];
      if(a > b) { // swap
	thekeys[j] = b; thekeys[i] = a;
      }
    }
  }

  //
  // Print out the histogram
  //
  for(i=0; i < (int)stat_lit_histogram.size(); i++) {
    int a = thekeys[i];
    int stat = stat_lit_histogram.value(a);
    double dyn = dyn_lit_histogram.value(a);
    fprintf(El_literals_file, "   %10i  %6i  %7.0f\n", a, stat, dyn);
  }

  delete [] thekeys;
}

//---------------------------------------------------------------------------
//  Return a list of pointers to all ops with literals in a region
//---------------------------------------------------------------------------
static List<Op*>
get_ops_with_literals(Compound_region *r) {
  Op *current_op;
  List<Op*> ops_with_lits;

  // iterate over all ops, make list of ops that have literal(s) as operand
  for(Region_all_ops ops_iter(r); ops_iter != 0; ops_iter++) {
    current_op = *ops_iter;
    // iterate over all source operands of current operation
    for(Op_explicit_sources src_iter(current_op); src_iter != 0; src_iter++) {
      if((*src_iter).is_lit()) {
	ops_with_lits.add_tail(current_op);
	break;
      }
    }
  }
  return ops_with_lits;
}


//---------------------------------------------------------------------------
//  Iterate over a compound region and make a histogram of literal values
//---------------------------------------------------------------------------
void
do_literal_histogram(Compound_region* r) {
  Op *current_op;
  Operand current_oper;
  Compound_region *reg;
  int intlit_val;
  double weight;
  List<Op*> ops_with_lits = get_ops_with_literals(r);
  Int_literal_histogram *cur_inthist;

  El_form_basic_blocks(r);

  if (dbg(genlit, 1))
    cdbg << "Starting literal histogram construction." << endl;

  // make a histogram
  // iterate over all operations in the list
  for(List_iterator<Op*> list_iter(ops_with_lits); list_iter!=0; list_iter++) {
    current_op = *list_iter;
    reg = (Compound_region *)current_op->parent();
    if (! reg->is_bb())
      El_punt("El_do_lit_histogram: op parent should be BB");
    weight = reg->weight;
    
    for(Op_explicit_sources src_iter(current_op); src_iter != 0; src_iter++) {
      current_oper = *src_iter;
      if(current_oper.is_lit())	{
	if(current_oper.is_int()) {
	  intlit_val = current_oper.int_value();
	  
	  if(is_pbr(current_op)) // select histogram
	    cur_inthist = &br_hist;
	  else
	    if(is_memory(current_op))
	      cur_inthist = &mem_hist;
	    else
	      cur_inthist = &int_hist;
	  
	  cur_inthist->add_value(intlit_val, weight);
	} else if(current_oper.is_float()) {
	  // do nothing
	}
      } // current_oper.is_lit()
    } // for all operands
  } // for all list entries

  El_remove_basic_blocks(r); // clean up after ourselves
}


//---------------------------------------------------------------------------
//  Print the histogram on stdout
//---------------------------------------------------------------------------
void
print_literal_histogram() {
  El_literals_file = fopen("ELCOR_LITERALS", "w");

  fprintf(El_literals_file, "mem_literals {\n");
  mem_hist.print();
  fprintf(El_literals_file, "}\n\n");

  fprintf(El_literals_file, "branch_literals {\n");
  br_hist.print();
  fprintf(El_literals_file, "}\n\n");

  fprintf(El_literals_file, "int_literals {\n");
  int_hist.print();
  fprintf(El_literals_file, "}\n\n");

  fclose(El_literals_file);
}
