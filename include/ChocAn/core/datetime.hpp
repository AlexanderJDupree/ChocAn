/* 
 
File: datetime.hpp

Brief: DateTime class standardizes the date time formats throughout ChocAn data
       processing services and provides methods for validation and receiving 
       the current date and time

Authors: Daniel Mendez 
         Alexander Salazar
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
#include <ChocAn/core/exception.hpp>

class datetime_unit
{
public:

    explicit datetime_unit(unsigned val = 1) : _val(val) {};
    
    virtual bool ok() const = 0;
    
    unsigned to_int() const { return _val; }

    explicit operator unsigned() const { return _val; }

protected:

    unsigned _val;

};

class Day : public datetime_unit
{
public:

    explicit Day(unsigned day = 1) : datetime_unit(day) {};

    bool ok() const override;

    bool operator <  (const Day& rhs) const;
    bool operator >  (const Day& rhs) const;
    bool operator >= (const Day& rhs) const;
    bool operator <= (const Day& rhs) const;
    bool operator == (const Day& rhs) const;
};

class Month : public datetime_unit
{
public:

    explicit Month(unsigned month = 1) : datetime_unit(month) {};

    bool ok() const override;

    bool operator <  (const Month& rhs) const;
    bool operator >  (const Month& rhs) const;
    bool operator >= (const Month& rhs) const;
    bool operator <= (const Month& rhs) const;
    bool operator == (const Month& rhs) const;
};

class Year : public datetime_unit
{
public:

    explicit Year(unsigned year = 2019) : datetime_unit(year) {};

    bool ok() const override;
    bool is_leap_year() const;

    bool operator <  (const Year& rhs) const;
    bool operator >  (const Year& rhs) const;
    bool operator >= (const Year& rhs) const;
    bool operator <= (const Year& rhs) const;
    bool operator == (const Year& rhs) const;
};


// Wraps std::chrono methods to return a tm (time) object
struct system_clock
{
    static tm get_utc_time();
};

class DateTime
{
public:

    DateTime(Day day, Month month, Year year);

    // remove system clock dependency by templating clock type
    template <typename clock_t = system_clock>
    static DateTime get_current_datetime();

    bool ok() const;

    const Day&   day() const;
    const Month& month() const;
    const Year&  year() const;

    bool operator <  (const DateTime& rhs) const;
    bool operator >  (const DateTime& rhs) const;
    bool operator >= (const DateTime& rhs) const;
    bool operator <= (const DateTime& rhs) const;
    bool operator == (const DateTime& rhs) const;

protected:

    Day   _day;
    Month _month;
    Year  _year;

};


template <typename clock_t>
DateTime DateTime::get_current_datetime()
{
    tm utc_time = clock_t::get_utc_time();

    // TODO 1 and 1900  are magic numbers
    return DateTime( Day(utc_time.tm_mday)
                   , Month(utc_time.tm_mon + 1)
                   , Year(utc_time.tm_year + 1900));
}


// Custom Exception
class invalid_datetime : public chocan_user_exception
{
public:

    const char* error;
    info exception_info;

    invalid_datetime(const char* error, info exception_info)
        : error(error), exception_info(std::move(exception_info)) {};

    const char* what() const throw()
    {
        return error;
    }

    const info& get_info() const
    {
        return exception_info;
    }

};

#endif // CHOCAN_DATETIME_HPP

