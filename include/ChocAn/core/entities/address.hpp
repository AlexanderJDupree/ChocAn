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
#include <vector>
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
    
    
    explicit invalid_address(const char* err, Info info) 
        : chocan_user_exception(err, info)
        { }

    struct Bad_Street{};
    struct Bad_City{};
    struct Bad_State{};
    struct Bad_Zip{};
    
    using Address_Errors = std::variant<Bad_Street,Bad_City,Bad_State,Bad_Zip>;

    std::vector<Address_Errors> specific_errors;
};

#endif // CHOCAN_ADDRESS_HPP
