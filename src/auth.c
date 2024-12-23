#include <termios.h>
#include "header.h"
#include <sqlite3.h>



void loginMenu(char username[50], char password[50]) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *sql = "SELECT password FROM users WHERE name = ?;";
    const char *dbPassword = NULL;

    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t\n");
    printf("Enter your username: ");
    scanf("%49s", username); // Prevent buffer overflow
    clear(); // Clear the screen for better UX

    // Trim the input username
    trimWhitespace(username);

    // Open the database
    if (sqlite3_open("data/database.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Prepare the SQL statement to fetch the password
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error preparing SQL query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Bind the username parameter to the query
    if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Error binding parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    // Execute the query and check for results
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        dbPassword = (const char *)sqlite3_column_text(stmt, 0); // Get the password from the database
        // Trim the dbPassword
        trimWhitespace((char *)dbPassword);
    } else {
        printf("✖ Username not found.\n");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return; // Username not found, exit the function
    }

    sqlite3_finalize(stmt);

    // Disable terminal echo for password input
    if (tcgetattr(fileno(stdin), &oflags) != 0) {
        perror("tcgetattr");
        exit(1);
    }

    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    printf("Enter your password: ");
    scanf("%49s", password); // Secure password input
    clear(); // Clear screen after password entry

    // Restore terminal settings
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    // Check if the entered password matches the one in the database
    if (dbPassword != NULL && strcmp(password, dbPassword) == 0) {
        printf("✔ Login successful!\n");
    }
    // } else {
    //     printf("✖ Incorrect password. Please try again.\n");
    // }

    sqlite3_close(db);
}
int registerMenu(char a[50], char pass[50])
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int newId = -1;
system("clear");
printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t    User Registration");
    // Open the database connection
    if (sqlite3_open("data/database.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    // Check if the username already exists
    const char *checkQuery = "SELECT id FROM users WHERE name = ?;";
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

retry_username:
    printf("\n\n\t\tEnter a username: ");
    scanf("%49s", a);
    clear();
    // Bind the username to the query
    if (sqlite3_bind_text(stmt, 1, a, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameters: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("\n✖ Username already exists. Please try again.\n");
        sqlite3_reset(stmt); // Reset the statement for reuse
        goto retry_username;
    }

    sqlite3_finalize(stmt); // Finalize the statement after use

    // Collect password securely
    struct termios oflags, nflags;
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        sqlite3_close(db);
        exit(1);
    }

    printf("\n\n\t\tEnter a password: ");
    scanf("%49s", pass);
    clear();
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        sqlite3_close(db);
        exit(1);
    }

    // Generate a new ID by finding the maximum ID
    const char *idQuery = "SELECT MAX(id) FROM users;";
    if (sqlite3_prepare_v2(db, idQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        newId = sqlite3_column_int(stmt, 0) + 1; // Increment max ID by 1
    }
    else
    {
        newId = 1; // Default to 1 if no users exist
    }

    sqlite3_finalize(stmt);

    // Insert the new user into the database
    const char *insertQuery = "INSERT INTO users (id, name, password) VALUES (?, ?, ?);";
    if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing insert query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    // Bind the values to the query
    if (sqlite3_bind_int(stmt, 1, newId) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, a, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, pass, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameters: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    // Execute the insert query
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Error inserting user: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    printf("\n✔ Registration successful! Your ID is %d. You can now log in.\n", newId);
    return newId;
}
int getPassword(const char *username, const char *password)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int result = -2; // Default return value for "username not found"
    const char *query = "SELECT id, password FROM users WHERE name = ?;";

    // Open the database connection
    if (sqlite3_open("./data/database.db", &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return -3; // Error code for database access issues
    }

    // Prepare the SQL query
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -3;
    }

    // Bind the username to the query
    if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) != SQLITE_OK)
    {
        fprintf(stderr, "Error binding parameters: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -3;
    }

    // Execute the query and check results
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *dbPassword = (const char *)sqlite3_column_text(stmt, 1);
        int userId = sqlite3_column_int(stmt, 0);

        if (strcmp(password, dbPassword) == 0)
        {
            result = userId; // Return the user ID on success
        }
        else
        {
            result = -1; // Password mismatch
        }
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result; // -2 if username not found, -1 if password mismatch, or user ID on success
}