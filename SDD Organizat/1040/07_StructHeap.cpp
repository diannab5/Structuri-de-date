#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define EXTEND_CAPACITY_SIZE 20

int* inserareNodStructHeap(int* hs, int &size, int &capacitate, int key) {
	if (size == capacitate) {
		// nu mai exista spatiu de adaugat element de vectorul de int
		capacitate = capacitate + EXTEND_CAPACITY_SIZE; // adaug 20 elemente peste capacitate existenta
		int * newHS = (int*)malloc(capacitate * sizeof(int));

		for (int i = 0; i < size; i++)
			newHS[i] = hs[i];

		// dezalocare structura veche
		free(hs);
		// actualizare hs catre noua structura creata
		hs = newHS;
	}

	// inserare propriu-zisa (obligatorie pt ambele cazuri alocare-realocare)
	hs[size] = key;
	size += 1; // creste size cu 1 element prin inserarea de pe linia anterioara

	int ofsCurent, ofsParinte;
	char flagStopFiltrare = 0; // variabila booleana
	ofsCurent = size - 1;

	while (flagStopFiltrare == 0 && ofsCurent != 0) {
		ofsParinte = (ofsCurent - 1) / 2;
		if (hs[ofsCurent] > hs[ofsParinte]) {
			// nu se respecta relatia de ordine pt un max-heap
			int aux = hs[ofsCurent];
			hs[ofsCurent] = hs[ofsParinte];
			hs[ofsParinte] = aux;

			// actualizare ofset curent cu ofset parinte
			ofsCurent = ofsParinte;
		}
		else {
			// relatia de ordine este respectata
			flagStopFiltrare = 1; // forte iesirea din comparatii succesive cu parinti ai nodului curent
		}
	}

	return hs; // poate fi adresa cu care a intrat in functie SAU nou adresa prin realocare pe capacitate sporita de stocare
}


void main() {
	int * heapStr, size, capacitate, keyTemp;
	// capacitate - nr maxim de chei pe care structura le poate stoca
	// size - dimensiunea efectiva a vectorulu; nr de noduri din str heap la un moment dat

	FILE * f;
	f = fopen("chei.txt", "r");

	// alocare spatiu pentru structura heap heapStr
	capacitate = EXTEND_CAPACITY_SIZE;
	size = 0;
	heapStr = (int*)malloc(capacitate * sizeof(int));

	fscanf(f, "%d", &keyTemp);
	while (!feof(f)) {

		// inserare cheie keyTemp in structura heap heapStr
		heapStr = inserareNodStructHeap(heapStr, size, capacitate, keyTemp);

		fscanf(f, "%d", &keyTemp);
	}

	// traversare structura heap cu evidentiere niveluri arborescenta
	int nivel = 1, j = 0;
	for (int i = 0; i < size; i++) {
		printf(" %d ", heapStr[i]);
		j += 1;
		if (j == nivel) {
			printf("\n");
			nivel = nivel * 2;
			j = 0;
		}
	}

	// inserare cheie 26 in structura heap
	heapStr = inserareNodStructHeap(heapStr, size, capacitate, 26);
	heapStr = inserareNodStructHeap(heapStr, size, capacitate, 32);
	printf("\n  Structura heap dupa inserarea cheii 26:\n\n");
	nivel = 1, j = 0;
	for (int i = 0; i < size; i++) {
		printf(" %d ", heapStr[i]);
		j += 1;
		if (j == nivel) {
			printf("\n");
			nivel = nivel * 2;
			j = 0;
		}
	}

	// stergere nod in structura heap

	fclose(f);
}

