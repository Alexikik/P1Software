#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h" 
#include <string.h>
#define MAX_CHAR 100

int ifcreateaccount(sqlite3 *db);
int iflogin(sqlite3 *db);
int giveValue();
int answer_to_points();
int mainFunk();
int get_value(sqlite3 *db, char password[]);

int main(void) {
	 printf("Value = %d\n", mainFunk());
	/*sqlite3 *db; */
	/*printf("test100 = %d\n", get_value(db, "Hejsadu")); */
	return 0;
}


int mainFunk() {
	sqlite3 *db;        // Pointer to database
	char *sql_cmd;      // String for sql command
	int value = 0, status = 0;
	char scan_answer[MAX_CHAR];
	
	sqlite3_open("Mobiltelefoner.db", &db);    // Opens database
	printf(" --- Opened database successfully --- \n");

	sql_cmd = "SELECT * from User";             // Sql command for selecting every line in the table "User"
	
	printf("1. Login    2. Create an account \n-> ");
	while (status == 0) {
		scanf(" %s", scan_answer);
		if (strcmp(scan_answer, "1") == 0) {
			value = iflogin(db);              // Starts login process
			status = 1;                               // Ups by one to exit while loop
		}
		else if (strcmp(scan_answer, "2") == 0) {   
			value = ifcreateaccount(db);             // Starts account create process
			status = 1;                               // Ups by one to exit while loop
		}
		else                                        // If the user input was unexpected
			printf("Type 1 to login, 2 to create an account\n");
	}
	return value;
}
int ifcreateaccount(sqlite3 *db) {
	int user_exists = 0, exit, value;
	char username[20], password[20], sql_cmd[100];
	char *messageError = "Something went wrong";
	sqlite3_stmt *selectstmt;

	while (user_exists == 0) {
		printf("Type a Username\n-> ");
		scanf(" %s", username);
		printf("Type a Password\n-> ");
		scanf(" %s", password);

		sprintf(sql_cmd, "SELECT * FROM User WHERE Password LIKE '%s' AND Username LIKE '%s'", password, username);
		sqlite3_prepare_v2(db, sql_cmd, -1, &selectstmt, NULL);        // Checks if the password is already used

		if (sqlite3_step(selectstmt) == SQLITE_ROW)		// If the password already exists
			printf("That username or password already exists. Type another one\n");
		else {											// If it doesn't
			user_exists = 1;
			value = giveValue();
			sprintf(sql_cmd, "INSERT INTO User VALUES(null, '%s', '%s', %d)", username, password, value);
			exit = sqlite3_exec(db, sql_cmd, NULL, 0, &messageError);
			if (exit != SQLITE_OK) {
				printf("Error Insert\n");
				sqlite3_free(messageError);
			}
			else
				printf("Records created successfully\nWelcome %s\n", username);   /*Get value from database into variable here!*/
		}
	}
	sqlite3_finalize(selectstmt);  
	return value;
}
int iflogin(sqlite3 *db) {
	int value;
	char new_account[MAX_CHAR];
	char username[MAX_CHAR], password[MAX_CHAR], sql_cmd[MAX_CHAR];
	sqlite3_stmt *selectstmt;

	while (1 == 1) {
		printf("Type your Username\n-> ");
		scanf(" %s", username);

		printf("Type your Password\n-> ");
		scanf(" %s", password);
		printf("\n");

		sprintf(sql_cmd, "SELECT * FROM User WHERE Password LIKE '%s' AND Username LIKE '%s'", password, username);
		sqlite3_prepare_v2(db, sql_cmd, -1, &selectstmt, NULL);

		if (sqlite3_step(selectstmt) == SQLITE_ROW) {
			printf("Velkommen %s\n", username); 									// Indsæt username
			return get_value(db, password);
		}
		else {
			printf("Du er ikke oprettet i systemet. Tastede du forkert?\n Tast 1 for at oprette en ny konto, tast 0 for at proeve igen\n");
			scanf(" %s", new_account);
			printf("\n");
			if (strcmp(new_account, "1") == 0)
				return ifcreateaccount(db);
		}
	}
	sqlite3_finalize(selectstmt);
}
int giveValue() {
	int value = 0, start_over = 0;
	char scan_answer[MAX_CHAR];

	/*Spørgsmål 1*/
	printf("%s%s%s%s%s", "Du faar nogle spoergsmaal som vil hjaelpe programmet med at finde de bedste produkter for dig\n",
					"Er det vigtigt for dig, at din genbrugsvare er forsikret eller nemID-verificeret?\n",
					"  1. Overhovedet ikke vigtigt\n",
					"  2. Har en betydning, men ikke altafgoerende for om jeg vil koebe produktet\n",
					"  3. Jeg koeber ikke produktet hvis det ikke er forsikret eller nemID-verificeret\n-> ");
	value += answer_to_points();

	/*Spørgsmål 2*/
	printf("%s%s%s%s", "\n\nHvor vigtigt er prisen paa produktet for dig?\n",
					"  1. Prisen paa produktet er det vigtiste kriterie, for om jeg koeber produktet\n",
					"  2. Prisen har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
					"  3. Jeg vurdere andre kriterier som kvalitet eller sikkerhed hoejere end pris\n-> ");
	value += answer_to_points();

	/*Spørgsmål 3*/
	printf("%s%s%s%s", "\n\nHvor vigtigt er kvaliteten af produktet for dig?\n",
				"  1. Jeg vurdere andre kriterier som pris eller sikkerhed hoejere end kvalitet\n",
				"  2. Kvaliteten har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
				"  3. Kvaliteten er det vigtigste kriterie for, om jeg koeber produktet\n-> ");
	value += answer_to_points();

	return value;
}
int answer_to_points() {
	int value = 0;
	char scan_answer[MAX_CHAR];

	while (value == 0) {
		scanf(" %s", scan_answer);
		if (strcmp(scan_answer, "1") == 0)
			value = 1;
		else if (strcmp(scan_answer, "2") == 0)
			value = 2;
		else if (strcmp(scan_answer, "3") == 0)
			value = 3;
		else
			printf("Tast venligst 1, 2 eller 3\n");
	}
	return value;
}
int get_value(sqlite3 *db, char password[]) {
	int value;
	sqlite3_stmt *stmt;       // Sql in binary
	char sql_state[MAX_CHAR];

	sprintf(sql_state, "SELECT * from User WHERE Password='%s'", password);
	sqlite3_open("Mobiltelefoner.db", &db);   // Opens database
	sqlite3_prepare_v2(db, sql_state, -1, &stmt, NULL);
	sqlite3_step(stmt);

	value = sqlite3_column_int(stmt, 3);
	return value;
}

























