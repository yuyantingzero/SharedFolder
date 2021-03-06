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


/* PtoNM_new
 *
 * An example of using the new symbol table. */

#define USE_IMPLICIT_FUNCTIONS 1

#include <Pcode/pcode.h>
#include <Pcode/parms.h>
#include <Pcode/struct.h>
#include <Pcode/query.h>
#if USE_IMPLICIT_FUNCTIONS
#include <Pcode/symtab_i.h>
#else
#include <Pcode/symtab.h>
#include <Pcode/struct_symtab.h>
#endif

int
P_gen_code (char *prog_name, Parm_Macro_List *external_list,
	    SymbolTable symbol_table, int file)
{
  SymTabEntry entry;
  Key key;
  int i;
  int last_file;
  
#if USE_IMPLICIT_FUNCTIONS
  PSI_SetTable (symbol_table);
#endif

  PSI_SetFlags (STF_READ_ONLY);

  if (file == 0)
    last_file = P_GetSymbolTableNumFiles (symbol_table);
  else
    last_file = file;

  /* Here's one way to process the files. */
  for (i = 1; i <= last_file; i++)
    {
#if USE_IMPLICIT_FUNCTIONS
      for (key = PSI_GetFileEntryByType (i, ET_ANY); P_ValidKey (key);
	   key = PSI_GetFileEntryByTypeNext (key, ET_ANY))
#else
      for (key = PST_GetFileEntryByType (symbol_table, i, ET_ANY);
	   P_ValidKey (key);
	   key = PST_GetFileEntryByTypeNext (symbol_table, key, ET_ANY))
#endif	
	{
#if USE_IMPLICIT_FUNCTIONS
	  entry = PSI_GetSymTabEntry (key);
#else
	  entry = PST_GetSymTabEntry (symbol_table, key);
#endif

	  switch (P_GetSymTabEntryType (entry))
	    {
	    case ET_FUNC:
	      {
		FuncDcl func_dcl = P_GetSymTabEntryFuncDcl (entry);

		fprintf (Fout, "         T %s\n", P_GetFuncDclName (func_dcl));
	      }
	      break;

	    case ET_TYPE_LOCAL:
	    case ET_TYPE_GLOBAL:
	    case ET_VAR_LOCAL:
	      break;

	    case ET_VAR_GLOBAL:
	      {
		VarDcl var_dcl = P_GetSymTabEntryVarDcl (entry);

		if (P_GetVarDclQualifier (var_dcl) & VQ_EXTERN)
		  {
		    fprintf (Fout, "         U %s\n",
			     P_GetVarDclName (var_dcl));
		  }
		else
		  {
		    fprintf (Fout, "         D %s\n",
			     P_GetVarDclName (var_dcl));
		  }
	      }
	      break;

	    case ET_STRUCT:
	    case ET_UNION:
	    case ET_ENUM:
	    case ET_ASM:
	    case ET_STMT:
	    case ET_EXPR:
	    case ET_FIELD:
	    case ET_ENUMFIELD:
	    case ET_LABEL:
	    case ET_SCOPE:
	    default:
	      break;
	    }
	}
    }

#if 0
  /* Here's another */
  for (i = 1; i <= P_GetSymbolTableNumFiles (symbol_table); i++)
    {
      for (key = PST_GetFileEntryByType (symbol_table, i,
					 ET_FUNC | ET_VAR_GLOBAL);
	   P_ValidKey (key);
	   key = PST_GetFileEntryByTypeNext (symbol_table, key,
					     ET_FUNC | ET_VAR_GLOBAL))
	{
	  if (P_GetSymTabEntryType (entry) == ET_FUNC)
	    {
	      FuncDcl func_dcl = P_GetSymTabEntryFuncDcl (entry);

	      fprintf (Fout, "         T %s\n", P_GetFuncDclName (func_dcl));
	    }
	  else
	    {
	      VarDcl var_dcl = P_GetSymTabEntryVarDcl (entry);

	      if ((P_GetVarDclQualifier (var_dcl) | \
		   PST_GetTypeQualifier (P_GetVarDclType (var_dcl))) & \
		  TY_EXTERN)
		{
		  fprintf (Fout, "         U %s\n", name);
		}
	      else
		{
		  fprintf (Fout, "         D %s\n", name);
		}
	    }
	}
    }
#endif

  return (0);
}




