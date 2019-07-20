#include <stdio.h>
#include <malloc.h>

struct NodLS;
struct NodLP {
	char id;
	NodLS *lVecini;
	NodLP *next;
};

struct NodLS {
	char idVecin;
	NodLS *next;
};

NodLP* inserareNodLP(NodLP* lp, char idNod) {
	// inserare la sfarsit in lista principala
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->id = idNod;
	nou->lVecini = NULL; // initilizare lista de vecini pt idNod (empty)
	nou->next = NULL;

	if (!lp) {
		// lista este empty
		return nou;
	}
	else {
		// lista principala contine cel putin 1 nod
		NodLP* tmp = lp;
		while (tmp->next)
			tmp = tmp->next;

		tmp->next = nou;
	}

	return lp;
}

NodLS* inserareNodLS(NodLS* ls, char idNod) {
	// inserare la sfarsit in lista secundara
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->idVecin = idNod;
	nou->next = NULL;

	if (!ls) {
		// lista este empty
		return nou;
	}
	else {
		// lista principala contine cel putin 1 nod
		NodLS* tmp = ls;
		while (tmp->next)
			tmp = tmp->next;

		tmp->next = nou;
	}

	return ls;
}

NodLS* extragereNodCoada(NodLS * coada, char& idNod) {
	if (coada) {
		NodLS* tmp = coada;
		coada = coada->next;

		idNod = tmp->idVecin;
		free(tmp);
	}

	return coada;
}

char* BF(NodLP* lAd, int nrv, char vStart) {
	NodLS* coada = NULL;
	char *vis, *out;
	vis = (char*)malloc(nrv * sizeof(char));
	out = (char*)malloc(nrv * sizeof(char));

	// initializare vector de indicatori de stare
	for (int i = 0; i < nrv; i++)
		vis[i] = 0;

	int k = 0;
	coada = inserareNodLS(coada, vStart);
	vis[vStart - 1] = 1;
	while (coada) {
		char vExtras;
		coada = extragereNodCoada(coada, vExtras);
		out[k] = vExtras;
		k += 1;
		NodLP* tmp = lAd;
		while (tmp) {
			if (tmp->id == vExtras) {
				// am identificat varf extras in LP
				// parsez lista de vecini si salvez in coada vecinii cu is nul
				NodLS* tls = tmp->lVecini;
				while (tls) {
					if (vis[tls->idVecin - 1] == 0) {
						// nodul tls->idVecin nu a trecut prin coada
						coada = inserareNodLS(coada, tls->idVecin);
						vis[tls->idVecin - 1] = 1;
					}
					tls = tls->next;
				}
			}

			tmp = tmp->next;
		}
	}
	// dezalocar vector vis
	free(vis);

	return out;
}

void main() {
	FILE *f = fopen("graf.txt", "r");
	int nod, nrv;
	NodLP* listAd = NULL;

	fscanf(f, "%d", &nrv); // nr de noduri din graf preluat din fisier
	for (char i = 1; i <= nrv; i++)
		listAd = inserareNodLP(listAd, i); // creare lista principala prin inserari succesive
	

	fscanf(f, "%d", &nod);
	while (!feof(f)) {
		int destinatie;
		fscanf(f, "%d", &destinatie);
		// exista arc (nod, destinatie)

		// inserare (nod, destinatie)
		NodLP* tmp = listAd;
		while (tmp) {
			if (tmp->id == nod) {
				tmp->lVecini = inserareNodLS(tmp->lVecini, destinatie);
			}

			tmp = tmp->next;
		}
		// inserare (destinatie, nod) -- graful nu este orientat
		tmp = listAd;
		while (tmp) {
			if (tmp->id == destinatie) {
				tmp->lVecini = inserareNodLS(tmp->lVecini, nod);
			}

			tmp = tmp->next;
		}

		fscanf(f, "%d", &nod);
	}

	// lista de adiacenta creata
	NodLP* tmp = listAd;
	while (tmp) {
		printf("Nodul %d are vecinii: ", tmp->id);
		NodLS* tls = tmp->lVecini;
		while (tls) {
			printf(" %d ", tls->idVecin);

			tls = tls->next;
		}
		printf("\n");

		tmp = tmp->next;
	}

	// traversare BF a grafului
	char* out = BF(listAd, nrv, 6);
	printf("\nTraversare BF:");
	for (int i = 0; i < nrv; i++)
		printf(" %d ", out[i]);
	printf("\n\n");

	free(out); // dezalocare banda de iesire traversare BF
	fclose(f);
}