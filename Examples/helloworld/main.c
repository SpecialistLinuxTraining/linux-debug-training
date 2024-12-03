#include <stdio.h>

void print_hello_to_the_world(char *message);

void main()
{
    print_hello_to_the_world("world");
}

void print_hello_to_the_world(char *message)
{
    printf("\nHello %s!\n", message);
    fflush(stdout); // flush stdout buffer
}