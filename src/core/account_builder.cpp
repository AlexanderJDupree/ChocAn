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
#include <algorithm>

bool Account_Builder::buildable() const
{
    return fields.type && name && address;
}

Account Account_Builder::build()
{
    if (!buildable())
    {
        throw invalid_account_build("Attempt made to build prematurely", issues);
    }

    try
    {
        return Account(name.value(), address.value(), yield_account_type(fields.type.value()), id_generator);
    }
    catch (...)
    {

        throw invalid_account_build("Failed to build account", issues);
    }
}

Account_Builder &Account_Builder::reset()
{
    name.reset();
    address.reset();
    fields.type.reset();
    fields.first.reset();
    fields.last.reset();
    fields.street.reset();
    fields.city.reset();
    fields.state.reset();
    fields.zip.reset();

    issues.clear();
    
    build_state = 0;

    return *this;
}

const std::string Account_Builder::get_status()
{
    return "here goes the status";
}

int Account_Builder::get_state() const
{ 
    if(buildable()) return -1;

    return build_state;   
}

std::optional<const chocan_user_exception> Account_Builder::get_issues() const
{

    if (issues.empty())
        return std::optional<const invalid_account_build>();

    return invalid_account_build("Issues with account", issues);
}

void Account_Builder::accept_input(const std::string &input)
{
    switch(build_state){
            
            case 0: deriveType(input);
            break;
            case 1: fields.first = input;
            break;
            case 2: fields.last = input;
            break;
            case 3: fields.street = input;
            break;
            case 4: fields.city = input;
            break;
            case 5: fields.state = input;
            break;
            case 6: deriveZip(input);
            break;
            default:;
    }
}

void Account_Builder::set_field(const std::string &input)
{

    if (buildable()) return;

    issues.clear();

    accept_input(input);

    if(!name && fields.first && fields.last){

        try
        {
            name = Name(fields.first.value(),fields.last.value());
        }
        catch (const invalid_name &err)
        {

            issues.insert(issues.end(), err.info().begin(), err.info().end());
        }
    }

    else if (!address && fields.street && fields.city && fields.state && fields.zip) 
    {
        try
        {
            address = Address(fields.street.value(),fields.city.value(),fields.state.value(),fields.zip.value() );
        }
        catch (const invalid_address &err)
        {
            issues.insert(issues.end(), err.info().begin(), err.info().end());
        }
    }
    
    reset_fields_as_needed();

    transition_state();
}

void Account_Builder::reset_fields_as_needed()
{

    if (issues.empty())
        return;

    for (const std::string &it : issues)
    {
        if (it.find("First") != std::string::npos)
        {
            fields.first.reset();
        }
        else if (it.find("Last") != std::string::npos)
        {
            fields.last.reset();
        }
        else if (it.find("Full") != std::string::npos)
        {
            fields.first.reset();
            fields.last.reset();
        }
        else if (it.find("Street") != std::string::npos)
        {
            fields.street.reset();
        }
        else if (it.find("Cities") != std::string::npos)
        {
            fields.city.reset();
        }
        else if (it.find("State") != std::string::npos)
        {
            fields.state.reset();
        }
        else if (it.find("Zip") != std::string::npos)
        {
            fields.zip.reset();
        }
    }
}

void Account_Builder::transition_state(){

    if(buildable()) return;

    if(!fields.type){
        
        build_state = 0;
    }
    else if(!name){

        if(!fields.first){

            build_state = 1;

        }else{

            build_state = 2;
        }

    }else if(!address){

        if(!fields.street){
            
            build_state = 3;

        }else if(!fields.city){

            build_state = 4;
        
        }else if(!fields.state){

            build_state = 5;

        }else{

            build_state = 6;
        }
    }
}

void Account_Builder::deriveType(const std::string &input)
{
    if(input == "Manager" || input == "manager") fields.type = "Manager";
    
    else if(input == "Member" || input == "member") fields.type = "Member";
    
    else if(input == "Provider" || input == "provider") fields.type = "Provider";

    else issues.emplace_back(input + " is not a valid type");

}

void Account_Builder::deriveZip(const std::string &input)
{

    try{
        
        unsigned temp = stoul(input);

        fields.zip = temp;

    }catch(...){

        issues.emplace_back(input + " is not a valid zip");
    }
}
Account::Account_Type Account_Builder::yield_account_type(const std::string& type){

    if(type == "Manager") return Manager();
    
    if(type == "Provider") return Provider();

    return Member();
}
