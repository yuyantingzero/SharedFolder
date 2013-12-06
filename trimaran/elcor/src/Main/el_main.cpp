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
//      File:           el_main.cpp
//      Author:         Sadun Anik, Scott Mahlke
//      Created:        May 1995
//      Description:    MAIN for the elcor compiler
//
/////////////////////////////////////////////////////////////////////////////

#include "el_main.h"
#include "el_control.h"
#include "region_purge.h"
#include "el_loop_tools.h"
#include "ir_reader.h"
#include "ir_writer.h"
#include "el_args.h"
#include "el_clock_init.h"
#include "dbg.h"
#include "codegen.h"
#include <sys/resource.h>

/*
#ifdef __cplusplus
extern "C" {
#endif

extern int setpriority(int, int, int);

#ifdef __cplusplus
}
#endif
*/

//
// Convert L_token_type to its Elcor equivalent
//

void El_convert_input_token_type(void)
{
    switch (L_token_type) {
        case L_INPUT_EOF:
	    El_input_type = EL_INPUT_EOF;
	    break;
        case L_INPUT_MS:
        case L_INPUT_VOID:
        case L_INPUT_BYTE:
        case L_INPUT_WORD:
        case L_INPUT_LONG:
        case L_INPUT_LONGLONG:
        case L_INPUT_FLOAT:
        case L_INPUT_DOUBLE:
        case L_INPUT_ALIGN:
        case L_INPUT_ASCII:
        case L_INPUT_ASCIZ:
        case L_INPUT_RESERVE:
        case L_INPUT_GLOBAL:
        case L_INPUT_WB:
        case L_INPUT_WW:
        case L_INPUT_WI:
        case L_INPUT_WF:
        case L_INPUT_WF2:
        case L_INPUT_WS:
        case L_INPUT_ELEMENT_SIZE:
	case L_INPUT_DEF_STRUCT:
	case L_INPUT_DEF_UNION:
	case L_INPUT_DEF_ENUM:
	case L_INPUT_FIELD:
	case L_INPUT_ENUMERATOR:
	    El_input_type = EL_INPUT_DATA;
	    break;
        case L_INPUT_FUNCTION:
	    El_input_type = EL_INPUT_CODE;
	    break;
	default:
	    El_punt("El_convert_input_token_type: unknown input type!");
	    break;
    }
}

/*========================================================================*/
/*
 *	Copies of Lcode routines, these will go away once we do
 *	not link with Lcode.
 */
/*========================================================================*/\

static void El_process_input(void)
{
   El_datalist *D = NULL;
   L_Datalist *datalist = NULL;
   Procedure *R = NULL;
   L_Func *fn = NULL;
   switch (El_input_type) {

    /* No more input */
    case EL_INPUT_EOF:
      break;

    /* Data token */
    case EL_INPUT_DATA:

      /*** Input ***/
      if (El_input_format==EL_IO_REBEL) {
          D = (El_datalist *) El_input;
      }
      /* Convert Lcode data into Elcor data */
      else if (El_input_format==EL_IO_LCODE) {
          D = El_lcode_to_elcor_datalist(L_data_list);
          L_delete_datalist(L_data_list);
      }
      else {
          El_punt("El_process_input: unrecognized input format type for data!!");
      }

      /*** Processing ***/
      /* Currently this is a NULL step, but future data processing should be
         invoked from this function */
      El_process_data(D);

      /*** Ouput ***/
      if (El_output_format==EL_IO_REBEL) {
          ir_write(*EL_OUT_STREAM, D);
      }
      /* Convert Elcor data back to Lcode data */
      else if (El_output_format==EL_IO_LCODE) {
          datalist = El_elcor_to_lcode_datalist(D);

          /* Integrity checking of the Lcode data */
          if (El_check_lcode) {
             L_check_datalist(datalist);
          }

	  /* Print out data using Lcode writer */ 
          L_print_datalist(L_OUT, datalist);
          L_delete_datalist(datalist);
      }
      else if (El_output_format != EL_IO_NONE) {
          El_punt("El_process_input: unrecognized output format type for data!!");
      }

      /* Deallocate the Elcor data */
      delete D;

      break;


    /* Function text token */
    case EL_INPUT_CODE:

      /*** Start the global clock ***/
      if (El_do_run_time) {
          El_clock_proc.start();
      }

      /*** Initialize Global Symbol Table ***/
      Operand::init_global_symbol_table();

      /*** Input ***/
      if (El_input_format==EL_IO_REBEL) {
	R = (Procedure *) El_input;
	if ((El_do_run_time) || (El_do_block_run_time))
	  El_stopwatch_print_init((char *)(R->get_name()));
      }
      /* Convert Lcode function into an Elcor Procedure */
      else if (El_input_format==EL_IO_LCODE) {
          El_set_memvr_setup_mode();
          if ((El_do_run_time) || (El_do_block_run_time))
              El_stopwatch_print_init(L_fn->name);
          El_setup_lcode_for_elcor(L_fn);
          R = El_lcode_to_elcor_func(L_fn) ;
          L_delete_func(L_fn);
      }
      else {
          El_punt("El_process_input: unrecognized input format type for code!!");
      }

      /*** Processing ***/
      /* This is essentially main for all practical purposes and is where the
	 Elcor transformations occur */
      R = El_process_function(R) ;

      Codegen::createDataSection(*EL_OUT_STREAM);

      /*** Ouput ***/
      if (El_output_format==EL_IO_REBEL) {
	if (dbg(status, 2))
	  cout << "Writing REBEL procedure '" << R->get_name() << "'\n";
	  if (! R->flag(EL_PROC_REGISTER_ALLOCATED)) {
	      El_fix_all_pbr_info(R, false);
	  }
          ir_write(*EL_OUT_STREAM, R);
      }

      /* Convert Elcor Procedure back to an Lcode function */
      else if (El_output_format==EL_IO_LCODE) {
	  El_remove_all_loop_regions(R);
          /* El_remove_all_tree_regions(R) */
	  if (! R->flag(EL_PROC_REGISTER_ALLOCATED)) {
	      El_fix_all_pbr_info(R, false);
	  }
	  fn = El_elcor_to_lcode_func(R, true);
          El_setup_lcode_for_impact(fn);

          /* Integrity checking of the Lcode function */
          if (El_check_lcode) {
              L_clear_src_flow(fn);
              L_rebuild_src_flow(fn);
              L_check_func(fn);
          }
   
	  /* Prnt out function using Lcode writer */ 
	  if (dbg(status, 2))
	    cout << "Writing LCODE procedure '" << R->get_name() << "'\n";
          L_print_func(L_OUT, fn);
          L_delete_func(fn);

      }
      else if (El_output_format != EL_IO_NONE) {
          El_punt("El_process_input: unrecognized output format type for code!!");
      }

      /* Deallocate the Elcor procedure */
      region_purge (R);
      /*** Clear Global Symbol Table ***/
      Operand::clear_global_symbol_table();

      /*** Stop the global clock ***/
      if (El_do_run_time) {
          El_clock_proc.stop();
          El_stopwatch_print_time("total_run_time",
				El_clock_proc.last_elapsed());
      }

      break;

    default:
      El_punt("El_process_input: illegal token");
   }
}

/*
 *	This function is just a wrapper around Impact L_get_input() to 
 *	concatenate consecutive data's into a datalist for convenience
 */
int El_get_input(void)
{
    int input_token_type, id;
    char *token;
    L_Datalist_Element *element;

    input_token_type = L_get_input();

    /* If we just got a data, get more until a non-data token is encountered */
    if (L_data_token_type(input_token_type)) {
	L_data_list = L_new_datalist();
	element = L_new_datalist_element(L_data);
	L_concat_datalist_element(L_data_list, element);

	while (1) {
	    if (! L_input_binary_format)
	        token = L_peek_next_real_lcode_token(&L_input_buf);
	    else
		token = L_binary_peek_next_string(L_IN, &L_input_buf);
	    if (token == NULL)
		id = L_INPUT_EOF;
	    else
	        id = L_lcode_id(token);
	    if (! L_data_token_type(id))
		break;
	    input_token_type = L_get_input();
	    element = L_new_datalist_element(L_data);
	    L_concat_datalist_element(L_data_list, element);
	}
    }

    return (input_token_type);
}

void El_gen_code(Parm_Macro_List *command_line_macro_list) 
{
    // try to autodetect the input format
    if (El_input_format == EL_IO_AUTODETECT) {
      ifstream inf(El_input_file_name);
      char line[10];

      // just probe first 100 lines
      for (int x = 0; inf && x < 100; x++) {
        // we're only interested in the beginning of each line
        inf.getline(line, 10);
    
        if (line[0] == '/') {
          // if line starts with /, it must be rebel.  (it's not valid in lcode)
          El_input_format = EL_IO_REBEL;
          break;
        } else if (line[0] == '(') {
          // if line starts with (, it must be lcode.
          El_input_format = EL_IO_LCODE;
          break;
        } else if (line[0] >= 'a' && line[0] <= 'z') {
          // must be rebel.
          El_input_format = EL_IO_REBEL;
          break;
        }
      }
      inf.close();
    
      // use lcode as default, as it gives a more obvious error if it's wrong.
      if (El_input_format == EL_IO_AUTODETECT)
        El_input_format = EL_IO_LCODE;
    }

    /* Rebel input */
    if (El_input_format==EL_IO_REBEL) {
      if (dbg (status, 2))
	cout << "Opening REBEL input file '" << El_input_file_name << "'\n";
      
      EL_IN_STREAM = new IR_instream(El_input_file_name);

      while (ir_read(*EL_IN_STREAM) != EL_INPUT_EOF) {
	El_process_input();
      }

      delete EL_IN_STREAM;
    }

    /* Lcode input */
    else if (El_input_format==EL_IO_LCODE) {
      if (dbg (status, 2))
	cout << "Opening LCODE input file '" << L_input_file << "'\n";

      L_open_input_file(L_input_file);

      while (El_get_input() != L_INPUT_EOF) {
	El_convert_input_token_type();
	El_process_input();
      }
      
      L_close_input_file(L_input_file);
    }

    /* Allow the user to run the compiler w/o input here */
    else {
 	El_input_type = EL_INPUT_CODE;
	El_process_function(NULL);
    }
}

/*========================================================================*/
/*
 *	MAIN itself
 */
/*========================================================================*/

int main(int argc, char **argv, char** envp)
{

   El_Arg arg[EL_MAX_ARG];
   int n_arg, i, j;
   Parm_Macro_List *external_list;
   int read_i, read_o, read_stats;

   /* 
    * Initialize io vars
    */
   /* Impact vars */
   L_arch = (char*)"playdoh";
   L_model = (char*)"v1.0";
   L_input_file = (char*)"stdin";
   L_output_file = (char*)"stdout";
   L_filelist_file = NULL;
   /* Elcor vars */
   El_arch = (char*)"HPL-PD";
   El_model = (char*)"v1.0";
   El_input_file_name = (char*)"stdin";
   El_output_file_name = (char*)"stdout";
   El_stat_file_name = (char*)"ELCOR_STATS";
   El_time_file_name = (char*)"ELCOR_TIME";
   El_resource_file_name = (char*)"ELCOR_RESOURCE";
   El_crit_path_file_name = (char*)"ELCOR_CRIT_PATH";

   /* save the current Lcode pass name for error messages */
   L_curr_pass_name = argv[0];

   /*
    * Get macro definitions from command line and environment.
    * This is the updated version of command_line_macro_list,
    * any it may be used in the same way.
    */
   external_list = L_create_external_macro_list (argv, envp);
   L_command_line_macro_list = external_list;

   /* 
    * Get L_parm_file from command line (-p path), or environment
    * variable "STD_PARMS_FILE", or default to "./STD_PARMS"
    */
   El_parm_file_name = L_get_std_parm_name (argv, envp, "ELCOR_PARMS_FILE",
				      "./ELCOR_PARMS");
   L_parm_file = El_parm_file_name;
	
   /*
    * Load parameters now, so that command line arguements
    * will override them if specified.  Pass in all command line
    * macro definitions.
    */

   /* Impact parameters */
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Lglobal", L_read_parm_global);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Larchitecture", L_read_parm_arch);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Lfile", L_read_parm_file);
   /*
    * SAM 8-97
    * Bypass alloc routines so the memory used by Impact can be reclaimed
    * and used by Elcor.  Otherwise, memory isn't freed until process is done
    */
   // bypass_alloc_routines = 1;

   /* Elcor parameters */
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Elcor_Global", El_read_parm_global);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Elcor_Architecture", El_read_parm_arch);
   L_load_parameters (El_parm_file_name, external_list,  
		      "(Elcor_IO", El_read_parm_io);

   El_set_io_formats();

   /* 
    * If the first argument does not contain an -, it is
    * ignored by Lcode.  Punt if this is the case.
    */
   if ((argv[1] != NULL) && (argv[1][0] != '-')) {
      El_punt ("main: Unknown command line option '%s'.", argv[1]);
   }

   /* Mark that we have not read the input or output or stats file yet */
   read_i = 0;
   read_o = 0;
   read_stats = 0;

   /* Process the rest of the command line arguments */
   n_arg = El_get_arg(argc-1, argv+1, arg, EL_MAX_ARG);
   for (i=0; i < n_arg; i++)
   {
      char *option;
      option = arg[i].name;

      /* Get input file name */
      if (! strcmp(option, "-i")) 
      {
	 /* Punt if input file specified twice */
	 if (read_i)
	    El_punt ("main: Parsing command line: -i specified twice.\n");

	 /* Make sure file name specified */
	 if (arg[i].count < 1)
	    El_punt("main: Parsing command line: -i needs input_file_name.");
	 /* Make sure that only one file name specified */
	 if (arg[i].count > 1)
	 {
	    fprintf (stderr, "Error parsing command line: -i");
	    for (j = 0; j < arg[i].count; j++)
	       fprintf (stderr, " %s", arg[i].value[j]);
	    fprintf (stderr, "\n");
	    El_punt ("main: Cannot specify more than one input file with -i.");
	 }

	 L_input_file = strdup(arg[i].value[0]);
	 El_input_file_name = strdup(arg[i].value[0]);

	 /* Mark that we have read an input file name */
	 read_i = 1;
      }

      /* Get output file name */
      else if (! strcmp(option, "-o")) 
      {
	 /* Punt if output file specified twice */
	 if (read_o)
	    El_punt ("main: Parsing command line: -o specified twice.\n");

	 /* Make sure file name specified */
	 if (arg[i].count < 1)
	    El_punt("main: Parsing command line: -o needs output_file_name.");
	 /* Make sure only one file name specified */
	 if (arg[i].count > 1)
	 {
	    fprintf (stderr, "Parsing command line: -o");
	    for (j = 0; j < arg[i].count; j++)
	       fprintf (stderr, " %s", arg[i].value[j]);
	    fprintf (stderr, "\n");
	    El_punt ("main: Cannot specify more than one output file with -o.");
	 }

	 L_output_file = strdup(arg[i].value[0]);
	 El_output_file_name = strdup(arg[i].value[0]);

	 /* Mark that we have read an output file */
	 read_o = 1;
      } 

      else if (! strcmp(option, "-st")) 
      {
	 /* Punt if stats file specified twice */
	 if (read_stats)
	    El_punt ("main: Parsing command line: -st specified twice.\n");

	 /* Make sure file name specified */
	 if (arg[i].count < 1)
	    El_punt("main: Parsing command line: -st needs stats_file_name.");
	 /* Make sure only one file name specified */
	 if (arg[i].count > 1)
	 {
	    fprintf (stderr, "Parsing command line: -st");
	    for (j = 0; j < arg[i].count; j++)
	       fprintf (stderr, " %s", arg[i].value[j]);
	    fprintf (stderr, "\n");
	    El_punt ("main: Cannot specify more than one stats_file_name file with -o.");
	 }

	 L_output_file = strdup(arg[i].value[0]);
	 El_stat_file_name = strdup(arg[i].value[0]);

	 /* Mark that we have read the stats file */
	 read_stats = 1;
      } 

      /* Ignore -p */
      else if (strcmp (option, "-p") == 0)
	 ;

      /* Ignore -P, -F  (and for now -D), accept to make sure there
       * are no argument after it.
       */
      else if ((option[0] == '-') && 
	       ((option[1] == 'P') || (option[1] == 'D') || 
		(option[1] == 'F')))
      {
	 /* Make sure there is nothing after the -Pmacro_name=val */
	 if (arg[i].count > 0)
	    El_punt ("main: Unknown command line option '%s'.", arg[i].value[0]);
	    
      }

      /* Otherwise, punt, unknown commmand line arguments */
      else
      {
	 /* Print out what we are punting on */
	 fprintf (stderr, "Error parsing command line: %s", option);
	 for (j = 0; j < arg[i].count; j++)
	    fprintf (stderr, " %s", arg[i].value[j]);
	 fprintf (stderr, "\n");

	 El_punt ("main: Unknown command line option.");
      }
      free(arg[i].name);
   }

   /* 
    * Renice this process to L_nice_value.  
    * Ignore the error codes that will be returned if
    * already reniced or El_nice_value is invalid. 
    */
   setpriority(PRIO_PROCESS, 0, El_nice_value);

   /*
    *	SET UP THE Lcode ENVIRONMENT.
    */
   L_init_symbol();
   if (El_output_format==EL_IO_LCODE)
       L_OUT = L_open_output_file(L_output_file);
   L_setup_alloc_pools();
   M_set_machine(L_arch, L_model, "XXX");
   M_define_macros(L_macro_symbol_table);
   M_define_opcode_name(L_opcode_symbol_table);

   /*
    *	SET UP the Elcor enviroment
    */
   el_init_lcode_opcode_maps();
   if (El_output_format==EL_IO_REBEL)
       EL_OUT_STREAM = new IR_outstream(El_output_file_name);

   /*
    *	General initialization (all users)
    */
   El_init_all(external_list);

   /*
    *	User specific initialization
    */
   El_preprocess_program(external_list);

   /*
    *	PROCESS INPUT.
    */
   El_gen_code(external_list);

   /*
    *	Post processing steps
    */
   El_postprocess_program();
   El_deinit_all();
   if (El_output_format==EL_IO_REBEL)
       delete EL_OUT_STREAM;
   else if (El_output_format==EL_IO_LCODE)
       L_close_output_file(L_OUT);

   /*
    *	One last check of data structure allocation
    */
   if (L_check_data_struct_alloc) {
      L_check_alloc_for_func();
      L_check_alloc_for_data();
   }

   /* 
    * Print warning about unused command line parameter arguments if
    * we have actually manipulated an Lcode function.  This prevents
    * producing warning messages for files only containing data
    * segments.
    */
   if (L_func_read)
      L_warn_about_unused_macros (stderr, external_list);

   return (0);
}
