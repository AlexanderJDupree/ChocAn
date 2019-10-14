/* 

File: address_tests.cpp

Brief: Unit tests for address class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/address.hpp>

TEST_CASE("Constructors for address classes", "[constructors], [address]"){

    std::string line_one = "line_one";
    std::string city = "city";
    std::string state = "OR";
    unsigned zip = 97080;

    SECTION("Construct valid address object")
    {
        REQUIRE(Address(line_one,city,state,zip).ok());
    }

    SECTION("Ensures usuable information has been entered into each field")
    {

        std::string temp = " ";
        REQUIRE_THROWS(Address(temp,city,state,zip));
        REQUIRE_THROWS(Address(line_one,temp,state,zip));
        REQUIRE_THROWS(Address(line_one,city,temp,zip));
        REQUIRE_THROWS(Address(line_one,city,state,0));
    }
    SECTION("Checks that exception is thrown for invalid entries")
    {  

        std::string temp = "extra long test string test";
        std::string nully("");

        REQUIRE_THROWS_AS(Address(temp,city,state,0), invalid_address);


        REQUIRE_THROWS_AS(Address(nully,nully,nully,0), invalid_address);
    }
}

