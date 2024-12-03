// mylib.c

#include <stdio.h>

void causeSegfault() {
  char *ptr = NULL;
  *ptr = 'A'; // This will cause a segfault
}
