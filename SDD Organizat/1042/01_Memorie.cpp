#include <stdio.h>
#include <malloc.h>

void main() {
	char a; // variabila locala, alocata la compilare in stack seg.
	a = 65; // char a = 0x41; char a = 'A';

	printf("Simbol(a) = %c, Intreg10(a) = %d, Intreg16(a) = %X\n\n", a, a, a);
	printf("Adresa10(a) = %d, Adresa16(a) = %p\n\n", &a, &a);

	char va[10];
	for (char i = 0; i < 10; i++)
		va[i] = a + i;

	printf("Adresa(10)(va) = %d, Adresa16(va) = %p\n\n", va, va);

	for (char i = 0; i < 10; i++) {
		printf("Simbol(va[%d]) = %c, Intreg10(va[%d]) = %d, Adresa16(va[%d]) = %p \n", i + 1, va[i], i + 1, va[i], i + 1, &va[i]);
	}

	//va = &a; // va este adresa constanta; nu poate fi modificata

	char *pa;
	pa = &a; // adresa de stack seg. a variabilei a este stocata in pa; acces la locatia a prin pa (in mod indirect)
	printf("Simbol(*pa) = %c, Intreg10(*pa) = %d, ContinutAdresa(pa) = %p, AdresaStack16(pa) = %p \n",*pa, *pa, pa, &pa);

	pa = va; // adresa de stack seg. unde incepe alocarea masivului va
	printf("ContinutAdresa(pa) = %p, AdresaStack16(pa) = %p \n", pa, &pa);

	pa[5] = 77;
	for (char i = 0; i < 10; i++) {
		printf("Simbol(va[%d]) = %c, Intreg10(va[%d]) = %d, Adresa16(va[%d]) = %p \n", i + 1, va[i], i + 1, va[i], i + 1, &va[i]);
	}

	int x = 80;
	printf("Intreg10(x) = %d, Adresa16(x) = %p\n\n", x, &x);
	pa = (char*)&x; // stocare adresa x in pa (adresa primului byte din cei 4)
	printf("Simbol(*pa) = %c, Intreg10(*pa) = %d, ContinutAdresa(pa) = %p, AdresaStack16(pa) = %p \n", *pa, *pa, pa, &pa);

	pa = pa + 1; // adresa byte-ul 2 din x (x este int si are 4 bytes)
	printf("Intreg10(*pa) = %d, ContinutAdresa(pa) = %p, AdresaStack16(pa) = %p \n", *pa, pa, &pa);

	pa = (char*)malloc(sizeof(char)); // alocare 1 item de tip char

	*pa = 'X';
	printf("Simbol(*pa) = %c, Intreg10(*pa) = %d, ContinutAdresa(pa) = %p, AdresaStack16(pa) = %p \n", *pa, *pa, pa, &pa);

	free(pa);
	printf("Simbol(*pa) = %c, Intreg10(*pa) = %d, ContinutAdresa(pa) = %p, AdresaStack16(pa) = %p \n", *pa, *pa, pa, &pa);
	pa = (char*)malloc(7 * sizeof(char)); // alocare vector de tip char

	for (char i = 0; i < 7; i++) { // initializare vector in heap seg
		pa[i] = va[i] + 3;
	}

	for (char i = 0; i < 7; i++) { // initializare vector in heap seg
		printf("Simbol(pa[i]) = %c, Intreg10(pa[i]) = %d, ContinutAdresa(pa) = %p, AdresaStack16(pa) = %p \n", pa[i], pa[i], pa + i, &pa);
	}

	free(pa);
}
