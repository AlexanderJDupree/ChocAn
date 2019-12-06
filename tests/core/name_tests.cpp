/* 

File: name_tests.cpp

Brief: Unit tests for name utility

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/core/entities/name.hpp>

TEST_CASE("Constructing Name objects", "[constructors], [name]")
{
    const std::string valid_first = "Alex";
    const std::string valid_last  = "DuPree";

    SECTION("Construct valid name object")
    {
        REQUIRE_NOTHROW( Name( valid_first, valid_last) );
    }
    SECTION("Missing first name")
    {
        REQUIRE_THROWS_AS( Name( "", valid_last), invalid_name);
    }
    SECTION("Missing last name")
    {
        REQUIRE_THROWS_AS( Name( valid_first, ""), invalid_name);
    }
    SECTION("Name is too long")
    {
        REQUIRE_THROWS_AS( Name( "Really really ", "really long name"), invalid_name);
    }
    SECTION("First name is not alphabetic")
    {
        REQUIRE_THROWS_AS( Name( "not 4lphabetic", "valid"), invalid_name);
    }
    SECTION("Last name is not alphabetic")
    {
        REQUIRE_THROWS_AS( Name("valid", "not 4lphabetic"), invalid_name);
    }
}

TEST_CASE("Name equality operators", "[operators], [name]")
{
    SECTION("Names are equal when first and last name are equal")
    {
        REQUIRE(Name("John", "Doe") == Name("John", "Doe"));
    }
    SECTION("Names are not equal when either first and last name are different")
    {
        REQUIRE(Name("John", "Yoyo") != Name("John", "Doe"));
    }
}