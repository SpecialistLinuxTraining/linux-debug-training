#include <stdio.h>

int get_value(char *message_recieved);
void *allocate_memory(size_t size_in_kb);

void main()
{
    int value = get_value("text");
    printf("Recieved return value of %d\n", value);

    void *ptr = allocate_memory(200); // Size of memory to allocate in kilobytes
    if (ptr != NULL)
    {
        // Wait for a key press
        printf("Press any key to exit...\n");
        getchar(); // This will block until a key is pressed
        // Free the allocated memory when done
        free(ptr);
    }
}

int get_value(char *message_recieved)
{
    printf("\nrecieved message %s!\n", message_recieved);
    fflush(stdout); // flush stdout buffer
    return 1;
}

void *allocate_memory(size_t size_in_kb)
{

    // Calculate the size in bytes
    size_t size_in_bytes = size_in_kb * 1024;

    // Allocate memory using malloc
    void *ptr = malloc(size_in_bytes);

    if (ptr == NULL)
    {
        // Memory allocation failed
        printf("Failed to allocate memory.\n");
    }
    else
    {
        // Memory allocation successful
        printf("Memory allocated successfully.\n");
    }
    return ptr;
}