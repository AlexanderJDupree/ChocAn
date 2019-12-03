/* 
 
File: datetime.hpp

Brief: DateTime class standardizes the date time formats throughout ChocAn data
       processing services and provides methods for validation and receiving 
       the current date and time

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_DATETIME_HPP
#define CHOCAN_DATETIME_HPP

#include <ctime>
#include <chrono>
#include <ChocAn/core/utils/exception.hpp>
#include <ChocAn/core/utils/serializable.hpp>

// A day is 24 hours
using Day = std::chrono::duration
    <int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>::type>;

// A year is 365.2425 Days
using Year = std::chrono::duration
    <int, std::ratio_multiply<std::ratio<146097, 400>, Day::period>::type>;

// A Month is 1/12th of a Year
using Month = std::chrono::duration
    <int, std::ratio_divide<Year::period, std::ratio<12>>::type>;

using Hours   = std::chrono::hours;
using Minutes = std::chrono::minutes;
using Seconds = std::chrono::seconds;

class DateTime : public Serializable<DateTime, std::string, std::string>
{
public:

    DateTime(Day, Month, Year);
    DateTime(Day, Year, Month);
    DateTime(Month, Day, Year);
    DateTime(Month, Year, Day);
    DateTime(Year, Month, Day);
    DateTime(Year, Day, Month);
    DateTime(const Data_Table& data);
    DateTime(double unix_timestamp);
    DateTime(Day, Month, Year, Hours, Minutes, Seconds);

    virtual ~DateTime() = default;

    Data_Table serialize() const override;

    // remove system clock dependency by templating clock type
    template <typename clock_t = std::chrono::system_clock>
    static DateTime get_current_datetime();

    const Day&     day()     const;
    const Month&   month()   const;
    const Year&    year()    const;
    const Hours&   hour()    const;
    const Minutes& minutes() const;
    const Seconds& seconds() const;

    static bool is_leap_year(const Year& year);
    int unix_timestamp() const;

    bool operator <  (const DateTime& rhs) const;
    bool operator >  (const DateTime& rhs) const;
    bool operator >= (const DateTime& rhs) const;
    bool operator <= (const DateTime& rhs) const;
    bool operator == (const DateTime& rhs) const;

protected:

    bool ok() const;

    Day     _day;
    Month   _month;
    Year    _year;
    Hours   _hour;
    Minutes _minutes;
    Seconds _seconds;
};


template <typename clock_t>
DateTime DateTime::get_current_datetime()
{
    using namespace std::chrono;

    auto time_point = time_point_cast<Seconds>(clock_t::now());

    time_t time = system_clock::to_time_t(time_point);

    tm utc_time = *gmtime(&time);

    return DateTime( Day     { utc_time.tm_mday }
                   , Month   { utc_time.tm_mon + 1 }
                   , Year    { utc_time.tm_year + 1900 } 
                   , Hours   { utc_time.tm_hour }
                   , Minutes { utc_time.tm_min  }
                   , Seconds { utc_time.tm_sec  } );
}

class invalid_datetime : public chocan_user_exception
{
public:

    invalid_datetime(const char* error, Info exception_info)
        : chocan_user_exception(error, exception_info)
        { }
};

#endif // CHOCAN_DATETIME_HPP

