/* 
 
File: sqlite_db.hpp

Brief: Implements Data Gateway interface with sqlite3 API

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_SQLITE_DB_HPP
#define CHOCAN_SQLITE_DB_HPP

#include <sqlite3.h>
#include <functional>
#include <ChocAn/core/data_gateway.hpp>

class SQLite_DB  : public Data_Gateway
{
public:

    using SQL_Row      = std::map<std::string, std::string>;
    using SQL_Callback = int (*) (void*,int,char**,char**);

    SQLite_DB(const char* db_name);

    SQLite_DB(const char* db_name, const char* schema_file);

    ~SQLite_DB();

    bool load_schema(const char* schema_file);

    unsigned create_account(const Account& account) override;

    // Will overwrite previous row data with account info
    bool update_account(const Account& account) override;

    bool delete_account(const unsigned ID) override;

    bool id_exists(const unsigned ID) const override;

    unsigned add_transaction(const Transaction& transaction) override;

    // DB retrieval may fail, wrap in Maybe type
    std::optional<Account> get_account(const unsigned ID) override;
    std::optional<Account> get_account(const std::string& ID) override;

    std::optional<Account> get_member_account(const unsigned ID) override;
    std::optional<Account> get_member_account(const std::string& ID) override;

    std::optional<Account> get_provider_account(const unsigned ID) override;
    std::optional<Account> get_provider_account(const std::string& ID) override;

    std::optional<Account> get_manager_account(const unsigned ID) override;
    std::optional<Account> get_manager_account(const std::string& ID) override;

    std::optional<Service> lookup_service(const unsigned code) override;
    std::optional<Service> lookup_service(const std::string& code) override;

    Transactions get_transactions(DateTime start, DateTime end, Account acct) override;
    Transactions get_transactions(DateTime start, DateTime end) override;

    Accounts get_member_accounts() override;
    Accounts get_provider_accounts() override;
    Accounts get_all_accounts(const std::string& type);

    Service_Directory service_directory() override;

private:


    std::optional<Account> get_account(const unsigned ID, const std::string& type);
    std::optional<Account> get_account(const std::string& ID, const std::string& type);

    std::vector<SQL_Row> get_transaction_data(DateTime start, DateTime end, unsigned id = 0, std::string type = "*");

    bool id_exists(const unsigned ID, std::string& table);

    std::string sqlquote(const std::string& str) const;

    bool execute_statement(const std::string& sql, SQL_Callback, void* data=nullptr);


    sqlite3* db;
    char* err_msg = 0;
    SQL_Callback no_callback = [](void*, int, char**, char**) -> int { return 0; };
};

#endif // CHOCAN_SQLITE_DB_HPP


