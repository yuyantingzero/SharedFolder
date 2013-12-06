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

Copyright 2007 University of Michigan

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



#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <getopt.h>
#include <string.h>

#include "util.h"
#include "hash_functions.h"
#include "region.h"
#include "op.h"
#include "region_attributes.h"

using namespace std;

void
print_usage_message()
{
  cout << "Usage: arm-trans input-file-name [-o output-file-name]\n";
  cout << "-o output-file-name\t\tredirect output to file output-file-name\n";
  cout << "-h\t\t\t\tprint this help message\n";
  exit(0);
}

void
parse_command_line(int argc, char* argv[], string &fname)
{
  int c;
  extern char *optarg;
  extern int optind;

  fname = "";
  while ((c = getopt(argc, argv, ":o:h")) != EOF) {
    switch(c) {
    case 'o':
      if (!freopen(optarg,"w",stdout)) {
	cerr << "ERROR: cannot open file!" << endl;
	exit(1);
      }
      break;
    case 'h':
      print_usage_message();
      break;
    case ':':
      cerr << "ERROR: Filename expected\n";
      exit(1);
      break;
    case '?':
      // ignore unrecognized parameters so that they can 
      // be passed to elcor/impact
      break;
    }
  }

  if (optind >= argc) {
    cerr << "ERROR: Filename expected" << endl;
    print_usage_message();
    exit(1);
  }

  fname = argv[optind];
}


unsigned
hash_string(string &str)
{
  char *cstr = (char*)str.c_str();

  return hash_char_ptr(cstr);
}


// ARM literal constants can be any 8 bit value rotated an even number of bits.
bool is_valid_dp_const(int n)
{
  if((n & 0xFFFFFF00) == 0)
    return true;
  if((n & 0xFFFFFC03) == 0)
    return true;
  if((n & 0xFFFFF00F) == 0)
    return true;
  if((n & 0xFFFFC03F) == 0)
    return true;
  if((n & 0xFFFF00FF) == 0)
    return true;
  if((n & 0xFFFC03FF) == 0)
    return true;
  if((n & 0xFFF00FFF) == 0)
    return true;
  if((n & 0xFFC03FFF) == 0)
    return true;
  if((n & 0xFF00FFFF) == 0)
    return true;
  if((n & 0xFC03FFFF) == 0)
    return true;
  if((n & 0xF00FFFFF) == 0)
    return true;
  if((n & 0xC03FFFFF) == 0)
    return true;
  if((n & 0x00FFFFFF) == 0)
    return true;
  if((n & 0x03FFFFFC) == 0)
    return true;
  if((n & 0x0FFFFFF0) == 0)
    return true;
  if((n & 0x3FFFFFC0) == 0)
    return true;

  return false;
}


bool is_valid_sl_const(int n)
{
  return ((n <= 31) && (n >= 0));
}


bool is_valid_sr_const(int n)
{
  return ((n <= 32) && (n >= 1));
}


bool is_dp_op(Op* op)
{
  Opcode opc = op->opcode();
  // FIX: should also include compares in the list of dp instructions,
  // but I think that might screw some things up.
  if(opc == AND_W || opc == XOR_W || opc == ADD_W || opc == ADDL_W ||
     opc == OR_W  || opc == SUB_W || opc == SUBL_W || opc == MOVE)
    return true;
  return false;
}


// FIX: this function shouldn't exist
string get_dp_opc(Op* op)
{
  switch(op->opcode()) {
  case AND_W:
    return string("\tand  ");
  case XOR_W:
    return string("\teor  ");
  case ADD_W:
  case ADDL_W:
    return string("\tadd  ");
  case OR_W:
    return string("\torr  ");
  case SUB_W:
  case SUBL_W:
    return string("\tsub  ");
  case MOVE:
    return string("\tmov  ");
  }
  return string("");
}


void 
output_register(Operand& oper, string& str)
{
  if (oper.is_reg()){ 
    if (!strcmp(oper.physical_file_type(),"GPR")) {
      str += 'r';
      char tmp[80];
      sprintf(tmp, "%d", oper.mc_num());
      str += tmp;
    }
    else if (!strcmp(oper.physical_file_type(),"FPR")) {
      str += 'f';
      char tmp[80];
      sprintf(tmp, "%d", oper.mc_num());
      str += tmp;
    }
    else {
      str += (char*)oper.physical_file_type();
      str += ":";
      char tmp[80];
      sprintf(tmp, "%d", oper.mc_num());
      str += tmp;
    }
  }
  else if (oper.is_macro_reg()){    
    switch (oper.name()){
      case SP_REG:
      case RGS_REG:{ //stack pointer
        //sp==r13
	str += "sp";
      } break;
      case RETURN_ADDR:{
	//lr==r14
	str += "lr";
      } break;
      case INT_RETURN:{ //return value r0 - r3
	str += "r0";
      } break;
      case INT_PARAM_1:{ //parameter 1
	str += "r0";    //a1 = r0
      } break;
      case INT_PARAM_2:{ //parameter 2
	str += "r1";    //a2 = r1
      } break;
      case INT_PARAM_3:{ //parameter 3
	str += "r2";    //a3 = r2
      } break;
      case INT_PARAM_4:{ //parameter 4
	str += "r3";    //a4 = r3
      } break;

      case FLT_RETURN:
      case FLT_PARAM_1:{ //parameter 1
	str += "f0";
      } break;
      case FLT_PARAM_2:{ //parameter 2
	str += "f1";
      } break;
      case FLT_PARAM_3:{ //parameter 3
	str += "f2";
      } break;
      case FLT_PARAM_4:{ //parameter 4
	str += "f3";
      } break;
      case DBL_RETURN:
      case DBL_PARAM_1:{ //parameter 1
	str += "f0";
      } break;
      case DBL_PARAM_2:{ //parameter 2
	str += "f1";
      } break;
      case DBL_PARAM_3:{ //parameter 3
	str += "f2";
      } break;
      case DBL_PARAM_4:{ //parameter 4
	str += "f3";
      } break;
      case ICMOVE_REG: {
	str += "r11";
      } break;
      default:{
        cerr << "ERROR: Unmapped macro register: " << oper.name() << endl;
	exit(-1);
      } break;
    }
  }
  else if (oper.is_lit()){
    if (oper.is_int()) {
      str += '#';
      char tmp[80];
      sprintf(tmp, "%d", oper.int_value());
      str += tmp;
    } else if(oper.is_label()) {
      string label = oper.label_value();

      // strip the prefix _$fn_ off of library calls in the rebel code
      if (!strncmp(label.c_str(), "_$fn_", 5)) 
	label = label.substr(5, label.length() - 1);
      else
	label = label.substr(1, label.length()-1);

      // FIX: this is a really bad hack to get alloca working quickly
      if( ! strcmp(label.c_str(), "__builtin_alloca"))
	label = "malloc";
      if( ! strcmp(label.c_str(), "__ctype_b_loc"))
	label = "__ctype_b";

      str += '=';
      str += label;
    }
    else {
      cerr << "ERROR: Untranslated literal " << oper;
      exit(-1);
    }
  } else {
    cerr << "ERROR: Unknown operand type in "
      "process_rebel.output_register()   " << oper << endl;
    exit(-1);
  }
}
