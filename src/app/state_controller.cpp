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
    runtime.push(std::visit(*this, runtime.top()));

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
                return Manager_Menu { "Welcome " + session_owner.name().first() };
            },
            [&](const Provider&) -> Application_State {
                return Provider_Menu { "Welcome " + session_owner.name().first() };
            },
            [&](const Member&) -> Application_State { 
                return Login { "Only Providers or Managers may log in"};
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
    state_viewer->render_state(menu) ;

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

Application_State State_Controller::operator()(Manager_Menu& menu)
{
    state_viewer->render_state(menu) ;

    const Transition_Table manager_menu
    {
        { "exit", [&](){ return Exit();  } },
        { "0"   , [&](){ chocan->login_manager.logout(); return Login(); } }
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
    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

    if(input == "exit")   { return Exit(); }
    if(input == "cancel") { return Provider_Menu { "Transaction Request Cancelled!" }; }
    // TODO allow user to print out provider directory

    chocan->transaction_builder.set_current_field(input);

    // If we can build the transaction then we're finished, otherwise loop
    return (chocan->transaction_builder.buildable()) 
           ? Confirm_Transaction { chocan->transaction_builder.build() }
           : Application_State   { state };
}

Application_State State_Controller::operator()(Confirm_Transaction& state)
{
    std::string input;
    state_viewer->render_state(state, [&]()
    {
        input = input_controller->read_input();
    } ) ;

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


