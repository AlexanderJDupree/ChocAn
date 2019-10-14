/* 
 
File: account.hpp

Brief: account class standardizes the account format throughout the ChocAn data services
       and provides methods for validation and reporting

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/account.hpp>

Account::Account( Address& address, std::string& f_name, 
                 std::string& l_name, unsigned ID )
    : address(address), first_name(f_name), last_name(l_name), ID(ID)
{
        if(!ok())
        {
            throw invalid_account( { "Name must be less than 25 characters" });
        }
}

bool Account::ok() const{

    int name_length = first_name.length() + last_name.length();

    if(name_length > 25 || name_length < 2) return false;

    // TODO For now we don't know how we want to validate the account ID
    //if(!account_number) return false;

    return true;
}

