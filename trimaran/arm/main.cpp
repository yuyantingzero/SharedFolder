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

Copyright 2007 University of Michigan

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



#include "el_main.h"
#include "el_init.h"
#include "el_extern.h"

#include "util.h"
#include "region_purge.h"
#include "ir_reader.h"
#include "process_rebel.h"
#include "el_main.h"
#include "mdes.h"
#include "data_segment.h"

List<El_datalist *> datalists_to_delete;

static void 
process_input(void)
{
  int data_section_num = 0;

  while (ir_read(*EL_IN_STREAM) != EL_INPUT_EOF) {
    switch (El_input_type) {
      
    case EL_INPUT_EOF:
      break;
      
    case EL_INPUT_DATA:
//       cerr << "emitting data section " << data_section_num << endl;
      emit_data_section(data_section_num++);     
      break;
 
    case EL_INPUT_CODE:  // text token
      Procedure *procedure;
      L_Func *fn;
      
      // initialize the global symbol table
      Operand::init_global_symbol_table();
      
      // get the input procedure
      procedure = (Procedure *) El_input;
      
      procedure = process_function(procedure) ;
      
      // cleanup
      region_purge (procedure);
      Operand::clear_global_symbol_table();
      
      break;
      
    default:
      El_punt("El_process_input: illegal token");
    }
  }
}

void
do_data_prepass()
{
  int data_section_num = 0;

  // we're only interested in data segments here, ignore everything else
  while (ir_read(*EL_IN_STREAM) != EL_INPUT_EOF) {
    if (El_input_type == EL_INPUT_DATA) {
      El_datalist *datalist;
      
      // get the input data
      datalist = (El_datalist *) El_input;
      
      // process the data
      process_data(datalist, data_section_num++);      

      // add the datalist to the list of cleanup items
      datalists_to_delete.add_tail(datalist);
    }
  }
}

void
read_impact_structure_definitions()
{
  // read the impact structure definition file
  EL_IN_STREAM = new IR_instream("__impact_data.O_el");

//   cerr << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
//   cerr << "reading impact structs: " << endl;

  while (ir_read(*EL_IN_STREAM) != EL_INPUT_EOF) {
    if (El_input_type == EL_INPUT_DATA) {
      El_datalist *datalist;
      
      // get the input data
      datalist = (El_datalist *) El_input;
      
      // process the data
      process_data(datalist, -1);      
      
      //delete datalist;
    }
  }

//   cerr << "done reading impact structs: " << endl;
//   cerr << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;


  delete EL_IN_STREAM;
}

void 
translate() 
{
  // initialize our global bookkeeping
  global_data_segments = new List<Data_segment*>();
  segments_to_preinitialize = new List<Data_segment*>();
  global_rename_map = new Hash_map<string, string>(&hash_string, 2048);
  global_struct_or_union_definitions = 
    new Hash_map<string, Struct_or_union_definition*> (&hash_string, 1024);
  assert(global_data_segments && segments_to_preinitialize && 
	 global_rename_map && global_struct_or_union_definitions && "out of memory");

  read_impact_structure_definitions();

  // create a new ir stream
  EL_IN_STREAM = new IR_instream(El_input_file_name);

  // prepass the data segments so that we can reorganize if necessary
  do_data_prepass();

  // start anew at the begining of the input file
  delete(EL_IN_STREAM);
  EL_IN_STREAM = new IR_instream(El_input_file_name);

  process_input();
  
  delete EL_IN_STREAM;    
}

void
elcor_init(int argc, char **argv, char** envp)
{
   int n_arg, i, j;
   Parm_Macro_List *external_list;
   int read_i, read_o, read_stats;

   external_list = L_create_external_macro_list (argv, envp);
   L_command_line_macro_list = external_list;

   El_parm_file_name = L_get_std_parm_name (argv, envp, "ELCOR_PARMS_FILE",
				      "./ELCOR_PARMS");
   L_parm_file = El_parm_file_name;

   L_load_parameters (El_parm_file_name, external_list,  
		      "(Lglobal", L_read_parm_global);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Larchitecture", L_read_parm_arch);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Lfile", L_read_parm_file);

   L_load_parameters (El_parm_file_name, external_list,  
		      "(Elcor_Global", El_read_parm_global);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Elcor_Architecture", El_read_parm_arch);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Elcor_IO", El_read_parm_io);

   El_set_io_formats();

   El_init_parms(external_list);

   /*
    *	Map initialization
    */
   el_init_elcor_opcode_maps();
   el_init_elcor_macro_maps();

   // This needs to be true in order for the MDES to correctly parse
   // the custom op fields, and currently the translator doesn't have
   // a parms file for this.
   El_do_prepass_custom_ops = 1;
   string arm_mdes_fname(getenv("ELCOR_HOME"));
   arm_mdes_fname = arm_mdes_fname + "/mdes/arm/arm_elcor_std.lmdes2";

   MDES *default_mdes = new MDES(eString(arm_mdes_fname), "MDES_debug");
   set_current_MDES(default_mdes);   
}

int 
main(int argc, char **argv, char** envp)
{
  string fname;

  parse_command_line(argc, argv, fname);

  // this file is special, we'll handle it on our own later
  if (fname == "__impact_data.O_el")
    return 0;

  // remove the filename from the argument list so that it isn't 
  // misinterpretted by elcor/impact
  argv[argc-1] = NULL;
  elcor_init(argc-1, argv, envp);

  // other initializations
  El_input_file_name = strdup(fname.c_str());
  El_input_type = EL_INPUT_CODE;

  // start the rebel translation process
  translate();

  // cleanup
  for (List_iterator<El_datalist *> li(datalists_to_delete); li!=0; li++){
    El_datalist *list = *li;
    delete list;
  }

  delete global_rename_map;
  delete global_data_segments;
  delete segments_to_preinitialize;

  return (0);
}
