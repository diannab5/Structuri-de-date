#include <stdio.h>
#include <malloc.h>

int changeInt(int a, char c) {
	a = a + 10;
	int b = a + c;

	return b;
}

int changeInt(int *a, char c) {
	*a = *a + 10;
	int b = *a + c;

	return b;
}

int * allocVector(int *pi, int m) {
	pi = (int*)malloc(m * sizeof(int));

	for (char i = 0; i < m; i++)
		pi[i] = i * 10 + 1;

	return pi;
}

void allocVector(int m, int *pi) {
	pi = (int*)malloc(m * sizeof(int));

	for (char i = 0; i < m; i++)
		pi[i] = i * 10 + 1;

}

void allocVector(int m, int* *pi) {
	*pi = (int*)malloc(m * sizeof(int));

	for (char i = 0; i < m; i++)
		(*pi)[i] = i * 10 + 1;

}


void main() {
	int x = 23;
	char y = 40;

	// transfer prin valoare argumente (a si c) ca variabile simple
	int rez = changeInt(x, y);

	// transfer prin adresa argument a ca variabile simple
	rez = changeInt(&x, y);

	int *prez, m = 7;
	/* prez = allocVector(prez, m);

	for (char i = 0; i < m; i++)
		printf(" %d ", prez[i]);
	printf("\n\n"); */

	// transfer prin valoare argument a ca variabile pointer
	/* allocVector(m, prez); // la terminarea apelului, prez va contine valoarea anterioara apelului
	for (char i = 0; i < m; i++)
		printf(" %d ", prez[i]);
	printf("\n\n"); */
	// transfer prin adresa argument a ca variabile pointer
	allocVector(m, &prez);
	for (char i = 0; i < m; i++)
		printf(" %d ", prez[i]);
	printf("\n\n");
}