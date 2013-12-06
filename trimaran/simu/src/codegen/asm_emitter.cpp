/******************************************************************************

                    SOFTWARE LICENSE AGREEMENT NOTICE
                   -----------------------------------

IT IS  A BREACH OF THIS  LICENSE AGREEMENT TO REMOVE  THIS NOTICE FROM
THE  FILE  OR SOFTWARE,  OR  ANY MODIFIED  VERSIONS  OF  THIS FILE  OR
SOFTWARE OR DERIVATIVE WORKS.
___________________________________________________

Copyright Notices/Identification  of Licensor(s) of  Original Software
in the File

Copyright (C) 2002 Georgia Institute of Technology

All rights reserved by the foregoing, respectively.
___________________________________________________

Copyright         Notices/Identification         of         Subsequent
Licensor(s)/Contributors of Derivative Works

Copyright <Year> <Owner>
<Optional: For Commercial license rights, contact:_______________>

All rights reserved by the foregoing, respectively.
___________________________________________________

The code contained in this  file, including both binary and source [if
released  by  the  owner(s)]   (hereafter,  Software)  is  subject  to
copyright  by the  respective Licensor(s)  and ownership  remains with
such  Licensor(s).  The  Licensor(s) of  the original  Software remain
free  to  license  their  respective proprietary  Software  for  other
purposes  that are independent  and separate  from this  file, without
obligation to any party.

Licensor(s) grant(s)  you (hereafter, Licensee)  a license to  use the
Software for  academic, research and internal  business purposes only,
without a  fee.  "Internal business purposes" means  that Licensee may
install, use and execute the Software for the purpose of designing and
evaluating  products.   Licensee  may  submit proposals  for  research
support, and receive funding  from private and Government sponsors for
continued development, support and maintenance of the Software for the
purposes permitted herein.

Licensee may also disclose results obtained by executing the Software,
as well as algorithms embodied therein.  Licensee may redistribute the
Software to third parties provided that the copyright notices and this
License Agreement  Notice statement are  reproduced on all  copies and
that  no charge is  associated with  such copies.  No patent  or other
intellectual property license is granted or implied by this Agreement,
and this  Agreement does not  license any acts except  those expressly
recited.

Licensee may modify the Software  to make derivative works (as defined
in Section 101 of Title  17, U.S. Code) (hereafter, Derivative Works),
as  necessary for  its own  academic, research  and  internal business
purposes.   Title  to  copyrights  and  other  proprietary  rights  in
Derivative  Works  created by  Licensee  shall  be  owned by  Licensee
subject,  however,  to the  underlying  ownership  interest(s) of  the
Licensor(s)  in the  copyrights and  other proprietary  rights  in the
original Software.   All the same  rights and licenses  granted herein
and  all  other  terms  and  conditions contained  in  this  Agreement
pertaining to the Software shall continue to apply to any parts of the
Software  included in  Derivative Works.   Licensee's  Derivative Work
should clearly notify users that it  is a modified version and not the
original Software distributed by the Licensor(s).

If  Licensee wants  to make  its Derivative  Works available  to other
parties,  such  distribution  will   be  governed  by  the  terms  and
conditions of this License  Agreement.  Licensee shall not modify this
License  Agreement, except  that Licensee  shall clearly  identify the
contribution  of  its  Derivative  Work  to this  file  by  adding  an
additional  copyright notice  to  the other  copyright notices  listed
above, to be added below the line "Copyright Notices/Identification of
Subsequent Licensor(s)/Contributors of Derivative Works."  A party who
is  not  an  owner of  such  Derivative  Work  within the  meaning  of
U.S. Copyright Law (i.e., the  original author, or the employer of the
author if "work  of hire") shall not modify  this License Agreement or
add such party's name to the copyright notices above.

Each party who contributes Software or makes a Derivative Work to this
file (hereafter, Contributed Code)  represents to each Licensor and to
other Licensees for its own Contributed Code that:

(a) Such Contributed  Code does not violate (or  cause the Software to
violate) the laws  of the United States, including  the export control
laws of the United States, or the laws of any other jurisdiction.

(b) The contributing  party has all legal right  and authority to make
such Contributed Code  available and to grant the  rights and licenses
contained in this License Agreement without violation or conflict with
any law.

(c) To the best of  the contributing party's knowledge and belief, the
Contributed  Code does  not infringe  upon any  proprietary  rights or
intellectual property rights of any third party.

LICENSOR(S) MAKE(S)  NO REPRESENTATIONS  ABOUT THE SUITABILITY  OF THE
SOFTWARE OR DERIVATIVE WORKS FOR  ANY PURPOSE.  IT IS PROVIDED "AS IS"
WITHOUT EXPRESS OR IMPLIED WARRANTY,  INCLUDING BUT NOT LIMITED TO THE
MERCHANTABILITY,  USE OR FITNESS  FOR ANY  PARTICULAR PURPOSE  AND ANY
WARRANTY  AGAINST INFRINGEMENT  OF ANY  INTELLECTUAL  PROPERTY RIGHTS.
LICENSOR(S) SHALL NOT BE LIABLE  FOR ANY DAMAGES SUFFERED BY THE USERS
OF THE SOFTWARE OR DERIVATIVE WORKS.

Any  Licensee  wishing to  make  commercial  use  of the  Software  or
Derivative Works  should contact each and every  Licensor to negotiate
an appropriate license for such commercial use, and written permission
of  all Licensors  will be  required  for such  a commercial  license.
Commercial use includes  (1) integration of all or  part of the source
code into  a product  for sale by  or on  behalf of Licensee  to third
parties, or  (2) distribution of  the Software or Derivative  Works to
third parties  that need  it to utilize  a commercial product  sold or
licensed by or on behalf of Licensee.

By using or copying this Contributed Code, Licensee agrees to abide by
the copyright law  and all other applicable laws of  the U.S., and the
terms of  this License Agreement.  Any individual  Licensor shall have
the right to terminate this license immediately by written notice upon
Licensee's  breach  of, or  non-compliance  with,  any  of its  terms.
Licensee   may  be   held  legally   responsible  for   any  copyright
infringement  that is caused  or encouraged  by Licensee's  failure to
abide by the terms of this License Agreement.

******************************************************************************/

///////////////////////////////////////////////////////////////////////////////
// File:     asm_emitter.cpp
// Author:   Mongkol Ekpanyapong
///////////////////////////////////////////////////////////////////////////////


#include "asm_emitter.h"


static const char LEFT_BRACE  = '{';
static const char RIGHT_BRACE = '}';
static const char COMMA1       = ',';
static const char SEMICOLON   = ';';


inline char* mprint(const eString& str) 
{
  if (str != null_string) {
    return (char*) str;
  }
  else return "0x0";
}


void print_asm(const Vector<OpEntry*>& function, int count, streamBuffer* out,
		   Vector<int>& control_block_id)
{
  eString function_name = function[0]->attribute;
  print_asm_header(function_name, out);
  for (int i = 0, count_control_block_id = 0; i < count; i++){ 
    print_asm_inst(*function[i], i, out, control_block_id, 
			 &count_control_block_id, function_name);
  }
  print_asm_ending(out);
}


void print_asm_header(eString& function_name, streamBuffer *out)
{
  (* out) << "" << function_name + 1 << "()" << LEFT_BRACE << endl;
}


void print_asm_ending(streamBuffer *out)
{
  (*out) << endl << RIGHT_BRACE << endl << endl;
}


void print_asm_inst(const OpEntry& op, int index, streamBuffer *out,
			  Vector<int>& control_block_id, int *count_control_block_id,
			  eString& function_name)
{
  if (index == control_block_id[*count_control_block_id] || index == 0) {
    (*out) << "label" << function_name <<  index << ":" << endl;
    if(index != 0) *count_control_block_id = *count_control_block_id + 1;
  }
  
  (* out) << TAB << op.id << SPACE;
  int start_count;
  if (op.id==0) start_count=1;
  else start_count = op.id;
  for(int i = start_count; i < 1000; i = i*10)
    (*out) << SPACE;
  (* out)  << mprint(op.op) + 5;
  
  // will change to hash
  int pot = 0;
  for ( ; pot < MAX_OP; pot++) {
    if(strstr(mprint(op.op), OP_TABLE[pot].op) != NULL)
	break;
  }
  
  for (int i = 0; i < OP_TABLE[pot].dest_no; i++) {
    print_operand(out, op.dest[i]);
    if (i < OP_TABLE[pot].dest_no - 1)
	(*out) << COMMA1;
  }
  for (int i = 0; i < OP_TABLE[pot].src_no; i++) {
    if(OP_TABLE[pot].dest_no!=0 || i!=0)
	(*out) << COMMA1;
    print_operand(out,op.src[i]);
  }
  
  (*out) << endl;
} 

char * print_macro(int num)
{
  switch(num){
  case 0: return "RET";break;
  case 1: return "INTRET";break;
  case 2: return "FLOATRET";break;
  case 3: return "DOUBLERET";break;
  case 4: return "INT_P1";break;
  case 5: return "INT_P2";break;
  case 6: return "INT_P3";break;
  case 7: return "INT_P4";break;
  case 8: return "FLOAT_P1";break;
  case 9: return "FLOAT_P2";break;
  case 10: return "FLOAT_P3";break;
  case 11: return "FLOAT_P4";break;
  case 12: return "DOUBLE_P1";break;
  case 13: return "DOUBLE_P2";break;
  case 14: return "RS";break;
  case 15: return "FP";break;
  case 16: return "IP";break;
  case 17: return "OPS";break;
  case 18: return "LV";break;
  case 19: return "LC";break;
  case 20: return "ESC";break;
  case 21: return "RRB";break;
  /* RMR { support vector rotating files */
  case 22: return "VRRB";break;
  /* } RMR */
  case 23: return "PV_0";break;
  case 24: return "PV_1";break;
  case 25: return "PV_2";break;
  case 26: return "PV_3";break;
  case 27: return "PV_4";break;
  case 28: return "PV_5";break;
  case 29: return "PV_6";break;
  case 30: return "PV_7";break;
  case 31: return "TEMP_REG";break;
  case 32: return "BLACK_HOLE";break;
  case 33: return "INT_ZERO";break;
  case 34: return "FLOAT_ZERO";break;
  case 35: return "DOUBLE_ZERO";break;
  case 36: return "FLOAT_ONE";break;
  case 37: return "DOUBLE_ONE";break;
  case 38: return "PRED_F";break;
  case 39: return "PRED_T";break;
  /* RMR { add default all true vector mask */
  case 40: return "VM_T";break;
  /* } RMR */
  case 41: return "PC";break;
  case 42: return "SP";break;
  case 43: return "VARARG";break;
  case 44: return "NUM_MACRO";break;
  }
  return NULL;
}

void print_operand(streamBuffer* out, Port iport)
{
  char *temp =mprint(iport.file);
  eString ROT ="";
  if (iport.is_reg == 1){
    if (strstr(temp,"_ROT")!=NULL) ROT = "_ROT";
    if(strstr(temp,"__PD_GPR_FILE")!=NULL) 
	(*out) << SPACE << "GPR" << ROT << SPACE <<iport.num;
    else if(strstr(temp,"__PD_FPR_FILE")!=NULL) 
	(*out) << SPACE << "FPR" << ROT << SPACE << iport.num;
    else if(strstr(temp,"__PD_PR_FILE")!=NULL) 
	(*out) << SPACE << "PR" << ROT << SPACE << iport.num;
    else if(strstr(temp,"__PD_CR_FILE")!=NULL) 
	(*out) << SPACE << "CR" << ROT << SPACE << iport.num;
    else if(strstr(temp,"__PD_BTR_FILE")!=NULL) 
	(*out) << SPACE << "BTR" << ROT << SPACE << iport.num;
    else if(strstr(temp,"__PD_MACRO_FILE")!=NULL) 
	(*out) << SPACE <<print_macro(iport.num);
    /* RMR { adding support for vector file type */
    else if(strstr(temp,"__PD_VIR_FILE")!=NULL) 
	(*out) << SPACE << "VIR" << ROT << SPACE << iport.num;	
    else if(strstr(temp,"__PD_VFR_FILE")!=NULL) 
	(*out) << SPACE << "VFR" << ROT << SPACE << iport.num;	
    else if(strstr(temp,"__PD_VMR_FILE")!=NULL) 
	(*out) << SPACE << "VMR" << ROT << SPACE << iport.num;	
    /* } RMR */
  }
  else (*out) << SPACE << temp ;
}


void remove_macro_name(char *output, char *input_string)
{
  char *temp;
  if (strcmp(input_string, "__PD_start_procedure") == 0) 
    strcpy(output, "__PD_start_procedure");
  else if (strcmp(input_string, "__PD_start_compound") == 0)
    strcpy(output, "__PD_start_compound");
  else if (strcmp(input_string, " __PD_prologue") == 0)
    strcpy(output, "__PD_prologue");
  else if (strcmp(input_string, " __PD_epilogue") == 0)
    strcpy(output, "__PD_epilogue");
  else if (strstr(input_string, "aclock") != NULL)
    strcpy(output, "__PD_aclock");
  else if ((temp = strstr(input_string, "__PD_")) != NULL) {
    strcpy(output, "PD_");
    strcat(output, temp + 5);
  }
  else strcpy(output, input_string);
}

