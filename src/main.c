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

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        // here
        updateAccountInfo(u);
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        checkDetailAccount(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        // here
        makeTransactions(u);
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        printf("\nExiting... Goodbye!\n");
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
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
            // Clear invalid input
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
                u->id = userId; // Save the logged-in user's ID
                printf("\n✔ Login successful! Welcome, %s.\n", u->name);
                printf("\nPress Enter to continue...");
                getchar();
                running = 0; // Exit the loop to proceed to mainMenu
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
            // student TODO : add your **Registration** function
            // here
             int registeredId= registerMenu(u->name, u->password);
           if (registeredId > 0)
    {
        u->id = registeredId; // Save the new user's ID
         printf("\n✔ Registration successful!\n");
         printf("\nPress Enter to continue...");
         while (getchar() != '\n');
        getchar();
        running = 0; // Exit the loop to proceed to mainMenu
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
    if (u.id > 0) // Only proceed to mainMenu if a user is logged in
    {
        mainMenu(u);
    }
    
    return 0;
}
