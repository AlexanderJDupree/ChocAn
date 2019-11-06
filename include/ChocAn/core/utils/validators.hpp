/* 
 
File: validators.hpp

Brief: Validitors file defines a set of validating functions for a variety of
       use cases.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_VALIDATORS_HPP
#define CHOCAN_VALIDATORS_HPP

#include <string>

namespace Validators
{

bool length(const std::string& field, unsigned min, unsigned max);

bool range(int value, int min, int max);

}; // namespace Validators

#endif // CHOCAN_VALIDATORS_HPP
