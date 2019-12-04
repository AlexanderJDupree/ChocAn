/* 
 
File: state_controller.hpp

Brief: state_controller implementation

Authors: Daniel Mendez 
         Alex Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn

 
*/

#include <functional>
#include <ChocAn/core/utils/parsers.hpp>
#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/core/utils/overloaded.hpp>
#include <ChocAn/core/utils/transaction_builder.hpp>

State_Controller::State_Controller( ChocAn_Ptr        chocan
                                  , State_Viewer_Ptr  state_viewer
                                  , Input_Control_Ptr input_controller
                                  , Application_State initial_state )
    : chocan           ( chocan            )
    , state_viewer     ( state_viewer      ) 
    , input_controller ( input_controller  )
    , runtime          ( { initial_state } )
    , is_end_state     ( false             )
    { 
        if (!(chocan && state_viewer && input_controller))
        {
            // TODO make exception verbose
            throw std::exception();
        }
    }

State_Controller& State_Controller::interact()
{
    // TODO implement runtime as ring buffer Or dequeue
    Application_State current_state = pop_runtime();
    runtime.push(std::visit(*this, current_state));

    return *this;
}

const Application_State& State_Controller::current_state() const
{
    return runtime.top();
}

bool State_Controller::end_state() const
{
    return is_end_state;
}

Application_State State_Controller::pop_runtime()
{
    Application_State temp = runtime.top();

    runtime.pop();

    return temp;
}

Application_State State_Controller::operator()(Login& login)
{
    std::string input;
    state_viewer->render_state(login, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    if(input == "exit") { return Exit(); }

    if(chocan->login_manager.login(input))
    {
        const Account& session_owner = chocan->login_manager.session_owner();
        return std::visit(overloaded {
            [&](const Manager&) -> Application_State { 
                return Manager_Menu {{ "Welcome " + session_owner.name().first() }};
            },
            [&](const Provider&) -> Application_State {
                return Provider_Menu {{ "Welcome " + session_owner.name().first() }};
            },
            [&](const Member&) -> Application_State { 
                return Login {{ "Only Providers or Managers may log in"}};
            }
        }, session_owner.type());
    }
    return Login { "Invalid login" };
}

Application_State State_Controller::operator()(Exit& exit)
{
    is_end_state = true;

    state_viewer->render_state(exit);

    // logout is idempotent
    chocan->login_manager.logout();

    return exit;
}

Application_State State_Controller::operator()(Provider_Menu& menu)
{
    runtime.push(menu);
    state_viewer->render_state(menu) ;

    const Transition_Table provider_menu
    {
        { "exit", [&](){ return Exit();  } },

        { "0"   , [&](){ chocan->login_manager.logout(); return Login(); } },

        { "1"   , [&](){ return View_Account {chocan->login_manager.session_owner() }; } },

        { "2"   , [&](){ return Update_Account { chocan->login_manager.session_owner() };  } },

        { "3"   , [&](){ return Find_Account { Find_Account::Next::Update_Account };  } },

        { "4"   , [&](){ return Find_Account(); } },

        { "5"   , [&](){ 
            chocan->transaction_builder.reset();
            chocan->transaction_builder.set_provider_acct_field(chocan->login_manager.session_owner());
            return Add_Transaction{ &chocan->transaction_builder }; } 
        }
    };

    try
    {
        return provider_menu.at(input_controller->read_input())();
    }
    catch(const std::out_of_range& err)
    {
        return Provider_Menu {{ "Unrecognized Input" }};
    }
}

Application_State State_Controller::operator()(Manager_Menu& menu)
{
    runtime.push(menu);
    state_viewer->render_state(menu) ;

    const Transition_Table manager_menu
    {
        { "0"   , [&](){ chocan->login_manager.logout(); return Login(); } },
        { "1"   , [&](){ return Find_Account(); } },
        { "2"   , [&](){ return Create_Account(); } },
        { "4"   , [&](){ return Generate_Report(); } },
        { "exit", [&](){ return Exit();  } }
    };

    try
    {
        return manager_menu.at(input_controller->read_input())();
    }
    catch(const std::out_of_range& err)
    {
        return Manager_Menu {{ "Unrecognized Input" }};
    }
}

Application_State State_Controller::operator()(Add_Transaction& state)
{
    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return Provider_Menu {{ "Transaction Request Cancelled!" }}; }
    if(input == "help")   { runtime.push(state); return View_Service_Directory { chocan->db }; }

    chocan->transaction_builder.set_current_field(input);

    // If we can build the transaction then we're finished, otherwise loop
    return (chocan->transaction_builder.buildable()) 
           ? Confirm_Transaction { chocan->transaction_builder.build() }
           : Application_State   { state };
}

Application_State State_Controller::operator()(View_Service_Directory& state)
{
    state_viewer->render_state(state, [&](){
        input_controller->read_input();
    }) ;
    return pop_runtime();
}

Application_State State_Controller::operator()(Confirm_Transaction& state)
{
    std::optional<bool> confirmed;
    while(!confirmed)
    {
        state_viewer->render_state(state, [&]()
        {
            confirmed = input_controller->confirm_input();
        } ) ;
    }

    if (confirmed.value())
    {
        unsigned id = chocan->db->add_transaction(state.transaction);
        std::string processed = "Transaction Processed, ID: " + std::to_string(id);
        return Provider_Menu {{ processed }};
    }
    else
    {
        return Add_Transaction { &chocan->transaction_builder.reset() };
    }
}

Application_State State_Controller::operator()(View_Account& state)
{
    state_viewer->render_state(state, [&](){
        input_controller->read_input();
    }) ;
    return pop_runtime();
}

Application_State State_Controller::operator()(Find_Account& state)
{
    using Get_Account_Function = std::function<std::optional<Account>(const std::string&)>;

    auto get_account = [&]() -> Get_Account_Function {
        if(std::holds_alternative<Manager>(chocan->login_manager.session_owner().type()))
        {
            return [&](const std::string& id) { return chocan->db->get_account(id);};
        }
        return [&](const std::string& id) { return chocan->db->get_member_account(id);};
    }();

    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return pop_runtime(); }

    if( auto maybe_account = get_account(input))
    {
        switch (state.next)
        {
        case Find_Account::Next::Delete_Account : void();
        case Find_Account::Next::Update_Account : return Update_Account{ maybe_account.value() };
        default: return View_Account { maybe_account.value() };
        }
    }
    state.status = "Invalid ID: " + input;
    return state;
}

Application_State State_Controller::operator()(Update_Account& state)
{
    std::string input;
    if (state.status != Update_Account::Status::Confirm)
    {
        state_viewer->render_state(state, [&]()
        {
            input = input_controller->read_input();
        } ) ;
    }

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return pop_runtime(); }

    std::map<Update_Account::Status, std::function<Application_State()>> transition_table
    {
        { Update_Account::Status::Choose, [&]() -> Application_State
        {
            if(input == "name")
            {
                state.msg.clear();
                state.builder = Account_Builder( Account_Builder::set_name_sequence);
                state.status = Update_Account::Status::Update_Field;
            }
            else if(input == "address")
            {
                state.msg.clear();
                state.builder = Account_Builder( Account_Builder::set_address_sequence);
                state.status = Update_Account::Status::Update_Field;
            }
            else
            {
                state.msg = "Unrecognized Input";
            }
            return state;
        } },

        { Update_Account::Status::Update_Field, [&]() -> Application_State
        {
            state.builder.set_current_field(input);

            if(state.builder.can_update())
            {
                state.builder.update(state.account);
                state.status = Update_Account::Status::Confirm;
            }
            return state;
        } },
        { Update_Account::Status::Confirm, [&]() -> Application_State
        {
            View_Account view_state { state.account, View_Account::Status::Confirm_Update };

            std::optional<bool> confirmed;
            while(!confirmed)
            {
                state_viewer->render_state(view_state, [&]()
                {
                    confirmed = input_controller->confirm_input();
                } ) ;
            }
            if(confirmed.value()) 
            { 
                state.msg.clear();
                state.status = Update_Account::Status::Choose;
                return state;
            }
            else
            {
                chocan->db->update_account(state.account);
                return Provider_Menu {{ "Account Updated!" }}; 
            }
        } },
    };

    return transition_table.at(state.status)();
}

Application_State State_Controller::operator()(Create_Account& state)
{
    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return Manager_Menu {{ "Account creation cancelled!" }}; }

    state.builder.set_current_field(input);

    if(state.builder.buildable())
    {
        Account account = state.builder.build(chocan->id_generator).value();

        View_Account view_state { account, View_Account::Status::Confirm_Creation };

        std::optional<bool> confirmed;
        while(!confirmed)
        {
            state_viewer->render_state(view_state, [&]()
            {
                confirmed = input_controller->confirm_input();
            } ) ;
        }
        if(confirmed.value()) 
        { 
            chocan->db->create_account(account);
            return Manager_Menu {{ "Account Created! ID: " + std::to_string(account.id()) }};
        }
        else
        {
            // Start Over
            return Create_Account();
        }
    }
    return state;
}

Application_State State_Controller::operator()(Generate_Report& state)
{
    // We have enough dates to generate a report
    if(state.date_range.size() == 2) 
    { 
        return View_Summary_Report { 
            chocan->reporter.gen_summary_report(state.date_range[0], state.date_range[1]) 
        }; 
    }

    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    state.error.reset();

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return pop_runtime(); }
    if(input == "all") 
    {
        state.date_range = { DateTime(0), DateTime::get_current_datetime() };
        return state;
    }

    try
    {
        state.date_range.push_back(Parsers::parse_date(input, state.date_structure, "-"));
    }
    catch(const invalid_datetime& err)
    {
        state.error.emplace(err);
    }
    return state;
}

Application_State State_Controller::operator()(View_Summary_Report& state)
{
    state_viewer->render_state(state, [&](){
        input_controller->read_input();
    }) ;
    return pop_runtime();
}