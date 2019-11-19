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
#include <ChocAn/core/utils/exception.hpp>

bool Account_Builder::buildable() const{

    return fields.empty();
}

Account Account_Builder::build(){
                   
    unsigned zip = 0;
    std::shared_ptr<Name> name_ptr;
    std::shared_ptr<Address> address_ptr;

    if(!buildable()) throw chocan_user_exception("Cannot create account with empty fields", {});

    try{
        
        zip = std::stoul(account_info.at("Zip"));
    
    }catch(const std::invalid_argument&){

        issues.emplace(chocan_user_exception("Zip must be 5 digit number", {}));

    }catch(const std::out_of_range&){

        issues.emplace(chocan_user_exception("Zip value was out of range", {}));
    }

    try{

        name_ptr = std::make_unique<Name>(account_info.at("First Name")
                                         ,account_info.at("Last Name"));
    
    }catch(const chocan_user_exception& err){
        
        issues.emplace(err);
    }
    
    try{
        
        address_ptr = std::make_unique<Address>(account_info.at("Street")
                                               ,account_info.at("City")
                                               ,account_info.at("State")
                                               ,zip);
    }catch(const chocan_user_exception& err){
        
        issues.emplace(err);
    }

    if(issues) throw issues;

    if(account_info.at("Account Type") == "Member") 
        return Account(*name_ptr, *address_ptr, Member(), id_generator);
    
    if(account_info.at("Account Type") == "Manager") 
        return Account(*name_ptr, *address_ptr, Manager(), id_generator);
            
    return Account(*name_ptr, *address_ptr, Provider(), id_generator);
}

Account_Builder& Account_Builder::reset(){
    
    while(!fields.empty()) fields.pop();

    fields.push("Zip");
    fields.push("State");
    fields.push("City");
    fields.push("Street");
    fields.push("Full Name");
    fields.push("Account Type");

    return *this;

}

void Account_Builder::set_current_field(const std::string& input){

    if(buildable()) return;
    
    if(fields.top() == "Full Name"){

        account_info["First Name"] = parseName(input,'f');
        account_info["Last Name"] = parseName(input,'l');
    }
    
    fields.pop();
   
    return;
}

const chocan_user_exception Account_Builder::get_current_issues(){

    std::optional<chocan_user_exception> return_msg = issues;

    if(issues){

        std::vector<std::string> error_msg(issues.value().info());

        for(std::vector<std::string>::iterator it = error_msg.begin(); it != error_msg.end(); ++it){

            //check for user input address errors
            if(it->find("Zip")    != std::string::npos) fields.push("Zip");
            if(it->find("Street") != std::string::npos) fields.push("Street");
            if(it->find("City")   != std::string::npos) fields.push("City");
            if(it->find("State")  != std::string::npos) fields.push("State");
            if(it->find("Name")   != std::string::npos) fields.push("Full Name");

        }

        issues.reset();
    }
    
    return return_msg.value_or(chocan_user_exception{"",{}});
}

std::string Account_Builder::get_current_field(){

    if(fields.empty()) return "";

    std::string field = fields.top();

    return field;
}

std::string Account_Builder::parseName(const std::string& input, char name_type){

    std::string temp; 
    unsigned index;
    
    if(name_type == 'f') temp = input.substr(0,input.find(" "));

    else if(name_type == 'l') temp = input.substr(input.find(" "), input.length());

    index = temp.length() - 1;

    while(!isalpha(temp[index])){

        temp.pop_back();
    }

    temp[0] = toupper(temp[0]);

    return temp;
}

