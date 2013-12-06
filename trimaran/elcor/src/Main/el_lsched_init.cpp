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
//      File:           el_lsched_init.cpp
//      Author:         Scott Mahlke, Sadun Anik, Shail Aditya
//      Created:        June 1995
//      Description:    Initialization routine for the loop scheduler
//
/////////////////////////////////////////////////////////////////////////////

#include "el_lsched_init.h"
#include "el_io.h"

//
//	Parameters of the loop scheduler
//

int El_lsched_hoist_pbr = 0;
int El_do_stage_scheduling = 0;
int El_lsched_priority_model = 0;
int El_swploop_size_threshold = USHRT_MAX;
int El_budget_ratio = 3;
int El_block = 0;
int El_ii = 0;
int El_bump_ii = 1;
MSched_algorithm El_msched_algorithm = ITERATIVE_MSCHED;
MSched_allocator El_msched_allocator = NO_MSCHED_ALLOCATION;

//
//	Read in loop scheduler parameters
//

void El_lsched_read_parm(Parm_Parse_Info *ppi)
{
    L_read_parm_b(ppi, "hoist_pbr", &El_lsched_hoist_pbr);
    L_read_parm_b(ppi, "do_stage_scheduling", &El_do_stage_scheduling);
    L_read_parm_i(ppi, "lsched_priority_model", &El_lsched_priority_model);
    L_read_parm_i(ppi, "budget_ratio", &El_budget_ratio);
    L_read_parm_i(ppi, "swploop_size_threshold", &El_swploop_size_threshold);

    L_read_parm_i(ppi, "block", &El_block);
    L_read_parm_i(ppi, "ii", &El_ii);
    L_read_parm_b(ppi, "bump_ii", &El_bump_ii);

    char* tmp = NULL;
    L_read_parm_s(ppi, "msched_algorithm", &tmp);
    if( ! strcasecmp("iterative", tmp))
      El_msched_algorithm = ITERATIVE_MSCHED;
    else if( ! strcasecmp("swing", tmp))
      El_msched_algorithm = SWING_MSCHED;
    else {
      cdbg << "ERROR: I don't know what type of modulo scheduling to do!\n"
	   << tmp << endl;
      El_punt("");
    }

    L_read_parm_s(ppi, "msched_allocator", &tmp);
    if( ! strcasecmp("rr", tmp))
      El_msched_allocator = ROTATING_REGISTERS;
    else if( ! strcasecmp("mve", tmp))
      El_msched_allocator = MODULO_VARIABLE_EXPANSION;
    else {
      cdbg << "ERROR: I don't know what type of register allocation to do in "
	   << "modulo scheduling!\n"
	   << tmp << endl;
      El_punt("");
    }

}


//
//	Initializations for the loop scheduler
//

void El_lsched_init(Parm_Macro_List *command_line_macro_list)
{
    L_load_parameters(El_parm_file_name, command_line_macro_list,
			"(Elcor_Loop_sched", El_lsched_read_parm);
}


//
//      Cleanup for the loop scheduler
//

void El_lsched_deinit(void)
{
}
