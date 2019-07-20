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

struct NodD {
	Carte book;
	NodD *next, *prev;
};
struct ListaDubla {
	NodD *p, *u;
};

ListaDubla interschimbAdiacente(NodD *t, ListaDubla l) {
	NodD *q, *r, *s;
	if (t == l.p) {
		// t este primul nod in lista dubla
		r = t->next;
		s = r->next;

		t->next = s;
		t->prev = r;
		r->next = t;
		r->prev = NULL; // r devine primul nod

		if (s) {
			// lista contine cel putin 3 noduri
			s->prev = t;
		}
		else {
			// s = NULL; lista are doar 2 noduri care se interschimba
			l.u = t;
		}

		l.p = r;
	}
	else {
		// t in interiorul listei duble

		q = t->prev;
		r = t->next;
		s = r->next; 

		if (t->next == l.u) {
			// t este penultimul nod
			l.u = t;
		}
		else {
			// exista s nod
			s->prev = t;
		}

		t->next = s;
		t->prev = r;
		r->next = t;
		r->prev = q;
		q->next = r;

		
	}

	return l;
}

// implementare BubbleSort pe lista dubla de carti
ListaDubla sortBubble(ListaDubla l) {
	if (!l.p || !l.p->next) // lista nu are cel putin 2 noduri
		return l;

	char vb; // boolean pentru interschimb de noduri adiacente

	do {
		vb = 0;
		NodD *t = l.p;
		while (t && t->next) { // parsare lista de la primul la ultimul nod
			if (t->book.id > t->next->book.id) { // sortare crescatoare dupa id carte
				l = interschimbAdiacente(t, l);
				vb = 1;
			}
			else {
				t = t->next;
			}
		}

	} while (vb);

	return l;
}

// inserare nod in lista dubla
ListaDubla inserareListaDubla(ListaDubla lstD, Carte c) {
	NodD * nou;
	nou = (NodD*)malloc(sizeof(NodD));
	nou->book = c;
	nou->next = NULL; // initializare succesor pentru nou
	nou->prev = lstD.u; // initializare predecesor pentru nou

	if (!lstD.p) // lista este empty; nou devine primul si unicul nod in lista; primul si ultimul nod sunt aceiasi (nou)
		lstD.p = lstD.u = nou;
	else {
		lstD.u->next = nou; // actualizez succesorul pentru sfarsitul curent al listei
		lstD.u = nou; // actualizez nou sfarsit de lista
	}

	return lstD;
}

void parseListDbl(ListaDubla lstD, char directie) {
	NodD *tmp;
	if (directie) { // primula -> ultimul
		tmp = lstD.p;
		while (tmp) {
			printf("%d %s\n", tmp->book.id, tmp->book.titlu);

			tmp = tmp->next;
		}
	}
	else { // ultimul nod -> primul nod
		tmp = lstD.u;
		while (tmp) {
			printf("%d %s\n", tmp->book.id, tmp->book.titlu);

			tmp = tmp->prev;
		}
	}
}



void main() {
	ListaDubla listD;
	listD.p = listD.u = NULL;

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

		// inserare carte in lista dubla
		listD = inserareListaDubla(listD, temp);

		fgets(buffer, sizeof(buffer), f); // citeire id carte urmatoare in fisier
	}

	parseListDbl(listD, 1);

	// sortare lista dubla
	listD = sortBubble(listD);

	printf("\n\nLista dupa sortare crescatoare Bubble:\n");
	parseListDbl(listD, 1);
	printf("\n\nLista dupa sortare Bubble (ordine inversa noduri):\n");
	parseListDbl(listD, 0);

	fclose(f);

}
