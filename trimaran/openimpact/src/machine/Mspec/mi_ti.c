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
 *	File :	mi_ti.c 
 *	Desc :	Machine dependent specification.  
 *	Date :	December 1995
 *	Auth :  Dan Connors and Sabrina Hwu
 *
 *==========================================================================*/

/*****************************************************************************\
 * NOTICE OF CONVENTION                                                      *
 * ------------------------------------------------------------------------- *
 * Mspec links to Pcode, Hcode, and Lcode modules.  In order to allow this   *
 * to take place without requiring front-end modules to link to liblcode.a,  *
 * Mspec code is divided into two classes as follows:                        *
 *  - mi_*.c must not depend on linkage to liblcode.a                        *
 *  - ml_*.c may depend on linkage to liblcode.a                             *
\*****************************************************************************/

/* 10/29/02 REK Adding config.h */
#include <config.h>
#include <stdio.h>
#ifdef M_TI_FOR_HCODE
#include <Hcode/h_ccode.h>
#endif
#include <Lcode/l_main.h>
#include "m_ti.h"

int use_standard_function_call_model = 0;

/*--------------------------------------------------------------------------*/
#define M_TI_SIZE_VOID		0
#define M_TI_SIZE_BIT		1
#define M_TI_SIZE_CHAR		32	/* 32-bit word addressable */
#define M_TI_SIZE_SHORT		32	/* 32-bit word addressable */
#define M_TI_SIZE_INT		32
#define M_TI_SIZE_LONG		32
#define M_TI_SIZE_FLOAT		32
#define M_TI_SIZE_DOUBLE	32	/* TI doesn't have double */
#define M_TI_SIZE_POINTER	32
#define M_TI_SIZE_UNION		-1
#define M_TI_SIZE_STRUCT	-1
#define M_TI_SIZE_BLOCK		-1
#define M_TI_SIZE_MAX	 	64

#define M_TI_ALIGN_VOID		-1
#define M_TI_ALIGN_BIT		1
#define M_TI_ALIGN_CHAR		32	/* 32-bit word address able */
#define M_TI_ALIGN_SHORT	32	/* 32-bit word address able */
#define M_TI_ALIGN_INT		32
#define M_TI_ALIGN_LONG		32
#define M_TI_ALIGN_FLOAT	32
#define M_TI_ALIGN_DOUBLE	32	/* TI doesn't have double */
#define M_TI_ALIGN_POINTER	32
#define M_TI_ALIGN_UNION	-1	/* depends on the field */
#define M_TI_ALIGN_STRUCT	-1
#define M_TI_ALIGN_BLOCK	-1
#define M_TI_ALIGN_MAX		64

int
M_ti_type_size (int mtype)
{
  switch (mtype)
    {
    case M_TYPE_VOID:
      return M_TI_SIZE_VOID;
    case M_TYPE_BIT_LONG:
      return M_TI_SIZE_BIT;
    case M_TYPE_BIT_CHAR:
      return M_TI_SIZE_BIT;
    case M_TYPE_CHAR:
      return M_TI_SIZE_CHAR;
    case M_TYPE_SHORT:
      return M_TI_SIZE_SHORT;
    case M_TYPE_INT:
      return M_TI_SIZE_INT;
    case M_TYPE_LONG:
      return M_TI_SIZE_LONG;
    case M_TYPE_FLOAT:
      return M_TI_SIZE_FLOAT;
    case M_TYPE_DOUBLE:
      return M_TI_SIZE_DOUBLE;
    case M_TYPE_POINTER:
      return M_TI_SIZE_POINTER;
    case M_TYPE_UNION:
      return M_TI_SIZE_UNION;
    case M_TYPE_STRUCT:
      return M_TI_SIZE_STRUCT;
    case M_TYPE_BLOCK:
      return M_TI_SIZE_BLOCK;
    default:
      return -1;
    }
}

int
M_ti_type_align (int mtype)
{
  switch (mtype)
    {
    case M_TYPE_VOID:
      return M_TI_ALIGN_VOID;
    case M_TYPE_BIT_LONG:
      return M_TI_ALIGN_BIT;
    case M_TYPE_BIT_CHAR:
      return M_TI_ALIGN_BIT;
    case M_TYPE_CHAR:
      return M_TI_ALIGN_CHAR;
    case M_TYPE_SHORT:
      return M_TI_ALIGN_SHORT;
    case M_TYPE_INT:
      return M_TI_ALIGN_INT;
    case M_TYPE_LONG:
      return M_TI_ALIGN_LONG;
    case M_TYPE_FLOAT:
      return M_TI_ALIGN_FLOAT;
    case M_TYPE_DOUBLE:
      return M_TI_ALIGN_DOUBLE;
    case M_TYPE_POINTER:
      return M_TI_ALIGN_POINTER;
    case M_TYPE_UNION:
      return M_TI_ALIGN_UNION;
    case M_TYPE_STRUCT:
      return M_TI_ALIGN_STRUCT;
    case M_TYPE_BLOCK:
      return M_TI_ALIGN_BLOCK;
    default:
      return -1;
    }
}

void
M_ti_char (M_Type type, int unsign)
{
  type->type = M_TYPE_CHAR;
  type->unsign = unsign;
  type->align = M_TI_ALIGN_CHAR;
  type->size = M_TI_SIZE_CHAR;
  type->nbits = M_TI_SIZE_CHAR;
}

void
M_ti_short (M_Type type, int unsign)
{
  type->type = M_TYPE_SHORT;
  type->unsign = unsign;
  type->align = M_TI_ALIGN_SHORT;
  type->size = M_TI_SIZE_SHORT;
  type->nbits = M_TI_SIZE_SHORT;
}

void
M_ti_int (M_Type type, int unsign)
{
  type->type = M_TYPE_INT;
  type->unsign = unsign;
  type->align = M_TI_ALIGN_INT;
  type->size = M_TI_SIZE_INT;
  type->nbits = M_TI_SIZE_INT;
}

void
M_ti_long (M_Type type, int unsign)
{
  type->type = M_TYPE_LONG;
  type->unsign = unsign;
  type->align = M_TI_ALIGN_LONG;
  type->size = M_TI_SIZE_LONG;
  type->nbits = M_TI_SIZE_LONG;
}

/*--------------------------------------------------------------------------*/
int
M_ti_layout_order (void)
{
  return M_BIG_ENDIAN;
}

/*--------------------------------------------------------------------------*/
void
M_set_model_ti (char *model_name)
{
  if (!strcasecmp (model_name, "1.0") ||
      !strcasecmp (model_name, "standard") || !strcasecmp (model_name, "1"))
    {
      M_model = M_TI_1;
      use_standard_function_call_model = 1;
    }
  else if (!strcasecmp (model_name, "register"))
    {
      M_model = M_TI_1;
      use_standard_function_call_model = 0;
    }
  else
    M_assert (0, "M_set_model_ti:  Illegal model specified!");
}
