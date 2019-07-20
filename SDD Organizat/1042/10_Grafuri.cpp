#include <stdio.h>
#include <malloc.h>

struct NodLS;
struct NodLP {
	int id;
	NodLS * listVecini;
	NodLP * next;
};

struct NodLS {
	int idVecin;
	// int pondere; // pt graf ponderat
	NodLS * next;
};

NodLP* inserareSf(NodLP* la, int idVarf) {
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->id = idVarf;
	nou->listVecini = NULL; // lista de vecini este empty la inserarea unui nod de LP
	nou->next = NULL;

	if (!la) {
		return nou;
	}
	else {
		NodLP* tmp = la;
		while (tmp->next)
			tmp = tmp->next;

		tmp->next = nou;
	}

	return la;
}

NodLS* inserareSf(NodLS* ls, int idNod) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->idVecin = idNod;	
	nou->next = NULL;

	if (!ls) {
		return nou;
	}
	else {
		NodLS* tmp = ls;
		while (tmp->next)
			tmp = tmp->next;

		tmp->next = nou;
	}

	return ls;
}

NodLS* extragereCoada(NodLS* q, int &varf) {
	if (q) {
		NodLS* tmp = q;
		q = q->next;

		varf = tmp->idVecin;
		free(tmp);
	}

	return q;
}

int* BF(NodLP* la, int vStart, int nrv) {
	int * vis, *out;
	NodLS* coada = NULL;

	vis = (int*)malloc(nrv * sizeof(int)); // vector indicatori de stare
	for (int i = 0; i < nrv; i++)
		vis[i] = 0; // nici un varf nu a fost inserat in coada

	out = (int*)malloc(nrv * sizeof(int)); // vector cu id varfuri vizitate/prelucrate
	int k = 0; // index acces la prima poz disponibila la inserare in vectorul out

	coada = inserareSf(coada, vStart);
	vis[vStart - 1] = 1; // marcare vStart ca fiind "vizitat"
	
	while (coada) {
		int varf;
		coada = extragereCoada(coada, varf);
		// salvare nod extras in banda de iesire
		out[k] = varf;
		k += 1;

		NodLP* tmp = la;
		while (tmp) {

			if (tmp->id == varf) {
				// nodul extras din coada este tmp
				NodLS* tms = tmp->listVecini;

				while (tms) {
					if (vis[tms->idVecin - 1] == 0) {
						// varf disponibil pentru prelucrare/vizitare
						coada = inserareSf(coada, tms->idVecin);
						vis[tms->idVecin - 1] = 1;
					}
					tms = tms->next;
				}
			}

			tmp = tmp->next;
		}
	}

	free(vis);

	return out;
}

void main() {
	FILE * f;
	f = fopen("graf.txt", "r");

	// pointer de gestionare a listei de adiacenta
	NodLP* listAd = NULL;

	int nrVarfuri;
	fscanf(f, "%d", &nrVarfuri);
	for (int i = 1; i <= nrVarfuri; i++) // creare lista principala
		listAd = inserareSf(listAd, i);

	int src, dest;

	fscanf(f, "%d", &src);
	while (!feof(f)) {
		fscanf(f, "%d", &dest);
		// plasare pe nodul cu id = src in LP
		// inserare arc (src, dest)
		NodLP* tmp = listAd;
		while (tmp) {
			if (tmp->id == src) {
				// am identificat nodul src
				// inserare dest in lista secundara atasata lui tmp
				tmp->listVecini = inserareSf(tmp->listVecini, dest);				
			}
			tmp = tmp->next;
		}

		// plasare pe nodul cu id = dest in LP
		// inserare arc (dest, src)
		tmp = listAd;
		while (tmp) {
			if (tmp->id == dest) {
				// am identificat nodul src
				// inserare dest in lista secundara atasata lui tmp
				tmp->listVecini = inserareSf(tmp->listVecini, src);
			}
			tmp = tmp->next;
		}

		fscanf(f, "%d", &src);
	}

	// afisare lista de adiacenta
	NodLP* tmp = listAd;
	// traversare LP
	while (tmp) {
		printf("Varful %d are vecinii: ", tmp->id);
		NodLS* tms = tmp->listVecini;
		// traversare LS pentru tmp
		while (tms) {
			printf(" %d ", tms->idVecin);

			tms = tms->next;
		}
		printf("\n");

		tmp = tmp->next;
	}

	int * out = BF(listAd, 6, nrVarfuri);
	// parsare vector rezultat la traversare BF
	printf("Ordine varfuri vizitate la BF: ");
	for (int i = 0; i < nrVarfuri; i++)
		printf(" %d ", out[i]);

	printf("\n");

	// dezalocare banda de iesire BF
	free(out);

	// dezalocare lista de adiacenta

	fclose(f);
}