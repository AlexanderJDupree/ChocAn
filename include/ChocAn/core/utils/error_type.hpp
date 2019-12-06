/* 
 
File: error_type.hpp

Brief: error_type is a variant where each member of the union encodes the 
       offending value and how to correct it

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_ERROR_TYPE_HPP
#define CHOCAN_ERROR_TYPE_HPP

#include <string>
#include <variant>

struct Invalid_Length
{
    std::string value;
    unsigned min;
    unsigned max;
};

struct Invalid_Range
{
    long int value;
    long int min;
    long int max;
};

struct Incompatible_Values
{
    std::string val1;
    std::string val2;
};

struct Invalid_Value
{
    std::string value;
    std::string expected;
};

struct Failed_With
{
    std::string value;
    std::string reason;
};

using Error_Type = std::variant< Invalid_Length
                               , Invalid_Range
                               , Invalid_Value
                               , Incompatible_Values
                               , Failed_With
                               >;

#endif // CHOCAN_ERROR_TYPE_HPP
