#include <stdio.h>

#define CAUSE_DIV_ZERO 
#define CAUSE_OVERFLOW 

void divide_by_zero() {
    int x = 10, y = 0;
#ifdef CAUSE_DIV_ZERO
    printf("Result: %d\n", x / y); // Division by zero
#endif
}

void integer_overflow() {
    int x = __INT_MAX__;
#ifdef CAUSE_OVERFLOW
    printf("Overflow: %d\n", x + 1); // Signed integer overflow
#endif
}

int main() {
    divide_by_zero();
    integer_overflow();
    return 0;
}
