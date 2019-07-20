#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define HEAP_STRUCTURE_MEMORY_CHUNK 20 // capacitate de stocare initiala

void filtrareBottomUp(int * vstr, int size) {
	int idxCurent = size - 1;
	while (idxCurent != 0) {
		int idxParinte = (idxCurent - 1) / 2; // trunchiere prin conbversie implicita la int
		if (vstr[idxCurent] > vstr[idxParinte]) {
			// criteriul de ordonare nu este indeplinit
			// interschimb intre chei
			int aux = vstr[idxCurent];
			vstr[idxCurent] = vstr[idxParinte];
			vstr[idxParinte] = aux;

			idxCurent = idxParinte; // actualizare index curent
		}
		else {
			// criteriul de ordonare este indeplinit; idxCurent este poz finala in struct heap
			idxCurent = 0; // fortare iesire din structura repetitiva
		}
	}
}
int* inserareStructHeap(int *vstr, int &size, int &capacitate, int key) {
	if (size == capacitate) {
		// vector incarcat la maxim; este necesara realocarea
		capacitate += HEAP_STRUCTURE_MEMORY_CHUNK;
		int * newStr = (int*)malloc(capacitate * sizeof(int));
		for (int i = 0; i < size; i++)
			newStr[i] = vstr[i];

		// dezalocare vector "plin" cu chei
		free(vstr);
		// actualizare vstr catre noul vector
		vstr = newStr;
	}
	// inserare element (key) in vector
	vstr[size] = key;
	size += 1; // noua dimensiune efectiva a vectorului < capcitate

	filtrareBottomUp(vstr, size);

	return vstr;
}

int stergereStructHeap(int * vstr, int size) {
	// 1. validare existenta cel putin 1 cheie in structura

	// 2. interschimb chei cu offset 0 si size - 1

	// 3. salvare cheie de pe size - 1 in variabila locala

	// 4. actualizare dimensiune efectiva a vectorului

	// 5. aplicare procedura de filtrare top-down (functie)

	// 6. returnare variabila locala cu cheia extrasa
}

void filtrareTopDown(int * vstr, int size) {
	// 1. index curent: 0

	// 2. determinare index descendent stanga, respectiv dreapta

	// 3. determinare valoare maxima a cheilor dintre stanga si dreapta

	// 4. validare criteriu de ordonare; eventual interschimb si actualizare index curent

	// 5. criteriu de oprire procedura de filtrare: nodul de pe index curent nu are descendenti SAU 
	// criteriul de ordonare este indeplinit pentru ambii descendenti
}

void main() {
	int* structHeap, capacitate, size;
	capacitate = HEAP_STRUCTURE_MEMORY_CHUNK;
	// alocare initiala a vectorului suport pt structura heap

	structHeap = (int *)malloc(capacitate * sizeof(int));

	// size = dimensiune efectiva vector
	size = 0; // nu exista cheie in structura heap

	FILE *f = fopen("chei.txt", "r");
	char* token;
	int cheie;

	fscanf(f, "%d", &cheie);
	while (!feof(f)) {
		structHeap = inserareStructHeap(structHeap, size, capacitate, cheie);
		
		fscanf(f, "%d", &cheie);
	}

	int j = 1;
	int nivel = 1;
	for (int i = 0; i < size; i++) {
		printf(" %d ", structHeap[i]);
		j -= 1;
		if (!j) {
			printf("\n");
			j = nivel * 2; // j este numarul de elemente de pe nivelul nivel
			nivel += 1;
		}
	}

	// inserare cheie cu valoarea 27
	structHeap = inserareStructHeap(structHeap, size, capacitate, 27);

	printf("\nStructura heap dupa inserarea cheii 27:\n");
	j = 1;
	nivel = 1;
	for (int i = 0; i < size; i++) {
		printf(" %d ", structHeap[i]);
		j -= 1;
		if (!j) {
			printf("\n");
			j = nivel * 2; // j este numarul de elemente de pe nivelul nivel
			nivel += 1;
		}
	}
}