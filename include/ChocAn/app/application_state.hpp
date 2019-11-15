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
#include <ChocAn/core/utils/transaction_builder.hpp>

class Login
{
public:
    std::string login_status = "Welcome";
};

class Exit { };

class Provider_Menu
{
public:
    std::string status;
};

class Manager_Menu
{
public:
    std::string status;
};

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

using Application_State = std::variant< Login
                                      , Exit
                                      , Provider_Menu
                                      , Manager_Menu
                                      , Add_Transaction
                                      , Confirm_Transaction
                                      >;

using State_Ptr = std::shared_ptr<Application_State>;

#endif // CHOCAN_APPLICATION_STATE