/* 

File: address_tests.cpp

Brief: Unit tests for address class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/address.hpp>

TEST_CASE("Constructors for address classes", "[constructors]")
{
    SECTION("Construct valid address object")
    {
        REQUIRE(Address("line one","city","OR",97080);
    }
    SECTION("Ensures usuable information has been entered into each field")
    {
        REQUIRE_FALSE(Address(" ","city","OR",97080).ok());
        REQUIRE_FALSE(Address("line one"," ","OR",97080).ok());
        REQUIRE_FALSE(Address("line one","city"," ",97080).ok());
        REQUIRE_FALSE(Address("line one","city","OR",0).ok());
    }
    SECTION("Checks that exception is thrown for invalid entries")
    {  

        REQUIRE_THROWS_AS(Address("extra long test string test","city","OR",97080),character_limit);
        REQUIRE_THROWS_AS(Address("","","",0),null_address);
    }
}
