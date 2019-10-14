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

#include <ChocAn/exception.hpp>

struct invalid_address : public chocan_user_exception
{

    info exception_info;

    explicit invalid_address(info exception_info) 
        : exception_info(std::move(exception_info)) {};

    const char* what() const throw(){
        return "Error with constructing address object";
    }

    const info& get_info() const
    {
        return exception_info;
    }

};

class Address
{
    public:

        Address(std::string& street, std::string& city, 
                std::string& state, unsigned zip);

        bool ok() const;

    private:

        std::string street;
        std::string city;
        std::string state;
        unsigned zip;
};

#endif
