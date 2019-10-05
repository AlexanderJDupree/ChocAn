/* 

File: datetime_tests.cpp

Brief: Unit tests for DateTime class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <iostream>
#include <catch.hpp>
#include <ChocAn/datetime.hpp>

std::ostream& operator<<(std::ostream& os, const DateTime& date)
{
    os << static_cast<unsigned>(date.day()) << "-" 
       << static_cast<unsigned>(date.month()) << "-" 
       << static_cast<unsigned>(date.year());
    return os;
}

TEST_CASE("Constructors for DateTime classes", "[constructors], [datetime]")
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
        REQUIRE(DateTime(Day(4), Month(7), Year(1776)).ok());
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
        REQUIRE(DateTime(Day(29), Month(2), Year(2020)).ok());
    }
    SECTION("Februrary has 28 days on non leap years")
    {
        REQUIRE(DateTime(Day(28), Month(2), Year(2019)).ok());
    }
    SECTION("Invalid date will throw invalid_datetime exception")
    {  
        Day   invalid_day(42);
        Month invalid_month(13);
        // TODO Establish year parameters and add to test

        REQUIRE_THROWS_AS(DateTime(Day(22), invalid_month, Year(1991))
                         , invalid_datetime);

        REQUIRE_THROWS_AS(DateTime(invalid_day, Month(9), Year(1991))
                         , invalid_datetime);
    }
}

TEST_CASE("Detecting Leap Years", "[leap_year], [datetime]")
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

/*
TODO Figure out how to mock system clock to run tests against
TEST_CASE("Getting current DateTime from system clock", "[current_time], [datetime]")
{
    DateTime epoch(Day(1), Month(1), Year(1970));

    SECTION("0 seconds since epoch")
    {
        test.set_utc_time.tm_mday = 1;
        test.set_utc_time.tm_mon  = 0;
        test.set_utc_time.tm_year = 70;

        REQUIRE(DateTime::get_current_datetime() == epoch);
    }
}
*/

TEST_CASE("DateTime comparison operators", "[operators], [datetime]")
{
// DateTime utilizes Day, Month, Year comparison operatos, as such we need only
// Test the DateTime class operators

    SECTION("DateTime objects with equal values are equal")
    {
        REQUIRE(DateTime(Day(1), Month(1), Year(2020)) 
             == DateTime(Day(1), Month(1), Year(2020)));
    }
    SECTION("DateTime objects with differing year values")
    {
        REQUIRE(DateTime(Day(1), Month(1), Year(2019)) 
              < DateTime(Day(1), Month(1), Year(2020)));
    }
    SECTION("DateTime objects with differing month values")
    {
        REQUIRE(DateTime(Day(10), Month(10), Year(2020)) 
              < DateTime(Day(10), Month(11), Year(2020)));
    }
    SECTION("DateTime objects with differing day values")
    {
        REQUIRE(DateTime(Day(10), Month(10), Year(2020)) 
              < DateTime(Day(11), Month(10), Year(2020)));
    }
}

