#include <stdio.h>
#include <stdlib.h>

void giveValue(int*);
int main(void) {
	int value = 0;
giveValue(&value);
printf("Din kritiske vaerdi er: %d ud af 9\n",value);
return 0;

}

void giveValue(int *value) {
  /*Spørgsmål 1*/
  int scan_answer;
  printf("%s%s%s%s%s", "Du faer nogle spoergsmael som vil hjaelpe programmet med at finde de bedste produkter for dig\n",
          "Er det vigtigt for dig, at din genbrugsvare er forsikret eller nemID-verificeret?\n",
          "1. Overhovedet ikke vigtigt\n",
          "2. Har en betydning, men ikke altafgørende for om jeg vil koebe produktet\n",
          "3. Jeg koeber ikke produktet hvis det ikke er forsikret eller nemID-verificeret\n");
  scanf("%d", &scan_answer);
  if (scan_answer == 1) {
    *value += 1;
  }
  else if (scan_answer == 2) {
    *value += 2;
  }
  else if (scan_answer == 3) {
    *value += 3;
  }
  printf("%s%s%s%s", "\n\nHvor vigtigt er prisen pae produktet for dig?\n",
          "1. Jeg vurdere andre kriterier som kvalitet eller sikkerhed højere end pris\n",
          "2. Prisen har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
          "3. Prisen på produktet er det vigtiste kriterie, for om jeg koeber produktet\n");
  scanf("%d", &scan_answer);
  if (scan_answer == 1) {
    *value += 3;
  }
  else if (scan_answer == 2) {
    *value += 2;
  }
  else if (scan_answer == 3) {
    *value += 1;
  }
printf("%s%s%s%s", "\n\nHvor vigtigt er kvaliteten af produktet for dig?\n",
        "1. Jeg vurdere andre kriterier som pris eller sikkerhed højere end kvalitet\n",
        "2. Kvaliteten har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
        "3. Kvaliteten er det vigtigste kriterie for, om jeg koeber produktet\n");
  scanf("%d", &scan_answer);
  if (scan_answer == 1) {
    *value += 1;
  }
  else if (scan_answer == 2) {
    *value += 2;
  }
  else if (scan_answer == 3) {
    *value += 3;
  }
}