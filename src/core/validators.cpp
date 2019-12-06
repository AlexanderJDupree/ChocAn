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

#include <ChocAn/core/utils/validators.hpp>

bool Validators::length(const std::string& field, unsigned min, unsigned max)
{
    return min <= field.length() && field.length() <= max;
}

bool Validators::range(int value, int min, int max)
{
    return min <= value && value <= max;
}

bool Validators::is_alphabetic(const std::string& word)
{
    for(const char c : word) {if(!std::isalpha(c)) return false;}

    return true;
}
