/* 

File: mock_db.cpp

Brief: Provides a Data_Gateway implementation for use in testing

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

class Mock_DB : public Data_Gateway
{
public:

    void update_account(const Account& account);

    void create_account(const Account& account);

    void delete_account(const unsigned ID);

    Account_Ptr retrieve_account(const unsigned ID) const;

    bool id_exists(const unsigned ID) const;

    static std::map<unsigned, Account> account_table;

};

#endif // CHOCAN_MOCK_DB_HPP