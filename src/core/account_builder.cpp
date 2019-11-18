/* 
File: account_builder.cpp

Brief:  Account builder performs exception handling and guides the user
        through the process of creating an account.

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/core/utils/account_builder.hpp>

bool Account_Builder::buildable() const{

    return fields.empty();
}

Account Account_Builder::build() const{
                   

    if(!buildable()) throw chocan_user_exception("Cannot create account", {});

    //TODO put try/catch blocks around stoul
    unsigned zip = 97080;//std::stoul(account_info.at("Zip"));

    Name name(account_info.at("First Name")
             ,account_info.at("Last Name"));

    Address address(account_info.at("Street")
                   ,account_info.at("City")
                   ,account_info.at("State")
                   ,zip);

    if(account_info.at("Account Type") == "Member") 
        return Account(name, address, Member(), id_generator);
    
    else if(account_info.at("Account Type") == "Manager") 
        return Account(name, address, Manager(), id_generator);
            
    return Account(name, address, Provider(), id_generator);
}

Account_Builder& Account_Builder::reset(){
    
    while(!fields.empty()) fields.pop();

    fields.push("Zip");
    fields.push("State");
    fields.push("City");
    fields.push("Street");
    fields.push("Last Name");
    fields.push("First Name");
    fields.push("Account Type");

    return *this;

}

std::string Account_Builder::set_current_field(const std::string& input){

    //TODO add validator method to ensure valid fields
    //std::string = invalidating characteristics of the field

    if(buildable()) return "";

    account_info[fields.top()] = input;
    
    fields.pop();

    return "";
}

std::string Account_Builder::get_current_field(){

    if(fields.empty()) return "";

    std::string field = fields.top();

    return field;

}


