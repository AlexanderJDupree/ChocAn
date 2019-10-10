/* 
 
File: account.hpp

Brief: account class packages all the personal information about a member.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/
 
*/

#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

struct account_error : std::exception{

    std::stack<std::string> error_log;

    account_error(std::stack<std::string>&) : error_log(errors) {};

    const char* what() const throw() {

        return "Error with constructing account object";
    }

    std::stack<std::string> get_info() const throw(){

        return error_log;
    }

};


class Account{

};


#endif


