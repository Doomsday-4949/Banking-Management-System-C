/*
 * Banking Management System - C Mini Project
 * Concepts: arrays, strings, pointers, dynamic memory, functions, recursion,
 * structures, unions, file handling, macros, command-line arguments.
 * Compile: gcc -std=c99 -Wall -Wextra -pedantic main.c -o bank
 * Run:     ./bank [accounts_file]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define NAME_LEN 60
#define PHONE_LEN 16
#define EMAIL_LEN 60
#define BRANCH_LEN 40
#define DATA_FILE "accounts.dat"
#define TRANSACTION_FILE "transactions.txt"
#define MIN_BALANCE 0.0
#define MAX_INPUT 256

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char branch[BRANCH_LEN];
} Customer;

typedef union {
    char nominee[NAME_LEN];       /* Used for savings accounts */
    char businessName[NAME_LEN];  /* Used for current accounts */
} AccountTag;

typedef struct {
    long accountNumber;
    Customer customer;
    int accountType;              /* 1 = Savings, 2 = Current */
    AccountTag tag;
    double balance;
    int active;
} Account;

typedef struct {
    Account *items;
    size_t count;
    size_t capacity;
} AccountList;

static char accountFile[260] = DATA_FILE;

static void printLine(void) {
    puts("------------------------------------------------------------");
}

/* Recursive search demonstrates recursion and pointer traversal. */
static int findAccountRecursive(const Account *items, int count,
                                long accountNumber) {
    if (count <= 0) return -1;
    if (items[count - 1].accountNumber == accountNumber &&
        items[count - 1].active) return count - 1;
    return findAccountRecursive(items, count - 1, accountNumber);
}

static void readText(const char *prompt, char *buffer, size_t size) {
    size_t len;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(buffer, (int)size, stdin)) {
            buffer[0] = '\0';
            return;
        }
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            return;
        }
        /* Clear remaining input if it did not fit. */
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        if (strlen(buffer) < size - 1) return;
        printf("Input too long. Please try again (maximum %lu characters).\n",
               (unsigned long)(size - 1));
    }
}

static int readInt(const char *prompt, int min, int max) {
    char input[MAX_INPUT], *end;
    long value;
    for (;;) {
        readText(prompt, input, sizeof(input));
        errno = 0;
        value = strtol(input, &end, 10);
        while (*end == ' ' || *end == '\t') end++;
        if (input[0] != '\0' && *end == '\0' && errno == 0 &&
            value >= min && value <= max) return (int)value;
        printf("Enter a valid number between %d and %d.\n", min, max);
    }
}

static long readLong(const char *prompt, long min, long max) {
    char input[MAX_INPUT], *end;
    long value;
    for (;;) {
        readText(prompt, input, sizeof(input));
        errno = 0;
        value = strtol(input, &end, 10);
        while (*end == ' ' || *end == '\t') end++;
        if (input[0] != '\0' && *end == '\0' && errno == 0 &&
            value >= min && value <= max) return value;
        printf("Enter a valid number between %ld and %ld.\n", min, max);
    }
}

static double readAmount(const char *prompt, double min) {
    char input[MAX_INPUT], *end;
    double value;
    for (;;) {
        readText(prompt, input, sizeof(input));
        errno = 0;
        value = strtod(input, &end);
        while (*end == ' ' || *end == '\t') end++;
        if (input[0] != '\0' && *end == '\0' && errno == 0 &&
            value >= min && value < 1000000000000.0) return value;
        printf("Enter a valid amount (minimum %.2f).\n", min);
    }
}

static void copyText(char *destination, size_t destinationSize, const char *source) {
    if (destinationSize == 0) return;
    strncpy(destination, source, destinationSize - 1);
    destination[destinationSize - 1] = '\0';
}

static void pauseScreen(void) {
    char temp[MAX_INPUT];
    readText("\nPress Enter to continue...", temp, sizeof(temp));
}

static int reserveAccounts(AccountList *list, size_t needed) {
    if (needed <= list->capacity) return 1;
    size_t newCapacity = list->capacity ? list->capacity * 2 : 10;
    while (newCapacity < needed) newCapacity *= 2;
    Account *newItems = (Account *)realloc(list->items,
                                           newCapacity * sizeof(Account));
    if (!newItems) {
        puts("Memory allocation failed.");
        return 0;
    }
    list->items = newItems;
    list->capacity = newCapacity;
    return 1;
}

static int loadAccounts(AccountList *list) {
    FILE *fp = fopen(accountFile, "rb");
    if (!fp) {
        if (errno == ENOENT) return 1; /* First run: no data file yet. */
        perror("Could not open account file");
        return 0;
    }

    Account temp;
    while (fread(&temp, sizeof(Account), 1, fp) == 1) {
        if (!reserveAccounts(list, list->count + 1)) {
            fclose(fp);
            return 0;
        }
        list->items[list->count++] = temp;
    }
    if (ferror(fp)) {
        perror("Error reading account file");
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

static int saveAccounts(const AccountList *list) {
    FILE *fp = fopen(accountFile, "wb");
    if (!fp) {
        perror("Could not save account file");
        return 0;
    }
    for (size_t i = 0; i < list->count; i++) {
        if (fwrite(&list->items[i], sizeof(Account), 1, fp) != 1) {
            perror("Error writing account file");
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

static int getAccountIndex(const AccountList *list, long number) {
    if (list->count > 2147483647U) return -1;
    return findAccountRecursive(list->items, (int)list->count, number);
}

static long generateAccountNumber(const AccountList *list) {
    long candidate = 100000L;
    for (;;) {
        int exists = 0;
        for (size_t i = 0; i < list->count; i++) {
            if (list->items[i].accountNumber == candidate) {
                exists = 1;
                break;
            }
        }
        if (!exists) return candidate;
        candidate++;
    }
}

static void logTransaction(long accountNumber, const char *type,
                           double amount, double balance, long related) {
    FILE *fp = fopen(TRANSACTION_FILE, "a");
    if (!fp) {
        perror("Could not write transaction history");
        return;
    }
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    char stamp[32] = "Unknown time";
    if (local) strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", local);
    fprintf(fp, "%s | Account: %ld | %-12s | Amount: %.2f | Balance: %.2f",
            stamp, accountNumber, type, amount, balance);
    if (related > 0) fprintf(fp, " | Related account: %ld", related);
    fputc('\n', fp);
    fclose(fp);
}

static void showAccount(const Account *a) {
    printLine();
    printf("Account number : %ld\n", a->accountNumber);
    printf("Customer name  : %s\n", a->customer.name);
    printf("Phone          : %s\n", a->customer.phone);
    printf("Email          : %s\n", a->customer.email);
    printf("Branch         : %s\n", a->customer.branch);
    printf("Account type   : %s\n", a->accountType == 1 ? "Savings" : "Current");
    if (a->accountType == 1)
        printf("Nominee        : %s\n", a->tag.nominee);
    else
        printf("Business name  : %s\n", a->tag.businessName);
    printf("Balance        : Rs. %.2f\n", a->balance);
    printLine();
}

static void createAccount(AccountList *list) {
    Account a;
    memset(&a, 0, sizeof(a));
    a.accountNumber = generateAccountNumber(list);
    a.active = 1;

    puts("\n--- CREATE NEW ACCOUNT ---");
    readText("Customer name: ", a.customer.name, sizeof(a.customer.name));
    readText("Phone number: ", a.customer.phone, sizeof(a.customer.phone));
    readText("Email address: ", a.customer.email, sizeof(a.customer.email));
    readText("Branch name: ", a.customer.branch, sizeof(a.customer.branch));
    a.accountType = readInt("Account type (1-Savings, 2-Current): ", 1, 2);

    if (a.accountType == 1)
        readText("Nominee name: ", a.tag.nominee, sizeof(a.tag.nominee));
    else
        readText("Business name: ", a.tag.businessName,
                 sizeof(a.tag.businessName));

    a.balance = readAmount("Opening deposit (minimum Rs. 500): ", 500.0);

    if (!reserveAccounts(list, list->count + 1)) return;
    list->items[list->count++] = a;
    if (!saveAccounts(list)) {
        list->count--;
        return;
    }
    logTransaction(a.accountNumber, "OPENING", a.balance, a.balance, 0);
    printf("\nAccount created successfully!\nYour account number is: %ld\n",
           a.accountNumber);
}

static void depositMoney(AccountList *list) {
    puts("\n--- DEPOSIT MONEY ---");
    long number = readLong("Enter account number: ", 1, 999999999L);
    int index = getAccountIndex(list, number);
    if (index < 0) { puts("Account not found."); return; }

    double amount = readAmount("Enter deposit amount: ", 0.01);
    list->items[index].balance += amount;
    if (!saveAccounts(list)) {
        list->items[index].balance -= amount;
        return;
    }
    logTransaction(number, "DEPOSIT", amount, list->items[index].balance, 0);
    printf("Deposit successful. New balance: Rs. %.2f\n",
           list->items[index].balance);
}

static void withdrawMoney(AccountList *list) {
    puts("\n--- WITHDRAW MONEY ---");
    long number = readLong("Enter account number: ", 1, 999999999L);
    int index = getAccountIndex(list, number);
    if (index < 0) { puts("Account not found."); return; }

    double amount = readAmount("Enter withdrawal amount: ", 0.01);
    if (amount > list->items[index].balance) {
        puts("Insufficient balance.");
        return;
    }
    list->items[index].balance -= amount;
    if (!saveAccounts(list)) {
        list->items[index].balance += amount;
        return;
    }
    logTransaction(number, "WITHDRAWAL", amount, list->items[index].balance, 0);
    printf("Withdrawal successful. Remaining balance: Rs. %.2f\n",
           list->items[index].balance);
}

static void transferMoney(AccountList *list) {
    puts("\n--- TRANSFER MONEY ---");
    long from = readLong("From account number: ", 1, 999999999L);
    long to = readLong("To account number: ", 1, 999999999L);
    int source = getAccountIndex(list, from);
    int destination = getAccountIndex(list, to);

    if (source < 0 || destination < 0) {
        puts("One or both accounts were not found.");
        return;
    }
    if (source == destination) {
        puts("Source and destination accounts must be different.");
        return;
    }

    double amount = readAmount("Enter transfer amount: ", 0.01);
    if (amount > list->items[source].balance) {
        puts("Insufficient balance.");
        return;
    }

    list->items[source].balance -= amount;
    list->items[destination].balance += amount;
    if (!saveAccounts(list)) {
        list->items[source].balance += amount;
        list->items[destination].balance -= amount;
        return;
    }
    logTransaction(from, "TRANSFER OUT", amount, list->items[source].balance, to);
    logTransaction(to, "TRANSFER IN", amount, list->items[destination].balance, from);
    puts("Transfer completed successfully.");
}

static void searchAccount(const AccountList *list) {
    puts("\n--- SEARCH ACCOUNT ---");
    long number = readLong("Enter account number: ", 1, 999999999L);
    int index = getAccountIndex(list, number);
    if (index < 0) puts("Account not found.");
    else showAccount(&list->items[index]);
}

static void displayAllAccounts(const AccountList *list) {
    puts("\n--- ALL BANK ACCOUNTS ---");
    int found = 0;
    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].active) {
            showAccount(&list->items[i]);
            found = 1;
        }
    }
    if (!found) puts("No accounts are currently available.");
}

static void updateAccount(AccountList *list) {
    puts("\n--- UPDATE CUSTOMER DETAILS ---");
    long number = readLong("Enter account number: ", 1, 999999999L);
    int index = getAccountIndex(list, number);
    if (index < 0) { puts("Account not found."); return; }

    Account old = list->items[index];
    puts("Leave a field blank to keep its current value.");
    char input[MAX_INPUT];

    printf("Current name: %s\nNew name: ", old.customer.name);
    readText("", input, sizeof(input));
    if (input[0]) copyText(list->items[index].customer.name, NAME_LEN, input);

    printf("Current phone: %s\nNew phone: ", old.customer.phone);
    readText("", input, sizeof(input));
    if (input[0]) copyText(list->items[index].customer.phone, PHONE_LEN, input);

    printf("Current email: %s\nNew email: ", old.customer.email);
    readText("", input, sizeof(input));
    if (input[0]) copyText(list->items[index].customer.email, EMAIL_LEN, input);

    printf("Current branch: %s\nNew branch: ", old.customer.branch);
    readText("", input, sizeof(input));
    if (input[0]) copyText(list->items[index].customer.branch, BRANCH_LEN, input);

    if (!saveAccounts(list)) list->items[index] = old;
    else puts("Customer details updated successfully.");
}

static void deleteAccount(AccountList *list) {
    puts("\n--- CLOSE ACCOUNT ---");
    long number = readLong("Enter account number: ", 1, 999999999L);
    int index = getAccountIndex(list, number);
    if (index < 0) { puts("Account not found."); return; }

    showAccount(&list->items[index]);
    if (list->items[index].balance > MIN_BALANCE) {
        puts("Account cannot be closed while it has a balance.");
        puts("Withdraw or transfer the remaining money first.");
        return;
    }
    char answer[MAX_INPUT];
    readText("Confirm account closure (yes/no): ", answer, sizeof(answer));
    if (strcmp(answer, "yes") != 0 && strcmp(answer, "YES") != 0) {
        puts("Account closure cancelled.");
        return;
    }

    Account old = list->items[index];
    list->items[index].active = 0;
    if (!saveAccounts(list)) list->items[index] = old;
    else {
        logTransaction(number, "CLOSED", 0.0, 0.0, 0);
        puts("Account closed successfully.");
    }
}

static void viewTransactions(void) {
    puts("\n--- TRANSACTION HISTORY ---");
    FILE *fp = fopen(TRANSACTION_FILE, "r");
    if (!fp) {
        puts("No transaction history found yet.");
        return;
    }
    char line[300];
    while (fgets(line, sizeof(line), fp)) fputs(line, stdout);
    fclose(fp);
}

static void showMenu(void) {
    printLine();
    puts("             BANKING MANAGEMENT SYSTEM");
    printLine();
    puts("1.  Create new account");
    puts("2.  Deposit money");
    puts("3.  Withdraw money");
    puts("4.  Transfer money");
    puts("5.  Search account");
    puts("6.  Display all accounts");
    puts("7.  Update customer details");
    puts("8.  Close account");
    puts("9.  View transaction history");
    puts("0.  Exit");
    printLine();
}

int main(int argc, char *argv[]) {
    AccountList accounts = {NULL, 0, 0};

    /* Command-line argument: optional custom account database filename. */
    if (argc > 1) {
        if (strlen(argv[1]) >= sizeof(accountFile)) {
            fprintf(stderr, "Data filename is too long.\n");
            return EXIT_FAILURE;
        }
        strcpy(accountFile, argv[1]);
    }

    if (!loadAccounts(&accounts)) {
        free(accounts.items);
        return EXIT_FAILURE;
    }

    puts("Welcome to the Banking Management System!");
    printf("Account data file: %s\n", accountFile);

    int choice;
    do {
        showMenu();
        choice = readInt("Enter your choice: ", 0, 9);
        switch (choice) {
            case 1: createAccount(&accounts); break;
            case 2: depositMoney(&accounts); break;
            case 3: withdrawMoney(&accounts); break;
            case 4: transferMoney(&accounts); break;
            case 5: searchAccount(&accounts); break;
            case 6: displayAllAccounts(&accounts); break;
            case 7: updateAccount(&accounts); break;
            case 8: deleteAccount(&accounts); break;
            case 9: viewTransactions(); break;
            case 0: puts("Thank you for using our banking system."); break;
        }
        if (choice != 0) pauseScreen();
    } while (choice != 0);

    free(accounts.items);
    return EXIT_SUCCESS;
}
