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

State_Controller::State_Controller( ChocAn_Ptr chocan
                                  , State_Viewer_Ptr  state_viewer
                                  , Input_Control_Ptr input_controller
                                  , Application_State initial_state )
    : chocan           ( chocan           )
    , state_viewer     ( state_viewer     ) 
    , input_controller ( input_controller )
    , state            ( initial_state    )
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
        if(std::holds_alternative<Manager>(session_owner.type))
        {
            return Manager_Menu { "Welcome, " + session_owner.name.first };
        }
        return Provider_Menu { "Welcome, " + session_owner.name.first };
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
        { "5"   , [&](){ return Add_Transaction(); } }
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

Application_State State_Controller::operator()(const Add_Transaction&)
{
    std::optional<Account> maybe_member = chocan->db->get_account(input_controller->read_input());

    if(!maybe_member) { return Add_Transaction { "Invalid ID" }; }

    if(!std::holds_alternative<Member>(maybe_member.value().type))
    {
        return Add_Transaction { "Non-Member account ID was entered" };
    }
    if(std::get<Member>(maybe_member.value().type).status() == Account_Status::Suspended)
    {
        return Add_Transaction { "Member Account is Suspended" };
    }

    Input_Controller::Form_Data transaction_form = input_controller->read_form
    ( { "Date of Provided Service (MM-DD-YYYY)"
      , "Service Code"
      , "Comments" }

    , [&](const std::string& field)
      {
          return state_viewer->render_prompt("Enter " + field + ": ");
      }
    );

    return Provider_Menu();
}