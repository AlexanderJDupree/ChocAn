/* 
 
File: name.hpp

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

    ( !Validators::length(first, 1, 25) ) 
        ? errors.push_back("First name cannot be empty")
        : void();
    ( !Validators::length(last, 1, 25) ) 
        ? errors.push_back("Last name cannot be empty")
        : void();
    ( !Validators::length(first + last, 1, 25) ) 
        ? errors.push_back("Full name must be less than 25 characters")
        : void();

    ( !errors.empty() ) 
        ? throw invalid_name("Invalid name length", errors)
        : void();
}

bool Name::operator==(const Name& rhs) const
{
    return _first == rhs._first && _last == rhs._last;
}

bool Name::operator!=(const Name& rhs) const
{
    return !(*this == rhs);
}
