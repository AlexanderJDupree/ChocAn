/* 
File: transaction_builder.cpp

Brief:  Transaction builder implementaion

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <map>
#include <functional>
#include <ChocAn/core/utils/parsers.hpp>
#include <ChocAn/core/utils/overloaded.hpp>
#include <ChocAn/core/utils/transaction_builder.hpp>

bool Transaction_Builder::buildable() const
{
    return service && service_date && provider_acct && member_acct && comments;
}

Transaction Transaction_Builder::build() const
{
    if (!buildable()) { throw chocan_user_exception("Cannot complete transaction", { }); }

    return Transaction ( provider_acct.value()
                       , member_acct.value()
                       , service_date.value()
                       , service.value()
                       , comments.value() );
}

Transaction_Builder& Transaction_Builder::reset()
{
    state = Set_Provider_Acct();
    service.reset();
    service_date.reset();
    provider_acct.reset();
    member_acct.reset();
    comments.reset();
    error.reset();
    return *this;
}

Transaction_Builder::Builder_State Transaction_Builder::builder_state() const
{
    return state;
}

void Transaction_Builder::set_current_field(const std::string& input)
{
    error.reset();

    std::visit( overloaded {
        [&](const Set_Provider_Acct&)
        { 
            set_provider_acct_field(input);

            if(!error) { state = Set_Member_Acct(); }
        },
        [&](const Set_Member_Acct&)
        { 
            set_member_acct_field(input);

            if(!error) { state = Set_Service_Date(); }
        },
        [&](const Set_Service_Date&)
        { 
            set_service_date_field(input);

            if(!error) { state = Set_Service(); }
        },
        [&](const Set_Service&)
        { 
            set_service_field(input);

            if(!error) { state = Set_Comments(); }
        },
        [&](const Set_Comments&)
        { 
            set_comments_field(input);
        }
    }, state);

}

const std::optional<chocan_user_exception>& Transaction_Builder::get_last_error() const
{
    return error;
}

void Transaction_Builder::set_member_acct_field(const std::string& input)
{
    try
    {
        member_acct.emplace(db->get_member_account(input).value());
    }
    catch(const std::bad_optional_access&)
    {
        error.emplace(chocan_user_exception("Invalid ID" , { {"Member Account", Invalid_Value { input, "does not belong to a member account" }}}));
        return;
    }
    if (std::get<Member>(member_acct.value().type()).status() == Account_Status::Suspended)
    {
        error.emplace(chocan_user_exception("Member account is suspended", {}));
        return;
    }
}

void Transaction_Builder::set_provider_acct_field(const std::string& input)
{
    try
    {
        provider_acct.emplace(db->get_provider_account(input).value());
    }
    catch(const std::bad_optional_access&)
    {
        error.emplace(chocan_user_exception("No provider account associated with ID", {}));
        return;
    }
}
void Transaction_Builder::set_provider_acct_field(const Account& account)
{
    if(!provider_acct) { provider_acct.emplace(account); }
    if(std::holds_alternative<Set_Provider_Acct>(state)) { state = Set_Member_Acct(); }
}

void Transaction_Builder::set_service_date_field(const std::string& input)
{
    try
    {
        DateTime date = Parsers::parse_date(input, "MM-DD-YYYY", "-");

        if(date > DateTime::get_current_datetime())
        {
            error.emplace(invalid_datetime("Invalid Datetime", {{"Service Date", Invalid_Value{"", "cannot be future dated"}}}));
            return;
        }
    
        service_date.emplace(date);
    }
    catch(const invalid_datetime& err)
    {
        error.emplace(err);
        return;
    }
}

void Transaction_Builder::set_service_field(const std::string& input)
{
    try
    {
        service.emplace(db->lookup_service(input).value());
    }
    catch(const std::bad_optional_access&)
    {
        error.emplace(chocan_user_exception("Invalid Service", {{"Code", Invalid_Value{input, "is not associated with a service"}}}));
    }
}

void Transaction_Builder::set_comments_field(const std::string& input)
{
    if (input.length() > 100)
    {
        error.emplace(chocan_user_exception("Comments must be less than 100 characters", {}));
        return;
    }
    comments.emplace(input);
}
