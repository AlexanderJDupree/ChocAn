/* 
 
File: login_manager.hpp

Brief: login_manager handles authorization and authentication for ChocAn

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

#include <optional>
#include <ChocAn/core/chocan_service.hpp>
#include <ChocAn/core/entities/account.hpp>

class Login_Manager : public ChocAn_Service
{
public:

    using Account_Ptr = Account::Account_Ptr;

    Login_Manager() : ChocAn_Service(), _session_owner(nullptr) {}

    bool login(const unsigned ID);

    void logout(); 

    const Account_Ptr session_owner() const;

private:

    Account_Ptr _session_owner;

};

#endif  // CHOCAN_LOGIN_MANAGER_HPP