#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <limits>

using namespace std;

class Account {
protected:
    string accountNumber;
    string ownerName;
    double balance;

public:
    Account(string accNum, string name, double bal) : accountNumber(accNum), ownerName(name), balance(bal) {}
    virtual void displayDetails() const {
        cout << "Account Number: " << accountNumber << "\nOwner: " << ownerName << "\nBalance: " << balance << "\n";
    }
    virtual void deposit(double amount) { balance += amount; }
    virtual bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
    string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }
    virtual string getType() const = 0;
    virtual ~Account() {}
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(string accNum, string name, double bal, double rate)
        : Account(accNum, name, bal), interestRate(rate) {}
    void applyInterest() { balance += balance * (interestRate / 100); }
    void displayDetails() const override {
        cout << "[Savings Account]\n";
        Account::displayDetails();
        cout << "Interest Rate: " << interestRate << "% \n";
    }
    string getType() const override { return "Savings"; }
    double getInterestRate() const { return interestRate; }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(string accNum, string name, double bal, double limit)
        : Account(accNum, name, bal), overdraftLimit(limit) {}
    bool withdraw(double amount) override {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
    void displayDetails() const override {
        cout << "[Current Account]\n";
        Account::displayDetails();
        cout << "Overdraft Limit: " << overdraftLimit << "\n";
    }
    string getType() const override { return "Current"; }
    double getOverdraftLimit() const { return overdraftLimit; }
};

class Transaction {
public:
    string accountNumber;
    string type;
    double amount;

    Transaction(string accNum, string t, double amt) : accountNumber(accNum), type(t), amount(amt) {}

    void saveToFile(ofstream& file) const {
        if (file.is_open()) {
            file << accountNumber << " " << type << " " << amount << "\n";
            file.flush();
        }
    }
};

class Bank {
private:
    vector<unique_ptr<Account>> accounts;
    vector<Transaction> transactions;
    ofstream transactionsFile;

public:
    Bank() : transactionsFile("transactions.txt", ios::app) {
        loadAccounts();
    }

    ~Bank() {
        saveAccounts();
        transactionsFile.close();
    }

    void addAccount(unique_ptr<Account> acc) {
        for (const auto& existingAcc : accounts) {
            if (existingAcc->getAccountNumber() == acc->getAccountNumber()) {
                cerr << "Account number already exists!\n";
                return;
            }
        }
        accounts.push_back(move(acc));
    }

    void displayAllAccounts() const {
        if (accounts.empty()) {
            cout << "No accounts available.\n";
            return;
        }
        cout << "==== Account Holder List ====\n";
        for (const auto& acc : accounts) {
            acc->displayDetails();
            cout << "---------------------------\n";
        }
    }

    Account* findAccount(const string& accNum) {
        for (const auto& acc : accounts) {
            if (acc->getAccountNumber() == accNum)
                return acc.get();
        }
        return nullptr;
    }

    void recordTransaction(const string& accNum, const string& type, double amount) {
        transactions.emplace_back(accNum, type, amount);
        transactions.back().saveToFile(transactionsFile);
    }

    void saveAccounts() const {
        ofstream file("accounts.txt");
        if (file.is_open()) {
            for (const auto& acc : accounts) {
                file << acc->getType() << " " << acc->getAccountNumber() << " " << acc->getBalance();
                if (acc->getType() == "Savings") {
                    file << " " << static_cast<SavingsAccount*>(acc.get())->getInterestRate();
                } else if (acc->getType() == "Current") {
                    file << " " << static_cast<CurrentAccount*>(acc.get())->getOverdraftLimit();
                }
                file << "\n";
            }
            file.close();
        } else {
            cerr << "Error saving accounts.\n";
        }
    }

    void loadAccounts() {
        ifstream file("accounts.txt");
        if (file.is_open()) {
            string type, accNum, name;
            double balance, rateOrLimit;
            while (file >> type >> accNum >> balance >> rateOrLimit) {
                cout << "Loading Account: " << accNum << "\n";
                name = "User" + accNum;
                if (type == "Savings") {
                    addAccount(make_unique<SavingsAccount>(accNum, name, balance, rateOrLimit));
                } else if (type == "Current") {
                    addAccount(make_unique<CurrentAccount>(accNum, name, balance, rateOrLimit));
                }
            }
            file.close();
        }
    }
};

int main() {
    Bank bank;
    int choice;
    string accNum, name;
    double balance, amount, rate, limit;

    do {
        cout << "====================================\n";
        cout << "  Welcome To Standard Chartered Bank  \n";
        cout << "====================================\n";
        cout << "OUR SERVICES:\n";
        cout << "1. ADD SAVINGS ACCOUNT\n";
        cout << "2. ADD CURRENT ACCOUNT\n";
        cout << "3. CASH DEPOSIT\n";
        cout << "4. CASH WITHDRAWAL\n";
        cout << "5. ALL ACCOUNT HOLDER LIST\n";
        cout << "6. EXIT\n";
        cout << ":..ENTER YOUR CHOICE [1-6]: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Owner Name: "; cin >> name;

                while (true) {
                    cout << "Enter Initial Balance: ";
                    if (cin >> balance && balance >= 0) break;
                    cout << "Invalid balance. Please enter a non-negative number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                while (true) {
                    cout << "Enter Interest Rate: ";
                    if (cin >> rate && rate >= 0) break;
                    cout << "Invalid rate. Please enter a non-negative number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                bank.addAccount(make_unique<SavingsAccount>(accNum, name, balance, rate));
                break;
            }
            case 2: {
                cout << "Enter Account Number: "; cin >> accNum;
                cout << "Enter Owner Name: "; cin >> name;
                while (true) {
                    cout << "Enter Initial Balance: ";
                    if (cin >> balance && balance >= 0) break;
                    cout << "Invalid balance. Please enter a non-negative number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                while (true) {
                    cout << "Enter Overdraft Limit: ";
                    if (cin >> limit && limit >= 0) break;
                    cout << "Invalid limit. Please enter a non-negative number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                bank.addAccount(make_unique<CurrentAccount>(accNum, name, balance, limit));
                break;
            }
            case 3: {
                cout << "Enter Account Number: "; cin >> accNum;
                Account* acc = bank.findAccount(accNum);
                if (acc) {
                    while (true) {
                        cout << "Enter Amount to Deposit: ";
                        if (cin >> amount && amount >= 0) break;
                        cout << "Invalid amount. Please enter a non-negative number.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    acc->deposit(amount);
                    bank.recordTransaction(accNum, "Deposit", amount);
                    cout << "Deposit Successful!\n";
                } else {
                    cout << "Account Not Found!\n";
                }
                break;
            }
            case 4: { 
                cout << "Enter Account Number: "; cin >> accNum;
                Account* acc = bank.findAccount(accNum);
                if (acc) {
                    while (true) {
                        cout << "Enter Amount to Withdraw: ";
                        if (cin >> amount && amount >= 0) break;
                        cout << "Invalid amount. Please enter a non-negative number.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    if (acc->withdraw(amount)) {
                        bank.recordTransaction(accNum, "Withdrawal", amount);
                        cout << "Withdrawal Successful!\n";
                    } else {
                        cout << "Insufficient Funds!\n";
                    }
                } else {
                    cout << "Account Not Found!\n";
                }
                break;
            }
            case 5:
                bank.displayAllAccounts();
                break;
            case 6:
                cout << "THANK YOU FOR BANKING WITH US, HAVE A GREAT DAY!\n";
                break;
            default:
                cout << "Invalid Choice!\n";
        }
    } while (choice != 6);

    return 0;
}