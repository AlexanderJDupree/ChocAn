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

#include <map>
#include <memory>
#include <optional>
#include <ChocAn/core/utils/passkey.hpp>

// Forward Declare
class Account;
class Service;
class Transaction;

class Data_Gateway
{
public:

    using Database_Ptr      = std::shared_ptr<Data_Gateway>;
    using Service_Directory = std::map<unsigned, Service>;

    virtual ~Data_Gateway() {}

    // Updates fields of specific accounts, creates account if account DNE
    virtual void update_account(const Account& account) = 0;

    virtual void create_account(const Account& account) = 0;

    virtual void delete_account(const unsigned ID) = 0;

    virtual void add_transaction(const Transaction& transaction) = 0;

    // DB retrieval may fail, wrap in Maybe type
    virtual std::optional<Account> get_account(const unsigned ID)      const = 0;
    virtual std::optional<Account> get_account(const std::string& ID)  const = 0;

    virtual std::optional<Account> get_member_account(const unsigned ID)      const = 0;
    virtual std::optional<Account> get_member_account(const std::string& ID)  const = 0;

    virtual std::optional<Account> get_provider_account(const unsigned ID)      const = 0;
    virtual std::optional<Account> get_provider_account(const std::string& ID)  const = 0;

    virtual std::optional<Account> get_manager_account(const unsigned ID)      const = 0;
    virtual std::optional<Account> get_manager_account(const std::string& ID)  const = 0;

    virtual std::optional<Service> lookup_service(const unsigned code)     const = 0;
    virtual std::optional<Service> lookup_service(const std::string& code) const = 0;

    virtual const Service_Directory& service_directory() const = 0;

    virtual bool id_exists(const unsigned ID) const = 0;

protected:

    // Used for de-serializing domain entities
    Key<Data_Gateway> db_key;

};

#endif // CHOCAN_DATA_GATEWAY_HPP