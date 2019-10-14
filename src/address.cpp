/* 
 
File: address.hpp

Brief: Address class standardizes the address format throughout the ChocAn data 
       services and provides methods for validation and reporting

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/address.hpp>

Address::Address(std::string& street, std::string& city, 
                 std::string& state, unsigned zip)
    : street(street), city(city), state(state), zip(zip)
{

        if(!ok())
        {
            throw invalid_address( { "Street address must be less than 25 characters" 
                                 , "City must be less than 14 characters"
                                 , "State must be in abbreviated 2 character form"
                                 , "Zip code must be 5 digits"
                                 } );
        }

}

bool Address::ok() const
{

    if(street.length() > 25 || street.empty() || street == " ") return false;

    if(city.length() > 14 || city.empty() || city == " ") return false;

    if(state.length() > 2 || state.empty() || state == " ") return false;

    // Range of 5 digit zip codes
    if(zip > 99999 || zip <= 10000) return false;

    return true;
}

