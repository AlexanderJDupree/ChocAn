/* 
 
File: name.cpp

Brief: Implementation for Name utility class

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/entities/name.hpp>

Name::Name(const std::string& first, const std::string& last)
        : _first ( first ) , _last ( last ) 
{
    chocan_user_exception::Info errors;

    invalid_name name_errors("", {});

    if (first == "")
        errors["First Name"] = Invalid_Value { first, "Cannot be empty" }; 

    if (last == "")
        errors["Last Name"] = Invalid_Value { last, "Cannot be empty" }; 

    if (!errors.empty())
        throw invalid_name("Invalid name length", errors);
}

bool Name::operator==(const Name& rhs) const
{
    return _first == rhs._first && _last == rhs._last;
}

bool Name::operator!=(const Name& rhs) const
{
    return !(*this == rhs);
}
