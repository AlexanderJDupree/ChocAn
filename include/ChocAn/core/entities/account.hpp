/* 
 
File: account.hpp

Brief: account class packages all the personal information about a member.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_ACCOUNT_HPP
#define CHOCAN_ACCOUNT_HPP

#include <ChocAn/core/utils/name.hpp>
#include <ChocAn/core/utils/address.hpp>
#include <ChocAn/core/utils/exception.hpp>

enum class Account_Type { Manager, Provider, Member };

class Account
{
public:

    Account(Name name, Address address, Account_Type type);

    Name         name;
    Address      address;

    const Account_Type type;

};

#endif // CHOCAN_ACCOUNT_HPP

