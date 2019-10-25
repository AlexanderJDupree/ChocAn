/* 
 
File: state_controller.hpp

Brief: State Controller models user interaction with the application as a 
       Finte State Machine. State Controller must be provided with an initial 
       state and an end state. Currently, controller can't enforce that there
       must be a path to an end state, and is dependent on the programmer to 
       verify that there exists a path to an end state. 

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#ifndef  CHOCAN_STATE_CONTROLLER_H
#define  CHOCAN_STATE_CONTROLLER_H

#include <set>
#include <algorithm>
#include <ChocAn/app/state.hpp>
#include <ChocAn/app/states/login_state.hpp>
#include <ChocAn/app/states/exit_state.hpp>

class State_Controller
{
public:

    typedef State::State_Ptr State_Ptr;
    typedef std::set<State_Ptr> State_Set;

    State_Controller( State_Ptr initial_state = std::make_unique<Login_State>()
                    , State_Set&& end_states  = { std::make_unique<Exit_State>() } )
        : state(std::move(initial_state)), end_states(std::move(end_states)) {}

    State_Controller& transition(const std::string& input)
    {
        state = state->evaluate(input);
        return *this;
    }

    // Use optional?? Is there any scenario where state is null?
    const State& current_state() const
    {
        return *state;
    }

    bool end_state() const
    {
        State& current = *state;

        return end_states.end() != std::find_if(end_states.begin(), end_states.end(), 
                                   [&current](const State_Ptr& accept)
                                   { 
                                       return *accept == current; 
                                   } );
    }

private:

    State_Ptr state;
    State_Set end_states;
};

#endif // CHOCAN_STATE_CONTROLLER_H
