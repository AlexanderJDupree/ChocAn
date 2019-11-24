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

Login_Manager::Login_Manager(Database_Ptr db)
    : database(db), _session_owner( { })
{
    if(!db) 
    { 
        throw std::logic_error("Login Manager: DB is null, construction failed"); 
    } 
}

bool Login_Manager::login(const unsigned ID)
{
    return (_session_owner = database->get_account(ID)) ? true : false;
}

bool Login_Manager::login(const std::string& ID)
{
    try
    {
        return login(std::stoi(ID));
    }
    catch(const std::exception& err)
    {
        return false;
    }
}

bool Login_Manager::logged_in() const
{
    return (_session_owner) ? true : false;
}

void Login_Manager::logout()
{
    _session_owner.reset();
}

const Account& Login_Manager::session_owner() const
{
    if(!logged_in())
    {
        throw std::logic_error("Login Manager: Session owner does not exist.");
    }
    return _session_owner.value();
}
