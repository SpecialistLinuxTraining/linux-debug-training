/* Electric Fence uses guard pages around allocated memory to catch overflows.
 * As a result, Electric Fence aligns the end of allocated blocks with the 
 * start of a guard page by padding the memory block. It may not detect 
 * out-of-bounds access within the same page 
 * (e.g., writing past the end of the block but within the same page) as 
 * no page boundary violation occurs.
 *
 * Refer to "Word-alignment and Overrun Detection" on 
 * https://linux.die.net/man/3/libefence 
 * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CAUSE_OUT_BOUND
#define CAUSE_USE_FREE

int out_of_bounds_access() {
/* This function demonstrates an out-of-bound memory access.
 * Note: Out-of-bound violations within the same page boundary
 * may go undetected when using Electric Fence (libefence). */

    size_t page_size = sysconf(_SC_PAGESIZE);
    printf("System page size: %zu bytes\n", page_size);

    // Dynamically allocate memory slightly less than 
    // a page size to avoid crossing into a guard page
    char *array = (char *)malloc(page_size - 1);
    if (!array) {
        perror("malloc failed");
        return 1;
    }

    printf("Allocated memory at %p\n", (void *)array);

    printf("Attempting in-page overflow...\n");
    array[page_size - 2] = 'X'; // Within bounds
    array[page_size - 1] = 'Y'; // Outside bound and overflow goes undetected.
#ifdef CAUSE_OUT_BOUND
    printf("Attempting overflow in guard page...\n");
    array[page_size] = 'Z'; // Overflowed into guard page
#endif

    free(array);

    return 0;
}

void use_after_free() {
/* Electric Fence detects use-after-free errors by placing allocated memory
 * in protected regions aligned with guard pages. When memory is freed, it
 * marks the corresponding page(s) as inaccessible using memory protection
 * mechanisms like mprotect(). Any subsequent attempt to read or write to 
 * the freed memory triggers a segmentation fault.
 */
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

int main() {
    out_of_bounds_access();
    use_after_free();
    
    return 0;
}
