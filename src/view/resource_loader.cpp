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

Resource_Loader::Resource_Table Resource_Loader::operator()(const Exit&)
{
    return { { "state_name", [&](){ return "Exit"; } } };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Login& login)
{
    return 
    { 
        {"login_status", [&](){ return login.login_status; } },
        {"state_name"  , [&](){ return "Login Service";    } }
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Provider_Menu& menu)
{
    return 
    { 
        { "state_name", [&](){ return "Provider Menu"; } },
        { "menu.status"    , [&](){ return menu.status;     } } 
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Manager_Menu& menu)
{
    return 
    { 
        { "state_name", [&](){ return "Manager Menu"; } },
        { "menu.status"    , [&](){ return menu.status;     } } 
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Add_Transaction& transaction)
{
    return 
    { 
        { "state_name", [&](){ return "Transaction Manager"; } },

        { "builder.user_error", [&]()
            {
                if(auto maybe_error = transaction.builder->get_last_error())
                {
                    return render_user_error(maybe_error.value());
                } 
                return std::string("");
            } 
        },

        { "builder.current_field", [&](){ return transaction.builder->get_current_field(); }}
    };
}
Resource_Loader::Resource_Table Resource_Loader::operator()(const Confirm_Transaction& state)
{
    return 
    { 
        { "state_name", [&](){ return "Confirm Transaction"; } },
        { "comments",     [&](){ return state.transaction.comments(); } },
        { "service.name", [&](){ return state.transaction.service().name(); } },
        { "service.code", [&](){ return std::to_string(state.transaction.service().code()); } },
        { "member.id",    [&](){ return std::to_string(state.transaction.member().id());    } },
        { "provider.id",  [&](){ return std::to_string(state.transaction.provider().id());  } },
        { "service_date.day",   [&](){ return std::to_string(state.transaction.service_date().day().count()); } },
        { "service_date.year",  [&](){ return std::to_string(state.transaction.service_date().year().count()); } },
        { "service_date.month", [&](){ return std::to_string(state.transaction.service_date().month().count()); } },
        { "filed_date.day",   [&](){ return std::to_string(state.transaction.filed_date().day().count()); } },
        { "filed_date.year",  [&](){ return std::to_string(state.transaction.filed_date().year().count()); } },
        { "filed_date.month", [&](){ return std::to_string(state.transaction.filed_date().month().count()); } }
    };
}

Resource_Loader::Resource_Table Resource_Loader::operator()(const Create_Account& state)
{

    return
    {
        {"state_name", [&](){return "Create Account";} },
        {"builder.current_field", [&]() {return state.builder->get_current_field();}}
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
