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
#include <ChocAn/app/application_state.hpp>
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
        { "1"   , [&](){ 
            return View_Account { chocan->db->get_account(chocan->login_manager.session_owner().id()).value() }; } },
        { "2"   , [&](){ 
            return Update_Account { chocan->login_manager.session_owner() 
                                  , &chocan->account_builder.reset() }; } 
        },
        { "3"   , [&](){ return Find_Account{ Find_Account::Next::Update_Account }; } },
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
        { "exit", [&](){ return Exit();  } },
        { "0"   , [&](){ chocan->login_manager.logout(); return Login(); } },
        { "1"   , [&](){ return Find_Account(); } },
        { "2"   , [&](){ return Create_Account{ 
            &chocan->account_builder.initiate_new_build_process() }; } 
        },
        { "3"   , [&](){ return Find_Account { Find_Account::Next::Delete_Account }; }},
        { "4"   , [&](){ return Generate_Report(); } },
        { "5"   , [&](){ return Find_Account { Find_Account::Next::Gen_Provider_Report }; }},
        { "6"   , [&](){ return Find_Account { Find_Account::Next::Gen_Member_Report }; }},
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

Application_State State_Controller::operator()(Create_Account& state)
{
    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return Manager_Menu {{ "Account Not Created" }}; }

    state.builder->set_field(input);
    
    state_viewer->render_state(state);

    if(!state.builder->buildable()) { return state; }

    Account temp_account = chocan->account_builder.build_new_account(chocan->db);

    std::optional<bool> confirmed;
    while(!confirmed)
    {
        View_Account view { temp_account, View_Account::Status::Confirm_Creation };
        state_viewer->render_state(view, [&]()
        {
            confirmed = input_controller->confirm_input();
        } ) ;
    }

    // User said yes
    if(confirmed.value())
    {
        chocan->db->create_account(temp_account);
        return Manager_Menu{{ "Account created! ID: " + std::to_string(temp_account.id()) }};
    }

    // User said no, restart account build
    state.builder->initiate_new_build_process();
    return state;
}

Application_State State_Controller::operator()(View_Account& state)
{
    std::string input;

    state_viewer->render_state(state, [&](){
        input = input_controller->read_input();
    }) ;
    
    return pop_runtime();
}

Application_State State_Controller::operator()(Delete_Account& state)
{
    std::optional<bool> confirmed;
    while(!confirmed)
    {
        //render the view account state with a confirm deletion prompt
        View_Account view_account { state.account, View_Account::Status::Confirm_Deletion };
        state_viewer->render_state(view_account, [&](){
            confirmed = input_controller->confirm_input();
        }) ;
    }

    //the user entered yes
    if(confirmed.value())
    {
        chocan->db->delete_account(state.account.id());
        return Manager_Menu {{ "Account deleted!" }};
    }

    //the user entered no
    return Manager_Menu {{ "Account not deleted" }};
}

Application_State State_Controller::operator()(Find_Account& state)
{
    using Get_Account_Function = std::function<std::optional<Account>(const std::string&)>;

    std::string err_msg;
    auto get_account = [&]() -> Get_Account_Function {

        if(state.next == Find_Account::Next::Gen_Member_Report || std::holds_alternative<Provider>(chocan->login_manager.session_owner().type()))
        {
            err_msg = "Error, not a ChocAn Member ID: ";
            return [&](const std::string& id) { return chocan->db->get_member_account(id); };
        }
        else if(state.next == Find_Account::Next::Gen_Provider_Report)
        {
            err_msg = "Error, not a ChocAn Provider ID: ";
            return [&](const std::string& id) { return chocan->db->get_provider_account(id); };
        }
        else
        {
            err_msg = "Error, not a ChocAn ID: ";
            return [&](const std::string& id) { return chocan->db->get_account(id);};
        }
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
        case Find_Account::Next::Update_Account : 
            return Update_Account{ maybe_account.value()        
                                 , &chocan->account_builder.reset() };
        case Find_Account::Next::Delete_Account : 
            return Delete_Account { maybe_account.value() };
        case Find_Account::Next::Gen_Member_Report : 
            return Generate_Report { Generate_Report::Report_Type::Member, maybe_account };
        case Find_Account::Next::Gen_Provider_Report : 
            return Generate_Report { Generate_Report::Report_Type::Provider, maybe_account };
        default: return View_Account { maybe_account.value() };
        }
    }
    state.status = err_msg + input;
    return state;
}

Application_State State_Controller::operator()(Generate_Report& state)
{
    // We have enough dates to generate a report
    if(state.date_range.size() == 2) 
    { 
        switch (state.type)
        {
        case Generate_Report::Report_Type::Member :
            return View_Report { 
                chocan->reporter.gen_member_report(state.date_range[0], state.date_range[1], state.account.value()) 
            }; 
        case Generate_Report::Report_Type::Provider :
            return View_Report { 
                chocan->reporter.gen_provider_report(state.date_range[0], state.date_range[1], state.account.value()) 
            }; 
        default: // Summary Report
            return View_Report { 
                chocan->reporter.gen_summary_report(state.date_range[0], state.date_range[1]) 
            }; 
        }
        return View_Report { 
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

Application_State State_Controller::operator()(View_Report& state)
{
    state_viewer->render_state(state, [&](){
        input_controller->read_input();
    }) ;
    return pop_runtime();
}

Application_State State_Controller::operator()(Update_Account& state)
{
    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return pop_runtime(); }

    if(state.status == Update_Account::Status::Choose)
    {
        state.msg.clear();
        if(input == "name")
        {
            state.builder->request_update_to_account( { Account_Builder::Last()
                                                      , Account_Builder::First() } );

            state.status = Update_Account::Status::Update_Field;
        }
        else if(input == "address")
        {
            state.builder->request_update_to_account( { Account_Builder::Zip()
                                                      , Account_Builder::State()
                                                      , Account_Builder::City()
                                                      , Account_Builder::Street() } );

            state.status = Update_Account::Status::Update_Field;
        }
        else
        {
            state.msg = "Unrecognized input";
        }
        return state;
    }
    if(state.status == Update_Account::Status::Update_Field)
    {
        state.builder->set_field(input);

        if(state.builder->buildable())
        {
            state.builder->apply_updates_to_account(state.account);
            state.status = Update_Account::Status::Confirm;
        }
    }
    if(state.status == Update_Account::Status::Confirm)
    {
        std::optional<bool> confirmed;
        while(!confirmed)
        {
            View_Account view { state.account, View_Account::Status::Confirm_Update };
            state_viewer->render_state(view, [&]()
            {
                confirmed = input_controller->confirm_input();
            } ) ;
        }

        if(confirmed.value())
        {
            state.builder->reset();
            state.status = Update_Account::Status::Choose;
        }
        else
        {
            if(chocan->db->update_account(state.account))
            {
                return Provider_Menu {{ "Account Updated!" }};
            }
            return Provider_Menu {{ "Account Failed to update." }};
        }
    }
    return state;
}
