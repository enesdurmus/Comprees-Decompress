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
			if (A[i][j] != 0) { // E�er matrisin gelen eleman� 0' dan farkl� ise .
				nz[count] = (int) A[i][j]; // nz pointer'na ekliyoruz
				col_ind[count] = j; // ve nz' ye ekledi�imiz eleman�n kolon indisini yani j'yi col_ind pointer'na ekliyoruz.
				if (rowCheck == 1) { // Sat�r�n 0'dan farkl� ilk eleman� ise row_ptr'ye
					row_ptr[count2] = count; // ekliyoruz ve sat�r de�i�ene kadar buraya girmiyoruz.
					count2++;
					rowCheck = 0; // Bu sat�r�n ilk eleman�n� ekledi�imiz i�in row_check'i 0 yap�yoruz bu sat�r boyunca bir daha girmeyecek buraya.
				}
				count++;
			}
		}
		rowCheck = 1; // yeni sat�ra ge�ti�imiz i�in bu count'� tekrar 1 yap�yoruz.
	}
	row_ptr[count2] = count;   //row_ptr nin son eleman�n� ekliyoruz.
}

int** decompress_CRS(int *nz, int *col_ind, int *row_ptr, int N, int M) {
	int **A = createMatrix(N, M); // D�nd�rece�imiz matrisimiz i�in bellekte yer a��yoruz.
	int count = 0, count2 = 1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (col_ind[count] == j) { // col_ind deki b�t�n de�erleri s�tunlar�m�zla kar��la�t�r�yoruz
				if (count != row_ptr[count2]) { // E�er bir a�a�� sat�r�n ilk eleman�n�n indisine gelmi� isek a�a�� sat�ra ge�ene kadar 0 ekliyoruz.
					                            // E�er bu kontrol� yazmak isek baz� durumlarda a�a�� sat�ra ge�meden yan�na ekliyor o y�zden bu kontrol laz�m.
					A[i][j] = (int) nz[count]; // bu kontrolleri ge�en yerlere nz deki elemanlar�, ge�emeyen yerlerede 0 lar� yerle�tiriyoruz.
					count++;
				} else {
					A[i][j] = 0;
				}
			} else {
				A[i][j] = 0;
			}
		}
		count2++; // Bir a�a�� sat�ra ge�ti�imiz i�in count2'yi artt�r�yoruz.
	}
	return A;
}

void compress_CCS(int **A, int *nz, int *row_ind, int *col_ptr, int N, int M) {
	int count = 0, count2 = 0;
	int colCheck = 1;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (A[j][i] != 0) { // E�er matrisin gelen eleman� 0' dan farkl� ise nz'ye ekliyoruz.
				nz[count] = (int) A[j][i]; // Ancak burada CRS'den farkl� olarak ilkten s�tundaki say�lar� al�yoruz,
				row_ind[count] = j;        // row_ind dizisinide sat�r indislerimizle dolduruyoruz.
				                           // Burada j sat�r indisi oluyor ��nk� nz'yi s�tun s�tun olu�turduk.
				if (colCheck == 1) { // S�tunun 0'dan farkl� ilk eleman� ise col_ptr'ye
					col_ptr[count2] = count; // ekliyoruz ve s�tun de�i�ene kadar buraya girmiyoruz.
					count2++;
					colCheck = 0;
				}
				count++;
			}
		}
		colCheck = 1;
	}
	col_ptr[count2] = count;        //col_ptr'nin son eleman�n� ekliyoruz.

}

int** decompress_CCS(int *nz, int *row_ind, int *col_ptr, int N, int M) {
	int **A = createMatrix(N, M); // D�nd�rece�imiz matrisimiz i�in bellekte yer a��yoruz.
	int count = 0, count2 = 1;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (row_ind[count] == j) { // row_ind deki b�t�n de�erleri s�tunlar�m�zla kar��la�t�r�yoruz.
				if (count != col_ptr[count2]) { // E�er bir yan s�tunun ilk eleman�n�n indisine gelmi� isek yan s�tuna ge�ene kadar 0 ekliyoruz.
					A[j][i] = (int) nz[count]; // bu kontrolleri ge�en yerlere nz deki elemanlar� ge�emeyen yerlerede 0 lar� yerle�tiriyoruz.
					count++;
				} else {
					A[j][i] = 0;
				}
			} else {
				A[j][i] = 0;
			}
		}
		count2++; // Bir sa� s�tuna ge�ti�imiz i�in count2'yi artt�r�yoruz.
	}
	return A;
}

void compress_IJ(int **A, int *nz, int *rows, int *cols, int N, int M) {
	int count = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (A[i][j] != 0) { // E�er matrisin bu eleman� 0'dan farkl� ise.
				nz[count] = (int) A[i][j]; // nz listesine ekliyoruz.
				rows[count] = i; // rows dizisine bulundu�umuz sat�r indisini.
				cols[count] = j; // ve cols dizisinede bulundu�umuz kolon indisini ekliyoruz.
				count++;  //ve count'� artt�r�yoruz.
			}
		}
	}
}

int** decompress_IJ(int *nz, int *rows, int *cols, int N, int M) {
	int **A = createMatrix(N, M); // D�nd�rece�imiz matrisi olu�turuyoruz.
	int counter = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (rows[counter] == i && cols[counter] == j) { // E�er rows'un de�eri i'ye
															// ve cols'un de�eri j'ye e�it ise
															// A matrisinin bu eleman�na nz'deki
				A[i][j] = nz[counter];                      // elemanlar� if teki kontrol sa�land�k�a s�rayla yerle�tiriyoruz.
				counter++;                                  // Ve counter� artt�r�yoruz.
			} else {
				A[i][j] = 0; // E�er kontrol sa�lanmaz ise 0 koyuyoruz.
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
					|| ((count + i + j + rand()) % 7) == 0) { // matrisimize daha �ok random
				M[i][j] = 0;             // 0 eklemek i�in b�yle bir �ey yazd�m.
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

