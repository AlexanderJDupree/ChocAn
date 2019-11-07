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
#include <variant>
#include <ChocAn/core/id_generator.hpp> 
#include <ChocAn/core/utils/passkey.hpp>
#include <ChocAn/core/entities/name.hpp>
#include <ChocAn/core/entities/address.hpp>

enum class Account_Status { Valid, Suspended };

class Provider {};

class Manager  
{
public:
    Key<Manager> manager_key;
};

class Member 
{
public:

    // By Default, Member accounts are valid
    Member(Account_Status status = Account_Status::Valid)
        : status(status) {}

    void set_account_status(Account_Status status, Key<Manager>&)
    {
        this->status = status;
    }

private: 

    Account_Status status;
};

class Account
{
public:

    typedef std::shared_ptr<Account> Account_Ptr;
    typedef std::variant<Manager, Member, Provider> Account_Type;

    // Client side account creation, utilizes the ChocAn Id generator
    Account(Name name, Address address, Account_Type type, const ID_Generator& id_gen);

    // Database side Account de-serialization
    Account(Name name, Address address, Account_Type type, unsigned id, Key<Data_Gateway>&);

    // Name and Address fields are mutable
    Name         name;
    Address      address;

    const Account_Type type;
    const unsigned     id;

    // Two accounts are equal iff they have the same id
    bool operator == (const Account& rhs) const { return id == rhs.id;   }
    bool operator != (const Account& rhs) const { return (*this) == rhs; }

};

#endif // CHOCAN_ACCOUNT_HPP

