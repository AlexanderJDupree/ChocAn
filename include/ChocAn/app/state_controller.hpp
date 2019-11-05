/* 
 
File: state_controller.hpp

Brief: State Controller models user interaction with the application as a 
       Finte State Machine. State Controller must be provided with an initial 
       state and an end state. Currently, controller can't enforce that there
       must be a path to an end state, and is dependent on the programmer to 
       verify that there exists a path to an end state. 

Authors: Daniel Mendez 
         Alex Salazar
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

    using State_Ptr  = State::State_Ptr;
    using ChocAn_Ptr = ChocAn::ChocAn_Ptr;

    typedef std::set<size_t> End_State_Set;

    // TODO set default chocan instance
    State_Controller( ChocAn_Ptr chocan
                    , State_Ptr initial_state = std::make_unique<Login_State>()
                    , End_State_Set&&  end_states    = { Exit_State().id() } )
        : state          ( std::move(initial_state) )
        , end_states     ( std::move(end_states)    ) 
        , chocan_service ( std::move(chocan)        )
        { state->set_service_instance(chocan_service); }
        // TODO throw if null

    State_Controller& transition(const State::Input_Vector& input)
    {
        state = state->evaluate(input);

        //TODO review how we inject state dependencies
        state->set_service_instance(chocan_service);
        return *this;
    }

    // Use optional?? Is there any scenario where state is null?
    const State& current_state() const
    {
        return *state;
    }

    bool end_state() const
    {
        return end_states.find(state->id()) != end_states.end();
    }

private:

    State_Ptr     state;
    End_State_Set end_states;
    ChocAn_Ptr    chocan_service;
};

#endif // CHOCAN_STATE_CONTROLLER_H
