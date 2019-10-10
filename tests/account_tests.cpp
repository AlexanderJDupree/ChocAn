/* 

File: account_tests.cpp

Brief: Unit tests for address class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/account.hpp>
#include <ChocAn/address.hpp>

TEST_CASE("Constructors for account class", "[constructors], [account]"){

    SECTION("Construct valid account object"){

        Address address("street", "city", "OR", 97080);

        REQUIRE(account(address,"first name", "last name",123456789).ok());

    }
}    









