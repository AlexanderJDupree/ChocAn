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

#include <ChocAn/app/state_controller.hpp>
#include <ChocAn/core/utils/transaction_builder.hpp>

State_Controller::State_Controller( ChocAn_Ptr        chocan
                                  , State_Viewer_Ptr  state_viewer
                                  , Input_Control_Ptr input_controller
                                  , Application_State initial_state )
    : chocan           ( chocan           )
    , state_viewer     ( state_viewer     ) 
    , input_controller ( input_controller )
    , state            ( initial_state )
    { 
        if (!(chocan && state_viewer && input_controller))
        {
            // TODO make exception verbose
            throw std::exception();
        }
    }

State_Controller& State_Controller::interact()
{
    state_viewer->render_state(state);

    return transition();
}

State_Controller& State_Controller::transition()
{
    state = std::visit(*this, state);

    return *this;
}

const Application_State& State_Controller::current_state() const
{
    return state;
}

bool State_Controller::end_state() const
{
    // Tests if current state is the exit state
    return std::holds_alternative<Exit>(state);
}

Application_State State_Controller::operator()(const Login&)
{
    std::string input = input_controller->read_input();

    if(input == "exit") { return Exit(); }

    if(chocan->login_manager.login(input))
    {
        Account session_owner = *chocan->login_manager.session_owner();
        if(std::holds_alternative<Manager>(session_owner.type()))
        {
            return Manager_Menu { "Welcome, " + session_owner.name().first() };
        }
        return Provider_Menu { "Welcome, " + session_owner.name().first() };
    }
    return Login { "Invalid Login" };
}

Application_State State_Controller::operator()(const Exit&)
{
    // logout is idempotent
    chocan->login_manager.logout();
    return Exit();
}

Application_State State_Controller::operator()(const Provider_Menu&)
{
    const Transition_Table provider_menu
    {
        { "exit", [&](){ return Exit();  } },
        { "0"   , [&](){ chocan->login_manager.logout(); return Login(); } },
        { "5"   , [&](){ return Add_Transaction{ &chocan->transaction_builder.reset() }; } }
    };

    try
    {
        return provider_menu.at(input_controller->read_input())();
    }
    catch(const std::out_of_range& err)
    {
        return Provider_Menu { "Unrecognized Input" };
    }
}

Application_State State_Controller::operator()(const Manager_Menu&)
{
    const Transition_Table manager_menu
    {
        { "exit", [&](){ return Exit();  } },
        { "0"   , [&](){ chocan->login_manager.logout(); return Login(); } },
        { "2"   , [&](){ return Create_Account{ &chocan->account_builder.reset()}; } }
    };

    try
    {
        return manager_menu.at(input_controller->read_input())();
    }
    catch(const std::out_of_range& err)
    {
        return Manager_Menu { "Unrecognized Input" };
    }
}

Application_State State_Controller::operator()(Add_Transaction& state)
{
    std::string input = input_controller->read_input();

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return Provider_Menu { "Transaction Request Cancelled!" }; }
    // TODO allow user to print out provider directory

    chocan->transaction_builder.set_current_field(input);

    // If we can build the transaction then we're finished, otherwise loop
    return (chocan->transaction_builder.buildable()) 
           ? Confirm_Transaction { chocan->transaction_builder.build() }
           : Application_State   { state };
}

Application_State State_Controller::operator()(const Confirm_Transaction& state)
{
    std::string input = input_controller->read_input();

    if (input == "y" || input == "yes"  || input == "Y" || input == "YES" )
    {
        chocan->db->add_transaction(state.transaction);
        return Provider_Menu { "Transaction Processed!" };
    }
    if (input == "n" || input == "no" || input == "N" || input == "NO")
    {
        return Add_Transaction { &chocan->transaction_builder.reset() };
    }
    return state;
}

Application_State State_Controller::operator()(const Create_Account& state)
{
    std::string input = input_controller->read_input();
    std::string issues;

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return Manager_Menu{ "Account Not Created" }; }

    chocan->account_builder.set_current_field(input);

    if(chocan->account_builder.buildable()){

        chocan->db->create_account(chocan->account_builder.build());
        return Manager_Menu{ "Account Successfully Created"};
    }

    return Application_State{ state };
}
