
#include <stdio.h>
#include <alloca.h>


void alloca_test(int k) {
  int i;
  int *a;

  a = (int *) alloca(k * sizeof (int));
  for (i=0; i < k;  i++)
    a[i] = 2*i;

  printf("Contents of array allocated with alloca:\n[");
  for (i=0; i < k;  i++)
    printf("%d ", a[i]);
  printf("]\n");
}

int main() {
  int i;
  for (i=0;  i < 10;  i++) {
    printf("Calling alloca_test with argument %d ...\n", i);
    alloca_test(i);
    printf("\n");
  }
  printf("Done\n");
  return 0;
}
