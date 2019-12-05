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



bool Account_Builder::buildable() const
{
    return build_state.empty();
}

bool Account_Builder::approve_build(char input)
{
    if(input == 'Y' || input == 'y') return true;

    reset();
    
    return false;
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

    name.reset();
    address.reset();

    return *this;
}


void Account_Builder::request_update_to_account(Build_State update_needed)
{
    reset();
    
    build_state.push(update_needed);
}

void Account_Builder::apply_updates_to_account(Account& account)
{    
    if (!buildable())
    {
        throw invalid_account_build("Attempt made to update prematurely", errors);
    }
    
    account.name() = Name(fields.first.value_or(account.name.first(),
                );
    account.address() = address.value();
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
        return  Account(Name(name.value()),Address(address.value()),yield_account_type(),id_generator);
    }
    catch (...)
    {
        throw invalid_account_build("Failed to build account", errors);
    }
}

const std::string Account_Builder::get_status()
{
    std::string status("\n");
    
    status += "Account Type   [" + fields.type.value_or("Manager?, Provider?, Member?") + "]\n\n";

    status += "Name on Account:\n\t";
    status += "First [" + fields.first.value_or(std::string(24,'_')) + "]\t";
    status += "Last [" + fields.last.value_or(std::string(24, '_')) + "]\n";
    
    status += "Address for Account:\n\t";
    status += "Street [" + fields.street.value_or(std::string(25,'_')) + "]\n\t";
    status += "City [" + fields.city.value_or(std::string(14,'_')) + "]\t";
    status += "State [" + fields.state.value_or(std::string(2,'_')) + "]\t";

    if(fields.zip)    status += "Zip [" + std::to_string(fields.zip.value()) + "]" + "\nBuild Complete";
    else              status += "Zip [" + std::string(5,'_') + "]";

    return status;
}

Account_Builder::Build_State Account_Builder::builder_state() const
{
    if(build_state.empty()) return Idle();
    return build_state.top();   
}

std::optional<const chocan_user_exception> Account_Builder::get_errors() const
{

    if (errors.empty())
        return std::optional<const invalid_account_build>();

    return invalid_account_build("Error:", errors);
}

void Account_Builder::accept_input(const std::string &input)
{
    if(buildable()) return;

    std::visit(overloaded{
            [&] (const Type&)  {deriveType(input);},
            [&] (const First&) {fields.first  = input;},
            [&] (const Last&)  {fields.last   = input;},
            [&] (const Street&){fields.street = input;},
            [&] (const City&)  {fields.city   = input;},
            [&] (const State&) {fields.state  = input;},
            [&] (const Zip&)   {deriveZip(input);},
            [&] (const Idle&)  {;}
            }, build_state.top());
}

void Account_Builder::maybe_build_name()
{
    if(buildable()) return;

    if(!name && fields.first && fields.last){

        try{
            
            name = Name(fields.first.value(),fields.last.value());
        }
        catch(const invalid_name& error)
        {
            adapt_to_name_errors(error.specific_errors);
        }
    }

}

void Account_Builder::maybe_build_address()
{
    if(buildable()) return;
    
    if(!address && fields.street && fields.city && fields.state && fields.zip)
    {
        try{
            address = Address(fields.street.value(),fields.city.value(),fields.state.value(),fields.zip.value());
        }
        catch(const invalid_address& errors)
        {
            adapt_to_address_errors(errors.specific_errors);
        }
    }
}

void Account_Builder::adapt_to_name_errors(const Errors_With_Name& errors)
{
    for(const invalid_name::Name_Errors& err : errors)
    {
        std::visit(overloaded{
               
                [&](const invalid_name::Bad_First&){fields.first.reset();},
                [&](const invalid_name::Bad_Last&) {fields.last.reset();},
                [&](const invalid_name::Bad_Full&) {fields.last.reset();fields.first.reset();}
                }
                ,err);
    }
}

void Account_Builder::adapt_to_address_errors(const Errors_With_Address& errors)
{
    for(const invalid_address::Address_Errors& err : errors)
    {
        std::visit(overloaded{
            
                [&](const invalid_address::Bad_Street&){fields.street.reset();},
                [&](const invalid_address::Bad_City&)  {fields.city.reset();},
                [&](const invalid_address::Bad_State&) {fields.state.reset();},
                [&](const invalid_address::Bad_Zip&)   {fields.zip.reset();}
                }
                ,err);
    }
}

void Account_Builder::set_field(const std::string &input)
{
    if (buildable()) return;

    errors.clear();

    accept_input(input);
    
    maybe_build_name();
    
    maybe_build_address();

    if(errors.empty()) build_state.pop();
}

void Account_Builder::deriveType(const std::string &input)
{
    if(input == "Manager" || input == "manager") fields.type = "Manager";
    
    else if(input == "Member" || input == "member") fields.type = "Member";
    
    else if(input == "Provider" || input == "provider") fields.type = "Provider";

    else errors["Account Type"] = Invalid_Value {input, "Manager,Provder,Member"};

}

void Account_Builder::deriveZip(const std::string &input)
{
    try
    {
        unsigned temp = stoul(input);

        fields.zip = temp;
    }
    catch (...)
    {

        errors["Zip"] = Incompatible_Values {input,"Number"};
        fields.zip.reset();
    }
}
Account::Account_Type Account_Builder::yield_account_type() const{

    if(fields.type.value() == "Manager") return Manager();
    
    if(fields.type.value() == "Provider") return Provider();

    return Member();
}
