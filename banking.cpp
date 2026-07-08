/******************************************************************************
Simple Banking System - Console Application (C++)
Updated Features:
 1. Unique customer name check (asks again if name already exists)
 2. Phone number must be exactly 10 digits (asks again if invalid)
 3. Account number must contain digits only, no letters/symbols
 4. Duplicate account number check
 5. Check Balance option added
*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

// ---------- Transaction Class ----------
class Transaction {
public:
    string type;
    double amount;
    string date;

    Transaction(string t, double a) {
        type = t;
        amount = a;

        time_t now = time(0);
        date = ctime(&now);
        if (!date.empty() && date.back() == '\n')
            date.pop_back();
    }

    void display() {
        cout << date << " | " << type << " | Rs." << amount << endl;
    }
};

// ---------- Account Class ----------
class Account {
private:
    int accountNo;
    double balance;
    vector<Transaction> history;

public:
    Account() {
        accountNo = 0;
        balance = 0;
    }

    Account(int accNo) {
        accountNo = accNo;
        balance = 0;
    }

    int getAccountNo() {
        return accountNo;
    }

    double getBalance() {
        return balance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid Amount!" << endl;
            return;
        }

        balance += amount;
        history.push_back(Transaction("Deposit", amount));

        cout << "Deposit Successful." << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid Amount!" << endl;
            return;
        }

        if (amount > balance) {
            cout << "Insufficient Balance!" << endl;
            return;
        }

        balance -= amount;
        history.push_back(Transaction("Withdraw", amount));

        cout << "Withdrawal Successful." << endl;
    }

    void transfer(Account &receiver, double amount) {

        if (amount <= 0) {
            cout << "Invalid Amount!" << endl;
            return;
        }

        if (amount > balance) {
            cout << "Insufficient Balance!" << endl;
            return;
        }

        balance -= amount;
        receiver.balance += amount;

        history.push_back(Transaction("Transfer to Account " + to_string(receiver.accountNo), amount));
        receiver.history.push_back(Transaction("Received from Account " + to_string(accountNo), amount));

        cout << "Transfer Successful." << endl;
    }

    // New: Check Balance option (quick balance-only view)
    void checkBalance() {
        cout << "Available Balance : Rs." << balance << endl;
    }

    void displayHistory() {
        if (history.empty()) {
            cout << "No Transactions Found." << endl;
            return;
        }

        for (auto t : history)
            t.display();
    }

    void displayAccount() {
        cout << "Account Number : " << accountNo << endl;
        cout << "Balance        : Rs." << balance << endl;
    }
};

// ---------- Customer Class ----------
class Customer {
private:
    int customerId;
    string name;
    string phone;

public:
    Account account;

    Customer() {}

    Customer(int id, string n, string p, int accNo) {
        customerId = id;
        name = n;
        phone = p;
        account = Account(accNo);
    }

    int getCustomerId() {
        return customerId;
    }

    string getName() {
        return name;
    }

    void displayCustomer() {
        cout << "\nCustomer ID : " << customerId << endl;
        cout << "Name        : " << name << endl;
        cout << "Phone       : " << phone << endl;
        account.displayAccount();
    }
};

vector<Customer> customers;

// ---------- Helper / Lookup Functions ----------

Customer* findCustomer(int id) {
    for (auto &c : customers) {
        if (c.getCustomerId() == id)
            return &c;
    }
    return nullptr;
}

// Check if a customer with the given name already exists
bool nameExists(const string &name) {
    for (auto &c : customers) {
        if (c.getName() == name)
            return true;
    }
    return false;
}

// Check if a string contains only digits (used for phone & account number)
bool isNumericString(const string &s) {
    if (s.empty()) return false;
    for (char ch : s) {
        if (!isdigit((unsigned char)ch))
            return false;
    }
    return true;
}

// Phone must be exactly 10 digits, numbers only
bool isValidPhone(const string &phone) {
    return phone.length() == 10 && isNumericString(phone);
}

// Check if an account number is already used by another customer
bool accountNoExists(int accNo) {
    for (auto &c : customers) {
        if (c.account.getAccountNo() == accNo)
            return true;
    }
    return false;
}

// ---------- Input Helper Functions (validation loops) ----------

// Keep asking for a name until a unique one is entered
string inputUniqueName() {
    string name;
    while (true) {
        cout << "Name : ";
        cin >> name;

        if (nameExists(name)) {
            cout << "This name is already used by another customer. Please enter a different name.\n";
            continue;
        }
        break;
    }
    return name;
}

// Keep asking for phone number until a valid 10-digit number is entered
string inputValidPhone() {
    string phone;
    while (true) {
        cout << "Phone (10 digits) : ";
        cin >> phone;

        if (!isValidPhone(phone)) {
            cout << "Invalid Phone Number! It must be exactly 10 digits (numbers only).\n";
            continue;
        }
        break;
    }
    return phone;
}

// Keep asking for account number until it's numeric-only and not already used
int inputValidAccountNo() {
    string accStr;
    while (true) {
        cout << "Account Number (digits only) : ";
        cin >> accStr;

        if (!isNumericString(accStr)) {
            cout << "Invalid Account Number! Only digits (0-9) are allowed, no letters or symbols.\n";
            continue;
        }

        int accNo = stoi(accStr);

        if (accountNoExists(accNo)) {
            cout << "This Account Number is already in use. Please enter a different one.\n";
            continue;
        }

        return accNo;
    }
}

// ---------- Main Program ----------

int main() {

    int choice;

    do {
        cout << "\n========== BANKING SYSTEM ==========\n";
        cout << "1. Create Customer\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Fund Transfer\n";
        cout << "5. Display Customer Details\n";
        cout << "6. Transaction History\n";
        cout << "7. Check Balance\n";
        cout << "8. Exit\n";

        cout << "Enter Choice : ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid Input! Please enter a number.\n";
            continue;
        }

        switch (choice) {

        case 1: {
            int id;

            cout << "Customer ID : ";
            cin >> id;

            if (findCustomer(id) != nullptr) {
                cout << "Customer Already Exists!" << endl;
                break;
            }

            string name = inputUniqueName();
            string phone = inputValidPhone();
            int accNo = inputValidAccountNo();

            customers.push_back(Customer(id, name, phone, accNo));

            cout << "Customer Created Successfully." << endl;
            break;
        }

        case 2: {
            int id;
            double amount;

            cout << "Customer ID : ";
            cin >> id;

            Customer *c = findCustomer(id);

            if (c == nullptr) {
                cout << "Customer Not Found!" << endl;
                break;
            }

            cout << "Amount : ";
            cin >> amount;

            c->account.deposit(amount);
            break;
        }

        case 3: {
            int id;
            double amount;

            cout << "Customer ID : ";
            cin >> id;

            Customer *c = findCustomer(id);

            if (c == nullptr) {
                cout << "Customer Not Found!" << endl;
                break;
            }

            cout << "Amount : ";
            cin >> amount;

            c->account.withdraw(amount);
            break;
        }

        case 4: {
            int senderId, receiverId;
            double amount;

            cout << "Sender Customer ID : ";
            cin >> senderId;

            cout << "Receiver Customer ID : ";
            cin >> receiverId;

            Customer *sender = findCustomer(senderId);
            Customer *receiver = findCustomer(receiverId);

            if (sender == nullptr || receiver == nullptr) {
                cout << "Customer Not Found!" << endl;
                break;
            }

            cout << "Amount : ";
            cin >> amount;

            sender->account.transfer(receiver->account, amount);
            break;
        }

        case 5: {
            int id;
            cout << "Customer ID : ";
            cin >> id;

            Customer *c = findCustomer(id);

            if (c == nullptr) {
                cout << "Customer Not Found!" << endl;
                break;
            }

            c->displayCustomer();
            break;
        }

        case 6: {
            int id;
            cout << "Customer ID : ";
            cin >> id;

            Customer *c = findCustomer(id);

            if (c == nullptr) {
                cout << "Customer Not Found!" << endl;
                break;
            }

            c->account.displayHistory();
            break;
        }

        case 7: {
            int id;
            cout << "Customer ID : ";
            cin >> id;

            Customer *c = findCustomer(id);

            if (c == nullptr) {
                cout << "Customer Not Found!" << endl;
                break;
            }

            c->account.checkBalance();
            break;
        }

        case 8:
            cout << "Thank You!" << endl;
            break;

        default:
            cout << "Invalid Choice!" << endl;
        }

    } while (choice != 8);

    return 0;
}