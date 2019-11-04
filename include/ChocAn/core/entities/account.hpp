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

#include <memory>
#include <ChocAn/core/utils/name.hpp>
#include <ChocAn/core/utils/address.hpp>

enum class Account_Type { Manager, Provider, Member };

class Account
{
public:

    typedef std::shared_ptr<Account> Account_Ptr;

    Account(Name name, Address address, Account_Type type);

    // Getters and setters?
    Name         name;
    Address      address;

    const unsigned     id = 1111;
    const Account_Type type;

    // Two accounts are equal iff they have the same id
    bool operator == (const Account& rhs) const { return id == rhs.id;   }
    bool operator != (const Account& rhs) const { return (*this) == rhs; }

};

#endif // CHOCAN_ACCOUNT_HPP

