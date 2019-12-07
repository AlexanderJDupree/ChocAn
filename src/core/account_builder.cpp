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

    // Reset build state
    build_state = {};
    return *this;
}


void Account_Builder::request_update_to_account(std::vector<Build_State> updates_needed)
{
    reset();
    for(const auto& update : updates_needed)
    {
        if(std::holds_alternative<Type>(update)) return;

        build_state.push(update);
    }
}

void Account_Builder::apply_updates_to_account(Account& account)
{    
    if (!buildable())
    {
        errors["Builder State at Exception"] = Invalid_Value{get_status(),""};
        
        throw invalid_account_build("Attempt made to update prematurely", errors);
    }
    
    account.name() = Name(fields.first.value_or(account.name().first())
                         ,fields.last.value_or(account.name().last()));

    account.address() = Address(fields.street.value_or(account.address().street())
                               ,fields.city.value_or(account.address().city())
                               ,fields.state.value_or(account.address().state())
                               ,fields.zip.value_or(account.address().zip()));
}

Account_Builder& Account_Builder::initiate_new_build_process()
{
    reset();

    build_state = Build_Stack({Zip(),State(),City(),Street(),Last(),First(),Type()});

    return *this;
}

const Account Account_Builder::build_new_account(const ID_Generator& id_generator)
{
    if (!buildable())
    {
        errors["Builder State at Exception"] = Invalid_Value{get_status(),""};
        
        throw invalid_account_build("Attempt made to build prematurely", errors);
    }
    
    return  Account( name.value() ,address.value(), yield_account_type(), id_generator);
}

const std::string Account_Builder::get_status()
{
    std::string status("\n");
    
    status += "Account Type[" + fields.type.value_or("Manager, Provider, Member") + "]\n\n";

    status += "Name on Account:\n\t";
    status += "First[" + fields.first.value_or(std::string(24,'_')) + "]\t";
    status += "Last[" + fields.last.value_or(std::string(24, '_')) + "]\n";
    
    status += "Address for Account:\n\t";
    status += "Street[" + fields.street.value_or(std::string(25,'_')) + "]\n\t";
    status += "City[" + fields.city.value_or(std::string(14,'_')) + "]\t";
    status += "State[" + fields.state.value_or(std::string(2,'_')) + "]\t";

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
    if (buildable())
        return;

    try
    {
        //a valid name object is maintained through out the build. only invalid user input can result in an invalid name 
        name = Name(fields.first.value_or("Pending"), fields.last.value_or("Pending"));
    }
    catch (const invalid_name &error_report)
    {
        adapt_to_name_errors(error_report.specific_errors);

        errors = error_report.error_info;

        name.reset();
    }
}

void Account_Builder::maybe_build_address()
{
    if (buildable())
        return;

    try
    {
        //a valid address object is maintained through out the build. only invalid user input can result in an invalid address 
        address = Address(fields.street.value_or("Pending"), fields.city.value_or("Pending"), fields.state.value_or("AK"), fields.zip.value_or(97080));
    }
    catch (const invalid_address &error_report)
    {
        adapt_to_address_errors(error_report.specific_errors);

        errors = error_report.error_info;

        address.reset();
    }
}

void Account_Builder::adapt_to_name_errors(const Errors_With_Name& error_report)
{
    for(const invalid_name::Name_Errors& err : error_report)
    {
        std::visit(overloaded{
               
                [&](const invalid_name::Bad_First&){fields.first.reset();},
                [&](const invalid_name::Bad_Last&) {fields.last.reset();},
                [&](const invalid_name::Bad_Full&) {fields.last.reset();fields.first.reset();}
                }
                ,err);
    }
}

void Account_Builder::adapt_to_address_errors(const Errors_With_Address& error_report)
{
    for(const invalid_address::Address_Errors& err : error_report)
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
    std::string temp(input);

    for(char& c : temp){c = toupper(c);}
    
    if(temp == "MANAGER") fields.type = "Manager";
    
    else if(temp == "MEMBER") fields.type = "Member";
    
    else if(temp == "PROVIDER") fields.type = "Provider";

    else errors["Account Type"] = Invalid_Value {input, "Manager, Provider, Member"};

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

        errors["Zip"] = Incompatible_Values {input,"[five digit number]"};
        fields.zip.reset();
    }
}
Account::Account_Type Account_Builder::yield_account_type() const{

    if(fields.type.value() == "Manager") return Manager();
    
    if(fields.type.value() == "Provider") return Provider();

    return Member();
}
