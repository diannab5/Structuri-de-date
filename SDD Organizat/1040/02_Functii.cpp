
#include <stdio.h>
#include <malloc.h>

/*int changeInt(int a, char b) {
	a = a + b;
	return a;
}*/

void changeInt(int a, char b) {
	a = a + b;
}

void changeInt(int *a, char b) {
	*a = *a + b;
}

int* allocVector(int *pi, char m) {
	pi = (int *)malloc(m *sizeof(int));

	for (char i = 0; i < m; i++)
		pi[i] = 0;

	return pi;
}

void allocVector(char m, int *pi) {
	pi = (int *)malloc(m * sizeof(int));

	for (char i = 0; i < m; i++)
		pi[i] = 0;
}

void allocVector(char m, int* *pi) {
	*pi = (int *)malloc(m * sizeof(int));

	for (char i = 0; i < m; i++)
		(*pi)[i] = 0;
}

void main() {
	int x = 8;
	char y = 10;

	printf("x = %d, y = %d\n", x, y);
	// x = changeInt(x, y); // transfer params prin valoare, rezultat intors prin return
	// changeInt(x, y); // transfer params prin valoare, rezultat nu este intors prin return (tip return este void)
	changeInt(&x, y); // transfer parametru a prin adresa, param b prin valoare, rezultat nu este intors prin return (tip return este void)
	printf("x = %d, y = %d\n", x, y);

	int *px = 0, m = 5;
	// px = allocVector(px, m); // primeste px nul si intoarce adresa de vector; px nul se rescrie dupa inchiderea apelului de functie
	// allocVector(m, px); // primeste px nul; px nul nu se rescrie dupa inchiderea apelului de functie
	allocVector(m, &px); // primeste px nul; px nul nu se rescrie dupa inchiderea apelului de functie

	for (char i = 0; i < m; i++)
		printf(" %d ", px[i]);
	printf("\n\n");

	char sMat[10][20];
	sMat[0][10] = 0; // sMat[0][10] = '\0' - scrie valoarea 0 la nivel fizic;  sMat[0][10] = '0' - scre valoarea 48 la nivel fizic
}


