#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{

    // Rewind the file to find the highest record ID
    rewind(ptr);
    struct Record tempRecord;
    char tempUserName[50];
    int maxRecordID = -1;

    while (getAccountFromFile(ptr, tempUserName, &tempRecord))
    {
        if (tempRecord.id > maxRecordID)
        {
            maxRecordID = tempRecord.id;
        }
    }

    // Assign the next record ID
    r.id = maxRecordID + 1;

    // User ID is already set in the current user structure
    r.userId = u.id;
    // Writing the data directly (removed & operator for struct members)
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id,               // Record ID
            u.id,               // User ID
            u.name,             // User Name
            r.accountNbr,       // Account Number
            r.deposit.month,    // Deposit Date: Month
            r.deposit.day,      // Deposit Date: Day
            r.deposit.year,     // Deposit Date: Year
            r.country,          // Country
            r.phone,            // Phone Number
            r.amount,           // Amount
            r.accountType);     // Account Type
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

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");
    if (!pf)
    {
        perror("Failed to open file");
        return;
    }
 
    system("clear");
    printf("\t\t\t===== New record =====\n");

enterDate:
    printf("\nEnter today's date(mm/dd/yyyy):");
    int validDate=scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    clear();  // to clear the newline character in input buffer
    if(validDate !=3||r.deposit.month<1||r.deposit.month>12||
    r.deposit.day<1||r.deposit.day>31||r.deposit.year<2023)
    {
        printf("✖ Invalid date. Please try again.\n");
        goto enterDate;
    }
enterAccountNumber :
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);
      clear();
    if(r.accountNbr<0){
        printf("✖ Invalid account number. Please try again.\n");
        goto enterAccountNumber;
    }
rewind(pf);
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (cr.accountNbr == r.accountNbr)
        {
            printf("✖ This account number already exists for this user\n\n");
            goto enterAccountNumber;
        }
    }

enterCountry:

    printf("\nEnter the country:");
    scanf("%s", r.country);
    clear();

enterPhoneNumber:

    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    clear();

    if(r.phone<0){
        printf("✖ Invalid phone number. Please try again.\n");
        goto enterPhoneNumber;
    }


enterAmount:

    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    clear();
    if(r.amount<0){
        printf("✖ Invalid amount. Please try again.\n");
        goto enterAmount;
    }

enterAccountType:
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    clear();
    if (strcmp(r.accountType, "saving") != 0 && strcmp(r.accountType, "current") != 0 &&
        strcmp(r.accountType, "fixed01") != 0 && strcmp(r.accountType, "fixed02") != 0 &&
        strcmp(r.accountType, "fixed03") != 0)
    {
        printf("✖ Invalid account type. Please try again.\n");
        goto enterAccountType;
    }

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}


void updateAccountInfo(struct User u){

}

void checkDetailAccount(struct User u){

}