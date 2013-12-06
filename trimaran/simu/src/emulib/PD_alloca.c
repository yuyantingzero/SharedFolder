/****************************************************************************** 
 * File:    PD_alloca.c 
 * Authors: Nate Clark 
 *****************************************************************************/ 
 
#include "PD.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern void* S_malloc(int);

 
void* __PD_alloca(int size) { 
  void* ptr = malloc (size);

  if(ptr == NULL) {
    fprintf(stderr, "alloca: out of memory!\n");
    exit(-1);
  }

  return ptr; 
} 



