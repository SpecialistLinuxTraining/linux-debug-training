#include <stdio.h>
#include <stdlib.h>

#define CAUSE_OUT_BOUND
#define CAUSE_USE_FREE
#define CAUSE_MEM_LEAK

void out_of_bounds_access() {
    int arr[5];
    // Condition: Accessing array out of bounds (i=5).
#ifdef CAUSE_OUT_BOUND
    for (int i = 0; i <= 5; i++) {
#else
    for (int i = 0; i < 5; i++) {
#endif
        arr[i] = i * 10;
    }
}

void use_after_free() {
    int *ptr = malloc(sizeof(int));
    *ptr = 42;
    free(ptr);

    // Re-allocate memory to overwrite the freed block
    int *new_ptr = malloc(sizeof(int));
    *new_ptr = 99;

    // Condition: use-after-free, ptr was freed before
#ifdef CAUSE_USE_FREE
    printf("Use-after-free value: %d\n", *ptr);
#endif
    free(new_ptr);
}

void memory_leak() {
    int *leaked_ptr = malloc(10 * sizeof(int)); 
    for (int i = 0; i < 10; i++) {
        leaked_ptr[i] = i * i;
    }
    printf("Leaked memory initialized.\n");
    // Condition: Memory leak, Intentionally no free here.
#ifndef CAUSE_MEM_LEAK
    free(leaked_ptr);
#endif
}

int main() {
    out_of_bounds_access();
    use_after_free();
    memory_leak();
    
    return 0;
}
