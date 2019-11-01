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
#include <ChocAn/core/entities/account.hpp>

TEST_CASE("Constructors for account class", "[constructors], [account]")
{
    SECTION("Construct valid account object")
    {
        REQUIRE_NOTHROW( Account( Name("John", "Doe")
                                , Address( "1234 Cool St." 
                                         , "Portland"
                                         , "OR"
                                         , 97030)
                                , Account_Type::Member ) );
    }
}    
