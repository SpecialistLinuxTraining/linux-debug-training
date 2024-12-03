#include <stdio.h>
#include <stdlib.h>
#include "sample_application_tp.h"

int allocate_memory()
{
    char *buffer = (char *)malloc(100);
    if (buffer == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Memory allocated.\n");
    free(buffer);
    return 0;
}

int main(int argc, char *argv[])
{
    printf("Hello World");
    lttng_ust_tracepoint(sample_application, information_tracepoint, 123, "Test tracepoint");
    allocate_memory();
    return 0;
}
