#include <stdlib.h>

void g(void)
{
	malloc(4000);
}

void f(void)
{
	malloc(2000);
	g();
}

int main() {
    int *ptr1 = malloc(100 * sizeof(int)); // Allocate memory
    int *ptr2 = malloc(200 * sizeof(int)); // Allocate more memory

    // Use allocated memory
    for (int i = 0; i < 100; ++i) {
        ptr1[i] = i;
    }

    free(ptr1); // Free memory
    ptr1 = NULL; // Avoid dangling pointer

    f();

    g();

    // Use allocated memory
    for (int i = 0; i < 200; ++i) {
        ptr2[i] = i;
    }

    free(ptr2); // Free memory
    ptr2 = NULL; // Avoid dangling pointer

    return 0;
}

