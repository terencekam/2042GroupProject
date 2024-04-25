// Include necessary libraries
#define FMT_HEADER_ONLY

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <string.h>
#include <utility>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <fmt/core.h>
//#include </home/runner/C20-Template-1/fmt/core.h>

// Forward declaration of Customer class
class Customer;

// Forward declaration of GiftRecord class
class GiftRecord;
// Define namespace
using namespace std;

// Function prototypes
bool Fo(char, int);

// Function to check if a customer exists
bool HasCustomer(char CustomerID);

// Function to get a customer
bool AddCustomer(Customer c);

// Function to delete a customer
bool hasLoadData = false;

// Enum for Rank
enum Rank {
    G, S, B
};

// Map to convert Rank to string
map<Rank, string> RanktoString = {
        {G, "G"},
        {S, "S"},
        {B, "B"}
};
time_t now = time(0);
tm *ltm = localtime(&now);
// File to store logs
ofstream Filelog;

// Custom Exception
class RangeException : exception {
public:
    const string m_msg;

    explicit RangeException(string msg) : m_msg(std::move(msg)) {
        cout << m_msg << "\n";
    }

    virtual const char *what() const throw() {
        return m_msg.c_str();
    }
};

// Class for Logger
class Logger {

protected:
    // Enum for level
    enum level {
        w, e, i
    };
    map<level, string> levelToString = {
            {w, "WARN "},
            {e, "ERROR"},
            {i, "INFO "}
    };

    virtual void logto(string log, level l) {
        auto localtime = asctime(ltm);
        localtime[strlen(localtime) - 1] = 0;
        cout << localtime << "[" << levelToString[l] << "]" << log << "\n";
        Filelog.open("latest.log", std::ofstream::app);
        Filelog << localtime << "[" << levelToString[l] << "]" << log << "\n";
        Filelog.close();
    }


public:
    void warn(string log) {
        logto(log, w);
    }

    void error(string log) {
        logto(log, e);
    }

    void info(string log) {
        logto(log, i);
    }

    virtual void printLog() {
        string s;
        ifstream getFile("latest.log");
        while (getline(getFile, s)) {
            cout << s << endl;
        }
    }
};

Logger logger;

// Struct for Date

struct tm *today = localtime(&now);

// Function to get auto rank
Rank getAutoRank(tm *date) {

    auto t = mktime(today);
    auto OtherDate = mktime(date);
    if (t - OtherDate > 31556926) { // 1 year
        return G;
    } else if (t - OtherDate > 15778463) { // 6 months
        return S;
    } else {
        return B;
    }
}

// Enum for GiftCategory
enum GiftCategory {
    a, b, c, d
};

// Map to convert GiftCategory to string
map<GiftCategory, string> GiftCategoryToString = {
        {a, "Audio & Video"},
        {b, "Kitchenware"},
        {c, "Coupons"},
        {d, "Computer Accessories"},
};

// Class for Customer
class Customer : Logger {
private:
    string log;
    string CustomerID = "";
    Rank Ranking = B;
    int PointBalance = 0;
public:
    Customer() {};

    Customer(string customerID, Rank rank, int pointBalance) {
        CustomerID = customerID;
        Ranking = rank;
        PointBalance = pointBalance;
    }

    string getCustomerID() {
        return CustomerID;
    }

    Rank getRank() {
        return Ranking;
    }

    int getPointBalance() {
        return PointBalance;
    }

    void addPointBalance(int amount) {
        if (amount < 0) {
            cout << "No Input occur amount<0\n";
            return;
        }
        PointBalance += amount;
        this->info(fmt::format("Customer with ID= '{0}' added Point = '{1}'", CustomerID, amount));
    }

    bool minusPointBalance(int amount) {
        if (amount < 0) {
            cout << "No Input occur amount<0\n";
            return false;
        }
        if (PointBalance >= amount) {
            PointBalance -= amount;
            this->info(fmt::format("Customer with ID= '{0}' deducted Point = '{1}'", CustomerID, amount));
            return true;
        }
        return false;
    }

    void setPointBalance(int PointBalance) {
        if (PointBalance < 0) {
            cout << "No Input occur amount<0\n";
        }
        this->PointBalance = PointBalance;
        this->info(fmt::format("Customer with ID = '{0}' set the Point as '{1}'", CustomerID, PointBalance));
    }

    void toString() {
        printf("%-15s %-4s %-d\n", CustomerID.c_str(), RanktoString[Ranking].c_str(), PointBalance);
    }

    void logto(string Log, level l) override {
        auto localtime = asctime(ltm);
        localtime[strlen(localtime) - 1] = 0;
        ostringstream os;
        os << localtime << "[" << levelToString[l] << "]" << Log << "\n";
        log += os.str();
        Logger::logto(Log, l);
    }

    void printLog() override {
        if(this->log.empty()){
            cout << "No Transaction on Customer: " << this->CustomerID << endl;
            return;
        }
        cout << this->log;
    }
};

// Vector to store list of customers
vector<Customer> customerList;

// Struct for GiftRecord
struct GiftRecord {

    char *GiftID = new char[3];
    char *GiftDiscription = new char[100];
    int price{};
    int PointRequired{};
    enum GiftCategory giftCategory;

    GiftRecord(char *gift_id, char *gift_discription, int price, int point_required) {
        giftCategory = static_cast<GiftCategory>(gift_id[0]);
        GiftID = gift_id;
        GiftDiscription = gift_discription;
        this->price = price;
        PointRequired = point_required;
    }

    void toString() {
        printf("%-7s %-10d %-17d %s\n", GiftID, price, PointRequired, GiftDiscription);
    }
};

// Vector to store list of gift records
vector<GiftRecord> giftRecordList;

// Function to check if a customer exists
bool HasCustomer(string CustomerID) {
    for (auto customer_list: customerList) {
        if (customer_list.getCustomerID() == CustomerID) {
            return true;
        }
    }
    return false;
}

// Function to get a customer
Customer GetCustomer(string CustomerID) {
    for (auto customer_list: customerList) {
        if (customer_list.getCustomerID() == CustomerID) {
            return customer_list;
        }
    }
    return {"", G, -1};
}

// Function to delete a customer
auto DeleteCustomer = [](string CustomerID) {
    customerList.erase(remove_if(customerList.begin(), customerList.end(), [&CustomerID](Customer o) {
        return CustomerID == o.getCustomerID();
    }), customerList.end());
};

// Function to add a customer
bool AddCustomer(Customer c) {
    if (HasCustomer(c.getCustomerID())) {
        return false;
    }
    // Check if the point balance is negative
    if (c.getPointBalance() < 0) {
        cout << "No Input occur amount<0\n";
    }
    customerList.emplace_back(c);
    cout << fmt::format("Added new Customer to CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'\n",
                        c.getCustomerID(), RanktoString[c.getRank()], c.getPointBalance());
    logger.info(fmt::format("Added new Customer to CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'",
                            c.getCustomerID(), RanktoString[c.getRank()], c.getPointBalance()));
    return true;
}

// Function to check if a record exists
bool HasRecord(GiftRecord r) {
    for (auto gift_record_list: giftRecordList) {
        if (r.GiftID == gift_record_list.GiftID) {
            return true;
        }
    }
    return false;
}

// Function to modify a customer
void ModifyCustomer(Customer c) {
    DeleteCustomer(c.getCustomerID());
    customerList.emplace_back(c);
}

// Function to add a record
void AddRecord(GiftRecord r) {
    if (!HasRecord(r)) {
        logger.info(fmt::format(
                "Added new Record: GiftCategory = '{0}' , GiftID = '{1}' , GiftDiscription = '{2}' , Price = '{3}' , PointRequired = {4}",
                to_string(r.giftCategory), r.GiftID, r.GiftDiscription, r.price, r.PointRequired));
        giftRecordList.emplace_back(r);
    }
}

// Function to initialize data
void initialsiation() {
    customerList = {Customer("Tommy2015", B, 8500),
                    Customer("DavidChan", B, 22800),
                    Customer("Luna123", B, 650),
                    Customer("TigerMan", B, 14000),
                    Customer("Max5678", S, 2580),
                    Customer("Neo2000", S, 8000),
                    Customer("CCTang", S, 33554),
                    Customer("EchoWong", G, 8650),
                    Customer("Rubychow", G, 28000),
                    Customer("Ivy2023", G, 12340)};
    giftRecordList = {GiftRecord("A01", "LG Internet TV", 3900, 19000),
                      GiftRecord("A02", "Pioneer Hifi Set", 2400, 11500),
                      GiftRecord("A03", "Sony DVD Player", 400, 2000),
                      GiftRecord("B01", "Healthy Air Fryer", 1500, 7300),
                      GiftRecord("B02", "Tefal Microwave Oven", 480, 2400),
                      GiftRecord("B03", "Famous Coffee Maker", 1800, 8800),
                      GiftRecord("B04", "Smart Rice Cooker", 600, 2900),
                      GiftRecord("B05", "TechCook Toaster Oven", 450, 2250),
                      GiftRecord("C01", "Wellcome 50 Coupon", 50, 250),
                      GiftRecord("C02", "Mannings 50 Coupon", 50, 250),
                      GiftRecord("C03", "Carol Restaurant 100 Coupon", 100, 500),
                      GiftRecord("C04", "Shell 200 Coupon", 200, 960),
                      GiftRecord("D01", "Clever Headset ", 350, 1750),
                      GiftRecord("D02", "HP Optic Mouse", 250, 1250),
                      GiftRecord("D03", "Stylish Bluetooth Speaker", 800, 3900)};
    hasLoadData = true;
}

bool HasGiftRecord(string giftId) {
    // Check if the gift record exists
    for (const auto &item: giftRecordList) {
        if (item.GiftID == giftId) {
            return true;
        }
    }
    return false;
}

GiftRecord getGiftRecord(string giftId) {
    // Get the gift record
    for (const auto &item: giftRecordList) {
        if (item.GiftID == giftId) {
            return item;
        }
    }
    return {"", "", 0, 0};
}

// Function to check if a date is correct
tm isCorrectDate(string date) {
    const char *dateToChar = date.data();
    if (strlen(dateToChar) != 8) {
        throw length_error("The date length size != 8\n");
    }
    try {
        stoi(date);
    } catch (invalid_argument e) {
        throw invalid_argument("The date are not numbers!\n");
    }
    if (stoi(date) < 0) {
        throw RangeException("Date < 0");
    }
    int year = stoi(date.substr(4, 4)); //DDMMYYY
    int month = stoi(date.substr(2, 2));
    int day = stoi(date.substr(0, 2));
    if (year > today->tm_year + 1900) {
        throw RangeException(fmt::format("Year > {}\n", today->tm_year + 1900));
    } else if (year == today->tm_year + 1900) {
        if (month > today->tm_mon + 1) {
            throw RangeException(
                    fmt::format("Month > {}, Since this Year = {}\n", today->tm_mon + 1, today->tm_year + 1900));
        } else if (month == today->tm_mon + 1 && day > today->tm_mday) {
            throw RangeException(fmt::format("Day > {}, Since this Year = {}, this Month = {}\n", today->tm_mday,
                                             today->tm_year + 1900, today->tm_mon + 1));
        }
    }
    // Check if the date is correct
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: {
            if (day > 31) {
                throw RangeException("Days > 31\n");
            }
            break;
        }
        case 2: {
            if (year % 4 == 0) {
                if (day > 29) {
                    throw RangeException("Days > 29\n");
                }

            } else if (day > 28) {
                throw RangeException("Days > 28\n");
            }
            break;
        }
        case 4:
        case 6:
        case 9:
        case 11: {
            if (day > 30) {
                throw RangeException("Days > 30\n");
            }
            break;
        }
        default:
            throw RangeException("month > 12\n");
    }
    auto t = [&year, &day, &month] {
        struct tm t = {0};
        t.tm_year = year - 1900;
        t.tm_mon = month - 1;
        t.tm_mday = day;
        return t;
    };

    return t();

}

// Function for Customer View
void CustomerView() {
    string tempinput;
    int input;
    string customerID;
    Customer customer;
    cout << "Please input CustomerID: ";
    getline(cin, customerID);
    if (HasCustomer(customerID)) {
        customer = GetCustomer(customerID);
    } else {
        cout << "Can't find customer , please try again!\n";
        return;
    }
    do {
        int oldPoint = customer.getPointBalance();
        // Get customer ID
        try {
            cout << "\033[1;35mAction for CustomerID: "
                 << customer.getCustomerID() <<
                 "\n***** Customer View Menu *****\n" // Display menu
                 "[1] Earn CC Points\n"
                 "[2] Redeem Gifts\n"
                 "[3] Modify CC Points Balance\n"
                 "[4] Return to Main Menu\n"
                 "**************************\033[0m\n"
                 "Option (1 - 4): ";
            getline(cin, tempinput); // Get user input
            input = stoi(tempinput); // Convert input to integer
            switch (input) { // Switch case based on input
                case 1: {
                    string tempmoney;
                    cout << "Input an amount of money: ";
                    getline(cin, tempmoney);
                    float money;
                    try {
                        money = stof(tempmoney);
                        if (money > 0) {
                            int points = (int) (money / 250);
                            customer.addPointBalance(points); // Add points to customer
                            cout << "Points added!";
                        } else {
                            cout << "Wrong Input , Please try again, amount < 0\n";
                            break;
                        }
                    } catch (exception e) {
                        cout << "Wrong input , PLease try again\n";
                    }
                    break;
                }
                case 2: {
                    int times = 0;
                    do {
                        try {
                            string temp;
                            cout << "Please input the Category: ";
                            getline(cin, temp); // Get category
                            char category[10];
                            strcpy(category, temp.c_str());
                            // Check if category is valid
                            if (69 > static_cast<int>(*category) && static_cast<int>(*category) > 64) {
                                sort(giftRecordList.begin(), giftRecordList.end(), // Sort gift records
                                     [](GiftRecord a, GiftRecord b) { // Lambda function to sort gift records
                                         if (a.PointRequired < b.PointRequired) {
                                             return true;
                                         }
                                         return false;
                                     }
                                );
                                printf("%-7s %-10s %-17s %s\n", "GiftID", "Price(HKD)", "Points Required", "Gift Description");
                                for_each(giftRecordList.begin(), giftRecordList.end(),
                                         [&category, &customer](GiftRecord j) {
                                             if ((char) j.giftCategory == (char) *category) {
                                                 printf("%-7s %-10d %-17d %s\n", j.GiftID, j.price, j.PointRequired,
                                                        j.PointRequired <= customer.getPointBalance()
                                                        ? "(enough Points!)" : "(not enough Points!)",
                                                        j.GiftDiscription);
                                             }
                                         });
                                cout << "Please Select: ";
                                getline(cin, temp);//
                                if (!temp.empty()) {
                                    if (HasGiftRecord(temp)) {
                                        GiftRecord g = getGiftRecord(temp);
                                        if ([&customer, &g]() {
                                            switch (customer.getRank()) {
                                                case G:
                                                    return g.PointRequired * 0.9;
                                                case S:
                                                    return g.PointRequired * 0.95;
                                                default:
                                                    return (double)g.PointRequired;
                                            }
                                        }.operator()() <= customer.getPointBalance()) {
                                            cout << "You can get the gift freely!\ncontinue?(y/n): ";
                                        } else {
                                            printf("You have not enough points, you may pay extra of %.0f to redeem the gift and all the points will be deducted, continue?(y/n) ",
                                                   [&customer, &g]() {
                                                       switch (customer.getRank()) {
                                                           case G:
                                                               return (g.PointRequired * 0.9 - customer.getPointBalance()) * 0.2;
                                                           case S:
                                                               return (g.PointRequired * 0.95 - customer.getPointBalance()) * 0.2;
                                                           default:
                                                               return (g.PointRequired - customer.getPointBalance()) * 0.2;
                                                       }
                                                   }.operator()());
                                        }
                                        getline(cin, temp);
                                        if (!temp.empty()) {
                                            if (temp == "y") {
                                                if (g.PointRequired <= customer.getPointBalance()) {
                                                    customer.minusPointBalance(g.PointRequired);
                                                    cout << "redeemed!\n";
                                                    break;
                                                } else {
                                                    customer.setPointBalance(0);
                                                    cout << "redeemed!\n";
                                                    break;
                                                }
                                            } else if (temp == "n") {
                                                break;
                                            } else {
                                                cout << "Wrong input received!\n";
                                            }
                                        } else {
                                            cout << "No input received!\n";
                                        }
                                    }
                                } else {
                                    cout << "no input , please try again!\n";
                                }
                            }else{
                                cout << "Wrong input received!\n";
                            }
                        } catch (exception e) {

                        }
                        times++;
                        if (times == 3) {
                            cout << "Input error > 3... Exiting\n";
                        }
                    } while (times != 3);
                    break;
                }
                case 3:
                    while (true) {
                        cout << "Your CC Points : " << customer.getPointBalance() << endl;
                        cout << "Set a new cc Points: ";
                        string temp;
                        getline(cin, temp);
                        try {
                            auto points = stoi(temp);
                            if (points > 0) {
                                customer.setPointBalance(points);
                                cout << "operation complete!" << endl;
                            } else {
                                cout << "Wrong Input , Please try again" << endl;
                                break;
                            }
                        } catch (exception e) {
                            cout << "wrong input , Please try again!" << endl;
                        }
                        break;
                    }
                case 4:
                    break;
                default:
                    cout << "Wrong input , please try again!";
            }
        } catch (exception e) {
        }
        if (customer.getPointBalance() > oldPoint) {
            printf("Customer with ID: %s added Point of : %i (Old Point: %i , New Point: %i) ...Full logs on latest.log or view it on transaction\n",
                   customer.getCustomerID().c_str(), customer.getPointBalance() - oldPoint, oldPoint,
                   customer.getPointBalance());
        } else if (customer.getPointBalance() < oldPoint) {
            printf("Customer with ID: %s reduced Point of : %i (Old Point: %i , New Point: %i) ...Full logs on latest.log or view it on transaction\n",
                   customer.getCustomerID().c_str(), oldPoint - customer.getPointBalance(), oldPoint,
                   customer.getPointBalance());
        } else {
            printf("Customer with ID: %s has Point: %i ...Full logs on latest.log or view it on transaction\n",
                   customer.getCustomerID().c_str(), customer.getPointBalance());
        }
    } while (input != 4);
    customer.toString();
    ModifyCustomer(customer);
}


// Main function
int main() {
    string tempselect;
    int select = 0;
    do {
        cout << "\033[1;34mWelcome Gift Redemption System\n"
                "*** Main Menu ***\n"
                "[1] Load Starting Data\n"
                "[2] Show Records\n"
                "[3] Edit Customer\n"
                "[4] Enter Customer View\n"
                "[5] Show Transaction History\n"
                "[6] Credit and Exit\033[0m\n";
        cout << "Option (1 - 6) : ";
        getline(cin, tempselect);
        if (tempselect.empty()) {
            select = -1;
        }
        try {
            if (!hasLoadData) {
                if (tempselect != "1" && tempselect != "6") {
                    cout << "You haven't load the data!\n";
                    continue;
                }
            }
            select = stoi(tempselect);
            switch (select) {
                case 1: {
                    initialsiation();
                    break;
                }
                case 2: {
                    sort(giftRecordList.begin(), giftRecordList.end(), [](GiftRecord a, GiftRecord b) {
                        if (tolower(*a.GiftID) > tolower(*b.GiftID)) {
                            return false;
                        }
                        return true;
                    });
                    sort(customerList.begin(), customerList.end(), [](Customer a, Customer b) {
                        if (tolower(*a.getCustomerID().c_str()) > tolower(*b.getCustomerID().c_str())) {
                            return false;
                        }
                        return true;
                    });
                    printf("%-15s %-4s %-s\n", "Customer ID", "Rank", "Point Balance");
                    for (auto customer_list: customerList) {
                        customer_list.toString();
                    }
                    printf("%-7s %-10s %-17s %s\n", "GiftID", "Price(HKD)", "Points Required", "Gift Description");
                    for (auto giftRecord: giftRecordList) {
                        giftRecord.toString();
                    }
                    break;
                }
                case 3: {
                    string customerID;
                    cout << "Please input an Customer ID: ";
                    string next;
                    bool passing = true;
                    getline(cin, customerID);
                    if (customerID.empty()) {
                        cout << "No input found!\n";
                        break;
                    }
                    for (int i = 0; i < strlen(customerID.c_str()); i++) {
                        if (customerID.c_str()[i] == ' ') {
                            cout << "There are space in the CustomerID! Please remove the space and try again! \n";
                            passing = false;
                            break;
                        }
                    }
                    if (!passing) {
                        break;
                    }
                    string choice;
                    if (HasCustomer(customerID)) {
                        do {
                            auto c = GetCustomer(customerID);
                            cout << fmt::format(
                                    "Customer: CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'\n",
                                    c.getCustomerID(), RanktoString[c.getRank()], c.getPointBalance());
                            cout << "Are you sure to remove the customerID?(y/n)\n";
                            getline(cin, choice);
                            choice = tolower(*choice.c_str());
                            if (choice == "y") {
                                logger.info(fmt::format(
                                        "Customer with ID = '{0}' removed , CustomerRank = '{1}' , PointBalance= '{2}",
                                        customerID, RanktoString[c.getRank()], c.getPointBalance()));
                                DeleteCustomer(customerID);
                            }
                            if (choice != "y" && choice != "n") {
                                cout << "Wrong Input , please try again!\n";
                            }
                        } while (choice != "y" && choice != "n");
                        break;
                    } else {
                        // if the customerID larger than 50 , it will not be added
                        if (customerID.size() > 50) {
                            cout << "Error ... CustomerID > 50\n";
                            break;
                        }
                        do {
                            cout << "No Customer ID found ... Are you sure to add the Customer?(y/n)\n";
                            getline(cin, choice);
                            choice = tolower(*choice.c_str());
                            if (choice != "y" && choice != "n") {
                                cout << "Wrong Input , please try again!\n";
                            }
                        } while (choice != "y" && choice != "n");
                        if (choice == "n") {
                            break;
                        }
                        string tempDate;
                        tm date;
                        do {
                            // Get the date
                            cout
                                    << "Please input a date with DDMMYYYY(There should be 8 character of integer , or otherwise an error will occur. You may type 'today'(case sensitive) such that the customer join as member today)\n";
                            getline(cin, tempDate);
                            try {
                                if (tempDate == "today") {
                                    date = *today;
                                    break;
                                } else {
                                    date = isCorrectDate(tempDate);
                                    break;
                                }
                            } catch (RangeException e) {
                                cout << "Please try again...\n";
                                continue;
                            } catch (length_error e) {
                                cout << "Please try again...\n";
                                continue;
                            }
                        } while (true);
                        int PointBalance;
                        do {
                            try {
                                // Get the initial points
                                cout << "Input the Initial points:";
                                string tempinput;
                                getline(cin, tempinput);
                                if (tempinput.empty()) {
                                    throw invalid_argument("");
                                }
                                PointBalance = stoi(tempinput);
                                if (PointBalance < 0) {
                                    cout << "The Point should not be > 0\n";
                                }
                            } catch (invalid_argument e) {
                                cout << "Wrong Input , Please try again" << endl;
                                PointBalance = -1;
                            }
                        } while (PointBalance < 0);
                        AddCustomer(Customer(customerID, getAutoRank(&date), PointBalance));
                    }
                    break;
                }
                case 4:
                    CustomerView();
                    break;
                case 5: {
                    string customerID;
                    cout << "Please input an Customer ID to get the log (You may type 'all' to get all the log): ";
                    string next;
                    bool passing = true;
                    getline(cin, customerID);
                    if (customerID.empty()) {
                        cout << "No input found!\n";
                        break;
                    }
                    for (int i = 0; i < strlen(customerID.c_str()); i++) {
                        if (customerID.c_str()[i] == ' ') {
                            cout << "There are space in the CustomerID! Please remove the space and try again! \n";
                            passing = false;
                            break;
                        }
                    }
                    if (!passing) {
                        break;
                    }
                    if (HasCustomer(customerID)) {
                        auto c = GetCustomer(customerID);
                        c.printLog();
                    } else if (customerID == "all") {
                        logger.printLog();
                    } else {
                        cout << "Wrong Type , Please try again!\n";
                    }
                    break;
                }
                case 6: {
                    string confirm;
                    do {
                        cout << "Confirm Exit?(y/n)? ";
                        getline(cin, confirm);
                        confirm = tolower(*confirm.c_str());
                        if (confirm == "y") {
                            auto k = [](string studentName, string studentID, string tutorGroup) {
                                printf("%-11s %-9s %-4s\n", studentName.c_str(), studentID.c_str(), tutorGroup.c_str());
                            };
                            // Print the group members
                            k("LUO Jia Wei", "23063148A", "B03A");
                            k("Siu Lok", "23092746A", "B03C");
                            k("Shih Richard", "23082415A", "B03B");
                            k("Ng Ka Ming", "23077823A", "B03C");
                            k("Liao Jia Ron", "23154894A", "B03C");
                            k("Kam Yiu Hei", "23091497A", "B03B");

                        } else if (confirm == "n") {
                            select = -1;
                        } else {
                            cout << "wrong input please try again\n";
                        }
                    } while (confirm != "y" && confirm != "n");
                    break;
                }
                default:
                    cout << "Wrong Input , Please try again" << endl;
            }
        } catch (invalid_argument e) {
            cout << "Wrong Input , Please try again" << endl;
        }

    } while (select != 6);
    return 0;


}
