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
// File:     simu_init.cpp
// Author:   Amit Nene, Rodric M. Rabbah
///////////////////////////////////////////////////////////////////////////////


#include "opcode.h"
#include "el_io.h"
#include "operand.h"
#include "mdes.h"
#include "simu.h"


// simulation and trace generation related variables
int S_nual_equals            = 0;
int S_unscheduled            = 0;
int S_virtual_regs           = 0;
int S_pipeline_stages        = 6;
int S_interlocking           = 0;
int S_bytes_per_op           = 4;
int S_performance_monitoring = 0;
int S_binary_trace_format    = 0;
int S_control_flow_trace     = 0;
int S_per_op_value_trace     = 0;
char *S_trace_function       = 0;
int S_address_trace          = 0;
int S_dynamic_stats          = 0;
int S_do_mem_simulation      = 1;
// code generation flags
int S_remap_optables         = 0;
/* WWF { wrap around standard heap management */
int S_use_custom_malloc      = 0;
/* } WWF */
int S_show_warnings          = 0;
int S_show_log_messages      = 1;
int S_generate_assembly      = 0;
int S_read_bb_flagme_file    = 0;
int S_read_op_flagme_file    = 0;
// stack manipulation flags
int S_stack_growth               = 0;
int S_stack_manipulated_in_elcor = 0;
int S_return_value_thru_stack    = 0;
int S_return_value_offset        = 8;

static MDES* default_mdes;


void init_simulator_parms(void)
{
  el_init_elcor_macro_maps();
  //  Mdes initialization
  default_mdes = new MDES(El_lmdes_file_name, El_lmdes_output_file_name);
  set_current_MDES(default_mdes);
  
  //  Map initialization
  el_init_elcor_opcode_maps();
}


void readParmSimu(Parm_Parse_Info *ppi)
{
  char *dir = "high_to_low";
  // trace generation control
  L_read_parm_b (ppi, "nual_equals",            &S_nual_equals);
  L_read_parm_b (ppi, "emulate_unscheduled",    &S_unscheduled);
  L_read_parm_b (ppi, "emulate_virtual_regs",   &S_virtual_regs);
  L_read_parm_i (ppi, "pipeline_stages",        &S_pipeline_stages);
  L_read_parm_b (ppi, "interlocking",           &S_interlocking);
  L_read_parm_i (ppi, "bytes_per_op",           &S_bytes_per_op);
  L_read_parm_b (ppi, "do_memory_simulation",   &S_do_mem_simulation);
  L_read_parm_b (ppi, "performance_monitoring", &S_performance_monitoring);
  L_read_parm_b (ppi, "binary_trace_format",    &S_binary_trace_format);
  L_read_parm_b (ppi, "control_flow_trace",     &S_control_flow_trace);
  L_read_parm_s (ppi, "trace_function_name",    &S_trace_function);
  L_read_parm_b (ppi, "per_op_value_trace",     &S_per_op_value_trace);
  L_read_parm_b (ppi, "address_trace",          &S_address_trace);
  L_read_parm_b (ppi, "dynamic_stats",          &S_dynamic_stats);
  L_read_parm_b (ppi, "remap_optables",         &S_remap_optables);
  /* WWF { wrap around standard heap management */
  L_read_parm_b (ppi, "use_custom_malloc",      &S_use_custom_malloc);
  /* } WWF */
  L_read_parm_b (ppi, "show_codegen_warnings",  &S_show_warnings);
  L_read_parm_b (ppi, "show_codegen_logs",      &S_show_log_messages);
  L_read_parm_b (ppi, "generate_assembly",      &S_generate_assembly);
  L_read_parm_b (ppi, "read_bb_flagme_file",    &S_read_bb_flagme_file);
  L_read_parm_b (ppi, "read_op_flagme_file",    &S_read_op_flagme_file);
  L_read_parm_s (ppi, "stack_growth",           &dir);
  L_read_parm_b (ppi, "return_value_thru_stack", &S_return_value_thru_stack);
  L_read_parm_b (ppi, "stack_manipulated_in_elcor", &S_stack_manipulated_in_elcor);
  L_read_parm_i (ppi, "return_value_offset", &S_return_value_offset);

  if (!strcmp(dir, "high_to_low"))
    S_stack_growth = MEM_HIGH_TO_LOW;
  else if (!strcmp(dir, "low_to_high"))
    S_stack_growth = MEM_LOW_TO_HIGH;
  else
    El_punt("readParmSimu: stack growth has to be either 'high_to_low' or 'low_to_high'");

}
