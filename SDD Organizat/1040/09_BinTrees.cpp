#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define LINESIZE 128

struct Student {
	int id; // id - camp cu rol de cheie in arborele binar de cautare
	char *nume;
	float medie; // data derivata
};

struct NodABC {
	Student s;
	NodABC *st, *dr;
};

NodABC *inserare(NodABC *r, Student st, int &er)
{
	if (r == NULL) { // a fost identificat locul de inserat
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

	return r; // intors la sfarsitul fiecarui apel recursiv de functie
}

void TraversareInordine(NodABC *r)
{
	if (r) {
		TraversareInordine(r->st);
		printf("\n%d %s", r->s.id, r->s.nume);
		TraversareInordine(r->dr);
	}
}

NodABC *stergere(NodABC *r, int key, int &er) {
	if (r) {
		if (r->s.id > key)
			r->st = stergere(r->st, key, er);
		else
			if (r->s.id < key) {
				r->dr = stergere(r->dr, key, er);
			}
			else {
				// am gasit nodul de sters
				er = 1;

				if (r->st == NULL) {
					NodABC* tmp = r->dr; // poate fi NULL sau nu
					// dezaloc nodul curent r
					free(r->s.nume);
					free(r);

					r = tmp;
				}
				else {
					if (r->dr == NULL) {
						// exista un sg descendent pe stanga nodului curent r
						NodABC* tmp = r->st;

						// dezalocare nod curent r
						free(r->s.nume);
						free(r);
						r = tmp;
					}
					else {
						// cazul cu 2 descendenti
						NodABC * tmp = r->st;
						while (tmp->dr)
							tmp = tmp->dr; // cautarea celui mai din dreapta nod din subarborele stanga al lui r

						// tmp este nodul cu cheia maxima din subarborele stanga
						// dezalocare nume student pentru nodul de sters
						free(r->s.nume);
						// mutarea datelor din tmp in r
						r->s.id = tmp->s.id;
						r->s.medie = tmp->s.medie;
						r->s.nume = (char*)malloc(strlen(tmp->s.nume) + 1);
						strcpy(r->s.nume, tmp->s.nume);

						r->st = stergere(r->st, tmp->s.id, er);

					}
				}
			}		
	}

	return r;
}

// salvare noduri de pe un nivel specificat intr-o lista simpla
struct NodLS {
	Student s;
	NodLS* next;
};

NodLS* inserareLS(NodLS* list, Student stud) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->s.id = stud.id;
	nou->s.nume = (char*)malloc(strlen(stud.nume) + 1);
	strcpy(nou->s.nume, stud.nume);
	nou->s.medie = stud.medie;

	nou->next = list;

	return nou;
}

NodLS* copiereNoduriNivel(NodABC* r, int nivel, NodLS* list) {
	if (r) {
		if (nivel == 1) {
			// nodul r este plasat pe nivelul dorit
			list = inserareLS(list, r->s);
		}
		else {
			// se continua cautarea nodurilor (apeluri recursive ale functiei pe descendentii lui r)
			list = copiereNoduriNivel(r->st, nivel - 1, list);
			list = copiereNoduriNivel(r->dr, nivel - 1, list);
		}
	}

	return list;
}

void main() {

	Student stud;
	NodABC *root = NULL;

	FILE * f;
	f = fopen("Studenti.txt", "r");
	char studBuff[20];
	int err = 0;

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
		root = inserare(root, stud, err);
		if (err) {
			printf("\nStudentul cu id %d exista in arbore.\n", stud.id);
			free(stud.nume);
		}
		else
			printf("\nStudentul %s a fost inserat\n", stud.nume);
	}

	// traversare in inordine (ordine crescatoare id-uri studenti)
	printf("\nTraversare arbore inordine:\n");
	TraversareInordine(root);
	printf("\n");

	// copiere noduri de pe nivelul 3 in lista simpla
	NodLS* prim = NULL;
	prim = copiereNoduriNivel(root, 3, prim);
	NodLS* t = prim;
	printf("\nLista simpla este:\n");
	while (t) {
		printf(" %d %s\n", t->s.id, t->s.nume);
		t = t->next;
	}

	// stergere nod in ABC
	err = 0;
	root = stergere(root, 31, err);
	if (err) {
		printf("\nNod sters!!!\n");
	}
	err = 0;
	root = stergere(root, 24, err);
	if (err) {
		printf("\nNod sters!!!\n");
	}
	printf("Traversare arbore dupa stergere:\n");
	TraversareInordine(root);
	printf("\n");

	// dezalocare arbore binar de cautare
	while (root) {
		err = 0;
		root = stergere(root, root->s.id, err);
	}
	printf("Traversare arbore dupa dezalocare:\n");
	TraversareInordine(root);
	printf("\n");

	// dezalocare lista simpla prim

	fclose(f);
}

