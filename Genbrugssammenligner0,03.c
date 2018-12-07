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

  /* Ligger kode hen i C variabler */
  /* Erklærer database handle */
  sqlite3_stmt *stmt;
  int ret;

  sql_data sql[MAX_GROUP];
  int id = 0;

  ret = sqlite3_prepare_v2(db, "SELECT * FROM Mobiltelefoner", -1, &stmt, NULL);
  ret = sqlite3_step(stmt);

  while(ret == SQLITE_ROW){
    strcpy(sql[id].maerke, sqlite3_column_text(stmt, 0));
    sql[id].pris = sqlite3_column_int(stmt, 3);
    
    /* Tester kun for de 20 første */
    if(id >= 100){
      break;
    }
  
    id++;
    ret = sqlite3_step(stmt);
  }

  /*  */
  sqlite3_finalize(stmt);

  int i;
  for ( i = 0; i < id; ++i)
  {
    printf(" [%d] Name: %s Pris:%d\n", i, sql[i].maerke, sql[i].pris);
  }

  /* Lukker database */
  sqlite3_close(db);
}