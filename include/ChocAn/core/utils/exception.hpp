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

#include <vector>
#include <string>
#include <exception>

/* 
 * chocan_user_exception is reserved for invalid IO action on behalf of the user. 
 * When creating and throwing the user exception it is important to populate
 * exception_info vector to provide feedback on what correct values are
 *
 */

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

    const char* error_msg;

    chocan_db_exception(const char* err)
        : error_msg(err) 
        { }

   virtual const char* what() const noexcept
   {
       return error_msg;
   } 
};

#endif // CHOCAN_EXCEPTION_HPP

