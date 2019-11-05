/* 
 
File: data_gateway.hpp

Brief: Defines interface for database connectors to implement

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_DATA_GATEWAY_HPP
#define CHOCAN_DATA_GATEWAY_HPP

#include <memory>
#include <optional>

// Maybe just forward declare account?
#include <ChocAn/core/entities/account.hpp>

class Data_Gateway
{
public:

    using Account_Ptr = Account::Account_Ptr;

    typedef std::shared_ptr<Data_Gateway> Database_Ptr;

    virtual ~Data_Gateway() {}

    // Updates fields of specific accounts, creates account if account DNE
    virtual void update_account(const Account& account) = 0;

    virtual void create_account(const Account& account) = 0;

    virtual void delete_account(const unsigned ID) = 0;

    // virtual void add_transactioni(const Transaction& transaction) = 0;

    // Retrieval from a DB may fail, must check for NULL
    virtual Account_Ptr retrieve_account(const unsigned ID) const = 0;

    virtual bool id_exists(const unsigned ID) const = 0;

};

#endif // CHOCAN_DATA_GATEWAY_HPP