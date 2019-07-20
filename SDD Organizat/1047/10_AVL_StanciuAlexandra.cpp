
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

// Definire structuri 

struct Student {
	int id;
	char *nume;
	float medie;
};

struct NodAVL {
	Student stud;
	int GE; // indicator grad de echilibru
	NodAVL *st, *dr;
};


// maximul a doi intregi
int max(int a, int b) {
	return a > b ? a : b;
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


// rotire simpla la dreapta (-2, -1/0)
NodAVL* rotireSimplaDr(NodAVL* pivot, NodAVL* fiuSt) {
	pivot->st = fiuSt->dr; // actualizare adresa descendent dreapta pivot
	calculGENod(pivot); // recalculez GE pentru pivot
	fiuSt->dr = pivot; // actualizare adresa descendent stanga fiu pivot
	calculGENod(fiuSt);

	return fiuSt;
}

// rotire simpla la stanga (+2, +1/0)
NodAVL* rotireSimplaSt(NodAVL* pivot, NodAVL* fiuDr) {
	pivot->dr = fiuDr->st;
	calculGENod(pivot);
	fiuDr->st = pivot;
	calculGENod(fiuDr);

	return fiuDr;
}

// rotire dubla stanga-dreapta (-2, +1)
NodAVL* rotireDblStDr(NodAVL* pivot, NodAVL* fiuSt) {
	//aducerea dezechilibrului pe aceeasi directie
	pivot->st = rotireSimplaSt(fiuSt, fiuSt->dr);
	calculGENod(pivot);
	fiuSt = pivot->st;
	//rotire propriu-zisa in pivot
	fiuSt = rotireSimplaDr(pivot, fiuSt);
	// calculGENod(fiuSt);

	return fiuSt;
}

// rotire dubla dreapta-stanga (+2, -1)
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

NodAVL *cautaNodUrmator(NodAVL* r) { //cauta cel mai mic nod din subarborele a carui radacina este primita ca parametru
	if (r) {
		if (r->st != NULL) {
			NodAVL* rezultat = cautaNodUrmator(r->st);
			return rezultat;
		}
		else return r;
	}
}

NodAVL *stergereAVL(NodAVL *r, int idStud, int &er) {
	if (r) {
		if (r->stud.id == idStud) {
			// am gasit nodul de sters
			if (r->dr) { // cazul 2 (exista doar dreapta), cazul 3: exista ambii descendenti
				NodAVL* unde = cautaNodUrmator(r->dr); //gaseste adresa nodului cu urm. cheie mai mare decat a nodului pe care vreau sa-l sterg
				if (r->st != NULL) { // r are 2 descendenti (cazul 3)
					Student aux = unde->stud;
					unde->stud = r->stud;
					r->stud = aux;
					r->dr = stergereAVL(r->dr, idStud, er);
				}
				else {
					// cazul 2, singur descendent (dreapta)
					NodAVL* aux = r;
					r = r->dr;
					free(aux->stud.nume);
					free(aux);
				}
			}
			else { // nu exista descendent dreapta
				if (r->st) { // exista stanga; cazul 2: stergere cu 1 descendent (stanga)
					NodAVL* aux = r;
					r = r->st;
					free(aux->stud.nume);
					free(aux);
				}
				else { // nodul de sters este frunza (cazul 1)
					free(r->stud.nume);
					free(r);
					r = NULL;
				}
			}
			er = 1;
		}
		else
			if (r->stud.id > idStud)
				r->st = stergereAVL(r->st, idStud, er);
			else
				r->dr = stergereAVL(r->dr, idStud, er);
	}


	//daca ne aflam initial pe un nod frunza nu se mai face echilibrare (r a devenit null)
	if (r) {
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
		printf(" %d %d \n", rAVL->stud.id, rAVL->GE);
		InordineAVL(rAVL->dr);
	}
}

void main() {

	Student stud;
	NodAVL *root = NULL;

	FILE * f;
	f = fopen("Studenti.txt", "r");
	char studBuff[20];

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
		int err = 0;
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
	printf("\nTraversare arbore inordine:\n\n");
	InordineAVL(root);

	//stergere nod AVL
	int err = 0;
	int idDeSters = 43;
	root = stergereAVL(root, idDeSters, err);
	if (err) {
		printf("S-a sters cu succes studentul cu id %d!", idDeSters);
	}

	printf("\nTraversare arbore inordine dupa stergerea unui student:\n\n");
	InordineAVL(root);

	fclose(f);
}


