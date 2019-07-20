#include <stdio.h>
#include <malloc.h>

int main() {
	char a;
	a = 65; // a = 'A'; a = 0x41;

	printf("Simbol(a) = %c, Intreg10(a) = %d, Intreg16(a) = %X\n\n", a, a, a);
	printf("Adresa10(a) = %d, AdresaStack16(a) = %p\n\n", &a, &a);

	char va[10];
	for (char i = 0; i < 10; i++)
		va[i] = a + i;
	printf("AdresaStack16(va) = %p\n\n", va);
	for (char i = 0; i < 10; i++) 
		printf("Simbol(va[%d]) = %c, Intreg10(va[%d]) = %d, Intreg16(va[%d]) = %X\n\n", 
			i+1, va[i], i+1, va[i], i+1, va[i]);

	// va = &a; // atribuire incorecta: va este adresa constanta
	char * pa = 0; // pointer nul
	pa = &a;

	printf("AdresaStack16(pa) = %p, ContinutAdresa(pa) = %p, Continut(*pa) = %c, Continut(*pa) = %d\n\n", &pa, pa, *pa, *pa);

	pa = va; // adresa de inceput a vectorului este in pa
	pa[7] = va[7] + 5;

	for (char i = 0; i < 10; i++)
		printf("Simbol(va[i]) = %c, Intreg10(va[i]) = %d\n", va[i], va[i]);

	int x = 80;
	pa = (char*)&x;

	printf("AdresaStack16(x) = %p, Continut(x) = %d \n\n", &x, x);

	printf("AdresaStack16(pa) = %p, ContinutAdresa(pa) = %p, Continut(*pa) = %c, Continut(*pa) = %d\n\n", &pa, pa, *pa, *pa);

	pa = pa + 1;

	printf("AdresaStack16(pa) = %p, ContinutAdresa(pa) = %p, Continut(*pa) = %c, Continut(*pa) = %d\n\n", &pa, pa, *pa, *pa);

	pa = (char*)malloc(sizeof(char));

	*pa = 'X';
	printf("AdresaStack16(pa) = %p, ContinutAdresa(pa) = %p, Continut(*pa) = %c, Continut(*pa) = %d\n\n", &pa, pa, *pa, *pa);
	pa[0] = *pa - 1;
	printf("AdresaStack16(pa) = %p, ContinutAdresa(pa) = %p, Continut(*pa) = %c, Continut(*pa) = %d\n\n", &pa, pa, *pa, *pa);

	free(pa);

	printf("AdresaStack16(pa) = %p, ContinutAdresa(pa) = %p, Continut(*pa) = %c, Continut(*pa) = %d\n\n", &pa, pa, *pa, *pa);

	pa = (char*)malloc(7 * sizeof(char));

	for (char i = 0; i < 7; i++)
		pa[i] = va[i] + 3;

	for (char i = 0; i < 7; i++)
		printf("AdresaHeap16(pa[i]) = %p, Continut(pa[i]) = %c, ContinutIntreg10(pa[i]) = %d\n", pa + i, pa[i], pa[i]);

	free(pa);

	return 0;
}