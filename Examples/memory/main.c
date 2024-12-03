#include <stdio.h>
#include <unistd.h>

void first_function(char *message, int val);
void second_function(char *message, int val);

int first_global_variable = 1;
int second_global_variable = 1;

void main()
{
    int first_local_variable = 1;
    int second_local_variable = 2;
    printf("The address of the main function is %p\n", main);
    printf("The address of the first function is %p\n", first_function);
    printf("The address of the second function is %p\n", second_function);
    printf("The address of the first local variable is %p\n", &first_local_variable);
    printf("The address of the second local variable is %p\n", &second_local_variable);
    printf("The address of the first global variable is %p\n", &first_global_variable);
    printf("The address of the second global variable is %p\n", &second_global_variable);
    first_function("I was called from main", first_local_variable);
    second_function("I was called from main", second_local_variable);
    while (1) // run indefinitely so that we can examine memory
    {
        sleep(10);
    }
}

void first_function(char *message, int val)
{
    printf("Hello from the first function: %s %d!", message, val);
}

void second_function(char *message, int val)
{
    printf("Hello from the second function: %s - %d!", message, val);
    printf("second_global_variable is %d in the second function.", second_global_variable);
    second_global_variable = 6;
}