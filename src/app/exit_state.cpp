/* 
 
File: exit_state.cpp

Brief: Implements exit state behavior

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
 
*/

#include <ChocAn/app/states/exit_state.hpp>

State::State_Ptr Exit_State::evaluate(const Input_Vector& input)
{
    // TODO the current loop in Main does not allow for exit state evaluation
    if( chocan && input.size() >= 0 )
    {
        chocan->login_manager.logout();
    }
    return std::make_unique<Exit_State>();
}

State_Info Exit_State::info() const
{
    return State_Info();
}