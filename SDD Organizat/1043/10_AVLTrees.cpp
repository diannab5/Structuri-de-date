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
	// calculGENod(fiuSt); // redundant cu calcul in apelul functiei de rotire simpla

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
	// calculGENod(fiuDr); // redundant cu calcul in apelul functiei de rotire simpla

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


// traversare AVL inordine
void InordineAVL(NodAVL *rAVL) {
	if (rAVL) {
		InordineAVL(rAVL->st);
		printf("\n %d %d ", rAVL->stud.id, rAVL->GE);
		InordineAVL(rAVL->dr);
	}
}


NodAVL * stergNodAVL(NodAVL *r, int delKey, int &er) {
	if (!r) {
		er = 1;
	}
	else {
		// cautare nod de sters
		if (r->stud.id > delKey)
			r->st = stergNodAVL(r->st, delKey, er);
		else
			if (r->stud.id < delKey) {
				r->dr = stergNodAVL(r->dr, delKey, er);
			}
			else {
				// am identificat nodul de sters (r)
				if (r->st == NULL && r->dr == NULL) {
					// nodul este frunza (cazul 1)
					free(r->stud.nume);
					free(r);

					r = NULL;
				}
				else {
					if (r->st != NULL && r->dr != NULL) {
						// nodul de sters are 2 descendenti (cazul 3)
						free(r->stud.nume);

						NodAVL* desc = r->st;
						while (desc->dr)
							desc = desc->dr; 
						
						// desc este cel mai din dreapta nod din subarborele stanga
						r->stud.id = desc->stud.id;
						r->stud.medie = desc->stud.medie;
						r->stud.nume = (char*)malloc(strlen(desc->stud.nume) + 1);
						strcpy(r->stud.nume, desc->stud.nume);

						r->st = stergNodAVL(r->st, r->stud.id, er);						
					}
					else {
						// nodul are 1 descendent (cazul 2)
						NodAVL* desc;
						if (r->st != NULL)
							desc = r->st;
						else
							desc = r->dr;

						free(r->stud.nume);
						free(r);

						r = desc;
					}
				}
			}

	}

	// reechilibrare subarbore r
	calculGENod(r);
	if (r) {
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

	return r; // de avut in vedere ca r este adresa intoarsa in apelul recursiv anterior
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

	// stergere nod in AVL
	err = 0;
	/*root = stergNodAVL(root, 9, err);
	root = stergNodAVL(root, 127, err);*/
	root = stergNodAVL(root, 43, err);
	printf("\nArbore AVL dupa stergere nod:\n");
	InordineAVL(root);
	printf("\n");

	fclose(f);
}

// creare lista simpla cu noduri avand 2 descendenti

// creare lista simpla cu noduri la traversarea in inordine a arborelui

// dezalocare AVL

// determinare numar noduri plasate pe un nivel specificat
