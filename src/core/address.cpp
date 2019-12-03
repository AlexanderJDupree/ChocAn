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

#include <ChocAn/core/entities/address.hpp>

Address::Address( const std::string& street 
                , const std::string& city 
                , const std::string& state 
                , const unsigned     zip   )
            : _street ( street )
            , _city   ( city   )
            , _state  ( state  )
            , _zip    ( zip    )
{
    chocan_user_exception::Info errors;

    //Grants access to class specific errors messages
    invalid_address error_msg("",{});

    ( street == "" ) 
        ? errors.push_back("Address requires a street")
        
        : void();
    ( city == "" ) 
        ? errors.push_back("Address requires a city")

        : void();
    ( state == "" ) 
        ? errors.push_back("Address requires a state")

        : void();
    ( !zip ) 
        ? errors.push_back("Address requires a zip code")
        : void();
    ( !errors.empty() ) 
        ? throw invalid_address("Invalid address values", errors)
        : void();
}

bool Address::operator==(const Address& rhs) const
{
    return _street == rhs._street && _city == rhs._city && _state == rhs._state && _zip == rhs._zip;
}

bool Address::operator!=(const Address& rhs) const
{
    return !(*this == rhs);
}
