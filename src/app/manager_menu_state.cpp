/* 
File: login_state.cpp

Brief: Implements login state behavior

Authors: Daniel Mendez 
         Alexander Salazar
         Arman Alauizadeh 
         Alexander DuPree
         Kyle Zalewski
         Dominique Moore

https://github.com/AlexanderJDupree/ChocAn
*/

#include <ChocAn/app/states/exit_state.hpp>
#include <ChocAn/app/states/login_state.hpp>
#include <ChocAn/app/states/manager_menu_state.hpp>

State::State_Ptr Manager_Menu_State::evaluate(const Input_Vector& input)
{
    if(input.at(0) == "exit")
    {
        return std::make_unique<Exit_State>();
    }
    if(input.at(0) == "0")
    {
        return std::make_unique<Login_State>();
    }
    return std::make_unique<Manager_Menu_State>();
}

State_Info Manager_Menu_State::info() const
{
    return State_Info();
}
