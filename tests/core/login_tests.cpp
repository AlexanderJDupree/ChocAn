/* 

File: login_tests.cpp

Brief: Unit tests for login_manager

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <map>
#include <catch.hpp>
#include <ChocAn/core/data_gateway.hpp>
#include <ChocAn/core/login_manager.hpp>

class Mock_DB : public Data_Gateway
{
public:

    void update_account(const Account& account)
    {
        return;
    }

    void create_account(const Account& account)
    {
        return;
    }

    void delete_account(const unsigned ID)
    {
        return;
    }

    Account_Ptr retrieve_account(const unsigned ID)
    {
        try
        {
            Account account(account_table.at(ID));

            return std::make_unique<Account>(std::move(account));
        }
        catch(const std::out_of_range& err)
        {
            return nullptr;
        }
    }

    bool id_exists(const unsigned ID)
    {
        return account_table.find(ID) != account_table.end();
    }

    static std::map<unsigned, Account> account_table;

};

std::map<unsigned, Account> Mock_DB::account_table
{
    { 1234, Account( Name ("John", "Doe")
                   , Address ( "1234 cool st."
                             , "Portland"
                             , "OR"
                             , 97030 )
                   , Account_Type::Manager ) },

    { 5678, Account( Name ("Arman", "Doe")
                   , Address ( "1234 cool st."
                             , "Portland"
                             , "OR"
                             , 97030 )
                   , Account_Type::Provider ) }
};

TEST_CASE("Using the Login_Manager")
{
    Data_Gateway::Database_Ptr db = std::make_unique<Mock_DB>();

    ChocAn chocan(db);

    SECTION("Login returns true when given a valid ID")
    {
        REQUIRE(chocan.login_manager.login(1234));
    }
}