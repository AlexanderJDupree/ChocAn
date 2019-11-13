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
    using Account_Ptr  = Account::Account_Ptr;

    Login_Manager(Database_Ptr db) 
        :  database(db), _session_owner(nullptr) 
        { if(!db) { throw std::exception(); } }

    bool login(const unsigned ID);
    bool login(const std::string& ID);

    // logout is idempotent
    void logout(); 

    const Account_Ptr session_owner() const;

private:

    Database_Ptr database;
    Account_Ptr _session_owner;

};

#endif  // CHOCAN_LOGIN_MANAGER_HPP