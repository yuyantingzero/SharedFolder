/* The Fibonacci number computation.

   Computes a fibonacci number using recursion.

   Spyros Triantafyllis, 03/16/04
*/

#include <stdio.h>
#include <stdlib.h>



int fib(int n) {
  int i, j, k;
  if (n <= 2)
    return 1;

  i = fib(n-1);
  j = fib(n-2);
  k = i + j;
  return k;
}


int main (int argc, char *argv[])
{
  int i, f1, f2, N;
  int f = 0;

  if (argc < 2) {
    printf("Usage: fib <n>\n");
    exit(1);
  }
  N = atoi(argv[1]);

  printf("fib %d = %d\n", N, fib(N));
  exit(0);
}
