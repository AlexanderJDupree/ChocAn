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

Address::Address(std::string street, std::string city, std::string state, unsigned zip)
    : street(street), city(city), state(state), zip(zip){

        if(!ok()){

            throw address_error(report_issues());
        }

    }
}

bool Address::ok() const{

    if(street.length() > 25 || street.empty()) return false;

    //TODO add checks for other fields
}

std::string Address::report_issues(){

    std::string issue_brief("ERROR(S): ");

    if(street.length() > 25) issue_brief += " street name too long\n";

    if(street.empty()) issue_brief += " street name empty\n";

    //TODO add briefs for other fields
}


