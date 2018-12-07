#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#define MAX_CHAR 1000

/* Prototyper */
void buy_product();
int list_products();

static int index = 1;   // Global int for testing purposes (Indexes the phones with a number)

/* Struct array */
typedef struct{
  

}sql_data;

static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: \n", (const char*)data);
   
   for(i = 0; i<argc; i++){
      printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }  
   
   printf("\n");
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
  }else{
    fprintf(stderr, "Opened database successfully\n");
    buy_product();
  }

  /* Print test */
  char input[MAX_CHAR];     // String for user input (What the user wants to sort by)
  char sqlone[MAX_CHAR];      // Not used
  char *sql_ori_pointer;      // Not used
  char sql_command[MAX_CHAR];   // "Blank" SQL command
  char sql_ori[MAX_CHAR] = "SELECT * FROM Mobiltelefoner;";     // SQL command combined with user input

  sqlite3_exec(db, sql_ori, callback, (void*)data, &zErrMsg);     // Executing SQL command
  fprintf(stdout, "Operation done successfully\n");         // Prints that it have been succesful




  /* Mere nyt kode */
  char tobuy[MAX_CHAR] = "ja";

  char *zSQL; 
  //zSQL = sqlite3_mprintf("INSERT INTO Mobiltelefoner (Maerke,Koebt) VALUES ('Apple', 'Ja');", tobuy);

  int id = 0;

  scanf(" %d", &id);

  char sql_test[MAX_CHAR] = "UPDATE Mobiltelefoner SET Koebt='Ja' WHERE Id='%d';";
  
  sqlite3_exec(db, sql_test, callback, (void*)data, &zErrMsg); 




  /* Lukker database */
  sqlite3_close(db);
}

void buy_product(){

  /* */
  //sqlite3_free();

  /* sqlite3_snprintf() */
  /* Kan udskrive ting */
}

int list_products(){

  return 1;
}


