/* 
 
File: name.hpp

Brief: Name handles logic and checks for name fields in accounts

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_NAME_HPP
#define CHOCAN_NAME_HPP

#include <string>
#include <ChocAn/core/utils/exception.hpp>

class Name
{
public:

    Name(const std::string& first, const std::string& last);

    bool operator==(const Name& rhs) const;
    bool operator!=(const Name& rhs) const;

    const std::string first;
    const std::string last;
};

struct invalid_name : public chocan_user_exception
{
    explicit invalid_name(const char* err, Info info) 
        : error_info ( std::move(info) )
        , error_msg  ( err ) 
        { }

    const char* what() const noexcept
    {
        return error_msg;
    }

    const Info& info() const noexcept
    {
        return error_info;
    }

    const Info error_info;
    const char* error_msg;
};


#endif // CHOCAN_NAME_HPP