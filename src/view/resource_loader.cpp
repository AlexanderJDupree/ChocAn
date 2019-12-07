/* 
File: resource_loader.hpp

Brief:  Resource Loader implementation

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <sstream>
#include <algorithm>
#include <ChocAn/view/resource_loader.hpp>
#include <ChocAn/core/utils/overloaded.hpp>

void Resource_Loader::update()
{
    if ( state ) table = std::visit(*this, *state);
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Exit&)
{

    return { { "state_name", "Exit" } };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Login& login)
{
    return 
    { 
        {"login_status", login.login_status },
        {"state_name"  , "Login Service"    }
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Provider_Menu& menu)
{
    return 
    { 
        { "state_name",  "Provider Menu" },
        { "menu.status", menu.status     } 
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Manager_Menu& menu)
{
    return 
    { 
        { "state_name", "Manager Menu" },
        { "menu.status", menu.status   } 
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Add_Transaction& transaction)
{
    return 
    { 
        { "state_name", "Transaction Manager" },

        { "builder.user_error", render_user_error(transaction.builder->get_last_error()) },

        { "builder.current_field", [&]() -> std::string
        {
            return std::visit( overloaded {
                [&](const Transaction_Builder::Set_Member_Acct)  { return "Enter Member ID:"; },
                [&](const Transaction_Builder::Set_Provider_Acct){ return "Enter Provider ID:"; },
                [&](const Transaction_Builder::Set_Service_Date) { return "Enter service date (MM-DD-YYYY):"; },
                [&](const Transaction_Builder::Set_Service)      { return "Enter Service Code:"; },
                [&](const Transaction_Builder::Set_Comments)     { return "Enter comments:"; }
            }, transaction.builder->builder_state() );
        }() }
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Confirm_Transaction& state)
{
    Resource_Table table = state.transaction.serialize();
    table.insert({ "state_name", "Confirm Transaction" });
    return table;
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Find_Account& state)
{
    return 
    { 
        { "state_name", "Find Account" },
        { "status", state.status},
        { "string_prompt", [&](){
            switch (state.next)
            {
            case Find_Account::Next::Delete_Account : return "Enter the ID of the account you wish to delete:";
            case Find_Account::Next::Update_Account : return "Enter the ID of the account you wish to update:";
            default: return "Enter the ID of the account you wish to view:";
            }
        }() }
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const View_Account& state)
{  
    Resource_Table table = state.account.serialize();
    table.insert({ "state_name", "View Account" });
    table.insert({ "state_status", [&](){
        switch(state.status)
        {
            case View_Account::Status::Confirm_Creation : return "Is this correct? (Y/N):";
            case View_Account::Status::Confirm_Deletion : return "Delete account? (Y/N):";
            case View_Account::Status::Confirm_Update   : return "Update another field? (Y/N):";
            default : return "Press 'Enter' to continue:";
        };
    }() });
    return table;
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Create_Account& state)
{

    return 
    {
        {"state_name", "Create Account"},
    
        {"builder.status", state.builder->get_status()},
   
        { "builder.errors", render_user_error(state.builder->get_errors()) },

        { "builder.current_field", render_builder_prompt(state.builder->builder_state()) }
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Update_Account& state)
{
    return
    {
        {"state_name", "Update Account" },
        
        { "state_msg", [&]() -> std::string {
            switch (state.status)
            {
            case Update_Account::Status::Update_Field : 
                return render_user_error(state.builder->get_errors());
            default: return state.msg;
            }
        }() },
        { "state_prompt", [&]() -> std::string {
            switch (state.status)
            {
            case Update_Account::Status::Choose : 
                return "Enter 'name' or 'address' to update the corresponding field:";
            case Update_Account::Status::Update_Field : 
                return render_builder_prompt(state.builder->builder_state());
            default: return {};
            }
        }() }
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Delete_Account&)
{
    return { };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Generate_Report& state)
{
    return 
    {
        { "state_name", "Report Generator" },
        { "errors", render_user_error(state.error) },
        { "status", (state.date_range.empty()) ? "Enter report start date (" + state.date_structure + "):" 
                                               : "Enter report end date (" + state.date_structure + "):" }
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const View_Report& state)
{
    return std::visit( overloaded {
        [&](const Summary_Report& report) -> Resource_Table
        {
            DateTime::Data_Table start = report.start_date().serialize();
            DateTime::Data_Table end   = report.end_date().serialize();
            return
            {
                { "state_name", "Summary Report" },
                { "summary_totals"   , render_summary(report) },
                { "provider_activity", render_provider_activity(report.activity()) },
                { "start_date"       , start["month"] + '/' + start["day"] + '/' + start["year"] },
                { "end_date"         , end["month"] + '/' + end["day"] + '/' + end["year"] }
            };
        },
        [&](const Provider_Report& report) -> Resource_Table
        {
            Resource_Table table = report.account().serialize();
            table.insert({ "state_name", "Account Report" });
            table.insert({ "account_activity", render_account_activity(report) });
            return table;
        },
        [&](const Member_Report& report) -> Resource_Table
        {

            Resource_Table table = report.account().serialize();
            table.insert({ "state_name", "Account Report" });
            table.insert({ "account_activity", render_account_activity(report) });
            return table;
        }
    }, state.report );
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const View_Service_Directory& state)
{
    return
    {
        { "state_name", "Service Directory" },
        { "directory", render_directory(state.db->service_directory()) }
    };
}

std::string Resource_Loader::render_directory(const Data_Gateway::Service_Directory& directory) const
{
    struct alphabetical_ordering
    {
        bool operator()(const Service& lhs, const Service& rhs) const
        {
            return lhs.name() < rhs.name();
        }
    };

    std::set<Service, alphabetical_ordering> services;
    std::for_each(directory.begin(), directory.end(), [&](const auto& entry)
    {
        services.insert(entry.second);
    } );

    std::string stream;
    for (const auto& service : services)
    {
        stream += ( '|' + center(service.name()) +
                    '|' + center(service.code()) + 
                    '|' + center('$' + service.cost().to_string()) + 
                    '|' + row_bar(3));
    }
    return stream;
}

std::string Resource_Loader::render_builder_prompt(Account_Builder::Build_State state) const
{
            return std::visit( overloaded {
                [&](const Account_Builder::Type)  { return "Enter Account Type: "; },
                [&](const Account_Builder::First) { return "Enter First Name: "; },
                [&](const Account_Builder::Last)  { return "Enter Last Name: "; },
                [&](const Account_Builder::Street){ return "Enter Street: "; },
                [&](const Account_Builder::City)  { return "Enter City: "; },
                [&](const Account_Builder::State) { return "Enter State: "; },
                [&](const Account_Builder::Zip)   { return "Enter Zip: "; },
                [&](const Account_Builder::Idle)  { return " "; }
            }, state );
}

std::string Resource_Loader::render_user_error(const std::optional<chocan_user_exception>& maybe_err) const
{
    if(!maybe_err) { return ""; }
    
    std::string stream = maybe_err.value().what();
    stream += ":";

    for (const auto& error : maybe_err.value().info())
    {
        stream += "\n\t" + error.first + ": ";
        stream += std::visit( overloaded {
            [](const Invalid_Range& err) 
            { 
                return "Must be between " + std::to_string(err.min) + '-' + std::to_string(err.max) 
                       + ", '"  + std::to_string(err.value) + "' was entered";
            },
            [](const Invalid_Length& err)
            {
                return "Must be between " + std::to_string(err.min) + '-' + std::to_string(err.max) 
                       + ", '" + err.value + "' is " + std::to_string(err.value.length()) + " characters";
            },
            [](const Invalid_Value& err)
            {
                return "Got [" + err.value + "], Expected [" + err.expected + ']';
            },
            [](const Failed_With& err)
            {
                return  err.value + ", " + err.reason;
            },
            [](const Incompatible_Values& err)
            {
                return err.val1 + " and " + err.val2 + " are incompatible values";
            }
        }, error.second);
    }
    return stream += '\n';
}

std::string Resource_Loader::render_account_activity(const Provider_Report& report) const
{
    std::string activity = "Activity for this period:\n";
    
    for ( const auto& transaction : report.transactions())
    {
        Transaction::Data_Table data = transaction.serialize();
        activity += ( "\n\nService Date: " + data["service_date_alt"] +
                      "\nFiled Date: " + data["filed_date_alt"] + 
                      "\n\tMember ID: " + data["member_id"] + 
                      "\n\tMember Name: " + data["member_name"] + 
                      "\n\tCost: " + data["service_cost"] + 
                      "\n\tService: " + data["service_name"] + " | " + data["service_code"]);
    }

    activity += "\n\nTotal Number of Consultations: " + std::to_string(report.services_rendered());
    activity += "\nTotal Fee: " + report.total_fee().to_string();
    return activity;
};

std::string Resource_Loader::render_account_activity(const Member_Report& report) const
{
    std::string activity = "Activity for this period:\n";
    
    for ( const auto& transaction : report.transactions())
    {
        Transaction::Data_Table data = transaction.serialize();
        activity += ( "\n\nService Date: " + data["service_date_alt"] +
                      "\n\tProvider ID: " + data["provider_id"] + 
                      "\n\tProvider Name: " + data["provider_name"] + 
                      "\n\tService: " + data["service_name"] + " | " + data["service_code"]);
    }
    return activity;
};

std::string Resource_Loader::render_provider_activity(const Provider_Activity& activity) const
{
    if(activity.empty())
    {
        return '|' + center("No Activity For This Period", 83) + '|' + row_bar(4);
    }

    std::string stream;
    for (const auto& report : activity)
    {
        std::string provider_name(report.account().name().last() + ", " + report.account().name().first()[0]);
        stream += ( '|' + center(report.account().id())      +
                    '|' + center(provider_name)              + 
                    '|' + center(report.services_rendered()) +
                    '|' + center('$' + report.total_fee().to_string()) + 
                    '|' + row_bar(4));
    }
    return stream;
}

std::string Resource_Loader::render_summary(const Summary_Report& report) const
{
    return '|' + center(report.num_providers()) +
           '|' + center(report.num_services())  + 
           '|' + center('$' + report.total_cost().to_string()) + 
           '|' + row_bar(3);
}

std::string Resource_Loader::center(unsigned num, unsigned width) const
{
    try
    {
        return center(std::to_string(num), width);
    }
    catch(const std::exception&)
    {
        return center("N/A", width);
    }
}
std::string Resource_Loader::center(const std::string& str, unsigned width) const
{
    if (str.length() >= width) { return str.substr(0, width); }

    unsigned pad = width - str.length();
    unsigned rpad = pad / 2;
    unsigned lpad = pad - rpad;

    return std::string(lpad, ' ') + str + std::string(rpad, ' ');
}

std::string Resource_Loader::row_bar(unsigned fields) const
{
    std::string bar("\n+");
    while (fields-- > 0)
    {
        bar += "--------------------+";
    }
    return bar += '\n';
}
