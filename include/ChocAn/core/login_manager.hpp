/* 
 
File: login_manager.hpp

Brief: login_manager handles authorization and authentication for ChocAn services

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_LOGIN_MANAGER_HPP
#define CHOCAN_LOGIN_MANAGER_HPP

#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/account.hpp>

class Login_Manager
{
public:

    using Database_Ptr = Data_Gateway::Database_Ptr;

    Login_Manager(Database_Ptr db);

    bool login(const unsigned ID);
    bool login(const std::string& ID);
    bool logged_in() const;

    // logout is idempotent
    void logout(); 

    const Account& session_owner() const;

private:

    Database_Ptr database;
    std::optional<Account> _session_owner;

};

#endif  // CHOCAN_LOGIN_MANAGER_HPP