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

    // Create tables
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




void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
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

    // Get deposit date
enterDate:
    printf("\nEnter today's date (mm/dd/yyyy): ");
    scanf("%2d/%2d/%4d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    clear();
    if (r.deposit.month < 1 || r.deposit.month > 12 || r.deposit.day < 1 || r.deposit.day > 31 || r.deposit.year < 1900) {
        printf("✖ Invalid date. Please try again.\n");
        goto enterDate;
    }
printf("%d %d %d",r.deposit.day,r.deposit.month,r.deposit.year);
    // Get account number
enterAccountNumber:
    printf("\nEnter the account number: ");
    scanf("%d", &r.accountNbr);
    clear();
    if (r.accountNbr < 0) {
        printf("✖ Invalid account number. Please try again.\n");
        goto enterAccountNumber;
    }
printf("%d",r.accountNbr);
    // Check if account number exists
const char *checkQuery = "SELECT id FROM records WHERE accountNbr = ?";
if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL) != SQLITE_OK) {
    printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    return;
}

// Bind the account number
sqlite3_bind_int(stmt, 1, r.accountNbr);

// Execute the query
if (sqlite3_step(stmt) == SQLITE_ROW) {
    printf("Account number exists.\n");
}
// Finalize the statement
sqlite3_finalize(stmt);

    // Get other details
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
            goto enterPhoneNumber; // Prompt again for valid input
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

    // Insert the new account
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
                        "FROM records WHERE userId = ?"; // Ensure this matches your records table

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind user ID
    sqlite3_bind_int(stmt, 1, u.id);

    system("clear");
    printf("\t\t====== All Accounts for User: %s =====\n\n", u.name);
    
    // Fetch and display each account
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("_____________________\n");
        int retrievedAccountNbr = sqlite3_column_int(stmt, 0);
        const char *country = (const char *)sqlite3_column_text(stmt, 1);
        const char *phone = (const char *)sqlite3_column_text(stmt, 2); // Changed to text for phone
        double amount = sqlite3_column_double(stmt, 3);
        const char *accountType = (const char *)sqlite3_column_text(stmt, 4);
        int depositMonth = sqlite3_column_int(stmt, 5);
        int depositDay = sqlite3_column_int(stmt, 6);
        int depositYear = sqlite3_column_int(stmt, 7);

        // Print account details
        printf("\nAccount Number: %d\n", retrievedAccountNbr);
        printf("Country: %s\n", country);
        printf("Phone Number: %s\n", phone); // Print phone as string
        printf("Amount Deposited: $%.2f\n", amount);
        printf("Account Type: %s\n", accountType);
        printf("Deposit Date: %02d/%02d/%04d\n", depositMonth, depositDay, depositYear);
    }

    // Finalize statement
    sqlite3_finalize(stmt);
}

void updateAccountInfo(struct User u) {
    int accountNbr; // Change from recordId to accountNbr
    char newCountry[100];
    char newPhone[15]; // Change to string to handle larger phone numbers
    int choice;
    sqlite3_stmt *stmt;

retry:
    system("clear");
    printf("\t\t\t===== Update Account Information =====\n\n");
    printf("\nEnter the account number of the record you want to update: ");
    scanf("%d", &accountNbr);

    // Check if the record exists
    const char *checkQuery = "SELECT accountNbr, phone, country FROM records WHERE accountNbr = ? AND userId = ?";
    
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL) != SQLITE_OK) {
        printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Bind parameters
    sqlite3_bind_int(stmt, 1, accountNbr);
    sqlite3_bind_int(stmt, 2, u.id);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("\n✖ Record not found or does not belong to you.\n");
        sqlite3_finalize(stmt);

        char retryOption;
        printf("\nWould you like to try again? (y/n): ");
        scanf(" %c", &retryOption);

        if (retryOption == 'y' || retryOption == 'Y') {
            goto retry;
        } else {
            printf("\nReturning to main menu...\n");
            return;
        }
    }

    // Ask what to update
    printf("\n✔ Record found!\n");
    printf("\nWhat would you like to update?\n");
    printf("[1] Phone Number\n");
    printf("[2] Country\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("\nEnter new phone number: ");
            scanf("%14s", newPhone); // Read as string
            break;
        case 2:
            printf("\nEnter new country: ");
            scanf("%49s", newCountry);
            break;
        default:
            printf("\n✖ Invalid choice.\n");
            sqlite3_finalize(stmt);
            return;
    }

    // Prepare the update statement
    const char *updateQuery = "UPDATE records SET ";
    
    // Determine which field to update
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

    // Execute the update statement
    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("\n✔ Record information updated successfully!\n");
        success(u);
    } else {
        printf("✖ Failed to update record: %s\n", sqlite3_errmsg(db));
    }
    // Finalize the statement
    sqlite3_finalize(stmt);
}
void checkDetailAccount(struct User u) {
    int accountNbr; // Account number to check
    sqlite3_stmt *stmt;
    char retryOption;

    do {
        system("clear");
        printf("\t\t\t===== Check Account Details =====\n\n");
        printf("Enter the account number you want to check: ");
        scanf("%d", &accountNbr);

        // Query to get account details
        const char *query = "SELECT accountNbr, country, phone, amount, accountType, depositMonth, depositDay, depositYear "
                            "FROM records WHERE accountNbr = ? AND userId = ?";

        if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
            printf("✖ Failed to prepare statement: %s\n", sqlite3_errmsg(db));
            return;
        }

        // Bind parameters
        sqlite3_bind_int(stmt, 1, accountNbr);
        sqlite3_bind_int(stmt, 2, u.id);

        // Execute query and check if record exists
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve values from the result
            int retrievedAccountNbr = sqlite3_column_int(stmt, 0);
            const char *country = (const char *)sqlite3_column_text(stmt, 1);
            const char *phone = (const char *)sqlite3_column_text(stmt, 2); // Changed to text for phone
            double amount = sqlite3_column_double(stmt, 3);
            const char *accountType = (const char *)sqlite3_column_text(stmt, 4);
            int depositMonth = sqlite3_column_int(stmt, 5);
            int depositDay = sqlite3_column_int(stmt, 6);
            int depositYear = sqlite3_column_int(stmt, 7);

            // Print account details
            printf("\nAccount Number: %d\n", retrievedAccountNbr);
            printf("Country: %s\n", country);
            printf("Phone Number: %s\n", phone); // Print phone as string
            printf("Amount Deposited: $%.2f\n", amount);
            printf("Account Type: %s\n", accountType);
            printf("Deposit Date: %02d/%02d/%04d\n", depositMonth, depositDay, depositYear);

            // Calculate interest based on account type
            double interest = 0.0; // Initialize interest variable

            if (strcmp(accountType, "savings") == 0) {
                interest = amount * 0.07; // Savings interest rate of 7%
                printf("You will get $%.2f as interest on day %d of every month.\n", interest / 12.0, depositDay);
            } else if (strcmp(accountType, "fixed01") == 0) {
                interest = amount * 0.04; // Fixed01 interest rate of 4%
                int maturityYear = depositYear + 1; // Maturity after one year
                printf("You will get $%.2f as interest on %02d/%02d/%04d.\n", interest / 12.0 * (12), depositDay,
                       depositMonth,maturityYear); 
            } else if (strcmp(accountType, "fixed02") == 0) {
                interest = amount * 0.05; // Fixed02 interest rate of 5%
                int maturityYear = depositYear + 2; // Maturity after two years
                printf("You will get $%.2f as interest on %02d/%02d/%04d.\n", interest*2 , depositDay,
                       depositMonth,maturityYear); 
            } else if (strcmp(accountType,"fixed03") ==0){
                interest=amount*0.08; // Fixed03 interest rate of 
                int maturityYear=depositYear+3; 
                printf ("You will get $%.2f as interest on %02d/%02d/%04d.\n" ,interest*3,depositDay,
                        depositMonth,maturityYear); 
            }
           else if(strcmp(accountType,"current")==0){
                printf ("You will not get interests because the account is of type current.\n");
            }
        } else {
            printf("\n✖ Account not found or does not belong to you.\n");
        }

        // Finalize statement
        sqlite3_finalize(stmt);

        // Ask user if they want to try again
        printf("\nWould you like to try again? (y/n): ");
        scanf(" %c", &retryOption);

    } while(retryOption == 'y' || retryOption == 'Y'); // Repeat if user wants to retry

    // Option to return to main menu or exit
    int option;
    printf("\nEnter 1 to go to the main menu and 0 to exit: ");
    scanf("%d", &option);

    if(option == 1){
        mainMenu(u); // Assuming you have a mainMenu function defined elsewhere
    } else {
        exit(0); // Exit program
    }
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