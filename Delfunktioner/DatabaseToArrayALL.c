#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#define MAX_CHAR 100
#define MAX_GROUP 248

typedef struct{
	int id;
	char maerke[MAX_CHAR];
	char model[MAX_CHAR];
	char styresystem[MAX_CHAR];
	int pris;
	char farve[MAX_CHAR];
	int hukommelse;
	char stand[MAX_CHAR];
	char forsikring[MAX_CHAR];
	char nemid[MAX_CHAR];
	int postnummer;
	char dato_tilfoejet[MAX_CHAR];
	char dato_solgt[MAX_CHAR];
	char link[MAX_CHAR];
}sql_data;

void initialize_data(sql_data db_arr[]);
void print_topX(sql_data db_arr[], int amount);

int main(int argc, char* argv[]) {
	sql_data db_arr[MAX_GROUP];		// Initializes array for all entries in the database

	initialize_data(db_arr);		// Database is now loaded in struct array
	print_topX(db_arr, 10);
}

void print_topX(sql_data db_arr[], int amount) {
	for (int i = 0; i < amount; ++i)		// Prints the top 20 results
		printf("ID: %d - Firma: %s - Mobil: %s %s - Stand: %s \n NemId: %s - Forsikring: %s\n  Pris: %d\n\n"
			, db_arr[i].id
			, db_arr[i].maerke
			, db_arr[i].farve
			, db_arr[i].model
			, db_arr[i].stand
			, db_arr[i].nemid
			, db_arr[i].forsikring
			, db_arr[i].pris);
}
void initialize_data(sql_data db_arr[]) {
	sqlite3 *db;					// Pointer to database
	sqlite3_stmt *stmt;				// Sql in binary
	char sql_ori[MAX_CHAR] = "SELECT * from Mobiltelefon";	// Selects all entries in database

	sqlite3_open("Mobiltelefoner.db", &db);		// Opens database
	sqlite3_prepare_v2(db, sql_ori, -1, &stmt, NULL);
	sqlite3_step(stmt);
	
	for (int id = 0; id < MAX_GROUP; ++id) {
		db_arr[id].id = sqlite3_column_int(stmt, 0);						// Saves ID
		strcpy(db_arr[id].maerke, sqlite3_column_text(stmt, 1));			// Saves maerke
		strcpy(db_arr[id].model, sqlite3_column_text(stmt, 2));				// Saves model
		strcpy(db_arr[id].styresystem, sqlite3_column_text(stmt, 3));		// Saves styresystem
		db_arr[id].pris = sqlite3_column_int(stmt, 4);						// Saves pris
		strcpy(db_arr[id].farve, sqlite3_column_text(stmt, 5));				// Saves farve
		db_arr[id].hukommelse = sqlite3_column_int(stmt, 6);				// Saves hukommelse
		strcpy(db_arr[id].stand, sqlite3_column_text(stmt, 7));				// Saves stand
		strcpy(db_arr[id].forsikring, sqlite3_column_text(stmt, 8));		// Saves forsikring
		strcpy(db_arr[id].nemid, sqlite3_column_text(stmt, 9));				// Saves nemid
		db_arr[id].postnummer = sqlite3_column_int(stmt, 10);				// Saves postnummer
		strcpy(db_arr[id].dato_tilfoejet, sqlite3_column_text(stmt, 11));	// Saves dato_tilføjet
		strcpy(db_arr[id].dato_solgt, sqlite3_column_text(stmt, 12));		// Saves dato_solgt
		strcpy(db_arr[id].link, sqlite3_column_text(stmt, 13));				// Saves link
	
		sqlite3_step(stmt);											// Selects next entry in Database
	}

	sqlite3_finalize(stmt);		// Idk, it may close something?
	sqlite3_close(db);			// Closes the database
}