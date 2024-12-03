#include <stdio.h>

void print_hello_world()
{
    fprintf(stderr, "Hello World From the Shared Library\n");
}

void second_shared_library_function()
{
    fprintf(stderr, "Hello World From the Shared Library's second function\n");
}