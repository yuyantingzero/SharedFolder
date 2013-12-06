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



#include "codegen.h"
#include "arm_convert.h"
#include "hplpd_convert.h"


// Top level driver for all machine specific changes that need to be
// initially made to the Rebel. The design philosophy here is that
// cerain transformations are difficult to do within the framework of
// MDES. By localizing those changes within this function (hopefully)
// we keep the rest of the compiler code cleaner and more
// straightforward.
void Codegen::MachineSpecificConversion(Procedure* f)
{
  if (!strcasecmp(El_arch, "hpl-pd"))
    ;
  else if (!strcasecmp(El_arch, "arm")) {
    mark_branch_targets(f);
    arm_replace_fp_params(f);
    arm_fix_op_formats(f);
  } else {
    El_punt("Codegen::MachineSpecificConversions(): Unknown arch: %s\n", El_arch);
  }

  OpConvert(f);
}


static List<Op*> NullOpConvert(Op *op)
{
  El_punt("Conversion unspecified for opc: %s", (char*)el_opcode_to_string_map.value(op->opcode()));
  return 0;
}

void Codegen::initialize_opc_table(Hash_map<Opcode, FuncImpl> **opcTable)
{
  (*opcTable) = new Hash_map<Opcode, FuncImpl>(hash_opcode, 1024);
  for (Hash_map_iterator<Opcode, eString> hiter(el_opcode_to_string_map); hiter!=0; hiter++) {
    Opcode opc = (*hiter).first;
    (*opcTable)->bind(opc, NullOpConvert);
  }
}

static void insert_op(Op *op, List<Op*>& oplist)
{
  for (List_iterator<Op*> liter(oplist); liter!=0; liter++) {
    Op *new_op = *liter;
    El_insert_op_before(new_op, op);
  }
}

void Codegen::OpConvert(Procedure *f)
{
  Hash_set<Op*> remove_ops(hash_op_ptr, 32);

  if (!strcasecmp(El_arch, "hpl-pd"))
    hplpd_opc_table_initialize(&opcTable);
  else if (!strcasecmp(El_arch, "arm")) {
    arm_opc_table_initialize(&opcTable);
  } else
   El_punt("Codegen::OpConvert(): Unknown arch: %s\n", El_arch);

  for (Region_all_ops opi(f); opi!=0; opi++) {
    Op *op = *opi;
    if (is_pseudo(op))
      continue;
    Opcode opc = op->opcode();
    if (MDES_unsupported(el_opcode_to_string_map.value(opc))) {
      assert (opcTable->is_bound(opc));
      List<Op*> oplist = (*opcTable->value(opc))(op);
      insert_op(op, oplist);
      remove_ops += op;
    }
  }

  for (Hash_set_iterator<Op*> opiter(remove_ops); opiter!=0; opiter++)
    El_remove_op(*opiter);

  if (dbg(genrio, 1)) {
    char buff[256];
    sprintf(buff, "%s_after_op_convert.reb", (char*)f->get_name());
    cdbg << "Printing Rebel after machine specific Op conversions" << endl;
    IR_outstream dbg_out(buff);
    ir_write(dbg_out, f);
  }
}


// This function is for fixing op formats that can't be handled by the
// MDES or Codegen. For example, in ARM a floating point store can not
// be base+displacement when the displacement is a register. When the
// displacement is a large literal Codegen doesn't understand that
// creating a move will be illegal.
void Codegen::fix_op_formats(Compound_region *f, bool prepass)
{
  if (!strcasecmp(El_arch, "hpl-pd"))
    ;
  else if (!strcasecmp(El_arch, "arm"))
    arm_fix_op_formats(f, prepass);
  else
   El_punt("Codegen::fix_op_formats(): Unknown arch: %s\n", El_arch);

}
