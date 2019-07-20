#include <stdio.h>
#include <malloc.h>
#include <string.h>

void main() {
	char ** pStrings;

	FILE * f;
	char buffer[100];
	char m = 0; // numarul de linii

	f = fopen("strings.txt", "r");

	fgets(buffer, sizeof(buffer), f); // citire initiala din fisier
	while (!feof(f)) {
		m++;

		// citire linie urmatoare din fisierul f (strings.txt)
		fgets(buffer, sizeof(buffer), f);
	}

	// m - numarul de string-uri din fisier
	pStrings = (char**)malloc(m * sizeof(char*)); // pStrings[i] contin valori reziduale

	fseek(f, 0, SEEK_SET);

	int i = 0; // index parsare vector de pointeri la linii
	// re-parsare fisier text
	fgets(buffer, sizeof(buffer), f); // citire initiala din fisier
	while (!feof(f)) {
		// alocare linie i
		pStrings[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		// copiere continut buffer in zona alocata 
		strcpy(pStrings[i], buffer);

		i++; // incrementare index de parsare a vectorului de pointeri la linii

		// citire linie urmatoare din fisierul f (strings.txt)
		fgets(buffer, sizeof(buffer), f);
	}

	// afisare matrice de char alocata dinamic
	for (char j = 0; j < m; j++)
		printf("%s", pStrings[j]);

	// dezalocare matrice din heap
	for (char j = 0; j < m; j++)
		free(pStrings[j]); // pStrings[j] contine adresa de inceput a liniei j
	// dezalocare vector de pointeri la linii (dezalocare structura intermediara)
	free(pStrings);


	fclose(f);
}