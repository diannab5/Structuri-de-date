#include <stdio.h>
#include <malloc.h>

void main() {
	char a; // a variabila locala; alocare la compilare in stack seg
			// continut arbitrar (fara initializare)

	a = 48;
	printf("Simbol(a) = %c, Continut(a) = %d\n\n", a, a);
	printf("Adresa10(a) = %d, Adresa16(a) = 0x%p\n\n", &a, &a);

	char v[10]; // vector de maxim 10 locatii de tip char
	printf("Adresa10(v) = %d, Adresa16(v) = 0x%p\n\n", v, v);

	for (char i = 0; i < 10; i++)
		v[i] = a + i;

	for (char i = 0; i < 10; i++)
		printf("Simbol(v[i]) = %c, Continut(v[i]) = %d\n\n", v[i], v[i]);

	// v = &a; // locatia v nu poate fi rescrisa

	char *pa; // pointer la char
	printf("Continut(pa) = 0x%p, AdresaStack16(pa) = 0x%p\n\n", pa, &pa);
	// printf("Continut(*pa) = %c\n\n", *pa); // pa cu continut arbitrar; eroare la run-time
	
	// utilizare variabila pa cu adrese de stack seg.
	pa = &a;
	printf("Continut(pa) = 0x%p, AdresaStack16(pa) = 0x%p\n\n", pa, &pa);
	printf("Continut(*pa) = %c\n\n", *pa);

	pa = &v[5]; // pa = v + 5;
	printf("Continut(pa) = 0x%p, AdresaStack16(pa) = 0x%p\n\n", pa, &pa);
	printf("Continut(*pa) = %c\n\n", *pa);

	pa = v;
	for (char i = 0; i < 10; i++)
		printf("Simbol(pa[i]) = %c, Continut(pa[i]) = %d\n\n", pa[i], pa[i]);

	int x = 348;
	printf("Adresa10(x) = %d, Adresa16(x) = 0x%p\n\n", &x, &x);
	printf("Continut(x) = %d\n\n", x);

	pa = (char*)&x;
	printf("Continut(pa) = 0x%p, AdresaStack16(pa) = 0x%p\n\n", pa, &pa);
	printf("Continut(*pa) = %c\n\n", *pa);

	// utilizare variabila pa cu adrese de heap seg.
	pa = (char*)malloc(sizeof(char));
	printf("Continut(pa) = 0x%p, AdresaStack16(pa) = 0x%p\n\n", pa, &pa);
	printf("Continut(*pa) = %c\n\n", *pa);

	*pa = 0x41; // 0x41 = 65
	printf("Continut(pa) = 0x%p, AdresaStack16(pa) = 0x%p\n\n", pa, &pa);
	printf("Continut(*pa) = %c\n\n", *pa);

	free(pa); // dezalocare zona heap; pa utilizat pentru alocare vector in heap
	pa = (char*)malloc(7 * sizeof(char)); // alocare spatiu elemente vector in heap seg.;

	for (char i = 0; i < 7; i++)
		pa[i] = v[i] + i;

	for (char i = 0; i < 7; i++)
		printf("Adresa16(pa[i]) = %p, Simbol(pa[i]) = %c, Continut(pa[i]) = %d\n\n", pa + i, pa[i], pa[i]);

	free(pa); // pa ramane cu acelasi continut 
	printf("Vector dupa dezalocare:\n\n");
	for (char i = 0; i < 7; i++)
		printf("Simbol(pa[i]) = %c, Continut(pa[i]) = %d\n\n", pa[i], pa[i]);

	pa = 0;
	printf("Vector dupa reinitializare pointer cu valoare nula:\n\n");
	for (char i = 0; i < 7; i++)
		printf("Simbol(pa[i]) = %c, Continut(pa[i]) = %d\n\n", pa[i], pa[i]);


}