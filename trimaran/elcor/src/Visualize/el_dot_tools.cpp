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
//      File:           el_dot_tools.cpp
//      Authors:        Sadun Anik, Shail Aditya
//      Created:        November 1995
//      Description:    Tools to display control-flow and data-flow
//                      graphs using dot and ghostview
//
/////////////////////////////////////////////////////////////////////////////

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include "el_dot_tools.h"
#include "defs.h"
#include "op.h"
#include "edge.h"
#include "iterators.h"
#include "edge_utilities.h"
#include "edge_attributes.h"
#include "attributes.h"
#include "opcode_properties.h"
#include "el_init.h"
#include "dbg.h"
#include "mdes.h"
#include "intf.h"
#include "el_system.h"

using namespace std;

extern eString data_type_to_text (DType d) ;
extern Reg_file data_type_to_file (DType d) ;

//static void el_dot_print_file_header(ostream& os, char* orntn, char* name) ;
//static void el_dot_print_file_tail(ostream& os) ;
//static void el_dot_process_dot_file(char* file_name, char* orntn) ;

static int cGraphNumber = 0;

 void el_dot_print_file_header(ostream& os, char* orntn, char* name) ;
 void el_dot_print_file_tail(ostream& os) ;
 void el_dot_process_dot_file(char* file_name, char* orntn, int bg=false) ;

char* getGraphFilename(char* origFilename) {

   char* Counted_el_dot_graph_filename = new char[strlen(origFilename)+6];
   if (cGraphNumber > 99999)	// if you're making more than 10000 graphs, you have to
	   cGraphNumber = -1;		// fix this yourself.  This is just an easy way to get a
								// filename without finding the length (in digits) of
								// cGraphNumber

   sprintf(Counted_el_dot_graph_filename, "%s_%d", origFilename, cGraphNumber++);
   return Counted_el_dot_graph_filename;

}

void el_dot_display_cfg(Region* r)
{
   int j;

   if (!El_print_dot_graph) return ;
   char* Counted_el_dot_graph_filename = getGraphFilename(El_dot_graph_filename);
   ofstream os(Counted_el_dot_graph_filename) ;
//   ofstream os(El_dot_graph_filename) ;
   
   char* orientation ;
   char name[256] ;
   Alist_region_tiling_graph gr(r, GRAPH_HBS_BBS) ;
   
   if (!strcmp(El_dot_graph_orientation, "portrait")) {
      orientation = (char*)"portrait" ;
   }
   else if (!strcmp(El_dot_graph_orientation, "landscape")) {
      orientation = (char*)"landscape" ;
   }
   else {                          // Default for CFGs
      orientation = (char*)"portrait" ;
   }
   if (r->is_procedure()) {
      strcpy(name, (char*)((Procedure*)r)->get_name()) ;
   }
   else {
      sprintf(name,"Region_%d",r->id()) ;
   }
   el_dot_print_file_header(os, orientation, name) ;

   for(j = 0 ; j < (int)gr.b_map.size() ; j++ ) {
      Region* srcr = gr.b_map[j] ;
      if (srcr->is_bb()) {
	 os << "BB_" ;
      }
      else {
	 os << "HB_" ;	    
      }
      os << srcr->id() << " [ " ;
      os << " fontname = \"Helvetica\" " ;
      os << ", label = \"" ;
      if (srcr->is_bb()) {
	 os << "BB " ;
      }
      else {
	 os << "HB " ;	    
      }
      os << srcr->id() << "\"" ;
      os << " ] ; \n" ;
   }
   
   
   for(j = 0 ; j < (int)gr.b_map.size() ; j++ ) {
      Region* srcr = gr.b_map[j] ;
      for (List_iterator<int> li(gr.succ_list[j]) ; li != 0 ; li++) {
	 os << "  " ;
	 Region* destr = gr.b_map[*li] ;
	 Edge* out_edge = get_connecting_CONTROL0_edge(srcr,destr) ;
	 if (out_edge == NULL) El_warn("Region hierarchy is not well connected") ;
	 if (srcr->is_bb()) {
	    os << "BB_" ;
	 }
	 else {
	    os << "HB_" ;	    
	 }
	 os << srcr->id() << " -> " ;
	 if (destr->is_bb()) {
	    os << "BB_" ;
	 }
	 else {
	    os << "HB_" ;	    
	 }
	 os << destr->id() ;
	 if (out_edge) {
	    if (get_control_flow_freq(out_edge)) {
	       os << "[label = " << get_control_flow_freq(out_edge)->freq << "]";
	    }
	 }
	 os <<  " ; \n" ;
      }
   }
   el_dot_print_file_tail(os) ;
   os.close();
//   el_dot_process_dot_file(El_dot_graph_filename,orientation) ;
   el_dot_process_dot_file(Counted_el_dot_graph_filename, orientation);
   delete [] Counted_el_dot_graph_filename;
}


void el_dot_print_file_header(ostream& os, char* orntn, char* graph_name)
{
   os << "digraph " << graph_name << " {\n" ;
   os << "  graph [ nslimit = \"1000.0\",\n" ;
   os << "          orientation = " <<  orntn << ", \n" ;
   os << "          center = true, \n" ;
   if (!strcmp(El_dot_graph_size,"large")) {
      os << "          page = \"11,17\",\n" ;
      if (strcmp(orntn,"landscape")) {
	 os << "          size = \"9.5,15\" ] ; \n" ;
      } else {
	 os << "          size = \"15,9.5\" ] ; \n" ;
      }
   } else if (!strcmp(El_dot_graph_size,"Large")) {
      os << "          page = \"24,36\",\n" ;
      if (strcmp(orntn,"landscape")) {
	 os << "          size = \"22,33\" ] ; \n" ;
      } else {
	 os << "          size = \"33,22\" ] ; \n" ;
      }
   } else if (!strcmp(El_dot_graph_size,"LARGE")) {
      os << "          page = \"36,48\",\n" ;
      if (strcmp(orntn,"landscape")) {
	 os << "          size = \"33,44\" ] ; \n" ;
      } else {
	 os << "          size = \"44,33\" ] ; \n" ;
      }
   } else {
      os << "          page = \"8.5,11\",\n" ;
      if (strcmp(orntn,"landscape")) {
	 os << "          size = \"7.5,10\" ] ; \n" ;
      } else {
	 os << "          size = \"10,7.5\" ] ; \n" ;
      }
   }
}

void el_dot_print_file_tail(ostream& os)
{
   os << "}" << endl ;
}

void el_dot_process_dot_file(char* file_name, char* orientation, int bg)
{
   char command[256] ;
   
   sprintf(command,"dot -Tps -o %s.ps %s",file_name,file_name) ;
   el_system(command) ;
   
   sprintf(command,"rm -f %s.eps",file_name) ;
   el_system(command) ;

   sprintf(command,"sed -e 's/PS-Adobe-2.0/PS-Adobe-2.0 EPSF-2.0/' %s.ps > %s.eps", file_name, file_name) ;
   el_system(command) ;

   /* ghostview no longer seems to have the orientation command-line argument.
    * And the .ps seems to get displayed in properly anyway..
    * KVM, Feb 2007
    */
   if (bg==true)
       sprintf(command,"ghostview %s.ps &",
	       file_name) ;
   else
       sprintf(command,"ghostview %s.ps",
	       file_name) ;

   el_system(command) ;
}

void el_dot_display_dfg(Region* r)
{
   if (!El_print_dot_graph) return ;
   char* Counted_el_dot_graph_filename = getGraphFilename(El_dot_graph_filename);
   ofstream os(Counted_el_dot_graph_filename) ;

   char* orientation ;
   char name[256] ;
   Region_all_ops op_iter ;

   if (!strcmp(El_dot_graph_orientation, "portrait")) {
      orientation = (char*)"portrait" ;
   }
   else if (!strcmp(El_dot_graph_orientation, "landscape")) {
      orientation = (char*)"landscape" ;
   }
   else {                          // Default for DFGs
      orientation = (char*)"landscape" ;
   }

   sprintf(name,"Region_%d",r->id()) ;
   el_dot_print_file_header(os,orientation,name) ;

   //
   // If the region is scheduled then a schedule time line is generated 
   //
   int maxtime = minus_infinity;
   int mintime = infinity;
   if (r->flag(EL_REGION_SCHEDULED)) {
     // initialize max/min sched times
     for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
       Op* op = *op_iter;
       if (is_pseudo(op) && !El_dot_pseudo_ops) continue;
       // use unrolled SWP time if possible -- Shail Aditya 10/01/97
       int* sched_time_ptr = (int*)get_generic_attribute(op, "unrolled_sched_time");
       int sched_time = (sched_time_ptr) ? *sched_time_ptr : op->sched_time();
       maxtime = ELCOR_MAX(maxtime, sched_time);
       mintime = ELCOR_MIN(mintime, sched_time);
     }
     // output time line
     int i,j;
     os << "node [ shape=plaintext, fontsize=24 ];\n";
     // output 10 time nodes per line (Dot parser overflows otherwise)
     for(i=mintime; i<(maxtime/10)*10; i+=10) {
       for(j=0; j<10 ; j++) os << i+j << " -> ";
       os << i+10 << " [ style = invis ];\n";
     }
     // output remaining nodes if any
     if (i<maxtime) {
       for(;i<maxtime; i++) os << i << " -> ";
       os << maxtime << " [ style = invis ];\n";
     }
   }

   //
   // Output ops 
   //
   for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
      Op* srcop = *op_iter ;
      if (is_pseudo(srcop) && !El_dot_pseudo_ops) continue;

      os << "\"Op_" << srcop->id() << "\" " ;
      os << "[" ;
      os << " fontname = \"Helvetica\" shape = box" ;
      if (is_switch(srcop) || is_merge(srcop)) {
	 if (is_switch(srcop)) {
	   os << ", label = \"";
	   if (El_dot_verbose) {
	     Operand d1 = srcop->dest(DEST1);
	     Operand u1 = srcop->src(SRC1);
	     if (d1.is_reg() || d1.is_macro_reg())  os << d1;
	     os << "=" ;
	     if (u1.is_reg() || u1.is_macro_reg())  os << u1;
	   } else {
	     os << "s" << srcop->id();
	   }
	   os << "\"" ;
	 }
	 else {
	   os << ", label = \"";
	   if (El_dot_verbose) {
	      if (is_remap(srcop)) {
		 Operand& tmp_oprnd = srcop->src(SRC1) ;
		 os << "REMAP(" ;
		 os << vregfile_to_mdesname(data_type_to_file
					  (tmp_oprnd.dtype())) ;
		 os << " " << tmp_oprnd.vr_num() ;
		 os << ")" ;
	      }
	      else {
		 Operand d1 = srcop->dest(DEST1);
		 Operand u1 = srcop->src(SRC1);
		 if (d1.is_reg() || d1.is_macro_reg())  os << d1;
		 os << "=" ;
		 if (u1.is_reg() || u1.is_macro_reg())  os << u1;
	      }
	   } else {
	     os << "m" << srcop->id();
	   }
	   os << "\"" ;
	 }
	 os << ", width = 0.5" ;
      }
      else {
	os << ", label = \"";
	if (El_dot_verbose) {
	  const int buflen = 256;
	  char ss[buflen];
	  char st[buflen];
	  ostringstream oss;
	  oss << *srcop;
          strncpy(ss, oss.str().c_str(), buflen);

	  // replace first newline with end-of-string, then print string
	  char* p = strchr(ss, '\n');
	  if (p) *p = '\0';
	  //
	  // Insert escape character before any double-quote in the
	  // string.
	  //
	  int ssi = 0 ;
	  int sti = 0 ;
	  while (ss[ssi] != '\0') {
	     if (ss[ssi] == '\"') {
		st[sti] = '\\' ;
		sti++ ;
	     }
	     st[sti] = ss[ssi] ;
	     sti++ ;
	     ssi++ ;
	  }
	  os << st ;
	} else {
	  os << srcop->id() << " ";
	  if (El_dot_schedop_labels && srcop->flag(EL_REGION_SCHEDULED)) {
	    os << (char *) srcop->sched_opcode();
          }
	  else {
            //os << (char *) el_opcode_to_string_map.value(srcop->opcode());
            os << get_mdes_opcode_string(srcop);
          }
        }
	os << "\"" ;
      }
      os << "] ; \n" ;
   }

   //
   // if the region is scheduled, rank all ops according to schedule
   //
   if (r->flag(EL_REGION_SCHEDULED)) {
     Vector<List<Op*> > ops(maxtime-mintime+1);
     for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
       Op* op = *op_iter;
       if (is_pseudo(op) && !El_dot_pseudo_ops) continue;
       // use unrolled SWP time if possible -- Shail Aditya 10/01/97
       int* sched_time_ptr = (int*)get_generic_attribute(op, "unrolled_sched_time");
       int sched_time = (sched_time_ptr) ? *sched_time_ptr : op->sched_time();
       ops[sched_time-mintime].add_tail(op);
     }
     for(int i=mintime; i<=maxtime; i++) {
       os << "{ rank = same ; " << i << "; ";
       for(List_iterator<Op*> iter(ops[i-mintime]); iter!=0; iter++) {
	 if (is_pseudo(*iter) && !El_dot_pseudo_ops) continue;
	 os << "\"Op_" << (*iter)->id() << "\"; "; 
       }
       os << "} \n";
     }
   } 

   //
   // If the op has related ops, put them at the same rank
   //
   else {
     for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++ ) {
       Op* srcop = *op_iter ;
       if (is_pseudo(srcop) && !El_dot_pseudo_ops) continue;
       if (!srcop->get_relops().is_empty()) {
	 // os << "subgraph \"cluster_" << srcop->id() <<"\"\n" ;
	 os << "{ rank = same ; \n" ;
	 // os << "set = cluster ;\n" ;
	 os << "\"Op_" << srcop->id() <<"\"; " ;
	 for (List_iterator<Op*> relopi(srcop->get_relops()) ;
	      relopi != 0 ; relopi++) {
	   if (is_pseudo(*relopi) && !El_dot_pseudo_ops) continue;
	   os << "\"Op_" << (*relopi)->id() << "\"; \n" ;
	 }
	 os << "} \n" ;
       }
     }
   }

   //
   // Output edges
   //
   for (Region_all_internal_edges edgei(r) ; edgei != 0 ; edgei++) {
      Edge* e = * edgei ;
      if (El_dot_c0 == 0 && e->is_control()) continue ;
      if (El_dot_mem == 0 && e->is_mem()) continue ;
      if (El_dot_flow == 0 && e->is_reg_flow()) continue ;
      if (El_dot_anti == 0 && e->is_reg_anti()) continue ;
      if (El_dot_out == 0 && e->is_reg_out()) continue ;

      if (!El_dot_pseudo_ops && 
	  (is_pseudo(e->src()) || is_pseudo(e->dest()))) continue;

      if (!El_dot_brf_edges && 
	  (is_brf(e->src()) || is_brf(e->dest()))) continue;

      os << "\"Op_" << e->src()->id() << "\"" ;
      os << " -> " ;
      os << "\"Op_" << e->dest()->id() << "\" " ;
      if (e->is_control()) {
	 os << "[style = bold]" ;
      }
      else if (e->is_mem()) {
	 os << "[color = green]" ;
      }
      else if (e->is_reg_flow()) {
	 os << "[style = bold, color = red]" ;
      }
      else if (e->is_reg_anti()) {
	 os << "[color = blue]" ;  //suren: cant make out pink from red
	 //os << "[color = pink]" ;
      }
      else if (e->is_reg_out()) {
	 os << "[style = dotted, color = red]" ;
      }
      os << " ;\n" ;
   }
   el_dot_print_file_tail(os) ;
   os.close();
   el_dot_process_dot_file(Counted_el_dot_graph_filename,orientation) ;
   delete [] Counted_el_dot_graph_filename;
}


void el_dot_display_cops(Region* r, const List<List<Op*> >& cop_insts)
{
  Region_all_ops op_iter ;

  // Do a quick check to keep from displaying empty graphs.
  unsigned useful_ops = 0;
  for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++) {
    Op* srcop = *op_iter ;
    if ( ! is_pseudo(srcop))
      useful_ops++;
  }

  if(useful_ops <= 1)
    return;

  char* Counted_el_dot_graph_filename = getGraphFilename(El_dot_graph_filename);
  ofstream os(Counted_el_dot_graph_filename) ;

  char* orientation ;
  char name[256] ;

  if (!strcmp(El_dot_graph_orientation, "portrait")) {
    orientation = (char*)"portrait" ;
  } else if (!strcmp(El_dot_graph_orientation, "landscape")) {
    orientation = (char*)"landscape" ;
  } else {                          // Default for DFGs
    orientation = (char*)"landscape" ;
  }

  sprintf(name,"Region_%d",r->id()) ;
  el_dot_print_file_header(os,orientation,name) ;

  Hash_set<Op*> cop_ops(hash_op_ptr, 67);
  for(List_iterator<List<Op*> > li(cop_insts); li != 0; li++) {
    for(List_iterator<Op*> li2(*li); li2 != 0; li2++) {
      cop_ops += *li2;
    }
  }

  //
  // Output ops 
  //

  for(op_iter((Compound_region*)r) ; op_iter != 0 ; op_iter++) {
    Op* srcop = *op_iter ;
    if(is_pseudo(srcop))
      if( ! is_switch(srcop) && ! is_merge(srcop))
	continue;

    if(is_pbr(srcop))
      continue;

    os << "\"Op_" << srcop->id() << "\" " ;
    os << "[" ;
    os << " fontname = \"Helvetica\" shape = ellipse" ;
    // os << " fontname = \"Helvetica\" shape = box" ;
    if(cop_ops.is_member(srcop))
      os << " fillcolor = \"red\" style = filled";
    if (is_switch(srcop) || is_merge(srcop)) {
      os << ", label = \"";
      if(is_switch(srcop)) {
	os << "Live Out";
      } else if(is_merge(srcop)) {
	os << "Live In";
      }
      os << "\", width = 0.5" ;
    } else {
      os << ", label = \"";
      if (El_dot_verbose) {
	const int buflen = 256;
	char ss[buflen];
	char st[buflen];
	ostringstream oss;
	oss << *srcop;
	strncpy(ss, oss.str().c_str(), buflen);

	// replace first newline with end-of-string, then print string
	char* p = strchr(ss, '\n');
	if (p) *p = '\0';
	//
	// Insert escape character before any double-quote in the
	// string.
	//
	int ssi = 0 ;
	int sti = 0 ;
	while (ss[ssi] != '\0') {
	  if (ss[ssi] == '\"') {
	    st[sti] = '\\' ;
	    sti++ ;
	  }
	  st[sti] = ss[ssi] ;
	  sti++ ;
	  ssi++ ;
	}
	os << st ;
      } else {
	os << srcop->id() << " ";
	// ntclark. 
	// os << get_mdes_opcode_string(srcop);
	char* tmp = get_mdes_opcode_string(srcop);
	if(strstr(tmp, "_C1")) {
	  if(strstr(tmp, "L")) {
	    os << "LD";
	  } else if(strstr(tmp, "S")) {
	    os << "ST";
	  } else {
	    os << tmp;
	    }
	} else {
	  if(strstr(tmp, "CMP")) {
	    os << "CMP";
	  } else {
	    os << tmp;
	  }
	}
      }
      os << "\"" ;
    }
    os << "] ; \n" ;
  }

  //
  // Output edges
  //
  for (Region_all_internal_edges edgei(r) ; edgei != 0 ; edgei++) {
    Edge* e = * edgei ;
    if (e->is_control()) continue ;
    if (e->is_mem()) continue ;
    if (e->is_reg_anti()) continue ;
    if (e->is_reg_out()) continue ;

    if (is_pseudo(e->src()))
      if( ! is_merge(e->src()) && ! is_switch(e->src()))
	continue;
    if (is_pseudo(e->dest()))
      if( ! is_merge(e->dest()) && ! is_switch(e->dest()))
	continue;

    if(is_pbr(e->src()) || is_pbr(e->dest()))
      continue;

    if ((is_brf(e->src()) || is_brf(e->dest()))) continue;

    os << "\"Op_" << e->src()->id() << "\"" ;
    os << " -> " ;
    os << "\"Op_" << e->dest()->id() << "\" " ;
    if (e->is_control()) {
      os << "[style = bold]" ;
    }
    else if (e->is_mem()) {
      os << "[color = green]" ;
    }
    else if (e->is_reg_flow()) {
      os << "[style = bold, color = blue]" ;
    }
    else if (e->is_reg_anti()) {
      os << "[color = blue]" ;
    }
    else if (e->is_reg_out()) {
      os << "[style = dotted, color = red]" ;
    }
    os << " ;\n" ;
  }

  //
  // Output each instance as a group
  //
  unsigned subg_num = 0;
  for(List_iterator<List<Op*> > li(cop_insts); li != 0; li++) {
    os << "subgraph cluster" << subg_num++ << " {\n\t";
    for(List_iterator<Op*> li2(*li); li2 != 0; li2++) {
      os << "\"Op_" << (*li2)->id() << "\"; ";
    }
    os << "\n}" << endl;
  }

  el_dot_print_file_tail(os) ;
  os.close();
  el_dot_process_dot_file(Counted_el_dot_graph_filename,orientation) ;
  delete [] Counted_el_dot_graph_filename;
}


void el_dot_display_alist_region_tiling_graph(Alist_region_tiling_graph* gr)
{
   int j;

   if (!El_print_dot_graph) return ;
   char* Counted_el_dot_graph_filename = getGraphFilename(El_dot_graph_filename);
   ofstream os(Counted_el_dot_graph_filename) ;
//   ofstream os(El_dot_graph_filename) ;
   
   char* orientation ;
   char name[256] ;
   
   if (!strcmp(El_dot_graph_orientation, "portrait")) {
      orientation = (char*)"portrait" ;
   }
   else if (!strcmp(El_dot_graph_orientation, "landscape")) {
      orientation = (char*)"landscape" ;
   }
   else {                          // Default for CFGs
      orientation = (char*)"portrait" ;
   }
   sprintf(name,"Plain") ;
   el_dot_print_file_header(os, orientation, name) ;

   for(j = 0 ; j < (int)gr->b_map.size() ; j++ ) {
      os << "Node_" << j ;
      os << " [ fontname = \"Helvetica\" " ;
      os << ", label = \"" << j << "\"" ;
      os << " ] ; \n" ;
   }
   
   
   for(j = 0 ; j < (int) gr->b_map.size() ; j++ ) {
      for (List_iterator<int> li(gr->succ_list[j]) ; li != 0 ; li++) {
	 os << "  " ;
	 int dest = *li ;
	 os << "Node_" << j << " -> Node_" << dest <<  " ; \n" ;
      }
   }
   el_dot_print_file_tail(os) ;
   os.close();
   el_dot_process_dot_file(Counted_el_dot_graph_filename,orientation) ;
   delete [] Counted_el_dot_graph_filename;
}

void el_dot_display_alist_dfg_graph(El_dfg* gr)
{
   int j;

   if (!El_print_dot_graph) return ;
   char* Counted_el_dot_graph_filename = getGraphFilename(El_dot_graph_filename);
   ofstream os(Counted_el_dot_graph_filename) ;
//   ofstream os(El_dot_graph_filename) ;
   
   char* orientation ;
   char name[256] ;
   
   if (!strcmp(El_dot_graph_orientation, "portrait")) {
      orientation = (char*)"portrait" ;
   }
   else if (!strcmp(El_dot_graph_orientation, "landscape")) {
      orientation = (char*)"landscape" ;
   }
   else {                          // Default for CFGs
      orientation = (char*)"portrait" ;
   }
   sprintf(name,"Plain") ;
   el_dot_print_file_header(os, orientation, name) ;

   for(j = 0 ; j < gr->g.node_count ; j++ ) {
      Op* srcop = (Op*) gr->g.b_map[j] ;
      os << "Node_" << j ;
      os << " [ fontname = \"Helvetica\" " ;
      if (gr->slack[j] == 0) {
	 os << ", color = red " ;
      }
      os << ", label = \"" << srcop->id() << " ";
      //os << (char *) el_opcode_to_string_map.value(srcop->opcode());
      os << get_mdes_opcode_string(srcop);
      os << "\"" ;
      os << " ] ; \n" ;
   }

   {
      // print the ranks
      int max_time = 0 ;
      for(j = 0 ; j < gr->g.node_count ; j++ ) {
	 if (max_time < gr->etime[j]) max_time = gr->etime[j] ;
      }
      max_time++ ;
      Vector<List<int> > ops(max_time);
      for(j = 0 ; j < gr->g.node_count ; j++ ) {
	 ops[gr->etime[j]].add_tail(j) ;
      }
      for(j=0 ; j < max_time; j++) {
	 os << "{ rank = same ; " << j << "; ";
	 for(List_iterator<int> iter(ops[j]); iter!=0; iter++)
	    os << "\"Node_" << *iter << "\"; "; 
	 os << "} \n";
      }
      for(j=1 ; j < max_time ; j++) {
	 os << j-1 << " -> " << j << " ; \n" ;
      }
   }
   
   
   for(j = 0 ; j < gr->g.edge_count ; j++ ) {
      int src_op_int = gr->g.edge_src_node_map[j] ;
      int dest_op_int = gr->g.edge_dest_node_map[j] ;
      os << "  " ;
      os << "Node_" << src_op_int << " -> Node_" << dest_op_int ;
      os << " [ label = " << (gr->latency)[j] << " ] "  ;
      os <<  " ; \n" ;
   }
   el_dot_print_file_tail(os) ;
   os.close();
   el_dot_process_dot_file(Counted_el_dot_graph_filename,orientation) ;
   delete [] Counted_el_dot_graph_filename;
}

// end of el_dot_tools.cpp
