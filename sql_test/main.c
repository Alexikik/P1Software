#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h" 

#define MAX_CHAR 1000

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
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("Mobiltelefoner.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "SELECT * from Mobiltelefon";

   


    /* EGEN KODE */
    sql = "SELECT from Mobiltelefon ORDER BY ASC";

    char input[MAX_CHAR];
    char sqlone[MAX_CHAR];
    char input2[MAX_CHAR];
    scanf(" %s %s", input, input2);

    sprintf(sqlone, "SELECT %s from Mobiltelefon ORDER BY %s ASC", input, input2);

    printf("\nVORES:%s", sqlone);

    sqlite3_exec(db, sqlone, callback, (void*)data, &zErrMsg);
    fprintf(stdout, "Operation done successfully\n");

   sqlite3_close(db);
   return 0;
}