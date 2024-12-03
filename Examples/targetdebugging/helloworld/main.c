#include <stdio.h>

void print_hello_to_the_world(char *message);
void print_goodbye_to_the_world(char *message);

void main()
{
    print_hello_to_the_world("world");
    printf("Press any key to exit...");
    getchar(); // Wait for a key press
    print_goodbye_to_the_world("world");
}

void print_hello_to_the_world(char *message)
{
    printf("\nHello %s!\n", message);
    fflush(stdout); // flush stdout buffer
}

void print_goodbye_to_the_world(char *message)
{
    printf("\nGoodbye %s!\n", message);
    fflush(stdout); // flush stdout buffer
}
