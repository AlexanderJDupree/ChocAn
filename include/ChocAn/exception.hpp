/* 
 
File: exceptions.hpp

Brief: chocAn exceptions provides standardized interfaces for specific exception
       cases within the application

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_EXCEPTION_HPP
#define CHOCAN_EXCEPTION_HPP

#include <vector>
#include <string>
#include <exception>

/* 
 * chocan_user_exception is reserved for invalid IO action on behalf of the user. 
 * When creating and throwing the user exception it is important to populate
 * exception_info vector to provide feedback on what correct values are
 *
 */

class chocan_user_exception : public std::exception
{
public:
    typedef std::vector<std::string> info;

    virtual const char* what() const noexcept = 0;

    virtual const info& get_info() const = 0;
};

#endif // CHOCAN_EXCEPTION_HPP

