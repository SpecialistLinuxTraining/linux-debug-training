#include <stdio.h>

// Disable optimization for this specific function
void __attribute__((optimize("O0"))) exampleFunction1()
{
    // Your function implementation
    printf("This function is not optimized.\n");
}

// Enable  optimization for this specific function
void exampleFunction2()
{
    // Your function implementation
    printf("This function can be optimized.\n");
}

int main()
{
    exampleFunction1();
    exampleFunction1();
    return 0;
}
