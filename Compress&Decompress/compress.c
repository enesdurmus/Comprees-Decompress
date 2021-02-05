/*
 * compress.c
 *
 *  Created on: 15 Kas 2020
 *      Author: X550V
 */

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"
#include <time.h>

void compress_CRS(int **A, int *nz, int *col_ind, int *row_ptr, int N, int M) {
	int count = 0, count2 = 0;
	int rowCheck = 1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (A[i][j] != 0) { // Eðer matrisin gelen elemaný 0' dan farklý ise .
				nz[count] = (int) A[i][j]; // nz pointer'na ekliyoruz
				col_ind[count] = j; // ve nz' ye eklediðimiz elemanýn kolon indisini yani j'yi col_ind pointer'na ekliyoruz.
				if (rowCheck == 1) { // Satýrýn 0'dan farklý ilk elemaný ise row_ptr'ye
					row_ptr[count2] = count; // ekliyoruz ve satýr deðiþene kadar buraya girmiyoruz.
					count2++;
					rowCheck = 0; // Bu satýrýn ilk elemanýný eklediðimiz için row_check'i 0 yapýyoruz bu satýr boyunca bir daha girmeyecek buraya.
				}
				count++;
			}
		}
		rowCheck = 1; // yeni satýra geçtiðimiz için bu count'ý tekrar 1 yapýyoruz.
	}
	row_ptr[count2] = count;   //row_ptr nin son elemanýný ekliyoruz.
}

int** decompress_CRS(int *nz, int *col_ind, int *row_ptr, int N, int M) {
	int **A = createMatrix(N, M); // Döndüreceðimiz matrisimiz için bellekte yer açýyoruz.
	int count = 0, count2 = 1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (col_ind[count] == j) { // col_ind deki bütün deðerleri sütunlarýmýzla karþýlaþtýrýyoruz
				if (count != row_ptr[count2]) { // Eðer bir aþaðý satýrýn ilk elemanýnýn indisine gelmiþ isek aþaðý satýra geçene kadar 0 ekliyoruz.
					                            // Eðer bu kontrolü yazmak isek bazý durumlarda aþaðý satýra geçmeden yanýna ekliyor o yüzden bu kontrol lazým.
					A[i][j] = (int) nz[count]; // bu kontrolleri geçen yerlere nz deki elemanlarý, geçemeyen yerlerede 0 larý yerleþtiriyoruz.
					count++;
				} else {
					A[i][j] = 0;
				}
			} else {
				A[i][j] = 0;
			}
		}
		count2++; // Bir aþaðý satýra geçtiðimiz için count2'yi arttýrýyoruz.
	}
	return A;
}

void compress_CCS(int **A, int *nz, int *row_ind, int *col_ptr, int N, int M) {
	int count = 0, count2 = 0;
	int colCheck = 1;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (A[j][i] != 0) { // Eðer matrisin gelen elemaný 0' dan farklý ise nz'ye ekliyoruz.
				nz[count] = (int) A[j][i]; // Ancak burada CRS'den farklý olarak ilkten sütundaki sayýlarý alýyoruz,
				row_ind[count] = j;        // row_ind dizisinide satýr indislerimizle dolduruyoruz.
				                           // Burada j satýr indisi oluyor çünkü nz'yi sütun sütun oluþturduk.
				if (colCheck == 1) { // Sütunun 0'dan farklý ilk elemaný ise col_ptr'ye
					col_ptr[count2] = count; // ekliyoruz ve sütun deðiþene kadar buraya girmiyoruz.
					count2++;
					colCheck = 0;
				}
				count++;
			}
		}
		colCheck = 1;
	}
	col_ptr[count2] = count;        //col_ptr'nin son elemanýný ekliyoruz.

}

int** decompress_CCS(int *nz, int *row_ind, int *col_ptr, int N, int M) {
	int **A = createMatrix(N, M); // Döndüreceðimiz matrisimiz için bellekte yer açýyoruz.
	int count = 0, count2 = 1;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (row_ind[count] == j) { // row_ind deki bütün deðerleri sütunlarýmýzla karþýlaþtýrýyoruz.
				if (count != col_ptr[count2]) { // Eðer bir yan sütunun ilk elemanýnýn indisine gelmiþ isek yan sütuna geçene kadar 0 ekliyoruz.
					A[j][i] = (int) nz[count]; // bu kontrolleri geçen yerlere nz deki elemanlarý geçemeyen yerlerede 0 larý yerleþtiriyoruz.
					count++;
				} else {
					A[j][i] = 0;
				}
			} else {
				A[j][i] = 0;
			}
		}
		count2++; // Bir sað sütuna geçtiðimiz için count2'yi arttýrýyoruz.
	}
	return A;
}

void compress_IJ(int **A, int *nz, int *rows, int *cols, int N, int M) {
	int count = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (A[i][j] != 0) { // Eðer matrisin bu elemaný 0'dan farklý ise.
				nz[count] = (int) A[i][j]; // nz listesine ekliyoruz.
				rows[count] = i; // rows dizisine bulunduðumuz satýr indisini.
				cols[count] = j; // ve cols dizisinede bulunduðumuz kolon indisini ekliyoruz.
				count++;  //ve count'ý arttýrýyoruz.
			}
		}
	}
}

int** decompress_IJ(int *nz, int *rows, int *cols, int N, int M) {
	int **A = createMatrix(N, M); // Döndüreceðimiz matrisi oluþturuyoruz.
	int counter = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (rows[counter] == i && cols[counter] == j) { // Eðer rows'un deðeri i'ye
															// ve cols'un deðeri j'ye eþit ise
															// A matrisinin bu elemanýna nz'deki
				A[i][j] = nz[counter];                      // elemanlarý if teki kontrol saðlandýkça sýrayla yerleþtiriyoruz.
				counter++;                                  // Ve counterý arttýrýyoruz.
			} else {
				A[i][j] = 0; // Eðer kontrol saðlanmaz ise 0 koyuyoruz.
			}
		}
	}
	return A;
}

int** createMatrix(int n, int m) {
	int **M;
	M = (int**) malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
		M[i] = (int*) malloc(m * sizeof(int));
	}
	srand(time(0));
	int count = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			count++;
			if ((count + i + j + rand()) % 3 == 0
					|| ((count + i + j + rand()) % 7) == 0) { // matrisimize daha çok random
				M[i][j] = 0;             // 0 eklemek için böyle bir þey yazdým.
			} else {
				M[i][j] = rand() % 10; // matrisimizi random int ler ile dolduruyoruz.
			}
		}
	return M;
}
void printMatrix(int n, int m, int **M) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%d   ", M[i][j]);
		}
		puts("");
	}
	puts("-----------");
}

