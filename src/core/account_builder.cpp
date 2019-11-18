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
#include <ChocAn/core/utils/validators.hpp>

bool Account_Builder::buildable() const{

    return fields.empty();
}

Account Account_Builder::build() const{
                   
    unsigned zip = 0;

    if(!buildable()) throw chocan_user_exception("Error: Cannot create account with empty fields", {});

    try{
            zip = std::stoul(account_info.at("Zip"));
    
    }catch(std::invalid_argument){

        throw chocan_user_exception("Error: invalid argument given for Zip", {});

    }catch(std::out_of_range){

        throw chocan_user_exception("Error: Zip value was out of range", {});
    }

    Name name(account_info.at("First Name")
             ,account_info.at("Last Name"));

    Address address(account_info.at("Street")
                   ,account_info.at("City")
                   ,account_info.at("State")
                   ,zip);

    if(account_info.at("Account Type") == "Member") 
        return Account(name, address, Member(), id_generator);
    
    if(account_info.at("Account Type") == "Manager") 
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

void Account_Builder::set_current_field(const std::string& input){

    std::string issues;

    if(buildable()) return;
    
    issues = valid_input(fields.top(),input);

    if(issues == ""){

        account_info[fields.top()] = input;
        fields.pop();

    }else{

        fields.push(issues);
    }
   
    return;
}

std::string Account_Builder::get_current_issues(){

    if(fields.empty()) return "";
    
    std::string field = fields.top();

    if(field.find("ERROR:") != std::string::npos){

        fields.pop();
        return field;
    }
    
    return "";
}

std::string Account_Builder::get_current_field(){

    if(fields.empty()) return "";

    std::string field = fields.top();

    return field;
}

std::string Account_Builder::valid_input(const std::string& field, const std::string& input){

    std::string issues = "ERROR: ";

    if(field == "First Name" && !Validators::length(input,1,24)) {

        issues += " first name must be 1 to 24 characters long.";
    }
    
    if(field == "Last Name" && !Validators::length(input,1,24)) {

        issues += " last name must be 1 to 24 characters long.";
    
    }else if(field == "Last Name" && !Validators::length(input + account_info.at("First Name"),1,25)) {

        issues += " full name must be 1 to 25 characters long.";
        fields.push("First Name");
    }
    
    if(field == "Zip" && !Validators::length(input,5,5)) {

        issues += " zip must be 5 digits.";
    }
    
    if(field == "State" && !Validators::length(input,2,2)) {

        issues += " state must be abbreviated 2 character format.";
    }
    
    if(field == "City" && !Validators::length(input,1,14)) {

        issues += " city must be 1 to 14 characters long.";
    }
    
    if(field == "Street" && !Validators::length(input,1,25)) {

        issues += " street must be 1 to 25 characters long.";
    }

    return (issues == "ERROR: ") ? "" : issues;
}
