#include "header.h"
sqlite3 *db;
void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    printf("\n\t\tEnter your choice: ");
    scanf("%d", &option);
    clear();
    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccountInfo(u);
        break;
    case 3:
        checkDetailAccount(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransactions(u);
        break;
    case 6:
        deleteAccount(u);
        break;
    case 7:
        transferOwnership(u);
        break;
    case 8:
        printf("\nExiting... Goodbye!\n");
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
        exitOrMenu(u);
    }
};

void initMenu(struct User *u)
{
    int running = 1;
    int option;
    while (running)
    {

    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    
        printf("\nChoose an option: ");
        if (scanf("%d", &option) != 1)
        {
            while (getchar() != '\n');
            printf("✖ Invalid input! Please enter a number.\n");
            printf("\nPress Enter to continue...");
            getchar();
            continue;
        }
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            int userId = getPassword(u->name, u->password);
            if (userId > 0)
            {
                // Save the logged-in user's ID
                u->id = userId; 
                printf("\n✔ Login successful! Welcome, %s.\n", u->name);
                printf("\nPress Enter to continue...");
                getchar();
                running = 0;
            }
            else if (userId ==-1)
            {
                printf("\nWrong password!! \n");
                printf("\nPress Enter to try again...");
                getchar();
               
            }else if(userId==-2) {

                printf("\nUser not found!! \n");
                printf("\nPress Enter to try again...");
                getchar();

                }else {
                printf("\nAn error occurred while accessing the database  \n");
                printf("\nPress Enter to try again...");
                getchar();
            }
           
            break;
        case 2:
             int registeredId= registerMenu(u->name, u->password);
           if (registeredId > 0)
    {
        // Save the new user's ID
        u->id = registeredId; 
         printf("\n✔ Registration successful!\n");
         printf("\nPress Enter to continue...");
         while (getchar() != '\n');
        getchar();
        running = 0;
    }
    break;
        case 3:
            printf("\nExiting... Goodbye!\n");
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
           
        }
    }
};

int main()
{
    struct User u;
    initDatabase();
    initMenu(&u);
    if (u.id > 0)
    {
        mainMenu(u);
    }
    
    return 0;
}
