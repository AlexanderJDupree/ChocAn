/* 
 
File: exceptions.hpp

Brief: chocAn exceptions provides standardized interfaces for specific exception
       cases within the application

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_EXCEPTIONS
#define CHOCAN_EXCEPTIONS

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

#endif // CHOCAN_EXCEPTIONS
