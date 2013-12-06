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
//      File:           intf.h
//      Authors:        Mike Schlansker, Vinod Kathail, Shail Aditya
//      Created:        December 1994
//      Description:    The Elcor interface to MDES database
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _INTF_H
#define _INTF_H

#include <stdio.h>
#include "list.h"
#include "list_set.h"
#include "hash_set.h"
#include "operand.h"
#include "opcode.h"

#define MAXIOPATLEN 256

extern class RU_mgr* ru_map;

// Added SAM, 3-98
extern int El_assume_infinite_resources;

class MDES;
class Res_use_descr;
class Reg_descr;
class Ft_descr;
class Op_descr;
class Io_descr;
class Alt_descr;

enum IO_Portkind { DATA_IN, DATA_OUT, SYNC_IN, SYNC_OUT };

// Current MDES setup

extern MDES* current_MDES;
void set_current_MDES(MDES* mdes);
void push_MDES(MDES* mdes);
void pop_MDES(void);

// MDES properties

#define MDES_num_resources() (current_MDES->get_MDES_num_resources())
#define MDES_num_reg_types() (current_MDES->get_MDES_num_reg_types())
#define MDES_num_lat_classes() (current_MDES->get_MDES_num_lat_classes())
#define MDES_num_io_formats() (current_MDES->get_MDES_num_io_formats())
#define MDES_num_field_types() (current_MDES->get_MDES_num_field_types())
#define MDES_num_vreg_types() (current_MDES->get_MDES_num_vreg_types())

// Opcode properties

int MDES_src_num(char *opcode) ;
int MDES_dest_num(char *opcode) ;
bool MDES_predicated(char *opcode) ;
bool MDES_has_speculative_version(char *opcode);
int MDES_priority(char *opname);

// Shivaram: Feb 09 2001 added the function to get the commutativity
bool MDES_commutative(char *opcode) ;

bool MDES_two_operand(char *opcode);
bool MDES_unsupported(char *opcode);

bool MDES_is_short_literal(char *opcode, int port, int value, char *shortlit_filename);
char* MDES_get_restricted_move(char *src_io, char *dest_io);

//int MDES_is_memory_medium_literal(int value);
//int MDES_is_branch_medium_literal(int value);
//int MDES_is_loadlit_medium_literal(int value);

void MDES_init_op_io(char *opcode, char * io);
int MDES_flow_time_io(IO_Portkind portkind, int portnum);
int MDES_anti_time_io(IO_Portkind portkind, int portnum);

// latencies across all alternatives (not qualified by io)
int MDES_flow_time(char *opcode, IO_Portkind, int portnum);
int MDES_anti_time(char *opcode, IO_Portkind, int portnum);

#define MDES_max_op_lat 20 /* maximum resource time allowed for an operation*/
int MDES_branch_latency(char *opcode);
int MDES_branch_latency();

// Register File properties

char* MDES_translate_vreg(char* vregname);

int MDES_reg_index(char* regname);
char* MDES_reg_name(int regindex);
void MDES_reg_names(List<char*>& regnames);

int MDES_reg_width(char* regname);
/* RMR { querry vector length */
int MDES_reg_vector_length(char* regname);
/* } RMR */
int MDES_reg_static_size(char* regname);
int MDES_reg_rotating_size(char* regname);
bool MDES_supports_rot_reg(char* regname);
bool MDES_reg_has_speculative_bit(char* regname);
bool MDES_reg_is_allocatable(char* regname);   // exclude literal and undefined files

// Returns a hash set of integers representing the number of the physical
// registers assigned to the given macro
void MDES_get_phys_regs(const Operand& mac, Hash_set<int>& regs);

// Direct MDES descr access

void MDES_collect_io_descrs(char* opcode, List_set<Io_descr*>& descset);
void MDES_collect_reg_descrs(char* opcode, IO_Portkind portkind, int portnum,
    List_set<Reg_descr*>& descset);
void MDES_collect_ft_descrs(char* opcode, IO_Portkind portkind, int portnum,
    List_set<Ft_descr*>& descset);
void MDES_collect_op_descrs(char* opcode, List_set<Op_descr*>& descset);
void MDES_collect_alt_descrs(char *opcode, List_set<Alt_descr*>& descset);
void MDES_collect_res_use_descrs(char *opcode, List_set<Res_use_descr*>& descset);
Op_descr *MDES_collect_generic_op_descrs(char* opcode);
Hash_set<eString>& get_MDES_pf_for_macro(Macro_name mname);

// moved these 2 functions here from Sched/schedule.cpp -KF 12/2004
Res_use_descr* MDES_get_res_use_descriptor(char *opcode, int res_idx);
void MDES_get_sched_opcode_from_res_idx(Op *op, char *iopat, int res_idx, char **outchar);

// Resource Management

void MDES_exchange_resources(int time, char** one, char** two);
void RU_init_iterator(char *opcode, void *op, char *io, int time);

bool RU_get_next_nonconfl_alt(char ** opcode, int * priority);
void RU_get_conflicting_ops(Hash_set<void*>& ops);

void RU_place(void);
void RU_remove(void *op, char *iopat, int scheduled_time);
void RU_alloc_map(int maxlength);
void RU_init_map(bool modulo, int length);
void RU_delete_map(void);
void *RU_at(int cycle, int res_index);
void RU_print_map(FILE *mout);
int RU_cur_sched_length();
//KVM : To check whether some resource is free at a sched_time
bool RU_check_resources_free(Res_use_descr*, int);

void RMSL_alloc(int lbound=0);
void RMSL_dealloc(void);
void RMSL_init(int lbound=0);
void RMSL_nextop(char * opcode, char * io);
int RMSL_value(void);

#endif
