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
#include <ChocAn/core/utils/validators.hpp>

Name::Name(const std::string& first, const std::string& last)
        : _first ( first ) , _last ( last ) 
{
    chocan_user_exception::Info errors;

    invalid_name name_errors("", {});

    if (!Validators::length(first, 1, 24) && Validators::length(last, 1, 24))
        errors.push_back("First Name must be 1 to 24 characters long");

    else if (!Validators::length(last, 1, 24) && Validators::length(first, 1, 24))
        errors.push_back("Last Name must be 1 to 24 characters long");

    else
        errors.push_back("Full Name must be 1 to 25 characters long");

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
