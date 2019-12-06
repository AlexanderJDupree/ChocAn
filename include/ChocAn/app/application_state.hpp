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
    enum class Next { View_Account, Delete_Account, Update_Account };
    std::string status;
    Next next = Next::View_Account;
};

class View_Account
{
public:
    enum class Status { Wait, Confirm_Creation, Confirm_Deletion };
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
    std::vector<DateTime> date_range;
    const std::string date_structure = "MM-DD-YYYY";
    std::optional<chocan_user_exception> error;
};

class View_Summary_Report
{
public:
    Summary_Report report;
};

class Update_Account
{
public: 
    enum class Status {Choose, Update_Field, Confirm};

    Account account;
    std::string msg = {};
    Account_Builder builder = {};
    Status status = Status::Choose;
};

using Application_State = std::variant< Login
                                      , Exit
                                      , Provider_Menu
                                      , Manager_Menu
                                      , Add_Transaction
                                      , Confirm_Transaction
                                      , Create_Account
                                      , Find_Account
                                      , View_Account
                                      , Generate_Report
                                      , View_Summary_Report
                                      , Update_Account
                                      >;

using State_Ptr = std::shared_ptr<Application_State>;

#endif // CHOCAN_APPLICATION_STATE
