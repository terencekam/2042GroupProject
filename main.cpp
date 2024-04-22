// Include necessary libraries
#define FMT_HEADER_ONLY
#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <map>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <fstream>
#include </home/runner/"YOUR_PROJECT_NAME"/fmt/core.h>

// Forward declaration of Customer class
class Customer;
// Forward declaration of GiftRecord class
class GiftRecord;
// Define namespace
using namespace std;
// Function prototypes
bool Fo(char , int);
bool HasCustomer(char CustomerID);
bool DeleteCustomer(char CustomerID);
bool AddCustomer(Customer c) ;
bool hasLoadData = false;
// Enum for Rank
enum Rank {
    G ,S ,B
};

// Map to convert Rank to string
map<Rank , string> RanktoString = {
        {G , "G"},
        {S , "S"},
        {B , "B"}
};
time_t now = time(0);
tm *ltm = localtime(&now);
ofstream Filelog("latest.log");
class Logger{
    enum level{
        w , e , i
    };
    map<level , string> levelToString = {
            {w , "WARN"},
            {e , "ERROR "},
            {i , "INFO"}
    };
private:
    void logto(string log , level l){
        Filelog << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << levelToString[l] << log << endl;
    }
public:
    void warn(string log){
        logto(log , w);
    }
    void error(string log){
        logto(log , e);
    }
    void info(string log){
        logto(log , i);
    }
    void printLog(){
        string s;
        Filelog.close();
        ifstream getFile("latest.log");
        while(getline(getFile , s)){
            cout << s;
        }
        Filelog.open("latest.log");
    }
};
Logger logger;
// Struct for Date
struct Date{
    Date(int year , int month , int day) {
        Year = year;
        Month = month;
        Day = day;
    }
    int Year;
    int Month;
    int Day;
    // Function to convert Date to string
    string toString() {
        return Year==0?"No Date":"Year: " + std::to_string(Year) + ", Month: " + std::to_string(Month) + ", Day: " + std::to_string(Day);
    }
};
// Get current date

Date today = Date(1900 + ltm->tm_year , 1 + ltm->tm_mon , ltm->tm_mday);
// Function to get automatic Rank based on Date
Rank getAutoRank(Date date) {
    int yeardiff;
    yeardiff = today.Year-date.Year;

    if (yeardiff > 1) {
        return G;
    }
    if (yeardiff > 0) {
        int tempTodayMonth;
        int tempTodayDay;
        int tempDateMonth;
        int tempDateDay;
        if ((today.Month == 2||date.Month == 2) &&(today.Day==29||date.Day==29)) {
            tempTodayMonth = today.Day==29?today.Month==2?3:today.Month:today.Month;
            tempTodayDay = today.Day==29?today.Month==2?1:today.Day:today.Day;
            tempDateMonth = date.Day==29?date.Month==2?3:date.Month:date.Month;
            tempDateDay = date.Day==29?date.Month==2?1:date.Day:date.Day;
        }else {
            tempDateDay = date.Day;
            tempDateMonth = date.Month;
            tempTodayDay = today.Day;
            tempTodayMonth = today.Month;
        }
        int totalMonth = 0;
        totalMonth = 12 - tempDateMonth;
        tempDateMonth = 0;

        if (tempTodayMonth - tempDateMonth > 1) {
            totalMonth += tempTodayMonth - tempDateMonth -1;
        }
        if (tempTodayDay <= tempDateDay) {
            totalMonth++;
        }
        if (totalMonth >=12) {
            return G;
        }
        if (totalMonth >= 6) {
            return S;
        }
        return B;
    }
}

// Enum for GiftCategory
enum GiftCategory {
    a ,b ,c , d
};

// Map to convert GiftCategory to string
map<GiftCategory , string> GiftCategoryToString = {
        {a , "Audio & Video"},
        {b , "Kitchenware"},
        {c , "Coupons"},
        {d , "Computer Accessories"},
};

// Class for Customer
class Customer {
private:
    string CustomerID = "";
    Rank Ranking = B;
    int PointBalance = 0;
public:
    Customer(){};
    Customer(string customerID , Rank rank , int pointBalance) {
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
        PointBalance += amount;
        logger.info(fmt::format("Customer with ID= '{0}' added Point = '{1}'" , CustomerID , amount));
    }
    bool minusPointBalance(int amount){
        if(PointBalance>=amount){
            PointBalance-=amount;
            logger.info(fmt::format("Customer with ID= '{0}' deducted Point = '{1}'" , CustomerID , amount));
            return true;
        }
        return false;
    }
    void setPointBalance(int PointBalance) {
        this -> PointBalance = PointBalance;
        logger.info(fmt::format("Customer with ID = '{0}' set the Point as '{1}'" , CustomerID , PointBalance));
    }
    void toString() {
        printf("%-15s %-s %-d\n",CustomerID.c_str(), RanktoString[Ranking].c_str() , PointBalance);
    }
};

// Vector to store list of customers
vector<Customer> customerList;

// Struct for GiftRecord
struct GiftRecord{

    char *GiftID = new char[3];
    char *GiftDiscription = new char[100];
    int price{};
    int PointRequired{};
    enum GiftCategory giftCategory;
    GiftRecord(char *gift_id, char *gift_discription, int price, int point_required){
        giftCategory = static_cast<GiftCategory>(gift_id[0]);
        GiftID = gift_id;
        GiftDiscription = gift_discription;
        this -> price = price;
        PointRequired = point_required;
    }
    void toString(){
        printf("%-3s %-5d %-5d %s\n",GiftID , price , PointRequired , GiftDiscription);
    }
};

// Vector to store list of gift records
vector<GiftRecord> giftRecordList ;

// Function to check if a customer exists
bool HasCustomer(string CustomerID) {
    for (auto customer_list : customerList) {
        if(customer_list.getCustomerID() == CustomerID) {
            return true;
        }
    }
    return false;
}

// Function to get a customer
Customer GetCustomer(string CustomerID) {
    for (auto customer_list : customerList) {
        if(customer_list.getCustomerID() == CustomerID) {
            return customer_list;
        }
    }
    return {"" , G , -1};
}

// Function to delete a customer
bool DeleteCustomer(string CustomerID) {
    if(HasCustomer(CustomerID)) {
        for (int i = 0 ; i< customerList.size() ; i++) {
            if(customerList[i].getCustomerID() == CustomerID) {
                auto c = GetCustomer(CustomerID);
                cout <<fmt::format("Deleted Customer to CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'",c.getCustomerID() , RanktoString[c.getRank()] , c.getPointBalance());
                customerList.erase(customerList.begin()+i);
                logger.warn(fmt::format("Customer with customerID = '{0}' was deleted" , CustomerID));
            }
        }
        return true;
    }
    return false;
}

// Function to add a customer
bool AddCustomer(Customer c) {
    if(HasCustomer(c.getCustomerID())) {
        return false;
    }
    customerList.emplace_back(c);
    cout <<fmt::format("Added new Customer to CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'",c.getCustomerID() , RanktoString[c.getRank()] , c.getPointBalance());
    logger.info(fmt::format("Added new Customer to CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'",c.getCustomerID() , RanktoString[c.getRank()] , c.getPointBalance()));
    return true;
}

// Function to check if a record exists
bool HasRecord(GiftRecord r) {
    for (auto gift_record_list : giftRecordList) {
        if (r.GiftID == gift_record_list.GiftID) {
            return true;
        }
    }
    return false;
}

// Function to add a record
void AddRecord(GiftRecord r) {
    if(!HasRecord(r)) {
        logger.info(fmt::format("Added new Record: GiftCategory = '{0}' , GiftID = '{1}' , GiftDiscription = '{2}' , Price = '{3}' , PointRequired = {4}" , to_string(r.giftCategory) , r.GiftID , r.GiftDiscription , r.price , r.PointRequired));
        giftRecordList.emplace_back(r);
    }
}

// Function to initialize data
void initialsiation() {
    list<Customer> l = {Customer("Tommy2015",B,8500),
                        Customer("DavidChan",B,22800),
                        Customer("Luna123",B,650),
                        Customer("TigerMan",B,14000),
                        Customer("Max5678",S,2580),
                        Customer("Neo2000",S,8000),
                        Customer("CCTang",S,33554),
                        Customer("EchoWong",G,8650),
                        Customer("Rubychow",G,28000),
                        Customer("Ivy2023",G,12340)};
    for (const auto& customer : l) {
        AddCustomer(customer);
    }
    list<GiftRecord> li = {GiftRecord("A01","LG Internet TV",3900,19000),
                           GiftRecord("A02","Pioneer Hifi Set",2400,11500),
                           GiftRecord("A03","Sony DVD Player",400,2000),
                           GiftRecord("B01","Healthy Air Fryer",1500,7300),
                           GiftRecord("B02","Tefal Microwave Oven",480,2400),
                           GiftRecord("B03","Famous Coffee Maker",1800,8800),
                           GiftRecord("B04","Smart Rice Cooker",600,2900),
                           GiftRecord("B05","TechCook Toaster Oven",450,2250),
                           GiftRecord("C01","Wellcome 50 Coupon",50,250),
                           GiftRecord("C02","Mannings 50 Coupon",50,250),
                           GiftRecord("C03","Carol Restaurant 100 Coupon",100,500),
                           GiftRecord("C04","Shell 200 Coupon",200,960),
                           GiftRecord("D01","Clever Headset ",350,1750),
                           GiftRecord("D02","HP Optic Mouse",250,1250),
                           GiftRecord("D03","Stylish Bluetooth Speaker",800,3900)};
    for (auto gift_record : li) {
        AddRecord(gift_record);
    }
    hasLoadData = true;
}

bool HasGiftRecord(string giftId){
    for (const auto &item: giftRecordList){
        if(item.GiftID==giftId){
            return true;
        }
    }
    return false;
}

GiftRecord getGiftRecord(string giftId){
    for (const auto &item: giftRecordList){
        if(item.GiftID==giftId){
            return item;
        }
    }
    return {"" , "" , 0 , 0};
}

// Function to check if a date is correct
bool isCorrectDate(string date , int *Year , int*Month , int*Day) {
    string tempstr = {date.c_str()[4] , date.c_str()[5] , date.c_str()[6] , date.c_str()[7]}; // Year
    bool isThisYear = false;
    if (stoi(tempstr) < today.Year) {
        *Year = stoi(tempstr);
    }else if (stoi(tempstr) == today.Year) {
        *Year = stoi(tempstr);
        isThisYear = true;
    }else {
        return false;
    }
    tempstr = {date[2] , date[3]};
    if (isThisYear) {
        if (stoi(tempstr) <= today.Month) {
            *Month = stoi(tempstr);
        }else {
            return false;
        }
    }else if (stoi(tempstr)<=12) {
        *Month = stoi(tempstr);
    }else {
        return false;
    }
    tempstr = {date[0] , date[1]};
    if (isThisYear) {
        if (today.Month==*Month){
            if (stoi(tempstr)<=today.Day) {
                *Day = stoi(tempstr);
            }else {
                return false;
            }
        }
    }
    if (2 == *Month&&*Year%4==0) {
        if (stoi(tempstr)<=29) {
            *Day = stoi(tempstr);
        }else {
            return false;
        }
    }
    if (*Month == 1||*Month == 3||*Month ==5 ||*Month == 7||*Month == 8||*Month == 10||*Month == 12) {
        if (stoi(tempstr)<=31) {
            *Day = stoi(tempstr);
        }else {
            return false;
        }
    }else if (*Month == 4||*Month == 6||*Month == 9 ||*Month == 11){
        if (stoi(tempstr)<=30) {
            *Day = stoi(tempstr);
        }else {
            return false;
        }
    }else {
        if (stoi(tempstr)<=28) {
            *Day = stoi(tempstr);
        }else {
            return false;
        }
    }
    return true;
}

// Function for Customer View
void CustomerView(){
    string tempinput;
    int input;
    string customerID;
    Customer customer;
    cout << "Please input CustomerID: ";
    getline(cin , customerID);
    if(HasCustomer(customerID)){
        customer = GetCustomer(customerID);
    }else{
        cout << "Can't find customer , please try again!\n";
        return;
    }
    do {
        int oldPoint = customer.getPointBalance();
        // Get customer ID
        try {
            cout << "\033[1;35m***** Customer View Menu *****\n" // Display menu
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
                    cout << "Input a amount of money";
                    getline(cin, tempmoney);
                    float money;
                    try {
                        money = stof(tempmoney);
                        if (money > 0) {
                            int points = (int) (money / 250);
                            customer.addPointBalance(points); // Add points to customer
                            cout << "Points added!";
                        }
                    } catch (exception e) {
                        cout << "Wrong input , PLease try again";
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
                            char category[1];
                            strcpy(category, temp.c_str());
                            if (69 > static_cast<int>(*category) && static_cast<int>(*category) > 64) {
                                sort(giftRecordList.begin(), giftRecordList.end(), // Sort gift records
                                     [](GiftRecord a, GiftRecord b) { // Lambda function to sort gift records
                                         if (a.PointRequired < b.PointRequired) {
                                             return true;
                                         }
                                         return false;
                                     }
                                );
                                for_each(giftRecordList.begin(), giftRecordList.end(),
                                         [&category , &customer](GiftRecord j) {
                                             if ((char)j.giftCategory == (char) *category){
                                                 printf("%-3s %-5d %-5d%s %s\n",j.GiftID , j.price ,  j.PointRequired , j.PointRequired<=customer.getPointBalance() ? "(enough Points!)": "(not enough Points!)" , j.GiftDiscription);
                                             }
                                         });
                                cout << "Please Select: ";
                                getline(cin , temp);//
                                if(!temp.empty()){
                                    if(HasGiftRecord(temp)){
                                        GiftRecord g = getGiftRecord(temp);
                                        if(g.PointRequired <= customer.getPointBalance()){
                                            cout << "You can get the gift freely!\ncontinue?(y/n): ";
                                        }else{
                                            printf("You have not enough points, you may pay extra of %.0f to redeem the gift and all the points will be deducted, continue?(y/n) ",
                                                   [&customer , &g](){
                                                       switch (customer.getRank()) {
                                                           case G:
                                                               return (g.PointRequired-customer.getPointBalance())*0.9*0.2;
                                                           case S:
                                                               return (g.PointRequired-customer.getPointBalance())*0.9*0.2;
                                                           default:
                                                               return g.PointRequired-customer.getPointBalance()*0.2;
                                                       }
                                                   }.operator()());
                                        }
                                        getline(cin , temp);
                                        if(!temp.empty()){
                                            if(temp=="y"){
                                                if(g.PointRequired <= customer.getPointBalance()){
                                                    customer.minusPointBalance(g.PointRequired);
                                                    cout << "redeemed!\n";
                                                    break;
                                                }else{
                                                    customer.setPointBalance(0);
                                                    cout << "redeemed!\n";
                                                    break;
                                                }
                                            } else if(temp == "n"){
                                                break;
                                            }else{
                                                cout << "Wrong input received!\n";
                                            }
                                        }else{
                                            cout << "No input received!\n";
                                        }
                                    }
                                }else{
                                    cout << "no input , please try again!\n";
                                }
                            }
                        } catch (exception e) {

                        }
                        times++;
                        if(times == 3){
                            cout << "Input error > 3... Exiting\n";
                        }
                    } while (times != 3);
                    break;
                }
                case 3:
                    while (true){
                        cout << "Your CC Points : " << customer.getPointBalance() << endl;
                        cout << "Set a new cc Points: ";
                        string temp;
                        getline(cin , temp);
                        try{
                            auto points = stoi(temp);
                            customer.setPointBalance(points);
                            cout << "operation complete!";
                        }catch (exception e){
                            cout << "wrong input , Please try again!";
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
        if(customer.getPointBalance() > oldPoint){
            printf("Customer with ID: %s added Point of : %i (Old Point: %i , New Point: %i) ...Full logs on latest.log or view it on transaction\n", customer.getCustomerID().c_str() , customer.getPointBalance() - oldPoint , oldPoint , customer.getPointBalance());
        }else if(customer.getPointBalance() < oldPoint){
            printf("Customer with ID: %s reduced Point of : %i (Old Point: %i , New Point: %i) ...Full logs on latest.log or view it on transaction\n", customer.getCustomerID().c_str() , oldPoint - customer.getPointBalance() , oldPoint , customer.getPointBalance());
        }else{
            printf("Customer with ID: %s has Point: %i ...Full logs on latest.log or view it on transaction\n", customer.getCustomerID().c_str() , customer.getPointBalance());
        }
    }while (input!=4);
    customer.toString();
}

// Main function
int main() {
    string tempselect;
    int select = 0;
    do{
        cout << "\033[1;34mWelcome Gift Redemption System\n"
                "*** Main Menu ***\n"
                "[1] Load Starting Data\n"
                "[2] Show Records\n"
                "[3] Edit Customer\n"
                "[4] Enter Customer View\n"
                "[5] Show Transaction History\n"
                "[6] Credit and Exit\033[0m\n";
        cout << "Option (1 - 6) : ";
        getline(cin , tempselect);
        if (tempselect.empty()){
            select = -1;
        }
        try {
            if(!hasLoadData){
                if(tempselect != "1"&&tempselect != "6"){
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
                    sort(giftRecordList.begin() , giftRecordList.end() , []( GiftRecord a , GiftRecord b){
                        if (tolower(*a.GiftID) > tolower(*b.GiftID)) {
                            return false;
                        }
                        return true;
                    });
                    sort(customerList.begin() , customerList.end() , [](Customer a , Customer b){
                        if (tolower(*a.getCustomerID().c_str()) > tolower(*b.getCustomerID().c_str())) {
                            return false;
                        }
                        return true;
                    });
                    for (auto customer_list : customerList) {
                        customer_list.toString();
                    }
                    for (auto giftRecord : giftRecordList) {
                        giftRecord.toString();
                    }
                    break;
                }
                case 3: {
                    string customerID;
                    cout << "Please input an Customer ID: ";
                    string next ;
                    bool passing = true;
                    getline(cin ,customerID);
                    if (customerID.empty()){
                        cout << "No input found!\n";
                        break;
                    }
                    for (int i = 0; i < strlen(customerID.c_str()) ; i++) {
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
                        do{
                            auto c = GetCustomer(customerID);
                            cout << fmt::format("Customer: CustomerID = '{0}' , CustomerRank = '{1}' , PointBalance= '{2}'",c.getCustomerID() , RanktoString[c.getRank()] , c.getPointBalance());
                            cout << "Are you sure to remove the customerID?(y/n)\n";
                            getline(cin , choice);
                            if (choice == "y") {
                                DeleteCustomer(customerID);
                            }
                            if(choice != "y" && choice !="n"){
                                cout << "Wrong Input , please try again!\n";
                            }
                        } while (choice != "y" && choice !="n");
                        break;
                    }else{
                        do {
                            cout << "No Customer ID found ... Are you sure to add the Customer?(y/n)\n";
                            getline(cin , choice);
                            if(choice != "y" && choice !="n"){
                                cout << "Wrong Input , please try again!\n";
                            }
                        }while (choice != "y" && choice !="n");
                        if(choice == "n"){
                            break;
                        }
                        string tempDate;
                        Date* date ;
                        bool CorrectDate;
                        do {
                            try{
                                cout << "Please input a date with DDMMYYYY(There should be 8 character of integer , or otherwise an error will occur. You may type 'today'(case sensitive) such that the customer join as member today)\n";
                                getline(cin , tempDate);
                                if (tempDate!="today"&&tempDate.size()!=8){
                                    cout << "The size was wrongly input , please try again!\n";
                                }else if(tempDate == "today") {
                                    date = &today;
                                    break;
                                }
                                int year, month , day;
                                CorrectDate = isCorrectDate(tempDate , &year, &month , &day);
                                if (!CorrectDate) {
                                    cout << "The date was wrongly input , please try again!\n";
                                }else {
                                    Date tempdate = Date(year , month , day);
                                    date = &tempdate;
                                    break;
                                }
                            }catch (exception e){
                            }
                        }while (true);
                        int PointBalance;
                        do{
                            try{
                                cout << "Input the Initial points:";
                                string tempinput;
                                getline(cin , tempinput);
                                if(tempinput.empty()){
                                    throw invalid_argument("");
                                }
                                PointBalance = stoi(tempinput);
                                if (PointBalance < 0) {
                                    cout << "The Point should not be > 0\n";
                                }
                            }catch (invalid_argument e) {
                                cout << "Wrong Input , Please try again" << endl;
                                PointBalance = -1;
                            }
                        } while (PointBalance < 0);
                        AddCustomer(Customer(customerID , getAutoRank(*date) , PointBalance));
                    }
                    break;
                }
                case 4:
                    CustomerView();
                    break;
                case 5:
                    logger.printLog();
                    break;
                case 6:
                {
                    string confirm;
                    do{
                        cout << "Confirm Exit?(y/n)?";
                        getline(cin ,  confirm);
                        if(confirm=="y"){
                            auto k = [](string studentName , string studentID , string tutorGroup){
                                printf("%11s %9s %4s",studentName.c_str() , studentID.c_str() , tutorGroup.c_str());
                            };
                            k("LUO Jia Wei","23063148A","B03A");
                            k("Siu Lok","23092746A","B03C");
                            k("Shih Richard","23082415A","B03B");
                            k("Ng Ka Ming","23077823A","B03C");
                            k("Liao Jia Ron","23154894A","B03C");
                            k("Kam Yiu Hei","23091497A","B03B");

                        } else if(confirm == "n"){
                            select = -1;
                        } else{
                            cout << "wrong input please try again";
                        }
                    } while (confirm!="y"&&confirm!="n");
                    break;
                }
                default:
                    cout << "Wrong Input , Please try again" << endl;
            }
        }catch (invalid_argument e) {
            cout << "Wrong Input , Please try again" << endl;
        }

    }while(select!=6);
    return 0;


}
