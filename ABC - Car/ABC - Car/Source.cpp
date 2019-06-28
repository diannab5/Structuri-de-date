#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>


struct Car
{
	int id;
	char* carMaker;
	int year;
	float price;
	int noSeats;
};

Car creareCar(int id, const char* carMaker, int year, float price, int noSeats)
{
	Car c;
	c.id = id;
	c.carMaker = (char*)malloc(sizeof(char)*(strlen(carMaker) + 1));
	strcpy(c.carMaker, carMaker);
	c.year = year;
	c.price = price;
	c.noSeats = noSeats;
	return c;
}

void afisareCar(Car c)
{
	printf("Id: %d, car maker: %s, anul: %d, pret: %5.2f, nr locuri: %d\n", c.id, c.carMaker, c.year, c.price, c.noSeats);
}

struct nod
{
	Car info;
	nod* st;
	nod* dr;
};

nod* inserareArbore(nod* rad, Car c)
{
	if (rad)
	{
		if (rad->info.id > c.id)
		{
			rad->st=inserareArbore(rad->st, c);
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
		nou->info = creareCar(c.id, c.carMaker, c.year, c.price, c.noSeats);
		nou->dr = NULL;
		nou->st = NULL;
		return nou;
	}
}

void afisareDescendingOrder(nod* rad)
{
	if (rad)
	{
		afisareDescendingOrder(rad->dr);
		afisareCar(rad->info);
		afisareDescendingOrder(rad->st);
	}
}

Car cautareCarDupaId(nod* rad, int id)
{
	Car car;
	if (rad)
	{
		if (rad->info.id == id)
		{
			car = rad->info;
			return car;
		}
		else if (rad->info.id > id)
		{
			return cautareCarDupaId(rad->st, id);
		}
		else
		{
			return cautareCarDupaId(rad->dr, id);
		}
	}
	else
	{
		return creareCar(-1, "", -1, NULL, -1);
	}
}


//functie de stergere a unei masini pe baza id-ului
nod* stergereArboreDupaId(nod* rad, int id)
{
	if (rad)
	{
		if (rad->info.id > id)
		{
			rad->st = stergereArboreDupaId(rad->st, id);
		}
		else if (rad->info.id < id)
		{
			rad->dr = stergereArboreDupaId(rad->dr, id);
		}
		else
		{
			//am gasit nodul cautat de sters
			if (rad->st == NULL) //nu are descendent in stanga
			{
				nod* temp = rad->dr; //poate sa fie null sau nu
				//dezaloc nodul curent
				free(rad->info.carMaker);
				free(rad);
				rad = temp;//refac legatura
			}
			else if (rad->dr = NULL)
			{
				nod* temp = rad->st;
				free(rad->info.carMaker);
				free(rad);
				rad = temp;
			}
			else
			{
				//cazul in care am doi descendenti
				nod* temp = rad->st; //temporar este nodul cu cheia maxima din subarborele stang
				while (temp->dr)
					temp = temp->dr; //cautarea celui mai din dreapta nod din subarborele stang
				free(rad->info.carMaker);
				//mutare date din temporar in radacina
				rad->info.carMaker = (char*)malloc(sizeof(char)*(strlen(temp->info.carMaker) + 1));
				strcpy(rad->info.carMaker, temp->info.carMaker);
				//sterg ce aveam in temporar recursiv
				rad->st = stergereArboreDupaId(rad->st, temp->info.id);
			}
		}
	}
	return rad;
}

//functie de strgere completa a arborelui
//utilizeaza parcurgerea in postordine SDR
nod* stergereABC(nod* rad)
{
	if (rad)
	{
		stergereABC(rad->st);
		stergereABC(rad->dr);
		free(rad->info.carMaker);
		free(rad);
		rad = NULL;
		return rad;
	}
}

void afisareArbroreInPostordine(nod* rad)
{
	if (rad)
	{
		afisareArbroreInPostordine(rad->st);
		afisareArbroreInPostordine(rad->dr);
		afisareCar(rad->info);
	}
}

//functie de afisare pe un nivel dat
void afisareNivel(nod* rad, int nivelDorit, int nivelCurent)
{
	if (rad)
	{
		if (nivelDorit == nivelCurent)
		{
			afisareCar(rad->info);
		}
		else
		{
			afisareNivel(rad->st, nivelDorit, nivelCurent + 1);
			afisareNivel(rad->dr, nivelDorit, nivelCurent + 1);
		}
	}
}

//functie pt determinarea nivelelor arborelui
int nivele(nod* rad)
{
	if (rad)
	{
		int nivelST = nivele(rad->st);
		int nivelDR = nivele(rad->dr);
		return 1 + (nivelST > nivelDR ? nivelST : nivelDR);
	}
	else
	{
		return 0;
	}
}
void main()
{
	nod* rad = NULL;
	rad = inserareArbore(rad, creareCar(20, "Honda", 2018, 5456.5, 4));
	rad = inserareArbore(rad, creareCar(15, "Mazda", 2017, 10000, 4));
	rad = inserareArbore(rad, creareCar(25, "Opel", 2017, 9000, 4));
	rad = inserareArbore(rad, creareCar(3, "Lamborghini", 2016, 5600, 2));
	rad = inserareArbore(rad, creareCar(16, "Bugatti", 2018, 7800, 2));
	afisareDescendingOrder(rad);

	Car c1 = cautareCarDupaId(rad, 25);
	afisareCar(c1);

	printf("\n\nArborele are: %d nivele", nivele(rad));

	printf("\n\nPe nivelul 2 se afla:");
	afisareNivel(rad, 2, 1);

	rad = stergereArboreDupaId(rad, 15);
	printf("\n\nArborele dupa stergere:");
	afisareDescendingOrder(rad);

	
	stergereABC(rad);
	printf("\n\nArborele a fost sters");

	
}