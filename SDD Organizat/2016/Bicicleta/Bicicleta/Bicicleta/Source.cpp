#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>

struct Bicicleta
{
	int nrIdentificare;
	int durataUtilizare;
	int idStatieParcare;
	int nrUtilizari;
	char *numeUtilizator;
};

struct nodABC
{
	Bicicleta info;
	nodABC *left, *right;
};



nodABC* creareNodAbc(Bicicleta bicicleta, nodABC *l, nodABC *r)
{
	nodABC *temp = (nodABC*)malloc(sizeof(nodABC));
	temp->info = bicicleta;
	temp->left = l;
	temp->right = r;
	return temp;
}

nodABC* inserareNodInABC(nodABC *root, Bicicleta bicicleta)
{
	nodABC *aux;
	aux = root;
	if (!root)
	{
		aux = creareNodAbc(bicicleta, NULL, NULL);
		return aux;
	}
	else
	{
		while (1)
		{
			if (bicicleta.durataUtilizare < aux->info.durataUtilizare)
			{
				if (aux->left)
					aux = aux->left;
				else
				{
					aux->left = creareNodAbc(bicicleta, NULL, NULL);
					return root;
				}
			}
			else if (bicicleta.durataUtilizare > aux->info.durataUtilizare)
			{
				if (aux->right)
						aux = aux->right;
				else
				{
					aux->right = creareNodAbc(bicicleta, NULL, NULL);
					return root;
				}				
			}

			else
			{
				return root;
			}
		}
		
	}
}

void calculareValoareIncasata(nodABC *root, int *valoareTotala)
{
	if (root)
	{
		calculareValoareIncasata(root->left, valoareTotala);
		*valoareTotala += (root->info.durataUtilizare - 10 * root->info.nrUtilizari)*2;
		calculareValoareIncasata(root->right, valoareTotala);
	}
}

void parcurgereLeftRRight(nodABC *root)
{
	if (root)
	{
		parcurgereLeftRRight(root->left);

		printf("\nId identificare=%d Durata Utilizare=%d IdStatie=%d Nr utilizari=%d Nume Utilizator=%s ", root->info.nrIdentificare, root->info.durataUtilizare, root->info.idStatieParcare, root->info.nrUtilizari, root->info.numeUtilizator);
		parcurgereLeftRRight(root->right);
	}
}

void calculareNumarTotalMinute(int *nrTotal, nodABC *root)
{
	if (root)
	{
		calculareNumarTotalMinute(nrTotal,root->left);
		*nrTotal += root->info.durataUtilizare;
		calculareNumarTotalMinute(nrTotal, root->right);
	}
}


void calculareNrStatiiDistincte(int *vectorStatii, int nr, int *nrStatiiDistincte)
{
	for (int i = 0; i < nr; i++)
	{
		for (int j = i + 1; j < nr;)
		{
			if (vectorStatii[j] == vectorStatii[i])
			{
				for (int k = j; k < nr; k++)
				{
					vectorStatii[k] = vectorStatii[k + 1];
				}
				nr--;
			}
			else
				j++;
		}
	}
	*nrStatiiDistincte = nr;
}


void main()
{
	int nrTotalMinute = 0;
	nodABC *root = NULL;
	int nrBiciclete = 0;
	Bicicleta *bicicleta=NULL;
	int valoareTotalaIncasata = 0;

	
	
	FILE *f = fopen("biciclete.txt", "r");
	char* token[120], linie[100], sep_list[] = ",";


	while (fgets(linie, sizeof(linie), f))
	{
		nrBiciclete++;
	}
	fclose(f);

	bicicleta = (Bicicleta*)malloc(nrBiciclete*sizeof(Bicicleta));
	int i = 0;
	f = fopen("biciclete.txt", "r");
	while (fgets(linie, sizeof(linie), f))
	{
		token[0] = strtok(linie, sep_list);
		for (int i = 1; i < 5; i++)
		{
			token[i] = strtok(NULL, sep_list);
		}
		
		bicicleta[i].nrIdentificare = atoi(token[0]);
		bicicleta[i].durataUtilizare = atoi(token[1]);
		bicicleta[i].idStatieParcare = atoi(token[2]);
		bicicleta[i].nrUtilizari = atoi(token[3]);
		bicicleta[i].numeUtilizator = (char*)malloc((strlen(token[4]) + 1)*sizeof(char));
		strcpy(bicicleta[i].numeUtilizator, token[4]);
		root = inserareNodInABC(root, bicicleta[i]);

		i++;
		
	}
	fclose(f);

	parcurgereLeftRRight(root);
	calculareNumarTotalMinute(&nrTotalMinute, root);
	printf("\nNumarul total de minute pe zi este=%d", nrTotalMinute);

	calculareValoareIncasata(root,&valoareTotalaIncasata);
	printf("\nValoarea incasata totala este=%d", valoareTotalaIncasata);

	int *vectorStatiiParcare = (int*)malloc(nrBiciclete*sizeof(int));
	for (int i = 0; i < nrBiciclete; i++)
	{
		vectorStatiiParcare[i] = bicicleta[i].idStatieParcare;
	}
	int nrStatiiDistincte = 0;

	calculareNrStatiiDistincte(vectorStatiiParcare, nrBiciclete, &nrStatiiDistincte);
	printf("\nNr statii distincte=%d", nrStatiiDistincte);
	_getch();
}