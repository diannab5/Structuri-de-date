#include <stdio.h>
#include <malloc.h>

void main() {
	char v; // variabila locala -> alocata la compilare in stack seg
	v = 55; // initializare byte v; alternative: v = '7'; v = 0x37;

	printf("SimbolASCII(v) = %c, Intreg10(v) = %d, Intreg16(v) = 0x%X\n\n", v, v, v);
	printf("Adresa16(v) = 0x%p, Adresa10(v) = %d\n\n", &v, &v);

	char mv[10]; // masiv de maxim 10 elemente de tip char; masivul ocupa 10 bytes

	for (char i = 0; i < 10; i++)
		mv[i] = v + i;

	for (char i = 0; i < 10; i++)
		printf("SimbolASCII(mv) = %c, Intreg10(mv) = %d, Intreg16(mv) = 0x%X\n", mv[i], mv[i], mv[i]);

	// mv = &v; // mv este adresa constanta si nu se permite modificarea
	printf("Adresa16(mv) = 0x%p, Adresa10(mv) = %d\n\n", mv, mv);

	char *pv; // declaratie variabile pointer
	pv = 0; // pv = NULL;
	printf("AdresaStack16(pv) = 0x%p, AdresaContinut16(pv) = 0x%p\n", &pv, pv);
	pv = &v; // adresa de stack seg a lui v este salvata in pv
	printf("AdresaStack16(pv) = 0x%p, AdresaContinut16(pv) = 0x%p, Continut(*pv) = %d\n", &pv, pv, *pv);

	pv = mv; // adresa inceput masiv de char este salvata in pv
	printf("AdresaStack16(pv) = 0x%p, AdresaContinut16(pv) = 0x%p, Continut(*pv) = %d\n\n\n", &pv, pv, *pv);

	pv[3] = mv[0] + 10; // elementul cu offs 3 este modificat in vectorul mv

	for (char i = 0; i < 10; i++)
		printf("AdresaStack16(pv) = 0x%p, AdresaContinut16(pv) = 0x%p, Continut(pv[i]) = %d\n", &pv, pv, pv[i]);


	int x = 81;

	pv = (char*)&x;
	printf("AdresaStack16(pv) = 0x%p, AdresaContinut16(pv) = 0x%p, Continut(*pv) = %d\n", &pv, pv, *pv);
	pv = pv + 2;
	printf("AdresaStack16(pv) = 0x%p, AdresaContinut16(pv) = 0x%p, Continut(*pv) = %d\n", &pv, pv, *pv);

	int* px;
	px = (int*)&v;
	printf("AdresaStack16(px) = 0x%p, AdresaContinut16(px) = 0x%p, Continut(*px) = %d\n", &px, px, *px);

	pv = (char*)malloc(sizeof(char));
	*pv = 77;
	printf("AdresaStack16(pv) = 0x%p, AdresaContinutHeap16(pv) = 0x%p, Continut(*pv) = %d\n", &pv, pv, *pv);
	free(pv);
	printf("AdresaStack16(pv) = 0x%p, AdresaContinutHeap16(pv) = 0x%p, Continut(*pv) = %d\n", &pv, pv, *pv);
}