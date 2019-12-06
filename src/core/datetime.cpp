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

DateTime::DateTime(Day day, Month month, Year year, Hours hour, Minutes min, Seconds sec)
    : _day(day), _month(month), _year(year), _hour(hour), _minutes(min), _seconds(sec)
{
    chocan_user_exception::Info errors;

    if( !Validators::range(day.count(), 1, 31) )
    {
        errors["Day"] = Invalid_Range { day.count(), 1, 31 };
    }
    if( !Validators::range(hour.count(), 0, 23) )
    {
        errors["Hour"] = Invalid_Range { hour.count(), 0, 23 };
    }
    if( !Validators::range(min.count(), 0, 59) )
    {
        errors["Minutes"] = Invalid_Range { min.count(), 0, 59 };
    }
    if( !Validators::range(sec.count(), 0, 60) )
    {
        errors["Seconds"] = Invalid_Range{ sec.count(), 0, 59 };
    }
    if( !Validators::range(year.count(), 1970, Year::max().count()) )
    {
        errors["Year"] = Failed_With { std::to_string(year.count()), "Must be greater than greater than 1970" };
    }
    if( !Validators::range(month.count(), 1, 12) )
    {
        errors["Month"] = Invalid_Range { month.count(), 1, 12 };
    }
    if(!ok())
    {
        errors["Date"] = Incompatible_Values{ "Month=" + std::to_string(month.count())
                                            , "Day=" + std::to_string(day.count()) };
    }
    (errors.empty())
        ? void() :throw invalid_datetime("Invalid datetime values", errors);
}

DateTime::DateTime(Day day, Month month, Year year)
    : DateTime(day, month, year, Hours(0), Minutes(0), Seconds(0))
    { }

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
                        , Year(std::stoi(data.at("year"))) 
                        , Hours(std::stoi(data.at("hour")))
                        , Minutes(std::stoi(data.at("minutes")))
                        , Seconds(std::stoi(data.at("seconds")))
                        );
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

    *this = DateTime( Day     { utc_time.tm_mday }
                    , Month   { utc_time.tm_mon + 1 }
                    , Year    { utc_time.tm_year + 1900 } 
                    , Hours   { utc_time.tm_hour }
                    , Minutes { utc_time.tm_min  }
                    , Seconds { utc_time.tm_sec  } );
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
        { "day"     , std::to_string(_day.count())     },
        { "month"   , std::to_string(_month.count())   },
        { "year"    , std::to_string(_year.count())    },
        { "hour"    , std::to_string(_hour.count())    },
        { "minutes" , std::to_string(_minutes.count()) },
        { "seconds" , std::to_string(_seconds.count()) },
        { "unix"    , std::to_string(unix_timestamp()) }
    };
}

int DateTime::unix_timestamp() const
{
    std::tm time;

    time.tm_year = _year.count() - 1900;
    time.tm_mon  = _month.count() - 1;
    time.tm_mday = _day.count();
    time.tm_hour = _hour.count();
    time.tm_min  = _minutes.count();
    time.tm_sec  = _seconds.count();

    return timegm(&time);
}

const Day& DateTime::day() const 
{ 
    return _day; 
}
const Month& DateTime::month() const 
{ 
    return _month; 
}
const Year& DateTime::year() const
{
    return _year;
}
const Hours& DateTime::hour() const 
{ 
    return _hour; 
}
const Minutes& DateTime::minutes() const 
{ 
    return _minutes; 
}
const Seconds& DateTime::seconds() const
{
    return _seconds;
}
bool DateTime::operator <  (const DateTime& rhs) const
{
    return std::tie(_year, _month, _day, _hour, _minutes, _seconds) 
         < std::tie(rhs._year, rhs._month, rhs._day, rhs._hour, rhs._minutes, rhs._seconds);
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
    return _year == rhs._year && _month == rhs._month && _day == rhs._day && _hour == rhs._hour && _minutes == rhs._minutes && _seconds == rhs._seconds;
}

