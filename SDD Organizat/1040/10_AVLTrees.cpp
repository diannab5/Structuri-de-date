#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define LINESIZE 128

#include <stdio.h>
#include <malloc.h>

// Definire structuri 

struct Student {
	int id;
	char *nume;
	float medie;
};

struct NodAVL {
	Student stud;
	int GE;
	NodAVL *st, *dr;
};


// maximul a doi intregi
int max(int a, int b) {
	return a>b ? a : b;
}


// inaltime (sub)arbore r
int h(NodAVL *r) {
	if (r)
		return 1 + max(h(r->st), h(r->dr));
	else
		return 0;
}


// gradul de echilibru al unui nod
void calculGENod(NodAVL *r) {
	if (r) {
		//prelucrare (calcul GE)
		r->GE = h(r->dr) - h(r->st);
	}
}


// rotire simpla la dreapta
NodAVL* rotireSimplaDr(NodAVL* pivot, NodAVL* fiuSt) {
	pivot->st = fiuSt->dr;
	calculGENod(pivot);
	fiuSt->dr = pivot;
	calculGENod(fiuSt);

	return fiuSt;
}

// rotire simpla la stanga
NodAVL* rotireSimplaSt(NodAVL* pivot, NodAVL* fiuDr) {
	pivot->dr = fiuDr->st;
	calculGENod(pivot);
	fiuDr->st = pivot;
	calculGENod(fiuDr);

	return fiuDr;
}

// rotire dubla stanga-dreapta
NodAVL* rotireDblStDr(NodAVL* pivot, NodAVL* fiuSt) {
	//aducerea dezechilibrului pe aceeasi directie
	pivot->st = rotireSimplaSt(fiuSt, fiuSt->dr);
	calculGENod(pivot);
	fiuSt = pivot->st; // fiusSt nu mai este cel din lista de param; s-a modificat prin rotirea de aducere a dezzechilibrului pe aceeasi directie (linia 67)
	//rotire propriu-zisa in pivot
	fiuSt = rotireSimplaDr(pivot, fiuSt);
	// calculGENod(fiuSt);

	return fiuSt;
}

// rotire dubla dreapta-stanga
NodAVL* rotireDblDrSt(NodAVL* pivot, NodAVL* fiuDr) {
	// aducerea dezechilibrului pe aceeasi directie
	pivot->dr = rotireSimplaDr(fiuDr, fiuDr->st);
	calculGENod(pivot);
	fiuDr = pivot->dr;
	// rotire propriu-zisa in pivot
	fiuDr = rotireSimplaSt(pivot, fiuDr);
	// calculGENod(fiuDr);

	return fiuDr;
}

// inserare nod in AVL
NodAVL* insNodAVL(NodAVL *r, Student s, int *err) {
	if (r) {
		if (r->stud.id > s.id)
			r->st = insNodAVL(r->st, s, err);
		else
			if (r->stud.id < s.id)
				r->dr = insNodAVL(r->dr, s, err);
			else
				*err = 1;
	}
	else {
		NodAVL * nou;
		nou = (NodAVL*)malloc(sizeof(NodAVL));
		nou->stud = s;
		nou->st = NULL;
		nou->dr = NULL;
		r = nou;
	}

	// recalculez grad de echilibru pt nodul curent
	calculGENod(r);
	if (r->GE == 2) {
		if (r->dr->GE == -1) {
			//dezechilibru dreapta-stanga
			r = rotireDblDrSt(r, r->dr);
		}
		else {
			//dezechilibru dreapta
			r = rotireSimplaSt(r, r->dr);
		}
	}
	else {
		if (r->GE == -2) {
			if (r->st->GE == 1) {
				//dezechilibru combinat stanga-dreapta
				r = rotireDblStDr(r, r->st);
			}
			else {
				//dezechilibru stanga
				r = rotireSimplaDr(r, r->st);
			}
		}
	}

	return r;
}

NodAVL *stergereNodAVL(NodAVL *r, int key, int &er) {
	if (r) {
		if (r->stud.id > key)
			r->st = stergereNodAVL(r->st, key, er);
		else
			if (r->stud.id < key) {
				r->dr = stergereNodAVL(r->dr, key, er);
			}
			else {
				// am gasit nodul de sters
				er = 1;

				if (r->st == NULL) {
					NodAVL* tmp = r->dr; // poate fi NULL sau nu
										 // dezaloc nodul curent r
					free(r->stud.nume);
					free(r);

					r = tmp;
				}
				else {
					if (r->dr == NULL) {
						// exista un sg descendent pe stanga nodului curent r
						NodAVL* tmp = r->st;

						// dezalocare nod curent r
						free(r->stud.nume);
						free(r);
						r = tmp;
					}
					else {
						// cazul cu 2 descendenti
						NodAVL * tmp = r->st;
						while (tmp->dr)
							tmp = tmp->dr; // cautarea celui mai din dreapta nod din subarborele stanga al lui r

										   // tmp este nodul cu cheia maxima din subarborele stanga
										   // dezalocare nume student pentru nodul de sters
						free(r->stud.nume);
						// mutarea datelor din tmp in r
						r->stud.id = tmp->stud.id;
						r->stud.medie = tmp->stud.medie;
						r->stud.nume = (char*)malloc(strlen(tmp->stud.nume) + 1);
						strcpy(r->stud.nume, tmp->stud.nume);

						r->st = stergereNodAVL(r->st, tmp->stud.id, er);

					}
				}
			}
	}

	if (r) {
		// recalculez grad de echilibru pt nodul curent
		calculGENod(r);
		if (r->GE == 2) {
			if (r->dr->GE == -1) {
				//dezechilibru dreapta-stanga
				r = rotireDblDrSt(r, r->dr);
			}
			else {
				//dezechilibru dreapta
				r = rotireSimplaSt(r, r->dr);
			}
		}
		else {
			if (r->GE == -2) {
				if (r->st->GE == 1) {
					//dezechilibru combinat stanga-dreapta
					r = rotireDblStDr(r, r->st);
				}
				else {
					//dezechilibru stanga
					r = rotireSimplaDr(r, r->st);
				}
			}
		}
	}

	return r;
}


// traversare AVL inordine
void InordineAVL(NodAVL *rAVL) {
	if (rAVL) {
		InordineAVL(rAVL->st);
		printf("\n %d %d ", rAVL->stud.id, rAVL->GE);
		InordineAVL(rAVL->dr);
	}
}

void main() {

	Student stud;
	NodAVL *root = NULL;
	int err;

	FILE * f;
	f = fopen("Studenti.txt", "r");	

	char * token, file_buf[LINESIZE], sep_list[] = ",";

	while (fgets(file_buf, sizeof(file_buf), f)) {
		token = strtok(file_buf, sep_list);
		stud.id = atoi(token);

		token = strtok(NULL, sep_list);
		stud.nume = (char*)malloc((strlen(token) + 1) * sizeof(char));
		strcpy(stud.nume, token);

		token = strtok(NULL, sep_list);
		stud.medie = atof(token);

		token = strtok(NULL, sep_list);
		if (token)
			printf("\nEroare preluare token!");

		// inserare student in ABC
		err = 0;
		// float test = 0; // test cu parametrul 3 ca referinta
		root = insNodAVL(root, stud, &err);
		if (err) {
			printf("\nStudentul cu id %d exista in arbore.\n", stud.id);
			free(stud.nume);
		}
		else
			printf("\nStudentul %s a fost inserat\n", stud.nume);
	}

	// traversare in inordine (ordine crescatoare id-uri studenti)
	printf("\nTraversare arbore inordine arbore AVL:\n\n");
	InordineAVL(root);
	printf("\n");

	// stergere nod arbore AVL
	err = 0;
	root = stergereNodAVL(root, 33, err);
	if (!err)
		printf("\nArborele nu contine date cu cheia 33.\n");
	printf("\nTraversare arbore inordine arbore AVL:\n");
	InordineAVL(root); // arborele este neschimbat
	printf("\n");

	// stergere nod frunza
	root = stergereNodAVL(root, 13, err);
	if (!err)
		printf("\nArborele nu contine date cu cheia 13.\n");
	printf("\nTraversare arbore inordine arbore AVL:\n");
	InordineAVL(root); // arborele are GE diferit pt 24 -> radacina (??)
	printf("\n");

	// stergere nod cu 1 descendent
	root = stergereNodAVL(root, 24, err);
	if (!err)
		printf("\nArborele nu contine date cu cheia 24.\n");
	printf("\nTraversare arbore inordine arbore AVL:\n");
	InordineAVL(root); // recalculare GE de la 24 -> radacina
	printf("\n");

	// stergere nod cu 1 descendent
	root = stergereNodAVL(root, 343, err);
	if (!err)
		printf("\nArborele nu contine date cu cheia 343.\n");
	printf("\nTraversare arbore inordine arbore AVL:\n");
	InordineAVL(root); // recalculare GE de la 122 -> radacina
	printf("\n");

	fclose(f);
}
