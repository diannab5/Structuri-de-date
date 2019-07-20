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
		printf("\n%d %s\n", r->s.id, r->s.nume);
		TraversareInordine(r->dr);
	}
}

NodABC * stergere(NodABC *r, int delKey, int &er) {
	if (!r) {
		er = 1;
	}
	else {
		// cautare nod de sters
		if (r->s.id > delKey)
			r->st = stergere(r->st, delKey, er);
		else
			if (r->s.id < delKey) {
				r->dr = stergere(r->dr, delKey, er);
			}
			else {
				// am identificat nodul de sters (r)
				if (r->st == NULL && r->dr == NULL) {
					// nodul este frunza (cazul 1)
					free(r->s.nume);
					free(r);

					r = NULL;
				}
				else {
					if (r->st != NULL && r->dr != NULL) {
						// nodul de sters are 2 descendenti (cazul 3)
						NodABC* desc = r->st;
						if (desc->dr == NULL) {
							desc->dr = r->dr;
							free(r->s.nume);
							free(r);
							r = desc;
						}
						else {
							NodABC* parinteDesc = NULL;
							while (desc->dr) {
								parinteDesc = desc;
								desc = desc->dr;
							}
							free(r->s.nume);
							// mutare continut in nodul de sters
							r->s = desc->s;

							// actualizare adresa legatura parinte cu subarborele lui desc
							parinteDesc->dr = desc->st;

							// dezalocare de nod (fara nume student care a fost mutat mai devereme - linia 84!?)
							free(desc);
						}
					}
					else {
						// nodul are 1 descendent (cazul 2)
						NodABC* desc;
						if (r->st != NULL)
							desc = r->st;
						else
							desc = r->dr;

						free(r->s.nume);
						free(r);

						r = desc;
					}
				}
			}
		
	}

	return r; // de avut in vedere ca r este adresa intoarsa in apelul recursiv anterior
}

void main() {

	Student stud;
	NodABC *root = NULL;

	FILE * f;
	f = fopen("Studenti.txt", "r");
	char studBuff[20];
	int err;

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
	printf("\nTraversare arbore inordine:\n\n");
	TraversareInordine(root);

	err = 0;
	root = stergere(root, 43, err);
	printf("\nTraversare arbore inordine dupa stergere:\n\n");
	TraversareInordine(root);

	fclose(f);
}