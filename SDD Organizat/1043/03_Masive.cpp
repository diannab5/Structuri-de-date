#include <stdio.h>
#include <malloc.h>
#include <string.h>

void main() {
	/* char sMat[5][5];

	strcpy(sMat[0], "Ionescu P");
	strcpy(sMat[1], "Marin G");
	strcpy(sMat[2], "Georgescu A");

	for (char i = 0; i < 3; i++)
		printf("%s\n", sMat[i]);

	printf("\n\n"); */ 

	char ** pStrings;
	FILE * f;
	char buffer[100];
	char m = 0;

	f = fopen("strings.txt", "r");

	fgets(buffer, sizeof(buffer), f);
	while (!feof(f)) {
		m ++;

		fgets(buffer, sizeof(buffer), f);
	}

	// alocare vector de pointeri la linii
	pStrings = (char**)malloc(m * sizeof(char*));

	fseek(f, 0, SEEK_SET); // repozitionare cursor de parsare pe inceputul de fisier

	int i = 0; // index utilizat in parsarea vectorului de pointeri la linii

	fgets(buffer, sizeof(buffer), f); // citire linie 1 din fisier
	while (!feof(f)) {
		// alocare heap seg pt linia i
		pStrings[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		// copiere continut buffer (string) in zona nou alocata
		strcpy(pStrings[i], buffer);

		i++; // incrementare index acces la urmatorul pointer in vectorul de adrese de linii

		fgets(buffer, sizeof(buffer), f); // citire linie k din fisier
	}

	// afisare lista de string-uri stocate in matrice (pStrings)
	for (i = 0; i < m; i++)
		printf("%s\n", pStrings[i]);

	// dezalocare matrice (inversa alocarii)
	for (i = 0; i < m; i++)
		free(pStrings[i]); // dezalocare linie i
	free(pStrings); // dezalocare vector de pointeri la linii

	
	fclose(f);

}