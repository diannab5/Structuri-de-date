#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define DIM_HASH_TABLE 100

struct Carte {
	int id;
	char nrAutori;
	char** Autori;
	char * titlu;
};

int functieHash(int idc, int sizeTable) {
	return idc % sizeTable;
}

Carte* inserare(Carte* hashTable, int &size, Carte c)
{

	int pozitie = functieHash(c.id, size); // determin pozitia pe care c tb inserat
	if (hashTable[pozitie].titlu == NULL) // pozitie disponibila la inserare carte c
	{
		hashTable[pozitie] = c; // copiere byte cu byte c peste hashTable[pozitie]
	}
	else // coliziune; caut prima pozitie disponibila pana la size tabela hash
	{
		int i = pozitie + 1;
		while (i != size && hashTable[i].titlu != NULL)
		{
			i++;
		}
		if (i == size) // nu este loc in tabela pentru inserarea luic
		{ // realocare tabela pe dimensiune mai mare
		  // mutarea elementelor din tabela in noua tabela (prin reinserare deoarece size este modificat -> functie hash cu rezultat diferit)
			Carte *newHashTable;
			{
				size = 2 * size; // dimensiunea pentru noua tabela (rescriere parametru size; modificare vizibila in exterior pt ca size e dat cu &)
				newHashTable = (Carte*)malloc((size) * sizeof(Carte)); // alocare noua tabela pe noul size
				
				for (int i = 0; i < size; i++)
					newHashTable[i].titlu = NULL; // disponibilizez toate locatiile din tabela noua la scriere de carti
				
				for (int j = 0; j < size / 2; j++) // mutare elemente in noua tabela
				{
					if (hashTable[j].titlu != NULL) // mutarea are sens doar pentru carti valide din tabela hash
					{
						newHashTable = inserare(newHashTable, size, hashTable[j]); // reinserare carte[j] din vechea tabela; apel recursiv al functiei de inserare pt ca 
					}
				}
				newHashTable = inserare(newHashTable, size, c); // in sfarsit, inserarea lui c dupa realocare si mutare elemente in noua tabela hash
			}

			free(hashTable); // dezalocare tabela hash insuficient de mare
			hashTable = newHashTable; // actualizare tabela curenta cu adresa noii tabele hash
		}
		else
			if (hashTable[i].titlu == NULL)
				hashTable[i] = c;
	}
	return hashTable;
}

Carte cautareCarte(Carte* hashTable, int size, int id)
{
	int pozitie = functieHash(id, size);

	for (int i = pozitie; i < size; i++)
	{
		if (hashTable[i].titlu != NULL) {
			if (hashTable[i].id == id)
				return hashTable[i];
		}
		else {
			i = size; // fortez iesirea din do-for; cartea nu exista in tabela de dispersie
		}
	}

	Carte aux;
	aux.titlu = NULL; // flag pentru semnalarea inexistentei cartii in tabela hash
	return aux;
}

// stergere carte din tabela de dispersie
void stergere(Carte* hashTable, int size, int ids) {
	int pozitie = functieHash(ids, size);
	int pozSters = -1, i;

	for (i = pozitie; i < size; i++)
	{
		if (hashTable[i].titlu != NULL) {
			if (hashTable[i].id == ids) {
				pozSters = i;
				i = size;
			}
		}
		else {
			i = size; // fortez iesirea din do-for; cartea nu exista in tabela de dispersie
		}
	}

	if (pozSters != -1) {
		// elementul de sters este indetificat pe pozSters
		//dezalocare titlu si lista autori
		for (int j = 0; j < hashTable[pozSters].nrAutori; j++)
			free(hashTable[pozSters].Autori[j]);
		free(hashTable[pozSters].Autori);

		free(hashTable[pozSters].titlu);

		// disponibilizez pozSters pentru eventuala scriere ulterioara
		hashTable[pozSters].titlu = NULL;
		// reinserare sub-cluster dreapta in tabela de dispersie

		// 1. mutare temporara elemente in vector, disponibilizarea pozitiilor la scriere
		int sizeClusterDreapta = 0;
		Carte* tempCarti = NULL;
		for (int j = pozSters + 1; j < size; j++) {
			if (hashTable[j].titlu)
				sizeClusterDreapta += 1;
			else
				j = size;
		}
		if (sizeClusterDreapta != 0) {
			tempCarti = (Carte*)malloc(sizeClusterDreapta * sizeof(Carte)); // alocare vector temporar pt sub-cluster dreapta
			for (int j = 0; j < sizeClusterDreapta; j++) {
				tempCarti[j] = hashTable[(pozSters + 1) + j];
				hashTable[(pozSters + 1) + j].titlu = NULL; // disponibilizare pozitie la scriere
			}
		}
		
		// 2. inserarea elementelor din vectorul temporar in tabela de dispersie
		for (int j = 0; j < sizeClusterDreapta; j++) {
			hashTable = inserare(hashTable, size, tempCarti[j]);
		}

		// 3. dezalocare vector temporar de carti
		if (tempCarti) { // daca exista vector temporar (exista subcluster dreapta)
			free(tempCarti);
		}
	}
}

void main() {
	Carte * hashT;
	// alocare tabela hash
	hashT = (Carte*)malloc(DIM_HASH_TABLE * sizeof(Carte));

	// initializare titlu cu rol de flag la inserare carte in tabela de dispersie
	for (int i = 0; i < DIM_HASH_TABLE; i++)
		hashT[i].titlu = NULL;

	int size = DIM_HASH_TABLE;

	// parsare fisier de intrare books.txt si inserarea cartilor in tabela hash
	char buffer[150];
	FILE* f;
	Carte temp;
	f = fopen("books.txt", "r");

	fgets(buffer, sizeof(buffer), f); // citire initiala fin fisier (id)
	while (!feof(f)) {
		temp.id = atoi(buffer);

		fgets(buffer, sizeof(buffer), f);
		temp.nrAutori = atoi(buffer);

		// alocare vector de adrese de linii spre autori
		temp.Autori = (char**)malloc(temp.nrAutori * sizeof(char*));
		for (char i = 0; i < temp.nrAutori; i++) {
			fgets(buffer, sizeof(buffer), f); // citire nume autor
			char* token = strtok(buffer, "\n"); // eliminare "\n" din coada lui buffer
			strcpy(buffer, token);
			// alocare linie i
			temp.Autori[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
			// copiere nume autor in linia i alocata in heap seg
			strcpy(temp.Autori[i], buffer);
		}

		fgets(buffer, sizeof(buffer), f); // citire titlu carte
		char* token = strtok(buffer, "\n"); // eliminare "\n" din coada lui buffer
		strcpy(buffer, token);
		temp.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(temp.titlu, buffer);

		// carte intializata in temp
		// apel functie de inserare in tabela de dispersie

		// functia de inserare in tabela de dispersie conform linear probing
		// eventuala realocare de tabela daca pe punctul de coliziune nu se mai identifica pozitie disponibila pana la sfarsitul verctorului
		hashT = inserare(hashT, size, temp);

		fgets(buffer, sizeof(buffer), f); // citire id pentru cartea urmatoare din fisier
	}

	// parsare secventiala a tabelei de dispersie cu evidentierea cluster-elor de carti
	for (int i = 0; i < size; i++) {
		if (hashT[i].titlu) {
			printf(" Pozitie: %d, Id carte: %d\n", i, hashT[i].id);
		}
	}

	// functia de cautare a unei carti pe baza de id (cheie de cautare) in tabela de dispersie
	temp = cautareCarte(hashT, size, 399);
	if (temp.titlu) {
		// carte identificata in tabela hash
		printf("Carte identificata -  id: %d, titlu: %s\n\n", temp.id, temp.titlu);
	}
	else {
		// nu exista carte cu id cautat in tabela hash
		printf("Cartea nu exista in tabela hash.\n\n");
	}

	stergere(hashT, size, 254);
	printf("\n  Tabela dupa stergere:\n");
	for (int i = 0; i < size; i++) {
		if (hashT[i].titlu) {
			printf(" Pozitie: %d, Id carte: %d\n", i, hashT[i].id);
		}
	}
}