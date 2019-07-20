#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define HASH_TABLE_SIZE 200

struct Carte {
	int id;
	char* titlu;
	char nrAutori;
	char** autori;
};

// functie hash 
int hash(int idc, int sizeHT) {
	return idc % sizeHT;
}

// inserare carte in tabela hash
Carte* inserareHTable(Carte* ht, int &size, Carte c) {
	int i = hash(c.id, size); // i este pozitia pe care tb sa ajunga c

	char vb = 1;
	while (vb) {
		if (ht[i].titlu == NULL) {
			// i este disponibil la inserare
			ht[i] = c;
			vb = 0;
		}
		else {
			// exista conflict
			// 1. cautare prima pozitie disponibila		
			int j;
			for (j = i + 1; j < size; j++) {
				if (ht[j].titlu == NULL) {
					ht[j] = c;
					j = size + 1; // flag de inserare cu succes
					vb = 0;
				}
			}

			// 2. nu exista pozitie disponibila (cluster-ul curent se inchide pe final de tabela hash)
			if (j == size) {			
				// nu am identificat pozitie disponibila
				// tabela se realoca pe dimensiune mai mare
				int oldSize = size; // pt tabela curenta
				size += HASH_TABLE_SIZE; // pt noua tabela
				Carte * newHT = (Carte*)malloc(size * sizeof(Carte));
				// initializare campuri titlu
				for (int k = 0; k < size; k++)
					newHT[k].titlu = NULL;

				// mutarea cartilor din tabela ht in noua tabela newHT
				for (int k = 0; k < oldSize; k++) {
					if (ht[k].titlu)
						newHT = inserareHTable(newHT, size, ht[k]);
				}

				// dezalocare tabela veche
				free(ht);
				// noua tabela devine tabela curenta
				ht = newHT;

				// inserare parametru carte c
				ht = inserareHTable(ht, size, c);
				vb = 0; // inserare cu succes al lui c in apelul curent
			}
		}
	}

	return ht;
}

Carte cautaCarteHT(Carte* ht, int size, int idc) {
	int i = hash(idc, size);

	for (int j = i; j < size; j++) {
		if(ht[j].titlu) {
			// poziti j este ocupata de o carte
			if (ht[j].id == idc) {
				return ht[j];
			}
		}
		else {
			j = size;
		}
	}

	Carte book;
	book.titlu = NULL;
	return book;
}

void stergereHT(Carte* ht, int size, int ids) {
	int i = hash(ids, size);
	int poz = -1;
	for (int j = i; j < size; j++) {
		if (ht[j].id == ids) {
			poz = j;
			j = size;
		}
	}

	if (poz != -1) {
		// am identificat pozitia elementului de sters din tabela de dispersie

		// 1. dezalocari element de pe poz
		free(ht[poz].titlu);
		for (int j = 0; j < ht[poz].nrAutori; j++)
			free(ht[poz].autori[j]);
		free(ht[poz].autori);
		ht[poz].titlu = NULL;

		// 2. determinare numar de carti din subcluster dreapta [poz + 1; end_idx]; end_idx = size SAU < size

		int nrc = 0;
		for (int j = poz + 1; j < size; j++) {
			if (ht[j].titlu)
				nrc += 1;
			else {
				j = size;
			}
		}

		// 3. alocare vector temporar de carti pe dimensiunea calculata la pasul 2
		Carte* vTemp = (Carte*)malloc(nrc * sizeof(Carte));

		// 4. mutare carti din sub-cluster dreapta in vector
		for (int j = 0; j < nrc; j++) {
			vTemp[j] = ht[(poz + 1) + j];
			// 5. disponibilizare pozitii sub-cluster dreapta la scriere in vederea reinserarii elementelor din vectorul temporar
			ht[(poz + 1) + j].titlu = NULL;
		}

		// 6. reinserarea cartilor din vectorul temporat inapoi in tabela de dispersie
		for (int j = 0; j < nrc; j++) {
			ht = inserareHTable(ht, size, vTemp[j]);
		}
	}
}

void main() {
	Carte *hashTable;
	// alocare memorie heap pt tabela hash
	hashTable = (Carte*)malloc(HASH_TABLE_SIZE * sizeof(Carte));

	// intializare camp titlu pt toate locatiile din tabela
	// disponibilizez la scriere toate cele 200 de locatii Carte
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		hashTable[i].titlu = NULL; // rol de flag in a semnala pozitie disponibila la scriere

	FILE * f;
	Carte temp;
	char buffer[100];
	int size = HASH_TABLE_SIZE;

	f = fopen("books.txt", "r");

	fgets(buffer, sizeof(buffer), f); // citire id carte 1 din fisier
	while (!feof(f)) {
		temp.id = atoi(buffer);

		fgets(buffer, sizeof(buffer), f); // citire titlu carte
		char* token = strtok(buffer, "\n"); // eliminare CR (0x0A) de la sfarsitul lui buffer
		temp.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(temp.titlu, buffer); // copiere  titlyu carte in heap pe zona alocata anterior

		fgets(buffer, sizeof(buffer), f); // citire numar autori carte
		temp.nrAutori = atoi(buffer);

		// alocare si initializare matrice pentru "lista" de autori ai cartii
		temp.autori = (char**)malloc(temp.nrAutori * sizeof(char*));

		for (char i = 0; i < temp.nrAutori; i++) {
			fgets(buffer, sizeof(buffer), f); // citire nume autor i 
			token = strtok(buffer, "\n");
			temp.autori[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); // alocare pentru nume autor
			strcpy(temp.autori[i], buffer);
		}

		// inserare in tabela de dispersie
		hashTable = inserareHTable(hashTable, size, temp);

		fgets(buffer, sizeof(buffer), f); // citire id carte urmatoare din fisier
	}

	temp.titlu = NULL;
	temp.autori = NULL;

	// traversare tabela hash
	for (int i = 0; i < size; i++) {
		if (hashTable[i].titlu) {
			printf(" Pozitie: %d, Id carte: %d\n", i, hashTable[i].id);
		}
	}

	temp = cautaCarteHT(hashTable, size, 255);
	if (temp.titlu) {
		printf("Cartea este: %d %s\n", temp.id, temp.titlu);
	}
	else {
		printf("Cartea nu exista.\n");
	}

	stergereHT(hashTable, size, 99);
	// traversare tabela hash
	printf("\n Tabela dupa stergere carte:\n");
	for (int i = 0; i < size; i++) {
		if (hashTable[i].titlu) {
			printf(" Pozitie: %d, Id carte: %d\n", i, hashTable[i].id);
		}
	}

	fclose(f);
}