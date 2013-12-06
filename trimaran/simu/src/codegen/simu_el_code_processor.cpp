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
// File:   simu_el_processor.cpp
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////

#include "simu.h"
#include "simu_el_processor.h"
#include "streamBuffer.h"
#include "PD_reg.h"

#include <unistd.h>
#include "ir_reader.h"
#include "mdes.h"
#include "intf.h"

#include "el_io.h"
#include "el_data.h"
#include "region_purge.h"
#include "attributes.h"
#include "ir_attribute.h"
#include "ir_writer.h"

#include "op.h"
#include "iterators.h"

static const int  SIZE        = 1000;
static const char LEFT_BRACE  = '{';
static const char RIGHT_BRACE = '}';
static const char SEMICOLON   = ';';

static const char* PD_START_PROCEDURE_STRING = "__PD_start_procedure";
static const char* PD_START_COMPOUND_STRING  = "__PD_start_block";
static const char* PD_PROLOGUE_STRING = "__PD_prologue";
//static const char* PD_EPILOGUE_STRING = "__PD_epilogue";
static const char* ADDRESS_OF_LABEL_STRING = "&";
static const char* PD_WORKAROUND  = "__PD";
static const char* SETJMP_STRING  = "_setjmp";
static const char* LONGJMP_STRING = "_longjmp";
static const char* PD_ACLOCK_STRING = "__PD_aclock";
static const char* ROTATING_REGISTER_FILE_STRING = "_ROT";
#define MACRO_FILE (Reg_file) -1

// Set of simulator parameters used
extern int S_nual_equals;
extern int S_unscheduled;
extern int S_virtual_regs;
extern int S_performance_monitoring;
extern int S_remap_optables;
extern int S_generate_assembly;
extern char *S_trace_function;
/* WWF { wrap around standard heap management */
extern int S_use_custom_malloc;
/* } WWF */

// writes out external variables
extern void write_externals(streamBuffer*, const Hash_table<eString>&, 
				    enum EXTERN_TYPE);

// write out function prototypes
extern void  gen_prototypes(streamBuffer*);

extern void parse_info_string(char *raw_info_string, hc_aggregate& fn);
extern void extract_struct_or_union_info(char* raw_call_info);

// normalize the input file name and converts it to a sequencing id
// used for creating a unique function that initializes global data
extern void normalize(eString& src, eString& dest);

// List of all global_data_labels collected during data section processing
// the members of this list will be declared as externs for program 
// initialization
extern Hash_table<eString> global_data_labels;

// List of all function prototypes encountered in the file being processed
Hash_map<eString, hc_aggregate*> prototypes_table(hash_estring, SIZE);

// binds the call_info string to a aggrefate function declaration entry
// currently its information is not used, instead, encountered_call_info
// has replaced its functionality
Hash_map<eString, hc_aggregate*> stored_call_info(hash_estring, SIZE);
Hash_table<eString> encountered_call_info(hash_estring, SIZE);

// binds the call_info string to a integer value
Hash_map<eString, int> call_info_map(hash_estring, SIZE);

// binds the op id of the entry operation to the offset index in the 
// emulation table
Hash_map<int, int> jump_table_map(hash_int, SIZE);

// store the set of bbs and the number of operations per bb
Hash_map<eString, int> bb_ops(hash_estring, SIZE);

// List of wrapper function candidates
// Wrappers generated for all functions implemented in Rebel file
Hash_map<eString, int> wrappers(hash_estring, SIZE);

// List of vector function candidates
// Vectors generated for all functions called from Rebel file
Hash_table<eString> vectors(hash_estring, SIZE);

// list of all labels encountered in the current file
Hash_table<eString> extern_functions(hash_estring, SIZE);
Hash_table<eString> extern_data_labels(hash_estring, SIZE);

// list of all structs/unions encountered in the current file
Hash_table<eString> extern_struct_or_union_defs_table(hash_estring, SIZE);
Hash_table<eString> global_struct_or_union_defs_table(hash_estring, SIZE);

// offset of each physical file into the simulator register file.
// we map all physical GPR's into the single simulator GPR file.
Hash_map<eString, int> stat_phys_offset(hash_estring, 32);
Hash_map<eString, int> rot_phys_offset(hash_estring, 32);

Vector<Map<eString, int> > macro_offset((int)LAST_MACRO);

// easier to determine vector length up front.  assumes all vector lengths are
// the same.  -KF 1/2007
int vector_length = 0;

// RMR number-gathering-tools {
Hash_map<eString, Hash_set<int>*> bb_flagme_info(hash_estring, SIZE);
Hash_map<eString, Hash_set<int>*> op_flagme_info(hash_estring, SIZE);

bool special_interest_bb(eString fn, int bb)
{
  if (!bb_flagme_info.is_bound(fn)) {
    return false;
  }

  Hash_set<int>* blocks = bb_flagme_info.value(fn);
  if (blocks->is_member(bb)) {
    return true;
  }
  else {
    return false;
  }
}

bool special_interest_op(eString fn, int op)
{
  if (!op_flagme_info.is_bound(fn)) {
    return false;
  }

  Hash_set<int>* ops = op_flagme_info.value(fn);
  if (ops->is_member(op)) {
    return true;
  }
  else {
    return false;
  }
}


static ulong clog2(ulong x)
{
  int c;

  if (x <= 1) return 0;

  for (c = -1; x != 0; c++) x >>= 1;

  return c;
}
// } RMR number-gathering-tools


// set up streams
el_Buffer::el_Buffer(eString& el_file_name, ofstream& tbls_outbuf, 
			   ofstream& inc_outbuf, ofstream& asm_outbuf)
{
  static const int INITIAL_OP_COUNT    = 50000;
  static const int INITIAL_LABEL_COUNT = 100;

  // initialize member fields
  Rebel_path_name = el_file_name;   // store the file name

  normalize(el_file_name, sequencing_id);

  // constant null emulation table entry
  NULL_OpEntry = (OpEntry*) NULL;

  // procedure emitter counters
  cur_proc = NULL;
  opcount = subregion_count = cbl_count = 0;

  // jump table emitter flag, statistics buffer and register infromation
  // buffer initializer flags
  print_jump_tables = false;
  print_stats_buffer = false;
  print_register_information = false;

  // global data initialization emitter flag
  initialize_global_data = false;

  // create all the streams used by this class
    
  // used to write out data, goes into header file
  data_outbuf        = new streamBuffer(inc_outbuf);

  datajmptbls_outbuf = new streamBuffer(inc_outbuf);
  datastats_outbuf   = new streamBuffer(inc_outbuf);
  datareg_outbuf     = new streamBuffer(inc_outbuf);

  // used to write out global data initialization
  datainit_outbuf    = new streamBuffer(inc_outbuf);

  // Per procedure you require the following streams
  emulation_tables_outbuf = new streamBuffer(tbls_outbuf);

  // for assembly code generation
  if (S_generate_assembly){
    emulation_asm_outbuf  = new streamBuffer(asm_outbuf);
  }

  // initialize the emulation table
  emulation_table.reshape(INITIAL_OP_COUNT, NULL_OpEntry);
  emulation_table[0] = new OpEntry;

  control_block_label.reshape(INITIAL_LABEL_COUNT);
  // for assembly code generation
  control_block_id.reshape(INITIAL_LABEL_COUNT);

  // RMR number-gathering-tools {
  extern int S_read_bb_flagme_file;
  if (S_read_bb_flagme_file) {
    ifstream in;    
    in.open("bb_flagme");
    if (!in) S_punt("could not open specified file <bb_flagme> containing flagged basic blocks");
    else {
	eString fn;
	int     bb;
	
	while (in >> fn) {
	  in >> bb;
	  
	  if (!bb_flagme_info.is_bound(fn)) {
	    bb_flagme_info.bind(fn, new Hash_set<int>(hash_int, SIZE));
	  }
	  
	  Hash_set<int>* blocks = bb_flagme_info.value(fn);
	  (*blocks) += bb;
	}
	in.close();
    }
  }

  extern int S_read_op_flagme_file;
  if (S_read_op_flagme_file) {
    ifstream in;
    in.open("op_flagme");
    if (!in) S_punt("could not open specified file <op_flagme> containing flagged operations");
    else {
	eString  fn;
	unsigned int op;
        float    avg_miss_latency;
        float    percent_of_program_miss_latency;
	
	while (in >> fn) {
	  in >> op;
	  in >> avg_miss_latency;
	  in >> percent_of_program_miss_latency;

	  if (!op_flagme_info.is_bound(fn)) {
	    op_flagme_info.bind(fn, new Hash_set<int>(hash_int, SIZE));
	  }
	  
	  Hash_set<int>* ops = op_flagme_info.value(fn);
	  (*ops) += op;
	}
    }
  }
  // } RMR number-gathering-tools
}

// Top level driver
bool el_Buffer::gen_code(eString& id, Host_Info& host_info) 
{
  S_log("Writing simulation tables...");

  read_reg_info(regfile);

  IR_instream *EL_IN_STREAM_C = new IR_instream(Rebel_path_name);
  IR_instream *EL_IN_STREAM_D = new IR_instream(Rebel_path_name);

  while (ir_read(*EL_IN_STREAM_C) != EL_INPUT_EOF) {
    process_code();
  }
  while (ir_read(*EL_IN_STREAM_D) != EL_INPUT_EOF) {
    process_data();
  }

  // commit global data declaration
  data_outbuf->commit();

  // write out "extern" global data
  // this call has to be called once the Rebel data sections have
  // been processed
  sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
	    "/*******************************************************/",
	    "/************ External Varaiable Encountered ***********/",
	    "/*******************************************************/");    
  (*data_outbuf) << output_buffer;

  write_externals(data_outbuf, extern_data_labels, IS_DATA);
  write_externals(data_outbuf, extern_struct_or_union_defs_table, IS_SU);

  // write vector/wrapper prototypes to avoid forward references in tables
  // also mark ellipsis functions as a side task
  gen_prototypes(data_outbuf);

  if (print_register_information) {
    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
		"/*******************************************************/",
		"/**************** Register Information *****************/",
		"/*******************************************************/");
    (*data_outbuf) << output_buffer;

    data_outbuf->commit();
    datareg_outbuf->commit();
  }

  if (print_jump_tables) {
    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
		"/*******************************************************/",
		"/*************** Jump Table Declarations ***************/",
		"/*******************************************************/");
    (*data_outbuf) << output_buffer;

    data_outbuf->commit();
    datajmptbls_outbuf->commit();
  }

  if (S_performance_monitoring && print_stats_buffer) {
    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n", 
		"/*******************************************************/",
		"/************ Statistic Tracking Structures ************/",
		"/*******************************************************/");
    
    for (Hash_map_iterator<eString, int> bb_ops_iter(bb_ops); bb_ops_iter != 0; bb_ops_iter++) {
	(*data_outbuf) << "__PD_opstats " << (*bb_ops_iter).first << "[" << (*bb_ops_iter).second + 1 << "] = {0};" << endl;
    }

    (*data_outbuf) << output_buffer;

    data_outbuf->commit();
    datastats_outbuf->commit();
  }

  // commit data initialization buffer
  if (initialize_global_data) {
    sprintf(output_buffer, "\n\n%s\n%s\n%s\n\n",
		"/*******************************************************/",
		"/************* Global Data Inititalization *************/",
		"/*******************************************************/");    
    (*data_outbuf) << output_buffer;

    id = sequencing_id;
    sprintf(output_buffer, "void __PD_initialize_%s_global_data()\n",
		(char*) sequencing_id);

    (*data_outbuf) << output_buffer << LEFT_BRACE << endl;

    // print out the header and function declaration
    data_outbuf->commit();

    // commit the data initialization code
    datainit_outbuf->commit();

    // terminate the function
    (*data_outbuf) << endl << RIGHT_BRACE << endl;

    // print to the file
    data_outbuf->commit();
  }

  delete EL_IN_STREAM_C;
  delete EL_IN_STREAM_D;

  host_info.platform = host_platform_info.platform;
  return initialize_global_data;
}

void el_Buffer::process_code(void)
{
  switch (El_input_type) {
  case EL_INPUT_EOF:  // No more input
    break;

  case EL_INPUT_CODE: { // Function text token
    Procedure *R ;
	
    // Read a Rebel procedure
    R = (Procedure *) El_input;
    write_region(R);     
      
    // Deallocate the Elcor procedure
    region_purge(R);
  } break;

  case EL_INPUT_DATA: break;

  default: 
    S_punt("process_code: illegal token");

  }
}

void el_Buffer::process_data(void)
{
  switch (El_input_type) {
  case EL_INPUT_EOF:  // No more input
    break;

  case EL_INPUT_DATA: { // Data token
    El_datalist *D;
	
    // Read data declaration information
    D = (El_datalist *) El_input;
    write_data(D);
	
    // Deallocate the Elcor data 
    delete D;
  } break;

  case EL_INPUT_CODE: break;

  default:
    S_punt("process_data: illegal token");

  }
}


// Process one DATA or CODE region at a time
void el_Buffer::process_input(void)
{
  switch (El_input_type) {
  case EL_INPUT_EOF:  // No more input
    break;

  case EL_INPUT_DATA: { // Data token
    El_datalist *D;
	
    // Read data declaration information
    D = (El_datalist *) El_input;
    write_data(D);
	
    // Deallocate the Elcor data 
    delete D;
  } break;
	
  case EL_INPUT_CODE: { // Function text token
    Procedure *R ;
	
    // Read a Rebel procedure
    R = (Procedure *) El_input;
    write_region(R);     
      
    // Deallocate the Elcor procedure
    region_purge(R);
  } break;
	
  default:
    S_punt("process_input: illegal token");
  }
}


// routes a call to the correct region handler
// Not used for the time being
void el_Buffer::write_region(Region *region)
{
  
  if (region->is_bb())
    write_bb(region);

  else if (region->is_hb())
    write_hb(region);
  
  else if (region->is_loopbody())
    write_loopbody(region);

  else if (region->is_procedure())
    write_procedure(region);

  else if (region->is_op()) {
    // all operation should be in blocks
    // write_op(region);
    S_punt("Operation (id %d) must belong to some block and it does not", region->id());
  }
  
  else if (region->is_tree()) {
    S_punt("Cannot handle tree-region (id %d)", region->id());
  }
}


// Writes the code part of the simulator assembly file
// The function does the following:
// - Renames the fuction back to its original name if defined as static
// - Marks the function for wrapper generation
// - Writes out a statistics operation for this procedure
// - Writes out its subregions
void el_Buffer::write_procedure(Region* region)
{
  char* procedure;

  if (region == NULL) 
    return;

  // holds stats related params
  // reset the stats field
  stats.reset();

  // reset opcounts
  clear_emulation_table();
  subregion_count = opcount = cbl_count = 0;
  
  eString procname = ((Procedure *) region)->get_name();

  // the the global current procedure pointer name to the procedure
  // being processed
  cur_proc  = (char*) procname;
  procedure = (char*) procname;

  emulation_table[opcount]->attribute = procname;

  // ignore leading underscore
  if (*procedure == '_') ++procedure;
  procname = procedure;
  
  S_log("Adding %s to wrappers list!", procedure);
  
  static eString call_info;   // function parameter information from IMPACT
  static hc_aggregate* entry; // new type entry will be added for procedure 
  
  entry = new hc_aggregate;
  if (!get_call_info(region, call_info)) {
    S_punt("expected call_info attribute for procedure %s!", procedure);
  }

  create_prototype(*entry, procname, call_info);
  append_parameter_attributes(region, *entry);
  
  if (prototypes_table.is_bound(entry->name)) {
    S_punt("procedure %s defined twice in file!", procedure);
  }
  prototypes_table.bind(entry->name, entry);
  wrappers.bind(prototypes_table.key(procname), 0);

  // will generate an extern declaration for this function for program
  // data initialization
  if (!extern_functions.is_member(procname))
    extern_functions.add_head(procname);
  
  if( ! ((Compound_region*) region)->first_subregion()->flag(EL_REGION_HAS_PROLOGUE_OP)) {
    cerr << procname << " doesn't have a prologue op!\n";
    cerr << "I wish I could tell you why you needed one, but no one commented this assert."
	 << endl;
    ir_write(region, "no_prologue_op.reb");
  }

  assert(((Compound_region*) region)->first_subregion()->flag(EL_REGION_HAS_PROLOGUE_OP));
  resize_emulation_table_if_necessary();
  opcount++; 
  resize_emulation_table_if_necessary();

  /* RMR { calculate vector length: while can simulate each vector
   * register file with different vector lengths, for now enforce that
   * they all have the same vector length value sicne this allows for the
   * use of just one rotating register base for all vector register files
   */
//   int vector_length = MDES_reg_vector_length(regfile_to_char(VIR));
//   assert(vector_length == MDES_reg_vector_length(regfile_to_char(VFR)));
//   assert(vector_length == MDES_reg_vector_length(regfile_to_char(VMR)));
//   assert(vector_length);
  /* } RMR */
  // Replace above for multi-cluster.  Also, the above doesn't seem like it
  // would work if your machine has no vector registers.  -KF 1/2007
  vector_length = 0;
  for (int i = 0; i < MDES_num_reg_types(); i++) {
    Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
    if (strstr(rd->get_vname(), vregfile_to_mdesname(VIR)) ||
        strstr(rd->get_vname(), vregfile_to_mdesname(VFR))) {
        //strstr(rd->get_vname(), vregfile_to_mdesname(VMR))) {
      int cur_vec_length = MDES_reg_vector_length(rd->get_aname());
      assert(cur_vec_length);
      if (vector_length == 0)
        vector_length = cur_vec_length;
      else
        assert(cur_vec_length == vector_length);
    }
  }

  // reserve the next operation as the stats operation
  int tmp_count;
  if (S_performance_monitoring) {
    tmp_count = opcount;
    opcount++; 
    resize_emulation_table_if_necessary();
    
    // write out the stats data structure
    print_stats_buffer = true;

    (*datastats_outbuf)
      << "__PD_stats " << "__PD_stats" << cur_proc
      << "[] = {" << endl;
	
    // write out the PROBE operation (region entry marker)
    write_start_compound(region, tmp_count, false);
  }
	
  // Next, process all the subregions
  Region *subregion_ptr;
  Compound_region *proc = (Compound_region*) region;
  
  for (subregion_ptr = proc->first_subregion();
       subregion_ptr !=0; 
       subregion_ptr = proc->successor_in_subregion_list(subregion_ptr) ) {  
    // writes out one subregion at a time
    write_region(subregion_ptr);
  }
    
  // write out the elcor jump tables
  if (El_proc_has_jump_tables((Procedure *)region)) {
    print_jump_tables = true;
    write_all_jump_tables((Procedure*) region);
  }
  jump_table_map.clear();
    
  // last entry in the stats field indicating no more region
  // used by emulation library before dumping out DYN_STATS
  if (S_performance_monitoring) {
    sprintf(output_buffer, ",\n\t{0}\n}; /* end __PD_stats%s */\n\n", cur_proc);
    (*datastats_outbuf) << output_buffer;
  }
    
  // Write out control block labels
  for (int i = 0; i < cbl_count; i++) {
    (*emulation_tables_outbuf) << control_block_label[i];
  }
  (*emulation_tables_outbuf) << endl;

  // for assembly code generation
  if (S_generate_assembly){
    print_asm(emulation_table, opcount, emulation_asm_outbuf, control_block_id);
    emulation_asm_outbuf->commit();
  }

  // Write out the stats operations and procedure operations
  print_table(emulation_table, opcount, emulation_tables_outbuf);
  emulation_tables_outbuf->commit();

  // Update wrapper opcount for this procedure
  assert(wrappers.is_bound(prototypes_table.key(procname)));
  wrappers.bind(prototypes_table.key(procname), opcount);

  // Set the register file size bounds
  // Always choose physical machine set for rotating registers
  int total_regs = get_rotating_physical_regs(regfile);

  print_register_information = true;

  if (!S_virtual_regs) {
    // Physical register parameters need to be written out only once
    static bool written = false;
    if (!written) {
      total_regs += get_static_physical_regs(regfile);

	/* RMR { support vector file type; added extra fields to array initializer */
      sprintf(output_buffer, "%s[] = {%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d};\n\n",
		  "static __PD_reg_file_info __PD_physical_reginfo",
		  regfile.gpr_stat_size, regfile.gpr_rot_size, 
		  regfile.fpr_stat_size, regfile.fpr_rot_size,
		  regfile.btr_stat_size,
		  regfile.pr_stat_size,  regfile.pr_rot_size,
		  regfile.cr_stat_size,  regfile.cr_rot_size, 
		  regfile.vir_stat_size, regfile.vir_rot_size, 
		  regfile.vfr_stat_size, regfile.vfr_rot_size,
		  regfile.vmr_stat_size, regfile.vmr_rot_size,
		  total_regs, 
		  vector_length);
	/* } RMR */
      (*datareg_outbuf) << output_buffer;

      written = true;
    }

  }

  // if virtual register emulation, write out an entry per procedure
  else {
    // sizes of static regfile is set to one more than the largest register
    // number accessed since C arrays are 0-indexed
    total_regs += regfile.increment(vector_length);

    // write out virtual register info 
    //    sprintf(output_buffer, "static __PD_reg __PD_virtual_registers%s[%d];", 
    //		cur_proc, total_regs);
    //    (*datareg_outbuf) << output_buffer;

    /* RMR { support vector file type; added extra fields to array initializer */
    sprintf(output_buffer, "%s%s[] = {%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d};\n\n",
		"static __PD_reg_file_info __PD_reginfo", cur_proc,
		regfile.gpr_stat_size, regfile.gpr_rot_size, 
		regfile.fpr_stat_size, regfile.fpr_rot_size,
		regfile.btr_stat_size,
		regfile.pr_stat_size,  regfile.pr_rot_size,
		regfile.cr_stat_size,  regfile.cr_rot_size,
		regfile.vir_stat_size, regfile.vir_rot_size, 
		regfile.vfr_stat_size, regfile.vfr_rot_size, 
		regfile.vmr_stat_size, regfile.vmr_rot_size,
		total_regs, vector_length 
		/* RMR { no need to create a static array for virtual register
		 * since the emulation library will create one
		 */ 
		// "__PD_virtual_registers", cur_proc 
		/* } RMR */);
    /* } RMR */
    (*datareg_outbuf) << output_buffer;

    regfile.reset();
    //get_static_physical_regs(regfile);
    get_rotating_physical_regs(regfile);
  }
}


// Write out the PROBE / Marker operation
void el_Buffer::write_start_compound(Region *region, int &count, bool flagme)
{
  char id[1024];

  if (region->is_procedure()) {
    //    sprintf(id,  "%s::%s", (char *) Rebel_path_name, cur_proc);
    sprintf(id, "%s", cur_proc);
  }
  
  /* RMR { if region is part of a loop body that is software pipelined
   * change the region text asci accordingly
   */
  else if (region->flag(EL_REGION_SOFTPIPE)) {
    sprintf(id, "lb-%d", region->id());
  }
  /* } RMR */

  else if (region->is_bb()) {
    sprintf(id, "bb-%d", region->id());
  }

  else if (region->is_hb()) {
    sprintf(id, "hb-%d", region->id());
  }

  // set the stats struct
  if (subregion_count != 0) (*datastats_outbuf) << ",\n\t";
  else (*datastats_outbuf) << "\t";

  // sanity check
  if (stats.Static_ops != stats.real_op_count) {
    S_punt("Static operation count not consistent for %s %d", cur_proc, region->id());
  }

  (*datastats_outbuf)
    << "{" 
    << "\"" << id << "\"" << ", "
    << region->is_procedure() << ", "
    << flagme << ", "
    << stats.sched_len << ", " 
    << stats.Static_ops << ", " 
    << stats.Static_ialu << ", " 
    << stats.Static_falu << ", " 
    << stats.Static_load << ", " 
    << stats.Static_store << ", " 
    << stats.Static_cmpp << ", " 
    << stats.Static_pbr << ", " 
    << stats.Static_branch << ", " 
    << stats.Static_icm << ", " 
    /* RMR { add category for SIMD ops */
    << stats.Static_vialu << ", " 
    << stats.Static_vfalu << ", " 
    << stats.Static_vload << ", " 
    << stats.Static_vstore << ", " 
    << stats.Static_vxfr << ", " 
    << stats.Static_vperm << ", " 
    /* } RMR */
    << stats.Static_caller_spill << ", " 
    << stats.Static_callee_spill << ", " 
    << "{0}, {0}, 0, 0, 0, "
    << "0, 0, 0, 0, 0, "
    << "0, 0, 0, 0, 0, ";

  if (region->is_procedure()) {
    (*datastats_outbuf) << "0";
  }
  else {
    sprintf(id, "__pd_opstats_%s_bb_%d", cur_proc, region->id());
    (*datastats_outbuf) << id;

    eString label(id);
    if (bb_ops.is_bound(label)) {
	S_punt("Duplicate block signatures encountered (label is %s)", id);
    }
    bb_ops.bind(label, stats.real_op_count);
  }

  (*datastats_outbuf) << "}";
  
  if(region->is_procedure()) {
    emulation_table[count]->op   = PD_START_PROCEDURE_STRING;
  }
  
  else {
    emulation_table[count]->op = PD_START_COMPOUND_STRING;
  }

  emulation_table[count]->id   = region->id();  
  emulation_table[count]->type = PSEUDO_OP;

  // set the PD_stats field
  sprintf(output_buffer, "&__PD_stats%s[%d]", cur_proc, subregion_count);
  emulation_table[count]->src[1].file = output_buffer;
  
  count++;
  resize_emulation_table_if_necessary();
  subregion_count++;
}

void el_Buffer::read_reg_info(PD_regfile& regfile)
{
  regfile.reset();

  get_rotating_physical_regs(regfile);
  get_static_physical_regs(regfile);

  get_macro_regs();
}

// Extracts MDES specific static register file related parameters
int el_Buffer::get_static_physical_regs(PD_regfile& regs)
{
   if (S_virtual_regs)
     return 0;

   int num_phys_files = MDES_num_reg_types();
   int total = 0;

   int gpr_tot = 0;
   int fpr_tot = 0;
   int pr_tot = 0;
   int cr_tot = 0;
   int btr_tot = 0;
   int vir_tot = 0;
   int vfr_tot = 0;
   int vmr_tot = 0;

   regs.gpr_stat_size = 0;
   regs.fpr_stat_size = 0;
   regs.pr_stat_size = 0;
   regs.cr_stat_size = 0;
   regs.btr_stat_size = 0;
   regs.vir_stat_size = 0;
   regs.vfr_stat_size = 0;
   regs.vmr_stat_size = 0;
  
   // fill in size of each file
   for (int i = 0; i < num_phys_files; i++) {
      Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
      if (rd->get_regkind() != REG_PHYSICAL)
	 continue;

      char* virt_file_name = rd->get_vname();
      Reg_file vrf = mdesname_to_vregfile(virt_file_name[0]);
      char* phys_file_name = rd->get_aname();
      int size = MDES_reg_static_size(phys_file_name);

      if (size == 0) continue;

      switch (vrf) {
      case GPR:
	 regs.gpr_stat_size += size; 
         stat_phys_offset.bind(eString(phys_file_name), gpr_tot);
	 gpr_tot += size;
	 break;
      case FPR:
	 regs.fpr_stat_size += size;  
         stat_phys_offset.bind(eString(phys_file_name), fpr_tot);
	 fpr_tot += size;
	 break;
      case PR:
         // Need to round size up to nearest multiple of 32 so that
         // PVs will work.  -KF 11/2005
         if (size % 32 != 0)
           size = ((size / 32) + 1) * 32;
         regs.pr_stat_size += size;
         stat_phys_offset.bind(eString(phys_file_name), pr_tot);
         pr_tot += size;
	 break;
      case CR:
	 regs.cr_stat_size += size;
         stat_phys_offset.bind(eString(phys_file_name), cr_tot);
	 cr_tot += size;
	 break;
      case BTR:
	 regs.btr_stat_size += size;
         stat_phys_offset.bind(eString(phys_file_name), btr_tot);
	 btr_tot += size;
	 break;
      case VIR:
         regs.vir_stat_size += size * MDES_reg_vector_length(phys_file_name);
         stat_phys_offset.bind(eString(phys_file_name), vir_tot);
         vir_tot += size * MDES_reg_vector_length(phys_file_name);
         break;
      case VFR:
         regs.vfr_stat_size += size * MDES_reg_vector_length(phys_file_name);
         stat_phys_offset.bind(eString(phys_file_name), vfr_tot);
         vfr_tot += size * MDES_reg_vector_length(phys_file_name);
         break;
      case VMR:
//          regs.vmr_stat_size += size * MDES_reg_vector_length(phys_file_name);
//          stat_phys_offset.bind(eString(phys_file_name), vmr_tot);
//          vmr_tot += size * MDES_reg_vector_length(phys_file_name);
	 regs.vmr_stat_size += size;
         stat_phys_offset.bind(eString(phys_file_name), vmr_tot);
	 vmr_tot += size;
         break;
      default:
	 S_punt("el_Buffer::get_static_physical_regs: invalid virtual file type");
      }
   }

   total = gpr_tot + fpr_tot + pr_tot + cr_tot + btr_tot + vir_tot + vfr_tot + vmr_tot;
   return total;
}


// Extracts MDES specific rotating register file related parameters
int el_Buffer::get_rotating_physical_regs(PD_regfile& regs)
{
   int num_phys_files = MDES_num_reg_types();
   int total = 0;

   int gpr_tot = 0;
   int fpr_tot = 0;
   int pr_tot = 0;
   int cr_tot = 0;
   int vir_tot = 0;
   int vfr_tot = 0;
   int vmr_tot = 0;

   regs.gpr_rot_size = 0;
   regs.fpr_rot_size = 0;
   regs.pr_rot_size = 0;
   regs.cr_rot_size = 0;
   regs.vir_rot_size = 0;
   regs.vfr_rot_size = 0;
   regs.vmr_rot_size = 0;

   // fill in size of each file
   for (int i = 0; i < num_phys_files; i++) {
      Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
      if (rd->get_regkind() != REG_PHYSICAL)
	 continue;

      char* virt_file_name = rd->get_vname();
      Reg_file vrf = mdesname_to_vregfile(virt_file_name[0]);
      char* phys_file_name = rd->get_aname();
      int size = MDES_reg_rotating_size(phys_file_name);

      if (size == 0) continue;

      switch (vrf) {
      case GPR:
	 regs.gpr_rot_size += size; 
         rot_phys_offset.bind(eString(phys_file_name), gpr_tot);
	 gpr_tot += size;
	 break;
      case FPR:
	 regs.fpr_rot_size += size;  
         rot_phys_offset.bind(eString(phys_file_name), fpr_tot);
	 fpr_tot += size;
	 break;
      case PR:
	 regs.pr_rot_size += size;
         rot_phys_offset.bind(eString(phys_file_name), pr_tot);
	 pr_tot += size;
	 break;
      case CR:
	 regs.cr_rot_size += size;
         rot_phys_offset.bind(eString(phys_file_name), cr_tot);
	 cr_tot += size;
	 break;
      case VIR:
         regs.vir_rot_size += size * MDES_reg_vector_length(phys_file_name);
         rot_phys_offset.bind(eString(phys_file_name), vir_tot);
         vir_tot += size * MDES_reg_vector_length(phys_file_name);
         break;
      case VFR:
         regs.vfr_rot_size += size * MDES_reg_vector_length(phys_file_name);
         rot_phys_offset.bind(eString(phys_file_name), vfr_tot);
         vfr_tot += size * MDES_reg_vector_length(phys_file_name);
         break;
      case VMR:
//          regs.vmr_rot_size += size * MDES_reg_vector_length(phys_file_name);
//          rot_phys_offset.bind(eString(phys_file_name), vmr_tot);
//          vmr_tot += size * MDES_reg_vector_length(phys_file_name);
         regs.vmr_rot_size += size;
         rot_phys_offset.bind(eString(phys_file_name), vmr_tot);
         vmr_tot += size;
         break;
      default:
	 S_punt("el_Buffer::get_rotating_physical_regs: invalid virtual file type");
      }
   }

  regs.init = true;
  total = gpr_tot + fpr_tot + cr_tot + pr_tot + vir_tot + vfr_tot + vmr_tot;
  return total;
}

void el_Buffer::get_macro_regs()
{
   int num_phys_files = MDES_num_reg_types();
   
   for (int mn = (int)FIRST_MACRO; mn < (int)LAST_MACRO; mn++) {
      Macro_reg mr((Macro_name)mn);
      int index = 0;
      bool found = false;

      for (int i = 0; i < num_phys_files; i++) {
	 Reg_descr* rd = current_MDES->get_MDES_reg_descr(i);
	 if (rd->get_regkind() != REG_PHYSICAL)
	    continue;

	 found = false;
	 Hash_set<eString> macros(hash_estring, 8);
         rd->get_macros(macros);

	 // Get the correct name for the macro
	 eString macro_name = el_macro_to_string_map.value((Macro_name) mn);

	 // See if the macro exists. 
	 if(rd->is_macro(macro_name)) {
	   // add the physical file for this macro
	   char* phys_file_name = rd->get_aname();
	   macro_offset[mn].bind(eString(phys_file_name), index++);	 
	 }

      }
   }
}

// write out all jump tables defined for a given procedure
void el_Buffer::write_all_jump_tables(Procedure* proc)
{
  El_jumptbl*       tbl;
  El_jumptbl_info*  tbl_info;
  List<El_jumptbl*> tbl_list;
  
  tbl_info = proc->get_jumptbl_info();

  // sanity check
  assert(tbl_info->get_num_tables() >= 0);
#if 0
  (*datajmptbls_outbuf) 
    << endl << "/** Jump Table for procedure " << cur_proc << " **/" << endl
    << "extern __PD_OP __PD_tbl" << cur_proc << "[];" << endl
    << endl;
#endif
  // iterate through the list of jump tables and print them out
  tbl_list = tbl_info->get_tbl_list();
  for (List_iterator<El_jumptbl*> li(tbl_list); li != 0; li++) {
    tbl = *li;
    write_jump_table(*tbl);
    (*datajmptbls_outbuf) << endl;
  }
}


// write out a sing jump table
void el_Buffer::write_jump_table(El_jumptbl &tbl)
{
  int   n_entries, align, reserve, addr;
  char* name;

  n_entries = tbl.get_n_entries();  // number of entries in a table
  align     = tbl.get_align();      // alignment information for table elements
  reserve   = tbl.get_reserve();    // total size of table
  name = (char *) tbl.get_name();   // table name

  assert(align == sizeof(int));     // sanity check, validate assumption

  if(!strncmp(name, "_$_", 3)) name += 3;     

  // declare the tables
  (*datajmptbls_outbuf) << "int " << name << "[" << n_entries << "] = {";
    
  // initialize the table - now defined as an array
  static Op* op;
  for (int i = 0; i < n_entries; i++) {
    addr = i * align;

    op = tbl.get_op_target(i);
    (*datajmptbls_outbuf) << jump_table_map.value(op->parent()->id());
#if 0
    (*datajmptbls_outbuf) << "(int) (&__PD_tbl" << cur_proc << "[" 
				  << jump_table_map.value(op->parent()->id()) << "])";
#endif      
    if (i != n_entries-1) (*datajmptbls_outbuf) << ",";
  }
  (*datajmptbls_outbuf) << "};" << endl;
}


// This function carries out the following:
// - If the control block is marked prolog then, a pseudo operation
//   PD_prologue is written. The operation is supposed to build the
//   activation record on entry into the function.
// - Invokes a call to Simu_write_op per non-pseudo operation in the block.
// - Writes out the special PD_aclock operations on detecting change in
//   scheduling times of operations.
void el_Buffer::write_bb(Region* region)
{
  static char buf[MAX_BUF_SIZE];
  int  region_id = region->id();

  stats.reset();
    
  // bind a label to the block id
  if (cbl_count+1 == (int)control_block_label.size()) {
    control_block_label.resize(cbl_count*2); // binary expansion
    // for assembly code generation
    control_block_id.resize(cbl_count*2);
  }

  sprintf(buf, "#define __PD_cb_%s_%d %d\n", cur_proc, region_id, opcount);
  control_block_label[cbl_count++] = buf;
  // for assembly code generation
  control_block_id[cbl_count] = opcount;

  if (jump_table_map.is_bound(region_id)) {
    S_punt("Block %d doubly encountered in current procedure %s!",
	   region_id, cur_proc);
  }
  else {
    jump_table_map.bind(region_id, opcount);
  }

  // reserve the first operation for stats operation
  int tmp_count;
  if (S_performance_monitoring) {
    tmp_count = opcount;
    opcount++;
    resize_emulation_table_if_necessary();
  }
    
  // Insert special pseudo operations PD_PROLOGUE and PD_EPILOGUE
  // to build the stack frame.
    
  // insert the prologue hack operation
  // epilogue is added before RTS
  if (region->flag(EL_REGION_HAS_PROLOGUE_OP)) {
    el_flag = EL_REGION_HAS_PROLOGUE_OP;

    calc_framesize(region);
    write_prologue();
  }

  else if (region->flag(EL_REGION_HAS_EPILOGUE_OP)) {
    el_flag = EL_REGION_HAS_EPILOGUE_OP;
  }

  else {
    el_flag = EL_REGION_LAST_FLAG;
  }

  // RMR number-gathering-tools {
  bool flagme = false;

  if (special_interest_bb(cur_proc, region->id())) {
    flagme = true;
  }
  // } RMR number-gathering-tools

  // traverse through all operations in a control block
  int cycle = 0;
  int max_cycle = 0;
  int latency = 0;
  int max_bundle_latency = 0;

  for (Region_ops_C0_order iter((Compound_region *) region); *iter; iter++) {
    Op* op = (Op *) (*iter);

    if (op->is_pseudo_op()) {
	continue;
    }

    // RMR number-gathering-tools {
    if (special_interest_op(cur_proc, op->id())) {
	flagme = true;
    }
    // } RMR number-gathering-tools

    // if emulating scheduled code add  a clock on every change in the
    // scheduling time; this assumes that any flow dependencies within
    // the block are resolved by the scheduler
    if (!S_unscheduled) {
      int time = op->sched_time();
	
      if (time > cycle) {
	if (max_cycle < (cycle + max_bundle_latency)) {
	  max_cycle = cycle + max_bundle_latency;
	}
	max_bundle_latency = 0;
	
	write_aclock(time - cycle);
	cycle = time;
      }

      latency = write_op(op);

      if (max_bundle_latency < latency) {
	max_bundle_latency = latency;
      }
      assert(max_bundle_latency);
    }

    // if emulating unscheduled code induce the right number of clocks
    // after every operation
    // clock length = max(write latency to destination operands)
    else {
	latency = write_op(op);
	write_aclock(latency);
	cycle += latency;
    }
  }
    
  /* the last instruction bundle in a block may contain
   * other operations with latencies that are greater than the branch latency
   *
   * the clock must tick for the maximum number of cycles
   * according to the latencies of all operations in the bundle
   *
   * also must account for any other operations in the block with long latencies
   * that must be accounted for before leaving this block
   */
  if (!S_unscheduled) {
    /* RMR { add interlocking support */
    extern int S_interlocking;

    if (S_interlocking) {
	Op* last_op = (Op*) ((Compound_region*)region)->last_subregion();
	assert(is_dummy_branch(last_op) || is_branch(last_op));
	if (is_dummy_branch(last_op)) {
	  write_aclock(1);
	  cycle += 1;
	}
	else {
	  assert (latency == MDES_branch_latency(el_opcode_to_string_map.value(last_op->opcode())));
	  write_aclock(latency);
	  cycle += latency;
	}
    }
    else {
	int extra_cycles = max_bundle_latency;

	if (max_cycle > (cycle + max_bundle_latency)) {
	  extra_cycles += max_cycle - (cycle + max_bundle_latency);
	}

	if (extra_cycles) {
	  write_aclock(extra_cycles);
	  cycle += extra_cycles;
	}
    }
  }
    
  // update sched_len field
  // cycle holds the current cycle within a basic block
  stats.sched_len = cycle;
    
  // now write out the basic block entry marker operation
  if (S_performance_monitoring) {
    write_start_compound(region, tmp_count, flagme);
  }
}


void el_Buffer::write_loopbody(Region* region)
{
  Compound_region* cregion = (Compound_region *) region;

  // write out subregions
  Region* subregion_ptr = cregion->first_subregion();
  
  while (subregion_ptr != 0) {
    // write the current region
    write_region(subregion_ptr);
    
    // go to the next region
    subregion_ptr = cregion->successor_in_subregion_list(subregion_ptr);
  }
}


void el_Buffer::write_hb(Region* region)
{
  // A Hyperblock is  identical to a basic block in structure(a collection
  // of operations). However, it is a multiple-exit block, but that is not 
  // relevant here
  write_bb(region);
}


// fake operation marking instruction boundaries ACLOCKS appears as
// PD_aclock in the assembly code where src[0]=1,2,3,etc. represents the
// number of clocks that are provided by this operation
void el_Buffer::write_aclock(int clocks)
{
  static int aclock_count = 1;

  emulation_table[opcount]->op   = PD_ACLOCK_STRING;
  emulation_table[opcount]->type = PSEUDO_OP; 
  
  sprintf(output_buffer, "%d", clocks);
  emulation_table[opcount]->src[0].file = output_buffer;

  sprintf(output_buffer, "%d", aclock_count);
  emulation_table[opcount]->src[1].file = output_buffer;

  aclock_count++;
  opcount++;
  resize_emulation_table_if_necessary();
}


// Write one HPL-PD operation
// The sequence of actions executed by the function are as follows:
// - Static Information of the operation is collected. That is, it is
//   determined if the operation is a branch, or an ialu, or falu, etc
// - If the operation is an RTS a special PD_epilogue operation to destroy
//   the current activation record is written out
// - Writes all the destination operands
// - The output latencies per operand are also written out
// - Writes all the source operands.
// - Writes the Rebel ID.
// - Writes out the operation name.
int el_Buffer::write_op(Region* region)
{    
  Op *op = (Op *) region;
  
  if (op->is_pseudo_op()) 
    return 0;

  // update stats
  collect_op_stats(op);
    
  //construct prefix of operation
  eString opname("__PD_");

  opname.cat(el_opcode_to_string_map.value(op->opcode()));

  // append "_pv" to opcode if op references a PV macro, to distinguish them
  // from ops that operate on standard predicate registers.  This should only
  // appear in PSAVEs, MOVEPGs, and related.  -KF 10/2005
  append_pv_if_necessary(op, opname);
  
  // write the destination operands and their latencies
  int op_latency = write_dest_operands(op);
  // all real ops should have a positive latency
  assert(op_latency > 0);

  // write the source operands
  write_src_operands(op, opname);

  // if this is a BRL operation (function call), then generate a routing
  // vector for the function call
  eString vector_type;
  if ((op->opcode() == BRL) && get_vector_key(op, vector_type)) {
    sprintf(output_buffer, "&__PD_vec_%s", (char*) vector_type);

    emulation_table[opcount]->src[2].file = output_buffer;
    
    // will generate a prototype for the vector in the codegen generated
    // header file
    if (!vectors.is_member(vector_type)) {
      vectors.add_head(prototypes_table.key(vector_type));
    }

    opname.cat("_lit");
  }
  
  // write out the guarded predicate
  int predicated = (int) write_pred_operand(op, opname);

  // true when the operation has a guarding predicate
  emulation_table[opcount]->predicated = predicated;
  
  // see if this operation is speculated
  emulation_table[opcount]->speculative = op->is_speculative();
  emulation_table[opcount]->mask = region->flag(EL_OPER_MASK_PE);

  // write out the Rebel id
  emulation_table[opcount]->id = op->id();
   
  // assign a unique index for this operation, relative to the other operations in the region
  emulation_table[opcount]->index = ++stats.real_op_count;

  // finally write the operation name
  emulation_table[opcount]->op = opname;

  // Epilogue operations
  if (op->opcode() == RTS) {
    write_epilogue();
  }

  // Attach cluster id to the op
  Cluster_id_attr *attr = get_cluster_id_attr(op);
  if (attr != NULL ){
    emulation_table[opcount]->cid = attr->cluster_id;
  } else {
    emulation_table[opcount]->cid = 0;
  }

  // Trace this op?
  if (strcmp(S_trace_function, "*")) {
    if (strstr(S_trace_function, (char*)(el_get_enclosing_procedure(op)->get_name()) ) != NULL)
      emulation_table[opcount]->gen_trace = true;
     else
       emulation_table[opcount]->gen_trace = false;
  } else {
    // if "*" trace all functions
    emulation_table[opcount]->gen_trace = true;
  }

  // increment the index in simulation table
  opcount++;
  resize_emulation_table_if_necessary();

  return op_latency;
}


// writes out destination operands
int el_Buffer::write_dest_operands(Op *op)
{
  int port, i, max_latency = 1;
  bool not_set = true;

  // iterate thru Dest operands
  for (i = 0, port = int(op->first_dest()); port <= int(op->last_dest()); 
       port++, i++) {
    
    Operand& d = (op->dest(Port_num(port)));
    
    // write out the destination operand latency
    // if equals model, latency = latency of operation
    if (S_nual_equals) {
      emulation_table[opcount]->lat[i] = op->flow_time(DEST, port, OUT);

	if (max_latency < emulation_table[opcount]->lat[i]) {
	  max_latency = emulation_table[opcount]->lat[i];
	}
    }
    else {
      // if lte model, latency = 1 (next cycle)
      emulation_table[opcount]->lat[i] = 1;
    }
    not_set = false;

    // now write out the operand
    eString iopat("dest");
    write_operand(d, iopat, i, false, false);
  }

  // brf also sets the program counter; the first unused port will be used
  if (is_brf(op)) {
    emulation_table[opcount]->lat[i] = S_nual_equals ?
      MDES_branch_latency(el_opcode_to_string_map.value(op->opcode())) : 1;
    
    if (max_latency < emulation_table[opcount]->lat[i]) {
	max_latency = emulation_table[opcount]->lat[i];
    }
  }
  // some branch operations (e.g. BRCT) do not have destination operands;
  // will use first available port for the latency
  else if ((is_brl(op) || is_branch(op)) && not_set) {
    emulation_table[opcount]->lat[0] = S_nual_equals ?
      MDES_branch_latency(el_opcode_to_string_map.value(op->opcode())) : 1;

    if (max_latency < emulation_table[opcount]->lat[0]) {
	max_latency = emulation_table[opcount]->lat[0];
    }
  }

  return max_latency;
}


// writes out source operands
void el_Buffer::write_src_operands(Op *op, eString& opname)
{
  int port, i; 

  // if this is a pbrr operation, then any data labels in the
  // source operands should be marked as functions
  static const eString branch_operation_string = "__PD_PBRR";
  bool is_branch_op = false;

  if (opname == branch_operation_string) {
    is_branch_op = true;
  }

  for (i = 0, port = int(op->first_src()); 
       port <= int(op->last_src()); port++, i++) {

    Operand& s = (op->src(Port_num(port)));
    eString iopat("src");
   
    write_operand(s, iopat, i, is_branch_op, is_load(op));
    opname.cat(iopat);
  }
}


// write out guarded predicate operand
bool el_Buffer::write_pred_operand(Op *op, eString& opname)
{
  bool cmpp = false;       // detect comparison instructions for special handle
  bool predicated = false;

  eString iopat("pred");
       
  Operand& operand = (op->src(op->pred()));
  
  // Special case for CMPP where the predicate needs
  // to be written out even when its p<t>
  if (!strncmp((char *)opname, "__PD_CMPP",  9) ||
      !strncmp((char *)opname, "__PD_FCMPP", 10)) {
    // set cmpp flag
    cmpp = true;
    if (operand.is_predicate_true()) {
      emulation_table[opcount]->pred.file = reg_file_to_string(MACRO_FILE);
	emulation_table[opcount]->pred.num  = macro_to_pd(PRED_TRUE);
      iopat = null_string;
    }
  }
    
  write_operand(operand, iopat, 0, false, false);

  // the _pred version of CMPP operations is invalid
  if (cmpp) iopat = null_string;
    
  // iopat is set to "" if the guarded predicate was a p<t>
  // dont append _pred to function name if guarded predicate is p<t>
  if (iopat != null_string) {
    iopat = "_pred";
    predicated = true;
  }

  opname.cat(iopat);
  return predicated;
}


// append "_pv" to opcode if op references a PV macro, to distinguish them
// from ops that operate on standard predicate registers.  This should only
// appear in PSAVEs, MOVEPGs, and related.  -KF 10/2005
void el_Buffer::append_pv_if_necessary(Op *op, eString& opname)
{
  for (Op_explicit_sources srci(op); srci != 0; srci++) {
    Operand& opnd = *srci;
    if (opnd.is_macro_reg()) {
      Macro_name mname = opnd.name();
      if (current_MDES->is_pv_macro(mname)) {
        opname.cat("_pv");
        return;
      }
    }
  }
  for (Op_explicit_dests desti(op); desti != 0; desti++) {
    Operand& opnd = *desti;
    if (opnd.is_macro_reg()) {
      Macro_name mname = opnd.name();
      if (current_MDES->is_pv_macro(mname)) {
        opname.cat("_pv");
        return;
      }
    }
  }
}


// update static statistics
void el_Buffer::collect_op_stats(Op *op)
{
  Optype op_type = PSEUDO_OP;

  if (is_move(op) && op->flag(EL_OPER_ICMOVE)) {
    op_type = ICM_OP;
    ++stats.Static_icm;
  }
  
  /* RMR { vector (SIMD) ops; handles these first
   * since there might be some overlap with scalar ops
   */
  else if (is_vector_ialu(op)) {
    op_type = IA_OP;
    ++stats.Static_vialu;
  }

  else if (is_vector_falu(op)) {
    op_type = VFA_OP;
    ++stats.Static_vfalu;
  }
  
  else if (is_vector_load(op)) {
    op_type = VLD_OP;
    ++stats.Static_vload;
  }
  
  else if (is_vector_store(op)) {
    op_type = VST_OP;
    ++stats.Static_vstore;
  }
  
  else if (is_svmove(op) || is_vsmove(op)) {
    op_type = VXF_OP;
    ++stats.Static_vxfr;
  }

  else if (is_vector_perm(op)) {
    op_type = VPR_OP;
    ++stats.Static_vperm;
  }
  
  else if (is_vector(op)) {
    // all other vector ops; what did we miss?
    S_punt("Did not recognize operation type in function <%s> operation <%d>", 
	     cur_proc, op->id());
  }
  /* } RMR */

  else if (is_ialu(op)) {
    op_type = IA_OP;
    ++stats.Static_ialu;
  }

  else if (is_predicate_clear(op) || is_predicate_set(op) || is_clear_all_rotating(op)) {
    op_type = IA_OP;
    ++stats.Static_ialu;
  }

  else if (is_falu(op)) {
    op_type = FA_OP;
    ++stats.Static_falu;
  }
  
  else if (is_load(op)) {
    op_type = LD_OP;
    ++stats.Static_load;
  }
  
  else if (is_store(op)) {
    op_type = ST_OP;
    ++stats.Static_store;
  }

  else if (is_cmpp(op)) {
    op_type = CP_OP;
    ++stats.Static_cmpp;
  }

  else if (is_pbr(op)) {
    op_type = PB_OP;
    ++stats.Static_pbr;
  }

  else if (is_brl(op) || is_branch(op)) {
    op_type = BR_OP;
    ++stats.Static_branch;
  }
 
  else {
    S_punt("Did not recognize operation type in function <%s> operation <%d>", 
            cur_proc, op->id());
  }

  assert(!op->is_pseudo_op());
  ++stats.Static_ops;
  emulation_table[opcount]->type = op_type;

  if (op->flag(EL_OPER_SPILL_CODE)) {
    /* RMR { spill code in the prologue and epilogue are done 
     * at function and entry and exit, hence they are callee saves
     */
    if ((el_flag == EL_REGION_HAS_PROLOGUE_OP) || 
	  (el_flag == EL_REGION_HAS_EPILOGUE_OP)) {
      emulation_table[opcount]->flags = CLRSAVE;
      ++stats.Static_callee_spill;
    }
    /* all other spill code is caller save */
    else {
      emulation_table[opcount]->flags = CLESAVE;
      ++stats.Static_caller_spill;
    }
    /* } RMR */
  }
}


// Writes an operand - called from write_op
// It handles the operands as follows:
// - If register, use the assembler notation and write it out,
//   e.g. __PD_REGISTER_FILE_[register number]
// - Macro registers are written directly in their string form,
//   - m<sp> would appear as __PD_REGISTER_FILE[__PD_SP]
//   - "Black holes" (scratch data holders) are attached if bit buckets appear
//     in destination ports
// - Integer Literal written out in the form  PD_SET_LIT(tbl[0].src[0], IVAL)
//   - IVAL is the integer value
// - Label is written out in the form PD_SET_LIT(tbl[0].src[0], &lab)
//   - lab is the label
//   - Static variable references are changed back to their original names
// - String is written out in the form PD_SET_LIT(tbl[0].src[0], "str")
// - Control Block is written out in the form 
//   PD_SET_LIT(tbl[0].src[0], &tbl[cb_8])
//   - cb_8 is an index to the start of control block 8
// - Predicate Literal is ignored
// - Float and Double Literals are currently UNIMPLEMENTED
// - Undefined - A black hole is written out
void el_Buffer::write_operand(Operand& oprnd, eString &iopat, int oprnd_num,
					bool is_branch_op, bool is_load_op)
{  
  eString port_type = iopat;
  static Port* op_entry;

  // what type of operand
  if (oprnd.is_reg()) { // Regular register
    Reg_file reg_file = oprnd.file_type();
    if (port_type == "dest") {
      op_entry = &(emulation_table[opcount]->dest[oprnd_num]);
    }
    else if (port_type == "src") {
      op_entry = &(emulation_table[opcount]->src[oprnd_num]);
    }
    else {
      assert (port_type == "pred");
      op_entry = &(emulation_table[opcount]->pred);
    }
    op_entry->file = reg_file_to_string(oprnd.file_type());

    if (oprnd.is_rotating()) {
      if (!regfile.init) {
	get_rotating_physical_regs(regfile);
      }
      op_entry->file.cat(ROTATING_REGISTER_FILE_STRING);
      op_entry->offset = rot_phys_offset.value(oprnd.physical_file_type());
      op_entry->rot = MDES_reg_rotating_size(oprnd.physical_file_type());

      // for vectors.  is this right?  -KF 2/2007
      //if (reg_file == VIR || reg_file == VFR || reg_file == VMR)
      if (reg_file == VIR || reg_file == VFR)
        op_entry->rot *= MDES_reg_vector_length(oprnd.physical_file_type());

      // a zero-sized rotating register file is not expected
      assert (op_entry->rot);
    }

    // If virtual register emulation turned on
    // Note : Currently the prepass scheduler of elcor does 
    // rotating register allocation, so we choose always take
    // physical regs for rotating regs
    if (oprnd.is_static() && S_virtual_regs) {
      int n = oprnd.vr_num();
      if (reg_file == VIR || reg_file == VFR)
        // scale the register number by vector length.  -KF 2/2007
        op_entry->num = n * vector_length;
      else
        op_entry->num = n;
	    
      // goal is to figure out width of reg files
      // set the file size to the max reg num encountered untill now
      switch (reg_file) {
      case GPR:
	  if (regfile.gpr_stat_size < n) regfile.gpr_stat_size = n;
	  break;
      case FPR:
	  if (regfile.fpr_stat_size < n) regfile.fpr_stat_size = n;
	  break;
      case BTR:
	  if (regfile.btr_stat_size < n) regfile.btr_stat_size = n;
	  break;
      case PR:
	  if (regfile.pr_stat_size < n) regfile.pr_stat_size = n;
	  break;
      case CR:
	  if (regfile.cr_stat_size < n) regfile.cr_stat_size = n;
	  break;
	/* RMR { support vector file type */
      case VIR:
	  if (regfile.vir_stat_size < n * vector_length)
            regfile.vir_stat_size = n * vector_length;
	  break;
      case VFR:
	  if (regfile.vfr_stat_size < n * vector_length)
            regfile.vfr_stat_size = n * vector_length;
	  break;
      case VMR:
// 	  if (regfile.vmr_stat_size < n * vector_length)
//             regfile.vmr_stat_size = n * vector_length;
          if (regfile.vmr_stat_size < n) regfile.vmr_stat_size = n;
	  break;
      /* } RMR */
      default:
	  break;
      }
    }

    // just write out the physical register
    else {
      if (oprnd.is_static()) {
	if (oprnd.allocated()) {
	  int offset = stat_phys_offset.value(oprnd.physical_file_type());
          if (reg_file == VIR || reg_file == VFR)
            // scale the register number by vector length.  -KF 2/2007
            op_entry->num = offset + oprnd.mc_num() * vector_length;
          else
            op_entry->num = offset + oprnd.mc_num();
	} else {
	  cerr << "Bad operand: " << oprnd << endl;
          S_punt ("Unallocated static register encountered!\n");
        }
      } else { //rotating
	if (oprnd.allocated()) {
          if (reg_file == VIR || reg_file == VFR)
            // scale the register number by vector length.  -KF 2/2007
            op_entry->num = oprnd.mc_num() * vector_length;
          else
            op_entry->num = oprnd.mc_num();
	} else {
	  S_punt("rotating registers must be allocated!\n");
	}
      }
    }

    op_entry->is_reg = true;
    /* RMR { support vector operation; if this is a vector register, write out the vector length */
    //if ((reg_file == VIR) || (reg_file == VFR) || (reg_file == VMR)) {
    if ((reg_file == VIR) || (reg_file == VFR)) {
	op_entry->vec = clog2(vector_length);
    }
    // We now do the multiply in codegen rather than emulib (to work with
    // multicluster).  However, still populate the vec field so simu trace knows
    // it is a vector op.  -KF 2/2007
    /* } RMR */
    iopat = "_reg";
  }

  else if (oprnd.is_macro_reg()) { // macro register
    Macro_name mname = oprnd.name();

    switch (mname) {       

      // for PV macros, use the op_entry->offset field to store offset of the
      // corresponding predicate file.  -KF 10/2005
      case PV_0:  case PV_1:  case PV_2:  case PV_3:
      case PV_4:  case PV_5:  case PV_6:  case PV_7:

        if (port_type == "src")
          op_entry = &(emulation_table[opcount]->src[oprnd_num]);
        else if (port_type == "dest")
          op_entry = &(emulation_table[opcount]->dest[oprnd_num]);
        else
          S_punt("PV operand must be src or dest");

        op_entry->file = reg_file_to_string(MACRO_FILE);
        op_entry->num = macro_to_pd(mname);
        op_entry->is_reg = true;
        if (!S_virtual_regs && oprnd.assigned_to_physical_file())
          op_entry->offset = stat_phys_offset.value(oprnd.physical_file_type());

        break;

    /* the macros listed here are simulated in physical files, macros not
     * listed are simulated in separate macro files. */
    case INT_PARAM_1: case INT_PARAM_2:case INT_PARAM_3:case INT_PARAM_4:
    case FLT_PARAM_1: case FLT_PARAM_2: case FLT_PARAM_3: case FLT_PARAM_4:
    case DBL_PARAM_1: case DBL_PARAM_2: case DBL_PARAM_3: case DBL_PARAM_4:
    case INT_RETURN: case FLT_RETURN: case DBL_RETURN:
    case LC_REG: case ESC_REG: 
    case RETURN_ADDR:
    case ICMOVE_REG:
    case VEC_SCRATCH_PAD:
      /*hongtaoz: map macros to physical files*/
      if (!S_virtual_regs && oprnd.allocated() ) {
	if (port_type == "dest") {
	  op_entry = &(emulation_table[opcount]->dest[oprnd_num]);
	}
	else if (port_type == "src") {
	  op_entry = &(emulation_table[opcount]->src[oprnd_num]);
	}
	else {
	  assert (port_type == "pred");
	  op_entry = &(emulation_table[opcount]->pred);
	}
	op_entry->file = reg_file_to_string(oprnd.file_type());
	int offset = stat_phys_offset.value(oprnd.physical_file_type());
	op_entry->num = offset + oprnd.mc_num();
	op_entry->is_reg = true;
	break;
      } 

      // fall thru on other port_types

      default:
 
        // The macros below (INT_ZERO, etc.) were previously simulated in physical
        // files.  This seemed wrong so I moved them here, where they will be
        // simulated in macro files.  -KF 1/2007

        // check for writes to bit buckets
        if (mname == INT_ZERO ||
            mname == FLT_ZERO || mname == FLT_ONE ||
            mname == DBL_ZERO || mname == DBL_ONE ||
            mname == PRED_FALSE || mname == PRED_TRUE ||
            mname == VEC_MASK_TRUE) {
          // All writes to bit buckets at the destination ports
          // substituted by black holes
          if (port_type == "dest") {
            S_warn("Macro register encountered as destination operand in %s", cur_proc); 
            assert(0);
          }
        }

      /*TODO: since each macro is an entry in the macro_file instead of a 
       * a separate file as it use to be, we need to figure out how to 
       * simulate multi-file virtual macros. */
	// macros assigned to hard-wired variables
	if (port_type == "pred") {
	  emulation_table[opcount]->pred.file   = reg_file_to_string(MACRO_FILE);
	  emulation_table[opcount]->pred.num    = macro_to_pd(mname);
	  emulation_table[opcount]->pred.is_reg = true;
	}
	else if (port_type == "src") {
	  emulation_table[opcount]->src[oprnd_num].file   = reg_file_to_string(MACRO_FILE);
	  emulation_table[opcount]->src[oprnd_num].num    = macro_to_pd(mname);
	  emulation_table[opcount]->src[oprnd_num].is_reg = true;
	}
	else {
	  assert (port_type == "dest"); 
	  emulation_table[opcount]->dest[oprnd_num].file   = reg_file_to_string(MACRO_FILE);
	  emulation_table[opcount]->dest[oprnd_num].num    = macro_to_pd(mname);
	  emulation_table[opcount]->dest[oprnd_num].is_reg = true;
	}
    }

    iopat = "_reg";
  }

  else if (oprnd.is_int()) { // integer literal
    assert (port_type == "src");

    sprintf(output_buffer, "%d", oprnd.int_value());
    emulation_table[opcount]->src[oprnd_num].file = output_buffer;

    iopat = "_lit";
  }

  else if (oprnd.is_label()) { // label literal
    assert (port_type == "src");

    bool  is_function   = false;
    bool  is_data_label = false;
    char* label = (char *) oprnd.label_value();

    emulation_table[opcount]->src[oprnd_num].file = ADDRESS_OF_LABEL_STRING;

    if (!strncmp(label, "_$_",   3)) {
      label += 3;     
    }
    else if (!strncmp(label, "_$fn",  4)) {
      is_function = true;
      label += 4;
    }
    else if (!strncmp(label, "_$$$_", 5)) {
      label += 5;
    }
    else if (!strncmp(label, "f_$$$_", 6)) {
      label += 6;
    }
    else is_data_label = true;

    if (*label == '_') ++label;

    /* WWF Nov 19 2003 { wrap around standard heap management and use custom allocators */
    if (S_use_custom_malloc) {
	if ((!strcmp(label, "malloc")) ||
	    (!strcmp(label, "calloc")) ||
	    (!strcmp(label, "realloc")) ||
	    (!strcmp(label, "free")) ||
	    (!strcmp(label, "cfree"))) {
	  char tmp[256];
	  
	  sprintf(tmp, "__PD_%s", label);
          /*hongtaoz: CANNOT do strcpy here! That will modify eString::string_table and possibly overwrite some other global variables*/
          //strcpy(label, tmp);
          label = tmp;
	}
    }
    /* } WWF Nov 19 2003 */

    /* hongtaoz: Mar 18 2005, wrap around alloca and __builtin_alloca*/
    if ( (!strcmp(label, "alloca")) ||
	(!strcmp(label, "__builtin_alloca")) ) {
       label = "__PD_alloca";
    }

    // HP-GCC vararg solution: use a wrapper for builtin functions
    eString name(label); 
    if ((name == "setjmp") || (name == "_setjmp") || (name == "__sigsetjmp")) {
      // ignore 'sigsetjmp' which is used on linux platforms
      label         = (char*) SETJMP_STRING;
      is_function   = false;
      is_data_label = false;
      emulation_table[opcount]->src[oprnd_num].file.cat(PD_WORKAROUND);
    }
    else if ((name == "longjmp") || (name == "_longjmp") || (name == "__siglongjmp")) {
      // ignore 'siglongjmp' which is used on linux platforms
      label         = (char*) LONGJMP_STRING;
      is_function   = false;
      is_data_label = false;
      emulation_table[opcount]->src[oprnd_num].file.cat(PD_WORKAROUND);
    }

    emulation_table[opcount]->src[oprnd_num].file.cat(label);

    // if this is a function, then add it to the list for extern declaration
    if (is_function || is_branch_op) {
	if (!extern_functions.is_member(name))
	  extern_functions.add_head(name);
    }
    // if this is a data label, then add it to the list for extern data
    // declaration
    else if (is_data_label) {
      if (!extern_data_labels.is_member(name))
	  extern_data_labels.add_head(name);
    }

    iopat = "_lit";
  }

  else if (oprnd.is_string()) { // literal is a string(char *)
    assert (port_type == "src");

    sprintf(output_buffer, "%s", (char*) oprnd.string_value());
    emulation_table[opcount]->src[oprnd_num].file = output_buffer;

    iopat = "_lit";
  }

  else if (oprnd.is_cb()) {  // literal is a control block label
    assert (port_type == "src");
    
    // Write out the control block label
    sprintf(output_buffer, "__PD_cb_%s_%d", cur_proc, oprnd.id());
    emulation_table[opcount]->src[oprnd_num].file = output_buffer;

    iopat = "_lit";
  }
  
  else if (oprnd.is_predicate_true()) { // literal is p<t> from Rebel
    // set iopat to "" indicating it was a p<t> used by 
    // write_pred_operand method
    //iopat = null_string;

     // predicate literals can be used as source operands. use _lit
     // version of the opcode, just like for other literals.
     // note - this required a change to write_pred_operand to not assume
     // null string for predicate true. -KF 8-2001
     if (port_type == "src") {
       sprintf(output_buffer, "%d", 1);
       emulation_table[opcount]->src[oprnd_num].file = output_buffer;

       iopat = "_lit";
     } else { //for predicate
       iopat = null_string;
     }
  }
  
  else if (oprnd.is_predicate_false()) { // literal is p<f> from Rebel
    // No reason why one would have a pred_false as a guarded predicate
    //
    // Changed mchu (5.20.04)  we are now inserting pred_falses to
    // handle AND-type predicates
    //    assert (0);

    assert (port_type == "src");
    
    sprintf(output_buffer, "%d", 0);
    emulation_table[opcount]->src[oprnd_num].file = output_buffer;
    
    iopat = "_lit"; 
  }
  
  else if (oprnd.is_float()) { // literal is a float 
    // quite unlikely that it will appear in a risc processor
    S_punt("float literal unimplemented - encounterd in procedure %s",
	     cur_proc);
  }
  
  else if (oprnd.is_double()) { // literal is a double
    // quite unlikely that it will appear in a risc processor
    S_punt("double literal unimplemented - encounterd in procedure %s",
	     cur_proc);
  }
  
  else if (oprnd.is_undefined()) {
    // write out a dummy place holder (black hole!) as an undefined reg
    // All undefined destination and source ports are substituted by black holes
    // Definition of a black hole : Simulator term for a bit bucket which
    // is actually not part of the PD register set. 

    //  RMR {  normal load  operations  will now  have an  undefined
    // second source operand; just ignore it
    if (is_load_op) iopat = ""; else
    // } RMR
    if (port_type == "src") {
      emulation_table[opcount]->src[oprnd_num].file = reg_file_to_string(MACRO_FILE);
      emulation_table[opcount]->src[oprnd_num].num  = macro_to_pd(UNDEFINED);
      iopat = "_reg";
    }
    
    else if (port_type == "dest") {
      emulation_table[opcount]->dest[oprnd_num].file = reg_file_to_string(MACRO_FILE);
      emulation_table[opcount]->dest[oprnd_num].num  = macro_to_pd(UNDEFINED);
    }
    
    else iopat = null_string; // ignore undefined predicate ports
  }
	
  else {
    S_punt("unknown operand type encountered in procedure %s", cur_proc);
  }
}


// Write an operation that builds the Activation record / stack frame
void el_Buffer::write_prologue()
{
  //hongtaoz: new stack model
  int stackoffset;
  Macro_name stackvar; // not made use of currently. gets SP back.
  //HACK
  int topcount = 0; // otherwise = opcount
	
  // src[0] holds the total stack frame size
  emulation_table[topcount]->op   = PD_PROLOGUE_STRING;
  emulation_table[topcount]->type = PSEUDO_OP;

  sprintf(output_buffer, "%d", frame.size);
  emulation_table[topcount]->src[0].file = output_buffer;
  
  // get the offset to be added to OP on function entry
  map_macros_to_stack(OP_REG, stackvar, stackoffset);
  sprintf(output_buffer, "%d", stackoffset);
  emulation_table[topcount]->src[1].file = output_buffer;

  // get the offset to be added to RGS on function entry
  map_macros_to_stack(RGS_REG, stackvar, stackoffset);
  sprintf(output_buffer, "%d", stackoffset);
  emulation_table[topcount]->src[2].file = output_buffer;
  
  // get the offset to be added to LV on function entry
  map_macros_to_stack(LV_REG, stackvar, stackoffset);
  sprintf(output_buffer, "%d", stackoffset);
  emulation_table[topcount]->src[3].file = output_buffer;
  
  // get the offset to be added to IP on function entry
  map_macros_to_stack(IP_REG, stackvar, stackoffset);
  sprintf(output_buffer, "%d", stackoffset);
  emulation_table[topcount]->src[4].file = output_buffer;

  sprintf(output_buffer, "\"%s\"", cur_proc);
  emulation_table[topcount]->dest[0].file = output_buffer;
}


// Write out the pseudo operation to destroy an activation record
void el_Buffer::write_epilogue()
{
  // src[0] holds the stack frame size

  sprintf(output_buffer, "%d", frame.size);

  emulation_table[opcount]->src[1].file = output_buffer;
}


// Utility to double word align an offset
// Assures there are no bus errors at run-time due to incorrect address
// alignment - example reading a dword from a non dword aligned addr.
void el_Buffer::dword_align(int& mval)
{
  if ((mval % sizeof(double)) > 0)
    mval += (sizeof(double) - (mval % sizeof(double))); 
}


// Computes the offsets within the stackframe required by the
// PD_prolog and PD_epilog operations
void el_Buffer::map_macros_to_stack(Macro_name mname, Macro_name &stackvar,
						int &stackoffset)
{   
  // hongtaoz; new stack model
/*
          Stack model 
       
        ------------------------
             incoming parameters     previous stack frame                    
IP   -> ========================   <--- initial SP before entry into function
             local variables
LVS  -> ------------------------   new stack frame
             spill code
RGS  -> ------------------------  
             output parameters
OP   -> ========================   <--- new SP after call to prologue 


IP,LV,RGS,OP are set in __PD_prologue. They are set relative to the new SP
Spill code, input and output parameters are accessed with a positive
offset when growing form high to low and negative offset for low to high
w.r.t RGS, IP and OP respectively. 
*/

  // offsets are relative to SP of the new frame
  stackvar = SP_REG;
  switch(mname) {
  case OP_REG: 
    stackoffset = 0;
    break;

  case RGS_REG:
    stackoffset = frame.param;
    break;

  case LV_REG: 
    // IMPACTFIX: uncomment below after fixing impact
    // stackoffset = frame.param + frame.swap; 
    stackoffset = frame.param + frame.swap + frame.local;
    break;

  case IP_REG: 
    stackoffset = 
      (frame.param + frame.swap + frame.local);
    break;

  default:
    break;
  }
}


// search for a particular macro register in a DEFINE operation
// if found, return its value, used to calculate the frame size
void el_Buffer::check_component(Op *op, Macro_name mname, char *mstring, 
					  int &mval) 
{
  Port_num dport = int(op->first_dest());
  Port_num sport = int(op->first_src());
   
  Operand doprnd = op->dest(dport);
  Operand soprnd = op->src(sport);
  
  if (doprnd.is_macro_reg()) {
    if (doprnd.name() == mname) {
      if (soprnd.is_int()) {
	  mval = soprnd.int_value();
	  dword_align(mval); // dword align it!
      }
      
      else {
	  S_warn("%s size undefined.", mstring);
      }
    }
  }
}


// Reads the prologue and returns the framesize
void el_Buffer::calc_framesize(Region *region)
{   
  frame.local = -1;
  frame.param = -1;
  frame.swap  = -1;
  frame.fma   = 16;
   
  // Traverse through all operations in the prologue basic block
  // and search for DEFINE operations
  for (Region_ops_C0_order iter((Compound_region *) region); *iter; iter++) {
    Region* R = (Region *) *iter;
    Op* op    = (Op *) R;
   
    // pick up only the "DEFINE" pseudo operations
    if (op->is_pseudo_op() &&  op->opcode() == DEFINE) {
      // get the sizes of the diff. components in a stack frame
      check_component(op, LOCAL, "m<local>", frame.local);
      check_component(op, PARAM, "m<param>", frame.param);
      check_component(op, SWAP,  "m<swap>",  frame.swap);
    }
  }
   
  if (frame.local == -1) {
    frame.local = 0;
  }
  
  if (frame.param == -1) {
    // HPL-PD ABI says this should be atleast 16 bytes, 
    // *aka* fixed arg. area
    S_warn("m<param> not defined, assuming 16.");
    frame.param = 16;  
  }
   
  if (frame.swap == -1) {
    S_warn("m<swap> not defined, assuming 0.");
    frame.swap = 0;
  }
  
  /* RMR { adjust local stack pointer to be aligned at 16-byte boundaries */
  if (frame.local & 15) {
    frame.local += 16 - (frame.local & 15);
  }

  if (frame.param & 15) {
    frame.param += 16 - (frame.param & 15);
  }

  if (frame.swap & 15) {
    frame.swap += 16 - (frame.swap & 15);
  }

  assert(((frame.local + frame.param + frame.swap + frame.fma) % 16) == 0);
  /* } RMR */

  // compute total size
  //frame.size = frame.local + frame.param + frame.swap + frame.fma;
  //hongtaoz: new stack model
  frame.size = frame.param + frame.local + frame.swap;
}


// binds a call_info string to a vector (called a template)
// if the call_info string was not encountered in the past, a new
// vector is generated and stored for future reference
int el_Buffer::get_vector_key(Op* op, eString& vector_type)
{
  eString call_info;
  static  hc_aggregate* entry;

  if (!get_call_info(op, call_info)) {
    S_punt("expected call_info attribute on op %d in procedure %s!",
	     op->id(), cur_proc);
  }

  int key = assign_key(call_info);
       
  vector_type  = "template_";
  vector_type.cat(int_to_str(key));
  vector_type |= '_';

  eString fname = vector_type;

  if (!prototypes_table.is_bound(fname)) {
    entry = new hc_aggregate;
    create_prototype(*entry, fname, call_info);
    append_parameter_attributes(op, *entry);
    
    prototypes_table.bind(entry->name, entry);
    
    entry->vec_temp = vector_type.length();
  }

  return true;
}


// given an operation, return the call_info attribute
int el_Buffer::get_call_info(Region *reg, eString& info)
{
  Lcode_attribute_map* map = get_lcode_attributes(reg);
  
  if (map && !map->is_empty()) {
    // iterate over the attributes, until call_info is encoutered
    for (Map_iterator<eString, List<Operand> > mptr(*map); mptr != 0; mptr++) {
      // recover operand list
      Pair<eString, List<Operand> > entry = *mptr;
      
      // only interested in call_info attribute
      if (entry.first == "call_info") {
	  // iterate through and recover parameter string
	  for (List_iterator<Operand> optr(entry.second);  optr != 0;  optr++) {
	    Base_operand& op = *(*optr).get_ptr();
	  
	    if(op.is_string()) {
		info = ((const String_lit&)op).value();
		
		// check to see if the current call_info field has been processed
		// for external structures or unions extraction, if it hasn't
		// process it and mark is as processed
		if (!encountered_call_info.is_member(info)) {
		  encountered_call_info.add_head(info);
		  extract_struct_or_union_info(info);
		}

		return true;
	    }
	  }
      }
    }
    
    S_warn("call_info attribute not found for operation %d of procedure %s",
	     reg->id(), cur_proc);
  }
  
  return false;
}


// append offset information to the parameter list of a given function
// or function call
int el_Buffer::append_parameter_attributes(Region *reg, hc_aggregate& func)
{
  bool tr_encountered   = false;
  bool tmo_encountered  = false;
  bool tmso_encountered = false;
  bool append_gcc_ellipsis = false;
  bool kandrc = false;

  int  current_parameter_number = 1;


  Lcode_attribute_map *map = get_lcode_attributes(reg);
  List<Operand> tmso_list;

  if (map && !map->is_empty()) {
    // iterate over the attributes, until the tr && tmo is encoutered
    for (Map_iterator<eString, List<Operand> > mptr(*map); mptr != 0; mptr++) {
      // recover operand list
      Pair<eString, List<Operand> > entry = *mptr;
      
      // this is the list of "through-register" parameters,
      // attribute the information to the entry
      // offset information is ignored, since is should be done properly
      // in IMPACT and ELCOR.  
      // May wish to use that information for error checking!
      if (entry.first == "tr") {
	  // so that the function may return when finished
	  tr_encountered = true;

	  // iterate through and recover parameter string
	  for (List_iterator<Operand> optr(entry.second);  optr != 0;  optr++) {
	    Base_operand& op = *(*optr).get_ptr();
	  
	    if(!op.is_macro_reg()) {
		S_warn("%s: expected macro register for current operation %d!",
			 "append_parameter_attributes", reg->id());
		assert(0);
	    }

	    Macro_name macro = ((const Macro_reg&) op).name();

	    if (current_parameter_number == (int)func.params.size()) {
	      S_warn("%s: extra register parameters at operation %d of %s",
		  "append_parameter_attributes", reg->id(), cur_proc);
	    }

	    else if ((macro == RETURN_ADDR) | (macro == INT_RETURN) | 
		  (macro == FLT_RETURN)  | (macro == DBL_RETURN)) {
		S_warn("%s: %s used as a parameter at operation %d of %s *%s*",
			 "append_parameter_attributes", "<return marco register>",
			 reg->id(), cur_proc, "ignoring parameter");
	    }
	    
	    // if a function returns a struct, impact adds the
	    // INT_RETURN macro to the "tr" attribute. i think this
	    // should be ignored here. -KF 1-2004
	    else if (macro != INT_RETURN) {
	      func.params[current_parameter_number].macro_reg  = true;
	      func.params[current_parameter_number].macro_name = macro;
              if(func.params[current_parameter_number].desc != DATA_POINTER) {
                if(func.params[current_parameter_number].type == LONGLONG) {
                  optr++;
                  Base_operand& temp_op = *(*optr).get_ptr();
                  Macro_name temp_macro = ((const Macro_reg&) temp_op).name();
                  func.params[current_parameter_number].second_macro_name = temp_macro;
                }
              }
              current_parameter_number++;
	    }
	  }
      }
      
      // These are parameters passed on the stack, the simulator
      // will use OP pointer + offset to access these parameters
      else if (entry.first == "tmo") {
	  // so that the function may return when finished
	  tmo_encountered = true;

	  // iterate through and recover parameter string
	  for (List_iterator<Operand> optr(entry.second);  optr != 0;  optr++) {
	    Base_operand& op = *(*optr).get_ptr();
	  
	    if(!op.is_int()) {
		S_punt("%s: expected integer value for op offset at operation %d!",
			 "append_parameter_attributes", reg->id());
	    }

	    int offset = ((const Int_lit&) op).int_value();
	    func.params[current_parameter_number].offset = offset;
	    current_parameter_number++;
	  }
      }
      
      else if (entry.first == "host_info") {
	  for (List_iterator<Operand> optr(entry.second);  optr != 0;  optr++) {
	    Base_operand& op = *(*optr).get_ptr();
	
	    assert (op.is_string());
	    eString platform = ((const String_lit&) op).value();
	  
	    if (platform == "\"hp\"") {
		host_platform_info.platform = HP;
	    }
	    else if (platform == "\"x86lin\"") {
		host_platform_info.platform = X86LIN;
	    }
            else if (platform == "\"x86win\"") {
                host_platform_info.platform = X86WIN;
            }
	    else if (platform == "\"sunos\"") {
		host_platform_info.platform = SUNOS;
	    }
	    else if (platform == "\"sunsol\"") {
		host_platform_info.platform = SUNOS;
	    }
	    else S_punt("unsupported <%s> host platform", (char*) platform);
	    break;
	  }
      }

      else if (entry.first == "tmso") {
	  tmso_encountered = true;
	  tmso_list = entry.second;
      }

      else if (entry.first == "append_gcc_ellipsis") {
	  append_gcc_ellipsis = true;
	  func.isVararg = true;
      }

      else if (entry.first == "VARARG") {
	  func.isVararg = true;
      }

      else if (entry.first == "preprocess_info") {
        for (List_iterator<Operand> optr(entry.second);  optr != 0;  optr++) {
          Base_operand& op = *(*optr).get_ptr();

	    // sometimes the "old_style_param" flag is missing, 
	    // the "K&R-C" flag should be more reliable
          if (op.is_string()) {
		eString info = ((const String_lit&) op).value();
	    
		if (info == "\"K&R-C\"") {
		  kandrc = true;
		  func.isKANDRC = true;
		}
	    }
	  }
      }

      else if (entry.first == "old_style_param") {
	  kandrc = true;
	  func.isKANDRC = true;
      }
    }
  }
  
  // add the structure location on the stack to the corresponding 
  // parameter entry
  if (tmso_encountered) {
    // expecting the tmso to be for a parameter, not the return type
    assert (func.params.size() > 1);
    
    // the first node in the list is the return type
    int last_struct_param = 1; 
    
    // iterate through the list and retrieve the memory offsets for 
    // any structure that are encountered
    for (List_iterator<Operand> optr(tmso_list); optr != 0; optr++) {
      Base_operand& op = *(*optr).get_ptr();

      // if the parameter is not a structure, I don't expect to see
      // a through-memory-structure-offset value
      if ((func.params[last_struct_param].type != STRUCT) &&
	    (func.params[last_struct_param].type != UNION)) {
	  assert (op.is_undefined());
      }
      
      // if the parameter is a structure, then I only expect to see
      // a through-memory-structure-offset value if it passed by value
      else if (func.params[last_struct_param].desc != NIL){
	  assert (op.is_undefined());
      }
      
      else {
	  if(!op.is_int()) {
	    S_warn("%s: expected integer value for %s offset at operation %d!",
		     "append_parameter_attributes", "struct", reg->id());
	    assert(0);
	  }
	
	  else if (last_struct_param > (int)func.params.size()-1) {
	    S_punt("%s: corresponding struct parameter for \"tmso\" of %s %d %s",
		     "append_parameter_attributes", "operation", reg->id(), 
		     "not found");
	  }
	
	  int soffset = ((const Int_lit&) op).int_value();
	  func.params[last_struct_param].has_soffset = true;
	  func.params[last_struct_param].soffset = soffset;
      }
      
      last_struct_param++;
    }
  }

  // sanity check
  if (append_gcc_ellipsis) {
    if (!kandrc) {
      S_punt("expected \"%s\" %s for operation %d of %s\n%s %s",
		 "old_style_param", "lcode attribute", reg->id(), cur_proc, 
		 "You may wish to manually add the attribute to",
		 (char*) Rebel_path_name);
    }
  }
  
  return true;
}


// return a unique key for a given call_info strings
int el_Buffer::assign_key(eString& call_info)
{
  static int key = 1;
  
  // if the call_info field was encountered before, then return that key
  if (call_info_map.is_bound(call_info))
    return call_info_map.value(call_info);

  // otherwise create a new key
  call_info_map.bind(call_info, key);
  return key++;
}


// given a call_info string, generate a function attribute
// this function is no longer necessary, and will be removed eventually
// its functionality has been replaced by the impact attributes
void el_Buffer::create_prototype(hc_aggregate& entry, eString& fname, 
					   eString& call_info)
{
  // if call_info was encountered already, make a copy of the generated
  // function attribute
  if (stored_call_info.is_bound(call_info)) {
    entry = *stored_call_info.value(call_info);
  }

  else { // create a new attribute
    static hc_aggregate* info_entry;

    // call_info string format:
    // parameters are seperated by '%'
    // first field is the return type
    // arrays and pointers are preceded by '+'
    // pointers == 'P', arrays == 'A' (and size)

    info_entry = new hc_aggregate;
    // save call_info, will be used in later processing (packager, vectorizer)
    info_entry->call_info = call_info; 
  
    // parse call_info string and generate a function entry
    parse_info_string((char*)(call_info), *info_entry);

    // bind the call_info field to a history map to avoid reprocessing
    stored_call_info.bind(call_info, info_entry);

    // make a copy and return
    entry = *info_entry;
  }
  
  entry.name = fname;
}
