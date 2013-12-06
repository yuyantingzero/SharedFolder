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
// File:     asm_emitter.h
// Author:   Mongkol Ekpanyapong
///////////////////////////////////////////////////////////////////////////////


#ifndef _ASM_EMITTER_H
#define _ASM_EMITTER_H


#include "emitter.h"


#define MAX_OP 79


void  print_asm(const Vector<OpEntry*>&, int, streamBuffer*, Vector<int>&);
void  print_asm_header(eString&, streamBuffer*);
void  print_asm_ending(streamBuffer*);
void  print_asm_inst(const OpEntry&, int, streamBuffer*, Vector<int>&, int*, eString&);
void  print_operand(streamBuffer*, Port);
void  get_name(char*, char*);
char* print_macro(int num);


typedef struct op_format{
  char* op;
  int   src_no;
  int   dest_no;
} OP_FORMAT;


const OP_FORMAT OP_TABLE[MAX_OP] = {
  {"FMPYADD",3,1},
  {"FMPYSUB",3,1},
  {"FMPYRSUB",3,1},
  {"RESTORE",1,1},
  {"LDCM",3,1},
  {"SH3",3,1},
  {"LDSI_",2,2},
  {"FLDSI_",2,2},
  {"SI_",3,1},
  {"FSI_",3,1},
  {"BRF",3,3},
  {"BRW",3,1},
  {"ADD",2,1},
  {"SUB",2,1},
  {"MPY",2,1},
  {"DIV",2,1},
  {"REM",2,1},
  {"FADD",2,1},
  {"FSUB",2,1},
  {"FMPY",2,1},
  {"FDIV",2,1},
  {"REM",2,1},
  {"AND",2,1},
  {"NAND",2,1},
  {"OR",2,1},
  {"NOR",2,1},
  {"XOR",2,1},
  {"SH",2,1},
  {"MAX",2,1},
  {"MIN",2,1},
  {"ABS",1,1},
  {"RCP",1,1},
  {"SQRT",1,1},
  {"FMAX",2,1},
  {"FMIN",2,1},
  {"FABS",1,1},
  {"FRCP",1,1},
  {"FSQRT",1,1},
  {"FRCP",1,1},
  {"FSQRT",1,1},
  {"CONV",1,1},
  {"EXTS",1,1},
  {"MOVE",1,1},
  {"CMPR",2,1},
  {"FCMPR",2,1},
  {"CMPP_W",2,2},
  {"FCMPP",2,2},
  {"RTS",1,0},
  {"FL_",1,1},
  {"LI_",2,2},
  {"FLI_",2,2},
  {"LDS_",1,1},
  {"FLDS_",1,1},
  {"LDV_",1,1},
  {"FLDV_",1,1},
  {"BRDV",2,1},
  {"PBR",2,1},
  {"BRU",1,0},
  {"BRC",2,0},
  {"BRLC",1,0},
  {"BRL",3,1},
  {"PRED_CLEAR",0,1},
  {"PRED_SET",0,1},
  {"PRED_AND",2,1},
  {"PRED_COMPL",1,1},
  {"PRED_LOAD",1,1},
  {"PRED_STORE",2,0},
  {"SAVE",2,0},
  {"FSAVE",2,0},
  {"FS_",2,0},
  {"MOVEGBP",2,1},
  {"MOVEGCM",3,1},
  {"start_procedure",2,0},
  {"start_compound",0,0},
  {"prologue",4,0},
  {"epilogue",1,0},
  {"aclock",1,0},
  {"L_",1,1},
  {"S_",2,0}
};


#endif /* _ASM_EMMITTER_H */
