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
#include <ChocAn/core/id_generator.hpp> 
#include <ChocAn/core/utils/passkey.hpp>
#include <ChocAn/core/entities/name.hpp>
#include <ChocAn/core/entities/address.hpp>

enum class Account_Type { Manager, Provider, Member };

class Account
{
public:

    typedef std::shared_ptr<Account> Account_Ptr;

    // Client side account creation, utilizes the ChocAn Id generator
    Account(Name name, Address address, Account_Type type, const ID_Generator& id_gen);

    // Database side Account de-serialization
    Account(Name name, Address address, Account_Type type, unsigned id, Key<Data_Gateway>&);

    Name         name;
    Address      address;

    const Account_Type type;
    const unsigned     id;

    // Two accounts are equal iff they have the same id
    bool operator == (const Account& rhs) const { return id == rhs.id;   }
    bool operator != (const Account& rhs) const { return (*this) == rhs; }

};

#endif // CHOCAN_ACCOUNT_HPP

