/*
 * main.c
 *
 *  Created on: 15 Kas 2020
 *      Author: X550V
 */

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"
#define N 5
#define M 5

int main(void) {

	fflush(stdin);
	fflush(stdout);

	int **A = createMatrix(N, M);
	printMatrix(N, M, A);

	int *nzForCRS = (int*) malloc(sizeof(int) * N * M);

	int *nzForCCS = (int*) malloc(sizeof(int) * N * M);

	int *nzForIJ = (int*) malloc(sizeof(int) * N * M);

	int *col_ind = (int*) malloc(sizeof(int) * N * M);

	int *row_ptr = (int*) malloc(sizeof(int) * (N + 1));

	int *row_ind = (int*) malloc(sizeof(int) * N * M);

	int *col_ptr = (int*) malloc(sizeof(int) * (N + 1));

	int *rows = (int*) malloc(sizeof(int) * N * M);

	int *cols = (int*) malloc(sizeof(int) * N * M);

	compress_CRS(A, nzForCRS, col_ind, row_ptr, N, M);

    compress_CCS(A, nzForCCS, row_ind, col_ptr, N, M);

    compress_IJ(A, nzForIJ, rows, cols, N, M);


	puts("decompress_CRS");
	int **B = decompress_CRS(nzForCRS, col_ind, row_ptr, N, M);
	printMatrix(N, M, B);

	puts("decompress_CCS");
	int **C = decompress_CCS(nzForCCS, row_ind, col_ptr, N, M);
	printMatrix(N, M, C);

	puts("decompress_IJ");
    int **D = decompress_IJ(nzForIJ, rows, cols, N, M);
	printMatrix(N, M, D);


	return EXIT_SUCCESS;
}
