#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char username[50], char password[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%49s", username); //Ensure the username buffer is limited to prevent overflow
    clear();
    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%49s", password);
    clear();
    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};
void registerMenu(char a[50], char pass[50])
{
    FILE *fp;
  int maxId = -1; // Initialize to -1 to handle empty files
    int tempId, readId; // Separate variables for reading IDs
    char tempName[50], tempPass[50];
    // Open the users file
    if ((fp = fopen(USERS, "a+")) == NULL)
    {
        perror("Error opening users file");
        exit(1);
    }

    // Find the highest existing ID
    rewind(fp);
    
    while (fscanf(fp, "%d %s %s", &tempId, tempName, tempPass) != EOF) {
        if (tempId > maxId) {
            maxId = tempId; // Update maxId to the highest ID found
        }
    }

    int newId = maxId + 1; // Assign the new ID as one higher than the max ID


    // Collect username
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t   User Registration:");

    printf("\n\n\t\tEnter a username: ");
    scanf("%s", a);

    // Check if the username already exists
    rewind(fp);
    while (fscanf(fp, "%d %s %s", &readId, tempName, tempPass) != EOF)
    {
        if (strcmp(tempName, a) == 0)
        {
            printf("\n✖ Username already exists. Please try again.\n");
            fclose(fp);
            return;
        }
    }

    // Collect password securely
    struct termios oflags, nflags;
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        fclose(fp);
        exit(1);
    }

    printf("\n\n\t\tEnter a password: ");
    scanf("%s", pass);

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        fclose(fp);
        exit(1);
    }

    // Save the new user to the file
    fprintf(fp, "%d %s %s\n", newId, a, pass);

    fclose(fp);

    printf("\n✔ Registration successful! Your ID is %d. You can now log in.\n", newId);
}
int getPassword(const char *username, const char *password)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        perror("Error opening users file");
        return -3;
    }

    // Read the file line by line and compare username and password
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, username) == 0)
        {
            // Username matches, verify password
            fclose(fp);
            if (strcmp(userChecker.password, password) == 0)
            {
                return userChecker.id; // Return the user ID on success
            }
            return -1; // Password mismatch
        }
    }

    fclose(fp);
    return -2; // Username not found
}