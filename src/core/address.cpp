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

Address::Address( const std::string& street 
                , const std::string& city 
                , const std::string& state 
                , const unsigned     zip   )
            : street ( street )
            , city   ( city   )
            , state  ( state  )
            , zip    ( zip    )
{
    chocan_user_exception::Info errors;

    ( !Validators::length(street, 25, 1) ) 
        ? errors.push_back("Street address must be less than 25 characters")
        : void();
    ( !Validators::length(city, 14, 1) ) 
        ? errors.push_back("City must be less than 14 character")
        : void();
    ( !Validators::length(state, 2, 2) ) 
        ? errors.push_back("State must be in abbreviated 2 character format")
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
    return street == rhs.street && city == rhs.city && state == rhs.state && zip == rhs.zip;
}

bool Address::operator!=(const Address& rhs) const
{
    return !(*this == rhs);
}