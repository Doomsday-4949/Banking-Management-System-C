# Banking Management System (C Language)

A console-based college mini project written in standard C. It is designed around the supplied C programming syllabus and uses only the C standard library.

## Features

1. Create savings or current accounts
2. Deposit money
3. Withdraw money
4. Transfer money between accounts
5. Search for an account
6. Display all active accounts
7. Update customer contact details
8. Close an account (only when its balance is zero)
9. View transaction history
10. Save account records between program runs
    ![Banking Management System](Screenshot%202026-11-03%20131956.png)
    ![Banking System Output](Screenshot%202026-11-03%20132044.png)

## Files in this project

- `main.c` — complete C source code
- `Makefile` — build instructions for GCC
- `README.md` — project overview, syllabus mapping and usage
- `PROJECT_REPORT.md` — ready-to-edit college project report
- `.gitignore` — ignores generated executable and data files

## Requirements

- GCC compiler (recommended), or another C99-compatible compiler
- Windows: MinGW-w64 / Code::Blocks / Dev-C++
- Linux/macOS: GCC or Clang

## Compile and run

### Linux / macOS

```bash
gcc -std=c99 -Wall -Wextra -pedantic main.c -o bank
./bank
```

### Windows (MinGW)

```bat
gcc -std=c99 -Wall -Wextra -pedantic main.c -o bank.exe
bank.exe
```

You may optionally provide a different account database filename:

```bash
./bank college_accounts.dat
```

## Data storage

- `accounts.dat` is created automatically and stores account records in binary format.
- `transactions.txt` stores a readable transaction log.
- Keep these files in the same working directory as the program if you want to retain your data.
- Do not manually edit `accounts.dat`.

**Important:** Binary structure files are intended for this learning project and the same compiled structure layout. They are not a production banking database.

## Syllabus mapping

| Syllabus topic | Where it is used |
|---|---|
| C program structure, compilation and execution | `main()` and compiler commands |
| Constants and variables | `#define` macros and local variables |
| Data types | `int`, `long`, `double`, `char`, `size_t`, `time_t` |
| Input and output | `printf()`, `puts()`, `fgets()` |
| Operators and type conversion | Arithmetic, comparison, logical and assignment operators |
| Decision control | `if`, `else`, `switch` |
| Looping constructs | `for`, `while`, `do-while` |
| One-dimensional arrays | Character arrays for names and input |
| Array traversal | Loops over the account list |
| Searching | Recursive account-number search |
| Strings | `strlen()`, `strcmp()`, `strcpy()`, `snprintf()` |
| Pointers | Account pointers, array pointers, file pointers |
| Dynamic memory allocation | `realloc()` and `free()` |
| Functions | Separate functions for each banking operation |
| Recursion | `findAccountRecursive()` |
| Structures | `Customer`, `Account`, `AccountList` |
| Unions | `AccountTag` stores nominee or business name |
| File operations | `fopen()`, `fread()`, `fwrite()`, `fprintf()`, `fgets()`, `fclose()` |
| Preprocessor directives | Header files and `#define` macros |
| Command-line arguments | Optional data filename in `argv[1]` |

## Suggested demonstration for practical / viva

1. Run the program and create an account.
2. Note the generated account number.
3. Deposit money into the account.
4. Create a second account and transfer money to it.
5. Search for an account and display all accounts.
6. Update a customer's details.
7. Show the transaction history.
8. Exit and run the program again to demonstrate permanent storage.

## Limitations and academic note

This is an educational console project, not real banking software. It does not implement authentication, encryption, multi-user access, interest rules, regulatory checks or production-grade database transactions. Do not store real customer or financial information in it.
