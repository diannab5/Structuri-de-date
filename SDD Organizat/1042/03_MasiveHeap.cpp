#include <stdio.h>
#include <malloc.h>
#include <string.h>


void main() {
	char ** pStrings;

	FILE* f;
	char buffer[150], m = 0;

	f = fopen("strings.txt", "r");

	fgets(buffer, sizeof(buffer), f);
	while (!feof(f)) {
		m++;

		fgets(buffer, sizeof(buffer), f);
	}

	// alocare vector de pointeri la linii
	pStrings = (char**)malloc(m * sizeof(char*));

	fseek(f, 0, SEEK_SET); // repozitionare pe inceputul de fisier

	char i = 0; // index parsare vector de pointeri

	fgets(buffer, sizeof(buffer), f);
	while (!feof(f)) {
		pStrings[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(pStrings[i], buffer);
		i++;

		fgets(buffer, sizeof(buffer), f);
	}

	// afisare matrice de strings
	for (i = 0; i < m; i++)
		printf("%s\n", pStrings[i]);

	// dezalocare matrice
	for (i = 0; i < m; i++)
		free(pStrings[i]); // dezalocare linie i
	free(pStrings); // dezalocare vector de pointeri la linii

	fclose(f);

}