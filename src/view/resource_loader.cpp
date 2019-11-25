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
Resource_Loader::Resource_Table Resource_Loader::operator()(const Find_Account_MM&)
{
    return { { "state_name", [&](){ return "Find Account"; } } };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Find_Account_PM&)
{
    return { { "state_name", [&](){ return "Find Account"; } } };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const View_Account& state)
{  
    return
    {
        { "state_name",   [&](){ return "View Account"; } },
        { "account.id",   [&](){ return std::to_string(state.account.id()); } },
        { "account.type", [&]()
            { 
                return std::visit( overloaded {
                    [](const Member&)   { return  "Member"; },
                    [](const Manager&)  { return "Manager"; },
                    [](const Provider&) { return  "Provider"; }
                }, state.account.type() );
            } },
        { "account.name.last",  [&](){ return state.account.name().last();  } },
        { "account.name.first", [&](){ return state.account.name().first(); } },

        { "account.address.street", [&](){ return state.account.address().street(); } },
        { "account.address.city",   [&](){ return state.account.address().city();   } },
        { "account.address.state",  [&](){ return state.account.address().state();  } },
        { "account.address.zip",    [&](){ return std::to_string(state.account.address().zip()); } }
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
