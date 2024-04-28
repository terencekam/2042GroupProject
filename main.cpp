/**
Discrimination: This program is using 360-day calendar for the calculation of the rank of the customer
1 Year = 360 days
6 Months = 180 days
1 Month = 30 days

All the program used std::getline to get the input from the user so as to avoid the buffer overflow
To decrease the complexity of the code, I used used lambda function.

 This project is built under:
1. C++20
2. CMake 3.21.3
3. GCC 11.2.0
4. Clang 13.0.0

 Dependencies:
1. fmt 8.0.1 or replace the fmt:: to std:: but std::format should be available
**/
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
#include <sstream>
#include <fmt\core.h>
//#include </home/runner/C20-Template-1/fmt/core.h>

// Define namespace
using namespace std;

// Forward declaration of Customer class
class Customer;

// Forward declaration of GiftRecord class
class GiftRecord;

// Function to delete a customer
bool hasLoadData = false;

// Enum for Rank
enum Rank {
    G, S, B
};

// Enum for level of logs so as to differentiate the level between the logs
enum level {
    warn, error, info
};

// Used Enum for GiftCategory so as to get the category of the gift
enum GiftCategory {
    a, b, c, d
};

// Map to convert Rank to string so that them enum can be printed without using switch case or if else
map<Rank, string> RanktoString = {
        {G, "G"},
        {S, "S"},
        {B, "B"}
};

//By using map we can convert the enum to string so as to print the level of logs
map<level, string> levelToString = {
        {warn, "WARN "},
        {error, "ERROR"},
        {info, "INFO "}
};

/** Map which combines the enum of GiftCatrgory and a pair of string(category)
 and string(description) so as to print the category of the gift
**/
map<GiftCategory, pair<string, string>> GiftCategoryToString = {
        {a, make_pair("A" , "Audio & Video")},
        {b, make_pair("B", "Kitchenware")},
        {c, make_pair("C", "Coupons")},
        {d, make_pair("D", "Computer Accessories")},
};

// Map to convert the string to GiftCategory so as to get the category of the gift
map<string, GiftCategory> StringToGiftCategory = {
        {"A" , a},
        {"B" , b},
        {"C" , c},
        {"D" , d}
};

// Time for logs
time_t now = time(0);
tm* ltm = localtime(&now);

// File to store logs so as to keep track of the transactions
ofstream Filelog;

// Custom Exception
// Used polymorphism to override the what() function so as to get the custom message when the exception is thrown
class RangeException : exception {
public:
    const string m_msg;

    explicit RangeException(string msg) : m_msg(std::move(msg)) {
        cout << m_msg << "\n";
    }

    // Override the what() function to get the custom message
    virtual const char* what() const throw() {
        return m_msg.c_str();
    }
};

// Class for Logger
class Logger {

public:
// Allowed to be overridden so as to make it polymorphic
    virtual void log(string log, level l) {
        auto localtime = asctime(ltm);
        localtime[strlen(localtime) - 1] = 0;
        cout << localtime << "[" << levelToString[l] << "]" << log << "\n";
        Filelog.open("latest.log", std::ofstream::app);
        Filelog << localtime << "[" << levelToString[l] << "]" << log << "\n";
        Filelog.close();
    }
// Allowed to be overridden so as to make it polymorphic
    virtual void printLog() {
        string s;
        ifstream getFile("latest.log");
        while (getline(getFile, s)) {
            cout << s << endl;
        }
    }
};

Logger logger;

/** Class for Customer which inherits Logger and overrides the log and printLog function.
It have a private log so as to store the logs of a specific customer
**/
class Customer : Logger {
private:
    string privatelog;
    string CustomerID = "";
    Rank Ranking = B;
    int PointBalance = 0;
public:
    // Default constructor
    Customer() = default;
// Constructor of Customer that takes in the CustomerID, Rank and PointBalance , usually used when adding a new customer
    Customer(string customerID, Rank rank, int pointBalance) {
        if (pointBalance < 0) {
            throw RangeException("Error: Point Balance < 0\n");
        }
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

    int getPointBalance() const {
        return PointBalance;
    }

    void addPointBalance(int amount) {
        //protect the point balance from being negative
        if (amount < 0) {
            cout << "No Input occur amount<0\n";
            return;
        }
        PointBalance += amount;
        // Log the transaction by the override log method from Logger
        this->log((fmt::format("Customer with ID= '{0}' added Point = '{1}, new Point = {2}'", CustomerID, amount, PointBalance)), info);
    }

    bool minusPointBalance(int amount) {
        if (amount < 0) {
            //protect the point balance from being negative
            cout << "No Input occur amount<0\n";
            return false;
        }
        if (PointBalance >= amount) {
            PointBalance -= amount;
            // Log the transaction by the override log method from Logger
            this->log(fmt::format("Customer with ID= '{0}': deducted Point = '{1}, new Point = {2}'", CustomerID, amount, PointBalance), info);
            return true;
        }
        return false;
    }

    void setPointBalance(int pointBalance) {
        if (pointBalance < 0) {
            //protect the point balance from being negative
            cout << "No Input occur amount<0\n";
            this->log(fmt::format("ERROR: Customer with ID = '{0}' trying to set a negative Point , Want to set Point : {1} , Old Point: {2} ", CustomerID, pointBalance, this->PointBalance), error);
        }
        auto oldPoint = this->PointBalance;
        this->PointBalance = pointBalance;
        // Log the transaction by the override log method from Logger
        this->log(fmt::format("Customer with ID = '{0}' set the Point as '{1}' , old point: {2}", CustomerID, pointBalance, oldPoint), info);
    }

    // print the customer details
    void toString() {
        printf("%-15s %-4s %-d\n", CustomerID.c_str(), RanktoString[Ranking].c_str(), PointBalance);
    }

    /** Override the log function from Logger so as to store the logs of a specific customer
     * as well as log it to the super class so that it can be log on the log file
     */
    void log(string Log, level l) override {
        auto localtime = asctime(ltm);
        localtime[strlen(localtime) - 1] = 0;
        ostringstream os;
        os << localtime << "[" << levelToString[l] << "]" << Log << "\n";
        privatelog += os.str();
        Logger::log(Log, l);
    }

    void printLog() override {
        if (this->privatelog.empty()) {
            cout << "No Transaction on Customer: " << this->CustomerID << endl;
            return;
        }
        cout << this->privatelog;
    }
};

// Vector to store list of customers
vector<Customer> customerList;

// Struct for GiftRecord
struct GiftRecord {

    string GiftID;
    string GiftDiscription;
    int price{};
    int PointRequired{};
    enum GiftCategory giftCategory;

    // Constructor for GiftRecord
    GiftRecord(const string& gift_id, const string& gift_discription, int price, int point_required) {
        giftCategory = StringToGiftCategory[gift_id.substr(0, 1)];
        GiftID = gift_id;
        GiftDiscription = gift_discription;
        this->price = price;
        PointRequired = point_required;
    }

    // print the gift record details
    void toString() const {
        printf("%-5s %-7s %-10d %-17d %s\n", GiftCategoryToString[giftCategory].first.c_str(), GiftID.c_str(), price, PointRequired, GiftDiscription.c_str());
    }
};

// Vector to store list of gift records. If using list, the sorting will be slower. Random access is faster in vector
vector<GiftRecord> giftRecordList;

// used tm so as to get the date of today
struct tm* today = localtime(&now);

// Function to delete a customer . To decrease the complexity of the code, used lambda function. Passing one annonymous customer object
auto DeleteCustomer = [](string CustomerID) {
    customerList.erase(remove_if(customerList.begin(), customerList.end(), [&CustomerID](Customer o) {
        return CustomerID == o.getCustomerID();
    }), customerList.end());
};

// Function to get auto rank
Rank getAutoRank(tm* date) {

    auto t = mktime(today);
    auto OtherDate = mktime(date);
    // check if the date is 1 year ago , if yes return G
    if (t - OtherDate > 31104000) { // 1 year
        return G;
    }
    // check if the date is 6 months ago , if yes return S
    else if (t - OtherDate > 15552000) { // 6 months
        return S;
    }
    else {
        // if the date is less than 6 months, return B
        return B;
    }
}

// Function to check if a customer exists
bool HasCustomer(string CustomerID) {
    for (auto customer_list : customerList) {
        if (customer_list.getCustomerID() == CustomerID) {
            return true;
        }
    }
    return false;
}

// Function to get a customer
Customer GetCustomer(string CustomerID) {
    for (Customer customer_list : customerList) {
        if (customer_list.getCustomerID() == CustomerID) {
            return customer_list;
        }
    }
    // Return empty customer
    return { "", G, -1 };
}

// Function to add a customer
bool AddCustomer(Customer c) {

    // Check if the customer exists
    if (HasCustomer(c.getCustomerID())) {
        return false;
    }
    // Check if the point balance is negative
    if (c.getPointBalance() < 0) {
        cout << "No Input occur amount<0\n";
    }
    // Add the customer to the list
    customerList.emplace_back(c);
    // Print the customer details
    cout << fmt::format("Added new Customer to CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'\n",
                        c.getCustomerID(), RanktoString[c.getRank()], c.getPointBalance());
    // Log the transaction
    logger.log(fmt::format("Added new Customer to CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'",
                           c.getCustomerID(), RanktoString[c.getRank()], c.getPointBalance()), info);
    return true;
}

// Function to modify a customer
void ModifyCustomer(Customer c) {
    // Delete the customer and add the new customer since vector does not have update function
    DeleteCustomer(c.getCustomerID());
    customerList.emplace_back(c);
}

// Function to initialize data
void initialsiation() {
    customerList = { Customer("Tommy2015", B, 8500),
                     Customer("DavidChan", B, 22800),
                     Customer("Luna123", B, 650),
                     Customer("TigerMan", B, 14000),
                     Customer("Max5678", S, 2580),
                     Customer("Neo2000", S, 8000),
                     Customer("CCTang", S, 33554),
                     Customer("EchoWong", G, 8650),
                     Customer("Rubychow", G, 28000),
                     Customer("Ivy2023", G, 12340) };
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
    for (const auto& item : giftRecordList) {
        if (item.GiftID == giftId) {
            return true;
        }
    }
    return false;
}

GiftRecord getGiftRecord(string giftId) {
    // Get the gift record
    for (const auto& item : giftRecordList) {
        if (item.GiftID == giftId) {
            return item;
        }
    }
    // Return empty gift record
    return { "", "", 0, 0 };
}

/**Since unix time is used, the time is calculated by the number of seconds since 1970-01-01 00:00:00. However, we added 1900
to the year and 1 to the month since the tm struct is 1900-based year and 0-based month
**/

// Function to check if a date is correct.If the date correct, return a tm struct of the date. Otherwise, throw an exception
tm isCorrectDate(string date) {
    const char* dateToChar = date.data();
    if (strlen(dateToChar) != 8) {
        throw length_error("The date length size != 8\n");
    }
    try {
        stoi(date);
    }
    catch (invalid_argument e) {
        throw invalid_argument("The date are not numbers!\n");
    }

    // Check if the date is negative
    if (stoi(date) < 0) {
        throw RangeException("Error: Date < 0");
    }

    // Get the year, month and day from the date of string
    int year = stoi(date.substr(4, 4)); //DDMMYYY
    int month = stoi(date.substr(2, 2));
    int day = stoi(date.substr(0, 2));

    // Check if the year is correct
    if (year > today->tm_year + 1900) {
        throw RangeException(fmt::format("Year > {}\n", today->tm_year + 1900));
    }
    else if (year == today->tm_year + 1900) {
        if (month > today->tm_mon + 1) {
            throw RangeException(
                    fmt::format("Month > {}, Since this Year = {}\n", today->tm_mon + 1, today->tm_year + 1900));
        }
        else if (month == today->tm_mon + 1 && day > today->tm_mday) {
            throw RangeException(fmt::format("Day > {}, Since this Year = {}, this Month = {}\n", today->tm_mday,
                                             today->tm_year + 1900, today->tm_mon + 1));
        }
    }
    if (day <= 0) {
        throw RangeException("Error: Days < 0\n");
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
            // Check if the month has 31 days
            if (day > 31) {
                throw RangeException("Error: Days > 31\n");
            }
            break;
        }
        case 2: {
            if (year % 4 == 0) {
                // Check if the year is a leap year
                if (day > 29) {
                    throw RangeException("Error: Days > 29\n");
                }

            }
            else if (day > 28) {
                // Check if the year is not a leap year
                throw RangeException("Error: Days > 28\n");
            }
            break;
        }
        case 4:
        case 6:
        case 9:
        case 11: {
            // Check if the month has 30 days
            if (day > 30) {
                throw RangeException("Error: Days > 30\n");
            }
            break;
        }
        default:
            throw RangeException("Error: The Month is not is range: 0 > month > 12\n");
    }
    auto t = [&year, &day, &month] {
        struct tm t = { 0 };
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
    }
    else {
        cout << "Can't find customer , please try again!\n";
        return;
    }
    do {
        // Get the old point balance so as to compare the new point balance
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
                        int tempsum = customer.getPointBalance() + (money / 250);
                        if (tempsum > 0) { // Overflow check
                            if (money > 0) {
                                int points = (int)(money / 250);
                                if (points == 0) {
                                    cout << "No points added!Since money<250\n";
                                }
                                else {
                                    customer.addPointBalance(points); // Add points to customer
                                    cout << "Points added!\n";
                                }
                            }
                            else {
                                cout << "Wrong Input , Please try again, amount < 0\n";
                                break;
                            }
                        }
                        else {
                            cout << "Wrong Input , Please try again, amount < 0\n";
                        }
                    }
                    catch (exception e) {
                        cout << "Wrong input , PLease try again\n";
                    }
                    break;
                }
                case 2: {
                    int end = false;
                    int times = 0;
                    do {
                        try {
                            string temp;
                            printf("%-5s : %s", "type", "Gift Category\n");
                            for_each(GiftCategoryToString.begin(), GiftCategoryToString.end(), [](
                                    const pair<const GiftCategory, pair<basic_string<char>, basic_string<char>>>& i) {
                                printf("%-5s : %s\n", GiftCategoryToString[i.first].first.c_str(), i.second.second.c_str());
                            });
                            cout << "Please input the Category type You want to redeem: ";
                            getline(cin, temp); // Get category
                            char category[10];
                            strcpy(category, temp.c_str());
                            // Check if category is valid
                            if (69 > static_cast<int>(*category) && static_cast<int>(*category) > 64) {
                                sort(giftRecordList.begin(), giftRecordList.end(), // Sort gift records
                                        // Lambda function to sort gift records , passing two annonymous gift record objects a and b
                                     [](GiftRecord a, GiftRecord b) {
                                         if (a.PointRequired < b.PointRequired) {
                                             return true;
                                         }
                                         return false;
                                     }
                                );
                                int InternalCount = 0;
                                times = 0;
                                do {
                                    // Get points required based on rank by using lambda function, passing annonymous customer object and gift record object
                                    auto getPointRequired = [](Customer c, GiftRecord g) {
                                        switch (c.getRank()) {
                                            case G:
                                                return g.PointRequired * 0.9;
                                            case S:
                                                return g.PointRequired * 0.95;
                                            default:
                                                return (double)g.PointRequired;
                                        }
                                    };
                                    printf("%-5s %-7s %-10s %-28s %s\n", "Type", "GiftID", "Price(HKD)", "Points Required", "Gift Description");
                                    // For each gift record, print the gift record details
                                    for_each(giftRecordList.begin(), giftRecordList.end(),
                                             // Lambda function to print gift record details, passing annonymous gift record object j and reference to category, customer and getPointRequired
                                             // This lambda expression also used another lambda expression to get the points required based on rank
                                             [&category, &customer, &getPointRequired](GiftRecord j) {
                                                 auto PointRequired = getPointRequired(customer, j);
                                                 if (tolower(*GiftCategoryToString[j.giftCategory].first.c_str()) == tolower(*category)) {

                                                     // Print gift record details. After converting the Point required of the customer by getting the discount based on the rank, it will be printed ref: PointRequired
                                                     printf("%-5s %-7s %-10d %-7d%-21s %s\n",
                                                            GiftCategoryToString[j.giftCategory].first.c_str(),
                                                            j.GiftID.c_str(),
                                                            j.price,
                                                            (int)PointRequired,
                                                            PointRequired <= customer.getPointBalance() ? "(enough Points!)" : "(not enough Points!)",
                                                            j.GiftDiscription.c_str());
                                                 }
                                             });
                                    cout << "Please Select: ";
                                    getline(cin, temp);
                                    if (!temp.empty()) {
                                        if (HasGiftRecord(temp)) {

                                            InternalCount = 0;
                                            do {
                                                GiftRecord g = getGiftRecord(temp);
                                                auto PointRequired = getPointRequired(customer, g);
                                                // A lambda function to get the extra pay if the customer don't have enough points. Based on the rank of the customer, the extra pay will be calculated
                                                auto extraPay = [&PointRequired](Customer c, const GiftRecord& g) {
                                                    switch (c.getRank()) {
                                                        case G:
                                                            return (PointRequired - c.getPointBalance()) * 0.2;
                                                        case S:
                                                            return (PointRequired * 0.95 - c.getPointBalance()) * 0.2;
                                                        default:
                                                            return (PointRequired - c.getPointBalance()) * 0.2;
                                                    }
                                                };
                                                if (
                                                        PointRequired <= customer.getPointBalance()) {
                                                    cout << "You can get the gift freely!\ncontinue?(y/n): ";
                                                }
                                                else {
                                                    printf("You have not enough points, you may pay extra of $%.0f to redeem the gift and all the points will be deducted, continue?(y/n) ",
                                                           extraPay(customer, g));
                                                }
                                                getline(cin, temp);
                                                if (!temp.empty()) {
                                                    if (temp == "y") {
                                                        if (PointRequired <= customer.getPointBalance()) {
                                                            // Deduct the points if the customer have enough points
                                                            customer.minusPointBalance((int)PointRequired);
                                                            // log the transaction
                                                            customer.log(fmt::format("Customer with ID = '{0}' redeemed the gift with ID = '{1}' , Points Required = '{2}' , Price = $'{3}' , Description = '{4}'",
                                                                                     customer.getCustomerID(), g.GiftID, PointRequired, g.price, g.GiftDiscription), info);
                                                            cout << "redeemed!\n";
                                                            end = true;
                                                            break;
                                                        }
                                                        else {
                                                            // Deduct the points and add the extra pay if the customer don't have enough points
                                                            customer.setPointBalance(0);
                                                            // log the transaction
                                                            customer.log(fmt::format("Customer with ID = '{0}' redeemed the gift with ID = '{1}' , Points Required = '{2}' , Price = $'{3}' , Description = '{4}' , ExtraPay = ${5}",
                                                                                     customer.getCustomerID(), g.GiftID, PointRequired, g.price, g.GiftDiscription, extraPay(customer, g)), info);
                                                            cout << "redeemed!\n";
                                                            end = true;
                                                            break;
                                                        }
                                                    }
                                                    else if (temp == "n") {
                                                        break;
                                                    }
                                                    else {
                                                        cout << "Wrong input received!\n";
                                                    }
                                                }
                                                else {
                                                    cout << "No input received!\n";
                                                }
                                                InternalCount++;
                                                times++;
                                            } while (InternalCount <= 2);
                                        }
                                        else {
                                            cout << "Wrong input , please try again!\n";
                                        }
                                    }
                                    else {
                                        cout << "no input , please try again!\n";
                                    }
                                    if (end) {
                                        break;
                                    }
                                    InternalCount++;
                                    times++;
                                } while (InternalCount <= 2);
                            }
                            else {
                                cout << "Wrong input received!\n";
                                throw invalid_argument("Wrong input received!\n");
                            }
                        }
                        catch (exception e) {
                            times++;
                        }
                        if (end) {
                            break;
                        }
                        if (times >= 2) {
                            cout << "Input error > 3... Exiting\n";
                        }
                    } while (times <= 2);
                    break;
                }
                case 3:
                    // modify the point balance of the customer
                    while (true) {
                        cout << "Your CC Points : " << customer.getPointBalance() << endl;
                        cout << "Set a new cc Points: ";
                        string temp;
                        getline(cin, temp);
                        try {
                            auto points = stoi(temp);
                            if (points >= 0) {
                                customer.setPointBalance(points);
                                cout << "operation complete!" << endl;
                            }
                            else {
                                cout << "Wrong Input , Please try again" << endl;
                                break;
                            }
                        }
                        catch (exception e) {
                            cout << "wrong input , Please try again!" << endl;
                        }
                        break;
                    }
                case 4:
                    break;
                default:
                    cout << "Wrong input , please try againn";
            }
        }
        catch (exception e) {
            cout << "Wrong input , please try again!";
        }
        // Print the new point balance of the customer as well as the change
        if (customer.getPointBalance() > oldPoint) {
            printf("Customer with ID: %s added Point of : %i (Old Point: %i , New Point: %i) ...Full logs on latest.log or view it on transaction\n",
                   customer.getCustomerID().c_str(), customer.getPointBalance() - oldPoint, oldPoint,
                   customer.getPointBalance());
        }
        else if (customer.getPointBalance() < oldPoint) {
            printf("Customer with ID: %s reduced Point of : %i (Old Point: %i , New Point: %i) ...Full logs on latest.log or view it on transaction\n",
                   customer.getCustomerID().c_str(), oldPoint - customer.getPointBalance(), oldPoint,
                   customer.getPointBalance());
        }
        else {
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
                    // using the lambda function to sort the customer and gift record
                    sort(giftRecordList.begin(), giftRecordList.end(), [](GiftRecord a, GiftRecord b) {
                        int maxCount;
                        /** Sorting method: if the length of the giftID of a is larger than b, maxCount will be the length of b,
                        vise versa for a. Then, compare the giftID of a and b by converting them to lowercase and loop through the length of the giftID.
                        If the character of a is larger than b, return false, else return true. If the character of a is smaller than b,
                        return true, else return false.

                         Vise versa for the customerID
                        **/
                        if (strlen(a.GiftID.c_str()) > strlen(b.GiftID.c_str())) {
                            maxCount = strlen(b.GiftID.c_str());
                        }
                        else {
                            maxCount = strlen(a.GiftID.c_str());
                        }
                        for (int i = 0; i < maxCount; ++i) {
                            if (tolower(a.GiftID[i]) > tolower(b.GiftID[i])) {
                                return false;
                            }
                            else if (tolower(a.GiftID[i]) < tolower(b.GiftID[i])) {
                                return true;
                            }
                        }
                        return false;
                    });
                    sort(customerList.begin(), customerList.end(), [](Customer a, Customer b) {
                        int maxCount;
                        if (strlen(a.getCustomerID().c_str()) > strlen(b.getCustomerID().c_str())) {
                            maxCount = strlen(b.getCustomerID().c_str());
                        }
                        else {
                            maxCount = strlen(a.getCustomerID().c_str());
                        }
                        for (int i = 0; i < maxCount; ++i) {
                            if (tolower(a.getCustomerID().c_str()[i]) > tolower(b.getCustomerID().c_str()[i])) {
                                return false;
                            }
                            else if (tolower(a.getCustomerID().c_str()[i]) < tolower(b.getCustomerID().c_str()[i])) {
                                return true;
                            }
                        }
                        return false;
                    });
                    // Print the customer and gift record details
                    printf("%-15s %-4s %-s\n", "Customer ID", "Rank", "Point Balance");
                    for (auto customer_list : customerList) {
                        customer_list.toString();
                    }
                    printf("%-5s : %s", "Type", "Gift Category\n");
                    for_each(GiftCategoryToString.begin(), GiftCategoryToString.end(), [](
                            // Print the gift category by using the lambda function
                            pair<const GiftCategory, pair<basic_string<char>, basic_string<char>>> i) {
                        printf("%-5s : %s\n", GiftCategoryToString[i.first].first.c_str(), i.second.second.c_str());
                    });
                    printf("%-5s %-7s %-10s %-17s %s\n", "Type", "GiftID", "Price(HKD)", "Points Required", "Gift Description");
                    for (auto giftRecord : giftRecordList) {
                        giftRecord.toString();
                    }
                    break;
                }

                case 3: {
                    int WrongInput = 0;
                    do {
                        try {
                            string customerID;
                            cout << "Please input an Customer ID: ";
                            string next;
                            getline(cin, customerID);
                            if (customerID.empty()) {
                                cout << "No input found!\n";
                                throw invalid_argument("No input found\n");
                            }
                            for (int i = 0; i < strlen(customerID.c_str()); i++) {
                                if (customerID.c_str()[i] == ' ') {
                                    cout
                                            << "There are space in the CustomerID! Please remove the space and try again! \n";
                                    throw invalid_argument("Space in CustomerID\n");
                                }
                            }
                            string choice;
                            if (HasCustomer(customerID)) {
                                int InternalCount = 0;
                                do {
                                    auto c = GetCustomer(customerID);
                                    cout << fmt::format(
                                            "Customer: CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'\n",
                                            c.getCustomerID(), RanktoString[c.getRank()], c.getPointBalance());
                                    cout << "Are you sure to remove the customerID?(y/n)\n";
                                    getline(cin, choice);
                                    choice = tolower(*choice.c_str());
                                    if (choice == "y") {
                                        logger.log(fmt::format(
                                                "Customer with ID = '{0}' removed , CustomerRank = '{1}' , PointBalance= '{2}",
                                                customerID, RanktoString[c.getRank()], c.getPointBalance()), warn);
                                        DeleteCustomer(customerID);
                                    }
                                    if (choice != "y" && choice != "n") {
                                        cout << "Wrong Input , please try again!\n";
                                    }
                                    InternalCount++;
                                    // If the input is wrong for 3 times, exit
                                    if (InternalCount == 3) {
                                        WrongInput = 3;
                                        throw invalid_argument("Wrong Input > 3\n");
                                    }
                                } while (choice != "y" && choice != "n");
                                break;
                            }
                            else {
                                // if the customerID larger than 50 , it will not be added
                                if (customerID.size() > 50) {
                                    cout << "Error ... CustomerID length > 50\n";
                                    break;
                                }
                                int count = 0;
                                do {
                                    if (count > 2) {
                                        break;
                                    }
                                    cout << "No Customer ID found ... Are you sure to add the Customer?(y/n)\n";
                                    getline(cin, choice);
                                    choice = tolower(*choice.c_str());
                                    if (choice != "y" && choice != "n") {
                                        cout << "Wrong Input , please try again!\n";
                                    }
                                    count++;
                                } while (choice != "y" && choice != "n");
                                if (choice != "y" && choice != "n") {
                                    cout << "Wrong Input > 3, exiting...\n";
                                    break;
                                }
                                if (choice == "n") {
                                    break;
                                }
                                string tempDate;
                                tm date;
                                int InternalCount = 0;
                                do {
                                    // Get the date
                                    cout
                                            << "Please input a date with DDMMYYYY(There should be 8 character of integer , or otherwise an error will occur. You may type 'today'(case sensitive) such that the customer join as member today)\n";
                                    getline(cin, tempDate);
                                    try {
                                        if (tempDate == "today") {
                                            date = *today;
                                            break;
                                        }
                                        else {
                                            date = isCorrectDate(tempDate);
                                            break;
                                        }
                                    }
                                    catch (RangeException e) {
                                        InternalCount++;
                                        cout << "Please try again...\n";
                                    }
                                    catch (length_error e) {
                                        InternalCount++;
                                        cout << "Please try again...\n";
                                    }
                                    // If the input is wrong for 3 times, exit
                                    if (InternalCount == 3) {
                                        WrongInput = 3;
                                        throw invalid_argument("Wrong Input > 3\n");
                                    }
                                } while (true);
                                int PointBalance;
                                InternalCount = 0;
                                do {
                                    try {
                                        // Get the initial points
                                        cout << "Input the Initial points:";
                                        string tempinput;
                                        getline(cin, tempinput);
                                        if (tempinput.empty()) {
                                            throw invalid_argument("Empty Input\n");
                                        }
                                        PointBalance = stoi(tempinput);
                                        if (stoi(tempinput) != PointBalance) {
                                            throw invalid_argument("Overflow\n");
                                        }
                                        if (PointBalance < 0) {
                                            cout << "The Point should not be > 0\n";
                                            throw RangeException("Error: Point Balance < 0\n");
                                        }
                                    }
                                    catch (invalid_argument e) {
                                        cout << "Wrong Input , Please try again" << endl;
                                        PointBalance = -1;
                                        InternalCount++;
                                    }
                                    catch (RangeException e) {
                                        cout << "Wrong Input , Please try again" << endl;
                                        PointBalance = -1;
                                        InternalCount++;
                                    }
                                    catch (exception e) {
                                        cout << "Wrong Input , Please try again(This may cased by overflow)" << endl;
                                        PointBalance = -1;
                                        InternalCount++;
                                    }
                                    // If the input is wrong for 3 times, exit
                                    if (InternalCount >= 3) {
                                        WrongInput = 3;
                                        throw invalid_argument("Wrong Input > 3\n");
                                    }
                                } while (PointBalance < 0);
                                AddCustomer(Customer(customerID, getAutoRank(&date), PointBalance));
                            }

                        }
                        catch (exception e) {
                            WrongInput++;
                        }
                        if (WrongInput >= 3) {
                            cout << "Wrong Input > 3, exiting...\n";
                            break;
                        }
                    } while (WrongInput < 3);
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
                    }
                    else if (customerID == "all") {
                        logger.printLog();
                    }
                    else {
                        cout << "Wrong Type , Please try again! The System can't find the customer\n";
                    }
                    break;
                }
                case 6: {
                    string confirm;
                    do {
                        try {
                            cout << "Confirm Exit?(y/n)? ";
                            getline(cin, confirm);
                            confirm = tolower(*confirm.c_str());
                            if (confirm == "y") {
                                printf("%-13s %-9s %s\n", "Name", "Student ID", "Tutor Group");
                                // Lambda function to print the group members
                                auto k = [](string studentName, string studentID, string tutorGroup) {
                                    printf("%-13s %-9s %s\n", studentName.c_str(), studentID.c_str(), tutorGroup.c_str());
                                };
                                // Print the group members
                                k("LUO Jia Wei", "23063148A", "B03A");
                                k("Siu Lok", "23092746A", "B03C");
                                k("Shih Richard", "23082415A", "B03B");
                                k("Ng Ka Ming", "23077823A", "B03C");
                                k("Liao Jia Ron", "23154894A", "B03C");
                                k("Kam Yiu Hei", "23091497A", "B03B");

                            }
                            else if (confirm == "n") {
                                select = -1;
                            }
                            else {
                                cout << "wrong input please try again\n";
                            }
                        }
                        catch (exception e) {
                            cout << "wrong input ,please try again\n";
                        }
                    } while (confirm != "y" && confirm != "n");
                    break;
                }
                default:
                    cout << "Wrong Input , Please try again" << endl;
            }
        }
        catch (invalid_argument e) {
            cout << "Wrong Input , Please try again" << endl;
        }
        catch (exception e) {
            cout << "Wrong Input , Please try again" << endl;
        }

    } while (select != 6);
    return 0;
}
