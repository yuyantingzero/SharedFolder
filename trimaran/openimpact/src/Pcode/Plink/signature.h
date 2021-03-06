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
 * \brief Functions to generate type signatures.
 *
 * \author Robert Kidd and Wen-mei Hwu.
 *
 * This file contains function prototypes to generate type signatures.
 *
 * A type signature is the type or struct definition boiled down to
 * a character string.
 */

#ifndef _PLINK_SIGNATURE_H_
#define _PLINK_SIGNATURE_H_

#include <config.h>
#include <Pcode/pcode.h>

/* Functions to build signatures for Pcode structures. */
extern void Plink_BuildSignatures (SymbolTable ip_table, int src_file_key);
extern void Plink_UpdateSignatures (SymbolTable ip_table,
				    SymTabEntry src_entry);
extern char *Plink_BuildTypeDclSignature (SymbolTable ip_table,
					  SymTabEntry type_dcl_entry);
extern char *Plink_BuildStructDclSignature (SymbolTable ip_table,
					    SymTabEntry struct_dcl_entry);
extern char *Plink_BuildUnionDclSignature (SymbolTable ip_table,
					   SymTabEntry union_dcl_entry);

#endif
