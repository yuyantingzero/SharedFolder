/******************************************************************************
LICENSE NOTICE
--------------

IT IS  A BREACH OF  THE LICENSE AGREEMENT  TO REMOVE THIS  NOTICE FROM
THIS  FILE  OR SOFTWARE  OR  ANY MODIFIED  VERSIONS  OF  THIS FILE  OR
SOFTWARE.

Copyright notices/Licensor(s) Identification
--------------------------------------------
Each of  the entity(ies) whose name properly  appear immediately below
in connection with a copyright notice is a Licensor(s) under the terms
that follow.

Copyright  1998  New  York  University.  All rights  reserved  by  the
foregoing, respectively.

Copyright 2001 Center for  Research on Embedded Systems and Technology
at the  Georgia Institute  of Technology. All  rights reserved  by the
foregoing, respectively.

License agreement
-----------------

The  code contained  in this  file  including both  binary and  source
(hereafter,  Software)  is subject  to  copyright  by Licensor(s)  and
ownership remains with Licensor(s).

Licensor(s)  grants you  (hereafter, Licensee)  a license  to  use the
Software for  academic, research and internal  business purposes only,
without  a  fee.  "Internal  business  use"  means  that Licensee  may
install, use and execute the Software for the purpose of designing and
evaluating products.   Licensee may also disclose  results obtained by
executing  the  Software,  as  well as  algorithms  embodied  therein.
Licensee may  distribute the Software  to third parties  provided that
the copyright notice and this statement appears on all copies and that
no  charge  is  associated  with  such copies.   No  patent  or  other
intellectual property license is granted or implied by this Agreement,
and this  Agreement does not  license any acts except  those expressly
recited.

Licensee may  make derivative works,  which shall also be  governed by
the  terms of  this  License Agreement.  If  Licensee distributes  any
derivative work based  on or derived from the  Software, then Licensee
will abide by the following terms.  Both Licensee and Licensor(s) will
be  considered joint  owners of  such derivative  work  and considered
Licensor(s) for  the purpose of distribution of  such derivative work.
Licensee shall  not modify this  agreement except that  Licensee shall
clearly  indicate  that  this  is  a  derivative  work  by  adding  an
additional copyright notice in  the form "Copyright <year> <Owner>" to
other copyright notices above, before the line "All rights reserved by
the foregoing, respectively".   A party who is not  an original author
of such derivative works within  the meaning of US Copyright Law shall
not modify or add his name to the copyright notices above.

Any Licensee  wishing to  make commercial use  of the  Software should
contact each and every Licensor(s) to negotiate an appropriate license
for  such  commercial  use;  permission  of all  Licensor(s)  will  be
required for such a  license.  Commercial use includes (1) integration
of all or part  of the source code into a product  for sale or license
by or on  behalf of Licensee to third parties,  or (2) distribution of
the Software  to third  parties that need  it to utilize  a commercial
product sold or licensed by or on behalf of Licensee.

LICENSOR (S)  MAKES NO REPRESENTATIONS  ABOUT THE SUITABILITY  OF THIS
SOFTWARE FOR ANY  PURPOSE.  IT IS PROVIDED "AS  IS" WITHOUT EXPRESS OR
IMPLIED WARRANTY.   LICENSOR (S) SHALL  NOT BE LIABLE FOR  ANY DAMAGES
SUFFERED BY THE USERS OF THIS SOFTWARE.

IN NO EVENT UNLESS REQUIRED BY  APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY  COPYRIGHT HOLDER, OR ANY  OTHER PARTY WHO  MAY MODIFY AND/OR
REDISTRIBUTE THE  PROGRAM AS PERMITTED  ABOVE, BE LIABLE  FOR DAMAGES,
INCLUDING  ANY GENERAL, SPECIAL,  INCIDENTAL OR  CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OR  INABILITY TO USE THE PROGRAM (INCLUDING BUT
NOT  LIMITED TO  LOSS OF  DATA OR  DATA BEING  RENDERED  INACCURATE OR
LOSSES SUSTAINED BY  YOU OR THIRD PARTIES OR A  FAILURE OF THE PROGRAM
TO  OPERATE WITH ANY  OTHER PROGRAMS),  EVEN IF  SUCH HOLDER  OR OTHER
PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

By using  or copying  this Software, Licensee  agrees to abide  by the
copyright law and all other applicable laws of the U.S., and the terms
of  this license  agreement. Any  individual Licensor  shall  have the
right  to terminate this  license immediately  by written  notice upon
Licensee's   breach   of,  or   non-compliance   with,   any  of   its
terms.  Licensee may  be held  legally responsible  for  any copyright
infringement  that is caused  or encouraged  by Licensee's  failure to
abide     by    the     terms    of     this     license    agreement.
******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
// File:   simu_el_processor.h
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


#ifndef _SIMU_EL_PROCESSOR_H
#define _SIMU_EL_PROCESSOR_H


#include "simu.h"
#include "streamBuffer.h"
#include "el_jumptbl.h"
#include "emitter.h"
#include "asm_emitter.h"


// Class for processing the Rebel Stream
class el_Buffer
{
public:
  el_Buffer(eString& el_file_name, ofstream&, ofstream&, ofstream&);
  bool gen_code(eString&, Host_Info&);
    
private:
  // functions
  void process_input(void);
  void process_code(void);
  void process_data(void);

  int  get_offset(El_expression* expr);
  int  add_padding(int cur_size, int next_size);
  void write_data(El_datalist *datalist);

  int  get_static_physical_regs(PD_regfile& regs);
  int  get_rotating_physical_regs(PD_regfile& regs);
    
  void read_reg_info(PD_regfile& regs);
  void print_reg_info(PD_regfile& regs);
  void set_rot_phys_offset(PD_regfile& regs);
  void set_stat_phys_offset(PD_regfile& regs);

  void get_macro_regs();
  void get_reg_breakdowns(Vector<Slist<Stuple<int, Pair<char*, int> > > >& static_reg_breakdown,
			  Vector<Slist<Stuple<int, Pair<char*, int> > > >& rot_reg_breakdown);

  void write_region(Region *region);
  void write_start_compound(Region *region,int& count, bool flagme);
  void write_memory_trace(char *);
  void write_aclock(int clocks);
  void write_procedure(Region *region);
  void write_all_jump_tables(Procedure*);
  void write_jump_table(El_jumptbl&);
  void write_loopbody(Region *region);
  void write_bb(Region *region);
  void write_hb(Region *region);
  int  write_op(Region* region);
  int  write_dest_operands(Op *op);
  void write_src_operands(Op *op, eString& opname);
  bool write_pred_operand(Op *op, eString& opname);
  void append_pv_if_necessary(Op *op, eString& opname);
  void write_return_profile_op();
  void collect_op_stats(Op *op);
  void write_operand(Operand& oprnd, eString &iopat, int oprnd_num, 
			   bool is_branch_op, bool is_load_op);
    
  int  get_vector_key(Op* op, eString& vector_type);
  int  get_call_info(Region *reg, eString& info);
  int  append_parameter_attributes(Region *reg, hc_aggregate& entry);
  int  assign_key(eString& call_info);
  void create_prototype(hc_aggregate& entry, eString& fname, 
				eString& call_info);

  void dword_align(int& space);
  void map_macros_to_stack(Macro_name mname, 
				   Macro_name &stackvar,
				   int &stackoffset);
  void check_component(Op *op, Macro_name mname, char *mstring, int &mval);
  void calc_framesize(Region *region);
  void write_prologue();
  void write_epilogue();

private:    
  // Full Rebel file path
  eString Rebel_path_name;
  
  // global data initialization function
  eString sequencing_id;

  Host_Info host_platform_info;

private:    
  // Stackframe Info
  Frame_params frame;

  // flag to detect caller/callee spill code
  Elcor_region_flag el_flag;
  
private:
  // data stream declarations
  streamBuffer *data_outbuf;

  // statistic structures
  streamBuffer *datastats_outbuf;
  bool print_stats_buffer;

  // register information
  streamBuffer *datareg_outbuf;
  bool print_register_information;

  // jump tables
  bool print_jump_tables;
  streamBuffer *datajmptbls_outbuf;

private:
  streamBuffer *datainit_outbuf;

  bool initialize_global_data;

private:
  char output_buffer[MAX_BUF_SIZE];

private: 
  streamBuffer *emulation_tables_outbuf;
  // for assembly code generation
  streamBuffer *emulation_asm_outbuf;

  char* cur_proc;       // current procedure being processed
  int   opcount;         // no of operations in a proc
  int   subregion_count; // no of subregions in a proc

  // static stats
  PD_static_stats stats;
    
  // reg file params
  PD_regfile regfile;
    
private:
  Vector<OpEntry*> emulation_table;
  OpEntry* NULL_OpEntry;

  inline void resize_emulation_table_if_necessary();
  inline void clear_emulation_table();

  Vector<eString> control_block_label;
  // for assembly code generation
  Vector<int> control_block_id;
  int cbl_count;
};


inline void el_Buffer::resize_emulation_table_if_necessary()
{
  if (opcount >= emulation_table.dim()) {
    emulation_table.resize(opcount*2, NULL_OpEntry);
  }

  // allocate a new OpEntry node
  if (emulation_table[opcount] == NULL) {
    emulation_table[opcount] = new OpEntry;
  }
}


inline void el_Buffer::clear_emulation_table()
{
  for (int i = 0; i < emulation_table.dim(); i++) {
    if (emulation_table[i])
	 emulation_table[i]->clear();
  }
}


#endif /* _SIMU_EL_PROCESSOR_H */
