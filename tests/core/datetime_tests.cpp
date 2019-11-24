/* 

File: datetime_tests.cpp

Brief: Unit tests for DateTime class and associated utility classes.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <iostream>
#include <sstream>
#include <catch.hpp>
#include <ChocAn/core/entities/datetime.hpp>

std::ostream& operator<<(std::ostream& os, const DateTime& date)
{
    os << static_cast<unsigned>(date.day().count()) << "-" 
       << static_cast<unsigned>(date.month().count()) << "-" 
       << static_cast<unsigned>(date.year().count());
    return os;
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
    static time_point now()
    {
        return time;
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

TEST_CASE("Converting DateTime to unix time stamps", "[unix_timestamp], [datetime]")
{
    DateTime date ( Day(24), Month(11), Year(2019));
    double nov_24_2019 = 1574553600;

    SECTION("unix_timestamp returns the number of seconds since the epoch")
    {
        REQUIRE(date.unix_timestamp() == nov_24_2019);
    }
    SECTION("A unix timestamp can be converted into a DateTime object")
    {
        REQUIRE(DateTime(nov_24_2019) == date);
    }
}

TEST_CASE("Constructors for DateTime classes", "[constructors], [datetime]")
{
    SECTION("Constructing Datetime in Day Month Year format")
    {
        REQUIRE_NOTHROW(DateTime(Day(1), Month(1), Year(1970)));
    }
    SECTION("Constructing Datetime in Day Year Month format")
    {
        REQUIRE_NOTHROW(DateTime(Day(1), Year(1970), Month(1)));
    }
    SECTION("Constructing Datetime in Month Day Year format")
    {
        REQUIRE_NOTHROW(DateTime(Month(1), Day(1), Year(1970)));
    }
    SECTION("Constructing Datetime in Month Year Day format")
    {
        REQUIRE_NOTHROW(DateTime(Month(1), Year(1970), Day(1)));
    }
    SECTION("Constructing Datetime in Year Month Day format")
    {
        REQUIRE_NOTHROW(DateTime(Year(1970), Month(1), Day(1)));
    }
    SECTION("Constructing Datetime in Year Day Month format")
    {
        REQUIRE_NOTHROW(DateTime(Year(1970), Day(1), Month(1)));
    }
    SECTION("Datetime can be constructed with february 29th on leap years")
    {
        REQUIRE_NOTHROW(DateTime(Day(29), Month(2), Year(2020)));
    }
    SECTION("Datetime cannot be constructed with february 29th on non-leap years")
    {
        REQUIRE_THROWS_AS(DateTime(Day(29), Month(2), Year(2019)), invalid_datetime);
    }
    SECTION("Days must be be greater than 1 and less than the limit for that month")
    {  
        // No zeroth day
        REQUIRE_THROWS_AS(DateTime(Day(0), Month(11), Year(1991))
                         , invalid_datetime);

        // No september 31st
        REQUIRE_THROWS_AS(DateTime(Day(31), Month(9), Year(1991))
                         , invalid_datetime);
    }
    SECTION("Dates must be greater than 1/1/1970")
    {
        REQUIRE_THROWS_AS(DateTime(Day(31), Month(12), Year(1969)), invalid_datetime);
    }
}

TEST_CASE("Detecting Leap Years", "[leap_year], [datetime]")
{
    SECTION("Non-leap year")
    {
        REQUIRE_FALSE(DateTime::is_leap_year(Year(2019)));
    }
    SECTION("Years divisible by 4 are leap years")
    {
        REQUIRE(DateTime::is_leap_year(Year(2020)));
    }
    SECTION("Years divisible by 100 are not leap years")
    {
        REQUIRE_FALSE(DateTime::is_leap_year(Year(1900)));
    }
    SECTION("Years divisible by 400 are leapyears")
    {
        REQUIRE(DateTime::is_leap_year(Year(2000)));
    }
}

TEST_CASE("DateTime comparison operators", "[operators], [datetime]")
{

    SECTION("DateTime objects with equal values are equal")
    {
        REQUIRE(DateTime(Day(1), Month(1), Year(2020)) 
             == DateTime(Day(1), Month(1), Year(2020)));
    }
    SECTION("DateTime objects with differing year values")
    {
        REQUIRE(DateTime(Day(1), Month(1), Year(2020)) 
              >= DateTime(Day(1), Month(1), Year(2019)));
    }
    SECTION("DateTime objects with differing month values")
    {
        REQUIRE(DateTime(Day(10), Month(10), Year(2020)) 
              < DateTime(Day(10), Month(11), Year(2020)));
    }
    SECTION("DateTime objects with differing day values")
    {
        REQUIRE( DateTime(Day(10), Month(10), Year(2020)) 
              <= DateTime(Day(11), Month(10), Year(2020)));
    }
    SECTION("Greater than operator")
    {
        REQUIRE( DateTime(Month(10), Day(10), Year(4020)) 
               > DateTime(Month(10), Day(10), Year(2010)));
    }
}

TEST_CASE("Testing ostream operator", "[datetime]")
{
    std::stringstream oss;

    oss << DateTime(Day(10), Month(10), Year(2020));

    REQUIRE(oss.str() == "10-10-2020");
}

TEST_CASE("Testing invalid_datetime exception", "[exception], [datetime]")
{
    SECTION("invalid_datetime error is populated with error messages")
    {
        chocan_user_exception::Info info;
        try
        {
            DateTime(Day(40), Month(12), Year(1200));
        }
        catch (const chocan_user_exception& err)
        {
            info = err.info();
            REQUIRE(std::string(err.what()) == "Invalid datetime values");

        }
        REQUIRE(info.size() == 3);
    }
}
