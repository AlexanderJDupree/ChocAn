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

    Mock_DB();

    void update_account(const Account& account) override;

    void create_account(const Account& account) override;

    void delete_account(const unsigned ID) override;

    std::optional<Account> get_account(const unsigned ID)     const override;
    std::optional<Account> get_account(const std::string& ID) const override;
    std::optional<Service> lookup_service(const unsigned code)  const override;

    const Service_Directory& service_directory() const override;

    bool id_exists(const unsigned ID) const override;

private:

    std::map<unsigned, Account> _account_table;
    Service_Directory _service_directory;

};

#endif // CHOCAN_MOCK_DB_HPP