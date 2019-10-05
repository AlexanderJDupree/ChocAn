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

struct mock_clock
{
    typedef std::chrono::system_clock clock;
    typedef clock::time_point time_point;

    static time_point time;

    static void duration_since_epoch(clock::duration d)
    {
        time = time_point(d);
    }
    static tm get_utc_time()
    {
        time_t tt = clock::to_time_t(time);

        return *gmtime(&tt);
    }
};

// Initialized out of line because time point is not const
mock_clock::time_point mock_clock::time = mock_clock::time_point();

TEST_CASE("Getting DateTime from system clock", "[clock], [datetime]")
{
    DateTime epoch(Day(1), Month(1), Year(1970));
    DateTime expected_date(Day(5), Month(10), Year(2019));

    SECTION("0 seconds since epoch")
    {
        mock_clock::duration_since_epoch(std::chrono::seconds(0));

        REQUIRE(DateTime::get_current_datetime<mock_clock>() == epoch);
    }
    SECTION("October 5th 2019, day I wrote this test")
    {
        mock_clock::duration_since_epoch(std::chrono::seconds(1570250129));

        REQUIRE(DateTime::get_current_datetime<mock_clock>() == expected_date);
    }
}

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

