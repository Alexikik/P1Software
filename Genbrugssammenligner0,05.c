/* 
    Gruppe: A317a
    Studieretning: Software
    Opdateret: 09-12-2018
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#define MAX_CHAR 100
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
    int postnr;
    char date_added[MAX_CHAR];
    char date_sold[MAX_CHAR];
    char link[MAX_CHAR];
}sql;

/* Prototyper */
static int callback(void *data, int argc, char **argv, char **azColName);
int buy_item(sqlite3 *db, int id, void *data, char *zErrMsg);
int select_genbrugs_type(char *genbrugstype, char *sql_ori);
int go_to_item(sql* sql_group, int len, int inputId);
// void list_struct(sql* sql_group, int len);
// int convert_struct(sql sql_group[], int len, sqlite3 *db, int index, void *data, char *zErrMsg, sqlite3_stmt *stmt, char *sql_ori);
void initialize_data(sql db_arr[], char *sql_ori);         // Alternativ
void print_topX(sql db_arr[], int amount);  // Alternativ

int main(int argc, char* argv[]) {
    sqlite3 *db;
    int len = 5;               // len er hvor mange der skal gemmes i vores struct
    char sql_ori[MAX_CHAR];
    char *zErrMsg = 0;
    const char* data = "Callback function called";
    sqlite3_stmt *stmt;             /* Erklærer database handle */
    sql sql_group[MAX_GROUP];       /* Erklærer struct til opbevaring af data */

    /* Lav en funktion til dette for genbrugstype for hvad der skal lægges over i struct -  
    WHERE Dato_Solgt IS NULL sorterer alle solgte fra databasen */
    /* Jakobs kode kommer til at styre denne her funktion */
    select_genbrugs_type("kritisk", sql_ori);
    initialize_data(sql_group, sql_ori);        // Ligger tingene over i en struct.

    /* Indlæser hvad brugeren har gjort efter go_to_item  */
    int go_to_res = 0;
    int inputId = 0;
    int inputRes;

    do{
        print_topX(sql_group, len);             // Printer tingene
        printf("\nWrite an ID to see more details about the product:\n (Type -1 to exit)\n");
        inputRes = scanf(" %d", &inputId);

        if (inputId == -1)
            printf("Goodbye!\n");
        else if(inputRes == 1) {
            go_to_res = go_to_item(sql_group, len, inputId-1);
            if(go_to_res == 3)      /* Hvis brugeren har indtastet 3 så købes produktet */
                buy_item(db, sql_group[inputId].id, (void*)data, zErrMsg);
        }
        else {
            printf("You did not enter a number, program closes!\n");
            exit(0);
        }
    } while(inputId != -1);
}

static int callback(void *data, int argc, char **argv, char **azColName){
     int i;
     fprintf(stderr, "%s: \n", (const char*)data);
     
     for(i = 0; i<argc; i++){
            printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
     }  
     
     printf("\n");
     return 0;
}
/* Funktionen markerer data som 'købt' i databasen med en dato */
int buy_item(sqlite3 *db, int idx, void *data, char *zErrMsg){
    int rc;

    char sql_test[MAX_CHAR];
    sprintf(sql_test, "UPDATE Mobiltelefon SET Dato_Solgt='09-12-2018' WHERE ID=%d;", idx);

    
    rc = sqlite3_exec(db, sql_test, callback, (void*)data, &zErrMsg);

    if(rc){
        printf("FAILED at function: buy_item");
        return 1;
    }else{
        return 0;
    }
}
/* Funktion til at vælge genbrugstype */
int select_genbrugs_type(char *genbrugstype, char *sql_ori){
    /* Switch statement virker ikke til det her ;)  */
    if (strcmp(genbrugstype, "kritisk") == 0) 
    {
        /* Pris */
        sprintf(sql_ori, "SELECT * from Mobiltelefon WHERE Dato_Solgt='NULL' ORDER BY %s", "Pris");
    } 
    else if (strcmp(genbrugstype, "social") == 0)
    {
        /* Sorterer med postnummer. Postnummer skal have en funktion der kan beregne distance mellem bruger og postnummer */
        sprintf(sql_ori, "SELECT * from Mobiltelefon WHERE Dato_Solgt='NULL' ORDER BY %s", "Postnummer");
    }
    /* more else if clauses */
    else /* default: */
    {
        printf("genbrugstype was not defined properly. Exitting program!");
        exit(0);
    }
    return 0;
}
/* Vælger item og man har tre muligheder 1) ingenting 2) gå til siden 3) køb produktet */
int go_to_item(sql* sql_group, int len, int inputId){
    /* Printer information om varen */
    printf("Details about #%d:\n ID: %d\n Maerke: %s\n Model: %s\n OS: %s\n Pris: %d\n Farve: %s\n Memory: %d\n Stand: %s\n Forsikring: %s\n NemID verificeret: %s\n Postnummer: %d\n Dato added: %s\n Link: %s \n\n"
        , inputId+1
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
    printf("Type 1 to return\nType 2 to visit page\nType 3 to buy\n->");
    int select;
    scanf(" %d", &select);

    if(select == 1){
        return 1;
    }else if(select == 2){
        printf("\n\n\n-----------Redirecting to %s...\n\n\n", sql_group[inputId].link);
        return 2;
    }else if(select == 3){
        printf("Item #%d purchased!\n", inputId);
        return 3;
    }
    return 0;
}
void initialize_data(sql db_arr[], char *sql_ori) {
    sqlite3 *db;                    // Pointer to database
    sqlite3_stmt *stmt;             // Sql in binary
    // char sql_ori[MAX_CHAR] = "SELECT * from Mobiltelefon";  // Selects all entries in database

    sqlite3_open("Mobiltelefoner.db", &db);     // Opens database
    sqlite3_prepare_v2(db, sql_ori, -1, &stmt, NULL);
    sqlite3_step(stmt);
    
    for (int id = 0; id < MAX_GROUP; ++id) {
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
        db_arr[id].postnr = sqlite3_column_int(stmt, 10);                   // Saves postnummer
        strcpy(db_arr[id].date_added, sqlite3_column_text(stmt, 11));       // Saves dato_tilføjet
        strcpy(db_arr[id].date_sold, sqlite3_column_text(stmt, 12));        // Saves dato_solgt
        strcpy(db_arr[id].link, sqlite3_column_text(stmt, 13));             // Saves link
    
        sqlite3_step(stmt);                                         // Selects next entry in Database
    }

    sqlite3_finalize(stmt);     // Idk, it may close something?
    sqlite3_close(db);          // Closes the database
}
void print_topX(sql db_arr[], int amount) {
    for (int i = 0; i < amount; ++i)        // Prints the top 20 results
        printf("[%d] ID: %d - Firma: %s - Mobil: %s %s - Stand: %s \n  NemId: %s - Forsikring: %s\n  Pris: %dkr\n"
            , i+1
            , db_arr[i].id
            , db_arr[i].maerke
            , db_arr[i].farve
            , db_arr[i].model
            , db_arr[i].stand
            , db_arr[i].nemid
            , db_arr[i].forsikring
            , db_arr[i].pris);
}



