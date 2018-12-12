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
#define MAX_GROUP 200

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
int convert_struct(sql* sql_group, int len, sqlite3 *db, int index, void *data, char *zErrMsg, sqlite3_stmt *stmt, char *sql_ori);
int select_genbrugs_type(char *genbrugstype, char *sql_ori);
int go_to_item(sql* sql_group, int len, int inputId);
void list_struct(sql* sql_group, int len);

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

  /* Erklærer database handle */
  sqlite3_stmt *stmt;

  /* Erklærer struct til opbevaring af data */
  sql sql_group[MAX_GROUP];
  /* Index til vores array af structs */
  int index = 1;

  /* Lav en funktion til dette for genbrugstype for hvad der skal lægges over i struct -  WHERE Dato_Solgt IS NULL sorterer alle solgte fra databasen */
  char sql_ori[MAX_CHAR];

  /* Jakobs kode kommer til at styre denne her funktion */
  select_genbrugs_type("kritisk", sql_ori);

  /* Ligger tingene over i en struct. len er hvor mange der skal gemmes i vores struct */
  int len = 20;
  convert_struct(sql_group, len, db, index, (void*)data, zErrMsg, stmt, sql_ori);

  /* Lister tingene */
  list_struct(sql_group, len);

  /* Indlæser hvad brugeren har gjort efter go_to_item  */
  int go_to_res = 0;
  
  int inputId = 0;
  int inputRes;

  do{

    printf("Write an ID to see more details about the product:\n");
    inputRes = scanf(" %d", &inputId);

    if(inputRes == 1){
      go_to_res = go_to_item(sql_group, len, inputId);

      /* Hvis brugeren har indtastet 3 så købes produktet */
      if(go_to_res == 3){
        buy_item(db, sql_group[inputId].id, (void*)data, zErrMsg);
      }
      /* Refresher helt */
      convert_struct(sql_group, len, db, index, (void*)data, zErrMsg, stmt, sql_ori);
      list_struct(sql_group, len);

      printf("Type -1 to exit!\n");
    }
    else{
      printf("You did not enter a number, program closes!\n");
      exit(0);
    }

    

  }while(inputId != -1);

  /* Lukker database */
  sqlite3_close(db);
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


  char sql_test[MAX_CHAR];
  sprintf(sql_test, "UPDATE Mobiltelefon SET Dato_Solgt='09-12-2018' WHERE ID=%d;", idx);

  sqlite3_exec(db, sql_test, callback, (void*)data, &zErrMsg);

}

/* Funktion til at ligge data fra database over i struct */
int convert_struct(sql* sql_group, int len, sqlite3 *db, int index, void *data, char *zErrMsg, sqlite3_stmt *stmt, char *sql_ori){
  int ret;

  ret = sqlite3_prepare_v2(db, sql_ori, -1, &stmt, NULL);

  /* Stepper til den første */
  ret = sqlite3_step(stmt);

  /* Kører gennem alle SQLITE rows */
  while(ret == SQLITE_ROW){
    /* Ligger over i struct*/
    sql_group[index].id = sqlite3_column_int(stmt, 0);
    strcpy(sql_group[index].maerke, sqlite3_column_text(stmt, 1));
    strcpy(sql_group[index].model, sqlite3_column_text(stmt, 2));
    strcpy(sql_group[index].os, sqlite3_column_text(stmt, 3));
    sql_group[index].pris = sqlite3_column_int(stmt, 4);
    strcpy(sql_group[index].farve, sqlite3_column_text(stmt, 5));
    sql_group[index].mem = sqlite3_column_int(stmt, 6);
    strcpy(sql_group[index].stand, sqlite3_column_text(stmt, 7));
    strcpy(sql_group[index].forsikring, sqlite3_column_text(stmt, 8));
    strcpy(sql_group[index].nemid, sqlite3_column_text(stmt, 9));
    sql_group[index].postnr = sqlite3_column_int(stmt, 10);
    strcpy(sql_group[index].date_added, sqlite3_column_text(stmt, 11));
    //strcpy(sql_group[index].date_sold, sqlite3_column_text(stmt, 12));
    strcpy(sql_group[index].link, sqlite3_column_text(stmt, 13));

    /* Tester kun for de 20 første */
    if(index >= len){
      break;
    }
    
    /* Tæller id op med 1, så vi har en specifik plads i vores array af structs */
    index++;
    /* Stepper til den næste*/
    ret = sqlite3_step(stmt);
  }

  /* Finalizer database handle */
  sqlite3_finalize(stmt);
}

/* Funktion til at vælge genbrugstype */
int select_genbrugs_type(char *genbrugstype, char *sql_ori){
  /* Switch statement virker ikke til det her ;)  */
  if (strcmp(genbrugstype, "kritisk") == 0) 
  {
    /* Pris */
    sprintf(sql_ori, "SELECT * from Mobiltelefon WHERE Dato_Solgt IS NULL ORDER BY %s;", "Pris");
  } 
  else if (strcmp(genbrugstype, "social") == 0)
  {
    /* Sorterer med postnummer. Postnummer skal have en funktion der kan beregne distance mellem bruger og postnummer */
    sprintf(sql_ori, "SELECT * from Mobiltelefon WHERE Dato_Solgt IS NULL ORDER BY %s;", "Postnummer");
  }
  /* more else if clauses */
  else /* default: */
  {
    printf("genbrugstype was not defined properly. Exitting program!");
    exit(0);
  }
}

/* Vælger item og man har tre muligheder 1) ingenting 2) gå til siden 3) køb produktet */
int go_to_item(sql* sql_group, int len, int inputId){
  /* Printer information om varen */
  printf("Details about #%d:\n ID:%d\n Maerke:%s\n Model:%s\n OS:%s\n Pris:%d\n Farve:%s\n Memory:%d\n Stand:%s\n Forsikring:%s\n NemID verificeret:%s\n Postnummer:%d\n Dato added:%s\n Link:%s \n\n", inputId, sql_group[inputId].id, sql_group[inputId].maerke, sql_group[inputId].model, sql_group[inputId].os, sql_group[inputId].pris, sql_group[inputId].farve, sql_group[inputId].mem, sql_group[inputId].stand, sql_group[inputId].forsikring, sql_group[inputId].nemid, sql_group[inputId].postnr, sql_group[inputId].date_added, sql_group[inputId].link);

  /* Prompter brugeren for at købe, gå til link eller gå tilbage */
  printf("Type 1 to return\nType 2 to visit page\nType 3 to buy\n->");
  int select = 0;
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
}

void list_struct(sql* sql_group, int len){
  /* Printer len af struct */
  int i;
  for ( i = 1; i < len; ++i)
  {
    printf("#%d [%d] %s %s --- Pris:%d [%s]\n", i, sql_group[i].id, sql_group[i].maerke, sql_group[i].model, sql_group[i].pris, sql_group[i].date_added);
  }
}