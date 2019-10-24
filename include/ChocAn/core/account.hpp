/* 
 
File: account.hpp

Brief: account class packages all the personal information about a member.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_ACCOUNT_HPP
#define CHOCAN_ACCOUNT_HPP

#include <ChocAn/core/address.hpp>
#include <ChocAn/core/exception.hpp>

class invalid_account : public chocan_user_exception
{
public:

    info exception_info;

    explicit invalid_account(info exception_info) 
        : exception_info(std::move(exception_info)) {};

    const char* what() const throw() 
    {
        return "Can't create account, invalid values";
    }

    const info& get_info() const
    {
        return exception_info;
    }

};


class Account{

    public:

        Account( Address& address
               , std::string& f_name
               , std::string& l_name
               , unsigned ID
               );

        bool ok() const;

    private:

        Address address;
        std::string first_name;
        std::string last_name;
        unsigned ID;

};

#endif // CHOCAN_ACCOUNT_HPP

