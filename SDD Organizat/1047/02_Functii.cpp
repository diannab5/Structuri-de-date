#include <stdio.h>
#include <malloc.h>

int* copyVector(int* x, int m) {
	int * px = (int*)malloc(m*sizeof(int)); // alocare vector duplicat

	for (char i = 0; i < m; i++)
		px[i] = x[i];

	return px;
}

int* copyVector(int*x, int *m) {
	int n = *m;
	*m = 7;

	int * px = (int*)malloc(*m * sizeof(int)); // alocare vector duplicat

	for (char i = 0; i < n; i++)
		px[i] = x[i];
	for (char i = n; i < *m; i++)
		px[i] = 10;

	return px;
}

void copyVector(int m, int* x) {
	int * px = (int*)malloc(m * sizeof(int)); // alocare vector duplicat

	for (char i = 0; i < m; i++)
		px[i] = x[i];
}

void copyVector(int m, int* x, int* *y) { // y pointer transferat prin adrresa
	// adresa este salvata in stack seg frame al functiei main
	*y = (int*)malloc(m * sizeof(int)); // alocare vector duplicat

	for (char i = 0; i < m; i++)
		(*y)[i] = x[i];
}

void copyVector(int m, int* x, int* y) { // y pointer transferat prin valoare
	// adresa alocate este salvata local; nu poate fi accesata din afara functiei
	y = (int*)malloc(m * sizeof(int)); // alocare vector duplicat

	for (char i = 0; i < m; i++)
		y[i] = x[i];
}

void main() {
	int * pa = 0, *pb = 0, m = 5;

	// alocare vector in heap
	pa = (int*)malloc(m * sizeof(int)); // 20 bytes

	// initializare valori vector alocat in heap seg
	for (char i = 0; i < m; i++) {
		pa[i] = i + 1;
	}

	pb = copyVector(pa, m); // apel functie; intoarce adresa memorie heap a vectorului duplicat

	printf("AdresaVector(pa) = %p, AdresaVector(pb) = %p\n\n", pa, pb);

	// dezalocare vector pb
	free(pb);

	pb = copyVector(pa, &m);

	printf("AdresaVector(pa) = %p, AdresaVector(pb) = %p\n\n", pa, pb);
	for (char i = 0; i < m; i++)
		printf(" %d ", pb[i]);
	printf("\n");

	// ?? vectorul pa nu mai corespunde din punct de vedere al lungimii
	free(pa);
	pa = 0;

	copyVector(m, pb); // memory leak; adresa alocata in functie nu este disponibila in main

	// copyVector(m, pb, &pa);
	printf("AdresaVector(pa) = %p, AdresaVector(pb) = %p\n\n", pa, pb);
}