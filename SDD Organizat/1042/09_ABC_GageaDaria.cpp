#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128

struct Student {
	int id;
	char *nume;
	float medie; // data derivata
};

struct NodABC {
	Student s;
	NodABC *st, *dr;
};

NodABC *inserare(NodABC *r, Student st, int &er)
{
	if (r == NULL) {
		NodABC *nou = new NodABC;
		nou->dr = NULL;
		nou->st = NULL;
		nou->s = st;
		r = nou;
	}
	else if (r->s.id == st.id) er = 1;
	else if (r->s.id > st.id)
		r->st = inserare(r->st, st, er);
	else
		r->dr = inserare(r->dr, st, er);
	return r;
}

void TraversareInordine(NodABC *r)
{
	if (r) {
		TraversareInordine(r->st);
		printf("%d %s\n", r->s.id, r->s.nume);
		TraversareInordine(r->dr);
	}
}

// stergere nod din arbore binar de cautare
// param root -> nod curent in arbore binar de cautare
NodABC* stergeNodArbore(NodABC* root, int idCautat) { //1.stergem nod frunza //2.stergem un nod cu 1 descendent //3.stergem un nod cu 2 descendenti <=> radacina
	if (root != NULL) {
		if (root->s.id > idCautat) { //daca idCautat e mai mic decat id din root caut in stanga
			root->st = stergeNodArbore(root->st, idCautat);
		}
		else if (root->s.id < idCautat) { //altfel caut in dreapta
			root->dr = stergeNodArbore(root->dr, idCautat);
		}
		else { //daca am gasit nodul
			if (root->st == NULL) { //daca nodul e frunza(1) sau are descendent doar pe dreapta(2)
				NodABC* temp = root->dr;
				free(root->s.nume);
				free(root);
				return temp; //pt recursivitate
			}
			else if (root->dr == NULL) { //daca nodul e frunza(1) sau are descendent doar pe stanga(2)
				NodABC *temp = root->st;
				free(root->s.nume);
				free(root);
				return temp;
			}
			else { //daca nodul are 2 descendenti(3)
				NodABC* temp = root->dr; //cautam cea mai din stanga valoare din subarborele din dreapta
				while (temp->st != NULL) {
					temp = temp->st;
				}
				free(root->s.nume);

				// mut continutul lui temp in nodul curent root
				root->s.id = temp->s.id; //cea mai din stanga valoare din subarborele din dreapta va deveni radacina
				root->s.nume = (char*)malloc(strlen(temp->s.nume) + 1);
				strcpy(root->s.nume, temp->s.nume);
				root->s.medie = temp->s.medie;
				root->dr = stergeNodArbore(root->dr, temp->s.id); //stergem nodul temporar
			}

		}
	}
	
	return root;
}

// numarul de noduri de pe un nivel specificat
int nrNoduriDePeNivelSpecificat(NodABC* root, int nivelDorit, int nivelCurent) {
	int nr = 0;
	if (root != NULL) {
		if (nivelDorit == nivelCurent) {
			nr++;
		}
		else {
			nr += nrNoduriDePeNivelSpecificat(root->st, nivelDorit, nivelCurent + 1);
			nr += nrNoduriDePeNivelSpecificat(root->dr, nivelDorit, nivelCurent + 1);
		}
	}
	return nr;
}

// inaltime de arbore binar de cautare
int calculInaltimeArbore(NodABC* root) {
	if (root != NULL) {
		int hLeft = calculInaltimeArbore(root->st);
		int hRight = calculInaltimeArbore(root->dr);
		return 1 + (hLeft > hRight ? hLeft : hRight);
	}
	else {
		return 0;
	}
}

// extragere (stergere) noduri frunza intr-o lista simpla
struct NodLS {
	Student s;
	NodLS* next;
};

Student creareStudent(int id, const char* nume, float medie) {
	Student s;
	s.id = id;
	s.nume = (char*)malloc(strlen(nume) * sizeof(char) + 1);
	strcpy(s.nume, nume);
	s.medie = medie;
	return s;
}

NodLS* inserareLS(NodLS* prim, Student s) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->s = s;
	nou->next = prim;
	return nou;
}

NodLS* extrageNoduriFrunzaInLS(NodABC* &root, NodLS* prim) {
	if (root != NULL) {
		if (root->st == NULL && root->dr == NULL) {
			Student aux = creareStudent(root->s.id, root->s.nume, root->s.medie);
			prim = inserareLS(prim, aux);
		}
		else {
			prim = extrageNoduriFrunzaInLS(root->st, prim);
			stergeNodArbore(root, prim->s.id);
			prim = extrageNoduriFrunzaInLS(root->dr, prim);
			stergeNodArbore(root, prim->s.id);
		}
	}
	return prim;
}

void main() {

	Student stud;
	NodABC *root = NULL;

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
		root = inserare(root, stud, err);
		if (err) {
			printf("\nStudentul cu id %d exista in arbore.\n", stud.id);
			free(stud.nume);
		}
		else
			printf("\nStudentul %s a fost inserat\n", stud.nume);
	}

	// traversare in inordine (ordine crescatoare id-uri studenti)
	printf("\nTraversare arbore inordine:\n\n");
	TraversareInordine(root);

	// stergere nod in arbore binar de cautare
	root = stergeNodArbore(root, 24);
	printf("\nTraversare arbore dupa stergere:\n");
	TraversareInordine(root);

	// nr de noduri pe nivel specificat
	int nr = nrNoduriDePeNivelSpecificat(root, 2, 1);
	printf("\n Nr noduri de pe nivelul specificat: %d\n", nr);

	// inaltime arbore binar de cautare
	int h = calculInaltimeArbore(root);
	printf("\n Inaltime ABC: %d\n", h);
	root = stergeNodArbore(root, 40);
	h = calculInaltimeArbore(root);
	printf("\n Inaltime ABC dupa stergere nod: %d\n", h);

	// extragere noduri frunza in lista simpla
	NodLS * prim = NULL;
	prim = extrageNoduriFrunzaInLS(root, prim);
	NodLS* t = prim;

	// parsare lista simpla cu noduri din ABC
	printf("Lista simpla cu studenti extrasi din ABC:\n");
	while (t) {
		printf(" %d %s\n", t->s.id, t->s.nume);
		t = t->next;
	}

	/* t = prim;
	while (t) {
		root = stergeNodArbore(root, t->s.id);
		t = t->next;
	} */

	printf("\nTraversare arbore dupa extragere noduri in lista simpla:\n");
	TraversareInordine(root);

	fclose(f);
}

// stergere nod in AVL