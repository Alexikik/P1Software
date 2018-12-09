#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h" 
void ifcreateaccount(sqlite3 *db, int *value);
void iflogin(sqlite3 *db, int *value) ;
static int callback(void *data, int argc, char **argv, char **azColName);
void giveValue(int*);

int main(void) {
  sqlite3 *db;
  int rc;
  char *sql;
     /* Open database */
  rc = sqlite3_open("Mobiltelefoner.db", &db);
   
  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return(0); 
  } 
  else {
    fprintf(stderr, "Opened database successfully\n");
  }

  sql = "SELECT * from User";
  int value = 0;

  int scan_answer, error = 0;
  printf("1. Login    2. Create an account:");
  while (error == 0) {
    scanf("%d", &scan_answer);
    if (scan_answer == 1) {
      iflogin(db, &value);
      error++;
    }
    else if (scan_answer == 2) {
      ifcreateaccount(db, &value);
      error++;
    }
    else {
      printf("Type 1 to login, 2 to create an account\n");
  }
}
  return 0;

}
void ifcreateaccount(sqlite3 *db, int *value) {
  int password_exist = 1, exit;
  char username[20], password[20], sql_one[100];
  const char* data = "Callback function called";
  char *messageError = "Something went wrong";
  printf("Type a Username\n");
  scanf(" %s", username);
  while (password_exist == 1) {
      printf("Type a Password\n");
      scanf(" %s", password);
      sprintf(sql_one, "SELECT * FROM User WHERE Password LIKE '%s%';", password );
      sqlite3_stmt *selectstmt_three;
      int result = sqlite3_prepare_v2(db, sql_one, -1, &selectstmt_three, NULL);
      if(result == SQLITE_OK) {
        if (sqlite3_step(selectstmt_three) == SQLITE_ROW) {
          printf("That password already exists. Type another one\n");
        }
        else {
          password_exist = 0;
          giveValue(value);
          sprintf(sql_one, "INSERT INTO User VALUES(null, '%s', '%s', %d);", username, password, value );
          printf("%s\n", sql_one); 
          exit = sqlite3_exec(db, sql_one, NULL, 0, &messageError);
          if (exit != SQLITE_OK) {
            printf("Error Insert\n");
            sqlite3_free(messageError);
          }
          else {
            printf("Records created successfully\n");
            /*Get value from database into variable here!*/
          }       
        }
      }
      sqlite3_finalize(selectstmt_three);  
    }
}
void iflogin(sqlite3 *db, int *value) {
  int user_exist = 2, exit, new_account;
  char username[20], password[20], sql_one[100];
  char *messageError = "Something went wrong";
  printf("Type your Username\n");
  scanf(" %s", username);
  while (user_exist == 2) {
    printf("Type your Password\n");
    scanf(" %s", password);
    sprintf(sql_one, "SELECT * FROM User WHERE Password LIKE '%s%';", password );
    sqlite3_stmt *selectstmt;
    int result_two = sqlite3_prepare_v2(db, sql_one, -1, &selectstmt, NULL);
    if(result_two == SQLITE_OK) {
      if (sqlite3_step(selectstmt) == SQLITE_ROW) {
        user_exist == 1;  
        printf("Velkommen %s\n", username);
        /*Get value from database into variable here!*/
        break;
      }
      else  {
        printf("Du er ikke oprettet i systemet. Tastede du forkert? Tast 1 for at oprette en ny konto, tast 0 for at prøve igen\n");
        scanf(" %d", &new_account);
        if (new_account == 1) {
          printf("Type a Username\n");
          scanf("%s", username);
          printf("Type a Password\n");
          scanf("%s", password);
          giveValue(value);
          printf("hej\n");
          sprintf(sql_one, "INSERT INTO User VALUES(null, '%s', '%s', %d);", username, password, value );
          exit = sqlite3_exec(db, sql_one, NULL, 0, &messageError);
          if (exit != SQLITE_OK) {
            printf("Error Insert\n");
            sqlite3_free(messageError);
          }
          else {
            printf("Records created successfully\n");
            printf("Velkommen %s. Du bliver nu sendt videre til programmet\n", username);
            /*Get value from database into variable here!*/
            break;
          }
        }
      }
    } 
    sqlite3_finalize(selectstmt);     
  }  
}


void giveValue(int *value) {
  /*Spørgsmål 1*/
  int scan_answer;
  printf("%s%s%s%s%s", "Du faer nogle spoergsmael som vil hjaelpe programmet med at finde de bedste produkter for dig\n",
          "Er det vigtigt for dig, at din genbrugsvare er forsikret eller nemID-verificeret?\n",
          "1. Overhovedet ikke vigtigt\n",
          "2. Har en betydning, men ikke altafgørende for om jeg vil koebe produktet\n",
          "3. Jeg koeber ikke produktet hvis det ikke er forsikret eller nemID-verificeret\n");
  scanf("%d", &scan_answer);
  if (scan_answer == 1) {
    *value += 1;
  }
  else if (scan_answer == 2) {
    *value += 2;
  }
  else if (scan_answer == 3) {
    *value += 3;
  }
  printf("%s%s%s%s", "\n\nHvor vigtigt er prisen pae produktet for dig?\n",
          "1. Jeg vurdere andre kriterier som kvalitet eller sikkerhed højere end pris\n",
          "2. Prisen har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
          "3. Prisen på produktet er det vigtiste kriterie, for om jeg koeber produktet\n");
  scanf("%d", &scan_answer);
  if (scan_answer == 1) {
    *value += 3;
  }
  else if (scan_answer == 2) {
    *value += 2;
  }
  else if (scan_answer == 3) {
    *value += 1;
  }
  printf("%s%s%s%s", "\n\nHvor vigtigt er kvaliteten af produktet for dig?\n",
        "1. Jeg vurdere andre kriterier som pris eller sikkerhed højere end kvalitet\n",
        "2. Kvaliteten har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
        "3. Kvaliteten er det vigtigste kriterie for, om jeg koeber produktet\n");
  scanf("%d", &scan_answer);
  if (scan_answer == 1) {
    *value += 1;
  }
  else if (scan_answer == 2) {
    *value += 2;
  }
  else if (scan_answer == 3) {
    *value += 3;
  }
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
