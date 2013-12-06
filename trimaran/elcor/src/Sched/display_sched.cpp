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
//      File:           display_sched.cpp
//      Authors:        Santosh G. Abraham, Richard Johnson
//      Created:        March 1995
//      Description:    Visualize hyperblocks after/before scheduling
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include "region.h"
#include "iterators.h"
#include "tuples.h"
#include "attributes.h"
#include "op_info.h"
#include "el_ssched_init.h"
#include "opcode_properties.h"
#include "edge.h"

using namespace std;

#include "daVinci-interface.h"
daVinci gui;

/*
 *
 *
Map<Opcode,eString> lcode_map ;

void initialize_OpToLcodeMap() {
   eString tmp_str;
   Opcode tmp_opc;
   for(Map_iterator<eString, Opcode> iter(opcode_map); iter != 0; iter++){
      tmp_str = (*iter).first;
      tmp_opc = (*iter).second;
      lcode_map.bind(tmp_opc, tmp_str);
   }
}
*
*
*/


void
format_operand (Operand oper, ostringstream& buf)
{
  Operand* oper_ptr;
  
  oper_ptr = &oper;
  
  if (oper_ptr->is_reg()) {
    buf <<  "(r " << oper_ptr->vr_num() << ' ' << oper_ptr->dtype() << ')'; 
    return;
  }
  if (oper_ptr->is_macro_reg()) {
    buf << "(mac " << oper_ptr->name() << ' ' << oper_ptr->dtype() << ')'; 
    return;
  }
  if (oper_ptr->is_lit()) {
    buf << '(' << oper_ptr->dtype() << ' ' << oper_ptr->int_value() << " )";   
    return;
  }
  if (oper_ptr->is_undefined()) {
    return;}
  assert(0);
  return;
}
     
void
format_op (Op* op, ostringstream& buf)
{
  Operand oper;
  int srcs, dests, i;
  
  buf << op->id() << "    " << op->opcode();

  if (El_ssched_display_operands) {
      dests = op->num_dests();
      buf << " [";
      for (i = 0; i<dests; i++)
	{
	  oper = op->dest((Port_num) (i + DEST1));
	  format_operand(oper, buf);
	}
      buf << "]";
      
      srcs = op->num_srcs();
      buf << " [";
      for (i = 0; i<srcs; i++)
	{
	  oper = op->src((Port_num) (i + SRC1));
	  format_operand(oper, buf);
	}
      buf << "]";
   }
      
      if (opInfoMap->is_bound(op)) {
         OpInfo* info = opInfoMap->value(op);
         buf << "\\nET: " << info->get_etime() <<
               "  P: " << info->get_priority();
      }
      buf << ends;
}


Map<int,int> expand_bb ;

void
display (Compound_region* blk)
{
  int blk_id;
  Operand tmp_oper;
  int edge_count = 0;
  char* color;
  char* sep3 = NULL;
  
  gui.font_size(10);
  gui << "new_term_placed([";
  char *sep = (char*)"";
  assert ( blk->is_bb() || blk->is_hb() );
  blk_id = blk->id();
  for(Region_ops_C0_order oiter(blk) ; oiter != 0 ; oiter++)
  {
      bool has_relop;
      bool first_op = true;
      List<Op*> ops ;
      ops.add_tail(*oiter);
      if ((is_control_switch(*oiter)) || (is_control_merge(*oiter))) {
 	 // add relops to ops list
	 for (List_iterator<Op*> ri((*oiter)->get_relops()); ri != 0; ri++) {
	   ops.add_tail(*ri);
	 }
	 has_relop = true;
      }
      for (List_iterator<Op*> piter(ops); piter != 0; piter++) {
         if (El_ssched_display_relops || first_op) {
	    Op* op = (*oiter);
	    if (El_ssched_display_relops) op = (*piter);
            // describe node for op
            char *opstr = new char[1024];
            ostringstream buf;
            const int boxlen = 20;
            format_op (op, buf);
            strcpy(opstr, buf.str().c_str());
            // pad string to fixed-length with spaces
            if ((int) strlen (opstr) < boxlen)
            {
	       int s ;
   	       for (s=strlen (opstr); s<boxlen; s++)
		   opstr[s] = ' ';
	       opstr[s] = '\0';
             }
             color = (char*)"cyan";
             if (opInfoMap->is_bound(*oiter)) {
                color = (char*)"lightred";
		OpInfo* info = opInfoMap->value(*oiter);
		if (info->is_scheduled())      color = (char*)"lightblue";
		if (info->is_data_ready())     color = (char*)"lightgreen";
		if (info->is_data_ready_now()) color = (char*)"green";
	     }
	     cerr << opstr << endl;
            gui << sep << "l(\"op " << op->id()
		<< "\",n(\"\",[a(\"OBJECT\",\"" << opstr
		<< "\"),a(\"COLOR\",\"" << color << "\")"
		<< ",a(\"FONTFAMILY\",\"courier\")],[";
	    sep = (char*)",";
            delete opstr;
            sep3 = (char*)"";
         }
         for(Op_outedges iter(*piter); iter != 0; ++iter) {
	    Op* succ_op = (*iter)->dest();
	    if (region_contains_op (blk, succ_op)) {
	       if ((!El_ssched_display_relops)
			&& (is_switch(succ_op) || is_merge(succ_op))) {
		  List_iterator<Op*> relsmop ((succ_op)->get_relops());
		  succ_op = (*relsmop);
               }
	       if ((*iter)->is_reg_flow()) {
		  gui << sep3 << "l(\"e " << edge_count++
			<< "\",e(\"\",[a(\"EDGECOLOR\",\"red\")],r(\"op "
			<< succ_op->id() << "\")))"; sep3 = (char*)","; }
	       if ((*iter)->is_reg_anti()) {
		  gui << sep3 << "l(\"e " << edge_count++
	     	  	<< "\",e(\"\",[a(\"EDGECOLOR\",\"orange\")],r(\"op "
		  	<< succ_op->id() << "\")))"; sep3 = (char*)","; }
	       if ((*iter)->is_reg_out()) {
		  gui << sep3 << "l(\"e " << edge_count++
	     	  	<< "\",e(\"\",[a(\"EDGECOLOR\",\"green\")],r(\"op "
		  	<< succ_op->id() << "\")))"; sep3 = (char*)","; }
	       if ((*iter)->is_mem()) {
		  gui << sep3 << "l(\"e " << edge_count++
	     	  	<< "\",e(\"\",[a(\"EDGECOLOR\",\"magenta\")],r(\"op "
		  	<< succ_op->id() << "\")))"; sep3 = (char*)","; }
	       if ((*iter)->is_control()) {
		  switch ((*iter)->src_port()) {
	             case CONTROL0:
		       if (El_ssched_display_C0edges) {
		          gui << sep3 << "l(\"e " << edge_count++
			  << "\",e(\"\",[a(\"EDGECOLOR\",\"lightblue\")],r(\"op "
			  << succ_op->id() << "\")))"; sep3 = (char*)",";
		       }
		       break;
		     case CONTROL1:
		        gui << sep3 << "l(\"e " << edge_count++
			   << "\",e(\"\",[a(\"EDGECOLOR\",\"blue\")],r(\"op "
			   << succ_op->id() << "\")))"; sep3 = (char*)",";
		        break;
		     case CONTROL2:
		        gui << sep3 << "l(\"e " << edge_count++
			   << "\",e(\"\",[a(\"EDGECOLOR\",\"violet\")],r(\"op "
			   << succ_op->id() << "\")))"; sep3 = (char*)",";
		        break;
		     default:
		        cerr << "Op: " << endl;
		        cerr << (*(*piter)) << endl << "Edge: " << endl;
		        cerr << (*(*iter));
		        assert((*iter)->src_port() <= CONTROL2);
	          }		    
              }
           }
        }
	if (El_ssched_display_relops) gui << "]))";
	first_op = false;
     }
       if (!El_ssched_display_relops) gui << "]))";
  }
  
  List<Op*> ops_inf;
  int length, num, sorttime, time_id;
  bool first;
  
  length = 0;
  
  for (Region_ops_C0_order op_iter(blk); op_iter != 0; op_iter++) {
           Op* op = (*op_iter);
           if (opInfoMap->is_bound(op)) {
	        ops_inf.add_head(op); length++;}
  }

  time_id = 0;
  num = 0;
  sorttime = 0;
  while (num < length) {
     first = true;
     for ( List_iterator<Op*> liter(ops_inf); liter !=0; ++liter) {
	assert(opInfoMap->is_bound(*liter));
	OpInfo* info = opInfoMap->value(*liter);
	if (info->get_etime() == sorttime) {
// gui stuff to put out invisible edges to ops with etime = sorttime
// and to next time node
	   num++;
	   if (first) {
	      gui << ",l(\"t " << time_id
		<< "\",n(\"\",[a(\"OBJECT\",\"Time: " << sorttime
		  << "\"),a(\"COLOR\",\"white\")"
		    << ",a(\"FONTFAMILY\",\"courier\")],[";
	      time_id++;
	      gui << "l(\"e " << edge_count++
	     	   << "\",e(\"\",[a(\"EDGECOLOR\",\"white\")],r(\"t "
		   << time_id << "\")))";
	      first = false;
	     }
	    gui << ",l(\"e " << edge_count++
	     	   << "\",e(\"\",[a(\"EDGECOLOR\",\"white\")],r(\"op "
		   << (*liter)->id() << "\")))";
         }
     }
     if (!first) gui << "]))";
     sorttime++;
  }
  gui << ",l(\"t " << time_id
	<< "\",n(\"\",[a(\"OBJECT\",\"Time: " << sorttime
	<< "\"),a(\"COLOR\",\"white\")"
	<< ",a(\"FONTFAMILY\",\"courier\")],[";
  gui << "]))";

  gui << "])\n";
  
}

void
display_sched (Compound_region *f)
{
  bool redisplay = true;
//  char buf[10000];
  char *msg, *cmd;
  
  if (!(El_ssched_graph_display)) return;
  display (f);
  
  while (redisplay)
    {
      msg = gui.get_msg();
      
      cmd = (char*)"node_selections_labels([\"";
      if (strncmp (msg, cmd, strlen (cmd)) == 0)
	{
	  char *pt = msg+strlen(cmd);
	  *strchr(pt, '"') = '\0';	// terminate substring at end of node
	  if (*pt == 'n')
	    {
	      int nd;

	      pt+=2;
	      sscanf(pt, "%d", &nd);
	      
	      int val = 1-expand_bb.value(nd);
	      expand_bb.unbind(nd).bind(nd, val);
	      display (f);
	    } else {
	    redisplay = false;
	    }
	}
    }
}

// end of draw-cfg.cpp
