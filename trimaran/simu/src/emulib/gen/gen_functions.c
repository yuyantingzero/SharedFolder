/******************************************************************************
LICENSE NOTICE
--------------

IT IS  A BREACH OF  THE LICENSE AGREEMENT  TO REMOVE THIS  NOTICE FROM
THIS  FILE  OR SOFTWARE  OR  ANY MODIFIED  VERSIONS  OF  THIS FILE  OR
SOFTWARE.

Copyright notices/Licensor(s) Identification
--------------------------------------------
Each of  the entity(ies) whose name properly  appear immediately below
in connection with a copyright notice is a Licensor(s) under the terms
that follow.

Copyright  1998  New  York  University.  All rights  reserved  by  the
foregoing, respectively.

Copyright 2001 Center for  Research on Embedded Systems and Technology
at the  Georgia Institute  of Technology. All  rights reserved  by the
foregoing, respectively.

License agreement
-----------------

The  code contained  in this  file  including both  binary and  source
(hereafter,  Software)  is subject  to  copyright  by Licensor(s)  and
ownership remains with Licensor(s).

Licensor(s)  grants you  (hereafter, Licensee)  a license  to  use the
Software for  academic, research and internal  business purposes only,
without  a  fee.  "Internal  business  use"  means  that Licensee  may
install, use and execute the Software for the purpose of designing and
evaluating products.   Licensee may also disclose  results obtained by
executing  the  Software,  as  well as  algorithms  embodied  therein.
Licensee may  distribute the Software  to third parties  provided that
the copyright notice and this statement appears on all copies and that
no  charge  is  associated  with  such copies.   No  patent  or  other
intellectual property license is granted or implied by this Agreement,
and this  Agreement does not  license any acts except  those expressly
recited.

Licensee may  make derivative works,  which shall also be  governed by
the  terms of  this  License Agreement.  If  Licensee distributes  any
derivative work based  on or derived from the  Software, then Licensee
will abide by the following terms.  Both Licensee and Licensor(s) will
be  considered joint  owners of  such derivative  work  and considered
Licensor(s) for  the purpose of distribution of  such derivative work.
Licensee shall  not modify this  agreement except that  Licensee shall
clearly  indicate  that  this  is  a  derivative  work  by  adding  an
additional copyright notice in  the form "Copyright <year> <Owner>" to
other copyright notices above, before the line "All rights reserved by
the foregoing, respectively".   A party who is not  an original author
of such derivative works within  the meaning of US Copyright Law shall
not modify or add his name to the copyright notices above.

Any Licensee  wishing to  make commercial use  of the  Software should
contact each and every Licensor(s) to negotiate an appropriate license
for  such  commercial  use;  permission  of all  Licensor(s)  will  be
required for such a  license.  Commercial use includes (1) integration
of all or part  of the source code into a product  for sale or license
by or on  behalf of Licensee to third parties,  or (2) distribution of
the Software  to third  parties that need  it to utilize  a commercial
product sold or licensed by or on behalf of Licensee.

LICENSOR (S)  MAKES NO REPRESENTATIONS  ABOUT THE SUITABILITY  OF THIS
SOFTWARE FOR ANY  PURPOSE.  IT IS PROVIDED "AS  IS" WITHOUT EXPRESS OR
IMPLIED WARRANTY.   LICENSOR (S) SHALL  NOT BE LIABLE FOR  ANY DAMAGES
SUFFERED BY THE USERS OF THIS SOFTWARE.

IN NO EVENT UNLESS REQUIRED BY  APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY  COPYRIGHT HOLDER, OR ANY  OTHER PARTY WHO  MAY MODIFY AND/OR
REDISTRIBUTE THE  PROGRAM AS PERMITTED  ABOVE, BE LIABLE  FOR DAMAGES,
INCLUDING  ANY GENERAL, SPECIAL,  INCIDENTAL OR  CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OR  INABILITY TO USE THE PROGRAM (INCLUDING BUT
NOT  LIMITED TO  LOSS OF  DATA OR  DATA BEING  RENDERED  INACCURATE OR
LOSSES SUSTAINED BY  YOU OR THIRD PARTIES OR A  FAILURE OF THE PROGRAM
TO  OPERATE WITH ANY  OTHER PROGRAMS),  EVEN IF  SUCH HOLDER  OR OTHER
PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

By using  or copying  this Software, Licensee  agrees to abide  by the
copyright law and all other applicable laws of the U.S., and the terms
of  this license  agreement. Any  individual Licensor  shall  have the
right  to terminate this  license immediately  by written  notice upon
Licensee's   breach   of,  or   non-compliance   with,   any  of   its
terms.  Licensee may  be held  legally responsible  for  any copyright
infringement  that is caused  or encouraged  by Licensee's  failure to
abide     by    the     terms    of     this     license    agreement.
******************************************************************************/

/******************************************************************************
 * File:    gen_functions.c
 * Authors: Amit Nene, Igor Pechtchanski, Rodric M. Rabbah
 *****************************************************************************/


#include "../PD_globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define MAX_LINE 1000

#ifdef _DEBUG_
#  define DEBUG printf
#endif

enum genType {C_file, header};

char *LICENSE_FILE = ".license.txt";

char *spaces(int n);

void hdrheader(FILE *outfile, char *filename, char *description);
void hdrfooter(FILE *outfile);
void openheader(char *filename, char *description);
void closeheader();

void srcheader(FILE *outfile, char *filename, char *description);
void srcfooter(FILE *outfile);
void opensource(char *filename, char *description);
void closesource();

void NYUheader(FILE *of);
void fileheader(FILE *of, char *filename);
void descheader(FILE *of, char *desc);
void autoheader(FILE *of);

/* substitution map methods */
struct SUBST;
void subst_init(struct SUBST *subst);
int  subst_lookup_index(struct SUBST *subst, char *var);
void subst_update(struct SUBST *subst, char *var, char *val);
char *subst_lookup(struct SUBST *subst, char *var);

/* loop map methods */
struct LOOP;
void loop_init(struct LOOP *loop);
void loop_add(struct LOOP *loop, char *var, int n, char *val[]);
bool loop_empty(struct LOOP *loop);
bool loop_more(struct LOOP *loop);
void loop_next(struct LOOP *loop);
char *loop_strsub(struct LOOP *loop, char *dest, char *src);

char *strsub(char *dest, char *src);

/* RMR { adding support for vector file type and vector operations; as well as custom functions } */
void generateFunction(char *name, char *opcode, char *desc, char *parm, char *code, int pred, int vectorize, int custom, int vmasked,
                      int nsrcs, char src[], int ndests, char dest[]);
char type2field(char *field, char type, char mod);
char port2field(char *field, char *port, int num, char type, char mod);
char dest2field(char *field, int num, char type, char mod);
/* RMR { adding support for custom functions: no delay()s generated } */
void generateBody(char *newcode, char *opcode, char src[], int ndests, char dest[], char *code, bool is_cmpp, bool custom);
/* RMR { adding support for vector file type and vector operations */
void generateVBody(char *newcode, int nsrcs, char src[], int ndests, char dest[], char *code, int vectorsize, int vmasked);
char port2Vfield(char *field, char *port, int num, char type, char mod, int voffset);
char dest2Vfield(char *field, int num, char type, char mod, int voffset);
/* } RMR */
void executeLoop(char *line);

void processSection(char *line, enum genType type);
void processInit(FILE *outfile, char *line);
void processLoop(char *line);
void processTemplate(char *line);
void processLine(char *line);

FILE *infile  = NULL;
FILE *hdrfile = NULL;
FILE *srcfile = NULL;

char *spaces(int n) {    /* n <= 80 */
  static char *sp = "                                                                                ";
  int i = 80 - n;
  if (i < 0) i = 0;
  if (i > 80) i = 80;
  return sp + i;
}

void closesource() {
  if (srcfile) {
	srcfooter(srcfile);
	fclose(srcfile);
  }
}

void opensource(char *filename, char *description) {
  closesource();
  srcfile = fopen(filename, "w");
  srcheader(srcfile, filename, description);
}

void closeheader() {
  if (hdrfile) {
	hdrfooter(hdrfile);
	fclose(hdrfile);
  }
}

void openheader(char *filename, char *description) {
  closeheader();
  hdrfile = fopen(filename, "w");
  hdrheader(hdrfile, filename, description);
}


void licenseheader(FILE *of) {
  char string[MAX_LINE];
  FILE *lcnsfile = fopen(LICENSE_FILE, "r");  

  if (lcnsfile == NULL) {
	fprintf (stderr, "Could not open License Agrrement file!\n");
	exit(1);
  }

  else {
	while (fgets(string, MAX_LINE, lcnsfile)) {
	  fprintf(of, "%s", string);
	  fflush(of);
	}
  }

  fclose(lcnsfile);
  fprintf(of, "\n");
  fflush(of);
}

void fileheader(FILE *of, char *filename) {
  fprintf(of, "/******************************************************************************\n");
  fprintf(of, " *      File:        %s\n", filename);
  fprintf(of, " *      Author:      Machine generated\n");
  fflush(of);
}

void descheader(FILE *of, char *desc) {
  char _desc[MAX_LINE];
  int len;
  strcpy(_desc, desc);
  desc = _desc;
  len = strlen(desc);
  fprintf(of, " *      Description: %.54s%s   \n", desc, spaces(54-len));
  while (len > 54) {
	desc += 54;
	len -= 54;
	fprintf(of, " *                   %.54s%s   \n", desc, spaces(54-len));
  } while (len > 54);
  fflush(of);
}

void autoheader(FILE *of) {
  fprintf(of, " *\n");
  fprintf(of, " *      Note:        THIS IS AN AUTOMATICALLY GENERATED FILE!\n");
  fprintf(of, " *                   DO NOT EDIT!!!!!\n");
  fprintf(of, " *****************************************************************************/\n");
  fprintf(of, "\n");
  fflush(of);
}

void srcheader(FILE *outfile, char *filename, char *description) {
  licenseheader(outfile);
  fileheader(outfile, filename);
  descheader(outfile, description);
  autoheader(outfile);

  fprintf(outfile, "\n#include \"PD.h\"\n\n");
  fflush(outfile);
}

void srcfooter(FILE *outfile) {
  fflush(outfile);
}

void hdrheader(FILE *outfile, char *filename, char *description) {
  static char define[MAX_LINE] = "_";
  char *p;
  licenseheader(outfile);
  fileheader(outfile, filename);
  descheader(outfile, description);
  autoheader(outfile);

  strcpy(define+1, filename);
  for (p = define + 1; *p; p++) {
	if (*p == '.')
	  *p = '_';
	*p = toupper(*p);
  }
  fprintf(outfile, "#ifndef %s\n", define);
  fprintf(outfile, "#define %s\n", define);
  fprintf(outfile, "\n");
  fprintf(outfile, "#include \"PD_op.h\"\n");
  fprintf(outfile, "\n");
  fflush(outfile);
}

void hdrfooter(FILE *outfile) {
  fprintf(outfile, "\n");
  fprintf(outfile, "#endif\n");
  fprintf(outfile, "\n");
  fflush(outfile);
}

void processSection(char *line, enum genType type) {
  static char filename[200] = "PD_";
  int len = strlen(line);
  char *desc;
#ifdef _DEBUG_
  DEBUG("New section: %s\n", line);
#endif
  strcpy(filename+3, line);

  if (type == C_file)
	strcpy(filename+3+len, "_ops.c");

  fgets(line, MAX_LINE, infile);
  if (*line == '#') {
	*strchr(line + 1, '\n') = '\0';
	desc = line + 2;
  } else
	desc = "??? operations";
  opensource(filename, desc);
  fprintf(hdrfile, "/* Section: %s */\n", desc);
  if (*line != '#')
	processLine(line);
  fflush(srcfile);
  fflush(hdrfile);
}

void processInit(FILE *outfile, char *line) {
#ifdef _DEBUG_
  DEBUG("INIT string: %s\n", line);
#endif
  fprintf(outfile, "%s", line);
}

struct LOOP {
  int num;
  struct ITER {
	char var[20];
	int  num;
	char val[20][20];
	int  i;
  } iter[20];

  struct SUBST {
	int num;
	struct MAP {
	  char *var;
	  char *val;
	} map[20];
  } substmap;
};

struct LOOP loopmap;

void subst_init(struct SUBST *subst) {
  subst->num = 0;
}

int subst_lookup_index(struct SUBST *subst, char *var) {
  int i;
  for (i = 0; i < subst->num; i++)
	if (!strcmp(subst->map[i].var, var))
	  return i;
  return -1;
}

void subst_update(struct SUBST *subst, char *var, char *val) {
  int i = subst_lookup_index(subst, var);
  if (i < 0) {
	i = subst->num++;
	subst->map[i].var = var;
  }
  subst->map[i].val = val;
}

char *subst_lookup(struct SUBST *subst, char *var) {
  int i = subst_lookup_index(subst, var);
  if (i < 0)
	return NULL;
  return subst->map[i].val;
}

void loop_init(struct LOOP *loop) {
  subst_init(&loop->substmap);
  loop->num = 0;
}

void loop_add(struct LOOP *loop, char *var, int n, char *val[]) {
  int i;
  strcpy(loop->iter[loop->num].var, var);
  loop->iter[loop->num].num = n;
  for (i = 0; i < n; i++)
	strcpy(loop->iter[loop->num].val[i], val[i]);
  subst_update(&loop->substmap, loop->iter[loop->num].var,
               loop->iter[loop->num].val[0]);
  loop->iter[loop->num++].i = 0;
}

bool loop_empty(struct LOOP *loop) {
  return (loop->num == 0);
}

bool loop_more(struct LOOP *loop) {
  return (loop->iter[0].i < loop->iter[0].num);
}

void loop_next(struct LOOP *loop) {
  int i;
  for (i = loop->num - 1; i >= 0; i--) {
	loop->iter[i].i++;
	if (loop->iter[i].i < loop->iter[i].num || i == 0)
	  break;
	loop->iter[i].i = 0;
  }
  if (loop->iter[0].i < loop->iter[0].num)
	for (i = 0; i < loop->num; i++)
	  subst_update(&loop->substmap, loop->iter[i].var,
				   loop->iter[i].val[loop->iter[i].i]);
}

char *loop_strsub(struct LOOP *loop, char *dest, char *src) {
  char *val, *d = dest;
  char var[100];
  for (; *src; ) {
	if (*src == '$') {
	  src++;
	  for (val = var; isalnum(*src);)
		*val++ = *src++;
	  *val = '\0';
	  if (!(val = subst_lookup(&loop->substmap, var))) {
#ifdef _DEBUG_
		DEBUG("Invalid variable %s\n", var);
#endif
		exit(1);
	  }
	  for (; *val; )
		*dest++ = *val++;
	} else {
	  *dest++ = *src++;
	}
  }
  *dest = *src;
  return d;
}

char *strsub(char *dest, char *src) {
  return loop_strsub(&loopmap, dest, src);
}

char *strupcase(char* src)
{
  int c;
  char* usrc = strdup(src);

  for (c = 0; c < strlen(usrc); c++) {
	usrc[c] = toupper(usrc[c]);
  }
  return usrc;
}


void processLoop(char *line) {
  char loop[MAX_LINE];
  int  num;
  char *var, *p;
  char *val[30];
  strcpy(loop, line);
#ifdef _DEBUG_
  DEBUG("LOOP: %s\n", loop);
#endif
  var = strtok(loop, " ");
  for (num = 0; (p = strtok(NULL, " \n")); num++)
	val[num] = p;
  loop_add(&loopmap, var, num, val);
}

char type2field(char *field, char type, char mod) {
  switch (type) {
  case 'L':   strcpy(field, "");
	break;
  case 'P':   strcpy(field, "pr");
	break;
  case 'B':   strcpy(field, "btr");
	break;
  case 'C':   strcpy(field, "cr");
	break;
  case 'I':   strcpy(field, "gpr");
	break;
  case 'H':   strcpy(field, "fpr_W.lo");
	break;
  case 'U':   strcpy(field, "fpr_W.hi");
	break;
  case 'D':   strcpy(field, "fpr_D");
	break;
  case 'F':   strcpy(field, "fpr_");
	/* use modifier */
	switch (mod) {
	case 'S': strcat(field, "S");
	  break;
	case 'D': strcat(field, "D");
	  type = 'D';
	  break;
	case 'L': if (type != 'D' && type != 'H') {
	  if (type == 'U')
		type = 'D';
	  else
		type = 'H';
	}
	strcat(field, "W.lo");
	break;
	case 'U': if (type != 'D' && type != 'U') {
	  if (type == 'H')
		type = 'D';
	  else
		type = 'U';
	}
	strcat(field, "W.hi");
	break;
	default:  
#ifdef _DEBUG_
	  DEBUG("Invalid FP format: '%c'\n", mod);
#endif
	  exit(1);
	}
	break;
  default:    
#ifdef _DEBUG_
	DEBUG("Invalid type: '%c'(%d)\n", type, type);
#endif
	exit(1);
  }
  return type;
}


char port2field(char *field, char *port, int num, char type, char mod) {
  if (type == 'L') {
	sprintf(field, "LIT(OP_%s%d(op))", strupcase(port), num);
  }
  else {
    if (strcmp(port, "dest")) {
	sprintf(field, "CHKLOCK(SREG(OP_%s%d(op)))->reg.", strupcase(port), num);
    }
    else {
	sprintf(field, "SREG(OP_%s%d(op)).reg.", strupcase(port), num);
    }
  }
  field += strlen(field);
  return type2field(field, type, mod);
}


void port_pending_bit(char *field, int num, bool vreg) {
  if (vreg) {
    sprintf(field, "VREG(OP_DEST%d(op), i).pending", num);
  }
  else {
    sprintf(field, "SREG(OP_DEST%d(op)).pending", num);
  }
  field += strlen(field);
}

char dest2field(char *field, int num, char type, char mod) {
  if (type == 'L') {
#ifdef _DEBUG_
	DEBUG("Invalid destination type: '%c'(%d)\n", type, type);
#endif
	exit(1);
  }
  sprintf(field, "dest[%d].reg.", num);
  field += strlen(field);
  return type2field(field, type, mod);
}

/* CMPP has a slightly different mechanism for commiting the outputs */
void generateBody(char *newcode, char *opcode, char src[], int ndests, char dest[], char *code, bool is_cmpp, bool custom) {
  char mod = 0;
  int i, indent = 0, nest = 0, dest_init = 0;

  if (!code || !*code) {
    sprintf(newcode, "{\n   PD_trace_op(op, \"%s\", \"\", \"\", NULL);\n}\n", opcode);
	strcpy(newcode, "{\n}\n");
	return;
  }

  indent += 3;
  sprintf(newcode, "{\n%s", spaces(indent));
  newcode += strlen(newcode);

  sprintf(newcode, "__PD_reg dest[%d];\n%s", ndests, spaces(indent));
  newcode += strlen(newcode);

  /* RMR - Oct 30, 2003 { the main simulation kernel will reset the pending tags */
#if 0 /* RMR { */
  /* WWF - Oct 23, 2003 {*/
  if (ndests > 0) {
    sprintf(newcode, "\n%s#ifdef __PD_DINERO_\n%s", spaces(indent), spaces(indent));
    newcode += strlen(newcode);

    for (i = 0; i < ndests; i++) {
	sprintf(newcode, "dest%d.pending = 0;\n%s", i+1, spaces(indent));
	newcode += strlen(newcode);
    }
    
    sprintf(newcode, "#endif /* __PD_DINERO_ */\n\n%s", spaces(indent));
    newcode += strlen(newcode);
  }
  /* } WWF - Oct 23, 2003 */
#endif /* } RMR */
  /* } RMR */

  for (; *code; ) {
	if (*code == ';' && nest == 0) {
	  *newcode++ = *code++;
	  while (isspace(*code))
		code++;
	  /* initialize temp destination registers for CMPP instructions */
	  if (is_cmpp && !dest_init) {
		dest_init = 1;
		sprintf(newcode, "\n\n%s", spaces(indent));
		newcode += strlen(newcode);
		for (i = 0; i < ndests; i++) {
		  sprintf(newcode, "dest[%d].reg.pr = SREG(OP_DEST%d(op)).reg.pr;\n%s", 
				  i, i, spaces(indent));
		  newcode += strlen(newcode);
		}
		newcode += strlen(newcode);
	  }
	  if (*code != '}') {
		*newcode++ = '\n';
		strcpy(newcode, spaces(indent));
		newcode += indent;
		continue;
	  }
	}
	if (*code == '{') {
	  *newcode++ = *code++;
	  while (isspace(*code))
		code++;
	  *newcode++ = '\n';
	  indent += 3;
	  strcpy(newcode, spaces(indent));
	  newcode += indent;
	  continue;
	}
	if (*code == '}') {
	  indent -= 3;
	  *newcode++ = '\n';
	  strcpy(newcode, spaces(indent));
	  newcode += indent;
	  *newcode++ = *code++;
	  *newcode++ = '\n';
	  while (isspace(*code))
		code++;
	  strcpy(newcode, spaces(indent));
	  newcode += indent;
	  continue;
	}

	if (*code == '(') nest++;
	if (*code == ')') nest--;
	if (!isalpha(*code)) {
	  *newcode++ = *code++;
	  continue;
	}
#if 0 /* this is no longer neessary - RMR 08292001 */
	if (!strncmp(code, "PD_", 3)) {
	  while (*code && (isalnum(*code) || *code == '_'))
		*newcode++ = *code++;
	  strcpy(newcode, ".reg.cr");
	  newcode += strlen(newcode);
	  continue;
	}
#endif
	if (!strncmp(code, "src", 3) && isdigit(*(code+3))) {
	  code += 3;
	  for (i = 0; *code && isdigit(*code); code++)
		i = i * 10 + (*code - '0');
	  i--;
	  if (src[i] == 'F') {
		if (*code++ != '.') {
#ifdef _DEBUG_
		  DEBUG("Missing FP format\n");
#endif
		  exit(1);
		}
		mod = *code++;
	  }
	  port2field(newcode, "src", i, src[i], mod);
	  newcode += strlen(newcode);
#ifdef _DEBUG_
	  DEBUG("\t\t\tSrc[%d]: '%c'\n", i, src[i]);
#endif
	  continue;
	}
	if (!strncmp(code, "dest", 4) && isdigit(*(code+4))) {
	  code += 4;
	  for (i = 0; *code && isdigit(*code); code++)
		i = i * 10 + (*code - '0');
	  i--;
	  if (dest[i] == 'F' || dest[i] == 'H' || dest[i] == 'U' || dest[i] == 'D') {
		if (*code++ != '.') {
#ifdef _DEBUG_
		  DEBUG("Missing FP format\n");
#endif
		  exit(1);
		}
		mod = *code++;
	  }
	  dest[i] = dest2field(newcode, i, dest[i], mod);
	  newcode += strlen(newcode);
#ifdef _DEBUG_
	  DEBUG("\t\t\tDest[%d]: '%c'\n", i, dest[i]);
#endif
	  continue;
	}
	if (!strncmp(code, "pred", 4)) {
	  code += 4;
	  sprintf(newcode, "CHKLOCK(SREG(OP_PRED(op)))->reg.pr");
	  newcode += strlen(newcode);
#ifdef _DEBUG_
	  DEBUG("\t\t\tPred: 'P'\n");
#endif
	  continue;
	}
	while (*code && (isalnum(*code) || *code == '_'))
	  *newcode++ = *code++;
  }

  sprintf(newcode, ";\n");
  newcode += strlen(newcode);

  sprintf(newcode, "%s", spaces(indent));
  newcode += strlen(newcode);

  sprintf(newcode, "PD_trace_op(op, \"%s\", \"%s\", \"%s\", dest);\n",
          opcode, src, dest);
  newcode += strlen(newcode);
  /* RMR { don't generate delay() calls for custom functions */
  if (!custom) {
    for (i = 0; i < ndests; i++) {
	sprintf(newcode, "%s", spaces(indent));
	newcode += strlen(newcode);

	/* The EQUALS code */
	if ( dest[i] == 'D') {
	  if(is_cmpp) {
		sprintf(newcode, "\n%sif(", spaces(indent));
		newcode += strlen(newcode);
		dest2field(newcode, i, dest[i], 'S');
		newcode += strlen(newcode);
		sprintf(newcode, " != ");
		newcode += strlen(newcode);
		port2field(newcode, "dest", i, dest[i], 'S');
		newcode += strlen(newcode);
		sprintf(newcode, ")\n%s", spaces(3 + indent));
		newcode += strlen(newcode);
	  }
	  sprintf(newcode, "__PD_delay2x(&");
	}
	else if (dest[i] == 'B') {
	  if(is_cmpp) {
		sprintf(newcode, "\n%sif(", spaces(indent));
		newcode += strlen(newcode);
		dest2field(newcode, i, dest[i], 'S');
		newcode += strlen(newcode);
		sprintf(newcode, " != ");
		newcode += strlen(newcode);
		port2field(newcode, "dest", i, dest[i], 'S');
		newcode += strlen(newcode);
		sprintf(newcode, ")\n%s", spaces(3 + indent));
		newcode += strlen(newcode);
	  }
	  sprintf(newcode, "__PD_delay2x(&");
	}
	else {
	  if(is_cmpp) {
		sprintf(newcode, "\n%sif(", spaces(indent));
		newcode += strlen(newcode);
		dest2field(newcode, i, dest[i], 'S');
		newcode += strlen(newcode);
		sprintf(newcode, " != ");
		newcode += strlen(newcode);
		port2field(newcode, "dest", i, dest[i], 'S');
		newcode += strlen(newcode);
		sprintf(newcode, ")\n%s", spaces(3 + indent));
		newcode += strlen(newcode);
	  }
	  sprintf(newcode, "__PD_delay(&");
	}
	newcode += strlen(newcode);
	dest2field(newcode, i, dest[i], 'S');
	newcode += strlen(newcode);
	sprintf(newcode, ", &");
	newcode += strlen(newcode);
	port2field(newcode, "dest", i, dest[i], 'S');
	newcode += strlen(newcode);
	/* RMR { for interlocking */
	sprintf(newcode, ", &");
	newcode += strlen(newcode);
	port_pending_bit(newcode, i, 0);
	newcode += strlen(newcode);
	/* } RMR */
	sprintf(newcode, ", LAT(OP_DEST%d(op)));\n", i);
	newcode += strlen(newcode);
    }
  }
   
  indent -= 3;
  sprintf(newcode, "}\n");
  newcode += strlen(newcode);
}

/* RMR { adding support for vector file type and vector operations; as well as custom functions } */
void generateFunction(char *name, char *opcode, char *desc, char *parm, char *code, int pred, int vectorize, int custom, int vmasked,
                      int nsrc, char src[], int ndest, char dest[]) {
  char newcode[MAX_LINE * 2];
  bool is_cmpp;

  /* WWF - Oct 23, 2003 { */
  char _branch_str[1024];
  char *_p;
  /* } WWF - Oct 23, 2003 */

#ifdef _DEBUG_
  int i;

  DEBUG("\tName: '%s'\n", name);
  DEBUG("\tParms: '%s'\n", parm);
  DEBUG("\tDescr: '%s'\n", desc);
  DEBUG("\tOld code: '%s'\n", code);

  DEBUG("\tnsrc=%d\n", nsrc);
  for (i = 0; i < nsrc; i++)
	DEBUG("\t\tsrc[%d]='%c'\n", i, src[i]);

  DEBUG("\tndest=%d\n", ndest);
  for (i = 0; i < ndest; i++)
	DEBUG("\t\tdest[%d]='%c'\n", i, dest[i]);
#endif

  is_cmpp = 0;

   /* CMPP has a slightly different mechanism for commiting the outputs */
   /* See Page 21, PlayDoh architecture manual */
  if(strstr(name, "CMPP"))
	is_cmpp = 1;

  /* RMR { adding support for vector file type and vector operations */
  if (vectorize) 
    generateVBody(newcode, nsrc, src, ndest, dest, code, vectorize, vmasked);
  else
    generateBody(newcode, opcode, src, ndest, dest, code, is_cmpp, custom);
  /* } RMR */

  /* WWF - Oct 23, 2003 { */
  _p = strstr(newcode, "handle_branch");
  if (_p) {
    int i = 0;
    while (*_p != ';') {
	_branch_str[i] = *_p;
	_p++; i++;
    }
    _branch_str[i-1] = 0;
  }
  /* } - WWF - Oct 23, 2003 */

#ifdef _DEBUG_
  DEBUG("\tNew code: '%s'\n", newcode);
#endif

  fprintf(hdrfile, "/* %s */\n", desc);
  fprintf(hdrfile, "void __PD_%s(__PD_OP *);\n", name);


  fprintf(srcfile, "/* %s */\n", desc);
  fprintf(srcfile, "/* %s */\n", parm);
  fprintf(srcfile, "void __PD_%s(__PD_OP *op) %s\n", name, newcode);

   /* The predicate input behaviour for CMPP is different than that for */
   /* other operations. A predicated version need not be created */
   /* See Page 21, Architecture Manual */
  if (pred && !is_cmpp) {
	fprintf(hdrfile, "/* %s (predicated version) */\n", desc);
	fprintf(hdrfile, "void __PD_%s_pred(__PD_OP *);\n", name);

	fprintf(srcfile, "/* %s */\n", desc);
	fprintf(srcfile, "/* Predicated version */\n");
	fprintf(srcfile, "/* %s */\n", parm);
	fprintf(srcfile, "void __PD_%s_pred(__PD_OP *op) {\n", name);

	/* WWF - Oct 23, 2003 { */
	if (_p) {
	  strcat(_branch_str, " && CHKLOCK(SREG(OP_PRED(op)))->reg.pr);");
	  fprintf(srcfile, "\n#ifdef __PD_DINERO_\n");
	  fprintf(srcfile, "      %s\n", _branch_str);
	  fprintf(srcfile, "      skip = 1;\n");
	  fprintf(srcfile, "#endif /* __PD_DINERO_ */\n\n");
	}
	/* } WWF - Oct 23, 2003 */

	fprintf(srcfile, "   if (CHKLOCK(SREG(OP_PRED(op)))->reg.pr) __PD_%s(op);\n", name);
#ifdef NO_INLINE
	fprintf(srcfile, "   else __PD_no_op(op);\n");
#else
	fprintf(srcfile, "   else {\n");
	fprintf(srcfile, "      if (__PD_parameters->flags & __PD_DYNSMASK) __PD_statistics->nullified++;\n");
	fprintf(srcfile, "#ifdef __PD_TRACE_\n");
	fprintf(srcfile, "      if (__PD_parameters->flags & __PD_CFGTMASK) __PD_trace_op_null(op);\n");
	fprintf(srcfile, "#endif /* __PD_TRACE_ */\n");
	fprintf(srcfile, "   }\n");
#endif

	/* WWF - Oct 23, 2003 { */
	if (_p) {
	  fprintf(srcfile, "\n#ifdef __PD_DINERO_\n");
	  fprintf(srcfile, "      skip = 0;\n");
	  fprintf(srcfile, "#endif /* __PD_DINERO_ */\n");
	}
	/* } WWF - Oct 23, 2003 */

	fprintf(srcfile, "}\n");
	fprintf(srcfile, "\n");
  }

  fflush(srcfile);
  fflush(hdrfile);
}

void processTemplate(char *line) {
  char string[MAX_LINE];
  int  i;
  char sources[__PD_MAX_SRC][MAX_LINE];
  int  nsources;
  char dests[__PD_MAX_DEST][MAX_LINE];
  int  ndests;
  char *dsts, *srcs;
  char *p;
  char name[MAX_LINE];
  char opcode[MAX_LINE];
  char *tail;
  char *parm;
  char newparm[MAX_LINE];
  char descr[MAX_LINE];
  char code[MAX_LINE];
  char _srcs[50], _dsts[50];
  int pred;
  /* RMR { adding support for vector file type and vector operations */
  int vectorize = 0;
  int custom    = 0;
  int vmasked   = 0;
  /* } RMR */

#ifdef _DEBUG_
  DEBUG("Template: %s\n", line);
#endif

  memset(_srcs, 0, 50);
  memset(_dsts, 0, 50);

  strncpy(string, line, MAX_LINE);
  strsub(name, strtok(string, "\t\n"));
  strcpy(opcode, name);
  tail = name + strlen(name);
  parm = strtok(NULL, "\t\n");
  strsub(descr, strtok(NULL, "\t\n"));
  strsub(code, strtok(NULL, "\t\n"));
  pred = 0;

  /* RMR { adding support for vector file type and vector operations */
  if (*parm == 'V') {
    vectorize = 1;
    parm++;
  }
  if (*parm == 'C') {
    custom = 1;
    parm++;
  }
  if (*parm == 'M') {
    vmasked = 1;
    parm++;
  }
  /* } RMR */
  if (*parm == '?') {
	pred = 1;
	parm++;
  }
#ifdef _DEBUG_
  DEBUG("\tName: '%s'\n", name);
  DEBUG("\tParms: '%s'\n", parm);
  DEBUG("\tDescr: '%s'\n", descr);
  DEBUG("\tOld code: '%s'\n", code);
#endif
  strncpy(newparm, parm, MAX_LINE);
  srcs = newparm;
  *(p = strchr(srcs, ':')) = '\0';
  dsts = strtok(p+1, ";");
  nsources = 0;
  for (p = strtok(srcs, ","); p; p = strtok(NULL, ",")) {
	strncpy(sources[nsources++], p, MAX_LINE);
  }
#ifdef _DEBUG_
  DEBUG("\tnsources=%d\n", nsources);
  for (i = 0; i < nsources; i++)
	DEBUG("\t\tsources[%d]='%s'\n", i, sources[i]);
#endif
  ndests = 0;
  for (p = strtok(dsts, ","); p; p = strtok(NULL, ",")) {
	strncpy(dests[ndests++], p, MAX_LINE);
  }
#ifdef _DEBUG_
  DEBUG("\tndests=%d\n", ndests);
  for (i = 0; i < ndests; i++)
	DEBUG("\t\tdests[%d]='%s'\n", i, dests[i]);
#endif

  {
	static struct LOOP params;
	char svar[20];
	char __src[400];
	char *__s;
	char *src[20];
	int nsrc;
	int lit, reg;

	loop_init(&params);
	for (i = 0; i < ndests; i++)
	  _dsts[i] = *dests[i];
	for (i = 0; i < nsources; i++) {
	  __s = __src;
	  nsrc = 0;
	  reg = lit = 0;
	  for (p = sources[i]; *p; p++) {
		if (!reg && *p != 'L') {
		  src[nsrc++] = __s;
		  *__s++ = *p;
		  *__s++ = '\0';
		  reg = 1;
		}
		if (!lit && *p == 'L') {
		  src[nsrc++] = __s;
		  *__s++ = *p;
		  *__s++ = '\0';
		  lit = 1;
		}
	  }
	  sprintf(svar, "s%d", i);
	  loop_add(&params, svar, nsrc, src);
	}
	for (; loop_more(&params); loop_next(&params)) {
	  for (i = 0; i < nsources; i++) {
		sprintf(svar, "s%d", i);
		p = subst_lookup(&params.substmap, svar);
		_srcs[i] = *p;
	  }
	  p = tail;
	  for (i = 0; i < nsources; i++) {
		if (_srcs[i] == 'L')
		  sprintf(p, "_lit");
		else
		  sprintf(p, "_reg");
		p += strlen(p);
	  }
	  /* RMR { adding support for vector file type and vector operations;
	   * (pass vectorize, custom, vmasked, flag to generateFunction)
	   * also adding support for custom function which won't require delay() calls
	   * (pass custom flag to generateFunction)
	   */
	  generateFunction(name, opcode, descr, parm, code, pred, vectorize, custom, vmasked,
				 nsources, _srcs, ndests, _dsts);
	  /* } RMR */
	}
  }
  if (nsources == 0)
    /* RMR { adding support for vector file type and vector operations;
     * (pass vectorize, custom, vmasked, flag to generateFunction)
     */
    generateFunction(name, opcode, descr, parm, code, pred, vectorize, custom, vmasked,
			   nsources, _srcs, ndests, _dsts);
    /* } RMR */
}

void executeLoop(char *line) {
  for (; loop_more(&loopmap); loop_next(&loopmap))
	processTemplate(line);
  loop_init(&loopmap);     /* reset the loop state */
}

void processLine(char *line) {
  switch (*line) {
  case '[':    /* new section */
	*strchr(line+1, ']') = '\0';
	processSection(line+1, C_file);
	break;
  case '<':    /* new header file section */
	*strchr(line+1, '>') = '\0';
	processSection(line+1, header);
	break;
  case '\0':
  case '\n':   /* blank string */
	if(srcfile) {
	  fprintf(srcfile, "%s", line);
	  fprintf(hdrfile, "%s", line);
	}
	break;
  case '#':    /* comment */
	*strchr(line+1, '\n') = ' ';
	if(srcfile) {
	  fprintf(srcfile, "/*%s*/\n", line+1);
	  fprintf(hdrfile, "/*%s*/\n", line+1);
	}
	break;
  case '\\':   /* meta command */
	if (!strncmp(line+1, "INIT ", 5)) {
	  processInit(srcfile, line+6);
	} else if (!strncmp(line+1, "LOOP ", 5)) {
	  processLoop(line+6);
	} else {
#ifdef _DEBUG_
	  DEBUG("Invalid command '%s'\n", line+1);
#endif
	  exit(1);
	}
	break;
  default:     /* regular template */
	if (loop_empty(&loopmap))         /* if we're not in a loop */
	  processTemplate(line);
	else
	  executeLoop(line);
  }
  fflush(srcfile);
  fflush(hdrfile);
}

int main(int argc, char **argv) {
  char string[MAX_LINE];

  infile = stdin;
  if (argc > 1)
	infile = fopen(argv[1], "r");

  openheader("PD_ops.h", "Simulator operations");

  loop_init(&loopmap);
  while (fgets(string, MAX_LINE, infile)) {
#ifdef _DEBUG_
	DEBUG("%s", string);
#endif
	processLine(string);
  }

  closesource();
  closeheader();
  if (infile != stdin)
	fclose(infile);

  return 0;
}

/* RMR { adding support for vector file type and vector operations */
void generateVBody(char *newcode, int nsrcs, char src[], int ndests, char dest[], char *code, int vectorsize, int vmasked) {
  char mod = 0;
  int i, indent = 0, nest = 0;
  int voffset = 0;

  if (vectorsize == 0) {
	fprintf (stderr, "generateVBody should only be called when vectorizing instructions.\n");
	exit(1);
  }

  if (!code || !*code) {
    strcpy(newcode, "{\n}\n");
    return;
  }
  
  indent += 3;
  sprintf(newcode, "{\n");
  newcode += strlen(newcode);

  if (!strncmp(code, "__load", 6) || !strncmp(code, "__store", 7)) {
    sprintf(newcode, "%s", spaces(indent));
    newcode += strlen(newcode);
    while (*code != ';')
	*newcode++ = *code++;
    *newcode++ = ';';
    *newcode++ = '\n';
    code++;
    while (isspace(*code))
	code++;
  }
  
  sprintf(newcode, 
	    "%sint i;\n"
	    "%sfor (i = 0; i < __PD_VECTOR_LENGTH; i++) {\n", spaces(indent), spaces(indent));
  newcode += strlen(newcode);
  indent  *= 2;

  // masked vector operations
  if (vmasked) {
    sprintf(newcode,
		"%sif (SREG(OP_SRC%d(op)).reg.gpr & (1 << i)) {\n", spaces(indent), nsrcs - 1);
    newcode += strlen(newcode);  
    indent  *= 2;
  }

  for (i = 0; i < ndests; i++) {
    sprintf(newcode, "%s__PD_reg dest%d;\n", spaces(indent), i+1);
    newcode += strlen(newcode);
  }

  sprintf(newcode, "%s", spaces(indent));
  newcode += strlen(newcode);
  
  for (; *code; ) {
    if (*code == ';' && nest == 0) {
	*newcode++ = *code++;
	while (isspace(*code))
	  code++;
	if (*code != '}') {
	  *newcode++ = '\n';
	  strcpy(newcode, spaces(indent));
	  newcode += indent;
	  continue;
	}
    }
    if (*code == '{') {
	*newcode++ = *code++;
	while (isspace(*code))
	  code++;
	*newcode++ = '\n';
	indent += 3;
	strcpy(newcode, spaces(indent));
	newcode += indent;
	continue;
    }
    if (*code == '}') {
	indent -= 3;
	*newcode++ = '\n';
	strcpy(newcode, spaces(indent));
	newcode += indent;
	*newcode++ = *code++;
	*newcode++ = '\n';
	  while (isspace(*code))
	    code++;
	  strcpy(newcode, spaces(indent));
	  newcode += indent;
	  continue;
    }
    
    if (*code == '(') nest++;
    if (*code == ')') nest--;
    
    if (*code == '@') {
	code++;
	// sprintf(newcode, "%d", voffset);
	sprintf(newcode, "i");
	newcode += strlen(newcode);
	continue;
    }
    
    if (!isalpha(*code)) {
	*newcode++ = *code++;
	continue;
    }
    
    if (!strncmp(code, "src", 3) && isdigit(*(code+3))) {
	code += 3;
	for (i = 0; *code && isdigit(*code); code++)
	  i = i * 10 + (*code - '0');
	i--;
	if (src[i] == 'F') {
	  if (*code++ != '.') {
#ifdef _DEBUG_
	    DEBUG("Missing FP format\n");
#endif
	    exit(1);
	  }
	  mod = *code++;
	}
	port2field(newcode, "src", i, src[i], mod);
	newcode += strlen(newcode);
#ifdef _DEBUG_
	DEBUG("\t\t\tSrc[%d]: '%c'\n", i, src[i]);
#endif
	continue;
    }
    if (!strncmp(code, "vsrc", 4) && isdigit(*(code+4))) {
	code += 4;
	for (i = 0; *code && isdigit(*code); code++)
	  i = i * 10 + (*code - '0');
	i--;
	if (src[i] == 'F') {
	  if (*code++ != '.') {
#ifdef _DEBUG_
	    DEBUG("Missing FP format\n");
#endif
	    exit(1);
	  }
	  mod = *code++;
	}
	port2Vfield(newcode, "src", i, src[i], mod, voffset);
	newcode += strlen(newcode);
#ifdef _DEBUG_
	// DEBUG("\t\t\tVSrc[%d][%d]: '%c'\n", i, voffset, src[i]);
	DEBUG("\t\t\tVSrc[%d][@]: '%c'\n", i, src[i]);
#endif
	continue;
    }
    if (!strncmp(code, "dest", 4) && isdigit(*(code+4))) {
	// RMR sanity check; I don't want this allowed
	fprintf (stderr, "scalar dest not allowed for vector operations!\n");	  
	exit(1);
	
	code += 4;
	for (i = 0; *code && isdigit(*code); code++)
	  i = i * 10 + (*code - '0');
	i--;
	if (dest[i] == 'F' || dest[i] == 'H' || dest[i] == 'U' || dest[i] == 'D') {
	  if (*code++ != '.') {
#ifdef _DEBUG_
	    DEBUG("Missing FP format\n");
#endif
	    exit(1);
	  }
	  mod = *code++;
	}
	dest[i] = dest2field(newcode, i + 1, dest[i], mod);
	newcode += strlen(newcode);
#ifdef _DEBUG_
	DEBUG("\t\t\tDest[%d]: '%c'\n", i, dest[i]);
#endif
	continue;
    }
    if (!strncmp(code, "vdest", 5) && isdigit(*(code+5))) {
	code += 5;
	for (i = 0; *code && isdigit(*code); code++)
	  i = i * 10 + (*code - '0');
	i--;
	if (dest[i] == 'F' || dest[i] == 'H' || dest[i] == 'U' || dest[i] == 'D') {
	  if (*code++ != '.') {
#ifdef _DEBUG_
	    DEBUG("Missing FP format\n");
#endif
	    exit(1);
	  }
	  mod = *code++;
	}
	dest[i] = dest2Vfield(newcode, i + 1, dest[i], mod, voffset);
	newcode += strlen(newcode);
#ifdef _DEBUG_
	// DEBUG("\t\t\tVDest[%d][%d]: '%c'\n", i, voffset, src[i]);
	DEBUG("\t\t\tVDest[%d][@]: '%c'\n", i, dest[i]);
#endif
	continue;
    }
    if (!strncmp(code, "pred", 4)) {
	code += 4;
	sprintf(newcode, "CHKLOCK(SREG(OP_PRED(op)))->reg.pr");
	newcode += strlen(newcode);
#ifdef _DEBUG_
	DEBUG("\t\t\tPred: 'P'\n");
#endif
	continue;
    }
    while (*code && (isalnum(*code) || *code == '_'))
	*newcode++ = *code++;
  }
  
  sprintf(newcode, ";\n");
  newcode += strlen(newcode);
  
  for (i = 0; i < ndests; i++) {
    sprintf(newcode, "%s", spaces(indent));
    newcode += strlen(newcode);
  
    if ( dest[i] == 'D') {
	sprintf(newcode, "__PD_delay2x(&");
    }
    else {
	sprintf(newcode, "__PD_delay(&");
    }
    newcode += strlen(newcode);
    dest2Vfield(newcode, i + 1, dest[i], 'S', voffset);
    newcode += strlen(newcode);
    sprintf(newcode, ", &");
    newcode += strlen(newcode);
    port2Vfield(newcode, "dest", i, dest[i], 'S', voffset);
    newcode += strlen(newcode);
    /* RMR { for interlocking */
    sprintf(newcode, ", &");
    newcode += strlen(newcode);
    port_pending_bit(newcode, i, voffset + 1);
    newcode += strlen(newcode);
    /* } RMR */
    sprintf(newcode, ", LAT(OP_DEST%d(op)));\n", i);
    newcode += strlen(newcode);
  }
  
  // masked vector operations
  if (vmasked) {
    indent /= 2;
    sprintf(newcode, "%s}\n", spaces(indent));
    newcode += strlen(newcode);
  }

  indent /= 2;
  sprintf(newcode, "%s}\n", spaces(indent));
  newcode += strlen(newcode);

  indent -= 3;
  newcode += strlen(newcode);
  sprintf(newcode, "}\n");
  newcode += strlen(newcode);
}

char port2Vfield(char *field, char *port, int num, char type, char mod, int voffset) {
  if (type == 'L') {
	sprintf(field, "LIT(OP_%s%d(op))", strupcase(port), num);
  }
  else {
    if (strcmp(port, "dest")) {
      // sprintf(field, "VREG((OP_%s%d(op)), %d).reg.", strupcase(port), num, voffset);
	sprintf(field, "CHKLOCK(VREG((OP_%s%d(op)), i))->reg.", strupcase(port), num);
    }
    else {
	sprintf(field, "VREG((OP_%s%d(op)), i).reg.", strupcase(port), num);
    }
  }
  field += strlen(field);
  return type2field(field, type, mod);
}

char dest2Vfield(char *field, int num, char type, char mod, int voffset) {
  if (type == 'L') {
#ifdef _DEBUG_
	DEBUG("Invalid destination type: '%c'(%d)\n", type, type);
#endif
	exit(1);
  }
  //  sprintf(field, "dest%d[%d].reg.", num, voffset);
  sprintf(field, "dest%d.reg.", num);
  field += strlen(field);
  return type2field(field, type, mod);
}

/* } RMR */
