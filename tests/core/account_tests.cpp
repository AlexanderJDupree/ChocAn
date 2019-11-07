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
