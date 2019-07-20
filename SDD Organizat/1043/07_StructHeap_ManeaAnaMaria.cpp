#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define INIT_HEAP_STRUCT_SIZE 20

int* inserareCheieHeapStruct(int* hs, int &size, int &capacitate, int key) {
	if (size == capacitate) { // nu exista spatiu pentru inserare cheie
		// realocare vector hs cu mutare chei in noul vector
		capacitate = capacitate + INIT_HEAP_STRUCT_SIZE;
		int * newHS = (int *)malloc(capacitate *sizeof(int));

		// mutare continut hs in noul vector newHS
		for (int i = 0; i < size; i++)
			newHS[i] = hs[i];

		// dezalocare vector incarcat la capcitate maxima
		free(hs);
		// actualizare parametru hs cu adresa vectorului marit ca dimensiune
		hs = newHS;
	}

	// inserare cheie in structura heap
	hs[size] = key;
	size += 1; // noua dimensiune efectiva a vectorului = noul numar de noduri din structura heap

	// procedura de filtrare bottom-up
	int offsetCurent, offsetParinte;
	char flagStopFiltrare = 0;
	offsetCurent = size - 1;

	while (!flagStopFiltrare && offsetCurent != 0) {
		offsetParinte = (offsetCurent - 1)/ 2; 
		if (hs[offsetCurent] > hs[offsetParinte]) {
			// relatia de ordine nu este indeplinita
			int aux = hs[offsetCurent];
			hs[offsetCurent] = hs[offsetParinte];
			hs[offsetParinte] = aux;

			offsetCurent = offsetParinte; // noul offset curent al nodului inserat este cel al parintelui cu care s-a intreschimbat
		}
		else {
			// relatia de ordine este indeplinita; stop procedura de filtrare
			flagStopFiltrare = 1;
		}
	}

	return hs;
}

int extrageCheieHeapStruct(int* hs, int &size) {
	int cheieExtrasa = -1;
	if (size > 0) {
		cheieExtrasa = hs[0];
		hs[0] = hs[size - 1];
		size = size - 1;
		int st, dr, max = 0;
		while (max < size) {
			int i = max;
			st = 2 * max + 1;
			dr = 2 * max + 2;
			if (dr < size && st < size) {
				if (hs[dr] < hs[st]) {
					max = st;
				}
				else {
					max = dr;
				}
			}
			else {
				if (st < size && hs[st]> hs[(st - 1) / 2])
					max = st;
			}

			if (max == i) {
				max = size;
			}
			else {
				int parinte = hs[(max - 1) / 2];
				hs[(max - 1) / 2] = hs[max];
				hs[max] = parinte;
			}
		}
	}

	return cheieExtrasa;
}


void main() {
	int * heapStruct, size, capacitate, keyTemp;
	size = 0;
	capacitate = INIT_HEAP_STRUCT_SIZE;

	// alocare vector suport pt structura heap
	heapStruct = (int *)malloc(capacitate * sizeof(int));

	FILE *f;
	f = fopen("chei.txt", "r");

	fscanf(f, "%d", &keyTemp); // citire int din fisier; elmentele int sunt separate prin blanks
	while (!feof(f)) {
		heapStruct = inserareCheieHeapStruct(heapStruct, size, capacitate, keyTemp);

		fscanf(f, "%d", &keyTemp);
	}

	// traversare vector suport al structurii heap
	int j = 0, nivel = 1;
	for (int i = 0; i < size; i++) {
		printf(" %d ", heapStruct[i]);
		j += 1;
		if (j == nivel) {
			printf("\n");

			j = 0;
			nivel = nivel * 2;
		}
	}
	printf("\n\n");

	heapStruct = inserareCheieHeapStruct(heapStruct, size, capacitate, 26);
	heapStruct = inserareCheieHeapStruct(heapStruct, size, capacitate, 42);
	printf(" Structura heap dupa inserare cheie:\n\n");
	j = 0, nivel = 1;
	for (int i = 0; i < size; i++) {
		printf(" %d ", heapStruct[i]);
		j += 1;
		if (j == nivel) {
			printf("\n");

			j = 0;
			nivel = nivel * 2;
		}
	}
	printf("\n\n");

	int CheieExtrasa = extrageCheieHeapStruct(heapStruct, size);
	printf("Cheie extrasa: %d\n", CheieExtrasa);

	printf("Structura heap dupa extragere cheie\n");
	j = 0, nivel = 1;
	for (int i = 0; i < size; i++) {
		printf(" %d ", heapStruct[i]);
		j += 1;
		if (j == nivel) {
			printf("\n");

			j = 0;
			nivel = nivel * 2;
		}
	}
	printf("\n\n");

	fclose(f);
}