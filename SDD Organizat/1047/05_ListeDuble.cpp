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

struct NodD {
	Carte book;
	NodD *next, *prev;
};

struct ListaDbl {
	NodD *p, *u;
};

ListaDbl inserareListaDubla(ListaDbl lstD, Carte c) {
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

void parseListaD(ListaDbl lstD) {
	NodD *t = lstD.p;
	while (t) {
		printf("Id carte: %d\n", t->book.id);
		for (char i = 0; i < t->book.nrAutori; i++)
			printf("\t %s \n", t->book.Autori[i]);
		printf("\n");

		t = t->next;
	}
}

ListaDbl interschimbNoduriAdiacente(ListaDbl lstD, int idc) {
	if (lstD.p == NULL || lstD.p->next == NULL)
		return lstD;

	NodD *t = lstD.p;
	while (t) { // parsare lista dubla pt identificarea nodului i
		if (t->book.id == idc) {
			// am identificat nodul i
			// nodul i se interschimba cu i+1
			NodD *p, *r, *s;
			if (t == lstD.p && t->next == lstD.u) {
				// exista 2 noduri in lista dubla si acestea se interschimba
				r = t->next; // r = lstD.u; este ultimul nod
				t->next = NULL;
				t->prev = r;
				r->next = t;
				r->prev = NULL;
				lstD.p = r;
				lstD.u = t;
			}
			else {
				if (t == lstD.p) {
					// se interschimba 1 cu 2
					r = t->next;
					s = r->next;
					t->next = s;
					t->prev = r;
					r->next = t;
					r->prev = NULL; // r devine primul nod in lista dubla
					s->prev = t;
					lstD.p = r;
				}
				else {
					if (t->next == lstD.u) {
						//se interschimba m-1 cu m
						p = t->prev;
						s = t->next;

						p->next = s;
						t->next = NULL; // t devine ultimul nod
						t->prev = s;
						s->next = t;
						s->prev = p;
						lstD.u = t;
					}
					else {
						// se intreschimba i cu i+1
						p = t->prev;
						r = t->next;
						s = r->next;

						p->next = r;
						t->next = s;
						r->next = t;
						t->prev = r;
						r->prev = p;
						s->prev = t;
					}
				}

			}
		}

		t = t->next;
	}

	return lstD;
}

void main() {
	Carte temp;
	FILE *f = fopen("books.txt", "r");
	char buffer[100];

	ListaDbl listD;
	listD.p = listD.u = NULL;

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
		// inserare carte temp in lista duble listD
		listD = inserareListaDubla(listD, temp);

		// citire linie urmatoare in fisierul books.txt
		fgets(buffer, sizeof(buffer), f); // citire id carte next
	}

	// afisare lista dubla
	parseListaD(listD);

	listD = interschimbNoduriAdiacente(listD, 144);

	// afisare lista dubla dupa interschimb
	printf("Lista dupa interschimb:\n");
	parseListaD(listD);

	// sortare cu apel de interschimb noduri adiacente
	// implementare pt interschimb de noduri oarecare in lista dubla
	// functii diverse de sortare prin apel la interschimb (cele 2 versiuni)

	fclose(f);
}