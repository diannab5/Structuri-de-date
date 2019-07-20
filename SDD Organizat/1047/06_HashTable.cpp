#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define DIM_VECTOR 200

struct Carte {
	int id;
	char* titlu;
	char nrAutori;
	char** Autori;
	char* ISBN;
};

int functieHash(int idc, int size) {
	return idc % size;
}

Carte* inserareHashTable(Carte* ht, int &size, Carte c) {
	int i = functieHash(c.id, size);

	if (ht[i].titlu == NULL) {
		// nu am coliziune; cartea c va ajunge pe pozitia i
		ht[i] = c;
	}
	else {
		// exista coliziunea; eliminare prin linear probing
		int j;
		char vb = 0; // boolean pt semnalare op inserare a lui c in tabela de dispersie
		while (vb == 0) {
			for (j = i + 1; j < size; j++) {
				if (ht[j].titlu == NULL) {
					// am gasit pozitie disponibila
					// inserez cartea c
					ht[j] = c;
					vb = 1; // cartea c a fost inserata
					j = size + 1; // fortez iesire din do-for
				}
			}

			if (j == size) { // nu am gasit pozitie disponibila in intervalul [i+1, size-1]
				int newSize = size + DIM_VECTOR;
				Carte * htNew = (Carte*)malloc(newSize * sizeof(Carte));
				// disponibilizare carti la scriere in noua tabela
				for (int k = 0; k < newSize; k++) {
					htNew[k].titlu = NULL;
				}

				// mutare elemente din tabela veche in noua tabela hash
				// mutarea presupune reinserarea cartilor pe noua tabela cu dimensiunea mai mare
				for (int k = 0; k < size; k++) {
					if (ht[k].titlu != NULL) {
						// exista carte pe pozitia k in tabela veche ht
						htNew = inserareHashTable(htNew, newSize, ht[k]); // posibil realocare; posibil noua adresa inceput vector (tabela hash)
					}
				}
				// toate elemente sunt mutate din ht in htNew
				// actualizare ht si size cu noile coordonate ale tabelei realocate
				free(ht);
				ht = htNew;
				size = newSize;
			}
		}
	}

	return ht;
}

// cautare in tabela de dispersie
Carte cautaCarte(Carte* ht, int size, int idc) {
	int poz = functieHash(idc, size);
	for (int i = poz; i < size; i++) {
		if (ht[i].titlu != NULL) {
			if (ht[i].id == idc)
				return ht[i]; // am gasit cartea cautata in tabela hash
		}
		else {
			// am terminat parsare cluster
			// nu a fost identificata cartea cu idc
			i = size; // fortez iesire din do-for
		}
	}

	Carte t;
	t.titlu = NULL;
	return t;
}

void main() {
	Carte *hashT; // vectorul suport pentru tabela de dispersie 
	hashT = (Carte*)malloc(DIM_VECTOR * sizeof(Carte)); // alocare vector de DIM_VECTOR carti in heap seg

	for (int i = 0; i < DIM_VECTOR; i++)
		hashT[i].titlu = NULL; // initial, toate locatiile din tabela hash sunt disponibile la inserare carte in structura tabela hash

	Carte temp;
	FILE *f = fopen("books.txt", "r");
	char buffer[100];
	int size = DIM_VECTOR;

	fgets(buffer, sizeof(buffer), f); // citire id carte 1
	while (!feof(f)) {
		temp.id = atoi(buffer);

		fgets(buffer, sizeof(buffer), f); // citire titlu carte
										  // buffer[strlen(buffer) - 1] = 0;
		char *token = strtok(buffer, "\n");
		temp.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(temp.titlu, buffer);

		fgets(buffer, sizeof(buffer), f); // citire numar de autori
		temp.nrAutori = atoi(buffer);

		temp.Autori = (char**)malloc(temp.nrAutori * sizeof(char*));
		for (char i = 0; i < temp.nrAutori; i++) {
			fgets(buffer, sizeof(buffer), f); // citire autor i din lista de autori din fisier
											  // buffer[strlen(buffer) - 1] = 0;
			token = strtok(buffer, "\n");
			temp.Autori[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
			strcpy(temp.Autori[i], buffer);
		}

		fgets(buffer, sizeof(buffer), f); // citire ISBN carte
		temp.ISBN = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(temp.ISBN, buffer);

		// cartea este preluata in intregime din fisier
		// inserare carte temp in tabela de dispersie
		hashT = inserareHashTable(hashT, size, temp);

		// citire linie urmatoare in fisierul books.txt
		fgets(buffer, sizeof(buffer), f); // citire id carte next
	}

	// parsare tabela de dispersie
	printf("Tabela de dispersie dupa creare:\n");
	for (int i = 0; i < size; i++) {
		if (hashT[i].titlu != NULL) {
			printf("poz: %d, id carte: %d\n", i, hashT[i].id);
		}
	}

	// cauta carte in tabela hash
	temp = cautaCarte(hashT, size, 599);
	if (temp.titlu != NULL) {
		// am gasit cartea
		printf("\nId carte: %d, titlu carte: %s\n", temp.id, temp.titlu);
	}
	else {
		// nu a fost identificata cartea
		printf("Cartea nu exista in tabela hash\n");
	}

	// stergere carte din tabela hash

	fclose(f);
}