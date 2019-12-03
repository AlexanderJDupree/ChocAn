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

const std::set<std::string> Account_Builder::US_states
{ "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA"
, "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD"
, "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ"
, "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC"
, "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY" };

bool Account_Builder::buildable() const
{
    return build_state == Build_State::Buildable;
}

Account Account_Builder::build()
{
    if (build_state != Build_State::Buildable)
    {
        throw invalid_account_build("Attempt made to build prematurely", issues);
    }

    try
    {
        return Account(
                Name(    fields.first.value()
                        ,fields.last.value()),
                Address( fields.street.value()
                        ,fields.city.value()
                        ,fields.state.value()
                        ,fields.zip.value()), 
                yield_account_type(), 
                id_generator);
    }
    catch (...)
    {

        throw invalid_account_build("Failed to build account", issues);
    }
}

Account_Builder &Account_Builder::reset()
{
    fields.type.reset();
    fields.first.reset();
    fields.last.reset();
    fields.street.reset();
    fields.city.reset();
    fields.state.reset();
    fields.zip.reset();

    issues.clear();
    
    build_state = Build_State::Type;

    return *this;
}

const std::string Account_Builder::get_status()
{
    return "here goes the status";
}

const Build_State& Account_Builder::get_state() const
{
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
            
            case Build_State::Type: deriveType(input);
            break;
            case Build_State::First: deriveFirst(input);
            break;
            case Build_State::Last: deriveLast(input);
            break;
            case Build_State::Street: deriveStreet(input);
            break;
            case Build_State::City: deriveCity(input);
            break;
            case Build_State::State: deriveState(input);
            break;
            case Build_State::Zip: deriveZip(input);
            break;
            default:;
    }
}

void Account_Builder::set_field(const std::string &input)
{

    if (buildable()) return;

    issues.clear();

    accept_input(input);
    
    transition_state();
}

void Account_Builder::transition_state(){

    if (buildable())
        return;

    if (!fields.type)
    {
        build_state = Build_State::Type;
    }
    else if (!fields.first)
    {
        build_state = Build_State::First;
    }
    else if (!fields.last)
    {
        build_state = Build_State::Last;
    }
    else if (!fields.street)
    {
        build_state = Build_State::Street;
    }
    else if (!fields.city)
    {
        build_state = Build_State::City;
    }
    else if (!fields.state)
    {
        build_state = Build_State::State;
    }
    else if (!fields.zip)
    {
        build_state = Build_State::Zip;
    }
    else
    {
        build_state = Build_State::Buildable;
    }
}

bool Account_Builder::full_name_valid()const
{
    return Validators::length(fields.last.value_or(" ") + fields.first.value_or(" "), 1, 25);
}

void Account_Builder::deriveType(const std::string &input)
{
    if(input == "Manager" || input == "manager") fields.type = "Manager";
    
    else if(input == "Member" || input == "member") fields.type = "Member";
    
    else if(input == "Provider" || input == "provider") fields.type = "Provider";

    else issues.emplace_back(input + " is not a valid type");

}

void Account_Builder::deriveFirst(const std::string &input)
{

    if (!Validators::length(input, 1, 24)){

        issues.push_back("First Name must be 1 to 24 characters long");
    }
    else if (!full_name_valid())
    {
        issues.push_back("Full Name must be 1 to 25 characters long");
        fields.first.reset();
        fields.last.reset();
    }
    else
    {
        fields.first = input;
    }
}

void Account_Builder::deriveLast(const std::string &input)
{

    if (!Validators::length(input, 1, 24))
    {
        issues.push_back("Last Name must be 1 to 24 characters long");
    }
    else if (!full_name_valid())
    {
        issues.push_back("Full Name must be 1 to 25 characters long");
        fields.first.reset();
        fields.last.reset();
    }
    else
    {
        fields.last= input;
    }
}

void Account_Builder::deriveStreet(const std::string &input)
{
    if( !Validators::length(input, 1, 25) ) 
    {
        issues.push_back("Street address must be less than 25 characters");
    }
    else
    {
        fields.street = input;
    }
}

void Account_Builder::deriveCity(const std::string &input)
{
    if( !Validators::length(input, 1, 14) ) 
    {
        issues.push_back("Cities must be less than 14 character");
    }
    else
    {
        fields.city = input;
    }
    

}

void Account_Builder::deriveState(const std::string &input)
{
    std::string temp = input;

    for(char& c : temp) { c = std::toupper(c); }
    
    if( !Validators::length(temp, 2, 2) ) {

        issues.push_back("State must be in abbreviated 2 character format");
    }
    else if(US_states.find(temp) == US_states.end() )
    {
        issues.push_back("\"" + input + "\" is not a U.S. state");
    }
    else
    {
        fields.state = temp;
    }
    
}

void Account_Builder::deriveZip(const std::string &input)
{

    if( !Validators::length(input, 5, 5) ) 
    {
         issues.push_back("Zip code must be 5 digit number");
    }
    else{

        try
        {
            unsigned temp = stoul(input);

            fields.zip = temp;
        }
        catch (...)
        {

            issues.push_back(input + " is not a valid zip");
        }
    }
}
Account::Account_Type Account_Builder::yield_account_type() const{

    if(fields.type.value() == "Manager") return Manager();
    
    if(fields.type.value() == "Provider") return Provider();

    return Member();
}
