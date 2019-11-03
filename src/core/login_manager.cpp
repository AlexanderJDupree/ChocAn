/* 
 
File: login_manager.hpp

Brief: login_manager implementation file

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/login_manager.hpp>

bool Login_Manager::login(const unsigned ID)
{
    return (_session_owner = database->retrieve_account(ID)) ? true : false;
}

void Login_Manager::logout()
{
    _session_owner = nullptr;
}

const Account::Account_Ptr Login_Manager::session_owner() const
{
    return _session_owner;
}
