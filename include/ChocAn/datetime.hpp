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

#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <tuple>
#include <ctime>
#include <chrono>
#include <iostream>
#include <exception>


class datetime_unit
{
public:

    explicit datetime_unit(unsigned val = 1) : _val(val) {};
    
    virtual bool ok() const = 0;

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

class DateTime
{
public:

    DateTime(Day day, Month month, Year year);
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

    static tm get_utc_time();

    Day   _day;
    Month _month;
    Year  _year;

};

// Custom Exception
struct invalid_datetime : std::exception
{
    typedef std::tuple<Day, Month, Year> Info;

    Info values;
    const char* error;

    invalid_datetime(Info values, const char* error)
        : values(values), error(error) {};

    const char* what() const throw()
    {
        return error;
    }

    Info get_info() const throw()
    {
        return values;
    }
};
#endif // DATETIME_HPP

