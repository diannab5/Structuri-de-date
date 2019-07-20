#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>


struct Aplicatie
{
	char *denumire;
	char *categorie;
	float dimensiune;
	float pret;
	float discount;

};

struct ListaAplicatii
{
	char *denumireAplicatie;
	ListaAplicatii *next;
};

struct Distributie
{
	char *denumireCategorie;
	int nrAplicatii;
};

struct HashAplicatie
{
	Aplicatie *elem;
	int dimensiune;
};

int HashFunction(char *denumire, HashAplicatie hs)
{
	return (denumire[0] + denumire[strlen(denumire)]) % hs.dimensiune;
}


ListaAplicatii* inserareAplicatieInLS(ListaAplicatii *cap, char *denumireAplicatie)
{
	ListaAplicatii *temp = (ListaAplicatii*)malloc(sizeof(ListaAplicatii));
	ListaAplicatii *p;
	temp->denumireAplicatie = (char*)malloc((strlen(denumireAplicatie) + 1)*sizeof(char));
	strcpy(temp->denumireAplicatie, denumireAplicatie);
	temp->next = NULL;
	if (cap == NULL)
	{
		return temp;
	}
	else
	{
		for (p = cap; p->next; p = p->next);
			p->next = temp;
	}
	return cap;
}

HashAplicatie AlocareMemorie()
{
	
	HashAplicatie hs;
	hs.dimensiune = 101;
	//vector de Aplicatii
	hs.elem = new Aplicatie[hs.dimensiune];
	for (int i = 0; i < hs.dimensiune; i++)
	{
		hs.elem[i].denumire=""; //pozitie disponibila la inserare
	}

	return hs;
}

int inserareAplicatieInHash(Aplicatie aplicatie, HashAplicatie hs)
{
	int pozitie = -1;
	if (aplicatie.denumire == "")
	{
		return pozitie;
	}

	if (hs.elem != NULL)
	{
		pozitie = HashFunction(aplicatie.denumire, hs);
		if (hs.elem[pozitie].denumire == "") //pozitie disponibila
		{
			hs.elem[pozitie] = aplicatie;
		}
		else
		{
			//coliziune, cautam o pozitie libera
			int gasit = 0;
			for (int i = pozitie + 1; i < hs.dimensiune && !gasit; i++) //cautam pozitie libera in dreapta elementului cu acelasi index
			{
				if (hs.elem[i].denumire == "")
				{
					gasit = i;
					hs.elem[i] = aplicatie;
				}
			}
			if (!gasit)
			{
				for (int i = pozitie - 1; i >= 0 && !gasit; i--) //cautam pozitie libera in stanga elem cu acelasi index
				{
					if (hs.elem[i].denumire == "")
					{
						gasit = i;
						hs.elem[i] = aplicatie;
					}
				}
			}
			pozitie = gasit;

		}
	}
	return pozitie;
}

void afisareHashAplicatie(HashAplicatie hs)
{
	if (hs.elem != NULL)
	{
		for (int i = 0; i < hs.dimensiune; i++)
		{
			if (hs.elem[i].denumire != "")
			{
				printf("\nAplicatia s-a inserat pe pozitia %d ", i);
				printf("\nDenumire=%s Categorie=%s Dimensiune=%5.2f Pret=%5.2f Discount=%5.2f ", hs.elem[i].denumire, hs.elem[i].categorie, hs.elem[i].dimensiune, hs.elem[i].pret, hs.elem[i].discount);
			}
		}
	}
}

float calculDimensiuneTotalaAplicatii(float dimensiuneTotala, HashAplicatie hs)
{
	if (hs.elem != NULL)
	{
		for (int i = 0; i < hs.dimensiune; i++)
		{
			if (hs.elem[i].denumire != "")
			{
				dimensiuneTotala += hs.elem[i].dimensiune;
			}
			
		}
	}

	return dimensiuneTotala;
}

float calculValoareTotalaDiscountPerCategorie(float val, char categorieCeruta[25], HashAplicatie hs)
{
	if (hs.elem != NULL)
	{
		for (int i = 0; i < hs.dimensiune; i++)
		{
			if (hs.elem[i].denumire != "" && strcmp(hs.elem[i].categorie, categorieCeruta) == 0)
			{
				val += hs.elem[i].pret*hs.elem[i].discount;
			}
		}
	}
	return val;
}

void calculareNrAplicatiiPerCategorie(HashAplicatie hs, Distributie *distributie, int nrElem)
{
	if (hs.elem != NULL)
	{
		for (int j = 0; j < nrElem; j++)
		{
			for (int i = 0; i < hs.dimensiune; i++)
			{
				if (hs.elem[i].denumire != "" && strcmp(hs.elem[i].categorie, distributie[j].denumireCategorie) == 0)
				{
					distributie[j].nrAplicatii++;
				}
			}
		}
	}
}

void eliminareCategoriiDuplicate(Distributie *distributie, int *nrElem)
{
	for (int i = 0; i < *nrElem; i++)
	{
		for (int j = i + 1; j < *nrElem;)
		{
			if (strcmp(distributie[i].denumireCategorie, distributie[j].denumireCategorie) == 0)
			{
				for (int k = j; k < *nrElem; k++)
				{
					distributie[k] = distributie[k + 1];
				}
				(*nrElem)--;
			}
			else
				j++;
		}
	}


}

void distributieCategorii(Distributie *distributie, int *nrElem,Aplicatie *aplicatie,int *nrCatDistincte)
{
	for (int i = 0; i < *nrElem; i++)
	{
		//copiem categoriile din vectorul aplicatiilor in vectorul distributiilor
		distributie[i].denumireCategorie = (char*)malloc((strlen(aplicatie[i].categorie) + 1)*sizeof(char));
		strcpy(distributie[i].denumireCategorie, aplicatie[i].categorie);
		distributie[i].nrAplicatii = 0;
	}
	*nrCatDistincte = *nrElem;
	//eliminam duplicatele
	eliminareCategoriiDuplicate(distributie, nrCatDistincte);
}

void parcurgereListaAplicatiiDiscountMin(ListaAplicatii *cap)
{
	ListaAplicatii *aux = cap;
	while (aux)
	{
		printf("\n%s ", aux->denumireAplicatie);
		aux = aux->next;
	}
}

void listaAplicatiiDiscountMinim(HashAplicatie hs, char cat[25])
{
	ListaAplicatii *cap = NULL;
	if (hs.elem != NULL)
	{
		for (int i = 0; i < hs.dimensiune; i++)
		{
			if (hs.elem[i].denumire != "" && strcmp(hs.elem[i].categorie, cat) == 0 && hs.elem[i].discount > 0.10)
			{
				cap = inserareAplicatieInLS(cap, hs.elem[i].denumire);
			}
		}
	}
	parcurgereListaAplicatiiDiscountMin(cap);
}

void main()
{
	HashAplicatie hs = AlocareMemorie();
	Aplicatie *aplicatie;
	int nrAplicatii = 0;
	FILE *f = fopen("aplicatii.txt", "r");
	char *token[120], linie[100], sep_list[] = ",";

	while (fgets(linie, sizeof(linie), f))
	{
		nrAplicatii++;
	}
	fclose(f);

	aplicatie = new Aplicatie[nrAplicatii];
	f = fopen("aplicatii.txt", "r");
	int k = 0;
	while (fgets(linie, sizeof(linie), f))
	{
		token[0] = strtok(linie, sep_list);
		aplicatie[k].denumire = (char*)malloc((strlen(token[0]) + 1)*sizeof(char));
		strcpy(aplicatie[k].denumire, token[0]);

		for (int i = 1; i < 5; i++)
		{
			token[i] = strtok(NULL, sep_list);
		}

		aplicatie[k].categorie = (char*)malloc((strlen(token[1]) + 1)*sizeof(char));
		strcpy(aplicatie[k].categorie, token[1]);

		aplicatie[k].dimensiune = atof(token[2]);
		aplicatie[k].pret = atof(token[3]);
		aplicatie[k].discount = atof(token[4]);
		inserareAplicatieInHash(aplicatie[k], hs);
		k++;
	}
	fclose(f);

	afisareHashAplicatie(hs);

	float dimensiuneTotalaAplicatii = 0;
	dimensiuneTotalaAplicatii = calculDimensiuneTotalaAplicatii(dimensiuneTotalaAplicatii, hs);
	printf("\nDimensiunea totala a aplicatiilor este=%5.2f ", dimensiuneTotalaAplicatii);

	fseek(stdin, 0, SEEK_END);
	fflush(stdin);
	char categorieCeruta[25];
	printf("\nIntroduceti numele categoriei pt care sa se afiseze valoarea totala a discount-ului");
	gets_s(categorieCeruta);

	float val = 0;
	val = calculValoareTotalaDiscountPerCategorie(val, categorieCeruta, hs);
	printf("\nValoare totala discount pe categoria de mai sus=%5.2f ", val);

	Distributie *vectorCategorii;
	vectorCategorii = new Distributie[nrAplicatii];
	int nrCategoriiDistincte = 0;

	distributieCategorii(*&vectorCategorii, &nrAplicatii,aplicatie,&nrCategoriiDistincte);
	

	calculareNrAplicatiiPerCategorie(hs, *&vectorCategorii, nrCategoriiDistincte);

	for (int i = 0; i < nrCategoriiDistincte; i++)
	{
		printf("\nCategorie=%s NrAplicatii=%d ", vectorCategorii[i].denumireCategorie, vectorCategorii[i].nrAplicatii);
	}
	
	
	char cat[25];
	printf("\nIntroduceti numele categoriei pt care sa se afiseze lista aplicatiilor ce beneficiaza de > 10%discount=");
	gets_s(cat);

	listaAplicatiiDiscountMinim(hs, cat);
	_getch();
}