/* 

File: parsers_test.cpp

Brief: Unit tests for parsers utility

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/core/utils/parsers.hpp>

TEST_CASE("Splitting a string on a delimiter", "[split], [parsers]")
{
    using namespace Parsers;

    SECTION("Tokenizing the empty string returns the empty vector")
    {
        REQUIRE(split("", ",").empty());
    }
    SECTION("Tokenizing a string with no matching delimiters returns the entire string")
    {
        REQUIRE(split("Hello World", "bad_delim").front() == "Hello World");
    }
    SECTION("Tokenizing a string with a matching delmiters returns a tokenized string")
    {
        std::vector<std::string> expected { "A", "B", "C" };

        REQUIRE(split(join(expected, "-"), "-") == expected);
    }
}

TEST_CASE("Joining a string from tokens", "[join], [parsers]")
{
    using namespace Parsers;

    SECTION("Joining a string from multiple tokens")
    {
        std::vector<std::string> tokens { "A", "B", "C" };

        REQUIRE(join(tokens, "-") == "A-B-C");
    }
    SECTION("Joining an empty set of tokens returns the empty string")
    {
        REQUIRE(join({}, "-") == "");
    }
}

TEST_CASE("Parsing datetime into DateTime objects", "[parse_date], [parsers]")
{
    using namespace Parsers;

    SECTION("Parsing a valid string in the form MM/DD/YY")
    {
        DateTime expected(Month(12), Day(10), Year(2019));

        REQUIRE(parse_date("12/10/2019", "MM/DD/YYYY", "/") == expected);
    }
    SECTION("Parsing a string with mismatched tokens results in an exception")
    {
        REQUIRE_THROWS_AS(parse_date("12/10", "MM-DD-YYYY", "/"), invalid_datetime);
    }
}