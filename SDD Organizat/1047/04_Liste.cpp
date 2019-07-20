#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct Carte {
	int id;
	char* titlu;
	char nrAutori;
	char** Autori;
	char* ISBN;
};

struct Nod {
	Carte book;
	Nod* next;
};

// functie de inserare nod la sfarsitul listei simple
Nod* inserareLSSfarsit(Nod* l, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod)); // alocare nod in heap seg.

	nou->book = c; // adresele din c se copiaza peste nou->book
	nou->next = 0; // NULL

	if (!l) { // l este pointer NULL; lista nu are nici un nod
		return nou;
	}
	else { // lista l contine cel putin 1 nod
		Nod* t = l;
		while (t->next) {
			t = t->next;
		}

		// sunt pe ultimul nod; actualizez campul next cu adresa noului nod alocat anterior
		t->next = nou;
	}

	return l;
}

void parseList(Nod* l) {
	while (l) {
		printf("Id carte: %d\n", l->book.id);
		printf("Lista autori:\n");
		for (char i = 0; i < l->book.nrAutori; i++)
			printf("\t %s\n", l->book.Autori[i]);
		printf("\n");

		l = l->next;
	}
}

int* cautaAutorCarti(Nod* l, char* nume, int *m) {
	*m = 0; // initilizare dimensiune vector
	// parsare lista simpla pentru identificarea cartilor cu autor dat
	Nod* t = l;
	while (t) { // t este un nod curent in parsarea listei
		for (char i = 0; i < t->book.nrAutori; i++) {
			if (strcmp(t->book.Autori[i], nume) == 0) {
				*m += 1; // incrementez dimensiune vector
				i = t->book.nrAutori;
			}
		}

		t = t->next; // urmatorul nod devine nod curent in traversarea listei
	}

	// avem numarul de elemente din vector = nr de carti cu autorul nume
	// alocare vector in heap seg
	
	int * v = NULL;
	if (*m) {
		v = (int*)malloc(*m * sizeof(int));
		char k = 0;

		// reparsare lista simpla pentru salvarea id-urilor de carte in vectorul rezultat
		t = l;
		while (t) {
			for (char i = 0; i < t->book.nrAutori; i++) {
				if (strcmp(t->book.Autori[i], nume) == 0) {
					v[k] = t->book.id; // salvare id carte in vector pe pozitia k
					k += 1; // incrementare pozitie curenta in vector
					i = t->book.nrAutori;
				}
			}

			t = t->next; // urmatorul nod devine nod curent in traversarea listei
		}
	}

	return v; // return adresa de heap seg unde se afla id-urile cartilor cu autorul dat prin param nume
}

void main() {
	Carte temp;
	FILE *f = fopen("books.txt", "r");
	char buffer[100];

	Nod* list = NULL; // 0

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
		// inserare carte in lista simpla
		list = inserareLSSfarsit(list, temp);
		
		// citire linie urmatoare in fisierul books.txt
		fgets(buffer, sizeof(buffer), f); // citire id carte next
	}

	// traversare lista simpla
	parseList(list);
	
	// stergere nod din lista simpla
	// identificare carti cu un autor specificat ca parametru de intrare
	int *vid, m = 0;
	// sortare lista pe baza de id carte
	char nume[] = "Grigorescu I";
	vid = cautaAutorCarti(list, nume, &m);

	printf("Id carti cu autor %s:\n", nume);
	for (int i = 0; i < m; i++) {
		printf(" %d ", vid[i]);
	}
	printf("\n");


	// sortare noduri in lista simpla

	// creare lista dubla
	// stergere nod din lista dubla
	// determinare numar carti cu cel putin 2 autori in lista dubla
	// stergerea listei duble
	// sortarea nodurilor in lista dubla
	fclose(f);
}