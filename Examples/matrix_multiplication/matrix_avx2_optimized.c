/* Description: This program multiplies two matrices of dimensions SIZE × SIZE
 * using SIMD AVX2 instruction set with added optimizations.
 * 1). Blocking (Cache Optimization): Matrices are divided into blocks of size BLOCK_SIZE
 * for better cache utilization, fewer main memory accesses.
 * 2). Loop Unrolling: Inner jj loop is unrolled by factor of 4.
 * Reduces loop overhead and branch predictions
*/

#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define BLOCK_SIZE 16
#define SIZE 512  // Matrix size

/* Matrix multiplication using AVX2 instruction set. Optimization included:
 * 1). Blocking (Cache Optimization): Matrices are divided into blocks of size BLOCK_SIZE
 * for better cache utilization, fewer main memory accesses.
 * 2). Loop Unrolling: Inner jj loop is unrolled by factor of 4.
 * Reduces loop overhead and branch predictions
 * */
void matrix_multiply_avx2_optimized(float *A, float *B, float *C) {
    for (int i = 0; i < SIZE; i += BLOCK_SIZE) {
        for (int j = 0; j < SIZE; j += BLOCK_SIZE) {
            for (int k = 0; k < SIZE; k += BLOCK_SIZE) {
                int i_max = i + BLOCK_SIZE > SIZE ? SIZE : i + BLOCK_SIZE;
                int j_max = j + BLOCK_SIZE > SIZE ? SIZE : j + BLOCK_SIZE;
                int k_max = k + BLOCK_SIZE > SIZE ? SIZE : k + BLOCK_SIZE;

                for (int ii = i; ii < i_max; ii++) {
                    for (int jj = j; jj < j_max; jj += 8) {
                        __m256 c_vec = _mm256_loadu_ps(&C[ii * SIZE + jj]);  // Load existing C values
  			// Inner Loop unrolling by 4
                        for (int kk = k; kk < k_max; kk += 4) {
                            // Load 4 elements of B
                            __m256 b_vec1 = _mm256_loadu_ps(&B[(kk + 0) * SIZE + jj]);
                            __m256 b_vec2 = _mm256_loadu_ps(&B[(kk + 1) * SIZE + jj]);
                            __m256 b_vec3 = _mm256_loadu_ps(&B[(kk + 2) * SIZE + jj]);
                            __m256 b_vec4 = _mm256_loadu_ps(&B[(kk + 3) * SIZE + jj]);

                            // Broadcast 4 elements from A
                            __m256 a_vec1 = _mm256_set1_ps(A[ii * SIZE + (kk + 0)]);
                            __m256 a_vec2 = _mm256_set1_ps(A[ii * SIZE + (kk + 1)]);
                            __m256 a_vec3 = _mm256_set1_ps(A[ii * SIZE + (kk + 2)]);
                            __m256 a_vec4 = _mm256_set1_ps(A[ii * SIZE + (kk + 3)]);

                            // Perform fused multiply-add (FMA)
                            c_vec = _mm256_fmadd_ps(a_vec1, b_vec1, c_vec);
                            c_vec = _mm256_fmadd_ps(a_vec2, b_vec2, c_vec);
                            c_vec = _mm256_fmadd_ps(a_vec3, b_vec3, c_vec);
                            c_vec = _mm256_fmadd_ps(a_vec4, b_vec4, c_vec);
                        }
                        _mm256_storeu_ps(&C[ii * SIZE + jj], c_vec);  // Store result
                    }
                }
            }
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
    float *A = _mm_malloc(SIZE * SIZE * sizeof(float), 32);
    float *B = _mm_malloc(SIZE * SIZE * sizeof(float), 32);
    float *result = _mm_malloc(SIZE * SIZE * sizeof(float), 32);
    
    srand(time(NULL));

    // Initialize matrices
    initialize_matrix(A);
    initialize_matrix(B);
    memset(result, 0, SIZE * SIZE * sizeof(float));
    
    printf("Performing matrix multiplication using avx2 optimized version...\n");
    // Perform matrix multiplication
    matrix_multiply_avx2_optimized(A, B, result);
    printf("Computation completed.\n");

    
    printf("Matrix A:\n");
    print_matrix(A);
    printf("Matrix B:\n");
    print_matrix(B);
    printf("Result (AVX2-Optimized):\n");
    print_matrix(result);
    
    // Free memory
    _mm_free(A);
    _mm_free(B);
    _mm_free(result);
    
    return 0;
}
