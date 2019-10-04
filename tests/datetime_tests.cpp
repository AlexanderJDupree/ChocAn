/* 

File: datetime_tests.cpp

Brief: Unit tests for DateTime class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <catch.hpp>
#include <ChocAn/datetime.hpp>

TEST_CASE("Constructors for DateTime classes", "[constructors]")
{
    SECTION("Construct valid Day object")
    {
        REQUIRE(Day(4).ok());
    }
    SECTION("Construct valid Month object")
    {
        REQUIRE(Month(7).ok());
    }
    SECTION("Construct valid Year object")
    {
        REQUIRE(Year(1776).ok());
    }
    SECTION("Construct valid DateTime object")
    {
        REQUIRE(DateTime(4, 7, 1776).ok());
    }
    SECTION("Days outside range [1-31] will set ok status to false")
    {
        REQUIRE_FALSE(Day(32).ok());
        REQUIRE_FALSE(Day(0).ok());
    }
    SECTION("Months outside range [1-12] will set ok status to false")
    {
        REQUIRE_FALSE(Month(13).ok());
        REQUIRE_FALSE(Day(0).ok());
    }
    SECTION("Leap years account for 29 days in February")
    {
        REQUIRE(DateTime(29, 2, 2020).ok());
    }
    SECTION("Februrary has 28 days on non leap years")
    {
        REQUIRE(DateTime(28, 2, 2019).ok());
    }
    SECTION("Invalid date will throw invalid_datetime exception")
    {  
        Day   invalid_day(42);
        Month invalid_month(13);
        // TODO Establish year parameters and add to test

        REQUIRE_THROWS_AS(DateTime(22, invalid_month, 1991), invalid_datetime);
        REQUIRE_THROWS_AS(DateTime(invalid_day, 9, 1991), invalid_datetime);
    }
}

TEST_CASE("Detecting Leap Years", "[leap_year]")
{
    SECTION("Non-leap year")
    {
        REQUIRE_FALSE(Year(2019).is_leap_year());
    }
    SECTION("Years divisible by 4 are leap years")
    {
        REQUIRE(Year(2020).is_leap_year());
    }
    SECTION("Years divisible by 100 are not leap years")
    {
        REQUIRE_FALSE(Year(1900).is_leap_year());
    }
    SECTION("Years divisible by 400 are leapyears")
    {
        REQUIRE(Year(2000).is_leap_year());
    }
}

TEST_CASE("DateTime comparison operators")
{
// DateTime utilizes Day, Month, Year comparison operatos, as such we need only
// Test the DateTime class operators
/*
    SECTION("DateTime objects with equal values are equal")
    {
        REQUIRE(DateTime(1, 1, 2020) == DateTime(1, 1, 2020));
    }
    SECTION("DateTime objects with differing year values")
    {
        REQUIRE(DateTime(1, 1, 2019) < DateTime(1, 1, 2020));
    }
    SECTION("DateTime objects with differing month values")
    {
        REQUIRE(DateTime(10, 10, 2020) < DateTime(10, 11, 2020));
    }
    SECTION("DateTime objects with differing day values")
    {
        REQUIRE(DateTime(10, 10, 2020) < DateTime(11, 10, 2020));
    }
    */
}

