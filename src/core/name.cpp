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

    if(!Validators::length(first + last, 1, 25) || !Validators::length(first,1,24) || !Validators::length(last,1,24)) {

        invalid_name name_errors("",{});

        if(!Validators::length(first, 1, 24) && Validators::length(last, 1, 24)) 
            errors.push_back(name_errors.first_name_out_of_range);
        
        else if(!Validators::length(last, 1, 24) && Validators::length(first, 1, 24) ) 
            errors.push_back(name_errors.last_name_out_of_range);

        else errors.push_back(name_errors.full_name_out_of_range);

    }

    if(!errors.empty() ) throw invalid_name("Invalid name length", errors);
}

bool Name::operator==(const Name& rhs) const
{
    return _first == rhs._first && _last == rhs._last;
}

bool Name::operator!=(const Name& rhs) const
{
    return !(*this == rhs);
}
