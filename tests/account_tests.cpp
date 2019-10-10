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

    std::string first = "first";
    std::string last = "last";
    unsigned acct_num = 123456789;

    SECTION("Construct valid account object"){
    
        std::string street = "street";
        std::string city = "city";
        std::string state = "OR";
        unsigned zip = 97080;

        Address address(street, city, state, zip);

        REQUIRE(Account(address,first,last,acct_num).ok());

    }
}    









