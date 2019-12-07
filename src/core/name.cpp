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

    if (!Validators::length(first, 1, 24)){
        
        errors["First Name"] = Invalid_Length {first,1,24};
        name_errors.specific_errors.push_back(invalid_name::Bad_First());
    }
    else if (!Validators::length(last, 1, 24))
    {
        errors["Last Name"] = Invalid_Length {last,1,24};
        name_errors.specific_errors.push_back(invalid_name::Bad_Last());
    }
    else if (!Validators::length(first + " " + last, 1, 25))
    {
        errors["Full Name"] = Invalid_Length {first + " " + last,2,25};
        name_errors.specific_errors.push_back(invalid_name::Bad_Full());

    }
    else if(!Validators::is_alphabetic(first))
    {
        errors["First Name"] = Invalid_Value {first,"Alphabetic characters only"};
        name_errors.specific_errors.push_back(invalid_name::Bad_First());
    }
    else if(!Validators::is_alphabetic(last))
    {
        errors["Last Name"] = Invalid_Value {first,"Alphabetic characters only"};
        name_errors.specific_errors.push_back(invalid_name::Bad_Last());
    }

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
