/* 

File: member_tests.cpp

Brief: Unit tests for address class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/member.hpp>
#include <ChocAn/address.hpp>

TEST_CASE("Constructors for member class", "[constructors], [member]"){

    SECTION("Construct valid member object"){

        Address address("street", "city", "OR", 97080);

        REQUIRE(Member(address,"first name", "last name",123456789).ok());

    }
}    









