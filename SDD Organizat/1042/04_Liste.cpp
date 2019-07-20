#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct Carte {
	int id;
	char nrAutori;
	char** Autori;
	char * titlu;
};

struct Nod {
	Carte book;
	Nod * next;
};

// inserare nod in lista simpla
Nod* inserareNodLS(Nod* l, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->book = c;
	nou->next = l;

	return nou;
}

// parsare lista simpla
void parseList(Nod* l) {
	while (l) {
		printf("Id carte: %d\n", l->book.id);
		for (char i = 0; i < l->book.nrAutori; i++)
			printf("\t%s\n", l->book.Autori[i]);

		l = l->next;
	}
}

// filtrare carti dupa nume autor
int* cautaCartiAutor(Nod* l, char* nume, int *m) { // m - nr de carti filtrate dupa nume autor
	*m = 0;
	Nod* t = l;
	while (t) {
		// t - nod curent
		for (char i = 0; i < t->book.nrAutori; i++) {
			if (strcmp(t->book.Autori[i], nume) == 0) {
				*m += 1;
				i = t->book.nrAutori;
			}
		}

		t = t->next; // acces la urmatorul nod din lista simpla
	}

	// *m - numarul de carti cu autorul cautat (nume)
	int* vid = (int*)malloc(*m * sizeof(int)); // alocare vector de id-uri carti filtrate dupa nume autor
	int k = 0; // index elemente curent in vector de id-uri

	// re-parsare lista simpla
	t = l; // t contine din nou adresa primului nod in lista
	while (t) {
		for (char i = 0; i < t->book.nrAutori; i++) {
			if (strcmp(t->book.Autori[i], nume) == 0) {
				vid[k] = t->book.id; // salvez id carte in vector id-uri
				k += 1;
				i = t->book.nrAutori;
			}
		}

		t = t->next;
	}
	
	return vid;
}

// dezalocare lista simpla
Nod* stergLista(Nod* l) {
	while (l) {
		Nod* t = l; // t - nodul care se dezaloca
		l = l->next; // noul inceput de lista va fi nodul succesor

		free(t->book.titlu);
		// dezalocare matrice de char pt numele autorilor
		for (char i = 0; i < t->book.nrAutori; i++)
			free(t->book.Autori[i]); // dezalocare linie i in matricea de char
		free(t->book.Autori); // dezalocare vector de pointeri la linii

		free(t); // dezalocare nod
	}

	return l;
}

void main() {
	Nod* prim = NULL; // prim = 0;
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
		temp.titlu = (char*)malloc((strlen(buffer) + 1) *sizeof(char));
		strcpy(temp.titlu, buffer);

		// Cartea este initializata; se poate adauga la lista simpla
		// apel functie inserare nod in lista simpla
		prim = inserareNodLS(prim, temp);

		fgets(buffer, sizeof(buffer), f); // citire id pentru cartea urmatoare din fisier
	}

	// apel functie de parsare lista simpla
	parseList(prim);

	// apel functie de stergere nod din lista simpla

	// apel functie pentru construirea listei cu titluri de carte pentru un autor specificat ca param I/
	int m; // dimensiune vector v
	char nume[] = "Georgescu M";
	int* v = cautaCartiAutor(prim, nume, &m);

	// afisare lista id-uri carte pentru autorul specificat prin nume
	printf("\nId carti pentru autorul %s:", nume);
	for (int i = 0; i < m; i++)
		printf(" %d ", v[i]);
	printf("\n");

	// dezalocare lista
	prim = stergLista(prim);
	printf("Lista dupa dezalocare:\n");
	parseList(prim);

	// sortare lista simpla (cu modificarea adreselor de legatura - campul next este modificat pentru nodurile implicate in interschimb)
	// creare lista dubla de carti
	// stergere lista dubla de carti
	// filtrare noduri din lista dubla in functie de numarul de autori (>3)
	// sortare lista dubla (prin modificarea adreselor de legatura - campul next este modificat pentru nodurile implicate in interschimb)

	fclose(f);
}