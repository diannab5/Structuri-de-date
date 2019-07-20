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

NodABC *cautaNodUrmator(NodABC* r) { //cauta cel mai mic nod din subarborele a carui radacina este primita ca parametru
	if (r) {
		if (r->st != NULL) {
			NodABC* rezultat = cautaNodUrmator(r->st);
			return rezultat;
		}
		else return r;
	}
}

NodABC *stergereNod(NodABC *r, int idStud, int &er) {
	if (r) {
		if (r->s.id == idStud) {
			// am gasit nodul de sters
			if (r->dr) { // cazul 2 (exista doar dreapta), cazul 3: exista ambii descendenti
				NodABC* unde = cautaNodUrmator(r->dr); //gaseste adresa nodului cu urm. cheie mai mare decat a nodului pe care vreau sa-l sterg
				if (r != unde) { // r are 2 descendenti (cazul 3)
					Student aux = unde->s;
					unde->s = r->s;
					r->s = aux;
					r->dr = stergereNod(r->dr, idStud, er);
				}
				else {
					// cazul 2, singur descendent (dreapta)
					NodABC* aux = r;
					r = r->dr;
					free(aux->s.nume);
					free(aux);
				}
			}
			else { // nu exista descendent dreapta
				if (r->st) { // exista stanga; cazul 2: stergere cu 1 descendent (stanga)
					NodABC* aux = r;
					r = r->st;
					free(aux->s.nume);
					free(aux);
				}
				else { // nodul de sters este frunza (cazul 1)
					free(r->s.nume);
					free(r);
					r = NULL;
				}
			}
			er = 1;
		}
		else
			if (r->s.id > idStud)
				r->st = stergereNod(r->st, idStud, er);
			else
				r->dr = stergereNod(r->dr, idStud, er);
	}

	return r;
}

// inaltime de ABC

int inaltimeArbore(NodABC* r) {
	if (r) {
		int inaltimeStanga = inaltimeArbore(r->st);
		int inaltimeDreapta = inaltimeArbore(r->dr);
		if (inaltimeDreapta > inaltimeStanga) {
			return 1 + inaltimeDreapta;
		}
		else {
			return 1 + inaltimeStanga;
		}
	}
	else {
		return 0;
	}
}

// drumul (secventa de id studenti) de la un nod (specificat prin idStud) catre radacina

void drumInvers(NodABC* r, int idStud, int &gasit) {
	if (r) {
		if (idStud < r->s.id) {
			drumInvers(r->st, idStud, gasit);
		}
		else if (idStud > r->s.id) {
			drumInvers(r->dr, idStud, gasit);
		}
		else {
			gasit = 1;
		}
		if (gasit) {
			printf("%d ", r->s.id);
		}

	}
}


void afisareAnumitNivel(NodABC* r, int nivelCurent, int nivelDorit) {
	if (r) {
		if (nivelCurent == nivelDorit) {
			printf("%d ", r->s.id);
		}
		else {
			afisareAnumitNivel(r->st, nivelCurent + 1, nivelDorit);
			afisareAnumitNivel(r->dr, nivelCurent + 1, nivelDorit);
		}
	}
}

// traversare pe niveluri a ABC
void traversarePeNiveluri(NodABC* r) {
	int nrNiveluri = inaltimeArbore(r);
	for (int i = 0; i < nrNiveluri; i++) {
		printf("Nivelul %d:\n", i + 1);
		afisareAnumitNivel(r, 1, i + 1);
		printf("\n");
	}
}

// salvare noduri frunza intr-o lista dubla

struct NodListaDubla {
	Student info;
	NodListaDubla* prev;
	NodListaDubla* next;
};

struct ListaDubla {
	NodListaDubla* prim;
	NodListaDubla* ultim;
};

NodListaDubla* initNod(Student s, NodListaDubla* prev, NodListaDubla* next) {
	NodListaDubla* nou = (NodListaDubla*)malloc(sizeof(NodListaDubla));
	nou->info = s; // SHALLOW COPY
	nou->next = next;
	nou->prev = prev;
	return nou;
}

void inserareLista(ListaDubla &lista, Student s) {
	if (lista.prim) {
		NodListaDubla* nou = initNod(s, lista.ultim, NULL);
		lista.ultim->next = nou;
		lista.ultim = nou;
	}
	else {
		lista.prim = lista.ultim = initNod(s, NULL, NULL);
	}
}

void traversareLista(ListaDubla lista) {
	if (lista.prim) {
		while (lista.prim) {
			printf("%d ", lista.prim->info.id);
			lista.prim = lista.prim->next;
		}
	}
}

void adaugareInLista(NodABC* r, ListaDubla &lista) {
	if (r) {
		if (r->st == NULL && r->dr == NULL) {
			inserareLista(lista, r->s); // AM FACUT SHALLOW COPY
		}
		else {
			adaugareInLista(r->st, lista);
			adaugareInLista(r->dr, lista);
		}
	}
}

void TraversareInordine(NodABC *r)
{
	if (r) {
		TraversareInordine(r->st);
		printf("%d %s\n", r->s.id, r->s.nume);
		TraversareInordine(r->dr);
	}
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

	// stergere nod in arbore binar de cautare
	root = stergereNod(root, 12, err);
	if (err)
		printf("Student identificat si sters\n");
	TraversareInordine(root);

	// afisare drum invers de la un nod din arbore catre radacina
	err = 0;
	printf("Drumul invers catre radacina:\n");
	drumInvers(root, 40, err);

	// traversare pe niveluri
	printf("\nTraversare pe niveluri:\n");
	traversarePeNiveluri(root);

	// salvare noduri frunza in lista dubla
	ListaDubla listaD;
	listaD.prim = listaD.ultim = NULL;
	adaugareInLista(root, listaD);
	printf("Lista dubla cu frunze:\n");
	traversareLista(listaD);
	printf("\n");

	// dezalocari pentru ABC si lista dubla

	// stergere nod in AVL

	fclose(f);
}