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
//      File:           intf.cpp
//      Authors:        Mike Schlansker, Vinod Kathail, Shail Aditya
//      Created:        December 1994
//      Description:    The Elcor interface to MDES database
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "opcode.h"
#include "intf.h"
#include "mdes.h"
#include "el_error.h"
#include "el_init.h"
#include "el_io.h"
#include "dbg.h"

#include "list.h"
#include "hash_set.h"

// Added SAM, 3-98
int El_assume_infinite_resources = 0;

static Op_descr *operation;     /* cache external interface operation descriptor*/
static Io_descr request_iod;  /*describes a specific IO request*/
static Rmsl* rmsl_obj=NULL;
RU_mgr* ru_map=NULL;

//////////////////////////////////////////////////////////////////////
// MDES functions

MDES* current_MDES = NULL;
static List<MDES*> MDES_stack;

void set_current_MDES(MDES* mdes) {
  // set global data structures
  current_MDES = mdes;
  request_iod.set_mdes(mdes);
}

void push_MDES(MDES* mdes) {
  // if something is missing we use the existing object
  // -- Shail Aditya 03/24/97
  if (!mdes) mdes = current_MDES;
  MDES_stack.push(current_MDES);
  set_current_MDES(mdes);
}

void pop_MDES(void) {
  set_current_MDES(MDES_stack.pop());
}

//////////////////////////////////////////////////////////////////////
// Opcode functions

int MDES_src_num(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation) {
    if (El_punt_on_unknown_op)
      El_punt("MDES_src_num: couldn't find operation '%s'", opcode);
    return MDES_maxin;
  }
  return operation->get_num_in();
}

int MDES_dest_num(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation) { 
    if (El_punt_on_unknown_op)
      El_punt("MDES_dest_num: couldn't find operation '%s'", opcode);
    return MDES_maxout;
  }
  return operation->get_num_out();
}

bool MDES_predicated(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation) {
    if (El_punt_on_unknown_op)
      El_punt("MDES_predicated: couldn't find operation '%s'", opcode);
    return true;
  }
  return operation->is_predicated();
}

bool MDES_has_speculative_version(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation) {
    if (El_punt_on_unknown_op)
      El_punt("MDES_has_speculative_version: couldn't find operation '%s'", opcode);
    return false;
  }
  return(operation->has_spec());
}

bool MDES_commutative(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation) {
    if (El_punt_on_unknown_op)
      El_punt("MDES_commutative: couldn't find operation '%s'", opcode);
    return false;
  }
  return operation->is_commutative();
}

bool MDES_two_operand(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if(!operation) El_punt("MDES_two_operand: couldn't find operation '%s'", opcode);
  return operation->is_two_operand();
}

bool MDES_unsupported(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  // more useful to return unsupported rather than punting -KF 4-2004
  //if(!operation) El_punt("MDES_unsupported: couldn't find operation '%s'", opcode);
  if (!operation) return true;
  return operation->is_unsupported();
}

int MDES_priority(char *opcode) {
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if(!operation) 
    El_punt("MDES_priority: couldn't find operation '%s'", opcode);
  return operation->get_priority();
}

bool MDES_is_short_literal(char *opcode, int port, int value,
     char *shortlit_filename) {
  List_set<Reg_descr*> regset;
  Reg_descr *regdes;

  MDES_collect_reg_descrs(opcode, DATA_IN, port, regset);

  for (List_set_iterator<Reg_descr*> setiter(regset); setiter != 0; setiter++){
    regdes = *setiter;
    if (regdes->is_intlit() && regdes->is_valid(value)) {
      strcpy(shortlit_filename, regdes->get_aname());
      return true;
    }
  }
  return false;
}

char* MDES_get_restricted_move(char *src_io, char *dest_io) {
  int src_index = current_MDES->find_MDES_rf_index(src_io, IO_PHYSICAL);
  int dest_index = current_MDES->find_MDES_rf_index(dest_io, IO_PHYSICAL);
  Reg_descr* src_desc = current_MDES->get_MDES_reg_descr(src_index);
  Reg_descr* dest_desc = current_MDES->get_MDES_reg_descr(dest_index);
  List_set<Reg_descr*> descset;

  MDES_collect_reg_descrs("MOVEGG", DATA_IN, 1, descset.clear());
  if (descset.is_member(src_desc)) {
    MDES_collect_reg_descrs("MOVEGG", DATA_OUT, 1, descset.clear());
    if (descset.is_member(dest_desc)) return "MOVEGG";
    MDES_collect_reg_descrs("MOVEGC", DATA_OUT, 1, descset.clear());
    if (descset.is_member(dest_desc)) return "MOVEGC";
  }
  MDES_collect_reg_descrs("MOVECG", DATA_IN, 1, descset.clear());
  if (descset.is_member(src_desc)) {
    MDES_collect_reg_descrs("MOVECG", DATA_OUT, 1, descset.clear());
    if (descset.is_member(dest_desc)) return "MOVECG";
  }
  return NULL;
}

static int MDES_flow_time_internal(IO_Portkind portkind, int portnum, 
      bool checkio=true){
  int result;
  Io_list* iol_elem;
  Alt_descr* altd_elem;

  // Currently, we compute conservative latency summaries (max for
  // output, min for input). For more agressive scheduling we should
  // compute optimistic summaries.  This will matter once we have
  // heterogenous latencies among alternatives -- Shail Aditya
  // 03/25/97
  bool in_or_out = (portkind == DATA_IN) || (portkind == SYNC_IN);
  result = (in_or_out) ? infinity : minus_infinity;
  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) != NULL){
    Io_descr* iod_elem = iol_elem->get_iod();
    // skip non-matching io specs if checkio is true
    if (checkio && !iod_elem->test_io(&request_iod)) continue;
    while((altd_elem=iol_elem->get_next_alt()) !=NULL){
      Lat_descr* lat_elem = altd_elem->get_lat();
      int latency = lat_elem->get_flow_lat(portkind, portnum);
      result = (in_or_out) ? ELCOR_MIN(result,latency) : ELCOR_MAX(result,latency);
    }
  }
  if(result==infinity||result==minus_infinity) {
    cdbg << "ERROR: cannot find match for op/iodescr in the mdes " << endl
         << " op: " << operation->get_lname_ptr() << endl;
    operation->io_list_init();
    while((iol_elem=operation->get_next_io()) != NULL){
      Io_descr* iod_elem=iol_elem->get_iod();
      cdbg << " iodescr: " << *iod_elem << endl;
    }
    cdbg << "\nThis error sometimes occurs when trying to use edge latencies\n"
	 << "before calling Codegen::do_initial_codegen(). Codegen performs some\n"
	 << "transformations which make the code better match the target MDES.\n"
	 << endl;
    assert(0);
  }
  return(result);
}

static int MDES_anti_time_internal(IO_Portkind portkind, int portnum, 
    bool checkio=true){
  int result;
  Io_list* iol_elem;
  Alt_descr* altd_elem;

  // Currently, we compute conservative latency summaries (max for
  // output, min for input). For more agressive scheduling we should
  // compute optimistic summaries.  This will matter once we have
  // heterogenous latencies among alternatives -- Shail Aditya
  // 03/25/97
  bool in_or_out = (portkind == DATA_OUT) || (portkind == SYNC_OUT);
  result = (in_or_out) ? infinity : minus_infinity;
  operation->io_list_init();
  while((iol_elem=operation->get_next_io()) !=NULL){
    Io_descr* iod_elem=iol_elem->get_iod();
    // ignore non-matching io specs
    if (checkio && !iod_elem->test_io(&request_iod)) continue;
    while((altd_elem=iol_elem->get_next_alt()) !=NULL){
      Lat_descr* lat_elem = altd_elem->get_lat();
      int latency = lat_elem->get_anti_lat(portkind, portnum);
      result = (in_or_out) ? ELCOR_MIN(result,latency) : ELCOR_MAX(result,latency);
    }
  }
  if(result==infinity||result==minus_infinity) {
    cdbg << "ERROR: cannot find match for op/iodescr in the mdes " << endl
         << " op: " << operation->get_lname_ptr() << endl;
    operation->io_list_init();
    while((iol_elem=operation->get_next_io()) != NULL){
      Io_descr* iod_elem=iol_elem->get_iod();
      cdbg << " iodescr: " << *iod_elem << endl;
    }
    cdbg << "\nThis error sometimes occurs when trying to use edge latencies\n"
	 << "before calling Codegen::do_initial_codegen(). Codegen performs some\n"
	 << "transformations which make the code better match the target MDES.\n"
	 << endl;
    assert(0);
  }
  return(result);
}

void MDES_init_op_io(char *opcode, char * io){
  operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation)
    El_punt("MDES_init_op_io: Couldn't find operation '%s' with iospec '%s'",
      opcode,io);

  request_iod.parse_iospec(io); /*place iospec into Io_descr representation*/
}

int MDES_flow_time_io(IO_Portkind portkind, int portnum) {
  return MDES_flow_time_internal(portkind, portnum);
}

int MDES_anti_time_io(IO_Portkind portkind, int portnum) {
  return MDES_anti_time_internal(portkind, portnum);
}

int MDES_flow_time(char *opcode, IO_Portkind portkind, int portnum) {
  operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation)
    El_punt("MDES_flow_time: Couldn't find operation '%s'", opcode);
  return MDES_flow_time_internal(portkind, portnum, false);
}

int MDES_anti_time(char *opcode, IO_Portkind portkind, int portnum) {
  operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation)
    El_punt("MDES_anti_time: Couldn't find operation '%s'", opcode);
  return MDES_anti_time_internal(portkind, portnum, false);
}

int MDES_branch_latency(char * opcode){
  operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);
  if (!operation) El_punt("MDES_branch_latency: Couldn't find operation '%s'",opcode);
  request_iod = *(operation->get_next_io()->get_iod());
  return MDES_flow_time_io(SYNC_OUT, 0);
}

int MDES_branch_latency(){
  operation = (current_MDES->get_MDES_hashtable())->find_op("BRU");
  if (!operation) El_punt("MDES_branch_latency: Couldn't find operation 'BRU'");
  request_iod = *(operation->get_next_io()->get_iod());
  return MDES_flow_time_io(SYNC_OUT, 0);
}

//////////////////////////////////////////////////////////////////////
// Register File functions

char* MDES_translate_vreg(char* vregname) {
  if ((!current_MDES) || MDES_num_reg_types()==0) return vregname;
  List<char*> regnames;
  for (int i=0; i<MDES_num_reg_types(); i++) {
    Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(i);
    char* vname = regdesc->get_vname();
    char* aname = regdesc->get_aname();
    // if the requested vregname is implemented by this regdesc
    // (occurs in its vname), add it to the list of rf candidates.
    if (strstr(vname, vregname)) regnames.add_tail(aname);
  }
  // currently, we simply return the first physical register file that
  // can implement the given vreg type.  -- Shail Aditya 11/25/97
  // We flag an error if no physical files can implement the given
  // vreg type. -- Shail Aditya 02/17/99
  if (regnames.is_empty()) {
    El_punt("MDES_translate_vreg: The virtual register file type '%s' is not implemented", vregname);
    return NULL;
  } else return regnames.head();
}

int MDES_reg_index(char* regname) {
  return current_MDES->find_MDES_rf_index(regname, IO_PHYSICAL); 
}

char* MDES_reg_name(int regindex) {
  return current_MDES->get_MDES_reg_descr(regindex)->get_aname();
}

void MDES_reg_names(List<char*>& regnames) {
  regnames.clear();
  for (int i=0; i<MDES_num_reg_types(); i++) {
    Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(i);
    regnames.add_tail(regdesc->get_aname());
  }
}

int MDES_reg_width(char *regname){
  int regindex = MDES_reg_index(regname);
  Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(regindex);
  // return width in bits instead of words  -- Marnix.
  return regdesc->get_width();
}

/* RMR { querry vector length */
int MDES_reg_vector_length(char* regname)
{
  int regindex = MDES_reg_index(regname);
  Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(regindex);
  return regdesc->get_vector_length();
}
/* } RMR */

int MDES_reg_static_size(char *regname){
  int regindex = MDES_reg_index(regname);
  Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(regindex);
  return regdesc->get_scap();
}

int MDES_reg_rotating_size(char *regname){
  int regindex = MDES_reg_index(regname);
  Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(regindex);
  return regdesc->get_rcap();
}

bool MDES_supports_rot_reg(char *regname) {
  return (MDES_reg_rotating_size(regname) > 0);
}

bool MDES_reg_has_speculative_bit(char *regname){
  int regindex = MDES_reg_index(regname);
  Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(regindex);
  return regdesc->get_tag();
}

bool MDES_reg_is_allocatable(char *regname){
  int regindex = MDES_reg_index(regname);
  Reg_descr* regdesc = current_MDES->get_MDES_reg_descr(regindex);
  return (!regdesc->is_lit() && !regdesc->is_undefined());
}

void MDES_get_phys_regs(const Operand& mac, Hash_set<int>& phys_regs) 
{
  phys_regs.clear();
  int i = MDES_reg_index(mac.physical_file_type());
  Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
  Hash_set<eString>& phys_strs = rd->get_physical_regs(mac.name());
  for (Hash_set_iterator<eString> hsi(phys_strs); hsi!=0; ++hsi)
    phys_regs += current_MDES->get_MDES_reg_number_for_pf(*hsi);
}

//////////////////////////////////////////////////////////////////////
// Direct MDES Access

void MDES_collect_io_descrs(char* opcode, List_set<Io_descr*>& descset) {
  current_MDES->collect_io_descrs(opcode, descset);
}

void MDES_collect_reg_descrs(char* opcode, IO_Portkind portkind, int portnum,
         List_set<Reg_descr*>& descset) {
  current_MDES->collect_reg_descrs(opcode, portkind, portnum, descset);
}

void MDES_collect_ft_descrs(char* opcode, IO_Portkind portkind, int portnum,
        List_set<Ft_descr*>& descset) {
  current_MDES->collect_ft_descrs(opcode, portkind, portnum, descset);
}

void MDES_collect_op_descrs(char* opcode, List_set<Op_descr*>& descset) {
  current_MDES->collect_op_descrs(opcode, descset);
}

void MDES_collect_alt_descrs(char* opcode, List_set<Alt_descr*>& descset) {
  current_MDES->collect_alt_descrs(opcode, descset);
}

void MDES_collect_res_use_descrs(char* opcode, List_set<Res_use_descr*>& descset) {
  current_MDES->collect_res_use_descrs(opcode, descset);
}

Op_descr *MDES_collect_generic_op_descrs(char* opcode) {
  return current_MDES->collect_generic_op_descrs(opcode);
}

Hash_set<eString>& get_MDES_pf_for_macro(Macro_name mname) {
  return current_MDES->get_MDES_pf_for_macro(mname);
}

// moved these 2 functions here from Sched/schedule.cpp -KF 12/2004
Res_use_descr* MDES_get_res_use_descriptor(char *opcode, int res_idx)
{
  List_set<Res_use_descr*> descset;
  
  descset.clear();
  current_MDES->collect_res_use_descrs(opcode, descset);
   
  for(List_set_iterator<Res_use_descr*> li(descset); li!=0; li++) {
    Res_use_descr *rd = *li;
    if(rd && rd->get_res() == res_idx) return rd;
  }

  return NULL;
}

void MDES_get_sched_opcode_from_res_idx(Op *op, char *iopat, int res_idx, char **outchar)
{
  Io_list* iol_elem;
  Io_descr* iod_elem;
  Alt_descr* altd_elem;
  Io_descr request_iod(current_MDES);

  char *opcode = get_mdes_opcode_string(op);
  Op_descr *operation = (current_MDES->get_MDES_hashtable())->find_op(opcode);

  operation->io_list_init();
  iol_elem = operation->get_next_io();
  iod_elem = iol_elem->get_iod();
  
  request_iod.parse_iospec(iopat);

  while(iol_elem) {
    if(iod_elem->test_io(&request_iod)) {
      while((altd_elem = iol_elem->get_next_alt()) != NULL){
        Op_descr* alt_op = altd_elem->get_op();
        char* aname = alt_op->get_aname_ptr();
        *outchar = aname;
        Res_use_descr* res_use = altd_elem->get_res_use();
        if(res_use->get_res() == res_idx)
          return;
      }
    }
    iol_elem = operation->get_next_io();
    if (iol_elem) iod_elem = iol_elem->get_iod();
  }
  cdbg << "get_sched_opcode : " << get_mdes_opcode_string(op) << " : " << res_idx << endl;
  assert(0);
}

//////////////////////////////////////////////////////////////////////
// Resource Usage Management

void RU_alloc_map(int maxlength) {
  if (ru_map) delete ru_map;
  ru_map = new RU_mgr(current_MDES, maxlength, MDES_num_resources());
}
  
void RU_delete_map() {
  if (ru_map) delete ru_map;
  ru_map = NULL;
}

void RU_init_map(bool modulo, int length) {
  if( ! ru_map)
    RU_alloc_map(length);
  ru_map->init(modulo, length);
}

void RU_print_map(FILE *mout) {
  if( ! ru_map)
    return;
  ru_map->print_stream(mout);
}

int RU_cur_sched_length() {
  assert(ru_map);
  return ru_map->cur_sched_length();
}

void RU_init_iterator(char *opcode, void *op, char *io, int time) {
  ru_map->init_op(opcode, op, io, time);
}

bool RU_get_next_nonconfl_alt(char ** opcode, int * priority) {
  return (ru_map->get_next_nonconfl_alt(opcode, priority));
}

//KVM : To check whether some resource is free at a sched_time
bool RU_check_resources_free(Res_use_descr* res_use, int stime){
  return (ru_map->check_resources_free(res_use, stime));
}


void RU_place() {
  ru_map->place();
}

void RU_get_conflicting_ops(Hash_set<void*>& ops) {
  ru_map->get_conflicting_ops(ops);
}

void RU_remove(void *op, char *iopat, int scheduled_time) {
  ru_map->remove(op, scheduled_time);
}

void *RU_at(int cycle, int res_index) {
  List<void*> ops;
  ru_map->at(cycle, res_index, ops);
  return ops.head();
}

// void MDES_exchange_resources(int time, char** one, char** two)
// {
//   char buff[256];
//   char* dotone = strchr(*one, '.');
//   char* dottwo = strchr(*two, '.');
//   assert(dotone && dottwo);
//   *dotone++ = *dottwo++ = 0;
//   sprintf(buff, "%s.%s", *one, dottwo);
//   char* newone = strdup(buff);
//   sprintf(buff, "%s.%s", *two, dotone);
//   char* newtwo = strdup(buff);
//   *(--dotone) = *(--dottwo) = '.';
//   *one = newone; *two = newtwo;
//   return;
// }

void MDES_exchange_resources(int time, char** one, char** two)
{
  MdesHashtable* table = current_MDES->get_MDES_hashtable();
  Op_descr* one_aop = table->find_op(*one);assert(one_aop);
  Op_descr* one_cop = table->find_op(one_aop->get_lname_ptr());
  Op_descr* two_aop = table->find_op(*two);assert(two_aop);
  Op_descr* two_cop = table->find_op(two_aop->get_lname_ptr());
  assert(one_cop);assert(two_cop);

  // find a new alternative of one_cop that retains the io format of
  // one_aop but matches the reservation table of two_aop.
  one_aop->io_list_init();
  Io_descr* one_iod = one_aop->get_next_io()->get_iod();
  two_aop->io_list_init();
  Res_use_descr* two_resv = two_aop->get_next_io()->get_next_alt()->get_res_use();
  Alt_descr* one_alt = one_cop->find_alt(one_iod, two_resv);
  assert(one_alt);

  // find a new alternative of two_cop that retains the io format of
  // two_aop but matches the reservation table of one_aop.
  two_aop->io_list_init();
  Io_descr* two_iod = two_aop->get_next_io()->get_iod();
  one_aop->io_list_init();
  Res_use_descr* one_resv = one_aop->get_next_io()->get_next_alt()->get_res_use();
  Alt_descr* two_alt = two_cop->find_alt(two_iod, one_resv);
  assert(two_alt);

  // do the swap
  *one = one_alt->get_op()->get_aname_ptr();
  *two = two_alt->get_op()->get_aname_ptr();
}

//////////////////////////////////////////////////////////////////////
// Resource Minimum Schedule Length Computation

void RMSL_alloc(int lbound){
  if (rmsl_obj) delete rmsl_obj;
  rmsl_obj = new Rmsl(current_MDES, lbound);
}

void RMSL_dealloc(void) {
  if (rmsl_obj) delete rmsl_obj;
  rmsl_obj = NULL;
}

void RMSL_init(int lbound){
  assert(rmsl_obj);
  rmsl_obj->init(lbound);
}

void RMSL_nextop(char * opcode, char * io){
  rmsl_obj->nextop_io(opcode, io);
}

int RMSL_value(void){ return(rmsl_obj->value());}

// end of intf.cpp

