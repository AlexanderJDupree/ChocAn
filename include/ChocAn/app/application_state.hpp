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
    std::string status;
};

class View_Account   { };
class Delete_Account { };
class Create_Account { };
class Update_Account { };

using Application_State = std::variant< Login
                                      , Exit
                                      , Provider_Menu
                                      , Manager_Menu
                                      , Add_Transaction
                                      >;

#endif // CHOCAN_APPLICATION_STATE