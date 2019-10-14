/* 

File: account_tests.cpp

Brief: Unit tests for account class

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
    std::string street = "street";
    std::string city = "city";
    std::string state = "OR";
    std::string string_too_long = "extra long string test length requirments";
    std::string string_empty("");
    unsigned acct_num = 123456789;
    unsigned zip = 97080;


    Address address(street, city, state, zip);
   
    SECTION("Construct valid account object")
    {
    
        REQUIRE(Account(address, first,last, acct_num).ok());
    }

    SECTION("Ensure attempts to construct invalid objects are handled")
    {

        REQUIRE_THROWS_AS( Account(address, string_too_long, last,acct_num)
                         , invalid_account );
    }
}    


