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
//      File:           el_stats_init.cpp
//      Author:         Scott Mahlke
//      Created:        July 1996
//      Description:    Initialization routine for stats generation
//
/////////////////////////////////////////////////////////////////////////////

#include "el_stats_init.h"
#include "el_io.h"
#include "el_clock.h"
#include "el_opcode_usage.h"

//
//	Elcor stats parameters
//

int El_do_cycle_count = 0;
int El_do_block_cycle_count = 0;
int El_do_cpr_cycle_count = 0;
int El_do_op_count = 0;
int El_block_types = 0;
int El_do_opcode_usage = 0;
int El_do_lit_histogram = 0;
int El_do_regalloc_overhead = 0;
int El_do_regalloc_file_overhead = 0;
int El_do_run_time = 0;
int El_do_block_run_time = 0;
int El_do_wgt_crit_path = 0;
int El_branch_mispredict_penalty = 0;
int El_do_cpr_blk_info = 0;
int El_do_resource = 0;
int El_do_crit_path = 0;
int El_do_branch_histogram = 0;
int El_br_hist_num_categories = 0;
int El_br_hist_include_bru = 0;
int El_br_hist_fold_over = 0;

//
//	Read in driver parameters
//

void El_stats_read_parm(Parm_Parse_Info *ppi)
{
    L_read_parm_b(ppi, "do_cycle_count", &El_do_cycle_count);
    L_read_parm_b(ppi, "do_block_cycle_count", &El_do_block_cycle_count);
    L_read_parm_b(ppi, "do_cpr_cycle_count", &El_do_cpr_cycle_count);
    L_read_parm_b(ppi, "do_op_count", &El_do_op_count);
    L_read_parm_b(ppi, "show_block_types", &El_block_types);
    L_read_parm_b(ppi, "do_opcode_usage", &El_do_opcode_usage);
    L_read_parm_b(ppi, "do_lit_histogram", &El_do_lit_histogram);
    L_read_parm_b(ppi, "do_regalloc_overhead", &El_do_regalloc_overhead);
    L_read_parm_b(ppi, "do_regalloc_file_overhead", &El_do_regalloc_file_overhead);
    L_read_parm_b(ppi, "do_run_time", &El_do_run_time);
    L_read_parm_b(ppi, "do_wgt_crit_path", &El_do_wgt_crit_path);
    L_read_parm_b(ppi, "do_block_run_time", &El_do_block_run_time);
    L_read_parm_b(ppi, "do_cpr_blk_info", &El_do_cpr_blk_info);

    L_read_parm_b(ppi, "do_resource", &El_do_resource);
    L_read_parm_b(ppi, "do_crit_path", &El_do_crit_path);

    L_read_parm_b(ppi, "do_branch_histogram", &El_do_branch_histogram);
    L_read_parm_i(ppi, "br_hist_num_categories", &El_br_hist_num_categories);
    L_read_parm_b(ppi, "br_hist_include_bru", &El_br_hist_include_bru);
    L_read_parm_b(ppi, "br_hist_fold_over", &El_br_hist_fold_over);

    L_read_parm_i(ppi, "branch_mispredict_penalty", &El_branch_mispredict_penalty);

    // block cycle counting implies global cycle counting.
    if (El_do_block_cycle_count)
      El_do_cycle_count = 1;

    if (El_do_regalloc_file_overhead)
	El_do_regalloc_overhead = 1;

    if (El_do_regalloc_overhead && !El_do_op_count)
	El_do_op_count = 1;

    // Check sanity of branch histogram parms
    if (El_do_branch_histogram) {
	// El_br_hist_num_categories must divide 100 evenly
	if ((100 % El_br_hist_num_categories) != 0)
	    El_punt("El_stats_read_parm: br_hist_num_categories must divide 100 evenly");

	// if El_br_hist_fold_over is True, El_br_hist_num_categories must be even
	if ((El_br_hist_fold_over) && ((El_br_hist_num_categories % 2) != 0))
	    El_punt("El_stats_read_parm: br_hist_num_categories must be even");
    }
}


//
//	Open/close stream for printing out cycle counts
//

void El_open_stat_stream(void)
{
    if (El_do_cycle_count || El_do_block_cycle_count || El_do_op_count ||
		El_block_types || El_do_regalloc_overhead ||
		El_do_wgt_crit_path || El_do_cpr_blk_info) {
	if (! strcmp(El_stat_file_name, "cout")) {
            EL_STAT_STREAM_PTR = NULL;
            EL_STAT_STREAM = &cout;
        }
        else if (! strcmp(El_stat_file_name, "cerr")) {
            EL_STAT_STREAM_PTR = NULL;
            EL_STAT_STREAM = &cerr;
        }
        else if (! strcmp(El_stat_file_name, "clog")) {
            EL_STAT_STREAM_PTR = NULL;
            EL_STAT_STREAM = &clog;
        }
	else {
	    EL_STAT_STREAM_PTR = new ofstream(El_stat_file_name, ios::app);
	    EL_STAT_STREAM = EL_STAT_STREAM_PTR;
	}

	EL_STAT_STREAM->setf(ios::fixed);
	EL_STAT_STREAM->precision(2);
    }
}

void El_close_stat_stream(void)
{
    EL_STAT_STREAM = NULL;
    if (EL_STAT_STREAM_PTR!=NULL) {
	delete EL_STAT_STREAM_PTR;
    }
}


//
//      Open/close stream for printing out run time info
//

void El_open_time_stream(void)
{
    if (El_do_run_time || El_do_block_run_time) {
        if (! strcmp(El_time_file_name, "cout")) {
            EL_TIME_STREAM_PTR = NULL;
            EL_TIME_STREAM = &cout;
        }
        else if (! strcmp(El_time_file_name, "cerr")) {
            EL_TIME_STREAM_PTR = NULL;
            EL_TIME_STREAM = &cerr;
        }
        else if (! strcmp(El_time_file_name, "clog")) {
            EL_TIME_STREAM_PTR = NULL;
            EL_TIME_STREAM = &clog;
        }
        else {
            EL_TIME_STREAM_PTR = new ofstream(El_time_file_name, ios::app);
            EL_TIME_STREAM = EL_TIME_STREAM_PTR;
        }

        EL_TIME_STREAM->setf(ios::fixed);
        EL_TIME_STREAM->precision(2);
    }
    else {
       EL_TIME_STREAM = &cerr ;
    }
}

void El_close_time_stream(void)
{
    EL_TIME_STREAM = NULL;
    if (EL_TIME_STREAM_PTR!=NULL) {
        delete EL_TIME_STREAM_PTR;
    }
}


//
//      Open/close stream for printing out opcode info
//

void El_open_opcode_stream(void)
{
    if (El_do_opcode_usage) {
        if (! strcmp(El_opcode_file_name, "cout")) {
            EL_OPCODE_STREAM_PTR = NULL;
            EL_OPCODE_STREAM = &cout;
        }
        else if (! strcmp(El_opcode_file_name, "cerr")) {
            EL_OPCODE_STREAM_PTR = NULL;
            EL_OPCODE_STREAM = &cerr;
        }
        else if (! strcmp(El_opcode_file_name, "clog")) {
            EL_OPCODE_STREAM_PTR = NULL;
            EL_OPCODE_STREAM = &clog;
        }
        else {
            EL_OPCODE_STREAM_PTR = new ofstream(El_opcode_file_name, ios::out);
            EL_OPCODE_STREAM = EL_OPCODE_STREAM_PTR;
        }
    }
    else {
       EL_OPCODE_STREAM = &cerr ;
    }
}

void El_close_opcode_stream(void)
{
    EL_OPCODE_STREAM = NULL;
    if (EL_OPCODE_STREAM_PTR!=NULL) {
        delete EL_OPCODE_STREAM_PTR;
    }
}

//      suren::
//      Open/close stream for printing out ELCOR_RESOURCE
//      data (exitid, freq, critical-path-len ...)

void El_open_resource_stream(void)
{
    if (El_do_resource) {
	if (! strcmp(El_resource_file_name, "cout")) {
            EL_RESOURCE_STREAM_PTR = NULL;
            EL_RESOURCE_STREAM = &cout;
        }
        else if (! strcmp(El_resource_file_name, "cerr")) {
            EL_RESOURCE_STREAM_PTR = NULL;
            EL_RESOURCE_STREAM = &cerr;
        }
        else if (! strcmp(El_resource_file_name, "clog")) {
            EL_RESOURCE_STREAM_PTR = NULL;
            EL_RESOURCE_STREAM = &clog;
        }
	else {
	    EL_RESOURCE_STREAM_PTR = new ofstream(El_resource_file_name, ios::app);
	    EL_RESOURCE_STREAM = EL_RESOURCE_STREAM_PTR;
	}

    }
    else {
       EL_RESOURCE_STREAM = &cerr ;
    }
}

void El_close_resource_stream(void)
{
    EL_RESOURCE_STREAM = NULL;
    if (EL_RESOURCE_STREAM_PTR!=NULL) {
        delete EL_RESOURCE_STREAM_PTR;
    }
}



//      suren::
//      Open/close stream for printing out ELCOR_CRIT_PATH

void El_open_crit_path_stream(void)
{
    if (El_do_crit_path) {
	if (! strcmp(El_crit_path_file_name, "cout")) {
            EL_CRIT_PATH_STREAM_PTR = NULL;
            EL_CRIT_PATH_STREAM = &cout;
        }
        else if (! strcmp(El_crit_path_file_name, "cerr")) {
            EL_CRIT_PATH_STREAM_PTR = NULL;
            EL_CRIT_PATH_STREAM = &cerr;
        }
        else if (! strcmp(El_crit_path_file_name, "clog")) {
            EL_CRIT_PATH_STREAM_PTR = NULL;
            EL_CRIT_PATH_STREAM = &clog;
        }
	else {
	    EL_CRIT_PATH_STREAM_PTR = new ofstream(El_crit_path_file_name, ios::app);
	    EL_CRIT_PATH_STREAM = EL_CRIT_PATH_STREAM_PTR;
	}

    }
    else {
       EL_CRIT_PATH_STREAM = &cerr ;
    }
}

void El_close_crit_path_stream(void)
{
    EL_CRIT_PATH_STREAM = NULL;
    if (EL_CRIT_PATH_STREAM_PTR!=NULL) {
        delete EL_CRIT_PATH_STREAM_PTR;
    }
}

//
//      Open/close stream for printing out branch histogram
//

void El_open_branch_stream(void)
{
    if (El_do_branch_histogram) {
        if (! strcmp(El_branch_file_name, "cout")) {
            EL_BRANCH_STREAM_PTR = NULL;
            EL_BRANCH_STREAM = &cout;
        }
        else if (! strcmp(El_branch_file_name, "cerr")) {
            EL_BRANCH_STREAM_PTR = NULL;
            EL_BRANCH_STREAM = &cerr;
        }
        else if (! strcmp(El_branch_file_name, "clog")) {
            EL_BRANCH_STREAM_PTR = NULL;
            EL_BRANCH_STREAM = &clog;
        }
        else {
            EL_BRANCH_STREAM_PTR = new ofstream(El_branch_file_name, ios::out);
            EL_BRANCH_STREAM = EL_BRANCH_STREAM_PTR;
        }
    }
    else {
       EL_BRANCH_STREAM = &cerr ;
    }
}

void El_close_branch_stream(void)
{
    EL_BRANCH_STREAM = NULL;
    if (EL_BRANCH_STREAM_PTR!=NULL) {
        delete EL_BRANCH_STREAM_PTR;
    }
}


//
//	Initializations for driver
//

void El_stats_init(Parm_Macro_List *command_line_macro_list)
{
    L_load_parameters(El_parm_file_name, command_line_macro_list,
			"(Elcor_Stats", El_stats_read_parm);

    El_open_stat_stream();
    El_open_time_stream();
    El_open_opcode_stream();
    El_open_resource_stream();
    El_open_crit_path_stream();
    El_open_branch_stream();
}


//
//      Cleanup for driver
//

void El_stats_deinit(void)
{
    El_close_stat_stream();
    El_close_time_stream();
    El_close_opcode_stream();
    El_close_resource_stream();
    El_close_crit_path_stream();
    El_close_branch_stream();
}


