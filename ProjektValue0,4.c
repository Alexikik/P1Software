#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h" 
#include <string.h>
#define MAX_CHAR 100
#define MAX_LEN 100

void ifcreateaccount(sqlite3 *db, int value);
void iflogin(sqlite3 *db, int value);
void giveValue(int*);
int mainFunk();
int get_value(sqlite3 *db, char password[]);

int main(void) {
     mainFunk();
    /*sqlite3 *db; */
    /*printf("test100 = %d\n", get_value(db, "Hejsadu")); */
    return 0;
}


int mainFunk() {
    sqlite3 *db;        // Pointer to database
    char *sql_cmd;      // String for sql command
    
    sqlite3_open("Mobiltelefoner.db", &db);    // Opens database
    printf(" --- Opened database successfully --- \n");

    sql_cmd = "SELECT * from User";

    int value = 0, error = 0;
    char scan_answer[100];
    printf("1. Login    2. Create an account:");
    while (error == 0) {
        scanf("%s", scan_answer);
        if (strcmp(scan_answer, "1") == 0) {
            iflogin(db, value);
            error++;
        }
        else if (strcmp(scan_answer, "2") == 0) {
            ifcreateaccount(db, value);
            error++;
        }
        else
            printf("Type 1 to login, 2 to create an account\n");
    }
    return 0;
}
void ifcreateaccount(sqlite3 *db, int value) {
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
        sqlite3_stmt *selectstmt;
        int result = sqlite3_prepare_v2(db, sql_one, -1, &selectstmt, NULL);
        if(result == SQLITE_OK) {
            if (sqlite3_step(selectstmt) == SQLITE_ROW) {
                printf("That password already exists. Type another one\n");
            }
            else {
                password_exist = 0;
                giveValue(&value);
                sprintf(sql_one, "INSERT INTO User VALUES(null, '%s', '%s', %d);", username, password, value );
                printf("%s\n", sql_one); 
                exit = sqlite3_exec(db, sql_one, NULL, 0, &messageError);
                if (exit != SQLITE_OK) {
                    printf("Error Insert\n");
                    sqlite3_free(messageError);
                    }
                else {
                    printf("Records created successfully\nWelcome %s", username);
                     /*Get value from database into variable here!*/
                }       
            }
        }
            sqlite3_finalize(selectstmt);  
    }
}
void iflogin(sqlite3 *db, int value) {
    int user_exist = 2, exit, error_in_type = 0;
    char new_account[MAX_LEN];

    char username[20], password[20], sql_one[100];
    char *messageError = "Something went wrong";
    /*  printf("Type your Username\n");
    scanf(" %s", username);
    printf("\n");*/
    while (user_exist == 2) {
        printf("Type your Password\n");
        scanf(" %s", password);
        printf("\n");
        sprintf(sql_one, "SELECT * FROM User WHERE Password LIKE '%s'", password );
        sqlite3_stmt *selectstmt;
        int result_two = sqlite3_prepare_v2(db, sql_one, -1, &selectstmt, NULL);
        if(result_two == SQLITE_OK) {
            if (sqlite3_step(selectstmt) == SQLITE_ROW) {
                user_exist = 1;  
                printf("Velkommen \n"); // Indsæt username
                /*Get value from database into variable here!*/
                break;
            }
            else  {
                printf("Du er ikke oprettet i systemet. Tastede du forkert? Tast 1 for at oprette en ny konto, tast 0 for at proeve igen\n");
                while (error_in_type == 0) {
                    scanf(" %s", new_account);
                    printf("\n");
                    if (strcmp(new_account, "1") == 0) {
                        printf("Type a Username\n");
                        scanf("%s", username);
                        printf("Type a Password\n");
                        scanf("%s", password);
                        giveValue(&value);
                        sprintf(sql_one, "INSERT INTO User VALUES(null, '%s', '%s', %d);", username, password, value );
                        exit = sqlite3_exec(db, sql_one, NULL, 0, &messageError);
                        if (exit != SQLITE_OK) {
                            printf("Error Insert2\n");
                            sqlite3_free(messageError);
                        }
                        else {
                            printf("Records created successfully\nWelcome: %s\n", username);
                            /*Get value from database into variable here!*/
                            error_in_type = 1;
                            user_exist = 1;
                            break;
                        }
                    }
                    else if (strcmp(new_account, "0") == 0) {
                        break;
                    }
                    else {
                        printf("Error. Please type 1 to create a new account, and 0 to retype\n");
                    }
                }
            }
        } 
        sqlite3_finalize(selectstmt);     
    }  
}
void giveValue(int *value) {
    /*Spørgsmål 1*/
    char scan_answer[MAX_LEN];
    printf("%s%s%s%s%s", "Du faar nogle spoergsmaal som vil hjaelpe programmet med at finde de bedste produkter for dig\n",
                    "Er det vigtigt for dig, at din genbrugsvare er forsikret eller nemID-verificeret?\n\n",
                    "1. Overhovedet ikke vigtigt\n",
                    "2. Har en betydning, men ikke altafgoerende for om jeg vil koebe produktet\n",
                    "3. Jeg koeber ikke produktet hvis det ikke er forsikret eller nemID-verificeret\n");
    int start_over = 0;
    while (start_over == 0) {
        scanf("%s", scan_answer);
        if (strcmp(scan_answer, "1") == 0) {
            *value += 1;
            start_over = 1;
        }
        else if (strcmp(scan_answer, "2") == 0) {
            *value += 2;
            start_over = 1;
        }
        else if (strcmp(scan_answer, "3") == 0) {
            *value += 3;
            start_over = 1;
        }
        else {
            printf("Tast venligst 1, 2 eller 3\n");
        }
    }
    printf("%s%s%s%s", "\n\nHvor vigtigt er prisen paa produktet for dig?\n\n",
                    "1. Prisen paa produktet er det vigtiste kriterie, for om jeg koeber produktet\n",
                    "2. Prisen har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
                    "3. Jeg vurdere andre kriterier som kvalitet eller sikkerhed hoejere end pris\n");
    start_over = 0;
    while (start_over == 0) {
        scanf("%s", scan_answer);
        if (strcmp(scan_answer, "1") == 0) {
            *value += 1;
            start_over = 1;
        }
        else if (strcmp(scan_answer, "2") == 0) {
            *value += 2;
            start_over = 1;
        }
        else if (strcmp(scan_answer, "3") == 0) {
            *value += 3;
            start_over = 1;
        }
        else {
            printf("Tast venligst 1, 2 eller 3\n");
        }
    }
    printf("%s%s%s%s", "\n\nHvor vigtigt er kvaliteten af produktet for dig?\n\n",
                "1. Jeg vurdere andre kriterier som pris eller sikkerhed hoejere end kvalitet\n",
                "2. Kvaliteten har en betydning, men det er ikke afgoerende for om jeg vil koebe produktet\n",
                "3. Kvaliteten er det vigtigste kriterie for, om jeg koeber produktet\n");
    start_over = 0;
    while (start_over == 0) {
        scanf("%s", scan_answer);
        if (strcmp(scan_answer, "1") == 0) {
            *value += 1;
            start_over = 1;
        }
        else if (strcmp(scan_answer, "2") == 0) {
            *value += 2;
            start_over = 1;
        }
        else if (strcmp(scan_answer, "3") == 0) {
            *value += 3;
            start_over = 1;
        }
        else {
            printf("Tast venligst 1, 2 eller 3\n");
        }
    }
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
































