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
//      File:           mdes_reader.cpp
//      Authors:        Shail Aditya
//      Created:        January 1996
//      Description:    LMDES2 customizer(loader) into Elcor
//
//
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"
#include "string_class.h"
#include "hash_map.h"
#include "list.h"
#include "mdes.h"
#include "macro.h"
#define MD_DEBUG_MACROS
#include "md.h"
#include "el_driver_init.h"

using namespace std;

//#define REGMASK(i) (1<<(i))

// FILE *MDES_out=stdout;
// List<FILE*> MDES_out_stack;
ofstream *MDES_out = NULL;
List<ofstream*> MDES_out_stack;

static void build_reg_descr_kind(Reg_descr* reg_desc, MD_Entry* entry);
static void build_reg_descr_sets(Reg_descr* reg_desc, MD_Entry* entry);
static int get_latency_time(MD_Entry *entry, MD_Field_Decl *decl, int index);
void process_properties(MD_Entry *opcs_entry, 
      ResClass *resclass, OpClass *opclass, bool *eager);
Alt_descr* make_alt_desc(MDES* mdesc, MD_Entry *alt_entry, Op_descr *opux);

/////////////////////////////////////////////////////////////////////////////
// MD Utilities

/* Assign each entry in a section an index.  
 * This index is used by get_entry_index().  -JCG 5/24/96
 */
static void assign_entry_indexes (MD_Section *section)
{
    MD_Entry *entry;
    MD_Field_Decl *index_field_decl;
    MD_Field *index_field;
    int index;
    
    /* Punt if there is already a field declared */
    if ((index_field_decl = MD_find_field_decl (section, "index")) != NULL)
      El_punt ("assign_entry_indexes: Section '%s' already has index field!",
    section->name);
    
    /* Create required index field with one INT */
    index_field_decl = MD_new_field_decl (section, "index", MD_REQUIRED_FIELD);
    MD_require_int (index_field_decl, 0);
    
    /* Add index to each entry, starting with 0 */
    index = 0;
    for (entry = MD_first_entry (section); entry != NULL;
   entry = MD_next_entry (entry))
    {
  /* Create index field */
  index_field = MD_new_field (entry, index_field_decl, 1);
  MD_set_int (index_field, 0, index);
  
  index++;
    }
}

/* Get the index for an entry.  If the entry has not been assigned
 * an index already, assign indexes to the whole section. 
 * Used to emulate MD_entry_index(). -JCG 5/24/96
 */
int get_entry_index (MD_Entry *entry)
{
    MD_Field_Decl *index_field_decl;
    MD_Field *index_field;
    int index;

    /* Get the index field decl.  Assign indexes to whole section
     * if this index field is not present.
     */
    index_field_decl = MD_find_field_decl (entry->section, "index");
    if (index_field_decl == NULL)
    {
  /* Create an index for all entries in the entry's section */
  assign_entry_indexes (entry->section);

  /* Get the newly created index field declaration */
  index_field_decl = MD_find_field_decl (entry->section, "index");

  if (index_field_decl == NULL)
      El_punt ("get_entry_index: assign_entry_indexes didn't work!");
    }
    
    /* Get the index for this entry */
    index_field = MD_find_field (entry, index_field_decl);
    index = MD_get_int (index_field, 0);

    return (index);
}

MD* El_read_md(char* mdfile) 
{
  FILE *mdstream;
  mdstream = fopen(mdfile, "r");
  if (!mdstream)
    El_punt("MD: Can not open LMDES file '%s'", mdfile);
  MD *md = MD_read_md(mdstream, mdfile);
  assert(MD_check_md(stderr, md) == 0);
  fclose(mdstream);
  return md;
}

//void push_MDES_out(FILE* file) {
void push_MDES_out(ofstream* file) {
  MDES_out_stack.push(MDES_out);
  MDES_out = file;
}

void pop_MDES_out(void) {
  MDES_out = MDES_out_stack.pop();
}


/////////////////////////////////////////////////////////////////////////////
// MDES Reader

MDES::MDES(char *mdes_input, char *mdes_output, int hashsize) 
  : input_file(strdup(mdes_input)), hashtable(new MdesHashtable(this, hashsize)), 
    num_vreg_types(0), macro_to_index_map(hash_estring, 1000),
    res_str_to_descr_map(hash_estring, 1000),
    name_to_const_desc_map(hash_estring, 32),
    macro_to_pf_map(hash_macro, 32),
    alias_macros(hash_estring, 8),
    pseudo_macros(hash_estring, 8),
    read_only_macros(hash_estring, 8),
    multialias_macros(hash_estring, 8),
    coherent_macros(hash_estring, 8)
{
  // Read in the lmdes description
  MD *md = El_read_md(mdes_input);

  // debugging
  if (dbg(mdes)) {
    push_MDES_out(new ofstream(mdes_output, ios::app));
    *MDES_out << "****MDES from input file: " << input_file << endl;
  }

  // setup resource table
  build_resource_table(md);

  // setup reservation table
  build_reservation_table(md);

  // setup latency table
  build_latency_table(md);

  // setup constants
  build_const_set_table(md);

  // setup register table
  build_register_table(md);

  // setup macro table
  build_macro_table(md);

  // setup stack descriptor table
  build_stack_descr_table(md);

  // setup field type table
  build_ftdescr_table(md);

  // setup io descriptor table
  build_iodescr_table(md);

  // setup ops
  build_opdescr_table(md);

  if(El_do_prepass_custom_ops || El_do_postpass_custom_ops){
    // This first call inserts the operation, scheduling alternative,
    // etc. into the already existing MDES data structures.
    build_custom_opdescr_table(md);

    // And this second call sets up data structures to describe the
    // dataflow pattern executed by the custom instruction.
    build_custom_op_table(md);
  }

  MD_delete_md(md);

  // debugging
  if (dbg(mdes)) { 
    print();
    MDES_out->close();
    delete MDES_out;
    pop_MDES_out();
  }
}


// Cop_operand_desc* get_cop_operand(MD* md,MDES* mdes, MD_Entry* operand_entry){
//   MD_Section *operand_section = MD_find_section(md, "Operand_Hookup");
//   MD_Field_Decl* decl6 = MD_find_field_decl(operand_section, "int_values");
//   MD_Field_Decl* decl7 = MD_find_field_decl(operand_section, "flags");

//   Cop_operand_desc* operand = new Cop_operand_desc(mdes);
//   char* name = strdup(operand_entry->name);
//   operand->set_name(name);
  
//   MD_Field* int_value_field = MD_find_field(operand_entry, decl6);
//   if(int_value_field==NULL){
//     operand->set_type(COP_REG);
//   }else{
//     operand->set_type(COP_HC_LIT);
//     for(int m1=0;m1<MD_num_elements(int_value_field);m1++){
//       int hc_int=MD_get_int(int_value_field,m1);
//       operand->add_int_value(hc_int);
//     }
//   }

//   MD_Field* flag_field = MD_find_field(operand_entry, decl7);
//   if(flag_field==NULL){
//     operand->set_live_in(false);
//     operand->set_live_out(false);
//   }else{
//     operand->set_live_in(false);
//     operand->set_live_out(false);
//     for(int m1=0;m1<MD_num_elements(flag_field);m1++){
//                 MD_Entry* flag_entry = MD_get_link(flag_field, m1);
//       char* flag=flag_entry->name;
//       if(strcmp(flag,"LIVE_IN")==0)
//         operand->set_live_in(true);
//       else if(strcmp(flag,"LIVE_OUT")==0)
//         operand->set_live_out(true);
//     }
//   }
//   return operand;
// }


void MDES::build_custom_op_table(MD *md)
{
  MD_Section *customop_section = MD_find_section(md, "Custom_Operation");
  MD_Field_Decl* decl0 = MD_find_field_decl(customop_section, "graph");

  MD_Section *graph_section = MD_find_section(md, "Customop_Graph");
  MD_Field_Decl* decl1 = MD_find_field_decl(graph_section, "nodes");

  MD_Section *node_section = MD_find_section(md, "Pattern_Node");
  MD_Field_Decl* decl2 = MD_find_field_decl(node_section, "op");
  MD_Field_Decl* decl3 = MD_find_field_decl(node_section, "pred");
  MD_Field_Decl* decl4 = MD_find_field_decl(node_section, "src");
  MD_Field_Decl* decl5 = MD_find_field_decl(node_section, "dest");
  MD_Field_Decl* decl6 = MD_find_field_decl(node_section, "flags");
  MD_Field_Decl* decl7 = MD_find_field_decl(node_section, "const");

  // FIX: consider adding some error checking here. For now we just
  // assume a well formatted MDES, which may create some odd errors if
  // that assumption is false.

  // Determine the number of fields in the Customop_Operation section
  // of the mdes
  MD_Entry* customop_entry = MD_first_entry(customop_section);
  custom_op_tab.resize(MD_num_entries(customop_section));

  for(int i = 0; i < MD_num_entries(customop_section); 
      i++, customop_entry = MD_next_entry(customop_entry)) {

    Custom_op_descr* descr = new Custom_op_descr(this);
    // MD_set_entry_ext(customop_entry, customop_semantics);
    descr->set_opcode(customop_entry->name);

    MD_Field* graph_field = MD_find_field(customop_entry, decl0);
    MD_Entry* graph_entry = MD_get_link(graph_field,0);

    MD_Field* nodes_field = MD_find_field(graph_entry, decl1);

    descr->set_pattern_size(MD_num_elements(nodes_field));

    Hash_map<eString, Pattern_edge*> edges(hash_estring, 31);
    for(int j = 0; j < MD_num_elements(nodes_field); j++) {
      Pattern_node* node = new Pattern_node(this);

      MD_Entry* node_entry = MD_get_link(nodes_field, j);
      descr->add_pattern_node(node, j);

      // Build up the Pattern_node class. Note that we're not really
      // doing much with the Pattern_edge class, except using the
      // names. The reason this class was included is for future use,
      // for example if we wanted to represent anti-dependence edges
      // in patterns for reduced encoding (i.e. the src and dst in a
      // pattern are the same register). Another example would be to
      // encode some type of bit width attribute on the edges.

      MD_Field* op_field   = MD_find_field(node_entry, decl2);
      MD_Field* pred_field = MD_find_field(node_entry, decl3);
      MD_Field* src_field  = MD_find_field(node_entry, decl4);
      MD_Field* dest_field = MD_find_field(node_entry, decl5);
      MD_Field* flags_field = MD_find_field(node_entry, decl6);
      MD_Field* const_field = MD_find_field(node_entry, decl7);

      // The opcode
      if(op_field) {
	node->set_type(COP_OPERATION);
	for(int m = 0; m < MD_num_elements(op_field); m++) {
	  MD_Entry* op_entry = MD_get_link(op_field, m);
	  node->add_opcode(eString(op_entry->name));
	}
      }

      // The next three if-blocks sets up the graph.
      if(pred_field) {
	for(int m = 0; m < MD_num_elements(pred_field); m++) {
	  MD_Entry* pred_entry = MD_get_link(pred_field, m);
	  eString pname(pred_entry->name);
	  Pattern_edge* ed;
	  if(edges.is_bound(pname)) {
	    ed = edges[pname];
	  } else {
	    ed = new Pattern_edge(this);
	    edges.bind(pname, ed);
	  }
	  ed->add_dest(node);
	  node->set_pred(ed);
	}
      }

      if(src_field) {
	for(int m = 0; m < MD_num_elements(src_field); m++) {
	  MD_Entry* src_entry = MD_get_link(src_field, m);
	  eString sname(src_entry->name);
	  Pattern_edge* ed;
	  if(edges.is_bound(sname)) {
	    ed = edges[sname];
	  } else {
	    ed = new Pattern_edge(this);
	    edges.bind(sname, ed);
	  }
	  ed->add_dest(node);
	  node->set_src(ed, m);
	}
      }

      if(dest_field) {
	for(int m = 0; m < MD_num_elements(dest_field); m++) {
	  MD_Entry* dest_entry = MD_get_link(dest_field, m);
	  eString dname(dest_entry->name);
	  Pattern_edge* ed;
	  if(edges.is_bound(dname)) {
	    ed = edges[dname];
	  } else {
	    ed = new Pattern_edge(this);
	    edges.bind(dname, ed);
	  }
	  ed->add_src(node);
	  node->set_dest(ed, m);
	}
      }

      if(flags_field) {
	for(int m = 0; m < MD_num_elements(flags_field); m ++) {
	  MD_Entry* flag_entry = MD_get_link(flags_field, m);
	  char* flag = flag_entry->name;
	  if(strcasecmp(flag, "live_in") == 0) {
	    node->set_type(COP_LIVE_IN);
	  } else if(strcasecmp(flag, "live_out") == 0) {
	    node->set_type(COP_LIVE_OUT);
	  }
	}
      }

      if(const_field) {
	node->set_type(COP_LITERAL);
	MD_Entry* cset_entry = MD_get_link(const_field, 0);
	eString cset_name = cset_entry->name;
	node->set_const_descr(name_to_const_desc_map[cset_name]);
      }

    }
    custom_op_tab[i] = descr;
  }
}


void MDES::build_resource_table(MD *md) {
  int i;
  MD_Entry  *res_entry;
  Res_descr *res_desc;
  char* name;

  MD_Section *res_section = MD_find_section(md, "Resource");
  res_descr_tab.resize(MD_num_entries(res_section));
  MD_Field_Decl* count_decl = MD_find_field_decl(res_section, "count");
  MD_Field_Decl* width_decl = MD_find_field_decl(res_section, "width");
  MD_Field_Decl* virtual_decl = MD_find_field_decl(res_section, "virtual");

  // BRC July 1, 1996: display decl & field is optional and may not be
  // present.  Deleted the "display" field and added "count" field for
  // counted resources -- Shail Aditya 08/11/98

  // 0 = int, 1 = float, 2 = mem, 3 = branch

  for (i=0, res_entry=MD_first_entry(res_section); 
       res_entry != NULL; i++, res_entry=MD_next_entry(res_entry)) {

    res_desc = new Res_descr(this);
    MD_set_entry_ext(res_entry, res_desc);
    name = strdup(res_entry->name);

    MD_Field* count_field = 
      (count_decl) ? MD_find_field(res_entry,count_decl) : NULL;

    MD_Field* width_field = 
      (width_decl) ? MD_find_field(res_entry,width_decl) : NULL;

    MD_Field* virtual_field =
      (virtual_decl) ? MD_find_field(res_entry, virtual_decl) : NULL;

    int count = 1;
    bool is_virtual = false;

    if (count_field)
      count = MD_get_int(count_field, 0);    

    if (virtual_field) 
      is_virtual = (bool)MD_get_int(virtual_field, 0);
    
    res_desc->set(name, count, is_virtual);

    if (width_field)
      res_desc->set_width(MD_get_int(width_field, 0));

    res_descr_tab[i] = res_desc;
  }
}

void MDES::build_reservation_table(MD *md) {
  int i, j, k;
  MD_Entry *resv_entry;
  Res_use_descr ruhead(this), *rutail;

  MD_Section* resv_section = MD_find_section(md, "Reservation_Table");
  res_use_descr_tab.resize(MD_num_entries(resv_section));
  MD_Field_Decl* decl1 = MD_find_field_decl(resv_section, "use");

  MD_Section* ru_section = MD_find_section(md, "Resource_Usage");
  MD_Field_Decl* decl2 = MD_find_field_decl(ru_section, "use");
  MD_Field_Decl* decl3 = MD_find_field_decl(ru_section, "time");
  MD_Field_Decl* decl4 = MD_find_field_decl(ru_section, "count");

  // build reservation chains: 1 null + 1/resource
  for (i=0, resv_entry=MD_first_entry(resv_section);
       resv_entry != NULL; i++, resv_entry=MD_next_entry(resv_entry)) {
    rutail = &ruhead;
    // REQUIRED use(LINK(Table_Option|Resource_Unit|Resource_Usage)*);
    MD_Field* rufield = MD_find_field(resv_entry, decl1);

    for (j=0; j < MD_num_elements(rufield); j++) {
      MD_Entry* ru_entry = MD_get_link(rufield, j);

      // REQUIRED use(LINK(Resource));
      int res_index = get_entry_index(MD_get_link(MD_find_field(ru_entry, decl2),0));
      MD_Field* tfield = MD_find_field(ru_entry, decl3);
      MD_Field* cfield = (decl4) ? MD_find_field(ru_entry, decl4) : NULL;
      if (cfield) assert(MD_num_elements(tfield)==MD_num_elements(cfield));

      for (k=0; k < MD_num_elements(tfield); k++) {
  // REQUIRED time(INT INT*);
  int time = MD_get_int(tfield, k);
  // OPTIONAL count(INT INT*); 
  int count = (cfield) ? MD_get_int(cfield, k) : 1;
  Res_use_descr* res_use_desc = new Res_use_descr(this);
  res_use_desc->set_res_use(res_index, time, count);
  rutail->set_next(res_use_desc);
  rutail = res_use_desc;
      }
    }
    rutail->set_next(NULL);
    res_use_descr_tab[i] = ruhead.get_next();
    MD_set_entry_ext(resv_entry, ruhead.get_next());
  }
}

void MDES::build_latency_table(MD *md) {
  int i, j, k;
  MD_Field_Decl *decl0, *decl1, *decl2, *decl3, *decl4, *decl5, *decl6; 
  MD_Entry *entry;
  Lat_descr *lat_desc;
  int lat_time[MDES_latencies_per_class];

  MD_Section *lat_section = MD_find_section(md, "Operation_Latency");
  lat_descr_tab.resize(MD_num_entries(lat_section));

  decl0 = MD_find_field_decl(lat_section, "exc");
  decl1 = MD_find_field_decl(lat_section, "rsv");
  decl2 = MD_find_field_decl(lat_section, "pred");
  decl3 = MD_find_field_decl(lat_section, "src");
  decl4 = MD_find_field_decl(lat_section, "dest");
  decl5 = MD_find_field_decl(lat_section, "sync_src");
  decl6 = MD_find_field_decl(lat_section, "sync_dest");

  //di0 is the predicate input and rsv0, do0 are unused
  //exc rsv1 rsv2 rsv3 rsv4 di0 di1 di2 di3 di4 si0 si1 do1 do2 do3 do4 so0 so1

  for (i=0, entry=MD_first_entry(lat_section);
       entry != NULL; i++, entry=MD_next_entry(entry)) {
    lat_desc = new Lat_descr(this); k=0;
    MD_set_entry_ext(entry, lat_desc);
    lat_time[k++] = get_latency_time(entry, decl0, 0);
    for (j=0; j < MDES_maxout-1 ; j++)
      lat_time[k++] = get_latency_time(entry, decl1, j);
    lat_time[k++] = get_latency_time(entry, decl2, 0);
    for (j=0; j < MDES_maxin-1 ; j++)
      lat_time[k++] = get_latency_time(entry, decl3, j);
    for (j=0; j < MDES_maxsynchin ; j++)
      lat_time[k++] = get_latency_time(entry, decl5, j);
    for (j=0; j < MDES_maxout-1 ; j++)
      lat_time[k++] = get_latency_time(entry, decl4, j);
    for (j=0; j < MDES_maxsynchout ; j++)
      lat_time[k++] = get_latency_time(entry, decl6, j);
    lat_desc->set_lat_descr(lat_time);
    lat_descr_tab[i] = lat_desc;
  }
}

void MDES::build_const_set_table(MD *md) {
  MD_Entry *entry;
  MD_Field *field;

  MD_Section* crange_section = MD_find_section(md, "Constant_Range");
  MD_Field_Decl* decl2 = MD_find_field_decl(crange_section, "min");
  MD_Field_Decl* decl3 = MD_find_field_decl(crange_section, "max");
  MD_Field_Decl* decl4 = MD_find_field_decl(crange_section, "step");

  // Read the const ranges
  Hash_map<eString, int> range_to_min(hash_estring, 32);
  Hash_map<eString, int> range_to_max(hash_estring, 32);
  Hash_map<eString, int> range_to_step(hash_estring, 32);

  int i, j;
  for (i = 0, entry = MD_first_entry(crange_section);
       entry != NULL; i++, entry = MD_next_entry(entry)) {
    eString range_name(entry->name);

    int min  = MD_get_int(MD_find_field(entry, decl2), 0);
    int max  = MD_get_int(MD_find_field(entry, decl3), 0);
    int step = MD_get_int(MD_find_field(entry, decl4), 0);

    if(min > max || step < 1) {
      cdbg << "Range " << range_name << " is invalid!\n"
	   << " min <= max && step > 0 must be true." << endl;
      cdbg << "\tmin:  " << min << endl;
      cdbg << "\tmax:  " << max << endl;
      cdbg << "\tstep: " << step << endl;
      El_punt("");
    }

    range_to_min.bind (range_name, min);
    range_to_max.bind (range_name, max);
    range_to_step.bind(range_name, step);
  }

  MD_Section* cset_section = MD_find_section(md, "Constant_Set");
  MD_Field_Decl* decl = MD_find_field_decl(cset_section, "ranges");

  for (i = 0, entry = MD_first_entry(cset_section);
       entry != NULL; i++, entry = MD_next_entry(entry)) {
    eString set_name(entry->name);

    Const_set_descr* cset_desc = new Const_set_descr(this);
    assert(cset_desc && "out of memory");
    cset_desc->set_name(set_name);
    name_to_const_desc_map.bind(set_name, cset_desc);

    field = MD_find_field(entry, decl);
    int num_ranges = field ? MD_num_elements(field) : 0;
    Vector<int> mins(num_ranges), maxes(num_ranges), steps(num_ranges);

    for(j = 0; j < num_ranges; j++) {
      MD_Entry *cur_range = MD_get_link(field, j);
      eString cur_range_name(cur_range->name);
      mins[j]  = range_to_min[cur_range_name];
      maxes[j] = range_to_max[cur_range_name];
      steps[j] = range_to_step[cur_range_name];
    }
    cset_desc->set_mins(mins);
    cset_desc->set_maxes(maxes);
    cset_desc->set_steps(steps);

  }
}

void MDES::build_register_table(MD *md) {
  int i,j,k,temp;
  MD_Entry *entry;
  MD_Field *field, *field2;

  MD_Section* regf_section = MD_find_section(md, "Register_File");
  reg_descr_tab.resize(MD_num_entries(regf_section));

  MD_Field_Decl* decl1 = MD_find_field_decl(regf_section, "width");
  // RMR { optional vector length
  MD_Field_Decl* decl2 = MD_find_field_decl(regf_section, "vector_length");
  // } RMR
  MD_Field_Decl* decl3 = MD_find_field_decl(regf_section, "static");
  MD_Field_Decl* decl4 = MD_find_field_decl(regf_section, "rotating");
  MD_Field_Decl* decl5 = MD_find_field_decl(regf_section, "speculative");
  MD_Field_Decl* decl6 = MD_find_field_decl(regf_section, "virtual");


  // Store all register info in reg_section
  MD_Section* reg_section = MD_find_section(md, "Register");
  MD_Field_Decl* decl7 = MD_find_field_decl(reg_section, "class");
  MD_Field_Decl* decl8 = MD_find_field_decl(reg_section, "macro");
  MD_Field_Decl* decl9 = MD_find_field_decl(reg_section, "overlaps");

  MD_Field_Decl* decl10 = MD_find_field_decl(regf_section, "donot_allocate");

  // Store all register info in reg_section
  //  MD_Section* macro_section = MD_find_section(md, "Macro");
  //  MD_Field_Decl* decl10 = MD_find_field_decl(macro_section, "class");

  for (i=0, entry=MD_first_entry(regf_section);
       entry != NULL; i++, entry=MD_next_entry(entry)) {
    Reg_descr* reg_desc = new Reg_descr(this);
    MD_set_entry_ext(entry, reg_desc);
    char* name = strdup(entry->name);
    eString pf(name);

    // REQUIRED width(INT);
    int width = MD_get_int(MD_find_field(entry, decl1), 0);
    // RMR { read optional vector length field; specified for vector register files
    field = MD_find_field(entry, decl2);
    int vlen = field ? MD_get_int(field, 0) : 1;
    width *= vlen;
    // } RMR
    // OPTIONAL static(LINK(Register)*);
    field = MD_find_field(entry, decl3);
    int scap = (field) ? MD_num_elements(field) : 0;

    // Create Hash_sets for Register classes, Macros and physical register map
    for(j=0; j < scap; j++) {
      MD_Entry *cur_reg = MD_get_link(field, j);
      eString current_reg_name(cur_reg->name);

      // OPTIONAL class(LINK(Register_Class)*);
      field2 = MD_find_field(cur_reg, decl7);
      temp = (field2) ? MD_num_elements(field2): 0;
      for(k=0; k < temp; k++)   {
        MD_Entry *entry2 = MD_get_link(field2, k);      
        if(strcmp(entry2->name, "CALLER") == 0)
          reg_desc->set_caller_save(current_reg_name);
        if(strcmp(entry2->name, "CALLEE") == 0)
          reg_desc->set_callee_save(current_reg_name);
      }

      // OPTIONAL macro(LINK(Macro_Class)*);
      field2 = MD_find_field(cur_reg, decl8);
      temp = (field2) ? MD_num_elements(field2): 0;

      for(k=0; k < temp; k++) {
        MD_Entry *entry2 = MD_get_link(field2, k);
        eString macro_name(entry2->name);
        if (dbg(mdes,2)) 
          cdbg << "found macro: " << macro_name << ":" << current_reg_name << endl;
          
        reg_desc->add_macro(macro_name);
        reg_desc->set_physical_reg_map(macro_name, current_reg_name)  ;
      }      

      // OPTIONAL overlaps(LINK(Register)*);
      field2 = MD_find_field(cur_reg, decl9);
      temp = (field2) ? MD_num_elements(field2): 0;
      for(k=0; k < temp; k++) {
        MD_Entry *entry2 = MD_get_link(field2, k);
        eString overlap_name(entry2->name);      
        reg_desc->set_physical_reg_map(current_reg_name, overlap_name);
        reg_desc->set_physical_reg_map(overlap_name, current_reg_name);
      }   
    }

    // OPTIONAL rotating(LINK(Register)*);
    field = MD_find_field(entry, decl4);
    int rcap = (field) ? MD_num_elements(field) : 0;

    // Create Hash_sets for Register classes, Macros and physical register map
    for(j=0; j < rcap; j++) {
      MD_Entry *cur_reg = MD_get_link(field, j);
      eString current_reg_name(cur_reg->name);

      // OPTIONAL macro(LINK(Macro_Class)*);
      field2 = MD_find_field(cur_reg, decl8);
      temp = (field2) ? MD_num_elements(field2): 0;

      for(k=0; k < temp; k++) {
        MD_Entry *entry2 = MD_get_link(field2, k);
        eString macro_name(entry2->name);
        if (dbg(mdes,2)) 
          cdbg << "found macro: " << macro_name << ":" << current_reg_name << endl;
          
        reg_desc->add_macro(macro_name);
        reg_desc->set_physical_reg_map(macro_name, current_reg_name)  ;
      }      
    }

    // OPTIONAL speculative(INT);
    field = MD_find_field(entry, decl5);
    bool tag = (field && MD_get_int(field,0)!=0) ? true : false;

    // OPTIONAL virtual(STRING);
    field = MD_find_field(entry, decl6);
    char* vname = (field) ? strdup(MD_get_string(field,0)) : name;
    reg_desc->set_reg(vname, name, scap, rcap, width, vlen, tag, i);

    // OPTIONAL doublelist(DOUBLE*);   // actual literals
    build_reg_descr_kind(reg_desc, entry);

    // OPTIONAL donot_allocate();
    if(decl10) {
      field = MD_find_field(entry, decl10);
      reg_desc->set_donot_allocate(field ? true : false);
    } else {
      reg_desc->set_donot_allocate(false);
    }


    reg_descr_tab[i] = reg_desc;
    
  }    
}

void MDES::build_stack_descr_table(MD *md) {
  MD_Entry *entry;
  stack_descr_tab = new StackDescr();

  MD_Section *stack_descr = MD_find_section(md, "Stack_Descr");
  if (!stack_descr)
    El_punt("build_stack_descr_table: Stack_Descr section missing in MDES!");
  MD_Field_Decl* dir_decl = MD_find_field_decl(stack_descr, "Dir");
  MD_Field_Decl* ret_decl = MD_find_field_decl(stack_descr, "RetAddrThruStack");
  MD_Field_Decl* ret_size_decl = MD_find_field_decl(stack_descr, "RetAddrSize");
  MD_Field_Decl* align_decl = MD_find_field_decl(stack_descr, "Alignment");

  entry = MD_first_entry(stack_descr);

  MD_Field *dir_field = MD_find_field(entry, dir_decl);
  char *dir = MD_get_string(dir_field, 0);
  if (!strcmp(dir, "HIGH_TO_LOW"))
    stack_descr_tab->setStackDir(HIGH_TO_LOW);
  else if (!strcmp(dir, "LOW_TO_HIGH"))
    stack_descr_tab->setStackDir(LOW_TO_HIGH);
  else
    El_punt("Dir field in Stack_Descr section in MDES has to be 'HIGH_TO_LOW' or 'LOW_TO_HIGH'");
    
  MD_Field *ret_field = MD_find_field(entry, ret_decl);
  char *ret = MD_get_string(ret_field, 0);
  if (!strcmp(ret, "true"))
    stack_descr_tab->setRetAddrThruStack();
  else if (!strcmp(ret, "false"))
    ;
  else
    El_punt("RetAddrThruStack field in Stack_Descr section in MDES has to be 'true' or 'false'");

  MD_Field *ret_size_field = MD_find_field(entry, ret_size_decl);
  int ret_size = MD_get_int(ret_size_field, 0);
  stack_descr_tab->setRetAddrSize(ret_size);

  MD_Field *align_field = MD_find_field(entry, align_decl);
  int align = MD_get_int(align_field, 0);
  stack_descr_tab->setAlignment(align);
}

void MDES::build_macro_table(MD *md) {
  int i, j, temp;
  MD_Entry *entry;
  MD_Field *field1, *field2;
  Hash_set<eString> macros_aliased(hash_estring);

  MD_Section *macro_section = MD_find_section(md, "Macro");
  MD_Field_Decl* decl1 = MD_find_field_decl(macro_section, "class");
  MD_Field_Decl* decl2 = MD_find_field_decl(macro_section, "alias");

  if (dbg(mdes,2)) 
    cdbg << "Macro section has: " << MD_num_entries(macro_section) << " entries" << endl;
  macro_descr_tab.resize(MD_num_entries(macro_section));

  // setup characteristics of macro
  for (i=0, entry=MD_first_entry(macro_section);
       entry != NULL; i++, entry=MD_next_entry(entry)) {
    eString mname(entry->name);

    if (dbg(mdes,2)) 
      cdbg << "Macro: " << mname << endl;

    Macro_descr *macro_desc = new Macro_descr(mname);
    macro_to_index_map.bind(mname, i);

    // set class
    field1 = MD_find_field(entry, decl1);
    temp = (field1) ? MD_num_elements(field1): 0;
    for(j=0; j < temp; j++)   {
      MD_Entry *entry2 = MD_get_link(field1, j);     

      if(strcmp(entry2->name, "READ_ONLY") == 0) {
        macro_desc->set_read_only(true);
        read_only_macros += mname;
      }
      if(strcmp(entry2->name, "PSEUDO") == 0) {
        macro_desc->set_pseudo(true);
        pseudo_macros += mname;
      }
      if(strcmp(entry2->name, "ALIAS") == 0) {
        macro_desc->set_alias(true);
        alias_macros += mname;
      }
      if(strcmp(entry2->name, "MULTI_ALIAS") == 0) {
        macro_desc->set_multialias(true);
        multialias_macros += mname;
      }
      if(strcmp(entry2->name, "COHERENT") == 0) {
        macro_desc->set_coherent(true);
        coherent_macros += mname;
      }
      if(strcmp(entry2->name, "PV") == 0) {
        macro_desc->set_pv(true);
      }
    }

    // set aliases
    field2 = MD_find_field(entry, decl2);
    temp = (field2) ? MD_num_elements(field2): 0;
    for(j=0; j < temp; j++)   {
      MD_Entry *entry2 = MD_get_link(field2, j);      
      if (dbg(mdes,2))         
        cdbg << "\taliases: " << entry2->name << endl;
      eString mn(entry2->name);

      macro_desc->set_macro_aliased(mn);
      macro_desc->set_alias(true);
      macros_aliased += mn;

    }

    macro_descr_tab[i] = macro_desc;    
  }

  // Fix Reg Descr's to include alias macros
  // For example, if SP exists in a cluster, then IP, OP, LV and RGS do too
  for(j=0; j<reg_descr_tab.dim(); j++) {
    eString regf_name = reg_descr_tab[j]->get_aname();

    // find out which registers in this file are aliased by another macro
    Hash_set<eString> regf_macros(hash_estring, 8);
    reg_descr_tab[j]->get_macros(regf_macros);
    regf_macros *= macros_aliased;

    for(Hash_set_iterator<eString> hsi(regf_macros); hsi!=0; hsi++) {
      eString aliased_macro = *hsi;
      // get the physical registers for this aliased macro (i.e. SP's registers)
      Hash_set<eString> phys_regs = reg_descr_tab[j]->get_physical_regs(aliased_macro);

      // find all macros which alias it (i.e. IP is the same as SP)
      for(int k=0; k<macro_descr_tab.dim(); k++) {
        if(macro_descr_tab[k]->aliases_macro(aliased_macro)) {
          eString alias = macro_descr_tab[k]->get_name();

          for(Hash_set_iterator<eString> hsi4(phys_regs); hsi4!=0; hsi4++) {
            eString physical_reg = *hsi4;
            // add this alias macro to the reg descr
            reg_descr_tab[j]->add_macro(alias);
            reg_descr_tab[j]->set_physical_reg_map(alias, physical_reg);
          }
          
        }            
      }
    }
  }


  // Populate the macro name to physical register file map
  // First, add the regular macros
  for (unsigned int i = 0; i < reg_descr_tab.size(); i++) {
    eString rf(reg_descr_tab[i]->get_aname());
    Hash_set<eString> regf_macros(hash_estring, 8);
    reg_descr_tab[i]->get_macros(regf_macros);
    for (Hash_set_iterator<eString> hiter(regf_macros); hiter!=0; hiter++) {
      Macro_name mn = el_string_to_macro_map.value(*hiter);
      
      if (dbg(mdes,2))         
        cdbg << "binding " << (eString) *hiter << " to rf " << rf << endl;
        
      if (macro_to_pf_map.is_bound(mn)) {
        macro_to_pf_map.value(mn) += rf;
      } else {
        Hash_set<eString> physical_files(hash_estring, 32);
        physical_files += rf;
        macro_to_pf_map.bind(mn, physical_files);
      }
    }
  }

}

void MDES::build_ftdescr_table(MD *md, IO_kind kind) {
  int i, j;
  MD_Entry *entry;
  MD_Field_Decl *decl1, *decl2;
  MD_Field *field1, *field2;
  Ft_descr *ftdesc;
  char *name;

  MD_Section *ft_section = MD_find_section(md, "Field_Type");
  ft_descr_tab.resize(MD_num_entries(ft_section));

  decl1 = MD_find_field_decl(ft_section, "regfile");
  decl2 = MD_find_field_decl(ft_section, "compatible_with");

  // we do two pass processing. First all base fields are processed
  // and assigned iocodes, then all compound fields are processed.
  // Shail Aditya 06/30/98

  // zeroth pass counts physical field types to determine the size of iocode
  // bitvectors. -KF 9-2004
  int max = 0;
  for (i=0, entry=MD_first_entry(ft_section);
       entry != NULL;
       entry=MD_next_entry(entry)) {
    field1 = MD_find_field(entry, decl1);
    field2 = MD_find_field(entry, decl2);
    if (field2) continue;
    if (field1) {
      Reg_descr* desc = (Reg_descr*)MD_get_entry_ext(MD_get_link(field1,0));
      assert(desc);
      int id = desc->get_io_id();
      if (id > max) max = id;
    } else {
      max = i;
    }
  }

  // This is a global variable set by each instance of an MDES, thus
  // we need to take the max of all MDESes read by the system.
  IOCODE_SIZE = ELCOR_MAX(IOCODE_SIZE, max + 1);

  Bitvector iocode(IOCODE_SIZE);

  // first pass skips over compound fields
  for (i=0, num_vreg_types=0, entry=MD_first_entry(ft_section);
       entry != NULL; entry=MD_next_entry(entry)) {
    field1 = MD_find_field(entry, decl1);
    field2 = MD_find_field(entry, decl2);
    if (field2) continue;

    num_vreg_types++;
    ftdesc = new Ft_descr(this, kind);
    MD_set_entry_ext(entry, ftdesc);
    name = strdup(entry->name);
    if (field1) {
      // if regfile field is present, setup iocode using it
      // Only one regfile mapping is allowed -- Shail Aditya 06/06/97
      assert(get_MDES_kind()==MDES_PHYSICAL);
      assert(kind==IO_PHYSICAL);
      Reg_descr* desc = (Reg_descr*)MD_get_entry_ext(MD_get_link(field1,0));
      assert(desc);
      iocode = desc->get_iocode();
    } else {
      // assign a new iocode to field type
      assert(get_MDES_kind()==MDES_VIRTUAL);
      assert(kind==IO_VIRTUAL);
      iocode.clear();
      iocode.set_bit(i);
    }
    ftdesc->set_ft_descr(name, iocode);
    ft_descr_tab[i++] = ftdesc;
  }

  // second pass processes compound fields (IO set)
  for (entry=MD_first_entry(ft_section);
       entry != NULL; entry=MD_next_entry(entry)) {
    field1 = MD_find_field(entry, decl1);
    field2 = MD_find_field(entry, decl2);
    if (!field2) continue;

    ftdesc = new Ft_descr(this, kind);
    MD_set_entry_ext(entry, ftdesc);
    name = strdup(entry->name);
    for (iocode.clear(), j=0; j < MD_num_elements(field2); j++) {
      Ft_descr* desc = (Ft_descr*)MD_get_entry_ext(MD_get_link(field2,j));
      assert(desc);
      iocode += desc->get_iocode(); 
      if (kind != desc->get_iokind()) 
        El_punt("MDES: Cannot mix virtual and physical field types.");
    }
    ftdesc->set_ft_descr(name, iocode);
    ft_descr_tab[i++] = ftdesc;
  }
  assert(i==MD_num_entries(ft_section));
}

void MDES::build_iodescr_table(MD *md, IO_kind kind) {
  int i, j;
  Bitvector iocode(IOCODE_SIZE);
  MD_Entry *entry;

  MD_Section* io_section = MD_find_section(md, "Operation_Format");
  io_descr_tab.resize(MD_num_entries(io_section));

  MD_Field_Decl* decl1 = MD_find_field_decl(io_section, "pred");
  MD_Field_Decl* decl2 = MD_find_field_decl(io_section, "src");
  MD_Field_Decl* decl3 = MD_find_field_decl(io_section, "dest");

  for (i=0, entry=MD_first_entry(io_section); 
       entry != NULL; i++, entry=MD_next_entry(entry)) {
    MD_Field* pfield = MD_find_field(entry, decl1);
    bool pred = (pfield && MD_num_elements(pfield) != 0);

    MD_Field* sfield = MD_find_field(entry,decl2);
    int numin = sfield ? MD_num_elements(sfield) : 0;

    MD_Field* dfield = MD_find_field(entry,decl3);
    int numout = dfield ? MD_num_elements(dfield) : 0;

    // allocate Io_descr
    Io_descr* iodesc = new Io_descr(this, kind, pred, numin, numout); 
    MD_set_entry_ext(entry, iodesc);

    // setup pred iocode
    if (pred) {
      if (MD_num_elements(pfield)!=1) 
        El_warn("MDES: only one predicate field is allowed, rest will be ignored.");
      Ft_descr* ftdesc = (Ft_descr*)MD_get_entry_ext(MD_get_link(pfield,0));
      assert(ftdesc && ftdesc->get_iokind()==kind);
      iocode = ftdesc->get_iocode();
      iodesc->set_pred(iocode);
    }

    // setup input iocodes
    for (j=0 ; j < MD_num_elements(sfield); j++) {
      Ft_descr* ftdesc = (Ft_descr*)MD_get_entry_ext(MD_get_link(sfield,j));
      assert(ftdesc && ftdesc->get_iokind()==kind);
      iocode = ftdesc->get_iocode();
      iodesc->set_in_reg(j, iocode);
    }

    // setup output iocodes
    for (j=0 ; j < MD_num_elements(dfield); j++) {
      Ft_descr* ftdesc = (Ft_descr*)MD_get_entry_ext(MD_get_link(dfield,j));
      assert(ftdesc && ftdesc->get_iokind()==kind);
      iocode = ftdesc->get_iocode();
      iodesc->set_out_reg(j, iocode);
    }

    io_descr_tab[i] = iodesc;
  }
}

void MDES::build_opdescr_table(MD *md) {
  int i, j, k, m;
  MD_Entry *opcs_entry, *opux_entry, *alt_entry, *io_entry;
  MD_Field *opfield, *altfield, *formatfield;
  int numin, numout, cspriority, uxpriority, altpriority;
  ResClass resclass;
  OpClass opclass;
  bool eager, is_predicated;

  MD_Section* elop_section = MD_find_section(md, "Elcor_Operation");
  MD_Field_Decl* decl1 = MD_find_field_decl(elop_section, "op");

  MD_Section* op_section = MD_find_section(md, "Operation");
  MD_Field_Decl* decl2 = MD_find_field_decl(op_section, "alt");

  MD_Section* alt_section = MD_find_section(md, "Scheduling_Alternative");
  MD_Field_Decl* decl3 = MD_find_field_decl(alt_section, "format");

  // Make (generic) compiler ops
  for (i=0, opcs_entry=MD_first_entry(elop_section);
       opcs_entry != NULL; i++, opcs_entry=MD_next_entry(opcs_entry)) {
    Op_descr* opcs = new Op_descr(this);
    MD_set_entry_ext(opcs_entry, opcs);
    char* lname = strdup(opcs_entry->name);
    // install cop on generic name
    hashtable->insert_op(lname, opcs);

    // fill cop properties
    process_properties(opcs_entry, &resclass, &opclass, &eager);
    // We assume all alternatives have same io format structure (arity, predication)
    opux_entry = MD_get_link(MD_find_field(opcs_entry, decl1), 0);
    alt_entry = MD_get_link(MD_find_field(opux_entry, decl2), 0);
    io_entry = MD_get_link(MD_find_field(alt_entry, decl3), 0);
    Io_descr *iodesc = (Io_descr*)MD_get_entry_ext(io_entry);
    assert(iodesc);

    numin = iodesc->get_num_in();
    numout = iodesc->get_num_out();
    is_predicated = iodesc->is_predicated();
    cspriority = 0;
    opcs->fill_op(lname, lname, NULL, CLSTROP, numin,
        is_predicated, numout, opclass, resclass, eager, cspriority);

    // Make (unit) architectural ops
    altpriority = 0;
    opfield = MD_find_field(opcs_entry, decl1);
    for (j=0; j < MD_num_elements(opfield); j++) {
      opux_entry = MD_get_link(opfield, j);
      Op_descr* opux = new Op_descr(this);
      MD_set_entry_ext(opux_entry, opux);
      char* aname = strdup(opux_entry->name);
      // install aop on arch name
      hashtable->insert_op(aname, opux);

      // fill aop properties. The execution priority of the opcode
      // reflects the hardware priority of the execution unit on which
      // this opcode executes. We use a combination of the resource
      // class of the opcode and the order of appearance of the
      // alternatives as the priority. -- Shail Aditya 02/26/98

      uxpriority = resclass * get_MDES_num_resources() + j;
      opux->fill_op(lname, aname, NULL, UNITOP, numin, 
          is_predicated, numout, opclass, resclass, eager, uxpriority);

      // make scheduling alternatives
      altfield = MD_find_field(opux_entry, decl2);
      for (k=0; k < MD_num_elements(altfield); k++) {
        alt_entry = MD_get_link(altfield, k);
        Alt_descr *altdesc = make_alt_desc(this, alt_entry, opux);
        // the alternative priority is the order in which the
        // alternatives are kept in the mdes. This is the same as the
        // external order of the operations and their alternatives.
        // -- Shail Aditya 09/18/98
        Alt_list altlist(altdesc, NULL, altpriority++);
  
        // install this alt on all io formats
        formatfield = MD_find_field(alt_entry, decl3);
        for (m=0; m < MD_num_elements(formatfield); m++) {
          io_entry = MD_get_link(formatfield, m);
          iodesc = (Io_descr*)MD_get_entry_ext(io_entry);
          assert(iodesc && numin==iodesc->get_num_in() && 
           numout==iodesc->get_num_out() &&
           is_predicated==iodesc->is_predicated());
          opux->add_alts(iodesc, altlist.copy());
          opcs->add_alts(iodesc, altlist.copy());
        }
      }
    }
  }
}


void MDES::build_custom_opdescr_table(MD *md) {
  int i, j, k, m;
  MD_Entry *opcs_entry, *opux_entry, *alt_entry, *io_entry;
  MD_Field *opfield, *altfield, *formatfield;
  int numin, numout, cspriority, uxpriority, altpriority;
  ResClass resclass;
  OpClass opclass;
  bool eager, is_predicated;

  MD_Section* cop_section = MD_find_section(md, "Custom_Operation");
  MD_Field_Decl* decl1 = MD_find_field_decl(cop_section, "op");

  MD_Section* op_section = MD_find_section(md, "Operation");
  MD_Field_Decl* decl2 = MD_find_field_decl(op_section, "alt");

  MD_Section* alt_section = MD_find_section(md, "Scheduling_Alternative");
  MD_Field_Decl* decl3 = MD_find_field_decl(alt_section, "format");

  // Make (generic) compiler ops
  for (i=0, opcs_entry = MD_first_entry(cop_section);
       opcs_entry != NULL; i++, opcs_entry = MD_next_entry(opcs_entry)) {
    Op_descr* opcs = new Op_descr(this);
    MD_set_entry_ext(opcs_entry, opcs);
    char* lname = strdup(opcs_entry->name);

    hashtable->insert_op(lname, opcs);

    // fill cop properties
    process_properties(opcs_entry, &resclass, &opclass, &eager);
    // We assume all alternatives have same io format structure (arity, predication)
    opux_entry = MD_get_link(MD_find_field(opcs_entry, decl1), 0);
    alt_entry = MD_get_link(MD_find_field(opux_entry, decl2), 0);
    io_entry = MD_get_link(MD_find_field(alt_entry, decl3), 0);
    Io_descr *iodesc = (Io_descr*)MD_get_entry_ext(io_entry);
    assert(iodesc);

    numin = iodesc->get_num_in();
    numout = iodesc->get_num_out();
    is_predicated = iodesc->is_predicated();
    cspriority = 0;
    opcs->fill_op(lname, lname, NULL, CLSTROP, numin,
      is_predicated, numout, opclass, resclass, eager, cspriority);

    // Make (unit) architectural ops
    altpriority = 0;
    opfield = MD_find_field(opcs_entry, decl1);
    for (j=0; j < MD_num_elements(opfield); j++) {
      opux_entry = MD_get_link(opfield, j);
      Op_descr* opux = new Op_descr(this);
      MD_set_entry_ext(opux_entry, opux);
      char* aname = strdup(opux_entry->name);

      hashtable->insert_op(aname, opux);

      // fill aop properties. The execution priority of the opcode
      // reflects the hardware priority of the execution unit on which
      // this opcode executes. We use a combination of the resource
      // class of the opcode and the order of appearance of the
      // alternatives as the priority. -- Shail Aditya 02/26/98

      uxpriority = resclass * get_MDES_num_resources() + j;
      opux->fill_op(lname, aname, NULL, UNITOP, numin, 
        is_predicated, numout, opclass, resclass, eager, uxpriority);

      // make scheduling alternatives
      altfield = MD_find_field(opux_entry, decl2);
      for (k=0; k < MD_num_elements(altfield); k++) {
        alt_entry = MD_get_link(altfield, k);
        Alt_descr *altdesc = make_alt_desc(this, alt_entry, opux);
        // the alternative priority is the order in which the
        // alternatives are kept in the mdes. This is the same as the
        // external order of the operations and their alternatives.
        // -- Shail Aditya 09/18/98
        Alt_list altlist(altdesc, NULL, altpriority++);

        // install this alt on all io formats
        formatfield = MD_find_field(alt_entry, decl3);
        for (m=0; m < MD_num_elements(formatfield); m++) {
          io_entry = MD_get_link(formatfield, m);
          iodesc = (Io_descr*)MD_get_entry_ext(io_entry);
          assert(iodesc && numin==iodesc->get_num_in() && 
           numout==iodesc->get_num_out() &&
           is_predicated==iodesc->is_predicated());
          opux->add_alts(iodesc, altlist.copy());
          opcs->add_alts(iodesc, altlist.copy());
        }
      }
    }
  }
}

Res_descr* MDES::get_res_descr(char* res_name) {
  eString res_str(res_name);
  if (res_str_to_descr_map.is_bound(res_str))
    return res_str_to_descr_map[res_str];
  else
    return NULL;
}

void MDES::check_mdes() {}

/////////////////////////////////////////////////////////////////////////////
// Support functions

static void build_reg_descr_kind(Reg_descr* reg_desc, MD_Entry* entry) {
  int j;
  MD_Field* field;

  // First identify various literal files
  MD_Section* regf_section = entry->section;
  MD_Field_Decl* decl6 = MD_find_field_decl(regf_section, "consts");
  MD_Field_Decl* decl7 = MD_find_field_decl(regf_section, "doublelist");
  

  MD_Field* field6 = MD_find_field(entry, decl6);
  MD_Field* field7 = MD_find_field(entry, decl7);
  if (field6 && field7)
    El_punt("MDES: only ONE of 'consts' and 'doublelist' should be specified.");
  if (field6) {
    MD_Entry *cset = MD_get_link(field6, 0);
    eString cset_name(cset->name);
    reg_desc->set_regkind(REG_LITINTRANGE);

    reg_desc->set_ilits(reg_desc->get_mdes()->get_MDES_const_descr(cset_name));
    return;
  }
  field = field7;
  if (field) {
    Vector<double>* vect = new Vector<double>(MD_num_elements(field));
    for (j=0; j<MD_num_elements(field); j++) (*vect)[j] = MD_get_double(field, j);
    reg_desc->set_regkind(REG_LITDOUBLE);
    reg_desc->set_dlits(vect);
    return;
  }

  // You reach here only when none of intlist, intrange, or doublelist
  // are defined, i.e. the file is either a physical register file or
  // is undefined. We classify unresolved virtual literals (vname 'L')
  // or virtual undefined (vname 'U') files as undefined and the rest
  // as physical (default).  -- Shail Aditya 06/27/97
  if (strchr(reg_desc->get_vname(), 'L')!=0 || 
      strchr(reg_desc->get_vname(), 'U')!=0)
    reg_desc->set_regkind(REG_UNDEFINED);
}

// build up sets of register classes/macros
static void build_reg_descr_sets(Reg_descr* reg_desc, MD_Entry* entry) {

}

static int get_latency_time(MD_Entry *entry, MD_Field_Decl *decl, int index) {
  MD *md = entry->section->md;
  MD_Field_Decl *time_decl;
  MD_Entry *time_entry;

  time_decl = MD_find_field_decl(MD_find_section(md, "Operand_Latency"), "time");
  time_entry = MD_get_link(MD_find_field(entry, decl), index);
  return (MD_get_int(MD_find_field(time_entry, time_decl), 0));
}

Alt_descr* make_alt_desc(MDES* mdesc, MD_Entry *alt_entry, Op_descr *opux) {
  MD_Field_Decl *decl1, *decl2;
  MD_Field *latfield, *resvfield;
  Alt_descr *altdesc;
  Res_use_descr *resuse;
  Lat_descr *lat;

  decl1 = MD_find_field_decl(alt_entry->section, "latency");
  decl2 = MD_find_field_decl(alt_entry->section, "resv");

  latfield = MD_find_field(alt_entry, decl1);
  lat = (Lat_descr*)MD_get_entry_ext(MD_get_link(latfield,0));
  assert(lat);
  resvfield = MD_find_field(alt_entry, decl2);
  resuse = (Res_use_descr*)MD_get_entry_ext(MD_get_link(resvfield,0));

  altdesc = new Alt_descr(mdesc, opux, lat, resuse);
  return(altdesc);
}

void process_properties(MD_Entry *opcs_entry,
            ResClass *rescls, OpClass *opcls, bool *eagercls) {
  // process flags
  // flag1 = opclass, flag2 = resclass, flag3 = eager (if present)
  MD_Field_Decl *flagdecl = MD_find_field_decl(opcs_entry->section, "flags");
  MD_Field *flagfield = MD_find_field(opcs_entry, flagdecl);

  *opcls = opclass(MD_get_link(flagfield, 0)->name);
  *rescls = resclass(MD_get_link(flagfield, 1)->name);
  *eagercls = eagerclass(MD_get_link(flagfield, 2)->name);

  Op_descr* opcs = ((Op_descr*) MD_get_entry_ext(opcs_entry));

  opcs->set_predicated (false);
  opcs->set_associative (false);
  opcs->set_commutative (false);
  opcs->set_le_op (false);
  opcs->set_advanced_load (false);
  opcs->set_two_operand (false);
  opcs->set_unsupported (false);

  // If additional flags are specified they will be set.
  if (MD_num_elements(flagfield) > 3) {
    for (int i=3; i<MD_num_elements(flagfield); i++) {
      MD_Entry *extra_field = MD_get_link(flagfield,i);
      eString field =  extra_field->name ;
      if (field == "is_pred") { 
        opcs -> set_predicated (true);
      }
      if (field == "is_leop") { 
        opcs->set_le_op(true);
      }
      if (field == "is_advload") { 
        opcs->set_advanced_load (true);
      }
      if (field == "is_assoc") { 
        opcs->set_associative(true);
      }
      if (field == "is_comm") { 
        opcs->set_commutative(true);
      }
      if (field == "is_two_operand") {
        opcs->set_two_operand(true);
      }
      if (field == "is_unsupported") {
        opcs->set_unsupported(true);
      }
    }
  }
}

