#define WMOPS 0

/******************************************************************************

      Internal release for ETSI SMG2 Speech Experts Group

      GSM Enhanced Full Rate C source code v5.0.0 with the changes defined
      in the following Change Requests (presented and approved in ETSI
      SMG2-SEG #7 meeting in Nuremberg, 19-21.06.1996) implemented:

      Subject                                Source                 Date
      =========================================================================
      Moving of the pre-channel coding       Nokia                  19.06.1996
      from the 06 series to the 05 series

      Correction to Bit Exact Code           Motorola, Nokia        19.06.1996

      Initialisation of algebraic code-      TI, Nokia              19.06.1996
      book search

      L_Extract routine                      TI, Nokia              19.06.1996

      Correction to Bit Exact Code           Motorola               19.06.1996

      Removal of unused lines in error       Nokia                  19.06.1996
      concealment C code

      Copyright (C) European Telecommunications Standards Institute 1996

******************************************************************************/

#include <limits.h>

/*_____________________
 |                     |
 | Basic types.        |
 |_____________________|
*/

#if defined(__BORLANDC__) || defined(__WATCOMC__) || defined(_MSC_VER) || defined(__ZTC__)
typedef short Word16;
typedef long Word32;
typedef int Flag;

#elif defined(__unix__) || defined(__unix)
typedef short Word16;
typedef int Word32;
typedef int Flag;

#endif

#define MIN_16 SHRT_MIN
#define MAX_16 SHRT_MAX

#define MIN_31 (-1073741824L)
#define MAX_31 1073741823L

#define MIN_32 INT_MIN
#define MAX_32 INT_MAX

