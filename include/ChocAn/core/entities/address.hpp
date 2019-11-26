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

#include <set>
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

    const std::string& street() const { return _street; }
    const std::string& city()   const { return _city;   }
    const std::string& state()  const { return _state;  }
    unsigned zip() const { return _zip;    }

private:

    std::string _street;
    std::string _city;
    std::string _state;
    unsigned _zip;

    static const std::set<std::string> US_states;
};

struct invalid_address : public chocan_user_exception
{
    const std::string street_range = "Street address must be less than 25 characters";
    const std::string city_range = "Cities must be less than 14 character";
    const std::string state_range = "State must be in abbreviated 2 character format";
    const std::string not_US_state = " is not a U.S. state";
    const std::string zip_range = "Zip code must be 5 digit number";
    
    explicit invalid_address(const char* err, Info info) 
        : chocan_user_exception(err, info)
        { }
};

#endif // CHOCAN_ADDRESS_HPP
