#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#define MAX_CHAR 1000

static int index = 1;		// Global int for testing purposes (Indexes the phones with a number)

static int callback(void *data, int argc, char **argv, char **azColName){
	
	if (index <= 5){
		printf("[%d] %s %s med %sgb i %s stand \nFor: %skr\n"
			, index++
			, argv[0]		// Mærke
			, argv[1]		// Model
			, argv[5]		// Hukommelse
			, argv[6]		// Stand
			, argv[3]);		// Pris

		printf("\n");		// New line between different phone listings
	}
	return 0;
}

int main(int argc, char* argv[]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char* data = "Callback function called";

	/* Open database */
	rc = sqlite3_open("Mobiltelefoner.db", &db);
	
	if(rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return 0;
	} 
	else
		fprintf(stderr, "Opened database successfully\n");

	


	/* EGEN KODE */
	char input[MAX_CHAR];			// String for user input (What the user wants to sort by)
	char sqlone[MAX_CHAR];			// Not used
	char *sql_ori_pointer;			// Not used
	char sql_command[MAX_CHAR];		// "Blank" SQL command
	char sql_ori[MAX_CHAR];			// SQL command combined with user input

	printf("Hvad vil du sortere efter?\n");	// Tells the user what to write
	scanf(" %s", input);					// Reads user input
	// strcpy(input, "pris");				// User input, written manually for testing purposes
	
	sprintf(sql_ori, "SELECT * from Mobiltelefon ORDER BY %s", input);	// Combines SQL command with user input


	printf("\nCurrently doing: %s\n", sql_ori);		// Prints what it's sorting by

	sqlite3_exec(db, sql_ori, callback, (void*)data, &zErrMsg);			// Executing SQL command
	fprintf(stdout, "Operation done successfully\n");					// Prints that it have been succesful

	sqlite3_close(db);		// Closes the database
	return 0;
}


