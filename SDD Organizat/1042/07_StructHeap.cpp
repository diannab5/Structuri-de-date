#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define INIT_HEAP_STRUCTURE 20

int * inserareNodStructHeap(int* hs, int &size, int &capacitate, int key) {
	if (size == capacitate) {
		// vectorul este incarcat la maxim
		// realocare vector si mutarea elementelor din vechiul vector
		capacitate += INIT_HEAP_STRUCTURE;
		int * newHStruct = (int *)malloc(capacitate * sizeof(int));
		// mutare elemente
		for (int i = 0; i < size; i++) {
			newHStruct[i] = hs[i];
		}

		// dezalocare veche structura heap
		free(hs);
		hs = newHStruct; // actualizare hs cu noua adresa a vectorului de capacitate sporita
	}

	// inserare cheie key in structura heap
	hs[size] = key;
	size += 1;

	// filtrare bottom-up pentru noua cheie key inserata
	char stopFiltrare = 0;
	int offsetCurent, offsetParinte;
	offsetCurent = size - 1;
	while (!stopFiltrare && offsetCurent != 0) {
		offsetParinte = (offsetCurent - 1) / 2;
		if (hs[offsetCurent] > hs[offsetParinte]) {
			// criteriul de ordonare nu este indeplinit
			int aux = hs[offsetCurent];
			hs[offsetCurent] = hs[offsetParinte];
			hs[offsetParinte] = aux;
			offsetCurent = offsetParinte;
		}
		else {
			// criteriul este indeplinit
			stopFiltrare = 1;
		}
	}

	return hs;
}


void main() {
	FILE* f;
	f = fopen("chei.txt", "r");

	int * heapStruct, capacitate, size = 0, keyTemp; // capacitate este numarul maxim de noduri care pot fi stocate in structura heap
	// size - dimensiune efectiva a vectorului: numarul de noduri din structura heap la un moment dat
	capacitate = INIT_HEAP_STRUCTURE;

	// alocare structura heap
	heapStruct = (int *)malloc(capacitate * sizeof(int));

	fscanf(f, "%d", &keyTemp);
	while (!feof(f)) {		
		// inserare cheie keyTemp in structura heap
		heapStruct = inserareNodStructHeap(heapStruct, size, capacitate, keyTemp); // inserare cheie in vector cu posibila realocare a vectorului

		fscanf(f, "%d", &keyTemp);
	}
	
	// parsare structura heap
	int nivel = 1, j = 0;
	for (int i = 0; i < size; i++) {
		j += 1;
		printf(" %d ", heapStruct[i]);
		if (nivel == j) {
			printf("\n");
			nivel = nivel * 2;
			j = 0;
		}
	}
	printf("\n\n");

	heapStruct = inserareNodStructHeap(heapStruct, size, capacitate, 25); 
	nivel = 1, j = 0;
	for (int i = 0; i < size; i++) {
		j += 1;
		printf(" %d ", heapStruct[i]);
		if (nivel == j) {
			printf("\n");
			nivel = nivel * 2;
			j = 0;
		}
	}
	
	printf("\n\n");

	// stergere/extragere nod din structura heap

}