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
//      File:           el_debug_init.cpp
//      Author:         Scott Mahlke, Sadun Anik
//      Created:        June 1995
//      Description:    Initialization routine for debugging
//
/////////////////////////////////////////////////////////////////////////////

#include "el_debug_init.h"
#include "el_io.h"
#include "dbg.h"

//
//	Debugging parameters
//

char *El_mdes_debug_filename = NULL;
int El_print_vcg_graph = 0;
char *El_vcg_graph_filename = NULL;
int El_print_dot_graph = 0;
int El_dot_pseudo_ops = 0;
int El_dot_brf_edges = 0;
int El_dot_c0 = 0;
int El_dot_c1 = 0;
int El_dot_mem = 0;
int El_dot_flow = 0;
int El_dot_anti = 0;
int El_dot_out = 0;
int El_dot_verbose = 0;
int El_dot_schedop_labels = 0;
char *El_dot_graph_filename = NULL;
char *El_dot_graph_size = NULL;
char *El_dot_graph_orientation = NULL;
int El_dbg_block_id = -1;

//
//	Read in debugging parameters
//

void El_debug_read_parm(Parm_Parse_Info *ppi)
{
    L_read_parm_s(ppi, "mdes_debug_filename", &El_mdes_debug_filename);
    L_read_parm_b(ppi, "print_vcg_graph", &El_print_vcg_graph);
    L_read_parm_s(ppi, "vcg_graph_filename", &El_vcg_graph_filename);
    L_read_parm_b(ppi, "print_dot_graph", &El_print_dot_graph);
    L_read_parm_b(ppi, "dot_c0", &El_dot_c0);
    L_read_parm_b(ppi, "dot_c1", &El_dot_c1);
    L_read_parm_b(ppi, "dot_mem", &El_dot_mem);
    L_read_parm_b(ppi, "dot_flow", &El_dot_flow);
    L_read_parm_b(ppi, "dot_anti", &El_dot_anti);
    L_read_parm_b(ppi, "dot_out", &El_dot_out);
    L_read_parm_b(ppi, "dot_verbose", &El_dot_verbose);
    L_read_parm_b(ppi, "dot_pseudo_ops", &El_dot_pseudo_ops);
    L_read_parm_b(ppi, "dot_pseudo_ops", &El_dot_brf_edges);
    L_read_parm_b(ppi, "?dot_schedop_labels", &El_dot_schedop_labels);
    L_read_parm_s(ppi, "dot_graph_filename", &El_dot_graph_filename);
    L_read_parm_s(ppi, "dot_graph_size", &El_dot_graph_size);
    L_read_parm_s(ppi, "dot_graph_orientation", &El_dot_graph_orientation);
    L_read_parm_i(ppi, "dbg_block_id", &El_dbg_block_id);

    // Scribble knobs are also available externally

    int val;
    L_read_parm_i(ppi, "dbg_pqs", &val);   dbg_set(pqs,val);
    L_read_parm_i(ppi, "dbg_dfa", &val);   dbg_set(dfa,val);
    L_read_parm_i(ppi, "dbg_cda", &val);   dbg_set(cda,val);
    L_read_parm_i(ppi, "dbg_sed", &val);   dbg_set(sed,val);
    L_read_parm_i(ppi, "dbg_ms", &val);    dbg_set(ms,val);
    L_read_parm_i(ppi, "dbg_ra", &val);    dbg_set(ra,val);
    L_read_parm_i(ppi, "dbg_ss", &val);    dbg_set(ss,val);
    L_read_parm_i(ppi, "dbg_sa", &val);    dbg_set(sa,val);
    L_read_parm_i(ppi, "dbg_glu", &val);   dbg_set(glu,val);
    L_read_parm_i(ppi, "dbg_mdes", &val);  dbg_set(mdes,val);
    L_read_parm_i(ppi, "dbg_status", &val); dbg_set(status,val);
    L_read_parm_i(ppi, "dbg_tree", &val);   dbg_set(tree,val);
    L_read_parm_i(ppi, "dbg_rdef", &val);   dbg_set(rdef,val);
    L_read_parm_i(ppi, "dbg_tdup", &val);   dbg_set(tdup,val);
    L_read_parm_i(ppi, "dbg_opti", &val);   dbg_set(opti,val);
    L_read_parm_i(ppi, "dbg_cpr", &val);   dbg_set(cpr,val);
    L_read_parm_i(ppi, "dbg_dcpr", &val);   dbg_set(dcpr,val);  
    L_read_parm_i(ppi, "dbg_cpr", &val);     dbg_set(cpr,val);
    L_read_parm_i(ppi, "dbg_genlit", &val);  dbg_set(genlit,val);
    L_read_parm_i(ppi, "dbg_genrio", &val);    dbg_set(genrio,val);
    L_read_parm_i(ppi, "dbg_genrwin", &val);    dbg_set(genrwin,val);
    L_read_parm_i(ppi, "dbg_clst", &val);    dbg_set(clst,val);
    L_read_parm_i(ppi, "dbg_stack", &val);    dbg_set(stack,val);
    L_read_parm_i(ppi, "dbg_rhop", &val);    dbg_set(rhop,val);
}

//
//	Initializations for debugging
//

void El_debug_init(Parm_Macro_List *command_line_macro_list)
{
    L_load_parameters(El_parm_file_name, command_line_macro_list,
			"(Elcor_Debug", El_debug_read_parm);
}


//
//      Cleanup for debugging
//

void El_debug_deinit(void)
{
}


