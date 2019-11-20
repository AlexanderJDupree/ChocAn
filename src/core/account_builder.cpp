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
#include <algorithm>

const std::string account_type = "Account Type";
const std::string full_Name = "Full Name";
const std::string street_address = "Address";

bool Account_Builder::buildable() const{

    return fields.empty() && !issues;
}

Account Account_Builder::build(){
                   
    unsigned zip = 0;

    if(!buildable()) throw chocan_user_exception("Cannot create account with empty fields", {});

    try{
        
        zip = std::stoul(account_info.at("Zip"));
    
    }catch(const std::invalid_argument&){

        issues.emplace(chocan_user_exception("Zip must be 5 digit number", {}));

    }catch(const std::out_of_range&){

        issues.emplace(chocan_user_exception("Zip value was out of range", {}));
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

    fields.push(Address);
    fields.push(Full_Name);
    fields.push(Account_Type);

    return *this;

}

void Account_Builder::set_current_field(const std::string& input){

    if(buildable()) return;

    std::string current_field = fields.top();
    
    if(current_field == account_type){

        std::string type = valid_account_type(input);

        if(type != "")  account_info[account_type] = input;

        else issues.emplace(chocan_user_exception("Invalid account type request", {}));

    }else if(current_field == full_Name){

        parseName(input);

        try{

            name = Name(account_info.at("First Name"), account_info.at("Last Name"));

        }catch(const chocan_user_exception& err){

            issues.emplace(err);
        }

    }else if(current_field == street_address){

        parseAddress(input);

        try{

            address = Address(account_info.at("Street"),account_info.at("City"),account_info.at("State"),account_info.at("Zip"));

        }catch(const chocan_user_exception& err){

            issues.emplace(err);
        }
    }
            
    if(!issues) fields.pop();
   
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

const std::string Account_Builder::get_formating() const{

}

const std::string Account_Builder::get_current_field() const{

    if(fields.empty()) return "";

    return fields.top();
}

void Account_Builder::parseName(const std::string& input){
    
    account_info["First Name"] = input.substr(0,input.find(" "));
    account_info["Last Name" ] = input.substr(input.find(" "), input.length());
}

void Account_Builder::parseAddress(const std::string& input){

    std::string temp;
    std::stack<std::string> current_field({"Zip","State","City","Street"});

    for(std::string::const_iterator it = input.begin(); it != input.end() && !current_field.empty(); ++it){

        switch(*it){

            case ',' : {

                account_info[current_field.top()] = temp;
                current_field.pop();
                temp.clear();

            }break;

            case ' ': break;

            default: temp += *it;

        }
    }

    if(!current_field.empty()) account_info[current_field.top()] = temp;
}


std::string Account_Builder::valid_account_type(const std::string& input){

    std::string temp("");

    for(std::vector<std::string>::iterator it = valid_types.begin(); it != valid_types.end(); ++it){

        if(input == *it){

            temp = *it;
            temp[0] = toupper(temp[0]);
            return temp;
        } 
    }

    return temp;
}
