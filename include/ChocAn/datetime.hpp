/* 
 
File: datetime.hpp

Brief: DateTime class standardizes the date time formats throughout ChocAn data
       processing services and provides methods for validation and reporting.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/
 
*/

#ifndef DATETIME_HPP
#define DATETIME_HPP

#include <tuple>
#include <chrono>
#include <exception>

struct invalid_datetime : std::exception
{
    typedef std::tuple<unsigned, unsigned, unsigned> Info;

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

class datetime_unit
{
public:

    datetime_unit(unsigned val = 1) : _val(val) {};
    
    virtual bool ok() const = 0;

    virtual operator unsigned() const { return _val; }

protected:

    unsigned _val;

};

class Day : public datetime_unit
{
public:

    Day(unsigned day = 1) : datetime_unit(day) {};

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

    Month(unsigned month = 1) : datetime_unit(month) {};

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

    Year(unsigned year = 2019) : datetime_unit(year) {};

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

    DateTime(unsigned day = 4, unsigned month = 7, unsigned year = 1776);

    bool ok() const;

    const Day&   day() const;
    const Month& month() const;
    const Year&  year() const;

    bool operator <  (const DateTime& rhs) const;
    bool operator >  (const DateTime& rhs) const;
    bool operator >= (const DateTime& rhs) const;
    bool operator <= (const DateTime& rhs) const;
    bool operator == (const DateTime& rhs) const;

private:

    Day   _day;
    Month _month;
    Year  _year;

};

#endif // DATETIME_HPP
