#include <stdio.h>
#include <stdlib.h>

void uninitialized_memory_read() {
    int *arr = (int *)malloc(5 * sizeof(int));
    printf("Value: %d\n", arr[3]); // Use of uninitialized memory
    free(arr);
}

int main() {
    uninitialized_memory_read();
    return 0;
}
