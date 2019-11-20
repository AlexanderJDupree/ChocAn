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
#include <sstream>
#include <functional>
#include <iostream>
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

std::string Transaction_Builder::get_current_field() const
{
    return std::visit( overloaded {
        [](const Set_Service_Date&){ return "Service Date (MM-DD-YYYY)"; },
        [](const Set_Provider_Acct&){ return "Provider ID"; },
        [](const Set_Member_Acct&){ return "Member ID"; },
        [](const Set_Service&){ return "Service Code"; },
        [](const Set_Comments&){ return "Comments"; }
    }, state);
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
        error.emplace(chocan_user_exception("No member account associated with ID", {}));
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

void Transaction_Builder::set_service_date_field(const std::string& input)
{
    std::string token;
    std::vector<unsigned> tokens;
    std::stringstream stream(input);

    // Parse Datetime
    while(std::getline(stream, token, '-'))
    {
        try
        {
            tokens.push_back(std::stoi(token));
        }
        catch(const std::exception&)
        {
            error.emplace(invalid_datetime("Unrecognized Datetime format", {}));
            return;
        }
    }
    if(tokens.size() != 3)
    {
        error.emplace(invalid_datetime("Invalid Datetime", {}));
        return;
    }

    try
    {
        DateTime date(Month{ tokens[0] }, Day{ tokens[1] }, Year{ tokens[2] });

        if(date > DateTime::get_current_datetime())
        {
            error.emplace(invalid_datetime("Service Date cannot be future dated", {}));
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
        error.emplace(chocan_user_exception("No service associated with service code", {}));
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
