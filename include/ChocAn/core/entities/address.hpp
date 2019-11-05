/* 
 
File: address.hpp

Brief: Address class provides the required info needed for a physical address.

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef CHOCAN_ADDRESS_HPP
#define CHOCAN_ADDRESS_HPP

#include <ChocAn/core/utils/exception.hpp>

class Address
{
public:

    Address( const std::string& street 
           , const std::string& city
           , const std::string& state
           , const unsigned     zip   );

    bool operator == (const Address& rhs) const;
    bool operator != (const Address& rhs) const;

    const std::string street;
    const std::string city;
    const std::string state;
    const unsigned zip;
};

struct invalid_address : public chocan_user_exception
{
    explicit invalid_address(const char* err, Info info) 
        : error_info ( std::move(info) )
        , error_msg  ( err ) 
        { }

    const char* what() const noexcept
    {
        return error_msg;
    }

    const Info& info() const noexcept
    {
        return error_info;
    }

    const Info error_info;
    const char* error_msg;
};

#endif // CHOCAN_ADDRESS_HPP
