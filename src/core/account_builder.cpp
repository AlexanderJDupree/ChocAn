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
#include <ChocAn/core/utils/overloaded.hpp>

const std::set<std::string> Account_Builder::US_states
{ "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA"
, "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD"
, "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ"
, "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC"
, "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY" };


bool Account_Builder::buildable() const
{
    return build_state.empty();
}

Account_Builder& Account_Builder::reset()
{
    fields.type.reset();
    fields.first.reset();
    fields.last.reset();
    fields.street.reset();
    fields.city.reset();
    fields.state.reset();
    fields.zip.reset();

    return *this;
}

void Account_Builder::request_updates_to_account(const Build_Stack& updates_needed)
{
    build_state = updates_needed;
}

void Account_Builder::apply_updates_to_account(Account& account)
{    
    if (!buildable())
    {
        throw invalid_account_build("Attempt made to update prematurely", errors);
    }

    if(fields.first)   account.name().update_first    (fields.first.value());
    if(fields.last)    account.name().update_last     (fields.last.value());
    if(fields.street)  account.address().update_street(fields.street.value());
    if(fields.city)    account.address().update_city  (fields.city.value());
    if(fields.state)   account.address().update_state (fields.state.value());
    if(fields.zip)     account.address().update_zip   (fields.zip.value());
}

void Account_Builder::initiate_new_build_process()
{
    build_state = Build_Stack({Zip(),State(),City(),Street(),Last(),First(),Type()});
}

const Account Account_Builder::build_new_account(const ID_Generator& id_generator)
{
    if (!buildable())
    {
        throw invalid_account_build("Attempt made to build prematurely", errors);
    }

    try
    {
        return  Account(
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
        throw invalid_account_build("Failed to build account", errors);
    }
}

const std::string Account_Builder::get_status()
{
    return "here goes the status";
}

Account_Builder::Build_State Account_Builder::builder_state() const
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
    if(buildable()) return;

    std::visit(overloaded{
            [&] (const Type&)  {deriveType(input);},
            [&] (const First&) {deriveFirst(input);},
            [&] (const Last&)  {deriveLast(input);},
            [&] (const Street&){deriveStreet(input);},
            [&] (const City&)  {deriveCity(input);},
            [&] (const State&) {deriveState(input);},
            [&] (const Zip&)   {deriveZip(input);}
            }, build_state.top());
}

void Account_Builder::set_field(const std::string &input)
{
    if (buildable()) return;

    errors.clear();

    accept_input(input);

    if(errors.empty()) build_state.pop();
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
