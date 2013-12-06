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

Copyright 1994 Hewlett-Packard Company

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



/* IMPACT Public Release (www.crhc.uiuc.edu/IMPACT)            Version 2.31  */
/* IMPACT Trimaran Release (www.trimaran.org)                  June 21, 1999 */
/*****************************************************************************\
 * LICENSE AGREEMENT NOTICE
 * 
 * IT IS A BREACH OF THIS LICENSE AGREEMENT TO REMOVE THIS NOTICE FROM
 * THE FILE OR SOFTWARE, OR ANY MODIFIED VERSIONS OF THIS FILE OR
 * SOFTWARE OR DERIVATIVE WORKS.
 * 
 * ------------------------------
 * 
 * Copyright Notices/Identification of Licensor(s) of 
 * Original Software in the File
 * 
 * Copyright 1990-1999 The Board of Trustees of the University of Illinois
 * For commercial license rights, contact: Research and Technology
 * Management Office, University of Illinois at Urbana-Champaign; 
 * FAX: 217-244-3716, or email: rtmo@uiuc.edu
 * 
 * All rights reserved by the foregoing, respectively.
 * 
 * ------------------------------
 * 	
 * Copyright Notices/Identification of Subsequent Licensor(s)/Contributors 
 * of Derivative Works
 * 
 * Copyright  <Year> <Owner>
 * <Optional:  For commercial license rights, contact:_____________________>
 * 
 * 
 * All rights reserved by the foregoing, respectively.
 * 
 * ------------------------------
 * 
 * The code contained in this file, including both binary and source 
 * [if released by the owner(s)] (hereafter, Software) is subject to
 * copyright by the respective Licensor(s) and ownership remains with
 * such Licensor(s).  The Licensor(s) of the original Software remain
 * free to license their respective proprietary Software for other
 * purposes that are independent and separate from this file, without
 * obligation to any party.
 * 
 * Licensor(s) grant(s) you (hereafter, Licensee) a license to use the
 * Software for academic, research and internal business purposes only,
 * without a fee.  "Internal business purposes" means that Licensee may
 * install, use and execute the Software for the purpose of designing and
 * evaluating products.  Licensee may submit proposals for research
 * support, and receive funding from private and Government sponsors for
 * continued development, support and maintenance of the Software for the
 * purposes permitted herein.
 * 
 * Licensee may also disclose results obtained by executing the Software,
 * as well as algorithms embodied therein.  Licensee may redistribute the
 * Software to third parties provided that the copyright notices and this
 * License Agreement Notice statement are reproduced on all copies and
 * that no charge is associated with such copies.  No patent or other
 * intellectual property license is granted or implied by this Agreement,
 * and this Agreement does not license any acts except those expressly
 * recited.
 * 
 * Licensee may modify the Software to make derivative works (as defined
 * in Section 101 of Title 17, U.S. Code) (hereafter, Derivative Works),
 * as necessary for its own academic, research and internal business
 * purposes.  Title to copyrights and other proprietary rights in
 * Derivative Works created by Licensee shall be owned by Licensee
 * subject, however, to the underlying ownership interest(s) of the
 * Licensor(s) in the copyrights and other proprietary rights in the
 * original Software.  All the same rights and licenses granted herein
 * and all other terms and conditions contained in this Agreement
 * pertaining to the Software shall continue to apply to any parts of the
 * Software included in Derivative Works.  Licensee's Derivative Work
 * should clearly notify users that it is a modified version and not the
 * original Software distributed by the Licensor(s).
 * 
 * If Licensee wants to make its Derivative Works available to other
 * parties, such distribution will be governed by the terms and
 * conditions of this License Agreement.  Licensee shall not modify this
 * License Agreement, except that Licensee shall clearly identify the
 * contribution of its Derivative Work to this file by adding an
 * additional copyright notice to the other copyright notices listed
 * above, to be added below the line "Copyright Notices/Identification of
 * Subsequent Licensor(s)/Contributors of Derivative Works."  A party who
 * is not an owner of such Derivative Work within the meaning of
 * U.S. Copyright Law (i.e., the original author, or the employer of the
 * author if "work of hire") shall not modify this License Agreement or
 * add such party's name to the copyright notices above.
 * 
 * Each party who contributes Software or makes a Derivative Work to this
 * file (hereafter, Contributed Code) represents to each Licensor and to
 * other Licensees for its own Contributed Code that:
 * 
 * (a) Such Contributed Code does not violate (or cause the Software to
 * violate) the laws of the United States, including the export control
 * laws of the United States, or the laws of any other jurisdiction.
 * 
 * (b) The contributing party has all legal right and authority to make
 * such Contributed Code available and to grant the rights and licenses
 * contained in this License Agreement without violation or conflict with
 * any law.
 * 
 * (c) To the best of the contributing party's knowledge and belief,
 * the Contributed Code does not infringe upon any proprietary rights or
 * intellectual property rights of any third party.
 * 
 * LICENSOR(S) MAKE(S) NO REPRESENTATIONS ABOUT THE SUITABILITY OF THE
 * SOFTWARE OR DERIVATIVE WORKS FOR ANY PURPOSE.  IT IS PROVIDED "AS IS"
 * WITHOUT EXPRESS OR IMPLIED WARRANTY, INCLUDING BUT NOT LIMITED TO THE
 * MERCHANTABILITY, USE OR FITNESS FOR ANY PARTICULAR PURPOSE AND ANY
 * WARRANTY AGAINST INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHTS.
 * LICENSOR(S) SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY THE USERS
 * OF THE SOFTWARE OR DERIVATIVE WORKS.
 * 
 * Any Licensee wishing to make commercial use of the Software or
 * Derivative Works should contact each and every Licensor to negotiate
 * an appropriate license for such commercial use, and written permission
 * of all Licensors will be required for such a commercial license.
 * Commercial use includes (1) integration of all or part of the source
 * code into a product for sale by or on behalf of Licensee to third
 * parties, or (2) distribution of the Software or Derivative Works to
 * third parties that need it to utilize a commercial product sold or
 * licensed by or on behalf of Licensee.
 * 
 * By using or copying this Contributed Code, Licensee agrees to abide by
 * the copyright law and all other applicable laws of the U.S., and the
 * terms of this License Agreement.  Any individual Licensor shall have
 * the right to terminate this license immediately by written notice upon
 * Licensee's breach of, or non-compliance with, any of its terms.
 * Licensee may be held legally responsible for any copyright
 * infringement that is caused or encouraged by Licensee's failure to
 * abide by the terms of this License Agreement.  
\*****************************************************************************/
/*****************************************************************************\
 *
 *  File:  l_parms.h
 *
 *  Description:  Header file for l_parms.c
 *
 *  Creation Date :  10-6-92 (Pulled into separate file 9-2-93)
 *
 *  Author:  John Gyllenhaal, Wen-mei Hwu
 *
 *  Revisions:
 *      Roger A. Bringmann - February 1993
 *      Modified for standard use within Lcode.
 *
 *      Brian Deitrich - September 1994
 *      Modified to alter the way parameters are handled.

 *      John C. Gyllenhaal - May 1998
 *      Enhanced load parameters to accept up to two equivalent names
 *      for a section.  This functionality was added to allow the Trimaran
 *      release to "rename" some IMPACT parameter sections (namely,
 *      global, file, arch, RegAlloc, Dependence, and Scheduler). 
 *
\*****************************************************************************/
#ifndef L_PARMS_H
#define L_PARMS_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include "l_punt.h"

#define PARM_LINE_MAX	2048	/* was 400 */
#define PARM_BUF_MAX	1024	/* was 200 */
#define PARM_LEVEL_MAX	20      /* was 10 */

#define PARM_FORCE_PARM	  1
#define PARM_COMMAND_LINE 2
#define PARM_ENVIRONMENT  3
#define PARM_INTERNAL     4

#define PARM_TABLE_HASH_SIZE    128	/* Must be power of 2 */
#define NOT_USED      0
#define USED          1

#define PARM_NOT_USED  0
#define NO_PARM_DEF    1
#define MULT_PARM_DEF  2

#define WARNINGS_ON    1
#define WARNINGS_OFF   0


typedef struct parm_table_node
{
    char                        *name;
    char                        *user_name;
    char                        *value;
    char                        *parm_file_name; /* used for error processing
                                 when converting value to real representation*/
    int                         parm_line_no; /* used for error processing
                                 when converting value to real representation*/
    int                         used; /* 0=parm not used yet (NOT_USED),
                                          else parm has been used */
    struct parm_table_node      *next_input; /* points to next element that
                                      was input from the parms file. used
                                      for printing out of parameters when
                                      DUMP_PARMS bit is set */
    struct parm_table_node      *next; /*points to next element in hash table*/
} Parm_Table_Node;

typedef struct parm_table_list
{
    Parm_Table_Node             *head[PARM_TABLE_HASH_SIZE];
} Parm_Table_List;

typedef struct parm_warn_node
{
    int                     flag; /* differentiates between NO_PARM_DEF,
                                     PARM_NOT_USED, and MULT_PARM_DEF */
    char                    *name;
    char                    *value; /* only used for MULT_PARM_DEF */
    char                    *old_value; /* only used for MULT_PARM_DEF */
    char                    *section_name1; /* Support aliased section names */
    char                    *section_name2;
    char                    *parm_file_name; /* not used in NO_PARM_DEF*/
    char                    *old_parm_file_name; /* only used for MULT_PARM_DEF */
    int                     parm_line_no; /* not used in NO_PARM_DEF */
    int                     old_parm_line_no; /* only used for MULT_PARM_DEF */
    struct parm_warn_node   *next;
} Parm_Warn_Node;

typedef struct parm_warn_list
{
    Parm_Warn_Node              *head;
} Parm_Warn_List;

typedef struct parm_macro_node
{
    int				from; /* Command line, environment, internal*/
    int				use_count;
    char                        *name;
    char                        *value;
    struct parm_macro_node      *next;
} Parm_Macro_Node;

typedef struct parm_macro_list
{
    Parm_Macro_Node             *head;
} Parm_Macro_List;

typedef struct parm_parse_info_st
{
    char        line_buf[PARM_LINE_MAX];  /* The raw parm file line */
    char        parm_buf[PARM_BUF_MAX];   /* The parameter being set */
    char        setting[PARM_BUF_MAX];    /* The setting for the parameter */
    FILE        *file;
    char        *name;
    int         line_no;
    Parm_Macro_List     *command_line_list;
    Parm_Macro_List     *internal_list[PARM_LEVEL_MAX];
    Parm_Table_List     *parm_list;
    int		internal_level;
    char        *calc_ptr;
    int         token;
    int         value;
} Parm_Parse_Info;

typedef struct parm_section_info
{
    char	*name1; /* Allow up to two different names for the same */
    char	*name2; /* section.  name2 = NULL, if no alias. */
    void	(*read_func)(Parm_Parse_Info *ppi);
    int		found[PARM_LEVEL_MAX];
    Parm_Table_List     *parm_list;
} Parm_Section_Info;

#ifdef __cplusplus
extern "C" {
#endif

/* Parameter file prototypes (private prototypes moved to l_parms.c 5/27/98) */

/* Using standard rules, get the parameter file to use from 1) the
 * command line, 2) the user's environment, or 3) default to 
 * 'default_std_parms'.
 */ 
extern char *L_get_std_parm_name (char **argv, char **envp, 
				  char *env_var_name, char *default_std_parms);

/* Create the external 'macro' list needed by L_load_parameters */
extern Parm_Macro_List *L_create_external_macro_list (char **argv, 
						      char **envp);

/* Used to read in parameter file section */
extern void L_load_parameters (char *parm_file_name,
                        Parm_Macro_List *command_line_macro_list,
                        char *section, 
			void (*read_func)(Parm_Parse_Info *ppi));

/* Allow two names to be specified for a section (a new and old name).
 * Both names will be accepted for the parameter file section name.
 */
extern void L_load_parameters_aliased (char *parm_file_name,
			Parm_Macro_List *command_line_macro_list,
			char *new_section_name, 
			char *old_section_name,
			void (*read_func)(Parm_Parse_Info *ppi));

/* Used to read in parameters of each type */
extern void L_read_parm_s (Parm_Parse_Info *ppi_in, char *name, char **ptr);
extern void L_read_parm_b (Parm_Parse_Info *ppi_in, char *name, int *ptr);
extern void L_read_parm_i (Parm_Parse_Info *ppi_in, char *name, int *ptr);
extern void L_read_parm_f (Parm_Parse_Info *ppi_in, char *name, float *ptr);
extern void L_read_parm_lf (Parm_Parse_Info *ppi_in, char *name, double *ptr);

/* These two functions should be called after all the parameter files
 * that can be read have been.
 */
extern void L_warn_about_unused_macros (FILE *out, 
					Parm_Macro_List *external_list);
extern void L_show_parms_warnings(void);

/*
 * Set to 1 to make L_parm_unknown punt.  Defaults to warn.
 */
extern int L_punt_on_unknown_parm;

/* 
 * Set to 1 to make the parm facility warn about old parameter section
 * names being used.  Defaults to "no warnings".  This allows mixing and
 * matching of new and old section names. -JCG 6/3/98
 */
extern int L_warn_old_parm_section_name_used;

extern char *L_parm_dump_file_name;
/*
 * -- output file name for dump of parameter information that was read in.
 * -- default is set to stderr.
 */


extern int L_warn_parm_not_used;
/*
 *  -- when this is set to 1, these warnings are enabled.
 *  -- warning occurs because a parameter defined in the parameter
 *     file was not used in a read routine.
 */

extern int L_warn_parm_not_defined;
/*
 *  -- when this is set to 1, these warnings are enabled.
 *  -- warning occurs because a parameter called in the read
 *     routine is not defined in the parameter file.
 *  -- NOTE: A parameter that is not in the parameter file, but
 *     is forced on the command line will be read correctly.
 *     in this case, this warning will not occur.
 */
extern int L_warn_dev_parm_not_defined;
/* 
 * --  same as above except for development parms, specified with an
 *     '?' before the name.  Allows warnings to be turned off for
 *     development parameters.
 */

extern int L_warn_parm_defined_twice;
/*
 *  -- when this is set to 1, these warnings are enabled.
 *  -- warning occurs because a parameter has multiple definitions
 *     in the same parameter file.
 */

extern int L_dump_parms;
/*
 *  -- when this is set to 1, the parameters that are used
 *     by the calling routine are appended to the file pointed to
 *     by the global variable, L_parm_dump_file_name.
 *  -- this variable is defined as:
 *     extern char *L_parm_dump_file_name
 *  -- the default value for this variable is "stderr".
 */

extern char *L_parm_warn_file_name;
/*
 *  -- output file name for parameter warnings.
 *  -- default is set to stderr.
 */

#ifdef __cplusplus
}
#endif


/* Macros for reading/printing parameters 
 *
 * If standard C, use # to put quotes around 'parm', otherwise
 * put the quotes around parm manually.
 * See John Gyllenhaal if you have problems.
 */
#if defined(__STDC__) || defined(__cplusplus)

#define READ_PARM_S(ppi,parm) L_read_parm_s(ppi,#parm,&parm)
#define READ_PARM_I(ppi,parm) L_read_parm_i(ppi,#parm,&parm)
#define READ_PARM_B(ppi,parm) L_read_parm_b(ppi,#parm,&parm)

#define PRINT_PARM_S(out,parm) fprintf (out, "    %s = %s;\n",#parm,parm)
#define PRINT_PARM_I(out,parm) fprintf (out, "    %s = %i;\n",#parm,parm)
#define PRINT_PARM_B(out,parm) fprintf (out, "    %s = %i;\n",#parm,parm)
#define PRINT_PARM_B_YES_NO(out,parm) \
    (parm ? fprintf (out, "    %s = yes;\n", #parm) : \
     fprintf (out, "    %s = no;\n", #parm))
#define PRINT_PARM_B_ON_OFF(out,parm) \
    (parm ? fprintf (out, "    %s = on;\n", #parm) : \
     fprintf (out, "    %s = off;\n", #parm))
#define PRINT_PARM_F(out,parm) fprintf (out, "    %s = %f;\n",#parm,parm)
#define PRINT_PARM_LF(out,parm) fprintf (out, "    %s = %lf;\n",#parm,parm)

#else

#define READ_PARM_S(ppi,parm) L_read_parm_s(ppi,"parm",&parm)
#define READ_PARM_I(ppi,parm) L_read_parm_i(ppi,"parm",&parm)
#define READ_PARM_B(ppi,parm) L_read_parm_b(ppi,"parm",&parm)

#define PRINT_PARM_S(out,parm) fprintf (out, "    %s = %s;\n","parm",parm)
#define PRINT_PARM_I(out,parm) fprintf (out, "    %s = %i;\n","parm",parm)
#define PRINT_PARM_B(out,parm) fprintf (out, "    %s = %i;\n","parm",parm)
#define PRINT_PARM_B_YES_NO(out,parm) \
    (parm ? fprintf (out, "    %s = yes;\n", "parm") : \
     fprintf (out, "    %s = no;\n", "parm"))
#define PRINT_PARM_B_ON_OFF(out,parm) \
    (parm ? fprintf (out, "    %s = on;\n", "parm") : \
     fprintf (out, "    %s = off;\n", "parm"))
#define PRINT_PARM_F(out,parm) fprintf (out, "    %s = %f;\n","parm",parm)
#define PRINT_PARM_LF(out,parm) fprintf (out, "    %s = %lf;\n","parm",parm)
#endif

#endif
