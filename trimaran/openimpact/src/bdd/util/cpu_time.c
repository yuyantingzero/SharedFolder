/* LINTLIBRARY */

/* 10/29/02 REK Adding config.h */
#include <config.h>
#include <stdio.h>
#include <bdd/util.h>

#ifdef IBM_WATC		/* IBM Waterloo-C compiler (same as bsd 4.2) */
#define void int
#define BSD
#endif

#ifdef BSD
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

#ifdef UNIX60
#include <sys/types.h>
#include <sys/times.h>
#endif

#ifdef UNIX100
#include <sys/types.h>
#include <sys/times.h>
#endif

#ifdef vms		/* VAX/C compiler -- times() with 100 HZ clock */
#include <types.h>
#include <time.h>
#endif



/*
 *   util_cpu_time -- return a long which represents the elapsed processor
 *   time in milliseconds since some constant reference
 */
long 
util_cpu_time()
{
    long t = 0;

#ifdef BSD
    struct rusage rusage;
    (void) getrusage(RUSAGE_SELF, &rusage);
    t = (long) rusage.ru_utime.tv_sec*1000 + rusage.ru_utime.tv_usec/1000;
#endif

#ifdef IBMPC
    long ltime;
    (void) time(&ltime);
    t = ltime * 1000;
#endif

#ifdef UNIX60			/* times() with 60 Hz resolution */
    struct tms buffer;
    times(&buffer);
    t = buffer.tms_utime * 16.6667;
#endif

#ifdef UNIX100
    struct tms buffer;		/* times() with 100 Hz resolution */
    times(&buffer);
    t = buffer.tms_utime * 10;
#endif

#ifdef vms
    tbuffer_t buffer;	/* times() with 100 Hz resolution */
    times(&buffer);
    t = buffer.proc_user_time * 10;
#endif

    return t;
}
