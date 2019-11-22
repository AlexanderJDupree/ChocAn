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
#include <ChocAn/core/utils/serializable.hpp>

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
        : _status(status) {}

    void set_account_status(Account_Status status, Key<Manager>&)
    {
        _status = status;
    }

    const Account_Status& status() const
    {
        return _status;
    }

private: 
    Account_Status _status;
};

class Account : public Serializable<Account, std::string, std::string, 9>
{
public:

    typedef std::shared_ptr<Account> Account_Ptr;
    typedef std::variant<Manager, Member, Provider> Account_Type;

    // Client side account creation, utilizes the ChocAn Id generator
    Account( Name name
           , Address address
           , Account_Type type
           , const ID_Generator& id_gen );

    // Database side Account de-serialization
    Account( Name name
           , Address address
           , Account_Type type
           , unsigned id
           , const Key<Data_Gateway>& );

    /* Serializable Interface */
    Data_Table serialize() const override;
    Account deserialize(const Data_Table&, const Key<Data_Gateway>&) const;

    /* Name and Address are Mutable */
    Name& name() { return _name; }
    const Name& name() const { return _name; }

    Address& address() { return _address; }
    const Address& address() const { return _address; }

    /* Type and ID are immutable */
    const Account_Type& type() const { return _type; }
    unsigned id() const { return _id; }

    // Two accounts are equal iff they have the same id
    bool operator == (const Account& rhs) const { return _id == rhs._id;   }
    bool operator != (const Account& rhs) const { return (*this) == rhs; }

private:

    Account( const Name& name
           , const Address& address
           , const Account_Type& type
           , unsigned id);

    Account deserialize(const Data_Table&) const override;

    Name         _name;
    Address      _address;
    Account_Type _type;
    unsigned     _id;
};

#endif // CHOCAN_ACCOUNT_HPP

