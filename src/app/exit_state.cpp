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

State::State_Ptr Exit_State::evaluate(const std::string& input)
{
    if(input == "test")
    {
        return std::make_unique<Exit_State>();
    }
    return std::make_unique<Exit_State>();
}
