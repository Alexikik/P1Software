#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#define MAX_CHAR 100
#define MAX_GROUP 200

/* Prototyper */

static int index = 1;   // Global int for testing purposes (Indexes the phones with a number)

/* Struct array */
typedef struct{
  
  char maerke[MAX_CHAR];
  int pris;

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
  }

  /* Dette kan bruges til at 'slette' fra databasen */
  //int scanid = 0;
  //scanf(" %d", &scanid);
  //char sql_test[MAX_CHAR];
  //sprintf(sql_test, "UPDATE Mobiltelefoner SET Koebt='Ja' WHERE Id='%d';", scanid);
  // printf(" %s", sql_test);

  /* Scanner string ind */
  printf("Hvad vil du sortere efter?\n");
  char sql_ori[MAX_CHAR];
  char input[MAX_CHAR]; 
  scanf(" %s", input);
  sprintf(sql_ori, "SELECT * from Mobiltelefon ORDER BY %s", input);  // Combines SQL command with user input

  //sqlite3_exec(db, sql_test, callback, (void*)data, &zErrMsg); 

  /* Ligger kode hen i C variabler */
  /* Erklærer database handle */
  sqlite3_stmt *stmt;
  int ret;

  /* Erklærer struct til opbevaring af data */
  sql_data sql[MAX_GROUP];
  int id = 0;

  ret = sqlite3_prepare_v2(db, sql_ori, -1, &stmt, NULL);
  ret = sqlite3_step(stmt);

  /* */
  while(ret == SQLITE_ROW){
    strcpy(sql[id].maerke, sqlite3_column_text(stmt, 0));
    sql[id].pris = sqlite3_column_int(stmt, 3);
    
    /* Tester kun for de 20 første */
    if(id >= 20){
      break;
    }
  
    id++;
    ret = sqlite3_step(stmt);
  }

  /*  */
  sqlite3_finalize(stmt);

  /* Printer 20 af resultaterne */
  int i;
  for ( i = 0; i < id; ++i)
  {
    printf(" [%d] Name: %s Pris:%d\n", i, sql[i].maerke, sql[i].pris);
  }

  /* Lukker database */
  sqlite3_close(db);
}