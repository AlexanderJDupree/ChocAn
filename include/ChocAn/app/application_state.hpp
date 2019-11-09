/* 

File: application_state.hpp

Brief:

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

class Exit { };


using Application_State = std::variant< Login
                                      , Exit
                                      , Provider_Menu
                                      , Manager_Menu
                                      >;

#endif // CHOCAN_APPLICATION_STATE