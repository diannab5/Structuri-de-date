#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>



struct Depozit
{
	int cod;
	char* locatie;
	int capacitate;
	int nrCategoriiCereale;
	char** denumireCereale;
};

Depozit creareDepozit(int cod, const char* locatie, int capacitate, int nrCategoriiCereale, const char** denumiriCereale)
{
	Depozit d;
	d.cod = cod;
	d.locatie = (char*)malloc(sizeof(char)*(strlen(locatie) + 1));
	strcpy(d.locatie, locatie);
	d.capacitate = capacitate;
	d.nrCategoriiCereale = nrCategoriiCereale;
	d.denumireCereale = (char**)malloc(sizeof(char*)*d.nrCategoriiCereale);
	for (int i = 0; i < d.nrCategoriiCereale; i++)
	{
		d.denumireCereale[i] = (char*)malloc(sizeof(char)*(strlen(denumiriCereale[i]) + 1));
		strcpy(d.denumireCereale[i], denumiriCereale[i]);
	}
	return d;
}

void afisareDepozit(Depozit d)
{
	printf("\nDepozitul cu codul: %d, din locatia %s, are o capacitate de %d si %d de categorii de cereale\n", d.cod, d.locatie, d.capacitate, d.nrCategoriiCereale);
	printf(" denumiri cereale: ");
	for (int i = 0; i < d.nrCategoriiCereale; i++)
	{
		printf(" %s",d.denumireCereale[i]); 
	}
}




struct nod
{
	Depozit info;
	nod *st;
	nod *dr;
};

int calculInaltime(nod* rad)
{
	if (rad)
	{
		int inaltimeST = calculInaltime(rad->st);
		int inaltimeDR = calculInaltime(rad->dr);
		return 1 + (inaltimeST > inaltimeDR ? inaltimeST : inaltimeDR);
	}
	else
	{
		return 0;
	}
}

int calculGradEchilibru(nod* rad)
{
	if (rad)
	{
		int inaltimeST = calculInaltime(rad->st);
		int inaltimeDR = calculInaltime(rad->dr);
		return inaltimeST - inaltimeDR;
	}
	else
	{
		return 0;
	}
}

nod* rotireLaStanga(nod* rad)
{
	if (rad)
	{
		nod* temp = rad->dr;
		rad->dr = temp->st;
		temp->st = rad;
		return temp;
	}
	else
	{
		return rad;
	}
}

nod* rotireLaDreapta(nod* rad)
{
	if (rad)
	{
		nod* temp = rad->st;
		rad->st = temp->dr;
		temp->dr = rad;
		return temp;
	}
	else
	{
		return rad;
	}
}

nod* inserareInAvl(nod* rad, Depozit d)
{
	if (rad)
	{
		if (rad->info.cod > d.cod)
		{
			rad->st = inserareInAvl(rad->st, d);
		}
		else if(rad->info.cod<d.cod)
		{
			rad->dr = inserareInAvl(rad->dr, d);
		}
		//inainte sa returnez rad verific echilibrul
		if (calculGradEchilibru(rad) == 2)
		{
			if (calculGradEchilibru(rad->st) != 1)
			{
				rad->st = rotireLaStanga(rad->st);
			}
			rad = rotireLaDreapta(rad);
		}
		if (calculGradEchilibru(rad) == -2)
		{
			if (calculGradEchilibru(rad->dr) != -1)
			{
				rad->dr = rotireLaDreapta(rad->dr);
			}
			rad = rotireLaStanga(rad);
		}
		return rad;
	}
	else
	{
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->dr = NULL;
		nou->st = NULL;
		nou->info = d;
		return nou;
	}
}


void afisareInOrdine(nod* rad)
{
	if (rad)
	{
		afisareInOrdine(rad->st);
		afisareDepozit(rad->info);
		afisareInOrdine(rad->dr);
	}
}



//afisare pe nivele
void afisareNivel(nod* rad, int nivel)
{
	if (rad)
	{
		if (nivel == 0)
		{
			afisareDepozit(rad->info);
		}
		else
		{
			nivel--;
			afisareNivel(rad->st, nivel);
			afisareNivel(rad->dr, nivel);
		}
	}
}

void afisarePeNivele(nod* rad)
{
	if (rad)
	{
		int nrNivele = calculInaltime(rad);
		for (int i = 0; i < nrNivele; i++)
		{
			printf("\nNivelul %d: \n", i + 1);
			afisareNivel(rad, i);
		}
	}
}


void nrDepCuSecara(nod* rad, int *nr)
{
	if (rad)
	{
		for (int i = 0; i < rad->info.nrCategoriiCereale; i++)
		
			if (strcmp("secara", rad->info.denumireCereale[i]))
				(*nr)++;
		
		nrDepCuSecara(rad->st, nr);
		nrDepCuSecara(rad->dr, nr);
	}
}

void afisareCoduriDeLaRadacina(nod *rad, int cod)
{
	if (rad)
	{
		if (rad->info.cod == cod)
		{
			return;
		}
		if (rad->info.cod != cod)
		{
			afisareCoduriDeLaRadacina(rad->st, cod);
			printf(" %d", rad->info.cod);
			afisareCoduriDeLaRadacina(rad->dr, cod);
	}
	}
}

void adaugaCategorie(nod* rad, const char* newCategorie, int cod)
{
	if (rad)
	{
		if (rad->info.cod == cod)
		{
			rad->info.nrCategoriiCereale += 1;
			rad->info.denumireCereale[rad->info.nrCategoriiCereale - 1] = (char*)malloc(sizeof(char)*strlen(newCategorie) + 1);
			strcpy(rad->info.denumireCereale[rad->info.nrCategoriiCereale - 1], newCategorie);
		}
		adaugaCategorie(rad->st, newCategorie, cod);
		adaugaCategorie(rad->dr, newCategorie, cod);
}

}
void main()
{
	const char* depozit1[2] = { "secara","ovaz" };
	const char* depozit2[3] = { "ovaz","orz","grau" };
	const char* depozit3[2] = { "malai","orz" };
	const char* depozit4[3] = { "ovaz","rapita","porumb" };
	const char* depozit5[2] = { "orz","porumb" };
	const char* depozit6[2] = { "porumb","ovaz" };

	nod* rad = NULL;
	rad = inserareInAvl(rad, creareDepozit(25, "Bucuresti", 10, 2, depozit1));
	rad = inserareInAvl(rad, creareDepozit(15, "Iasi", 15, 3, depozit2));
	rad = inserareInAvl(rad, creareDepozit(38, "Timisoara", 4, 2, depozit3));
	rad = inserareInAvl(rad, creareDepozit(10, "Bucuresti", 19, 3, depozit4));
	rad = inserareInAvl(rad, creareDepozit(4, "Ploiesti", 20, 2, depozit5));
	
	afisareInOrdine(rad);
	
	afisarePeNivele(rad);

	int nr = 0;
	nrDepCuSecara(rad, &nr);
	printf("\n\nNr de depozite cu secara %d", nr);

	afisareCoduriDeLaRadacina(rad, 10);

	adaugaCategorie(rad, "grau", 10);
	afisareInOrdine(rad);
	
}