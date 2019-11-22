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
#include <ChocAn/core/utils/validators.hpp>

const std::set<std::string> Address::US_states
{ "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA"
, "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD"
, "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ"
, "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC"
, "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY" };

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

    // Capitalize state
    for(char& c : _state) { c = std::toupper(c); }

    ( !Validators::length(street, 1, 25) ) 
        ? errors.push_back("Street address must be less than 25 characters")
        : void();
    ( !Validators::length(city, 1, 14) ) 
        ? errors.push_back("City must be less than 14 character")
        : void();
    ( !Validators::length(state, 2, 2) ) 
        ? errors.push_back("State must be in abbreviated 2 character format")
        : void();
    ( US_states.find(_state) == US_states.end() )
        ? errors.push_back(state + " is not a U.S. state")
        : void();
    ( !Validators::length(std::to_string(zip), 5, 5) ) 
        ? errors.push_back("Zip code must be 5 digits")
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
