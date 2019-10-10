/* 
 
File: account.hpp

Brief: account class standardizes the account format throughout the ChocAn data services
       and provides methods for validation and reporting

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/account.hpp>

Account::Account(Address& address, std::string& f_name, std::string& l_name, unsigned num)
    : address(address), first_name(f_name), last_name(l_name), account_number(num){

        if(!ok()){

            std::stack<std::string> fields;

            fields.push(f_name);
            fields.push(l_name);
            fields.push(std::to_string(num));

            throw account_error(fields);
        }

    }

bool Account::ok() const{

    int name_length = first_name.length() + last_name.length();

    if(name_length > 25 || name_length < 2) return false;

    if(!account_number) return false;

    return true;
}



