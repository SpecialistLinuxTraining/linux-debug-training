/* Description: This program multiplies two matrices of dimensions SIZE × SIZE
 * using the traditional matrix multiplication method.
 * */

#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SIZE 512  // Matrix size

// Traditional matrix multiplication
void matrix_multiply(float* A, float* B, float* C) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            float sum = 0.0f;
            for (int k = 0; k < SIZE; k++) {
                sum += A[i * SIZE + k] * B[k * SIZE + j];
            }
            C[i * SIZE + j] = sum;
        }
    }
}

// Initialize matrix with random values
void initialize_matrix(float* matrix) {
    for (int i = 0; i < SIZE * SIZE; i++) {
            matrix[i] = (float)rand() / RAND_MAX;
    }
}

// Print a small part of the matrix for verification
void print_matrix(float* M) {
    for (int i = 0; i < MIN(4, SIZE); i++) {
        for (int j = 0; j < MIN(4, SIZE); j++) {
            printf("%0.2f ", M[i * SIZE + j]);
        }
        printf("...\n");
    }
    printf("...\n");
}

int main() {
    // Allocate aligned memory for matrices
    float* A = (float*)_mm_malloc(SIZE * SIZE * sizeof(float), 32);
    float* B = (float*)_mm_malloc(SIZE * SIZE * sizeof(float), 32);
    float* result = (float*)_mm_malloc(SIZE * SIZE * sizeof(float), 32);

    srand(time(NULL));
    
    // Initialize matrices
    initialize_matrix(A);
    initialize_matrix(B);

    printf("Performing traditional matrix multiplication...\n");
    // Perform matrix multiplication
    matrix_multiply(A, B, result);
    printf("Computation completed.\n");

    printf("Matrix A:\n");
    print_matrix(A);
    printf("Matrix B:\n");
    print_matrix(B);
    printf("Result (Traditional):\n");
    print_matrix(result);

    _mm_free(A);
    _mm_free(B);
    _mm_free(result);

    return 0;
}
