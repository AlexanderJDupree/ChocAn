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

Account_Builder::Account_Builder(Database_Ptr db) 
: id_generator(db), build_state({Zip(),State(),City(),Street(),Last(),First(),Type()})
{
    
}
Account_Builder::Account_Builder(Account& account_to_update, const Build_Stack& updates_requested) 
: account(account_to_update), build_state(updates_requested)
{

}
bool Account_Builder::buildable() const
{
    return std::holds_alternative<Buildable>(build_state.top());
}

const Account& Account_Builder::build_new_account()
{
    if (!buildable())
    {
        throw invalid_account_build("Attempt made to build prematurely", errors);
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
                id_generator.value());
    }
    catch (...)
    {

        throw invalid_account_build("Failed to build account", errors);
    }
}

const std::string Account_Builder::get_status()
{
    return "here goes the status";
}

const Account_Builder::Build_State& Account_Builder::builder_state() const
{
    return build_state.top();   
}

std::optional<const chocan_user_exception> Account_Builder::get_errors() const
{

    if (errors.empty())
        return std::optional<const invalid_account_build>();

    return invalid_account_build("Issues with account", errors);
}

void Account_Builder::accept_input(const std::string &input)
{
    //visitor pattern here
}

void Account_Builder::set_field(const std::string &input)
{

    if (buildable()) return;

    errors.clear();

    accept_input(input);
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

    else errors["Account Type"] = Invalid_Value {input, "Manager,Provder,Member"};

}

void Account_Builder::deriveFirst(const std::string &input)
{

    if (!Validators::length(input, 1, 24)){

        errors["First Name"] = Invalid_Length {input,1,24};
        
    }
    else if (!full_name_valid())
    {
        errors["Full Name"] = Invalid_Length {input,2,25};
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
        errors["Last Name"] = Invalid_Length {input,1,24};
    }
    else if (!full_name_valid())
    {
        errors["Full Name"] = Invalid_Length {input,2,25};
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
        errors["Street"] = Invalid_Length {input,1,25};
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
        errors["City"] = Invalid_Length {input,1,14};
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

        errors["State"] = Invalid_Length {input,2,2};
    }
    else if(US_states.find(temp) == US_states.end() )
    {
        errors["State"] = Invalid_Value {input,"Us state"};
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
        errors["State"] = Invalid_Length {input,5,5};
    }
    else{

        try
        {
            unsigned temp = stoul(input);

            fields.zip = temp;
        }
        catch (...)
        {

            errors["State"] = Incompatible_Values {input,"Number"};
        }
    }
}
Account::Account_Type Account_Builder::yield_account_type() const{

    if(fields.type.value() == "Manager") return Manager();
    
    if(fields.type.value() == "Provider") return Provider();

    return Member();
}
