/* Description: This program multiplies two matrices of dimensions SIZE × SIZE
 * using SIMD AVX2 instruction set.
*/

#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SIZE 512  // Matrix size

// Matrix multiplication with AVX2
void matrix_multiply_avx2(float* A, float* B, float* C) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j += 8) {  // Process 8 elements at a time
            __m256 c_vec = _mm256_setzero_ps();  // Initialize result vector to 0
            
            for (int k = 0; k < SIZE; k++) {
                // Load 8 elements from B matrix (row k, columns j to j+7)
                __m256 b_vec = _mm256_loadu_ps(&B[k * SIZE + j]);
                
                // Broadcast single element from A matrix
                __m256 a_vec = _mm256_set1_ps(A[i * SIZE + k]);
                
                // FMA: c = c + a * b
                c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
            }
            
            // Store the result
            _mm256_storeu_ps(&C[i * SIZE + j], c_vec);
        }
    }
}

// Function to initialize matrix with random values
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

    printf("Performing matrix multiplication using avx2 instructions...\n");
    // Perform matrix multiplication
    matrix_multiply_avx2(A, B, result);
    printf("Computation completed.\n");

    printf("Matrix A:\n");
    print_matrix(A);
    printf("Matrix B:\n");
    print_matrix(B);
    printf("Result (AVX2):\n");
    print_matrix(result);

    _mm_free(A);
    _mm_free(B);
    _mm_free(result);

    return 0;
}
