/* 

File: mock_db.cpp

Brief: Provides a Data_Gateway implementation for use in testing. 
       Mock DB simply creates a tree in memory with a dummy account data.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_MOCK_DB_HPP
#define CHOCAN_MOCK_DB_HPP

#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/account.hpp>
#include <ChocAn/core/entities/service.hpp>

class Mock_DB : public Data_Gateway
{
public:

    using Account_Table   = std::map<unsigned, Account>;
    using Reference_Table = std::map<unsigned, Account&>;

    Mock_DB();

    bool update_account(const Account& account) override;

    bool create_account(const Account& account) override;

    bool delete_account(const unsigned ID) override;

    bool add_transaction(const Transaction& transaction) override;

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

    Service_Directory service_directory() override;

    bool id_exists(const unsigned ID) const override;

    std::optional<Account> account_table_lookup(const unsigned ID, const Account_Table& table) const;
    std::optional<Account> account_table_lookup(const unsigned ID, const Reference_Table& table) const;

    std::map<unsigned, Account> _account_table;
    std::map<unsigned, Account&> _member_table;
    std::map<unsigned, Account&> _provider_table;
    std::map<unsigned, Account&> _manager_table;

    Service_Directory _service_directory;
};

#endif // CHOCAN_MOCK_DB_HPP