#include <stdio.h>

void print_python_gdb_example1(char *message);
void print_python_gdb_example2();

void main()
{
    int local_var = 10;
    printf("local Var %d\n", local_var);
    print_python_gdb_example1("Example");
    print_python_gdb_example2();
}

void print_python_gdb_example1(char *message)
{
    printf("\nWelcome to the Use of Python in GDB %s!\n", message);
    fflush(stdout); // flush stdout buffer
}

void print_python_gdb_example2()
{
    char local_var_in_function[25];
    printf("\nThe GDB Python API is very powerful and can be used to automate tasks, write custom debugging tools, and gain deeper insights into your programs.\n");
    fflush(stdout); // flush stdout buffer
}
