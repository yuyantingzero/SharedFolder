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
/*! \file
 * \brief Declarations for routines to read Pcode from a file.
 *
 * \author Robert Kidd, Wen-mei Hwu
 *
 * This file contains declarations for routines to read Pcode and the
 * symbol table from a file.
 */
/*****************************************************************************/

#ifndef _PCODE_READ_SYMTAB_H_
#define _PCODE_READ_SYMTAB_H_

#include <config.h>
#include <stdio.h>
#include "pcode.h"

/*! \brief A struct to hold the return values from the symbol table parser. */
typedef struct SymTabParserArg
{
  SymbolTable table;
  IPSymTabEnt ipste;
  char *include_name;
  Key key;
} SymTabParserArg;

extern SymbolTable PST_ReadFile (FILE *in, int *file);
extern SymTabEntry PST_ReadSymbolFromIn (SymbolTable table, Key key);
extern SymTabEntry PST_ReadSymbolFromOut (SymbolTable table, Key key);
extern SymTabEntry PST_ReadSymbolCopyFromIn (SymbolTable table, Key key);
extern SymTabEntry PST_ReadSymbolCopyFromOut (SymbolTable table, Key key);

#endif
