/* 
 
File: address.hpp

Brief: Address class standardizes the address format throughout the ChocAn data services
       and provides methods for validation and reporting

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/address.hpp>

Address::Address(std::string& street, std::string& city, std::string& state, unsigned zip)
    : street(street), city(city), state(state), zip(zip){

        if(!ok()){

            std::vector<std::string> fields = {street,city,state,std::to_string(zip)};

            throw address_error(fields);
        }

    }

bool Address::ok() const{

    if(street.length() > 25 || street.empty() || street == " ") return false;

    if(city.length() > 14 || city.empty() || city == " ") return false;

    if(state.length() > 2 || state.empty() || state == " ") return false;

    if(!zip) return false;

    return true;
}



