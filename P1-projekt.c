//Dette er en test
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LGT 50
#define MAX_PERSONS 50

  struct person{
    char fornavn[MAX_NAME_LGT];
    char efternavn[MAX_NAME_LGT];
    char vejnavn[MAX_NAME_LGT];
    int vejnummer;
    int postnummer;
    char bynavn[MAX_NAME_LGT];
  };

typedef struct person person;

int el_cmp(const void *ep1, const void *ep2);

int main(void) {
	person gruppe[MAX_PERSONS];
	int index = 0, hej;

	FILE *input_file_pointer;
	FILE *output_file_pointer;
	input_file_pointer = fopen("adresser.txt", "r");

	if (input_file_pointer != NULL) {
		int scan_res;
		printf(" -- Input opened successfully -- \n");
 
		while ((hej = fgetc(input_file_pointer)) != EOF) {
	 		(hej == '.') ? printf("") : ungetc(hej, input_file_pointer);	// Puts "hej" back in the buffer
	 		scan_res = fscanf(input_file_pointer, " %s %[^,], %[^0123456789] %d, %d %[^.]", 
	 					gruppe[index].fornavn, gruppe[index].efternavn, gruppe[index].vejnavn, &gruppe[index].vejnummer, &gruppe[index].postnummer, gruppe[index].bynavn);
	 		index++;
 		}	
		// printf(" -- Scanned items: %d -- \n", scan_res);

		fclose(input_file_pointer);
	}
	else printf(" -- Input file could not open! -- \n");

	qsort(gruppe, index-1, sizeof(person), el_cmp);

	output_file_pointer = fopen("output.txt", "w");
	if (output_file_pointer != NULL) {
		printf(" -- Output opened successfully -- \n");
		for (int i = 0; i < index-1; ++i)
			fprintf(output_file_pointer, "%s: %s \n", gruppe[i].bynavn, gruppe[i].efternavn);
			// printf("%s: %s \n", gruppe[i].bynavn, gruppe[i].efternavn);
		fclose(output_file_pointer);
	}
	else printf(" -- Output file could not open! -- \n");
	
	return 0;
}



int el_cmp(const void *ep1, const void *ep2) {
	person *a = ((person*)ep1),
		   *b = ((person*)ep2);
	
	if (strcmp(a->efternavn, b->efternavn) < 0)
		return -1;
	else 
		return 1;
}





