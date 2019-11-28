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

        { "builder.user_error", [&]()
            {
                if(auto maybe_error = transaction.builder->get_last_error())
                {
                    return render_user_error(maybe_error.value());
                } 
                return std::string("");
            }()
        },

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

Resource_Loader::Resource_Table Resource_Loader::operator()(const Create_Account& state)
{

    return 
    {
        {"state_name", "Create Account"},
    
        {"builder.status", state.builder->get_status()},
   
        {"builder.issues", [&]() {
             if (std::optional<const chocan_user_exception> issues = state.builder->get_issues())
             {
                return render_user_error(issues.value());

             }
             return std::string("");
         }()
        }

    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Get_Type&){
    
    return
    {
        {"state_name", "Get_Type"}
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Get_First&){
    
    return
    {
        {"state_name", "Get_First"}
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Get_Last&){
    
    return
    {
        {"state_name", "Get_Last"}
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Get_Street&){
    
    return
    {
        {"state_name", "Get_Street"}
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Get_City&){
    
    return
    {
        {"state_name", "Get_City"}
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Get_State&){
    
    return
    {
        {"state_name", "Get_State"}
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Get_Zip&){
    
    return
    {
        {"state_name", "Get_Zip"}
    };
}

std::string Resource_Loader::render_user_error(const chocan_user_exception& err)
{
    std::stringstream stream;
    stream << err.what() << "\n";
    for (const std::string& item : err.info())
    {
        stream << '\t' << item << '\n';
    }
    return stream.str();
}
