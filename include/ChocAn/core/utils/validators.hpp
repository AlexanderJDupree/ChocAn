/* 
 
File: validators.hpp

Brief: Validitors file defines a set of validators for specific use cases

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

namespace Validators
{

bool length(const std::string& field, unsigned max, unsigned min);

bool range(int value, int max, int min);

}; // namespace Validators

#endif // CHOCAN_VALIDATORS_HPP
