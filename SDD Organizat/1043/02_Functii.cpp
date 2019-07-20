#include <stdio.h>
#include <malloc.h>

//int changeInt(int a, char b) {
//	a = b - 3;
//	return a;
//}

void changeInt(int &a, char b) {
	a = b - 3;
}

//void changeInt(char b, int a) {
//	a = b - 3;
//}

void changeInt(char b, int *a) {
	*a = b - 3;
}

int* allocVector(int * pv, char n) {
	if (pv)
		free(pv);

	pv = (int*)malloc(n * sizeof(int));

	for (char i = 0; i < n; i++)
		pv[i] = i + 1;

	return pv;
}

void allocVector(char n, int * pv) {
	if (pv)
		free(pv);

	pv = (int*)malloc(n * sizeof(int));

	for (char i = 0; i < n; i++)
		pv[i] = i + 1;
}

void allocVector(char n, int * *pv) {
	if (*pv)
		free(*pv);

	*pv = (int*)malloc(n * sizeof(int));

	for (char i = 0; i < n; i++)
		(*pv)[i] = i + 1;
}


void main() {
	int x = 10;
	char y = 9;

	// transfer params I/ prin valoare
	// rezultat intors prin return
	//printf("x = %d\n", x);
	//x = changeInt(x, y);
	//printf("x = %d\n\n", x);

	// transfer params I/ prin valoare
	// functia intoarce 0 rezultate
	//printf("x = %d\n", x);
	//changeInt(y, x);
	//printf("x = %d\n\n", x);

	// transfer params I/ prin valoare
	// functia intoarce 0 rezultate
	//printf("x = %d\n", x);
	//changeInt(y, &x);
	//printf("x = %d\n\n", x);

	int *pi = 0; // pointer null in pi variabila locala
	char n = 9;

	// transfer params prin valoare
	/*pi = allocVector(pi, n);
	for (char i = 0; i < n; i++)
		printf(" %d ", pi[i]);
	printf("\n\n");*/

	// transfer params prin valoare
	// functia nu intoarce nici un rezultat
	//allocVector(n, pi);
	//for (char i = 0; i < n; i++)
	//	printf(" %d ", pi[i]);
	//printf("\n\n");

	// transfer params prin valoare (n) si adresa (pv)
	// functia nu intoarce nici un rezultat
	allocVector(n, &pi);
	for (char i = 0; i < n; i++)
		printf(" %d ", pi[i]);
	printf("\n\n");

	// transfer params I/ prin referinta (a), valoare (b)
	// functia intoarce 0 rezultate
	printf("x = %d\n", x);
	changeInt(x, y); // primul argument poate fi doar int pentru versiunea changeInt(int&, char);
	// changeInt(y, y); // eroare de compilare; versiunea changeInt(char, int) este comentata
	printf("x = %d\n\n", x);
}