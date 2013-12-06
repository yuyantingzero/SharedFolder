/* The Wavefront computation.
   First row and the column of the array is initialized to 1.0
   The remaining array is filled with the sum of west, north and
   northwest neighbor. The final answer is the sum of all the
   elements of the array. The parallelism in the computation is
   proportional to the size of the off-diagonal. 

   October 31, 1995
   Shail Aditya.
*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

float a[SIZE][SIZE];
int NUM;

int main (int argc, char *argv[])
{
  int i,j;
  float s1,s2;

  if (argc < 2) {
    printf("Usage: wave <size>\n");
    exit(1);
  }
  NUM = atoi(argv[1]);

  for (i=0 ; i < NUM ; i++) a[i][0] = 1.0;
  for (i=0 ; i < NUM ; i++) a[0][i] = 1.0;

  for (i=1 ; i < NUM ; i++) 
    for (j=1 ; j < NUM ; j++)
      a[i][j] = a[i-1][j] + a[i][j-1] + a[i-1][j-1];

  for (i=0, s1=0.0 ; i < NUM ; i++) {
    for (j=0, s2=0.0 ; j < NUM ; j++) s2 += a[i][j];
    s1 += s2;
  }
  printf("sum = %f\n", s1);
  exit(0);
}
