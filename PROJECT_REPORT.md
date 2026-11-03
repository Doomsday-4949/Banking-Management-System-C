# BANKING MANAGEMENT SYSTEM
## C Language Mini Project Report

**Student Name:** __________________________  
**Roll Number:** ____________________________  
**Course / Section:** ________________________  
**Subject:** C Programming  
**Submitted To:** ___________________________  
**Academic Session:** _______________________

---

## 1. Introduction

The Banking Management System is a menu-driven console application developed using the C programming language. It helps demonstrate how basic banking activities can be represented through a computer program. The user can create accounts, deposit and withdraw money, transfer funds, search account details, update customer information and view transaction records.

The project applies the topics taught in the C programming syllabus, including decision-making, loops, arrays, strings, pointers, dynamic memory allocation, functions, recursion, structures, unions and file handling.

## 2. Objectives

- To understand the practical use of C programming concepts.
- To maintain basic customer account records.
- To perform deposit, withdrawal and fund-transfer operations.
- To search, display, update and close accounts.
- To store records permanently using files.
- To develop logical thinking and problem-solving skills.

## 3. Software and Hardware Requirements

**Software**
- C compiler such as GCC / MinGW
- Code editor or IDE such as VS Code, Code::Blocks or Dev-C++
- Windows, Linux or macOS

**Hardware**
- Computer or laptop
- Basic keyboard and display

## 4. Main Modules

1. **Create Account:** Accepts customer information, account type and opening deposit. A unique account number is generated.
2. **Deposit:** Adds the entered amount to an existing account balance.
3. **Withdraw:** Deducts money only when sufficient balance is available.
4. **Transfer:** Moves money between two different existing accounts.
5. **Search:** Finds an account using its account number.
6. **Display:** Shows all active account records.
7. **Update:** Changes customer contact details.
8. **Close Account:** Closes an account only when its balance is zero.
9. **Transaction History:** Displays recorded account activities.

## 5. Concepts of C Used

- **Variables and data types:** Store account numbers, names, balances and menu choices.
- **Operators:** Perform calculations and comparisons.
- **Decision statements:** Validate transactions and select menu operations.
- **Loops:** Repeat the menu and traverse account records.
- **Arrays and strings:** Store names, phone numbers, email addresses and account collections.
- **Pointers:** Access account records and manage dynamically allocated memory.
- **Dynamic memory allocation:** `realloc()` expands the account list and `free()` releases memory.
- **Functions:** Divide the application into smaller modules.
- **Recursion:** Search account records using a recursive function.
- **Structures:** Group related customer and account information.
- **Union:** Store either a nominee name or business name in the account tag.
- **File handling:** Save account data and append transaction history.
- **Preprocessor directives:** Use header files and macros.

## 6. Algorithm

1. Start the program.
2. Load existing account records from the data file.
3. Display the main menu.
4. Accept the user's choice.
5. Execute the selected operation.
6. Validate the account number and amount.
7. Save modified account data to the file.
8. Record successful transactions in the history file.
9. Return to the menu.
10. Stop when the user selects Exit and release allocated memory.

## 7. Sample Menu

```text
------------------------------------------------------------
             BANKING MANAGEMENT SYSTEM
------------------------------------------------------------
1. Create new account
2. Deposit money
3. Withdraw money
4. Transfer money
5. Search account
6. Display all accounts
7. Update customer details
8. Close account
9. View transaction history
0. Exit
------------------------------------------------------------
Enter your choice:
```

## 8. File Handling

The project uses two files:
- `accounts.dat` stores account structures in binary format.
- `transactions.txt` stores transaction entries in readable text format.

This allows account information to remain available after the program is closed and started again.

## 9. Testing

| Test | Expected result |
|---|---|
| Create account with valid details | Account is created |
| Deposit a positive amount | Balance increases |
| Withdraw within available balance | Balance decreases |
| Withdraw more than available balance | Insufficient balance message |
| Transfer between valid accounts | Both balances update |
| Search an existing account | Account details appear |
| Search an unknown account | Account not found |
| Close account with non-zero balance | Closure is rejected |
| Restart the application | Saved accounts load again |

## 10. Conclusion

The Banking Management System demonstrates how the fundamentals of C programming can be combined to create a useful menu-driven application. It provides practice in modular programming, data organization, input validation, memory management and persistent file storage. The project can be extended in the future with login authentication, PIN protection, interest calculation, account statements and a graphical interface.

## 11. Future Scope

- Add secure customer login and PIN verification.
- Add savings interest calculation.
- Generate account statements for a selected account.
- Add date-wise transaction search.
- Replace binary files with a database.
- Develop a graphical user interface.

## 12. References

- C programming class notes and syllabus.
- C standard library documentation.
- GCC compiler documentation.
