/* 
 
File: exceptions.hpp

Brief: chocAn exceptions provides standardized interfaces for specific exception
       cases within the application

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_EXCEPTION_HPP
#define CHOCAN_EXCEPTION_HPP

#include <map>
#include <vector>
#include <string>
#include <exception>

class chocan_user_exception : public std::exception
{
public:
    using Info = std::vector<std::string>;

    const Info error_info;
    const char* error_msg;

    chocan_user_exception(const char* err, Info info)
        : error_info ( std::move(info) )
        , error_msg  ( err )
        {}

    virtual const char* what() const noexcept
    {
        return error_msg;
    }

    virtual const Info& info() const noexcept
    {
        return error_info;
    }
};

class chocan_db_exception : public std::exception
{
public:

    using Row_Info = std::map<std::string, std::string>;

    const Row_Info error_info; // Holds the specific DB row that caused the error
    const char* error_msg;

    chocan_db_exception(const char* err, Row_Info info)
        : error_info ( std::move(info) ) 
        , error_msg  ( err )
        { }

   virtual const char* what() const noexcept
   {
       return error_msg;
   } 

    virtual const Row_Info& info() const noexcept
    {
        return error_info;
    }
};

#endif // CHOCAN_EXCEPTION_HPP

