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
    std::optional<Account> maybe_account = database->get_account(ID);
    
    // If retrieval succeded and that account is not a member, then create session
    _session_owner = (maybe_account && !std::holds_alternative<Member>(maybe_account.value().type))
                   ? std::make_unique<Account>(maybe_account.value())
                   : nullptr;

    return (_session_owner) ? true : false;
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

void Login_Manager::logout()
{
    _session_owner = nullptr;
}

const Account::Account_Ptr Login_Manager::session_owner() const
{
    return _session_owner;
}
