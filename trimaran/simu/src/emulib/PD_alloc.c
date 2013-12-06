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

Copyright  2003  Massachusetts  Institute  of Technology.  All  rights
reserved by the foregoing, respectively.

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
 * File:    PD_alloc.c
 * Authors: Weng-Fai Wong
 *
 * Description: modified malloc.c - obtained from Mark B. Hanson
 *              http://www.panix.com/~mbh/malloc/malloc.tar.gz
 *****************************************************************************/

/*
** IDEAS:
**	- keep one link of the list for "sorted by address",
**	  another for for "sorted by size"
**	- mallopt options to turn NDEBUG, DISPLAY, etc. on and off
**	  and to change FIT function
**	  and to switch to functions with no error checking
**	  and to print statistics
**	- return freed memory over to OS if there's enough of it at the
**	  end of the segment
**	- X utility to show fragmentation
**	- specify which file to put output in
**	- output how many times each function has been called
*/


#define _POSIX_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <sys/types.h>
#include <limits.h>
#include <values.h>

#define NDEBUG
#include <assert.h>

#undef OBLITERATE
#undef DISPLAY
#undef BOUNDS
#undef ZERO_ERROR

#define FALSE 0
#define TRUE 1

/* #define FIT(x) bestfit(x) */
#define FIT(x) firstfit(x)

#define PATTERN(p) ((char)((unsigned int)(p) & 0xff))
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define LOCATION(f,l) \
	if ((f) != NULL) { \
	    fprintf(stderr, " at `%s:%d'\n", (f), (l)); \
	} else { \
	    fprintf(stderr, "\n"); \
	}

#define ALIGN(p) (((p) + (sizeof(align_t) - 1)) & ~(sizeof(align_t) - 1))

#ifdef BOUNDS
#define ADDR(n) ((void*)(n) + sizeof(node) + sizeof(align_t))
#define AFTER(n) (ADDR(n) + (n)->x.rsize + sizeof(align_t))
#define OVERHEAD (sizeof(node) + (2 * sizeof(align_t)))
#else
#define ADDR(n) ((void*)(n) + sizeof(node))
#define AFTER(n) (ADDR(n) + (n)->x.rsize)
#define OVERHEAD (sizeof(node))
#endif /* BOUNDS */

#ifdef DISPLAY
#define STATUS(s) \
    fprintf(stderr, \
	"%8d %05ldu/%04ldf %4.1f%% (%7s) <%24s:%-5d>\r", \
	total, \
	alloc_len, \
	avail_len, \
	(avail_len + alloc_len) ? \
	    avail_len * 99.9 / (avail_len + alloc_len) : \
	    0.0, \
	(s), \
	((file) ? (file) : "unknown"), \
	((line) ? (line) : 0))
#else
#define STATUS(s)
#endif /* DISPLAY */

#define MINNODE (64 * sizeof(align_t))

#define ADJACENT(n1,n2) \
	((void*)(n1) + OVERHEAD + (n1)->x.rsize == (void*)(n2))


extern void* sbrk();
extern int fputs();
extern int getpagesize();


typedef double align_t;

union _node {
    struct {
	int rsize; /* real size */
	int usize; /* user size */
	char *file;
	unsigned int line;
	union _node *next;
    } x;
    align_t align;
};
typedef union _node node;

enum _list {
    AVAILABLE,
    ALLOCATED
};
typedef enum _list list;


static node *available = NULL;
static node *allocated = NULL;
static unsigned long total = 0;
static unsigned long alloc_len = 0;
static unsigned long avail_len = 0;

/* WWF { this variable was moved to global scope to allow malloc and calloc calls in any order */
static int once = 0;
/* } WWF */

int pagesize = 0;


/* put a breakpoint here for debugging */
static inline void debug() {
  total = total;
} /* debug */


static void pnode(n)
     node *n;
{
  fprintf(stderr,
	    "0x%x: rsize = %u, usize = %u, addr = 0x%x, file = %s:%d, next = 0x%x\n",
	    (unsigned int)n,
	    n->x.rsize,
	    n->x.usize,
	    (unsigned int)ADDR(n),
	    n->x.file ? n->x.file : "unknown",
	    n->x.line,
	    (unsigned int)n->x.next);
} /* pnode */


static void plist(head)
     node *head;
{
  node *c;

  fprintf(stderr, "\n=== list starting at 0x%x: ===\n", (unsigned int)head);

  c = head;
  while (c != NULL) {
    pnode(c);
    c = c->x.next;
  }
} /* plist */


static unsigned long llength(head)
     node *head;
{
  unsigned long i = 0;
  node *c;

  c = head;
  while (c != NULL) {
    i++;
    c = c->x.next;
  }
  return i;
} /* llength */


static void delete(l, n)
     list l;
     node *n;
{
  node *c, *p, **head;

  assert(l == ALLOCATED || l == AVAILABLE);
  assert(n != NULL);

  head = (l == ALLOCATED) ? &allocated : &available;
  assert(*head != NULL);

  assert(llength(allocated) == alloc_len);
  assert(llength(available) == avail_len);

  c = p = *head;
  while ((c != NULL) && (ADDR(c) > ADDR(n))) {
    p = c;
    c = c->x.next;
  }

  assert(c != NULL);

  if (l == ALLOCATED) {
    alloc_len--;
  } else {
    avail_len--;
  }

  if (c == p) {
    *head = c->x.next;
  } else {
    p->x.next = c->x.next;
  }
} /* delete */


static void insert(l, n)
     list l;
     node *n;
{
  node *c, *p, **head;

  assert(l == ALLOCATED || l == AVAILABLE);
  assert(n != NULL);

  head = (l == ALLOCATED) ? &allocated : &available;

  assert(llength(allocated) == alloc_len);
  assert(llength(available) == avail_len);

  c = p = *head;
  while ((c != NULL) && (ADDR(c) > ADDR(n))) {
    p = c;
    c = c->x.next;
  }

  assert(c == NULL || ADDR(c) != ADDR(n));

  if ((l == AVAILABLE) && (*head != NULL) && (c != NULL) && ADJACENT(c,n)) {
    c->x.rsize += OVERHEAD + n->x.rsize;
    if (ADJACENT(c,p)) {
	delete(AVAILABLE, p);
	c->x.rsize += OVERHEAD + p->x.rsize;
    }
  } else {
    if (l == ALLOCATED) {
	alloc_len++;
    } else {
	avail_len++;
    }
    n->x.next = c;
    if (c == p) {
	*head = n;
    } else {
	p->x.next = n;
	if ((l == AVAILABLE) && (*head != NULL) && ADJACENT(n,p)) {
	  delete(AVAILABLE, p);
	  n->x.rsize += OVERHEAD + p->x.rsize;
	}
    }
  }
} /* insert */


#ifdef BOUNDS
static void fillbounds(n)
     node *n;
{
  char *start;
  char pattern = PATTERN(n);
  int length, i;

  start = (char *)n + sizeof(node);
  length = sizeof(align_t);
  for (i = 0; i < length; i++) {
    start[i] = pattern;
  }

  start = (char *)n + sizeof(node) + sizeof(align_t) + n->x.usize;
  length = n->x.rsize - n->x.usize + sizeof(align_t);
  for (i = 0; i < length; i++) {
    start[i] = pattern;
  }
} /* fillbounds */


static void checkbounds(n, func)
     node *n;
     char *func;
{
  int i, start, end;
  unsigned char pattern = PATTERN(n);
  unsigned char current;
    
#define CHECK(i) \
	current = ADDR(n)[(i)]; \
	if (current != pattern) { \
	    fprintf(stderr, "\n%s: found `\\%03o' instead of `\\%03o' at 0x%x[%d].\nnode =", \
		      func, \
 	 	      current, \
		      pattern, \
		      (unsigned int)ADDR(n), \
		      (i)); \
	    pnode(n); \
	    debug(); \
	}

  start = -sizeof(align_t);
  end = 0;
  for (i = start; i < end; i++) {
    CHECK(i);
  }

  start = n->x.usize;
  end = n->x.rsize + sizeof(align_t);
  for (i = start; i < end; i++) {
    CHECK(i);
  }
} /* checkbounds */


static void lbounds(head)
     node *head;
{
  node *c;

  c = head;
  while (c != NULL) {
    checkbounds(c, "lbounds");
    c = c->x.next;
  }
} /* lbounds */
#endif /* BOUNDS */


static node *find(head, ptr)
     node *head;
     void* ptr;
{
  node *c;

  c = head;
  while((c != NULL) && (ADDR(c) > ptr)) {
    c = c->x.next;
  }

  if ((c == NULL) || (ADDR(c) != ptr)) {
    return NULL;
  }

  return c;
} /* find */

#define HUGE 128*1024*1024

static node *expand(size)
     int size;
{
  node *n;
  int big;

#ifdef BOUNDS
  lbounds(allocated);
#endif /* BOUNDS */

  if (pagesize == 0) {
    pagesize = getpagesize();
  }

  if (available == NULL) {
    big = HUGE;
    n = available = (node *)(malloc(big));

    assert(n);
    //    assert(big >= size + OVERHEAD);
    //    n = (node *)sbrk(big);

    if (n == (node *)0) {
	fprintf(stderr, "\nexpand: sbrk(%d) failed!\n", big);
	debug();
	return NULL;
    }

    n->x.rsize = big - OVERHEAD;
    n->x.next = NULL;
    n->x.file = __FILE__;
    n->x.line = __LINE__;

    return n;
  } else 
    return available;

} /* expand */


static node *firstfit(size)
     int size;
{
  node *c;
  int aligned = ALIGN(size);

#ifdef ZERO_ERROR
  assert(size != 0);
#endif /* ZERO_ERROR */

  c = available;
  while ((c != NULL) && (c->x.rsize < aligned)) {
    c = c->x.next;
  }

  if (c == NULL) {
    if ((c = expand(aligned)) == NULL) {
	return NULL;
    }
  } else {
    assert(find(allocated, ADDR(c)) == NULL);
    assert(find(available, ADDR(c)) == c);

    delete(AVAILABLE, c);
  }

  assert(find(allocated, ADDR(c)) == NULL);
  assert(find(available, ADDR(c)) == NULL);

  if (c->x.rsize >= aligned + OVERHEAD + MINNODE) {
    node *n;
    int leftover;
	
    leftover = c->x.rsize - aligned - OVERHEAD;
    c->x.rsize = aligned;

    n = (node *)AFTER(c);
    n->x.rsize = leftover;

    assert(find(allocated, ADDR(n)) == NULL);
    assert(find(available, ADDR(n)) == NULL);

    insert(AVAILABLE, n);

    assert(find(allocated, ADDR(n)) == NULL);
    assert(find(available, ADDR(n)) == n);
  }

  c->x.usize = size;

#ifdef BOUNDS
  fillbounds(c);
#endif /* BOUNDS */

  insert(ALLOCATED, c);

  assert(find(allocated, ADDR(c)) == c);
  assert(find(available, ADDR(c)) == NULL);

  return c;
} /* firstfit */


static node *bestfit(size)
     int size;
{
  node *c, *b;
  int aligned = ALIGN(size);
  int over, best = MAXINT;

#ifdef ZERO_ERROR
  assert(size != 0);
#endif /* ZERO_ERROR */

  b = NULL;
  c = available;
  while (c != NULL) {
    if (c->x.rsize >= size) {
	over = c->x.rsize - size;
	if ((over < best) || (c == available)) {
	  best = over;
	  b = c;
	}
    }
    c = c->x.next;
  }

  if (b == NULL) {
    if ((b = expand(aligned)) == NULL) {
	return NULL;
    }
  } else {
    assert(find(allocated, ADDR(b)) == NULL);
    assert(find(available, ADDR(b)) == b);

    delete(AVAILABLE, b);
  }

  assert(find(allocated, ADDR(b)) == NULL);
  assert(find(available, ADDR(b)) == NULL);

  if (b->x.rsize >= aligned + OVERHEAD + MINNODE) {
    node *n;
    int leftover;
	
    leftover = b->x.rsize - aligned - OVERHEAD;
    b->x.rsize = aligned;

    n = (node *)AFTER(b);
    n->x.rsize = leftover;

    assert(find(allocated, ADDR(n)) == NULL);
    assert(find(available, ADDR(n)) == NULL);

    insert(AVAILABLE, n);

    assert(find(allocated, ADDR(n)) == NULL);
    assert(find(available, ADDR(n)) == n);
  }

  b->x.usize = size;

#ifdef BOUNDS
  fillbounds(b);
#endif /* BOUNDS */

  insert(ALLOCATED, b);

  assert(find(allocated, ADDR(b)) == b);
  assert(find(available, ADDR(b)) == NULL);

  return b;
} /* bestfit */

/*---------------------------------------------------------------------------*/

static void* _Mark_B_Hanson_malloc(size, file, line)
     int size;
     char *file;
     unsigned int line;
{
  node *store;
  /* WWF { once is upgraded to global scope to allow malloc and calloc calls in any order */
  /* static int once = 0; */
  /* } WWF */

  if (!once) {
    once = 1;
    _Mark_B_Hanson_malloc(1, file, line);
  }

  if (size == 0) {
    //	fprintf(stderr, "\nmalloc: attempt to allocate 0 bytes");
    //	LOCATION(file, line);
#ifdef ZERO_ERROR
    //	if (file != NULL)
    //	    debug();
    return NULL;
#endif /* ZERO_ERROR */
  }

  store = FIT(size);

  if (store == NULL) {
    fprintf(stderr, "\nmalloc: unable to allocate %d bytes", size);
    LOCATION(file, line);
    debug();
    return NULL;
  }

#ifdef OBLITERATE
  memset(ADDR(store), '\001', size);
#endif /* OBLITERATE */

  store->x.file = file;
  store->x.line = line;

  total += size;
  STATUS("malloc ");

  return ADDR(store);
} /* _malloc */


static void* _Mark_B_Hanson_calloc(num, size, file, line)
     unsigned int num;
     int size;
     char *file;
     unsigned int line;
{
  node *store;
  unsigned int bytes;

  /* WWF { */
  if (!once) {
    once = 1;
    _Mark_B_Hanson_calloc(num, size, file, line);
  }
  /* } WWF */

  if ((num == 0) && (size == 0)) {
    fprintf(stderr, "\ncalloc: attempt to allocate 0 items of size 0");
    LOCATION(file, line);
#ifdef ZERO_ERROR
    if (file != NULL)
	debug();
    return NULL;
#endif /* ZERO_ERROR */
  }

  if (num == 0) {
    fprintf(stderr, "\ncalloc: attempt to allocate 0 items of size %d",
		size);
    LOCATION(file, line);
#ifdef ZERO_ERROR
    if (file != NULL)
	debug();
    return NULL;
#endif /* ZERO_ERROR */
  }

  if (size == 0) {
    fprintf(stderr, "\ncalloc: attempt to allocate %d items of size 0",
		num);
    LOCATION(file, line);
#ifdef ZERO_ERROR
    if (file != NULL)
	debug();
    return NULL;
#endif /* ZERO_ERROR */
  }

  bytes = num * size;

  store = FIT(bytes);

  if (store == NULL) {
    fprintf(stderr, "\ncalloc: unable to allocate %d bytes", bytes);
    LOCATION(file, line);
    debug();
    return NULL;
  }

  memset(ADDR(store), '\000', bytes);
  store->x.file = file;
  store->x.line = line;

  total += bytes;
  STATUS("calloc ");

  return ADDR(store);
} /* _calloc */


static void* _Mark_B_Hanson_realloc(ptr, size, file, line)
     void* ptr;
     int size;
     char *file;
     unsigned int line;
{
  node *t;
  node *store;

  if (ptr == NULL) {
    return _Mark_B_Hanson_malloc(size, file, line);
  }

  t = find(allocated, ptr);

  if (t == NULL) {
    fprintf(stderr,
		"\nrealloc: attempt to realloc unallocated memory (0x%x)",
		(unsigned int)ptr);
    LOCATION(file, line);
    debug();
    return NULL;
  }

#ifdef BOUNDS
  checkbounds(t, "realloc");
#endif /* BOUNDS */

  total += size - t->x.usize;
  STATUS("realloc");

  if (t->x.rsize >= size) {

#ifdef OBLITERATE
    if (t->x.rsize != size) {
	memset(ADDR(t) + t->x.usize, '\001', size - t->x.usize);
    }
#endif /* OBLITERATE */

    t->x.usize = size;
    t->x.file = file;
    t->x.line = line;

#ifdef BOUNDS
    fillbounds(t);
#endif /* BOUNDS */

    return ADDR(t);
  }

  store = FIT(size);

  if (store == NULL) {
    fprintf(stderr, "\nrealloc: unable to allocate %d bytes", size);
    LOCATION(file, line);
    debug();
    return NULL;
  }

  memcpy(ADDR(store), ADDR(t), MIN(t->x.usize, size));

  assert(find(available, ptr) == NULL);
  assert(find(allocated, ptr) == t);

  delete(ALLOCATED, t);
  insert(AVAILABLE, t);

  assert(find(allocated, ptr) == NULL);
  /*
  ** collapsing prevents us from checking:
  ** assert(find(available, ptr) == t);
  */

  store->x.file = file;
  store->x.line = line;

  return ADDR(store);
} /* _realloc */


static void _Mark_B_Hanson_free(ptr, file, line)
     void* ptr;
     char *file;
     unsigned int line;
{
  node *t;

  if (ptr == NULL) {
    fprintf(stderr, "\nfree: attempt to free NULL");
    LOCATION(file, line);
    if (file != NULL)
	debug();
    return;
  }

  t = find(allocated, ptr);

  if (t == NULL) {
    fprintf(stderr, "\nfree: attempt to free unallocated memory (0x%x)",
		(unsigned int)ptr);
    LOCATION(file, line);
    debug();
    return;
  }

#ifdef BOUNDS
  checkbounds(t, "free");
#endif /* BOUNDS */

  assert(find(allocated, ptr) == t);
  assert(find(available, ptr) == NULL);

  delete(ALLOCATED, t);
  insert(AVAILABLE, t);

  assert(find(allocated, ptr) == NULL);
  /*
  ** collapsing prevents us from checking:
  ** assert(find(available, ptr) == t);
  */

#ifdef OBLITERATE
  memset(ADDR(t), '\002', t->x.usize);
#endif /* OBLITERATE */

  total -= t->x.usize;
  STATUS("free   ");
} /* _free */


/*---------------------------------------------------------------------------*/
/*----------------------------- USER INTERFACE: -----------------------------*/
/*---------------------------------------------------------------------------*/


void* __PD_malloc(int size)
{
  return _Mark_B_Hanson_malloc((int)size, NULL , 0);
} /* malloc */


void* __PD_calloc(int num, int size)
{
  return _Mark_B_Hanson_calloc(num, (int)size, __FILE__, __LINE__);
} /* calloc */


void* __PD_realloc(void* ptr, int size)
{
  return _Mark_B_Hanson_realloc((void*)ptr, (int)size, __FILE__, __LINE__);
} /* realloc */


void __PD_free(void *ptr)
{
  _Mark_B_Hanson_free((void*)ptr, __FILE__, __LINE__);
} /* free */


void __PD_cfree(void *ptr)
{
  _Mark_B_Hanson_free((void*)ptr, __FILE__, __LINE__);
} /* cfree */

