#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

const int MAX_ACCOUNTS = 100;

class Account {
protected:
    string accountNumber;
    string name;
    double balance;
    string pinCode;

public:
    Account() {
        accountNumber = "";
        name = "";
        balance = 0.0;
        pinCode = "";
    }

    Account(string accNum, string accName, double accBalance, string accPinCode) {
        accountNumber = accNum;
        name = accName;
        balance = accBalance;
        pinCode = accPinCode;
    }

    virtual void displayAccountDetails() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Name: " << name << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    string getName() const {
        return name;
    }

    double getBalance() const {
        return balance;
    }

    string getPinCode() const {
        return pinCode;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Amount deposited: $" << fixed << setprecision(2) << amount << endl;
        cout << "New balance: $" << fixed << setprecision(2) << balance << endl;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance! Current balance: $" << fixed << setprecision(2) << balance << endl;
            return false;
        }
        balance -= amount;
        cout << "Amount withdrawn: $" << fixed << setprecision(2) << amount << endl;
        cout << "New balance: $" << fixed << setprecision(2) << balance << endl;
        return true;
    }

    bool payUtilityBill(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance to pay utility bill! Current balance: $" << fixed << setprecision(2) << balance << endl;
            return false;
        }
        balance -= amount;
        cout << "Utility bill of $" << fixed << setprecision(2) << amount << " paid successfully!" << endl;
        cout << "New balance: $" << fixed << setprecision(2) << balance << endl;
        return true;
    }

    void reviewBank() const {
        cout << "Bank Review: This bank provides excellent services with a user-friendly interface." << endl;
    }

    void reportBank() const {
        cout << "Bank Report: The bank has maintained a good track record and ensures the safety and security of its customers." << endl;
    }
};

class SavingsAccount : public Account {
public:
    SavingsAccount() : Account() {}

    SavingsAccount(string accNum, string accName, double accBalance, string accPinCode)
        : Account(accNum, accName, accBalance, accPinCode) {}

    void displayAccountDetails() const override {
        cout << "Savings Account Details:" << endl;
        Account::displayAccountDetails();
    }
};

class CurrentAccount : public Account {
public:
    CurrentAccount() : Account() {}

    CurrentAccount(string accNum, string accName, double accBalance, string accPinCode)
        : Account(accNum, accName, accBalance, accPinCode) {}

    void displayAccountDetails() const override {
        cout << "Current Account Details:" << endl;
        Account::displayAccountDetails();
    }
};

class Bank {
private:
    Account* accounts[MAX_ACCOUNTS];
    int numAccounts;

public:
    Bank() {
        numAccounts = 0;
    }

    ~Bank() {
        for (int i = 0; i < numAccounts; ++i) {
            delete accounts[i];
        }
    }

    bool isAccountNumberUnique(string accNum) {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getAccountNumber() == accNum) {
                return false;
            }
        }
        return true;
    }

    void registerAccount(string type, string accNum, string accName, double accBalance, string accPinCode) {
        if (!isAccountNumberUnique(accNum)) {
            cout << "Account with number " << accNum << " already exists.\n";
            return;
        }
        if (numAccounts < MAX_ACCOUNTS) {
            if (type == "savings") {
                accounts[numAccounts++] = new SavingsAccount(accNum, accName, accBalance, accPinCode);
            } else if (type == "current") {
                accounts[numAccounts++] = new CurrentAccount(accNum, accName, accBalance, accPinCode);
            }
            saveAccountsToFile();
            cout << "Account successfully registered." << endl;
        } else {
            cout << "Bank is full, cannot register more accounts.\n";
        }
    }

    int login(string accNum, string accPinCode) const {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getAccountNumber() == accNum && accounts[i]->getPinCode() == accPinCode) {
                return i;
            }
        }
        return -1;
    }

    void displayAllAccounts() const {
        for (int i = 0; i < numAccounts; ++i) {
            accounts[i]->displayAccountDetails();
            cout << "---------------------" << endl;
        }
    }

    void saveAccountsToFile() const {
        ofstream outFile("accounts.txt");
        for (int i = 0; i < numAccounts; ++i) {
            outFile << accounts[i]->getAccountNumber() << " " << accounts[i]->getName() << " "
                    << accounts[i]->getBalance() << " " << accounts[i]->getPinCode() << endl;
        }
        outFile.close();
    }

    void loadAccountsFromFile() {
        ifstream inFile("accounts.txt");
        string accNum, accName, accPinCode;
        double accBalance;
        numAccounts = 0;
        while (inFile >> accNum >> accName >> accBalance >> accPinCode) {
            if (numAccounts < MAX_ACCOUNTS) {
                accounts[numAccounts++] = new SavingsAccount(accNum, accName, accBalance, accPinCode);
            }
        }
        inFile.close();
    }

    void deleteAccount(string accNum, string accPinCode) {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getAccountNumber() == accNum && accounts[i]->getPinCode() == accPinCode) {
                delete accounts[i];
                for (int j = i; j < numAccounts - 1; ++j) {
                    accounts[j] = accounts[j + 1];
                }
                --numAccounts;
                saveAccountsToFile();
                cout << "Account successfully deleted." << endl;
                return;
            }
        }
        cout << "Account not found or invalid PIN code." << endl;
    }

    bool transferMoney(int fromAccIndex, string toAccNum, double amount) {
        int toAccIndex = -1;
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getAccountNumber() == toAccNum) {
                toAccIndex = i;
                break;
            }
        }
        if (fromAccIndex >= 0 && toAccIndex >= 0 && accounts[fromAccIndex]->withdraw(amount)) {
            accounts[toAccIndex]->deposit(amount);
            saveAccountsToFile();
            cout << "Transfer successful! Amount of $" << fixed << setprecision(2) << amount << " transferred from Account " << accounts[fromAccIndex]->getAccountNumber() << " to Account " << accounts[toAccIndex]->getAccountNumber() << "." << endl;
            return true;
        }
        cout << "Transfer failed!" << endl;
        return false;
    }

    void displayAccountDetails(int index) const {
        if (index >= 0 && index < numAccounts) {
            accounts[index]->displayAccountDetails();
        }
    }

    void depositMoney(int index, double amount) {
        if (index >= 0 && index < numAccounts) {
            accounts[index]->deposit(amount);
            saveAccountsToFile();
        }
    }

    bool withdrawMoney(int index, double amount) {
        if (index >= 0 && index < numAccounts) {
            bool result = accounts[index]->withdraw(amount);
            saveAccountsToFile();
            return result;
        }
        return false;
    }

    bool payUtilityBill(int index, double amount) {
        if (index >= 0 && index < numAccounts) {
            bool result = accounts[index]->payUtilityBill(amount);
            saveAccountsToFile();
            return result;
        }
        return false;
    }

    void reviewBank(int index) const {
        if (index >= 0 && index < numAccounts) {
            accounts[index]->reviewBank();
        }
    }

    void reportBank(int index) const {
        if (index >= 0 && index < numAccounts) {
            accounts[index]->reportBank();
        }
    }

    Account& getAccount(string accNum) const {
        for (int i = 0; i < numAccounts; ++i) {
            if (accounts[i]->getAccountNumber() == accNum) {
                return *accounts[i];
            }
        }
        throw "Account not found";
    }
};

// Function to display a welcome message
void displayWelcomeMessage() {
    cout << "*********************************************" << endl;
    cout << "**      Welcome to Your Bank Management     **" << endl;
    cout << "*********************************************" << endl;
}

int main() {
    Bank bank;
    bank.loadAccountsFromFile();
    int choice;
    do {
        displayWelcomeMessage();
        cout << "\n1. Register Account\n2. Login\n3. Display All Accounts\n4. Delete Account\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string type, accNum, accName, accPinCode;
                double accBalance;
                cout << "*********** Register Account ***********\n";
                cout << "Enter Account Type (savings/current): ";
                cin >> type;
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter Account Name: ";
                cin.ignore(); // Ignore newline character left by previous cin
                getline(cin, accName);
                cout << "Enter Initial Balance: ";
                cin >> accBalance;
                cout << "Enter PIN Code: ";
                cin >> accPinCode;
                bank.registerAccount(type, accNum, accName, accBalance, accPinCode);
                break;
            }
            case 2: {
                string accNum, accPinCode;
                cout << "************* Login *************\n";
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter PIN Code: ";
                cin >> accPinCode;
                int loggedInIndex = bank.login(accNum, accPinCode);
                if (loggedInIndex != -1) {
                    int accountChoice;
                    do {
                        cout << "\nLogged in as " << bank.getAccount(accNum).getName() << endl;
                        cout << "1. Display Account Details\n2. Deposit Money\n3. Withdraw Money\n4. Pay Utility Bill\n5. Transfer Money\n6. Review Bank\n7. Report Bank\n8. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> accountChoice;
                        switch (accountChoice) {
                            case 1:
                                bank.displayAccountDetails(loggedInIndex);
                                break;
                            case 2: {
                                double amount;
                                cout << "Enter amount to deposit: ";
                                cin >> amount;
                                bank.depositMoney(loggedInIndex, amount);
                                break;
                            }
                            case 3: {
                                double amount;
                                cout << "Enter amount to withdraw: ";
                                cin >> amount;
                                bank.withdrawMoney(loggedInIndex, amount);
                                break;
                            }
                            case 4: {
                                double amount;
                                cout << "Enter amount to pay for utility bill: ";
                                cin >> amount;
                                bank.payUtilityBill(loggedInIndex, amount);
                                break;
                            }
                            case 5: {
                                string toAccNum;
                                double amount;
                                cout << "Enter recipient account number: ";
                                cin >> toAccNum;
                                cout << "Enter amount to transfer: ";
                                cin >> amount;
                                bank.transferMoney(loggedInIndex, toAccNum, amount);
                                break;
                            }
                            case 6:
                                bank.reviewBank(loggedInIndex);
                                break;
                            case 7:
                                bank.reportBank(loggedInIndex);
                                break;
                            case 8:
                                cout << "Logging out...\n";
                                break;
                            default:
                                cout << "Invalid choice. Please enter a number from 1 to 8.\n";
                        }
                    } while (accountChoice != 8);
                } else {
                    cout << "Login failed. Invalid account number or PIN code.\n";
                }
                break;
            }
            case 3:
                cout << "************ All Accounts ************\n";
                bank.displayAllAccounts();
                break;
            case 4: {
                string accNum, accPinCode;
                cout << "************ Delete Account ************\n";
                cout << "Enter Account Number: ";
                cin >> accNum;
                cout << "Enter PIN Code: ";
                cin >> accPinCode;
                bank.deleteAccount(accNum, accPinCode);
                break;
            }
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number from 1 to 5.\n";
        }
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get(); // Wait for user to press Enter
    } while (choice != 5);

    return 0;
}
