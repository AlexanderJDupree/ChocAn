/* 
 
File: address.hpp

Brief: Address class provides the required info needed for a physical address.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/
 
*/

#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <string>
#include <exception>

struct address_error{

    std::string error;

    address_error(std::string error) : error(error) {};

    std::string what() const throw(){ 
        
        return error;
    }

};

class Address{

    public:

        Address(std::string street, std::string city, std::string state, unsigned zip);

//--utilities
        bool ok() const;
        std::string report_issues();

    private:

        std::string street;
        std::string city;
        std::string state;
        unsigned zip;
};

#endif
