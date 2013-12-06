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
//      File:           print_sched.cpp
//      Authors:        Santosh G. Abraham
//      Created:        November 1995
//      Description:    Print out scheduling region after scheduling
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>

#include "iterators.h"
#include "opcode_properties.h"
#include "el_ssched_init.h"
#include "dbg.h"
#include "operand.h"
#include "print_sched.h"

#include "ls_get_profile.h"

using namespace std;

void
print_operand (Operand oper, ostringstream& buf)
{
  Operand* oper_ptr;
  
  oper_ptr = &oper;
  
  if (oper_ptr->is_reg()) {
      buf << oper_ptr->dtype() << "r" << oper_ptr->vr_num();  return;}
  if (oper_ptr->is_macro_reg()) {
      buf << oper_ptr->dtype() << "m" << oper_ptr->vr_num();  return;}
  if (oper_ptr->is_lit()) {
      buf << oper_ptr->dtype() << "l" << oper_ptr->vr_num();  return;}
  if (oper_ptr->is_undefined()) {
    return;}
  assert(0);
  return;
}


void
print_op (Op* op, ostringstream& buf1, ostringstream& buf2, int wid1, int wid2)
{
  Operand oper;
  int srcs, dests, i;
  int init_wid, curr_wid, fill_wid, opc_wid;
  const char SPC = ' ';
  //char opc_str[80];
  ostringstream opc_buf;

  init_wid = buf1.str().size();
  buf1.width(4);  buf1 << op->id() << " ";
  curr_wid = buf1.str().size();

  opc_wid = wid1 - (curr_wid - init_wid) - 5;
  
  opc_buf.setf(ios::left, ios::adjustfield);
  opc_buf.width(opc_wid-1); opc_buf << op->opcode();
  char opc_str[80];
  strcpy(opc_str, opc_buf.str().c_str());
  if ((int)strlen(opc_str) > opc_wid) {
    opc_str[opc_wid - 1] = '*';
    opc_str[opc_wid]     = '\0';
  }
  buf1 << opc_str;

  oper = op->src(PRED1);
  if (!oper.is_undefined()) {
    buf1 << " ";
    print_operand (oper, buf1);
  }
  curr_wid = buf1.str().size();
  fill_wid = wid1 - (curr_wid - init_wid);
  // assert (fill_wid >= 0);
  if (fill_wid > 0) {
    for (i = 0; i<fill_wid; i++) {
      buf1 << SPC;
    }
  }
  

  char oper_str[80];
  ostringstream oper_buf;

  init_wid = buf2.str().size();
  dests = op->num_dests();
  oper_buf << " [";
  for (i = 0; i<dests; i++)
    {
      if (i != 0) oper_buf << " ";
      oper = op->dest((Port_num) (i + DEST1));
      print_operand(oper, oper_buf);
    }
  oper_buf << "]";
      
  srcs = op->num_srcs();
  oper_buf << " [";
  for (i = 0; i<srcs; i++)
    {
      if (i != 0) oper_buf << " ";
      oper = op->src((Port_num) (i + SRC1));
      print_operand(oper, oper_buf);
    }
  oper_buf << "]";
  strcpy(oper_str, oper_buf.str().c_str());
  if ((int)strlen(oper_str) > wid2) {
    oper_str[wid2 - 1] = '*';
    oper_str[wid2]     = '\0';
  }
  buf2 << oper_str;

  curr_wid = buf2.str().size();
  fill_wid = wid2 - (curr_wid - init_wid);
  //  assert (fill_wid >= 0);
  if (fill_wid > 0) {
    for (i = 0; i<fill_wid; i++) {
      buf2 << SPC;
    }
  }
}

/*
void
print_opcode (Op* op, int width)
{
  Operand oper;
  int srcs, dests, i;
  const char SPC = ' ';

  cout << op->id() << " ";
  cout.width(3); cout << op->opcode();
  oper = op->src(PRED1);
  if (!oper.is_undefined()) {
    cout << " ";
    print_operand (oper);
  }
  cout << endl;

  dests = op->num_dests();
  cout << " [";
  for (i = 0; i<dests; i++)
    {
      if (i != 0) cout << " ";
      oper = op->dest((Port_num) (i + DEST1));
      print_operand(oper);
    }
  cout << "]";
      
  srcs = op->num_srcs();
  cout << " [";
  for (i = 0; i<srcs; i++)
    {
      if (i != 0) cout << " ";
      oper = op->src((Port_num) (i + SRC1));
      print_operand(oper);
    }
  cout << "]";
  cout << endl;
}
*/

void print_sched(Compound_region* reg) {

  int i, j;

  const int OP_WIDTH = 21;
  const int OPS_PERLINE = 6;
  const int LINE_LEN = (OP_WIDTH + 1) * OPS_PERLINE + 12;

  const int MEMOPS_PERLINE = 2;
  const int NMOPS_PERLINE = OPS_PERLINE - MEMOPS_PERLINE;

  const char VSEP = '|';

  char hline[LINE_LEN+1];
  for (i = 0; i < LINE_LEN; i++) {
    hline[i] = '-';
  }
  hline[i] = '\0';

  assert (reg->is_bb() || reg->is_hb());

  if (reg->is_bb()) cout << "Schedule for BB:" << reg->id();
  if (reg->is_hb()) cout << "Schedule for HB:" << reg->id();
  cout << endl ;

  // Header info
  cout << hline << endl;
  cout << "Cyc " << VSEP;
  for (j = 0; j < MEMOPS_PERLINE; j++) {
    //       123456789
    cout << "   MemOp " ;
    cout.width(1) ;
    //		  12345678901
    cout << j << "           " << VSEP;
  }
  for (j = 0; j < (OPS_PERLINE - MEMOPS_PERLINE); j++) {
    //       1234567890123           
    cout << "    OtherOps " ;
    cout.width(1);
    //		  5678901
    cout << j << "       " << VSEP;
  }
  //       1234567
  cout << "MoreOps" << VSEP << endl;
  // Total width = 5 + 6*20 + 8 = 133
  cout << hline << endl;

  Region_ops_C0_order oi (reg);
  int cycle = -1;
  while (oi != 0) {
    cycle++;
    //    cerr << "Processing for Cycle: " << cycle << endl;
    cout.width(4);     cout << cycle << VSEP;
    {
    ostringstream mbuf1, mbuf2, buf1, buf2;

    int mops = 0;
    int ops = 0;
    int wid1, wid2;
    while ((oi != 0) && ((*oi)->sched_time() == cycle)) {
      Op* op = (*oi);
      oi++;
      //      cerr << "  Processing for Op id: " << op->id() << endl;
      if (is_pseudo(op)) continue;
      if (is_memory(op)) {
	mops++;
	if (mops <= MEMOPS_PERLINE) {	
	  wid1 = ((OP_WIDTH + 1) * mops) - 1 -  mbuf1.str().size();
	  wid2 = ((OP_WIDTH + 1) * mops) - 1 -  mbuf2.str().size();
	  print_op (op, mbuf1, mbuf2, wid1, wid2);
	  mbuf1 << VSEP;
	  mbuf2 << VSEP;
	}
      } else {
	ops++;
	if (ops <= NMOPS_PERLINE) {
	  wid1 = ((OP_WIDTH + 1) * ops) - 1 -  buf1.str().size();
	  wid2 = ((OP_WIDTH + 1) * ops) - 1 -  buf2.str().size();
	  print_op (op, buf1, buf2, wid1, wid2);
	  buf1 << VSEP;
	  buf2 << VSEP;
	}
      }
    }
    if (mops < MEMOPS_PERLINE) {
      for (i=0; i<(MEMOPS_PERLINE-mops); i++) {
	//        123456789012345678901234
	mbuf1 << "                     " << VSEP;
	mbuf2 << "                     " << VSEP;
      }
    }
    if (ops < NMOPS_PERLINE) {
      for (i=0; i<(NMOPS_PERLINE-ops); i++) {
	//       123456789012345678901234
	buf1 << "                     " << VSEP;
	buf2 << "                     " << VSEP;
      }
    }

    cout << mbuf1.str() << buf1.str();
    if (ops - NMOPS_PERLINE > 0) {
      cout << "***";
      cout.width(1); cout << (ops-NMOPS_PERLINE) << "***" << VSEP << endl;
    } else {
      //       1234567
      cout << "       " << VSEP << endl;
    }
    // Second line of cycle output
    //       1234
    cout << "    " << VSEP;
    //                            1234567
    cout << mbuf2.str() << buf2.str() << "       " << VSEP << endl;
    cout << hline << endl;

    }
  }
}
