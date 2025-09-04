#include <stdlib.h>

#define CAUSE_MEM_LEAK

void g(void)
{
	int *ptr = malloc(4000);
#ifndef CAUSE_MEM_LEAK
	free(ptr);
#endif
}

void f(void)
{
	int *ptr = malloc(2000);
	g();
#ifndef CAUSE_MEM_LEAK
	free(ptr);
#endif
}

int main() {
    int *ptr1 = malloc(100 * sizeof(int));
    int *ptr2 = malloc(200 * sizeof(int));

    for (int i = 0; i < 100; ++i) {
        ptr1[i] = i;
    }

    free(ptr1);
    ptr1 = NULL;

    f();

    g();

    for (int i = 0; i < 200; ++i) {
        ptr2[i] = i;
    }

    free(ptr2);
    ptr2 = NULL;

    return 0;
}

