/* 

File: account_tests.cpp

Brief: Unit tests for account class

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/core/chocan.hpp>
#include <ChocAn/data/mock_db.hpp>
#include <ChocAn/core/entities/account.hpp>

TEST_CASE("Constructors for account class", "[constructors], [account]")
{
    Data_Gateway::Database_Ptr mock_db = std::make_unique<Mock_DB>();

    ChocAn chocan(mock_db);

    SECTION("Account can only be constructed with the ID_Generator service")
    {
        REQUIRE_NOTHROW( Account( Name("John", "Doe")
                                , Address( "1234 Cool St." 
                                         , "Portland"
                                         , "OR"
                                         , 97030)
                                , Member()
                                , chocan.id_generator ) );
    }
}    

TEST_CASE("Serializing Accounts into data tables", "[serialize], [account]")
{

    Mock_DB mock_db;

    Account account = mock_db.get_account(5678).value();

    Account::Data_Table expected
    {
        { "f_name", "Dan" }, { "l_name", "Manager"}, 
        { "street", "1234 cool st." }, { "city", "Portland" }, 
        { "state", "OR" }, {"zip", "97030" }, { "chocan_id", "5678" },
        { "type", "Manager" }, { "status", "Valid" }
    };

    SECTION("Account serializaition returns a string table of the accounts data")
    {
        REQUIRE( account.serialize() == expected);
    }
    SECTION("Deserialization returns an Account object that matches the data table")
    {
        REQUIRE( Account(expected, mock_db.get_db_key()).serialize() == expected);
    }
    SECTION("Incomplete data tables will cause the constructor to throw")
    {
        REQUIRE_THROWS(Account(Account::build_key_table(), mock_db.get_db_key()));
    }
}
