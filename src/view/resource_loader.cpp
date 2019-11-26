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

        { "builder.current_field", transaction.builder->get_current_field() }
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
        { "status", state.status}
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
            default : return "Press 'Enter' to continue:";
        };
    }() });
    return table;
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Generate_Report& state)
{
    return 
    {
        { "state_name", "Report Generator" },
        { "errors", render_user_error(state.error) },
        { "status", (state.date_range.empty()) ? "Enter report starting date (" + state.date_structure + "):" 
                                               : "Enter report ending date (" + state.date_structure + "):" }
    };
}

std::string Resource_Loader::render_user_error(const std::optional<chocan_user_exception>& maybe_err)
{
    if(!maybe_err) { return ""; }

    std::stringstream stream;
    stream << maybe_err.value().what() << "\n";
    for (const std::string& item : maybe_err.value().info())
    {
        stream << '\t' << item << '\n';
    }
    return stream.str();
}
