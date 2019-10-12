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

#include <map>
#include <exception>

/* 
 * chocan_user_exception is reserved for invalid IO action on behalf of the user. 
 * When creating the throwing the user exception it is important to populate
 * the Info table with the values that caused the exception 
 *
 */

class chocan_user_exception : public std::exception
{
public:
    typedef std::map<std::string, std::string> Info;

    virtual ~chocan_user_exception() {};

    // Implements std::exception interface
    virtual const char* what() const throw() = 0;

    virtual const Info& get_info() const = 0;
};


#endif // CHOCAN_EXCEPTIONS
