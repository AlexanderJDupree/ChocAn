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

    const std::string& first() const { return _first; }
    const std::string& last() const  { return _last;  }

private:

    std::string _first;
    std::string _last;
};

struct invalid_name : public chocan_user_exception
{
    const std::string first_name_out_of_range = "First Name must be 1 to 24 characters long";
    const std::string last_name_out_of_range = "Last Name must be 1 to 24 characters long";
    const std::string full_name_out_of_range  = "Full Name must be 1 to 25 characters long";
    explicit invalid_name(const char* err, Info info) 
        : chocan_user_exception(err, info)
        { }
};


#endif // CHOCAN_NAME_HPP
