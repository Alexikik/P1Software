/* 
	Gruppe: A317a
	Studieretning: Software
	Opdateret: 16-12-2018
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> 	// Used for sleeping
#include "sqlite3.h"
#define MAX_CHAR 100
#define MAX_LONG_CHAR 300
#define MAX_GROUP 248

/* Struct array */
typedef struct{
	int id;
	char maerke[MAX_CHAR];
	char model[MAX_CHAR];
	char os[MAX_CHAR];
	int pris;
	char farve[MAX_CHAR];
	int mem;
	char stand[MAX_CHAR];
	char forsikring[MAX_CHAR];
	char nemid[MAX_CHAR];
	char postnr[MAX_CHAR];
	char date_added[MAX_CHAR];
	char date_sold[MAX_CHAR];
	char link[MAX_CHAR];
}sql;
struct user_typ {
	int group;
	int postnum;
};
typedef struct{
	char maerke[MAX_CHAR];
	char model[MAX_CHAR];
	char os[MAX_CHAR];
	int max_pris;
	char farve[MAX_CHAR];
	int hukommelse;
	char min_stand[MAX_CHAR];
	char forsikring[MAX_CHAR];
	char nemid[MAX_CHAR];
	char postnr[MAX_CHAR];
	char link[MAX_CHAR];
}filter_typ;

/* Prototype af funktioner */
void genbrugsvare(struct user_typ user, sqlite3 *db);
static int callback(void *data, int argc, char **argv, char **azColName);
int buy_item(sqlite3 *db, int id);
void change_filters(filter_typ *filters_arr);
void filter_to_sql(filter_typ *filters_arr, char *filter_str, struct user_typ user);
void initial_filters_by_group(filter_typ *filters_arr, struct user_typ user);
void min_stand (char *sql_temp, filter_typ filters_arr[]);
int go_to_item(sql* sql_group, int len, int inputId, sqlite3 *db, sqlite3_stmt *stmt, char *sql_ori);
void initialize_data(sqlite3* db, sqlite3_stmt *stmt, sql db_arr[], char *sql_ori, int len);
void print_topX(sql db_arr[], int amount);
/* Funktioner til login */
struct user_typ ifcreateaccount(sqlite3 *db);
struct user_typ iflogin(sqlite3 *db);
int giveValue();	// Delete me
struct user_typ give_group();
int answer_to_points();
struct user_typ login_system(sqlite3 *db);
struct user_typ get_group(sqlite3 *db, char password[], char username[]);


int main(int argc, char* argv[]) {
	sqlite3 *db;
	int rc;               	// len er hvor mange der skal gemmes i vores struct
	struct user_typ user;

	rc = sqlite3_open("Mobiltelefoner.db", &db);    	/* Åbner databasen */
	if(rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}else
		fprintf(stderr, "Opened database successfully\n");

	user = login_system(db);		// Logs in the user and retrieves his/her critical value
	printf("Din gruppe og postnum er: %d og %d\n", user.group, user.postnum);
	genbrugsvare(user, db);		// Genbrugssammenligner

	sqlite3_close(db);						// Closes the database
}

/* Funktioner til genbrugssammenligner */
void genbrugsvare(struct user_typ user, sqlite3 *db) {
	sqlite3_stmt *stmt;             /* Erklærer database handle */
	sql sql_group[MAX_GROUP];       /* Erklærer struct til opbevaring af data */
	int len = 6;               // len er hvor mange der skal gemmes i vores struct
	char sql_ori[MAX_LONG_CHAR], filter_str[MAX_LONG_CHAR];
	filter_typ filters_arr;
	filter_typ *filters_ptr = &filters_arr;
	
	initial_filters_by_group(filters_ptr, user);
	change_filters(filters_ptr);					// Changes the filters
	filter_to_sql(filters_ptr, filter_str, user);	// Makes sql command with filters
	sprintf(sql_ori, "%s ORDER BY Pris", filter_str);	// Afslutter sql commanden, så den nu kan sorteres efter
	initialize_data(db, stmt, sql_group, sql_ori, len);     /* Ligger tingene over i en struct. */
	 
	/* Indlæser hvad brugeren har gjort efter go_to_item  */
	int inputId = 0;
	int inputRes;

	do{ 
		print_topX(sql_group, len);             // Printer tingene
		printf("\nWrite an ID to see more details about the product:\n (Type [0] to change filters or [-1] to exit)\n");
		inputRes = scanf(" %d", &inputId);

		if (inputId == -1)
			printf("Goodbye!\n");
		else if (inputId == 0) {
			change_filters(filters_ptr);					// Changes the filters
			filter_to_sql(filters_ptr, filter_str, user);	// Makes sql command with filters
			sprintf(sql_ori, "%s ORDER BY Pris", filter_str);	// Afslutter sql commanden, så den nu kan sorteres efter
			initialize_data(db, stmt, sql_group, sql_ori, len);
		}
		else if(inputRes == 1)
			go_to_item(sql_group, len, inputId, db, stmt, sql_ori);
		else {
			printf("You did not enter a number, program closes!\n");
			exit(0);
		}
	} while(inputId != -1);
}
static int callback(void *data, int argc, char **argv, char **azColName){
	int i;
	// fprintf(stderr, "%s: \n", (const char*)data);
	
	for(i = 0; i<argc; i++)
		printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	 
	printf("\n");
	return 0;
}
/* Funktionen markerer data som 'købt' i databasen med en dato */
int buy_item(sqlite3 *db, int idx){
	int rc;
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[320];

	/* Til tid - http://www.cplusplus.com/reference/ctime/strftime/ */
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(buffer,60,"%d-%m-%Y",timeinfo);

	/* SQL statement */
	char sql_test[MAX_CHAR];
	sprintf(sql_test, "UPDATE Mobiltelefon SET Dato_Solgt='%s' WHERE ID=%d", buffer, idx);

	/* Execute sql statement */    
	rc = sqlite3_exec(db, sql_test, callback, NULL, NULL);

	if(rc)
		return 0;	// Buy item wasn't succesful
	else
		return 1; 	// Buy item was succesful
}
void change_filters(filter_typ *filters_arr) {
	char answer[MAX_CHAR];
	int user_input, answer_int;
	do{
		printf("Skriv tallet ud fra det filter du vil tilfoeje:\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n  %s\n (Tast [0] for at gemme) \n-> "
		, "[1]  Maerke"
		, "[2]  Model"
		, "[3]  Styresystem"
		, "[4]  Maksimums pris"
		, "[5]  Farve"
		, "[6]  Hukommelse"
		, "[7]  Minimum stand"
		, "[8]  Forsikring"
		, "[9]  NemID"
		, "[10] Lokation"
		, "[11] Hjemmeside");
		scanf(" %d", &user_input);

		if (user_input == 1){
			printf("Hvilket firma oensker du at se resultater fra? \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->maerke, answer);
		}
		else if (user_input == 2) {
			printf("Hvilken model oensker du at se resultater fra? \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->model, answer);
		}
		else if (user_input == 3) {
			printf("Hvilket styresystem oensker du at se resultater for? (Android eller iOS) \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->os, answer);
		}
		else if (user_input == 4) {
			printf("Hvilken maksimums pris oensker du at varen må have? \n");
			printf(" (Skriv 0 for at nulstille filteret)\n-> ");
			scanf(" %d", &answer_int);
			filters_arr->max_pris = answer_int;
		}
		else if (user_input == 5) {
			printf("Hvilken farve oensker du at se produkter med? \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->farve, answer);
		}
		else if (user_input == 6) {
			printf("Hvor stor skal hukommelsen vaere? (i gb) \n");
			printf(" (Skriv 0 for at nulstille filteret)\n-> ");
			scanf(" %d", &answer_int);
			filters_arr->hukommelse = answer_int;
		}
		else if (user_input == 7) {
			printf("Hvilken minimim stand skal produktet vaere i? (Daarlig, Rimelig, God eller Perfekt) \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->min_stand, answer);
		}
		else if (user_input == 8) {
			printf("Skal produktet vaere forsikret? (Ja/Nej) \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->forsikring, answer);
		}
		else if (user_input == 9) {
			printf("Skal produktet vaere NemID verificeret? (Ja/Nej/Eller) \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->nemid, answer);
		}
		else if (user_input == 10) {
			printf("Skal saelger af produktet befinde sig i samme region? (Ja/Nej/Eller) \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->postnr, answer);
		}
		else if (user_input == 11) {
			printf("Hvilken hjemmeside vil du se resultater fra? (Facebook, Dba, Guloggratis) \n");
			printf(" (Skriv x for at nulstille filteret)\n-> ");
			scanf(" %[^\n]", answer);
			strcpy(filters_arr->link, answer);
		}
		else if (user_input == 0)
			printf("Dine filtre er blevet opdateret\n\n");
		else
			printf("Indtast venligst et tal mellem 0 og 11\n");
	} while (user_input != 0);
}
void filter_to_sql(filter_typ *filters_arr, char *filter_str, struct user_typ user) {
	strcpy(filter_str, "SELECT * from Mobiltelefon WHERE Dato_Solgt='NULL'");
	char sql_temp[MAX_CHAR];

	/* Makes the filter string by the filter array */
	if (strcmp(filters_arr->maerke, "") != 0 && strcmp(filters_arr->maerke, "x") != 0){
		sprintf(sql_temp, " AND Maerke='%s'", filters_arr->maerke);
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->model, "") != 0 && strcmp(filters_arr->model, "x") != 0){
		sprintf(sql_temp, " AND Model='%s'", filters_arr->model);
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->os, "") != 0 && strcmp(filters_arr->os, "x") != 0){
		sprintf(sql_temp, " AND Styresystem='%s'", filters_arr->os);
		strcat(filter_str, sql_temp);
	}
	if (filters_arr->max_pris != 0){
		sprintf(sql_temp, " AND Pris<%d", filters_arr->max_pris);
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->farve, "") != 0 && strcmp(filters_arr->farve, "x") != 0){
		sprintf(sql_temp, " AND Farve='%s'", filters_arr->farve);
		strcat(filter_str, sql_temp);
	}
	if (filters_arr->hukommelse != 0){
		sprintf(sql_temp, " AND Hukommelse=%d", filters_arr->hukommelse);
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->min_stand, "") != 0 && strcmp(filters_arr->min_stand, "x") != 0){
		min_stand(sql_temp, filters_arr);	// Function to set minimim Stand
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->forsikring, "") != 0 && strcmp(filters_arr->forsikring, "x") != 0 && strcmp(filters_arr->forsikring, "Nej") != 0){
		sprintf(sql_temp, " AND Forsikring='%s'", filters_arr->forsikring);
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->nemid, "") != 0 && strcmp(filters_arr->nemid, "x") != 0 && strcmp(filters_arr->nemid, "Nej") != 0){
		if (strcmp(filters_arr->nemid, "Eller") == 0)
			sprintf(sql_temp, " AND (Nemid='Ja' OR Postnummer LIKE '%d%%')", user.postnum/1000);
		else
			sprintf(sql_temp, " AND Nemid='%s'", filters_arr->nemid);
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->postnr, "") != 0 && strcmp(filters_arr->postnr, "x") != 0 && strcmp(filters_arr->postnr, "Nej") != 0){
		if (strcmp(filters_arr->postnr, "Eller") == 0) {
			if (strcmp(filters_arr->nemid, "Eller") != 0)
				sprintf(sql_temp, " AND (Nemid='Ja' OR Postnummer LIKE '%d%%')", user.postnum/1000);
			else
				strcpy(sql_temp, "");
		}
		else
			sprintf(sql_temp, " AND Postnummer LIKE '%d%%'", user.postnum/1000);
		strcat(filter_str, sql_temp);
	}
	if (strcmp(filters_arr->link, "") != 0 && strcmp(filters_arr->link, "x") != 0){
		if (strcmp(filters_arr->link, "Facebook") == 0)
			strcpy(filters_arr->link, "facebook.dk");
		else if (strcmp(filters_arr->link, "Dba") == 0)
			strcpy(filters_arr->link, "dba.dk");
		else if (strcmp(filters_arr->link, "Guloggratis") == 0)
			strcpy(filters_arr->link, "Guloggratis.dk");
		sprintf(sql_temp, " AND Link='%s'", filters_arr->link);
		strcat(filter_str, sql_temp);
	}
}
void min_stand (char *sql_temp, filter_typ filters_arr[]) {
	if (strcmp(filters_arr->min_stand, "Perfekt") == 0)
		sprintf(sql_temp, " AND Stand='Perfekt'", filters_arr->min_stand);
	else if (strcmp(filters_arr->min_stand, "God") == 0)
		sprintf(sql_temp, " AND (Stand='Perfekt' OR Stand='God')", filters_arr->min_stand);
	else if (strcmp(filters_arr->min_stand, "Rimelig") == 0)
		sprintf(sql_temp, " AND (Stand='Perfekt' OR Stand='God' OR Stand='Rimelig')", filters_arr->min_stand);
	else if (strcmp(filters_arr->min_stand, "Daarlig") == 0)
		sprintf(sql_temp, " AND (Stand='Perfekt' OR Stand='God' OR Stand='Rimelig' OR Stand='Daarlig')", filters_arr->min_stand);
}
void initial_filters_by_group(filter_typ *filters_arr, struct user_typ user) {
	// No filters set for group 0 (neutral)
	if (user.group == 1)			// Group 1
		strcpy(filters_arr->postnr, "Ja");
	else if (user.group == 2)		// Group 2
		strcpy(filters_arr->min_stand, "God");
	else if (user.group == 3) {		// Group 3
		strcpy(filters_arr->nemid, "Eller");
		strcpy(filters_arr->forsikring, "Ja");
	}
	else if (user.group == 12) {		// Group 12
		strcpy(filters_arr->postnr, "Ja");
		strcpy(filters_arr->min_stand, "God");
	}
	else if (user.group == 13) {		// Group 13
		strcpy(filters_arr->nemid, "Ja");
		strcpy(filters_arr->forsikring, "Ja");
		strcpy(filters_arr->postnr, "Ja");
	}
	else if (user.group == 23) {		// Group 23
		strcpy(filters_arr->nemid, "Eller");
		strcpy(filters_arr->forsikring, "Ja");
		strcpy(filters_arr->min_stand, "God");
	}
	else if (user.group == 123) {		// Group 123
		strcpy(filters_arr->nemid, "Ja");
		strcpy(filters_arr->forsikring, "Ja");
		strcpy(filters_arr->postnr, "Ja");
		strcpy(filters_arr->min_stand, "God");
	}
}
/* Vælger item og man har tre muligheder 1) ingenting 2) gå til siden 3) køb produktet */
int go_to_item(sql* sql_group, int len, int inputId, sqlite3 *db, sqlite3_stmt *stmt, char *sql_ori){
	int buy_item_status;
	/* Printer information om varen */
	printf("Details about #%d:\n ID: %d\n Maerke: %s\n Model: %s\n OS: %s\n Pris: %dkr\n Farve: %s\n Memory: %dgb\n Stand: %s\n Forsikring: %s\n NemID verificeret: %s\n Postnummer: %s\n Dato added: %s\n Link: %s \n\n"
		, inputId
		, sql_group[inputId].id
		, sql_group[inputId].maerke
		, sql_group[inputId].model
		, sql_group[inputId].os
		, sql_group[inputId].pris
		, sql_group[inputId].farve
		, sql_group[inputId].mem
		, sql_group[inputId].stand
		, sql_group[inputId].forsikring
		, sql_group[inputId].nemid
		, sql_group[inputId].postnr
		, sql_group[inputId].date_added
		, sql_group[inputId].link);

	/* Prompter brugeren for at købe, gå til link eller gå tilbage */
	printf("Type 1 to return\nType 2 to visit page\nType 3 to buy\n-> ");
	int select;
	scanf(" %d", &select);

	if(select == 1){
		return 1;
	}else if(select == 2){
		printf("\n\n\n-----------Redirecting to %s...\n\n\n", sql_group[inputId].link);
		return 2;
	}else if(select == 3){
		buy_item_status = buy_item(db, sql_group[inputId].id);
		if (buy_item_status == 0)
			printf("FAILED at function: buy_item\n");
		else {
			printf("Item #%d purchased! [id=%d]\n", inputId, sql_group[inputId].id);
			initialize_data(db, stmt, sql_group, sql_ori, len); 	// Reinitializes struct, without the bought item  
			sleep(3);
		}
		return 3;
	}
	return 0;
}
void initialize_data(sqlite3* db, sqlite3_stmt *stmt, sql db_arr[], char *sql_ori, int len) {
	int ret;

	printf("TESTING: sql_cmd = %s\n", sql_ori);		// Prints the SQL command for testing purposes

	sqlite3_prepare_v2(db, sql_ori, -1, &stmt, NULL);
	ret = sqlite3_step(stmt);
	
	int id = 1;

	for (id = 1; id < len; ++id) {
		if (ret == 101) {
			for (int i = id; i < len; ++i)
				db_arr[i].pris = 0;                      // Saves pris
			break;
		}
		else {
			db_arr[id].id = sqlite3_column_int(stmt, 0);                        // Saves ID
			strcpy(db_arr[id].maerke, sqlite3_column_text(stmt, 1));            // Saves maerke
			strcpy(db_arr[id].model, sqlite3_column_text(stmt, 2));             // Saves model
			strcpy(db_arr[id].os, sqlite3_column_text(stmt, 3));                // Saves styresystem
			db_arr[id].pris = sqlite3_column_int(stmt, 4);                      // Saves pris
			strcpy(db_arr[id].farve, sqlite3_column_text(stmt, 5));             // Saves farve
			db_arr[id].mem = sqlite3_column_int(stmt, 6);                       // Saves hukommelse
			strcpy(db_arr[id].stand, sqlite3_column_text(stmt, 7));             // Saves stand
			strcpy(db_arr[id].forsikring, sqlite3_column_text(stmt, 8));        // Saves forsikring
			strcpy(db_arr[id].nemid, sqlite3_column_text(stmt, 9));             // Saves nemid
			strcpy(db_arr[id].postnr, sqlite3_column_text(stmt, 10));           // Saves postnummer
			strcpy(db_arr[id].date_added, sqlite3_column_text(stmt, 11));       // Saves dato_tilføjet
			strcpy(db_arr[id].date_sold, sqlite3_column_text(stmt, 12));        // Saves dato_solgt
			strcpy(db_arr[id].link, sqlite3_column_text(stmt, 13));             // Saves link
		
			/* Tester kun for de len første */
			if(id >= len)
				break;
		}
		/* Stepper til den næste*/
		ret = sqlite3_step(stmt);
	}
}
void print_topX(sql db_arr[], int amount) {
	for (int i = 1; i < amount; ++i)        // Prints the top 20 results
		if (db_arr[i].pris == 0)
			printf("");
		else 
			printf("[%d] ID: %d - Firma: %s - Mobil: %s %s - Stand: %s \n  NemId: %s - Forsikring: %s\n  Pris: %dkr\n"
				, i
				, db_arr[i].id
				, db_arr[i].maerke
				, db_arr[i].farve
				, db_arr[i].model
				, db_arr[i].stand
				, db_arr[i].nemid
				, db_arr[i].forsikring
				, db_arr[i].pris);
}

/* Funktioner til login */
struct user_typ login_system(sqlite3 *db) {
	int value = 0, status = 0;
	char scan_answer[MAX_CHAR];
	struct user_typ user;
	
	printf("1. Login    2. Create an account \n-> ");
	while (status == 0) {
		scanf(" %s", scan_answer);
		if (strcmp(scan_answer, "1") == 0) {
			user = iflogin(db);              // Starts login process
			status = 1;                               // Ups by one to exit while loop
		}
		else if (strcmp(scan_answer, "2") == 0) {   
			user = ifcreateaccount(db);             // Starts account create process
			status = 1;                               // Ups by one to exit while loop
		}
		else                                        // If the user input was unexpected
			printf("Type 1 to login, 2 to create an account\n");
	}
	return user;
}
struct user_typ ifcreateaccount(sqlite3 *db) {
	int user_exists = 0, exit;
	char username[20], password[20], sql_cmd[100];
	char *messageError = "Something went wrong";
	sqlite3_stmt *selectstmt;
	struct user_typ user;

	while (user_exists == 0) {
		printf("Type a Username\n-> ");
		scanf(" %s", username);
		printf("Type a Password\n-> ");
		scanf(" %s", password);

		sprintf(sql_cmd, "SELECT * FROM User WHERE Password LIKE '%s' AND Username LIKE '%s'", password, username);
		sqlite3_prepare_v2(db, sql_cmd, -1, &selectstmt, NULL);        // Checks if the password is already used

		if (sqlite3_step(selectstmt) == SQLITE_ROW)     // If the password already exists
			printf("That username or password already exists. Type another one\n");
		else {                                          // If it doesn't
			user_exists = 1;
			user = give_group();
			printf("Din gruppe er : %d ----- post: %d \n", user.group, user.postnum);
			sprintf(sql_cmd, "INSERT INTO User VALUES(null, '%s', '%s', %d, %d)", username, password, user.group, user.postnum);
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
	return user;
}
struct user_typ iflogin(sqlite3 *db) {
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
			printf("Velkommen %s\n", username);
			sqlite3_finalize(selectstmt);
			return get_group(db, password, username);
		}
		else {
			printf("Du er ikke oprettet i systemet. Tastede du forkert?\n Tast 1 for at oprette en ny konto, tast 0 for at proeve igen\n");
			scanf(" %s", new_account);
			printf("\n");
			if (strcmp(new_account, "1") == 0) {
				sqlite3_finalize(selectstmt);
				return ifcreateaccount(db);
			}
		}
	}
}
int giveValue() {		// Delete me
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
struct user_typ give_group() {
	int group, postnum, spg1, spg2, spg3;
	struct user_typ user;

	/*Spørgsmål 1*/
	printf("%s%s%s%s%s", "Du faar nogle spoergsmaal som vil hjaelpe programmet med at finde de bedste produkter for dig\n",
					" Hvor vigtigt er det at du kan besoege saelger naar du koeber genbrugte varer?\n",
					"  1. Ligegyldigt\n",
					"  2. Lille betydning\n",
					"  3. Stor betydning\n-> ");
	spg1 = answer_to_points();

	/*Spørgsmål 2*/
	printf("%s%s%s%s", "\n\nHvor vigtig er standen af varen, naar du koeber genbrug?\n",
					"  1. Ligegyldigt\n",
					"  2. Lille betydning\n",
					"  3. Stor betydning\n-> ");
	spg2 = answer_to_points();

	/*Spørgsmål 3*/
	printf("%s%s%s%s", "\n\nHvor vigtig er sikkerhed for dig, naar du koeber genbrugte varer?\n",
					"  1. Ligegyldigt\n",
					"  2. Lille betydning\n",
					"  3. Stor betydning\n-> ");
	spg3 = answer_to_points();
	
	printf("Hvad er dit postnummer?\n-> ");
	scanf(" %d", &postnum);

	if (spg1 > spg2 && spg1 > spg3)				// If question 1 got the greatest rating
		group = 1;		// Group 1
	else if (spg2 > spg1 && spg2 > spg3)		// If question 2 got the greatest rating
		group = 2;		// Group 2
	else if (spg3 > spg1 && spg3 > spg2)		// If question 3 got the greatest rating
		group = 3;		// Group 3
	else if (spg1 == spg2 && spg1 == spg3 && spg1 != 3)	// If all questions got the same low rating
		group = 0;		// Group_neutral
	else if (spg1 == spg2 && spg1 > spg3)		// If 1 and 2 question got the same, and it's greater than 3
		group = 12;		// Group 1 and 2
	else if (spg1 == spg3 && spg1 > spg2)		// If 1 and 3 question got the same, and it's greater than 2
		group = 13;		// Group 1 and 3
	else if (spg2 == spg3 && spg2 > spg1)		// If 2 and 3 question got the same, and it's greater than 1
		group = 23;		// Group 2 and 3
	else if (spg1 == spg2 && spg1 == spg3 && spg1 == 3)	// All three grups
		group = 123;	// Group 1, 2 and 3
	else 									// Default for testing
		group = 99;		// Error

	user.group = group;
	user.postnum = postnum;
	return user;
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
struct user_typ get_group(sqlite3 *db, char password[], char username[]) {
	int group, postnum;
	struct user_typ user;
	sqlite3_stmt *stmt;       // Sql in binary
	char sql_state[MAX_CHAR];

	sprintf(sql_state, "SELECT * from User WHERE Password='%s' AND Username='%s'", password, username);
	sqlite3_prepare_v2(db, sql_state, -1, &stmt, NULL);
	sqlite3_step(stmt);

	user.group = sqlite3_column_int(stmt, 3);
	user.postnum = sqlite3_column_int(stmt, 4);

	return user;
}