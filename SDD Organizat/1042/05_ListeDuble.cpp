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

struct NodD {
	Carte book;
	NodD *next, *prev;
};

struct ListaDubla {
	NodD *p, *u;
};

// interschimb nodul t cu succesorul sau
ListaDubla interschimbAdiacente(NodD *t, ListaDubla l) {
	NodD *q, *r, *s;
	// test t este primul nod
	if (t == l.p) {
		r = t->next;
		s = r->next;

		t->next = s;
		t->prev = r;
		r->next = t;
		r->prev = NULL;

		if (s) {
			// exista cel putin 3 noduri in lista dubla
			s->prev = t;
		}
		else {
			// exista doar 2 noduri in lista dubla si cele doua se interschimba
			l.u = t;
		}

		l.p = r;
	}
	else {
		q = t->prev;
		r = t->next;
		s = r->next;

		t->next = s;
		t->prev = r;
		r->next = t;
		r->prev = q;
		q->next = r;

		if (s) {
			// t nu este penultimul nod; nu se actualizeaza sfarsitul de lista
			s->prev = t;
		}
		else {
			// t este penultimul nod, se actualizeaza sfarsitul de lista dubla
			l.u = t;
		}
	}

	return l;
}

// sortare lista dubla prin BubbleSort
ListaDubla sortareBubble(ListaDubla l) {
	if (!l.p || !l.p->next)
		return l;

	char vb = 1; // pt a forta intrarea in structura do-while
	while (vb) {
		vb = 0; // ipoteza: nu exista interschimb
		NodD *t = l.p;
		while (t->next) {
			if (t->book.id > t->next->book.id) {
				// t si t->next nu respecta criteriul de ordonare
				l = interschimbAdiacente(t, l); // interschimb t cu t->next; nu stiu pozitia lui t in lista
				vb = 1;
			}
			else {
				t = t->next;
			}
		}
	}

	return l;
}

ListaDubla inserareListaDubla(ListaDubla lstD, Carte c) {
	NodD * nou;
	nou = (NodD*)malloc(sizeof(NodD));
	nou->book = c;
	nou->next = 0;
	nou->prev = lstD.u;

	if (!lstD.p)
		lstD.p = lstD.u = nou;
	else {
		lstD.u->next = nou;
		lstD.u = nou;
	}

	return lstD;
}

void parseListaD(ListaDubla lstD) {
	NodD *t = lstD.p;
	while (t) {
		printf("Id carte: %d\n", t->book.id);
		for (char i = 0; i < t->book.nrAutori; i++)
			printf("\t %s \n", t->book.Autori[i]);
		printf("\n");

		t = t->next;
	}
}

void main() {
	char buffer[150];
	FILE* f;
	Carte temp;
	f = fopen("books.txt", "r");

	ListaDubla list;
	list.p = list.u = NULL;

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

		// inserare nod in lista dubla
		list = inserareListaDubla(list, temp);

		fgets(buffer, sizeof(buffer), f); // citire id pentru cartea urmatoare din fisier
	}

	// apel functie de parsare lista duble
	parseListaD(list);

	// sortare bubble
	list = sortareBubble(list);
	printf("\n\nLista dupa sortare:\n");
	parseListaD(list);

	fclose(f);
}