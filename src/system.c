#include "header.h"
#include <sqlite3.h>
#include <ctype.h>

void initDatabase() {
   
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open("data/database.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    const char *sqlUser = "CREATE TABLE IF NOT EXISTS users ("
                          "id INTEGER PRIMARY KEY, "
                          "name TEXT UNIQUE, "
                          "password TEXT);";

    const char *sqlRecord = "CREATE TABLE IF NOT EXISTS records ("
                            "id INTEGER PRIMARY KEY, "
                            "userId INTEGER, "
                            "name TEXT, "
                            "accountNbr INTEGER, "
                            "country TEXT, "
                            "phone INTEGER, "
                            "accountType TEXT, "
                            "amount REAL, "
                            "depositMonth INTEGER, "
                            "depositDay INTEGER, "
                            "depositYear INTEGER, "
                            "withdrawMonth INTEGER, "
                            "withdrawDay INTEGER, "
                            "withdrawYear INTEGER, "
                            "FOREIGN KEY(userId) REFERENCES users(id));";

    rc = sqlite3_exec(db, sqlUser, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    rc = sqlite3_exec(db, sqlRecord, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

}




void exitOrMenu(struct User u)
{
    int option;
    printf("\nEnter 1 to go to the main menu or click any to exit: ");
    scanf("%d", &option);
    clear();

    if(option == 1){
        mainMenu(u);
    } else {
        exit(0);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    printf("response: ");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void clear(void){
    char buffer ;
    while ((buffer = getchar())!= '\n' && buffer!= EOF);
}
void createNewAcc(struct User u) {
    struct Record r;
    sqlite3_stmt *stmt;
    system("clear");
    printf("\t\t\t===== New Record =====\n");

enterDate:
    printf("\nEnter today's date (mm/dd/yyyy): ");
    scanf("%2d/%2d/%4d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    clear();
    if (r.deposit.month < 1 || r.deposit.month > 12 || r.deposit.day < 1 || r.deposit.day > 31 || r.deposit.year < 1900) {
        printf("✖ Invalid date. Please try again.\n");
        goto enterDate;
    }
   // printf("%d %d %d",r.deposit.day,r.deposit.month,r.deposit.year);


enterAccountNumber:
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNbr);
    clear();
    if (r.accountNbr < 0) {
        printf("✖ Invalid account number. Please try again.\n");
        goto enterAccountNumber;
    }
    //printf("%d",r.accountNbr);


    // Check if account number exists
    const char *checkQuery = "SELECT id FROM records WHERE accountNbr = ?";
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL) != SQLITE_OK) {
    printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
    }

    sqlite3_bind_int(stmt, 1, r.accountNbr);


    if (sqlite3_step(stmt) == SQLITE_ROW) {
    printf("Account number exists.\n");
    }

    sqlite3_finalize(stmt);

    printf("\nEnter the country: ");
    scanf("%49s", r.country);
    clear();



enterPhoneNumber:
    printf("\nEnter the phone number: ");
    scanf("%14s", r.phone);
    clear();
     
          for (int i = 0; r.phone[i] != '\0'; i++) {
            if (!isdigit(r.phone[i])) {
            printf("✖ Invalid phone number. Please enter numbers only.\n");
            goto enterPhoneNumber;
        }
    }

enterAmount:
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    clear();
    if (r.amount < 0) {
        printf("✖ Invalid amount. Please try again.\n");
        goto enterAmount;
    }

enterAccountType:
    printf("\nChoose the type of account:\n\t-> savings\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice: ");
    scanf("%49s", r.accountType);
    clear();
    if (strcmp(r.accountType, "savings") != 0 && strcmp(r.accountType, "current") != 0 &&
        strcmp(r.accountType, "fixed01") != 0 && strcmp(r.accountType, "fixed02") != 0 &&
        strcmp(r.accountType, "fixed03") != 0) {
        printf("✖ Invalid account type. Please try again.\n");
        goto enterAccountType;
    }

    const char *insertQuery = "INSERT INTO records (userId, accountNbr, country, phone, accountType, amount, depositMonth, depositDay, depositYear,name) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, u.id);
    sqlite3_bind_int(stmt, 2, r.accountNbr);
    sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt , 4 , r.phone , -1 , SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, r.accountType, -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, r.amount);
    sqlite3_bind_int(stmt, 7, r.deposit.month);
    sqlite3_bind_int(stmt, 8, r.deposit.day);
    sqlite3_bind_int(stmt, 9, r.deposit.year);
    sqlite3_bind_text(stmt, 10, u.name , -1 , SQLITE_TRANSIENT);
// printf("name :%s\n", u.name);
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("✔ Account created successfully.\n");
         success(u);
    } else {
        printf("✖ Failed to create account: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
   
}
void checkAllAccounts(struct User u) {
    sqlite3_stmt *stmt;
    const char *query = "SELECT accountNbr, country, phone, amount, accountType, depositMonth, depositDay, depositYear "
                        "FROM records WHERE userId = ?"; 

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

 
    sqlite3_bind_int(stmt, 1, u.id);

    system("clear");
    printf("\t\t====== All Accounts for User: %s =====\n\n", u.name);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("_____________________\n");
        int retrievedAccountNbr = sqlite3_column_int(stmt, 0);
        const char *country = (const char *)sqlite3_column_text(stmt, 1);
        const char *phone = (const char *)sqlite3_column_text(stmt, 2);
        double amount = sqlite3_column_double(stmt, 3);
        const char *accountType = (const char *)sqlite3_column_text(stmt, 4);
        int depositMonth = sqlite3_column_int(stmt, 5);
        int depositDay = sqlite3_column_int(stmt, 6);
        int depositYear = sqlite3_column_int(stmt, 7);

        printf("\nAccount Number: %d\n", retrievedAccountNbr);
        printf("Country: %s\n", country);
        printf("Phone Number: %s\n", phone); 
        printf("Amount Deposited: $%.2f\n", amount);
        printf("Account Type: %s\n", accountType);
        printf("Deposit Date: %02d/%02d/%04d\n", depositMonth, depositDay, depositYear);
    }
    sqlite3_finalize(stmt);

  exitOrMenu(u);
}

void updateAccountInfo(struct User u) {
    int accountNbr; 
    char newCountry[50];
    char newPhone[15]; 
    int choice;
    sqlite3_stmt *stmt;

retry:
    system("clear");
    printf("\t\t\t===== Update Account Information =====\n\n");
    printf("\nEnter the account number of the record you want to update: ");
    scanf("%d", &accountNbr);
    clear();

    // Check if the record exists
    const char *checkQuery = "SELECT accountNbr, phone, country FROM records WHERE accountNbr = ? AND userId = ?";
    
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, accountNbr);
    sqlite3_bind_int(stmt, 2, u.id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("\n✖ Record not found or does not belong to you.\n");
        sqlite3_finalize(stmt);

        char retryOption;
        printf("\nWould you like to try again? (y/n): ");
        scanf(" %c", &retryOption);
        clear();

        if (retryOption == 'y' || retryOption == 'Y') {
            goto retry;
        } else {
            printf("\nReturning to main menu...\n");
            return;
        }
    }

    printf("\n✔ Record found!\n");
    printf("\nWhat would you like to update?\n");
    printf("[1] Phone Number\n");
    printf("[2] Country\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    clear();

    switch (choice) {
        case 1:
            printf("\nEnter new phone number: ");
            scanf("%14s", newPhone); 
            clear();
            break;
        case 2:
            printf("\nEnter new country: ");
            scanf("%49s", newCountry);
            clear();
            break;
        default:
            printf("\n✖ Invalid choice.\n");
            sqlite3_finalize(stmt);
            clear();
            return;
    }

    const char *updateQuery = "UPDATE records SET ";
    
    if (choice == 1) {
        updateQuery = "UPDATE records SET phone = ? WHERE accountNbr = ? AND userId = ?";
        
        if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, NULL) != SQLITE_OK) {
            printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            return;
        }
        
        sqlite3_bind_text(stmt, 1, newPhone, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, accountNbr);
        sqlite3_bind_int(stmt, 3, u.id);
        
    } else if (choice == 2) {
        updateQuery = "UPDATE records SET country = ? WHERE accountNbr = ? AND userId = ?";
        
        if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, NULL) != SQLITE_OK) {
            printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            return;
        }
        
        sqlite3_bind_text(stmt, 1, newCountry, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, accountNbr);
        sqlite3_bind_int(stmt, 3, u.id);
    }


    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("\n✔ Record information updated successfully!\n");
        success(u);
    } else {
        printf("✖ Failed to update record: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
    success(u);
}
void checkDetailAccount(struct User u) {
    int accountNbr; 
    sqlite3_stmt *stmt;
    char retryOption;

    do {
        system("clear");
        printf("\t\t\t===== Check Account Details =====\n\n");
        printf("Enter the account number you want to check: ");
        scanf("%d", &accountNbr);
        clear();
        const char *query = "SELECT accountNbr, country, phone, amount, accountType, depositMonth, depositDay, depositYear "
                            "FROM records WHERE accountNbr = ? AND userId = ?";

        if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
            printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            return;
        }
        sqlite3_bind_int(stmt, 1, accountNbr);
        sqlite3_bind_int(stmt, 2, u.id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int retrievedAccountNbr = sqlite3_column_int(stmt, 0);
            const char *country = (const char *)sqlite3_column_text(stmt, 1);
            const char *phone = (const char *)sqlite3_column_text(stmt, 2); 
            double amount = sqlite3_column_double(stmt, 3);
            const char *accountType = (const char *)sqlite3_column_text(stmt, 4);
            int depositMonth = sqlite3_column_int(stmt, 5);
            int depositDay = sqlite3_column_int(stmt, 6);
            int depositYear = sqlite3_column_int(stmt, 7);

            printf("\nAccount Number: %d\n", retrievedAccountNbr);
            printf("Country: %s\n", country);
            printf("Phone Number: %s\n", phone); 
            printf("Amount Deposited: $%.2f\n", amount);
            printf("Account Type: %s\n", accountType);
            printf("Deposit Date: %02d/%02d/%04d\n", depositMonth, depositDay, depositYear);

            double interest = 0.0; 

            if (strcmp(accountType, "savings") == 0) {
                interest = amount * 0.07;
                printf("You will get $%.2f as interest on day %d of every month.\n", interest / 12.0, depositDay);
            } else if (strcmp(accountType, "fixed01") == 0) {
                interest = amount * 0.04; 
                int finalYear = depositYear + 1; 
                printf("You will get $%.2f as interest on %02d/%02d/%04d.\n", interest / 12.0 * (12), depositDay,
                       depositMonth,finalYear); 
            } else if (strcmp(accountType, "fixed02") == 0) {
                interest = amount * 0.05;
                int finalYear = depositYear + 2; 
                printf("You will get $%.2f as interest on %02d/%02d/%04d.\n", interest*2 , depositDay,
                       depositMonth,finalYear); 
            } else if (strcmp(accountType,"fixed03") ==0){
                interest=amount*0.08;
                int finalYear=depositYear+3; 
                printf ("You will get $%.2f as interest on %02d/%02d/%04d.\n" ,interest*3,depositDay,
                        depositMonth,finalYear); 
            }
           else if(strcmp(accountType,"current")==0){
                printf ("You will not get interests because the account is of type current.\n");
            }
        } else {
            printf("\n✖ Account not found or does not belong to you.\n");
        }

        sqlite3_finalize(stmt);

        printf("\nWould you like to try again? (y/n): ");
        scanf(" %c", &retryOption);
        clear();
    } while(retryOption == 'y');

     exitOrMenu(u);

}


void trimWhitespace(char *str) {
    int end;
    int start = 0;
    
    // Trim leading spaces
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }
    
    // Trim trailing spaces
    end = strlen(str) - 1;
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }
    
    // Shift characters to remove leading spaces
    memmove(str, str + start, end - start + 1);
    str[end - start + 1] = '\0';  // Null-terminate the string
}
void makeTransactions(struct User u) {
    int accountNbr; 
    double amount; 
    char transactionType; 
    sqlite3_stmt *stmt;

    system("clear");
    printf("\t\t\t===== Make Transactions =====\n\n");
    printf("Enter the account number you want to transact on: ");
    scanf("%d", &accountNbr);
    clear();

    const char *query = "SELECT accountType, amount FROM records WHERE accountNbr = ? AND userId = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, accountNbr);
    sqlite3_bind_int(stmt, 2, u.id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *accountType = (const char *)sqlite3_column_text(stmt, 0);
        double currentBalance = sqlite3_column_double(stmt, 1);

        if (strcmp(accountType, "fixed01") == 0 || strcmp(accountType, "fixed02") == 0 || strcmp(accountType, "fixed03") == 0) {
            printf("✖ Transactions are not allowed for accounts of type %s.\n", accountType);
            sqlite3_finalize(stmt);
    exitOrMenu(u);
            
        }

        char retryOption;
        do {
            printf("Current Balance: $%.2f\n", currentBalance);
            printf("Enter 'd' to deposit or 'w' to withdraw: ");
            scanf(" %c", &transactionType);
            clear();
            if (transactionType != 'd' && transactionType != 'w') {
                printf("✖ Invalid transaction type. Would you like to try again? (y/n): ");
                scanf(" %c", &retryOption);
                clear();
                if (retryOption == 'y' || retryOption == 'Y') {
                    continue; 
                } else {
                    sqlite3_finalize(stmt);
                    return; 
                }
            }
            printf("Enter amount: $");
            scanf("%lf", &amount);
            clear();
            if (amount <= 0) {
                printf("✖ Amount must be positive. Please enter a valid amount.\n");
                printf("Would you like to try again? (y/n): ");
                scanf(" %c", &retryOption);
                clear();
                if (retryOption == 'y' || retryOption == 'Y') {
                    continue; 
                } else {
                    sqlite3_finalize(stmt);
                    return; 
                }
            }

            if (transactionType == 'd') {

                currentBalance += amount; 
                printf("Depositing $%.2f...\n", amount);
            } else if (transactionType == 'w') {

                if (amount > currentBalance) {
                    printf("✖ Insufficient funds for withdrawal.\n");
                    sqlite3_finalize(stmt);
                    exitOrMenu(u);
                    
                }
                currentBalance -= amount;
                printf("Withdrawing $%.2f...\n", amount);
            }

            break; 

        } while (1);
        const char *updateQuery = "UPDATE records SET amount = ? WHERE accountNbr = ? AND userId = ?";
        
        if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, NULL) != SQLITE_OK) {
            printf("✖ Failed to prepare update statement: %s\n", sqlite3_errmsg(db));
            return;
        }

        sqlite3_bind_double(stmt, 1, currentBalance);
        sqlite3_bind_int(stmt, 2, accountNbr);
        sqlite3_bind_int(stmt, 3, u.id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("✔ Transaction completed successfully! New Balance: $%.2f\n", currentBalance);

        } else {
            printf("✖ Failed to update account balance: %s\n", sqlite3_errmsg(db));
    exitOrMenu(u);

        }
        
    } else {
        printf("\n✖ Account not found or does not belong to you.\n");
    exitOrMenu(u);

    }

    sqlite3_finalize(stmt);
    exitOrMenu(u);
}



void deleteAccount(struct User u) {
    int accountNbr; 
    sqlite3_stmt *stmt;

    system("clear");
    printf("\t\t\t===== Delete Account =====\n\n");
    printf("Enter the account number you want to delete: ");
    scanf("%d", &accountNbr);
                clear();

    const char *checkQuery = "SELECT accountNbr FROM records WHERE accountNbr = ? AND userId = ?";
    
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    sqlite3_bind_int(stmt, 1, accountNbr);
    sqlite3_bind_int(stmt, 2, u.id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        sqlite3_finalize(stmt); 
        const char *deleteQuery = "DELETE FROM records WHERE accountNbr = ? AND userId = ?";
        
        if (sqlite3_prepare_v2(db, deleteQuery, -1, &stmt, NULL) != SQLITE_OK) {
            printf("✖ Failed to prepare delete statement: %s\n", sqlite3_errmsg(db));
            return;
        }

        sqlite3_bind_int(stmt, 1, accountNbr);
        sqlite3_bind_int(stmt, 2, u.id);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("✔ Account number %d deleted successfully.\n", accountNbr);
        } else {
            printf("✖ Failed to delete account: %s\n", sqlite3_errmsg(db));
        }
    } else {
        printf("\n✖ Account not found or does not belong to you.\n");
    }

    sqlite3_finalize(stmt);
    exitOrMenu(u);
}

void transferOwnership(struct User u) {
    int accountNbr; 
    char newOwnerName[50]; 
    sqlite3_stmt *stmt;

    system("clear");
    printf("\t\t\t===== Transfer Ownership =====\n\n");
    printf("Enter the account number you want to transfer: ");
    scanf("%d", &accountNbr);
                clear();

    
    const char *checkQuery = "SELECT userId FROM records WHERE accountNbr = ? AND userId = ?";
    
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_int(stmt, 1, accountNbr);
    sqlite3_bind_int(stmt, 2, u.id);


    if (sqlite3_step(stmt) == SQLITE_ROW) {

        sqlite3_finalize(stmt); 

        printf("Enter the name of the new owner: ");
        scanf("%49s", newOwnerName);
                clear();


        const char *ownerCheckQuery = "SELECT id FROM users WHERE name = ?";

        if (sqlite3_prepare_v2(db, ownerCheckQuery, -1, &stmt, NULL) != SQLITE_OK) {
            printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            return;
        }

           

        sqlite3_bind_text(stmt, 1, newOwnerName, -1, SQLITE_TRANSIENT);


        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int newOwnerId = sqlite3_column_int(stmt, 0); 

            const char *updateQuery = "UPDATE records SET userId = ? WHERE accountNbr = ?";
            sqlite3_finalize(stmt); 

            if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, NULL) != SQLITE_OK) {
                printf("✖ Failed to prepare update statement: %s\n", sqlite3_errmsg(db));
                return;
            }

            sqlite3_bind_int(stmt, 1, newOwnerId);
            sqlite3_bind_int(stmt, 2, accountNbr);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                printf("✔ Ownership of account number %d has been successfully transferred to %s.\n", accountNbr, newOwnerName);
            } else {
                printf("✖ Failed to transfer ownership: %s\n", sqlite3_errmsg(db));
            }
        } else {
            printf("✖ There is no owner with the name'%s' .\n", newOwnerName);
        }
    } else {
        printf("\n✖ Account not found or does not belong to you.\n");
    }

    sqlite3_finalize(stmt);
    exitOrMenu(u);
}
