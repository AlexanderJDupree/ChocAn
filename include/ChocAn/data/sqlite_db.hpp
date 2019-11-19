/* 
 
File: sqlite_db.hpp

Brief: Implements Data Gateway interface with sqlite3 API

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_SQLITE_DB_HPP
#define CHOCAN_SQLITE_DB_HPP

#include <sqlite3.h>
#include <ChocAn/core/data_gateway.hpp>

class SQLite_DB // : public Data_Gateway
{
public:

    SQLite_DB(const char* db_name);

    ~SQLite_DB();

    std::string serialize_account(const Account& account) const;

    void create_account(const Account& account);
/*
    void update_account(const Account& account);


    void delete_account(const unsigned ID);

    void add_transaction(const Transaction& transaction);

    // DB retrieval may fail, wrap in Maybe type
    std::optional<Account> get_account(const unsigned ID);
    std::optional<Account> get_account(const std::string& ID);

    std::optional<Account> get_member_account(const unsigned ID)      const;
    std::optional<Account> get_member_account(const std::string& ID)  const;

    std::optional<Account> get_provider_account(const unsigned ID)      const;
    std::optional<Account> get_provider_account(const std::string& ID)  const;

    std::optional<Account> get_manager_account(const unsigned ID)      const;
    std::optional<Account> get_manager_account(const std::string& ID)  const;

    std::optional<Service> lookup_service(const unsigned code)     const;
    std::optional<Service> lookup_service(const std::string& code) const;

    const Service_Directory& service_directory() const;

    bool id_exists(const unsigned ID) const;
*/

private:

    sqlite3* db;

};

#endif // CHOCAN_SQLITE_DB_HPP


