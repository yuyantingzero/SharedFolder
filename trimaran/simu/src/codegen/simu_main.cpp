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
// File:   simu_main.cpp
// Author: Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// This module contains the main of the assembler.
// It is basically a driver routine that carries out the following
// sequence of actions,
//
// -> Invokes the .inc Stream Processor
//       (To analyze typedef information)
// -> Invokes the .c Stream Processor
//       (To copy global data)
// -> Invokes the Vector Prototypes generator
//       (Writes out prototypes for all vectors since they will be 
//        refered to from the Simulation tables)
// -> Invokes the function to write out the literal pool
//       (Currently contains float and double items only)
// -> The Simulation Table generator
//       (Reads Rebel and writes out the tables)
// -> Invokes the vectorizer
//       (Writes out Vector bodies for all functions being called to from the
//        the Rebel source)
// -> Invokes the Packager
//       (Writes out wrapper bodies for all functions present in the Rebel 
//        source)
///////////////////////////////////////////////////////////////////////////////

#include "simu_main.h"
#include "el_args.h"
#include "simu.h"
#include "simu_el_processor.h"
#include "streamBuffer.h"


static const int number_of_lines_in_header  = 7;

const char*  SIMU_GLOBAL_DATA_INIT_FUNCTION = 
"__S_initialize_benchmark_global_data_if_necessary()";

extern void gen_wrappers(ofstream &outf, const Host_Info&);
extern void gen_vectors (ofstream &outf);

extern void init_simulator_parms(void);

static eString Simu_file_name;
static eString Simu_include_file_name;
static eString Simu_tables_file_name;
// for assembly code generation
static eString Simu_asm_file_name;
// headers requested for inclusion
static Vector<eString> Simu_user_header_files;

static const char* S_benchmark_init_file = "benchmark_data_init.simu.c";
static eString sequencing_id;

// simulator parameters
extern int S_nual_equals;             // the NUAL model
extern int S_virtual_regs;            // virtual or physical register model
extern int S_performance_monitoring;  // enable trace generator
extern int S_binary_trace_format;     // binary trace format
extern int S_control_flow_trace;      // control flow trace
extern int S_address_trace;           // address trace
extern int S_dynamic_stats;           // execution statistics
// for assembly code generation
extern int S_generate_assembly;       // generate assembly code

inline void start_timer_for_phase(const char* phase);
inline void stop_timer_for_last_phase();
void add_init_function_to_master_file(const bool);


extern eString get_macro_file (Macro_name macro);
extern int get_macro_reg_num(Macro_name macro);

// write common header in generated output files
static void write_common_header(streamBuffer& out, const eString& file_name, 
					  const char* description)
{
  out << "/******************************************************* " << endl 
      << " *" << endl 
      << " * File:        " << file_name << endl
      << " * Description: " << description << endl
      << " * Note:        DO NOT EDIT!" << endl 
      << " *" << endl
      << " *******************************************************/" << endl;
	
  out.commit();
}

// mchu - new impact separates out data header
// kind returns whether it is O, S, or HS
static bool check_for_data_header(int *kind)
{
  // mchu - impact now outputs data definitions to a different file
  //        which needs to be included
  eString data_file_name_O = "__impact_data.O_el";
  eString data_file_name_S = "__impact_data.S_el";
  eString data_file_name_HS = "__impact_data.HS_el";
  if ((Simu_file_name != "__impact_data.O_el.simu.c") &&
      (Simu_file_name != "__impact_data.S_el.simu.c") && 
      (Simu_file_name != "__impact_data.HS_el.simu.c")) {
    FILE *fp_O = fopen((char *) data_file_name_O, "r");
    FILE *fp_S = fopen((char *) data_file_name_S, "r");
    FILE *fp_HS = fopen((char *) data_file_name_HS, "r");
    // does the file actually exist?
    if (fp_O != NULL) {
      *kind = 0;
      return true;
    } else if (fp_S != NULL) {
      *kind = 1;
      return true;
    } else if (fp_HS != NULL) {
      *kind = 2;
      return true;
    }
  }
  return false;
}


// *.simu.c header
static void Simu_write_header(ofstream& outf)
{
  streamBuffer out(outf);
    
  write_common_header(out, Simu_file_name, (char*) "Emulatable Assembly file");

  int kind = -1;
  if (check_for_data_header(&kind)) {
     if (kind == 0)
      out << endl
          << "#include \"__impact_data.O_el.simu.c.inc\"" << endl;
     else if (kind == 1)
      out << endl
          << "#include \"__impact_data.S_el.simu.c.inc\"" << endl;
     else if (kind == 2)
      out << endl
          << "#include \"__impact_data.HS_el.simu.c.inc\"" << endl;
  }

  out << endl
      << "#include \"" << Simu_include_file_name << "\"" << endl
      << "#include \"" << Simu_tables_file_name  << "\"" << endl
      << endl << endl
      << "extern void " << SIMU_GLOBAL_DATA_INIT_FUNCTION << ";" << endl
      << endl;

  out.commit();
}


// *.simu.c.tbls header
static void Simu_write_header_of_tables(ofstream& outf)
{
  streamBuffer out(outf);
  
  write_common_header(out, Simu_tables_file_name, 
			    (char*) "Emulatable Assembly Simulation Tables file");

  out << endl << endl;
  
  out.commit();
}


// *.simu.c.inc header
static void Simu_write_header_of_include(ofstream& outf)
{
  streamBuffer out(outf);
  
  write_common_header(out, Simu_include_file_name, 
			    (char*) "Emulatable Assembly include file");
    
  out << endl 
	<< "#include \"PD.h\"" << endl;

  // include any other headers
  for (int i = 0; i < Simu_user_header_files.dim(); i++) {
    out << "#include \"" << Simu_user_header_files[i] << "\"" << endl;
  }

  out << endl;

  out.commit();
}


// *.simu.asm header
static void Simu_write_header_of_asm(ofstream& outf)
{
  streamBuffer out(outf);

  write_common_header(out, Simu_asm_file_name,
                      (char*) "HPL-PD ASSEMBLY file");

  out.commit();
}


///////////////////////////////////////////////////////////////////////////////
// This is the main driver module of the assembler
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv, char** envp)
{
  El_Arg arg[EL_MAX_ARG];

  Parm_Macro_List *external_list;
  bool read_i, read_o;

  // initialize Elcor vars
  El_arch  = (char*)"HPL-PD";
  El_model = (char*)"v1.0";

  // Get macro definitions from command line and environment.
  // This is the updated version of command_line_macro_list,
  // and it may be used in the same way.
  external_list = L_create_external_macro_list(argv, envp);
   
  // Get L_parm_file from command line (-p path), or environment
  // variable "STD_PARMS_FILE", or default to "./STD_PARMS"
  El_parm_file_name = L_get_std_parm_name(argv, envp, "ELCOR_PARMS_FILE",
							"./ELCOR_PARMS");
	
  // Load parameters now, so that command line arguments
  // will override them if specified.  Pass in all command line
  // macro definitions.

  // Elcor parameters
  L_load_parameters(El_parm_file_name, external_list,  
			  "(Elcor_Global", El_read_parm_global);
  L_load_parameters(El_parm_file_name, external_list,  
			  "(Elcor_Architecture", El_read_parm_arch);
  L_load_parameters(El_parm_file_name, external_list,  
			  "(Elcor_IO", El_read_parm_io);
  L_load_parameters(El_parm_file_name, external_list,
			  "(Elcor_Driver", El_driver_read_parm);
    
  // Simulator parameters
  char* S_parm_file_name;
  S_parm_file_name = L_get_std_parm_name(argv, envp, "SIMU_PARMS_FILE",
						     "./SIMULATOR_DEFAULTS");
  L_load_parameters(S_parm_file_name, external_list,
			  "(simulator", readParmSimu);
    
  El_set_io_formats();

  start_timer_for_phase("setup");
    
  // If the first argument does not contain an -, it is
  // ignored by Lcode.  Punt if this is the case.
  if ((argv[1] != NULL) && (argv[1][0] != '-')) {
    S_punt ("main: Unknown command line option '%s'.", argv[1]);
  }

  // Mark that we have not read the input or output file yet
  read_i = read_o = false;

  // Process the rest of the command line arguments
  int n_arg = El_get_arg(argc-1, argv+1, arg, EL_MAX_ARG);
  for (int i = 0; i < n_arg; i++) {
    char *option;
    option = arg[i].name;
	    
    // Get rebel input file name
    if (strcmp(option, "-el") == 0) {
      // Punt if input file specified twice
      if (read_i)
	  S_punt("Parsing command line: -el specified twice.\n");

      // Make sure file name specified
      if (arg[i].count < 1)
	  S_punt("Parsing command line: -el needs input_file_name.");

      // Make sure that only one file name specified
      if (arg[i].count > 1) {
	  fprintf (stderr, "Error parsing command line: -el");
	  for (int j = 0; j < arg[i].count; j++)
	    fprintf (stderr, " %s", arg[i].value[j]);
	  fprintf (stderr, "\n");
	  S_punt("Cannot specify more than one input file with -el.");
      }
	    
      El_input_file_name = strdup(arg[i].value[0]);

      // Mark that we have read an input file name 
      read_i = true;
    }

    // Get output file name 
    else if (strcmp(option, "-o") == 0) {
      // Punt if output file specified twice
      if (read_o)
	  S_punt("Parsing command line: -o specified twice.\n");

      // Make sure file name specified 
      if (arg[i].count < 1)
	  S_punt("Parsng command line: -o needs output_file_name.");
	    
      // Make sure only one file name specified 
      if (arg[i].count > 1) {
	  fprintf (stderr, "Parsing command line: -o");
	  for (int j = 0; j < arg[i].count; j++)
	    fprintf (stderr, " %s", arg[i].value[j]);
	  fprintf (stderr, "\n");
	  S_punt("Cannot specify more than one output file with -o.");
      }
	    
      Simu_file_name = strdup(arg[i].value[0]);
       
      // Mark that we have read an output file 
      read_o = true;
    } 
    
    // Get output file name 
    else if (strcmp(option, "-I") == 0) {
      // Make sure file name specified 
      if (arg[i].count < 1)
	  S_punt("Parsing command line: -I needs file_name.");
	    
      // Make sure only one file name specified 
      if (arg[i].count > 1) {
	  fprintf (stderr, "Parsing command line: -I");
	  for (int j = 0; j < arg[i].count; j++)
	    fprintf (stderr, " %s", arg[i].value[j]);
	  fprintf (stderr, "\n");
	  S_punt("Cannot specify more than one file per -I.");
      }

	Simu_user_header_files.resize(Simu_user_header_files.dim() + 1);
	Simu_user_header_files[Simu_user_header_files.dim() -1] = strdup(arg[i].value[0]);
    }

    // Ignore -p
    else if (strcmp (option, "-p") == 0) {
    }

    // Ignore -P, -F (and for now -D), except to make sure there
    // are no argument after it.
    else if ((option[0] == '-') && 
		 ((option[1] == 'P') || (option[1] == 'D') ||
		  (option[1] == 'F'))) {
      // Make sure there is nothing after the -Pmacro_name=val
      if (arg[i].count > 0)
	  S_punt("Unknown command line option '%s'", arg[i].value[0]);
    }

    // Otherwise, punt, unknown commmand line arguments 
    else {
      // Print out what we are punting on 
      fprintf (stderr, "Error parsing command line: %s", option);
      for (int j = 0; j < arg[i].count; j++)
	  fprintf (stderr, " %s", arg[i].value[j]);
      fprintf (stderr, "\n");

      S_punt("Unknown command line option");
    }
  }

  if (!read_i) {
    S_warn("Elcor input file not specified");
    exit(0);
  }

  if (!read_o) {
    S_warn("Emulation output file not specified");
    exit(0);
  }

  // Initialize execution and simulation parameters
  init_simulator_parms();

  // if performance monitoring turned off then disable everything else 
  // In the future one can have diff param sets for each procedure
  if (!S_performance_monitoring) {
    S_control_flow_trace = 0;
    S_address_trace = 0;
    S_dynamic_stats = 0;
  }
    
  // Driver code starts here
  ofstream *o_file;
  Simu_file_name = Simu_file_name.substr(Simu_file_name.charrpos('/', 1)+1,
						     Simu_file_name.length()-1);

  o_file  = new ofstream((char *) Simu_file_name);
  if (o_file->fail())
    S_punt("Could not open file ", (char*) Simu_file_name);

  // Output simulation tables to a unique file
  ofstream *o_tbls_file;
  Simu_tables_file_name = Simu_file_name;
  Simu_tables_file_name.cat(".tbls");

  o_tbls_file = new ofstream((char *) Simu_tables_file_name);
  if (o_tbls_file->fail())
    S_punt("Could not open file ", (char*) Simu_tables_file_name);

  // Create handle to *.inc file
  ofstream *o_inc_file;
  Simu_include_file_name = Simu_file_name;
  Simu_include_file_name.cat(".inc");

  o_inc_file = new ofstream((char *) Simu_include_file_name);
  if (o_inc_file->fail())
    S_punt("Could not open file ", (char*) Simu_include_file_name);

  // for assembly code generation
  ofstream *o_asm_file = NULL;
  if (S_generate_assembly) {
    Simu_asm_file_name = Simu_file_name;
    Simu_asm_file_name.cat(".asm");

    o_asm_file = new ofstream((char*) Simu_asm_file_name);
    if (o_asm_file->fail())
	S_punt("Could not open file ", (char*) Simu_asm_file_name);
  }

  // write the header of all three files
  Simu_write_header(*o_file);
  Simu_write_header_of_tables(*o_tbls_file);
  Simu_write_header_of_include(*o_inc_file);
  if (S_generate_assembly) {
    Simu_write_header_of_asm(*o_asm_file);
  }

  stop_timer_for_last_phase();

  // Phase 1 : Invoke the Rebel processor, dep on p#0
  start_timer_for_phase("1");
  eString el_file_name(El_input_file_name);
  el_Buffer el_buf(el_file_name, *o_tbls_file, *o_inc_file, *o_asm_file);

  // Generate the simulation tables
  Host_Info host_info;
  bool regen_init_file = el_buf.gen_code(sequencing_id, host_info);
  stop_timer_for_last_phase();
 
  // Phase 2
  // call the packager
  start_timer_for_phase("2");
  gen_wrappers(*o_file, host_info);
  stop_timer_for_last_phase();

  // Phase 3
  // call the vectorizer
  start_timer_for_phase("3");
  gen_vectors(*o_file);
  stop_timer_for_last_phase();

  // Phase 4
  start_timer_for_phase("4");
  add_init_function_to_master_file(regen_init_file);
  stop_timer_for_last_phase();

  // Note : The order Phase 1->2->3->4 should be preserved!

  // cleanup
  start_timer_for_phase("cleanup");
  o_file->close();      delete o_file;
  o_tbls_file->close(); delete o_tbls_file; 
  o_inc_file->close();  delete o_inc_file;
  if (S_generate_assembly) {
    o_asm_file->close(); delete o_asm_file;
  }

  S_log("Codegen finished...");
  stop_timer_for_last_phase();
  return (0);
}

inline void start_timer_for_phase(const char* phase)
{
  S_log("starting phase %s", phase); 
  start_timer();
}

inline void stop_timer_for_last_phase()
{
  S_log("last phase took %s", stop_timer());
}


void add_init_function_to_master_file(bool add_new_entry)
{
  S_log("adding initialization function to master file...");

  static char buffer[MAX_BUF_SIZE];
  static Hash_table<eString> entry_list(hash_estring, 100);

  int entry_count = 0;
  eString new_entry;

  // generate the entry to be added
  if (add_new_entry) {
    sprintf(buffer, "%s__PD_initialize_%s_global_data();",
		STAB, (char*) sequencing_id); 
    new_entry = buffer;
  }

  ifstream *i_init_file = new ifstream(S_benchmark_init_file);
  // file may not exist 
  if (!i_init_file->fail()) {
    // read in header plus extra blank line
    for (int i = 0; i < number_of_lines_in_header+1; i++) {
      i_init_file->getline(buffer, MAX_BUF_SIZE);
    }
    
    // read in the current initialization functions count
    eString entry;
    i_init_file->getline(buffer, MAX_BUF_SIZE);
    entry = buffer;
    entry = entry.substr(entry.charrpos('<')+1,entry.charrpos('>')-1);
    entry_count = str_to_int(entry);

    // skip to init function
    do {
      i_init_file->getline(buffer, MAX_BUF_SIZE);
    } while (strstr(buffer, SIMU_GLOBAL_DATA_INIT_FUNCTION) == 0);

    // skip opening brace
    i_init_file->getline(buffer, MAX_BUF_SIZE); 
    // skip if called check
    i_init_file->getline(buffer, MAX_BUF_SIZE); 
    // skip extra blank line
    i_init_file->getline(buffer, MAX_BUF_SIZE);

    // read in entry_count entries
    for (int i = 0; i < entry_count; i++) {
      i_init_file->getline(buffer, MAX_BUF_SIZE);
      entry = buffer;
      
      // if an entry already exists for the function to be added, then
      // do nothing - close file and return
      if (entry == new_entry) {
	  S_warn("%s already found in %s -- will not change file!",
		   (char*) new_entry, S_benchmark_init_file);
	  return;
      }
      
      // otherwise, store the entry for rewriting
      assert (!entry_list.is_member(entry)); // sanity check
      entry_list.add_head(entry);
    }

    // close file so that it may be rewritten rewrite the file
    i_init_file->close();
  }
  
  
  ofstream* o_init_file = new ofstream(S_benchmark_init_file);
  if (o_init_file->fail()) {
    S_punt("Could not open file ", S_benchmark_init_file);
  }
  streamBuffer out(*o_init_file);

  // write header    
  write_common_header(out, S_benchmark_init_file, 
			    (char*) "Global Data Initialization file");
  
  // write new entry count
  out << endl 
      << "/* number of entries is: <" 
      << (add_new_entry ? (entry_count+1) : (entry_count))
      << "> */" << endl 
      << endl
      << "#include \"PD.h\"" << endl
      << endl
      << "static int already_initialized = 0;" << endl
      << endl
      << "__PD_reg** int_ret_file = "<< get_macro_file(INT_RETURN) << ";" << endl
      << "int int_ret_reg_num = "<<get_macro_reg_num(INT_RETURN)<<";" << endl
      << "__PD_reg** lc_file = "<< get_macro_file(LC_REG) << ";" << endl
      << "int lc_reg_num = "<<get_macro_reg_num(LC_REG)<<";" << endl
      << "__PD_reg** esc_file = "<< get_macro_file(ESC_REG) << ";" << endl
      << "int esc_reg_num = "<<get_macro_reg_num(ESC_REG)<<";" << endl
      << "__PD_reg** int_zero_file = "<< get_macro_file(INT_ZERO) << ";" << endl
      << "int int_zero_reg_num = "<<get_macro_reg_num(INT_ZERO)<<";" << endl
      << "__PD_reg** flt_zero_file = "<< get_macro_file(FLT_ZERO) << ";" << endl
      << "int flt_zero_reg_num = "<<get_macro_reg_num(FLT_ZERO)<<";" << endl
      << "__PD_reg** flt_one_file = "<< get_macro_file(FLT_ONE) << ";" << endl
      << "int flt_one_reg_num = "<<get_macro_reg_num(FLT_ONE)<<";" << endl
      << "__PD_reg** dbl_zero_file = "<< get_macro_file(DBL_ZERO) << ";" << endl
      << "int dbl_zero_reg_num = "<<get_macro_reg_num(DBL_ZERO)<<";" << endl
      << "__PD_reg** dbl_one_file = "<< get_macro_file(DBL_ONE) << ";" << endl
      << "int dbl_one_reg_num = "<<get_macro_reg_num(DBL_ONE)<<";" << endl
      << endl
      << "void " << SIMU_GLOBAL_DATA_INIT_FUNCTION << endl << "{" << endl
      << STAB 
      << "if (already_initialized) return; else already_initialized = 1;"
      << endl << endl;

  // output the old entries
  if (entry_count > 0) {
    for (Hash_table_iterator<eString> iter(entry_list); iter != 0; iter++) {
      out << *iter << endl;
    }
  }
  
  // output the new entry
  if (add_new_entry) {
    out << new_entry << endl;
  }

  out << "}" << endl;
  // done
  out.commit();
  o_init_file->close();
}

