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
// File:     emitter.cpp
// Author:   Rodric M. Rabbah, Marc Saint
///////////////////////////////////////////////////////////////////////////////


#include "emitter.h"


extern int S_bytes_per_op;

static const char LEFT_BRACE  = '{';
static const char RIGHT_BRACE = '}';
//static const char COMMA       = ',';
static const char SEMICOLON   = ';';


inline char* mprint(const eString& str) 
{
  if (str != null_string) {
    return (char*) str;
  }
  else return "0x0";
}
#define fprint(str) "(void*) " << mprint(str)


int genmask(const OpEntry& op)
{
  int flags = op.type;

  if (op.type == PSEUDO_OP) return flags;

  switch (op.flags) {
  case CLRSAVE: flags |= CLRSAVE; break;
  case CLESAVE: flags |= CLESAVE; break;
  default:      flags |= XXX_OPS; break;
  }

  flags |= op.predicated  ? PREDICATED_OP : 0;
  flags |= op.speculative ? SPECULATED_OP : 0;
  flags |= op.mask        ? NOTRAPPING_OP : 0;
  return flags;
}

eString calcaddress(const OpEntry& op, const eString& tbl_name, int& real_op_count)
{
  char buffer[1024];
  eString address;

  if (op.type == PSEUDO_OP) {
    /* WWF - Oct 30  2003 - { even pseudo-op needs a  PC as it may be
     * the target of a branch and we want the branch predictor to work
     * correctly
     *
     * address = "0";
     */
    sprintf(buffer, "(unsigned long)(((ulong) %s__pd_%s) + %u)", (char*) tbl_name, (S_remap_optables ? "addrs" : "ops"), 
		(real_op_count * S_bytes_per_op));
    address = buffer;
    /* } - WWF - Oct 30 2003 */
  }
  else {
    sprintf(buffer, "(unsigned long)(((ulong) %s__pd_%s) + %u)", (char*) tbl_name, (S_remap_optables ? "addrs" : "ops"), 
		(real_op_count++ * S_bytes_per_op));
    address = buffer;
  }
  
  return address;
}

// Prints one table entry in the format of an opnode
// struct __PD_opnode is:
//   const ushort    rebel_id;
//   const ubyte     flags;
//   const uint      address;
//   const int       cid;
//   const __PD_impl op;                  
//   const __PD_port pred;                
//   const __PD_port src [__PD_MAX_SRC];  
//   const __PD_port dest[__PD_MAX_DEST]; 
//
// struct __PD_port is
//   __PD_reg* file;
//   uint      num;
//   uint      rot;
//   ubyte     lat | is_reg;
void print(const OpEntry& op, streamBuffer* out, const eString& tbl_name, int& real_op_count)
{
  static int i;
  
  /* open op brace */
  (* out) << TAB << LEFT_BRACE;

  /* op id and index with a block */
  (* out) << op.id << COMMA;
  (* out) << SPACE << op.index << COMMA;

  /* print type, and flags */
  (* out) << SPACE << genmask(op) << COMMA;

  /* (calculate and) print out the instruction address */
  (* out) << SPACE << calcaddress(op, tbl_name, real_op_count) << COMMA;

  /* print cluster id (cid) */
  (* out) << SPACE << op.cid << COMMA;

  /* trace this op? */
  (* out) << SPACE << op.gen_trace << COMMA;

  /* op implementation */
  (* out) << SPACE << mprint(op.op) << COMMA;

  /* predicate */
  (* out) << SPACE << LEFT_BRACE 
	    << (int) op.pred.is_reg << COMMA
	    << fprint(op.pred.file) << COMMA 
	    << op.pred.offset	    << COMMA 
	    << op.pred.num          << COMMA 
	    << op.pred.rot          << COMMA 
	    << op.pred.vec          << COMMA 
	    << 0 // no latency on predicate port
	    << RIGHT_BRACE  
	    << COMMA;

  /* sources */
  (* out) << SPACE << LEFT_BRACE;

  for (i = 0; i < MAX_SRC; i++) {
    (* out) << SPACE << LEFT_BRACE
		<< (int) op.src[i].is_reg << COMMA
		<< fprint(op.src[i].file) << COMMA
		<< op.src[i].offset	  << COMMA
		<< op.src[i].num          << COMMA
		<< op.src[i].rot          << COMMA
		<< op.src[i].vec          << COMMA
		<< 0 // no latency on source ports
		<< RIGHT_BRACE;
    
    if (i < MAX_SRC-1) (* out) << COMMA;
  }
  
  (* out) << SPACE << RIGHT_BRACE << COMMA;
  
  /* destinations */
  (* out) << SPACE << LEFT_BRACE;
  
  for (i = 0; i < MAX_DEST; i++) {
    (* out) << SPACE << LEFT_BRACE
		<< (int) op.dest[i].is_reg << COMMA
		<< fprint(op.dest[i].file) << COMMA
		<< op.dest[i].offset 	   << COMMA
		<< op.dest[i].num          << COMMA
		<< op.dest[i].rot          << COMMA
		<< op.dest[i].vec          << COMMA
		<< op.lat[i]
		<< RIGHT_BRACE;

    if (i < MAX_DEST-1) (* out) << COMMA;
  }
  
  (* out) << SPACE << RIGHT_BRACE;
  
  /* close op brace */
  (* out) << RIGHT_BRACE;
}

void print_table(const Vector<OpEntry*>& table, int count, streamBuffer* out)
{
  eString tbl_name = table[0]->attribute;

  if (S_remap_optables) {
    remapped_layout(table, count, out);
  }
  else {
    classic_layout(table, count, out);
  }
}

// Prints the entire contents of the current table
// to outf in the format of a an initializer of a global array of struct PD_OP
void classic_layout(const Vector<OpEntry*>& table, int count, streamBuffer* out)
{
  int i;
  int real_op_count = 0;

  eString tbl_name = table[0]->attribute;
  
  (* out) << endl << endl 
	    << "__PD_opnode " << tbl_name << "__pd_ops[" << count << "] " 
	    << EQUALS << endl;
  
  (* out) << LEFT_BRACE << endl;
  
  for (i = 0; i < count-1; i++) {
    (* out) << TAB << "/* op# " << i << " */" << endl;
    print(*table[i], out, tbl_name, real_op_count);
    
    (* out) << COMMA << endl << endl;
  }
  (* out) << TAB << "/* op# " << i << " */" << endl;
  print(*table[i], out, tbl_name, real_op_count);
  
  (* out) << endl << RIGHT_BRACE << SEMICOLON << endl << endl;
}

// Prints the entire contents of the current table
// to outf in the format of a an initializer of a global remapped array
// struct __PD_table is
//   const ushort*    rebel_id;
//   const ubyte*     flags;
//   const uint*      address;
//   const __PD_impl* op;
//   const __PD_port* pred;
//   const __PD_port* src[__PD_MAX_SRC];
//   const __PD_port* dest[__PD_MAX_DEST];
void remapped_layout(const Vector<OpEntry*>& table, int count, streamBuffer* out)
{
  int i;
  int real_op_count = 0;
  eString tbl_name  = table[0]->attribute;

  /* op id */
  (* out) << "ushort " << tbl_name << "__pd_id[" << count << "] " 
	    << EQUALS << SPACE << LEFT_BRACE;
  for (i = 0; i < count-1; i++) {
    (* out) << table[i]->id << COMMA << SPACE;
  }
  (* out) << table[i]->id;
  (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;

  /* op index within a block  */
  (* out) << "ushort " << tbl_name << "__pd_index[" << count << "] " 
	    << EQUALS << SPACE << LEFT_BRACE;
  for (i = 0; i < count-1; i++) {
    (* out) << table[i]->index << COMMA << SPACE;
  }
  (* out) << table[i]->index;
  (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;

  /* print type, and flags */
  (* out) << "ubyte " << tbl_name << "__pd_flags[" << count << "] " 
	    << EQUALS << SPACE << LEFT_BRACE;
  for (i = 0; i < count-1; i++) {
    (* out) << genmask(*table[i]) << COMMA << SPACE;
  }
  (* out) << genmask(*table[i]);
  (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;

  /* instruction address */
  (* out) << "uint " << tbl_name << "__pd_addrs[" << count << "] " 
	    << EQUALS << SPACE << LEFT_BRACE;
  for (i = 0; i < count-1; i++) {
    (* out) << calcaddress(*table[i], tbl_name, real_op_count) << COMMA << SPACE;
  }
  (* out) << calcaddress(*table[i], tbl_name, real_op_count);
  (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;

  /* op implementation */
  (* out) << "__PD_impl " << tbl_name << "__pd_op[" << count << "] " 
	    << EQUALS << SPACE << LEFT_BRACE;
  for (i = 0; i < count-1; i++) {
    (* out) << mprint(table[i]->op) << COMMA << SPACE;
  }
  (* out) << mprint(table[i]->op);
  (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;

  /* predicate */
  (* out) << "__PD_port " << tbl_name << "__pd_pred[" << count << "] " 
	    << EQUALS << SPACE << LEFT_BRACE;
  for (i = 0; i < count-1; i++) {
    (* out) << SPACE << LEFT_BRACE
		<< (int) table[i]->pred.is_reg << COMMA
		<< fprint(table[i]->pred.file) << COMMA
		<< table[i]->pred.num          << COMMA
		<< table[i]->pred.rot          << COMMA
		<< table[i]->pred.vec          << COMMA
		<< 0 // no latency on predicate port
		<< RIGHT_BRACE << COMMA;
  }
  (* out) << SPACE << LEFT_BRACE
	    << (int) table[i]->pred.is_reg << COMMA
	    << fprint(table[i]->pred.file) << COMMA
	    << table[i]->pred.num          << COMMA
	    << table[i]->pred.rot          << COMMA
	    << table[i]->pred.vec          << COMMA
	    << 0 // no latency on predicate port
	    << RIGHT_BRACE << SPACE;
  (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;
  

  /* sources */
  for (int si = 0; si < MAX_SRC; si++) {
    (* out) << "__PD_port " << tbl_name << "__pd_src" << si << "[" << count << "] " 
		<< EQUALS << SPACE << LEFT_BRACE;
    for (i = 0; i < count-1; i++) {
	(* out) << SPACE << LEFT_BRACE
		  << (int) table[i]->src[si].is_reg << COMMA
		  << fprint(table[i]->src[si].file) << COMMA
		  << table[i]->src[si].num          << COMMA
		  << table[i]->src[si].rot          << COMMA
		  << table[i]->src[si].vec          << COMMA
		  << 0 // no latency on predicate port
		  << RIGHT_BRACE << COMMA;
    }
    (* out) << SPACE << LEFT_BRACE
		<< (int) table[i]->src[si].is_reg << COMMA
		<< fprint(table[i]->src[si].file) << COMMA
		<< table[i]->src[si].num          << COMMA
		<< table[i]->src[si].rot          << COMMA
		<< table[i]->src[si].vec          << COMMA
		<< 0 // no latency on predicate port
		<< RIGHT_BRACE << SPACE;
    (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;
  }

  /* destinations */
  for (int si = 0; si < MAX_DEST; si++) {
    (* out) << "__PD_port " << tbl_name << "__pd_dest" << si << "[" << count << "] "
		<< EQUALS << SPACE << LEFT_BRACE;
    for (i = 0; i < count-1; i++) {
	(* out) << SPACE << LEFT_BRACE
		  << (int) table[i]->dest[si].is_reg << COMMA
		  << fprint(table[i]->dest[si].file) << COMMA
		  << table[i]->dest[si].num          << COMMA
		  << table[i]->dest[si].rot          << COMMA
		  << table[i]->dest[si].vec          << COMMA
		  << (int) table[i]->lat[si]
		  << RIGHT_BRACE << COMMA;
    }
    (* out) << SPACE << LEFT_BRACE
		<< (int) table[i]->dest[si].is_reg << COMMA
		<< fprint(table[i]->dest[si].file) << COMMA
		<< table[i]->dest[si].num          << COMMA
		<< table[i]->dest[si].rot          << COMMA
		<< table[i]->dest[si].vec          << COMMA
		<< (int) table[i]->lat[si]
		<< RIGHT_BRACE << SPACE;
    (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;
  }

  /* print operation table pointers */
  (* out) << "__PD_table " << tbl_name << "__pd_ops" << SPACE << EQUALS << SPACE
	    << LEFT_BRACE << endl;

  (* out) << TAB << tbl_name << "__pd_id"    << COMMA << endl;
  (* out) << TAB << tbl_name << "__pd_index" << COMMA << endl;
  (* out) << TAB << tbl_name << "__pd_flags" << COMMA << endl;
  (* out) << TAB << tbl_name << "__pd_addrs" << COMMA << endl;
  (* out) << TAB << tbl_name << "__pd_op"    << COMMA << endl;
  (* out) << TAB << tbl_name << "__pd_pred"  << COMMA << endl;

  (* out) << TAB << LEFT_BRACE << endl;
  for (int i = 0; i < MAX_SRC; i++) {
    (* out) << TAB << TAB << tbl_name << "__pd_src" << i;
    if (i < MAX_SRC - 1) (* out) << COMMA << endl;
  }
  (* out) << endl;
  (* out) << TAB << RIGHT_BRACE << COMMA << endl;

  (* out) << TAB << LEFT_BRACE << endl;
  for (int i = 0; i < MAX_DEST; i++) {
    (* out) << TAB << TAB << tbl_name << "__pd_dest" << i;
    if (i < MAX_DEST - 1) (* out) << COMMA << endl;
  }
  (* out) << endl;
  (* out) << TAB << RIGHT_BRACE << endl;
  (* out) << RIGHT_BRACE << SEMICOLON << endl << endl;  
}
