#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#define MAX_CHAR 100
#define MAX_GROUP 100

/* Prototyper */
void buy_product();
int list_products();

static int index = 1;   // Global int for testing purposes (Indexes the phones with a number)

/* Struct array */
typedef struct{
  
  char maerke[MAX_CHAR];  

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
  rc = sqlite3_open("v2_Mobiltelefoner.db", &db);
  
  if(rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return 0;
  }else{
    fprintf(stderr, "Opened database successfully\n");
  }

  /* Ligger kode hen i C variabler */
  /* Erklærer database handle */
  sqlite3_stmt *res;
  int ret;

  sql_data sql[MAX_GROUP];
  int id = 0;

  ret = sqlite3_prepare_v2(db, "SELECT * FROM Mobiltelefon", -1, &res, NULL);
  ret = sqlite3_step(res);

  while(ret == SQLITE_ROW){

    sql[id].maerke = sqlite3_column_text(res, 2);  

    /* Tester kun for de 20 første */
    if(id >= 20){
      break;
    }
    
    sqlite3_step (res);
    id++;
    ret = sqlite3_step(res);
  }

  sqlite3_finalize(res);

  int i;
  for ( i = 0; i < id; ++i)
  {
    printf(" [%d] Name:%s\n", i, sql[i].maerke);
  }

  /* Lukker database */
  sqlite3_close(db);
}

void buy_product(){

  /* */
  //sqlite3_free();

  char tobuy[MAX_CHAR] = "ja";

  char *zSQL; 
  //zSQL = sqlite3_mprintf("INSERT INTO Mobiltelefoner (Maerke,Koebt) VALUES ('Apple', 'Ja');", tobuy);

  int id = 0;

  scanf(" %d", &id);

  char sql_test[MAX_CHAR] = "UPDATE Mobiltelefoner SET Koebt='Ja' WHERE Id='%d';";
  
  sqlite3_exec(db, sql_test, callback, (void*)data, &zErrMsg); 


  /* sqlite3_snprintf() */
  /* Kan udskrive ting */
}

int list_products(){

  return 1;
}


