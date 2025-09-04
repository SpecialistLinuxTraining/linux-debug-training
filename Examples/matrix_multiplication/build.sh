gcc -O3 -g matrix_traditional.c -o matrix_traditional -Wall
gcc -O3 -g -mavx2 -mfma matrix_avx2_basic.c -o matrix_avx2_basic -Wall
gcc -O3 -g -mavx2 -mfma matrix_avx2_optimized.c -o matrix_avx2_optimized -Wall

