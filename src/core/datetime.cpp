/* 
 
File: datetime.cpp

Brief: Implementation file for DateTime and associated utility classes

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <tuple>
#include <ChocAn/core/utils/validators.hpp>
#include <ChocAn/core/entities/datetime.hpp>

/** DATETIME CLASS **/

DateTime::DateTime(Day day, Month month, Year year)
    : _day(day), _month(month), _year(year)
{
    chocan_user_exception::Info errors;

    (Validators::range(day.count(), 1, 31))
        ? void() : errors.push_back("Day should be between 1 - 31");

    (Validators::range(year.count(), 1970, Year::max().count()))
        ? void() : errors.push_back("Year should be greater than 1970");

    (Validators::range(month.count(), 1, 12) && ok()) 
        ? void() : errors.push_back( "Month: " 
                                   + std::to_string(month.count()) 
                                   + ", and Day: "
                                   + std::to_string(day.count())
                                   + " does not exist." );
    (errors.empty())
        ? void() :throw invalid_datetime("Invalid datetime values", errors);
}

/* Out of Order constructors */
DateTime::DateTime(Day day, Year year, Month month)
    : DateTime(day, month, year) {}

DateTime::DateTime(Month month, Day day, Year year)
    : DateTime(day, month, year) {}

DateTime::DateTime(Month month, Year year, Day day)
    : DateTime(day, month, year) {}

DateTime::DateTime(Year year, Month month, Day day)
    : DateTime(day, month, year) {}

DateTime::DateTime(Year year, Day day, Month month)
    : DateTime(day, month, year) {}

DateTime::DateTime(const Data_Table& data)
{
    try
    {
        // Try to build from day/month/year values
        *this = DateTime( Day(std::stoi(data.at("day")))
                        , Month(std::stoi(data.at("month")))
                        , Year(std::stoi(data.at("year"))) );
    }
    catch(const std::exception&)
    {
        // Try to build from unix timestamp, if this fails the exception goes uncaught
        DateTime(std::stoi(data.at("unix")));
    }
}

DateTime::DateTime(double unix_timestamp)
{
    std::time_t time = unix_timestamp;

    tm utc_time = *gmtime(&time);

    *this =  DateTime( Day   { utc_time.tm_mday }
                     , Month { utc_time.tm_mon + 1 }
                     , Year  { utc_time.tm_year + 1900 } );
}

bool DateTime::ok() const
{
    const Day calendar[] = 
    {
        Day(31), Day((is_leap_year(_year) ? 29 : 28)), Day(31),
        Day(30), Day(31), Day(30), 
        Day(31), Day(31), Day(30), 
        Day(31), Day(31), Day(30)
    };

    return _day <= calendar[_month.count() - 1];
}

bool DateTime::is_leap_year(const Year& year)
{
    return year.count() % 4 == 0 && (year.count() % 100 != 0 || year.count() % 400 == 0);
}

DateTime::Data_Table DateTime::serialize() const
{
    return
    {
        { "day"  , std::to_string(_day.count())     },
        { "month", std::to_string(_month.count())   },
        { "year" , std::to_string(_year.count())    },
        { "unix" , std::to_string(unix_timestamp()) }
    };
}

double DateTime::unix_timestamp() const
{
    std::tm time;

    time.tm_year = _year.count() - 1900;
    time.tm_mon  = _month.count() - 1;
    time.tm_mday = _day.count();
    time.tm_hour = 0;
    time.tm_min  = 0;
    time.tm_sec  = 0;

    return timegm(&time);
}

const Day&   DateTime::day() const 
{ 
    return _day; 
}
const Month& DateTime::month() const 
{ 
    return _month; 
}
const Year&  DateTime::year() const
{
    return _year;
}

bool DateTime::operator <  (const DateTime& rhs) const
{
    return std::tie(_year, _month, _day) < std::tie(rhs._year, rhs._month, rhs._day);
}
bool DateTime::operator >  (const DateTime& rhs) const
{
    return !(*this < rhs);
}
bool DateTime::operator >= (const DateTime& rhs) const
{
    return *this > rhs || *this == rhs;
}
bool DateTime::operator <= (const DateTime& rhs) const
{
    return *this < rhs || *this == rhs;
}
bool DateTime::operator == (const DateTime& rhs) const
{
    return _year == rhs._year && _month == rhs._month && _day == rhs._day;
}

