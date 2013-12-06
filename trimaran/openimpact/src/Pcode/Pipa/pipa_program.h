/*****************************************************************************\
 *
 *                    Illinois Open Source License
 *                     University of Illinois/NCSA
 *                         Open Source License
 *
 * Copyright (c) 2004, The University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:
 *
 *              IMPACT Research Group
 *
 *              University of Illinois at Urbana-Champaign
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
 *      File:    pipa_program.h
 *      Author:  Erik Nystrom
 *      Copyright (c) 2003  Erik Nystrom, Wen-mei Hwu
 *              and The Board of Trustees of the University of Illinois.
 *              All rights reserved.
 *      The University of Illinois software License Agreement
 *      specifies the terms and conditions for redistribution.
\*****************************************************************************/

#ifndef _PIPA_PROGRAM_H_
#define _PIPA_PROGRAM_H_

#include "pipa_common.h"
#include "pipa_callsite.h"
#include "pipa_symbols.h"
#include "pipa_graph.h"

extern struct IPA_prog_stats_t *program_stats;

/**************************************************************************
 * General points-to information
 **************************************************************************/

#define  IPA_GLOBAL_VAR_NAME      "GLOBALS"
#define  IPA_ANONYMOUS_PREFIX     "HEAP"
#define  IPA_FN_SUFFIX            "_REALFN"
#define  IPA_FORMAL_PARAM_PREFIX  "F_PARAM"
#define  IPA_ACTUAL_RET_PREFIX    "A_RET"
#define  IPA_ACTUAL_PARAM_PREFIX  "A_PARAM"
#define  IPA_ALLOC_RETURN         "RETURN_OUT"
#define  IPA_ALLOC_VAR_PREFIX     "RETURN_MEM_ALLOC_"
#define  IPA_IND_CALLEE_PREFIX    "CALLEE_"

typedef struct IPA_prog_info_t
{
  /* This helps with parsing */
  SymbolTable symboltable;
  IPA_funcsymbol_info_t *cur_fninfo;  

  /* List of all functions */
  IPA_funcsymbol_info_t *globals;
  IPA_funcsymbol_info_t *malloc;
  List fninfos;

  /* Callgraph */
  struct IPA_callg_t *call_graph;
  List new_callg_nodes;

  int max_type_id;
  int max_var_id;
  int max_cs_id;
  int in_library;

  /* func name -> func info */
  STRING_Symbol_Table *fnname2info_stab;
  /* symbol id   -> symbol info */
  HashTable id2sym_htab;
  /* symbol key  -> symbol info */
  IPA_Hashtab_t *symtab;
  /* Type name -> aggregate info ptr */
  STRING_Symbol_Table *typename2type;
  /* Type id -> aggregate info ptr */
  HashTable typeid2type;

  /* STATS */
  int zero_occur;
  int nonzero_occur[3];
  int total_base_count[3];
  int total_comp_count[3];
  int total_red_count[3]; 
     
  /* IO */
  FILE *errfile;
  IPA_funcsymbol_info_t **node_loc;
}
IPA_prog_info_t;

IPA_prog_info_t *IPA_prog_new ();
void IPA_prog_free (IPA_prog_info_t * info);


void
IPA_prog_stats_count(IPA_prog_info_t *info);

void
IPA_prog_classify_all(IPA_prog_info_t *info, List tsort);

#endif
