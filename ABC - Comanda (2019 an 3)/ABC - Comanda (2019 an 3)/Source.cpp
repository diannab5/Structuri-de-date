#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>


struct Produs
{
	char* denumireProdus;
	int cantitate;
	char* unitateMasura;
};

struct Comanda
{
	int idComanda;
	char* denumireClient;
	int nrProduse;
	Produs* produse;
	char* dataLivrare;
};

Produs creareProdus(const char* denumireProdus, int cantitate,const char* unitateMasura)
{
	Produs p;
	p.denumireProdus = (char*)malloc(sizeof(char)*(strlen(denumireProdus) + 1));
	strcpy(p.denumireProdus, denumireProdus);
	p.cantitate = cantitate;
	p.unitateMasura = (char*)malloc(sizeof(char)*(strlen(unitateMasura) + 1));
	strcpy(p.unitateMasura, unitateMasura);
	return p;
}

void afisareProdus(Produs p)
{
	printf("\nProdusul %s, cantitatea %d %s", p.denumireProdus, p.cantitate, p.unitateMasura);
}

Comanda crearecomanda(int idComanda, const char* denumireClient, int nrProduse, Produs* produse, const char* dataLivrare)
{
	Comanda c;
	c.idComanda = idComanda;
	c.denumireClient = (char*)malloc(sizeof(char)*(strlen(denumireClient) + 1));
	strcpy(c.denumireClient, denumireClient);
	c.nrProduse = nrProduse;
	c.produse = (Produs*)malloc(sizeof(Produs)*nrProduse);
	for (int i = 0; i < nrProduse; i++)
	{
		c.produse[i] = creareProdus(produse[i].denumireProdus, produse[i].cantitate, produse[i].unitateMasura);
	}
	c.dataLivrare = (char*)malloc(sizeof(char)*(strlen(dataLivrare) + 1));
	strcpy(c.dataLivrare, dataLivrare);
	return c;

}

void afisareComanda(Comanda c)
{
	printf("\nComanda cu id %d, plasata de %s va fi livrata pe %s si are %d produse: ", c.idComanda, c.denumireClient, c.dataLivrare, c.nrProduse);
	for (int i = 0; i < c.nrProduse; i++)
	{
		afisareProdus(c.produse[i]);
	}
	printf("\n");
}

struct nod
{
	Comanda info;
	nod* st;
	nod* dr;
};

nod* inserareArbore(nod* rad, Comanda c)
{
	if (rad)
	{
		if (rad->info.idComanda > c.idComanda)
		{
			rad->st = inserareArbore(rad->st, c);
		}
		else
		{
			rad->dr = inserareArbore(rad->dr, c);
		}
		return rad;
	}
	else
	{
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->dr = NULL;
		nou->st = NULL;
		nou->info = crearecomanda(c.idComanda, c.denumireClient, c.nrProduse, c.produse, c.dataLivrare);
		return nou;
	}
}

void afisareArboreInOrdine(nod* rad)
{
	if (rad)
	{
		afisareArboreInOrdine(rad->st);
		afisareComanda(rad->info);
		afisareArboreInOrdine(rad->dr);
	}
}

int nrFrunze(nod* rad, const char* denumireClient, int &nrf)
{
	if (rad)
	{
		nrFrunze(rad->st, denumireClient, nrf);
		if ( strcmp(rad->info.denumireClient, denumireClient) == 0 && rad->st == NULL & rad->dr == NULL);
		{
			nrf++;
			return nrf;
		}
		nrFrunze(rad->dr, denumireClient, nrf);
	}
}

void modificaProduse(nod* rad, int idComanda, Produs* produse, int nrProduse)
{
	if (rad)
	{
		if (rad->info.idComanda == idComanda)
		{
			free(rad->info.produse);
			rad->info.nrProduse = nrProduse;
			rad->info.produse = (Produs*)malloc(sizeof(Produs)*nrProduse);
			for (int i = 0; i < nrProduse; i++)
			{
				rad->info.produse[i] = creareProdus(produse[i].denumireProdus, produse[i].cantitate, produse[i].unitateMasura);
			}
		}
		else if (rad->info.idComanda > idComanda)
		{
			modificaProduse(rad->st, idComanda, produse, nrProduse);

		}
		else
		{
			modificaProduse(rad->dr, idComanda, produse, nrProduse);
		}
	}
}

Comanda cautaComandaDupaId(nod* rad, int idComanda)
{
	if (rad)
	{
		if (rad->info.idComanda == idComanda)
		{
			return rad->info;
		}
		else if (rad->info.idComanda > idComanda)
		{
			return cautaComandaDupaId(rad->st, idComanda);
		}
		else
		{
			return cautaComandaDupaId(rad->dr, idComanda);
		}
	}
}

int* idDupaData(int* &vector, int &dim, nod* rad, const char* data)
{
	if (rad)
	{
		idDupaData(vector, dim, rad->st, data);
		if (strcmp(rad->info.dataLivrare,data) == 0)
		{
			int* temp = (int*)malloc(sizeof(int)*dim); //aloc spatiu pt temp
			for (int i = 0; i < dim; i++) //parcurg vectorul
			{
				temp[i] = vector[i]; //temporar = vectorul meu
			}
			temp[dim] = rad->info.idComanda;
			dim++;
			vector = temp;
			return vector;
		}
		idDupaData(vector, dim, rad->dr, data);
	}
}
void main()
{
	nod* rad = NULL;
	Produs* produse1 = (Produs*)malloc(sizeof(Produs) * 2);
	Produs* produse2 = (Produs*)malloc(sizeof(Produs));
	Produs* produse3 = (Produs*)malloc(sizeof(Produs) * 2);
	Produs* produse4 = (Produs*)malloc(sizeof(Produs) * 3);
	Produs* produse5 = (Produs*)malloc(sizeof(Produs));

	produse1[0] = creareProdus("Huawei P30", 2, "unitate");
	produse1[1] = creareProdus("Iphone 8s", 5, "unitate");

	produse2[0] = creareProdus("lapte", 4, "litri");

	produse3[0] = creareProdus("oua", 3, "kg");
	produse3[1] = creareProdus("zahar", 5, "kg");

	produse4[0] = creareProdus("mere", 2, "kg");
	produse4[1] = creareProdus("pere", 4, "kg");
	produse4[2] = creareProdus("prune", 3, "kg");

	produse5[0] = creareProdus("cana", 3, "seturi");

	rad = inserareArbore(rad, crearecomanda(20, "Ioana", 2, produse1, "12-10-2019"));
	rad = inserareArbore(rad, crearecomanda(15, "Diana", 1, produse2, "13-10-2019"));
	rad = inserareArbore(rad, crearecomanda(25, "Miruna", 2, produse3, "19-10-2019"));
	rad = inserareArbore(rad, crearecomanda(3, "Andreea", 3, produse4, "12-10-2019"));
	rad = inserareArbore(rad, crearecomanda(16, "Mara", 1, produse5, "14-10-2019"));
	afisareArboreInOrdine(rad);
	int nrf = 0;
	nrFrunze(rad, "Andreea", nrf);
	printf("\n\n Nr de frunze al clientului Andreea %d \n", nrf);


	printf("\n\nComanda inainte de modificare:");
	Comanda c = cautaComandaDupaId(rad, 16);
	afisareComanda(c);

	printf("\n\nComanda dupa modificare:");
	modificaProduse(rad, 16, produse4, 3);
	c = cautaComandaDupaId(rad, 16);
	afisareComanda(c);


	int* vector;
	int dim = 0;
	int* rezultat = idDupaData(vector, dim, rad, "12-10-2019");
	printf("\n\nPe data de 12-10-2019 vor fi livrate comenzile: ");
	for (int i = 0; i < dim; i++)
	{
		printf(" %d ", rezultat[i]);
	}

}