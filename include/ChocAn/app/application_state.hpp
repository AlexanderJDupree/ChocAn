/* 

File: application_state.hpp

Brief: Application State is a discriminated union of several other different,
       but fixed types. Each type represents a specific state within the 
       application.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_APPLICATION_STATE_H
#define CHOCAN_APPLICATION_STATE_H

#include <variant>
#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/transaction.hpp>
#include <ChocAn/core/entities/account_report.hpp>
#include <ChocAn/core/utils/transaction_builder.hpp>
#include <ChocAn/core/utils/account_builder.hpp>

class Menu 
{
public:
    std::string status;
};

class Login
{
public:
    std::string login_status = "Welcome";
};

class Exit { };

class Provider_Menu : public Menu { };

class Manager_Menu : public Menu { };

class Add_Transaction 
{ 
public:
    Transaction_Builder* builder;
};

class Confirm_Transaction 
{
public: 
    Transaction transaction;
};
class Find_Account 
{
public:
    // Represents the next state find account will proceed to
    enum class Next { View_Account
                    , Delete_Account
                    , Update_Account
                    , Gen_Member_Report
                    , Gen_Provider_Report };

    Next next = Next::View_Account;
    std::string status = "";
};

class View_Account
{
public:
    enum class Status { Wait, Confirm_Creation, Confirm_Deletion, Confirm_Update };
    Account account;
    Status status = Status::Wait;
};

class Create_Account
{
public:
    Account_Builder* builder; 
};

class Generate_Report 
{ 
public:

    enum class Report_Type { Member, Provider, Summary };

    Report_Type type = Report_Type::Summary;
    std::optional<Account> account = {};
    std::vector<DateTime> date_range = {};
    const std::string date_structure = "MM-DD-YYYY";
    std::optional<chocan_user_exception> error = {};
};

class View_Report
{
public:
    ChocAn_Report report;
};

class Update_Account
{
public: 
    enum class Status { Choose, Update_Field, Confirm };

    Account account;
    Account_Builder* builder;
    std::string msg = {};
    Status status = Status::Choose;
};

class Delete_Account
{
public:
    Account account;
};

class View_Service_Directory
{
public:
    Data_Gateway::Database_Ptr db;
};

using Application_State = std::variant< Exit
                                      , Login
                                      , View_Report
                                      , Manager_Menu
                                      , Find_Account
                                      , View_Account
                                      , Provider_Menu
                                      , Create_Account
                                      , Update_Account
                                      , Delete_Account
                                      , Generate_Report
                                      , Add_Transaction
                                      , Confirm_Transaction
                                      , View_Service_Directory
                                      >;

using State_Ptr = std::shared_ptr<Application_State>;

#endif // CHOCAN_APPLICATION_STATE
