#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define NR_DESC_ARBORE 10

struct Carte {
	int id;
	char nrAutori;
	char** Autori;
	char * titlu;
};

struct NodArbore {
	// Carte book;
	int key;
	NodArbore* desc[NR_DESC_ARBORE];
};

struct NodCoada {
	NodArbore * pNod;
	NodCoada * next;
};

NodCoada * put(NodCoada * q, NodArbore * p) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->pNod = p;
	nou->next = NULL;
	if (q) {
		// exista cel putin 1 nod in coada
		NodCoada * tmp = q;
		while (tmp->next)
			tmp = tmp->next;

		tmp->next = nou;
	}
	else {
		// coada este empty
		q = nou;
	}

	return q;
}

NodCoada* get(NodCoada* q, NodArbore *& pSave) {
	pSave = NULL;
	if (q) {
		NodCoada* tmp = q;
		pSave = tmp->pNod;

		q = q->next; // actualizez inceputul cozii

		// dezalocare nod extras
		free(tmp);
	}

	return q;
}

NodArbore* determinaNodParinte(NodArbore *r, NodCoada *&q) {
	NodArbore* nodParinte = NULL;
	while (q) {
		NodArbore* tmp;
		// extrage nod din coada
		q = get(q, tmp);

		// verifica daca nodul extras are cel putin 1 descendent NULL
		// daca DA, inseamna ca nodul extras este parinte pt nodul pe care vreau sa-l inserez
		// daca NU, inseamna ca nu am gasit nod parinte -> toti descendentii lui tmp vor ajunge in coada
		for(int i=0; i<NR_DESC_ARBORE; i++)
			if (tmp->desc[i] == NULL) {
				// am gasit loc de inserat; tmp este parinte pentru nodul de inserat
				return tmp;
			}

		// nu am gsit loc in tmp; toti cei 10 descendenti se vor insera in coada
		for (int i = 0; i < NR_DESC_ARBORE; i++) {
			q = put(q, tmp->desc[i]);
		}

	}

	return nodParinte;
}

NodArbore* inserareNodArbore(NodArbore* r, int cheie) {
	NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
	nou->key = cheie;
	for (int i = 0; i < NR_DESC_ARBORE; i++)
		nou->desc[i] = NULL; // nodul inserat nu are descendenti

	if (!r) {
		// arborele este empty	
		r = nou;
	}
	else {
		// arborele contine cel putin 1 nod
		NodCoada* coada = NULL;
		coada = put(coada, r);
		NodArbore* parinte = determinaNodParinte(r, coada);
		for (int i = 0; i < NR_DESC_ARBORE; i++) {
			if (parinte->desc[i] == NULL) {
				// am gasit locul de inserat
				parinte->desc[i] = nou;
				i = NR_DESC_ARBORE; // fortez iesire din structura DO-FOR
			}
		}

		// dezalocare coada (pt elementele ramase din apelul determinaNodParinte)
		NodArbore *tmp;
		while (coada) {
			coada = get(coada, tmp);
		}
	}

	return r;
}

void parsarePreordine(NodArbore* r) {
	if (r) {
		printf(" %d ", r->key); // prelucrarea nod curent

		for (int i = 0; i < NR_DESC_ARBORE; i++)
			parsarePreordine(r->desc[i]);
	}
}

// stergere nod din arbore oarecare (max NR_DESC_ARBORE descendenti pt fiecare nod)

void main() {
	NodArbore * root = NULL;
	for (int cheie = 1; cheie < 77; cheie++) {
		root = inserareNodArbore(root, cheie);
	}

	parsarePreordine(root);

}