// main.c

#include "mylib.h"
#include <stdio.h>

int main() {
  printf("Starting program...\n");
  causeSegfault(); // Call the function that causes a segfault
  printf("Program finished.\n");
  return 0;
}
