#include <stdio.h>
#include <stdlib.h>

int main() {
  int *ptr = (int *)malloc(5 * sizeof(int));

  printf("Dynamic array allocated with 5 elements\n");

  return 0;
}
