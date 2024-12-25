#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>



struct Date
{
    int month, day, year;
};

struct Record
{
    int id;
    int userId;
    char name[50];
    int accountNbr;
    char country[20];
    char phone[15];
    char accountType[10];
    double amount;
    struct Date deposit;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

extern sqlite3 *db;
// authentication functions
void loginMenu(char username[50], char password[50]);
int registerMenu(char a[50], char pass[50]);
int getPassword(const char *username, const char *password);

// system function
void trimWhitespace(char *str);
void createNewAcc(struct User u);
void clear();
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
void checkDetailAccount(struct User u);
void updateAccountInfo(struct User u);
void makeTransactions(struct User u);
void deleteAccount(struct User u);
void  transferOwnership(struct User u);
void exitOrMenu(struct User u);
//database 
void initDatabase();