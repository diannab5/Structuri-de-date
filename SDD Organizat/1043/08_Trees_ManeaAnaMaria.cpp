#include <string.h>
#include <stdio.h>
#include <malloc.h>

struct NodTree {
	int key;
	NodTree *fiu, *frate;
};

void cautaNod(NodTree* r, int id, NodTree* &gasit) {
	if (r) {
		if (r->key == id) { // prelucrez nodul curent r
			gasit = r;
			return;
		}
		cautaNod(r->fiu, id, gasit); // parsez primul subarbore descendent lui r
		if (gasit)
			return;
		if (r->fiu) {
			NodTree *tmp = r->fiu;
			while (tmp->frate) { // parsez restul de subarbori descendenti lui r 
				cautaNod(tmp->frate, id, gasit);
				if (gasit)
					return;
				tmp = tmp->frate;
			}
		}
	}
}


void preordine(NodTree* r) {
	if (r) {
		printf("%d  ", r->key);

		preordine(r->fiu); // parsez primul subarbore descendent lui r	

		if (r->fiu) {
			NodTree *tmp = r->fiu;
			while (tmp->frate) { // parsez restul de subarbori descendenti lui r 
				preordine(tmp->frate);
				tmp = tmp->frate;
			}
		}
	}
}


NodTree *inserare(NodTree *r, int cheie, int idParinte) {

	if (!idParinte) {
		NodTree* nou = (NodTree*)malloc(sizeof(NodTree)); // nodul care se insereaza devine nod radacina
		nou->key = cheie;
		nou->fiu = NULL;
		nou->frate = NULL;

		if (r) {
			nou->fiu = r;
		}

		r = nou;
	}
	else {
		NodTree* nou = (NodTree*)malloc(sizeof(NodTree));
		nou->key = cheie;
		nou->fiu = NULL;
		nou->frate = NULL;

		NodTree* p = 0;
		cautaNod(r, idParinte, p);
		if (!p->fiu)
			p->fiu = nou;
		else {

			if (!p->fiu->frate)
				p->fiu->frate = nou;
			else {
				NodTree* tmp = p->fiu;
				while (tmp->frate)
					tmp = tmp->frate;

				tmp->frate = nou;
			}
		}
	}

	return r;
}

void cautaParinteNodSters(NodTree* r, int cheie, NodTree* &parinte) {
	if (r) {
		if (r->fiu) {
			if (r->fiu->key == cheie) {
				parinte = r;
				return;
			}
		}

		if (r->fiu->frate) {
			NodTree* parse = r->fiu;
			while (parse->frate) {
				if (parse->frate->key == cheie) {
					parinte = parse;
					return;
				}

				parse = parse->frate;
			}
		}

		// continuare traversare structura arborescenta
		cautaParinteNodSters(r->fiu, cheie, parinte);
		NodTree* t = r->fiu->frate;
		while (t) {
			cautaParinteNodSters(t, cheie, parinte);
			t = t->frate;
		}
	}
}

NodTree* stergereNodArbore(NodTree* r, int cheie) {
	if (r->key == cheie) {
		//nodul de sters este radacina
		NodTree* tmp = r;
		if (tmp->fiu == NULL)
		{
			r = NULL;
			return r;
		}
		r = tmp->fiu;

		//actualizez legaturi intre noua radacina si fostii frati ai acesteia
		if (r->fiu) {
			NodTree* parse = r->fiu;
			while (parse->frate) {
				parse = parse->frate;
			}
			//parse este acum ultimul nod din lista de descendenti ai nodii radacini
			parse->frate = r->frate; // am legat fratii lui r de fii lui
			r->frate = NULL; //noua radacina nu are frati!
		}
		else {
			if (r->frate) {
				r->fiu = r->frate;
				r->frate = NULL;
			}
		}

		//dezalocare nod
		free(tmp);
	}
	else {
		//cauta parintele nodului de sters
		NodTree* parinte = NULL;
		cautaParinteNodSters(r, cheie, parinte);
		if (parinte) {
			//am identificat nod de sters daca parinte != NULL
			if (parinte->fiu &&parinte->fiu->key == cheie) {
				//nodul de sters este primul in lista de descendenti ai lui parinte
				NodTree*  tmp = parinte->fiu;
				if (tmp->fiu == NULL)
					parinte->fiu = tmp->frate;
				else
				{
					parinte->fiu = tmp->fiu;

					if (parinte->fiu) {
						NodTree* parse = parinte->fiu; //tmp->fiu;

						while (parse->frate) {
							parse = parse->frate;
						}
						parse->frate = tmp->frate;//aici
												  //leg descendentii nodului de sters la lista de frati ai acestuia
					}
				}

				//dezalocare
				free(tmp);
			}
			else {
				//if (parinte->frate && parinte->frate->key == cheie) {
				NodTree* tmp = parinte->frate;
				parinte->frate = tmp->frate;

				NodTree* parse = parinte->frate;
				if (parse) {
					while (parse->frate) {
						parse = parse->frate;
					}
					parse->frate = tmp->fiu;
					//leg descendentii nodului de sters la lista de frati ai nodului de sters

				}
				else {
					//nodul de sters este ultimul din lista de frati 
					parinte->frate = tmp->fiu;
				}

				//dezalocare
				free(tmp);
			}
		}
	}
	return r;
}

// functie dezalocare arbore
void dezalocareArbore(NodTree* &r) {
	while (r) {
		NodTree* tmp = r;
		r = stergereNodArbore(tmp, tmp->key);
	}

}

// functie determinare numar de noduri frunza
int nrFrunze(NodTree* r) {
	int nr = 0;
	if (r) {
		nr += nrFrunze(r->fiu);

		if (r->fiu) {
			NodTree* tmp = r->fiu;
			while (tmp->frate) {
				nr += nrFrunze(tmp->frate);
				tmp = tmp->frate;
			}
		}
		else {
			nr++;
		}
	}
	return nr;
}


// functie determinare inaltime arbore
int  inaltimeArbore(NodTree* r) {
	int h = 0;
	int y = 0;
	if (r) {		

		if (r->fiu) {
			h = 1 + inaltimeArbore(r->fiu);			
		}		
			
		NodTree* tmp = r->frate;
		while (tmp) {
			if (!tmp->fiu && !tmp->frate) y = inaltimeArbore(tmp);
			else y = 1 + inaltimeArbore(tmp);
			if (h < y)
				h = y;
			tmp = tmp->frate;
		}
	}

	return h;
}

void main() {
	NodTree *root = NULL;

	// inserare cheie in arbore
	root = inserare(root, 1, 0);
	root = inserare(root, 2, 1);
	root = inserare(root, 3, 1);
	root = inserare(root, 4, 1);
	root = inserare(root, 5, 2);
	root = inserare(root, 6, 2);
	root = inserare(root, 7, 6);
	root = inserare(root, 8, 7);

	//printf("\nArborele in traversare in preordine:\n\n");
	//preordine(root);
	//printf("\n\n");

	root = inserare(root, 9, 4);
	printf("\nArborele in traversare in preordine:\n");
	preordine(root);
	printf("\n\n");

	root = stergereNodArbore(root, 4);
	printf("\nArborele dupa stergere:\n");
	preordine(root);
	printf("\n");

	int frunze = nrFrunze(root);
	printf("\n\nArborele are %d frunze.\n\n", frunze);

	int inaltime = inaltimeArbore(root);
	printf("Arborele are inaltimea: %d \n", inaltime);

	root = inserare(root, 4, 9);
	root = inserare(root, 10, 7);
	root = inserare(root, 11, 10);	
	inaltime = inaltimeArbore(root);
	printf("Arborele are inaltimea: %d \n", inaltime);

	dezalocareArbore(root);
	if (root == NULL) {
		printf("Arborele a fost dezalocat.\n");
	}
}