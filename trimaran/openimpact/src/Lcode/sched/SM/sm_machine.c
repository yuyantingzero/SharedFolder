/*****************************************************************************\
 *
 *		      Illinois Open Source License
 *                     University of Illinois/NCSA
 *                         Open Source License
 *
 * Copyright (c) 2004, The University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:             
 *
 *		IMPACT Research Group
 *
 *		University of Illinois at Urbana-Champaign
 *
 *              http://www.crhc.uiuc.edu/IMPACT
 *              http://www.gelato.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal with the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimers.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimers in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the names of the IMPACT Research Group, the University of
 * Illinois, nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without specific
 * prior written permission.  THE SOFTWARE IS PROVIDED "AS IS",
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
 *
\*****************************************************************************/
/*****************************************************************************\
 *      File:   sm_machine.c
 *      Author: Sain-Zee Ueng
 *      Creation Date:  March 2003
\*****************************************************************************/

#include <config.h>
#include "sm.h"

/* 20030313 SZU
 * Added since modules other than code generators link SM.
 * Punt if don't redefine versions of these functions.
 */
int
S_machine_check (L_Oper * oper)
{
  L_punt ("S_machine_check: Dummy version from SM. Should never be here!\n");

  return (0);
}

void
S_machine_rts (L_Oper * oper)
{
  L_punt ("S_machine_rts: Dummy version from SM. Should never be here!\n");
}

void
S_machine_jump (L_Oper * oper)
{
  L_punt ("S_machine_jump: Dummy version from SM. Should never be here!\n");
}
