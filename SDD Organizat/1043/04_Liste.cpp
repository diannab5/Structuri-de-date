#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

struct Carte {
	int id;
	char* titlu;
	char nrAutori;
	char** autori;
};

struct Nod {
	Carte book;
	Nod* next;
};

// inserare nod la inceputul listei simple
Nod* inserareNodLista(Nod* l, Carte c) {
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
			printf("\t%s\n", l->book.autori[i]);


		l = l->next;
	}
}

int* cartiNumarAutori(Nod* l, char nr_autori, int *n) {
	*n = 0; // initializare dimensiune vector rezultat; intreg transmis prin adresa la functie
	Nod* t = l;
	while (t) {
		// t - nod curent din lista
		if (t->book.nrAutori == nr_autori) {
			*n += 1;
		}

		t = t->next;
	}

	// definire si alocare vector de id carte
	int* vid = (int*)malloc(*n * sizeof(int));
	int k = 0; // index de inserare in vectorul vid

	// re-parsare lista simpla
	t = l;
	while (t) {
		if (t->book.nrAutori == nr_autori) {
			vid[k] = t->book.id;
			k += 1; // urmatoarea positie in vector devine disponibila la inserare de id carte
		}

		t = t->next;
	}

	return vid;
}

// interschimbare noduri oarecare in lista simpla
Nod* interschimbNoduri(Nod* l, int id1, int id2) {
	if (!l || !l->next) return l;
	Nod *p, *q, *r, *s, *t, *u;
	q = l;
	int idcurent;
	if (q->book.id == id1 || q->book.id == id2) {
		// nodul i este primul din lista
		r = q->next;
		if (q->book.id == id1)
			idcurent = id2; // idcurent este id de cautat pentru j
		else
			idcurent = id1; // idcurent este id de cautat pentru j
		s = q;
		while (s->next) {
			// caut nodul j dupa idcurent
			if (s->next->book.id == idcurent) {
				// am identificat nodul j
				t = s->next;
				u = t->next;

				q->next = u;
				if (s != q) {
					s->next = q;// doar pe caz general
					t->next = r;
				}
				else {
					// s este q, deci interschimb noduri adiacente
					t->next = q;
				}
				
				return t; // t este noul nod de inceput al listei
			}

			s = s->next;
		}

	}
	else {
		// nodul i nu este primul din lista
		p = l;
		while (p->next) {

			if (p->next->book.id == id1 || p->next->book.id == id2) {
				q = p->next;
				r = q->next;
				if (q->book.id == id1)
					idcurent = id2; // idcurent este id de cautat pentru j
				else
					idcurent = id1; // idcurent este id de cautat pentru j
				s = q;

				// parsarea restului d elista pt identificarea lui j
				while (s->next) {
					// caut nodul j dupa idcurent
					if (s->next->book.id == idcurent) {
						// am identificat nodul j
						t = s->next;
						u = t->next;

						p->next = t;
						q->next = u;
						if (s != q) {
							// caza general interschimb
							s->next = q;
							t->next = r;
						}
						else {
							// caz particular adiacente
							t->next = q;
						}
						return l; // nu se modifica inceputul de lista
					}

					s = s->next;
				}
			}

			p = p->next;
		}
	}

	return l; // executat atunci cand nu s-au identificat i si j valizi in lista simpla
}


void main() {
	Nod* list = 0;
	FILE * f;
	Carte temp;
	char buffer[100];

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

		// inserare carte (temp)/nod in lista simpla - functie
		list = inserareNodLista(list, temp);

		fgets(buffer, sizeof(buffer), f); // citire id carte urmatoare din fisier
	}

	// functie de parsare a listei simple
	parseList(list);

	// functie de stergere nod in lista simpla

	// functie pentru determinarea cartilor (la nivel de id) cu acelasi numar de autori (param I/ al functiei)
	int *v, n;
	v = cartiNumarAutori(list, 2, &n); // filtrare carti din lista simpla cu numar de autori = 2
	// afisare continut vector de id-uri
	printf("\nCarti filtrate dupa numar de autori:");
	for (int i = 0; i < n; i++)
		printf(" %d ", v[i]);
	printf("\n\n");

	// interschimb noduri oarecare in lista simpla
	list = interschimbNoduri(list, 234, 34);
	printf("\n\nLista dupa interschimb:\n");
	parseList(list);

	// functie pentru dezalocarea/stergerea completa a listei simple

	// testare functie interschimb
	// sortare lista simpla cu apel de functie de interschimb de mai sus
	// creare lista dubla de carti
	// stergere lista dubla de carti
	// functie pt interschimb de noduri oarecare (cu modif legaturilor) in lista dubla
	// sortare noduri in lista dubla apeland functia de interschmb de mai sus

	fclose(f);
}