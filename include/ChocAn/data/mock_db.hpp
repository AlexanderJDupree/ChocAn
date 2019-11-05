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

#include <map>
#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/entities/account.hpp>

class Mock_DB : public Data_Gateway
{
public:

    Mock_DB();

    void update_account(const Account& account);

    void create_account(const Account& account);

    void delete_account(const unsigned ID);

    std::optional<Account> get_account(const unsigned ID) const;

    bool id_exists(const unsigned ID) const;

    std::map<unsigned, Account> account_table;

};

#endif // CHOCAN_MOCK_DB_HPP