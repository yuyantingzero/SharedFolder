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
//      File:           el_cpr_init.cpp
//      Author:         Scott Mahlke
//      Created:        February 1997
//      Description:    Initialization routine for the cpr xforms
//
/////////////////////////////////////////////////////////////////////////////

#include "el_cpr_init.h"
#include "el_io.h"


//
//	Parameters of the cpr routines
//

char *El_cpr_strategy_name = NULL;
El_Cpr_Strategy El_cpr_strategy = EL_CPR_STRATEGY_UNDEFINED;

int El_do_frpize = 0;
int El_predicate_speculation = 0;
int El_do_find_chains = 0;
int El_do_prune_cpr_blocks = 0;
int El_do_bypass_branch = 0;
int El_do_off_trace_motion = 0;

char *El_cpr_blocking_meth_name = NULL;
El_Cpr_Blocking_Method El_cpr_blocking_meth = EL_CPR_BLOCKING_NONE;
double El_cpr_max_exit_ratio = 0.0;
double El_cpr_predict_taken_ratio = 0.0;
double El_cpr_predict_break_ratio = 0.0;
double El_cpr_predict_pauseb_ratio = 0.0;
double El_cpr_predict_pausea_ratio = 0.0;
int El_cpr_max_block_length = 0;
int El_cpr_min_block_length = 0;
double El_min_cpr_weight = 0.0;

//
//	Read in cpr parameters
//

void El_cpr_read_parm(Parm_Parse_Info *ppi)
{
  L_read_parm_s(ppi, "cpr_strategy", &El_cpr_strategy_name);

  L_read_parm_b(ppi, "do_frpize", &El_do_frpize);
  L_read_parm_b(ppi, "predicate_speculation", &El_predicate_speculation);
  L_read_parm_b(ppi, "do_find_chains", &El_do_find_chains);
  L_read_parm_b(ppi, "do_prune_cpr_blocks", &El_do_prune_cpr_blocks);
  L_read_parm_b(ppi, "do_bypass_branch", &El_do_bypass_branch);
  L_read_parm_b(ppi, "do_off_trace_motion", &El_do_off_trace_motion);

  L_read_parm_s(ppi, "blocking_method", &El_cpr_blocking_meth_name);
  L_read_parm_lf(ppi, "max_exit_ratio", &El_cpr_max_exit_ratio);
  L_read_parm_lf(ppi, "predict_taken_ratio", &El_cpr_predict_taken_ratio);
  L_read_parm_lf(ppi, "predict_break_ratio", &El_cpr_predict_break_ratio);
  L_read_parm_lf(ppi, "predict_pauseb_ratio", &El_cpr_predict_pauseb_ratio);
  L_read_parm_lf(ppi, "predict_pausea_ratio", &El_cpr_predict_pausea_ratio);
  L_read_parm_i(ppi, "max_block_length", &El_cpr_max_block_length);
  L_read_parm_i(ppi, "min_block_length", &El_cpr_min_block_length);
  L_read_parm_lf(ppi, "min_cpr_weight", &El_min_cpr_weight);

  if (El_cpr_strategy_name == NULL)
    El_cpr_strategy = EL_CPR_STRATEGY_UNDEFINED;
  else if (!strcasecmp(El_cpr_strategy_name, "original"))
    El_cpr_strategy = EL_CPR_STRATEGY_ORIGINAL;
  else if (!strcasecmp(El_cpr_strategy_name, "max_reduce"))
    El_cpr_strategy = EL_CPR_STRATEGY_MAX_REDUCE;
  else
    El_punt("El_cpr_read_parm: unknown cpr strategy: %s",
		El_cpr_strategy_name);

  if (El_cpr_blocking_meth_name == NULL)
    El_cpr_blocking_meth = EL_CPR_BLOCKING_NONE;
  else if (! strcasecmp(El_cpr_blocking_meth_name, "none"))
    El_cpr_blocking_meth = EL_CPR_BLOCKING_NONE;
  else if (! strcasecmp(El_cpr_blocking_meth_name, "constant"))
    El_cpr_blocking_meth = EL_CPR_BLOCKING_CONSTANT;
  else if (! strcasecmp(El_cpr_blocking_meth_name, "heuristic"))
    El_cpr_blocking_meth = EL_CPR_BLOCKING_HEURISTIC;
  else
    El_punt("El_cpr_read_parm: unknown blocking method: %s",
		El_cpr_blocking_meth_name);

  // Hard code some of the parameters based on the blocking method 
  // Values of 1.1 essentially disable the parms from taking effect
  if (El_cpr_blocking_meth == EL_CPR_BLOCKING_NONE) {
    El_cpr_max_exit_ratio = 1.1;
    El_cpr_predict_taken_ratio = 1.1;
    El_cpr_predict_break_ratio = 1.1;
    El_cpr_predict_pauseb_ratio = 1.1;
    El_cpr_predict_pausea_ratio = 1.1;
    El_cpr_max_block_length = -1;
  }
  else if (El_cpr_blocking_meth == EL_CPR_BLOCKING_CONSTANT) {
    // leave predict taken enable w/ const blocking
    El_cpr_max_exit_ratio = 1.1;
    El_cpr_predict_break_ratio = 1.1;
    El_cpr_predict_pauseb_ratio = 1.1;
    El_cpr_predict_pausea_ratio = 1.1;
  }
}


//
//	Initializations for the cpr routines
//

void El_cpr_init(Parm_Macro_List *command_line_macro_list)
{
    L_load_parameters(El_parm_file_name, command_line_macro_list,
			"(Elcor_CPR", El_cpr_read_parm);
}


//
//      Cleanup for the cpr routines
//

void El_cpr_deinit(void)
{
}
