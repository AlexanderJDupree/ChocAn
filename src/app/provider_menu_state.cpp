/* 
 
File: provider_menu_state.cpp

Brief: Implements provider menu state behavior

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
#include <ChocAn/app/states/provider_menu_state.hpp>

State::State_Ptr Provider_Menu_State::evaluate(const std::string& input)
{
    if(input == "0")
    {
        return std::make_unique<Login_State>();
    }
    if(input == "exit")
    {
        return std::make_unique<Exit_State>();
    }
    return std::make_unique<Provider_Menu_State>();
}

State_Info Provider_Menu_State::info() const
{
    return State_Info();
}