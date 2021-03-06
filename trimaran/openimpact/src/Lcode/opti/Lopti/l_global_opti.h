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
/*===========================================================================
 *      File :          l_global_opti.h
 *      Description :   global optimization declarations
 *      Creation Date : July, 1990
 *      Authors :       Scott Mahlke, Pohua Chang.
 *
 *      (C) Copyright 1990, Pohua Chang, Scott Mahlke.
 *      All rights granted to University of Illinois Board of Regents.
 *==========================================================================*/
#ifndef L_GLOBAL_OPTI_H
#define L_GLOBAL_OPTI_H

/* 10/29/02 REK Adding config.h */
#include <config.h>

/*
 *      External functions
 */

extern int L_global_dead_code_removal (L_Cb *);
extern int L_global_constant_propagation (L_Cb *, L_Cb *, int);
extern int L_global_copy_propagation (L_Cb *, L_Cb *);
extern int L_global_branch_val_propagation (L_Cb *, L_Cb *);
extern int L_global_memory_copy_propagation (L_Cb *, L_Cb *);
extern int L_global_common_subexpression (L_Cb *, L_Cb *, int);
#if 0
extern int L_global_redundant_load (L_Cb *, L_Cb *);
extern int L_global_redundant_store (L_Cb *, L_Cb *);
#endif
extern int L_global_memflow_redundant_load (L_Cb *, L_Cb *, int *);
extern int L_global_memflow_redundant_load_with_store (L_Cb *, L_Cb *, int *);
extern int L_global_memflow_redundant_store (L_Cb *, L_Cb *);
extern int L_global_dead_if_then_else_rem (L_Cb *, L_Cb *);
extern int L_global_remove_unnec_boolean (L_Cb *);
extern int L_global_mem_expression_copy_prop (L_Func *);
extern int L_global_dead_store_removal (L_Func *);
#endif
