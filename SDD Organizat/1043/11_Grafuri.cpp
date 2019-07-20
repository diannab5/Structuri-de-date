#include <stdio.h>
#include <malloc.h>

// structura nod lista principala
struct NodLS;

struct NodLP {
	unsigned int idVarf;
	NodLS* primListaVecini;
	NodLP* next;
};

// structura nod lista secundara
struct NodLS {
	unsigned int idNodAdiacent;
	// int pondereArc; // daca este cazul, se utilizeaza campul pentru greutate/pondere arc
	NodLS* next;
};

// structura nod stiva
struct Nod {
	unsigned int idVarf;
	Nod* next;
};

// inserare nod in lista secundara
NodLS* inserareNodLS(NodLS* pLS, unsigned int id) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->idNodAdiacent = id;
	nou->next = 0; // NULL

	if (!pLS) { // LS nu contine nici un nod
		return nou;
	}
	else {
		// parsare LS pana la ultimul nod
		NodLS* tmp = pLS;
		while (tmp->next)
			tmp = tmp->next;

		tmp->next = nou;
	}

	return pLS;
}

// inserare nod in lista principala
NodLP* inserareNodLP(NodLP* pLP, unsigned int id) {
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->idVarf = id;
	nou->primListaVecini = NULL;
	nou->next = 0; // NULL

	if (!pLP) { // LP nu contine nici un nod
		return nou;
	}
	else {
		// parsare LP pana la ultimul nod
		NodLP* tmp = pLP;
		while (tmp->next)
			tmp = tmp->next;

		tmp->next = nou;
	}

	return pLP;
}

NodLP* cautaNodLP(NodLP* pLP, unsigned int idCautat) {
	NodLP* tmp = pLP;
	while (tmp) {
		if (tmp->idVarf == idCautat)
			return tmp; // adresa nod in LP cu idCautat

		tmp = tmp->next;
	}

	return tmp; // NULL; nu exista nod in LP cu idCautat
}

Nod* push(Nod* s, unsigned int varf) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->idVarf = varf;
	nou->next = s;

	return nou;
}

Nod* pop(Nod* s, unsigned int &varf) {
	if (s) {
		Nod* tmp = s;
		s = s->next;
		varf = tmp->idVarf;

		free(tmp);
	}

	return s;
}

// traversare DF a grafului
unsigned int* DF(NodLP* pLP, unsigned int start, unsigned int nrVarfuri) {
	unsigned int * flags, *output, k = 0;
	Nod * stack = 0;

	// alocare vector de flaguri
	flags = (unsigned int*)malloc(nrVarfuri * sizeof(unsigned int));
	// initializare vector flaguri
	for (unsigned int i = 0; i<nrVarfuri; i++)
		flags[i] = 0;

	// alocare vector de iesire
	output = (unsigned int*)malloc(nrVarfuri * sizeof(unsigned int));

	// push varf de start
	stack = push(stack, start);
	flags[start - 1] = 1; // marcare start cu "vizitat"

	while (stack) {
		unsigned int varf;
		// extragere nod graf din varful stivei
		stack = pop(stack, varf);

		// salvare in banda de iesire a varfului extras in vederea prelucrarii
		output[k] = varf;
		k = k + 1;

		NodLP* tmp = cautaNodLP(pLP, varf);
		NodLS* tms = tmp->primListaVecini;
		while (tms) {
			// stabilire vecini pentru varf
			if (flags[tms->idNodAdiacent - 1] == 0) {
				// push pentru vecinii cu flags nul
				stack = push(stack, tms->idNodAdiacent);
				// comutare flags pentru vecinii salvati pe stiva
				flags[tms->idNodAdiacent - 1] = 1;
			}

			tms = tms->next;
		}

	}

	free(flags);

	return output;
}

// functia put pentru coada

// functia get pentru coada

// functia traversare BF
unsigned int* BF(NodLP* pLP, unsigned int start, unsigned int nrVarfuri) {

}

void main() {
	FILE *f;
	f = fopen("Graf.txt", "r");

	char buffer[128];
	fgets(buffer, sizeof(buffer), f);

	NodLP* pListaAdiacenta = 0;
	unsigned int nrVarfuri = 0;

	sscanf(buffer, "%d", &nrVarfuri); // preluare nr varfuri de pe prima linie din fisier
	for (unsigned int i = 1; i <= nrVarfuri; i++) // creare noduri LP
		pListaAdiacenta = inserareNodLP(pListaAdiacenta, i);


	while (fgets(buffer, sizeof(buffer), f)) {
		unsigned int src, dst;
		sscanf(buffer, "%d %d", &src, &dst);

		NodLP* tmpLP = cautaNodLP(pListaAdiacenta, src);

		if (tmpLP) {
			// nodul cu idVarf == src
			tmpLP->primListaVecini = inserareNodLS(tmpLP->primListaVecini, dst);
		}
		else {
			// nu exista nodul cautat in LP
			printf("\nNodul cautat cu id %d nu exista.\n", src);
		}

		tmpLP = cautaNodLP(pListaAdiacenta, dst);
		if (tmpLP) {
			// nodul cu idVarf == src
			tmpLP->primListaVecini = inserareNodLS(tmpLP->primListaVecini, src);
		}
		else {
			// nu exista nodul cautat in LP
			printf("\nNodul cautat cu id %d nu exista.\n", dst);
		}
	}

	// parsare lista de adiacenta
	NodLP* tmpLP = pListaAdiacenta;
	while (tmpLP) {

		printf("\nVarf %d cu vecinii: ", tmpLP->idVarf);

		NodLS* tmpLS = tmpLP->primListaVecini;
		while (tmpLS) {
			printf(" %d ", tmpLS->idNodAdiacent);

			tmpLS = tmpLS->next;
		}

		tmpLP = tmpLP->next;
	}

	// traversare depth-first graf
	unsigned int* out = DF(pListaAdiacenta, 1, nrVarfuri);

	printf("\n Traversare DF graf:");
	for (int i = 0; i<nrVarfuri; i++)
		printf(" %d ", out[i]);

	printf("\n\n");
	fclose(f);
}


