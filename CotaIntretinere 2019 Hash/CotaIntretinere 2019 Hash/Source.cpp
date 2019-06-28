#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

struct Adresa
{
	char* strada;
	int nr;
};

Adresa creareAdresa(const char* strada, int nr)
{
	Adresa a;
	a.strada = (char*)malloc(sizeof(char)*(strlen(strada) + 1));
	strcpy(a.strada, strada);
	a.nr = nr;
	return a;
}

void afisareAdresa(Adresa a)
{
	printf(" strada %s, nr. %d", a.strada, a.nr);
}
struct CotaIntretinere
{
	Adresa adresa;
	int nrApartament; //cheie cautare;
	int nrPersoane;
	int anul;
	int luna;
	float valIntretinere;
};

CotaIntretinere creareCotaIntr(Adresa adresa, int nrApartament, int nrPersoane, int an, int luna, float intretinere)
{
	CotaIntretinere c;
	c.adresa = creareAdresa(adresa.strada, adresa.nr);
	c.nrApartament = nrApartament;
	c.nrPersoane = nrPersoane;
	c.anul = an;
	c.luna = luna;
	c.valIntretinere = intretinere;
	return c;
}

void afisareCotaIntretinere(CotaIntretinere c)
{
	printf("\nApartmantul nr %d, de la adresa:", c.nrApartament);
	afisareAdresa(c.adresa);
	printf(",in care locuiesc %d persoane are de plata pentru luna %d a anului %d, suma %5.2f lei", c.nrPersoane, c.luna, c.anul, c.valIntretinere);
}


struct nod
{
	CotaIntretinere info;
	nod* next;
};

nod* inserareInceput(nod* cap, CotaIntretinere c)
{
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = c;
	nou->next = cap;
	return nou;
}

struct HashTable
{
	nod* *vector;
	int dim;
};

HashTable creareHashTable(int dim)
{
	HashTable h;
	h.dim = dim;
	h.vector = (nod**)malloc(sizeof(nod)*dim);
	for (int i = 0; i < h.dim; i++)
	{
		h.vector[i] = NULL;
	}
	return h;
}

int functieHash(int nrApartament, int dim)
{
	return nrApartament % dim;
}

int inserareHashTable(HashTable h, CotaIntretinere c)
{
	if (h.dim > 0)
	{
		int k = functieHash(c.nrApartament, h.dim);
		h.vector[k] = inserareInceput(h.vector[k], c);
		return k;
	}
	else
	{
		return -2;
	}
}

void parseHashTable(HashTable h)
{
	if (h.dim > 0)
	{for(int i=0;i<h.dim;i++)
		if (h.vector[i])
		{
			nod* temp = h.vector[i];
			while (temp)
			{
				afisareCotaIntretinere(temp->info);
				temp = temp->next;
			}
		}

	}
}


int cotePestePrag(HashTable h, float prag)
{
	if (h.dim > 0)
	{
		int nrCote = 0;
		for(int i=0;i<h.dim;i++)
			if (h.vector[i])
			{
				nod* temp = h.vector[i];
				while (temp)
				{
					if (temp->info.valIntretinere > prag)
					{
						nrCote++;
					}
					temp = temp->next;
				}
			
			}
		return nrCote;
	}
	
}


float valoareAnualaDupaNr(HashTable h, int nrApartament, int an, Adresa adresa)
{
	if (h.dim > 0)
	{
		int suma = 0;
		int pozitie = functieHash(nrApartament, h.dim);
		if (h.vector[pozitie])
		{
			nod* temp = h.vector[pozitie];
			while (temp)
			{
				if (temp->info.adresa.nr == adresa.nr && strcmp(temp->info.adresa.strada, adresa.strada) == 0 && temp->info.anul == an)
				{
					suma += temp->info.valIntretinere * 12;
				}
				temp = temp->next;
			}
		}
		return suma;
	}

}



void main()
{
	HashTable h = creareHashTable(10);
	inserareHashTable(h, creareCotaIntr(creareAdresa("Strada Fizicienilor", 3), 2, 3, 2019, 6, 340.5));
	inserareHashTable(h, creareCotaIntr(creareAdresa("Strada Lucian Iatropol", 6), 5, 4, 2019, 6, 890));
	inserareHashTable(h, creareCotaIntr(creareAdresa("Strada Victor Babes", 10), 5, 4, 2019, 6, 450.7));
	inserareHashTable(h, creareCotaIntr(creareAdresa("Strada Baba Novac", 26), 3, 4, 2019, 6, 234));
	inserareHashTable(h, creareCotaIntr(creareAdresa("Strada Stefan Stefanescu", 15), 7, 4, 2019, 6, 80));
	inserareHashTable(h, creareCotaIntr(creareAdresa("Strada Tineretului", 45), 3, 4, 2019, 6, 540));
	parseHashTable(h);

	int nrCote = cotePestePrag(h, 100);
	printf("\n\nNr cotelor care depasesc 500 de lei este: %d\n", nrCote);
	float valoareIntretinere = valoareAnualaDupaNr(h, 2, 2019, creareAdresa("Strada Fizicienilor", 3));
	printf("\n\nApartamentul 2 are de plata intr-un an suma de %5.2f lei", valoareIntretinere);
}