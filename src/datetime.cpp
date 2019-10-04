/* 
 
File: datetime.hpp

Brief: DateTime class standardizes the date time formats throughout ChocAn data
       processing services and provides methods for validation and reporting.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/datetime.hpp>

/** DATETIME CLASS **/

DateTime::DateTime(Day day, Month month, Year year)
    : _day(day), _month(month), _year(year)
{
    if (!ok())
    {
        throw invalid_datetime( std::make_tuple(day, month, year)
                              , "Invalid Date Time Values"      );
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
        && _day <= calendar[static_cast<unsigned>(_month) - 1];
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

