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
//      File:           el_io.h
//      Author:         Scott Mahlke
//      Created:        September 1995
//      Description:    declarations for I/O handling in Elcor
//
/////////////////////////////////////////////////////////////////////////////

#ifndef EL_IO_H
#define EL_IO_H

#include <stdio.h>
#include "l_parms.h"	/* Lcode parameter library */
#include "IR_instream.h"
#include "IR_outstream.h"


/*
 *	Elcor input/output format types
 */
enum El_IO_Format {
    EL_IO_NONE=0,		/* no output, except possibly debugging output */
    EL_IO_LCODE,		/* Lcode (this is default for both i/o) */
    EL_IO_REBEL,		/* Region Based External Language */
    EL_IO_AUTODETECT		/* Try to autodetect the input format */
};


/*
 *	High level Elcor input/output token types
 */
enum El_Input_Token {
    EL_INPUT_EOF=0,
    EL_INPUT_DATA,
    EL_INPUT_CODE
};


/*
 *	High level Elcor input memory dependence setup modes
 */

enum El_Memvr_Setup_Mode {
    EL_MEMVR_SETUP_MODE_AUTOMATIC=0,
    EL_MEMVR_SETUP_MODE_SEQUENTIAL,
    EL_MEMVR_SETUP_MODE_FULL_LCODE_SYNC_ARCS,
    EL_MEMVR_SETUP_MODE_NO_JSR_LCODE_SYNC_ARCS,
    EL_MEMVR_SETUP_MODE_NONE
};


/*
 *	Global variables/parameters for all of elcor
 */

extern char *El_input_file_name;
extern char *El_input_format_name;
extern El_IO_Format El_input_format;

extern char *El_output_file_name;
extern char *El_output_format_name;
extern El_IO_Format El_output_format;
extern int El_indent_width;
extern int El_output_max_line_width;
extern int El_print_verbose_header;
extern char *El_memvr_setup_mode_name;
extern El_Memvr_Setup_Mode El_memvr_setup_mode;
extern int El_punt_on_unknown_op;

extern char *El_parm_file_name;
extern char *El_lmdes_file_name;
extern char *El_lmdes_output_file_name;

extern char* El_patterns_file_name;                // patterns specification file name

extern Parm_Macro_List *EL_command_line_macro_list;
extern char *El_stat_file_name;
extern char *El_time_file_name;
extern char *El_opcode_file_name;
extern char *El_resource_file_name;           
extern char *El_crit_path_file_name;           
extern char *El_branch_file_name;

extern FILE *EL_IN;
extern FILE *EL_OUT;

extern IR_instream *EL_IN_STREAM;
extern IR_outstream *EL_OUT_STREAM;
extern ofstream *EL_STAT_STREAM_PTR;
extern ofstream *EL_TIME_STREAM_PTR;
extern ofstream *EL_OPCODE_STREAM_PTR;
extern ofstream *EL_RESOURCE_STREAM_PTR;
extern ofstream *EL_CRIT_PATH_STREAM_PTR;
extern ofstream *EL_BRANCH_STREAM_PTR;

// SAM 8-01.  These used to be _IO_ostream_withassign, changed for gcc 3.0
extern ostream *EL_STAT_STREAM;
extern ostream *EL_OPCODE_STREAM;
extern ostream *EL_RESOURCE_STREAM;
extern ostream *EL_CRIT_PATH_STREAM;
extern ostream *EL_BRANCH_STREAM;

extern void *El_input;
extern El_Input_Token El_input_type;

extern char *El_arch;
extern char *El_model;
extern int El_arch_speculation_model;
		// GENERAL_SPECULATION = 0;
		// RESTRICTED_SPECULATION = 1 (provable non-excepting ops)

extern int El_nice_value;
extern int El_check_lcode;

/* Use mem profiled info for refining sync vars, if parameter is non-zero */
extern int El_memvr_profiled;

#endif
