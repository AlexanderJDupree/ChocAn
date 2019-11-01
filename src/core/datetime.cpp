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

#include <ChocAn/core/utils/datetime.hpp>

/** DATETIME CLASS **/

DateTime::DateTime(Day day, Month month, Year year)
    : _day(day), _month(month), _year(year)
{
    if (!ok())
    {
        throw invalid_datetime( "Invalid datetime values",
                              { "Day should be between 1 - 31"
                              , "Months should be between 1 - 12"
                              , "Years should be greater than 1970"
                              } );
    }
}

bool DateTime::ok() const
{
    const Day calendar[] = 
    {
        Day(31), Day((_year.is_leap_year() ? 29 : 28)), Day(31),
        Day(30), Day(31), Day(30), 
        Day(31), Day(31), Day(30), 
        Day(31), Day(31), Day(30)
    };

    return _year.ok() 
        && _month.ok() 
        && _day.ok() 
        && _day <= calendar[_month.to_int() - 1];
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
    return _year < rhs._year || _month < rhs._month || _day < rhs._day;
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

/** SYSTEM CLOCK INTERFACE **/

tm system_clock::get_utc_time()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    time_t time = std::chrono::system_clock::to_time_t(now);
    
    return *gmtime(&time);
}

/** DAY CLASS **/

bool Day::ok() const
{
    return (1 <= _val && _val <= 31);
};

bool Day::operator <  (const Day& rhs) const
{
    return _val < rhs._val;
}
bool Day::operator >  (const Day& rhs) const
{
    return !(*this < rhs);
}
bool Day::operator >= (const Day& rhs) const
{
    return *this > rhs || *this == rhs;
}
bool Day::operator <= (const Day& rhs) const
{
    return *this < rhs || *this == rhs;
}
bool Day::operator == (const Day& rhs) const
{
    return _val == rhs._val;
}

/** MONTH CLASS **/

bool Month::ok() const
{
    return (1 <= _val && _val <= 12);
};

bool Month::operator <  (const Month& rhs) const
{
    return _val < rhs._val;
}
bool Month::operator >  (const Month& rhs) const
{
    return !(*this < rhs);
}
bool Month::operator >= (const Month& rhs) const
{
    return *this > rhs || *this == rhs;
}
bool Month::operator <= (const Month& rhs) const
{
    return *this < rhs || *this == rhs;
}
bool Month::operator == (const Month& rhs) const
{
    return _val == rhs._val;
}

/** YEAR CLASS **/

bool Year::ok() const
{
    return _val > 0;
};

bool Year::is_leap_year() const
{
    return _val % 4 == 0 && (_val % 100 != 0 || _val % 400 == 0);
}

bool Year::operator <  (const Year& rhs) const
{
    return _val < rhs._val;
}
bool Year::operator >  (const Year& rhs) const
{
    return !(*this < rhs);
}
bool Year::operator >= (const Year& rhs) const
{
    return *this > rhs || *this == rhs;
}
bool Year::operator <= (const Year& rhs) const
{
    return *this < rhs || *this == rhs;
}
bool Year::operator == (const Year& rhs) const
{
    return _val == rhs._val;
}

