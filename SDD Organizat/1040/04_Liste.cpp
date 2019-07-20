#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct Carte {
	int id;
	char nrAutori;
	char ** autori;
	char * titlu;
};

struct Nod {
	Carte book;
	Nod* next;
};

Nod* sortareBubble(Nod* l) {
	if (!l || !l->next)
		return l;

	char vb = 1;
	Nod *p, *q, *r, *s;
	while (vb) {
		vb = 0; // nu are loc nici o interschimbare

		if (l->book.id > l->next->book.id) {
			// criteriul de sortare neindeplinit intre primul si al doilea nod
			p = l->next;
			q = p->next;

			l->next = q;
			p->next = l;
			l = p;
			vb = 1;
		}

		Nod* t = l;
		while (t->next->next) { // t->next->next corespunde nodului j
			p = t->next; // p este nodul i
			q = p->next; // q este nodul j
			if (p->book.id > q->book.id) {
				r = q->next; // corespunde lui j+1

				t->next = q;
				p->next = r;
				q->next = p;
				vb = 1;
			}

			t = t->next;
		}
	}

	return l;
}

Nod* inserareNodLista(Nod* l, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod)); // alocare nod in heap seg
	nou->book = c;
	nou->next = l;

	return nou; // noul inceput de lista salvat in apelator in pointerul de gestionare a listei simple
}

// parsare lista simpla
void parseList(Nod* l) {
	while (l) {
		printf("Id carte: %d\n", l->book.id);
		for (char i = 0; i < l->book.nrAutori; i++)
			printf("\t%s\n", l->book.autori[i]);


		l = l->next;
	}
}

Nod* dezalocareLista(Nod* l) {
	while (l) {
		Nod* t = l; // nodul care se dezaloca

		l = l->next; // noul inceput de lista

		for (char i = 0; i < t->book.nrAutori; i++)
			free(t->book.autori[i]); // dezalocare linie i din matricea de char (nume autori)
		free(t->book.autori);

		free(t->book.titlu); // dezalocare heap seg pt titlu carte

		free(t); // dezalocare nod
	}

	return l;
}

void main() {
	Nod* list = NULL; // list = 0;
	Carte temp;
	FILE* f;
	char buffer[150];


	f = fopen("books.txt", "r");

	fgets(buffer, sizeof(buffer), f); // citire id carte 1
	while (!feof(f)) {
		temp.id = atoi(buffer); // conversie string la intreg pe 4 baiti (int)

		fgets(buffer, sizeof(buffer), f); // citire nr autori din fisier
		temp.nrAutori = atoi(buffer); // conversie string la intreg pe 1 bait (char)

		temp.autori = (char**)malloc(temp.nrAutori * sizeof(char*));

		for (char i = 0; i < temp.nrAutori; i++) {
			fgets(buffer, sizeof(buffer), f); // citire nume autor i din fisier
			char * token = strtok(buffer, "\n");
			temp.autori[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); // alocare heap seg pt nume autor i

			strcpy(temp.autori[i], buffer); // copiere nume autor din buffer in heap seg alocat anterior
		}

		fgets(buffer, sizeof(buffer), f); // citire titlu carte din fisier
		char * token = strtok(buffer, "\n");
		temp.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char)); // alocare heap pt titlu carte
		strcpy(temp.titlu, buffer); // copiere titlu carte in heap alocat anterior
		token = strdup(temp.titlu);

		// inserare carte ca nod in lista simpla list (functie)
		list = inserareNodLista(list, temp);

		fgets(buffer, sizeof(buffer), f); // citeire id carte urmatoare in fisier
	}


	// functie pentru parsare lista simpla
	parseList(list);

	// functie pentru stergerea unui nod in lista simpla

	// functie pentru sortarea nodurilor dn lista simpla (id este criteriul de sortare)
	list = sortareBubble(list);
	printf("\nLista dupa sortare:\n");
	parseList(list);

	// functie pentru dezalocarea structurii de date de tip lista
	list = dezalocareLista(list);
	printf("Lista dupa dezalocare:\n");

	parseList(list);



	// inserare nod in lista dubla
	// dezalocare structura de tip lista dubla
	// sortare noduri lista dubla cu modificarea adreselor de legatura
	// determinare ("lista" de carti) carti cu un numar minim de autori (param de intrare in functie)

	fclose(f);
}