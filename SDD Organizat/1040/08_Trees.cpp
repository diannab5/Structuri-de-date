#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define NR_DESC_NOD 10

struct Carte {
	int id;
	char nrAutori;
	char ** autori;
	char * titlu;
};

struct NodTree {
	int key;
	// Carte book;
	NodTree* desc[NR_DESC_NOD];
};

NodTree* cautaNodParinte(NodTree* r, int idp) {
	if (r) {
		// traversare in preordine a arborelui
		if (r->key == idp)
			return r;

		for (int i = 0; i < NR_DESC_NOD; i++)
			return cautaNodParinte(r->desc[i], idp);

	}
	return r;
}

NodTree* inserareNodOriunde(NodTree* r, int cheie, char &flagInserare) {
	if (r) {
		// traversare in preordine a arborelui
		int i;
		for (i = 0; i < NR_DESC_NOD; i++) {
			if (r->desc[i] == NULL) {
				// am gasit pozitie de inserat
				NodTree* nou = (NodTree*)malloc(sizeof(NodTree));
				nou->key = cheie;
				for (int i = 0; i < NR_DESC_NOD; i++) {
					nou->desc[i] = NULL;
				}

				r->desc[i] = nou;
				i = NR_DESC_NOD + 1;
				flagInserare = 1;
			}
		}

		if (i == NR_DESC_NOD) {
			for (i = 0; i < NR_DESC_NOD; i++) {
				if(flagInserare == 0)
					r->desc[i] = inserareNodOriunde(r->desc[i], cheie, flagInserare);
			}
		}

	}

	return r;
}

NodTree* inserareNodTree(NodTree* r, int cheie, int idp) { // idp - cheie nod parinte
	if (!r) {
		// arbore empty
		r = (NodTree*)malloc(sizeof(NodTree));
		r->key = cheie;
		for (int i = 0; i < NR_DESC_NOD; i++) {
			r->desc[i] = NULL;
		}
	}
	else {
		char flagInserare;
		// exista cel putin 1 nod in arbore
		NodTree* parinte = NULL;
		if (r->key == idp) {
			parinte = r;
		}
		else {
			parinte = cautaNodParinte(r, idp);
		}

		if (parinte != NULL) {
			// am gasit parinte lui cheie
			// verific daca exista loc in parinte
			int i;
			for (i = 0; i < NR_DESC_NOD; i++) {
				if (parinte->desc[i] == NULL) {
					// am identificat locul de inserat in parinte
					NodTree* nou = (NodTree*)malloc(sizeof(NodTree));
					nou->key = cheie;
					for (int i = 0; i < NR_DESC_NOD; i++) {
						nou->desc[i] = NULL;
					}

					parinte->desc[i] = nou;
					// fortez iesire din do-for
					i = NR_DESC_NOD + 1;
				}
			}

			// verific daca inserarea a esuat (parintele nu are suficient spatiu)
			if (i == NR_DESC_NOD) {
				flagInserare = 0;
				parinte = inserareNodOriunde(parinte, cheie, flagInserare);
			}
		}
		else {
			// nu s-a identificat nici un nod cu idp
			flagInserare = 0;
			r = inserareNodOriunde(r, cheie, flagInserare);
		}
	}

	return r;
}

NodTree* copiazaSubarbore(NodTree * newr, NodTree* r, int key) {
	if (r) {
		newr = inserareNodTree(newr, key, 0);

		for (int i = 0; i < NR_DESC_NOD; i++) {
			newr = copiazaSubarbore(newr, r->desc[i], r->desc[i]->key);
		}
	}

	return newr;
}

void dezalocareSubarbore(NodTree* r) {
	if (r) {
		for (int i = 0; i < NR_DESC_NOD; i++) {
			dezalocareSubarbore(r->desc[i]);
		}

		// dezalocare nod curent 
		free(r);
	}
}

char cautaNodSters(NodTree* r, int delKey, NodTree* &parinte, NodTree* &gasit) {
	if (r) {
		if (r->key == delKey) {
			gasit = r;
			return 1;
		}

		for (int i = 0; i < NR_DESC_NOD; i++) {
			char x = cautaNodSters(r->desc[i], delKey, parinte, gasit);
			if (x != 0) {
				// fortez iesirea din DO-FOR
				i = NR_DESC_NOD;
				parinte = r;
			}
		}
	}
	return 0;
}

NodTree* stergereNodArbore(NodTree* r, int delKey) {
	if (r->key == delKey) {
		// cheia de sters se afla in nodul radacina

		// copiaza subarborii radacinii in alt arbore 
		NodTree* newRoot = NULL;
		for (int i = 0; i < NR_DESC_NOD; i++) {
			newRoot = copiazaSubarbore(newRoot, r->desc[i], r->desc[i]->key);
		}		

		// dezalocare subarbori ai radacinii; cheile sunt mutate in noul arbore newRoot
		for (int i = 0; i < NR_DESC_NOD; i++) {
			dezalocareSubarbore(r->desc[i]);
		}
		

		//dezalocare nod radacina
		free(r);

		r = newRoot;
	}
	else {
		// nodul de sters nu este radacina de arbore
		NodTree* parinte = NULL, *nodSters = NULL;
		char x = 0;
		parinte = r;
		for (int i = 0; i < NR_DESC_NOD; i++) {
			x = cautaNodSters(r->desc[i], delKey, parinte, nodSters);
			if (x) {
				// am gasit nodul de sters si parintele sau
				// fortez iesirea din structura DO-FOR
				i = NR_DESC_NOD;
			}
		}

		// nodSters == NULL, nu exista nod cu cheia delKey in arbore
		if (nodSters != NULL) {
			// punem pointerul spre nodSters pe NULL in nodul parinte
			for (int i = 0; i < NR_DESC_NOD; i++) {
				if (parinte->desc[i] && parinte->desc[i]->key == delKey) {
					parinte->desc[i] = NULL;  // disponibilizare pozitie pentru reinserare noduri in arbore
				}
			}

			// copiere subarbori ai nodului de sters in acelasi arbore r
			for (int i = 0; i < NR_DESC_NOD; i++) {
				if (nodSters->desc[i]) {
					r = copiazaSubarbore(r, nodSters->desc[i], nodSters->desc[i]->key);
				}
			}

			// dezalocare subarbori ai nodului de sters; cheile sunt mutate in noul arbore newRoot
			for (int i = 0; i < NR_DESC_NOD; i++) {
				dezalocareSubarbore(nodSters->desc[i]);
			}

			//dezalocare nod de sters
			free(nodSters);
		}
	}

	return r;
}


void main() {
	NodTree* root = NULL;

	root = inserareNodTree(root, 13, 9);
	root = inserareNodTree(root, 6, 13);
	root = inserareNodTree(root, 7, 13);

	root = inserareNodTree(root, 25, 6);
	root = inserareNodTree(root, 31, 25);

	// parintele nu exista
	root = inserareNodTree(root, 66, 4);

	// exista 3 noduri descendente pt radacina 13
	for(int i = 1; i<8; i++)
		root = inserareNodTree(root, i + 10, 13);

	// inserare nod cu parinte incarcat la maxim
	root = inserareNodTree(root, 101, 13);

	// inserari suplimentare pentru nodul de sters (11)
	root = inserareNodTree(root, 102, 11);
	root = inserareNodTree(root, 103, 11);

	// stergere nod (cheie) in arbore
	root = stergereNodArbore(root, 11);
}
