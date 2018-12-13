#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> 	// Used for sleeping
#include "sqlite3.h"
#define MAX_CHAR 100
#define MAX_GROUP 248

struct user_typ {
	int group;
	int postnum;
};

struct user_typ test();

int main(void) {
	struct user_typ user = test();

	printf("hej\n");
	printf("grup: %d -- postnr: %d\n", user.group, user.postnum);


	return 0;
}

struct user_typ test() {
	struct user_typ user = {3, 9220};

	return user;
}