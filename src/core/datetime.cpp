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

