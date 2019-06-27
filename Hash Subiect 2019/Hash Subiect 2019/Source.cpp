#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>


struct Rezervare
{
	int idRezervare;
	char* denumire;
	unsigned int nrCamereRezervate;
	char* numeClient;
	char* dataRezervarii;
	float sumaPlata;

};

Rezervare creareRezervare(int idRezervare, const char* denumire, unsigned int nrCamereRezervate,const char* numeClient,const char* dataRezervarii, float sumaPlata)
{
	Rezervare r;
	r.idRezervare = idRezervare;
	r.denumire = (char*)malloc(sizeof(char)*(strlen(denumire) + 1));
	strcpy(r.denumire, denumire);
	r.nrCamereRezervate = nrCamereRezervate;
	r.numeClient = (char*)malloc(sizeof(char)*(strlen(numeClient) + 1));
	strcpy(r.numeClient, numeClient);
	r.dataRezervarii = (char*)malloc(sizeof(char)*(strlen(dataRezervarii) + 1));
	strcpy(r.dataRezervarii, dataRezervarii);
	r.sumaPlata = sumaPlata;
	return r;
}

void afisareRezervare(Rezervare r)
{
	printf("Id: %d, denumire hotel: %s, nr camere rezervate: %d, nume client: %s, data rezervare: %s, suma de plata: %5.2f\n", r.idRezervare, r.denumire, r.nrCamereRezervate, r.numeClient, r.dataRezervarii, r.sumaPlata);
}

struct nod
{
	Rezervare info;
	nod* next;
};

nod* inserareInceput(nod* cap,Rezervare r)
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

int functieHash(int idRezervare,int dim)
{
	return idRezervare % dim;
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

int inserareHashTable(HashTable h, Rezervare r)
{
	if (h.dim > 0)
	{
		int k = functieHash(r.idRezervare, h.dim);
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
	for (int i = 0; i<h.dim; i++)
		if (h.vector[i])
		{
			nod* temp = h.vector[i];
			while (temp)
			{
				afisareRezervare(temp->info);
				temp = temp->next;
			}
		}
}


int afisareNumar(HashTable h, const char* dataCalendaristica)
{
	if (h.dim > 0)
	{
		for (int i = 0; i < h.dim; i++)
		{
			if (h.vector[i])
			{
				nod* temp = h.vector[i];
				while (temp)
				{
					if (strcmp(temp->info.dataRezervarii, dataCalendaristica)==0)
						return temp->info.nrCamereRezervate;
					temp = temp->next;
				}
			}
		}
	}
}

float sumaTotalaDePlata(HashTable h, const char* numeClient)
{
	float suma = 0;
	if (h.dim > 0)
	{
		for (int i = 0; i < h.dim; i++)
		{
			if (h.vector[i])
			{
				nod* temp = h.vector[i];
				while (temp)
				{
					if (!(strcmp(temp->info.numeClient, numeClient)))
						suma += temp->info.sumaPlata;
					temp = temp->next;
			 }
			}
		}
	}
	return suma;
}

void modificaData(HashTable h, int idRezervare, char* dataNoua)
{
	if (h.dim > 0)
	{
		for (int i = 0; i < h.dim; i++)
		{
			if (h.vector[i])
			{
				nod* temp = h.vector[i];
				while (temp)
				{
					if (temp->info.idRezervare == idRezervare)
						temp->info.dataRezervarii = dataNoua;
					temp = temp->next;
				}
			}
		}
	}
}

Rezervare cautaDupaId(int idRezervare, HashTable h)
{
	if (h.vector)
	{
		int pozitie = functieHash(idRezervare, h.dim);
		nod* temp = h.vector[pozitie];
		while (temp&& idRezervare == temp->info.idRezervare)
		{
			return temp->info;
		}
		temp = temp->next;
	}
	else
	{
		return creareRezervare(0, "", 0, "", "", NULL);
	}
}


void main()
{
	HashTable h = creareHashTable(10);
	inserareHashTable(h, creareRezervare(1, "Hotel Medusa", 2, "Mircea Iulian", "12-10-2019", 500.6));
	inserareHashTable(h, creareRezervare(5, "Hotel Miriam", 4, "Laura Ludovic", "13-10-2019", 600.56));
	inserareHashTable(h, creareRezervare(4, "Hotel Samson", 2, "Kilian Mara", "17-10-2019", 405));
	inserareHashTable(h, creareRezervare(7, "Hotel Beluga", 1, "Kara Morodan", "18-11-2019", 305));
	inserareHashTable(h, creareRezervare(2, "Hotel Imerse", 3, "Sara Mal", "15-10-2019", 307));
	inserareHashTable(h, creareRezervare(4, "Hotel Mirage", 1, "Alexandra Marva", "14-10-2019", 200));
	afisareHashTable(h);

	printf("\nNr de camere rezervate la data cautata: %d", afisareNumar(h, "12-10-2019"));
	printf("\nSuma totala de plata pe care trebuie sa o plateasca clientul este: %5.2f\n", sumaTotalaDePlata(h, "Sara Mal"));
	
	char newData[] = { "01.01.2019" };
	modificaData(h, 5, newData);
	printf("\n\nDupa modificarea datei:");
	afisareHashTable(h);
	Rezervare rezervareCautata = cautaDupaId(1, h);
	printf("\n\n");
	afisareRezervare(rezervareCautata);
}