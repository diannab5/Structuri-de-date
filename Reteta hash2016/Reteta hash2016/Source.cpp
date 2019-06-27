#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>


struct Reteta
{
	unsigned int nrReteta;
	int nrMedPresc;
	char* numeMedic;
	float valBruta;
	float procentCompensare;
	char** listaMedPresc;
};

Reteta creareReteta(unsigned int nrReteta, int nrMedPresc, const char** listaMedPresc,const char* numeMedic, float valBruta, float procentCompensare)
{
	Reteta r;
	r.nrReteta = nrReteta;
	r.nrMedPresc = nrMedPresc;
	r.numeMedic = (char*)malloc(sizeof(char)*(strlen(numeMedic) + 1));
	strcpy(r.numeMedic, numeMedic);
	r.valBruta = valBruta;
	r.procentCompensare = procentCompensare;
	r.listaMedPresc = (char**)malloc(sizeof(char*)*r.nrMedPresc);
	for (int i = 0; i < nrMedPresc; i++)
	{
		r.listaMedPresc[i] = (char*)malloc((strlen(listaMedPresc[i]) + 1) * sizeof(char));
		strcpy(r.listaMedPresc[i], listaMedPresc[i]);
	}
	return r;
}

void afisareReteta(Reteta r)
{
	printf("\nNr reteta: %d, nr medicamente: %d, nume medic: %s, val bruta: %5.2f, procent compensare: %5.2f\n", r.nrReteta, r.nrMedPresc, r.numeMedic, r.valBruta, r.procentCompensare);
	printf(" lista medicamente: ");
	for (int i = 0; i < r.nrMedPresc; i++)
		printf("%s ", r.listaMedPresc[i]);
}

struct nod
{
	Reteta info;
	nod* next;

};

nod* inserareInceput(nod* cap, Reteta r)
{
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = r;
	nou->next = cap;
	return nou;
}

struct HashTable
{
	nod* *vector;
	int dim;
};

int functieHash(unsigned int nrReteta,int dim)
{
	return nrReteta % dim;
}

HashTable creareHashTable(int dim)
{
	HashTable h;
	h.dim = dim;
	h.vector = (nod**)malloc(sizeof(nod*)*dim);
	for (int i = 0; i < dim; i++)
	{
		h.vector[i] = NULL;
	}
	return h;
}

int inserareHashTable(HashTable h, Reteta r)
{
	if (h.dim > 0)
	{
		int k = functieHash(h.dim, r.nrReteta);
		h.vector[k] = inserareInceput(h.vector[k], r);
		return k;
	}
	else
	{
		return -2;
	}
}


void afisareHashTable(HashTable h)
{
	for(int i=0;i<h.dim;i++)
		if (h.vector[i])
		{
			nod* temp = h.vector[i];
			while (temp)
			{
				afisareReteta(temp->info);
				temp = temp->next;

			}
		}
}



void calculValoareNeta(HashTable h,float *valoareNeta)
{
	float valNeta = 0;
	if (h.dim > 0)
	{
		for(int i=0;i<h.dim;i++)
			if (h.vector[i])
			{
				nod* temp = h.vector[i];
				while (temp)
				{
					valNeta = temp->info.valBruta*(1-temp->info.procentCompensare/100);
					*valoareNeta += valNeta;
					temp = temp->next;
				}
			}
	}
}


void main()
{
	const char* lista1[2] = { "sirop","paracetamol" };
	const char* lista2[2] = { "metroprorol","ceva" };
	const char* lista3[3] = { "antinevralgic","sintromp","probiotice" };
	const char* lista4[2] = { "ibuprofen","no-spa" };
	const char* lista5[2]={"antinevralgic","sirop"};
	const char* lista6[3] = { "sirop","paracetamol","aspirina" };
	HashTable h=creareHashTable(10);
	inserareHashTable(h, creareReteta(4, 2, lista1, "Iulian Ilie", 345, 1.5));
	inserareHashTable(h, creareReteta(3, 2, lista2, "Marian Stan", 400, 1.2));
	inserareHashTable(h, creareReteta(4, 3, lista3, "Laura Mavrodin", 590, 1.3));
	inserareHashTable(h, creareReteta(7, 2, lista4, "Laura Mavrodin", 450, 1.2));
	inserareHashTable(h, creareReteta(2, 2, lista2, "Julian Mai", 230.5, 1.2));
	inserareHashTable(h, creareReteta(4, 3, lista6, "Mauro Iribin", 690.5, 1.4));
	afisareHashTable(h);
	float valoareNeta = 0;
	calculValoareNeta(h, &valoareNeta);
	printf("\nValoarea neta a tuturor retetelor este=%5.2f", valoareNeta);
}