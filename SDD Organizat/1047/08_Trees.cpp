#include <stdio.h>
#include <malloc.h>

struct NodTree {
	int key;
	NodTree *fiu, *frate;
};

void cautaNod(NodTree* r, int id, NodTree* &gasit) {
	if (r) {
		printf("Nod curent este: %d\n", r->key);
		if (r->key == id) { // prelucrez nodul curent r
			gasit = r;
			return;
		}
		cautaNod(r->fiu, id, gasit); // parsez primul subarbore descendent lui r
		if (gasit)
			return;
		if (r->fiu != NULL) { // este posibil ca nodul curent r sa fie frunza
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
			nou->fiu = r; // noi devine radacina pentru arborele existent
		}

		r = nou;
	}
	else {
		NodTree* nou = (NodTree*)malloc(sizeof(NodTree));
		nou->key = cheie;
		nou->fiu = NULL;
		nou->frate = NULL;

		NodTree* p = NULL;
		cautaNod(r, idParinte, p); // cauta nodul cu idParinte; nodul identificat este p
		if (p) { // s-a identificat nod cu idParinte in arbore
			if (!p->fiu) // nodul parinte identificat nu are descendent
				p->fiu = nou;
			else { // nodul identificat p are cel putin 1 descendent

				if (!p->fiu->frate) // primul fiu descendent al lui p nu are frate
					p->fiu->frate = nou;
				else {
					NodTree* tmp = p->fiu;
					while (tmp->frate) // parsare lista de frati pentru primul descendent al lui p
						tmp = tmp->frate;

					tmp->frate = nou;
				}
			}
		}
	}

	return r; // radacina / noua radacina de arbore oarecare
}

void cautaNodStergere(NodTree* r, int id, NodTree* &gasit, NodTree *& anterior) {
	if (r) {
		if (r->key == id) { // prelucrez nodul curent r
			gasit = r;
			return;
		}
		anterior = NULL;
		cautaNodStergere(r->fiu, id, gasit, anterior); 
		if (gasit)
			return;
		if (r->fiu != NULL) { // este posibil ca nodul curent r sa fie frunza
			NodTree *tmp = r->fiu;
			while (tmp->frate) { // parsez restul de subarbori descendenti lui r 
				anterior = tmp;
				cautaNodStergere(tmp->frate, id, gasit, anterior);
				if (gasit)
					return;
				tmp = tmp->frate;
			}
		}
	}
}

char cautaNodParinte(NodTree* r, int idSters, NodTree* &parinte) {
	if (r) {
		if (r->key == idSters) { // prelucrez nodul curent r
			return 1;
		}
		parinte = r;
		char gasit = cautaNodParinte(r->fiu, idSters, parinte);
		if (gasit)
			return gasit;
		if (r->fiu != NULL) { // este posibil ca nodul curent r sa fie frunza
			NodTree *tmp = r->fiu;
			while (tmp->frate) { // parsez restul de subarbori descendenti lui r 
				gasit = cautaNodParinte(tmp->frate, idSters, parinte);
				if (gasit)
					return gasit;
				tmp = tmp->frate;
			}
		}
	}

	return 0;
}

NodTree* stergere(NodTree *r, int cheie) {
	if (r->key == cheie) {
		// nodul de sters este radacina de arbore
		if (r->fiu) {
			// exista cel putin un descendent al radacinii
			NodTree * tmp = r;
			// actualizez radacina de arbore
			r = r->fiu;
			// dezalocare nod radacina veche
			free(tmp);

			// actualizari legaturi intre noduri
			// salvez lista de frati ai noii radacini
			tmp = r->frate;
			// actualizez liusta de frati pt noua radacina
			r->frate = NULL;
			// leg lista de frati ai noii radacini de lista de descendenti ai noii radacini
			NodTree * parse = r->fiu; 
			while (parse->frate) {
				parse = parse->frate;
			}
			parse->frate = tmp;
		}
	}
	else {
		// caut nodul de sters !!! de fapt am nevoie de nodul frate din fata nodului de sters
		NodTree* p = NULL; // p este nodul de sters
		NodTree* q = NULL; // q este nodul anterior lui p
		cautaNodStergere(r, cheie, p, q); // functie apelata recursiv
		
		// daca q (anterior) este NULL, atunci p este primul frate
		if (!q) {
			if (p) { // nodul de sters a fost gasit in arbore
				// caut parintele lui gasit
				NodTree * parinte = NULL;
				char gasit = cautaNodParinte(r, p->key, parinte);

				// izolare nod de sters
				NodTree * tmp = p;
				NodTree *listFrati = p->frate;
				NodTree *pFiu = p->fiu;
				// dezalocare tmp
				free(tmp);

				// actualizare legatura parinte la descentii lui p
				if (pFiu) {
					parinte->fiu = pFiu;
					tmp = pFiu;
					if (tmp != NULL) {
						while (tmp->frate) {
							tmp = tmp->frate;
						}
						// tmp este adresa ultimul nod din lista de frati ai lui pFiu
						tmp->frate = listFrati;
					}
				}
				else {
					// nodul p nu are descendenti
					parinte->fiu = listFrati;
				}
			}
		}
		else {
			// nodul de sters nu este primul frate
			// 2 sub-cazuri: nodul are/nu are descendenti
			if (p->fiu == NULL) {
				// nodul frate de sters nu are descendenti
				NodTree *tmp = p->frate;
				// dezalocare nod
				free(p);
				// actualizare legatura in lista de frati
				q->frate = tmp;
			}
			else {
				// nodul de sters are descendenti
				NodTree * tmp = p->frate;
				NodTree *pFiu = p->fiu;

				//dezalocare p
				free(p);
				// refac legatura in lista de frati
				q->frate = tmp;
				// parsez lista de frati
				while (q->frate) {
					q = q->frate;
				}
				q->frate = pFiu;
			}
		}
	}

	return r;
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

	printf("\nArborele in traversare in preordine:\n\n");
	preordine(root);
	printf("\n\n");

	root = inserare(root, 9, 4);
	root = inserare(root, 10, 1);
	root = inserare(root, 11, 1);
	printf("\nArborele in traversare in preordine:\n\n");
	preordine(root);
	printf("\n");

	// stergere nod in arbore oarecare
	root = stergere(root, 4);
	printf("\nArborele dupa stergere:\n\n");
	preordine(root);
}