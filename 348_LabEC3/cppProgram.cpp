//Elizabeth Channel//
//Lab EC 3//
//Baking System//

//includes
#include <iostream>
#include <iomanip>

//create account class
class Account {
protected: //protected values
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public: //public functions
    //account info
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    //display details function
    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }

    //deposit function
    void deposit(double amount) {
        //if amount is greater than zero
        if (amount > 0) {
            balance += amount; //add amount to balance
            //success message
            std::cout << "Deposit of $" << amount << " successful." << std::endl;
        } else {
            //if not greater than zero print error message
            std::cout << "Invalid deposit amount." << std::endl;
        }
    }

    //withdrawal funtion
    virtual void withdraw(double amount) {
        //if the amount is greater than zero and is less than or equal to the balance
        if (amount > 0 && amount <= balance) {
            balance -= amount; //subtract the withdrawal amount fromt he balance
            std::cout << "Withdrawal of $" << amount << " successful." << std::endl; //success message
        } else {
            //if zero or less, error message
            std::cout << "Invalid withdrawal amount or insufficient balance." << std::endl;
        }
    }

    //transfer
    friend Account operator+(const Account& acc1, const Account& acc2) {
        Account result("T000", "Transfer", 0);
        result.balance = acc1.balance + acc2.balance; //resulting balance is the addition of account 1 balance and account 2 balance
        std::cout << "Transfer of $" << result.balance << " successful." << std::endl;
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& acc) {
        acc.displayDetails();
        return os;
    }
};

//create savign account class
class SavingsAccount : public Account {
private: //private values
    double interestRate;

public: //public functions
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    //display details
    void displayDetails() const {
        Account::displayDetails();
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }

    //withdrawal function
    void withdraw(double amount) override {
        const double minimumBalance = 100; //set minimum balance for account
        //if the withdrawal amount is greater than zero
        //and the balance amount after withdrawal is greater/equal to the minimum balance
        if (amount > 0 && (balance - amount) >= minimumBalance) {
            balance -= amount; //subtract withdrawal amount from balance
            std::cout << "Withdrawal of $" << amount << " from savings account successful." << std::endl; //success message
        } else {
          //if not, error message
            std::cout << "Invalid withdrawal amount or insufficient balance (minimum balance: $" << minimumBalance << ")." << std::endl;
        }
    }
};

//create current account class
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    //display details
    void displayDetails() const {
        Account::displayDetails();
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    //withdrawal for overdraft
    void withdraw(double amount) override {
      //if the withdrawal amount is greater than zero
      //and the balance after withdrawal is greater than/equal to the overdraft limit
        if (amount > 0 && (balance - amount) >= -overdraftLimit) {
            balance -= amount; //subtract amount from balance
            std::cout << "Withdrawal of $" << amount << " from current account successful." << std::endl; //success message
        } else {
          //otherwise, error message
            std::cout << "Invalid withdrawal amount or overdraft limit reached." << std::endl;
        }
    }

    CurrentAccount& operator=(const Account& other) {
        if (const CurrentAccount* derived = dynamic_cast<const CurrentAccount*>(&other)) {
            //perform the assignment for CurrentAccount objects
            accountNumber = derived->accountNumber;
            accountHolder = derived->accountHolder;
            balance = derived->balance;
            overdraftLimit = derived->overdraftLimit;
        } else {
            //handle assignment for other types (or error)
            std::cerr << "Invalid assignment to CurrentAccount from a different account type." << std::endl;
        }
        return *this;
    }
};

//main
int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    //transfer 300 from savings to current
    current = current + savings;

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    return 0;
}
