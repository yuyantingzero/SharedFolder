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
//      File:           el_init.cpp                  
//      Author:         Scott Mahlke, Sadun Anik, Shail Aditya
//      Created:        June 1995                    
//      Description:    Master initialization routine
//
/////////////////////////////////////////////////////////////////////////////

#include "defs.h"
#include "el_error.h"
#include "el_init.h"
#include "mdes.h"
#include "el_opcode_map.h"
#include "opcode.h"
#include "operand.h"
#include "el_opti_init.h"

/*========================================================================*/
/*
 *	Global Elcor parameters
 */
/*========================================================================*/

char *El_input_file_name = NULL;
char *El_input_format_name = NULL;
El_IO_Format El_input_format = EL_IO_NONE;

char *El_output_file_name = NULL;
char *El_output_format_name = NULL;
El_IO_Format El_output_format = EL_IO_NONE;
int El_indent_width = 2;
// had to change here -- only way to get systolic synthesis to read
// it, since IO_DEFAULTS not read, dcronqui 6/14/00
int El_output_max_line_width = 170; 
int El_print_verbose_header = 1;
char *El_memvr_setup_mode_name = NULL;
El_Memvr_Setup_Mode El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_AUTOMATIC;

char *El_parm_file_name = NULL;
char *El_lmdes_file_name = NULL;

char *El_lmdes_output_file_name = NULL;

Parm_Macro_List *EL_command_line_macro_list = NULL;
char *El_stat_file_name = NULL;
char *El_time_file_name = NULL;
char *El_opcode_file_name = NULL;
char *El_resource_file_name = NULL;       
char *El_crit_path_file_name = NULL;       
char *El_branch_file_name = NULL;


FILE *EL_IN = NULL;
FILE *EL_OUT = NULL;

IR_instream *EL_IN_STREAM = NULL;
IR_outstream *EL_OUT_STREAM = NULL;
ofstream *EL_STAT_STREAM_PTR = NULL;
ofstream *EL_TIME_STREAM_PTR = NULL;
ofstream *EL_OPCODE_STREAM_PTR = NULL;
ofstream *EL_RESOURCE_STREAM_PTR = NULL;
ofstream *EL_CRIT_PATH_STREAM_PTR = NULL;
ofstream *EL_BRANCH_STREAM_PTR = NULL;

ostream *EL_STAT_STREAM = NULL;
ostream *EL_OPCODE_STREAM = NULL;
ostream *EL_RESOURCE_STREAM = NULL;
ostream *EL_CRIT_PATH_STREAM = NULL;
ostream *EL_BRANCH_STREAM = NULL;

void *El_input=NULL;
El_Input_Token El_input_type = EL_INPUT_EOF;

char *El_arch = NULL;
char *El_model = NULL;
int El_arch_speculation_model = GENERAL_SPECULATION;

int El_nice_value = 10;
int El_check_lcode = 0;

int El_memvr_profiled = 0;
int El_punt_on_unknown_op = 1;

void El_read_parm_arch (Parm_Parse_Info *ppi)
{
   L_read_parm_s(ppi, "arch", &El_arch);
   L_read_parm_s(ppi, "model", &El_model);
   L_read_parm_i(ppi, "speculation_model", &El_arch_speculation_model);
   L_read_parm_b(ppi, "assume_infinite_resources",
			&El_assume_infinite_resources);
}

void El_read_parm_io (Parm_Parse_Info *ppi)
{
   L_read_parm_s(ppi, "input_file_name", &El_input_file_name);
   L_read_parm_s(ppi, "input_format", &El_input_format_name);
   L_read_parm_s(ppi, "output_file_name", &El_output_file_name);
   L_read_parm_s(ppi, "output_format", &El_output_format_name);
   L_read_parm_s(ppi, "lmdes_file_name", &El_lmdes_file_name);
   L_read_parm_s(ppi, "lmdes_output_file_name", &El_lmdes_output_file_name);
   L_read_parm_s(ppi, "stat_file_name", &El_stat_file_name);
   L_read_parm_s(ppi, "time_file_name", &El_time_file_name);
   L_read_parm_s(ppi, "opcode_file_name", &El_opcode_file_name);
   L_read_parm_s(ppi, "branch_file_name", &El_branch_file_name);
   L_read_parm_i(ppi, "indent_width", &El_indent_width);
   L_read_parm_i(ppi, "output_max_line_width", &El_output_max_line_width);
   L_read_parm_b(ppi, "print_verbose_header", &El_print_verbose_header);
   L_read_parm_s(ppi, "memvr_setup_mode", &El_memvr_setup_mode_name);
   L_read_parm_b(ppi, "memvr_profiled", &El_memvr_profiled);
   L_read_parm_b(ppi, "punt_on_unknown_op", &El_punt_on_unknown_op);
}

void El_read_parm_global (Parm_Parse_Info *ppi)
{
    /* these are parameters of the parameter library */
    L_read_parm_b (ppi, "punt_on_unknown_parm", &L_punt_on_unknown_parm);
    L_read_parm_b (ppi, "warn_parm_not_defined", &L_warn_parm_not_defined);
    L_read_parm_b (ppi, "?warn_dev_parm_not_defined",
                   &L_warn_dev_parm_not_defined);
    L_read_parm_b (ppi, "warn_parm_defined_twice", &L_warn_parm_defined_twice);
    L_read_parm_b (ppi, "warn_parm_not_used", &L_warn_parm_not_used);
    L_read_parm_b (ppi, "dump_parms", &L_dump_parms);
    L_read_parm_s (ppi, "parm_warn_file_name", &L_parm_warn_file_name);
    L_read_parm_s (ppi, "parm_dump_file_name", &L_parm_dump_file_name);

    /* true elcor parms */
    L_read_parm_i(ppi, "nice_value", &El_nice_value);
    L_read_parm_b(ppi, "check_lcode", &El_check_lcode);
    L_read_parm_b(ppi, "show_warnings", &El_show_warnings);
    L_read_parm_b(ppi, "coredump_failed_assert", &El_coredump_failed_assert);
    L_read_parm_b(ppi, "coredump_on_punt", &El_coredump_on_punt);
}

void El_set_memvr_setup_mode(void)
{
    if (! strcasecmp(El_memvr_setup_mode_name, "seq"))
	El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_SEQUENTIAL;
    else if (! strcasecmp(El_memvr_setup_mode_name, "sync"))
	El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_FULL_LCODE_SYNC_ARCS;
    else if (! strcasecmp(El_memvr_setup_mode_name, "njsync"))
	El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_NO_JSR_LCODE_SYNC_ARCS;
    else if (! strcasecmp(El_memvr_setup_mode_name, "auto"))
	El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_AUTOMATIC;
    else if (! strcasecmp(El_memvr_setup_mode_name, "none"))
	El_memvr_setup_mode = EL_MEMVR_SETUP_MODE_NONE;
    else
	El_punt("El_set_io_formats: unknown memvr setup mode %s", El_output_format_name);
}

void El_set_io_formats(void)
{
    if (! strcasecmp(El_input_format_name, "none")) {
	El_warn("El_set_io_formats: no input format selected");
	El_input_format = EL_IO_NONE;
    }
    else if (! strcasecmp(El_input_format_name, "lcode")) {
	El_input_format = EL_IO_LCODE;
    }
    else if (! strcasecmp(El_input_format_name, "rebel")) {
	El_input_format = EL_IO_REBEL;
    }
    else if (! strcasecmp(El_input_format_name, "auto")) {
	El_input_format = EL_IO_AUTODETECT;
    }
    else {
	El_punt("El_set_io_formats: unknown input format %s", El_input_format_name);
    }

    if (! strcasecmp(El_output_format_name, "none")) {
	El_warn("El_set_io_formats: no output format selected");
	El_output_format = EL_IO_NONE;
    }
    else if (! strcasecmp(El_output_format_name, "lcode")) {
	El_output_format = EL_IO_LCODE;
    }
    else if (! strcasecmp(El_output_format_name, "rebel")) {
	El_output_format = EL_IO_REBEL;
    }
    else {
	El_punt("El_set_io_formats: unknown ouput format %s", El_output_format_name);
    }
}

FILE *El_open_output_file(char *name)
{
    FILE *F = 0;

    if (! strcmp(name, "stdout"))  {
        return stdout;
    }
    if (! strcmp(name, "stderr"))  {
        return stderr;
    }

    F = fopen(name, "w");
    if (F==0)
        El_punt("El_open_output_file: cannot open output file");

    return(F);
}

void El_close_output_file(FILE *F)
{
    if (F==NULL)
        El_punt("L_close_output_file: trying to close NULL file");

    if ((F==stdout) || (F==stderr))
        return;
    fclose(F);
    F = NULL;
}


/////////////////////////////////////////////////////////////////

MDES* default_mdes = NULL;


// Initialization is broken into two parts: (Shail Aditya 02/23/98)
// 1. initializing all the external parameters
// 2. initializing elcor internal data structures based on external parameters

void El_init_all(Parm_Macro_List *command_line_macro_list)
{
  El_init_parms(command_line_macro_list);
  El_init_elcor();
}

void El_init_parms(Parm_Macro_List *command_line_macro_list)
{
    /* Module specific initializations */
    El_anal_init(command_line_macro_list);
    El_control_init(command_line_macro_list);
    El_data_cpr_init(command_line_macro_list);
    El_debug_init(command_line_macro_list);
    El_driver_init(command_line_macro_list);
    El_stats_init(command_line_macro_list);
    El_lreg_init(command_line_macro_list);
    El_lsched_init(command_line_macro_list);
    El_sreg_init(command_line_macro_list);
    El_ssched_init(command_line_macro_list);
    El_opti_init(command_line_macro_list);
    El_cpr_init(command_line_macro_list);
    El_cluster_init(command_line_macro_list);
    El_custom_op_init(command_line_macro_list);
    El_cache_init(command_line_macro_list);
    El_codegen_init(command_line_macro_list);
    El_vectorize_init(command_line_macro_list);
}

void El_init_elcor(void)
{
    /*
     *	Map initialization
     */
    el_init_elcor_opcode_maps();

    el_init_elcor_macro_maps();

    /*
     *	Mdes initialization
     */
    default_mdes = new MDES(El_lmdes_file_name, El_mdes_debug_filename);

    set_current_MDES(default_mdes);

    // emu-map (temporary!)
    //    char filename[] = "f_0.rb";
    //    test_emu_map = new Emulation_map(ADD_W, filename);
}

// De-Initialization is broken into two parts: (Shail Aditya 12/17/2001)
// 1. de-initializing all the external parameters
// 2. de-initializing elcor internal data structures based on external parameters

void El_deinit_all(void)
{
  El_deinit_parms();
  El_deinit_elcor();
}

void El_deinit_parms(void)
{
    /*
     *	Module specific de-initializations
     */
    El_anal_deinit();
    El_codegen_deinit();
    El_control_deinit();
    El_cpr_deinit();
    El_data_cpr_deinit();
    El_debug_deinit();
    El_driver_deinit();
    El_stats_deinit();
    El_lreg_deinit();
    El_lsched_deinit();
    El_sreg_deinit();
    El_ssched_deinit();
    El_opti_deinit();
    El_cluster_deinit();
    El_custom_op_deinit();
    El_cache_deinit();
}

void El_deinit_elcor(void)
{
    el_deinit_elcor_opcode_maps();

#ifdef DELETE_ALL
    delete default_mdes; default_mdes = NULL;
#endif
}

