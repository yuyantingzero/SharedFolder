/* IMPACT Public Release (www.crhc.uiuc.edu/IMPACT)            Version 2.33  */
/* IMPACT Trimaran Release (www.trimaran.org)                  July 12, 1999 */
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
 * Copyright 1990-1998 The Board of Trustees of the University of Illinois
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
#include <stdio.h>
#include <stdarg.h>

#if 0
typedef struct struct1
{
    char c;
    float f;
} Struct1;

typedef struct struct2
{
    double d;
    char c;
    float f;
} Struct2;

typedef struct struct3
{
    Struct1  s1;
    Struct2  s2;
    struct {
	char c;
	double d;
    } s3;
    int       i;
} Struct3;
#endif

/* In ansi C, only int, long, and double parameters may be accessed
 * via varargs (if the code is to be portable).
 */
void stdarg_print_int_list (int count, ...)
{
    va_list ap;
    int index, value;

    printf ("stdarg_print_int_list entered with count = %i\n", count);
    
    va_start(ap, count); /* make ap point to 1st unamed arg */
    for (index = 1; index <= count; index++)
    {
	value = va_arg(ap, int);
	printf ("  Arg %i: %i\n", index, value);
    }

    va_end(ap); /* clean up when done */
    printf ("\n");
}

void stdarg_print_int_list2 (char *header, int count, ...)
{
    va_list ap;
    int index, value;

    printf ("stdarg_print_int_list2 entered with header ='%s' and "
	    "count = %i\n",
	    header, count);
    
    va_start(ap, count); /* make ap point to 1st unamed arg */
    for (index = 1; index <= count; index++)
    {
	value = va_arg(ap, int);
	printf ("  Arg %i: %i\n", index, value);
    }

    va_end(ap); /* clean up when done */
    printf ("\n");
}

void stdarg_print_long_list (int count, ...)
{
    va_list ap;
    int index;
    long value;

    printf ("stdarg_print_long_list entered with count = %i\n", count);
    
    va_start(ap, count); /* make ap point to 1st unamed arg */
    for (index = 1; index <= count; index++)
    {
	value = va_arg(ap, long);
	printf ("  Arg %i: %i\n", index, value);
    }

    va_end(ap); /* clean up when done */
    printf ("\n");
}

void stdarg_print_double_list (int count, ...)
{
    va_list ap;
    int index;
    double value;

    printf ("stdarg_print_double_list entered with count = %i\n", count);
    
    va_start(ap, count); /* make ap point to 1st unamed arg */
    for (index = 1; index <= count; index++)
    {
	value = va_arg(ap, double);
	printf ("  Arg %i: %f\n", index, value);
    }

    va_end(ap); /* clean up when done */
    printf ("\n");
}

void stdarg_vfprintf (char *fmt, ...)
{
    va_list     args;

    fprintf(stdout, "In stdarg_vfprintf:\n");
    va_start (args, fmt);
    vfprintf (stdout, fmt, args);
    va_end(args);
}

int test_stdargs()
{
#if 0
    Struct1 S1;
    Struct2 S2;
    Struct3 S3;

    S1.c = 'a';
    S1.f = 1.1;

    S2.d = 2.12;
    S2.c = 'a';
    S2.f = 1.1;

    S3.s1.c = 'f';
    S3.s2.f = 3.313;
    S3.s3.d = 4.14;
    S3.i = 5;
#endif

    stdarg_print_int_list (1, 2);
    stdarg_print_int_list (3, 20, 30, 40);
    stdarg_print_int_list (10, 200, 300, 400, 500, 600, 700, 800, 900, 
			   1000, 1100);

    stdarg_print_int_list2 ("header2", 3, 21, 31, 41);

    stdarg_print_long_list (1, 200);
    stdarg_print_long_list (3, 2000, 3000, 4000);
    stdarg_print_long_list (10, 20000, 30000, 40000, 50000, 60000, 70000, 
			    80000, 90000, 100000, 110000);

    stdarg_print_double_list (1, 2.202);
    stdarg_print_double_list (3, 20.02, 30.03, 40.04);
    stdarg_print_double_list (10, 20.0202, 30.0303, 40.0404, 50.0505, 60.0606,
		       70.0707, 80.0808, 90.0909, 100.1010, 110.1111);

    stdarg_vfprintf ("vfprintf: '%c' '%s' '%i' '%f'\n"
		     "          '%c' '%s' '%i' '%f'\n",
		     'c', "string1", -5, -3.23,
		     'q', "string2", 50, 30.203);

    return (0);
}
