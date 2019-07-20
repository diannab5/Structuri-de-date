#include <stdio.h>
#include <malloc.h>
#include <string.h>

void main() {

	char ** pStrings;

	FILE* f;
	char buffer[100];
	char m = 0; // utilizat ca intreg pe 1 bait

	f = fopen("strings.txt", "r");

	// parsare fisier pt determinare numar de linii == nr de string-uri
	fgets(buffer, sizeof(buffer), f);
	while (!feof(f)) {
		m++;

		fgets(buffer, sizeof(buffer), f);
	}

	// alocare vector de pointeri la linii
	pStrings = (char**)malloc(m * sizeof(char*));

	fseek(f, 0, SEEK_SET); // repozitionare la inceputul fisierului
	char i = 0; // index de acces la elemente vector de pointeri la linii

	fgets(buffer, sizeof(buffer), f);
	while (!feof(f)) {
		// alocare spatiu in heap seg pt string-ul preluat din fisier in buffer
		pStrings[i] = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		// copiere continut buffer in zona alocata pe linia anterioara de cod sursa
		strcpy(pStrings[i], buffer);

		// incrementare index de acces la elementele vectorului de pointeri
		i++;

		fgets(buffer, sizeof(buffer), f);
	}

	// afisare continut matrice de char ("lista" de string-uri)
	for (i = 0; i < m; i++)
		printf("%s\n", pStrings[i]);

	// dezalocare matrice
	for (i = 0; i < m; i++)
		free(pStrings[i]); // dezalocare linie i din matrice
	free(pStrings); // dezalocare vector de pointeri la linii



	fclose(f);

}