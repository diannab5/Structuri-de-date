#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

struct Bicicleta
{
	int nrIdentificare;
	int minute;
	int idStatieParcare;
	int nrUtilizariZi;
	char* numeUtilizator;
};

Bicicleta creareBicicleta(int nrIdentificare, int minute, int idStatieParcare, int nrUtilizari,const char* numeUtilizator)
{
	Bicicleta b;
	b.nrIdentificare = nrIdentificare;
	b.minute = minute;
	b.idStatieParcare = idStatieParcare;
	b.nrUtilizariZi = nrUtilizari;
	b.numeUtilizator = (char*)malloc(sizeof(char)*(strlen(numeUtilizator) + 1));
	strcpy(b.numeUtilizator, numeUtilizator);
	return b;
}

void afisareBicicleta(Bicicleta b)
{
	printf("Bicicleta cu id-ul %d, durata de utilizare intr-o zi: %d, id-ul statiei de parcare %d, a fost utilizata de %d ori, numele utilizatorului %s\n", b.nrIdentificare, b.minute, b.idStatieParcare, b.nrUtilizariZi, b.numeUtilizator);
}

struct nod
{
	Bicicleta info;
	nod* st;
	nod* dr;
};

nod* inserareInArbore(nod* rad, Bicicleta b)
{
	if (rad)
	{
		if (rad->info.minute > b.minute)
		{
			rad->st = inserareInArbore(rad->st, b);
		}
		else
		{
			rad->dr = inserareInArbore(rad->dr, b);
		}
		return rad;
	}
	else
	{
		//cream nodul
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->st = NULL;
		nou->dr = NULL;
		nou->info = creareBicicleta(b.nrIdentificare, b.minute, b.idStatieParcare, b.nrUtilizariZi, b.numeUtilizator);
		return nou;
	}
}

void afisareArboreInordine(nod* rad)
{
	if (rad)
	{
		afisareArboreInordine(rad->st);
		afisareBicicleta(rad->info);
		afisareArboreInordine(rad->dr);
	}
}

void nrTotalDeMinute(nod* rad,int &total)
{
	if (rad)
	{
		nrTotalDeMinute(rad->st, total);
		total += rad->info.minute;
		nrTotalDeMinute(rad->dr, total);
	}
}

void calculareProfitTotal(nod* rad, int &profit) {
	if (rad) {
		calculareProfitTotal(rad->st, profit);
		profit += rad->info.minute > 10 ?( (rad->info.minute - 10) * 2)*rad->info.nrUtilizariZi : (rad->info.minute * 2)*rad->info.nrUtilizariZi;
		calculareProfitTotal(rad->dr, profit);
	}
}



void main()
{

	nod* rad = NULL;
	rad = inserareInArbore(rad, creareBicicleta(1, 40, 2, 3, "Marius Neagu"));
	rad = inserareInArbore(rad, creareBicicleta(2, 35, 3, 2, "Sara Ana"));
	rad = inserareInArbore(rad, creareBicicleta(3, 50, 2, 1, "Javier Dominguez"));
	rad = inserareInArbore(rad, creareBicicleta(4, 20, 2, 2, "Kara Imas"));
	rad = inserareInArbore(rad, creareBicicleta(5, 37, 2, 3, "Manuela Pop"));
	rad = inserareInArbore(rad, creareBicicleta(6, 55, 3, 2, "Lara Gomez"));
	rad = inserareInArbore(rad, creareBicicleta(7, 60, 3, 3, "Kiara Sparks"));
	afisareArboreInordine(rad);

	int minute = 0;
	nrTotalDeMinute(rad, minute);
	
	printf("\nNr total de minute in care biciclete au fost utilizate %d\n", minute);

	int profit = 0;
	calculareProfitTotal(rad, profit);
	printf("\nProfitul total este %d", profit);
}