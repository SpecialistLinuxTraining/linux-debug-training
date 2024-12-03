#include <stdio.h>

int main(int argc, char *argv[])
{
    // argc: Number of command-line arguments
    // argv: Array of command-line argument strings

    // Check if there are at least two arguments (including the program name)
    if (argc < 2)
    {
        printf("Usage: %s <arg1> [arg2] [arg3] ...\n", argv[0]);
        return 1; // Indicates an error
    }

    // Print the program name
    printf("Program name: %s\n", argv[0]);

    // Print each command-line argument
    printf("Arguments passed to the program:\n");
    for (int i = 1; i < argc; i++)
    {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    return 0; // Indicates successful execution
}
