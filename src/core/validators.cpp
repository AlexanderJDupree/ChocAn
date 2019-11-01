/* 
 
File: validators.cpp

Brief: Validitors file defines a set of validators for specific use cases

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <string>
#include <ChocAn/core/utils/validators.hpp>

bool Validators::length(const std::string& field, unsigned max, unsigned min)
{
    return min <= field.length() && field.length() <= max;
}

bool Validators::range(int value, int max, int min)
{
    return min <= value && value <= max;
}