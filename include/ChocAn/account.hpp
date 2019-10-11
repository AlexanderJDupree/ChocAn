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

#include <ChocAn/address.hpp>

struct account_error : std::exception{

    std::vector<std::string> error_log;

    explicit account_error(std::vector<std::string>& errors) : error_log(errors) {};

    const char* what() const throw() {

        return "Error with constructing account object";
    }

    std::vector<std::string> get_info() const throw(){

        return error_log;
    }

};


class Account{

    public:

        Account(Address& address,std::string& f_name,std::string& l_name,unsigned acct_num);

        bool ok() const;

    private:

        Address address;
        std::string first_name;
        std::string last_name;
        unsigned account_number;

};


#endif


