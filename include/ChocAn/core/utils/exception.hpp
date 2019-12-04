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
#include <string>
#include <exception>
#include <ChocAn/core/utils/error_type.hpp>

class chocan_user_exception : public std::exception
{
public:
    // Key is the field name, Error Type holds specific error info
    using Info = std::map<std::string, Error_Type>;

    Info error_info;
    std::string error_msg;

    chocan_user_exception(std::string err, Info info)
        : error_info ( std::move(info) )
        , error_msg  ( std::move(err)  )
        {}

    virtual const char* what() const noexcept
    {
        return error_msg.c_str();
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
    const std::string error_msg;

    chocan_db_exception(std::string err, Row_Info info)
        : error_info ( std::move(info) ) 
        , error_msg  ( std::move(err)  )
        { }

    virtual const char* what() const noexcept
    {
        return error_msg.c_str();
    } 

    virtual const Row_Info& info() const noexcept
    {
        return error_info;
    }
};

#endif // CHOCAN_EXCEPTION_HPP

