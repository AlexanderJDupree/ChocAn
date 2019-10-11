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
#include <vector>
#include <exception>

struct address_error : std::exception{

    std::vector<std::string> error_log;

    explicit address_error(std::vector<std::string>& errors) : error_log(errors) {};

    const char* what() const throw(){
        
        return "Error with constructing address object";
    }

    std::vector<std::string> get_info() const throw(){

        return error_log;
    }

};

class Address{

    public:

        Address(std::string& street, std::string& city, std::string& state, unsigned zip);

        bool ok() const;

    private:

        std::string street;
        std::string city;
        std::string state;
        unsigned zip;
};

#endif
