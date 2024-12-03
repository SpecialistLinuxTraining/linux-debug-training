#include <stdio.h>
#include <unistd.h>
#include <my_library/my_shared_library.h>

void main()
{
    print_hello_world(); // this is a call to the hello world function in the shared library
    while (1)            // run indefinitely so that we can examine memory
    {
        sleep(10);
    }
}
